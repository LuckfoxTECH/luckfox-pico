
tools_board-builds: \
		board-build-toolkits \
		board-build-gdb \
		board-build-eudev \
		board-build-rndis \
		board-build-adbd \
		board-build-rk_ota \
		board-build-rockchip_test \
		board-build-e2fsprogs \
		board-build-sysstat \
		board-build-mtd_utils
	@echo "build tools board done"

tools_board-clean:
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/e2fsprogs distclean
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/mtd-utils distclean
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/android-tools distclean
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/eudev distclean
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/toolkits distclean
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/gdb distclean
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/memtester distclean
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/stressapptest distclean
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/rk_ota distclean
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/sysstat distclean

board-build-toolkits:
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/toolkits

board-build-gdb:
ifeq ($(ENABLE_GDB),y)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/gdb;
endif

board-build-eudev:
ifeq ($(ENABLE_EUDEV),y)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/eudev;
endif

board-build-rndis:
ifeq ($(ENABLE_RNDIS),y)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/rndis;
endif

board-build-adbd:
ifeq ($(ENABLE_ADBD),y)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/android-tools
endif

board-build-rk_ota:
ifeq ($(ENABLE_OTA),y)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/rk_ota
endif

board-build-rockchip_test:
ifeq ($(ENABLE_ROCKCHIP_TEST),y)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/rockchip_test
endif

board-build-e2fsprogs:
ifeq ($(BOOT_MEDIUM),emmc)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/e2fsprogs
endif

board-build-mtd_utils:
ifneq ($(findstring $(BOOT_MEDIUM),spi_nand slc_nand spi_nor),)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/mtd-utils
ifeq ($(BOOT_MEDIUM),spi_nor)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/idb_bootconfig
	@pushd $(SYSDRV_DIR_OUT_ROOTFS)/usr/bin; \
		rm -f nanddump nandwrite \
			ubiattach ubiblock ubidetach ubiformat \
			ubimkvol ubirename ubirmvol ubirsvol; \
	popd;
endif
endif
board-build-sysstat:
ifeq ($(ENABLE_SYSSTAT),y)
	$(MAKE) -C $(SYSDRV_DIR)/tools/board/sysstat
endif
