# SPDX -License-Identifier: BSD-3-Clause */

# Copyright (c) 2023 Rockchip Electronics Co., Ltd.

PROJECT := RK3562
SOC := RK3562
CPU = -mcpu=cortex-a53+crypto -mfloat-abi=hard

FIRMWARE_CPU_BASE ?= 0x02000000
DRAM_SIZE ?= 0x00800000
SRAM_BASE ?= 0xfe480000
SRAM_SIZE ?= 0x00010000
SHMEM_BASE ?= 0x07800000
SHMEM_SIZE ?= 0x00400000
LINUX_RPMSG_BASE ?= 0x07c00000
LINUX_RPMSG_SIZE ?= 0x00500000

CFLAGS += -DFIRMWARE_BASE=$(FIRMWARE_CPU_BASE) -DDRAM_SIZE=$(DRAM_SIZE) -DSRAM_BASE=$(SRAM_BASE) -DSRAM_SIZE=$(SRAM_SIZE) -DSHMEM_BASE=$(SHMEM_BASE) -DSHMEM_SIZE=$(SHMEM_SIZE) -DLINUX_RPMSG_BASE=$(LINUX_RPMSG_BASE) -DLINUX_RPMSG_SIZE=$(LINUX_RPMSG_SIZE)

# The shared memory is initialized by cpu1.
ifeq ($(CUR_CPU), 1)
        CFLAGS += -DPRIMARY_CPU
endif
ifeq ($(CUR_CPU), 0)
        CFLAGS += -DCPU0
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
include ../../common/GCC/Cortex-A.mk
