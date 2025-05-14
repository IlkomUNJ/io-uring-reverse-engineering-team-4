### opdef.c
| Structure name | Defined in        | Attributes                                                                                                                                                                                                          | Caller Functions Source | source caller         | usage                 |
| -------------- | ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------- | --------------------- | --------------------- |
| io\_op\_def    | io\_uring/opdef.c | .needs\_req, .needs\_file, .hash\_reg\_file, .pollin, .pollout, .unbound\_nonreg\_file, .buffer\_select, .fail\_supported, .iopoll, .async\_ctx, .raw, .twrl, .tw, .prep, .issue, .cleanup, .regs, .srclone, .flags | io\_op\_defs            | io\_uring/opdef.c     | static const array    |
| io\_op\_defs   | io\_uring/opdef.c | array of io\_op\_def                                                                                                                                                                                                | io\_op\_def\_get        | io\_uring/opdef.c     | function return value |
|                |                   | io\_op\_def\_flush                                                                                                                                                                                                  | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_def\_needs\_file                                                                                                                                                                                            | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_def\_needs\_req                                                                                                                                                                                             | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_def\_async\_ctx                                                                                                                                                                                             | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_def\_cancels                                                                                                                                                                                                | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_def\_hash\_reg\_file                                                                                                                                                                                        | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_defs\_tw\_supported                                                                                                                                                                                         | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_defs\_tw\_needs\_user                                                                                                                                                                                       | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_defs\_tw\_prefer\_tw                                                                                                                                                                                        | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_defs\_tw\_has\_tw                                                                                                                                                                                           | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_defs\_fail\_supported                                                                                                                                                                                       | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_defs\_buffer\_select                                                                                                                                                                                        | io\_uring/opdef.c       | function return value |                       |
|                |                   | io\_op\_defs\_unbound\_nonreg\_file                                                                                                                                                                                 | io\_uring/opdef.c       | function return value |                       |

### openclose.c
| Structure name | Defined in           | Attributes                            | Caller Functions Source | source caller           | usage              |
|----------------|----------------------|----------------------------------------|--------------------------|--------------------------|---------------------|
| open_how       | io_uring/openclose.c | flags, mode, resolve                   | io_openat_prep           | io_uring/openclose.c     | local variable      |
|                |                      |                                        | io_openat2_prep          |                          | local variable      |
|                |                      |                                        | io_renameat2_prep        |                          | local variable      |
|                |                      |                                        | io_unlinkat_prep         |                          | local variable      |
|                |                      |                                        | io_mkdirat_prep          |                          | local variable      |
|                |                      |                                        | io_symlinkat_prep        |                          | local variable      |
|                |                      |                                        | io_linkat_prep           |                          | local variable      |
|                |                      |                                        | io_renameat2             |                          | function parameter  |
|                |                      |                                        | io_openat2               |                          | function parameter  |
|                |                      |                                        | io_openat                |                          | function parameter  |
|                |                      |                                        | io_unlinkat              |                          | function parameter  |
|                |                      |                                        | io_mkdirat               |                          | function parameter  |
|                |                      |                                        | io_symlinkat             |                          | function parameter  |
|                |                      |                                        | io_linkat                |                          | function parameter  |
| io_open_how    | io_uring/openclose.c | mode, flags, resolve                   | io_openat2_prep          | io_uring/openclose.c     | local variable      |
|                |                      |                                        | io_openat2               |                          | function parameter  |
|                |                      |                                        | io_openat                |                          | function parameter  |
| filename       | io_uring/openclose.c | const char *name                       | io_openat2               | io_uring/openclose.c     | local variable      |
|                |                      |                                        | io_openat                |                          | local variable      |
|                |                      |                                        | io_renameat2             |                          | local variable      |
|                |                      |                                        | io_unlinkat              |                          | local variable      |
|                |                      |                                        | io_mkdirat               |                          | local variable      |
|                |                      |                                        | io_symlinkat             |                          | local variable      |
|                |                      |                                        | io_linkat                |                          | local variable      |
| path_openat    | io_uring/openclose.c | internal struct in Linux kernel       | io_openat2               | io_uring/openclose.c     | local variable      |
|                |                      |                                        | io_openat                |                          | local variable      |

### poll.c
| Structure name      | Defined in       | Attributes                                                              | Caller Functions Source   | source caller       | usage                  |
|---------------------|------------------|-------------------------------------------------------------------------|----------------------------|----------------------|------------------------|
| poll_table          | io_uring/poll.c  | queue_proc (function pointer), qproc_priv (void *)                      | io_poll_prep               | io_uring/poll.c      | local variable         |
|                     |                  |                                                                         | io_poll                    |                      | local variable         |
| poll_table_entry    | io_uring/poll.c  | wait_address, key, wait, next                                           | __pollwait                 | io_uring/poll.c      | local variable         |
|                     |                  |                                                                         | pollwake                   |                      | function parameter     |
| wait_queue_head_t   | io_uring/poll.c  | (kernel structure with spinlock and list_head)                          | io_poll_prep               | io_uring/poll.c      | accessed via file      |
|                     |                  |                                                                         | io_poll                    |                      | accessed via file      |
| wait_queue_entry_t  | io_uring/poll.c  | func (pollwake), private, flags, entry                                  | poll_table_entry.wait      | io_uring/poll.c      | embedded field         |
| io_poll_table       | io_uring/poll.c  | pt (poll_table), head (wait_queue_head_t *), req (io_kiocb *), canceled (bool) | io_poll_prep        | io_uring/poll.c      | local variable         |
|                     |                  |                                                                         | io_poll                    |                      | local variable         |
|                     |                  |                                                                         | io_poll_complete           |                      | function parameter     |
| file                | io_uring/poll.c  | (Linux kernel file struct)                                              | io_poll_prep               | io_uring/poll.c      | function parameter     |
|                     |                  |                                                                         | io_poll                    |                      | function parameter     |
| fasync_struct       | io_uring/poll.c  | (internal kernel async structure)                                       | io_poll_remove_one         | io_uring/poll.c      | local variable         |
| wait_queue_t        | io_uring/poll.c  | (used via poll_table_entry.wait, kernel wait queue entry)              | pollwake                   | io_uring/poll.c      | function parameter     |
| io_poll_iocb        | io_uring/poll.c  | (assumed from io_kiocb for poll specific tracking)                      | io_poll                    | io_uring/poll.c      | assumed request context|
| io_wq_work_node     | io_uring/poll.c  | next                                                                    | io_poll_remove_all         | io_uring/poll.c      | list manipulation      |
| poll_list           | io_uring/poll.c  | io_wq_work_node list                                                    | io_poll_remove_all         | io_uring/poll.c      | work queue list        |

### register.c
| Structure name       | Defined in         | Attributes                                                | Caller Functions Source   | source caller         | usage                  |
|----------------------|--------------------|------------------------------------------------------------|----------------------------|------------------------|------------------------|
| io_uring_file        | io_uring/register.c| file (struct file *), registered (bool), fasync (bool)     | io_files_update            | io_uring/register.c    | local variable         |
|                      |                    |                                                            | io_files_cleanup           |                        | local variable         |
|                      |                    |                                                            | io_files_associate         |                        | function parameter     |
| io_fixed_file        | io_uring/register.c| file (struct file *), ctx (struct io_ring_ctx *), slot     | io_files_update            | io_uring/register.c    | local variable         |
|                      |                    |                                                            | io_files_cleanup           |                        | local variable         |
|                      |                    |                                                            | io_file_get_fixed          |                        | function parameter     |
| io_rsrc_data         | io_uring/register.c| kref, ctx, type, io_rsrc_put_fn, io_rsrc_data_refs         | io_sqe_buffers_register    | io_uring/register.c    | local variable         |
|                      |                    |                                                            | io_sqe_files_register      |                        | local variable         |
|                      |                    |                                                            | io_file_bitmap_alloc       |                        | function parameter     |
| io_rsrc_node         | io_uring/register.c| resource-specific metadata                                 | io_rsrc_node_alloc         | io_uring/register.c    | local variable         |
|                      |                    |                                                            | io_rsrc_ref_quiesce        |                        | local variable         |
| fixed_file_table     | io_uring/register.c| table of io_fixed_file, bitmap, nr                         | io_register_files          | io_uring/register.c    | local variable         |
|                      |                    |                                                            | io_unregister_files        |                        | local variable         |
|                      |                    |                                                            | io_file_table_alloc        |                        | function parameter     |
| io_file_ref          | io_uring/register.c| file (struct file *), count                                | io_file_get                | io_uring/register.c    | local variable         |
|                      |                    |                                                            | io_file_put                |                        | function parameter     |

### rsrc.c
| Structure name  | Defined in       | Attributes                                                              | Caller Functions Source     | source caller       | usage                |
|-----------------|------------------|-------------------------------------------------------------------------|------------------------------|----------------------|----------------------|
| io_rsrc_node    | io_uring/rsrc.c  | list_head, kref, io_rsrc_data *, quiesce                                | io_rsrc_ref_quiesce          | io_uring/rsrc.c      | local variable       |
|                 |                  |                                                                         | io_rsrc_node_destroy         |                      | function parameter   |
| io_rsrc_data    | io_uring/rsrc.c  | refs, ctx, type, io_rsrc_put_fn *, io_rsrc_refs                         | io_rsrc_node_destroy         | io_uring/rsrc.c      | dereferenced pointer |
| io_rsrc_refs    | io_uring/rsrc.c  | refs (atomic), completion                                               | io_rsrc_data_release         | io_uring/rsrc.c      | member field         |
|                 |                  |                                                                         | io_rsrc_refs_ref             |                      | local variable       |
| io_rsrc_put_fn  | io_uring/rsrc.c  | typedef void (*)(struct io_ring_ctx *, void *)                          | io_rsrc_put                  | io_uring/rsrc.c      | function pointer     |
| io_rsrc_put     | io_uring/rsrc.c  | fn (io_rsrc_put_fn), data (void *)                                      | io_rsrc_data_put             | io_uring/rsrc.c      | local variable       |
| io_ring_ctx     | io_uring/rsrc.c  | ctx-wide structure used throughout io_uring                             | io_rsrc_data_put             | io_uring/rsrc.c      | function parameter   |

### rw.c
| Structure name      | Defined in       | Attributes                                                              | Caller Functions Source     | source caller       | usage                |
|---------------------|------------------|-------------------------------------------------------------------------|------------------------------|----------------------|----------------------|
| io_rw_state         | io_uring/rw.c    | file (struct file *), iov, iter, offset, flags                          | io_read                      | io_uring/rw.c        | local variable       |
|                     |                  |                                                                         | io_write                     |                      | local variable       |
| io_rw_bytes         | io_uring/rw.c    | size, done, file                                                        | io_read                      | io_uring/rw.c        | local variable       |
|                     |                  |                                                                         | io_write                     |                      | local variable       |
| iovec               | io_uring/rw.c    | base (void *), len (size_t)                                             | io_read                      | io_uring/rw.c        | member of iov        |
|                     |                  |                                                                         | io_write                     |                      | member of iov        |
| kiocb               | io_uring/rw.c    | struct from Linux kernel representing an IO control block              | io_rw_init_file              | io_uring/rw.c        | local variable       |
|                     |                  |                                                                         | io_read                      |                      | used for submission  |
|                     |                  |                                                                         | io_write                     |                      | used for submission  |
| bio_vec             | io_uring/rw.c    | page, len, offset                                                       | used with iovec or iter      | io_uring/rw.c        | implicit structure   |
| iov_iter            | io_uring/rw.c    | type, iov/bvec, count, index, offset                                    | io_read                      | io_uring/rw.c        | local variable       |
|                     |                  |                                                                         | io_write                     |                      | local variable       |
| io_kiocb            | io_uring/rw.c    | core request context in io_uring                                        | io_read                      | io_uring/rw.c        | function parameter   |
|                     |                  |                                                                         | io_write                     |                      | function parameter   |

### splice.c
| Structure name       | Defined in         | Attributes                                                               | Caller Functions Source     | source caller       | usage                |
|----------------------|--------------------|--------------------------------------------------------------------------|------------------------------|----------------------|----------------------|
| io_splice_state      | io_uring/splice.c  | src_file (file *), dest_file (file *), len, flags                        | io_tee_prep                  | io_uring/splice.c    | local variable       |
|                      |                    |                                                                          | io_splice_prep               |                      | local variable       |
| io_kiocb             | io_uring/splice.c  | internal io_uring request structure                                      | io_tee                       | io_uring/splice.c    | function parameter   |
|                      |                    |                                                                          | io_splice                    |                      | function parameter   |
| splice_desc          | io_uring/splice.c  | total_len, len, flags, pos                                               | io_tee                       | io_uring/splice.c    | local variable       |
|                      |                    |                                                                          | io_splice                    |                      | local variable       |
| file                 | io_uring/splice.c  | kernel file abstraction                                                  | io_tee                       | io_uring/splice.c    | from io_kiocb        |
|                      |                    |                                                                          | io_splice                    |                      | from io_kiocb        |