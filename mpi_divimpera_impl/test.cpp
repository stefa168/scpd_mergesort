#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "../common/data_generation.h"
#include <iostream>
#include <string>

using namespace std;

void merge_sort(int *, int *, int, int);
void merge_size(int *, int *, int, int);

int main(int argc, char** argv) {
    int myid;
    int num_procs;

    clock_t start, end;
    double elapsed_time;

    uint64_t len;
    int* original_array;


    /********** Create and populate the array **********/
    if (argc < 3) {
        std::cerr << "Please specify input file, output file path" << endl;
        return 1;
    }

    len = std::stoull(argv[2]);
    cout << "Generating data... " << flush;
    originalArray = arrayGenerator(len);
    cout << "Done." << endl << flush;



    /********** Initialize MPI **********/
    MPI_Init(&argc, &argv);
    start = clock();
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);



    /********** Divide the array in equal-sized chunks **********/
    int size = len / num_procs;
//	if(myid == 0){
//	   std::cout << "length: " << len << "\nsize: " << size << std::endl;
//           for(int tre = 0; tre < len; tre++){
//               std::cout << "a[" << tre << "]\t" << original_array[tre] << std::endl;
//           }
//	   std::cout << std::endl;
//	}


    /********** Send each subarray to each process **********/
    int *sub_array = (int *) malloc(size * sizeof(int));
    MPI_Scatter(original_array, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

    /********** Perform the merge_sort on each process **********/
    int *tmp_array = (int *) malloc(size * sizeof(int));
    merge_sort(sub_array, tmp_array, 0, (size - 1));

    /********** Gather the sorted subarrays into one **********/
    int *sorted = NULL;
    if(myid == 0) {
        sorted = (int *) malloc(len * sizeof(int));
    }

    MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

    /********** reorder all size subarray **********/
    if(myid == 0) {

//		std::cout << "size: " << size << std::endl;
//		for(int tre = 0; tre < len; tre++){
//			std::cout << "s[" << tre << "]\t" << sorted[tre] << std::endl;
//		}
//		std::cout << std::endl;

        int *other_array = (int *) malloc(len * sizeof(int));
//		merge_sort(sorted, other_array, 0, (len - 1));
        merge_size(sorted, other_array, size, len-1);

//		std::cout << std::endl;
//              for(int tre = 0; tre < len; tre++){
//                        std::cout << "s[" << tre << "]\t" << sorted[tre] << std::endl;
//                }


        /********** Display the sorted array **********/
        end = clock();
        elapsed_time = double(end - start) / CLOCKS_PER_SEC;
        std::cout << "elapsed_time: " << elapsed_time << " sec" << std::endl;

        write_data_to_file(sorted, len, argv[2], elapsed_time);

        /********** Clean up root **********/
        free(sorted);
        free(other_array);

    }

    /********** Clean up rest **********/
    free(original_array);
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
        indexes[i].max = (i + 1) * size;
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
