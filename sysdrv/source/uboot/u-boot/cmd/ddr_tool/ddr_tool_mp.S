/* SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause */
/*
 * Copyright (C) 2023 Rockchip Electronics Co., Ltd.
 */

	.global __sp

	.global secondary_main
	.global secondary_init
	.global get_cpu_id
	.global lock_byte_mutex
	.global unlock_byte_mutex

#ifndef CONFIG_ARM64
	.align	7
vectors_2:
	ldr	pc, =die_loop	/* reset */
	ldr	pc, =die_loop	/* undefine */
	ldr	pc, =die_loop	/* swi */
	ldr	pc, =die_loop	/* iabort */
	ldr	pc, =die_loop	/* dabort */
	ldr	pc, =die_loop	/* reserved */
	ldr	pc, =die_loop	/* irq */
	ldr	pc, =die_loop	/* fiq */

	.align	7
die_loop:
	b	die_loop

	.align	7
	.type	secondary_init, %function
secondary_init:
	bl	irq_disable
	bl	icache_invalid

	/* set sp */
	ldr	r2, =__sp
	ldr	r1, [r2]
	bic	r1, r1, #0xf
	mov	sp, r1
	bl	icache_invalid

	mrc	p15, 0, r0, c1, c0, 0	/* CP15 C1 System Control Register */
	bic	r0, r0, #0x2000		/* clear V (bit[13], VBAR) */
	mcr	p15, 0, r0, c1, c0, 0	/* for remap VBAR */
	ldr	r0, =vectors_2
	mcr	p15, 0, r0, c12, c0, 0
	bl	icache_invalid

	b	secondary_main

irq_disable:
	mrs	r0, cpsr
	orr	r0, r0, #0xc0
	msr	cpsr, r0
	bx	lr

icache_invalid:
	mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0
	bx	lr

	.type	get_cpu_id, %function
get_cpu_id:
	mrc	p15, 0, r0, c0, c0, 5
	and	r0, r0, #0x3
	bx	lr

	.align	7
	.type	lock_byte_mutex, %function
lock_byte_mutex:
	mov	r2, #0x1
try:
	ldrex	r1, [r0]
	cmp	r1, #0
	strexeq	r1, r2, [r0]
	cmpeq	r1, #0
	bne	try
	dmb
	bx	lr

	.align	7
	.type	unlock_byte_mutex, %function
unlock_byte_mutex:
	dmb
	mov	r1, #0
	str	r1, [r0]
	dsb
	bx	lr
#else /* CONFIG_ARM64 */
	.align	7
el2_vectors:
synchronous_sp0:
	b	secondary_init
	.align	7
irq_sp0:
	b	irq_sp0
	.align	7
fiq_sp0:
	b	fiq_sp0
	.align	7
serror_sp0:
	b	serror_sp0
	.align	7
synchronous_spx:
	b	synchronous_spx
	.align	7
irq_spx:
	b	irq_spx
	.align	7
fiq_spx:
	b	fiq_spx
	.align	7
serror_spx:
	b	serror_spx

	.align	7
	.type	secondary_init, %function
secondary_init:
	bl	irq_disable

	/* set sp */
	ldr	x2, =__sp
	ldr	x1, [x2]
	bic	x1, x1, #0xf
	mov	sp, x1
	bl	icache_invalid

	ldr	w0, =el2_vectors
	msr	vbar_el2, x0
	bl	icache_invalid

	bl	secondary_main

	.type	irq_disable, %function
irq_disable:
	msr	daifset, #0x3
	ic	iallu
	ret

	.type	icache_invalid, %function
icache_invalid:
	ic	iallu
	ret

	.align	7
	.type	lock_byte_mutex, %function
lock_byte_mutex:
	ldxrb	w1, [x0]
	cmp	w1, #1
	bne	1f
	wfe
	b	lock_byte_mutex
1:
	mov	x1, #1
	stxrb	w2, w1, [x0]
	cmp	w2, #0
	bne	lock_byte_mutex
	dmb	sy
	ret

	.align	7
	.type	unlock_byte_mutex, %function
unlock_byte_mutex:
	dmb	sy
	mov	x1, #0
	strb	w1, [x0]
	dsb	sy
	sev
	ret
#endif	/* #ifdef CONFIG_ARM */
