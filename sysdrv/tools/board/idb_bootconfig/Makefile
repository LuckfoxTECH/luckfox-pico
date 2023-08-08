ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
	include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_BIN := out

PKG_CONF_OPTS += $(SYSDRV_CROSS_CFLAGS) -Os

ifeq ($(SYSDRV_CROSS),arm-rockchip830-linux-uclibcgnueabihf)
PKG_CONF_OPTS += -DRV1106_RV1103
PKG_TARGET := idb-bootconfig-build
else
$(warning idb_bootconfig NOT SUPPORT this platform, ignore)
PKG_TARGET :=
endif

ifneq ($(SYSDRV_CROSS),)
PKG_CC:=$(SYSDRV_CROSS)-gcc
else
PKG_CC:=$(CC)
endif

ifeq ($(PKG_USE_THUMB2),YES)
PKG_CONF_OPTS += -mthumb -Wa,-mimplicit-it=thumb -mthumb-interwork
endif

all: $(PKG_TARGET)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(CURRENT_DIR)/$(PKG_BIN))

idb-bootconfig-build:
	@rm -rf $(CURRENT_DIR)/$(PKG_BIN);
	@mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/bin $(CURRENT_DIR)/$(PKG_BIN)/etc/init.d;
	@cp -af $(CURRENT_DIR)/S80idb_bootconfig $(CURRENT_DIR)/$(PKG_BIN)/etc/init.d;
	$(PKG_CC) $(PKG_CONF_OPTS) idb_bootconfig.c -o $(CURRENT_DIR)/$(PKG_BIN)/bin/idb_bootconfig

clean: distclean

distclean:
	-rm -rf $(CURRENT_DIR)/$(PKG_BIN)
