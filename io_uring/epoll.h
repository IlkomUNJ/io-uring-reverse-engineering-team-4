// SPDX-License-Identifier: GPL-2.0

#if defined(CONFIG_EPOLL)
/*
 * Prepares an epoll control operation (add, modify, delete) by
 * extracting necessary parameters from the SQE and validating them.
 */
int io_epoll_ctl_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
/*
 * Executes the epoll control operation previously prepared.
 * Calls do_epoll_ctl with the specified epfd, fd, operation, and event.
 */
int io_epoll_ctl(struct io_kiocb *req, unsigned int issue_flags);
/*
 * Prepares an epoll wait operation by reading the maxevents and the
 * user-provided buffer pointer from the SQE.
 */
int io_epoll_wait_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
/*
 * Executes an epoll wait operation to deliver ready events to userspace.
 * Uses epoll_sendevents to fill the user buffer with events.
 */
int io_epoll_wait(struct io_kiocb *req, unsigned int issue_flags);
#endif
