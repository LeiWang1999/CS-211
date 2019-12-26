#include<stdio.h>
#include<stdlib.h>

struct BSTNode{
  int value;
  struct BSTNode* left;
  struct BSTNode* right;
};

struct BSTNode* root = NULL;

void insert(int);
void search(int);
struct BSTNode* allocate_treenode(int);
void print_tree(struct BSTNode* root);
void freeMyFellaTree(struct BSTNode* node);
struct BSTNode* delete(struct BSTNode* node, int num, int firstgo);
struct BSTNode* smallRight(struct BSTNode* node);
int search2(int);

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
    else if(letter == 's') search(number);
    else if(letter == 'd') {
      int exists = search2(number);
      if(exists == 0) printf("fail\n");
      else root = delete(root, number, 1);
    }
  }
  freeMyFellaTree(root);
  fclose(fp);
  return 0;
}

int search2(int num){
  struct BSTNode* current = root;
  int heightCounter = 1;
  while(current != NULL){
    if(num == current->value){
      return 1;
    }
    if(num < current->value) current = current->left;
    else current = current->right;
    heightCounter += 1;
  }

  return 0;
}

struct BSTNode* smallRight(struct BSTNode* node){
  struct BSTNode* current = node;
  while(current && current->left != NULL) current = current->left;
  return current;
}

struct BSTNode* delete(struct BSTNode* node, int num, int firstgo){
  if(node == NULL){
    return NULL;
  }
  if(num < node->value) node->left = delete(node->left, num, 8008135);
  else if(num > node->value) node->right = delete(node->right, num, 342);
  else{
    if(node->left == NULL && node->right == NULL){
      node = NULL;
    }
    else if(node->left == NULL){
      struct BSTNode* temp = node;
      node = node->right;
      free(temp);
    }
    else if(node->right == NULL){
      struct BSTNode* temp = node;
      node = node->left;
      free(temp);
    }
    else{
      struct BSTNode* temp = smallRight(node->right);
      node->value = temp->value;
      node->right = delete(node->right, temp->value, 42069);
    }
  }
  if(firstgo == 1) printf("success\n");
  return node;
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

  if(parent == NULL) root = toInsert;
  else if(cmp == -1) parent->left = toInsert;
  else parent->right = toInsert;
  printf("inserted %d\n", heightCounter);
}

void search(int num){
  struct BSTNode* current = root;
  int heightCounter = 1;
  while(current != NULL){
    if(num == current->value){
      printf("present %d\n", heightCounter);
      return;
    }
    if(num < current->value) current = current->left;
    else current = current->right;
    heightCounter += 1;
  }

  printf("absent\n");
}

struct BSTNode* allocate_treenode(int value){
  struct BSTNode* temp = malloc(sizeof(struct BSTNode));
  temp->value = value;
  temp->left = NULL;
  temp->right = NULL;
  return temp;
}

void print_tree(struct BSTNode* root){
  if(root == NULL) return;
  printf("value: %d\n", root->value);
  printf("\n");
  print_tree(root->left);
  print_tree(root->right);
}

void freeMyFellaTree(struct BSTNode* node){
  if(node != NULL){
    freeMyFellaTree(node->left);
    freeMyFellaTree(node->right);
    free(node);
  }
}
