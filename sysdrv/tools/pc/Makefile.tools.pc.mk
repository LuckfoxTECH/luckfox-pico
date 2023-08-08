
tools_pc-builds: \
		pc-build-genromfs \
		pc-build-uboot_tools \
		pc-build-mksquashfs \
		pc-build-erofs_utils \
		pc-build-cramfs_tool \
		pc-build-initramfs \
		pc-build-toolkits \
		pc-build-e2fsprogs \
		pc-build-mtd_utils
	@echo "build tools pc done"

pc-build-genromfs:
	$(MAKE) -C $(SYSDRV_DIR)/tools/pc/genromfs

pc-build-uboot_tools:
	$(MAKE) -C $(SYSDRV_DIR)/tools/pc/uboot_tools

pc-build-mksquashfs:
	$(MAKE) -C $(SYSDRV_DIR)/tools/pc/mksquashfs

pc-build-erofs_utils:
	$(MAKE) -C $(SYSDRV_DIR)/tools/pc/erofs-utils

pc-build-cramfs_tool:
	$(MAKE) -C $(SYSDRV_DIR)/tools/pc/cramfs_tool

pc-build-initramfs:
	$(MAKE) -C $(SYSDRV_DIR)/tools/pc/initramfs

pc-build-toolkits:
	$(MAKE) -C $(SYSDRV_DIR)/tools/pc/toolkits

pc-build-e2fsprogs:
	$(MAKE) -C $(SYSDRV_DIR)/tools/pc/e2fsprogs

pc-build-mtd_utils:
	$(MAKE) -C $(SYSDRV_DIR)/tools/pc/mtd-utils
