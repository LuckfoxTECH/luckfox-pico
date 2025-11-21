cmake_minimum_required(VERSION 3.10)
project(webrtc_demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include & lib của libdatachannel
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Module .so
add_library(webrtc_module SHARED
    src/module.cpp
)
target_link_libraries(webrtc_module
    datachannel
    ssl
    crypto
    sctp
    pthread
)

# Main app
add_executable(webrtc_app
    src/main.cpp
)
target_link_libraries(webrtc_app
    dl
)
