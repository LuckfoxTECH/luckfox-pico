cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -gdwarf")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -gdwarf")

project(rockive)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
set (TARGET_LIB_ARCH lib64)
else()
set (TARGET_LIB_ARCH lib)
endif()

set(ROCKIVE_DIR ${PROJECT_SOURCE_DIR})

install(DIRECTORY ${ROCKIVE_DIR}/include/ DESTINATION include/rockive)
install(DIRECTORY ${ROCKIVE_DIR}/${TARGET_LIB_ARCH}/ DESTINATION lib)

# build sample test
set(SAMPLE_BIN_NAME rk_rve_sample_test)

set(SAMPLE_SRCS
	sample/main.c
	sample/sample_comm_rve.c
	sample/sample_rve_mem.c
	sample/sample_rve_hist.c
	sample/sample_rve_object_tracking.c
)

find_library(LIBRVE_LIBRARY rve ${ROCKIVE_DIR}/${TARGET_LIB_ARCH})

include_directories(
	sample
	include
)

find_library(LIBROCKIT_LIBRARY
    rockit
)

add_executable(${SAMPLE_BIN_NAME} ${SAMPLE_SRCS})
target_link_libraries(${SAMPLE_BIN_NAME} rockit ${LIBRVE_LIBRARY})
install(PROGRAMS ${PROJECT_BINARY_DIR}/${SAMPLE_BIN_NAME} DESTINATION bin)
