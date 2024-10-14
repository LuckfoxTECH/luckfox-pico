LOCAL_PATH := $(call my-dir)
#======================================================================
#
# rga_allocator_graphicbuffer_demo
#
#======================================================================
include $(CLEAR_VARS)
LOCAL_VENDOR_MODULE := true

LOCAL_CFLAGS += -Wall -Werror -Wunreachable-code

LOCAL_C_INCLUDES += \
    hardware/rockchip/librga \
    hardware/rockchip/librga/include \
    hardware/rockchip/librga/im2d_api

LOCAL_C_INCLUDES += \
    hardware/rockchip/librga/samples/utils \
    hardware/rockchip/librga/samples/3rdparty \
    hardware/rockchip/librga/samples/3rdparty/libdrm/include \
    hardware/rockchip/librga/samples/3rdparty/libdrm/include/libdrm \

LOCAL_SHARED_LIBRARIES := \
    libui \
    libutils \
    librga

LOCAL_HEADER_LIBRARIES += \
    libhardware_headers \
    libgui_headers \

LOCAL_SRC_FILES:= \
    ../utils/utils.cpp \
    src/rga_allocator_graphicbuffer_demo.cpp

LOCAL_MODULE:= \
    rga_allocator_graphicbuffer_demo

ifdef TARGET_32_BIT_SURFACEFLINGER
LOCAL_32_BIT_ONLY := true
endif

include $(BUILD_EXECUTABLE)

