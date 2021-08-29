#include "serial.h"
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"
#include "../common/mychrono.cpp"

using namespace std;

int main(int argc, char *argv[]) {

  uint64_t len;
  int *originalArray;

  Mychrono ch;

  originalArray = common_begin(argc, argv, &len, NULL, NULL);
  int *tmp_array = (int *) malloc(len * sizeof(int));

  ch.start_chrono();
  merge_sort(originalArray, tmp_array, 0, len - 1);
  ch.end_chrono();

  common_end(ch.get_diff(), originalArray, len);

#ifdef THEWALL
  for(int i = 0; i < 10000; i++){
    std::cout << "#T#H#E##W#A#L#L";
  }
  std::cout << "#" << std::endl;
#endif

  free(originalArray);
  free(tmp_array);

  return 0;
}
