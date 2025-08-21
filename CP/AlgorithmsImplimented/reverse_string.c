#include <stdio.h>
#include <string.h>

void reverse0(char * str){
  int len = strlen(str);    
  for(int i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = temp;
  }
}

void reverse1(char * str){
  char *end = str + strlen(str) - 1;
  while (str < end) {
    char temp = *str;
    *str++ = *end;
    *end-- = temp;
  }
}

void reverse2(char *str, int len){
  if (len==1) return;
  char temp = str[0];
  str[0] = str[len - 1];
  str[len - 1] = temp;
  reverse2(str + 1, len - 2);
    
}
