#pragma once

typedef struct {
    int *arr;
    int left;
    int right;
} mergesort_thread_data;

void pmerge(int *arr, int size);

void *p_merge_sort(void *in_args);

void merge(int *a, int left, int center, int right);
