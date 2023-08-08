ifeq ($(SYSDRV_PARAM), )
SYSDRV_PARAM:=../../../Makefile.param
include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := memtester-4.3.0.tar.gz
PKG_NAME := memtester-4.3.0
PKG_BIN := out
SED := sed -i -e

all:
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/bin/memtester || (\
	rm -rf $(CURRENT_DIR)/$(PKG_NAME); \
	rm -rf $(CURRENT_DIR)/$(PKG_BIN); \
	tar -xf $(PKG_TARBALL); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/bin; \
	pushd $(CURRENT_DIR)/$(PKG_NAME)/; \
		$(SED) "s%^cc%$(SYSDRV_CROSS)-gcc $(SYSDRV_OPTS)%" conf-cc; \
		$(SED) "s%^cc%$(SYSDRV_CROSS)-gcc $(SYSDRV_CROSS_CFLAGS)%" conf-ld; \
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		cp -fv memtester $(CURRENT_DIR)/$(PKG_BIN)/bin > /dev/null; \
	popd; )
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)
