
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := rockit
PKG_BIN ?= out
PKG_BUILD ?= build

RK_MEDIA_OPTS += -Wl,-rpath-link,${RK_MEDIA_OUTPUT}/lib:$(RK_MEDIA_OUTPUT)/root/usr/lib
PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_MEDIA_OPTS)" -DCMAKE_CXX_FLAGS="$(RK_MEDIA_OPTS)"
PKG_CONF_OPTS += -DRKPLATFORM=ON

# debug: build cmake with more message
# PKG_CONF_OPTS += -DCMAKE_VERBOSE_MAKEFILE=ON
#
ifeq ($(RK_MEDIA_ARCH_TYPE), arm)
PKG_CONF_OPTS += -DARCH64=OFF
endif

ifeq ($(RK_MEDIA_ARCH_TYPE), arm64)
PKG_CONF_OPTS += -DARCH64=ON
endif

ifeq ($(CONFIG_RK_ROCKIT),y)
ifeq ($(CURRENT_DIR)/$(PKG_NAME),$(wildcard $(CURRENT_DIR)/$(PKG_NAME)))
ifeq ($(CONFIG_RK_ROCKIT_TGI),y)
PKG_CONF_OPTS += -DUSE_ROCKIT_TGI=ON
else
PKG_CONF_OPTS += -DUSE_ROCKIT_TGI=OFF
endif
PKG_TARGET := rockit-build
else
PKG_TARGET :=
$(warning Not found source of $(PKG_NAME), Skip...)
endif
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done";

rga-build:
	@test ! -d $(RK_MEDIA_TOP_DIR)/rga || make -C $(RK_MEDIA_TOP_DIR)/rga

common_algorithm-build :
	@test ! -d $(RK_MEDIA_TOP_DIR)/common_algorithm || make -C $(RK_MEDIA_TOP_DIR)/common_algorithm

rockit-build: rga-build common_algorithm-build libv4l mpp libdrm
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/librockit.so || (\
	echo "Start to build $(PKG_NAME)"; \
	rm -rf $(PKG_BIN) $(PKG_BUILD); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD)/; \
	pushd $(CURRENT_DIR)/$(PKG_BUILD); \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME)/ \
			-DCMAKE_C_COMPILER=$(RK_MEDIA_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_MEDIA_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DBUILD_DOC=OFF \
			-DBUILD_DOCS=OFF \
			-DBUILD_EXAMPLE=OFF \
			-DBUILD_EXAMPLES=OFF \
			-DBUILD_TESTS=OFF \
			-DBUILD_TESTING=OFF \
			-DBUILD_SHARED_LIBS=ON \
			-DCMAKE_SYSTEM_NAME=Linux \
			$(PKG_CONF_OPTS) \
			-DARCH="$(RK_MEDIA_ARCH_TYPE)" ;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
	popd; ) || exit -1;
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))
	@echo "build $(PKG_NAME) done";

libdrm:
	@test ! -d $(RK_MEDIA_TOP_DIR)/libdrm || make -C $(RK_MEDIA_TOP_DIR)/libdrm

mpp:
	@test ! -d $(RK_MEDIA_TOP_DIR)/mpp || make -C $(RK_MEDIA_TOP_DIR)/mpp

libv4l:
	@test ! -d $(RK_MEDIA_TOP_DIR)/libv4l || make -C $(RK_MEDIA_TOP_DIR)/libv4l

clean:
	@rm -fr $(PKG_BUILD) $(PKG_BIN)

distclean: clean
