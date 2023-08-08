
ifeq ($(APP_PARAM), )
    APP_PARAM:=../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_INSTALL_TO_ROOTFS ?= NO
PKG_NAME := ipcweb-backend
PKG_BIN ?= out
PKG_BUILD ?= build
PKG_TARBALL := ipcweb-backend/ipcweb-env-$(RK_APP_ARCH_TYPE)
PKG_TARPATH ?= $(PKG_BIN)/
# debug: build cmake with more message
# PKG_CONF_OPTS += -DCMAKE_VERBOSE_MAKEFILE=ON
#

RK_APP_CFLAGS = -I $(RK_APP_MEDIA_INCLUDE_PATH) \
				-I $(CURRENT_DIR)/$(PKG_TARPATH)/include \

RK_APP_LDFLAGS = -L $(CURRENT_DIR)/$(PKG_TARPATH)/usr/lib/

RK_APP_OPTS += -Wl,-rpath-link,$(RK_APP_MEDIA_LIBS_PATH):$(RK_APP_PATH_LIB_INCLUDE)/root/usr/lib
PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_APP_CFLAGS) $(RK_APP_LDFLAGS) $(RK_APP_OPTS)" \
				-DCMAKE_CXX_FLAGS="$(RK_APP_CFLAGS) $(RK_APP_LDFLAGS) $(RK_APP_OPTS)"

# define project/cfg/BoardConfig*.mk
ifeq ($(RK_APP_IPCWEB_BACKEND),y)
PKG_TARGET := ipcweb-build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

ipcweb-build:
	@rm -rf $(PKG_BIN) $(PKG_TARPATH) && mkdir -p $(PKG_TARPATH) $(PKG_BIN) $(PKG_BUILD);
	@cp -rfa $(PKG_TARBALL)/include $(PKG_TARPATH)/;
	@cp -rfa $(PKG_TARBALL)/usr $(PKG_TARPATH)/;
	@echo "PKG_TARPATH is $(PKG_TARPATH)";
	pushd $(PKG_BUILD)/; \
		rm -rf CMakeCache.txt; \
		cmake $(CURRENT_DIR)/$(PKG_NAME)/ \
			-DCMAKE_C_COMPILER=$(RK_APP_CROSS)-gcc \
			-DCMAKE_CXX_COMPILER=$(RK_APP_CROSS)-g++ \
			-DCMAKE_INSTALL_PREFIX="$(CURRENT_DIR)/$(PKG_BIN)" \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_COLOR_MAKEFILE=OFF \
			-DCMAKE_SYSTEM_NAME=Linux \
			-DUSE_RKIPC=ON \
			$(PKG_CONF_OPTS) ;\
			make -j$(RK_APP_JOBS) || exit -1; \
			make install; \
	popd;
	(mkdir -p $(PKG_BIN)/usr;mv $(PKG_BIN)/www $(PKG_BIN)/usr;);

ifeq ($(PKG_INSTALL_TO_ROOTFS),YES)
	@cp -rfa $(PKG_TARBALL)/etc $(PKG_TARPATH)/;
	@cp -rfa $(PKG_TARBALL)/usr $(PKG_TARPATH)/;
else
	@cp -rfa $(PKG_TARBALL)/etc4oem $(PKG_TARPATH)/etc
endif

	@test ! -d $(PKG_BIN)/root/include || mv $(PKG_BIN)/root/include $(PKG_BIN)/;
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD) $(PKG_TARPATH)

distclean: clean

info:
ifeq ($(RK_APP_IPCWEB_BACKEND),y)
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
	@echo -e "$(C_YELLOW)-------------------------------------------------------------------------$(C_NORMAL)"
endif
