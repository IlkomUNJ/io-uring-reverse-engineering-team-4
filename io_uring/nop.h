// SPDX-License-Identifier: GPL-2.0

/**
 * This function sets up a NOP operation, which is a placeholder or dummy
 * operation that does not perform any actual I/O. It is useful for testing
 * or for cases where a no-op is required in the submission queue.
 */
int io_nop_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
/**
 * This function serves as a placeholder or a dummy operation for io_uring
 * requests. It does not perform any actual I/O operation but can be used
 * for testing or as a default handler.
 */
int io_nop(struct io_kiocb *req, unsigned int issue_flags);
