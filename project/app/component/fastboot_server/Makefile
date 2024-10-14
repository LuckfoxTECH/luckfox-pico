
ifeq ($(APP_PARAM), )
    APP_PARAM:=../../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := fastboot_server
PKG_BIN ?= out

RK_APP_CFLAGS = -I $(RK_APP_MEDIA_INCLUDE_PATH) \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/libdrm \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/rkaiq \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/rkaiq/uAPI \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/rkaiq/uAPI2 \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/rkaiq/algos \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/rkaiq/common \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/rkaiq/xcore \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/rkaiq/iq_parser \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/rkaiq/iq_parser_v2 \
				-I $(RK_APP_OUTPUT)/include \
				-I $(RK_APP_OUTPUT)/include/freetype2 \
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/easymedia

RK_APP_LDFLAGS = -L $(RK_APP_MEDIA_LIBS_PATH) -L$(RK_APP_OUTPUT)/lib

RK_APP_OPTS += -Wl,-rpath-link,$(RK_APP_MEDIA_LIBS_PATH):$(RK_APP_PATH_LIB_INCLUDE)/root/usr/lib:$(RK_APP_OUTPUT)/lib

ifneq (, $(filter SMART_DOOR_RV1106_RMSL, $(RK_APP_TYPE)))
FASTBOOT_SERVER_SRC := fastboot_server_multi_cam.c v4l2_link.c
FASTBOOT_PKG_LDFLAGS := -Wl,-Bstatic \
		-lrockit_tiny -lrockchip_mpp -lrksysutils \
		-Wl,-Bdynamic \
		-ldl -lstdc++
else

ifneq (, $(filter RK_FASTBOOT_CLIENT_DUAL_CAMERA, $(RK_APP_TYPE)))
PKG_TARGET_RES_DIR := $(PKG_BIN)/root/oem/usr/share
FASTBOOT_SERVER_SRC := fastboot_server_dual_camera.c
FASTBOOT_PKG_LDFLAGS := -Wl,-Bstatic -pthread -lrockit_full -lrtsp \
		-O2 -mthumb -mthumb-interwork \
		-lrockchip_mpp -lrksysutils -lsmartIr \
		-lrkaudio_detect -laec_bf_process \
		-lrga -Wl,-Bdynamic \
		-Wl,--gc-sections -Wl,--as-needed \
		-lrkaiq -lstdc++ -lm
endif

ifneq (, $(filter RK_FASTBOOT_SERVER, $(RK_APP_TYPE)))
FASTBOOT_SERVER_BUILD_WITH_DLOPEN := y
FASTBOOT_SERVER_SRC := fastboot_server.c
ifeq ($(FASTBOOT_SERVER_BUILD_WITH_DLOPEN),y)
RK_APP_CFLAGS += -DRKAIQ_USE_DLOPEN
else
FASTBOOT_PKG_LDFLAGS := -Wl,-Bstatic -lrkaiq -Wl,-Bdynamic
endif

FASTBOOT_PKG_LDFLAGS += -Wl,-Bstatic \
		-lrockit_tiny -lrockchip_mpp -lrksysutils -lstdc++ \
		-Wl,-Bdynamic \
		-ldl
endif

endif

ifeq ("$(RK_ENABLE_AOV)", "y")
	RK_APP_CFLAGS += -DENABLE_AOV
endif

ifneq ($(filter RK_FASTBOOT_SERVER SMART_DOOR_RV1106_RMSL RK_FASTBOOT_CLIENT_DUAL_CAMERA,$(RK_APP_TYPE)),)
PKG_TARGET := fastboot_server-build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

fastboot_server-build:
	rm -rf $(PKG_BIN); \
	mkdir -p $(PKG_BIN)/bin;
	@echo "$(PKG_NAME) building $(FASTBOOT_SERVER_SRC)"
	@$(RK_APP_CROSS)-gcc $(FASTBOOT_SERVER_SRC) -o $(PKG_BIN)/bin/fastboot_server \
		-O2 -mthumb -mthumb-interwork \
		-fdata-sections -ffunction-sections -Wl,--gc-sections \
		$(RK_APP_LDFLAGS) $(RK_APP_CFLAGS) \
		-L$(RK_APP_MEDIA_LIBS_PATH)/lib \
		$(FASTBOOT_PKG_LDFLAGS)
	@test -z $(PKG_TARGET_RES_DIR) || (mkdir -p $(PKG_TARGET_RES_DIR); cp $(CURRENT_DIR)/osd_image.data $(PKG_TARGET_RES_DIR);)
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

clean:
	@rm -rf $(PKG_BIN)

distclean: clean

info:
ifneq ($(RK_ENABLE_FASTBOOT),)
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
endif
