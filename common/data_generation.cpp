#include "data_generation.h"
#include <string>
#include <fstream>
#include <iostream>

using std::string, std::stoi, std::cout, std::endl;

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
        vec[i] = 1 + rand() % size;
    }

    return vec;
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

        if(i > 20) {
            cout << "[...]" << endl;
            break;
        }
    }
    cout << endl;
}