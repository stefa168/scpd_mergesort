#pragma once

#include <cstdlib>
#include <cstdint>
#include <iostream>


typedef struct {
    int i;
    int max;
} index_type;


void merge_size(int *a, int *b, int size, int length);
void merge_sort(int *a, int *b, uint64_t left, uint64_t right);
void merge(int *vec, int *b, uint64_t low, uint64_t mid, uint64_t top);
