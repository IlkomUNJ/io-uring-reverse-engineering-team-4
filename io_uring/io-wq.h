#ifndef INTERNAL_IO_WQ_H
#define INTERNAL_IO_WQ_H

#include <linux/refcount.h>
#include <linux/io_uring_types.h>

struct io_wq;

enum {
	IO_WQ_WORK_CANCEL	= 1,
	IO_WQ_WORK_HASHED	= 2,
	IO_WQ_WORK_UNBOUND	= 4,
	IO_WQ_WORK_CONCURRENT	= 16,

	IO_WQ_HASH_SHIFT	= 24,	/* upper 8 bits are used for hash key */
};

enum io_wq_cancel {
	IO_WQ_CANCEL_OK,	/* cancelled before started */
	IO_WQ_CANCEL_RUNNING,	/* found, running, and attempted cancelled */
	IO_WQ_CANCEL_NOTFOUND,	/* work not found */
};

typedef struct io_wq_work *(free_work_fn)(struct io_wq_work *);
typedef void (io_wq_work_fn)(struct io_wq_work *);

struct io_wq_hash {
	refcount_t refs;
	unsigned long map;
	struct wait_queue_head wait;
};

static inline void io_wq_put_hash(struct io_wq_hash *hash)
{
	if (refcount_dec_and_test(&hash->refs))
		kfree(hash);
}

struct io_wq_data {
	struct io_wq_hash *hash;
	struct task_struct *task;
	io_wq_work_fn *do_work;
	free_work_fn *free_work;
};

/**
 * This function initializes and returns a new I/O workqueue instance. The
 * bounded parameter determines whether the workqueue has a limit on the number
 * of concurrent tasks it can handle. The data parameter provides additional
 * context or configuration required for the workqueue's operation.
 */
struct io_wq *io_wq_create(unsigned bounded, struct io_wq_data *data);


/**
 * This function begins the process of shutting down the specified io_wq
 * (I/O work queue). It ensures that any ongoing operations are properly
 * handled and prepares the work queue for cleanup and resource deallocation.
 */
void io_wq_exit_start(struct io_wq *wq);

/**
 * This function is used to clean up and release the resources associated with
 * the specified io_wq (I/O workqueue) structure. It ensures that all pending
 * tasks or operations are properly handled before exiting. This is typically
 * called when the io_wq is no longer needed or during the shutdown process.
 */
void io_wq_put_and_exit(struct io_wq *wq);

/**
 * This function adds a work item to the specified io_wq work queue for
 * asynchronous processing. The work item will be processed by one of the
 * workers associated with the work queue.
 */
void io_wq_enqueue(struct io_wq *wq, struct io_wq_work *work);

/**
 * This function is responsible for hashing a work item into the appropriate
 * location within the io_uring workqueue system. The hashing mechanism may
 * use the provided value to determine the placement or categorization of the
 * work item.
 */
void io_wq_hash_work(struct io_wq_work *work, void *val);

/**
 * This function configures the CPU affinity for the given io_uring task
 * by applying the specified CPU mask. The CPU mask determines the set of
 * CPUs on which the task is allowed to run.
 */
int io_wq_cpu_affinity(struct io_uring_task *tctx, cpumask_var_t mask);
int io_wq_max_workers(struct io_wq *wq, int *new_count);
bool io_wq_worker_stopped(void);

static inline bool __io_wq_is_hashed(unsigned int work_flags)
{
	return work_flags & IO_WQ_WORK_HASHED;
}

static inline bool io_wq_is_hashed(struct io_wq_work *work)
{
	return __io_wq_is_hashed(atomic_read(&work->flags));
}

typedef bool (work_cancel_fn)(struct io_wq_work *, void *);

enum io_wq_cancel io_wq_cancel_cb(struct io_wq *wq, work_cancel_fn *cancel,
					void *data, bool cancel_all);

#if defined(CONFIG_IO_WQ)
extern void io_wq_worker_sleeping(struct task_struct *);
extern void io_wq_worker_running(struct task_struct *);
#else
static inline void io_wq_worker_sleeping(struct task_struct *tsk)
{
}
static inline void io_wq_worker_running(struct task_struct *tsk)
{
}
#endif

static inline bool io_wq_current_is_worker(void)
{
	return in_task() && (current->flags & PF_IO_WORKER) &&
		current->worker_private;
}
#endif
