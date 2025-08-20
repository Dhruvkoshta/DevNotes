#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct to hold the key and the two values
typedef struct {
  char key[4];
  char left[4];
  char right[4];
} Node;

// FIND NODE
Node *find_node(Node *map, int num_nodes, const char *key) {
  for (int i = 0; i < num_nodes; i++) {
    if (strcmp(map[i].key, key) == 0) {
      return &map[i];
    }
  }
  return NULL;
}

long long gcd(long long a, long long b) {
  while (b) {
    long long temp = a % b;
    a = b;
    b = temp;
  }
  return a;
}

long long lcm(long long a, long long b) {
  if (a == 0 || b == 0)
    return 0;
  return (a * b) / gcd(a, b);
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

  char line[1028];
  Node *map = NULL;
  int num_nodes = 0;
  char direction[1024];

  if (fgets(line, sizeof(line), fptr)) {
    line[strcspn(line, "\n")] = 0;
    strcpy(direction, line);
  } else {
    fclose(fptr);
    return 1;
  }

  fgets(line, sizeof(line), fptr);

  // Loop through the rest of the lines and parse them
  while (fgets(line, sizeof(line), fptr)) {
    if (line[0] == '\n')
      continue;

    map = realloc(map, sizeof(Node) * (num_nodes + 1));
    if (map == NULL) {
      perror("Memory allocation failed");
      fclose(fptr);
      free(map);
      return 1;
    }

    char *key_token = strtok(line, " =(),\n");
    char *left_token = strtok(NULL, " =(),\n");
    char *right_token = strtok(NULL, " =(),\n");

    if (key_token && left_token && right_token) {
      strcpy(map[num_nodes].key, key_token);
      strcpy(map[num_nodes].left, left_token);
      strcpy(map[num_nodes].right, right_token);
      num_nodes++;
    }
  }
  fclose(fptr);

  // 1. Find all starting nodes (ending with 'A')
  const char *start_nodes[num_nodes];
  int num_start_nodes = 0;
  for (int i = 0; i < num_nodes; i++) {
    if (map[i].key[2] == 'A') {
      start_nodes[num_start_nodes++] = map[i].key;
    }
  }

  long long steps_to_z[num_start_nodes];
  int direction_length = strlen(direction);

  // 2. Calculate steps for each path to reach a 'Z' ending node
  for (int i = 0; i < num_start_nodes; i++) {
    const char *current_key = start_nodes[i];
    long long steps = 0;

    while (current_key[2] != 'Z') {
      Node *current_node = find_node(map, num_nodes, current_key);
      if (!current_node) {
        fprintf(stderr, "Error: Node '%s' not found!\n", current_key);
        break;
      }

      char current_dir = direction[steps % direction_length];

      if (current_dir == 'L') {
        current_key = current_node->left;
      } else if (current_dir == 'R') {
        current_key = current_node->right;
      }
      steps++;
    }
    steps_to_z[i] = steps;
  }

  // 3. Find the LCM of all path lengths
  long long total_steps = steps_to_z[0];
  for (int i = 1; i < num_start_nodes; i++) {
    total_steps = lcm(total_steps, steps_to_z[i]);
  }

  printf("Total steps to reach all 'Z' nodes simultaneously: %lld\n",
         total_steps);

  free(map);

  return 0;
}
