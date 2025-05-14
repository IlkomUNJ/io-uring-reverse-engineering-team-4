// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/fsnotify.h>
#include <linux/namei.h>
#include <linux/io_uring.h>

#include <uapi/linux/io_uring.h>

#include "../fs/internal.h"

#include "io_uring.h"
#include "rsrc.h"
#include "openclose.h"

/**
 * struct io_open - Represents the data structure for an open operation in io_uring.

 * This structure is used to encapsulate all the necessary information
 * for performing an open operation within the io_uring subsystem.
 */
struct io_open {
	struct file			*file;
	int				dfd;
	u32				file_slot;
	struct filename			*filename;
	struct open_how			how;
	unsigned long			nofile;
};

/**
 * struct io_close - Represents the structure for closing a file descriptor.
 * @file: Pointer to the file structure associated with the file descriptor.
 * @fd: The file descriptor to be closed.
 * @file_slot: The slot index in the file table, used for managing file descriptors.
 */
struct io_close {
	struct file			*file;
	int				fd;
	u32				file_slot;
};

/**
 * struct io_fixed_install - Represents the structure for installing a fixed file descriptor.
 * @file: Pointer to the file structure to be installed.
 * @o_flags: Flags associated with the file operation, such as access mode or permissions.
 */
struct io_fixed_install {
	struct file			*file;
	unsigned int			o_flags;
};

/**
 * io_openat_force_async - Determines if an open operation should be forced to run asynchronously.
 
 * This function checks the flags in the `how` field of the `io_open` structure
 * to determine if the open operation should be forced to run asynchronously.

 * Return: true if the operation should be forced to run asynchronously, false otherwise.
 */
static bool io_openat_force_async(struct io_open *open)
{
	/*
	 * Don't bother trying for O_TRUNC, O_CREAT, or O_TMPFILE open,
	 * it'll always -EAGAIN. Note that we test for __O_TMPFILE because
	 * O_TMPFILE includes O_DIRECTORY, which isn't a flag we need to force
	 * async for.
	 */
	return open->how.flags & (O_TRUNC | O_CREAT | __O_TMPFILE);
}

/**
 * __io_openat_prep - Prepares an open operation for io_uring.
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring_sqe structure containing the submission queue entry.
 *
 * This function initializes and validates the necessary data structures
 * for handling an open operation in the io_uring context. It extracts
 * relevant information from the submission queue entry (sqe) and sets
 * up the request (req) accordingly.
 *
 * Return:
 * - 0 on success.
 * - Negative error codes (e.g., -EINVAL, -EBADF) on failure.
 */
static int __io_openat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe)
{
	struct io_open *open = io_kiocb_to_cmd(req, struct io_open);
	const char __user *fname;
	int ret;

	if (unlikely(sqe->buf_index))
		return -EINVAL;
	if (unlikely(req->flags & REQ_F_FIXED_FILE))
		return -EBADF;

	/* open.how should be already initialised */
	if (!(open->how.flags & O_PATH) && force_o_largefile())
		open->how.flags |= O_LARGEFILE;

	open->dfd = READ_ONCE(sqe->fd);
	fname = u64_to_user_ptr(READ_ONCE(sqe->addr));
	open->filename = getname(fname);
	if (IS_ERR(open->filename)) {
		ret = PTR_ERR(open->filename);
		open->filename = NULL;
		return ret;
	}

	open->file_slot = READ_ONCE(sqe->file_index);
	if (open->file_slot && (open->how.flags & O_CLOEXEC))
		return -EINVAL;

	open->nofile = rlimit(RLIMIT_NOFILE);
	req->flags |= REQ_F_NEED_CLEANUP;
	if (io_openat_force_async(open))
		req->flags |= REQ_F_FORCE_ASYNC;
	return 0;
}

/**
 * io_openat_prep - Wrapper function to prepare an open operation.
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring_sqe structure containing the submission queue entry.
 *
 * This function builds the `open_how` structure using the flags and mode
 * provided in the submission queue entry (sqe) and then calls
 * `__io_openat_prep` to complete the preparation.
 *
 * Return:
 * - 0 on success.
 * - Negative error codes on failure.
 */
int io_openat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe)
{
	struct io_open *open = io_kiocb_to_cmd(req, struct io_open);
	u64 mode = READ_ONCE(sqe->len);
	u64 flags = READ_ONCE(sqe->open_flags);

	open->how = build_open_how(flags, mode);
	return __io_openat_prep(req, sqe);
}

/**
 * io_openat2_prep - Prepares an openat2 operation for io_uring.
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring_sqe structure containing the submission queue entry.
 *
 * This function validates and prepares the `open_how` structure for an
 * openat2 operation. It ensures that the provided user-space structure
 * is valid and then calls `__io_openat_prep` to complete the preparation.
 *
 * Return:
 * - 0 on success.
 * - Negative error codes (e.g., -EINVAL) on failure.
 */
int io_openat2_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe)
{
	struct io_open *open = io_kiocb_to_cmd(req, struct io_open);
	struct open_how __user *how;
	size_t len;
	int ret;

	how = u64_to_user_ptr(READ_ONCE(sqe->addr2));
	len = READ_ONCE(sqe->len);
	if (len < OPEN_HOW_SIZE_VER0)
		return -EINVAL;

	ret = copy_struct_from_user(&open->how, sizeof(open->how), how, len);
	if (ret)
		return ret;

	return __io_openat_prep(req, sqe);
}

/**
 * io_openat2 - Handles the io_uring openat2 operation.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating how the operation should be issued.
 *
 * This function performs an openat2 operation using io_uring. It converts
 * the request into an io_open command, builds the necessary open flags,
 * and attempts to open the specified file. The function supports both
 * blocking and non-blocking modes, as well as fixed file descriptors.
 *
 */
int io_openat2(struct io_kiocb *req, unsigned int issue_flags)
{
	struct io_open *open = io_kiocb_to_cmd(req, struct io_open);
	struct open_flags op;
	struct file *file;
	bool resolve_nonblock, nonblock_set;
	bool fixed = !!open->file_slot;
	int ret;

	ret = build_open_flags(&open->how, &op);
	if (ret)
		goto err;
	nonblock_set = op.open_flag & O_NONBLOCK;
	resolve_nonblock = open->how.resolve & RESOLVE_CACHED;
	if (issue_flags & IO_URING_F_NONBLOCK) {
		WARN_ON_ONCE(io_openat_force_async(open));
		op.lookup_flags |= LOOKUP_CACHED;
		op.open_flag |= O_NONBLOCK;
	}

	if (!fixed) {
		ret = __get_unused_fd_flags(open->how.flags, open->nofile);
		if (ret < 0)
			goto err;
	}

	file = do_filp_open(open->dfd, open->filename, &op);
	if (IS_ERR(file)) {
		/*
		 * We could hang on to this 'fd' on retrying, but seems like
		 * marginal gain for something that is now known to be a slower
		 * path. So just put it, and we'll get a new one when we retry.
		 */
		if (!fixed)
			put_unused_fd(ret);

		ret = PTR_ERR(file);
		/* only retry if RESOLVE_CACHED wasn't already set by application */
		if (ret == -EAGAIN &&
		    (!resolve_nonblock && (issue_flags & IO_URING_F_NONBLOCK)))
			return -EAGAIN;
		goto err;
	}

	if ((issue_flags & IO_URING_F_NONBLOCK) && !nonblock_set)
		file->f_flags &= ~O_NONBLOCK;

	if (!fixed)
		fd_install(ret, file);
	else
		ret = io_fixed_fd_install(req, issue_flags, file,
						open->file_slot);
err:
	putname(open->filename);
	req->flags &= ~REQ_F_NEED_CLEANUP;
	if (ret < 0)
		req_set_fail(req);
	io_req_set_res(req, ret, 0);
	return IOU_OK;
}

/**
 * io_openat - Wrapper function to handle openat system call
 * @req: Pointer to the io_kiocb structure representing the I/O request
 * @issue_flags: Flags indicating how the operation should be issued
 *
 * This function serves as a wrapper for the io_openat2 function, passing
 * the provided I/O request and issue flags to it. It simplifies the
 * handling of openat system calls within the io_uring framework.
 *
 * Return: The return value of the io_openat2 function, which typically
 * indicates success or failure of the operation.
 */
int io_openat(struct io_kiocb *req, unsigned int issue_flags)
{
	return io_openat2(req, issue_flags);
}

/**
 * io_open_cleanup - Cleans up resources associated with an io_kiocb open request.
 * @req: Pointer to the io_kiocb structure representing the request.
 *
 * This function is responsible for releasing any resources allocated for an
 * io_kiocb open request. Specifically, if a filename was allocated for the
 * request, it releases the memory associated with it using the putname function.
 */
void io_open_cleanup(struct io_kiocb *req)
{
	struct io_open *open = io_kiocb_to_cmd(req, struct io_open);

	if (open->filename)
		putname(open->filename);
}

/**
 * __io_close_fixed - Closes a fixed file descriptor in the io_uring context.
 * @ctx: Pointer to the io_uring context structure.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 * @offset: The offset representing the fixed file descriptor to be removed.
 *
 * This function removes a fixed file descriptor from the io_uring context
 * by locking the submission queue, performing the removal, and then unlocking
 * the submission queue. It ensures thread-safe access to the io_uring context
 * during the operation.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
int __io_close_fixed(struct io_ring_ctx *ctx, unsigned int issue_flags,
		     unsigned int offset)
{
	int ret;

	io_ring_submit_lock(ctx, issue_flags);
	ret = io_fixed_fd_remove(ctx, offset);
	io_ring_submit_unlock(ctx, issue_flags);

	return ret;
}

/**
 * io_close_fixed - Closes a fixed file descriptor in io_uring.
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the issue context.
 *
 * This function closes a fixed file descriptor associated with an
 * io_uring request. It retrieves the file slot from the request and
 * calls __io_close_fixed() to perform the actual closing operation.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
static inline int io_close_fixed(struct io_kiocb *req, unsigned int issue_flags)
{
	struct io_close *close = io_kiocb_to_cmd(req, struct io_close);

	return __io_close_fixed(req->ctx, issue_flags, close->file_slot - 1);
}

/**
 * io_close_prep - Prepares a close operation for io_uring.

 * This function initializes and prepares the necessary data structures
 * for handling a close operation in the io_uring context. It extracts
 * relevant information from the submission queue entry (sqe) and sets
 * up the request (req) accordingly.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
int io_close_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe)
{
	struct io_close *close = io_kiocb_to_cmd(req, struct io_close);

	if (sqe->off || sqe->addr || sqe->len || sqe->rw_flags || sqe->buf_index)
		return -EINVAL;
	if (req->flags & REQ_F_FIXED_FILE)
		return -EBADF;

	close->fd = READ_ONCE(sqe->fd);
	close->file_slot = READ_ONCE(sqe->file_index);
	if (close->file_slot && close->fd)
		return -EINVAL;

	return 0;
}

/**
 * io_close - Handles the closing of a file descriptor in io_uring.
 *
 * This function is responsible for closing a file descriptor associated
 * with an io_uring request. It supports both fixed and regular file
 * descriptors. The function performs the following steps:
 *
 * 1. If the file descriptor is fixed (indicated by close->file_slot),
 *    it delegates the operation to io_close_fixed().
 * 2. Acquires a spin lock on the files_struct to safely look up the file
 *    descriptor and ensure it is not associated with io_uring operations.
 * 3. If the file descriptor has a flush method and the operation is
 *    non-blocking, it returns -EAGAIN to indicate the operation should
 *    be retried asynchronously.
 * 4. Closes the file descriptor and releases the spin lock.
 * 5. If the operation fails, it sets the request as failed.
 * 6. Sets the result of the request and returns IOU_OK.
 *
 * Return:
 * - IOU_OK on success.
 * - Appropriate error codes (e.g., -EBADF, -EAGAIN) on failure.
 */
int io_close(struct io_kiocb *req, unsigned int issue_flags)
{
	struct files_struct *files = current->files;
	struct io_close *close = io_kiocb_to_cmd(req, struct io_close);
	struct file *file;
	int ret = -EBADF;

	if (close->file_slot) {
		ret = io_close_fixed(req, issue_flags);
		goto err;
	}

	spin_lock(&files->file_lock);
	file = files_lookup_fd_locked(files, close->fd);
	if (!file || io_is_uring_fops(file)) {
		spin_unlock(&files->file_lock);
		goto err;
	}

	/* if the file has a flush method, be safe and punt to async */
	if (file->f_op->flush && (issue_flags & IO_URING_F_NONBLOCK)) {
		spin_unlock(&files->file_lock);
		return -EAGAIN;
	}

	file = file_close_fd_locked(files, close->fd);
	spin_unlock(&files->file_lock);
	if (!file)
		goto err;

	/* No ->flush() or already async, safely close from here */
	ret = filp_close(file, current->files);
err:
	if (ret < 0)
		req_set_fail(req);
	io_req_set_res(req, ret, 0);
	return IOU_OK;
}

/**
 * io_install_fixed_fd_prep - Prepares a fixed file descriptor installation request.
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring_sqe structure containing the submission queue entry.
 *
 * This function validates and prepares a request to install a fixed file descriptor.
 * It performs the following checks:
 * - Ensures that certain fields in the submission queue entry (sqe) are not set.
 * - Verifies that the request is associated with a fixed file.
 * - Checks that the install_fd_flags in the sqe do not contain invalid flags.
 * - Ensures that the task's credentials are not overridden during the operation.
 *
 * If all checks pass, the function sets the appropriate flags for the operation,
 * defaulting to O_CLOEXEC unless the IORING_FIXED_FD_NO_CLOEXEC flag is specified.
 *
 * Return:
 * - 0 on success.
 * - -EINVAL if invalid parameters are detected.
 * - -EBADF if the request is not associated with a fixed file.
 * - -EPERM if the task's credentials are being overridden.
 */
int io_install_fixed_fd_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe)
{
	struct io_fixed_install *ifi;
	unsigned int flags;

	if (sqe->off || sqe->addr || sqe->len || sqe->buf_index ||
	    sqe->splice_fd_in || sqe->addr3)
		return -EINVAL;

	/* must be a fixed file */
	if (!(req->flags & REQ_F_FIXED_FILE))
		return -EBADF;

	flags = READ_ONCE(sqe->install_fd_flags);
	if (flags & ~IORING_FIXED_FD_NO_CLOEXEC)
		return -EINVAL;

	/* ensure the task's creds are used when installing/receiving fds */
	if (req->flags & REQ_F_CREDS)
		return -EPERM;

	/* default to O_CLOEXEC, disable if IORING_FIXED_FD_NO_CLOEXEC is set */
	ifi = io_kiocb_to_cmd(req, struct io_fixed_install);
	ifi->o_flags = O_CLOEXEC;
	if (flags & IORING_FIXED_FD_NO_CLOEXEC)
		ifi->o_flags = 0;

	return 0;
}

/**
 * io_install_fixed_fd - Installs a fixed file descriptor for an I/O request.
 * 
 * This function retrieves the fixed file descriptor installation command
 * (io_fixed_install) from the provided I/O request. It then attempts to
 * receive a file descriptor using the `receive_fd` function, passing the
 * file associated with the request, a NULL pointer for additional data, and
 * the open flags (o_flags) from the installation command.
 * 
 * If the `receive_fd` operation fails (returns a negative value), the request
 * is marked as failed using `req_set_fail`. The result of the operation is
 * then set in the request using `io_req_set_res`.
 * 
 * Returns:
 * IOU_OK - Indicates that the function executed successfully, regardless of
 *          whether the file descriptor installation succeeded or failed.
 */
int io_install_fixed_fd(struct io_kiocb *req, unsigned int issue_flags)
{
	struct io_fixed_install *ifi;
	int ret;

	ifi = io_kiocb_to_cmd(req, struct io_fixed_install);
	ret = receive_fd(req->file, NULL, ifi->o_flags);
	if (ret < 0)
		req_set_fail(req);
	io_req_set_res(req, ret, 0);
	return IOU_OK;
}
