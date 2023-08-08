ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_BIN := out

PKG_CONF_OPTS += $(SYSDRV_CROSS_CFLAGS) -Os

ifeq ($(PKG_USE_THUMB2),YES)
PKG_CONF_OPTS += -mthumb -Wa,-mimplicit-it=thumb -mthumb-interwork
endif

all:
	rm -rf $(PKG_BIN)/io;
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/bin;
	$(SYSDRV_CROSS)-gcc $(PKG_CONF_OPTS) io.c -o $(PKG_BIN)/usr/bin/io
	$(SYSDRV_CROSS)-strip -s $(PKG_BIN)/usr/bin/io
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(CURRENT_DIR)/$(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(CURRENT_DIR)/$(PKG_BIN)
