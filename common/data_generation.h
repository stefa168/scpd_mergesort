#pragma once

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>


#ifdef FILE
int *readDataFromFile(const std::string& file_path);
void write_data_to_file(int *a, int len, std::string file_path, double merge_time);
#endif

void error_args(int *grain, int *num_threads);
int* common_begin(int argc, char **argv, uint64_t *array_len, int *grain, int *num_threads);
void common_end(double time, int *array, uint64_t len);
int *arrayGenerator(int size, unsigned int seed);
void check_order(int *array, int len);
void print_array(int *a, uint64_t len);
