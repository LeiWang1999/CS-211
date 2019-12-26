#include<stdio.h>
#include<stdlib.h>

int** allocate_matrix(int rows, int cols);
void free_matrix(int** arr1, int rows);
void printArray(int** arr1, int rows, int columns);
void printArray2(int** arr1, int rows, int columns);
int whatNum(char c);

int main(int argc, char** argv){

  FILE* fp = fopen(argv[1], "r");
  if(fp == NULL) return 0;

  int** hexadoku = allocate_matrix(16, 16);
  int i, j, r, s, t;
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
  int end = -1, temp = -999;
  int doneRows = 0, doneColumns = 0, doneBoxes = 0;
  int* remainingNum = malloc(16*sizeof(int));
  while(end == -1){
    int cantFillRow = 0, cantFillColumn = 0, cantFillBox = 0, cantcrossCheck = 0;
    int filledRows = 0, filledColumns = 0, filledBoxes = 0;

    for(i = 0; i < 16; i++){//rows
      int howManyBlank = 0, placeOfCertainty = -1, remaining = -1;
      for(r = 0; r < 16; r++) remainingNum[r] = r;
      for(j = 0; j < 16; j++){
        temp = hexadoku[i][j];
        if(temp == -1){
          howManyBlank++;
          placeOfCertainty = j;
        }
        else{
          if(remainingNum[temp] == -1){
            printf("no-solution");
            return 0; //duplicate number in row
          }
          else remainingNum[temp] = -1;
        }
      }
      if(howManyBlank > 1) continue; //cant fill this row with certainty
      else if(howManyBlank == 0){
        filledRows++;
        if(filledRows == 16){
          doneRows = 1;
          cantFillRow = -1;
          break;
        }
      }
      else if(howManyBlank == 1){//fill the only empty thing in the row
        for(r = 0; r < 16; r++){
          if(remainingNum[r] != -1) remaining = remainingNum[r];
        }
        hexadoku[i][placeOfCertainty] = remaining;
        cantFillRow = -1;
      }
    }
    /*printArray(hexadoku, 16, 16);
    printf("\n\n\n");*/
    for(i = 0; i < 16; i++){//columns
      int howManyBlank = 0, placeOfCertainty = -1, remaining = -1;
      for(r = 0; r < 16; r++) remainingNum[r] = r;
      for(j = 0; j < 16; j++){
        temp = hexadoku[j][i];
        if(temp == -1){
          howManyBlank++;
          placeOfCertainty = j;
        }
        else{
          if(remainingNum[temp] == -1){
            printf("no-solution");
            return 0;
          }
          else remainingNum[temp] = -1;
        }
      }
      if(howManyBlank > 1) continue;
      else if(howManyBlank == 0){
        filledColumns++;
        if(filledColumns == 16){
          doneColumns = 1;
          cantFillColumn = -1;
          break;
        }
      }
      else if(howManyBlank == 1){
        for(r = 0; r < 16; r++){
          if(remainingNum[r] != -1) remaining = remainingNum[r];
        }
        hexadoku[placeOfCertainty][i] = remaining;
        cantFillColumn = -1;
      }
    }
    /*printArray(hexadoku, 16, 16);
    printf("\n\n\n");*/
    for(i = 0; i < 16; i+=4){//boxes
      for(j = 0; j < 16; j+=4){
        int howManyBlank = 0, placeOfCertaintyX = -1, placeOfCertaintyY = -1, remaining = -1;
        for(r = 0; r < 16; r++) remainingNum[r] = r;
        for(s = i; s < i+4; s++){
          for(t = j; t < j+4; t++){
            temp = hexadoku[s][t];
            if(temp == -1){
              howManyBlank++;
              placeOfCertaintyX = t;
              placeOfCertaintyY = s;
            }
            else{
              if(remainingNum[temp] == -1){
                printf("no-solution");
                return 0;
              }
              else remainingNum[temp] = -1;
            }
          }
        }
        if(howManyBlank > 1) continue;
        else if(howManyBlank == 0){
          filledBoxes++;
          if(filledBoxes == 16){
            doneBoxes = 1;
            cantFillBox = -1;
            break;
          }
        }
        else if(howManyBlank == 1){
          for(r = 0; r < 16; r++){
            if(remainingNum[r] != -1) remaining = remainingNum[r];
          }
          hexadoku[placeOfCertaintyY][placeOfCertaintyX] = remaining;
          cantFillBox = -1;
        }
      }
    }
    /*printArray(hexadoku, 16, 16);
    printf("\n\n\n");*/

    for(i = 0; i < 16; i++){//crosscheck
      for(j = 0; j < 16; j++){
        int howManyNums = 0, remaining = -1;
        temp = hexadoku[i][j];
        for(r = 0; r < 16; r++) remainingNum[r] = r;
        if(temp == -1){
          for(s = 0; s < 16; s++){
            int temp2 = hexadoku[i][s];
            if(temp2 != -1) remainingNum[temp2] = -1; //removes numberes that were already used in the row
          }
          for(s = 0; s < 16; s++){
            int temp2 = hexadoku[s][j];
            if(temp2 != -1) remainingNum[temp2] = -1; //removes numbers that were already used in the column
          }
          for(r = 0; r < 16; r++){
            int temp2 = remainingNum[r];
            if(temp2 != -1){
              howManyNums++;
              remaining = remainingNum[r];
            }
          }
          if(howManyNums > 1) continue; //can't fill it up with certainty
          if(howManyNums == 1){
            hexadoku[i][j] = remaining;
            cantcrossCheck = -1;
          }
        }
      }
    }

    //end = 342;
    if(doneRows == 1 && doneColumns == 1 && doneBoxes == 1) end = 342;
    else if(cantFillRow == -1 || cantFillColumn == -1 || cantFillBox == -1 || cantcrossCheck == -1) continue;
    else{
      printf("no-solution");
      return 0;
    }

  }//end of while loop
  free(remainingNum);
  printArray2(hexadoku, 16, 16);
  free_matrix(hexadoku, 16);
  fclose(fp);
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
