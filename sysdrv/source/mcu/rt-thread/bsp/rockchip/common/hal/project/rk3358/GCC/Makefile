# SPDX -License-Identifier: BSD-3-Clause */

# Copyright (c) 2023 Rockchip Electronics Co., Ltd.

PROJECT := RK3358
SOC := RK3358
CPU = -mcpu=cortex-a35+crypto -mfloat-abi=hard

FIRMWARE_CPU_BASE ?= 0x02600000
DRAM_SIZE ?= 0x00900000
SHMEM_BASE ?= 0x02f00000
SHMEM_SIZE ?= 0x00100000

CFLAGS += -DFIRMWARE_BASE=$(FIRMWARE_CPU_BASE) -DDRAM_SIZE=$(DRAM_SIZE) -DSHMEM_BASE=$(SHMEM_BASE) -DSHMEM_SIZE=$(SHMEM_SIZE)

# The shared memory is initialized by cpu1.
ifeq ($(CUR_CPU), 1)
        CFLAGS += -DPRIMARY_CPU -DNC_MEM_BASE=$(NC_MEM_BASE) -DNC_MEM_SIZE=$(NC_MEM_SIZE)
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
-include ../../../middleware/rpmsg-lite/rpmsg-lite.mk
include ../../common/GCC/Cortex-A.mk
