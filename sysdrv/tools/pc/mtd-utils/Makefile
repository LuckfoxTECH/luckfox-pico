export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR ?= $(shell pwd)

all:
ifneq ($(SYSDRV_DIR_OUT_PC),)
	cp -f $(CURRENT_DIR)/mkfs.jffs2   $(SYSDRV_DIR_OUT_PC);
	cp -f $(CURRENT_DIR)/mkfs.ubifs   $(SYSDRV_DIR_OUT_PC);
	cp -f $(CURRENT_DIR)/ubinize   $(SYSDRV_DIR_OUT_PC);
	cp -f $(CURRENT_DIR)/mkfs_ubi.sh $(SYSDRV_DIR_OUT_PC);
	cp -f $(CURRENT_DIR)/mkfs_jffs2.sh $(SYSDRV_DIR_OUT_PC);
endif
