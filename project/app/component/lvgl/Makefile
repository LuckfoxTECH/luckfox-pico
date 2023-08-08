ifeq ($(APP_PARAM), )
    APP_PARAM:=../../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := lvgl
PKG_BIN ?= out
PKG_BUILD ?= build
PKG_CFG := conf

LVGL_DEF_CONF := $(CURRENT_DIR)/$(PKG_CFG)/lv_conf_default.h
LVGL_DST_CONF := $(CURRENT_DIR)/$(PKG_NAME)/lv_conf.h

ifeq ($(RK_APP_TYPE), RKCVR_RV1106)
LVGL_SRC_CONF := $(CURRENT_DIR)/$(PKG_CFG)/lv_conf_cvr.h
else ifeq ($(RK_APP_TYPE), FACIAL_GATE_RV1106_NORMAL)
LVGL_SRC_CONF := $(CURRENT_DIR)/$(PKG_CFG)/lv_conf_facialgate.h
else
LVGL_SRC_CONF := $(LVGL_DEF_CONF)
endif

RK_APP_OPTS =

RK_APP_CFLAGS = -I $(RK_APP_OUTPUT)/include/freetype2

PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_APP_CFLAGS) $(RK_APP_OPTS)" \
				-DCMAKE_CXX_FLAGS="$(RK_APP_CFLAGS) $(RK_APP_OPTS)"

ifeq ($(CONFIG_LVGL),y)
    PKG_TARGET := lvgl-build
else ifeq ($(RK_ENABLE_LVGL),y)
    PKG_TARGET := lvgl-build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))
	@echo "build $(PKG_NAME) done"

thirdlibs-build:
	@test ! -d $(RK_APP_COMPONENT)/third_libs || make -C $(RK_APP_COMPONENT)/third_libs

lvgl-build: thirdlibs-build lvgl-release
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/liblvgl.a || (\
	echo "Start to build $(PKG_NAME)"; \
	if [ -n $(LVGL_SRC_CONF) ] && [ -f $(LVGL_SRC_CONF) ] ; then \
		cp $(LVGL_SRC_CONF) $(LVGL_DST_CONF); \
	fi; \
	rm -rf $(PKG_BIN) $(PKG_BUILD); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD); \
	pushd $(CURRENT_DIR)/$(PKG_BUILD); \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME) \
			-DCMAKE_C_COMPILER=$(RK_APP_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_APP_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DBUILD_DOC=OFF \
			-DBUILD_DOCS=OFF \
			-DBUILD_EXAMPLE=OFF \
			-DBUILD_EXAMPLES=OFF \
			-DBUILD_TEST=OFF \
			-DBUILD_TESTS=OFF \
			-DBUILD_TESTING=OFF \
			-DBUILD_SHARED_LIBS=OFF \
			$(PKG_CONF_OPTS) \
			-DCMAKE_BUILD_TARGET=cmake_linux \
			-DARCH="$(RK_APP_ARCH_TYPE)" ;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
	popd; \
	) || exit 1;

lvgl-release:
	$(call MAROC_PKG_CHECK_PREBUILT, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD);

distclean: clean
################################################################################
__RELEASE__: # DO NOT RUN THIS TARGET
	$(call MAROC_STRIP_DEBUG_SYMBOL, $(PKG_BIN))
	$(call MAROC_PKG_RELEASE, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))
