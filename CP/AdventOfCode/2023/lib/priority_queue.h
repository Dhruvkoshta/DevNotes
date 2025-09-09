#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h> // For size_t

/**
 * @struct Node
 * @brief Represents an element in the priority queue.
 *
 * @var Node::data
 * A generic pointer to the data stored in the node.
 * @var Node::priority
 * The priority of the node. Lower values represent higher priority.
 */
typedef struct {
  void *data;
  int priority;
} Node;

/**
 * @struct PriorityQueue
 * @brief Represents the priority queue itself, implemented as a min-heap.
 *
 * @var PriorityQueue::nodes
 * A dynamic array of nodes that form the heap.
 * @var PriorityQueue::capacity
 * The maximum number of elements the queue can hold.
 * @var PriorityQueue::size
 * The current number of elements in the queue.
 */
typedef struct {
  Node *nodes;
  int capacity;
  int size;
} PriorityQueue;

/**
 * @brief Creates and initializes a new priority queue.
 *
 * @param capacity The maximum number of elements the priority queue can hold.
 * @return A pointer to the newly created PriorityQueue, or NULL if memory
 * allocation fails.
 */
PriorityQueue *pq_create(int capacity);

/**
 * @brief Frees all memory associated with the priority queue.
 * @note This function does NOT free the data pointed to by each node.
 * The user is responsible for managing that memory.
 *
 * @param pq A pointer to the PriorityQueue to be destroyed.
 */
void pq_destroy(PriorityQueue *pq);

/**
 * @brief Inserts a new element into the priority queue.
 *
 * @param pq A pointer to the PriorityQueue.
 * @param data A pointer to the data to be stored.
 * @param priority The priority of the data. Lower values are higher priority.
 */
void pq_push(PriorityQueue *pq, void *data, int priority);

/**
 * @brief Removes and returns the element with the highest priority (lowest
 * priority value).
 *
 * @param pq A pointer to the PriorityQueue.
 * @return A pointer to the data of the highest-priority element, or NULL if
 * the queue is empty.
 */
void *pq_pop(PriorityQueue *pq);

/**
 * @brief Returns the element with the highest priority without removing it.
 *
 * @param pq A pointer to the PriorityQueue.
 * @return A pointer to the data of the highest-priority element, or NULL if
 * the queue is empty.
 */
void *pq_peek(PriorityQueue *pq);

/**
 * @brief Checks if the priority queue is empty.
 *
 * @param pq A pointer to the PriorityQueue.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int pq_is_empty(PriorityQueue *pq);

/**
 * @brief Checks if the priority queue is full.
 *
 * @param pq A pointer to the PriorityQueue.
 * @return 1 if the queue is full, 0 otherwise.
 */
int pq_is_full(PriorityQueue *pq);

/**
 * @brief Gets the current number of elements in the priority queue.
 *
 * @param pq A pointer to the PriorityQueue.
 * @return The number of elements in the queue.
 */
int pq_get_size(PriorityQueue *pq);

#endif // PRIORITY_QUEUE_H
