// SPDX-License-Identifier: GPL-2.0

// Prepares for the renameat operation by extracting the necessary parameters from the io_uring submission queue entry (SQE).
int io_renameat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
// Executes the renameat operation, performing the actual file renaming using the parameters set up in the preparation function.
int io_renameat(struct io_kiocb *req, unsigned int issue_flags);
// Cleans up resources allocated during the renameat operation, such as the old and new file paths.
void io_renameat_cleanup(struct io_kiocb *req);

// Prepares for the unlinkat operation by extracting the necessary parameters from the io_uring submission queue entry (SQE).
int io_unlinkat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
// Executes the unlinkat operation, which removes a file or directory specified by the parameters set up in the prepa
int io_unlinkat(struct io_kiocb *req, unsigned int issue_flags);
// Cleans up resources allocated during the unlinkat operation, such as the filename.
void io_unlinkat_cleanup(struct io_kiocb *req);

// Prepares for the mkdirat operation by extracting the necessary parameters from the io_uring submission queue entry (SQE).
int io_mkdirat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
// Executes the mkdirat operation, which creates a directory at the specified location with the provided mode.
int io_mkdirat(struct io_kiocb *req, unsigned int issue_flags);
// Cleans up resources allocated during the mkdirat operation, such as the directory name.
void io_mkdirat_cleanup(struct io_kiocb *req);

// Prepares for the symlinkat operation by extracting the necessary parameters from the io_uring submission queue entry (SQE).
int io_symlinkat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);
// Executes the symlinkat operation, which creates a symbolic link from the source to the target path.
int io_symlinkat(struct io_kiocb *req, unsigned int issue_flags);

// Prepares for the linkat operation by extracting the necessary parameters from the io_uring submission queue entry (SQE).
int io_linkat_prep(struct io_kiocb *req, const struct io_uring_sqe *sqe);

// Executes the linkat operation, which creates a hard link from the source to the target path.
int io_linkat(struct io_kiocb *req, unsigned int issue_flags);
// Cleans up resources allocated during the linkat operation, such as the old and new paths.
void io_link_cleanup(struct io_kiocb *req);
