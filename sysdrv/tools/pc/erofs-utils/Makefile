export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR ?= $(shell pwd)
PKG_SRC := erofs-utils
PKG_OUT := out
PKG_DEP_LZ4_INC_LIB := $(CURRENT_DIR)/../toolkits/lz4-src/lz4-1.9.3/lib/

all: pkg-build
ifneq ($(SYSDRV_DIR_OUT_PC),)
	cp -f   $(CURRENT_DIR)/mkfs.erofs   $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/mkfs_erofs.sh     $(SYSDRV_DIR_OUT_PC);
endif

pkg-build:
	@test ! -d $(CURRENT_DIR)/$(PKG_SRC) || (\
		rm -rf $(CURRENT_DIR)/$(PKG_OUT); \
		mkdir -p $(CURRENT_DIR)/$(PKG_OUT); \
		make -C $(CURRENT_DIR)/../toolkits/lz4-src/; \
		cd $(CURRENT_DIR)/$(PKG_SRC) ; \
		./autogen.sh; \
		CFLAGS="-O3 -I$(PKG_DEP_LZ4_INC_LIB)" \
		LDFLAGS="-L$(PKG_DEP_LZ4_INC_LIB) --static -Wl,-rpath,$(PKG_DEP_LZ4_INC_LIB)" \
		./configure --prefix=$(CURRENT_DIR)/$(PKG_OUT) \
			--disable-shared --enable-static --enable-lz4 \
			--disable-fuse --without-selinux --without-uuid; \
		make -j9 > /dev/null || exit -1; \
		make install-strip > /dev/null; \
	);

