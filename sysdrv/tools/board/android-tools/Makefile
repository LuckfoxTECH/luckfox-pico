
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash
# default build adbd with static link
PKG_ADBD_BUILD_STATIC ?= -static -pthread
# arch=arm default use thumb-2 compile
ifeq ($(SYSDRV_ARCH_TYPE),arm)
PKG_USE_THUMB2 ?= YES
endif

CURRENT_DIR := $(shell pwd)
PKG_TARBALL_ANDROID_TOOLS := android-tools_4.2.2+git20130218.orig.tar.xz
PKG_NAME_ANDROID_TOOLS:= android-tools
PKG_PATCHES_ANDROID_TOOLS := android-tools-patches

PKG_TARBALL_DEBIAN := android-tools_4.2.2+git20130218-3ubuntu41.debian.tar.gz

PKG_BIN := out
PKG_INSTALL_BIN_DIR := $(CURRENT_DIR)/$(PKG_BIN)/usr/bin

PKG_DEP_OPENSSL := $(CURRENT_DIR)/../toolkits/openssl
PKG_DEP_ZLIB := $(CURRENT_DIR)/../toolkits/zlib

PKG_CONF_OPTS += $(SYSDRV_OPTS) -Os \
				 -I $(PKG_DEP_OPENSSL)/out/include \
				 -L $(PKG_DEP_OPENSSL)/out/lib \
				 -I $(PKG_DEP_ZLIB)/out/include \
				 -L $(PKG_DEP_ZLIB)/out/lib
ifeq ($(PKG_USE_THUMB2),YES)
PKG_CONF_OPTS += -mthumb -Wa,-mimplicit-it=thumb -mthumb-interwork
endif

all: dep-openssl
	@test -f $(PKG_INSTALL_BIN_DIR)/adbd || (\
	rm -rf $(CURRENT_DIR)/$(PKG_NAME_ANDROID_TOOLS); \
	tar -xf $(PKG_TARBALL_ANDROID_TOOLS); \
	tar -xf $(PKG_TARBALL_DEBIAN) -C $(PKG_NAME_ANDROID_TOOLS); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME_ANDROID_TOOLS)/$(PKG_BIN); \
	mkdir -p $(PKG_INSTALL_BIN_DIR); \
	pushd $(CURRENT_DIR)/$(PKG_NAME_ANDROID_TOOLS)/; \
		cp -af $(CURRENT_DIR)/$(PKG_PATCHES_ANDROID_TOOLS)/* ./; \
		for item in `cat ./debian/patches/series`; do \
			patch -p1 < ./debian/patches/$$item; \
		done; \
		$(SHELL) ./android-tools.patch.sh; \
		CC="$(SYSDRV_CROSS)-gcc" GCC="$(SYSDRV_CROSS)-gcc" CXX="$(SYSDRV_CROSS)-g++" CPP="$(SYSDRV_CROSS)-cpp" \
		CFLAGS="$(PKG_CONF_OPTS)" CPPFLAGS="$(PKG_CONF_OPTS)" \
		CXXFLAGS="$(PKG_CONF_OPTS)" LDFLAGS="$(PKG_CONF_OPTS) $(PKG_ADBD_BUILD_STATIC)" \
		make -j$(SYSDRV_JOBS) -C $(CURRENT_DIR)/$(PKG_NAME_ANDROID_TOOLS)/$(PKG_BIN) \
			SRCDIR=$(CURRENT_DIR)/$(PKG_NAME_ANDROID_TOOLS) \
			-f $(CURRENT_DIR)/$(PKG_NAME_ANDROID_TOOLS)/debian/makefiles/adbd.mk 1>/dev/null || exit -1; \
			cp -f $(CURRENT_DIR)/$(PKG_NAME_ANDROID_TOOLS)/$(PKG_BIN)/adbd $(PKG_INSTALL_BIN_DIR)/ ; \
	popd; )
	$(SYSDRV_CROSS)-strip -s $(PKG_INSTALL_BIN_DIR)/adbd ;
	@mkdir -p $(PKG_BIN)/etc/init.d;
	@cp -fa $(CURRENT_DIR)/S50usbdevice $(PKG_BIN)/etc/init.d;
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(CURRENT_DIR)/$(PKG_BIN))

dep-openssl:
	$(MAKE) -C $(PKG_DEP_OPENSSL)

clean: distclean

distclean:
	-rm -rf $(PKG_NAME_ANDROID_TOOLS) $(PKG_BIN)
	-$(MAKE) clean -C $(PKG_DEP_OPENSSL)

