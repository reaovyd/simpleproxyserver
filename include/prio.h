/**
 * @file prio.h
 * @brief A simple priority queue implemented
 * by a binary heap. The priority queue is 
 * represented as an array. The operations
 * supported are inserting into the priority 
 * queue, removing the max value from the priority 
 * queue, and getting the max value from priority
 * queue. The capacity of the priority queue is
 * bounded by a fixed size of 1024. The priority
 * queue is implemented by a max heap.
 *
 */

#ifndef PRIO_H
#define PRIO_H

/**
 * @struct PrioQueue prio.h "include/prio.h"
 * @brief This is the structure that represents
 * the priority queue. The operations below
 * manipulate this object. The priority queue
 * follows the max-heap invariant as we can
 * quickly yield the maximum element. The structure
 * is implemented as the following inside the source
 * file:
 *
 * ```
 * struct prio_queue {
 *     int heap[MAX_PRIO_CAPACITY];
 *     int len;
 * };
 * ```
 */

typedef struct prio_queue PrioQueue;

/**
 * @brief Initializes a new priority queue for the caller
 * to use. On failure, it returns NULL, which is
 * likely due to a malloc error. 
 *
 * @return On success, a pointer to the block that 
 * is allocated for the priority queue object. On
 * failure, it returns NULL.
 *
 */
extern PrioQueue *prio_init();

/**
 * @brief Takes an argument `get_max`, a pointer given by 
 * the caller so that the function can copy to that 
 * address the max value of the priority queue `pq`.
 * The function returns -1 and no copying is done
 * if the function fails. The function will fail
 * if there is nothing currently in the priority 
 * queue. Also, if `get_max` points to the `NULL` 
 * address, then the function also fails. 
 *
 * @param pq A pointer to a priority queue object
 * @param get_max A pointer to some address where
 * the value at that address gets overwritten with 
 * the max value on success. Nothing happens if the
 * function call fails.
 * @return 0 if the call succeeds. -1, otherwise. 
 *
 */

extern int prio_peek_max(PrioQueue *pq, int *get_max);

/**
 * @brief Takes an argument `val`, a value given by 
 * the caller so that the function can insert this
 * value into the priority queue. The function returns 
 * -1 and no insertion is done if the function fails. 
 * The function will fail if it attempts to add to a
 * priority queue where the capacity of 1024 is reached.
 *
 * @param pq   A pointer to a priority queue object
 * @param val  The value to be inserted 
 * @return 0 if the call succeeds. -1, otherwise. 
 *
 */

extern int prio_insert(PrioQueue *pq, int val);

/**
 * @brief This function operates the same as 
 * ``prio_peek_max`` except for the fact
 * that it removes the current top element.
 * Similar to ``prio_peek_max``, the function
 * fails and returns -1 if there is nothing 
 * inside the priority queue.
 *
 * @param pq A pointer to a priority queue object
 * @param get_max a pointer to some address where
 * the value at that address gets overwritten with 
 * the max value on success. Nothing happens if the
 * function call fails.
 * @return 0 if the call succeeds. -1, otherwise. 
 *
 */
extern int prio_remove_max(PrioQueue *pq, int *get_max);

/**
 * @brief This function frees the block pointed
 * to by the priority queue ``pq``. 
 *
 * @param pq A poitner to a priority queue object
 *
 */
extern void prio_destroy(PrioQueue *pq);



#endif /* PRIO_H */
