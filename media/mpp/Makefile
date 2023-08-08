
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := mpp
PKG_BIN ?= out
PKG_BUILD ?= build

PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_MEDIA_OPTS) -DARMLINUX -D__gnu_linux__"
PKG_CONF_OPTS += -DCMAKE_CXX_FLAGS="$(RK_MEDIA_OPTS) -DARMLINUX -D__gnu_linux__"
PKG_CONF_OPTS += -DRKPLATFORM=ON
# debug: build cmake with more message
# PKG_CONF_OPTS += -DCMAKE_VERBOSE_MAKEFILE=ON
#

ifeq ($(RK_MEDIA_CHIP), rv1126)
PKG_CONF_OPTS += -DCMAKE_SYSTEM_PROCESSOR=armv7l
endif

ifeq ($(RK_MEDIA_CHIP), rk3588)
PKG_CONF_OPTS += -DCMAKE_SYSTEM_PROCESSOR=aarch64
endif

ifeq ($(RK_MEDIA_CHIP), rv1106)
PKG_CONF_OPTS += -DCMAKE_SYSTEM_PROCESSOR=armv7l
endif

ifeq ($(CONFIG_RK_MPP_ALLOCATOR_DRM),y)
PKG_CONF_OPTS += -DBUILD_TEST=ON
endif
ifeq ($(CONFIG_RK_MPP_ALLOCATOR_DRM),y)
PKG_CONF_OPTS += -DHAVE_DRM=ON
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

ifeq ($(CONFIG_RK_MPP),y)
PKG_TARGET := mpp-build
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done";
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))

mpp-build: mpp-release
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/librockchip_mpp.so.0 || (\
	echo "Start to build $(PKG_NAME)"; \
	rm -rf $(PKG_BIN) $(PKG_BUILD); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD); \
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

mpp-release:
	$(call MAROC_PKG_CHECK_PREBUILT, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD);

distclean: clean
################################################################################
__RELEASE__: # DO NOT RUN THIS TARGET
	$(call MAROC_STRIP_DEBUG_SYMBOL, $(PKG_BIN))
	$(call MAROC_PKG_RELEASE, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))
