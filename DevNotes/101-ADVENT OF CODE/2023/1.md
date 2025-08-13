### PART 1

```C
#include <ctype.h>
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
    int sum = 0;

    while (fgets(line, sizeof(line), fptr)) {
        // Remove trailing newline character if present
        line[strcspn(line, "\n")] = 0;
        printf("Processing line: %s\n", line); // Changed from original printf

        int left = -1;
        int right = -1;

        for (int i = 0; i < strlen(line); i++) {
            if (isdigit(line[i])) {
                if (left == -1) {
                    left = line[i] - '0'; // Use '0' instead of 0x30 for clarity
                    right = line[i] - '0';
                } else {
                    right = line[i] - '0';
                }
            }
        }

        // Handle lines with no digits
        if (left != -1) {
            int digit = left * 10 + right;
            printf("Extracted Digit: %d\n", digit);
            sum += digit;
        } else {
            printf("No digits found in this line.\n");
        }
    }

    printf("Final Sum: %d \n", sum); // Changed from original printf

    fclose(fptr);
    return 0;
}
```


### PART 2

```C

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char str[10];
    int i;
} lookup_t;

lookup_t gTable[] = {
    {"zero", 0},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

#define GTABLE_SIZE (sizeof(gTable) / sizeof(gTable[0]))

int match(char *pattern) {
    for (int i = 0; i < GTABLE_SIZE; i++) {
        if (!strncmp(gTable[i].str, pattern, strlen(gTable[i].str))) {
            return gTable[i].i;
        }
    }
    return -1;
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
    int sum = 0;

    while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = 0;
        printf("Processing line: %s\n", line);

        int left = -1;
        int right = -1;

        for (int i = 0; i < strlen(line); i++) {
            int current_digit = -1;

            if (isdigit(line[i])) {
                current_digit = line[i] - '0';
            } else {
                int matched_word_value = match(&line[i]);
                if (matched_word_value != -1) {
                    current_digit = matched_word_value;
                }
            }

            if (current_digit != -1) {
                if (left == -1) {
                    left = current_digit;
                }
                right = current_digit;
            }
        }

        if (left != -1) {
            int calibration_value = left * 10 + right;
            printf("Extracted Calibration Value: %d\n", calibration_value);
            sum += calibration_value;
        } else {
            printf("No valid digits (numeric or word) found in this line.\n");
        }
    }

    printf("Final Sum of Calibration Values: %d \n", sum);

    fclose(fptr);
    return 0;
}
```


