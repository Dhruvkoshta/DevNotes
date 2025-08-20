#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct to hold the key and the two values
typedef struct {
  char key[4];
  char left[4];
  char right[4];
} Node;

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

  char line[1028];
  Node *map = NULL;
  int num_nodes = 0;

  // Read and store the direction line (first line)
  char *token = fgets(line, sizeof(line), fptr);
  if (token) {
    line[strcspn(line, "\n")] = 0; // Remove trailing newline
    printf("Direction: %s\n", line);
  }
  char *direction = malloc(1024 * sizeof(char));
  strcpy(direction, token);

  // Skip the empty line
  fgets(line, sizeof(line), fptr);

  // Loop through the rest of the lines and parse them
  while (fgets(line, sizeof(line), fptr)) {
    if (line[0] == '\n') {
      continue; // Skip any empty lines
    }

    // Dynamically resize the array to store the new node
    map = realloc(map, sizeof(Node) * (num_nodes + 1));
    if (map == NULL) {
      perror("Memory allocation failed");
      fclose(fptr);
      // Free any previously allocated memory
      for (int i = 0; i < num_nodes; i++) {
        free(map);
      }
      return 1;
    }

    // Use a comprehensive set of delimiters
    char *key_token = strtok(line, " =(),\n");
    char *left_token = strtok(NULL, " =(),\n");
    char *right_token = strtok(NULL, " =(),\n");

    // Ensure all three parts are found before storing
    if (key_token && left_token && right_token) {
      strcpy(map[num_nodes].key, key_token);
      strcpy(map[num_nodes].left, left_token);
      strcpy(map[num_nodes].right, right_token);
      num_nodes++;
    }
  }

  char *current = "AAA";
  int j = 0;
  int steps = 0;
  // char currentDir = direction[j];
  // printf("%c\n", currentDir);
  while (strcmp(current, "ZZZ") != 0) {
    for (int i = 0; i < num_nodes; i++) {
      if (strcmp(map[i].key, current) == 0) {

        if (j >= strlen(direction)) {
          j = 0;
        }
        char currentDir = direction[j];

        if (currentDir == 'L') {
          current = map[i].left;
        } else if (currentDir == 'R') {
          current = map[i].right;
        }
        j++;
        steps++;
        printf("%d\n", steps);
        break;
      }
    }
  }
  printf("Steps: %d\n", steps);
  fclose(fptr);

  // Clean up dynamically allocated memory
  free(map);

  return 0;
}
