RWNX_VERS_NUM := 6.4.4.3

# Platform support list
CONFIG_PLATFORM_NANOPI_M4 ?= n
CONFIG_PLATFORM_ALLWINNER ?= n
CONFIG_PLATFORM_INGENIC_T31 ?= n
CONFIG_PLATFORM_INGENIC_T40 ?= n
CONFIG_PLATFORM_UBUNTU ?= n
CONFIG_PLATFORM_ROCKCHIP ?= y

# Driver mode support list
CONFIG_VNET_MODE ?= y
CONFIG_RAWDATA_MODE ?= n

# Insmod mode
CONFIG_FAST_INSMOD ?= n

# Msg Callback setting
CONFIG_APP_FASYNC ?= y

CONFIG_AIC8800_WLAN_SUPPORT = m
MODULE_NAME = aic8800_netdrv

# Support of bootrom start
CONFIG_START_FROM_BOOTROM = n

# Support of pmic setting, new version bootrom avaliable
CONFIG_PMIC_SETTING ?=n

# ROM patch enabled option
CONFIG_ROM_PATCH_EN ?=n

# Support chip with mcu
CONFIG_MCU_INTEGRATED ?= n
CONFIG_MCU_MESSAGE ?= n
ifeq ($(CONFIG_MCU_INTEGRATED), y)
CONFIG_PMIC_SETTING = n
else
CONFIG_MCU_MESSAGE ?= n
endif

#
# WAITING FOR KCONFIG {
#
CONFIG_RWNX_SOFTMAC ?= n
CONFIG_RWNX_FULLMAC ?= y
CONFIG_RWNX_FHOST ?= n

#
# DEBUG OPTIONS
#CONFIG_RWNX_UM_HELPER_DFLT ?= "/dini/dini_bin/rwnx_umh.sh"
#CONFIG_AIC_FW_PATH = "/vendor/etc/firmware"
#export CONFIG_AIC_FW_PATH

#
# FW ARCH:
CONFIG_RWNX_SDM ?= n
CONFIG_RWNX_TL4 ?= n

# IPC version
CONFIG_RWNX_OLD_IPC ?= n

# Support of P2P DebugFS for enabling/disabling NoA and OppPS
CONFIG_RWNX_P2P_DEBUGFS ?= y

# FW path
ifeq ($(CONFIG_PLATFORM_NANOPI_M4), y) # NANOPI_M4
CONFIG_VENDOR_SPECIFIED_FW_PATH ?= "/vendor/firmware"
else # NANOPI_M4
ifeq ($(CONFIG_PLATFORM_ALLWINNER), y) # ALLWINNER
CONFIG_VENDOR_SPECIFIED_FW_PATH ?= "/vendor/etc/firmware"
else # ALLWINNER
ifeq ($(CONFIG_PLATFORM_INGENIC_T31), y) # INGENTIC_T31
#CONFIG_VENDOR_SPECIFIED_FW_PATH ?= "/lib/firmware"
else # INGENTIC_T31
ifeq ($(CONFIG_PLATFORM_INGENIC_T40), y) # INGENTIC_T40
#CONFIG_VENDOR_SPECIFIED_FW_PATH ?= "/lib/firmware"
else # INGENTIC_T40
ifeq ($(CONFIG_PLATFORM_UBUNTU), y) # PC
#CONFIG_VENDOR_SPECIFIED_FW_PATH ?= "/lib/firmware"
CONFIG_VENDOR_SUBDIR_NAME ?= "aic8800"
endif # PC
endif # INGENTIC_T31
endif # INGENTIC_T40
endif # ALLWINNER
endif # NANOPI_M4
ifneq ($(CONFIG_VENDOR_SPECIFIED_FW_PATH),)
subdir-ccflags-y += -DCONFIG_VENDOR_SPECIFIED_FW_PATH=\"$(CONFIG_VENDOR_SPECIFIED_FW_PATH)\"
endif
ifneq ($(CONFIG_VENDOR_SUBDIR_NAME),)
subdir-ccflags-y += -DCONFIG_VENDOR_SUBDIR_NAME=\"$(CONFIG_VENDOR_SUBDIR_NAME)\"
endif

#
# } // WAITING FOR KCONFIG
#

# Enable A-MSDU support (need FW support)
## Select this if FW is compiled with AMSDU support
CONFIG_RWNX_SPLIT_TX_BUF ?= n
## Select this TO send AMSDU
CONFIG_RWNX_AMSDUS_TX ?= n

# Enable BFMER support (need FW support)
CONFIG_RWNX_BFMER ?= n

CONFIG_SDIO_SUPPORT =y
CONFIG_USB_SUPPORT =n
CONFIG_RX_REORDER ?=y
CONFIG_ARP_OFFLOAD =n
CONFIG_USE_5G =n
CONFIG_RADAR_OR_IR_DETECT =n
CONFIG_DOWNLOAD_FW =n
CONFIG_LOAD_USERCONFIG ?=n
CONFIG_BT_SUPPORT ?=n
CONFIG_RFTEST=n
CONFIG_USB_BT=n
CONFIG_MAC_RANDOM_IF_NO_MAC_IN_EFUSE=n
CONFIG_SDIO_PWRCTRL ?=y
ifeq ($(CONFIG_VNET_MODE), y)
CONFIG_TX_NETIF_FLOWCTRL = y
endif
ifeq ($(CONFIG_RAWDATA_MODE), y)
CONFIG_TX_NETIF_FLOWCTRL = n
endif

# Support of MU-MIMO transmission (need FW support)
ifeq ($(CONFIG_RWNX_BFMER), y)
CONFIG_RWNX_MUMIMO_TX ?= n
else
CONFIG_RWNX_MUMIMO_TX = n
endif

# Enable handling of radar event
CONFIG_RWNX_RADAR ?= y

# Enable HW queue for Broadcast/Multicast traffic (need FW support)
CONFIG_RWNX_BCMC ?= y

# Enable Monitor+Data interface support (need FW support)
CONFIG_RWNX_MON_DATA =y

# extra DEBUG config
CONFIG_RWNX_SW_PROFILING ?= n
CONFIG_RWNX_DBG ?= y

obj-$(CONFIG_AIC8800_WLAN_SUPPORT) := $(MODULE_NAME).o
$(MODULE_NAME)-y :=       \
	rwnx_main.o           \
	rwnx_rx.o             \
	rwnx_tx.o             \
	rwnx_platform.o       \
	rwnx_term_ops.o       \
	virt_net.o            \
	aicwf_custom_utils.o
#	rwnx_msg_tx.o          \
#	rwnx_msg_rx.o          \
#	rwnx_utils.o           \
#	rwnx_cmds.o            \
#	rwnx_irqs.o            \
#	rwnx_cfgfile.o         \
#	rwnx_strs.o            \
#	rwnx_txq.o             \
#	rwnx_mesh.o            \
#	rwnx_pci.o             \
#	rwnx_dini.o            \
#	rwnx_v7.o              \
#	ipc_host.o             \
#	rwnx_tdls.o

$(MODULE_NAME)-$(CONFIG_SDIO_SUPPORT)     += sdio_host.o
$(MODULE_NAME)-$(CONFIG_SDIO_SUPPORT)     += aicwf_txrxif.o
$(MODULE_NAME)-$(CONFIG_SDIO_SUPPORT)     += aicwf_sdio.o

$(MODULE_NAME)-$(CONFIG_USB_SUPPORT)     += usb_host.o
$(MODULE_NAME)-$(CONFIG_USB_SUPPORT)     += aicwf_txrxif.o
$(MODULE_NAME)-$(CONFIG_USB_SUPPORT)     += aicwf_usb.o

ccflags-$(CONFIG_DEBUG_FS) += -DCONFIG_RWNX_DEBUGFS
ccflags-$(CONFIG_DEBUG_FS) += -DCONFIG_RWNX_UM_HELPER_DFLT=\"$(CONFIG_RWNX_UM_HELPER_DFLT)\"
ccflags-$(CONFIG_RWNX_P2P_DEBUGFS) += -DCONFIG_RWNX_P2P_DEBUGFS

# FW VARS
ccflags-y += -DNX_VIRT_DEV_MAX=4
ccflags-y += -DNX_REMOTE_STA_MAX=10
ccflags-y += -DNX_MU_GROUP_MAX=62
ccflags-y += -DNX_TXDESC_CNT=64
ccflags-y += -DNX_TX_MAX_RATES=4
ccflags-y += -DNX_CHAN_CTXT_CNT=3

# FW ARCH:
ccflags-$(CONFIG_RWNX_SDM) += -DCONFIG_RWNX_SDM
ccflags-$(CONFIG_RWNX_TL4) += -DCONFIG_RWNX_TL4
ccflags-$(CONFIG_RWNX_OLD_IPC) += -DCONFIG_RWNX_OLD_IPC
ccflags-$(CONFIG_PLATFORM_NANOPI_M4) += -DCONFIG_NANOPI_M4
ccflags-$(CONFIG_PLATFORM_INGENIC_T31) += -DCONFIG_INGENIC_T31
ccflags-$(CONFIG_PLATFORM_INGENIC_T40) += -DCONFIG_INGENIC_T40
ccflags-$(CONFIG_PLATFORM_ALLWINNER) += -DCONFIG_PLATFORM_ALLWINNER
ccflags-$(CONFIG_START_FROM_BOOTROM) += -DCONFIG_START_FROM_BOOTROM
ccflags-$(CONFIG_PMIC_SETTING) += -DCONFIG_PMIC_SETTING
ccflags-$(CONFIG_ROM_PATCH_EN) += -DCONFIG_ROM_PATCH_EN
ccflags-$(CONFIG_PLATFORM_ALLWINNER) += -DCONFIG_HE_FOR_OLD_KERNEL
ccflags-$(CONFIG_PLATFORM_ALLWINNER) += -DCONFIG_COEX
ccflags-$(CONFIG_PLATFORM_NANOPI_M4) += -DCONFIG_COEX

ccflags-$(CONFIG_VNET_MODE) += -DCONFIG_VNET_MODE
ccflags-$(CONFIG_RAWDATA_MODE) += -DCONFIG_RAWDATA_MODE

ccflags-$(CONFIG_FAST_INSMOD) += -DCONFIG_FAST_INSMOD
ccflags-$(CONFIG_APP_FASYNC) += -DCONFIG_APP_FASYNC

ccflags-y += -DCONFIG_RWNX_FULLMAC
ccflags-y += -I$(src)/.
ccflags-$(CONFIG_RWNX_RADAR) += -DCONFIG_RWNX_RADAR
ccflags-$(CONFIG_RWNX_MON_DATA) += -DCONFIG_RWNX_MON_DATA
ccflags-$(CONFIG_RWNX_BFMER) += -DCONFIG_RWNX_BFMER
ccflags-$(CONFIG_RWNX_SPLIT_TX_BUF) += -DCONFIG_RWNX_SPLIT_TX_BUF
ifeq ($(CONFIG_RWNX_SPLIT_TX_BUF), y)
ccflags-$(CONFIG_RWNX_AMSDUS_TX) += -DCONFIG_RWNX_AMSDUS_TX
endif
ccflags-$(CONFIG_RWNX_DBG) += -DCONFIG_RWNX_DBG
ccflags-$(CONFIG_RWNX_SW_PROFILING) += -DCONFIG_RWNX_SW_PROFILING
ccflags-$(CONFIG_RWNX_MUMIMO_TX) += -DCONFIG_RWNX_MUMIMO_TX
ccflags-$(CONFIG_RFTEST) += -DCONFIG_RFTEST

ifeq ($(CONFIG_SDIO_SUPPORT), y)
ccflags-y += -DAICWF_SDIO_SUPPORT
ccflags-$(CONFIG_SDIO_PWRCTRL) += -DCONFIG_SDIO_PWRCTRL
endif

ifeq ($(CONFIG_USB_SUPPORT), y)
ccflags-y += -DAICWF_USB_SUPPORT
endif

ifeq ($(CONFIG_RWNX_MUMIMO_TX), y)
ccflags-y += -DCONFIG_USER_MAX=2
else
ccflags-y += -DCONFIG_USER_MAX=1
endif

ifeq ($(CONFIG_RWNX_BCMC), y)
ccflags-y += -DNX_TXQ_CNT=5
else
ccflags-y += -DNX_TXQ_CNT=4
endif

# For old kernel (<=3.19)
ifeq ($(shell test $(VERSION) -lt 4 -a "$(CONFIG_VENDOR_RWNX)" = y ; echo $$?),0)
ccflags-y += -DCONFIG_VENDOR_RWNX_VHT_NO80
endif

ccflags-$(CONFIG_RX_REORDER) += -DAICWF_RX_REORDER
ccflags-$(CONFIG_ARP_OFFLOAD) += -DAICWF_ARP_OFFLOAD
ccflags-$(CONFIG_USE_5G) += -DUSE_5G
ccflags-$(CONFIG_RADAR_OR_IR_DETECT) += -DRADAR_OR_IR_DETECT
ccflags-$(CONFIG_DOWNLOAD_FW)  += -DCONFIG_DOWNLOAD_FW
ccflags-$(CONFIG_LOAD_USERCONFIG) += -DCONFIG_LOAD_USERCONFIG
ccflags-$(CONFIG_BT_SUPPORT) += -DCONFIG_BT_SUPPORT
ccflags-$(CONFIG_USB_BT)  += -DCONFIG_USB_BT
ccflags-$(CONFIG_MAC_RANDOM_IF_NO_MAC_IN_EFUSE) += -DCONFIG_MAC_RANDOM_IF_NO_MAC_IN_EFUSE
ccflags-$(CONFIG_TX_NETIF_FLOWCTRL) += -DCONFIG_TX_NETIF_FLOWCTRL

ifeq ($(CONFIG_PLATFORM_ROCKCHIP), y)
KDIR  := /home/yaya/E/Rockchip/3229/Android10/SDK/kernel/
ARCH ?= arm
CROSS_COMPILE ?= /home/yaya/E/Rockchip/3229/Android10/SDK/prebuilts/gcc/linux-x86/arm/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
ccflags-y += -DANDROID_PLATFORM

EXTRA_CFLAGS += -Wno-error=unused-result -Werror=return-type
all: modules
modules:
	#make -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(shell pwd) -j8 O=$(WIFI_BUILD_KERNEL_OBJ_DIR)
	$(CROSS_COMPILE)strip --strip-debug $(shell pwd)/aic8800_netdrv.ko
	cp $(shell pwd)/aic8800_netdrv.ko $(M_OUT_DIR)
clean:
	rm -rf *.o *.ko *.o.* *.mod.* modules.* Module.* .a* .o* .*.o.* *.mod .tmp* .cache.mk
endif

ifeq ($(CONFIG_PLATFORM_ALLWINNER), y)
ccflags-y += -DANDROID_PLATFORM
endif

ifeq ($(CONFIG_PLATFORM_AMLOGIC), y)
ccflags-y += -DANDROID_PLATFORM
endif

ifeq ($(CONFIG_PLATFORM_UBUNTU), y)
KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
KVER := $(shell uname -r)
MODDESTDIR := /lib/modules/$(KVER)/kernel/drivers/net/wireless/aic8800
ARCH ?= x86_64
CROSS_COMPILE ?=
all: modules
modules:
	make -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

install:
	mkdir -p $(MODDESTDIR)
	install -p -m 644 $(MODULE_NAME).ko  $(MODDESTDIR)
	/sbin/depmod -a ${KVER}

uninstall:
	rm -rfv $(MODDESTDIR)/$(MODULE_NAME).ko
	/sbin/depmod -a ${KVER}

clean:
	rm -rf *.o *.ko *.o.* *.mod.* modules.* Module.* .a* .o* .*.o.* *.mod .tmp* .cache.mk
endif

ifeq ($(CONFIG_PLATFORM_INGENIC_T31), y)
ARCH ?= mips
CROSS_COMPILE ?=mips-linux-gnu-
PWD ?= .
#KERNELDIR depends on the location of t31 kernel code
KERNELDIR ?=/your_kernel_dir/kernel
all: modules
modules clean:
	@$(PWD)/mklink.sh
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) $@
	@$(PWD)/mklink.sh clean
endif

ifeq ($(CONFIG_PLATFORM_INGENIC_T40), y)
ARCH ?= mips
CROSS_COMPILE ?=mips-linux-gnu-
PWD ?= .
#KERNELDIR depends on the location of t40 kernel code
KERNELDIR ?=/your_kernel_dir/kernel-4.4.94
all: modules
modules clean:
	@$(PWD)/mklink.sh
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) $@
	@$(PWD)/mklink.sh clean
endif

