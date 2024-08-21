
ifeq ($(APP_PARAM), )
    APP_PARAM:=../../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := thirdlibs
PKG_BIN ?= out

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all:
	@rm -rf $(PKG_BIN)
	@mkdir -p ./$(PKG_BIN)/lib
	@cp -rfa include ./$(PKG_BIN)
	@cp -rfa lib/$(RK_APP_CROSS)/* ./$(PKG_BIN)/lib
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))
	@echo "build $(PKG_NAME) done"

clean:
	@rm -rf $(PKG_BIN)

distclean: clean

info:
	@echo "$(PKG_NAME) done"
