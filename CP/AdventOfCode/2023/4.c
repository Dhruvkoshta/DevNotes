#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  int total_points = 0;

  while (fgets(line, sizeof(line), fptr)) {
    line[strcspn(line, "\n")] = 0;

    char *line_copy = strdup(line); // Create a copy for strtok
    if (!line_copy) {
      // Handle memory allocation error
      perror("Memory allocation failed");
      fclose(fptr);
      return 1;
    }

    // Split the line into winning and my numbers strings
    char *winning_numbers_str = strtok(line_copy, "|");
    char *my_numbers_str = strtok(NULL, "|");

    // Skip "Card X:" part
    strtok(winning_numbers_str, ":");
    winning_numbers_str = strtok(NULL, ":");

    // Arrays to hold the numbers (assuming max 20 numbers for safety)
    int winning_numbers[50];
    int my_numbers[50];
    int winning_count = 0;
    int my_count = 0;

    // Populate winning_numbers array
    char *token = strtok(winning_numbers_str, " ");
    while (token != NULL) {
      winning_numbers[winning_count++] = atoi(token);
      token = strtok(NULL, " ");
    }

    // Populate my_numbers array
    token = strtok(my_numbers_str, " ");
    while (token != NULL) {
      my_numbers[my_count++] = atoi(token);
      token = strtok(NULL, " ");
    }

    int matches_per_card = 0;
    for (int i = 0; i < my_count; i++) {
      for (int j = 0; j < winning_count; j++) {
        if (my_numbers[i] == winning_numbers[j]) {
          matches_per_card++;
        }
      }
    }

    int card_points = 0;
    if (matches_per_card > 0) {
      // card_points = (int)pow(2, matches_per_card - 1);
      card_points = 1 << (matches_per_card - 1);
    }

    total_points += card_points;

    free(line_copy);
  }
  fclose(fptr);

  printf("Total points: %d\n", total_points);

  return 0;
}
