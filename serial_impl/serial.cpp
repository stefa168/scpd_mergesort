#include "serial.h"
#include <iostream>
#include <ctime>
#include <cstring>
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

using std::cout, std::endl, std::string;

void merge_sort(int *a, uint64_t left, uint64_t right) {
    if (left < right) {
        uint64_t center = (left + right) / 2;
        merge_sort(a, left, center);
        merge_sort(a, center + 1, right);
        merge(a, left, center, right);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Please specify file path and number count" << endl;
        return 1;
    }

    uint64_t len = std::stoull(argv[2]);

    // a = readDataFromFile(argv[1]);
    int *originalArray = arrayGenerator(len);

    double totalTime = 0;
    for (int i = 0; i < 10; ++i) {
        int *t = static_cast<int *>(calloc(len, sizeof(int)));
        memcpy(t, originalArray, len);

        clock_t start = clock();
        merge_sort(t, 0, len);
        clock_t end = clock();

        free(t);

        double time = double(end - start) / CLOCKS_PER_SEC;
        printf("%d: %f\n", i, time);

        totalTime += time;
    }

    cout << "elapsed_time: " << std::fixed << (totalTime / 10) << " sec" << endl;

    return 0;
}