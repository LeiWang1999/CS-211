#include<stdio.h>
#include<stdlib.h>

struct node{
  int value;
  struct node* next;
};

struct node* head = NULL;

void insert(int);
void delete(int);
void print_list();
void free_list();
int howmany();

int main(int argc, char** argv){
  FILE* fp = fopen(argv[1], "r");
  if(fp == NULL){
    printf("error"); //file doesn't exist
    return 0;
  }
  while(!feof(fp)){
    char letter;
    int number;
    fscanf(fp, "%c\t%d\n", &letter, &number);
    //printf("letter %c number %d\n", letter, number);
    if(letter == 'i') insert(number);
    if(letter == 'd') delete(number);
  }
  fclose(fp);
  print_list();
  free_list();
  return 0;
}

int howmany(){
  if(head == NULL) return 0;
  int counter = 0;
  struct node* current = head;
  while(current != NULL){
    current = current->next;
    counter++;
  }
  return counter;
}

void insert(int val){
  struct node* toInsert = malloc(sizeof(struct node));
  toInsert->value = val;
  toInsert->next = NULL;
  if(head == NULL){ //empty linked list thus far
    head = toInsert;
  }
  else if(val < head->value){
    struct node* temp3 = head;
    head = toInsert;
    toInsert->next = temp3;
  }
  else{
    struct node* prev = NULL;
    struct node* temp2 = head;
    int inserted = 0;
    while(temp2 != NULL && inserted != 1){
      if(val > temp2-> value){
        if(temp2->next == NULL){
          temp2->next = toInsert;
          break;
        }
        else{
        prev = temp2;
        temp2 = temp2->next;
        }
      }
      else if(val < temp2->value){
        prev->next = toInsert;
        toInsert-> next = temp2;
        //here
        inserted++;
      }
      else if(val == temp2->value) break;
    }
  }
}

void delete(int val){
  struct node* current = head;
  struct node* prev = NULL;
  if(head == NULL){
    return;
  }
  if(head->value == val){
    if(head->next == NULL){
      head->next = NULL;
      head = NULL;
      return;
    }
    head = head->next;
  }
  else{
  while(current != NULL){
    if(current->value == val){
      prev->next = current->next;
      if(current->next == NULL) break;
      else{
        prev = current->next;
        current = current->next->next;
      }
    }
    else{
    prev = current;
    current = current->next;
    }
  }
  }
}

void print_list(){
  if(head == NULL){
    printf("0\n");
    return;
  }
  struct node* temp = head;
  int values = howmany();
  printf("%d\n", values);
  while(temp != NULL){
    if(temp->next == NULL) printf("%d", temp-> value);
    else printf("%d\t", temp-> value);
    temp = temp->next;
  }
}

void free_list(){
  struct node* temp = head;
  while(temp != NULL){
    struct node* temp2 = temp;
    temp = temp->next;
    free(temp2);
  }
}
