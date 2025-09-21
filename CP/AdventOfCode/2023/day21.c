#include <stdio.h>
#include <string.h>

#define MAX_DIM 150
#define TARGET_STEPS 64

// A state in our BFS: position (r, c) and steps taken.
typedef struct {
  int r;
  int c;
  int steps;
} State;

// --- QUEUE IMPLEMENTATION ---
State queue[MAX_DIM * MAX_DIM];
int front = 0;
int rear = -1;
int queue_size = 0;

void enqueue(State s) {
  rear = (rear + 1) % (MAX_DIM * MAX_DIM);
  queue[rear] = s;
  queue_size++;
}

State dequeue() {
  State s = queue[front];
  front = (front + 1) % (MAX_DIM * MAX_DIM);
  queue_size--;
  return s;
}

int is_queue_empty() { return queue_size == 0; }
// --- END QUEUE ---

int main(int argc, char *argv[]) {
  char grid[MAX_DIM][MAX_DIM];
  // visited array to store the shortest distance to each cell.
  int visited[MAX_DIM][MAX_DIM];
  int num_rows = 0, num_cols = 0;
  int start_r = -1, start_c = -1;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  FILE *fptr = fopen(argv[1], "r");
  if (fptr == NULL) {
    perror("Error opening file");
    return 1;
  }

  // Initialize visited array with -1 (unvisited)
  for (int i = 0; i < MAX_DIM; i++) {
    for (int j = 0; j < MAX_DIM; j++) {
      visited[i][j] = -1;
    }
  }

  while (fgets(grid[num_rows], MAX_DIM, fptr) != NULL) {
    grid[num_rows][strcspn(grid[num_rows], "\n")] = '\0';

    char *s_ptr = strchr(grid[num_rows], 'S');
    if (s_ptr != NULL) {
      start_r = num_rows;
      start_c = s_ptr - grid[num_rows];
    }

    if (num_cols == 0) {
      num_cols = strlen(grid[num_rows]);
    }
    num_rows++;
  }
  fclose(fptr);

  // Start BFS from the 'S' position
  State start = {start_r, start_c, 0};
  enqueue(start);
  visited[start_r][start_c] = 0;

  int dr[] = {-1, 1, 0, 0}; // Up, Down
  int dc[] = {0, 0, -1, 1}; // Left, Right

  while (!is_queue_empty()) {
    State current = dequeue();

    // Stop exploring from this path if we've reached the step limit
    if (current.steps >= TARGET_STEPS) {
      continue;
    }

    // Explore neighbors
    for (int i = 0; i < 4; i++) {
      int next_r = current.r + dr[i];
      int next_c = current.c + dc[i];

      // Check boundaries, if it's not a rock, and if it's unvisited
      if (next_r >= 0 && next_r < num_rows && next_c >= 0 &&
          next_c < num_cols && grid[next_r][next_c] != '#' &&
          visited[next_r][next_c] == -1) {

        visited[next_r][next_c] = current.steps + 1;
        State next_state = {next_r, next_c, current.steps + 1};
        enqueue(next_state);
      }
    }
  }

  // Count plots with the correct parity
  long total = 0;
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      // A plot is reachable if:
      // 1. It has been visited (distance != -1)
      // 2. Its distance is <= the target steps
      // 3. Its distance has the same parity (even/odd) as the target steps
      if (visited[i][j] != -1 && visited[i][j] <= TARGET_STEPS &&
          visited[i][j] % 2 == TARGET_STEPS % 2) {
        total++;
      }
    }
  }

  printf("Total reachable plots in %d steps: %ld\n", TARGET_STEPS, total);
  return 0;
}
