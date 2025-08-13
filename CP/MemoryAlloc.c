#include <stdio.h>
#include <stdlib.h>

struct list {
  int *data;
  int numItems;
  int size;
};

void addToList(struct list *l, int item);

int main(int argc, char *argv[]) {
  struct list myList;
  int amount;

  myList.numItems = 0;
  myList.size = 10;
  myList.data = malloc(myList.size * sizeof(int));

  if (myList.data == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }

  amount = 55;
  for (int i = 0; i < amount; i++) {
    addToList(&myList, i + 1);
  }

  for (int i = 0; i < myList.numItems; i++) {
    printf("%d ", myList.data[i]);
  }

  free(myList.data);
  myList.data = NULL;
}

void addToList(struct list *l, int item) {
  if (l->numItems == l->size) {
    l->size += 10;
    l->data = realloc(l->data, l->size * sizeof(int));
  }
  l->data[l->numItems] = item;
  l->numItems++;
}
