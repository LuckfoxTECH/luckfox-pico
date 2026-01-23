cmake_minimum_required(VERSION 3.16)

# Your project name
project(WebRTCDataChannelExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Manual include + lib paths
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Executable name
set(TARGET_NAME webrtc_module)

add_executable(${TARGET_NAME}
    src/main.cpp
)

# Link libs used by libdatachannel
target_link_libraries(${TARGET_NAME}
    datachannel
    sctp
    ssl
    crypto
    pthread
)
