
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := dosfstools-4.2.tar.gz
PKG_NAME := dosfstools-4.2
PKG_BIN := out

all:
	@test -f $(PKG_BIN)/usr/sbin/fsck.fat || (\
	rm -rf $(CURRENT_DIR)/$(PKG_NAME); \
	tar -xzf $(PKG_TARBALL); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/; \
	pushd $(CURRENT_DIR)/$(PKG_NAME)/; \
		./configure --host=$(SYSDRV_CROSS) \
			--target=$(SYSDRV_CROSS) CFLAGS="$(SYSDRV_CROSS_CFLAGS)" \
			LDFLAGS="$(SYSDRV_CROSS_CFLAGS)" \
			--prefix=$(CURRENT_DIR)/$(PKG_BIN) \
			--disable-gtk-doc --disable-gtk-doc-html --disable-doc \
			--disable-docs --disable-documentation --with-xmlto=no \
			--with-fop=no --disable-dependency-tracking --enable-ipv6 \
			--disable-nls --disable-static --enable-shared \
			--enable-compat-symlinks --with-udev; \
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		make install > /dev/null; \
		rm -rf $(CURRENT_DIR)/$(PKG_BIN)/share ; \
		mv $(CURRENT_DIR)/$(PKG_BIN)/sbin $(CURRENT_DIR)/$(PKG_BIN)/usr ; \
	popd; )
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)
