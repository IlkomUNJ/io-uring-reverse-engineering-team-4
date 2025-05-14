### advice.c
| Structure name | Defined in | Attributes                | Caller Functions Source   | Source caller | Usage                 |
| -------------- | ---------- | ------------------------- | ------------------------- | ------------- | --------------------- |
| io\_fadvise    | advice.c   | file, offset, len, advice | io\_fadvise\_prep         | advice.c      | local variable        |
|                |            |                           | io\_fadvise               |               | function parameter    |
| io\_madvise    | advice.c   | file, addr, len, advice   | io\_madvise\_prep         | advice.c      | local variable        |
|                |            |                           | io\_madvise               |               | function parameter    |
|                |            |                           | io\_fadvise\_force\_async |               | local helper function |

### alloc_cache.c
| Structure name   | Defined in     | Attributes                                                | Caller Functions Source | Source caller  | Usage              |
| ---------------- | -------------- | --------------------------------------------------------- | ----------------------- | -------------- | ------------------ |
| io\_alloc\_cache | alloc\_cache.c | entries, nr\_cached, max\_cached, elem\_size, init\_clear | io\_alloc\_cache\_free  | alloc\_cache.c | function parameter |
|                  |                |                                                           | io\_alloc\_cache\_init  |                | function parameter |
|                  |                |                                                           | io\_cache\_alloc\_new   |                | function parameter |

### cancel.c
| Structure name   | Defined in | Attributes                                | Caller Functions Source | Source caller | Usage                 |
| ---------------- | ---------- | ----------------------------------------- | ----------------------- | ------------- | --------------------- |
| io\_cancel       | cancel.c   | file, addr, flags, fd, opcode             | io\_async\_cancel\_prep | cancel.c      | function parameter    |
|                  |            |                                           | io\_async\_cancel       |               | function parameter    |
|                  |            |                                           | io\_try\_cancel         |               | function parameter    |
|                  |            |                                           | io\_sync\_cancel        |               | function parameter    |
|                  |            |                                           | io\_cancel\_req\_match  |               | local helper function |
|                  |            |                                           | io\_cancel\_cb          |               | local helper function |
| io\_cancel\_data | cancel.c   | ctx, data, flags, opcode, seq             | io\_async\_cancel       | cancel.c      | function parameter    |
|                  |            |                                           | io\_try\_cancel         |               | function parameter    |
|                  |            |                                           | io\_sync\_cancel        |               | function parameter    |
|                  |            |                                           | io\_cancel\_req\_match  |               | function parameter    |
|                  |            |                                           | io\_async\_cancel\_one  |               | local helper function |
| io\_ring\_ctx    | cancel.c   | completion\_lock, cancel\_seq, tctx\_list | io\_sync\_cancel        | cancel.c      | function parameter    |
|                  |            |                                           | io\_try\_cancel         |               | function parameter    |
|                  |            |                                           | io\_cancel\_remove\_all |               | function parameter    |
|                  |            |                                           | io\_cancel\_remove      |               | function parameter    |
| io\_uring\_task  | cancel.c   | io\_uring (task)                          | io\_try\_cancel         | cancel.c      | function parameter    |

### epoll.c
| Structure name  | Defined in | Attributes                | Caller Functions Source | Source caller | Usage              |
| --------------- | ---------- | ------------------------- | ----------------------- | ------------- | ------------------ |
| io\_epoll       | epoll.c    | file, epfd, op, fd, event | io\_epoll\_ctl\_prep    | epoll.c       | function parameter |
|                 |            |                           | io\_epoll\_ctl          |               | function parameter |
| io\_epoll\_wait | epoll.c    | file, maxevents, events   | io\_epoll\_wait\_prep   | epoll.c       | function parameter |
|                 |            |                           | io\_epoll\_wait         |               | function parameter |

### eventfd.c
| Structure name | Defined in | Attributes                                                 | Caller Functions Source    | Source caller | Usage                                  |
| -------------- | ---------- | ---------------------------------------------------------- | -------------------------- | ------------- | -------------------------------------- |
| io\_ev\_fd     | eventfd.c  | cq\_ev\_fd, eventfd\_async, last\_cq\_tail, refs, ops, rcu | io\_eventfd\_free          | eventfd.c     | Memory management (allocation/freeing) |
|                |            |                                                            | io\_eventfd\_put           | eventfd.c     | Reference counting                     |
|                |            |                                                            | io\_eventfd\_do\_signal    | eventfd.c     | Signal handling                        |
|                |            |                                                            | io\_eventfd\_release       | eventfd.c     | Release resources                      |
|                |            |                                                            | io\_eventfd\_grab          | eventfd.c     | Eventfd grabbing                       |
|                |            |                                                            | io\_eventfd\_signal        | eventfd.c     | Signal sending                         |
|                |            |                                                            | io\_eventfd\_flush\_signal | eventfd.c     | Flushing signals                       |
|                |            |                                                            | io\_eventfd\_register      | eventfd.c     | Registration                           |
|                |            |                                                            | io\_eventfd\_unregister    | eventfd.c     | Unregistration                         |

### fdinfo.c
| Structure name   | Defined in | Attributes                                                                                                                                                                                         | Caller Functions Source | Source caller | Usage                                                                                     |
| ---------------- | ---------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------- | ------------- | ----------------------------------------------------------------------------------------- |
| io\_ring\_ctx    | fdinfo.c   | flags, sq\_entries, cq\_entries, sq\_head, sq\_tail, cached\_sq\_head, cached\_cq\_tail, sq\_array, sq\_sqes, file\_table, buf\_table, personalities, cancel\_table, completion\_lock, uring\_lock | io\_uring\_show\_fdinfo | fdinfo.c      | Core structure for io\_uring context, handles various state and operational data for ring |
| io\_uring\_sqe   | fdinfo.c   | opcode, fd, flags, off, addr, rw\_flags, buf\_index, user\_data                                                                                                                                    | io\_uring\_show\_fdinfo | fdinfo.c      | Represents a submission queue entry (SQE), used for I/O requests                          |
| io\_uring\_cqe   | fdinfo.c   | user\_data, res, flags, big\_cqe                                                                                                                                                                   | io\_uring\_show\_fdinfo | fdinfo.c      | Represents a completion queue entry (CQE)                                                 |
| io\_sq\_data     | fdinfo.c   | thread, task\_pid, sq\_cpu, work\_time                                                                                                                                                             | io\_uring\_show\_fdinfo | fdinfo.c      | Contains information about the submission queue (SQ) worker thread                        |
| io\_mapped\_ubuf | fdinfo.c   | ubuf, len                                                                                                                                                                                          | io\_uring\_show\_fdinfo | fdinfo.c      | Represents user buffer in the io\_uring context                                           |
| io\_hash\_bucket | fdinfo.c   | list (of io\_kiocb requests)                                                                                                                                                                       | io\_uring\_show\_fdinfo | fdinfo.c      | Hash bucket for canceling pending requests in io\_uring                                   |
| io\_kiocb        | fdinfo.c   | opcode, tctx (task context), hash\_node (for hash bucket)                                                                                                                                          | io\_uring\_show\_fdinfo | fdinfo.c      | Represents an I/O request, used for tracking in the cancel table                          |

### filetable.c
| Structure name                | Defined in  | Attributes                                                          | Caller Functions Source                                                                                                                                                     | Source caller | Usage                                                                                   |
| ----------------------------- | ----------- | ------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------- | --------------------------------------------------------------------------------------- |
| io\_ring\_ctx                 | filetable.c | file\_table (io\_file\_table), file\_alloc\_start, file\_alloc\_end | io\_file\_bitmap\_get, io\_alloc\_file\_tables, io\_free\_file\_tables, io\_install\_fixed\_file, \_\_io\_fixed\_fd\_install, io\_fixed\_fd\_install, io\_fixed\_fd\_remove | filetable.c   | Represents the io\_uring context, holds the file table and allocation range for files   |
| io\_file\_table               | filetable.c | bitmap, data (io\_rsrc\_data), alloc\_hint                          | io\_alloc\_file\_tables, io\_free\_file\_tables, io\_file\_bitmap\_get, io\_install\_fixed\_file, io\_fixed\_fd\_remove, io\_register\_file\_alloc\_range                   | filetable.c   | A table of files managed by the io\_uring context, tracks file allocation and resources |
| io\_rsrc\_node                | filetable.c | file resources (used with files), nodes list                        | io\_install\_fixed\_file, io\_fixed\_fd\_remove                                                                                                                             | filetable.c   | Represents a resource node in the file table, holds a file                              |
| io\_uring\_file\_index\_range | filetable.c | off, len, resv                                                      | io\_register\_file\_alloc\_range                                                                                                                                            | filetable.c   | Defines a range of file indices for allocation and registration in the file table       |

### fs.c
| Structure name | Defined in | Attributes                                                                            | Caller Functions Source                                                             | Source caller | Usage                                                                                      |
| -------------- | ---------- | ------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- | ------------- | ------------------------------------------------------------------------------------------ |
| io\_rename     | fs.c       | file (struct file), old\_dfd, new\_dfd, oldpath (filename), newpath (filename), flags | io\_renameat\_prep, io\_renameat, io\_renameat\_cleanup                             | fs.c          | Holds data related to the rename operation (file paths, descriptors, and flags)            |
| io\_unlink     | fs.c       | file (struct file), dfd, flags, filename (filename)                                   | io\_unlinkat\_prep, io\_unlinkat, io\_unlinkat\_cleanup                             | fs.c          | Holds data related to the unlink operation (file paths, flags)                             |
| io\_mkdir      | fs.c       | file (struct file), dfd, mode (umode\_t), filename (filename)                         | io\_mkdirat\_prep, io\_mkdirat, io\_mkdirat\_cleanup                                | fs.c          | Holds data related to the mkdir operation (directory paths, mode)                          |
| io\_link       | fs.c       | file (struct file), old\_dfd, new\_dfd, oldpath (filename), newpath (filename), flags | io\_symlinkat\_prep, io\_symlinkat, io\_linkat\_prep, io\_linkat, io\_link\_cleanup | fs.c          | Holds data related to the symlink and link operations (file paths, descriptors, and flags) |

### futex.c
| **Structure name** | **Defined in** | **Attributes**                                                                                                                        | **Caller Functions Source**                                          | **Source caller** | **Usage**                                                                                              |
| ------------------ | -------------- | ------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------- | ------------------------------------------------------------------------------------------------------ |
| io_futex         | futex.c        | file (struct file), uaddr (user pointer), futex_val, futex_mask, futexv_owned, futex_flags, futex_nr, futexv_unqueued | io_futex_prep, io_futex_cancel, io_futex_wait, io_futex_wake | futex.c           | Holds data for futex operations (address, value, mask, flags, number of futexes, etc.)                 |
| io_futex_data    | futex.c        | q (futex\_q), req (struct io\_kiocb\*)                                                                                            | io_futex_wait, io_futex_complete, io_futex_wake_fn             | futex.c           | Holds the futex queue and request data for processing in asynchronous tasks.                           |
| futex_vector     | futex.c        | futexv (vector of futexes), futexv_owned, futexv_unqueued                                                                       | io_futexv_prep, io_futexv_complete, io_futexv_claim            | futex.c           | Manages operations on multiple futexes in the same vector for batch processing.                        |
| futex_q          | futex.c        | bitset, wake (callback function), wake_data                                                                                     | futex_queue, futex_unqueue, futex_wait_setup                   | futex.c           | Holds data for individual futex queue entries (bitset for mask, wake function pointer, etc.)           |
| io_futex_data    | futex.c        | q (futex\_q), req (io\_kiocb)                                                                                                     | io_futex_wait, io_futex_complete, io_futex_wake_fn             | futex.c           | Used for managing the task request and its corresponding futex queue when handling waits and wake-ups. |
