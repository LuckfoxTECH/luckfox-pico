export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR ?= $(shell pwd)
PKG_TARBALL := lz4-v1.9.3.tar.gz
PKG_NAME := lz4-1.9.3

all:
	rm -rf $(PKG_NAME); \
	tar -xf $(PKG_TARBALL); \
	pushd $(CURRENT_DIR)/$(PKG_NAME)/; \
		patch -p1 < $(CURRENT_DIR)/0001-Fix-potential-memory-corruption-with-negative-memmov.patch; \
		make -j$(SYSDRV_JOBS);\
	popd;

clean: distclean

distclean:
	-rm -rf $(PKG_NAME)
