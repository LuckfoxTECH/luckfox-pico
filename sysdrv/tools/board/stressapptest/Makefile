ifeq ($(SYSDRV_PARAM), )
SYSDRV_PARAM:=../../../Makefile.param
include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := stressapptest-master.tar.gz
PKG_NAME := stressapptest-master
PKG_BIN := out

all:
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/bin/stressapptest || (\
	rm -rf $(CURRENT_DIR)/$(PKG_NAME); \
	rm -rf $(CURRENT_DIR)/$(PKG_BIN); \
	tar -xf $(PKG_TARBALL); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME)/; \
		sed -i "s/\*armv7a\*) :/ arm \| &/" ./configure; \
		./configure --host=$(SYSDRV_CROSS) \
		--target=$(SYSDRV_CROSS) CFLAGS="$(SYSDRV_CROSS_CFLAGS)" \
		LDFLAGS="$(SYSDRV_CROSS_CFLAGS)" \
		--prefix=$(CURRENT_DIR)/$(PKG_BIN); \
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		make install > /dev/null; \
		rm -rf $(CURRENT_DIR)/$(PKG_BIN)/share; \
	popd; )
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)
