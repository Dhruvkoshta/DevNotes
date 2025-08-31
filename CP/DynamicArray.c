#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *array;
  size_t used;
  size_t size;
} DynamicArray;

int main() {
  DynamicArray arr = {0};
  for (int i = 0; i < 10; i++) {
    if (arr.used == arr.size) {
      arr.size = arr.size ? arr.size * 2 : 128;
      arr.array = realloc(arr.array, arr.size * sizeof(int));
    }
    arr.array[arr.used++] = i;
  }
}
