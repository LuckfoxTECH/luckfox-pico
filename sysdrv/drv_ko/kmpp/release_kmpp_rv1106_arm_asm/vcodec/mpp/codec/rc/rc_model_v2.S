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
	.file	"rc_model_v2.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	bits_model_param_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_model_param_deinit, %function
bits_model_param_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r5, .L62
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L2
	mov	r3, r0
	movs	r2, #73
	ldr	r1, .L62+4
	ldr	r0, .L62+8
	bl	printk
.L2:
	ldr	r0, [r4, #400]
	cbz	r0, .L3
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #400]
.L3:
	ldr	r0, [r4, #436]
	cbz	r0, .L4
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #436]
.L4:
	ldr	r0, [r4, #424]
	cbz	r0, .L5
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #424]
.L5:
	ldr	r0, [r4, #448]
	cbz	r0, .L6
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #448]
.L6:
	ldr	r0, [r4, #452]
	cbz	r0, .L7
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #452]
.L7:
	ldr	r0, [r4, #456]
	cbz	r0, .L8
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #456]
.L8:
	ldr	r0, [r4, #460]
	cbz	r0, .L9
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #460]
.L9:
	ldr	r0, [r4, #464]
	cbz	r0, .L10
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #464]
.L10:
	ldr	r0, [r4, #536]
	cbz	r0, .L11
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #536]
.L11:
	ldr	r0, [r4, #528]
	cbz	r0, .L12
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #528]
.L12:
	ldr	r0, [r4, #532]
	cbz	r0, .L13
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #532]
.L13:
	ldr	r0, [r4, #468]
	cbz	r0, .L14
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #468]
.L14:
	ldr	r0, [r4, #472]
	cbz	r0, .L15
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #472]
.L15:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L16
	mov	r3, r4
	movs	r2, #135
	ldr	r1, .L62+4
	ldr	r0, .L62+12
	bl	printk
.L16:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L63:
	.align	2
.L62:
	.word	rc_debug
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.fnend
	.size	bits_model_param_deinit, .-bits_model_param_deinit
	.global	__aeabi_idiv
	.align	1
	.global	bits_model_param_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_model_param_init, %function
bits_model_param_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r3, [r0, #28]
	ldr	r6, [r0, #40]
	ldr	r0, [r0, #60]
	ldr	r1, [r4, #32]
	muls	r0, r3, r0
	bl	__aeabi_idiv
	subs	r5, r0, #0
	mov	r0, r4
	it	eq
	moveq	r5, #1
	bl	bits_model_param_deinit
	movs	r2, #0
	movs	r1, #2
	add	r0, r4, #400
	bl	mpp_data_init_v2
	ldr	r3, [r4, #400]
	cbnz	r3, .L66
	ldr	r0, .L83
	movs	r1, #151
.L82:
	bl	printk
.L81:
	mvn	r0, #3
.L64:
	pop	{r4, r5, r6, pc}
.L66:
	movs	r2, #0
	movs	r1, #2
	add	r0, r4, #424
	bl	mpp_data_init_v2
	ldr	r3, [r4, #424]
	cbnz	r3, .L68
	movs	r1, #157
	ldr	r0, .L83+4
	b	.L82
.L68:
	movs	r2, #0
	movs	r1, #5
	add	r0, r4, #436
	bl	mpp_data_init_v2
	ldr	r3, [r4, #436]
	cbnz	r3, .L69
	movs	r1, #162
	ldr	r0, .L83+8
	b	.L82
.L69:
	movs	r2, #0
	movs	r1, #8
	add	r0, r4, #448
	bl	mpp_data_init_v2
	ldr	r3, [r4, #448]
	cbnz	r3, .L70
	movs	r1, #168
	ldr	r0, .L83+12
	b	.L82
.L70:
	movs	r2, #0
	movs	r1, #2
	add	r0, r4, #452
	bl	mpp_data_init_v2
	ldr	r3, [r4, #452]
	cbnz	r3, .L71
	movs	r1, #174
	ldr	r0, .L83+16
	b	.L82
.L71:
	mov	r2, #-1
	movs	r1, #2
	add	r0, r4, #456
	bl	mpp_data_init_v2
	ldr	r3, [r4, #456]
	cbnz	r3, .L72
	movs	r1, #180
	ldr	r0, .L83+20
	b	.L82
.L72:
	movs	r2, #0
	movs	r1, #8
	add	r0, r4, #460
	bl	mpp_data_init_v2
	ldr	r3, [r4, #460]
	cbnz	r3, .L73
	movs	r1, #185
	ldr	r0, .L83+24
	b	.L82
.L73:
	movs	r2, #0
	movs	r1, #8
	add	r0, r4, #464
	bl	mpp_data_init_v2
	ldr	r3, [r4, #464]
	cbnz	r3, .L74
	movs	r1, #190
	ldr	r0, .L83+28
	b	.L82
.L74:
	movs	r2, #0
	ldr	r1, [r4, #28]
	add	r0, r4, #536
	bl	mpp_data_init_v2
	ldr	r3, [r4, #536]
	cbnz	r3, .L75
	ldr	r2, [r4, #28]
	movs	r1, #195
	ldr	r0, .L83+32
.L80:
	bl	printk
	b	.L81
.L75:
	ldr	r2, [r4, #376]
	mov	r1, r5
	add	r0, r4, #528
	bl	mpp_data_init_v2
	ldr	r3, [r4, #528]
	cbnz	r3, .L76
	mov	r2, r5
	movs	r1, #201
	ldr	r0, .L83+36
	b	.L80
.L76:
	movs	r2, #0
	mov	r1, r6
	add	r0, r4, #532
	bl	mpp_data_init_v2
	ldr	r3, [r4, #532]
	cbnz	r3, .L77
	mov	r2, r6
	movs	r1, #207
	ldr	r0, .L83+40
	b	.L80
.L77:
	movs	r2, #0
	movs	r1, #10
	add	r0, r4, #468
	bl	mpp_data_init_v2
	ldr	r3, [r4, #468]
	cbnz	r3, .L78
	movs	r2, #10
	movs	r1, #213
	ldr	r0, .L83+44
	b	.L80
.L78:
	movs	r2, #0
	movs	r1, #10
	add	r0, r4, #472
	bl	mpp_data_init_v2
	ldr	r3, [r4, #472]
	cbnz	r3, .L79
	movs	r2, #10
	movs	r1, #219
	ldr	r0, .L83+48
	b	.L80
.L79:
	movs	r0, #0
	b	.L64
.L84:
	.align	2
.L83:
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.fnend
	.size	bits_model_param_init, .-bits_model_param_init
	.global	__aeabi_uidiv
	.align	1
	.global	bits_frm_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_frm_init, %function
bits_frm_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r0
	ldr	r6, .L106
	.pad #20
	sub	sp, sp, #20
	ldr	r5, [r0, #40]
	ldr	r3, [r6]
	lsls	r7, r3, #31
	bpl	.L86
	mov	r3, r0
	movs	r2, #232
	ldr	r1, .L106+4
	ldr	r0, .L106+8
	bl	printk
.L86:
	ldr	r3, [r4, #36]
	cbz	r3, .L87
	cmp	r3, #1
	beq	.L88
.L89:
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L94
	ldr	r3, [r4, #428]
	mov	r2, #284
	ldr	r1, .L106+4
	ldr	r0, .L106+12
	str	r3, [sp, #4]
	ldr	r3, [r4, #404]
	str	r3, [sp]
	ldr	r3, [r4, #440]
	bl	printk
.L94:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L85
	mov	r3, r4
	movw	r2, #285
	ldr	r1, .L106+4
	ldr	r0, .L106+16
	bl	printk
.L85:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L87:
	ldr	r2, [r4, #72]
	cmp	r5, #1
	ldrd	r7, r1, [r4, #368]
	mov	r3, #16
	str	r3, [r4, #444]
	str	r2, [r4, #408]
	lsl	r0, r7, #4
	bhi	.L90
.L105:
	mov	r5, r0
	mov	r1, r0
	ldr	r0, [r4, #436]
	bl	mpp_data_reset_v2
	ldr	r1, [r4, #408]
	add	r3, r5, r5, lsl #2
	ldr	r0, [r4, #400]
	str	r3, [r4, #440]
	muls	r1, r5, r1
	lsrs	r1, r1, #4
	bl	mpp_data_reset_v2
	ldr	r0, [r4, #408]
	muls	r0, r5, r0
	ubfx	r0, r0, #3, #28
	str	r0, [r4, #404]
	b	.L89
.L90:
	subs	r5, r5, #1
	lsls	r1, r1, #4
	add	r3, sp, #12
	orr	r1, r1, r7, lsr #28
	add	r2, r2, r5, lsl #4
	bl	div_s64_rem
	b	.L105
.L88:
	ldr	r3, [r4, #44]
	cmp	r3, #1
	bgt	.L92
	movs	r1, #255
	ldr	r3, .L106+20
	str	r1, [sp]
	ldr	r2, .L106+24
	ldr	r0, .L106+28
	bl	printk
.L92:
	ldr	r7, [r4, #44]
	mov	r3, #320
	mov	r0, r5
	str	r3, [r4, #408]
	movs	r3, #16
	str	r3, [r4, #444]
	mov	r1, r7
	movs	r3, #32
	str	r3, [r4, #432]
	bl	__aeabi_uidiv
	cmp	r7, r5
	it	ls
	addls	r0, r0, #-1
	subs	r2, r5, r0
	ldrd	r5, r3, [r4, #368]
	lsls	r0, r0, #5
	add	r0, r0, #320
	lsls	r1, r3, #4
	add	r2, r0, r2, lsl #4
	add	r3, sp, #12
	lsls	r0, r5, #4
	orr	r1, r1, r5, lsr #28
	bl	div_s64_rem
	mov	r5, r0
	mov	r1, r0
	ldr	r0, [r4, #436]
	bl	mpp_data_reset_v2
	ldr	r1, [r4, #408]
	add	r3, r5, r5, lsl #2
	ldr	r0, [r4, #400]
	str	r3, [r4, #440]
	muls	r1, r5, r1
	lsrs	r1, r1, #4
	bl	mpp_data_reset_v2
	ldr	r1, [r4, #432]
	ldr	r3, [r4, #408]
	ldr	r0, [r4, #424]
	muls	r1, r5, r1
	muls	r3, r5, r3
	lsrs	r1, r1, #4
	ubfx	r3, r3, #3, #28
	str	r3, [r4, #404]
	bl	mpp_data_reset_v2
	ldr	r3, [r4, #432]
	muls	r5, r3, r5
	ubfx	r5, r5, #3, #28
	str	r5, [r4, #428]
	b	.L89
.L107:
	.align	2
.L106:
	.word	rc_debug
	.word	.LANCHOR0+24
	.word	.LC0
	.word	.LC17
	.word	.LC1
	.word	.LANCHOR0+38
	.word	.LC15
	.word	.LC16
	.fnend
	.size	bits_frm_init, .-bits_frm_init
	.align	1
	.global	moving_judge_update
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	moving_judge_update, %function
moving_judge_update:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r3, [r0, #356]
	mov	r5, r1
	cbz	r3, .L109
	cmp	r3, #2
	bne	.L110
	ldr	r1, [r1, #24]
	ldr	r0, [r0, #452]
	bl	mpp_data_update_v2
	ldr	r1, [r5, #28]
	ldr	r0, [r4, #456]
.L111:
	bl	mpp_data_update_v2
.L110:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L109:
	ldr	r1, [r1, #24]
	ldr	r0, [r0, #448]
	bl	mpp_data_update_v2
	ldr	r1, [r5, #36]
	ldr	r0, [r4, #464]
	b	.L111
	.fnend
	.size	moving_judge_update, .-moving_judge_update
	.align	1
	.global	bit_statics_update
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bit_statics_update, %function
bit_statics_update:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, fp, lr}
	mov	r5, r0
	ldr	r6, [r0, #40]
	ldr	r0, [r0, #532]
	bl	mpp_data_update_v2
	ldr	r0, [r5, #448]
	bl	mpp_data_mean_v2
	mov	r4, r0
	ldr	r0, [r5, #452]
	bl	mpp_data_mean_v2
	asr	r9, r4, #31
	mov	r7, r0
	ldr	r0, [r5, #532]
	bl	mpp_data_sum_v2
	subs	r3, r6, #1
	umull	fp, ip, r3, r4
	mla	ip, r3, r9, ip
	asrs	r1, r0, #31
	strd	r0, [r5, #392]
	adds	r0, fp, r7
	adc	r1, ip, r7, asr #31
	strd	r0, [r5, #384]
	pop	{r4, r5, r6, r7, r8, r9, fp, pc}
	.fnend
	.size	bit_statics_update, .-bit_statics_update
	.align	1
	.global	bits_model_update
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_model_update, %function
bits_model_update:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r5, r1
	ldr	r6, .L133
	mov	r4, r0
	mov	r7, r2
	ldr	r3, [r6]
	lsls	r1, r3, #31
	bpl	.L114
	mov	r3, r0
	mov	r2, #328
	ldr	r1, .L133+4
	ldr	r0, .L133+8
	bl	printk
.L114:
	adds	r1, r5, #0
	ldr	r0, [r4, #536]
	it	ne
	movne	r1, #1
	bl	mpp_data_update_v2
	mov	r1, r5
	ldr	r0, [r4, #528]
	bl	mpp_data_update_v2
	ldr	r0, [r4, #528]
	bl	mpp_data_sum_v2
	ldr	r1, [r4, #60]
	bl	__aeabi_idiv
	ldr	r3, [r4, #544]
	ldr	r1, [r4, #540]
	adds	r2, r5, r3
	ldr	r3, [r4, #376]
	cmp	r2, r1
	ite	gt
	subgt	r3, r1, r3
	suble	r3, r2, r3
	bic	r3, r3, r3, asr #31
	str	r3, [r4, #544]
	ldr	r3, [r4, #356]
	cmp	r3, #2
	str	r0, [r4, #492]
	beq	.L117
	cmp	r3, #3
	beq	.L118
	cbnz	r3, .L120
	mov	r1, r5
	ldr	r0, [r4, #436]
	bl	mpp_data_update_v2
	ldr	r0, [r4, #460]
	mov	r1, r7
	bl	mpp_data_update_v2
	ldr	r0, [r4, #436]
	bl	mpp_data_sum_v2
	cbz	r0, .L121
	str	r0, [r4, #440]
.L122:
	movs	r3, #16
	str	r3, [r4, #444]
	b	.L120
.L117:
	mov	r1, r5
	ldr	r0, [r4, #400]
	bl	mpp_data_update_v2
	ldr	r0, [r4, #400]
	bl	mpp_data_sum_v2
	ldr	r7, [r4, #440]
	lsls	r1, r7, #1
	mov	r5, r0
	str	r0, [r4, #404]
	movs	r0, #80
	muls	r0, r5, r0
	bl	__aeabi_uidiv
	ldr	r3, [r6]
	lsls	r2, r3, #25
	str	r0, [r4, #408]
	bpl	.L120
	strd	r7, r0, [sp]
	mov	r3, r5
	mov	r2, #348
	ldr	r1, .L133+4
	ldr	r0, .L133+12
	bl	printk
.L120:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L123
	mov	r3, r4
	movw	r2, #379
	ldr	r1, .L133+4
	ldr	r0, .L133+16
	bl	printk
.L123:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L121:
	movs	r3, #1
	str	r3, [r4, #440]
	b	.L122
.L118:
	mov	r1, r5
	ldr	r0, [r4, #424]
	bl	mpp_data_update_v2
	ldr	r0, [r4, #424]
	bl	mpp_data_sum_v2
	ldr	r1, [r4, #440]
	movs	r3, #80
	lsls	r1, r1, #1
	str	r0, [r4, #428]
	muls	r0, r3, r0
	bl	__aeabi_uidiv
	cmp	r0, #320
	it	ge
	movge	r0, #320
	cmp	r0, #16
	it	lt
	movlt	r0, #16
	str	r0, [r4, #432]
	b	.L120
.L134:
	.align	2
.L133:
	.word	rc_debug
	.word	.LANCHOR0+52
	.word	.LC0
	.word	.LC18
	.word	.LC1
	.fnend
	.size	bits_model_update, .-bits_model_update
	.align	1
	.global	bits_model_alloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_model_alloc, %function
bits_model_alloc:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r3
	ldr	r3, [r0, #64]
	.pad #52
	sub	sp, sp, #52
	mov	r6, r0
	mov	r4, r2
	str	r1, [sp, #36]
	lsls	r3, r3, #4
	str	r3, [sp, #32]
	ldr	r3, [r0, #40]
	ldr	r1, [r0, #440]
	str	r3, [sp, #28]
	ldr	r3, [r0, #432]
	lsls	r1, r1, #1
	str	r3, [sp, #24]
	ldr	r3, [r0, #404]
	movs	r0, #80
	muls	r0, r3, r0
	bl	__aeabi_uidiv
	ldr	r3, .L186
	ldr	r2, [r3]
	mov	r10, r3
	tst	r2, #1
	mov	r8, r0
	str	r0, [r6, #408]
	mov	r7, r0
	beq	.L136
	mov	r3, r6
	movw	r2, #397
	ldr	r1, .L186+4
	ldr	r0, .L186+8
	bl	printk
.L136:
	ldr	r3, [r10]
	lsls	r0, r3, #25
	bpl	.L137
	ldr	r3, [sp, #32]
	movw	r2, #399
	strd	r4, [sp, #8]
	str	r8, [sp, #4]
	str	r3, [sp]
	ldr	r1, .L186+4
	ldr	r3, [r6, #356]
	ldr	r0, .L186+12
	bl	printk
.L137:
	ldr	r2, [r6, #184]
	cbz	r2, .L157
	ldr	r9, [r6, #192]
.L138:
	ldr	r3, [r6, #36]
	cmp	r3, #1
	bne	.L139
	ldr	r3, [r6, #44]
	cmp	r3, #1
	bgt	.L140
	mov	r1, #404
	ldr	r3, .L186+16
	str	r1, [sp]
	ldr	r2, .L186+20
	ldr	r0, .L186+24
	bl	printk
.L140:
	ldr	r1, [r6, #44]
	ldr	r0, [sp, #28]
	bl	__aeabi_idiv
	mov	fp, r0
	cbz	r0, .L141
	add	fp, r0, #-1
.L141:
	ldr	r3, [r6, #356]
	cmp	r3, #2
	beq	.L142
	cmp	r3, #3
	beq	.L143
	cbnz	r3, .L145
	cmp	r8, #15
	ble	.L158
	ldr	r3, [sp, #32]
	cmp	r3, r8
	it	ge
	movge	r3, r8
	mov	r7, r3
.L146:
	lsls	r3, r5, #4
	lsls	r2, r4, #4
	orr	r3, r3, r4, lsr #28
	mov	r4, r2
	mov	r5, r3
	b	.L145
.L157:
	mvn	r9, #-2147483648
	b	.L138
.L142:
	cmp	r8, #16000
	mov	r7, r8
	it	ge
	movge	r7, #16000
	cmp	r7, #16
	it	lt
	movlt	r7, #16
	asrs	r3, r7, #31
	muls	r3, r4, r3
	mla	r3, r7, r5, r3
	umull	r4, r5, r4, r7
	add	r5, r5, r3
	ldr	r3, [r6, #184]
	cbz	r3, .L145
	ldr	r9, [r6, #188]
.L145:
	ldr	r3, [sp, #28]
	mov	r0, r4
	mov	r1, r5
	sub	r2, r3, fp
	ldr	r3, [sp, #24]
	mul	r3, fp, r3
	add	r2, r3, r2, lsl #4
	add	r3, sp, #44
	add	r2, r2, r7
	bl	div_s64_rem
	mov	r8, r0
	cbnz	r0, .L148
	movw	r2, #439
	ldr	r1, .L186+4
	ldr	r0, .L186+28
	bl	printk
	mov	r2, #442
	ldr	r1, .L186+4
	ldr	r0, .L186+32
	ldr	r3, [sp, #24]
	strd	r4, [sp]
	strd	fp, r3, [sp, #16]
	ldr	r3, [sp, #28]
	strd	r7, r3, [sp, #8]
	bl	printk
	ldr	r3, [r6, #40]
	ldr	r1, .L186+4
	ldr	r0, .L186+36
	str	r3, [sp, #20]
	ldr	r3, [r6, #44]
	str	r3, [sp, #16]
	ldr	r3, [r6, #440]
	str	r3, [sp, #12]
	ldr	r3, [r6, #404]
	str	r3, [sp, #8]
	ldrd	r2, [r6, #368]
	strd	r2, [sp]
	mov	r2, #446
	bl	printk
.L148:
	ldr	r3, [r10]
	lsls	r1, r3, #25
	bpl	.L153
	strd	r4, [sp]
	mov	r3, r7
	movw	r2, #475
	ldr	r1, .L186+4
	ldr	r0, .L186+40
	bl	printk
.L153:
	cmp	r8, r9
	ble	.L154
	ldr	r3, [r6, #196]
	cmp	r3, #1
	bne	.L154
	ldr	r3, [r10]
	sub	r8, r9, r9, asr #4
	lsls	r2, r3, #25
	bpl	.L154
	mov	r3, r9
	mov	r2, #480
	ldr	r1, .L186+4
	ldr	r0, .L186+44
	bl	printk
.L154:
	ldr	r3, [sp, #36]
	str	r9, [r6, #524]
	str	r8, [r3]
	cmp	r8, #0
	bne	.L155
	mov	r1, #486
	ldr	r3, .L186+16
	str	r1, [sp]
	ldr	r2, .L186+48
	ldr	r0, .L186+24
	bl	printk
.L155:
	ldr	r3, [r10]
	lsls	r3, r3, #31
	bpl	.L156
	mov	r3, r6
	mov	r2, #488
	ldr	r1, .L186+4
	ldr	r0, .L186+52
	bl	printk
.L156:
	movs	r0, #0
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L158:
	movs	r7, #16
	b	.L146
.L143:
	cmp	r8, #15
	ble	.L159
	ldr	r3, [sp, #32]
	cmp	r3, r8
	it	ge
	movge	r3, r8
	mov	r7, r3
.L147:
	ldr	r3, [sp, #24]
	asrs	r2, r3, #31
	mov	r3, r2
	ldr	r2, [sp, #24]
	muls	r3, r4, r3
	mla	r3, r2, r5, r3
	umull	r4, r5, r4, r2
	add	r5, r5, r3
	b	.L145
.L159:
	movs	r7, #16
	b	.L147
.L139:
	ldr	r3, [r6, #356]
	cbz	r3, .L149
	cmp	r3, #2
	bne	.L150
	cmp	r8, #16000
	mov	r7, r8
	it	ge
	movge	r7, #16000
	cmp	r7, #16
	it	lt
	movlt	r7, #16
	asrs	r1, r7, #31
	mul	r3, r4, r1
	mla	r3, r7, r5, r3
	umull	r4, r5, r4, r7
	add	r5, r5, r3
	cbz	r2, .L150
	ldr	r9, [r6, #188]
.L150:
	ldr	r3, [sp, #28]
	mov	r0, r4
	mov	r1, r5
	cmp	r3, #1
	itete	gt
	addgt	r2, r3, #-1
	addle	r3, sp, #44
	addgt	r3, sp, #44
	movle	r2, r7
	it	gt
	addgt	r2, r7, r2, lsl #4
	bl	div_s64_rem
	mov	r8, r0
	b	.L148
.L149:
	cmp	r8, #15
	ble	.L160
	ldr	r3, [sp, #32]
	cmp	r3, r8
	it	ge
	movge	r3, r8
	mov	r7, r3
.L151:
	lsls	r3, r5, #4
	lsls	r2, r4, #4
	orr	r3, r3, r4, lsr #28
	mov	r4, r2
	mov	r5, r3
	b	.L150
.L160:
	movs	r7, #16
	b	.L151
.L187:
	.align	2
.L186:
	.word	rc_debug
	.word	.LANCHOR0+70
	.word	.LC0
	.word	.LC19
	.word	.LANCHOR0+87
	.word	.LC15
	.word	.LC16
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC1
	.fnend
	.size	bits_model_alloc, .-bits_model_alloc
	.align	1
	.global	calc_cbr_ratio
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	calc_cbr_ratio, %function
calc_cbr_ratio:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r0
	ldr	r6, .L217
	.pad #28
	sub	sp, sp, #28
	mov	fp, r1
	ldr	r8, [r0, #476]
	ldr	r7, [r0, #492]
	ldr	r3, [r6]
	ldrd	r4, r9, [r0, #480]
	lsls	r2, r3, #31
	ldr	r10, [r0, #496]
	bpl	.L189
	mov	r3, r0
	mov	r2, #576
	ldr	r1, .L217+4
	ldr	r0, .L217+8
	bl	printk
.L189:
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L191
	ldr	r3, .L217+12
	movw	r2, #579
	ldr	r1, .L217+4
	ldr	r0, .L217+16
	str	r3, [sp, #16]
	ldr	r3, .L217+20
	str	r3, [sp, #12]
	ldr	r3, .L217+24
	str	r3, [sp, #8]
	ldr	r3, .L217+28
	str	r3, [sp, #4]
	ldr	r3, .L217+32
	str	r3, [sp]
	ldr	r3, .L217+36
	bl	printk
	ldr	r3, [r6]
	lsls	r0, r3, #26
	bpl	.L191
	ldr	r3, [r5, #544]
	movw	r2, #582
	strd	r7, r10, [sp, #4]
	str	r4, [sp]
	asrs	r3, r3, #10
	ldr	r1, .L217+4
	strd	r8, r3, [sp, #12]
	mov	r3, r9
	ldr	r0, .L217+40
	bl	printk
.L191:
	ldrd	r2, [r5, #368]
	mov	r1, fp
	mov	r0, r5
	bl	bits_model_alloc
	cmp	r8, #0
	bgt	.L193
	movw	r1, #586
	ldr	r3, .L217+44
	str	r1, [sp]
	ldr	r2, .L217+48
	ldr	r0, .L217+52
	bl	printk
.L193:
	cmp	r4, r9
	sub	r0, r9, r4
	mov	r1, r4
	ldr	fp, .L217+92
	itet	gt
	movgt	r3, #52
	lslle	r0, r0, #6
	mulgt	r0, r3, r0
	bl	__aeabi_idiv
	mov	r1, r8
	mov	r9, r0
	lsls	r0, r7, #5
	bl	__aeabi_idiv
	mov	r1, r8
	usat	r4, #6, r0
	lsl	r0, r10, #5
	bl	__aeabi_idiv
	cmp	r7, r10
	ldr	r4, [fp, r4, lsl #2]
	asr	r1, r8, #4
	usat	r0, #6, r0
	ldr	r3, [fp, r0, lsl #2]
	sub	r4, r4, r3
	ble	.L196
	sub	r3, r8, r10
	cmp	r3, r1
	bge	.L196
	movs	r3, #6
	muls	r4, r3, r4
.L197:
	cmp	r9, #256
	mvn	r3, #127
	sub	r0, r7, r8
	it	ge
	movge	r9, #256
	cmp	r9, r3
	add	r0, r0, r0, lsl #1
	ldr	r10, [r5, #548]
	it	lt
	movlt	r9, r3
	cmp	r4, #256
	it	ge
	movge	r4, #256
	cmp	r4, r3
	it	lt
	movlt	r4, r3
	bl	__aeabi_idiv
	mvn	r3, #31
	mov	r1, r10
	cmp	r0, #32
	it	ge
	movge	r0, #32
	cmp	r0, r3
	mov	r7, r0
	ldr	r0, [r5, #544]
	it	lt
	movlt	r7, r3
	movs	r3, #12
	sub	r0, r0, r10
	muls	r0, r3, r0
	bl	__aeabi_idiv
	mvn	r3, #15
	cmp	r0, #32
	it	ge
	movge	r0, #32
	cmp	r0, r3
	mov	r8, r0
	it	lt
	movlt	r8, r3
	add	r3, r8, r7
	add	r3, r3, r9
	add	r3, r3, r4
	str	r3, [r5, #556]
	ldr	r3, [r6]
	lsls	r1, r3, #24
	bpl	.L199
	ldr	r3, .L217+56
	movw	r2, #627
	ldr	r1, .L217+4
	ldr	r0, .L217+60
	str	r3, [sp, #16]
	ldr	r3, .L217+64
	str	r3, [sp, #12]
	ldr	r3, .L217+68
	str	r3, [sp, #8]
	ldr	r3, .L217+72
	str	r3, [sp, #4]
	ldr	r3, .L217+76
	str	r3, [sp]
	ldr	r3, .L217+80
	bl	printk
	ldr	r3, [r6]
	lsls	r2, r3, #24
	bpl	.L199
	ldr	r3, [r5, #580]
	sub	r4, r4, r9
	str	r7, [sp, #4]
	movw	r2, #630
	str	r4, [sp]
	str	r3, [sp, #16]
	ldr	r3, [r5, #556]
	ldr	r1, .L217+4
	ldr	r0, .L217+84
	strd	r8, r3, [sp, #8]
	mov	r3, r9
	bl	printk
.L199:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L201
	mov	r3, r5
	mov	r2, #632
	ldr	r1, .L217+4
	ldr	r0, .L217+88
	bl	printk
.L201:
	movs	r0, #0
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L196:
	cmp	r7, r10
	bge	.L203
	sub	r10, r10, r8
	cmp	r10, r1
	bge	.L203
	lsls	r4, r4, #2
	b	.L197
.L203:
	movs	r4, #0
	b	.L197
.L218:
	.align	2
.L217:
	.word	rc_debug
	.word	.LANCHOR0+104
	.word	.LC0
	.word	.LC28
	.word	.LC27
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC26
	.word	.LC33
	.word	.LANCHOR0+119
	.word	.LC34
	.word	.LC16
	.word	.LC37
	.word	.LC36
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC35
	.word	.LC42
	.word	.LC1
	.word	.LANCHOR1
	.fnend
	.size	calc_cbr_ratio, .-calc_cbr_ratio
	.align	1
	.global	calc_vbr_ratio
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	calc_vbr_ratio, %function
calc_vbr_ratio:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #44
	sub	sp, sp, #44
	ldr	r3, [r0, #56]
	mov	r6, r0
	mov	r4, r1
	ldr	r10, [r0, #476]
	ldr	r8, [r0, #492]
	str	r3, [sp, #28]
	ldrd	r7, r3, [r0, #480]
	ldr	r9, [r0, #496]
	str	r3, [sp, #32]
	ldr	r3, .L257
	ldr	r2, [r3]
	mov	fp, r3
	tst	r2, #1
	beq	.L220
	mov	r3, r0
	movw	r2, #785
	ldr	r1, .L257+4
	ldr	r0, .L257+8
	bl	printk
.L220:
	ldrd	r2, [r6, #368]
	mov	r1, r4
	mov	r0, r6
	bl	bits_model_alloc
	mov	r1, r7
	ldr	r3, [sp, #32]
	cmp	r7, r3
	sub	r0, r3, r7
	ldr	r3, [r6, #584]
	ble	.L221
	cmp	r3, #36
	ble	.L222
	lsls	r0, r0, #7
.L255:
	bl	__aeabi_idiv
	ldr	r3, [sp, #28]
	mov	r5, r0
	mov	r0, r8
	asrs	r4, r3, #5
	mov	r1, r4
	bl	__aeabi_idiv
	mov	r1, r4
	usat	r2, #6, r0
	mov	r0, r9
	str	r2, [sp, #36]
	bl	__aeabi_idiv
	ldr	r3, .L257+12
	ldr	r2, [sp, #36]
	usat	r0, #6, r0
	ldr	r4, [r3, r2, lsl #2]
	ldr	r3, [r3, r0, lsl #2]
	subs	r4, r4, r3
	ldr	r3, [fp]
	lsls	r3, r3, #26
	bpl	.L231
	ldr	r3, .L257+16
	movw	r2, #827
	ldr	r1, .L257+4
	ldr	r0, .L257+20
	str	r3, [sp, #16]
	ldr	r3, .L257+24
	str	r3, [sp, #12]
	ldr	r3, .L257+28
	str	r3, [sp, #8]
	ldr	r3, .L257+32
	str	r3, [sp, #4]
	ldr	r3, .L257+36
	str	r3, [sp]
	ldr	r3, .L257+40
	bl	printk
	ldr	r3, [fp]
	lsls	r0, r3, #26
	bpl	.L231
	ldr	r3, [sp, #28]
	movw	r2, #830
	strd	r8, r9, [sp, #4]
	str	r7, [sp]
	strd	r10, r3, [sp, #12]
	ldr	r1, .L257+4
	ldr	r3, [sp, #32]
	ldr	r0, .L257+44
	bl	printk
.L231:
	cmp	r10, r8
	bge	.L233
	cmp	r8, r9
	ble	.L234
	movs	r3, #6
	muls	r4, r3, r4
.L235:
	cmp	r5, #256
	mvn	r3, #127
	sub	r8, r8, r10
	it	ge
	movge	r5, #256
	cmp	r5, r3
	mov	r0, #6
	it	lt
	movlt	r5, r3
	cmp	r4, #96
	mvn	r3, #63
	it	ge
	movge	r4, #96
	cmp	r4, r3
	mul	r0, r0, r8
	it	lt
	movlt	r4, r3
	ldr	r3, [sp, #28]
	asrs	r1, r3, #4
	bl	__aeabi_idiv
	mvn	r3, #47
	cmp	r0, #96
	it	ge
	movge	r0, #96
	cmp	r0, r3
	mov	r8, r0
	it	lt
	movlt	r8, r3
	ldr	r3, [r6, #408]
	cmp	r3, #640
	bls	.L236
	cmp	r5, #32
	mvn	r3, #15
	it	ge
	movge	r5, #32
	cmp	r5, r3
	it	lt
	movlt	r5, r3
	cmp	r4, #32
	it	ge
	movge	r4, #32
	cmp	r4, r3
	it	lt
	movlt	r4, r3
.L236:
	ldr	r7, .L257
	adds	r3, r5, r4
	add	r3, r3, r8
	str	r3, [r6, #556]
	ldr	r3, [r7]
	lsls	r1, r3, #24
	bpl	.L238
	ldr	r3, .L257+48
	movw	r2, #853
	ldr	r1, .L257+4
	ldr	r0, .L257+52
	str	r3, [sp, #12]
	ldr	r3, .L257+56
	str	r3, [sp, #8]
	ldr	r3, .L257+60
	str	r3, [sp, #4]
	ldr	r3, .L257+64
	str	r3, [sp]
	ldr	r3, .L257+68
	bl	printk
	ldr	r3, [r7]
	lsls	r2, r3, #24
	bpl	.L238
	ldr	r3, [r6, #580]
	movw	r2, #855
	str	r4, [sp]
	ldr	r1, .L257+4
	str	r3, [sp, #12]
	ldr	r3, [r6, #556]
	ldr	r0, .L257+72
	strd	r8, r3, [sp, #4]
	mov	r3, r5
	bl	printk
.L238:
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L240
	mov	r3, r6
	mov	r2, #856
	ldr	r1, .L257+4
	ldr	r0, .L257+76
	bl	printk
.L240:
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L222:
	cmp	r3, #34
	ble	.L224
	movs	r3, #80
.L256:
	muls	r0, r3, r0
	b	.L255
.L224:
	cmp	r3, #32
	ble	.L225
.L229:
	lsls	r0, r0, #5
	b	.L255
.L225:
	lsls	r0, r0, #4
	b	.L255
.L221:
	cmp	r3, #29
	bgt	.L226
	movs	r3, #192
	b	.L256
.L226:
	cmp	r3, #31
	bgt	.L227
	movs	r3, #144
	b	.L256
.L227:
	cmp	r3, #33
	bgt	.L228
	movs	r3, #96
	b	.L256
.L228:
	cmp	r3, #36
	bgt	.L229
	lsls	r0, r0, #6
	b	.L255
.L233:
	cmp	r8, r9
	bgt	.L242
.L234:
	cmp	r10, r9
	ble	.L242
	add	r4, r4, r4, lsl #1
	b	.L235
.L242:
	movs	r4, #0
	b	.L235
.L258:
	.align	2
.L257:
	.word	rc_debug
	.word	.LANCHOR0+134
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC43
	.word	.LC36
	.word	.LC44
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC26
	.word	.LC45
	.word	.LC37
	.word	.LC46
	.word	.LC38
	.word	.LC40
	.word	.LC41
	.word	.LC35
	.word	.LC47
	.word	.LC1
	.fnend
	.size	calc_vbr_ratio, .-calc_vbr_ratio
	.align	1
	.global	calc_next_i_ratio
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	calc_next_i_ratio, %function
calc_next_i_ratio:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r10, fp, lr}
	.pad #8
	mov	r4, r0
	ldr	r6, .L279
	ldr	r5, [r0, #64]
	ldr	r8, [r0, #40]
	ldr	r3, [r6]
	lsls	r5, r5, #4
	ldr	r7, [r0, #560]
	lsls	r1, r3, #31
	bpl	.L260
	mov	r3, r0
	mov	r2, #500
	ldr	r1, .L279+4
	ldr	r0, .L279+8
	bl	printk
.L260:
	ldrd	r0, r3, [r4, #368]
	asr	fp, r5, #31
	cmp	r8, #1
	it	gt
	addgt	r2, r8, #-1
	mul	r3, r5, r3
	it	gt
	addgt	r2, r5, r2, lsl #4
	mla	r3, r0, fp, r3
	it	le
	movle	r2, r5
	umull	r0, r1, r0, r5
	add	r1, r1, r3
	ite	gt
	addgt	r3, sp, #4
	addle	r3, sp, #4
	bl	div_s64_rem
	ldr	r3, [r4, #484]
	ldr	r5, [r4, #552]
	cmp	r3, r0
	bgt	.L263
	cbz	r5, .L264
.L263:
	subs	r3, r3, r0
	mov	r1, r0
	lsls	r0, r3, #8
	bl	__aeabi_idiv
	mvn	r3, #255
	cmp	r0, #256
	it	ge
	movge	r0, #256
	cmp	r0, r3
	ite	ge
	addge	r3, r5, r0
	addlt	r3, r5, r3
	cmp	r3, #0
	blt	.L268
	ldr	r2, .L279+12
	add	r7, r2, r7, lsl #2
	ldr	r0, [r7, #168]
	cmp	r3, r0
	it	ge
	movge	r3, r0
.L265:
	ldr	r2, [r6]
	str	r3, [r4, #552]
	lsls	r2, r2, #25
	bpl	.L264
	movw	r2, #523
	ldr	r1, .L279+4
	ldr	r0, .L279+16
	bl	printk
.L264:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L267
	mov	r3, r4
	movw	r2, #526
	ldr	r1, .L279+4
	ldr	r0, .L279+20
	bl	printk
.L267:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L268:
	movs	r3, #0
	b	.L265
.L280:
	.align	2
.L279:
	.word	rc_debug
	.word	.LANCHOR0+149
	.word	.LC0
	.word	.LANCHOR0
	.word	.LC48
	.word	.LC1
	.fnend
	.size	calc_next_i_ratio, .-calc_next_i_ratio
	.align	1
	.global	calc_debreath_qp
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	calc_debreath_qp, %function
calc_debreath_qp:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r4, r0
	ldr	r5, [r0, #604]
	ldr	r7, .L296
	asrs	r5, r5, #5
	cmp	r5, #7
	ldr	r3, [r7]
	it	ge
	movge	r5, #7
	lsls	r1, r3, #31
	bpl	.L282
	mov	r3, r0
	movw	r2, #545
	ldr	r1, .L296+4
	ldr	r0, .L296+8
	bl	printk
.L282:
	ldrd	r0, r1, [r4, #596]
	bl	__aeabi_idiv
	ldr	r3, [r7]
	cmp	r0, #35
	it	ge
	movge	r0, #35
	lsls	r2, r3, #24
	mov	r6, r0
	bpl	.L283
	ldr	r3, [r4, #604]
	movw	r2, #550
	ldr	r1, .L296+4
	strd	r0, r3, [sp]
	ldr	r3, [r4, #584]
	ldr	r0, .L296+12
	bl	printk
.L283:
	ldr	r2, [r4, #204]
	ldr	r3, .L296+16
	add	r3, r3, r2
	ldr	r2, .L296+20
	uxtab	r5, r2, r5
	ldrsb	r3, [r3, #389]
	ldrsb	r2, [r5, #256]
	subs	r1, r3, r2
	cmp	r6, r1
	itt	gt
	subgt	r6, r6, r3
	addgt	r6, r6, r2
	ldrd	r2, r3, [r4, #104]
	cmp	r3, r6
	bgt	.L285
	cmp	r2, r6
	mov	r3, r2
	it	ge
	movge	r3, r6
.L285:
	str	r3, [r4, #584]
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L286
	mov	r3, r4
	mov	r2, #560
	ldr	r1, .L296+4
	ldr	r0, .L296+24
	bl	printk
.L286:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L297:
	.align	2
.L296:
	.word	rc_debug
	.word	.LANCHOR0+372
	.word	.LC0
	.word	.LC49
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	.LC1
	.fnend
	.size	calc_debreath_qp, .-calc_debreath_qp
	.align	1
	.global	reenc_calc_super_frm_ratio
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	reenc_calc_super_frm_ratio, %function
reenc_calc_super_frm_ratio:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L307
	mov	r6, r1
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L299
	mov	r3, r0
	mov	r2, #640
	ldr	r1, .L307+4
	ldr	r0, .L307+8
	bl	printk
.L299:
	ldr	r3, [r4, #524]
	ldr	r0, [r6, #24]
	ldr	r1, [r6]
	subs	r0, r0, r3
	lsls	r0, r0, #2
	bl	__aeabi_idiv
	movs	r3, #160
	muls	r0, r3, r0
	ldr	r3, [r5]
	cmp	r0, #640
	it	ge
	movge	r0, #640
	cmp	r0, #128
	it	lt
	movlt	r0, #128
	lsls	r3, r3, #31
	str	r0, [r4, #556]
	bpl	.L300
	mov	r3, r4
	mov	r2, #644
	ldr	r1, .L307+4
	ldr	r0, .L307+12
	bl	printk
.L300:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L308:
	.align	2
.L307:
	.word	rc_debug
	.word	.LANCHOR0+425
	.word	.LC0
	.word	.LC1
	.fnend
	.size	reenc_calc_super_frm_ratio, .-reenc_calc_super_frm_ratio
	.align	1
	.global	reenc_calc_cbr_ratio
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	reenc_calc_cbr_ratio, %function
reenc_calc_cbr_ratio:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r6, [r0, #60]
	.pad #28
	sub	sp, sp, #28
	ldr	r0, [r0, #528]
	mov	r8, r1
	bl	mpp_data_sum_v2
	mov	r1, r6
	bl	__aeabi_idiv
	mov	r1, #-1
	mul	r7, r0, r6
	mov	r10, r0
	ldr	r0, [r4, #528]
	bl	mpp_data_get_pre_val_v2
	ldr	r5, [r8, #24]
	mov	r1, r6
	ldr	r6, .L331
	subs	r0, r5, r0
	add	r0, r0, r7
	bl	__aeabi_idiv
	ldr	r3, [r6]
	ldr	r7, [r8]
	lsls	r1, r3, #31
	ldr	r9, [r4, #476]
	mov	fp, r0
	bpl	.L310
	mov	r3, r4
	movw	r2, #667
	ldr	r1, .L331+4
	ldr	r0, .L331+8
	bl	printk
.L310:
	ldr	r2, [r4, #524]
	ldr	r3, [r8, #24]
	cmp	r2, r3
	bgt	.L311
	ldr	r3, [r4, #196]
	cmp	r3, #1
	bne	.L311
	mov	r1, r8
	mov	r0, r4
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	reenc_calc_super_frm_ratio
.L311:
	ldr	r0, [r4, #544]
	ldr	r2, [r4, #540]
	adds	r3, r5, r0
	ldr	r0, [r4, #376]
	cmp	r3, r2
	ite	gt
	subgt	r0, r2, r0
	suble	r0, r3, r0
	cmp	r5, r7
	bic	r3, r0, r0, asr #31
	sub	r0, r5, r7
	ite	lt
	movlt	r1, r5
	movge	r1, r7
	lsls	r0, r0, #5
	str	r3, [sp, #12]
	bl	__aeabi_idiv
	asr	r3, r9, #5
	mov	r1, r3
	mov	r8, r0
	mov	r0, fp
	str	r3, [sp, #20]
	bl	__aeabi_idiv
	ldr	r3, [sp, #20]
	usat	r2, #6, r0
	mov	r0, r10
	str	r2, [sp, #16]
	mov	r1, r3
	bl	__aeabi_idiv
	ldr	r3, .L331+12
	mov	r1, r9
	ldr	r2, [sp, #16]
	usat	r0, #6, r0
	ldr	r2, [r3, r2, lsl #2]
	ldr	r3, [r3, r0, lsl #2]
	movs	r0, #96
	subs	r3, r2, r3
	sub	r2, fp, r9
	str	r3, [sp, #20]
	muls	r0, r2, r0
	bl	__aeabi_idiv
	ldr	r1, [r4, #548]
	ldr	r3, [sp, #12]
	str	r0, [sp, #16]
	subs	r0, r3, r1
	lsls	r0, r0, #5
	bl	__aeabi_idiv
	cmp	r10, fp
	ldrd	r2, r3, [sp, #16]
	bge	.L316
	cmp	r10, r9
	beq	.L316
	movs	r1, #6
	muls	r3, r1, r3
.L330:
	cmp	r3, #256
	mvn	r1, #191
	it	ge
	movge	r3, #256
	cmp	r3, r1
	it	lt
	movlt	r3, r1
.L317:
	cmp	r0, #32
	mvn	r1, #31
	it	ge
	movge	r0, #32
	cmp	r2, #32
	it	ge
	movge	r2, #32
	cmp	r2, r1
	it	lt
	movlt	r2, r1
	cmp	r0, r1
	ite	ge
	addge	r2, r2, r0
	addlt	r2, r2, r1
	cmp	r8, #256
	mvn	r0, #127
	it	ge
	movge	r8, #256
	cmp	r8, r0
	ite	ge
	addge	r8, r2, r8
	addlt	r8, r2, r0
	ldr	r2, [r6]
	add	r3, r3, r8
	str	r3, [r4, #556]
	lsls	r2, r2, #25
	bpl	.L318
	strd	r5, r3, [sp]
	movw	r2, #722
	mov	r3, r7
	ldr	r1, .L331+4
	ldr	r0, .L331+16
	bl	printk
.L318:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L309
	mov	r3, r4
	movw	r2, #723
	ldr	r1, .L331+4
	ldr	r0, .L331+20
	bl	printk
.L309:
	movs	r0, #0
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L316:
	ldr	r1, [r4, #356]
	cmp	r1, #2
	bne	.L320
	add	r3, r3, r3, lsl #1
	b	.L330
.L320:
	movs	r3, #0
	b	.L317
.L332:
	.align	2
.L331:
	.word	rc_debug
	.word	.LANCHOR0+452
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC50
	.word	.LC1
	.fnend
	.size	reenc_calc_cbr_ratio, .-reenc_calc_cbr_ratio
	.align	1
	.global	reenc_calc_vbr_ratio
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	reenc_calc_vbr_ratio, %function
reenc_calc_vbr_ratio:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r4, r0
	ldr	r5, [r0, #60]
	mov	r8, r1
	ldr	r0, [r0, #528]
	bl	mpp_data_sum_v2
	mov	r1, r5
	bl	__aeabi_idiv
	mov	r1, #-1
	mul	r6, r0, r5
	mov	r10, r0
	ldr	r0, [r4, #528]
	bl	mpp_data_get_pre_val_v2
	ldr	r7, [r8, #24]
	mov	r1, r5
	subs	r0, r7, r0
	add	r0, r0, r6
	ldr	r6, .L352
	bl	__aeabi_idiv
	ldr	r3, [r4, #56]
	ldr	r9, [r4, #476]
	ldr	fp, [r8]
	str	r3, [sp, #4]
	mov	r5, r0
	ldr	r3, [r6]
	lsls	r1, r3, #31
	bpl	.L334
	mov	r3, r4
	movw	r2, #879
	ldr	r1, .L352+4
	ldr	r0, .L352+8
	bl	printk
.L334:
	ldr	r2, [r4, #524]
	ldr	r3, [r8, #24]
	cmp	r2, r3
	bgt	.L335
	ldr	r3, [r4, #196]
	cmp	r3, #1
	bne	.L335
	mov	r1, r8
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	reenc_calc_super_frm_ratio
.L335:
	cmp	r7, fp
	sub	r0, r7, fp
	ite	lt
	movlt	r1, r7
	movge	r1, fp
	lsls	r0, r0, #5
	bl	__aeabi_idiv
	cmp	r10, r5
	mov	r7, r0
	bge	.L342
	cmp	r5, r9
	ble	.L342
	ldr	r3, [sp, #4]
	mov	r0, r5
	ldr	fp, .L352+20
	asr	r8, r3, #5
	mov	r1, r8
	bl	__aeabi_idiv
	mov	r1, r8
	mov	r8, #6
	usat	r3, #6, r0
	mov	r0, r10
	str	r3, [sp, #4]
	bl	__aeabi_idiv
	ldr	r3, [sp, #4]
	usat	r0, #6, r0
	ldr	r2, [fp, r0, lsl #2]
	ldr	r3, [fp, r3, lsl #2]
	subs	r3, r3, r2
	mul	r8, r8, r3
	mvn	r3, #191
	cmp	r8, #256
	it	ge
	movge	r8, #256
	cmp	r8, r3
	it	lt
	movlt	r8, r3
.L338:
	sub	r5, r5, r9
	movs	r0, #96
	mov	r1, r9
	muls	r0, r5, r0
	bl	__aeabi_idiv
	mov	r2, r7
	mvn	r3, #127
	cmp	r0, #32
	it	ge
	movge	r0, #32
	cmp	r7, #256
	it	ge
	movge	r2, #256
	cmp	r2, r3
	it	lt
	movlt	r2, r3
	mvn	r3, #31
	cmp	r0, r3
	ite	ge
	addge	r2, r2, r0
	addlt	r2, r2, r3
	add	r3, r2, r8
	ldr	r2, [r6]
	str	r3, [r4, #556]
	lsls	r2, r2, #25
	bpl	.L339
	movw	r2, #905
	ldr	r1, .L352+4
	ldr	r0, .L352+12
	bl	printk
.L339:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L333
	mov	r3, r4
	movw	r2, #906
	ldr	r1, .L352+4
	ldr	r0, .L352+16
	bl	printk
.L333:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L342:
	mov	r8, #0
	b	.L338
.L353:
	.align	2
.L352:
	.word	rc_debug
	.word	.LANCHOR0+473
	.word	.LC0
	.word	.LC51
	.word	.LC1
	.word	.LANCHOR1
	.fnend
	.size	reenc_calc_vbr_ratio, .-reenc_calc_vbr_ratio
	.align	1
	.global	rc_hier_calc_dealt_qp
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_hier_calc_dealt_qp, %function
rc_hier_calc_dealt_qp:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #208]
	cbz	r3, .L354
	ldr	r3, [r0, #356]
	subs	r3, r3, #2
	cmp	r3, #1
	bhi	.L356
.L360:
	movs	r3, #0
	strd	r3, r3, [r0, #636]
	strd	r3, r3, [r0, #628]
.L364:
	str	r3, [r0, #624]
.L354:
	bx	lr
.L356:
	ldr	r3, [r0, #640]
	ldr	r2, [r0, #240]
	cmp	r3, r2
	bge	.L357
	adds	r3, r3, #1
	str	r3, [r0, #640]
	movs	r3, #4
	b	.L364
.L357:
	ldr	r3, [r0, #636]
	ldr	r2, [r0, #236]
	cmp	r3, r2
	bge	.L358
	adds	r3, r3, #1
	str	r3, [r0, #636]
	movs	r3, #0
	str	r3, [r0, #640]
	movs	r3, #3
	b	.L364
.L358:
	ldr	r3, [r0, #632]
	ldr	r2, [r0, #232]
	cmp	r3, r2
	bge	.L359
	adds	r3, r3, #1
	str	r3, [r0, #632]
	movs	r3, #0
	strd	r3, r3, [r0, #636]
	movs	r3, #2
	b	.L364
.L359:
	ldr	r3, [r0, #628]
	ldr	r2, [r0, #228]
	cmp	r3, r2
	bge	.L360
	adds	r3, r3, #1
	str	r3, [r0, #628]
	movs	r3, #0
	strd	r3, r3, [r0, #636]
	str	r3, [r0, #632]
	movs	r3, #1
	b	.L364
	.fnend
	.size	rc_hier_calc_dealt_qp, .-rc_hier_calc_dealt_qp
	.align	1
	.global	moving_ratio_calc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	moving_ratio_calc, %function
moving_ratio_calc:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	movs	r5, #0
	ldr	r9, .L408+48
	.pad #28
	sub	sp, sp, #28
	ldr	r3, [r0, #500]
	mov	r4, r0
	ldr	fp, .L408+20
	mov	r7, r5
	mov	r6, r9
	str	r3, [sp, #20]
.L370:
	mov	r1, r7
	ldr	r0, [r4, #452]
	bl	mpp_data_get_pre_val_v2
	mov	r1, r7
	mov	r10, r0
	ldr	r0, [r4, #456]
	bl	mpp_data_get_pre_val_v2
	adds	r1, r0, #1
	mov	r3, r0
	beq	.L381
	ldr	r2, [r4, #516]
	rsb	r2, r2, #8
	adds	r2, r2, r0
	bmi	.L382
	cmp	r2, #15
	it	ge
	movge	r2, #15
	add	r2, fp, r2, lsl #2
	ldr	r8, [r2, #264]
.L366:
	mul	r2, r10, r8
	add	r5, r5, r2, asr #5
	ldr	r2, [r9]
	lsls	r2, r2, #25
	bpl	.L368
	ldr	r2, [r4, #516]
	ldr	r1, .L408
	ldr	r0, .L408+4
	str	r2, [sp]
	movw	r2, #937
	bl	printk
	ldr	r3, [r9]
	lsls	r0, r3, #25
	bpl	.L368
	str	r10, [sp]
	mov	r3, r8
	movw	r2, #938
	ldr	r1, .L408
	ldr	r0, .L408+8
	bl	printk
.L368:
	cmp	r7, #0
	beq	.L383
	ldr	r0, [r4, #448]
	bl	mpp_data_sum_v2
	mov	r8, r0
	ldr	r0, [r4, #460]
	bl	mpp_data_sum_v2
	mov	r9, r0
	ldr	r0, [r4, #464]
	bl	mpp_data_sum_v2
	ldr	r3, [r6]
	lsls	r1, r3, #25
	mov	fp, r0
	bpl	.L371
	strd	r9, r0, [sp]
	mov	r3, r8
	movw	r2, #945
	ldr	r1, .L408
	ldr	r0, .L408+12
	bl	printk
.L371:
	ldr	r7, .L408+16
	cmp	r8, #0
	beq	.L385
	cmp	r5, #0
	beq	.L385
	mov	r1, r8
	lsls	r0, r5, #6
	bl	__aeabi_idiv
	ldr	r3, .L408+20
	asrs	r0, r0, #4
	cmp	r0, #99
	it	ge
	movge	r0, #99
	cmp	r0, #1
	it	lt
	movlt	r0, #1
	add	r0, r3, r0, lsl #2
	ldr	r2, [r0, #328]
	lsls	r2, r2, #8
	umull	r2, r3, r2, r7
	lsr	r10, r3, #5
.L372:
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L373
	ldr	r3, [r4, #504]
	movw	r2, #955
	ldr	r1, .L408
	ldr	r0, .L408+24
	str	r3, [sp]
	mov	r3, r10
	bl	printk
.L373:
	ldr	r3, [r4, #504]
	movs	r1, #70
	sub	r5, r3, #30
	rsb	r8, r3, #100
	lsls	r5, r5, #8
	mla	r0, r8, r10, r5
	bl	__aeabi_idiv
	ldr	r3, [r6]
	lsls	r3, r3, #25
	mov	r10, r0
	bpl	.L374
	str	r8, [sp]
	mov	r3, r5
	mov	r2, #960
	ldr	r1, .L408
	ldr	r0, .L408+28
	bl	printk
.L374:
	ldr	r3, [r4, #592]
	movs	r4, #100
	muls	r4, r3, r4
	asrs	r4, r4, #8
	mla	r4, r8, r4, r5
	add	r4, r4, #7680
	smull	r2, r3, r4, r7
	asrs	r4, r4, #31
	rsb	r4, r4, r3, asr #5
	ldr	r3, [r6]
	lsls	r0, r3, #25
	bpl	.L375
	mov	r3, r4
	movw	r2, #963
	ldr	r1, .L408
	ldr	r0, .L408+32
	bl	printk
.L375:
	cmp	r9, #0
	beq	.L386
	movs	r0, #20
	mov	r1, r9
	mul	r0, r0, fp
	bl	__aeabi_idiv
	ldr	r3, [r6]
	cmp	r0, #100
	it	ge
	movge	r0, #100
	cmp	r0, #5
	mov	r9, r0
	it	lt
	movlt	r9, #5
	lsls	r1, r3, #25
	bpl	.L377
	mov	r3, r9
	mov	r2, #968
	ldr	r1, .L408
	ldr	r0, .L408+36
	bl	printk
.L377:
	lsl	r0, r9, #8
	umull	r0, r1, r0, r7
	lsrs	r3, r1, #5
.L376:
	add	r5, r5, #7680
	mla	r8, r8, r3, r5
	smull	r2, r3, r8, r7
	asr	r8, r8, #31
	ldr	r2, [sp, #20]
	muls	r2, r4, r2
	rsb	r5, r8, r3, asr #5
	ldr	r3, [sp, #20]
	rsb	r3, r3, #100
	mla	r3, r5, r3, r2
	smull	r0, r1, r3, r7
	asrs	r7, r3, #31
	add	r3, r10, #1
	rsb	r7, r7, r1, asr #5
	add	r7, r7, r3
	ldr	r3, [r6]
	lsls	r2, r3, #25
	asr	r7, r7, #1
	bpl	.L365
	ldr	r3, [sp, #20]
	movw	r2, #978
	ldr	r1, .L408
	ldr	r0, .L408+40
	str	r3, [sp]
	mov	r3, r7
	bl	printk
	ldr	r3, [r6]
	lsls	r3, r3, #25
	bpl	.L365
	strd	r4, r7, [sp, #4]
	mov	r3, r10
	str	r5, [sp]
	mov	r2, #980
	ldr	r1, .L408
	ldr	r0, .L408+44
	bl	printk
.L365:
	mov	r0, r7
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L383:
	movs	r7, #1
	b	.L370
.L381:
	mov	r8, #32
	b	.L366
.L382:
	mov	r8, #14
	b	.L366
.L385:
	mov	r10, #255
	b	.L372
.L386:
	mov	r3, #256
	b	.L376
.L409:
	.align	2
.L408:
	.word	.LANCHOR0+494
	.word	.LC52
	.word	.LC53
	.word	.LC54
	.word	1374389535
	.word	.LANCHOR1
	.word	.LC55
	.word	.LC56
	.word	.LC57
	.word	.LC58
	.word	.LC59
	.word	.LC60
	.word	rc_debug
	.fnend
	.size	moving_ratio_calc, .-moving_ratio_calc
	.align	1
	.global	calc_avbr_ratio
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	calc_avbr_ratio, %function
calc_avbr_ratio:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #52
	sub	sp, sp, #52
	ldr	r3, [r0, #56]
	mov	r4, r0
	mov	fp, r1
	ldr	r10, [r0, #476]
	ldr	r6, [r0, #492]
	str	r3, [sp, #36]
	ldrd	r8, r3, [r0, #480]
	ldr	r7, [r0, #496]
	str	r3, [sp, #32]
	ldr	r3, .L450
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #1
	beq	.L411
	mov	r3, r0
	movw	r2, #1001
	ldr	r1, .L450+4
	ldr	r0, .L450+8
	bl	printk
.L411:
	mov	r0, r4
	bl	moving_ratio_calc
	ldr	r3, [r4, #512]
	subs	r5, r3, #2
	cmp	r5, r0
	it	lt
	movlt	r5, r0
	cmp	r3, r5
	ble	.L412
	ldr	r1, [r4, #508]
	ldr	r2, [r4, #568]
	cmp	r2, r1, lsl #6
	bcc	.L412
	adds	r3, r3, #1
	cmp	r3, #255
	it	ge
	movge	r3, #255
	cmp	r3, #1
	it	lt
	movlt	r3, #1
	str	r3, [r4, #512]
.L413:
	ldr	r3, [r9]
	lsls	r3, r3, #25
	bpl	.L414
	mov	r3, r5
	movw	r2, #1013
	ldr	r1, .L450+4
	ldr	r0, .L450+12
	bl	printk
.L414:
	cmp	r5, #1
	ldrd	r2, r3, [r4, #368]
	it	lt
	movlt	r5, #1
	asrs	r1, r5, #31
	muls	r3, r5, r3
	mla	r1, r2, r1, r3
	umull	r2, r3, r2, r5
	mul	r10, r5, r10
	strd	r2, [sp, #24]
	ldr	r3, [sp, #28]
	add	r3, r3, r1
	str	r3, [sp, #28]
	ldr	r3, [sp, #24]
	lsrs	r2, r3, #8
	ldr	r3, [sp, #28]
	orr	r2, r2, r3, lsl #24
	asrs	r1, r2, #31
	asr	r9, r2, #10
	mov	r0, r2
	strd	r0, [sp, #24]
	mov	r3, r1
	mov	r1, fp
	asr	fp, r10, #8
	ldr	r10, .L450
	cmp	r9, #1
	mov	r0, r4
	it	lt
	movlt	r9, #1
	bl	bits_model_alloc
	ldr	r3, [r10]
	lsls	r0, r3, #26
	bpl	.L416
	ldr	r3, .L450+16
	movw	r2, #1030
	ldr	r1, .L450+4
	ldr	r0, .L450+20
	str	r3, [sp, #12]
	ldr	r3, .L450+24
	str	r3, [sp, #8]
	ldr	r3, .L450+28
	str	r3, [sp, #4]
	ldr	r3, .L450+32
	str	r3, [sp]
	ldr	r3, .L450+36
	bl	printk
	ldr	r3, [r10]
	lsls	r1, r3, #26
	bpl	.L416
	ldr	r3, [sp, #32]
	mov	r2, #1032
	strd	r6, r7, [sp, #8]
	str	r8, [sp, #4]
	str	r3, [sp]
	ldr	r1, .L450+4
	ldr	r3, [r4, #512]
	ldr	r0, .L450+40
	bl	printk
.L416:
	ldr	r3, [sp, #32]
	mov	r1, r8
	cmp	r8, r3
	sub	r0, r3, r8
	ble	.L418
	lsls	r0, r0, #5
	bl	__aeabi_idiv
	mov	r5, r0
.L419:
	ldr	r3, [r4, #520]
	lsr	r8, r3, #4
	cmp	r8, #9
	bls	.L429
	cmp	r8, #200
	it	ge
	movge	r8, #200
.L420:
	ldr	r3, [sp, #36]
	mov	r0, r7
	asr	r10, r3, #5
	mov	r1, r10
	bl	__aeabi_idiv
	mov	r1, r10
	usat	r2, #6, r0
	mov	r0, r6
	str	r2, [sp, #32]
	bl	__aeabi_idiv
	ldr	r3, .L450+44
	cmp	r7, fp
	ldr	r2, [sp, #32]
	usat	r0, #6, r0
	ldr	r2, [r3, r2, lsl #2]
	ldr	r3, [r3, r0, lsl #2]
	sub	r3, r2, r3
	mov	r2, r7
	it	lt
	movlt	r2, fp
	cmp	r2, r6
	bge	.L421
	movs	r7, #6
	muls	r7, r3, r7
.L422:
	asrs	r7, r7, #2
	cmp	r7, #256
	mvn	r10, #127
	mov	r0, #10
	mov	r1, r8
	it	ge
	movge	r7, #256
	cmp	r7, r10
	mul	r0, r0, r5
	it	lt
	movlt	r7, r10
	bl	__aeabi_idiv
	mov	r2, r9
	mov	r8, #24
	cmp	r0, #256
	ldr	r3, [sp, #24]
	it	ge
	movge	r0, #256
	cmp	r0, r10
	mov	r5, r0
	ldrd	r0, r1, [r4, #392]
	it	lt
	movlt	r5, r10
	subs	r0, r0, r3
	ldr	r3, [sp, #28]
	sbc	r1, r1, r3
	lsrs	r0, r0, #10
	add	r3, sp, #44
	orr	r0, r0, r1, lsl #22
	asrs	r1, r1, #10
	bl	div_s64_rem
	mov	r2, r9
	mov	r9, #48
	mul	r8, r8, r0
	ldr	r3, [sp, #24]
	ldrd	r0, r1, [r4, #384]
	cmp	r8, #1
	it	ge
	movge	r8, #1
	subs	r0, r0, r3
	ldr	r3, [sp, #28]
	orr	r8, r8, r8, asr #31
	lsr	r0, r0, #10
	sbc	r1, r1, r3
	add	r3, sp, #44
	orr	r0, r0, r1, lsl #22
	asrs	r1, r1, #10
	bl	div_s64_rem
	ldr	r3, [r4, #520]
	mul	r9, r9, r0
	cmp	r9, #1
	it	ge
	movge	r9, #1
	cmp	r3, #640
	orr	r9, r9, r9, asr #31
	bls	.L423
	cmp	r5, #32
	mvn	r3, #15
	it	ge
	movge	r5, #32
	cmp	r5, r3
	it	lt
	movlt	r5, r3
	cmp	r7, #32
	it	ge
	movge	r7, #32
	cmp	r7, r3
	it	lt
	movlt	r7, r3
.L423:
	ldr	r1, [sp, #36]
	add	r2, r9, r8
	adds	r3, r5, r7
	cmp	r1, r6
	add	r3, r3, r2
	blt	.L434
	cmp	r3, #0
	ble	.L434
	ldr	r0, [r4, #508]
	ldr	r1, [r4, #568]
	cmp	r1, r0, lsl #6
	bcc	.L434
	adds	r3, r7, r2
	mvn	r6, #5
.L424:
	add	r3, r3, r6
	str	r3, [r4, #556]
	ldr	r3, .L450
	ldr	r2, [r3]
	mov	r10, r3
	tst	r2, #128
	beq	.L426
	ldr	r3, .L450+48
	movw	r2, #1091
	ldr	r1, .L450+4
	ldr	r0, .L450+52
	str	r3, [sp, #20]
	ldr	r3, .L450+56
	str	r3, [sp, #16]
	ldr	r3, .L450+60
	str	r3, [sp, #12]
	ldr	r3, .L450+64
	str	r3, [sp, #8]
	ldr	r3, .L450+68
	str	r3, [sp, #4]
	ldr	r3, .L450+72
	str	r3, [sp]
	ldr	r3, .L450+76
	bl	printk
	ldr	r3, [r10]
	lsls	r2, r3, #24
	bpl	.L426
	ldr	r3, [r4, #580]
	movw	r2, #1094
	strd	r8, r9, [sp, #4]
	str	r7, [sp]
	str	r3, [sp, #20]
	ldr	r3, [r4, #556]
	ldr	r1, .L450+4
	ldr	r0, .L450+80
	strd	r6, r3, [sp, #12]
	mov	r3, r5
	bl	printk
.L426:
	ldr	r3, [r10]
	lsls	r3, r3, #31
	bpl	.L428
	mov	r3, r4
	movw	r2, #1095
	ldr	r1, .L450+4
	ldr	r0, .L450+84
	bl	printk
.L428:
	movs	r0, #0
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L412:
	str	r5, [r4, #512]
	b	.L413
.L418:
	lsls	r0, r0, #6
	bl	__aeabi_idiv
	movs	r3, #100
	muls	r5, r3, r5
	asrs	r5, r5, #8
	muls	r5, r0, r5
	b	.L419
.L429:
	mov	r8, #10
	b	.L420
.L421:
	cmp	r6, r7
	bge	.L431
	cmp	r7, fp
	bge	.L431
	add	r7, r3, r3, lsl #1
	b	.L422
.L431:
	movs	r7, #0
	b	.L422
.L434:
	movs	r6, #0
	b	.L424
.L451:
	.align	2
.L450:
	.word	rc_debug
	.word	.LANCHOR0+512
	.word	.LC0
	.word	.LC61
	.word	.LC30
	.word	.LC46
	.word	.LC31
	.word	.LC32
	.word	.LC26
	.word	.LC62
	.word	.LC63
	.word	.LANCHOR1
	.word	.LC37
	.word	.LC64
	.word	.LC38
	.word	.LC65
	.word	.LC66
	.word	.LC67
	.word	.LC41
	.word	.LC35
	.word	.LC68
	.word	.LC1
	.fnend
	.size	calc_avbr_ratio, .-calc_avbr_ratio
	.align	1
	.global	bits_model_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_model_init, %function
bits_model_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	ldr	r6, .L509
	.pad #28
	sub	sp, sp, #28
	ldr	r5, [r0, #40]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L453
	mov	r3, r0
	movw	r2, #1107
	ldr	r1, .L509+4
	ldr	r0, .L509+8
	bl	printk
.L453:
	ldr	r3, [r4, #68]
	ldr	r2, [r4, #64]
	cmp	r3, #100
	it	ge
	movge	r3, #100
	cmp	r3, #10
	it	lt
	movlt	r3, #10
	cmp	r2, r3
	str	r3, [r4, #68]
	blt	.L477
	cmp	r2, #100
	it	ge
	movge	r2, #100
.L454:
	ldr	r1, [r4, #72]
	ldr	r0, [r6]
	cmp	r1, #640
	str	r2, [r4, #64]
	it	ge
	movge	r1, #640
	cmp	r1, #160
	it	lt
	movlt	r1, #160
	lsls	r7, r0, #25
	str	r1, [r4, #72]
	bpl	.L455
	strd	r2, r1, [sp]
	movw	r2, #1116
	ldr	r1, .L509+4
	ldr	r0, .L509+12
	bl	printk
	cmp	r5, #0
	bne	.L456
	ldr	r3, [r6]
	lsls	r0, r3, #25
	bpl	.L458
	movw	r2, #1119
	ldr	r1, .L509+4
	ldr	r0, .L509+16
	bl	printk
.L458:
	mov	r7, #500
.L506:
	ldr	r3, [r4, #504]
	str	r7, [r4, #40]
	cbnz	r3, .L464
	ldr	r3, [r4, #48]
	cmp	r3, #0
	beq	.L479
	ldr	r1, [r4, #56]
	cmp	r1, #0
	beq	.L479
	movs	r0, #100
	muls	r0, r3, r0
	bl	__aeabi_idiv
	mov	r3, r0
.L465:
	ldr	r2, [r6]
	str	r3, [r4, #504]
	lsls	r2, r2, #25
	bpl	.L464
	movw	r2, #1141
	ldr	r1, .L509+4
	ldr	r0, .L509+20
	bl	printk
.L464:
	movs	r3, #35
	str	r3, [r4, #508]
	movs	r3, #90
	str	r3, [r4, #500]
	movs	r3, #1
	str	r3, [r4, #380]
	movs	r3, #0
	strd	r3, r3, [r4, #596]
	ldr	r3, .L509+24
	ldr	r5, [r4, #56]
	str	r3, [r4, #660]
	ldr	r3, [r4, #8]
	cmp	r3, #3
	bhi	.L467
	tbb	[pc, r3]
.L469:
	.byte	(.L472-.L469)/2
	.byte	(.L471-.L469)/2
	.byte	(.L480-.L469)/2
	.byte	(.L468-.L469)/2
	.p2align 1
.L477:
	mov	r2, r3
	b	.L454
.L456:
	cmp	r5, #1
	bne	.L460
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L461
	movw	r2, #1122
	ldr	r1, .L509+4
	ldr	r0, .L509+28
	bl	printk
.L461:
	movs	r3, #16
	str	r3, [r4, #72]
	ldr	r3, [r4, #200]
	cmp	r3, #0
	beq	.L458
	movs	r3, #0
	str	r3, [r4, #200]
	b	.L458
.L479:
	movs	r3, #25
	b	.L465
.L471:
	ldr	r3, .L509+32
	ldr	r5, [r4, #52]
	str	r3, [r4, #656]
	ldr	r3, .L509+36
	str	r3, [r4, #660]
.L473:
	ldr	r3, [r4, #52]
	cmp	r7, #0
	str	r3, [r4, #476]
	ldrd	r2, r3, [r4, #28]
	it	gt
	smullgt	r8, r9, r7, r3
	mov	r7, #0
	it	le
	smullle	r8, r9, r2, r3
	add	r3, sp, #20
	umull	r0, r1, r8, r5
	mla	r1, r5, r9, r1
	bl	div_s64_rem
	strd	r0, r7, [r4, #368]
	mov	r8, r0
	ldrd	r1, r0, [r4, #28]
	muls	r0, r5, r0
	bl	__aeabi_uidiv
	ldr	r3, [r6]
	add	r5, r5, r5, lsl #1
	str	r5, [r4, #540]
	lsls	r2, r3, #25
	asr	r5, r5, #3
	strd	r5, r5, [r4, #544]
	str	r0, [r4, #376]
	bpl	.L476
	ldr	r3, [r4, #60]
	movw	r2, #1194
	strd	r8, r7, [sp]
	ldr	r1, .L509+4
	strd	r0, r3, [sp, #8]
	ldr	r3, [r4, #40]
	ldr	r0, .L509+40
	bl	printk
.L476:
	mov	r0, r4
	bl	bits_model_param_init
	mov	r5, r0
	cbnz	r0, .L481
	mov	r0, r4
	bl	bits_frm_init
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L452
	mov	r3, r4
	mov	r2, #1200
	ldr	r1, .L509+4
	ldr	r0, .L509+44
	bl	printk
.L452:
	mov	r0, r5
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L472:
	ldr	r3, .L509+48
.L507:
	str	r3, [r4, #656]
	b	.L473
.L468:
	ldr	r3, .L509+52
	b	.L507
.L467:
	movw	r1, #1171
	ldr	r0, .L509+56
	bl	printk
	b	.L473
.L480:
	movs	r5, #0
	b	.L452
.L481:
	mov	r5, #-1
	b	.L452
.L455:
	cmp	r5, #0
	beq	.L458
	cmp	r5, #1
	beq	.L461
.L460:
	ldr	r7, [r4, #40]
	cmp	r7, #500
	it	ge
	movge	r7, #500
	b	.L506
.L510:
	.align	2
.L509:
	.word	rc_debug
	.word	.LANCHOR0+528
	.word	.LC0
	.word	.LC69
	.word	.LC70
	.word	.LC72
	.word	reenc_calc_vbr_ratio
	.word	.LC71
	.word	calc_cbr_ratio
	.word	reenc_calc_cbr_ratio
	.word	.LC74
	.word	.LC1
	.word	calc_vbr_ratio
	.word	calc_avbr_ratio
	.word	.LC73
	.fnend
	.size	bits_model_init, .-bits_model_init
	.align	1
	.global	bits_mode_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_mode_reset, %function
bits_mode_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r5, .L518
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L513
	mov	r3, r0
	movw	r2, #1206
	ldr	r1, .L518+4
	ldr	r0, .L518+8
	bl	printk
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L513
	mov	r3, r4
	mov	r2, #1208
	ldr	r1, .L518+4
	ldr	r0, .L518+12
	bl	printk
.L513:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L519:
	.align	2
.L518:
	.word	rc_debug
	.word	.LANCHOR0+544
	.word	.LC0
	.word	.LC1
	.fnend
	.size	bits_mode_reset, .-bits_mode_reset
	.align	1
	.global	check_super_frame
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_super_frame, %function
check_super_frame:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r5, r0
	ldr	r6, .L538
	mov	r7, r1
	ldr	r8, [r0, #356]
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L521
	mov	r3, r5
	movw	r2, #1219
	ldr	r1, .L538+4
	ldr	r0, .L538+8
	bl	printk
.L521:
	ldr	r4, [r5, #184]
	cbz	r4, .L522
	cmp	r8, #2
	ldr	r3, [r5, #192]
	ldr	r2, [r7, #24]
	it	eq
	ldreq	r3, [r5, #188]
	cmp	r2, r3
	bcc	.L527
	cmp	r4, #1
	bne	.L528
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L524
	movw	r2, #1228
	ldr	r1, .L538+4
	ldr	r0, .L538+12
	bl	printk
.L524:
	movs	r3, #1
	str	r3, [r5, #172]
	movs	r3, #0
	str	r3, [r5, #180]
.L528:
	mov	r4, #-1
	b	.L522
.L527:
	movs	r4, #0
.L522:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L520
	mov	r3, r5
	movw	r2, #1235
	ldr	r1, .L538+4
	ldr	r0, .L538+16
	bl	printk
.L520:
	mov	r0, r4
	pop	{r4, r5, r6, r7, r8, pc}
.L539:
	.align	2
.L538:
	.word	rc_debug
	.word	.LANCHOR0+560
	.word	.LC0
	.word	.LC75
	.word	.LC1
	.fnend
	.size	check_super_frame, .-check_super_frame
	.align	1
	.global	check_re_enc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_re_enc, %function
check_re_enc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r4, r0
	ldr	r5, [r0, #60]
	mov	r9, r1
	ldr	r10, [r0, #356]
	ldr	r0, [r0, #528]
	bl	mpp_data_sum_v2
	mov	r1, r5
	bl	__aeabi_idiv
	mov	r1, #-1
	ldr	r7, .L581
	mul	r6, r0, r5
	mov	r8, r0
	ldr	r0, [r4, #528]
	bl	mpp_data_get_pre_val_v2
	ldr	r3, [r9, #24]
	mov	r1, r5
	subs	r0, r3, r0
	add	r0, r0, r6
	bl	__aeabi_idiv
	ldr	r3, [r7]
	lsls	r5, r3, #31
	mov	r6, r0
	bpl	.L541
	mov	r3, r4
	movw	r2, #1253
	ldr	r1, .L581+4
	ldr	r0, .L581+8
	bl	printk
.L541:
	ldr	r3, [r7]
	lsls	r0, r3, #25
	bpl	.L542
	strd	r8, r6, [sp]
	movw	r2, #1255
	ldr	r3, [r4, #52]
	ldr	r1, .L581+4
	ldr	r0, .L581+12
	bl	printk
.L542:
	ldr	r2, [r4, #608]
	ldr	r3, [r4, #152]
	cmp	r2, r3
	blt	.L543
.L546:
	movs	r5, #0
.L540:
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L543:
	mov	r1, r9
	mov	r0, r4
	bl	check_super_frame
	mov	r5, r0
	cmp	r0, #0
	bne	.L554
	ldr	r3, [r4, #200]
	cbz	r3, .L545
	ldr	r3, [r4, #380]
	cmp	r3, #0
	beq	.L546
.L545:
	ldr	r3, [r7]
	lsls	r1, r3, #22
	bpl	.L547
	str	r10, [sp]
	movw	r2, #1267
	ldr	r3, [r4, #172]
	ldr	r1, .L581+4
	ldr	r0, .L581+16
	bl	printk
.L547:
	ldr	r0, [r4, #172]
	cbz	r0, .L548
	cmp	r10, #0
	bne	.L548
	ldr	r2, [r4, #176]
	ldr	r3, .L581+20
	add	r1, r2, #100
	ldr	r2, [r4, #56]
	muls	r2, r1, r2
	umull	r2, r3, r2, r3
	lsrs	r5, r3, #5
	ldr	r3, [r7]
	lsls	r2, r3, #22
	bpl	.L549
	ldr	r3, [r4, #52]
	movw	r2, #1274
	strd	r5, r6, [sp, #4]
	ldr	r1, .L581+4
	str	r3, [sp]
	mov	r3, r0
	ldr	r0, .L581+24
	bl	printk
.L549:
	cmp	r6, r5
	mov	r5, #-1
	it	le
	movle	r5, #0
	b	.L540
.L548:
	ldr	r3, [r4, #244]
	cmp	r3, #0
	bne	.L546
	cmp	r10, #0
	beq	.L550
	cmp	r10, #2
	bne	.L551
	ldr	r3, [r9]
	add	r3, r3, r3, lsl #1
	add	r3, r3, r3, lsr #31
	asrs	r3, r3, #1
.L551:
	ldr	r2, [r9, #24]
	cmp	r2, r3
	ble	.L552
	ldr	r3, [r4, #8]
	cmp	r3, #1
	bne	.L553
	ldr	r2, [r4, #52]
	sub	r8, r6, r8
	ldr	r0, .L581+28
	asrs	r3, r2, #31
	smull	r0, r1, r2, r0
	rsb	ip, r3, r1, asr #3
	cmp	ip, r8
	bge	.L552
	asrs	r5, r1, #2
	subs	r1, r5, r3
	add	r1, r1, r2
	cmp	r1, r6
	blt	.L556
	subs	r5, r3, r5
	add	r5, r5, r2
	cmp	r5, r6
	mov	r5, #-1
	it	le
	movle	r5, #0
.L552:
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L540
	str	r5, [sp]
	mov	r3, r4
	movw	r2, #1307
	ldr	r1, .L581+4
	ldr	r0, .L581+32
	bl	printk
	b	.L540
.L550:
	ldr	r3, [r9]
	add	r3, r3, r3, lsl #1
	b	.L551
.L553:
	ldr	r3, [r4, #56]
	sub	r2, r3, r3, asr #3
	cmp	r2, r6
	bge	.L552
	ldr	r0, .L581+28
	sub	r5, r6, r8
	smull	r0, r1, r3, r0
	asrs	r3, r3, #31
	rsb	r3, r3, r1, asr #3
	cmp	r3, r5
	mov	r5, #-1
	it	ge
	movge	r5, #0
	b	.L552
.L556:
	mov	r5, #-1
	b	.L552
.L554:
	mov	r5, #-1
	b	.L540
.L582:
	.align	2
.L581:
	.word	rc_debug
	.word	.LANCHOR0+578
	.word	.LC0
	.word	.LC76
	.word	.LC77
	.word	1374389535
	.word	.LC78
	.word	1717986919
	.word	.LC79
	.fnend
	.size	check_re_enc, .-check_re_enc
	.align	1
	.global	rc_model_v2_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_init, %function
rc_model_v2_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L592
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L584
	mov	r3, r0
	movw	r2, #1315
	ldr	r1, .L592+4
	ldr	r0, .L592+8
	bl	printk
.L584:
	movs	r2, #252
	mov	r1, r6
	mov	r0, r4
	bl	memcpy
	mov	r0, r4
	bl	bits_model_init
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L585
	mov	r3, r4
	mov	r2, #1320
	ldr	r1, .L592+4
	ldr	r0, .L592+12
	bl	printk
.L585:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L593:
	.align	2
.L592:
	.word	rc_debug
	.word	.LANCHOR0+591
	.word	.LC0
	.word	.LC1
	.fnend
	.size	rc_model_v2_init, .-rc_model_v2_init
	.align	1
	.global	rc_model_v2_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_deinit, %function
rc_model_v2_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	ldr	r4, .L603
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L595
	mov	r3, r0
	mov	r2, #1328
	ldr	r1, .L603+4
	ldr	r0, .L603+8
	bl	printk
.L595:
	mov	r0, r5
	bl	bits_model_param_deinit
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L596
	mov	r3, r5
	movw	r2, #1331
	ldr	r1, .L603+4
	ldr	r0, .L603+12
	bl	printk
.L596:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L604:
	.align	2
.L603:
	.word	rc_debug
	.word	.LANCHOR0+608
	.word	.LC0
	.word	.LC1
	.fnend
	.size	rc_model_v2_deinit, .-rc_model_v2_deinit
	.align	1
	.global	rc_model_v2_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_start, %function
rc_model_v2_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r1
	ldr	r6, .L661
	mov	r4, r0
	ldr	r3, [r6]
	lsls	r1, r3, #31
	bpl	.L606
	mov	r3, r0
	movw	r2, #1342
	ldr	r1, .L661+4
	ldr	r0, .L661+8
	bl	printk
.L606:
	ldr	r3, [r4, #8]
	cmp	r3, #2
	bne	.L607
	ldr	r2, [r4, #92]
	cmp	r2, #0
	bgt	.L608
	movw	r1, #1347
	ldr	r0, .L661+12
	bl	printk
	movs	r3, #26
	str	r3, [r4, #92]
.L608:
	ldr	r3, [r4, #96]
	cmp	r3, #0
	itt	le
	ldrle	r3, [r4, #92]
	strle	r3, [r4, #96]
	ldr	r3, [r4, #100]
	cmp	r3, #0
	itt	le
	ldrle	r3, [r4, #92]
	strle	r3, [r4, #100]
	ldr	r3, [r4, #104]
	cmp	r3, #0
	itt	le
	ldrle	r3, [r4, #96]
	strle	r3, [r4, #104]
	ldr	r3, [r4, #108]
	cmp	r3, #0
	itt	le
	ldrle	r3, [r4, #100]
	strle	r3, [r4, #108]
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	lsls	r2, r3, #27
	itete	mi
	ldrmi	r2, [r4, #104]
	ldrpl	r2, [r4, #96]
	strmi	r2, [r5, #176]
	strpl	r2, [r5, #176]
	ite	mi
	ldrmi	r2, [r4, #108]
	ldrpl	r2, [r4, #100]
	str	r2, [r5, #180]
	ldr	r2, [r4, #92]
	str	r2, [r5, #172]
	ldr	r2, [r6]
	lsls	r4, r2, #25
	bpl	.L617
	ubfx	r3, r3, #4, #1
	movw	r2, #1370
	str	r3, [sp]
	ldr	r1, .L661+4
	ldrh	r3, [r5, #158]
	ldr	r0, .L661+16
	bl	printk
	ldr	r3, [r6]
	lsls	r0, r3, #25
	bpl	.L617
	ldr	r3, [r5, #164]
	movw	r2, #1372
	ldr	r1, .L661+4
	ldr	r0, .L661+20
	str	r3, [sp, #4]
	ldr	r3, [r5, #160]
	str	r3, [sp]
	ldr	r3, [r5, #168]
	bl	printk
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L617
	ldr	r3, [r5, #176]
	movw	r2, #1374
	ldr	r1, .L661+4
	ldr	r0, .L661+24
	str	r3, [sp, #4]
	ldr	r3, [r5, #172]
	str	r3, [sp]
	ldr	r3, [r5, #180]
	bl	printk
.L617:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L607:
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	lsrs	r3, r3, #3
	and	r3, r3, #2
	str	r3, [r4, #356]
	ldrb	r2, [r5, #154]	@ zero_extendqisi2
	and	r2, r2, #63
	cmp	r2, #3
	it	eq
	moveq	r3, r2
	str	r3, [r4, #356]
	movs	r3, #0
	str	r3, [r4, #556]
	ldr	r3, [r4, #360]
	cmp	r3, #2
	bne	.L619
	mov	r0, r4
	bl	calc_next_i_ratio
.L619:
	ldr	r3, [r4, #380]
	add	r1, r5, #160
	cmp	r3, #0
	bne	.L620
	ldr	r3, [r4, #656]
	cbz	r3, .L622
	mov	r0, r4
	blx	r3
.L623:
	ldr	r3, [r4, #380]
	cbz	r3, .L622
	ldr	r3, [r4, #92]
	cmp	r3, #0
	it	le
	movle	r3, #-1
	str	r3, [r5, #172]
.L622:
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	lsls	r2, r3, #27
	itete	mi
	ldrmi	r2, [r4, #104]
	ldrpl	r2, [r4, #96]
	strmi	r2, [r5, #176]
	strpl	r2, [r5, #176]
	ite	mi
	ldrmi	r2, [r4, #108]
	ldrpl	r2, [r4, #100]
	str	r2, [r5, #180]
	ldr	r2, [r6]
	lsls	r0, r2, #25
	bpl	.L630
	ubfx	r3, r3, #4, #1
	ldr	r1, .L661+4
	str	r3, [sp]
	movw	r2, #1407
	ldrh	r3, [r5, #158]
	ldr	r0, .L661+16
	bl	printk
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L630
	ldr	r3, [r5, #164]
	movw	r2, #1409
	ldr	r1, .L661+4
	ldr	r0, .L661+20
	str	r3, [sp, #4]
	ldr	r3, [r5, #160]
	str	r3, [sp]
	ldr	r3, [r5, #168]
	bl	printk
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L630
	ldr	r3, [r5, #176]
	movw	r2, #1411
	ldr	r1, .L661+4
	ldr	r0, .L661+24
	str	r3, [sp, #4]
	ldr	r3, [r5, #172]
	str	r3, [sp]
	ldr	r3, [r5, #180]
	bl	printk
.L630:
	movs	r3, #0
	str	r3, [r4, #608]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L617
	mov	r3, r4
	movw	r2, #1415
	ldr	r1, .L661+4
	ldr	r0, .L661+28
	bl	printk
	b	.L617
.L620:
	ldrd	r2, [r4, #368]
	mov	r0, r4
	bl	bits_model_alloc
	b	.L623
.L662:
	.align	2
.L661:
	.word	rc_debug
	.word	.LANCHOR0+627
	.word	.LC0
	.word	.LC80
	.word	.LC81
	.word	.LC82
	.word	.LC83
	.word	.LC1
	.fnend
	.size	rc_model_v2_start, .-rc_model_v2_start
	.align	1
	.global	rc_model_v2_hal_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_hal_start, %function
rc_model_v2_hal_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #68
	sub	sp, sp, #68
	ldr	r3, [r1, #168]
	mov	r4, r0
	mov	r5, r1
	ldrd	r8, r7, [r0]
	ldrd	r10, r6, [r0, #120]
	str	r3, [sp, #28]
	ldr	r3, [r1, #164]
	str	r3, [sp, #32]
	ldr	r3, [r1, #160]
	str	r3, [sp, #36]
	ldrd	r3, r9, [r1, #176]
	str	r3, [sp, #40]
	ldr	r3, [r1, #172]
	str	r3, [sp, #44]
	ldr	r3, [r0, #132]
	str	r3, [sp, #48]
	ldr	r3, [r0, #128]
	str	r3, [sp, #52]
	ldr	r3, .L790
	ldr	r2, [r3]
	mov	fp, r3
	tst	r2, #1
	beq	.L664
	str	r1, [sp]
	mov	r3, r0
	movw	r2, #1483
	ldr	r1, .L790+4
	ldr	r0, .L790+8
	bl	printk
.L664:
	ldr	r3, [fp]
	lsls	r1, r3, #25
	bpl	.L665
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	movw	r2, #1485
	ldr	r1, .L790+4
	ubfx	r3, r3, #4, #1
	ldr	r0, .L790+12
	str	r3, [sp]
	ldrh	r3, [r5, #158]
	bl	printk
.L665:
	ldr	r3, [r5, #264]
	ands	r3, r3, #1
	beq	.L666
	ldr	r3, [r5, #268]
	strd	r3, r3, [r5, #172]
	str	r3, [r5, #180]
.L766:
	movs	r0, #0
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L666:
	ldr	r2, [r4, #8]
	cmp	r2, #2
	beq	.L766
	ldr	r2, [r4, #380]
	cmp	r2, #0
	beq	.L669
	ldrb	r2, [r5, #152]	@ zero_extendqisi2
	lsls	r2, r2, #27
	bpl	.L669
	ldr	r2, [r5, #172]
	ldr	r6, [r4, #112]
	cmp	r2, #0
	bge	.L670
	ldr	r1, [r5, #160]
	cmp	r1, #0
	beq	.L671
	add	r0, r7, #15
	add	r8, r8, #15
	ldr	r7, .L790+16
	asr	r8, r8, #4
	asrs	r0, r0, #4
	mul	r0, r0, r8
.L672:
	adds	r3, r3, #1
	cmp	r3, #8
	beq	.L673
	ldr	r2, [r7, r3, lsl #2]
	cmp	r0, r2
	bcs	.L672
.L673:
	ldr	r7, .L790+20
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #800]
	muls	r0, r3, r0
	sub	r0, r0, #350
	bl	__aeabi_uidiv
	cmp	r0, #95
	it	ge
	movge	r0, #95
	cmp	r0, #4
	ite	ge
	addge	r7, r7, r0
	addlt	r7, r7, #4
	ldrb	r3, [r7, #704]	@ zero_extendqisi2
.L785:
	str	r3, [r5, #172]
.L670:
	ldr	r2, [r4, #608]
	cmp	r2, #0
	beq	.L675
	ldr	r3, [r4, #580]
	ldr	r1, [r4, #556]
	add	r3, r3, r1
	str	r3, [r4, #580]
	rsb	r6, r6, r3, asr #6
	str	r6, [r4, #584]
.L676:
	ldr	r3, .L790
	ldr	r3, [r3]
	lsls	r0, r3, #25
	bpl	.L677
	ldr	r3, [r4, #556]
	str	r2, [sp, #8]
	movw	r2, #1525
	ldr	r1, .L790+4
	str	r3, [sp, #4]
	ldr	r3, [r4, #580]
	ldr	r0, .L790+24
	str	r3, [sp]
	ldr	r3, [r4, #584]
	bl	printk
.L677:
	ldr	r3, [r5, #180]
	ldr	r2, [r4, #580]
	ldr	r1, [r5, #176]
	lsls	r3, r3, #6
	cmp	r2, r3
	blt	.L678
	lsls	r3, r1, #6
	cmp	r3, r2
	it	ge
	movge	r3, r2
.L678:
	mov	r6, r9
	str	r3, [r4, #580]
.L679:
	ldr	r3, [r4, #208]
	ldr	r7, .L790
	cbz	r3, .L715
	ldr	r3, [r4, #608]
	cbnz	r3, .L715
	mov	r0, r4
	bl	rc_hier_calc_dealt_qp
	ldr	r3, [r4, #624]
	cbz	r3, .L715
	add	r2, r4, r3, lsl #2
	ldr	r1, [r2, #208]
	ldr	r2, [r4, #584]
	subs	r2, r2, r1
	str	r2, [r4, #584]
	ldr	r2, [r7]
	lsls	r0, r2, #24
	bpl	.L715
	str	r1, [sp]
	movw	r2, #1635
	ldr	r1, .L790+4
	ldr	r0, .L790+28
	bl	printk
.L715:
	ldr	r3, [r4, #584]
	ldr	r2, [r5, #176]
	cmp	r3, r6
	blt	.L717
	cmp	r3, r2
	it	ge
	movge	r3, r2
	mov	r6, r3
.L717:
	ldr	r3, [r7]
	str	r6, [r4, #584]
	lsls	r1, r3, #25
	str	r6, [r5, #172]
	bpl	.L719
	ldr	r3, [r5, #164]
	movw	r2, #1645
	ldr	r1, .L790+4
	ldr	r0, .L790+32
	str	r3, [sp, #16]
	ldr	r3, [r5, #160]
	str	r3, [sp, #12]
	ldr	r3, [r5, #168]
	str	r3, [sp, #8]
	ldr	r3, [sp, #32]
	str	r3, [sp, #4]
	ldr	r3, [sp, #36]
	str	r3, [sp]
	ldr	r3, [sp, #28]
	bl	printk
	ldr	r3, [r7]
	lsls	r2, r3, #25
	bpl	.L719
	ldr	r3, [r5, #176]
	mov	r2, #1648
	ldr	r1, .L790+4
	ldr	r0, .L790+36
	str	r3, [sp, #16]
	ldr	r3, [r5, #172]
	str	r3, [sp, #12]
	ldr	r3, [r5, #180]
	str	r3, [sp, #8]
	ldr	r3, [sp, #40]
	str	r3, [sp, #4]
	ldr	r3, [sp, #44]
	str	r3, [sp]
	mov	r3, r9
	bl	printk
.L719:
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L766
	mov	r3, r4
	movw	r2, #1649
	ldr	r1, .L790+4
	ldr	r0, .L790+40
	bl	printk
	b	.L766
.L671:
	movw	r1, #1509
	ldr	r0, .L790+44
	bl	printk
	movs	r3, #26
	b	.L785
.L675:
	ldr	r3, [r5, #172]
	add	r6, r6, r3
	str	r3, [r4, #584]
	lsls	r6, r6, #6
	str	r6, [r4, #580]
	b	.L676
.L791:
	.align	2
.L790:
	.word	rc_debug
	.word	.LANCHOR0+645
	.word	.LC84
	.word	.LC81
	.word	.LANCHOR0+668
	.word	.LANCHOR0
	.word	.LC86
	.word	.LC90
	.word	.LC91
	.word	.LC92
	.word	.LC1
	.word	.LC85
.L669:
	ldr	r3, [r4, #556]
	ldr	r7, [r4, #580]
	ldr	r0, [r4, #472]
	add	r7, r7, r3
	bl	mpp_data_sum_v2
	mov	fp, r0
	ldr	r0, [r4, #468]
	bl	mpp_data_sum_v2
	movs	r1, #0
	str	r0, [sp, #60]
	ldr	r0, [r4, #468]
	bl	mpp_data_get_pre_val_v2
	movs	r1, #1
	mov	r8, r0
	ldr	r0, [r4, #468]
	bl	mpp_data_get_pre_val_v2
	movs	r1, #2
	str	r0, [sp, #56]
	ldr	r0, [r4, #468]
	bl	mpp_data_get_pre_val_v2
	ldr	r1, [r4, #8]
	cmp	r1, #3
	ldrd	r2, r3, [sp, #56]
	beq	.L680
	cmp	r1, #0
	beq	.L680
	cmp	r1, #1
	beq	.L681
	mov	r6, r9
.L682:
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	ldr	r2, [r5, #176]
	lsls	r1, r3, #27
	lsl	r2, r2, #6
	bpl	.L696
	ldr	r3, [r5, #180]
	ldr	fp, [r4, #112]
	lsl	r8, r3, #6
	cmp	r8, r7
	bgt	.L697
	cmp	r7, r2
	it	ge
	movge	r7, r2
	mov	r8, r7
.L697:
	ldr	r3, [r4, #552]
	ldr	r2, [r4, #560]
	add	r3, r3, r8
	asrs	r3, r3, #6
	cmp	r3, #34
	ble	.L698
	cmp	r2, #33
	bgt	.L698
	movw	r10, #717
	mul	r10, r10, r3
	movw	r3, #307
	mla	r10, r3, r2, r10
	asr	r10, r10, #10
.L699:
	cmp	fp, #0
	beq	.L700
	ldr	r0, [r4, #460]
	bl	mpp_data_mean_v2
	adds	r3, r0, #3
	blt	.L727
	cmp	r0, #31
	bgt	.L728
	cmp	r0, #0
	it	lt
	addlt	r0, r0, #3
	asrs	r0, r0, #2
.L701:
	ldr	r1, .L792
	ldr	r2, .L792+4
	add	r0, r0, r1
	ldrsb	r3, [r0, #836]
	ldr	r2, [r2]
	cmp	fp, r3
	ite	le
	suble	r7, r10, fp
	subgt	r7, r10, r3
	lsls	r0, r2, #25
	bpl	.L703
	ldr	r2, [r4, #608]
	addw	r1, r1, #645
	str	r3, [sp, #12]
	ldr	r3, [r4, #112]
	strd	r7, r2, [sp, #16]
	movw	r2, #1593
	ldr	r0, .L792+8
	strd	r10, r3, [sp, #4]
	asr	r3, r8, #6
	str	r3, [sp]
	ldr	r3, [r4, #560]
	bl	printk
.L703:
	mov	r10, r7
.L700:
	cmp	r6, r10
	ldr	r3, [r5, #176]
	bgt	.L729
	cmp	r3, r10
	bge	.L704
	cmp	r3, r6
	blt	.L730
	mov	r10, r3
.L704:
	ldr	r3, [sp, #48]
	cmp	r3, r10
	it	ge
	movge	r3, r10
.L705:
	str	r3, [r4, #584]
	ldr	r3, [r4, #608]
	cbnz	r3, .L707
	ldr	r3, [r4, #200]
	str	r8, [r4, #580]
	cbz	r3, .L707
	mov	r0, r4
	bl	calc_debreath_qp
.L707:
	movs	r3, #0
	strd	r3, r3, [r4, #596]
	b	.L679
.L722:
	add	r8, r8, r2
	ldrb	r2, [r5, #152]	@ zero_extendqisi2
	add	r0, r0, r8
	and	r2, r2, #16
	cmp	r2, #0
	it	eq
	moveq	r6, r10
	cmp	r3, #1400
	blt	.L683
	cmp	r0, #300
	ite	gt
	movgt	r3, #3
	movle	r3, #2
.L786:
	add	r6, r6, r3
	cmp	fp, #14
.L789:
	ble	.L689
	b	.L787
.L683:
	cmp	r0, #300
	ite	gt
	movgt	r3, #2
	movle	r3, #1
	b	.L786
.L692:
	add	r6, r10, #1
.L689:
	ldr	r3, [r5, #176]
	cmp	r6, r3
	it	ge
	movge	r6, r3
	ldr	r3, [r5, #180]
	cmp	r6, r3
	it	lt
	movlt	r6, r3
	b	.L682
.L691:
	cmp	fp, #9
	ble	.L726
	cmp	r1, #0
	it	eq
	moveq	r6, r10
	b	.L689
.L690:
	cmp	r1, #0
	it	eq
	moveq	r6, r10
	cmp	fp, #14
	ble	.L694
	adds	r6, r6, #3
	b	.L689
.L694:
	cmp	fp, #9
	ble	.L695
	adds	r6, r6, #2
	b	.L689
.L695:
	cmp	fp, #4
	b	.L789
.L698:
	add	r10, r3, r2
	asr	r10, r10, #1
	b	.L699
.L727:
	movs	r0, #0
	b	.L701
.L728:
	movs	r0, #7
	b	.L701
.L729:
	mov	r10, r6
	b	.L704
.L730:
	mov	r3, r6
	b	.L705
.L696:
	lsls	r3, r6, #6
	cmp	r3, r7
	bgt	.L709
	cmp	r7, r2
	mov	r3, r7
	it	ge
	movge	r3, r2
.L709:
	str	r3, [r4, #580]
	asrs	r7, r3, #6
	ldr	r3, .L792+4
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #64
	beq	.L710
	str	r7, [sp]
	movw	r2, #1613
	ldr	r3, [r4, #584]
	ldr	r1, .L792+12
	ldr	r0, .L792+16
	bl	printk
.L710:
	str	r7, [r4, #584]
	ldrb	r3, [r5, #154]	@ zero_extendqisi2
	and	r3, r3, #63
	cmp	r3, #3
	bne	.L711
	ldr	r3, [r4, #116]
	cbz	r3, .L711
	ldr	r2, [r8]
	lsls	r2, r2, #25
	bpl	.L712
	subs	r3, r7, r3
	movw	r2, #1619
	str	r3, [sp]
	mov	r3, r7
	ldr	r1, .L792+12
	ldr	r0, .L792+20
	bl	printk
.L712:
	ldr	r3, [r4, #584]
	ldr	r2, [r4, #116]
	subs	r3, r3, r2
	str	r3, [r4, #584]
.L711:
	ldr	r3, [r4, #584]
	cmp	r3, r6
	blt	.L731
	ldr	r2, [sp, #52]
	cmp	r3, r2
	it	ge
	movge	r3, r2
.L713:
	str	r3, [r4, #584]
	b	.L679
.L731:
	mov	r3, r6
	b	.L713
.L680:
	cmp	r3, #700
	ldrb	r1, [r5, #152]	@ zero_extendqisi2
	bge	.L722
	cmp	r3, #100
	and	r1, r1, #16
	ble	.L690
	cmp	fp, #15
	ble	.L691
	cmp	r1, #0
	beq	.L692
.L787:
	adds	r6, r6, #1
	b	.L689
.L681:
	cmp	r3, #700
	bge	.L722
.L726:
	mov	r6, r9
	b	.L689
.L793:
	.align	2
.L792:
	.word	.LANCHOR0
	.word	rc_debug
	.word	.LC87
	.word	.LANCHOR0+645
	.word	.LC88
	.word	.LC89
	.fnend
	.size	rc_model_v2_hal_start, .-rc_model_v2_hal_start
	.align	1
	.global	rc_model_v2_hal_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_hal_end, %function
rc_model_v2_hal_end:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r6, r1
	ldr	r5, .L805
	mov	r4, r0
	ldr	r2, [r5]
	lsls	r1, r2, #31
	bpl	.L795
	mov	r3, r0
	str	r6, [sp]
	movw	r2, #1658
	ldr	r1, .L805+4
	ldr	r0, .L805+8
	bl	printk
.L795:
	ldr	r3, [r4, #580]
	ldrb	r2, [r6, #152]	@ zero_extendqisi2
	asrs	r3, r3, #6
	lsls	r2, r2, #27
	ite	mi
	strmi	r3, [r4, #560]
	strpl	r3, [r4, #564]
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L798
	mov	r3, r4
	movw	r2, #1665
	ldr	r1, .L805+4
	ldr	r0, .L805+12
	bl	printk
.L798:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L806:
	.align	2
.L805:
	.word	rc_debug
	.word	.LANCHOR0+844
	.word	.LC93
	.word	.LC1
	.fnend
	.size	rc_model_v2_hal_end, .-rc_model_v2_hal_end
	.align	1
	.global	rc_model_v2_check_reenc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_check_reenc, %function
rc_model_v2_check_reenc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r0
	ldr	r7, .L848
	mov	r5, r1
	add	r8, r1, #160
	ldr	r3, [r7]
	lsls	r6, r3, #31
	bpl	.L808
	mov	r3, r0
	str	r8, [sp]
	movw	r2, #1676
	ldr	r1, .L848+4
	ldr	r0, .L848+8
	bl	printk
.L808:
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	bfc	r3, #2, #1
	strb	r3, [r5, #156]
	ldr	r3, [r4, #8]
	cmp	r3, #2
	beq	.L810
	ldr	r3, [r5, #264]
	lsls	r0, r3, #31
	bmi	.L810
	ldr	r3, [r4, #616]
	cmp	r3, #0
	bne	.L810
	ldr	r3, [r4, #620]
	cmp	r3, #0
	bne	.L810
	mov	r1, r8
	mov	r0, r4
	bl	check_re_enc
	cmp	r0, #0
	beq	.L810
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	ldr	r6, [r4, #172]
	tst	r3, #16
	ldr	r3, [r4, #180]
	it	ne
	movne	r6, #0
	cbz	r3, .L813
	ldr	r2, [r4, #612]
	cmp	r3, r2
	it	ls
	movls	r6, #0
.L813:
	ldr	r3, [r7]
	lsls	r1, r3, #22
	bpl	.L814
	ldr	r3, [r4, #612]
	movw	r2, #1695
	ldr	r1, .L848+4
	ldr	r0, .L848+12
	str	r3, [sp]
	mov	r3, r6
	bl	printk
.L814:
	cmp	r6, #1
	beq	.L815
	cmp	r6, #2
	beq	.L816
	ldr	r3, [r4, #660]
	cbz	r3, .L818
	mov	r1, r8
	mov	r0, r4
	blx	r3
.L818:
	ldr	r3, [r4, #556]
	cbz	r3, .L819
	ldrd	r2, r3, [r5, #172]
	cmp	r2, r3
	bge	.L819
	ldr	r3, [r4, #608]
	adds	r3, r3, #1
	str	r3, [r4, #608]
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	orr	r3, r3, #4
	strb	r3, [r5, #156]
.L819:
	movs	r3, #0
	str	r3, [r4, #612]
	b	.L810
.L815:
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	bic	r3, r3, #5
	orr	r3, r3, #5
	strb	r3, [r5, #156]
	ldr	r3, [r4, #612]
	str	r6, [r4, #616]
	adds	r3, r3, #1
	str	r3, [r4, #612]
	ldr	r3, [r7]
	lsls	r2, r3, #22
	bpl	.L810
	ldr	r1, .L848+4
	movw	r2, #1703
	ldr	r0, .L848+16
.L847:
	bl	printk
.L810:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L816:
	ldr	r3, [r7]
	lsls	r3, r3, #22
	bpl	.L810
	movw	r2, #1713
	ldr	r1, .L848+4
	ldr	r0, .L848+20
	b	.L847
.L849:
	.align	2
.L848:
	.word	rc_debug
	.word	.LANCHOR0+864
	.word	.LC94
	.word	.LC95
	.word	.LC96
	.word	.LC97
	.fnend
	.size	rc_model_v2_check_reenc, .-rc_model_v2_check_reenc
	.align	1
	.global	rc_model_v2_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_end, %function
rc_model_v2_end:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r4, r0
	ldr	r6, .L872
	mov	r5, r1
	add	r7, r1, #160
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L851
	str	r7, [sp]
	mov	r3, r4
	movw	r2, #1741
	ldr	r1, .L872+4
	ldr	r0, .L872+8
	bl	printk
.L851:
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L852
	ldr	r3, [r5, #184]
	movw	r2, #1743
	ldr	r1, .L872+4
	ldr	r0, .L872+12
	bl	printk
.L852:
	ldr	r3, [r4, #8]
	cmp	r3, #2
	beq	.L853
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L854
	ldr	r3, [r5, #208]
	movw	r2, #1748
	ldr	r1, .L872+4
	ldr	r0, .L872+16
	str	r3, [sp]
	ldr	r3, [r5, #204]
	bl	printk
.L854:
	ldr	r1, [r5, #204]
	ldr	r0, [r4, #468]
	bl	mpp_data_update_v2
	ldr	r1, [r5, #208]
	ldr	r0, [r4, #472]
	bl	mpp_data_update_v2
	ldr	r3, [r4, #492]
	mov	r0, r4
	str	r3, [r4, #496]
	movs	r3, #0
	str	r3, [r4, #380]
	ldr	r2, [r5, #192]
	ldr	r1, [r5, #184]
	bl	bits_model_update
	ldr	r3, [r4, #8]
	cmp	r3, #3
	bne	.L855
	mov	r1, r7
	mov	r0, r4
	bl	moving_judge_update
	ldr	r1, [r5, #184]
	mov	r0, r4
	bl	bit_statics_update
.L855:
	ldr	r3, [r5, #172]
	ldr	r2, [r4, #584]
	str	r3, [r4, #644]
	ldr	r3, [r5, #180]
	str	r3, [r4, #648]
	ldr	r3, [r5, #176]
	str	r3, [r4, #652]
	ldr	r3, [r4, #600]
	adds	r3, r3, #1
	str	r3, [r4, #600]
	ldr	r3, [r4, #596]
	add	r3, r3, r2
	str	r3, [r4, #596]
	ldr	r3, [r4, #356]
	movs	r2, #0
	str	r3, [r4, #360]
	ldr	r3, [r5, #188]
	str	r3, [r4, #516]
	ldr	r3, [r5, #200]
	str	r2, [r4, #592]
	str	r3, [r4, #604]
	ldr	r3, [r4, #580]
	str	r3, [r4, #568]
	asrs	r3, r3, #6
	ldr	r1, [r5, #160]
	str	r1, [r4, #480]
	ldr	r1, [r5, #184]
	strd	r2, r2, [r4, #616]
	str	r1, [r4, #484]
	str	r3, [r5, #300]
.L853:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L856
	mov	r3, r4
	movw	r2, #1781
	ldr	r1, .L872+4
	ldr	r0, .L872+20
	bl	printk
.L856:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L873:
	.align	2
.L872:
	.word	rc_debug
	.word	.LANCHOR0+888
	.word	.LC94
	.word	.LC98
	.word	.LC99
	.word	.LC1
	.fnend
	.size	rc_model_v2_end, .-rc_model_v2_end
	.align	1
	.global	rc_model_v2_proc_show
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_proc_show, %function
rc_model_v2_proc_show:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	ldr	r3, [r1, #8]
	.pad #60
	sub	sp, sp, #60
	ldr	r7, [r1, #56]
	mov	r5, r0
	cmp	r3, #1
	mov	r6, r2
	it	eq
	ldreq	r7, [r1, #52]
	ldr	r1, .L889
	bl	seq_puts
	ldr	r3, .L889+4
	ldr	r2, .L889+8
	mov	r0, r5
	ldr	r1, .L889+12
	str	r3, [sp, #24]
	ldr	r3, .L889+16
	str	r3, [sp, #20]
	ldr	r3, .L889+20
	str	r3, [sp, #16]
	ldr	r3, .L889+24
	str	r3, [sp, #12]
	ldr	r3, .L889+28
	str	r3, [sp, #8]
	ldr	r3, .L889+32
	str	r3, [sp, #4]
	ldr	r3, .L889+36
	str	r3, [sp]
	ldr	r3, .L889+40
	bl	seq_printf
	ldr	r1, .L889+44
	ldr	r9, [r4, #8]
	ldr	r8, [r4, #60]
	umull	r2, r3, r7, r1
	cmp	r9, #2
	ldr	r7, [r4, #40]
	lsr	r3, r3, #6
	str	r1, [sp, #52]
	str	r3, [sp, #48]
	ldrd	r0, r1, [r4, #16]
	bne	.L876
	bl	__aeabi_idiv
	mov	r10, r0
	ldrd	r0, r1, [r4, #28]
	bl	__aeabi_idiv
	mov	fp, r0
	mov	r0, r9
	bl	strof_rc_mode
	ldr	r3, [r4, #92]
	ldr	r1, .L889+48
	mov	r2, r6
	strd	r3, r3, [sp, #20]
	ldr	r3, .L889+52
	strd	r10, fp, [sp, #4]
	str	r8, [sp]
	strd	r0, r3, [sp, #12]
	mov	r3, r7
.L886:
	mov	r0, r5
	ldr	r8, .L889+272
	bl	seq_printf
	ldr	r1, .L889+56
	mov	r0, r5
	ldr	r9, .L889+276
	bl	seq_puts
	ldr	r7, .L889+60
	ldr	r3, .L889+64
	mov	r0, r5
	ldr	r2, .L889+8
	ldr	r1, .L889+68
	str	r3, [sp, #12]
	str	r8, [sp, #8]
	ldr	r3, .L889+72
	str	r9, [sp, #4]
	str	r7, [sp]
	bl	seq_printf
	ldr	r3, .L889+76
	mov	r2, r6
	ldr	r1, .L889+80
	mov	r0, r5
	str	r3, [sp, #12]
	ldr	r3, [r4, #104]
	str	r3, [sp, #8]
	ldr	r3, [r4, #108]
	str	r3, [sp, #4]
	ldr	r3, [r4, #96]
	str	r3, [sp]
	ldr	r3, [r4, #100]
	bl	seq_printf
	ldr	r1, .L889+84
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L889+88
	ldr	r2, .L889+8
	mov	r0, r5
	ldr	r1, .L889+92
	str	r3, [sp, #8]
	ldr	r3, .L889+96
	str	r3, [sp, #4]
	ldr	r3, .L889+100
	str	r3, [sp]
	ldr	r3, .L889+104
	bl	seq_printf
	ldr	r0, [r4, #172]
	bl	strof_drop
	ldr	r3, [r4, #180]
	mov	r2, r6
	ldr	r1, .L889+108
	str	r3, [sp, #8]
	ldr	r3, [r4, #612]
	str	r3, [sp, #4]
	ldr	r3, [r4, #176]
	str	r3, [sp]
	mov	r3, r0
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L889+112
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L889+116
	ldr	r2, .L889+8
	mov	r0, r5
	ldr	r1, .L889+120
	str	r3, [sp, #8]
	ldr	r3, .L889+124
	str	r3, [sp, #4]
	ldr	r3, .L889+128
	str	r3, [sp]
	ldr	r3, .L889+132
	bl	seq_printf
	ldr	r0, [r4, #184]
	bl	strof_suprmode
	ldr	r3, [r4, #196]
	mov	r2, r6
	ldr	r1, .L889+136
	str	r3, [sp, #8]
	ldr	r3, [r4, #192]
	str	r3, [sp, #4]
	ldr	r3, [r4, #188]
	str	r3, [sp]
	mov	r3, r0
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L889+140
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L889+144
	mov	r0, r5
	ldr	r2, .L889+8
	ldr	r1, .L889+148
	str	r3, [sp, #8]
	ldr	r3, .L889+152
	str	r3, [sp, #4]
	ldr	r3, .L889+156
	str	r3, [sp]
	ldr	r3, .L889+160
	bl	seq_printf
	ldr	r0, [r4, #36]
	cmp	r0, #1
	bne	.L878
	bl	strof_gop_mode
	ldr	r3, [r4, #112]
	ldr	r2, [r4, #44]
	ldr	r1, .L889+164
	strd	r3, r2, [sp]
	mov	r2, r6
	str	r3, [sp, #8]
	mov	r3, r0
.L887:
	mov	r0, r5
	ldr	r10, .L889+72
	bl	seq_printf
	ldr	r3, [r4, #8]
	cmp	r3, #1
	beq	.L880
	cmp	r3, #0
	beq	.L881
	cmp	r3, #3
	beq	.L882
.L883:
	ldr	r1, .L889+168
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L889+172
	ldr	r2, .L889+8
	mov	r0, r5
	ldr	r1, .L889+176
	str	r3, [sp, #28]
	ldr	r3, .L889+180
	str	r3, [sp, #24]
	ldr	r3, .L889+184
	str	r3, [sp, #20]
	ldr	r3, .L889+188
	str	r3, [sp, #16]
	ldr	r3, .L889+192
	str	r3, [sp, #12]
	ldr	r3, .L889+196
	str	r3, [sp, #8]
	ldr	r3, .L889+200
	str	r3, [sp, #4]
	ldr	r3, .L889+204
	str	r3, [sp]
	ldr	r3, .L889+208
	bl	seq_printf
	ldr	r0, [r4, #208]
	bl	strof_bool
	ldr	r3, [r4, #224]
	mov	r2, r6
	ldr	r1, .L889+212
	str	r3, [sp, #28]
	ldr	r3, [r4, #220]
	str	r3, [sp, #24]
	ldr	r3, [r4, #216]
	str	r3, [sp, #20]
	ldr	r3, [r4, #212]
	str	r3, [sp, #16]
	ldr	r3, [r4, #240]
	str	r3, [sp, #12]
	ldr	r3, [r4, #236]
	str	r3, [sp, #8]
	ldr	r3, [r4, #232]
	str	r3, [sp, #4]
	ldr	r3, [r4, #228]
	str	r3, [sp]
	mov	r3, r0
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L889+216
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L889+220
	mov	r0, r5
	ldr	r2, .L889+8
	ldr	r1, .L889+224
	str	r3, [sp, #4]
	ldr	r3, .L889+228
	str	r3, [sp]
	ldr	r3, .L889+208
	bl	seq_printf
	ldr	r8, [r4, #200]
	mov	r0, r8
	cmp	r8, #0
	beq	.L884
	bl	strof_bool
	movs	r3, #0
	ldr	r1, .L889+232
	mov	r2, r6
	str	r3, [sp, #4]
	ldr	r3, [r4, #204]
	str	r3, [sp]
	mov	r3, r0
.L888:
	mov	r0, r5
	bl	seq_printf
	mov	r0, r5
	ldr	r1, .L889+236
	bl	seq_puts
	ldr	r3, .L889+240
	mov	r0, r5
	ldr	r2, .L889+8
	ldr	r1, .L889+244
	str	r3, [sp, #20]
	ldr	r3, .L889+248
	strd	r10, r7, [sp, #24]
	str	r3, [sp, #16]
	ldr	r3, .L889+252
	str	r3, [sp, #12]
	ldr	r3, .L889+256
	str	r3, [sp, #8]
	ldr	r3, .L889+260
	str	r3, [sp, #4]
	ldr	r3, .L889+264
	str	r3, [sp]
	ldr	r3, .L889+268
	bl	seq_printf
	ldr	r1, [r4, #492]
	mov	r0, r5
	asrs	r2, r1, #31
	ldr	r3, [sp, #52]
	smull	r10, fp, r1, r3
	ldr	r1, [r4, #652]
	str	r1, [sp, #28]
	ldr	r1, [r4, #648]
	str	r1, [sp, #24]
	ldr	r1, [r4, #644]
	str	r1, [sp, #20]
	ldr	r1, [r4, #72]
	str	r1, [sp, #16]
	ldr	r1, [r4, #496]
	b	.L890
.L891:
	.align	2
.L889:
	.word	.LC100
	.word	.LC104
	.word	.LC102
	.word	.LC103
	.word	.LC105
	.word	.LC106
	.word	.LC107
	.word	.LC108
	.word	.LC109
	.word	.LC110
	.word	.LC101
	.word	274877907
	.word	.LC111
	.word	.LC112
	.word	.LC114
	.word	.LC120
	.word	.LC117
	.word	.LC116
	.word	.LC115
	.word	.LC122
	.word	.LC121
	.word	.LC123
	.word	.LC126
	.word	.LC125
	.word	.LC127
	.word	.LC128
	.word	.LC124
	.word	.LC129
	.word	.LC130
	.word	.LC133
	.word	.LC132
	.word	.LC134
	.word	.LC135
	.word	.LC131
	.word	.LC136
	.word	.LC137
	.word	.LC140
	.word	.LC139
	.word	.LC141
	.word	.LC142
	.word	.LC138
	.word	.LC143
	.word	.LC166
	.word	.LC169
	.word	.LC168
	.word	.LC170
	.word	.LC171
	.word	.LC172
	.word	.LC173
	.word	.LC174
	.word	.LC175
	.word	.LC176
	.word	.LC167
	.word	.LC177
	.word	.LC178
	.word	.LC180
	.word	.LC179
	.word	.LC181
	.word	.LC182
	.word	.LC184
	.word	.LC187
	.word	.LC186
	.word	.LC188
	.word	.LC189
	.word	.LC190
	.word	.LC191
	.word	.LC192
	.word	.LC185
	.word	.LC118
	.word	.LC119
.L890:
	smull	r8, r9, r1, r3
	asrs	r1, r1, #31
	ldr	r3, [sp, #48]
	str	r3, [sp, #8]
	rsb	r3, r2, fp, asr #6
	rsb	r1, r1, r9, asr #6
	str	r1, [sp, #12]
	ldr	r1, [r4, #544]
	mov	r2, r6
	str	r1, [sp, #4]
	movs	r1, #0
	str	r1, [sp]
	ldr	r1, .L892
	bl	seq_printf
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L876:
	bl	__aeabi_idiv
	mov	r10, r0
	ldrd	r0, r1, [r4, #28]
	bl	__aeabi_idiv
	mov	fp, r0
	mov	r0, r9
	bl	strof_rc_mode
	ldr	r3, .L892+4
	mov	r2, r6
	ldr	r1, .L892+8
	strd	r3, r3, [sp, #20]
	ldr	r3, [sp, #48]
	strd	r10, fp, [sp, #4]
	str	r8, [sp]
	strd	r0, r3, [sp, #12]
	mov	r3, r7
	b	.L886
.L878:
	bl	strof_gop_mode
	ldr	r3, .L892+4
	mov	r2, r6
	ldr	r1, .L892+12
	strd	r3, r3, [sp, #4]
	ldr	r3, [r4, #112]
	str	r3, [sp]
	mov	r3, r0
	b	.L887
.L880:
	mov	r0, r5
	ldr	r1, .L892+16
	bl	seq_puts
	ldr	r3, .L892+20
	mov	r0, r5
	ldr	r2, .L892+24
	ldr	r1, .L892+28
	str	r3, [sp, #36]
	ldr	r3, .L892+32
	strd	r10, r8, [sp, #8]
	str	r7, [sp, #4]
	str	r3, [sp, #32]
	ldr	r3, .L892+36
	str	r3, [sp, #28]
	ldr	r3, .L892+40
	str	r3, [sp, #24]
	ldr	r3, .L892+44
	strd	r9, r3, [sp, #16]
	ldr	r3, .L892+48
	str	r3, [sp]
	ldr	r3, .L892+52
	bl	seq_printf
	ldr	r3, [r4, #152]
	mov	r2, r6
	ldr	r1, .L892+56
	mov	r0, r5
	str	r3, [sp, #36]
	ldr	r3, [r4, #124]
	str	r3, [sp, #32]
	ldr	r3, [r4, #132]
	str	r3, [sp, #28]
	ldr	r3, [r4, #120]
	str	r3, [sp, #24]
	ldr	r3, [r4, #128]
	str	r3, [sp, #20]
	ldr	r3, [r4, #108]
	str	r3, [sp, #16]
	ldr	r3, [r4, #104]
	str	r3, [sp, #12]
	ldr	r3, [r4, #100]
	str	r3, [sp, #8]
	ldr	r3, [r4, #96]
	str	r3, [sp, #4]
	ldr	r3, [r4, #64]
	str	r3, [sp]
	ldr	r3, [r4, #68]
	bl	seq_printf
	b	.L883
.L881:
	ldrd	r3, r1, [r4, #52]
	movs	r0, #100
	muls	r0, r3, r0
	bl	__aeabi_idiv
	ldr	r1, .L892+60
	mov	fp, r0
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L892+20
	mov	r0, r5
	ldr	r2, .L892+24
	ldr	r1, .L892+64
	str	r3, [sp, #40]
	ldr	r3, .L892+32
	strd	r10, r8, [sp, #12]
	str	r7, [sp, #8]
	str	r3, [sp, #36]
	ldr	r3, .L892+36
	str	r3, [sp, #32]
	ldr	r3, .L892+40
	str	r3, [sp, #28]
	ldr	r3, .L892+44
	strd	r9, r3, [sp, #20]
	ldr	r3, .L892+48
	str	r3, [sp, #4]
	ldr	r3, .L892+52
	str	r3, [sp]
	ldr	r3, .L892+68
	bl	seq_printf
	ldr	r3, [r4, #152]
	mov	r2, r6
	ldr	r1, .L892+72
	mov	r0, r5
	str	r3, [sp, #40]
	ldr	r3, [r4, #124]
	str	r3, [sp, #36]
	ldr	r3, [r4, #132]
	str	r3, [sp, #32]
	ldr	r3, [r4, #120]
	str	r3, [sp, #28]
	ldr	r3, [r4, #128]
	str	r3, [sp, #24]
	ldr	r3, [r4, #108]
	str	r3, [sp, #20]
	ldr	r3, [r4, #104]
	str	r3, [sp, #16]
	ldr	r3, [r4, #100]
	str	r3, [sp, #12]
	ldr	r3, [r4, #96]
	str	r3, [sp, #8]
	ldr	r3, [r4, #64]
	str	r3, [sp, #4]
	ldr	r3, [r4, #68]
	str	r3, [sp]
	mov	r3, fp
	bl	seq_printf
	b	.L883
.L882:
	mov	r0, r5
	ldr	r1, .L892+76
	bl	seq_puts
	ldr	r3, .L892+80
	mov	r0, r5
	ldr	r2, .L892+24
	ldr	r1, .L892+84
	str	r3, [sp, #8]
	ldr	r3, .L892+88
	str	r3, [sp, #4]
	ldr	r3, .L892+92
	str	r3, [sp]
	ldr	r3, .L892+96
	bl	seq_printf
	movs	r3, #0
	mov	r2, r6
	ldr	r1, .L892+100
	mov	r0, r5
	str	r3, [sp, #8]
	ldr	r3, [r4, #504]
	str	r3, [sp, #4]
	ldr	r3, [r4, #500]
	str	r3, [sp]
	ldr	r3, [r4, #160]
	bl	seq_printf
	b	.L883
.L884:
	bl	strof_bool
	ldr	r3, .L892+4
	mov	r2, r6
	ldr	r1, .L892+104
	str	r3, [sp]
	mov	r3, r0
	str	r8, [sp, #4]
	b	.L888
.L893:
	.align	2
.L892:
	.word	.LC193
	.word	.LC112
	.word	.LC113
	.word	.LC144
	.word	.LC145
	.word	.LC148
	.word	.LC102
	.word	.LC147
	.word	.LC149
	.word	.LC150
	.word	.LC151
	.word	.LC152
	.word	.LC153
	.word	.LC146
	.word	.LC154
	.word	.LC155
	.word	.LC157
	.word	.LC156
	.word	.LC158
	.word	.LC159
	.word	.LC162
	.word	.LC161
	.word	.LC163
	.word	.LC164
	.word	.LC160
	.word	.LC165
	.word	.LC183
	.fnend
	.size	rc_model_v2_proc_show, .-rc_model_v2_proc_show
	.global	intra_qp_map
	.global	bit2percent
	.global	max_ip_qp_dealt
	.global	mean_qp2scale
	.global	tab_lnx
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.25463, %object
	.size	__func__.25463, 24
__func__.25463:
	.ascii	"bits_model_param_deinit\000"
	.type	__func__.25477, %object
	.size	__func__.25477, 14
__func__.25477:
	.ascii	"bits_frm_init\000"
	.type	__FUNCTION__.25482, %object
	.size	__FUNCTION__.25482, 14
__FUNCTION__.25482:
	.ascii	"bits_frm_init\000"
	.type	__func__.25509, %object
	.size	__func__.25509, 18
__func__.25509:
	.ascii	"bits_model_update\000"
	.type	__func__.25527, %object
	.size	__func__.25527, 17
__func__.25527:
	.ascii	"bits_model_alloc\000"
	.type	__FUNCTION__.25529, %object
	.size	__FUNCTION__.25529, 17
__FUNCTION__.25529:
	.ascii	"bits_model_alloc\000"
	.type	__func__.25580, %object
	.size	__func__.25580, 15
__func__.25580:
	.ascii	"calc_cbr_ratio\000"
	.type	__FUNCTION__.25581, %object
	.size	__FUNCTION__.25581, 15
__FUNCTION__.25581:
	.ascii	"calc_cbr_ratio\000"
	.type	__func__.25631, %object
	.size	__func__.25631, 15
__func__.25631:
	.ascii	"calc_vbr_ratio\000"
	.type	__func__.25551, %object
	.size	__func__.25551, 18
__func__.25551:
	.ascii	"calc_next_i_ratio\000"
	.space	1
	.type	max_i_delta_qp, %object
	.size	max_i_delta_qp, 204
max_i_delta_qp:
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	640
	.word	576
	.word	576
	.word	512
	.word	512
	.word	448
	.word	448
	.word	384
	.word	384
	.word	320
	.word	320
	.word	320
	.word	256
	.word	256
	.word	256
	.word	192
	.word	192
	.word	192
	.word	192
	.word	192
	.word	128
	.word	128
	.word	128
	.word	128
	.word	128
	.word	128
	.word	64
	.word	64
	.word	64
	.word	64
	.word	64
	.word	64
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.type	__func__.25562, %object
	.size	__func__.25562, 17
__func__.25562:
	.ascii	"calc_debreath_qp\000"
	.type	strength_map.25561, %object
	.size	strength_map.25561, 36
strength_map.25561:
	.byte	0
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	3
	.byte	3
	.byte	3
	.byte	4
	.byte	4
	.byte	4
	.byte	5
	.byte	5
	.byte	5
	.byte	6
	.byte	6
	.byte	6
	.byte	7
	.byte	7
	.byte	7
	.byte	8
	.byte	8
	.byte	8
	.byte	9
	.byte	9
	.byte	9
	.byte	10
	.byte	10
	.byte	10
	.byte	11
	.byte	11
	.byte	11
	.byte	12
	.byte	12
	.byte	12
	.type	__func__.25589, %object
	.size	__func__.25589, 27
__func__.25589:
	.ascii	"reenc_calc_super_frm_ratio\000"
	.type	__func__.25609, %object
	.size	__func__.25609, 21
__func__.25609:
	.ascii	"reenc_calc_cbr_ratio\000"
	.type	__func__.25650, %object
	.size	__func__.25650, 21
__func__.25650:
	.ascii	"reenc_calc_vbr_ratio\000"
	.type	__func__.25671, %object
	.size	__func__.25671, 18
__func__.25671:
	.ascii	"moving_ratio_calc\000"
	.type	__func__.25702, %object
	.size	__func__.25702, 16
__func__.25702:
	.ascii	"calc_avbr_ratio\000"
	.type	__func__.25711, %object
	.size	__func__.25711, 16
__func__.25711:
	.ascii	"bits_model_init\000"
	.type	__func__.25721, %object
	.size	__func__.25721, 16
__func__.25721:
	.ascii	"bits_mode_reset\000"
	.type	__func__.25730, %object
	.size	__func__.25730, 18
__func__.25730:
	.ascii	"check_super_frame\000"
	.type	__func__.25743, %object
	.size	__func__.25743, 13
__func__.25743:
	.ascii	"check_re_enc\000"
	.type	__func__.25753, %object
	.size	__func__.25753, 17
__func__.25753:
	.ascii	"rc_model_v2_init\000"
	.type	__func__.25758, %object
	.size	__func__.25758, 19
__func__.25758:
	.ascii	"rc_model_v2_deinit\000"
	.type	__func__.25767, %object
	.size	__func__.25767, 18
__func__.25767:
	.ascii	"rc_model_v2_start\000"
	.type	__func__.25803, %object
	.size	__func__.25803, 22
__func__.25803:
	.ascii	"rc_model_v2_hal_start\000"
	.space	1
	.type	mb_num, %object
	.size	mb_num, 36
mb_num:
	.word	0
	.word	200
	.word	700
	.word	1200
	.word	2000
	.word	4000
	.word	8000
	.word	16000
	.word	20000
	.type	qscale2qp, %object
	.size	qscale2qp, 96
qscale2qp:
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	16
	.byte	18
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	24
	.byte	25
	.byte	25
	.byte	26
	.byte	27
	.byte	28
	.byte	28
	.byte	29
	.byte	29
	.byte	30
	.byte	30
	.byte	30
	.byte	31
	.byte	31
	.byte	32
	.byte	32
	.byte	33
	.byte	33
	.byte	33
	.byte	34
	.byte	34
	.byte	34
	.byte	34
	.byte	35
	.byte	35
	.byte	35
	.byte	36
	.byte	36
	.byte	36
	.byte	36
	.byte	36
	.byte	37
	.byte	37
	.byte	37
	.byte	37
	.byte	38
	.byte	38
	.byte	38
	.byte	38
	.byte	38
	.byte	39
	.byte	39
	.byte	39
	.byte	39
	.byte	39
	.byte	39
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	41
	.byte	41
	.byte	41
	.byte	41
	.byte	41
	.byte	41
	.byte	41
	.byte	42
	.byte	42
	.byte	42
	.byte	42
	.byte	42
	.byte	42
	.byte	42
	.byte	42
	.byte	43
	.byte	43
	.byte	43
	.byte	43
	.byte	43
	.byte	43
	.byte	43
	.byte	43
	.byte	44
	.byte	44
	.byte	44
	.byte	44
	.byte	44
	.byte	44
	.byte	44
	.byte	44
	.byte	45
	.byte	45
	.byte	45
	.byte	45
	.type	tab_bit, %object
	.size	tab_bit, 36
tab_bit:
	.word	3780
	.word	3570
	.word	3150
	.word	2940
	.word	2730
	.word	3780
	.word	2100
	.word	1680
	.word	2100
	.type	max_ip_qp_dealt, %object
	.size	max_ip_qp_dealt, 8
max_ip_qp_dealt:
	.byte	7
	.byte	7
	.byte	6
	.byte	6
	.byte	5
	.byte	5
	.byte	4
	.byte	4
	.type	__func__.25822, %object
	.size	__func__.25822, 20
__func__.25822:
	.ascii	"rc_model_v2_hal_end\000"
	.type	__func__.25831, %object
	.size	__func__.25831, 24
__func__.25831:
	.ascii	"rc_model_v2_check_reenc\000"
	.type	__func__.25845, %object
	.size	__func__.25845, 16
__func__.25845:
	.ascii	"rc_model_v2_end\000"
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	tab_lnx, %object
	.size	tab_lnx, 256
tab_lnx:
	.word	-1216
	.word	-972
	.word	-830
	.word	-729
	.word	-651
	.word	-587
	.word	-533
	.word	-486
	.word	-445
	.word	-408
	.word	-374
	.word	-344
	.word	-316
	.word	-290
	.word	-265
	.word	-243
	.word	-221
	.word	-201
	.word	-182
	.word	-164
	.word	-147
	.word	-131
	.word	-115
	.word	-100
	.word	-86
	.word	-72
	.word	-59
	.word	-46
	.word	-34
	.word	-22
	.word	-11
	.word	0
	.word	10
	.word	21
	.word	31
	.word	41
	.word	50
	.word	60
	.word	69
	.word	78
	.word	86
	.word	95
	.word	87
	.word	103
	.word	111
	.word	119
	.word	127
	.word	134
	.word	142
	.word	149
	.word	156
	.word	163
	.word	170
	.word	177
	.word	183
	.word	190
	.word	196
	.word	202
	.word	208
	.word	214
	.word	220
	.word	226
	.word	232
	.word	237
	.type	intra_qp_map, %object
	.size	intra_qp_map, 8
intra_qp_map:
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.type	mean_qp2scale, %object
	.size	mean_qp2scale, 64
mean_qp2scale:
	.word	14
	.word	15
	.word	16
	.word	18
	.word	20
	.word	22
	.word	25
	.word	28
	.word	32
	.word	36
	.word	40
	.word	44
	.word	50
	.word	56
	.word	64
	.word	72
	.type	bit2percent, %object
	.size	bit2percent, 400
bit2percent:
	.word	99
	.word	99
	.word	99
	.word	99
	.word	99
	.word	99
	.word	99
	.word	99
	.word	99
	.word	99
	.word	99
	.word	95
	.word	90
	.word	85
	.word	80
	.word	75
	.word	72
	.word	69
	.word	66
	.word	64
	.word	62
	.word	59
	.word	58
	.word	57
	.word	56
	.word	55
	.word	54
	.word	53
	.word	52
	.word	51
	.word	50
	.word	49
	.word	48
	.word	47
	.word	47
	.word	46
	.word	46
	.word	45
	.word	45
	.word	44
	.word	44
	.word	43
	.word	43
	.word	43
	.word	42
	.word	42
	.word	42
	.word	41
	.word	41
	.word	41
	.word	40
	.word	40
	.word	40
	.word	39
	.word	39
	.word	39
	.word	38
	.word	38
	.word	38
	.word	37
	.word	37
	.word	37
	.word	37
	.word	37
	.word	37
	.word	37
	.word	36
	.word	36
	.word	36
	.word	36
	.word	36
	.word	36
	.word	36
	.word	35
	.word	35
	.word	35
	.word	34
	.word	34
	.word	34
	.word	34
	.word	34
	.word	33
	.word	33
	.word	33
	.word	33
	.word	33
	.word	33
	.word	33
	.word	33
	.word	33
	.word	32
	.word	32
	.word	32
	.word	32
	.word	32
	.word	32
	.word	32
	.word	32
	.word	32
	.word	32
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC1:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC2:
	.ascii	"\0013%d: i_bit init fail\000"
.LC3:
	.ascii	"\0013%d: vi_bit init fail\000"
.LC4:
	.ascii	"\0013%d: p_bit init fail\000"
.LC5:
	.ascii	"\0013%d: pre_p_bit init fail\000"
.LC6:
	.ascii	"\0013%d: pre_i_bit init fail\000"
.LC7:
	.ascii	"\0013%d: pre_i_mean_qp init fail\000"
.LC8:
	.ascii	"\0013%d: madi init fail\000"
.LC9:
	.ascii	"\0013%d: madp init fail\000"
.LC10:
	.ascii	"\0013%d: stat_rate init fail fps_out_num %d\000"
.LC11:
	.ascii	"\0013%d: stat_bits init fail stat_len %d\000"
.LC12:
	.ascii	"\0013%d: gop_bits init fail gop_len %d\000"
.LC13:
	.ascii	"\0013%d: motion_level init fail mad_len %d\000"
.LC14:
	.ascii	"\0013%d: complex_level init fail mad_len %d\000"
.LC15:
	.ascii	"usr_cfg->vgop > 1\000"
.LC16:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC17:
	.ascii	"\0016%s:%d: p_sumbits %d i_sumbits %d vi_sumbits %d"
	.ascii	"\012\000"
.LC18:
	.ascii	"\0016%s:%d: i_sumbits %d p_sumbits %d i_scale %d\012"
	.ascii	"\000"
.LC19:
	.ascii	"\0016%s:%d: frame_type %d max_i_prop %d i_scale %d "
	.ascii	"total_bits %lld\012\000"
.LC20:
	.ascii	"\0016%s:%d: found zero alloc bits\012\000"
.LC21:
	.ascii	"\0016%s:%d: total_bits %lld, i_scale %d, gop_len %d"
	.ascii	", vi_num %d, vi_scale %d\000"
.LC22:
	.ascii	"\0016%s:%d: gop_total_bits %lld, i_sumbits %d, p_su"
	.ascii	"mbits %d, vgop %d igop %d\000"
.LC23:
	.ascii	"\0016%s:%d: i_scale  %d, total_bits %lld\000"
.LC24:
	.ascii	"\0016%s:%d: alloc bits max then super_bit_thr %d\000"
.LC25:
	.ascii	"alloc_bits\000"
.LC26:
	.ascii	"r_bits\000"
.LC27:
	.ascii	"\0016%s:%d: %10s|%10s|%10s|%10s|%10s|%8s\000"
.LC28:
	.ascii	"watl\000"
.LC29:
	.ascii	"target_bps\000"
.LC30:
	.ascii	"p_ins_bps\000"
.LC31:
	.ascii	"ins_bps\000"
.LC32:
	.ascii	"t_bits\000"
.LC33:
	.ascii	"\0016%s:%d: %10d %10d %10d %10d %10d %8d\000"
.LC34:
	.ascii	"target_bps > 0\000"
.LC35:
	.ascii	"diff_ratio\000"
.LC36:
	.ascii	"\0016%s:%d: %10s|%10s|%10s|%10s|%10s|%10s\000"
.LC37:
	.ascii	"cur_qp_s\000"
.LC38:
	.ascii	"next_ratio\000"
.LC39:
	.ascii	"wl_ratio\000"
.LC40:
	.ascii	"bps_ratio\000"
.LC41:
	.ascii	"ins_ratio\000"
.LC42:
	.ascii	"\0016%s:%d: %10d %10d %10d %10d %10d|%10d\000"
.LC43:
	.ascii	"max_bps\000"
.LC44:
	.ascii	"bps_ch\000"
.LC45:
	.ascii	"\0016%s:%d: %10d %10d %10d %10d %10d %10d\000"
.LC46:
	.ascii	"\0016%s:%d: %10s|%10s|%10s|%10s|%10s\000"
.LC47:
	.ascii	"\0016%s:%d: %10d %10d %10d %10d|%10d\000"
.LC48:
	.ascii	"\0016%s:%d: ctx->next_i_ratio %d\000"
.LC49:
	.ascii	"\0016%s:%d: i start_qp %d, qp_start_sum = %d, intra"
	.ascii	"_lv4_prop %d\000"
.LC50:
	.ascii	"\0016%s:%d: cbr target_bit %d real_bit %d reenc nex"
	.ascii	"t ratio %d\000"
.LC51:
	.ascii	"\0016%s:%d: vbr reenc next ratio %d\000"
.LC52:
	.ascii	"\0016%s:%d: pre_mean_qp = %d, ctx->pre_mean_qp %d\000"
.LC53:
	.ascii	"\0016%s:%d: scale = %d, pre_I_bit %d\000"
.LC54:
	.ascii	"\0016%s:%d: pbit_sum %d,madi_sum = %d, madp_sum = %"
	.ascii	"d\000"
.LC55:
	.ascii	"\0016%s:%d: means qp percent %d min_still_percent %"
	.ascii	"d\000"
.LC56:
	.ascii	"\0016%s:%d: percent_a = %d percent_b %d\000"
.LC57:
	.ascii	"\0016%s:%d: mv_ratio = %d\000"
.LC58:
	.ascii	"\0016%s:%d: mad_ratio = %d\000"
.LC59:
	.ascii	"\0016%s:%d: moving_ratio = %d, motion_sensitivity ="
	.ascii	" %d\000"
.LC60:
	.ascii	"\0016%s:%d: percent %d mad_ratio %d hr_ratio %d, mo"
	.ascii	"ving_ratio %d\000"
.LC61:
	.ascii	"\0016%s:%d: final moving_ratio = %d\000"
.LC62:
	.ascii	"m_ratio\000"
.LC63:
	.ascii	"\0016%s:%d: %10d %10d %10d %10d %10d\000"
.LC64:
	.ascii	"\0016%s:%d: %10s|%10s|%10s|%10s|%10s|%10s|%10s\000"
.LC65:
	.ascii	"qratio\000"
.LC66:
	.ascii	"ag_ratio\000"
.LC67:
	.ascii	"rg_ratio\000"
.LC68:
	.ascii	"\0016%s:%d: %10d %10d %10d %10d %10d %10d %10d\000"
.LC69:
	.ascii	"\0016%s:%d: min_i_bit_prop %d max_i_bit_prop %d, in"
	.ascii	"it_ip_ratio %d\000"
.LC70:
	.ascii	"\0016%s:%d: infinte gop, set default for rc bit cal"
	.ascii	"c\012\000"
.LC71:
	.ascii	"\0016%s:%d: all intra gop \012\000"
.LC72:
	.ascii	"\0016%s:%d: min_still_percent  %d\000"
.LC73:
	.ascii	"\0016%d: rc mode set error\000"
.LC74:
	.ascii	"\0016%s:%d: gop %d total bit %lld per_frame %d stat"
	.ascii	"istics time %d second\012\000"
.LC75:
	.ascii	"\0016%s:%d: super frame drop current frame\000"
.LC76:
	.ascii	"\0016%s:%d: reenc check target_bps %d last_ins_bps "
	.ascii	"%d ins_bps %d\000"
.LC77:
	.ascii	"\0016%s:%d: drop mode %d frame_type %d\012\000"
.LC78:
	.ascii	"\0016%s:%d: drop mode %d check max_bps %d bit_thr %"
	.ascii	"d ins_bps %d\000"
.LC79:
	.ascii	"\0016%s:%d: leave %p ret %d\012\000"
.LC80:
	.ascii	"\0016%d: invalid fix %d qp found set default qp 26\012"
	.ascii	"\000"
.LC81:
	.ascii	"\0016%s:%d: seq_idx %d intra %d\012\000"
.LC82:
	.ascii	"\0016%s:%d: bitrate [%d : %d : %d]\012\000"
.LC83:
	.ascii	"\0016%s:%d: quality [%d : %d : %d]\012\000"
.LC84:
	.ascii	"\0016%s:%d: enter p %p task %p\012\000"
.LC85:
	.ascii	"\0016%d: init qp not set on fix qp mode, use defaul"
	.ascii	"t qp\012\000"
.LC86:
	.ascii	"\0016%s:%d: qp: start %2d cur_scale %d next_ratio %"
	.ascii	"d reenc %d\012\000"
.LC87:
	.ascii	"\0016%s:%d: qp prev %d:%d curr %d - %d (max %d) -> "
	.ascii	"%d reenc %d\012\000"
.LC88:
	.ascii	"\0016%s:%d: qp %d -> %d\012\000"
.LC89:
	.ascii	"\0016%s:%d: qp %d -> %d (vi)\012\000"
.LC90:
	.ascii	"\0016%s:%d: hier_qp: layer %d delta %d\000"
.LC91:
	.ascii	"\0016%s:%d: bitrate [%d : %d : %d] -> [%d : %d : %d"
	.ascii	"]\012\000"
.LC92:
	.ascii	"\0016%s:%d: quality [%d : %d : %d] -> [%d : %d : %d"
	.ascii	"]\012\000"
.LC93:
	.ascii	"\0016%s:%d: enter ctx %p task %p\012\000"
.LC94:
	.ascii	"\0016%s:%d: enter ctx %p cfg %p\012\000"
.LC95:
	.ascii	"\0016%s:%d: reenc drop_mode %d drop_cnt %d\012\000"
.LC96:
	.ascii	"\0016%s:%d: drop\012\000"
.LC97:
	.ascii	"\0016%s:%d: force_pskip\012\000"
.LC98:
	.ascii	"\0016%s:%d: bits_mode_update real_bit %d\000"
.LC99:
	.ascii	"\0016%s:%d: motion_level %u, complex_level %u\012\000"
.LC100:
	.ascii	"\012---------RC base param 1-----------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC101:
	.ascii	"Gop\000"
.LC102:
	.ascii	"ChnId\000"
.LC103:
	.ascii	"%7s%7s%8s%6s%6s%8s%10s%5s%5s \012\000"
.LC104:
	.ascii	"PQp\000"
.LC105:
	.ascii	"IQp\000"
.LC106:
	.ascii	"Br(kbps)\000"
.LC107:
	.ascii	"RcMode\000"
.LC108:
	.ascii	"TrgFr\000"
.LC109:
	.ascii	"ViFr\000"
.LC110:
	.ascii	"StatTm\000"
.LC111:
	.ascii	"%7d%7u%8u%6u%6u%8s%10s%5u%5u \012\000"
.LC112:
	.ascii	"N/A\000"
.LC113:
	.ascii	"%7d%7u%8u%6u%6u%8s%10u%5s%5s \012\000"
.LC114:
	.ascii	"\012---------RC base param 2-----------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC115:
	.ascii	"MinQp\000"
.LC116:
	.ascii	"%7s%8s%8s%8s%8s%12s \012\000"
.LC117:
	.ascii	"EnableIdr\000"
.LC118:
	.ascii	"MaxIQp\000"
.LC119:
	.ascii	"MinIQp\000"
.LC120:
	.ascii	"MaxQp\000"
.LC121:
	.ascii	"%7d%8d%8d%8d%8d%12s \012\000"
.LC122:
	.ascii	"Y\000"
.LC123:
	.ascii	"\012---------RC run comm param 1-------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC124:
	.ascii	"bLost\000"
.LC125:
	.ascii	"%7s%8s%12s%12s%10s \012\000"
.LC126:
	.ascii	"EncGap\000"
.LC127:
	.ascii	"LostFrmStr\000"
.LC128:
	.ascii	"LostThr\000"
.LC129:
	.ascii	"%7d%8s%12u%12d%10u \012\000"
.LC130:
	.ascii	"\012---------RC run comm param 2-------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC131:
	.ascii	"SprFrmMod\000"
.LC132:
	.ascii	"%7s%12s%12s%12s%12s \012\000"
.LC133:
	.ascii	"RCPriority\000"
.LC134:
	.ascii	"SprPFrm\000"
.LC135:
	.ascii	"SprIFrm\000"
.LC136:
	.ascii	"%7d%12s%12u%12u%12u \012\000"
.LC137:
	.ascii	"\012---------RC gop mode attr----------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC138:
	.ascii	"GopMode\000"
.LC139:
	.ascii	"%7s%10s%10s%12s%10s \012\000"
.LC140:
	.ascii	"ViQpDelta\000"
.LC141:
	.ascii	"BgInterval\000"
.LC142:
	.ascii	"IpQpDelta\000"
.LC143:
	.ascii	"%7d%10s%10d%12u%10d\012\000"
.LC144:
	.ascii	"%7d%10s%10d%12s%10s\012\000"
.LC145:
	.ascii	"\012---------RC run cbr param----------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC146:
	.ascii	"MinIprop\000"
.LC147:
	.ascii	"%7s%10s%10s%8s%8s%8s%8s%10s%10s%10s%10s%15s \012\000"
.LC148:
	.ascii	"MaxReEncTimes\000"
.LC149:
	.ascii	"FrmMinIQp\000"
.LC150:
	.ascii	"FrmMaxIQp\000"
.LC151:
	.ascii	"FrmMinQp\000"
.LC152:
	.ascii	"FrmMaxQp\000"
.LC153:
	.ascii	"MaxIprop\000"
.LC154:
	.ascii	"%7d%10u%10u%8u%8u%8u%8u%10d%10d%10d%10d%15d\012\000"
.LC155:
	.ascii	"\012---------RC run vbr common param---------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC156:
	.ascii	"ChgPs\000"
.LC157:
	.ascii	"%7s%8s%10s%10s%8s%8s%8s%8s%10s%10s%10s%10s%15s \012"
	.ascii	"\000"
.LC158:
	.ascii	"%7d%8d%10u%10u%8u%8u%8u%8u%10d%10d%10d%10d%15d\012\000"
.LC159:
	.ascii	"\012---------RC run avbr param---------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC160:
	.ascii	"MaxStillQP\000"
.LC161:
	.ascii	"%7s%15s%15s%15s%15s \012\000"
.LC162:
	.ascii	"MinQpDelta\000"
.LC163:
	.ascii	"MinPercent\000"
.LC164:
	.ascii	"MotionSensi\000"
.LC165:
	.ascii	"%7d%15u%15u%15d%15u \012\000"
.LC166:
	.ascii	"\012--------RC HierarchicalQp INFO-----------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC167:
	.ascii	"bEnable\000"
.LC168:
	.ascii	"%7s%10s%12s%12s%12s%12s%12s%12s%12s%12s\012\000"
.LC169:
	.ascii	"QpDelta[3]\000"
.LC170:
	.ascii	"QpDelta[2]\000"
.LC171:
	.ascii	"QpDelta[1]\000"
.LC172:
	.ascii	"QpDelta[0]\000"
.LC173:
	.ascii	"FrameNum[3]\000"
.LC174:
	.ascii	"FrameNum[2]\000"
.LC175:
	.ascii	"FrameNum[1]\000"
.LC176:
	.ascii	"FrameNum[0]\000"
.LC177:
	.ascii	"%7d%10s%12d%12d%12d%12d%12d%12d%12d%12d\012\000"
.LC178:
	.ascii	"\012--------RC debreath_effect info----------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC179:
	.ascii	"%7s%10s%10s%18s\012\000"
.LC180:
	.ascii	"DeBrthEfctCnt\000"
.LC181:
	.ascii	"Strength0\000"
.LC182:
	.ascii	"%7d%10s%10d%18u\012\000"
.LC183:
	.ascii	"%7d%10s%10s%18u\012\000"
.LC184:
	.ascii	"\012--------RC run info1---------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC185:
	.ascii	"InsBr(kbps)\000"
.LC186:
	.ascii	"%7s%14s%8s%8s%12s%12s%10s%10s%8s%8s\012\000"
.LC187:
	.ascii	"StartQp\000"
.LC188:
	.ascii	"IPRatio\000"
.LC189:
	.ascii	"RealBt(kb)\000"
.LC190:
	.ascii	"CfgBt(kb)\000"
.LC191:
	.ascii	"WatL\000"
.LC192:
	.ascii	"InsFr\000"
.LC193:
	.ascii	"%7d%14d%8u%8d%12u%12u%10d%10u%8u%8u\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
