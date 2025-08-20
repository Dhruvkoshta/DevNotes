#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 150
#define MAX_GALAXIES 500

typedef struct {
  long long r;
  long long c;
} Point;

long long solve(const char grid[MAX_LINES][MAX_LINES], int rows, int cols) {
  Point original_galaxies[MAX_GALAXIES];
  int num_galax = 0;
  // 1
  bool empty_row[MAX_LINES] = {false};
  bool empty_col[MAX_LINES] = {false};
  for (int i = 0; i < rows; i++)
    empty_row[i] = true;
  for (int i = 0; i < cols; i++)
    empty_col[i] = true;

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (grid[r][c] == '#') {
        original_galaxies[num_galax++] = (Point){r, c};
        empty_row[r] = false;
        empty_col[c] = false;
      }
    }
  }
  // 2
  Point expanded_galaxies[MAX_GALAXIES];
  for (int i = 0; i < num_galax; i++) {
    long long row_offset = 0;
    long long col_offset = 0;

    for (int r = 0; r < original_galaxies[i].r; r++) {
      if (empty_row[r]) {
        row_offset++;
      }
    }

    for (int c = 0; c < original_galaxies[i].c; c++) {
      if (empty_col[c]) {
        col_offset++;
      }
    }

    expanded_galaxies[i].r =
        original_galaxies[i].r + row_offset * (1000000 - 1);
    expanded_galaxies[i].c =
        original_galaxies[i].c + col_offset * (1000000 - 1);
  }

  // 3
  long long total = 0;

  for (int i = 0; i < num_galax; i++) {
    for (int j = i + 1; j < num_galax; j++) {
      long long dist = llabs(expanded_galaxies[i].r - expanded_galaxies[j].r) +
                       llabs(expanded_galaxies[i].c - expanded_galaxies[j].c);
      total += dist;
    }
  }

  return total;
}

int main(int argc, char *argv[]) {
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

  char grid[MAX_LINES][MAX_LINES];
  int rows = 0, cols = 0;

  while (rows < MAX_LINES && fgets(grid[rows], MAX_LINES, fptr) != NULL) {
    grid[rows][strcspn(grid[rows], "\n")] = 0;

    if (cols == 0) {
      cols = strlen(grid[rows]);
    }
    rows++;
  }

  fclose(fptr);

  long long total = solve(grid, rows, cols);

  printf("Total distance: %lld\n", total);
}
