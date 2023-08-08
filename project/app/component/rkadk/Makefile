ifeq ($(APP_PARAM), )
    APP_PARAM:=../../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
include rkadk.cfg.mk

PKG_NAME := rkadk
PKG_BIN ?= out
PKG_BUILD ?= build

PKG_TARGET := rkadk-build

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

RKADK_CFLAGS = -I $(RK_APP_MEDIA_INCLUDE_PATH) \
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
PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RKADK_CFLAGS) $(RK_APP_LDFLAGS) $(RK_APP_OPTS)" \
				-DCMAKE_CXX_FLAGS="$(RKADK_CFLAGS) $(RK_APP_LDFLAGS) $(RK_APP_OPTS)"

# debug: build cmake with more message
# PKG_CONF_OPTS += -DCMAKE_VERBOSE_MAKEFILE=ON
#
ifeq ($(BR2_PACKAGE_RKADK_USE_AIQ), y)
PKG_CONF_OPTS += -DUSE_RKAIQ=ON
endif

PKG_CONF_OPTS += -DRKADK_CHIP=$(RK_APP_CHIP)

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

rkfsmk-build:
	@test ! -d $(RK_APP_COMPONENT)/rkfsmk || make -C $(RK_APP_COMPONENT)/rkfsmk

thirdlibs-build:
	@test ! -d $(RK_APP_COMPONENT)/third_libs || make -C $(RK_APP_COMPONENT)/third_libs

rkadk-build: rkfsmk-build thirdlibs-build rkadk-release
	@echo "RKADK_CHIP is $(RK_APP_CHIP)"
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME) || \
		(\
	echo "Start to build $(PKG_NAME)"; \
	rm -rf $(PKG_BIN) $(PKG_BUILD); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD)/; \
	pushd $(CURRENT_DIR)/$(PKG_BUILD); \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME)/ \
			-DCMAKE_C_COMPILER=$(RK_APP_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_APP_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DBUILD_DOC=OFF \
			-DBUILD_DOCS=OFF \
			-DBUILD_EXAMPLE=OFF \
			-DBUILD_EXAMPLES=OFF \
			-DBUILD_TESTS=OFF \
			-DBUILD_TESTING=OFF \
			-DBUILD_SHARED_LIBS=ON \
			-DCMAKE_SYSTEM_NAME=Linux \
			-DRKAIQ_INCLUDE_DIR=$(RK_APP_MEDIA_INCLUDE_PATH)/include/rkaiq \
			-DRKAIQ_LIBRARY=$(RK_APP_MEDIA_INCLUDE_PATH)/lib/librkaiq.so \
			$(PKG_CONF_OPTS) \
			-DARCH="$(RK_APP_ARCH_TYPE)" ;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
	popd; )
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))
	@echo "build $(PKG_NAME) done";

rkadk-release:
	$(call MAROC_PKG_CHECK_PREBUILT, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD);

distclean: clean
################################################################################
__RELEASE__: # DO NOT RUN THIS TARGET
	$(call MAROC_STRIP_DEBUG_SYMBOL, $(PKG_BIN))
	$(call MAROC_PKG_RELEASE, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))
