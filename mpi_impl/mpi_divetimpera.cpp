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

    int cursor [2]; // cursor[0]: span, cursor[1]: array_len

    if (myid == 0) {
      fflush(stdout);
      array = common_begin(argc, argv, &array_len, &grain, NULL);
      cursor[1] = (int) array_len;
      ch.start_chrono();

      tmp_array = (int *) malloc(cursor[1] * sizeof(int));

      // span between the myid and the id of the other processes based on the level of the tree you are working on
      cursor[0] = num_procs / 2;
      init_span = cursor[0];
    } else {
      // processes in the internal nodes of the tree: receive all data (span, array_len and array)
      MPI_Recv(&cursor, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      mitt = status.MPI_SOURCE % num_procs;
      init_span = cursor[0];
      array = (int *) malloc(cursor[1] * sizeof(int));
      tmp_array = (int *) malloc(cursor[1] * sizeof(int));
      MPI_Recv(array, cursor[1], MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    }

    dest = myid + cursor[0];
    cursor[0] = cursor[0] / 2;
    while(dest > myid){   // when span to previous cycle was zero
      // divide array
      cursor[1] = cursor[1] / 2;

      // send second half of array
      MPI_Send(&cursor, 2, MPI_INT, dest, 0, MPI_COMM_WORLD);
      MPI_Send(&array[cursor[1]], cursor[1], MPI_INT, dest, 0, MPI_COMM_WORLD);

      // calculation of the id of the next process
      dest = myid + cursor[0];
      cursor[0] = cursor[0] / 2;
    }

    // only if it sent to all process
    if(cursor[0] == 0) {
      cursor[0] = 1;
      dest = myid + cursor[0];
    }

    // sorted first half of array
    merge_sort(array, tmp_array, 0, cursor[1] - 1);

    while(cursor[0] <= init_span){
      // get the second half of the array computed by the other process
      MPI_Recv(&array[cursor[1]], cursor[1], MPI_INT, dest, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // combines the two ordered subarrays
      cursor[1] = 2 * cursor[1];
      merge(array, tmp_array, 0, ((cursor[1] - 1) / 2), cursor[1] - 1);

      // calculation of the id of the previous process
      cursor[0] = cursor[0] * 2;
      dest = cursor[0] + myid;
    }

    if(myid == 0) {
      ch.end_chrono();
      common_end(ch.get_diff(), array, cursor[1]);

    } else {
      // send subarray sorted
      MPI_Send(array, cursor[1], MPI_INT, mitt, 0, MPI_COMM_WORLD);
    }

    free(array);
    free(tmp_array);

    MPI_Finalize();
}
