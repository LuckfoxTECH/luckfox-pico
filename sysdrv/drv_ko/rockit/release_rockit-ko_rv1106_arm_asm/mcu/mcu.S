	.arch armv7-a
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 4
	.eabi_attribute 34, 1
	.eabi_attribute 18, 2
	.file	"mcu.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_send_message, %function
mcu_send_message:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L2
	ldr	r2, [r3, #232]
	str	r1, [r2]
	ldr	r3, [r3, #228]
	str	r0, [r3]
	bx	lr
.L3:
	.align	2
.L2:
	.word	.LANCHOR0
	.fnend
	.size	mcu_send_message, .-mcu_send_message
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_compat_ioctl, %function
vmcu_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vmcu_compat_ioctl, .-vmcu_compat_ioctl
	.section	.init.text,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	isp_max_w_setup, %function
isp_max_w_setup:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #0
	push	{r3, lr}
	.save {r3, lr}
	mov	r1, r2
	bl	simple_strtoul
	ldr	r3, .L6
	str	r0, [r3, #252]
	movs	r0, #1
	pop	{r3, pc}
.L7:
	.align	2
.L6:
	.word	.LANCHOR0
	.fnend
	.size	isp_max_w_setup, .-isp_max_w_setup
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	isp_max_h_setup, %function
isp_max_h_setup:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #0
	push	{r3, lr}
	.save {r3, lr}
	mov	r1, r2
	bl	simple_strtoul
	ldr	r3, .L9
	str	r0, [r3, #256]
	movs	r0, #1
	pop	{r3, pc}
.L10:
	.align	2
.L9:
	.word	.LANCHOR0
	.fnend
	.size	isp_max_h_setup, .-isp_max_h_setup
	.text
	.align	1
	.global	vmcu_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_version, %function
vmcu_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r3, [r3]
	ldr	r5, [r3, #160]
	ldr	r3, [r1, #32]
	cmp	r3, #76
	beq	.L12
	movs	r1, #2
	movs	r3, #217
	ldr	r2, .L18
	mov	r0, r1
	str	r3, [sp]
	ldr	r3, .L18+4
	bl	vlog
.L12:
	movs	r3, #0
	movs	r2, #1
	str	r3, [r4, #8]
	strd	r2, r3, [r4]
	ldr	r0, [r5]
	bl	strlen
	cmp	r0, #16
	it	ls
	strls	r0, [r4, #12]
	add	r0, r4, #16
	itt	hi
	movhi	r3, #16
	strhi	r3, [r4, #12]
	ldr	r2, [r4, #12]
	ldr	r1, [r5]
	bl	strncpy
	movs	r0, #0
	str	r0, [r4, #36]
	str	r0, [r4, #56]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L19:
	.align	2
.L18:
	.word	.LC0
	.word	.LANCHOR1
	.fnend
	.size	vmcu_version, .-vmcu_version
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_release, %function
vmcu_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	ldr	r4, [r1, #128]
	ldr	r0, [r4]
	bl	mpi_obj_unref
	ldr	r3, [r4]
	ldr	r2, .L21
	movs	r1, #4
	movs	r0, #2
	ldr	r3, [r3]
	str	r3, [sp, #4]
	mov	r3, #296
	str	r3, [sp]
	ldr	r3, .L21+4
	bl	vlog
	mov	r0, r4
	bl	kfree
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L22:
	.align	2
.L21:
	.word	.LC1
	.word	.LANCHOR1+13
	.fnend
	.size	vmcu_release, .-vmcu_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_read, %function
vmcu_read:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L25
	bl	mpi_fs_buf_read
	cmp	r0, #-1
	it	eq
	moveq	r0, #0
	pop	{r3, pc}
.L26:
	.align	2
.L25:
	.word	.LANCHOR0+168
	.fnend
	.size	vmcu_read, .-vmcu_read
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_ioctl, %function
vmcu_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	uxtb	r4, r1
	ldr	r3, .L32
	mov	r6, r0
	mov	r5, r1
	mov	r7, r2
	adds	r3, r3, #4
	movs	r1, #5
	ldr	r8, [r3, #160]
	movs	r0, #2
	strd	r4, r3, [sp, #8]
	movs	r3, #249
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L32+4
	ldr	r3, .L32+8
	bl	vlog
	ldr	r1, [r8, #212]
	cmp	r1, r4
	bhi	.L28
	movs	r3, #252
	ldr	r2, .L32+12
	str	r3, [sp]
	ldr	r3, .L32+8
.L31:
	movs	r1, #2
	mov	r0, r1
	bl	vlog
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L29:
.L28:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
	cmp	r4, r1
	sbc	r1, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r4, r4, r1
	ldr	r1, [r8, #208]
	movs	r3, #20
	mla	r1, r3, r4, r1
	cbnz	r1, .L30
	movw	r3, #259
	ldr	r2, .L32+16
	str	r3, [sp]
	ldr	r3, .L32+8
	b	.L31
.L30:
	mov	r3, r7
	mov	r2, r5
	mov	r0, r6
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L33:
	.align	2
.L32:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LANCHOR1+26
	.word	.LC3
	.word	.LC4
	.fnend
	.size	vmcu_ioctl, .-vmcu_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_thread_fn, %function
vmcu_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r7, r0
	ldr	r8, .L45+24
	movs	r6, #0
	ldr	r9, .L45+28
.L36:
	ldr	r5, [r7, #204]
	cbz	r5, .L39
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L39:
	movs	r0, #10
	bl	msleep
	ldr	r3, [r8, #236]
	ldr	r4, [r3]
	cmp	r6, r4
	beq	.L36
	cmp	r4, #0
	beq	.L37
	mov	r1, r5
	movs	r0, #8
	bl	mcu_send_message
	cmp	r4, #7
	bne	.L37
	movs	r1, #4
	mov	r0, r9
	bl	ioremap
	movs	r1, #4
	ldr	r10, [r0]
	ldr	r0, .L45
	bl	ioremap
	movs	r1, #4
	ldr	fp, [r0]
	ldr	r0, .L45+4
	bl	ioremap
	movs	r1, #4
	ldrh	r5, [r0, #2]
	ldr	r0, .L45+8
	bl	ioremap
	movs	r1, #4
	lsls	r5, r5, #4
	ldrh	r6, [r0, #2]
	ldr	r0, .L45+12
	bl	ioremap
	mov	r2, fp
	mov	r1, r10
	lsls	r6, r6, #4
	ldr	fp, .L45+32
	ldr	r10, .L45+36
	ldr	r3, [r0]
	ldr	r0, .L45+16
	strd	r6, r3, [sp]
	mov	r3, r5
	bl	printk
	movs	r1, #44
	ldr	r0, .L45+20
	bl	ioremap
	ldr	r5, .L45+20
	subs	r6, r0, #4
.L38:
	mov	r1, r5
	ldr	r2, [r6, #4]!
	mov	r0, fp
	adds	r5, r5, #4
	bl	printk
	cmp	r5, r10
	bne	.L38
	movs	r0, #10
	bl	msleep
	movs	r1, #0
	mov	r0, r1
	bl	mcu_send_message
.L37:
	mov	r6, r4
	b	.L36
.L46:
	.align	2
.L45:
	.word	-5963752
	.word	-5943288
	.word	-5943284
	.word	-5963000
	.word	.LC5
	.word	-5943292
	.word	.LANCHOR0
	.word	-5963680
	.word	.LC6
	.word	-5943248
	.fnend
	.size	vmcu_thread_fn, .-vmcu_thread_fn
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_open, %function
vmcu_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L50
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r7, r1
	ldr	r0, [r3, #12]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbnz	r0, .L48
	movs	r1, #2
	movw	r3, #283
	ldr	r2, .L50+4
	mov	r0, r1
	str	r3, [sp]
	ldr	r3, .L50+8
	bl	vlog
	mov	r0, #-1
.L47:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L48:
	ldr	r5, .L50+12
	adds	r6, r5, #4
	mov	r0, r6
	bl	mpi_obj_ref
	ldr	r3, [r5, #4]
	str	r6, [r4]
	movs	r1, #4
	str	r4, [r7, #128]
	movs	r0, #2
	ldr	r2, .L50+16
	str	r3, [sp, #4]
	mov	r3, #288
	str	r3, [sp]
	ldr	r3, .L50+8
	bl	vlog
	movs	r0, #0
	b	.L47
.L51:
	.align	2
.L50:
	.word	kmalloc_caches
	.word	.LC7
	.word	.LANCHOR1+37
	.word	.LANCHOR0
	.word	.LC1
	.fnend
	.size	vmcu_open, .-vmcu_open
	.global	__aeabi_idiv
	.align	1
	.global	mcu_wrap_config
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_wrap_config, %function
mcu_wrap_config:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r6, r0
	ldr	r4, .L62
	.pad #28
	sub	sp, sp, #28
	mov	fp, r1
	ldr	r3, [r4, #248]
	cmp	r3, #1
	bne	.L52
	movs	r1, #0
	movs	r0, #6
	bl	mcu_send_message
	ldr	r5, [r4, #256]
	cbnz	r5, .L55
	ldr	r3, [r4, #244]
	ldr	r5, [r3]
	ubfx	r5, r5, #0, #14
.L55:
	ldr	r3, [r4, #220]
	mov	r0, #1000
	mov	r1, r5
	mov	r10, #0
	muls	r0, r6, r0
	ldr	r7, [r3, #220]
	bl	__aeabi_idiv
	movs	r3, #112
	ldr	r2, .L62+4
	movs	r1, #4
	sub	r8, r7, #4
	mov	r9, r0
	str	r3, [sp]
	strd	r7, r5, [sp, #4]
	movs	r0, #2
	ldr	r3, .L62+8
	bl	vlog
	lsl	r3, r9, #5
	mov	r9, #32
	str	r3, [sp, #20]
.L57:
	movs	r3, #0
	str	r3, [r8, #4]!
	str	r3, [r8, #256]
	ldr	r3, .L62+12
	str	r9, [r8, #768]
	ldr	r2, .L62+16
	smull	r0, r1, r10, r3
	asr	r3, r10, #31
	movs	r0, #2
	rsb	r3, r3, r1, asr #6
	movs	r1, #4
	str	r3, [r8, #512]
	strd	r9, r3, [sp, #4]
	movs	r3, #120
	str	r3, [sp]
	ldr	r3, .L62+8
	bl	vlog
	ldr	r3, [r8, #512]
	cmp	r3, r6
	bge	.L56
	add	r9, r9, #32
	ldr	r3, [sp, #20]
	cmp	r9, #2080
	add	r10, r10, r3
	bne	.L57
.L56:
	ldr	r3, [r4, #224]
	adds	r6, r6, #31
	str	r5, [r7, #1024]
	bic	r6, r6, #31
	cmp	r3, #0
	it	ne
	movne	fp, r3
	ldr	r3, [r4, #220]
	str	r6, [r7, #1028]
	movs	r1, #1
	str	fp, [r7, #1032]
	ldr	r0, [r3, #64]
	bl	dma_buf_end_cpu_access
	ldr	r3, [r7, #1032]
	ldr	r2, .L62+20
	movs	r1, #4
	movs	r0, #2
	str	r3, [sp, #8]
	ldr	r3, [r4, #220]
	ldr	r3, [r3, #224]
	str	r3, [sp, #4]
	movs	r3, #131
	str	r3, [sp]
	ldr	r3, .L62+8
	bl	vlog
	ldr	r3, [r4, #220]
	movs	r0, #1
	ldr	r1, [r3, #224]
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	mcu_send_message
.L52:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L63:
	.align	2
.L62:
	.word	.LANCHOR0
	.word	.LC8
	.word	.LANCHOR1+47
	.word	274877907
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mcu_wrap_config, .-mcu_wrap_config
	.align	1
	.global	mcu_set_wrap_line
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_set_wrap_line, %function
mcu_set_wrap_line:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L65
	str	r0, [r3, #224]
	bx	lr
.L66:
	.align	2
.L65:
	.word	.LANCHOR0
	.fnend
	.size	mcu_set_wrap_line, .-mcu_set_wrap_line
	.align	1
	.global	mcu_get_wrap_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_get_wrap_status, %function
mcu_get_wrap_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L68
	ldr	r0, [r3, #248]
	bx	lr
.L69:
	.align	2
.L68:
	.word	.LANCHOR0
	.fnend
	.size	mcu_get_wrap_status, .-mcu_get_wrap_status
	.align	1
	.global	mcu_wrap_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_wrap_start, %function
mcu_wrap_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	ldr	r4, .L74
	ldr	r3, [r4, #248]
	cmp	r3, #1
	bne	.L70
	ldr	r2, .L74+4
	movs	r3, #148
	movs	r1, #4
	str	r3, [sp]
	movs	r0, #2
	ldr	r3, .L74+8
	bl	vlog
	movs	r1, #0
	movs	r0, #5
	bl	mcu_send_message
	ldr	r3, [r4, #244]
	movs	r1, #2
	str	r1, [r4, #248]
	ldr	r2, [r4, #256]
	ldr	r3, [r3]
	ubfx	r3, r3, #0, #14
	cmp	r3, r2
	beq	.L70
	strd	r2, r3, [sp, #4]
	mov	r0, r1
	movs	r3, #153
	ldr	r2, .L74+12
	str	r3, [sp]
	ldr	r3, .L74+8
	bl	vlog
.L70:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L75:
	.align	2
.L74:
	.word	.LANCHOR0
	.word	.LC11
	.word	.LANCHOR1+63
	.word	.LC12
	.fnend
	.size	mcu_wrap_start, .-mcu_wrap_start
	.align	1
	.global	mcu_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_reset, %function
mcu_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r3, .L78
	ldr	r3, [r3, #248]
	cbz	r3, .L76
	movs	r1, #0
	movs	r0, #9
	bl	mcu_send_message
	movs	r0, #100
	bl	msleep
	movs	r1, #4
	ldr	r0, .L78+4
	bl	ioremap
	mov	r3, #1966110
	str	r3, [r0]
	movs	r0, #100
	pop	{r3, lr}
	b	msleep
.L76:
	pop	{r3, pc}
.L79:
	.align	2
.L78:
	.word	.LANCHOR0
	.word	-12875260
	.fnend
	.size	mcu_reset, .-mcu_reset
	.align	1
	.global	mcu_wrap_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_wrap_end, %function
mcu_wrap_end:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	ldr	r4, .L82
	ldr	r0, [r4, #248]
	cmp	r0, #2
	bne	.L80
	movs	r3, #171
	ldr	r2, .L82+4
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L82+8
	bl	vlog
	movs	r3, #1
	movs	r1, #0
	str	r3, [r4, #248]
	movs	r0, #6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, lr}
	b	mcu_send_message
.L80:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L83:
	.align	2
.L82:
	.word	.LANCHOR0
	.word	.LC13
	.word	.LANCHOR1+78
	.fnend
	.size	mcu_wrap_end, .-mcu_wrap_end
	.align	1
	.global	vmcu_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_probe, %function
vmcu_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #28
	sub	sp, sp, #28
	mov	r6, r0
	movs	r1, #4
	ldr	r0, .L103
	movs	r3, #0
	movs	r2, #0
	strd	r2, [sp, #16]
	bl	ioremap
	ldr	r3, [r0]
	and	r3, r3, #30
	cmp	r3, #30
	beq	.L85
	movs	r1, #4
	ldr	r0, .L103
	bl	ioremap
	ldr	r2, .L103+4
	movs	r1, #4
	ldr	r3, [r0]
	str	r3, [sp, #4]
	movw	r3, #333
	str	r3, [sp]
	ldr	r3, .L103+8
.L102:
	movs	r0, #2
	bl	vlog
.L84:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L85:
	ldr	r4, .L103+12
	movs	r2, #252
	movs	r1, #0
	mov	r0, r4
	bl	memset
	ldr	r0, .L103+16
	bl	mpi_allocator_find_by_name
	str	r0, [r4, #216]
	cbnz	r0, .L87
	mov	r3, #340
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L103+8
	ldr	r2, .L103+20
	b	.L102
.L87:
	ldr	r0, [r4, #260]
	ldr	r5, .L103+24
	cmp	r0, #0
	beq	.L89
	mov	r2, #416
	movs	r1, #0
	bl	filp_open
	movw	r3, #345
	ldr	r2, .L103+28
	movs	r1, #4
	mov	r7, r0
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r0, #2
	ldr	r3, .L103+8
	bl	vlog
	cbz	r7, .L89
	mov	r1, #4096
	ldr	r0, [r5]
	bl	ioremap
	mov	r2, #4096
	movs	r1, #0
	bl	memset
	mov	r1, #4096
	ldr	r0, [r5]
	bl	ioremap
	mov	r2, #4096
	mov	r1, r0
	add	r3, sp, #16
	mov	r0, r7
	bl	kernel_read
	movs	r1, #4
	ldrd	r2, [sp, #16]
	str	r0, [sp, #4]
	movs	r0, #2
	strd	r2, [sp, #8]
	mov	r3, #354
	ldr	r2, .L103+32
	str	r3, [sp]
	ldr	r3, .L103+8
	bl	vlog
.L89:
	movs	r1, #4
	ldr	r0, .L103+36
	bl	ioremap
	ldr	r3, [r5]
	movs	r1, #4
	str	r3, [r0]
	ldr	r0, .L103
	bl	ioremap
	mov	r3, #1966080
	movs	r1, #4
	str	r3, [r0]
	ldr	r0, .L103+40
	bl	ioremap
	movs	r1, #4
	str	r0, [r4, #228]
	ldr	r0, .L103+44
	bl	ioremap
	movs	r1, #4
	str	r0, [r4, #232]
	ldr	r0, .L103+48
	bl	ioremap
	movs	r1, #4
	str	r0, [r4, #236]
	ldr	r0, .L103+52
	bl	ioremap
	movs	r1, #4
	str	r0, [r4, #240]
	ldr	r0, .L103+56
	bl	ioremap
	mov	r2, #10240
	movs	r1, #1
	str	r0, [r4, #244]
	ldr	r0, [r4, #216]
	bl	mpi_allocator_alloc
	mov	r5, r0
	cbz	r0, .L92
	movs	r1, #0
	bl	mpi_buf_create
	str	r0, [r4, #220]
	cbz	r0, .L92
	ldr	r3, .L103+60
	str	r3, [r0, #4]
	mov	r0, r5
	bl	dma_buf_put
	mov	r0, r5
	ldr	r7, [r4, #220]
	bl	get_dma_buf_phy_addr
	str	r0, [r7, #224]
	mov	r0, r5
	ldr	r7, [r4, #220]
	bl	dma_buf_vmap
	mov	r2, #10240
	movs	r1, #0
	str	r0, [r7, #220]
	ldr	r3, [r4, #220]
	ldr	r0, [r3, #220]
	bl	memset
	ldr	r3, [r4, #220]
	movs	r1, #1
	ldrd	r2, r3, [r3, #220]
	add	r3, r3, #2048
	str	r3, [r2]
	mov	r3, #8192
	str	r3, [r2, #4]
	ldr	r3, [r4, #220]
	ldr	r0, [r3, #64]
	bl	dma_buf_end_cpu_access
	ldr	r3, [r4, #220]
	movs	r0, #3
	ldr	r1, [r3, #224]
	bl	mcu_send_message
.L92:
	mov	r1, #3264
	mov	r0, r6
	bl	kstrdup
	ldr	r7, .L103+64
	ldr	r5, .L103+68
	mov	r2, r6
	movs	r1, #8
	movs	r3, #1
	str	r4, [r4, #164]
	str	r3, [r4, #212]
	str	r7, [r4, #208]
	str	r0, [r4]
	ldr	r0, .L103+72
	bl	mpi_dev_register
	add	r1, r7, #20
	sub	r0, r5, #188
	movs	r3, #0
	strd	r5, r5, [r4, #192]
	str	r3, [r4, #72]
	strd	r3, r3, [r4, #80]
	str	r3, [r4, #188]
	bl	mpi_sysfs_dev_add
	sub	r1, r5, #192
	mov	r3, r6
	mov	r2, #-1
	ldr	r0, .L103+76
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L94
	bl	wake_up_process
.L94:
	mov	r1, #8192
	ldr	r0, .L103+80
	str	r5, [r4, #200]
	bl	mpi_fs_buf_init
	ldr	r3, [r4, #220]
	movs	r1, #4
	movs	r0, #2
	ldr	r3, [r3, #220]
	add	r2, r3, #2048
	str	r2, [r4, #184]
	ldr	r2, [r4, #176]
	adds	r3, r3, #64
	str	r2, [r4, #180]
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	mov	r3, #394
	ldr	r2, .L103+84
	str	r3, [sp]
	ldr	r3, .L103+8
	bl	vlog
	movs	r3, #1
	str	r3, [r4, #248]
	b	.L84
.L104:
	.align	2
.L103:
	.word	-12875260
	.word	.LC14
	.word	.LANCHOR1+91
	.word	.LANCHOR0
	.word	.LC15
	.word	.LC16
	.word	.LANCHOR2
	.word	.LC17
	.word	.LC18
	.word	-16293820
	.word	-10747896
	.word	-10747892
	.word	-10747856
	.word	-10747852
	.word	-6290412
	.word	.LC19
	.word	.LANCHOR1+104
	.word	.LANCHOR0+192
	.word	.LANCHOR0+4
	.word	vmcu_thread_fn
	.word	.LANCHOR0+168
	.word	.LC20
	.fnend
	.size	vmcu_probe, .-vmcu_probe
	.align	1
	.global	vmcu_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_shutdown, %function
vmcu_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L106
	movs	r3, #1
	adds	r0, r0, #4
	str	r3, [r0, #200]
	b	mpi_dev_unregister
.L107:
	.align	2
.L106:
	.word	.LANCHOR0
	.fnend
	.size	vmcu_shutdown, .-vmcu_shutdown
	.align	1
	.global	vmcu_load
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_load, %function
vmcu_load:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L109
	bl	vmcu_probe
	movs	r0, #0
	pop	{r3, pc}
.L110:
	.align	2
.L109:
	.word	.LC21
	.fnend
	.size	vmcu_load, .-vmcu_load
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	vmcu_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_exit, %function
vmcu_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	vmcu_shutdown
	.fnend
	.size	vmcu_exit, .-vmcu_exit
	.global	g_vmcu_dev
	.global	isp_max_h
	.global	isp_max_w
	.global	mcu_fw_addr
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__func__.37977, %object
	.size	__func__.37977, 13
__func__.37977:
	.ascii	"vmcu_version\000"
	.type	__func__.38019, %object
	.size	__func__.38019, 13
__func__.38019:
	.ascii	"vmcu_release\000"
	.type	__func__.37990, %object
	.size	__func__.37990, 11
__func__.37990:
	.ascii	"vmcu_ioctl\000"
	.type	__func__.38012, %object
	.size	__func__.38012, 10
__func__.38012:
	.ascii	"vmcu_open\000"
	.type	__func__.37935, %object
	.size	__func__.37935, 16
__func__.37935:
	.ascii	"mcu_wrap_config\000"
	.type	__func__.37949, %object
	.size	__func__.37949, 15
__func__.37949:
	.ascii	"mcu_wrap_start\000"
	.type	__func__.37956, %object
	.size	__func__.37956, 13
__func__.37956:
	.ascii	"mcu_wrap_end\000"
	.type	__func__.38038, %object
	.size	__func__.38038, 11
__func__.38038:
	.ascii	"vmcu_probe\000"
	.space	2
	.type	vmcu_ioctls, %object
	.size	vmcu_ioctls, 20
vmcu_ioctls:
	.word	-1068733184
	.word	0
	.word	vmcu_version
	.word	.LC22
	.word	0
	.type	vmcu_driver_fops, %object
	.size	vmcu_driver_fops, 128
vmcu_driver_fops:
	.word	0
	.space	4
	.word	vmcu_read
	.space	24
	.word	0
	.word	vmcu_ioctl
	.word	vmcu_compat_ioctl
	.word	0
	.space	4
	.word	vmcu_open
	.space	4
	.word	vmcu_release
	.space	60
	.type	__param_str_isp_max_h, %object
	.size	__param_str_isp_max_h, 17
__param_str_isp_max_h:
	.ascii	"rockit.isp_max_h\000"
	.type	__param_str_isp_max_w, %object
	.size	__param_str_isp_max_w, 17
__param_str_isp_max_w:
	.ascii	"rockit.isp_max_w\000"
	.type	__param_str_mcu_fw_addr, %object
	.size	__param_str_mcu_fw_addr, 19
__param_str_mcu_fw_addr:
	.ascii	"rockit.mcu_fw_addr\000"
	.type	__param_str_mcu_fw_path, %object
	.size	__param_str_mcu_fw_path, 19
__param_str_mcu_fw_path:
	.ascii	"rockit.mcu_fw_path\000"
	.data
	.align	2
	.set	.LANCHOR2,. + 0
	.type	mcu_fw_addr, %object
	.size	mcu_fw_addr, 4
mcu_fw_addr:
	.word	-9441280
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_vmcu_dev, %object
	.size	g_vmcu_dev, 252
g_vmcu_dev:
	.space	252
	.type	isp_max_w, %object
	.size	isp_max_w, 4
isp_max_w:
	.space	4
	.type	isp_max_h, %object
	.size	isp_max_h, 4
isp_max_h:
	.space	4
	.type	mcu_fw_path, %object
	.size	mcu_fw_path, 4
mcu_fw_path:
	.space	4
	.section	.init.rodata,"a"
	.type	__setup_str_isp_max_h_setup, %object
	.size	__setup_str_isp_max_h_setup, 10
__setup_str_isp_max_h_setup:
	.ascii	"rk_cam_h=\000"
	.type	__setup_str_isp_max_w_setup, %object
	.size	__setup_str_isp_max_w_setup, 10
__setup_str_isp_max_w_setup:
	.ascii	"rk_cam_w=\000"
	.section	.init.setup,"aw"
	.align	2
	.type	__setup_isp_max_h_setup, %object
	.size	__setup_isp_max_h_setup, 12
__setup_isp_max_h_setup:
	.word	__setup_str_isp_max_h_setup
	.word	isp_max_h_setup
	.word	0
	.type	__setup_isp_max_w_setup, %object
	.size	__setup_isp_max_w_setup, 12
__setup_isp_max_w_setup:
	.word	__setup_str_isp_max_w_setup
	.word	isp_max_w_setup
	.word	0
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_isp_max_htype231, %object
	.size	__UNIQUE_ID_isp_max_htype231, 31
__UNIQUE_ID_isp_max_htype231:
	.ascii	"rockit.parmtype=isp_max_h:uint\000"
	.type	__UNIQUE_ID_isp_max_wtype230, %object
	.size	__UNIQUE_ID_isp_max_wtype230, 31
__UNIQUE_ID_isp_max_wtype230:
	.ascii	"rockit.parmtype=isp_max_w:uint\000"
	.type	__UNIQUE_ID_mcu_fw_addrtype229, %object
	.size	__UNIQUE_ID_mcu_fw_addrtype229, 33
__UNIQUE_ID_mcu_fw_addrtype229:
	.ascii	"rockit.parmtype=mcu_fw_addr:uint\000"
	.type	__UNIQUE_ID_mcu_fw_pathtype228, %object
	.size	__UNIQUE_ID_mcu_fw_pathtype228, 34
__UNIQUE_ID_mcu_fw_pathtype228:
	.ascii	"rockit.parmtype=mcu_fw_path:charp\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[31m\033[1m%s,%d:struct vmcu_version mismatchin"
	.ascii	"g\033[0m\000"
.LC1:
	.ascii	"\033[32m\033[1m%s,%d:vmcu refcnt %d\033[0m\000"
.LC2:
	.ascii	"%s,%d:vmcu_ioctl: nr = %d, index = %d dev = %p\000"
.LC3:
	.ascii	"\033[31m\033[1m%s,%d:index = %d\033[0m\000"
.LC4:
	.ascii	"\033[31m\033[1m%s,%d:ioctl is null\033[0m\000"
.LC5:
	.ascii	"dvbm en = %x,  line_cnt = %x,  read1 = %x, read2 = "
	.ascii	"%x, 0x308 = %x\000"
.LC6:
	.ascii	"[%x]: %x\000"
.LC7:
	.ascii	"\033[31m\033[1m%s,%d:valloc open fail\033[0m\000"
.LC8:
	.ascii	"\033[32m\033[1m%s,%d:config_addr=%p, isp_max_h=%d\033"
	.ascii	"[0m\000"
.LC9:
	.ascii	"\033[32m\033[1m%s,%d:isp line = %08x, enc_line = %0"
	.ascii	"8x\033[0m\000"
.LC10:
	.ascii	"\033[32m\033[1m%s,%d:g_vmcu_dev.plog_buf->paddr = %"
	.ascii	"x, wrap_line = %d\033[0m\000"
.LC11:
	.ascii	"\033[32m\033[1m%s,%d:send wrap start\033[0m\000"
.LC12:
	.ascii	"\033[31m\033[1m%s,%d:isp input height config err, c"
	.ascii	"onfig h= %d, real h = %d\033[0m\000"
.LC13:
	.ascii	"\033[32m\033[1m%s,%d:send wrap end\033[0m\000"
.LC14:
	.ascii	"\033[32m\033[1m%s,%d:mcu status is not reset = %08x"
	.ascii	", please check\033[0m\000"
.LC15:
	.ascii	"valloc\000"
.LC16:
	.ascii	"\033[31m\033[1m%s,%d:getting alloc fail %p\033[0m\000"
.LC17:
	.ascii	"\033[32m\033[1m%s,%d:open fp = %p\033[0m\000"
.LC18:
	.ascii	"\033[32m\033[1m%s,%d:size = %d, pos = %d\033[0m\000"
.LC19:
	.ascii	"mcu\000"
.LC20:
	.ascii	"\033[32m\033[1m%s,%d:log buf = %08x, len = %d\033[0"
	.ascii	"m\000"
.LC21:
	.ascii	"vmcu\000"
.LC22:
	.ascii	"VMCU_IOCTL_VERSION\000"
	.section	__param,"a"
	.align	2
	.type	__param_isp_max_h, %object
	.size	__param_isp_max_h, 20
__param_isp_max_h:
	.word	__param_str_isp_max_h
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	isp_max_h
	.type	__param_isp_max_w, %object
	.size	__param_isp_max_w, 20
__param_isp_max_w:
	.word	__param_str_isp_max_w
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	isp_max_w
	.type	__param_mcu_fw_addr, %object
	.size	__param_mcu_fw_addr, 20
__param_mcu_fw_addr:
	.word	__param_str_mcu_fw_addr
	.word	0
	.word	param_ops_uint
	.short	256
	.byte	-1
	.byte	0
	.word	mcu_fw_addr
	.type	__param_mcu_fw_path, %object
	.size	__param_mcu_fw_path, 20
__param_mcu_fw_path:
	.word	__param_str_mcu_fw_path
	.word	0
	.word	param_ops_charp
	.short	256
	.byte	-1
	.byte	0
	.word	mcu_fw_path
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
