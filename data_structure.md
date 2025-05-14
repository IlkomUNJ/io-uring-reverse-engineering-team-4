# Task 3: Data Structure Investigation
The objective of this task is to document all internal data structures defined in io_uring. 

Structure name | Defined in | Attributes | Caller Functions Source | source caller | usage
---------------|------------|------------|-------------------------|---------------|-------------------
io_ev_fd       | io_uring/eventfd.c | eventfd_ctx, uint, uint, refcount_t, atomic_t, rcu_head | io_eventfd_free | io_uring/eventfd.c | local variable
| | | | io_eventfd_put | io_uring/eventfd.c | function parameter
| | | | io_eventfd_do_signal | io_uring/eventfd.c | local variable, function parameter
| | | | __io_eventfd_signal | io_uring/eventfd.c | function parameter
| | | | io_eventfd_grab | io_uring/eventfd.c | return value, local variable
| | | | io_eventfd_signal | io_uring/eventfd.c | local variable 
| | | | io_eventfd_flush_signal | io_uring/eventfd.c | local variable
| | | | io_eventfd_register | io_uring/eventfd.c | local variable
| | | | io_eventfd_unregister | io_uring/eventfd.c | function parameter

If the following row value in a column is missing, assume the value is the same with the previous row in the same column. 
Continue until all data structures documented properly.


Structure name | Defined in | Attributes | Caller Functions Source | source caller | usage
---------------|------------|------------|-------------------------|---------------|-------------------
io_worker      | io_uring/io_wq.c | refcount_t, ulong, hlist_nulls_node, list_head, task_struct, io_wq, io_wq_acct, io_wq_work, raw_spinlock_t, completion, ulong, callback_head, int, rcu_head, delayed_work | io_wq_dec_running | io_uring/io_wq.c | function parameter
| | | | io_worker_get | io_uring/io_wq.c | function parameter
| | | | io_wq_acct | io_uring/io_wq.c | function parameter
| | | | io_wq_worker_stopped | io_uring/io_wq.c | local variable
| | | | void io_worker_cancel_cb | io_uring/io_wq.c | function parameter, local variable
| | | | io_task_worker_match | io_uring/io_wq.c | local variable
| | | | io_worker_exit| io_uring/io_wq.c | function parameter
| | | | io_worker_match | io_uring/io_wq.c | function parameter
| | | | io_acct_activate_free_worker | io_uring/io_wq.c | local variable
| | | | io_wq_inc_running | io_uring/io_wq.c | function parameter
| | | | create_worker_cb | io_uring/io_wq.c | local variable
| | | | io_queue_worker_create | io_uring/io_wq.c | function parameter
| | | | io_wq_dec_running | io_uring/io_wq.c | function parameter
| | | | __io_worker_busy | io_uring/io_wq.c | function parameter
| | | | __io_worker_idle | io_uring/io_wq.c | function parameter
| | | | io_assign_current_work | io_uring/io_wq.c | function parameter
| | | | io_worker_handle_work| io_uring/io_wq.c | local variable
| | | | io_wq_worker| io_uring/io_wq.c | local variable
| | | | io_wq_worker_running| io_uring/io_wq.c | local variable
| | | | io_wq_worker_sleeping| io_uring/io_wq.c | local variable
| | | | io_init_new_worker | io_uring/io_wq.c | function parameter
| | | | io_should_retry_thread | io_uring/io_wq.c | function parameter
| | | | queue_create_worker_retry | io_uring/io_wq.c | function parameter
| | | | create_worker_cont | io_uring/io_wq.c | local variable
| | | | io_workqueue_create | io_uring/io_wq.c | local variable
| | | | create_io_worker | io_uring/io_wq.c | local variable
| | | | io_acct_for_each_worker | io_uring/io_wq.c | function parameter, local variable
| | | | io_wq_for_each_worker | io_uring/io_wq.c | function parameter
| | | | io_wq_worker_wake | io_uring/io_wq.c | function parameter
| | | | __io_wq_worker_cancel | io_uring/io_wq.c | function parameter
| | | | io_wq_worker_cancel | io_uring/io_wq.c | function parameter
| | | | io_task_work_match | io_uring/io_wq.c | local variable
| | | | io_wq_cancel_tw_create | io_uring/io_wq.c | local variable
| | | | io_wq_worker_affinity | io_uring/io_wq.c | function parameter
io_wq      | io_uring/io_wq.c | ulong, free_work_fn, io_wq_work_fn, io_wq_hash, atomic_t, completion, hlist_node, task_struct, io_wq_acct, wait_queue, io_wq_work, cpumask_var_t | create_io_worker | io_uring/io_wq.c | function parameter
| | | | io_acct_cancel_pending_work | io_uring/io_wq.c | function parameter
| | | | io_wq_cancel_tw_create | io_uring/io_wq.c | function parameter
| | | | *io_get_acct | io_uring/io_wq.c | function parameter
| | | | *io_work_get_acct | io_uring/io_wq.c | function parameter
| | | | io_worker_ref_put | io_uring/io_wq.c | function parameter
| | | | io_worker_cancel_cb | io_uring/io_wq.c | local variable
| | | | io_worker_exit | io_uring/io_wq.c | local variable
| | | | io_wq_create_worker | io_uring/io_wq.c | function parameter
| | | | create_worker_cb | io_uring/io_wq.c | local variable
| | | | io_queue_worker_create | io_uring/io_wq.c | local variable
| | | | io_wq_dec_running | io_uring/io_wq.c | local variable
| | | | io_wait_on_hash | io_uring/io_wq.c | function parameter
| | | | io_wq_work | io_uring/io_wq.c | function parameter
| | | | io_worker_handle_work | io_uring/io_wq.c | local variable
| | | | io_wq_worker | io_uring/io_wq.c | local variable
| | | | io_init_new_worker | io_uring/io_wq.c | function parameter
| | | | create_worker_cont | io_uring/io_wq.c | local variable
| | | | create_io_worker | io_uring/io_wq.c | function parameter
| | | | io_wq_for_each_worker | io_uring/io_wq.c | function parameter
| | | | io_run_cancel | io_uring/io_wq.c | function parameter
| | | | io_wq_insert_work | io_uring/io_wq.c | function parameter
| | | | io_wq_enqueue | io_uring/io_wq.c | function parameter
| | | | io_wq_remove_pending | io_uring/io_wq.c | function parameter
| | | | io_acct_cancel_pending_work | io_uring/io_wq.c | function parameter
| | | | io_wq_cancel_pending_work | io_uring/io_wq.c | function parameter
| | | | io_wq_cancel_running_work | io_uring/io_wq.c | function parameter
| | | | io_wq_cancel_cb | io_uring/io_wq.c | function parameter
| | | | io_wq_hash_wake | io_uring/io_wq.c | local variable
| | | | *io_wq_create | io_uring/io_wq.c | return value, local variable
| | | | io_wq_exit_start | io_uring/io_wq.c | function parameter
| | | | io_wq_cancel_tw_create | io_uring/io_wq.c | function parameter
| | | | io_wq_exit_workers| io_uring/io_wq.c | function parameter
| | | | io_wq_destroy| io_uring/io_wq.c | function parameter
| | | | io_wq_put_and_exit| io_uring/io_wq.c | function parameter
| | | | __io_wq_cpu_online| io_uring/io_wq.c | function parameter
| | | | io_wq_cpu_online | io_uring/io_wq.c | local variable
| | | | io_wq_cpu_offline | io_uring/io_wq.c | local variable
io_provide_buf       | io_uring/kbuf.c | file, __u64, __u32, __u32, __u32, __u32, __u32, __u16 | io_remove_buffers_prep | io_uring/kbuf.c | local variable
| | | | io_remove_buffers | io_uring/kbuf.c | local variable
| | | | io_remove_buffers_prep | io_uring/kbuf.c | local variable
| | | | io_add_buffers | io_uring/kbuf.c | function parameter
| | | | io_provide_buffers | io_uring/kbuf.c | local variable
| | | | io_add_buffers | io_uring/kbuf.c | function parameter
io_msg       | io_uring/msg_ring.c | file, file, callback_head, u64, u32, u32, u32, u32, u32, u32 | io_msg_ring_cleanup | io_uring/msg_ring.c | local variable
| | | | io_msg_data_remote | io_uring/msg_ring.c | function parameter
| | | | __io_msg_ring_data | io_uring/msg_ring.c | function parameter
| | | | io_msg_ring_data | io_uring/msg_ring.c | local variable
| | | | io_msg_grab_file | io_uring/msg_ring.c | local variable
| | | | io_msg_grab_file | io_uring/msg_ring.c | local variable
| | | | io_msg_install_complete | io_uring/msg_ring.c | local variable
| | | | io_msg_tw_fd_complete | io_uring/msg_ring.c | local variable
| | | | io_msg_fd_remote | io_uring/msg_ring.c | local variable
| | | | io_msg_send_fd | io_uring/msg_ring.c | local variable
| | | | __io_msg_ring_prep | io_uring/msg_ring.c | function parameter
| | | | io_msg_ring | io_uring/msg_ring.c | local variable
| | | | io_uring_sync_msg_ring | io_uring/msg_ring.c | local variable
io_napi_entry       | io_uring/napi.c | uint, list_head, ulong, hlist_node, rcu_head | io_napi_entry | io_uring/napi.c | return value, local variable
| | | | __io_napi_add_id | io_uring/napi.c | local variable
| | | | __io_napi_del_id | io_uring/napi.c | local variable
| | | | __io_napi_remove_stale | io_uring/napi.c | local variable
| | | | static_tracking_do_busy_loop | io_uring/napi.c | local variable
| | | | dynamic_tracking_do_busy_loop | io_uring/napi.c | local variable
| | | | io_napi_free | io_uring/napi.c | local variable
io_shutdown        | io_uring/net.c | file, int | io_shutdown_prep | io_uring/net.c | local variable
| | | | io_shutdown | io_uring/net.c | local variable
io_accept       | io_uring/net.c | file, sockaddr, __user, int, int, u32, ulong | io_accept_prep | io_uring/net.c | local variable
| | | | io_accept | io_uring/net.c | local variable
| | | | io_accept | io_uring/net.c | local variable
io_socket       | io_uring/net.c | file, int, int, int, int, u32, ulong | io_socket_prep | io_uring/net.c | local variable
| | | | io_socket | io_uring/net.c | local variable
io_connect       | io_uring/net.c | file, sockaddr, int, bool, bool | io_connect_prep | io_uring/net.c | local variable
| | | | io_connect | io_uring/net.c | local variable
io_bind       | io_uring/net.c | file, int | io_bind_prep | io_uring/net.c | local variable
| | | | io_bind | io_uring/net.c | local variable
io_listen       | io_uring/net.c | file, int | io_listen_prep | io_uring/net.c | local variable
| | | | io_listen | io_uring/net.c | local variable
io_sr_msg       | io_uring/net.c | file, compat msghdr, user_msghdr, __user, int, uint, uint, uint, u16, u16, bool, __user, io_kiocb | io_mshot_prep_retry | io_uring/net.c | local variable
| | | | io_compat_msg_copy_hdr | io_uring/net.c | local variable
| | | | io_msg_copy_hdr | io_uring/net.c | local variable
| | | | io_send_setup | io_uring/net.c | local variable
| | | | io_sendmsg_setup | io_uring/net.c | local variable
| | | | io_sendmsg_prep | io_uring/net.c | local variable
| | | | io_send_finish | io_uring/net.c | local variable
| | | | io_sendmsg | io_uring/net.c | local variable
| | | | io_send_select_buffer | io_uring/net.c | local variable
| | | | io_send | io_uring/net.c | local variable
| | | | io_recvmsg_prep_setup | io_uring/net.c | local variable
| | | | io_recvmsg_prep | io_uring/net.c | local variable
| | | | io_recv_finish | io_uring/net.c | local variable
| | | | io_recvmsg_prep_multishot | io_uring/net.c | function parameter
| | | | io_recvmsg_multishot | io_uring/net.c | function parameter
| | | | io_recvmsg | io_uring/net.c | local variable
| | | | io_recv_buf_select | io_uring/net.c | local variable
| | | | io_recv | io_uring/net.c | local variable
| | | | io_send_zc_cleanup | io_uring/net.c | local variable
| | | | io_send_zc_prep | io_uring/net.c | local variable
| | | | io_send_zc_import | io_uring/net.c | local variable
| | | | io_send_zc | io_uring/net.c | local variable
| | | | io_sendmsg_zc | io_uring/net.c | local variable
| | | | io_sendrecv_fail | io_uring/net.c | local variable
io_recvzc       | io_uring/net.c | file, int, int, uint | io_recvzc_prep | io_uring/net.c | local variable
| | | | io_recvzc | io_uring/net.c | local variable
io_nop       | io_uring/nop.c | file, int, int, uint | io_nop_prep | io_uring/nop.c | local variable
| | | | io_nop | io_uring/nop.c | local variable
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
| io_rsrc_node    | io_uring/rsrc.c  | list_head, kref, io_rsrc_data *, quiesce                                | io_rsrc_ref_quiesce          | io_uring/rsrc.c      | local variable       |
|                 |                  |                                                                         | io_rsrc_node_destroy         |                      | function parameter   |
| io_rsrc_data    | io_uring/rsrc.c  | refs, ctx, type, io_rsrc_put_fn *, io_rsrc_refs                         | io_rsrc_node_destroy         | io_uring/rsrc.c      | dereferenced pointer |
| io_rsrc_refs    | io_uring/rsrc.c  | refs (atomic), completion                                               | io_rsrc_data_release         | io_uring/rsrc.c      | member field         |
|                 |                  |                                                                         | io_rsrc_refs_ref             |                      | local variable       |
| io_rsrc_put_fn  | io_uring/rsrc.c  | typedef void (*)(struct io_ring_ctx *, void *)                          | io_rsrc_put                  | io_uring/rsrc.c      | function pointer     |
| io_rsrc_put     | io_uring/rsrc.c  | fn (io_rsrc_put_fn), data (void *)                                      | io_rsrc_data_put             | io_uring/rsrc.c      | local variable       |
| io_ring_ctx     | io_uring/rsrc.c  | ctx-wide structure used throughout io_uring                             | io_rsrc_data_put             | io_uring/rsrc.c      | function parameter   |
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
| io_splice_state      | io_uring/splice.c  | src_file (file *), dest_file (file *), len, flags                        | io_tee_prep                  | io_uring/splice.c    | local variable       |
|                      |                    |                                                                          | io_splice_prep               |                      | local variable       |
| io_kiocb             | io_uring/splice.c  | internal io_uring request structure                                      | io_tee                       | io_uring/splice.c    | function parameter   |
|                      |                    |                                                                          | io_splice                    |                      | function parameter   |
| splice_desc          | io_uring/splice.c  | total_len, len, flags, pos                                               | io_tee                       | io_uring/splice.c    | local variable       |
|                      |                    |                                                                          | io_splice                    |                      | local variable       |
| file                 | io_uring/splice.c  | kernel file abstraction                                                  | io_tee                       | io_uring/splice.c    | from io_kiocb        |
|                      |                    |                                                                          | io_splice                    |                      | from io_kiocb        |
