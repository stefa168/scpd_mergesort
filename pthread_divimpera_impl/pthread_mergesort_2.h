#pragma once

#include <cstdint>
#include <cstring>

typedef struct {
    int *arr;
    int *b;
    uint64_t left;
    uint64_t right;
} ms_task;

void merge_sort(int *a, uint64_t left, uint64_t right);

void* p_merge_sort(void *in_args);


void merge_size(int *a, int *b, int size, int len);
