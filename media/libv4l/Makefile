
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_TARBALL := v4l-utils-1.16.5.tar.bz2
PKG_NAME := v4l-utils-1.16.5
PKG_PATCHES_LIBV4L := patches
PKG_BIN ?= out
PKG_BIN_NEED_INSTALL_ROOT := $(RK_MEDIA_NEED_INSTALL_ROOT_DIR_NAME)
PKG_DEPEN_BUILD:=

pkg-build += pre-built

ifeq ($(RK_MEDIA_LIB_TYPE),uclibc)
PKG_NAME_ARGP:= argp-standalone-1.3
PKG_TARBALL_ARGP:= argp-standalone-1.3.tar.gz
PKG_PATCHES_ARGP:=patches_argp
PKG_DEPEN_BUILD:= \
		CPPFLAGS="-I$(CURRENT_DIR)/$(PKG_NAME_ARGP) -L$(CURRENT_DIR)/$(PKG_NAME_ARGP)" \
		LDFLAGS="$(RK_MEDIA_CROSS_CFLAGS) -L$(CURRENT_DIR)/$(PKG_NAME_ARGP) -largp"
pkg-build += build-argp
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

ifeq ($(CONFIG_LIBV4L),y)
PKG_TARGET := libv4l-build
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

libv4l-build: $(pkg-build)
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/lib/libv4l2.so.0.0.0 || (\
	rm -rf $(PKG_BIN); \
	tar -xf $(PKG_TARBALL); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME); \
		cp -af $(CURRENT_DIR)/$(PKG_PATCHES_LIBV4L)/* ./; \
		$(SHELL) ./libv4l.patch.sh; \
		autoreconf -f -i; \
		CFLAGS="$(RK_MEDIA_CROSS_CFLAGS) -fPIC" \
		$(PKG_DEPEN_BUILD) \
		ac_cv_lbl_unaligned_fail=yes \
		ac_cv_func_mmap_fixed_mapped=yes \
		ac_cv_func_memcmp_working=yes \
		ac_cv_have_decl_malloc=yes \
		gl_cv_func_malloc_0_nonnull=yes \
		ac_cv_func_malloc_0_nonnull=yes \
		ac_cv_func_calloc_0_nonnull=yes \
		ac_cv_func_realloc_0_nonnull=yes \
		lt_cv_sys_lib_search_path_spec="" \
		ac_cv_c_bigendian=no \
		ac_cv_prog_cc_c99='-std=gnu99' \
		PKG_CONFIG="" \
		ALSA_LIBS="$(RK_MEDIA_OUTPUT)/lib" \
		ALSA_CFLAGS="$(RK_MEDIA_OUTPUT)/include" \
		CFLAGS="$(RK_MEDIA_CROSS_CFLAGS)" \
		./configure --host=$(RK_MEDIA_CROSS) --target=$(RK_MEDIA_CROSS) \
			--prefix=$(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN) \
			--disable-gtk-doc --disable-gtk-doc-html --disable-doc --disable-docs \
			--disable-documentation \
			--with-xmlto=no --with-fop=no --disable-dependency-tracking \
			--disable-nls --disable-static --enable-shared \
			--disable-doxygen-doc --disable-qvidcap --without-jpeg \
			--disable-libdvbv5 \
			--enable-builtin-plugins \
			--exec-prefix=/usr \
			--localstatedir=/var \
			--datarootdir=/usr/share \
			--includedir=/usr/include \
			--sysconfdir=/etc \
			--libdir=/usr/lib \
			--datadir=/usr/share \
			--with-udevdir=/usr/lib/udev \
			--disable-bpf --disable-qv4l2 \
			--enable-v4l-utils ;\
		make -j$(RK_JOBS) || exit -1; \
		make install DESTDIR=$(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN) > /dev/null; \
		cp -fra $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN)/* \
			$(CURRENT_DIR)/$(PKG_BIN); \
		mv $(CURRENT_DIR)/$(PKG_BIN)/usr/include $(CURRENT_DIR)/$(PKG_BIN); \
		rm -rf $(CURRENT_DIR)/$(PKG_BIN)/usr/share $(CURRENT_DIR)/$(PKG_BIN)/usr/lib/udev $(CURRENT_DIR)/$(PKG_BIN)/etc; \
		mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/root; \
		mv $(CURRENT_DIR)/$(PKG_BIN)/{etc,usr} $(CURRENT_DIR)/$(PKG_BIN)/root; \
	popd; ) || exit -1;
	@rm -rf  \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/lib/pkgconfig \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/lib/libv4l/ov518-decomp \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/lib/libv4l/ov511-decomp \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/rds-ctl \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/v4l2-compliance \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/cec-compliance \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/v4l2-sysfs-path \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/cec-follower \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/ir-ctl \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/decode_tm6000 \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/cx18-ctl \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/ivtv-ctl \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/ir-keytable \
		$(PKG_BIN)/$(PKG_BIN_NEED_INSTALL_ROOT)/usr/bin/cec-ctl ;
	@find $(PKG_BIN) -name "*.la" -exec rm -rf {} \; ;
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))

pre-built: alsa_lib 
	@test ! -f $(CURRENT_DIR)/$(PKG_NAME)-$(RK_MEDIA_CROSS).tar.bz2 || (\
		rm -rf $(PKG_BIN); \
	echo "Uncompress pre-built $(PKG_NAME)"; \
	mkdir -p $(PKG_BIN); \
	tar xf $(CURRENT_DIR)/$(PKG_NAME)-$(RK_MEDIA_CROSS).tar.bz2 -C $(PKG_BIN); \
	exit 0; )

build-argp:
	@test -f $(CURRENT_DIR)/$(PKG_NAME)-$(RK_MEDIA_CROSS).tar.bz2 || (\
	rm -rf $(PKG_NAME_ARGP); \
	tar -xf $(PKG_TARBALL_ARGP); \
	pushd $(CURRENT_DIR)/$(PKG_NAME_ARGP); \
		cp -af $(CURRENT_DIR)/$(PKG_PATCHES_ARGP)/* ./; \
		$(SHELL) ./argp.patch.sh; \
		CFLAGS="$(RK_MEDIA_CROSS_CFLAGS) -fPIC -fgnu89-inline" \
		./configure --target=$(RK_MEDIA_CROSS) \
					--host=$(RK_MEDIA_CROSS) \
					--exec-prefix=/usr \
					--sysconfdir=/etc \
					--localstatedir=/var \
					--disable-gtk-doc --disable-gtk-doc-html --disable-doc \
					--disable-docs --disable-documentation \
					--with-xmlto=no --with-fop=no --disable-dependency-tracking ; \
		make -j$(RK_JOBS) || exit -1; \
	popd; ) || exit -1;

alsa_lib:
	@make -C $(RK_MEDIA_TOP_DIR)/alsa-lib

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN) $(PKG_NAME_ARGP)
