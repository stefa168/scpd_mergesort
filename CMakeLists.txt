cmake_minimum_required(VERSION 3.19)
project(parallel_merge_sort)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "${CMAKE_CFLAGS} -fopenmp")
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG -g")
# https://stackoverflow.com/questions/53795731/undefined-reference-to-pthread-create-clion
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)


set(COMMON_LIBS common/data_generation.cpp common/data_generation.h common/merge_implementations.cpp common/merge_implementations.h)

add_executable(serial_merge_sort serial_impl/serial.cpp serial_impl/serial.h ${COMMON_LIBS})
add_executable(pthread_merge_sort pthread_impl/pthread_mergesort.cpp pthread_impl/pthread_mergesort.h ${COMMON_LIBS})