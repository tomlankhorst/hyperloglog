cmake_minimum_required(VERSION 3.10)
project(HyperLogLog)

set(CMAKE_CXX_STANDARD 17)

add_library(murmur MurmurHash3.cpp)

add_executable(hll hll.cpp)

target_link_libraries(hll murmur)