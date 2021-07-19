cmake_minimum_required(VERSION 3.19)
project(parallel_merge_sort)

set(CMAKE_CXX_STANDARD 14)

add_executable(serial_merge_sort serial_impl/main.cpp)