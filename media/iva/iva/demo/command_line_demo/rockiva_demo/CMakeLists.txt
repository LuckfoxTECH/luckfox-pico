cmake_minimum_required(VERSION 3.10)

project(rockiva_demo)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "64bit")
	set (TARGET_LIB_ARCH lib64)
else()
	message(STATUS "32bit")
	set (TARGET_LIB_ARCH lib)
endif()

set(OUT_PATH ${PROJECT_NAME})

file(GLOB IMGS ${CMAKE_CURRENT_SOURCE_DIR}/*.jpg)

add_subdirectory(3rdparty/rk_dma_heap)

include_directories(3rdparty/stb_image)

set(JPEG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/jpeg_turbo)
if (CMAKE_C_COMPILER MATCHES "uclibc")
set(LIBJPEG ${JPEG_PATH}/${TARGET_LIB_ARCH}_uclibc/libturbojpeg.a)
else()
set(LIBJPEG ${JPEG_PATH}/${TARGET_LIB_ARCH}/libturbojpeg.a)
endif()
# set(LIBJPEG ${JPEG_PATH}/x64/libturbojpeg.a)
include_directories(${JPEG_PATH}/include)

set(UTILS_SRCS
        utils/common_utils.c
        utils/image_utils.c
        utils/image_buffer.c
)

set(COMMON_LIBS
        rk_dma_heap
        dl
        pthread
        ${LIBJPEG}
)

add_executable(rockiva_demo
        rockiva_demo.c 
        ${UTILS_SRCS}
)

target_link_libraries(rockiva_demo
        ${ROCKIVA_LIBS}
        ${COMMON_LIBS}
)

install(TARGETS rockiva_demo DESTINATION ${OUT_PATH})
install(FILES ${IMGS} DESTINATION ${OUT_PATH})
