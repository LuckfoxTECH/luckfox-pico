
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export CONFIG_RK_SAMPLE=y
export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

ifeq ($(rk_static),1)
export BUILD_STATIC_LINK=y
else
export BUILD_STATIC_LINK=n
endif

ifeq ($(RK_MEDIA_SAMPLE_STATIC_LINK),y)
export BUILD_STATIC_LINK=y
else
export BUILD_STATIC_LINK=n
endif

SAMPLE_CC := $(RK_MEDIA_CROSS)-gcc
SAMPLE_AR := $(RK_MEDIA_CROSS)-ar

PKG_NAME := sample
PKG_BIN ?= out
PKG_BUILD ?= build

RK_MEDIA_OPTS += -Wl,-rpath-link,${RK_MEDIA_OUTPUT}/lib:$(RK_MEDIA_OUTPUT)/root/usr/lib
PKG_CONF_OPTS += -DRKPLATFORM=ON

ifeq ($(RK_MEDIA_CHIP), rv1126)
PKG_CONF_OPTS += -DCMAKE_SYSTEM_PROCESSOR=armv7l
PKG_CONF_OPTS += -DARCH64=OFF
endif

ifeq ($(RK_MEDIA_CHIP), rv1106)
PKG_CONF_OPTS += -DARCH64=OFF
endif

ifeq ($(CONFIG_RK_IVA),y)
	LD_FLAGS += -DROCKIVA
	LD_FLAGS += -DROCKIVA -lrockiva
endif

ifeq ($(CONFIG_RK_SAMPLE),y)
PKG_TARGET := sample-build
else
PKG_TARGET :=
$(warning Not config source RK_SAMPLE, Skip...)
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

COMM_DIR := $(CURRENT_DIR)/common
COMM_SRC := $(wildcard $(COMM_DIR)/*.c)
ifeq ($(RK_MEDIA_CHIP), rv1126)
COMM_SRC += $(wildcard $(COMM_DIR)/isp2.x/*.c)
endif
ifeq ($(RK_MEDIA_CHIP), rv1106)
COMM_SRC += $(wildcard $(COMM_DIR)/isp3.x/*.c)
endif
COMM_OBJ := $(COMM_SRC:%.c=%.o)

INC_FLAGS := -I$(COMM_DIR)
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq
ifeq ($(RK_MEDIA_CHIP), rv1126)
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/uAPI
LD_FLAGS += -DUAPI
else
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/uAPI2
LD_FLAGS += -DUAPI2
endif
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/common
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/xcore
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/algos
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/iq_parser
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/iq_parser_v2
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/smartIr
SAMPLE_CFLAGS += -g -Wall $(INC_FLAGS) $(PKG_CONF_OPTS)
LD_FLAGS += -L$(SAMPLE_OUT_DIR)/lib -lsample_comm
ifeq ($(BUILD_STATIC_LINK), y)
LD_FLAGS += $(RK_MEDIA_OPTS) -L$(RK_MEDIA_OUTPUT)/lib -Wl,-Bstatic -lpthread -lrockit -lrockchip_mpp -lrkaiq \
			-lrkaudio_detect -laec_bf_process  \
		  -lm -lrga -lstdc++ -Wl,-Bdynamic
LD_FLAGS += -L$(CURRENT_DIR)/lib/$(RK_MEDIA_CROSS) -Wl,-Bstatic -lrtsp -Wl,-Bdynamic -Wl,--gc-sections -Wl,--as-needed
else
LD_FLAGS += $(RK_MEDIA_OPTS) -L$(RK_MEDIA_OUTPUT)/lib  -lrockit -lrockchip_mpp -lrkaiq -lpthread -lm -ldl
LD_FLAGS += -L$(CURRENT_DIR)/lib/$(RK_MEDIA_CROSS) -lrtsp
endif

ifeq ($(RK_MEDIA_CHIP), rv1126)
INC_FLAGS += -I$(COMM_DIR)/isp2.x
SAMPLE_CFLAGS += -DISP_HW_V20
LD_FLAGS += -L$(RK_MEDIA_OUTPUT)/root/usr/lib -lasound
SAMPLE_CFLAGS += -DHAVE_VO
LD_FLAGS += -ldrm
endif

ifeq ($(RK_MEDIA_CHIP), rv1106)
INC_FLAGS += -I$(COMM_DIR)/isp3.x
SAMPLE_CFLAGS += -DISP_HW_V30
endif

export SAMPLE_OUT_DIR=$(CURRENT_DIR)/out
export SAMPLE_SRC_DIR=$(CURRENT_DIR)/simple_test/
export PKG_CONF_OPTS
export COMM_OBJ COMM_SRC COMM_DIR
export SAMPLE_CC SAMPLE_AR
export SAMPLE_CFLAGS
export LD_FLAGS

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done";

sample-build: libasound librkaiq librockit librockiva librksysutils COMM_LIB
	@mkdir -p $(SAMPLE_OUT_DIR)/bin
	@make -C $(CURRENT_DIR)/audio;
	@make -C $(CURRENT_DIR)/audio install;
	@make -C $(CURRENT_DIR)/vi;
	@make -C $(CURRENT_DIR)/vi install;
	@make -C $(CURRENT_DIR)/simple_test;
	@make -C $(CURRENT_DIR)/simple_test install;
ifneq ($(RK_MEDIA_CHIP), rv1106)
	@make -C $(CURRENT_DIR)/vo;
	@make -C $(CURRENT_DIR)/vo install;
endif
	@make -C $(CURRENT_DIR)/venc;
	@make -C $(CURRENT_DIR)/venc install;
	@make -C $(CURRENT_DIR)/test;
	@make -C $(CURRENT_DIR)/test install;
	@make -C $(CURRENT_DIR)/demo;
	@make -C $(CURRENT_DIR)/demo install;
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))

COMM_LIB:
	@make -C $(CURRENT_DIR)/common

libasound:
	@test ! -d $(RK_MEDIA_TOP_DIR)/alsa-lib || make -C $(RK_MEDIA_TOP_DIR)/alsa-lib

librkaiq:
	@test ! -d $(RK_MEDIA_TOP_DIR)/isp || make -C $(RK_MEDIA_TOP_DIR)/isp

librockit:
	@test ! -d $(RK_MEDIA_TOP_DIR)/rockit || make -C $(RK_MEDIA_TOP_DIR)/rockit

librockiva:
	@test ! -d $(RK_MEDIA_TOP_DIR)/iva || make -C $(RK_MEDIA_TOP_DIR)/iva

librksysutils:
	@test ! -d $(RK_MEDIA_TOP_DIR)/sysutils || make -C $(RK_MEDIA_TOP_DIR)/sysutils

clean:
	@make -C $(CURRENT_DIR)/common clean
	@make -C $(CURRENT_DIR)/audio clean
	@make -C $(CURRENT_DIR)/vi clean
	@make -C $(CURRENT_DIR)/vo clean
	@make -C $(CURRENT_DIR)/venc clean
	@make -C $(CURRENT_DIR)/simple_test clean
	@make -C $(CURRENT_DIR)/test clean
	@make -C $(CURRENT_DIR)/demo clean
	@rm -rf $(SAMPLE_OUT_DIR)

help:
	@echo "help message:"
	@echo "     build with dynamic link:  make "
	@echo "     build with static  link:  make rk_static=1"

distclean: clean
