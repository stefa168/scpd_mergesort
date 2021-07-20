#include "pthread_mergesort.h"
#include <pthread.h>

using namespace std;

int *a;

void merge(int left, int center, int right) {
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

void pmerge(int size) {
    // istantiate first data for the mergesort
    mergesort_thread_data *args;

    args = (mergesort_thread_data *) malloc(sizeof(mergesort_thread_data));
    args->left = 0;
    args->right = size;
//    p_merge_sort(&args);
    pthread_t thread;
    pthread_create(&thread, NULL, p_merge_sort, (void *) args);

    pthread_join(thread, nullptr);
}

void *p_merge_sort(void *in_args) {
    int center;

    mergesort_thread_data *fst_args;
    mergesort_thread_data *snd_args;
    mergesort_thread_data *args = (mergesort_thread_data *) in_args;
    int left = args->left;
    int right = args->right;

    std::cout << "l: " << left << ", r: " << right << std::endl;

    pthread_t thread;

    if (left < right) {
        center = (left + right) / 2;

        fst_args = (mergesort_thread_data *) malloc(sizeof(mergesort_thread_data));
        fst_args->left = left;
        fst_args->right = center;
        pthread_create(&thread, NULL, p_merge_sort, (void *) fst_args);

        snd_args = (mergesort_thread_data *) malloc(sizeof(mergesort_thread_data));
        snd_args->left = center + 1;
        snd_args->right = right;
        p_merge_sort(snd_args);

        pthread_join(thread, nullptr);
        merge(left, center, right);
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

int main(int argc, char *argv[]) {
    int len = 0, i;
    double elapsed_time;

    if (argc < 3) {
        std::cerr << "Please specify file path and number count" << endl;
        return 1;
    }

    len = std::stoi(argv[2]);

    a = readDataFromFile(argv[1]);

//    cout << "Original array" << endl;
//    print_array(a, len);

    clock_t start = clock();
    pmerge(len);
    clock_t end = clock();

//    cout << "Ordered array" << endl;
    print_array(a, 10);

    elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    cout << "elapsed_time: " << elapsed_time << " sec" << endl;

    return 0;
}