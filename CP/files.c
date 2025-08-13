#include <stdio.h>
#include <stdlib.h>


int main() {
    // Open a file for reading
    FILE *fptr = fopen("example.txt", "w");
    char darta[100] = "HELLO GUYSSS";
    
    if(fptr == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }else{
    fputs(darta, fptr);
    printf("Data written to file successfully.\n");
    
    }
    // Close the file
    fclose(fptr);

    return 0;
}

