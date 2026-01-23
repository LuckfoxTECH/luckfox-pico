cmake_minimum_required(VERSION 3.10)
project(peer_client_armv7)

set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------
# Toolchain cho Luckfox ARMv7
# ---------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/luckfox-armv7.cmake)

# ---------------------------------------------------------
# Include + Lib directory của libdatachannel ARMv7
# ---------------------------------------------------------
include_directories($ENV{HOME}/armv7/libdatachannel/include)
link_directories(
    $ENV{HOME}/armv7/libdatachannel/lib
    $ENV{HOME}/armv7/openssl/lib
)

# ---------------------------------------------------------
# Source files
# ---------------------------------------------------------
set(SOURCES
    src/udp_signaling.cpp
    src/peer_client.cpp
)

# ---------------------------------------------------------
# Build executable
# ---------------------------------------------------------
add_executable(peer_client ${SOURCES})

# ---------------------------------------------------------
# Link libraries
# ---------------------------------------------------------
target_link_libraries(peer_client
    datachannel
    ssl
    crypto
)

