// SPDX-License-Identifier: GPL-2.0
#ifndef IORING_CANCEL_H
#define IORING_CANCEL_H

#include <linux/io_uring_types.h>

/*
 * Struct used to store data for canceling I/O requests. It includes the
 * target io_uring context, identifier for the request (data or file), opcode,
 * flags to control behavior (e.g., cancel all), and a sequence number.
 */
struct io_cancel_data {
	struct io_ring_ctx *ctx;
	union {
		u64 data;
		struct file *file;
	};
	u8 opcode;
	u32 flags;
	int seq;
};

/*
 * Prepares an async cancel request based on the submission queue entry (SQE).
 */
int io_async_cancel_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
/*
 * Executes cancellation of an asynchronous request.
 */
int io_async_cancel(struct io_kiocb *req, unsigned int issue_flags);

/*
 * Attempts to cancel a request asynchronously, given the task context and
 * cancel data. Used for cancellation by task.
 */
int io_try_cancel(struct io_uring_task *tctx, struct io_cancel_data *cd,
		  unsigned int issue_flags);

/*
 * Cancels a request synchronously using data passed from userspace.
 */
int io_sync_cancel(struct io_ring_ctx *ctx, void __user *arg);

/*
 * Checks if a given request matches the provided cancel data.
 */
bool io_cancel_req_match(struct io_kiocb *req, struct io_cancel_data *cd);

/*
 * Cancels all requests in a list that match the given task context or all,
 * depending on the `cancel_all` flag. The `cancel` function is called on each
 * matched request.
 */
bool io_cancel_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx,
			  struct hlist_head *list, bool cancel_all,
			  bool (*cancel)(struct io_kiocb *));


/*
 * Cancels matching requests from a list based on cancel data. If the
 * IORING_ASYNC_CANCEL_ALL flag is set, continues canceling all matches;
 * otherwise, cancels the first match only.
 */
int io_cancel_remove(struct io_ring_ctx *ctx, struct io_cancel_data *cd,
		     unsigned int issue_flags, struct hlist_head *list,
		     bool (*cancel)(struct io_kiocb *));

/*
 * Ensures that a cancellation sequence is only matched once per request.
 * Prevents duplicate processing for the same sequence.
 */
static inline bool io_cancel_match_sequence(struct io_kiocb *req, int sequence)
{
	if (req->cancel_seq_set && sequence == req->work.cancel_seq)
		return true;

	req->cancel_seq_set = true;
	req->work.cancel_seq = sequence;
	return false;
}

#endif
