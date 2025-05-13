# Task 1: Information about io_uring source
List in this section source and headers of io_uring. For each of the C source/header, you must put description what's the prime responsibily of the source. Take notes, description of the source should be slightly technical like the example given. 

### Kconfig  
This configuration file defines build-time options for io_uring features in the Linux kernel. It allows enabling or disabling specific io_uring capabilities during kernel compilation.

## Source
### advice.c
Store io_madvice & io_fadvice structures, both have the same exact attributes. Which make them basically the same thing. Except function body treat them as separate. Codes which make use of io_madvice are guarded by compilation macro, which make its relevant functions only active if the build flag is set. But functions that make use of io_fadvice are active all the time. The exact difference between io_madvice & io_fadvice will only known after exploring do_madvise function for io_madvice & vfs_fadvise function for io_fadvice. 

### io-wq.c
Implements the asynchronous workqueue backend for io_uring, providing infrastructure for deferred execution of potentially blocking tasks. Centralizes per-instance state via struct io_wq, which manages worker lifecycles, task accounting (io_wq_acct), and CPU affinity. Supports both bounded and unbounded worker pools, concurrency control, and per-task resource isolation for efficient and safe async offloading.

### kbuf.c
Handles buffer provisioning for io_uring. Manages registration and tracking of user-supplied buffer groups (bgid), enabling efficient, zero-copy I/O through pre-allocated buffers. Implements fast lookup, lifetime management, and reuse of buffers defined in struct io_provide_buf.

### memmap.c
Manages memory region tracking for io_uring buffer registrations. Handles kernel and user-mapped memory lifecycles, page pinning, and reference accounting. Supports multiple allocation strategies via region flags (e.g., IO_REGION_F_VMAP, IO_REGION_F_USER_PROVIDED), ensuring safe and efficient access to user-space and kernel-mapped buffers.

### msg_ring.c
Implements message-passing between io_uring instances or file descriptors using the IORING_OP_MSG_RING opcode. Manages asynchronous control messages via struct io_msg, enabling cross-ring communication, CQE injection, and advanced coordination patterns. Supports message routing through registered file descriptors with configurable flags and metadata.

### napi.c  
Integrates io_uring with the Linux NAPI networking subsystem to enable low-latency, event-driven I/O. Tracks NAPI IDs using struct io_napi_entry and provides efficient polling and cleanup of stale NAPI associations via RCU-safe list handling. Facilitates adaptive busy-polling and tighter integration with high-performance networking stacks.

### net.c
Implements asynchronous networking operations for io_uring, including non-blocking socket creation, connection management, and message transmission. Supports operations like accept, connect, sendmsg, recvmsg, and shutdown, with extended features such as multishot polling, zero-copy sends, and per-request buffer group selection. Each network operation is encapsulated in dedicated request structures (e.g., io_accept, io_connect, io_sr_msg) for fine-grained control over socket behavior and integration with the kernel's file descriptor and networking subsystems.  

### nop.c 
Implements the IORING_NOP operation, providing a placeholder or barrier request within io_uring submission queues. Useful for testing, ordering, and constructing complex dependency chains without performing actual I/O.

### notif.c 
Handles asynchronous notification infrastructure for io_uring, including support for linked completion events and zero-copy send notifications. Integrates with kernel signaling mechanisms to track and report deferred operation status.

### opdef.c  
Defines various operation types and their corresponding handlers for `io_uring`. It includes the registration of supported operations and their dispatch functions, acting as the central registry for operation types.

### openclose.c  
Implements file open and close operations for `io_uring`. Handles asynchronous file opening and closing via the `io_uring` interface, efficiently managing the lifecycle of files.

### poll.c  
Provides the implementation of polling operations for `io_uring`. It allows asynchronous monitoring of file descriptors for events, integrating with the kernel’s polling mechanism for efficient event notification.

### register.c  
Handles resource registration with `io_uring`. Manages the pre-registration of buffers, files, and other resources, optimizing their access through the registration API.

### rsrc.c  
Manages resources used in `io_uring` operations. Responsible for resource allocation, tracking, and cleanup, offering abstractions for efficient resource management.

### rw.c  
Implements the read and write operations for `io_uring`. Facilitates core data transfer functionality for file I/O with optimizations designed for high-throughput and low-latency operations.

### splice.c  
Handles splice operations in `io_uring`. Enables efficient data transfer between file descriptors without copying to userspace, supporting zero-copy data movement between pipes and other file descriptors.

## Headers
### advice.h
Just declare the function specification. 

### io-wq.h
Defines the internal interfaces and data structures for io_uring's asynchronous workqueue system. Includes worker types, cancellation states, and scheduling flags for managing deferred task execution in io-wq.c.

### kbuf.h
Declares structures and flags for managing provided buffers in io_uring, including classic and ring-mapped buffer models. Supports buffer group selection, dynamic buffer allocation, and incremental consumption modes.

### memmap.h
Declares interfaces for memory region tracking and mapping used in registered buffer handling. Supports management of user-pinned and kernel-mapped memory for safe and efficient access from io_uring.

### msg_ring.h
Declares the interface for sending control messages and CQEs between io_uring instances. Provides function declarations for cross-ring communication setup and message handling.

### napi.h
Declares NAPI integration functions for io_uring, enabling low-latency polling and tight coupling with the Linux networking stack. Available when CONFIG_NET_RX_BUSY_POLL is enabled.

### net.h
Declares networking-related structures and helpers for async socket operations in io_uring. Includes support for message headers, address management, and zero-copy optimizations.

### nop.h
Declares support functions for the IORING_NOP operation, used as a no-op or placeholder in io_uring submission chains.

### notif.h
Defines the data structures for asynchronous notification handling in io_uring, including support for zero-copy send tracking and deferred completion signaling.

### opdef.h
Declares operation type definitions and handler registration interfaces. Contains mappings between operation codes and handler functions. Provides the framework for extending io_uring with new operation types.

### openclose.h
Defines structures and constants for file open and close operations. Contains declarations for path handling and file mode parameters. Provides interfaces for asynchronous file lifecycle management through io_uring.

### poll.h
Declares structures and constants for polling operations. Contains definitions for event types and polling modes. Provides interfaces for monitoring file descriptors for events through the io_uring subsystem.

### refs.h
Defines reference counting mechanisms for io_uring resources. Contains declarations for acquiring, releasing, and tracking references. Provides interfaces for memory management and resource lifetime control.

### register.h
Declares structures and constants for resource registration with io_uring. Contains definitions for registering buffers, files, and other resources. Provides interfaces for optimizing access to frequently used resources.

### rsrc.h
Defines resource management interfaces for io_uring operations. Contains declarations for resource allocation, tracking, and deallocation. Provides abstractions for generic resource handling across different operation types.

### rw.h
Declares structures and constants for read and write operations. Contains definitions for various I/O modes and buffer parameters. Provides interfaces for data transfer operations through io_uring.

### slist.h
Defines generic single-linked list data structures used throughout io_uring. Contains macros and inline functions for list manipulation. Provides common data structure implementations for internal use by io_uring subsystems.

### splice.h
Declares interfaces for splice operations between file descriptors. Contains definitions for pipe and file splicing parameters. Provides declarations for zero-copy data transfer operations through io_uring.