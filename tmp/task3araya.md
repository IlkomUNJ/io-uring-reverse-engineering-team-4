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
