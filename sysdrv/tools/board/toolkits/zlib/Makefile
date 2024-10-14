
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash
CURRENT_DIR := $(shell pwd)

# arch=arm default use thumb-2 compile
ifeq ($(SYSDRV_ARCH_TYPE),arm)
PKG_USE_THUMB2 ?= YES
endif

PKG_TARBALL_ZLIB := zlib-1.2.11.tar.xz
PKG_NAME_ZLIB := zlib-1.2.11

PKG_BIN := out

PKG_CONF_OPTS += $(SYSDRV_CROSS_CFLAGS) $(SYSDRV_OPTS) -Os

ifeq ($(PKG_USE_THUMB2),YES)
PKG_CONF_OPTS += -mthumb -Wa,-mimplicit-it=thumb -mthumb-interwork
endif

all:
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/libz.a || (\
	rm -rf $(CURRENT_DIR)/$(PKG_BIN); \
	rm -fr $(CURRENT_DIR)/$(PKG_NAME_ZLIB)/; \
	tar -xf $(PKG_TARBALL_ZLIB); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME_ZLIB)/; \
		CC="$(SYSDRV_CROSS)-gcc" GCC="$(SYSDRV_CROSS)-gcc" \
		CXX="$(SYSDRV_CROSS)-g++" CPP="$(SYSDRV_CROSS)-cpp" \
		CFLAGS="$(PKG_CONF_OPTS)" CPPFLAGS="$(PKG_CONF_OPTS)" \
		CXXFLAGS="$(PKG_CONF_OPTS)" LDFLAGS="$(PKG_CONF_OPTS)" \
			./configure \
			--prefix=$(CURRENT_DIR)/$(PKG_BIN) ;\
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
		make install > /dev/null; \
	popd; )

clean: distclean

distclean:
	-rm -rf $(PKG_BIN) $(PKG_NAME_ZLIB)
