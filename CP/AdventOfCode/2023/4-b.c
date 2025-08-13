#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countMatches(int *winning_numbers, int winning_count, int *my_numbers,
                 int my_count);

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

  char ch;
  int lineCount = 0;
  while ((ch = fgetc(fptr)) != EOF) {
    if (ch == '\n') {
      lineCount++;
    }
  }
  rewind(fptr); // Rewind file pointer

  int cardCopies[lineCount];
  for (int i = 0; i < lineCount; i++) {
    cardCopies[i] = 1; // Initialize with 1 copy
  }

  char line[1028];

  while (fgets(line, sizeof(line), fptr)) {
    line[strcspn(line, "\n")] = 0; // Remove newline

    char *line_copy = strdup(line); // Copy for strtok
    if (!line_copy) {
      perror("Memory allocation failed");
      fclose(fptr);
      return 1;
    }

    char *card_and_winning_str = strtok(line_copy, "|");
    char *my_numbers_str_raw = strtok(NULL, "|");

    char *card_part = strtok(card_and_winning_str, ":");
    char *winning_numbers_str_raw = strtok(NULL, ":");

    int cardIndex = -1;
    if (card_part != NULL) {
      char *index_str = strchr(card_part, ' ');
      if (index_str != NULL && *(index_str + 1) != '\0') {
        index_str++;
        cardIndex = atoi(index_str);
      }
    }

    if (cardIndex == -1 || cardIndex < 1 || cardIndex > lineCount) {
      fprintf(stderr, "Error: Invalid card index %d\n", cardIndex);
      free(line_copy);
      fclose(fptr);
      return 1;
    }
    int currentCardArrayIndex = cardIndex - 1; // 0-based index

    int winning_numbers_arr[50];
    int my_numbers_arr[50];
    int winning_count = 0;
    int my_count = 0;

    // Copies for independent strtok parsing
    char *winning_numbers_copy_for_parsing = NULL;
    if (winning_numbers_str_raw != NULL) {
      winning_numbers_copy_for_parsing = strdup(winning_numbers_str_raw);
      if (!winning_numbers_copy_for_parsing) {
        perror("Memory allocation failed");
        free(line_copy);
        fclose(fptr);
        return 1;
      }
    }

    char *my_numbers_copy_for_parsing = NULL;
    if (my_numbers_str_raw != NULL) {
      my_numbers_copy_for_parsing = strdup(my_numbers_str_raw);
      if (!my_numbers_copy_for_parsing) {
        perror("Memory allocation failed");
        free(line_copy);
        free(winning_numbers_copy_for_parsing);
        fclose(fptr);
        return 1;
      }
    }

    char *token = strtok(winning_numbers_copy_for_parsing, " ");
    while (token != NULL && winning_count < 50) {
      if (strlen(token) > 0) {
        winning_numbers_arr[winning_count++] = atoi(token);
      }
      token = strtok(NULL, " ");
    }

    token = strtok(my_numbers_copy_for_parsing, " ");
    while (token != NULL && my_count < 50) {
      if (strlen(token) > 0) {
        my_numbers_arr[my_count++] = atoi(token);
      }
      token = strtok(NULL, " ");
    }

    int matches = countMatches(winning_numbers_arr, winning_count,
                               my_numbers_arr, my_count);

    int num_current_card_copies = cardCopies[currentCardArrayIndex];

    for (int i = 1; i <= matches; i++) {
      if (currentCardArrayIndex + i < lineCount) {
        cardCopies[currentCardArrayIndex + i] += num_current_card_copies;
      }
    }

    // Free duplicated strings
    free(line_copy);
    free(winning_numbers_copy_for_parsing);
    free(my_numbers_copy_for_parsing);
  }
  fclose(fptr);

  int total_cards_part2 = 0;
  for (int i = 0; i < lineCount; i++) {
    total_cards_part2 += cardCopies[i];
  }
  printf("Total : %d\n", total_cards_part2);

  return 0;
}

int countMatches(int *winning_numbers, int winning_count, int *my_numbers,
                 int my_count) {
  int matches = 0;
  for (int i = 0; i < winning_count; i++) {
    for (int j = 0; j < my_count; j++) {
      if (winning_numbers[i] == my_numbers[j]) {
        matches++;
      }
    }
  }
  return matches;
}
