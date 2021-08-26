#include "serial.h"
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

using namespace std;

int main(int argc, char *argv[]) {

  uint64_t len;
  int *originalArray;

  originalArray = common_begin(argc, argv, &len);
  int *tmp_array = (int *) malloc(len * sizeof(int));

  clock_t start = clock();
  merge_sort(originalArray, tmp_array, 0, len - 1);
  clock_t end = clock();

  common_end(start, end, originalArray, len);

  free(originalArray);
  free(tmp_array);

  return 0;
}
