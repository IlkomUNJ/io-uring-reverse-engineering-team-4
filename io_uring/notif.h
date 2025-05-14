// SPDX-License-Identifier: GPL-2.0

#include <linux/net.h>
#include <linux/uio.h>
#include <net/sock.h>
#include <linux/nospec.h>

#include "rsrc.h"

#define IO_NOTIF_UBUF_FLAGS	(SKBFL_ZEROCOPY_FRAG | SKBFL_DONT_ORPHAN)
#define IO_NOTIF_SPLICE_BATCH	32

struct io_notif_data {
	struct file		*file;
	struct ubuf_info	uarg;

	struct io_notif_data	*next;
	struct io_notif_data	*head;

	unsigned		account_pages;
	bool			zc_report;
	bool			zc_used;
	bool			zc_copied;
};

/**
 * This function is responsible for allocating and initializing a notification
 * request (`io_kiocb`) within the given io_uring context (`io_ring_ctx`).
 */
struct io_kiocb *io_alloc_notif(struct io_ring_ctx *ctx);

/**
 * This function is called to finalize the transmission of a user buffer.
 * It performs necessary cleanup and updates based on the success or failure
 * of the transmission.
 */
void io_tx_ubuf_complete(struct sk_buff *skb, struct ubuf_info *uarg,
			 bool success);

/**
 * This inline function takes a pointer to an io_kiocb notification and
 * converts it to a pointer to an io_notif_data structure. It uses the
 * io_kiocb_to_cmd macro to perform the conversion.
 */
static inline struct io_notif_data *io_notif_to_data(struct io_kiocb *notif)
{
	return io_kiocb_to_cmd(notif, struct io_notif_data);
}

static inline void io_notif_flush(struct io_kiocb *notif)
	__must_hold(&notif->ctx->uring_lock)
{
	struct io_notif_data *nd = io_notif_to_data(notif);

	io_tx_ubuf_complete(NULL, &nd->uarg, true);
}

static inline int io_notif_account_mem(struct io_kiocb *notif, unsigned len)
{
	struct io_ring_ctx *ctx = notif->ctx;
	struct io_notif_data *nd = io_notif_to_data(notif);
	unsigned nr_pages = (len >> PAGE_SHIFT) + 2;
	int ret;

	if (ctx->user) {
		ret = __io_account_mem(ctx->user, nr_pages);
		if (ret)
			return ret;
		nd->account_pages += nr_pages;
	}
	return 0;
}
