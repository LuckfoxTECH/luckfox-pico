ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_BIN := out

PKG_CONF_OPTS += $(SYSDRV_CROSS_CFLAGS) -Os

ifeq ($(PKG_USE_THUMB2),YES)
PKG_CONF_OPTS += -mthumb -Wa,-mimplicit-it=thumb -mthumb-interwork
endif

all: memtester-build stressapptest-build
	@test -d $(CURRENT_DIR)/$(PKG_BIN)/rockchip_test || (\
		rm -rf $(PKG_BIN)/rockchip_test; \
		mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/etc/init.d/; \
		cp -rfa $(CURRENT_DIR)/rockchip_test $(PKG_BIN); \
		cp -fa $(CURRENT_DIR)/rockchip_test/auto_reboot/S99_auto_reboot $(CURRENT_DIR)/$(PKG_BIN)/etc/init.d/; \
	);
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(CURRENT_DIR)/$(PKG_BIN))

memtester-build:
	$(MAKE) -C $(CURRENT_DIR)/../memtester;

stressapptest-build:
	$(MAKE) -C $(CURRENT_DIR)/../stressapptest;

clean: distclean

distclean:
	-rm -rf $(CURRENT_DIR)/$(PKG_BIN)
