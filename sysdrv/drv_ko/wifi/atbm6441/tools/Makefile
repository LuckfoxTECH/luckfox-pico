TOOL_NAME:=atbm_iot_supplicant
TOOL_DEMO_NAME:=atbm_iot_supplicant_demo
TOOL_CLI_NAME:=atbm_iot_cli
MODULE_NAME:=power_ctrl

NOSTDINC_FLAGS :=

$(MODULE_NAME)-y = power_func.o

obj-m = $(MODULE_NAME).o

tool_cflags += -I./ -I./hal_apollo/ -lpthread

ifeq ($(CONFIG_ATBM_SDIO_ATCMD),y)
tool_cflags += -DCONFIG_ATBM_SDIO_ATCMD
endif

tools_install:
	$(CROSS_COMPILE)gcc tools/tools.c $(tool_cflags) -o $(TOOL_NAME)
	$(CROSS_COMPILE)gcc tools/tools.c $(tool_cflags) -DDEMO_TCP_SEND -o $(TOOL_DEMO_NAME)
	$(CROSS_COMPILE)gcc tools/tools_cli.c $(tool_cflags) -o $(TOOL_CLI_NAME)
	cp $(TOOL_NAME) driver_install/
	cp $(TOOL_DEMO_NAME) driver_install/
	cp $(TOOL_CLI_NAME) driver_install/
	
power_install: power_clean
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(shell pwd)/tools/ modules -j8
	cp tools/*.ko driver_install/
	
power_clean:
	rm -rf tools/*.ko
	rm -rf tools/*.o
	rm -rf tools/*.mod.c
	rm -rf tools/*.cmd
