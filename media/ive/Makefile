ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := ive
PKG_BIN ?= out
PKG_BUILD ?= build

PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_MEDIA_OPTS)" -DCMAKE_CXX_FLAGS="$(RK_MEDIA_OPTS)"
PKG_CONF_OPTS += -DTARGET_SOC=$(CONFIG_RK_CHIP)
RK_MEDIA_OPTS += -I${RK_MEDIA_OUTPUT}/include -L${RK_MEDIA_OUTPUT}/lib
RK_MEDIA_OPTS += -Wl,-rpath-link,${RK_MEDIA_OUTPUT}/lib:$(RK_MEDIA_OUTPUT)/root/usr/lib
PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_MEDIA_OPTS)" -DCMAKE_CXX_FLAGS="$(RK_MEDIA_OPTS)"
PKG_TOOLCHAIN_SYSROOT := $(shell $(RK_MEDIA_CROSS)-gcc -print-sysroot)
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH="$(RK_MEDIA_OUTPUT);$(PKG_TOOLCHAIN_SYSROOT)"
PKG_CONF_OPTS += -DCMAKE_SYSROOT="$(PKG_TOOLCHAIN_SYSROOT)"

PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY
# PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY
PKG_CONF_OPTS += -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
PKG_CONF_OPTS += -DCMAKE_NO_SYSTEM_FROM_IMPORTED=ON
# debug: build cmake with more message
# PKG_CONF_OPTS += -DCMAKE_VERBOSE_MAKEFILE=ON
#

ifeq ($(CONFIG_RK_IVE),y)
PKG_TARGET := rockit-build ive-build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))
	@echo "build $(PKG_NAME) done"

rockit-build:
	@test ! -d $(RK_MEDIA_TOP_DIR)/rockit || make -C $(RK_MEDIA_TOP_DIR)/rockit

ive-build:
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/librve.so || (\
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
			-DARCH="$(RK_MEDIA_ARCH_TYPE)" ;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
	popd; \
	);
ifneq ($(RK_MEDIA_OUTPUT),)
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))
endif

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD);

distclean: clean
