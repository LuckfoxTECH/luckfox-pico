cmake_minimum_required(VERSION 3.10)
project(autosar_webrtc_server LANGUAGES CXX)

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

# ============================================================
# APP LAYER
# ============================================================
add_library(app STATIC
    src/app/ControlState.cpp
    src/app/EventRing.cpp
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
