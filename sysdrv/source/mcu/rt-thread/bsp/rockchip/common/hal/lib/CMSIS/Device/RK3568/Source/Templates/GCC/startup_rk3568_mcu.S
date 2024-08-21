/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_MCU_CORE

#include "riscv_csr_encoding.h"

# define LREG     lw
# define SREG     sw
# define REGBYTES 4

.globl _start
.weak sc_exit, SystemInit, entry, main
.text
/* skip supervisor & hypervisor & user vectors */
j _start
.org(64 * 4 - 2 + 64 * 3)
.align 6

machine_trap_entry:
j sc_exit
vec_reserved1:
j sc_exit
vec_reserved2:
j sc_exit
vec_machine_soft_int_entry:
j sc_exit
vec_reserved4:
j sc_exit
vec_reserved5:
j sc_exit
vec_reserved6:
j sc_exit
vec_machine_timer_int_entry:
j sc_exit
vec_reserved8:
j sc_exit
vec_reserved9:
j sc_exit
vec_reserved10:
j sc_exit
vec_machine_ext_int_entry:
j sc_exit
vec_reserved12:
j sc_exit
vec_reserved13:
j sc_exit
vec_reserved14:
j sc_exit
vec_reserved15:
j sc_exit

.align 6

_start:
# clear bss
la a1, __BSS_START__
la a2, __BSS_END__
j       4f
3 :
sw zero, 0 (a1)
add a1, a1, 4
4 :
bne a1, a2, 3b
sw zero, 0 (a1)

auipc gp, % hi(_gp)
addi gp, gp, % lo(_gp)
la sp, __C_STACK_TOP__

li a0, 0
li a1, 0
call SystemInit
call entry
call main
j sc_exit

.globl _wfi
.type _wfi, @function
_wfi:
wfi
ret
.globl _fence
.type _fence, @function
_fence:
fence
ret

SystemInit:
entry:
ret

sc_exit:
j sc_exit

.section .text.entry
.align 6

.global scr1_trap_entry

scr1_trap_entry:
addi sp, sp, -32 * 4
sw x1, 1 * 4 (sp)
sw x4, 4 * 4 (sp)
sw x5, 5 * 4 (sp)
sw x6, 6 * 4 (sp)
sw x7, 7 * 4 (sp)
sw x8, 8 * 4 (sp)
sw x9, 9 * 4 (sp)
sw x10, 10 * 4 (sp)
sw x11, 11 * 4 (sp)
sw x12, 12 * 4 (sp)
sw x13, 13 * 4 (sp)
sw x14, 14 * 4 (sp)
sw x15, 15 * 4 (sp)
sw x16, 16 * 4 (sp)
sw x17, 17 * 4 (sp)
sw x18, 18 * 4 (sp)
sw x19, 19 * 4 (sp)
sw x20, 20 * 4 (sp)
sw x21, 21 * 4 (sp)
sw x22, 22 * 4 (sp)
sw x23, 23 * 4 (sp)
sw x24, 24 * 4 (sp)
sw x25, 25 * 4 (sp)
sw x26, 26 * 4 (sp)
sw x27, 27 * 4 (sp)
sw x28, 28 * 4 (sp)
sw x29, 29 * 4 (sp)
sw x30, 30 * 4 (sp)
sw x31, 31 * 4 (sp)
li t0, 0x80
sw t0, 2 * 4 (sp)
/* switch to interrupt stack */
move s0, sp
la sp, _sp
csrr a0, mcause
csrr a1, mepc
mv a2, s0
call HAL_RISCVIC_IRQHandler
/* switch main stack */
move sp, s0
/* Remain in M-mode after mret */
li t0, 0x00001800
csrs mstatus, t0
lw t0, 2 * 4 (sp)
csrs mstatus, t0
lw x1, 1 * 4 (sp)
lw x4, 4 * 4 (sp)
lw x5, 5 * 4 (sp)
lw x6, 6 * 4 (sp)
lw x7, 7 * 4 (sp)
lw x8, 8 * 4 (sp)
lw x9, 9 * 4 (sp)
lw x10, 10 * 4 (sp)
lw x11, 11 * 4 (sp)
lw x12, 12 * 4 (sp)
lw x13, 13 * 4 (sp)
lw x14, 14 * 4 (sp)
lw x15, 15 * 4 (sp)
lw x16, 16 * 4 (sp)
lw x17, 17 * 4 (sp)
lw x18, 18 * 4 (sp)
lw x19, 19 * 4 (sp)
lw x20, 20 * 4 (sp)
lw x21, 21 * 4 (sp)
lw x22, 22 * 4 (sp)
lw x23, 23 * 4 (sp)
lw x24, 24 * 4 (sp)
lw x25, 25 * 4 (sp)
lw x26, 26 * 4 (sp)
lw x27, 27 * 4 (sp)
lw x28, 28 * 4 (sp)
lw x29, 29 * 4 (sp)
lw x30, 30 * 4 (sp)
lw x31, 31 * 4 (sp)
addi sp, sp, 32 * 4
mret

.weak HAL_RISCVIC_IRQHandler
HAL_RISCVIC_IRQHandler:
1 :
j 1b
#endif /* HAL_MCU_CORE */
