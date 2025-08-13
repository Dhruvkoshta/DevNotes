
### Reading from a File Line by Line

```c

  FILE *fptr;
  fptr = fopen(argv[1], "r");
  if (fptr == NULL) {
    perror("Error opening file");
    return 1;
  }

  char line[1028];
  int points = 0;

  while (fgets(line, sizeof(line), fptr)) {
    line[strcspn(line, "\n")] = 0;
    printf("Processing line: %s\n", line);
}

```


### Splitting Strings using strtok

```c
#include <string.h>
#include <stdio.h>

char** split_string(const char *str, char delimiter) {
    char *delimiter_ptr = strchr(str, delimiter);

    if (delimiter_ptr == NULL) {
        return NULL;
    }

    size_t first_part_len = delimiter_ptr - str;

    char **result = malloc(sizeof(char*) * 2);
    if (result == NULL) {
        return NULL;
    }

    result[0] = malloc(first_part_len + 1);
    if (result[0] == NULL) {
        free(result);
        return NULL;
    }
    strncpy(result[0], str, first_part_len);
    result[0][first_part_len] = '\0';

    size_t second_part_len = strlen(delimiter_ptr + 1);
    result[1] = malloc(second_part_len + 1);
    if (result[1] == NULL) {
        free(result[0]);
        free(result);
        return NULL;
    }
    strcpy(result[1], delimiter_ptr + 1);

    return result;
}
```
