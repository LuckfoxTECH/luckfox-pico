export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR ?= $(shell pwd)

all:
ifneq ($(SYSDRV_DIR_OUT_PC),)
	cp -f $(CURRENT_DIR)/mksquashfs   $(SYSDRV_DIR_OUT_PC);
	cp -f $(CURRENT_DIR)/unsquashfs   $(SYSDRV_DIR_OUT_PC);
	cp -f $(CURRENT_DIR)/mkfs_squashfs.sh $(SYSDRV_DIR_OUT_PC);
endif
