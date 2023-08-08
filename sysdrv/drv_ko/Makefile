ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
ifeq ($(RK_PROJECT_OUTPUT),)
-include $(CURRENT_DIR)/../../.BoardConfig.mk
endif
PKG_BIN := out

M_OUT_DIR := $(CURRENT_DIR)/$(PKG_BIN)
export M_OUT_DIR
M_DIRS := rockit kmpp wifi motor
################################################################################
##  build target
################################################################################
all:
	@mkdir -p $(CURRENT_DIR)/$(PKG_BIN);
	@$(foreach target,$(M_DIRS), test ! -f $(target)/Makefile || make -C $(target) || exit $$?;)
	@cp insmod_ko.sh $(CURRENT_DIR)/$(PKG_BIN);
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(KERNEL_DRV_KO_INSTALL_PATH), $(CURRENT_DIR)/$(PKG_BIN))

distclean: clean

clean:
	@$(foreach target,$(M_DIRS), test ! -f $(target)/Makefile || make -C $(target) clean;)
	@rm -rf $(PKG_BIN)
