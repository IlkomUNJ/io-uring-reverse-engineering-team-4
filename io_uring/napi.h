/* SPDX-License-Identifier: GPL-2.0 */

#ifndef IOU_NAPI_H
#define IOU_NAPI_H

#include <linux/kernel.h>
#include <linux/io_uring.h>
#include <net/busy_poll.h>

#ifdef CONFIG_NET_RX_BUSY_POLL

void io_napi_init(struct io_ring_ctx *ctx);
/**
 * This function is responsible for releasing any memory or resources
 * allocated for the specified io_uring context (`ctx`). It ensures
 * proper cleanup to prevent resource leaks.
 */

/**
 * This function is responsible for releasing any memory or resources
 * allocated for the specified io_ring_ctx structure. It ensures proper
 * cleanup to prevent resource leaks.
 */
 void io_napi_free(struct io_ring_ctx *ctx);

int io_register_napi(struct io_ring_ctx *ctx, void __user *arg);
/**
 * This function is responsible for removing a napi instance associated with the
 * provided io_uring context. It ensures that the napi instance is properly
 * unregistered and any associated resources are released.
 */
int io_unregister_napi(struct io_ring_ctx *ctx, void __user *arg);

/**
 * This function is used to link a specific NAPI identifier to an io_uring
 * context, enabling the integration of NAPI-based networking with io_uring.
 * It ensures that the napi_id is properly registered within the context.
 */
int __io_napi_add_id(struct io_ring_ctx *ctx, unsigned int napi_id);

void __io_napi_busy_loop(struct io_ring_ctx *ctx, struct io_wait_queue *iowq);
int io_napi_sqpoll_busy_poll(struct io_ring_ctx *ctx);

static inline bool io_napi(struct io_ring_ctx *ctx)
{
	return !list_empty(&ctx->napi_list);
}

static inline void io_napi_busy_loop(struct io_ring_ctx *ctx,
				     struct io_wait_queue *iowq)
{
	if (!io_napi(ctx))
		return;
	__io_napi_busy_loop(ctx, iowq);
}

/*
 * io_napi_add() - Add napi id to the busy poll list
 * @req: pointer to io_kiocb request
 *
 * Add the napi id of the socket to the napi busy poll list and hash table.
 */
static inline void io_napi_add(struct io_kiocb *req)
{
	struct io_ring_ctx *ctx = req->ctx;
	struct socket *sock;

	if (READ_ONCE(ctx->napi_track_mode) != IO_URING_NAPI_TRACKING_DYNAMIC)
		return;

	sock = sock_from_file(req->file);
	if (sock && sock->sk)
		__io_napi_add_id(ctx, READ_ONCE(sock->sk->sk_napi_id));
}

#else

static inline void io_napi_init(struct io_ring_ctx *ctx)
{
}
static inline void io_napi_free(struct io_ring_ctx *ctx)
{
}
static inline int io_register_napi(struct io_ring_ctx *ctx, void __user *arg)
{
	return -EOPNOTSUPP;
}
static inline int io_unregister_napi(struct io_ring_ctx *ctx, void __user *arg)
{
	return -EOPNOTSUPP;
}
static inline bool io_napi(struct io_ring_ctx *ctx)
{
	return false;
}
static inline void io_napi_add(struct io_kiocb *req)
{
}
static inline void io_napi_busy_loop(struct io_ring_ctx *ctx,
				     struct io_wait_queue *iowq)
{
}
static inline int io_napi_sqpoll_busy_poll(struct io_ring_ctx *ctx)
{
	return 0;
}
#endif /* CONFIG_NET_RX_BUSY_POLL */

#endif
