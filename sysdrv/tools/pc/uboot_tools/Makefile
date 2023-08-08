export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR ?= $(shell pwd)

all:
ifneq ($(SYSDRV_DIR_OUT_PC),)
	cp -f $(CURRENT_DIR)/mkimage $(SYSDRV_DIR_OUT_PC);
	cp -f $(CURRENT_DIR)/mkenvimage $(SYSDRV_DIR_OUT_PC);
endif
