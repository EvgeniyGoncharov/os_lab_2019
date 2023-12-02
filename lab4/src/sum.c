#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(const struct SumArgs *args) {
  int sum = 0;
  // TODO: your code here 
  for(uint32_t i = (*args).begin; i < (*args).end; i++){
    sum += (*args).array[i];
  }
  return sum;
}