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
	.type	vepu540c_jpeg_set_uv_offset.constprop.0, %function
vepu540c_jpeg_set_uv_offset.constprop.0:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r0
	ldr	r0, [r2, #52]
	mov	r7, r1
	mov	r5, r2
	bl	mpp_frame_get_hor_stride
	cbz	r0, .L2
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_hor_stride
.L24:
	mov	r4, r0
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_ver_stride
	cbz	r0, .L4
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_ver_stride
.L5:
	muls	r4, r0, r4
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_fmt
	tst	r0, #15728640
	beq	.L6
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_fbc_offset
	movw	r1, #594
	mov	r4, r0
	mov	r2, r0
	ldr	r0, .L26
	bl	printk
	cmp	r4, #0
	bne	.L16
.L8:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L2:
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_width
	b	.L24
.L4:
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_height
	b	.L5
.L6:
	cmp	r7, #9
	bhi	.L9
	adr	r3, .L10
	ldr	pc, [r3, r7, lsl #2]
	.p2align 2
.L10:
	.word	.L8+1
	.word	.L8+1
	.word	.L8+1
	.word	.L9+1
	.word	.L17+1
	.word	.L13+1
	.word	.L17+1
	.word	.L25+1
	.word	.L8+1
	.word	.L8+1
	.p2align 1
.L13:
	add	r3, r4, r4, lsl #1
	lsrs	r3, r3, #1
.L12:
	cbz	r4, .L14
.L7:
	ldr	r0, [r6, #36]
	add	r0, r0, r4
	str	r0, [r6, #36]
.L14:
	cmp	r3, #0
	beq	.L8
	ldr	r2, [r6, #40]
	add	r3, r3, r2
	str	r3, [r6, #40]
	b	.L8
.L9:
	mov	r2, r7
	movw	r1, #627
	ldr	r0, .L26+4
	bl	printk
.L25:
	add	r3, r4, r4, lsl #2
	lsrs	r3, r3, #2
	b	.L12
.L17:
	mov	r3, r4
	b	.L12
.L16:
	movs	r3, #0
	b	.L7
.L27:
	.align	2
.L26:
	.word	.LC0
	.word	.LC1
	.fnend
	.size	vepu540c_jpeg_set_uv_offset.constprop.0, .-vepu540c_jpeg_set_uv_offset.constprop.0
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
	cbz	r5, .L29
	cbnz	r6, .L30
.L29:
	ldr	r1, .L55
	mov	r3, r5
	ldr	r0, .L55+4
	movs	r2, #39
	str	r6, [sp]
.L54:
	bl	printk
	b	.L53
.L30:
	ldr	r3, [r6, #4]
	cmp	r3, #8
	bhi	.L32
	ldr	r0, .L55+8
	add	r2, r6, #24
	ldr	ip, .L55+32
	movs	r3, #0
.L33:
	ldr	r1, [r6, #4]
	sub	r5, r2, #16
	cmp	r1, r3
	bgt	.L41
.L31:
	movs	r0, #0
	b	.L28
.L32:
	movs	r2, #44
	ldr	r1, .L55
	ldr	r0, .L55+12
	bl	printk
.L53:
	mov	r0, #-1
.L28:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L41:
	ldrh	lr, [r2, #-16]
	ldrh	r1, [r2, #-12]
	add	r1, r1, lr
	cmp	r1, r7
	bgt	.L42
	ldrh	r1, [r2, #-14]
	ldrh	r9, [r2, #-10]
	add	r1, r1, r9
	cmp	r1, r8
	mov	r1, #-1
	it	le
	movle	r1, #0
.L34:
	ldrh	r9, [r2, #-8]
	ldrb	r10, [r2, #-1]	@ zero_extendqisi2
	cmp	r9, #1
	bhi	.L45
	ldrh	r9, [r2, #-4]
	cmp	r9, #7
	bhi	.L45
	ldrb	r9, [r2, #-2]	@ zero_extendqisi2
	cmp	r9, #1
	bhi	.L45
	cmp	r10, #2
	it	cs
	movcs	r1, #-1
.L35:
	ldrsh	r9, [r2, #-6]
	cmp	r10, #0
	beq	.L36
	cmp	r9, #51
	ble	.L38
.L37:
	movs	r2, #65
	ldr	r1, .L55
	ldr	r0, .L55+16
	bl	printk
	ldrh	r3, [r5]
	ldr	r1, .L55
	ldr	r0, .L55+20
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
	ldr	r1, .L55
	ldr	r0, .L55+24
	str	r2, [sp]
	movs	r2, #70
	bl	printk
	ldrsh	r2, [r5, #10]
	ldrb	r3, [r5, #15]	@ zero_extendqisi2
	ldr	r1, .L55
	ldr	r0, .L55+28
	str	r2, [sp]
	movs	r2, #72
	b	.L54
.L42:
	mov	r1, #-1
	b	.L34
.L45:
	mov	r1, #-1
	b	.L35
.L36:
	add	r9, r9, #51
	uxth	r9, r9
	cmp	r9, #102
	bhi	.L37
.L38:
	cmp	r1, #0
	bne	.L37
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
	cbz	r1, .L40
	ldr	r1, [r4, #12]
	ands	r1, r1, r0
	orr	r1, r1, ip
	str	r1, [r4, #12]
.L40:
	adds	r4, r4, #16
	adds	r3, r3, #1
	adds	r2, r2, #16
	b	.L33
.L56:
	.align	2
.L55:
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
	.align	1
	.global	vepu540c_set_qpmap_smart
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_set_qpmap_smart, %function
vepu540c_set_qpmap_smart:
	.fnstart
	@ args = 28, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r7, r0
	mov	r8, r3
	mov	r10, r2
	ldrd	fp, r5, [sp, #48]
	mov	r0, r1
	ldrd	r4, r3, [sp, #56]
	ldrd	r6, r9, [sp, #64]
	cbz	r1, .L117
	cbz	r2, .L117
	cmp	r8, #0
	beq	.L117
	clz	r2, r6
	ldrb	r1, [r7, #12]	@ zero_extendqisi2
	lsrs	r2, r2, #5
	bfi	r1, r2, #0, #1
	strb	r1, [r7, #12]
	ldr	r2, [r7, #12]
	ldr	r1, .L163
	ands	r1, r1, r2
	ldr	r2, .L163+4
	orrs	r2, r2, r1
	ldr	r1, .L163+8
	str	r2, [r7, #12]
	cmp	r3, #0
	bne	.L59
	bl	mpp_buffer_get_ptr_with_caller
	adds	r5, r5, #63
	ldr	r1, .L163+8
	adds	r4, r4, #15
	asrs	r4, r4, #4
	str	r0, [sp]
	mov	r0, r10
	bl	mpp_buffer_get_ptr_with_caller
	bics	r5, r5, #63
	it	mi
	addmi	r5, r5, #15
	asrs	r5, r5, #4
	muls	r4, r5, r4
	cbz	r6, .L61
.L162:
	mov	r2, r4
	movs	r1, #0
	mov	r0, r8
	bl	memset
.L117:
	mov	r6, #-1
	b	.L58
.L61:
	lsls	r2, r4, #2
	mov	r1, r6
	bl	memset
	mov	r3, r0
	cmp	r9, #0
	bne	.L118
	add	r0, r8, #-1
	mov	r1, r9
.L63:
	cmp	r1, r4
	bge	.L67
	ldr	r2, [sp]
	ldr	r5, [r2, r1, lsl #2]
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	ubfx	r5, r5, #0, #12
	cmp	r5, #144
	lsl	r2, r2, #2
	and	r2, r2, #60
	bhi	.L119
	cmp	r5, #72
	ite	ls
	movls	r5, #0
	movhi	r5, #1
.L68:
	orrs	r2, r2, r5
	strb	r2, [r0, #1]!
	lsls	r5, r2, #28
	bmi	.L69
	tst	r2, #48
	beq	.L70
	tst	r2, #12
	beq	.L70
.L69:
	lsls	r2, r2, #30
	it	eq
	addeq	r9, r9, #1
.L70:
	adds	r1, r1, #1
	b	.L63
.L66:
	ldrb	r1, [r8, r2]	@ zero_extendqisi2
	lsls	r0, r1, #28
	bmi	.L64
	tst	r1, #48
	beq	.L65
	tst	r1, #12
	beq	.L65
.L64:
	lsls	r1, r1, #30
	it	eq
	addeq	r9, r9, #1
.L65:
	adds	r2, r2, #1
.L62:
	cmp	r2, r4
	blt	.L66
.L67:
	ldr	r1, .L163+8
	mov	r0, r10
	str	r3, [sp]
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_begin_cpu_access
	cmp	fp, #35
	ldr	r3, [sp]
	bls	.L120
	cmp	fp, #41
	bls	.L121
	cmp	fp, #46
	ite	cc
	movcc	r2, #2
	movcs	r2, #3
.L72:
	movs	r1, #100
	mul	r9, r1, r9
	movs	r1, #10
	muls	r1, r4, r1
	cmp	r1, r9
	bgt	.L73
.L114:
	ldr	r1, .L163+8
	mov	r0, r10
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_end_cpu_access
.L58:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L118:
	mov	r9, r6
	mov	r2, r6
	b	.L62
.L119:
	movs	r5, #2
	b	.L68
.L120:
	movs	r2, #0
	b	.L72
.L121:
	movs	r2, #1
	b	.L72
.L73:
	cmp	fp, #40
	ldrh	r1, [r7, #12]
	bls	.L74
	movs	r0, #27
.L160:
	bfi	r1, r0, #6, #6
	strh	r1, [r7, #12]	@ movhi
	cmp	r9, r4, lsl #1
	ldr	r1, [sp, #72]
	ite	lt
	addlt	r2, r2, #2
	addge	r2, r2, #1
	cmp	r1, #1
	it	le
	suble	r2, r2, #1
	movs	r7, #3
	cmp	r2, #0
	mov	r1, #0
	it	eq
	moveq	r2, #1
	rsb	r2, r2, #128
.L80:
	cmp	r1, r4
	bge	.L114
	ldrb	r0, [r8, r1]	@ zero_extendqisi2
	lsls	r5, r0, #28
	bmi	.L81
	tst	r0, #48
	beq	.L82
	tst	r0, #12
	beq	.L82
.L81:
	lsls	r0, r0, #30
	bne	.L82
	add	r0, r3, r1, lsl #2
	strb	r2, [r3, r1, lsl #2]
	ldrb	r5, [r0, #2]	@ zero_extendqisi2
	bfi	r5, r7, #0, #4
	strb	r5, [r0, #2]
.L82:
	adds	r1, r1, #1
	b	.L80
.L74:
	cmp	fp, #35
	ite	hi
	movhi	r0, #26
	movls	r0, #25
	b	.L160
.L59:
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L163+8
	adds	r5, r5, #31
	adds	r4, r4, #31
	str	r0, [sp, #4]
	mov	r0, r10
	bl	mpp_buffer_get_ptr_with_caller
	bics	r5, r5, #31
	it	mi
	addmi	r5, r5, #15
	bics	r4, r4, #31
	it	mi
	addmi	r4, r4, #15
	asrs	r5, r5, #4
	asrs	r4, r4, #4
	muls	r4, r5, r4
	str	r0, [sp]
	cmp	r6, #0
	bne	.L162
	bic	r2, r4, #-1073741824
	mov	r1, r6
	ldr	r0, [sp]
	bl	memset
	cmp	r9, #0
	bne	.L123
	add	r1, r8, #-1
	mov	r5, r6
	mov	r2, r6
.L89:
	cmp	r2, r4
	bge	.L93
	ldr	r3, [sp, #4]
	ldrb	r0, [r1, #1]	@ zero_extendqisi2
	ldr	r3, [r3, r2, lsl #2]
	lsls	r0, r0, #2
	ubfx	r3, r3, #0, #12
	and	r0, r0, #60
	cmp	r3, #144
	bhi	.L125
	cmp	r3, #72
	ite	ls
	movls	r3, #0
	movhi	r3, #1
.L94:
	orrs	r3, r3, r0
	strb	r3, [r1, #1]!
	tst	r3, #40
	ite	ne
	movne	ip, #1
	moveq	ip, #0
	ands	r0, r3, #48
	beq	.L95
	tst	r3, #12
	ite	ne
	movne	r0, #1
	moveq	r0, #0
.L95:
	tst	r3, #3
	orr	r0, r0, ip
	it	ne
	movne	r0, #0
	cbz	r0, .L96
	adds	r5, r5, #1
.L96:
	adds	r2, r2, #1
	b	.L89
.L164:
	.align	2
.L163:
	.word	-524287
	.word	209570
	.word	.LANCHOR0+17
.L92:
	ldrb	r1, [r8, r2]	@ zero_extendqisi2
	tst	r1, #40
	ite	ne
	movne	r0, #1
	moveq	r0, #0
	ands	r3, r1, #48
	beq	.L90
	tst	r1, #12
	ite	ne
	movne	r3, #1
	moveq	r3, #0
.L90:
	tst	r1, #3
	orr	r3, r3, r0
	it	ne
	movne	r3, #0
	cbz	r3, .L91
	adds	r5, r5, #1
.L91:
	adds	r2, r2, #1
.L88:
	cmp	r2, r4
	blt	.L92
.L93:
	ldr	r1, .L165
	mov	r0, r10
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_begin_cpu_access
	cmp	fp, #35
	bls	.L127
	cmp	fp, #41
	bls	.L128
	cmp	fp, #46
	ite	cc
	movcc	r3, #2
	movcs	r3, #3
.L98:
	movs	r2, #100
	muls	r5, r2, r5
	movs	r2, #10
	muls	r2, r4, r2
	cmp	r2, r5
	ble	.L114
	cmp	fp, #40
	ldrh	r2, [r7, #12]
	bls	.L100
	movs	r1, #27
.L161:
	cmp	r5, r4
	bfi	r2, r1, #6, #6
	strh	r2, [r7, #12]	@ movhi
	bge	.L105
	adds	r3, r3, #5
.L106:
	ldr	r2, [sp, #72]
	movs	r1, #0
	mov	r0, r1
	mov	r5, r1
	cmp	r2, #1
	it	le
	suble	r3, r3, #1
	rsb	r3, r3, #128
.L109:
	cmp	r1, r4
	bge	.L114
	ldrb	r7, [r8, r1]	@ zero_extendqisi2
	tst	r7, #40
	ite	ne
	movne	ip, #1
	moveq	ip, #0
	ands	r2, r7, #48
	beq	.L110
	tst	r7, #12
	ite	ne
	movne	r2, #1
	moveq	r2, #0
.L110:
	tst	r7, #3
	orr	r2, r2, ip
	add	r1, r1, #1
	it	ne
	movne	r2, #0
	cmp	r0, r2
	it	cc
	movcc	r0, r2
	lsls	r2, r1, #30
	bne	.L109
	cbz	r0, .L112
	ldr	r2, [sp]
	strb	r3, [r2, r5, lsl #2]
.L112:
	adds	r5, r5, #1
	movs	r0, #0
	b	.L109
.L123:
	mov	r5, r6
	mov	r2, r6
	b	.L88
.L125:
	movs	r3, #2
	b	.L94
.L127:
	movs	r3, #0
	b	.L98
.L128:
	movs	r3, #1
	b	.L98
.L100:
	cmp	fp, #35
	ldr	r1, [sp, #72]
	bls	.L102
	cmp	r1, #2
	ite	lt
	movlt	r1, #26
	movge	r1, #25
	b	.L161
.L102:
	cmp	r1, #2
	ite	lt
	movlt	r1, #25
	movge	r1, #24
	b	.L161
.L105:
	add	r2, r4, r4, lsl #1
	cmp	r2, r5
	ble	.L107
	adds	r3, r3, #4
	b	.L106
.L107:
	add	r2, r4, r4, lsl #2
	cmp	r2, r5
	ite	gt
	addgt	r3, r3, #3
	addle	r3, r3, #2
	b	.L106
.L166:
	.align	2
.L165:
	.word	.LANCHOR0+17
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
	@ args = 28, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	mov	r7, r0
	mov	r9, r2
	mov	r8, r3
	ldrd	r5, r4, [sp, #60]
	ldrd	r2, r6, [sp, #68]
	mov	r0, r1
	ldr	fp, [sp, #76]
	cbz	r1, .L233
	cmp	r9, #0
	beq	.L233
	cbz	r3, .L233
	clz	r1, r6
	ldrb	ip, [r7, #12]	@ zero_extendqisi2
	ldr	r3, .L278
	lsrs	r1, r1, #5
	bfi	ip, r1, #0, #1
	strb	ip, [r7, #12]
	ldr	r1, [r7, #12]
	ands	r3, r3, r1
	ldr	r1, .L278+4
	orrs	r1, r1, r3
	str	r1, [r7, #12]
	ldr	r1, .L278+8
	cmp	r2, #0
	bne	.L169
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L278+8
	adds	r5, r5, #63
	adds	r4, r4, #15
	asrs	r4, r4, #4
	str	r0, [sp, #4]
	mov	r0, r9
	bl	mpp_buffer_get_ptr_with_caller
	bics	r5, r5, #63
	it	mi
	addmi	r5, r5, #15
	asrs	r5, r5, #4
	muls	r4, r5, r4
	mov	r10, r0
	cbz	r6, .L171
.L277:
	mov	r2, r4
	movs	r1, #0
	mov	r0, r8
	bl	memset
.L233:
	mov	r6, #-1
	b	.L168
.L171:
	lsls	r2, r4, #2
	mov	r1, r6
	bl	memset
	cmp	fp, #0
	bne	.L234
	add	r0, r8, #-1
	mov	r1, fp
.L173:
	cmp	r1, r4
	bge	.L177
	ldr	r3, [sp, #4]
	ldrb	r2, [r0, #1]	@ zero_extendqisi2
	ldr	r5, [r3, r1, lsl #2]
	lsls	r2, r2, #2
	ubfx	r5, r5, #0, #12
	and	r2, r2, #60
	cmp	r5, #144
	bhi	.L235
	cmp	r5, #72
	ite	ls
	movls	r5, #0
	movhi	r5, #1
.L178:
	orrs	r2, r2, r5
	strb	r2, [r0, #1]!
	lsls	r3, r2, #28
	bmi	.L179
	tst	r2, #48
	beq	.L180
	tst	r2, #12
	beq	.L180
.L179:
	lsls	r3, r2, #30
	it	eq
	addeq	fp, fp, #1
.L180:
	adds	r1, r1, #1
	b	.L173
.L176:
	ldrb	r1, [r8, r2]	@ zero_extendqisi2
	lsls	r0, r1, #28
	bmi	.L174
	tst	r1, #48
	beq	.L175
	tst	r1, #12
	beq	.L175
.L174:
	lsls	r1, r1, #30
	it	eq
	addeq	fp, fp, #1
.L175:
	adds	r2, r2, #1
.L172:
	cmp	r2, r4
	blt	.L176
.L177:
	ldr	r3, [sp, #56]
	cmp	r3, #35
	bls	.L236
	cmp	r3, #46
	ite	cc
	movcc	r5, #4
	movcs	r5, #3
.L182:
	movs	r3, #100
	mul	fp, r3, fp
	rsb	r3, r4, r4, lsl #4
	cmp	r3, fp
	ble	.L183
	cmp	fp, r4, asr #5
	ble	.L183
	ldr	r2, [sp, #56]
	cmp	r2, #40
	ldrh	r2, [r7, #12]
	bls	.L184
	movs	r1, #28
.L275:
	bfi	r2, r1, #6, #6
	strh	r2, [r7, #12]	@ movhi
.L183:
	ldr	r1, .L278+8
	mov	r0, r9
	str	r3, [sp, #4]
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_begin_cpu_access
	ldr	r3, [sp, #4]
	cmp	r3, fp
	bgt	.L186
.L220:
	ldr	r1, .L278+8
	mov	r0, r9
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_end_cpu_access
.L168:
	mov	r0, r6
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L234:
	mov	fp, r6
	mov	r2, r6
	b	.L172
.L235:
	movs	r5, #2
	b	.L178
.L236:
	movs	r5, #0
	b	.L182
.L184:
	ldr	r1, [sp, #56]
	cmp	r1, #35
	ite	hi
	movhi	r1, #27
	movls	r1, #25
	b	.L275
.L186:
	cmp	fp, #0
	ble	.L220
	cmp	fp, r4
	bge	.L188
	adds	r5, r5, #7
.L189:
	ldr	r3, [sp, #80]
	cmp	r3, #1
	mov	r3, #0
	it	le
	suble	r5, r5, #2
	rsb	r5, r5, #128
.L193:
	cmp	r3, r4
	bge	.L220
	ldrb	r2, [r8, r3]	@ zero_extendqisi2
	lsls	r0, r2, #28
	bmi	.L194
	tst	r2, #48
	beq	.L195
	tst	r2, #12
	beq	.L195
.L194:
	lsls	r1, r2, #30
	it	eq
	strbeq	r5, [r10, r3, lsl #2]
.L195:
	adds	r3, r3, #1
	b	.L193
.L188:
	add	r3, r4, r4, lsl #1
	cmp	r3, fp
	ble	.L190
	adds	r5, r5, #6
	b	.L189
.L190:
	rsb	r3, r4, r4, lsl #3
	cmp	r3, fp
	ite	gt
	addgt	r5, r5, #5
	addle	r5, r5, #4
	b	.L189
.L169:
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L278+8
	adds	r5, r5, #31
	adds	r4, r4, #31
	mov	r10, r0
	mov	r0, r9
	bl	mpp_buffer_get_ptr_with_caller
	bics	r5, r5, #31
	it	mi
	addmi	r5, r5, #15
	bics	r4, r4, #31
	it	mi
	addmi	r4, r4, #15
	asrs	r5, r5, #4
	asrs	r4, r4, #4
	muls	r4, r5, r4
	str	r0, [sp, #8]
	cmp	r6, #0
	bne	.L277
	bic	r2, r4, #-1073741824
	mov	r1, r6
	ldr	r0, [sp, #8]
	bl	memset
	cmp	r4, #0
	mov	r2, r4
	it	lt
	addlt	r2, r4, #3
	asrs	r3, r2, #2
	str	r3, [sp, #12]
	cmp	fp, #0
	bne	.L201
	add	r10, r10, #16
	mov	r3, fp
	mov	r0, fp
.L202:
	ldr	r2, [sp, #12]
	cmp	r0, r2
	bge	.L208
	add	lr, r8, r0, lsl #2
	mov	fp, #0
	sub	r2, r10, #16
	str	r2, [sp, #4]
.L212:
	ldr	r1, [sp, #4]
	ldr	r2, [r1], #4
	ubfx	r2, r2, #0, #12
	str	r1, [sp, #4]
	cmp	r2, #160
	ldrb	r1, [lr], #1	@ zero_extendqisi2
	lsl	r1, r1, #2
	and	r1, r1, #60
	bhi	.L239
	cmp	r2, #120
	bhi	.L240
	cmp	r2, #72
	ite	ls
	movls	r2, #0
	movhi	r2, #1
.L209:
	orrs	r2, r2, r1
	strb	r2, [lr, #-1]
	and	ip, r2, #12
	sub	r5, ip, #12
	rsbs	r1, r5, #0
	adcs	r1, r1, r5
	mov	r5, r1
	ands	r1, r2, #32
	beq	.L210
	adds	r1, ip, #0
	it	ne
	movne	r1, #1
.L210:
	tst	r2, #3
	orr	r1, r1, r5
	it	ne
	movne	r1, #0
	cbz	r1, .L211
	add	fp, fp, #1
.L211:
	ldr	r2, [sp, #4]
	cmp	r10, r2
	bne	.L212
	cmp	fp, #0
	it	gt
	addgt	r3, r3, #1
	adds	r0, r0, #1
	add	r10, r10, #16
	b	.L202
.L279:
	.align	2
.L278:
	.word	-524287
	.word	209570
	.word	.LANCHOR0+42
.L201:
	movs	r3, #0
	add	r1, r8, #4
	mov	r5, r3
.L203:
	ldr	r2, [sp, #12]
	cmp	r5, r2
	blt	.L207
.L208:
	ldr	r2, [sp, #56]
	cmp	r2, #35
	bls	.L242
	cmp	r2, #46
	ite	cc
	movcc	r5, #3
	movcs	r5, #2
.L214:
	mov	r10, #400
	rsb	fp, r4, r4, lsl #4
	mul	r10, r10, r3
	cmp	fp, r10
	ble	.L215
	cmp	r10, r4, asr #5
	ble	.L215
	ldr	r3, [sp, #56]
	cmp	r3, #40
	ldrh	r3, [r7, #12]
	bls	.L216
	movs	r2, #29
.L276:
	bfi	r3, r2, #6, #6
	strh	r3, [r7, #12]	@ movhi
.L215:
	ldr	r1, .L280
	mov	r0, r9
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #2
	bl	dma_buf_begin_cpu_access
	cmp	fp, r10
	ble	.L220
	cmp	r10, #0
	ble	.L220
	cmp	r10, r4
	bge	.L221
	adds	r5, r5, #7
.L222:
	ldr	r3, [sp, #80]
	movs	r2, #0
	mov	r1, r2
	mov	r0, r2
	cmp	r3, #1
	it	le
	suble	r5, r5, #2
	rsb	r5, r5, #128
.L226:
	cmp	r2, r4
	bge	.L220
	ldrb	r7, [r8, r2]	@ zero_extendqisi2
	and	ip, r7, #12
	sub	r10, ip, #12
	rsbs	lr, r10, #0
	adc	lr, lr, r10
	ands	r3, r7, #32
	beq	.L227
	adds	r3, ip, #0
	it	ne
	movne	r3, #1
.L227:
	tst	r7, #3
	orr	r3, r3, lr
	add	r2, r2, #1
	it	ne
	movne	r3, #0
	cmp	r1, r3
	it	cc
	movcc	r1, r3
	lsls	r3, r2, #30
	bne	.L226
	cbz	r1, .L229
	ldr	r3, [sp, #8]
	strb	r5, [r3, r0, lsl #2]
.L229:
	adds	r0, r0, #1
	movs	r1, #0
	b	.L226
.L207:
	sub	lr, r1, #4
	mov	ip, #0
.L206:
	ldrb	r0, [lr], #1	@ zero_extendqisi2
	and	r10, r0, #12
	sub	r2, r10, #12
	rsbs	fp, r2, #0
	adc	fp, fp, r2
	ands	r2, r0, #32
	beq	.L204
	adds	r2, r10, #0
	it	ne
	movne	r2, #1
.L204:
	tst	r0, #3
	orr	r2, r2, fp
	it	ne
	movne	r2, #0
	cbz	r2, .L205
	add	ip, ip, #1
.L205:
	cmp	r1, lr
	bne	.L206
	cmp	ip, #0
	it	gt
	addgt	r3, r3, #1
	adds	r5, r5, #1
	adds	r1, r1, #4
	b	.L203
.L239:
	movs	r2, #3
	b	.L209
.L240:
	movs	r2, #2
	b	.L209
.L242:
	movs	r5, #0
	b	.L214
.L216:
	ldr	r2, [sp, #56]
	cmp	r2, #35
	bls	.L217
	movs	r2, #28
	b	.L276
.L217:
	ldr	r2, [sp, #80]
	cmp	r2, #2
	ite	lt
	movlt	r2, #27
	movge	r2, #26
	b	.L276
.L221:
	add	r3, r4, r4, lsl #1
	cmp	r3, r10
	ble	.L223
	adds	r5, r5, #6
	b	.L222
.L223:
	rsb	r3, r4, r4, lsl #3
	cmp	r3, r10
	ite	gt
	addgt	r5, r5, #5
	addle	r5, r5, #4
	b	.L222
.L281:
	.align	2
.L280:
	.word	.LANCHOR0+42
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
	beq	.L298
	ldr	r6, [r4, #4]
	cmp	r6, #0
	beq	.L298
	cmp	r6, #8
	bls	.L284
	mov	r3, r6
	movw	r2, #503
	ldr	r1, .L299
	ldr	r0, .L299+4
	bl	printk
	ldr	r3, [r4, #4]
	cmp	r3, #8
	bls	.L290
	mov	r1, #504
	ldr	r3, .L299+8
	str	r1, [sp]
	ldr	r2, .L299+12
	ldr	r0, .L299+16
	bl	printk
.L290:
	mov	r0, #-1
	b	.L282
.L284:
	ldr	r5, [r0]
	adds	r4, r4, #148
	mov	r9, #0
	mov	r10, #36
	add	r7, r5, #24
.L287:
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
	cbz	r1, .L285
	movs	r2, #0
	ldr	r0, [r8, #4]
	bl	mpp_dev_get_mpi_ioaddress
	str	r0, [r7, #-8]
.L285:
	ldr	r1, [r4, #-12]
	cbz	r1, .L286
	movs	r2, #0
	ldr	r0, [r8, #4]
	bl	mpp_dev_get_mpi_ioaddress
	str	r0, [r7, #-12]
.L286:
	mov	r2, r4
	add	r9, r9, #1
	ldr	r3, [r2, #-8]!	@ unaligned
	cmp	r6, r9
	add	r4, r4, #140
	add	r7, r7, #36
	str	r3, [r7, #-36]	@ unaligned
	ldr	r3, [r2, #4]	@ unaligned
	str	r3, [r7, #-32]	@ unaligned
	bne	.L287
	ldr	r3, [r5, #288]
	and	r2, r3, #-134217728
	ldr	r3, .L299+20
	orrs	r3, r3, r2
	str	r3, [r5, #288]
	ldr	r3, [r5, #292]
	and	r2, r3, #-134217728
	ldr	r3, .L299+24
	orrs	r3, r3, r2
	str	r3, [r5, #292]
	ldr	r3, [r5, #296]
	and	r2, r3, #-134217728
	ldr	r3, .L299+28
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
.L298:
	movs	r0, #0
.L282:
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L300:
	.align	2
.L299:
	.word	.LANCHOR0+68
	.word	.LC8
	.word	.LANCHOR0+85
	.word	.LC9
	.word	.LC10
	.word	7679053
	.word	33773525
	.word	128920192
	.fnend
	.size	vepu540c_set_osd, .-vepu540c_set_osd
	.global	__aeabi_uidivmod
	.align	1
	.global	vepu540c_set_jpeg_reg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_set_jpeg_reg, %function
vepu540c_set_jpeg_reg:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r5, [r0, #12]
	mov	r6, r0
	ldr	r4, [r0, #4]
	ldr	r10, [r0, #16]
	ldr	r3, [r5, #52]
	ldr	fp, [r5, #16]
	mov	r0, r3
	str	r3, [sp]
	ldrd	r9, r8, [fp]
	bl	mpp_frame_get_height
	cmp	r0, r8
	bcs	.L322
	ldr	r7, [fp, #136]
	cbz	r7, .L302
	ldr	r0, [sp]
	bl	mpp_frame_get_width
	mov	r9, r0
	ldr	r0, [sp]
	bl	mpp_frame_get_height
	ldr	r2, [r6, #24]
	mov	r8, r0
	cbz	r2, .L324
	movs	r1, #0
	ldr	r0, [r5, #28]
	str	r1, [r5, #48]
	bl	mpp_packet_set_length
.L324:
	movs	r7, #1
	b	.L302
.L322:
	movs	r7, #0
.L302:
	ldr	r2, [r6, #20]
	cbnz	r2, .L303
	mov	r2, #264
	ldr	r1, [r5, #56]
	ldr	r0, [r6]
	bl	mpp_dev_get_iova_address
	mov	r2, r5
	strd	r0, r0, [r4, #32]
	str	r0, [r4, #40]
	mov	r0, r4
	ldr	r1, [r10]
	bl	vepu540c_jpeg_set_uv_offset.constprop.0
.L303:
	ldr	r2, [r5, #32]
	ldr	r1, [r2, #28]
	ldr	r0, [r2]
	cmp	r1, #0
	bne	.L304
	cmp	r0, #0
	beq	.L305
	movw	r2, #257
	mov	r1, r0
	ldr	r0, [r6]
	bl	mpp_dev_get_iova_address
	ldr	r2, [r5, #32]
	ldr	r2, [r2, #12]
	add	r0, r0, r2
	str	r0, [r4, #4]
.L306:
	ldr	r1, [r5, #32]
	ldr	r2, [r4, #4]
	ldr	r1, [r1, #24]
	subs	r0, r2, #1
	str	r2, [r4, #8]
	add	r1, r1, r0
	str	r1, [r4]
	ldr	r0, [r5, #28]
	str	r2, [sp, #4]
	bl	mpp_packet_get_length
	ldr	r2, [sp, #4]
	add	r0, r0, r2
	str	r0, [r4, #12]
.L307:
	add	r1, r9, #15
	ldr	r2, .L335
	ldrh	r0, [r4, #64]
	uxtb	r9, r9
	and	r1, r2, r1, lsr #3
	subs	r1, r1, #1
	bfi	r0, r1, #0, #11
	add	r1, r9, #15
	bic	r1, r1, #15
	strh	r0, [r4, #64]	@ movhi
	sub	r9, r1, r9
	ldrb	r1, [r4, #68]	@ zero_extendqisi2
	bfi	r1, r9, #0, #6
	strb	r1, [r4, #68]
	add	r1, r8, #15
	uxtb	r8, r8
	and	r2, r2, r1, lsr #3
	ldrh	r1, [r4, #66]
	subs	r2, r2, #1
	bfi	r1, r2, #0, #11
	add	r2, r8, #15
	bic	r2, r2, #15
	strh	r1, [r4, #66]	@ movhi
	sub	r8, r2, r8
	ldrb	r2, [r4, #70]	@ zero_extendqisi2
	bfi	r2, r8, #0, #6
	strb	r2, [r4, #70]
	ldr	r0, [r10]
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	and	r0, r0, #15
	bfi	r2, r0, #2, #4
	strb	r2, [r4, #72]
	ldr	r1, [r10, #4]
	bfi	r2, r1, #0, #1
	strb	r2, [r4, #72]
	ldr	r1, [r10, #8]
	bfi	r2, r1, #1, #1
	strb	r2, [r4, #72]
	ldrh	r2, [r4, #72]
	bic	r2, r2, #1920
	orr	r2, r2, #128
	strh	r2, [r4, #72]	@ movhi
	ldr	r2, [fp, #36]
	cmp	r2, #0
	ldrb	r2, [r4, #95]	@ zero_extendqisi2
	ite	le
	movle	r1, #0
	movgt	r1, #1
	bfi	r2, r1, #2, #1
	strb	r2, [r4, #95]
	ldr	r1, [fp, #32]
	bfi	r2, r1, #3, #2
	strb	r2, [r4, #95]
	ldr	r2, [fp, #8]
	ldrb	r1, [r4, #72]	@ zero_extendqisi2
	cbnz	r2, .L309
	ands	ip, r1, #60
	bne	.L310
	ldr	r2, [fp]
	lsls	r2, r2, #2
.L309:
	and	r1, r1, #52
	cmp	r1, #16
	itet	ne
	addne	ip, r2, r2, lsr #31
	moveq	ip, r2
	asrne	ip, ip, #1
	cmp	r0, #2
	bhi	.L314
	ldr	r1, [r4, #76]
	and	r0, r1, #-134217728
	ldr	r1, .L335+4
	orrs	r1, r1, r0
	str	r1, [r4, #76]
	ldr	r1, [r4, #80]
	and	r0, r1, #-134217728
	ldr	r1, .L335+8
	orrs	r1, r1, r0
	str	r1, [r4, #80]
	ldr	r1, [r4, #84]
	and	r0, r1, #-134217728
	ldr	r1, .L335+12
	orrs	r1, r1, r0
	str	r1, [r4, #84]
	ldr	r1, [r4, #88]
	lsrs	r1, r1, #21
	lsls	r1, r1, #21
	orr	r1, r1, #32768
	orr	r1, r1, #128
	str	r1, [r4, #88]
.L314:
	ldr	r1, [r4, #100]
	strh	ip, [r4, #104]	@ movhi
	bfi	r1, r2, #0, #17
	str	r1, [r4, #100]
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_offset_y
	ldrh	r2, [r4, #98]
	bfi	r2, r0, #0, #14
	strh	r2, [r4, #98]	@ movhi
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_offset_x
	ldrh	r2, [r4, #96]
	bfi	r2, r0, #0, #14
	strh	r2, [r4, #96]	@ movhi
	ldrh	r2, [r4, #108]
	bic	r2, r2, #816
	bic	r2, r2, #3
	strh	r2, [r4, #108]	@ movhi
	ldrh	r2, [r4, #112]
	bfc	r2, #0, #15
	strh	r2, [r4, #112]	@ movhi
	ldrh	r2, [r4, #116]
	bfc	r2, #0, #15
	strh	r2, [r4, #116]	@ movhi
	ldrh	r2, [r4, #120]
	bfc	r2, #0, #15
	strh	r2, [r4, #120]	@ movhi
	ldr	r2, [r4, #124]
	ldr	r3, [fp, #136]
	bfi	r2, r3, #0, #25
	str	r2, [r4, #124]
	ubfx	r3, r2, #24, #8
	bfc	r3, #1, #1
	strb	r3, [r4, #127]
	ldr	r2, [r6, #24]
	bfi	r3, r2, #2, #3
	strb	r3, [r4, #127]
	cmp	r7, #0
	beq	.L315
	ldr	r0, [sp]
	bl	mpp_frame_get_eos
	ldrb	r3, [r4, #127]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L316
	orr	r3, r3, #32
	strb	r3, [r4, #127]
	movs	r3, #0
.L333:
	str	r3, [r6, #24]
.L317:
	ldrb	r3, [r4, #127]	@ zero_extendqisi2
	and	r3, r3, #63
	orn	r3, r3, #127
	strb	r3, [r4, #127]
	ldr	r3, [r4, #128]
	and	r3, r3, #-1073741824
	str	r3, [r4, #128]
	ldr	r3, [r6, #20]
	cmp	r3, #0
	bne	.L318
.L334:
	movs	r0, #0
	b	.L301
.L305:
	ldr	r1, [r2, #4]
	ldr	r2, [r2, #12]
	add	r2, r2, r1
	str	r2, [r4, #4]
	b	.L306
.L304:
	ldr	r1, .L335+16
	bl	mpp_buffer_get_size_with_caller
	ldr	r1, [r5, #32]
	movw	r2, #257
	ldr	r1, [r1]
	str	r0, [sp, #4]
	ldr	r0, [r6]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #4]
	ldr	r2, [r5, #32]
	str	r0, [sp, #12]
	ldr	r0, [r5, #28]
	ldr	r2, [r2, #12]
	str	r2, [sp, #8]
	bl	mpp_packet_get_length
	ldrd	r1, r2, [sp, #4]
	add	r0, r0, r2
	bl	__aeabi_uidivmod
	ldr	r2, [r4, #4]
	ldr	r3, [sp, #12]
	add	r1, r1, r3
	str	r1, [r4, #12]
	ldr	r1, [r5, #32]
	ldr	r3, [sp, #4]
	ldr	r1, [r1, #16]
	add	r1, r1, r2
	add	r2, r2, r3
	str	r1, [r4, #8]
	str	r2, [r4]
	b	.L307
.L310:
	cmp	ip, #4
	bne	.L311
	ldr	r2, [fp]
	add	r2, r2, r2, lsl #1
	b	.L309
.L311:
	cmp	ip, #8
	beq	.L312
	and	ip, r1, #56
	cmp	ip, #32
	bne	.L309
.L312:
	ldr	r2, [fp]
	lsls	r2, r2, #1
	b	.L309
.L316:
	bfi	r3, r0, #5, #1
	strb	r3, [r4, #127]
	ldr	r3, [r6, #24]
	adds	r3, r3, #1
	b	.L333
.L315:
	ldrb	r3, [r4, #127]	@ zero_extendqisi2
	orr	r3, r3, #32
	strb	r3, [r4, #127]
	b	.L317
.L318:
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_is_full
	cmp	r0, #0
	beq	.L334
	ldr	r0, [r5, #52]
	bl	mpp_frame_get_phy_addr
	cbnz	r0, .L321
	movw	r1, #787
	ldr	r0, .L335+20
	bl	printk
	mov	r0, #-1
.L301:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L321:
	strd	r0, r0, [r4, #32]
	mov	r2, r5
	str	r0, [r4, #40]
	mov	r0, r4
	ldr	r1, [r10]
	bl	vepu540c_jpeg_set_uv_offset.constprop.0
	b	.L334
.L336:
	.align	2
.L335:
	.word	536870910
	.word	20261917
	.word	123164288
	.word	33762283
	.word	.LANCHOR0+102
	.word	.LC11
	.fnend
	.size	vepu540c_set_jpeg_reg, .-vepu540c_set_jpeg_reg
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.35495, %object
	.size	__func__.35495, 17
__func__.35495:
	.ascii	"vepu540c_set_roi\000"
	.type	__FUNCTION__.35531, %object
	.size	__FUNCTION__.35531, 25
__FUNCTION__.35531:
	.ascii	"vepu540c_set_qpmap_smart\000"
	.type	__FUNCTION__.35588, %object
	.size	__FUNCTION__.35588, 26
__FUNCTION__.35588:
	.ascii	"vepu540c_set_qpmap_normal\000"
	.type	__func__.35626, %object
	.size	__func__.35626, 17
__func__.35626:
	.ascii	"vepu540c_set_osd\000"
	.type	__FUNCTION__.35627, %object
	.size	__FUNCTION__.35627, 17
__FUNCTION__.35627:
	.ascii	"vepu540c_set_osd\000"
	.type	__FUNCTION__.35673, %object
	.size	__FUNCTION__.35673, 22
__FUNCTION__.35673:
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
	.ascii	"\0013%d: online case set full frame err\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
