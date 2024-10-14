
ifeq ($(APP_PARAM), )
    APP_PARAM:=../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := fastboot_client
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
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/rkaiq/smartIr \
				-I $(RK_APP_OUTPUT)/include \
				-I $(RK_APP_OUTPUT)/include/freetype2 \

RK_APP_LDFLAGS = -L $(RK_APP_MEDIA_LIBS_PATH) -L$(RK_APP_OUTPUT)/lib

RK_APP_OPTS += -Wl,-rpath-link,$(RK_APP_MEDIA_LIBS_PATH):$(RK_APP_PATH_LIB_INCLUDE)/root/usr/lib:$(RK_APP_OUTPUT)/lib

FASTBOOT_CLIENT_LD_FLAGS += $(RK_APP_OPTS) \
	$(RK_APP_LDFLAGS) $(RK_APP_CFLAGS) \
	-L$(RK_APP_OUTPUT)/lib -Wl,-Bstatic -pthread -lrockit -lrtsp \
	-O2 -mthumb -mthumb-interwork \
	-lrockchip_mpp -lrksysutils -lsmartIr \
	-lrkaudio_detect -laec_bf_process \
	-lrga -Wl,-Bdynamic \
	-Wl,--gc-sections -Wl,--as-needed \
	-lrkaiq -lstdc++ -lm

ifeq ($(RK_ENABLE_FASTBOOT),y)
ifneq (, $(filter RK_FASTBOOT_CLIENT, $(RK_APP_TYPE)))
PKG_TARGET := fastboot_client-build
endif
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

fastboot_client-build:
	rm -rf $(PKG_BIN); \
	mkdir -p $(PKG_BIN)/bin;
	$(RK_APP_CROSS)-gcc fastboot_client.c -o $(PKG_BIN)/bin/fastboot_client $(FASTBOOT_CLIENT_LD_FLAGS)
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

clean:
	@rm -rf $(PKG_BIN)

distclean: clean

info:
ifneq ($(RK_FASTBOOT_CLIENT),)
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
	@echo -e "RK_APP_TYPE=$(RK_APP_TYPE)"
	@echo -e "option support as follow:"
	@echo -e "	RK_FASTBOOT_CLIENT"
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
endif
