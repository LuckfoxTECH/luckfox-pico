cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(rkadk_examples)

add_definitions(-Wall -g -O0 -ggdb -gdwarf -funwind-tables -rdynamic -Wno-unused-parameter -D_GNU_SOURCE)

aux_source_directory(common/mp3_header MP3_SRC)

if(${RKADK_CHIP} STREQUAL "rv1126")
	file(GLOB_RECURSE ISP_SRC "common/isp/sample_aiq_uapi1.c")
else()
	file(GLOB_RECURSE ISP_SRC "common/isp/sample_aiq_uapi2.c")
endif()

if(ENABLE_EIS)
	file(GLOB_RECURSE IIO_SRC "common/isp/sample_iio_aiq.c")
endif()

#--------------------------
# rkadk_photo_test
#--------------------------
add_executable(rkadk_photo_test rkadk_photo_test.c ${ISP_SRC})
add_dependencies(rkadk_photo_test rkadk)
target_link_libraries(rkadk_photo_test rkadk)

if(USE_RKAIQ)
	target_link_libraries(rkadk_photo_test rkaiq)
endif()

target_include_directories(rkadk_photo_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(rkadk_photo_test PRIVATE ${CMAKE_SOURCE_DIR}/examples/common)
install(TARGETS rkadk_photo_test DESTINATION "bin")

#--------------------------
# rkadk_record_test
#--------------------------
add_executable(rkadk_record_test rkadk_record_test.c ${ISP_SRC} ${IIO_SRC})
add_dependencies(rkadk_record_test rkadk)
target_link_libraries(rkadk_record_test rkadk)

if(USE_RKAIQ)
	target_link_libraries(rkadk_record_test rkaiq)
endif()

if(ENABLE_EIS)
        target_link_libraries(rkadk_record_test easyiio iio)
endif()

target_include_directories(rkadk_record_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(rkadk_record_test PRIVATE ${CMAKE_SOURCE_DIR}/examples/common)
install(TARGETS rkadk_record_test DESTINATION "bin")

if(ENABLE_AOV)
#--------------------------
# rkadk_aov_record_test
#--------------------------
add_executable(rkadk_aov_record_test rkadk_aov_record_test.c ${ISP_SRC})
add_dependencies(rkadk_aov_record_test rkadk)
target_link_libraries(rkadk_aov_record_test rkadk)

if(USE_RKAIQ)
        target_link_libraries(rkadk_aov_record_test rkaiq)
endif()

target_include_directories(rkadk_aov_record_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(rkadk_aov_record_test PRIVATE ${CMAKE_SOURCE_DIR}/examples/common)
install(TARGETS rkadk_aov_record_test DESTINATION "bin")
endif()

#--------------------------
# rkadk_rtsp_test
#--------------------------
add_executable(rkadk_rtsp_test rkadk_rtsp_test.c ${ISP_SRC} ${IIO_SRC})
add_dependencies(rkadk_rtsp_test rkadk)
target_link_libraries(rkadk_rtsp_test rkadk)

if(USE_RKAIQ)
	target_link_libraries(rkadk_rtsp_test rkaiq)
endif()

if(ENABLE_EIS)
        target_link_libraries(rkadk_rtsp_test easyiio iio)
endif()

target_include_directories(rkadk_rtsp_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(rkadk_rtsp_test PRIVATE ${CMAKE_SOURCE_DIR}/examples/common)
install(TARGETS rkadk_rtsp_test DESTINATION "bin")

#--------------------------
# rkadk_player_test
#--------------------------
add_executable(rkadk_player_test rkadk_player_test.c)
add_dependencies(rkadk_player_test rkadk)
target_link_libraries(rkadk_player_test rkadk pthread)
target_include_directories(rkadk_player_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkadk_player_test DESTINATION "bin")

#-----------------------------------
# rkadk_setting_test
#-----------------------------------
add_executable(rkadk_setting_test rkadk_setting_test.c)
add_dependencies(rkadk_setting_test rkadk)
target_link_libraries(rkadk_setting_test rkadk)
target_include_directories(rkadk_setting_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkadk_setting_test DESTINATION "bin")

#--------------------------
# rkadk_stream_test
#--------------------------
add_executable(rkadk_stream_test rkadk_stream_test.c ${MP3_SRC} ${ISP_SRC})
add_dependencies(rkadk_stream_test rkadk)
target_link_libraries(rkadk_stream_test rkadk)

if(USE_RKAIQ)
	target_link_libraries(rkadk_stream_test rkaiq)
endif()

target_include_directories(rkadk_stream_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(rkadk_stream_test PRIVATE ${CMAKE_SOURCE_DIR}/examples/common)
install(TARGETS rkadk_stream_test DESTINATION "bin")

if(ENABLE_STORAGE)
#--------------------------
# rkadk_storage_test
#--------------------------
add_executable(rkadk_storage_test rkadk_storage_test.c)
add_dependencies(rkadk_storage_test rkadk)
target_link_libraries(rkadk_storage_test rkadk pthread)
target_include_directories(rkadk_storage_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkadk_storage_test DESTINATION "bin")
endif()

#--------------------------
# rkadk_thumb_test
#--------------------------
add_executable(rkadk_thumb_test rkadk_thumb_test.c)
add_dependencies(rkadk_thumb_test rkadk)
target_link_libraries(rkadk_thumb_test rkadk)
target_include_directories(rkadk_thumb_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS rkadk_thumb_test DESTINATION "bin")

#--------------------------
# rkadk_rtmp_test
#--------------------------
add_executable(rkadk_rtmp_test rkadk_rtmp_test.c ${ISP_SRC})
add_dependencies(rkadk_rtmp_test rkadk)
target_link_libraries(rkadk_rtmp_test rkadk)

if(USE_RKAIQ)
	target_link_libraries(rkadk_rtmp_test rkaiq)
endif()

target_include_directories(rkadk_rtmp_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(rkadk_rtmp_test PRIVATE ${CMAKE_SOURCE_DIR}/examples/common)
install(TARGETS rkadk_rtmp_test DESTINATION "bin")

#--------------------------
# rkadk_media_test
#--------------------------
add_executable(rkadk_media_test rkadk_media_test.c ${ISP_SRC})
add_dependencies(rkadk_media_test rkadk)
target_link_libraries(rkadk_media_test rkadk)

if(USE_RKAIQ)
	target_link_libraries(rkadk_media_test rkaiq)
endif()

target_include_directories(rkadk_media_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(rkadk_media_test PRIVATE ${CMAKE_SOURCE_DIR}/examples/common)
install(TARGETS rkadk_media_test DESTINATION "bin")

#--------------------------
# rkadk_disp_test
#--------------------------
add_executable(rkadk_disp_test rkadk_disp_test.c ${ISP_SRC})
add_dependencies(rkadk_disp_test rkadk)
target_link_libraries(rkadk_disp_test rkadk)

if(USE_RKAIQ)
	target_link_libraries(rkadk_disp_test rkaiq)
endif()

target_include_directories(rkadk_disp_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(rkadk_disp_test PRIVATE ${CMAKE_SOURCE_DIR}/examples/common)
install(TARGETS rkadk_disp_test DESTINATION "bin")

#--------------------------
# rkadk_ui_test
#--------------------------
add_executable(rkadk_ui_test rkadk_ui_test.c ${ISP_SRC})
add_dependencies(rkadk_ui_test rkadk)
target_link_libraries(rkadk_ui_test rkadk)

if(USE_RKAIQ)
	target_link_libraries(rkadk_ui_test rkaiq)
endif()

target_include_directories(rkadk_ui_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(rkadk_ui_test PRIVATE ${CMAKE_SOURCE_DIR}/examples/common)
install(TARGETS rkadk_ui_test DESTINATION "bin")
