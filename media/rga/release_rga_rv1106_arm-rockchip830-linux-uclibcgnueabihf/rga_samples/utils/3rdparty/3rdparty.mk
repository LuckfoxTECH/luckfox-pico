#======================================================================
#
# samples/utils/3rdparty
#
#======================================================================
3RDPARTY_PATH:=$(call my-dir)

LOCAL_C_INCLUDES += \
    $(3RDPARTY_PATH)/libdrm/include \
    $(3RDPARTY_PATH)/libdrm/include/libdrm \
