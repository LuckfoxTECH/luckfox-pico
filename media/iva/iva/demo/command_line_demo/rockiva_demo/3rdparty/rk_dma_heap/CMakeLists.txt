cmake_minimum_required(VERSION 3.10)

project(rk_dma_heap)

add_library(rk_dma_heap STATIC rk_dma_heap.cpp)

target_include_directories(rk_dma_heap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})