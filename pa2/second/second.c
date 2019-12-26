#include<stdio.h>
#include<stdlib.h>

int** allocate_matrix(int rows, int cols);
void free_matrix(int** arr1, int rows);
void printArray(int** arr1, int rows, int columns);
void printArray2(int** arr1, int rows, int columns);
int whatNum(char c);
int validNumber(int** arr1, int row, int col);
int solve_board(int** arr1, int rows, int cols, int* check);
int check_validity(int** arr1, int rows, int cols, int* check);
int hasEmpty(int** arr1, int rows, int cols);
int hasEmptyi(int** arr1, int rows, int cols);
int hasEmptyj(int** arr1, int rows, int cols);

int main(int argc, char** argv){

  FILE* fp = fopen(argv[1], "r");
  if(fp == NULL) return 0;

  int** hexadoku = allocate_matrix(16, 16);
  int i, j;
  //scanning and converting the hexadoku to ints
  for(i = 0; i < 16; i++){
    for(j = 0; j < 16; j++){
      char tempC;
      int tempNum;
      fscanf(fp, "%c\t", &tempC);
      tempNum = whatNum(tempC); //conversion
      hexadoku[i][j] = tempNum; //adding in
    }
    fscanf(fp, "\n");
  }
  /*printArray(hexadoku, 16, 16);
  printf("\n\n\n");*/
  int* remainingNum = malloc(16*sizeof(int));

  int success = solve_board(hexadoku, 16, 16, remainingNum);
  if(success == 0){
    printf("no-solution");
    return 0;
  }

  free(remainingNum);
  printArray2(hexadoku, 16, 16);
  free_matrix(hexadoku, 16);
  fclose(fp);
  return 0;
}

int solve_board(int** arr1, int rows, int cols, int* check){
  int y, x, i;
  int valid = check_validity(arr1, 16, 16, check);
  if(valid == 0) return 0;
  int hEmpty = hasEmpty(arr1, 16, 16);
  if(hEmpty == 0) return 1;
  else{
    y = hasEmptyi(arr1, 16, 16);
    x = hasEmptyj(arr1, 16, 16);
    for(i = 0; i < 16; i++){
      arr1[y][x] = i;
      if(solve_board(arr1, 16, 16, check)) return 1;
    }
  }
  arr1[y][x] = 0;
  return 0;
}

int check_validity(int** arr1, int rows, int cols, int* check){
  int i, j, r, s, t, temp, howManyNums;
  for(i = 0; i < 16; i++){
    for(j = 0; j < 16; j++){
      temp = arr1[i][j];
      for(r = 0; r < 16; r++) check[r] = r;
      for(s = 0; s < 16; s++){
        int temp2 = arr1[i][s];
        if(temp2 != -1) check[temp2] = -1; //removes numberes that were already used in the row
      }
      for(s = 0; s < 16; s++){
        int temp2 = arr1[s][j];
        if(temp2 != -1) check[temp2] = -1; //removes numbers that were already used in the column
      }
      for(r = 0; r < 16; r++){
        int temp2 = check[r];
        if(temp2 == -1){
          howManyNums++;
        }
      }
      if(howManyNums >= 1) return 0; //nope
    }
  }
  for(i = 0; i < 16; i+=4){//boxes
    for(j = 0; j < 16; j+=4){
      int howManyNums = 0;
      for(r = 0; r < 16; r++) check[r] = r;
      for(s = i; s < i+4; s++){
        for(t = j; t < j+4; t++){
          temp = arr1[s][t];
          if(temp == -1){
            howManyNums++;
          }
          else{
            if(check[temp] == -1){
              printf("no-solution");
              return 0;
            }
            else check[temp] = -1;
          }
        }
      }
      if(howManyNums >= 1){
        return 0;
      }
    }
  }
  return 1;
}

int hasEmpty(int** arr1, int rows, int cols){
  int i, j;
  for(i = 0; i < 16; i++){
    for(j = 0; j < 16; j++){
      int temp = arr1[i][j];
      if(temp == -1) return 1;
    }
  }
  return 0;
}

int hasEmptyi(int** arr1, int rows, int cols){
  int i, j;
  for(i = 0; i < 16; i++){
    for(j = 0; j < 16; j++){
      int temp = arr1[i][j];
      if(temp == -1) return i;
    }
  }
  return 0;
}

int hasEmptyj(int** arr1, int rows, int cols){
  int i, j;
  for(i = 0; i < 16; i++){
    for(j = 0; j < 16; j++){
      int temp = arr1[i][j];
      if(temp == -1) return j;
    }
  }
  return 0;
}

//everything under here is utility that functions perfectly
int whatNum(char c){
  int toReturn;
  switch(c){
    case '0': toReturn = 0; break;
    case '1': toReturn = 1; break;
    case '2': toReturn = 2; break;
    case '3': toReturn = 3; break;
    case '4': toReturn = 4; break;
    case '5': toReturn = 5; break;
    case '6': toReturn = 6; break;
    case '7': toReturn = 7; break;
    case '8': toReturn = 8; break;
    case '9': toReturn = 9; break;
    case 'A': toReturn = 10; break;
    case 'B': toReturn = 11; break;
    case 'C': toReturn = 12; break;
    case 'D': toReturn = 13; break;
    case 'E': toReturn = 14; break;
    case 'F': toReturn = 15; break;
    case '-': toReturn = -1; break;
  }
  return toReturn;
}

int** allocate_matrix(int rows, int cols){
  int** ret_val = (int**)malloc(rows * sizeof(int*));
  int i;
  for(i = 0; i < rows; i++){
    ret_val[i] = (int*)malloc(cols * sizeof(int));
  }
  return ret_val;
}

void free_matrix(int** arr1, int rows){
  int i;
  for(i = 0; i < rows; i++){
    free(arr1[i]);
  }
  free(arr1);
}

void printArray(int** arr1, int rows, int columns){
  int i, j;
  for(i = 0; i < rows; i++){
    for(j = 0; j < columns; j++){
      int toPrint;
      toPrint = arr1[i][j];
      if(j+1 == columns) printf("%d", toPrint);
      else printf("%d\t", toPrint);
    }
    printf("\n");
  }
}

void printArray2(int** arr1, int rows, int columns){
  int i, j;
  for(i = 0; i < rows; i++){
    for(j = 0; j < columns; j++){
      char toPrint;
      int temp = arr1[i][j];
      switch(temp){
        case 0: toPrint = '0'; break;
        case 1: toPrint = '1'; break;
        case 2: toPrint = '2'; break;
        case 3: toPrint = '3'; break;
        case 4: toPrint = '4'; break;
        case 5: toPrint = '5'; break;
        case 6: toPrint = '6'; break;
        case 7: toPrint = '7'; break;
        case 8: toPrint = '8'; break;
        case 9: toPrint = '9'; break;
        case 10: toPrint = 'A'; break;
        case 11: toPrint = 'B'; break;
        case 12: toPrint = 'C'; break;
        case 13: toPrint = 'D'; break;
        case 14: toPrint = 'E'; break;
        case 15: toPrint = 'F'; break;
        case -1: toPrint = '-'; break;
      }
      if(j+1 == columns) printf("%c", toPrint);
      else printf("%c\t", toPrint);
    }
    printf("\n");
  }
}
