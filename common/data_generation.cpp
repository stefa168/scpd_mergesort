#include "data_generation.h"

using std::string;
using std::stoi;
using std::cout;
using std::endl;
using std::flush;
using std::cerr;


int *arrayGenerator(int size, unsigned int seed) {

  srand(seed);

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
      throw "Array not sorted";
    }
  }
  cout << "The resulting array is correctly sorted in non-descending order." << endl;
}

void error_args(int *num_threads){
  std::cerr << "Please specify the length of the array" << (num_threads != NULL ? ", number of threads" : "") << " and optionally the seed for the random generator" << endl;
  throw "Missing arguments";
}

int* common_begin(int argc, char **argv, uint64_t *array_len, int *num_threads){
  unsigned int seed;
  int *array;

  cout << " ##############################################" << endl;
  cout << " # Implementation of the mergesort algorithm. #" << endl;
  cout << " ##############################################" << endl;

  if(argc > 1) {
    *array_len = std::stoull(argv[1]);

    int total_args = 2;
    if(num_threads != NULL){
      if(argc > 2){
        *num_threads = std::stoull(argv[2]);
        total_args++;
      } else {
        error_args(num_threads);
      }
    }

    if(argc > total_args) {
      seed = std::stoull(argv[total_args]);
    } else {
      seed = time(NULL);
    }

    string str_thread = "";
    if(num_threads) {
      str_thread = ", num_threads: ";
      str_thread.append(argv[2]);
    }
    cout << "Arguments are len: " << *array_len << str_thread << ", seed: " << seed << endl;
  } else {
    error_args(num_threads);
  }

  cout << "Generating data... " << flush;
  array = arrayGenerator(*array_len, seed);
  cout << "Done." << endl << flush;

  #ifdef PRINT_ARRAY
    print_array(array, *array_len);
  #endif

  cout << "Sorting... " << flush;

  return array;
}


void common_end(clock_t start, clock_t end, int *array, uint64_t array_len){
  double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
  cout << "Done. Elapsed_time: " << elapsed_time << " sec" << endl;

  check_order(array, array_len);
#ifdef PRINT_ARRAY
  print_array(array, array_len);
#endif
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
    cout << endl;
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
