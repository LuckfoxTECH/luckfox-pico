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
	.file	"mpp_vcodec_base.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpp_vcodec_clear_buf_resource:					
	.asciz 	"mpp_vcodec_clear_buf_resource"					
__kstrtabns_mpp_vcodec_clear_buf_resource:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_unbind, %function
mpp_vcodec_unbind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_vcodec_unbind, .-mpp_vcodec_unbind
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_chan_clear_buf_resource, %function
mpp_chan_clear_buf_resource:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r0, [r0, #164]
	cbz	r0, .L3
	bl	hal_bufs_deinit
	movs	r3, #0
	str	r3, [r4, #164]
.L3:
	ldr	r0, [r4, #168]
	cbz	r0, .L4
	ldr	r1, .L19
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #168]
.L4:
	ldr	r0, [r4, #176]
	cbz	r0, .L5
	ldr	r1, .L19
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #176]
.L5:
	ldr	r0, [r4, #172]
	cbz	r0, .L6
	ldr	r1, .L19
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #172]
.L6:
	movs	r3, #0
	strd	r3, r3, [r4, #152]
	str	r3, [r4, #180]
	str	r3, [r4, #160]
	pop	{r4, pc}
.L20:
	.align	2
.L19:
	.word	.LANCHOR0
	.fnend
	.size	mpp_chan_clear_buf_resource, .-mpp_chan_clear_buf_resource
	.align	1
	.global	mpp_vcodec_clear_buf_resource
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_clear_buf_resource, %function
mpp_vcodec_clear_buf_resource:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L22
	add	r0, r4, #16
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #200
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #384
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #568
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #752
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #936
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #1120
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #1304
	bl	mpp_chan_clear_buf_resource
	movs	r0, #0
	pop	{r4, pc}
.L23:
	.align	2
.L22:
	.word	.LANCHOR1
	.fnend
	.size	mpp_vcodec_clear_buf_resource, .-mpp_vcodec_clear_buf_resource
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_msg_handle, %function
mpp_vcodec_msg_handle:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r5, r0
	mov	r7, r1
	mov	r8, r2
	bl	get_mpidev_ops
	mov	r6, r0
	cbnz	r0, .L25
	movs	r1, #110
	ldr	r0, .L48
	bl	printk
	mov	r0, #-1
.L24:
	pop	{r4, r5, r6, r7, r8, pc}
.L25:
	bl	get_mpidev_ops
	mov	r4, r0
	cbnz	r0, .L27
	movs	r1, #77
	ldr	r0, .L48
	bl	printk
.L28:
	ldr	r3, [r6, #8]
	cbnz	r3, .L32
.L47:
	movs	r0, #0
	b	.L24
.L27:
	ldr	r3, [r0, #12]
	cbnz	r3, .L29
.L46:
	movs	r4, #0
	b	.L28
.L29:
	mov	r0, r5
	blx	r3
	cmp	r0, #0
	beq	.L46
	cmp	r0, #44
	beq	.L46
	ldr	r2, [r0, #-32]
	cmp	r2, #1
	bne	.L31
	ldr	r3, .L48+4
	ldr	r4, [r3, #1568]
	b	.L28
.L31:
	movs	r1, #94
	ldr	r0, .L48+8
	bl	printk
	b	.L46
.L32:
	mov	r2, r8
	mov	r1, r7
	mov	r0, r5
	blx	r3
	cmp	r0, #0
	beq	.L47
	cmp	r4, #0
	beq	.L47
	mov	r0, r4
	bl	vcodec_thread_trigger
	b	.L47
.L49:
	.align	2
.L48:
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC1
	.fnend
	.size	mpp_vcodec_msg_handle, .-mpp_vcodec_msg_handle
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	__refcount_sub_and_test.constprop.5, %function
__refcount_sub_and_test.constprop.5:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r0, r1
	pld	[r1]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_fetch_sub
1:	ldrex	r3, [r0]
	sub	r2, r3, #1
	strex	r1, r2, [r0]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r3, #1
	bne	.L51
	mov	r0, r3
.L52:
	pop	{r3, pc}
.L51:
	cmp	r3, #0
	bgt	.L53
	movs	r1, #3
	bl	refcount_warn_saturate
.L53:
	movs	r0, #0
	b	.L52
	.fnend
	.size	__refcount_sub_and_test.constprop.5, .-__refcount_sub_and_test.constprop.5
	.align	1
	.global	vcodec_create_mpi_dev
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_create_mpi_dev, %function
vcodec_create_mpi_dev:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	bl	get_mpidev_ops
	ldr	r4, .L62
	ldr	r3, [r0]
	cbz	r3, .L55
	ldr	r2, [r4, #8]
	cbnz	r2, .L57
	ldr	r1, .L62+4
	ldr	r0, [r4, #4]
	blx	r3
	str	r0, [r4, #8]
.L55:
	ldr	r3, [r4, #8]
	cbnz	r3, .L57
	movs	r1, #136
	ldr	r0, .L62+8
	bl	printk
.L57:
	movs	r0, #0
	pop	{r4, pc}
.L63:
	.align	2
.L62:
	.word	.LANCHOR1
	.word	.LANCHOR2
	.word	.LC2
	.fnend
	.size	vcodec_create_mpi_dev, .-vcodec_create_mpi_dev
	.align	1
	.global	mpp_vcodec_get_free_chan
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_get_free_chan, %function
mpp_vcodec_get_free_chan:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #1
	mov	r2, r0
	push	{r3, lr}
	.save {r3, lr}
	bne	.L65
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L73
	movs	r0, #0
.L67:
	ldr	r1, [r3, #56]
	cbz	r1, .L66
	adds	r0, r0, #1
	adds	r3, r3, #184
	cmp	r0, #8
	bne	.L67
	mov	r0, #-1
.L66:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L64:
	pop	{r3, pc}
.L65:
	movs	r1, #193
	ldr	r0, .L73+4
	bl	printk
	movs	r0, #0
	b	.L64
.L74:
	.align	2
.L73:
	.word	.LANCHOR1
	.word	.LC1
	.fnend
	.size	mpp_vcodec_get_free_chan, .-mpp_vcodec_get_free_chan
	.align	1
	.global	mpp_vcodec_get_chan_entry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_get_chan_entry, %function
mpp_vcodec_get_chan_entry:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r1, #1
	mov	r2, r1
	push	{r3, lr}
	.save {r3, lr}
	bne	.L76
	cmp	r0, #8
	bls	.L77
	mov	r2, r0
	movs	r3, #8
	movs	r1, #208
	ldr	r0, .L80
	bl	printk
.L79:
	movs	r0, #0
	b	.L75
.L77:
	ldr	r2, .L80+4
	movs	r3, #184
	mla	r0, r3, r0, r2
	adds	r0, r0, #16
.L75:
	pop	{r3, pc}
.L76:
	movs	r1, #215
	ldr	r0, .L80+8
	bl	printk
	b	.L79
.L81:
	.align	2
.L80:
	.word	.LC3
	.word	.LANCHOR1
	.word	.LC1
	.fnend
	.size	mpp_vcodec_get_chan_entry, .-mpp_vcodec_get_chan_entry
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_bind, %function
mpp_vcodec_bind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	bl	get_mpidev_ops
	mov	r4, r0
	cbnz	r0, .L83
	movs	r1, #48
	ldr	r0, .L90
	bl	printk
.L82:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L83:
	ldr	r3, [r0, #16]
	cbz	r3, .L88
	mov	r0, r5
	blx	r3
	mov	r6, r0
.L85:
	ldr	r3, [r4, #20]
	cbz	r3, .L89
	mov	r0, r5
	blx	r3
	mov	r5, r0
.L86:
	mov	r1, r5
	mov	r0, r6
	bl	mpp_vcodec_get_chan_entry
	ldr	r4, [r0, #40]
	cbnz	r4, .L87
	mov	r3, r6
	mov	r2, r5
	movs	r1, #59
	ldr	r0, .L90+4
	bl	printk
	b	.L82
.L88:
	mov	r6, #-1
	b	.L85
.L89:
	movs	r5, #1
	b	.L86
.L87:
	add	r4, r0, #44
	b	.L82
.L91:
	.align	2
.L90:
	.word	.LC0
	.word	.LC4
	.fnend
	.size	mpp_vcodec_bind, .-mpp_vcodec_bind
	.align	1
	.global	mpp_vcodec_get_chan_num
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_get_chan_num, %function
mpp_vcodec_get_chan_num:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #1
	mov	r2, r0
	push	{r3, lr}
	.save {r3, lr}
	bne	.L93
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, .L95
	ldr	r0, [r2, #1488]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L92:
	pop	{r3, pc}
.L93:
	movs	r1, #235
	ldr	r0, .L95+4
	bl	printk
	movs	r0, #0
	b	.L92
.L96:
	.align	2
.L95:
	.word	.LANCHOR1
	.word	.LC1
	.fnend
	.size	mpp_vcodec_get_chan_num, .-mpp_vcodec_get_chan_num
	.align	1
	.global	mpp_vcodec_inc_chan_num
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_inc_chan_num, %function
mpp_vcodec_inc_chan_num:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #1
	mov	r2, r0
	bne	.L98
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, .L101
	ldr	r3, [r2, #1488]
	adds	r3, r3, #1
	str	r3, [r2, #1488]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
.L98:
	movs	r1, #253
	ldr	r0, .L101+4
	b	printk
.L102:
	.align	2
.L101:
	.word	.LANCHOR1
	.word	.LC1
	.fnend
	.size	mpp_vcodec_inc_chan_num, .-mpp_vcodec_inc_chan_num
	.align	1
	.global	mpp_vcodec_dec_chan_num
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_dec_chan_num, %function
mpp_vcodec_dec_chan_num:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #1
	mov	r2, r0
	bne	.L104
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, .L107
	ldr	r3, [r2, #1488]
	subs	r3, r3, #1
	str	r3, [r2, #1488]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
.L104:
	mov	r1, #272
	ldr	r0, .L107+4
	b	printk
.L108:
	.align	2
.L107:
	.word	.LANCHOR1
	.word	.LC1
	.fnend
	.size	mpp_vcodec_dec_chan_num, .-mpp_vcodec_dec_chan_num
	.align	1
	.global	mpp_vcodec_chan_setup_hal_bufs
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_setup_hal_bufs, %function
mpp_vcodec_chan_setup_hal_bufs:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r1
	ldrd	r6, r7, [r1, #32]
	.pad #36
	sub	sp, sp, #36
	ldrd	r1, r3, [r0, #152]
	mov	r4, r0
	mul	r2, r7, r6
	muls	r3, r1, r3
	cmp	r2, r3
	bhi	.L110
	ldr	r2, [r5, #40]
	ldr	r3, [r0, #180]
	cmp	r2, r3
	bls	.L111
.L110:
	ldr	r3, [r5, #4]
	cmp	r3, #8
	beq	.L111
	mov	r3, r7
	mov	r2, r6
	mov	r1, #368
	ldr	r0, .L160
	ldr	r9, [r5, #40]
	bl	printk
	ldr	r0, [r4, #164]
	cbz	r0, .L112
	bl	hal_bufs_deinit
	movs	r3, #0
	str	r3, [r4, #164]
.L112:
	add	fp, r6, #63
	add	r10, r7, #63
	bic	r3, fp, #63
	bic	r6, r10, #63
	addw	r2, r3, #511
	addw	r7, r6, #1023
	add	r0, r4, #164
	str	r3, [sp, #12]
	asrs	r7, r7, #10
	asrs	r2, r2, #9
	muls	r7, r3, r7
	it	mi
	addmi	r7, r7, #15
	muls	r2, r6, r2
	it	mi
	addmi	r2, r2, #31
	bic	r7, r7, #15
	asr	fp, fp, #6
	asrs	r2, r2, #5
	add	r8, r9, #2
	lsls	r2, r2, #4
	cmp	r7, r2
	it	cc
	movcc	r7, r2
	addw	r2, r3, #1023
	asrs	r2, r2, #10
	muls	r2, r6, r2
	it	mi
	addmi	r2, r2, #15
	bic	r2, r2, #15
	cmp	r7, r2
	it	cc
	movcc	r7, r2
	addw	r2, r6, #511
	asrs	r2, r2, #9
	muls	r2, r3, r2
	it	mi
	addmi	r2, r2, #31
	asrs	r2, r2, #5
	lsls	r2, r2, #4
	cmp	r7, r2
	it	cc
	movcc	r7, r2
	bl	hal_bufs_init
	mov	r1, fp
	muls	r1, r6, r1
	it	mi
	addmi	r1, r1, #63
	ldr	r2, [r5, #24]
	asrs	r1, r1, #6
	lsls	r1, r1, #8
	add	r1, r1, #8160
	adds	r1, r1, #31
	bic	r1, r1, #8160
	bic	r1, r1, #31
	ldr	r3, [sp, #12]
	cmp	r2, #0
	beq	.L118
	movs	r3, #0
	str	r1, [sp, #16]
	strd	r3, r7, [sp, #20]
	movs	r2, #4
	str	r3, [sp, #28]
	mov	r1, r8
	add	r3, sp, #16
	ldr	r0, [r4, #164]
	bl	hal_bufs_setup
	ldr	r3, [r5, #32]
	ldr	r2, [r5, #36]
	adds	r3, r3, #63
	adds	r2, r2, #63
	bic	r1, r3, #63
	bic	r3, r2, #63
	cmp	r3, r1
	mul	r0, r3, r1
	it	lt
	movlt	r3, r1
	movs	r1, #48
	asrs	r6, r3, #1
	muls	r1, r3, r1
	add	r2, r0, r0, lsl #1
	cmp	r0, #0
	it	lt
	addlt	r0, r0, #63
	addw	r1, r1, #4095
	add	r1, r1, r2, asr #1
	ldr	r2, .L160+4
	add	r0, r6, r0, asr #6
	movs	r6, #240
	muls	r6, r3, r6
	add	r3, r3, r3, lsl #1
	addw	r3, r3, #4095
	ands	r1, r1, r2
	ands	r3, r3, r2
	addw	r0, r0, #4095
	addw	r6, r6, #4095
	cmp	r9, #0
	and	r6, r6, r2
	and	r0, r0, r2
	add	r6, r6, r3
	add	r6, r6, r1
	add	r6, r6, r0
	ble	.L120
	ldr	r3, [r5, #56]
	cmp	r3, #0
	beq	.L121
	add	r1, r1, r0
	add	r6, r6, r1
.L120:
	ldr	r0, [r4, #168]
	cbz	r0, .L122
	ldr	r1, .L160+8
	bl	mpp_buffer_put_with_caller
.L122:
	ldr	r3, .L160+8
	mov	r2, r6
	add	r1, r4, #168
	str	r3, [sp]
	movs	r3, #0
	mov	r0, r3
	bl	mpp_buffer_get_with_tag
	cmp	r0, #0
	bne	.L124
.L127:
	movs	r6, #0
.L125:
	cmp	r8, r6
	bhi	.L129
	ldrd	r2, r3, [r5, #32]
	cmp	r2, r3
	it	cc
	strdcc	r3, r2, [r5, #32]
	ldr	r6, [r5, #32]
	ldr	r3, [r4, #152]
	cmp	r6, r3
	bhi	.L131
.L132:
	ldr	r3, [r5, #32]
	str	r3, [r4, #152]
	ldr	r3, [r5, #36]
	str	r3, [r4, #156]
	ldr	r3, [r5, #40]
	str	r3, [r4, #180]
.L111:
	bl	get_vsm_ops
	mov	r6, r0
	cmp	r0, #0
	bne	.L137
	ldr	r5, [r5, #16]
	ldr	r3, [r4, #160]
	cmp	r5, r3
	bls	.L109
	ldr	r0, [r4, #176]
	cmp	r5, #16384
	it	cc
	movcc	r5, #16384
	cbz	r0, .L136
	ldr	r1, .L160+12
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #176]
.L136:
	ldr	r3, .L160+12
	addw	r2, r5, #4095
	bic	r2, r2, #4080
	add	r1, r4, #176
	bic	r2, r2, #15
	str	r3, [sp]
	movs	r3, #0
	mov	r0, r3
	bl	mpp_ring_buffer_get_with_tag
	cmp	r0, #0
	bne	.L124
	str	r5, [r4, #160]
.L109:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L121:
	lsls	r6, r6, #1
	b	.L120
.L118:
	asr	r10, r10, #6
	strd	r1, r2, [sp, #16]
	muls	r6, r3, r6
	ldr	r3, .L160+16
	mul	fp, fp, r10
	movw	r1, #8191
	cmp	r6, #0
	ldr	r0, [r4, #164]
	str	r7, [sp, #24]
	mov	r2, r6
	it	lt
	addlt	r2, r6, #63
	add	r6, r6, r6, lsl #1
	lsl	r10, fp, #13
	add	r2, r1, r2, asr #6
	add	r1, r1, fp, lsl #6
	add	fp, r10, fp, lsl #12
	ands	r2, r2, r3
	ands	r1, r1, r3
	add	r6, r2, r6, asr #1
	add	r1, r1, fp, asr #1
	add	r3, sp, #16
	cmp	r1, r6
	ite	ge
	strge	r1, [sp, #28]
	strlt	r6, [sp, #28]
	movs	r2, #4
	mov	r1, r8
	bl	hal_bufs_setup
	b	.L127
.L129:
	mov	r1, r6
	ldr	r0, [r4, #164]
	bl	hal_bufs_get_buf
	cbz	r0, .L124
	adds	r6, r6, #1
	b	.L125
.L131:
	cmp	r6, #3072
	bls	.L132
	ldr	r0, [r4, #172]
	cbz	r0, .L133
	ldr	r1, .L160+12
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #172]
.L133:
	adds	r6, r6, #63
	ldr	r3, .L160+12
	bics	r1, r6, #63
	it	mi
	addmi	r1, r1, #31
	asrs	r6, r6, #6
	str	r3, [sp]
	movs	r3, #0
	asrs	r2, r1, #5
	mov	r1, #416
	mov	r0, r3
	muls	r1, r2, r1
	mov	r2, #896
	muls	r6, r2, r6
	sub	r1, r1, #37632
	subs	r1, r1, #224
	sub	r2, r6, #32256
	cmp	r2, r1
	it	lt
	movlt	r2, r1
	add	r1, r4, #172
	bl	mpp_buffer_get_with_tag
	cmp	r0, #0
	beq	.L132
.L124:
	mov	r0, r4
	bl	mpp_chan_clear_buf_resource
	mov	r0, #-1
	b	.L109
.L137:
	movs	r0, #0
	b	.L109
.L161:
	.align	2
.L160:
	.word	.LC5
	.word	-4096
	.word	.LANCHOR0+28
	.word	.LANCHOR0+41
	.word	-8192
	.fnend
	.size	mpp_vcodec_chan_setup_hal_bufs, .-mpp_vcodec_chan_setup_hal_bufs
	.align	1
	.global	mpp_vcodec_chan_entry_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_entry_init, %function
mpp_vcodec_chan_entry_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	mov	r7, r1
	mov	r8, r2
	mov	r9, r3
	bl	get_mpibuf_ops
	mov	r5, r0
	cbnz	r0, .L163
	movw	r2, #465
	ldr	r1, .L169
	ldr	r0, .L169+4
	bl	printk
	mov	r0, #-1
.L162:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L163:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r3, #0
	ldr	r2, .L169+8
	str	r3, [r4, #104]
	add	r0, r4, #64
	str	r3, [r4, #108]
	ldr	r1, .L169+12
	str	r3, [r4, #32]
	str	r9, [r4, #40]
	str	r3, [r4, #20]
	add	r3, r4, #88
	strd	r8, r7, [r4, #8]
	str	r3, [r4, #88]
	str	r3, [r4, #92]
	add	r3, r4, #96
	str	r3, [r4, #96]
	str	r3, [r4, #100]
	bl	__mutex_init
	ldr	r2, .L169+8
	ldr	r1, .L169+16
	add	r0, r4, #76
	bl	__mutex_init
	ldr	r2, .L169+8
	ldr	r1, .L169+20
	add	r0, r4, #48
	bl	__init_waitqueue_head
	ldr	r2, .L169+8
	ldr	r1, .L169+24
	add	r0, r4, #56
	bl	__init_waitqueue_head
	ldr	r3, [r5, #24]
	cbz	r3, .L165
	ldr	r2, [r4, #44]
	cbnz	r2, .L165
	movs	r0, #1
	blx	r3
	str	r0, [r4, #44]
.L165:
	movs	r3, #1
	str	r3, [r4, #24]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	b	.L162
.L170:
	.align	2
.L169:
	.word	.LANCHOR0+72
	.word	.LC6
	.word	.LANCHOR1+1576
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mpp_vcodec_chan_entry_init, .-mpp_vcodec_chan_entry_init
	.align	1
	.global	mpp_vcodec_chan_entry_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_entry_deinit, %function
mpp_vcodec_chan_entry_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	bl	get_mpibuf_ops
	cbnz	r0, .L172
	mov	r2, #500
	ldr	r1, .L175
	ldr	r0, .L175+4
	bl	printk
	mov	r0, #-1
.L171:
	pop	{r3, r4, r5, pc}
.L172:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r5, #0
	mov	r2, #-1
	str	r5, [r4, #40]
	str	r5, [r4, #24]
	str	r5, [r4, #36]
	str	r2, [r4, #4]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	str	r5, [r4, #32]
	ldr	r3, [r0, #28]
	cbz	r3, .L174
	ldr	r0, [r4, #44]
	blx	r3
	str	r5, [r4, #44]
	mov	r0, r5
	b	.L171
.L174:
	mov	r0, r3
	b	.L171
.L176:
	.align	2
.L175:
	.word	.LANCHOR0+99
	.word	.LC6
	.fnend
	.size	mpp_vcodec_chan_entry_deinit, .-mpp_vcodec_chan_entry_deinit
	.align	1
	.global	stream_packet_free
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	stream_packet_free, %function
stream_packet_free:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	subs	r0, r0, #20
	str	r0, [sp, #4]
	cbz	r0, .L177
	add	r0, sp, #4
	bl	mpp_packet_deinit
.L177:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
	.fnend
	.size	stream_packet_free, .-stream_packet_free
	.align	1
	.global	mpp_vcodec_stream_clear
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_stream_clear, %function
mpp_vcodec_stream_clear:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	add	r8, r0, #64
	mov	r4, r0
	mov	r7, r4
	mov	r0, r8
	bl	mutex_lock
	ldr	r5, [r7, #88]!
	ldr	r6, [r5], #-12
	subs	r6, r6, #12
.L183:
	add	r3, r5, #12
	cmp	r3, r7
	bne	.L185
	mov	r0, r8
	add	r8, r4, #76
	bl	mutex_unlock
	mov	r7, r4
	mov	r0, r8
	bl	mutex_lock
	ldr	r5, [r7, #96]!
	ldr	r6, [r5], #-12
	subs	r6, r6, #12
.L186:
	add	r3, r5, #12
	cmp	r3, r7
	bne	.L188
	mov	r0, r8
	pop	{r4, r5, r6, r7, r8, lr}
	b	mutex_unlock
.L185:
	ldrd	r1, r2, [r5, #12]
	adds	r5, r5, #20
	movs	r0, #1
	str	r2, [r1, #4]
	str	r1, [r2]
	movs	r2, #0
	str	r3, [r5, #-8]
	str	r3, [r5, #-4]
	mov	r1, r5
	bl	__refcount_sub_and_test.constprop.5
	cbz	r0, .L184
	mov	r0, r5
	bl	stream_packet_free
.L184:
	add	r3, r4, #104
	pld	[r3]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r2, [r3]
	sub	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r6, #12]
	mov	r5, r6
	sub	r6, r3, #12
	b	.L183
.L188:
	ldrd	r1, r2, [r5, #12]
	adds	r5, r5, #20
	movs	r0, #1
	str	r2, [r1, #4]
	str	r1, [r2]
	movs	r2, #0
	str	r3, [r5, #-8]
	str	r3, [r5, #-4]
	mov	r1, r5
	bl	__refcount_sub_and_test.constprop.5
	cbz	r0, .L187
	mov	r0, r5
	bl	stream_packet_free
.L187:
	add	r3, r4, #108
	pld	[r3]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r2, [r3]
	sub	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r6, #12]
	mov	r5, r6
	sub	r6, r3, #12
	b	.L186
	.fnend
	.size	mpp_vcodec_stream_clear, .-mpp_vcodec_stream_clear
	.align	1
	.global	mpp_vcodec_get_enc_module_entry
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_get_enc_module_entry, %function
mpp_vcodec_get_enc_module_entry:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L196
	bx	lr
.L197:
	.align	2
.L196:
	.word	.LANCHOR1
	.fnend
	.size	mpp_vcodec_get_enc_module_entry, .-mpp_vcodec_get_enc_module_entry
	.align	1
	.global	enc_chan_update_chan_prior_tab
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_chan_update_chan_prior_tab, %function
enc_chan_update_chan_prior_tab:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L217
	movs	r1, #0
	mov	r0, r1
	mov	r4, r1
	mov	r5, r3
	mov	r2, r3
	str	r1, [r3, #1496]
	str	r1, [r3, #1492]
.L200:
	ldr	r7, [r5, #40]
	cmp	r7, #3
	bne	.L199
	ldrb	r1, [r5, #32]	@ zero_extendqisi2
	add	r7, r2, r0, lsl #3
	adds	r1, r1, #1
	str	r1, [r7, #1508]
	add	r1, r0, #188
	adds	r0, r0, #1
	str	r4, [r2, r1, lsl #3]
	movs	r1, #1
.L199:
	adds	r4, r4, #1
	adds	r5, r5, #184
	cmp	r4, #8
	bne	.L200
	cbz	r1, .L201
	str	r0, [r2, #1492]
.L201:
	ldr	r1, [r2, #1492]
	movs	r0, #0
	add	r2, r2, r1, lsl #3
.L202:
	cmp	r1, #7
	add	r2, r2, #8
	ble	.L203
	movs	r1, #0
.L207:
	adds	r1, r1, #1
	mov	r2, r3
	movs	r4, #0
	mov	r0, r1
.L205:
	ldr	r5, [r3, #1508]
	ldr	r7, [r2, #1516]
	cmp	r5, r7
	bge	.L204
	ldr	r7, [r2, #1512]
	ldr	r4, [r3, #1504]
	str	r7, [r3, #1504]
	ldr	r7, [r2, #1516]
	str	r7, [r3, #1508]
	str	r4, [r2, #1512]
	mov	r4, #-1
	str	r5, [r2, #1516]
.L204:
	adds	r0, r0, #1
	adds	r2, r2, #8
	cmp	r0, #8
	bne	.L205
	cbz	r4, .L206
	cmp	r1, #7
	add	r3, r3, #8
	bne	.L207
.L206:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r4, r5, r6, r7, pc}
.L203:
	str	r0, [r2, #1500]
	adds	r1, r1, #1
	str	r0, [r2, #1496]
	b	.L202
.L218:
	.align	2
.L217:
	.word	.LANCHOR1
	.fnend
	.size	enc_chan_update_chan_prior_tab, .-enc_chan_update_chan_prior_tab
	.align	1
	.global	enc_chan_update_tab_after_enc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_chan_update_tab_after_enc, %function
enc_chan_update_tab_after_enc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, .L228
	movs	r3, #0
	ldr	r1, [r2, #1492]
	str	r3, [r2, #1496]
.L220:
	cmp	r1, r3
	bne	.L223
	cbz	r1, .L225
	movs	r3, #0
	b	.L224
.L223:
	add	r4, r2, r3, lsl #3
	ldr	r4, [r4, #1504]
	cmp	r4, r0
	bne	.L221
.L224:
	add	r4, r2, r3, lsl #3
	subs	r6, r1, #1
.L222:
	cmp	r6, r3
	add	r4, r4, #8
	bhi	.L226
	adds	r1, r1, #187
	str	r0, [r2, r1, lsl #3]
.L225:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	pop	{r4, r5, r6, r7, pc}
.L221:
	adds	r3, r3, #1
	b	.L220
.L226:
	ldr	r7, [r4, #1504]
	adds	r3, r3, #1
	str	r7, [r4, #1496]
	b	.L222
.L229:
	.align	2
.L228:
	.word	.LANCHOR1
	.fnend
	.size	enc_chan_update_tab_after_enc, .-enc_chan_update_tab_after_enc
	.align	1
	.global	enc_chan_get_high_prior_chan
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_chan_get_high_prior_chan, %function
enc_chan_get_high_prior_chan:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L231
	ldr	r2, [r3, #1496]
	add	r0, r2, #188
	adds	r2, r2, #1
	str	r2, [r3, #1496]
	ldr	r0, [r3, r0, lsl #3]
	str	r0, [r3, #1500]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
.L232:
	.align	2
.L231:
	.word	.LANCHOR1
	.fnend
	.size	enc_chan_get_high_prior_chan, .-enc_chan_get_high_prior_chan
	.align	1
	.global	mpp_vcodec_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_init, %function
mpp_vcodec_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r2, #1576
	ldr	r4, .L236
	movs	r1, #0
	mov	r0, r4
	str	r4, [sp, #4]
	bl	memset
	mov	r1, #3264
	ldr	r0, .L236+4
	bl	kstrdup
	movs	r3, #0
	mov	r1, r3
	str	r0, [r4, #4]
	mov	r4, #-1
	ldr	r0, [sp, #4]
	mov	r2, r0
.L234:
	str	r3, [r2, #16]
	adds	r3, r3, #1
	cmp	r3, #8
	str	r4, [r2, #24]
	str	r1, [r2, #56]
	add	r2, r2, #184
	bne	.L234
	str	r1, [r0, #1488]
	bl	vcodec_thread_create
	movs	r1, #1
	mov	r4, r0
	bl	vcodec_thread_set_count
	ldr	r1, .L236+8
	mov	r0, r4
	ldr	r2, [sp, #4]
	bl	vcodec_thread_set_callback
	ldr	r3, .L236+12
	ldr	r0, [r3, #12]
	bl	mpp_packet_pool_init
	mov	r0, r4
	bl	vcodec_thread_start
	movs	r0, #0
	ldr	r3, [sp, #4]
	add	r2, sp, #4
	str	r2, [r3]
	ldr	r3, [sp, #4]
	str	r4, [r3, #1568]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L237:
	.align	2
.L236:
	.word	.LANCHOR1
	.word	.LC11
	.word	mpp_vcodec_enc_routine
	.word	.LANCHOR2
	.fnend
	.size	mpp_vcodec_init, .-mpp_vcodec_init
	.align	1
	.global	mpp_vcodec_unregister_mpidev
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_unregister_mpidev, %function
mpp_vcodec_unregister_mpidev:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	bl	get_mpidev_ops
	cbz	r0, .L240
	ldr	r3, [r0, #4]
	cbz	r3, .L241
	ldr	r5, .L243
	movs	r4, #0
	ldr	r0, [r5, #8]
	cbz	r0, .L238
	blx	r3
	mov	r0, r4
	str	r4, [r5, #8]
.L238:
	pop	{r3, r4, r5, pc}
.L240:
	mov	r0, #-1
	b	.L238
.L241:
	mov	r0, r3
	b	.L238
.L244:
	.align	2
.L243:
	.word	.LANCHOR1
	.fnend
	.size	mpp_vcodec_unregister_mpidev, .-mpp_vcodec_unregister_mpidev
	.align	1
	.global	mpp_vcodec_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_deinit, %function
mpp_vcodec_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L250
	add	r0, r4, #16
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #200
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #384
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #568
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #752
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #936
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #1120
	bl	mpp_chan_clear_buf_resource
	add	r0, r4, #1304
	bl	mpp_chan_clear_buf_resource
	ldr	r0, [r4, #1568]
	cbz	r0, .L246
	bl	vcodec_thread_stop
	ldr	r0, [r4, #1568]
	bl	vcodec_thread_destroy
	movs	r3, #0
	str	r3, [r4, #1568]
.L246:
	bl	mpp_packet_pool_deinit
	movs	r0, #0
	pop	{r4, pc}
.L251:
	.align	2
.L250:
	.word	.LANCHOR1
	.fnend
	.size	mpp_vcodec_deinit, .-mpp_vcodec_deinit
	.global	gdev_fn
	.global	max_stream_cnt
	.global	mpp_vcodec_debug
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.25033, %object
	.size	__FUNCTION__.25033, 28
__FUNCTION__.25033:
	.ascii	"mpp_chan_clear_buf_resource\000"
	.type	__FUNCTION__.25028, %object
	.size	__FUNCTION__.25028, 13
__FUNCTION__.25028:
	.ascii	"get_wrap_buf\000"
	.type	__FUNCTION__.25067, %object
	.size	__FUNCTION__.25067, 31
__FUNCTION__.25067:
	.ascii	"mpp_vcodec_chan_setup_hal_bufs\000"
	.type	__func__.25078, %object
	.size	__func__.25078, 27
__func__.25078:
	.ascii	"mpp_vcodec_chan_entry_init\000"
	.type	__func__.25106, %object
	.size	__func__.25106, 29
__func__.25106:
	.ascii	"mpp_vcodec_chan_entry_deinit\000"
	.type	__param_str_max_stream_cnt, %object
	.size	__param_str_max_stream_cnt, 26
__param_str_max_stream_cnt:
	.ascii	"mpp_vcodec.max_stream_cnt\000"
	.data
	.align	2
	.set	.LANCHOR2,. + 0
	.type	gdev_fn, %object
	.size	gdev_fn, 12
gdev_fn:
	.word	mpp_vcodec_bind
	.word	mpp_vcodec_unbind
	.word	mpp_vcodec_msg_handle
	.type	max_stream_cnt, %object
	.size	max_stream_cnt, 4
max_stream_cnt:
	.word	512
	.bss
	.align	3
	.set	.LANCHOR1,. + 0
	.type	g_vcodec_entry, %object
	.size	g_vcodec_entry, 1576
g_vcodec_entry:
	.space	1576
	.type	__key.25097, %object
	.size	__key.25097, 0
__key.25097:
	.type	__key.25098, %object
	.size	__key.25098, 0
__key.25098:
	.type	__key.25099, %object
	.size	__key.25099, 0
__key.25099:
	.type	__key.25100, %object
	.size	__key.25100, 0
__key.25100:
	.type	mpp_vcodec_debug, %object
	.size	mpp_vcodec_debug, 4
mpp_vcodec_debug:
	.space	4
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_max_stream_cnttype162, %object
	.size	__UNIQUE_ID_max_stream_cnttype162, 39
__UNIQUE_ID_max_stream_cnttype162:
	.ascii	"mpp_vcodec.parmtype=max_stream_cnt:int\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013mpp_vcodec: %d: get_mpidev_ops fail\000"
.LC1:
	.ascii	"\0013mpp_vcodec: %d: MppCtxType error %d\000"
.LC2:
	.ascii	"\0013mpp_vcodec: %d: creat mpi dev & register fail "
	.ascii	"\012\000"
.LC3:
	.ascii	"\0013mpp_vcodec: %d: chan id %d is over, full max i"
	.ascii	"s %d \012\000"
.LC4:
	.ascii	"\0013mpp_vcodec: %d: type %d chnl %d is no create\000"
.LC5:
	.ascii	"\0016mpp_vcodec: %d: attr->max_width = %d, attr->ma"
	.ascii	"x_height = %d\000"
.LC6:
	.ascii	"\0013mpp_vcodec: %s:%d: mpibuf_ops get fail\000"
.LC7:
	.ascii	"&entry->stream_done_lock\000"
.LC8:
	.ascii	"&entry->stream_remove_lock\000"
.LC9:
	.ascii	"&entry->wait\000"
.LC10:
	.ascii	"&entry->stop_wait\000"
.LC11:
	.ascii	"rkv_enc\000"
	.section	___ksymtab+mpp_vcodec_clear_buf_resource,"a"
	.align	2
	.type	__ksymtab_mpp_vcodec_clear_buf_resource, %object
	.size	__ksymtab_mpp_vcodec_clear_buf_resource, 12
__ksymtab_mpp_vcodec_clear_buf_resource:
	.word	mpp_vcodec_clear_buf_resource
	.word	__kstrtab_mpp_vcodec_clear_buf_resource
	.word	__kstrtabns_mpp_vcodec_clear_buf_resource
	.section	__param,"a"
	.align	2
	.type	__param_max_stream_cnt, %object
	.size	__param_max_stream_cnt, 20
__param_max_stream_cnt:
	.word	__param_str_max_stream_cnt
	.word	0
	.word	param_ops_int
	.short	420
	.byte	-1
	.byte	0
	.word	max_stream_cnt
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
