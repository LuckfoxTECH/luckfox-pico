LOCAL_PATH:= $(call my-dir)
#======================================================================
#
#im2d_slt
#
#======================================================================
include $(CLEAR_VARS)
LOCAL_VENDOR_MODULE := true

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES

LOCAL_CFLAGS += -DROCKCHIP_GPU_LIB_ENABLE

LOCAL_CFLAGS += -Wall -Werror -Wunreachable-code

LOCAL_C_INCLUDES += external/tinyalsa/include

LOCAL_C_INCLUDES += hardware/rockchip/librga
LOCAL_C_INCLUDES += hardware/rk29/librga \
    system/core \
    system/core/include/utils \
    system/core/liblog/include \
    hardware/rockchip/librga \
    hardware/rockchip/librga/include \
    # $(LOCAL_PATH)/third-party/libdrm/include \
    # $(LOCAL_PATH)/third-party/libdrm/include/libdrm

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libutils \
    libui \
    libEGL \
    libGLESv1_CM \
    libhardware \
    librga

LOCAL_HEADER_LIBRARIES += \
    libutils_headers \
    libcutils_headers \
    libhardware_headers \
    liblog_headers \
    libgui_headers \
    libbinder_headers

#has no "external/stlport" from Android 6.0 on
ifeq (1,$(strip $(shell expr $(PLATFORM_VERSION) \< 6.0)))
LOCAL_C_INCLUDES += \
    external/stlport/stlport

LOCAL_C_INCLUDES += bionic
endif

include $(LOCAL_PATH)/../utils/utils.mk

LOCAL_SRC_FILES += \
    sources/rga_im2d_slt.cpp

LOCAL_MODULE:= im2d_slt

ifdef TARGET_32_BIT_SURFACEFLINGER
LOCAL_32_BIT_ONLY := true
endif

include $(BUILD_EXECUTABLE)

