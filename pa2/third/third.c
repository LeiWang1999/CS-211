#include<stdio.h>
#include<stdlib.h>
void printArray(double** arr1, int rows, int columns);
double** allocate_matrix(int rows, int cols);
void free_matrix(double** arr1, int rows);
//double** multiply_matrix(double** arr1, double** arr2, int rows, int cols);
/*x1 = bedrooms
x2 = size of house
x3 = baths
x4 = build date*/
int main(int argc, char** argv){
  FILE* fp = fopen(argv[1], "r"); //training file

  int row, col;
  fscanf(fp, "%d", &col); col = col+1; //1s column needed
  fscanf(fp, "%d", &row);
  double** X = allocate_matrix(row, col);
  double* Y = (double*)malloc(row * sizeof(double));;//declare a 1d array here for prices

  double temp = 0, factor = 0.0;
  int i, j, r, s;
  char c;
  for(i = 0; i < row; i++){
    for(j = 0; j < col+1; j++){
      if(j == 0){
        X[i][j] = 1.0;
        continue;
      }
      fscanf(fp, "%lf", &temp);
      fscanf(fp, "%c", &c); //commas fuck everything up why are they even here wtf
      if(j == col) Y[i] = temp;
      else X[i][j] = temp;
    }
  }//training matrix
  //printArray(X, row, col); printf("data\n");
  //printArray(Y, row, 1); printf("prices\n");

  //W = (XT.X)^−1.XT.Y
  //transpose the data array - rows are now the columns - this is XT
  double** XT = allocate_matrix(col, row); //u kinda just reverse the array
  for(i = 0; i < col; i++){
    for(j = 0; j < row; j++){
      XT[i][j] = X[j][i];
    }
  }
  //printArray(XT, col, row); printf("XT\n");
  //(XT times X) this is my pa1 code so if it doesnt work we know why i didnt get full points lol lmao
  double** XTX = allocate_matrix(col, col);
  for(i = 0; i < col; i++){
    for(j = 0; j < col; j++){
      XTX[i][j] = 0.0;
      for(r = 0; r < row; r++) XTX[i][j] += XT[i][r] * X[r][j];
    }
  }
  //printArray(XTX, col, col); printf("multplied XT and X\n");
  r = 0;
  //GAUSSIAN BULLSHIT
  double** inverseAugmented = allocate_matrix(col, col*2);
  for(i = 0; i < col; i++){
    for(j = 0; j < col; j++){
      inverseAugmented[i][j] = XTX[i][j]; //first copy it all into first half
    }
  }
  //printArray(inverseAugmented, col, col*2); printf("inverseaugmented\n");

  double** identity = allocate_matrix(col, col);
  for(i = 0; i < col; i++){
    for(j = 0; j < col; j++){//need diagonal 1s
      if(i == j) identity[i][j] = 1.0;
      else identity[i][j] = 0.0;
    }
  }
  //printArray(identity, col, col); printf("identity\n");
  for(i = 0; i < col; i++){
    r = 0;
    for(j = col; j < col*2; j++){
      inverseAugmented[i][j] = identity[i][r]; //add on the identity to the end
      r++;
    }
  }
  //printArray(inverseAugmented, col, col*2); printf("array to inverse\n");
  r = 0;
  for(i = 0; i < col; i++){
    for(j = 0; j < col; j++){
      if(i == j){
        for(r = 0; r < col; r++){
          if(r != i){
            temp = inverseAugmented[r][j];
            factor = -1.0*(temp/inverseAugmented[i][j]);
            for(s = 0; s < col*2; s++) inverseAugmented[r][s] = (inverseAugmented[i][s] * factor) + inverseAugmented[r][s];
          }
          else continue;
        }
      }
      else continue;
    }
  }
  //printArray(inverseAugmented, col, col*2); printf("operations\n");
  for(i = 0; i < col; i++){
    temp = 1.0;
    for(j = 0; j < col*2; j++){
      if(i == j) temp = inverseAugmented[i][j];
      inverseAugmented[i][j] = inverseAugmented[i][j]/temp;
    }
  }
  //printArray(inverseAugmented, col, col*2); printf("operations\n");

  r = 0;
  s = col;
  for(i = 0; i < col; i++){
    for(j = 0; j < col; j++){
      identity[i][j] = inverseAugmented[r][s];
      s++;
    }
    r++; s = col;
  } //identity is now the inverted matrix
  //printArray(identity, col, col); printf("inversion complete\n");

  //(XT*X)^-1 * XT
  double** XTXXT = allocate_matrix(col, row);
  for(i = 0; i < col; i++){
    for(j = 0; j < row; j++){
      XTXXT[i][j] = 0.0;
      for(r = 0; r < col; r++) XTXXT[i][j] += identity[i][r] * XT[r][j];
    }
  }
  //printArray(XTXXT, col, row); printf("inverse times XT\n");

  double* XTXXTY = (double*)malloc(col * sizeof(double));;
  for(i = 0; i < col; i++){
    XTXXTY[i] = 0.0;
    for(j = 0; j < row; j++) XTXXTY[i] += XTXXT[i][j] * Y[j];
    //this stupid bitch right here i spent 6 hours looking for bugs and it was because += was a = holy FUCKING shit dude
  }
  //printArray(XTXXTY, col, 1); printf("final weights\n");

  FILE* fp2 = fopen(argv[2], "r");
  int row2;
  fscanf(fp2, "%d", &row2);
  double** testData = allocate_matrix(row2, col);
  for(i = 0; i < row2; i++){
    for(j = 0; j < col; j++){
      if(j == 0){
        testData[i][j] = 1.0;
        continue;
      }
      fscanf(fp2, "%lf", &temp);
      fscanf(fp2, "%c", &c);//commas again
      testData[i][j] = temp;
    }
  }
  //printArray(testData, row2, col); printf("\n");

  double* finalWeights = (double*)malloc(row * sizeof(double));;
  for(i = 0; i < row2; i++){
    finalWeights[i] = 0.0;
    for(j = 0; j < col; j++) finalWeights[i] += testData[i][j] * XTXXTY[j];
  }

  for(i = 0; i < row2; i++){
    printf("%0.0lf\n", finalWeights[i]);
  }

  free(Y);
  free(XTXXTY);
  free(finalWeights);
  free_matrix(X, row);
  free_matrix(XT, col);
  free_matrix(XTX, col);
  free_matrix(inverseAugmented, col);
  free_matrix(identity, col);
  free_matrix(XTXXT, col);
  free_matrix(testData, row2);
  //free(me from this hell)
  fclose(fp);
  fclose(fp2);
  return 0;
}

/*double** multiply_matrix(double** arr1, double** arr2, int rows, int cols){
  double** ret_val = (double**)malloc(rows * sizeof(double*));
  int i, j, r;
  for(i = 0; i < rows; i++){
    ret_val[i] = (double*)malloc(cols * sizeof(double));
  }
  for(i = 0; i < rows; i++){
    for(j = 0; j < cols; j++){
      ret_val[i][j] = 0.0;
      for(r = 0; r < rows; r++) ret_val[i][j] += arr1[i][r] * arr2[r][j];
    }
  }
  return ret_val;
}*/

void free_matrix(double** arr1, int rows){
  int i;
  for(i = 0; i < rows; i++){
    free(arr1[i]);
  }
  free(arr1);
}

void printArray(double** arr1, int rows, int columns){
  int i, j;
  for(i = 0; i < rows; i++){
    for(j = 0; j < columns; j++){
      double toPrint;
      toPrint = arr1[i][j];
      if(j+1 == columns) printf("%lf", toPrint);
      else printf("%lf\t", toPrint);
    }
    printf("\n");
  }
}

double** allocate_matrix(int rows, int cols){
  double** ret_val = (double**)malloc(rows * sizeof(double*));
  int i;
  for(i = 0; i < rows; i++){
    ret_val[i] = (double*)malloc(cols * sizeof(double));
  }
  return ret_val;
}
