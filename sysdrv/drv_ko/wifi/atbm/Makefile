#
#makefile for build atbm_wifi.ko
#
###############################################################################
#
# when release to customer ,the CUSTOMER_SUPPORT_USED must set to y!!!!!!!!!!!!!
#
###############################################################################
CUSTOMER_SUPPORT_USED=y
###############################################################################
#PLATFORM_XUNWEI    				1
#PLATFORM_SUN6I						2
#PLATFORM_FRIENDLY					3
#PLATFORM_SUN6I_64					4
#PLATFORM_SUN8I						5
#PLATFORM_HI3518E					6
#PLATFORM_X86PC         			7
#PLATFORM_AMLOGIC					8
#PLATFORM_AMLOGIC_905				8	
#PLATFORM_AMLOGIC_905X				9
#PLATFORM_ROCKCHIP      			10
#PLATFORM_MSTAR						11
#PLATFORM_CDLINUX					12
#PLATFORM_AMLOGIC_S805				13
#PLATFORM_HIS_LINUX_3_4				14
#PLATFORM_ROCKCHIP_3229				15
#PLATFORM_ROCKCHIP_3229_ANDROID8	16
#PLATFORM_HS_IPC					17
#PLATFORM_SIGMASTAR                 18
#PLATFORM_HI3516EV200               19
#PLATFORM_XUNWEI_2G                 20
#PLATFORM_NVT98517       			21
#PLATFORM_INGENICT31				22
#PLATFORM_INGENICT41				23


platform ?= PLATFORM_ROCKCHIP
#ATBM_BUILD_IN_KERNEL?=


ifeq ($(KERNELRELEASE),)
export
ATBM_BUILD_IN_KERNEL=n
export DRIVER_PATH ?= $(shell pwd)
ifeq ($(CUSTOMER_SUPPORT_USED),y)
$(info "hello makefile")
MAKEFILE_SUB ?= Makefile.build.customer
else
MAKEFILE_SUB ?= Makefile.build
endif

KERDIR ?= /usr/lchome/yuzhihuang/Mstar/IPC_I3/linux3.18_i3/
CROSS_COMPILE ?= /usr/lchome/yuzhihuang/Mstar/IPC_I3/arm-linux-gnueabihf-4.8.3-201404/bin/arm-linux-gnueabihf-
#Android
#Linux
sys ?= Linux
#arch:arm or arm64 or mips(NVT98517)
arch ?= arm

all:install

install:
	@echo "make PLATFORM_CROSS=$(platform)"
	#$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(shell pwd) modules -j12
	$(MAKE) all -f $(MAKEFILE_SUB) ARCH=$(ARCH)  CROSS_COMPILE=$(CROSS_COMPILE) KDIR=$(KERNEL_DIR) SYS=$(sys) PLAT=$(platform) -j8 O=$(WIFI_BUILD_KERNEL_OBJ_DIR)
	#arm-rockchip830-linux-uclibcgnueabihf-strip --strip-debug $(shell pwd)/driver_install/atbm603x_.ko
	cp $(shell pwd)/driver_install/atbm603x_wifi_usb.ko $(M_OUT_DIR)/atbm603x_.ko
clean:
	$(MAKE) -f $(MAKEFILE_SUB) KDIR=$(KERDIR) ARCH=$(arch) clean
strip:	
	$(MAKE) -f $(MAKEFILE_SUB) KDIR=$(KERDIR) ARCH=$(arch) SYS=$(sys) PLAT=$(platform) strip
get_ver:
	$(MAKE) -f $(MAKEFILE_SUB) KDIR=$(KERDIR) ARCH=$(arch) SYS=$(sys) PLAT=$(platform) get_ver
buid_config:
	$(MAKE) -C atbm_kconf clean
	$(MAKE) -C atbm_kconf mconf -f Makefile
menuconfig:buid_config
	@./atbm_kconf/mconf ./atbm_kconf/Kconfig



else

ifeq ($(platform),PLATFORM_XUNWEI)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=1
endif
ifeq ($(platform),PLATFORM_SUN6I)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=2
endif
ifeq ($(platform),PLATFORM_FRIENDLY)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=3
endif
ifeq ($(platform),PLATFORM_SUN6I_64)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=4
endif
ifeq ($(platform),PLATFORM_SUN8I)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=5
endif
ifeq ($(platform),PLATFORM_AMLOGIC_905)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=8
endif
ifeq ($(platform),PLATFORM_CDLINUX)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=12
endif
ifeq ($(platform),PLATFORM_AMLOGIC_S805)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=13
endif
ifeq ($(platform),PLATFORM_INGENICT31)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=22
endif
ifeq ($(platform),PLATFORM_INGENICT41)
export
ATBM_WIFI__EXT_CCFLAGS ?= -DATBM_WIFI_PLATFORM=23
endif



ifeq ($(ATBM_BUILD_IN_KERNEL),)

obj-m += hal_apollo/
export ATBM_BUILD_IN_KERNEL=y

else

export
include $(src)/Makefile.build.kernel

endif

endif

