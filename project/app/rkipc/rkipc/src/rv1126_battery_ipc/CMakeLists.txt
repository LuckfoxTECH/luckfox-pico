cmake_minimum_required(VERSION 3.5)

# SET(CMAKE_C_COMPILER "${PROJECT_SOURCE_DIR}/${PROJECT_SOURCE_DIR}/${PROJECT_SOURCE_DIR}/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc")

include_directories(${PROJECT_SOURCE_DIR}/./include
					${CMAKE_SYSROOT}/usr/include/rkaiq/uAPI
					${CMAKE_SYSROOT}/usr/include/rkaiq/algos
					${CMAKE_SYSROOT}/usr/include/rkaiq/common
					${CMAKE_SYSROOT}/usr/include/rkaiq/xcore
					${CMAKE_SYSROOT}/usr/include/rkaiq/iq_parser
					${WPA_SUPPLICANT_INCLUDE_DIRS}
					)

aux_source_directory(./ SRCS)
aux_source_directory(audio SRCS)
aux_source_directory(video SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/isp/rv1126 SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/ SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/param SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/rtsp SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/tuya_ipc/4.1.1 SRCS)

include_directories(audio
					${PROJECT_SOURCE_DIR}/common/isp/rv1126
					video
					${PROJECT_SOURCE_DIR}/common
					${PROJECT_SOURCE_DIR}/common/rtsp
					${PROJECT_SOURCE_DIR}/common/param
					${PROJECT_SOURCE_DIR}/common/rkbar
					${PROJECT_SOURCE_DIR}/common/tuya_ipc/4.1.1/include
					${PROJECT_SOURCE_DIR}/common/tuya_ipc/4.1.1)

link_directories(${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-gnueabihf/)

add_definitions(-DISP_HW_V20)
add_definitions(-g -ggdb)

add_executable(${PROJECT_NAME} ${SRCS})

add_library(mbedtls STATIC IMPORTED)
add_library(tuya_ipc STATIC IMPORTED)
set_target_properties(mbedtls PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-gnueabihf/libmbedtls.a)
set_target_properties(tuya_ipc PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-gnueabihf/libtuya_ipc.a)
set(LIBHWL_WF_SRC ${PROJECT_SOURCE_DIR}/common/tuya_ipc/4.1.1/hwl_wf.c)
add_library(hwl_wf STATIC ${LIBHWL_WF_SRC})
target_link_libraries(${PROJECT_NAME} m pthread rockit rkaiq rtsp tuya_ipc mbedtls hwl_wf rkbar vendor_storage)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES rkipc.ini DESTINATION share)
