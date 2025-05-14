// SPDX-License-Identifier: GPL-2.0
#ifndef IOU_RSRC_H
#define IOU_RSRC_H

#include <linux/io_uring_types.h>
#include <linux/lockdep.h>

#define IO_VEC_CACHE_SOFT_CAP 256

enum
{
	IORING_RSRC_FILE = 0,
	IORING_RSRC_BUFFER = 1,
};

struct io_rsrc_node
{
	unsigned char type;
	int refs;

	u64 tag;
	union
	{
		unsigned long file_ptr;
		struct io_mapped_ubuf *buf;
	};
};

enum
{
	IO_IMU_DEST = 1 << ITER_DEST,
	IO_IMU_SOURCE = 1 << ITER_SOURCE,
};

struct io_mapped_ubuf
{
	u64 ubuf;
	unsigned int len;
	unsigned int nr_bvecs;
	unsigned int folio_shift;
	refcount_t refs;
	unsigned long acct_pages;
	void (*release)(void *);
	void *priv;
	bool is_kbuf;
	u8 dir;
	struct bio_vec bvec[] __counted_by(nr_bvecs);
};

struct io_imu_folio_data
{
	/* Head folio can be partially included in the fixed buf */
	unsigned int nr_pages_head;
	/* For non-head/tail folios, has to be fully included */
	unsigned int nr_pages_mid;
	unsigned int folio_shift;
	unsigned int nr_folios;
};

bool io_rsrc_cache_init(struct io_ring_ctx *ctx);
void io_rsrc_cache_free(struct io_ring_ctx *ctx);
struct io_rsrc_node *io_rsrc_node_alloc(struct io_ring_ctx *ctx, int type);
void io_free_rsrc_node(struct io_ring_ctx *ctx, struct io_rsrc_node *node);
void io_rsrc_data_free(struct io_ring_ctx *ctx, struct io_rsrc_data *data);
int io_rsrc_data_alloc(struct io_rsrc_data *data, unsigned nr);

struct io_rsrc_node *io_find_buf_node(struct io_kiocb *req,
									  unsigned issue_flags);
int io_import_reg_buf(struct io_kiocb *req, struct iov_iter *iter,
					  u64 buf_addr, size_t len, int ddir,
					  unsigned issue_flags);
int io_import_reg_vec(int ddir, struct iov_iter *iter,
					  struct io_kiocb *req, struct iou_vec *vec,
					  unsigned nr_iovs, unsigned issue_flags);
int io_prep_reg_iovec(struct io_kiocb *req, struct iou_vec *iv,
					  const struct iovec __user *uvec, size_t uvec_segs);

int io_register_clone_buffers(struct io_ring_ctx *ctx, void __user *arg);
int io_sqe_buffers_unregister(struct io_ring_ctx *ctx);
int io_sqe_buffers_register(struct io_ring_ctx *ctx, void __user *arg,
							unsigned int nr_args, u64 __user *tags);
int io_sqe_files_unregister(struct io_ring_ctx *ctx);
int io_sqe_files_register(struct io_ring_ctx *ctx, void __user *arg,
						  unsigned nr_args, u64 __user *tags);

int io_register_files_update(struct io_ring_ctx *ctx, void __user *arg,
							 unsigned nr_args);
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
							unsigned size, unsigned type);

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
int io_register_rsrc(struct io_ring_ctx *ctx, void __user *arg,
					 unsigned int size, unsigned int type);

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
int io_buffer_validate(struct iovec *iov);

/**
 * io_check_coalesce_buffer - Checks if a buffer can be coalesced.
 *
 * @page_array: Array of pages representing the buffer.
 * @nr_pages: Number of pages in the buffer.
 * @data: Pointer to the io_imu_folio_data structure containing folio details.
 *
 * This function checks if the given buffer can be coalesced into a single
 * contiguous memory region based on the provided folio data.
 *
 * Returns:
 * - true if the buffer can be coalesced.
 * - false otherwise.
 */
bool io_check_coalesce_buffer(struct page **page_array, int nr_pages,
							  struct io_imu_folio_data *data);

/**
 * io_rsrc_node_lookup - Looks up a resource node by index.
 *
 * @data: Pointer to the io_rsrc_data structure containing resource nodes.
 * @index: Index of the resource node to look up.
 *
 * This function retrieves a resource node from the resource data structure
 * based on the specified index. It ensures that the index is within bounds
 * and uses array_index_nospec to prevent speculative execution attacks.
 *
 * Returns:
 * - Pointer to the resource node on success.
 * - NULL if the index is out of bounds.
 */
static inline struct io_rsrc_node *io_rsrc_node_lookup(struct io_rsrc_data *data,
						       int index)
{
	if (index < data->nr)
		return data->nodes[array_index_nospec(index, data->nr)];
	return NULL;
}

static inline void io_put_rsrc_node(struct io_ring_ctx *ctx, struct io_rsrc_node *node)
{
	lockdep_assert_held(&ctx->uring_lock);
	if (!--node->refs)
		io_free_rsrc_node(ctx, node);
}

static inline bool io_reset_rsrc_node(struct io_ring_ctx *ctx,
				      struct io_rsrc_data *data, int index)
{
	struct io_rsrc_node *node = data->nodes[index];

	if (!node)
		return false;
	io_put_rsrc_node(ctx, node);
	data->nodes[index] = NULL;
	return true;
}

static inline void io_req_put_rsrc_nodes(struct io_kiocb *req)
{
	if (req->file_node) {
		io_put_rsrc_node(req->ctx, req->file_node);
		req->file_node = NULL;
	}
	if (req->flags & REQ_F_BUF_NODE) {
		io_put_rsrc_node(req->ctx, req->buf_node);
		req->buf_node = NULL;
	}
}

static inline void io_req_assign_rsrc_node(struct io_rsrc_node **dst_node,
					   struct io_rsrc_node *node)
{
	node->refs++;
	*dst_node = node;
}

static inline void io_req_assign_buf_node(struct io_kiocb *req,
					  struct io_rsrc_node *node)
{
	io_req_assign_rsrc_node(&req->buf_node, node);
	req->flags |= REQ_F_BUF_NODE;
}

int io_files_update(struct io_kiocb *req, unsigned int issue_flags);

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
int io_files_update_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

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
int __io_account_mem(struct user_struct *user, unsigned long nr_pages);

/**
 * __io_unaccount_mem - Unaccounts memory usage for a user.
 *
 * @user: Pointer to the user structure.
 * @nr_pages: Number of pages to unaccount.
 *
 * This function decreases the memory usage count for the given user.
 */
static inline void __io_unaccount_mem(struct user_struct *user,
				      unsigned long nr_pages)
{
	atomic_long_sub(nr_pages, &user->locked_vm);
}

void io_vec_free(struct iou_vec *iv);

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
int io_vec_realloc(struct iou_vec *iv, unsigned nr_entries);

/**
 * io_vec_reset_iovec - Resets an iovec structure.
 *
 * @iv: Pointer to the iou_vec structure to be reset.
 * @iovec: Pointer to the new iovec array.
 * @nr: Number of entries in the new iovec array.
 *
 * This function resets the iovec structure by freeing its existing memory
 * and assigning the new iovec array.
 */
static inline void io_vec_reset_iovec(struct iou_vec *iv,
				      struct iovec *iovec, unsigned nr)
{
	io_vec_free(iv);
	iv->iovec = iovec;
	iv->nr = nr;
}

static inline void io_alloc_cache_vec_kasan(struct iou_vec *iv)
{
	if (IS_ENABLED(CONFIG_KASAN))
		io_vec_free(iv);
}

#endif
