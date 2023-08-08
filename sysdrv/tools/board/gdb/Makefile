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
PKG_TARBALL_GDB := gdb-10.2.tar.xz
PKG_NAME_GDB := gdb-10.2

PKG_TARBALL_EXPAT := expat-2.4.1.tar.xz
PKG_NAME_EXPAT := expat-2.4.1

PKG_BIN := out

PKG_CONF_OPTS += $(SYSDRV_OPTS) -Os

ifeq ($(PKG_USE_THUMB2),YES)
PKG_CONF_OPTS += -mthumb -Wa,-mimplicit-it=thumb -mthumb-interwork
endif

all: expat
	@test -f $(PKG_BIN)/bin/gdb -a -f $(PKG_BIN)/bin/gdbserver || (\
	rm -rf $(PKG_NAME_GDB); \
	tar -xf $(PKG_TARBALL_GDB); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME_GDB)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME_GDB)/; \
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
		ac_cv_type_uintptr_t=yes \
		gt_cv_func_gettext_libintl=yes \
		ac_cv_func_dcgettext=yes \
		gdb_cv_func_sigsetjmp=yes \
		bash_cv_func_strcoll_broken=no \
		bash_cv_must_reinstall_sighandlers=no \
		bash_cv_func_sigsetjmp=present \
		bash_cv_have_mbstate_t=yes \
		gdb_cv_func_sigsetjmp=yes \
		gl_cv_func_gettimeofday_clobber=no \
		gl_cv_func_working_strerror=yes \
		gl_cv_func_strerror_0_works=yes \
		gdb_cv_prfpregset_t_broken=no \
		CFLAGS="$(PKG_CONF_OPTS)" CPPFLAGS="$(PKG_CONF_OPTS)" \
		CXXFLAGS="$(PKG_CONF_OPTS)" LDFLAGS="$(PKG_CONF_OPTS)" \
		./configure \
			--host=$(SYSDRV_CROSS) \
			--target=$(SYSDRV_CROSS) \
			--prefix=$(CURRENT_DIR)/$(PKG_NAME_GDB)/$(PKG_BIN) \
			--disable-gtk-doc  --disable-gtk-doc-html  --disable-doc  --disable-docs  \
			--disable-documentation  --with-xmlto=no  --with-fop=no  --disable-dependency-tracking  \
			--enable-ipv6  --disable-nls  --disable-static  --enable-shared  --without-uiout  \
			--disable-gdbtk  --without-x  --disable-sim  --disable-binutils  --disable-install-libbfd  \
			--disable-ld  --disable-gas  --enable-gdbserver  --with-curses  --without-included-gettext  \
			--disable-werror  --enable-static  --without-mpfr  --disable-tui  --without-python  \
			--without-lzma \
			--with-expat \
			--with-libexpat-prefix=$(CURRENT_DIR)/$(PKG_NAME_EXPAT)/$(PKG_BIN) \
			--with-zlib ;\
		make -j$(SYSDRV_JOBS) >/dev/null || exit -1; \
		make install > /dev/null; \
		test -d $(CURRENT_DIR)/$(PKG_NAME_GDB)/$(PKG_BIN)/bin && \
			cp -arf $(CURRENT_DIR)/$(PKG_NAME_GDB)/$(PKG_BIN)/bin $(CURRENT_DIR)/$(PKG_BIN); \
		test -d $(CURRENT_DIR)/$(PKG_NAME_GDB)/$(PKG_BIN)/lib && \
			cp -arf $(CURRENT_DIR)/$(PKG_NAME_GDB)/$(PKG_BIN)/lib $(CURRENT_DIR)/$(PKG_BIN); \
	popd; )
	$(SYSDRV_CROSS)-strip -s $(CURRENT_DIR)/$(PKG_BIN)/bin/{gdb,gdbserver};
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))

expat:
	@test -f $(CURRENT_DIR)/$(PKG_NAME_EXPAT)/$(PKG_BIN)/lib/libexpat.a || (\
	rm -fr $(PKG_NAME_EXPAT); \
	tar -xf $(PKG_TARBALL_EXPAT); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME_EXPAT)/; \
		CFLAGS="$(PKG_CONF_OPTS)" CPPFLAGS="$(PKG_CONF_OPTS)" \
		CXXFLAGS="$(PKG_CONF_OPTS)" LDFLAGS="$(PKG_CONF_OPTS)" \
		./configure \
			--host=$(SYSDRV_CROSS) \
			--target=$(SYSDRV_CROSS) \
			--prefix=$(CURRENT_DIR)/$(PKG_NAME_EXPAT)/$(PKG_BIN) \
			--disable-gtk-doc \
			--disable-gtk-doc-html \
			--disable-doc \
			--disable-docs \
			--disable-documentation \
			--without-docbook \
			--with-xmlto=no \
			--with-fop=no \
			--disable-dependency-tracking \
			--enable-ipv6 \
			--disable-nls \
			--enable-static \
			--disable-shared; \
		make -j$(SYSDRV_JOBS) >/dev/null || exit -1; \
		make install > /dev/null; \
	popd; )

clean: distclean

distclean:
	-rm -rf $(PKG_NAME_GDB) \
		$(PKG_BIN) $(PKG_NAME_EXPAT)
