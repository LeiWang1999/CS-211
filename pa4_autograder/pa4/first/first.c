#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

struct block{
  unsigned long long tag;
  int ttime;
  int valid;
};

int powerOf2(int n);
int validateArgs(int cSize, int bSize, char* pol, int pSize);
struct block** makeCache(int cSize, int numOfSets, int blocksInSet);
void free_matrix(struct block** arr1, int numOfSets, int blocksInSet);
int checkHit(struct block** cash, unsigned long long bTag, unsigned int index, int blocksIn, char* pol);
void insertBlock(struct block** cash, unsigned long long bTag, unsigned int index, int blocksIn);
void print();

int reads, writes, hits, misses;
int time = 1;
struct block** cache = NULL;
struct block** pFetchCache = NULL;

int main(int argc, char** argv){
  if(argc < 6){
    printf("error");
    return 0;
  } //has to have correct # of inputs
  FILE* fp = fopen(argv[6], "r");
  if(fp == NULL){
    printf("error");
    return 0;
  }
  //making sure numerical inputs are fine
  int cacheSize = atoi(argv[1]), blockSize = atoi(argv[2]), prefetchSize = atoi(argv[5]);
  if(cacheSize == 0 || blockSize == 0 || prefetchSize < 0){
    printf("error");
    return 0;
  }
  char* policy = argv[3]; char* associativity = argv[4];
  if(validateArgs(cacheSize, blockSize, policy, prefetchSize) == 0){
    printf("error");
    return 0;
  }
  //feeling out associativity
  int assocType;
  int assocNum = 0;
  if(strcmp(associativity, "direct") == 0) assocType = 1;
  else if(strcmp(associativity, "assoc") == 0) assocType = 2;
  else{
    char* lookFor = "assoc:";
    if(strstr(associativity, lookFor) == NULL){
      printf("error");
      return 0;
    }
    if(strlen(associativity) == 6){
      printf("error");
      return 0;
    }
    int length = strlen(associativity);
    int numOfNums = length - 5;
    char* newString = malloc(sizeof(char) * numOfNums);
    int i = 0, j = 0;
    for(i = 6; i < length; i++){
      newString[j] = associativity[i];
      j++;
    }
    assocNum = atoi(newString);
    free(newString);
    if(powerOf2(assocNum) == 0){
      printf("error");
      return 0;
    }
    assocType = 3;
  }   //1 = direct, 2 = fully assoc, 3 = n way assoc
  int noSets, noBlocks; //number of sets for mask operation
  if(assocType == 1 || assocNum == 1) { //direct or assoc:n where n = 1
    noSets = cacheSize/blockSize;
    noBlocks = 1;
  }
  else if(assocType == 2) { //fully associative
    noSets = 1;
    noBlocks = cacheSize/blockSize;
  }
  else{ //assoc:n where n != 1
    noSets = cacheSize/(blockSize*assocNum);
    noBlocks = assocNum;
  }

  cache = makeCache(cacheSize, noSets, noBlocks);
  //gettin goffset/index/tag bits
  int offset_bits = log2(blockSize);
  int numSetBits = log2(noSets);

  char command; unsigned long long address;
  while(fscanf(fp, "%c %llx\n", &command, &address) > 0){ //no prefetch
    time++;
    if(command == '#') break;
    if(command == 'W') writes++;
    unsigned long long mask = (1 << numSetBits) - 1;
    unsigned int set_index = (address >> offset_bits) & mask;
    unsigned long long tag = (address >> offset_bits) >> numSetBits;
    if(checkHit(cache, tag, set_index, noBlocks, policy) == 1) hits++;
    else{
      misses++;
      reads++;
      insertBlock(cache, tag, set_index, noBlocks);
    }
  }
  printf("no-prefetch\n"); print();

  reads = 0; writes = 0; hits = 0; misses = 0;

  pFetchCache = makeCache(cacheSize, noSets, noBlocks);
  FILE* fp2 = fopen(argv[6], "r");
  if(fp2 == NULL){
    printf("error");
    return 0;
  }
  time = 1;

  while(fscanf(fp2, "%c %llx\n", &command, &address) > 0){ //prefetch loop
    time++;
    if(command == '#') break;
    if(command == 'W') writes++;
    unsigned long long mask = (1 << numSetBits) - 1;
    unsigned int set_index = (address >> offset_bits) & mask;
    unsigned long long tag = (address >> offset_bits) >> numSetBits;
    if(checkHit(pFetchCache, tag, set_index, noBlocks, policy) == 1) hits++;
    else{
      misses++;
      reads++;
      insertBlock(pFetchCache, tag, set_index, noBlocks);
      int i;
      //prefetch parts
      unsigned long long prefetch_address = address;
      for(i = 0; i < prefetchSize; i++){
        time++;
        prefetch_address = prefetch_address + blockSize;
        unsigned int pSet_index = (prefetch_address >> offset_bits) & mask;
        unsigned long long pTag = (prefetch_address >> offset_bits) >> numSetBits;
        if(checkHit(pFetchCache, pTag, pSet_index, noBlocks, policy) == 0){
          reads++;
          time++;
          insertBlock(pFetchCache, pTag, pSet_index, noBlocks);
        }
      }
    }
  }
  printf("with-prefetch\n"); print();
  
  fclose(fp); fclose(fp2);
  free_matrix(cache, noSets, noBlocks);
  free_matrix(pFetchCache, noSets, noBlocks);
  return 0;
}

void insertBlock(struct block** cash, unsigned long long bTag, unsigned int index, int blocksIn){
  int place, i, smallest;
  smallest = time;
  for(i = 0; i < blocksIn; i++){
    if(cash[index][i].valid == 0){
      place = i;
      break;
    }
    else if(cash[index][i].ttime < smallest){
      smallest = cash[index][i].ttime;
      place = i;
    }
  }
  cash[index][place].tag = bTag;
  cash[index][place].ttime = time;
  cash[index][place].valid = 1;
}

int checkHit(struct block** cash, unsigned long long bTag, unsigned int index, int blocksIn, char* pol){
  int i;
  for(i = 0; i < blocksIn; i++){
    if(cash[index][i].tag == bTag){
      if(!strcmp(pol,"lru")) cash[index][i].ttime = time;
      return 1;
    } 
  }
  return 0;
}

struct block** makeCache(int cSize, int numOfSets, int blocksInSet){
  struct block** ret_val = (struct block**)malloc(numOfSets * sizeof(struct block*));
  int i, j;
  for(i = 0; i < numOfSets; i++) ret_val[i] = (struct block*)malloc(blocksInSet * sizeof(struct block));
  for(i = 0; i < numOfSets; i++){
    for(j = 0; j < blocksInSet; j++){
      ret_val[i][j].tag = 0;
      ret_val[i][j].ttime = 0;
      ret_val[i][j].valid = 0;
    }
  }
  return ret_val;
}

void free_matrix(struct block** arr1, int numOfSets, int blocksInSet){
  int i;
  for(i = 0; i < numOfSets; i++) free(arr1[i]);
  free(arr1);
}

int validateArgs(int cSize, int bSize, char* pol, int pSize){
  int goodC, goodB;
  goodC = powerOf2(cSize);
  goodB = powerOf2(bSize);
  if(goodC == 0 || goodB == 0 || pSize < 0) return 0;
  if(strcmp(pol, "fifo") != 0 && strcmp(pol, "lru") != 0) return 0;
  return 342;
}

int powerOf2(int n){
  if(n == 1 || n == 2) return 1;
  double num = n + 0.0;
  if(ceil(log2(num)) == floor(log2(num))) return 1;
  return 0;
}

void print(){
  printf("Memory reads: %d\n", reads);
  printf("Memory writes: %d\n", writes);
  printf("Cache hits: %d\n", hits);
  printf("Cache misses: %d\n", misses);
}
