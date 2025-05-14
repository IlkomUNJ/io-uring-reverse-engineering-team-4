### advice.c

| Source   | Library  | Function Utilized         | Times Used |
| -------- | -------- | ------------------------- | ---------- |
| advise.c | advise.c | io\_madvise\_prep         | 1          |
| advise.c | advise.c | io\_madvise               | 1          |
| advise.c | advise.c | io\_fadvise\_prep         | 1          |
| advise.c | advise.c | io\_fadvise               | 1          |
| advise.c | advise.c | io\_fadvise\_force\_async | 2          |
| advise.c | advise.c | io\_kiocb\_to\_cmd        | 4          |
| advise.c | advise.c | READ\_ONCE                | 7          |
| advise.c | advise.c | WARN\_ON\_ONCE            | 2          |
| advise.c | advise.c | do\_madvise               | 1          |
| advise.c | advise.c | io\_req\_set\_res         | 2          |
| advise.c | advise.c | vfs\_fadvise              | 1          |
| advise.c | advise.c | req\_set\_fail            | 1          |
| advise.c | <linux/kernel.h>        | WARN\_ON\_ONCE            | 2          |
| advise.c | <linux/errno.h>         | -                         | 0          |
| advise.c | <linux/fs.h>            | vfs\_fadvise              | 1          |
| advise.c | <linux/file.h>          | -                         | 0          |
| advise.c | <linux/mm.h>            | do\_madvise               | 1          |
| advise.c | <linux/slab.h>          | -                         | 0          |
| advise.c | <linux/namei.h>         | -                         | 0          |
| advise.c | <linux/io_uring.h>      | io\_fadvise\_prep         | 1          |
| advise.c | <uapi/linux/fadvise.h>  | -                         | 0          |
| advise.c | <uapi/linux/io_uring.h> | io\_madvise\_prep         | 1          |
| advise.c | "io_uring.h"            | io\_kiocb\_to\_cmd        | 4          |
| advise.c | "advise.h"              | io\_fadvise\_force\_async | 2          |
| advise.c | "advise.h"              | io\_fadvise               | 1          |
| advise.c | "io_uring.h"            | io\_req\_set\_res         | 2          |
| advise.c | "io_uring.h"            | req\_set\_fail            | 1          |
| advise.c | "io_uring.h"            | READ\_ONCE                | 7          |

### advise.h

| Source   | Library        | Function Utilized | Times Used |
| -------- | -------------- | ----------------- | ---------- |
| advise.h | io_uring.h | io\_madvise\_prep | 1          |
| advise.h | io_uring.h | io\_madvise       | 1          |
| advise.h | io_uring.h | io\_fadvise\_prep | 1          |
| advise.h | io_uring.h | io\_fadvise       | 1          |

### alloc_cache.c

| Source         | Library            | Function Utilized      | Times Used |
| -------------- | ------------------ | ---------------------- | ---------- |
| alloc\_cache.c | alloc_cache.h  | io\_alloc\_cache\_free | 1          |
| alloc\_cache.c | alloc_cache.h  | io\_alloc\_cache\_init | 1          |
| alloc\_cache.c | alloc_cache.h  | io\_cache\_alloc\_new  | 1          |
| alloc\_cache.c | <linux/slab.h>   | kvmalloc\_array        | 1          |
| alloc\_cache.c | <linux/slab.h>   | kvfree                 | 1          |
| alloc\_cache.c | <linux/slab.h>   | kmalloc                | 1          |
| alloc\_cache.c | <linux/string.h> | memset                 | 1          |

### alloc_cache.h
| Source         | Library                    | Function Utilized                | Times Used |
| -------------- | -------------------------- | -------------------------------- | ---------- |
| alloc\_cache.h | <linux/io_uring_types.h> | -                                | 0          |
| alloc\_cache.h | <linux/slab.h>           | kasan\_mempool\_poison\_object   | 1          |
| alloc\_cache.h | <linux/slab.h>           | kasan\_mempool\_unpoison\_object | 1          |
| alloc\_cache.h | <linux/slab.h>           | memset                           | 3          |
| alloc\_cache.h | <linux/slab.h>           | kfree                            | 1          |
| alloc\_cache.h | <linux/slab.h>           | kmalloc                          | 1          |
| alloc\_cache.h | <linux/slab.h>           | kvmalloc\_array                  | 1          |

### cancel.c
| Source   | Library              | Function Utilized           | Times Used |
| -------- | -------------------- | --------------------------- | ---------- |
| cancel.c | <linux/kernel.h>   | WARN\_ON\_ONCE              | 1          |
| cancel.c | <linux/errno.h>    | -                           | 0          |
| cancel.c | <linux/fs.h>       | -                           | 0          |
| cancel.c | <linux/file.h>     | -                           | 0          |
| cancel.c | <linux/mm.h>       | -                           | 0          |
| cancel.c | <linux/slab.h>     | kfree                       | 1          |
| cancel.c | <linux/namei.h>    | -                           | 0          |
| cancel.c | <linux/nospec.h>   | -                           | 0          |
| cancel.c | <linux/io_uring.h> | io\_kiocb\_to\_cmd          | 1          |
| cancel.c | <linux/io_uring.h> | io\_file\_get\_fixed        | 1          |
| cancel.c | <linux/io_uring.h> | io\_file\_get\_normal       | 1          |
| cancel.c | <linux/io_uring.h> | io\_wq\_cancel\_cb          | 1          |
| cancel.c | <linux/io_uring.h> | io\_ring\_submit\_lock      | 2          |
| cancel.c | <linux/io_uring.h> | io\_ring\_submit\_unlock    | 2          |
| cancel.c | <linux/io_uring.h> | io\_wq\_current\_is\_worker | 1          |
| cancel.c | <linux/io_uring.h> | io\_req\_set\_res           | 1          |
| cancel.c | <linux/io_uring.h> | io\_tctx\_node              | 1          |
| cancel.c | <linux/io_uring.h> | io\_match\_task\_safe       | 1          |
| cancel.c | <linux/io_uring.h> | io\_cancel\_match\_sequence | 1          |
| cancel.c | <linux/io_uring.h> | io\_ring\_ctx               | 1          |

### cancel.h
| Source   | Library                    | Function Utilized       | Times Used |
| -------- | -------------------------- | ----------------------- | ---------- |
| cancel.h | <linux/io_uring_types.h> | -                       | 0          |
| cancel.h | <linux/io_uring.h>       | io\_async\_cancel\_prep | 1          |
| cancel.h | <linux/io_uring.h>       | io\_async\_cancel       | 1          |
| cancel.h | <linux/io_uring.h>       | io\_try\_cancel         | 1          |
| cancel.h | <linux/io_uring.h>       | io\_sync\_cancel        | 1          |
| cancel.h | <linux/io_uring.h>       | io\_cancel\_req\_match  | 2          |
| cancel.h | <linux/io_uring.h>       | io\_cancel\_remove\_all | 1          |
| cancel.h | <linux/io_uring.h>       | io\_cancel\_remove      | 1          |

### epoll.c
| Source  | Library               | Function Utilized  | Times Used |
| ------- | --------------------- | ------------------ | ---------- |
| epoll.c | <linux/kernel.h>    | WARN\_ON\_ONCE     | 0          |
| epoll.c | <linux/errno.h>     | -                  | 0          |
| epoll.c | <linux/file.h>      | -                  | 0          |
| epoll.c | <linux/fs.h>        | -                  | 0          |
| epoll.c | <linux/uaccess.h>   | copy\_from\_user   | 1          |
| epoll.c | <linux/io_uring.h>  | io\_kiocb\_to\_cmd | 3          |
| epoll.c | <linux/io_uring.h>  | io\_req\_set\_res  | 2          |
| epoll.c | <linux/eventpoll.h> | do\_epoll\_ctl     | 1          |
| epoll.c | <linux/eventpoll.h> | epoll\_sendevents  | 1          |

### epoll.h
| Source  | Library                    | Function Utilized     | Times Used |
| ------- | -------------------------- | --------------------- | ---------- |
| epoll.h | <linux/io_uring_types.h> | -                     | 0          |
| epoll.h | <linux/io_uring.h>       | io\_epoll\_ctl\_prep  | 1          |
| epoll.h | <linux/io_uring.h>       | io\_epoll\_ctl        | 1          |
| epoll.h | <linux/io_uring.h>       | io\_epoll\_wait\_prep | 1          |
| epoll.h | <linux/io_uring.h>       | io\_epoll\_wait       | 1          |

### eventfd.c
| Source    | Library               | Function Utilized     | Times Used |
| --------- | --------------------- | --------------------- | ---------- |
| eventfd.c | <linux/kernel.h>    | kmalloc               | 1          |
| eventfd.c | <linux/errno.h>     | -                     | 0          |
| eventfd.c | <linux/mm.h>        | -                     | 0          |
| eventfd.c | <linux/slab.h>      | kmalloc               | 1          |
| eventfd.c | <linux/eventfd.h>   | eventfd\_ctx\_put     | 1          |
| eventfd.c | <linux/eventfd.h>   | eventfd\_signal\_mask | 3          |
| eventfd.c | <linux/eventpoll.h> | -                     | 0          |
| eventfd.c | <linux/io_uring.h>  | rcu\_read\_lock       | 2          |
| eventfd.c | <linux/io_uring.h>  | rcu\_dereference      | 1          |
| eventfd.c | <linux/io_uring.h>  | refcount\_set         | 1          |
| eventfd.c | <linux/io_uring.h>  | atomic\_set           | 1          |

### eventfd.h
| Source    | Library                    | Function Utilized          | Times Used |
| --------- | -------------------------- | -------------------------- | ---------- |
| eventfd.h | <linux/io_uring_types.h> | -                          | 0          |
| eventfd.h | <linux/io_uring.h>       | io\_eventfd\_register      | 1          |
| eventfd.h | <linux/io_uring.h>       | io\_eventfd\_unregister    | 1          |
| eventfd.h | <linux/io_uring.h>       | io\_eventfd\_flush\_signal | 1          |
| eventfd.h | <linux/io_uring.h>       | io\_eventfd\_signal        | 1          |

### fdinfo.c
| Source   | Library              | Function Utilized       | Times Used |
| -------- | -------------------- | ----------------------- | ---------- |
| fdinfo.c | \<linux/kernel.h>    | -                       | 0          |
| fdinfo.c | \<linux/errno.h>     | -                       | 0          |
| fdinfo.c | \<linux/fs.h>        | -                       | 0          |
| fdinfo.c | \<linux/file.h>      | -                       | 0          |
| fdinfo.c | \<linux/proc\_fs.h>  | -                       | 0          |
| fdinfo.c | \<linux/seq\_file.h> | seq\_printf             | Multiple   |
| fdinfo.c | \<linux/seq\_file.h> | seq\_puts               | Multiple   |
| fdinfo.c | \<linux/seq\_file.h> | seq\_put\_decimal\_ull  | Multiple   |
| fdinfo.c | \<linux/seq\_file.h> | seq\_put\_hex\_ll       | 1          |
| fdinfo.c | \<linux/seq\_file.h> | seq\_putc               | 1          |
| fdinfo.c | \<linux/io\_uring.h> | io\_uring\_show\_fdinfo | 1          |
| fdinfo.c | \<linux/io\_uring.h> | io\_uring\_get\_opcode  | 1          |
| fdinfo.c | \<linux/io\_uring.h> | io\_slot\_file          | Multiple   |
| fdinfo.c | \<linux/io\_uring.h> | seq\_file\_path         | Multiple   |
| fdinfo.c | \<linux/io\_uring.h> | io\_uring\_show\_cred   | Multiple   |
| fdinfo.c | \<linux/io\_uring.h> | seq\_printf             | Multiple   |
| fdinfo.c | \<linux/io\_uring.h> | seq\_put\_decimal\_ull  | Multiple   |
| fdinfo.c | \<linux/io\_uring.h> | seq\_put\_hex\_ll       | 1          |
| fdinfo.c | \<linux/io\_uring.h> | xa\_for\_each           | 1          |
| fdinfo.c | \<linux/io\_uring.h> | hlist\_for\_each\_entry | 1          |
| fdinfo.c | \<linux/io\_uring.h> | task\_work\_pending     | 1          |
| fdinfo.c | \<linux/io\_uring.h> | mutex\_trylock          | 1          |
| fdinfo.c | \<linux/io\_uring.h> | mutex\_unlock           | 1          |
| fdinfo.c | \<linux/io\_uring.h> | spin\_lock              | 1          |
| fdinfo.c | \<linux/io\_uring.h> | spin\_unlock            | 1          |
| fdinfo.c | \<linux/io\_uring.h> | list\_for\_each\_entry  | 1          |

### fdinfo.h
| Source   | Library/Include      | Function Utilized       | Times Used |
| -------- | -------------------- | ----------------------- | ---------- |
| fdinfo.h | \<linux/io\_uring.h> | io\_uring\_show\_fdinfo | 1          |

### filetable.c
| Source      | Library              | Function Utilized                  | Times Used |
| ----------- | -------------------- | ---------------------------------- | ---------- |
| filetable.c | \<linux/kernel.h>    | -                                  | 0          |
| filetable.c | \<linux/errno.h>     | -                                  | 0          |
| filetable.c | \<linux/file.h>      | -                                  | 0          |
| filetable.c | \<linux/mm.h>        | -                                  | 0          |
| filetable.c | \<linux/slab.h>      | -                                  | 0          |
| filetable.c | \<linux/nospec.h>    | -                                  | 0          |
| filetable.c | \<linux/io\_uring.h> | io\_file\_bitmap\_get              | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_alloc\_file\_tables            | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_free\_file\_tables             | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_install\_fixed\_file           | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_is\_uring\_fops                | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_reset\_rsrc\_node              | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_file\_bitmap\_set              | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_fixed\_file\_set               | 1          |
| filetable.c | \<linux/io\_uring.h> | \_\_io\_fixed\_fd\_install         | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_ring\_submit\_lock             | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_ring\_submit\_unlock           | 1          |
| filetable.c | \<linux/io\_uring.h> | fput                               | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_file\_table\_set\_alloc\_range | 1          |
| filetable.c | \<linux/io\_uring.h> | copy\_from\_user                   | 1          |
| filetable.c | \<linux/io\_uring.h> | check\_add\_overflow               | 1          |
| filetable.c | \<linux/io\_uring.h> | io\_register\_file\_alloc\_range   | 1          |

### filetable.h
| Source      | Library/Include             | Function Utilized                  | Times Used |
| ----------- | --------------------------- | ---------------------------------- | ---------- |
| filetable.h | \<linux/file.h>             | -                                  | 0          |
| filetable.h | \<linux/io\_uring\_types.h> | -                                  | 0          |
| filetable.h | \<linux/io\_uring.h>        | io\_alloc\_file\_tables            | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_free\_file\_tables             | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_fixed\_fd\_install             | 1          |
| filetable.h | \<linux/io\_uring.h>        | \_\_io\_fixed\_fd\_install         | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_fixed\_fd\_remove              | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_register\_file\_alloc\_range   | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_file\_get\_flags               | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_file\_bitmap\_clear            | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_file\_bitmap\_set              | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_slot\_flags                    | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_slot\_file                     | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_fixed\_file\_set               | 1          |
| filetable.h | \<linux/io\_uring.h>        | io\_file\_table\_set\_alloc\_range | 1          |

### fs.c
| Source | Library/Include      | Function Utilized     | Times Used |
| ------ | -------------------- | --------------------- | ---------- |
| fs.c   | \<linux/kernel.h>    | -                     | 0          |
| fs.c   | \<linux/errno.h>     | -                     | 0          |
| fs.c   | \<linux/fs.h>        | -                     | 0          |
| fs.c   | \<linux/file.h>      | -                     | 0          |
| fs.c   | \<linux/mm.h>        | -                     | 0          |
| fs.c   | \<linux/slab.h>      | -                     | 0          |
| fs.c   | \<linux/namei.h>     | -                     | 0          |
| fs.c   | \<linux/io\_uring.h> | io\_renameat\_prep    | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_renameat          | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_renameat\_cleanup | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_unlinkat\_prep    | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_unlinkat          | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_unlinkat\_cleanup | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_mkdirat\_prep     | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_mkdirat           | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_mkdirat\_cleanup  | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_symlinkat\_prep   | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_symlinkat         | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_linkat\_prep      | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_linkat            | 1          |
| fs.c   | \<linux/io\_uring.h> | io\_link\_cleanup     | 1          |

### fs.h
| Source | Library/Include      | Function Utilized     | Times Used |
| ------ | -------------------- | --------------------- | ---------- |
| fs.h   | \<linux/io\_uring.h> | io\_renameat\_prep    | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_renameat          | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_renameat\_cleanup | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_unlinkat\_prep    | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_unlinkat          | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_unlinkat\_cleanup | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_mkdirat\_prep     | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_mkdirat           | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_mkdirat\_cleanup  | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_symlinkat\_prep   | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_symlinkat         | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_linkat\_prep      | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_linkat            | 1          |
| fs.h   | \<linux/io\_uring.h> | io\_link\_cleanup     | 1          |

### futex.c
| Source  | Library/Include        | Function Utilized            | Times Used |
| ------- | ---------------------- | ---------------------------- | ---------- |
| futex.c | \<linux/io\_uring.h>   | io\_futex\_prep              | 1          |
| futex.c | \<linux/io\_uring.h>   | io\_futexv\_prep             | 1          |
| futex.c | \<linux/io\_uring.h>   | io\_futex\_wait              | 1          |
| futex.c | \<linux/io\_uring.h>   | io\_futexv\_wait             | 1          |
| futex.c | \<linux/io\_uring.h>   | io\_futex\_wake              | 1          |
| futex.c | \<linux/io\_uring.h>   | io\_futex\_cancel            | 1          |
| futex.c | \<linux/io\_uring.h>   | io\_futex\_remove\_all       | 1          |
| futex.c | \<linux/io\_uring.h>   | io\_futex\_cache\_init       | 1          |
| futex.c | \<linux/io\_uring.h>   | io\_futex\_cache\_free       | 1          |
| futex.c | \<linux/futex/futex.h> | futex\_validate\_input       | 2          |
| futex.c | \<linux/futex/futex.h> | futex\_flags\_valid          | 1          |
| futex.c | \<linux/futex/futex.h> | futex\_wait\_multiple\_setup | 1          |
| futex.c | \<linux/futex/futex.h> | futex\_unqueue\_multiple     | 1          |
| futex.c | \<linux/futex/futex.h> | futex\_wake                  | 1          |
| futex.c | \<linux/futex/futex.h> | futex\_parse\_waitv          | 1          |
| futex.c | \<linux/futex/futex.h> | futex\_queue                 | 1          |
| futex.c | \<linux/futex/futex.h> | futex\_q\_init               | 1          |
| futex.c | \<linux/futex/futex.h> | futex\_unqueue               | 1          |

### futex.h
| Source  | Library/Include        | Function Utilized            | Times Used |
| ------- | ---------------------- | ---------------------------- | ---------- |
| futex.h | \<linux/io\_uring.h>   | io\_futex\_prep              | 1          |
| futex.h | \<linux/io\_uring.h>   | io\_futexv\_prep             | 1          |
| futex.h | \<linux/io\_uring.h>   | io\_futex\_wait              | 1          |
| futex.h | \<linux/io\_uring.h>   | io\_futexv\_wait             | 1          |
| futex.h | \<linux/io\_uring.h>   | io\_futex\_wake              | 1          |
| futex.h | \<linux/io\_uring.h>   | io\_futex\_cancel            | 1          |
| futex.h | \<linux/io\_uring.h>   | io\_futex\_remove\_all       | 1          |
| futex.h | \<linux/io\_uring.h>   | io\_futex\_cache\_init       | 1          |
| futex.h | \<linux/io\_uring.h>   | io\_futex\_cache\_free       | 1          |
| futex.h | \<linux/futex/futex.h> | futex\_validate\_input       | 2          |
| futex.h | \<linux/futex/futex.h> | futex\_flags\_valid          | 1          |
| futex.h | \<linux/futex/futex.h> | futex\_wait\_multiple\_setup | 1          |
| futex.h | \<linux/futex/futex.h> | futex\_unqueue\_multiple     | 1          |
| futex.h | \<linux/futex/futex.h> | futex\_wake                  | 1          |
| futex.h | \<linux/futex/futex.h> | futex\_parse\_waitv          | 1          |
| futex.h | \<linux/futex/futex.h> | futex\_queue                 | 1          |
| futex.h | \<linux/futex/futex.h> | futex\_q\_init               | 1          |
| futex.h | \<linux/futex/futex.h> | futex\_unqueue               | 1          |

### built-in.a
| Object File    | File Size (bytes) | Permissions |
| -------------- | ----------------- | ----------- |
| io\_uring.o    | 905000            | 644         |
| opdef.o        | 239480            | 644         |
| kbuf.o         | 273000            | 644         |
| rsrc.o         | 295480            | 644         |
| notif.o        | 349928            | 644         |
| tctx.o         | 246112            | 644         |
| filetable.o    | 231984            | 644         |
| rw\.o          | 317784            | 644         |
| net.o          | 784               | 644         |
| poll.o         | 440312            | 644         |
| eventfd.o      | 203616            | 644         |
| uring\_cmd.o   | 358912            | 644         |
| openclose.o    | 239112            | 644         |
| sqpoll.o       | 425248            | 644         |
| xattr.o        | 234368            | 644         |
| nop.o          | 221480            | 644         |
| fs.o           | 231272            | 644         |
| splice.o       | 228112            | 644         |
| sync.o         | 223360            | 644         |
| msg\_ring.o    | 245920            | 644         |
| advise.o       | 222056            | 644         |
| epoll.o        | 220288            | 644         |
| statx.o        | 222928            | 644         |
| timeout.o      | 279936            | 644         |
| fdinfo.o       | 235168            | 644         |
| cancel.o       | 239952            | 644         |
| waitid.o       | 251768            | 644         |
| register.o     | 459912            | 644         |
| truncate.o     | 277528            | 644         |
| memmap.o       | 211760            | 644         |
| alloc\_cache.o | 9960              | 644         |
| io-wq.o        | 347256            | 644         |
| futex.o        | 249976            | 644         |