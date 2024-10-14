
KVERSION="`uname -r`"
#DRVPATH=/lib/modules/$(KVERSION)/kernel/drivers/net/wireless/ssv6200
#DRVPATH=kernel/drivers/net/wireless/ssv6200
KCFLAG += -Werror
#EXTRA_CFLAGS := -I$(KBUILD_TOP) -I$(KBUILD_TOP)/include
EXTRA_CFLAGS := -I$(KBUILD_TOP) 
EXTRA_CFLAGS += -I$(KBUILD_TOP)/include 

CONFIG_SSV6X5X=m

#Compile option
ccflags-y := -D__CHECK_ENDIAN__ -DDEBUG
ccflags-y += -Werror
# Enable -g to help debug. Deassembly from .o to .S would help to track to 
# the problomatic line from call stack dump.
#ccflags-y += -g

###########################################################
# FW VARS for FMAC (must align fw setting)
###########################################################
CONFIG_SSV_VER ?= 691
ccflags-y += -DSSV_VER=$(CONFIG_SSV_VER)
ccflags-y += -DNX_VIRT_DEV_MAX=2
ccflags-y += -DNX_REMOTE_STA_MAX=11
ccflags-y += -DNX_CHAN_CTXT_CNT=3
ccflags-y += -DNX_TXQ_CNT=5

###########################################################
# SW component
###########################################################
## HCI使用
ccflags-y += -DCONFIG_HWIF_AND_HCI

# TX MULTI URB 開啟或關閉
# 此功能不能與HCI TX AGGR共存
# 需要設定ssv6x5x-wifi.cfg 中 hw_cap_hci_tx_aggr = off
CONFIG_SSV_MULTI_URB ?= n
ifeq ($(CONFIG_SSV_MULTI_URB), y)
ccflags-y += -DCONFIG_USB_TX_MULTI_URB
endif

## FMAC使用
ccflags-y += -DCONFIG_FMAC
# OBSS scans and generate 20/40 BSS coex report
#ccflags-y += -DNEED_OBSS_SCAN
# send keep alive frame in driver side every 30 seconds
#ccflags-y += -DSEND_KEEP_ALIVE

## NIMBLE使用((注意与BLE的使用为2选1))
#ccflags-y += -DCONFIG_NIMBLE

## BLE HCI usage (注意与NIMBLE的使用为2选1)
CONFIG_BLE ?= n
ccflags-$(CONFIG_BLE) += -DCONFIG_BLE

# BLE HCI BUS 取值：
# CONFIG_BLE_HCI_OVER_UART:0
# CONFIG_BLE_HCI_OVER_HWIF:1  USB or SDIO, base on HWIF
ccflags-y += -DCONFIG_BLE_HCI_BUS=1

## wpa_supplicant接口使用
CONFIG_WPA_SUPPLICANT_CTL ?= y
ccflags-$(CONFIG_WPA_SUPPLICANT_CTL) += -DCONFIG_WPA_SUPPLICANT_CTL

## pre-alloc skb功能
# CONFIG_PRE_ALLOC_SKB的设定说明如下：
#  0:  关闭pre-alloc skb功能
#  1:  打开功能，并编译成1个ko文件
#  2:  打开功能，并编译成2个ko文件
#
# 关闭pre-alloc skb功能的情况下，rx queue len是128。
# 打开pre-alloc skb功能之后，rx queue len默认值SSV_PRE_RX_ALLOC_LEN是32，pre-alloc rx skb len为32+2。
# 用户可在insmod driver时通过模组参数的形式修改rx queue len的值。
# 例如，将rx queue len改为10，可通过以下实现：
# (1) 1个ko的情况：
#     insmod ssv6x5x.ko qlen=10 stacfgpath=ssv6x5x-wifi.cfg (注意：pre-alloc rx skb len是qlen+2，即为12)
# (2) 2个ko的情况：
#     insmod pre-allocate.ko qlen=10 (注意：pre-alloc rx skb len是qlen+2，即为12)
#     insmod ssv6x5x.ko stacfgpath=ssv6x5x-wifi.cfg
CONFIG_PRE_ALLOC_SKB ?= 0
ccflags-y += -DCONFIG_PRE_ALLOC_SKB=$(CONFIG_PRE_ALLOC_SKB)


#网桥功能
#CONFIG_FMAC_BRIDGE ?= y
#ccflags-$(CONFIG_FMAC_BRIDGE) += -DFMAC_BRIDGE

## Mifi lowpower suupport
#ccflags-y += -DCONFIG_MIFI_LOWPOWER

###########################################################
# HWIF相關設定
###########################################################
## HWIF uses
# CONFIG_HWIF_SUPPORT, 1: USB only; 2: SDIO only; others: USB + SDIO
CONFIG_HWIF_SUPPORT ?= 0
ccflags-y += -DHWIF_SUPPORT=$(CONFIG_HWIF_SUPPORT)

# Trim code (量產用)
ccflags-y += -DHWIF_TRIM_CODE

## HWIF設定Padmux
## useless if defined HWIF_DIS_FW_DOWNLOAD ##
#############################################
#ccflags-y += -DHWIF_SET_PADMUX
#############################################

## SDIO相關設定
# SDIO RX在IRQ收封包傳到上層
ccflags-y += -DHWIF_SDIO_RX_IRQ
# SDIO使用CMD52確認TX resource
ccflags-y += -DSDIO_CMD52_CHK_TX_RESOURCE
# SDIO使用CMD52取得RX size
ccflags-y += -DSDIO_CMD52_GET_RX_SIZE
# SDIO使用one bit mode
#ccflags-y += -DSDIO_USE_ONE_BIT


###########################################################
# Debug使用
###########################################################
## 驅動不載入firmware
#ccflags-y += -DHWIF_DIS_FW_DOWNLOAD
## 不做Net device初始化
#ccflags-y += -DDIS_NETDEV_INIT
## FMAC初始化後通知FW內部reset (若有特殊情況不想HOST通知FW內部reset時再mark掉)
ccflags-y += -DFW_RESET_AT_INIT

## FW checksum檢查
#ccflags-y += -DHOST_CALCULATE_CHECKSUM

## 不顯示LOG
#ccflags-y += -DSSV_NO_LOG
## 依照config的dbg level顯示LOG
#ccflags-y += -DSSV_DEBUG_LOG
## 依照config的Module Dump Date FLAG顯示DATA的Hex dump 
#ccflags-y += -DSSV_DEBUG_LOG_DATA_DUMP

###########################################################
# 吞吐分析
###########################################################
## 效能分析
#ccflags-y += -DSSV_PERFORMANCE_WATCH
#./cli performance tx set value 
#     value [bit0: enable/disable drop netdev to fmac]
#     value [bit1: enable/disable drop fmac to hci]
#     value [bit2: enable/disable drop fmac to hwif]
#./cli performance tx show value 
#     value [bit0: enable/disable show sw txq skb囤積在肚子的數量
#     value [bit1: enable/disable show freesram]
#./cli performance tx watch value 
#     value 0- 4294967295 ms, 預期觀查時間


###########################################################
# 測試使用
###########################################################
## Module test
ccflags-y += -DSSV_MODULE_TEST

###########################################################
# CONFIG_FW_ALIGMNET_CHECK
# enable/disable 4 bytes alignment, PLATFORM_DEF_DMA_ALIGN_SIZE define to 32 (bytes) Now
###########################################################
ccflags-y += -DPLATFORM_DEF_DMA_ALIGN_SIZE=32

###########################################################
# CONFIG_SSV_CHANNEL_FOLLOW
# concurrent mode，softap跟随sta信道功能
# 此功能开启需要hostapd搭配额外的patch 和 kernel版本大于3.11
# 否则会有不预期的结果
###########################################################
#ccflags-y += -DCONFIG_SSV_CHANNEL_FOLLOW
