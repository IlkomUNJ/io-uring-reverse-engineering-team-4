// SPDX-License-Identifier: GPL-2.0

/**
 * __io_close_fixed - Closes a file descriptor using a fixed file table entry.
 */
int __io_close_fixed(struct io_ring_ctx *ctx, unsigned int issue_flags,
		     unsigned int offset);

/**
 * Prepares an openat operation for io_uring.
 * This function sets up the necessary parameters for an openat system call
 * to be executed via io_uring. It ensures that the request is properly
 * initialized and ready for submission.
 */
int io_openat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);


/**
 * io_openat - Handles the openat system call for asynchronous I/O operations.
 *
 * This function is responsible for processing an asynchronous openat system call,
 * which opens a file relative to a directory file descriptor. It utilizes io_uring
 * to manage the operation efficiently.
 */
int io_openat(struct io_kiocb *req, unsigned int issue_flags);


/**
 * This function is responsible for performing cleanup operations for a given
 * I/O request represented by the `io_kiocb` structure. It ensures that any
 * resources allocated during the I/O operation are properly released.
 */
void io_open_cleanup(struct io_kiocb *req);

/**
 * This function sets up the necessary parameters and state for an openat2
 * system call to be executed as part of an io_uring request. The openat2
 * system call is used to open a file descriptor relative to a directory
 * file descriptor with extended options.
 */
int io_openat2_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);


/**
 * Handles the openat2 system call for asynchronous I/O operations.
 *
 * This function is responsible for processing an openat2 request within the
 * io_uring framework. It performs the necessary setup and execution of the
 * openat2 system call, which allows opening a file relative to a directory
 * file descriptor with extended options.
 */
int io_openat2(struct io_kiocb *req, unsigned int issue_flags);

/**
 * io_close_prep - Prepares a close operation for io_uring.
 *
 * This function initializes the necessary parameters for a close system call
 * to be executed via io_uring. It ensures that the request is properly set up
 * before submission.
 */
 int io_close_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

 /**
  * io_close - Handles the close system call for asynchronous I/O operations.
  *
  * This function processes an asynchronous close system call, which closes
  * a file descriptor. It utilizes io_uring to manage the operation efficiently.
  */
 int io_close(struct io_kiocb *req, unsigned int issue_flags);
 
 /**
  * io_install_fixed_fd_prep - Prepares a fixed file descriptor installation for io_uring.
  *
  * This function sets up the parameters required to install a fixed file descriptor
  * into the io_uring fixed file table. It ensures the request is ready for submission.
  */
 int io_install_fixed_fd_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
 
 /**
  * io_install_fixed_fd - Installs a fixed file descriptor for asynchronous I/O operations.
  *
  * This function handles the installation of a file descriptor into the fixed file table
  * used by io_uring. It allows efficient reuse of file descriptors for I/O operations.
  */
 int io_install_fixed_fd(struct io_kiocb *req, unsigned int issue_flags);
