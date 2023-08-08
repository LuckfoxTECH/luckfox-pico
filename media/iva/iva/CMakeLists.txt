cmake_minimum_required(VERSION 3.5.1)

project(rockiva)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
set (TARGET_LIB_ARCH lib64)
else()
set (TARGET_LIB_ARCH lib)
endif()

set(ROCKIVA_LIB_DIR librockiva/rockiva-${TARGET_SOC}-Linux)
set(ROCKIVA_DATA_DIR models/rockiva_data_${TARGET_SOC})

install(DIRECTORY ${ROCKIVA_LIB_DIR}/include/ DESTINATION include/rockiva)
install(DIRECTORY ${ROCKIVA_LIB_DIR}/${TARGET_LIB_ARCH}/ DESTINATION lib)

install(DIRECTORY ${ROCKIVA_DATA_DIR}/ DESTINATION lib)
