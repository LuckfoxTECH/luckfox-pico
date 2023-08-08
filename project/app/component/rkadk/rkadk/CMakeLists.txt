message(STATUS "cmake version ${CMAKE_VERSION}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(rkadk)
include(version/Version.cmake)

install(DIRECTORY include/ DESTINATION include/rkadk FILES_MATCHING PATTERN "*.h")
include_directories(include)

if(USE_RKAIQ)
    find_package(RkAiq REQUIRED)
    include_directories(${RKAIQ_INCLUDE_DIRS})
    add_definitions(-DRKAIQ)

    if(${RKADK_CHIP} STREQUAL "rv1126")
	add_definitions(-DRKAIQ_UAPI1 -DRV1126_1109 -DTHUMB_NORMAL)
    endif()
endif()

#-Wextra
add_definitions(-Wall
		-Wshadow
		-ffast-math
		-Wignored-qualifiers
		-Werror-implicit-function-declaration
		-fPIC
		-Wl,-Bsymbolic)

add_subdirectory(src)
add_subdirectory(examples)
