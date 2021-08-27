#include "mpi_divetimpera.h"
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

using namespace std;

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

    int *array, *tmp_array;
    uint64_t array_len;

    clock_t start, end;
    int grain;

    int dest, span, init_span, mitt;


    if (myid == 0) {


      start = clock();

      tmp_array = (int *) malloc(array_len * sizeof(int));

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
      common_end(start, end, array, array_len);

    } else {
      // send subarray sorted
      MPI_Send(array, array_len, MPI_INT, mitt, 0, MPI_COMM_WORLD);
    }

    free(array);
    free(tmp_array);


    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
