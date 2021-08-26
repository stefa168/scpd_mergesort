#include "merge_implementations.h"

using std::cout;
using std::endl;


void merge_size(int *a, int *b, int size, int len){
    int num_idx = len / size;
    index_type indexes[num_idx];
    int k, i, found;

    for(i = 0; i < num_idx; i++){
        indexes[i].i = i * size;
        if(i == num_idx - 1){
          indexes[i].max = len;
        } else {
          indexes[i].max = (i + 1) * size;
        }
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


void merge_sort(int *a, int *b, uint64_t left, uint64_t right) {
    uint64_t center = (left + right) / 2;

    if (left < right) {
        merge_sort(a, b, left, center);
        merge_sort(a, b, center + 1, right);
        merge(a, b, left, center, right);
    }
}

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
