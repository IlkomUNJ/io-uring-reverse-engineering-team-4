### advice.c
| Structure name | Defined in | Attributes                | Caller Functions Source   | Source caller | Usage                 |
| -------------- | ---------- | ------------------------- | ------------------------- | ------------- | --------------------- |
| io\_fadvise    | advice.c   | file, offset, len, advice | io\_fadvise\_prep         | advice.c      | local variable        |
|                |            |                           | io\_fadvise               |               | function parameter    |
| io\_madvise    | advice.c   | file, addr, len, advice   | io\_madvise\_prep         | advice.c      | local variable        |
|                |            |                           | io\_madvise               |               | function parameter    |
|                |            |                           | io\_fadvise\_force\_async |               | local helper function |'

### advise.h
| Structure name | Defined in | Attributes        | Caller Functions Source | Source caller | Usage              |
| -------------- | ---------- | ----------------- | ----------------------- | ------------- | ------------------ |
| io\_madvise    | advise.h   | req, issue\_flags | io\_madvise\_prep       | advise.h      | local variable     |
|                |            |                   | io\_madvise             |               | function parameter |
| io\_fadvise    | advise.h   | req, issue\_flags | io\_fadvise\_prep       | advise.h      | local variable     |
|                |            |                   | io\_fadvise             |               | function parameter |

### alloc_cache.c
| Structure name   | Defined in     | Attributes                                                | Caller Functions Source | Source caller  | Usage              |
| ---------------- | -------------- | --------------------------------------------------------- | ----------------------- | -------------- | ------------------ |
| io\_alloc\_cache | alloc\_cache.c | entries, nr\_cached, max\_cached, elem\_size, init\_clear | io\_alloc\_cache\_free  | alloc\_cache.c | function parameter |
|                  |                |                                                           | io\_alloc\_cache\_init  |                | function parameter |
|                  |                |                                                           | io\_cache\_alloc\_new   |                | function parameter |

### alloc_cache.h
| Structure name   | Defined in          | Attributes                                             | Caller Functions Source | Source caller       | Usage              |
| ---------------- | ------------------- | ------------------------------------------------------ | ----------------------- | ------------------- | ------------------ |
| io\_alloc\_cache | iou\_alloc\_cache.h | cache, max\_nr, size, init\_bytes, nr\_cached, entries | io\_alloc\_cache\_init  | iou\_alloc\_cache.h | local variable     |
|                  |                     |                                                        | io\_alloc\_cache\_free  |                     | function parameter |
|                  |                     |                                                        | io\_cache\_alloc\_new   |                     | function parameter |
|                  |                     |                                                        | io\_alloc\_cache\_put   |                     | function parameter |
|                  |                     |                                                        | io\_alloc\_cache\_get   |                     | function parameter |
|                  |                     |                                                        | io\_cache\_alloc        |                     | function parameter |
|                  |                     |                                                        | io\_cache\_free         |                     | function parameter |

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

### cancel.h
| Structure name   | Defined in       | Attributes                         | Caller Functions Source     | Source caller    | Usage              |
| ---------------- | ---------------- | ---------------------------------- | --------------------------- | ---------------- | ------------------ |
| io\_cancel\_data | ioring\_cancel.h | ctx, data/file, opcode, flags, seq | io\_async\_cancel\_prep     | ioring\_cancel.h | function parameter |
|                  |                  |                                    | io\_async\_cancel           |                  | function parameter |
|                  |                  |                                    | io\_try\_cancel             |                  | function parameter |
|                  |                  |                                    | io\_sync\_cancel            |                  | function parameter |
|                  |                  |                                    | io\_cancel\_req\_match      |                  | function parameter |
|                  |                  |                                    | io\_cancel\_remove\_all     |                  | function parameter |
|                  |                  |                                    | io\_cancel\_remove          |                  | function parameter |
|                  |                  |                                    | io\_cancel\_match\_sequence |                  | function parameter |

### epoll.c
| Structure name  | Defined in | Attributes                | Caller Functions Source | Source caller | Usage              |
| --------------- | ---------- | ------------------------- | ----------------------- | ------------- | ------------------ |
| io\_epoll       | epoll.c    | file, epfd, op, fd, event | io\_epoll\_ctl\_prep    | epoll.c       | function parameter |
|                 |            |                           | io\_epoll\_ctl          |               | function parameter |
| io\_epoll\_wait | epoll.c    | file, maxevents, events   | io\_epoll\_wait\_prep   | epoll.c       | function parameter |
|                 |            |                           | io\_epoll\_wait         |               | function parameter |

### epoll.h
| Structure name | Defined in    | Attributes | Caller Functions Source | Source caller      | Usage              |
| -------------- | ------------- | ---------- | ----------------------- | ------------------ | ------------------ |
| io\_kiocb      | epoll control | req, sqe   | io\_epoll\_ctl\_prep    | epoll control file | function parameter |
|                |               |            | io\_epoll\_ctl          |                    | function parameter |
|                |               |            | io\_epoll\_wait\_prep   |                    | function parameter |
|                |               |            | io\_epoll\_wait         |                    | function parameter |

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

### eventfd.h
| Structure name | Defined in         | Attributes               | Caller Functions Source    | Source caller      | Usage              |
| -------------- | ------------------ | ------------------------ | -------------------------- | ------------------ | ------------------ |
| io\_ring\_ctx  | eventfd operations | ctx, arg, eventfd\_async | io\_eventfd\_register      | eventfd operations | function parameter |
|                |                    |                          | io\_eventfd\_unregister    |                    | function parameter |
|                |                    |                          | io\_eventfd\_flush\_signal |                    | function parameter |
|                |                    |                          | io\_eventfd\_signal        |                    | function parameter |

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

### eventfd.h
| Structure name | Defined in         | Attributes               | Caller Functions Source    | Source caller      | Usage              |
| -------------- | ------------------ | ------------------------ | -------------------------- | ------------------ | ------------------ |
| io\_ring\_ctx  | eventfd operations | ctx, arg, eventfd\_async | io\_eventfd\_register      | eventfd operations | function parameter |
|                |                    |                          | io\_eventfd\_unregister    |                    | function parameter |
|                |                    |                          | io\_eventfd\_flush\_signal |                    | function parameter |
|                |                    |                          | io\_eventfd\_signal        |                    | function parameter |

### filetable.c
| Structure name                | Defined in  | Attributes                                                          | Caller Functions Source                                                                                                                                                     | Source caller | Usage                                                                                   |
| ----------------------------- | ----------- | ------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------- | --------------------------------------------------------------------------------------- |
| io\_ring\_ctx                 | filetable.c | file\_table (io\_file\_table), file\_alloc\_start, file\_alloc\_end | io\_file\_bitmap\_get, io\_alloc\_file\_tables, io\_free\_file\_tables, io\_install\_fixed\_file, \_\_io\_fixed\_fd\_install, io\_fixed\_fd\_install, io\_fixed\_fd\_remove | filetable.c   | Represents the io\_uring context, holds the file table and allocation range for files   |
| io\_file\_table               | filetable.c | bitmap, data (io\_rsrc\_data), alloc\_hint                          | io\_alloc\_file\_tables, io\_free\_file\_tables, io\_file\_bitmap\_get, io\_install\_fixed\_file, io\_fixed\_fd\_remove, io\_register\_file\_alloc\_range                   | filetable.c   | A table of files managed by the io\_uring context, tracks file allocation and resources |
| io\_rsrc\_node                | filetable.c | file resources (used with files), nodes list                        | io\_install\_fixed\_file, io\_fixed\_fd\_remove                                                                                                                             | filetable.c   | Represents a resource node in the file table, holds a file                              |
| io\_uring\_file\_index\_range | filetable.c | off, len, resv                                                      | io\_register\_file\_alloc\_range                                                                                                                                            | filetable.c   | Defines a range of file indices for allocation and registration in the file table       |

### filetable.h
| Structure Name                | Defined in            | Attributes                                                                               | Caller Functions Source            | Source Caller       | Usage                                     |
| ----------------------------- | --------------------- | ---------------------------------------------------------------------------------------- | ---------------------------------- | ------------------- | ----------------------------------------- |
| io\_ring\_ctx                 | file table operations | ctx, nr\_files, file\_slot, file\_ptr, alloc\_hint, file\_alloc\_start, file\_alloc\_end | io\_alloc\_file\_tables            | File table alloc    | Function parameters                       |
|                               |                       |                                                                                          | io\_free\_file\_tables             | File table free     | Function parameters                       |
|                               |                       |                                                                                          | io\_fixed\_fd\_install             | Fixed file install  | Function parameters                       |
|                               |                       |                                                                                          | \_\_io\_fixed\_fd\_install         | Fixed file install  | Function parameters                       |
|                               |                       |                                                                                          | io\_fixed\_fd\_remove              | Fixed file remove   | Function parameters                       |
|                               |                       |                                                                                          | io\_register\_file\_alloc\_range   | File range register | Function parameters                       |
|                               |                       |                                                                                          | io\_file\_get\_flags               | Retrieve file flags | Function parameters                       |
|                               |                       |                                                                                          | io\_file\_bitmap\_clear            | File bitmap clear   | Function parameters                       |
|                               |                       |                                                                                          | io\_file\_bitmap\_set              | File bitmap set     | Function parameters                       |
| io\_file\_table               | file table operations | table, bitmap, alloc\_hint                                                               | io\_file\_bitmap\_clear            | File bitmap clear   | Bitmap operations                         |
|                               |                       |                                                                                          | io\_file\_bitmap\_set              | File bitmap set     | Bitmap operations                         |
| io\_rsrc\_node                | resource node         | file\_ptr, flags                                                                         | io\_slot\_flags                    | Slot flag extract   | Inline function for flag extraction       |
|                               |                       |                                                                                          | io\_slot\_file                     | Slot file retrieval | Inline function for file retrieval        |
|                               |                       |                                                                                          | io\_fixed\_file\_set               | File set operation  | Inline function for file set              |
| io\_uring\_file\_index\_range | File allocation range | start, end                                                                               | io\_file\_table\_set\_alloc\_range | File range set      | Inline function for file range allocation |

### fs.c
| Structure name | Defined in | Attributes                                                                            | Caller Functions Source                                                             | Source caller | Usage                                                                                      |
| -------------- | ---------- | ------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- | ------------- | ------------------------------------------------------------------------------------------ |
| io\_rename     | fs.c       | file (struct file), old\_dfd, new\_dfd, oldpath (filename), newpath (filename), flags | io\_renameat\_prep, io\_renameat, io\_renameat\_cleanup                             | fs.c          | Holds data related to the rename operation (file paths, descriptors, and flags)            |
| io\_unlink     | fs.c       | file (struct file), dfd, flags, filename (filename)                                   | io\_unlinkat\_prep, io\_unlinkat, io\_unlinkat\_cleanup                             | fs.c          | Holds data related to the unlink operation (file paths, flags)                             |
| io\_mkdir      | fs.c       | file (struct file), dfd, mode (umode\_t), filename (filename)                         | io\_mkdirat\_prep, io\_mkdirat, io\_mkdirat\_cleanup                                | fs.c          | Holds data related to the mkdir operation (directory paths, mode)                          |
| io\_link       | fs.c       | file (struct file), old\_dfd, new\_dfd, oldpath (filename), newpath (filename), flags | io\_symlinkat\_prep, io\_symlinkat, io\_linkat\_prep, io\_linkat, io\_link\_cleanup | fs.c          | Holds data related to the symlink and link operations (file paths, descriptors, and flags) |

### fs.h
| Function Name         | Structure Name | Defined in                                     | Caller Functions Source                   | Source Caller     | Usage                                                         |
| --------------------- | -------------- | ---------------------------------------------- | ----------------------------------------- | ----------------- | ------------------------------------------------------------- |
| io\_renameat\_prep    | io\_kiocb      | Prepares parameters for `renameat` operation.  | io\_renameat, io\_renameat\_cleanup       | Rename operation  | Prepares for the rename operation using the SQE.              |
| io\_renameat          | io\_kiocb      | Executes the `renameat` operation.             | io\_renameat\_prep, io\_renameat\_cleanup | Rename operation  | Executes file renaming based on prepared parameters.          |
| io\_renameat\_cleanup | io\_kiocb      | Cleans up resources after rename operation.    | io\_renameat\_prep, io\_renameat          | Cleanup operation | Cleans up resources like file paths after renaming.           |
| io\_unlinkat\_prep    | io\_kiocb      | Prepares parameters for `unlinkat` operation.  | io\_unlinkat, io\_unlinkat\_cleanup       | Unlink operation  | Prepares for unlinking a file/directory.                      |
| io\_unlinkat          | io\_kiocb      | Executes the `unlinkat` operation.             | io\_unlinkat\_prep, io\_unlinkat\_cleanup | Unlink operation  | Performs unlinking of the file/directory.                     |
| io\_unlinkat\_cleanup | io\_kiocb      | Cleans up resources after unlink operation.    | io\_unlinkat\_prep, io\_unlinkat          | Cleanup operation | Cleans up resources like file name after unlinking.           |
| io\_mkdirat\_prep     | io\_kiocb      | Prepares parameters for `mkdirat` operation.   | io\_mkdirat, io\_mkdirat\_cleanup         | Mkdir operation   | Prepares for creating a directory at the specified location.  |
| io\_mkdirat           | io\_kiocb      | Executes the `mkdirat` operation.              | io\_mkdirat\_prep, io\_mkdirat\_cleanup   | Mkdir operation   | Executes directory creation with specified permissions.       |
| io\_mkdirat\_cleanup  | io\_kiocb      | Cleans up resources after mkdir operation.     | io\_mkdirat\_prep, io\_mkdirat            | Cleanup operation | Cleans up resources like the directory name after creation.   |
| io\_symlinkat\_prep   | io\_kiocb      | Prepares parameters for `symlinkat` operation. | io\_symlinkat                             | Symlink operation | Prepares for creating a symlink between source and target.    |
| io\_symlinkat         | io\_kiocb      | Executes the `symlinkat` operation.            | io\_symlinkat\_prep                       | Symlink operation | Executes symbolic link creation from source to target.        |
| io\_linkat\_prep      | io\_kiocb      | Prepares parameters for `linkat` operation.    | io\_linkat, io\_link\_cleanup             | Link operation    | Prepares for creating a hard link between source and target.  |
| io\_linkat            | io\_kiocb      | Executes the `linkat` operation.               | io\_linkat\_prep, io\_link\_cleanup       | Link operation    | Executes hard link creation from source to target.            |
| io\_link\_cleanup     | io\_kiocb      | Cleans up resources after link operation.      | io\_linkat\_prep, io\_linkat              | Cleanup operation | Cleans up resources like the old and new paths after linking. |

### futex.c
| **Structure name** | **Defined in** | **Attributes**                                                                                                                        | **Caller Functions Source**                                          | **Source caller** | **Usage**                                                                                              |
| ------------------ | -------------- | ------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------- | ------------------------------------------------------------------------------------------------------ |
| io_futex         | futex.c        | file (struct file), uaddr (user pointer), futex_val, futex_mask, futexv_owned, futex_flags, futex_nr, futexv_unqueued | io_futex_prep, io_futex_cancel, io_futex_wait, io_futex_wake | futex.c           | Holds data for futex operations (address, value, mask, flags, number of futexes, etc.)                 |
| io_futex_data    | futex.c        | q (futex\_q), req (struct io\_kiocb\*)                                                                                            | io_futex_wait, io_futex_complete, io_futex_wake_fn             | futex.c           | Holds the futex queue and request data for processing in asynchronous tasks.                           |
| futex_vector     | futex.c        | futexv (vector of futexes), futexv_owned, futexv_unqueued                                                                       | io_futexv_prep, io_futexv_complete, io_futexv_claim            | futex.c           | Manages operations on multiple futexes in the same vector for batch processing.                        |
| futex_q          | futex.c        | bitset, wake (callback function), wake_data                                                                                     | futex_queue, futex_unqueue, futex_wait_setup                   | futex.c           | Holds data for individual futex queue entries (bitset for mask, wake function pointer, etc.)           |
| io_futex_data    | futex.c        | q (futex\_q), req (io\_kiocb)                                                                                                     | io_futex_wait, io_futex_complete, io_futex_wake_fn             | futex.c           | Used for managing the task request and its corresponding futex queue when handling waits and wake-ups. |

### futex.h
| Function Name          | Structure Name                  | Defined in                                              | Caller Functions Source                        | Source Caller        | Usage                                              |
| ---------------------- | ------------------------------- | ------------------------------------------------------- | ---------------------------------------------- | -------------------- | -------------------------------------------------- |
| io\_futex\_prep        | io\_kiocb                       | Prepares a futex operation in the io\_kiocb request.    | io\_futex\_wait, io\_futex\_wake               | Futex operations     | Prepares for the futex operation using the SQE.    |
| io\_futexv\_prep       | io\_kiocb                       | Prepares a futex operation for a vectorized request.    | io\_futexv\_wait, io\_futex\_wake              | Futex operations     | Prepares for a vectorized futex operation.         |
| io\_futex\_wait        | io\_kiocb                       | Waits for a futex operation to complete.                | io\_futex\_prep, io\_futex\_wake               | Futex operations     | Waits for the completion of a futex operation.     |
| io\_futexv\_wait       | io\_kiocb                       | Waits for a vectorized futex operation to complete.     | io\_futexv\_prep, io\_futex\_wake              | Futex operations     | Waits for the completion of a vectorized futex.    |
| io\_futex\_wake        | io\_kiocb                       | Wakes up a futex operation.                             | io\_futex\_prep, io\_futex\_wait               | Futex operations     | Wakes up a futex operation after waiting.          |
| io\_futex\_cancel      | io\_ring\_ctx, io\_cancel\_data | Cancels a futex operation in the io\_ring context.      | io\_futex\_remove\_all, io\_futex\_cache\_init | Futex cancellation   | Cancels a futex operation using cancellation data. |
| io\_futex\_remove\_all | io\_ring\_ctx, io\_uring\_task  | Removes all futex operations from the io\_ring context. | io\_futex\_cancel, io\_futex\_cache\_free      | Futex cleanup        | Removes all futex operations for the task context. |
| io\_futex\_cache\_init | io\_ring\_ctx                   | Initializes the futex cache for the io\_ring context.   | io\_futex\_cache\_free                         | Cache initialization | Initializes futex cache for the ring context.      |
| io\_futex\_cache\_free | io\_ring\_ctx                   | Frees the futex cache for the io\_ring context.         | io\_futex\_cache\_init                         | Cache cleanup        | Frees the futex cache allocated in the context.    |