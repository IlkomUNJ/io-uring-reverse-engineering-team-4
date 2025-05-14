#ifndef INTERNAL_IO_SLIST_H
#define INTERNAL_IO_SLIST_H

#include <linux/io_uring_types.h>

/**
 * __wq_list_for_each - Iterates over each node in a work queue list.
 *
 * @pos: Pointer to the current node in the iteration.
 * @head: Pointer to the head of the work queue list.
 *
 * This macro iterates over each node in the work queue list starting from
 * the first node.
 */
#define __wq_list_for_each(pos, head)				\
    for (pos = (head)->first; pos; pos = (pos)->next)

/**
 * wq_list_for_each - Iterates over each node in a work queue list with a previous pointer.
 *
 * @pos: Pointer to the current node in the iteration.
 * @prv: Pointer to the previous node in the iteration.
 * @head: Pointer to the head of the work queue list.
 *
 * This macro iterates over each node in the work queue list while keeping
 * track of the previous node.
 */
#define wq_list_for_each(pos, prv, head)			\
    for (pos = (head)->first, prv = NULL; pos; prv = pos, pos = (pos)->next)

/**
 * wq_list_for_each_resume - Resumes iteration over a work queue list.
 *
 * @pos: Pointer to the current node in the iteration.
 * @prv: Pointer to the previous node in the iteration.
 *
 * This macro resumes iteration over a work queue list from the current
 * position.
 */
#define wq_list_for_each_resume(pos, prv)			\
    for (; pos; prv = pos, pos = (pos)->next)

/**
 * wq_list_empty - Checks if a work queue list is empty.
 *
 * @list: Pointer to the work queue list.
 *
 * Returns:
 * - true if the list is empty.
 * - false otherwise.
 */
#define wq_list_empty(list)	(READ_ONCE((list)->first) == NULL)

/**
 * INIT_WQ_LIST - Initializes a work queue list.
 *
 * @list: Pointer to the work queue list to be initialized.
 *
 * This macro initializes a work queue list by setting its first node to NULL.
 */
#define INIT_WQ_LIST(list)	do {				\
    (list)->first = NULL;					\
} while (0)

/**
 * wq_list_add_after - Adds a node after a specified position in a work queue list.
 *
 * @node: Pointer to the node to be added.
 * @pos: Pointer to the position after which the node will be added.
 * @list: Pointer to the work queue list.
 *
 * This function inserts a node after the specified position in the work queue list.
 */
static inline void wq_list_add_after(struct io_wq_work_node *node,
                     struct io_wq_work_node *pos,
                     struct io_wq_work_list *list)
{
    struct io_wq_work_node *next = pos->next;

    pos->next = node;
    node->next = next;
    if (!next)
        list->last = node;
}

/**
 * wq_list_add_tail - Adds a node to the tail of a work queue list.
 *
 * @node: Pointer to the node to be added.
 * @list: Pointer to the work queue list.
 *
 * This function appends a node to the end of the work queue list.
 */
static inline void wq_list_add_tail(struct io_wq_work_node *node,
                    struct io_wq_work_list *list)
{
    node->next = NULL;
    if (!list->first) {
        list->last = node;
        WRITE_ONCE(list->first, node);
    } else {
        list->last->next = node;
        list->last = node;
    }
}

/**
 * wq_list_add_head - Adds a node to the head of a work queue list.
 *
 * @node: Pointer to the node to be added.
 * @list: Pointer to the work queue list.
 *
 * This function prepends a node to the beginning of the work queue list.
 */
static inline void wq_list_add_head(struct io_wq_work_node *node,
                    struct io_wq_work_list *list)
{
    node->next = list->first;
    if (!node->next)
        list->last = node;
    WRITE_ONCE(list->first, node);
}

/**
 * wq_list_cut - Cuts a segment from a work queue list.
 *
 * @list: Pointer to the work queue list.
 * @last: Pointer to the last node in the segment to be cut.
 * @prev: Pointer to the node preceding the segment to be cut.
 *
 * This function removes a segment of nodes from the work queue list.
 */
static inline void wq_list_cut(struct io_wq_work_list *list,
                   struct io_wq_work_node *last,
                   struct io_wq_work_node *prev)
{
	/* first in the list, if prev==NULL */
    if (!prev)
        WRITE_ONCE(list->first, last->next);
    else
        prev->next = last->next;

    if (last == list->last)
        list->last = prev;
    last->next = NULL;
}

/**
 * __wq_list_splice - Splices a work queue list into another list.
 *
 * @list: Pointer to the work queue list to be spliced.
 * @to: Pointer to the node where the list will be spliced.
 *
 * This function appends the nodes of one work queue list to another.
 */
static inline void __wq_list_splice(struct io_wq_work_list *list,
                    struct io_wq_work_node *to)
{
    list->last->next = to->next;
    to->next = list->first;
    INIT_WQ_LIST(list);
}

/**
 * wq_list_splice - Splices a work queue list into another list if not empty.
 *
 * @list: Pointer to the work queue list to be spliced.
 * @to: Pointer to the node where the list will be spliced.
 *
 * Returns:
 * - true if the list was spliced successfully.
 * - false if the list was empty.
 */
static inline bool wq_list_splice(struct io_wq_work_list *list,
                  struct io_wq_work_node *to)
{
    if (!wq_list_empty(list)) {
        __wq_list_splice(list, to);
        return true;
    }
    return false;
}

/**
 * wq_stack_add_head - Adds a node to the head of a stack.
 *
 * @node: Pointer to the node to be added.
 * @stack: Pointer to the stack.
 *
 * This function pushes a node onto the stack.
 */
static inline void wq_stack_add_head(struct io_wq_work_node *node,
                     struct io_wq_work_node *stack)
{
    node->next = stack->next;
    stack->next = node;
}

/**
 * wq_list_del - Deletes a node from a work queue list.
 *
 * @list: Pointer to the work queue list.
 * @node: Pointer to the node to be deleted.
 * @prev: Pointer to the node preceding the node to be deleted.
 *
 * This function removes a node from the work queue list.
 */
static inline void wq_list_del(struct io_wq_work_list *list,
                   struct io_wq_work_node *node,
                   struct io_wq_work_node *prev)
{
    wq_list_cut(list, node, prev);
}

/**
 * wq_stack_extract - Extracts a node from the head of a stack.
 *
 * @stack: Pointer to the stack.
 *
 * Returns:
 * - Pointer to the extracted node.
 */
static inline
struct io_wq_work_node *wq_stack_extract(struct io_wq_work_node *stack)
{
    struct io_wq_work_node *node = stack->next;

    stack->next = node->next;
    return node;
}

/**
 * wq_next_work - Retrieves the next work item in a work queue.
 *
 * @work: Pointer to the current work item.
 *
 * Returns:
 * - Pointer to the next work item, or NULL if there are no more items.
 */
static inline struct io_wq_work *wq_next_work(struct io_wq_work *work)
{
    if (!work->list.next)
        return NULL;

    return container_of(work->list.next, struct io_wq_work, list);
}

#endif // INTERNAL_IO_SLIST_H