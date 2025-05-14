// SPDX-License-Identifier: GPL-2.0
/*
 * Basic worker thread pool for io_uring
 *
 * Copyright (C) 2019 Jens Axboe
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/sched/signal.h>
#include <linux/percpu.h>
#include <linux/slab.h>
#include <linux/rculist_nulls.h>
#include <linux/cpu.h>
#include <linux/cpuset.h>
#include <linux/task_work.h>
#include <linux/audit.h>
#include <linux/mmu_context.h>
#include <uapi/linux/io_uring.h>

#include "io-wq.h"
#include "slist.h"
#include "io_uring.h"

#define WORKER_IDLE_TIMEOUT	(5 * HZ)
#define WORKER_INIT_LIMIT	3

enum {
	IO_WORKER_F_UP		= 0,	/* up and active */
	IO_WORKER_F_RUNNING	= 1,	/* account as running */
	IO_WORKER_F_FREE	= 2,	/* worker on free list */
};

enum {
	IO_WQ_BIT_EXIT		= 0,	/* wq exiting */
};

enum {
	IO_ACCT_STALLED_BIT	= 0,	/* stalled on hash */
};

/*
 * One for each thread in a wq pool
 */
struct io_worker {
	refcount_t ref;
	unsigned long flags;
	struct hlist_nulls_node nulls_node;
	struct list_head all_list;
	struct task_struct *task;
	struct io_wq *wq;
	struct io_wq_acct *acct;

	struct io_wq_work *cur_work;
	raw_spinlock_t lock;

	struct completion ref_done;

	unsigned long create_state;
	struct callback_head create_work;
	int init_retries;

	union {
		struct rcu_head rcu;
		struct delayed_work work;
	};
};

#if BITS_PER_LONG == 64
#define IO_WQ_HASH_ORDER	6
#else
#define IO_WQ_HASH_ORDER	5
#endif

#define IO_WQ_NR_HASH_BUCKETS	(1u << IO_WQ_HASH_ORDER)

struct io_wq_acct {
	/**
	 * Protects access to the worker lists.
	 */
	raw_spinlock_t workers_lock;

	unsigned nr_workers;
	unsigned max_workers;
	atomic_t nr_running;

	/**
	 * The list of free workers.  Protected by #workers_lock
	 * (write) and RCU (read).
	 */
	struct hlist_nulls_head free_list;

	/**
	 * The list of all workers.  Protected by #workers_lock
	 * (write) and RCU (read).
	 */
	struct list_head all_list;

	raw_spinlock_t lock;
	struct io_wq_work_list work_list;
	unsigned long flags;
};

enum {
	IO_WQ_ACCT_BOUND,
	IO_WQ_ACCT_UNBOUND,
	IO_WQ_ACCT_NR,
};

/*
 * Per io_wq state
  */
struct io_wq {
	unsigned long state;

	free_work_fn *free_work;
	io_wq_work_fn *do_work;

	struct io_wq_hash *hash;

	atomic_t worker_refs;
	struct completion worker_done;

	struct hlist_node cpuhp_node;

	struct task_struct *task;

	struct io_wq_acct acct[IO_WQ_ACCT_NR];

	struct wait_queue_entry wait;

	struct io_wq_work *hash_tail[IO_WQ_NR_HASH_BUCKETS];

	cpumask_var_t cpu_mask;
};

static enum cpuhp_state io_wq_online;

struct io_cb_cancel_data {
	work_cancel_fn *fn;
	void *data;
	int nr_running;
	int nr_pending;
	bool cancel_all;
};

static bool create_io_worker(struct io_wq *wq, struct io_wq_acct *acct);

/**
 * Decrements the count of running workers for the given worker.
 * This function is used to update the state of the worker and the associated
 * work queue when a worker stops running.
 */
static void io_wq_dec_running(struct io_worker *worker);

/**
 * Cancels pending work for a specific accounting context.
 * Returns true if any pending work was successfully canceled, false otherwise.
 */
static bool io_acct_cancel_pending_work(struct io_wq *wq,
					struct io_wq_acct *acct,
					struct io_cb_cancel_data *match);

/**
 * Callback function to handle worker creation.
 * This function is invoked as part of the worker creation process.
 */
static void create_worker_cb(struct callback_head *cb);

/**
 * ancels the creation of a task worker for the given work queue.
 * This function is used to abort the creation of a task worker if necessary.
 */
static void io_wq_cancel_tw_create(struct io_wq *wq);

 /**
 * Creates a new I/O worker for the given work queue.
 * Returns true if the worker was successfully created, false otherwise.
 */
static bool io_worker_get(struct io_worker *worker)
{
	return refcount_inc_not_zero(&worker->ref);
}

/**
 * This function decrements the reference count of the given io_worker
 * structure. If the reference count reaches zero, it signals that the
 * worker is no longer in use by completing the associated completion
 */
static void io_worker_release(struct io_worker *worker)
{
	if (refcount_dec_and_test(&worker->ref))
		complete(&worker->ref_done);
}

/**
 * This function returns a pointer to the appropriate accounting structure
 * within the given work queue based on the value of the `bound` parameter.
 * If `bound` is true, it retrieves the accounting structure for bound workers;
 * otherwise, it retrieves the accounting structure for unbound workers.
 */
static inline struct io_wq_acct *io_get_acct(struct io_wq *wq, bool bound)
{
	return &wq->acct[bound ? IO_WQ_ACCT_BOUND : IO_WQ_ACCT_UNBOUND];
}

/**
Retrieves the appropriate accounting structure (bound/unbound) for a work queue based on work flags
 */
static inline struct io_wq_acct *io_work_get_acct(struct io_wq *wq,
						  unsigned int work_flags)
{
	return io_get_acct(wq, !(work_flags & IO_WQ_WORK_UNBOUND));
}

/**
 * This function returns the accounting structure (`io_wq_acct`) associated
 * with the specified io_worker. The accounting structure is used to track
 * resource usage or other worker-specific metrics.
 */
static inline struct io_wq_acct *io_wq_get_acct(struct io_worker *worker)
{
	return worker->acct;
}

/**
 * This function atomically decrements the reference count of workers in the
 * given work queue. If the reference count reaches zero, it signals the
 * completion of worker operations by completing the worker_done completion
 * structure.
 */
static void io_worker_ref_put(struct io_wq *wq)
{
	if (atomic_dec_and_test(&wq->worker_refs))
		complete(&wq->worker_done);
}

/**
 * io_wq_worker_stopped - Checks if the current io_uring worker has stopped.
 *
 * This function determines whether the current io_uring worker thread has been
 * stopped by checking the worker's state. It first verifies that the current
 * thread is indeed an io_uring worker thread. If not, it triggers a warning
 * and assumes the worker is stopped. Otherwise, it checks the worker's state
 * for the `IO_WQ_BIT_EXIT` flag to determine if the worker has exited.
 *
 * Return:
 * true  - If the worker is stopped or the current thread is not a valid worker.
 * false - If the worker is still active.
 */
bool io_wq_worker_stopped(void)
{
	struct io_worker *worker = current->worker_private;

	if (WARN_ON_ONCE(!io_wq_current_is_worker()))
		return true;

	return test_bit(IO_WQ_BIT_EXIT, &worker->wq->state);
}

/**
 * io_worker_cancel_cb - Cancels a worker and updates its associated accounting.

 * This function is responsible for canceling a worker and performing the necessary
 * cleanup and accounting updates. It decrements the number of running workers
 * in the associated accounting structure, updates the worker count, and releases
 * the worker's reference. Additionally, it clears the worker's creation state
 * and releases the worker resources.
 */
static void io_worker_cancel_cb(struct io_worker *worker)
{
	struct io_wq_acct *acct = io_wq_get_acct(worker);
	struct io_wq *wq = worker->wq;

	atomic_dec(&acct->nr_running);
	raw_spin_lock(&acct->workers_lock);
	acct->nr_workers--;
	raw_spin_unlock(&acct->workers_lock);
	io_worker_ref_put(wq);
	clear_bit_unlock(0, &worker->create_state);
	io_worker_release(worker);
}

/**
 * io_task_worker_match - Matches a callback with a specific io_worker.

 * This function checks if the given callback corresponds to the creation
 * of an io_worker and whether the io_worker matches the provided data.
 * It first verifies that the callback function is `create_worker_cb`.
 * If the function matches, it retrieves the io_worker associated with
 * the callback and compares it with the provided data.
 */
static bool io_task_worker_match(struct callback_head *cb, void *data)
{
	struct io_worker *worker;

	if (cb->func != create_worker_cb)
		return false;
	worker = container_of(cb, struct io_worker, create_work);
	return worker == data;
}

/**
 * io_worker_exit - Cleans up and exits an io_worker.
 *
 * This function handles the cleanup and exit process for an io_worker, ensuring that all resources associated with the worker are
 * properly released and that the worker is safely removed from the system.
 */
static void io_worker_exit(struct io_worker *worker)
{
	struct io_wq *wq = worker->wq;
	struct io_wq_acct *acct = io_wq_get_acct(worker);

	while (1) {
		struct callback_head *cb = task_work_cancel_match(wq->task,
						io_task_worker_match, worker);

		if (!cb)
			break;
		io_worker_cancel_cb(worker);
	}

	io_worker_release(worker);
	wait_for_completion(&worker->ref_done);

	raw_spin_lock(&acct->workers_lock);
	if (test_bit(IO_WORKER_F_FREE, &worker->flags))
		hlist_nulls_del_rcu(&worker->nulls_node);
	list_del_rcu(&worker->all_list);
	raw_spin_unlock(&acct->workers_lock);
	io_wq_dec_running(worker);
	/*
	 * this worker is a goner, clear ->worker_private to avoid any
	 * inc/dec running calls that could happen as part of exit from
	 * touching 'worker'.
	 */
	current->worker_private = NULL;

	kfree_rcu(worker, rcu);
	io_worker_ref_put(wq);
	do_exit(0);
}

/**
 * __io_acct_run_queue - Check if the work queue can run

 * This function determines whether the work queue associated with the given
 * io_wq_acct structure is eligible to run
 */
static inline bool __io_acct_run_queue(struct io_wq_acct *acct)
{
	return !test_bit(IO_ACCT_STALLED_BIT, &acct->flags) &&
		!wq_list_empty(&acct->work_list);
}

/*
 * If there's work to do, returns true with acct->lock acquired. If not,
 * returns false with no lock held.
 */
static inline bool io_acct_run_queue(struct io_wq_acct *acct)
	__acquires(&acct->lock)
{
	raw_spin_lock(&acct->lock);
	if (__io_acct_run_queue(acct))
		return true;

	raw_spin_unlock(&acct->lock);
	return false;
}

/*
 * Check head of free list for an available worker. If one isn't available,
 * caller must create one.
 */
static bool io_acct_activate_free_worker(struct io_wq_acct *acct)
	__must_hold(RCU)
{
	struct hlist_nulls_node *n;
	struct io_worker *worker;

	/*
	 * Iterate free_list and see if we can find an idle worker to
	 * activate. If a given worker is on the free_list but in the process
	 * of exiting, keep trying.
	 */
	hlist_nulls_for_each_entry_rcu(worker, n, &acct->free_list, nulls_node) {
		if (!io_worker_get(worker))
			continue;
		/*
		 * If the worker is already running, it's either already
		 * starting work or finishing work. In either case, if it does
		 * to go sleep, we'll kick off a new task for this work anyway.
		 */
		wake_up_process(worker->task);
		io_worker_release(worker);
		return true;
	}

	return false;
}

/*
 * We need a worker. If we find a free one, we're good. If not, and we're
 * below the max number of workers, create one.
 */
static bool io_wq_create_worker(struct io_wq *wq, struct io_wq_acct *acct)
{
	/*
	 * Most likely an attempt to queue unbounded work on an io_wq that
	 * wasn't setup with any unbounded workers.
	 */
	if (unlikely(!acct->max_workers))
		pr_warn_once("io-wq is not configured for unbound workers");

	raw_spin_lock(&acct->workers_lock);
	if (acct->nr_workers >= acct->max_workers) {
		raw_spin_unlock(&acct->workers_lock);
		return true;
	}
	acct->nr_workers++;
	raw_spin_unlock(&acct->workers_lock);
	atomic_inc(&acct->nr_running);
	atomic_inc(&wq->worker_refs);
	return create_io_worker(wq, acct);
}

/**
 * This function retrieves the accounting structure associated with the given
 * worker and increments the atomic counter that tracks the number of running
 * workers. It is used to maintain an accurate count of active workers in the
 * io_uring workqueue system.
 */
static void io_wq_inc_running(struct io_worker *worker)
{
	struct io_wq_acct *acct = io_wq_get_acct(worker);

	atomic_inc(&acct->nr_running);
}

/**
 * The function ensures proper synchronization using a spinlock to protect the
 * worker accounting structure. It also clears the worker's creation state and
 * releases the worker resources when done.
 */
static void create_worker_cb(struct callback_head *cb)
{
	struct io_worker *worker;
	struct io_wq *wq;

	struct io_wq_acct *acct;
	bool do_create = false;

	worker = container_of(cb, struct io_worker, create_work);
	wq = worker->wq;
	acct = worker->acct;
	raw_spin_lock(&acct->workers_lock);

	if (acct->nr_workers < acct->max_workers) {
		acct->nr_workers++;
		do_create = true;
	}
	raw_spin_unlock(&acct->workers_lock);
	if (do_create) {
		create_io_worker(wq, acct);
	} else {
		atomic_dec(&acct->nr_running);
		io_worker_ref_put(wq);
	}
	clear_bit_unlock(0, &worker->create_state);
	io_worker_release(worker);
}

/**
 * The function performs the following steps:
 * 1. Checks if the workqueue is in the process of exiting. If so, it skips
 *    the creation process.
 * 2. Attempts to acquire a reference to the worker. If unsuccessful, it
 *    aborts the creation process.
 * 3. Ensures that the worker's creation state is not already set or locked.
 * 4. Increments the worker reference count and initializes the task work
 *    structure with the provided function.
 * 5. Adds the task work to the workqueue's task. If successful, it verifies
 *    the exit state again and cancels the creation task if necessary.
 * 6. If any step fails, it releases resources and decrements the running
 *    worker count in the accounting structure.
 *
 * Note: This function uses atomic operations and bit manipulation to ensure
 *       thread safety during worker creation.
 */
static bool io_queue_worker_create(struct io_worker *worker,
				   struct io_wq_acct *acct,
				   task_work_func_t func)
{
	struct io_wq *wq = worker->wq;

	/* raced with exit, just ignore create call */
	if (test_bit(IO_WQ_BIT_EXIT, &wq->state))
		goto fail;
	if (!io_worker_get(worker))
		goto fail;
	/*
	 * create_state manages ownership of create_work/index. We should
	 * only need one entry per worker, as the worker going to sleep
	 * will trigger the condition, and waking will clear it once it
	 * runs the task_work.
	 */
	if (test_bit(0, &worker->create_state) ||
	    test_and_set_bit_lock(0, &worker->create_state))
		goto fail_release;

	atomic_inc(&wq->worker_refs);
	init_task_work(&worker->create_work, func);
	if (!task_work_add(wq->task, &worker->create_work, TWA_SIGNAL)) {
		/*
		 * EXIT may have been set after checking it above, check after
		 * adding the task_work and remove any creation item if it is
		 * now set. wq exit does that too, but we can have added this
		 * work item after we canceled in io_wq_exit_workers().
		 */
		if (test_bit(IO_WQ_BIT_EXIT, &wq->state))
			io_wq_cancel_tw_create(wq);
		io_worker_ref_put(wq);
		return true;
	}
	io_worker_ref_put(wq);
	clear_bit_unlock(0, &worker->create_state);
fail_release:
	io_worker_release(worker);
fail:
	atomic_dec(&acct->nr_running);
	io_worker_ref_put(wq);
	return false;
}

/**
 * This function is responsible for decrementing the running worker count for the
 * associated accounting structure (`io_wq_acct`) of the given worker. It ensures
 * that the worker is in the "up" state before proceeding. If the running count
 * reaches zero and the run queue is empty, it attempts to create a new worker
 * using the provided callback function.
 */
static void io_wq_dec_running(struct io_worker *worker)
{
	struct io_wq_acct *acct = io_wq_get_acct(worker);
	struct io_wq *wq = worker->wq;

	if (!test_bit(IO_WORKER_F_UP, &worker->flags))
		return;

	if (!atomic_dec_and_test(&acct->nr_running))
		return;
	if (!io_acct_run_queue(acct))
		return;

	raw_spin_unlock(&acct->lock);
	atomic_inc(&acct->nr_running);
	atomic_inc(&wq->worker_refs);
	io_queue_worker_create(worker, acct, create_worker_cb);
}

/*
 * Worker will start processing some work. Move it to the busy list, if
 * it's currently on the freelist
 */
static void __io_worker_busy(struct io_wq_acct *acct, struct io_worker *worker)
{
	if (test_bit(IO_WORKER_F_FREE, &worker->flags)) {
		clear_bit(IO_WORKER_F_FREE, &worker->flags);
		raw_spin_lock(&acct->workers_lock);
		hlist_nulls_del_init_rcu(&worker->nulls_node);
		raw_spin_unlock(&acct->workers_lock);
	}
}

/*
 * No work, worker going to sleep. Move to freelist.
 */
static void __io_worker_idle(struct io_wq_acct *acct, struct io_worker *worker)
	__must_hold(acct->workers_lock)
{
	if (!test_bit(IO_WORKER_F_FREE, &worker->flags)) {
		set_bit(IO_WORKER_F_FREE, &worker->flags);
		hlist_nulls_add_head_rcu(&worker->nulls_node, &acct->free_list);
	}
}

/**
 * This function shifts the provided work flags to the right by
 * the value of IO_WQ_HASH_SHIFT to extract the hash value. The
 * hash value is used to categorize or identify work items in
 * the io_uring work queue.
 */
static inline unsigned int __io_get_work_hash(unsigned int work_flags)
{
	return work_flags >> IO_WQ_HASH_SHIFT;
}

/**
 * This function calculates a hash value for the provided work item by
 * reading its flags atomically and passing them to the internal hashing
 * function __io_get_work_hash. The resulting hash value can be used for
 * work item identification or categorization.
 */
static inline unsigned int io_get_work_hash(struct io_wq_work *work)
{
	return __io_get_work_hash(atomic_read(&work->flags));
}

/**
 * This function checks if the wait queue associated with the given hash
 * is empty. If it is, the current task is added to the wait queue and
 * the function tests whether the bit corresponding to the hash is set
 * in the hash map. If the bit is not set, the current task is marked
 * as running, removed from the wait queue, and the function returns true.
 * Otherwise, the function returns false.
 */
static bool io_wait_on_hash(struct io_wq *wq, unsigned int hash)
{
	bool ret = false;

	spin_lock_irq(&wq->hash->wait.lock);
	if (list_empty(&wq->wait.entry)) {
		__add_wait_queue(&wq->hash->wait, &wq->wait);
		if (!test_bit(hash, &wq->hash->map)) {
			__set_current_state(TASK_RUNNING);
			list_del_init(&wq->wait.entry);
			ret = true;
		}
	}
	spin_unlock_irq(&wq->hash->wait.lock);
	return ret;
}

/**
 * This function retrieves the next available work item from the work queue
 * associated with the given account. It prioritizes non-hashed work items
 * that can run anytime. If no non-hashed work items are available, it attempts
 * to retrieve a hashed work item that is not currently running. The function
 * also handles cases where the work queue is stalled due to hashed work items
 * being blocked.
 */
static struct io_wq_work *io_get_next_work(struct io_wq_acct *acct,
					   struct io_wq *wq)
	__must_hold(acct->lock)
{
	struct io_wq_work_node *node, *prev;
	struct io_wq_work *work, *tail;
	unsigned int stall_hash = -1U;

	wq_list_for_each(node, prev, &acct->work_list) {
		unsigned int work_flags;
		unsigned int hash;

		work = container_of(node, struct io_wq_work, list);

		/* not hashed, can run anytime */
		work_flags = atomic_read(&work->flags);
		if (!__io_wq_is_hashed(work_flags)) {
			wq_list_del(&acct->work_list, node, prev);
			return work;
		}

		hash = __io_get_work_hash(work_flags);
		/* all items with this hash lie in [work, tail] */
		tail = wq->hash_tail[hash];

		/* hashed, can run if not already running */
		if (!test_and_set_bit(hash, &wq->hash->map)) {
			wq->hash_tail[hash] = NULL;
			wq_list_cut(&acct->work_list, &tail->list, prev);
			return work;
		}
		if (stall_hash == -1U)
			stall_hash = hash;
		/* fast forward to a next hash, for-each will fix up @prev */
		node = &tail->list;
	}

	if (stall_hash != -1U) {
		bool unstalled;

		/*
		 * Set this before dropping the lock to avoid racing with new
		 * work being added and clearing the stalled bit.
		 */
		set_bit(IO_ACCT_STALLED_BIT, &acct->flags);
		raw_spin_unlock(&acct->lock);
		unstalled = io_wait_on_hash(wq, stall_hash);
		raw_spin_lock(&acct->lock);
		if (unstalled) {
			clear_bit(IO_ACCT_STALLED_BIT, &acct->flags);
			if (wq_has_sleeper(&wq->hash->wait))
				wake_up(&wq->hash->wait);
		}
	}

	return NULL;
}

/**
 * This function assigns a work item to the specified worker. If a work item
 * is provided, it first executes any pending task work and yields the CPU
 * if necessary to allow other tasks to run. The assignment is performed
 * while holding a spin lock to ensure thread safety.
 */
static void io_assign_current_work(struct io_worker *worker,
				   struct io_wq_work *work)
{
	if (work) {
		io_run_task_work();
		cond_resched();
	}

	raw_spin_lock(&worker->lock);
	worker->cur_work = work;
	raw_spin_unlock(&worker->lock);
}

/*
 * Called with acct->lock held, drops it before returning
 */
static void io_worker_handle_work(struct io_wq_acct *acct,
				  struct io_worker *worker)
	__releases(&acct->lock)
{
	struct io_wq *wq = worker->wq;
	bool do_kill = test_bit(IO_WQ_BIT_EXIT, &wq->state);

	do {
		struct io_wq_work *work;

		/*
		 * If we got some work, mark us as busy. If we didn't, but
		 * the list isn't empty, it means we stalled on hashed work.
		 * Mark us stalled so we don't keep looking for work when we
		 * can't make progress, any work completion or insertion will
		 * clear the stalled flag.
		 */
		work = io_get_next_work(acct, wq);
		if (work) {
			/*
			 * Make sure cancelation can find this, even before
			 * it becomes the active work. That avoids a window
			 * where the work has been removed from our general
			 * work list, but isn't yet discoverable as the
			 * current work item for this worker.
			 */
			raw_spin_lock(&worker->lock);
			worker->cur_work = work;
			raw_spin_unlock(&worker->lock);
		}

		raw_spin_unlock(&acct->lock);

		if (!work)
			break;

		__io_worker_busy(acct, worker);

		io_assign_current_work(worker, work);
		__set_current_state(TASK_RUNNING);

		/* handle a whole dependent link */
		do {
			struct io_wq_work *next_hashed, *linked;
			unsigned int work_flags = atomic_read(&work->flags);
			unsigned int hash = __io_wq_is_hashed(work_flags)
				? __io_get_work_hash(work_flags)
				: -1U;

			next_hashed = wq_next_work(work);

			if (do_kill &&
			    (work_flags & IO_WQ_WORK_UNBOUND))
				atomic_or(IO_WQ_WORK_CANCEL, &work->flags);
			wq->do_work(work);
			io_assign_current_work(worker, NULL);

			linked = wq->free_work(work);
			work = next_hashed;
			if (!work && linked && !io_wq_is_hashed(linked)) {
				work = linked;
				linked = NULL;
			}
			io_assign_current_work(worker, work);
			if (linked)
				io_wq_enqueue(wq, linked);

			if (hash != -1U && !next_hashed) {
				/* serialize hash clear with wake_up() */
				spin_lock_irq(&wq->hash->wait.lock);
				clear_bit(hash, &wq->hash->map);
				clear_bit(IO_ACCT_STALLED_BIT, &acct->flags);
				spin_unlock_irq(&wq->hash->wait.lock);
				if (wq_has_sleeper(&wq->hash->wait))
					wake_up(&wq->hash->wait);
			}
		} while (work);

		if (!__io_acct_run_queue(acct))
			break;
		raw_spin_lock(&acct->lock);
	} while (1);
}

/**
 * This function is the main loop for an io_uring worker thread. It handles
 * processing work items from the workqueue, managing worker state, and
 * handling idle timeouts or exit conditions.
 *
 * Return: Always returns 0 upon thread exit.
 */
static int io_wq_worker(void *data)
{
	struct io_worker *worker = data;
	struct io_wq_acct *acct = io_wq_get_acct(worker);
	struct io_wq *wq = worker->wq;
	bool exit_mask = false, last_timeout = false;
	char buf[TASK_COMM_LEN] = {};

	set_mask_bits(&worker->flags, 0,
		      BIT(IO_WORKER_F_UP) | BIT(IO_WORKER_F_RUNNING));

	snprintf(buf, sizeof(buf), "iou-wrk-%d", wq->task->pid);
	set_task_comm(current, buf);

	while (!test_bit(IO_WQ_BIT_EXIT, &wq->state)) {
		long ret;

		set_current_state(TASK_INTERRUPTIBLE);

		/*
		 * If we have work to do, io_acct_run_queue() returns with
		 * the acct->lock held. If not, it will drop it.
		 */
		while (io_acct_run_queue(acct))
			io_worker_handle_work(acct, worker);

		raw_spin_lock(&acct->workers_lock);
		/*
		 * Last sleep timed out. Exit if we're not the last worker,
		 * or if someone modified our affinity.
		 */
		if (last_timeout && (exit_mask || acct->nr_workers > 1)) {
			acct->nr_workers--;
			raw_spin_unlock(&acct->workers_lock);
			__set_current_state(TASK_RUNNING);
			break;
		}
		last_timeout = false;
		__io_worker_idle(acct, worker);
		raw_spin_unlock(&acct->workers_lock);
		if (io_run_task_work())
			continue;
		ret = schedule_timeout(WORKER_IDLE_TIMEOUT);
		if (signal_pending(current)) {
			struct ksignal ksig;

			if (!get_signal(&ksig))
				continue;
			break;
		}
		if (!ret) {
			last_timeout = true;
			exit_mask = !cpumask_test_cpu(raw_smp_processor_id(),
							wq->cpu_mask);
		}
	}

	if (test_bit(IO_WQ_BIT_EXIT, &wq->state) && io_acct_run_queue(acct))
		io_worker_handle_work(acct, worker);

	io_worker_exit(worker);
	return 0;
}

/*
 * Called when a worker is scheduled in. Mark us as currently running.
 */
void io_wq_worker_running(struct task_struct *tsk)
{
	struct io_worker *worker = tsk->worker_private;

	if (!worker)
		return;
	if (!test_bit(IO_WORKER_F_UP, &worker->flags))
		return;
	if (test_bit(IO_WORKER_F_RUNNING, &worker->flags))
		return;
	set_bit(IO_WORKER_F_RUNNING, &worker->flags);
	io_wq_inc_running(worker);
}

/*
 * Called when worker is going to sleep. If there are no workers currently
 * running and we have work pending, wake up a free one or create a new one.
 */
void io_wq_worker_sleeping(struct task_struct *tsk)
{
	struct io_worker *worker = tsk->worker_private;

	if (!worker)
		return;
	if (!test_bit(IO_WORKER_F_UP, &worker->flags))
		return;
	if (!test_bit(IO_WORKER_F_RUNNING, &worker->flags))
		return;

	clear_bit(IO_WORKER_F_RUNNING, &worker->flags);
	io_wq_dec_running(worker);
}

/**
 * This function initializes a new io_uring worker by associating the worker
 * with the given task, setting its CPU affinity, and adding it to the
 * appropriate worker lists. The worker is marked as free and ready for use.
 * Finally, the function wakes up the new task to start execution.
 */
static void io_init_new_worker(struct io_wq *wq, struct io_wq_acct *acct, struct io_worker *worker,
			       struct task_struct *tsk)
{
	tsk->worker_private = worker;
	worker->task = tsk;
	set_cpus_allowed_ptr(tsk, wq->cpu_mask);

	raw_spin_lock(&acct->workers_lock);
	hlist_nulls_add_head_rcu(&worker->nulls_node, &acct->free_list);
	list_add_tail_rcu(&worker->all_list, &acct->all_list);
	set_bit(IO_WORKER_F_FREE, &worker->flags);
	raw_spin_unlock(&acct->workers_lock);
	wake_up_new_task(tsk);
}

/**
 * This function is used as a match-all predicate, always returning true
 * regardless of the input parameters. It can be used in scenarios where
 * all work items need to be matched or processed without any filtering.
 *
 * Return: Always returns true.
 */
static bool io_wq_work_match_all(struct io_wq_work *work, void *data)
{
	return true;
}

/**
 * This function evaluates whether a worker thread should retry an operation
 * based on the provided error code and the state of the worker. It prevents
 * perpetual retries in certain scenarios, such as when the task or its group
 * is exiting or when the retry limit has been reached.
 */
static inline bool io_should_retry_thread(struct io_worker *worker, long err)
{
	/*
	 * Prevent perpetual task_work retry, if the task (or its group) is
	 * exiting.
	 */
	if (fatal_signal_pending(current))
		return false;
	if (worker->init_retries++ >= WORKER_INIT_LIMIT)
		return false;

	switch (err) {
	case -EAGAIN:
	case -ERESTARTSYS:
	case -ERESTARTNOINTR:
	case -ERESTARTNOHAND:
		return true;
	default:
		return false;
	}
}

/**
 * This function schedules a delayed retry for creating a worker in case
 * the initial attempt fails. The failure might be due to temporary conditions
 * in the forking task, such as outstanding signals. By scheduling a retry
 * with a delay, it allows the task some time to clear such conditions.
 */
static void queue_create_worker_retry(struct io_worker *worker)
{
	/*
	 * We only bother retrying because there's a chance that the
	 * failure to create a worker is due to some temporary condition
	 * in the forking task (e.g. outstanding signal); give the task
	 * some time to clear that condition.
	 */
	schedule_delayed_work(&worker->work,
			      msecs_to_jiffies(worker->init_retries * 5));
}

/**
 * This function is invoked as part of the worker creation process. It attempts
 * to create a new I/O thread for the worker. If the thread creation is
 * successful, the worker is initialized and released. If the thread creation
 * fails and retrying is not feasible, it decrements the running worker count
 * and handles cleanup, including canceling pending work if necessary. If
 * re-creation is required, it queues a retry for worker creation.
 */
static void create_worker_cont(struct callback_head *cb)
{
	struct io_worker *worker;
	struct task_struct *tsk;
	struct io_wq *wq;
	struct io_wq_acct *acct;

	worker = container_of(cb, struct io_worker, create_work);
	clear_bit_unlock(0, &worker->create_state);
	wq = worker->wq;
	acct = io_wq_get_acct(worker);
	tsk = create_io_thread(io_wq_worker, worker, NUMA_NO_NODE);
	if (!IS_ERR(tsk)) {
		io_init_new_worker(wq, acct, worker, tsk);
		io_worker_release(worker);
		return;
	} else if (!io_should_retry_thread(worker, PTR_ERR(tsk))) {
		atomic_dec(&acct->nr_running);
		raw_spin_lock(&acct->workers_lock);
		acct->nr_workers--;
		if (!acct->nr_workers) {
			struct io_cb_cancel_data match = {
				.fn		= io_wq_work_match_all,
				.cancel_all	= true,
			};

			raw_spin_unlock(&acct->workers_lock);
			while (io_acct_cancel_pending_work(wq, acct, &match))
				;
		} else {
			raw_spin_unlock(&acct->workers_lock);
		}
		io_worker_ref_put(wq);
		kfree(worker);
		return;
	}

	/* re-create attempts grab a new worker ref, drop the existing one */
	io_worker_release(worker);
	queue_create_worker_retry(worker);
}

/**
 * This function initializes an io_worker structure by extracting it from the
 * provided work_struct. It then retrieves the associated io_wq_acct structure
 * for the worker. If the worker creation process fails (via io_queue_worker_create),
 * the allocated worker memory is freed to prevent memory leaks.
 */
static void io_workqueue_create(struct work_struct *work)
{
	struct io_worker *worker = container_of(work, struct io_worker,
						work.work);
	struct io_wq_acct *acct = io_wq_get_acct(worker);

	if (!io_queue_worker_create(worker, acct, create_worker_cont))
		kfree(worker);
}

/**
 * This function is responsible for creating and initializing an I/O worker
 * thread in the context of an io_uring workqueue. It performs the following:
 *
 * 1. Allocates memory for a new `io_worker` structure.
 * 2. Initializes the worker's fields, including reference counting, locks,
 *    and completion structures.
 * 3. Creates a new kernel thread for the worker using `create_io_thread`.
 * 4. Handles failure scenarios, including retrying thread creation or
 *    cleaning up resources if the worker cannot be created.
 *
 * Return:
 * - `true` if the worker thread is successfully created and initialized.
 * - `false` if the worker thread creation fails and cannot be retried.
 *
 * Failure Handling:
 * - If memory allocation for the worker fails, the function decrements the
 *   running worker count, updates the worker accounting, and releases the
 *   workqueue reference before returning `false`.
 * - If thread creation fails and retrying is not possible, the allocated
 *   worker is freed, and the function returns `false`.
 */
static bool create_io_worker(struct io_wq *wq, struct io_wq_acct *acct)
{
	struct io_worker *worker;
	struct task_struct *tsk;

	__set_current_state(TASK_RUNNING);

	worker = kzalloc(sizeof(*worker), GFP_KERNEL);
	if (!worker) {
fail:
		atomic_dec(&acct->nr_running);
		raw_spin_lock(&acct->workers_lock);
		acct->nr_workers--;
		raw_spin_unlock(&acct->workers_lock);
		io_worker_ref_put(wq);
		return false;
	}

	refcount_set(&worker->ref, 1);
	worker->wq = wq;
	worker->acct = acct;
	raw_spin_lock_init(&worker->lock);
	init_completion(&worker->ref_done);

	tsk = create_io_thread(io_wq_worker, worker, NUMA_NO_NODE);
	if (!IS_ERR(tsk)) {
		io_init_new_worker(wq, acct, worker, tsk);
	} else if (!io_should_retry_thread(worker, PTR_ERR(tsk))) {
		kfree(worker);
		goto fail;
	} else {
		INIT_DELAYED_WORK(&worker->work, io_workqueue_create);
		queue_create_worker_retry(worker);
	}

	return true;
}

/*
 * Iterate the passed in list and call the specific function for each
 * worker that isn't exiting
 */
static bool io_acct_for_each_worker(struct io_wq_acct *acct,
				    bool (*func)(struct io_worker *, void *),
				    void *data)
{
	struct io_worker *worker;
	bool ret = false;

	list_for_each_entry_rcu(worker, &acct->all_list, all_list) {
		if (io_worker_get(worker)) {
			/* no task if node is/was offline */
			if (worker->task)
				ret = func(worker, data);
			io_worker_release(worker);
			if (ret)
				break;
		}
	}

	return ret;
}

/**
 * This function iterates over all workers in the io_wq structure, calling
 * the provided callback function for each worker. If the callback function
 * returns false for any worker, the iteration stops, and the function
 * returns false. If all workers are successfully processed, the function
 * returns true.
 */
static bool io_wq_for_each_worker(struct io_wq *wq,
				  bool (*func)(struct io_worker *, void *),
				  void *data)
{
	for (int i = 0; i < IO_WQ_ACCT_NR; i++) {
		if (!io_acct_for_each_worker(&wq->acct[i], func, data))
			return false;
	}

	return true;
}

/**
 * This function is responsible for waking up the task associated with the
 * given io_worker. It sets a notification signal for the task and then
 * calls wake_up_process() to ensure the task is scheduled to run.
 */
static bool io_wq_worker_wake(struct io_worker *worker, void *data)
{
	__set_notify_signal(worker->task);
	wake_up_process(worker->task);
	return false;
}

/**
 * This function iteratively cancels and processes a chain of work items
 * in the provided work queue. It sets the IO_WQ_WORK_CANCEL flag on each
 * work item, invokes the work queue's do_work() function to process the
 * work, and then frees the work item using the work queue's free_work()
 * function. The process continues until there are no more work items in
 * the chain.
 */
static void io_run_cancel(struct io_wq_work *work, struct io_wq *wq)
{
	do {
		atomic_or(IO_WQ_WORK_CANCEL, &work->flags);
		wq->do_work(work);
		work = wq->free_work(work);
	} while (work);
}

/**
 * This function inserts a work item into the appropriate position in the work queue.
 * If the work item is not hashed (determined by the work_flags), it is appended to
 * the end of the work list. If the work item is hashed, it is inserted after the
 * last work item in the same hash bucket, maintaining the hash-based ordering.
 */
static void io_wq_insert_work(struct io_wq *wq, struct io_wq_acct *acct,
			      struct io_wq_work *work, unsigned int work_flags)
{
	unsigned int hash;
	struct io_wq_work *tail;

	if (!__io_wq_is_hashed(work_flags)) {
append:
		wq_list_add_tail(&work->list, &acct->work_list);
		return;
	}

	hash = __io_get_work_hash(work_flags);
	tail = wq->hash_tail[hash];
	wq->hash_tail[hash] = work;
	if (!tail)
		goto append;

	wq_list_add_after(&work->list, &tail->list, &acct->work_list);
}

/**
 * This function checks if the given work item matches the provided data
 * by comparing their memory addresses. It returns true if they are the
 * same, otherwise false.
 */
static bool io_wq_work_match_item(struct io_wq_work *work, void *data)
{
	return work == data;
}

/**
 * This function enqueues a work item into the specified io-wq workqueue. It performs
 * the following steps:
 * 1. Checks if the workqueue is in an exiting state or if the work item is marked
 *    for cancellation. If so, the work item is canceled and the function returns.
 * 2. Acquires a lock on the accounting structure associated with the work item and
 *    inserts the work item into the workqueue.
 * 3. Clears the stalled bit in the accounting structure's flags and releases the lock.
 * 4. Activates a free worker if necessary, or creates a new worker if no workers are
 *    running and the work item allows concurrent execution.
 * 5. Handles the failure to create the first worker by canceling all pending work
 *    associated with the accounting structure.
 */
void io_wq_enqueue(struct io_wq *wq, struct io_wq_work *work)
{
	unsigned int work_flags = atomic_read(&work->flags);
	struct io_wq_acct *acct = io_work_get_acct(wq, work_flags);
	struct io_cb_cancel_data match = {
		.fn		= io_wq_work_match_item,
		.data		= work,
		.cancel_all	= false,
	};
	bool do_create;

	/*
	 * If io-wq is exiting for this task, or if the request has explicitly
	 * been marked as one that should not get executed, cancel it here.
	 */
	if (test_bit(IO_WQ_BIT_EXIT, &wq->state) ||
	    (work_flags & IO_WQ_WORK_CANCEL)) {
		io_run_cancel(work, wq);
		return;
	}

	raw_spin_lock(&acct->lock);
	io_wq_insert_work(wq, acct, work, work_flags);
	clear_bit(IO_ACCT_STALLED_BIT, &acct->flags);
	raw_spin_unlock(&acct->lock);

	rcu_read_lock();
	do_create = !io_acct_activate_free_worker(acct);
	rcu_read_unlock();

	if (do_create && ((work_flags & IO_WQ_WORK_CONCURRENT) ||
	    !atomic_read(&acct->nr_running))) {
		bool did_create;

		did_create = io_wq_create_worker(wq, acct);
		if (likely(did_create))
			return;

		raw_spin_lock(&acct->workers_lock);
		if (acct->nr_workers) {
			raw_spin_unlock(&acct->workers_lock);
			return;
		}
		raw_spin_unlock(&acct->workers_lock);

		/* fatal condition, failed to create the first worker */
		io_acct_cancel_pending_work(wq, acct, &match);
	}
}

/*
 * Work items that hash to the same value will not be done in parallel.
 * Used to limit concurrent writes, generally hashed by inode.
 */
void io_wq_hash_work(struct io_wq_work *work, void *val)
{
	unsigned int bit;

	bit = hash_ptr(val, IO_WQ_HASH_ORDER);
	atomic_or(IO_WQ_WORK_HASHED | (bit << IO_WQ_HASH_SHIFT), &work->flags);
}

/**
 * This function checks if the given work item matches the criteria specified
 * in the match structure. If it matches, the work is marked as canceled by
 * setting the IO_WQ_WORK_CANCEL flag in its flags field, and a notification
 * signal is sent to the worker's task. The function returns true if the work
 * was successfully canceled, and false otherwise.
 */
static bool __io_wq_worker_cancel(struct io_worker *worker,
				  struct io_cb_cancel_data *match,
				  struct io_wq_work *work)
{
	if (work && match->fn(work, match->data)) {
		atomic_or(IO_WQ_WORK_CANCEL, &work->flags);
		__set_notify_signal(worker->task);
		return true;
	}

	return false;
}

/**
 * This function attempts to cancel the current work being processed by the
 * specified worker. It locks the worker's spinlock to ensure that the
 * worker's current work (`cur_work`) does not go out of scope while being
 * accessed. If the cancellation is successful, the number of running tasks
 * (`nr_running`) in the cancellation data is incremented.
 */
static bool io_wq_worker_cancel(struct io_worker *worker, void *data)
{
	struct io_cb_cancel_data *match = data;

	/*
	 * Hold the lock to avoid ->cur_work going out of scope, caller
	 * may dereference the passed in work.
	 */
	raw_spin_lock(&worker->lock);
	if (__io_wq_worker_cancel(worker, match, worker->cur_work))
		match->nr_running++;
	raw_spin_unlock(&worker->lock);

	return match->nr_running && !match->cancel_all;
}

/**
 * This function removes a pending work item from the work queue's accounting
 * list and updates the hash tail pointer if the work item is hashed. If the
 * work item is the last one in its hash bucket, the hash tail pointer is
 * updated to point to the previous work item in the bucket or set to NULL
 * if no such item exists.
 */
static inline void io_wq_remove_pending(struct io_wq *wq,
					struct io_wq_acct *acct,
					 struct io_wq_work *work,
					 struct io_wq_work_node *prev)
{
	unsigned int hash = io_get_work_hash(work);
	struct io_wq_work *prev_work = NULL;

	if (io_wq_is_hashed(work) && work == wq->hash_tail[hash]) {
		if (prev)
			prev_work = container_of(prev, struct io_wq_work, list);
		if (prev_work && io_get_work_hash(prev_work) == hash)
			wq->hash_tail[hash] = prev_work;
		else
			wq->hash_tail[hash] = NULL;
	}
	wq_list_del(&acct->work_list, &work->list, prev);
}

/**
 * This function iterates through the list of pending work in the specified
 * io_wq account and attempts to cancel work items that match the criteria
 * defined by the matching function in the @match parameter. If a matching
 * work item is found, it is removed from the pending list, the cancel
 * callback is executed, and the number of pending cancellations is incremented.
 *
 * Return:
 * - true if a matching work item was found and canceled.
 * - false if no matching work item was found.
 */
static bool io_acct_cancel_pending_work(struct io_wq *wq,
					struct io_wq_acct *acct,
					struct io_cb_cancel_data *match)
{
	struct io_wq_work_node *node, *prev;
	struct io_wq_work *work;

	raw_spin_lock(&acct->lock);
	wq_list_for_each(node, prev, &acct->work_list) {
		work = container_of(node, struct io_wq_work, list);
		if (!match->fn(work, match->data))
			continue;
		io_wq_remove_pending(wq, acct, work, prev);
		raw_spin_unlock(&acct->lock);
		io_run_cancel(work, wq);
		match->nr_pending++;
		/* not safe to continue after unlock */
		return true;
	}
	raw_spin_unlock(&acct->lock);

	return false;
}

/**
 * This function iterates through the accounting structures of the workqueue
 * and attempts to cancel pending work that matches the criteria specified in
 * the @match parameter. If the `cancel_all` flag in @match is set, the function
 * retries the cancellation process until all matching work items are canceled.
 */
static void io_wq_cancel_pending_work(struct io_wq *wq,
				      struct io_cb_cancel_data *match)
{
	int i;
retry:
	for (i = 0; i < IO_WQ_ACCT_NR; i++) {
		struct io_wq_acct *acct = io_get_acct(wq, i == 0);

		if (io_acct_cancel_pending_work(wq, acct, match)) {
			if (match->cancel_all)
				goto retry;
			break;
		}
	}
}

/**
 * This function cancels all running work associated with the given account
 * that matches the criteria specified in the @match parameter. It acquires
 * a spin lock on the workers_lock to ensure thread safety while iterating
 * through the workers and invoking the cancellation logic.
 */
static void io_acct_cancel_running_work(struct io_wq_acct *acct,
					struct io_cb_cancel_data *match)
{
	raw_spin_lock(&acct->workers_lock);
	io_acct_for_each_worker(acct, io_wq_worker_cancel, match);
	raw_spin_unlock(&acct->workers_lock);
}

/**
 * This function iterates over all accounting types in the io_wq structure
 * and cancels any running work that matches the criteria specified in the
 * provided io_cb_cancel_data structure. The function ensures thread safety
 * by using RCU (Read-Copy-Update) locking during the operation.
 */
static void io_wq_cancel_running_work(struct io_wq *wq,
				       struct io_cb_cancel_data *match)
{
	rcu_read_lock();

	for (int i = 0; i < IO_WQ_ACCT_NR; i++)
		io_acct_cancel_running_work(&wq->acct[i], match);

	rcu_read_unlock();
}

/**
 * This function attempts to cancel work in the specified io_wq workqueue.
 * It first checks the pending list for matching work items and removes them
 * if found. If a work item is removed from the pending list, the function
 * returns IO_WQ_CANCEL_OK, indicating that the work is returned as-new and
 * no completion will be posted for it.
 */
enum io_wq_cancel io_wq_cancel_cb(struct io_wq *wq, work_cancel_fn *cancel,
				  void *data, bool cancel_all)
{
	struct io_cb_cancel_data match = {
		.fn		= cancel,
		.data		= data,
		.cancel_all	= cancel_all,
	};

	/*
	 * First check pending list, if we're lucky we can just remove it
	 * from there. CANCEL_OK means that the work is returned as-new,
	 * no completion will be posted for it.
	 *
	 * Then check if a free (going busy) or busy worker has the work
	 * currently running. If we find it there, we'll return CANCEL_RUNNING
	 * as an indication that we attempt to signal cancellation. The
	 * completion will run normally in this case.
	 *
	 * Do both of these while holding the acct->workers_lock, to ensure that
	 * we'll find a work item regardless of state.
	 */
	io_wq_cancel_pending_work(wq, &match);
	if (match.nr_pending && !match.cancel_all)
		return IO_WQ_CANCEL_OK;

	io_wq_cancel_running_work(wq, &match);
	if (match.nr_running && !match.cancel_all)
		return IO_WQ_CANCEL_RUNNING;

	if (match.nr_running)
		return IO_WQ_CANCEL_RUNNING;
	if (match.nr_pending)
		return IO_WQ_CANCEL_OK;
	return IO_WQ_CANCEL_NOTFOUND;
}

/**
 * This function is called to wake up a work queue and ensure that any stalled
 * workers are reactivated. It removes the wait queue entry from its list,
 * iterates over the accounting structures of the work queue, and checks if
 * any of them are marked as stalled. If a stalled state is detected, it
 * activates a free worker to handle pending tasks.
 */
static int io_wq_hash_wake(struct wait_queue_entry *wait, unsigned mode,
			    int sync, void *key)
{
	struct io_wq *wq = container_of(wait, struct io_wq, wait);
	int i;

	list_del_init(&wait->entry);

	rcu_read_lock();
	for (i = 0; i < IO_WQ_ACCT_NR; i++) {
		struct io_wq_acct *acct = &wq->acct[i];

		if (test_and_clear_bit(IO_ACCT_STALLED_BIT, &acct->flags))
			io_acct_activate_free_worker(acct);
	}
	rcu_read_unlock();
	return 1;
}

/**
 * This function initializes and allocates resources for an io_wq instance.
 * It sets up the workqueue's hash, worker limits, CPU mask, and other
 * internal structures. The function also registers the workqueue with
 * the CPU hotplug state.
 */
struct io_wq *io_wq_create(unsigned bounded, struct io_wq_data *data)
{
	int ret, i;
	struct io_wq *wq;

	if (WARN_ON_ONCE(!data->free_work || !data->do_work))
		return ERR_PTR(-EINVAL);
	if (WARN_ON_ONCE(!bounded))
		return ERR_PTR(-EINVAL);

	wq = kzalloc(sizeof(struct io_wq), GFP_KERNEL);
	if (!wq)
		return ERR_PTR(-ENOMEM);

	refcount_inc(&data->hash->refs);
	wq->hash = data->hash;
	wq->free_work = data->free_work;
	wq->do_work = data->do_work;

	ret = -ENOMEM;

	if (!alloc_cpumask_var(&wq->cpu_mask, GFP_KERNEL))
		goto err;
	cpuset_cpus_allowed(data->task, wq->cpu_mask);
	wq->acct[IO_WQ_ACCT_BOUND].max_workers = bounded;
	wq->acct[IO_WQ_ACCT_UNBOUND].max_workers =
				task_rlimit(current, RLIMIT_NPROC);
	INIT_LIST_HEAD(&wq->wait.entry);
	wq->wait.func = io_wq_hash_wake;
	for (i = 0; i < IO_WQ_ACCT_NR; i++) {
		struct io_wq_acct *acct = &wq->acct[i];

		atomic_set(&acct->nr_running, 0);

		raw_spin_lock_init(&acct->workers_lock);
		INIT_HLIST_NULLS_HEAD(&acct->free_list, 0);
		INIT_LIST_HEAD(&acct->all_list);

		INIT_WQ_LIST(&acct->work_list);
		raw_spin_lock_init(&acct->lock);
	}

	wq->task = get_task_struct(data->task);
	atomic_set(&wq->worker_refs, 1);
	init_completion(&wq->worker_done);
	ret = cpuhp_state_add_instance_nocalls(io_wq_online, &wq->cpuhp_node);
	if (ret)
		goto err;

	return wq;
err:
	io_wq_put_hash(data->hash);
	free_cpumask_var(wq->cpu_mask);
	kfree(wq);
	return ERR_PTR(ret);
}

/**
 * This function determines whether the given callback corresponds to
 * the creation of an io_worker associated with a specific io_wq. It
 * checks if the callback function is either `create_worker_cb` or
 * `create_worker_cont`, and if so, verifies that the io_worker's work
 * queue (`wq`) matches the provided `data`.
 */
static bool io_task_work_match(struct callback_head *cb, void *data)
{
	struct io_worker *worker;

	if (cb->func != create_worker_cb && cb->func != create_worker_cont)
		return false;
	worker = container_of(cb, struct io_worker, create_work);
	return worker->wq == data;
}

/**
 * This function sets the IO_WQ_BIT_EXIT bit in the state of the given
 * io_wq instance, signaling that the work queue is in the process of
 * exiting. This is typically used to indicate that no further work
 * should be submitted to the queue and cleanup operations can begin.
 */
void io_wq_exit_start(struct io_wq *wq)
{
	set_bit(IO_WQ_BIT_EXIT, &wq->state);
}

/**

 * This function iteratively cancels task work associated with the creation
 * of io_wq workers. It uses task_work_cancel_match to find and cancel
 * matching task work items. For each canceled task work item, it retrieves
 * the associated io_worker structure and invokes io_worker_cancel_cb to
 * handle the cancellation logic.
 */
static void io_wq_cancel_tw_create(struct io_wq *wq)
{
	struct callback_head *cb;

	while ((cb = task_work_cancel_match(wq->task, io_task_work_match, wq)) != NULL) {
		struct io_worker *worker;

		worker = container_of(cb, struct io_worker, create_work);
		io_worker_cancel_cb(worker);
		/*
		 * Only the worker continuation helper has worker allocated and
		 * hence needs freeing.
		 */
		if (cb->func == create_worker_cont)
			kfree(worker);
	}
}

/**
 * This function performs the necessary cleanup for all workers associated
 * with the given io_wq. It ensures that all worker threads are properly
 * woken up and terminated, and that resources associated with the workqueue
 * are released.
 */
static void io_wq_exit_workers(struct io_wq *wq)
{
	if (!wq->task)
		return;

	io_wq_cancel_tw_create(wq);

	rcu_read_lock();
	io_wq_for_each_worker(wq, io_wq_worker_wake, NULL);
	rcu_read_unlock();
	io_worker_ref_put(wq);
	wait_for_completion(&wq->worker_done);

	spin_lock_irq(&wq->hash->wait.lock);
	list_del_init(&wq->wait.entry);
	spin_unlock_irq(&wq->hash->wait.lock);

	put_task_struct(wq->task);
	wq->task = NULL;
}

/**
 * This function performs the following steps to safely destroy an io_wq:
 * 1. Removes the CPU hotplug state instance associated with the workqueue.
 * 2. Cancels all pending work items in the workqueue.
 * 3. Frees the CPU mask allocated for the workqueue.
 * 4. Releases the hash table associated with the workqueue.
 * 5. Frees the memory allocated for the io_wq structure itself.
 */
static void io_wq_destroy(struct io_wq *wq)
{
	struct io_cb_cancel_data match = {
		.fn		= io_wq_work_match_all,
		.cancel_all	= true,
	};

	cpuhp_state_remove_instance_nocalls(io_wq_online, &wq->cpuhp_node);
	io_wq_cancel_pending_work(wq, &match);
	free_cpumask_var(wq->cpu_mask);
	io_wq_put_hash(wq->hash);
	kfree(wq);
}

/**
 * This function ensures that the io_wq instance is properly cleaned up
 * and its associated workers are exited. It first checks that the
 * IO_WQ_BIT_EXIT flag is set in the state of the io_wq structure, issuing
 * a warning if it is not. Then, it calls the necessary functions to
 * terminate the workers and destroy the io_wq instance.
 */
void io_wq_put_and_exit(struct io_wq *wq)
{
	WARN_ON_ONCE(!test_bit(IO_WQ_BIT_EXIT, &wq->state));

	io_wq_exit_workers(wq);
	io_wq_destroy(wq);
}

struct online_data {
	unsigned int cpu;
	bool online;
};

/**
 * This function modifies the CPU affinity mask of the worker's workqueue
 * based on the online status provided in the online_data structure. If the
 * `online` field in `online_data` is true, the specified CPU is added to the
 * worker's CPU mask. Otherwise, the CPU is removed from the mask.
 */
static bool io_wq_worker_affinity(struct io_worker *worker, void *data)
{
	struct online_data *od = data;

	if (od->online)
		cpumask_set_cpu(od->cpu, worker->wq->cpu_mask);
	else
		cpumask_clear_cpu(od->cpu, worker->wq->cpu_mask);
	return false;
}

/**
 * This function is called to update the worker threads' affinity in response
 * to CPU hotplug events. It iterates over all workers in the io_wq instance
 * and adjusts their affinity based on the CPU's online/offline status
 */
static int __io_wq_cpu_online(struct io_wq *wq, unsigned int cpu, bool online)
{
	struct online_data od = {
		.cpu = cpu,
		.online = online
	};

	rcu_read_lock();
	io_wq_for_each_worker(wq, io_wq_worker_affinity, &od);
	rcu_read_unlock();
	return 0;
}

/**
 * This function is called when a CPU comes online. It retrieves the
 * io_wq instance associated with the given hlist_node and invokes
 * the internal __io_wq_cpu_online function to handle the event.
 */
static int io_wq_cpu_online(unsigned int cpu, struct hlist_node *node)
{
	struct io_wq *wq = hlist_entry_safe(node, struct io_wq, cpuhp_node);

	return __io_wq_cpu_online(wq, cpu, true);
}

/**
 * This function is called when a CPU is going offline. It retrieves the
 * io_wq instance associated with the given hlist_node and performs the
 * necessary operations to handle the CPU offline event by invoking
 * __io_wq_cpu_online with the 'online' parameter set to false.
 */
static int io_wq_cpu_offline(unsigned int cpu, struct hlist_node *node)
{
	struct io_wq *wq = hlist_entry_safe(node, struct io_wq, cpuhp_node);

	return __io_wq_cpu_online(wq, cpu, false);
}

/**
 * This function sets or updates the CPU affinity mask for the io_uring task
 * associated with the given task context. If a specific mask is provided, it
 * checks whether the mask is a subset of the allowed CPUs for the task. If the
 * mask is valid, it updates the CPU mask for the io_uring workqueue. If no mask
 * is provided, the function sets the CPU mask to the allowed CPUs for the task.
 */
int io_wq_cpu_affinity(struct io_uring_task *tctx, cpumask_var_t mask)
{
	cpumask_var_t allowed_mask;
	int ret = 0;

	if (!tctx || !tctx->io_wq)
		return -EINVAL;

	if (!alloc_cpumask_var(&allowed_mask, GFP_KERNEL))
		return -ENOMEM;

	rcu_read_lock();
	cpuset_cpus_allowed(tctx->io_wq->task, allowed_mask);
	if (mask) {
		if (cpumask_subset(mask, allowed_mask))
			cpumask_copy(tctx->io_wq->cpu_mask, mask);
		else
			ret = -EINVAL;
	} else {
		cpumask_copy(tctx->io_wq->cpu_mask, allowed_mask);
	}
	rcu_read_unlock();

	free_cpumask_var(allowed_mask);
	return ret;
}

/*
 * Set max number of unbounded workers, returns old value. If new_count is 0,
 * then just return the old value.
 */
int io_wq_max_workers(struct io_wq *wq, int *new_count)
{
	struct io_wq_acct *acct;
	int prev[IO_WQ_ACCT_NR];
	int i;

	BUILD_BUG_ON((int) IO_WQ_ACCT_BOUND   != (int) IO_WQ_BOUND);
	BUILD_BUG_ON((int) IO_WQ_ACCT_UNBOUND != (int) IO_WQ_UNBOUND);
	BUILD_BUG_ON((int) IO_WQ_ACCT_NR      != 2);

	for (i = 0; i < IO_WQ_ACCT_NR; i++) {
		if (new_count[i] > task_rlimit(current, RLIMIT_NPROC))
			new_count[i] = task_rlimit(current, RLIMIT_NPROC);
	}

	for (i = 0; i < IO_WQ_ACCT_NR; i++)
		prev[i] = 0;

	rcu_read_lock();

	for (i = 0; i < IO_WQ_ACCT_NR; i++) {
		acct = &wq->acct[i];
		raw_spin_lock(&acct->workers_lock);
		prev[i] = max_t(int, acct->max_workers, prev[i]);
		if (new_count[i])
			acct->max_workers = new_count[i];
		raw_spin_unlock(&acct->workers_lock);
	}
	rcu_read_unlock();

	for (i = 0; i < IO_WQ_ACCT_NR; i++)
		new_count[i] = prev[i];

	return 0;
}

/**
 * This function sets up the CPU hotplug state for the io-wq subsystem,
 * allowing it to handle CPU online and offline events. It registers
 * callbacks for these events using `cpuhp_setup_state_multi`.
 */
static __init int io_wq_init(void)
{
	int ret;

	ret = cpuhp_setup_state_multi(CPUHP_AP_ONLINE_DYN, "io-wq/online",
					io_wq_cpu_online, io_wq_cpu_offline);
	if (ret < 0)
		return ret;
	io_wq_online = ret;
	return 0;
}
subsys_initcall(io_wq_init);
