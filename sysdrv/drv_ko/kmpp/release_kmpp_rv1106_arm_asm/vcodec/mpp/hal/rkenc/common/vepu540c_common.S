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
	.file	"vepu540c_common.c"
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
	.type	vepu540c_jpeg_set_uv_offset, %function
vepu540c_jpeg_set_uv_offset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r2
	ldrd	r4, r2, [r1, #8]
	mov	r5, r0
	mov	r7, r3
	cbnz	r2, .L2
	ldr	r2, [r1, #4]
.L2:
	ldr	r0, [r7, #52]
	muls	r4, r2, r4
	bl	mpp_frame_get_fmt
	tst	r0, #15728640
	beq	.L3
	ldr	r0, [r7, #52]
	bl	mpp_frame_get_fbc_offset
	movw	r1, #619
	mov	r4, r0
	mov	r2, r0
	ldr	r0, .L22
	bl	printk
	cbnz	r4, .L13
.L5:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L3:
	cmp	r6, #9
	bhi	.L6
	adr	r3, .L7
	ldr	pc, [r3, r6, lsl #2]
	.p2align 2
.L7:
	.word	.L5+1
	.word	.L5+1
	.word	.L5+1
	.word	.L6+1
	.word	.L14+1
	.word	.L10+1
	.word	.L14+1
	.word	.L21+1
	.word	.L5+1
	.word	.L5+1
	.p2align 1
.L10:
	add	r3, r4, r4, lsl #1
	lsrs	r3, r3, #1
.L9:
	cbz	r4, .L11
.L4:
	ldr	r2, [r5, #36]
	add	r4, r4, r2
	str	r4, [r5, #36]
.L11:
	cmp	r3, #0
	beq	.L5
	ldr	r2, [r5, #40]
	add	r3, r3, r2
	str	r3, [r5, #40]
	b	.L5
.L6:
	mov	r2, r6
	mov	r1, #652
	ldr	r0, .L22+4
	bl	printk
.L21:
	add	r3, r4, r4, lsl #2
	lsrs	r3, r3, #2
	b	.L9
.L14:
	mov	r3, r4
	b	.L9
.L13:
	movs	r3, #0
	b	.L4
.L23:
	.align	2
.L22:
	.word	.LC0
	.word	.LC1
	.fnend
	.size	vepu540c_jpeg_set_uv_offset, .-vepu540c_jpeg_set_uv_offset
	.align	1
	.global	vepu540c_set_roi
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_set_roi, %function
vepu540c_set_roi:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	add	r4, r0, #16
	mov	r5, r0
	.pad #24
	sub	sp, sp, #24
	mov	r6, r1
	mov	r7, r2
	movs	r1, #0
	movs	r2, #128
	mov	r0, r4
	mov	r8, r3
	bl	memset
	cbz	r5, .L25
	cbnz	r6, .L26
.L25:
	ldr	r1, .L51
	mov	r3, r5
	ldr	r0, .L51+4
	movs	r2, #39
	str	r6, [sp]
.L50:
	bl	printk
	b	.L49
.L26:
	ldr	r3, [r6, #4]
	cmp	r3, #8
	bhi	.L28
	ldr	r0, .L51+8
	add	r2, r6, #24
	ldr	ip, .L51+32
	movs	r3, #0
.L29:
	ldr	r1, [r6, #4]
	sub	r5, r2, #16
	cmp	r1, r3
	bgt	.L37
.L27:
	movs	r0, #0
	b	.L24
.L28:
	movs	r2, #44
	ldr	r1, .L51
	ldr	r0, .L51+12
	bl	printk
.L49:
	mov	r0, #-1
.L24:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L37:
	ldrh	lr, [r2, #-16]
	ldrh	r1, [r2, #-12]
	add	r1, r1, lr
	cmp	r1, r7
	bgt	.L38
	ldrh	r1, [r2, #-14]
	ldrh	r9, [r2, #-10]
	add	r1, r1, r9
	cmp	r1, r8
	mov	r1, #-1
	it	le
	movle	r1, #0
.L30:
	ldrh	r9, [r2, #-8]
	ldrb	r10, [r2, #-1]	@ zero_extendqisi2
	cmp	r9, #1
	bhi	.L41
	ldrh	r9, [r2, #-4]
	cmp	r9, #7
	bhi	.L41
	ldrb	r9, [r2, #-2]	@ zero_extendqisi2
	cmp	r9, #1
	bhi	.L41
	cmp	r10, #2
	it	cs
	movcs	r1, #-1
.L31:
	ldrsh	r9, [r2, #-6]
	cmp	r10, #0
	beq	.L32
	cmp	r9, #51
	ble	.L34
.L33:
	movs	r2, #65
	ldr	r1, .L51
	ldr	r0, .L51+16
	bl	printk
	ldrh	r3, [r5]
	ldr	r1, .L51
	ldr	r0, .L51+20
	strd	r7, r8, [sp, #12]
	ldrh	r2, [r5, #6]
	str	r2, [sp, #8]
	ldrh	r2, [r5, #4]
	str	r2, [sp, #4]
	ldrh	r2, [r5, #2]
	str	r2, [sp]
	movs	r2, #68
	bl	printk
	ldrh	r2, [r5, #12]
	ldrh	r3, [r5, #8]
	ldr	r1, .L51
	ldr	r0, .L51+24
	str	r2, [sp]
	movs	r2, #70
	bl	printk
	ldrsh	r2, [r5, #10]
	ldrb	r3, [r5, #15]	@ zero_extendqisi2
	ldr	r1, .L51
	ldr	r0, .L51+28
	str	r2, [sp]
	movs	r2, #72
	b	.L50
.L38:
	mov	r1, #-1
	b	.L30
.L41:
	mov	r1, #-1
	b	.L31
.L32:
	add	r9, r9, #51
	uxth	r9, r9
	cmp	r9, #102
	bhi	.L33
.L34:
	cmp	r1, #0
	bne	.L33
	add	lr, lr, #15
	ldrh	r1, [r4]
	ldrh	r5, [r4, #2]
	asr	lr, lr, #4
	bfi	r1, lr, #0, #10
	strh	r1, [r4]	@ movhi
	ldrh	r1, [r2, #-14]
	adds	r1, r1, #15
	asrs	r1, r1, #4
	bfi	r5, r1, #0, #10
	strh	r5, [r4, #2]	@ movhi
	ldrh	r5, [r2, #-12]
	ldrh	r1, [r2, #-16]
	add	r1, r1, r5
	ldrh	r5, [r4, #4]
	adds	r1, r1, #15
	asrs	r1, r1, #4
	bfi	r5, r1, #0, #10
	strh	r5, [r4, #4]	@ movhi
	ldrh	r5, [r2, #-10]
	ldrh	r1, [r2, #-14]
	add	r1, r1, r5
	ldrh	r5, [r4, #6]
	adds	r1, r1, #15
	asrs	r1, r1, #4
	bfi	r5, r1, #0, #10
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	strh	r5, [r4, #6]	@ movhi
	ldrh	r5, [r2, #-6]
	bfi	r1, r5, #0, #7
	strb	r1, [r4, #8]
	ldrb	r5, [r2, #-1]	@ zero_extendqisi2
	bfi	r1, r5, #7, #1
	strb	r1, [r4, #8]
	ldrb	r1, [r4, #9]	@ zero_extendqisi2
	bic	r1, r1, #63
	orr	r1, r1, #63
	strb	r1, [r4, #9]
	ldrh	r1, [r2, #-8]
	cbz	r1, .L36
	ldr	r1, [r4, #12]
	ands	r1, r1, r0
	orr	r1, r1, ip
	str	r1, [r4, #12]
.L36:
	adds	r4, r4, #16
	adds	r3, r3, #1
	adds	r2, r2, #16
	b	.L29
.L52:
	.align	2
.L51:
	.word	.LANCHOR0
	.word	.LC2
	.word	-15732481
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	1048832
	.fnend
	.size	vepu540c_set_roi, .-vepu540c_set_roi
	.global	__aeabi_uidivmod
	.global	__aeabi_idivmod
	.align	1
	.global	vepu540c_set_qpmap_smart
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_set_qpmap_smart, %function
vepu540c_set_qpmap_smart:
	.fnstart
	@ args = 24, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #28
	sub	sp, sp, #28
	mov	r6, r0
	mov	r10, r2
	mov	r9, r3
	ldr	r8, [sp, #68]
	ldr	r5, [sp, #84]
	mov	r4, r1
	cmp	r1, #0
	beq	.L129
	cmp	r2, #0
	beq	.L129
	ldr	r2, [r3]
	cmp	r2, #0
	beq	.L129
	ldr	r2, [r3, #4]
	cmp	r2, #0
	beq	.L129
	ldr	r2, [r3, #8]
	cmp	r2, #0
	beq	.L129
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq	.L129
	clz	r2, r5
	ldrb	r1, [r0, #12]	@ zero_extendqisi2
	lsrs	r2, r2, #5
	bfi	r1, r2, #0, #1
	strb	r1, [r0, #12]
	ldr	r1, .L154
	ldr	r2, [r0, #12]
	ands	r1, r1, r2
	ldr	r2, .L154+4
	orrs	r2, r2, r1
	str	r2, [r0, #12]
	ldrb	r7, [r3]	@ zero_extendqisi2
	movs	r1, #3
	adds	r0, r7, #1
	sxth	r3, r7
	str	r3, [sp, #16]
	ldr	fp, [r9, r3, lsl #2]
	bl	__aeabi_idivmod
	adds	r0, r7, #2
	ldr	r3, [r9, r1, lsl #2]
	movs	r1, #3
	str	r3, [sp, #4]
	bl	__aeabi_idivmod
	mov	r0, r4
	ldr	r3, [r9, r1, lsl #2]
	ldr	r1, .L154+8
	str	r3, [sp, #8]
	ldr	r3, [sp, #80]
	cmp	r3, #0
	bne	.L55
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L154+8
	str	r0, [sp, #20]
	mov	r0, r10
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r3, [sp, #72]
	str	r0, [sp, #12]
	add	r7, r3, #63
	ldr	r3, [sp, #76]
	bics	r7, r7, #63
	it	mi
	addmi	r7, r7, #15
	add	r4, r3, #15
	asrs	r7, r7, #4
	asrs	r4, r4, #4
	muls	r4, r7, r4
	cbz	r5, .L57
.L153:
	mov	r2, r4
	movs	r1, #0
	ldr	r0, [r9]
	bl	memset
	mov	r2, r4
	movs	r1, #0
	ldr	r0, [r9, #4]
	bl	memset
	mov	r2, r4
	movs	r1, #0
	ldr	r0, [r9, #8]
	bl	memset
.L129:
	mov	r0, #-1
	b	.L53
.L57:
	lsls	r2, r4, #2
	mov	r1, r5
	ldr	r0, [sp, #12]
	mov	r7, r5
	bl	memset
	mov	r2, r4
	mov	r1, r5
	ldr	r3, [sp, #16]
	ldr	r0, [r9, r3, lsl #2]
	mov	r9, r5
	bl	memset
	ldr	r3, .L154+12
	mov	r2, r5
.L58:
	cmp	r2, r4
	blt	.L62
	ldr	r1, .L154+8
	mov	r0, r10
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_begin_cpu_access
	add	r2, r9, r7
	cmp	r2, #0
	bgt	.L63
.L111:
	ldr	r1, .L154+8
	mov	r0, r10
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_end_cpu_access
	movs	r1, #3
	ldr	r3, [sp, #64]
	ldrb	r0, [r3]	@ zero_extendqisi2
	adds	r0, r0, #1
	bl	__aeabi_idivmod
	movs	r0, #0
	ldr	r3, [sp, #64]
	strb	r1, [r3]
.L53:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L62:
	ldr	r1, [sp, #20]
	ldr	r1, [r1, r2, lsl #2]
	uxtb	r0, r1
	ubfx	r1, r1, #8, #4
	adds	r0, r0, #7
	add	r1, r1, r3
	add	r0, r3, r0, lsr #3
	ldrb	r1, [r1, #75]	@ zero_extendqisi2
	ldrb	r0, [r0, #42]	@ zero_extendqisi2
	orrs	r1, r1, r0
	strb	r1, [fp, r2]
	cbz	r1, .L59
	adds	r7, r7, #1
.L60:
	adds	r2, r2, #1
	b	.L58
.L59:
	ldr	r1, [sp, #4]
	ldrb	r1, [r1, r2]	@ zero_extendqisi2
	cmp	r1, #1
	bhi	.L61
	ldr	r1, [sp, #8]
	ldrb	r1, [r1, r2]	@ zero_extendqisi2
	cmp	r1, #1
	bls	.L60
.L61:
	add	r9, r9, #1
	b	.L60
.L63:
	cmp	r8, #35
	bls	.L130
	cmp	r8, #41
	bls	.L131
	cmp	r8, #46
	ite	cc
	movcc	r2, #2
	movcs	r2, #3
.L65:
	movs	r1, #100
	add	r0, r4, r4, lsl #2
	muls	r7, r1, r7
	cmp	r0, r7
	ble	.L66
	cmp	r8, #40
	ldrh	r1, [r6, #12]
	bls	.L67
	mov	ip, #26
.L149:
	bfi	r1, ip, #6, #6
	strh	r1, [r6, #12]	@ movhi
	add	r1, r4, r4, lsr #31
	cmp	r7, r1, asr #1
	bge	.L70
	adds	r2, r2, #5
.L71:
	rsb	r2, r2, #128
.L74:
	cmp	r5, r4
	bge	.L111
	ldrb	r1, [fp, r5]	@ zero_extendqisi2
	cbz	r1, .L75
	ldr	r3, [sp, #12]
	strb	r2, [r3, r5, lsl #2]
.L75:
	adds	r5, r5, #1
	b	.L74
.L130:
	movs	r2, #0
	b	.L65
.L131:
	movs	r2, #1
	b	.L65
.L67:
	cmp	r8, #35
	ite	hi
	movhi	ip, #25
	movls	ip, #23
	b	.L149
.L70:
	cmp	r7, r4
	bge	.L72
	adds	r2, r2, #4
	b	.L71
.L72:
	add	r0, r0, r0, lsr #31
	cmp	r7, r0, asr #1
	ite	lt
	addlt	r2, r2, #3
	addge	r2, r2, #2
	b	.L71
.L66:
	movs	r0, #25
	mul	r9, r1, r9
	mul	r1, r0, r4
	cmp	r1, r9
	ble	.L111
	cmp	r8, #40
	ldrh	r1, [r6, #12]
	bls	.L78
	movs	r0, #26
.L150:
	cmp	r9, r4
	bfi	r1, r0, #6, #6
	strh	r1, [r6, #12]	@ movhi
	bge	.L81
	adds	r2, r2, #6
.L82:
	rsb	r2, r2, #128
.L86:
	cmp	r5, r4
	bge	.L111
	ldrb	r1, [fp, r5]	@ zero_extendqisi2
	cbnz	r1, .L87
	ldr	r3, [sp, #4]
	ldrb	r1, [r3, r5]	@ zero_extendqisi2
	cmp	r1, #1
	bhi	.L88
	ldr	r3, [sp, #8]
	ldrb	r1, [r3, r5]	@ zero_extendqisi2
	cmp	r1, #1
	bls	.L87
.L88:
	ldr	r3, [sp, #12]
	strb	r2, [r3, r5, lsl #2]
.L87:
	adds	r5, r5, #1
	b	.L86
.L78:
	cmp	r8, #35
	it	ls
	movls	r0, #23
	b	.L150
.L81:
	add	r1, r4, r4, lsl #1
	cmp	r1, r9
	ble	.L83
	adds	r2, r2, #5
	b	.L82
.L83:
	rsb	r1, r4, r4, lsl #3
	cmp	r1, r9
	ble	.L84
	adds	r2, r2, #4
	b	.L82
.L84:
	rsb	r1, r4, r4, lsl #4
	cmp	r1, r9
	ite	gt
	addgt	r2, r2, #3
	addle	r2, r2, #2
	b	.L82
.L155:
	.align	2
.L154:
	.word	-524287
	.word	209570
	.word	.LANCHOR0+17
	.word	.LANCHOR0
.L55:
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L156
	str	r0, [sp, #20]
	mov	r0, r10
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r3, [sp, #72]
	str	r0, [sp, #12]
	add	r7, r3, #31
	ldr	r3, [sp, #76]
	bics	r7, r7, #31
	add	r4, r3, #31
	it	mi
	addmi	r7, r7, #15
	bics	r4, r4, #31
	it	mi
	addmi	r4, r4, #15
	asrs	r7, r7, #4
	asrs	r4, r4, #4
	muls	r4, r7, r4
	cmp	r5, #0
	bne	.L153
	bic	r2, r4, #-1073741824
	mov	r1, r5
	ldr	r0, [sp, #12]
	mov	r7, r5
	bl	memset
	mov	r2, r4
	mov	r1, r5
	ldr	r3, [sp, #16]
	ldr	r0, [r9, r3, lsl #2]
	mov	r9, r5
	bl	memset
	ldr	r3, .L156+4
	mov	r2, r5
.L93:
	cmp	r2, r4
	blt	.L97
	ldr	r1, .L156
	mov	r0, r10
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_begin_cpu_access
	add	r2, r9, r7
	cmp	r2, #0
	ble	.L111
	cmp	r8, #35
	bls	.L133
	cmp	r8, #41
	bls	.L134
	cmp	r8, #46
	ite	cc
	movcc	r2, #2
	movcs	r2, #3
.L99:
	movs	r1, #100
	add	r0, r4, r4, lsl #2
	muls	r7, r1, r7
	cmp	r0, r7
	ble	.L100
	cmp	r8, #40
	ldrh	r1, [r6, #12]
	bls	.L101
	mov	ip, #26
.L151:
	bfi	r1, ip, #6, #6
	strh	r1, [r6, #12]	@ movhi
	add	r1, r4, r4, lsr #31
	cmp	r7, r1, asr #1
	bge	.L104
	adds	r2, r2, #5
.L105:
	movs	r1, #0
	rsb	r2, r2, #128
	mov	r0, r1
.L108:
	cmp	r5, r4
	bge	.L111
	ldrb	r6, [fp, r5]	@ zero_extendqisi2
	adds	r5, r5, #1
	adds	r6, r6, #0
	it	ne
	movne	r6, #1
	cmp	r1, r6
	it	cc
	movcc	r1, r6
	ands	r7, r5, #3
	bne	.L136
	adds	r6, r0, #1
	cbz	r1, .L109
	ldr	r3, [sp, #12]
	mov	r1, r7
	strb	r2, [r3, r0, lsl #2]
.L109:
	mov	r0, r6
	b	.L108
.L97:
	ldr	r1, [sp, #20]
	ldr	r1, [r1, r2, lsl #2]
	uxtb	r0, r1
	ubfx	r1, r1, #8, #4
	adds	r0, r0, #7
	add	r1, r1, r3
	add	r0, r3, r0, lsr #3
	ldrb	r1, [r1, #75]	@ zero_extendqisi2
	ldrb	r0, [r0, #42]	@ zero_extendqisi2
	orrs	r1, r1, r0
	strb	r1, [fp, r2]
	cbz	r1, .L94
	adds	r7, r7, #1
.L95:
	adds	r2, r2, #1
	b	.L93
.L94:
	ldr	r1, [sp, #4]
	ldrb	r1, [r1, r2]	@ zero_extendqisi2
	cmp	r1, #1
	bhi	.L96
	ldr	r1, [sp, #8]
	ldrb	r1, [r1, r2]	@ zero_extendqisi2
	cmp	r1, #1
	bls	.L95
.L96:
	add	r9, r9, #1
	b	.L95
.L133:
	movs	r2, #0
	b	.L99
.L134:
	movs	r2, #1
	b	.L99
.L101:
	cmp	r8, #35
	ite	hi
	movhi	ip, #25
	movls	ip, #23
	b	.L151
.L104:
	cmp	r7, r4
	bge	.L106
	adds	r2, r2, #4
	b	.L105
.L106:
	add	r0, r0, r0, lsr #31
	cmp	r7, r0, asr #1
	ite	lt
	addlt	r2, r2, #3
	addge	r2, r2, #2
	b	.L105
.L136:
	mov	r6, r0
	b	.L109
.L100:
	movs	r0, #25
	mul	r9, r1, r9
	mul	r1, r0, r4
	cmp	r1, r9
	ble	.L111
	cmp	r8, #40
	ldrh	r1, [r6, #12]
	bls	.L112
	movs	r0, #26
.L152:
	cmp	r9, r4
	bfi	r1, r0, #6, #6
	strh	r1, [r6, #12]	@ movhi
	bge	.L115
	adds	r2, r2, #6
.L116:
	movs	r1, #0
	rsb	r2, r2, #128
	mov	r6, r1
.L120:
	cmp	r5, r4
	bge	.L111
	ldrb	r0, [fp, r5]	@ zero_extendqisi2
	cbnz	r0, .L137
	ldr	r3, [sp, #4]
	ldrb	r0, [r3, r5]	@ zero_extendqisi2
	cmp	r0, #1
	bhi	.L138
	ldr	r3, [sp, #8]
	ldrb	r0, [r3, r5]	@ zero_extendqisi2
	cmp	r0, #1
	ite	ls
	movls	r0, #0
	movhi	r0, #1
.L121:
	cmp	r1, r0
	add	r5, r5, #1
	it	cc
	movcc	r1, r0
	ands	r7, r5, #3
	bne	.L139
	adds	r0, r6, #1
	cbz	r1, .L122
	ldr	r3, [sp, #12]
	mov	r1, r7
	strb	r2, [r3, r6, lsl #2]
.L122:
	mov	r6, r0
	b	.L120
.L112:
	cmp	r8, #35
	it	ls
	movls	r0, #23
	b	.L152
.L115:
	add	r1, r4, r4, lsl #1
	cmp	r1, r9
	ble	.L117
	adds	r2, r2, #5
	b	.L116
.L117:
	rsb	r1, r4, r4, lsl #3
	cmp	r1, r9
	ble	.L118
	adds	r2, r2, #4
	b	.L116
.L118:
	rsb	r1, r4, r4, lsl #4
	cmp	r1, r9
	ite	gt
	addgt	r2, r2, #3
	addle	r2, r2, #2
	b	.L116
.L137:
	movs	r0, #0
	b	.L121
.L138:
	movs	r0, #1
	b	.L121
.L139:
	mov	r0, r6
	b	.L122
.L157:
	.align	2
.L156:
	.word	.LANCHOR0+17
	.word	.LANCHOR0
	.fnend
	.size	vepu540c_set_qpmap_smart, .-vepu540c_set_qpmap_smart
	.align	1
	.global	vepu540c_set_qpmap_normal
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_set_qpmap_normal, %function
vepu540c_set_qpmap_normal:
	.fnstart
	@ args = 24, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	mov	r6, r0
	mov	r8, r2
	mov	r5, r3
	ldr	r7, [sp, #76]
	mov	r4, r1
	cmp	r1, #0
	beq	.L208
	cmp	r2, #0
	beq	.L208
	ldr	r2, [r3]
	cmp	r2, #0
	beq	.L208
	ldr	r2, [r3, #4]
	cmp	r2, #0
	beq	.L208
	ldr	r2, [r3, #8]
	cmp	r2, #0
	beq	.L208
	ldr	r3, [sp, #56]
	cmp	r3, #0
	beq	.L208
	clz	r2, r7
	ldrb	r1, [r0, #12]	@ zero_extendqisi2
	lsrs	r2, r2, #5
	bfi	r1, r2, #0, #1
	strb	r1, [r0, #12]
	ldr	r1, .L222
	ldr	r2, [r0, #12]
	ands	r1, r1, r2
	ldr	r2, .L222+4
	orrs	r2, r2, r1
	str	r2, [r0, #12]
	ldrb	r10, [r3]	@ zero_extendqisi2
	movs	r1, #3
	add	r0, r10, #1
	sxth	r9, r10
	ldr	r3, [r5, r9, lsl #2]
	str	r3, [sp, #4]
	bl	__aeabi_idivmod
	add	r0, r10, #2
	ldr	r3, [r5, r1, lsl #2]
	movs	r1, #3
	str	r3, [sp, #8]
	bl	__aeabi_idivmod
	mov	r0, r4
	ldr	r3, [r5, r1, lsl #2]
	ldr	r1, .L222+8
	str	r3, [sp, #12]
	ldr	r3, [sp, #72]
	cmp	r3, #0
	bne	.L160
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L222+8
	mov	r10, r0
	mov	r0, r8
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r3, [sp, #64]
	mov	r4, r0
	add	fp, r3, #63
	ldr	r3, [sp, #68]
	bics	fp, fp, #63
	it	mi
	addmi	fp, fp, #15
	add	r2, r3, #15
	asr	fp, fp, #4
	asrs	r2, r2, #4
	mul	fp, r2, fp
	cbz	r7, .L162
	mov	r2, fp
	movs	r1, #0
	ldr	r0, [r5]
	bl	memset
	mov	r2, fp
	movs	r1, #0
	ldr	r0, [r5, #4]
	bl	memset
	mov	r2, fp
.L221:
	movs	r1, #0
	ldr	r0, [r5, #8]
	bl	memset
.L208:
	mov	r0, #-1
	b	.L158
.L162:
	lsl	r2, fp, #2
	mov	r1, r7
	bl	memset
	ldr	r0, [r5, r9, lsl #2]
	ldr	r5, .L222+12
	mov	r2, fp
	mov	r1, r7
	bl	memset
	mov	ip, r7
	mov	r2, r7
.L163:
	cmp	r2, fp
	blt	.L166
	ldr	r3, [sp, #60]
	cmp	r3, #35
	bls	.L209
	cmp	r3, #46
	ite	cc
	movcc	r5, #4
	movcs	r5, #3
.L167:
	mov	r9, #100
	rsb	r10, fp, fp, lsl #4
	mul	r9, r9, ip
	cmp	r10, r9
	ble	.L168
	cmp	r9, fp, asr #5
	ble	.L168
	ldr	r3, [sp, #60]
	ldrh	r2, [r6, #12]
	cmp	r3, #40
	bls	.L169
	movs	r1, #28
.L219:
	bfi	r2, r1, #6, #6
	strh	r2, [r6, #12]	@ movhi
.L168:
	ldr	r1, .L222+8
	mov	r0, r8
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_begin_cpu_access
	cmp	r10, r9
	bgt	.L171
.L194:
	ldr	r1, .L222+8
	mov	r0, r8
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_end_cpu_access
	movs	r1, #3
	ldr	r3, [sp, #56]
	ldrb	r0, [r3]	@ zero_extendqisi2
	adds	r0, r0, #1
	bl	__aeabi_idivmod
	movs	r0, #0
	ldr	r3, [sp, #56]
	strb	r1, [r3]
.L158:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L166:
	ldr	r1, [r10, r2, lsl #2]
	ldr	r3, [sp, #4]
	uxtb	r0, r1
	ubfx	r1, r1, #8, #4
	adds	r0, r0, #7
	add	r1, r1, r5
	add	r0, r5, r0, lsr #3
	ldrb	r1, [r1, #75]	@ zero_extendqisi2
	ldrb	r0, [r0, #42]	@ zero_extendqisi2
	orrs	r1, r1, r0
	strb	r1, [r3, r2]
	cbnz	r1, .L164
	ldr	r3, [sp, #8]
	ldrb	r1, [r3, r2]	@ zero_extendqisi2
	cmp	r1, #1
	bhi	.L165
	ldr	r3, [sp, #12]
	ldrb	r1, [r3, r2]	@ zero_extendqisi2
	cmp	r1, #1
	bls	.L164
.L165:
	add	ip, ip, #1
.L164:
	adds	r2, r2, #1
	b	.L163
.L209:
	movs	r5, #0
	b	.L167
.L169:
	ldr	r3, [sp, #60]
	cmp	r3, #35
	ite	hi
	movhi	r1, #27
	movls	r1, #25
	b	.L219
.L171:
	cmp	r9, #0
	ble	.L194
	cmp	r9, fp
	bge	.L174
	adds	r5, r5, #7
.L175:
	rsb	r5, r5, #128
.L178:
	cmp	r7, fp
	bge	.L194
	ldr	r3, [sp, #4]
	ldrb	r2, [r3, r7]	@ zero_extendqisi2
	cbnz	r2, .L179
	ldr	r3, [sp, #8]
	ldrb	r2, [r3, r7]	@ zero_extendqisi2
	cmp	r2, #1
	bhi	.L180
	ldr	r3, [sp, #12]
	ldrb	r2, [r3, r7]	@ zero_extendqisi2
	cmp	r2, #1
	bls	.L179
.L180:
	strb	r5, [r4, r7, lsl #2]
.L179:
	adds	r7, r7, #1
	b	.L178
.L174:
	add	r2, fp, fp, lsl #1
	cmp	r2, r9
	ble	.L176
	adds	r5, r5, #6
	b	.L175
.L176:
	rsb	r2, fp, fp, lsl #3
	cmp	r2, r9
	ite	gt
	addgt	r5, r5, #5
	addle	r5, r5, #4
	b	.L175
.L160:
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L222+8
	mov	fp, r0
	mov	r0, r8
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r3, [sp, #64]
	mov	r10, r0
	add	r4, r3, #31
	ldr	r3, [sp, #68]
	bics	r4, r4, #31
	add	r2, r3, #31
	it	mi
	addmi	r4, r4, #15
	bics	r2, r2, #31
	it	mi
	addmi	r2, r2, #15
	asrs	r4, r4, #4
	asrs	r2, r2, #4
	muls	r4, r2, r4
	cbz	r7, .L184
	mov	r2, r4
	movs	r1, #0
	ldr	r0, [r5]
	bl	memset
	mov	r2, r4
	movs	r1, #0
	ldr	r0, [r5, #4]
	bl	memset
	mov	r2, r4
	b	.L221
.L184:
	bic	r2, r4, #-1073741824
	mov	r1, r7
	bl	memset
	ldr	r0, [r5, r9, lsl #2]
	ldr	r5, .L222+12
	mov	r2, r4
	mov	r1, r7
	bl	memset
	mov	ip, r7
	mov	r2, r7
.L185:
	cmp	r2, r4
	blt	.L188
	ldr	r3, [sp, #60]
	cmp	r3, #35
	bls	.L211
	cmp	r3, #46
	ite	cc
	movcc	r5, #4
	movcs	r5, #3
.L189:
	mov	r9, #100
	rsb	fp, r4, r4, lsl #4
	mul	r9, r9, ip
	cmp	fp, r9
	ble	.L190
	cmp	r9, r4, asr #5
	ble	.L190
	ldr	r3, [sp, #60]
	ldrh	r2, [r6, #12]
	cmp	r3, #40
	bls	.L191
	movs	r1, #28
.L220:
	bfi	r2, r1, #6, #6
	strh	r2, [r6, #12]	@ movhi
.L190:
	ldr	r1, .L222+8
	mov	r0, r8
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_begin_cpu_access
	cmp	fp, r9
	ble	.L194
	cmp	r9, #0
	ble	.L194
	cmp	r9, r4
	bge	.L195
	adds	r5, r5, #7
.L196:
	movs	r2, #0
	rsb	r5, r5, #128
	mov	r0, r2
.L199:
	cmp	r7, r4
	bge	.L194
	ldr	r3, [sp, #4]
	ldrb	r1, [r3, r7]	@ zero_extendqisi2
	cmp	r1, #0
	bne	.L213
	ldr	r3, [sp, #8]
	ldrb	r1, [r3, r7]	@ zero_extendqisi2
	cmp	r1, #1
	bhi	.L214
	ldr	r3, [sp, #12]
	ldrb	r1, [r3, r7]	@ zero_extendqisi2
	cmp	r1, #1
	ite	ls
	movls	r1, #0
	movhi	r1, #1
.L200:
	cmp	r2, r1
	add	r7, r7, #1
	it	cc
	movcc	r2, r1
	ands	r6, r7, #3
	bne	.L215
	adds	r1, r0, #1
	cbz	r2, .L201
	mov	r2, r6
	strb	r5, [r10, r0, lsl #2]
.L201:
	mov	r0, r1
	b	.L199
.L223:
	.align	2
.L222:
	.word	-524287
	.word	209570
	.word	.LANCHOR0+91
	.word	.LANCHOR0
.L188:
	ldr	r1, [fp, r2, lsl #2]
	ldr	r3, [sp, #4]
	uxtb	r0, r1
	ubfx	r1, r1, #8, #4
	adds	r0, r0, #7
	add	r1, r1, r5
	add	r0, r5, r0, lsr #3
	ldrb	r1, [r1, #75]	@ zero_extendqisi2
	ldrb	r0, [r0, #42]	@ zero_extendqisi2
	orrs	r1, r1, r0
	strb	r1, [r3, r2]
	cbnz	r1, .L186
	ldr	r3, [sp, #8]
	ldrb	r1, [r3, r2]	@ zero_extendqisi2
	cmp	r1, #1
	bhi	.L187
	ldr	r3, [sp, #12]
	ldrb	r1, [r3, r2]	@ zero_extendqisi2
	cmp	r1, #1
	bls	.L186
.L187:
	add	ip, ip, #1
.L186:
	adds	r2, r2, #1
	b	.L185
.L211:
	movs	r5, #0
	b	.L189
.L191:
	ldr	r3, [sp, #60]
	cmp	r3, #35
	ite	hi
	movhi	r1, #27
	movls	r1, #25
	b	.L220
.L195:
	add	r2, r4, r4, lsl #1
	cmp	r2, r9
	ble	.L197
	adds	r5, r5, #6
	b	.L196
.L197:
	rsb	r2, r4, r4, lsl #3
	cmp	r2, r9
	ite	gt
	addgt	r5, r5, #5
	addle	r5, r5, #4
	b	.L196
.L213:
	movs	r1, #0
	b	.L200
.L214:
	movs	r1, #1
	b	.L200
.L215:
	mov	r1, r0
	b	.L201
	.fnend
	.size	vepu540c_set_qpmap_normal, .-vepu540c_set_qpmap_normal
	.align	1
	.global	vepu540c_set_osd
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_set_osd, %function
vepu540c_set_osd:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r8, r0
	ldr	r4, [r0, #8]
	.pad #40
	sub	sp, sp, #40
	cmp	r4, #0
	beq	.L240
	ldr	r6, [r4, #4]
	cmp	r6, #0
	beq	.L240
	cmp	r6, #8
	bls	.L226
	mov	r3, r6
	mov	r2, #532
	ldr	r1, .L241
	ldr	r0, .L241+4
	bl	printk
	ldr	r3, [r4, #4]
	cmp	r3, #8
	bls	.L232
	movw	r1, #533
	ldr	r3, .L241+8
	str	r1, [sp]
	ldr	r2, .L241+12
	ldr	r0, .L241+16
	bl	printk
.L232:
	mov	r0, #-1
	b	.L224
.L226:
	ldr	r5, [r0]
	adds	r4, r4, #148
	mov	r9, #0
	mov	r10, #36
	add	r7, r5, #24
.L229:
	ldr	r1, [r4, #-128]
	add	r0, sp, #12
	bl	vepu541_set_fmt
	mul	r1, r10, r9
	ldr	r3, [r4, #-140]
	adds	r2, r5, r1
	ldrb	r0, [r5, r1]	@ zero_extendqisi2
	bfi	r0, r3, #0, #1
	strb	r0, [r5, r1]
	ldr	r3, [r4, #-136]
	bfi	r0, r3, #6, #1
	strb	r0, [r5, r1]
	ldr	r3, [r4, #-132]
	bfi	r0, r3, #7, #1
	strb	r0, [r5, r1]
	ldrb	r0, [r2, #1]	@ zero_extendqisi2
	ldr	lr, [r2, #20]
	ldr	r3, [sp, #12]
	bfi	r0, r3, #0, #4
	strb	r0, [r2, #1]
	ldr	r3, [r4, #-124]
	bfi	r0, r3, #5, #1
	strb	r0, [r2, #1]
	ldrh	r0, [r2, #4]
	ldr	r3, [r4, #-120]
	bfi	r0, r3, #0, #14
	strh	r0, [r2, #4]	@ movhi
	ldr	r3, [r4, #-116]
	ldr	r0, [r2, #4]
	bfi	r0, r3, #14, #14
	str	r0, [r2, #4]
	ldrh	r0, [r2, #8]
	ldr	r3, [r4, #-112]
	bfi	r0, r3, #0, #14
	strh	r0, [r2, #8]	@ movhi
	ldr	r3, [r4, #-108]
	ldr	r0, [r2, #8]
	bfi	r0, r3, #14, #14
	str	r0, [r2, #8]
	ldrb	r0, [r2, #7]	@ zero_extendqisi2
	ldr	r3, [r4, #-96]
	bfi	r0, r3, #4, #4
	strb	r0, [r2, #7]
	ldr	r0, [r4, #-104]
	bfi	lr, r0, #0, #17
	str	lr, [r2, #20]
	ldr	r0, [r4, #-100]
	ubfx	lr, lr, #24, #8
	bfi	lr, r0, #1, #1
	strb	lr, [r2, #23]
	ldrb	lr, [r5, r1]	@ zero_extendqisi2
	ldr	r0, [r4, #-92]
	bfi	lr, r0, #1, #1
	strb	lr, [r5, r1]
	ldr	r0, [r4, #-88]
	bfi	lr, r0, #2, #1
	strb	lr, [r5, r1]
	ldr	r0, [r4, #-84]
	bfi	lr, r0, #3, #1
	strb	lr, [r5, r1]
	ldr	r0, [r4, #-80]
	bfi	lr, r0, #4, #1
	strb	lr, [r5, r1]
	ldr	r0, [r4, #-76]
	ldrb	lr, [r2, #11]	@ zero_extendqisi2
	ldrh	ip, [r2, #22]
	bfi	lr, r0, #4, #1
	strb	lr, [r2, #11]
	ldr	r0, [r4, #-72]
	bfi	lr, r0, #5, #2
	strb	lr, [r2, #11]
	ldr	r0, [r4, #-68]
	bfi	ip, r0, #1, #8
	ldrb	r0, [r2, #1]	@ zero_extendqisi2
	strh	ip, [r2, #22]	@ movhi
	ldr	r3, [r4, #-56]
	bfi	r0, r3, #4, #1
	strb	r0, [r2, #1]
	ldr	r3, [r4, #-52]
	ldr	r0, [r5, r1]
	bfi	r0, r3, #14, #8
	str	r0, [r5, r1]
	ldr	r3, [r4, #-48]
	ldrh	r0, [r2, #2]
	bfi	r0, r3, #6, #8
	strh	r0, [r2, #2]	@ movhi
	ldr	r3, [r4, #-44]
	ubfx	r0, r0, #8, #8
	bfi	r0, r3, #6, #2
	strb	r0, [r2, #3]
	ldr	r3, [r4, #-40]
	ldrb	r0, [r5, r1]	@ zero_extendqisi2
	bfi	r0, r3, #5, #1
	strb	r0, [r5, r1]
	ldrb	r3, [r2, #32]	@ zero_extendqisi2
	add	r0, r2, #32
	ldr	r1, [r4, #-36]
	bfi	r3, r1, #0, #1
	strb	r3, [r2, #32]
	ldr	r1, [r4, #-32]
	bfi	r3, r1, #1, #7
	ldrb	r1, [r2, #33]	@ zero_extendqisi2
	strb	r3, [r2, #32]
	ldr	r3, [r4, #-28]
	bfi	r1, r3, #0, #6
	strb	r1, [r2, #33]
	ldr	r3, [r2, #32]
	ldr	r1, [r4, #-24]
	bfi	r3, r1, #14, #6
	str	r3, [r2, #32]
	ldr	r2, [r4, #-20]
	ubfx	r3, r3, #16, #16
	bfi	r3, r2, #4, #5
	strh	r3, [r0, #2]	@ movhi
	ldr	r1, [r4, #-60]
	cbz	r1, .L227
	movs	r2, #0
	ldr	r0, [r8, #4]
	bl	mpp_dev_get_mpi_ioaddress
	str	r0, [r7, #-8]
.L227:
	ldr	r1, [r4, #-12]
	cbz	r1, .L228
	movs	r2, #0
	ldr	r0, [r8, #4]
	bl	mpp_dev_get_mpi_ioaddress
	str	r0, [r7, #-12]
.L228:
	mov	r2, r4
	add	r9, r9, #1
	ldr	r3, [r2, #-8]!	@ unaligned
	cmp	r6, r9
	add	r4, r4, #140
	add	r7, r7, #36
	str	r3, [r7, #-36]	@ unaligned
	ldr	r3, [r2, #4]	@ unaligned
	str	r3, [r7, #-32]	@ unaligned
	bne	.L229
	ldr	r3, [r5, #288]
	and	r2, r3, #-134217728
	ldr	r3, .L241+20
	orrs	r3, r3, r2
	str	r3, [r5, #288]
	ldr	r3, [r5, #292]
	and	r2, r3, #-134217728
	ldr	r3, .L241+24
	orrs	r3, r3, r2
	str	r3, [r5, #292]
	ldr	r3, [r5, #296]
	and	r2, r3, #-134217728
	ldr	r3, .L241+28
	orrs	r3, r3, r2
	str	r3, [r5, #296]
	ldr	r3, [r5, #300]
	and	r3, r3, #-16777216
	orr	r3, r3, #8388608
	orr	r3, r3, #32768
	str	r3, [r5, #300]
	mov	r3, #-16711936
	str	r3, [r5, #304]
	mov	r3, #65280
	strh	r3, [r5, #308]	@ movhi
.L240:
	movs	r0, #0
.L224:
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L242:
	.align	2
.L241:
	.word	.LANCHOR0+117
	.word	.LC8
	.word	.LANCHOR0+134
	.word	.LC9
	.word	.LC10
	.word	7679053
	.word	33773525
	.word	128920192
	.fnend
	.size	vepu540c_set_osd, .-vepu540c_set_osd
	.align	1
	.global	vepu540c_set_jpeg_reg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_set_jpeg_reg, %function
vepu540c_set_jpeg_reg:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r7, r0
	ldr	r5, [r0, #12]
	.pad #44
	sub	sp, sp, #44
	ldrd	r8, r3, [r0, #16]
	ldr	r4, [r0, #4]
	ldr	r6, [r5, #16]
	cbnz	r3, .L244
	mov	r2, #264
	ldr	r1, [r5, #56]
	ldr	r0, [r0]
	bl	mpp_dev_get_iova_address
	mov	r3, r5
	mov	r1, r6
	strd	r0, r0, [r4, #32]
	str	r0, [r4, #40]
	mov	r0, r4
	ldr	r2, [r8]
	bl	vepu540c_jpeg_set_uv_offset
.L244:
	ldr	r3, [r5, #32]
	ldr	r2, [r3, #28]
	ldr	r0, [r3]
	cmp	r2, #0
	bne	.L245
	cmp	r0, #0
	beq	.L246
	mov	r1, r0
	movw	r2, #257
	ldr	r0, [r7]
	bl	mpp_dev_get_iova_address
	ldr	r3, [r5, #32]
	ldr	r3, [r3, #12]
	add	r0, r0, r3
	str	r0, [r4, #4]
.L247:
	ldr	r3, [r5, #32]
	ldr	r9, [r4, #4]
	ldr	r3, [r3, #24]
	add	r2, r9, #-1
	str	r9, [r4, #8]
	add	r3, r3, r2
	str	r3, [r4]
	ldr	r0, [r5, #28]
	bl	mpp_packet_get_length
	add	r0, r0, r9
	str	r0, [r4, #12]
.L248:
	ldr	r3, [r6]
	ldr	r2, .L267
	adds	r3, r3, #15
	ldrh	r1, [r4, #64]
	and	r3, r2, r3, lsr #3
	subs	r3, r3, #1
	bfi	r1, r3, #0, #11
	strh	r1, [r4, #64]	@ movhi
	ldrb	r1, [r6]	@ zero_extendqisi2
	add	r3, r1, #15
	bic	r3, r3, #15
	subs	r3, r3, r1
	ldrb	r1, [r4, #68]	@ zero_extendqisi2
	bfi	r1, r3, #0, #6
	strb	r1, [r4, #68]
	ldr	r3, [r6, #4]
	adds	r3, r3, #15
	and	r2, r2, r3, lsr #3
	ldrh	r3, [r4, #66]
	subs	r2, r2, #1
	bfi	r3, r2, #0, #11
	strh	r3, [r4, #66]	@ movhi
	ldrb	r2, [r6, #4]	@ zero_extendqisi2
	add	r3, r2, #15
	bic	r3, r3, #15
	subs	r3, r3, r2
	ldrb	r2, [r4, #70]	@ zero_extendqisi2
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #70]
	ldr	r1, [r8]
	ldrb	r3, [r4, #72]	@ zero_extendqisi2
	and	r1, r1, #15
	bfi	r3, r1, #2, #4
	strb	r3, [r4, #72]
	ldr	r2, [r8, #4]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #72]
	ldr	r2, [r8, #8]
	bfi	r3, r2, #1, #1
	strb	r3, [r4, #72]
	ldrh	r3, [r4, #72]
	bic	r3, r3, #1920
	orr	r3, r3, #128
	strh	r3, [r4, #72]	@ movhi
	ldr	r3, [r6, #36]
	cmp	r3, #0
	ldrb	r3, [r4, #95]	@ zero_extendqisi2
	ite	le
	movle	r2, #0
	movgt	r2, #1
	bfi	r3, r2, #2, #1
	strb	r3, [r4, #95]
	ldr	r2, [r6, #32]
	bfi	r3, r2, #3, #2
	strb	r3, [r4, #95]
	ldr	r3, [r6, #8]
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	cbnz	r3, .L250
	ands	r0, r2, #60
	bne	.L251
	ldr	r3, [r6]
	lsls	r3, r3, #2
.L250:
	and	r2, r2, #52
	cmp	r2, #16
	itet	ne
	addne	r0, r3, r3, lsr #31
	moveq	r0, r3
	asrne	r0, r0, #1
	cmp	r1, #2
	bhi	.L255
	ldr	r2, [r4, #76]
	and	r1, r2, #-134217728
	ldr	r2, .L267+4
	orrs	r2, r2, r1
	str	r2, [r4, #76]
	ldr	r2, [r4, #80]
	and	r1, r2, #-134217728
	ldr	r2, .L267+8
	orrs	r2, r2, r1
	str	r2, [r4, #80]
	ldr	r2, [r4, #84]
	and	r1, r2, #-134217728
	ldr	r2, .L267+12
	orrs	r2, r2, r1
	str	r2, [r4, #84]
	ldr	r2, [r4, #88]
	lsrs	r2, r2, #21
	lsls	r2, r2, #21
	orr	r2, r2, #32768
	orr	r2, r2, #128
	str	r2, [r4, #88]
.L255:
	ldr	r2, [r4, #100]
	strh	r0, [r4, #104]	@ movhi
	bfi	r2, r3, #0, #17
	str	r2, [r4, #100]
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_offset_y
	ldrh	r3, [r4, #98]
	bfi	r3, r0, #0, #14
	strh	r3, [r4, #98]	@ movhi
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_offset_x
	ldrh	r3, [r4, #96]
	bfi	r3, r0, #0, #14
	strh	r3, [r4, #96]	@ movhi
	ldrh	r3, [r4, #108]
	bic	r3, r3, #816
	bic	r3, r3, #3
	strh	r3, [r4, #108]	@ movhi
	ldrh	r3, [r4, #112]
	bfc	r3, #0, #15
	strh	r3, [r4, #112]	@ movhi
	ldrh	r3, [r4, #116]
	bfc	r3, #0, #15
	strh	r3, [r4, #116]	@ movhi
	ldrh	r3, [r4, #120]
	bfc	r3, #0, #15
	strh	r3, [r4, #120]	@ movhi
	mov	r3, #-1610612736
	str	r3, [r4, #124]
	ldr	r3, [r4, #128]
	and	r3, r3, #-1073741824
	str	r3, [r4, #128]
	ldr	r0, [r7, #20]
	cbz	r0, .L243
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_is_full
	cmp	r0, #0
	bne	.L257
	add	r2, sp, #4
	movs	r1, #19
	bl	rk_dvbm_ctrl
	ldr	r3, [sp, #36]
	cmp	r3, #0
	beq	.L258
	ldrd	r2, r3, [sp, #28]
	movw	r1, #790
	ldr	r0, .L267+16
	bl	printk
	mov	r0, #-1
.L243:
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L246:
	ldr	r2, [r3, #4]
	ldr	r3, [r3, #12]
	add	r3, r3, r2
	str	r3, [r4, #4]
	b	.L247
.L245:
	ldr	r1, .L267+20
	bl	mpp_buffer_get_size_with_caller
	ldr	r3, [r5, #32]
	movw	r2, #257
	ldr	r1, [r3]
	mov	r10, r0
	ldr	r0, [r7]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #4]
	mov	r9, r0
	ldr	r3, [r5, #32]
	ldr	r0, [r5, #28]
	ldr	fp, [r3, #12]
	bl	mpp_packet_get_length
	mov	r1, r10
	add	r0, r0, fp
	bl	__aeabi_uidivmod
	ldr	r0, [r4, #4]
	add	r1, r1, r9
	str	r1, [r4, #12]
	ldr	r3, [r5, #32]
	ldr	r3, [r3, #16]
	add	r3, r3, r0
	add	r0, r0, r10
	str	r3, [r4, #8]
	str	r0, [r4]
	b	.L248
.L251:
	cmp	r0, #4
	bne	.L252
	ldr	r3, [r6]
	add	r3, r3, r3, lsl #1
	b	.L250
.L252:
	cmp	r0, #8
	beq	.L253
	and	r0, r2, #56
	cmp	r0, #32
	bne	.L250
.L253:
	ldr	r3, [r6]
	lsls	r3, r3, #1
	b	.L250
.L258:
	ldr	r3, [sp, #8]
	str	r3, [r4, #16]
	ldr	r3, [sp, #20]
	str	r3, [r4, #20]
	ldr	r3, [sp, #4]
	str	r3, [r4, #24]
	ldr	r3, [sp, #16]
	str	r3, [r4, #28]
	ldr	r3, [sp, #12]
	str	r3, [r4, #32]
	ldr	r3, [sp, #24]
	strd	r3, r3, [r4, #36]
.L259:
	movs	r0, #0
	b	.L243
.L257:
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_phy_addr
	cbz	r0, .L260
	strd	r0, r0, [r4, #32]
	mov	r3, r5
	str	r0, [r4, #40]
	mov	r1, r6
	ldr	r2, [r8]
	mov	r0, r4
	bl	vepu540c_jpeg_set_uv_offset
	b	.L259
.L260:
	movw	r1, #809
	ldr	r0, .L267+24
	bl	printk
	b	.L259
.L268:
	.align	2
.L267:
	.word	536870910
	.word	20261917
	.word	123164288
	.word	33762283
	.word	.LC11
	.word	.LANCHOR0+151
	.word	.LC12
	.fnend
	.size	vepu540c_set_jpeg_reg, .-vepu540c_set_jpeg_reg
	.align	1
	.global	vepu540c_set_dvbm
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_set_dvbm, %function
vepu540c_set_dvbm:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	.pad #40
	sub	sp, sp, #40
	mov	r4, r0
	add	r2, sp, #4
	movs	r1, #19
	movs	r0, #0
	bl	rk_dvbm_ctrl
	ldr	r3, [sp, #4]
	str	r3, [r4]
	ldr	r3, [sp, #16]
	str	r3, [r4, #4]
	ldr	r3, [sp, #8]
	str	r3, [r4, #8]
	ldr	r3, [sp, #20]
	str	r3, [r4, #12]
	add	sp, sp, #40
	@ sp needed
	pop	{r4, pc}
	.fnend
	.size	vepu540c_set_dvbm, .-vepu540c_set_dvbm
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.35362, %object
	.size	__func__.35362, 17
__func__.35362:
	.ascii	"vepu540c_set_roi\000"
	.type	__FUNCTION__.35403, %object
	.size	__FUNCTION__.35403, 25
__FUNCTION__.35403:
	.ascii	"vepu540c_set_qpmap_smart\000"
	.type	md_idx0, %object
	.size	md_idx0, 33
md_idx0:
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.type	md_idx1, %object
	.size	md_idx1, 16
md_idx1:
	.byte	0
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.type	__FUNCTION__.35462, %object
	.size	__FUNCTION__.35462, 26
__FUNCTION__.35462:
	.ascii	"vepu540c_set_qpmap_normal\000"
	.type	__func__.35488, %object
	.size	__func__.35488, 17
__func__.35488:
	.ascii	"vepu540c_set_osd\000"
	.type	__FUNCTION__.35489, %object
	.size	__FUNCTION__.35489, 17
__FUNCTION__.35489:
	.ascii	"vepu540c_set_osd\000"
	.type	__FUNCTION__.35531, %object
	.size	__FUNCTION__.35531, 22
__FUNCTION__.35531:
	.ascii	"vepu540c_set_jpeg_reg\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%d: fbc case u_offset = %d\000"
.LC1:
	.ascii	"\0013%d: unknown color space: %d\012\000"
.LC2:
	.ascii	"\0013%s:%d: invalid buf %p roi %p\012\000"
.LC3:
	.ascii	"\0013%s:%d: invalid region number %d\012\000"
.LC4:
	.ascii	"\0013%s:%d: region %d invalid param:\012\000"
.LC5:
	.ascii	"\0013%s:%d: position [%d:%d:%d:%d] vs [%d:%d]\012\000"
.LC6:
	.ascii	"\0013%s:%d: force intra %d qp area index %d\012\000"
.LC7:
	.ascii	"\0013%s:%d: abs qp mode %d value %d\012\000"
.LC8:
	.ascii	"\0013%s:%d: do NOT support more than 8 regions inva"
	.ascii	"lid num %d\012\000"
.LC9:
	.ascii	"osd->num_region <= 8\000"
.LC10:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC11:
	.ascii	"\0013%d: cur frame already overflow [%d %d]!\012\000"
.LC12:
	.ascii	"\0013%d: online case set full frame err\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
