#include<stdio.h>
#include<stdlib.h>

struct node{
  int value;
  struct node* next;
};

struct node* hashTable[1000] = {NULL};

int hash(int, int);
int insert(int num);
int search(int num);
void freeMyFellaHash();

int main(int argc, char** argv){
  FILE* fp = fopen(argv[1], "r");
  if(fp == NULL) return 0; //i think the assignment said assume all correct inputs but just in case!

  while(!feof(fp)){
    char letter;
    int number;
    int toPrint;
    fscanf(fp, "%c\t%d\n", &letter, &number);
    if(letter == 'i') {
      toPrint = insert(number);
      if(toPrint == 1) printf("inserted\n");
      else printf("duplicate\n");
    }
    if(letter == 's'){
      search(number);
      toPrint = search(number);
      if(toPrint == 1) printf("present\n");
      else printf("absent\n");
    }
  }
  freeMyFellaHash();
  fclose(fp);
  return 0;
}

void freeMyFellaHash(){
  int i;
  for(i = 0; i < 1000; i++){
    if(hashTable[i] == NULL){
      continue;
    }
    else{
      struct node* current;
      struct node* current2;
      current = hashTable[i];
      while(current != NULL){
        current2 = current;
        current = current->next;
        free(current2);
      }
    }
  }
}

int hash(int num, int size){
  int toHash = num % size;
  if(toHash < 0) toHash = abs(toHash);
  return toHash;
}

int insert(int num){
  struct node* toInsert = malloc(sizeof(struct node));
  toInsert->value = num;
  toInsert->next = NULL;

  int key = hash(num, 1000);

  if(hashTable[key] == NULL){
    hashTable[key] = toInsert;
    return 1;
  }
  struct node* current = hashTable[key];
  struct node* prev = NULL;
  while(current != NULL){
    if(current->value == num){
      return 0;
      break; //DUPLICATE FAILURE
    }
    prev = current;
    current = current->next;
  }
  prev->next = toInsert;
  return 1;
}

int search(int num){
  int key = hash(num, 1000);
  if(hashTable[key] == NULL) return 0;
  struct node* current = hashTable[key];
  while(current != NULL){
    if(current->value == num){
      return 1;
      break;
    }
    current = current->next;
  }
  return 0;
}
