KMODULE_NAME = ssv6x5x

ifeq ($(KERNEL_OBJ_PATH),)
KERNEL_OBJ_PATH := /lib/modules/`uname -r`/build
endif

ifeq ($(SSV_DRV_PATH),)
SSV_DRV_PATH := $(PWD)/wifi/ssv6115
endif
$(info "SSV_DRV_PATH: :" $(SSV_DRV_PATH))

KBUILD_TOP := $(SSV_DRV_PATH)
ifeq ($(MAKELEVEL),0)
KBUILD_TOP := .
endif

ifeq ($(ARCH),)
ARCH := x86
#ARCH := arm
endif

ifeq ($(INSTALL_PATH),)
INSTALL_PATH := kernel/drivers/net/wireless/ssv6200
endif

include $(KBUILD_TOP)/config.mak
#include $(PWD)/config.mak

# Generate version strings
# GEN_VER := $(shell cd $(KBUILD_TOP); ./ver_info.pl include/ssv_version.h)
# Generate include/ssv_conf_parser.h
# GEN_CONF_PARSER := $(shell cd $(KBUILD_TOP); env ccflags="$(ccflags-y)" ./parser-conf.sh include/ssv_conf_parser.h)
# Generate $(KMODULE_NAME)-wifi.cfg
#BKP_CFG := $(shell cp $(KBUILD_TOP)/$(PLATFORMS)-wifi.cfg $(KBUILD_TOP)/image/$(KMODULE_NAME)-wifi.cfg)

EXTRA_CFLAGS := -I$(KBUILD_TOP) -I$(KBUILD_TOP)/include

#DEF_PARSER_H = $(KBUILD_TOP)/include/ssv_conf_parser.h
#$(shell env ccflags="$(ccflags-y)" $(KBUILD_TOP)/parser-conf.sh $(DEF_PARSER_H))

#------------------------------
# ssvdevice/
KERN_SRCS := ssvdevice/ssvdevice.c
KERN_SRCS += ssvdevice/init.c
KERN_SRCS += ssvdevice/debugfs_cmd.c
KERN_SRCS += ssvdevice/rftool/ssv_phy_rf.c
KERN_SRCS += ssvdevice/ssv_custom_func.c
KERN_SRCS += ssvdevice/rftool/ssv_efuse.c
KERN_SRCS += ssvdevice/ssv_version.c

# KERN_SRCS += hwif/common/common.c
KERN_SRCS += hwif/hwif.c
ifneq ($(CONFIG_HWIF_SUPPORT),2)
KERN_SRCS += hwif/usb/usb.c
endif
ifneq ($(CONFIG_HWIF_SUPPORT),1)
KERN_SRCS += hwif/sdio/sdio.c
endif

KERN_SRCS += hci/ssv_hci.c

KERN_SRCS += utils/debugfs.c
KERN_SRCS += utils/ssv_netlink_ctl.c

ifeq ($(CONFIG_WPA_SUPPLICANT_CTL),y)
KERN_SRCS += utils/ssv_wpas_ctl.c
endif

KERN_SRCS += utils/ssv_alloc_skb.c

#.fmac/
KERN_SRCS += fmac/cfg_ops.c
KERN_SRCS += fmac/fmac.c

KERN_SRCS += fmac/fmac_mod_params.c
KERN_SRCS += fmac/fmac_cmds.c
KERN_SRCS += fmac/fmac_msg_tx.c
KERN_SRCS += fmac/fmac_msg_rx.c
KERN_SRCS += fmac/fmac_utils.c
KERN_SRCS += fmac/fmac_strs.c
KERN_SRCS += fmac/fmac_tx.c
KERN_SRCS += fmac/fmac_rx.c
KERN_SRCS += fmac/ipc_host.c

KERN_SRCS += fmac/netdev_ops.c

KERN_SRCS += ssvdevice/rftool/ssv_rftool.c
KERN_SRCS += ssvdevice/rftool/ssv_rftool_msg.c

# nimble/
KERN_SRCS += ble/nimble/nimble.c
KERN_SRCS += ble/nimble/nimble_msg.c

ifeq ($(CONFIG_FMAC_BRIDGE),y)
KERN_SRCS += fmac/fmac_bridge.c
endif

# ble-hci/
ifeq ($(CONFIG_BLE),y)
KERN_SRCS += ble/ble_hci/ble_hci.c
KERN_SRCS += ble/ble_hci/ble_hci_msg.c
KERN_SRCS += ble/ble_hci/hcidev_ops.c
endif
#------------------------------

KERN_SRCS += $(KMODULE_NAME)-generic-wlan.c

$(KMODULE_NAME)-y += $(KERN_SRCS_S:.S=.o)
$(KMODULE_NAME)-y += $(KERN_SRCS:.c=.o)

#obj-$(CONFIG_SSV6X5X) += $(KMODULE_NAME).o
obj-m += $(KMODULE_NAME).o

ifeq ($(CONFIG_PRE_ALLOC_SKB),2)
KMODULE_PRE_ALLOCATE_NAME = pre-allocate

PRE_ALLOC_SRCS += utils/pre_alloc_skb.c
PRE_ALLOC_SRCS += utils/pre_allocate.c
$(KMODULE_PRE_ALLOCATE_NAME)-y += $(PRE_ALLOC_SRCS:.c=.o)
obj-$(CONFIG_SSV6X5X) += $(KMODULE_PRE_ALLOCATE_NAME).o
else
  ifeq ($(CONFIG_PRE_ALLOC_SKB),1)
  KERN_SRCS += utils/pre_alloc_skb.c
  KERN_SRCS += utils/pre_allocate.c
  endif
endif

#export CONFIG_SSV6X5X=m

.PHONY: all ver modules clean

all: version modules

version:
	chmod 755 ./script/check_coding_rule.sh
	chmod 755 ./script/ver_info.sh
	./script/check_coding_rule.sh
	./script/ver_info.sh

modules:
	#$(MAKE) -C $(KERNEL_OBJ_PATH) M=$(SSV_DRV_PATH) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules -j12 O=$(WIFI_BUILD_KERNEL_OBJ_DIR)
	cp $(SSV_DRV_PATH)/ssv6x5x.ko $(M_OUT_DIR)/ssv6115.ko

strip:
	$(CROSS_COMPILE)strip $(KMODULE_NAME).ko --strip-unneeded

install:
	$(MAKE) INSTALL_MOD_DIR=$(INSTALL_PATH) -C $(KERNEL_OBJ_PATH) \
            M=$(SSV_DRV_PATH) modules_install
	depmod -a

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean O=$(WIFI_BUILD_KERNEL_OBJ_DIR)
