#include "pthread_mergesort.h"
#include <iostream>
#include <ctime>
#include <pthread.h>
#include "../common/data_generation.h"

using namespace std;

void merge(int *a, int left, int center, int right) {
    int i = left;
    int j = center + 1;
    int k = 0;
    int b[right - left + 1];

    while (i <= center && j <= right) {
        if (a[i] <= a[j]) {
            b[k] = a[i];
            i++;
        } else {
            b[k] = a[j];
            j++;
        }
        k++;
    }

    while (i <= center) {
        b[k] = a[i];
        k++;
        i++;
    }

    while (j <= right) {
        b[k] = a[j];
        k++;
        j++;
    }

    for (k = left; k <= right; k++) {
        a[k] = b[k - left];
    }
}

void *p_merge_sort(void *in_args) {
    mergesort_thread_data *fst_args;
    mergesort_thread_data *snd_args;

    auto *args = (mergesort_thread_data *) in_args;
    int left = args->left;
    int right = args->right;
    int center = (left + right) / 2;

    pthread_t thread_left, thread_right;

    // todo stop branching after a minimum array size.
    if ((right - left) > 50){
        if (left < right) {
            fst_args = (mergesort_thread_data *) malloc(sizeof(mergesort_thread_data));
            fst_args->arr = args->arr;
            fst_args->left = left;
            fst_args->right = center;

            snd_args = (mergesort_thread_data *) malloc(sizeof(mergesort_thread_data));
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

void print_array(int *a, int len) {
    string end_char;
    int i;

    end_char = ", ";
    for (i = 0; i < len; i++) {
        if (i == len - 1) {
            end_char = ".";
        }
        cout << a[i] << end_char;
    }
    cout << endl;
}

void pmerge(int *arr, int size) {
    // instantiate initial data for the mergesort
    mergesort_thread_data args;
    args.arr = arr;
    args.left = 0;
    args.right = size;

    pthread_t thread;
    pthread_create(&thread, nullptr, p_merge_sort, &args);

    pthread_join(thread, nullptr);
}

int main(int argc, char *argv[]) {
    int len;
    int *a;
    double elapsed_time;

    if (argc < 3) {
        std::cerr << "Please specify file path and number count" << endl;
        return 1;
    }

    len = std::stoi(argv[2]);

    a = readDataFromFile(argv[1]);

    clock_t start = clock();
    pmerge(a, len);
    clock_t end = clock();

    elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    cout << "elapsed_time: " << elapsed_time << " sec" << endl;

    return 0;
}