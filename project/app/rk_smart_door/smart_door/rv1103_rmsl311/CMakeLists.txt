cmake_minimum_required(VERSION 3.5)

option(SC035_VGA_SENSOR "option for vga sensor" ON)

include_directories(${PROJECT_SOURCE_DIR}/common/uart)
include_directories(${PROJECT_SOURCE_DIR}/common/face/algo)
include_directories(${PROJECT_SOURCE_DIR}/common/face/face)
include_directories(${PROJECT_SOURCE_DIR}/common/face/interface)
include_directories(${PROJECT_SOURCE_DIR}/common/face/luma)
include_directories(${PROJECT_SOURCE_DIR}/common)

aux_source_directory(./ SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/uart SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/face/algo SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/face/face SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/face/interface SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/face/luma SOURCE)
aux_source_directory(${PROJECT_SOURCE_DIR}/common/mediactl SOURCE)

ADD_EXECUTABLE(smart_door ${SOURCE})

set(smart_unlock ${CMAKE_SOURCE_DIR}/common/face/algo/lib/libsmart_unlock.a)
set(chrombase ${CMAKE_SOURCE_DIR}/common/face/algo/lib/libchromiumbase64.a)

if (SC035_VGA_SENSOR)
set(rkdepth ${CMAKE_SOURCE_DIR}/common/face/algo/lib/librkdepth_vga.a)
else ()
set(rkdepth ${CMAKE_SOURCE_DIR}/common/face/algo/lib/librkdepth.a)
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

target_link_libraries(${PROJECT_NAME} libpthread.a
    libstdc++.a
    librockit.a
    librkaiq.a
    libsmartIr.a
    ${smart_unlock}
    ${rknn}
    ${chrombase}
    ${rkdepth}
    librga.a
    ${OPENCV_LIBS})

if (RK_BUILD_APP_TO_OEM_PARTITION STREQUAL y)
	SET(INSTALL_PATH bin)
else()
	SET(INSTALL_PATH root/usr/bin)
endif()

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
else ()
add_definitions(-DSC035_VGA_SENSOR=0)
install(FILES ${PROJECT_SOURCE_DIR}/common/face/algo/bin/sc132/calib_data.bin DESTINATION userdata/lib)
endif()
install(FILES ${PROJECT_SOURCE_DIR}/common/face/luma/bin/isp_param.bin DESTINATION userdata/lib)
