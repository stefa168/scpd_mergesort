cmake_minimum_required(VERSION 3.19)
project(parallel_merge_sort)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_CFLAGS} -fopenmp")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG -g")

set(COMMON_LIBS common/data_loading.cpp common/data_loading.h)

add_executable(serial_merge_sort serial_impl/serial.cpp serial_impl/serial.h common/data_loading.cpp common/data_loading.h)