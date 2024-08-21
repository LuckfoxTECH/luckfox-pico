# SPDX-License-Identifier: BSD-3-Clause */

# Copyright (c) 2023 Rockchip Electronics Co., Ltd.

PROJECT		:= RK3588
SOC		:= RK3588
CPU		:= -mcpu=cortex-m0

CFLAGS := -O2
LDFLAGS := --specs=nosys.specs
LINKER_SCRIPT := gcc_m0.ld

-include ../../../middleware/benchmark/benchmark.mk
include ../../common/GCC/Cortex-M.mk
