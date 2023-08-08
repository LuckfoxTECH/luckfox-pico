##############################CONFIGURATION##########################
CFG_3518EV300 = n
CFG_T31 = y

HICHANNEL_DEBUG = n

###############################PLATFORM##############################
WLAN_CFLAGS +=-D_PRE_PLATFORM_JZ=1
WLAN_CFLAGS +=-D_PRE_PLATFORM_HISILICON=2
WLAN_CFLAGS +=-D_PRE_OS_PLATFORM=_PRE_PLATFORM_JZ

ifeq ($(HICHANNEL_DEBUG), y)
WLAN_CFLAGS += -D_PRE_HICHANNEL_DEBUG
endif

################################INCLUDE##############################
WLAN_CFLAGS += -I$(WLAN_DIR)/oal
WLAN_CFLAGS += -I$(WLAN_DIR)/wal
WLAN_CFLAGS += -I$(WLAN_DIR)/hcc
WLAN_CFLAGS += -I$(WLAN_DIR)/inc
WLAN_CFLAGS += -I$(WLAN_DIR)/oam
WLAN_CFLAGS += -I$(WLAN_DIR)/libsec

