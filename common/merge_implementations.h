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

#ifndef MERGE_LOCAL_MALLOC_IMPL
void merge(int *vec, int *b, uint64_t low, uint64_t mid, uint64_t top);
#else
void merge(int *a, uint64_t low, uint64_t mid, uint64_t high);
#endif
