
CONFIG_SYSDRV_CHIP:=rv1106

CONFIG_SYSDRV_CROSS := arm-rockchip830-linux-uclibcgnueabihf

################################################
# Configure for emmc
################################################
ifeq ($(BOOT_MEDIUM),emmc)
ifeq ($(KERNEL_DTS),)
KERNEL_DTS := rv1106g-evb1-v10.dts
endif

ifeq ($(KERNEL_CFG),)
KERNEL_CFG := rv1106_defconfig
ifeq ($(KERNEL_CFG_FRAGMENT),)
KERNEL_CFG_FRAGMENT := rv1106-evb.config
endif
endif

ifeq ($(UBOOT_CFG),)
UBOOT_CFG := rv1106_defconfig
ifeq ($(UBOOT_CFG_FRAGMENT),)
UBOOT_CFG_FRAGMENT :=
endif
endif

ifeq ($(CONFIG_SYSDRV_PARTITION),)
CONFIG_SYSDRV_PARTITION := 32K(env),512K@32K(idblock),4M(uboot),32M(boot),2G(rootfs),1G(oem),2G(userdata),-(media)
endif
endif
################################################

ifeq ($(BOOT_MEDIUM),spi_nor)
ifeq ($(CONFIG_SYSDRV_PARTITION),)
CONFIG_SYSDRV_PARTITION := 64K(env),128K@64K(idblock),128K(uboot),2M(boot),2M(rootfs),2M(userdata),-(media)
endif

ifeq ($(KERNEL_DTS),)
KERNEL_DTS := rv1106g-38x38-ipc-v10.dts
endif

ifeq ($(KERNEL_CFG),)
KERNEL_CFG := rv1106_defconfig
ifeq ($(KERNEL_CFG_FRAGMENT),)
KERNEL_CFG_FRAGMENT := rv1106-ipc.config
endif
endif

ifeq ($(UBOOT_CFG),)
UBOOT_CFG := rv1106-spi-nor_defconfig
ifeq ($(UBOOT_CFG_FRAGMENT),)
UBOOT_CFG_FRAGMENT :=
endif
endif

endif
################################################

ifeq ($(BOOT_MEDIUM),spi_nand)
ifeq ($(CONFIG_SYSDRV_PARTITION),)
CONFIG_SYSDRV_PARTITION := 256K(env),256K@256K(idblock),1M(uboot),8M(boot),64M(rootfs),32M(userdata),-(media)
endif

ifeq ($(KERNEL_DTS),)
KERNEL_DTS := rv1106g-evb1-v10-spi-nand.dts
endif

ifeq ($(KERNEL_CFG),)
KERNEL_CFG := rv1106_defconfig
ifeq ($(KERNEL_CFG_FRAGMENT),)
KERNEL_CFG_FRAGMENT := rv1106-evb.config rv1106_spinand.config
endif
endif

ifeq ($(UBOOT_CFG),)
UBOOT_CFG := rv1106_defconfig
ifeq ($(UBOOT_CFG_FRAGMENT),)
UBOOT_CFG_FRAGMENT :=
endif
endif

endif

################################################
##   Public Configuraton
################################################
TINY_ROOTFS_BUSYBOX_CFG := config_tiny_arm
