
### PART 1

```c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Define a struct to hold the maximum allowed cube counts for the entire game
typedef struct {
  int red;
  int green;
  int blue;
} CubeLimits;

// Define a struct to hold the current cube counts observed within a single set
typedef struct {
  int red;
  int green;
  int blue;
} CurrentSetCubes;

int process_set(char *set_str, const CubeLimits *limits) {
  char *color_token;
  char *rest_of_colors_ptr; // For strtok_r state

  // Initialize current cube counts for this set to zero
  CurrentSetCubes current_cubes = {0, 0, 0};

  // Tokenize the set string by commas to get individual color-count pairs
  color_token = strtok_r(set_str, ",", &rest_of_colors_ptr);
  while (color_token != NULL) {
    int count = 0;
    char color_name[10]; // To store "red", "blue", "green"

    // Trim leading whitespace from color_token (e.g., " 3 blue" -> "3 blue")
    while (*color_token == ' ') {
      color_token++;
    }

    // Parse the count and color name from the token
    if (sscanf(color_token, "%d %s", &count, color_name) == 2) {
      // Accumulate the counts into the CurrentSetCubes struct
      if (strcmp(color_name, "red") == 0) {
        current_cubes.red += count;
      } else if (strcmp(color_name, "green") == 0) {
        current_cubes.green += count;
      } else if (strcmp(color_name, "blue") == 0) {
        current_cubes.blue += count;
      }
      // Note: No 'else' for unrecognized colors, they are ignored as per AoC
      // problem
    }
    // Get the next color-count pair
    color_token = strtok_r(NULL, ",", &rest_of_colors_ptr);
  }

  // After accumulating all counts for this set, check against the limits
  if (current_cubes.red > limits->red || current_cubes.green > limits->green ||
      current_cubes.blue > limits->blue) {
    return 0; // This set is impossible because one or more colors exceed limits
  }

  return 1; // All accumulated counts in this set are within limits, so the set
            // is possible
}

int is_game_possible(char *sets_string, int game_id, const CubeLimits *limits) {
  char *set_token;
  char *rest_of_sets_ptr; // For strtok_r state
  // Tokenize the sets string by semicolons to get individual sets
  set_token = strtok_r(sets_string, ";", &rest_of_sets_ptr);
  while (set_token != NULL) {
    // Process each set and check if it's possible
    if (!process_set(set_token, limits)) { // Pass limits to process_set
      // If any set is impossible, the entire game is impossible
      return 0;
    }
    // Get the next set
    set_token = strtok_r(NULL, ";", &rest_of_sets_ptr);
  }
  return 1; // All sets in the game are possible, so the game is possible
}

char *parse_game_line(char *line_copy, int *game_id_out) {
  char *token;
  char *game_id_end_ptr; // For strtok_r state
  // Extract the "Game X" part
  token = strtok_r(line_copy, ":", &game_id_end_ptr);
  if (token == NULL) {
    return NULL; // Error: No game ID part found
  }

  // Skip "Game " and convert the rest to an integer
  *game_id_out = atoi(token + strlen("Game "));

  // Get the rest of the string containing the sets
  // Using an empty string as delimiter for strtok_r effectively gets the rest
  // of the string
  return strtok_r(NULL, "", &game_id_end_ptr);
}

int main() {
  FILE *file;
  char line[512]; // Assuming a max line length of 511 characters + null
                  // terminator
  int sum_of_possible_game_ids = 0;

  // Initialize the CubeLimits struct with the maximum allowed values
  CubeLimits max_limits = {.red = 12, .green = 13, .blue = 14};

  // Open the input file
  // Ensure 'input.txt' is in the same directory as your compiled executable
  file = fopen("input.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  // Read the file line by line
  while (fgets(line, sizeof(line), file) != NULL) {
    // Remove trailing newline character if present
    line[strcspn(line, "\n")] = 0;

    // Create a mutable copy of the line for strtok_r to modify
    char *line_copy = strdup(line);
    if (line_copy == NULL) {
      perror("Failed to duplicate line");
      fclose(file);
      return 1;
    }

    int game_id = 0;
    char *sets_string = NULL;

    // Parse the game line to get ID and sets string
    sets_string = parse_game_line(line_copy, &game_id);

    if (sets_string == NULL) {
      fprintf(stderr, "Error parsing line: %s\n", line);
      free(line_copy);
      continue; // Skip to the next line
    }

    // Check if the entire game is possible based on its sets, passing the
    // limits
    if (is_game_possible(sets_string, game_id, &max_limits)) {
      sum_of_possible_game_ids += game_id;
    }

    free(line_copy); // Free the duplicated line for the current iteration
  }

  fclose(file); // Close the file after processing all lines

  printf("Sum of IDs of possible games: %d\n", sum_of_possible_game_ids);

  return 0;
}

```

### Part 2
```c

#include <stdio.h>
#include <stdlib.h> // For atoi, strdup, free
#include <string.h>

// Define a struct to hold the cube counts observed within a single set
typedef struct {
  int red;
  int green;
  int blue;
} SetCubes; // Renamed from CurrentSetCubes for clarity in Part B context

// Define a struct to hold the minimum required cube counts for the entire game
// In Part B, this stores the maximum observed count for each color across all
// sets
typedef struct {
  int red;
  int green;
  int blue;
} GameMinCubes;

/**
 * @brief Processes a single set of cubes (e.g., "3 blue, 4 red") and returns
 * the counts.
 *
 * This function tokenizes the set string by commas to get individual
 * color-count pairs. It accumulates the counts for red, green, and blue into a
 * SetCubes struct.
 *
 * @param set_str The string representing a single set of cubes. This string
 * will be modified.
 * @return A SetCubes struct containing the accumulated counts for this set.
 */
SetCubes process_set(char *set_str) {
  char *color_token;
  char *rest_of_colors_ptr; // For strtok_r state

  // Initialize current cube counts for this set to zero
  SetCubes current_set_cubes = {0, 0, 0};

  // Tokenize the set string by commas to get individual color-count pairs
  color_token = strtok_r(set_str, ",", &rest_of_colors_ptr);
  while (color_token != NULL) {
    int count = 0;
    char color_name[10]; // To store "red", "blue", "green"

    // Trim leading whitespace from color_token (e.g., " 3 blue" -> "3 blue")
    while (*color_token == ' ') {
      color_token++;
    }

    // Parse the count and color name from the token
    if (sscanf(color_token, "%d %s", &count, color_name) == 2) {
      // Accumulate the counts into the SetCubes struct
      if (strcmp(color_name, "red") == 0) {
        current_set_cubes.red += count;
      } else if (strcmp(color_name, "green") == 0) {
        current_set_cubes.green += count;
      } else if (strcmp(color_name, "blue") == 0) {
        current_set_cubes.blue += count;
      }
      // Note: No 'else' for unrecognized colors, they are ignored as per AoC
      // problem
    }
    // Get the next color-count pair
    color_token = strtok_r(NULL, ",", &rest_of_colors_ptr);
  }

  return current_set_cubes;
}

/**
 * @brief Calculates the "power" of a game for Advent of Code Day 2 Part B.
 *
 * The power is the product of the minimum number of red, green, and blue cubes
 * required for the game to be possible. This means finding the maximum count
 * for each color across all sets in the game.
 *
 * @param sets_string The string containing all sets for a game (e.g., "3 blue,
 * 4 red; 1 red, ...").
 * @param game_id The ID of the current game (used for error
 * reporting/debugging).
 * @return The power of the game (min_red * min_green * min_blue).
 */
long long calculate_game_power(char *sets_string, int game_id) {
  char *set_token;
  char *rest_of_sets_ptr; // For strtok_r state

  // Initialize minimum required cubes for the game to 0
  // These will store the maximum observed count for each color across all sets
  GameMinCubes min_required_cubes = {0, 0, 0};

  // Tokenize the sets string by semicolons to get individual sets
  set_token = strtok_r(sets_string, ";", &rest_of_sets_ptr);
  while (set_token != NULL) {
    // Process each set to get its cube counts
    SetCubes current_set_cubes = process_set(set_token);

    // Update the minimum required cubes for the game if current set has higher
    // counts This effectively finds the maximum count of each color across all
    // sets
    if (current_set_cubes.red > min_required_cubes.red) {
      min_required_cubes.red = current_set_cubes.red;
    }
    if (current_set_cubes.green > min_required_cubes.green) {
      min_required_cubes.green = current_set_cubes.green;
    }
    if (current_set_cubes.blue > min_required_cubes.blue) {
      min_required_cubes.blue = current_set_cubes.blue;
    }

    // Get the next set
    set_token = strtok_r(NULL, ";", &rest_of_sets_ptr);
  }

  // Calculate the power of the game: product of the maximum observed counts
  return (long long)min_required_cubes.red * min_required_cubes.green *
         min_required_cubes.blue;
}

/**
 * @brief Parses a game line to extract the game ID and the sets string.
 *
 * @param line_copy A mutable copy of the full game line. This string will be
 * modified.
 * @param game_id_out Pointer to an integer where the extracted game ID will be
 * stored.
 * @return A pointer to the start of the sets string (after "Game X:"), or NULL
 * on error.
 */
char *parse_game_line(char *line_copy, int *game_id_out) {
  char *token;
  char *game_id_end_ptr; // For strtok_r state

  // Extract the "Game X" part
  token = strtok_r(line_copy, ":", &game_id_end_ptr);
  if (token == NULL) {
    return NULL; // Error: No game ID part found
  }

  // Skip "Game " and convert the rest to an integer
  *game_id_out = atoi(token + strlen("Game "));

  // Get the rest of the string containing the sets
  // Using an empty string as delimiter for strtok_r effectively gets the rest
  // of the string
  return strtok_r(NULL, "", &game_id_end_ptr);
}

int main() {
  FILE *file;
  char line[512]; // Assuming a max line length of 511 characters + null
                  // terminator
  long long total_power_sum = 0; // Changed to long long for sum of powers

  // Open the input file
  // Ensure 'input.txt' is in the same directory as your compiled executable
  file = fopen("input.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  // Read the file line by line
  while (fgets(line, sizeof(line), file) != NULL) {
    // Remove trailing newline character if present
    line[strcspn(line, "\n")] = 0;

    // Create a mutable copy of the line for strtok_r to modify
    char *line_copy = strdup(line);
    if (line_copy == NULL) {
      perror("Failed to duplicate line");
      fclose(file);
      return 1;
    }

    int game_id = 0;
    char *sets_string = NULL;

    // Parse the game line to get ID and sets string
    sets_string = parse_game_line(line_copy, &game_id);

    if (sets_string == NULL) {
      fprintf(stderr, "Error parsing line: %s\n", line);
      free(line_copy);
      continue; // Skip to the next line
    }

    // Calculate the power of the current game
    total_power_sum += calculate_game_power(sets_string, game_id);

    free(line_copy); // Free the duplicated line for the current iteration
  }

  fclose(file); // Close the file after processing all lines

  printf("Sum of the powers of all games: %lld\n",
         total_power_sum); // Use %lld for long long

  return 0;
}

```