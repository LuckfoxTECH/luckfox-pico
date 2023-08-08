
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := libdrm-2.4.89-$(RK_MEDIA_CROSS).tar
PKG_NAME := libdrm-2.4.89
PKG_BIN ?= out

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

ifeq ($(CONFIG_LIBDRM),y)
PKG_TARGET := libdrm-build
else ifeq ($(RK_ENABLE_LIBDRM),y)
PKG_TARGET := libdrm-build
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

libdrm-build:
	@rm -rf $(PKG_BIN) && mkdir -p $(PKG_BIN);
	tar -xf $(PKG_TARBALL) -C $(PKG_BIN)/;
	@rm -rf $(PKG_BIN)/share;
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(CURRENT_DIR)/$(PKG_NAME)
	-rm -rf $(CURRENT_DIR)/$(PKG_BIN)

