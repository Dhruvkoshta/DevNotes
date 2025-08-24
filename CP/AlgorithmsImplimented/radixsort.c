#include <stdio.h>
#include <stdlib.h>

void radixSort(int arr[], int n) {
  int max = getMax(arr, n);
  for (int exp = 1; max / exp > 0; exp *= 10) {
    countingSort(arr, n, exp);
  }
}

void printArray(int arr[], int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int main() {
  int arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
  int n = sizeof(arr) / sizeof(arr[0]);

  printf("Original array: \n");
  printArray(arr, n);

  radixSort(arr, n);

  printf("Sorted array: \n");
  printArray(arr, n);

  return 0;
}
