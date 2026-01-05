cmake_minimum_required(VERSION 3.10)
project(autosar_webrtc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Threads
# =========================
find_package(Threads REQUIRED)

# =========================
# libdatachannel (WebRTC)
# =========================
find_package(PkgConfig QUIET)

set(LIBDATACHANNEL_LIBS "")
set(LIBDATACHANNEL_INCLUDE_DIRS "")
set(LIBDATACHANNEL_LIBRARY_DIRS "")

if (PKG_CONFIG_FOUND)
    pkg_check_modules(LIBDATACHANNEL libdatachannel)
endif()

if (LIBDATACHANNEL_FOUND)
    message(STATUS "Found libdatachannel via pkg-config")
    set(LIBDATACHANNEL_LIBS ${LIBDATACHANNEL_LIBRARIES})
    set(LIBDATACHANNEL_INCLUDE_DIRS ${LIBDATACHANNEL_INCLUDE_DIRS})
else()
    message(WARNING "libdatachannel not found via pkg-config, using fallback")
    set(LIBDATACHANNEL_LIBS datachannel sctp ssl crypto pthread dl)
    set(LIBDATACHANNEL_INCLUDE_DIRS /usr/local/include)
endif()

# =========================
# Global include
# =========================
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${LIBDATACHANNEL_INCLUDE_DIRS}
)

# ============================================================
# APP LAYER (Application SWC)
# ============================================================
add_library(app STATIC
    src/app/ControlState.cpp
    src/app/SafetyManager.cpp
    src/app/EventHandler.cpp
    src/app/Time.cpp
    src/app/EventRing.cpp
)

target_include_directories(app PUBLIC src/app)

# ============================================================
# COM LAYER (AUTOSAR COM)
# ============================================================
add_library(com STATIC
    src/com/FrameCodec.cpp
)

target_include_directories(com PUBLIC src/com)

# ============================================================
# IO LAYER (Drivers / Sensors)
# ============================================================
add_library(io STATIC
    src/io/JoystickInput.cpp
    src/io/Watchdog.cpp
)

target_include_directories(io PUBLIC src/io)

# ============================================================
# SIGNALING LAYER (UDP + SDP/ICE FSM)
# ============================================================
add_library(signaling STATIC
    src/signaling/udp_signaling.cpp
    src/signaling/SignalingManager.cpp
    src/signaling/PeerClient.cpp
)

target_include_directories(signaling PUBLIC src/signaling)

target_link_libraries(signaling
    PRIVATE
        Threads::Threads
)

# ============================================================
# TRANSPORT LAYER
# ============================================================
add_library(transport STATIC
    src/transport/WebRTCTransport.cpp
)

target_include_directories(transport PUBLIC src/transport)

target_link_libraries(transport
    PRIVATE
        ${LIBDATACHANNEL_LIBS}
        Threads::Threads
)

# ============================================================
# MAIN EXECUTABLE
# ============================================================
add_executable(peer_client
    src/main.cpp
)

target_link_libraries(peer_client
    PRIVATE
        app
        com
        io
        signaling
        transport
        Threads::Threads
)

target_link_libraries(io
    PUBLIC
        app
)


# ============================================================
# INSTALL (optional)
# ============================================================
install(TARGETS peer_client DESTINATION bin)
