#pragma once

#include <iostream>
#include <ctime>
#include "../common/data_loading.h"

void merge_sort(int *a, int left, int right);

void merge(int *a, int left, int center, int right);

void print_array(int *a, int len);
