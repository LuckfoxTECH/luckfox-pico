cmake_minimum_required(VERSION 3.5)

include_directories(${PROJECT_SOURCE_DIR}/./include
					${CMAKE_SYSROOT}/usr/include/rkaiq/
					${CMAKE_SYSROOT}/usr/include/rkaiq/uAPI
					${CMAKE_SYSROOT}/usr/include/rkaiq/uAPI2
					${CMAKE_SYSROOT}/usr/include/rkaiq/algos
					${CMAKE_SYSROOT}/usr/include/rkaiq/common
					${CMAKE_SYSROOT}/usr/include/rkaiq/xcore
					${CMAKE_SYSROOT}/usr/include/rkaiq/iq_parser
					${CMAKE_SYSROOT}/usr/include/rkaiq/iq_parser_v2
					${CMAKE_SYSROOT}/usr/include/libdrm
					${WPA_SUPPLICANT_INCLUDE_DIRS}
					)

aux_source_directory(./ SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/isp/rv1106 SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/param SRCS)

include_directories(audio
					${PROJECT_SOURCE_DIR}/common/isp/rv1106
					${PROJECT_SOURCE_DIR}/common
					${PROJECT_SOURCE_DIR}/common/param
					)

link_directories(${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-uclibcgnueabihf)

add_definitions(-DISP_HW_V32)
add_definitions(-g -ggdb)
#add_definitions(-Os)

# add_definitions(-fsanitize=address -g -ggdb -gdwarf -funwind-tables -rdynamic -O0)
# add_definitions(-fno-stack-protector -fno-omit-frame-pointer -fsanitize-recover=address)
# link_libraries(asan)

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} pthread rkaiq drm)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(PROGRAMS RkLunch.sh DESTINATION bin)
install(PROGRAMS modetest DESTINATION bin)
