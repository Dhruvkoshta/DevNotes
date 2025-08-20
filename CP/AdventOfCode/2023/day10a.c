#include <stdio.h>
#include <string.h>

#define MAX_DIM 200

typedef struct {
  int x;
  int y;
} Point;

// QUEUE IMPLIMENTATION

Point queue[MAX_DIM * MAX_DIM];
int front = 0;
int rear = -1;
int queue_size = 0;

void enqueue(Point p) {
  rear = (rear + 1) % (MAX_DIM * MAX_DIM);
  queue[rear] = p;
  queue_size++;
}

Point dequeue() {
  Point p = queue[front];
  front = (front + 1) % (MAX_DIM * MAX_DIM);
  queue_size--;
  return p;
}
int is_queue_empty() { return queue_size == 0; }

int main(int argc, char *argv[]) {
  char grid[MAX_DIM][MAX_DIM];
  int visited[MAX_DIM][MAX_DIM] = {0};
  int num_rows = 0, num_cols = 0;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  FILE *fptr;
  fptr = fopen(argv[1], "r");
  if (fptr == NULL) {
    perror("Error opening file");
    return 1;
  }

  while (fgets(grid[num_rows], MAX_DIM, fptr) != NULL) {
    grid[num_rows][strcspn(grid[num_rows], "\n")] =
        '\0'; // Remove newline character
    if (num_cols == 0) {
      num_cols = strlen(grid[num_rows]);
    }
    num_rows++;
  }
  fclose(fptr);

  int start_x = -1, start_y = -1;

  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      if (grid[i][j] == 'S') {
        start_x = j;
        start_y = i;
        break;
      }
    }
    if (start_y != -1)
      break;
  }
  // BFS
  Point start = {start_x, start_y};
  enqueue(start);
  visited[start_y][start_x] = 1;
  int loop_count = 0;

  while (!is_queue_empty()) {
    Point current = dequeue();
    loop_count++;
    int x = current.x;
    int y = current.y;
    char current_char = grid[y][x];
    // 1. check up
    if (y > 0 && !visited[y - 1][x] && strchr("S|JL", current_char) &&
        strchr("|7F", grid[y - 1][x])) {
      visited[y - 1][x] = 1;
      Point next = {x, y - 1};
      enqueue(next);
    }
    // 2. check down
    if (y < num_rows - 1 && !visited[y + 1][x] &&
        strchr("S|7F", current_char) && strchr("|JL", grid[y + 1][x])) {
      visited[y + 1][x] = 1;
      Point next = {x, y + 1};
      enqueue(next);
    }
    // 3. check left
    if (x > 0 && !visited[y][x - 1] && strchr("S-J7", current_char) &&
        strchr("-LF", grid[y][x - 1])) {
      visited[y][x - 1] = 1;
      Point next = {x - 1, y};
      enqueue(next);
    }
    // 4. check right
    if (x < num_cols - 1 && !visited[y][x + 1] &&
        strchr("S-LF", current_char) && strchr("-J7", grid[y][x + 1])) {
      visited[y][x + 1] = 1;
      Point next = {x + 1, y};
      enqueue(next);
    }
  }

  printf("Number of loops: %d\n", loop_count / 2);
  return 0;
}
