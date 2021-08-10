#include "serial.h"
#include <iostream>
#include <cstring>
#include <chrono>
#include "../common/data_generation.h"
#include "../common/merge_implementations.h"

using std::cout, std::endl, std::flush, std::string;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

void merge_sort(int *a, uint64_t left, uint64_t right) {
    uint64_t center = (left + right) / 2;

    if (left < right) {
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
    cout << "Generating data... " << flush;
    int *originalArray = arrayGenerator(len);
    cout << "Done." << endl << flush;

    unsigned long totalTime = 0;
    for (int i = 0; i < 10; ++i) {
        int *t = static_cast<int *>(calloc(len, sizeof(int)));
        memcpy(t, originalArray, len);

        cout << "Run " << i + 1 << "- sorting " << flush;

        auto start = steady_clock::now();
        merge_sort(t, b, 0, len-1);
        auto end = steady_clock::now();

        free(t);

        auto runTime = duration_cast<milliseconds>(end - start);
        cout << "took " << runTime.count() << "ms" << endl << flush;
        totalTime += runTime.count();
    }

    cout << "10 Runs took on average " << totalTime / 10 << "ms." << endl;

    return 0;
}