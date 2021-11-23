#include "merge_implementations.h"

using std::cout;
using std::endl;

void merge_size(int *a, int *b, int a_len, int *sizes, int sizes_len){
    int indexes[sizes_len], idx_max[sizes_len];
    int k, i, found;

    idx_max[0] = sizes[0];
    indexes[0] = 0;
    for(i = 1; i < sizes_len; i++){
        idx_max[i] = idx_max[i - 1] + sizes[i];
        indexes[i] = idx_max[i - 1];
    }

    for(k = 0; k < a_len; k++){
        found = -1;
        for(i = 0; i < sizes_len; i++){
            // se l'indice i è nel suo range
            // o si deve inizializzare found
            // o si è trovato un elemento minore di quello attulamente trovato con found
            if(indexes[i] < idx_max[i] && (found == -1 || a[indexes[i]] < a[indexes[found]])) {
                found = i;
            }
        }
        b[k] = a[indexes[found]];
        indexes[found]++;
    }

    for(k = 0; k < a_len; k++){
        a[k] = b[k];
    }
}


void merge_sort(int *a, int *b, uint64_t left, uint64_t right) {
    uint64_t center = (left + right) / 2;

    if (left < right) {
        merge_sort(a, b, left, center);
        merge_sort(a, b, center + 1, right);
        merge(a, b, left, center, right);
    }
}


#ifndef MERGE_LOCAL_MALLOC_IMPL
/* default implementation */
void merge(int *vec, int *b, uint64_t low, uint64_t mid, uint64_t top) {
    uint64_t i = low;
    uint64_t j = mid + 1;
    uint64_t k = low;

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
        vec[k] = b[k];
    }
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
