

> [!NOTE] Def:
> Priority queue is an abstract data type(ADT) in the computer science which is designed to the operate much like the regular queue except that each element has the certain priority. The priority can determines the order in which elements are dequeued - elements with the higher priority are removed from queue before those with lower priority

```c
typedef struct {
    int items[MAX];
    int size;
} PriorityQueue;

```

### 1. Enqueue Operation

This operation can be used to add the new element to the priority queue with the given priority.

****Algorithm****

> - Add the element to end of the heap.
> - Restore the heap property by the comparing the added element with its parent. If it can violates the heap property, swap them.
> - Continues this process up the heap until the correct position is found or root is reached.

### 2. Dequeue (Extract - Min/Max)

This operation can be used to removes and return the elements with the highest max in the max-heap and min in the min-heap of the priority queue.

****Algorithms****

> 1. Replace the root of heap with the last element in the heap.
> 2. Reduce the size of the heap by the one.
> 3. Restore the heap property by the recursively comparing the new root with its children and swapping it
> 4. with the higher priority child in the max-heap or the lower priority child in the min heap.
> 5. Continues this process down the heap until the correct position is found or the leaf is reached.

### 3. Peek

This operation can be used to returns the element with the highest priority without the removing it from the priority queue.

****Algorithm****

> 1. Return the element at the root of the heap.

### 4. Increase/Decrease Key

This operation can be used to change the priority of the element in the priority queue.

****Algorithm****

> 1. Locate the element whose the priority needs to be updated.
> 2. Update the priority of the element.
> 3. If the priority is increased in the max-heap or decreased in the min-heap and it can restore the heap property by the heapifying up from the element.
> 4. If the priority is decreased in the max-heap or increased in the min-heap and restore the heap property by the heapifying down from element.

```c
#include <stdio.h>
#include <stdlib.h>

// Define maximum size of the priority queue
#define MAX 100

// Define PriorityQueue structure
typedef struct {
    int items[MAX];
    int size;
} PriorityQueue;

// Define swap function to swap two integers
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Define heapifyUp function to maintain heap property
// during insertion
void heapifyUp(PriorityQueue* pq, int index)
{
    if (index
        && pq->items[(index - 1) / 2] > pq->items[index]) {
        swap(&pq->items[(index - 1) / 2],
             &pq->items[index]);
        heapifyUp(pq, (index - 1) / 2);
    }
}

// Define enqueue function to add an item to the queue
void enqueue(PriorityQueue* pq, int value)
{
    if (pq->size == MAX) {
        printf("Priority queue is full\n");
        return;
    }

    pq->items[pq->size++] = value;
    heapifyUp(pq, pq->size - 1);
}

// Define heapifyDown function to maintain heap property
// during deletion
int heapifyDown(PriorityQueue* pq, int index)
{
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size
        && pq->items[left] < pq->items[smallest])
        smallest = left;

    if (right < pq->size
        && pq->items[right] < pq->items[smallest])
        smallest = right;

    if (smallest != index) {
        swap(&pq->items[index], &pq->items[smallest]);
        heapifyDown(pq, smallest);
    }
}

// Define dequeue function to remove an item from the queue
int dequeue(PriorityQueue* pq)
{
    if (!pq->size) {
        printf("Priority queue is empty\n");
        return -1;
    }

    int item = pq->items[0];
    pq->items[0] = pq->items[--pq->size];
    heapifyDown(pq, 0);
    return item;
}

// Define peek function to get the top item from the queue
int peek(PriorityQueue* pq)
{
    if (!pq->size) {
        printf("Priority queue is empty\n");
        return -1;
    }
    return pq->items[0];
}

// Define main function
int main()
{
    // Initialize priority queue
    PriorityQueue pq = { { 0 }, 0 };
    // Add items to the queue
    enqueue(&pq, 3);
    enqueue(&pq, 2);
    enqueue(&pq, 15);
    enqueue(&pq, 5);
    enqueue(&pq, 4);
    enqueue(&pq, 45);

    // Dequeue an item and print it
    printf("%d dequeued from queue\n", dequeue(&pq));
    // Print the top item of the queue
    printf("Top element is %d\n", peek(&pq));

    return 0;
}
```