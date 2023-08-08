
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export CONFIG_RK_SYSUTILS=y
export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

CC := $(RK_MEDIA_CROSS)-gcc
AR := $(RK_MEDIA_CROSS)-ar

PKG_NAME := sysutils
PKG_BIN ?= out
PKG_BUILD ?= build

RK_MEDIA_OPTS += -Wl,-rpath-link,${RK_MEDIA_OUTPUT}/lib:$(RK_MEDIA_OUTPUT)/root/usr/lib
PKG_CONF_OPTS += $(RK_MEDIA_OPTS)
PKG_CONF_OPTS += -DRKPLATFORM=ON

ifeq ($(RK_MEDIA_CHIP), rv1126)
PKG_CONF_OPTS += -DCMAKE_SYSTEM_PROCESSOR=armv7l
PKG_CONF_OPTS += -DARCH64=OFF
endif

ifeq ($(RK_MEDIA_CHIP), rk3588)
PKG_CONF_OPTS += -DARCH64=ON
endif

ifeq ($(CONFIG_RK_SYSUTILS),y)
PKG_TARGET := sysutils-build
else
PKG_TARGET :=
$(warning Not config source RK_SYSUTILS, Skip...)
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

SRC += $(wildcard $(CURRENT_DIR)/src/*.c)
OBJ := $(SRC:%.c=%.o)

INC_FLAGS := -I$(CURRENT_DIR)/include
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include
CFLAGS += -g -Wall $(INC_FLAGS) $(PKG_CONF_OPTS) -fPIC -lpthread -lm -ldl
LD_FLAGS += -L$(RK_MEDIA_OUTPUT)/lib  -lrockit -lrockchip_mpp -lasound -ldrm -lrkaiq
LD_FLAGS += -L$(RK_MEDIA_OUTPUT)/root/usr/lib -lasound

export SYSUTILS_OUT_DIR=$(CURRENT_DIR)/out
export PKG_CONF_OPTS
export OBJ
export CC
export AR
export CFLAGS
export LD_FLAGS

all: $(PKG_TARGET)
	@$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(SYSUTILS_OUT_DIR))
	@echo "build $(PKG_NAME) done";

sysutils-build:
	@mkdir -p $(SYSUTILS_OUT_DIR)/bin
	@cp -rfa $(CURRENT_DIR)/include $(SYSUTILS_OUT_DIR)/
	@make -C $(CURRENT_DIR)/src;
	@make -C $(CURRENT_DIR)/examples;
	@make -C $(CURRENT_DIR)/examples install;

clean:
	@make -C $(CURRENT_DIR)/examples clean
	@make -C $(CURRENT_DIR)/src clean
	@rm -rf $(SYSUTILS_OUT_DIR)

distclean: clean
