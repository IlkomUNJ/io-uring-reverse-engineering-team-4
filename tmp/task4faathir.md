### `sqpoll.h`

* `io_sq_offload_create` – Creates and starts a submission polling (SQPOLL) offload thread if required.
* `io_sq_thread_finish` – Detaches a context from its SQ thread and updates idle state.
* `io_sq_thread_stop` – Signals the SQPOLL thread to stop and waits for its termination.
* `io_sq_thread_park` – Requests the SQ thread to park (sleep mode).
* `io_sq_thread_unpark` – Wakes the SQ thread from a parked (sleep) state.
* `io_put_sq_data` – Decreases refcount and frees SQ thread data if no longer used.
* `io_sqpoll_wait_sq` – Blocks until space is available in the submission queue.
* `io_sqpoll_wq_cpu_affinity` – Sets CPU affinity for the SQ thread if supported.

### `sqpoll.c`

* `io_sq_thread_unpark` – Wakes the SQ thread and clears the park request.
* `io_sq_thread_park` – Parks the SQ thread and wakes it if already running.
* `io_sq_thread_stop` – Signals the SQ thread to exit and waits for completion.
* `io_put_sq_data` – Cleans up SQ data when no contexts are using it.
* `io_sqd_update_thread_idle` – Updates idle timeout value based on contexts.
* `io_sq_thread_finish` – Cleans up SQ thread association for a context.
* `io_attach_sq_data` – Attaches to an existing SQPOLL thread after validation.
* `io_get_sq_data` – Gets or creates SQPOLL thread data as needed.
* `io_sqd_events_pending` – Checks if park or stop events are pending for SQ thread.
* `__io_sq_thread` – Executes SQ submission and polling for a context.
* `io_sqd_handle_event` – Handles control events for the SQ thread (e.g., stop, park).
* `io_sq_tw` – Runs task\_work queue for the current thread.
* `io_sq_tw_pending` – Returns whether task\_work is pending.
* `io_sq_update_worktime` – Tracks CPU time used by the SQ thread.
* `io_sq_thread` – Main loop of the SQ thread: handles polling, submission, sleeping.
* `io_sqpoll_wait_sq` – Waits for room in the SQ; blocks if queue is full.
* `io_sq_offload_create` – Initializes SQPOLL thread if setup parameters require it.
* `io_sqpoll_wq_cpu_affinity` – Assigns SQ thread to specific CPUs per user settings.

### `statx.h`

* `io_statx_prep` – Prepares a statx request by parsing SQE fields and resolving the filename.
* `io_statx` – Executes the statx system call and stores the result.
* `io_statx_cleanup` – Frees resources used by the statx request, like the filename.

### `statx.c`

* `io_statx_prep` – Initializes a statx operation from io\_uring submission parameters.
* `io_statx` – Performs the actual statx syscall with prepared inputs.
* `io_statx_cleanup` – Cleans up any allocated memory or structures used in statx.

### `sync.h`

* `io_sfr_prep` – Prepares a `sync_file_range` operation by extracting offset, length, and flags from the SQE.
* `io_sync_file_range` – Performs a `sync_file_range()` call to flush a file range to disk.
* `io_fsync_prep` – Prepares an `fsync` operation by validating and reading the flags, offset, and length.
* `io_fsync` – Executes an `fsync` or `fdatasync` syscall depending on flags to flush file metadata/data.
* `io_fallocate_prep` – Prepares a `fallocate` operation by reading offset, length, and allocation mode.
* `io_fallocate` – Performs space allocation with `vfs_fallocate()` and notifies file modification.

### `sync.c`

* `io_sfr_prep` – Extracts parameters for `sync_file_range` from SQE and marks the request as async.
* `io_sync_file_range` – Issues the actual `sync_file_range()` syscall to sync a file region.
* `io_fsync_prep` – Parses and validates parameters for an `fsync` or `fdatasync` operation.
* `io_fsync` – Calls `vfs_fsync_range()` to flush data and optionally metadata to stable storage.
* `io_fallocate_prep` – Gathers offset, length, and mode for a `fallocate` call from the SQE.
* `io_fallocate` – Executes a `fallocate` syscall to allocate file space and triggers fsnotify if successful.

### `tctx.h`

* `io_uring_alloc_task_context` – Allocates and initializes a per-task `io_uring` context (`io_uring_task`) with workqueue support.
* `io_uring_del_tctx_node` – Removes a specific `io_tctx_node` from a task's context based on the provided index.
* `__io_uring_add_tctx_node` – Associates a task with a `io_ring_ctx` if not already mapped, creating and storing a `io_tctx_node`.
* `__io_uring_add_tctx_node_from_submit` – Like `__io_uring_add_tctx_node`, but tailored for submission paths with single-issuer constraints.
* `io_uring_clean_tctx` – Cleans up all `io_tctx_node` entries and releases the workqueue from a `io_uring_task`.
* `io_uring_unreg_ringfd` – Unregisters all ring file descriptors associated with the current task's `io_uring_task`.
* `io_ringfd_register` – Registers ring file descriptors for reuse, avoiding repeated fd lookup on `io_uring_enter()`.
* `io_ringfd_unregister` – Unregisters specific ring file descriptors by index using an array of resource update structs.
* `io_uring_add_tctx_node` – Fast path to associate the current task with a given `io_ring_ctx`, falling back to submit path if needed.

### `tctx.c`

* `io_init_wq_offload` – Initializes a new `io_wq` workqueue for offloading, reusing or creating a hash map for task concurrency limits.
* `__io_uring_free` – Frees all resources associated with a task's `io_uring_task`, including workqueue and inflight counter.
* `io_uring_alloc_task_context` – Creates and initializes an `io_uring_task`, including inflight tracking, workqueue, and waitqueue setup.
* `__io_uring_add_tctx_node` – Maps a task to a `io_ring_ctx` by inserting a `io_tctx_node` in the task’s `xarray`, managing list links.
* `__io_uring_add_tctx_node_from_submit` – Ensures the task is associated with the `ctx` and updates its last-used context; rejects if not the submitter in single-issuer mode.
* `io_uring_del_tctx_node` – Erases a specific context-task mapping and removes the node from the context's list.
* `io_uring_clean_tctx` – Iterates through and deletes all context-task mappings and shuts down the task's workqueue.
* `io_uring_unreg_ringfd` – Frees all registered ring file descriptors held in the current task’s ringfd table.
* `io_ring_add_registered_file` – Attempts to assign a file to a free slot in the task’s ringfd table between `start` and `end`.
* `io_ring_add_registered_fd` – Gets a `file` from a user-provided fd, validates it's an `io_uring` instance, and registers it in the ringfd table.
* `io_ringfd_register` – Handles user-space registration of one or more ring file descriptors into the per-task ringfd table.
* `io_ringfd_unregister` – Unregisters specified ring file descriptors based on user input by index, validating and releasing each one.

Here’s the summary of each function in `timeout.h` and `timeout.c` with bullet points:

### `timeout.h`

* `__io_disarm_linked_timeout`: Disarms the timeout associated with a linked I/O request (`req`), canceling its timer if necessary.
* `io_disarm_linked_timeout`: Checks if the I/O request (`req`) is linked to a timeout and disarms the linked timeout if applicable.
* `io_flush_timeouts`: Flushes the timeout list for the given `io_ring_ctx` (`ctx`), processing all timeouts.
* `io_timeout_cancel`: Cancels an active timeout based on the provided `io_cancel_data`.
* `io_kill_timeouts`: Terminates timeouts for a given task context (`tctx`), optionally canceling all timeouts.
* `io_queue_linked_timeout`: Queues a linked timeout request.
* `io_disarm_next`: Disarms the next linked timeout in the I/O request chain.
* `io_timeout_prep`: Prepares a timeout operation for the provided I/O request (`req`) based on the provided submission queue entry (`sqe`).
* `io_link_timeout_prep`: Prepares a linked timeout operation for the provided I/O request (`req`) based on the submission queue entry (`sqe`).
* `io_timeout_remove_prep`: Prepares the removal of a timeout operation for the provided I/O request (`req`) based on the submission queue entry (`sqe`).
* `io_timeout_remove`: Removes a timeout operation, either updating or canceling the timeout.

### `timeout.c`

* `io_is_timeout_noseq`: Checks if the timeout has no sequence or if it’s a multishot timeout.
* `io_put_req`: Releases the I/O request (`req`) and processes the next request if required.
* `io_timeout_finish`: Determines if a timeout operation has finished based on flags and repeat counts.
* `io_timeout_fn`: Timer callback for the timeout operation, completing the request after a timeout.
* `io_timeout_complete`: Completes the timeout operation, re-arming the timer if the timeout is multishot.
* `io_flush_killed_timeouts`: Flushes and completes the killed timeouts in the given list.
* `io_kill_timeout`: Tries to cancel the timeout associated with the I/O request (`req`) and moves it to a list for completion.
* `io_req_tw_fail_links`: Fails linked timeouts for a given request, marking them as canceled.
* `io_fail_links`: Fails all the linked I/O requests and ensures that their completion is marked as skipped if necessary.
* `io_remove_next_linked`: Removes the next linked I/O request from the chain.
* `io_disarm_next`: Disarms the next linked timeout in the I/O request chain.
* `__io_disarm_linked_timeout`: Internal function to disarm a linked timeout, removing it from the list and canceling the timer if applicable.
* `io_timeout_fn`: Callback function for the timeout timer, which completes the request after the timeout expires.
* `io_timeout_extract`: Extracts a timeout request from the timeout list, given a cancel data structure.
* `io_timeout_cancel`: Cancels a timeout request based on the provided cancel data.
* `io_req_task_link_timeout`: Handles the completion of a task that is linked to a timeout, marking it as completed or failed.
* `io_link_timeout_fn`: Callback for the linked timeout timer, handling the task completion or failure.
* `io_timeout_get_clock`: Returns the clock to be used for the timeout based on the flags.
* `io_linked_timeout_update`: Updates a linked timeout's timer with new settings.
* `io_timeout_update`: Updates a timeout's timer for an existing request.
* `io_timeout_remove_prep`: Prepares for the removal of a timeout request by checking the validity of the flags and provided data.
* `io_translate_timeout_mode`: Translates timeout flags to the appropriate `hrtimer_mode`.
* `io_timeout_remove`: Removes or updates a timeout request, based on the flags set in the request.
* `__io_timeout_prep`: Internal function to prepare a timeout request, either regular or linked, based on flags and settings in the submission queue entry.
* `io_timeout_prep`: Prepares a regular timeout operation for the given I/O request.
* `io_link_timeout_prep`: Prepares a linked timeout operation for the given I/O request.
* `io_timeout`: Handles the timeout operation, setting it up for issue.

### `truncate.h`

* `io_ftruncate_prep`: Prepares the I/O request for file truncation and validates the input parameters.
* `io_ftruncate`: Executes the file truncation and sets the result in the I/O request.

### `truncate.c`

* `io_ftruncate_prep`: Validates the submission parameters and prepares the truncation request, setting the truncation length.
* `io_ftruncate`: Performs the truncation and stores the result in the I/O request.

### `uring_cmd.h`

* `io_uring_cmd`: Executes an I/O command by invoking the corresponding file operation.
* `io_uring_cmd_prep`: Prepares the I/O command, validates flags, and sets up necessary data.
* `io_uring_cmd_cleanup`: Cleans up the I/O command's resources after execution.
* `io_uring_try_cancel_uring_cmd`: Attempts to cancel an I/O command, optionally canceling all or specific commands.
* `io_cmd_cache_free`: Frees memory used by an asynchronous command.
* `io_uring_cmd_import_fixed_vec`: Imports a vector of fixed I/O buffers for the command.

### `uring_cmd.c`

* `io_cmd_cache_free`: Frees the cache used by asynchronous commands.
* `io_req_uring_cleanup`: Cleans up resources associated with an I/O request after the command execution.
* `io_uring_cmd_cleanup`: Cleans up the I/O command's resources after execution.
* `io_uring_try_cancel_uring_cmd`: Attempts to cancel an I/O command from the cancelable list.
* `io_uring_cmd_del_cancelable`: Removes an I/O command from the cancelable list.
* `io_uring_cmd_mark_cancelable`: Marks an I/O command as cancelable and adds it to the cancelable list.
* `io_uring_cmd_work`: Executes the task work associated with the I/O command.
* `__io_uring_cmd_do_in_task`: Sets up task work for an I/O command to be executed in the current task.
* `io_req_set_cqe32_extra`: Sets additional information in the completion queue entry (CQE).
* `io_uring_cmd_done`: Marks an I/O command as completed, cleaning up and setting the result.
* `io_uring_cmd_prep_setup`: Prepares the setup for an I/O command, copying SQE data.
* `io_uring_cmd_prep`: Prepares the I/O command, validates flags, and calls the setup function.
* `io_uring_cmd`: Executes the I/O command by invoking the command-specific function from the file operations.
* `io_uring_cmd_import_fixed`: Imports a fixed buffer for the I/O command.
* `io_uring_cmd_import_fixed_vec`: Imports a vector of fixed buffers for the I/O command.
* `io_uring_cmd_issue_blocking`: Issues a blocking I/O command.
* `io_uring_cmd_getsockopt`: Retrieves socket options for a command.
* `io_uring_cmd_setsockopt`: Sets socket options for a command.
* `io_uring_cmd_sock`: Handles socket-specific commands like getting and setting socket options.

### `waitid.h`

* `io_waitid_prep`: Prepares the request for the `waitid` operation by validating and setting up the necessary data from the submission queue entry (SQE).
* `io_waitid`: Executes the `waitid` operation by preparing the wait options and waiting for the child process, handling completion and cancellation.
* `io_waitid_cancel`: Cancels a `waitid` operation by removing it from the cancellation list and ensuring proper resource cleanup.
* `io_waitid_remove_all`: Removes all `waitid` operations from the list, optionally canceling them.

### `waitid.c`

* `io_waitid_free`: Frees the resources associated with a completed `waitid` request, including PID references and async data.
* `io_waitid_compat_copy_si`: Copies signal information for compatibility between 32-bit and 64-bit systems.
* `io_waitid_copy_si`: Copies signal information into the user-provided buffer.
* `io_waitid_finish`: Finalizes the `waitid` operation by copying the signal info and freeing the request.
* `io_waitid_complete`: Completes the `waitid` request, checks references, and handles cleanup and completion.
* `__io_waitid_cancel`: Handles cancellation of a `waitid` request, ensuring it is safely removed from the wait queue.
* `io_waitid_cancel`: Calls `io_cancel_remove` to cancel a `waitid` request, using a custom cancel handler.
* `io_waitid_remove_all`: Removes all `waitid` operations from the context’s list, optionally canceling them.
* `io_waitid_drop_issue_ref`: Drops a reference count and handles the case when the operation is racing with a wakeup.
* `io_waitid_cb`: Callback function for completing the `waitid` request, handling retries and cancellations.
* `io_waitid_wait`: Handles the wait queue logic for the `waitid` operation, checking conditions and triggering the callback.
* `io_waitid_prep`: Prepares the `waitid` operation, allocating necessary async data and copying parameters from the SQE.
* `io_waitid`: Executes the `waitid` operation, managing the wait queue, handling wakeups, and processing the result.

### `xattr.h`

* `io_xattr_cleanup`: Cleans up resources used by the `io_xattr` structure, including freeing the filename and xattr context.
* `io_fsetxattr_prep`: Prepares a `fsetxattr` operation by setting up the necessary data in the `io_xattr` structure.
* `io_fsetxattr`: Executes the `fsetxattr` operation, setting the extended attribute for a file.
* `io_setxattr_prep`: Prepares a `setxattr` operation by setting up the necessary data in the `io_xattr` structure.
* `io_setxattr`: Executes the `setxattr` operation, setting the extended attribute for a file or directory.
* `io_fgetxattr_prep`: Prepares a `fgetxattr` operation by setting up the necessary data in the `io_xattr` structure.
* `io_fgetxattr`: Executes the `fgetxattr` operation, retrieving the extended attribute for a file.
* `io_getxattr_prep`: Prepares a `getxattr` operation by setting up the necessary data in the `io_xattr` structure.
* `io_getxattr`: Executes the `getxattr` operation, retrieving the extended attribute for a file or directory.

### `xattr.c`

* `io_xattr_cleanup`: Frees any resources allocated during the `xattr` operation, such as the filename and kernel context.
* `io_xattr_finish`: Finalizes the `xattr` operation, cleans up and sets the result of the operation.
* `__io_getxattr_prep`: Prepares the `getxattr` operation by allocating necessary memory and setting up the xattr context.
* `io_fgetxattr_prep`: Prepares the `fgetxattr` operation using the `__io_getxattr_prep` function.
* `io_getxattr_prep`: Prepares the `getxattr` operation, validating file descriptors and setting up the xattr context with the provided filename.
* `io_fgetxattr`: Executes the `fgetxattr` operation by retrieving the extended attribute for a file.
* `io_getxattr`: Executes the `getxattr` operation by retrieving the extended attribute for a file or directory.
* `__io_setxattr_prep`: Prepares the `setxattr` operation by allocating memory and setting up the xattr context.
* `io_setxattr_prep`: Prepares the `setxattr` operation, validating file descriptors and setting up the xattr context with the provided filename.
* `io_fsetxattr_prep`: Prepares the `fsetxattr` operation using the `__io_setxattr_prep` function.
* `io_fsetxattr`: Executes the `fsetxattr` operation by setting the extended attribute for a file.
* `io_setxattr`: Executes the `setxattr` operation by setting the extended attribute for a file or directory.

### `zcrx.h`

* `io_register_zcrx_ifq`: Registers a Zero-Copy Receive (ZCRX) interface queue for an `io_ring_ctx`. Returns an error if ZCRX support is not available (`-EOPNOTSUPP` in the else section).
* `io_unregister_zcrx_ifqs`: Unregisters the ZCRX interface queues for a given `io_ring_ctx`. Does nothing if ZCRX support is not available.
* `io_shutdown_zcrx_ifqs`: Shuts down ZCRX interface queues for a given `io_ring_ctx`. Does nothing if ZCRX support is not available.
* `io_zcrx_recv`: Receives a packet through a ZCRX interface queue, using the provided socket and flags. Returns an error if ZCRX support is not available.
* `io_recvzc`: Receives data from a Zero-Copy socket using the `io_kiocb` request structure. Processes ZCRX reception asynchronously.
* `io_recvzc_prep`: Prepares the `io_recvzc` operation by setting up the request structure based on the provided submission queue entry (`sqe`).

### `zcrx.c`

* `__io_zcrx_unmap_area`: Unmaps a given area from the device, ensuring that DMA addresses are cleared for all `net_iovs` within the area.
* `io_zcrx_unmap_area`: Calls `__io_zcrx_unmap_area` if the area is mapped, unmapping all `net_iovs` associated with that area.
* `io_zcrx_map_area`: Maps the pages of an area to DMA addresses and updates the corresponding `net_iovs` with these addresses. Returns an error if mapping fails.
* `io_zcrx_sync_for_device`: Syncs memory for the device if necessary, ensuring that the data is properly synchronized for DMA operations.
* `io_allocate_rbuf_ring`: Allocates a ring buffer for request entries (RQEs) for a zero-copy receive (ZCRX) interface queue (IFQ).
* `io_free_rbuf_ring`: Frees the ring buffer resources allocated for the IFQ.
* `io_zcrx_free_area`: Frees the resources associated with a ZCRX area, including unpinning pages, and freeing allocated memory for `net_iovs` and references.
* `io_zcrx_create_area`: Creates a ZCRX area by pinning user memory pages, setting up the necessary structures, and validating the area properties.
* `io_zcrx_ifq_alloc`: Allocates and initializes a new IO ZCRX IFQ (interface queue), setting up locks and linking it to the context.
* `io_zcrx_drop_netdev`: Drops the network device from the ZCRX IFQ, releasing its reference.
* `io_close_queue`: Closes the receive queue of the network device associated with the IFQ, ensuring proper cleanup.
* `io_zcrx_ifq_free`: Frees all resources associated with a ZCRX IFQ, including the area, device, and ring buffer.
* `io_register_zcrx_ifq`: Registers a new ZCRX IFQ with the IO ring context, performing necessary validation and memory allocation.
* `io_unregister_zcrx_ifqs`: Unregisters the ZCRX IFQ and frees its resources.
* `__io_zcrx_get_free_niov`: Retrieves a free `net_iov` from the freelist of a ZCRX area, locking the freelist during the operation.
* `io_zcrx_return_niov_freelist`: Returns a `net_iov` to the freelist for later reuse, ensuring proper synchronization.
* `io_zcrx_return_niov`: Returns a `net_iov` either by placing it back in the freelist or by cleaning up the associated page pool.
* `io_zcrx_scrub`: Reclaims buffers that were given to user space, ensuring the resources are cleaned up properly.
* `io_shutdown_zcrx_ifqs`: Shuts down the ZCRX IFQ by scrubbing the area and closing the queue.
* `io_zcrx_rqring_entries`: Returns the number of entries currently available in the request queue ring, based on the tail index.
* `io_zcrx_get_rqe`: Retrieves a request entry (RQE) from the ring buffer based on the head index.
* `io_zcrx_ring_refill`: Refills the ring buffer by processing available `net_iovs` and placing them back into the page pool.
* `io_zcrx_refill_slow`: Refill function that operates slower, used when there is not enough capacity in the page pool to refill quickly.
* `io_pp_zc_alloc_netmems`: Allocates network memory from the page pool, refilling the pool if necessary.
* `io_pp_zc_release_netmem`: Releases network memory back to the page pool, ensuring it is properly cleaned up.
* `io_pp_zc_init`: Initializes a page pool for zero-copy operations, validating parameters and preparing the pool for use.
* `io_pp_zc_destroy`: Destroys a page pool for zero-copy operations, ensuring all resources are cleaned up.
* `io_pp_nl_fill`: Fills netlink messages related to the page pool, potentially interacting with network device queues.

