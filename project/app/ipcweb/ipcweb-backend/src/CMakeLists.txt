#
# Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#

add_library(restapi
    STATIC
    http_response.cpp
    rest_api.cpp
    common.cpp
    network_api.cpp
    storage_api.cpp
    video_api.cpp
    audio_api.cpp
    stream_api.cpp
    image_api.cpp
    system_api.cpp
    osd_api.cpp
    roi_api.cpp
    event_api.cpp
    peripherals_api.cpp
    socket_client/socket.cpp
    socket_client/client.cpp
)

target_compile_features(restapi PRIVATE cxx_std_14)
target_include_directories(restapi
    PRIVATE
    ${JWTCPP_INCLUDE_DIR}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

add_executable(entry.cgi
    main.cpp)

target_compile_features(entry.cgi PRIVATE cxx_std_14)
if (${USE_RKIPC})
    add_definitions(-DUSE_RKIPC)
    target_link_libraries(entry.cgi PRIVATE restapi pthread cgicc)
else()
    target_link_libraries(restapi
        PRIVATE OpenSSL::Crypto Cgicc::Cgicc pthread
        PUBLIC MiniLogger::MiniLogger nlohmann_json::nlohmann_json)
    target_link_libraries(entry.cgi PRIVATE restapi rkdb gdbus IPCProtocol)
endif()

option(IPCWEBBACKEND_INSTALL_ON_OEM_PARTITION "compile: install on oem partition" OFF)
if(IPCWEBBACKEND_INSTALL_ON_OEM_PARTITION)
install(TARGETS entry.cgi RUNTIME DESTINATION /www/cgi-bin)
else()
install(TARGETS entry.cgi RUNTIME DESTINATION www/cgi-bin)
endif()

if (${USE_RKIPC})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/www-rkipc/ DESTINATION www)
endif()