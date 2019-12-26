#include<stdio.h>
#include<stdlib.h>

struct BSTNode{
  int value;
  int height;
  struct BSTNode* left;
  struct BSTNode* right;
};

struct BSTNode* root = NULL;

void insert(int);
void search(int);
struct BSTNode* allocate_treenode(int);
void print_tree(struct BSTNode* root);
void freeMyFellaTree(struct BSTNode* node);

int main(int argc, char** argv){
  FILE* fp = fopen(argv[1], "r");
  if(fp == NULL){
    printf("error"); //file doesn't exist
    return 0;
  }

  while(!feof(fp)){
    char letter;
    int number;
    fscanf(fp, "%c %d\n", &letter, &number);
    if(letter == 'i') insert(number);
    if(letter == 's') search(number);
  }
  fclose(fp);
  freeMyFellaTree(root);
  return 0;
}

void freeMyFellaTree(struct BSTNode* node){
  if(node != NULL){
    freeMyFellaTree(node->left);
    freeMyFellaTree(node->right);
    free(node);
  }
}

void insert(int num){
  int heightCounter = 1;
  int cmp = 0;
  struct BSTNode* current = root;
  struct BSTNode* parent = NULL;
  while(current != NULL){
    if(num == current->value){
      printf("duplicate\n");
      return;
    }
    else if(num < current->value) cmp = -1;
    else cmp = 1;
    parent = current;
    if(cmp == -1) current = current->left;
    else current = current->right;
    heightCounter += 1;
  }

  struct BSTNode* toInsert = allocate_treenode(num);
  toInsert->height = heightCounter;

  if(parent == NULL) root = toInsert;
  else if(cmp == -1) parent->left = toInsert;
  else parent->right = toInsert;
  printf("inserted %d\n", heightCounter);
}

void search(int num){
  struct BSTNode* current = root;

  while(current != NULL){
    if(num == current->value){
      printf("present %d\n", current->height);
      return;
    }
    if(num < current->value) current = current->left;
    else current = current->right;
  }

  printf("absent\n");
}

struct BSTNode* allocate_treenode(int value){
  struct BSTNode* temp = malloc(sizeof(struct BSTNode));
  temp->value = value;
  temp->height = 1;
  temp->left = NULL;
  temp->right = NULL;
  return temp;
}

void print_tree(struct BSTNode* root){
  if(root == NULL) return;
  printf("value: %d\n", root->value);
  print_tree(root->left);
  print_tree(root->right);
}
