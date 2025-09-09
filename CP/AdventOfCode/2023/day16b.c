
#include <stdio.h>
#include <string.h>

#define MAX_DIM 200
#define DIR_RIGHT 1 // 0001
#define DIR_LEFT 2  // 0010
#define DIR_UP 4    // 0100
#define DIR_DOWN 8  // 1000

typedef struct {
  int r; // row
  int c; // column
  int dir;
} Beam;
// QUEUE IMPLIMENTATION

Beam queue[MAX_DIM * MAX_DIM * 4];
int front = 0;
int rear = -1;
int queue_size = 0;

void enqueue(Beam p) {
  rear = (rear + 1) % (MAX_DIM * MAX_DIM * 4);
  queue[rear] = p;
  queue_size++;
}

Beam dequeue() {
  Beam p = queue[front];
  front = (front + 1) % (MAX_DIM * MAX_DIM * 4);
  queue_size--;
  return p;
}
int is_queue_empty() { return queue_size == 0; }

int calculate_energized(Beam start_beam, int num_rows, int num_cols,
                        char grid[MAX_DIM][MAX_DIM]) {
  // State for a single run
  int visited[MAX_DIM][MAX_DIM] = {0};
  int energized[MAX_DIM][MAX_DIM] = {0};

  // Reset queue for each run
  front = 0;
  rear = -1;
  queue_size = 0;

  // Start with the initial beam
  enqueue(start_beam);
  visited[start_beam.r][start_beam.c] |= start_beam.dir;

  while (!is_queue_empty()) {
    Beam current = dequeue();
    energized[current.r][current.c] = 1;

    Beam next_beams[2];
    int num_next_beams = 0;
    char cell = grid[current.r][current.c];

    if (cell == '.') {
      next_beams[0] = (Beam){current.r, current.c, current.dir};
      num_next_beams = 1;
    } else if (cell == '/') {
      if (current.dir == DIR_RIGHT)
        next_beams[0].dir = DIR_UP;
      else if (current.dir == DIR_LEFT)
        next_beams[0].dir = DIR_DOWN;
      else if (current.dir == DIR_UP)
        next_beams[0].dir = DIR_RIGHT;
      else if (current.dir == DIR_DOWN)
        next_beams[0].dir = DIR_LEFT;
      next_beams[0].r = current.r;
      next_beams[0].c = current.c;
      num_next_beams = 1;
    } else if (cell == '\\') {
      if (current.dir == DIR_RIGHT)
        next_beams[0].dir = DIR_DOWN;
      else if (current.dir == DIR_LEFT)
        next_beams[0].dir = DIR_UP;
      else if (current.dir == DIR_UP)
        next_beams[0].dir = DIR_LEFT;
      else if (current.dir == DIR_DOWN)
        next_beams[0].dir = DIR_RIGHT;
      next_beams[0].r = current.r;
      next_beams[0].c = current.c;
      num_next_beams = 1;
    } else if (cell == '|') {
      if (current.dir == DIR_UP || current.dir == DIR_DOWN) {
        next_beams[0] = (Beam){current.r, current.c, current.dir};
        num_next_beams = 1;
      } else {
        next_beams[0] = (Beam){current.r, current.c, DIR_UP};
        next_beams[1] = (Beam){current.r, current.c, DIR_DOWN};
        num_next_beams = 2;
      }
    } else if (cell == '-') {
      if (current.dir == DIR_LEFT || current.dir == DIR_RIGHT) {
        next_beams[0] = (Beam){current.r, current.c, current.dir};
        num_next_beams = 1;
      } else {
        next_beams[0] = (Beam){current.r, current.c, DIR_LEFT};
        next_beams[1] = (Beam){current.r, current.c, DIR_RIGHT};
        num_next_beams = 2;
      }
    }

    for (int i = 0; i < num_next_beams; i++) {
      Beam next = next_beams[i];
      if (next.dir == DIR_RIGHT)
        next.c++;
      else if (next.dir == DIR_LEFT)
        next.c--;
      else if (next.dir == DIR_UP)
        next.r--;
      else if (next.dir == DIR_DOWN)
        next.r++;

      if (next.r >= 0 && next.r < num_rows && next.c >= 0 &&
          next.c < num_cols) {
        if (!(visited[next.r][next.c] & next.dir)) {
          visited[next.r][next.c] |= next.dir;
          enqueue(next);
        }
      }
    }
  }

  int total_energized = 0;
  for (int r = 0; r < num_rows; r++) {
    for (int c = 0; c < num_cols; c++) {
      if (energized[r][c]) {
        total_energized++;
      }
    }
  }
  return total_energized;
}

int main(int argc, char *argv[]) {
  char grid[MAX_DIM][MAX_DIM];
  int visited[MAX_DIM][MAX_DIM] = {0};
  int energized[MAX_DIM][MAX_DIM] = {0};
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

  int max_energized = 0;

#define UPDATE_MAX(new_val)                                                    \
  if ((new_val) > max_energized)                                               \
  max_energized = (new_val)

  for (int c = 0; c < num_cols; c++) {
    Beam start = {0, c, DIR_DOWN};
    int current_energized =
        calculate_energized(start, num_rows, num_cols, grid);
    UPDATE_MAX(current_energized);
  }

  // 2. Bottom Edge (moving up)
  for (int c = 0; c < num_cols; c++) {
    Beam start = {num_rows - 1, c, DIR_UP};
    int current_energized =
        calculate_energized(start, num_rows, num_cols, grid);
    UPDATE_MAX(current_energized);
  }

  // 3. Left Edge (moving right)
  for (int r = 0; r < num_rows; r++) {
    Beam start = {r, 0, DIR_RIGHT};
    int current_energized =
        calculate_energized(start, num_rows, num_cols, grid);
    UPDATE_MAX(current_energized);
  }

  // 4. Right Edge (moving left)
  for (int r = 0; r < num_rows; r++) {
    Beam start = {r, num_cols - 1, DIR_LEFT};
    int current_energized =
        calculate_energized(start, num_rows, num_cols, grid);
    UPDATE_MAX(current_energized);
  }

  printf("Maximum energized tiles: %d\n", max_energized);

  return 0;
}
