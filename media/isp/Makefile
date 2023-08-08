
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := camera_engine_rkaiq
PKG_BIN ?= out
PKG_BUILD ?= build
PKG_RELEASE := $(strip release_$(strip $(PKG_NAME))_$(RK_MEDIA_CHIP)_$(RK_MEDIA_CROSS))
PKG_NAME_RKISP_DEMO := $(PKG_NAME)/rkisp_demo

ifeq ($(CURRENT_DIR)/$(PKG_NAME)/rkaiq/CMakeLists.txt, $(wildcard $(CURRENT_DIR)/$(PKG_NAME)/rkaiq/CMakeLists.txt))
PKG_NAME_RKAIQ := $(PKG_NAME)/rkaiq
PKG_NAME_RK3A_SERVER := $(PKG_NAME)/rkaiq_3A_server
PKG_NAME_RKAIQ_SMARTIR := $(PKG_NAME)/smart_ir
PKG_NAME_RK_STREAM := $(PKG_NAME)/rk_stream
PKG_CONF_OPTS += -DRKAIQ_BUILD_BINARY_IQ=ON
ifeq ($(CONFIG_RK_ISP_BUILD_DEMO),y)
PKG_CONF_OPTS += -DRKAIQ_ENABLE_RGA=ON
PKG_CONF_OPTS += -DRK_AIQ_INSTALLED_DIR=$(CURRENT_DIR)/$(PKG_BIN)
PKG_TARGET += rga-build rkisp_demo-build
endif
PKG_TARGET += rkaiq_3a_server-build
PKG_TARGET += rkaiq_smart_ir-build
PKG_TARGET += rk_stream-build
else
PKG_NAME_RKAIQ := $(PKG_NAME)
ifeq ($(CONFIG_RK_ISP_BUILD_DEMO),y)
PKG_CONF_OPTS += -DENABLE_RKISP_DEMO=ON
PKG_CONF_OPTS += -DRKAIQ_ENABLE_RGA=ON
PKG_TARGET += rga-build
endif
endif

RK_MEDIA_OPTS += -I${RK_MEDIA_OUTPUT}/include -I${RK_MEDIA_OUTPUT}/include/libdrm -L${RK_MEDIA_OUTPUT}/lib -L$(CURRENT_DIR)/$(PKG_BIN)/lib
RK_MEDIA_OPTS += -Wl,-rpath-link,${RK_MEDIA_OUTPUT}/lib:$(RK_MEDIA_OUTPUT)/root/usr/lib
PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_MEDIA_OPTS)" -DCMAKE_CXX_FLAGS="$(RK_MEDIA_OPTS)"
PKG_TOOLCHAIN_SYSROOT := $(shell $(RK_MEDIA_CROSS)-gcc -print-sysroot)
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH="$(CURRENT_DIR)/$(PKG_BIN);$(RK_MEDIA_OUTPUT);$(PKG_TOOLCHAIN_SYSROOT)"
PKG_CONF_OPTS += -DCMAKE_SYSROOT="$(PKG_TOOLCHAIN_SYSROOT)"

PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
PKG_CONF_OPTS += -DCMAKE_NO_SYSTEM_FROM_IMPORTED=ON
PKG_CONF_OPTS += -DCMAKE_SYSTEM_NAME=Linux

RK_ISP_ARCH_TYPE := $(RK_MEDIA_ARCH_TYPE)
# debug: build cmake with more message
# PKG_CONF_OPTS += -DCMAKE_VERBOSE_MAKEFILE=ON
#

PKG_CONF_OPTS += -DRKAIQ_TARGET_SOC=$(CONFIG_RK_CHIP)

ifeq ($(CONFIG_RK_ISP_HW_VERSION_V20),y)
PKG_CONF_OPTS += -DISP_HW_VERSION=-DISP_HW_V20
PKG_RESOURCE_DIR := $(CURRENT_DIR)/$(PKG_NAME_RKAIQ)/iqfiles
endif

ifeq ($(CONFIG_RK_ISP_HW_VERSION_V30),y)
PKG_CONF_OPTS += -DISP_HW_VERSION=-DISP_HW_V30
PKG_RESOURCE_DIR := $(CURRENT_DIR)/$(PKG_NAME_RKAIQ)/iqfiles/isp3x
endif

ifeq ($(CONFIG_RK_ISP_HW_VERSION_V32),y)
PKG_CONF_OPTS += -DISP_HW_VERSION=-DISP_HW_V32
PKG_RESOURCE_DIR := $(CURRENT_DIR)/$(PKG_NAME_RKAIQ)/iqfiles/isp32
endif

ifeq ($(RK_MEDIA_ARCH_TYPE), arm)
RK_ISP_ARCH_TYPE := arm
else
RK_ISP_ARCH_TYPE := aarch64
endif

ifeq ($(CONFIG_RK_RKAIQ),y)
PKG_TARGET += camera_engine_rkaiq-build
endif

ifeq ($(CONFIG_RK_CHIP),rk3588)
PKG_CONF_OPTS += -DRKAIQ_TOOL_SERVER=ON
PKG_NAME_RKISP_DEMO := $(PKG_NAME)/tests/rkisp_demo
endif

ifeq ($(CONFIG_RK_CHIP),rv1106)
PKG_CONF_OPTS += -DRKAIQ_ENABLE_LIBDRM=OFF
endif

PKG_CONF_OPTS += -DRKAIQ_ENABLE_AF=ON

ifeq ($(wildcard $(CURRENT_DIR)/$(PKG_NAME_RKISP_DEMO)/CMakeLists.txt),)
PKG_NAME_RKISP_DEMO:=$(PKG_RELEASE)/rkisp_demo
PKG_CONF_OPTS += -DRK_AIQ_INSTALLED_DIR=$(CURRENT_DIR)/$(PKG_BIN)
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	@rm -fr $(CURRENT_DIR)/$(PKG_BIN)/etc $(CURRENT_DIR)/$(PKG_BIN)/usr;
	@rm -fr $(CURRENT_DIR)/$(PKG_BIN)/rkisp_demo;
ifneq ($(RK_MEDIA_OUTPUT),)
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))
endif
	@echo "build $(PKG_NAME) done"

rga-build:
	@test ! -d $(RK_MEDIA_TOP_DIR)/rga || make -C $(RK_MEDIA_TOP_DIR)/rga

camera_engine_rkaiq-build: camera_engine_rkaiq-release
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME_RKAIQ) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/librkaiq.so || (\
	echo "Start to build $(PKG_NAME_RKAIQ)"; \
	rm -rf $(PKG_BIN) $(PKG_BUILD)/$(PKG_NAME_RKAIQ); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RKAIQ); \
	pushd $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RKAIQ); \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME_RKAIQ) \
			-DCMAKE_C_COMPILER=$(RK_MEDIA_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_MEDIA_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_BUILD_TYPE=MinSizeRel \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DBUILD_DOC=OFF \
			-DBUILD_DOCS=OFF \
			-DBUILD_EXAMPLE=OFF \
			-DBUILD_EXAMPLES=OFF \
			-DBUILD_TEST=OFF \
			-DBUILD_TESTS=OFF \
			-DBUILD_TESTING=OFF \
			-DBUILD_SHARED_LIBS=ON \
			-DARCH="$(RK_ISP_ARCH_TYPE)" \
			$(PKG_CONF_OPTS) \
			;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
			mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/host; \
			mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/bin; \
			cp -fr $(CURRENT_DIR)/$(PKG_BIN)/usr/bin/* $(CURRENT_DIR)/$(PKG_BIN)/bin; \
			rm -fr $(CURRENT_DIR)/$(PKG_BIN)/etc $(CURRENT_DIR)/$(PKG_BIN)/isp_iqfiles; \
			mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/isp_iqfiles && cp -rfa $(PKG_RESOURCE_DIR)/* $(CURRENT_DIR)/$(PKG_BIN)/isp_iqfiles; \
			cp -rfa $(CURRENT_DIR)/$(PKG_BIN)/usr/share/iqfiles/* $(CURRENT_DIR)/$(PKG_BIN)/isp_iqfiles; \
			rm -rf $(CURRENT_DIR)/$(PKG_BIN)/usr; \
	popd; ) || exit -1;
ifneq ($(RK_MEDIA_OUTPUT),)
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))
endif

camera_engine_rkaiq-release:
	$(call MAROC_PKG_CHECK_PREBUILT, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))

rkaiq_3a_server-build: camera_engine_rkaiq-build
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME_RK3A_SERVER) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/bin/rkaiq_3A_server || (\
	echo "Start to build $(PKG_NAME_RK3A_SERVER)"; \
	rm -rf $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RK3A_SERVER) ; \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RK3A_SERVER) ; \
	pushd $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RK3A_SERVER) ; \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME_RK3A_SERVER) \
			-DCMAKE_C_COMPILER=$(RK_MEDIA_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_MEDIA_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_BUILD_TYPE=MinSizeRel \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DBUILD_DOC=OFF \
			-DBUILD_DOCS=OFF \
			-DBUILD_EXAMPLE=OFF \
			-DBUILD_EXAMPLES=OFF \
			-DBUILD_TEST=OFF \
			-DBUILD_TESTS=OFF \
			-DBUILD_TESTING=OFF \
			-DBUILD_SHARED_LIBS=ON \
			-DARCH="$(RK_ISP_ARCH_TYPE)" \
			$(PKG_CONF_OPTS) \
			;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
			cp -fr $(CURRENT_DIR)/$(PKG_BIN)/usr/bin/* $(CURRENT_DIR)/$(PKG_BIN)/bin; \
	popd; ) || exit -1;

rkisp_demo-build: camera_engine_rkaiq-build
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME_RKISP_DEMO) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/bin/rkisp_demo || (\
	echo "Start to build $(PKG_NAME_RKISP_DEMO)"; \
	rm -rf $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RKISP_DEMO) ; \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RKISP_DEMO) ; \
	pushd $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RKISP_DEMO) ; \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME_RKISP_DEMO) \
			-DCMAKE_C_COMPILER=$(RK_MEDIA_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_MEDIA_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_BUILD_TYPE=MinSizeRel \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DBUILD_DOC=OFF \
			-DBUILD_DOCS=OFF \
			-DBUILD_EXAMPLE=OFF \
			-DBUILD_EXAMPLES=OFF \
			-DBUILD_TEST=OFF \
			-DBUILD_TESTS=OFF \
			-DBUILD_TESTING=OFF \
			-DBUILD_SHARED_LIBS=ON \
			-DARCH="$(RK_ISP_ARCH_TYPE)" \
			$(PKG_CONF_OPTS) \
			;\
			make -j$(RK_JOBS) || exit -1; \
			(find $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RKISP_DEMO) -type f -name rkisp_demo | \
				xargs -n 1 -I{} cp {} $(CURRENT_DIR)/$(PKG_BIN)/bin -fv); \
	popd; ) || exit -1;

rkaiq_smart_ir-build: camera_engine_rkaiq-build
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME_RKAIQ_SMARTIR) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/libsmartIr.so || (\
	echo "Start to build $(PKG_NAME_RKAIQ_SMARTIR)"; \
	rm -rf $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RKAIQ_SMARTIR) ; \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RKAIQ_SMARTIR) ; \
	pushd $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RKAIQ_SMARTIR) ; \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME_RKAIQ_SMARTIR) \
			-DCMAKE_C_COMPILER=$(RK_MEDIA_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_MEDIA_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_BUILD_TYPE=MinSizeRel \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DBUILD_DOC=OFF \
			-DBUILD_DOCS=OFF \
			-DBUILD_EXAMPLE=OFF \
			-DBUILD_EXAMPLES=OFF \
			-DBUILD_TEST=OFF \
			-DBUILD_TESTS=OFF \
			-DBUILD_TESTING=OFF \
			-DBUILD_SHARED_LIBS=ON \
			-DARCH="$(RK_ISP_ARCH_TYPE)" \
			$(PKG_CONF_OPTS) ;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
	popd; ) || exit -1;

rk_stream-build: camera_engine_rkaiq-build
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME_RK_STREAM) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/librkrawstream.so || (\
	echo "Start to build $(PKG_NAME_RK_STREAM)"; \
	rm -rf $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RK_STREAM) ; \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RK_STREAM) ; \
	pushd $(CURRENT_DIR)/$(PKG_BUILD)/$(PKG_NAME_RK_STREAM) ; \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME_RK_STREAM) \
			-DCMAKE_C_COMPILER=$(RK_MEDIA_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_MEDIA_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_BUILD_TYPE=MinSizeRel \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DBUILD_DOC=OFF \
			-DBUILD_DOCS=OFF \
			-DBUILD_EXAMPLE=OFF \
			-DBUILD_EXAMPLES=OFF \
			-DBUILD_TEST=OFF \
			-DBUILD_TESTS=OFF \
			-DBUILD_TESTING=OFF \
			-DBUILD_SHARED_LIBS=ON \
			-DARCH="$(RK_ISP_ARCH_TYPE)" \
			-DAIQ_DIR="../../$(PKG_NAME_RKAIQ)" \
			$(PKG_CONF_OPTS) ;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
	popd; ) || exit -1;

clean:
	@rm -rf $(PKG_BUILD) $(PKG_BIN);

distclean: clean
################################################################################
__RELEASE__: # DO NOT RUN THIS TARGET
	$(call MAROC_STRIP_DEBUG_SYMBOL, $(PKG_BIN))
	$(call MAROC_PKG_RELEASE, $(CURRENT_DIR),$(PKG_NAME),$(PKG_BIN))
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME_RKAIQ) || (\
		rm -rf $(CURRENT_DIR)/$(PKG_RELEASE)/rkisp_demo; \
		cp -rfa $(CURRENT_DIR)/$(PKG_NAME_RKISP_DEMO) \
		$(CURRENT_DIR)/$(PKG_RELEASE) || exit 1; \
	)
