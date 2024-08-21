WLAN_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

include $(WLAN_DIR)/env_config.mk

ifeq ($(CFG_3518EV300), y)
ARCH ?= arm
KDIR ?= /home/shell/linux-4.9.y
CROSS_COMPILE ?= arm-himix100-linux-
endif

#ARCH ?= arm
#KDIR ?= ~/rv1109_battery_ipc/kernel/
#CROSS_COMPILE ?= ~/rv1109_battery_ipc/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
#ARCH ?= arm64
#KDIR ?= ~/rk3588/kernel/
#CROSS_COMPILE ?= /home/xy/rk3588/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-rockchip1031-linux-gnu-

ARCH=arm
CROSS_COMPILE=arm-rockchip830-linux-uclibcgnueabihf-

ifneq ($(WIFI_RFKILL),)
WLAN_CFLAGS += -DRFKILL_RK
endif

ifeq ($(CFG_T31), y)
ARCH ?= mips
KDIR ?= /home/shell/kernel_3861l
CROSS_COMPILE ?= mips-linux-uclibc-gnu-
endif

MAIN_PATH :=
OAL_PATH := oal/
OAM_PATH := oam/
WAL_PATH := wal/
HCC_PATH := hcc/
SEC_PATH := libsec/
SDIO_PATH := sdio/

WLAN_CFLAGS += -std=gnu99 -Wno-declaration-after-statement -fsigned-char -freg-struct-return -Wtrampolines -Wfloat-equal
#WLAN_CFLAGS += -std=gnu99 -Wno-declaration-after-statement -fsigned-char -freg-struct-return -Wtrampolines -Wfloat-equal -fvisibility=hidden

ifeq ($(CFG_3518EV300), y)
WLAN_CFLAGS += -Wdate-time
endif
WLAN_CFLAGS += -I$(WLAN_DIR)
WLAN_CFLAGS += -I$(WLAN_DIR)hcc
WLAN_CFLAGS += -I$(WLAN_DIR)ipcm
WLAN_CFLAGS += -I$(WLAN_DIR)include
WLAN_CFLAGS += -I$(WLAN_DIR)libsec
WLAN_CFLAGS += -I$(WLAN_DIR)sdio
WLAN_CFLAGS += -I$(WLAN_DIR)oal
WLAN_CFLAGS += -I$(WLAN_DIR)oam
WLAN_CFLAGS += -I$(WLAN_DIR)uart

main-objs += main.o
main-objs := $(addprefix $(MAIN_PATH),$(main-objs))

oal-objs += oal_util.o oal_netbuf.o
oal-objs := $(addprefix $(OAL_PATH),$(oal-objs))

oam-objs  += oam_main.o oam_log.o
oam-objs := $(addprefix $(OAM_PATH),$(oam-objs))

wal-objs += wal_net.o wal_netlink.o
wal-objs := $(addprefix $(WAL_PATH),$(wal-objs))

hcc-objs := hcc_host.o hcc_task.o hcc_adapt.o
hcc-objs := $(addprefix $(HCC_PATH),$(hcc-objs))

sdio-objs := sdio_host.o
sdio-objs := $(addprefix $(SDIO_PATH),$(sdio-objs))

sec-objs += memcpy_s.o memmove_s.o memset_s.o strcat_s.o strcpy_s.o strncat_s.o strncpy_s.o strtok_s.o \
            snprintf_s.o vsnprintf_s.o secureprintoutput_a.o
sec-objs := $(addprefix $(SEC_PATH),$(sec-objs))

WLAN_OBJS += $(main-objs) $(oal-objs) $(oam-objs) $(wal-objs) $(hcc-objs) $(sec-objs) $(sdio-objs)

EXTRA_CFLAGS += $(WLAN_CFLAGS)
PWD := $(shell pwd)
USER_MODULE_NAME := hichannel
MODULE_NAME := $(USER_MODULE_NAME)
$(MODULE_NAME)-y += $(WLAN_OBJS)
obj-m = $(MODULE_NAME).o

#==================================================================
#                          modules
#==================================================================
.PHONY: all sample sample_clean clean

all:
	#make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) modules
	#$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules -j12
	@echo "build wifi $(WIFI_RFKILL)"
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(shell pwd) modules -j12 O=$(WIFI_BUILD_KERNEL_OBJ_DIR)
	$(CROSS_COMPILE)strip --strip-debug $(shell pwd)/hichannel.ko
	cp $(shell pwd)/hichannel.ko $(M_OUT_DIR)

sample:
	$(MAKE) -C sample all
    
sample_clean:
	$(MAKE) -C sample clean
	
clean:
	rm -rf $(COBJS) $(MODULE_NAME).o $(MODULE_NAME).ko
	rm -rf ./*.ko
	rm -rf ./*.o
	rm -rf ./.*.cmd
	rm -rf ./*.symvers
	rm -rf ./*.order
	rm -rf ./*.mod.c
	rm -rf ./oal/*.o
	rm -rf ./oal/.*.cmd
	rm -rf ./wal/*.o
	rm -rf ./wal/.*.cmd
	rm -rf ./libsec/*.o
	rm -rf ./libsec/.*.cmd
	rm -rf ./hcc/*.o
	rm -rf ./hcc/.*.cmd
	rm -rf ./oam/*.o
	rm -rf ./oam/.*.cmd
	rm -rf ./sdio/*.o
	rm -rf ./sdio/.*.cmd
	rm -rf .tmp_versions
