#include "pthread_divetimpera_uncontrolled.h"
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"
#include "../common/mychrono.c"

using namespace std;

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
            fst_args->b = args->b;
            fst_args->left = left;
            fst_args->right = center;

            ms_task *snd_args;
            snd_args = (ms_task *) malloc(sizeof(ms_task));
            snd_args->arr = args->arr;
            snd_args->b = args->b;
            snd_args->left = center + 1;
            snd_args->right = right;

            pthread_t thread_left, thread_right;
            pthread_create(&thread_left, nullptr, p_merge_sort, (void *) fst_args);
#ifdef TWO_PTHREAD
            pthread_create(&thread_right, nullptr, p_merge_sort, (void *) snd_args);
#else
            p_merge_sort(snd_args);
#endif

            pthread_join(thread_left, nullptr);
#ifdef TWO_PTHREAD
            pthread_join(thread_right, nullptr);
#endif
            merge(args->arr, args->b, left, center, right);
        }
    } else {
        merge_sort(args->arr, args->b, left, right);
    }

#ifdef TWO_PTHREAD
    pthread_exit(nullptr);
#else
    return NULL;
#endif
}

void pmerge(int *arr, uint64_t size) {
    // instantiate initial data for the mergesort
    ms_task args;
    args.arr = arr;
    args.b = static_cast<int *>(calloc(size, sizeof(int)));
    args.left = 0;
    args.right = size;

    pthread_t thread;
    pthread_create(&thread, nullptr, p_merge_sort, &args);

    pthread_join(thread, nullptr);
}

int main(int argc, char *argv[]) {
    uint64_t len;
    int *originalArray;
    int grain;

    Mychrono ch;

    originalArray = common_begin(argc, argv, &len, &grain, NULL);

    ch.start_chrono();
    pmerge(originalArray, len - 1);
    ch.end_chrono();

    common_end(ch.get_diff(), originalArray, len);
    free(originalArray);

    return 0;
}
