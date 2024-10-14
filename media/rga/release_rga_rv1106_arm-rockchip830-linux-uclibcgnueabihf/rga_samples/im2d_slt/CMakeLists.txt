cmake_minimum_required(VERSION 3.12)

if (EXISTS ${BUILD_TOOLCHAINS_PATH})
    message("load ${BUILD_TOOLCHAINS_PATH}")
    include(${BUILD_TOOLCHAINS_PATH})
endif()

project(im2d_slt)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--allow-shlib-undefined -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wl,--allow-shlib-undefined -pthread")

if (EXISTS ${LIBRGA_FILE_LIB}/librga.so)
	message("load ${LIBRGA_FILE_LIB}/librga.so")
    set(RGA_LIB ${LIBRGA_FILE_LIB}/librga.so)
else ()
    set(RGA_LIB rga)
endif()

if(NOT CMAKE_BUILD_TYPE)
    message("default to Release build for GCC builds")
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

#install path
if (NOT DEFINED CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR bin)
endif()

set(IM2D_SLT_NAME ${PROJECT_NAME})

set(RGA_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../im2d_api
)
set(IM2D_RGA_SLT_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/rga_im2d_slt.cpp
)

if (NOT DEFINED RGA_SAMPLES_UTILS_COMPILED)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../utils/CMakeLists.txt)
endif()

add_executable(${IM2D_SLT_NAME} ${IM2D_RGA_SLT_SRCS})
target_include_directories(${IM2D_SLT_NAME} PUBLIC ${RGA_INCLUDE})
target_link_libraries(${IM2D_SLT_NAME} ${RGA_LIB} utils_obj)

install(TARGETS ${IM2D_SLT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
