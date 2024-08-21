/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#if defined(HAL_SMCCC_MODULE_ENABLED)
#if defined(__GNUC__)

.globl HAL_SMCCC_Call

.section .text, "ax"
.type HAL_SMCCC_Call, % function

#ifdef __aarch64__
#define SMCCC_PARAM_X0_OFFS 0
#define SMCCC_PARAM_X2_OFFS 16
#define SMCCC_PARAM_X4_OFFS 32
#define SMCCC_PARAM_X6_OFFS 48

HAL_SMCCC_Call:
    stp x28, x30, [sp, #-16] !
    mov x28, x0
    ldp x0, x1, [x28, #SMCCC_PARAM_X0_OFFS]
    ldp x2, x3, [x28, #SMCCC_PARAM_X2_OFFS]
    ldp x4, x5, [x28, #SMCCC_PARAM_X4_OFFS]
    ldp x6, x7, [x28, #SMCCC_PARAM_X6_OFFS]
    smc #0
    stp x0, x1, [x28, #SMCCC_PARAM_X0_OFFS]
    stp x2, x3, [x28, #SMCCC_PARAM_X2_OFFS]
    ldp x28, x30, [sp], #16
    ret
.size HAL_SMCCC_Call, .- HAL_SMCCC_Call

#else
.align 2

HAL_SMCCC_Call:
    push    { r4 - r8, lr }
    mov r8, r0
    ldm r8, { r0 - r7 }
.arch_extension sec
    smc #0
    stm r8, { r0 - r7 }
    pop { r4 - r8, pc }
.size HAL_SMCCC_Call, .- HAL_SMCCC_Call
#endif

#endif /* __GNUC__ */
#endif /* HAL_SMCCC_MODULE_ENABLED */
