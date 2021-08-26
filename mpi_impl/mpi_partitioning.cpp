#include "mpi_partitioning.h"
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

using namespace std;

int main(int argc, char** argv) {
    int myid;
    int num_procs;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    clock_t start, end;
    double elapsed_time;

    uint64_t len;
    int* originalArray;


    if(myid == 0) {

      originalArray = common_begin(argc, argv, &len, NULL);

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

      free(sorted);
      free(other_array);
    }

    free(originalArray);
    free(sub_array);
    free(tmp_array);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

}
