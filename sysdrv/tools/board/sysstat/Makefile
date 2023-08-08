
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := sysstat-12.6.1.tar.xz
PKG_NAME := sysstat-12.6.1
PKG_BIN := out

SYSDRV_CROSS_CFLAGS += $(SYSDRV_OPTS) -Os

all:
	@test -f $(PKG_BIN)/usr/bin/iostat -a -f $(PKG_BIN)/usr/bin/pidstat || (\
	rm -rf $(CURRENT_DIR)/$(PKG_NAME); \
	tar -xf $(PKG_TARBALL); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME)/; \
		sa_dir=/tmp/sa \
		./configure --host=$(SYSDRV_CROSS) --target=$(SYSDRV_CROSS) CFLAGS="$(SYSDRV_CROSS_CFLAGS)" \
			LDFLAGS="$(SYSDRV_CROSS_CFLAGS)" --prefix=$(CURRENT_DIR)/$(PKG_BIN) \
		--disable-file-attr --disable-sensors \
		--exec-prefix=/usr --sysconfdir=/etc \
		--localstatedir=/var runstatedir=/run \
		sa_lib_dir="/usr/lib/sa" \
		--disable-nls ;\
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		make install DESTDIR=$(CURRENT_DIR)/$(PKG_BIN) PREFIX="/usr" > /dev/null || exit -1; \
		rm -rf $(CURRENT_DIR)/$(PKG_BIN)/tmp $(CURRENT_DIR)/$(PKG_BIN)/usr/share; \
	popd; )
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(CURRENT_DIR)/$(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)
