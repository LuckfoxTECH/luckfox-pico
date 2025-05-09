EXTRA_CFLAGS += $(USER_EXTRA_CFLAGS)
EXTRA_CFLAGS += -Wno-implicit-fallthrough
EXTRA_CFLAGS += -Wno-unused-function
#EXTRA_CFLAGS += -Wno-maybe-uninitialized
EXTRA_CFLAGS += -Wno-unused-variable

RWNX_VERS_NUM := 6.4.3.0

CONFIG_COUNTRY_CODE = "00"

MODULE_NAME = aic8800_fdrv
CONFIG_AIC8800_WLAN_SUPPORT = m

# Support of bootrom start
CONFIG_START_FROM_BOOTROM = y

# Support of pmic setting, new version bootrom avaliable
CONFIG_PMIC_SETTING ?=y

# Select 8800DC/DW DCDC_VRF mode, check your board
CONFIG_VRF_DCDC_MODE = y


# ROM patch enabled option
CONFIG_ROM_PATCH_EN ?=y
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
CONFIG_RWNX_FULLMAC ?= y
CONFIG_RWNX_FHOST ?= n

#
# DEBUG OPTIONS
CONFIG_RWNX_UM_HELPER_DFLT ?= "/dini/dini_bin/rwnx_umh.sh"

#
# FW ARCH:
CONFIG_RWNX_SDM ?= n
CONFIG_RWNX_TL4 ?= n

# IPC version
CONFIG_RWNX_OLD_IPC ?= n

# Support of P2P DebugFS for enabling/disabling NoA and OppPS
CONFIG_RWNX_P2P_DEBUGFS := n
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
CONFIG_ARP_OFFLOAD =y
CONFIG_RADAR_OR_IR_DETECT =n
CONFIG_DOWNLOAD_FW =n
CONFIG_RFTEST=y
CONFIG_USB_BT =y
CONFIG_SDIO_BT=n
CONFIG_USE_5G ?= y
CONFIG_CREATE_TRACE_POINTS = n
CONFIG_TXRX_THREAD_PRIO = y
# CONFIG_COEX = n for BT_ONLY, CONFIG_COEX =y for combo and sw
CONFIG_COEX = y
CONFIG_RX_NETIF_RECV_SKB = y
CONFIG_SET_VENDOR_EXTENSION_IE = n
CONFIG_SUPPORT_REALTIME_CHANGE_MAC = y
CONFIG_WPA3_FOR_OLD_KERNEL ?= n
CONFIG_VHT_FOR_OLD_KERNEL ?= n
CONFIG_HE_FOR_OLD_KERNEL ?= n
CONFIG_PREALLOC_RX_SKB ?= n
CONFIG_WIFI_SUSPEND_FOR_LINUX = n
# Need to set fw path in BOARD_KERNEL_CMDLINE
CONFIG_USE_FW_REQUEST ?= n
CONFIG_USE_P2P0=n
CONFIG_TX_NETIF_FLOWCTRL = n
CONFIG_ONE_TXQ = n
CONFIG_BR_SUPPORT =n
BR_NAME = br0
CONFIG_FDRV_NO_REG_SDIO=n
CONFIG_SCHED_SCAN = n
CONFIG_OOB ?= n
CONFIG_USE_CUSTOMER_MAC = n
CONFIG_PREALLOC_TXQ ?= y
CONFIG_DPD = y
CONFIG_FORCE_DPD_CALIB = y
CONFIG_LOFT_CALIB = n
CONFIG_FILTER_TCP_ACK =y
CONFIG_RESV_MEM_SUPPORT ?= y
CONFIG_GKI = n
CONFIG_TEMP_COMP = n
CONFIG_TEMP_CONTROL = n
CONFIG_EXT_FEM_8800DCDW = n
CONFIG_SHUTDOWN_CALLBACK = y
CONFIG_FOR_IPCAM = n
CONFIG_SDIO_ADMA = n
# CONFIG_MCC = n for sta and p2p concurrent in same channel.
CONFIG_MCC = y
CONFIG_APF = n
CONFIG_POWER_LIMIT = n

#CONFIG FOR LOW POWER MODE
CONFIG_SDIO_PWRCTRL ?= y
CONFIG_GPIO_WAKEUP ?= n
CONFIG_AUTO_POWERSAVE = n

# Enable wifi-hal Latency Mode
CONFIG_AICWF_LATENCY_MODE = n

ifneq ($(CONFIG_WIRELESS_EXT), y)
CONFIG_USE_WIRELESS_EXT = n
endif

ifeq ($(CONFIG_EXT_FEM_8800DCDW), y)
CONFIG_DPD = n
CONFIG_FORCE_DPD_CALIB = n
CONFIG_LOFT_CALIB = y
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
CONFIG_RWNX_MON_DATA = n
CONFIG_RWNX_MON_XMIT = n
CONFIG_RWNX_MON_RXFILTER = n


# extra DEBUG config
CONFIG_RWNX_SW_PROFILING ?= n
CONFIG_RWNX_DBG ?= y
CONFIG_DEBUG_FS ?= n

obj-$(CONFIG_AIC8800_WLAN_SUPPORT) := $(MODULE_NAME).o
$(MODULE_NAME)-y := \
	rwnx_msg_tx.o          \
	rwnx_msg_rx.o          \
	rwnx_utils.o           \
	rwnx_cmds.o            \
	rwnx_irqs.o            \
	rwnx_cfgfile.o         \
	rwnx_strs.o            \
	rwnx_rx.o              \
	rwnx_tx.o              \
	rwnx_txq.o             \
	rwnx_main.o            \
	rwnx_mod_params.o      \
	rwnx_mesh.o            \
	rwnx_platform.o        \
	rwnx_pci.o             \
	rwnx_dini.o            \
	rwnx_v7.o              \
	ipc_host.o             \
	rwnx_tdls.o            \
	aic_vendor.o	       \
	md5.o                  \
	aicwf_compat_8800dc.o  \
	aicwf_compat_8800d80.o \
	rwnx_wakelock.o		\
	regdb.o				   \
	aic_priv_cmd.o		\
	aicwf_rx_prealloc.o

$(MODULE_NAME)-$(CONFIG_BR_SUPPORT)       += aic_br_ext.o
$(MODULE_NAME)-$(CONFIG_RWNX_RADAR)       += rwnx_radar.o
$(MODULE_NAME)-$(CONFIG_DEBUG_FS)         += rwnx_debugfs.o
$(MODULE_NAME)-$(CONFIG_DEBUG_FS)         += rwnx_fw_trace.o
$(MODULE_NAME)-$(CONFIG_NL80211_TESTMODE) += rwnx_testmode.o
$(MODULE_NAME)-$(CONFIG_RWNX_BFMER)       += rwnx_bfmer.o
$(MODULE_NAME)-$(CONFIG_RWNX_MUMIMO_TX)   += rwnx_mu_group.o
$(MODULE_NAME)-$(CONFIG_SDIO_SUPPORT)     += sdio_host.o
$(MODULE_NAME)-$(CONFIG_SDIO_SUPPORT)     += aicwf_txrxif.o
$(MODULE_NAME)-$(CONFIG_SDIO_SUPPORT)     += aicwf_sdio.o
$(MODULE_NAME)-$(CONFIG_FILTER_TCP_ACK)   += aicwf_tcp_ack.o
$(MODULE_NAME)-$(CONFIG_SDIO_BT)   += aic_btsdio.o
$(MODULE_NAME)-$(CONFIG_SDIO_BT)   += btsdio.o

$(MODULE_NAME)-$(CONFIG_USB_SUPPORT)     += usb_host.o
$(MODULE_NAME)-$(CONFIG_USB_SUPPORT)     += aicwf_txrxif.o
$(MODULE_NAME)-$(CONFIG_USB_SUPPORT)     += aicwf_usb.o
$(MODULE_NAME)-$(CONFIG_GKI)     += rwnx_gki.o



ccflags-$(CONFIG_DEBUG_FS) += -DCONFIG_RWNX_DEBUGFS
ccflags-$(CONFIG_DEBUG_FS) += -DCONFIG_RWNX_UM_HELPER_DFLT=\"$(CONFIG_RWNX_UM_HELPER_DFLT)\"
ccflags-$(CONFIG_RWNX_P2P_DEBUGFS) += -DCONFIG_RWNX_P2P_DEBUGFS

# FW VARS
ccflags-y += -DNX_VIRT_DEV_MAX=4

#for 8800D and DCDW u01
#ccflags-y += -DNX_REMOTE_STA_MAX=10

#for 8800DCDW u02
ccflags-y += -DNX_REMOTE_STA_MAX_FOR_OLD_IC=10
ccflags-y += -DNX_REMOTE_STA_MAX=32

ccflags-y += -DNX_MU_GROUP_MAX=62
ccflags-y += -DNX_TXDESC_CNT=64
ccflags-y += -DNX_TX_MAX_RATES=4
ccflags-y += -DNX_CHAN_CTXT_CNT=3

# FW ARCH:
ccflags-$(CONFIG_RWNX_SDM) += -DCONFIG_RWNX_SDM
ccflags-$(CONFIG_RWNX_TL4) += -DCONFIG_RWNX_TL4
ccflags-$(CONFIG_RWNX_OLD_IPC) += -DCONFIG_RWNX_OLD_IPC
ccflags-$(CONFIG_PLATFORM_NANOPI_M4) += -DCONFIG_NANOPI_M4
ccflags-$(CONFIG_PLATFORM_INGENIC_T20) += -DCONFIG_INGENIC_T20
ccflags-$(CONFIG_PLATFORM_ALLWINNER) += -DCONFIG_PLATFORM_ALLWINNER
ccflags-$(CONFIG_START_FROM_BOOTROM) += -DCONFIG_START_FROM_BOOTROM
ccflags-$(CONFIG_PMIC_SETTING) += -DCONFIG_PMIC_SETTING
ccflags-$(CONFIG_VRF_DCDC_MODE) += -DCONFIG_VRF_DCDC_MODE
ccflags-$(CONFIG_ROM_PATCH_EN) += -DCONFIG_ROM_PATCH_EN
ccflags-$(CONFIG_HE_FOR_OLD_KERNEL) += -DCONFIG_HE_FOR_OLD_KERNEL
ccflags-$(CONFIG_MCU_INTEGRATED) += -DCONFIG_MCU_INTEGRATED
ccflags-$(CONFIG_MCU_MESSAGE) += -DCONFIG_MCU_MESSAGE
ccflags-$(CONFIG_COEX) += -DCONFIG_COEX

ccflags-y += -DCONFIG_RWNX_FULLMAC
ccflags-y += -I$(srctree)
ccflags-y += -I$(srctree)/$(src)
ccflags-y += -I$(srctree)/$(src)/../aic8800_bsp
ccflags-y += -DCONFIG_AIC_FW_PATH=\"$(CONFIG_AIC_FW_PATH)\"
ccflags-$(CONFIG_RWNX_RADAR) += -DCONFIG_RWNX_RADAR
ccflags-$(CONFIG_RWNX_MON_DATA) += -DCONFIG_RWNX_MON_DATA
ccflags-$(CONFIG_RWNX_MON_XMIT) += -DCONFIG_RWNX_MON_XMIT
ccflags-$(CONFIG_RWNX_MON_RXFILTER) += -DCONFIG_RWNX_MON_RXFILTER

ccflags-$(CONFIG_RWNX_BFMER) += -DCONFIG_RWNX_BFMER
ccflags-$(CONFIG_RWNX_SPLIT_TX_BUF) += -DCONFIG_RWNX_SPLIT_TX_BUF
ifeq ($(CONFIG_RWNX_SPLIT_TX_BUF), y)
ccflags-$(CONFIG_RWNX_AMSDUS_TX) += -DCONFIG_RWNX_AMSDUS_TX
endif
ccflags-$(CONFIG_RWNX_DBG) += -DCONFIG_RWNX_DBG
ccflags-$(CONFIG_RWNX_SW_PROFILING) += -DCONFIG_RWNX_SW_PROFILING
ccflags-$(CONFIG_RWNX_MUMIMO_TX) += -DCONFIG_RWNX_MUMIMO_TX
ccflags-$(CONFIG_RFTEST) += -DCONFIG_RFTEST
ccflags-y += -DDEFAULT_COUNTRY_CODE=""\$(CONFIG_COUNTRY_CODE)"\"
ccflags-$(CONFIG_USE_5G) += -DUSE_5G
ccflags-$(CONFIG_CREATE_TRACE_POINTS) += -DCREATE_TRACE_POINTS
ccflags-$(CONFIG_TXRX_THREAD_PRIO) += -DCONFIG_TXRX_THREAD_PRIO
ccflags-$(CONFIG_GPIO_WAKEUP) += -DCONFIG_GPIO_WAKEUP
ccflags-$(CONFIG_SET_VENDOR_EXTENSION_IE) += -DCONFIG_SET_VENDOR_EXTENSION_IE
ccflags-$(CONFIG_SUPPORT_REALTIME_CHANGE_MAC) += -DCONFIG_SUPPORT_REALTIME_CHANGE_MAC
ccflags-$(CONFIG_WPA3_FOR_OLD_KERNEL) += -DCONFIG_WPA3_FOR_OLD_KERNEL
ccflags-$(CONFIG_VHT_FOR_OLD_KERNEL) += -DCONFIG_VHT_FOR_OLD_KERNEL
ccflags-$(CONFIG_PREALLOC_RX_SKB) += -DCONFIG_PREALLOC_RX_SKB
ccflags-$(CONFIG_WIFI_SUSPEND_FOR_LINUX) += -DCONFIG_WIFI_SUSPEND_FOR_LINUX
ccflags-$(CONFIG_USE_FW_REQUEST) += -DCONFIG_USE_FW_REQUEST
ccflags-$(CONFIG_USE_P2P0) += -DCONFIG_USE_P2P0
ccflags-$(CONFIG_FDRV_NO_REG_SDIO) += -DCONFIG_FDRV_NO_REG_SDIO
ccflags-$(CONFIG_SCHED_SCAN) += -DCONFIG_SCHED_SCAN
ccflags-$(CONFIG_OOB) += -DCONFIG_OOB
ccflags-$(CONFIG_AICWF_LATENCY_MODE) += -DAICWF_LATENCY_MODE
ccflags-$(CONFIG_USE_CUSTOMER_MAC) += -DCONFIG_USE_CUSTOMER_MAC
ccflags-$(CONFIG_PREALLOC_TXQ) += -DCONFIG_PREALLOC_TXQ
ccflags-$(CONFIG_DPD) += -DCONFIG_DPD
ccflags-$(CONFIG_FORCE_DPD_CALIB) += -DCONFIG_FORCE_DPD_CALIB -DCONFIG_DPD
ccflags-$(CONFIG_LOFT_CALIB) += -DCONFIG_LOFT_CALIB
ccflags-$(CONFIG_FILTER_TCP_ACK) += -DCONFIG_FILTER_TCP_ACK
ccflags-$(CONFIG_SDIO_BT) += -DCONFIG_SDIO_BT
ccflags-$(CONFIG_RESV_MEM_SUPPORT) += -DCONFIG_RESV_MEM_SUPPORT
ccflags-$(CONFIG_GKI) += -DCONFIG_GKI
ccflags-$(CONFIG_TEMP_COMP) += -DCONFIG_TEMP_COMP
ccflags-$(CONFIG_TEMP_CONTROL) += -DCONFIG_TEMP_CONTROL
ccflags-$(CONFIG_POWER_LIMIT) += -DCONFIG_POWER_LIMIT
ccflags-$(CONFIG_EXT_FEM_8800DCDW) += -DCONFIG_EXT_FEM_8800DCDW
ccflags-$(CONFIG_MCC) += -DCONFIG_MCC
ccflags-$(CONFIG_SHUTDOWN_CALLBACK) += -DCONFIG_SHUTDOWN_CALLBACK
ccflags-$(CONFIG_FOR_IPCAM) += -DCONFIG_FOR_IPCAM
ccflags-$(CONFIG_SDIO_ADMA) += -DCONFIG_SDIO_ADMA


ifeq ($(CONFIG_SDIO_SUPPORT), y)
ccflags-y += -DAICWF_SDIO_SUPPORT
ccflags-$(CONFIG_SDIO_PWRCTRL) += -DCONFIG_SDIO_PWRCTRL
endif

ifeq ($(CONFIG_USB_SUPPORT), y)
ccflags-y += -DAICWF_USB_SUPPORT
endif

ifeq ($(CONFIG_BR_SUPPORT), y)
ccflags-y += -DCONFIG_BR_SUPPORT
ccflags-y += '-DCONFIG_BR_SUPPORT_BRNAME="'$(BR_NAME)'"'
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
ccflags-$(CONFIG_RADAR_OR_IR_DETECT) += -DCONFIG_RADAR_OR_IR_DETECT
ccflags-$(CONFIG_DOWNLOAD_FW)  += -DCONFIG_DOWNLOAD_FW
ccflags-$(CONFIG_RX_NETIF_RECV_SKB) += -DCONFIG_RX_NETIF_RECV_SKB
ccflags-$(CONFIG_ONE_TXQ)  += -DCONFIG_ONE_TXQ
ccflags-$(CONFIG_TX_NETIF_FLOWCTRL) += -DCONFIG_TX_NETIF_FLOWCTRL
ccflags-$(CONFIG_APF) += -DCONFIG_APF
ccflags-$(CONFIG_AUTO_POWERSAVE) += -DCONFIG_AUTO_POWERSAVE

ccflags-y += -DAIC_TRACE_INCLUDE_PATH=$(src)
MAKEFLAGS +=-j$(shell nproc)

########## Platform support list ##########
CONFIG_PLATFORM_ROCKCHIP ?= n
CONFIG_PLATFORM_ROCKCHIP2 ?= n
CONFIG_PLATFORM_ALLWINNER ?= n
CONFIG_PLATFORM_INGENIC_T20 ?= n
CONFIG_PLATFORM_AMLOGIC ?= n
CONFIG_PLATFORM_UBUNTU ?= y

ifeq ($(CONFIG_PLATFORM_ROCKCHIP), y)
ARCH := arm64
KDIR ?= /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/kernel
CROSS_COMPILE := /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
ccflags-$(CONFIG_PLATFORM_ROCKCHIP) += -DCONFIG_PLATFORM_ROCKCHIP
ccflags-y += -DANDROID_PLATFORM
endif

ifeq ($(CONFIG_PLATFORM_ROCKCHIP2), y)
ARCH := arm64
KDIR ?= /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/kernel
CROSS_COMPILE := /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

ccflags-$(CONFIG_PLATFORM_ROCKCHIP2) += -DCONFIG_PLATFORM_ROCKCHIP2
ccflags-y += -DANDROID_PLATFORM
endif


ifeq ($(CONFIG_PLATFORM_ALLWINNER), y)
ccflags-$(CONFIG_PLATFORM_ALLWINNER) += -DCONFIG_PLATFORM_ALLWINNER
ccflags-y += -DANDROID_PLATFORM
KDIR  ?= /home/yaya/E/Allwinner/r818/Android10/lichee/kernel/linux-4.9/
ARCH ?= arm64
CROSS_COMPILE ?= /home/yaya/E/Allwinner/r818/Android10/android/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-
endif

ifeq ($(CONFIG_PLATFORM_INGENIC_T20), y)
KDIR ?= /home/yaya/E/T40/kernel
ARCH ?= mips
CROSS_COMPILE ?= /home/yaya/E/T40/mips-linux-gnu-ingenic-gcc7.2.0-glibc2.29-fp64/bin/mips-linux-gnu-
endif

ifeq ($(CONFIG_PLATFORM_AMLOGIC), y)
ccflags-$(CONFIG_PLATFORM_AMLOGIC) += -DCONFIG_PLATFORM_AMLOGIC
ccflags-y += -DANDROID_PLATFORM
endif

ifeq ($(CONFIG_PLATFORM_UBUNTU), y)
ccflags-$(CONFIG_PLATFORM_UBUNTU) += -DCONFIG_PLATFORM_UBUNTU
KDIR  ?= /lib/modules/$(shell uname -r)/build
PWD   ?= $(shell pwd)
KVER ?= $(shell uname -r)
MODDESTDIR ?= /lib/modules/$(KVER)/kernel/drivers/net/wireless/
ARCH ?= x86_64
CROSS_COMPILE ?=
endif
###########################################


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


