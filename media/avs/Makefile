
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := avs_calib
PKG_TARBALL += avs_lut
PKG_LIB_INSTALL_PATH := lib
PKG_BIN ?= out

ifeq ($(CONFIG_RK_AVS),y)
PKG_TARGET := avs-build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done";
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))

avs-build:
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/librkAVS_genLut.so  || (\
		rm -rf $(PKG_BIN) && mkdir -p $(PKG_BIN); \
		cp -rfa $(PKG_TARBALL) $(PKG_BIN)/; \
		cp -rfa lib/ $(PKG_BIN)/lib/; \
	);

clean: distclean

distclean:
	-rm -rf $(PKG_BIN)
