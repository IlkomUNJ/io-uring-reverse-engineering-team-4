// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/namei.h>
#include <linux/io_uring.h>
#include <linux/splice.h>

#include <uapi/linux/io_uring.h>

#include "io_uring.h"
#include "splice.h"

struct io_splice {
	struct file			*file_out;
	loff_t				off_out;
	loff_t				off_in;
	u64				len;
	int				splice_fd_in;
	unsigned int			flags;
	struct io_rsrc_node		*rsrc_node;
};

/**
 * __io_splice_prep - Prepares a splice request for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a splice request
 * based on the submission queue entry (SQE). It validates the input flags
 * and sets up the splice operation.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
static int __io_splice_prep(struct io_kiocb *req,
			    const struct io_uring_sqe *sqe)
{
	struct io_splice *sp = io_kiocb_to_cmd(req, struct io_splice);
	unsigned int valid_flags = SPLICE_F_FD_IN_FIXED | SPLICE_F_ALL;

	sp->len = READ_ONCE(sqe->len);
	sp->flags = READ_ONCE(sqe->splice_flags);
	if (unlikely(sp->flags & ~valid_flags))
		return -EINVAL;
	sp->splice_fd_in = READ_ONCE(sqe->splice_fd_in);
	sp->rsrc_node = NULL;
	req->flags |= REQ_F_FORCE_ASYNC;
	return 0;
}

/**
 * io_tee_prep - Prepares a tee request for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function prepares a tee operation, which duplicates data from one
 * pipe to another without consuming it. It validates the input parameters
 * and initializes the request.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_tee_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe)
{
	if (READ_ONCE(sqe->splice_off_in) || READ_ONCE(sqe->off))
		return -EINVAL;
	return __io_splice_prep(req, sqe);
}

/**
 * io_splice_cleanup - Cleans up resources for a splice request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 *
 * This function releases resources associated with a splice request,
 * such as decrementing the reference count of the resource node.
 */
void io_splice_cleanup(struct io_kiocb *req)
{
	struct io_splice *sp = io_kiocb_to_cmd(req, struct io_splice);

	if (sp->rsrc_node)
		io_put_rsrc_node(req->ctx, sp->rsrc_node);
}

/**
 * io_splice_get_file - Retrieves the input file for a splice operation.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function retrieves the input file for a splice operation. If the
 * SPLICE_F_FD_IN_FIXED flag is set, it retrieves the file from the fixed
 * file table. Otherwise, it retrieves the file using the normal file
 * descriptor table.
 *
 * Returns:
 * - Pointer to the file structure on success.
 * - NULL on failure.
 */
static struct file *io_splice_get_file(struct io_kiocb *req,
				       unsigned int issue_flags)
{
	struct io_splice *sp = io_kiocb_to_cmd(req, struct io_splice);
	struct io_ring_ctx *ctx = req->ctx;
	struct io_rsrc_node *node;
	struct file *file = NULL;

	if (!(sp->flags & SPLICE_F_FD_IN_FIXED))
		return io_file_get_normal(req, sp->splice_fd_in);

	io_ring_submit_lock(ctx, issue_flags);
	node = io_rsrc_node_lookup(&ctx->file_table.data, sp->splice_fd_in);
	if (node) {
		node->refs++;
		sp->rsrc_node = node;
		file = io_slot_file(node);
		req->flags |= REQ_F_NEED_CLEANUP;
	}
	io_ring_submit_unlock(ctx, issue_flags);
	return file;
}

/**
 * io_tee - Handles a tee operation for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function processes a tee operation, which duplicates data from one
 * pipe to another without consuming it. It performs the operation and sets
 * the result in the request.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_tee(struct io_kiocb *req, unsigned int issue_flags)
{
	struct io_splice *sp = io_kiocb_to_cmd(req, struct io_splice);
	struct file *out = sp->file_out;
	unsigned int flags = sp->flags & ~SPLICE_F_FD_IN_FIXED;
	struct file *in;
	ssize_t ret = 0;

	WARN_ON_ONCE(issue_flags & IO_URING_F_NONBLOCK);

	in = io_splice_get_file(req, issue_flags);
	if (!in) {
		ret = -EBADF;
		goto done;
	}

	if (sp->len)
		ret = do_tee(in, out, sp->len, flags);

	if (!(sp->flags & SPLICE_F_FD_IN_FIXED))
		fput(in);
done:
	if (ret != sp->len)
		req_set_fail(req);
	io_req_set_res(req, ret, 0);
	return IOU_OK;
}

/**
 * io_splice_prep - Prepares a splice request for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function prepares a splice operation, which transfers data between
 * two file descriptors. It initializes the necessary parameters for the
 * operation.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_splice_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe)
{
	struct io_splice *sp = io_kiocb_to_cmd(req, struct io_splice);

	sp->off_in = READ_ONCE(sqe->splice_off_in);
	sp->off_out = READ_ONCE(sqe->off);
	return __io_splice_prep(req, sqe);
}

/**
 * io_splice - Handles a splice operation for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function processes a splice operation, which transfers data between
 * two file descriptors. It performs the operation and sets the result in
 * the request.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_splice(struct io_kiocb *req, unsigned int issue_flags)
{
	struct io_splice *sp = io_kiocb_to_cmd(req, struct io_splice);
	struct file *out = sp->file_out;
	unsigned int flags = sp->flags & ~SPLICE_F_FD_IN_FIXED;
	loff_t *poff_in, *poff_out;
	struct file *in;
	ssize_t ret = 0;

	WARN_ON_ONCE(issue_flags & IO_URING_F_NONBLOCK);

	in = io_splice_get_file(req, issue_flags);
	if (!in) {
		ret = -EBADF;
		goto done;
	}

	poff_in = (sp->off_in == -1) ? NULL : &sp->off_in;
	poff_out = (sp->off_out == -1) ? NULL : &sp->off_out;

	if (sp->len)
		ret = do_splice(in, poff_in, out, poff_out, sp->len, flags);

	if (!(sp->flags & SPLICE_F_FD_IN_FIXED))
		fput(in);
done:
	if (ret != sp->len)
		req_set_fail(req);
	io_req_set_res(req, ret, 0);
	return IOU_OK;
}
