
#include "string_split.h"
#include <stdio.h>
#include <string.h>

// Helper function to count occurrences of the delimiter.
// This helps in allocating the correct amount of memory for the result array.
static size_t count_tokens(const char *str, const char *delimiter) {
  if (!str || !delimiter || *delimiter == '\0') {
    return 0;
  }

  size_t count = 0;
  const char *tmp = str;
  while ((tmp = strstr(tmp, delimiter))) {
    count++;
    tmp += strlen(delimiter);
  }
  // The number of tokens is one more than the number of delimiters.
  return count + 1;
}

char **string_split(const char *str, const char *delimiter, size_t *count) {
  // --- Input Validation ---
  if (str == NULL || delimiter == NULL || count == NULL) {
    if (count)
      *count = 0;
    return NULL;
  }

  // --- Create a mutable copy of the input string ---
  // strtok modifies the string it processes, so we must work on a copy.
  char *str_copy = strdup(str);
  if (str_copy == NULL) {
    *count = 0;
    perror("Failed to duplicate string");
    return NULL;
  }

  // --- Pre-calculate the number of tokens ---
  size_t token_count = count_tokens(str, delimiter);
  if (token_count == 0) {
    *count = 0;
    free(str_copy);
    return NULL;
  }

  // --- Allocate memory for the array of string pointers ---
  // We allocate one extra pointer for the NULL terminator at the end of the
  // array.
  char **result = (char **)malloc(sizeof(char *) * (token_count + 1));
  if (result == NULL) {
    *count = 0;
    free(str_copy);
    perror("Failed to allocate memory for result array");
    return NULL;
  }

  // --- Tokenize the string and populate the result array ---
  char *token = strtok(str_copy, delimiter);
  size_t current_index = 0;

  while (token != NULL && current_index < token_count) {
    // Allocate memory for the token and copy it.
    result[current_index] = strdup(token);
    if (result[current_index] == NULL) {
      perror("Failed to duplicate token");
      // On failure, free all previously allocated memory.
      for (size_t i = 0; i < current_index; ++i) {
        free(result[i]);
      }
      free(result);
      free(str_copy);
      *count = 0;
      return NULL;
    }
    current_index++;
    // Get the next token.
    token = strtok(NULL, delimiter);
  }

  // --- Finalize the array ---
  result[current_index] = NULL; // Null-terminate the array of strings.
  *count = current_index;       // Set the final count.

  // --- Cleanup and Return ---
  free(str_copy); // Free the initial copy of the string.
  return result;
}

void free_split_string_array(char **arr, size_t count) {
  if (arr == NULL) {
    return;
  }
  for (size_t i = 0; i < count; ++i) {
    free(arr[i]); // Free each individual string.
  }
  free(arr); // Free the array of pointers.
}
