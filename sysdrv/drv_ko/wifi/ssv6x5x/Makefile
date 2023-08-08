KMODULE_NAME = ssv6x5x

ifeq ($(KERNEL_OBJ_PATH),)
KERNEL_OBJ_PATH := /lib/modules/`uname -r`/build
endif

#SSV_DRV_PATH = $(KERNEL_DIR)/drivers/net/wireless/rockchip_wlan/ssv6x5x

ifeq ($(SSV_DRV_PATH),)
SSV_DRV_PATH := $(PWD)/wifi/ssv6x5x
endif

KBUILD_TOP := $(SSV_DRV_PATH)
ifeq ($(MAKELEVEL),0)
KBUILD_TOP := .
endif

ifeq ($(ARCH),)
ARCH := x86
endif

ifeq ($(INSTALL_PATH),)
INSTALL_PATH := kernel/drivers/net/wireless/ssv6200
endif

include $(KBUILD_TOP)/config_common.mak
include $(KBUILD_TOP)/config.mak

MOD_DEF_H = include/ssv_mod_conf.h

# Generate version strings
# GEN_VER := $(shell cd $(KBUILD_TOP); ./ver_info.pl include/ssv_version.h)
# Generate include/ssv_conf_parser.h
# GEN_CONF_PARSER := $(shell cd $(KBUILD_TOP); env ccflags="$(ccflags-y)" ./parser-conf.sh include/ssv_conf_parser.h)
# Generate $(KMODULE_NAME)-wifi.cfg
#BKP_CFG := $(shell cp $(KBUILD_TOP)/$(PLATFORMS)-wifi.cfg $(KBUILD_TOP)/image/$(KMODULE_NAME)-wifi.cfg)

EXTRA_CFLAGS := -I$(KBUILD_TOP) -I$(KBUILD_TOP)/include

DEF_PARSER_H = $(KBUILD_TOP)/include/ssv_conf_parser.h
$(shell env ccflags="$(ccflags-y)" $(KBUILD_TOP)/parser-conf.sh $(DEF_PARSER_H))

#------------------------------
# ssvdevice/
KERN_SRCS := ssvdevice/ssvdevice.c
KERN_SRCS += ssvdevice/ssv_cmd.c

# hci/
KERN_SRCS += hci/ssv_hci.c

# smac/
KERN_SRCS += smac/init.c
KERN_SRCS += smac/dev.c
KERN_SRCS += smac/regd.c
KERN_SRCS += smac/wow.c
KERN_SRCS += smac/hw_scan.c

KERN_SRCS += smac/lib.c
KERN_SRCS += smac/ap.c

ifeq ($(findstring -DCONFIG_SSV6XXX_DEBUGFS, $(ccflags-y)), -DCONFIG_SSV6XXX_DEBUGFS)
KERN_SRCS += smac/ssv6xxx_debugfs.c
endif

KERN_SRCS += smac/efuse.c
KERN_SRCS += smac/ssv_skb.c
ifeq ($(findstring -DCONFIG_SSV_CTL, $(ccflags-y)), -DCONFIG_SSV_CTL)
KERN_SRCS += smac/ssv_wifi_ctl.c
endif

KERN_SRCS += smac/hal/hal.c
ifeq ($(findstring -DSSV_SUPPORT_SSV6006, $(ccflags-y)), -DSSV_SUPPORT_SSV6006)
KERN_SRCS += smac/hal/ssv6006c/ssv6006_common.c
KERN_SRCS += smac/hal/ssv6006c/ssv6006C_mac.c
KERN_SRCS += smac/hal/ssv6006c/ssv6006_phy.c
KERN_SRCS += smac/hal/ssv6006c/ssv6006_turismoC.c
endif

# hwif/hal/
KERN_SRCS += hwif/hal/hwif_hal.c
ifeq ($(findstring -DSSV_SUPPORT_SSV6006, $(ccflags-y)), -DSSV_SUPPORT_SSV6006)
KERN_SRCS += hwif/hal/ssv6006c/ssv6006C_hwif.c
endif

ifeq ($(findstring -DSSV_SUPPORT_SDIO, $(ccflags-y)), -DSSV_SUPPORT_SDIO)
# hwif/sdio/
KERN_SRCS += hwif/sdio/sdio.c
endif

ifeq ($(findstring -DSSV_SUPPORT_USB, $(ccflags-y)), -DSSV_SUPPORT_USB)
# hwif/usb
KERN_SRCS += hwif/usb/usb.c
endif

#------------------------------

KERN_SRCS += $(KMODULE_NAME)-generic-wlan.c

$(KMODULE_NAME)-y += $(KERN_SRCS_S:.S=.o)
$(KMODULE_NAME)-y += $(KERN_SRCS:.c=.o)

#obj-$(CONFIG_SSV6X5X) += $(KMODULE_NAME).o
obj-m += $(KMODULE_NAME).o

export CONFIG_SSV6X5X=m

.PHONY: all ver modules clean

all: $(MOD_DEF_H) modules

modules:
	#$(MAKE) -C $(KERNEL_DIR) M=$(SSV_DRV_PATH) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
	#$(MAKE) -C $(KERNEL_DIR) M=$(SSV_DRV_PATH) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules -j12
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules -j12
	cp $(SSV_DRV_PATH)/ssv6x5x.ko $(M_OUT_DIR)

strip:
	$(CROSS_COMPILE)strip $(KMODULE_NAME).ko --strip-unneeded

install:
	$(MAKE) INSTALL_MOD_DIR=$(INSTALL_PATH) -C $(KERNEL_OBJ_PATH) \
            M=$(SSV_DRV_PATH) modules_install
	depmod -a

clean:
	#$(MAKE) -C $(KERNEL_DIR) M=$(SSV_DRV_PATH) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules clean
	rm -fr *.mod.c *.mod *.o .*.cmd *.ko *~
	rm -fr .tmp_versions
	rm -fr Module.symvers
	rm -fr Module.markers
	rm -fr modules.order
	cd ssvdevice/; 		rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd hci/; 			rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd hci/ssv6006c/; 			rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd smac/; 	rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd smac/hal/; 	rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd smac/hal/ssv6006c/; 	rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd hwif/; 			rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd hwif/sdio/; 			rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd hwif/hal/; 			rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd hwif/hal/ssv6006c/; 			rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd hwif/usb/; 			rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	cd crypto/; 		rm -fr *.mod.c *.mod *.o .*.cmd *.ko
	rm -fr lib.a
	@rm -rf $(MOD_DEF_H)

$(MOD_DEF_H): config.mak config_common.mak
	#@echo "#ifndef __SSV_MOD_CONF_H__" > $@
	#@echo "#define __SSV_MOD_CONF_H__" >> $@
	#for flag in $(ccflags-y_no_dash); do \
	#   if [ "$$flag" =~ ^D.* ]; then \
	#       echo "#define $$flag" | sed -e s/D//g >> $@; \
	#   fi; \
	#done
	#echo "#endif // __SSV_MOD_CONF_H__" >> $@
	env ccflags="$(ccflags-y)" ./genconf.sh $@

