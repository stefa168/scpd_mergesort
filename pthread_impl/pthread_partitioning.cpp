#include "pthread_partitioning.h"
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"
#include "../common/mychrono.cpp"



void* p_merge_sort(void *in_args) {
  auto *args = (ms_task *) in_args;
  merge_sort(args->arr, args->b, args->left, args->right);
  pthread_exit(nullptr);
}

int main(int argc, char *argv[]) {
    uint64_t len;
    int *originalArray;
    pthread_t *threads;
    int num_threads, grain;

    Mychrono ch;

    originalArray = common_begin(argc, argv, &len, &grain, &num_threads);

    ch.start_chrono();

    threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
    int size =  len / num_threads;
    int *b = (int *) malloc(len * sizeof(int));

    // create threads
    for(int j = 0; j < num_threads; j++){
        ms_task *args;
        args = (ms_task *) malloc(sizeof(ms_task));
        args->arr = originalArray;
        args->b = b;
        args->left = j * size;
        if(j == num_threads - 1){
          args->right = len - 1;
        } else {
            args->right = (j + 1) * size - 1;
        }
        pthread_create(&threads[j], nullptr, p_merge_sort, (void *) args);
    }

    // waiting threads
    for(int j = 0; j < num_threads; j++){
        pthread_join(threads[j], nullptr);
    }

    // final merge
    #ifdef CLASSIC_MERGE_PARTITION
          merge_sort(originalArray, b, 0, (len - 1));
    #else
          merge_size(originalArray, b, size, len);
    #endif

    ch.end_chrono();
    common_end(ch.get_diff(), originalArray, len);

    free(originalArray);
    free(b);

    return 0;
}
