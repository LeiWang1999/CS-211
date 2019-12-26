#include<stdio.h>
#include<stdlib.h>

int isprime(int num);

int main(int argc, char** argv){
  //getting first line, how many numbers there are in the file
  FILE* fp = fopen(argv[1], "r");
  int numOfNums = 0;
  fscanf(fp, "%d\n", &numOfNums);
  //determining whether or not each number is a right truncatable prime

  int i = 0;
  for(i = 0; i < numOfNums; i++){
    int temp = 0;
    int tempPrime = 0;
    fscanf(fp, "%d\n", &temp);
    //if number is single digit
    if(temp < 10 && temp > 0){
      tempPrime = isprime(temp);
      if(tempPrime == 1) printf("yes\n");
      else printf("no\n");
      continue;
    }
    //numbers longer than single digit
    while(temp / 10 != 0){
      tempPrime = isprime(temp);
      if(tempPrime == 0) break;
      else temp = temp / 10;
    }
    if(temp < 10 && temp > 0){
      tempPrime = isprime(temp);
      if(tempPrime == 1) printf("yes\n");
      else printf("no\n");
    }
    else{
      if(tempPrime == 0) printf("no\n");
      else printf("yes\n");
    }
  }
  /*
  int test = isprime(4);
  printf("%d\n", test);*/

  fclose(fp); //closing the file
  return 0;
}

int isprime(int num){
  if(num == 1) return 0;
  int i = 0;
  for(i = 2; i < num; i++){
    if(num % i == 0 && i != num) return 0;
  }
  return 1;
}
