// SPDX-License-Identifier: GPL-2.0
#ifndef IORING_REGISTER_H
#define IORING_REGISTER_H

/**
 * io_eventfd_unregister - Unregisters an eventfd from the io_uring context.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 *
 * This function removes an eventfd previously registered with the io_uring
 * instance. It ensures that the eventfd is properly unlinked and any associated
 * resources are released.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_eventfd_unregister(struct io_ring_ctx *ctx);

/**
 * io_unregister_personality - Unregisters a personality from the io_uring context.
 *
 * @ctx: Pointer to the io_ring_ctx structure representing the io_uring context.
 * @id: The ID of the personality to be unregistered.
 *
 * This function removes a personality previously registered with the io_uring
 * instance. It ensures that the personality is properly unlinked and any
 * associated resources are released.
 *
 * Returns:
 * - 0 on success.
 * - Negative error code on failure.
 */
int io_unregister_personality(struct io_ring_ctx *ctx, unsigned id);

/**
 * io_uring_register_get_file - Retrieves a file pointer for a given file descriptor.
 *
 * @fd: The file descriptor to retrieve.
 * @registered: Indicates whether the file descriptor is registered via
 *              IORING_REGISTER_RING_FDS.
 *
 * This function retrieves the file pointer associated with a given file
 * descriptor. If the file descriptor is registered via IORING_REGISTER_RING_FDS,
 * it retrieves the file from the task's private array. Otherwise, it uses the
 * standard file descriptor table.
 *
 * Returns:
 * - A pointer to the file structure on success.
 * - ERR_PTR with a negative error code on failure.
 */
struct file *io_uring_register_get_file(unsigned int fd, bool registered);

#endif
