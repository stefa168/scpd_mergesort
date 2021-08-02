#pragma once

#include <string>

int *readDataFromFile(const std::string& file_path);

int *arrayGenerator(uint64_t size);

void print_array(int *a, uint64_t len);