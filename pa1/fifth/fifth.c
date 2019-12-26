#include<stdio.h>
#include<stdlib.h>

int** allocate_matrix(int, int);
void free_matrix(int**, int);
int hasPotential(int**, int);
int magicRows(int**, int);
int magicColumns(int**, int);
int magicDiags(int**, int);

int main(int argc, char** argv){
  FILE* fp = fopen(argv[1], "r");
  if(fp == NULL) return 0; //i think the assignment said assume all correct inputs but just in case!
  int dims, y, x;
  fscanf(fp, "%d\n", &dims);
  int** matrix = allocate_matrix(dims, dims);
  for(y = 0; y < dims; y++){
    for(x = 0; x < dims; x++){
      int toAdd;
      fscanf(fp, "%d\t", &toAdd);
      matrix[y][x] = toAdd;
    }
    fscanf(fp, "\n");
  }

  int p, mr, mc, md;
  p = hasPotential(matrix, dims);
  mr = magicRows(matrix, dims);
  mc = magicColumns(matrix, dims);
  md = magicDiags(matrix, dims);
  if(p == 1 && mr != 0 && mr == mc && mc == md){
    printf("magic");
    return 0;
  }
  printf("not-magic");
  free_matrix(matrix, dims);
  fclose(fp);
  return 0;
}

int hasPotential(int** arr1, int dimens){
  int squared = dimens * dimens;
  int numbers[squared];
  int i, j, h;

  for(i = 0; i < squared; i++){
    numbers[i] = 0;
  }

  h = 0;
  for(i = 0; i < dimens; i++){
    for(j = 0; j < dimens; j++){
      numbers[h] += arr1[i][j];
      h++;
    }
  }

  /*for(i = 0; i < squared; i++){
    printf("%d ", numbers[i]);
  }
  printf("\n"); //PRINT HERE*/

  for(i = 0; i < squared; i++){
    for(j = 0; j < squared; j++){
      if(numbers[j] > numbers[i]){
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
      }
    }
  } //sort the array
  /*for(i = 0; i < squared; i++){
    printf("%d ", numbers[i]);
  }
  printf("\n"); //PRINT HERE*/
  for(i = 0; i < squared; i++){
    if(numbers[i] == 0){
      continue;
    }
    else if(numbers[i] == numbers[i-1]){
      return 0;
      break;
    }
    else if(numbers[i] > squared || numbers[i-1] > squared){
      return 0;
      break;
    }
    else if(abs(numbers[i] - numbers[i-1]) > 1){
      return 0;
      break;
    }
  }
  //printf("it has potential\n");
  return 1;
}

int magicDiags(int** arr1, int dimens){
  int sum, i, j;
  sum = 0;
  for(i = dimens-1; i > 0; i--){
    for(j = 0; j < dimens; j++){
      sum += arr1[i][j];
    }
  }

  int temp = 0;
  for(i = 0; i < dimens; i++){
    for(j = dimens-1; j > 0; j--){
      temp += arr1[i][j];
    }
  }
  if(sum != temp){
    return 0;
  }
  //printf("%d\n", sum);
  return sum/2;
}

int magicRows(int** arr1, int dimens){
  int sum, i, j;
  sum = 0;

  for(i = 0; i < dimens; i++){
    for(j = 0; j < dimens; j++){
      sum += arr1[i][j];
    }
    break;
  }

  for(i = 0; i < dimens; i++){
    int temp = 0;
    for(j = 0; j < dimens; j++){
      temp += arr1[j][i];
    }
    if(temp != sum){
      return 0;
      break;
    }
  }
  //printf("%d\n", sum);
  return sum;
}

int magicColumns(int** arr1, int dimens){
  int sum, i, j;
  sum = 0;
  for(i = 0; i < dimens; i++){
    sum += arr1[i][0];
  }
  for(i = 0; i < dimens; i++){
    int temp = 0;
    for(j = 0; j < dimens; j++){
      temp += arr1[i][j];
    }
    if(temp != sum){
      return 0;
      break;
    }
  }
  //printf("%d\n", sum);
  return sum;
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
