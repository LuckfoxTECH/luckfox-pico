cmake_minimum_required(VERSION 3.5)

option(VIDEO_CHN0_WORKING "option for the status of video channel_0" OFF)
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
include_directories(${PROJECT_SOURCE_DIR}/common/face/algo_dual_ir/include)

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

ADD_EXECUTABLE(smart_door ${SOURCE})

set(smart_unlock ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/libsmart_unlock.a)
set(rknn ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/librknnmrt.a)
set(chrombase ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/libchromiumbase64.a)
set(dual_stereo ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/libdual_stereo.a)

set(opencv
    ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/opencv-linux-armhf/lib/libopencv_calib3d.a
    ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/opencv-linux-armhf/lib/libopencv_features2d.a
    ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/opencv-linux-armhf/lib/libopencv_flann.a
    ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/opencv-linux-armhf/lib/libopencv_imgproc.a
    ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/opencv-linux-armhf/lib/libopencv_core.a
    ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/opencv-linux-armhf/lib2/libittnotify.a
    ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/opencv-linux-armhf/lib2/libtegra_hal.a
    ${CMAKE_SOURCE_DIR}/common/face/algo_dual_ir/lib/opencv-linux-armhf/lib2/libzlib.a)

target_link_libraries(${PROJECT_NAME} libpthread.a librt.a librockit.a librockchip_mpp.a
    librtsp.a
    ${smart_unlock}
    ${rknn}
    ${chrombase}
    ${dual_stereo}
    ${opencv}
    librga.a
    -lrkaiq
    -lstdc++)

if (RK_BUILD_APP_TO_OEM_PARTITION STREQUAL y)
	SET(INSTALL_PATH bin)
else()
	SET(INSTALL_PATH root/usr/bin)
endif()

install(FILES ${PROJECT_SOURCE_DIR}/common/uvc/uvc_mpi_cfg.conf DESTINATION root/usr/share)
install(FILES ${PROJECT_SOURCE_DIR}/common/uvc/rkuvc.ini DESTINATION root/usr/share)
install(FILES ${PROJECT_SOURCE_DIR}/common/uvc/usb_config.sh DESTINATION root/usr/bin)
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_PATH})

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/share/models
	COMMAND cp -r ${PROJECT_SOURCE_DIR}/common/face/algo_dual_ir/models/* ${CMAKE_INSTALL_PREFIX}/share/models/
)
install(FILES ${PROJECT_SOURCE_DIR}/common/face/algo_dual_ir/bin/gc1084/face_mask.bin DESTINATION userdata/lib)
install(FILES ${PROJECT_SOURCE_DIR}/common/face/algo_dual_ir/bin/gc1084/face.cfg DESTINATION userdata/lib)
install(FILES ${PROJECT_SOURCE_DIR}/common/face/algo_dual_ir/bin/gc1084/calib.yml DESTINATION userdata/lib)
