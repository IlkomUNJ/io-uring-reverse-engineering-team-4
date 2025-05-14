| Structure name | Defined in          | Attributes                                                                                                              | Caller Functions Source                            | Source Caller       | Usage                              |
| -------------- | ------------------- | ----------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------- | ------------------- | ---------------------------------- |
| io\_sq\_data | io\_uring/sqpoll.h | refcount\_t refs, atomic\_t park\_pending, struct mutex lock, struct list\_head ctx\_list | io\_sq\_offload\_create | io\_uring/sqpoll.c | local variable |
|  |  | struct task\_struct \*thread, struct wait\_queue\_head wait | io\_sq\_offload\_create | io\_uring/sqpoll.c | assigned field |
|  |  | unsigned sq\_thread\_idle, int sq\_cpu, pid\_t task\_pid, pid\_t task\_tgid | io\_sq\_offload\_create | io\_uring/sqpoll.c | assigned field |
|  |  | u64 work\_time, unsigned long state, struct completion exited | io\_sq\_offload\_create | io\_uring/sqpoll.c | assigned field |
|  |  |  | io\_sq\_thread\_park | io\_uring/sqpoll.c | function parameter |
|  |  |  | io\_sq\_thread\_unpark | io\_uring/sqpoll.c | function parameter |
|  |  |  | io\_sq\_thread\_stop | io\_uring/sqpoll.c | function parameter |
|  |  |  | io\_put\_sq\_data | io\_uring/sqpoll.c | function parameter, local variable |
|  |  |  | io\_sq\_thread\_finish | io\_uring/sqpoll.c | local variable, ctx->sq\_data |
|  |  |  | io\_sqd\_update\_thread\_idle | io\_uring/sqpoll.c | function parameter, struct field |
|  |  |  | io\_sqd\_events\_pending | io\_uring/sqpoll.c | function parameter |
|  |  |  | io\_sqd\_handle\_event | io\_uring/sqpoll.c | function parameter |
|  |  |  | io\_sq\_thread | io\_uring/sqpoll.c | function parameter |
|  |  |  | io\_sq\_update\_worktime | io\_uring/sqpoll.c | function parameter |
|  |  |  | io\_get\_sq\_data | io\_uring/sqpoll.c | return value |
|  |  |  | io\_attach\_sq\_data | io\_uring/sqpoll.c | local variable, return value |
|  |  |  | io\_uring\_cancel\_generic | io\_uring/sqpoll.c | function parameter |
|  |  |  | create\_io\_thread | io\_uring/sqpoll.c | function parameter |
|  |  |  | io\_uring\_alloc\_task\_context | io\_uring/sqpoll.c | function parameter |
|  |  |  | set\_task\_comm | io\_uring/sqpoll.c | assigned field |
|  |  |  | set\_cpus\_allowed\_ptr | io\_uring/sqpoll.c | reads field |
|  |  |  | cpuset\_cpus\_allowed | io\_uring/sqpoll.c | reads field |
|  |  |  | list\_for\_each\_entry(ctx, \&sqd->ctx\_list, ...) | io\_uring/sqpoll.c | iterated field |
| io\_ev\_fd | io\_uring/eventfd.c | eventfd\_ctx, uint, uint, refcount\_t, atomic\_t, rcu\_head | io\_eventfd\_free | io\_uring/eventfd.c | local variable |
|  |  |  | io\_eventfd\_put | io\_uring/eventfd.c | function parameter |
|  |  |  | io\_eventfd\_do\_signal | io\_uring/eventfd.c | local variable, function parameter |
|  |  |  | \_\_io\_eventfd\_signal | io\_uring/eventfd.c | function parameter |
|  |  |  | io\_eventfd\_grab | io\_uring/eventfd.c | return value, local variable |
|  |  |  | io\_eventfd\_signal | io\_uring/eventfd.c | local variable |
|  |  |  | io\_eventfd\_flush\_signal | io\_uring/eventfd.c | local variable |
|  |  |  | io\_eventfd\_register | io\_uring/eventfd.c | local variable |
|  |  |  | io\_eventfd\_unregister | io\_uring/eventfd.c | function parameter |
| io\_statx | io\_uring/statx.c | file \*file, int dfd, unsigned int mask, unsigned int flags, struct filename \*filename, struct statx \_\_user \*buffer | io\_statx\_prep | io\_uring/statx.c | local variable |
|  |  |  | io\_statx | io\_uring/statx.c | local variable |
|  |  |  | io\_statx\_cleanup | io\_uring/statx.c | local variable |
| io\_sync | io\_uring/sync.c | struct file \*file, loff\_t len, loff\_t off, int flags, int mode | io\_sfr\_prep | io\_uring/sync.c | local variable |
|  |  |  | io\_sync\_file\_range | io\_uring/sync.c | local variable |
|  |  |  | io\_fsync\_prep | io\_uring/sync.c | local variable |
|  |  |  | io\_fsync | io\_uring/sync.c | local variable |
|  |  |  | io\_fallocate\_prep | io\_uring/sync.c | local variable |
|  |  |  | io\_fallocate | io\_uring/sync.c | local variable |
| io_tctx_node | io_uring/tctx.h | list_head, task_struct \*, io_ring_ctx \* | __io_uring_add_tctx_node | io_uring/tctx.c | local variable, dynamically allocated |
|  |  |  | __io_uring_add_tctx_node_from_submit | io_uring/tctx.c | indirect, through call to __io_uring_add_tctx_node |
|  |  |  | io_uring_del_tctx_node | io_uring/tctx.c | xarray element, function parameter |
|  |  |  | io_uring_clean_tctx | io_uring/tctx.c | xarray element in loop |
|  |  |  | __io_uring_free | io_uring/tctx.c | xarray element in loop (warn check) |
| io_uring_task | io_uring/io_uring.h | io_wq \*, task_struct \*, xarray, wait_queue_head_t, atomic_t, atomic_t, llist_head, task_work_struct, file \*[IO_RINGFD_REG_MAX], percpu_counter, io_ring_ctx \* | io_uring_alloc_task_context | io_uring/tctx.c | dynamically allocated, initialized, assigned to task_struct |
|  |  |  | __io_uring_free | io_uring/tctx.c | function parameter, memory freed |
|  |  |  | __io_uring_add_tctx_node | io_uring/tctx.c | accessed via current->io_uring |
|  |  |  | io_uring_del_tctx_node | io_uring/tctx.c | accessed via current->io_uring |
|  |  |  | io_uring_clean_tctx | io_uring/tctx.c | function parameter |
|  |  |  | io_uring_unreg_ringfd | io_uring/tctx.c | accessed via current->io_uring |
|  |  |  | io_ringfd_register | io_uring/tctx.c | accessed via current->io_uring |
|  |  |  | io_ringfd_unregister | io_uring/tctx.c | accessed via current->io_uring |
|  |  |  | io_ring_add_registered_file | io_uring/tctx.c | function parameter |
|  |  |  | io_ring_add_registered_fd | io_uring/tctx.c | function parameter |
| io\_timeout\_data | io\_uring/timeout.h | io\_kiocb \*, hrtimer, timespec64, enum hrtimer\_mode, u32 | io\_timeout\_fn | io\_uring/timeout.c | container of, async\_data |
|  |  |  | io\_link\_timeout\_fn | io\_uring/timeout.c | container of, async\_data |
|  |  |  | io\_timeout\_finish | io\_uring/timeout.c | function parameter |
|  |  |  | io\_timeout\_complete | io\_uring/timeout.c | function parameter |
|  |  |  | \_\_io\_disarm\_linked\_timeout | io\_uring/timeout.c | async\_data |
|  |  |  | io\_kill\_timeout | io\_uring/timeout.c | async\_data |
|  |  |  | io\_timeout\_update | io\_uring/timeout.c | local variable |
|  |  |  | io\_linked\_timeout\_update | io\_uring/timeout.c | local variable |
| io\_timeout | io\_uring/timeout.c | file \*, u32 off, target\_seq, repeats; list\_head list; io\_kiocb \* head, prev | io\_timeout\_fn | io\_uring/timeout.c | local variable |
|  |  |  | io\_link\_timeout\_fn | io\_uring/timeout.c | local variable |
|  |  |  | io\_timeout\_complete | io\_uring/timeout.c | local variable |
|  |  |  | io\_timeout\_finish | io\_uring/timeout.c | function parameter |
|  |  |  | io\_kill\_timeout | io\_uring/timeout.c | local variable |
|  |  |  | io\_flush\_timeouts | io\_uring/timeout.c | local variable |
|  |  |  | io\_timeout\_extract | io\_uring/timeout.c | local variable |
|  |  |  | io\_timeout\_update | io\_uring/timeout.c | local variable |
|  |  |  | io\_linked\_timeout\_update | io\_uring/timeout.c | local variable |
|  |  |  | \_\_io\_disarm\_linked\_timeout | io\_uring/timeout.c | local variable |
|  |  |  | io\_remove\_next\_linked | io\_uring/timeout.c | derived from io\_kiocb |
| io\_timeout\_rem | io\_uring/timeout.c | file \*, u64 addr; timespec64 ts; u32 flags; bool ltimeout | io\_timeout\_remove\_prep | io\_uring/timeout.c | cmd structure in req |
|  |  |  | io\_timeout\_remove | io\_uring/timeout.c | local variable from req |
| io\_ftrunc | io\_uring/truncate.c | struct file \*, loff\_t | io\_ftruncate\_prep | io\_uring/truncate.c | local variable, derived from req |
|  |  |  |  |  | used via io_kiocb_to_cmd() |
|  |  |  | io\_ftruncate | io\_uring/truncate.c | local variable, derived from req |
|  |  |  |  |  | used via io_kiocb_to_cmd() |
| io\_async\_cmd | io\_uring/uring\_cmd.h | io\_uring\_cmd\_data data, iou\_vec vec, io\_uring\_sqe sqes\[2] | io\_cmd\_cache\_free | io\_uring/uring\_cmd.c | function parameter |
|  |  | io\_req\_uring\_cleanup | io\_uring/uring\_cmd.c | local variable |  |
|  |  |  | io\_uring\_cmd\_prep\_setup | io\_uring/uring\_cmd.c | return value |
|  |  |  | io\_uring\_cmd\_import\_fixed\_vec | io\_uring/uring\_cmd.c | local variable |
| io\_uring\_cmd\_data | io\_uring/uring\_cmd.h | (unspecified, assumed to be internal op data struct) | io\_req\_uring\_cleanup | io\_uring/uring\_cmd.c | field of io\_async\_cmd |
|  |  |  | io\_uring\_cmd\_prep\_setup | io\_uring/uring\_cmd.c | field of io\_async\_cmd |
|  |  |  | io\_cmd\_cache\_free | io\_uring/uring\_cmd.c | field of io\_async\_cmd |
|  |  |  | io\_uring\_cmd\_import\_fixed\_vec | io\_uring/uring\_cmd.c | field of io\_async\_cmd |
| io\_uring\_cmd | indirectly from \<linux/io\_uring/cmd.h> | flags, cmd\_op, task\_work\_cb, sqe | io\_uring\_cmd | io\_uring/uring\_cmd.c | local variable |
|  |  |  | io\_uring\_cmd\_prep | io\_uring/uring\_cmd.c | local variable, function parameter |
|  |  |  | io\_uring\_cmd\_mark\_cancelable | io\_uring/uring\_cmd.c | function parameter |
|  |  |  | io\_uring\_cmd\_done | io\_uring/uring\_cmd.c | function parameter |
|  |  |  | \_\_io\_uring\_cmd\_do\_in\_task | io\_uring/uring\_cmd.c | function parameter |
|  |  |  | io\_uring\_cmd\_sock | io\_uring/uring\_cmd.c | function parameter |
|  |  |  | io\_uring\_cmd\_import\_fixed | io\_uring/uring\_cmd.c | function parameter |
|  |  |  | io\_uring\_cmd\_import\_fixed\_vec | io\_uring/uring\_cmd.c | function parameter |
|  |  |  | io\_uring\_cmd\_issue\_blocking | io\_uring/uring\_cmd.c | function parameter |
| io\_waitid\_async | waitid.h | struct io\_kiocb \*req, struct wait\_opts wo | io\_waitid\_prep | io\_uring/waitid.c | function parameter , local variable |
| io\_waitid | waitid.c | struct file \*file, int which, pid\_t upid, int options, atomic\_t refs, struct wait\_queue\_head \*head, | io\_waitid\_free | io\_uring/waitid.c | local variable |
|  |  | struct siginfo \_\_user \*infop, struct waitid\_info info | io\_waitid\_compat\_copy\_si | io\_uring/waitid.c | function parameter , local variable |
|  |  |  | io\_waitid\_copy\_si | io\_uring/waitid.c | function parameter , local variable |
|  |  |  | io\_waitid\_finish | io\_uring/waitid.c | local variable , function parameter |
|  |  |  | io\_waitid\_complete | io\_uring/waitid.c | local variable |
|  |  |  | \_\_io\_waitid\_cancel | io\_uring/waitid.c | function parameter , local variable |
|  |  |  | io\_waitid\_cancel | io\_uring/waitid.c | function parameter , local variable |
|  |  |  | io\_waitid\_remove\_all | io\_uring/waitid.c | function parameter , local variable |
|  |  |  | io\_waitid\_drop\_issue\_ref | io\_uring/waitid.c | function parameter , local variable |
|  |  |  | io\_waitid\_cb | io\_uring/waitid.c | function parameter , local variable |
|  |  |  | io\_waitid\_wait | io\_uring/waitid.c | function parameter , local variable |
| io\_xattr | xattr.c | struct file \*file, struct kernel\_xattr\_ctx ctx, struct filename \*filename | io\_xattr\_cleanup | io\_uring/xattr.c | local variable |
|  |  |  | io\_fsetxattr\_prep | io\_uring/xattr.c | function parameter |
|  |  |  | io\_fsetxattr | io\_uring/xattr.c | function parameter |
|  |  |  | io\_setxattr\_prep | io\_uring/xattr.c | function parameter |
|  |  |  | io\_setxattr | io\_uring/xattr.c | function parameter |
|  |  |  | io\_fgetxattr\_prep | io\_uring/xattr.c | function parameter |
|  |  |  | io\_fgetxattr | io\_uring/xattr.c | function parameter |
|  |  |  | io\_getxattr\_prep | io\_uring/xattr.c | function parameter |
|  |  |  | io\_getxattr | io\_uring/xattr.c | function parameter |
|  |  |  | io\_xattr\_cleanup | io\_uring/xattr.c | local variable |
|  |  |  | io\_xattr\_finish | io\_uring/xattr.c | local variable |
|  |  |  | \_\_io\_getxattr\_prep | io\_uring/xattr.c | local variable |
|  |  |  | \_\_io\_setxattr\_prep | io\_uring/xattr.c | local variable |
| io\_zcrx\_area | zcrx.h | struct net\_iov\_area nia, struct io\_zcrx\_ifq \*ifq, atomic\_t \*user\_refs, bool is\_mapped, u16 area\_id, struct page \*\*pages, spinlock\_t freelist\_lock, u32 free\_count, u32 \*freelist | io\_register\_zcrx\_ifq | io\_uring/zcrx.h | local variable |
|  |  |  | io\_unregister\_zcrx\_ifqs | io\_uring/zcrx.h | function parameter |
|  |  |  | io\_shutdown\_zcrx\_ifqs | io\_uring/zcrx.h | function parameter |
|  |  |  | io\_zcrx\_recv | io\_uring/zcrx.h | function parameter |
| io\_zcrx\_ifq | zcrx.h | struct io\_ring\_ctx \*ctx, struct io\_zcrx\_area \*area, struct io\_uring \*rq\_ring, struct io\_uring\_zcrx\_rqe \*rqes, u32 rq\_entries, u32 cached\_rq\_head, spinlock\_t rq\_lock, u32 if\_rxq, struct device \*dev, struct net\_device \*netdev, netdevice\_tracker netdev\_tracker, spinlock\_t lock | io\_register\_zcrx\_ifq | io\_uring/zcrx.h | local variable |
|  |  |  | io\_unregister\_zcrx\_ifqs | io\_uring/zcrx.h | function parameter |
|  |  |  | io\_shutdown\_zcrx\_ifqs | io\_uring/zcrx.h | function parameter |
|  |  |  | io\_zcrx\_recv | io\_uring/zcrx.h | function parameter |
| io\_zcrx\_args | io\_uring/zcrx.c | io\_kiocb, io\_zcrx\_ifq, socket, unsigned (nr\_skbs) | io\_register\_zcrx\_ifq | io\_uring/zcrx.c | function parameter |
| io\_zcrx\_area | io\_uring/zcrx.c | net\_iov\_area, freelist, user\_refs, pages, nia (net\_iovs), free\_count, area\_id, ifq | io\_zcrx\_create\_area | io\_uring/zcrx.c | function parameter |
| io\_zcrx\_ifq | io\_uring/zcrx.c | net\_device, dev, if\_rxq, lock, rq\_lock, rq\_ring, rqes, area, ctx, netdev\_tracker | io\_zcrx\_ifq\_alloc | io\_uring/zcrx.c | local variable |
| io\_uring\_zcrx\_ifq\_reg | io\_uring/zcrx.c | region\_ptr, if\_idx, rq\_entries, rq\_area\_token, flags, offsets | io\_register\_zcrx\_ifq | io\_uring/zcrx.c | function parameter |
| io\_uring\_zcrx\_area\_reg | io\_uring/zcrx.c | addr, len, flags, rq\_area\_token, \_\_resv1, \_\_resv2 | io\_zcrx\_create\_area | io\_uring/zcrx.c | function parameter |
| io\_uring\_zcrx\_rqe | io\_uring/zcrx.c | off, \_\_pad, rq\_id | io\_zcrx\_ring\_refill | io\_uring/zcrx.c | function parameter |
| net\_iov | io\_uring/zcrx.c | owner, pp, dma\_addr, iov\_idx, netmem\_ref, niov\_idx | io\_zcrx\_map\_area | io\_uring/zcrx.c | local variable |
| page\_pool | io\_uring/zcrx.c | alloc, cache, count, dev, p, sync, offset, dma\_dir | io\_zcrx\_sync\_for\_device | io\_uring/zcrx.c | function parameter |
| net\_device | io\_uring/zcrx.c | netdev, if\_rxq | io\_zcrx\_drop\_netdev | io\_uring/zcrx.c | local variable |
| io\_ring\_ctx | io\_uring/zcrx.c | ifq, uring\_lock, flags, zcrx\_region | io\_register\_zcrx\_ifq | io\_uring/zcrx.c | function parameter |
| pp\_memory\_provider\_params | io\_uring/zcrx.c | mp\_ops, mp\_priv | io\_zcrx\_ifq\_alloc | io\_uring/zcrx.c | function parameter |
| netmem\_ref | io\_uring/zcrx.c | net\_iov to netmem reference, used for memory operations like release and allocation | io\_pp\_zc\_alloc\_netmems | io\_uring/zcrx.c | function parameter |