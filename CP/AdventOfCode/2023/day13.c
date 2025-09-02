#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char **grid;
  int width;
  int height;
} Pattern;

int find_horiz_mirror(const Pattern *p);
Pattern transpose_grid(const Pattern *p);
void free_pattern(Pattern *p);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
    return 1;
  }
  FILE *f = fopen(argv[1], "r");
  if (!f) {
    perror("fopen");
    return 1;
  }

  long long total_summary = 0;
  char line[256];
  char **current_grid_line = NULL;
  int current_height = 0;

  while (fgets(line, sizeof(line), f) != NULL) {
    if (strcmp(line, "\n") == 0) {
      if (current_height > 0) {
        Pattern p = {current_grid_line, strlen(current_grid_line[0]),
                     current_height};
        int horizontal_row = find_horiz_mirror(&p);
        if (horizontal_row > 0) {
          total_summary += horizontal_row * 100;
        } else {
          Pattern transposed = transpose_grid(&p);
          int vertical_col = find_horiz_mirror(&transposed);
          if (vertical_col > 0) {
            total_summary += vertical_col;
          }
          free_pattern(&transposed);
        }
        free_pattern(&p);
        current_grid_line = NULL;
        current_height = 0;
      }
    } else {
      line[strcspn(line, "\n")] = 0; // Remove newline
      current_height++;
      current_grid_line =
          realloc(current_grid_line, current_height * sizeof(char *));
      current_grid_line[current_height - 1] =
          strdup(line); // strdup to allocate memory and copy the line
    }
  }
  fclose(f);
  printf("Total Summary: %lld\n", total_summary);
  return 0;
}

int find_horiz_mirror(const Pattern *p) {
  for (int i = 0; i < p->height - 1; i++) {
    int top_ptr = i;
    int bottom_ptr = i + 1;
    int is_mirror = 1;

    while (top_ptr >= 0 && bottom_ptr < p->height) {
      if (strcmp(p->grid[top_ptr], p->grid[bottom_ptr]) != 0) {
        is_mirror = 0;
        break;
      }
      top_ptr--;
      bottom_ptr++;
    }
    if (is_mirror) {
      return i + 1; // Return 1-based index
    }
  }

  return 0;
}

Pattern transpose_grid(const Pattern *p) {
  Pattern transposed;
  transposed.width = p->height;
  transposed.height = p->width;
  transposed.grid = malloc(transposed.height * sizeof(char *));

  for (int i = 0; i < transposed.height; i++) {
    transposed.grid[i] = malloc((transposed.width + 1) * sizeof(char));
    for (int j = 0; j < transposed.width; j++) {
      transposed.grid[i][j] = p->grid[j][i];
    }
    transposed.grid[i][transposed.width] = '\0'; // Null-terminate the string
  }
  return transposed;
}

void free_pattern(Pattern *p) {
  for (int i = 0; i < p->height; i++) {
    free(p->grid[i]);
  }
  free(p->grid);
  p->grid = NULL;
  p->width = 0;
  p->height = 0;
}
