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

  Beam start = {0, 0, DIR_RIGHT};
  enqueue(start);
  visited[0][0] |= DIR_RIGHT;

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
      Beam nb = next_beams[i];
      if (nb.dir == DIR_RIGHT) {
        nb.c++;
      } else if (nb.dir == DIR_LEFT) {
        nb.c--;
      } else if (nb.dir == DIR_UP) {
        nb.r--;
      } else if (nb.dir == DIR_DOWN) {
        nb.r++;
      }
      if (nb.r >= 0 && nb.r < num_rows && nb.c >= 0 && nb.c < num_cols) {
        if (!(visited[nb.r][nb.c] & nb.dir)) {
          visited[nb.r][nb.c] |= nb.dir;
          enqueue(nb);
        }
      }
    }
  }

  int total = 0;
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      if (energized[i][j])
        total++;
    }
  }
  printf("Total energized cells: %d\n", total);
}
