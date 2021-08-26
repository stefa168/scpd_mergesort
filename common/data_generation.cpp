#include "data_generation.h"

using std::string;
using std::stoi;
using std::cout;
using std::endl;
using std::flush;
using std::cerr;


int *arrayGenerator(int size, bool enable_seed, unsigned int seed) {
  if(enable_seed) {
    srand(seed);
  } else {
    srand(time(NULL));
  }

  int *vec = static_cast<int *>(calloc(size, sizeof(int)));

  for (int i = 0; i < size; i++) {
      vec[i] = 1 + rand() % size;
  }

  return vec;
}

void check_order(int *array, int len){
  for(int i = 0; i < len - 1; i++){
    if(array[i] > array[i + 1]) {
      cerr << "Values [" << array[i] << ", " << array[i+1] << "] aren't ordered. Their positions are [" << i << ", " << i+1 << "]" << endl;
      throw "Result array not sorted";
    }
  }
  cout << "Result array correctely ordered" << endl;
}


int* common_begin(int argc, char **argv, uint64_t *array_len){
  bool enable_seed;
  unsigned int seed;
  int *array;

  if(argc >= 2) {
    *array_len = std::stoull(argv[1]);
    enable_seed = argc > 2;
    if(enable_seed) {
      seed = std::stoull(argv[2]);
    }
  } else {
    std::cerr << "Please specify the length of the array and optionally the seed for the random generator" << endl;
    throw "Missing arguments";
  }

  cout << "Generating data... " << flush;
  array = arrayGenerator(*array_len, enable_seed, seed);
  cout << "Done." << endl << flush;
  print_array(array, *array_len);

  return array;
}


void common_end(clock_t start, clock_t end, int *array, uint64_t array_len){
  double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
  cout << "elapsed_time: " << elapsed_time << " sec" << endl;

  print_array(array, array_len);
  check_order(array, array_len);
}


void print_array(int *a, uint64_t len) {
    string end_char;
    uint64_t i;

    if(len == 0) {
      cout << "empty array" << endl;
      return;
    }

    end_char = ", ";
    for (i = 0; i < len; i++) {
        if (i == len - 1) {
            end_char = ".";
        }
        cout << a[i] << end_char;

        if(i > 20) {
            cout << "[...]";
            break;
        }
    }
    cout << "\n" << endl;
}


#ifdef FILE
int *readDataFromFile(const string &file_path) {
    std::ifstream MyFile(file_path);
    string text;
    int length, i = 0;
    int *vec;

    getline(MyFile, text);
    length = stoi(text);
    vec = (int *) calloc(length, sizeof(int));

    while (getline(MyFile, text) && i < length) {
        vec[i++] = stoi(text);
    }

    MyFile.close();

    return vec;
}

void write_data_to_file(int *a, int len, std::string file_path, double merge_time) {
    int i;
    std::ofstream MyFile(file_path);

    MyFile << "len: " << len << ", merge time: " << merge_time <<"\n";
    for(i = 0; i < len; i++){
        MyFile << a[i] << "\n";
    }
    MyFile.close();
}
#endif
