ifeq ($(SYSDRV_PARAM), )
    SYSDRV_PARAM:=../../../Makefile.param
    include $(SYSDRV_PARAM)
endif

export LC_ALL=C

CURRENT_DIR := $(shell pwd)

PKG_BIN := out
TARGET_BIN := rk_ota
PKG_SRCS := src/mtdutils.c src/md5sum.cpp src/bootloader.cpp src/main.cpp
CONFIG_IS_SUCCESSFUL_BOOT := n

PKG_DEP_OPENSSL := $(CURRENT_DIR)/../toolkits/openssl
PKG_DEP_ZLIB := $(CURRENT_DIR)/../toolkits/zlib
CFLAGS += \
		$(SYSDRV_CROSS_CFLAGS) \
		-I $(CURRENT_DIR)/src \
		-I $(PKG_DEP_OPENSSL)/out/include \
		-L $(PKG_DEP_OPENSSL)/out/lib \
		-I $(PKG_DEP_ZLIB)/out/include \
		-L $(PKG_DEP_ZLIB)/out/lib \
		-Wl,-Bstatic -lz -lcrypto -lssl -Wl,-Bdynamic \
		-Wl,--gc-sections -Wl,--as-needed

ifeq ($(CONFIG_IS_SUCCESSFUL_BOOT),y)
CFLAGS += -DSUCCESSFUL_BOOT=ON
else
CFLAGS += -DRETRY_BOOT=ON
endif

all: dep-openssl
	test -f $(CURRENT_DIR)/$(PKG_BIN)/usr/bin/$(TARGET_BIN) || (\
	rm -rf $(CURRENT_DIR)/$(PKG_BIN); \
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/bin; \
	$(SYSDRV_CROSS)-g++ $(PKG_SRCS) \
		-o $(CURRENT_DIR)/$(PKG_BIN)/usr/bin/$(TARGET_BIN) $(CFLAGS) || exit -1; \
	);
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(CURRENT_DIR)/$(PKG_BIN))

dep-openssl:
	$(Q)$(MAKE) -C $(PKG_DEP_OPENSSL)

clean: distclean

distclean:
	-$(Q)rm -rf $(PKG_BIN)
