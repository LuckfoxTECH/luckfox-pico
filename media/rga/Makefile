
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := rga
PKG_BIN ?= out
PKG_BUILD ?= build

PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_MEDIA_OPTS)" -DCMAKE_CXX_FLAGS="$(RK_MEDIA_OPTS)"

ifeq ($(CONFIG_RK_RGA2_USE_LIBDRM),y)
PKG_CONF_OPTS += -DBUILD_WITH_LIBDRM=true
PKG_CONF_OPTS += -DLIBDRM_FILE_INCLUDE=$(CURRENT_DIR)/../libdrm/out/include
PKG_CONF_OPTS += -DLIBDRM_FILE_LIB=$(CURRENT_DIR)/../libdrm/out/lib
endif

PKG_CONF_OPTS += -DRKRGA_TARGET_SOC=$(CONFIG_RK_CHIP)

PKG_CONF_OPTS += -DRGA_ENABLE_INSTALL_SAMPLES=ON

ifeq ($(CONFIG_RK_RGA2),y)
PKG_TARGET := rga-build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))
	@echo "build $(PKG_NAME) done"

rga-build: libdrm rga-release
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/librga.so || (\
	echo "Start to build $(PKG_NAME)"; \
	rm -rf $(PKG_BIN) $(PKG_BUILD); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD); \
	pushd $(CURRENT_DIR)/$(PKG_BUILD); \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME) \
			-DCMAKE_C_COMPILER=$(RK_MEDIA_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_MEDIA_CROSS)-g++ \
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
			-DBUILD_SHARED_LIBS=ON \
			$(PKG_CONF_OPTS) \
			-DCMAKE_BUILD_TARGET=cmake_linux \
			-DARCH="$(RK_MEDIA_ARCH_TYPE)" ;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
	popd; \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/include/rga; \
	mv -f `find $(CURRENT_DIR)/$(PKG_BIN)/include -type f` $(CURRENT_DIR)/$(PKG_BIN)/include/rga; \
	mv $(CURRENT_DIR)/$(PKG_BIN)/samples $(CURRENT_DIR)/$(PKG_BIN)/rga_samples; \
	) || exit -1;

rga-release:
	$(call MAROC_PKG_CHECK_PREBUILT, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))

libdrm:
ifeq ($(CONFIG_RK_RGA2_USE_LIBDRM),y)
	@test -f $(RK_MEDIA_OUTPUT)/lib/libdrm.so || make -C $(RK_MEDIA_TOP_DIR)/libdrm
endif

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD);

distclean: clean
################################################################################
__RELEASE__: # DO NOT RUN THIS TARGET
	$(call MAROC_STRIP_DEBUG_SYMBOL, $(PKG_BIN))
	$(call MAROC_PKG_RELEASE, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))
