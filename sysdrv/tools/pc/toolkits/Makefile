export LC_ALL=C
SHELL:=/bin/bash

TOOLKITS_CURRENT_DIR ?= $(shell pwd)

all:
	@test -f $(KERNEL_DIR)/scripts/resource_tool && \
		cp $(KERNEL_DIR)/scripts/resource_tool $(TOOLKITS_CURRENT_DIR) || \
		gcc -Wall -Wmissing-prototypes -Wstrict-prototypes \
		-O2 -fomit-frame-pointer -std=gnu89 \
		$(TOOLKITS_CURRENT_DIR)/resource_tool.c \
		-o $(TOOLKITS_CURRENT_DIR)/resource_tool
ifneq ($(SYSDRV_DIR_OUT_PC),)
	cp -f $(TOOLKITS_CURRENT_DIR)/get_part_info.sh $(SYSDRV_DIR_OUT_PC);
	cp -f $(TOOLKITS_CURRENT_DIR)/repack_bootimg.sh $(SYSDRV_DIR_OUT_PC);
	cp -f $(TOOLKITS_CURRENT_DIR)/update_dtb_bootargs.sh $(SYSDRV_DIR_OUT_PC);
	cp -f $(TOOLKITS_CURRENT_DIR)/lz4 $(SYSDRV_DIR_OUT_PC);
	cp -f $(TOOLKITS_CURRENT_DIR)/resource_tool $(SYSDRV_DIR_OUT_PC);
endif

clean:
	@echo "clean toolkis done"
