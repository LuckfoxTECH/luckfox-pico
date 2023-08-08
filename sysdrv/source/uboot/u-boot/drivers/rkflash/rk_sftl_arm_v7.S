/*
 * Copyright (c) 2018 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:    GPL-2.0
 * date: 2022-08-15
 */
	.arch armv7-a
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 4
	.eabi_attribute 34, 0
	.eabi_attribute 18, 2
	.file	"rk_sftl.c"
	.global	__aeabi_uidiv
	.section	.text.l2p_addr_tran.isra.0,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	l2p_addr_tran.isra.0, %function
l2p_addr_tran.isra.0:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r10, lr}
	lsrs	r7, r0, #10
	ldr	r3, .L3
	mov	r8, r1
	mov	r6, r0
	uxth	r0, r7
	mov	r10, r2
	uxth	r7, r7
	ldrh	r4, [r3, #8]
	ubfx	r6, r6, #0, #10
	ldrh	r5, [r3, #10]
	ldrh	r3, [r3, #14]
	cmp	r3, #4
	itt	eq
	lsreq	r4, r4, #1
	lsleq	r5, r5, #1
	mov	r1, r4
	it	eq
	uxtheq	r5, r5
	bl	__aeabi_uidiv
	uxth	r0, r0
	mls	r4, r0, r4, r7
	mla	r4, r5, r4, r6
	str	r4, [r8]
	str	r0, [r10]
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r10, pc}
.L4:
	.align	2
.L3:
	.word	.LANCHOR0
	.size	l2p_addr_tran.isra.0, .-l2p_addr_tran.isra.0
	.section	.text.Ftl_log2,"ax",%progbits
	.align	1
	.global	Ftl_log2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	Ftl_log2, %function
Ftl_log2:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #0
	movs	r2, #1
.L6:
	cmp	r2, r0
	uxth	r3, r1
	add	r1, r1, #1
	bls	.L7
	subs	r0, r3, #1
	uxth	r0, r0
	bx	lr
.L7:
	lsls	r2, r2, #1
	b	.L6
	.size	Ftl_log2, .-Ftl_log2
	.section	.text.FtlPrintInfo,"ax",%progbits
	.align	1
	.global	FtlPrintInfo
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlPrintInfo, %function
FtlPrintInfo:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.size	FtlPrintInfo, .-FtlPrintInfo
	.section	.text.FtlSysBlkNumInit,"ax",%progbits
	.align	1
	.global	FtlSysBlkNumInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlSysBlkNumInit, %function
FtlSysBlkNumInit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L10
	cmp	r0, #24
	it	cc
	movcc	r0, #24
	ldr	r2, .L10+4
	str	r0, [r3]
	ldr	r3, .L10+8
	ldrh	r3, [r3]
	muls	r3, r0, r3
	str	r3, [r2]
	ldr	r2, .L10+12
	ldrh	r2, [r2]
	subs	r0, r2, r0
	ldr	r2, .L10+16
	strh	r0, [r2]	@ movhi
	movs	r0, #0
	ldr	r2, .L10+20
	ldr	r2, [r2]
	subs	r3, r2, r3
	ldr	r2, .L10+24
	str	r3, [r2]
	bx	lr
.L11:
	.align	2
.L10:
	.word	.LANCHOR1
	.word	.LANCHOR3
	.word	.LANCHOR2
	.word	.LANCHOR5
	.word	.LANCHOR4
	.word	.LANCHOR7
	.word	.LANCHOR6
	.size	FtlSysBlkNumInit, .-FtlSysBlkNumInit
	.global	__aeabi_idiv
	.section	.text.FtlConstantsInit,"ax",%progbits
	.align	1
	.global	FtlConstantsInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlConstantsInit, %function
FtlConstantsInit:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r3, r0
	ldrh	r4, [r0]
	ldr	r2, .L23
	ldrh	r5, [r0, #2]
	ldrh	r7, [r3, #14]
	strh	r4, [r2]	@ movhi
	ldr	r2, .L23+4
	ldrh	ip, [r0, #4]
	cmp	r7, #4
	ldr	r0, .L23+8
	strh	r5, [r2]	@ movhi
	ldr	r1, .L23+12
	ldrh	r2, [r3, #6]
	ldr	r6, .L23+16
	strh	ip, [r0]	@ movhi
	strh	r2, [r1]	@ movhi
	strh	r7, [r6]	@ movhi
	bne	.L13
	lsrs	r2, r2, #1
	strh	r2, [r1]	@ movhi
	movs	r2, #8
	strh	r2, [r6]	@ movhi
.L14:
	ldr	r7, .L23+20
	movs	r2, #0
.L15:
	strb	r2, [r2, r7]
	adds	r2, r2, #1
	cmp	r2, #32
	bne	.L15
	ldr	r2, .L23+24
	cmp	r4, #1
	mov	r7, #5
	ldrh	r0, [r0]
	mov	ip, #0
	str	r3, [sp, #4]
	strh	r7, [r2]	@ movhi
	it	eq
	strheq	r4, [r2]	@ movhi
	mov	r4, #640
	ldr	r2, .L23+28
	smulbb	r5, r5, r0
	ldr	r7, .L23+32
	strh	r4, [r2]	@ movhi
	uxth	r5, r5
	ldrh	r4, [r1]
	ldr	r2, .L23+36
	strh	ip, [r7]	@ movhi
	ldr	r7, .L23+40
	smulbb	r0, r0, r4
	strh	r5, [r2]	@ movhi
	ldr	r2, .L23+44
	uxth	r0, r0
	strh	r0, [r2]	@ movhi
	bl	Ftl_log2
	ldr	r3, [sp, #4]
	ldr	r2, .L23+48
	ldrh	fp, [r6]
	ldrh	r8, [r3, #12]
	strh	r0, [r2]	@ movhi
	ldr	r2, .L23+52
	mov	r0, fp
	strh	r8, [r7]	@ movhi
	smulbb	r1, r5, r8
	ldr	r6, .L23+56
	strh	r8, [r2]	@ movhi
	ldr	r2, .L23+60
	strh	r1, [r2]	@ movhi
	bl	Ftl_log2
	lsl	r2, fp, #9
	ldr	r1, .L23+64
	ldr	r3, [sp, #4]
	mov	r10, r0
	uxth	r2, r2
	strh	r0, [r6]	@ movhi
	mov	r0, #5120
	strh	r2, [r1]	@ movhi
	lsrs	r2, r2, #8
	ldr	r1, .L23+68
	strh	r2, [r1]	@ movhi
	mul	r1, r8, fp
	ldrh	r2, [r3, #20]
	ldr	r3, .L23+72
	strh	r2, [r3]	@ movhi
	mul	r3, r4, r5
	ldr	r2, .L23+76
	lsls	r4, r4, #6
	str	r3, [r2]
	mul	r3, fp, r3
	ldr	r2, .L23+80
	mul	r3, r8, r3
	ldr	r8, .L23+116
	asrs	r3, r3, #11
	str	r3, [r2]
	bl	__aeabi_idiv
	uxth	r0, r0
	ldr	r2, .L23+84
	mov	r1, r5
	cmp	r0, #4
	itet	ls
	movls	r3, #4
	strhhi	r0, [r8]	@ movhi
	strhls	r3, [r8]	@ movhi
	mov	r3, #640
	asr	r3, r3, r10
	add	r10, r10, #9
	asr	r4, r4, r10
	ldr	r10, .L23+120
	adds	r3, r3, #2
	ldrh	r0, [r8]
	strh	r3, [r2]	@ movhi
	ldr	r3, .L23+88
	strh	r4, [r3]	@ movhi
	uxth	r4, r4
	mul	r3, r5, r4
	adds	r4, r4, #8
	str	r3, [r10]
	bl	__aeabi_uidiv
	uxtah	r0, r4, r0
	ldr	r4, .L23+92
	cmp	r5, #1
	it	eq
	addeq	r0, r0, #4
	str	r0, [r4]
	ldrh	r0, [r4]
	bl	FtlSysBlkNumInit
	ldr	r2, [r4]
	movs	r0, #0
	ldr	r3, .L23+96
	str	r2, [r3]
	ldr	r3, .L23+100
	ldr	r2, [r3]
	ldrh	r3, [r7]
	lsls	r2, r2, #2
	muls	r3, r2, r3
	ldrh	r2, [r6]
	adds	r2, r2, #9
	lsrs	r3, r3, r2
	ldr	r2, .L23+104
	adds	r3, r3, #2
	strh	r3, [r2]	@ movhi
	movs	r2, #32
	ldr	r3, .L23+108
	strh	r2, [r3]	@ movhi
	ldr	r3, .L23+112
	str	r0, [r3]
	ldrh	r3, [r8]
	adds	r3, r3, #3
	strh	r3, [r8]	@ movhi
	ldr	r3, [r10]
	adds	r3, r3, #3
	str	r3, [r10]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L13:
	cmp	r7, #8
	bne	.L14
	cmp	ip, #1
	itttt	eq
	lsreq	r2, r2, #1
	strheq	r2, [r1]	@ movhi
	moveq	r2, #2
	strheq	r2, [r0]	@ movhi
	b	.L14
.L24:
	.align	2
.L23:
	.word	.LANCHOR8
	.word	.LANCHOR9
	.word	.LANCHOR10
	.word	.LANCHOR5
	.word	.LANCHOR11
	.word	.LANCHOR12
	.word	.LANCHOR13
	.word	.LANCHOR15
	.word	.LANCHOR14
	.word	.LANCHOR2
	.word	.LANCHOR18
	.word	.LANCHOR16
	.word	.LANCHOR17
	.word	.LANCHOR19
	.word	.LANCHOR21
	.word	.LANCHOR20
	.word	.LANCHOR22
	.word	.LANCHOR23
	.word	.LANCHOR24
	.word	.LANCHOR7
	.word	.LANCHOR25
	.word	.LANCHOR27
	.word	.LANCHOR28
	.word	.LANCHOR1
	.word	.LANCHOR30
	.word	.LANCHOR6
	.word	.LANCHOR31
	.word	.LANCHOR32
	.word	.LANCHOR33
	.word	.LANCHOR26
	.word	.LANCHOR29
	.size	FtlConstantsInit, .-FtlConstantsInit
	.section	.text.IsBlkInVendorPart,"ax",%progbits
	.align	1
	.global	IsBlkInVendorPart
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	IsBlkInVendorPart, %function
IsBlkInVendorPart:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L32
	ldrh	r3, [r3]
	cbz	r3, .L31
	ldr	r3, .L32+4
	ldr	r2, .L32+8
	ldr	r3, [r3]
	ldrh	r2, [r2]
	add	r2, r3, r2, lsl #1
.L27:
	cmp	r3, r2
	bne	.L28
.L31:
	movs	r0, #0
	bx	lr
.L28:
	ldrh	r1, [r3], #2
	cmp	r0, r1
	bne	.L27
	movs	r0, #1
	bx	lr
.L33:
	.align	2
.L32:
	.word	.LANCHOR34
	.word	.LANCHOR35
	.word	.LANCHOR26
	.size	IsBlkInVendorPart, .-IsBlkInVendorPart
	.section	.text.FtlCacheWriteBack,"ax",%progbits
	.align	1
	.global	FtlCacheWriteBack
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlCacheWriteBack, %function
FtlCacheWriteBack:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.size	FtlCacheWriteBack, .-FtlCacheWriteBack
	.section	.text.sftl_get_density,"ax",%progbits
	.align	1
	.global	sftl_get_density
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sftl_get_density, %function
sftl_get_density:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L36
	ldr	r0, [r3]
	bx	lr
.L37:
	.align	2
.L36:
	.word	.LANCHOR33
	.size	sftl_get_density, .-sftl_get_density
	.global	__aeabi_uidivmod
	.section	.text.FtlBbmMapBadBlock,"ax",%progbits
	.align	1
	.global	FtlBbmMapBadBlock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlBbmMapBadBlock, %function
FtlBbmMapBadBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L39
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	mov	r5, r0
	ldrh	r4, [r3]
	mov	r1, r4
	bl	__aeabi_uidiv
	mov	r1, r4
	ldr	r4, .L39+4
	uxth	r6, r0
	mov	r0, r5
	bl	__aeabi_uidivmod
	add	r2, r4, r6, lsl #2
	uxth	r3, r1
	ldr	r2, [r2, #28]
	lsrs	r1, r3, #5
	and	r7, r3, #31
	movs	r0, #1
	lsls	r0, r0, r7
	ldr	r7, [r2, r1, lsl #2]
	orrs	r0, r0, r7
	str	r0, [r2, r1, lsl #2]
	mov	r2, r6
	str	r0, [sp]
	mov	r1, r5
	ldr	r0, .L39+8
	bl	printf
	ldrh	r3, [r4, #6]
	movs	r0, #0
	adds	r3, r3, #1
	strh	r3, [r4, #6]	@ movhi
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L40:
	.align	2
.L39:
	.word	.LANCHOR16
	.word	.LANCHOR36
	.word	.LC0
	.size	FtlBbmMapBadBlock, .-FtlBbmMapBadBlock
	.section	.text.FtlBbmIsBadBlock,"ax",%progbits
	.align	1
	.global	FtlBbmIsBadBlock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlBbmIsBadBlock, %function
FtlBbmIsBadBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L42
	push	{r4, r5, r6, lr}
	mov	r6, r0
	ldrh	r5, [r3]
	mov	r1, r5
	bl	__aeabi_uidivmod
	mov	r0, r6
	uxth	r4, r1
	mov	r1, r5
	bl	__aeabi_uidiv
	ldr	r3, .L42+4
	uxth	r0, r0
	lsrs	r2, r4, #5
	and	r4, r4, #31
	add	r0, r3, r0, lsl #2
	ldr	r3, [r0, #28]
	ldr	r0, [r3, r2, lsl #2]
	lsrs	r0, r0, r4
	and	r0, r0, #1
	pop	{r4, r5, r6, pc}
.L43:
	.align	2
.L42:
	.word	.LANCHOR16
	.word	.LANCHOR36
	.size	FtlBbmIsBadBlock, .-FtlBbmIsBadBlock
	.section	.text.FtlBbtInfoPrint,"ax",%progbits
	.align	1
	.global	FtlBbtInfoPrint
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlBbtInfoPrint, %function
FtlBbtInfoPrint:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.size	FtlBbtInfoPrint, .-FtlBbtInfoPrint
	.section	.text.V2P_block,"ax",%progbits
	.align	1
	.global	V2P_block
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	V2P_block, %function
V2P_block:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r5, r1
	ldr	r3, .L46
	mov	r7, r0
	ldrh	r6, [r3]
	mov	r1, r6
	bl	__aeabi_uidiv
	ldr	r3, .L46+4
	smulbb	r5, r6, r5
	mov	r1, r6
	ldrh	r4, [r3]
	smulbb	r4, r4, r0
	mov	r0, r7
	bl	__aeabi_uidivmod
	adds	r0, r5, r1
	add	r0, r0, r4
	uxth	r0, r0
	pop	{r3, r4, r5, r6, r7, pc}
.L47:
	.align	2
.L46:
	.word	.LANCHOR10
	.word	.LANCHOR16
	.size	V2P_block, .-V2P_block
	.section	.text.P2V_plane,"ax",%progbits
	.align	1
	.global	P2V_plane
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	P2V_plane, %function
P2V_plane:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L49
	push	{r4, r5, r6, lr}
	mov	r6, r0
	ldrh	r5, [r3]
	ldr	r3, .L49+4
	ldrh	r1, [r3]
	bl	__aeabi_uidiv
	smulbb	r4, r0, r5
	mov	r1, r5
	mov	r0, r6
	bl	__aeabi_uidivmod
	add	r1, r1, r4
	uxth	r0, r1
	pop	{r4, r5, r6, pc}
.L50:
	.align	2
.L49:
	.word	.LANCHOR10
	.word	.LANCHOR16
	.size	P2V_plane, .-P2V_plane
	.section	.text.P2V_block_in_plane,"ax",%progbits
	.align	1
	.global	P2V_block_in_plane
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	P2V_block_in_plane, %function
P2V_block_in_plane:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r3, .L52
	ldrh	r1, [r3]
	bl	__aeabi_uidivmod
	ldr	r3, .L52+4
	uxth	r0, r1
	ldrh	r1, [r3]
	bl	__aeabi_uidiv
	uxth	r0, r0
	pop	{r3, pc}
.L53:
	.align	2
.L52:
	.word	.LANCHOR16
	.word	.LANCHOR10
	.size	P2V_block_in_plane, .-P2V_block_in_plane
	.section	.text.ftl_cmp_data_ver,"ax",%progbits
	.align	1
	.global	ftl_cmp_data_ver
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_cmp_data_ver, %function
ftl_cmp_data_ver:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, r1
	bls	.L55
	subs	r0, r0, r1
	cmp	r0, #-2147483648
	ite	hi
	movhi	r0, #0
	movls	r0, #1
	bx	lr
.L55:
	subs	r0, r1, r0
	cmp	r0, #-2147483648
	ite	ls
	movls	r0, #0
	movhi	r0, #1
	bx	lr
	.size	ftl_cmp_data_ver, .-ftl_cmp_data_ver
	.section	.text.FtlFreeSysBlkQueueEmpty,"ax",%progbits
	.align	1
	.global	FtlFreeSysBlkQueueEmpty
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlFreeSysBlkQueueEmpty, %function
FtlFreeSysBlkQueueEmpty:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L58
	ldrh	r0, [r3, #6]
	clz	r0, r0
	lsrs	r0, r0, #5
	bx	lr
.L59:
	.align	2
.L58:
	.word	.LANCHOR37
	.size	FtlFreeSysBlkQueueEmpty, .-FtlFreeSysBlkQueueEmpty
	.section	.text.FtlFreeSysBlkQueueFull,"ax",%progbits
	.align	1
	.global	FtlFreeSysBlkQueueFull
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlFreeSysBlkQueueFull, %function
FtlFreeSysBlkQueueFull:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L61
	ldrh	r0, [r3, #6]
	sub	r3, r0, #1024
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	bx	lr
.L62:
	.align	2
.L61:
	.word	.LANCHOR37
	.size	FtlFreeSysBlkQueueFull, .-FtlFreeSysBlkQueueFull
	.section	.text.FtlFreeSysBLkSort,"ax",%progbits
	.align	1
	.global	FtlFreeSysBLkSort
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlFreeSysBLkSort, %function
FtlFreeSysBLkSort:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L72
	push	{r4, r5, r6, lr}
	ldrh	r2, [r3, #6]
	cbz	r2, .L63
	ldr	r2, .L72+4
	movs	r6, #0
	ldrh	r1, [r3, #2]
	mov	r5, r6
	ldrh	r4, [r2, #28]
	ldrh	r2, [r3, #4]
	and	r4, r4, #31
.L65:
	uxth	r0, r6
	adds	r6, r6, #1
	cmp	r4, r0
	bgt	.L66
	cbz	r5, .L63
	strh	r1, [r3, #2]	@ movhi
	strh	r2, [r3, #4]	@ movhi
.L63:
	pop	{r4, r5, r6, pc}
.L66:
	adds	r0, r1, #4
	adds	r1, r1, #1
	ldrh	r5, [r3, r0, lsl #1]
	adds	r0, r2, #4
	ubfx	r1, r1, #0, #10
	strh	r5, [r3, r0, lsl #1]	@ movhi
	movs	r5, #1
	add	r2, r2, r5
	ubfx	r2, r2, #0, #10
	b	.L65
.L73:
	.align	2
.L72:
	.word	.LANCHOR37
	.word	.LANCHOR38
	.size	FtlFreeSysBLkSort, .-FtlFreeSysBLkSort
	.section	.text.IsInFreeQueue,"ax",%progbits
	.align	1
	.global	IsInFreeQueue
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	IsInFreeQueue, %function
IsInFreeQueue:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L80
	push	{r4, r5, lr}
	ldrh	r4, [r3, #6]
	cmp	r4, #1024
	beq	.L78
	ldrh	r5, [r3, #2]
	movs	r1, #0
.L76:
	cmp	r1, r4
	bcc	.L77
.L78:
	movs	r0, #0
	pop	{r4, r5, pc}
.L77:
	adds	r2, r1, r5
	ubfx	r2, r2, #0, #10
	adds	r2, r2, #4
	ldrh	r2, [r3, r2, lsl #1]
	cmp	r2, r0
	beq	.L79
	adds	r1, r1, #1
	b	.L76
.L79:
	movs	r0, #1
	pop	{r4, r5, pc}
.L81:
	.align	2
.L80:
	.word	.LANCHOR37
	.size	IsInFreeQueue, .-IsInFreeQueue
	.section	.text.insert_data_list,"ax",%progbits
	.align	1
	.global	insert_data_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	insert_data_list, %function
insert_data_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L97
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	ldrh	r8, [r3]
	cmp	r8, r0
	bls	.L84
	ldr	r3, .L97+4
	movs	r5, #6
	muls	r5, r0, r5
	ldr	r4, [r3]
	movw	r3, #65535
	adds	r1, r4, r5
	strh	r3, [r1, #2]	@ movhi
	strh	r3, [r4, r5]	@ movhi
	ldr	r3, .L97+8
	ldr	ip, [r3]
	cmp	ip, #0
	bne	.L85
.L96:
	str	r1, [r3]
.L84:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L85:
	ldr	r3, .L97+12
	ldrh	r6, [r1, #4]
	ldr	r10, [r3]
	ldrh	r3, [r10, r0, lsl #1]
	cbz	r6, .L92
	muls	r6, r3, r6
.L86:
	sub	r2, ip, r4
	movs	r7, #0
	asrs	r3, r2, #1
	ldr	r2, .L97+16
	muls	r2, r3, r2
	mov	r3, ip
	uxth	r2, r2
.L90:
	adds	r7, r7, #1
	uxth	r7, r7
	cmp	r8, r7
	bcc	.L84
	cmp	r2, r0
	beq	.L84
	ldrh	lr, [r3, #4]
	cmp	lr, #0
	beq	.L88
	ldrh	fp, [r10, r2, lsl #1]
	mul	lr, lr, fp
	cmp	r6, lr
	bls	.L88
	ldrh	lr, [r3]
	movw	fp, #65535
	cmp	lr, fp
	bne	.L89
	strh	r2, [r1, #2]	@ movhi
	strh	r0, [r3]	@ movhi
	ldr	r3, .L97+20
	b	.L96
.L92:
	mov	r6, #-1
	b	.L86
.L89:
	movs	r3, #6
	mov	r2, lr
	mla	r3, r3, lr, r4
	b	.L90
.L88:
	strh	r2, [r4, r5]	@ movhi
	cmp	r3, ip
	ldrh	r2, [r3, #2]
	strh	r2, [r1, #2]	@ movhi
	bne	.L91
	strh	r0, [r3, #2]	@ movhi
	ldr	r3, .L97+8
	b	.L96
.L91:
	ldrh	r1, [r3, #2]
	movs	r2, #6
	muls	r2, r1, r2
	strh	r0, [r4, r2]	@ movhi
	strh	r0, [r3, #2]	@ movhi
	b	.L84
.L98:
	.align	2
.L97:
	.word	.LANCHOR4
	.word	.LANCHOR39
	.word	.LANCHOR40
	.word	.LANCHOR41
	.word	-1431655765
	.word	.LANCHOR42
	.size	insert_data_list, .-insert_data_list
	.section	.text.INSERT_DATA_LIST,"ax",%progbits
	.align	1
	.global	INSERT_DATA_LIST
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	INSERT_DATA_LIST, %function
INSERT_DATA_LIST:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	insert_data_list
	ldr	r2, .L101
	ldrh	r3, [r2]
	adds	r3, r3, #1
	uxth	r3, r3
	strh	r3, [r2]	@ movhi
	ldr	r2, .L101+4
	ldrh	r2, [r2]
	cmp	r2, r3
	bcs	.L99
	movs	r2, #214
	ldr	r1, .L101+8
	ldr	r0, .L101+12
	pop	{r3, lr}
	b	printf
.L99:
	pop	{r3, pc}
.L102:
	.align	2
.L101:
	.word	.LANCHOR43
	.word	.LANCHOR4
	.word	.LANCHOR44
	.word	.LC1
	.size	INSERT_DATA_LIST, .-INSERT_DATA_LIST
	.section	.text.insert_free_list,"ax",%progbits
	.align	1
	.global	insert_free_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	insert_free_list, %function
insert_free_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movw	r1, #65535
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	cmp	r0, r1
	beq	.L104
	ldr	r3, .L110
	mov	r10, #6
	mul	r7, r10, r0
	ldr	r4, [r3]
	ldr	r3, .L110+4
	adds	r5, r4, r7
	ldr	r6, [r3]
	mov	lr, r3
	strh	r1, [r5, #2]	@ movhi
	strh	r1, [r4, r7]	@ movhi
	cbnz	r6, .L105
	str	r5, [r3]
.L104:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L105:
	ldr	r3, .L110+8
	subs	r2, r6, r4
	mov	fp, r1
	ldr	ip, [r3]
	asrs	r3, r2, #1
	ldr	r2, .L110+12
	ldrh	r8, [ip, r0, lsl #1]
	muls	r2, r3, r2
	mov	r3, r6
	uxth	r2, r2
.L108:
	ldrh	r1, [ip, r2, lsl #1]
	cmp	r1, r8
	bcs	.L106
	ldrh	r1, [r3]
	cmp	r1, fp
	bne	.L107
	strh	r2, [r5, #2]	@ movhi
	strh	r0, [r3]	@ movhi
	b	.L104
.L107:
	mla	r3, r10, r1, r4
	mov	r2, r1
	b	.L108
.L106:
	ldrh	r1, [r3, #2]
	cmp	r3, r6
	strh	r1, [r5, #2]	@ movhi
	it	ne
	ldrhne	r1, [r3, #2]
	strh	r2, [r4, r7]	@ movhi
	iteet	ne
	movne	r2, #6
	strheq	r0, [r3, #2]	@ movhi
	streq	r5, [lr]
	mulne	r2, r2, r1
	itt	ne
	strhne	r0, [r4, r2]	@ movhi
	strhne	r0, [r3, #2]	@ movhi
	b	.L104
.L111:
	.align	2
.L110:
	.word	.LANCHOR39
	.word	.LANCHOR45
	.word	.LANCHOR46
	.word	-1431655765
	.size	insert_free_list, .-insert_free_list
	.section	.text.INSERT_FREE_LIST,"ax",%progbits
	.align	1
	.global	INSERT_FREE_LIST
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	INSERT_FREE_LIST, %function
INSERT_FREE_LIST:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	insert_free_list
	ldr	r2, .L114
	ldrh	r3, [r2]
	adds	r3, r3, #1
	uxth	r3, r3
	strh	r3, [r2]	@ movhi
	ldr	r2, .L114+4
	ldrh	r2, [r2]
	cmp	r2, r3
	bcs	.L112
	movs	r2, #207
	ldr	r1, .L114+8
	ldr	r0, .L114+12
	pop	{r3, lr}
	b	printf
.L112:
	pop	{r3, pc}
.L115:
	.align	2
.L114:
	.word	.LANCHOR47
	.word	.LANCHOR4
	.word	.LANCHOR48
	.word	.LC1
	.size	INSERT_FREE_LIST, .-INSERT_FREE_LIST
	.section	.text.List_remove_node,"ax",%progbits
	.align	1
	.global	List_remove_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	List_remove_node, %function
List_remove_node:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	movs	r6, #6
	ldr	r5, .L122
	muls	r6, r1, r6
	movw	r3, #65535
	mov	r8, r0
	ldr	r7, [r5]
	adds	r4, r7, r6
	ldrh	r2, [r4, #2]
	cmp	r2, r3
	bne	.L117
	ldr	r3, [r0]
	cmp	r4, r3
	beq	.L117
	mov	r2, #372
	ldr	r1, .L122+4
	ldr	r0, .L122+8
	bl	printf
.L117:
	ldr	r3, [r8]
	movw	r1, #65535
	cmp	r4, r3
	ldrh	r3, [r7, r6]
	bne	.L118
	cmp	r3, r1
	ittee	ne
	ldrne	r0, [r5]
	movne	r2, #6
	moveq	r3, #0
	streq	r3, [r8]
	ittt	ne
	mlane	r3, r2, r3, r0
	strne	r3, [r8]
	strhne	r1, [r3, #2]	@ movhi
.L120:
	movw	r3, #65535
	movs	r0, #0
	strh	r3, [r7, r6]	@ movhi
	strh	r3, [r4, #2]	@ movhi
	pop	{r4, r5, r6, r7, r8, pc}
.L118:
	cmp	r3, r1
	ldrh	r1, [r4, #2]
	bne	.L121
	cmp	r1, r3
	beq	.L120
	movs	r2, #6
	ldr	r0, [r5]
	muls	r1, r2, r1
	strh	r3, [r0, r1]	@ movhi
	b	.L120
.L121:
	ldr	r0, [r5]
	movs	r2, #6
	mla	r5, r2, r3, r0
	strh	r1, [r5, #2]	@ movhi
	ldrh	r1, [r4, #2]
	muls	r2, r1, r2
	strh	r3, [r0, r2]	@ movhi
	b	.L120
.L123:
	.align	2
.L122:
	.word	.LANCHOR39
	.word	.LANCHOR49
	.word	.LC1
	.size	List_remove_node, .-List_remove_node
	.section	.text.List_pop_index_node,"ax",%progbits
	.align	1
	.global	List_pop_index_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	List_pop_index_node, %function
List_pop_index_node:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0]
	push	{r4, r5, r6, lr}
	cbz	r3, .L130
	ldr	r2, .L131
	movw	r5, #65535
	movs	r6, #6
	ldr	r2, [r2]
.L126:
	cbnz	r1, .L127
.L129:
	ldr	r4, .L131+4
	subs	r3, r3, r2
	asrs	r3, r3, #1
	muls	r4, r3, r4
	uxth	r1, r4
	bl	List_remove_node
	uxth	r0, r4
	pop	{r4, r5, r6, pc}
.L127:
	ldrh	r4, [r3]
	cmp	r4, r5
	beq	.L129
	subs	r1, r1, #1
	mla	r3, r6, r4, r2
	uxth	r1, r1
	b	.L126
.L130:
	movw	r0, #65535
	pop	{r4, r5, r6, pc}
.L132:
	.align	2
.L131:
	.word	.LANCHOR39
	.word	-1431655765
	.size	List_pop_index_node, .-List_pop_index_node
	.section	.text.List_pop_head_node,"ax",%progbits
	.align	1
	.global	List_pop_head_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	List_pop_head_node, %function
List_pop_head_node:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #0
	b	List_pop_index_node
	.size	List_pop_head_node, .-List_pop_head_node
	.section	.text.List_get_gc_head_node,"ax",%progbits
	.align	1
	.global	List_get_gc_head_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	List_get_gc_head_node, %function
List_get_gc_head_node:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L140
	push	{r4, lr}
	ldr	r3, [r3]
	cbz	r3, .L139
	ldr	r2, .L140+4
	movs	r4, #6
	ldr	r1, [r2]
	movw	r2, #65535
.L136:
	cbz	r0, .L137
	ldrh	r3, [r3]
	cmp	r3, r2
	bne	.L138
.L139:
	movw	r0, #65535
	pop	{r4, pc}
.L138:
	subs	r0, r0, #1
	mla	r3, r4, r3, r1
	uxth	r0, r0
	b	.L136
.L137:
	ldr	r0, .L140+8
	subs	r3, r3, r1
	asrs	r3, r3, #1
	muls	r3, r0, r3
	uxth	r0, r3
	pop	{r4, pc}
.L141:
	.align	2
.L140:
	.word	.LANCHOR40
	.word	.LANCHOR39
	.word	-1431655765
	.size	List_get_gc_head_node, .-List_get_gc_head_node
	.section	.text.List_update_data_list,"ax",%progbits
	.align	1
	.global	List_update_data_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	List_update_data_list, %function
List_update_data_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L153
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldrh	r3, [r3]
	cmp	r3, r0
	beq	.L144
	ldr	r3, .L153+4
	ldrh	r3, [r3]
	cmp	r3, r0
	beq	.L144
	ldr	r3, .L153+8
	ldrh	r3, [r3]
	cmp	r3, r0
	beq	.L144
	ldr	r7, .L153+12
	movs	r6, #6
	ldr	r3, .L153+16
	muls	r6, r0, r6
	ldr	fp, [r7]
	ldr	r3, [r3]
	add	r10, fp, r6
	cmp	r10, r3
	beq	.L144
	ldr	r3, .L153+20
	ldrh	r5, [r10, #4]
	ldr	r2, [r3]
	mov	r8, r3
	ldrh	r2, [r2, r0, lsl #1]
	cmp	r5, #0
	beq	.L151
	muls	r5, r2, r5
.L146:
	ldrh	r3, [r10, #2]
	movw	r2, #65535
	cmp	r3, r2
	bne	.L147
	ldrh	r2, [fp, r6]
	cmp	r2, r3
	bne	.L147
	movw	r2, #463
	ldr	r1, .L153+24
	ldr	r0, .L153+28
	bl	printf
.L147:
	ldrh	r3, [r10, #2]
	movw	r2, #65535
	cmp	r3, r2
	bne	.L148
	ldrh	r2, [fp, r6]
	cmp	r2, r3
	beq	.L144
.L148:
	movs	r2, #6
	muls	r2, r3, r2
	ldr	r3, .L153+32
	asrs	r1, r2, #1
	muls	r3, r1, r3
	ldr	r1, [r8]
	ldrh	r0, [r1, r3, lsl #1]
	ldr	r1, [r7]
	add	r2, r2, r1
	ldrh	r3, [r2, #4]
	cbz	r3, .L152
	muls	r3, r0, r3
.L149:
	cmp	r5, r3
	bcs	.L144
	ldr	r5, .L153+36
	mov	r1, r4
	ldr	r0, .L153+16
	bl	List_remove_node
	ldrh	r3, [r5]
	cbnz	r3, .L150
	mov	r2, #474
	ldr	r1, .L153+24
	ldr	r0, .L153+28
	bl	printf
.L150:
	ldrh	r3, [r5]
	mov	r0, r4
	subs	r3, r3, #1
	strh	r3, [r5]	@ movhi
	bl	INSERT_DATA_LIST
.L144:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L151:
	mov	r5, #-1
	b	.L146
.L152:
	mov	r3, #-1
	b	.L149
.L154:
	.align	2
.L153:
	.word	.LANCHOR50
	.word	.LANCHOR51
	.word	.LANCHOR52
	.word	.LANCHOR39
	.word	.LANCHOR40
	.word	.LANCHOR41
	.word	.LANCHOR53
	.word	.LC1
	.word	-1431655765
	.word	.LANCHOR43
	.size	List_update_data_list, .-List_update_data_list
	.section	.text.select_l2p_ram_region,"ax",%progbits
	.align	1
	.global	select_l2p_ram_region
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	select_l2p_ram_region, %function
select_l2p_ram_region:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	movs	r1, #0
	ldr	r3, .L165
	movs	r0, #12
	movw	r5, #65535
	ldrh	r2, [r3]
	ldr	r3, .L165+4
	ldr	r3, [r3]
.L156:
	uxth	r4, r1
	cmp	r4, r2
	bcc	.L158
	mov	r4, r2
	movs	r1, #0
	mov	r6, #-2147483648
	movs	r7, #12
.L159:
	uxth	r5, r1
	cmp	r5, r2
	bcc	.L161
	cmp	r4, r2
	bcc	.L157
	ldr	r1, .L165+8
	mov	r4, r2
	mov	r0, #-1
	ldrh	r7, [r1]
	movs	r1, #0
.L162:
	uxth	r5, r1
	cmp	r5, r2
	bcc	.L164
	cmp	r4, r2
	bcc	.L157
	mov	r2, #796
	ldr	r1, .L165+12
	ldr	r0, .L165+16
	bl	printf
	b	.L157
.L158:
	adds	r1, r1, #1
	mla	r6, r0, r1, r3
	ldrh	r6, [r6, #-12]
	cmp	r6, r5
	bne	.L156
.L157:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L161:
	mla	r0, r7, r1, r3
	ldr	r0, [r0, #4]
	cmp	r0, #0
	blt	.L160
	cmp	r6, r0
	itt	hi
	movhi	r6, r0
	movhi	r4, r5
.L160:
	adds	r1, r1, #1
	b	.L159
.L164:
	ldr	r6, [r3, #4]
	cmp	r0, r6
	bls	.L163
	ldrh	ip, [r3]
	cmp	ip, r7
	itt	ne
	movne	r0, r6
	movne	r4, r5
.L163:
	adds	r1, r1, #1
	adds	r3, r3, #12
	b	.L162
.L166:
	.align	2
.L165:
	.word	.LANCHOR32
	.word	.LANCHOR54
	.word	.LANCHOR55
	.word	.LANCHOR56
	.word	.LC1
	.size	select_l2p_ram_region, .-select_l2p_ram_region
	.section	.text.FtlUpdateVaildLpn,"ax",%progbits
	.align	1
	.global	FtlUpdateVaildLpn
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlUpdateVaildLpn, %function
FtlUpdateVaildLpn:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r2, .L177
	push	{r4, r5, r6, lr}
	mov	r1, r2
	ldrh	r3, [r2]
	cmp	r3, #4
	bhi	.L168
	cbnz	r0, .L168
	adds	r3, r3, #1
	strh	r3, [r2]	@ movhi
	pop	{r4, r5, r6, pc}
.L168:
	movs	r3, #0
	ldr	r0, .L177+4
	strh	r3, [r1]	@ movhi
	movw	r6, #65535
	ldr	r1, .L177+8
	ldrh	r4, [r0]
	mov	r0, r3
	ldr	r2, .L177+12
	ldr	r1, [r1]
	str	r3, [r2]
	add	r4, r1, r4, lsl #1
.L169:
	cmp	r1, r4
	bne	.L171
	cbz	r3, .L167
	str	r0, [r2]
.L167:
	pop	{r4, r5, r6, pc}
.L171:
	ldrh	r5, [r1], #2
	cmp	r5, r6
	itt	ne
	addne	r0, r0, r5
	movne	r3, #1
	b	.L169
.L178:
	.align	2
.L177:
	.word	.LANCHOR57
	.word	.LANCHOR4
	.word	.LANCHOR41
	.word	.LANCHOR58
	.size	FtlUpdateVaildLpn, .-FtlUpdateVaildLpn
	.section	.text.ftl_sb_update_avl_pages,"ax",%progbits
	.align	1
	.global	ftl_sb_update_avl_pages
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_sb_update_avl_pages, %function
ftl_sb_update_avl_pages:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, #0
	push	{r4, r5, r6, lr}
	strh	r3, [r0, #4]	@ movhi
	movw	r6, #65535
	ldr	r3, .L186
	ldrh	r4, [r3]
	add	r3, r0, r2, lsl #1
	adds	r3, r3, #14
.L180:
	cmp	r2, r4
	bcc	.L182
	ldr	r3, .L186+4
	add	r5, r0, #16
	movw	r6, #65535
	ldrh	r3, [r3]
	subs	r3, r3, #1
	subs	r1, r3, r1
	movs	r3, #0
	uxth	r1, r1
.L183:
	uxth	r2, r3
	cmp	r4, r2
	bhi	.L185
	pop	{r4, r5, r6, pc}
.L182:
	ldrh	r5, [r3, #2]!
	adds	r2, r2, #1
	uxth	r2, r2
	cmp	r5, r6
	ittt	ne
	ldrhne	r5, [r0, #4]
	addne	r5, r5, #1
	strhne	r5, [r0, #4]	@ movhi
	b	.L180
.L185:
	ldrh	r2, [r5], #2
	adds	r3, r3, #1
	cmp	r2, r6
	ittt	ne
	ldrhne	r2, [r0, #4]
	addne	r2, r2, r1
	strhne	r2, [r0, #4]	@ movhi
	b	.L183
.L187:
	.align	2
.L186:
	.word	.LANCHOR2
	.word	.LANCHOR18
	.size	ftl_sb_update_avl_pages, .-ftl_sb_update_avl_pages
	.section	.text.FtlSlcSuperblockCheck,"ax",%progbits
	.align	1
	.global	FtlSlcSuperblockCheck
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlSlcSuperblockCheck, %function
FtlSlcSuperblockCheck:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r3, [r0, #4]
	push	{r4, r5, lr}
	cbz	r3, .L188
	ldrh	r2, [r0]
	movw	r3, #65535
	cmp	r2, r3
	beq	.L188
	ldrb	r2, [r0, #6]	@ zero_extendqisi2
	movs	r5, #0
	adds	r2, r2, #8
	ldrh	r1, [r0, r2, lsl #1]
	ldr	r2, .L194
	ldrh	r4, [r2]
	mov	r2, r3
.L191:
	cmp	r1, r2
	beq	.L193
.L188:
	pop	{r4, r5, pc}
.L193:
	ldrb	r3, [r0, #6]	@ zero_extendqisi2
	adds	r3, r3, #1
	uxtb	r3, r3
	cmp	r3, r4
	strb	r3, [r0, #6]
	itttt	eq
	ldrheq	r3, [r0, #2]
	strbeq	r5, [r0, #6]
	addeq	r3, r3, #1
	strheq	r3, [r0, #2]	@ movhi
	ldrb	r3, [r0, #6]	@ zero_extendqisi2
	adds	r3, r3, #8
	ldrh	r1, [r0, r3, lsl #1]
	b	.L191
.L195:
	.align	2
.L194:
	.word	.LANCHOR2
	.size	FtlSlcSuperblockCheck, .-FtlSlcSuperblockCheck
	.section	.text.make_superblock,"ax",%progbits
	.align	1
	.global	make_superblock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	make_superblock, %function
make_superblock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L202
	ldrh	r2, [r0]
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldrh	r3, [r3]
	cmp	r2, r3
	bcc	.L197
	movw	r2, #2157
	ldr	r1, .L202+4
	ldr	r0, .L202+8
	bl	printf
.L197:
	ldr	r3, .L202+12
	add	r6, r4, #16
	ldr	r10, .L202+24
	movw	r7, #65535
	movs	r5, #0
	ldrh	r8, [r3]
	strh	r5, [r4, #4]	@ movhi
	strb	r5, [r4, #7]
.L198:
	uxth	r3, r5
	ldrh	r1, [r4]
	cmp	r8, r3
	bhi	.L200
	ldr	r2, .L202+16
	movs	r0, #0
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldrh	r2, [r2]
	smulbb	r3, r3, r2
	strh	r3, [r4, #4]	@ movhi
	movs	r3, #0
	strb	r3, [r4, #9]
	ldr	r3, .L202+20
	ldr	r3, [r3]
	ldrh	r2, [r3, r1, lsl #1]
	movw	r3, #10000
	cmp	r2, r3
	itt	hi
	movhi	r3, #1
	strbhi	r3, [r4, #9]
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L200:
	ldrb	r0, [r10, r5]	@ zero_extendqisi2
	bl	V2P_block
	strh	r7, [r6]	@ movhi
	mov	fp, r0
	bl	FtlBbmIsBadBlock
	cbnz	r0, .L199
	strh	fp, [r6]	@ movhi
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	adds	r3, r3, #1
	strb	r3, [r4, #7]
.L199:
	adds	r5, r5, #1
	adds	r6, r6, #2
	b	.L198
.L203:
	.align	2
.L202:
	.word	.LANCHOR4
	.word	.LANCHOR59
	.word	.LC1
	.word	.LANCHOR2
	.word	.LANCHOR18
	.word	.LANCHOR46
	.word	.LANCHOR12
	.size	make_superblock, .-make_superblock
	.section	.text.update_multiplier_value,"ax",%progbits
	.align	1
	.global	update_multiplier_value
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	update_multiplier_value, %function
update_multiplier_value:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r10, lr}
	movs	r5, #0
	ldr	r3, .L210
	mov	r6, r0
	mov	r4, r5
	ldr	r10, .L210+12
	ldrh	r7, [r3]
	ldr	r3, .L210+4
	ldrh	r8, [r3]
.L205:
	uxth	r3, r5
	cmp	r7, r3
	bhi	.L207
	cbz	r4, .L209
	mov	r1, r4
	mov	r0, #32768
	bl	__aeabi_idiv
.L208:
	ldr	r3, .L210+8
	movs	r2, #6
	ldr	r3, [r3]
	mla	r6, r2, r6, r3
	strh	r0, [r6, #4]	@ movhi
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r10, pc}
.L207:
	mov	r1, r6
	ldrb	r0, [r10, r5]	@ zero_extendqisi2
	bl	V2P_block
	bl	FtlBbmIsBadBlock
	cbnz	r0, .L206
	add	r4, r4, r8
	uxth	r4, r4
.L206:
	adds	r5, r5, #1
	b	.L205
.L209:
	mov	r0, r4
	b	.L208
.L211:
	.align	2
.L210:
	.word	.LANCHOR2
	.word	.LANCHOR18
	.word	.LANCHOR39
	.word	.LANCHOR12
	.size	update_multiplier_value, .-update_multiplier_value
	.section	.text.GetFreeBlockMinEraseCount,"ax",%progbits
	.align	1
	.global	GetFreeBlockMinEraseCount
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	GetFreeBlockMinEraseCount, %function
GetFreeBlockMinEraseCount:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L215
	ldr	r0, [r3]
	cbz	r0, .L213
	ldr	r3, .L215+4
	ldr	r3, [r3]
	subs	r0, r0, r3
	ldr	r3, .L215+8
	asrs	r0, r0, #1
	muls	r0, r3, r0
	ldr	r3, .L215+12
	ldr	r3, [r3]
	uxth	r0, r0
	ldrh	r0, [r3, r0, lsl #1]
.L213:
	bx	lr
.L216:
	.align	2
.L215:
	.word	.LANCHOR45
	.word	.LANCHOR39
	.word	-1431655765
	.word	.LANCHOR46
	.size	GetFreeBlockMinEraseCount, .-GetFreeBlockMinEraseCount
	.section	.text.GetFreeBlockMaxEraseCount,"ax",%progbits
	.align	1
	.global	GetFreeBlockMaxEraseCount
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	GetFreeBlockMaxEraseCount, %function
GetFreeBlockMaxEraseCount:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L225
	push	{r4, r5, r6, lr}
	ldr	r3, [r3]
	cbz	r3, .L223
	ldr	r2, .L225+4
	movs	r5, #6
	movw	r6, #65535
	ldrh	r2, [r2]
	rsb	r2, r2, r2, lsl #3
	asrs	r2, r2, #3
	cmp	r0, r2
	it	gt
	uxthgt	r0, r2
	ldr	r2, .L225+8
	ldr	r1, [r2]
	ldr	r2, .L225+12
	subs	r3, r3, r1
	asrs	r3, r3, #1
	muls	r3, r2, r3
	movs	r2, #0
	uxth	r3, r3
.L220:
	uxth	r4, r2
	cmp	r0, r4
	bls	.L222
	mul	r4, r5, r3
	adds	r2, r2, #1
	ldrh	r4, [r1, r4]
	cmp	r4, r6
	bne	.L224
.L222:
	ldr	r2, .L225+16
	ldr	r2, [r2]
	ldrh	r0, [r2, r3, lsl #1]
	pop	{r4, r5, r6, pc}
.L224:
	mov	r3, r4
	b	.L220
.L223:
	mov	r0, r3
	pop	{r4, r5, r6, pc}
.L226:
	.align	2
.L225:
	.word	.LANCHOR45
	.word	.LANCHOR47
	.word	.LANCHOR39
	.word	-1431655765
	.word	.LANCHOR46
	.size	GetFreeBlockMaxEraseCount, .-GetFreeBlockMaxEraseCount
	.section	.text.free_data_superblock,"ax",%progbits
	.align	1
	.global	free_data_superblock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	free_data_superblock, %function
free_data_superblock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movw	r2, #65535
	push	{r3, lr}
	cmp	r0, r2
	beq	.L228
	ldr	r2, .L229
	movs	r1, #0
	ldr	r2, [r2]
	strh	r1, [r2, r0, lsl #1]	@ movhi
	bl	INSERT_FREE_LIST
.L228:
	movs	r0, #0
	pop	{r3, pc}
.L230:
	.align	2
.L229:
	.word	.LANCHOR41
	.size	free_data_superblock, .-free_data_superblock
	.section	.text.get_new_active_ppa,"ax",%progbits
	.align	1
	.global	get_new_active_ppa
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_new_active_ppa, %function
get_new_active_ppa:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r2, [r0]
	push	{r3, r4, r5, r6, r7, lr}
	movw	r3, #65535
	mov	r4, r0
	cmp	r2, r3
	bne	.L232
	movw	r2, #2792
	ldr	r1, .L246
	ldr	r0, .L246+4
	bl	printf
.L232:
	ldr	r6, .L246+8
	ldrh	r2, [r4, #2]
	ldrh	r3, [r6]
	cmp	r2, r3
	bne	.L233
	movw	r2, #2793
	ldr	r1, .L246
	ldr	r0, .L246+4
	bl	printf
.L233:
	ldrh	r3, [r4, #4]
	cbnz	r3, .L234
	movw	r2, #2794
	ldr	r1, .L246
	ldr	r0, .L246+4
	bl	printf
.L234:
	ldrb	r2, [r4, #6]	@ zero_extendqisi2
	movs	r3, #0
	strb	r3, [r4, #10]
	movw	r5, #65535
	adds	r2, r2, #8
	ldrh	r0, [r4, r2, lsl #1]
	ldr	r2, .L246+12
	ldrh	r1, [r2]
	mov	r2, r3
.L235:
	cmp	r0, r5
	beq	.L237
	ldrh	r5, [r4, #2]
	ldrh	r6, [r6]
	cmp	r5, r6
	bcs	.L241
	ldrh	r2, [r4, #4]
	orr	r5, r5, r0, lsl #10
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	movw	r0, #65535
	subs	r2, r2, #1
	uxth	r2, r2
	strh	r2, [r4, #4]	@ movhi
.L240:
	adds	r3, r3, #1
	uxtb	r3, r3
	cmp	r1, r3
	itttt	eq
	ldrheq	r3, [r4, #2]
	addeq	r3, r3, #1
	strheq	r3, [r4, #2]	@ movhi
	moveq	r3, #0
	add	r7, r3, #8
	ldrh	r7, [r4, r7, lsl #1]
	cmp	r7, r0
	beq	.L240
	strb	r3, [r4, #6]
	ldrh	r3, [r4, #2]
	cmp	r3, r6
	bne	.L231
	cbz	r2, .L231
	movw	r2, #2817
	ldr	r1, .L246
	ldr	r0, .L246+4
	bl	printf
.L231:
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, pc}
.L237:
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	adds	r3, r3, #1
	uxtb	r3, r3
	cmp	r3, r1
	strb	r3, [r4, #6]
	itttt	eq
	ldrheq	r3, [r4, #2]
	strbeq	r2, [r4, #6]
	addeq	r3, r3, #1
	strheq	r3, [r4, #2]	@ movhi
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	adds	r3, r3, #8
	ldrh	r0, [r4, r3, lsl #1]
	b	.L235
.L241:
	movw	r5, #65535
	b	.L231
.L247:
	.align	2
.L246:
	.word	.LANCHOR60
	.word	.LC1
	.word	.LANCHOR18
	.word	.LANCHOR2
	.size	get_new_active_ppa, .-get_new_active_ppa
	.section	.text.FtlGcBufInit,"ax",%progbits
	.align	1
	.global	FtlGcBufInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcBufInit, %function
FtlGcBufInit:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r3, #0
	ldr	r1, .L253
	mov	fp, #1
	ldr	r2, .L253+4
	ldr	r5, [r1]
	ldr	r1, .L253+8
	str	r3, [r2]
	ldr	r2, .L253+12
	mov	r0, r5
	ldr	r1, [r1]
	ldrh	r2, [r2]
	str	r1, [sp, #4]
	ldr	r1, .L253+16
	ldrh	r1, [r1]
	str	r1, [sp]
	ldr	r1, .L253+20
	ldr	r10, [r1]
	ldr	r1, .L253+24
	ldrh	r7, [r1]
	ldr	r1, .L253+28
	ldr	r4, [r1]
	movs	r1, #12
	mla	r1, r2, r1, r1
	adds	r4, r4, #8
	add	r8, r5, r1
	mov	r1, r3
.L249:
	adds	r0, r0, #12
	ldr	r6, [sp]
	cmp	r0, r8
	add	ip, r3, r7
	add	r4, r4, #20
	add	lr, r1, r6
	bne	.L250
	ldr	r3, .L253+32
	mov	lr, #12
	mov	r8, #0
	ldr	r0, [r3]
	ldr	r3, .L253+8
	ldr	r4, [r3]
	ldr	r3, .L253+20
	ldr	ip, [r3]
.L251:
	cmp	r2, r0
	bcc	.L252
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L250:
	bic	r1, r1, #3
	bic	r3, r3, #3
	mov	r6, r1
	ldr	r1, [sp, #4]
	add	r3, r3, r10
	str	fp, [r0, #-4]
	str	r3, [r0, #-8]
	add	r6, r6, r1
	mov	r1, lr
	str	r6, [r0, #-12]
	str	r3, [r4, #-16]
	mov	r3, ip
	str	r6, [r4, #-20]
	b	.L249
.L252:
	ldr	r3, [sp]
	mul	r10, lr, r2
	muls	r3, r2, r3
	add	r1, r5, r10
	str	r8, [r1, #8]
	bic	r3, r3, #3
	add	r3, r3, r4
	str	r3, [r5, r10]
	mul	r3, r2, r7
	adds	r2, r2, #1
	uxth	r2, r2
	bic	r3, r3, #3
	add	r3, r3, ip
	str	r3, [r1, #4]
	b	.L251
.L254:
	.align	2
.L253:
	.word	.LANCHOR62
	.word	.LANCHOR61
	.word	.LANCHOR63
	.word	.LANCHOR2
	.word	.LANCHOR22
	.word	.LANCHOR64
	.word	.LANCHOR23
	.word	.LANCHOR65
	.word	.LANCHOR66
	.size	FtlGcBufInit, .-FtlGcBufInit
	.section	.text.FtlGcBufFree,"ax",%progbits
	.align	1
	.global	FtlGcBufFree
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcBufFree, %function
FtlGcBufFree:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L262
	mov	ip, #12
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r4, #0
	mov	fp, #20
	mov	lr, r4
	ldr	r7, [r3]
	ldr	r3, .L262+4
	ldr	r5, [r3]
.L256:
	uxth	r3, r4
	cmp	r1, r3
	bls	.L255
	mla	r8, fp, r3, r0
	movs	r2, #0
.L257:
	uxth	r3, r2
	cmp	r7, r3
	bls	.L258
	mul	r3, ip, r3
	ldr	r6, [r8, #8]
	adds	r2, r2, #1
	add	r10, r5, r3
	ldr	r3, [r5, r3]
	cmp	r3, r6
	bne	.L257
	str	lr, [r10, #8]
.L258:
	adds	r4, r4, #1
	b	.L256
.L255:
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L263:
	.align	2
.L262:
	.word	.LANCHOR66
	.word	.LANCHOR62
	.size	FtlGcBufFree, .-FtlGcBufFree
	.section	.text.FtlGcBufAlloc,"ax",%progbits
	.align	1
	.global	FtlGcBufAlloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcBufAlloc, %function
FtlGcBufAlloc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L271
	movs	r2, #0
	push	{r4, r5, r6, r7, r8, r10, lr}
	mov	ip, #12
	movs	r7, #1
	mov	lr, #20
	ldr	r4, [r3]
	ldr	r3, .L271+4
	ldr	r5, [r3]
.L265:
	uxth	r8, r2
	cmp	r1, r8
	bhi	.L269
	pop	{r4, r5, r6, r7, r8, r10, pc}
.L269:
	mov	r10, #0
.L266:
	uxth	r3, r10
	cmp	r4, r3
	bls	.L267
	mla	r3, ip, r3, r5
	add	r10, r10, #1
	ldr	r6, [r3, #8]
	cmp	r6, #0
	bne	.L266
	mla	r8, lr, r8, r0
	str	r7, [r3, #8]
	ldr	r6, [r3]
	ldr	r3, [r3, #4]
	str	r6, [r8, #8]
	str	r3, [r8, #12]
.L267:
	adds	r2, r2, #1
	b	.L265
.L272:
	.align	2
.L271:
	.word	.LANCHOR66
	.word	.LANCHOR62
	.size	FtlGcBufAlloc, .-FtlGcBufAlloc
	.section	.text.IsBlkInGcList,"ax",%progbits
	.align	1
	.global	IsBlkInGcList
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	IsBlkInGcList, %function
IsBlkInGcList:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L278
	ldr	r2, .L278+4
	ldr	r3, [r3]
	ldrh	r2, [r2]
	add	r2, r3, r2, lsl #1
.L274:
	cmp	r3, r2
	bne	.L276
	movs	r0, #0
	bx	lr
.L276:
	ldrh	r1, [r3], #2
	cmp	r1, r0
	bne	.L274
	movs	r0, #1
	bx	lr
.L279:
	.align	2
.L278:
	.word	.LANCHOR67
	.word	.LANCHOR68
	.size	IsBlkInGcList, .-IsBlkInGcList
	.section	.text.FtlGcUpdatePage,"ax",%progbits
	.align	1
	.global	FtlGcUpdatePage
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcUpdatePage, %function
FtlGcUpdatePage:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	mov	r5, r0
	ldr	r4, .L284
	ubfx	r0, r0, #10, #16
	mov	r6, r1
	mov	r7, r2
	bl	P2V_block_in_plane
	ldr	r3, .L284+4
	ldrh	r1, [r4]
	ldr	r2, [r3]
	movs	r3, #0
.L281:
	uxth	ip, r3
	cmp	ip, r1
	bcc	.L283
	bne	.L282
	strh	r0, [r2, ip, lsl #1]	@ movhi
	ldrh	r3, [r4]
	adds	r3, r3, #1
	strh	r3, [r4]	@ movhi
	b	.L282
.L283:
	adds	r3, r3, #1
	add	ip, r2, r3, lsl #1
	ldrh	ip, [ip, #-2]
	cmp	ip, r0
	bne	.L281
.L282:
	ldr	r2, .L284+8
	movs	r0, #12
	ldr	r1, .L284+12
	ldrh	r3, [r2]
	ldr	r1, [r1]
	muls	r0, r3, r0
	adds	r3, r3, #1
	adds	r4, r1, r0
	str	r6, [r4, #4]
	str	r7, [r4, #8]
	str	r5, [r1, r0]
	strh	r3, [r2]	@ movhi
	pop	{r3, r4, r5, r6, r7, pc}
.L285:
	.align	2
.L284:
	.word	.LANCHOR68
	.word	.LANCHOR67
	.word	.LANCHOR69
	.word	.LANCHOR70
	.size	FtlGcUpdatePage, .-FtlGcUpdatePage
	.section	.text.FtlGcRefreshBlock,"ax",%progbits
	.align	1
	.global	FtlGcRefreshBlock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcRefreshBlock, %function
FtlGcRefreshBlock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	ldr	r4, .L289
	ldrh	r5, [r4]
	cmp	r5, r0
	beq	.L287
	ldr	r3, .L289+4
	ldrh	r1, [r3]
	cmp	r0, r1
	beq	.L287
	movw	r2, #65535
	cmp	r5, r2
	bne	.L288
	strh	r0, [r4]	@ movhi
.L287:
	movs	r0, #0
	pop	{r4, r5, pc}
.L288:
	cmp	r1, r2
	it	eq
	strheq	r0, [r3]	@ movhi
	b	.L287
.L290:
	.align	2
.L289:
	.word	.LANCHOR71
	.word	.LANCHOR72
	.size	FtlGcRefreshBlock, .-FtlGcRefreshBlock
	.section	.text.FtlGcMarkBadPhyBlk,"ax",%progbits
	.align	1
	.global	FtlGcMarkBadPhyBlk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcMarkBadPhyBlk, %function
FtlGcMarkBadPhyBlk:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r0
	bl	P2V_block_in_plane
	bl	FtlGcRefreshBlock
	ldr	r2, .L295
	movs	r1, #0
	ldr	r5, .L295+4
	ldrh	r3, [r2]
.L292:
	uxth	r0, r1
	cmp	r3, r0
	bhi	.L294
	cmp	r3, #15
	itttt	ls
	addls	r1, r3, #1
	strhls	r1, [r2]	@ movhi
	ldrls	r2, .L295+4
	strhls	r4, [r2, r3, lsl #1]	@ movhi
	b	.L293
.L294:
	adds	r1, r1, #1
	add	r0, r5, r1, lsl #1
	ldrh	r0, [r0, #-2]
	cmp	r0, r4
	bne	.L292
.L293:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L296:
	.align	2
.L295:
	.word	.LANCHOR73
	.word	.LANCHOR74
	.size	FtlGcMarkBadPhyBlk, .-FtlGcMarkBadPhyBlk
	.section	.text.FtlGcReFreshBadBlk,"ax",%progbits
	.align	1
	.global	FtlGcReFreshBadBlk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcReFreshBadBlk, %function
FtlGcReFreshBadBlk:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L303
	push	{r4, lr}
	ldrh	r3, [r3]
	cbz	r3, .L298
	ldr	r2, .L303+4
	ldrh	r1, [r2]
	movw	r2, #65535
	cmp	r1, r2
	bne	.L298
	ldr	r4, .L303+8
	ldrh	r2, [r4]
	cmp	r2, r3
	itt	cs
	movcs	r3, #0
	strhcs	r3, [r4]	@ movhi
	ldr	r3, .L303+12
	ldrh	r2, [r4]
	ldrh	r0, [r3, r2, lsl #1]
	bl	P2V_block_in_plane
	bl	FtlGcRefreshBlock
	ldrh	r3, [r4]
	adds	r3, r3, #1
	strh	r3, [r4]	@ movhi
.L298:
	movs	r0, #0
	pop	{r4, pc}
.L304:
	.align	2
.L303:
	.word	.LANCHOR73
	.word	.LANCHOR71
	.word	.LANCHOR75
	.word	.LANCHOR74
	.size	FtlGcReFreshBadBlk, .-FtlGcReFreshBadBlk
	.section	.text.ftl_malloc,"ax",%progbits
	.align	1
	.global	ftl_malloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_malloc, %function
ftl_malloc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #0
	b	kmalloc
	.size	ftl_malloc, .-ftl_malloc
	.section	.text.ftl_free,"ax",%progbits
	.align	1
	.global	ftl_free
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_free, %function
ftl_free:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	free
	.size	ftl_free, .-ftl_free
	.section	.text.rknand_print_hex,"ax",%progbits
	.align	1
	.global	rknand_print_hex
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rknand_print_hex, %function
rknand_print_hex:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r6, #0
	mov	fp, r0
	mov	r7, r1
	mov	r8, r2
	mov	r10, r3
	mov	r5, r6
	mov	r4, r6
.L308:
	cmp	r4, r10
	bcc	.L314
	ldr	r0, .L317
	pop	{r4, r5, r6, r7, r8, r10, fp, lr}
	b	printf
.L314:
	cbnz	r5, .L309
	mov	r2, r6
	mov	r1, fp
	ldr	r0, .L317+4
	bl	printf
.L309:
	cmp	r8, #4
	bne	.L310
	ldr	r1, [r7, r4, lsl #2]
.L316:
	ldr	r0, .L317+8
.L315:
	adds	r5, r5, #1
	bl	printf
	cmp	r5, #15
	bls	.L313
	movs	r5, #0
	ldr	r0, .L317
	bl	printf
.L313:
	adds	r4, r4, #1
	add	r6, r6, r8
	b	.L308
.L310:
	cmp	r8, #2
	bne	.L312
	ldrh	r1, [r7, r4, lsl #1]
	b	.L316
.L312:
	ldrb	r1, [r7, r4]	@ zero_extendqisi2
	ldr	r0, .L317+12
	b	.L315
.L318:
	.align	2
.L317:
	.word	.LC5
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.size	rknand_print_hex, .-rknand_print_hex
	.section	.text.FlashEraseBlocks,"ax",%progbits
	.align	1
	.global	FlashEraseBlocks
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FlashEraseBlocks, %function
FlashEraseBlocks:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r6, .L335
	add	r8, r0, #4
	movs	r7, #0
	ldr	fp, .L335+20
	ldrh	r10, [r6, #12]
	str	r2, [sp]
	lsl	r3, r10, #3
	str	r3, [sp, #4]
.L320:
	ldr	r3, [sp]
	cmp	r7, r3
	beq	.L334
	add	r2, sp, #8
	add	r1, sp, #12
	ldr	r0, [r8]
	bl	l2p_addr_tran.isra.0
	ldr	r5, [sp, #8]
	cbnz	r5, .L321
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #4]
	cmp	r3, r2
	bls	.L321
	ldr	r6, .L335+4
	ldr	r7, .L335+8
.L322:
	ldr	r3, [sp]
	adds	r4, r4, #20
	cmp	r5, r3
	bne	.L323
.L334:
	movs	r0, #0
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L323:
	mov	r3, #-1
	ldr	r2, [sp, #12]
	str	r3, [r4, #-20]
	mov	r1, r6
	mov	r0, r7
	adds	r5, r5, #1
	bl	printf
	movs	r3, #16
	movs	r2, #4
	ldr	r1, [r4, #-12]
	ldr	r0, .L335+12
	bl	rknand_print_hex
	movs	r3, #4
	ldr	r1, [r4, #-8]
	mov	r2, r3
	ldr	r0, .L335+16
	bl	rknand_print_hex
	b	.L322
.L321:
	ldr	r2, [fp, #4]
	uxtb	r0, r5
	ldr	r1, [sp, #12]
	blx	r2
	cbnz	r0, .L324
	str	r0, [r8, #-4]
.L325:
	ldrh	r2, [r6, #14]
	cmp	r2, #4
	bne	.L327
	ldr	r1, [sp, #12]
	ldr	r2, [fp, #4]
	ldrb	r0, [sp, #8]	@ zero_extendqisi2
	add	r1, r1, r10
	blx	r2
	cbz	r0, .L327
	mov	r2, #-1
	str	r2, [r8, #-4]
.L327:
	adds	r7, r7, #1
	add	r8, r8, #20
	b	.L320
.L324:
	mov	r2, #-1
	str	r2, [r8, #-4]
	b	.L325
.L336:
	.align	2
.L335:
	.word	.LANCHOR0
	.word	.LANCHOR76
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LANCHOR77
	.size	FlashEraseBlocks, .-FlashEraseBlocks
	.section	.text.FtlFreeSysBlkQueueIn,"ax",%progbits
	.align	1
	.global	FtlFreeSysBlkQueueIn
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlFreeSysBlkQueueIn, %function
FtlFreeSysBlkQueueIn:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	subs	r3, r0, #1
	movw	r2, #65533
	uxth	r3, r3
	push	{r4, r5, r6, lr}
	mov	r5, r0
	cmp	r3, r2
	bhi	.L337
	ldr	r4, .L346
	ldrh	r3, [r4, #6]
	cmp	r3, #1024
	beq	.L337
	cbz	r1, .L339
	bl	P2V_block_in_plane
	ldr	r3, .L346+4
	mov	r6, r0
	movs	r2, #1
	mov	r1, r2
	ldr	r0, [r3]
	lsls	r3, r5, #10
	str	r3, [r0, #4]
	bl	FlashEraseBlocks
	ldr	r3, .L346+8
	ldr	r2, [r3]
	ldrh	r3, [r2, r6, lsl #1]
	adds	r3, r3, #1
	strh	r3, [r2, r6, lsl #1]	@ movhi
	ldr	r2, .L346+12
	ldr	r3, [r2]
	adds	r3, r3, #1
	str	r3, [r2]
.L339:
	ldrh	r3, [r4, #6]
	adds	r3, r3, #1
	strh	r3, [r4, #6]	@ movhi
	ldrh	r3, [r4, #4]
	adds	r2, r3, #4
	adds	r3, r3, #1
	ubfx	r3, r3, #0, #10
	strh	r5, [r4, r2, lsl #1]	@ movhi
	strh	r3, [r4, #4]	@ movhi
.L337:
	pop	{r4, r5, r6, pc}
.L347:
	.align	2
.L346:
	.word	.LANCHOR37
	.word	.LANCHOR78
	.word	.LANCHOR46
	.word	.LANCHOR79
	.size	FtlFreeSysBlkQueueIn, .-FtlFreeSysBlkQueueIn
	.section	.text.FtlFreeSysBlkQueueOut,"ax",%progbits
	.align	1
	.global	FtlFreeSysBlkQueueOut
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlFreeSysBlkQueueOut, %function
FtlFreeSysBlkQueueOut:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	ldr	r4, .L353
	ldrh	r2, [r4, #6]
	cbz	r2, .L352
	ldrh	r3, [r4, #2]
	subs	r2, r2, #1
	strh	r2, [r4, #6]	@ movhi
	movs	r2, #1
	adds	r1, r3, #4
	adds	r3, r3, #1
	ubfx	r3, r3, #0, #10
	ldrh	r5, [r4, r1, lsl #1]
	strh	r3, [r4, #2]	@ movhi
	mov	r1, r2
	ldr	r3, .L353+4
	ldr	r0, [r3]
	lsls	r3, r5, #10
	str	r3, [r0, #4]
	bl	FlashEraseBlocks
	ldr	r2, .L353+8
	ldr	r3, [r2]
	adds	r3, r3, #1
	str	r3, [r2]
.L349:
	subs	r3, r5, #1
	movw	r2, #65533
	uxth	r3, r3
	cmp	r3, r2
	bls	.L350
	ldrh	r2, [r4, #6]
	mov	r1, r5
	ldr	r0, .L353+12
	bl	printf
.L351:
	b	.L351
.L352:
	movw	r5, #65535
	b	.L349
.L350:
	mov	r0, r5
	pop	{r3, r4, r5, pc}
.L354:
	.align	2
.L353:
	.word	.LANCHOR37
	.word	.LANCHOR78
	.word	.LANCHOR79
	.word	.LC9
	.size	FtlFreeSysBlkQueueOut, .-FtlFreeSysBlkQueueOut
	.section	.text.ftl_map_blk_alloc_new_blk,"ax",%progbits
	.align	1
	.global	ftl_map_blk_alloc_new_blk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_map_blk_alloc_new_blk, %function
ftl_map_blk_alloc_new_blk:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r1, [r0, #10]
	ldr	r2, [r0, #12]
	push	{r3, r4, r5, r6, r7, lr}
	mov	r4, r0
	movs	r3, #0
.L356:
	uxth	r5, r3
	cmp	r5, r1
	bcs	.L359
	mov	r7, r2
	adds	r3, r3, #1
	ldrh	r6, [r7]
	adds	r2, r2, #2
	cmp	r6, #0
	bne	.L356
	bl	FtlFreeSysBlkQueueOut
	subs	r3, r0, #1
	movw	r2, #65533
	uxth	r3, r3
	mov	r1, r0
	strh	r0, [r7]	@ movhi
	cmp	r3, r2
	bls	.L357
	ldr	r3, .L363
	ldr	r0, .L363+4
	ldrh	r2, [r3, #6]
	bl	printf
.L358:
	b	.L358
.L357:
	ldr	r3, [r4, #28]
	strh	r6, [r4, #2]	@ movhi
	strh	r5, [r4]	@ movhi
	adds	r3, r3, #1
	str	r3, [r4, #28]
	ldrh	r3, [r4, #8]
	adds	r3, r3, #1
	strh	r3, [r4, #8]	@ movhi
.L359:
	ldrh	r3, [r4, #10]
	cmp	r3, r5
	bhi	.L361
	movw	r2, #578
	ldr	r1, .L363+8
	ldr	r0, .L363+12
	bl	printf
.L361:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L364:
	.align	2
.L363:
	.word	.LANCHOR37
	.word	.LC10
	.word	.LANCHOR80
	.word	.LC1
	.size	ftl_map_blk_alloc_new_blk, .-ftl_map_blk_alloc_new_blk
	.section	.text.ftl_memset,"ax",%progbits
	.align	1
	.global	ftl_memset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_memset, %function
ftl_memset:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	memset
	.size	ftl_memset, .-ftl_memset
	.section	.text.FtlMemInit,"ax",%progbits
	.align	1
	.global	FtlMemInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlMemInit, %function
FtlMemInit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L371
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r4, #0
	ldr	r2, .L371+4
	movs	r6, #12
	str	r4, [r3]
	ldr	r3, .L371+8
	ldr	r5, .L371+12
	ldr	r10, .L371+292
	str	r4, [r3]
	ldr	r3, .L371+16
	ldrh	r0, [r5]
	ldr	r8, .L371+296
	str	r4, [r3]
	ldr	r3, .L371+20
	lsls	r0, r0, #1
	ldr	fp, .L371+300
	str	r4, [r3]
	ldr	r3, .L371+24
	str	r4, [r3]
	ldr	r3, .L371+28
	str	r4, [r3]
	ldr	r3, .L371+32
	str	r4, [r3]
	ldr	r3, .L371+36
	str	r4, [r3]
	ldr	r3, .L371+40
	str	r4, [r3]
	ldr	r3, .L371+44
	str	r4, [r3]
	ldr	r3, .L371+48
	str	r4, [r3]
	ldr	r3, .L371+52
	str	r4, [r3]
	ldr	r3, .L371+56
	str	r4, [r3]
	ldr	r3, .L371+60
	str	r4, [r3]
	ldr	r3, .L371+64
	str	r4, [r3]
	movw	r3, #65535
	str	r3, [r2]
	ldr	r2, .L371+68
	str	r4, [r2]
	ldr	r2, .L371+72
	str	r4, [r2]
	ldr	r2, .L371+76
	str	r4, [r2]
	ldr	r2, .L371+80
	strh	r3, [r2]	@ movhi
	ldr	r2, .L371+84
	strh	r3, [r2]	@ movhi
	movs	r2, #32
	ldr	r3, .L371+88
	strh	r2, [r3]	@ movhi
	movs	r2, #128
	ldr	r3, .L371+92
	strh	r2, [r3]	@ movhi
	ldr	r3, .L371+96
	strh	r4, [r3]	@ movhi
	ldr	r3, .L371+100
	strh	r4, [r3]	@ movhi
	ldr	r3, .L371+104
	strh	r4, [r3]	@ movhi
	bl	ftl_malloc
	ldr	r3, .L371+108
	str	r0, [r3]
	ldrh	r0, [r5]
	movs	r5, #20
	muls	r0, r6, r0
	bl	ftl_malloc
	ldr	r3, .L371+112
	str	r0, [r3]
	ldrh	r3, [r10]
	muls	r5, r3, r5
	lsls	r7, r5, #2
	mov	r0, r7
	bl	ftl_malloc
	ldr	r3, .L371+116
	str	r0, [r3]
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+120
	str	r0, [r3]
	mov	r0, r7
	bl	ftl_malloc
	ldr	r3, .L371+124
	ldr	r7, .L371+128
	str	r0, [r3]
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+132
	str	r0, [r3]
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+136
	ldrh	r5, [r7]
	str	r0, [r3]
	ldrh	r3, [r10]
	mov	r0, r5
	lsls	r3, r3, #1
	adds	r3, r3, #1
	str	r3, [r8]
	bl	ftl_malloc
	ldr	r3, .L371+140
	str	r0, [r3]
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+144
	str	r0, [r3]
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+148
	str	r0, [r3]
	ldr	r0, [r8]
	muls	r0, r5, r0
	bl	ftl_malloc
	ldr	r3, .L371+152
	str	r0, [r3]
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+156
	str	r0, [r3]
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+160
	str	r0, [r3]
	ldr	r0, [r8]
	muls	r0, r6, r0
	bl	ftl_malloc
	ldr	r3, .L371+164
	str	r0, [r3]
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+168
	str	r0, [r3]
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+172
	str	r0, [r3]
	ldr	r3, .L371+176
	ldrh	r0, [r3]
	lsls	r0, r0, #2
	bl	ftl_malloc
	ldr	r3, .L371+180
	ldrh	r5, [r10]
	ldr	r10, .L371+304
	str	r0, [r3]
	ldrh	r3, [fp]
	muls	r5, r3, r5
	mov	r0, r5
	bl	ftl_malloc
	ldr	r3, .L371+184
	str	r0, [r3]
	lsls	r0, r5, #2
	ldr	r5, .L371+188
	bl	ftl_malloc
	ldr	r3, .L371+192
	str	r0, [r3]
	ldrh	r3, [fp]
	ldr	r0, [r8]
	ldr	r8, .L371+308
	muls	r0, r3, r0
	bl	ftl_malloc
	ldr	r3, .L371+196
	str	r0, [r3]
	ldrh	r0, [r5]
	lsls	r0, r0, #1
	uxth	r0, r0
	strh	r0, [r8]	@ movhi
	bl	ftl_malloc
	ldr	r3, .L371+200
	str	r0, [r3]
	ldrh	r3, [r8]
	ldr	r0, .L371+204
	addw	r3, r3, #547
	lsrs	r3, r3, #9
	and	r0, r0, r3, lsl #9
	strh	r3, [r8]	@ movhi
	bl	ftl_malloc
	ldr	r3, .L371+208
	str	r0, [r3]
	adds	r0, r0, #32
	ldr	r3, .L371+212
	str	r0, [r3]
	ldrh	r0, [r5]
	lsls	r0, r0, #1
	bl	ftl_malloc
	ldr	r3, .L371+216
	str	r0, [r3]
	ldr	r3, [r10]
	lsl	r8, r3, #1
	mov	r0, r8
	bl	ftl_malloc
	ldr	r3, .L371+220
	str	r0, [r3]
	mov	r0, r8
	bl	ftl_malloc
	ldr	r8, .L371+312
	ldr	r3, .L371+224
	str	r0, [r3]
	ldrh	r0, [r8]
	lsls	r0, r0, #1
	bl	ftl_malloc
	ldr	r3, .L371+228
	str	r0, [r3]
	ldrh	r0, [r8]
	lsls	r0, r0, #1
	bl	ftl_malloc
	ldr	r3, .L371+232
	str	r0, [r3]
	ldrh	r0, [r8]
	ldr	r8, .L371+316
	lsls	r0, r0, #2
	bl	ftl_malloc
	ldr	r3, .L371+236
	str	r0, [r3]
	ldrh	r0, [r8]
	lsls	r0, r0, #2
	bl	ftl_malloc
	ldrh	r2, [r8]
	mov	r1, r4
	ldr	r3, .L371+240
	ldr	r4, .L371+244
	lsls	r2, r2, #2
	str	r0, [r3]
	bl	ftl_memset
	ldr	r3, .L371+248
	ldrh	r0, [r3]
	lsls	r0, r0, #2
	bl	ftl_malloc
	ldr	r3, .L371+252
	str	r0, [r3]
	ldr	r0, [r10]
	lsls	r0, r0, #2
	bl	ftl_malloc
	ldr	r3, .L371+256
	str	r0, [r3]
	ldrh	r0, [r4]
	muls	r0, r6, r0
	ldr	r6, .L371+260
	bl	ftl_malloc
	ldr	r3, .L371+264
	str	r0, [r3]
	ldrh	r3, [r4]
	ldrh	r0, [r7]
	muls	r0, r3, r0
	bl	ftl_malloc
	ldr	r3, .L371+268
	str	r0, [r3]
	movs	r0, #6
	ldrh	r3, [r5]
	ldr	r5, .L371+272
	muls	r0, r3, r0
	bl	ftl_malloc
	ldr	r3, .L371+276
	str	r0, [r3]
	ldr	r3, .L371+280
	ldrh	r0, [r3]
	ldrh	r3, [r6]
	adds	r0, r0, #31
	asrs	r0, r0, #5
	strh	r0, [r5]	@ movhi
	muls	r0, r3, r0
	lsls	r0, r0, #2
	bl	ftl_malloc
	ldrh	r1, [r5]
	movs	r2, #1
	ldr	r3, .L371+284
	ldrh	r6, [r6]
	lsls	r1, r1, #2
	mov	r4, r3
	str	r0, [r4, #28]!
	mov	r0, r1
.L367:
	cmp	r2, r6
	bcc	.L368
	add	r3, r3, r2, lsl #2
	ldr	r2, .L371+288
	movs	r1, #0
	adds	r3, r3, #24
.L369:
	cmp	r3, r2
	bne	.L370
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L372:
	.align	2
.L371:
	.word	.LANCHOR81
	.word	.LANCHOR95
	.word	.LANCHOR82
	.word	.LANCHOR20
	.word	.LANCHOR83
	.word	.LANCHOR84
	.word	.LANCHOR85
	.word	.LANCHOR86
	.word	.LANCHOR87
	.word	.LANCHOR88
	.word	.LANCHOR89
	.word	.LANCHOR90
	.word	.LANCHOR79
	.word	.LANCHOR91
	.word	.LANCHOR92
	.word	.LANCHOR93
	.word	.LANCHOR94
	.word	.LANCHOR96
	.word	.LANCHOR97
	.word	.LANCHOR98
	.word	.LANCHOR71
	.word	.LANCHOR72
	.word	.LANCHOR99
	.word	.LANCHOR100
	.word	.LANCHOR101
	.word	.LANCHOR73
	.word	.LANCHOR75
	.word	.LANCHOR67
	.word	.LANCHOR70
	.word	.LANCHOR102
	.word	.LANCHOR103
	.word	.LANCHOR104
	.word	.LANCHOR22
	.word	.LANCHOR78
	.word	.LANCHOR65
	.word	.LANCHOR105
	.word	.LANCHOR106
	.word	.LANCHOR107
	.word	.LANCHOR63
	.word	.LANCHOR108
	.word	.LANCHOR109
	.word	.LANCHOR62
	.word	.LANCHOR110
	.word	.LANCHOR111
	.word	.LANCHOR11
	.word	.LANCHOR112
	.word	.LANCHOR113
	.word	.LANCHOR5
	.word	.LANCHOR114
	.word	.LANCHOR64
	.word	.LANCHOR116
	.word	33553920
	.word	.LANCHOR117
	.word	.LANCHOR46
	.word	.LANCHOR41
	.word	.LANCHOR118
	.word	.LANCHOR119
	.word	.LANCHOR35
	.word	.LANCHOR120
	.word	.LANCHOR121
	.word	.LANCHOR122
	.word	.LANCHOR32
	.word	.LANCHOR31
	.word	.LANCHOR123
	.word	.LANCHOR124
	.word	.LANCHOR9
	.word	.LANCHOR54
	.word	.LANCHOR125
	.word	.LANCHOR126
	.word	.LANCHOR39
	.word	.LANCHOR16
	.word	.LANCHOR36
	.word	.LANCHOR36+56
	.word	.LANCHOR2
	.word	.LANCHOR66
	.word	.LANCHOR23
	.word	.LANCHOR29
	.word	.LANCHOR115
	.word	.LANCHOR26
	.word	.LANCHOR27
.L368:
	ldr	r5, [r3, #28]
	adds	r2, r2, #1
	add	r5, r5, r0
	add	r0, r0, r1
	str	r5, [r4, #4]!
	b	.L367
.L370:
	str	r1, [r3, #4]!
	b	.L369
	.size	FtlMemInit, .-FtlMemInit
	.section	.text.FtlBbt2Bitmap,"ax",%progbits
	.align	1
	.global	FtlBbt2Bitmap
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlBbt2Bitmap, %function
FtlBbt2Bitmap:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L379
	push	{r4, r5, r6, r7, r8, lr}
	mov	r5, r0
	ldr	r7, .L379+4
	mov	r6, r1
	subs	r4, r5, #2
	addw	r5, r5, #1022
	ldrh	r2, [r3]
	movs	r1, #0
	ldr	r8, .L379+12
	mov	r0, r6
	lsls	r2, r2, #2
	bl	ftl_memset
.L376:
	ldrh	r3, [r4, #2]
	movw	r2, #65535
	cmp	r3, r2
	beq	.L373
	ldrh	r2, [r7]
	cmp	r2, r3
	bhi	.L375
	movs	r2, #74
	mov	r1, r8
	ldr	r0, .L379+8
	bl	printf
.L375:
	ldrh	r3, [r4, #2]!
	movs	r2, #1
	cmp	r5, r4
	lsr	r1, r3, #5
	and	r3, r3, #31
	lsl	r3, r2, r3
	ldr	r2, [r6, r1, lsl #2]
	orr	r2, r2, r3
	str	r2, [r6, r1, lsl #2]
	bne	.L376
.L373:
	pop	{r4, r5, r6, r7, r8, pc}
.L380:
	.align	2
.L379:
	.word	.LANCHOR126
	.word	.LANCHOR16
	.word	.LC1
	.word	.LANCHOR127
	.size	FtlBbt2Bitmap, .-FtlBbt2Bitmap
	.section	.text.FtlBbtMemInit,"ax",%progbits
	.align	1
	.global	FtlBbtMemInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlBbtMemInit, %function
FtlBbtMemInit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L382
	movw	r3, #65535
	movs	r2, #16
	movs	r1, #255
	strh	r3, [r0]	@ movhi
	movs	r3, #0
	strh	r3, [r0, #6]	@ movhi
	adds	r0, r0, #12
	b	ftl_memset
.L383:
	.align	2
.L382:
	.word	.LANCHOR36
	.size	FtlBbtMemInit, .-FtlBbtMemInit
	.section	.text.FtlFreeSysBlkQueueInit,"ax",%progbits
	.align	1
	.global	FtlFreeSysBlkQueueInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlFreeSysBlkQueueInit, %function
FtlFreeSysBlkQueueInit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L385
	mov	r2, #2048
	push	{r4, lr}
	movs	r4, #0
	mov	r1, r4
	strh	r4, [r3, #2]	@ movhi
	strh	r4, [r3, #4]	@ movhi
	strh	r4, [r3, #6]	@ movhi
	strh	r0, [r3], #8	@ movhi
	mov	r0, r3
	bl	ftl_memset
	mov	r0, r4
	pop	{r4, pc}
.L386:
	.align	2
.L385:
	.word	.LANCHOR37
	.size	FtlFreeSysBlkQueueInit, .-FtlFreeSysBlkQueueInit
	.section	.text.ftl_free_no_use_map_blk,"ax",%progbits
	.align	1
	.global	ftl_free_no_use_map_blk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_free_no_use_map_blk, %function
ftl_free_no_use_map_blk:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r2, [r0, #10]
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r5, [r0, #20]
	movs	r1, #0
	ldr	r7, [r0, #12]
	lsls	r2, r2, #1
	ldr	r6, [r0, #24]
	mov	r0, r5
	bl	ftl_memset
	movs	r3, #0
.L388:
	ldrh	r1, [r4, #6]
	uxth	r2, r3
	cmp	r1, r2
	bhi	.L392
	ldr	r3, .L407
	movs	r6, #0
	mov	r10, r6
	ldrh	r2, [r3]
	ldrh	r3, [r4]
	strh	r2, [r5, r3, lsl #1]	@ movhi
	mov	r2, r6
	ldrh	fp, [r5]
.L393:
	ldrh	r3, [r4, #10]
	uxth	ip, r6
	cmp	r3, ip
	bhi	.L397
	mov	r0, r10
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L392:
	uxth	r2, r3
	ldr	r1, [r6, r2, lsl #2]
	movs	r2, #0
	ubfx	r1, r1, #10, #16
.L389:
	ldrh	ip, [r4, #10]
	uxth	r0, r2
	cmp	ip, r0
	bhi	.L391
	adds	r3, r3, #1
	b	.L388
.L391:
	uxth	r0, r2
	ldrh	ip, [r7, r0, lsl #1]
	cmp	ip, r1
	bne	.L390
	cbz	r1, .L390
	ldrh	ip, [r5, r0, lsl #1]
	add	ip, ip, #1
	strh	ip, [r5, r0, lsl #1]	@ movhi
.L390:
	adds	r2, r2, #1
	b	.L389
.L397:
	uxth	r1, r6
	ldrh	r3, [r5, r1, lsl #1]
	lsl	r8, r1, #1
	cmp	fp, r3
	bls	.L394
	ldrh	r0, [r7, r1, lsl #1]
	add	r8, r8, r7
	cbnz	r0, .L395
.L396:
	adds	r6, r6, #1
	b	.L393
.L394:
	cmp	r3, #0
	bne	.L396
	ldrh	r0, [r7, r1, lsl #1]
	add	r8, r8, r7
	cmp	r0, #0
	beq	.L396
.L398:
	movs	r1, #1
	str	r2, [sp, #4]
	bl	FtlFreeSysBlkQueueIn
	ldr	r2, [sp, #4]
	strh	r2, [r8]	@ movhi
	ldrh	r3, [r4, #8]
	subs	r3, r3, #1
	strh	r3, [r4, #8]	@ movhi
	b	.L396
.L395:
	mov	r10, ip
	mov	fp, r3
	cmp	r3, #0
	beq	.L398
	b	.L396
.L408:
	.align	2
.L407:
	.word	.LANCHOR19
	.size	ftl_free_no_use_map_blk, .-ftl_free_no_use_map_blk
	.section	.text.FtlL2PDataInit,"ax",%progbits
	.align	1
	.global	FtlL2PDataInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlL2PDataInit, %function
FtlL2PDataInit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	movs	r1, #0
	ldr	r5, .L412
	ldr	r4, .L412+4
	ldr	r2, [r5]
	ldr	r7, .L412+8
	ldr	r6, .L412+12
	ldr	r0, [r4]
	lsls	r2, r2, #1
	ldr	r8, .L412+44
	bl	ftl_memset
	ldrh	r3, [r7]
	movs	r1, #255
	ldrh	r2, [r6]
	ldr	r0, [r8]
	muls	r2, r3, r2
	bl	ftl_memset
	ldr	r3, .L412+16
	movw	r0, #65535
	ldrh	r1, [r6]
	ldr	ip, [r8]
	ldr	r2, [r3]
	movs	r3, #12
	ldrh	r7, [r7]
	mla	r3, r1, r3, r3
	adds	r1, r2, r3
	movs	r3, #0
	mov	lr, r3
.L410:
	adds	r2, r2, #12
	adds	r6, r3, r7
	cmp	r2, r1
	bne	.L411
	ldr	r3, .L412+20
	ldr	r2, [r5]
	strh	r0, [r3, #2]	@ movhi
	strh	r2, [r3, #10]	@ movhi
	movw	r2, #61634
	strh	r2, [r3, #4]	@ movhi
	ldr	r2, .L412+24
	strh	r0, [r3]	@ movhi
	ldrh	r2, [r2]
	strh	r2, [r3, #8]	@ movhi
	ldr	r2, .L412+28
	ldrh	r2, [r2]
	strh	r2, [r3, #6]	@ movhi
	ldr	r2, .L412+32
	ldr	r2, [r2]
	str	r2, [r3, #12]
	ldr	r2, .L412+36
	ldr	r2, [r2]
	str	r2, [r3, #16]
	ldr	r2, [r4]
	str	r2, [r3, #20]
	ldr	r2, .L412+40
	ldr	r2, [r2]
	str	r2, [r3, #24]
	pop	{r4, r5, r6, r7, r8, pc}
.L411:
	bic	r3, r3, #3
	str	lr, [r2, #-8]
	add	r3, r3, ip
	strh	r0, [r2, #-12]	@ movhi
	str	r3, [r2, #-4]
	mov	r3, r6
	b	.L410
.L413:
	.align	2
.L412:
	.word	.LANCHOR29
	.word	.LANCHOR119
	.word	.LANCHOR22
	.word	.LANCHOR32
	.word	.LANCHOR54
	.word	.LANCHOR128
	.word	.LANCHOR129
	.word	.LANCHOR31
	.word	.LANCHOR118
	.word	.LANCHOR124
	.word	.LANCHOR123
	.word	.LANCHOR125
	.size	FtlL2PDataInit, .-FtlL2PDataInit
	.section	.text.FtlVariablesInit,"ax",%progbits
	.align	1
	.global	FtlVariablesInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlVariablesInit, %function
FtlVariablesInit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movw	r2, #65535
	ldr	r3, .L415
	movs	r4, #0
	mov	r1, r4
	ldr	r5, .L415+4
	strh	r2, [r3]	@ movhi
	mov	r2, #-1
	ldr	r3, .L415+8
	str	r4, [r3]
	ldr	r3, .L415+12
	str	r4, [r3]
	ldr	r3, .L415+16
	str	r2, [r3]
	ldr	r3, .L415+20
	strh	r4, [r3]	@ movhi
	ldr	r3, .L415+24
	ldrh	r2, [r3]
	ldr	r3, .L415+28
	lsls	r2, r2, #1
	ldr	r0, [r3]
	bl	ftl_memset
	ldrh	r2, [r5]
	mov	r1, r4
	ldr	r3, .L415+32
	lsls	r2, r2, #1
	ldr	r0, [r3]
	bl	ftl_memset
	ldrh	r2, [r5]
	mov	r1, r4
	ldr	r3, .L415+36
	lsls	r2, r2, #1
	ldr	r0, [r3]
	bl	ftl_memset
	mov	r1, r4
	movs	r2, #48
	ldr	r0, .L415+40
	bl	ftl_memset
	mov	r2, #512
	mov	r1, r4
	ldr	r0, .L415+44
	bl	ftl_memset
	bl	FtlGcBufInit
	bl	FtlL2PDataInit
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L416:
	.align	2
.L415:
	.word	.LANCHOR130
	.word	.LANCHOR5
	.word	.LANCHOR131
	.word	.LANCHOR132
	.word	.LANCHOR133
	.word	.LANCHOR34
	.word	.LANCHOR26
	.word	.LANCHOR35
	.word	.LANCHOR46
	.word	.LANCHOR116
	.word	.LANCHOR38
	.word	.LANCHOR134
	.size	FtlVariablesInit, .-FtlVariablesInit
	.section	.text.SupperBlkListInit,"ax",%progbits
	.align	1
	.global	SupperBlkListInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	SupperBlkListInit, %function
SupperBlkListInit:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r2, #6
	ldr	r3, .L428
	movs	r1, #0
	movs	r4, #0
	ldr	r6, .L428+4
	mov	r8, r4
	ldrh	r3, [r3]
	mov	r5, r4
	ldr	r0, [r6]
	mov	r10, r6
	muls	r2, r3, r2
	bl	ftl_memset
	ldr	r3, .L428+8
	ldr	r2, .L428+12
	str	r4, [r3]
	ldr	r3, .L428+16
	strh	r4, [r2]	@ movhi
	str	r2, [sp, #4]
	str	r4, [r3]
	ldr	r3, .L428+20
	str	r4, [r3]
	ldr	r3, .L428+24
	mov	fp, r3
	strh	r4, [r3]	@ movhi
.L418:
	ldr	r3, .L428+28
	uxth	r7, r4
	ldrh	r3, [r3]
	cmp	r7, r3
	bcs	.L425
	ldr	r3, .L428+32
	ldrh	r2, [r3]
	ldr	r3, .L428+36
	ldrh	r3, [r3]
	str	r3, [sp]
	movs	r3, #0
	mov	r6, r3
	b	.L426
.L420:
	str	r2, [sp, #12]
	mov	r1, r7
	ldr	r2, .L428+40
	str	r3, [sp, #8]
	ldrb	r0, [r2, r3]	@ zero_extendqisi2
	bl	V2P_block
	bl	FtlBbmIsBadBlock
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #12]
	cbnz	r0, .L419
	ldr	r1, [sp]
	add	r6, r6, r1
	uxth	r6, r6
.L419:
	adds	r3, r3, #1
.L426:
	uxth	r1, r3
	cmp	r2, r1
	bhi	.L420
	uxth	r3, r4
	cbz	r6, .L421
	mov	r1, r6
	str	r3, [sp]
	mov	r0, #32768
	bl	__aeabi_idiv
	ldr	r3, [sp]
	uxth	r6, r0
.L422:
	ldr	r1, [r10]
	movs	r2, #6
	mla	r2, r2, r3, r1
	strh	r6, [r2, #4]	@ movhi
	ldr	r2, .L428+44
	ldrh	r2, [r2]
	cmp	r2, r7
	beq	.L423
	ldr	r2, .L428+48
	ldrh	r2, [r2]
	cmp	r2, r7
	beq	.L423
	ldr	r2, .L428+52
	ldrh	r2, [r2]
	cmp	r2, r7
	beq	.L423
	ldr	r2, .L428+56
	ldr	r2, [r2]
	ldrh	r3, [r2, r3, lsl #1]
	cbnz	r3, .L424
	add	r8, r8, #1
	mov	r0, r7
	uxth	r8, r8
	bl	INSERT_FREE_LIST
.L423:
	adds	r4, r4, #1
	b	.L418
.L421:
	ldr	r2, .L428+56
	movw	r1, #65535
	ldr	r2, [r2]
	strh	r1, [r2, r3, lsl #1]	@ movhi
	b	.L422
.L424:
	adds	r5, r5, #1
	mov	r0, r7
	uxth	r5, r5
	bl	INSERT_DATA_LIST
	b	.L423
.L425:
	ldr	r2, [sp, #4]
	strh	r8, [fp]	@ movhi
	strh	r5, [r2]	@ movhi
	add	r5, r5, r8
	cmp	r5, r3
	ble	.L427
	movw	r2, #2219
	ldr	r1, .L428+60
	ldr	r0, .L428+64
	bl	printf
.L427:
	movs	r0, #0
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L429:
	.align	2
.L428:
	.word	.LANCHOR5
	.word	.LANCHOR39
	.word	.LANCHOR45
	.word	.LANCHOR43
	.word	.LANCHOR40
	.word	.LANCHOR42
	.word	.LANCHOR47
	.word	.LANCHOR4
	.word	.LANCHOR2
	.word	.LANCHOR18
	.word	.LANCHOR12
	.word	.LANCHOR50
	.word	.LANCHOR51
	.word	.LANCHOR52
	.word	.LANCHOR41
	.word	.LANCHOR135
	.word	.LC1
	.size	SupperBlkListInit, .-SupperBlkListInit
	.section	.text.FtlGcPageVarInit,"ax",%progbits
	.align	1
	.global	FtlGcPageVarInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcPageVarInit, %function
FtlGcPageVarInit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r2, .L431
	movs	r3, #0
	push	{r4, lr}
	movs	r1, #255
	ldr	r4, .L431+4
	strh	r3, [r2]	@ movhi
	ldr	r2, .L431+8
	strh	r3, [r2]	@ movhi
	ldrh	r2, [r4]
	ldr	r3, .L431+12
	lsls	r2, r2, #1
	ldr	r0, [r3]
	bl	ftl_memset
	ldrh	r3, [r4]
	movs	r2, #12
	movs	r1, #255
	muls	r2, r3, r2
	ldr	r3, .L431+16
	ldr	r0, [r3]
	bl	ftl_memset
	pop	{r4, lr}
	b	FtlGcBufInit
.L432:
	.align	2
.L431:
	.word	.LANCHOR68
	.word	.LANCHOR20
	.word	.LANCHOR69
	.word	.LANCHOR67
	.word	.LANCHOR70
	.size	FtlGcPageVarInit, .-FtlGcPageVarInit
	.section	.text.FlashGetBadBlockList,"ax",%progbits
	.align	1
	.global	FlashGetBadBlockList
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FlashGetBadBlockList, %function
FlashGetBadBlockList:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r2, #256
	mov	r5, r1
	movs	r1, #255
	mov	r4, r0
	bl	ftl_memset
	ldr	r3, .L440
	mov	r1, r5
	mov	r0, r4
	ldr	r3, [r3]
	blx	r3
	uxth	r0, r0
	cmp	r0, #50
	bls	.L434
	mov	r2, #256
	movs	r1, #255
	mov	r0, r4
	bl	ftl_memset
	movs	r0, #0
.L434:
	ldr	r3, .L440+4
	ldrh	r3, [r3, #14]
	cmp	r3, #4
	bne	.L439
	add	r1, r4, r0, lsl #1
	mov	r3, r4
.L436:
	cmp	r3, r1
	bne	.L437
.L439:
	pop	{r3, r4, r5, pc}
.L437:
	ldrh	r2, [r3]
	lsrs	r2, r2, #1
	strh	r2, [r3], #2	@ movhi
	b	.L436
.L441:
	.align	2
.L440:
	.word	.LANCHOR77
	.word	.LANCHOR0
	.size	FlashGetBadBlockList, .-FlashGetBadBlockList
	.section	.text.ftl_memcpy,"ax",%progbits
	.align	1
	.global	ftl_memcpy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_memcpy, %function
ftl_memcpy:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	memcpy
	.size	ftl_memcpy, .-ftl_memcpy
	.section	.text.FlashReadPages,"ax",%progbits
	.align	1
	.global	FlashReadPages
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FlashReadPages, %function
FlashReadPages:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	fp, r1
	ldr	r3, .L483
	mov	r4, r0
	mov	r10, #0
	ldrh	r2, [r3, #12]
	str	r3, [sp, #4]
	str	r2, [sp]
.L444:
	cmp	r10, fp
	bne	.L458
	movs	r0, #0
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L458:
	ldr	r3, [r4, #8]
	cbz	r3, .L445
	ldr	r3, [r4, #12]
	cbnz	r3, .L446
.L445:
	movs	r2, #90
	ldr	r1, .L483+4
	ldr	r0, .L483+8
	bl	printf
.L446:
	add	r2, sp, #8
	add	r1, sp, #12
	ldr	r0, [r4, #4]
	bl	l2p_addr_tran.isra.0
	ldr	r0, [sp, #8]
	cmp	r0, #3
	bls	.L447
	mov	r3, #-1
	str	r3, [r4]
.L448:
	add	r10, r10, #1
	adds	r4, r4, #20
	b	.L444
.L447:
	ldr	r5, [r4, #8]
	uxtb	r0, r0
	ldr	r8, .L483+32
	ldr	r7, .L483+12
	tst	r5, #63
	ldr	r3, [r4, #12]
	it	ne
	ldrne	r5, [r8]
	ldr	r1, [sp, #12]
	ldr	r6, [r7, #12]
	mov	r2, r5
	blx	r6
	ldr	r3, [sp, #4]
	str	r0, [r4]
	ldrh	r3, [r3, #14]
	cmp	r3, #4
	bne	.L451
	ldr	r0, [sp]
	add	r2, r5, #2048
	ldr	r3, [r4, #12]
	ldr	r1, [sp, #12]
	ldr	r7, [r7, #12]
	adds	r3, r3, #8
	add	r1, r1, r0
	ldrb	r0, [sp, #8]	@ zero_extendqisi2
	blx	r7
	adds	r1, r0, #1
	beq	.L452
	ldr	r3, [r4, #12]
	ldr	r2, [r3, #12]
	adds	r2, r2, #1
	bne	.L453
	ldr	r2, [r3, #8]
	adds	r2, r2, #1
	bne	.L453
	ldr	r3, [r3]
	adds	r3, r3, #1
	beq	.L453
.L452:
	mov	r3, #-1
	str	r3, [r4]
.L453:
	ldr	r3, [r4]
	adds	r3, r3, #1
	beq	.L454
	cmp	r0, #256
	it	eq
	streq	r0, [r4]
.L454:
	ldr	r3, [r4]
	adds	r2, r3, #1
	beq	.L455
	cmp	r3, #256
	bne	.L451
.L455:
	ldr	r1, [r4, #4]
	ldr	r2, [sp, #12]
	ldr	r0, .L483+16
	bl	printf
	ldr	r1, [r4, #8]
	cbz	r1, .L457
	movs	r3, #4
	ldr	r0, .L483+20
	mov	r2, r3
	bl	rknand_print_hex
.L457:
	ldr	r1, [r4, #12]
	cbz	r1, .L451
	movs	r3, #4
	ldr	r0, .L483+24
	mov	r2, r3
	bl	rknand_print_hex
.L451:
	ldr	r3, [r8]
	cmp	r5, r3
	bne	.L448
	ldr	r0, [r4, #8]
	cmp	r5, r0
	beq	.L448
	ldr	r3, .L483+28
	mov	r1, r5
	ldrh	r2, [r3]
	lsls	r2, r2, #9
	bl	ftl_memcpy
	b	.L448
.L484:
	.align	2
.L483:
	.word	.LANCHOR0
	.word	.LANCHOR136
	.word	.LC1
	.word	.LANCHOR77
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LANCHOR11
	.word	.LANCHOR110
	.size	FlashReadPages, .-FlashReadPages
	.section	.text.FtlLoadFactoryBbt,"ax",%progbits
	.align	1
	.global	FtlLoadFactoryBbt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlLoadFactoryBbt, %function
FtlLoadFactoryBbt:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L495
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r6, #0
	ldr	r5, .L495+4
	ldr	r3, [r3]
	ldr	r7, .L495+8
	ldr	r10, .L495+20
	str	r3, [r5, #8]
	ldr	r3, .L495+12
	ldr	r8, [r3]
	str	r8, [r5, #12]
.L486:
	ldr	r3, .L495+16
	ldrh	r3, [r3]
	cmp	r6, r3
	bcc	.L491
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L491:
	ldrh	r4, [r10]
	movw	r3, #65535
	ldr	fp, .L495+4
	strh	r3, [r7, #2]!	@ movhi
	subs	r4, r4, #1
	uxth	r4, r4
.L487:
	ldrh	r3, [r10]
	sub	r2, r3, #15
	cmp	r2, r4
	bgt	.L489
	mla	r3, r6, r3, r4
	movs	r2, #1
	mov	r1, r2
	mov	r0, fp
	lsls	r3, r3, #10
	str	r3, [r5, #4]
	bl	FlashReadPages
	ldr	r3, [r5]
	adds	r3, r3, #1
	beq	.L488
	ldrh	r2, [r8]
	movw	r3, #61664
	cmp	r2, r3
	bne	.L488
	strh	r4, [r7]	@ movhi
.L489:
	adds	r6, r6, #1
	b	.L486
.L488:
	subs	r4, r4, #1
	uxth	r4, r4
	b	.L487
.L496:
	.align	2
.L495:
	.word	.LANCHOR105
	.word	.LANCHOR137
	.word	.LANCHOR36+10
	.word	.LANCHOR113
	.word	.LANCHOR9
	.word	.LANCHOR16
	.size	FtlLoadFactoryBbt, .-FtlLoadFactoryBbt
	.section	.text.FtlGetLastWrittenPage,"ax",%progbits
	.align	1
	.global	FtlGetLastWrittenPage
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGetLastWrittenPage, %function
FtlGetLastWrittenPage:
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r1, #1
	push	{r4, r5, r6, r7, r8, lr}
	it	eq
	ldreq	r3, .L509
	sub	sp, sp, #88
	lsl	r7, r0, #10
	mov	r2, r1
	it	ne
	ldrne	r3, .L509+4
	mov	r6, r1
	add	r0, sp, #4
	movs	r1, #1
	ldrh	r5, [r3]
	ldr	r3, .L509+8
	subs	r5, r5, #1
	ldr	r3, [r3]
	sxth	r5, r5
	str	r3, [sp, #12]
	add	r3, sp, #24
	str	r3, [sp, #16]
	orr	r3, r5, r7
	str	r3, [sp, #8]
	bl	FlashReadPages
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	bne	.L500
	mov	r8, #0
.L501:
	cmp	r8, r5
	ble	.L504
.L500:
	mov	r0, r5
	add	sp, sp, #88
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L504:
	add	r3, r8, r5
	mov	r2, r6
	add	r3, r3, r3, lsr #31
	movs	r1, #1
	add	r0, sp, #4
	asrs	r4, r3, #1
	sxth	r3, r4
	orrs	r3, r3, r7
	str	r3, [sp, #8]
	bl	FlashReadPages
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	bne	.L502
	ldr	r3, [sp, #28]
	adds	r3, r3, #1
	bne	.L502
	ldr	r3, [sp, #4]
	adds	r3, r3, #1
	beq	.L502
	subs	r4, r4, #1
	sxth	r5, r4
	b	.L501
.L502:
	adds	r4, r4, #1
	sxth	r8, r4
	b	.L501
.L510:
	.align	2
.L509:
	.word	.LANCHOR19
	.word	.LANCHOR18
	.word	.LANCHOR111
	.size	FtlGetLastWrittenPage, .-FtlGetLastWrittenPage
	.section	.text.FtlScanSysBlk,"ax",%progbits
	.align	1
	.global	FtlScanSysBlk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlScanSysBlk, %function
FtlScanSysBlk:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r4, #0
	ldr	r5, .L592
	sub	sp, sp, #32
	mov	r1, r4
	ldr	r3, .L592+4
	ldr	r2, [r5]
	ldr	r6, .L592+8
	strh	r4, [r3]	@ movhi
	ldr	r3, .L592+12
	lsls	r2, r2, #2
	strh	r4, [r6]	@ movhi
	ldr	r7, .L592+16
	ldr	r0, [r3]
	bl	ftl_memset
	ldr	r2, [r5]
	mov	r1, r4
	ldr	r3, .L592+20
	lsls	r2, r2, #1
	ldr	r0, [r3]
	bl	ftl_memset
	ldrh	r2, [r7]
	mov	r1, r4
	ldr	r3, .L592+24
	lsls	r2, r2, #2
	ldr	r0, [r3]
	bl	ftl_memset
	ldrh	r2, [r7]
	mov	r1, r4
	ldr	r3, .L592+28
	lsls	r2, r2, #1
	ldr	r0, [r3]
	bl	ftl_memset
	movs	r2, #16
	movs	r1, #255
	ldr	r0, .L592+32
	bl	ftl_memset
	ldr	r3, .L592+36
	str	r6, [sp, #12]
	str	r5, [sp, #16]
	ldrh	r3, [r3]
	str	r3, [sp, #4]
.L512:
	ldr	r3, .L592+40
	ldr	r2, [sp, #4]
	ldrh	r3, [r3]
	cmp	r3, r2
	bls	.L553
	ldr	r3, .L592+44
	movs	r5, #0
	ldr	r1, .L592+48
	mov	fp, r5
	movs	r7, #20
	ldrh	r8, [r3]
	ldr	r3, .L592+52
	ldr	r2, [r1]
	ldr	r1, .L592+56
	ldr	r6, [r3]
	ldr	r3, .L592+60
	str	r2, [sp, #8]
	ldrh	r10, [r1]
	ldr	r3, [r3]
	ldr	r2, .L592+64
	b	.L554
.L514:
	ldrb	r0, [r2, r5]	@ zero_extendqisi2
	ldr	r1, [sp, #4]
	str	r3, [sp, #28]
	str	r2, [sp, #24]
	bl	V2P_block
	str	r0, [sp, #20]
	bl	FtlBbmIsBadBlock
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #28]
	cbnz	r0, .L513
	ldr	r1, [sp, #20]
	mla	r0, r7, fp, r6
	ldr	r4, [sp, #8]
	lsls	r1, r1, #10
	str	r3, [r0, #8]
	str	r1, [r0, #4]
	mul	r1, r10, fp
	bic	r1, r1, #3
	add	r1, r1, r4
	str	r1, [r0, #12]
	add	r1, fp, #1
	uxth	fp, r1
.L513:
	adds	r5, r5, #1
.L554:
	uxth	r1, r5
	cmp	r8, r1
	bhi	.L514
	cmp	fp, #0
	bne	.L515
.L552:
	ldr	r3, [sp, #4]
	adds	r3, r3, #1
	uxth	r3, r3
	str	r3, [sp, #4]
	b	.L512
.L515:
	movs	r7, #0
	movs	r2, #1
	mov	r1, fp
	mov	r0, r6
	bl	FlashReadPages
.L516:
	uxth	r3, r7
	cmp	fp, r3
	bls	.L552
	ldr	r3, .L592+52
	mov	r8, #20
	mul	r8, r8, r7
	ldr	r3, [r3]
	add	r2, r3, r8
	ldr	r3, [r3, r8]
	ldr	r5, [r2, #4]
	ldr	r6, [r2, #12]
	adds	r3, r3, #1
	ubfx	r5, r5, #10, #16
	bne	.L519
	mov	r10, #16
.L521:
	ldr	r3, .L592+52
	movs	r2, #1
	mov	r1, r2
	ldr	r0, [r3]
	add	r0, r0, r8
	ldr	r3, [r0, #4]
	adds	r3, r3, #1
	str	r3, [r0, #4]
	bl	FlashReadPages
	ldrh	r2, [r6]
	movw	r3, #65535
	cmp	r2, r3
	ldr	r3, .L592+52
	ldr	r3, [r3]
	bne	.L518
	mov	r2, #-1
	str	r2, [r3, r8]
	ldr	r3, .L592+52
	ldr	r3, [r3]
	ldr	r3, [r3, r8]
	cmp	r3, r2
	bne	.L519
.L520:
	movs	r1, #1
	b	.L591
.L518:
	ldr	r3, [r3, r8]
	adds	r3, r3, #1
	bne	.L519
	add	r10, r10, #-1
	uxth	r10, r10
	cmp	r10, #0
	bne	.L521
	b	.L520
.L519:
	ldr	r3, .L592+68
	ldr	r2, [r3]
	ldr	r3, [r6, #4]
	adds	r1, r2, #1
	beq	.L522
	cmp	r2, r3
	bhi	.L523
.L522:
	adds	r2, r3, #1
	ittt	ne
	ldrne	r1, .L592+68
	addne	r2, r3, #1
	strne	r2, [r1]
.L523:
	ldrh	r2, [r6]
	movw	r1, #61604
	cmp	r2, r1
	beq	.L525
	bhi	.L526
	movw	r3, #61574
	cmp	r2, r3
	beq	.L527
.L524:
	adds	r7, r7, #1
	b	.L516
.L526:
	movw	r3, #61634
	cmp	r2, r3
	beq	.L528
	movw	r3, #65535
	cmp	r2, r3
	bne	.L524
	movs	r1, #0
.L591:
	mov	r0, r5
	bl	FtlFreeSysBlkQueueIn
	b	.L524
.L528:
	ldr	r3, .L592+8
	ldrh	r2, [r3]
	ldr	r3, .L592
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L530
	mov	r2, #1232
	ldr	r1, .L592+72
	ldr	r0, .L592+76
	bl	printf
.L530:
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #12]
	ldr	r1, [r3]
	ldrh	r0, [r2]
	ldr	r2, .L592+12
	uxth	r10, r1
	ldr	ip, [r2]
	add	r3, r10, #-1
	sub	r10, r10, r0
	add	r10, r10, #-1
	sxth	r3, r3
	sxth	r10, r10
.L531:
	cmp	r3, r10
	bgt	.L537
	cmp	r3, #0
	bge	.L569
	b	.L524
.L537:
	ldr	r2, [ip, r3, lsl #2]
	add	r8, ip, r3, lsl #2
	ldr	r4, [r6, #4]
	cmp	r4, r2
	bls	.L532
	ldr	r2, [ip]
	cbnz	r2, .L533
	cmp	r1, r0
	ittt	ne
	ldrne	r2, .L592+8
	addne	r0, r0, #1
	strhne	r0, [r2]	@ movhi
.L533:
	ldr	r2, .L592+20
	uxth	r10, r3
	ldr	r0, [r2]
	movs	r2, #0
.L534:
	uxth	lr, r2
	sxth	r1, r2
	cmp	r10, lr
	bhi	.L535
	ldr	r2, [r6, #4]
	cmp	r3, #0
	str	r2, [r8]
	strh	r5, [r0, r3, lsl #1]	@ movhi
	blt	.L524
	ldr	r2, .L592+8
	ldrh	r0, [r2]
	ldr	r2, .L592
	ldr	r2, [r2]
	subs	r2, r2, r0
	subs	r2, r2, #1
	sxth	r2, r2
	cmp	r3, r2
	bgt	.L524
.L569:
	ldr	r2, .L592+8
	adds	r0, r0, #1
	strh	r0, [r2]	@ movhi
	ldr	r2, [r6, #4]
	str	r2, [ip, r3, lsl #2]
	ldr	r2, .L592+20
.L589:
	ldr	r2, [r2]
	strh	r5, [r2, r3, lsl #1]	@ movhi
	b	.L524
.L535:
	add	lr, ip, r1, lsl #2
	adds	r2, r2, #1
	ldr	r4, [lr, #4]
	add	lr, r0, r1, lsl #1
	ldrh	lr, [lr, #2]
	str	r4, [ip, r1, lsl #2]
	strh	lr, [r0, r1, lsl #1]	@ movhi
	b	.L534
.L532:
	subs	r3, r3, #1
	sxth	r3, r3
	b	.L531
.L593:
	.align	2
.L592:
	.word	.LANCHOR29
	.word	.LANCHOR34
	.word	.LANCHOR129
	.word	.LANCHOR124
	.word	.LANCHOR26
	.word	.LANCHOR118
	.word	.LANCHOR121
	.word	.LANCHOR35
	.word	.LANCHOR138
	.word	.LANCHOR4
	.word	.LANCHOR5
	.word	.LANCHOR2
	.word	.LANCHOR64
	.word	.LANCHOR102
	.word	.LANCHOR23
	.word	.LANCHOR63
	.word	.LANCHOR12
	.word	.LANCHOR81
	.word	.LANCHOR139
	.word	.LC1
.L527:
	ldr	r8, .L594+12
	ldr	r10, .L594+20
	ldrh	r2, [r8]
	ldrh	r3, [r10]
	cmp	r2, r3
	bls	.L540
	movw	r2, #1273
	ldr	r1, .L594
	ldr	r0, .L594+4
	bl	printf
.L540:
	ldr	r2, .L594+8
	ldrh	lr, [r10]
	ldrh	ip, [r8]
	ldr	r0, [r2]
	add	r10, lr, #-1
	sxth	r3, r10
	sub	r10, r10, ip
.L541:
	cmp	r3, r10
	ble	.L546
	ldr	r1, [r6, #4]
	add	r8, r0, r3, lsl #2
	ldr	r2, [r0, r3, lsl #2]
	cmp	r1, r2
	bls	.L542
	ldr	r2, [r0]
	cbnz	r2, .L543
	cmp	lr, ip
	ittt	ne
	ldrne	r2, .L594+12
	addne	ip, ip, #1
	strhne	ip, [r2]	@ movhi
.L543:
	ldr	r2, .L594+16
	uxth	r10, r3
	ldr	ip, [r2]
	movs	r2, #0
.L544:
	uxth	lr, r2
	sxth	r1, r2
	cmp	r10, lr
	bhi	.L545
	ldr	r2, [r6, #4]
	str	r2, [r8]
	strh	r5, [ip, r3, lsl #1]	@ movhi
.L546:
	cmp	r3, #0
	blt	.L524
	ldr	r2, .L594+20
	ldr	ip, .L594+12
	ldrh	r2, [r2]
	ldrh	r1, [ip]
	subs	r2, r2, #1
	subs	r2, r2, r1
	sxth	r2, r2
	cmp	r3, r2
	bgt	.L524
	ldr	r2, [r6, #4]
	adds	r1, r1, #1
	strh	r1, [ip]	@ movhi
	str	r2, [r0, r3, lsl #2]
	ldr	r2, .L594+16
	b	.L589
.L545:
	add	lr, r0, r1, lsl #2
	adds	r2, r2, #1
	ldr	r4, [lr, #4]
	add	lr, ip, r1, lsl #1
	ldrh	lr, [lr, #2]
	str	r4, [r0, r1, lsl #2]
	strh	lr, [ip, r1, lsl #1]	@ movhi
	b	.L544
.L542:
	subs	r3, r3, #1
	sxth	r3, r3
	b	.L541
.L525:
	ldr	r8, .L594+40
	movw	r2, #65535
	ldrh	r1, [r8]
	cmp	r1, r2
	bne	.L548
.L590:
	strh	r5, [r8]	@ movhi
	str	r3, [r8, #8]
	b	.L524
.L548:
	ldrh	r0, [r8, #4]
	cmp	r0, r2
	beq	.L549
	movs	r1, #1
	bl	FtlFreeSysBlkQueueIn
.L549:
	ldr	r3, [r6, #4]
	ldr	r2, [r8, #8]
	cmp	r2, r3
	bcs	.L550
	ldrh	r2, [r8]
	strh	r2, [r8, #4]	@ movhi
	b	.L590
.L550:
	strh	r5, [r8, #4]	@ movhi
	b	.L524
.L553:
	ldr	r3, .L594+24
	ldr	r2, [r3]
	ldrh	r3, [r2]
	cbz	r3, .L555
.L558:
	ldr	r3, .L594+16
	ldr	r4, [r3]
	ldrh	r2, [r4]
	cmp	r2, #0
	beq	.L556
.L557:
	ldr	r3, .L594+28
	ldrh	r2, [r3]
	ldr	r3, .L594+32
	ldr	r3, [r3]
	cmp	r2, r3
	bls	.L588
	movw	r2, #1398
	ldr	r1, .L594
	ldr	r0, .L594+4
	bl	printf
.L588:
	movs	r0, #0
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L555:
	ldr	r1, .L594+28
	ldrh	r1, [r1]
	cmp	r1, #0
	beq	.L558
	ldr	r5, .L594+32
	ldr	r0, [r5]
.L559:
	sxth	r1, r3
	cmp	r1, r0
	bcs	.L558
	ldrh	r4, [r2, r1, lsl #1]
	adds	r3, r3, #1
	cmp	r4, #0
	beq	.L559
	ldr	r3, .L594+36
	movs	r6, #0
	ldr	r0, [r3]
	mov	r3, r1
.L560:
	ldr	r4, [r5]
	cmp	r3, r4
	bcs	.L558
	ldrh	r7, [r2, r3, lsl #1]
	subs	r4, r3, r1
	strh	r7, [r2, r4, lsl #1]	@ movhi
	ldr	r7, [r0, r3, lsl #2]
	str	r7, [r0, r4, lsl #2]
	strh	r6, [r2, r3, lsl #1]	@ movhi
	adds	r3, r3, #1
	sxth	r3, r3
	b	.L560
.L556:
	ldr	r3, .L594+12
	ldrh	r3, [r3]
	cmp	r3, #0
	beq	.L557
	ldr	r5, .L594+20
	ldrh	r1, [r5]
.L565:
	sxth	r3, r2
	cmp	r3, r1
	mov	r6, r3
	bge	.L557
	ldrh	r0, [r4, r3, lsl #1]
	adds	r2, r2, #1
	cmp	r0, #0
	beq	.L565
	ldr	r2, .L594+8
	movs	r0, #0
	ldr	r2, [r2]
.L566:
	ldrh	r1, [r5]
	cmp	r3, r1
	bge	.L557
	ldrh	r7, [r4, r3, lsl #1]
	subs	r1, r3, r6
	strh	r7, [r4, r1, lsl #1]	@ movhi
	ldr	r7, [r2, r3, lsl #2]
	str	r7, [r2, r1, lsl #2]
	adds	r1, r3, #1
	strh	r0, [r4, r3, lsl #1]	@ movhi
	sxth	r3, r1
	b	.L566
.L595:
	.align	2
.L594:
	.word	.LANCHOR139
	.word	.LC1
	.word	.LANCHOR121
	.word	.LANCHOR34
	.word	.LANCHOR35
	.word	.LANCHOR26
	.word	.LANCHOR118
	.word	.LANCHOR129
	.word	.LANCHOR29
	.word	.LANCHOR124
	.word	.LANCHOR138
	.size	FtlScanSysBlk, .-FtlScanSysBlk
	.section	.text.FtlLoadBbt,"ax",%progbits
	.align	1
	.global	FtlLoadBbt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlLoadBbt, %function
FtlLoadBbt:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r10, lr}
	ldr	r8, .L625+40
	ldr	r4, .L625
	ldr	r3, [r8]
	ldr	r7, .L625+4
	mov	r10, r4
	str	r3, [r4, #8]
	ldr	r3, .L625+8
	ldr	r6, [r3]
	str	r6, [r4, #12]
	bl	FtlBbtMemInit
	ldrh	r5, [r7]
	subs	r5, r5, #1
	uxth	r5, r5
.L597:
	ldrh	r3, [r7]
	subs	r3, r3, #15
	cmp	r3, r5
	bgt	.L600
	lsls	r3, r5, #10
	movs	r2, #1
	mov	r1, r2
	mov	r0, r10
	str	r3, [r4, #4]
	bl	FlashReadPages
	ldr	r3, [r4]
	adds	r3, r3, #1
	bne	.L598
	ldr	r3, [r4, #4]
	movs	r2, #1
	mov	r1, r2
	mov	r0, r10
	adds	r3, r3, #1
	str	r3, [r4, #4]
	bl	FlashReadPages
.L598:
	ldr	r3, [r4]
	adds	r3, r3, #1
	beq	.L599
	ldrh	r2, [r6]
	movw	r3, #61649
	cmp	r2, r3
	bne	.L599
	ldr	r3, .L625+12
	ldr	r2, [r6, #4]
	strh	r5, [r3]	@ movhi
	str	r2, [r3, #8]
	ldrh	r2, [r6, #8]
	strh	r2, [r3, #4]	@ movhi
.L600:
	ldr	r5, .L625+12
	movw	r2, #65535
	ldrh	r3, [r5]
	cmp	r3, r2
	beq	.L614
	ldrh	r3, [r5, #4]
	cmp	r3, r2
	beq	.L604
	lsls	r3, r3, #10
	movs	r2, #1
	mov	r1, r2
	ldr	r0, .L625
	str	r3, [r4, #4]
	bl	FlashReadPages
	ldr	r3, [r4]
	adds	r3, r3, #1
	beq	.L604
	ldrh	r2, [r6]
	movw	r3, #61649
	cmp	r2, r3
	bne	.L604
	ldr	r3, [r6, #4]
	ldr	r2, [r5, #8]
	cmp	r3, r2
	bls	.L604
	ldrh	r2, [r5, #4]
	str	r3, [r5, #8]
	ldrh	r3, [r6, #8]
	strh	r2, [r5]	@ movhi
	strh	r3, [r5, #4]	@ movhi
.L604:
	ldr	r10, .L625
	movs	r1, #1
	ldrh	r0, [r5]
	bl	FtlGetLastWrittenPage
	sxth	r7, r0
	adds	r0, r0, #1
	strh	r0, [r5, #2]	@ movhi
.L606:
	cmp	r7, #0
	bge	.L609
	movs	r2, #251
	ldr	r1, .L625+16
	ldr	r0, .L625+20
	bl	printf
.L608:
	ldrh	r3, [r6, #10]
	ldrh	r0, [r6, #12]
	strh	r3, [r5, #6]	@ movhi
	movw	r3, #65535
	cmp	r0, r3
	beq	.L611
	ldr	r3, .L625+24
	ldr	r2, [r3]
	cmp	r0, r2
	beq	.L611
	ldr	r3, .L625+28
	ldrh	r3, [r3]
	lsrs	r3, r3, #2
	cmp	r2, r3
	bcs	.L611
	cmp	r0, r3
	bcs	.L611
	bl	FtlSysBlkNumInit
.L611:
	ldr	r6, .L625+32
	movs	r5, #0
	ldr	r7, .L625+36
	ldr	r8, .L625+44
.L612:
	ldrh	r3, [r7]
	cmp	r5, r3
	bcc	.L613
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r10, pc}
.L599:
	subs	r5, r5, #1
	uxth	r5, r5
	b	.L597
.L609:
	ldrh	r3, [r5]
	movs	r2, #1
	mov	r1, r2
	mov	r0, r10
	orr	r3, r7, r3, lsl #10
	str	r3, [r4, #4]
	ldr	r3, [r8]
	str	r3, [r4, #8]
	bl	FlashReadPages
	ldr	r3, [r4]
	adds	r3, r3, #1
	beq	.L607
	ldrh	r2, [r6]
	movw	r3, #61649
	cmp	r2, r3
	beq	.L608
.L607:
	subs	r7, r7, #1
	sxth	r7, r7
	b	.L606
.L613:
	ldrh	r2, [r8]
	ldr	r1, [r4, #8]
	ldr	r0, [r6, #4]!
	lsls	r2, r2, #2
	mla	r1, r5, r2, r1
	adds	r5, r5, #1
	bl	ftl_memcpy
	b	.L612
.L614:
	mov	r0, #-1
	pop	{r3, r4, r5, r6, r7, r8, r10, pc}
.L626:
	.align	2
.L625:
	.word	.LANCHOR137
	.word	.LANCHOR16
	.word	.LANCHOR113
	.word	.LANCHOR36
	.word	.LANCHOR140
	.word	.LC1
	.word	.LANCHOR1
	.word	.LANCHOR5
	.word	.LANCHOR36+24
	.word	.LANCHOR9
	.word	.LANCHOR105
	.word	.LANCHOR126
	.size	FtlLoadBbt, .-FtlLoadBbt
	.section	.text.FlashProgPages,"ax",%progbits
	.align	1
	.global	FlashProgPages
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FlashProgPages, %function
FlashProgPages:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L667
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #56
	ldr	fp, .L667+52
	mov	r10, r1
	mov	r4, r0
	mov	r5, r0
	ldrh	r3, [r3, #12]
	mov	r8, #0
	str	r2, [sp, #20]
	str	r3, [sp, #12]
	lsls	r3, r3, #3
	str	r3, [sp, #16]
.L628:
	cmp	r8, r10
	bne	.L641
	ldr	fp, .L667+44
	movs	r5, #0
.L642:
	cmp	r8, r5
	beq	.L665
	ldr	r7, .L667+4
	movs	r1, #0
	ldr	r2, [fp]
	add	r0, sp, #36
	ldr	r3, [r7]
	str	r1, [r2]
	str	r1, [r3]
	ldr	r1, [r4, #4]
	str	r2, [sp, #44]
	ldr	r2, [sp, #20]
	str	r1, [sp, #40]
	movs	r1, #1
	str	r3, [sp, #48]
	bl	FlashReadPages
	ldr	r10, [sp, #36]
	cmp	r10, #-1
	bne	.L643
	ldr	r1, [r4, #4]
	ldr	r0, .L667+8
	bl	printf
	str	r10, [r4]
.L643:
	ldr	r10, [sp, #36]
	cmp	r10, #256
	bne	.L644
	ldr	r1, [r4, #4]
	ldr	r0, .L667+12
	bl	printf
	str	r10, [r4]
.L644:
	ldr	r3, [r4, #12]
	cbz	r3, .L645
	ldr	r1, [r7]
	ldr	r2, [r3]
	ldr	r0, [r1]
	cmp	r2, r0
	bne	.L646
	ldr	r7, [r3, #8]
	ldr	r6, [r1, #8]
	cmp	r7, r6
	beq	.L645
.L646:
	ldr	r1, [r1, #4]
	stm	sp, {r0, r1}
	ldr	r0, .L667+16
	ldr	r3, [r3, #4]
	ldr	r1, [r4, #4]
	bl	printf
	mov	r3, #-1
	str	r3, [r4]
.L645:
	ldr	r3, [r4, #8]
	cbz	r3, .L647
	ldr	r1, [fp]
	ldr	r2, [r3]
	ldr	r0, [r1]
	cmp	r2, r0
	bne	.L648
	ldr	r7, [r3, #2048]
	ldr	r6, [r1, #2048]
	cmp	r7, r6
	beq	.L647
.L648:
	ldr	r1, [r1, #4]
	stm	sp, {r0, r1}
	ldr	r0, .L667+20
	ldr	r3, [r3, #4]
	ldr	r1, [r4, #4]
	bl	printf
	mov	r3, #-1
	str	r3, [r4]
.L647:
	adds	r5, r5, #1
	adds	r4, r4, #20
	b	.L642
.L641:
	ldr	r3, [r5, #8]
	cbz	r3, .L629
	ldr	r3, [r5, #12]
	cbnz	r3, .L630
.L629:
	movs	r2, #142
	ldr	r1, .L667+24
	ldr	r0, .L667+28
	bl	printf
.L630:
	add	r2, sp, #28
	add	r1, sp, #32
	ldr	r0, [r5, #4]
	bl	l2p_addr_tran.isra.0
	ldr	r6, [sp, #28]
	cmp	r6, #3
	bls	.L631
.L666:
	mov	r3, #-1
	str	r3, [r5]
	b	.L632
.L631:
	cbnz	r6, .L633
	ldr	r3, [sp, #32]
	ldr	r2, [sp, #16]
	cmp	r2, r3
	bls	.L633
	ldr	r5, .L667+24
	ldr	r7, .L667+32
	b	.L664
.L635:
	mov	r3, #-1
	ldr	r2, [r4, #-16]
	str	r3, [r4, #-20]
	mov	r1, r5
	mov	r0, r7
	adds	r6, r6, #1
	bl	printf
	movs	r3, #16
	movs	r2, #4
	ldr	r1, [r4, #-12]
	ldr	r0, .L667+36
	bl	rknand_print_hex
	movs	r3, #4
	ldr	r1, [r4, #-8]
	mov	r2, r3
	ldr	r0, .L667+40
	bl	rknand_print_hex
.L664:
	cmp	r6, r10
	add	r4, r4, #20
	bne	.L635
.L665:
	movs	r0, #0
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L633:
	ldr	r1, [r5, #8]
	lsls	r3, r1, #26
	beq	.L651
	ldr	r3, .L667+44
	ldr	r6, [r3]
	cmp	r1, r6
	beq	.L636
	ldr	r3, .L667+48
	mov	r0, r6
	ldrh	r2, [r3]
	lsls	r2, r2, #9
	bl	ftl_memcpy
.L636:
	ldr	r3, [r5, #12]
	mov	r2, r6
	ldr	r1, [sp, #32]
	ldrb	r0, [sp, #28]	@ zero_extendqisi2
	ldr	r7, [fp, #8]
	blx	r7
	cbnz	r0, .L637
	str	r0, [r5]
.L638:
	ldr	r3, .L667
	ldrh	r3, [r3, #14]
	cmp	r3, #4
	bne	.L632
	ldr	r0, [sp, #12]
	add	r2, r6, #2048
	ldr	r3, [r5, #12]
	ldr	r1, [sp, #32]
	ldr	r6, [fp, #8]
	adds	r3, r3, #8
	add	r1, r1, r0
	ldrb	r0, [sp, #28]	@ zero_extendqisi2
	blx	r6
	cmp	r0, #0
	bne	.L666
.L632:
	add	r8, r8, #1
	adds	r5, r5, #20
	b	.L628
.L651:
	mov	r6, r1
	b	.L636
.L637:
	mov	r3, #-1
	str	r3, [r5]
	b	.L638
.L668:
	.align	2
.L667:
	.word	.LANCHOR0
	.word	.LANCHOR112
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LANCHOR141
	.word	.LC1
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LANCHOR110
	.word	.LANCHOR11
	.word	.LANCHOR77
	.size	FlashProgPages, .-FlashProgPages
	.section	.text.FtlLowFormatEraseBlock,"ax",%progbits
	.align	1
	.global	FtlLowFormatEraseBlock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlLowFormatEraseBlock, %function
FtlLowFormatEraseBlock:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L712
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #32
	ldr	r7, .L712+4
	mov	r8, #0
	mov	r6, r1
	mov	r5, r8
	str	r0, [r3]
	mov	r4, r8
	ldr	r3, .L712+8
	mov	fp, #20
	ldr	r10, [r7]
	str	r0, [sp, #4]
	ldrh	r3, [r3]
	str	r3, [sp, #12]
	ldr	r3, .L712+12
	ldr	r3, [r3]
	str	r3, [sp, #16]
	ldr	r3, .L712+16
	ldr	r3, [r3]
	str	r3, [sp, #20]
	ldr	r3, .L712+20
	ldrh	r3, [r3]
	str	r3, [sp, #24]
.L670:
	ldr	r3, [sp, #12]
	uxth	r2, r8
	cmp	r3, r2
	bhi	.L674
	cmp	r5, #0
	beq	.L669
	mov	r0, r10
	mov	r8, #0
	mov	r10, #20
	mov	r2, r5
	movs	r1, #0
	bl	FlashEraseBlocks
.L677:
	uxth	r3, r8
	cmp	r5, r3
	bhi	.L679
	cmp	r6, #0
	beq	.L695
	ldr	r3, .L712+24
	mov	r10, #1
	ldrh	r3, [r3]
	str	r3, [sp, #8]
.L680:
	movs	r7, #0
.L689:
	ldr	r3, .L712+8
	mov	r8, #0
	mov	r5, r8
	ldrh	r3, [r3]
	str	r3, [sp, #16]
	ldr	r3, .L712+4
	ldr	fp, [r3]
	ldr	r3, .L712+28
	ldr	r3, [r3]
	str	r3, [sp, #20]
	ldr	r3, .L712+12
	ldr	r3, [r3]
	str	r3, [sp, #24]
	ldr	r3, .L712+20
	ldrh	r3, [r3]
	str	r3, [sp, #28]
.L681:
	ldr	r3, [sp, #16]
	uxth	r2, r8
	cmp	r3, r2
	bhi	.L684
	cbz	r5, .L669
	mov	r0, fp
	ldr	fp, .L712+4
	movs	r3, #1
	mov	r2, r10
	mov	r1, r5
	mov	r8, #0
	bl	FlashProgPages
	movs	r3, #20
.L686:
	uxth	r2, r8
	cmp	r5, r2
	bhi	.L688
	adds	r7, r7, #1
	ldr	r2, [sp, #8]
	uxth	r3, r7
	cmp	r2, r3
	bhi	.L689
	ldr	r8, .L712+4
	movs	r7, #0
	mov	fp, #20
.L690:
	uxth	r3, r7
	cmp	r5, r3
	bhi	.L692
	ldr	r3, [sp, #4]
	cmp	r3, #63
	bls	.L693
	cbz	r6, .L669
.L693:
	ldr	r3, .L712+4
	mov	r2, r5
	mov	r1, r10
	ldr	r0, [r3]
	bl	FlashEraseBlocks
.L669:
	mov	r0, r4
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L674:
	mul	r2, fp, r8
	movs	r3, #0
	ldr	r1, [sp, #4]
	str	r3, [r10, r2]
	ldr	r3, .L712+32
	ldrb	r0, [r3, r8]	@ zero_extendqisi2
	bl	V2P_block
	str	r0, [sp, #8]
	cbz	r6, .L671
	bl	IsBlkInVendorPart
	cbnz	r0, .L672
.L671:
	ldr	r0, [sp, #8]
	bl	FtlBbmIsBadBlock
	cbnz	r0, .L673
	ldr	r3, [sp, #8]
	mla	r1, fp, r5, r10
	lsls	r2, r3, #10
	ldr	r3, [sp, #16]
	str	r2, [r1, #4]
	str	r3, [r1, #8]
	ldr	r3, [sp, #24]
	mul	r2, r3, r5
	ldr	r3, [sp, #20]
	adds	r5, r5, #1
	uxth	r5, r5
	bic	r2, r2, #3
	add	r2, r2, r3
	str	r2, [r1, #12]
.L672:
	add	r8, r8, #1
	b	.L670
.L673:
	adds	r4, r4, #1
	uxth	r4, r4
	b	.L672
.L679:
	mul	r3, r10, r8
	ldr	r2, [r7]
	adds	r1, r2, r3
	ldr	r3, [r2, r3]
	adds	r3, r3, #1
	bne	.L678
	ldr	r0, [r1, #4]
	adds	r4, r4, #1
	uxth	r4, r4
	ubfx	r0, r0, #10, #16
	bl	FtlBbmMapBadBlock
.L678:
	add	r8, r8, #1
	b	.L677
.L695:
	movs	r3, #2
	mov	r10, r6
	str	r3, [sp, #8]
	b	.L680
.L684:
	movs	r3, #20
	ldr	r1, [sp, #4]
	mul	r2, r3, r8
	movs	r3, #0
	str	r3, [fp, r2]
	ldr	r3, .L712+32
	ldrb	r0, [r3, r8]	@ zero_extendqisi2
	bl	V2P_block
	str	r0, [sp, #12]
	cbz	r6, .L682
	bl	IsBlkInVendorPart
	cbnz	r0, .L683
.L682:
	ldr	r0, [sp, #12]
	bl	FtlBbmIsBadBlock
	cbnz	r0, .L683
	movs	r3, #20
	mla	r1, r3, r5, fp
	ldr	r3, [sp, #12]
	add	r2, r7, r3, lsl #10
	ldr	r3, [sp, #20]
	str	r2, [r1, #4]
	str	r3, [r1, #8]
	ldr	r3, [sp, #28]
	mul	r2, r3, r5
	ldr	r3, [sp, #24]
	adds	r5, r5, #1
	uxth	r5, r5
	bic	r2, r2, #3
	add	r2, r2, r3
	str	r2, [r1, #12]
.L683:
	add	r8, r8, #1
	b	.L681
.L688:
	mul	r2, r3, r8
	ldr	r1, [fp]
	adds	r0, r1, r2
	ldr	r2, [r1, r2]
	cbz	r2, .L687
	ldr	r0, [r0, #4]
	adds	r4, r4, #1
	str	r3, [sp, #12]
	uxth	r4, r4
	ubfx	r0, r0, #10, #16
	bl	FtlBbmMapBadBlock
	ldr	r3, [sp, #12]
.L687:
	add	r8, r8, #1
	b	.L686
.L692:
	cbz	r6, .L691
	mul	r3, fp, r7
	ldr	r2, [r8]
	adds	r1, r2, r3
	ldr	r3, [r2, r3]
	cbnz	r3, .L691
	ldr	r0, [r1, #4]
	movs	r1, #1
	ubfx	r0, r0, #10, #16
	bl	FtlFreeSysBlkQueueIn
.L691:
	adds	r7, r7, #1
	b	.L690
.L713:
	.align	2
.L712:
	.word	.LANCHOR98
	.word	.LANCHOR78
	.word	.LANCHOR2
	.word	.LANCHOR109
	.word	.LANCHOR114
	.word	.LANCHOR23
	.word	.LANCHOR19
	.word	.LANCHOR108
	.word	.LANCHOR12
	.size	FtlLowFormatEraseBlock, .-FtlLowFormatEraseBlock
	.section	.text.FlashTestBlk,"ax",%progbits
	.align	1
	.global	FlashTestBlk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FlashTestBlk, %function
FlashTestBlk:
	@ args = 0, pretend = 0, frame = 88
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #11
	push	{r4, r5, lr}
	mov	r5, r0
	sub	sp, sp, #92
	bls	.L717
	ldr	r4, .L719
	add	r0, sp, #24
	movs	r2, #32
	movs	r1, #165
	str	r0, [sp, #16]
	lsls	r5, r5, #10
	ldr	r3, [r4]
	str	r3, [sp, #12]
	bl	ftl_memset
	movs	r2, #8
	movs	r1, #90
	ldr	r0, [r4]
	bl	ftl_memset
	movs	r2, #1
	add	r0, sp, #4
	mov	r1, r2
	str	r5, [sp, #8]
	bl	FlashEraseBlocks
	movs	r3, #1
	add	r0, sp, #4
	mov	r2, r3
	mov	r1, r3
	bl	FlashProgPages
	ldr	r3, [sp, #4]
	cbnz	r3, .L718
	adds	r3, r5, #1
	add	r0, sp, #4
	str	r3, [sp, #8]
	movs	r3, #1
	mov	r2, r3
	mov	r1, r3
	bl	FlashProgPages
	ldr	r4, [sp, #4]
	adds	r4, r4, #0
	it	ne
	movne	r4, #1
	negs	r4, r4
.L716:
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	str	r5, [sp, #8]
	bl	FlashEraseBlocks
.L714:
	mov	r0, r4
	add	sp, sp, #92
	@ sp needed
	pop	{r4, r5, pc}
.L718:
	mov	r4, #-1
	b	.L716
.L717:
	movs	r4, #0
	b	.L714
.L720:
	.align	2
.L719:
	.word	.LANCHOR111
	.size	FlashTestBlk, .-FlashTestBlk
	.section	.text.FtlBbmTblFlush,"ax",%progbits
	.align	1
	.global	FtlBbmTblFlush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlBbmTblFlush, %function
FtlBbmTblFlush:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r1, #0
	ldr	r7, .L735
	movs	r4, #0
	ldr	r5, .L735+4
	ldr	r3, [r7]
	ldr	fp, .L735+52
	ldr	r6, .L735+8
	str	r3, [r5, #12]
	ldr	r3, .L735+12
	ldr	r0, [fp]
	ldr	r8, .L735+56
	ldrh	r2, [r3]
	str	r0, [r5, #8]
	bl	ftl_memset
	str	r7, [sp, #12]
	ldr	r7, .L735+16
.L722:
	ldrh	r3, [r7]
	cmp	r4, r3
	blt	.L723
	ldr	r6, [r5, #12]
	movs	r2, #16
	ldr	r4, .L735+20
	movs	r1, #255
	movs	r7, #0
	mov	r0, r6
	mov	r8, r7
	bl	ftl_memset
	movw	r3, #61649
	mov	r10, r4
	strh	r3, [r6]	@ movhi
	ldr	r3, [r4, #8]
	str	r3, [r6, #4]
	ldrh	r3, [r4]
	strh	r3, [r6, #2]	@ movhi
	ldrh	r3, [r4, #4]
	strh	r3, [r6, #8]	@ movhi
	ldrh	r3, [r4, #6]
	strh	r3, [r6, #10]	@ movhi
	ldr	r3, .L735+24
	ldr	r3, [r3]
	strh	r3, [r6, #12]	@ movhi
	ldr	r3, .L735+28
	ldr	r3, [r3]
	strh	r3, [r6, #14]	@ movhi
.L724:
	ldr	r3, [fp]
	ldrh	r2, [r4, #2]
	ldrh	r1, [r4]
	str	r3, [r5, #8]
	ldr	r3, [sp, #12]
	ldrh	r0, [r6, #10]
	ldr	r3, [r3]
	str	r3, [r5, #12]
	movs	r3, #0
	str	r3, [r5]
	orr	r3, r2, r1, lsl #10
	str	r3, [r5, #4]
	ldrh	r3, [r4, #4]
	str	r0, [sp]
	ldr	r0, .L735+32
	bl	printf
	ldr	r3, .L735+36
	ldrh	r2, [r4, #2]
	ldrh	r3, [r3]
	subs	r3, r3, #1
	cmp	r2, r3
	blt	.L725
	ldr	r3, [r4, #8]
	mov	r1, #0	@ movhi
	ldrh	r2, [r4]
	strh	r1, [r4, #2]	@ movhi
	adds	r3, r3, #1
	str	r3, [r4, #8]
	str	r3, [r6, #4]
	strh	r2, [r6, #8]	@ movhi
	ldrh	r3, [r4, #4]
	strh	r2, [r4, #4]	@ movhi
	ldr	r2, .L735+40
	strh	r3, [r4]	@ movhi
	lsls	r3, r3, #10
	ldr	r0, [r2]
	movs	r2, #1
	str	r3, [r5, #4]
	mov	r1, r2
	str	r3, [r0, #4]
	bl	FlashEraseBlocks
.L725:
	movs	r3, #1
	ldr	r0, .L735+4
	mov	r2, r3
	mov	r1, r3
	bl	FlashProgPages
	ldrh	r3, [r10, #2]
	adds	r3, r3, #1
	strh	r3, [r10, #2]	@ movhi
	ldr	r3, [r5]
	adds	r2, r3, #1
	bne	.L726
	adds	r7, r7, #1
	ldr	r1, [r5, #4]
	uxth	r7, r7
	ldr	r0, .L735+44
	bl	printf
	cmp	r7, #3
	bls	.L724
	mov	r2, r7
	ldr	r1, [r5, #4]
	ldr	r0, .L735+48
	bl	printf
.L728:
	b	.L728
.L723:
	ldrh	r2, [r8]
	ldr	r3, [r5, #8]
	ldr	r1, [r6, #4]!
	mul	r0, r2, r4
	lsls	r2, r2, #2
	adds	r4, r4, #1
	add	r0, r3, r0, lsl #2
	bl	ftl_memcpy
	b	.L722
.L726:
	add	r8, r8, #1
	cmp	r8, #1
	beq	.L724
	cmp	r3, #256
	beq	.L724
	movs	r0, #0
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L736:
	.align	2
.L735:
	.word	.LANCHOR113
	.word	.LANCHOR137
	.word	.LANCHOR36+24
	.word	.LANCHOR22
	.word	.LANCHOR9
	.word	.LANCHOR36
	.word	.LANCHOR1
	.word	.LANCHOR142
	.word	.LC18
	.word	.LANCHOR19
	.word	.LANCHOR78
	.word	.LC19
	.word	.LC20
	.word	.LANCHOR105
	.word	.LANCHOR126
	.size	FtlBbmTblFlush, .-FtlBbmTblFlush
	.section	.text.allocate_data_superblock,"ax",%progbits
	.align	1
	.global	allocate_data_superblock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	allocate_data_superblock, %function
allocate_data_superblock:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r10, .L776
.L738:
	ldr	r3, .L776
	ldr	r8, .L776+80
	ldr	r7, .L776+4
	ldrh	r3, [r3]
	ldrh	r2, [r8]
	add	r3, r3, r2
	ldrh	r2, [r7]
	cmp	r3, r2
	ble	.L739
	movw	r2, #2667
	ldr	r1, .L776+8
	ldr	r0, .L776+12
	bl	printf
.L739:
	ldr	r3, .L776+16
	cmp	r4, r3
	bne	.L765
	ldr	r2, .L776+20
	ldrh	r3, [r10]
	ldr	r2, [r2]
	lsrs	r1, r3, #1
	muls	r2, r3, r2
	adds	r1, r1, #1
	add	r1, r1, r2, lsr #2
	uxth	r1, r1
	cbz	r1, .L740
	subs	r1, r1, #1
	uxth	r1, r1
.L740:
	ldr	r0, .L776+24
	bl	List_pop_index_node
	ldrh	r3, [r10]
	mov	r5, r0
	uxth	fp, r0
	cbnz	r3, .L741
	movw	r2, #2676
	ldr	r1, .L776+8
	ldr	r0, .L776+12
	bl	printf
.L741:
	ldrh	r3, [r10]
	subs	r3, r3, #1
	strh	r3, [r10]	@ movhi
	ldrh	r3, [r7]
	cmp	r3, fp
	bls	.L738
	uxth	r5, r5
	lsls	r3, r5, #1
	str	r3, [sp]
	ldr	r3, .L776+28
	ldr	r3, [r3]
	ldrh	r6, [r3, r5, lsl #1]
	cmp	r6, #0
	bne	.L738
	strh	fp, [r4]	@ movhi
	mov	r0, r4
	bl	make_superblock
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cbnz	r3, .L743
	ldr	r3, .L776+28
	movw	r2, #65535
	mov	r0, fp
	ldr	r3, [r3]
	strh	r2, [r3, r5, lsl #1]	@ movhi
	bl	INSERT_DATA_LIST
	ldrh	r2, [r8]
	ldrh	r3, [r10]
	add	r3, r3, r2
	ldrh	r2, [r7]
	cmp	r3, r2
	ble	.L738
	movw	r2, #2690
	ldr	r1, .L776+8
	ldr	r0, .L776+12
	bl	printf
	b	.L738
.L765:
	movs	r1, #0
	b	.L740
.L743:
	ldrh	r2, [r8]
	ldrh	r3, [r10]
	add	r3, r3, r2
	ldrh	r2, [r7]
	cmp	r3, r2
	ble	.L745
	movw	r2, #2693
	ldr	r1, .L776+8
	ldr	r0, .L776+12
	bl	printf
.L745:
	ldr	r3, .L776+32
	add	lr, r4, #16
	ldr	r2, .L776+36
	mov	r8, #0
	ldr	ip, [r3]
	ldrh	r0, [r2]
	movs	r2, #20
	str	r2, [sp, #4]
	mov	r3, ip
	mla	r0, r2, r0, ip
.L746:
	cmp	r0, r3
	bne	.L748
	cbnz	r6, .L749
	mov	r2, #2704
	ldr	r1, .L776+8
	ldr	r0, .L776+12
	bl	printf
.L749:
	ldr	r3, .L776+40
	ldrh	r3, [r3]
	cmp	r3, fp
	bne	.L750
	movw	r2, #2706
	ldr	r1, .L776+8
	ldr	r0, .L776+12
	bl	printf
.L750:
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	ldr	r0, .L776+44
	ldr	ip, .L776+84
	ldr	r1, .L776+48
	ldr	r2, .L776+52
	ldr	lr, [r0]
	cmp	r3, #0
	bne	.L751
	ldrh	r3, [lr, r5, lsl #1]
	cmp	r3, #0
	beq	.L752
	ldrh	r8, [ip]
	add	r3, r3, r8
.L775:
	strh	r3, [lr, r5, lsl #1]	@ movhi
	ldr	r3, [r1]
	adds	r3, r3, #1
	str	r3, [r1]
.L754:
	ldr	r3, [r0]
	ldr	r0, .L776+56
	ldrh	lr, [r3, r5, lsl #1]
	ldr	r3, [r0]
	cmp	lr, r3
	ldr	r3, [r2]
	it	hi
	strhi	lr, [r0]
	ldrh	r0, [ip]
	ldr	ip, [r1]
	ldrh	r1, [r7]
	ldr	r7, .L776+60
	mla	r0, ip, r0, r3
	bl	__aeabi_uidiv
	ldr	r3, .L776+64
	str	r0, [r3]
	ldr	r3, .L776+68
	ldr	r2, [r3]
	ldr	r3, [r2, #16]
	adds	r3, r3, #1
	str	r3, [r2, #16]
	movs	r2, #20
	ldr	r3, .L776+32
	ldr	r0, [r3]
	adds	r3, r0, #4
	mla	r2, r2, r6, r0
	adds	r2, r2, #24
.L756:
	adds	r3, r3, #20
	cmp	r2, r3
	bne	.L757
	movs	r7, #0
	mov	r2, r6
	mov	r8, r7
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	bl	FlashEraseBlocks
.L758:
	uxth	r3, r7
	cmp	r6, r3
	bhi	.L760
	cmp	r8, #0
	beq	.L761
	mov	r0, fp
	bl	update_multiplier_value
	bl	FtlBbmTblFlush
.L761:
	ldrb	r2, [r4, #7]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L762
	ldr	r3, .L776+28
	movw	r2, #65535
	ldr	r3, [r3]
	strh	r2, [r3, r5, lsl #1]	@ movhi
	b	.L738
.L748:
	ldrh	r1, [lr], #2
	movw	r2, #65535
	str	r8, [r3, #8]
	str	r8, [r3, #12]
	cmp	r1, r2
	beq	.L747
	ldr	r2, [sp, #4]
	lsls	r1, r1, #10
	mla	r2, r2, r6, ip
	adds	r6, r6, #1
	uxth	r6, r6
	str	r1, [r2, #4]
.L747:
	adds	r3, r3, #20
	b	.L746
.L752:
	movs	r3, #2
	b	.L775
.L751:
	ldrh	r3, [lr, r5, lsl #1]
	adds	r3, r3, #1
	strh	r3, [lr, r5, lsl #1]	@ movhi
	ldr	r3, [r2]
	adds	r3, r3, #1
	str	r3, [r2]
	b	.L754
.L757:
	ldr	r1, [r3, #-20]
	ands	r1, r1, r7
	str	r1, [r3, #-20]
	b	.L756
.L760:
	ldr	r2, .L776+32
	movs	r3, #20
	muls	r3, r7, r3
	ldr	r2, [r2]
	adds	r1, r2, r3
	ldr	r2, [r2, r3]
	adds	r3, r2, #1
	bne	.L759
	ldr	r0, [r1, #4]
	add	r8, r8, #1
	str	r2, [sp, #4]
	ubfx	r0, r0, #10, #16
	bl	FtlBbmMapBadBlock
	ldr	r2, [sp, #4]
	add	r3, r4, r7, lsl #1
	strh	r2, [r3, #16]	@ movhi
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	subs	r3, r3, #1
	strb	r3, [r4, #7]
.L759:
	adds	r7, r7, #1
	b	.L758
.L762:
	ldr	r3, .L776+72
	ldr	r1, .L776+76
	ldrh	r3, [r3]
	strh	fp, [r4]	@ movhi
	smulbb	r3, r3, r2
	movs	r2, #0
	strh	r2, [r4, #2]	@ movhi
	strb	r2, [r4, #6]
	ldr	r2, [r1]
	uxth	r3, r3
	strh	r3, [r4, #4]	@ movhi
	str	r2, [r4, #12]
	adds	r2, r2, #1
	str	r2, [r1]
	ldr	r2, .L776+28
	ldr	r1, [sp]
	ldr	r2, [r2]
	strh	r3, [r2, r1]	@ movhi
	ldrh	r3, [r4, #4]
	cbz	r3, .L763
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cbnz	r3, .L764
.L763:
	movw	r2, #2759
	ldr	r1, .L776+8
	ldr	r0, .L776+12
	bl	printf
.L764:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L777:
	.align	2
.L776:
	.word	.LANCHOR47
	.word	.LANCHOR4
	.word	.LANCHOR143
	.word	.LC1
	.word	.LANCHOR52
	.word	.LANCHOR94
	.word	.LANCHOR45
	.word	.LANCHOR41
	.word	.LANCHOR78
	.word	.LANCHOR2
	.word	.LANCHOR144
	.word	.LANCHOR46
	.word	.LANCHOR89
	.word	.LANCHOR90
	.word	.LANCHOR91
	.word	-1024
	.word	.LANCHOR142
	.word	.LANCHOR117
	.word	.LANCHOR18
	.word	.LANCHOR81
	.word	.LANCHOR43
	.word	.LANCHOR13
	.size	allocate_data_superblock, .-allocate_data_superblock
	.section	.text.FtlGcFreeBadSuperBlk,"ax",%progbits
	.align	1
	.global	FtlGcFreeBadSuperBlk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcFreeBadSuperBlk, %function
FtlGcFreeBadSuperBlk:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r10, r0
	ldr	r4, .L790
	ldrh	r3, [r4]
	cbz	r3, .L779
	ldr	r8, .L790+12
	movs	r7, #0
.L780:
	ldr	r3, .L790+4
	ldrh	r2, [r3]
	uxth	r3, r7
	cmp	r2, r3
	bhi	.L786
	bl	FtlGcReFreshBadBlk
.L779:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L786:
	ldr	r2, .L790+8
	uxth	r3, r7
	mov	r1, r10
	mov	fp, #0
	ldrb	r0, [r2, r3]	@ zero_extendqisi2
	bl	V2P_block
	mov	r1, r0
.L781:
	ldrh	r3, [r4]
	uxth	r5, fp
	cmp	r3, r5
	bhi	.L785
	adds	r7, r7, #1
	b	.L780
.L785:
	uxth	r6, fp
	ldrh	r3, [r8, r6, lsl #1]
	cmp	r3, r1
	bne	.L782
	mov	r0, r1
	str	r1, [sp, #4]
	bl	FtlBbmMapBadBlock
	bl	FtlBbmTblFlush
	ldrh	r2, [r4]
	add	r3, r8, r6, lsl #1
	ldr	r1, [sp, #4]
.L783:
	cmp	r5, r2
	bcc	.L784
	subs	r2, r2, #1
	strh	r2, [r4]	@ movhi
.L782:
	add	fp, fp, #1
	b	.L781
.L784:
	ldrh	r0, [r3, #2]!
	adds	r5, r5, #1
	uxth	r5, r5
	strh	r0, [r3, #-2]	@ movhi
	b	.L783
.L791:
	.align	2
.L790:
	.word	.LANCHOR73
	.word	.LANCHOR2
	.word	.LANCHOR12
	.word	.LANCHOR74
	.size	FtlGcFreeBadSuperBlk, .-FtlGcFreeBadSuperBlk
	.section	.text.update_vpc_list,"ax",%progbits
	.align	1
	.global	update_vpc_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	update_vpc_list, %function
update_vpc_list:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	mov	r4, r0
	ldr	r3, .L804
	ldr	r3, [r3]
	ldrh	r3, [r3, r0, lsl #1]
	cmp	r3, #0
	bne	.L793
	ldr	r3, .L804+4
	ldrh	r2, [r3]
	cmp	r2, r0
	itt	eq
	movweq	r2, #65535
	strheq	r2, [r3]	@ movhi
	ldr	r3, .L804+8
	ldrh	r2, [r3]
	cmp	r2, r0
	itt	eq
	movweq	r2, #65535
	strheq	r2, [r3]	@ movhi
	ldr	r3, .L804+12
	ldrh	r2, [r3]
	cmp	r2, r0
	bne	.L796
	movw	r2, #65535
	strh	r2, [r3]	@ movhi
.L797:
	ldr	r5, .L804+16
	mov	r1, r4
	ldr	r0, .L804+20
	bl	List_remove_node
	ldrh	r3, [r5]
	cbnz	r3, .L799
	movw	r2, #2835
	ldr	r1, .L804+24
	ldr	r0, .L804+28
	bl	printf
.L799:
	ldrh	r3, [r5]
	mov	r0, r4
	subs	r3, r3, #1
	strh	r3, [r5]	@ movhi
	bl	free_data_superblock
	mov	r0, r4
	bl	FtlGcFreeBadSuperBlk
	ldr	r3, .L804+32
	ldrh	r2, [r5]
	ldrh	r3, [r3]
	add	r3, r3, r2
	ldr	r2, .L804+36
	ldrh	r2, [r2]
	cmp	r3, r2
	ble	.L803
	movw	r2, #2838
	ldr	r1, .L804+24
	ldr	r0, .L804+28
	bl	printf
.L803:
	movs	r0, #1
	pop	{r3, r4, r5, pc}
.L796:
	ldr	r3, .L804+40
	ldrh	r3, [r3]
	cmp	r3, r0
	beq	.L802
	ldr	r3, .L804+44
	ldrh	r3, [r3]
	cmp	r3, r0
	beq	.L802
	ldr	r3, .L804+48
	ldrh	r3, [r3]
	cmp	r3, r0
	bne	.L797
.L802:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L793:
	bl	List_update_data_list
	b	.L802
.L805:
	.align	2
.L804:
	.word	.LANCHOR41
	.word	.LANCHOR71
	.word	.LANCHOR72
	.word	.LANCHOR144
	.word	.LANCHOR43
	.word	.LANCHOR40
	.word	.LANCHOR145
	.word	.LC1
	.word	.LANCHOR47
	.word	.LANCHOR4
	.word	.LANCHOR50
	.word	.LANCHOR51
	.word	.LANCHOR52
	.size	update_vpc_list, .-update_vpc_list
	.section	.text.decrement_vpc_count,"ax",%progbits
	.align	1
	.global	decrement_vpc_count
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	decrement_vpc_count, %function
decrement_vpc_count:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movw	r3, #65535
	push	{r4, r5, r6, lr}
	cmp	r0, r3
	mov	r4, r0
	beq	.L807
	ldr	r5, .L815
	ldr	r3, [r5]
	ldrh	r2, [r3, r0, lsl #1]
	cbnz	r2, .L808
	mov	r1, r0
	ldr	r0, .L815+4
	bl	printf
	ldr	r3, [r5]
	ldrh	r5, [r3, r4, lsl #1]
	cbz	r5, .L809
.L813:
	movs	r5, #0
.L806:
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L809:
	movw	r2, #2853
.L814:
	ldr	r1, .L815+8
	ldr	r0, .L815+12
	bl	printf
	b	.L806
.L808:
	subs	r2, r2, #1
	strh	r2, [r3, r0, lsl #1]	@ movhi
.L807:
	ldr	r6, .L815+16
	movw	r3, #65535
	ldrh	r0, [r6]
	cmp	r0, r3
	bne	.L811
	strh	r4, [r6]	@ movhi
	b	.L813
.L811:
	cmp	r4, r0
	beq	.L813
	bl	update_vpc_list
	ldr	r3, .L815+20
	adds	r5, r0, #0
	ldr	r2, .L815+24
	it	ne
	movne	r5, #1
	strh	r4, [r6]	@ movhi
	ldr	r3, [r3]
	ldr	r2, [r2]
	subs	r3, r3, r2
	asrs	r2, r3, #1
	ldr	r3, .L815+28
	muls	r3, r2, r3
	ldr	r2, .L815
	ldr	r2, [r2]
	uxth	r1, r3
	ldrh	r2, [r2, r1, lsl #1]
	cmp	r2, #0
	bne	.L806
	cmp	r4, r1
	beq	.L806
	movw	r2, #2869
	b	.L814
.L816:
	.align	2
.L815:
	.word	.LANCHOR41
	.word	.LC21
	.word	.LANCHOR146
	.word	.LC1
	.word	.LANCHOR130
	.word	.LANCHOR40
	.word	.LANCHOR39
	.word	-1431655765
	.size	decrement_vpc_count, .-decrement_vpc_count
	.section	.text.FtlSuperblockPowerLostFix,"ax",%progbits
	.align	1
	.global	FtlSuperblockPowerLostFix
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlSuperblockPowerLostFix, %function
FtlSuperblockPowerLostFix:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	mov	r3, #-1
	sub	sp, sp, #24
	ldr	r2, .L830
	movs	r6, #0
	mov	r4, r0
	str	r3, [sp, #20]
	ldr	r3, .L830+4
	ldr	r5, [r2]
	mvn	r2, #2
	ldr	r8, .L830+16
	ldr	r3, [r3]
	str	r5, [sp, #16]
	str	r3, [sp, #12]
	str	r2, [r5, #8]
	mvn	r2, #1
	str	r2, [r5, #12]
	ldrh	r2, [r0]
	strh	r6, [r5]	@ movhi
	strh	r2, [r5, #2]	@ movhi
	movw	r2, #61589
	str	r2, [r3]
	add	r2, r2, #304087040
	add	r2, r2, #1269760
	addw	r2, r2, #1507
	str	r2, [r3, #4]
	ldrh	r3, [r0, #4]
	tst	r3, #1
	ite	eq
	moveq	r7, #6
	movne	r7, #7
.L823:
	ldrh	r3, [r4, #4]
	cbnz	r3, .L819
.L820:
	ldr	r3, .L830+8
	ldrh	r1, [r4]
	ldrh	r0, [r4, #4]
	ldr	r2, [r3]
	ldrh	r3, [r2, r1, lsl #1]
	subs	r3, r3, r0
	strh	r3, [r2, r1, lsl #1]	@ movhi
	ldr	r3, .L830+12
	ldrh	r3, [r3]
	strh	r3, [r4, #2]	@ movhi
	movs	r3, #0
	strb	r3, [r4, #6]
	strh	r3, [r4, #4]	@ movhi
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L819:
	mov	r0, r4
	bl	get_new_active_ppa
	str	r0, [sp, #8]
	adds	r0, r0, #1
	beq	.L820
	ldr	r3, [r8]
	movs	r1, #1
	add	r0, sp, #4
	str	r3, [r5, #4]
	adds	r3, r3, #1
	adds	r2, r3, #1
	it	eq
	moveq	r3, r6
	str	r3, [r8]
	movs	r3, #0
	mov	r2, r3
	bl	FlashProgPages
	ldrh	r0, [r4]
	bl	decrement_vpc_count
	subs	r7, r7, #1
	bne	.L823
	b	.L820
.L831:
	.align	2
.L830:
	.word	.LANCHOR113
	.word	.LANCHOR105
	.word	.LANCHOR41
	.word	.LANCHOR18
	.word	.LANCHOR82
	.size	FtlSuperblockPowerLostFix, .-FtlSuperblockPowerLostFix
	.section	.text.FtlMakeBbt,"ax",%progbits
	.align	1
	.global	FtlMakeBbt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlMakeBbt, %function
FtlMakeBbt:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r6, #0
	ldr	r7, .L851
	bl	FtlBbtMemInit
	sub	r8, r7, #18
	bl	FtlLoadFactoryBbt
.L833:
	ldr	r3, .L851+4
	ldrh	r3, [r3]
	cmp	r6, r3
	bcc	.L839
	ldr	r5, .L851+8
	movs	r4, #0
.L840:
	ldrh	r3, [r5]
	uxth	r0, r4
	adds	r4, r4, #1
	cmp	r3, r0
	bhi	.L841
	ldr	r4, .L851+12
	movw	r6, #65535
	ldrh	r5, [r4, #12]
	subs	r5, r5, #1
	uxth	r5, r5
.L842:
	ldrh	r3, [r4, #12]
	subs	r3, r3, #47
	cmp	r3, r5
	bgt	.L846
	mov	r0, r5
	bl	FtlBbmIsBadBlock
	cmp	r0, #1
	beq	.L843
	mov	r0, r5
	bl	FlashTestBlk
	cmp	r0, #0
	beq	.L844
	mov	r0, r5
	bl	FtlBbmMapBadBlock
.L843:
	subs	r5, r5, #1
	uxth	r5, r5
	b	.L842
.L839:
	ldr	r3, .L851+16
	ldr	r5, .L851+20
	ldrh	r2, [r8, #2]!
	ldr	r4, .L851+24
	ldr	r0, [r3]
	movw	r3, #65535
	ldr	r10, [r5]
	cmp	r2, r3
	mov	fp, r4
	ldr	r3, .L851+28
	str	r0, [r4, #8]
	str	r10, [r4, #12]
	beq	.L834
	ldrh	r5, [r3]
	mov	r0, r4
	str	r3, [sp]
	mla	r5, r6, r5, r2
	lsls	r2, r5, #10
	str	r2, [r4, #4]
	movs	r2, #1
	mov	r1, r2
	bl	FlashReadPages
	ldr	r3, [sp]
	ldr	r1, [r4, #8]
	ldr	r0, [r7]
	ldrh	r2, [r3]
	adds	r2, r2, #7
	asrs	r2, r2, #3
	bl	ftl_memcpy
.L835:
	uxth	r0, r5
	adds	r6, r6, #1
	adds	r7, r7, #4
	bl	FtlBbmMapBadBlock
	b	.L833
.L834:
	mov	r1, r6
	str	r3, [sp]
	bl	FlashGetBadBlockList
	ldr	r0, [r4, #8]
	ldr	r1, [r7]
	bl	FtlBbt2Bitmap
	ldr	r3, [sp]
	str	r5, [sp, #4]
	ldrh	r4, [r3]
	subs	r4, r4, #1
	uxth	r4, r4
.L836:
	ldr	r3, [sp]
	ldrh	r0, [r3]
	smlabb	r0, r0, r6, r4
	uxth	r0, r0
	bl	FtlBbmIsBadBlock
	cmp	r0, #1
	beq	.L837
	ldr	r3, [sp, #4]
	movs	r2, #16
	movs	r1, #0
	strh	r4, [r8]	@ movhi
	ldr	r0, [r3]
	bl	ftl_memset
	ldr	r3, .L851+16
	mov	r2, #4096
	movs	r1, #0
	ldr	r0, [r3]
	bl	ftl_memset
	ldr	r2, [sp]
	movw	r3, #61664
	strh	r3, [r10]	@ movhi
	movs	r3, #0
	str	r3, [r10, #4]
	ldrh	r5, [r2]
	ldrh	r3, [r8]
	ldr	r1, [r7]
	ldr	r0, [fp, #8]
	strh	r3, [r10, #2]	@ movhi
	mla	r5, r6, r5, r3
	lsls	r3, r5, #10
	str	r3, [fp, #4]
	ldr	r3, .L851+32
	ldrh	r2, [r3]
	lsls	r2, r2, #2
	bl	ftl_memcpy
	movs	r2, #1
	ldr	r0, .L851+24
	mov	r1, r2
	bl	FlashEraseBlocks
	movs	r3, #1
	ldr	r0, .L851+24
	mov	r2, r3
	mov	r1, r3
	bl	FlashProgPages
	ldr	r3, [fp]
	adds	r3, r3, #1
	bne	.L835
	uxth	r0, r5
	bl	FtlBbmMapBadBlock
	b	.L836
.L837:
	subs	r4, r4, #1
	uxth	r4, r4
	b	.L836
.L841:
	bl	FtlBbmMapBadBlock
	b	.L840
.L844:
	ldrh	r3, [r4]
	cmp	r3, r6
	bne	.L845
	strh	r5, [r4]	@ movhi
	b	.L843
.L845:
	strh	r5, [r4, #4]	@ movhi
.L846:
	ldr	r3, .L851+36
	movs	r5, #0
	str	r5, [r4, #8]
	movs	r1, #1
	movs	r2, #2
	strh	r5, [r4, #2]	@ movhi
	ldr	r0, [r3]
	ldrh	r3, [r4]
	lsls	r3, r3, #10
	str	r3, [r0, #4]
	ldrh	r3, [r4, #4]
	lsls	r3, r3, #10
	str	r3, [r0, #24]
	bl	FlashEraseBlocks
	ldrh	r0, [r4]
	bl	FtlBbmMapBadBlock
	ldrh	r0, [r4, #4]
	bl	FtlBbmMapBadBlock
	bl	FtlBbmTblFlush
	ldr	r3, [r4, #8]
	ldrh	r2, [r4, #4]
	strh	r5, [r4, #2]	@ movhi
	adds	r3, r3, #1
	str	r3, [r4, #8]
	ldrh	r3, [r4]
	strh	r2, [r4]	@ movhi
	strh	r3, [r4, #4]	@ movhi
	bl	FtlBbmTblFlush
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L852:
	.align	2
.L851:
	.word	.LANCHOR36+28
	.word	.LANCHOR9
	.word	.LANCHOR24
	.word	.LANCHOR36
	.word	.LANCHOR105
	.word	.LANCHOR113
	.word	.LANCHOR137
	.word	.LANCHOR16
	.word	.LANCHOR126
	.word	.LANCHOR78
	.size	FtlMakeBbt, .-FtlMakeBbt
	.section	.text.ftl_memcmp,"ax",%progbits
	.align	1
	.global	ftl_memcmp
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_memcmp, %function
ftl_memcmp:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	memcmp
	.size	ftl_memcmp, .-ftl_memcmp
	.section	.text.js_hash,"ax",%progbits
	.align	1
	.global	js_hash
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	js_hash, %function
js_hash:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L857
	add	r1, r1, r0
	push	{r4, lr}
.L855:
	cmp	r0, r1
	bne	.L856
	mov	r0, r3
	pop	{r4, pc}
.L856:
	lsrs	r2, r3, #2
	ldrb	r4, [r0], #1	@ zero_extendqisi2
	add	r2, r2, r3, lsl #5
	add	r2, r2, r4
	eors	r3, r3, r2
	b	.L855
.L858:
	.align	2
.L857:
	.word	1204201446
	.size	js_hash, .-js_hash
	.section	.text.Ftl_write_map_blk_to_last_page,"ax",%progbits
	.align	1
	.global	Ftl_write_map_blk_to_last_page
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	Ftl_write_map_blk_to_last_page, %function
Ftl_write_map_blk_to_last_page:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r10, lr}
	movw	r2, #65535
	ldrh	r3, [r0]
	mov	r4, r0
	ldr	r5, [r0, #12]
	cmp	r3, r2
	bne	.L860
	ldrh	r3, [r0, #8]
	cbz	r3, .L861
	movw	r2, #641
	ldr	r1, .L869
	ldr	r0, .L869+4
	bl	printf
.L861:
	ldrh	r3, [r4, #8]
	adds	r3, r3, #1
	strh	r3, [r4, #8]	@ movhi
	bl	FtlFreeSysBlkQueueOut
	movs	r3, #0
	strh	r0, [r5]	@ movhi
	strh	r3, [r4, #2]	@ movhi
	strh	r3, [r4]	@ movhi
	ldr	r3, [r4, #28]
	adds	r3, r3, #1
	str	r3, [r4, #28]
.L862:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r10, pc}
.L860:
	ldrh	r7, [r5, r3, lsl #1]
	movs	r1, #255
	ldrh	r3, [r0, #2]
	ldr	r6, .L869+8
	ldr	r10, .L869+24
	ldr	r8, [r0, #24]
	orr	r3, r3, r7, lsl #10
	ldr	r0, [r10]
	str	r3, [r6, #4]
	ldr	r3, .L869+12
	str	r0, [r6, #8]
	ldr	r5, [r3]
	ldr	r3, [r4, #28]
	str	r5, [r6, #12]
	str	r3, [r5, #4]
	movw	r3, #64245
	strh	r3, [r5, #8]	@ movhi
	ldrh	r3, [r4, #4]
	strh	r7, [r5, #2]	@ movhi
	strh	r3, [r5]	@ movhi
	ldr	r3, .L869+16
	ldrh	r2, [r3]
	lsls	r2, r2, #3
	bl	ftl_memset
	ldrh	ip, [r4, #6]
	movs	r3, #0
	ldr	r1, [r10]
	mov	r2, r3
.L863:
	uxth	r0, r3
	cmp	ip, r0
	bhi	.L865
	ldr	r3, .L869+20
	ldr	r0, [r6, #8]
	ldrh	r1, [r3]
	bl	js_hash
	movs	r2, #1
	str	r0, [r5, #12]
	movs	r3, #0
	mov	r1, r2
	ldr	r0, .L869+8
	bl	FlashProgPages
	ldrh	r3, [r4, #2]
	mov	r0, r4
	adds	r3, r3, #1
	strh	r3, [r4, #2]	@ movhi
	bl	ftl_map_blk_gc
	b	.L862
.L865:
	ldr	r0, [r8, r3, lsl #2]
	cmp	r7, r0, lsr #10
	bne	.L864
	adds	r2, r2, #1
	uxth	r2, r2
	str	r3, [r1, r2, lsl #3]
	add	lr, r1, r2, lsl #3
	ldr	r0, [r8, r3, lsl #2]
	str	r0, [lr, #4]
.L864:
	adds	r3, r3, #1
	b	.L863
.L870:
	.align	2
.L869:
	.word	.LANCHOR147
	.word	.LC1
	.word	.LANCHOR137
	.word	.LANCHOR113
	.word	.LANCHOR19
	.word	.LANCHOR22
	.word	.LANCHOR105
	.size	Ftl_write_map_blk_to_last_page, .-Ftl_write_map_blk_to_last_page
	.section	.text.FtlMapWritePage,"ax",%progbits
	.align	1
	.global	FtlMapWritePage
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlMapWritePage, %function
FtlMapWritePage:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	lsl	r10, r1, #2
	ldr	r8, .L896+32
	mov	r4, r0
	mov	r7, r1
	movs	r6, #0
	str	r2, [sp]
	mov	fp, r8
.L872:
	ldr	r2, .L896
	ldr	r3, [r2]
	adds	r3, r3, #1
	str	r3, [r2]
	ldrh	r3, [r8]
	ldrh	r2, [r4, #2]
	subs	r3, r3, #1
	cmp	r2, r3
	bge	.L873
	ldrh	r2, [r4]
	movw	r3, #65535
	cmp	r2, r3
	bne	.L874
.L873:
	mov	r0, r4
	bl	Ftl_write_map_blk_to_last_page
.L874:
	ldrh	r2, [r4]
	ldr	r3, [r4, #12]
	ldrh	r3, [r3, r2, lsl #1]
	cbnz	r3, .L875
	mov	r2, #700
	ldr	r1, .L896+4
	ldr	r0, .L896+8
	bl	printf
.L875:
	ldrh	r2, [r4]
	ldrh	r3, [r4, #10]
	cmp	r2, r3
	bcc	.L876
	movw	r2, #701
	ldr	r1, .L896+4
	ldr	r0, .L896+8
	bl	printf
.L876:
	ldrh	r2, [r4]
	movs	r1, #0
	ldr	r3, [r4, #12]
	ldr	r5, .L896+12
	ldrh	r3, [r3, r2, lsl #1]
	ldrh	r2, [r4, #2]
	str	r3, [sp, #4]
	orr	r2, r2, r3, lsl #10
	ldr	r3, [sp]
	str	r2, [r5, #4]
	ldr	r2, .L896+16
	str	r3, [r5, #8]
	ldr	r0, [r2]
	movs	r2, #16
	str	r0, [r5, #12]
	bl	ftl_memset
	ldr	r2, [r5, #12]
	ldr	r1, [r4, #28]
	ldr	r3, [sp, #4]
	strh	r7, [r2, #8]	@ movhi
	str	r1, [r2, #4]
	ldrh	r1, [r4, #4]
	strh	r3, [r2, #2]	@ movhi
	ldr	r3, .L896+20
	strh	r1, [r2]	@ movhi
	ldr	r0, [r5, #8]
	ldrh	r1, [r3]
	str	r2, [sp, #4]
	bl	js_hash
	ldr	r2, [sp, #4]
	movs	r3, #1
	mov	r1, r3
	str	r0, [r2, #12]
	mov	r2, r3
	mov	r0, r5
	bl	FlashProgPages
	ldrh	r2, [r4, #2]
	ldr	r3, [r5]
	adds	r2, r2, #1
	uxth	r2, r2
	adds	r3, r3, #1
	strh	r2, [r4, #2]	@ movhi
	bne	.L877
	ldr	r1, [r5, #4]
	adds	r6, r6, #1
	ldr	r0, .L896+24
	uxth	r6, r6
	str	r5, [sp, #4]
	bl	printf
	ldrh	r2, [r4, #2]
	ldr	r3, [sp, #4]
	cmp	r2, #2
	ittt	ls
	ldrhls	r2, [fp]
	addls	r2, r2, #-1
	strhls	r2, [r4, #2]	@ movhi
	cmp	r6, #3
	bls	.L879
	mov	r2, r6
	ldr	r1, [r3, #4]
	ldr	r0, .L896+28
	bl	printf
.L880:
	b	.L880
.L879:
	ldr	r3, [r4, #32]
	cmp	r3, #0
	beq	.L872
.L895:
	b	.L895
.L877:
	ldr	r1, [r5, #4]
	cmp	r2, #1
	ldr	r3, [r4, #24]
	str	r1, [r3, r10]
	beq	.L883
	ldr	r3, [r5]
	cmp	r3, #256
	beq	.L883
	ldr	r2, [r4, #36]
	cbz	r2, .L884
.L883:
	movs	r3, #0
	str	r3, [r4, #36]
	b	.L872
.L884:
	adds	r3, r3, #1
	bne	.L885
	movw	r2, #745
	ldr	r1, .L896+4
	ldr	r0, .L896+8
	bl	printf
.L885:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L897:
	.align	2
.L896:
	.word	.LANCHOR87
	.word	.LANCHOR148
	.word	.LC1
	.word	.LANCHOR137
	.word	.LANCHOR113
	.word	.LANCHOR22
	.word	.LC22
	.word	.LC23
	.word	.LANCHOR19
	.size	FtlMapWritePage, .-FtlMapWritePage
	.section	.text.load_l2p_region,"ax",%progbits
	.align	1
	.global	load_l2p_region
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	load_l2p_region, %function
load_l2p_region:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L905
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r5, r0
	mov	r10, r1
	ldrh	r2, [r3]
	str	r3, [sp, #4]
	cmp	r2, r0
	bcs	.L899
	movw	r2, #485
	ldr	r1, .L905+4
	ldr	r0, .L905+8
	bl	printf
.L899:
	ldr	fp, .L905+48
	movs	r4, #12
	ldr	r7, .L905+12
	ldr	r3, [fp]
	ldr	r8, [r3, r5, lsl #2]
	cmp	r8, #0
	bne	.L900
	mul	r4, r4, r10
	ldr	r2, [r7]
	movs	r1, #255
	adds	r0, r2, r4
	ldr	r2, .L905+16
	ldr	r0, [r0, #8]
	ldrh	r2, [r2]
	bl	ftl_memset
	ldr	r2, [r7]
	adds	r1, r2, r4
	strh	r5, [r2, r4]	@ movhi
	str	r8, [r1, #4]
.L901:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L900:
	mul	r4, r4, r10
	ldr	r2, [r7]
	ldr	r6, .L905+20
	add	r2, r2, r4
	mov	r0, r6
	ldr	r2, [r2, #8]
	str	r8, [r6, #4]
	str	r2, [r6, #8]
	ldr	r2, .L905+24
	ldr	r2, [r2]
	str	r2, [r6, #12]
	movs	r2, #1
	mov	r1, r2
	bl	FlashReadPages
	ldr	r10, [r6, #12]
	ldrh	r2, [r10, #8]
	cmp	r2, r5
	beq	.L902
	mov	r2, r8
	mov	r1, r5
	ldr	r0, .L905+28
	bl	printf
	movs	r3, #4
	ldr	r1, [r6, #12]
	mov	r2, r3
	ldr	r0, .L905+32
	bl	rknand_print_hex
	ldr	r3, [sp, #4]
	movs	r2, #4
	ldr	r1, [fp]
	ldr	r0, .L905+36
	ldrh	r3, [r3]
	bl	rknand_print_hex
.L903:
	ldrh	r3, [r10, #8]
	cmp	r3, r5
	beq	.L904
	mov	r2, #508
	ldr	r1, .L905+4
	ldr	r0, .L905+8
	bl	printf
.L904:
	ldr	r3, [r7]
	movs	r1, #0
	adds	r2, r3, r4
	str	r1, [r2, #4]
	strh	r5, [r3, r4]	@ movhi
	b	.L901
.L902:
	ldr	r2, [r6]
	cmp	r2, #256
	bne	.L903
	mov	r2, r8
	mov	r1, r5
	ldr	r0, .L905+40
	bl	printf
	ldr	r3, [r7]
	mov	r1, r5
	ldr	r0, .L905+44
	add	r3, r3, r4
	ldr	r2, [r3, #8]
	bl	FtlMapWritePage
	b	.L903
.L906:
	.align	2
.L905:
	.word	.LANCHOR31
	.word	.LANCHOR149
	.word	.LC1
	.word	.LANCHOR54
	.word	.LANCHOR22
	.word	.LANCHOR137
	.word	.LANCHOR113
	.word	.LC24
	.word	.LC13
	.word	.LC25
	.word	.LC26
	.word	.LANCHOR128
	.word	.LANCHOR123
	.size	load_l2p_region, .-load_l2p_region
	.section	.text.ftl_map_blk_gc,"ax",%progbits
	.align	1
	.global	ftl_map_blk_gc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_map_blk_gc, %function
ftl_map_blk_gc:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #24]
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r4, r0
	ldr	r5, [r0, #12]
	str	r3, [sp]
	bl	ftl_free_no_use_map_blk
	ldrh	r3, [r4, #10]
	ldrh	r2, [r4, #8]
	subs	r3, r3, #5
	cmp	r2, r3
	blt	.L908
	uxth	r0, r0
	ldrh	r8, [r5, r0, lsl #1]
	cmp	r8, #0
	beq	.L908
	ldr	r3, [r4, #32]
	cbnz	r3, .L908
	movs	r2, #1
	str	r2, [r4, #32]
	strh	r3, [r5, r0, lsl #1]	@ movhi
	ldrh	r3, [r4, #8]
	ldrh	r2, [r4, #2]
	subs	r3, r3, #1
	strh	r3, [r4, #8]	@ movhi
	ldr	r3, .L923
	ldrh	r3, [r3]
	cmp	r2, r3
	bcc	.L909
	mov	r0, r4
	bl	ftl_map_blk_alloc_new_blk
.L909:
	ldr	r5, .L923+4
	movs	r6, #0
.L910:
	ldrh	r3, [r4, #6]
	uxth	r10, r6
	cmp	r3, r10
	bhi	.L917
	movs	r1, #1
	mov	r0, r8
	bl	FtlFreeSysBlkQueueIn
	movs	r3, #0
	str	r3, [r4, #32]
.L908:
	ldr	r3, .L923
	ldrh	r2, [r4, #2]
	ldrh	r3, [r3]
	cmp	r2, r3
	bcc	.L918
	mov	r0, r4
	bl	ftl_map_blk_alloc_new_blk
.L918:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L917:
	ldr	r3, [sp]
	uxth	fp, r6
	add	r3, r3, fp, lsl #2
	str	r3, [sp, #4]
	ldr	r3, [sp]
	ldr	r2, [r3, fp, lsl #2]
	cmp	r8, r2, lsr #10
	bne	.L911
	ldr	r3, .L923+8
	str	r2, [r5, #4]
	movs	r2, #1
	ldr	r0, .L923+4
	ldr	r1, [r3]
	str	r1, [r5, #8]
	ldr	r1, .L923+12
	ldr	r7, [r1]
	mov	r1, r2
	str	r7, [r5, #12]
	bl	FlashReadPages
	ldrh	r2, [r7, #8]
	cmp	r2, r10
	beq	.L912
	movw	r2, #611
	ldr	r1, .L923+16
	ldr	r0, .L923+20
	bl	printf
.L912:
	ldr	r2, [r5]
	adds	r2, r2, #1
	bne	.L913
.L915:
	ldr	r2, [sp, #4]
	movs	r3, #0
	str	r3, [r2]
.L914:
	b	.L914
.L913:
	ldrh	r2, [r7, #8]
	cmp	r2, r10
	bne	.L915
	ldrh	r1, [r7]
	ldrh	r2, [r4, #4]
	cmp	r1, r2
	bne	.L915
	ldr	r2, [r5, #8]
	mov	r1, fp
	mov	r0, r4
	bl	FtlMapWritePage
.L911:
	adds	r6, r6, #1
	b	.L910
.L924:
	.align	2
.L923:
	.word	.LANCHOR19
	.word	.LANCHOR137
	.word	.LANCHOR106
	.word	.LANCHOR113
	.word	.LANCHOR150
	.word	.LC1
	.size	ftl_map_blk_gc, .-ftl_map_blk_gc
	.section	.text.flush_l2p_region,"ax",%progbits
	.align	1
	.global	flush_l2p_region
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	flush_l2p_region, %function
flush_l2p_region:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #12
	ldr	r5, .L926
	muls	r4, r0, r4
	ldr	r0, .L926+4
	ldr	r3, [r5]
	adds	r2, r3, r4
	ldrh	r1, [r3, r4]
	ldr	r2, [r2, #8]
	bl	FtlMapWritePage
	ldr	r3, [r5]
	movs	r0, #0
	add	r4, r4, r3
	ldr	r3, [r4, #4]
	bic	r3, r3, #-2147483648
	str	r3, [r4, #4]
	pop	{r3, r4, r5, pc}
.L927:
	.align	2
.L926:
	.word	.LANCHOR54
	.word	.LANCHOR128
	.size	flush_l2p_region, .-flush_l2p_region
	.section	.text.l2p_flush,"ax",%progbits
	.align	1
	.global	l2p_flush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	l2p_flush, %function
l2p_flush:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	movs	r4, #0
	ldr	r5, .L932
	ldr	r6, .L932+4
.L929:
	ldrh	r3, [r5]
	uxth	r0, r4
	cmp	r3, r0
	bhi	.L931
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L931:
	ldr	r2, [r6]
	uxth	r3, r4
	movs	r1, #12
	mla	r3, r1, r3, r2
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bge	.L930
	bl	flush_l2p_region
.L930:
	adds	r4, r4, #1
	b	.L929
.L933:
	.align	2
.L932:
	.word	.LANCHOR32
	.word	.LANCHOR54
	.size	l2p_flush, .-l2p_flush
	.section	.text.log2phys,"ax",%progbits
	.align	1
	.global	log2phys
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	log2phys, %function
log2phys:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r7, r2
	ldr	r4, .L947
	mov	r10, r0
	mov	r5, r1
	ldr	r3, .L947+4
	ldr	r2, [r4]
	ldrh	fp, [r3]
	cmp	r0, r2
	bcc	.L935
	mov	r2, #820
	ldr	r1, .L947+8
	ldr	r0, .L947+12
	bl	printf
.L935:
	ldr	r3, [r4]
	cmp	r10, r3
	bcs	.L936
	ldr	r3, .L947+16
	add	fp, fp, #7
	lsr	r6, r10, fp
	movs	r2, #0
	ldrh	r1, [r3]
	uxth	r6, r6
	ldr	r3, .L947+20
	ldr	r0, [r3]
	mov	r8, r3
	movs	r3, #12
.L937:
	uxth	r4, r2
	cmp	r4, r1
	bcc	.L942
	str	r3, [sp, #4]
	bl	select_l2p_ram_region
	ldr	r3, [sp, #4]
	mov	r4, r0
	ldr	r2, [r8]
	muls	r3, r0, r3
	adds	r1, r2, r3
	ldrh	r2, [r2, r3]
	movw	r3, #65535
	cmp	r2, r3
	beq	.L943
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bge	.L943
	bl	flush_l2p_region
.L943:
	mov	r1, r4
	mov	r0, r6
	bl	load_l2p_region
	b	.L939
.L936:
	mov	r0, #-1
	cbnz	r7, .L934
	str	r0, [r5]
.L934:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L942:
	adds	r2, r2, #1
	mla	ip, r3, r2, r0
	ldrh	ip, [ip, #-12]
	cmp	ip, r6
	bne	.L937
.L939:
	movs	r0, #1
	ldr	r2, [r8]
	lsl	r0, r0, fp
	movs	r3, #12
	subs	r0, r0, #1
	and	r0, r0, r10
	uxth	r0, r0
	mla	r3, r3, r4, r2
	cbnz	r7, .L940
	ldr	r3, [r3, #8]
	ldr	r3, [r3, r0, lsl #2]
	str	r3, [r5]
.L941:
	ldr	r2, [r8]
	movs	r3, #12
	mla	r4, r3, r4, r2
	ldr	r3, [r4, #4]
	adds	r2, r3, #1
	beq	.L945
	adds	r3, r3, #1
	str	r3, [r4, #4]
.L945:
	movs	r0, #0
	b	.L934
.L940:
	ldr	r1, [r5]
	ldr	r2, [r3, #8]
	str	r1, [r2, r0, lsl #2]
	ldr	r2, [r3, #4]
	orr	r2, r2, #-2147483648
	str	r2, [r3, #4]
	ldr	r3, .L947+24
	strh	r6, [r3]	@ movhi
	b	.L941
.L948:
	.align	2
.L947:
	.word	.LANCHOR151
	.word	.LANCHOR21
	.word	.LANCHOR152
	.word	.LC1
	.word	.LANCHOR32
	.word	.LANCHOR54
	.word	.LANCHOR55
	.size	log2phys, .-log2phys
	.section	.text.FtlReUsePrevPpa,"ax",%progbits
	.align	1
	.global	FtlReUsePrevPpa
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlReUsePrevPpa, %function
FtlReUsePrevPpa:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	ldr	r5, .L959
	ubfx	r0, r1, #10, #16
	str	r1, [sp, #4]
	bl	P2V_block_in_plane
	ldr	r2, [r5]
	mov	r7, r0
	ldrh	r3, [r2, r0, lsl #1]
	cbnz	r3, .L950
	ldr	r2, .L959+4
	ldr	r4, [r2]
	cmp	r4, #0
	beq	.L951
	ldr	r2, .L959+8
	movw	lr, #65535
	ldr	ip, .L959+24
	ldr	r0, .L959+12
	ldr	r2, [r2]
	ldrh	r1, [r0]
	mov	r8, r0
	subs	r4, r4, r2
	asrs	r4, r4, #1
	mul	r4, ip, r4
	mov	ip, #6
	uxth	r4, r4
.L952:
	uxth	r0, r3
	cmp	r1, r0
	bls	.L951
	cmp	r4, r7
	bne	.L953
	mov	r1, r4
	ldr	r0, .L959+4
	bl	List_remove_node
	ldrh	r3, [r8]
	cbnz	r3, .L954
	movw	r2, #1742
	ldr	r1, .L959+16
	ldr	r0, .L959+20
	bl	printf
.L954:
	ldrh	r3, [r8]
	mov	r0, r4
	subs	r3, r3, #1
	strh	r3, [r8]	@ movhi
	bl	INSERT_DATA_LIST
	ldr	r2, [r5]
	ldrh	r3, [r2, r7, lsl #1]
.L950:
	adds	r3, r3, #1
	strh	r3, [r2, r7, lsl #1]	@ movhi
	b	.L951
.L953:
	mul	r4, ip, r4
	adds	r3, r3, #1
	ldrh	r4, [r2, r4]
	cmp	r4, lr
	bne	.L952
.L951:
	movs	r2, #1
	add	r1, sp, #4
	mov	r0, r6
	bl	log2phys
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L960:
	.align	2
.L959:
	.word	.LANCHOR41
	.word	.LANCHOR45
	.word	.LANCHOR39
	.word	.LANCHOR47
	.word	.LANCHOR153
	.word	.LC1
	.word	-1431655765
	.size	FtlReUsePrevPpa, .-FtlReUsePrevPpa
	.section	.text.FtlRecoverySuperblock,"ax",%progbits
	.align	1
	.global	FtlRecoverySuperblock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlRecoverySuperblock, %function
FtlRecoverySuperblock:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	ldrh	r3, [r0]
	movw	r2, #65535
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r8, r0
	sub	sp, sp, #48
	cmp	r3, r2
	beq	.L1093
	ldrh	r3, [r0, #2]
	str	r3, [sp, #4]
	ldrb	r3, [r0, #6]	@ zero_extendqisi2
	ldr	r1, [sp, #4]
	str	r3, [sp, #20]
	ldr	r3, .L1102
	ldrh	r3, [r3]
	cmp	r3, r1
	mov	r3, #0
	bne	.L964
	strh	r3, [r0, #4]	@ movhi
.L1100:
	strb	r3, [r8, #6]
.L1093:
	movs	r0, #0
	add	sp, sp, #48
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L964:
	ldrh	r0, [r0, #16]
.L965:
	cmp	r0, r2
	uxth	r5, r3
	add	r3, r3, #1
	beq	.L966
	movs	r1, #1
	bl	FtlGetLastWrittenPage
	mov	r4, r0
	adds	r0, r0, #1
	beq	.L967
	ldr	r3, .L1102+4
	movs	r2, #0
	mov	r5, r2
	movw	fp, #65535
	mov	r10, #20
	ldrh	ip, [r3]
	ldr	r3, .L1102+8
	ldr	r0, [r3]
	ldr	r3, .L1102+12
	ldr	r3, [r3]
	str	r3, [sp, #8]
	ldr	r3, .L1102+16
	ldrh	r7, [r3]
	ldr	r3, .L1102+20
	ldr	r3, [r3]
	str	r3, [sp, #12]
	ldr	r3, .L1102+24
	ldrh	lr, [r3]
	add	r3, r8, #16
	str	r3, [sp, #16]
	str	r3, [sp]
.L968:
	uxth	r3, r2
	cmp	ip, r3
	bhi	.L972
	movs	r2, #0
	mov	r1, r5
	bl	FlashReadPages
	ldr	r2, .L1102+28
	uxth	r1, r4
	movw	fp, #65535
	str	r1, [sp, #12]
	ldr	r3, [r2]
	subs	r3, r3, #1
	str	r3, [sp]
	ldr	r3, .L1102+8
	ldr	r10, [r3]
	movs	r3, #0
	mov	r7, r10
.L973:
	uxth	r1, r3
	cmp	r5, r1
	bhi	.L978
	bne	.L976
	adds	r3, r4, #1
	uxth	r3, r3
	str	r3, [sp, #8]
.L1094:
	ldr	r0, [r10, #4]
	ubfx	r0, r0, #10, #16
	bl	P2V_plane
	ldr	r3, .L1102
	mov	r10, r0
	ldr	r2, [sp, #8]
	ldrh	r3, [r3]
	cmp	r3, r2
	bne	.L980
	ldrh	r3, [sp, #8]
	strh	r3, [r8, #2]	@ movhi
	movs	r3, #0
	strb	r3, [r8, #6]
	strh	r3, [r8, #4]	@ movhi
.L980:
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #4]
	cmp	r3, r2
	bne	.L981
	ldr	r3, [sp, #20]
	cmp	r10, r3
	bne	.L981
	ldr	r1, [sp, #8]
	mov	r2, r10
.L1101:
	mov	r0, r8
	bl	ftl_sb_update_avl_pages
	b	.L1093
.L966:
	uxth	r1, r3
	adds	r1, r1, #8
	ldrh	r0, [r8, r1, lsl #1]
	b	.L965
.L967:
	ldr	r3, [sp, #4]
	cbz	r3, .L969
	movw	r2, #1809
	ldr	r1, .L1102+32
	ldr	r0, .L1102+36
	bl	printf
.L969:
	ldr	r3, [sp, #20]
	cbz	r3, .L970
	cmp	r5, r3
	beq	.L970
	movw	r2, #1810
	ldr	r1, .L1102+32
	ldr	r0, .L1102+36
	bl	printf
.L970:
	movs	r3, #0
	strh	r3, [r8, #2]	@ movhi
	b	.L1100
.L972:
	ldr	r1, [sp]
	ldrh	r3, [r1], #2
	cmp	r3, fp
	str	r1, [sp]
	beq	.L971
	mla	r1, r10, r5, r0
	ldr	r6, [sp, #8]
	orr	r3, r4, r3, lsl #10
	str	r3, [r1, #4]
	mul	r3, r7, r5
	bic	r3, r3, #3
	add	r3, r3, r6
	ldr	r6, [sp, #12]
	str	r3, [r1, #8]
	mul	r3, lr, r5
	adds	r5, r5, #1
	uxth	r5, r5
	bic	r3, r3, #3
	add	r3, r3, r6
	str	r3, [r1, #12]
.L971:
	adds	r2, r2, #1
	b	.L968
.L978:
	ldr	r1, [r7]
	cbnz	r1, .L974
	ldr	r1, [r7, #12]
	ldr	r6, [r1, #4]
	str	r1, [sp, #8]
	adds	r1, r6, #1
	beq	.L975
	ldr	r1, [r2]
	mov	r0, r6
	bl	ftl_cmp_data_ver
	cbz	r0, .L975
	adds	r6, r6, #1
	str	r6, [r2]
.L975:
	ldr	r1, [sp, #8]
	ldr	r1, [r1]
	adds	r1, r1, #1
	bne	.L977
.L976:
	uxth	r2, r4
	uxth	r3, r3
	str	r2, [sp, #8]
	movs	r2, #20
	mla	r10, r2, r3, r10
	b	.L1094
.L974:
	ldr	fp, [sp, #12]
.L977:
	adds	r3, r3, #1
	adds	r7, r7, #20
	b	.L973
.L981:
	movw	r3, #65535
	cmp	fp, r3
	bne	.L982
	ldrb	r3, [r8, #8]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L983
.L982:
	ldr	r3, .L1102+40
	uxth	r6, r4
	uxth	r4, r4
	ldr	r7, .L1102+8
	ldr	r2, [r3]
	adds	r2, r2, #1
	itt	eq
	ldreq	r2, [sp]
	streq	r2, [r3]
	ldr	r3, [sp, #4]
	adds	r3, r3, #7
	cmp	r4, r3
	itet	gt
	subgt	r4, r6, #7
	ldrle	r4, [sp, #4]
	uxthgt	r4, r4
.L986:
	cmp	r4, r6
	bhi	.L996
	ldr	r3, .L1102+4
	movw	lr, #65535
	ldr	r0, [r7]
	mov	ip, #20
	ldrh	fp, [r3]
	ldr	r3, [sp, #16]
	str	r3, [sp, #12]
	movs	r3, #0
	mov	r5, r3
	b	.L997
.L988:
	ldr	r1, [sp, #12]
	ldrh	r2, [r1], #2
	cmp	r2, lr
	str	r1, [sp, #12]
	beq	.L987
	mla	r1, ip, r5, r0
	adds	r5, r5, #1
	orr	r2, r4, r2, lsl #10
	uxth	r5, r5
	str	r2, [r1, #4]
.L987:
	adds	r3, r3, #1
.L997:
	uxth	r2, r3
	cmp	fp, r2
	bhi	.L988
	mov	r1, r5
	movs	r2, #0
	bl	FlashReadPages
	ldr	r3, .L1102+40
	movs	r1, #20
	movs	r0, #0
	movw	ip, #65535
	ldr	r2, [r3]
	ldr	r3, [r7]
	mla	r5, r1, r5, r3
.L989:
	cmp	r5, r3
	bne	.L994
	cbz	r0, .L995
	ldr	r3, .L1102+40
	str	r2, [r3]
.L995:
	adds	r4, r4, #1
	uxth	r4, r4
	b	.L986
.L1103:
	.align	2
.L1102:
	.word	.LANCHOR18
	.word	.LANCHOR2
	.word	.LANCHOR102
	.word	.LANCHOR63
	.word	.LANCHOR22
	.word	.LANCHOR64
	.word	.LANCHOR23
	.word	.LANCHOR82
	.word	.LANCHOR154
	.word	.LC1
	.word	.LANCHOR133
.L994:
	ldr	r1, [r3]
	cmp	r1, #0
	beq	.L990
	cbz	r0, .L983
.L1095:
	ldr	r3, .L1104
	str	r2, [r3]
.L983:
	ldr	fp, [sp, #4]
	movs	r2, #1
	ldr	r6, .L1104+4
	ldr	r3, .L1104+8
	strh	r2, [r3]	@ movhi
.L998:
	ldr	r3, .L1104+12
	movw	lr, #65535
	ldr	r0, [r6]
	movs	r7, #20
	ldr	r1, [sp, #16]
	ldrh	r4, [r3]
	movs	r3, #0
	str	r3, [sp, #12]
.L999:
	uxth	r2, r3
	cmp	r4, r2
	bhi	.L1001
	movs	r2, #0
	ldr	r1, [sp, #12]
	bl	FlashReadPages
	movs	r3, #0
.L1099:
	str	r3, [sp, #24]
	ldr	r2, [sp, #12]
	ldrh	r3, [sp, #24]
	cmp	r2, r3
	bhi	.L1030
	ldr	r3, .L1104+16
	add	fp, fp, #1
	uxth	fp, fp
	ldrh	r3, [r3]
	cmp	r3, fp
	bne	.L998
	ldr	r2, .L1104+12
	movw	r0, #65535
	movs	r3, #0
	strh	fp, [r8, #2]	@ movhi
	strh	r3, [r8, #4]	@ movhi
	ldrh	r2, [r2]
.L1031:
	uxth	r1, r3
	cmp	r1, r2
	bcs	.L1093
	ldr	r1, [sp, #16]
	ldrh	r4, [r1], #2
	cmp	r4, r0
	str	r1, [sp, #16]
	add	r1, r3, #1
	bne	.L1100
	mov	r3, r1
	b	.L1031
.L990:
	ldr	r1, [r3, #12]
	ldrh	lr, [r1]
	cmp	lr, ip
	beq	.L993
	ldr	r1, [r1, #4]
	cmp	r1, #-1
	itt	ne
	movne	r2, r1
	movne	r0, #1
.L993:
	adds	r3, r3, #20
	b	.L989
.L996:
	mov	r2, #-1
	b	.L1095
.L1001:
	ldrh	r2, [r1], #2
	cmp	r2, lr
	beq	.L1000
	ldr	r5, [sp, #12]
	orr	r2, fp, r2, lsl #10
	mla	ip, r7, r5, r0
	str	r2, [ip, #4]
	mov	r2, r5
	adds	r2, r2, #1
	uxth	r2, r2
	str	r2, [sp, #12]
.L1000:
	adds	r3, r3, #1
	b	.L999
.L1030:
	ldr	r3, [sp, #24]
	movs	r5, #20
	muls	r5, r3, r5
	ldr	r3, [r6]
	str	r3, [sp, #28]
	adds	r7, r3, r5
	ldr	r4, [r7, #4]
	ubfx	r0, r4, #10, #16
	str	r4, [sp, #44]
	bl	P2V_plane
	ldr	r3, [sp, #4]
	cmp	fp, r3
	bcc	.L1003
	ldr	r3, [sp, #28]
	bne	.L1004
	ldr	r2, [sp, #20]
	cmp	r2, r0
	bhi	.L1003
.L1004:
	ldr	r2, [sp, #8]
	cmp	fp, r2
	bne	.L1005
	cmp	r10, r0
	beq	.L1006
.L1005:
	ldr	r3, [r3, r5]
	adds	r3, r3, #1
	beq	.L1007
	ldr	r3, [r7, #12]
	movw	r2, #61589
	ldrh	r1, [r3]
	cmp	r1, r2
	beq	.L1008
	ldrh	r0, [r8]
.L1096:
	bl	decrement_vpc_count
	b	.L1003
.L1008:
	ldr	r2, [r3, #4]
	str	r2, [sp]
	adds	r2, r2, #1
	beq	.L1009
	ldr	r2, .L1104+20
	ldr	r0, [sp]
	ldr	r1, [r2]
	bl	ftl_cmp_data_ver
	cbz	r0, .L1009
	ldr	r1, [sp]
	adds	r1, r1, #1
	str	r1, [r2]
.L1009:
	ldr	r4, [r3, #8]
	add	r1, sp, #40
	ldr	r3, [r3, #12]
	movs	r2, #0
	mov	r0, r4
	str	r3, [sp, #36]
	bl	log2phys
	ldr	r3, .L1104
	ldr	r1, [r3]
	adds	r3, r1, #1
	beq	.L1010
	ldr	r0, [sp]
	bl	ftl_cmp_data_ver
	cmp	r0, #0
	beq	.L1010
	ldr	r3, [sp, #36]
	adds	r7, r3, #1
	beq	.L1011
	ldr	r0, [r6]
	movs	r2, #0
	movs	r1, #1
	add	r0, r0, r5
	str	r3, [r0, #4]
	ldr	r7, [r0, #12]
	bl	FlashReadPages
	ldr	r2, [r6]
	ldr	r1, [r2, r5]
	adds	r3, r2, r5
	adds	r1, r1, #1
	bne	.L1012
.L1013:
	mov	r3, #-1
	str	r3, [sp, #36]
.L1020:
	ldr	r7, [sp, #36]
	adds	r0, r7, #1
	beq	.L1003
.L1034:
	ubfx	r0, r7, #10, #16
	bl	P2V_block_in_plane
	ldr	r3, .L1104+24
	mov	r4, r0
	ldrh	r3, [r3]
	cmp	r3, r0
	bhi	.L1026
	movw	r2, #2066
	ldr	r1, .L1104+28
	ldr	r0, .L1104+32
	bl	printf
.L1026:
	ldr	r3, .L1104+36
	ldr	r3, [r3]
	ldrh	r3, [r3, r4, lsl #1]
	cmp	r3, #0
	beq	.L1027
	mov	r0, r4
	b	.L1096
.L1011:
	ldr	r3, [sp, #44]
	ldr	r2, [sp, #40]
	cmp	r2, r3
	bne	.L1003
	movs	r2, #1
	add	r1, sp, #36
	mov	r0, r4
	bl	log2phys
.L1003:
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	b	.L1099
.L1012:
	ldr	r1, [r7, #8]
	cmp	r4, r1
	bne	.L1013
	ldr	r0, .L1104
	ldr	r1, [r7, #4]
	ldr	r0, [r0]
	str	r1, [sp, #28]
	bl	ftl_cmp_data_ver
	cmp	r0, #0
	beq	.L1013
	ldr	r1, [sp, #40]
	ldr	r0, [sp, #44]
	cmp	r1, r0
	bne	.L1015
.L1018:
	ldr	r1, [sp, #36]
	mov	r0, r4
	bl	FtlReUsePrevPpa
	b	.L1013
.L1015:
	ldr	r0, [sp, #36]
	cmp	r1, r0
	beq	.L1013
	adds	r0, r1, #1
	beq	.L1016
	str	r1, [r3, #4]
	movs	r2, #0
	movs	r1, #1
	mov	r0, r3
	ldr	r7, [r3, #12]
	bl	FlashReadPages
.L1017:
	ldr	r3, [r6]
	ldr	r3, [r3, r5]
	adds	r3, r3, #1
	beq	.L1018
	ldr	r3, [r7, #4]
	ldr	r2, .L1104
	mov	r1, r3
	ldr	r0, [r2]
	bl	ftl_cmp_data_ver
	cmp	r0, #0
	beq	.L1018
	mov	r1, r3
	ldr	r0, [sp, #28]
	bl	ftl_cmp_data_ver
	cmp	r0, #0
	beq	.L1013
	b	.L1018
.L1016:
	str	r1, [r2, r5]
	b	.L1017
.L1010:
	ldr	r3, [sp, #44]
	ldr	r2, [sp, #40]
	cmp	r2, r3
	beq	.L1020
	ldr	r1, [sp, #36]
	adds	r7, r1, #1
	beq	.L1022
	ldr	r3, .L1104+40
	ldr	r3, [r3]
	cmp	r3, r1, lsr #10
	bhi	.L1022
	ldr	r0, .L1104+44
.L1098:
	bl	printf
	b	.L1003
.L1105:
	.align	2
.L1104:
	.word	.LANCHOR133
	.word	.LANCHOR102
	.word	.LANCHOR155
	.word	.LANCHOR2
	.word	.LANCHOR18
	.word	.LANCHOR82
	.word	.LANCHOR4
	.word	.LANCHOR154
	.word	.LC1
	.word	.LANCHOR41
	.word	.LANCHOR7
	.word	.LC27
.L1022:
	movs	r2, #1
	add	r1, sp, #44
	mov	r0, r4
	bl	log2phys
	ldr	r7, [sp, #40]
	adds	r5, r7, #1
	beq	.L1020
	ldr	r3, [sp, #36]
	cmp	r7, r3
	beq	.L1034
	ubfx	r0, r7, #10, #16
	bl	P2V_block_in_plane
	ldr	r3, .L1106
	ldrh	r3, [r3]
	cmp	r3, r0
	beq	.L1025
	ldr	r3, .L1106+4
	ldrh	r3, [r3]
	cmp	r3, r0
	beq	.L1025
	ldr	r3, .L1106+8
	ldrh	r3, [r3]
	cmp	r3, r0
	bne	.L1020
.L1025:
	ldr	r0, [r6]
	movs	r2, #0
	movs	r1, #1
	str	r7, [r0, #4]
	ldr	r5, [r0, #12]
	bl	FlashReadPages
	ldr	r3, [r6]
	ldr	r3, [r3]
	adds	r3, r3, #1
	beq	.L1020
	ldr	r1, [r5, #4]
	ldr	r0, [sp]
	bl	ftl_cmp_data_ver
	cmp	r0, #0
	bne	.L1020
	movs	r2, #1
	add	r1, sp, #40
	mov	r0, r4
	bl	log2phys
	b	.L1020
.L1027:
	mov	r1, r4
	ldr	r0, .L1106+12
	b	.L1098
.L1007:
	ldr	r3, .L1106+16
	ldr	r3, [r3]
	cmp	r3, #31
	bhi	.L1028
	ldr	r2, .L1106+20
	str	r4, [r2, r3, lsl #2]
	adds	r3, r3, #1
	ldr	r2, .L1106+16
	str	r3, [r2]
.L1028:
	ldrh	r0, [r8]
	bl	decrement_vpc_count
	ldr	r3, .L1106+24
	ldr	r2, [r3]
	adds	r1, r2, #1
	bne	.L1029
	ldr	r2, [sp]
.L1097:
	str	r2, [r3]
	b	.L1003
.L1029:
	ldr	r1, [sp]
	cmp	r1, r2
	bcs	.L1003
	mov	r2, r1
	b	.L1097
.L1006:
	strb	r10, [r8, #6]
	mov	r2, r10
	strh	fp, [r8, #2]	@ movhi
	mov	r1, fp
	b	.L1101
.L1107:
	.align	2
.L1106:
	.word	.LANCHOR50
	.word	.LANCHOR51
	.word	.LANCHOR52
	.word	.LC28
	.word	.LANCHOR156
	.word	.LANCHOR157
	.word	.LANCHOR133
	.size	FtlRecoverySuperblock, .-FtlRecoverySuperblock
	.section	.text.ftl_check_vpc,"ax",%progbits
	.align	1
	.global	ftl_check_vpc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_check_vpc, %function
ftl_check_vpc:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r4, #0
	ldr	r6, .L1132
	ldr	r5, .L1132+4
	ldr	r1, .L1132+8
	ldr	r0, .L1132+12
	bl	printf
	mov	r2, #8192
	movs	r1, #0
	ldr	r0, .L1132+4
	bl	ftl_memset
.L1109:
	ldr	r3, [r6]
	cmp	r4, r3
	bcc	.L1111
	ldr	r10, .L1132+48
	movs	r4, #0
	ldr	r7, .L1132+16
	mov	r6, r4
.L1112:
	ldrh	r2, [r10]
	uxth	r3, r4
	cmp	r2, r3
	bhi	.L1114
	ldr	r3, .L1132+20
	ldr	r4, [r3]
	cbz	r4, .L1115
	ldr	r3, .L1132+24
	mov	r8, #0
	ldr	r5, .L1132+28
	ldr	r10, .L1132+16
	ldrh	r7, [r3]
	ldr	r3, [r5]
	ldr	fp, .L1132+4
	subs	r4, r4, r3
	ldr	r3, .L1132+32
	asrs	r4, r4, #1
	muls	r4, r3, r4
	uxth	r4, r4
.L1116:
	uxth	r3, r8
	cmp	r7, r3
	bls	.L1115
	ldr	r3, [r10]
	ldrh	r2, [r3, r4, lsl #1]
	cbz	r2, .L1117
	movs	r6, #1
	ldrh	r3, [fp, r4, lsl #1]
	mov	r1, r4
	ldr	r0, .L1132+36
	bl	printf
.L1117:
	movs	r3, #6
	ldr	r2, [r5]
	muls	r4, r3, r4
	movw	r3, #65535
	add	r8, r8, #1
	ldrh	r4, [r2, r4]
	cmp	r4, r3
	bne	.L1116
.L1115:
	cbz	r6, .L1108
	movw	r2, #2394
	ldr	r1, .L1132+8
	ldr	r0, .L1132+40
	bl	printf
.L1108:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1111:
	movs	r2, #0
	add	r1, sp, #4
	mov	r0, r4
	bl	log2phys
	ldr	r0, [sp, #4]
	adds	r3, r0, #1
	beq	.L1110
	ubfx	r0, r0, #10, #16
	bl	P2V_block_in_plane
	ldrh	r3, [r5, r0, lsl #1]
	adds	r3, r3, #1
	strh	r3, [r5, r0, lsl #1]	@ movhi
.L1110:
	adds	r4, r4, #1
	b	.L1109
.L1114:
	ldr	r3, [r7]
	uxth	r5, r4
	ldr	r8, .L1132+4
	ldrh	r2, [r3, r5, lsl #1]
	ldrh	r3, [r8, r5, lsl #1]
	cmp	r2, r3
	beq	.L1113
	mov	r1, r5
	ldr	r0, .L1132+44
	bl	printf
	ldr	r3, [r7]
	movw	r2, #65535
	ldrh	r3, [r3, r5, lsl #1]
	cmp	r3, r2
	beq	.L1113
	ldrh	r2, [r8, r5, lsl #1]
	cmp	r2, r3
	it	hi
	movhi	r6, #1
.L1113:
	adds	r4, r4, #1
	b	.L1112
.L1133:
	.align	2
.L1132:
	.word	.LANCHOR151
	.word	check_vpc_table
	.word	.LANCHOR158
	.word	.LC29
	.word	.LANCHOR41
	.word	.LANCHOR45
	.word	.LANCHOR47
	.word	.LANCHOR39
	.word	-1431655765
	.word	.LC31
	.word	.LC1
	.word	.LC30
	.word	.LANCHOR4
	.size	ftl_check_vpc, .-ftl_check_vpc
	.section	.text.ftl_scan_all_data,"ax",%progbits
	.align	1
	.global	ftl_scan_all_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_scan_all_data, %function
ftl_scan_all_data:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	movs	r5, #0
	ldr	r7, .L1148
	sub	sp, sp, #32
	movs	r1, #0
	ldr	r8, .L1148+24
	ldr	r0, .L1148+4
	bl	printf
.L1135:
	ldr	r3, [r7]
	cmp	r5, r3
	bcc	.L1141
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L1141:
	movs	r2, #0
	add	r1, sp, #28
	mov	r0, r5
	bl	log2phys
	ubfx	r3, r5, #0, #11
	cbnz	r3, .L1136
	ldr	r2, [sp, #28]
	mov	r1, r5
	mov	r0, r8
	bl	printf
.L1136:
	ldr	r3, [sp, #28]
	adds	r2, r3, #1
	beq	.L1138
	ldr	r4, .L1148+8
	movs	r2, #0
	movs	r1, #1
	str	r3, [r4, #4]
	mov	r0, r4
	ldr	r3, .L1148+12
	str	r5, [r4, #16]
	str	r2, [r4]
	ldr	r3, [r3]
	str	r3, [r4, #8]
	ldr	r3, .L1148+16
	ldr	r6, [r3]
	str	r6, [r4, #12]
	bl	FlashReadPages
	ldr	r3, [r4]
	cmp	r3, #256
	beq	.L1139
	adds	r3, r3, #1
	beq	.L1139
	ldr	r3, [r6, #8]
	cmp	r5, r3
	beq	.L1138
.L1139:
	ldr	r2, [r4, #8]
	ldr	r3, [r4, #12]
	ldr	r0, .L1148+20
	ldr	r1, [r2, #4]
	str	r1, [sp, #16]
	mov	r1, r5
	ldr	r2, [r2]
	str	r2, [sp, #12]
	ldr	r2, [r3, #12]
	str	r2, [sp, #8]
	ldr	r2, [r3, #8]
	str	r2, [sp, #4]
	ldr	r2, [r3, #4]
	str	r2, [sp]
	ldr	r2, [r4, #4]
	ldr	r3, [r3]
	bl	printf
.L1138:
	adds	r5, r5, #1
	b	.L1135
.L1149:
	.align	2
.L1148:
	.word	.LANCHOR151
	.word	.LC32
	.word	.LANCHOR137
	.word	.LANCHOR105
	.word	.LANCHOR113
	.word	.LC34
	.word	.LC33
	.size	ftl_scan_all_data, .-ftl_scan_all_data
	.section	.text.FtlGcScanTempBlk,"ax",%progbits
	.align	1
	.global	FtlGcScanTempBlk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcScanTempBlk, %function
FtlGcScanTempBlk:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L1182
	movw	r2, #65535
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #48
	mov	r4, r0
	ldrh	r5, [r3]
	str	r3, [sp, #20]
	cmp	r5, r2
	beq	.L1172
	cbnz	r5, .L1151
.L1152:
	bl	FtlGcPageVarInit
	b	.L1153
.L1172:
	movs	r5, #0
.L1151:
	ldr	r3, .L1182+4
	ldrh	r3, [r3]
	cmp	r3, r1
	beq	.L1152
.L1153:
	ldr	fp, .L1182+56
.L1169:
	ldrh	r2, [r4]
	movs	r3, #0
	strb	r3, [r4, #8]
	movw	r3, #65535
	cmp	r2, r3
	beq	.L1154
.L1171:
	ldr	r3, .L1182+8
	movs	r2, #0
	ldr	r0, [fp]
	mov	r10, r2
	movw	lr, #65535
	mov	r8, #20
	ldrh	r3, [r3]
	str	r3, [sp, #8]
	ldr	r3, .L1182+12
	ldr	r3, [r3]
	str	r3, [sp, #12]
	ldr	r3, .L1182+16
	ldrh	r3, [r3]
	str	r3, [sp, #16]
	ldr	r3, .L1182+20
	ldr	r7, [r3]
	ldr	r3, .L1182+24
	ldrh	ip, [r3]
	add	r3, r4, #16
	str	r3, [sp, #4]
.L1155:
	ldr	r1, [sp, #8]
	uxth	r3, r2
	cmp	r1, r3
	bhi	.L1157
	mov	r8, #0
	movs	r2, #0
	mov	r1, r10
	bl	FlashReadPages
.L1158:
	uxth	r3, r8
	cmp	r10, r3
	bhi	.L1170
	ldr	r3, .L1182+4
	adds	r5, r5, #1
	uxth	r5, r5
	ldrh	r3, [r3]
	cmp	r3, r5
	bhi	.L1171
.L1154:
	ldr	r2, [sp, #20]
	movw	r3, #65535
	mov	r0, r4
	strh	r5, [r4, #2]	@ movhi
	mov	r1, r5
	strh	r3, [r2]	@ movhi
	movs	r2, #0
	strb	r2, [r4, #6]
	bl	ftl_sb_update_avl_pages
	mov	r0, #-1
	add	sp, sp, #48
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1157:
	ldr	r1, [sp, #4]
	ldrh	r3, [r1], #2
	cmp	r3, lr
	str	r1, [sp, #4]
	beq	.L1156
	mla	r1, r8, r10, r0
	orr	r3, r5, r3, lsl #10
	str	r3, [r1, #4]
	ldr	r3, [sp, #16]
	mul	r3, r3, r10
	bic	r3, r3, #3
	mov	r6, r3
	ldr	r3, [sp, #12]
	add	r6, r6, r3
	mul	r3, ip, r10
	str	r6, [r1, #8]
	bic	r3, r3, #3
	add	r3, r3, r7
	str	r3, [r1, #12]
	add	r3, r10, #1
	uxth	r10, r3
.L1156:
	adds	r2, r2, #1
	b	.L1155
.L1170:
	movs	r3, #20
	ldr	r2, [fp]
	mul	r3, r3, r8
	adds	r1, r2, r3
	ldr	r6, [r2, r3]
	ldr	r0, [r1, #4]
	ldr	r7, [r1, #12]
	str	r0, [sp, #4]
	cmp	r6, #0
	bne	.L1159
	ldrh	r1, [r7]
	movw	r2, #65535
	cmp	r1, r2
	beq	.L1180
	ldr	r2, .L1182+28
	ldr	r0, [r7, #8]
	ldr	r2, [r2]
	cmp	r0, r2
	bls	.L1161
.L1180:
	ldr	r3, .L1182+32
	movs	r1, #0
	ldrh	r2, [r4]
	movs	r5, #0
	ldr	r3, [r3]
	strh	r1, [r3, r2, lsl #1]	@ movhi
	ldrh	r0, [r4]
	bl	INSERT_FREE_LIST
	ldr	r2, .L1182+36
	movw	r3, #65535
	strh	r3, [r4]	@ movhi
	strh	r3, [r2]	@ movhi
	bl	FtlGcPageVarInit
	b	.L1169
.L1161:
	mov	r2, r6
	add	r1, sp, #24
	str	r3, [sp, #8]
	bl	log2phys
	ldr	r2, [r7, #12]
	ldr	r1, [sp, #24]
	ldr	r3, [sp, #8]
	cmp	r2, r1
	beq	.L1163
.L1165:
	ldr	r2, [r7, #8]
.L1181:
	ldr	r1, [sp, #4]
	add	r8, r8, #1
	ldr	r0, [r7, #12]
	bl	FtlGcUpdatePage
	b	.L1158
.L1163:
	str	r3, [sp, #8]
	adds	r3, r2, #1
	beq	.L1165
	str	r2, [sp, #32]
	movs	r1, #1
	ldr	r2, .L1182+40
	add	r0, sp, #28
	ldr	r2, [r2]
	str	r2, [sp, #36]
	ldr	r2, .L1182+44
	ldr	r2, [r2]
	str	r2, [sp, #40]
	mov	r2, r6
	bl	FlashReadPages
	ldr	r2, .L1182+48
	ldr	r1, [fp]
	ldr	r3, [sp, #8]
	ldrh	r2, [r2]
	ldr	r0, [sp, #36]
	add	ip, r3, r1
	lsls	r2, r2, #7
.L1166:
	cmp	r6, r2
	beq	.L1165
	ldr	r1, [ip, #8]
	ldr	r3, [r0, r6, lsl #2]
	ldr	r1, [r1, r6, lsl #2]
	cmp	r1, r3
	beq	.L1167
	ldr	r2, [sp, #32]
	ldrh	r1, [r4]
	ldr	r0, .L1182+52
	bl	printf
	b	.L1180
.L1167:
	adds	r6, r6, #1
	b	.L1166
.L1159:
	mov	r2, #-1
	b	.L1181
.L1183:
	.align	2
.L1182:
	.word	.LANCHOR159
	.word	.LANCHOR18
	.word	.LANCHOR2
	.word	.LANCHOR63
	.word	.LANCHOR22
	.word	.LANCHOR64
	.word	.LANCHOR23
	.word	.LANCHOR151
	.word	.LANCHOR41
	.word	.LANCHOR144
	.word	.LANCHOR109
	.word	.LANCHOR114
	.word	.LANCHOR11
	.word	.LC35
	.word	.LANCHOR102
	.size	FtlGcScanTempBlk, .-FtlGcScanTempBlk
	.section	.text.FtlVendorPartWrite,"ax",%progbits
	.align	1
	.global	FtlVendorPartWrite
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlVendorPartWrite, %function
FtlVendorPartWrite:
	@ args = 0, pretend = 0, frame = 104
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L1196
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #104
	str	r2, [sp]
	adds	r2, r0, r1
	mov	r7, r0
	mov	r5, r1
	ldrh	r3, [r3]
	cmp	r2, r3
	bhi	.L1192
	ldr	r3, .L1196+4
	mov	r8, #0
	ldrh	r6, [r3]
	lsr	r6, r0, r6
	lsl	fp, r6, #2
.L1186:
	cbnz	r5, .L1191
.L1184:
	mov	r0, r8
	add	sp, sp, #104
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1191:
	ldr	r3, .L1196+8
	mov	r0, r7
	ldr	r10, .L1196+24
	ldr	r3, [r3]
	ldr	r2, [r3, fp]
	ldr	r3, .L1196+12
	str	r2, [sp, #12]
	ldrh	r3, [r3]
	mov	r1, r3
	str	r3, [sp, #8]
	bl	__aeabi_uidivmod
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #12]
	str	r1, [sp, #4]
	subs	r4, r3, r1
	uxth	r4, r4
	cmp	r5, r4
	it	cc
	uxthcc	r4, r5
	cbz	r2, .L1188
	cmp	r4, r3
	beq	.L1188
	ldr	r3, [r10]
	add	r0, sp, #20
	str	r2, [sp, #24]
	movs	r2, #1
	mov	r1, r2
	str	r3, [sp, #28]
	add	r3, sp, #40
	str	r3, [sp, #32]
	bl	FlashReadPages
.L1189:
	lsls	r3, r4, #9
	ldr	r0, [r10]
	subs	r5, r5, r4
	mov	r2, r3
	str	r3, [sp, #8]
	ldm	sp, {r1, r3}
	add	r7, r7, r4
	add	fp, fp, #4
	add	r0, r0, r3, lsl #9
	bl	ftl_memcpy
	ldr	r2, [r10]
	mov	r1, r6
	ldr	r0, .L1196+16
	adds	r6, r6, #1
	bl	FtlMapWritePage
	ldr	r3, [sp]
	adds	r0, r0, #1
	it	eq
	moveq	r8, #-1
	mov	r2, r3
	ldr	r3, [sp, #8]
	add	r2, r2, r3
	str	r2, [sp]
	b	.L1186
.L1188:
	ldr	r3, .L1196+20
	movs	r1, #0
	ldr	r0, [r10]
	ldrh	r2, [r3]
	bl	ftl_memset
	b	.L1189
.L1192:
	mov	r8, #-1
	b	.L1184
.L1197:
	.align	2
.L1196:
	.word	.LANCHOR15
	.word	.LANCHOR21
	.word	.LANCHOR122
	.word	.LANCHOR11
	.word	.LANCHOR160
	.word	.LANCHOR22
	.word	.LANCHOR107
	.size	FtlVendorPartWrite, .-FtlVendorPartWrite
	.section	.text.Ftl_save_ext_data,"ax",%progbits
	.align	1
	.global	Ftl_save_ext_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	Ftl_save_ext_data, %function
Ftl_save_ext_data:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L1200
	ldr	r3, .L1200+4
	ldr	r1, [r2]
	cmp	r1, r3
	bne	.L1198
	ldr	r3, .L1200+8
	movs	r1, #1
	movs	r0, #0
	str	r3, [r2, #4]
	ldr	r3, .L1200+12
	ldr	r3, [r3]
	str	r3, [r2, #88]
	ldr	r3, .L1200+16
	ldr	r3, [r3]
	str	r3, [r2, #92]
	ldr	r3, .L1200+20
	ldr	r3, [r3]
	str	r3, [r2, #8]
	ldr	r3, .L1200+24
	ldr	r3, [r3]
	str	r3, [r2, #12]
	ldr	r3, .L1200+28
	ldr	r3, [r3]
	str	r3, [r2, #16]
	ldr	r3, .L1200+32
	ldr	r3, [r3]
	str	r3, [r2, #20]
	ldr	r3, .L1200+36
	ldr	r3, [r3]
	str	r3, [r2, #28]
	ldr	r3, .L1200+40
	ldr	r3, [r3]
	str	r3, [r2, #32]
	ldr	r3, .L1200+44
	ldr	r3, [r3]
	str	r3, [r2, #36]
	ldr	r3, .L1200+48
	ldr	r3, [r3]
	str	r3, [r2, #40]
	ldr	r3, .L1200+52
	ldr	r3, [r3]
	str	r3, [r2, #44]
	ldr	r3, .L1200+56
	ldr	r3, [r3]
	str	r3, [r2, #48]
	b	FtlVendorPartWrite
.L1198:
	bx	lr
.L1201:
	.align	2
.L1200:
	.word	.LANCHOR134
	.word	1179929683
	.word	1342177367
	.word	.LANCHOR161
	.word	.LANCHOR162
	.word	.LANCHOR83
	.word	.LANCHOR84
	.word	.LANCHOR88
	.word	.LANCHOR87
	.word	.LANCHOR90
	.word	.LANCHOR79
	.word	.LANCHOR85
	.word	.LANCHOR86
	.word	.LANCHOR91
	.word	.LANCHOR92
	.size	Ftl_save_ext_data, .-Ftl_save_ext_data
	.section	.text.FtlEctTblFlush,"ax",%progbits
	.align	1
	.global	FtlEctTblFlush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlEctTblFlush, %function
FtlEctTblFlush:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r2, .L1207
	push	{r3, lr}
	ldrh	r3, [r2]
	cmp	r3, #31
	itett	ls
	addls	r3, r3, #1
	movhi	r3, #32
	strhls	r3, [r2]	@ movhi
	movls	r3, #1
	ldr	r2, .L1207+4
	cbnz	r0, .L1204
	ldr	r1, [r2]
	ldr	r0, [r1, #20]
	ldr	r1, [r1, #16]
	add	r3, r3, r0
	cmp	r1, r3
	bcc	.L1205
.L1204:
	ldr	r2, [r2]
	movs	r0, #64
	ldr	r3, [r2, #16]
	str	r3, [r2, #20]
	ldr	r3, .L1207+8
	str	r3, [r2]
	ldr	r3, .L1207+12
	ldrh	r1, [r3]
	lsls	r3, r1, #9
	str	r3, [r2, #12]
	ldr	r3, [r2, #8]
	adds	r3, r3, #1
	str	r3, [r2, #8]
	movs	r3, #0
	str	r3, [r2, #4]
	bl	FtlVendorPartWrite
	bl	Ftl_save_ext_data
.L1205:
	movs	r0, #0
	pop	{r3, pc}
.L1208:
	.align	2
.L1207:
	.word	.LANCHOR163
	.word	.LANCHOR117
	.word	1112818501
	.word	.LANCHOR115
	.size	FtlEctTblFlush, .-FtlEctTblFlush
	.section	.text.sftl_vendor_write,"ax",%progbits
	.align	1
	.global	sftl_vendor_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sftl_vendor_write, %function
sftl_vendor_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r0, r0, #256
	b	FtlVendorPartWrite
	.size	sftl_vendor_write, .-sftl_vendor_write
	.section	.text.FtlVendorPartRead,"ax",%progbits
	.align	1
	.global	FtlVendorPartRead
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlVendorPartRead, %function
FtlVendorPartRead:
	@ args = 0, pretend = 0, frame = 104
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L1220
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r10, r2
	adds	r2, r0, r1
	sub	sp, sp, #104
	mov	r7, r0
	mov	r6, r1
	ldrh	r3, [r3]
	cmp	r2, r3
	bhi	.L1219
	ldr	r3, .L1220+4
	mov	r8, #0
	ldr	fp, .L1220+28
	ldrh	r5, [r3]
	lsr	r5, r0, r5
	lsls	r3, r5, #2
	str	r3, [sp]
.L1212:
	cbnz	r6, .L1218
.L1210:
	mov	r0, r8
	add	sp, sp, #104
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1218:
	ldr	r3, .L1220+8
	mov	r0, r7
	ldr	r2, [sp]
	ldr	r3, [r3]
	ldr	r3, [r3, r2]
	ldr	r2, .L1220+12
	str	r3, [sp, #8]
	ldrh	r4, [r2]
	mov	r1, r4
	bl	__aeabi_uidivmod
	subs	r4, r4, r1
	ldr	r3, [sp, #8]
	uxth	r4, r4
	str	r1, [sp, #4]
	cmp	r6, r4
	it	cc
	uxthcc	r4, r6
	lsls	r2, r4, #9
	str	r2, [sp, #8]
	cmp	r3, #0
	beq	.L1214
	ldr	r2, [fp]
	add	r0, sp, #20
	str	r3, [sp, #24]
	str	r3, [sp, #12]
	str	r2, [sp, #28]
	add	r2, sp, #40
	str	r2, [sp, #32]
	movs	r2, #1
	mov	r1, r2
	bl	FlashReadPages
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #12]
	adds	r2, r2, #1
	ldr	r2, .L1220+16
	it	eq
	moveq	r8, #-1
	ldr	r2, [r2]
	cmp	r2, #256
	bne	.L1216
	mov	r2, r3
	mov	r1, r5
	ldr	r0, .L1220+20
	bl	printf
	ldr	r2, [fp]
	mov	r1, r5
	ldr	r0, .L1220+24
	bl	FtlMapWritePage
.L1216:
	ldr	r1, [fp]
	lsls	r2, r4, #9
	ldr	r3, [sp, #4]
	mov	r0, r10
	add	r1, r1, r3, lsl #9
	bl	ftl_memcpy
.L1217:
	ldr	r3, [sp, #8]
	adds	r5, r5, #1
	subs	r6, r6, r4
	add	r7, r7, r4
	add	r10, r10, r3
	ldr	r3, [sp]
	adds	r3, r3, #4
	str	r3, [sp]
	b	.L1212
.L1214:
	lsls	r2, r4, #9
	mov	r1, r3
	mov	r0, r10
	bl	ftl_memset
	b	.L1217
.L1219:
	mov	r8, #-1
	b	.L1210
.L1221:
	.align	2
.L1220:
	.word	.LANCHOR15
	.word	.LANCHOR21
	.word	.LANCHOR122
	.word	.LANCHOR11
	.word	.LANCHOR137
	.word	.LC36
	.word	.LANCHOR160
	.word	.LANCHOR107
	.size	FtlVendorPartRead, .-FtlVendorPartRead
	.section	.text.FtlLoadEctTbl,"ax",%progbits
	.align	1
	.global	FtlLoadEctTbl
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlLoadEctTbl, %function
FtlLoadEctTbl:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r0, #64
	ldr	r4, .L1224
	ldr	r5, .L1224+4
	ldr	r2, [r4]
	ldrh	r1, [r5]
	bl	FtlVendorPartRead
	ldr	r3, [r4]
	ldr	r2, [r3]
	ldr	r3, .L1224+8
	cmp	r2, r3
	beq	.L1223
	ldr	r1, .L1224+12
	ldr	r0, .L1224+16
	bl	printf
	ldrh	r2, [r5]
	movs	r1, #0
	ldr	r0, [r4]
	lsls	r2, r2, #9
	bl	ftl_memset
.L1223:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L1225:
	.align	2
.L1224:
	.word	.LANCHOR117
	.word	.LANCHOR115
	.word	1112818501
	.word	.LC37
	.word	.LC38
	.size	FtlLoadEctTbl, .-FtlLoadEctTbl
	.section	.text.Ftl_load_ext_data,"ax",%progbits
	.align	1
	.global	Ftl_load_ext_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	Ftl_load_ext_data, %function
Ftl_load_ext_data:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r1, #1
	ldr	r4, .L1229
	movs	r0, #0
	ldr	r5, .L1229+4
	mov	r2, r4
	bl	FtlVendorPartRead
	ldr	r3, [r4]
	cmp	r3, r5
	beq	.L1227
	mov	r2, #512
	movs	r1, #0
	mov	r0, r4
	bl	ftl_memset
	str	r5, [r4]
.L1227:
	ldr	r3, [r4]
	cmp	r3, r5
	ldr	r3, .L1229+8
	bne	.L1228
	ldr	r1, [r4, #88]
	ldr	r2, .L1229+12
	str	r1, [r2]
	ldr	r1, [r4, #92]
	ldr	r2, .L1229+16
	str	r1, [r2]
	ldr	r1, [r4, #8]
	ldr	r2, .L1229+20
	str	r1, [r2]
	ldr	r1, [r4, #12]
	ldr	r2, .L1229+24
	str	r1, [r2]
	ldr	r1, [r4, #16]
	ldr	r2, .L1229+28
	str	r1, [r2]
	ldr	r1, [r4, #20]
	ldr	r2, .L1229+32
	str	r1, [r2]
	ldr	r2, [r4, #28]
	ldr	r1, [r4, #32]
	str	r2, [r3]
	ldr	r2, .L1229+36
	str	r1, [r2]
	ldr	r1, [r4, #36]
	ldr	r2, .L1229+40
	str	r1, [r2]
	ldr	r1, [r4, #40]
	ldr	r2, .L1229+44
	str	r1, [r2]
	ldr	r1, [r4, #44]
	ldr	r2, .L1229+48
	str	r1, [r2]
	ldr	r1, [r4, #48]
	ldr	r2, .L1229+52
	str	r1, [r2]
.L1228:
	ldr	r1, .L1229+56
	ldr	r2, .L1229+60
	ldr	r3, [r3]
	ldr	r0, [r1]
	ldrh	r2, [r2]
	ldr	r1, .L1229+64
	mla	r0, r0, r2, r3
	ldrh	r1, [r1]
	bl	__aeabi_uidiv
	ldr	r3, .L1229+68
	str	r0, [r3]
	pop	{r3, r4, r5, pc}
.L1230:
	.align	2
.L1229:
	.word	.LANCHOR134
	.word	1179929683
	.word	.LANCHOR90
	.word	.LANCHOR161
	.word	.LANCHOR162
	.word	.LANCHOR83
	.word	.LANCHOR84
	.word	.LANCHOR88
	.word	.LANCHOR87
	.word	.LANCHOR79
	.word	.LANCHOR85
	.word	.LANCHOR86
	.word	.LANCHOR91
	.word	.LANCHOR92
	.word	.LANCHOR89
	.word	.LANCHOR13
	.word	.LANCHOR4
	.word	.LANCHOR142
	.size	Ftl_load_ext_data, .-Ftl_load_ext_data
	.section	.text.sftl_vendor_read,"ax",%progbits
	.align	1
	.global	sftl_vendor_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sftl_vendor_read, %function
sftl_vendor_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r0, r0, #256
	b	FtlVendorPartRead
	.size	sftl_vendor_read, .-sftl_vendor_read
	.section	.text.FtlMapBlkWriteDump_data,"ax",%progbits
	.align	1
	.global	FtlMapBlkWriteDump_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlMapBlkWriteDump_data, %function
FtlMapBlkWriteDump_data:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r10, lr}
	mov	r4, r0
	ldr	r3, [r0, #36]
	cmp	r3, #0
	beq	.L1232
	movs	r3, #0
	ldr	r5, .L1244
	str	r3, [r0, #36]
	ldr	r3, .L1244+4
	ldrh	r6, [r0, #6]
	mov	r7, r5
	ldr	r10, [r0, #24]
	ldr	r3, [r3]
	str	r3, [r5, #8]
	ldr	r3, .L1244+8
	ldr	r8, [r3]
	ldrh	r3, [r0, #2]
	str	r8, [r5, #12]
	cbz	r3, .L1234
	ldr	r2, .L1244+12
	ldrh	r2, [r2]
	subs	r2, r2, #1
	cmp	r3, r2
	bge	.L1234
	ldrh	r2, [r0]
	movw	r1, #65535
	cmp	r2, r1
	beq	.L1234
	ldr	r1, [r0, #12]
	subs	r3, r3, #1
	mov	r0, r5
	ldrh	r2, [r1, r2, lsl #1]
	orr	r3, r3, r2, lsl #10
	movs	r2, #1
	mov	r1, r2
	str	r3, [r5, #4]
	bl	FlashReadPages
	ldr	r3, [r5]
	adds	r3, r3, #1
	beq	.L1234
	ldr	r3, [r4, #24]
	ldrh	r1, [r8, #8]
	ldr	r2, [r3, r1, lsl #2]
	ldr	r3, [r5, #4]
	cmp	r2, r3
	bne	.L1234
	ldr	r2, [r5, #8]
.L1243:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, r8, r10, lr}
	b	FtlMapWritePage
.L1234:
	subs	r6, r6, #1
	uxth	r6, r6
	ldr	r3, [r10, r6, lsl #2]
	str	r3, [r7, #4]
	cbz	r3, .L1235
	movs	r2, #1
	ldr	r0, .L1244
	mov	r1, r2
	bl	FlashReadPages
.L1236:
	ldr	r2, [r7, #8]
	mov	r1, r6
	b	.L1243
.L1235:
	ldr	r3, .L1244+16
	movs	r1, #255
	ldr	r0, [r7, #8]
	ldrh	r2, [r3]
	bl	ftl_memset
	b	.L1236
.L1232:
	pop	{r3, r4, r5, r6, r7, r8, r10, pc}
.L1245:
	.align	2
.L1244:
	.word	.LANCHOR137
	.word	.LANCHOR106
	.word	.LANCHOR113
	.word	.LANCHOR19
	.word	.LANCHOR22
	.size	FtlMapBlkWriteDump_data, .-FtlMapBlkWriteDump_data
	.section	.text.FtlVpcTblFlush,"ax",%progbits
	.align	1
	.global	FtlVpcTblFlush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlVpcTblFlush, %function
FtlVpcTblFlush:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	mov	r8, #0
	ldr	r3, .L1261
	ldr	r5, .L1261+4
	ldr	fp, .L1261+72
	ldr	r0, [r3]
	ldr	r6, .L1261+8
	ldr	r7, [fp]
	ldrh	r2, [r5]
	str	r0, [r6, #8]
	str	r7, [r6, #12]
	strh	r2, [r7, #2]	@ movhi
	movw	r2, #61604
	strh	r2, [r7]	@ movhi
	ldr	r2, [r5, #8]
	ldr	r4, .L1261+12
	ldr	ip, .L1261+76
	str	r2, [r7, #4]
	ldr	r2, .L1261+16
	str	r8, [r7, #8]
	str	r8, [r7, #12]
	stm	r4, {r2, ip}
	ldrh	r2, [r5, #6]
	str	r3, [sp]
	ldr	r10, .L1261+80
	strh	r2, [r4, #8]	@ movhi
	ldr	r2, .L1261+20
	ldr	r3, .L1261+24
	ldrh	r2, [r2]
	strb	r2, [r4, #10]
	ldr	r2, .L1261+28
	ldrh	r1, [r2]
	ldrh	ip, [r2, #2]
	strh	r1, [r4, #14]	@ movhi
	ldrb	r1, [r2, #6]	@ zero_extendqisi2
	ldrb	r2, [r2, #8]	@ zero_extendqisi2
	strb	r2, [r4, #11]
	orr	r1, r1, ip, lsl #6
	ldr	r2, .L1261+32
	strh	r1, [r4, #16]	@ movhi
	ldrh	r1, [r2]
	ldrh	ip, [r2, #2]
	strh	r1, [r4, #18]	@ movhi
	ldrb	r1, [r2, #6]	@ zero_extendqisi2
	ldrb	r2, [r2, #8]	@ zero_extendqisi2
	orr	r1, r1, ip, lsl #6
	strh	r1, [r4, #20]	@ movhi
	strb	r2, [r4, #12]
	ldr	r2, .L1261+36
	ldrh	r1, [r2]
	ldrh	ip, [r2, #2]
	strh	r1, [r4, #22]	@ movhi
	ldrb	r1, [r2, #6]	@ zero_extendqisi2
	ldrb	r2, [r2, #8]	@ zero_extendqisi2
	strb	r2, [r4, #13]
	orr	r1, r1, ip, lsl #6
	ldr	r2, .L1261+40
	strh	r1, [r4, #24]	@ movhi
	movs	r1, #255
	ldr	r2, [r2]
	str	r2, [r4, #32]
	ldr	r2, [r10]
	str	r2, [r4, #40]
	ldr	r2, .L1261+44
	ldr	r2, [r2]
	str	r2, [r4, #36]
	ldrh	r2, [r3]
	bl	ftl_memset
	mov	r1, r4
	movs	r2, #48
	ldr	r0, [r6, #8]
	movw	r4, #65535
	bl	ftl_memcpy
	ldr	r2, .L1261+48
	ldr	r0, [r6, #8]
	ldr	r1, .L1261+52
	ldrh	r2, [r2]
	adds	r0, r0, #48
	ldr	r1, [r1]
	lsls	r2, r2, #1
	bl	ftl_memcpy
	mov	r0, r8
	bl	FtlUpdateVaildLpn
	ldr	r3, .L1261+24
	str	r3, [sp, #4]
.L1247:
	ldr	r3, [sp]
	ldrh	r1, [r5, #2]
	ldrh	r2, [r5]
	ldr	r3, [r3]
	str	r3, [r6, #8]
	ldr	r3, [fp]
	str	r3, [r6, #12]
	orr	r3, r1, r2, lsl #10
	str	r3, [r6, #4]
	ldr	r3, .L1261+56
	ldrh	r3, [r3]
	subs	r3, r3, #1
	cmp	r1, r3
	blt	.L1248
	movs	r3, #0
	ldrh	r4, [r5, #4]
	strh	r3, [r5, #2]	@ movhi
	strh	r2, [r5, #4]	@ movhi
	bl	FtlFreeSysBlkQueueOut
	ldr	r3, [r10]
	strh	r0, [r5]	@ movhi
	adds	r2, r3, #1
	str	r3, [r5, #8]
	str	r2, [r10]
	lsls	r2, r0, #10
	str	r2, [r6, #4]
	str	r3, [r7, #4]
	strh	r0, [r7, #2]	@ movhi
.L1248:
	ldr	r3, [sp, #4]
	ldrh	r1, [r3]
	ldr	r3, [sp]
	ldr	r0, [r3]
	bl	js_hash
	movs	r3, #1
	str	r0, [r7, #12]
	mov	r2, r3
	mov	r1, r3
	ldr	r0, .L1261+8
	bl	FlashProgPages
	ldrh	r3, [r5, #2]
	ldr	r2, [r6]
	adds	r3, r3, #1
	uxth	r3, r3
	adds	r1, r2, #1
	strh	r3, [r5, #2]	@ movhi
	bne	.L1249
	cmp	r3, #1
	bne	.L1250
	movw	r2, #1145
	ldr	r1, .L1261+60
	ldr	r0, .L1261+64
	bl	printf
.L1250:
	ldrh	r3, [r5, #2]
	add	r8, r8, #1
	uxth	r8, r8
	cmp	r3, #1
	itttt	eq
	ldreq	r3, .L1261+56
	ldrheq	r3, [r3]
	addeq	r3, r3, #-1
	strheq	r3, [r5, #2]	@ movhi
	cmp	r8, #3
	bls	.L1247
	mov	r2, r8
	ldr	r1, [r6, #4]
	ldr	r0, .L1261+68
	bl	printf
.L1253:
	b	.L1253
.L1249:
	cmp	r3, #1
	beq	.L1247
	cmp	r2, #256
	beq	.L1247
	movw	r3, #65535
	cmp	r4, r3
	beq	.L1254
	movs	r1, #1
	mov	r0, r4
	bl	FtlFreeSysBlkQueueIn
.L1254:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1262:
	.align	2
.L1261:
	.word	.LANCHOR105
	.word	.LANCHOR138
	.word	.LANCHOR137
	.word	.LANCHOR38
	.word	1179929683
	.word	.LANCHOR9
	.word	.LANCHOR22
	.word	.LANCHOR50
	.word	.LANCHOR51
	.word	.LANCHOR52
	.word	.LANCHOR89
	.word	.LANCHOR82
	.word	.LANCHOR4
	.word	.LANCHOR41
	.word	.LANCHOR19
	.word	.LANCHOR164
	.word	.LC1
	.word	.LC39
	.word	.LANCHOR113
	.word	1342177367
	.word	.LANCHOR81
	.size	FtlVpcTblFlush, .-FtlVpcTblFlush
	.section	.text.FtlSysFlush,"ax",%progbits
	.align	1
	.global	FtlSysFlush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlSysFlush, %function
FtlSysFlush:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	l2p_flush
	movs	r0, #1
	bl	FtlEctTblFlush
	bl	FtlVpcTblFlush
	movs	r0, #0
	pop	{r3, pc}
	.size	FtlSysFlush, .-FtlSysFlush
	.section	.text.sftl_deinit,"ax",%progbits
	.align	1
	.global	sftl_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sftl_deinit, %function
sftl_deinit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r3, .L1266
	ldr	r3, [r3]
	cmp	r3, #1
	bne	.L1265
	bl	FtlSysFlush
.L1265:
	movs	r0, #0
	pop	{r3, pc}
.L1267:
	.align	2
.L1266:
	.word	.LANCHOR165
	.size	sftl_deinit, .-sftl_deinit
	.section	.text.FtlDiscard,"ax",%progbits
	.align	1
	.global	FtlDiscard
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlDiscard, %function
FtlDiscard:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L1285
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	mov	r4, r1
	ldr	r3, [r3]
	cmp	r3, r0
	bls	.L1278
	cmp	r3, r1
	bcc	.L1278
	adds	r2, r0, r1
	cmp	r3, r2
	bcc	.L1278
	cmp	r1, #31
	bhi	.L1270
.L1275:
	movs	r0, #0
.L1268:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L1270:
	ldr	r8, .L1285+12
	ldrh	r5, [r8]
	mov	r1, r5
	bl	__aeabi_uidiv
	smulbb	r3, r0, r5
	mov	r7, r0
	subs	r6, r6, r3
	uxth	r6, r6
	cbz	r6, .L1271
	subs	r5, r5, r6
	adds	r7, r7, #1
	cmp	r5, r4
	it	cs
	movcs	r5, r4
	uxth	r5, r5
	subs	r4, r4, r5
.L1271:
	ldr	r5, .L1285+4
	mov	r3, #-1
	ldr	r6, .L1285+8
	str	r3, [sp, #4]
.L1272:
	ldrh	r3, [r8]
	cmp	r4, r3
	bcs	.L1274
	ldr	r3, .L1285+4
	ldr	r2, [r3]
	cmp	r2, #32
	bls	.L1275
	movs	r4, #0
	str	r4, [r3]
	bl	l2p_flush
	bl	FtlVpcTblFlush
	b	.L1275
.L1274:
	movs	r2, #0
	mov	r1, sp
	mov	r0, r7
	bl	log2phys
	ldr	r3, [sp]
	adds	r3, r3, #1
	beq	.L1273
	ldr	r3, [r5]
	movs	r2, #1
	add	r1, sp, #4
	mov	r0, r7
	adds	r3, r3, #1
	str	r3, [r5]
	ldr	r3, [r6]
	adds	r3, r3, #1
	str	r3, [r6]
	bl	log2phys
	ldr	r0, [sp]
	ubfx	r0, r0, #10, #16
	bl	P2V_block_in_plane
	bl	decrement_vpc_count
.L1273:
	ldrh	r3, [r8]
	adds	r7, r7, #1
	subs	r4, r4, r3
	b	.L1272
.L1278:
	mov	r0, #-1
	b	.L1268
.L1286:
	.align	2
.L1285:
	.word	.LANCHOR33
	.word	.LANCHOR166
	.word	.LANCHOR85
	.word	.LANCHOR11
	.size	FtlDiscard, .-FtlDiscard
	.section	.text.allocate_new_data_superblock,"ax",%progbits
	.align	1
	.global	allocate_new_data_superblock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	allocate_new_data_superblock, %function
allocate_new_data_superblock:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L1297
	push	{r4, r5, r6, lr}
	mov	r6, r0
	ldrh	r4, [r0]
	ldrh	r3, [r3]
	cmp	r3, r4
	bcs	.L1288
	movw	r2, #2766
	ldr	r1, .L1297+4
	ldr	r0, .L1297+8
	bl	printf
.L1288:
	movw	r3, #65535
	cmp	r4, r3
	beq	.L1289
	ldr	r3, .L1297+12
	mov	r0, r4
	ldr	r3, [r3]
	ldrh	r3, [r3, r4, lsl #1]
	cbz	r3, .L1290
	bl	INSERT_DATA_LIST
.L1289:
	ldr	r5, .L1297+16
	movw	r2, #65535
	movs	r3, #1
	strb	r3, [r6, #8]
	ldrh	r0, [r5]
	cmp	r0, r2
	beq	.L1291
	cmp	r4, r0
	bne	.L1292
	ldr	r3, .L1297+12
	ldr	r3, [r3]
	ldrh	r3, [r3, r0, lsl #1]
	cbz	r3, .L1293
.L1292:
	bl	update_vpc_list
.L1293:
	movw	r3, #65535
	strh	r3, [r5]	@ movhi
.L1291:
	mov	r0, r6
	bl	allocate_data_superblock
	bl	l2p_flush
	movs	r0, #0
	bl	FtlEctTblFlush
	bl	FtlVpcTblFlush
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L1290:
	bl	INSERT_FREE_LIST
	b	.L1289
.L1298:
	.align	2
.L1297:
	.word	.LANCHOR4
	.word	.LANCHOR167
	.word	.LC1
	.word	.LANCHOR41
	.word	.LANCHOR130
	.size	allocate_new_data_superblock, .-allocate_new_data_superblock
	.section	.text.FtlProgPages,"ax",%progbits
	.align	1
	.global	FtlProgPages
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlProgPages, %function
FtlProgPages:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r6, #0
	ldr	r10, .L1318+20
	mov	r5, r3
	movs	r2, #0
	mov	r4, r0
	ldr	fp, .L1318
	mov	r8, r1
	ldrb	r3, [r3, #9]	@ zero_extendqisi2
	bl	FlashProgPages
.L1300:
	cmp	r6, r8
	bne	.L1307
	ldr	r3, .L1318
	ldrb	r2, [r5, #6]	@ zero_extendqisi2
	ldrh	r3, [r3]
	cmp	r2, r3
	bcc	.L1299
	mov	r2, #1000
	ldr	r1, .L1318+4
	ldr	r0, .L1318+8
	bl	printf
.L1299:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1302:
	ldr	r1, [r4, #4]
	mov	r0, r10
	bl	printf
	ldr	r0, [r4, #4]
	ubfx	r0, r0, #10, #16
	bl	P2V_block_in_plane
	bl	decrement_vpc_count
	ldrh	r3, [r5, #4]
	cbnz	r3, .L1301
	mov	r0, r5
	bl	allocate_new_data_superblock
.L1301:
	mov	r0, r5
	bl	get_new_active_ppa
	movs	r2, #0
	str	r0, [r4, #4]
	str	r0, [sp, #12]
	movs	r1, #1
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	mov	r0, r4
	bl	FlashProgPages
.L1307:
	ldr	r2, [r4]
	adds	r3, r2, #1
	beq	.L1302
	cmp	r2, #256
	beq	.L1302
	ldrb	r2, [r5, #6]	@ zero_extendqisi2
	ldrh	r3, [fp]
	cmp	r2, r3
	bcc	.L1303
	movw	r2, #985
	ldr	r1, .L1318+4
	ldr	r0, .L1318+8
	bl	printf
.L1303:
	ldr	r3, [r4, #4]
	add	r1, sp, #16
	movs	r2, #1
	ldr	r0, [r4, #16]
	str	r3, [r1, #-4]!
	bl	log2phys
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #12]
	ubfx	r0, r3, #10, #16
	str	r3, [sp, #4]
	bl	P2V_block_in_plane
	ldr	r3, [sp, #4]
	mov	r7, r0
	adds	r3, r3, #1
	beq	.L1304
	ldr	r3, .L1318+12
	ldr	r3, [r3]
	ldrh	r2, [r3, r0, lsl #1]
	cbnz	r2, .L1305
	mov	r1, r0
	ldr	r0, .L1318+16
	bl	printf
.L1305:
	mov	r0, r7
	bl	decrement_vpc_count
.L1304:
	adds	r6, r6, #1
	adds	r4, r4, #20
	b	.L1300
.L1319:
	.align	2
.L1318:
	.word	.LANCHOR2
	.word	.LANCHOR168
	.word	.LC1
	.word	.LANCHOR41
	.word	.LC41
	.word	.LC40
	.size	FtlProgPages, .-FtlProgPages
	.section	.text.FtlGcFreeTempBlock,"ax",%progbits
	.align	1
	.global	FtlGcFreeTempBlock
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcFreeTempBlock, %function
FtlGcFreeTempBlock:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	movw	r3, #65535
	ldr	r4, .L1335
	ldrh	r2, [r4]
	cmp	r2, r3
	beq	.L1321
	ldr	r3, .L1335+4
	mov	r0, r4
	ldrh	r1, [r3]
	bl	FtlGcScanTempBlk
	str	r0, [sp, #12]
.L1321:
	ldr	r3, .L1335+8
	movs	r2, #0
	str	r2, [r3]
	movw	r3, #65535
	ldrh	r2, [r4]
	cmp	r2, r3
	beq	.L1323
	ldr	r6, .L1335+4
	ldrb	r0, [r4, #7]	@ zero_extendqisi2
	ldr	r5, .L1335+12
	ldrh	r2, [r6]
	ldrh	r1, [r5]
	muls	r2, r0, r2
	cmp	r1, r2
	beq	.L1324
	movs	r2, #164
	ldr	r1, .L1335+16
	ldr	r0, .L1335+20
	bl	printf
.L1324:
	ldrh	r6, [r6]
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldr	r2, .L1335+24
	ldrh	r0, [r4]
	ldr	fp, .L1335+56
	smulbb	r3, r3, r6
	ldr	r1, [r2]
	movs	r6, #0
	mov	r10, r2
	strh	r3, [r1, r0, lsl #1]	@ movhi
	ldr	r1, .L1335+28
	ldrh	r3, [r5]
	ldr	r0, [r1]
	add	r3, r3, r0
	str	r3, [r1]
.L1325:
	ldrh	r2, [r5]
	uxth	r3, r6
	cmp	r2, r3
	bhi	.L1329
	movw	r0, #65535
	bl	decrement_vpc_count
	ldrh	r0, [r4]
	ldr	r3, [r10]
	ldrh	r3, [r3, r0, lsl #1]
	cmp	r3, #0
	beq	.L1330
	bl	INSERT_DATA_LIST
.L1331:
	ldr	r2, .L1335+32
	movs	r3, #0
	movw	r6, #65535
	strh	r3, [r5]	@ movhi
	strh	r6, [r4]	@ movhi
	strh	r3, [r2]	@ movhi
	bl	l2p_flush
	bl	FtlVpcTblFlush
	ldr	r3, .L1335+36
	strh	r6, [r3]	@ movhi
	ldr	r3, .L1335+40
	ldrh	r2, [r3]
	ldr	r3, .L1335+44
	ldrh	r3, [r3]
	add	r3, r3, r3, lsl #1
	cmp	r2, r3, asr #2
	ittt	gt
	ldrgt	r3, .L1335+48
	movgt	r2, #20
	strhgt	r2, [r3]	@ movhi
.L1323:
	movs	r0, #0
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1329:
	uxth	r3, r6
	mov	r8, #12
	ldr	r2, [fp]
	mul	r8, r8, r3
	ldr	r3, .L1335+52
	ldr	r3, [r3]
	add	r7, r3, r8
	ldr	r0, [r7, #8]
	cmp	r0, r2
	bcc	.L1326
.L1333:
	ldrh	r0, [r4]
	b	.L1334
.L1326:
	movs	r2, #0
	add	r1, sp, #12
	str	r3, [sp, #4]
	bl	log2phys
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #12]
	ldr	r0, [r3, r8]
	cmp	r0, r2
	bne	.L1328
	ubfx	r0, r0, #10, #16
	bl	P2V_block_in_plane
	movs	r2, #1
	mov	r8, r0
	adds	r1, r7, #4
	ldr	r0, [r7, #8]
	bl	log2phys
	mov	r0, r8
.L1334:
	bl	decrement_vpc_count
	b	.L1327
.L1328:
	ldr	r3, [r7, #4]
	cmp	r2, r3
	bne	.L1333
.L1327:
	adds	r6, r6, #1
	b	.L1325
.L1330:
	bl	INSERT_FREE_LIST
	b	.L1331
.L1336:
	.align	2
.L1335:
	.word	.LANCHOR52
	.word	.LANCHOR18
	.word	.LANCHOR132
	.word	.LANCHOR69
	.word	.LANCHOR169
	.word	.LC1
	.word	.LANCHOR41
	.word	.LANCHOR83
	.word	.LANCHOR68
	.word	.LANCHOR144
	.word	.LANCHOR47
	.word	.LANCHOR170
	.word	.LANCHOR99
	.word	.LANCHOR70
	.word	.LANCHOR151
	.size	FtlGcFreeTempBlock, .-FtlGcFreeTempBlock
	.section	.text.FtlGcPageRecovery,"ax",%progbits
	.align	1
	.global	FtlGcPageRecovery
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlGcPageRecovery, %function
FtlGcPageRecovery:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	ldr	r4, .L1339
	ldr	r5, .L1339+4
	ldrh	r1, [r4]
	mov	r0, r5
	bl	FtlGcScanTempBlk
	ldrh	r2, [r5, #2]
	ldrh	r3, [r4]
	cmp	r2, r3
	bcc	.L1337
	ldr	r0, .L1339+8
	bl	FtlMapBlkWriteDump_data
	movs	r0, #0
	bl	FtlGcFreeTempBlock
	ldr	r3, .L1339+12
	movs	r2, #0
	str	r2, [r3]
.L1337:
	pop	{r3, r4, r5, pc}
.L1340:
	.align	2
.L1339:
	.word	.LANCHOR18
	.word	.LANCHOR52
	.word	.LANCHOR128
	.word	.LANCHOR132
	.size	FtlGcPageRecovery, .-FtlGcPageRecovery
	.section	.text.FtlPowerLostRecovery,"ax",%progbits
	.align	1
	.global	FtlPowerLostRecovery
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlPowerLostRecovery, %function
FtlPowerLostRecovery:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movs	r4, #0
	ldr	r5, .L1342
	ldr	r3, .L1342+4
	mov	r0, r5
	str	r4, [r3]
	bl	FtlRecoverySuperblock
	mov	r0, r5
	ldr	r5, .L1342+8
	bl	FtlSlcSuperblockCheck
	mov	r0, r5
	bl	FtlRecoverySuperblock
	mov	r0, r5
	bl	FtlSlcSuperblockCheck
	bl	FtlGcPageRecovery
	movw	r0, #65535
	bl	decrement_vpc_count
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L1343:
	.align	2
.L1342:
	.word	.LANCHOR50
	.word	.LANCHOR156
	.word	.LANCHOR51
	.size	FtlPowerLostRecovery, .-FtlPowerLostRecovery
	.section	.text.Ftl_gc_temp_data_write_back,"ax",%progbits
	.align	1
	.global	Ftl_gc_temp_data_write_back
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	Ftl_gc_temp_data_write_back, %function
Ftl_gc_temp_data_write_back:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	movs	r3, #0
	ldr	r4, .L1352
	movs	r6, #0
	movs	r7, #20
	mov	r2, r3
	ldr	r5, .L1352+4
	ldr	r1, [r4]
	ldr	r0, [r5]
	bl	FlashProgPages
.L1345:
	ldr	r1, [r4]
	uxth	r3, r6
	cmp	r1, r3
	bhi	.L1348
	ldr	r0, [r5]
	bl	FtlGcBufFree
	ldr	r3, .L1352+8
	movs	r0, #0
	str	r0, [r4]
	ldrh	r3, [r3, #4]
	cbnz	r3, .L1344
	movs	r0, #1
	bl	FtlGcFreeTempBlock
	movs	r0, #1
.L1344:
	pop	{r3, r4, r5, r6, r7, pc}
.L1348:
	muls	r3, r7, r3
	ldr	r2, [r5]
	adds	r6, r6, #1
	adds	r1, r2, r3
	ldr	r2, [r2, r3]
	ldr	r0, [r1, #12]
	ldr	r1, [r1, #4]
	adds	r3, r2, #1
	it	ne
	ldrne	r2, [r0, #8]
	ldr	r0, [r0, #12]
	bl	FtlGcUpdatePage
	b	.L1345
.L1353:
	.align	2
.L1352:
	.word	.LANCHOR61
	.word	.LANCHOR103
	.word	.LANCHOR52
	.size	Ftl_gc_temp_data_write_back, .-Ftl_gc_temp_data_write_back
	.section	.text.Ftl_get_new_temp_ppa,"ax",%progbits
	.align	1
	.global	Ftl_get_new_temp_ppa
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	Ftl_get_new_temp_ppa, %function
Ftl_get_new_temp_ppa:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	movw	r3, #65535
	ldr	r4, .L1357
	ldrh	r2, [r4]
	cmp	r2, r3
	beq	.L1355
	ldrh	r3, [r4, #4]
	cbnz	r3, .L1356
.L1355:
	movs	r0, #0
	movs	r5, #0
	bl	FtlGcFreeTempBlock
	ldr	r0, .L1357
	strb	r5, [r4, #8]
	bl	allocate_data_superblock
	ldr	r3, .L1357+4
	strh	r5, [r3]	@ movhi
	ldr	r3, .L1357+8
	strh	r5, [r3]	@ movhi
	bl	l2p_flush
	mov	r0, r5
	bl	FtlEctTblFlush
	bl	FtlVpcTblFlush
.L1356:
	ldr	r0, .L1357
	pop	{r3, r4, r5, lr}
	b	get_new_active_ppa
.L1358:
	.align	2
.L1357:
	.word	.LANCHOR52
	.word	.LANCHOR68
	.word	.LANCHOR69
	.size	Ftl_get_new_temp_ppa, .-Ftl_get_new_temp_ppa
	.section	.text.rk_ftl_garbage_collect,"ax",%progbits
	.align	1
	.global	rk_ftl_garbage_collect
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rk_ftl_garbage_collect, %function
rk_ftl_garbage_collect:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L1449
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #48
	str	r0, [sp, #28]
	ldr	r0, [r3]
	cmp	r0, #0
	bne	.L1416
	ldr	r3, .L1449+4
	ldrh	r3, [r3]
	cmp	r3, #47
	bls	.L1359
	ldr	r3, .L1449+8
	movw	r4, #65535
	ldrh	r0, [r3]
	cmp	r0, r4
	beq	.L1361
	ldr	r1, .L1449+12
	ldrh	r2, [r1]
	cmp	r2, r4
	itt	eq
	strheq	r0, [r1]	@ movhi
	strheq	r2, [r3]	@ movhi
.L1361:
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bne	.L1418
	ldr	r3, .L1449+16
	ldrh	r3, [r3]
	cmp	r3, #24
	bhi	.L1419
	ldr	r2, .L1449+20
	cmp	r3, #16
	ldrh	r4, [r2]
	bls	.L1364
	lsrs	r4, r4, #5
.L1363:
	ldr	r2, .L1449+24
	ldrh	r1, [r2]
	cmp	r1, r3
	mov	r1, r2
	bcs	.L1367
	ldr	r3, .L1449+28
	movw	r0, #65535
	ldrh	r3, [r3]
	cmp	r3, r0
	bne	.L1368
	ldr	r0, .L1449+12
	ldrh	r0, [r0]
	cmp	r0, r3
	bne	.L1368
	ldr	r3, .L1449+32
	ldrh	r0, [r3]
	cbnz	r0, .L1369
	ldr	r3, .L1449+36
	ldr	r4, .L1449+40
	ldr	r3, [r3]
	ldr	r4, [r4]
	add	r3, r3, r3, lsl #1
	cmp	r4, r3, lsr #2
	bcs	.L1370
.L1369:
	ldr	r3, .L1449+44
	ldrh	r3, [r3]
	add	r3, r3, r3, lsl #1
	asrs	r3, r3, #2
	strh	r3, [r1]	@ movhi
.L1371:
	ldr	r3, .L1449+48
	movs	r2, #0
	str	r2, [r3]
.L1359:
	add	sp, sp, #48
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1364:
	cmp	r3, #12
	bls	.L1365
	lsrs	r4, r4, #4
	b	.L1363
.L1365:
	cmp	r3, #8
	bls	.L1363
	lsrs	r4, r4, #2
	b	.L1363
.L1419:
	movs	r4, #1
	b	.L1363
.L1370:
	movs	r3, #18
	strh	r3, [r2]	@ movhi
	b	.L1371
.L1368:
	ldr	r3, .L1449+44
	ldrh	r3, [r3]
	add	r3, r3, r3, lsl #1
	asrs	r3, r3, #2
	strh	r3, [r1]	@ movhi
.L1367:
	ldr	r3, .L1449+52
	ldrh	r3, [r3]
	cbz	r3, .L1362
	adds	r4, r4, #32
	uxth	r4, r4
.L1362:
	ldr	r5, .L1449+56
	movw	r3, #65535
	ldrh	r2, [r5]
	cmp	r2, r3
	bne	.L1374
	ldr	r3, .L1449+12
	ldrh	r1, [r3]
	cmp	r1, r2
	beq	.L1375
	ldr	r0, .L1449+60
	ldr	r0, [r0]
	ldrh	r1, [r0, r1, lsl #1]
	cbnz	r1, .L1376
	strh	r2, [r3]	@ movhi
.L1376:
	ldrh	r2, [r3]
	strh	r2, [r5]	@ movhi
	movw	r2, #65535
	strh	r2, [r3]	@ movhi
.L1375:
	ldrh	r0, [r5]
	movw	r6, #65535
	movs	r3, #0
	strb	r3, [r5, #8]
	cmp	r0, r6
	beq	.L1374
	bl	IsBlkInGcList
	cbz	r0, .L1378
	strh	r6, [r5]	@ movhi
.L1378:
	ldrh	r2, [r5]
	movw	r3, #65535
	cmp	r2, r3
	beq	.L1374
	ldr	r0, .L1449+56
	bl	make_superblock
	ldr	r2, .L1449+64
	movs	r3, #0
	strh	r3, [r5, #2]	@ movhi
	strb	r3, [r5, #6]
	strh	r3, [r2]	@ movhi
	ldr	r3, .L1449+60
	ldrh	r2, [r5]
	ldr	r3, [r3]
	ldrh	r2, [r3, r2, lsl #1]
	ldr	r3, .L1449+68
	strh	r2, [r3]	@ movhi
.L1374:
	ldr	r2, .L1449+72
	ldrh	r3, [r5]
	ldrh	r2, [r2]
	cmp	r2, r3
	beq	.L1379
	ldr	r2, .L1449+76
	ldrh	r2, [r2]
	cmp	r2, r3
	beq	.L1379
	ldr	r2, .L1449+28
	ldrh	r2, [r2]
	cmp	r2, r3
	bne	.L1380
.L1379:
	movw	r3, #65535
	strh	r3, [r5]	@ movhi
.L1380:
	ldr	r5, .L1449+56
	mov	r6, r5
.L1415:
	ldrh	r2, [r5]
	movw	r3, #65535
	cmp	r2, r3
	bne	.L1381
	ldr	fp, .L1449+88
	movs	r2, #0
	ldr	r3, .L1449+48
	mov	r8, fp
	str	r2, [r3]
.L1382:
	ldrh	r7, [fp]
	mov	r0, r7
	bl	List_get_gc_head_node
	uxth	r3, r0
	movw	r1, #65535
	cmp	r3, r1
	strh	r3, [r6]	@ movhi
	bne	.L1383
	movs	r3, #0
	movs	r0, #8
	strh	r3, [fp]	@ movhi
	b	.L1359
.L1418:
	movs	r4, #1
	b	.L1362
.L1383:
	str	r0, [sp, #16]
	mov	r0, r3
	str	r3, [sp, #12]
	adds	r7, r7, #1
	bl	IsBlkInGcList
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #16]
	cbz	r0, .L1384
	strh	r7, [fp]	@ movhi
	b	.L1382
.L1384:
	uxth	r0, r2
	ldr	r2, .L1449+20
	ldr	r10, .L1449+60
	uxth	r7, r7
	ldrh	lr, [r2]
	ldr	r2, .L1449+80
	ldr	r1, [r10]
	strh	r7, [fp]	@ movhi
	ldrh	r2, [r2]
	ldrh	ip, [r1, r0, lsl #1]
	mul	r2, r2, lr
	cmp	ip, r2, asr #1
	bgt	.L1386
	cmp	r7, #48
	bls	.L1387
	cmp	ip, #8
	bls	.L1387
	ldr	r7, .L1449+84
	ldrh	r7, [r7]
	cmp	r7, #35
	bhi	.L1387
.L1386:
	movs	r7, #0
	strh	r7, [r8]	@ movhi
.L1387:
	ldrh	r1, [r1, r0, lsl #1]
	cmp	r2, r1
	bgt	.L1388
	ldrh	r2, [r8]
	cmp	r2, #3
	bhi	.L1388
	movw	r3, #65535
	strh	r3, [r6]	@ movhi
	movs	r3, #0
	strh	r3, [r8]	@ movhi
.L1448:
	ldr	r3, .L1449+32
	ldrh	r0, [r3]
	b	.L1359
.L1388:
	cbnz	r1, .L1389
	movw	r0, #65535
	bl	decrement_vpc_count
	ldrh	r3, [r8]
	adds	r3, r3, #1
	strh	r3, [r8]	@ movhi
	b	.L1382
.L1450:
	.align	2
.L1449:
	.word	.LANCHOR93
	.word	.LANCHOR43
	.word	.LANCHOR72
	.word	.LANCHOR71
	.word	.LANCHOR47
	.word	.LANCHOR18
	.word	.LANCHOR99
	.word	.LANCHOR52
	.word	.LANCHOR171
	.word	.LANCHOR151
	.word	.LANCHOR58
	.word	.LANCHOR170
	.word	.LANCHOR94
	.word	.LANCHOR73
	.word	.LANCHOR144
	.word	.LANCHOR41
	.word	.LANCHOR172
	.word	.LANCHOR173
	.word	.LANCHOR50
	.word	.LANCHOR51
	.word	.LANCHOR2
	.word	.LANCHOR68
	.word	.LANCHOR101
.L1389:
	movs	r2, #0
	strb	r2, [r6, #8]
	ldr	r2, .L1451
	ldrh	r2, [r2]
	cmp	r2, r3
	bne	.L1390
	movw	r2, #717
	ldr	r1, .L1451+4
	ldr	r0, .L1451+8
	bl	printf
.L1390:
	ldr	r3, .L1451+12
	ldrh	r2, [r6]
	ldrh	r3, [r3]
	cmp	r2, r3
	bne	.L1391
	movw	r2, #718
	ldr	r1, .L1451+4
	ldr	r0, .L1451+8
	bl	printf
.L1391:
	ldr	r3, .L1451+16
	ldrh	r2, [r6]
	ldrh	r3, [r3]
	cmp	r2, r3
	bne	.L1392
	movw	r2, #719
	ldr	r1, .L1451+4
	ldr	r0, .L1451+8
	bl	printf
.L1392:
	mov	r0, r5
	bl	make_superblock
	ldr	r2, .L1451+20
	movs	r3, #0
	ldrh	r1, [r6]
	strh	r3, [r2]	@ movhi
	ldr	r2, [r10]
	ldrh	r1, [r2, r1, lsl #1]
	ldr	r2, .L1451+24
	strh	r3, [r6, #2]	@ movhi
	strb	r3, [r6, #6]
	strh	r1, [r2]	@ movhi
.L1381:
	ldr	r3, .L1451+28
	movs	r2, #1
	str	r2, [r3]
	ldr	r3, .L1451+32
	ldrh	r3, [r3]
	str	r3, [sp, #16]
	ldr	r3, [sp, #28]
	cbz	r3, .L1393
	ldr	r3, .L1451+36
	ldr	r2, [sp, #16]
	ldrh	r1, [r6]
	ldrh	r3, [r3]
	muls	r3, r2, r3
	ldr	r2, .L1451+40
	ldr	r2, [r2]
	ldrh	r2, [r2, r1, lsl #1]
	subs	r3, r3, r2
	it	mi
	addmi	r3, r3, #3
	add	r4, r4, r3, asr #2
	uxth	r4, r4
.L1393:
	ldrh	r3, [r6, #2]
	ldr	r1, [sp, #16]
	ldr	fp, .L1451+64
	adds	r2, r3, r4
	cmp	r2, r1
	itt	gt
	movgt	r2, r1
	subgt	r4, r2, r3
	mov	r3, #0
	it	gt
	uxthgt	r4, r4
	str	r3, [sp, #24]
.L1396:
	ldrh	r3, [sp, #24]
	cmp	r4, r3
	bls	.L1404
	ldr	r3, .L1451+36
	movw	r10, #65535
	ldrh	r1, [r5, #2]
	mov	ip, #20
	ldr	r0, [fp]
	ldrh	lr, [r3]
	ldr	r3, [sp, #24]
	ldr	r7, .L1451+44
	adds	r3, r1, r3
	str	r3, [sp, #20]
	movs	r3, #0
	str	r3, [sp, #12]
	b	.L1405
.L1398:
	ldrh	r2, [r7, #2]!
	cmp	r2, r10
	beq	.L1397
	ldr	r1, [sp, #12]
	mla	r8, ip, r1, r0
	ldr	r1, [sp, #20]
	orr	r2, r1, r2, lsl #10
	str	r2, [r8, #4]
	ldr	r2, [sp, #12]
	adds	r2, r2, #1
	uxth	r2, r2
	str	r2, [sp, #12]
.L1397:
	adds	r3, r3, #1
.L1405:
	uxth	r2, r3
	cmp	lr, r2
	bhi	.L1398
	ldrb	r2, [r5, #8]	@ zero_extendqisi2
	ldr	r1, [sp, #12]
	bl	FlashReadPages
	movs	r3, #0
.L1447:
	str	r3, [sp, #20]
	ldr	r2, [sp, #12]
	ldrh	r3, [sp, #20]
	cmp	r2, r3
	bhi	.L1403
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	str	r3, [sp, #24]
	b	.L1396
.L1403:
	ldr	r3, [sp, #20]
	movs	r7, #20
	muls	r7, r3, r7
	ldr	r3, [fp]
	adds	r2, r3, r7
	ldr	r3, [r3, r7]
	adds	r3, r3, #1
	beq	.L1400
	ldr	r8, [r2, #12]
	movw	r3, #61589
	ldrh	r2, [r8]
	cmp	r2, r3
	bne	.L1400
	ldr	r10, [r8, #8]
	cmp	r10, #-1
	bne	.L1401
	movw	r2, #753
	ldr	r1, .L1451+4
	ldr	r0, .L1451+8
	bl	printf
.L1401:
	movs	r2, #0
	add	r1, sp, #40
	mov	r0, r10
	bl	log2phys
	ldr	r0, [fp]
	ldr	r3, [sp, #40]
	add	r0, r0, r7
	ldr	r2, [r0, #4]
	cmp	r2, r3
	bne	.L1400
	ldr	r2, .L1451+20
	ldr	r10, .L1451+60
	ldr	r1, .L1451+48
	ldrh	r3, [r2]
	str	r1, [sp, #36]
	adds	r3, r3, #1
	strh	r3, [r2]	@ movhi
	ldr	r2, [r1]
	movs	r1, #20
	ldr	r3, [r10]
	mla	r3, r1, r3, r2
	ldr	r2, [r0, #16]
	str	r2, [r3, #16]
	str	r3, [sp, #32]
	bl	Ftl_get_new_temp_ppa
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #36]
	str	r0, [r3, #4]
	ldr	r2, [r1]
	movs	r1, #20
	ldr	r3, [r10]
	ldr	r0, [fp]
	mla	r2, r1, r3, r2
	add	r0, r0, r7
	ldr	r7, .L1451+16
	ldr	r1, [r0, #8]
	adds	r3, r3, #1
	str	r1, [r2, #8]
	ldr	r1, [r0, #12]
	str	r1, [r2, #12]
	movs	r1, #1
	ldr	r2, [sp, #40]
	str	r2, [r8, #12]
	ldrh	r2, [r7]
	strh	r2, [r8, #2]	@ movhi
	ldr	r2, .L1451+52
	str	r3, [r10]
	ldr	r2, [r2]
	str	r2, [r8, #4]
	bl	FtlGcBufAlloc
	ldrb	r2, [r7, #7]	@ zero_extendqisi2
	ldr	r3, [r10]
	cmp	r2, r3
	beq	.L1402
	ldrh	r3, [r7, #4]
	cbnz	r3, .L1400
.L1402:
	bl	Ftl_gc_temp_data_write_back
	cbz	r0, .L1400
	ldr	r3, .L1451+28
	movs	r2, #0
	movw	r1, #65535
	str	r2, [r3]
	ldr	r3, .L1451+56
	strh	r1, [r3]	@ movhi
	strh	r2, [r3, #2]	@ movhi
	b	.L1448
.L1400:
	ldr	r3, [sp, #20]
	adds	r3, r3, #1
	b	.L1447
.L1404:
	ldrh	r3, [r5, #2]
	add	r4, r4, r3
	ldr	r3, [sp, #16]
	uxth	r4, r4
	cmp	r3, r4
	strh	r4, [r5, #2]	@ movhi
	bhi	.L1406
	ldr	r3, .L1451+60
	ldr	r3, [r3]
	cbz	r3, .L1407
	bl	Ftl_gc_temp_data_write_back
	cbz	r0, .L1407
	ldr	r3, .L1451+28
	movs	r2, #0
	str	r2, [r3]
	b	.L1448
.L1452:
	.align	2
.L1451:
	.word	.LANCHOR50
	.word	.LANCHOR174
	.word	.LC1
	.word	.LANCHOR51
	.word	.LANCHOR52
	.word	.LANCHOR172
	.word	.LANCHOR173
	.word	.LANCHOR93
	.word	.LANCHOR18
	.word	.LANCHOR2
	.word	.LANCHOR41
	.word	.LANCHOR144+14
	.word	.LANCHOR103
	.word	.LANCHOR82
	.word	.LANCHOR144
	.word	.LANCHOR61
	.word	.LANCHOR65
.L1407:
	ldr	r3, .L1453
	ldrh	r4, [r3]
	cmp	r4, #0
	bne	.L1408
	ldr	r8, .L1453+28
	ldrh	r1, [r5]
	ldr	r3, [r8]
	ldrh	r3, [r3, r1, lsl #1]
	cmp	r3, #0
	beq	.L1408
	ldr	r0, .L1453+4
	ldr	r10, .L1453+32
	ldrh	r2, [r5, #2]
	ldrh	r0, [r0]
	str	r3, [sp]
	mov	r3, r4
	str	r0, [sp, #4]
	ldr	r0, .L1453+8
	bl	printf
.L1409:
	ldr	r3, [r10]
	cmp	r4, r3
	bcs	.L1411
	movs	r2, #0
	add	r1, sp, #44
	mov	r0, r4
	bl	log2phys
	ldr	r7, [sp, #44]
	adds	r3, r7, #1
	beq	.L1410
	ubfx	r0, r7, #10, #16
	bl	P2V_block_in_plane
	ldrh	r3, [r5]
	cmp	r3, r0
	bne	.L1410
	mov	r2, r7
	mov	r1, r4
	ldr	r0, .L1453+12
	bl	printf
.L1411:
	ldr	r3, [r10]
	cmp	r4, r3
	bcc	.L1408
	ldrh	r2, [r5]
	movs	r1, #0
	ldr	r3, [r8]
	strh	r1, [r3, r2, lsl #1]	@ movhi
	ldrh	r0, [r5]
	bl	update_vpc_list
	bl	l2p_flush
	bl	FtlVpcTblFlush
.L1408:
	movw	r3, #65535
	strh	r3, [r5]	@ movhi
.L1406:
	ldr	r3, .L1453+16
	movs	r2, #0
	str	r2, [r3]
	ldr	r3, .L1453+20
	ldrh	r0, [r3]
	cmp	r0, #2
	bhi	.L1414
	ldr	r3, .L1453+24
	ldrh	r4, [r3]
	b	.L1415
.L1410:
	adds	r4, r4, #1
	b	.L1409
.L1414:
	adds	r0, r0, #1
	b	.L1359
.L1416:
	movs	r0, #0
	b	.L1359
.L1454:
	.align	2
.L1453:
	.word	.LANCHOR172
	.word	.LANCHOR173
	.word	.LC42
	.word	.LC43
	.word	.LANCHOR93
	.word	.LANCHOR47
	.word	.LANCHOR18
	.word	.LANCHOR41
	.word	.LANCHOR151
	.size	rk_ftl_garbage_collect, .-rk_ftl_garbage_collect
	.section	.text.FtlRead,"ax",%progbits
	.align	1
	.global	FtlRead
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlRead, %function
FtlRead:
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #16
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #56
	mov	r5, r1
	mov	r10, r3
	str	r2, [sp, #4]
	bne	.L1456
	mov	r2, r3
	ldr	r1, [sp, #4]
	add	r0, r5, #256
	bl	FtlVendorPartRead
	mov	r4, r0
.L1455:
	mov	r0, r4
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1456:
	ldr	r3, .L1490
	ldr	r3, [r3]
	cmp	r1, r3
	bcs	.L1479
	ldr	r2, [sp, #4]
	cmp	r2, r3
	bhi	.L1479
	adds	r2, r1, r2
	cmp	r3, r2
	str	r2, [sp, #12]
	bcc	.L1479
	ldr	r3, .L1490+4
	ldr	r4, [r3]
	adds	r6, r4, #1
	beq	.L1455
	ldr	r3, .L1490+8
	mov	r0, r5
	mov	r8, #0
	mov	r7, r8
	ldrh	r4, [r3]
	mov	r1, r4
	bl	__aeabi_uidiv
	ldr	r3, [sp, #12]
	mov	r1, r4
	str	r0, [sp, #16]
	mov	r4, r8
	subs	r0, r3, #1
	bl	__aeabi_uidiv
	ldr	r3, [sp, #16]
	ldr	r2, .L1490+12
	ldr	r1, [sp, #4]
	rsb	r3, r3, #1
	ldr	r6, [sp, #16]
	add	r3, r3, r0
	str	r0, [sp, #20]
	str	r3, [sp, #8]
	ldr	r3, [r2]
	str	r8, [sp, #28]
	str	r8, [sp, #24]
	add	r3, r3, r1
	ldr	r1, [sp, #8]
	str	r3, [r2]
	ldr	r2, .L1490+16
	ldr	r3, [r2]
	add	r3, r3, r1
	str	r3, [r2]
.L1458:
	ldr	r3, [sp, #8]
	cbnz	r3, .L1475
	ldr	r3, .L1490+20
	ldrh	r3, [r3]
	cbnz	r3, .L1476
	ldr	r3, .L1490+24
	ldrh	r3, [r3]
	cmp	r3, #31
	bhi	.L1455
.L1476:
	movs	r1, #1
	movs	r0, #0
	bl	rk_ftl_garbage_collect
	b	.L1455
.L1475:
	movs	r2, #0
	add	r1, sp, #52
	mov	r0, r6
	bl	log2phys
	ldr	r3, [sp, #52]
	adds	r0, r3, #1
	bne	.L1459
	ldr	r3, .L1490+8
	mov	fp, #0
.L1460:
	ldrh	r0, [r3]
	cmp	fp, r0
	bcc	.L1462
.L1463:
	ldr	r3, [sp, #8]
	adds	r6, r6, #1
	subs	r3, r3, #1
	str	r3, [sp, #8]
	beq	.L1467
	ldr	r3, .L1490+28
	ldrh	r3, [r3]
	cmp	r7, r3, lsl #2
	bne	.L1458
.L1467:
	cmp	r7, #0
	beq	.L1458
	ldr	r3, .L1490+32
	movs	r2, #0
	mov	r1, r7
	mov	fp, #0
	ldr	r0, [r3]
	bl	FlashReadPages
	lsl	r3, r8, #9
	str	r3, [sp, #44]
	ldr	r3, [sp, #24]
	lsls	r3, r3, #9
	str	r3, [sp, #36]
	ldr	r3, [sp, #28]
	lsls	r3, r3, #9
	str	r3, [sp, #40]
.L1474:
	movs	r3, #20
	mul	r3, r3, fp
	str	r3, [sp, #32]
	ldr	r3, .L1490+32
	ldr	r2, [r3]
	ldr	r3, [sp, #32]
	add	r2, r2, r3
	ldr	r3, [sp, #16]
	ldr	r1, [r2, #16]
	cmp	r3, r1
	bne	.L1469
	ldr	r1, [r2, #8]
	ldr	r2, .L1490+36
	ldr	r2, [r2]
	cmp	r1, r2
	bne	.L1470
	ldr	r3, [sp, #36]
	mov	r0, r10
	ldr	r2, [sp, #40]
	add	r1, r1, r3
.L1489:
	bl	ftl_memcpy
.L1470:
	ldr	r3, .L1490+32
	ldr	r2, [sp, #32]
	ldr	r3, [r3]
	adds	r2, r3, r2
	ldr	r0, [r2, #12]
	ldr	r1, [r2, #16]
	ldr	r0, [r0, #8]
	cmp	r1, r0
	itttt	ne
	ldrne	r0, .L1490+40
	ldrne	r1, [r0, #72]
	addne	r1, r1, #1
	strne	r1, [r0, #72]
	ldr	r1, [sp, #32]
	ldr	r3, [r3, r1]
	adds	r1, r3, #1
	bne	.L1472
	ldr	r1, .L1490+40
	mov	r4, r3
	ldr	r2, [r1, #72]
	adds	r2, r2, #1
	str	r2, [r1, #72]
.L1473:
	add	fp, fp, #1
	cmp	r7, fp
	bne	.L1474
	movs	r7, #0
	b	.L1458
.L1462:
	mla	r0, r0, r6, fp
	cmp	r5, r0
	bhi	.L1461
	ldr	r2, [sp, #12]
	cmp	r2, r0
	bls	.L1461
	subs	r0, r0, r5
	mov	r2, #512
	movs	r1, #0
	add	r0, r10, r0, lsl #9
	str	r3, [sp, #32]
	bl	ftl_memset
	ldr	r3, [sp, #32]
.L1461:
	add	fp, fp, #1
	b	.L1460
.L1459:
	ldr	r2, .L1490+32
	mov	fp, #20
	ldr	r2, [r2]
	mla	fp, fp, r7, r2
	str	r3, [fp, #4]
	ldr	r3, [sp, #16]
	cmp	r6, r3
	ldr	r3, .L1490+8
	bne	.L1464
	ldr	r2, .L1490+36
	mov	r0, r5
	ldrh	r3, [r3]
	ldr	r2, [r2]
	mov	r1, r3
	str	r3, [sp, #28]
	str	r2, [fp, #8]
	bl	__aeabi_uidivmod
	ldr	r3, [sp, #28]
	str	r1, [sp, #24]
	subs	r2, r3, r1
	ldr	r1, [sp, #4]
	cmp	r2, r1
	it	cs
	movcs	r2, r1
	cmp	r3, r2
	str	r2, [sp, #28]
	bne	.L1465
	str	r10, [fp, #8]
.L1465:
	ldr	r3, .L1490+44
	ldr	r2, .L1490+48
	str	r6, [fp, #16]
	ldrh	r3, [r3]
	ldr	r2, [r2]
	muls	r3, r7, r3
	adds	r7, r7, #1
	bic	r3, r3, #3
	add	r3, r3, r2
	str	r3, [fp, #12]
	b	.L1463
.L1464:
	ldr	r2, [sp, #20]
	cmp	r6, r2
	bne	.L1466
	ldr	r2, .L1490+52
	ldr	r1, [sp, #12]
	ldr	r2, [r2]
	str	r2, [fp, #8]
	ldrh	r2, [r3]
	mul	r3, r2, r6
	sub	r8, r1, r3
	cmp	r2, r8
	bne	.L1465
.L1488:
	subs	r3, r3, r5
	add	r3, r10, r3, lsl #9
	str	r3, [fp, #8]
	b	.L1465
.L1466:
	ldrh	r3, [r3]
	muls	r3, r6, r3
	b	.L1488
.L1469:
	ldr	r3, [sp, #20]
	cmp	r3, r1
	bne	.L1470
	ldr	r3, .L1490+52
	ldr	r1, [r2, #8]
	ldr	r2, [r3]
	cmp	r1, r2
	bne	.L1470
	ldr	r2, .L1490+8
	ldr	r3, [sp, #20]
	ldrh	r0, [r2]
	ldr	r2, [sp, #44]
	muls	r0, r3, r0
	subs	r0, r0, r5
	add	r0, r10, r0, lsl #9
	b	.L1489
.L1472:
	cmp	r3, #256
	bne	.L1473
	ldr	r0, [r2, #4]
	ubfx	r0, r0, #10, #16
	bl	P2V_block_in_plane
	bl	FtlGcRefreshBlock
	b	.L1473
.L1479:
	mov	r4, #-1
	b	.L1455
.L1491:
	.align	2
.L1490:
	.word	.LANCHOR33
	.word	.LANCHOR165
	.word	.LANCHOR11
	.word	.LANCHOR162
	.word	.LANCHOR88
	.word	.LANCHOR73
	.word	.LANCHOR47
	.word	.LANCHOR2
	.word	.LANCHOR102
	.word	.LANCHOR108
	.word	.LANCHOR134
	.word	.LANCHOR23
	.word	.LANCHOR114
	.word	.LANCHOR109
	.size	FtlRead, .-FtlRead
	.section	.text.sftl_read,"ax",%progbits
	.align	1
	.global	sftl_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sftl_read, %function
sftl_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r2
	mov	r2, r1
	mov	r1, r0
	movs	r0, #0
	b	FtlRead
	.size	sftl_read, .-sftl_read
	.section	.text.FtlWrite,"ax",%progbits
	.align	1
	.global	FtlWrite
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlWrite, %function
FtlWrite:
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #16
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #72
	mov	r10, r1
	str	r2, [sp, #16]
	str	r3, [sp, #20]
	bne	.L1494
	mov	r2, r3
	ldr	r1, [sp, #16]
	add	r0, r10, #256
	bl	FtlVendorPartWrite
.L1493:
	add	sp, sp, #72
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1494:
	ldr	r3, .L1540
	ldr	r3, [r3]
	cmp	r1, r3
	bcs	.L1529
	ldr	r2, [sp, #16]
	cmp	r2, r3
	bhi	.L1529
	adds	r4, r1, r2
	cmp	r3, r4
	bcc	.L1529
	ldr	r3, .L1540+4
	ldr	r0, [r3]
	adds	r1, r0, #1
	beq	.L1493
	ldr	r3, .L1540+8
	mov	r2, #2048
	mov	r0, r10
	ldr	r7, .L1540+12
	str	r2, [r3]
	ldr	r3, .L1540+16
	ldrh	r5, [r3]
	mov	r1, r5
	bl	__aeabi_uidiv
	mov	r1, r5
	str	r0, [sp, #12]
	subs	r0, r4, #1
	bl	__aeabi_uidiv
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #16]
	ldr	r5, [sp, #12]
	subs	r3, r0, r2
	ldr	r2, .L1540+20
	str	r3, [sp, #32]
	adds	r3, r3, #1
	str	r3, [sp, #8]
	ldr	r3, [r2]
	str	r0, [sp, #28]
	add	r3, r3, r1
	ldr	r1, [sp, #8]
	str	r3, [r2]
	ldr	r2, .L1540+24
	ldr	r3, [r2]
	add	r3, r3, r1
	str	r3, [r2]
.L1496:
	ldr	r3, [sp, #8]
	cbnz	r3, .L1522
	ldr	r5, .L1540+28
	mov	r0, r3
	ldr	r1, [sp, #32]
	bl	rk_ftl_garbage_collect
	ldrh	r2, [r5]
	cmp	r2, #5
	bls	.L1523
.L1525:
	movs	r0, #0
	b	.L1493
.L1522:
	ldr	r4, .L1540+32
	ldrb	r2, [r7, #6]	@ zero_extendqisi2
	ldrh	r3, [r4]
	cmp	r2, r3
	bcc	.L1497
	movw	r2, #1041
	ldr	r1, .L1540+36
	ldr	r0, .L1540+40
	bl	printf
.L1497:
	ldrh	r3, [r7, #4]
	cbnz	r3, .L1498
	mov	r0, r7
	bl	allocate_new_data_superblock
.L1498:
	ldrb	r3, [r7, #7]	@ zero_extendqisi2
	ldrh	r2, [r7, #4]
	lsls	r3, r3, #2
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	ldr	r2, [sp, #8]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	ldrb	r2, [r7, #6]	@ zero_extendqisi2
	str	r3, [sp, #44]
	ldrh	r3, [r4]
	cmp	r2, r3
	bcc	.L1499
	movw	r2, #1074
	ldr	r1, .L1540+36
	ldr	r0, .L1540+40
	bl	printf
.L1499:
	ldr	r8, .L1540+44
	movs	r3, #0
	str	r3, [sp, #4]
.L1500:
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #44]
	cmp	r3, r2
	bne	.L1520
.L1501:
	ldr	r0, .L1540+44
	mov	r3, r7
	movs	r2, #0
	ldr	r1, [sp, #4]
	ldr	r0, [r0]
	bl	FtlProgPages
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #8]
	cmp	r3, r2
	bls	.L1521
	mov	r2, #1152
	ldr	r1, .L1540+36
	ldr	r0, .L1540+40
	bl	printf
.L1521:
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #4]
	subs	r3, r3, r2
	str	r3, [sp, #8]
	b	.L1496
.L1520:
	ldrh	r3, [r7, #4]
	cmp	r3, #0
	beq	.L1501
	movs	r2, #0
	add	r1, sp, #48
	mov	r0, r5
	movs	r6, #20
	bl	log2phys
	mov	r0, r7
	bl	get_new_active_ppa
	ldr	r2, .L1540+48
	ldr	r1, [sp, #4]
	ldr	r3, [sp, #4]
	ldrh	r2, [r2]
	muls	r6, r3, r6
	ldr	r3, [r8]
	muls	r1, r2, r1
	add	r3, r3, r6
	str	r0, [r3, #4]
	bic	r1, r1, #3
	str	r5, [r3, #16]
	str	r1, [sp, #36]
	ldr	r1, .L1540+52
	ldr	r0, [sp, #36]
	ldr	r1, [r1]
	add	fp, r1, r0
	str	r1, [sp, #40]
	str	fp, [r3, #12]
	movs	r1, #0
	mov	r0, fp
	bl	ftl_memset
	ldr	r3, [sp, #12]
	ldr	r2, .L1540+16
	cmp	r5, r3
	beq	.L1502
	ldr	r3, [sp, #28]
	cmp	r5, r3
	bne	.L1535
	ldrh	r2, [r2]
	ldr	r3, [sp, #16]
	smulbb	r2, r2, r5
	add	r4, r10, r3
	movs	r3, #0
	str	r3, [sp, #24]
	subs	r4, r4, r2
	uxth	r4, r4
	b	.L1505
.L1502:
	ldrh	r4, [r2]
	mov	r0, r10
	mov	r1, r4
	bl	__aeabi_uidivmod
	ldr	r3, [sp, #16]
	subs	r4, r4, r1
	str	r1, [sp, #24]
	cmp	r4, r3
	it	cs
	movcs	r4, r3
.L1505:
	ldr	r3, .L1540+16
	ldrh	r3, [r3]
	cmp	r4, r3
	ldr	r3, [sp, #12]
	bne	.L1506
	cmp	r5, r3
	ldr	r3, [r8]
	add	r6, r6, r3
	bne	.L1507
	ldr	r3, [sp, #20]
.L1537:
	str	r3, [r6, #8]
	b	.L1508
.L1507:
	muls	r4, r5, r4
	ldr	r3, [sp, #20]
	sub	r4, r4, r10
	add	r4, r3, r4, lsl #9
	str	r4, [r6, #8]
.L1508:
	ldr	r3, .L1540+32
	ldrb	r2, [r7, #6]	@ zero_extendqisi2
	ldrh	r3, [r3]
	cmp	r2, r3
	bcc	.L1517
	movw	r2, #1143
	ldr	r1, .L1540+36
	ldr	r0, .L1540+40
	bl	printf
.L1517:
	ldr	r2, [sp, #40]
	movw	r3, #61589
	ldr	r1, [sp, #36]
	strh	r3, [r2, r1]	@ movhi
	ldr	r2, .L1540+56
	str	r5, [fp, #8]
	adds	r5, r5, #1
	ldr	r3, [r2]
	str	r3, [fp, #4]
	adds	r3, r3, #1
	adds	r1, r3, #1
	it	eq
	moveq	r3, #0
	str	r3, [r2]
	ldr	r3, [sp, #48]
	str	r3, [fp, #12]
	ldrh	r3, [r7]
	strh	r3, [fp, #2]	@ movhi
	ldr	r3, [sp, #4]
	adds	r3, r3, #1
	str	r3, [sp, #4]
	b	.L1500
.L1506:
	cmp	r5, r3
	ldr	r3, [r8]
	ite	eq
	ldreq	r2, .L1540+60
	ldrne	r2, .L1540+64
	add	r3, r3, r6
	ldr	r2, [r2]
	str	r2, [r3, #8]
	ldr	r3, [sp, #48]
	adds	r2, r3, #1
	beq	.L1511
	str	r3, [sp, #56]
	movs	r1, #1
	ldr	r3, [r8]
	add	r0, sp, #52
	str	r5, [sp, #68]
	add	r3, r3, r6
	ldr	r2, [r3, #8]
	ldr	r3, [r3, #12]
	str	r2, [sp, #60]
	movs	r2, #0
	str	r3, [sp, #64]
	bl	FlashReadPages
	ldr	r3, [sp, #52]
	adds	r0, r3, #1
	bne	.L1512
	ldr	r1, .L1540+68
	ldr	r0, .L1540+72
	ldr	r2, [r1, #72]
	adds	r2, r2, #1
	str	r2, [r1, #72]
	mov	r2, r5
	ldr	r1, [fp, #8]
	bl	printf
.L1515:
	ldr	r3, [sp, #12]
	lsls	r2, r4, #9
	cmp	r5, r3
	bne	.L1516
	ldr	r3, [r8]
	ldr	r1, [sp, #20]
	add	r6, r6, r3
	ldr	r3, [sp, #24]
	ldr	r0, [r6, #8]
	add	r0, r0, r3, lsl #9
.L1538:
	bl	ftl_memcpy
	b	.L1508
.L1512:
	ldr	r1, [fp, #8]
	cmp	r5, r1
	beq	.L1514
	ldr	r2, .L1540+68
	ldr	r0, .L1540+76
	ldr	r3, [r2, #72]
	adds	r3, r3, #1
	str	r3, [r2, #72]
	mov	r2, r5
	bl	printf
.L1514:
	ldr	r3, [fp, #8]
	cmp	r5, r3
	beq	.L1515
	mov	r2, #1128
	ldr	r1, .L1540+36
	ldr	r0, .L1540+40
	bl	printf
	b	.L1515
.L1541:
	.align	2
.L1540:
	.word	.LANCHOR33
	.word	.LANCHOR165
	.word	.LANCHOR175
	.word	.LANCHOR50
	.word	.LANCHOR11
	.word	.LANCHOR161
	.word	.LANCHOR84
	.word	.LANCHOR47
	.word	.LANCHOR2
	.word	.LANCHOR176
	.word	.LC1
	.word	.LANCHOR104
	.word	.LANCHOR23
	.word	.LANCHOR114
	.word	.LANCHOR82
	.word	.LANCHOR108
	.word	.LANCHOR109
	.word	.LANCHOR134
	.word	.LC44
	.word	.LC45
.L1511:
	ldr	r3, [r8]
	movs	r1, #0
	ldr	r2, .L1542
	add	r3, r3, r6
	ldrh	r2, [r2]
	ldr	r0, [r3, #8]
	bl	ftl_memset
	b	.L1515
.L1516:
	ldr	r3, .L1542+4
	ldrh	r1, [r3]
	ldr	r3, [r8]
	muls	r1, r5, r1
	add	r6, r6, r3
	ldr	r3, [sp, #20]
	ldr	r0, [r6, #8]
	sub	r1, r1, r10
	add	r1, r3, r1, lsl #9
	b	.L1538
.L1535:
	ldr	r3, [r8]
	add	r6, r6, r3
	ldrh	r3, [r2]
	ldr	r2, [sp, #20]
	muls	r3, r5, r3
	sub	r3, r3, r10
	add	r3, r2, r3, lsl #9
	b	.L1537
.L1523:
	ldr	r6, .L1542+8
	mov	r4, #256
	ldr	r7, .L1542+12
.L1526:
	ldrh	r3, [r6]
	movw	r2, #65535
	cmp	r3, r2
	bne	.L1524
	ldrh	r2, [r7]
	cmp	r2, r3
	bne	.L1524
	movs	r0, #0
	bl	List_get_gc_head_node
	uxth	r0, r0
	bl	FtlGcRefreshBlock
.L1524:
	ldr	r2, .L1542+16
	movs	r3, #128
	movs	r1, #1
	mov	r0, r1
	strh	r3, [r2]	@ movhi
	ldr	r2, .L1542+20
	strh	r3, [r2]	@ movhi
	bl	rk_ftl_garbage_collect
	movs	r1, #1
	movs	r0, #0
	bl	rk_ftl_garbage_collect
	ldrh	r3, [r5]
	cmp	r3, #2
	bhi	.L1525
	subs	r4, r4, #1
	bne	.L1526
	b	.L1525
.L1529:
	mov	r0, #-1
	b	.L1493
.L1543:
	.align	2
.L1542:
	.word	.LANCHOR22
	.word	.LANCHOR11
	.word	.LANCHOR144
	.word	.LANCHOR71
	.word	.LANCHOR100
	.word	.LANCHOR99
	.size	FtlWrite, .-FtlWrite
	.section	.text.sftl_gc,"ax",%progbits
	.align	1
	.global	sftl_gc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sftl_gc, %function
sftl_gc:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #1
	mov	r0, r1
	b	rk_ftl_garbage_collect
	.size	sftl_gc, .-sftl_gc
	.section	.text.FtlLoadSysInfo,"ax",%progbits
	.align	1
	.global	FtlLoadSysInfo
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlLoadSysInfo, %function
FtlLoadSysInfo:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r1, #0
	ldr	r8, .L1575+140
	sub	sp, sp, #24
	ldr	r7, .L1575
	ldr	r3, [r8]
	ldr	r4, .L1575+4
	ldr	fp, .L1575+144
	ldrh	r2, [r7]
	ldr	r5, .L1575+8
	str	r3, [r4, #8]
	ldr	r6, .L1575+12
	ldr	r3, [fp]
	lsls	r2, r2, #1
	ldr	r0, [r5]
	str	r3, [r4, #12]
	bl	ftl_memset
	ldrh	r0, [r6]
	movw	r3, #65535
	str	r5, [sp, #16]
	cmp	r0, r3
	bne	.L1546
.L1557:
	mov	r0, #-1
.L1545:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1546:
	movs	r1, #1
	bl	FtlGetLastWrittenPage
	ldrsh	r10, [r6]
	sxth	r5, r0
	adds	r0, r0, #1
	strh	r0, [r6, #2]	@ movhi
.L1548:
	cmp	r5, #0
	bge	.L1554
	movw	r2, #1474
	ldr	r1, .L1575+16
	ldr	r0, .L1575+20
	bl	printf
	b	.L1553
.L1554:
	orr	r3, r5, r10, lsl #10
	movs	r2, #1
	mov	r1, r2
	ldr	r0, .L1575+4
	str	r3, [r4, #4]
	ldr	r3, [r8]
	str	r3, [r4, #8]
	bl	FlashReadPages
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #12]
	str	r3, [sp, #20]
	cbz	r3, .L1549
	ldr	r2, [r4]
	adds	r2, r2, #1
	beq	.L1549
	ldr	r2, .L1575+24
	ldr	r0, [r4, #8]
	ldrh	r1, [r2]
	bl	js_hash
	ldr	r3, [sp, #20]
	cmp	r3, r0
	beq	.L1549
	str	r0, [sp, #8]
	mov	r2, r10
	str	r3, [sp, #4]
	ldrh	r3, [r6, #4]
	ldr	r1, .L1575+16
	ldr	r0, .L1575+28
	str	r3, [sp]
	mov	r3, r5
	bl	printf
	cbnz	r5, .L1550
	ldrh	r3, [r6, #4]
	cmp	r10, r3
	beq	.L1550
	sxth	r10, r3
	ldr	r3, .L1575+32
	ldrh	r5, [r3]
.L1552:
	subs	r5, r5, #1
	sxth	r5, r5
	b	.L1548
.L1550:
	mov	r3, #-1
	str	r3, [r4]
.L1549:
	ldr	r3, [r4]
	adds	r3, r3, #1
	beq	.L1552
	ldr	r3, [r8]
	ldr	r2, .L1575+36
	ldr	r3, [r3]
	cmp	r3, r2
	bne	.L1552
	ldr	r3, [fp]
	ldrh	r2, [r3]
	movw	r3, #61604
	cmp	r2, r3
	bne	.L1552
.L1553:
	ldr	r2, .L1575+24
	ldrh	r3, [r7]
	ldrh	r2, [r2]
	adds	r3, r3, #24
	cmp	r2, r3, lsl #1
	bcs	.L1556
	movw	r2, #1476
	ldr	r1, .L1575+16
	ldr	r0, .L1575+20
	bl	printf
.L1556:
	ldr	r5, .L1575+40
	movs	r2, #48
	ldr	r1, [r4, #8]
	mov	r0, r5
	bl	ftl_memcpy
	ldrh	r2, [r7]
	ldr	r1, [r4, #8]
	mov	r4, r5
	ldr	r3, [sp, #16]
	lsls	r2, r2, #1
	adds	r1, r1, #48
	ldr	r0, [r3]
	bl	ftl_memcpy
	ldr	r2, [r5]
	ldr	r3, .L1575+36
	cmp	r2, r3
	bne	.L1557
	ldr	r3, .L1575+44
	ldrb	r2, [r4, #10]	@ zero_extendqisi2
	ldrh	r5, [r5, #8]
	ldrh	r3, [r3]
	strh	r5, [r6, #6]	@ movhi
	cmp	r2, r3
	bne	.L1557
	ldr	r3, .L1575+48
	ldr	r2, .L1575+52
	str	r5, [r3]
	ldr	r3, .L1575+56
	ldrh	r3, [r3]
	muls	r3, r5, r3
	str	r3, [r2]
	ldr	r2, .L1575+60
	ldrh	r2, [r2]
	muls	r3, r2, r3
	ldr	r2, .L1575+64
	str	r3, [r2]
	ldr	r3, .L1575+68
	ldr	r6, [r3]
	ldr	r3, .L1575+72
	ldrh	r0, [r3, #6]
	ldr	r3, .L1575+76
	subs	r0, r6, r0
	ldrh	r1, [r3]
	subs	r0, r0, r5
	bl	__aeabi_uidiv
	ldr	r3, .L1575+80
	cmp	r5, r6
	strh	r0, [r3]	@ movhi
	bls	.L1558
	movw	r2, #1498
	ldr	r1, .L1575+16
	ldr	r0, .L1575+20
	bl	printf
.L1558:
	ldrh	r2, [r4, #16]
	ldr	r3, .L1575+84
	ldrh	ip, [r4, #14]
	ldr	r7, .L1575+88
	lsrs	r1, r2, #6
	and	r2, r2, #63
	strb	r2, [r3, #6]
	ldrb	r2, [r4, #11]	@ zero_extendqisi2
	strh	r1, [r3, #2]	@ movhi
	ldr	r1, .L1575+92
	strb	r2, [r3, #8]
	ldrh	r2, [r4, #18]
	strh	ip, [r3]	@ movhi
	movw	r3, #65535
	strh	r3, [r7]	@ movhi
	movs	r3, #0
	strh	r2, [r1]	@ movhi
	ldrh	r2, [r4, #20]
	strh	r3, [r7, #2]	@ movhi
	strb	r3, [r7, #6]
	strb	r3, [r7, #8]
	lsrs	r5, r2, #6
	and	r2, r2, #63
	strb	r2, [r1, #6]
	ldrb	r2, [r4, #12]	@ zero_extendqisi2
	strh	r5, [r1, #2]	@ movhi
	ldrh	r5, [r4, #22]
	strb	r2, [r1, #8]
	ldr	r2, .L1575+96
	strh	r5, [r2]	@ movhi
	ldrh	r5, [r4, #24]
	lsrs	r6, r5, #6
	and	r5, r5, #63
	strb	r5, [r2, #6]
	ldrb	r5, [r4, #13]	@ zero_extendqisi2
	strh	r6, [r2, #2]	@ movhi
	ldr	r6, [r4, #32]
	strb	r5, [r2, #8]
	ldr	r5, .L1575+100
	str	r3, [r5]
	ldr	r5, .L1575+104
	str	r3, [r5]
	ldr	r5, .L1575+108
	str	r3, [r5]
	ldr	r5, .L1575+112
	str	r3, [r5]
	ldr	r5, .L1575+116
	str	r6, [r5]
	mov	r6, r1
	ldr	r5, .L1575+120
	str	r3, [r5]
	ldr	r5, .L1575+124
	str	r3, [r5]
	ldr	r5, .L1575+128
	ldr	lr, [r4, #40]
	str	r3, [r5]
	ldr	r3, .L1575+132
	ldr	r5, [r3]
	cmp	lr, r5
	mov	r5, r2
	it	hi
	strhi	lr, [r3]
	ldr	r3, .L1575+136
	ldr	r2, [r4, #36]
	ldr	r1, [r3]
	cmp	r2, r1
	it	hi
	strhi	r2, [r3]
	movw	r3, #65535
	cmp	ip, r3
	beq	.L1561
	ldr	r0, .L1575+84
	bl	make_superblock
.L1561:
	ldrh	r2, [r6]
	movw	r3, #65535
	cmp	r2, r3
	beq	.L1562
	ldr	r0, .L1575+92
	bl	make_superblock
.L1562:
	ldrh	r2, [r5]
	movw	r3, #65535
	cmp	r2, r3
	beq	.L1563
	ldr	r0, .L1575+96
	bl	make_superblock
.L1563:
	ldrh	r2, [r7]
	movw	r3, #65535
	cmp	r2, r3
	beq	.L1564
	ldr	r0, .L1575+88
	bl	make_superblock
.L1564:
	movs	r0, #0
	b	.L1545
.L1576:
	.align	2
.L1575:
	.word	.LANCHOR4
	.word	.LANCHOR137
	.word	.LANCHOR41
	.word	.LANCHOR138
	.word	.LANCHOR177
	.word	.LC1
	.word	.LANCHOR22
	.word	.LC46
	.word	.LANCHOR19
	.word	1179929683
	.word	.LANCHOR38
	.word	.LANCHOR9
	.word	.LANCHOR178
	.word	.LANCHOR151
	.word	.LANCHOR18
	.word	.LANCHOR11
	.word	.LANCHOR33
	.word	.LANCHOR6
	.word	.LANCHOR36
	.word	.LANCHOR2
	.word	.LANCHOR170
	.word	.LANCHOR50
	.word	.LANCHOR144
	.word	.LANCHOR51
	.word	.LANCHOR52
	.word	.LANCHOR83
	.word	.LANCHOR84
	.word	.LANCHOR88
	.word	.LANCHOR87
	.word	.LANCHOR89
	.word	.LANCHOR90
	.word	.LANCHOR91
	.word	.LANCHOR86
	.word	.LANCHOR81
	.word	.LANCHOR82
	.word	.LANCHOR105
	.word	.LANCHOR113
	.size	FtlLoadSysInfo, .-FtlLoadSysInfo
	.section	.text.FtlMapTblRecovery,"ax",%progbits
	.align	1
	.global	FtlMapTblRecovery
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlMapTblRecovery, %function
FtlMapTblRecovery:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r0, #24]
	movs	r1, #0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	sub	sp, sp, #40
	mov	r4, r0
	ldr	r8, [r0, #12]
	movs	r7, #0
	str	r3, [sp, #8]
	ldr	r3, [r0, #16]
	ldr	r6, .L1618
	str	r3, [sp, #24]
	ldrh	r3, [r0, #6]
	str	r3, [sp, #12]
	ldrh	r3, [r0, #8]
	ldr	r0, [sp, #8]
	str	r3, [sp, #16]
	ldr	r3, [sp, #12]
	lsls	r2, r3, #2
	bl	ftl_memset
	ldr	r3, .L1618+4
	str	r7, [r4, #32]
	str	r7, [r4, #28]
	ldr	r2, [r3]
	str	r3, [sp, #20]
	str	r2, [r6, #8]
	ldr	r2, .L1618+8
	ldr	fp, [r2]
	movw	r2, #65535
	str	fp, [r6, #12]
	strh	r2, [r4]	@ movhi
	strh	r2, [r4, #2]	@ movhi
	movs	r2, #1
	str	r2, [r4, #36]
.L1578:
	ldr	r3, [sp, #16]
	sxth	r5, r7
	cmp	r5, r3
	bge	.L1597
	ldr	r3, [sp, #16]
	subs	r3, r3, #1
	cmp	r5, r3
	bne	.L1579
	movs	r1, #1
	ldrh	r0, [r8, r5, lsl #1]
	bl	FtlGetLastWrittenPage
	sxth	r3, r0
	ldr	r6, .L1618
	add	r10, r8, r5, lsl #1
	strh	r7, [r4]	@ movhi
	str	r3, [sp, #16]
	movs	r7, #0
	ldr	r3, [sp, #24]
	adds	r0, r0, #1
	strh	r0, [r4, #2]	@ movhi
	ldr	r3, [r3, r5, lsl #2]
	str	r3, [r4, #28]
.L1580:
	ldr	r3, [sp, #16]
	sxth	r8, r7
	cmp	r8, r3
	ble	.L1583
.L1597:
	mov	r0, r4
	bl	ftl_free_no_use_map_blk
	ldr	r3, .L1618+12
	ldrh	r2, [r4, #2]
	ldrh	r3, [r3]
	cmp	r2, r3
	bne	.L1585
	mov	r0, r4
	bl	ftl_map_blk_alloc_new_blk
.L1585:
	mov	r0, r4
	bl	ftl_map_blk_gc
	mov	r0, r4
	bl	ftl_map_blk_gc
	movs	r0, #0
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1583:
	ldrh	r2, [r10]
	ldr	r0, .L1618
	orr	r2, r8, r2, lsl #10
	str	r2, [r6, #4]
	movs	r2, #1
	mov	r1, r2
	bl	FlashReadPages
	ldr	r2, [r6, #12]
	ldr	r2, [r2, #12]
	str	r2, [sp, #20]
	cbz	r2, .L1581
	ldr	r1, [r6]
	adds	r1, r1, #1
	beq	.L1581
	ldr	r3, .L1618+16
	ldr	r0, [r6, #8]
	ldrh	r1, [r3]
	bl	js_hash
	ldr	r2, [sp, #20]
	cmp	r2, r0
	beq	.L1581
	str	r0, [sp, #4]
	mov	r3, r8
	str	r2, [sp]
	mov	r2, r5
	ldr	r1, .L1618+20
	ldr	r0, .L1618+24
	bl	printf
	mov	r3, #-1
	str	r3, [r6]
.L1581:
	ldr	r3, [r6]
	adds	r3, r3, #1
	beq	.L1582
	ldrh	r3, [fp, #8]
	ldr	r2, [sp, #12]
	cmp	r2, r3
	bls	.L1582
	ldrh	r1, [fp]
	ldrh	r2, [r4, #4]
	cmp	r1, r2
	ittt	eq
	ldreq	r2, [r6, #4]
	ldreq	r1, [sp, #8]
	streq	r2, [r1, r3, lsl #2]
.L1582:
	adds	r7, r7, #1
	b	.L1580
.L1579:
	ldr	r3, [sp, #20]
	ldr	r10, .L1618+12
	ldrh	r2, [r8, r5, lsl #1]
	ldr	r3, [r3]
	ldr	r0, .L1618
	str	r3, [r6, #8]
	add	r3, r8, r5, lsl #1
	str	r3, [sp, #28]
	ldrh	r3, [r10]
	subs	r3, r3, #1
	orr	r3, r3, r2, lsl #10
	movs	r2, #1
	mov	r1, r2
	str	r3, [r6, #4]
	bl	FlashReadPages
	ldr	r3, [r6]
	adds	r3, r3, #1
	beq	.L1599
	ldrh	r2, [fp]
	ldrh	r3, [r4, #4]
	cmp	r2, r3
	bne	.L1599
	ldrh	r2, [fp, #8]
	movw	r3, #64245
	cmp	r2, r3
	beq	.L1587
.L1599:
	mov	r10, #0
.L1588:
	ldr	r2, .L1618+12
	sxth	r3, r10
	ldrh	r2, [r2]
	cmp	r3, r2
	bge	.L1595
	ldr	r2, [sp, #28]
	ldr	r0, .L1618
	str	r3, [sp, #36]
	ldrh	r2, [r2]
	orr	r2, r3, r2, lsl #10
	str	r2, [r6, #4]
	movs	r2, #1
	mov	r1, r2
	bl	FlashReadPages
	ldr	r2, [r6, #12]
	ldr	r2, [r2, #12]
	str	r2, [sp, #32]
	cbz	r2, .L1592
	ldr	r1, [r6]
	adds	r1, r1, #1
	beq	.L1592
	ldr	r1, .L1618+16
	ldr	r0, [r6, #8]
	ldrh	r1, [r1]
	bl	js_hash
	ldr	r2, [sp, #32]
	cmp	r2, r0
	beq	.L1592
	str	r0, [sp, #4]
	str	r2, [sp]
	mov	r2, r5
	ldr	r3, [sp, #36]
	ldr	r1, .L1618+20
	ldr	r0, .L1618+28
	bl	printf
	mov	r3, #-1
	str	r3, [r6]
.L1592:
	ldr	r3, [r6]
	adds	r3, r3, #1
	beq	.L1593
	ldrh	r3, [fp, #8]
	ldr	r2, [sp, #12]
	cmp	r2, r3
	bls	.L1593
	ldrh	r1, [fp]
	ldrh	r2, [r4, #4]
	cmp	r1, r2
	ittt	eq
	ldreq	r2, [r6, #4]
	ldreq	r1, [sp, #8]
	streq	r2, [r1, r3, lsl #2]
.L1593:
	add	r10, r10, #1
	b	.L1588
.L1587:
	ldr	r3, [sp, #20]
	movs	r1, #0
	ldrh	r2, [r10]
	ldr	r0, [r3]
	add	lr, r2, #-1
.L1589:
	sxth	r3, r1
	cmp	r3, lr
	blt	.L1591
.L1595:
	adds	r7, r7, #1
	b	.L1578
.L1591:
	lsls	r5, r3, #3
	ldr	r3, [r0, r3, lsl #3]
	ldr	r2, [sp, #12]
	uxth	ip, r3
	cmp	r2, ip
	bls	.L1590
	add	r5, r5, r0
	ldr	r2, [sp, #8]
	ldr	r5, [r5, #4]
	str	r5, [r2, ip, lsl #2]
.L1590:
	adds	r1, r1, #1
	b	.L1589
.L1619:
	.align	2
.L1618:
	.word	.LANCHOR137
	.word	.LANCHOR105
	.word	.LANCHOR113
	.word	.LANCHOR19
	.word	.LANCHOR22
	.word	.LANCHOR179
	.word	.LC47
	.word	.LC48
	.size	FtlMapTblRecovery, .-FtlMapTblRecovery
	.section	.text.FtlLoadVonderInfo,"ax",%progbits
	.align	1
	.global	FtlLoadVonderInfo
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlLoadVonderInfo, %function
FtlLoadVonderInfo:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r3, .L1621
	ldr	r0, .L1621+4
	ldrh	r3, [r3]
	strh	r3, [r0, #10]	@ movhi
	movw	r3, #61574
	strh	r3, [r0, #4]	@ movhi
	ldr	r3, .L1621+8
	ldrh	r3, [r3]
	strh	r3, [r0, #8]	@ movhi
	ldr	r3, .L1621+12
	ldrh	r3, [r3]
	strh	r3, [r0, #6]	@ movhi
	ldr	r3, .L1621+16
	ldr	r3, [r3]
	str	r3, [r0, #12]
	ldr	r3, .L1621+20
	ldr	r3, [r3]
	str	r3, [r0, #16]
	ldr	r3, .L1621+24
	ldr	r3, [r3]
	str	r3, [r0, #20]
	ldr	r3, .L1621+28
	ldr	r3, [r3]
	str	r3, [r0, #24]
	bl	FtlMapTblRecovery
	movs	r0, #0
	pop	{r3, pc}
.L1622:
	.align	2
.L1621:
	.word	.LANCHOR26
	.word	.LANCHOR160
	.word	.LANCHOR34
	.word	.LANCHOR27
	.word	.LANCHOR35
	.word	.LANCHOR121
	.word	.LANCHOR120
	.word	.LANCHOR122
	.size	FtlLoadVonderInfo, .-FtlLoadVonderInfo
	.section	.text.FtlLoadMapInfo,"ax",%progbits
	.align	1
	.global	FtlLoadMapInfo
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlLoadMapInfo, %function
FtlLoadMapInfo:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	bl	FtlL2PDataInit
	ldr	r0, .L1624
	bl	FtlMapTblRecovery
	movs	r0, #0
	pop	{r3, pc}
.L1625:
	.align	2
.L1624:
	.word	.LANCHOR128
	.size	FtlLoadMapInfo, .-FtlLoadMapInfo
	.section	.text.FtlSysBlkInit,"ax",%progbits
	.align	1
	.global	FtlSysBlkInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlSysBlkInit, %function
FtlSysBlkInit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	movs	r3, #0
	ldr	r4, .L1639
	strh	r3, [r4]	@ movhi
	ldr	r3, .L1639+4
	ldrh	r0, [r3]
	bl	FtlFreeSysBlkQueueInit
	bl	FtlScanSysBlk
	ldr	r3, .L1639+8
	ldrh	r2, [r3]
	movw	r3, #65535
	cmp	r2, r3
	bne	.L1627
.L1629:
	mov	r6, #-1
.L1626:
	mov	r0, r6
	pop	{r3, r4, r5, r6, r7, pc}
.L1627:
	bl	FtlLoadSysInfo
	mov	r6, r0
	cmp	r0, #0
	bne	.L1629
	bl	FtlLoadMapInfo
	bl	FtlLoadVonderInfo
	bl	Ftl_load_ext_data
	bl	FtlLoadEctTbl
	bl	FtlFreeSysBLkSort
	bl	SupperBlkListInit
	bl	FtlPowerLostRecovery
	movs	r0, #1
	bl	FtlUpdateVaildLpn
	ldr	r3, .L1639+12
	movs	r0, #12
	ldrh	r1, [r3]
	ldr	r3, .L1639+16
	ldr	r2, [r3]
	mov	r3, r6
.L1630:
	cmp	r3, r1
	bge	.L1635
	mla	r5, r0, r3, r2
	ldr	r5, [r5, #4]
	cmp	r5, #0
	bge	.L1631
.L1635:
	ldr	r0, .L1639+20
	cmp	r3, r1
	ldr	r2, [r0]
	add	r2, r2, #32
	str	r2, [r0]
	bge	.L1637
.L1632:
	ldr	r5, .L1639+24
	ldr	r4, .L1639+28
	mov	r0, r5
	bl	FtlSuperblockPowerLostFix
	mov	r0, r4
	bl	FtlSuperblockPowerLostFix
	ldr	r3, .L1639+32
	ldrh	r1, [r5]
	ldrh	r0, [r5, #4]
	ldr	r2, [r3]
	ldrh	r3, [r2, r1, lsl #1]
	subs	r3, r3, r0
	ldr	r0, .L1639+36
	strh	r3, [r2, r1, lsl #1]	@ movhi
	ldrh	r7, [r4, #4]
	ldrh	r3, [r0]
	strh	r3, [r5, #2]	@ movhi
	movs	r3, #0
	strb	r3, [r5, #6]
	strh	r3, [r5, #4]	@ movhi
	ldrh	r5, [r4]
	ldrh	r1, [r2, r5, lsl #1]
	subs	r1, r1, r7
	strh	r1, [r2, r5, lsl #1]	@ movhi
	ldrh	r2, [r0]
	strb	r3, [r4, #6]
	strh	r3, [r4, #4]	@ movhi
	strh	r2, [r4, #2]	@ movhi
	ldr	r2, .L1639+40
	ldrh	r3, [r2, #30]
	adds	r3, r3, #1
	strh	r3, [r2, #30]	@ movhi
	bl	l2p_flush
	bl	FtlVpcTblFlush
.L1638:
	bl	FtlVpcTblFlush
	b	.L1626
.L1631:
	adds	r3, r3, #1
	b	.L1630
.L1637:
	ldrh	r3, [r4]
	cmp	r3, #0
	bne	.L1632
	bl	l2p_flush
	b	.L1638
.L1640:
	.align	2
.L1639:
	.word	.LANCHOR155
	.word	.LANCHOR3
	.word	.LANCHOR138
	.word	.LANCHOR32
	.word	.LANCHOR54
	.word	.LANCHOR82
	.word	.LANCHOR50
	.word	.LANCHOR51
	.word	.LANCHOR41
	.word	.LANCHOR18
	.word	.LANCHOR38
	.size	FtlSysBlkInit, .-FtlSysBlkInit
	.section	.text.ftl_low_format,"ax",%progbits
	.align	1
	.global	ftl_low_format
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ftl_low_format, %function
ftl_low_format:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r10, fp, lr}
	movs	r3, #0
	ldr	r2, .L1665
	ldr	r4, .L1665+4
	str	r3, [r2]
	ldr	r5, .L1665+8
	ldr	r2, .L1665+12
	mov	r8, r4
	ldrh	r0, [r4]
	str	r3, [r5]
	str	r3, [r2]
	bl	FtlFreeSysBlkQueueInit
	bl	FtlLoadBbt
	cbz	r0, .L1642
	bl	FtlMakeBbt
.L1642:
	ldr	r3, .L1665+16
	ldr	r2, .L1665+20
	ldr	r7, .L1665+24
	ldrh	r1, [r3]
	ldr	r4, [r2]
	ldr	r2, .L1665+28
	str	r3, [sp, #4]
	lsls	r1, r1, #7
	ldr	r6, [r2]
	movs	r2, #0
.L1643:
	uxth	r3, r2
	adds	r2, r2, #1
	cmp	r3, r1
	blt	.L1644
	ldr	r7, .L1665+32
	mov	r10, #0
	ldr	r6, .L1665+36
	ldrh	r4, [r7]
	mov	fp, r6
.L1645:
	ldrh	r3, [r6]
	cmp	r3, r4
	bhi	.L1646
	ldr	r6, .L1665+40
	sub	r3, r10, #2
	ldrh	r1, [r6]
	cmp	r3, r1, lsl #1
	bgt	.L1647
.L1651:
	movs	r4, #0
	mov	r8, r4
.L1648:
	ldrh	r3, [r7]
	uxth	r0, r4
	adds	r4, r4, #1
	cmp	r3, r0
	bhi	.L1652
	ldrh	r2, [fp]
	ldr	r3, .L1665+44
	ldrh	r4, [r6]
	ldr	r6, .L1665+48
	str	r2, [r3]
	ldr	r3, .L1665+52
	mov	r1, r4
	ldr	r7, .L1665+56
	ldr	r2, [r3]
	mov	r0, r2
	str	r2, [sp, #12]
	bl	__aeabi_uidiv
	ubfx	r10, r0, #5, #16
	mov	r3, r0
	add	r1, r10, #36
	str	r0, [r7]
	strh	r1, [r6]	@ movhi
	movs	r1, #24
	muls	r1, r4, r1
	cmp	r8, r1
	ble	.L1653
	ldr	r2, [sp, #12]
	mov	r1, r4
	str	r0, [sp, #8]
	sub	r0, r2, r8
	bl	__aeabi_uidiv
	ldr	r3, [sp, #8]
	str	r0, [r7]
	lsrs	r0, r0, #5
	adds	r0, r0, #24
	strh	r0, [r6]	@ movhi
.L1653:
	ldr	r2, .L1665+60
	ldrh	r2, [r2]
	cbz	r2, .L1655
	ldrh	r1, [r6]
	add	r1, r1, r2, lsr #1
	strh	r1, [r6]	@ movhi
	mul	r1, r4, r2
	cmp	r8, r1
	itttt	lt
	addlt	r2, r2, #32
	strlt	r3, [r7]
	addlt	r2, r2, r10
	strhlt	r2, [r6]	@ movhi
.L1655:
	ldrh	r2, [r6]
	ldr	r3, [r7]
	ldr	r6, .L1665+64
	subs	r3, r3, r2
	muls	r4, r3, r4
	ldr	r3, .L1665+68
	ldrh	r3, [r3]
	str	r4, [r6]
	muls	r4, r3, r4
	ldr	r3, [sp, #4]
	ldrh	r3, [r3]
	str	r4, [r7]
	ldr	r7, .L1665+72
	muls	r4, r3, r4
	ldr	r3, .L1665+76
	str	r4, [r3]
	bl	FtlBbmTblFlush
	ldrh	r2, [fp]
	movs	r1, #0
	ldr	r0, [r7]
	ldr	r4, .L1665+80
	lsls	r2, r2, #1
	mov	r10, r4
	bl	ftl_memset
	ldr	r2, .L1665+84
	movs	r3, #0
	movw	r1, #65535
	strh	r3, [r4, #2]	@ movhi
	mov	r8, r1
	strb	r3, [r4, #6]
	str	r3, [r2]
	ldr	r2, .L1665+88
	strh	r3, [r4]	@ movhi
	strh	r3, [r2, #2]	@ movhi
	strb	r3, [r2, #6]
	strb	r3, [r2, #8]
	movs	r3, #1
	strh	r1, [r2]	@ movhi
	strb	r3, [r4, #8]
.L1657:
	mov	r0, r10
	bl	make_superblock
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldrh	r2, [r4]
	cmp	r3, #0
	bne	.L1658
	ldr	r3, [r7]
	strh	r8, [r3, r2, lsl #1]	@ movhi
	ldrh	r3, [r4]
	adds	r3, r3, #1
	strh	r3, [r4]	@ movhi
	b	.L1657
.L1644:
	mvns	r0, r3
	orr	r0, r3, r0, lsl #16
	str	r0, [r4, r3, lsl #2]
	str	r7, [r6, r3, lsl #2]
	b	.L1643
.L1646:
	mov	r0, r4
	movs	r1, #1
	bl	FtlLowFormatEraseBlock
	adds	r4, r4, #1
	add	r10, r10, r0
	uxth	r10, r10
	uxth	r4, r4
	b	.L1645
.L1647:
	mov	r0, r10
	bl	__aeabi_uidiv
	ldr	r3, .L1665+92
	ldr	r3, [r3]
	add	r0, r0, r3
	uxth	r0, r0
	bl	FtlSysBlkNumInit
	ldrh	r0, [r8]
	bl	FtlFreeSysBlkQueueInit
	ldrh	r4, [r7]
.L1649:
	ldrh	r3, [fp]
	cmp	r3, r4
	bls	.L1651
	mov	r0, r4
	movs	r1, #1
	adds	r4, r4, #1
	bl	FtlLowFormatEraseBlock
	uxth	r4, r4
	b	.L1649
.L1652:
	movs	r1, #0
	bl	FtlLowFormatEraseBlock
	add	r8, r8, r0
	uxth	r8, r8
	b	.L1648
.L1658:
	ldr	r3, [r5]
	movw	r8, #65535
	ldrh	r1, [r4, #4]
	str	r3, [r4, #12]
	adds	r3, r3, #1
	str	r3, [r5]
	ldr	r3, [r7]
	strh	r1, [r3, r2, lsl #1]	@ movhi
	movs	r2, #0
	ldr	r3, .L1665+96
	strh	r2, [r3, #2]	@ movhi
	mov	r10, r3
	strb	r2, [r3, #6]
	ldrh	r2, [r4]
	mov	r4, r3
	adds	r2, r2, #1
	strh	r2, [r3]	@ movhi
	movs	r2, #1
	strb	r2, [r3, #8]
.L1659:
	mov	r0, r10
	bl	make_superblock
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	ldrh	r2, [r4]
	cbnz	r3, .L1660
	ldr	r3, [r7]
	strh	r8, [r3, r2, lsl #1]	@ movhi
	ldrh	r3, [r4]
	adds	r3, r3, #1
	strh	r3, [r4]	@ movhi
	b	.L1659
.L1660:
	ldr	r3, [r5]
	ldrh	r1, [r4, #4]
	str	r3, [r4, #12]
	adds	r3, r3, #1
	str	r3, [r5]
	movw	r4, #65535
	ldr	r3, [r7]
	strh	r1, [r3, r2, lsl #1]	@ movhi
	ldr	r3, .L1665+100
	strh	r4, [r3]	@ movhi
	bl	FtlFreeSysBlkQueueOut
	ldr	r3, .L1665+104
	movs	r2, #0
	strh	r2, [r3, #2]	@ movhi
	ldr	r2, [r6]
	strh	r0, [r3]	@ movhi
	strh	r4, [r3, #4]	@ movhi
	strh	r2, [r3, #6]	@ movhi
	ldr	r2, [r5]
	str	r2, [r3, #8]
	adds	r2, r2, #1
	str	r2, [r5]
	bl	FtlVpcTblFlush
	bl	FtlSysBlkInit
	cbnz	r0, .L1661
	ldr	r3, .L1665+108
	movs	r2, #1
	str	r2, [r3]
.L1661:
	movs	r0, #0
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1666:
	.align	2
.L1665:
	.word	.LANCHOR82
	.word	.LANCHOR3
	.word	.LANCHOR81
	.word	.LANCHOR142
	.word	.LANCHOR11
	.word	.LANCHOR108
	.word	168778952
	.word	.LANCHOR109
	.word	.LANCHOR4
	.word	.LANCHOR5
	.word	.LANCHOR2
	.word	.LANCHOR98
	.word	.LANCHOR170
	.word	.LANCHOR6
	.word	.LANCHOR151
	.word	.LANCHOR14
	.word	.LANCHOR178
	.word	.LANCHOR18
	.word	.LANCHOR41
	.word	.LANCHOR33
	.word	.LANCHOR50
	.word	.LANCHOR58
	.word	.LANCHOR144
	.word	.LANCHOR30
	.word	.LANCHOR51
	.word	.LANCHOR52
	.word	.LANCHOR138
	.word	.LANCHOR165
	.size	ftl_low_format, .-ftl_low_format
	.section	.text.sftl_init,"ax",%progbits
	.align	1
	.global	sftl_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sftl_init, %function
sftl_init:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r3, #-1
	ldr	r4, .L1679
	movs	r6, #0
	ldr	r1, .L1679+4
	ldr	r0, .L1679+8
	str	r3, [r4]
	bl	printf
	ldr	r0, .L1679+12
	bl	FtlConstantsInit
	ldr	r5, .L1679+16
	bl	FtlMemInit
	bl	FtlVariablesInit
	ldr	r3, .L1679+20
	ldrh	r0, [r3]
	bl	FtlFreeSysBlkQueueInit
.L1668:
	bl	FtlLoadBbt
	cbz	r0, .L1669
.L1678:
	ldr	r3, [r5]
	cmp	r3, #1
	bne	.L1668
	str	r6, [r5]
	bl	ftl_low_format
	b	.L1668
.L1669:
	bl	FtlSysBlkInit
	cmp	r0, #0
	bne	.L1678
	movs	r3, #1
	str	r3, [r4]
	pop	{r4, r5, r6, pc}
.L1680:
	.align	2
.L1679:
	.word	.LANCHOR165
	.word	.LC49
	.word	.LC38
	.word	.LANCHOR0
	.word	.LANCHOR180
	.word	.LANCHOR3
	.size	sftl_init, .-sftl_init
	.section	.text.FtlWriteToIDB,"ax",%progbits
	.align	1
	.global	FtlWriteToIDB
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	FtlWriteToIDB, %function
FtlWriteToIDB:
	@ args = 0, pretend = 0, frame = 96
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r10, fp, lr}
	adds	r7, r1, r0
	add	r8, r7, #-1
	sub	sp, sp, #104
	cmp	r8, #63
	mov	r4, r0
	mov	r5, r1
	mov	r6, r2
	ldr	r10, .L1749+16
	bls	.L1682
	cmp	r0, #576
	bcc	.L1683
.L1682:
	ldr	r3, [r10]
	cbnz	r3, .L1684
.L1748:
	movs	r4, #0
	b	.L1681
.L1684:
	ldr	r3, .L1749
	ldr	fp, [r3]
	ldr	r3, .L1749+4
	ldr	r2, [fp]
	cmp	r2, r3
	bne	.L1686
	ldr	r3, .L1749+8
	add	r0, fp, #260096
	movs	r2, #0
	ldrh	r1, [r3, #10]
	movw	r3, #65023
.L1690:
	ldr	r4, [r0, #-4]!
	cbnz	r4, .L1687
	ldr	r4, [fp, r2, lsl #2]
	adds	r2, r2, #1
	cmp	r2, #4096
	add	r3, r3, #-1
	it	hi
	movhi	r2, #0
	cmp	r3, #4096
	str	r4, [r0, #2048]
	bne	.L1690
	mov	r3, #512
	b	.L1745
.L1687:
	adds	r3, r3, #127
	lsrs	r3, r3, #7
.L1745:
	str	r3, [sp, #8]
	lsls	r1, r1, #2
	ldr	r3, [sp, #8]
	uxth	r1, r1
	adds	r0, r3, #4
	bl	__aeabi_uidiv
	adds	r3, r0, #1
	ldr	r1, [sp, #8]
	mov	r2, r3
	ldr	r0, .L1749+12
	str	r3, [sp, #16]
	bl	printf
	ldr	r3, [sp, #8]
	lsls	r3, r3, #7
	str	r3, [sp, #32]
	movs	r3, #0
	str	r3, [sp, #20]
.L1747:
	str	r3, [sp, #12]
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #16]
	add	r3, r3, r2
	cmp	r3, #8
	str	r3, [sp, #28]
	bls	.L1713
	ldr	r3, [sp, #20]
	cbnz	r3, .L1714
.L1686:
	mov	r3, #-1
	str	r3, [sp, #8]
.L1714:
	ldr	r3, .L1749+16
	movs	r2, #0
	ldr	r4, [sp, #8]
	str	r2, [r3]
	ldr	r3, .L1749
	ldr	r0, [r3]
	bl	free
	ldr	r3, .L1749+20
	ldr	r0, [r3]
	bl	free
.L1681:
	mov	r0, r4
	add	sp, sp, #104
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L1713:
	ldr	r3, .L1749+20
	mov	r2, #512
	movs	r1, #0
	ldr	r0, [r3]
	bl	memset
	ldr	r3, .L1749+8
	ldrh	r5, [r3, #10]
	ldr	r3, [sp, #12]
	muls	r3, r5, r3
	str	r3, [sp, #24]
	ldr	r3, .L1749+24
	ldr	r2, [r3, #16]
	cmp	r2, #0
	beq	.L1721
	ldr	r3, [r3, #20]
	cmp	r3, #0
	ite	eq
	moveq	r6, #6
	movne	r6, #9
.L1692:
	ldr	r7, [sp, #24]
	movs	r4, #0
	ldr	r8, .L1749+24
.L1693:
	mov	r1, r7
	ldr	r3, [r8, #4]
	movs	r0, #0
	adds	r4, r4, #1
	blx	r3
	ldr	r3, [sp, #16]
	add	r7, r7, r5
	cmp	r3, r4
	bhi	.L1693
	cmp	r6, #9
	bne	.L1723
	ldr	r3, .L1749+20
	mov	r2, #1024
	movs	r1, #0
	ldr	r4, [r3]
	mov	r0, r4
	bl	ftl_memset
	movs	r2, #4
	ldr	r3, .L1749+28
	strb	r2, [r4, #17]
	movs	r1, #12
	ldr	r2, .L1749+8
	adds	r0, r4, r1
	str	r3, [r4]
	movs	r3, #0
	str	r1, [r4, #4]
	ldrh	r2, [r2, #10]
	str	r3, [r4, #12]
	strb	r3, [r4, #16]
	strh	r2, [r4, #18]	@ movhi
	movs	r2, #16
	strb	r3, [r4, #20]
	strb	r2, [r4, #21]
	strh	r3, [r4, #22]	@ movhi
	bl	js_hash
	str	r0, [r4, #8]
.L1694:
	ldr	r3, [sp, #16]
	mov	r10, fp
	ldr	r8, .L1749+24
	muls	r3, r5, r3
	movs	r5, #0
	str	r3, [sp, #36]
.L1695:
	ldr	r3, [sp, #36]
	cmp	r5, r3
	beq	.L1702
	cmp	r6, #9
	ite	ne
	lslne	r3, r5, #2
	addeq	r3, r5, #1
	str	r3, [sp, #40]
	movw	r3, #61424
	str	r3, [sp, #44]
	cmp	r5, #0
	bne	.L1698
	cmp	r6, #9
	bne	.L1698
	ldr	r3, [r8, #16]
	movs	r0, #70
	blx	r3
	ldr	r7, [r8, #8]
	add	r3, sp, #40
	mov	r2, r4
	ldr	r1, [sp, #24]
	mov	r0, r5
	blx	r7
	ldr	r2, .L1749+8
	mov	r7, r0
	ldr	r3, [r8, #16]
	ldrb	r0, [r2, #22]	@ zero_extendqisi2
	blx	r3
	adds	r7, r7, #1
	bne	.L1699
.L1702:
	ldr	r3, .L1749+20
	ldr	r7, [r3]
	ldr	r3, .L1749+8
	ldrb	r2, [r3, #14]	@ zero_extendqisi2
	ldrh	r3, [r3, #10]
	str	r2, [sp, #24]
	ldr	r2, [sp, #12]
	mul	r10, r3, r2
	ldr	r2, .L1749+24
	ldr	r1, [r2, #16]
	cmp	r1, #0
	bne	.L1700
	mov	r8, #6
.L1701:
	ldr	r2, [sp, #16]
	movs	r4, #0
	ldr	r5, .L1749+24
	muls	r3, r2, r3
	str	r3, [sp, #36]
.L1704:
	ldr	r3, [sp, #36]
	cmp	r4, r3
	beq	.L1708
	cmp	r4, #0
	bne	.L1705
	cmp	r8, #9
	bne	.L1705
	ldr	r3, [r5, #16]
	movs	r0, #70
	blx	r3
	ldr	r3, [r5, #20]
	movs	r0, #2
	blx	r3
	mov	r2, r7
	mov	r1, r10
	add	r3, sp, #40
	mov	r0, r4
	ldr	r6, [r5, #12]
	blx	r6
	ldr	r3, [r5, #20]
	ldr	r0, [sp, #24]
	blx	r3
	ldr	r2, .L1749+8
	ldr	r3, [r5, #16]
	ldrb	r0, [r2, #22]	@ zero_extendqisi2
	blx	r3
	ldr	r3, [r7]
	ldr	r2, .L1749+28
	cmp	r3, r2
	beq	.L1706
.L1708:
	ldr	r3, .L1749+20
	mov	r1, fp
	movs	r4, #0
	ldr	r0, [r3]
.L1707:
	mov	r6, r1
	ldr	r2, [r0, r4, lsl #2]
	ldr	r3, [r6]
	lsls	r5, r4, #2
	adds	r1, r1, #4
	cmp	r2, r3
	beq	.L1710
	mov	r2, #512
	movs	r1, #0
	bl	memset
	ldr	r3, .L1749+20
	ldr	r1, [sp, #12]
	ldr	r0, .L1749+32
	ldr	r2, [r3]
	str	r4, [sp]
	ldr	r3, [r6]
	ldr	r2, [r2, r5]
	bl	printf
	ldr	r3, .L1749+8
	movs	r0, #0
	ldr	r2, [sp, #12]
	ldrh	r1, [r3, #10]
	ldr	r3, .L1749+24
	muls	r2, r1, r2
	ldr	r3, [r3, #4]
	mov	r1, r2
	blx	r3
.L1711:
	ldr	r3, [sp, #28]
	b	.L1747
.L1721:
	movs	r6, #6
	b	.L1692
.L1723:
	movs	r4, #0
	b	.L1694
.L1698:
	ldr	r1, [sp, #24]
	add	r3, sp, #40
	ldr	r7, [r8, #8]
	mov	r2, r10
	movs	r0, #0
	add	r1, r1, r5
	blx	r7
	adds	r0, r0, #1
	beq	.L1702
	add	r10, r10, #2048
.L1699:
	adds	r5, r5, #1
	b	.L1695
.L1700:
	ldr	r2, [r2, #20]
	cmp	r2, #0
	ite	eq
	moveq	r8, #6
	movne	r8, #9
	b	.L1701
.L1705:
	add	r3, sp, #40
	mov	r2, r7
	add	r1, r10, r4
	movs	r0, #0
	ldr	r6, [r5, #12]
	blx	r6
	adds	r0, r0, #1
	beq	.L1708
	ldr	r2, [sp, #44]
	movw	r3, #61424
	cmp	r2, r3
	bne	.L1708
	add	r7, r7, #2048
.L1706:
	adds	r4, r4, #1
	b	.L1704
.L1710:
	ldr	r3, [sp, #32]
	adds	r4, r4, #1
	cmp	r4, r3
	bne	.L1707
	ldr	r3, [sp, #20]
	adds	r3, r3, #1
	cmp	r3, #5
	str	r3, [sp, #20]
	bls	.L1711
	b	.L1714
.L1750:
	.align	2
.L1749:
	.word	.LANCHOR182
	.word	-52655045
	.word	.LANCHOR0
	.word	.LC50
	.word	.LANCHOR181
	.word	.LANCHOR183
	.word	.LANCHOR77
	.word	1179535694
	.word	.LC51
.L1683:
	cmp	r0, #64
	bne	.L1715
	ldr	fp, .L1751+4
	mov	r0, #262144
	bl	ftl_malloc
	str	r0, [fp]
	mov	r0, #262144
	bl	ftl_malloc
	ldr	r3, .L1751
	str	r0, [r3]
	ldr	r3, [fp]
	cbz	r3, .L1716
	cbz	r0, .L1716
	movs	r2, #1
	movs	r1, #0
	str	r2, [r10]
	mov	r0, r3
	mov	r2, #262144
	bl	ftl_memset
.L1715:
	ldr	r3, [r10]
	cmp	r3, #0
	beq	.L1748
	ldr	r3, .L1751+4
	cmp	r4, #63
	itett	ls
	rsbls	r1, r4, #64
	movhi	r1, r6
	subls	r5, r5, r1
	addls	r1, r6, r1, lsl #9
	ldr	r0, [r3]
	ittt	hi
	ldrhi	r3, .L1751+8
	addhi	r3, r3, r4
	addhi	r0, r0, r3, lsl #9
	cmp	r8, #576
	itt	cs
	subcs	r5, r5, r7
	subcs	r5, r5, #446
	lsls	r2, r5, #9
	bl	ftl_memcpy
	b	.L1748
.L1716:
	ldr	r1, .L1751+12
	ldr	r0, .L1751+16
	bl	printf
	b	.L1715
.L1752:
	.align	2
.L1751:
	.word	.LANCHOR183
	.word	.LANCHOR182
	.word	8388544
	.word	.LANCHOR184
	.word	.LC52
	.size	FtlWriteToIDB, .-FtlWriteToIDB
	.section	.text.sftl_write,"ax",%progbits
	.align	1
	.global	sftl_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sftl_write, %function
sftl_write:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	mov	r6, r0
	mov	r4, r1
	mov	r5, r2
	add	r6, r6, r4
	bl	FtlWriteToIDB
.L1754:
	cmp	r4, #256
	sub	r1, r6, r4
	mov	r3, r5
	bhi	.L1756
	mov	r2, r4
	movs	r0, #0
	pop	{r4, r5, r6, lr}
	b	FtlWrite
.L1756:
	mov	r2, #256
	movs	r0, #0
	bl	FtlWrite
	cbnz	r0, .L1753
	add	r5, r5, #131072
	sub	r4, r4, #256
	b	.L1754
.L1753:
	pop	{r4, r5, r6, pc}
	.size	sftl_write, .-sftl_write
	.global	g_nand_ops
	.global	g_nand_phy_info
	.global	gc_ink_free_return_value
	.global	check_vpc_table
	.global	FtlUpdateVaildLpnCount
	.global	g_ect_tbl_power_up_flush
	.global	power_up_flag
	.global	low_format_en
	.global	gFtlInitStatus
	.global	DeviceCapacity
	.global	g_power_lost_recovery_flag
	.global	c_mlc_erase_count_value
	.global	g_recovery_ppa_tbl
	.global	g_recovery_page_min_ver
	.global	g_recovery_page_num
	.global	sftl_nand_check_spare_buf
	.global	sftl_temp_buf
	.global	sftl_nand_check_buf
	.global	g_cur_erase_blk
	.global	g_gc_skip_write_count
	.global	g_gc_head_data_block_count
	.global	g_gc_head_data_block
	.global	g_ftl_nand_free_count
	.global	g_in_swl_replace
	.global	g_in_gc_progress
	.global	g_max_erase_count
	.global	g_totle_sys_slc_erase_count
	.global	g_totle_slc_erase_count
	.global	g_min_erase_count
	.global	g_totle_avg_erase_count
	.global	g_totle_mlc_erase_count
	.global	g_totle_l2p_write_count
	.global	g_totle_cache_write_count
	.global	g_tmp_data_superblock_id
	.global	g_totle_read_page_count
	.global	g_totle_discard_page_count
	.global	g_totle_read_sector
	.global	g_totle_write_sector
	.global	g_totle_write_page_count
	.global	g_totle_gc_page_count
	.global	g_gc_blk_index
	.global	g_gc_merge_free_blk_threshold
	.global	g_gc_free_blk_threshold
	.global	g_gc_bad_block_temp_tbl
	.global	g_gc_bad_block_gc_index
	.global	g_gc_bad_block_temp_num
	.global	g_gc_next_blk_1
	.global	g_gc_next_blk
	.global	g_gc_cur_blk_max_valid_pages
	.global	g_gc_cur_blk_valid_pages
	.global	g_gc_page_offset
	.global	g_gc_blk_num
	.global	p_gc_blk_tbl
	.global	p_gc_page_info
	.global	g_sys_ext_data
	.global	g_sys_save_data
	.global	gp_last_act_superblock
	.global	g_gc_superblock
	.global	g_gc_temp_superblock
	.global	g_buffer_superblock
	.global	g_active_superblock
	.global	g_num_data_superblocks
	.global	g_num_free_superblocks
	.global	p_data_block_list_tail
	.global	p_data_block_list_head
	.global	p_free_data_block_list_head
	.global	p_data_block_list_table
	.global	g_l2p_last_update_region_id
	.global	p_l2p_map_buf
	.global	p_l2p_ram_map
	.global	g_totle_vendor_block
	.global	p_vendor_region_ppn_table
	.global	p_vendor_block_ver_table
	.global	p_vendor_block_valid_page_count
	.global	p_vendor_block_table
	.global	g_totle_map_block
	.global	p_map_region_ppn_table
	.global	p_map_block_ver_table
	.global	p_map_block_valid_page_count
	.global	p_map_block_table
	.global	p_valid_page_count_check_table
	.global	p_valid_page_count_table
	.global	g_totle_swl_count
	.global	p_swl_mul_table
	.global	p_erase_count_table
	.global	g_ect_tbl_info_size
	.global	gp_ect_tbl_info
	.global	g_gc_num_req
	.global	c_gc_page_buf_num
	.global	gp_gc_page_buf_info
	.global	p_gc_data_buf
	.global	p_gc_spare_buf
	.global	p_io_spare_buf
	.global	p_io_data_buf_1
	.global	p_io_data_buf_0
	.global	p_sys_spare_buf
	.global	p_vendor_data_buf
	.global	p_sys_data_buf_1
	.global	p_sys_data_buf
	.global	p_plane_order_table
	.global	req_gc_dst
	.global	req_gc
	.global	req_erase
	.global	req_prgm
	.global	req_read
	.global	req_sys
	.global	gVendorBlkInfo
	.global	gL2pMapInfo
	.global	gSysFreeQueue
	.global	gSysInfo
	.global	gBbtInfo
	.global	g_MaxLbn
	.global	g_VaildLpn
	.global	g_MaxLpn
	.global	g_MaxLbaSector
	.global	g_GlobalDataVersion
	.global	g_GlobalSysVersion
	.global	ftl_gc_temp_power_lost_recovery_flag
	.global	c_ftl_nand_max_data_blks
	.global	c_ftl_nand_data_op_blks_per_plane
	.global	c_ftl_nand_data_blks_per_plane
	.global	c_ftl_nand_max_sys_blks
	.global	c_ftl_nand_init_sys_blks_per_plane
	.global	c_ftl_nand_sys_blks_per_plane
	.global	c_ftl_vendor_part_size
	.global	c_ftl_nand_max_vendor_blks
	.global	c_ftl_nand_max_map_blks
	.global	c_ftl_nand_map_blks_per_plane
	.global	c_ftl_nand_vendor_region_num
	.global	c_ftl_nand_l2pmap_ram_region_num
	.global	c_ftl_nand_map_region_num
	.global	c_ftl_nand_totle_phy_blks
	.global	c_ftl_nand_reserved_blks
	.global	c_ftl_nand_byte_pre_oob
	.global	c_ftl_nand_byte_pre_page
	.global	c_ftl_nand_sec_pre_page_shift
	.global	c_ftl_nand_sec_pre_page
	.global	c_ftl_nand_page_pre_super_blk
	.global	c_ftl_nand_page_pre_slc_blk
	.global	c_ftl_nand_page_pre_blk
	.global	c_ftl_nand_bbm_buf_size
	.global	c_ftl_nand_ext_blk_pre_plane
	.global	c_ftl_nand_blk_pre_plane
	.global	c_ftl_nand_planes_num
	.global	c_ftl_nand_blks_per_die_shift
	.global	c_ftl_nand_blks_per_die
	.global	c_ftl_nand_planes_per_die
	.global	c_ftl_nand_die_num
	.global	c_ftl_nand_type
	.section	.bss.DeviceCapacity,"aw",%nobits
	.align	2
	.set	.LANCHOR25,. + 0
	.type	DeviceCapacity, %object
	.size	DeviceCapacity, 4
DeviceCapacity:
	.space	4
	.section	.bss.FtlUpdateVaildLpnCount,"aw",%nobits
	.align	1
	.set	.LANCHOR57,. + 0
	.type	FtlUpdateVaildLpnCount, %object
	.size	FtlUpdateVaildLpnCount, 2
FtlUpdateVaildLpnCount:
	.space	2
	.section	.bss.c_ftl_nand_bbm_buf_size,"aw",%nobits
	.align	1
	.set	.LANCHOR126,. + 0
	.type	c_ftl_nand_bbm_buf_size, %object
	.size	c_ftl_nand_bbm_buf_size, 2
c_ftl_nand_bbm_buf_size:
	.space	2
	.section	.bss.c_ftl_nand_blk_pre_plane,"aw",%nobits
	.align	1
	.set	.LANCHOR5,. + 0
	.type	c_ftl_nand_blk_pre_plane, %object
	.size	c_ftl_nand_blk_pre_plane, 2
c_ftl_nand_blk_pre_plane:
	.space	2
	.section	.bss.c_ftl_nand_blks_per_die,"aw",%nobits
	.align	1
	.set	.LANCHOR16,. + 0
	.type	c_ftl_nand_blks_per_die, %object
	.size	c_ftl_nand_blks_per_die, 2
c_ftl_nand_blks_per_die:
	.space	2
	.section	.bss.c_ftl_nand_blks_per_die_shift,"aw",%nobits
	.align	1
	.set	.LANCHOR17,. + 0
	.type	c_ftl_nand_blks_per_die_shift, %object
	.size	c_ftl_nand_blks_per_die_shift, 2
c_ftl_nand_blks_per_die_shift:
	.space	2
	.section	.bss.c_ftl_nand_byte_pre_oob,"aw",%nobits
	.align	1
	.set	.LANCHOR23,. + 0
	.type	c_ftl_nand_byte_pre_oob, %object
	.size	c_ftl_nand_byte_pre_oob, 2
c_ftl_nand_byte_pre_oob:
	.space	2
	.section	.bss.c_ftl_nand_byte_pre_page,"aw",%nobits
	.align	1
	.set	.LANCHOR22,. + 0
	.type	c_ftl_nand_byte_pre_page, %object
	.size	c_ftl_nand_byte_pre_page, 2
c_ftl_nand_byte_pre_page:
	.space	2
	.section	.bss.c_ftl_nand_data_blks_per_plane,"aw",%nobits
	.align	1
	.set	.LANCHOR4,. + 0
	.type	c_ftl_nand_data_blks_per_plane, %object
	.size	c_ftl_nand_data_blks_per_plane, 2
c_ftl_nand_data_blks_per_plane:
	.space	2
	.section	.bss.c_ftl_nand_data_op_blks_per_plane,"aw",%nobits
	.align	1
	.set	.LANCHOR170,. + 0
	.type	c_ftl_nand_data_op_blks_per_plane, %object
	.size	c_ftl_nand_data_op_blks_per_plane, 2
c_ftl_nand_data_op_blks_per_plane:
	.space	2
	.section	.bss.c_ftl_nand_die_num,"aw",%nobits
	.align	1
	.set	.LANCHOR9,. + 0
	.type	c_ftl_nand_die_num, %object
	.size	c_ftl_nand_die_num, 2
c_ftl_nand_die_num:
	.space	2
	.section	.bss.c_ftl_nand_ext_blk_pre_plane,"aw",%nobits
	.align	1
	.set	.LANCHOR14,. + 0
	.type	c_ftl_nand_ext_blk_pre_plane, %object
	.size	c_ftl_nand_ext_blk_pre_plane, 2
c_ftl_nand_ext_blk_pre_plane:
	.space	2
	.section	.bss.c_ftl_nand_init_sys_blks_per_plane,"aw",%nobits
	.align	2
	.set	.LANCHOR30,. + 0
	.type	c_ftl_nand_init_sys_blks_per_plane, %object
	.size	c_ftl_nand_init_sys_blks_per_plane, 4
c_ftl_nand_init_sys_blks_per_plane:
	.space	4
	.section	.bss.c_ftl_nand_l2pmap_ram_region_num,"aw",%nobits
	.align	1
	.set	.LANCHOR32,. + 0
	.type	c_ftl_nand_l2pmap_ram_region_num, %object
	.size	c_ftl_nand_l2pmap_ram_region_num, 2
c_ftl_nand_l2pmap_ram_region_num:
	.space	2
	.section	.bss.c_ftl_nand_map_blks_per_plane,"aw",%nobits
	.align	1
	.set	.LANCHOR28,. + 0
	.type	c_ftl_nand_map_blks_per_plane, %object
	.size	c_ftl_nand_map_blks_per_plane, 2
c_ftl_nand_map_blks_per_plane:
	.space	2
	.section	.bss.c_ftl_nand_map_region_num,"aw",%nobits
	.align	1
	.set	.LANCHOR31,. + 0
	.type	c_ftl_nand_map_region_num, %object
	.size	c_ftl_nand_map_region_num, 2
c_ftl_nand_map_region_num:
	.space	2
	.section	.bss.c_ftl_nand_max_data_blks,"aw",%nobits
	.align	2
	.set	.LANCHOR6,. + 0
	.type	c_ftl_nand_max_data_blks, %object
	.size	c_ftl_nand_max_data_blks, 4
c_ftl_nand_max_data_blks:
	.space	4
	.section	.bss.c_ftl_nand_max_map_blks,"aw",%nobits
	.align	2
	.set	.LANCHOR29,. + 0
	.type	c_ftl_nand_max_map_blks, %object
	.size	c_ftl_nand_max_map_blks, 4
c_ftl_nand_max_map_blks:
	.space	4
	.section	.bss.c_ftl_nand_max_sys_blks,"aw",%nobits
	.align	2
	.set	.LANCHOR3,. + 0
	.type	c_ftl_nand_max_sys_blks, %object
	.size	c_ftl_nand_max_sys_blks, 4
c_ftl_nand_max_sys_blks:
	.space	4
	.section	.bss.c_ftl_nand_max_vendor_blks,"aw",%nobits
	.align	1
	.set	.LANCHOR26,. + 0
	.type	c_ftl_nand_max_vendor_blks, %object
	.size	c_ftl_nand_max_vendor_blks, 2
c_ftl_nand_max_vendor_blks:
	.space	2
	.section	.bss.c_ftl_nand_page_pre_blk,"aw",%nobits
	.align	1
	.set	.LANCHOR18,. + 0
	.type	c_ftl_nand_page_pre_blk, %object
	.size	c_ftl_nand_page_pre_blk, 2
c_ftl_nand_page_pre_blk:
	.space	2
	.section	.bss.c_ftl_nand_page_pre_slc_blk,"aw",%nobits
	.align	1
	.set	.LANCHOR19,. + 0
	.type	c_ftl_nand_page_pre_slc_blk, %object
	.size	c_ftl_nand_page_pre_slc_blk, 2
c_ftl_nand_page_pre_slc_blk:
	.space	2
	.section	.bss.c_ftl_nand_page_pre_super_blk,"aw",%nobits
	.align	1
	.set	.LANCHOR20,. + 0
	.type	c_ftl_nand_page_pre_super_blk, %object
	.size	c_ftl_nand_page_pre_super_blk, 2
c_ftl_nand_page_pre_super_blk:
	.space	2
	.section	.bss.c_ftl_nand_planes_num,"aw",%nobits
	.align	1
	.set	.LANCHOR2,. + 0
	.type	c_ftl_nand_planes_num, %object
	.size	c_ftl_nand_planes_num, 2
c_ftl_nand_planes_num:
	.space	2
	.section	.bss.c_ftl_nand_planes_per_die,"aw",%nobits
	.align	1
	.set	.LANCHOR10,. + 0
	.type	c_ftl_nand_planes_per_die, %object
	.size	c_ftl_nand_planes_per_die, 2
c_ftl_nand_planes_per_die:
	.space	2
	.section	.bss.c_ftl_nand_reserved_blks,"aw",%nobits
	.align	1
	.set	.LANCHOR24,. + 0
	.type	c_ftl_nand_reserved_blks, %object
	.size	c_ftl_nand_reserved_blks, 2
c_ftl_nand_reserved_blks:
	.space	2
	.section	.bss.c_ftl_nand_sec_pre_page,"aw",%nobits
	.align	1
	.set	.LANCHOR11,. + 0
	.type	c_ftl_nand_sec_pre_page, %object
	.size	c_ftl_nand_sec_pre_page, 2
c_ftl_nand_sec_pre_page:
	.space	2
	.section	.bss.c_ftl_nand_sec_pre_page_shift,"aw",%nobits
	.align	1
	.set	.LANCHOR21,. + 0
	.type	c_ftl_nand_sec_pre_page_shift, %object
	.size	c_ftl_nand_sec_pre_page_shift, 2
c_ftl_nand_sec_pre_page_shift:
	.space	2
	.section	.bss.c_ftl_nand_sys_blks_per_plane,"aw",%nobits
	.align	2
	.set	.LANCHOR1,. + 0
	.type	c_ftl_nand_sys_blks_per_plane, %object
	.size	c_ftl_nand_sys_blks_per_plane, 4
c_ftl_nand_sys_blks_per_plane:
	.space	4
	.section	.bss.c_ftl_nand_totle_phy_blks,"aw",%nobits
	.align	2
	.set	.LANCHOR7,. + 0
	.type	c_ftl_nand_totle_phy_blks, %object
	.size	c_ftl_nand_totle_phy_blks, 4
c_ftl_nand_totle_phy_blks:
	.space	4
	.section	.bss.c_ftl_nand_type,"aw",%nobits
	.align	1
	.set	.LANCHOR8,. + 0
	.type	c_ftl_nand_type, %object
	.size	c_ftl_nand_type, 2
c_ftl_nand_type:
	.space	2
	.section	.bss.c_ftl_nand_vendor_region_num,"aw",%nobits
	.align	1
	.set	.LANCHOR27,. + 0
	.type	c_ftl_nand_vendor_region_num, %object
	.size	c_ftl_nand_vendor_region_num, 2
c_ftl_nand_vendor_region_num:
	.space	2
	.section	.bss.c_ftl_vendor_part_size,"aw",%nobits
	.align	1
	.set	.LANCHOR15,. + 0
	.type	c_ftl_vendor_part_size, %object
	.size	c_ftl_vendor_part_size, 2
c_ftl_vendor_part_size:
	.space	2
	.section	.bss.c_gc_page_buf_num,"aw",%nobits
	.align	2
	.set	.LANCHOR66,. + 0
	.type	c_gc_page_buf_num, %object
	.size	c_gc_page_buf_num, 4
c_gc_page_buf_num:
	.space	4
	.section	.bss.c_mlc_erase_count_value,"aw",%nobits
	.align	1
	.set	.LANCHOR13,. + 0
	.type	c_mlc_erase_count_value, %object
	.size	c_mlc_erase_count_value, 2
c_mlc_erase_count_value:
	.space	2
	.section	.bss.check_vpc_table,"aw",%nobits
	.align	1
	.type	check_vpc_table, %object
	.size	check_vpc_table, 16384
check_vpc_table:
	.space	16384
	.section	.bss.ftl_gc_temp_power_lost_recovery_flag,"aw",%nobits
	.align	2
	.set	.LANCHOR132,. + 0
	.type	ftl_gc_temp_power_lost_recovery_flag, %object
	.size	ftl_gc_temp_power_lost_recovery_flag, 4
ftl_gc_temp_power_lost_recovery_flag:
	.space	4
	.section	.bss.gBbtInfo,"aw",%nobits
	.align	2
	.set	.LANCHOR36,. + 0
	.type	gBbtInfo, %object
	.size	gBbtInfo, 60
gBbtInfo:
	.space	60
	.section	.bss.gL2pMapInfo,"aw",%nobits
	.align	2
	.set	.LANCHOR128,. + 0
	.type	gL2pMapInfo, %object
	.size	gL2pMapInfo, 44
gL2pMapInfo:
	.space	44
	.section	.bss.gSysFreeQueue,"aw",%nobits
	.align	1
	.set	.LANCHOR37,. + 0
	.type	gSysFreeQueue, %object
	.size	gSysFreeQueue, 2056
gSysFreeQueue:
	.space	2056
	.section	.bss.gSysInfo,"aw",%nobits
	.align	2
	.set	.LANCHOR138,. + 0
	.type	gSysInfo, %object
	.size	gSysInfo, 16
gSysInfo:
	.space	16
	.section	.bss.gVendorBlkInfo,"aw",%nobits
	.align	2
	.set	.LANCHOR160,. + 0
	.type	gVendorBlkInfo, %object
	.size	gVendorBlkInfo, 44
gVendorBlkInfo:
	.space	44
	.section	.bss.g_GlobalDataVersion,"aw",%nobits
	.align	2
	.set	.LANCHOR82,. + 0
	.type	g_GlobalDataVersion, %object
	.size	g_GlobalDataVersion, 4
g_GlobalDataVersion:
	.space	4
	.section	.bss.g_GlobalSysVersion,"aw",%nobits
	.align	2
	.set	.LANCHOR81,. + 0
	.type	g_GlobalSysVersion, %object
	.size	g_GlobalSysVersion, 4
g_GlobalSysVersion:
	.space	4
	.section	.bss.g_MaxLbaSector,"aw",%nobits
	.align	2
	.set	.LANCHOR33,. + 0
	.type	g_MaxLbaSector, %object
	.size	g_MaxLbaSector, 4
g_MaxLbaSector:
	.space	4
	.section	.bss.g_MaxLbn,"aw",%nobits
	.align	2
	.set	.LANCHOR178,. + 0
	.type	g_MaxLbn, %object
	.size	g_MaxLbn, 4
g_MaxLbn:
	.space	4
	.section	.bss.g_MaxLpn,"aw",%nobits
	.align	2
	.set	.LANCHOR151,. + 0
	.type	g_MaxLpn, %object
	.size	g_MaxLpn, 4
g_MaxLpn:
	.space	4
	.section	.bss.g_VaildLpn,"aw",%nobits
	.align	2
	.set	.LANCHOR58,. + 0
	.type	g_VaildLpn, %object
	.size	g_VaildLpn, 4
g_VaildLpn:
	.space	4
	.section	.bss.g_active_superblock,"aw",%nobits
	.align	2
	.set	.LANCHOR50,. + 0
	.type	g_active_superblock, %object
	.size	g_active_superblock, 48
g_active_superblock:
	.space	48
	.section	.bss.g_buffer_superblock,"aw",%nobits
	.align	2
	.set	.LANCHOR51,. + 0
	.type	g_buffer_superblock, %object
	.size	g_buffer_superblock, 48
g_buffer_superblock:
	.space	48
	.section	.bss.g_cur_erase_blk,"aw",%nobits
	.align	2
	.set	.LANCHOR98,. + 0
	.type	g_cur_erase_blk, %object
	.size	g_cur_erase_blk, 4
g_cur_erase_blk:
	.space	4
	.section	.bss.g_ect_tbl_info_size,"aw",%nobits
	.align	1
	.set	.LANCHOR115,. + 0
	.type	g_ect_tbl_info_size, %object
	.size	g_ect_tbl_info_size, 2
g_ect_tbl_info_size:
	.space	2
	.section	.bss.g_ect_tbl_power_up_flush,"aw",%nobits
	.align	1
	.set	.LANCHOR163,. + 0
	.type	g_ect_tbl_power_up_flush, %object
	.size	g_ect_tbl_power_up_flush, 2
g_ect_tbl_power_up_flush:
	.space	2
	.section	.bss.g_ftl_nand_free_count,"aw",%nobits
	.align	2
	.set	.LANCHOR175,. + 0
	.type	g_ftl_nand_free_count, %object
	.size	g_ftl_nand_free_count, 4
g_ftl_nand_free_count:
	.space	4
	.section	.bss.g_gc_bad_block_gc_index,"aw",%nobits
	.align	1
	.set	.LANCHOR75,. + 0
	.type	g_gc_bad_block_gc_index, %object
	.size	g_gc_bad_block_gc_index, 2
g_gc_bad_block_gc_index:
	.space	2
	.section	.bss.g_gc_bad_block_temp_num,"aw",%nobits
	.align	1
	.set	.LANCHOR73,. + 0
	.type	g_gc_bad_block_temp_num, %object
	.size	g_gc_bad_block_temp_num, 2
g_gc_bad_block_temp_num:
	.space	2
	.section	.bss.g_gc_bad_block_temp_tbl,"aw",%nobits
	.align	1
	.set	.LANCHOR74,. + 0
	.type	g_gc_bad_block_temp_tbl, %object
	.size	g_gc_bad_block_temp_tbl, 34
g_gc_bad_block_temp_tbl:
	.space	34
	.section	.bss.g_gc_blk_index,"aw",%nobits
	.align	1
	.set	.LANCHOR101,. + 0
	.type	g_gc_blk_index, %object
	.size	g_gc_blk_index, 2
g_gc_blk_index:
	.space	2
	.section	.bss.g_gc_blk_num,"aw",%nobits
	.align	1
	.set	.LANCHOR68,. + 0
	.type	g_gc_blk_num, %object
	.size	g_gc_blk_num, 2
g_gc_blk_num:
	.space	2
	.section	.bss.g_gc_cur_blk_max_valid_pages,"aw",%nobits
	.align	1
	.set	.LANCHOR173,. + 0
	.type	g_gc_cur_blk_max_valid_pages, %object
	.size	g_gc_cur_blk_max_valid_pages, 2
g_gc_cur_blk_max_valid_pages:
	.space	2
	.section	.bss.g_gc_cur_blk_valid_pages,"aw",%nobits
	.align	1
	.set	.LANCHOR172,. + 0
	.type	g_gc_cur_blk_valid_pages, %object
	.size	g_gc_cur_blk_valid_pages, 2
g_gc_cur_blk_valid_pages:
	.space	2
	.section	.bss.g_gc_free_blk_threshold,"aw",%nobits
	.align	1
	.set	.LANCHOR99,. + 0
	.type	g_gc_free_blk_threshold, %object
	.size	g_gc_free_blk_threshold, 2
g_gc_free_blk_threshold:
	.space	2
	.section	.bss.g_gc_head_data_block,"aw",%nobits
	.align	2
	.set	.LANCHOR95,. + 0
	.type	g_gc_head_data_block, %object
	.size	g_gc_head_data_block, 4
g_gc_head_data_block:
	.space	4
	.section	.bss.g_gc_head_data_block_count,"aw",%nobits
	.align	2
	.set	.LANCHOR96,. + 0
	.type	g_gc_head_data_block_count, %object
	.size	g_gc_head_data_block_count, 4
g_gc_head_data_block_count:
	.space	4
	.section	.bss.g_gc_merge_free_blk_threshold,"aw",%nobits
	.align	1
	.set	.LANCHOR100,. + 0
	.type	g_gc_merge_free_blk_threshold, %object
	.size	g_gc_merge_free_blk_threshold, 2
g_gc_merge_free_blk_threshold:
	.space	2
	.section	.bss.g_gc_next_blk,"aw",%nobits
	.align	1
	.set	.LANCHOR71,. + 0
	.type	g_gc_next_blk, %object
	.size	g_gc_next_blk, 2
g_gc_next_blk:
	.space	2
	.section	.bss.g_gc_next_blk_1,"aw",%nobits
	.align	1
	.set	.LANCHOR72,. + 0
	.type	g_gc_next_blk_1, %object
	.size	g_gc_next_blk_1, 2
g_gc_next_blk_1:
	.space	2
	.section	.bss.g_gc_num_req,"aw",%nobits
	.align	2
	.set	.LANCHOR61,. + 0
	.type	g_gc_num_req, %object
	.size	g_gc_num_req, 4
g_gc_num_req:
	.space	4
	.section	.bss.g_gc_page_offset,"aw",%nobits
	.align	1
	.set	.LANCHOR69,. + 0
	.type	g_gc_page_offset, %object
	.size	g_gc_page_offset, 2
g_gc_page_offset:
	.space	2
	.section	.bss.g_gc_skip_write_count,"aw",%nobits
	.align	2
	.set	.LANCHOR97,. + 0
	.type	g_gc_skip_write_count, %object
	.size	g_gc_skip_write_count, 4
g_gc_skip_write_count:
	.space	4
	.section	.bss.g_gc_superblock,"aw",%nobits
	.align	2
	.set	.LANCHOR144,. + 0
	.type	g_gc_superblock, %object
	.size	g_gc_superblock, 48
g_gc_superblock:
	.space	48
	.section	.bss.g_gc_temp_superblock,"aw",%nobits
	.align	2
	.set	.LANCHOR52,. + 0
	.type	g_gc_temp_superblock, %object
	.size	g_gc_temp_superblock, 48
g_gc_temp_superblock:
	.space	48
	.section	.bss.g_in_gc_progress,"aw",%nobits
	.align	2
	.set	.LANCHOR93,. + 0
	.type	g_in_gc_progress, %object
	.size	g_in_gc_progress, 4
g_in_gc_progress:
	.space	4
	.section	.bss.g_in_swl_replace,"aw",%nobits
	.align	2
	.set	.LANCHOR94,. + 0
	.type	g_in_swl_replace, %object
	.size	g_in_swl_replace, 4
g_in_swl_replace:
	.space	4
	.section	.bss.g_l2p_last_update_region_id,"aw",%nobits
	.align	1
	.set	.LANCHOR55,. + 0
	.type	g_l2p_last_update_region_id, %object
	.size	g_l2p_last_update_region_id, 2
g_l2p_last_update_region_id:
	.space	2
	.section	.bss.g_max_erase_count,"aw",%nobits
	.align	2
	.set	.LANCHOR91,. + 0
	.type	g_max_erase_count, %object
	.size	g_max_erase_count, 4
g_max_erase_count:
	.space	4
	.section	.bss.g_min_erase_count,"aw",%nobits
	.align	2
	.set	.LANCHOR92,. + 0
	.type	g_min_erase_count, %object
	.size	g_min_erase_count, 4
g_min_erase_count:
	.space	4
	.section	.bss.g_nand_ops,"aw",%nobits
	.align	2
	.set	.LANCHOR77,. + 0
	.type	g_nand_ops, %object
	.size	g_nand_ops, 24
g_nand_ops:
	.space	24
	.section	.bss.g_nand_phy_info,"aw",%nobits
	.align	1
	.set	.LANCHOR0,. + 0
	.type	g_nand_phy_info, %object
	.size	g_nand_phy_info, 24
g_nand_phy_info:
	.space	24
	.section	.bss.g_num_data_superblocks,"aw",%nobits
	.align	1
	.set	.LANCHOR43,. + 0
	.type	g_num_data_superblocks, %object
	.size	g_num_data_superblocks, 2
g_num_data_superblocks:
	.space	2
	.section	.bss.g_num_free_superblocks,"aw",%nobits
	.align	1
	.set	.LANCHOR47,. + 0
	.type	g_num_free_superblocks, %object
	.size	g_num_free_superblocks, 2
g_num_free_superblocks:
	.space	2
	.section	.bss.g_power_lost_recovery_flag,"aw",%nobits
	.align	1
	.set	.LANCHOR155,. + 0
	.type	g_power_lost_recovery_flag, %object
	.size	g_power_lost_recovery_flag, 2
g_power_lost_recovery_flag:
	.space	2
	.section	.bss.g_recovery_page_min_ver,"aw",%nobits
	.align	2
	.set	.LANCHOR133,. + 0
	.type	g_recovery_page_min_ver, %object
	.size	g_recovery_page_min_ver, 4
g_recovery_page_min_ver:
	.space	4
	.section	.bss.g_recovery_page_num,"aw",%nobits
	.align	2
	.set	.LANCHOR156,. + 0
	.type	g_recovery_page_num, %object
	.size	g_recovery_page_num, 4
g_recovery_page_num:
	.space	4
	.section	.bss.g_recovery_ppa_tbl,"aw",%nobits
	.align	2
	.set	.LANCHOR157,. + 0
	.type	g_recovery_ppa_tbl, %object
	.size	g_recovery_ppa_tbl, 128
g_recovery_ppa_tbl:
	.space	128
	.section	.bss.g_sys_ext_data,"aw",%nobits
	.align	2
	.set	.LANCHOR134,. + 0
	.type	g_sys_ext_data, %object
	.size	g_sys_ext_data, 512
g_sys_ext_data:
	.space	512
	.section	.bss.g_sys_save_data,"aw",%nobits
	.align	2
	.set	.LANCHOR38,. + 0
	.type	g_sys_save_data, %object
	.size	g_sys_save_data, 48
g_sys_save_data:
	.space	48
	.section	.bss.g_tmp_data_superblock_id,"aw",%nobits
	.align	1
	.set	.LANCHOR130,. + 0
	.type	g_tmp_data_superblock_id, %object
	.size	g_tmp_data_superblock_id, 2
g_tmp_data_superblock_id:
	.space	2
	.section	.bss.g_totle_avg_erase_count,"aw",%nobits
	.align	2
	.set	.LANCHOR142,. + 0
	.type	g_totle_avg_erase_count, %object
	.size	g_totle_avg_erase_count, 4
g_totle_avg_erase_count:
	.space	4
	.section	.bss.g_totle_cache_write_count,"aw",%nobits
	.align	2
	.set	.LANCHOR86,. + 0
	.type	g_totle_cache_write_count, %object
	.size	g_totle_cache_write_count, 4
g_totle_cache_write_count:
	.space	4
	.section	.bss.g_totle_discard_page_count,"aw",%nobits
	.align	2
	.set	.LANCHOR85,. + 0
	.type	g_totle_discard_page_count, %object
	.size	g_totle_discard_page_count, 4
g_totle_discard_page_count:
	.space	4
	.section	.bss.g_totle_gc_page_count,"aw",%nobits
	.align	2
	.set	.LANCHOR83,. + 0
	.type	g_totle_gc_page_count, %object
	.size	g_totle_gc_page_count, 4
g_totle_gc_page_count:
	.space	4
	.section	.bss.g_totle_l2p_write_count,"aw",%nobits
	.align	2
	.set	.LANCHOR87,. + 0
	.type	g_totle_l2p_write_count, %object
	.size	g_totle_l2p_write_count, 4
g_totle_l2p_write_count:
	.space	4
	.section	.bss.g_totle_map_block,"aw",%nobits
	.align	1
	.set	.LANCHOR129,. + 0
	.type	g_totle_map_block, %object
	.size	g_totle_map_block, 2
g_totle_map_block:
	.space	2
	.section	.bss.g_totle_mlc_erase_count,"aw",%nobits
	.align	2
	.set	.LANCHOR89,. + 0
	.type	g_totle_mlc_erase_count, %object
	.size	g_totle_mlc_erase_count, 4
g_totle_mlc_erase_count:
	.space	4
	.section	.bss.g_totle_read_page_count,"aw",%nobits
	.align	2
	.set	.LANCHOR88,. + 0
	.type	g_totle_read_page_count, %object
	.size	g_totle_read_page_count, 4
g_totle_read_page_count:
	.space	4
	.section	.bss.g_totle_read_sector,"aw",%nobits
	.align	2
	.set	.LANCHOR162,. + 0
	.type	g_totle_read_sector, %object
	.size	g_totle_read_sector, 4
g_totle_read_sector:
	.space	4
	.section	.bss.g_totle_slc_erase_count,"aw",%nobits
	.align	2
	.set	.LANCHOR90,. + 0
	.type	g_totle_slc_erase_count, %object
	.size	g_totle_slc_erase_count, 4
g_totle_slc_erase_count:
	.space	4
	.section	.bss.g_totle_swl_count,"aw",%nobits
	.align	2
	.set	.LANCHOR131,. + 0
	.type	g_totle_swl_count, %object
	.size	g_totle_swl_count, 4
g_totle_swl_count:
	.space	4
	.section	.bss.g_totle_sys_slc_erase_count,"aw",%nobits
	.align	2
	.set	.LANCHOR79,. + 0
	.type	g_totle_sys_slc_erase_count, %object
	.size	g_totle_sys_slc_erase_count, 4
g_totle_sys_slc_erase_count:
	.space	4
	.section	.bss.g_totle_vendor_block,"aw",%nobits
	.align	1
	.set	.LANCHOR34,. + 0
	.type	g_totle_vendor_block, %object
	.size	g_totle_vendor_block, 2
g_totle_vendor_block:
	.space	2
	.section	.bss.g_totle_write_page_count,"aw",%nobits
	.align	2
	.set	.LANCHOR84,. + 0
	.type	g_totle_write_page_count, %object
	.size	g_totle_write_page_count, 4
g_totle_write_page_count:
	.space	4
	.section	.bss.g_totle_write_sector,"aw",%nobits
	.align	2
	.set	.LANCHOR161,. + 0
	.type	g_totle_write_sector, %object
	.size	g_totle_write_sector, 4
g_totle_write_sector:
	.space	4
	.section	.bss.gc_discard_updated,"aw",%nobits
	.align	2
	.set	.LANCHOR166,. + 0
	.type	gc_discard_updated, %object
	.size	gc_discard_updated, 4
gc_discard_updated:
	.space	4
	.section	.bss.gc_ink_free_return_value,"aw",%nobits
	.align	1
	.set	.LANCHOR171,. + 0
	.type	gc_ink_free_return_value, %object
	.size	gc_ink_free_return_value, 2
gc_ink_free_return_value:
	.space	2
	.section	.bss.gp_ect_tbl_info,"aw",%nobits
	.align	2
	.set	.LANCHOR117,. + 0
	.type	gp_ect_tbl_info, %object
	.size	gp_ect_tbl_info, 4
gp_ect_tbl_info:
	.space	4
	.section	.bss.gp_flash_check_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR183,. + 0
	.type	gp_flash_check_buf, %object
	.size	gp_flash_check_buf, 4
gp_flash_check_buf:
	.space	4
	.section	.bss.gp_gc_page_buf_info,"aw",%nobits
	.align	2
	.set	.LANCHOR62,. + 0
	.type	gp_gc_page_buf_info, %object
	.size	gp_gc_page_buf_info, 4
gp_gc_page_buf_info:
	.space	4
	.section	.bss.gp_last_act_superblock,"aw",%nobits
	.align	2
	.type	gp_last_act_superblock, %object
	.size	gp_last_act_superblock, 4
gp_last_act_superblock:
	.space	4
	.section	.bss.idb_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR182,. + 0
	.type	idb_buf, %object
	.size	idb_buf, 4
idb_buf:
	.space	4
	.section	.bss.idb_need_write_back,"aw",%nobits
	.align	2
	.set	.LANCHOR181,. + 0
	.type	idb_need_write_back, %object
	.size	idb_need_write_back, 4
idb_need_write_back:
	.space	4
	.section	.bss.low_format_en,"aw",%nobits
	.align	2
	.set	.LANCHOR180,. + 0
	.type	low_format_en, %object
	.size	low_format_en, 4
low_format_en:
	.space	4
	.section	.bss.p_data_block_list_head,"aw",%nobits
	.align	2
	.set	.LANCHOR40,. + 0
	.type	p_data_block_list_head, %object
	.size	p_data_block_list_head, 4
p_data_block_list_head:
	.space	4
	.section	.bss.p_data_block_list_table,"aw",%nobits
	.align	2
	.set	.LANCHOR39,. + 0
	.type	p_data_block_list_table, %object
	.size	p_data_block_list_table, 4
p_data_block_list_table:
	.space	4
	.section	.bss.p_data_block_list_tail,"aw",%nobits
	.align	2
	.set	.LANCHOR42,. + 0
	.type	p_data_block_list_tail, %object
	.size	p_data_block_list_tail, 4
p_data_block_list_tail:
	.space	4
	.section	.bss.p_erase_count_table,"aw",%nobits
	.align	2
	.set	.LANCHOR46,. + 0
	.type	p_erase_count_table, %object
	.size	p_erase_count_table, 4
p_erase_count_table:
	.space	4
	.section	.bss.p_free_data_block_list_head,"aw",%nobits
	.align	2
	.set	.LANCHOR45,. + 0
	.type	p_free_data_block_list_head, %object
	.size	p_free_data_block_list_head, 4
p_free_data_block_list_head:
	.space	4
	.section	.bss.p_gc_blk_tbl,"aw",%nobits
	.align	2
	.set	.LANCHOR67,. + 0
	.type	p_gc_blk_tbl, %object
	.size	p_gc_blk_tbl, 4
p_gc_blk_tbl:
	.space	4
	.section	.bss.p_gc_data_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR63,. + 0
	.type	p_gc_data_buf, %object
	.size	p_gc_data_buf, 4
p_gc_data_buf:
	.space	4
	.section	.bss.p_gc_page_info,"aw",%nobits
	.align	2
	.set	.LANCHOR70,. + 0
	.type	p_gc_page_info, %object
	.size	p_gc_page_info, 4
p_gc_page_info:
	.space	4
	.section	.bss.p_gc_spare_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR64,. + 0
	.type	p_gc_spare_buf, %object
	.size	p_gc_spare_buf, 4
p_gc_spare_buf:
	.space	4
	.section	.bss.p_io_data_buf_0,"aw",%nobits
	.align	2
	.set	.LANCHOR108,. + 0
	.type	p_io_data_buf_0, %object
	.size	p_io_data_buf_0, 4
p_io_data_buf_0:
	.space	4
	.section	.bss.p_io_data_buf_1,"aw",%nobits
	.align	2
	.set	.LANCHOR109,. + 0
	.type	p_io_data_buf_1, %object
	.size	p_io_data_buf_1, 4
p_io_data_buf_1:
	.space	4
	.section	.bss.p_io_spare_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR114,. + 0
	.type	p_io_spare_buf, %object
	.size	p_io_spare_buf, 4
p_io_spare_buf:
	.space	4
	.section	.bss.p_l2p_map_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR125,. + 0
	.type	p_l2p_map_buf, %object
	.size	p_l2p_map_buf, 4
p_l2p_map_buf:
	.space	4
	.section	.bss.p_l2p_ram_map,"aw",%nobits
	.align	2
	.set	.LANCHOR54,. + 0
	.type	p_l2p_ram_map, %object
	.size	p_l2p_ram_map, 4
p_l2p_ram_map:
	.space	4
	.section	.bss.p_map_block_table,"aw",%nobits
	.align	2
	.set	.LANCHOR118,. + 0
	.type	p_map_block_table, %object
	.size	p_map_block_table, 4
p_map_block_table:
	.space	4
	.section	.bss.p_map_block_valid_page_count,"aw",%nobits
	.align	2
	.set	.LANCHOR119,. + 0
	.type	p_map_block_valid_page_count, %object
	.size	p_map_block_valid_page_count, 4
p_map_block_valid_page_count:
	.space	4
	.section	.bss.p_map_block_ver_table,"aw",%nobits
	.align	2
	.set	.LANCHOR124,. + 0
	.type	p_map_block_ver_table, %object
	.size	p_map_block_ver_table, 4
p_map_block_ver_table:
	.space	4
	.section	.bss.p_map_region_ppn_table,"aw",%nobits
	.align	2
	.set	.LANCHOR123,. + 0
	.type	p_map_region_ppn_table, %object
	.size	p_map_region_ppn_table, 4
p_map_region_ppn_table:
	.space	4
	.section	.bss.p_plane_order_table,"aw",%nobits
	.set	.LANCHOR12,. + 0
	.type	p_plane_order_table, %object
	.size	p_plane_order_table, 32
p_plane_order_table:
	.space	32
	.section	.bss.p_swl_mul_table,"aw",%nobits
	.align	2
	.set	.LANCHOR116,. + 0
	.type	p_swl_mul_table, %object
	.size	p_swl_mul_table, 4
p_swl_mul_table:
	.space	4
	.section	.bss.p_sys_data_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR105,. + 0
	.type	p_sys_data_buf, %object
	.size	p_sys_data_buf, 4
p_sys_data_buf:
	.space	4
	.section	.bss.p_sys_data_buf_1,"aw",%nobits
	.align	2
	.set	.LANCHOR106,. + 0
	.type	p_sys_data_buf_1, %object
	.size	p_sys_data_buf_1, 4
p_sys_data_buf_1:
	.space	4
	.section	.bss.p_sys_spare_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR113,. + 0
	.type	p_sys_spare_buf, %object
	.size	p_sys_spare_buf, 4
p_sys_spare_buf:
	.space	4
	.section	.bss.p_valid_page_count_check_table,"aw",%nobits
	.align	2
	.type	p_valid_page_count_check_table, %object
	.size	p_valid_page_count_check_table, 4
p_valid_page_count_check_table:
	.space	4
	.section	.bss.p_valid_page_count_table,"aw",%nobits
	.align	2
	.set	.LANCHOR41,. + 0
	.type	p_valid_page_count_table, %object
	.size	p_valid_page_count_table, 4
p_valid_page_count_table:
	.space	4
	.section	.bss.p_vendor_block_table,"aw",%nobits
	.align	2
	.set	.LANCHOR35,. + 0
	.type	p_vendor_block_table, %object
	.size	p_vendor_block_table, 4
p_vendor_block_table:
	.space	4
	.section	.bss.p_vendor_block_valid_page_count,"aw",%nobits
	.align	2
	.set	.LANCHOR120,. + 0
	.type	p_vendor_block_valid_page_count, %object
	.size	p_vendor_block_valid_page_count, 4
p_vendor_block_valid_page_count:
	.space	4
	.section	.bss.p_vendor_block_ver_table,"aw",%nobits
	.align	2
	.set	.LANCHOR121,. + 0
	.type	p_vendor_block_ver_table, %object
	.size	p_vendor_block_ver_table, 4
p_vendor_block_ver_table:
	.space	4
	.section	.bss.p_vendor_data_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR107,. + 0
	.type	p_vendor_data_buf, %object
	.size	p_vendor_data_buf, 4
p_vendor_data_buf:
	.space	4
	.section	.bss.p_vendor_region_ppn_table,"aw",%nobits
	.align	2
	.set	.LANCHOR122,. + 0
	.type	p_vendor_region_ppn_table, %object
	.size	p_vendor_region_ppn_table, 4
p_vendor_region_ppn_table:
	.space	4
	.section	.bss.req_erase,"aw",%nobits
	.align	2
	.set	.LANCHOR78,. + 0
	.type	req_erase, %object
	.size	req_erase, 4
req_erase:
	.space	4
	.section	.bss.req_gc,"aw",%nobits
	.align	2
	.set	.LANCHOR65,. + 0
	.type	req_gc, %object
	.size	req_gc, 4
req_gc:
	.space	4
	.section	.bss.req_gc_dst,"aw",%nobits
	.align	2
	.set	.LANCHOR103,. + 0
	.type	req_gc_dst, %object
	.size	req_gc_dst, 4
req_gc_dst:
	.space	4
	.section	.bss.req_prgm,"aw",%nobits
	.align	2
	.set	.LANCHOR104,. + 0
	.type	req_prgm, %object
	.size	req_prgm, 4
req_prgm:
	.space	4
	.section	.bss.req_read,"aw",%nobits
	.align	2
	.set	.LANCHOR102,. + 0
	.type	req_read, %object
	.size	req_read, 4
req_read:
	.space	4
	.section	.bss.req_sys,"aw",%nobits
	.align	2
	.set	.LANCHOR137,. + 0
	.type	req_sys, %object
	.size	req_sys, 20
req_sys:
	.space	20
	.section	.bss.sftl_nand_check_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR110,. + 0
	.type	sftl_nand_check_buf, %object
	.size	sftl_nand_check_buf, 4
sftl_nand_check_buf:
	.space	4
	.section	.bss.sftl_nand_check_spare_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR112,. + 0
	.type	sftl_nand_check_spare_buf, %object
	.size	sftl_nand_check_spare_buf, 4
sftl_nand_check_spare_buf:
	.space	4
	.section	.bss.sftl_temp_buf,"aw",%nobits
	.align	2
	.set	.LANCHOR111,. + 0
	.type	sftl_temp_buf, %object
	.size	sftl_temp_buf, 4
sftl_temp_buf:
	.space	4
	.section	.data.ftl_gc_temp_block_bops_scan_page_addr,"aw",%progbits
	.align	1
	.set	.LANCHOR159,. + 0
	.type	ftl_gc_temp_block_bops_scan_page_addr, %object
	.size	ftl_gc_temp_block_bops_scan_page_addr, 2
ftl_gc_temp_block_bops_scan_page_addr:
	.short	-1
	.section	.data.gFtlInitStatus,"aw",%progbits
	.align	2
	.set	.LANCHOR165,. + 0
	.type	gFtlInitStatus, %object
	.size	gFtlInitStatus, 4
gFtlInitStatus:
	.word	-1
	.section	.data.power_up_flag,"aw",%progbits
	.align	2
	.type	power_up_flag, %object
	.size	power_up_flag, 4
power_up_flag:
	.word	1
	.section	.rodata.FlashEraseBlocks.str1.1,"aMS",%progbits,1
.LC6:
	.ascii	"%s: addr: %x is in id block!!!!!!!!!!\012\000"
.LC7:
	.ascii	"not free: w: d:\000"
.LC8:
	.ascii	"not free: w: s:\000"
	.section	.rodata.FlashProgPages.str1.1,"aMS",%progbits,1
.LC14:
	.ascii	"prog read error: = %x\012\000"
.LC15:
	.ascii	"prog read REFRESH: = %x\012\000"
.LC16:
	.ascii	"prog read s error: = %x %x %x %x %x\012\000"
.LC17:
	.ascii	"prog read d error: = %x %x %x %x %x\012\000"
	.section	.rodata.FlashReadPages.str1.1,"aMS",%progbits,1
.LC11:
	.ascii	"FlashReadPages %x %x error_ecc_bits %d\012\000"
.LC12:
	.ascii	"data:\000"
.LC13:
	.ascii	"spare:\000"
	.section	.rodata.FtlBbmMapBadBlock.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"phyBlk = 0x%x die = %d block_in_die = 0x%x 0x%8x\012"
	.ascii	"\000"
	.section	.rodata.FtlBbmTblFlush.str1.1,"aMS",%progbits,1
.LC18:
	.ascii	"FtlBbmTblFlush id=%x,page=%x,previd=%x cnt=%d\012\000"
.LC19:
	.ascii	"FtlBbmTblFlush error:%x\012\000"
.LC20:
	.ascii	"FtlBbmTblFlush error = %x error count = %d\012\000"
	.section	.rodata.FtlFreeSysBlkQueueOut.str1.1,"aMS",%progbits,1
.LC9:
	.ascii	"FtlFreeSysBlkQueueOut = %x, free count = %d, error\012"
	.ascii	"\000"
	.section	.rodata.FtlGcScanTempBlk.str1.1,"aMS",%progbits,1
.LC35:
	.ascii	"FtlGcScanTempBlk Error ID %x %x!!!!!!! \012\000"
	.section	.rodata.FtlLoadEctTbl.str1.1,"aMS",%progbits,1
.LC37:
	.ascii	"no ect\000"
.LC38:
	.ascii	"%s\012\000"
	.section	.rodata.FtlLoadSysInfo.str1.1,"aMS",%progbits,1
.LC46:
	.ascii	"%s hash error this.id =%x page =%x pre_id =%x hash "
	.ascii	"=%x hash_r =%x\012\000"
	.section	.rodata.FtlMapTblRecovery.str1.1,"aMS",%progbits,1
.LC47:
	.ascii	"%s last blk_id =%x page =%x hash error hash =%x has"
	.ascii	"h_r =%x\012\000"
.LC48:
	.ascii	"%s scan blk_id =%x page =%x hash error hash =%x has"
	.ascii	"h_r =%x\012\000"
	.section	.rodata.FtlMapWritePage.str1.1,"aMS",%progbits,1
.LC22:
	.ascii	"FtlMapWritePage error = %x \012\000"
.LC23:
	.ascii	"FtlMapWritePage error = %x error count = %d\012\000"
	.section	.rodata.FtlProgPages.str1.1,"aMS",%progbits,1
.LC40:
	.ascii	"FtlProgPages error %x = %d\012\000"
.LC41:
	.ascii	"Ftlwrite decrement_vpc_count %x = %d\012\000"
	.section	.rodata.FtlRecoverySuperblock.str1.1,"aMS",%progbits,1
.LC27:
	.ascii	"data prev_ppa = %x error...................\012\000"
.LC28:
	.ascii	"spuer block %x vpn is 0\012 \000"
	.section	.rodata.FtlVendorPartRead.str1.1,"aMS",%progbits,1
.LC36:
	.ascii	"FtlVendorPartRead refresh = %x phyAddr = %x\012\000"
	.section	.rodata.FtlVpcTblFlush.str1.1,"aMS",%progbits,1
.LC39:
	.ascii	"FtlVpcTblFlush error = %x error count = %d\012\000"
	.section	.rodata.FtlWrite.str1.1,"aMS",%progbits,1
.LC44:
	.ascii	"FtlWrite: ecc error:%x %x %x\012\000"
.LC45:
	.ascii	"FtlWrite: lpa error:%x %x\012\000"
	.section	.rodata.FtlWriteToIDB.str1.1,"aMS",%progbits,1
.LC50:
	.ascii	"write_idblock %x %x\012\000"
.LC51:
	.ascii	"write_idblock fail! %x %x %x %x\012\000"
.LC52:
	.ascii	"%s idb buffer alloc fail\012\000"
	.section	.rodata.INSERT_DATA_LIST.str1.1,"aMS",%progbits,1
.LC1:
	.ascii	"\012!!!!! error @ func:%s - line:%d\012\000"
	.section	.rodata.__func__.10007,"a",%progbits
	.set	.LANCHOR167,. + 0
	.type	__func__.10007, %object
	.size	__func__.10007, 29
__func__.10007:
	.ascii	"allocate_new_data_superblock\000"
	.section	.rodata.__func__.10014,"a",%progbits
	.set	.LANCHOR60,. + 0
	.type	__func__.10014, %object
	.size	__func__.10014, 19
__func__.10014:
	.ascii	"get_new_active_ppa\000"
	.section	.rodata.__func__.10027,"a",%progbits
	.set	.LANCHOR145,. + 0
	.type	__func__.10027, %object
	.size	__func__.10027, 16
__func__.10027:
	.ascii	"update_vpc_list\000"
	.section	.rodata.__func__.10034,"a",%progbits
	.set	.LANCHOR146,. + 0
	.type	__func__.10034, %object
	.size	__func__.10034, 20
__func__.10034:
	.ascii	"decrement_vpc_count\000"
	.section	.rodata.__func__.10104,"a",%progbits
	.set	.LANCHOR169,. + 0
	.type	__func__.10104, %object
	.size	__func__.10104, 19
__func__.10104:
	.ascii	"FtlGcFreeTempBlock\000"
	.section	.rodata.__func__.10203,"a",%progbits
	.set	.LANCHOR174,. + 0
	.type	__func__.10203, %object
	.size	__func__.10203, 23
__func__.10203:
	.ascii	"rk_ftl_garbage_collect\000"
	.section	.rodata.__func__.10471,"a",%progbits
	.set	.LANCHOR136,. + 0
	.type	__func__.10471, %object
	.size	__func__.10471, 15
__func__.10471:
	.ascii	"FlashReadPages\000"
	.section	.rodata.__func__.10490,"a",%progbits
	.set	.LANCHOR141,. + 0
	.type	__func__.10490, %object
	.size	__func__.10490, 15
__func__.10490:
	.ascii	"FlashProgPages\000"
	.section	.rodata.__func__.10514,"a",%progbits
	.set	.LANCHOR76,. + 0
	.type	__func__.10514, %object
	.size	__func__.10514, 17
__func__.10514:
	.ascii	"FlashEraseBlocks\000"
	.section	.rodata.__func__.10633,"a",%progbits
	.set	.LANCHOR184,. + 0
	.type	__func__.10633, %object
	.size	__func__.10633, 14
__func__.10633:
	.ascii	"FtlWriteToIDB\000"
	.section	.rodata.__func__.9127,"a",%progbits
	.set	.LANCHOR168,. + 0
	.type	__func__.9127, %object
	.size	__func__.9127, 13
__func__.9127:
	.ascii	"FtlProgPages\000"
	.section	.rodata.__func__.9155,"a",%progbits
	.set	.LANCHOR176,. + 0
	.type	__func__.9155, %object
	.size	__func__.9155, 9
__func__.9155:
	.ascii	"FtlWrite\000"
	.section	.rodata.__func__.9237,"a",%progbits
	.set	.LANCHOR127,. + 0
	.type	__func__.9237, %object
	.size	__func__.9237, 14
__func__.9237:
	.ascii	"FtlBbt2Bitmap\000"
	.section	.rodata.__func__.9272,"a",%progbits
	.set	.LANCHOR140,. + 0
	.type	__func__.9272, %object
	.size	__func__.9272, 11
__func__.9272:
	.ascii	"FtlLoadBbt\000"
	.section	.rodata.__func__.9387,"a",%progbits
	.set	.LANCHOR48,. + 0
	.type	__func__.9387, %object
	.size	__func__.9387, 17
__func__.9387:
	.ascii	"INSERT_FREE_LIST\000"
	.section	.rodata.__func__.9392,"a",%progbits
	.set	.LANCHOR44,. + 0
	.type	__func__.9392, %object
	.size	__func__.9392, 17
__func__.9392:
	.ascii	"INSERT_DATA_LIST\000"
	.section	.rodata.__func__.9423,"a",%progbits
	.set	.LANCHOR49,. + 0
	.type	__func__.9423, %object
	.size	__func__.9423, 17
__func__.9423:
	.ascii	"List_remove_node\000"
	.section	.rodata.__func__.9455,"a",%progbits
	.set	.LANCHOR53,. + 0
	.type	__func__.9455, %object
	.size	__func__.9455, 22
__func__.9455:
	.ascii	"List_update_data_list\000"
	.section	.rodata.__func__.9464,"a",%progbits
	.set	.LANCHOR149,. + 0
	.type	__func__.9464, %object
	.size	__func__.9464, 16
__func__.9464:
	.ascii	"load_l2p_region\000"
	.section	.rodata.__func__.9497,"a",%progbits
	.set	.LANCHOR80,. + 0
	.type	__func__.9497, %object
	.size	__func__.9497, 26
__func__.9497:
	.ascii	"ftl_map_blk_alloc_new_blk\000"
	.section	.rodata.__func__.9508,"a",%progbits
	.set	.LANCHOR150,. + 0
	.type	__func__.9508, %object
	.size	__func__.9508, 15
__func__.9508:
	.ascii	"ftl_map_blk_gc\000"
	.section	.rodata.__func__.9523,"a",%progbits
	.set	.LANCHOR147,. + 0
	.type	__func__.9523, %object
	.size	__func__.9523, 31
__func__.9523:
	.ascii	"Ftl_write_map_blk_to_last_page\000"
	.section	.rodata.__func__.9537,"a",%progbits
	.set	.LANCHOR148,. + 0
	.type	__func__.9537, %object
	.size	__func__.9537, 16
__func__.9537:
	.ascii	"FtlMapWritePage\000"
	.section	.rodata.__func__.9562,"a",%progbits
	.set	.LANCHOR56,. + 0
	.type	__func__.9562, %object
	.size	__func__.9562, 22
__func__.9562:
	.ascii	"select_l2p_ram_region\000"
	.section	.rodata.__func__.9579,"a",%progbits
	.set	.LANCHOR152,. + 0
	.type	__func__.9579, %object
	.size	__func__.9579, 9
__func__.9579:
	.ascii	"log2phys\000"
	.section	.rodata.__func__.9643,"a",%progbits
	.set	.LANCHOR164,. + 0
	.type	__func__.9643, %object
	.size	__func__.9643, 15
__func__.9643:
	.ascii	"FtlVpcTblFlush\000"
	.section	.rodata.__func__.9665,"a",%progbits
	.set	.LANCHOR139,. + 0
	.type	__func__.9665, %object
	.size	__func__.9665, 14
__func__.9665:
	.ascii	"FtlScanSysBlk\000"
	.section	.rodata.__func__.9714,"a",%progbits
	.set	.LANCHOR177,. + 0
	.type	__func__.9714, %object
	.size	__func__.9714, 15
__func__.9714:
	.ascii	"FtlLoadSysInfo\000"
	.section	.rodata.__func__.9736,"a",%progbits
	.set	.LANCHOR179,. + 0
	.type	__func__.9736, %object
	.size	__func__.9736, 18
__func__.9736:
	.ascii	"FtlMapTblRecovery\000"
	.section	.rodata.__func__.9782,"a",%progbits
	.set	.LANCHOR153,. + 0
	.type	__func__.9782, %object
	.size	__func__.9782, 16
__func__.9782:
	.ascii	"FtlReUsePrevPpa\000"
	.section	.rodata.__func__.9816,"a",%progbits
	.set	.LANCHOR154,. + 0
	.type	__func__.9816, %object
	.size	__func__.9816, 22
__func__.9816:
	.ascii	"FtlRecoverySuperblock\000"
	.section	.rodata.__func__.9873,"a",%progbits
	.set	.LANCHOR59,. + 0
	.type	__func__.9873, %object
	.size	__func__.9873, 16
__func__.9873:
	.ascii	"make_superblock\000"
	.section	.rodata.__func__.9894,"a",%progbits
	.set	.LANCHOR135,. + 0
	.type	__func__.9894, %object
	.size	__func__.9894, 18
__func__.9894:
	.ascii	"SupperBlkListInit\000"
	.section	.rodata.__func__.9921,"a",%progbits
	.set	.LANCHOR158,. + 0
	.type	__func__.9921, %object
	.size	__func__.9921, 14
__func__.9921:
	.ascii	"ftl_check_vpc\000"
	.section	.rodata.__func__.9986,"a",%progbits
	.set	.LANCHOR143,. + 0
	.type	__func__.9986, %object
	.size	__func__.9986, 25
__func__.9986:
	.ascii	"allocate_data_superblock\000"
	.section	.rodata.decrement_vpc_count.str1.1,"aMS",%progbits,1
.LC21:
	.ascii	"decrement_vpc_count %x = %d\012\000"
	.section	.rodata.ftl_check_vpc.str1.1,"aMS",%progbits,1
.LC29:
	.ascii	"...%s enter...\012\000"
.LC30:
	.ascii	"FtlCheckVpc2 %x = %x  %x\012\000"
.LC31:
	.ascii	"free blk vpc error %x = %x  %x\012\000"
	.section	.rodata.ftl_map_blk_alloc_new_blk.str1.1,"aMS",%progbits,1
.LC10:
	.ascii	"FtlFreeSysBlkQueueOut = %x, free count = %d\012\000"
	.section	.rodata.ftl_scan_all_data.str1.1,"aMS",%progbits,1
.LC32:
	.ascii	"ftl_scan_all_data = %x\012\000"
.LC33:
	.ascii	"scan lpa = %x ppa= %x\012\000"
.LC34:
	.ascii	"lpa = %x,addr= %x,spare= %x %x %x %x data=%x %x\012"
	.ascii	"\000"
	.section	.rodata.load_l2p_region.str1.1,"aMS",%progbits,1
.LC24:
	.ascii	"region_id = %x phyAddr = %x\012\000"
.LC25:
	.ascii	"map_ppn:\000"
.LC26:
	.ascii	"load_l2p_region refresh = %x phyAddr = %x\012\000"
	.section	.rodata.rk_ftl_garbage_collect.str1.1,"aMS",%progbits,1
.LC42:
	.ascii	"g_gc_superblock_free %x %x %x %x %x\012\000"
.LC43:
	.ascii	"lpa=%x, ppa=%x\012\000"
	.section	.rodata.rknand_print_hex.str1.1,"aMS",%progbits,1
.LC2:
	.ascii	"%s 0x%x:\000"
.LC3:
	.ascii	"%x \000"
.LC4:
	.ascii	"%02x \000"
.LC5:
	.ascii	"\012\000"
	.section	.rodata.sftl_init.str1.1,"aMS",%progbits,1
.LC49:
	.ascii	"SFTL version: 5.0.58 20220814\000"
	.hidden	free
