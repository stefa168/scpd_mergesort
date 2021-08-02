#include "serial.h"
#include <iostream>
#include <ctime>
#include "../common/data_generation.h"

using std::cout, std::endl, std::string;

void merge_sort(int *a, uint64_t left, uint64_t right) {
    uint64_t center;
    if (left < right) {
        center = (left + right) / 2;
        merge_sort(a, left, center);
        merge_sort(a, center + 1, right);
        merge(a, left, center, right);
    }
}

void merge(int *a, uint64_t left, uint64_t center, uint64_t right) {
    uint64_t i = left;
    uint64_t j = center + 1;
    uint64_t k = 0;
//    int b[right - left + 1];
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


void print_array(int *a, uint64_t len) {
    string end_char;
    uint64_t i;

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
    int *a;
    uint64_t len = 0, i;
    double elapsed_time;

    if (argc < 3) {
        std::cerr << "Please specify file path and number count" << endl;
        return 1;
    }

    len = std::stoull(argv[2]);

    cout << len << std::endl;

//    a = readDataFromFile(argv[1]);
    a = arrayGenerator(len);

//    cout << "Original array" << endl;
//    print_array(a, len);

    clock_t start = clock();
    merge_sort(a, 0, len);
    clock_t end = clock();

//    cout << "Ordered array" << endl;
//    print_array(a, len);
    elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    cout << "elapsed_time: " << std::fixed << elapsed_time << " sec" << endl;

    return 0;
}