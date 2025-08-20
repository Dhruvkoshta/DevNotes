#include <stdlib.h>
#include <string.h>

typedef struct {
  int r;
  int c;
} Point;

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume
 * caller calls free().
 */
int **floodFill(int **image, int imageSize, int *imageColSize, int sr, int sc,
                int color, int *returnSize, int **returnColumnSizes) {
  *returnSize = imageSize;
  int numCols = imageColSize[0];

  //  Allocate memory for the output and create a deep copy

  *returnColumnSizes = (int *)malloc(imageSize * sizeof(int));

  int **resultImage = (int **)malloc(imageSize * sizeof(int *));

  for (int i = 0; i < imageSize; i++) {
    (*returnColumnSizes)[i] = numCols;

    resultImage[i] = (int *)malloc(numCols * sizeof(int));

    memcpy(resultImage[i], image[i], numCols * sizeof(int));
  }

  // Handle Edge Case
  int startColor = image[sr][sc];
  if (startColor == color) {
    return resultImage;
  }

  Point *queue = (Point *)malloc(imageSize * numCols * sizeof(Point));
  int front = 0;
  int rear = 0;

  // A 2D array to keep track of visited pixels, initialized to 0.
  int **visited = (int **)calloc(imageSize, sizeof(int *));
  for (int i = 0; i < imageSize; i++) {
    visited[i] = (int *)calloc(numCols, sizeof(int));
  }

  // --- Start BFS ---

  queue[rear].r = sr;
  queue[rear].c = sc;
  rear++;
  visited[sr][sc] = 1;

  int dr[] = {-1, 1, 0, 0};
  int dc[] = {0, 0, -1, 1};

  while (front < rear) {
    Point current = queue[front];
    front++;

    resultImage[current.r][current.c] = color;

    for (int i = 0; i < 4; i++) {
      int new_r = current.r + dr[i];
      int new_c = current.c + dc[i];

      if (new_r >= 0 && new_r < imageSize && new_c >= 0 && new_c < numCols &&
          !visited[new_r][new_c] && image[new_r][new_c] == startColor) {

        visited[new_r][new_c] = 1;
        queue[rear].r = new_r;
        queue[rear].c = new_c;
        rear++;
      }
    }
  }

  free(queue);
  for (int i = 0; i < imageSize; i++) {
    free(visited[i]);
  }
  free(visited);

  return resultImage;
}
