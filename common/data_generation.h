#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>


#ifdef FILE
int *readDataFromFile(const std::string& file_path);
void write_data_to_file(int *a, int len, std::string file_path, double merge_time);
#endif

int* common_begin(int argc, char **argv, uint64_t *array_len);
void common_end(clock_t start, clock_t end, int *array, uint64_t len);
int *arrayGenerator(int size, bool enable_seed, unsigned int seed);
void check_order(int *array, int len);
void print_array(int *a, uint64_t len);
