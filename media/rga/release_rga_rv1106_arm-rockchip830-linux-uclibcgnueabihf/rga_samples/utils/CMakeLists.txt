cmake_minimum_required(VERSION 3.12)

add_library(utils_obj OBJECT "")

SET(THIRD_PARTY_PATH ${CMAKE_CURRENT_LIST_DIR}/3rdparty)
SET(UTILS_LIBRGA_SOURCE_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)

include(${CMAKE_CURRENT_LIST_DIR}/allocator/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/3rdparty/CMakeLists.txt)

target_sources(utils_obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/utils.h
)

target_include_directories(utils_obj
    PRIVATE
        ${UTILS_LIBRGA_SOURCE_PATH}/include
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

SET(RGA_SAMPLES_UTILS_COMPILED "true" CACHE STRING "")
