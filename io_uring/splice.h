// SPDX-License-Identifier: GPL-2.0

/**
 * io_tee_prep - Prepares a tee request for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function prepares a tee operation, which duplicates data from one
 * pipe to another without consuming it. It validates the input parameters
 * and initializes the request.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_tee_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_tee - Handles a tee operation for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function processes a tee operation, which duplicates data from one
 * pipe to another without consuming it. It performs the operation and sets
 * the result in the request.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_tee(struct io_kiocb *req, unsigned int issue_flags);

/**
 * io_splice_cleanup - Cleans up resources for a splice request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 *
 * This function releases resources associated with a splice request,
 * such as decrementing the reference count of the resource node.
 */
void io_splice_cleanup(struct io_kiocb *req);

/**
 * io_splice_prep - Prepares a splice request for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function prepares a splice operation, which transfers data between
 * two file descriptors. It initializes the necessary parameters for the
 * operation.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_splice_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_splice - Handles a splice operation for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function processes a splice operation, which transfers data between
 * two file descriptors. It performs the operation and sets the result in
 * the request.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_splice(struct io_kiocb *req, unsigned int issue_flags);