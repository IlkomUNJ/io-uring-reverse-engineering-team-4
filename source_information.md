# Task 1: Information about io_uring source
List in this section source and headers of io_uring. For each of the C source/header, you must put description what's the prime responsibily of the source. Take notes, description of the source should be slightly technical like the example given. 

### Kconfig  
This configuration file defines build-time options for io_uring features in the Linux kernel. It allows enabling or disabling specific io_uring capabilities during kernel compilation.

## Source
### advice.c
Store io_madvice & io_fadvice structures, both have the same exact attributes. Which make them basically the same thing. Except function body treat them as separate. Codes which make use of io_madvice are guarded by compilation macro, which make its relevant functions only active if the build flag is set. But functions that make use of io_fadvice are active all the time. The exact difference between io_madvice & io_fadvice will only known after exploring do_madvise function for io_madvice & vfs_fadvise function for io_fadvice. 

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