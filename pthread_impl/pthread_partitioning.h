#pragma once

#include <cstdint>
#include <cstring>

typedef struct {
    uint64_t left;
    uint64_t right;
} ms_task;

void merge_sort(int *a, uint64_t left, uint64_t right);

void* p_merge_sort(void *in_args);
