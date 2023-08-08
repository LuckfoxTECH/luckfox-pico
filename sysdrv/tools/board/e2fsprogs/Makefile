
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := e2fsprogs-1.43.9.tar.xz
PKG_NAME := e2fsprogs-1.43.9
PKG_BIN := out

all:
	@test -f $(PKG_BIN)/usr/sbin/e2fsck || (\
	rm -rf $(CURRENT_DIR)/$(PKG_NAME); \
	tar -xf $(PKG_TARBALL); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/; \
	pushd $(CURRENT_DIR)/$(PKG_NAME)/; \
		./configure --host=$(SYSDRV_CROSS) --target=$(SYSDRV_CROSS) CFLAGS="$(SYSDRV_CROSS_CFLAGS)" \
			LDFLAGS="$(SYSDRV_CROSS_CFLAGS)" --prefix=$(CURRENT_DIR)/$(PKG_BIN) \
			--disable-nls \
			--disable-debugfs --disable-imager --disable-defrag --enable-fsck --enable-resizer \
			--disable-testio-debug --disable-rpath --enable-symlink-install --disable-fuse2fs; \
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		make install > /dev/null; \
		rm -rf $(CURRENT_DIR)/$(PKG_BIN)/share ; \
		rm -rf $(CURRENT_DIR)/$(PKG_BIN)/lib ; \
		mv $(CURRENT_DIR)/$(PKG_BIN)/bin $(CURRENT_DIR)/$(PKG_BIN)/sbin $(CURRENT_DIR)/$(PKG_BIN)/usr ; \
	popd; )
	@sed -i 's/huge_file,//g' $(CURRENT_DIR)/$(PKG_BIN)/etc/mke2fs.conf;
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)

