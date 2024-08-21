/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#if defined(HAL_PM_CPU_SLEEP_MODULE_ENABLED)
#include "soc.h"

#if defined(__CM3_REV) || defined(__CM4_REV)
#if defined(__GNUC__)

.thumb
.syntax unified

.global HAL_CPU_SuspendEnter
.global HAL_CPU_ArchSuspend
.global HAL_CPU_ArchResume
.global HAL_CPU_DoResume

.equ CPU_SUSPEND_SIZE, 4 * 6

.section .text, "ax"
.align  2

.type HAL_CPU_ArchSuspend, % function
HAL_CPU_ArchSuspend:
    stmfd sp !, {r4 - r10, lr}
    mrs r4, PSP
    mrs r5, CONTROL
    mrs r6, MSP
    stmia r0 !, {r4 - r6}
    mrs r4, BASEPRI
    mrs r5, PRIMASK
    mrs r6, FAULTMASK
    ldr r8, =0xE000ED08
    ldr r7, [r8]
    stmia r0 !, {r4 - r7}
    ldmfd sp !, {r4 - r10, pc}
.size HAL_CPU_ArchSuspend, .- HAL_CPU_ArchSuspend

.type HAL_CPU_ArchResume, % function
HAL_CPU_ArchResume:
    ldmia r0 !, {r4 - r6}
    msr CONTROL, r5
    msr PSP, r4
    msr MSP, r6
    ldmia r0 !, {r4 - r7}
    ldr r8, =0xE000ED08
    str r7, [r8]
    msr BASEPRI, r4
    msr PRIMASK, r5
    msr FAULTMASK, r6
    mov sp, r0
    ldmfd sp !, {r4 - r12, pc}
.size HAL_CPU_ArchResume, .- HAL_CPU_ArchResume

.type HAL_CPU_SuspendEnter, % function
HAL_CPU_SuspendEnter:
    stmfd sp !, {r4 - r12, lr}
    ldr r4, = CPU_SUSPEND_SIZE
     mov r5, sp
    add r4, r4, #12
    sub sp, sp, r4
    stmfd sp !, {r0, r1}
    add r0, sp, #8
    mov r1, r4
    mov r2, r5
    ldr r3, = sleepSaveSp
    bl HAL_CPU_SuspendSave
    adr lr, CpuSuspendAbort
    ldmfd sp !, {r0, pc}
CpuSuspendAbort:
    ldmia sp !, {r1 - r2}
    mov r0, #1
    mov sp, r2
    ldmfd sp !, {r4 - r12, pc}
.size HAL_CPU_SuspendEnter, .- HAL_CPU_SuspendEnter

.type HAL_CPU_DoResume, % function
HAL_CPU_DoResume:
    ldr r0, sleepSaveSp
    ldmia r0 !, {r1, r2}
    bx r2
.size HAL_CPU_DoResume, .- HAL_CPU_DoResume
.ltorg
sleepSaveSp:
.word   0
#endif /* __GNUC__ */
#endif /* __CM3_REV || __CM4_REV */
#endif /* HAL_PM_CPU_SLEEP_MODULE_ENABLED */
