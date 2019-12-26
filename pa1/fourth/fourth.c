#include<stdlib.h>
#include<stdio.h>

void printArray();
int** allocate_matrix(int, int);
void free_matrix(int**, int);

int main(int argc, char** argv){
  FILE* fp = fopen(argv[1], "r");
  if(fp == NULL) return 0; //i think the assignment said assume all correct inputs but just in case!

  int x, Ax, Bx;
  int y, Ay, By;
  int i;

  //1st array
  fscanf(fp, "%d %d\n", &Ay, &Ax);
  int** arrayOne = allocate_matrix(Ay, Ax);
  for(y = 0; y < Ay; y++){
    for(x = 0; x < Ax; x++){
      int toAdd;
      fscanf(fp, "%d\t", &toAdd);
      arrayOne[y][x] = toAdd;
    }
    fscanf(fp, "\n");
  }
  //print 1st
  /*for(y = 0; y < Ay; y++){
    for(x = 0; x < Ax; x++){
      printf("%d\t", arrayOne[y][x]);
    }
    printf("\n");
  }
  printf("\n\n"); //PRINT HERE*/
  //2nd array
  fscanf(fp, "%d %d\n", &By, &Bx);
  int** arrayTwo = allocate_matrix(By, Bx);
  for(y = 0; y < By; y++){
    for(x = 0; x < Bx; x++){
      int toAdd;
      fscanf(fp, "%d\t", &toAdd);
      arrayTwo[y][x] = toAdd;
    }
    fscanf(fp, "\n");
  }
  //print 2nd
  /*for(y = 0; y < By; y++){
    for(x = 0; x < Bx; x++){
      printf("%d\t", arrayTwo[y][x]);
    }
    printf("\n");
  }
  printf("\n\n"); //PRINT HERE*/
  //multiplication
  //i dont fucking remember how to multiply matrices
  //https://www.mathsisfun.com/algebra/matrix-multiplying.html
  if(Ax != By) {
    printf("bad-matrices");
    return 0;
  }

  int Cy = Ay;
  int Cx = Bx;
  int** arrayThree = allocate_matrix(Cy, Cx);
  for(y = 0; y < Cy; y++){
    for(x = 0; x < Cx; x++){
      arrayThree[y][x] = 0;
      for(i = 0; i < Ax; i++){
        arrayThree[y][x] += arrayOne[y][i] * arrayTwo[i][x];
      }
    }
  }

  printArray(arrayThree, Cy, Cx);
  free_matrix(arrayOne, Ay);
  free_matrix(arrayTwo, By);
  free_matrix(arrayThree, Cy);
  //fin
  fclose(fp);
  return 0;
}

int** allocate_matrix(int rows, int cols){
  int** ret_val = (int**)malloc(rows * sizeof(int*));
  int i;
  for(i = 0; i < rows; i++){
    ret_val[i] = (int*)malloc(cols * sizeof(int));
  }
  return ret_val;
}

void printArray(int** arr1, int rows, int columns){
  int i, j;
  for(i = 0; i < rows; i++){
    for(j = 0; j < columns; j++){
      int toPrint;
      toPrint = arr1[i][j];
      printf("%d\t", toPrint);
    }
    printf("\n");
  }
}

void free_matrix(int** arr1, int rows){
  int i;
  for(i = 0; i < rows; i++){
    free(arr1[i]);
  }
  free(arr1);
}
