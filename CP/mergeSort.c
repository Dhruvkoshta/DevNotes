
#include <stdio.h>
#include <stdlib.h>

void printArray(int arr[], int size);

void merge(int arr[], int left, int mid, int right) {
  int i, j, k;
  int n1 = mid - left + 1; // Size of the first subarray
  int n2 = right - mid;    // Size of the second subarray
  printf("Merging from %d to %d with mid at %d\n", left, right, mid);
  // Create temporary arrays
  int *L = (int *)malloc(n1 * sizeof(int));
  int *R = (int *)malloc(n2 * sizeof(int));

  // Copy data to temporary arrays L[] and R[]
  for (i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];
  printf("Left array: ");
  printArray(L, n1);
  printf("Right array: ");
  printArray(R, n2);
  // Merge the temporary arrays back into arr[left..right]
  i = 0;    // Initial index of first subarray
  j = 0;    // Initial index of second subarray
  k = left; // Initial index of merged subarray

  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
      printf("Added %d from left array to merged array\n", L[i - 1]);
    } else {
      arr[k] = R[j];
      j++;
      printf("Added %d from right array to merged array\n", R[j - 1]);
    }
    k++;
  }

  // Copy the remaining elements of L[], if any
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  // Copy the remaining elements of R[], if any
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }

  // Free the dynamically allocated temporary arrays
  free(L);
  free(R);
}

// Function to implement merge sort
void mergeSort(int arr[], int left, int right) {
  if (left < right) {
    // Find the middle point to divide the array into two halves
    int mid = left + (right - left) / 2;

    printf("Dividing from %d to %d with mid at %d\n", left, right, mid);
    // Recursively sort the first and second halves
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // Merge the sorted halves
    merge(arr, left, mid, right);
  }
}

// Function to print an array
void printArray(int arr[], int size) {
  int i;
  for (i = 0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}

// Main function to test the merge sort implementation
int main() {
  int arr[] = {12, 11, 13, 5, 6, 7};
  int arr_size = sizeof(arr) / sizeof(arr[0]);

  printf("Given array is \n");
  printArray(arr, arr_size);

  mergeSort(arr, 0, arr_size - 1);

  printf("\nSorted array is \n");
  printArray(arr, arr_size);

  return 0;
}
