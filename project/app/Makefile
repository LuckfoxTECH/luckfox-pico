
MAKEFILE_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
export LC_ALL=C
SHELL:=/bin/bash
MAKE=make
AT?=@

################################################################################
##   Platform Specified  Configuraton
################################################################################
include $(MAKEFILE_DIR)/Makefile.param

################################################################################
##   Public Configuraton
################################################################################

app_src := $(wildcard ./*/Makefile)
app_src := $(dir $(app_src))
app_src := $(filter-out ./component/,$(app_src))

ifeq ($(RK_ENABLE_FASTBOOT),y)
pkg-build := fastboot-build
endif

all: $(pkg-build)
	$(foreach target,$(app_src),make -C $(target)||exit -1;)
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_PROJECT_PATH_APP), $(RK_APP_OUTPUT))

fastboot-build:
	@test ! -d $(RK_APP_COMPONENT)/fastboot_server || make -C $(RK_APP_COMPONENT)/fastboot_server

distclean: clean

clean:
	$(foreach target,$(app_src),make clean -C $(target)||exit -1;)
	@rm -rf $(RK_APP_OUTPUT)

info:
	@$(foreach target,$(app_src),make -C $(target) info||exit 0;)
