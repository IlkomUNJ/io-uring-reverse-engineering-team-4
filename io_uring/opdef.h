// SPDX-License-Identifier: GPL-2.0
#ifndef IOU_OP_DEF_H
#define IOU_OP_DEF_H

/*
 * Operation handler definition for each io_uring opcode.
 * Describes behavior and capabilities of each supported operation.
 */
struct io_issue_def {
	/* needs req->file assigned */
	unsigned		needs_file : 1;
	/* should block plug */
	unsigned		plug : 1;
	/* supports ioprio */
	unsigned		ioprio : 1;
	/* supports iopoll */
	unsigned		iopoll : 1;
	/* op supports buffer selection */
	unsigned		buffer_select : 1;
	/* hash wq insertion if file is a regular file */
	unsigned		hash_reg_file : 1;
	/* unbound wq insertion if file is a non-regular file */
	unsigned		unbound_nonreg_file : 1;
	/* set if opcode supports polled "wait" */
	unsigned		pollin : 1;
	unsigned		pollout : 1;
	unsigned		poll_exclusive : 1;
	/* skip auditing */
	unsigned		audit_skip : 1;
	/* have to be put into the iopoll list */
	unsigned		iopoll_queue : 1;
	/* vectored opcode, set if 1) vectored, and 2) handler needs to know */
	unsigned		vectored : 1;

	/* size of async data needed, if any */
	unsigned short		async_size;

	/* function pointer for issuing the operation */
	int (*issue)(struct io_kiocb *, unsigned int);
	/* function pointer for preparing the operation */
	int (*prep)(struct io_kiocb *, const struct io_uring_sqe *);
};

/*
 * Cold path definition for io_uring operations.
 * Includes cleanup and failure handling for each opcode.
 */
struct io_cold_def {
	const char		*name;

	/* optional cleanup handler */
	void (*cleanup)(struct io_kiocb *);
	/* optional failure handler */
	void (*fail)(struct io_kiocb *);
};

/*
 * Array of operation definitions, indexed by opcode.
 */
extern const struct io_issue_def io_issue_defs[];

/*
 * Array of cold path metadata for each operation.
 */
extern const struct io_cold_def io_cold_defs[];

/*
 * Return true if the given opcode is supported by io_uring.
 */
bool io_uring_op_supported(u8 opcode);

/*
 * Initialize operation tables and validate definitions.
 */
void io_uring_optable_init(void);

#endif
