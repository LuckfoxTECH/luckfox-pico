#======================================================================
#
# samples/utils
#
#======================================================================
UTILS_PATH:=$(call my-dir)
UTILS_SOURCE_PATH := $(patsubst  $(LOCAL_PATH)/%,%,$(UTILS_PATH))

LOCAL_C_INCLUDES += \
    $(UTILS_PATH) \

LOCAL_SRC_FILES += \
    $(UTILS_SOURCE_PATH)/utils.cpp

include $(UTILS_PATH)/3rdparty/3rdparty.mk
include $(UTILS_PATH)/allocator/allocator.mk

