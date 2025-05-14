// SPDX-License-Identifier: GPL-2.0

#include <linux/io_uring_types.h>
#include <linux/pagemap.h>

struct io_meta_state {
	u32			seed;
	struct iov_iter_state	iter_meta;
};

struct io_async_rw {
	struct iou_vec			vec;
	size_t				bytes_done;

	struct_group(clear,
		struct iov_iter			iter;
		struct iov_iter_state		iter_state;
		struct iovec			fast_iov;
		/*
		 * wpq is for buffered io, while meta fields are used with
		 * direct io
		 */
		union {
			struct wait_page_queue		wpq;
			struct {
				struct uio_meta			meta;
				struct io_meta_state		meta_state;
			};
		};
	);
};

/**
 * io_prep_read_fixed - Prepares a fixed-buffer read request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a read request
 * using a fixed buffer based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_prep_read_fixed(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_prep_write_fixed - Prepares a fixed-buffer write request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a write request
 * using a fixed buffer based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_prep_write_fixed(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_prep_readv_fixed - Prepares a fixed-buffer vectored read request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a vectored read
 * request using a fixed buffer based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_prep_readv_fixed(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_prep_writev_fixed - Prepares a fixed-buffer vectored write request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a vectored write
 * request using a fixed buffer based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_prep_writev_fixed(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_prep_readv - Prepares a vectored read request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a vectored read
 * request based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_prep_readv(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_prep_writev - Prepares a vectored write request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a vectored write
 * request based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_prep_writev(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_prep_read - Prepares a read request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a read request
 * based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_prep_read(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_prep_write - Prepares a write request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a write request
 * based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_prep_write(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_read - Handles a read operation for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function processes a read request, including preparing the request,
 * importing buffers, and performing the read operation.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_read(struct io_kiocb *req, unsigned int issue_flags);

/**
 * io_write - Handles a write operation for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function processes a write request, including preparing the request,
 * importing buffers, and performing the write operation.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_write(struct io_kiocb *req, unsigned int issue_flags);

/**
 * io_read_fixed - Handles a fixed-buffer read operation for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function processes a read request using a fixed buffer.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_read_fixed(struct io_kiocb *req, unsigned int issue_flags);

/**
 * io_write_fixed - Handles a fixed-buffer write operation for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function processes a write request using a fixed buffer.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_write_fixed(struct io_kiocb *req, unsigned int issue_flags);

/**
 * io_readv_writev_cleanup - Cleans up resources for a vectored read/write request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 *
 * This function releases resources associated with a vectored read/write
 * request, such as the iovec structure.
 */
void io_readv_writev_cleanup(struct io_kiocb *req);

/**
 * io_rw_fail - Handles a failed read/write request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 *
 * This function sets the result of a failed read/write request and marks
 * the request as failed.
 */
void io_rw_fail(struct io_kiocb *req);

/**
 * io_req_rw_complete - Completes a read/write request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @tw: Token representing the task work associated with the request.
 *
 * This function completes a read/write request by finalizing its result
 * and performing any necessary cleanup.
 */
void io_req_rw_complete(struct io_kiocb *req, io_tw_token_t tw);

/**
 * io_read_mshot_prep - Prepares a multi-shot read request.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @sqe: Pointer to the io_uring submission queue entry.
 *
 * This function initializes the necessary parameters for a multi-shot
 * read request based on the submission queue entry (SQE).
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_read_mshot_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

/**
 * io_read_mshot - Handles a multi-shot read operation for io_uring.
 *
 * @req: Pointer to the io_kiocb structure representing the request.
 * @issue_flags: Flags indicating the context in which the operation is issued.
 *
 * This function processes a multi-shot read request, allowing multiple
 * reads to be performed in a single operation.
 *
 * Returns:
 * - IOU_OK on success.
 * - Negative error code on failure.
 */
int io_read_mshot(struct io_kiocb *req, unsigned int issue_flags);

/**
 * io_rw_cache_free - Frees an asynchronous read/write structure.
 *
 * @entry: Pointer to the asynchronous read/write structure to be freed.
 *
 * This function releases the memory allocated for the asynchronous read/write
 * structure and its associated resources, such as the iovec structure.
 */
void io_rw_cache_free(const void *entry);
