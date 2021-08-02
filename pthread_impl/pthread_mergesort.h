#pragma once

typedef struct {
    int *arr;
    int left;
    int right;
} ms_task;

void pmerge(int *arr, int size);

void *p_merge_sort(void *in_args);
