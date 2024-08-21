cmake_minimum_required(VERSION 3.5)

option(VIDEO_CHN0_WORKING "option for the status of vi channel_0" ON)
option(UVC_EPTZ "option for uvc eptz func" OFF)
option(UVC_OSD "option for uvc osd func" OFF)
option(UVC_PU "option for uvc pu control func" OFF)

option(SC035_VGA_SENSOR "option for vga sensor" ON)
#option(SC132_SENSOR "option for sc132 sensor" ON)
#option(SC2355_SENSOR "option for vga sensor" ON)

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
include_directories(${PROJECT_SOURCE_DIR}/common/rkaiq_3A)
include_directories(${PROJECT_SOURCE_DIR}/common/smart_ir)
include_directories(${PROJECT_SOURCE_DIR}/common/uart)
include_directories(${PROJECT_SOURCE_DIR}/common/face/algo)
include_directories(${PROJECT_SOURCE_DIR}/common/face/face)
include_directories(${PROJECT_SOURCE_DIR}/common/face/interface)
include_directories(${PROJECT_SOURCE_DIR}/rv1106_rmsl/wifi)
include_directories(${PROJECT_SOURCE_DIR}/common/face/luma)

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
aux_source_directory(${PROJECT_SOURCE_DIR}/common/uart SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/face/algo SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/face/face SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/face/interface SOURCE)
aux_source_directory(./wifi SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/face/luma SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/rockit SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/uvc SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/uvc/cJSON SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/uvc/param SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/isp SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/mediactl SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/rkaiq_3A SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/smart_ir SOURCE)

ADD_EXECUTABLE(smart_door ${SOURCE})

set(smart_unlock ${CMAKE_SOURCE_DIR}/common/face/algo/lib/libsmart_unlock.a)
set(chrombase ${CMAKE_SOURCE_DIR}/common/face/algo/lib/libchromiumbase64.a)

if (SC035_VGA_SENSOR)
	message("sc035 use librkdepth_vga.a")
	set(rkdepth ${CMAKE_SOURCE_DIR}/common/face/algo/lib/librkdepth_vga.a)
elseif (SC132_SENSOR)
	message("sc132 use librkdepth.a")
	set(rkdepth ${CMAKE_SOURCE_DIR}/common/face/algo/lib/librkdepth_sc132.a)
elseif (SC2355_SENSOR)
	message("sc2355 use librkdepth_sc2355.a")
	set(rkdepth ${CMAKE_SOURCE_DIR}/common/face/algo/lib/librkdepth_sc2355.a)
else ()
	message("use librkdepth.a")
	set(rkdepth ${CMAKE_SOURCE_DIR}/common/face/algo/lib/librkdepth.a)
	message("error sensor type")
endif()

set(rknn ${CMAKE_SOURCE_DIR}/common/face/algo/lib/librknnmrt.a)
set(OPENCV_PATH ${CMAKE_SOURCE_DIR}/common/face/algo/lib/opencv-linux-armhf1106)
set(OPENCV_LIBS
	${OPENCV_PATH}/lib/libopencv_calib3d.a
	${OPENCV_PATH}/lib/libopencv_features2d.a
	${OPENCV_PATH}/lib/libopencv_flann.a
	${OPENCV_PATH}/lib/libopencv_imgproc.a
	${OPENCV_PATH}/lib/libopencv_core.a

	${OPENCV_PATH}/lib2/libittnotify.a
	${OPENCV_PATH}/lib2/libtegra_hal.a
	${OPENCV_PATH}/lib2/libzlib.a
	)

target_link_libraries(${PROJECT_NAME} libpthread.a librt.a librockit.a librockchip_mpp.a
    librtsp.a librkaudio_detect.a libaec_bf_process.a libm.a librockiva.a
    libsmartIr.a
    ${smart_unlock}
    ${rknn}
    ${chrombase}
    ${rkdepth}
    librga.a
    ${OPENCV_LIBS}
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

#libs for algo, compress face rknn models by gzip
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/share
	COMMAND cp -r ${PROJECT_SOURCE_DIR}/common/face/algo/models ${CMAKE_INSTALL_PREFIX}/share/
	COMMAND gzip -9 -n -f ${CMAKE_INSTALL_PREFIX}/share/models/*.rknn
)

if (SC035_VGA_SENSOR)
	add_definitions(-DSC035_VGA_SENSOR=1)
	install(FILES ${PROJECT_SOURCE_DIR}/common/face/algo/bin/sc035/calib_data.bin DESTINATION userdata/lib)
	message("use sc035 calib")
elseif (SC132_SENSOR)
	add_definitions(-DSC132GS_SENSOR=1)
	install(FILES ${PROJECT_SOURCE_DIR}/common/face/algo/bin/sc132/calib_data.bin DESTINATION userdata/lib)
	message("use sc132 calib")
elseif (SC2355_SENSOR)
	add_definitions(-DSC2355_SENSOR=1)
	install(FILES ${PROJECT_SOURCE_DIR}/common/face/algo/bin/sc2355/calib_data.bin DESTINATION userdata/lib)
	message("use sc2355 calib")
endif()
install(FILES ${PROJECT_SOURCE_DIR}/common/face/luma/bin/isp_param.bin DESTINATION userdata/lib)

# install configs for audio
install(FILES ${PROJECT_SOURCE_DIR}/rv1106_rmsl/configs/configs_skv.json DESTINATION userdata)
