
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../Makefile.param
    include $(MEDIA_PARAM)
endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := librkcrypto
PKG_BIN ?= out
PKG_BUILD ?= build

PKG_CONF_OPTS += -DCMAKE_C_FLAGS="$(RK_MEDIA_OPTS)" -DCMAKE_CXX_FLAGS="$(RK_MEDIA_OPTS)"

ifeq ($(CONFIG_RK_CRYPTO),y)
PKG_TARGET := rkcrypto-build
endif

ifeq ($(PKG_BIN),)
$(error ### $(CURRENT_DIR): PKG_BIN is NULL, Please Check !!!)
endif

all: $(PKG_TARGET)
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT), $(PKG_BIN))
	@echo "build $(PKG_NAME) done"

rkcrypto-build:
	@test ! -d $(CURRENT_DIR)/$(PKG_NAME) || \
		test -f $(CURRENT_DIR)/$(PKG_BIN)/lib/librkcrypto.so || (\
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
			$(PKG_CONF_OPTS) \
			-DARCH="$(RK_MEDIA_ARCH_TYPE)" ;\
			make -j$(RK_JOBS) || exit -1; \
			make install; \
	popd; \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/include/rkcrypto; \
	mv -f `find $(CURRENT_DIR)/$(PKG_BIN)/include -type f` $(CURRENT_DIR)/$(PKG_BIN)/include/rkcrypto; \
	) || exit -1;

clean:
	@rm -rf $(PKG_BIN) $(PKG_BUILD);

distclean: clean
