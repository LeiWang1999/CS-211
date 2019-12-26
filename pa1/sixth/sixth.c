#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void whatDo(char*, int, int);
int wheresVowel(char*, int);
int isVowel(char*, int);

int main(int argc, char** argv){
  if(argc == 1) return 0;
  int i;

  int j;
  for(i = 1; i < argc; i++){
    int length = strlen(argv[i]);
    char* word = argv[i];
    int firstVowel = isVowel(word, 0);

    if(firstVowel == 1){
      for(j = 0; j < length; j++){
        printf("%c", word[j]);
      }
      if(i == argc-1) printf("yay");
      else printf("yay ");
    }

    else{
      int whereV = wheresVowel(word, length);
      if(whereV == -1){
        for(j = 0; j < length; j++){
          printf("%c", word[j]);
        }
        if(i == argc-1) printf("ay");
        else printf("ay ");
      }
      else{
        for(j = whereV; j < length; j++){
          printf("%c", word[j]);
        }
        for(j = 0; j < whereV; j++){
          printf("%c", word[j]);
        }
        if(i == argc-1) printf("ay");
        else printf("ay ");
      }
    }
  }
  return 0;
}

int wheresVowel(char* str, int len){
  int placement = -1;
  int i;
  for(i = 0; i < len; i++){
    if(isVowel(str, i) == 1){
      placement = i;
      break;
    }
  }
  return placement;
}

int isVowel(char* str, int place){
  char first = str[place];
  if(first == 'a') return 1;
  if(first == 'e') return 1;
  if(first == 'i') return 1;
  if(first == 'o') return 1;
  if(first == 'u') return 1;
  if(first == 'A') return 1;
  if(first == 'E') return 1;
  if(first == 'I') return 1;
  if(first == 'O') return 1;
  if(first == 'U') return 1;
  return 0;
}
