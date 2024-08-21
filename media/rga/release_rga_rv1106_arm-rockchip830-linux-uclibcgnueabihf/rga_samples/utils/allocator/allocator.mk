#======================================================================
#
# samples/utils/allocator
#
#======================================================================
ALLOCATOR_PATH:=$(call my-dir)
ALLOCATOR_SOURCE_PATH := $(patsubst  $(LOCAL_PATH)/%,%,$(ALLOCATOR_PATH))

LOCAL_C_INCLUDES += \
    $(ALLOCATOR_PATH)/include \

LOCAL_SRC_FILES += \
    $(ALLOCATOR_SOURCE_PATH)/drm_alloc.cpp \
    $(ALLOCATOR_SOURCE_PATH)/dma_alloc.cpp \
