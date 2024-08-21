
ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_TARBALL := fio-3.28.tar.xz
PKG_NAME := fio-3.28
PKG_BIN := out

SYSDRV_CROSS_CFLAGS += $(SYSDRV_OPTS) -Os

all:
	@test -f $(PKG_BIN)/usr/bin/fio || (\
	rm -rf $(CURRENT_DIR)/$(PKG_NAME); \
	tar -xf $(PKG_TARBALL); \
	mkdir -p $(CURRENT_DIR)/$(PKG_NAME)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN); \
	pushd $(CURRENT_DIR)/$(PKG_NAME)/; \
		make -j$(SYSDRV_JOBS) > /dev/null || exit -1; \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/bin ;\
	cp -vf $(CURRENT_DIR)/$(PKG_NAME)/fio $(CURRENT_DIR)/$(PKG_BIN)/usr/bin ; \
	popd; )
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))
	@echo "make $(PKG_NAME) done"

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)
