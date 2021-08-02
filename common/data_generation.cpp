#include "data_generation.h"
#include <string>
#include <fstream>

using std::string, std::stoi;

int *readDataFromFile(const string &file_path) {
    std::ifstream MyFile(file_path);
    string text;
    int length, i = 0;
    int *vec;

    getline(MyFile, text);
    length = stoi(text);
    vec = (int *) calloc(length, sizeof(int));

    while (getline(MyFile, text) && i < length) {
        vec[i++] = stoi(text);
    }

    MyFile.close();

    return vec;
}

int *arrayGenerator(uint64_t size) {
    int *vec = static_cast<int *>(calloc(size, sizeof(int)));

    for (uint64_t i = 0; i < size; i++) {
        vec[i] = rand() % size;
    }

    return vec;
}
