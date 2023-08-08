cmake_minimum_required(VERSION 3.5)

include_directories(${PROJECT_SOURCE_DIR}/./include
					${CMAKE_SYSROOT}/usr/include/rkaiq/uAPI
					${CMAKE_SYSROOT}/usr/include/rkaiq/algos
					${CMAKE_SYSROOT}/usr/include/rkaiq/common
					${CMAKE_SYSROOT}/usr/include/rkaiq/xcore
					${CMAKE_SYSROOT}/usr/include/rkaiq/iq_parser
					${CMAKE_SYSROOT}/usr/include/libdrm
					${CMAKE_SYSROOT}/usr/include/freetype2
					${CMAKE_SYSROOT}/usr/include/easymedia
					${WPA_SUPPLICANT_INCLUDE_DIRS}
					)

aux_source_directory(./ SRCS)
aux_source_directory(audio SRCS)
aux_source_directory(video SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/isp/rv1126 SRCS)
aux_source_directory(server SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/ SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/socket_server SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/param SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/system SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/rtmp SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/osd SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/roi SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/osd/freetype2 SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/network SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/storage SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/ivm SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/region_clip SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/sysutil SRCS)

include_directories(audio
					${PROJECT_SOURCE_DIR}/common/isp/rv1126
					video
					server
					${PROJECT_SOURCE_DIR}/common
					${PROJECT_SOURCE_DIR}/common/socket_server
					${PROJECT_SOURCE_DIR}/common/rtsp
					${PROJECT_SOURCE_DIR}/common/rtmp
					${PROJECT_SOURCE_DIR}/common/param
					${PROJECT_SOURCE_DIR}/common/system
					${PROJECT_SOURCE_DIR}/common/osd
					${PROJECT_SOURCE_DIR}/common/osd/freetype2
					${PROJECT_SOURCE_DIR}/common/roi
					${PROJECT_SOURCE_DIR}/common/network
					${PROJECT_SOURCE_DIR}/common/storage
					${PROJECT_SOURCE_DIR}/common/ivm
					${PROJECT_SOURCE_DIR}/common/event
					${PROJECT_SOURCE_DIR}/common/region_clip
					${PROJECT_SOURCE_DIR}/common/sysutil
					./
					)

link_directories(${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-gnueabihf)

add_definitions(-DISP_HW_V20)
add_definitions(-g -ggdb)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} pthread easymedia third_media rkaiq rtsp rkmuxer freetype wpa_client m rga rknn_api drm png16 z)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES rkipc.ini DESTINATION share)
install(PROGRAMS RkLunch.sh DESTINATION bin )
install(PROGRAMS RkLunch-stop.sh DESTINATION bin )
install(FILES ${PROJECT_SOURCE_DIR}/common/osd/image.bmp DESTINATION share)
install(FILES ${PROJECT_SOURCE_DIR}/common/osd/simsun_cn.ttc DESTINATION share)
install(FILES ${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-gnueabihf/libivm_share.so DESTINATION lib)
install(FILES ${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-gnueabihf/libwpa_client.so DESTINATION lib)
install(FILES ${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-gnueabihf/librknn_api.so DESTINATION lib)
