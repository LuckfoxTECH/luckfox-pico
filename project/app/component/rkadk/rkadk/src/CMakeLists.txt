file(GLOB_RECURSE AUDIO_ENCODER_SRC "${CMAKE_SOURCE_DIR}/src/audio/encoder/*.cpp")
file(GLOB_RECURSE AUDIO_DECODER_SRC "${CMAKE_SOURCE_DIR}/src/audio/decoder/*.cpp")
file(GLOB_RECURSE COMMON_SRC "${CMAKE_SOURCE_DIR}/src/common/*.cpp")
file(GLOB_RECURSE DEMUXER_SRC "${CMAKE_SOURCE_DIR}/src/demuxer/*.cpp")
file(GLOB_RECURSE MUXER_SRC "${CMAKE_SOURCE_DIR}/src/muxer/*.cpp")
file(GLOB_RECURSE RECORD_SRC "${CMAKE_SOURCE_DIR}/src/record/*.cpp")
file(GLOB_RECURSE PHOTO_SRC "${CMAKE_SOURCE_DIR}/src/photo/*.cpp")
file(GLOB_RECURSE STREAM_SRC "${CMAKE_SOURCE_DIR}/src/stream/*.cpp")
file(GLOB_RECURSE PLAYER_SRC "${CMAKE_SOURCE_DIR}/src/player/*.cpp")
file(GLOB_RECURSE PARAM_SRC "${CMAKE_SOURCE_DIR}/src/param/*.cpp")
file(GLOB_RECURSE RTSP_SRC "${CMAKE_SOURCE_DIR}/src/live/rtsp/*.cpp")
file(GLOB_RECURSE OSD_SRC "${CMAKE_SOURCE_DIR}/src/osd/*.cpp")
file(GLOB_RECURSE RTMP_SRC "${CMAKE_SOURCE_DIR}/src/live/rtmp/*.cpp")
#file(GLOB_RECURSE DISP_SRC "${CMAKE_SOURCE_DIR}/src/display/*.cpp")
file(GLOB_RECURSE STORAGE_SRC "${CMAKE_SOURCE_DIR}/src/storage/*.cpp")
file(GLOB_RECURSE INIPARSER_SRC "${CMAKE_SOURCE_DIR}/src/third-party/iniparser/*.c")
file(GLOB_RECURSE CJSON_SRC "${CMAKE_SOURCE_DIR}/src/third-party/cjson/*.c")
add_definitions(-g -O0 -ggdb -gdwarf -funwind-tables -rdynamic -D_GNU_SOURCE)

link_directories(${CMAKE_SOURCE_DIR}/lib/arm-rockchip830-linux-gnueabihf)

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
		${STORAGE_SRC}
		${INIPARSER_SRC}
		${CJSON_SRC})

set_target_properties(rkadk PROPERTIES LINK_FLAGS -Wl,--version-script,${CMAKE_CURRENT_LIST_DIR}/export.map)
#set_target_properties(rkadk PROPERTIES COMPILE_OPTIONS -Wl,-Bsymbolic)
target_include_directories(rkadk PUBLIC "${CMAKE_SOURCE_DIR}/src/record")
target_include_directories(rkadk PUBLIC "${CMAKE_SOURCE_DIR}/src/demuxer")
target_include_directories(rkadk PUBLIC "${CMAKE_SOURCE_DIR}/src/third-party")
target_include_directories(rkadk PUBLIC "${CMAKE_SOURCE_DIR}/src/common")

if(CMAKE_C_COMPILER MATCHES "arm-rockchip830-linux-uclibcgnueabihf*")
	target_link_libraries(rkadk rockit rkmuxer iconv rtsp z rkaudio rkfsmk rkdemuxer)
else()
	target_link_libraries(rkadk rockit rtsp rkfsmk rkmuxer z rkdemuxer)
endif()

if(USE_RKAIQ)
    target_link_libraries(rkadk rkaiq)
endif()

# install target
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
