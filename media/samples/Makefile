
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

ifneq ($(findstring $(RK_ENABLE_SAMPLE),n y),)
CONFIG_RK_SAMPLE=$(RK_ENABLE_SAMPLE)
else
CONFIG_RK_SAMPLE=y
endif

CURRENT_DIR := $(shell pwd)

ifeq ($(rk_static),1)
export BUILD_STATIC_LINK=y
else
export BUILD_STATIC_LINK=n
endif

ifeq ($(RK_MEDIA_SAMPLE_STATIC_LINK),y)
export BUILD_STATIC_LINK=y
else
export BUILD_STATIC_LINK=n
endif

PKG_NAME := sample
PKG_BUILD ?= build

all: $(PKG_TARGET)
ifeq ($(CONFIG_RK_SAMPLE),y)
	@make -C $(CURRENT_DIR)/simple_test/ -j$(RK_MEDIA_JOBS)
	@make -C $(CURRENT_DIR)/example/ -j$(RK_MEDIA_JOBS)
endif
	@echo "build $(PKG_NAME) done";

example-build:

simple_test-build:

clean:
	@make -C $(CURRENT_DIR)/example/ clean
	@make -C $(CURRENT_DIR)/simple_test/ clean

help:
	@echo "help message:"
	@echo "     build with dynamic link:  make "
	@echo "     build with static  link:  make rk_static=1"

distclean: clean
