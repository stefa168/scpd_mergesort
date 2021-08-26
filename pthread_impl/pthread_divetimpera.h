#pragma once

#include <cstdint>
#include <cstring>

#include <unistd.h>


typedef struct {
  uint64_t mitt;
  uint64_t span;
  uint64_t left;
  uint64_t right;
} mydata_t;


void *function_worker(void *args);
