#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

// --- Helper Function Prototypes (Static) ---

/**
 * @brief Swaps two nodes in the priority queue.
 * @param a Pointer to the first node.
 * @param b Pointer to the second node.
 */
static void swap_nodes(Node *a, Node *b);

/**
 * @brief Restores the min-heap property by moving an element up the tree.
 * This is called after an insertion.
 * @param pq A pointer to the PriorityQueue.
 * @param index The index of the element to heapify up.
 */
static void heapify_up(PriorityQueue *pq, int index);

/**
 * @brief Restores the min-heap property by moving an element down the tree.
 * This is called after a pop operation.
 * @param pq A pointer to the PriorityQueue.
 * @param index The index of the element to heapify down.
 */
static void heapify_down(PriorityQueue *pq, int index);

// --- Public Function Implementations ---

PriorityQueue *pq_create(int capacity) {
  if (capacity <= 0) {
    return NULL;
  }

  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  if (pq == NULL) {
    perror("Failed to allocate memory for priority queue");
    return NULL;
  }

  pq->nodes = (Node *)malloc(sizeof(Node) * capacity);
  if (pq->nodes == NULL) {
    perror("Failed to allocate memory for nodes");
    free(pq); // Clean up partially allocated memory
    return NULL;
  }

  pq->capacity = capacity;
  pq->size = 0;

  return pq;
}

void pq_destroy(PriorityQueue *pq) {
  if (pq == NULL) {
    return;
  }
  // Note: This library does not own the data inside the nodes.
  // The user is responsible for freeing the data pointed to by each node's
  // `data` pointer.
  free(pq->nodes);
  free(pq);
}

void pq_push(PriorityQueue *pq, void *data, int priority) {
  if (pq == NULL || pq_is_full(pq)) {
    return; // Or handle error appropriately
  }

  // Add the new element to the end of the array
  pq->nodes[pq->size].data = data;
  pq->nodes[pq->size].priority = priority;
  pq->size++;

  // Restore the heap property by moving the new element up
  heapify_up(pq, pq->size - 1);
}

void *pq_pop(PriorityQueue *pq) {
  if (pq == NULL || pq_is_empty(pq)) {
    return NULL;
  }

  // The root of the heap is the element with the highest priority
  void *data = pq->nodes[0].data;

  // Replace the root with the last element
  pq->nodes[0] = pq->nodes[pq->size - 1];
  pq->size--;

  // Restore the heap property by moving the new root down
  heapify_down(pq, 0);

  return data;
}

void *pq_peek(PriorityQueue *pq) {
  if (pq == NULL || pq_is_empty(pq)) {
    return NULL;
  }
  return pq->nodes[0].data;
}

int pq_is_empty(PriorityQueue *pq) { return (pq == NULL || pq->size == 0); }

int pq_is_full(PriorityQueue *pq) {
  return (pq != NULL && pq->size == pq->capacity);
}

int pq_get_size(PriorityQueue *pq) {
  if (pq == NULL) {
    return 0;
  }
  return pq->size;
}

// --- Helper Function Implementations (Static) ---

static void swap_nodes(Node *a, Node *b) {
  Node temp = *a;
  *a = *b;
  *b = temp;
}

static void heapify_up(PriorityQueue *pq, int index) {
  if (index == 0)
    return; // Root of the heap

  int parent_index = (index - 1) / 2;

  // If the child's priority is higher (value is lower) than the parent's, swap
  // them
  if (pq->nodes[index].priority < pq->nodes[parent_index].priority) {
    swap_nodes(&pq->nodes[index], &pq->nodes[parent_index]);
    // Recursively heapify up from the parent's new position
    heapify_up(pq, parent_index);
  }
}

static void heapify_down(PriorityQueue *pq, int index) {
  int left_child_index = 2 * index + 1;
  int right_child_index = 2 * index + 2;
  int smallest = index; // Assume current node is the smallest

  // Check if left child exists and has a higher priority (smaller value)
  if (left_child_index < pq->size &&
      pq->nodes[left_child_index].priority < pq->nodes[smallest].priority) {
    smallest = left_child_index;
  }

  // Check if right child exists and has a higher priority (smaller value)
  if (right_child_index < pq->size &&
      pq->nodes[right_child_index].priority < pq->nodes[smallest].priority) {
    smallest = right_child_index;
  }

  // If the smallest is not the current node, swap and continue heapifying down
  if (smallest != index) {
    swap_nodes(&pq->nodes[index], &pq->nodes[smallest]);
    heapify_down(pq, smallest);
  }
}
