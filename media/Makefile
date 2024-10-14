
MAKEFILE_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
export LC_ALL=C
SHELL:=/bin/bash
MAKE=make
AT?=@

################################################################################
##   Input Configuraton Overlay
################################################################################
ifneq ($(RK_TOOLCHAIN_CROSS),)
RK_MEDIA_CROSS:= $(RK_TOOLCHAIN_CROSS)
endif

################################################################################
##   Public Configuraton
################################################################################
include $(MAKEFILE_DIR)/Makefile.param

media_src := $(wildcard ./*/Makefile)
media_src := $(dir $(media_src))
media_src := $(filter-out ./samples/,$(media_src))
################################################################################
##  build target
################################################################################
all: media_libs
	make -C ./samples
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_PROJECT_PATH_MEDIA), $(RK_MEDIA_OUTPUT))

media_libs:
	@rm -rf $(RK_MEDIA_OUTPUT)
	$(foreach target,$(media_src),make -C $(target)||exit -1;)

distclean: clean

clean:
	$(foreach target,$(media_src),make distclean -C $(target)||exit -1;)
	make -C ./samples distclean
	@rm -rf $(RK_MEDIA_OUTPUT)

info:
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
	@echo -e "$(C_YELLOW)$(RK_MEDIA_CHIP) media info:"
	@env|grep "^CONFIG\|^RK_MEDIA" | grep -v "CONFIG_RK_MEDIA_CROSS\|CONFIG_RK_CHIP" | sort
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"

strip:
	$(call MAROC_STRIP_DEBUG_SYMBOL, $(RK_MEDIA_OUTPUT))
