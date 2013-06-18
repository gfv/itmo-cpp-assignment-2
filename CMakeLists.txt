cmake_minimum_required (VERSION 2.6)
project (Huffman)

set (Huffman_VERSION_MAJOR 1)
set (Huffman_VERSION_MINOR 0)
set (CMAKE_CXX_FLAGS "-std=c++0x -g")

add_library(hcompress SHARED compress.cpp)
add_executable(huffman main.cpp)

target_link_libraries(huffman hcompress)

