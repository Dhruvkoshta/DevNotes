#include "./lib/priority_queue.h"
#include <stdio.h>
#include <stdlib.h> // Required for malloc, calloc, and free
#include <string.h>

#define MAX_DIM 150

// PART 2 CHANGE: Define new constants for the ultra crucible rules.
#define MIN_STEPS_PART2 4
#define MAX_STEPS_PART2 10
#define MAX_DIRECTIONS 4

// PART 2 CHANGE: Macro for a heap-allocated 1D array to prevent stack overflow.
// The size of the steps dimension is MAX_STEPS_PART2 + 1 to hold indices 1-10.
#define VISITED_INDEX(r, c, dir, steps)                                        \
  ((r) * MAX_DIM * MAX_DIRECTIONS * (MAX_STEPS_PART2 + 1) +                    \
   (c) * MAX_DIRECTIONS * (MAX_STEPS_PART2 + 1) +                              \
   (dir) * (MAX_STEPS_PART2 + 1) + (steps))

typedef struct {
  int dx;
  int dy;
} Direction;

typedef struct {
  int heatloss;
  int row;
  int col;
  int direction;
  int steps;
} State;

// Helper function to check if coordinates are within the grid bounds
int isValidCoor(int r, int c, int num_rows, int num_cols) {
  return r >= 0 && r < num_rows && c >= 0 && c < num_cols;
}

int main(int argc, char *argv[]) {
  char grid[MAX_DIM][MAX_DIM];

  // PART 2 CHANGE: Allocate the large visited array on the heap.
  size_t visited_size =
      MAX_DIM * MAX_DIM * MAX_DIRECTIONS * (MAX_STEPS_PART2 + 1);
  char *visited = calloc(visited_size, sizeof(char));
  if (visited == NULL) {
    perror("Failed to allocate memory for visited array");
    return 1;
  }

  int num_rows = 0, num_cols = 0;
  PriorityQueue *pq =
      pq_create(4000000); // Increased capacity slightly for part 2

  Direction directions[4] = {
      {-1, 0}, // 0: UP
      {0, 1},  // 1: RIGHT
      {1, 0},  // 2: DOWN
      {0, -1}  // 3: LEFT
  };

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    free(visited);
    return 1;
  }

  FILE *fptr = fopen(argv[1], "r");
  if (fptr == NULL) {
    perror("Error opening file");
    free(visited);
    return 1;
  }

  while (fgets(grid[num_rows], MAX_DIM, fptr) != NULL) {
    grid[num_rows][strcspn(grid[num_rows], "\n")] = '\0';
    if (num_cols == 0) {
      num_cols = strlen(grid[num_rows]);
    }
    num_rows++;
  }
  fclose(fptr);

  // -- Initialization --
  // The starting logic remains the same.
  if (isValidCoor(0, 1, num_rows, num_cols)) {
    State *start_state_r = calloc(1, sizeof(State));
    if (start_state_r) {
      start_state_r->heatloss = grid[0][1] - '0';
      start_state_r->row = 0;
      start_state_r->col = 1;
      start_state_r->direction = 1; // RIGHT
      start_state_r->steps = 1;
      pq_push(pq, start_state_r, start_state_r->heatloss);
    }
  }
  if (isValidCoor(1, 0, num_rows, num_cols)) {
    State *start_state_d = calloc(1, sizeof(State));
    if (start_state_d) {
      start_state_d->heatloss = grid[1][0] - '0';
      start_state_d->row = 1;
      start_state_d->col = 0;
      start_state_d->direction = 2; // DOWN
      start_state_d->steps = 1;
      pq_push(pq, start_state_d, start_state_d->heatloss);
    }
  }

  while (!pq_is_empty(pq)) {
    State *current = (State *)pq_pop(pq);
    if (current == NULL)
      continue;

    // PART 2 CHANGE: The goal is only valid if the crucible has moved at least
    // the minimum number of steps.
    if (current->row == num_rows - 1 && current->col == num_cols - 1 &&
        current->steps >= MIN_STEPS_PART2) {
      printf("Minimum heat loss (Part 2): %d\n", current->heatloss);
      free(current);
      break;
    }

    size_t idx = VISITED_INDEX(current->row, current->col, current->direction,
                               current->steps);
    if (visited[idx]) {
      free(current);
      continue;
    }
    visited[idx] = 1;

    // -- 1. Continue Straight --
    // PART 2 CHANGE: The crucible can move up to 10 steps straight.
    if (current->steps < MAX_STEPS_PART2) {
      int new_row = current->row + directions[current->direction].dx;
      int new_col = current->col + directions[current->direction].dy;

      if (isValidCoor(new_row, new_col, num_rows, num_cols)) {
        State *next_state = calloc(1, sizeof(State));
        if (next_state) {
          next_state->heatloss =
              current->heatloss + (grid[new_row][new_col] - '0');
          next_state->row = new_row;
          next_state->col = new_col;
          next_state->direction = current->direction;
          next_state->steps = current->steps + 1;
          pq_push(pq, next_state, next_state->heatloss);
        }
      }
    }

    // -- 2. Turn Left & Right --
    // PART 2 CHANGE: The crucible can only turn AFTER moving at least 4 steps.
    if (current->steps >= MIN_STEPS_PART2) {
      for (int i = 0; i < 4; i++) {
        if (i == current->direction || i == (current->direction + 2) % 4) {
          continue;
        }

        int new_row = current->row + directions[i].dx;
        int new_col = current->col + directions[i].dy;

        if (isValidCoor(new_row, new_col, num_rows, num_cols)) {
          State *next_state = calloc(1, sizeof(State));
          if (next_state) {
            next_state->heatloss =
                current->heatloss + (grid[new_row][new_col] - '0');
            next_state->row = new_row;
            next_state->col = new_col;
            next_state->direction = i;
            next_state->steps = 1; // Steps reset to 1 after a turn
            pq_push(pq, next_state, next_state->heatloss);
          }
        }
      }
    }
    free(current);
  }

  while (!pq_is_empty(pq)) {
    free(pq_pop(pq));
  }
  pq_destroy(pq);
  free(visited); // Free the heap-allocated visited array

  return 0;
}
