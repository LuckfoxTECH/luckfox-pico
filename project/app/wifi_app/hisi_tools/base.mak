#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#       config
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CURDIR ?= $(shell pwd)
SDK_DIR := $(CURDIR)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#       config
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFG_HI_TOOLCHAINS_NAME = ~/rv1109_battery_ipc/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf
CFG_HI_TOOLCHAINS_NAME = $(SYSDRV_CROSS)
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#       TOOL CHAINS
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
AR=${CFG_HI_TOOLCHAINS_NAME}-ar
AS=${CFG_HI_TOOLCHAINS_NAME}-as
LD=${CFG_HI_TOOLCHAINS_NAME}-ld
CPP=${CFG_HI_TOOLCHAINS_NAME}-cpp
CC=${CFG_HI_TOOLCHAINS_NAME}-gcc
CXX=${CFG_HI_TOOLCHAINS_NAME}-g++
NM=${CFG_HI_TOOLCHAINS_NAME}-nm
STRIP=${CFG_HI_TOOLCHAINS_NAME}-strip
OBJCOPY=${CFG_HI_TOOLCHAINS_NAME}-objcopy
OBJDUMP=${CFG_HI_TOOLCHAINS_NAME}-objdump
CFG_HI_BASE_ENV+=" AR AS LD CPP CC NM STRIP OBJCOPY OBJDUMP "
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#       COMPILE TOOLS
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CROSS_COMPILE=$(CFG_HI_TOOLCHAINS_NAME)-
CFG_HI_BASE_ENV+=" CROSS_COMPILE "
