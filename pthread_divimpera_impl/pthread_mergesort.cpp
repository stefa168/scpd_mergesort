#include "pthread_mergesort.h"
#include <iostream>
#include <pthread.h>
#include <chrono>
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

using namespace std;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

void merge_sort(int *a, uint64_t left, uint64_t right) {
    uint64_t center = (left + right) / 2;

    if (left < right) {
        merge_sort(a, left, center);
        merge_sort(a, center + 1, right);
        merge(a, left, center, right);
    }
}

void *p_merge_sort(void *in_args) {
    auto *args = (ms_task *) in_args;
    uint64_t left = args->left;
    uint64_t right = args->right;

    // todo divide the problem in no more than about 1/20ths of it, to avoid segmentation faults
    if ((right - left) > 1000000) {
        if (left < right) {
            uint64_t center = (left + right) / 2;

            ms_task *fst_args;
            fst_args = (ms_task *) malloc(sizeof(ms_task));
            fst_args->arr = args->arr;
            fst_args->left = left;
            fst_args->right = center;

            ms_task *snd_args;
            snd_args = (ms_task *) malloc(sizeof(ms_task));
            snd_args->arr = args->arr;
            snd_args->left = center + 1;
            snd_args->right = right;

            pthread_t thread_left, thread_right;
            pthread_create(&thread_left, nullptr, p_merge_sort, (void *) fst_args);
            pthread_create(&thread_right, nullptr, p_merge_sort, (void *) snd_args);

            pthread_join(thread_left, nullptr);
            pthread_join(thread_right, nullptr);
            merge(args->arr, left, center, right);
        }
    } else {
        merge_sort(args->arr, left, right);
    }

    pthread_exit(nullptr);
}

void pmerge(int *arr, uint64_t size) {
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

    if (argc < 3) {
        std::cerr << "Please specify file path and number count" << endl;
        return 1;
    }

    len = std::stoull(argv[2]);

//    a = readDataFromFile(argv[1]);
    cout << "Generating data... " << flush;
    a = arrayGenerator(len);
    cout << "Done." << endl << "Sorting " << flush;

    auto start = steady_clock::now();
    pmerge(originalArray, len-1);
    auto end = steady_clock::now();

    auto totalTime = duration_cast<milliseconds>(end - start);

    cout << "took " << totalTime.count() << "ms" << endl;

    return 0;
}