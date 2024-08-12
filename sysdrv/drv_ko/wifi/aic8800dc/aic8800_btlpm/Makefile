CONFIG_AIC8800_BTLPM_SUPPORT = m

obj-$(CONFIG_AIC8800_BTLPM_SUPPORT) := aic8800_btlpm.o

ccflags-y += -I$(srctree)/$(src)/../aic8800_bsp

# Platform support list
CONFIG_PLATFORM_ROCKCHIP ?= n
CONFIG_PLATFORM_ROCKCHIP2 ?= n
CONFIG_PLATFORM_ALLWINNER ?= n
CONFIG_PLATFORM_AMLOGIC ?= n
CONFIG_PLATFORM_UBUNTU ?= y


CONFIG_SUPPORT_LPM ?= n
CONFIG_AUTO_PM ?= n

aic8800_btlpm-y := \
	aic_bluetooth_main.o \
	rfkill.o \

aic8800_btlpm-$(CONFIG_SUPPORT_LPM) += lpm.o

ccflags-y += -DAIC_TRACE_INCLUDE_PATH=$(src)

ccflags-$(CONFIG_AUTO_PM) += -DCONFIG_AUTO_PM

ifeq ($(CONFIG_PLATFORM_ROCKCHIP), y)
ARCH = arm64
KDIR = /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/kernel
CROSS_COMPILE = /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
ccflags-$(CONFIG_PLATFORM_ROCKCHIP) += -DCONFIG_PLATFORM_ROCKCHIP
ccflags-y += -DANDROID_PLATFORM
endif

ifeq ($(CONFIG_PLATFORM_ROCKCHIP2), y)
ARCH = arm64
KDIR = /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/kernel
CROSS_COMPILE = /home/yaya/E/Rockchip/3566/firefly/Android11.0/Firefly-RK356X_Android11.0_git_20210824/RK356X_Android11.0/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
ccflags-$(CONFIG_PLATFORM_ROCKCHIP2) += -DCONFIG_PLATFORM_ROCKCHIP2
ccflags-y += -DANDROID_PLATFORM
endif

ifeq ($(CONFIG_PLATFORM_ALLWINNER), y)
ccflags-$(CONFIG_PLATFORM_ALLWINNER) += -DCONFIG_PLATFORM_ALLWINNER
KDIR  ?= /home/yaya/E/Allwinner/R818/R818/AndroidQ/lichee/kernel/linux-4.9/
ARCH ?= arm64
CROSS_COMPILE ?= /home/yaya/E/Allwinner/R818/R818/AndroidQ/android/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-
ccflags-y += -DANDROID_PLATFORM
endif

ifeq ($(CONFIG_PLATFORM_AMLOGIC), y)
ccflags-$(CONFIG_PLATFORM_NANOPI) += -DCONFIG_PLATFORM_NANOPI
ccflags-y += -DANDROID_PLATFORM
endif

ifeq ($(CONFIG_PLATFORM_UBUNTU), y)
KDIR  ?= /lib/modules/$(shell uname -r)/build
PWD   ?= $(shell pwd)
KVER ?= $(shell uname -r)
MODDESTDIR ?= /lib/modules/$(KVER)/kernel/drivers/net/wireless/
ARCH ?= x86_64
CROSS_COMPILE ?=
endif


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


