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
	.file	"rockit_isp.c"
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
	.type	rockit_rkisp_ioctl, %function
rockit_rkisp_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	rockit_rkisp_ioctl, .-rockit_rkisp_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_open, %function
rockit_rkisp_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	rockit_rkisp_open, .-rockit_rkisp_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_buf_que, %function
rockit_rkisp_buf_que:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r8, r1
	.pad #20
	sub	sp, sp, #20
	mov	r6, r0
	cmp	r0, #0
	beq	.L4
	ldr	r5, [r0, #248]
	cmp	r5, #0
	beq	.L14
	ldrb	r0, [r5]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L4
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r9, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	fp, .L23+8
	movs	r7, #0
.L5:
	movs	r1, #0
	mov	r0, r6
	bl	mpi_node_request_buf
	mov	r4, r0
	cmp	r0, #0
	beq	.L11
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L23
	movs	r0, #0
	str	r3, [sp, #12]
	ldr	r3, [r6]
	str	r3, [sp, #8]
	ldr	r3, [r6, #44]
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	movs	r3, #215
	str	r3, [sp]
	ldr	r3, .L23+4
	bl	vlog
	adds	r1, r5, #4
	movs	r3, #0
.L9:
	ldr	r2, [r1, r3, lsl #3]
	cbz	r2, .L6
	ldr	r0, [r2]
	ldr	r2, [r4]
	cmp	r0, r2
	bne	.L22
.L7:
	add	r10, r5, r3, lsl #3
	movs	r2, #1
	strb	r2, [r10, #8]
	ldr	r1, [r6, #56]
	strd	r6, r4, [r5, #80]
	strb	r2, [r5, #48]
	str	r1, [r5, #72]
	ldr	r2, [r4]
	str	r2, [r5, #64]
	ldr	r2, [r4, #64]
	str	r2, [r5, #100]
	ldr	r2, [r6, #44]
	ldr	r2, [r2, #44]
	str	r2, [r5, #52]
	ldr	r2, [fp]
	cbz	r2, .L10
	add	r0, r5, #48
	blx	r2
	cmp	r0, #0
	bge	.L10
	movs	r2, #0
	mov	r0, r4
	strb	r2, [r10, #8]
	strb	r2, [r5, #1]
	bl	mpi_obj_unref
.L11:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r9	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	adds	r0, r4, #0
	it	ne
	movne	r0, #1
.L4:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L6:
	add	r2, r5, r3, lsl #3
	str	r4, [r2, #4]
	b	.L7
.L22:
	adds	r3, r3, #1
	cmp	r3, #5
	bne	.L9
	b	.L7
.L10:
	adds	r7, r7, #1
	cmp	r7, r8
	bne	.L5
	b	.L11
.L14:
	mov	r0, r5
	b	.L4
.L24:
	.align	2
.L23:
	.word	.LC0
	.word	.LANCHOR0
	.word	.LANCHOR1
	.fnend
	.size	rockit_rkisp_buf_que, .-rockit_rkisp_buf_que
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	isp_destroy_ctx, %function
isp_destroy_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	kfree
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	isp_destroy_ctx, .-isp_destroy_ctx
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_handle_message, %function
rockit_rkisp_handle_message:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r0
	ldr	r3, [r0, #8]
	mov	r6, r1
	cmp	r3, #1
	bne	.L28
	cbnz	r0, .L29
	movw	r3, #261
	ldr	r2, .L60
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L60+4
	bl	vlog
.L28:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L29:
	ldr	r4, [r0, #248]
	cbz	r4, .L30
	cmp	r1, #5
	beq	.L31
	cmp	r1, #6
	beq	.L32
	cmp	r1, #0
	bne	.L28
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
.L34:
	ldrb	r2, [r4]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L28
	cmp	r3, #0
	beq	.L28
	movs	r1, #1
	mov	r0, r5
	bl	rockit_rkisp_buf_que
	b	.L28
.L30:
	mov	r3, #270
	ldr	r2, .L60+8
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L60+4
	mov	r0, r4
	bl	vlog
	cbz	r6, .L39
	cmp	r6, #5
	bne	.L28
.L31:
	movs	r3, #1
	movs	r1, #5
	strb	r3, [r4]
	mov	r0, r5
	bl	rockit_rkisp_buf_que
	ldr	r0, .L60+12
.L59:
	bl	printk
	b	.L28
.L39:
	mov	r3, r4
	b	.L34
.L32:
	movs	r3, #0
	mov	r5, r4
	adds	r4, r4, #48
	mov	r6, r3
	strb	r3, [r5], #8
.L38:
	ldr	r0, [r5, #-4]
	cbz	r0, .L37
	ldrb	r3, [r5]	@ zero_extendqisi2
	cbz	r3, .L37
	bl	mpi_obj_unref
	str	r6, [r5, #-4]
.L37:
	adds	r5, r5, #8
	cmp	r4, r5
	bne	.L38
	ldr	r0, .L60+16
	b	.L59
.L61:
	.align	2
.L60:
	.word	.LC1
	.word	.LANCHOR0+21
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.fnend
	.size	rockit_rkisp_handle_message, .-rockit_rkisp_handle_message
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_compat_ioctl, %function
rockit_rkisp_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	rockit_rkisp_compat_ioctl, .-rockit_rkisp_compat_ioctl
	.align	1
	.global	rockit_rkisp_mpibuf_done
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_mpibuf_done, %function
rockit_rkisp_mpibuf_done:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r5, r0
	.pad #20
	sub	sp, sp, #20
	cbnz	r0, .L64
	movs	r3, #99
	ldr	r2, .L105
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L105+4
.L103:
	bl	vlog
.L94:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L64:
	ldr	r6, [r0, #32]
	cbnz	r6, .L66
	movs	r3, #104
	ldr	r2, .L105+8
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L105+4
	mov	r0, r6
	b	.L103
.L66:
	ldrb	r1, [r0, #2]	@ zero_extendqisi2
	cbz	r1, .L67
	movs	r1, #1
	mov	r0, r6
	bl	rockit_rkisp_buf_que
	movs	r3, #0
	strb	r3, [r5, #2]
	b	.L94
.L67:
	ldr	r3, [r0, #28]
	cmp	r3, #0
	beq	.L68
	mov	r0, r6
	bl	mpi_node_request_buf
	mov	r4, r0
.L69:
	cmp	r4, #0
	beq	.L94
	ldr	r3, [r5, #64]
	movs	r7, #0
	str	r3, [r4, #240]
	ldr	r3, [r5, #60]
	str	r3, [r4, #236]
	ldr	r3, .L105+12
	ldr	r2, [r5, #80]
	sub	r8, r3, #72
.L71:
	ldr	r1, [r3, r7, lsl #3]
	cmp	r2, r1
	beq	.L70
	adds	r7, r7, #1
	cmp	r7, #24
	bne	.L71
	movs	r3, #130
	ldr	r2, .L105+16
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L105+4
	movs	r0, #0
	bl	vlog
.L70:
	add	r7, r8, r7, lsl #3
	mov	r2, #1000
	ldr	r3, [r7, #76]
	str	r3, [r4, #256]
	ldr	r3, [r5, #68]
	str	r3, [r4, #244]
	ldr	r3, [r5, #72]
	str	r3, [r4, #248]
	add	r3, sp, #12
	ldrd	r0, [r5, #104]
	bl	div_s64_rem
	strd	r0, [r4, #288]
	ldr	r3, [r5, #100]
	str	r3, [r4, #284]
	ldr	r3, [r5, #28]
	str	r3, [r4, #200]
	ldrb	r2, [r5, #3]	@ zero_extendqisi2
	cbz	r2, .L73
	orr	r3, r3, #4
.L74:
	movs	r2, #0
	str	r3, [r4, #200]
	str	r2, [r4, #312]
	mov	r1, r4
	mov	r0, r6
	bl	mpi_node_buffer_out
	mov	r3, sp
	bic	r2, r3, #8160
	bic	r2, r2, #31
	ldr	r3, [r2, #4]
	bic	r3, r3, #-16777216
	bic	r3, r3, #255
	cbnz	r3, .L75
.L78:
	ldr	r3, [r4, #64]
	cbnz	r3, .L102
.L104:
	mov	r0, r4
	bl	mpi_obj_unref
	b	.L94
.L68:
	ldr	r4, [r0, #36]
	b	.L69
.L73:
	orr	r3, r3, #2
	b	.L74
.L75:
	ldr	r3, [r2, #4]
	tst	r3, #983040
	bne	.L78
	ldr	r0, .L105+20
	movs	r7, #8
	bl	printk
	ldr	r8, .L105+24
	ldr	r9, .L105+28
.L79:
	subs	r7, r7, #1
	beq	.L78
	ldr	r3, [r8, #4]
	mov	r0, r9
	blx	r3
	b	.L79
.L102:
	ldr	r7, [r6, #248]
	cmp	r7, #0
	beq	.L104
	adds	r1, r7, #4
	movs	r3, #0
.L81:
	ldr	r2, [r1, r3, lsl #3]
	cbz	r2, .L82
	ldr	r0, [r4]
	ldr	r2, [r2]
	cmp	r0, r2
	bne	.L82
	add	r3, r7, r3, lsl #3
	movs	r2, #0
	strb	r2, [r3, #8]
.L83:
	mov	r0, r4
	bl	mpi_obj_unref
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	cbz	r3, .L84
	movs	r1, #1
	mov	r0, r6
	bl	rockit_rkisp_buf_que
	cbz	r0, .L84
	movs	r3, #0
	strb	r3, [r5, #1]
	b	.L94
.L82:
	adds	r3, r3, #1
	cmp	r3, #5
	bne	.L81
	b	.L83
.L84:
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	strb	r3, [r7, #1]
	b	.L94
.L106:
	.align	2
.L105:
	.word	.LC5
	.word	.LANCHOR0+47
	.word	.LC6
	.word	.LANCHOR0+72
	.word	.LC8
	.word	.LC7
	.word	arm_delay_ops
	.word	644245000
	.fnend
	.size	rockit_rkisp_mpibuf_done, .-rockit_rkisp_mpibuf_done
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	isp_create_ctx, %function
isp_create_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L108
	mov	r1, #3520
	ldr	r0, [r3, #48]
	b	kmem_cache_alloc
.L109:
	.align	2
.L108:
	.word	kmalloc_caches
	.fnend
	.size	isp_create_ctx, .-isp_create_ctx
	.align	1
	.global	rockit_rkisp_dev_register
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_dev_register, %function
rockit_rkisp_dev_register:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r1, #3264
	ldr	r6, .L127
	mov	r8, r0
	mov	r9, #0
	mov	r10, #1
	mov	fp, r9
	ldr	r0, [r6, #8]
	bl	kmem_cache_alloc
	movs	r2, #164
	movs	r1, #0
	mov	r4, r0
	bl	memset
	ldr	r3, .L127+4
	add	r7, r4, #60
	str	r7, [r4, #60]
	str	r7, [r4, #64]
	str	r3, [r4, #68]
	ldr	r3, .L127+8
	str	r3, [r4, #76]
	movs	r3, #2
	strd	r3, r3, [r4, #48]
	movs	r3, #3
	str	r3, [r4, #56]
.L111:
	ldr	r2, [r4, #52]
	ldr	r0, [r6, #20]
	cmp	r2, fp
	bgt	.L115
	ldr	r2, [r4, #48]
	add	r9, r9, #1
	cmp	r2, r9
	bgt	.L120
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r5, r0
	cbz	r0, .L118
	mov	r3, #-1
	movs	r1, #2
	str	r3, [r0, #16]
	strd	r1, r3, [r0, #8]
	mov	r1, #3520
	ldr	r0, [r6, #20]
	bl	kmem_cache_alloc
	str	r0, [r5, #20]
	cbz	r0, .L118
	movs	r3, #16
	movs	r2, #1
	strd	r3, r3, [r0, #8]
	strd	r2, r3, [r0]
	str	r3, [r0, #16]
	ldr	r3, [r4, #64]
	str	r5, [r4, #64]
	strd	r7, r3, [r5]
	str	r5, [r3]
.L118:
	mov	r2, r8
	movs	r1, #8
	mov	r0, r4
	bl	mpi_dev_register
	ldr	r3, .L127+12
	mov	r0, r4
	ldr	r1, .L127+16
	str	r3, [r4, #80]
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	mpi_sysfs_dev_add
.L115:
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r5, r0
	cmp	r0, #0
	beq	.L113
	strd	r10, r9, [r0, #8]
	mov	r1, #3520
	str	fp, [r0, #16]
	ldr	r0, [r6, #4]
	bl	kmem_cache_alloc
	str	r0, [r5, #20]
	cbz	r0, .L113
	mov	r2, #2160
	mov	r3, #3840
	strd	r3, r2, [r0, #8]
	movs	r2, #4
	str	r2, [r0, #36]
	ldr	r2, [r0, #64]
	str	r10, [r0, #32]
	orr	r2, r2, #3
	str	r2, [r0, #64]
	ldr	r2, [r0, #68]
	strd	r10, r10, [r0, #40]
	orr	r2, r2, #3
	str	r2, [r0, #68]
	ldr	r2, [r0, #56]
	orr	r2, r2, #3
	str	r2, [r0, #56]
	ldr	r2, [r0, #60]
	orr	r2, r2, #3
	str	r2, [r0, #60]
	ldr	r2, [r0, #72]
	orr	r2, r2, #3
	str	r2, [r0, #72]
	ldr	r2, [r0, #76]
	orr	r2, r2, #14
	str	r2, [r0, #76]
	ldr	r2, [r4, #64]
	str	r5, [r4, #64]
	strd	r7, r2, [r5]
	str	r5, [r2]
.L113:
	add	fp, fp, #1
	b	.L111
.L120:
	mov	fp, #0
	b	.L111
.L128:
	.align	2
.L127:
	.word	kmalloc_caches
	.word	isp_create_ctx
	.word	isp_destroy_ctx
	.word	rockit_rkisp_handle_message
	.word	.LANCHOR0+264
	.fnend
	.size	rockit_rkisp_dev_register, .-rockit_rkisp_dev_register
	.section	.init.text,"ax",%progbits
	.align	1
	.global	rockit_rkisp_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_init, %function
rockit_rkisp_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	movs	r4, #0
	ldr	r5, .L136
	.pad #20
	sub	sp, sp, #20
	mov	r1, r4
	strd	r4, r4, [sp, #4]
	ldr	r0, [r5]
	str	r4, [sp, #12]
	bl	rkisp_rockit_function_register
	movs	r1, #1
	str	r0, [r5]
	ldr	r0, .L136+4
	bl	rkisp_rockit_function_register
	add	r0, sp, #4
	bl	rkisp_rockit_get_ispdev
	add	r5, sp, #4
.L131:
	ldr	r0, [r5], #4
	cbz	r0, .L130
	bl	rockit_rkisp_dev_register
.L130:
	adds	r4, r4, #1
	cmp	r4, #3
	bne	.L131
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L137:
	.align	2
.L136:
	.word	.LANCHOR1
	.word	rockit_rkisp_mpibuf_done
	.fnend
	.size	rockit_rkisp_init, .-rockit_rkisp_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	rockit_rkisp_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_exit, %function
rockit_rkisp_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	add	r0, sp, #4
	movs	r3, #0
	strd	r3, r3, [sp, #4]
	str	r3, [sp, #12]
	bl	rkisp_rockit_get_ispdev
	mov	r5, r0
	cbnz	r0, .L138
	add	r4, sp, #4
.L143:
	ldr	r0, [r4], #4
	cbz	r0, .L141
	bl	mpi_dev_find_by_name
	cbz	r0, .L141
	bl	mpi_dev_unregister
.L141:
	adds	r5, r5, #1
	cmp	r5, #3
	bne	.L143
.L138:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
	.fnend
	.size	rockit_rkisp_exit, .-rockit_rkisp_exit
	.global	rockit_buf_que
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.40799, %object
	.size	__func__.40799, 21
__func__.40799:
	.ascii	"rockit_rkisp_buf_que\000"
	.type	__func__.40820, %object
	.size	__func__.40820, 26
__func__.40820:
	.ascii	"rockit_rkisp_node_message\000"
	.type	__func__.40777, %object
	.size	__func__.40777, 25
__func__.40777:
	.ascii	"rockit_rkisp_mpibuf_done\000"
	.type	v4l2_rockit_fmt, %object
	.size	v4l2_rockit_fmt, 192
v4l2_rockit_fmt:
	.word	842094158
	.word	0
	.word	909203022
	.word	2
	.word	1345466932
	.word	7
	.word	1448695129
	.word	9
	.word	1498831189
	.word	10
	.word	875836505
	.word	14
	.word	1431918169
	.word	17
	.word	1498765654
	.word	18
	.word	1346520914
	.word	65536
	.word	1329743698
	.word	65538
	.word	875836498
	.word	65540
	.word	825770306
	.word	131072
	.word	1196573255
	.word	131073
	.word	1195528775
	.word	131074
	.word	1111967570
	.word	131075
	.word	808535874
	.word	131076
	.word	808534599
	.word	131077
	.word	808534338
	.word	131078
	.word	808535890
	.word	131079
	.word	842090306
	.word	131080
	.word	842089031
	.word	131081
	.word	842088770
	.word	131082
	.word	842090322
	.word	131083
	.word	844257602
	.word	131085
	.type	rockit_rkisp_driver_fops, %object
	.size	rockit_rkisp_driver_fops, 128
rockit_rkisp_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	0
	.word	rockit_rkisp_ioctl
	.word	rockit_rkisp_compat_ioctl
	.word	0
	.space	4
	.word	rockit_rkisp_open
	.space	4
	.word	0
	.space	60
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	rockit_buf_que, %object
	.size	rockit_buf_que, 4
rockit_buf_que:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"%s,%d:vvi(%s-%d) processing, bufs: %d\000"
.LC1:
	.ascii	"\033[31m\033[1m%s,%d:err:node null\033[0m\000"
.LC2:
	.ascii	"\033[31m\033[1m%s,%d:err: ctx is null\033[0m\000"
.LC3:
	.ascii	"rkisp queue buf done\000"
.LC4:
	.ascii	"rockit isp stream off\000"
.LC5:
	.ascii	"\033[31m\033[1m%s,%d:err, rockit_cfg null\033[0m\000"
.LC6:
	.ascii	"\033[31m\033[1m%s,%d:err, node null\033[0m\000"
.LC7:
	.ascii	"wait 7 ms\000"
.LC8:
	.ascii	"\033[31m\033[1m%s,%d:the format is not find!\033[0m"
	.ascii	"\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
