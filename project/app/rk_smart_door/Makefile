
ifeq ($(APP_PARAM), )
    APP_PARAM:=../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := smart_door
PKG_BIN ?= out
PKG_BUILD ?= build

SMART_DOOR_CONFIG :=
ifneq (, $(filter SMART_DOOR_RV1106_RMSL, $(RK_APP_TYPE)))
	SMART_DOOR_CONFIG := -DCOMPILE_FOR_RV1106_RMSL=ON
else ifneq (, $(filter SMART_DOOR_RV1106_NORMAL, $(RK_APP_TYPE)))
	SMART_DOOR_CONFIG := -DCOMPILE_FOR_RV1106_NORMAL=ON
else ifneq (, $(filter SMART_DOOR_RV1106_IR_FACE, $(RK_APP_TYPE)))
	SMART_DOOR_CONFIG := -DCOMPILE_FOR_RV1106_IR_FACE=ON
else ifneq (, $(filter SMART_DOOR_RV1103_RMSL, $(RK_APP_TYPE)))
	SMART_DOOR_CONFIG := -DCOMPILE_FOR_RV1103_RMSL=ON
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
				-I $(RK_APP_MEDIA_INCLUDE_PATH)/easymedia \
				-I $(RK_APP_OUTPUT)/include


RK_APP_LDFLAGS = -L $(RK_APP_MEDIA_LIBS_PATH) -L $(RK_APP_OUTPUT)/lib

RK_APP_OPTS += -Wl,-rpath-link,$(RK_APP_MEDIA_LIBS_PATH):$(RK_APP_PATH_LIB_INCLUDE)/root/usr/lib
PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_APP_CFLAGS) $(RK_APP_LDFLAGS) $(RK_APP_OPTS)" \
		 -DCMAKE_CXX_FLAGS="$(RK_APP_CFLAGS) $(RK_APP_LDFLAGS) $(RK_APP_OPTS)"

# define in project/cfg/BoardConfig_SmartDoor/BoardConfig*.mk
ifneq ($(SMART_DOOR_CONFIG),)
PKG_TARGET := thirdlibs-build smart_door_build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

smart_door_build:
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
			-DRK_BUILD_APP_TO_OEM_PARTITION=$(RK_BUILD_APP_TO_OEM_PARTITION) \
			$(SMART_DOOR_CONFIG) \
			$(PKG_CONF_OPTS) ;\
			make -j$(RK_APP_JOBS) || exit -1; \
			make install; \
	popd;
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

thirdlibs-build:
	@test ! -d $(RK_APP_COMPONENT)/third_libs || make -C $(RK_APP_COMPONENT)/third_libs

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD)

distclean: clean

info:
ifneq ($(SMART_DOOR_CONFIG),)
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
	@echo -e "RK_APP_TYPE=$(RK_APP_TYPE)"
	@echo -e "option support as follow:"
	@echo -e "	SMART_DOOR_RV1106_RMSL"
	@echo -e "	SMART_DOOR_RV1106_NORMAL"
	@echo -e "	SMART_DOOR_RV1106_IR_FACE"
	@echo -e "	SMART_DOOR_RV1103_RMSL"
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
endif
