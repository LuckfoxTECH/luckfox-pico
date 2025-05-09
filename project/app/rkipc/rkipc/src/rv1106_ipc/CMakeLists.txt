cmake_minimum_required(VERSION 3.5)

# Add RockIva include directories
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
                    # Add RockIva include paths
                    ${PROJECT_SOURCE_DIR}/media/iva/iva/librockiva/rockiva-rv1106-Linux/include
                    /home/luckfox/Downloads/rv1106_ipc_custom/media/iva/iva/librockiva/rockiva-rv1106-Linux/include
                    )

aux_source_directory(./ SRCS)
aux_source_directory(video SRCS)
aux_source_directory(server SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/ SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/audio/rv1106 SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/isp/rv1106 SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/socket_server SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/param SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/system SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/rtsp SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/rtmp SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/osd SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/roi SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/network SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/storage SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/rockiva SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/event SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/region_clip SRCS)

include_directories(video
                    server
                    ${PROJECT_SOURCE_DIR}/common
                    ${PROJECT_SOURCE_DIR}/common/audio/rv1106
                    ${PROJECT_SOURCE_DIR}/common/isp/rv1106
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
                    ${PROJECT_SOURCE_DIR}/common/rockiva
                    ${PROJECT_SOURCE_DIR}/common/event
                    ${PROJECT_SOURCE_DIR}/common/region_clip
                    )

# Add RockIva library directory
link_directories(${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-uclibcgnueabihf
                 ${PROJECT_SOURCE_DIR}/media/iva/iva/librockiva/rockiva-rv1106-Linux/lib
                 /home/luckfox/Downloads/rv1106_ipc_custom/media/iva/iva/librockiva/rockiva-rv1106-Linux/lib
                 )

add_definitions(-DISP_HW_V32)
#add_definitions(-g -ggdb)
# add_definitions(-Os)
# add_definitions(-fsanitize=address -g -ggdb -gdwarf -funwind-tables -rdynamic -O0)
# add_definitions(-fno-stack-protector -fno-omit-frame-pointer -fsanitize-recover=address)
# link_libraries(asan)

add_executable(${PROJECT_NAME} ${SRCS})

# Link with rockiva library
target_link_libraries(${PROJECT_NAME} pthread rockit rockchip_mpp rkaiq rtsp rkaudio_detect aec_bf_process wpa_client m rkmuxer freetype rockiva iconv rknnmrt rga stdc++ rksysutils rkaudio)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES rkipc-300w.ini DESTINATION share)
install(FILES rkipc-400w.ini DESTINATION share)
install(FILES rkipc-500w.ini DESTINATION share)
install(FILES rkipc-mis5001-500w.ini DESTINATION share)
install(FILES rkipc-800w.ini DESTINATION share)
install(FILES ../../common/speaker_test.wav DESTINATION share)
install(PROGRAMS RkLunch.sh DESTINATION bin)
install(PROGRAMS RkLunch-stop.sh DESTINATION bin)
install(FILES resource/swatah_logo.bmp DESTINATION share)
install(FILES ${PROJECT_SOURCE_DIR}/common/osd/simsun_en.ttf DESTINATION share)
install(FILES ${PROJECT_SOURCE_DIR}/lib/arm-rockchip830-linux-uclibcgnueabihf/libwpa_client.so DESTINATION lib)