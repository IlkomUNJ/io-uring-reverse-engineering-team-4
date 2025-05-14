### modules,order
This file specifies the build order of kernel modules for the io_uring subsystem. When modules are built from source, modules.order lists them in the correct sequence to ensure proper dependency resolution and linking. Although it may appear empty if no modules were compiled from io_uring, it is automatically generated during the kernel build process. An empty modules.order simply indicates that no loadable modules were built for io_uring in the current configuration.

# Source
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