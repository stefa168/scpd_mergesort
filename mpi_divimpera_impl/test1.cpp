#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <stack>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;


void merge_sort(int *vec, int *b, int left, int right);

void merge(int *vec, int *b, int low, int mid, int top);
void print_array(int *a, int len);
void check_order(int *a, int len);
int *arrayGenerator(int size, bool enable_seed, unsigned int seed);


/*

For a correct execution the number of processes and the length of the array must be a power of 2.
Example of the tree with 8 processes.
            P0
        /       \
      P0         P4
   /     \     /     \
  P0    P2    P4    P6
 /  \  /  \  /  \  / \
P0 P1 P2 P3 P4 P5 P6 P7

*/



int main(int argc, char** argv) {
    int myid;
    int num_procs;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    MPI_Status status;

    int *array, *tmp_array, array_len;
    unsigned int seed;
    bool enable_seed;

    clock_t start, end;
    double elapsed_time;

    int dest, span, init_span, mitt;


    if (myid == 0) {

      if(argc >= 2) {
        array_len = std::stoull(argv[1]);
        enable_seed = argc > 2;
        if(enable_seed) {
          seed = std::stoull(argv[2]);
        }
      } else {
        std::cerr << "Please specify the length of the array and optionally the seed for the random generator" << endl;
        throw "Missing arguments";
      }


      cout << "Generating data... " << flush;
      array = arrayGenerator(array_len, enable_seed, seed);
      cout << "Done." << endl << flush;
      tmp_array = (int *) malloc(array_len * sizeof(int));

      print_array(array, array_len);

      start = clock();

      // span between the myid and the id of the other processes based on the level of the tree you are working on
      span = num_procs / 2;
      init_span = span;
    } else {
      // processes in the internal nodes of the tree: receive all data (span, array_len and array)
      MPI_Recv(&span, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      mitt = status.MPI_SOURCE % num_procs;
      init_span = span;

      MPI_Recv(&array_len, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      array = (int *) malloc(array_len * sizeof(int));
      tmp_array = (int *) malloc(array_len * sizeof(int));
      MPI_Recv(array, array_len, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    dest = myid + span;
    span = span / 2;
    while(dest > myid){   // when span to previous cycle was zero
      // divide array
      array_len = array_len / 2;

      // send second half of array
      MPI_Send(&span, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
      MPI_Send(&array_len, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
      MPI_Send(&array[array_len], array_len, MPI_INT, dest, 0, MPI_COMM_WORLD);

      // calculation of the id of the next process
      dest = myid + span;
      span = span / 2;
    }

    /*
     * operations inside the "if" are necessary but the control of the "if" is useless,
     * it was written for any future extra conditions in the while
     * (e.g. adding a gain for the subarray size)
    */
    if(span == 0) {
      span = 1;
      dest = myid + span;
    }

    // sorted first half of array
    merge_sort(array, tmp_array, 0, array_len - 1);

    while(span <= init_span){
      // get the second half of the array computed by the other process
      MPI_Recv(&array[array_len], array_len, MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // combines the two ordered subarrays
      array_len = 2 * array_len;
      merge(array, tmp_array, 0, ((array_len - 1) / 2), array_len - 1);

      // calculation of the id of the previous process
      span = span * 2;
      dest = span + myid;
    }

    if(myid == 0) {
      end = clock();
      elapsed_time = double(end - start) / CLOCKS_PER_SEC;
      std::cout << "elapsed_time: " << elapsed_time << " sec" << std::endl;

      print_array(array, array_len);
      check_order(array, array_len);

    } else {
      // send subarray sorted
      MPI_Send(array, array_len, MPI_INT, mitt, 0, MPI_COMM_WORLD);
    }

    free(array);
    free(tmp_array);


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
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


/*****************************************************/

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
