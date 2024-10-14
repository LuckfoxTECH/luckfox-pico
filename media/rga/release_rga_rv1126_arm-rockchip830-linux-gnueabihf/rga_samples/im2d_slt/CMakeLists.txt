cmake_minimum_required(VERSION 3.5)
enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT CMAKE_BUILD_TYPE)
    message("default to Release build for GCC builds")
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

##########################
# Command
#execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/version.sh)
#execute_process(COMMAND ls)

##########################
# Choose compilation method
set(TOOL_CHAIN_DIR ${CMAKE_MODULE_PATH})

if(CMAKE_BUILD_TARGET STREQUAL android_ndk)
    message("set android_ndk toolchain")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error -DLOG_TAG=librga")
    add_compile_options(-DLINUX)
    add_compile_options(-DANDROID)
#    add_compile_options(-DANDROID_VNDK)
endif()

if(CMAKE_BUILD_TARGET STREQUAL buildroot)
    message("set buildroot toolchain")
    set(CMAKE_TOOLCHAIN_FILE
        ${TOOL_CHAIN_DIR}/buildroot.cmake)
    include(${TOOL_CHAIN_DIR}/buildroot.cmake)
    add_compile_options(-DLINUX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -pthread")
endif()

##########################
# Choose a third-party library
if(CMAKE_BUILD_THIRD_PARTY STREQUAL drm)
    message("set third party drm")
    add_compile_options(-DLIBDRM=1)
endif()
#add_compile_options(-DRGA3=1)

##########################
# Set project
project(im2d_rga_slt)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/sources
    ${CMAKE_CURRENT_SOURCE_DIR}/../../
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../im2d_api
)

if(CMAKE_BUILD_THIRD_PARTY STREQUAL drm)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libdrm/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libdrm/include/libdrm
)
endif()

set(IM2D_RGA_SLT_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/rga_im2d_slt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/dma_alloc.cpp
)
if(CMAKE_BUILD_THIRD_PARTY STREQUAL drm)
set(IM2D_RGA_SLT_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/drm_alloc.cpp
)
endif()

add_executable(im2d_slt ${IM2D_RGA_SLT_SRCS})

target_link_libraries(im2d_slt "-Wl,--allow-shlib-undefined"  dl)
target_link_libraries(im2d_slt ${CMAKE_CURRENT_SOURCE_DIR}/../../build_linux/install/lib/librga.so)

if(CMAKE_BUILD_THIRD_PARTY STREQUAL drm)
    target_link_libraries(im2d_slt drm)
endif()

if(CMAKE_BUILD_TARGET STREQUAL ndk)
    string(REPLACE "-DANDROID" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

install(TARGETS im2d_slt RUNTIME DESTINATION bin)
