
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_NAME := rndis
PKG_SCRIPT = rndis.sh
PKG_BIN := out

all:
	@mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/bin
	@cp -fv $(CURRENT_DIR)/$(PKG_SCRIPT) $(CURRENT_DIR)/$(PKG_BIN)/usr/bin
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)

