---

# 📘 **LINKED LISTS – Detailed Notes (with C Implementation)**

---

# 1️⃣ **What is a Linked List?**

A **Linked List** is a linear data structure where elements (called **nodes**) are stored in **non-contiguous memory locations**.

Each node contains:

1. **Data**
2. **Pointer to the next node**

```
[Data | Next] → [Data | Next] → [Data | Next] → NULL
```

---

# 2️⃣ **Why Linked List?**

### ✔ Advantages

* **Dynamic size** (grows/shrinks at runtime)
* **Efficient insertion/deletion** (O(1) if position known)
* No need for contiguous memory (unlike arrays)
* Useful for implementing stacks, queues, graphs.

### ❌ Disadvantages

* No random access (must traverse sequentially)
* Extra memory for storing pointers
* Slower traversal due to pointer overhead
* Not cache-friendly

---

# 3️⃣ **Types of Linked Lists**

---

## 🔹 3.1 **Singly Linked List**

Each node has:

* data
* pointer to **next** node

```
+------+------+
| Data | Next | --->  +------+------+
+------+------         | Data | Next | ---> NULL
```

### Pointer structure (C):

```c
struct Node {
    int data;
    struct Node* next;
};
```

---

## 🔹 3.2 **Doubly Linked List**

Each node has:

* data
* pointer to previous node
* pointer to next node

```
NULL ←→ [Prev | Data | Next] ←→ [Prev | Data | Next] ←→ NULL
```

### Structure:

```c
struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};
```

---

## 🔹 3.3 **Circular Linked List**

Last node points back to the **head**, not NULL.

### Circular Singly:

```
A → B → C → D → A (head)
```

### Circular Doubly:

```
↔ A ↔ B ↔ C ↔ D ↔ (back to A)
```

### Structure similar to singly/doubly — only last node links to head.

---

---

# 4️⃣ **SINGLY LINKED LIST – Full Implementation in C**

---

## ✔ 4.1 Node Structure

```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};
```

---

## ✔ 4.2 Insert at Beginning

```c
struct Node* insertAtBeginning(struct Node* head, int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = head;
    return newNode;
}
```

---

## ✔ 4.3 Insert at End

```c
struct Node* insertAtEnd(struct Node* head, int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;

    if (head == NULL) return newNode;

    struct Node* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;
    return head;
}
```

---

## ✔ 4.4 Insert at Position

```c
struct Node* insertAtPosition(struct Node* head, int value, int pos) {
    if (pos == 1)
        return insertAtBeginning(head, value);

    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = value;

    struct Node* temp = head;
    for (int i = 1; i < pos - 1 && temp != NULL; i++)
        temp = temp->next;

    if (temp == NULL) {
        printf("Position out of bounds\n");
        return head;
    }

    newNode->next = temp->next;
    temp->next = newNode;

    return head;
}
```

---

## ✔ 4.5 Delete from Beginning

```c
struct Node* deleteFromBeginning(struct Node* head) {
    if (head == NULL) return NULL;

    struct Node* temp = head;
    head = head->next;
    free(temp);
    return head;
}
```

---

## ✔ 4.6 Delete from End

```c
struct Node* deleteFromEnd(struct Node* head) {
    if (head == NULL) return NULL;

    if (head->next == NULL) {
        free(head);
        return NULL;
    }

    struct Node* temp = head;
    while (temp->next->next != NULL)
        temp = temp->next;

    free(temp->next);
    temp->next = NULL;

    return head;
}
```

---

## ✔ 4.7 Delete at Position

```c
struct Node* deleteAtPosition(struct Node* head, int pos) {
    if (pos == 1)
        return deleteFromBeginning(head);

    struct Node* temp = head;
    for (int i = 1; i < pos - 1 && temp != NULL; i++)
        temp = temp->next;

    if (temp == NULL || temp->next == NULL) {
        printf("Position invalid\n");
        return head;
    }

    struct Node* target = temp->next;
    temp->next = target->next;
    free(target);

    return head;
}
```

---

## ✔ 4.8 Searching in Linked List

```c
void search(struct Node* head, int key) {
    int pos = 1;
    while (head != NULL) {
        if (head->data == key) {
            printf("Found at position %d\n", pos);
            return;
        }
        head = head->next;
        pos++;
    }
    printf("Not found\n");
}
```

---

## ✔ 4.9 Display List

```c
void display(struct Node* head) {
    while (head != NULL) {
        printf("%d → ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}
```

---

# 5️⃣ **DOUBLY LINKED LIST – Main Operations**

(Short version; can expand if you want)

---

## ✔ 5.1 Structure

```c
struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};
```

---

## ✔ 5.2 Insert at Beginning

```c
struct Node* insertAtBeginning(struct Node* head, int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->prev = NULL;
    newNode->next = head;

    if (head != NULL)
        head->prev = newNode;

    return newNode;
}
```

---

## ✔ 5.3 Insert at End

```c
struct Node* insertAtEnd(struct Node* head, int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;

    if (head == NULL) {
        newNode->prev = NULL;
        return newNode;
    }

    struct Node* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;
    newNode->prev = temp;
    return head;
}
```

---

# 6️⃣ **CIRCULAR LINKED LIST (Concept)**

### Changes:

* Last node points back to **head**.
* Useful for round-robin scheduling, music playlists, queues.

### Example:

```
head → A → B → C → (back to head)
```

---

# 7️⃣ **Linked List vs Array**

| Feature         | Array             | Linked List |
| --------------- | ----------------- | ----------- |
| Memory          | Contiguous        | Anywhere    |
| Insert/Delete   | Expensive (shift) | O(1)        |
| Access          | O(1) random       | O(n)        |
| Memory overhead | Low               | High        |
| Cache-friendly  | Yes               | No          |

---

# 8️⃣ **Real-life Applications**

* Undo/Redo in editors
* Playlist navigation
* Browser forward/back history
* Queues/stacks implementation
* Graph adjacency lists
* Memory management (free list)

---

