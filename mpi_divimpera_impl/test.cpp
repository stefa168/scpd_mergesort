#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stack>
#include <string>
#include <ctime>
#include <fstream>
#include <cmath>

using namespace std;


void merge_sort(int *, int *, int, int);
void merge_size(int *, int *, int, int);

void merge(int *vec, int *b, int low, int mid, int top);
void print_array(int *a, int len);
void check_order(int *a, int len);
int *arrayGenerator(int size, bool enable_seed, unsigned int seed);


int main(int argc, char** argv) {
    int myid;
    int num_procs;


    /********** Initialize MPI **********/
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    clock_t start, end;
    double elapsed_time;

    int len;
    unsigned int seed;
    bool enable_seed;
    int* originalArray;


    if(myid == 0) {

      if(argc >= 2) {
        len = std::stoull(argv[1]);
        enable_seed = argc > 2;
        if(enable_seed) {
          seed = std::stoull(argv[2]);
        }
      } else {
        std::cerr << "Please specify the length of the array and optionally the seed for the random generator" << endl;
        throw "Missing arguments";
      }

      cout << "Generating data... " << flush;
      originalArray = arrayGenerator(len, enable_seed, seed);
      cout << "Done." << endl << flush;

      start = clock();

    }

    // send len array to all process in broadcast
    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // divide array into equals subarrays
    int size = len / num_procs;

    // send the subarrays at other processes
    int *sub_array = (int *) malloc(size * sizeof(int));;
    MPI_Scatter(originalArray, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

    // every process orders its subarray
    int *tmp_array = (int *) malloc(size * sizeof(int));
    merge_sort(sub_array, tmp_array, 0, (size - 1));


    int *sorted = NULL;
    if(myid == 0) {
        sorted = (int *) malloc(len * sizeof(int));
    }

    // gather all subarray in the "main" process
    MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

    if(myid == 0) {

      int *other_array = (int *) malloc(len * sizeof(int));

      // reorder all subarray (already ordered)
#ifdef CLASSIC_MERGE
      merge_sort(sorted, other_array, 0, (len - 1));
#else
      merge_size(sorted, other_array, size, len);
#endif

      end = clock();
      elapsed_time = double(end - start) / CLOCKS_PER_SEC;
      std::cout << "elapsed_time: " << elapsed_time << " sec" << std::endl;

      print_array(sorted, len);
      check_order(sorted, len);

      /********** Clean up root **********/
      free(sorted);
      free(other_array);
    }

    /********** Clean up rest **********/
    free(originalArray);
    free(sub_array);
    free(tmp_array);

    /********** Finalize MPI **********/
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

}

typedef struct {
    int i;
    int max;
} index_type;

void merge_size(int *a, int *b, int size, int len){
    int num_idx = len / size;
    index_type indexes[num_idx];
    int k, i, found;

    for(i = 0; i < num_idx; i++){
        indexes[i].i = i * size;
        if(i == num_idx - 1){
          indexes[i].max = len;
        } else {
          indexes[i].max = (i + 1) * size;
        }
    }

    for(k = 0; k < len; k++){
        found = -1;
        for(i = 0; i < num_idx; i++){
            // se l'indice i è nel suo range
            // o si deve inizializzare found
            // o si è trovato un elemento minore di quello attulamente trovato con found
            if(indexes[i].i < indexes[i].max && (found == -1 || a[indexes[i].i] < a[indexes[found].i])) {
                found = i;
            }
        }
        b[k] = a[indexes[found].i];
        indexes[found].i++;
    }

    for(k = 0; k < len; k++){
        a[k] = b[k];
    }
}

void merge_sort(int *a, int *b, int l, int r) {
    int m;

    if(l < r) {
        m = (l + r)/2;
        merge_sort(a, b, l, m);
        merge_sort(a, b, (m + 1), r);
        merge(a, b, l, m, r);
    }
}




/**************************************************/

void merge(int *vec, int *b, int low, int mid, int top) {
    int i = low;
    int j = mid + 1;
    int k = low;

    while (i <= mid && j <= top) {
        if (vec[i] <= vec[j]) {
            b[k] = vec[i];
            i++;
        } else {
            b[k] = vec[j];
            j++;
        }
        k++;
    }

    while (i <= mid) {
        b[k] = vec[i];
        k++;
        i++;
    }

    while (j <= top) {
        b[k] = vec[j];
        k++;
        j++;
    }

    for (k = low; k <= top; k++) {
        vec[k] = b[k];
    }
}

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
      std::cerr << "Values [" << array[i] << ", " << array[i+1] << "] aren't ordered. Their positions are [" << i << ", " << i+1 << "]" << endl;
      throw "Result array not sorted";
    }
  }
  cout << "Result array correctely ordered" << endl;
}

void print_array(int *a, int len) {
    string end_char;
    int i;

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
            cout << "[...]" << endl;
            break;
        }
    }
    cout << endl << flush;
}
