export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR ?= $(shell pwd)

all:
ifneq ($(SYSDRV_DIR_OUT_PC),)
	cp -f   $(CURRENT_DIR)/mkfs.ext2   $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/mkfs.ext3   $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/mkfs.ext4   $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/mke2fs      $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/fsck.ext2   $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/fsck.ext3   $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/fsck.ext4   $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/e2fsck      $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/resize2fs   $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/tune2fs     $(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/mkfs_ext4.sh     $(SYSDRV_DIR_OUT_PC);
endif
