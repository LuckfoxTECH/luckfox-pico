cmake_minimum_required(VERSION 3.10)
project(libdc_udp_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------
# Dependencies
# --------------------------------------------------
find_package(Threads REQUIRED)
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
    set(LIBDATACHANNEL_LIBRARY_DIRS ${LIBDATACHANNEL_LIBRARY_DIRS})
else()
    message(STATUS "pkg-config couldn't find libdatachannel, using fallback")

    if (DEFINED ENV{LIBDATACHANNEL_INCLUDE_DIR})
        set(LIBDATACHANNEL_INCLUDE_DIRS $ENV{LIBDATACHANNEL_INCLUDE_DIR})
    else()
        set(LIBDATACHANNEL_INCLUDE_DIRS /usr/local/include)
    endif()

    if (DEFINED ENV{LIBDATACHANNEL_LIB})
        set(LIBDATACHANNEL_LIBS $ENV{LIBDATACHANNEL_LIB})
    else()
        set(LIBDATACHANNEL_LIBS
            datachannel
            sctp
            ssl
            crypto
            pthread
            dl
        )
    endif()

    if (DEFINED ENV{LIBDATACHANNEL_LIBRARY_DIR})
        set(LIBDATACHANNEL_LIBRARY_DIRS $ENV{LIBDATACHANNEL_LIBRARY_DIR})
    else()
        set(LIBDATACHANNEL_LIBRARY_DIRS /usr/local/lib)
    endif()
endif()

# --------------------------------------------------
# Libraries
# --------------------------------------------------

# --- UDP signaling layer ---------------------------
add_library(signaling
    src/udp_signaling.cpp
)

target_include_directories(signaling
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(signaling
    PUBLIC
        Threads::Threads
)

# --- Signaling FSM layer ---------------------------
add_library(signaling_fsm
    src/SignalingManager.cpp
)

target_include_directories(signaling_fsm
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${LIBDATACHANNEL_INCLUDE_DIRS}
)

target_link_directories(signaling_fsm
    PUBLIC
        ${LIBDATACHANNEL_LIBRARY_DIRS}
)

target_link_libraries(signaling_fsm
    PUBLIC
        signaling
)

# --- WebRTC transport layer ------------------------
add_library(webrtc_transport
    src/WebRTCTransport.cpp
)

target_include_directories(webrtc_transport
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${LIBDATACHANNEL_INCLUDE_DIRS}
)

target_link_directories(webrtc_transport
    PUBLIC
        ${LIBDATACHANNEL_LIBRARY_DIRS}
)

target_link_libraries(webrtc_transport
    PUBLIC
        ${LIBDATACHANNEL_LIBS}
        Threads::Threads
)

# --- PeerServer (glue layer) -----------------------
add_library(peer
    src/PeerServer.cpp
)

target_include_directories(peer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(peer
    PUBLIC
        signaling_fsm
        webrtc_transport
)

# --------------------------------------------------
# Executable
# --------------------------------------------------

add_executable(peer_server
    src/main.cpp
)

target_link_libraries(peer_server
    PRIVATE
        peer
)

# --------------------------------------------------
# Install (optional)
# --------------------------------------------------
install(TARGETS peer_server DESTINATION bin)
