# Task 2: Dependency Injection
For this assigment, we want a little clarity regarding what kind of functions being imported and used on each source. Do note, we record all function actually being used by the source including function defined by itself if actually used inside the file. For the sake of completion, it's better if you straight disregard include list on the source. Instead, trace each function being used to the declared source.

Source | Libary | Function utilized | Time Used
-------|--------|--------------| ------------------
alloc_cache.h | /include/linux/kasan.h | kasan_mempool_unpoison_object | 1
| | arch/x86/include/asm/string_64.h| memset | 1
| | alloc_cache.h | io_alloc_cache_get | 1
| | alloc_cache.h | io_cache_alloc_new | 1
| | alloc_cache.h | io_alloc_cache_put | 1
| | linux/mm/slub.c | kfree | 1

Continue with the list until all functions used in each source are listed.

### opdef.c
| Source  | Library                | Function Utilized        | Time Used |
| ------- | ---------------------- | ------------------------ | --------- |
| opdef.c | opdef.c                | io\_uring\_optable\_init | 1         |
| opdef.c | opdef.c                | io\_uring\_op\_supported | 1         |
| opdef.c | include/linux/kernel.h | ARRAY\_SIZE              | 1         |
| opdef.c | include/linux/string.h | strcmp                   | 1         |
| opdef.c | opdef.c                | cold\_def\_fail          | 1         |
| opdef.c | opdef.c                | cold\_def\_cleanup       | 1         |
| opdef.c | io\_uring/opcode.h     | IORING\_OP\_LAST         | 1         |
| opdef.c | io\_uring/opdef.h      | io\_issue\_defs          | 1         |
| opdef.c | io\_uring/opdef.h      | io\_cold\_defs           | 1         |


### opdef.h
| Source  | Library               | Function Utilized        | Time Used |
| ------- | --------------------- | ------------------------ | --------- |
| opdef.h | include/linux/types.h | u8                       | 1         |
| opdef.h | opdef.h               | io\_uring\_op\_supported | 1         |
| opdef.h | opdef.h               | io\_uring\_optable\_init | 1         |

### openclose.c
| Source      | Library                | Function Utilized            | Time Used |
| ----------- | ---------------------- | ---------------------------- | --------- |
| openclose.c | openclose.c            | \_\_io\_close\_fixed         | 1         |
| openclose.c | openclose.c            | io\_openat\_prep             | 1         |
| openclose.c | openclose.c            | io\_openat                   | 1         |
| openclose.c | openclose.c            | io\_open\_cleanup            | 1         |
| openclose.c | openclose.c            | io\_openat2\_prep            | 1         |
| openclose.c | openclose.c            | io\_openat2                  | 1         |
| openclose.c | openclose.c            | io\_close\_prep              | 1         |
| openclose.c | openclose.c            | io\_close                    | 1         |
| openclose.c | openclose.c            | io\_install\_fixed\_fd\_prep | 1         |
| openclose.c | openclose.c            | io\_install\_fixed\_fd       | 1         |
| openclose.c | linux/fs/file.c        | filp\_close                  | 1         |
| openclose.c | linux/fs/file\_table.c | fput                         | 1         |
| openclose.c | linux/fs/open.c        | do\_filp\_open               | 2         |
| openclose.c | linux/fs/open.c        | build\_open\_flags           | 1         |
| openclose.c | linux/fs/file.c        | \_\_close\_fd                | 1         |
| openclose.c | linux/fs/file.c        | close\_fd\_get\_file         | 1         |
| openclose.c | linux/fs/file.c        | \_\_alloc\_file              | 1         |
| openclose.c | linux/fs/file\_table.c | fd\_install                  | 1         |
| openclose.c | include/linux/fs.h     | get\_unused\_fd\_flags       | 1         |
| openclose.c | linux/fs/file.c        | fdput                        | 1         |
| openclose.c | linux/fs/file.c        | put\_unused\_fd              | 1         |
| openclose.c | include/linux/kernel.h | IS\_ERR                      | 1         |
| openclose.c | include/linux/err.h    | PTR\_ERR                     | 1         |
| openclose.c | include/linux/err.h    | IS\_ERR\_OR\_NULL            | 1         |
| openclose.c | linux/fs/open.c        | open\_last\_lookups          | 1         |
| openclose.c | linux/fs/open.c        | open\_check\_o\_direct       | 1         |
| openclose.c | linux/fs/fdtable.c     | close\_fd                    | 1         |
| openclose.c | linux/fs/fdtable.c     | \_\_close\_range             | 1         |
| openclose.c | include/linux/cred.h   | current\_cred                | 1         |
| openclose.c | io\_uring/openclose.h  | io\_openat\_prep             | 1         |
| openclose.c | io\_uring/openclose.h  | io\_openat2\_prep            | 1         |

### openclose.h
| Source      | Library     | Function Utilized            | Time Used |
| ----------- | ----------- | ---------------------------- | --------- |
| openclose.h | openclose.h | \_\_io\_close\_fixed         | 1         |
| openclose.h | openclose.h | io\_openat\_prep             | 1         |
| openclose.h | openclose.h | io\_openat                   | 1         |
| openclose.h | openclose.h | io\_open\_cleanup            | 1         |
| openclose.h | openclose.h | io\_openat2\_prep            | 1         |
| openclose.h | openclose.h | io\_openat2                  | 1         |
| openclose.h | openclose.h | io\_close\_prep              | 1         |
| openclose.h | openclose.h | io\_close                    | 1         |
| openclose.h | openclose.h | io\_install\_fixed\_fd\_prep | 1         |
| openclose.h | openclose.h | io\_install\_fixed\_fd       | 1         |

### poll.c
| Source | Library                          | Function Utilized          | Time Used |
| ------ | -------------------------------- | -------------------------- | --------- |
| poll.c | poll.c                           | io\_poll\_add\_prep        | 1         |
| poll.c | poll.c                           | io\_poll\_add              | 1         |
| poll.c | poll.c                           | io\_poll\_remove\_prep     | 1         |
| poll.c | poll.c                           | io\_poll\_remove           | 1         |
| poll.c | poll.c                           | io\_poll\_cancel           | 1         |
| poll.c | poll.c                           | io\_arm\_poll\_handler     | 1         |
| poll.c | poll.c                           | io\_poll\_remove\_all      | 1         |
| poll.c | poll.c                           | io\_poll\_task\_func       | 1         |
| poll.c | poll.h                           | io\_poll\_multishot\_retry | 1         |
| poll.c | include/linux/fs.h               | vfs\_poll                  | 1         |
| poll.c | include/linux/poll.h             | poll\_wait                 | 1         |
| poll.c | include/linux/poll.h             | poll\_does\_not\_wait      | 1         |
| poll.c | include/linux/wait.h             | add\_wait\_queue           | 1         |
| poll.c | include/linux/wait.h             | remove\_wait\_queue        | 1         |
| poll.c | include/linux/wait.h             | wake\_up\_pollfree         | 1         |
| poll.c | include/linux/sched.h            | current                    | 1         |
| poll.c | include/linux/sched.h            | signal\_pending            | 1         |
| poll.c | include/linux/sched/signal.h     | fatal\_signal\_pending     | 1         |
| poll.c | include/linux/err.h              | IS\_ERR                    | 1         |
| poll.c | include/linux/err.h              | PTR\_ERR                   | 1         |
| poll.c | include/linux/errno.h            | -EAGAIN (constant)         | 1         |
| poll.c | include/linux/errno.h            | -ENOENT (constant)         | 1         |
| poll.c | include/linux/errno.h            | -EINVAL (constant)         | 1         |
| poll.c | include/linux/errno.h            | -EBUSY (constant)          | 1         |
| poll.c | include/linux/io\_uring\_types.h | io\_kiocb, io\_ring\_ctx   | 1         |
| poll.c | kernel/time/timer.c              | mod\_timer                 | 1         |
| poll.c | include/linux/spinlock.h         | spin\_lock\_irqsave        | 1         |
| poll.c | include/linux/spinlock.h         | spin\_unlock\_irqrestore   | 1         |
| poll.c | include/linux/atomic.h           | atomic\_dec\_and\_test     | 1         |
| poll.c | include/linux/atomic.h           | atomic\_inc                | 1         |
| poll.c | include/linux/rcupdate.h         | rcu\_read\_lock            | 1         |
| poll.c | include/linux/rcupdate.h         | rcu\_read\_unlock          | 1         |
| poll.c | include/linux/slab.h             | kzalloc                    | 1         |
| poll.c | include/linux/slab.h             | kfree                      | 1         |
| poll.c | include/linux/kernel.h           | likely                     | 1         |
| poll.c | include/linux/kernel.h           | unlikely                   | 1         |
| poll.c | include/linux/io\_uring.h        | io\_put\_req               | 1         |
| poll.c | include/linux/io\_uring.h        | io\_req\_complete          | 1         |
| poll.c | include/linux/io\_uring.h        | io\_req\_task\_complete    | 1         |

### poll.h
| Source | Library        | Function Utilized          | Time Used |
| ------ | -------------- | -------------------------- | --------- |
| poll.h | poll.h         | io\_poll\_multishot\_retry | 1         |
| poll.h | poll.h         | io\_poll\_add\_prep        | 1         |
| poll.h | poll.h         | io\_poll\_add              | 1         |
| poll.h | poll.h         | io\_poll\_remove\_prep     | 1         |
| poll.h | poll.h         | io\_poll\_remove           | 1         |
| poll.h | poll.h         | io\_poll\_cancel           | 1         |
| poll.h | poll.h         | io\_arm\_poll\_handler     | 1         |
| poll.h | poll.h         | io\_poll\_remove\_all      | 1         |
| poll.h | poll.h         | io\_poll\_task\_func       | 1         |
| poll.h | linux/atomic.h | atomic\_inc                | 1         |

### refs.h
| Source | Library          | Function Utilized                       | Time Used |
| ------ | ---------------- | --------------------------------------- | --------- |
| refs.h | refs.h           | req\_ref\_zero\_or\_close\_to\_overflow | 2         |
| refs.h | refs.h           | req\_ref\_inc\_not\_zero                | 1         |
| refs.h | refs.h           | req\_ref\_put\_and\_test\_atomic        | 1         |
| refs.h | refs.h           | req\_ref\_put\_and\_test                | 1         |
| refs.h | refs.h           | req\_ref\_get                           | 1         |
| refs.h | refs.h           | req\_ref\_put                           | 1         |
| refs.h | refs.h           | \_\_io\_req\_set\_refcount              | 1         |
| refs.h | refs.h           | io\_req\_set\_refcount                  | 1         |
| refs.h | linux/atomic.h   | atomic\_read                            | 1         |
| refs.h | linux/atomic.h   | atomic\_inc\_not\_zero                  | 1         |
| refs.h | linux/atomic.h   | atomic\_dec\_and\_test                  | 2         |
| refs.h | linux/atomic.h   | atomic\_inc                             | 1         |
| refs.h | linux/atomic.h   | atomic\_dec                             | 1         |
| refs.h | linux/atomic.h   | atomic\_set                             | 1         |
| refs.h | linux/compiler.h | likely                                  | 1         |
| refs.h | linux/compiler.h | data\_race                              | 1         |
| refs.h | linux/bug.h      | WARN\_ON\_ONCE                          | 6         |

### register.c
| Source     | Library                          | Function Utilized              | Time Used |
| ---------- | -------------------------------- | ------------------------------ | --------- |
| register.c | register.c                       | io\_eventfd\_unregister        | 1         |
| register.c | register.c                       | io\_unregister\_personality    | 1         |
| register.c | register.c                       | io\_uring\_register\_get\_file | 1         |
| register.c | include/linux/file.h             | fget                           | 1         |
| register.c | include/linux/file.h             | fput                           | 1         |
| register.c | include/linux/file.h             | get\_file                      | 1         |
| register.c | include/linux/fs.h               | eventfd\_ctx\_fdget            | 1         |
| register.c | include/linux/fs.h               | eventfd\_ctx\_put              | 1         |
| register.c | include/linux/fs.h               | fd\_install                    | 1         |
| register.c | include/linux/fs.h               | get\_unused\_fd\_flags         | 1         |
| register.c | include/linux/fs.h               | put\_unused\_fd                | 1         |
| register.c | include/linux/slab.h             | kzalloc                        | 1         |
| register.c | include/linux/slab.h             | kfree                          | 1         |
| register.c | include/linux/errno.h            | -EINVAL (constant)             | 1         |
| register.c | include/linux/errno.h            | -EBADF (constant)              | 1         |
| register.c | include/linux/errno.h            | -EEXIST (constant)             | 1         |
| register.c | include/linux/errno.h            | -ENOENT (constant)             | 1         |
| register.c | include/linux/uaccess.h          | copy\_from\_user               | 1         |
| register.c | include/linux/uaccess.h          | copy\_to\_user                 | 1         |
| register.c | include/linux/rcupdate.h         | rcu\_read\_lock                | 1         |
| register.c | include/linux/rcupdate.h         | rcu\_read\_unlock              | 1         |
| register.c | include/linux/rculist.h          | list\_del\_rcu                 | 1         |
| register.c | include/linux/rculist.h          | list\_add\_tail\_rcu           | 1         |
| register.c | include/linux/sched.h            | current                        | 1         |
| register.c | include/linux/io\_uring\_types.h | io\_ring\_ctx, io\_personality | 1         |
| register.c | include/linux/refcount.h         | refcount\_inc\_not\_zero       | 1         |
| register.c | include/linux/refcount.h         | refcount\_dec\_and\_test       | 1         |
| register.c | include/linux/mutex.h            | mutex\_lock                    | 1         |
| register.c | include/linux/mutex.h            | mutex\_unlock                  | 1         |
| register.c | include/linux/kernel.h           | likely                         | 1         |
| register.c | include/linux/kernel.h           | unlikely                       | 1         |
| register.c | include/linux/idr.h              | ida\_alloc                     | 1         |
| register.c | include/linux/idr.h              | ida\_free                      | 1         |
| register.c | include/linux/fs.h               | put\_unused\_fd                | 1         |

### register.h
| Source     | Library    | Function Utilized              | Time Used |
| ---------- | ---------- | ------------------------------ | --------- |
| register.h | register.h | io\_eventfd\_unregister        | 1         |
| register.h | register.h | io\_unregister\_personality    | 1         |
| register.h | register.h | io\_uring\_register\_get\_file | 1         |

### rsrc.c
| Source | Library                               | Function Utilized                                | Time Used |
| ------ | -------------------------------- | ------------------------------------------------ | --------- |
| rsrc.c | rsrc.c                           | io\_rsrc\_cache\_init                            | 1         |
| rsrc.c | rsrc.c                           | io\_rsrc\_cache\_free                            | 1         |
| rsrc.c | rsrc.c                           | io\_rsrc\_node\_alloc                            | 1         |
| rsrc.c | rsrc.c                           | io\_free\_rsrc\_node                             | 1         |
| rsrc.c | rsrc.c                           | io\_rsrc\_data\_free                             | 1         |
| rsrc.c | rsrc.c                           | io\_rsrc\_data\_alloc                            | 1         |
| rsrc.c | rsrc.c                           | io\_find\_buf\_node                              | 1         |
| rsrc.c | rsrc.c                           | io\_import\_reg\_buf                             | 1         |
| rsrc.c | rsrc.c                           | io\_import\_reg\_vec                             | 1         |
| rsrc.c | rsrc.c                           | io\_prep\_reg\_iovec                             | 1         |
| rsrc.c | rsrc.c                           | io\_register\_clone\_buffers                     | 1         |
| rsrc.c | rsrc.c                           | io\_sqe\_buffers\_unregister                     | 1         |
| rsrc.c | rsrc.c                           | io\_sqe\_buffers\_register                       | 1         |
| rsrc.c | rsrc.c                           | io\_sqe\_files\_unregister                       | 1         |
| rsrc.c | rsrc.c                           | io\_sqe\_files\_register                         | 1         |
| rsrc.c | rsrc.c                           | io\_register\_files\_update                      | 1         |
| rsrc.c | rsrc.c                           | io\_register\_rsrc\_update                       | 1         |
| rsrc.c | rsrc.c                           | io\_register\_rsrc                               | 1         |
| rsrc.c | rsrc.c                           | io\_buffer\_validate                             | 1         |
| rsrc.c | rsrc.c                           | io\_check\_coalesce\_buffer                      | 1         |
| rsrc.c | rsrc.c                           | io\_files\_update                                | 1         |
| rsrc.c | rsrc.c                           | io\_files\_update\_prep                          | 1         |
| rsrc.c | rsrc.c                           | \_\_io\_account\_mem                             | 1         |
| rsrc.c | rsrc.c                           | io\_vec\_free                                    | 1         |
| rsrc.c | rsrc.c                           | io\_vec\_realloc                                 | 1         |
| rsrc.c | rsrc.c                           | io\_req\_put\_rsrc\_nodes (inline)               | 1         |
| rsrc.c | rsrc.c                           | io\_reset\_rsrc\_node (inline)                   | 1         |
| rsrc.c | rsrc.c                           | io\_req\_assign\_rsrc\_node (inline)             | 1         |
| rsrc.c | rsrc.c                           | io\_req\_assign\_buf\_node (inline)              | 1         |
| rsrc.c | include/linux/mm.h               | get\_user\_pages\_remote                         | 1         |
| rsrc.c | include/linux/mm.h               | put\_page                                        | 1         |
| rsrc.c | include/linux/mm.h               | get\_user\_pages\_fast                           | 1         |
| rsrc.c | include/linux/uaccess.h          | copy\_from\_user                                 | 1         |
| rsrc.c | include/linux/io\_uring\_types.h | struct io\_ring\_ctx, struct io\_kiocb           | 1         |
| rsrc.c | include/linux/slab.h             | kmalloc, kfree                                   | 1         |
| rsrc.c | include/linux/atomic.h           | atomic\_inc, atomic\_dec                         | 1         |
| rsrc.c | include/linux/refcount.h         | refcount\_inc, refcount\_dec                     | 1         |
| rsrc.c | include/linux/vmalloc.h          | vmalloc, vfree                                   | 1         |
| rsrc.c | include/linux/errno.h            | error codes (e.g., -ENOMEM)                      | 1         |
| rsrc.c | include/linux/kernel.h           | IS\_ENABLED, likely, unlikely                    | 1         |
| rsrc.c | include/linux/nospec.h           | array\_index\_nospec                             | 1         |
| rsrc.c | include/linux/lockdep.h          | lockdep\_assert\_held                            | 1         |
| rsrc.c | include/linux/highmem.h          | kmap, kunmap                                     | 1         |
| rsrc.c | include/linux/mm\_types.h        | struct page                                      | 1         |
| rsrc.c | include/linux/sched/mm.h         | get\_task\_mm, mmput                             | 1         |
| rsrc.c | include/linux/user\_namespace.h  | struct user\_struct                              | 1         |
| rsrc.c | include/linux/mm.h               | atomic\_long\_add/sub for \_\_io\_unaccount\_mem | 1         |

### rsrc.h
| Source | Library                          | Function Utilized                         | Time Used |
| ------ | -------------------------------- | ----------------------------------------- | --------- |
| rsrc.h | rsrc.h                           | io\_rsrc\_cache\_init                     | 1         |
| rsrc.h | rsrc.h                           | io\_rsrc\_cache\_free                     | 1         |
| rsrc.h | rsrc.h                           | io\_rsrc\_node\_alloc                     | 1         |
| rsrc.h | rsrc.h                           | io\_free\_rsrc\_node                      | 1         |
| rsrc.h | rsrc.h                           | io\_rsrc\_data\_free                      | 1         |
| rsrc.h | rsrc.h                           | io\_rsrc\_data\_alloc                     | 1         |
| rsrc.h | rsrc.h                           | io\_find\_buf\_node                       | 1         |
| rsrc.h | rsrc.h                           | io\_import\_reg\_buf                      | 1         |
| rsrc.h | rsrc.h                           | io\_import\_reg\_vec                      | 1         |
| rsrc.h | rsrc.h                           | io\_prep\_reg\_iovec                      | 1         |
| rsrc.h | rsrc.h                           | io\_register\_clone\_buffers              | 1         |
| rsrc.h | rsrc.h                           | io\_sqe\_buffers\_unregister              | 1         |
| rsrc.h | rsrc.h                           | io\_sqe\_buffers\_register                | 1         |
| rsrc.h | rsrc.h                           | io\_sqe\_files\_unregister                | 1         |
| rsrc.h | rsrc.h                           | io\_sqe\_files\_register                  | 1         |
| rsrc.h | rsrc.h                           | io\_register\_files\_update               | 1         |
| rsrc.h | rsrc.h                           | io\_register\_rsrc\_update                | 1         |
| rsrc.h | rsrc.h                           | io\_register\_rsrc                        | 1         |
| rsrc.h | rsrc.h                           | io\_buffer\_validate                      | 1         |
| rsrc.h | rsrc.h                           | io\_check\_coalesce\_buffer               | 1         |
| rsrc.h | rsrc.h                           | io\_rsrc\_node\_lookup (inline)           | 1         |
| rsrc.h | rsrc.h                           | io\_put\_rsrc\_node (inline)              | 1         |
| rsrc.h | rsrc.h                           | io\_reset\_rsrc\_node (inline)            | 1         |
| rsrc.h | rsrc.h                           | io\_req\_put\_rsrc\_nodes (inline)        | 1         |
| rsrc.h | rsrc.h                           | io\_req\_assign\_rsrc\_node (inline)      | 1         |
| rsrc.h | rsrc.h                           | io\_req\_assign\_buf\_node (inline)       | 1         |
| rsrc.h | rsrc.h                           | io\_files\_update                         | 1         |
| rsrc.h | rsrc.h                           | io\_files\_update\_prep                   | 1         |
| rsrc.h | rsrc.h                           | \_\_io\_account\_mem                      | 1         |
| rsrc.h | rsrc.h                           | \_\_io\_unaccount\_mem (inline)           | 1         |
| rsrc.h | rsrc.h                           | io\_vec\_free                             | 1         |
| rsrc.h | rsrc.h                           | io\_vec\_realloc                          | 1         |
| rsrc.h | rsrc.h                           | io\_vec\_reset\_iovec (inline)            | 1         |
| rsrc.h | rsrc.h                           | io\_alloc\_cache\_vec\_kasan (inline)     | 1         |
| rsrc.h | include/linux/atomic.h           | atomic\_long\_sub                         | 1         |
| rsrc.h | include/linux/lockdep.h          | lockdep\_assert\_held                     | 1         |
| rsrc.h | include/linux/io\_uring\_types.h | struct io\_ring\_ctx, io\_kiocb, iou\_vec | 1         |
| rsrc.h | include/linux/uaccess.h          | \_\_user (used in pointer annotations)    | 1         |
| rsrc.h | include/linux/bio.h              | struct bio\_vec                           | 1         |
| rsrc.h | include/linux/refcount.h         | refcount\_t, refcount APIs                | 1         |
| rsrc.h | include/linux/kernel.h           | IS\_ENABLED(CONFIG\_KASAN)                | 1         |
| rsrc.h | include/linux/nospec.h           | array\_index\_nospec                      | 1         |

### rw.c
| Source | Library                                 | Function Utilized                      | Time Used |
| ------ | --------------------------------------- | -------------------------------------- | --------- |
| rw\.c  | rw\.c                                   | io\_read                               | 1         |
| rw\.c  | rw\.c                                   | io\_write                              | 1         |
| rw\.c  | rw\.c                                   | io\_readv                              | 1         |
| rw\.c  | rw\.c                                   | io\_writev                             | 1         |
| rw\.c  | rw\.c                                   | io\_read\_fixed                        | 1         |
| rw\.c  | rw\.c                                   | io\_write\_fixed                       | 1         |
| rw\.c  | rw\.c                                   | io\_prep\_readv                        | 1         |
| rw\.c  | rw\.c                                   | io\_prep\_writev                       | 1         |
| rw\.c  | rw\.c                                   | io\_prep\_read\_fixed                  | 1         |
| rw\.c  | rw\.c                                   | io\_prep\_write\_fixed                 | 1         |
| rw\.c  | rw\.c                                   | io\_prep\_read                         | 1         |
| rw\.c  | rw\.c                                   | io\_prep\_write                        | 1         |
| rw\.c  | rw\.c                                   | io\_readv\_writev\_cleanup             | 1         |
| rw\.c  | rw\.c                                   | io\_rw\_fail                           | 1         |
| rw\.c  | rw\.c                                   | io\_req\_rw\_complete                  | 1         |
| rw\.c  | rw\.c                                   | io\_read\_mshot\_prep                  | 1         |
| rw\.c  | rw\.c                                   | io\_read\_mshot                        | 1         |
| rw\.c  | rw\.c                                   | io\_rw\_cache\_free                    | 1         |
| rw\.c  | rsrc.h                                  | io\_import\_reg\_buf                   | 1         |
| rw\.c  | rsrc.h                                  | io\_req\_put\_rsrc\_nodes              | 1         |
| rw\.c  | rsrc.h                                  | io\_req\_assign\_buf\_node             | 1         |
| rw\.c  | rsrc.h                                  | io\_buffer\_validate                   | 1         |
| rw\.c  | rsrc.h                                  | io\_find\_buf\_node                    | 1         |
| rw\.c  | rsrc.h                                  | io\_import\_reg\_vec                   | 1         |
| rw\.c  | rsrc.h                                  | io\_prep\_reg\_iovec                   | 1         |
| rw\.c  | rsrc.h                                  | io\_vec\_reset\_iovec                  | 1         |
| rw\.c  | rsrc.h                                  | io\_alloc\_cache\_vec\_kasan           | 1         |
| rw\.c  | rsrc.h                                  | io\_vec\_free                          | 1         |
| rw\.c  | refs.h                                  | req\_ref\_put                          | 1         |
| rw\.c  | refs.h                                  | req\_ref\_get                          | 1         |
| rw\.c  | refs.h                                  | io\_req\_set\_refcount                 | 1         |
| rw\.c  | io\_uring\_types.h (or internal struct) | io\_kiocb (struct used)                | -         |
| rw\.c  | linux/uio.h                             | iov\_iter\_\* family (e.g. init, copy) | 1         |
| rw\.c  | linux/uio.h                             | iov\_iter\_init                        | 1         |
| rw\.c  | linux/slab.h                            | kmalloc / kfree                        | 1         |
| rw\.c  | linux/kernel.h                          | likely / unlikely macros               | 1         |
| rw\.c  | linux/fs.h                              | iov\_iter\_advance / iov\_iter\_count  | 1         |
| rw\.c  | linux/bio.h                             | bio\_copy\_user\_iov / bio\_add\_page  | 1         |
| rw\.c  | linux/pagemap.h                         | generic\_perform\_write                | 1         |
| rw\.c  | asm/uaccess.h                           | copy\_from\_user / copy\_to\_user      | 1         |
| rw\.c  | linux/uio.h                             | import\_iovec                          | 1         |

### rw.h
| Source | Library | Function Utilized          | Time Used |
| ------ | ------- | -------------------------- | --------- |
| rw\.h  | rw\.h   | io\_prep\_read\_fixed      | 1         |
| rw\.h  | rw\.h   | io\_prep\_write\_fixed     | 1         |
| rw\.h  | rw\.h   | io\_prep\_readv\_fixed     | 1         |
| rw\.h  | rw\.h   | io\_prep\_writev\_fixed    | 1         |
| rw\.h  | rw\.h   | io\_prep\_readv            | 1         |
| rw\.h  | rw\.h   | io\_prep\_writev           | 1         |
| rw\.h  | rw\.h   | io\_prep\_read             | 1         |
| rw\.h  | rw\.h   | io\_prep\_write            | 1         |
| rw\.h  | rw\.h   | io\_read                   | 1         |
| rw\.h  | rw\.h   | io\_write                  | 1         |
| rw\.h  | rw\.h   | io\_read\_fixed            | 1         |
| rw\.h  | rw\.h   | io\_write\_fixed           | 1         |
| rw\.h  | rw\.h   | io\_read\_mshot\_prep      | 1         |
| rw\.h  | rw\.h   | io\_read\_mshot            | 1         |
| rw\.h  | rw\.h   | io\_readv\_writev\_cleanup | 1         |
| rw\.h  | rw\.h   | io\_rw\_fail               | 1         |
| rw\.h  | rw\.h   | io\_req\_rw\_complete      | 1         |
| rw\.h  | rw\.h   | io\_rw\_cache\_free        | 1         |

### slist.h
| Source  | Library                       | Function Utilized                        | Time Used |
| ------- | ----------------------------- | ---------------------------------------- | --------- |
| slist.h | linux/io\_uring\_types.h      | struct io\_wq\_work\_node, io\_wq\_work  | 1         |
| slist.h | slist.h                       | \_\_wq\_list\_for\_each (macro)          | 1         |
| slist.h | slist.h                       | wq\_list\_for\_each (macro)              | 1         |
| slist.h | slist.h                       | wq\_list\_for\_each\_resume (macro)      | 1         |
| slist.h | slist.h                       | wq\_list\_empty                          | 1         |
| slist.h | slist.h                       | INIT\_WQ\_LIST                           | 1         |
| slist.h | slist.h                       | wq\_list\_add\_after                     | 1         |
| slist.h | slist.h                       | wq\_list\_add\_tail                      | 1         |
| slist.h | slist.h                       | wq\_list\_add\_head                      | 1         |
| slist.h | slist.h                       | wq\_list\_cut                            | 1         |
| slist.h | slist.h                       | \_\_wq\_list\_splice                     | 1         |
| slist.h | slist.h                       | wq\_list\_splice                         | 1         |
| slist.h | slist.h                       | wq\_stack\_add\_head                     | 1         |
| slist.h | slist.h                       | wq\_list\_del                            | 1         |
| slist.h | slist.h                       | wq\_stack\_extract                       | 1         |
| slist.h | slist.h                       | wq\_next\_work                           | 1         |
| slist.h | linux/compiler.h (implisit)   | likely/unlikely/WRITE\_ONCE macros       | 1         |
| slist.h | linux/container\_of.h (impl.) | container\_of macro (for wq\_next\_work) | 1         |

### splice.c
| Source   | Library                  | Function Utilized      | Time Used |
| -------- | ------------------------ | ---------------------- | --------- |
| splice.c | splice.h                 | io\_tee\_prep          | 1         |
| splice.c | splice.h                 | io\_tee                | 1         |
| splice.c | splice.h                 | io\_splice\_prep       | 1         |
| splice.c | splice.h                 | io\_splice             | 1         |
| splice.c | splice.h                 | io\_splice\_cleanup    | 1         |
| splice.c | linux/fs.h               | vfs\_splice\_read      | 1         |
| splice.c | linux/fs.h               | do\_splice             | 1         |
| splice.c | linux/fs.h               | do\_tee                | 1         |
| splice.c | linux/file.h             | fget                   | 1         |
| splice.c | linux/file.h             | fput                   | 1         |
| splice.c | linux/uio.h              | iov\_iter\_is\_xxx     | 1         |
| splice.c | linux/errno.h            | -EFAULT, -EINVAL, etc. | 1         |
| splice.c | linux/io\_uring\_types.h | struct io\_kiocb       | 1         |
| splice.c | linux/io\_uring\_types.h | req->file, req->ctx    | 1         |
| splice.c | linux/io\_uring.h        | struct io\_uring\_sqe  | 1         |
| splice.c | linux/kernel.h           | likely/unlikely macros | 1         |

### splice.h
| Source   | Library                   | Function Utilized     | Time Used |
| -------- | ------------------------- | --------------------- | --------- |
| splice.h | splice.h                  | io\_tee\_prep         | 1         |
| splice.h | splice.h                  | io\_tee               | 1         |
| splice.h | splice.h                  | io\_splice\_prep      | 1         |
| splice.h | splice.h                  | io\_splice            | 1         |
| splice.h | splice.h                  | io\_splice\_cleanup   | 1         |