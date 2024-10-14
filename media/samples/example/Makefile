
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../../Makefile.param
    include $(MEDIA_PARAM)
endif

export CONFIG_RK_SAMPLE=y
export LC_ALL=C
export HAVE_AVS=y
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

RK_MEDIA_OPTS += -Wl,-rpath-link,${RK_MEDIA_OUTPUT}/lib:$(RK_MEDIA_OUTPUT)/root/usr/lib
PKG_CONF_OPTS += -DRKPLATFORM=ON

ifeq ($(RK_MEDIA_CHIP), rv1126)
PKG_CONF_OPTS += -DCMAKE_SYSTEM_PROCESSOR=armv7l
PKG_CONF_OPTS += -DARCH64=OFF
endif

ifeq ($(RK_MEDIA_CHIP), rv1106)
PKG_CONF_OPTS += -DARCH64=OFF
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
else
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/uAPI2
endif
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/common
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/xcore
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/algos
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/iq_parser
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/iq_parser_v2
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/smartIr
SAMPLE_CFLAGS += -g -Wall $(INC_FLAGS) $(PKG_CONF_OPTS) -lpthread $(RK_MEDIA_CROSS_CFLAGS)

ifeq ($(RK_MEDIA_CHIP), rv1126)
INC_FLAGS += -I$(COMM_DIR)/isp2.x
SAMPLE_CFLAGS += -DISP_HW_V20
SAMPLE_CFLAGS += -DHAVE_VO -DRV1126
SAMPLE_CFLAGS += -DRKAIQ_GRP
endif

ifeq ($(RK_MEDIA_CHIP), rv1106)
INC_FLAGS += -I$(COMM_DIR)/isp3.x
SAMPLE_CFLAGS += -DISP_HW_V30 -DRV1106 -DROCKIT_IVS -DROCKCOMBO -DUAPI2 -DRKAIQ_GRP
endif

ifeq ($(CONFIG_RK_IVA),y)
SAMPLE_CFLAGS += -DROCKIVA
endif

ifeq ($(HAVE_AVS), y)
SAMPLE_CFLAGS += -DAVS_ENABLE
endif

ifeq ($(CONFIG_RK_LIBEASYIIO),y)
SAMPLE_CFLAGS += -DEIS_ENABLE
endif

export SAMPLE_OUT_DIR=$(CURRENT_DIR)/out
export PKG_CONF_OPTS
export COMM_OBJ COMM_SRC COMM_DIR
export SAMPLE_CC SAMPLE_AR
export SAMPLE_CFLAGS RK_MEDIA_CROSS

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done";

COMM_LIB:
	@make -C $(CURRENT_DIR)/common

sample-build:
	@test -f $(RK_MEDIA_OUTPUT)/lib/librockit.so  || make -C $(RK_MEDIA_TOP_DIR) media_libs
	@mkdir -p $(SAMPLE_OUT_DIR)/bin
	@make COMM_LIB
	@make -C $(CURRENT_DIR)/audio;
	@make -C $(CURRENT_DIR)/audio install;
	@make -C $(CURRENT_DIR)/vi;
	@make -C $(CURRENT_DIR)/vi install;
ifneq ($(RK_MEDIA_CHIP), rv1106)
	@make -C $(CURRENT_DIR)/vo;
	@make -C $(CURRENT_DIR)/vo install;
endif
	@make -C $(CURRENT_DIR)/venc;
	@make -C $(CURRENT_DIR)/venc install;
ifeq ($(HAVE_AVS), y)
	@make -C $(CURRENT_DIR)/avs;
	@make -C $(CURRENT_DIR)/avs  install;
endif
	@make -C $(CURRENT_DIR)/test;
	@make -C $(CURRENT_DIR)/test install;
	@make -C $(CURRENT_DIR)/demo;
	@make -C $(CURRENT_DIR)/demo install;
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))

clean:
	@make -C $(CURRENT_DIR)/common clean
	@make -C $(CURRENT_DIR)/audio clean
	@make -C $(CURRENT_DIR)/vi clean
	@make -C $(CURRENT_DIR)/vo clean
	@make -C $(CURRENT_DIR)/venc clean
	@make -C $(CURRENT_DIR)/test clean
	@make -C $(CURRENT_DIR)/demo clean
	@rm -rf $(SAMPLE_OUT_DIR)

help:
	@echo "help message:"
	@echo "     build with dynamic link:  make "
	@echo "     build with static  link:  make rk_static=1"

distclean: clean
