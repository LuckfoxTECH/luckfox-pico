
export LC_ALL=C
SHELL:=/bin/bash
CURRENT_DIR := $(shell pwd)

ifneq ($(RK_TOOLCHAIN_CROSS),)
META_CROSS ?= $(RK_TOOLCHAIN_CROSS)
else
META_CROSS ?= arm-rockchip830-linux-uclibcgnueabihf
endif
$(info META_CROSS is [$(META_CROSS)])
ifeq ($(META_CROSS),arm-rockchip830-linux-uclibcgnueabihf)
PKG_USE_THUMB2=YES
endif

PKG_TARBALL_ZLIB := zlib-1.2.11.tar.xz
PKG_NAME_ZLIB := zlib-1.2.11

PKG_BIN := out_board
PKG_HOST_PC_BIN := out_host_pc

PKG_CONF_OPTS += -Os

ifeq ($(PKG_USE_THUMB2),YES)
PKG_CONF_OPTS += -mthumb -Wa,-mimplicit-it=thumb -mthumb-interwork
endif

all: host-pc-lib-build
	@test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/libz.a || (\
	rm -rf $(CURRENT_DIR)/$(PKG_BIN); \
	rm -fr $(CURRENT_DIR)/$(PKG_NAME_ZLIB)/; \
	tar -xf $(PKG_TARBALL_ZLIB); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME_ZLIB)/; \
		CC="$(META_CROSS)-gcc" GCC="$(META_CROSS)-gcc" \
		CXX="$(META_CROSS)-g++" CPP="$(META_CROSS)-cpp" \
		CFLAGS="$(PKG_CONF_OPTS)" CPPFLAGS="$(PKG_CONF_OPTS)" \
		CXXFLAGS="$(PKG_CONF_OPTS)" LDFLAGS="$(PKG_CONF_OPTS)" \
			./configure \
			--prefix=$(CURRENT_DIR)/$(PKG_BIN) ;\
		make -j4 > /dev/null || exit -1; \
		make install > /dev/null; \
	popd; )

host-pc-lib-build:
	@test -f $(CURRENT_DIR)/$(PKG_HOST_PC_BIN)/lib/libz.a || (\
	rm -rf $(CURRENT_DIR)/$(PKG_HOST_PC_BIN); \
	rm -fr $(CURRENT_DIR)/$(PKG_NAME_ZLIB)/; \
	tar -xf $(PKG_TARBALL_ZLIB); \
	mkdir -p $(CURRENT_DIR)/$(PKG_HOST_PC_BIN)/bin; \
	pushd $(CURRENT_DIR)/$(PKG_NAME_ZLIB)/; \
			./configure \
			--prefix=$(CURRENT_DIR)/$(PKG_HOST_PC_BIN) ;\
		make -j4 > /dev/null || exit -1; \
		make install > /dev/null; \
		cp -f minigzip $(CURRENT_DIR)/$(PKG_HOST_PC_BIN)/bin; \
	popd; )

clean: distclean

distclean:
	-rm -rf $(PKG_BIN) $(PKG_NAME_ZLIB) $(PKG_HOST_PC_BIN)
