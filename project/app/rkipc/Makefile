
ifeq ($(APP_PARAM), )
    APP_PARAM:=../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := rkipc
PKG_BIN ?= out
PKG_BUILD ?= build

# debug: build cmake with more message
# PKG_CONF_OPTS += -DCMAKE_VERBOSE_MAKEFILE=ON
#
ifneq ($(RK_BUILD_VERSION_TYPE), DEBUG)
PKG_CONF_OPTS += -DCMAKE_BUILD_TYPE=Release
# PKG_CONF_OPTS += -DCMAKE_BUILD_TYPE=RelWithDebInfo
# PKG_CONF_OPTS += -DCMAKE_BUILD_TYPE=MinSizeRel
else
PKG_CONF_OPTS += -DCMAKE_BUILD_TYPE=Debug
endif
#

RK_RKIPC_CONFIG :=
ifneq (, $(filter RKIPC_RV1126_RKMEDIA, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1126_RKMEDIA=ON
else ifneq (, $(filter RKIPC_RV1126_ROCKIT, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1126_ROCKIT=ON
else ifneq (, $(filter RKIPC_RV1126_SNAPSHOT, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1126_SNAPSHOT=ON
else ifneq (, $(filter RKIPC_RV1126_BATTERY_IPC, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1126_BATTERY_IPC=ON
else ifneq (, $(filter RKIPC_RV1126_DUAL_IPC, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1126_DUAL_IPC=ON
else ifneq (, $(filter RKIPC_RV1126_AIISP, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1126_AIISP=ON
else ifneq (, $(filter RKIPC_RK3588, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RK3588=ON
else ifneq (, $(filter RKIPC_RK3588_MULTI_IPC, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RK3588_MULTI_IPC=ON
else ifneq (, $(filter RKIPC_RV1103, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1103_IPC=ON
else ifneq (, $(filter RKIPC_RV1106, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1106_IPC=ON
else ifneq (, $(filter RKIPC_RV1106_BATTERY_IPC, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1106_BATTERY_IPC_CLIENT=ON
else ifneq (, $(filter RKIPC_RV1106_BATTERY_IPC_TUYA, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1106_BATTERY_IPC_TUYA=ON
else ifneq (, $(filter RKIPC_RV1106_DUAL_IPC, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1106_DUAL_IPC=ON
else ifneq (, $(filter RKIPC_RV1106_WAKEUP_IPC, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1106_WAKEUP_IPC=ON
else ifneq (, $(filter RKIPC_RV1106_AIISP, $(RK_APP_TYPE)))
	RK_RKIPC_CONFIG := -DCOMPILE_FOR_RV1106_AIISP=ON
endif

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
PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_APP_CFLAGS) $(RK_APP_LDFLAGS) $(RK_APP_OPTS)" \
				-DCMAKE_CXX_FLAGS="$(RK_APP_CFLAGS) $(RK_APP_LDFLAGS) $(RK_APP_OPTS)"

PKG_CONF_OPTS += -DRKIPC_CROSS_COMPILE="$(RK_APP_CROSS)"

PKG_TOOLCHAIN_SYSROOT := $(shell $(RK_APP_CROSS)-gcc -print-sysroot)
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH="$(CURRENT_DIR)/$(PKG_BIN);$(RK_APP_OUTPUT);$(PKG_TOOLCHAIN_SYSROOT)"
PKG_CONF_OPTS += -DCMAKE_SYSROOT="$(PKG_TOOLCHAIN_SYSROOT)"

PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
PKG_CONF_OPTS += -DCMAKE_NO_SYSTEM_FROM_IMPORTED=ON
PKG_CONF_OPTS += -DCMAKE_SYSTEM_NAME=Linux

ifneq ($(RK_RKIPC_CONFIG),)
PKG_TARGET := rkipc-build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

rkipc-build: rkfsmk-build thirdlibs-build
	@echo "RK_APP_CHIP is $(RK_APP_CHIP)"
	@echo "RK_APP_TYPE is $(RK_APP_TYPE)"
	rm -rf $(PKG_BIN) $(PKG_BUILD); \
	mkdir -p $(PKG_BIN);
	mkdir -p $(PKG_BUILD);
	pushd $(PKG_BUILD)/; \
		rm -rf CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME)/ \
			-DCMAKE_C_COMPILER=$(RK_APP_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_APP_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DCMAKE_SYSTEM_NAME=Linux \
			$(RK_RKIPC_CONFIG) \
			$(PKG_CONF_OPTS) ;\
			make -j$(RK_APP_JOBS) || exit -1; \
			make install; \
	popd;
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

thirdlibs-build:
	@test ! -d $(RK_APP_COMPONENT)/third_libs || make -C $(RK_APP_COMPONENT)/third_libs

rkfsmk-build:
	@test ! -d $(RK_APP_COMPONENT)/rkfsmk || make -C $(RK_APP_COMPONENT)/rkfsmk

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD)

distclean: clean

info:
ifneq ($(RK_RKIPC_CONFIG),)
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
	@echo -e "RK_APP_TYPE=$(RK_APP_TYPE)"
	@echo -e "option support as follow:"
	@echo -e "	RKIPC_RV1126_RKMEDIA"
	@echo -e "	RKIPC_RV1126_ROCKIT"
	@echo -e "	RKIPC_RV1126_SNAPSHOT"
	@echo -e "	RKIPC_RV1126_BATTERY_IPC"
	@echo -e "	RKIPC_RV1126_DUAL_IPC"
	@echo -e "	RKIPC_RV1126_AIISP"
	@echo -e "	RKIPC_RK3588"
	@echo -e "	RKIPC_RK3588_MULTI_IPC"
	@echo -e "	RKIPC_RV1103"
	@echo -e "	RKIPC_RV1106"
	@echo -e "	RKIPC_RV1106_BATTERY_IPC"
	@echo -e "	RKIPC_RV1106_BATTERY_IPC_TUYA"
	@echo -e "	RKIPC_RV1106_DUAL_IPC"
	@echo -e "	RKIPC_RV1106_WAKEUP_IPC"
	@echo -e "	RKIPC_RV1106_AIISP"
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
endif
