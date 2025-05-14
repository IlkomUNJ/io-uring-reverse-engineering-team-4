// SPDX-License-Identifier: GPL-2.0

/**
 * This function is used to perform a message ring operation in io_uring, ensuring
 * synchronization between the producer and consumer of the ring. The SQE provided
 * should be properly initialized before calling this function.
 */
int io_uring_sync_msg_ring(struct io_uring_sqe *sqe);
/**
 * Prepares a message ring operation for the specified request.
 */
int io_msg_ring_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
/**
 * This function processes a message ring request, which allows communication
 * between io_uring instances or between different parts of an application
 * using io_uring. It performs the necessary actions based on the provided
 * request and issue flags.
 */
int io_msg_ring(struct io_kiocb *req, unsigned int issue_flags);
/**
 * This function is responsible for releasing any resources or performing any
 * necessary cleanup operations associated with the specified message ring request.
 * It ensures that the request is properly finalized and no memory leaks or
 * dangling references remain.
 */
void io_msg_ring_cleanup(struct io_kiocb *req);
