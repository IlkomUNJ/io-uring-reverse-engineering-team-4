// SPDX-License-Identifier: GPL-2.0
#ifndef IOU_KBUF_H
#define IOU_KBUF_H

#include <uapi/linux/io_uring.h>
#include <linux/io_uring_types.h>

enum {
	/* ring mapped provided buffers */
	IOBL_BUF_RING	= 1,
	/* buffers are consumed incrementally rather than always fully */
	IOBL_INC	= 2,
};

struct io_buffer_list {
	/*
	 * If ->buf_nr_pages is set, then buf_pages/buf_ring are used. If not,
	 * then these are classic provided buffers and ->buf_list is used.
	 */
	union {
		struct list_head buf_list;
		struct io_uring_buf_ring *buf_ring;
	};
	__u16 bgid;

	/* below is for ring provided buffers */
	__u16 buf_nr_pages;
	__u16 nr_entries;
	__u16 head;
	__u16 mask;

	__u16 flags;

	struct io_mapped_region region;
};

struct io_buffer {
	struct list_head list;
	__u64 addr;
	__u32 len;
	__u16 bid;
	__u16 bgid;
};

enum {
	/* can alloc a bigger vec */
	KBUF_MODE_EXPAND	= 1,
	/* if bigger vec allocated, free old one */
	KBUF_MODE_FREE		= 2,
};

struct buf_sel_arg {
	struct iovec *iovs;
	size_t out_len;
	size_t max_len;
	unsigned short nr_iovs;
	unsigned short mode;
};

/**
 * This function is responsible for selecting an appropriate I/O buffer for
 * the given request. It may involve operations such as validating the request,
 * determining the buffer's size, and applying any specified flags. The selected
 * buffer is returned as a pointer to a user-space memory region.
 */
void __user *io_buffer_select(struct io_kiocb *req, size_t *len,
			      unsigned int issue_flags);
/**
 * This function is responsible for selecting an appropriate buffer for the
 * given I/O request based on the provided arguments and issue flags. It is
 * typically used in scenarios where buffer management is required for
 * efficient I/O operations.
 */
int io_buffers_select(struct io_kiocb *req, struct buf_sel_arg *arg,
		      unsigned int issue_flags);
/**
 * This function is used to inspect or retrieve information about IO buffers
 * associated with a specific IO request. It does not modify the buffers but
 * provides a mechanism to query their state or properties.
 */
int io_buffers_peek(struct io_kiocb *req, struct buf_sel_arg *arg);
/**
 * This function is responsible for releasing any memory or resources
 * associated with the registered buffers in the provided io_uring context.
 * It ensures that all buffers are properly deallocated to prevent memory leaks.
 */
void io_destroy_buffers(struct io_ring_ctx *ctx);

/**

 * This function is responsible for setting up the necessary parameters
 * to handle the removal of previously registered buffers in the io_uring
 * context. It validates the input parameters and prepares the request
 * for execution.
 */
int io_remove_buffers_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
/**
 * This function is responsible for cleaning up and removing any buffers
 * associated with the specified I/O request. The behavior of the removal
 * process may be influenced by the provided issue_flags.
 */
int io_remove_buffers(struct io_kiocb *req, unsigned int issue_flags);

/**

 * This function is responsible for setting up the necessary data structures
 * and parameters to provide buffers for io_uring operations. It processes
 * the submission queue entry (SQE) and prepares the request accordingly.
 */
int io_provide_buffers_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
/**
 * This function is responsible for setting up buffers that can be used
 * for asynchronous I/O operations. It associates the provided buffers
 * with the given request and applies the specified issue flags.
 */
int io_provide_buffers(struct io_kiocb *req, unsigned int issue_flags);

/**
 * This function associates a user-provided buffer ring with the specified
 * io_uring context. The buffer ring is used for efficient data transfer
 * between user space and kernel space. The user must ensure that the
 * provided buffer ring is properly allocated and accessible.
 */
int io_register_pbuf_ring(struct io_ring_ctx *ctx, void __user *arg);
int io_unregister_pbuf_ring(struct io_ring_ctx *ctx, void __user *arg);
int io_register_pbuf_status(struct io_ring_ctx *ctx, void __user *arg);

bool io_kbuf_recycle_legacy(struct io_kiocb *req, unsigned issue_flags);
void io_kbuf_drop_legacy(struct io_kiocb *req);

unsigned int __io_put_kbufs(struct io_kiocb *req, int len, int nbufs);
bool io_kbuf_commit(struct io_kiocb *req,
		    struct io_buffer_list *bl, int len, int nr);

struct io_mapped_region *io_pbuf_get_region(struct io_ring_ctx *ctx,
					    unsigned int bgid);

static inline bool io_kbuf_recycle_ring(struct io_kiocb *req)
{
	/*
	 * We don't need to recycle for REQ_F_BUFFER_RING, we can just clear
	 * the flag and hence ensure that bl->head doesn't get incremented.
	 * If the tail has already been incremented, hang on to it.
	 * The exception is partial io, that case we should increment bl->head
	 * to monopolize the buffer.
	 */
	if (req->buf_list) {
		req->buf_index = req->buf_list->bgid;
		req->flags &= ~(REQ_F_BUFFER_RING|REQ_F_BUFFERS_COMMIT);
		return true;
	}
	return false;
}

static inline bool io_do_buffer_select(struct io_kiocb *req)
{
	if (!(req->flags & REQ_F_BUFFER_SELECT))
		return false;
	return !(req->flags & (REQ_F_BUFFER_SELECTED|REQ_F_BUFFER_RING));
}

static inline bool io_kbuf_recycle(struct io_kiocb *req, unsigned issue_flags)
{
	if (req->flags & REQ_F_BL_NO_RECYCLE)
		return false;
	if (req->flags & REQ_F_BUFFER_SELECTED)
		return io_kbuf_recycle_legacy(req, issue_flags);
	if (req->flags & REQ_F_BUFFER_RING)
		return io_kbuf_recycle_ring(req);
	return false;
}

static inline unsigned int io_put_kbuf(struct io_kiocb *req, int len,
				       unsigned issue_flags)
{
	if (!(req->flags & (REQ_F_BUFFER_RING | REQ_F_BUFFER_SELECTED)))
		return 0;
	return __io_put_kbufs(req, len, 1);
}

static inline unsigned int io_put_kbufs(struct io_kiocb *req, int len,
					int nbufs, unsigned issue_flags)
{
	if (!(req->flags & (REQ_F_BUFFER_RING | REQ_F_BUFFER_SELECTED)))
		return 0;
	return __io_put_kbufs(req, len, nbufs);
}
#endif
