cmake_minimum_required(VERSION 3.22)

project(autosar_webrtc_android LANGUAGES CXX)

# =========================
# C++ standard
# =========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Root source
# =========================
set(SRC_ROOT ${CMAKE_SOURCE_DIR}/core)

# =========================
# APP LAYER
# =========================
set(APP_SOURCES
    ${SRC_ROOT}/app/ControlState.cpp
    ${SRC_ROOT}/app/VehicleState.cpp
    ${SRC_ROOT}/app/SafetyManager.cpp
    ${SRC_ROOT}/app/EventHandler.cpp
    ${SRC_ROOT}/app/Time.cpp
    ${SRC_ROOT}/app/EventRing.cpp
)

# =========================
# COM LAYER
# =========================
set(COM_SOURCES
    ${SRC_ROOT}/com/FrameCodec.cpp
    ${SRC_ROOT}/com/RxDispatcher.cpp
)

# =========================
# IO LAYER (ANDROID)
# =========================
set(IO_SOURCES
    ${SRC_ROOT}/io/android/JoystickInputAndroid.cpp
    ${SRC_ROOT}/io/android/WatchdogAndroid.cpp
)

# =========================
# SIGNALING LAYER
# =========================
set(SIGNALING_SOURCES
    ${SRC_ROOT}/signaling/udp_signaling.cpp
    ${SRC_ROOT}/signaling/SignalingManager.cpp
    ${SRC_ROOT}/signaling/PeerClient.cpp
)

# =========================
# TRANSPORT LAYER
# =========================
set(TRANSPORT_SOURCES
    ${SRC_ROOT}/transport/WebRTCTransport.cpp
)

# =========================
# JNI BRIDGE
# =========================
set(JNI_SOURCES
    jni/NativeBridge.cpp
)

# =========================
# BUILD SHARED LIBRARY
# =========================
add_library(autosar_webrtc SHARED
    ${APP_SOURCES}
    ${COM_SOURCES}
    ${IO_SOURCES}
    ${SIGNALING_SOURCES}
    ${TRANSPORT_SOURCES}
    ${JNI_SOURCES}
)

# =========================
# Include directories
# =========================
target_include_directories(autosar_webrtc PRIVATE
    ${SRC_ROOT}
    ${SRC_ROOT}/app
    ${SRC_ROOT}/com
    ${SRC_ROOT}/io
    ${SRC_ROOT}/signaling
    ${SRC_ROOT}/transport
)

# =========================
# Android system libs
# =========================
find_library(log-lib log)

target_link_libraries(autosar_webrtc
    ${log-lib}
)
