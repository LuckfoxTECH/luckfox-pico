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
#endif /* HAL_MCU_CORE */
