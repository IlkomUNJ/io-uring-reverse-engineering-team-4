// SPDX-License-Identifier: GPL-2.0
#ifndef IOU_FILE_TABLE_H
#define IOU_FILE_TABLE_H

#include <linux/file.h>
#include <linux/io_uring_types.h>
#include "rsrc.h"

/*
 * Allocates memory for file tables and initializes the bitmap for tracking file descriptors.
 * The function takes in the io_ring_ctx context, a pointer to the io_file_table, and the number
 * of files to be allocated.
 */
bool io_alloc_file_tables(struct io_ring_ctx *ctx, struct io_file_table *table, unsigned nr_files);
/*
 * Frees the memory allocated for file tables and cleans up any associated resources.
 * This function takes in the io_ring_ctx context and a pointer to the io_file_table.
 */
void io_free_file_tables(struct io_ring_ctx *ctx, struct io_file_table *table);

/*
 * Installs a fixed file descriptor at a specified slot in the file table.
 * Takes in the request (`io_kiocb`), issue flags, the file to be installed, and the file slot index.
 * Returns an integer indicating success or failure.
 */
int io_fixed_fd_install(struct io_kiocb *req, unsigned int issue_flags,
			struct file *file, unsigned int file_slot);
/*
 * Helper function that installs a fixed file descriptor into a specified slot,
 * potentially allocating the slot if needed.
 * Takes the io_ring_ctx, the file to be installed, and the slot index.
 */
int __io_fixed_fd_install(struct io_ring_ctx *ctx, struct file *file,
				unsigned int file_slot);
/*
 * Removes a fixed file descriptor from the file table at a specified offset.
 * Takes the io_ring_ctx context and the offset index to remove the file descriptor.
 * Returns an integer indicating success or failure.
 */
int io_fixed_fd_remove(struct io_ring_ctx *ctx, unsigned int offset);

/*
 * Registers a range of file descriptors to be allocated.
 * Takes the io_ring_ctx context and a user-provided structure specifying the allocation range.
 */
int io_register_file_alloc_range(struct io_ring_ctx *ctx,
				 struct io_uring_file_index_range __user *arg);

/*
 * Retrieves the flags for a given file descriptor.
 * This function takes the file pointer as input and returns the file descriptor flags.
 */
io_req_flags_t io_file_get_flags(struct file *file);

/*
 * Clears the specified bit in the bitmap for the file table, marking the slot as available.
 * Updates the allocation hint to point to the next available slot.
 */
static inline void io_file_bitmap_clear(struct io_file_table *table, int bit)
{
	WARN_ON_ONCE(!test_bit(bit, table->bitmap));
	__clear_bit(bit, table->bitmap);
	table->alloc_hint = bit;
}

/*
 * Sets the specified bit in the bitmap for the file table, marking the slot as in use.
 * Updates the allocation hint to point to the next available slot.
 */
static inline void io_file_bitmap_set(struct io_file_table *table, int bit)
{
	WARN_ON_ONCE(test_bit(bit, table->bitmap));
	__set_bit(bit, table->bitmap);
	table->alloc_hint = bit + 1;
}

#define FFS_NOWAIT		0x1UL
#define FFS_ISREG		0x2UL
#define FFS_MASK		~(FFS_NOWAIT|FFS_ISREG)

/*
 * Extracts the flags from a resource node, shifting the file pointer to indicate properties
 * like whether the file descriptor supports non-blocking operations.
 */
static inline unsigned int io_slot_flags(struct io_rsrc_node *node)
{

	return (node->file_ptr & ~FFS_MASK) << REQ_F_SUPPORT_NOWAIT_BIT;
}

/*
 * Retrieves the file pointer from a resource node by masking out non-file-related data.
 * This function returns the actual file descriptor associated with the node.
 */
static inline struct file *io_slot_file(struct io_rsrc_node *node)
{
	return (struct file *)(node->file_ptr & FFS_MASK);
}

/*
 * Sets a fixed file descriptor and its flags in the specified resource node.
 * The file pointer and flags are packed into the node's file_ptr.
 */
static inline void io_fixed_file_set(struct io_rsrc_node *node,
				     struct file *file)
{
	node->file_ptr = (unsigned long)file |
		(io_file_get_flags(file) >> REQ_F_SUPPORT_NOWAIT_BIT);
}

/*
 * Sets the allocation range for file descriptors within the io_ring_ctx context.
 * This specifies the start and end of the file descriptor allocation range and updates the allocation hint.
 */
static inline void io_file_table_set_alloc_range(struct io_ring_ctx *ctx,
						 unsigned off, unsigned len)
{
	ctx->file_alloc_start = off;
	ctx->file_alloc_end = off + len;
	ctx->file_table.alloc_hint = ctx->file_alloc_start;
}

#endif
