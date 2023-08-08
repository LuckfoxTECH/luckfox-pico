ifeq ($(KBUILD_TOP),)
    ifneq ($(KBUILD_EXTMOD),)
    KBUILD_DIR := $(KBUILD_EXTMOD)
    else
    KBUILD_DIR := $(PWD)
    endif
KBUILD_TOP := $(KBUILD_DIR)/../../
endif

include $(KBUILD_TOP)/config.mak

KBUILD_EXTRA_SYMBOLS += $(KBUILD_TOP)/smac/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(KBUILD_TOP)/ssvdevice/Module.symvers

KMODULE_NAME=ssv6200_sdio_delay_tuning
KERN_SRCS := sdio_delay_tuning.c
KERN_SRCS += os_dep/linux/attach_linux.c
KERN_SRCS += os_dep/linux/sdio_linux.c

include $(KBUILD_TOP)/rules.mak

ccflags-y += -I$(KBUILD_DIR)/os_dep/linux -I$(KBUILD_DIR)/include
