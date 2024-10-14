cmake_minimum_required(VERSION 3.12)

if (EXISTS ${BUILD_TOOLCHAINS_PATH})
    message("load ${BUILD_TOOLCHAINS_PATH}")
    include(${BUILD_TOOLCHAINS_PATH})
endif()

if (EXISTS ${LIBRGA_FILE_LIB}/librga.so)
	message("load ${LIBRGA_FILE_LIB}/librga.so")
    set(RGA_LIB ${LIBRGA_FILE_LIB}/librga.so)
else ()
    set(RGA_LIB rga)
endif()

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(rga_${TARGET_NAME})

#install path
if (NOT DEFINED CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR bin)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--allow-shlib-undefined -ldl")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wl,--allow-shlib-undefined -ldl")

set(RGA_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../im2d_api)
include_directories(${RGA_INCLUDE})

if (NOT DEFINED RGA_SAMPLES_UTILS_COMPILED)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../utils/CMakeLists.txt)
endif()

string(REPLACE "-DANDROID" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_subdirectory(src)

