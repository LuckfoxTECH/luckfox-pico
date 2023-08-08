
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash
# arch=arm default use thumb-2 compile
ifeq ($(SYSDRV_ARCH_TYPE),arm)
PKG_USE_THUMB2 ?= YES
endif

CURRENT_DIR := $(shell pwd)

ifeq ($(shell pkg-config --version),)
$(info ***************************)
$(info Please Install pkg-config tool)
$(info sudo apt-get install pkg-config)
$(error ***************************)
endif

PKG_TARBALL_EUDEV := eudev-3.2.7.tar.gz
PKG_NAME_EUDEV := eudev-3.2.7
PKG_PATCHES_EUDEV := eudev-patches

PKG_TARBALL_KMOD := kmod-29.tar.xz
PKG_NAME_KMOD := kmod-29
PKG_PATCHES_KMOD := kmod-26-patches

PKG_TARBALL_UTIL_LINUX := util-linux-2.37.2.tar.xz
PKG_NAME_UTIL_LINUX := util-linux-2.37.2
PKG_PATCHES_UTIL_LINUX := util-linux-2.37.2-patches

PKG_BIN := out

PKG_CONF_OPTS += $(SYSDRV_OPTS) -Os -I$(CURRENT_DIR)/$(PKG_BIN)/include -L$(CURRENT_DIR)/$(PKG_BIN)/lib

ifeq ($(PKG_USE_THUMB2),YES)
PKG_CONF_OPTS += -mthumb -Wa,-mimplicit-it=thumb -mthumb-interwork
endif

all: kmod util-linux
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/sbin/udevd || (\
	rm -rf $(PKG_NAME_EUDEV); \
	tar -xf $(PKG_TARBALL_EUDEV); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME_EUDEV)/; \
		cp -af $(CURRENT_DIR)/$(PKG_PATCHES_EUDEV)/* ./; \
		$(SHELL) ./eudev.patch.sh; \
		PKG_CONFIG_PATH="$(CURRENT_DIR)/$(PKG_BIN)/lib/pkgconfig" \
		CFLAGS="$(PKG_CONF_OPTS)" CPPFLAGS="$(PKG_CONF_OPTS)" \
		./configure \
		--target=$(SYSDRV_CROSS) \
		--host=$(SYSDRV_CROSS) \
		--disable-gtk-doc --disable-gtk-doc-html --disable-doc --disable-docs --disable-documentation \
		--with-xmlto=no --with-fop=no --disable-dependency-tracking --enable-ipv6 --disable-nls \
		--disable-manpages --disable-introspection --disable-rule-generator \
		--enable-kmod \
		--enable-blkid \
		--enable-split-usr \
		--disable-hwdb \
		--disable-selinux \
		--disable-static \
		--enable-shared \
		--prefix=$(CURRENT_DIR)/$(PKG_BIN) ;\
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		make install > /dev/null; \
		rm -f $(CURRENT_DIR)/$(PKG_BIN)/lib/libudev.so* ;\
	popd; )
	@pushd $(CURRENT_DIR)/$(PKG_BIN)/; \
		find . -name "lib*.la" -exec rm -f {} \; ;\
		rm -rf share include lib/pkgconfig;\
		rm -f sbin/udevadm kmod;\
		rm -f lib/udev/ata_id ;\
		rm -f lib/udev/scsi_id ;\
		rm -f lib/udev/mtd_probe ;\
		rm -f lib/udev/v4l_id ;\
		rm -f lib/udev/collect ;\
		rm -f lib/udev/cdrom_id ;\
	popd;
	@cp -f $(CURRENT_DIR)/rules/* $(CURRENT_DIR)/$(PKG_BIN)/lib/udev/rules.d/ ;
	@mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/etc/init.d ;
	@cp -f S10udev $(CURRENT_DIR)/$(PKG_BIN)/etc/init.d ;
	@mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/bin ;
	@cp -f usbdevice $(CURRENT_DIR)/$(PKG_BIN)/usr/bin ;
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(CURRENT_DIR)/$(PKG_BIN))

kmod:
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/libkmod.so.2.3.7 || (\
	rm -fr $(PKG_NAME_KMOD); \
	tar -xf $(PKG_TARBALL_KMOD); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME_KMOD)/; \
		CFLAGS="$(PKG_CONF_OPTS)" CPPFLAGS="$(PKG_CONF_OPTS)" \
		./configure \
		--target=$(SYSDRV_CROSS) \
		--host=$(SYSDRV_CROSS) \
		--disable-static --enable-shared --disable-manpages \
		--disable-tools --disable-test-modules \
		--with-bashcompletiondir=$(CURRENT_DIR)/$(PKG_BIN) \
		--prefix=$(CURRENT_DIR)/$(PKG_BIN) ;\
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		make install > /dev/null; \
	popd; )

util-linux:
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/libblkid.so.1.1.0 || (\
	rm -fr $(PKG_NAME_UTIL_LINUX); \
	tar -xf $(PKG_TARBALL_UTIL_LINUX); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME_UTIL_LINUX)/; \
		cp -af $(CURRENT_DIR)/$(PKG_PATCHES_UTIL_LINUX)/* ./; \
		$(SHELL) ./util-linux.patch.sh; \
		scanf_cv_type_modifier=no \
		CFLAGS="$(PKG_CONF_OPTS)" CPPFLAGS="$(PKG_CONF_OPTS)" \
		./configure \
		--target=$(SYSDRV_CROSS) \
		--host=$(SYSDRV_CROSS) \
		--disable-gtk-doc --disable-gtk-doc-html --disable-doc --disable-docs --disable-documentation \
		--with-xmlto=no --with-fop=no --disable-dependency-tracking --enable-ipv6 --disable-nls \
		--disable-rpath --disable-makeinstall-chown --without-systemd --with-systemdsystemunitdir=no \
		--without-ncursesw --without-ncurses --disable-widechar --without-selinux \
		--disable-all-programs --disable-agetty --disable-bfs --disable-cal \
		--disable-chfn-chsh --disable-chmem --disable-cramfs --disable-eject \
		--disable-fallocate --disable-fdformat --disable-fsck --disable-hwclock \
		--disable-ipcrm --disable-ipcs --disable-kill --disable-last --disable-write \
		--disable-libfdisk --disable-libmount --disable-libsmartcols \
		--disable-line --disable-logger --disable-login --disable-losetup \
		--disable-lslogins --disable-lsmem --disable-mesg --disable-minix \
		--disable-more --disable-mount --disable-mountpoint --disable-newgrp \
		--disable-nologin --disable-nsenter --disable-partx --disable-pg \
		--disable-pivot_root --disable-raw --disable-rename --disable-rfkill \
		--disable-runuser --disable-schedutils --disable-setpriv \
		--disable-setterm --disable-su --disable-sulogin --disable-switch_root \
		--disable-tunelp --disable-ul --disable-unshare --disable-utmpdump \
		--disable-uuidd --disable-vipw --disable-wall --disable-wdctl \
		--disable-zramctl --without-python --without-readline --without-audit \
		--disable-static --enable-shared \
		--disable-libuuid --enable-libblkid \
		--with-bashcompletiondir=$(CURRENT_DIR)/$(PKG_BIN) \
		--prefix=$(CURRENT_DIR)/$(PKG_BIN) ;\
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		make install > /dev/null; \
	popd; )

clean: distclean

distclean:
	-rm -rf $(PKG_NAME_EUDEV) $(PKG_BIN) $(PKG_NAME_KMOD) $(PKG_NAME_UTIL_LINUX)

