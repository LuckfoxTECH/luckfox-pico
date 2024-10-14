
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_TARBALL := alsa-lib-1.1.5.tar.bz2
PATCH_DIR := alsa-lib-patches
PKG_NAME := alsa-lib-1.1.5
PKG_BIN ?= out
PKG_BIN_NEED_INSTALL_ROOT := $(RK_MEDIA_NEED_INSTALL_ROOT_DIR_NAME)

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

ifeq ($(CONFIG_ALSA_LIB),y)
PKG_TARGET := alsa_lib-build
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

alsa_lib-build: pre-built
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/lib/libasound.so.2.0.0 || (\
	rm -fr $(PKG_NAME); \
	tar -xjf $(PKG_TARBALL); \
	pushd $(PKG_NAME)/; \
		cp -af $(CURRENT_DIR)/$(PATCH_DIR)/* ./; \
		$(SHELL) ./alsa-lib.patch.sh; \
		autoreconf -f -i; \
		CFLAGS="$(RK_MEDIA_CROSS_CFLAGS)" \
		./configure --target=$(RK_MEDIA_CROSS) \
					--host=$(RK_MEDIA_CROSS) \
					--prefix=$(CURRENT_DIR)/$(PKG_BIN)/ \
					--exec-prefix=/usr \
					--localstatedir=/var \
					--datarootdir=/usr/share \
					--includedir=/usr/include \
					--sysconfdir=/etc \
					--disable-gtk-doc  \
					--disable-gtk-doc-html \
					--disable-doc \
					--disable-docs \
					--disable-documentation \
					--with-xmlto=no \
					--with-fop=no \
					--disable-dependency-tracking \
					--enable-ipv6 \
					--disable-nls \
					--disable-static \
					--enable-shared  \
					--with-configdir=/usr/share/alsa \
					--with-plugindir=/usr/lib/alsa-lib \
					--with-pkgconfdir=/usr/lib/pkgconfig \
					--with-tmpdir=/tmp \
					--with-aload-devdir=/dev \
					--with-alsa-devdir=/dev/snd \
					--with-pcm-plugins="all" \
					--with-ctl-plugins="all" \
					--enable-static=no \
					--disable-python; \
		make -j$(RK_JOBS) || exit -1; \
		make install DESTDIR=$(CURRENT_DIR)/$(PKG_BIN) > /dev/null; \
		mv $(CURRENT_DIR)/$(PKG_BIN)/usr/include $(CURRENT_DIR)/$(PKG_BIN); \
		mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT); \
		mv $(CURRENT_DIR)/$(PKG_BIN)/usr $(CURRENT_DIR)/$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT); \
		rm -rf $(CURRENT_DIR)/$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/lib/pkgconfig; \
	popd; ) || exit -1;
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))

pre-built:
	@test ! -f $(CURRENT_DIR)/$(PKG_NAME)-$(RK_MEDIA_CROSS).tar.bz2 || (\
		rm -rf $(PKG_BIN); \
	echo "Uncompress pre-built alsa-lib"; \
	mkdir -p $(PKG_BIN); \
	tar xf $(CURRENT_DIR)/$(PKG_NAME)-$(RK_MEDIA_CROSS).tar.bz2 -C $(PKG_BIN); \
	exit 0; )

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)
