#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000000
#define NUM_BOXES 256
typedef struct {
  char *label;
  int focal_length;
} Lens;

typedef struct {
  Lens *lenses;
  int count;
} Box;

long long calculate_hash(const char *s) {
  long long current_value = 0;
  for (int i = 0; i < strlen(s); i++) {
    current_value += (int)s[i];
    current_value *= 17;
    current_value %= 256;
  }
  return current_value;
}
void cleanup(Box boxes[]);

int main() {
  FILE *file;
  char line[MAX_LINE_LENGTH];
  char *token;
  Box boxes[NUM_BOXES] = {0};
  long long total_focusing_power = 0;

  // Initialize boxes
  for (int i = 0; i < NUM_BOXES; i++) {
    boxes[i].lenses = NULL;
    boxes[i].count = 0;
  }

  file = fopen("input.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  if (fgets(line, sizeof(line), file) != NULL) {
    // Remove trailing newline character
    line[strcspn(line, "\n")] = 0;

    token = strtok(line, ",");
    while (token != NULL) {
      char *label;
      int focal_length = -1;
      char *op_pos;

      op_pos = strpbrk(token, "-=");

      // Extract the label
      label = malloc(op_pos - token + 1);
      strncpy(label, token, op_pos - token);
      label[op_pos - token] = '\0';

      int box_num = calculate_hash(label);

      if (*op_pos == '-') { // Remove operation
        int lens_index = -1;
        for (int i = 0; i < boxes[box_num].count; i++) {
          if (strcmp(boxes[box_num].lenses[i].label, label) == 0) {
            lens_index = i;
            break;
          }
        }

        if (lens_index != -1) {
          free(boxes[box_num].lenses[lens_index].label);
          for (int i = lens_index; i < boxes[box_num].count - 1; i++) {
            boxes[box_num].lenses[i] = boxes[box_num].lenses[i + 1];
          }
          boxes[box_num].count--;
          boxes[box_num].lenses = realloc(boxes[box_num].lenses,
                                          boxes[box_num].count * sizeof(Lens));
        }
        free(label); // Free the temp label since it was not stored
      } else if (*op_pos == '=') { // Add/Update operation
        focal_length = atoi(op_pos + 1);

        int lens_index = -1;
        for (int i = 0; i < boxes[box_num].count; i++) {
          if (strcmp(boxes[box_num].lenses[i].label, label) == 0) {
            lens_index = i;
            break;
          }
        }

        if (lens_index != -1) {
          boxes[box_num].lenses[lens_index].focal_length = focal_length;
          free(label);
        } else {
          boxes[box_num].count++;
          boxes[box_num].lenses = realloc(boxes[box_num].lenses,
                                          boxes[box_num].count * sizeof(Lens));
          boxes[box_num].lenses[boxes[box_num].count - 1].label = label;
          boxes[box_num].lenses[boxes[box_num].count - 1].focal_length =
              focal_length;
        }
      }
      token = strtok(NULL, ",");
    }
  }
  fclose(file);

  // Calculate the total focusing power
  for (int i = 0; i < NUM_BOXES; i++) {
    for (int j = 0; j < boxes[i].count; j++) {
      total_focusing_power +=
          (long long)(i + 1) * (j + 1) * boxes[i].lenses[j].focal_length;
    }
  }

  printf("Total Focusing Power: %lld\n", total_focusing_power);

  cleanup(boxes);
  return 0;
}

void cleanup(Box boxes[]) {
  for (int i = 0; i < NUM_BOXES; i++) {
    for (int j = 0; j < boxes[i].count; j++) {
      free(boxes[i].lenses[j].label);
    }
    if (boxes[i].lenses) {
      free(boxes[i].lenses);
    }
  }
}
