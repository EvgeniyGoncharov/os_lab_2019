#include "find_min_max.h"
#include <signal.h>
#include <limits.h>

int isSignal = 0;

void alarm_handler(int signum){
        isSignal = 1;
    }

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
  
  for (unsigned int i = begin; i < end; ++i) {
        if (array[i] < min_max.min) {
            min_max.min = array[i];
        }
        if (array[i] > min_max.max) {
            min_max.max = array[i];
        }
        if(isSignal == 1){
            printf("Signal is received. Stop calculating.\n");
            return min_max;
        }
    }
  return min_max;
}
