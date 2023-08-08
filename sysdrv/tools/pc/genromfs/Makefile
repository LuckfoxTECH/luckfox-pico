export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR ?= $(shell pwd)

all:
ifneq ($(SYSDRV_DIR_OUT_PC),)
	cp -f   $(CURRENT_DIR)/genromfs   	$(SYSDRV_DIR_OUT_PC);
	cp -f   $(CURRENT_DIR)/mkfs_romfs.sh 	$(SYSDRV_DIR_OUT_PC);
endif
