### io_wq.c
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
io-wq.c | /include/linux/refcount.h | refcount_inc_not_zero | 1
io-wq.c| io-wq.h| io_get_acct | 2
io-wq.c | /include/linux/refcount.h | refcount_dec_and_test | 1
io-wq.c | /include/linux/completion.h | complete | 2
io-wq.c | /include/linux/atomic/atomic-instrumented.h | atomic_dec_and_test | 2
io-wq.c | /include/asm-generic/bug.h | WARN_ON_ONCE | 16
io-wq.c | /include/linux/bitops.h | test_bit | 19
io-wq.c | /include/linux/container_of.h | container_of | 10
io-wq.c | io-wq.h | io_wq_get_acct | 7
io-wq.c | /tools/memory-model/linux-kernel.def | atomic_dec | 5
io-wq.c | include/linux/spinlock.h | raw_spin_lock | 22
io-wq.c | include/linux/spinlock.h | raw_spin_unlock | 26
io-wq.c | io-wq.h | io_worker_release | 8
io-wq.c | include/linux/task_work.h: | task_work_cancel_match | 2
io-wq.c | io-wq.h | io_worker_cancel_cb | 2
io-wq.c | include/linux/completion.h | wait_for_completion | 2
io-wq.c | include/linux/rculist_nulls.h | hlist_nulls_del_rcu | 1
io-wq.c | io-wq.c | io_wq_dec_running | 3
io-wq.c | include/linux/rcupdate.h | kfree_rcu | 1
io-wq.c | include/linux/compiler_types.h | __acquires | 1
io-wq.c | io-wq.h | __io_acct_run_queue | 2
io-wq.c | include/linux/compiler_types.h | __must_hold | 3
io-wq.c | io-wq.h | io_worker_get | 3
io-wq.c | include/linux/sched.h | wake_up_process | 2
io-wq.c | include/linux/compiler.h | unlikely | 1
io-wq.c | include/linux/printk.h | pr_warn_once | 1
io-wq.c | include/linux/atomic/atomic-instrumented.h | atomic_inc | 6
io-wq.c | io-wq.c | create_io_worker | 3
io-wq.c | io-wq.h | io_worker_ref_put | 9
io-wq.c | include/linux/kernel.h | do_exit | 1
io-wq.c | include/asm-generic/bitops/instrumented-lock.h | test_and_set_bit_lock | 1
io-wq.c | include/linux/task_work.h | init_task_work | 1
io-wq.c | include/linux/task_work.h | task_work_add | 1
io-wq.c | io-wq.h | io_wq_cancel_tw_create | 3
io-wq.c | /include/linux/bitops.h | clear_bit_unlock | 4
io-wq.c | /include/linux/bitops.h | set_bit | 7
io-wq.c | include/linux/rculist_nulls.h | hlist_nulls_add_head_rcu | 2
io-wq.c | io-wq.h | __io_get_work_hash | 4
io-wq.c | /include/linux/atomic/atomic-instrumented.h| atomic_read | 5
io-wq.c | include/linux/spinlock.h | raw_spin_lock | 22
io_wq.c | include/linux/fortify-string.h | kfree | 6
io_wq.c | io-wq.c | io_acct_cancel_pending_work | 5
io_wq.c | io-wq.h | io_acct_run_queue | 5
io_wq.c | include/linux/err.h | ERR_PTR | 4
io_wq.c | include/linux/sched.h | __set_current_state | 4
io_wq.c | include/linux/atomic/atomic-instrumented.h | atomic_or | 4
io_wq.c | io-wq.h | io_assign_current_work | 4
io_wq.c | io-wq.c | io_wq_cancel_tw_create | 4
io_wq.c | include/linux/build-bug.h | BUILD_BUG_ON | 3
io_wq.c | io-wq.h | __io_wq_cpu_online | 3
io_wq.c | io-wq.h | __io_wq_is_hashed | 3
io_wq.c | Unknown | free_cpumask_var | 3
io_wq.c | io-wq.h | io_acct_activate_free_worker | 2
io_wq.c | io-wq.h | io_acct_for_each_worker | 2
io_wq.c | io-wq.h | io_init_new_worker | 2
io_wq.c | io-wq.h | io_queue_worker_create | 2
io_wq.c | io-wq.h | io_run_cancel | 2
io_wq.c | io-wq.h | io_worker_handle_work | 2
io_wq.c | io-wq.h | io_wq_cancel_pending_work | 2
io_wq.c | io-wq.h | io_wq_exit_workers | 2
io_wq.c | io-wq.h | io_wq_for_each_worker | 2
io_wq.c | include/linux/list.h | list_del_init | 3
io_wq.c | io-wq.h | queue_create_worker_retry | 3
io_wq.c | include/linux/spinlock.h| raw_spin_lock_init | 3
io_wq.c | include/linux/ieee80211.h | sizeof | 3
io_wq.c | include/linux/spinlock_rt.h | spin_lock_irq | 3
io_wq.c | include/linux/spinlock_rt.h | spin_unlock_irq | 3
io_wq.c | include/linux/sched/signal.h | task_rlimit | 3
io_wq.c | include/linux/wait.h | wake_up | 3
io_wq.c | include/vdso/bits.h | BIT | 2
io_wq.c | include/linux/list.h | INIT_LIST_HEAD | 2
io_wq.c | include/linux/err.h | IS_ERR | 2
io_wq.c | include/linux/rculist_nulls.h | RCU | 2
io_wq.c | io-wq.h | __io_worker_busy | 1
io_wq.c | io-wq.h | __io_worker_idle | 1
io_wq.c | io-wq.h | __io_wq_worker_cancel | 1
io_wq.c | include/linux/sched/signal.h | __set_notify_signal | 2
io_wq.c | include/linux/cpumask.h | alloc_cpumask_var | 2
io_wq.c | include/linux/atomic/atomic-instrumented.h | atomic_set | 2
io_wq.c | include/linux/cpumask.h | cpumask_copy | 2
io_wq.c | include/linux/cpuset.h | cpuset_cpus_allowed | 2
io_wq.c | include/linux/sched/task.h | create_io_thread | 2
io_wq.c | io-wq.c | create_worker_cb | 2
io_wq.c | include/linux/list.h| hlist_entry_safe | 2
io_wq.c | include/linux/list.h | hlist_nulls_add_head_rcu | 2
io_wq.c | include/linux/completion.h | init_completion | 2
io_wq.c | io-wq.h | io_get_next_work | 2
io_wq.c | io-wq.h | io_run_task_work | 2
io_wq.c | io-wq.h | io_wait_on_hash | 2
io_wq.c | io-wq.h | io_work_get_acct | 2
io_wq.c | io-wq.h | io_worker_exit | 2
io_wq.c | io-wq.h | io_wq_cancel_running_work | 2
io_wq.c | io-wq.h | io_wq_create_worker | 2
io_wq.c | io-wq.h | io_wq_destroy | 2
io_wq.c | io-wq.h | io_wq_enqueue | 2
io_wq.c | io-wq.h | io_wq_inc_running | 2
io_wq.c | io-wq.h | io_wq_insert_work | 2
io_wq.c | io-wq.h | io_wq_is_hashed | 2
io_wq.c | io-wq.h | io_wq_put_hash | 2
io_wq.c | io-wq.h | io_wq_remove_pending | 2
io_wq.c | include/linux/slab.h| kzalloc | 2
io_wq.c | linux/task_work.h | task | 2
io_wq.c | include/linux/task_work.h | task_work_cancel_match | 2
io_wq.c | include/linux/task_work.h | wq_has_sleeper | 2
io_wq.c | include/linux/workqueue.h | INIT_DELAYED_WORK | 1
io_wq.c | include/linux/list_nulls.h | INIT_HLIST_NULLS_HEAD | 1
io_wq.c | include/linux/wait.h | __add_wait_queue | 1
io_wq.c | include/linux/compiler_types.h | __releases | 1
io_wq.c | include/linux/sched.h | cond_resched | 1
io_wq.c | include/linux/cpuhotplug.h | cpuhp_setup_state_multi | 1
io_wq.c | include/linux/cpuhotplug.h| cpuhp_state_add_instance_nocalls | 1
io_wq.c | include/linux/cpuhotplug.h | cpuhp_state_remove_instance_nocalls | 1
io_wq.c | include/linux/cpumask.h| cpumask_clear_cpu | 1
io_wq.c | include/linux/cpumask.h | cpumask_set_cpu | 1
io_wq.c | include/linux/cpumask.h | cpumask_subset | 1
io_wq.c | include/linux/cpumask.h | cpumask_test_cpu | 1
io_wq.c | io-wq.h  | create_worker_cont | 1
io_wq.c | include/linux/sched/signal.h | fatal_signal_pending | 1
io_wq.c | include/linux/signal.h | get_signal | 1
io_wq.c | include/linux/sched/task.h | get_task_struct | 1
io_wq.c | include/linux/hash.h | hash_ptr | 1
io_wq.c | include/linux/rculist_nulls.h | hlist_nulls_del_init_rcu | 1
io_wq.c | include/linux/rculist_nulls.h | hlist_nulls_for_each_entry_rcu | 1
io_wq.c | include/linux/task_work.h | init_task_work | 1
io_wq.c | io-wq.h | io_should_retry_thread | 1
io_wq.c | io-wq.h | io_task_work_match | 1
io_wq.c | io-wq.h | io_task_worker_match | 1
io_wq.c | io-wq.h | io_workqueue_create | 1
io_wq.c | io-wq.h | io_wq_cancel_cb | 1
io_wq.c | io-wq.h | io_wq_cpu_affinity | 1
io_wq.c | io-wq.h | io_wq_cpu_offline | 1
io_wq.c | io-wq.h | io_wq_cpu_online | 1
io_wq.c | io-wq.h | io_wq_create | 1
io_wq.c | io-wq.h | io_wq_current_is_worker | 1
io_wq.c | io-wq.h | io_wq_exit_start | 1
io_wq.c | io-wq.h | io_wq_hash_wake | 1
io_wq.c | io-wq.h | io_wq_hash_work | 1
io_wq.c | io-wq.h | io_wq_init | 1
io_wq.c | io-wq.h | io_wq_max_workers | 1
io_wq.c | io-wq.h | io_wq_put_and_exit | 1
io_wq.c | io-wq.h | io_wq_work_match_all | 1
io_wq.c | io-wq.h | io_wq_work_match_item | 1
io_wq.c | io-wq.h | io_wq_worker | 1
io_wq.c | io-wq.h | io_wq_worker_affinity | 1
io_wq.c | io-wq.h | io_wq_worker_cancel | 1
io_wq.c | io-wq.h | io_wq_worker_running | 1
io_wq.c | io-wq.h | io_wq_worker_sleeping | 1
io_wq.c | io-wq.h | io_wq_worker_stopped | 1
io_wq.c | io-wq.h | io_wq_worker_wake | 1
io_wq.c | include/linux/compiler.h | likely | 1
io_wq.c | include/linux/rculist.h: | list_add_tail_rcu | 1
io_wq.c | include/linux/rculist.h: | list_del_rcu | 1
io_wq.c | include/linux/list.h | list_empty | 1
io_wq.c | include/linux/rculist.h: | list_for_each_entry_rcu | 1
io_wq.c | include/linux/minmax.h | max_t | 1
io_wq.c | include/linux/jiffies.h | msecs_to_jiffies | 1
io_wq.c | include/linux/sched/task.h: | put_task_struct | 1
io_wq.c | include/linux/smp.h | raw_smp_processor_id | 1
io_wq.c | include/linux/refcount.h | refcount_inc | 1
io_wq.c | include/linux/refcount.h | refcount_set | 1
io_wq.c | include/linux/workqueue.h | schedule_delayed_work | 1
io_wq.c | include/linux/sched.h | schedule_timeout | 1
io_wq.c | include/linux/sched.h| set_cpus_allowed_ptr | 1
io_wq.c | include/linux/sched.h| set_current_state | 1
io_wq.c | include/linux/sched.h | set_mask_bits | 1
io_wq.c | include/linux/sched.h | set_task_comm | 1
io_wq.c | include/linux/sched/signal.h: | signal_pending | 1
io_wq.c | include/linux/sprintf.h | snprintf | 1
io_wq.c | include/linux/init.h | subsys_initcall | 1
io_wq.c | include/linux/blk_types.h | switch | 1
io_wq.c | include/linux/task_work.h | task_work_add | 1
io_wq.c | include/asm-generic/bitops/instrumented-atomic.h | test_and_clear_bit | 1
io_wq.c | include/asm-generic/bitops/instrumented-atomic.h | test_and_set_bit | 1
io_wq.c | include/linux/sched.h | wake_up_new_task | 1



### io_wq.h
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
io-wq.h | io-wq.h | __io_wq_is_hashed | 2
io-wq.h | io-wq.h | io_wq_worker_running | 2
io-wq.h | io-wq.h | io_wq_worker_sleeping | 2
io-wq.h | io-wq.h | atomic_read | 1
io-wq.h | io-wq.h | in_task | 1
io-wq.h | io-wq.h | io_wq_cancel_cb | 1
io-wq.h | io-wq.h | io_wq_cpu_affinity | 1
io-wq.h | io-wq.h | io_wq_create | 1
io-wq.h | io-wq.h | io_wq_current_is_worker | 1
io-wq.h | io-wq.h | io_wq_enqueue | 1
io-wq.h | io-wq.h | io_wq_exit_start | 1
io-wq.h | io-wq.h| io_wq_hash_work | 1
io-wq.h | io-wq.h | io_wq_is_hashed | 1
io-wq.h | io-wq.h | io_wq_max_workers | 1
io-wq.h | io-wq.h | io_wq_put_and_exit | 1
io-wq.h | io-wq.h | io_wq_put_hash | 1
io-wq.h | io-wq.h | io_wq_worker_stopped | 1
io-wq.h | include/linux/fortify-string.h | kfree | 1
io-wq.h | /include/linux/refcount.h | refcount_dec_and_test | 1



### kbuf.c
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
kbuf.c | include/linux/compiler.h | unlikely | 11
kbuf.c | io_uring/kbuf.c | io_buffer_get_list | 10
kbuf.c | include/linux/ieee80211.h | sizeof | 10
kbuf.c | include/linux/compiler.h | READ_ONCE | 8
kbuf.c | include/linux/slab.h | kfree | 6
kbuf.c | include/linux/lockdep.h | lockdep_assert_held | 6
kbuf.c | io_uring/io_uring.c | io_ring_submit_lock | 5
kbuf.c | io_uring/io_uring.c | io_ring_submit_unlock | 5
kbuf.c | io_uring/kbuf.c | io_kbuf_commit | 4
kbuf.c | include/linux/io_uring_types.h | io_kiocb_to_cmd | 4
kbuf.c | io_uring/kbuf.c | io_put_bl | 4
kbuf.c | io_uring/kbuf.c | io_ring_head_to_buf | 4
kbuf.c | include/linux/list.h | list_empty | 4
kbuf.c | include/linux/uaccess.h | u64_to_user_ptr | 4
kbuf.c | include/linux/compiler.h | while | 4
kbuf.c | io_uring/kbuf.c | __io_remove_buffers | 3
kbuf.c | include/linux/uaccess.h | copy_from_user | 3
kbuf.c | io_uring/kbuf.c | io_buffer_add_list | 3
kbuf.c | io_uring/kbuf.c | io_provided_buffer_select | 3
kbuf.c | io_uring/kbuf.c | io_provided_buffers_select | 3
kbuf.c | io_uring/kbuf.c | io_ring_buffers_peek | 3
kbuf.c | include/linux/minmax.h | min_t | 3
kbuf.c | include/linux/scoped_guard.h | scoped_guard | 3
kbuf.c | include/linux/xarray.h | xa_erase | 3
kbuf.c | include/linux/list.h | INIT_LIST_HEAD | 2
kbuf.c | include/asm-generic/bug.h | WARN_ON_ONCE | 2
kbuf.c | io_uring/kbuf.c | __io_put_kbuf_ring | 2
kbuf.c | include/linux/sched.h | cond_resched | 2
kbuf.c | include/linux/compiler.h | for | 2
kbuf.c | io_uring/kbuf.c | io_add_buffers | 2
kbuf.c | io_uring/kbuf.c | io_destroy_bl | 2
kbuf.c | io_uring/kbuf.c | io_free_region | 2
kbuf.c | io_uring/kbuf.c | io_kbuf_drop_legacy | 2
kbuf.c | io_uring/kbuf.c | io_kbuf_inc_commit | 2
kbuf.c | io_uring/kbuf.c | io_req_set_res | 2
kbuf.c | io_uring/kbuf.c | io_ring_buffer_select | 2
kbuf.c | include/linux/slab.h | kzalloc | 2
kbuf.c | include/linux/list.h | list_del | 2
kbuf.c | include/linux/list.h | list_first_entry | 2
kbuf.c | include/linux/string.h | memset | 2
kbuf.c | io_uring/kbuf.c | req_set_fail | 2
kbuf.c | include/linux/smp.h | smp_load_acquire | 2
kbuf.c | include/linux/xarray.h | xa_load | 2
kbuf.c | include/linux/kernel.h | ARRAY_SIZE | 1
kbuf.c | include/linux/mm.h | PAGE_ALIGN | 1
kbuf.c | io_uring/kbuf.c | __io_put_kbufs | 1
kbuf.c | include/linux/uaccess.h | access_ok | 1
kbuf.c | include/linux/overflow.h | check_add_overflow | 1
kbuf.c | include/linux/overflow.h | check_mul_overflow | 1
kbuf.c | io_uring/kbuf.c | completes | 1
kbuf.c | include/linux/flex_array.h | flex_array_size | 1
kbuf.c | include/linux/guard.h | guard | 1
kbuf.c | io_uring/kbuf.c | io_buffer_select | 1
kbuf.c | io_uring/kbuf.c | io_buffers_peek | 1
kbuf.c | io_uring/kbuf.c | io_buffers_select | 1
kbuf.c | io_uring/kbuf.c | io_create_region_mmap_safe | 1
kbuf.c | io_uring/kbuf.c | io_destroy_buffers | 1
kbuf.c | io_uring/kbuf.c | io_file_can_poll | 1
kbuf.c | io_uring/kbuf.c | io_kbuf_recycle_legacy | 1
kbuf.c | io_uring/kbuf.c | io_pbuf_get_region | 1
kbuf.c | io_uring/kbuf.c | io_provide_buffers | 1
kbuf.c | io_uring/kbuf.c | io_provide_buffers_prep | 1
kbuf.c | io_uring/kbuf.c | io_region_get_ptr | 1
kbuf.c | io_uring/kbuf.c | io_register_pbuf_ring | 1
kbuf.c | io_uring/kbuf.c | io_register_pbuf_status | 1
kbuf.c | io_uring/kbuf.c | io_remove_buffers | 1
kbuf.c | io_uring/kbuf.c | io_remove_buffers_prep | 1
kbuf.c | io_uring/kbuf.c | io_unregister_pbuf_ring | 1
kbuf.c | include/linux/kernel.h | is_power_of_2 | 1
kbuf.c | include/linux/slab.h | kmalloc | 1
kbuf.c | include/linux/slab.h | kmalloc_array | 1
kbuf.c | include/linux/compiler.h | likely | 1
kbuf.c | include/linux/list.h | list_add | 1
kbuf.c | include/linux/list.h | list_add_tail | 1
kbuf.c | include/linux/minmax.h | min_not_zero | 1
kbuf.c | include/linux/xarray.h | xa_err | 1
kbuf.c | include/linux/xarray.h | xa_find | 1
kbuf.c | include/linux/xarray.h | xa_store | 1



### kbuf.h
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
kbuf.h | kbuf.h | __io_put_kbufs | 3
kbuf.h | kbuf.h | io_kbuf_recycle_legacy | 2
kbuf.h | kbuf.h | io_kbuf_recycle_ring | 2
kbuf.h | kbuf.h | io_buffer_select | 1
kbuf.h | kbuf.h | io_buffers_peek | 1
kbuf.h | kbuf.h | io_buffers_select | 1
kbuf.h | kbuf.h | io_destroy_buffers | 1
kbuf.h | kbuf.h | io_do_buffer_select | 1
kbuf.h | kbuf.h | io_kbuf_commit | 1
kbuf.h | kbuf.h | io_kbuf_drop_legacy | 1
kbuf.h | kbuf.h | io_kbuf_recycle | 1
kbuf.h | kbuf.h | io_pbuf_get_region | 1
kbuf.h | kbuf.h | io_provide_buffers | 1
kbuf.h | kbuf.h | io_provide_buffers_prep | 1
kbuf.h | kbuf.h | io_put_kbuf | 1
kbuf.h | kbuf.h | io_put_kbufs | 1
kbuf.h | kbuf.h | io_register_pbuf_ring | 1
kbuf.h | kbuf.h | io_register_pbuf_status | 1
kbuf.h | kbuf.h | io_remove_buffers | 1
kbuf.h | kbuf.h | io_remove_buffers_prep | 1
kbuf.h | kbuf.h | io_unregister_pbuf_ring | 1



### memmap.c
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
memmap.c | include/linux/err.h | ERR_PTR | 11
memmap.c | include/linux/ieee80211.h | sizeof | 6
memmap.c | include/linux/err.h | IS_ERR | 5
memmap.c | include/asm-generic/bug.h | WARN_ON_ONCE | 4
memmap.c | include/linux/cleanup.h | guard | 4
memmap.c | memmap.c | io_uring_validate_mmap_request | 4
memmap.c | include/linux/err.h | PTR_ERR | 3
memmap.c | include/linux/overflow.h | check_add_overflow | 3
memmap.c | memmap.c | io_mmap_get_region | 3
memmap.c | include/linux/mm.h | kvfree | 3
memmap.c | memmap.c | io_create_region | 2
memmap.c | memmap.c | io_free_region | 2
memmap.c | memmap.c | io_mem_alloc_compound | 2
memmap.c | memmap.c | io_pin_pages | 2
memmap.c | memmap.c | io_region_allocate_pages | 2
memmap.c | memmap.c | io_region_init_ptr | 2
memmap.c | memmap.c | io_region_mmap | 2
memmap.c | memmap.c | io_region_pin_pages | 2
memmap.c | memmap.c | io_region_validate_mmap | 2
memmap.c | memmap.c | io_uring_get_unmapped_area | 2
memmap.c | memmap.c | io_uring_mmap | 2
memmap.c | include/linux/mm.h | kvmalloc_array | 2
memmap.c | include/linux/string.h | memcpy | 2
memmap.c | include/linux/mm.h | page_address | 2
memmap.c | mm/swap.c | release_pages | 2
memmap.c | mm/gup.c | unpin_user_pages | 2
memmap.c | memmap.c | __io_account_mem | 1
memmap.c | memmap.c | __io_unaccount_mem | 1
memmap.c | include/linux/gfp.h | alloc_pages | 1
memmap.c | mm/page_alloc.c | alloc_pages_bulk_node | 1
memmap.c | N/A | for | 1
memmap.c | include/linux/mm.h | get_order | 1
memmap.c | mm/util.c | get_unmapped_area | 1
memmap.c | memmap.c | io_check_coalesce_buffer | 1
memmap.c | memmap.c | io_create_region_mmap_safe | 1
memmap.c | memmap.c | io_pbuf_get_region | 1
memmap.c | memmap.c | io_region_get_ptr | 1
memmap.c | memmap.c | io_region_is_set | 1
memmap.c | memmap.c | io_uring_nommu_mmap_capabilities | 1
memmap.c | mm/nommu.c | is_nommu_shared_mapping | 1
memmap.c | include/linux/slab.h | kmalloc | 1
memmap.c | include/linux/lockdep.h | lockdep_assert_held | 1
memmap.c | lib/string.c | memchr_inv | 1
memmap.c | include/linux/string.h | memset | 1
memmap.c | include/linux/minmax.h | min | 1
memmap.c | mm/util.c | mm_get_unmapped_area | 1
memmap.c | mm/gup.c | pin_user_pages_fast | 1
memmap.c | mm/mmap.c | vm_flags_set | 1
memmap.c | mm/memory.c | vm_insert_pages | 1
memmap.c | mm/vmalloc.c | vmap | 1
memmap.c | mm/vmalloc.c | vunmap | 1

### memmap.h
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
memmap.h | memmap.h | io_create_region | 1
memmap.h | memmap.h | io_create_region_mmap_safe | 1
memmap.h | memmap.h | io_free_region | 1
memmap.h | memmap.h | io_pin_pages | 1
memmap.h | memmap.h | io_region_get_ptr | 1
memmap.h | memmap.h | io_region_is_set | 1
memmap.h | memmap.h | io_uring_get_unmapped_area | 1
memmap.h | memmap.h | io_uring_mmap | 1
memmap.h | memmap.h | io_uring_nommu_mmap_capabilities | 1



### msg_ring.c
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
msg_ring.c | include/linux/compiler.h | READ_ONCE | 8
msg_ring.c | include/linux/io_uring_types.h | io_kiocb_to_cmd | 8
msg_ring.c | include/linux/compiler.h | unlikely | 7
msg_ring.c | msg_ring.c | __io_msg_ring_data | 3
msg_ring.c | msg_ring.c | __io_msg_ring_prep | 3
msg_ring.c | msg_ring.c | io_double_unlock_ctx | 3
msg_ring.c | msg_ring.c | io_lock_external_ctx | 3
msg_ring.c | msg_ring.c | io_msg_install_complete | 3
msg_ring.c | msg_ring.c | io_msg_need_remote | 3
msg_ring.c | msg_ring.c | fd_file | 2
msg_ring.c | msg_ring.c | io_is_uring_fops | 2
msg_ring.c | msg_ring.c | io_msg_data_remote | 2
msg_ring.c | msg_ring.c | io_msg_fd_remote | 2
msg_ring.c | msg_ring.c | io_msg_get_kiocb | 2
msg_ring.c | msg_ring.c | io_msg_grab_file | 2
msg_ring.c | msg_ring.c | io_msg_remote_post | 2
msg_ring.c | msg_ring.c | io_msg_ring_data | 2
msg_ring.c | msg_ring.c | io_msg_send_fd | 2
msg_ring.c | msg_ring.c | io_post_aux_cqe | 2
msg_ring.c | msg_ring.c | io_req_set_res | 2
msg_ring.c | mm/slab.c | kmem_cache_free | 2
msg_ring.c | msg_ring.c | req_set_fail | 2
msg_ring.c | include/linux/spinlock.h | spin_trylock | 2
msg_ring.c | include/linux/spinlock.h | spin_unlock | 2
msg_ring.c | include/linux/io_uring_types.h | CLASS | 1
msg_ring.c | include/asm-generic/bug.h | WARN_ON_ONCE | 1
msg_ring.c | msg_ring.c | __io_fixed_fd_install | 1
msg_ring.c | msg_ring.c | cmd_to_io_kiocb | 1
msg_ring.c | include/linux/kernel.h | container_of | 1
msg_ring.c | msg_ring.c | fd_empty | 1
msg_ring.c | fs/file_table.c | fput | 1
msg_ring.c | fs/file_table.c | get_file | 1
msg_ring.c | kernel/task_work.c | init_task_work | 1
msg_ring.c | msg_ring.c | io_add_aux_cqe | 1
msg_ring.c | msg_ring.c | io_alloc_cache_get | 1
msg_ring.c | msg_ring.c | io_alloc_cache_put | 1
msg_ring.c | msg_ring.c | io_msg_ring | 1
msg_ring.c | msg_ring.c | io_msg_ring_cleanup | 1
msg_ring.c | msg_ring.c | io_msg_ring_prep | 1
msg_ring.c | msg_ring.c | io_msg_tw_complete | 1
msg_ring.c | msg_ring.c | io_msg_tw_fd_complete | 1
msg_ring.c | msg_ring.c | io_req_queue_tw_complete | 1
msg_ring.c | msg_ring.c | io_req_task_work_add_remote | 1
msg_ring.c | msg_ring.c | io_ring_submit_lock | 1
msg_ring.c | msg_ring.c | io_ring_submit_unlock | 1
msg_ring.c | msg_ring.c | io_rsrc_node_lookup | 1
msg_ring.c | msg_ring.c | io_slot_file | 1
msg_ring.c | msg_ring.c | io_uring_sync_msg_ring | 1
msg_ring.c | mm/slab.c | kmem_cache_alloc | 1
msg_ring.c | kernel/locking/mutex.c | mutex_lock | 1
msg_ring.c | kernel/locking/mutex.c | mutex_trylock | 1
msg_ring.c | kernel/locking/mutex.c | mutex_unlock | 1
msg_ring.c | lib/percpu-refcount.c | percpu_ref_get | 1
msg_ring.c | lib/percpu-refcount.c | percpu_ref_put | 1
msg_ring.c | msg_ring.c | switch | 1
msg_ring.c | kernel/task_work.c | task_work_add | 1



### msg_ring.h
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
msg_ring.h | msg_ring.h | io_msg_ring | 1
msg_ring.h | msg_ring.h | io_msg_ring_cleanup | 1
msg_ring.h | msg_ring.h | io_msg_ring_prep | 1
msg_ring.h | msg_ring.h | io_uring_sync_msg_ring | 1



### napi.c
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
napi.c | include/linux/compiler.h | READ_ONCE | 7
napi.c | include/asm-generic/rwonce.h | WRITE_ONCE | 7
napi.c | include/linux/slab.h | guard | 4
napi.c | Unknown | io_napi_hash_find | 4
napi.c | Unknown | kfree_rcu | 4
napi.c | include/linux/ieee80211.h | sizeof | 4
napi.c | Unknown | __io_napi_do_busy_loop | 3
napi.c | Unknown | hash_del_rcu | 3
napi.c | Unknown | io_napi_free | 3
napi.c | Unknown | io_napi_remove_stale | 3
napi.c | Unknown | list_del_rcu | 3
napi.c | Unknown | net_to_ktime | 3
napi.c | Unknown | scoped_guard | 3
napi.c | Unknown | HASH_BITS | 2
napi.c | Unknown | __io_napi_add_id | 2
napi.c | Unknown | __io_napi_busy_loop | 2
napi.c | Unknown | __io_napi_del_id | 2
napi.c | Unknown | __io_napi_remove_stale | 2
napi.c | Unknown | busy_loop_current_time | 2
napi.c | include/linux/uaccess.h | copy_to_user | 2
napi.c | Unknown | dynamic_tracking_do_busy_loop | 2
napi.c | Unknown | hash_min | 2
napi.c | Unknown | io_napi_blocking_busy_loop | 2
napi.c | Unknown | io_napi_busy_loop_should_end | 2
napi.c | Unknown | io_napi_busy_loop_timeout | 2
napi.c | Unknown | io_napi_init | 2
napi.c | Unknown | io_napi_register_napi | 2
napi.c | Unknown | io_napi_sqpoll_busy_poll | 2
napi.c | Unknown | kfree | 2
napi.c | Unknown | ktime_to_us | 2
napi.c | Unknown | list_for_each_entry | 2
napi.c | Unknown | list_for_each_entry_rcu | 2
napi.c | Unknown | napi_busy_loop_rcu | 2
napi.c | Unknown | napi_id_valid | 2
napi.c | Unknown | ns_to_ktime | 2
napi.c | Unknown | spin_unlock | 2
napi.c | Unknown | static_tracking_do_busy_loop | 2
napi.c | Unknown | switch | 2
napi.c | Unknown | time_after | 2
napi.c | Unknown | INIT_LIST_HEAD | 1
napi.c | Unknown | INIT_LIST_HEAD_RCU | 1
napi.c | Unknown | copy_from_user | 1
napi.c | Unknown | hlist_add_tail_rcu | 1
napi.c | Unknown | hlist_for_each_entry_rcu | 1
napi.c | Unknown | io_get_time | 1
napi.c | Unknown | io_has_work | 1
napi.c | Unknown | io_napi_register | 1
napi.c | Unknown | io_napi_unregister | 1
napi.c | Unknown | io_register_napi | 1
napi.c | Unknown | io_should_wake | 1
napi.c | Unknown | io_unregister_napi | 1
napi.c | Unknown | kmalloc | 1
napi.c | Unknown | ktime_add | 1
napi.c | Unknown | ktime_after | 1
napi.c | Unknown | ktime_sub | 1
napi.c | Unknown | list_add_tail_rcu | 1
napi.c | Unknown | list_empty_careful | 1
napi.c | Unknown | list_for_each_entry_safe | 1
napi.c | Unknown | list_is_singular | 1
napi.c | Unknown | min_t | 1
napi.c | Unknown | signal_pending | 1
napi.c | Unknown | spin_lock | 1
napi.c | Unknown | spin_lock_init | 1
napi.c | Unknown | unlikely | 1
napi.c | Unknown | while | 1



### napi.h
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
napi.h | napi.h | io_napi | 3
napi.h | napi.h | io_napi_add | 3
napi.h | include/linux/compiler.h | READ_ONCE | 2
napi.h | napi.h | __io_napi_add_id | 2
napi.h | napi.h | __io_napi_busy_loop | 2
napi.h | napi.h | io_napi_busy_loop | 2
napi.h | napi.h | io_napi_free | 2
napi.h | napi.h | io_napi_init | 2
napi.h | napi.h | io_napi_sqpoll_busy_poll | 2
napi.h | napi.h | io_register_napi | 2
napi.h | napi.h | io_unregister_napi | 2
napi.h | include/linux/list.h | list_empty | 1
napi.h | net.h | sock_from_file | 1



### net.c
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
net.c | include/linux/compiler.h | unlikely | 47
net.c | include/linux/compiler.h | READ_ONCE | 37
net.c | include/linux/io_uring_types.h | io_kiocb_to_cmd | 36
net.c | io_uring.h | req_set_fail | 14
net.c | nop.c | io_req_set_res | 13
net.c | net.h | sock_from_file | 11
net.c | include/linux/ieee80211.h | sizeof | 10
net.c | include/linux/kernel.h | u64_to_user_ptr | 8
net.c | net.c | io_net_retry | 7
net.c | net.c | io_req_msg_cleanup | 7
net.c | include/linux/uio.h | iov_iter_count | 7
net.c | net.c | io_msg_alloc_async | 6
net.c | include/linux/uaccess.h | unsafe_get_user | 6
net.c | kbuf.h | io_do_buffer_select | 5
net.c | io_uring.h | io_is_compat | 5
net.c | kbuf.h | io_kbuf_recycle | 5
net.c | include/linux/uio.h | import_ubuf | 4
net.c | net.c | io_netmsg_iovec_free | 4
net.c | include/linux/uaccess.h | copy_from_user | 3
net.c | net.c | io_bundle_nbufs | 3
net.c | net.c | io_msg_copy_hdr | 3
net.c | net.c | io_mshot_prep_retry | 3
net.c | net.c| io_net_import_vec | 3
net.c | notif.h | io_notif_flush | 3
net.c | notif.h | io_notif_to_data | 3
net.c | net.c | io_recv_finish | 3
net.c | net.c | io_req_post_cqe | 3
net.c | net.c | io_send_setup | 3
net.c | net.c | io_send_zc_cleanup | 3
net.c | net.c | io_sendmsg_setup | 3
net.c | rsrc.c | io_vec_free | 3
net.c | /include/linux/socket.h | move_addr_to_kernel | 3
net.c | include/linux/err.h | IS_ERR | 2
net.c | include/linux/err.h | PTR_ERR | 2
net.c | /include/asm-generic/bug.h | WARN_ON_ONCE | 2
net.c | include/linux/file.h | __get_unused_fd_flags | 2
net.c | include/linux/socket.h | __sys_sendmsg_sock | 2
net.c | include/linux/overflow.h | check_add_overflow | 2
net.c | include/linux/compat.h | compat_ptr | 2
net.c | include/linux/file.h | fd_install | 2
net.c | kbuf.h | io_buffer_select | 2
net.c | include/linux/compat.h | io_compat_msg_copy_hdr | 2
net.c |net.c | io_copy_msghdr_from_user | 2
net.c | filetable.c | io_fixed_fd_install | 2
net.c | Unknown | io_netmsg_recycle | 2
net.c | kbuf.h | io_put_kbuf | 2
net.c | kbuf.h | io_put_kbufs | 2
net.c | net.h | io_recv_buf_select | 2
net.c | net.h | io_recvmsg_copy_hdr | 2
net.c | net.h | io_recvmsg_mshot_prep | 2
net.c | net.h | io_recvmsg_multishot | 2
net.c | net.h | io_recvmsg_prep_multishot | 2
net.c | net.h | io_recvmsg_prep_setup | 2
net.c | net.c | io_send_finish | 2
net.c | net.c | io_send_select_buffer | 2
net.c | net.c | io_send_zc_import | 2
net.c | net.c | io_sg_from_iter | 2
net.c | net.c | io_sg_from_iter_iovec | 2
net.c | net.c | iov_iter_init | 2
net.c | include/linux/minmax.h | min_not_zero | 2
net.c | include/linux/file.h | put_unused_fd | 2
net.c | include/linux/sched/signal.h | rlimit | 2
net.c | include/net/sock.h | sock_error | 2
net.c | include/net/sock.h | sock_recvmsg | 2
net.c | include/net/sock.h | sock_sendmsg | 2
net.c | /include/linux/bitops.h | test_bit | 2
net.c | include/linux/uaccess.h | user_access_end | 2
net.c | include/linux/skbuff.h | zerocopy_fill_skb_from_iter | 2
net.c | include/linux/build-bug.h | BUILD_BUG_ON | 1
net.c | include/linux/mm.h | PAGE_ALIGN | 1
net.c | include/linux/socket.h | __copy_msghdr | 1
net.c | __get_compat_msghdr | __get_compat_msghdr | 1
net.c | include/linux/uio.h | __import_iovec | 1
net.c | include/linux/skbuff.h | __skb_fill_page_desc_noacc | 1
net.c | include/linux/socket.h | __sys_bind_socket | 1
net.c | include/linux/socket.h | __sys_connect_file | 1
net.c | include/linux/socket.h | __sys_listen_socket | 1
net.c | include/linux/socket.h | __sys_recvmsg_sock | 1
net.c | include/linux/socket.h | __sys_shutdown_sock | 1
net.c | include/linux/socket.h | __sys_socket_file | 1
net.c | include/linux/bvec.h | bvec_iter_advance_single | 1
net.c | include/linux/uaccess.h | copy_to_user | 1
net.c | include/linux/socket.h | do_accept | 1
net.c | net.h | io_accept | 1
net.c | net.h | io_accept_prep | 1
net.c | net.h | io_alloc_cache_put | 1
net.c | net.h | io_alloc_cache_vec_kasan | 1
net.c | net.h | io_alloc_notif | 1
net.c | net.h | io_bind | 1
net.c | net.h | io_bind_prep | 1
net.c | net.h | io_buffers_peek | 1
net.c | net.h | io_buffers_select | 1
net.c | net.h | io_connect | 1
net.c | net.h | io_connect_prep | 1
net.c | net.h | io_import_reg_buf | 1
net.c | net.h | io_import_reg_vec | 1
net.c | net.h | io_listen | 1
net.c | net.h | io_listen_prep | 1
net.c | net.c | io_netmsg_cache_free | 1
net.c | net.c | io_notif_account_mem | 1
net.c | net.c | io_prep_reg_iovec | 1
net.c | net.c | io_recv | 1
net.c | net.c | io_recvmsg | 1
net.c | net.c | io_recvmsg_prep | 1
net.c | net.c | io_recvzc | 1
net.c | net.c | io_recvzc_prep | 1
net.c | net.h | io_send | 1
net.c | net.h | io_send_zc | 1
net.c | net.h | io_send_zc_prep | 1
net.c | net.h | io_sendmsg | 1
net.c | net.h | io_sendmsg_prep | 1
net.c | net.h | io_sendmsg_recvmsg_cleanup | 1
net.c | net.h | io_sendmsg_zc | 1
net.c | net.h | io_sendrecv_fail | 1
net.c | U=net.h | io_shutdown | 1
net.c | net.h | io_shutdown_prep | 1
net.c | net.h | io_socket | 1
net.c | net.h | io_socket_prep | 1
net.c | io_uring.h | io_uring_alloc_async_data | 1
net.c | rsrc.h | io_vec_reset_iovec | 1
net.c | zcrx.h | io_zcrx_recv | 1
net.c | include/linux/uio.h | iov_iter_ubuf | 1
net.c | include/linux/uio.h | iter_iov | 1
net.c | include/linux/uio.h | iter_is_ubuf | 1
net.c | include/linux/fortify-string.h | kfree | 1
net.c | include/linux/fortify-string.h | memset | 1
net.c | include/linux/minmax.h | min | 1
net.c | include/linux/minmax.h | min_t | 1
net.c | include/linux/bvec.h | mp_bvec_iter_bvec | 1
net.c | include/linux/stddef.h | offsetof | 1
net.c | io_uring.h | req_has_async_data | 1
net.c | /include/linux/slab.h | s | 1
net.c | include/linux/skbuff.h | skb_shinfo | 1
net.c | include/linux/skbuff.h | skb_zcopy_downgrade_managed | 1
net.c | include/linux/skbuff.h | skb_zcopy_managed | 1
net.c | include/linux/syscalls.h | sys_sendmsg | 1
net.c | include/linux/uaccess.h | user_access_begin | 1



### net.h
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
net.h | net.h | io_netmsg_cache_free | 2
net.h | include/linux/stddef.h | struct_group | 2
net.h | net.h | io_accept | 1
net.h | net.h | io_accept_prep | 1
net.h | net.h | io_bind | 1
net.h | net.h | io_bind_prep | 1
net.h | net.h | io_connect | 1
net.h | net.h | io_connect_prep | 1
net.h | net.h | io_listen | 1
net.h | net.h | io_listen_prep | 1
net.h | net.h | io_recv | 1
net.h | net.h | io_recvmsg | 1
net.h | net.h | io_recvmsg_prep | 1
net.h | net.h | io_send | 1
net.h | net.h | io_send_zc | 1
net.h | net.h | io_send_zc_cleanup | 1
net.h | net.h | io_send_zc_prep | 1
net.h | net.h | io_sendmsg | 1
net.h | net.h | io_sendmsg_prep | 1
net.h | net.h | io_sendmsg_recvmsg_cleanup | 1
net.h | net.h | io_sendmsg_zc | 1
net.h | net.h | io_sendrecv_fail | 1
net.h | net.h | io_shutdown | 1
net.h | net.h | io_shutdown_prep | 1
net.h | net.h | io_socket | 1
net.h | net.h | io_socket_prep | 1



### nop.c
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
nop.c | include/linux/compiler.h | READ_ONCE | 4
nop.c | include/linux/io_uring_types.h | io_kiocb_to_cmd | 2
nop.c | io_uring.c| io_file_get_fixed | 1
nop.c | io_uring.c | io_file_get_normal | 1
nop.c | io_uring.c | io_find_buf_node | 1
nop.c | nop.h | io_nop | 1
nop.c | nop.h | io_nop_prep | 1
nop.c | nop.c | io_req_set_res | 1
nop.c | io_uring.h | req_set_fail | 1


### nop.h
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
nop.h | nop.h | io_nop | 1
nop.h | nop.h | io_nop_prep | 1



### notif.c
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
notif.c | include/linux/compiler.h | unlikely | 6
notif.c | include/linux/io_uring_types.h | cmd_to_io_kiocb | 4
notif.c | /include/linux/container_of.h | container_of | 3
notif.c | include/asm-generic/rwonce.h | WRITE_ONCE | 2
notif.c | notif.h | io_notif_to_data | 2
notif.c | notif.h | io_tx_ubuf_complete | 2
notif.c | include/linux/skbuff.h | net_zcopy_get | 2
notif.c | notif.h  | io_alloc_notif | 1
notif.c | notif.c | io_link_skb | 1
notif.c | notif.c | io_notif_tw_complete | 1
notif.c | include/linux/lockdep.h | lockdep_assert | 1
notif.c | /include/linux/refcount.h | refcount_dec_and_test | 1
notif.c | /include/linux/refcount.h | refcount_read | 1
notif.c | /include/linux/refcount.h | refcount_set | 1
notif.c | include/linux/skbuff.h | skb_zcopy | 1
notif.c | include/linux/skbuff.h | skb_zcopy_init | 1



### notif.h
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
notif.h | notif.h | io_notif_to_data | 3
notif.h | notif.h | io_tx_ubuf_complete | 2
notif.h | notif.h | io_alloc_notif | 1
notif.h | notif.h | io_notif_account_mem | 1
notif.h | notif.h | io_notif_flush | 1