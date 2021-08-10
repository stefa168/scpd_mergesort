#include "pthread_mergesort_2.h"
#include <iostream>
#include <pthread.h>
#include <chrono>
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

using std::cout, std::endl, std::flush, std::string;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;


void merge_sort(int *a, int *b, uint64_t left, uint64_t right) {
    uint64_t center = (left + right) / 2;

    if (left < right) {
        merge_sort(a, b, left, center);
        merge_sort(a, b, center + 1, right);
        merge(a, b, left, center, right);
    }
}


void* p_merge_sort(void *in_args) {
  auto *args = (ms_task *) in_args;
  merge_sort(args->arr, args->b, args->left, args->right);
  pthread_exit(nullptr);
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


#define ROUND 1
int main(int argc, char *argv[]) {
    uint64_t len;
    int *originalArray;
    pthread_t *threads;
    int num_threads;

    if (argc < 3) {
        std::cerr << "Please specify file path and number count" << endl;
        return 1;
    }

    len = std::stoull(argv[2]);
    cout << "Generating data... " << flush;
    originalArray = arrayGenerator(len);
    cout << "Done." << endl << flush;

    if(argc > 3) {
      num_threads = std::stoull(argv[3]);
    } else {
      num_threads = 8;
    }
    threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
    int size =  len / num_threads;
    int *b = static_cast<int *>(calloc(len, sizeof(int)));

    cout << "num_threads: " << num_threads << ", len: " << len <<  ", size: " << size << endl;
    print_array(originalArray, len);

    cout << "Sorting " << flush;

    auto start = steady_clock::now();

    // create threads
    for(int j = 0; j < num_threads; j++){
        ms_task *args;
        args = (ms_task *) malloc(sizeof(ms_task));
        args->arr = originalArray;
        args->b = b;
        args->left = j * size;
        args->right = (j + 1) * size-1;
        pthread_create(&threads[j], nullptr, p_merge_sort, (void *) args);
    }

    // waiting threads
    for(int j = 0; j < num_threads; j++){
        pthread_join(threads[j], nullptr);
    }

    // final merge
    merge_size(originalArray, b, size, len);
    auto end = steady_clock::now();
    auto runTime = duration_cast<milliseconds>(end - start);
    cout << "took " << runTime.count() << "ms" << endl << flush;

    print_array(originalArray, len);



    return 0;
}
