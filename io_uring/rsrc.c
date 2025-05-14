// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/nospec.h>
#include <linux/hugetlb.h>
#include <linux/compat.h>
#include <linux/io_uring.h>
#include <linux/io_uring/cmd.h>

#include <uapi/linux/io_uring.h>

#include "io_uring.h"
#include "openclose.h"
#include "rsrc.h"
#include "memmap.h"
#include "register.h"

struct io_rsrc_update {
	struct file			*file;
	u64				arg;
	u32				nr_args;
	u32				offset;
};

static struct io_rsrc_node *io_sqe_buffer_register(struct io_ring_ctx *ctx,
			struct iovec *iov, struct page **last_hpage);

/* only define max */
#define IORING_MAX_FIXED_FILES	(1U << 20)
#define IORING_MAX_REG_BUFFERS	(1U << 14)

#define IO_CACHED_BVECS_SEGS	32

/**
 * __io_account_mem - Accounts memory usage for a user.
 *
 * @user: Pointer to the user structure.
 * @nr_pages: Number of pages to account.
 *
 * This function checks if the user has sufficient memory quota to account
 * for the specified number of pages. If the quota is exceeded, it returns
 * an error. Otherwise, it updates the user's locked memory count.
 *
 * Returns:
 * - 0 on success.
 * - -ENOMEM if the memory limit is exceeded.
 */
int __io_account_mem(struct user_struct *user, unsigned long nr_pages)
{
	unsigned long page_limit, cur_pages, new_pages;

	if (!nr_pages)
		return 0;

	/* Don't allow more pages than we can safely lock */
	page_limit = rlimit(RLIMIT_MEMLOCK) >> PAGE_SHIFT;

	cur_pages = atomic_long_read(&user->locked_vm);
	do {
		new_pages = cur_pages + nr_pages;
		if (new_pages > page_limit)
			return -ENOMEM;
	} while (!atomic_long_try_cmpxchg(&user->locked_vm,
					  &cur_pages, new_pages));
	return 0;
}

/**
 * io_unaccount_mem - Unaccounts memory usage for a context.
 *
 * @ctx: Pointer to the io_ring_ctx structure.
 * @nr_pages: Number of pages to unaccount.
 *
 * This function decreases the memory usage count for the given context
 * and its associated user.
 */
static void io_unaccount_mem(struct io_ring_ctx *ctx, unsigned long nr_pages)
{
	if (ctx->user)
		__io_unaccount_mem(ctx->user, nr_pages);

	if (ctx->mm_account)
		atomic64_sub(nr_pages, &ctx->mm_account->pinned_vm);
}

/**
 * io_account_mem - Accounts memory usage for a context.
 *
 * @ctx: Pointer to the io_ring_ctx structure.
 * @nr_pages: Number of pages to account.
 *
 * This function accounts memory usage for the given context and its
 * associated user. It ensures that the memory usage does not exceed
 * the allowed limits.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
static int io_account_mem(struct io_ring_ctx *ctx, unsigned long nr_pages)
{
	int ret;

	if (ctx->user) {
		ret = __io_account_mem(ctx->user, nr_pages);
		if (ret)
			return ret;
	}

	if (ctx->mm_account)
		atomic64_add(nr_pages, &ctx->mm_account->pinned_vm);

	return 0;
}

/**
 * io_buffer_validate - Validates a user-provided buffer.
 *
 * @iov: Pointer to the iovec structure representing the buffer.
 *
 * This function checks if the provided buffer is valid, ensuring that
 * it meets the constraints for size, alignment, and address range.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on invalid input.
 */
int io_buffer_validate(struct iovec *iov)
{
	unsigned long tmp, acct_len = iov->iov_len + (PAGE_SIZE - 1);

	/*
	 * Don't impose further limits on the size and buffer
	 * constraints here, we'll -EINVAL later when IO is
	 * submitted if they are wrong.
	 */
	if (!iov->iov_base)
		return iov->iov_len ? -EFAULT : 0;
	if (!iov->iov_len)
		return -EFAULT;

	/* arbitrary limit, but we need something */
	if (iov->iov_len > SZ_1G)
		return -EFAULT;

	if (check_add_overflow((unsigned long)iov->iov_base, acct_len, &tmp))
		return -EOVERFLOW;

	return 0;
}

/**
 * io_release_ubuf - Releases user-mapped buffer pages.
 *
 * @priv: Pointer to the io_mapped_ubuf structure.
 *
 * This function unpins the pages associated with a user-mapped buffer,
 * releasing the resources held by the buffer.
 */
static void io_release_ubuf(void *priv)
{
	struct io_mapped_ubuf *imu = priv;
	unsigned int i;

	for (i = 0; i < imu->nr_bvecs; i++)
		unpin_user_page(imu->bvec[i].bv_page);
}

/**
 * io_alloc_imu - Allocates an io_mapped_ubuf structure.
 *
 * @ctx: Pointer to the io_ring_ctx structure.
 * @nr_bvecs: Number of bio_vec entries to allocate.
 *
 * This function allocates an io_mapped_ubuf structure with the specified
 * number of bio_vec entries. It uses a cache for small allocations and
 * falls back to dynamic allocation for larger sizes.
 *
 * Returns:
 * - Pointer to the allocated io_mapped_ubuf structure on success.
 * - NULL on failure.
 */
static struct io_mapped_ubuf *io_alloc_imu(struct io_ring_ctx *ctx,
					   int nr_bvecs)
{
	if (nr_bvecs <= IO_CACHED_BVECS_SEGS)
		return io_cache_alloc(&ctx->imu_cache, GFP_KERNEL);
	return kvmalloc(struct_size_t(struct io_mapped_ubuf, bvec, nr_bvecs),
			GFP_KERNEL);
}

/**
 * io_free_imu - Frees an io_mapped_ubuf structure.
 *
 * @ctx: Pointer to the io_ring_ctx structure.
 * @imu: Pointer to the io_mapped_ubuf structure to free.
 *
 * This function releases the resources associated with an io_mapped_ubuf
 * structure, including its bio_vec entries.
 */
static void io_free_imu(struct io_ring_ctx *ctx, struct io_mapped_ubuf *imu)
{
	if (imu->nr_bvecs <= IO_CACHED_BVECS_SEGS)
		io_cache_free(&ctx->imu_cache, imu);
	else
		kvfree(imu);
}

/**
 * io_buffer_unmap - Unmaps and releases a user-mapped buffer.
 *
 * @ctx: Pointer to the io_ring_ctx structure.
 * @imu: Pointer to the io_mapped_ubuf structure to unmap.
 *
 * This function unmaps a user-mapped buffer, releasing its resources
 * and decrementing its reference count. If the reference count reaches
 * zero, the buffer is freed.
 */
static void io_buffer_unmap(struct io_ring_ctx *ctx, struct io_mapped_ubuf *imu)
{
	if (!refcount_dec_and_test(&imu->refs))
		return;

	if (imu->acct_pages)
		io_unaccount_mem(ctx, imu->acct_pages);
	imu->release(imu->priv);
	io_free_imu(ctx, imu);
}

/**
 * io_rsrc_node_alloc - Allocates a resource node.
 *
 * @ctx: Pointer to the io_ring_ctx structure.
 * @type: Type of the resource node.
 *
 * This function allocates a resource node from the context's cache.
 * The node is initialized with the specified type and a reference count
 * of 1.
 *
 * Returns:
 * - Pointer to the allocated resource node on success.
 * - NULL on failure.
 */
struct io_rsrc_node *io_rsrc_node_alloc(struct io_ring_ctx *ctx, int type)
{
	struct io_rsrc_node *node;

	node = io_cache_alloc(&ctx->node_cache, GFP_KERNEL);
	if (node) {
		node->type = type;
		node->refs = 1;
		node->tag = 0;
		node->file_ptr = 0;
	}
	return node;
}

/**
 * io_rsrc_cache_init - Initializes resource caches for a context.
 *
 * @ctx: Pointer to the io_ring_ctx structure.
 *
 * This function initializes the caches for resource nodes and
 * io_mapped_ubuf structures in the given context.
 *
 * Returns:
 * - true on success.
 * - false on failure.
 */
bool io_rsrc_cache_init(struct io_ring_ctx *ctx)
{
	const int imu_cache_size = struct_size_t(struct io_mapped_ubuf, bvec,
						 IO_CACHED_BVECS_SEGS);
	const int node_size = sizeof(struct io_rsrc_node);
	bool ret;

	ret = io_alloc_cache_init(&ctx->node_cache, IO_ALLOC_CACHE_MAX,
				  node_size, 0);
	ret |= io_alloc_cache_init(&ctx->imu_cache, IO_ALLOC_CACHE_MAX,
				   imu_cache_size, 0);
	return ret;
}

/**
 * io_rsrc_cache_free - Frees resource caches for a context.
 *
 * @ctx: Pointer to the io_ring_ctx structure.
 *
 * This function releases the caches for resource nodes and
 * io_mapped_ubuf structures in the given context.
 */
void io_rsrc_cache_free(struct io_ring_ctx *ctx)
{
	io_alloc_cache_free(&ctx->node_cache, kfree);
	io_alloc_cache_free(&ctx->imu_cache, kfree);
}

/**
 * io_rsrc_data_free - Frees resource data for a context.
 *
 * @ctx: Pointer to the io_ring_ctx structure.
 * @data: Pointer to the io_rsrc_data structure to free.
 *
 * This function releases all resource nodes in the given resource data
 * structure and frees the memory allocated for the nodes array.
 */
__cold void io_rsrc_data_free(struct io_ring_ctx *ctx,
			      struct io_rsrc_data *data)
{
	if (!data->nr)
		return;
	while (data->nr--) {
		if (data->nodes[data->nr])
			io_put_rsrc_node(ctx, data->nodes[data->nr]);
	}
	kvfree(data->nodes);
	data->nodes = NULL;
	data->nr = 0;
}

/**
 * io_rsrc_data_alloc - Allocates resource data.
 *
 * @data: Pointer to the io_rsrc_data structure to initialize.
 * @nr: Number of resource nodes to allocate.
 *
 * This function allocates memory for the resource nodes array in the
 * given resource data structure.
 *
 * Returns:
 * - 0 on success.
 * - -ENOMEM on memory allocation failure.
 */
__cold int io_rsrc_data_alloc(struct io_rsrc_data *data, unsigned nr)
{
	data->nodes = kvmalloc_array(nr, sizeof(struct io_rsrc_node *),
					GFP_KERNEL_ACCOUNT | __GFP_ZERO);
	if (data->nodes) {
		data->nr = nr;
		return 0;
	}
	return -ENOMEM;
}

/**
 * __io_sqe_files_update - Updates registered files in the io_uring context.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 * @up: Pointer to the io_uring_rsrc_update2 structure containing update details.
 * @nr_args: Number of file descriptors to update.
 *
 * This function updates the registered files in the io_uring context based on
 * the provided update details. It validates the input parameters, retrieves
 * the new file descriptors, and replaces the existing ones in the file table.
 *
 * Returns:
 * - The number of successfully updated file descriptors on success.
 * - Negative error code on failure.
 */
static int __io_sqe_files_update(struct io_ring_ctx *ctx,
				 struct io_uring_rsrc_update2 *up,
				 unsigned nr_args)
{
	u64 __user *tags = u64_to_user_ptr(up->tags);
	__s32 __user *fds = u64_to_user_ptr(up->data);
	int fd, i, err = 0;
	unsigned int done;

	if (!ctx->file_table.data.nr)
		return -ENXIO;
	if (up->offset + nr_args > ctx->file_table.data.nr)
		return -EINVAL;

	for (done = 0; done < nr_args; done++) {
		u64 tag = 0;

		if ((tags && copy_from_user(&tag, &tags[done], sizeof(tag))) ||
		    copy_from_user(&fd, &fds[done], sizeof(fd))) {
			err = -EFAULT;
			break;
		}
		if ((fd == IORING_REGISTER_FILES_SKIP || fd == -1) && tag) {
			err = -EINVAL;
			break;
		}
		if (fd == IORING_REGISTER_FILES_SKIP)
			continue;

		i = up->offset + done;
		if (io_reset_rsrc_node(ctx, &ctx->file_table.data, i))
			io_file_bitmap_clear(&ctx->file_table, i);

		if (fd != -1) {
			struct file *file = fget(fd);
			struct io_rsrc_node *node;

			if (!file) {
				err = -EBADF;
				break;
			}
			/*
			 * Don't allow io_uring instances to be registered.
			 */
			if (io_is_uring_fops(file)) {
				fput(file);
				err = -EBADF;
				break;
			}
			node = io_rsrc_node_alloc(ctx, IORING_RSRC_FILE);
			if (!node) {
				err = -ENOMEM;
				fput(file);
				break;
			}
			ctx->file_table.data.nodes[i] = node;
			if (tag)
				node->tag = tag;
			io_fixed_file_set(node, file);
			io_file_bitmap_set(&ctx->file_table, i);
		}
	}
	return done ? done : err;
}

/**
 * __io_sqe_buffers_update - Updates registered buffers in the io_uring context.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 * @up: Pointer to the io_uring_rsrc_update2 structure containing update details.
 * @nr_args: Number of buffers to update.
 *
 * This function updates the registered buffers in the io_uring context based on
 * the provided update details. It validates the input parameters, retrieves
 * the new buffers, and replaces the existing ones in the buffer table.
 *
 * Returns:
 * - The number of successfully updated buffers on success.
 * - Negative error code on failure.
 */
static int __io_sqe_buffers_update(struct io_ring_ctx *ctx,
				   struct io_uring_rsrc_update2 *up,
				   unsigned int nr_args)
{
	u64 __user *tags = u64_to_user_ptr(up->tags);
	struct iovec fast_iov, *iov;
	struct page *last_hpage = NULL;
	struct iovec __user *uvec;
	u64 user_data = up->data;
	__u32 done;
	int i, err;

	if (!ctx->buf_table.nr)
		return -ENXIO;
	if (up->offset + nr_args > ctx->buf_table.nr)
		return -EINVAL;

	for (done = 0; done < nr_args; done++) {
		struct io_rsrc_node *node;
		u64 tag = 0;

		uvec = u64_to_user_ptr(user_data);
		iov = iovec_from_user(uvec, 1, 1, &fast_iov, ctx->compat);
		if (IS_ERR(iov)) {
			err = PTR_ERR(iov);
			break;
		}
		if (tags && copy_from_user(&tag, &tags[done], sizeof(tag))) {
			err = -EFAULT;
			break;
		}
		err = io_buffer_validate(iov);
		if (err)
			break;
		node = io_sqe_buffer_register(ctx, iov, &last_hpage);
		if (IS_ERR(node)) {
			err = PTR_ERR(node);
			break;
		}
		if (tag) {
			if (!node) {
				err = -EINVAL;
				break;
			}
			node->tag = tag;
		}
		i = array_index_nospec(up->offset + done, ctx->buf_table.nr);
		io_reset_rsrc_node(ctx, &ctx->buf_table, i);
		ctx->buf_table.nodes[i] = node;
		if (ctx->compat)
			user_data += sizeof(struct compat_iovec);
		else
			user_data += sizeof(struct iovec);
	}
	return done ? done : err;
}

/**
 * __io_register_rsrc_update - Handles resource updates for io_uring.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 * @type: The type of resource to update (e.g., files or buffers).
 * @up: Pointer to the io_uring_rsrc_update2 structure containing update details.
 * @nr_args: Number of resources to update.
 *
 * This function processes resource updates for io_uring, such as updating
 * registered files or buffers. It validates the input parameters and invokes
 * the appropriate update handler based on the resource type.
 *
 * Returns:
 * - The number of successfully updated resources on success.
 * - Negative error code on failure.
 */
static int __io_register_rsrc_update(struct io_ring_ctx *ctx, unsigned type,
				     struct io_uring_rsrc_update2 *up,
				     unsigned nr_args)
{
	__u32 tmp;

	lockdep_assert_held(&ctx->uring_lock);

	if (check_add_overflow(up->offset, nr_args, &tmp))
		return -EOVERFLOW;

	switch (type) {
	case IORING_RSRC_FILE:
		return __io_sqe_files_update(ctx, up, nr_args);
	case IORING_RSRC_BUFFER:
		return __io_sqe_buffers_update(ctx, up, nr_args);
	}
	return -EINVAL;
}

/**
 * io_register_files_update - Updates registered files in the io_uring context.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 * @arg: Pointer to the user-provided update data.
 * @nr_args: Number of file descriptors to update.
 *
 * This function updates the registered files in the io_uring context based on
 * the user-provided update data. It validates the input and invokes the
 * resource update handler for files.
 *
 * Returns:
 * - The number of successfully updated file descriptors on success.
 * - Negative error code on failure.
 */
int io_register_files_update(struct io_ring_ctx *ctx, void __user *arg,
			     unsigned nr_args)
{
	struct io_uring_rsrc_update2 up;

	if (!nr_args)
		return -EINVAL;
	memset(&up, 0, sizeof(up));
	if (copy_from_user(&up, arg, sizeof(struct io_uring_rsrc_update)))
		return -EFAULT;
	if (up.resv || up.resv2)
		return -EINVAL;
	return __io_register_rsrc_update(ctx, IORING_RSRC_FILE, &up, nr_args);
}

/**
 * io_register_rsrc_update - Updates registered resources in the io_uring context.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 * @arg: Pointer to the user-provided update data.
 * @size: Size of the update data structure.
 * @type: The type of resource to update (e.g., files or buffers).
 *
 * This function updates the registered resources in the io_uring context based
 * on the user-provided update data. It validates the input and invokes the
 * appropriate resource update handler.
 *
 * Returns:
 * - The number of successfully updated resources on success.
 * - Negative error code on failure.
 */
int io_register_rsrc_update(struct io_ring_ctx *ctx, void __user *arg,
			    unsigned size, unsigned type)
{
	struct io_uring_rsrc_update2 up;

	if (size != sizeof(up))
		return -EINVAL;
	if (copy_from_user(&up, arg, sizeof(up)))
		return -EFAULT;
	if (!up.nr || up.resv || up.resv2)
		return -EINVAL;
	return __io_register_rsrc_update(ctx, type, &up, up.nr);
}

/**
 * io_register_rsrc - Registers resources (files or buffers) for io_uring.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 * @arg: Pointer to the user-provided resource registration data.
 * @size: Size of the resource registration data structure.
 * @type: The type of resource to register (e.g., files or buffers).
 *
 * This function handles the registration of resources, such as files or buffers,
 * for use with io_uring. It validates the input parameters, allocates the necessary
 * resources, and updates the io_uring context with the registered resources.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
__cold int io_register_rsrc(struct io_ring_ctx *ctx, void __user *arg,
			    unsigned int size, unsigned int type)
{
	struct io_uring_rsrc_register rr;

	/* keep it extendible */
	if (size != sizeof(rr))
		return -EINVAL;

	memset(&rr, 0, sizeof(rr));
	if (copy_from_user(&rr, arg, size))
		return -EFAULT;
	if (!rr.nr || rr.resv2)
		return -EINVAL;
	if (rr.flags & ~IORING_RSRC_REGISTER_SPARSE)
		return -EINVAL;

	switch (type) {
	case IORING_RSRC_FILE:
		if (rr.flags & IORING_RSRC_REGISTER_SPARSE && rr.data)
			break;
		return io_sqe_files_register(ctx, u64_to_user_ptr(rr.data),
					     rr.nr, u64_to_user_ptr(rr.tags));
	case IORING_RSRC_BUFFER:
		if (rr.flags & IORING_RSRC_REGISTER_SPARSE && rr.data)
			break;
		return io_sqe_buffers_register(ctx, u64_to_user_ptr(rr.data),
					       rr.nr, u64_to_user_ptr(rr.tags));
	}
	return -EINVAL;
}

/**
 * io_files_update_prep - Prepares a file update request for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for updating files
 * in the io_uring context based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on invalid input.
 */
int io_files_update_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe)
{
	struct io_rsrc_update *up = io_kiocb_to_cmd(req, struct io_rsrc_update);

	if (unlikely(req->flags & (REQ_F_FIXED_FILE | REQ_F_BUFFER_SELECT)))
		return -EINVAL;
	if (sqe->rw_flags || sqe->splice_fd_in)
		return -EINVAL;

	up->offset = READ_ONCE(sqe->off);
	up->nr_args = READ_ONCE(sqe->len);
	if (!up->nr_args)
		return -EINVAL;
	up->arg = READ_ONCE(sqe->addr);
	return 0;
}

/**
 * io_files_update_with_index_alloc - Updates file descriptors with index allocation.
 *
 * @req: Pointer to the io_kiocb structure representing the I/O request.
 * @issue_flags: Flags indicating specific behaviors or conditions for the operation.
 *
 * This function is responsible for updating file descriptors in the io_uring
 * context, allocating indices as necessary. It ensures that the file descriptor
 * table is properly managed and updated to reflect the current state of the
 * operation.
 *
 * Returns:
 *   0 on success, or a negative error code on failure.
 */
static int io_files_update_with_index_alloc(struct io_kiocb *req,
					    unsigned int issue_flags)
{
	struct io_rsrc_update *up = io_kiocb_to_cmd(req, struct io_rsrc_update);
	__s32 __user *fds = u64_to_user_ptr(up->arg);
	unsigned int done;
	struct file *file;
	int ret, fd;

	if (!req->ctx->file_table.data.nr)
		return -ENXIO;

	for (done = 0; done < up->nr_args; done++) {
		if (copy_from_user(&fd, &fds[done], sizeof(fd))) {
			ret = -EFAULT;
			break;
		}

		file = fget(fd);
		if (!file) {
			ret = -EBADF;
			break;
		}
		ret = io_fixed_fd_install(req, issue_flags, file,
					  IORING_FILE_INDEX_ALLOC);
		if (ret < 0)
			break;
		if (copy_to_user(&fds[done], &ret, sizeof(ret))) {
			__io_close_fixed(req->ctx, issue_flags, ret);
			ret = -EFAULT;
			break;
		}
	}

	if (done)
		return done;
	return ret;
}

/**
 * io_files_update - Updates registered files in the io_uring context.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function updates the registered files in the io_uring context based
 * on the parameters provided in the request. It handles both direct updates
 * and updates with index allocation.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_files_update(struct io_kiocb *req, unsigned int issue_flags)
{
	struct io_rsrc_update *up = io_kiocb_to_cmd(req, struct io_rsrc_update);
	struct io_ring_ctx *ctx = req->ctx;
	struct io_uring_rsrc_update2 up2;
	int ret;

	up2.offset = up->offset;
	up2.data = up->arg;
	up2.nr = 0;
	up2.tags = 0;
	up2.resv = 0;
	up2.resv2 = 0;

	if (up->offset == IORING_FILE_INDEX_ALLOC) {
		ret = io_files_update_with_index_alloc(req, issue_flags);
	} else {
		io_ring_submit_lock(ctx, issue_flags);
		ret = __io_register_rsrc_update(ctx, IORING_RSRC_FILE,
						&up2, up->nr_args);
		io_ring_submit_unlock(ctx, issue_flags);
	}

	if (ret < 0)
		req_set_fail(req);
	io_req_set_res(req, ret, 0);
	return IOU_OK;
}

/**
 * io_free_rsrc_node - Frees a resource node in the io_uring context.
 * @ctx: Pointer to the io_uring context structure.
 * @node: Pointer to the resource node to be freed.
 *
 * This function is responsible for releasing the memory and resources
 * associated with a specific resource node in the io_uring context. It
 * ensures proper cleanup of the node to prevent memory leaks or dangling
 * pointers.
 */
void io_free_rsrc_node(struct io_ring_ctx *ctx, struct io_rsrc_node *node)
{
	if (node->tag)
		io_post_aux_cqe(ctx, node->tag, 0, 0);

	switch (node->type) {
	case IORING_RSRC_FILE:
		fput(io_slot_file(node));
		break;
	case IORING_RSRC_BUFFER:
		io_buffer_unmap(ctx, node->buf);
		break;
	default:
		WARN_ON_ONCE(1);
		break;
	}

	io_cache_free(&ctx->node_cache, node);
}

/**
 * io_sqe_files_unregister - Unregisters all files from the io_uring context.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 *
 * This function releases all file descriptors registered in the io_uring
 * context. It ensures that the file table is properly cleared and resources
 * are freed.
 *
 * Returns:
 * - 0 on success.
 * - -ENXIO if no files are registered.
 */
int io_sqe_files_unregister(struct io_ring_ctx *ctx)
{
	if (!ctx->file_table.data.nr)
		return -ENXIO;

	io_free_file_tables(ctx, &ctx->file_table);
	io_file_table_set_alloc_range(ctx, 0, 0);
	return 0;
}

/**
 * io_sqe_files_register - Registers files for io_uring.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 * @arg: Pointer to the user-provided file descriptor array.
 * @nr_args: Number of file descriptors to register.
 * @tags: Pointer to the user-provided tags array.
 *
 * This function registers an array of file descriptors for use with io_uring.
 * It validates the input, allocates resources, and updates the file table
 * in the io_uring context.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_sqe_files_register(struct io_ring_ctx *ctx, void __user *arg,
			  unsigned nr_args, u64 __user *tags)
{
	__s32 __user *fds = (__s32 __user *) arg;
	struct file *file;
	int fd, ret;
	unsigned i;

	if (ctx->file_table.data.nr)
		return -EBUSY;
	if (!nr_args)
		return -EINVAL;
	if (nr_args > IORING_MAX_FIXED_FILES)
		return -EMFILE;
	if (nr_args > rlimit(RLIMIT_NOFILE))
		return -EMFILE;
	if (!io_alloc_file_tables(ctx, &ctx->file_table, nr_args))
		return -ENOMEM;

	for (i = 0; i < nr_args; i++) {
		struct io_rsrc_node *node;
		u64 tag = 0;

		ret = -EFAULT;
		if (tags && copy_from_user(&tag, &tags[i], sizeof(tag)))
			goto fail;
		if (fds && copy_from_user(&fd, &fds[i], sizeof(fd)))
			goto fail;
		/* allow sparse sets */
		if (!fds || fd == -1) {
			ret = -EINVAL;
			if (tag)
				goto fail;
			continue;
		}

		file = fget(fd);
		ret = -EBADF;
		if (unlikely(!file))
			goto fail;

		/*
		 * Don't allow io_uring instances to be registered.
		 */
		if (io_is_uring_fops(file)) {
			fput(file);
			goto fail;
		}
		ret = -ENOMEM;
		node = io_rsrc_node_alloc(ctx, IORING_RSRC_FILE);
		if (!node) {
			fput(file);
			goto fail;
		}
		if (tag)
			node->tag = tag;
		ctx->file_table.data.nodes[i] = node;
		io_fixed_file_set(node, file);
		io_file_bitmap_set(&ctx->file_table, i);
	}

	/* default it to the whole table */
	io_file_table_set_alloc_range(ctx, 0, ctx->file_table.data.nr);
	return 0;
fail:
	io_sqe_files_unregister(ctx);
	return ret;
}

/**
 * io_sqe_buffers_unregister - Unregisters all buffers from the io_uring context.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 *
 * This function releases all buffers registered in the io_uring context.
 * It ensures that the buffer table is properly cleared and resources are freed.
 *
 * Returns:
 * - 0 on success.
 * - -ENXIO if no buffers are registered.
 */
int io_sqe_buffers_unregister(struct io_ring_ctx *ctx)
{
	if (!ctx->buf_table.nr)
		return -ENXIO;
	io_rsrc_data_free(ctx, &ctx->buf_table);
	return 0;
}

/*
 * Not super efficient, but this is just a registration time. And we do cache
 * the last compound head, so generally we'll only do a full search if we don't
 * match that one.
 *
 * We check if the given compound head page has already been accounted, to
 * avoid double accounting it. This allows us to account the full size of the
 * page, not just the constituent pages of a huge page.
 */
static bool headpage_already_acct(struct io_ring_ctx *ctx, struct page **pages,
				  int nr_pages, struct page *hpage)
{
	int i, j;

	/* check current page array */
	for (i = 0; i < nr_pages; i++) {
		if (!PageCompound(pages[i]))
			continue;
		if (compound_head(pages[i]) == hpage)
			return true;
	}

	/* check previously registered pages */
	for (i = 0; i < ctx->buf_table.nr; i++) {
		struct io_rsrc_node *node = ctx->buf_table.nodes[i];
		struct io_mapped_ubuf *imu;

		if (!node)
			continue;
		imu = node->buf;
		for (j = 0; j < imu->nr_bvecs; j++) {
			if (!PageCompound(imu->bvec[j].bv_page))
				continue;
			if (compound_head(imu->bvec[j].bv_page) == hpage)
				return true;
		}
	}

	return false;
}

static int io_buffer_account_pin(struct io_ring_ctx *ctx, struct page **pages,
				 int nr_pages, struct io_mapped_ubuf *imu,
				 struct page **last_hpage)
{
	int i, ret;

	imu->acct_pages = 0;
	for (i = 0; i < nr_pages; i++) {
		if (!PageCompound(pages[i])) {
			imu->acct_pages++;
		} else {
			struct page *hpage;

			hpage = compound_head(pages[i]);
			if (hpage == *last_hpage)
				continue;
			*last_hpage = hpage;
			if (headpage_already_acct(ctx, pages, i, hpage))
				continue;
			imu->acct_pages += page_size(hpage) >> PAGE_SHIFT;
		}
	}

	if (!imu->acct_pages)
		return 0;

	ret = io_account_mem(ctx, imu->acct_pages);
	if (ret)
		imu->acct_pages = 0;
	return ret;
}

static bool io_coalesce_buffer(struct page ***pages, int *nr_pages,
				struct io_imu_folio_data *data)
{
	struct page **page_array = *pages, **new_array = NULL;
	int nr_pages_left = *nr_pages, i, j;
	int nr_folios = data->nr_folios;

	/* Store head pages only*/
	new_array = kvmalloc_array(nr_folios, sizeof(struct page *),
					GFP_KERNEL);
	if (!new_array)
		return false;

	new_array[0] = compound_head(page_array[0]);
	/*
	 * The pages are bound to the folio, it doesn't
	 * actually unpin them but drops all but one reference,
	 * which is usually put down by io_buffer_unmap().
	 * Note, needs a better helper.
	 */
	if (data->nr_pages_head > 1)
		unpin_user_pages(&page_array[1], data->nr_pages_head - 1);

	j = data->nr_pages_head;
	nr_pages_left -= data->nr_pages_head;
	for (i = 1; i < nr_folios; i++) {
		unsigned int nr_unpin;

		new_array[i] = page_array[j];
		nr_unpin = min_t(unsigned int, nr_pages_left - 1,
					data->nr_pages_mid - 1);
		if (nr_unpin)
			unpin_user_pages(&page_array[j+1], nr_unpin);
		j += data->nr_pages_mid;
		nr_pages_left -= data->nr_pages_mid;
	}
	kvfree(page_array);
	*pages = new_array;
	*nr_pages = nr_folios;
	return true;
}

bool io_check_coalesce_buffer(struct page **page_array, int nr_pages,
			      struct io_imu_folio_data *data)
{
	struct folio *folio = page_folio(page_array[0]);
	unsigned int count = 1, nr_folios = 1;
	int i;

	data->nr_pages_mid = folio_nr_pages(folio);
	data->folio_shift = folio_shift(folio);

	/*
	 * Check if pages are contiguous inside a folio, and all folios have
	 * the same page count except for the head and tail.
	 */
	for (i = 1; i < nr_pages; i++) {
		if (page_folio(page_array[i]) == folio &&
			page_array[i] == page_array[i-1] + 1) {
			count++;
			continue;
		}

		if (nr_folios == 1) {
			if (folio_page_idx(folio, page_array[i-1]) !=
				data->nr_pages_mid - 1)
				return false;

			data->nr_pages_head = count;
		} else if (count != data->nr_pages_mid) {
			return false;
		}

		folio = page_folio(page_array[i]);
		if (folio_size(folio) != (1UL << data->folio_shift) ||
			folio_page_idx(folio, page_array[i]) != 0)
			return false;

		count = 1;
		nr_folios++;
	}
	if (nr_folios == 1)
		data->nr_pages_head = count;

	data->nr_folios = nr_folios;
	return true;
}

static struct io_rsrc_node *io_sqe_buffer_register(struct io_ring_ctx *ctx,
						   struct iovec *iov,
						   struct page **last_hpage)
{
	struct io_mapped_ubuf *imu = NULL;
	struct page **pages = NULL;
	struct io_rsrc_node *node;
	unsigned long off;
	size_t size;
	int ret, nr_pages, i;
	struct io_imu_folio_data data;
	bool coalesced = false;

	if (!iov->iov_base)
		return NULL;

	node = io_rsrc_node_alloc(ctx, IORING_RSRC_BUFFER);
	if (!node)
		return ERR_PTR(-ENOMEM);

	ret = -ENOMEM;
	pages = io_pin_pages((unsigned long) iov->iov_base, iov->iov_len,
				&nr_pages);
	if (IS_ERR(pages)) {
		ret = PTR_ERR(pages);
		pages = NULL;
		goto done;
	}

	/* If it's huge page(s), try to coalesce them into fewer bvec entries */
	if (nr_pages > 1 && io_check_coalesce_buffer(pages, nr_pages, &data)) {
		if (data.nr_pages_mid != 1)
			coalesced = io_coalesce_buffer(&pages, &nr_pages, &data);
	}

	imu = io_alloc_imu(ctx, nr_pages);
	if (!imu)
		goto done;

	imu->nr_bvecs = nr_pages;
	ret = io_buffer_account_pin(ctx, pages, nr_pages, imu, last_hpage);
	if (ret) {
		unpin_user_pages(pages, nr_pages);
		goto done;
	}

	size = iov->iov_len;
	/* store original address for later verification */
	imu->ubuf = (unsigned long) iov->iov_base;
	imu->len = iov->iov_len;
	imu->folio_shift = PAGE_SHIFT;
	imu->release = io_release_ubuf;
	imu->priv = imu;
	imu->is_kbuf = false;
	imu->dir = IO_IMU_DEST | IO_IMU_SOURCE;
	if (coalesced)
		imu->folio_shift = data.folio_shift;
	refcount_set(&imu->refs, 1);
	off = (unsigned long) iov->iov_base & ((1UL << imu->folio_shift) - 1);
	node->buf = imu;
	ret = 0;

	for (i = 0; i < nr_pages; i++) {
		size_t vec_len;

		vec_len = min_t(size_t, size, (1UL << imu->folio_shift) - off);
		bvec_set_page(&imu->bvec[i], pages[i], vec_len, off);
		off = 0;
		size -= vec_len;
	}
done:
	if (ret) {
		if (imu)
			io_free_imu(ctx, imu);
		io_cache_free(&ctx->node_cache, node);
		node = ERR_PTR(ret);
	}
	kvfree(pages);
	return node;
}

/**
 * io_sqe_buffers_register - Registers buffers for io_uring.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 * @arg: Pointer to the user-provided buffer array.
 * @nr_args: Number of buffers to register.
 * @tags: Pointer to the user-provided tags array.
 *
 * This function registers an array of buffers for use with io_uring.
 * It validates the input, allocates resources, and updates the buffer table
 * in the io_uring context.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_sqe_buffers_register(struct io_ring_ctx *ctx, void __user *arg,
			    unsigned int nr_args, u64 __user *tags)
{
	struct page *last_hpage = NULL;
	struct io_rsrc_data data;
	struct iovec fast_iov, *iov = &fast_iov;
	const struct iovec __user *uvec;
	int i, ret;

	BUILD_BUG_ON(IORING_MAX_REG_BUFFERS >= (1u << 16));

	if (ctx->buf_table.nr)
		return -EBUSY;
	if (!nr_args || nr_args > IORING_MAX_REG_BUFFERS)
		return -EINVAL;
	ret = io_rsrc_data_alloc(&data, nr_args);
	if (ret)
		return ret;

	if (!arg)
		memset(iov, 0, sizeof(*iov));

	for (i = 0; i < nr_args; i++) {
		struct io_rsrc_node *node;
		u64 tag = 0;

		if (arg) {
			uvec = (struct iovec __user *) arg;
			iov = iovec_from_user(uvec, 1, 1, &fast_iov, ctx->compat);
			if (IS_ERR(iov)) {
				ret = PTR_ERR(iov);
				break;
			}
			ret = io_buffer_validate(iov);
			if (ret)
				break;
			if (ctx->compat)
				arg += sizeof(struct compat_iovec);
			else
				arg += sizeof(struct iovec);
		}

		if (tags) {
			if (copy_from_user(&tag, &tags[i], sizeof(tag))) {
				ret = -EFAULT;
				break;
			}
		}

		node = io_sqe_buffer_register(ctx, iov, &last_hpage);
		if (IS_ERR(node)) {
			ret = PTR_ERR(node);
			break;
		}
		if (tag) {
			if (!node) {
				ret = -EINVAL;
				break;
			}
			node->tag = tag;
		}
		data.nodes[i] = node;
	}

	ctx->buf_table = data;
	if (ret)
		io_sqe_buffers_unregister(ctx);
	return ret;
}

/**
 * io_buffer_register_bvec - Registers a buffer using a bio_vec for io_uring.
 *
 * @cmd: Pointer to the io_uring_cmd structure representing the command.
 * @rq: Pointer to the request structure.
 * @release: Function pointer for releasing the buffer.
 * @index: Index in the buffer table where the buffer will be registered.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function registers a buffer for io_uring using a bio_vec structure.
 * It validates the input, allocates resources, and updates the buffer table
 * in the io_uring context.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_buffer_register_bvec(struct io_uring_cmd *cmd, struct request *rq,
			    void (*release)(void *), unsigned int index,
			    unsigned int issue_flags)
{
	struct io_ring_ctx *ctx = cmd_to_io_kiocb(cmd)->ctx;
	struct io_rsrc_data *data = &ctx->buf_table;
	struct req_iterator rq_iter;
	struct io_mapped_ubuf *imu;
	struct io_rsrc_node *node;
	struct bio_vec bv, *bvec;
	u16 nr_bvecs;
	int ret = 0;

	io_ring_submit_lock(ctx, issue_flags);
	if (index >= data->nr) {
		ret = -EINVAL;
		goto unlock;
	}
	index = array_index_nospec(index, data->nr);

	if (data->nodes[index]) {
		ret = -EBUSY;
		goto unlock;
	}

	node = io_rsrc_node_alloc(ctx, IORING_RSRC_BUFFER);
	if (!node) {
		ret = -ENOMEM;
		goto unlock;
	}

	nr_bvecs = blk_rq_nr_phys_segments(rq);
	imu = io_alloc_imu(ctx, nr_bvecs);
	if (!imu) {
		kfree(node);
		ret = -ENOMEM;
		goto unlock;
	}

	imu->ubuf = 0;
	imu->len = blk_rq_bytes(rq);
	imu->acct_pages = 0;
	imu->folio_shift = PAGE_SHIFT;
	imu->nr_bvecs = nr_bvecs;
	refcount_set(&imu->refs, 1);
	imu->release = release;
	imu->priv = rq;
	imu->is_kbuf = true;
	imu->dir = 1 << rq_data_dir(rq);

	bvec = imu->bvec;
	rq_for_each_bvec(bv, rq, rq_iter)
		*bvec++ = bv;

	node->buf = imu;
	data->nodes[index] = node;
unlock:
	io_ring_submit_unlock(ctx, issue_flags);
	return ret;
}
EXPORT_SYMBOL_GPL(io_buffer_register_bvec);

/**
 * io_buffer_unregister_bvec - Unregisters a buffer using a bio_vec for io_uring.
 *
 * @cmd: Pointer to the io_uring_cmd structure representing the command.
 * @index: Index in the buffer table where the buffer is registered.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function unregisters a buffer for io_uring using a bio_vec structure.
 * It validates the input and releases the resources associated with the buffer.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_buffer_unregister_bvec(struct io_uring_cmd *cmd, unsigned int index,
			      unsigned int issue_flags)
{
	struct io_ring_ctx *ctx = cmd_to_io_kiocb(cmd)->ctx;
	struct io_rsrc_data *data = &ctx->buf_table;
	struct io_rsrc_node *node;
	int ret = 0;

	io_ring_submit_lock(ctx, issue_flags);
	if (index >= data->nr) {
		ret = -EINVAL;
		goto unlock;
	}
	index = array_index_nospec(index, data->nr);

	node = data->nodes[index];
	if (!node) {
		ret = -EINVAL;
		goto unlock;
	}
	if (!node->buf->is_kbuf) {
		ret = -EBUSY;
		goto unlock;
	}

	io_put_rsrc_node(ctx, node);
	data->nodes[index] = NULL;
unlock:
	io_ring_submit_unlock(ctx, issue_flags);
	return ret;
}
EXPORT_SYMBOL_GPL(io_buffer_unregister_bvec);

/**
 * validate_fixed_range - Validates a fixed buffer range.
 *
 * @buf_addr: Starting address of the buffer.
 * @len: Length of the buffer.
 * @imu: Pointer to the io_mapped_ubuf structure representing the buffer.
 *
 * This function checks if the specified buffer range is valid and within
 * the bounds of the registered buffer. It ensures that the buffer does not
 * exceed the allowed size or address range.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on invalid input.
 */
static int validate_fixed_range(u64 buf_addr, size_t len,
				const struct io_mapped_ubuf *imu)
{
	u64 buf_end;

	if (unlikely(check_add_overflow(buf_addr, (u64)len, &buf_end)))
		return -EFAULT;
	/* not inside the mapped region */
	if (unlikely(buf_addr < imu->ubuf || buf_end > (imu->ubuf + imu->len)))
		return -EFAULT;
	if (unlikely(len > MAX_RW_COUNT))
		return -EFAULT;
	return 0;
}

/**
 * io_import_fixed - Imports a fixed buffer for I/O operations.
 *
 * @ddir: Direction of the data transfer (read or write).
 * @iter: Pointer to the iov_iter structure for the I/O operation.
 * @imu: Pointer to the io_mapped_ubuf structure representing the buffer.
 * @buf_addr: Starting address of the buffer.
 * @len: Length of the buffer.
 *
 * This function imports a fixed buffer for use in I/O operations. It validates
 * the buffer range, sets up the I/O iterator, and adjusts the iterator to
 * account for the buffer offset.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
static int io_import_fixed(int ddir, struct iov_iter *iter,
			   struct io_mapped_ubuf *imu,
			   u64 buf_addr, size_t len)
{
	size_t offset;
	int ret;

	if (WARN_ON_ONCE(!imu))
		return -EFAULT;
	ret = validate_fixed_range(buf_addr, len, imu);
	if (unlikely(ret))
		return ret;
	if (!(imu->dir & (1 << ddir)))
		return -EFAULT;

	/*
	 * Might not be a start of buffer, set size appropriately
	 * and advance us to the beginning.
	 */
	offset = buf_addr - imu->ubuf;
	iov_iter_bvec(iter, ddir, imu->bvec, imu->nr_bvecs, offset + len);

	if (offset) {
		/*
		 * Don't use iov_iter_advance() here, as it's really slow for
		 * using the latter parts of a big fixed buffer - it iterates
		 * over each segment manually. We can cheat a bit here for user
		 * registered nodes, because we know that:
		 *
		 * 1) it's a BVEC iter, we set it up
		 * 2) all bvecs are the same in size, except potentially the
		 *    first and last bvec
		 *
		 * So just find our index, and adjust the iterator afterwards.
		 * If the offset is within the first bvec (or the whole first
		 * bvec, just use iov_iter_advance(). This makes it easier
		 * since we can just skip the first segment, which may not
		 * be folio_size aligned.
		 */
		const struct bio_vec *bvec = imu->bvec;

		/*
		 * Kernel buffer bvecs, on the other hand, don't necessarily
		 * have the size property of user registered ones, so we have
		 * to use the slow iter advance.
		 */
		if (offset < bvec->bv_len) {
			iter->count -= offset;
			iter->iov_offset = offset;
		} else if (imu->is_kbuf) {
			iov_iter_advance(iter, offset);
		} else {
			unsigned long seg_skip;

			/* skip first vec */
			offset -= bvec->bv_len;
			seg_skip = 1 + (offset >> imu->folio_shift);

			iter->bvec += seg_skip;
			iter->nr_segs -= seg_skip;
			iter->count -= bvec->bv_len + offset;
			iter->iov_offset = offset & ((1UL << imu->folio_shift) - 1);
		}
	}

	return 0;
}

inline struct io_rsrc_node *io_find_buf_node(struct io_kiocb *req,
					     unsigned issue_flags)
{
	struct io_ring_ctx *ctx = req->ctx;
	struct io_rsrc_node *node;

	if (req->flags & REQ_F_BUF_NODE)
		return req->buf_node;

	io_ring_submit_lock(ctx, issue_flags);
	node = io_rsrc_node_lookup(&ctx->buf_table, req->buf_index);
	if (node)
		io_req_assign_buf_node(req, node);
	io_ring_submit_unlock(ctx, issue_flags);
	return node;
}

/**
 * io_import_reg_buf - Imports a registered buffer for I/O operations.
 *
 * @req: Pointer to the io_kiocb structure representing the I/O request.
 * @iter: Pointer to the iov_iter structure for the I/O operation.
 * @buf_addr: Starting address of the buffer.
 * @len: Length of the buffer.
 * @ddir: Direction of the data transfer (read or write).
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function imports a registered buffer for use in I/O operations. It
 * retrieves the buffer node associated with the request, validates the buffer
 * range, and sets up the I/O iterator for the operation.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_import_reg_buf(struct io_kiocb *req, struct iov_iter *iter,
			u64 buf_addr, size_t len, int ddir,
			unsigned issue_flags)
{
	struct io_rsrc_node *node;

	node = io_find_buf_node(req, issue_flags);
	if (!node)
		return -EFAULT;
	return io_import_fixed(ddir, iter, node->buf, buf_addr, len);
}

/* Lock two rings at once. The rings must be different! */

/**
 * lock_two_rings - Locks two io_uring contexts in a specific order.
 *
 * @ctx1: Pointer to the first io_ring_ctx structure.
 * @ctx2: Pointer to the second io_ring_ctx structure.
 *
 * This function locks two io_uring contexts in a specific order to avoid
 * deadlocks. The locks are acquired in ascending order of the context
 * pointers.
 */
static void lock_two_rings(struct io_ring_ctx *ctx1, struct io_ring_ctx *ctx2)
{
	if (ctx1 > ctx2)
		swap(ctx1, ctx2);
	mutex_lock(&ctx1->uring_lock);
	mutex_lock_nested(&ctx2->uring_lock, SINGLE_DEPTH_NESTING);
}

/* Both rings are locked by the caller. */

/**
 * io_clone_buffers - Clones registered buffers from one io_uring context to another.
 *
 * @ctx: Pointer to the destination io_ring_ctx structure.
 * @src_ctx: Pointer to the source io_ring_ctx structure.
 * @arg: Pointer to the io_uring_clone_buffers structure containing clone details.
 *
 * This function clones registered buffers from the source io_uring context to
 * the destination io_uring context. It validates the input parameters, allocates
 * resources, and updates the destination context with the cloned buffers.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
static int io_clone_buffers(struct io_ring_ctx *ctx, struct io_ring_ctx *src_ctx,
			    struct io_uring_clone_buffers *arg)
{
	struct io_rsrc_data data;
	int i, ret, off, nr;
	unsigned int nbufs;

	lockdep_assert_held(&ctx->uring_lock);
	lockdep_assert_held(&src_ctx->uring_lock);

	/*
	 * Accounting state is shared between the two rings; that only works if
	 * both rings are accounted towards the same counters.
	 */
	if (ctx->user != src_ctx->user || ctx->mm_account != src_ctx->mm_account)
		return -EINVAL;

	/* if offsets are given, must have nr specified too */
	if (!arg->nr && (arg->dst_off || arg->src_off))
		return -EINVAL;
	/* not allowed unless REPLACE is set */
	if (ctx->buf_table.nr && !(arg->flags & IORING_REGISTER_DST_REPLACE))
		return -EBUSY;

	nbufs = src_ctx->buf_table.nr;
	if (!arg->nr)
		arg->nr = nbufs;
	else if (arg->nr > nbufs)
		return -EINVAL;
	else if (arg->nr > IORING_MAX_REG_BUFFERS)
		return -EINVAL;
	if (check_add_overflow(arg->nr, arg->dst_off, &nbufs))
		return -EOVERFLOW;

	ret = io_rsrc_data_alloc(&data, max(nbufs, ctx->buf_table.nr));
	if (ret)
		return ret;

	/* Fill entries in data from dst that won't overlap with src */
	for (i = 0; i < min(arg->dst_off, ctx->buf_table.nr); i++) {
		struct io_rsrc_node *src_node = ctx->buf_table.nodes[i];

		if (src_node) {
			data.nodes[i] = src_node;
			src_node->refs++;
		}
	}

	ret = -ENXIO;
	nbufs = src_ctx->buf_table.nr;
	if (!nbufs)
		goto out_free;
	ret = -EINVAL;
	if (!arg->nr)
		arg->nr = nbufs;
	else if (arg->nr > nbufs)
		goto out_free;
	ret = -EOVERFLOW;
	if (check_add_overflow(arg->nr, arg->src_off, &off))
		goto out_free;
	if (off > nbufs)
		goto out_free;

	off = arg->dst_off;
	i = arg->src_off;
	nr = arg->nr;
	while (nr--) {
		struct io_rsrc_node *dst_node, *src_node;

		src_node = io_rsrc_node_lookup(&src_ctx->buf_table, i);
		if (!src_node) {
			dst_node = NULL;
		} else {
			dst_node = io_rsrc_node_alloc(ctx, IORING_RSRC_BUFFER);
			if (!dst_node) {
				ret = -ENOMEM;
				goto out_free;
			}

			refcount_inc(&src_node->buf->refs);
			dst_node->buf = src_node->buf;
		}
		data.nodes[off++] = dst_node;
		i++;
	}

	/*
	 * If asked for replace, put the old table. data->nodes[] holds both
	 * old and new nodes at this point.
	 */
	if (arg->flags & IORING_REGISTER_DST_REPLACE)
		io_rsrc_data_free(ctx, &ctx->buf_table);

	/*
	 * ctx->buf_table must be empty now - either the contents are being
	 * replaced and we just freed the table, or the contents are being
	 * copied to a ring that does not have buffers yet (checked at function
	 * entry).
	 */
	WARN_ON_ONCE(ctx->buf_table.nr);
	ctx->buf_table = data;
	return 0;

out_free:
	io_rsrc_data_free(ctx, &data);
	return ret;
}

/*
 * Copy the registered buffers from the source ring whose file descriptor
 * is given in the src_fd to the current ring. This is identical to registering
 * the buffers with ctx, except faster as mappings already exist.
 *
 * Since the memory is already accounted once, don't account it again.
 */

 /**
 * io_register_clone_buffers - Clones registered buffers from another io_uring instance.
 *
 * @ctx: Pointer to the destination io_ring_ctx structure.
 * @arg: Pointer to the user-provided io_uring_clone_buffers structure.
 *
 * This function clones registered buffers from the source io_uring instance
 * specified by the file descriptor in the user-provided structure. It ensures
 * that the memory mappings are reused for efficiency and avoids double accounting
 * of memory usage.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_register_clone_buffers(struct io_ring_ctx *ctx, void __user *arg)
{
	struct io_uring_clone_buffers buf;
	struct io_ring_ctx *src_ctx;
	bool registered_src;
	struct file *file;
	int ret;

	if (copy_from_user(&buf, arg, sizeof(buf)))
		return -EFAULT;
	if (buf.flags & ~(IORING_REGISTER_SRC_REGISTERED|IORING_REGISTER_DST_REPLACE))
		return -EINVAL;
	if (!(buf.flags & IORING_REGISTER_DST_REPLACE) && ctx->buf_table.nr)
		return -EBUSY;
	if (memchr_inv(buf.pad, 0, sizeof(buf.pad)))
		return -EINVAL;

	registered_src = (buf.flags & IORING_REGISTER_SRC_REGISTERED) != 0;
	file = io_uring_register_get_file(buf.src_fd, registered_src);
	if (IS_ERR(file))
		return PTR_ERR(file);

	src_ctx = file->private_data;
	if (src_ctx != ctx) {
		mutex_unlock(&ctx->uring_lock);
		lock_two_rings(ctx, src_ctx);
	}

	ret = io_clone_buffers(ctx, src_ctx, &buf);

	if (src_ctx != ctx)
		mutex_unlock(&src_ctx->uring_lock);

	fput(file);
	return ret;
}

/**
 * io_vec_free - Frees an iovec structure.
 *
 * @iv: Pointer to the iou_vec structure to be freed.
 *
 * This function releases the memory allocated for the iovec structure
 * and resets its fields to indicate that it is no longer in use.
 */
void io_vec_free(struct iou_vec *iv)
{
	if (!iv->iovec)
		return;
	kfree(iv->iovec);
	iv->iovec = NULL;
	iv->nr = 0;
}

/**
 * io_vec_realloc - Reallocates an iovec structure.
 *
 * @iv: Pointer to the iou_vec structure to be reallocated.
 * @nr_entries: The new number of entries for the iovec structure.
 *
 * This function reallocates the memory for the iovec structure to accommodate
 * the specified number of entries. If the reallocation is successful, the
 * existing iovec structure is updated with the new memory.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_vec_realloc(struct iou_vec *iv, unsigned nr_entries)
{
	gfp_t gfp = GFP_KERNEL | __GFP_NOWARN;
	struct iovec *iov;

	iov = kmalloc_array(nr_entries, sizeof(iov[0]), gfp);
	if (!iov)
		return -ENOMEM;

	io_vec_free(iv);
	iv->iovec = iov;
	iv->nr = nr_entries;
	return 0;
}

/**
 * io_vec_fill_bvec - Fills a bio_vec structure from an iovec for I/O operations.
 *
 * @ddir: Direction of the data transfer (read or write).
 * @iter: Pointer to the iov_iter structure for the I/O operation.
 * @imu: Pointer to the io_mapped_ubuf structure representing the buffer.
 * @iovec: Pointer to the iovec array containing the buffer details.
 * @nr_iovs: Number of iovec entries to process.
 * @vec: Pointer to the iou_vec structure to be filled with bio_vec entries.
 *
 * This function converts an iovec structure into a bio_vec structure for use
 * in I/O operations. It validates the buffer range, calculates the offsets,
 * and fills the bio_vec entries with the appropriate page and offset details.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
static int io_vec_fill_bvec(int ddir, struct iov_iter *iter,
				struct io_mapped_ubuf *imu,
				struct iovec *iovec, unsigned nr_iovs,
				struct iou_vec *vec)
{
	unsigned long folio_size = 1 << imu->folio_shift;
	unsigned long folio_mask = folio_size - 1;
	u64 folio_addr = imu->ubuf & ~folio_mask;
	struct bio_vec *res_bvec = vec->bvec;
	size_t total_len = 0;
	unsigned bvec_idx = 0;
	unsigned iov_idx;

	for (iov_idx = 0; iov_idx < nr_iovs; iov_idx++) {
		size_t iov_len = iovec[iov_idx].iov_len;
		u64 buf_addr = (u64)(uintptr_t)iovec[iov_idx].iov_base;
		struct bio_vec *src_bvec;
		size_t offset;
		int ret;

		ret = validate_fixed_range(buf_addr, iov_len, imu);
		if (unlikely(ret))
			return ret;

		if (unlikely(!iov_len))
			return -EFAULT;
		if (unlikely(check_add_overflow(total_len, iov_len, &total_len)))
			return -EOVERFLOW;

		/* by using folio address it also accounts for bvec offset */
		offset = buf_addr - folio_addr;
		src_bvec = imu->bvec + (offset >> imu->folio_shift);
		offset &= folio_mask;

		for (; iov_len; offset = 0, bvec_idx++, src_bvec++) {
			size_t seg_size = min_t(size_t, iov_len,
						folio_size - offset);

			bvec_set_page(&res_bvec[bvec_idx],
				      src_bvec->bv_page, seg_size, offset);
			iov_len -= seg_size;
		}
	}
	if (total_len > MAX_RW_COUNT)
		return -EINVAL;

	iov_iter_bvec(iter, ddir, res_bvec, bvec_idx, total_len);
	return 0;
}

/**
 * io_estimate_bvec_size - Estimates the number of bio_vec entries needed for an iovec.
 *
 * @iov: Pointer to the iovec array containing the buffer details.
 * @nr_iovs: Number of iovec entries to process.
 * @imu: Pointer to the io_mapped_ubuf structure representing the buffer.
 *
 * This function calculates the maximum number of bio_vec entries required to
 * represent the given iovec structure. It considers the folio size and alignment
 * of the buffer to determine the number of segments needed.
 *
 * Returns:
 * - The estimated number of bio_vec entries.
 */
static int io_estimate_bvec_size(struct iovec *iov, unsigned nr_iovs,
				 struct io_mapped_ubuf *imu)
{
	unsigned shift = imu->folio_shift;
	size_t max_segs = 0;
	unsigned i;

	for (i = 0; i < nr_iovs; i++)
		max_segs += (iov[i].iov_len >> shift) + 2;
	return max_segs;
}

/**
 * io_vec_fill_kern_bvec - Fills a bio_vec structure for kernel buffers.
 *
 * @ddir: Direction of the data transfer (read or write).
 * @iter: Pointer to the iov_iter structure for the I/O operation.
 * @imu: Pointer to the io_mapped_ubuf structure representing the buffer.
 * @iovec: Pointer to the iovec array containing the buffer details.
 * @nr_iovs: Number of iovec entries to process.
 * @vec: Pointer to the iou_vec structure to be filled with bio_vec entries.
 *
 * This function converts an iovec structure into a bio_vec structure for kernel
 * buffers. It validates the buffer range, calculates the offsets, and fills the
 * bio_vec entries with the appropriate page and offset details.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
static int io_vec_fill_kern_bvec(int ddir, struct iov_iter *iter,
				 struct io_mapped_ubuf *imu,
				 struct iovec *iovec, unsigned nr_iovs,
				 struct iou_vec *vec)
{
	const struct bio_vec *src_bvec = imu->bvec;
	struct bio_vec *res_bvec = vec->bvec;
	unsigned res_idx = 0;
	size_t total_len = 0;
	unsigned iov_idx;

	for (iov_idx = 0; iov_idx < nr_iovs; iov_idx++) {
		size_t offset = (size_t)(uintptr_t)iovec[iov_idx].iov_base;
		size_t iov_len = iovec[iov_idx].iov_len;
		struct bvec_iter bi = {
			.bi_size        = offset + iov_len,
		};
		struct bio_vec bv;

		bvec_iter_advance(src_bvec, &bi, offset);
		for_each_mp_bvec(bv, src_bvec, bi, bi)
			res_bvec[res_idx++] = bv;
		total_len += iov_len;
	}
	iov_iter_bvec(iter, ddir, res_bvec, res_idx, total_len);
	return 0;
}

/**
 * iov_kern_bvec_size - Calculate the size of kernel bvec for an iovec structure
 * @iov: Pointer to the iovec structure containing user-space memory buffers
 * @imu: Pointer to the io_mapped_ubuf structure for mapped user buffers
 * @nr_seg: Pointer to an unsigned int to store the number of segments
 *
 * This function computes the size of the kernel bvec required to represent
 * the memory described by the given iovec structure. It also calculates
 * the number of segments and stores it in the location pointed to by @nr_seg.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
static int iov_kern_bvec_size(const struct iovec *iov,
			      const struct io_mapped_ubuf *imu,
			      unsigned int *nr_seg)
{
	size_t offset = (size_t)(uintptr_t)iov->iov_base;
	const struct bio_vec *bvec = imu->bvec;
	int start = 0, i = 0;
	size_t off = 0;
	int ret;

	ret = validate_fixed_range(offset, iov->iov_len, imu);
	if (unlikely(ret))
		return ret;

	for (i = 0; off < offset + iov->iov_len && i < imu->nr_bvecs;
			off += bvec[i].bv_len, i++) {
		if (offset >= off && offset < off + bvec[i].bv_len)
			start = i;
	}
	*nr_seg = i - start;
	return 0;
}

/**
 * io_kern_bvec_size - Calculates the size of kernel bio-vector segments.
 * @iov: Pointer to an array of iovec structures representing user buffers.
 * @nr_iovs: The number of iovec structures in the array.
 * @imu: Pointer to an io_mapped_ubuf structure for storing mapped user buffers.
 * @nr_segs: Pointer to an unsigned integer to store the number of segments.
 *
 * This function computes the size of kernel bio-vector segments based on the
 * provided iovec array and updates the number of segments in the provided
 * pointer. It is used for managing memory mappings and segmenting user buffers
 * for kernel operations.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
static int io_kern_bvec_size(struct iovec *iov, unsigned nr_iovs,
			     struct io_mapped_ubuf *imu, unsigned *nr_segs)
{
	unsigned max_segs = 0;
	size_t total_len = 0;
	unsigned i;
	int ret;

	*nr_segs = 0;
	for (i = 0; i < nr_iovs; i++) {
		if (unlikely(!iov[i].iov_len))
			return -EFAULT;
		if (unlikely(check_add_overflow(total_len, iov[i].iov_len,
						&total_len)))
			return -EOVERFLOW;
		ret = iov_kern_bvec_size(&iov[i], imu, &max_segs);
		if (unlikely(ret))
			return ret;
		*nr_segs += max_segs;
	}
	if (total_len > MAX_RW_COUNT)
		return -EINVAL;
	return 0;
}

/**
 * io_import_reg_vec - Imports a registered vector for I/O operations.
 *
 * @ddir: Direction of the data transfer (read or write).
 * @iter: Pointer to the iov_iter structure for the I/O operation.
 * @req: Pointer to the io_kiocb structure representing the request.
 * @vec: Pointer to the iou_vec structure to be filled.
 * @nr_iovs: Number of iovec entries to process.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function imports a registered vector for use in I/O operations. It
 * retrieves the buffer node associated with the request, validates the buffer
 * range, and sets up the I/O iterator for the operation.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_import_reg_vec(int ddir, struct iov_iter *iter,
			struct io_kiocb *req, struct iou_vec *vec,
			unsigned nr_iovs, unsigned issue_flags)
{
	struct io_rsrc_node *node;
	struct io_mapped_ubuf *imu;
	unsigned iovec_off;
	struct iovec *iov;
	unsigned nr_segs;

	node = io_find_buf_node(req, issue_flags);
	if (!node)
		return -EFAULT;
	imu = node->buf;
	if (!(imu->dir & (1 << ddir)))
		return -EFAULT;

	iovec_off = vec->nr - nr_iovs;
	iov = vec->iovec + iovec_off;

	if (imu->is_kbuf) {
		int ret = io_kern_bvec_size(iov, nr_iovs, imu, &nr_segs);

		if (unlikely(ret))
			return ret;
	} else {
		nr_segs = io_estimate_bvec_size(iov, nr_iovs, imu);
	}

	if (sizeof(struct bio_vec) > sizeof(struct iovec)) {
		size_t bvec_bytes;

		bvec_bytes = nr_segs * sizeof(struct bio_vec);
		nr_segs = (bvec_bytes + sizeof(*iov) - 1) / sizeof(*iov);
		nr_segs += nr_iovs;
	}

	if (nr_segs > vec->nr) {
		struct iou_vec tmp_vec = {};
		int ret;

		ret = io_vec_realloc(&tmp_vec, nr_segs);
		if (ret)
			return ret;

		iovec_off = tmp_vec.nr - nr_iovs;
		memcpy(tmp_vec.iovec + iovec_off, iov, sizeof(*iov) * nr_iovs);
		io_vec_free(vec);

		*vec = tmp_vec;
		iov = vec->iovec + iovec_off;
		req->flags |= REQ_F_NEED_CLEANUP;
	}

	if (imu->is_kbuf)
		return io_vec_fill_kern_bvec(ddir, iter, imu, iov, nr_iovs, vec);

	return io_vec_fill_bvec(ddir, iter, imu, iov, nr_iovs, vec);
}

/**
 * io_prep_reg_iovec - Prepares an iovec structure for registration.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @iv: Pointer to the iou_vec structure to be prepared.
 * @uvec: Pointer to the user-provided iovec array.
 * @uvec_segs: Number of segments in the user-provided iovec array.
 *
 * This function prepares an iovec structure for registration by copying the
 * user-provided iovec array into the kernel space. It ensures that the iovec
 * structure is properly aligned and ready for use in I/O operations.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_prep_reg_iovec(struct io_kiocb *req, struct iou_vec *iv,
		      const struct iovec __user *uvec, size_t uvec_segs)
{
	struct iovec *iov;
	int iovec_off, ret;
	void *res;

	if (uvec_segs > iv->nr) {
		ret = io_vec_realloc(iv, uvec_segs);
		if (ret)
			return ret;
		req->flags |= REQ_F_NEED_CLEANUP;
	}

	/* pad iovec to the right */
	iovec_off = iv->nr - uvec_segs;
	iov = iv->iovec + iovec_off;
	res = iovec_from_user(uvec, uvec_segs, uvec_segs, iov,
			      io_is_compat(req->ctx));
	if (IS_ERR(res))
		return PTR_ERR(res);

	req->flags |= REQ_F_IMPORT_BUFFER;
	return 0;
}
