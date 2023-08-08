ifeq ($(APP_PARAM), )
    APP_PARAM:=../../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := rkfsmk
PKG_BIN ?= out
PKG_BUILD ?= build

ifeq ($(CURRENT_DIR)/$(PKG_NAME),$(wildcard $(CURRENT_DIR)/$(PKG_NAME)))
PKG_TARGET := rkfsmk-build
else
PKG_TARGET :=
$(warning Not found source of $(PKG_NAME), Skip...)
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

RK_APP_OPTS += -Wl,-rpath-link,${RK_APP_OUTPUT}/lib:$(RK_APP_OUTPUT)/root/usr/lib
RK_APP_OPTS += -L${RK_APP_OUTPUT}/lib -L$(RK_APP_OUTPUT)/root/usr/lib
PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_APP_OPTS) -I${RK_APP_OUTPUT}/include" \
                 -DCMAKE_CXX_FLAGS="$(RK_APP_OPTS) -I${RK_APP_OUTPUT}/include"

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

rkfsmk-build:
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/librkfsmk.so || (\
	echo "Start to build $(PKG_NAME)"; \
	rm -rf $(PKG_BIN) $(PKG_BUILD); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BUILD)/; \
	pushd $(CURRENT_DIR)/$(PKG_BUILD); \
		rm -f CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME)/ \
			-DCMAKE_C_COMPILER=$(RK_APP_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_APP_CROSS)-g++ \
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
			-DARCH="$(RK_APP_ARCH_TYPE)" ;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
	popd; )
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))
	@echo "build $(PKG_NAME) done";

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD);

distclean: clean
