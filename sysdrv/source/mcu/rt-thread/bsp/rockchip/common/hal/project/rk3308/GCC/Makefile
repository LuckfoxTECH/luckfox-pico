# SPDX -License-Identifier: BSD-3-Clause */

# Copyright (c) 2022 Rockchip Electronics Co., Ltd.

PROJECT := RK3308
SOC := RK3308
CPU = -mcpu=cortex-a35+crypto -mfloat-abi=hard

FIRMWARE_CPU_BASE ?= 0x02600000
DRAM_SIZE ?= 0x00900000
SHMEM_BASE ?= 0x02f00000
SHMEM_SIZE ?= 0x00100000
SRAM_BASE ?= 0xfff88000
SRAM_SIZE ?= 0x00008000

SHRPMSG_SIZE ?= 0x00080000
SHRAMFS_SIZE ?= 0x00020000
SHLOG0_SIZE ?= 0x00001000
SHLOG1_SIZE ?= 0x00001000
SHLOG2_SIZE ?= 0x00001000
SHLOG3_SIZE ?= 0x00001000

CFLAGS += -DFIRMWARE_BASE=$(FIRMWARE_CPU_BASE) -DDRAM_SIZE=$(DRAM_SIZE) -DSHMEM_BASE=$(SHMEM_BASE) -DSHMEM_SIZE=$(SHMEM_SIZE)
CFLAGS += -DSRAM_BASE=$(SRAM_BASE) -DSRAM_SIZE=$(SRAM_SIZE)

CFLAGS += -DSHRPMSG_SIZE=$(SHRPMSG_SIZE) -DSHRAMFS_SIZE=$(SHRAMFS_SIZE)
CFLAGS += -DSHLOG0_SIZE=$(SHLOG0_SIZE) -DSHLOG1_SIZE=$(SHLOG1_SIZE) -DSHLOG2_SIZE=$(SHLOG2_SIZE) -DSHLOG3_SIZE=$(SHLOG3_SIZE)

# Define default IRQ route CPU
ifeq ("$(origin AMP_KERNEL_ENABLE)", "undefined")
CFLAGS += -DDEFAULT_IRQ_CPU=1
else
CFLAGS += -DDEFAULT_IRQ_CPU=0
CFLAGS += -DAMP_LINUX_ENABLE
endif

# Define the primary CPU
ifeq ("$(origin AMP_PRIMARY_CORE)", "undefined")
AMP_PRIMARY_CPU ?=1
else
AMP_PRIMARY_CPU ?=$(AMP_PRIMARY_CORE)
endif

# The shared memory is initialized by cpu1.

ifeq ($(CUR_CPU), $(AMP_PRIMARY_CPU))
        CFLAGS += -DPRIMARY_CPU
endif
ifeq ($(CUR_CPU), 0)
        CFLAGS += -DCPU0
endif
ifeq ($(CUR_CPU), 1)
        CFLAGS += -DCPU1
endif
ifeq ($(CUR_CPU), 2)
        CFLAGS += -DCPU2
endif
ifeq ($(CUR_CPU), 3)
        CFLAGS += -DCPU3
endif

LINKER_SCRIPT := gcc_arm.ld
CLEAN_FILES += $(LINKER_SCRIPT)

-include ../../../middleware/benchmark/benchmark.mk
-include ../../../middleware/rpmsg-lite/rpmsg-lite.mk
include ../../common/GCC/Cortex-A.mk
