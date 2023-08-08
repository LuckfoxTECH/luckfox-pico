LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= libc_mode

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include

LOCAL_CFLAGS += -Wno-unused-parameter -Wno-unused-function

SRC_FILES_DIR := $(wildcard $(LOCAL_PATH)/*.c)
LOCAL_SRC_FILES := $(SRC_FILES_DIR:$(LOCAL_PATH)/%=%)


TARGET_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional


ifeq ($(strip $(TARGET_ARCH)), arm64)
	LOCAL_MULTILIB := 64
else
	LOCAL_MULTILIB := 32
endif

LOCAL_VENDOR_MODULE := true
include $(BUILD_STATIC_LIBRARY)
