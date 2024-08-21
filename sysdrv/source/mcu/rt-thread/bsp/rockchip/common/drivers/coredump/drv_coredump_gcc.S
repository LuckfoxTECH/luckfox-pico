/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_coredump_gcc.S
  * @version V0.1
  * @brief   coredump
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-04-10     hhb           first implementation
  *
  ******************************************************************************
  */

.cpu cortex-m4
.syntax unified
.thumb
.text

.global rt_save_context_to
.type rt_save_context_to, %function
rt_save_context_to:
    PUSH {r0, r1}
    MOV r0, sp
    LDR sp, =cd_regs
    ADD sp, sp, 16*4
    MOV r1, pc
    PUSH {r1}
    MOV r1, lr
    PUSH {r1}
    ADD r1, r0, 8
    PUSH {r1}
    PUSH {r2-r12}
    MOV sp, r0
    POP {r0, r1}
    LDR r2, =cd_regs
    STR r0, [r2]
    STR r1, [r2, 4]
    MOV pc, lr