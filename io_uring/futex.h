// SPDX-License-Identifier: GPL-2.0

#include "cancel.h"

// Prepares a futex operation in the io_kiocb request.
int io_futex_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
// Prepares a futex operation for a vectorized request in the io_kiocb request.
int io_futexv_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
// Waits for a futex operation to complete for the given request.
int io_futex_wait(struct io_kiocb *req, unsigned int issue_flags);
// Waits for a vectorized futex operation to complete for the given request.
int io_futexv_wait(struct io_kiocb *req, unsigned int issue_flags);
// Wakes up a futex operation for the given request.
int io_futex_wake(struct io_kiocb *req, unsigned int issue_flags);

#if defined(CONFIG_FUTEX)
int io_futex_cancel(struct io_ring_ctx *ctx, struct io_cancel_data *cd,
		    unsigned int issue_flags);
bool io_futex_remove_all(struct io_ring_ctx *ctx, struct io_uring_task *tctx,
			 bool cancel_all);
bool io_futex_cache_init(struct io_ring_ctx *ctx);
void io_futex_cache_free(struct io_ring_ctx *ctx);
#else
// Cancels a futex operation in the io_ring context, using cancellation data.
static inline int io_futex_cancel(struct io_ring_ctx *ctx,
				  struct io_cancel_data *cd,
				  unsigned int issue_flags)
{
	return 0;
}
// Removes all futex operations from the io_ring context for the given task context, possibly cancelin
static inline bool io_futex_remove_all(struct io_ring_ctx *ctx,
				       struct io_uring_task *tctx, bool cancel_all)
{
	return false;
}
// Initializes the futex cache for the io_ring context.
static inline bool io_futex_cache_init(struct io_ring_ctx *ctx)
{
	return false;
}
// Frees the futex cache for the io_ring context.
static inline void io_futex_cache_free(struct io_ring_ctx *ctx)
{
}
#endif
