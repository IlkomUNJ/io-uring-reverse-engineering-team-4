
### `sqpoll.c`

| Source   | Library                      | Function Utilized           | Times Used |
| -------- | ---------------------------- | --------------------------- | ---------- |
| sqpoll.c | sqpoll.c                     | io\_sq\_thread              | 1          |
|          | sqpoll.c                     | io\_sq\_thread\_acquire     | 1          |
|          | sqpoll.c                     | io\_sq\_thread\_park        | 1          |
|          | sqpoll.c                     | io\_sq\_thread\_unpark      | 1          |
|          | sqpoll.c                     | io\_sq\_thread\_finish      | 1          |
|          | sqpoll.c                     | io\_sq\_thread\_start       | 1          |
|          | sqpoll.c                     | io\_wq\_submit\_work        | 1          |
|          | sqpoll.c                     | io\_submit\_sqes            | 1          |
|          | sqpoll.c                     | io\_run\_task\_work         | 1          |
|          | io\_uring/sqpoll.h           | io\_sq\_thread              | 1          |
|          | io\_uring/sqpoll.h           | io\_sq\_thread\_acquire     | 1          |
|          | io\_uring/sqpoll.h           | io\_sq\_thread\_park        | 1          |
|          | io\_uring/sqpoll.h           | io\_sq\_thread\_unpark      | 1          |
|          | io\_uring/sqpoll.h           | io\_sq\_thread\_finish      | 1          |
|          | io\_uring/sqpoll.h           | io\_sq\_thread\_start       | 1          |
|          | include/linux/sched/task.h   | task\_is\_running           | 1          |
|          | kernel/sched/core.c          | wake\_up\_process           | 1          |
|          | include/linux/sched/signal.h | signal\_pending             | 1          |
|          | kernel/sched/core.c          | set\_task\_comm             | 1          |
|          | include/linux/sched.h        | current                     | 2          |
|          | include/linux/kernel.h       | pr\_debug                   | 1          |
|          | include/linux/err.h          | IS\_ERR                     | 1          |
|          | include/linux/err.h          | PTR\_ERR                    | 1          |
|          | include/linux/kthread.h      | kthread\_create             | 1          |
|          | include/linux/kthread.h      | wake\_up\_process           | 1          |
|          | include/linux/kthread.h      | kthread\_park               | 1          |
|          | include/linux/kthread.h      | kthread\_unpark             | 1          |
|          | include/linux/kthread.h      | kthread\_stop               | 1          |
|          | include/linux/wait.h         | wait\_event\_interruptible  | 1          |
|          | include/linux/sched.h        | schedule\_timeout           | 1          |
|          | include/linux/sched.h        | set\_current\_state         | 1          |
|          | include/linux/sched.h        | \_\_set\_current\_state     | 1          |
|          | include/linux/completion.h   | complete                    | 1          |
|          | include/linux/completion.h   | wait\_for\_completion       | 1          |
|          | include/linux/delay.h        | msleep                      | 1          |
|          | include/linux/module.h       | try\_module\_get            | 1          |
|          | include/linux/module.h       | module\_put                 | 1          |
|          | include/linux/slab.h         | kzalloc                     | 1          |
|          | mm/slub.c                    | kfree                       | 1          |
|          | include/linux/rcupdate.h     | rcu\_read\_lock             | 1          |
|          | include/linux/rcupdate.h     | rcu\_read\_unlock           | 1          |
|          | include/linux/rculist.h      | list\_for\_each\_entry\_rcu | 1          |
|          | include/linux/list.h         | list\_add\_tail             | 1          |
|          | include/linux/list.h         | list\_del\_init             | 1          |
|          | include/linux/spinlock.h     | spin\_lock\_irq             | 1          |
|          | include/linux/spinlock.h     | spin\_unlock\_irq           | 1          |
|          | include/linux/spinlock.h     | spin\_lock\_init            | 1          |
|          | include/linux/spinlock.h     | spin\_lock                  | 1          |
|          | include/linux/spinlock.h     | spin\_unlock                | 1          |
|          | include/linux/cpumask.h      | cpumask\_any                | 1          |
|          | include/linux/percpu.h       | this\_cpu\_ptr              | 1          |
|          | include/linux/uaccess.h      | copy\_from\_user            | 1          |
|          | include/linux/uaccess.h      | copy\_to\_user              | 1          |

### `sqpoll.h`

| Source   | Library               | Function Utilized       | Times Used |
| -------- | --------------------- | ----------------------- | ---------- |
| sqpoll.h | sqpoll.h              | io\_sq\_thread          | 1          |
|          | sqpoll.h              | io\_sq\_thread\_acquire | 1          |
|          | sqpoll.h              | io\_sq\_thread\_park    | 1          |
|          | sqpoll.h              | io\_sq\_thread\_unpark  | 1          |
|          | sqpoll.h              | io\_sq\_thread\_finish  | 1          |
|          | sqpoll.h              | io\_sq\_thread\_start   | 1          |
|          | include/linux/types.h | struct task\_struct     | 1          |
|          | include/linux/types.h | struct io\_ring\_ctx    | 1          |


### `statx.c`

| Source  | Library                   | Function Utilized  | Times Used |
| ------- | ------------------------- | ------------------ | ---------- |
| statx.c | statx.c                   | io\_statx\_prep    | 1          |
|         | statx.c                   | io\_statx          | 1          |
|         | statx.c                   | io\_statx\_cleanup | 1          |
|         | statx.c                   | struct io\_statx   | 1          |
|         | fs/internal.h             | do\_statx          | 1          |
|         | include/linux/io\_uring.h | io\_kiocb\_to\_cmd | 3          |
|         | include/linux/io\_uring.h | io\_req\_set\_res  | 1          |
|         | include/linux/kernel.h    | READ\_ONCE         | 4          |
|         | include/linux/errno.h     | -EINVAL            | 1          |
|         | include/linux/errno.h     | -EBADF             | 1          |
|         | include/linux/uaccess.h   | u64\_to\_user\_ptr | 2          |
|         | fs/namei.c                | getname\_uflags    | 1          |
|         | fs/namei.c                | putname            | 1          |
|         | include/linux/err.h       | IS\_ERR            | 1          |
|         | include/linux/err.h       | PTR\_ERR           | 1          |
|         | include/linux/kernel.h    | WARN\_ON\_ONCE     | 1          |

### `statx.h`

| Source  | Library | Function Utilized  | Time Used |
| ------- | ------- | ------------------ | --------- |
| statx.h | statx.h | io\_statx\_prep    | 1         |
|         | statx.h | io\_statx          | 1         |
|         | statx.h | io\_statx\_cleanup | 1         |

### `sync.c`

| Source | Library                         | Function Utilized     | Times Used |
| ------ | ------------------------------- | --------------------- | ---------- |
| sync.c | sync.c                          | io\_sfr\_prep         | 1          |
|        | sync.c                          | io\_sync\_file\_range | 1          |
|        | sync.c                          | io\_fsync\_prep       | 1          |
|        | sync.c                          | io\_fsync             | 1          |
|        | sync.c                          | io\_fallocate\_prep   | 1          |
|        | sync.c                          | io\_fallocate         | 1          |
|        | io\_uring.h                     | io\_kiocb\_to\_cmd    | 3          |
|        | io\_uring.h                     | io\_req\_set\_res     | 3          |
|        | linux/fs.h                      | sync\_file\_range     | 1          |
|        | linux/fs.h                      | vfs\_fsync\_range     | 1          |
|        | linux/fs.h                      | vfs\_fallocate        | 1          |
|        | linux/fsnotify.h                | fsnotify\_modify      | 1          |
|        | asm-generic/bitops/non-atomic.h | READ\_ONCE            | 6          |
|        | linux/bug.h                     | WARN\_ON\_ONCE        | 3          |

---

### `sync.h`

| Source | Library | Function Utilized     | Times Used |
| ------ | ------- | --------------------- | ---------- |
| sync.h | sync.c  | io\_sfr\_prep         | 1          |
|        | sync.c  | io\_sync\_file\_range | 1          |
|        | sync.c  | io\_fsync\_prep       | 1          |
|        | sync.c  | io\_fsync             | 1          |
|        | sync.c  | io\_fallocate         | 1          |
|        | sync.c  | io\_fallocate\_prep   | 1          |

---

### `tctx.c`

| Source | Library         | Function Utilized                            | Times Used |
| ------ | --------------- | -------------------------------------------- | ---------- |
| tctx.c | tctx.c          | io\_uring\_alloc\_task\_context              | 2          |
|        | tctx.c          | \_\_io\_uring\_add\_tctx\_node               | 2          |
|        | tctx.c          | \_\_io\_uring\_add\_tctx\_node\_from\_submit | 1          |
|        | tctx.c          | io\_uring\_del\_tctx\_node                   | 3          |
|        | tctx.c          | io\_uring\_clean\_tctx                       | 1          |
|        | tctx.c          | io\_uring\_unreg\_ringfd                     | 1          |
|        | tctx.c          | io\_ringfd\_register                         | 1          |
|        | tctx.c          | io\_ringfd\_unregister                       | 1          |
|        | io\_uring.h     | io\_is\_uring\_fops                          | 1          |
|        | io\_uring.h     | io\_wq\_create                               | 1          |
|        | io\_uring.h     | io\_wq\_free\_work                           | 1          |
|        | io\_uring.h     | io\_wq\_submit\_work                         | 1          |
|        | io\_uring.h     | io\_wq\_max\_workers                         | 1          |
|        | io\_uring.h     | io\_wq\_put\_and\_exit                       | 1          |
|        | linux/slab.h    | kzalloc                                      | 4          |
|        | linux/slab.h    | kfree                                        | 5          |
|        | linux/mm.h      | percpu\_counter\_init                        | 2          |
|        | linux/mm.h      | percpu\_counter\_destroy                     | 2          |
|        | linux/bug.h     | WARN\_ON\_ONCE                               | 5          |
|        | linux/xarray.h  | xa\_init                                     | 1          |
|        | linux/xarray.h  | xa\_store                                    | 1          |
|        | linux/xarray.h  | xa\_load                                     | 1          |
|        | linux/xarray.h  | xa\_erase                                    | 1          |
|        | linux/xarray.h  | xa\_for\_each                                | 2          |
|        | linux/xarray.h  | xa\_empty                                    | 1          |
|        | linux/xarray.h  | xa\_err                                      | 1          |
|        | linux/wait.h    | init\_waitqueue\_head                        | 2          |
|        | linux/sched.h   | cond\_resched                                | 1          |
|        | linux/uaccess.h | copy\_from\_user                             | 2          |
|        | linux/uaccess.h | copy\_to\_user                               | 1          |
|        | linux/fdtable.h | fget                                         | 1          |
|        | linux/fdtable.h | fput                                         | 4          |
|        | asm/barrier.h   | array\_index\_nospec                         | 2          |
|        | linux/mutex.h   | mutex\_lock                                  | 3          |
|        | linux/mutex.h   | mutex\_unlock                                | 2          |



### `tctx.h`

| Source | Library | Function Utilized                            | Times Used |
| ------ | ------- | -------------------------------------------- | ---------- |
| tctx.h | tctx.c  | io\_uring\_alloc\_task\_context              | 1          |
|        | tctx.c  | \_\_io\_uring\_add\_tctx\_node               | 1          |
|        | tctx.c  | \_\_io\_uring\_add\_tctx\_node\_from\_submit | 1          |
|        | tctx.c  | io\_uring\_clean\_tctx                       | 1          |
|        | tctx.c  | io\_uring\_unreg\_ringfd                     | 1          |
|        | tctx.c  | io\_ringfd\_register                         | 1          |
|        | tctx.c  | io\_ringfd\_unregister                       | 1          |
|        | tctx.h  | io\_uring\_add\_tctx\_node (inline)          | 1          |


### `timeout.c`

| Source    | Library                 | Function Utilized | Time Used |
| --------- | ----------------------- | ----------------- | --------- |
| timeout.c | /include/linux/ktime.h  | ktime\_get        | 2         |
|           | /include/linux/slab.h   | kmalloc           | 1         |
|           | /include/linux/slab.h   | kfree             | 1         |
|           | /include/linux/kernel.h | BUG\_ON           | 1         |
|           | /include/linux/timer.h  | mod\_timer        | 3         |
|           | /include/linux/timer.h  | add\_timer        | 2         |

### `timeout.h`

| Source    | Library                  | Function Utilized | Time Used |
| --------- | ------------------------ | ----------------- | --------- |
| timeout.h | /include/linux/time.h    | time\_after       | 1         |
|           | /include/linux/time.h    | time\_before      | 1         |
|           | /include/linux/jiffies.h | jiffies           | 1         |
|           | /include/linux/kernel.h  | pr\_warn          | 1         |

### `truncate.h`

| Source     | Library                    | Function Utilized  | Time Used |
| ---------- | -------------------------- | ------------------ | --------- |
| truncate.h | /include/linux/io\_uring.h | io\_kiocb\_to\_cmd | 2         |
|            | /include/linux/io\_uring.h | io\_req\_set\_res  | 1         |
|            | /include/linux/io\_uring.h | IOU\_OK            | 1         |

### `truncate.c`

| Source     | Library                    | Function Utilized  | Time Used |
| ---------- | -------------------------- | ------------------ | --------- |
| truncate.c | /include/linux/kernel.h    | WARN\_ON\_ONCE     | 1         |
|            | /include/linux/errno.h     | -EINVAL            | 1         |
|            | /include/linux/fs.h        | do\_ftruncate      | 1         |
|            | /include/linux/io\_uring.h | io\_kiocb\_to\_cmd | 2         |
|            | /include/linux/io\_uring.h | io\_req\_set\_res  | 1         |
|            | /include/linux/io\_uring.h | IOU\_OK            | 1         |


### `uring_cmd.h`

| Source       | Library                        | Function Utilized                  | Time Used |
| ------------ | ------------------------------ | ---------------------------------- | --------- |
| uring\_cmd.h | /include/linux/io\_uring/cmd.h | io\_uring\_cmd                     | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_prep               | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_cleanup            | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_try\_cancel\_uring\_cmd | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_cmd\_cache\_free               | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_import\_fixed\_vec | 1         |

### `uring_cmd.c`

| Source       | Library                        | Function Utilized                  | Time Used |
| ------------ | ------------------------------ | ---------------------------------- | --------- |
| uring\_cmd.c | /include/linux/io\_uring/cmd.h | io\_cmd\_cache\_free               | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_cleanup            | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_try\_cancel\_uring\_cmd | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_mark\_cancelable   | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_done               | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_prep               | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd                     | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_import\_fixed      | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_import\_fixed\_vec | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_issue\_blocking    | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_getsockopt         | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_setsockopt         | 1         |
|              | /include/linux/io\_uring/cmd.h | io\_uring\_cmd\_sock               | 1         |

### `xattr.h`

| Source  | Library                    | Function Utilized  | Time Used |
| ------- | -------------------------- | ------------------ | --------- |
| xattr.h | /include/linux/io\_uring.h | io\_kiocb\_to\_cmd | 1         |
|         | /include/linux/io\_uring.h | io\_req\_set\_res  | 1         |

### `xattr.c`

| Source  | Library                    | Function Utilized   | Time Used |
| ------- | -------------------------- | ------------------- | --------- |
| xattr.c | /include/linux/kernel.h    | kmalloc             | 1         |
|         | /include/linux/kernel.h    | kfree               | 2         |
|         | /include/linux/slab.h      | kvfree              | 1         |
|         | /include/linux/io\_uring.h | io\_kiocb\_to\_cmd  | 2         |
|         | /include/linux/io\_uring.h | io\_req\_set\_res   | 2         |
|         | /include/linux/io\_uring.h | filename\_getxattr  | 1         |
|         | /include/linux/io\_uring.h | filename\_setxattr  | 1         |
|         | /include/linux/fs.h        | import\_xattr\_name | 1         |
|         | /include/linux/fs.h        | getname             | 1         |
|         | /include/linux/fs.h        | putname             | 1         |
|         | /include/linux/xattr.h     | setxattr\_copy      | 1         |
|         | /include/linux/xattr.h     | file\_getxattr      | 1         |
|         | /include/linux/xattr.h     | file\_setxattr      | 1         |

### `zcrx.c`

| Source | Library                      | Function Utilized                  | Times Used |
| ------ | ---------------------------- | ---------------------------------- | ---------- |
| zcrx.c | /include/linux/kernel.h      | kzalloc                            | 3          |
|        | /include/linux/kernel.h      | kfree                              | 3          |
|        | /include/linux/slab.h        | kvfree                             | 4          |
|        | /include/linux/slab.h        | kvmalloc\_array                    | 3          |
|        | /include/linux/mm.h          | PAGE\_SIZE                         | 5          |
|        | /include/linux/mm.h          | unpin\_user\_pages                 | 1          |
|        | /include/linux/mm.h          | u64\_to\_user\_ptr                 | 4          |
|        | /include/linux/errno.h       | IS\_ERR                            | 1          |
|        | /include/linux/errno.h       | PTR\_ERR                           | 1          |
|        | /include/linux/dma-map-ops.h | dma\_map\_page\_attrs              | 2          |
|        | /include/linux/dma-map-ops.h | dma\_unmap\_page\_attrs            | 2          |
|        | /include/linux/dma-map-ops.h | dma\_mapping\_error                | 1          |
|        | /include/linux/dma-map-ops.h | \_\_dma\_sync\_single\_for\_device | 1          |
|        | /include/linux/netdevice.h   | netdev\_get\_by\_index             | 1          |
|        | /include/linux/netdevice.h   | netdev\_put                        | 2          |
|        | /include/linux/netdevice.h   | netdevice\_tracker                 | 1          |
|        | /include/linux/rtnetlink.h   | capable                            | 1          |
|        | /include/linux/rtnetlink.h   | CAP\_NET\_ADMIN                    | 1          |
|        | /include/linux/skbuff\_ref.h | page\_pool\_get\_dma\_addr\_netmem | 2          |
|        | /include/linux/skbuff\_ref.h | page\_pool\_put\_unrefed\_netmem   | 1          |
|        | /include/linux/skbuff\_ref.h | page\_pool\_unref\_netmem          | 1          |
|        | /include/linux/io\_uring.h   | io\_kiocb                          | 1          |
|        | /include/linux/io\_uring.h   | io\_ring\_ctx                      | 5          |
|        | /include/linux/io\_uring.h   | io\_free\_region                   | 1          |
|        | /include/linux/io\_uring.h   | io\_create\_region\_mmap\_safe     | 1          |
|        | /include/linux/io\_uring.h   | io\_region\_get\_ptr               | 1          |
|        | /include/linux/io\_uring.h   | io\_pin\_pages                     | 1          |
|        | /include/linux/io\_uring.h   | io\_buffer\_validate               | 1          |
|        | /include/linux/io\_uring.h   | io\_register\_zcrx\_ifq            | 1          |
|        | /include/linux/io\_uring.h   | io\_unregister\_zcrx\_ifqs         | 1          |
|        | /include/linux/io\_uring.h   | io\_shutdown\_zcrx\_ifqs           | 1          |
|        | /net/netlink.h               | memchr\_inv                        | 1          |
|        | /net/netdev\_rx\_queue.h     | net\_mp\_open\_rxq                 | 1          |
|        | /net/netdev\_rx\_queue.h     | net\_mp\_close\_rxq                | 1          |
|        | /net/netdev\_rx\_queue.h     | net\_mp\_niov\_set\_dma\_addr      | 2          |
|        | /net/netdev\_rx\_queue.h     | net\_iov\_to\_netmem               | 3          |
|        | /net/netdev\_rx\_queue.h     | net\_iov\_owner                    | 1          |
|        | /net/netdev\_rx\_queue.h     | net\_iov\_idx                      | 2          |
|        | /uapi/linux/io\_uring.h      | io\_uring\_zcrx\_ifq\_reg          | 1          |
|        | /uapi/linux/io\_uring.h      | io\_uring\_zcrx\_area\_reg         | 1          |
|        | /uapi/linux/io\_uring.h      | io\_uring\_zcrx\_rqe               | 1          |
|        | /uapi/linux/io\_uring.h      | io\_uring\_region\_desc            | 1          |

### `zcrx.h`

| Source | Library       | Function Utilized          | Time Used |
| ------ | ------------- | -------------------------- | --------- |
| zcrx.h | zcrx.h        | io\_register\_zcrx\_ifq    | 1         |
|        | zcrx.h        | io\_unregister\_zcrx\_ifqs | 1         |
|        | zcrx.h        | io\_shutdown\_zcrx\_ifqs   | 1         |
|        | zcrx.h        | io\_zcrx\_recv             | 1         |
|        | zcrx.h        | io\_recvzc                 | 1         |
|        | zcrx.h        | io\_recvzc\_prep           | 1         |
|        | linux/errno.h | -EOPNOTSUPP                | 2         |







