LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

###############################################################################
# common compile flags
###############################################################################
LOCAL_CFLAGS += -DANDROID_BUILD -DUSER_SPACE
LOCAL_CFLAGS += -Wall \
		-Wno-error \
		-Wno-enum-conversion \
		-Wno-unused-parameter

###############################################################################
# libdrm dependencies
###############################################################################
LOCAL_CFLAGS += -DMAJOR_IN_SYSMACROS=1 -D_GNU_SOURCE

###############################################################################
# libteec dependencies
###############################################################################
LOCAL_CFLAGS += -DBINARY_PREFIX=\"TEEC\"
TEEC_PATH := $(LOCAL_PATH)/third_party/optee_client/libteec

###############################################################################
# build librkcrypto
###############################################################################
LOCAL_LDFLAGS += -llog \
		 -lcutils

SRC_FILES_DIR := $(wildcard $(LOCAL_PATH)/src/*.c)
SRC_FILES_DIR += $(wildcard $(LOCAL_PATH)/third_party/libdrm/src/*.c)
SRC_FILES_DIR += $(wildcard $(TEEC_PATH)/src/tee_client_api.c)
SRC_FILES_DIR += $(wildcard $(TEEC_PATH)/src/teec_trace.c)
LOCAL_SRC_FILES := $(SRC_FILES_DIR:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/third_party/libdrm/include \
                    $(LOCAL_PATH)/third_party/libdrm/include/drm \
		    $(TEEC_PATH)/include \
		    $(TEEC_PATH)/../public

LOCAL_MODULE:= librkcrypto
LOCAL_MODULE_TAGS := optional

LOCAL_VENDOR_MODULE := true
include $(BUILD_SHARED_LIBRARY)

###############################################################################
# build other components
###############################################################################
include $(call all-makefiles-under, $(LOCAL_PATH))
