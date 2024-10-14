ifneq ($(filter rk312% rk322% rk3288 rk3328 rk3368 rk3399 rk3399pro px3 px5, $(TARGET_BOARD_PLATFORM)), )
$(info 'building rk_tee_user v1')

LOCAL_PATH := $(call my-dir)
OPTEE_TEST_PATH := $(shell pwd)/$(LOCAL_PATH)

VERSION = $(shell git describe --always --dirty=-dev 2>/dev/null || echo Unknown)
OPTEE_CLIENT_PATH ?= $(LOCAL_PATH)/client_export
TA_DEV_KIT_DIR ?= $(OPTEE_TEST_PATH)/export-user_ta
-include $(TA_DEV_KIT_DIR)/host_include/conf.mk

ifeq (1,$(strip $(shell expr $(PLATFORM_VERSION) \>= 11)))
ifeq ($(strip $(TARGET_ARCH)), arm64)
CLIENT_LIB_PATH ?= $(shell pwd)/hardware/rockchip/optee/v1/arm64
else
CLIENT_LIB_PATH ?= $(shell pwd)/hardware/rockchip/optee/v1/arm
endif
else ifeq (1,$(strip $(shell expr $(PLATFORM_VERSION) \>= 8)))
ifeq ($(strip $(TARGET_ARCH)), arm64)
CLIENT_LIB_PATH ?= $(shell pwd)/vendor/rockchip/common/security/optee/v1/lib/arm64
else
CLIENT_LIB_PATH ?= $(shell pwd)/vendor/rockchip/common/security/optee/v1/lib/arm
endif
else
ifeq ($(strip $(TARGET_ARCH)), arm64)
CLIENT_LIB_PATH ?= $(shell pwd)/vendor/rockchip/common/security/optee/lib/arm64
else
CLIENT_LIB_PATH ?= $(shell pwd)/vendor/rockchip/common/security/optee/lib/arm
endif
endif

################################################################################
# Build rktest                                                                 #
################################################################################
include $(CLEAR_VARS)
LOCAL_CFLAGS += -DANDROID_BUILD -DUSER_SPACE
LOCAL_LDFLAGS += $(CLIENT_LIB_PATH)/libteec.so
LOCAL_LDFLAGS += -llog

SRC_FILES_DIR := $(wildcard $(LOCAL_PATH)/host/rk_test/*.c)
LOCAL_SRC_FILES += $(SRC_FILES_DIR:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ta/rk_test/include \
		$(LOCAL_PATH)/host/rk_test/include \
		$(LOCAL_PATH)/export-user_ta/host_include \
		$(OPTEE_CLIENT_PATH)/public

LOCAL_MODULE := rktest
LOCAL_MODULE_TAGS := optional
ifeq (1,$(strip $(shell expr $(PLATFORM_VERSION) \>= 8)))
LOCAL_PROPRIETARY_MODULE := true
endif
include $(BUILD_EXECUTABLE)


include $(LOCAL_PATH)/ta/Android.mk
endif