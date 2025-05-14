#ifndef IOU_REQ_REF_H
#define IOU_REQ_REF_H

#include <linux/atomic.h>
#include <linux/io_uring_types.h>

/*
 * Shamelessly stolen from the mm implementation of page reference checking,
 * see commit f958d7b528b1 for details.
 */
/*
 * Checks if the reference count of a request is zero or close to overflowing.
 * This is used to ensure the reference count remains within valid bounds.
 */
#define req_ref_zero_or_close_to_overflow(req) \
	((unsigned int)atomic_read(&(req->refs)) + 127u <= 127u)

/*
 * Increments the reference count of a request if it is not zero.
 * Returns true if the increment was successful, false otherwise.
 */
static inline bool req_ref_inc_not_zero(struct io_kiocb *req)
{
	WARN_ON_ONCE(!(req->flags & REQ_F_REFCOUNT));
	return atomic_inc_not_zero(&req->refs);
}

/*
 * Decrements the reference count of a request atomically and checks if it reached zero.
 * Returns true if the reference count is zero, false otherwise.
 */
static inline bool req_ref_put_and_test_atomic(struct io_kiocb *req)
{
	WARN_ON_ONCE(!(data_race(req->flags) & REQ_F_REFCOUNT));
	WARN_ON_ONCE(req_ref_zero_or_close_to_overflow(req));
	return atomic_dec_and_test(&req->refs);
}

/*
 * Decrements the reference count of a request and checks if it reached zero.
 * Returns true if the reference count is zero, false otherwise.
 */
static inline bool req_ref_put_and_test(struct io_kiocb *req)
{
	if (likely(!(req->flags & REQ_F_REFCOUNT)))
		return true;

	WARN_ON_ONCE(req_ref_zero_or_close_to_overflow(req));
	return atomic_dec_and_test(&req->refs);
}

/*
 * Increments the reference count of a request.
 * Ensures the reference count is valid and not close to overflowing.
 */
static inline void req_ref_get(struct io_kiocb *req)
{
	WARN_ON_ONCE(!(req->flags & REQ_F_REFCOUNT));
	WARN_ON_ONCE(req_ref_zero_or_close_to_overflow(req));
	atomic_inc(&req->refs);
}

/*
 * Decrements the reference count of a request.
 * Ensures the reference count is valid and not close to overflowing.
 */
static inline void req_ref_put(struct io_kiocb *req)
{
	WARN_ON_ONCE(!(req->flags & REQ_F_REFCOUNT));
	WARN_ON_ONCE(req_ref_zero_or_close_to_overflow(req));
	atomic_dec(&req->refs);
}

/*
 * Sets the reference count of a request to a specified value.
 * If the request does not already have a reference count, it initializes it.
 */
static inline void __io_req_set_refcount(struct io_kiocb *req, int nr)
{
	if (!(req->flags & REQ_F_REFCOUNT))
	{
		req->flags |= REQ_F_REFCOUNT;
		atomic_set(&req->refs, nr);
	}
}

/*
 * Initializes the reference count of a request to 1.
 * Ensures the request is properly set up for reference counting.
 */
static inline void io_req_set_refcount(struct io_kiocb *req)
{
	__io_req_set_refcount(req, 1);
}

#endif
