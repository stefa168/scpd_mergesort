#include <cstdlib>
#include "merge_implementations.h"

#define MERGE_ONE_MALLOC_IMPL

#ifdef MERGE_ONE_MALLOC_IMPL

void merge(int *vec, uint64_t low, uint64_t mid, uint64_t top) {
    uint64_t i = low;
    uint64_t j = mid + 1;
    uint64_t k = 0;
    int *b = static_cast<int *>(calloc(top - low + 1, sizeof(int)));

    while (i <= mid && j <= top) {
        if (vec[i] <= vec[j]) {
            b[k] = vec[i];
            i++;
        } else {
            b[k] = vec[j];
            j++;
        }
        k++;
    }

    while (i <= mid) {
        b[k] = vec[i];
        k++;
        i++;
    }

    while (j <= top) {
        b[k] = vec[j];
        k++;
        j++;
    }

    for (k = low; k <= top; k++) {
        vec[k] = b[k - low];
    }

    free(b);
}

#else

void merge(int *a, uint64_t low, uint64_t mid, uint64_t high) {

    // leftSize is size of left part and rightSize is size of right part
    uint64_t leftSize = mid - low + 1;
    uint64_t rightSize = high - mid;

    int *left = static_cast<int *>(calloc(leftSize, sizeof(int)));
    int *right = static_cast<int *>(calloc(rightSize, sizeof(int)));

    uint64_t i, j;

    // Copy first half of values in *left
    for (i = 0; i < leftSize; i++)
        left[i] = a[i + low];

    // Do the same for the other half
    for (i = 0; i < rightSize; i++)
        right[i] = a[i + mid + 1];

    uint64_t k = low;

    i = j = 0;

    // Merge left and right in ascending order
    while (i < leftSize && j < rightSize) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }

    // Copy remaining values from left
    while (i < leftSize)
        a[k++] = left[i++];

    // Copy remaining values from right
    while (j < rightSize)
        a[k++] = right[j++];

    free(left);
    free(right);
}

#endif