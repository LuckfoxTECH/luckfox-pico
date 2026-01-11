cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../luckfox-armv7.cmake \
  -DCMAKE_INSTALL_PREFIX=$HOME/armv7/libdatachannel \
  -DBUILD_SHARED_LIBS=ON \
  -DOPENSSL_ROOT_DIR=$HOME/armv7/openssl
  
export PATH=~/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin:$PATH 
export CROSS_COMPILE=arm-rockchip830-linux-uclibcgnueabihf- 
export ARCH=arm 

cmake_minimum_required(VERSION 3.10)
project(peer_server_armv7 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# Toolchain cho Luckfox ARMv7
# ============================================================
include(${CMAKE_SOURCE_DIR}/luckfox-armv7.cmake)

# ============================================================
# Threads
# ============================================================
find_package(Threads REQUIRED)

# ============================================================
# libdatachannel (ARMv7)
# ============================================================
set(LIBDATACHANNEL_INCLUDE_DIRS
    $ENV{HOME}/armv7/libdatachannel/include
)

set(LIBDATACHANNEL_LIBRARY_DIRS
    $ENV{HOME}/armv7/libdatachannel/lib
    $ENV{HOME}/armv7/openssl/lib
)

set(LIBDATACHANNEL_LIBS
    datachannel
    ssl
    crypto
)

# ============================================================
# APP LAYER
# ============================================================
add_library(app STATIC
    src/app/ControlState.cpp
    src/app/EventRing.cpp
    src/app/IoctlController.cpp
    src/app/Time.cpp
)

target_include_directories(app
    PUBLIC
        src/app
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ============================================================
# COM LAYER
# ============================================================
add_library(com STATIC
    src/com/FrameCodec.cpp
    src/com/RxDispatcher.cpp
)

target_include_directories(com
    PUBLIC
        src/com
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(com
    PUBLIC
        app
)

# ============================================================
# SIGNALING LAYER
# ============================================================
add_library(signaling STATIC
    src/signaling/udp_signaling.cpp
    src/signaling/SignalingManager.cpp
    src/signaling/PeerServer.cpp
)

target_include_directories(signaling
    PUBLIC
        src/signaling
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${LIBDATACHANNEL_INCLUDE_DIRS}
)

target_link_directories(signaling
    PUBLIC
        ${LIBDATACHANNEL_LIBRARY_DIRS}
)

target_link_libraries(signaling
    PRIVATE
        com
        app
        Threads::Threads
)

# ============================================================
# TRANSPORT LAYER (WebRTC)
# ============================================================
add_library(transport STATIC
    src/transport/WebRTCTransport.cpp
)

target_include_directories(transport
    PUBLIC
        src/transport
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${LIBDATACHANNEL_INCLUDE_DIRS}
)

target_link_directories(transport
    PUBLIC
        ${LIBDATACHANNEL_LIBRARY_DIRS}
)

target_link_libraries(transport
    PRIVATE
        ${LIBDATACHANNEL_LIBS}
        Threads::Threads
)

# ============================================================
# MAIN EXECUTABLE
# ============================================================
add_executable(peer_server
    src/main.cpp
)

target_include_directories(peer_server
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(peer_server
    PRIVATE
        app
        com
        signaling
        transport
        Threads::Threads
)

# ============================================================
# INSTALL (optional)
# ============================================================
install(TARGETS peer_server DESTINATION bin)
