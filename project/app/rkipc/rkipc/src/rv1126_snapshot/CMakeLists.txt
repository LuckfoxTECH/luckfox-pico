cmake_minimum_required(VERSION 3.5)

# SET(CMAKE_C_COMPILER "${PROJECT_SOURCE_DIR}/${PROJECT_SOURCE_DIR}/${PROJECT_SOURCE_DIR}/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc")

include_directories(${PROJECT_SOURCE_DIR}/./include
					${CMAKE_SYSROOT}/usr/include/rkaiq/uAPI
					${CMAKE_SYSROOT}/usr/include/rkaiq/algos
					${CMAKE_SYSROOT}/usr/include/rkaiq/common
					${CMAKE_SYSROOT}/usr/include/rkaiq/xcore
					${CMAKE_SYSROOT}/usr/include/rkaiq/iq_parser
					${CMAKE_SYSROOT}/usr/include/easymedia)

aux_source_directory(./ SRCS)
aux_source_directory(video SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/isp/rv1126 SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/param SRCS)

include_directories(${PROJECT_SOURCE_DIR}/common
					${PROJECT_SOURCE_DIR}/common/isp/rv1126
					${PROJECT_SOURCE_DIR}/common/param
					video)

add_definitions(-DISP_HW_V20)
#add_definitions(-g -ggdb)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} pthread easymedia rkaiq)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(PROGRAMS RkLunch-stop.sh DESTINATION bin)
