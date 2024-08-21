ifeq ($(SYSDRV_PARAM), )
	SYSDRV_PARAM:=../../Makefile.param
	include $(SYSDRV_PARAM)
endif

ifneq ($(SYSDRV_KERNEL_OBJS_OUTPUT_DIR),)
WIFI_BUILD_KERNEL_OBJ_DIR=$(SYSDRV_KERNEL_OBJS_OUTPUT_DIR)
else
WIFI_BUILD_KERNEL_OBJ_DIR=$(KERNEL_DIR)
endif

ifeq ($(ENABLE_WIFI),y)

WIFI_USB = $(shell grep -o "CONFIG_USB=y" $(WIFI_BUILD_KERNEL_OBJ_DIR)/.config)
WIFI_SDIO = $(shell grep -o "CONFIG_MMC=y" $(WIFI_BUILD_KERNEL_OBJ_DIR)/.config)
WIFI_RFKILL = $(shell grep -o "CONFIG_RFKILL=y" $(WIFI_BUILD_KERNEL_OBJ_DIR)/.config)
export WIFI_RFKILL WIFI_BUILD_KERNEL_OBJ_DIR

ifneq ($(WIFI_USB),)
	PKG_TARGET := build-usb
	PKG_CLEAN_TARGET += build-usb-clean
endif

ifneq ($(WIFI_SDIO),)
	PKG_TARGET += build-sdio
	PKG_CLEAN_TARGET += build-sdio-clean
endif

endif

ifeq ($(M_OUT_DIR),)
# TODO
$(error Please build on upper directory)
endif

all: $(PKG_TARGET)
	@echo "build wifi done"
	@echo "build wifi $(WIFI_RFKILL)"
ifeq ($(ENABLE_WIFI),y)
	@cp insmod_wifi.sh $(M_OUT_DIR);
endif

ifneq ($(RK_ENABLE_WIFI_CHIP),)

build-usb:
ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"RTL8188FTV"),)
	@make -C rtl8188ftv/
endif

ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"SSV6X5X"),)
	@make -C ssv6x5x/
endif

ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"SSV6115"),)
	@make -C ssv6115/
endif

ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"AIC8800DW_USB"),)
	@make -C aic8800_usb/
endif

ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"AIC8800MC"),)
	@make -C aic8800_netdrv/
endif

build-sdio:
ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"RTL8189FS"),)
	@make -C rtl8189fs/
endif

ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"HI3861L"),)
	@make -C hichannel/
endif

ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"ATBM603X"),)
	@make -C atbm/
endif

ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"ATBM6441"),)
	@make -C atbm6441/
endif

ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"AIC8800DC"),)
	@make -C aic8800dc/
endif

ifneq ($(WIFI_RFKILL),)
ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"AP6XXX"),)
	@make -C bcmdhd/
endif
endif

else

build-usb:
	@make -C rtl8188ftv/
	@make -C ssv6x5x/
	@make -C ssv6115/
	@make -C aic8800_usb/

build-sdio:
	@make -C rtl8189fs/
	@make -C hichannel/
	@make -C atbm/
	@make -C atbm6441/
	@make -C aic8800dc/
ifneq ($(WIFI_RFKILL),)
	@make -C bcmdhd/
endif
	@make -C aic8800_netdrv/

endif

build-sdio-clean:
	@make -C rtl8189fs/ clean
	@make -C bcmdhd/ clean
	@make -C hichannel/ clean
	@make -C atbm/ clean
	@make -C atbm6441/ clean
	@make -C aic8800_netdrv/ clean

build-usb-clean:
	@make -C rtl8188ftv clean
	@make -C ssv6x5x clean
	@make -C ssv6115 clean
	@make -C aic8800_usb clean

clean: $(PKG_CLEAN_TARGET)
