#include "mpi_divetimpera.h"
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"
#include "../common/mychrono.cpp"


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

using std::cout;
using std::endl;

// it must be a negative number.
#define EXIT_VAL -8

int main(int argc, char** argv) {
    int myid;
    int num_procs;

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int proc_name_len;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Get_processor_name(processor_name, &proc_name_len);

    MPI_Status status;

    int *array, *tmp_array;
    uint64_t array_len;
    int grain = 0;

    int dest, span, init_span;
    int mitt = -1;

    Mychrono ch;

    printf("i am %d on %s\n", myid, processor_name);
    MPI_Barrier(MPI_COMM_WORLD);

    void **cursor = (void **) malloc(2 * sizeof(void *));
    cursor[0] = &span;
    cursor[1] = &array_len;

    if (myid == 0) {
      fflush(stdout);
      array = common_begin(argc, argv, &array_len, &grain, NULL);
      ch.start_chrono();

      tmp_array = (int *) malloc(array_len * sizeof(int));

      // span between the myid and the id of the other processes based on the level of the tree you are working on
      span = num_procs / 2;
      init_span = span;
    } else {
      // processes in the internal nodes of the tree: receive all data (span, array_len and array)
      MPI_Recv(&cursor, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      if(span == EXIT_VAL){ // exit value
        MPI_Finalize();
        return 0;
      }
      mitt = status.MPI_SOURCE % num_procs;
      init_span = span;
      array = (int *) malloc(array_len * sizeof(int));
      tmp_array = (int *) malloc(array_len * sizeof(int));
      MPI_Recv(array, array_len, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    }

    dest = myid + span;
    span = span / 2;
    while(array_len > grain && dest > myid){   // when span to previous cycle was zero
      // divide array
      array_len = array_len / 2;

      // send second half of array
      MPI_Send(&cursor, 2, MPI_INT, dest, 0, MPI_COMM_WORLD);
      MPI_Send(&array[array_len], array_len, MPI_INT, dest, 0, MPI_COMM_WORLD);

      // calculation of the id of the next process
      dest = myid + span;
      span = span / 2;
    }

    // only if it sent to all process
    if(span == 0) {
      span = 1;
      dest = myid + span;
    }

    // sorted first half of array
    merge_sort(array, tmp_array, 0, array_len - 1);

    while(array_len > grain && span <= init_span){
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
      int exit = EXIT_VAL;
      for(int i = 1; i < num_procs; i++){
        MPI_Send(&exit, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      }

      ch.end_chrono();
      common_end(ch.get_diff(), array, array_len);

    } else {
      // send subarray sorted
      MPI_Send(array, array_len, MPI_INT, mitt, 0, MPI_COMM_WORLD);
    }

    free(array);
    free(tmp_array);

    MPI_Finalize();
}
