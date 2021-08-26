#pragma once

#include <cstdint>
#include <cstring>

typedef struct {
    int *arr;
    int *b;
    uint64_t left;
    uint64_t right;
} ms_task;


void pmerge(int *arr, uint64_t size);
void *p_merge_sort(void *in_args);
