#include "mpi_partitioning.h"
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"
#include "../common/mychrono.cpp"

using namespace std;

int main(int argc, char** argv) {
    int myid;
    int num_procs;

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int proc_name_len;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Get_processor_name(processor_name, &proc_name_len);

    uint64_t len;
    int *originalArray = NULL;
    int grain;

    Mychrono ch;

//    cout << "i am " << myid << " on " << processor_name << endl;
    printf("i am %d on %s\n", myid, processor_name);
    MPI_Barrier(MPI_COMM_WORLD);

    if(myid == 0) {
      originalArray = common_begin(argc, argv, &len, &grain, NULL);

      ch.start_chrono();
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
#ifdef CLASSIC_MERGE_PARTITION
      merge_sort(sorted, other_array, 0, (len - 1));
#else
      merge_size(sorted, other_array, size, len);
#endif

      ch.end_chrono();
      common_end(ch.get_diff(), sorted, len);

      free(sorted);
      free(other_array);

      free(originalArray);
    }

    free(sub_array);
    free(tmp_array);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

}
