
KVERSION="`uname -r`"
#DRVPATH=/lib/modules/$(KVERSION)/kernel/drivers/net/wireless/ssv6200
#DRVPATH=kernel/drivers/net/wireless/ssv6200
KCFLAG += -Werror
#EXTRA_CFLAGS := -I$(KBUILD_TOP) -I$(KBUILD_TOP)/include
EXTRA_CFLAGS := -I$(KBUILD_TOP) 
EXTRA_CFLAGS += -I$(KBUILD_TOP)/include 


include $(KBUILD_TOP)/config_common.mak


ccflags-y += -DREPORT_TX_STATUS_DIRECTLY

########################################################
## The following definition move from common mak

# use for debug
ccflags-y += -DCONFIG_SSV6XXX_DEBUGFS
#### end of move from  common mak


## Use crypto in SSV driver for ssv6051
#ccflags-y += -DKTHREAD_BIND

####################################################################
# mac80211 option for use local ssv_mac80211 or kernel mac80211
# if set SSV means our smac driver use local ssv_mac80211.ko
# otherwise use kernel mac80211 build-in sub-sys
####################################################################
#MAC80211_OPT = SSV
#For HUW to define some resources
ifeq ($(MAC80211_OPT), SSV)
EXTRA_CFLAGS += -I$(KBUILD_TOP)/mac80211 
EXTRA_CFLAGS += -I$(KBUILD_TOP)/mac80211/include

ccflags-y += -DSSV_MAC80211
endif
#
