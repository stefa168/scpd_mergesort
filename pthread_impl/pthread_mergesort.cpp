#include "pthread_mergesort.h"
#include <iostream>
#include <ctime>
#include <pthread.h>
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

using namespace std;

void *p_merge_sort(void *in_args) {

    ms_task *fst_args;
    ms_task *snd_args;

    auto *args = (ms_task *) in_args;
    int left = args->left;
    int right = args->right;
    int center = (left + right) / 2;

    pthread_t thread_left, thread_right;

    // todo stop branching after a minimum array size.
    if ((right - left) > 50) {
        if (left < right) {
            fst_args = (ms_task *) malloc(sizeof(ms_task));
            fst_args->arr = args->arr;
            fst_args->left = left;
            fst_args->right = center;

            snd_args = (ms_task *) malloc(sizeof(ms_task));
            snd_args->arr = args->arr;
            snd_args->left = center + 1;
            snd_args->right = right;

            pthread_create(&thread_left, nullptr, p_merge_sort, (void *) fst_args);
            pthread_create(&thread_right, nullptr, p_merge_sort, (void *) snd_args);

            pthread_join(thread_left, nullptr);
            pthread_join(thread_right, nullptr);
            merge(args->arr, left, center, right);
        }
    } else {
        merge(args->arr, left, center, right);
    }

    pthread_exit(nullptr);
}

void pmerge(int *arr, int size) {
    // instantiate initial data for the mergesort
    ms_task args;
    args.arr = arr;
    args.left = 0;
    args.right = size;

    pthread_t thread;
    pthread_create(&thread, nullptr, p_merge_sort, &args);

    pthread_join(thread, nullptr);
}

int main(int argc, char *argv[]) {
    uint64_t len;
    int *a;
    double elapsed_time;

    if (argc < 3) {
        std::cerr << "Please specify file path and number count" << endl;
        return 1;
    }

    len = std::stoull(argv[2]);

//    a = readDataFromFile(argv[1]);
    a = arrayGenerator(len);

    clock_t start = clock();
    pmerge(a, len);
    clock_t end = clock();

    elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    cout << "elapsed_time: " << elapsed_time << " sec" << endl;

    return 0;
}