CONFIG_SDIO_SUPPORT := y
CONFIG_SDIO_PWRCTRL := y
CONFIG_AIC_FW_PATH = "/oem/usr/ko/aic8800dc_fw"
#CONFIG_AIC_FW_PATH = "/lib/firmware/aic8800"
export CONFIG_AIC_FW_PATH
ccflags-y += -DCONFIG_AIC_FW_PATH=\"$(CONFIG_AIC_FW_PATH)\"

MODULE_NAME := aic8800_bsp
ifeq ($(CONFIG_SDIO_SUPPORT), y)
ccflags-y += -DAICWF_SDIO_SUPPORT
ccflags-$(CONFIG_SDIO_PWRCTRL) += -DCONFIG_SDIO_PWRCTRL
endif

CONFIG_GPIO_WAKEUP ?= n
CONFIG_M2D_OTA_AUTO_SUPPORT = n
CONFIG_M2D_OTA_LZMA_SUPPORT = n
CONFIG_LINK_DET_5G = y
CONFIG_MCU_MESSAGE = n
CONFIG_FIRMWARE_ARRAY = n
# Need to set fw path in BOARD_KERNEL_CMDLINE
CONFIG_USE_FW_REQUEST ?= n
CONFIG_FDRV_NO_REG_SDIO = n
CONFIG_VRF_DCDC_MODE = y
CONFIG_OOB ?= n
CONFIG_PREALLOC_TXQ = y
CONFIG_DPD = y
CONFIG_FORCE_DPD_CALIB = y
CONFIG_LOFT_CALIB = n
CONFIG_EXT_FEM_8800DCDW = n
CONFIG_RESV_MEM_SUPPORT ?= y
CONFIG_AMSDU_RX = y
CONFIG_IRQ_FALL ?= n
CONFIG_SDIO_BT = n
CONFIG_RADAR_OR_IR_DETECT =n
CONFIG_FOR_IPCAM = n

ifeq ($(CONFIG_EXT_FEM_8800DCDW), y)
CONFIG_DPD = n
CONFIG_FORCE_DPD_CALIB = n
CONFIG_LOFT_CALIB = y
endif

ccflags-$(CONFIG_GPIO_WAKEUP) += -DCONFIG_GPIO_WAKEUP
ccflags-$(CONFIG_M2D_OTA_AUTO_SUPPORT) += -DCONFIG_M2D_OTA_AUTO_SUPPORT
ccflags-$(CONFIG_M2D_OTA_LZMA_SUPPORT) += -DCONFIG_M2D_OTA_LZMA_SUPPORT
ccflags-$(CONFIG_LINK_DET_5G) += -DCONFIG_LINK_DET_5G
ccflags-$(CONFIG_MCU_MESSAGE) += -DCONFIG_MCU_MESSAGE
ccflags-$(CONFIG_FIRMWARE_ARRAY) += -DCONFIG_FIRMWARE_ARRAY
ccflags-$(CONFIG_USE_FW_REQUEST) += -DCONFIG_USE_FW_REQUEST
ccflags-$(CONFIG_FDRV_NO_REG_SDIO) += -DCONFIG_FDRV_NO_REG_SDIO
ccflags-$(CONFIG_VRF_DCDC_MODE) += -DCONFIG_VRF_DCDC_MODE
ccflags-$(CONFIG_OOB) += -DCONFIG_OOB
ccflags-$(CONFIG_PREALLOC_TXQ) += -DCONFIG_PREALLOC_TXQ
ccflags-$(CONFIG_DPD)  += -DCONFIG_DPD
ccflags-$(CONFIG_FORCE_DPD_CALIB) += -DCONFIG_FORCE_DPD_CALIB -DCONFIG_DPD
ccflags-$(CONFIG_LOFT_CALIB) += -DCONFIG_LOFT_CALIB
ccflags-$(CONFIG_EXT_FEM_8800DCDW) += -DCONFIG_EXT_FEM_8800DCDW
ccflags-$(CONFIG_RESV_MEM_SUPPORT) += -DCONFIG_RESV_MEM_SUPPORT
ccflags-$(CONFIG_AMSDU_RX) += -DCONFIG_AMSDU_RX
ccflags-$(CONFIG_IRQ_FALL) += -DCONFIG_IRQ_FALL
ccflags-$(CONFIG_SDIO_BT) += -DCONFIG_SDIO_BT
ccflags-$(CONFIG_RADAR_OR_IR_DETECT) += -DCONFIG_RADAR_OR_IR_DETECT
ccflags-$(CONFIG_FOR_IPCAM) += -DCONFIG_FOR_IPCAM


obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-y := \
	aic8800dc_compat.o \
	aic8800d80_compat.o \
	aic_bsp_main.o \
	aic_bsp_driver.o \
	aicsdio.o \
	aicsdio_txrxif.o \
	md5.o

$(MODULE_NAME)-$(CONFIG_PREALLOC_TXQ)     += aicwf_txq_prealloc.o

ifeq ($(CONFIG_FIRMWARE_ARRAY),y)
$(MODULE_NAME)-y +=	aicwf_firmware_array.o
endif

########## Platform support list ##########
CONFIG_PLATFORM_ROCKCHIP ?= n
CONFIG_PLATFORM_ROCKCHIP2 ?= n
CONFIG_PLATFORM_ALLWINNER ?=n
CONFIG_PLATFORM_INGENIC_T20 ?= n
CONFIG_PLATFORM_AMLOGIC ?= n
CONFIG_PLATFORM_UBUNTU ?= y

ifeq ($(CONFIG_PLATFORM_ROCKCHIP), y)
ccflags-$(CONFIG_PLATFORM_ROCKCHIP) += -DCONFIG_PLATFORM_ROCKCHIP
KDIR ?= /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/kernel
ARCH ?= arm64
CROSS_COMPILE ?= /home/yaya/E/Rockchip/3566/Android11/rk3566_rk3568_android11_oranth/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
endif

ifeq ($(CONFIG_PLATFORM_ROCKCHIP2), y)
ccflags-$(CONFIG_PLATFORM_ROCKCHIP2) += -DCONFIG_PLATFORM_ROCKCHIP2
ARCH ?= arm64
KDIR ?= /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/kernel
CROSS_COMPILE ?= /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
endif

ifeq ($(CONFIG_PLATFORM_ALLWINNER), y)
ccflags-$(CONFIG_PLATFORM_ALLWINNER) += -DCONFIG_PLATFORM_ALLWINNER
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
KDIR  ?= /home/aiden/D1/SDK/Amlogic/905x3_a9/android9.0/out/target/product/u202/obj/KERNEL_OBJ/
ARCH ?= arm
CROSS_COMPILE ?= /home/aiden/D1/SDK/Amlogic/905x3_a9/android9.0/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-
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

ccflags-y += -Wno-unused-function
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
	rm -rf *.o *.ko *.o.* *.mod.* modules.* Module.* .a* .o* .*.o.* *.mod .tmp* .cache.mk built-in.a

