#include "serial.h"
#include <iostream>
#include <ctime>
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

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