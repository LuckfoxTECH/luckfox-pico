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
	ldr	r2, [r3, #236]
	str	r1, [r2]
	ldr	r3, [r3, #232]
	str	r0, [r3]
	bx	lr
.L3:
	.align	2
.L2:
	.word	g_vmcu_dev
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
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r3, [r3]
	ldr	r6, [r3, #160]
	ldr	r3, [r1, #32]	@ unaligned
	cmp	r3, #76
	beq	.L6
	movs	r1, #2
	movs	r3, #234
	ldr	r2, .L12
	mov	r0, r1
	str	r3, [sp]
	ldr	r3, .L12+4
	bl	vlog
.L6:
	movs	r5, #0
	movs	r3, #1
	strb	r5, [r4, #1]
	strb	r3, [r4]
	strb	r5, [r4, #2]
	strb	r5, [r4, #3]
	strb	r5, [r4, #4]
	strb	r5, [r4, #5]
	strb	r5, [r4, #6]
	strb	r5, [r4, #7]
	strb	r5, [r4, #8]
	strb	r5, [r4, #9]
	strb	r5, [r4, #10]
	strb	r5, [r4, #11]
	ldr	r0, [r6]
	bl	strlen
	cmp	r0, #16
	ite	ls
	strls	r0, [r4, #12]	@ unaligned
	strbhi	r5, [r4, #13]
	add	r0, r4, #16
	itttt	hi
	strbhi	r5, [r4, #14]
	movhi	r3, #16
	strbhi	r5, [r4, #15]
	strbhi	r3, [r4, #12]
	ldr	r2, [r4, #12]	@ unaligned
	ldr	r1, [r6]
	bl	strncpy
	movs	r0, #0
	strb	r0, [r4, #36]
	strb	r0, [r4, #37]
	strb	r0, [r4, #38]
	strb	r0, [r4, #39]
	strb	r0, [r4, #56]
	strb	r0, [r4, #57]
	strb	r0, [r4, #58]
	strb	r0, [r4, #59]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L13:
	.align	2
.L12:
	.word	.LC0
	.word	.LANCHOR0
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
	ldr	r2, .L15
	movs	r1, #4
	movs	r0, #2
	ldr	r3, [r3]
	str	r3, [sp, #4]
	movw	r3, #313
	str	r3, [sp]
	ldr	r3, .L15+4
	bl	vlog
	mov	r0, r4
	bl	kfree
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L16:
	.align	2
.L15:
	.word	.LC1
	.word	.LANCHOR0+13
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
	ldr	r0, .L19
	bl	mpi_fs_buf_read
	cmp	r0, #-1
	it	eq
	moveq	r0, #0
	pop	{r3, pc}
.L20:
	.align	2
.L19:
	.word	g_vmcu_dev+168
	.fnend
	.size	vmcu_read, .-vmcu_read
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_wrap_suspend, %function
mcu_wrap_suspend:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L26
	ldr	r3, [r4, #264]
	cmp	r3, #1
	beq	.L22
	ldr	r3, [r4, #252]
	ldr	r0, .L26+4
	ldr	r1, [r3]
	bl	printk
	movs	r3, #3
	str	r3, [r4, #264]
.L22:
	movs	r0, #0
	pop	{r4, pc}
.L27:
	.align	2
.L26:
	.word	g_vmcu_dev
	.word	.LC2
	.fnend
	.size	mcu_wrap_suspend, .-mcu_wrap_suspend
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
	ldr	r3, .L33
	mov	r6, r0
	mov	r5, r1
	mov	r7, r2
	adds	r3, r3, #4
	movs	r1, #5
	ldr	r8, [r3, #160]
	movs	r0, #2
	strd	r4, r3, [sp, #8]
	mov	r3, #266
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L33+4
	ldr	r3, .L33+8
	bl	vlog
	ldr	r1, [r8, #212]
	cmp	r1, r4
	bhi	.L29
	movw	r3, #269
	ldr	r2, .L33+12
	str	r3, [sp]
	ldr	r3, .L33+8
.L32:
	movs	r1, #2
	mov	r0, r1
	bl	vlog
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L30:
.L29:
	.syntax unified
@ 87 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
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
	cbnz	r1, .L31
	mov	r3, #276
	ldr	r2, .L33+16
	str	r3, [sp]
	ldr	r3, .L33+8
	b	.L32
.L31:
	mov	r3, r7
	mov	r2, r5
	mov	r0, r6
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L34:
	.align	2
.L33:
	.word	g_vmcu_dev
	.word	.LC3
	.word	.LANCHOR0+26
	.word	.LC4
	.word	.LC5
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
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r8, r0
	ldr	r9, .L49+4
	movs	r5, #0
	ldr	r7, .L49
	ldr	r10, .L49+8
.L36:
	bl	kthread_should_stop
	cbz	r0, .L41
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L41:
	ldr	r6, [r8, #204]
	cbz	r6, .L37
	movs	r0, #1
	mov	r4, r5
	bl	msleep
.L38:
	mov	r5, r4
	b	.L36
.L37:
	movs	r0, #10
	bl	msleep
	ldr	r3, [r9, #240]
	ldr	r4, [r3]
	cmp	r5, r4
	beq	.L38
	cmp	r4, #0
	beq	.L38
	mov	r1, r6
	movs	r0, #8
	bl	mcu_send_message
	cmp	r4, #7
	bne	.L38
	movs	r1, #44
	mov	r0, r7
	bl	ioremap
	cbz	r0, .L39
	ldr	r6, .L49
	subs	r5, r0, #4
	ldr	fp, .L49+12
.L40:
	mov	r1, r6
	ldr	r2, [r5, #4]!
	mov	r0, fp
	adds	r6, r6, #4
	bl	printk
	cmp	r6, r10
	bne	.L40
	mov	r0, r7
	bl	iounmap
.L39:
	movs	r0, #10
	bl	msleep
	movs	r1, #0
	mov	r0, r1
	bl	mcu_send_message
	b	.L38
.L50:
	.align	2
.L49:
	.word	-5943292
	.word	g_vmcu_dev
	.word	-5943248
	.word	.LC6
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
	ldr	r3, .L54
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r6, r1
	ldr	r0, [r3, #12]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r5, r0
	cbnz	r0, .L52
	movs	r1, #2
	mov	r3, #300
	ldr	r2, .L54+4
	mov	r0, r1
	str	r3, [sp]
	ldr	r3, .L54+8
	bl	vlog
	mov	r0, #-1
.L51:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L52:
	ldr	r4, .L54+12
	mov	r0, r4
	bl	mpi_obj_ref
	ldr	r3, [r4]
	str	r4, [r5]
	movs	r1, #4
	str	r5, [r6, #128]
	movs	r0, #2
	ldr	r2, .L54+16
	str	r3, [sp, #4]
	movw	r3, #305
	str	r3, [sp]
	ldr	r3, .L54+8
	bl	vlog
	movs	r0, #0
	b	.L51
.L55:
	.align	2
.L54:
	.word	kmalloc_caches
	.word	.LC7
	.word	.LANCHOR0+37
	.word	g_vmcu_dev+4
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
	mov	r9, r0
	ldr	r4, .L68
	.pad #28
	sub	sp, sp, #28
	mov	fp, r1
	ldr	r3, [r4, #264]
	bic	r3, r3, #2
	cmp	r3, #1
	bne	.L56
	ldr	r5, .L68+4
	movs	r1, #0
	movs	r0, #6
	bl	mcu_send_message
	ldr	r3, [r5]
	cbnz	r3, .L58
	bl	get_rk_cam_h
	str	r0, [r5]
.L58:
	ldr	r5, [r5]
	cbnz	r5, .L60
	ldr	r3, [r4, #248]
	ldr	r5, [r3]
	ubfx	r5, r5, #0, #14
.L60:
	ldr	r3, [r4, #220]
	mov	r0, #1000
	mov	r1, r5
	mov	r10, #0
	mul	r0, r0, r9
	ldr	r6, [r3, #604]
	bl	__aeabi_idiv
	movs	r3, #113
	ldr	r2, .L68+8
	movs	r1, #4
	subs	r7, r6, #4
	mov	r8, r0
	str	r3, [sp]
	strd	r6, r5, [sp, #4]
	movs	r0, #2
	ldr	r3, .L68+12
	bl	vlog
	lsl	r3, r8, #5
	mov	r8, #32
	str	r3, [sp, #20]
.L63:
	movs	r3, #0
	str	r3, [r7, #4]!
	str	r3, [r7, #256]
	ldr	r3, .L68+16
	str	r8, [r7, #768]
	smull	r0, r1, r10, r3
	asr	r3, r10, #31
	rsb	r3, r3, r1, asr #6
	str	r3, [r7, #512]
	ldr	r2, [r4, #264]
	cmp	r2, #1
	bne	.L61
	strd	r8, r3, [sp, #4]
	movs	r1, #4
	movs	r3, #122
	ldr	r2, .L68+20
	str	r3, [sp]
	movs	r0, #2
	ldr	r3, .L68+12
	bl	vlog
.L61:
	ldr	r3, [r7, #512]
	cmp	r9, r3
	ble	.L62
	add	r8, r8, #32
	ldr	r3, [sp, #20]
	cmp	r8, #2080
	add	r10, r10, r3
	bne	.L63
.L62:
	add	r3, r9, #31
	str	r5, [r6, #1024]
	bic	r3, r3, #31
	str	r3, [r6, #1028]
	ldr	r3, [r4, #224]
	movs	r1, #1
	str	r9, [r4, #228]
	cmp	r3, #0
	it	ne
	movne	fp, r3
	ldr	r3, [r4, #220]
	str	fp, [r6, #1032]
	ldr	r0, [r3, #448]
	bl	dma_buf_end_cpu_access
	ldr	r3, [r6, #1032]
	ldr	r2, .L68+24
	movs	r1, #4
	movs	r0, #2
	str	r3, [sp, #8]
	ldr	r3, [r4, #220]
	ldr	r3, [r3, #608]
	str	r3, [sp, #4]
	movs	r3, #134
	str	r3, [sp]
	ldr	r3, .L68+12
	bl	vlog
	ldr	r3, [r4, #220]
	movs	r0, #1
	ldr	r1, [r3, #608]
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	mcu_send_message
.L56:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L69:
	.align	2
.L68:
	.word	g_vmcu_dev
	.word	.LANCHOR1
	.word	.LC8
	.word	.LANCHOR0+47
	.word	274877907
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mcu_wrap_config, .-mcu_wrap_config
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_wrap_resume, %function
mcu_wrap_resume:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	ldr	r4, .L72
	ldr	r6, [r4, #264]
	cmp	r6, #3
	bne	.L71
	mov	r5, #8192
	movs	r1, #0
	mov	r2, r5
	ldr	r0, [r4, #252]
	bl	memset
	mov	r2, r5
	add	r1, r4, #272
	ldr	r0, [r4, #252]
	bl	memcpy
	ldr	r2, .L72+4
	ldr	r3, [r4, #256]
	ldr	r0, .L72+8
	ldr	r2, [r2]
	str	r2, [r3]
	mov	r2, #1966080
	ldr	r3, [r4, #260]
	str	r2, [r3]
	ldr	r3, [r4, #252]
	ldr	r1, [r3]
	bl	printk
	ldr	r3, [r4, #220]
	mov	r2, #10240
	movs	r1, #0
	ldr	r0, [r3, #604]
	bl	memset
	ldr	r3, [r4, #220]
	movs	r1, #1
	ldrd	r2, r3, [r3, #604]
	add	r3, r3, #2048
	strd	r3, r5, [r2]
	ldr	r3, [r4, #220]
	ldr	r5, .L72+12
	ldr	r0, [r3, #448]
	bl	dma_buf_end_cpu_access
	ldr	r3, [r4, #220]
	mov	r0, r6
	ldr	r1, [r3, #608]
	bl	mcu_send_message
	ldr	r3, [r5, #4]
	ldr	r0, .L72+16
	blx	r3
	ldrd	r1, r0, [r4, #224]
	bl	mcu_wrap_config
	ldr	r3, [r5, #4]
	ldr	r0, .L72+16
	blx	r3
	movs	r3, #1
	str	r3, [r4, #264]
.L71:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L73:
	.align	2
.L72:
	.word	g_vmcu_dev
	.word	.LANCHOR2
	.word	.LC11
	.word	arm_delay_ops
	.word	214748000
	.fnend
	.size	mcu_wrap_resume, .-mcu_wrap_resume
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
	ldr	r3, .L75
	str	r0, [r3, #224]
	bx	lr
.L76:
	.align	2
.L75:
	.word	g_vmcu_dev
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
	ldr	r3, .L79
	ldr	r0, [r3, #264]
	cmp	r0, #0
	it	eq
	moveq	r0, #2
	bx	lr
.L80:
	.align	2
.L79:
	.word	g_vmcu_dev
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
	ldr	r4, .L85
	ldr	r3, [r4, #264]
	cmp	r3, #1
	bne	.L81
	ldr	r2, .L85+4
	movs	r3, #153
	movs	r1, #4
	str	r3, [sp]
	movs	r0, #2
	ldr	r3, .L85+8
	bl	vlog
	movs	r1, #0
	movs	r0, #5
	bl	mcu_send_message
	ldr	r3, [r4, #248]
	ldr	r2, .L85+12
	movs	r1, #2
	str	r1, [r4, #264]
	ldr	r3, [r3]
	ldr	r2, [r2]
	ubfx	r3, r3, #0, #14
	cmp	r3, r2
	beq	.L81
	strd	r2, r3, [sp, #4]
	mov	r0, r1
	movs	r3, #158
	ldr	r2, .L85+16
	str	r3, [sp]
	ldr	r3, .L85+8
	bl	vlog
.L81:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L86:
	.align	2
.L85:
	.word	g_vmcu_dev
	.word	.LC12
	.word	.LANCHOR0+63
	.word	.LANCHOR1
	.word	.LC13
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
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L89
	ldr	r3, [r4, #264]
	cbz	r3, .L87
	movs	r1, #0
	movs	r0, #9
	bl	mcu_send_message
	movs	r0, #100
	bl	msleep
	ldr	r3, [r4, #260]
	mov	r2, #1966110
	movs	r0, #100
	str	r2, [r3]
	pop	{r4, lr}
	b	msleep
.L87:
	pop	{r4, pc}
.L90:
	.align	2
.L89:
	.word	g_vmcu_dev
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
	ldr	r4, .L93
	ldr	r0, [r4, #264]
	cmp	r0, #2
	bne	.L91
	movs	r3, #176
	ldr	r2, .L93+4
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L93+8
	bl	vlog
	movs	r3, #1
	movs	r1, #0
	str	r3, [r4, #264]
	movs	r0, #6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, lr}
	b	mcu_send_message
.L91:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L94:
	.align	2
.L93:
	.word	g_vmcu_dev
	.word	.LC14
	.word	.LANCHOR0+78
	.fnend
	.size	mcu_wrap_end, .-mcu_wrap_end
	.align	1
	.global	mcu_wrap_set_id
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mcu_wrap_set_id, %function
mcu_wrap_set_id:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L101
	ldr	r3, [r4, #264]
	cmp	r3, #2
	bne	.L95
	orr	r1, r0, r1, lsl #2
	ldr	r0, [r4, #268]
	bl	mcu_send_message
	ldr	r3, [r4, #268]
	adds	r3, r3, #1
	cmp	r3, #12
	it	gt
	movgt	r3, #10
	str	r3, [r4, #268]
.L95:
	pop	{r4, pc}
.L102:
	.align	2
.L101:
	.word	g_vmcu_dev
	.fnend
	.size	mcu_wrap_set_id, .-mcu_wrap_set_id
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
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #24
	sub	sp, sp, #24
	ldr	r5, .L124
	movs	r3, #0
	movs	r2, #0
	movs	r1, #0
	strd	r2, [sp, #16]
	mov	r6, r0
	movw	r2, #8464
	mov	r0, r5
	bl	memset
	movs	r1, #4
	ldr	r0, .L124+4
	mov	r4, r5
	bl	ioremap
	ldr	r7, .L124+8
	mov	r1, #8192
	str	r0, [r5, #260]
	ldr	r0, [r7]
	bl	ioremap
	movs	r1, #4
	str	r0, [r5, #252]
	ldr	r0, .L124+12
	bl	ioremap
	ldr	r3, [r5, #260]
	str	r0, [r5, #256]
	ldr	r3, [r3]
	and	r2, r3, #30
	cmp	r2, #30
	beq	.L104
	str	r3, [sp, #4]
	mov	r3, #398
	ldr	r2, .L124+16
	movs	r1, #4
	str	r3, [sp]
	ldr	r3, .L124+20
.L123:
	movs	r0, #2
	bl	vlog
.L103:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L104:
	ldr	r0, .L124+24
	bl	mpi_allocator_find_by_name
	str	r0, [r5, #216]
	cbnz	r0, .L106
	mov	r3, #404
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L124+20
	ldr	r2, .L124+28
	b	.L123
.L106:
	ldr	r3, .L124+32
	ldr	r0, [r3, #4]
	cmp	r0, #0
	beq	.L107
	mov	r2, #416
	movs	r1, #0
	bl	filp_open
	movw	r3, #409
	ldr	r2, .L124+36
	movs	r1, #4
	mov	r5, r0
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r0, #2
	ldr	r3, .L124+20
	bl	vlog
	cmp	r5, #0
	beq	.L107
	mov	r2, #8192
	movs	r1, #0
	ldr	r0, [r4, #252]
	bl	memset
	mov	r2, #8192
	add	r1, r4, #272
	mov	r0, r5
	add	r3, sp, #16
	bl	kernel_read
	mov	r2, #8192
	add	r1, r4, #272
	mov	r8, r0
	ldr	r0, [r4, #252]
	bl	memcpy
	movs	r1, #4
	movs	r0, #2
	ldrd	r2, [sp, #16]
	str	r8, [sp, #4]
	strd	r2, [sp, #8]
	movw	r3, #419
	ldr	r2, .L124+40
	str	r3, [sp]
	ldr	r3, .L124+20
	bl	vlog
	ldr	r3, [r4, #256]
	ldr	r2, [r7]
	movs	r1, #0
	mov	r0, r5
	str	r2, [r3]
	bl	filp_close
.L108:
	ldr	r3, [r4, #260]
	mov	r2, #1966080
	movs	r1, #4
	ldr	r0, .L124+44
	str	r2, [r3]
	bl	ioremap
	movs	r1, #4
	str	r0, [r4, #232]
	ldr	r0, .L124+48
	bl	ioremap
	movs	r1, #4
	str	r0, [r4, #236]
	ldr	r0, .L124+52
	bl	ioremap
	movs	r1, #4
	str	r0, [r4, #240]
	ldr	r0, .L124+56
	bl	ioremap
	movs	r1, #4
	str	r0, [r4, #244]
	ldr	r0, .L124+60
	bl	ioremap
	mov	r2, #10240
	movs	r1, #1
	str	r0, [r4, #248]
	ldr	r0, [r4, #216]
	bl	mpi_allocator_alloc
	mov	r5, r0
	cbz	r0, .L110
	movs	r1, #0
	bl	mpi_buf_create
	str	r0, [r4, #220]
	cbz	r0, .L110
	ldr	r3, .L124+64
	str	r3, [r0, #4]
	mov	r0, r5
	bl	dma_buf_put
	mov	r0, r5
	ldr	r7, [r4, #220]
	bl	get_dma_buf_phy_addr
	str	r0, [r7, #608]
	mov	r0, r5
	ldr	r7, [r4, #220]
	bl	dma_buf_vmap
	ldr	r3, [r4, #220]
	mov	r2, #10240
	movs	r1, #0
	str	r0, [r7, #604]
	ldr	r0, [r3, #604]
	bl	memset
	ldr	r3, [r4, #220]
	movs	r1, #1
	ldrd	r2, r3, [r3, #604]
	add	r3, r3, #2048
	str	r3, [r2]
	mov	r3, #8192
	str	r3, [r2, #4]
	ldr	r3, [r4, #220]
	ldr	r0, [r3, #448]
	bl	dma_buf_end_cpu_access
	ldr	r3, [r4, #220]
	movs	r0, #3
	ldr	r1, [r3, #608]
	bl	mcu_send_message
.L110:
	mov	r1, #3264
	mov	r0, r6
	bl	kstrdup
	ldr	r5, .L124+68
	ldr	r7, .L124+72
	mov	r2, r6
	movs	r1, #8
	str	r4, [r4, #164]
	movs	r3, #1
	str	r3, [r4, #212]
	str	r7, [r4, #208]
	str	r0, [r4]
	ldr	r0, .L124+76
	bl	mpi_dev_register
	movs	r3, #0
	strd	r5, r5, [r4, #192]
	add	r1, r7, #20
	str	r3, [r4, #72]
	sub	r0, r5, #188
	strd	r3, r3, [r4, #80]
	str	r3, [r4, #188]
	ldr	r2, .L124+80
	bl	mpi_sysfs_dev_add
	sub	r1, r5, #192
	mov	r3, r6
	mov	r2, #-1
	ldr	r0, .L124+84
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L112
	bl	wake_up_process
.L112:
	mov	r1, #8192
	ldr	r0, .L124+88
	str	r5, [r4, #200]
	bl	mpi_fs_buf_init
	ldr	r3, [r4, #220]
	movs	r1, #4
	movs	r0, #2
	ldr	r3, [r3, #604]
	add	r2, r3, #2048
	str	r2, [r4, #184]
	ldr	r2, [r4, #176]
	adds	r3, r3, #64
	str	r2, [r4, #180]
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	movw	r3, #461
	ldr	r2, .L124+92
	str	r3, [sp]
	ldr	r3, .L124+20
	bl	vlog
	movs	r2, #1
	movs	r3, #10
	strd	r2, r3, [r4, #264]
	b	.L103
.L107:
	mov	r2, #8192
	ldr	r1, [r4, #252]
	ldr	r0, .L124+96
	bl	memcpy
	b	.L108
.L125:
	.align	2
.L124:
	.word	g_vmcu_dev
	.word	-12875260
	.word	.LANCHOR2
	.word	-16293820
	.word	.LC15
	.word	.LANCHOR0+91
	.word	.LC16
	.word	.LC17
	.word	.LANCHOR1
	.word	.LC18
	.word	.LC19
	.word	-10747896
	.word	-10747892
	.word	-10747856
	.word	-10747852
	.word	-6290412
	.word	.LC20
	.word	g_vmcu_dev+192
	.word	.LANCHOR0+104
	.word	g_vmcu_dev+4
	.word	.LANCHOR2+4
	.word	vmcu_thread_fn
	.word	g_vmcu_dev+168
	.word	.LC21
	.word	g_vmcu_dev+272
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
	push	{r4, lr}
	.save {r4, lr}
	movs	r3, #1
	ldr	r4, .L157
	ldr	r0, [r4, #260]
	str	r3, [r4, #204]
	cbz	r0, .L127
	bl	iounmap
.L127:
	ldr	r0, [r4, #252]
	cbz	r0, .L128
	bl	iounmap
.L128:
	ldr	r0, [r4, #256]
	cbz	r0, .L129
	bl	iounmap
.L129:
	ldr	r0, [r4, #232]
	cbz	r0, .L130
	bl	iounmap
.L130:
	ldr	r0, [r4, #236]
	cbz	r0, .L132
	bl	iounmap
	ldr	r3, [r4, #236]
	cbz	r3, .L132
	ldr	r0, [r4, #240]
	bl	iounmap
.L132:
	ldr	r0, [r4, #244]
	cbz	r0, .L134
	bl	iounmap
.L134:
	ldr	r0, [r4, #248]
	cbz	r0, .L135
	bl	iounmap
.L135:
	ldr	r0, .L157+4
	pop	{r4, lr}
	b	mpi_dev_unregister
.L158:
	.align	2
.L157:
	.word	g_vmcu_dev
	.word	g_vmcu_dev+4
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
	ldr	r0, .L160
	bl	vmcu_probe
	movs	r0, #0
	pop	{r3, pc}
.L161:
	.align	2
.L160:
	.word	.LC22
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
	.set	.LANCHOR0,. + 0
	.type	__func__.37330, %object
	.size	__func__.37330, 13
__func__.37330:
	.ascii	"vmcu_version\000"
	.type	__func__.37372, %object
	.size	__func__.37372, 13
__func__.37372:
	.ascii	"vmcu_release\000"
	.type	__func__.37343, %object
	.size	__func__.37343, 11
__func__.37343:
	.ascii	"vmcu_ioctl\000"
	.type	__func__.37365, %object
	.size	__func__.37365, 10
__func__.37365:
	.ascii	"vmcu_open\000"
	.type	__func__.37284, %object
	.size	__func__.37284, 16
__func__.37284:
	.ascii	"mcu_wrap_config\000"
	.type	__func__.37298, %object
	.size	__func__.37298, 15
__func__.37298:
	.ascii	"mcu_wrap_start\000"
	.type	__func__.37305, %object
	.size	__func__.37305, 13
__func__.37305:
	.ascii	"mcu_wrap_end\000"
	.type	__func__.37408, %object
	.size	__func__.37408, 11
__func__.37408:
	.ascii	"vmcu_probe\000"
	.space	2
	.type	vmcu_ioctls, %object
	.size	vmcu_ioctls, 20
vmcu_ioctls:
	.word	-1068733184
	.word	0
	.word	vmcu_version
	.word	.LC23
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
	.type	mcu_pm_ops, %object
	.size	mcu_pm_ops, 92
mcu_pm_ops:
	.space	56
	.word	mcu_wrap_suspend
	.word	mcu_wrap_resume
	.word	mcu_wrap_suspend
	.word	mcu_wrap_resume
	.word	mcu_wrap_suspend
	.word	mcu_wrap_resume
	.space	12
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
	.word	-9445376
	.type	mcu_driver, %object
	.size	mcu_driver, 104
mcu_driver:
	.space	20
	.space	64
	.word	mcu_pm_ops
	.space	8
	.space	8
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	isp_max_h, %object
	.size	isp_max_h, 4
isp_max_h:
	.space	4
	.type	mcu_fw_path, %object
	.size	mcu_fw_path, 4
mcu_fw_path:
	.space	4
	.type	g_vmcu_dev, %object
	.size	g_vmcu_dev, 8464
g_vmcu_dev:
	.space	8464
	.type	isp_max_w, %object
	.size	isp_max_w, 4
isp_max_w:
	.space	4
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
	.ascii	"\033[1;31mstruct vmcu_version mismatching\033[0m\000"
.LC1:
	.ascii	"\033[1;32mvmcu refcnt %d\033[0m\000"
.LC2:
	.ascii	"mcu wrap suspend = %08x\000"
.LC3:
	.ascii	"vmcu_ioctl: nr = %d, index = %d dev = %p\000"
.LC4:
	.ascii	"\033[1;31mindex = %d\033[0m\000"
.LC5:
	.ascii	"\033[1;31mioctl is null\033[0m\000"
.LC6:
	.ascii	"[%x]: %x\000"
.LC7:
	.ascii	"\033[1;31mvalloc open fail\033[0m\000"
.LC8:
	.ascii	"\033[1;32mconfig_addr=%p, isp_max_h=%d\033[0m\000"
.LC9:
	.ascii	"\033[1;32misp line = %08x, enc_line = %08x\033[0m\000"
.LC10:
	.ascii	"\033[1;32mg_vmcu_dev.plog_buf->paddr = %x, wrap_lin"
	.ascii	"e = %d\033[0m\000"
.LC11:
	.ascii	"mcu wrap resume ok = %08x\000"
.LC12:
	.ascii	"\033[1;32msend wrap start\033[0m\000"
.LC13:
	.ascii	"\033[1;31misp input height config err, config h= %d"
	.ascii	", real h = %d\033[0m\000"
.LC14:
	.ascii	"\033[1;32msend wrap end\033[0m\000"
.LC15:
	.ascii	"\033[1;32mmcu status is not reset = %08x, please ch"
	.ascii	"eck\033[0m\000"
.LC16:
	.ascii	"valloc\000"
.LC17:
	.ascii	"\033[1;31mgetting alloc fail %p\033[0m\000"
.LC18:
	.ascii	"\033[1;32mopen fp = %p\033[0m\000"
.LC19:
	.ascii	"\033[1;32msize = %d, pos = %d\033[0m\000"
.LC20:
	.ascii	"mcu\000"
.LC21:
	.ascii	"\033[1;32mlog buf = %08x, len = %d\033[0m\000"
.LC22:
	.ascii	"vmcu\000"
.LC23:
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
