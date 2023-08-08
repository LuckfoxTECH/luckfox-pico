cmake_minimum_required(VERSION 3.10)

project(rockiva_demo)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "64bit")
	set (TARGET_LIB_ARCH lib64)
else()
	message(STATUS "32bit")
	set (TARGET_LIB_ARCH lib)
endif()

# librockiva
set(LIBROCKIVA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../librockiva/rockiva-rv1106-Linux/)
include_directories(${LIBROCKIVA_PATH}/include)
set(ROCKIVA_LIBS
${LIBROCKIVA_PATH}/${TARGET_LIB_ARCH}/librockiva.so
${LIBROCKIVA_PATH}/${TARGET_LIB_ARCH}/librknnmrt.so
)

# install prefix
if (NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../install/rockiva_rv1106_linux)
endif()

# demos
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
add_subdirectory(rockiva_demo)

install(PROGRAMS ${ROCKIVA_LIBS} DESTINATION lib)
