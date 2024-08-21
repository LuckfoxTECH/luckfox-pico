file(GLOB_RECURSE AUDIO_ENCODER_SRC "${CMAKE_SOURCE_DIR}/src/audio/encoder/*.c")
file(GLOB_RECURSE AUDIO_DECODER_SRC "${CMAKE_SOURCE_DIR}/src/audio/decoder/*.c")
file(GLOB_RECURSE COMMON_SRC "${CMAKE_SOURCE_DIR}/src/common/*.c")
file(GLOB_RECURSE DEMUXER_SRC "${CMAKE_SOURCE_DIR}/src/demuxer/*.c")
file(GLOB_RECURSE MUXER_SRC "${CMAKE_SOURCE_DIR}/src/muxer/*.c")
file(GLOB_RECURSE RECORD_SRC "${CMAKE_SOURCE_DIR}/src/record/*.c")
file(GLOB_RECURSE PHOTO_SRC "${CMAKE_SOURCE_DIR}/src/photo/*.c")
file(GLOB_RECURSE STREAM_SRC "${CMAKE_SOURCE_DIR}/src/stream/*.c")
file(GLOB_RECURSE PLAYER_SRC "${CMAKE_SOURCE_DIR}/src/player/*.c")
file(GLOB_RECURSE PARAM_SRC "${CMAKE_SOURCE_DIR}/src/param/*.c")
file(GLOB_RECURSE RTSP_SRC "${CMAKE_SOURCE_DIR}/src/live/rtsp/*.c")
file(GLOB_RECURSE OSD_SRC "${CMAKE_SOURCE_DIR}/src/osd/*.c")
file(GLOB_RECURSE RTMP_SRC "${CMAKE_SOURCE_DIR}/src/live/rtmp/*.c")
file(GLOB_RECURSE DISP_SRC "${CMAKE_SOURCE_DIR}/src/display/*.c")
file(GLOB_RECURSE UI_SRC "${CMAKE_SOURCE_DIR}/src/ui/*.c")
file(GLOB_RECURSE INIPARSER_SRC "${CMAKE_SOURCE_DIR}/src/third-party/iniparser/*.c")
file(GLOB_RECURSE CJSON_SRC "${CMAKE_SOURCE_DIR}/src/third-party/cjson/*.c")

if(ENABLE_STORAGE)
	file(GLOB_RECURSE STORAGE_SRC "${CMAKE_SOURCE_DIR}/src/storage/*.c")
endif()

if(ENABLE_AOV)
	file(GLOB_RECURSE AOV_SRC "${CMAKE_SOURCE_DIR}/src/aov/*.c")
endif()

add_definitions(-g -O0 -ggdb -gdwarf -funwind-tables -rdynamic -D_GNU_SOURCE)

add_library(rkadk SHARED
		${AUDIO_ENCODER_SRC}
		${AUDIO_DECODER_SRC}
		${COMMON_SRC}
		${DEMUXER_SRC}
		${MUXER_SRC}
		${RECORD_SRC}
		${PHOTO_SRC}
		${STREAM_SRC}
		${PLAYER_SRC}
		${PARAM_SRC}
		${RTMP_SRC}
		${RTSP_SRC}
		${OSD_SRC}
		${DISP_SRC}
		${UI_SRC}
		${STORAGE_SRC}
		${INIPARSER_SRC}
		${CJSON_SRC}
		${AOV_SRC})

set_target_properties(rkadk PROPERTIES LINK_FLAGS -Wl,--version-script,${CMAKE_CURRENT_LIST_DIR}/export.map)
#set_target_properties(rkadk PROPERTIES COMPILE_OPTIONS -Wl,-Bsymbolic)
target_include_directories(rkadk PUBLIC "${CMAKE_SOURCE_DIR}/src/record")
target_include_directories(rkadk PUBLIC "${CMAKE_SOURCE_DIR}/src/demuxer")
target_include_directories(rkadk PUBLIC "${CMAKE_SOURCE_DIR}/src/third-party")
target_include_directories(rkadk PUBLIC "${CMAKE_SOURCE_DIR}/src/common")

target_link_libraries(rkadk rkmuxer rtsp rkaudio rkdemuxer)

if(${RKADK_CHIP} STREQUAL "rv1106")
	target_link_libraries(rkadk rockit_full)
else()
	target_link_libraries(rkadk rockit)
endif()

if(ENABLE_JPEG_SLICE)
	add_definitions(-DJPEG_SLICE)
	target_link_libraries(rkadk rkyuvscaler)
endif()

if(ENABLE_STORAGE)
	target_link_libraries(rkadk iconv rkfsmk)
endif()

# install target
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
