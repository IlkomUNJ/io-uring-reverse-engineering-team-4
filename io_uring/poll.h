// SPDX-License-Identifier: GPL-2.0

#include <linux/io_uring_types.h>

#define IO_POLL_ALLOC_CACHE_MAX 32

enum {
	IO_APOLL_OK,
	IO_APOLL_ABORTED,
	IO_APOLL_READY
};

struct io_poll {
	struct file			*file;
	struct wait_queue_head		*head;
	__poll_t			events;
	int				retries;
	struct wait_queue_entry		wait;
};

struct async_poll {
	struct io_poll		poll;
	struct io_poll		*double_poll;
};

/*
 * Must only be called inside issue_flags & IO_URING_F_MULTISHOT, or
 * potentially other cases where we already "own" this poll request.
 */
static inline void io_poll_multishot_retry(struct io_kiocb *req)
{
	atomic_inc(&req->poll_refs);
}
 
 /*
  * Prepares a poll add request by initializing the necessary fields in the request.
  * Returns 0 on success or an error code on failure.
  */
int io_poll_add_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
 
 /*
  * Adds a poll request to monitor events on a file descriptor.
  * Returns 0 on success or an error code on failure.
  */
int io_poll_add(struct io_kiocb *req, unsigned int issue_flags);
 
 /*
  * Prepares a poll remove request by validating the request and initializing fields.
  * Returns 0 on success or an error code on failure.
  */
int io_poll_remove_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
 
 /*
  * Removes a poll request, stopping the monitoring of events on a file descriptor.
  * Returns 0 on success or an error code on failure.
  */
int io_poll_remove(struct io_kiocb *req, unsigned int issue_flags);
 
struct io_cancel_data;
/*
  * Cancels a poll request based on the provided cancel data.
  * Returns 0 on success or an error code on failure.
  */
int io_poll_cancel(struct io_ring_ctx *ctx, struct io_cancel_data *cd,
			unsigned issue_flags);
 
 /*
  * Arms a poll handler to monitor events for a specific request.
  * Returns 0 on success or an error code on failure.
  */
int io_arm_poll_handler(struct io_kiocb *req, unsigned issue_flags);
 
 /*
  * Removes all poll requests associated with a specific task context.
  * If cancel_all is true, all requests are canceled.
  * Returns true if any requests were removed, false otherwise.
  */
bool io_poll_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx,
			bool cancel_all);
 
 /*
  * Handles poll-related tasks for a specific request.
  * This function is executed as part of the task work mechanism.
  */
void io_poll_task_func(struct io_kiocb *req, io_tw_token_t tw);
