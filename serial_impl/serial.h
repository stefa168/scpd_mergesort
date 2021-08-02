#pragma once

#include <iostream>
#include <ctime>
#include "../common/data_generation.h"

void merge_sort(int *a, uint64_t left, uint64_t right);

void merge(int *a, uint64_t left, uint64_t center, uint64_t right);

void print_array(uint64_t *a, uint64_t len);
