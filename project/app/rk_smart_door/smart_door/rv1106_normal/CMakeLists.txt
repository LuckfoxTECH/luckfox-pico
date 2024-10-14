cmake_minimum_required(VERSION 3.5)

option(VIDEO_CHN0_WORKING "option for the status of vi channel_0" ON)
option(UVC_EPTZ "option for uvc eptz func" OFF)
option(UVC_OSD "option for uvc osd func" OFF)
option(UVC_PU "option for uvc pu control func" OFF)

if(VIDEO_CHN0_WORKING)
	add_definitions(-DVIDEO_CHN0_WORKING=1)
	message("VIDEO_CHN0_WORKING is enable")
else()
	add_definitions(-DVIDEO_CHN0_WORKING=0)
	message("VIDEO_CHN0_WORKING is disable")
endif()

if(UVC_EPTZ)
	add_definitions(-DUVC_EPTZ=1)
	message("UVC_EPTZ is enable")
else()
	add_definitions(-DUVC_EPTZ=0)
	message("UVC_EPTZ is disable")
endif()

if(UVC_OSD)
	add_definitions(-DUVC_OSD=1)
	message("UVC_OSD is enable")
else()
	add_definitions(-DUVC_OSD=0)
	message("UVC_OSD is disable")
endif()

if(UVC_PU)
	add_definitions(-DUVC_PU=1)
	message("UVC_PU is enable")
else()
	add_definitions(-DUVC_PU=0)
	message("UVC_PU is disable")
endif()

include_directories(${PROJECT_SOURCE_DIR}/common)
include_directories(${PROJECT_SOURCE_DIR}/common/rockit)
include_directories(${PROJECT_SOURCE_DIR}/common/uvc)
include_directories(${PROJECT_SOURCE_DIR}/common/uvc/cJSON)
include_directories(${PROJECT_SOURCE_DIR}/common/uvc/param)
include_directories(${PROJECT_SOURCE_DIR}/common/isp)
include_directories(${PROJECT_SOURCE_DIR}/common/mediactl)
include_directories(${PROJECT_SOURCE_DIR}/common/rkaiq_3A)
include_directories(${PROJECT_SOURCE_DIR}/common/smart_ir)

include_directories(${PROJECT_SOURCE_DIR}/include
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/uAPI
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/uAPI2
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/algos
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/common
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/xcore
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/iq_parser
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/iq_parser_v2
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/smartIr
                                        ${CMAKE_SYSROOT}/usr/include/libdrm
                                        )

aux_source_directory(./ SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/rockit SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/uvc SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/uvc/cJSON SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/uvc/param SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/isp SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/mediactl SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/rkaiq_3A SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/smart_ir SOURCE)

ADD_EXECUTABLE(smart_door ${SOURCE})

target_link_libraries(${PROJECT_NAME} libpthread.a librt.a librockit.a librockchip_mpp.a librkaiq.a
    librtsp.a librkaudio_detect.a libaec_bf_process.a libm.a librockiva.a libstdc++.a librknnmrt.a librga.a libsmartIr.a)

if (RK_BUILD_APP_TO_OEM_PARTITION STREQUAL y)
	SET(INSTALL_PATH bin)
else()
	SET(INSTALL_PATH root/usr/bin)
endif()

install(FILES ${PROJECT_SOURCE_DIR}/common/uvc/uvc_mpi_cfg.conf DESTINATION root/usr/share)
install(FILES ${PROJECT_SOURCE_DIR}/common/uvc/rkuvc.ini DESTINATION root/usr/share)
install(FILES ${PROJECT_SOURCE_DIR}/common/uvc/usb_config.sh DESTINATION root/usr/bin)
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_PATH})
