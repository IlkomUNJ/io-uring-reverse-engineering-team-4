// SPDX-License-Identifier: GPL-2.0

/*
 * Prepares a madvise operation based on the submission queue entry (SQE).
 * 'madvise' is a system call that gives advice to the kernel about how to handle memory in a given range.
 * Returns 0 on success, or a negative error code.
 */
int io_madvise_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
/*
 * Submits the prepared madvise request to actually be executed.
 * 'issue_flags' can modify how the request is handled (e.g., sync/async).
 */
int io_madvise(struct io_kiocb *req, unsigned int issue_flags);

/*
 * Prepares an fadvise operation based on the SQE.
 * 'fadvise' gives the kernel hints about how a file will be accessed (e.g., random/sequential reads),
 * allowing the kernel to optimize file caching behavior.
 */
int io_fadvise_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
/*
 * Submits the prepared fadvise request for execution.
 * Like madvise, 'issue_flags' may modify how the request is processed.
 */
int io_fadvise(struct io_kiocb *req, unsigned int issue_flags);
