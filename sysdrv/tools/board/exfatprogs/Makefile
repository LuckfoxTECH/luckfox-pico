
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := exfatprogs-v1.2.1.tar.bz2
PKG_NAME := exfatprogs-v1.2.1
PKG_BIN := out

all:
	@test -f $(PKG_BIN)/usr/sbin/fsck.exfat || (\
	rm -rf $(CURRENT_DIR)/$(PKG_NAME); \
	tar -xf $(PKG_TARBALL); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/; \
	pushd $(CURRENT_DIR)/$(PKG_NAME)/; \
		./autogen.sh; \
		./configure --host=$(SYSDRV_CROSS) --target=$(SYSDRV_CROSS) CFLAGS="$(SYSDRV_CROSS_CFLAGS)" \
			LDFLAGS="$(SYSDRV_CROSS_CFLAGS)" --prefix=$(CURRENT_DIR)/$(PKG_BIN); \
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		make install > /dev/null; \
		rm -rf $(CURRENT_DIR)/$(PKG_BIN)/share ; \
		rm -rf $(CURRENT_DIR)/$(PKG_BIN)/lib ; \
		mv $(CURRENT_DIR)/$(PKG_BIN)/sbin $(CURRENT_DIR)/$(PKG_BIN)/usr ; \
	popd; )
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)

