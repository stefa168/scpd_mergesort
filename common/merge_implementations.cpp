#include "merge_implementations.h"

void merge(int *a, uint64_t left, uint64_t center, uint64_t right) {
    uint64_t i = left;
    uint64_t j = center + 1;
    uint64_t k = 0;
    int *b = static_cast<int *>(calloc(right - left + 1, sizeof(int)));

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

    free(b);
}
