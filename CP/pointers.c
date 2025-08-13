#include <stdio.h>


int main(){
  int numbers[] = {25,323,234,23,3};

  for(int i = 0; i < sizeof(numbers)/sizeof(numbers[0]); i++) {
    printf("Address of numbers[%d]: %d\n", i, numbers[i]);  
    printf("Address of numbers[%d]: %p\n", i, (void*)&numbers[i]);  
  }

}
 
