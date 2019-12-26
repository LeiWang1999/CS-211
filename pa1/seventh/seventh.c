#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv){
  if(argc == 1) return 0;
  int i, length;
  char* word;
  for(i = 1; i < argc; i++){
    word = argv[i];
    length = strlen(word);
    printf("%c", word[length-1]);
  }
  return 0;
}
