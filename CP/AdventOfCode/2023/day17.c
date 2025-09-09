#include "./lib/priority_queue.h"
#include <stdio.h>
#include <stdlib.h> // Required for malloc, calloc, and free
#include <string.h>

#define MAX_DIM 150 // Max grid dimension for AoC inputs
#define MAX_STEPS 4 // To store steps 1, 2, and 3 (indices 1-3)

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
  // For simplicity and safety, declare the visited array on the stack.
  // It's automatically zero-initialized.
  char visited[MAX_DIM][MAX_DIM][4][MAX_STEPS] = {0};

  int num_rows = 0, num_cols = 0;

  // Use a reasonable capacity to avoid excessive memory allocation.
  PriorityQueue *pq = pq_create(2000000);

  Direction directions[4] = {
      {-1, 0}, // 0: UP
      {0, 1},  // 1: RIGHT
      {1, 0},  // 2: DOWN
      {0, -1}  // 3: LEFT
  };

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
    return 1;
  }

  FILE *fptr = fopen(argv[1], "r");
  if (fptr == NULL) {
    perror("Error opening file");
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
  // Move Right from start
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
  // Move Down from start
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

    if (current->row == num_rows - 1 && current->col == num_cols - 1) {
      printf("Minimum heat loss: %d\n", current->heatloss);
      free(current); // Free the final state
      break;
    }

    if (visited[current->row][current->col][current->direction]
               [current->steps]) {
      free(current);
      continue;
    }
    visited[current->row][current->col][current->direction][current->steps] = 1;

    // -- 1. Continue Straight --
    if (current->steps < 3) {
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

    free(current); // Free the memory for the state we just finished processing
  }

  // Clean up any remaining items in the PQ to prevent memory leaks
  while (!pq_is_empty(pq)) {
    free(pq_pop(pq));
  }
  pq_destroy(pq);

  return 0;
}
