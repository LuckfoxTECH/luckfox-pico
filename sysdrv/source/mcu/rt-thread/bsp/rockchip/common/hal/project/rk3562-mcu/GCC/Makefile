# SPDX-License-Identifier: BSD-3-Clause */

# Copyright (c) 2022 Rockchip Electronics Co., Ltd.

PROJECT		:= RK3562
SOC		:= RK3562
CPU		:= -mcpu=cortex-m0

# BUS M0
CFLAGS := -O2
LDFLAGS := --specs=nosys.specs
LINKER_SCRIPT := gcc_bus_m0.ld

# PMU M0
# CFLAGS := -Os
# LDFLAGS := --specs=nano.specs
# LINKER_SCRIPT := gcc_pmu_m0.ld

-include ../../../middleware/benchmark/benchmark.mk
-include ../../../middleware/rpmsg-lite/rpmsg-lite.mk
-include ../../../middleware/simple_console/simple_console.mk
include ../../common/GCC/Cortex-M.mk
