### PART 1
```c
#include <stdio.h>

int main(int argc, char *argv[]) {
  long int time = 48876981;
  long int distance = 255128811171623;

  int count = 0;
  for (long int j = 0; j <= time; j++) {
    long int mydist = j * (time - j);
    if (mydist > distance)
      count++;
  }

  printf("%d \n", count);
  return 0;
}
```