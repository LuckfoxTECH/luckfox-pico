# SPDX-License-Identifier: BSD-3-Clause */

# Copyright (c) 2022 Rockchip Electronics Co., Ltd.

PROJECT		:= RK3528
SOC		:= RK3528
CPU		:= -mcpu=cortex-m0

CFLAGS := -O2
LDFLAGS := --specs=nosys.specs
LINKER_SCRIPT := gcc_m0.ld

-include ../../../middleware/benchmark/benchmark.mk
include ../../common/GCC/Cortex-M.mk
