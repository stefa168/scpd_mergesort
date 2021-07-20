#pragma once

#include <iostream>
#include <ctime>
#include "../common/data_loading.h"
#include <pthread.h>

typedef struct {
    int tid;
    int *a;
    int left;
    int right;
} mergesort_thread_data;

void pmerge(int *a, int size);

void *p_merge_sort(void *in_args);

void merge(int *a, int left, int center, int right);
