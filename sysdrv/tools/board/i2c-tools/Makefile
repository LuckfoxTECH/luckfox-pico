
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := i2c-tools-4.3.tar.xz
PKG_NAME := i2c-tools-4.3
PKG_BIN := out
PKG_MAKE_OPTS := BUILD_DYNAMIC_LIB=0 USE_STATIC_LIB=1

all:
	@test -f $(PKG_BIN)/i2cdetect || (\
	rm -rf $(CURRENT_DIR)/$(PKG_NAME); \
	tar -xf $(PKG_TARBALL); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME); \
		make $(PKG_MAKE_OPTS) CC=$(SYSDRV_CROSS)-gcc -j$(SYSDRV_JOBS) || exit -1; \
		make $(PKG_MAKE_OPTS) \
			DESTDIR="$(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN)" \
			PREFIX=/usr -j$(SYSDRV_JOBS) \
			install > /dev/null || exit -1; \
		cp -vf $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN)/usr/sbin/* $(CURRENT_DIR)/$(PKG_BIN) ; \
	popd; )
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))
	@echo "make $(PKG_NAME) done"

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)
