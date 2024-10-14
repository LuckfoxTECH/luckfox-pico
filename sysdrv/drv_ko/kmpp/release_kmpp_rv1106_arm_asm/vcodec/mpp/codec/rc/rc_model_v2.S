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
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_clip, %function
mpp_clip:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r1, r2
	bgt	.L2
	cmp	r1, r0
	bgt	.L3
	cmp	r2, r0
	it	ge
	movge	r2, r0
.L2:
	mov	r0, r2
	bx	lr
.L3:
	mov	r2, r1
	b	.L2
	.fnend
	.size	mpp_clip, .-mpp_clip
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
	ldr	r5, .L65
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L5
	mov	r3, r0
	movs	r2, #73
	ldr	r1, .L65+4
	ldr	r0, .L65+8
	bl	printk
.L5:
	ldr	r0, [r4, #424]
	cbz	r0, .L6
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #424]
.L6:
	ldr	r0, [r4, #460]
	cbz	r0, .L7
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #460]
.L7:
	ldr	r0, [r4, #448]
	cbz	r0, .L8
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #448]
.L8:
	ldr	r0, [r4, #472]
	cbz	r0, .L9
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #472]
.L9:
	ldr	r0, [r4, #476]
	cbz	r0, .L10
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #476]
.L10:
	ldr	r0, [r4, #480]
	cbz	r0, .L11
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #480]
.L11:
	ldr	r0, [r4, #484]
	cbz	r0, .L12
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #484]
.L12:
	ldr	r0, [r4, #488]
	cbz	r0, .L13
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #488]
.L13:
	ldr	r0, [r4, #560]
	cbz	r0, .L14
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #560]
.L14:
	ldr	r0, [r4, #552]
	cbz	r0, .L15
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #552]
.L15:
	ldr	r0, [r4, #556]
	cbz	r0, .L16
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #556]
.L16:
	ldr	r0, [r4, #492]
	cbz	r0, .L17
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #492]
.L17:
	ldr	r0, [r4, #496]
	cbz	r0, .L18
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #496]
.L18:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L19
	mov	r3, r4
	movs	r2, #135
	ldr	r1, .L65+4
	ldr	r0, .L65+12
	bl	printk
.L19:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L66:
	.align	2
.L65:
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
	add	r0, r4, #424
	bl	mpp_data_init_v2
	ldr	r3, [r4, #424]
	cbnz	r3, .L69
	ldr	r0, .L86
	movs	r1, #152
.L85:
	bl	printk
.L84:
	mvn	r0, #3
.L67:
	pop	{r4, r5, r6, pc}
.L69:
	movs	r2, #0
	movs	r1, #2
	add	r0, r4, #448
	bl	mpp_data_init_v2
	ldr	r3, [r4, #448]
	cbnz	r3, .L71
	movs	r1, #158
	ldr	r0, .L86+4
	b	.L85
.L71:
	movs	r2, #0
	movs	r1, #5
	add	r0, r4, #460
	bl	mpp_data_init_v2
	ldr	r3, [r4, #460]
	cbnz	r3, .L72
	movs	r1, #163
	ldr	r0, .L86+8
	b	.L85
.L72:
	movs	r2, #0
	movs	r1, #8
	add	r0, r4, #472
	bl	mpp_data_init_v2
	ldr	r3, [r4, #472]
	cbnz	r3, .L73
	movs	r1, #169
	ldr	r0, .L86+12
	b	.L85
.L73:
	movs	r2, #0
	movs	r1, #2
	add	r0, r4, #476
	bl	mpp_data_init_v2
	ldr	r3, [r4, #476]
	cbnz	r3, .L74
	movs	r1, #175
	ldr	r0, .L86+16
	b	.L85
.L74:
	mov	r2, #-1
	movs	r1, #2
	add	r0, r4, #480
	bl	mpp_data_init_v2
	ldr	r3, [r4, #480]
	cbnz	r3, .L75
	movs	r1, #181
	ldr	r0, .L86+20
	b	.L85
.L75:
	movs	r2, #0
	movs	r1, #8
	add	r0, r4, #484
	bl	mpp_data_init_v2
	ldr	r3, [r4, #484]
	cbnz	r3, .L76
	movs	r1, #186
	ldr	r0, .L86+24
	b	.L85
.L76:
	movs	r2, #0
	movs	r1, #8
	add	r0, r4, #488
	bl	mpp_data_init_v2
	ldr	r3, [r4, #488]
	cbnz	r3, .L77
	movs	r1, #191
	ldr	r0, .L86+28
	b	.L85
.L77:
	movs	r2, #0
	ldr	r1, [r4, #28]
	add	r0, r4, #560
	bl	mpp_data_init_v2
	ldr	r3, [r4, #560]
	cbnz	r3, .L78
	ldr	r2, [r4, #28]
	movs	r1, #196
	ldr	r0, .L86+32
.L83:
	bl	printk
	b	.L84
.L78:
	ldr	r2, [r4, #400]
	mov	r1, r5
	add	r0, r4, #552
	bl	mpp_data_init_v2
	ldr	r3, [r4, #552]
	cbnz	r3, .L79
	mov	r2, r5
	movs	r1, #202
	ldr	r0, .L86+36
	b	.L83
.L79:
	movs	r2, #0
	mov	r1, r6
	add	r0, r4, #556
	bl	mpp_data_init_v2
	ldr	r3, [r4, #556]
	cbnz	r3, .L80
	mov	r2, r6
	movs	r1, #208
	ldr	r0, .L86+40
	b	.L83
.L80:
	movs	r2, #0
	movs	r1, #10
	add	r0, r4, #492
	bl	mpp_data_init_v2
	ldr	r3, [r4, #492]
	cbnz	r3, .L81
	movs	r2, #10
	movs	r1, #214
	ldr	r0, .L86+44
	b	.L83
.L81:
	movs	r2, #0
	movs	r1, #10
	add	r0, r4, #496
	bl	mpp_data_init_v2
	ldr	r3, [r4, #496]
	cbnz	r3, .L82
	movs	r2, #10
	movs	r1, #220
	ldr	r0, .L86+48
	b	.L83
.L82:
	movs	r0, #0
	b	.L67
.L87:
	.align	2
.L86:
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
	ldr	r6, .L109
	.pad #20
	sub	sp, sp, #20
	ldr	r5, [r0, #40]
	ldr	r3, [r6]
	lsls	r7, r3, #31
	bpl	.L89
	mov	r3, r0
	movs	r2, #233
	ldr	r1, .L109+4
	ldr	r0, .L109+8
	bl	printk
.L89:
	ldr	r3, [r4, #36]
	cbz	r3, .L90
	cmp	r3, #1
	beq	.L91
.L92:
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L97
	ldr	r3, [r4, #452]
	movw	r2, #283
	ldr	r1, .L109+4
	ldr	r0, .L109+12
	str	r3, [sp, #4]
	ldr	r3, [r4, #428]
	str	r3, [sp]
	ldr	r3, [r4, #464]
	bl	printk
.L97:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L88
	mov	r3, r4
	mov	r2, #284
	ldr	r1, .L109+4
	ldr	r0, .L109+16
	bl	printk
.L88:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L90:
	ldr	r2, [r4, #72]
	cmp	r5, #1
	ldrd	r7, r1, [r4, #392]
	mov	r3, #16
	str	r3, [r4, #468]
	str	r2, [r4, #432]
	lsl	r0, r7, #4
	bhi	.L93
.L108:
	mov	r5, r0
	mov	r1, r0
	ldr	r0, [r4, #460]
	bl	mpp_data_reset_v2
	ldr	r1, [r4, #432]
	add	r3, r5, r5, lsl #2
	ldr	r0, [r4, #424]
	str	r3, [r4, #464]
	muls	r1, r5, r1
	lsrs	r1, r1, #4
	bl	mpp_data_reset_v2
	ldr	r0, [r4, #432]
	muls	r0, r5, r0
	ubfx	r0, r0, #3, #28
	str	r0, [r4, #428]
	b	.L92
.L93:
	subs	r5, r5, #1
	lsls	r1, r1, #4
	add	r3, sp, #12
	orr	r1, r1, r7, lsr #28
	add	r2, r2, r5, lsl #4
	bl	div_s64_rem
	b	.L108
.L91:
	ldr	r3, [r4, #44]
	cmp	r3, #1
	bgt	.L95
	movs	r1, #255
	ldr	r3, .L109+20
	str	r1, [sp]
	ldr	r2, .L109+24
	ldr	r0, .L109+28
	bl	printk
.L95:
	ldr	r7, [r4, #44]
	mov	r3, #320
	mov	r0, r5
	str	r3, [r4, #432]
	movs	r3, #16
	str	r3, [r4, #468]
	mov	r1, r7
	movs	r3, #32
	str	r3, [r4, #456]
	bl	__aeabi_uidiv
	cmp	r7, r5
	it	ls
	addls	r0, r0, #-1
	subs	r2, r5, r0
	ldrd	r5, r3, [r4, #392]
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
	ldr	r0, [r4, #460]
	bl	mpp_data_reset_v2
	ldr	r1, [r4, #432]
	add	r3, r5, r5, lsl #2
	ldr	r0, [r4, #424]
	str	r3, [r4, #464]
	muls	r1, r5, r1
	lsrs	r1, r1, #4
	bl	mpp_data_reset_v2
	ldr	r1, [r4, #456]
	ldr	r3, [r4, #432]
	ldr	r0, [r4, #448]
	muls	r1, r5, r1
	muls	r3, r5, r3
	lsrs	r1, r1, #4
	ubfx	r3, r3, #3, #28
	str	r3, [r4, #428]
	bl	mpp_data_reset_v2
	ldr	r3, [r4, #456]
	muls	r5, r3, r5
	ubfx	r5, r5, #3, #28
	str	r5, [r4, #452]
	b	.L92
.L110:
	.align	2
.L109:
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
	ldr	r3, [r0, #380]
	mov	r5, r1
	cbz	r3, .L112
	cmp	r3, #2
	bne	.L113
	ldr	r1, [r1, #24]
	ldr	r0, [r0, #476]
	bl	mpp_data_update_v2
	ldr	r1, [r5, #28]
	ldr	r0, [r4, #480]
.L114:
	bl	mpp_data_update_v2
.L113:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L112:
	ldr	r1, [r1, #24]
	ldr	r0, [r0, #472]
	bl	mpp_data_update_v2
	ldr	r1, [r5, #36]
	ldr	r0, [r4, #488]
	b	.L114
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
	ldr	r0, [r0, #556]
	bl	mpp_data_update_v2
	ldr	r0, [r5, #472]
	bl	mpp_data_mean_v2
	mov	r4, r0
	ldr	r0, [r5, #476]
	bl	mpp_data_mean_v2
	asr	r9, r4, #31
	mov	r7, r0
	ldr	r0, [r5, #556]
	bl	mpp_data_sum_v2
	subs	r3, r6, #1
	umull	fp, ip, r3, r4
	mla	ip, r3, r9, ip
	asrs	r1, r0, #31
	strd	r0, [r5, #416]
	adds	r0, fp, r7
	adc	r1, ip, r7, asr #31
	strd	r0, [r5, #408]
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
	ldr	r6, .L136
	mov	r4, r0
	mov	r7, r2
	ldr	r3, [r6]
	lsls	r1, r3, #31
	bpl	.L117
	mov	r3, r0
	mov	r2, #324
	ldr	r1, .L136+4
	ldr	r0, .L136+8
	bl	printk
.L117:
	adds	r1, r5, #0
	ldr	r0, [r4, #560]
	it	ne
	movne	r1, #1
	bl	mpp_data_update_v2
	mov	r1, r5
	ldr	r0, [r4, #552]
	bl	mpp_data_update_v2
	ldr	r0, [r4, #552]
	bl	mpp_data_sum_v2
	ldr	r1, [r4, #60]
	bl	__aeabi_idiv
	ldr	r3, [r4, #568]
	ldr	r1, [r4, #564]
	adds	r2, r5, r3
	ldr	r3, [r4, #400]
	cmp	r2, r1
	ite	gt
	subgt	r3, r1, r3
	suble	r3, r2, r3
	bic	r3, r3, r3, asr #31
	str	r3, [r4, #568]
	ldr	r3, [r4, #380]
	cmp	r3, #2
	str	r0, [r4, #516]
	beq	.L120
	cmp	r3, #3
	beq	.L121
	cbnz	r3, .L123
	mov	r1, r5
	ldr	r0, [r4, #460]
	bl	mpp_data_update_v2
	ldr	r0, [r4, #484]
	mov	r1, r7
	bl	mpp_data_update_v2
	ldr	r0, [r4, #460]
	bl	mpp_data_sum_v2
	cbz	r0, .L124
	str	r0, [r4, #464]
.L125:
	movs	r3, #16
	str	r3, [r4, #468]
	b	.L123
.L120:
	mov	r1, r5
	ldr	r0, [r4, #424]
	bl	mpp_data_update_v2
	ldr	r0, [r4, #424]
	bl	mpp_data_sum_v2
	ldr	r7, [r4, #464]
	lsls	r1, r7, #1
	mov	r5, r0
	str	r0, [r4, #428]
	movs	r0, #80
	muls	r0, r5, r0
	bl	__aeabi_uidiv
	ldr	r3, [r6]
	lsls	r2, r3, #25
	str	r0, [r4, #432]
	bpl	.L123
	strd	r7, r0, [sp]
	mov	r3, r5
	movw	r2, #343
	ldr	r1, .L136+4
	ldr	r0, .L136+12
	bl	printk
.L123:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L126
	mov	r3, r4
	movw	r2, #373
	ldr	r1, .L136+4
	ldr	r0, .L136+16
	bl	printk
.L126:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L124:
	movs	r3, #1
	str	r3, [r4, #464]
	b	.L125
.L121:
	mov	r1, r5
	ldr	r0, [r4, #448]
	bl	mpp_data_update_v2
	ldr	r0, [r4, #448]
	bl	mpp_data_sum_v2
	ldr	r1, [r4, #464]
	movs	r3, #80
	lsls	r1, r1, #1
	str	r0, [r4, #452]
	muls	r0, r3, r0
	bl	__aeabi_uidiv
	cmp	r0, #320
	it	ge
	movge	r0, #320
	cmp	r0, #16
	it	lt
	movlt	r0, #16
	str	r0, [r4, #456]
	b	.L123
.L137:
	.align	2
.L136:
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
	ldr	r1, [r0, #464]
	str	r3, [sp, #28]
	ldr	r3, [r0, #456]
	lsls	r1, r1, #1
	str	r3, [sp, #24]
	ldr	r3, [r0, #428]
	movs	r0, #80
	muls	r0, r3, r0
	bl	__aeabi_uidiv
	ldr	r3, .L183
	ldr	r2, [r3]
	mov	r10, r3
	tst	r2, #1
	mov	r9, r0
	str	r0, [r6, #432]
	mov	r7, r0
	beq	.L139
	mov	r3, r6
	movw	r2, #391
	ldr	r1, .L183+4
	ldr	r0, .L183+8
	bl	printk
.L139:
	ldr	r3, [r10]
	lsls	r0, r3, #25
	bpl	.L140
	ldr	r3, [sp, #32]
	movw	r2, #393
	strd	r4, [sp, #8]
	str	r9, [sp, #4]
	str	r3, [sp]
	ldr	r1, .L183+4
	ldr	r3, [r6, #380]
	ldr	r0, .L183+12
	bl	printk
.L140:
	ldr	r2, [r6, #188]
	cbz	r2, .L157
	ldr	r8, [r6, #196]
.L141:
	ldr	r3, [r6, #36]
	cmp	r3, #1
	bne	.L142
	ldr	r3, [r6, #44]
	cmp	r3, #1
	bgt	.L143
	movw	r1, #399
	ldr	r3, .L183+16
	str	r1, [sp]
	ldr	r2, .L183+20
	ldr	r0, .L183+24
	bl	printk
.L143:
	ldr	r1, [r6, #44]
	ldr	r0, [sp, #28]
	bl	__aeabi_idiv
	mov	fp, r0
	cbz	r0, .L144
	add	fp, r0, #-1
.L144:
	ldr	r3, [r6, #380]
	cmp	r3, #2
	beq	.L145
	cmp	r3, #3
	beq	.L146
	cbnz	r3, .L148
	ldr	r2, [sp, #32]
	movs	r1, #16
	mov	r0, r9
	bl	mpp_clip
	lsls	r3, r5, #4
	orr	r3, r3, r4, lsr #28
	lsls	r2, r4, #4
	mov	r5, r3
	mov	r4, r2
	mov	r7, r0
	b	.L148
.L157:
	mvn	r8, #-2147483648
	b	.L141
.L145:
	cmp	r9, #16000
	mov	r7, r9
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
	ldr	r3, [r6, #188]
	cbz	r3, .L148
	ldr	r8, [r6, #192]
.L148:
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
	mov	r9, r0
	cbnz	r0, .L149
	mov	r2, #428
	ldr	r1, .L183+4
	ldr	r0, .L183+28
	bl	printk
	mov	r2, #430
	ldr	r1, .L183+4
	ldr	r0, .L183+32
	ldr	r3, [sp, #24]
	strd	r4, [sp]
	strd	fp, r3, [sp, #16]
	ldr	r3, [sp, #28]
	strd	r7, r3, [sp, #8]
	bl	printk
	ldr	r3, [r6, #40]
	ldr	r1, .L183+4
	ldr	r0, .L183+36
	str	r3, [sp, #20]
	ldr	r3, [r6, #44]
	str	r3, [sp, #16]
	ldr	r3, [r6, #464]
	str	r3, [sp, #12]
	ldr	r3, [r6, #428]
	str	r3, [sp, #8]
	ldrd	r2, [r6, #392]
	strd	r2, [sp]
	movw	r2, #433
	bl	printk
.L149:
	ldr	r3, [r10]
	lsls	r1, r3, #25
	bpl	.L153
	strd	r4, [sp]
	mov	r3, r7
	movw	r2, #461
	ldr	r1, .L183+4
	ldr	r0, .L183+40
	bl	printk
.L153:
	cmp	r9, r8
	ble	.L154
	ldr	r3, [r6, #200]
	cmp	r3, #1
	bne	.L154
	ldr	r3, [r10]
	sub	r9, r8, r8, asr #4
	lsls	r2, r3, #25
	bpl	.L154
	mov	r3, r8
	mov	r2, #466
	ldr	r1, .L183+4
	ldr	r0, .L183+44
	bl	printk
.L154:
	ldr	r3, [sp, #36]
	str	r8, [r6, #548]
	str	r9, [r3]
	cmp	r9, #0
	bne	.L155
	mov	r1, #472
	ldr	r3, .L183+16
	str	r1, [sp]
	ldr	r2, .L183+48
	ldr	r0, .L183+24
	bl	printk
.L155:
	ldr	r3, [r10]
	lsls	r3, r3, #31
	bpl	.L156
	mov	r3, r6
	mov	r2, #474
	ldr	r1, .L183+4
	ldr	r0, .L183+52
	bl	printk
.L156:
	movs	r0, #0
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L146:
	ldr	r2, [sp, #32]
	movs	r1, #16
	mov	r0, r9
	bl	mpp_clip
	ldr	r3, [sp, #24]
	mov	r7, r0
	asrs	r2, r3, #31
	mov	r3, r2
	ldr	r2, [sp, #24]
	muls	r3, r4, r3
	mla	r3, r2, r5, r3
	umull	r4, r5, r4, r2
	add	r5, r5, r3
	b	.L148
.L142:
	ldr	r3, [r6, #380]
	cbz	r3, .L150
	cmp	r3, #2
	bne	.L151
	cmp	r9, #16000
	mov	r7, r9
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
	cbz	r2, .L151
	ldr	r8, [r6, #192]
.L151:
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
	mov	r9, r0
	b	.L149
.L150:
	ldr	r2, [sp, #32]
	movs	r1, #16
	mov	r0, r9
	bl	mpp_clip
	lsls	r3, r5, #4
	orr	r3, r3, r4, lsr #28
	lsls	r2, r4, #4
	mov	r5, r3
	mov	r4, r2
	mov	r7, r0
	b	.L151
.L184:
	.align	2
.L183:
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
	ldr	r6, .L214
	.pad #28
	sub	sp, sp, #28
	mov	fp, r1
	ldr	r8, [r0, #500]
	ldr	r7, [r0, #516]
	ldr	r3, [r6]
	ldrd	r4, r9, [r0, #504]
	lsls	r2, r3, #31
	ldr	r10, [r0, #520]
	bpl	.L186
	mov	r3, r0
	mov	r2, #560
	ldr	r1, .L214+4
	ldr	r0, .L214+8
	bl	printk
.L186:
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L188
	ldr	r3, .L214+12
	mov	r2, #564
	ldr	r1, .L214+4
	ldr	r0, .L214+16
	str	r3, [sp, #16]
	ldr	r3, .L214+20
	str	r3, [sp, #12]
	ldr	r3, .L214+24
	str	r3, [sp, #8]
	ldr	r3, .L214+28
	str	r3, [sp, #4]
	ldr	r3, .L214+32
	str	r3, [sp]
	ldr	r3, .L214+36
	bl	printk
	ldr	r3, [r6]
	lsls	r0, r3, #26
	bpl	.L188
	ldr	r3, [r5, #568]
	movw	r2, #567
	strd	r7, r10, [sp, #4]
	str	r4, [sp]
	asrs	r3, r3, #10
	ldr	r1, .L214+4
	strd	r8, r3, [sp, #12]
	mov	r3, r9
	ldr	r0, .L214+40
	bl	printk
.L188:
	ldrd	r2, [r5, #392]
	mov	r1, fp
	mov	r0, r5
	bl	bits_model_alloc
	cmp	r8, #0
	bgt	.L190
	movw	r1, #571
	ldr	r3, .L214+44
	str	r1, [sp]
	ldr	r2, .L214+48
	ldr	r0, .L214+52
	bl	printk
.L190:
	cmp	r4, r9
	sub	r0, r9, r4
	mov	r1, r4
	ldr	fp, .L214+92
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
	ble	.L193
	sub	r3, r8, r10
	cmp	r3, r1
	bge	.L193
	movs	r3, #6
	muls	r4, r3, r4
.L194:
	cmp	r9, #256
	mvn	r3, #127
	sub	r0, r7, r8
	it	ge
	movge	r9, #256
	cmp	r9, r3
	add	r0, r0, r0, lsl #1
	ldr	r10, [r5, #572]
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
	ldr	r0, [r5, #568]
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
	str	r3, [r5, #580]
	ldr	r3, [r6]
	lsls	r1, r3, #24
	bpl	.L196
	ldr	r3, .L214+56
	movw	r2, #613
	ldr	r1, .L214+4
	ldr	r0, .L214+60
	str	r3, [sp, #16]
	ldr	r3, .L214+64
	str	r3, [sp, #12]
	ldr	r3, .L214+68
	str	r3, [sp, #8]
	ldr	r3, .L214+72
	str	r3, [sp, #4]
	ldr	r3, .L214+76
	str	r3, [sp]
	ldr	r3, .L214+80
	bl	printk
	ldr	r3, [r6]
	lsls	r2, r3, #24
	bpl	.L196
	ldr	r3, [r5, #604]
	sub	r4, r4, r9
	str	r7, [sp, #4]
	mov	r2, #616
	str	r4, [sp]
	str	r3, [sp, #16]
	ldr	r3, [r5, #580]
	ldr	r1, .L214+4
	ldr	r0, .L214+84
	strd	r8, r3, [sp, #8]
	mov	r3, r9
	bl	printk
.L196:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L198
	mov	r3, r5
	movw	r2, #618
	ldr	r1, .L214+4
	ldr	r0, .L214+88
	bl	printk
.L198:
	movs	r0, #0
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L193:
	cmp	r7, r10
	bge	.L200
	sub	r10, r10, r8
	cmp	r10, r1
	bge	.L200
	lsls	r4, r4, #2
	b	.L194
.L200:
	movs	r4, #0
	b	.L194
.L215:
	.align	2
.L214:
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
	ldr	r10, [r0, #500]
	ldr	r8, [r0, #516]
	str	r3, [sp, #28]
	ldrd	r7, r3, [r0, #504]
	ldr	r9, [r0, #520]
	str	r3, [sp, #32]
	ldr	r3, .L254
	ldr	r2, [r3]
	mov	fp, r3
	tst	r2, #1
	beq	.L217
	mov	r3, r0
	movw	r2, #773
	ldr	r1, .L254+4
	ldr	r0, .L254+8
	bl	printk
.L217:
	ldrd	r2, [r6, #392]
	mov	r1, r4
	mov	r0, r6
	bl	bits_model_alloc
	mov	r1, r7
	ldr	r3, [sp, #32]
	cmp	r7, r3
	sub	r0, r3, r7
	ldr	r3, [r6, #608]
	ble	.L218
	cmp	r3, #36
	ble	.L219
	lsls	r0, r0, #7
.L252:
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
	ldr	r3, .L254+12
	ldr	r2, [sp, #36]
	usat	r0, #6, r0
	ldr	r4, [r3, r2, lsl #2]
	ldr	r3, [r3, r0, lsl #2]
	subs	r4, r4, r3
	ldr	r3, [fp]
	lsls	r3, r3, #26
	bpl	.L228
	ldr	r3, .L254+16
	movw	r2, #806
	ldr	r1, .L254+4
	ldr	r0, .L254+20
	str	r3, [sp, #16]
	ldr	r3, .L254+24
	str	r3, [sp, #12]
	ldr	r3, .L254+28
	str	r3, [sp, #8]
	ldr	r3, .L254+32
	str	r3, [sp, #4]
	ldr	r3, .L254+36
	str	r3, [sp]
	ldr	r3, .L254+40
	bl	printk
	ldr	r3, [fp]
	lsls	r0, r3, #26
	bpl	.L228
	ldr	r3, [sp, #28]
	mov	r2, #808
	strd	r8, r9, [sp, #4]
	str	r7, [sp]
	strd	r10, r3, [sp, #12]
	ldr	r1, .L254+4
	ldr	r3, [sp, #32]
	ldr	r0, .L254+44
	bl	printk
.L228:
	cmp	r10, r8
	bge	.L230
	cmp	r8, r9
	ble	.L231
	movs	r3, #6
	muls	r4, r3, r4
.L232:
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
	ldr	r3, [r6, #432]
	cmp	r3, #640
	bls	.L233
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
.L233:
	ldr	r7, .L254
	adds	r3, r5, r4
	add	r3, r3, r8
	str	r3, [r6, #580]
	ldr	r3, [r7]
	lsls	r1, r3, #24
	bpl	.L235
	ldr	r3, .L254+48
	movw	r2, #831
	ldr	r1, .L254+4
	ldr	r0, .L254+52
	str	r3, [sp, #12]
	ldr	r3, .L254+56
	str	r3, [sp, #8]
	ldr	r3, .L254+60
	str	r3, [sp, #4]
	ldr	r3, .L254+64
	str	r3, [sp]
	ldr	r3, .L254+68
	bl	printk
	ldr	r3, [r7]
	lsls	r2, r3, #24
	bpl	.L235
	ldr	r3, [r6, #604]
	movw	r2, #833
	str	r4, [sp]
	ldr	r1, .L254+4
	str	r3, [sp, #12]
	ldr	r3, [r6, #580]
	ldr	r0, .L254+72
	strd	r8, r3, [sp, #4]
	mov	r3, r5
	bl	printk
.L235:
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L237
	mov	r3, r6
	movw	r2, #834
	ldr	r1, .L254+4
	ldr	r0, .L254+76
	bl	printk
.L237:
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L219:
	cmp	r3, #34
	ble	.L221
	movs	r3, #80
.L253:
	muls	r0, r3, r0
	b	.L252
.L221:
	cmp	r3, #32
	ble	.L222
.L226:
	lsls	r0, r0, #5
	b	.L252
.L222:
	lsls	r0, r0, #4
	b	.L252
.L218:
	cmp	r3, #29
	bgt	.L223
	movs	r3, #192
	b	.L253
.L223:
	cmp	r3, #31
	bgt	.L224
	movs	r3, #144
	b	.L253
.L224:
	cmp	r3, #33
	bgt	.L225
	movs	r3, #96
	b	.L253
.L225:
	cmp	r3, #36
	bgt	.L226
	lsls	r0, r0, #6
	b	.L252
.L230:
	cmp	r8, r9
	bgt	.L239
.L231:
	cmp	r10, r9
	ble	.L239
	add	r4, r4, r4, lsl #1
	b	.L232
.L239:
	movs	r4, #0
	b	.L232
.L255:
	.align	2
.L254:
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
	ldr	r6, .L276
	ldr	r5, [r0, #64]
	ldr	r8, [r0, #40]
	ldr	r3, [r6]
	lsls	r5, r5, #4
	ldr	r7, [r0, #584]
	lsls	r1, r3, #31
	bpl	.L257
	mov	r3, r0
	movw	r2, #487
	ldr	r1, .L276+4
	ldr	r0, .L276+8
	bl	printk
.L257:
	ldrd	r0, r3, [r4, #392]
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
	ldr	r3, [r4, #508]
	ldr	r5, [r4, #576]
	cmp	r3, r0
	bgt	.L260
	cbz	r5, .L261
.L260:
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
	blt	.L265
	ldr	r2, .L276+12
	add	r7, r2, r7, lsl #2
	ldr	r0, [r7, #168]
	cmp	r3, r0
	it	ge
	movge	r3, r0
.L262:
	ldr	r2, [r6]
	str	r3, [r4, #576]
	lsls	r2, r2, #25
	bpl	.L261
	movw	r2, #505
	ldr	r1, .L276+4
	ldr	r0, .L276+16
	bl	printk
.L261:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L264
	mov	r3, r4
	mov	r2, #508
	ldr	r1, .L276+4
	ldr	r0, .L276+20
	bl	printk
.L264:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r10, fp, pc}
.L265:
	movs	r3, #0
	b	.L262
.L277:
	.align	2
.L276:
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
	ldr	r5, [r0, #628]
	ldr	r6, .L292
	asrs	r5, r5, #5
	cmp	r5, #7
	ldr	r3, [r6]
	it	ge
	movge	r5, #7
	lsls	r1, r3, #31
	bpl	.L279
	mov	r3, r0
	mov	r2, #528
	ldr	r1, .L292+4
	ldr	r0, .L292+8
	bl	printk
.L279:
	ldrd	r0, r1, [r4, #620]
	bl	__aeabi_idiv
	ldr	r3, [r6]
	cmp	r0, #35
	it	ge
	movge	r0, #35
	lsls	r2, r3, #24
	mov	r7, r0
	bpl	.L280
	ldr	r3, [r4, #628]
	movw	r2, #533
	ldr	r1, .L292+4
	strd	r0, r3, [sp]
	ldr	r3, [r4, #608]
	ldr	r0, .L292+12
	bl	printk
.L280:
	ldr	r2, [r4, #208]
	ldr	r3, .L292+16
	add	r3, r3, r2
	ldr	r2, .L292+20
	uxtab	r5, r2, r5
	ldrsb	r3, [r3, #389]
	ldrsb	r2, [r5, #256]
	subs	r1, r3, r2
	cmp	r7, r1
	itt	gt
	subgt	r7, r7, r3
	addgt	r7, r7, r2
	ldrd	r2, r1, [r4, #104]
	mov	r0, r7
	bl	mpp_clip
	ldr	r3, [r6]
	lsls	r3, r3, #31
	str	r0, [r4, #608]
	bpl	.L282
	mov	r3, r4
	movw	r2, #543
	ldr	r1, .L292+4
	ldr	r0, .L292+24
	bl	printk
.L282:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L293:
	.align	2
.L292:
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
	ldr	r5, .L303
	mov	r6, r1
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L295
	mov	r3, r0
	movw	r2, #627
	ldr	r1, .L303+4
	ldr	r0, .L303+8
	bl	printk
.L295:
	ldr	r3, [r4, #548]
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
	str	r0, [r4, #580]
	bpl	.L296
	mov	r3, r4
	movw	r2, #630
	ldr	r1, .L303+4
	ldr	r0, .L303+12
	bl	printk
.L296:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L304:
	.align	2
.L303:
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
	ldr	r0, [r0, #552]
	mov	r8, r1
	bl	mpp_data_sum_v2
	mov	r1, r6
	bl	__aeabi_idiv
	mov	r1, #-1
	mul	r7, r0, r6
	mov	r10, r0
	ldr	r0, [r4, #552]
	bl	mpp_data_get_pre_val_v2
	ldr	r5, [r8, #24]
	mov	r1, r6
	ldr	r6, .L327
	subs	r0, r5, r0
	add	r0, r0, r7
	bl	__aeabi_idiv
	ldr	r3, [r6]
	ldr	r7, [r8]
	lsls	r1, r3, #31
	ldr	r9, [r4, #500]
	mov	fp, r0
	bpl	.L306
	mov	r3, r4
	movw	r2, #654
	ldr	r1, .L327+4
	ldr	r0, .L327+8
	bl	printk
.L306:
	ldr	r2, [r4, #548]
	ldr	r3, [r8, #24]
	cmp	r2, r3
	bgt	.L307
	ldr	r3, [r4, #200]
	cmp	r3, #1
	bne	.L307
	mov	r1, r8
	mov	r0, r4
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	reenc_calc_super_frm_ratio
.L307:
	ldr	r0, [r4, #568]
	ldr	r2, [r4, #564]
	adds	r3, r5, r0
	ldr	r0, [r4, #400]
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
	ldr	r3, .L327+12
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
	ldr	r1, [r4, #572]
	ldr	r3, [sp, #12]
	str	r0, [sp, #16]
	subs	r0, r3, r1
	lsls	r0, r0, #5
	bl	__aeabi_idiv
	cmp	r10, fp
	ldrd	r2, r3, [sp, #16]
	bge	.L312
	cmp	r10, r9
	beq	.L312
	movs	r1, #6
	muls	r3, r1, r3
.L326:
	cmp	r3, #256
	mvn	r1, #191
	it	ge
	movge	r3, #256
	cmp	r3, r1
	it	lt
	movlt	r3, r1
.L313:
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
	str	r3, [r4, #580]
	lsls	r2, r2, #25
	bpl	.L314
	strd	r5, r3, [sp]
	movw	r2, #709
	mov	r3, r7
	ldr	r1, .L327+4
	ldr	r0, .L327+16
	bl	printk
.L314:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L305
	mov	r3, r4
	movw	r2, #710
	ldr	r1, .L327+4
	ldr	r0, .L327+20
	bl	printk
.L305:
	movs	r0, #0
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L312:
	ldr	r1, [r4, #380]
	cmp	r1, #2
	bne	.L316
	add	r3, r3, r3, lsl #1
	b	.L326
.L316:
	movs	r3, #0
	b	.L313
.L328:
	.align	2
.L327:
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
	ldr	r0, [r0, #552]
	bl	mpp_data_sum_v2
	mov	r1, r5
	bl	__aeabi_idiv
	mov	r1, #-1
	mul	r6, r0, r5
	mov	r10, r0
	ldr	r0, [r4, #552]
	bl	mpp_data_get_pre_val_v2
	ldr	r7, [r8, #24]
	mov	r1, r5
	subs	r0, r7, r0
	add	r0, r0, r6
	ldr	r6, .L348
	bl	__aeabi_idiv
	ldr	r3, [r4, #56]
	ldr	r9, [r4, #500]
	ldr	fp, [r8]
	str	r3, [sp, #4]
	mov	r5, r0
	ldr	r3, [r6]
	lsls	r1, r3, #31
	bpl	.L330
	mov	r3, r4
	mov	r2, #856
	ldr	r1, .L348+4
	ldr	r0, .L348+8
	bl	printk
.L330:
	ldr	r2, [r4, #548]
	ldr	r3, [r8, #24]
	cmp	r2, r3
	bgt	.L331
	ldr	r3, [r4, #200]
	cmp	r3, #1
	bne	.L331
	mov	r1, r8
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	reenc_calc_super_frm_ratio
.L331:
	cmp	r7, fp
	sub	r0, r7, fp
	ite	lt
	movlt	r1, r7
	movge	r1, fp
	lsls	r0, r0, #5
	bl	__aeabi_idiv
	cmp	r10, r5
	mov	r7, r0
	bge	.L338
	cmp	r5, r9
	ble	.L338
	ldr	r3, [sp, #4]
	mov	r0, r5
	ldr	fp, .L348+20
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
.L334:
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
	str	r3, [r4, #580]
	lsls	r2, r2, #25
	bpl	.L335
	movw	r2, #882
	ldr	r1, .L348+4
	ldr	r0, .L348+12
	bl	printk
.L335:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L329
	mov	r3, r4
	movw	r2, #883
	ldr	r1, .L348+4
	ldr	r0, .L348+16
	bl	printk
.L329:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L338:
	mov	r8, #0
	b	.L334
.L349:
	.align	2
.L348:
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
	ldr	r3, [r0, #212]
	cbz	r3, .L350
	ldr	r3, [r0, #380]
	subs	r3, r3, #2
	cmp	r3, #1
	bhi	.L352
.L356:
	movs	r3, #0
	strd	r3, r3, [r0, #660]
	strd	r3, r3, [r0, #652]
.L360:
	str	r3, [r0, #648]
.L350:
	bx	lr
.L352:
	ldr	r3, [r0, #664]
	ldr	r2, [r0, #244]
	cmp	r3, r2
	bge	.L353
	adds	r3, r3, #1
	str	r3, [r0, #664]
	movs	r3, #4
	b	.L360
.L353:
	ldr	r3, [r0, #660]
	ldr	r2, [r0, #240]
	cmp	r3, r2
	bge	.L354
	adds	r3, r3, #1
	str	r3, [r0, #660]
	movs	r3, #0
	str	r3, [r0, #664]
	movs	r3, #3
	b	.L360
.L354:
	ldr	r3, [r0, #656]
	ldr	r2, [r0, #236]
	cmp	r3, r2
	bge	.L355
	adds	r3, r3, #1
	str	r3, [r0, #656]
	movs	r3, #0
	strd	r3, r3, [r0, #660]
	movs	r3, #2
	b	.L360
.L355:
	ldr	r3, [r0, #652]
	ldr	r2, [r0, #232]
	cmp	r3, r2
	bge	.L356
	adds	r3, r3, #1
	str	r3, [r0, #652]
	movs	r3, #0
	strd	r3, r3, [r0, #660]
	str	r3, [r0, #656]
	movs	r3, #1
	b	.L360
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
	ldr	r9, .L404+48
	.pad #28
	sub	sp, sp, #28
	ldr	r3, [r0, #524]
	mov	r4, r0
	ldr	fp, .L404+20
	mov	r7, r5
	mov	r6, r9
	str	r3, [sp, #20]
.L366:
	mov	r1, r7
	ldr	r0, [r4, #476]
	bl	mpp_data_get_pre_val_v2
	mov	r1, r7
	mov	r10, r0
	ldr	r0, [r4, #480]
	bl	mpp_data_get_pre_val_v2
	adds	r1, r0, #1
	mov	r3, r0
	beq	.L377
	ldr	r2, [r4, #540]
	rsb	r2, r2, #8
	adds	r2, r2, r0
	bmi	.L378
	cmp	r2, #15
	it	ge
	movge	r2, #15
	add	r2, fp, r2, lsl #2
	ldr	r8, [r2, #264]
.L362:
	mul	r2, r10, r8
	add	r5, r5, r2, asr #5
	ldr	r2, [r9]
	lsls	r2, r2, #25
	bpl	.L364
	ldr	r2, [r4, #540]
	ldr	r1, .L404
	ldr	r0, .L404+4
	str	r2, [sp]
	movw	r2, #914
	bl	printk
	ldr	r3, [r9]
	lsls	r0, r3, #25
	bpl	.L364
	str	r10, [sp]
	mov	r3, r8
	movw	r2, #915
	ldr	r1, .L404
	ldr	r0, .L404+8
	bl	printk
.L364:
	cmp	r7, #0
	beq	.L379
	ldr	r0, [r4, #472]
	bl	mpp_data_sum_v2
	mov	r8, r0
	ldr	r0, [r4, #484]
	bl	mpp_data_sum_v2
	mov	r9, r0
	ldr	r0, [r4, #488]
	bl	mpp_data_sum_v2
	ldr	r3, [r6]
	lsls	r1, r3, #25
	mov	fp, r0
	bpl	.L367
	strd	r9, r0, [sp]
	mov	r3, r8
	movw	r2, #921
	ldr	r1, .L404
	ldr	r0, .L404+12
	bl	printk
.L367:
	ldr	r7, .L404+16
	cmp	r8, #0
	beq	.L381
	cmp	r5, #0
	beq	.L381
	mov	r1, r8
	lsls	r0, r5, #6
	bl	__aeabi_idiv
	ldr	r3, .L404+20
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
.L368:
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L369
	ldr	r3, [r4, #528]
	movw	r2, #931
	ldr	r1, .L404
	ldr	r0, .L404+24
	str	r3, [sp]
	mov	r3, r10
	bl	printk
.L369:
	ldr	r3, [r4, #528]
	movs	r1, #70
	sub	r5, r3, #30
	rsb	r8, r3, #100
	lsls	r5, r5, #8
	mla	r0, r8, r10, r5
	bl	__aeabi_idiv
	ldr	r3, [r6]
	lsls	r3, r3, #25
	mov	r10, r0
	bpl	.L370
	str	r8, [sp]
	mov	r3, r5
	mov	r2, #936
	ldr	r1, .L404
	ldr	r0, .L404+28
	bl	printk
.L370:
	ldr	r3, [r4, #616]
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
	bpl	.L371
	mov	r3, r4
	movw	r2, #939
	ldr	r1, .L404
	ldr	r0, .L404+32
	bl	printk
.L371:
	cmp	r9, #0
	beq	.L382
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
	bpl	.L373
	mov	r3, r9
	mov	r2, #944
	ldr	r1, .L404
	ldr	r0, .L404+36
	bl	printk
.L373:
	lsl	r0, r9, #8
	umull	r0, r1, r0, r7
	lsrs	r3, r1, #5
.L372:
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
	bpl	.L361
	ldr	r3, [sp, #20]
	movw	r2, #951
	ldr	r1, .L404
	ldr	r0, .L404+40
	str	r3, [sp]
	mov	r3, r7
	bl	printk
	ldr	r3, [r6]
	lsls	r3, r3, #25
	bpl	.L361
	strd	r4, r7, [sp, #4]
	mov	r3, r10
	str	r5, [sp]
	movw	r2, #953
	ldr	r1, .L404
	ldr	r0, .L404+44
	bl	printk
.L361:
	mov	r0, r7
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L379:
	movs	r7, #1
	b	.L366
.L377:
	mov	r8, #32
	b	.L362
.L378:
	mov	r8, #14
	b	.L362
.L381:
	mov	r10, #255
	b	.L368
.L382:
	mov	r3, #256
	b	.L372
.L405:
	.align	2
.L404:
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
	ldr	r10, [r0, #500]
	ldr	r6, [r0, #516]
	str	r3, [sp, #36]
	ldrd	r8, r3, [r0, #504]
	ldr	r7, [r0, #520]
	str	r3, [sp, #32]
	ldr	r3, .L446
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #1
	beq	.L407
	mov	r3, r0
	movw	r2, #975
	ldr	r1, .L446+4
	ldr	r0, .L446+8
	bl	printk
.L407:
	mov	r0, r4
	bl	moving_ratio_calc
	ldr	r3, [r4, #536]
	subs	r5, r3, #2
	cmp	r5, r0
	it	lt
	movlt	r5, r0
	cmp	r3, r5
	ble	.L408
	ldr	r1, [r4, #532]
	ldr	r2, [r4, #592]
	cmp	r2, r1, lsl #6
	bcc	.L408
	adds	r3, r3, #1
	cmp	r3, #255
	it	ge
	movge	r3, #255
	cmp	r3, #1
	it	lt
	movlt	r3, #1
	str	r3, [r4, #536]
.L409:
	ldr	r3, [r9]
	lsls	r3, r3, #25
	bpl	.L410
	mov	r3, r5
	movw	r2, #987
	ldr	r1, .L446+4
	ldr	r0, .L446+12
	bl	printk
.L410:
	cmp	r5, #1
	ldrd	r2, r3, [r4, #392]
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
	ldr	r10, .L446
	cmp	r9, #1
	mov	r0, r4
	it	lt
	movlt	r9, #1
	bl	bits_model_alloc
	ldr	r3, [r10]
	lsls	r0, r3, #26
	bpl	.L412
	ldr	r3, .L446+16
	mov	r2, #1004
	ldr	r1, .L446+4
	ldr	r0, .L446+20
	str	r3, [sp, #12]
	ldr	r3, .L446+24
	str	r3, [sp, #8]
	ldr	r3, .L446+28
	str	r3, [sp, #4]
	ldr	r3, .L446+32
	str	r3, [sp]
	ldr	r3, .L446+36
	bl	printk
	ldr	r3, [r10]
	lsls	r1, r3, #26
	bpl	.L412
	ldr	r3, [sp, #32]
	movw	r2, #1006
	strd	r6, r7, [sp, #8]
	str	r8, [sp, #4]
	str	r3, [sp]
	ldr	r1, .L446+4
	ldr	r3, [r4, #536]
	ldr	r0, .L446+40
	bl	printk
.L412:
	ldr	r3, [sp, #32]
	mov	r1, r8
	cmp	r8, r3
	sub	r0, r3, r8
	ble	.L414
	lsls	r0, r0, #5
	bl	__aeabi_idiv
	mov	r5, r0
.L415:
	ldr	r3, [r4, #544]
	lsr	r8, r3, #4
	cmp	r8, #9
	bls	.L425
	cmp	r8, #200
	it	ge
	movge	r8, #200
.L416:
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
	ldr	r3, .L446+44
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
	bge	.L417
	movs	r7, #6
	muls	r7, r3, r7
.L418:
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
	ldrd	r0, r1, [r4, #416]
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
	ldrd	r0, r1, [r4, #408]
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
	ldr	r3, [r4, #544]
	mul	r9, r9, r0
	cmp	r9, #1
	it	ge
	movge	r9, #1
	cmp	r3, #640
	orr	r9, r9, r9, asr #31
	bls	.L419
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
.L419:
	ldr	r1, [sp, #36]
	add	r2, r9, r8
	adds	r3, r5, r7
	cmp	r1, r6
	add	r3, r3, r2
	blt	.L430
	cmp	r3, #0
	ble	.L430
	ldr	r0, [r4, #532]
	ldr	r1, [r4, #592]
	cmp	r1, r0, lsl #6
	bcc	.L430
	adds	r3, r7, r2
	mvn	r6, #5
.L420:
	add	r3, r3, r6
	str	r3, [r4, #580]
	ldr	r3, .L446
	ldr	r2, [r3]
	mov	r10, r3
	tst	r2, #128
	beq	.L422
	ldr	r3, .L446+48
	movw	r2, #1057
	ldr	r1, .L446+4
	ldr	r0, .L446+52
	str	r3, [sp, #20]
	ldr	r3, .L446+56
	str	r3, [sp, #16]
	ldr	r3, .L446+60
	str	r3, [sp, #12]
	ldr	r3, .L446+64
	str	r3, [sp, #8]
	ldr	r3, .L446+68
	str	r3, [sp, #4]
	ldr	r3, .L446+72
	str	r3, [sp]
	ldr	r3, .L446+76
	bl	printk
	ldr	r3, [r10]
	lsls	r2, r3, #24
	bpl	.L422
	ldr	r3, [r4, #604]
	movw	r2, #1060
	strd	r8, r9, [sp, #4]
	str	r7, [sp]
	str	r3, [sp, #20]
	ldr	r3, [r4, #580]
	ldr	r1, .L446+4
	ldr	r0, .L446+80
	strd	r6, r3, [sp, #12]
	mov	r3, r5
	bl	printk
.L422:
	ldr	r3, [r10]
	lsls	r3, r3, #31
	bpl	.L424
	mov	r3, r4
	movw	r2, #1061
	ldr	r1, .L446+4
	ldr	r0, .L446+84
	bl	printk
.L424:
	movs	r0, #0
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L408:
	str	r5, [r4, #536]
	b	.L409
.L414:
	lsls	r0, r0, #6
	bl	__aeabi_idiv
	movs	r3, #100
	muls	r5, r3, r5
	asrs	r5, r5, #8
	muls	r5, r0, r5
	b	.L415
.L425:
	mov	r8, #10
	b	.L416
.L417:
	cmp	r6, r7
	bge	.L427
	cmp	r7, fp
	bge	.L427
	add	r7, r3, r3, lsl #1
	b	.L418
.L427:
	movs	r7, #0
	b	.L418
.L430:
	movs	r6, #0
	b	.L420
.L447:
	.align	2
.L446:
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
	ldr	r3, .L503
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r2, [r3]
	.pad #24
	sub	sp, sp, #24
	ldr	r5, [r0, #40]
	mov	r8, r3
	tst	r2, #1
	beq	.L449
	mov	r3, r0
	movw	r2, #1074
	ldr	r1, .L503+4
	ldr	r0, .L503+8
	bl	printk
.L449:
	ldr	r3, [r4, #68]
	movs	r2, #100
	ldr	r0, [r4, #64]
	cmp	r3, #100
	it	ge
	movge	r3, #100
	cmp	r3, #10
	it	lt
	movlt	r3, #10
	str	r3, [r4, #68]
	mov	r1, r3
	bl	mpp_clip
	ldr	r2, [r4, #72]
	ldr	r1, [r8]
	cmp	r2, #640
	it	ge
	movge	r2, #640
	cmp	r2, #160
	it	lt
	movlt	r2, #160
	lsls	r1, r1, #25
	str	r2, [r4, #72]
	str	r0, [r4, #64]
	bpl	.L450
	strd	r0, r2, [sp]
	movw	r2, #1083
	ldr	r1, .L503+4
	ldr	r0, .L503+12
	bl	printk
	cmp	r5, #0
	bne	.L451
	ldr	r3, [r8]
	lsls	r2, r3, #25
	bpl	.L453
	movw	r2, #1086
	ldr	r1, .L503+4
	ldr	r0, .L503+16
	bl	printk
.L453:
	mov	r6, #500
.L500:
	ldr	r3, [r4, #528]
	str	r6, [r4, #40]
	cbnz	r3, .L459
	ldr	r3, [r4, #48]
	cmp	r3, #0
	beq	.L473
	ldr	r1, [r4, #56]
	cmp	r1, #0
	beq	.L473
	movs	r0, #100
	muls	r0, r3, r0
	bl	__aeabi_idiv
	mov	r3, r0
.L460:
	ldr	r2, [r8]
	str	r3, [r4, #528]
	lsls	r7, r2, #25
	bpl	.L459
	movw	r2, #1108
	ldr	r1, .L503+4
	ldr	r0, .L503+20
	bl	printk
.L459:
	movs	r3, #35
	str	r3, [r4, #532]
	movs	r3, #90
	str	r3, [r4, #524]
	movs	r3, #1
	str	r3, [r4, #404]
	movs	r3, #0
	strd	r3, r3, [r4, #620]
	ldr	r3, .L503+24
	ldr	r5, [r4, #56]
	str	r3, [r4, #684]
	ldr	r3, [r4, #8]
	cmp	r3, #3
	bhi	.L462
	tbb	[pc, r3]
.L464:
	.byte	(.L467-.L464)/2
	.byte	(.L466-.L464)/2
	.byte	(.L474-.L464)/2
	.byte	(.L463-.L464)/2
	.p2align 1
.L451:
	cmp	r5, #1
	bne	.L455
	ldr	r3, [r8]
	lsls	r3, r3, #25
	bpl	.L456
	movw	r2, #1089
	ldr	r1, .L503+4
	ldr	r0, .L503+28
	bl	printk
.L456:
	movs	r3, #16
	str	r3, [r4, #72]
	ldr	r3, [r4, #204]
	cmp	r3, #0
	beq	.L453
	movs	r3, #0
	str	r3, [r4, #204]
	b	.L453
.L473:
	movs	r3, #25
	b	.L460
.L466:
	ldr	r3, .L503+32
	ldr	r5, [r4, #52]
	str	r3, [r4, #680]
	ldr	r3, .L503+36
	str	r3, [r4, #684]
.L468:
	ldr	r3, [r4, #52]
	cmp	r6, #0
	str	r3, [r4, #500]
	ldrd	r2, r3, [r4, #28]
	ite	gt
	smullgt	r6, r7, r6, r3
	smullle	r6, r7, r2, r3
	add	r3, sp, #20
	umull	r0, r1, r6, r5
	movs	r6, #0
	mla	r1, r5, r7, r1
	bl	div_s64_rem
	strd	r0, r6, [r4, #392]
	mov	r7, r0
	ldrd	r1, r0, [r4, #28]
	muls	r0, r5, r0
	bl	__aeabi_uidiv
	ldr	r3, [r8]
	add	r5, r5, r5, lsl #1
	str	r5, [r4, #564]
	lsls	r2, r3, #25
	asr	r5, r5, #3
	strd	r5, r5, [r4, #568]
	str	r0, [r4, #400]
	bpl	.L471
	ldr	r3, [r4, #60]
	movw	r2, #1157
	strd	r7, r6, [sp]
	ldr	r1, .L503+4
	strd	r0, r3, [sp, #8]
	ldr	r3, [r4, #40]
	ldr	r0, .L503+40
	bl	printk
.L471:
	mov	r0, r4
	bl	bits_model_param_init
	mov	r5, r0
	cbnz	r0, .L475
	mov	r0, r4
	bl	bits_frm_init
	ldr	r3, [r8]
	lsls	r3, r3, #31
	bpl	.L448
	mov	r3, r4
	movw	r2, #1163
	ldr	r1, .L503+4
	ldr	r0, .L503+44
	bl	printk
.L448:
	mov	r0, r5
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L467:
	ldr	r3, .L503+48
.L501:
	str	r3, [r4, #680]
	b	.L468
.L463:
	ldr	r3, .L503+52
	b	.L501
.L462:
	movw	r1, #1138
	ldr	r0, .L503+56
	bl	printk
	b	.L468
.L474:
	movs	r5, #0
	b	.L448
.L475:
	mov	r5, #-1
	b	.L448
.L450:
	cmp	r5, #0
	beq	.L453
	cmp	r5, #1
	beq	.L456
.L455:
	ldr	r1, [r4, #40]
	mov	r2, #500
	mov	r0, r1
	bl	mpp_clip
	mov	r6, r0
	b	.L500
.L504:
	.align	2
.L503:
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
	ldr	r5, .L512
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L507
	mov	r3, r0
	movw	r2, #1169
	ldr	r1, .L512+4
	ldr	r0, .L512+8
	bl	printk
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L507
	mov	r3, r4
	movw	r2, #1171
	ldr	r1, .L512+4
	ldr	r0, .L512+12
	bl	printk
.L507:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L513:
	.align	2
.L512:
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
	ldr	r6, .L532
	mov	r7, r1
	ldr	r8, [r0, #380]
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L515
	mov	r3, r5
	movw	r2, #1182
	ldr	r1, .L532+4
	ldr	r0, .L532+8
	bl	printk
.L515:
	ldr	r4, [r5, #188]
	cbz	r4, .L516
	cmp	r8, #2
	ldr	r3, [r5, #196]
	ldr	r2, [r7, #24]
	it	eq
	ldreq	r3, [r5, #192]
	cmp	r2, r3
	bcc	.L521
	cmp	r4, #1
	bne	.L522
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L518
	movw	r2, #1190
	ldr	r1, .L532+4
	ldr	r0, .L532+12
	bl	printk
.L518:
	movs	r3, #1
	str	r3, [r5, #176]
	movs	r3, #0
	str	r3, [r5, #184]
.L522:
	mov	r4, #-1
	b	.L516
.L521:
	movs	r4, #0
.L516:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L514
	mov	r3, r5
	movw	r2, #1197
	ldr	r1, .L532+4
	ldr	r0, .L532+16
	bl	printk
.L514:
	mov	r0, r4
	pop	{r4, r5, r6, r7, r8, pc}
.L533:
	.align	2
.L532:
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
	mov	r8, r1
	ldr	r10, [r0, #380]
	ldr	r0, [r0, #552]
	bl	mpp_data_sum_v2
	mov	r1, r5
	bl	__aeabi_idiv
	mov	r1, #-1
	mul	r6, r0, r5
	mov	r7, r0
	ldr	r0, [r4, #552]
	bl	mpp_data_get_pre_val_v2
	ldr	r3, [r8, #24]
	mov	r1, r5
	subs	r0, r3, r0
	add	r0, r0, r6
	bl	__aeabi_idiv
	ldr	r3, .L575
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #1
	mov	r6, r0
	beq	.L535
	mov	r3, r4
	movw	r2, #1214
	ldr	r1, .L575+4
	ldr	r0, .L575+8
	bl	printk
.L535:
	ldr	r3, [r9]
	lsls	r0, r3, #25
	bpl	.L536
	strd	r7, r6, [sp]
	mov	r2, #1216
	ldr	r3, [r4, #52]
	ldr	r1, .L575+4
	ldr	r0, .L575+12
	bl	printk
.L536:
	ldr	r2, [r4, #632]
	ldr	r3, [r4, #156]
	cmp	r2, r3
	blt	.L537
.L539:
	movs	r5, #0
.L534:
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L537:
	ldr	r3, [r4, #248]
	cmp	r3, #0
	bne	.L539
	mov	r1, r8
	mov	r0, r4
	bl	check_super_frame
	mov	r5, r0
	cmp	r0, #0
	bne	.L548
	ldr	r3, [r4, #204]
	cbz	r3, .L540
	ldr	r3, [r4, #404]
	cmp	r3, #0
	beq	.L539
.L540:
	ldr	r3, [r9]
	lsls	r1, r3, #22
	bpl	.L541
	str	r10, [sp]
	movw	r2, #1230
	ldr	r3, [r4, #176]
	ldr	r1, .L575+4
	ldr	r0, .L575+16
	bl	printk
.L541:
	ldr	r3, [r4, #176]
	ldr	r2, [r9]
	cbz	r3, .L542
	cmp	r10, #0
	bne	.L542
	ldr	r0, [r4, #180]
	lsls	r2, r2, #22
	add	r1, r0, #100
	ldr	r0, [r4, #56]
	mul	r0, r0, r1
	ldr	r1, .L575+20
	umull	r0, r1, r0, r1
	lsr	r5, r1, #5
	bpl	.L543
	ldr	r2, [r4, #52]
	strd	r5, r6, [sp, #4]
	ldr	r1, .L575+4
	str	r2, [sp]
	movw	r2, #1234
	ldr	r0, .L575+24
	bl	printk
.L543:
	cmp	r6, r5
	mov	r5, #-1
	it	le
	movle	r5, #0
	b	.L534
.L542:
	cmp	r10, #0
	beq	.L544
	cmp	r10, #2
	bne	.L549
	ldr	r3, [r8]
	add	r3, r3, r3, lsl #1
	add	r3, r3, r3, lsr #31
	asrs	r3, r3, #1
.L545:
	ldr	r1, [r8, #24]
	cmp	r1, r3
	ble	.L546
	ldr	r3, [r4, #8]
	cmp	r3, #1
	bne	.L547
	ldr	ip, [r4, #52]
	subs	r7, r6, r7
	ldr	r0, .L575+28
	asr	r3, ip, #31
	smull	r0, r1, ip, r0
	rsb	lr, r3, r1, asr #3
	cmp	lr, r7
	bge	.L546
	asrs	r5, r1, #2
	subs	r1, r5, r3
	add	r1, r1, ip
	cmp	r1, r6
	blt	.L550
	subs	r5, r3, r5
	add	r5, r5, ip
	cmp	r5, r6
	mov	r5, #-1
	it	le
	movle	r5, #0
.L546:
	lsls	r3, r2, #31
	bpl	.L534
	str	r5, [sp]
	mov	r3, r4
	movw	r2, #1265
	ldr	r1, .L575+4
	ldr	r0, .L575+32
	bl	printk
	b	.L534
.L544:
	ldr	r3, [r8]
	add	r3, r3, r3, lsl #1
	b	.L545
.L549:
	movs	r3, #0
	b	.L545
.L547:
	ldr	r3, [r4, #56]
	sub	r1, r3, r3, asr #3
	cmp	r1, r6
	bge	.L546
	ldr	r0, .L575+28
	subs	r5, r6, r7
	smull	r0, r1, r3, r0
	asrs	r3, r3, #31
	rsb	r3, r3, r1, asr #3
	cmp	r3, r5
	mov	r5, #-1
	it	ge
	movge	r5, #0
	b	.L546
.L550:
	mov	r5, #-1
	b	.L546
.L548:
	mov	r5, #-1
	b	.L534
.L576:
	.align	2
.L575:
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
	ldr	r5, .L586
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L578
	mov	r3, r0
	movw	r2, #1274
	ldr	r1, .L586+4
	ldr	r0, .L586+8
	bl	printk
.L578:
	mov	r2, #276
	mov	r1, r6
	mov	r0, r4
	bl	memcpy
	mov	r0, r4
	bl	bits_model_init
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L579
	mov	r3, r4
	movw	r2, #1279
	ldr	r1, .L586+4
	ldr	r0, .L586+12
	bl	printk
.L579:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L587:
	.align	2
.L586:
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
	ldr	r4, .L597
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L589
	mov	r3, r0
	mov	r2, #1288
	ldr	r1, .L597+4
	ldr	r0, .L597+8
	bl	printk
.L589:
	mov	r0, r5
	bl	bits_model_param_deinit
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L590
	mov	r3, r5
	movw	r2, #1291
	ldr	r1, .L597+4
	ldr	r0, .L597+12
	bl	printk
.L590:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L598:
	.align	2
.L597:
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
	ldr	r6, .L655
	mov	r4, r0
	ldr	r3, [r6]
	lsls	r1, r3, #31
	bpl	.L600
	mov	r3, r0
	movw	r2, #1303
	ldr	r1, .L655+4
	ldr	r0, .L655+8
	bl	printk
.L600:
	ldr	r3, [r4, #8]
	cmp	r3, #2
	bne	.L601
	ldr	r2, [r4, #92]
	cmp	r2, #0
	bgt	.L602
	movw	r1, #1308
	ldr	r0, .L655+12
	bl	printk
	movs	r3, #26
	str	r3, [r4, #92]
.L602:
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
	bpl	.L611
	ubfx	r3, r3, #4, #1
	movw	r2, #1331
	str	r3, [sp]
	ldr	r1, .L655+4
	ldrh	r3, [r5, #158]
	ldr	r0, .L655+16
	bl	printk
	ldr	r3, [r6]
	lsls	r0, r3, #25
	bpl	.L611
	ldr	r3, [r5, #164]
	movw	r2, #1333
	ldr	r1, .L655+4
	ldr	r0, .L655+20
	str	r3, [sp, #4]
	ldr	r3, [r5, #160]
	str	r3, [sp]
	ldr	r3, [r5, #168]
	bl	printk
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L611
	ldr	r3, [r5, #176]
	movw	r2, #1335
	ldr	r1, .L655+4
	ldr	r0, .L655+24
	str	r3, [sp, #4]
	ldr	r3, [r5, #172]
	str	r3, [sp]
	ldr	r3, [r5, #180]
	bl	printk
.L611:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L601:
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	lsrs	r3, r3, #3
	and	r3, r3, #2
	str	r3, [r4, #380]
	ldrb	r2, [r5, #154]	@ zero_extendqisi2
	and	r2, r2, #63
	cmp	r2, #3
	it	eq
	moveq	r3, r2
	str	r3, [r4, #380]
	movs	r3, #0
	str	r3, [r4, #580]
	ldr	r3, [r4, #384]
	cmp	r3, #2
	bne	.L613
	mov	r0, r4
	bl	calc_next_i_ratio
.L613:
	ldr	r3, [r4, #404]
	add	r1, r5, #160
	cmp	r3, #0
	bne	.L614
	ldr	r3, [r4, #680]
	cbz	r3, .L616
	mov	r0, r4
	blx	r3
.L617:
	ldr	r3, [r4, #404]
	cbz	r3, .L616
	ldr	r3, [r4, #92]
	cmp	r3, #0
	it	le
	movle	r3, #-1
	str	r3, [r5, #172]
.L616:
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
	bpl	.L624
	ubfx	r3, r3, #4, #1
	ldr	r1, .L655+4
	str	r3, [sp]
	movw	r2, #1367
	ldrh	r3, [r5, #158]
	ldr	r0, .L655+16
	bl	printk
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L624
	ldr	r3, [r5, #164]
	movw	r2, #1369
	ldr	r1, .L655+4
	ldr	r0, .L655+20
	str	r3, [sp, #4]
	ldr	r3, [r5, #160]
	str	r3, [sp]
	ldr	r3, [r5, #168]
	bl	printk
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L624
	ldr	r3, [r5, #176]
	movw	r2, #1371
	ldr	r1, .L655+4
	ldr	r0, .L655+24
	str	r3, [sp, #4]
	ldr	r3, [r5, #172]
	str	r3, [sp]
	ldr	r3, [r5, #180]
	bl	printk
.L624:
	movs	r3, #0
	str	r3, [r4, #632]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L611
	mov	r3, r4
	movw	r2, #1375
	ldr	r1, .L655+4
	ldr	r0, .L655+28
	bl	printk
	b	.L611
.L614:
	ldrd	r2, [r4, #392]
	mov	r0, r4
	bl	bits_model_alloc
	b	.L617
.L656:
	.align	2
.L655:
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
	ldr	r3, .L774
	ldr	r2, [r3]
	mov	fp, r3
	tst	r2, #1
	beq	.L658
	str	r1, [sp]
	mov	r3, r0
	movw	r2, #1443
	ldr	r1, .L774+4
	ldr	r0, .L774+8
	bl	printk
.L658:
	ldr	r3, [fp]
	lsls	r1, r3, #25
	bpl	.L659
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	movw	r2, #1445
	ldr	r1, .L774+4
	ubfx	r3, r3, #4, #1
	ldr	r0, .L774+12
	str	r3, [sp]
	ldrh	r3, [r5, #158]
	bl	printk
.L659:
	ldr	r3, [r5, #264]
	ands	r3, r3, #1
	beq	.L660
	ldr	r3, [r5, #268]
	strd	r3, r3, [r5, #172]
	str	r3, [r5, #180]
.L750:
	movs	r0, #0
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L660:
	ldr	r2, [r4, #8]
	cmp	r2, #2
	beq	.L750
	ldr	r2, [r4, #404]
	cmp	r2, #0
	beq	.L663
	ldrb	r2, [r5, #152]	@ zero_extendqisi2
	lsls	r2, r2, #27
	bpl	.L663
	ldr	r2, [r5, #172]
	ldr	r6, [r4, #112]
	cmp	r2, #0
	bge	.L664
	ldr	r1, [r5, #160]
	cmp	r1, #0
	beq	.L665
	add	r0, r7, #15
	add	r8, r8, #15
	ldr	r7, .L774+16
	asr	r8, r8, #4
	asrs	r0, r0, #4
	mul	r0, r0, r8
.L666:
	adds	r3, r3, #1
	cmp	r3, #8
	beq	.L667
	ldr	r2, [r7, r3, lsl #2]
	cmp	r0, r2
	bcs	.L666
.L667:
	ldr	r7, .L774+20
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
.L769:
	str	r3, [r5, #172]
.L664:
	ldr	r2, [r4, #632]
	cmp	r2, #0
	beq	.L669
	ldr	r3, [r4, #604]
	ldr	r1, [r4, #580]
	add	r3, r3, r1
	str	r3, [r4, #604]
	rsb	r6, r6, r3, asr #6
	str	r6, [r4, #608]
.L670:
	ldr	r3, .L774
	ldr	r3, [r3]
	lsls	r0, r3, #25
	bpl	.L671
	ldr	r3, [r4, #580]
	str	r2, [sp, #8]
	movw	r2, #1484
	ldr	r1, .L774+4
	str	r3, [sp, #4]
	ldr	r3, [r4, #604]
	ldr	r0, .L774+24
	str	r3, [sp]
	ldr	r3, [r4, #608]
	bl	printk
.L671:
	ldrd	r2, r1, [r5, #176]
	mov	r6, r9
	ldr	r0, [r4, #604]
	lsls	r2, r2, #6
	lsls	r1, r1, #6
	bl	mpp_clip
	str	r0, [r4, #604]
.L672:
	ldr	r3, [r4, #212]
	ldr	r7, .L774
	cbz	r3, .L703
	ldr	r3, [r4, #632]
	cbnz	r3, .L703
	mov	r0, r4
	bl	rc_hier_calc_dealt_qp
	ldr	r3, [r4, #648]
	cbz	r3, .L703
	add	r2, r4, r3, lsl #2
	ldr	r1, [r2, #212]
	ldr	r2, [r4, #608]
	subs	r2, r2, r1
	str	r2, [r4, #608]
	ldr	r2, [r7]
	lsls	r0, r2, #24
	bpl	.L703
	str	r1, [sp]
	movw	r2, #1587
	ldr	r1, .L774+4
	ldr	r0, .L774+28
	bl	printk
.L703:
	ldr	r0, [r4, #608]
	mov	r1, r6
	ldr	r2, [r5, #176]
	bl	mpp_clip
	ldr	r3, [r7]
	lsls	r1, r3, #25
	str	r0, [r4, #608]
	str	r0, [r5, #172]
	bpl	.L706
	ldr	r3, [r5, #164]
	movw	r2, #1595
	ldr	r1, .L774+4
	ldr	r0, .L774+32
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
	bpl	.L706
	ldr	r3, [r5, #176]
	movw	r2, #1598
	ldr	r1, .L774+4
	ldr	r0, .L774+36
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
.L706:
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L750
	mov	r3, r4
	movw	r2, #1599
	ldr	r1, .L774+4
	ldr	r0, .L774+40
	bl	printk
	b	.L750
.L665:
	movw	r1, #1468
	ldr	r0, .L774+44
	bl	printk
	movs	r3, #26
	b	.L769
.L669:
	ldr	r3, [r5, #172]
	add	r6, r6, r3
	str	r3, [r4, #608]
	lsls	r6, r6, #6
	str	r6, [r4, #604]
	b	.L670
.L775:
	.align	2
.L774:
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
.L663:
	ldr	r2, [r4, #604]
	ldr	r3, [r4, #580]
	ldr	r0, [r4, #496]
	add	fp, r2, r3
	bl	mpp_data_sum_v2
	mov	r8, r0
	ldr	r0, [r4, #492]
	bl	mpp_data_sum_v2
	movs	r1, #0
	str	r0, [sp, #60]
	ldr	r0, [r4, #492]
	bl	mpp_data_get_pre_val_v2
	movs	r1, #1
	mov	r7, r0
	ldr	r0, [r4, #492]
	bl	mpp_data_get_pre_val_v2
	movs	r1, #2
	str	r0, [sp, #56]
	ldr	r0, [r4, #492]
	bl	mpp_data_get_pre_val_v2
	ldr	r1, [r4, #8]
	cmp	r1, #3
	ldrd	r2, r3, [sp, #56]
	beq	.L673
	cmp	r1, #0
	beq	.L673
	cmp	r1, #1
	beq	.L674
	mov	r6, r9
.L675:
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	ldr	r2, [r5, #176]
	lsls	r1, r3, #27
	lsl	r2, r2, #6
	bpl	.L689
	ldr	r1, [r5, #180]
	mov	r0, fp
	ldr	r7, [r4, #112]
	lsls	r1, r1, #6
	bl	mpp_clip
	ldr	r3, [r4, #576]
	ldr	r2, [r4, #584]
	add	r3, r3, r0
	mov	r8, r0
	asrs	r3, r3, #6
	cmp	r3, #34
	ble	.L690
	cmp	r2, #33
	bgt	.L690
	movw	r10, #717
	mul	r10, r10, r3
	movw	r3, #307
	mla	r10, r3, r2, r10
	asr	r10, r10, #10
.L691:
	cbz	r7, .L692
	ldr	r0, [r4, #484]
	bl	mpp_data_mean_v2
	adds	r3, r0, #3
	blt	.L714
	cmp	r0, #31
	bgt	.L715
	cmp	r0, #0
	it	lt
	addlt	r0, r0, #3
	asrs	r0, r0, #2
.L693:
	ldr	r1, .L776
	ldr	r2, .L776+4
	add	r0, r0, r1
	ldrsb	r3, [r0, #836]
	ldr	r2, [r2]
	cmp	r7, r3
	ite	le
	suble	r7, r10, r7
	subgt	r7, r10, r3
	lsls	r0, r2, #25
	bpl	.L695
	ldr	r2, [r4, #632]
	addw	r1, r1, #645
	str	r3, [sp, #12]
	ldr	r3, [r4, #112]
	strd	r7, r2, [sp, #16]
	movw	r2, #1551
	ldr	r0, .L776+8
	strd	r10, r3, [sp, #4]
	asr	r3, r8, #6
	str	r3, [sp]
	ldr	r3, [r4, #584]
	bl	printk
.L695:
	mov	r10, r7
.L692:
	ldr	r2, [r5, #176]
	mov	r1, r6
	mov	r0, r10
	bl	mpp_clip
	ldr	r2, [sp, #48]
	bl	mpp_clip
	ldr	r3, [r4, #632]
	str	r0, [r4, #608]
	cbnz	r3, .L697
	ldr	r3, [r4, #204]
	str	r8, [r4, #604]
	cbz	r3, .L697
	mov	r0, r4
	bl	calc_debreath_qp
.L697:
	movs	r3, #0
	strd	r3, r3, [r4, #620]
	b	.L672
.L709:
	add	r7, r7, r2
	ldrb	r2, [r5, #152]	@ zero_extendqisi2
	add	r0, r0, r7
	and	r2, r2, #16
	cmp	r2, #0
	it	eq
	moveq	r6, r10
	cmp	r3, #1400
	blt	.L676
	cmp	r0, #300
	ite	gt
	movgt	r3, #3
	movle	r3, #2
.L770:
	add	r6, r6, r3
	cmp	r8, #14
.L773:
	ble	.L682
	b	.L771
.L676:
	cmp	r0, #300
	ite	gt
	movgt	r3, #2
	movle	r3, #1
	b	.L770
.L685:
	add	r6, r10, #1
.L682:
	ldr	r3, [r5, #176]
	cmp	r6, r3
	it	ge
	movge	r6, r3
	ldr	r3, [r5, #180]
	cmp	r6, r3
	it	lt
	movlt	r6, r3
	b	.L675
.L684:
	cmp	r8, #9
	ble	.L713
	cmp	r1, #0
	it	eq
	moveq	r6, r10
	b	.L682
.L683:
	cmp	r1, #0
	it	eq
	moveq	r6, r10
	cmp	r8, #14
	ble	.L687
	adds	r6, r6, #3
	b	.L682
.L687:
	cmp	r8, #9
	ble	.L688
	adds	r6, r6, #2
	b	.L682
.L688:
	cmp	r8, #4
	b	.L773
.L690:
	add	r10, r3, r2
	asr	r10, r10, #1
	b	.L691
.L714:
	movs	r0, #0
	b	.L693
.L715:
	movs	r0, #7
	b	.L693
.L689:
	lsls	r1, r6, #6
	mov	r0, fp
	bl	mpp_clip
	ldr	r3, .L776+4
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #64
	str	r0, [r4, #604]
	asr	r7, r0, #6
	beq	.L699
	str	r7, [sp]
	movw	r2, #1569
	ldr	r3, [r4, #608]
	ldr	r1, .L776+12
	ldr	r0, .L776+16
	bl	printk
.L699:
	str	r7, [r4, #608]
	ldrb	r3, [r5, #154]	@ zero_extendqisi2
	and	r3, r3, #63
	cmp	r3, #3
	bne	.L700
	ldr	r3, [r4, #116]
	cbz	r3, .L700
	ldr	r2, [r8]
	lsls	r2, r2, #25
	bpl	.L701
	subs	r3, r7, r3
	movw	r2, #1574
	str	r3, [sp]
	mov	r3, r7
	ldr	r1, .L776+12
	ldr	r0, .L776+20
	bl	printk
.L701:
	ldr	r3, [r4, #608]
	ldr	r2, [r4, #116]
	subs	r3, r3, r2
	str	r3, [r4, #608]
.L700:
	ldr	r0, [r4, #608]
	mov	r1, r6
	ldr	r2, [sp, #52]
	bl	mpp_clip
	str	r0, [r4, #608]
	b	.L672
.L673:
	cmp	r3, #700
	ldrb	r1, [r5, #152]	@ zero_extendqisi2
	bge	.L709
	cmp	r3, #100
	and	r1, r1, #16
	ble	.L683
	cmp	r8, #15
	ble	.L684
	cmp	r1, #0
	beq	.L685
.L771:
	adds	r6, r6, #1
	b	.L682
.L674:
	cmp	r3, #700
	bge	.L709
.L713:
	mov	r6, r9
	b	.L682
.L777:
	.align	2
.L776:
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
	ldr	r5, .L789
	mov	r4, r0
	ldr	r2, [r5]
	lsls	r1, r2, #31
	bpl	.L779
	mov	r3, r0
	str	r6, [sp]
	movw	r2, #1609
	ldr	r1, .L789+4
	ldr	r0, .L789+8
	bl	printk
.L779:
	ldr	r3, [r4, #604]
	ldrb	r2, [r6, #152]	@ zero_extendqisi2
	asrs	r3, r3, #6
	lsls	r2, r2, #27
	ite	mi
	strmi	r3, [r4, #584]
	strpl	r3, [r4, #588]
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L782
	mov	r3, r4
	mov	r2, #1616
	ldr	r1, .L789+4
	ldr	r0, .L789+12
	bl	printk
.L782:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L790:
	.align	2
.L789:
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
	ldr	r7, .L832
	mov	r5, r1
	add	r8, r1, #160
	ldr	r3, [r7]
	lsls	r6, r3, #31
	bpl	.L792
	mov	r3, r0
	str	r8, [sp]
	movw	r2, #1628
	ldr	r1, .L832+4
	ldr	r0, .L832+8
	bl	printk
.L792:
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	bfc	r3, #2, #1
	strb	r3, [r5, #156]
	ldr	r3, [r4, #8]
	cmp	r3, #2
	beq	.L794
	ldr	r3, [r5, #264]
	lsls	r0, r3, #31
	bmi	.L794
	ldr	r3, [r4, #640]
	cmp	r3, #0
	bne	.L794
	ldr	r3, [r4, #644]
	cmp	r3, #0
	bne	.L794
	mov	r1, r8
	mov	r0, r4
	bl	check_re_enc
	cmp	r0, #0
	beq	.L794
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	ldr	r6, [r4, #176]
	tst	r3, #16
	ldr	r3, [r4, #184]
	it	ne
	movne	r6, #0
	cbz	r3, .L797
	ldr	r2, [r4, #636]
	cmp	r3, r2
	it	ls
	movls	r6, #0
.L797:
	ldr	r3, [r7]
	lsls	r1, r3, #22
	bpl	.L798
	ldr	r3, [r4, #636]
	movw	r2, #1645
	ldr	r1, .L832+4
	ldr	r0, .L832+12
	str	r3, [sp]
	mov	r3, r6
	bl	printk
.L798:
	cmp	r6, #1
	beq	.L799
	cmp	r6, #2
	beq	.L800
	ldr	r3, [r4, #684]
	cbz	r3, .L802
	mov	r1, r8
	mov	r0, r4
	blx	r3
.L802:
	ldr	r3, [r4, #580]
	cbz	r3, .L803
	ldrd	r2, r3, [r5, #172]
	cmp	r2, r3
	bge	.L803
	ldr	r3, [r4, #632]
	adds	r3, r3, #1
	str	r3, [r4, #632]
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	orr	r3, r3, #4
	strb	r3, [r5, #156]
.L803:
	movs	r3, #0
	str	r3, [r4, #636]
	b	.L794
.L799:
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	bic	r3, r3, #5
	orr	r3, r3, #5
	strb	r3, [r5, #156]
	ldr	r3, [r4, #636]
	str	r6, [r4, #640]
	adds	r3, r3, #1
	str	r3, [r4, #636]
	ldr	r3, [r7]
	lsls	r2, r3, #22
	bpl	.L794
	ldr	r1, .L832+4
	movw	r2, #1653
	ldr	r0, .L832+16
.L831:
	bl	printk
.L794:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L800:
	ldr	r3, [r7]
	lsls	r3, r3, #22
	bpl	.L794
	movw	r2, #1663
	ldr	r1, .L832+4
	ldr	r0, .L832+20
	b	.L831
.L833:
	.align	2
.L832:
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
	ldr	r6, .L856
	mov	r5, r1
	add	r7, r1, #160
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L835
	str	r7, [sp]
	mov	r3, r4
	movw	r2, #1690
	ldr	r1, .L856+4
	ldr	r0, .L856+8
	bl	printk
.L835:
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L836
	ldr	r3, [r5, #184]
	movw	r2, #1692
	ldr	r1, .L856+4
	ldr	r0, .L856+12
	bl	printk
.L836:
	ldr	r3, [r4, #8]
	cmp	r3, #2
	beq	.L837
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L838
	ldr	r3, [r5, #208]
	movw	r2, #1697
	ldr	r1, .L856+4
	ldr	r0, .L856+16
	str	r3, [sp]
	ldr	r3, [r5, #204]
	bl	printk
.L838:
	ldr	r1, [r5, #204]
	ldr	r0, [r4, #492]
	bl	mpp_data_update_v2
	ldr	r1, [r5, #208]
	ldr	r0, [r4, #496]
	bl	mpp_data_update_v2
	ldr	r3, [r4, #516]
	mov	r0, r4
	str	r3, [r4, #520]
	movs	r3, #0
	str	r3, [r4, #404]
	ldr	r2, [r5, #192]
	ldr	r1, [r5, #184]
	bl	bits_model_update
	ldr	r3, [r4, #8]
	cmp	r3, #3
	bne	.L839
	mov	r1, r7
	mov	r0, r4
	bl	moving_judge_update
	ldr	r1, [r5, #184]
	mov	r0, r4
	bl	bit_statics_update
.L839:
	ldr	r3, [r5, #172]
	ldr	r2, [r4, #608]
	str	r3, [r4, #668]
	ldr	r3, [r5, #180]
	str	r3, [r4, #672]
	ldr	r3, [r5, #176]
	str	r3, [r4, #676]
	ldr	r3, [r4, #624]
	adds	r3, r3, #1
	str	r3, [r4, #624]
	ldr	r3, [r4, #620]
	add	r3, r3, r2
	str	r3, [r4, #620]
	ldr	r3, [r4, #380]
	movs	r2, #0
	str	r3, [r4, #384]
	ldr	r3, [r5, #188]
	str	r3, [r4, #540]
	ldr	r3, [r5, #200]
	str	r2, [r4, #616]
	str	r3, [r4, #628]
	ldr	r3, [r4, #604]
	str	r3, [r4, #592]
	asrs	r3, r3, #6
	ldr	r1, [r5, #160]
	str	r1, [r4, #504]
	ldr	r1, [r5, #184]
	strd	r2, r2, [r4, #640]
	str	r1, [r4, #508]
	str	r3, [r5, #300]
.L837:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L840
	mov	r3, r4
	movw	r2, #1730
	ldr	r1, .L856+4
	ldr	r0, .L856+20
	bl	printk
.L840:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L857:
	.align	2
.L856:
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
	ldr	r1, .L873
	bl	seq_puts
	ldr	r3, .L873+4
	ldr	r2, .L873+8
	mov	r0, r5
	ldr	r1, .L873+12
	str	r3, [sp, #24]
	ldr	r3, .L873+16
	str	r3, [sp, #20]
	ldr	r3, .L873+20
	str	r3, [sp, #16]
	ldr	r3, .L873+24
	str	r3, [sp, #12]
	ldr	r3, .L873+28
	str	r3, [sp, #8]
	ldr	r3, .L873+32
	str	r3, [sp, #4]
	ldr	r3, .L873+36
	str	r3, [sp]
	ldr	r3, .L873+40
	bl	seq_printf
	ldr	r1, .L873+44
	ldr	r9, [r4, #8]
	ldr	r8, [r4, #60]
	umull	r2, r3, r7, r1
	cmp	r9, #2
	ldr	r7, [r4, #40]
	lsr	r3, r3, #6
	str	r1, [sp, #52]
	str	r3, [sp, #48]
	ldrd	r0, r1, [r4, #16]
	bne	.L860
	bl	__aeabi_idiv
	mov	r10, r0
	ldrd	r0, r1, [r4, #28]
	bl	__aeabi_idiv
	mov	fp, r0
	mov	r0, r9
	bl	strof_rc_mode
	ldr	r3, [r4, #92]
	ldr	r1, .L873+48
	mov	r2, r6
	strd	r3, r3, [sp, #20]
	ldr	r3, .L873+52
	strd	r10, fp, [sp, #4]
	str	r8, [sp]
	strd	r0, r3, [sp, #12]
	mov	r3, r7
.L870:
	mov	r0, r5
	ldr	r8, .L873+272
	bl	seq_printf
	ldr	r1, .L873+56
	mov	r0, r5
	ldr	r9, .L873+276
	bl	seq_puts
	ldr	r7, .L873+60
	ldr	r3, .L873+64
	mov	r0, r5
	ldr	r2, .L873+8
	ldr	r1, .L873+68
	str	r3, [sp, #12]
	str	r8, [sp, #8]
	ldr	r3, .L873+72
	str	r9, [sp, #4]
	str	r7, [sp]
	bl	seq_printf
	ldr	r3, .L873+76
	mov	r2, r6
	ldr	r1, .L873+80
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
	ldr	r1, .L873+84
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L873+88
	ldr	r2, .L873+8
	mov	r0, r5
	ldr	r1, .L873+92
	str	r3, [sp, #8]
	ldr	r3, .L873+96
	str	r3, [sp, #4]
	ldr	r3, .L873+100
	str	r3, [sp]
	ldr	r3, .L873+104
	bl	seq_printf
	ldr	r0, [r4, #176]
	bl	strof_drop
	ldr	r3, [r4, #184]
	mov	r2, r6
	ldr	r1, .L873+108
	str	r3, [sp, #8]
	ldr	r3, [r4, #636]
	str	r3, [sp, #4]
	ldr	r3, [r4, #180]
	str	r3, [sp]
	mov	r3, r0
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L873+112
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L873+116
	ldr	r2, .L873+8
	mov	r0, r5
	ldr	r1, .L873+120
	str	r3, [sp, #8]
	ldr	r3, .L873+124
	str	r3, [sp, #4]
	ldr	r3, .L873+128
	str	r3, [sp]
	ldr	r3, .L873+132
	bl	seq_printf
	ldr	r0, [r4, #188]
	bl	strof_suprmode
	ldr	r3, [r4, #200]
	mov	r2, r6
	ldr	r1, .L873+136
	str	r3, [sp, #8]
	ldr	r3, [r4, #196]
	str	r3, [sp, #4]
	ldr	r3, [r4, #192]
	str	r3, [sp]
	mov	r3, r0
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L873+140
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L873+144
	mov	r0, r5
	ldr	r2, .L873+8
	ldr	r1, .L873+148
	str	r3, [sp, #8]
	ldr	r3, .L873+152
	str	r3, [sp, #4]
	ldr	r3, .L873+156
	str	r3, [sp]
	ldr	r3, .L873+160
	bl	seq_printf
	ldr	r0, [r4, #36]
	cmp	r0, #1
	bne	.L862
	bl	strof_gop_mode
	ldr	r3, [r4, #112]
	ldr	r2, [r4, #44]
	ldr	r1, .L873+164
	strd	r3, r2, [sp]
	mov	r2, r6
	str	r3, [sp, #8]
	mov	r3, r0
.L871:
	mov	r0, r5
	ldr	r10, .L873+72
	bl	seq_printf
	ldr	r3, [r4, #8]
	cmp	r3, #1
	beq	.L864
	cmp	r3, #0
	beq	.L865
	cmp	r3, #3
	beq	.L866
.L867:
	ldr	r1, .L873+168
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L873+172
	ldr	r2, .L873+8
	mov	r0, r5
	ldr	r1, .L873+176
	str	r3, [sp, #28]
	ldr	r3, .L873+180
	str	r3, [sp, #24]
	ldr	r3, .L873+184
	str	r3, [sp, #20]
	ldr	r3, .L873+188
	str	r3, [sp, #16]
	ldr	r3, .L873+192
	str	r3, [sp, #12]
	ldr	r3, .L873+196
	str	r3, [sp, #8]
	ldr	r3, .L873+200
	str	r3, [sp, #4]
	ldr	r3, .L873+204
	str	r3, [sp]
	ldr	r3, .L873+208
	bl	seq_printf
	ldr	r0, [r4, #212]
	bl	strof_bool
	ldr	r3, [r4, #228]
	mov	r2, r6
	ldr	r1, .L873+212
	str	r3, [sp, #28]
	ldr	r3, [r4, #224]
	str	r3, [sp, #24]
	ldr	r3, [r4, #220]
	str	r3, [sp, #20]
	ldr	r3, [r4, #216]
	str	r3, [sp, #16]
	ldr	r3, [r4, #244]
	str	r3, [sp, #12]
	ldr	r3, [r4, #240]
	str	r3, [sp, #8]
	ldr	r3, [r4, #236]
	str	r3, [sp, #4]
	ldr	r3, [r4, #232]
	str	r3, [sp]
	mov	r3, r0
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L873+216
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L873+220
	mov	r0, r5
	ldr	r2, .L873+8
	ldr	r1, .L873+224
	str	r3, [sp, #4]
	ldr	r3, .L873+228
	str	r3, [sp]
	ldr	r3, .L873+208
	bl	seq_printf
	ldr	r8, [r4, #204]
	mov	r0, r8
	cmp	r8, #0
	beq	.L868
	bl	strof_bool
	movs	r3, #0
	ldr	r1, .L873+232
	mov	r2, r6
	str	r3, [sp, #4]
	ldr	r3, [r4, #208]
	str	r3, [sp]
	mov	r3, r0
.L872:
	mov	r0, r5
	bl	seq_printf
	mov	r0, r5
	ldr	r1, .L873+236
	bl	seq_puts
	ldr	r3, .L873+240
	mov	r0, r5
	ldr	r2, .L873+8
	ldr	r1, .L873+244
	str	r3, [sp, #20]
	ldr	r3, .L873+248
	strd	r10, r7, [sp, #24]
	str	r3, [sp, #16]
	ldr	r3, .L873+252
	str	r3, [sp, #12]
	ldr	r3, .L873+256
	str	r3, [sp, #8]
	ldr	r3, .L873+260
	str	r3, [sp, #4]
	ldr	r3, .L873+264
	str	r3, [sp]
	ldr	r3, .L873+268
	bl	seq_printf
	ldr	r1, [r4, #516]
	mov	r0, r5
	asrs	r2, r1, #31
	ldr	r3, [sp, #52]
	smull	r10, fp, r1, r3
	ldr	r1, [r4, #676]
	str	r1, [sp, #28]
	ldr	r1, [r4, #672]
	str	r1, [sp, #24]
	ldr	r1, [r4, #92]
	str	r1, [sp, #20]
	ldr	r1, [r4, #72]
	str	r1, [sp, #16]
	ldr	r1, [r4, #520]
	b	.L874
.L875:
	.align	2
.L873:
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
.L874:
	smull	r8, r9, r1, r3
	asrs	r1, r1, #31
	ldr	r3, [sp, #48]
	str	r3, [sp, #8]
	rsb	r3, r2, fp, asr #6
	rsb	r1, r1, r9, asr #6
	str	r1, [sp, #12]
	ldr	r1, [r4, #568]
	mov	r2, r6
	str	r1, [sp, #4]
	movs	r1, #0
	str	r1, [sp]
	ldr	r1, .L876
	bl	seq_printf
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L860:
	bl	__aeabi_idiv
	mov	r10, r0
	ldrd	r0, r1, [r4, #28]
	bl	__aeabi_idiv
	mov	fp, r0
	mov	r0, r9
	bl	strof_rc_mode
	ldr	r3, .L876+4
	mov	r2, r6
	ldr	r1, .L876+8
	strd	r3, r3, [sp, #20]
	ldr	r3, [sp, #48]
	strd	r10, fp, [sp, #4]
	str	r8, [sp]
	strd	r0, r3, [sp, #12]
	mov	r3, r7
	b	.L870
.L862:
	bl	strof_gop_mode
	ldr	r3, .L876+4
	mov	r2, r6
	ldr	r1, .L876+12
	strd	r3, r3, [sp, #4]
	ldr	r3, [r4, #112]
	str	r3, [sp]
	mov	r3, r0
	b	.L871
.L864:
	mov	r0, r5
	ldr	r1, .L876+16
	bl	seq_puts
	ldr	r3, .L876+20
	mov	r0, r5
	ldr	r2, .L876+24
	ldr	r1, .L876+28
	str	r3, [sp, #36]
	ldr	r3, .L876+32
	strd	r10, r8, [sp, #8]
	str	r7, [sp, #4]
	str	r3, [sp, #32]
	ldr	r3, .L876+36
	str	r3, [sp, #28]
	ldr	r3, .L876+40
	str	r3, [sp, #24]
	ldr	r3, .L876+44
	strd	r9, r3, [sp, #16]
	ldr	r3, .L876+48
	str	r3, [sp]
	ldr	r3, .L876+52
	bl	seq_printf
	ldr	r3, [r4, #156]
	mov	r2, r6
	ldr	r1, .L876+56
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
	b	.L867
.L865:
	ldrd	r3, r1, [r4, #52]
	movs	r0, #100
	muls	r0, r3, r0
	bl	__aeabi_idiv
	ldr	r1, .L876+60
	mov	fp, r0
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L876+20
	mov	r0, r5
	ldr	r2, .L876+24
	ldr	r1, .L876+64
	str	r3, [sp, #40]
	ldr	r3, .L876+32
	strd	r10, r8, [sp, #12]
	str	r7, [sp, #8]
	str	r3, [sp, #36]
	ldr	r3, .L876+36
	str	r3, [sp, #32]
	ldr	r3, .L876+40
	str	r3, [sp, #28]
	ldr	r3, .L876+44
	strd	r9, r3, [sp, #20]
	ldr	r3, .L876+48
	str	r3, [sp, #4]
	ldr	r3, .L876+52
	str	r3, [sp]
	ldr	r3, .L876+68
	bl	seq_printf
	ldr	r3, [r4, #156]
	mov	r2, r6
	ldr	r1, .L876+72
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
	b	.L867
.L866:
	mov	r0, r5
	ldr	r1, .L876+76
	bl	seq_puts
	ldr	r3, .L876+80
	mov	r0, r5
	ldr	r2, .L876+24
	ldr	r1, .L876+84
	str	r3, [sp, #8]
	ldr	r3, .L876+88
	str	r3, [sp, #4]
	ldr	r3, .L876+92
	str	r3, [sp]
	ldr	r3, .L876+96
	bl	seq_printf
	movs	r3, #0
	mov	r2, r6
	ldr	r1, .L876+100
	mov	r0, r5
	str	r3, [sp, #8]
	ldr	r3, [r4, #528]
	str	r3, [sp, #4]
	ldr	r3, [r4, #524]
	str	r3, [sp]
	ldr	r3, [r4, #164]
	bl	seq_printf
	b	.L867
.L868:
	bl	strof_bool
	ldr	r3, .L876+4
	mov	r2, r6
	ldr	r1, .L876+104
	str	r3, [sp]
	mov	r3, r0
	str	r8, [sp, #4]
	b	.L872
.L877:
	.align	2
.L876:
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
	.type	__func__.25558, %object
	.size	__func__.25558, 24
__func__.25558:
	.ascii	"bits_model_param_deinit\000"
	.type	__func__.25572, %object
	.size	__func__.25572, 14
__func__.25572:
	.ascii	"bits_frm_init\000"
	.type	__FUNCTION__.25577, %object
	.size	__FUNCTION__.25577, 14
__FUNCTION__.25577:
	.ascii	"bits_frm_init\000"
	.type	__func__.25604, %object
	.size	__func__.25604, 18
__func__.25604:
	.ascii	"bits_model_update\000"
	.type	__func__.25622, %object
	.size	__func__.25622, 17
__func__.25622:
	.ascii	"bits_model_alloc\000"
	.type	__FUNCTION__.25624, %object
	.size	__FUNCTION__.25624, 17
__FUNCTION__.25624:
	.ascii	"bits_model_alloc\000"
	.type	__func__.25675, %object
	.size	__func__.25675, 15
__func__.25675:
	.ascii	"calc_cbr_ratio\000"
	.type	__FUNCTION__.25676, %object
	.size	__FUNCTION__.25676, 15
__FUNCTION__.25676:
	.ascii	"calc_cbr_ratio\000"
	.type	__func__.25726, %object
	.size	__func__.25726, 15
__func__.25726:
	.ascii	"calc_vbr_ratio\000"
	.type	__func__.25646, %object
	.size	__func__.25646, 18
__func__.25646:
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
	.type	__func__.25657, %object
	.size	__func__.25657, 17
__func__.25657:
	.ascii	"calc_debreath_qp\000"
	.type	strength_map.25656, %object
	.size	strength_map.25656, 36
strength_map.25656:
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
	.type	__func__.25684, %object
	.size	__func__.25684, 27
__func__.25684:
	.ascii	"reenc_calc_super_frm_ratio\000"
	.type	__func__.25704, %object
	.size	__func__.25704, 21
__func__.25704:
	.ascii	"reenc_calc_cbr_ratio\000"
	.type	__func__.25745, %object
	.size	__func__.25745, 21
__func__.25745:
	.ascii	"reenc_calc_vbr_ratio\000"
	.type	__func__.25766, %object
	.size	__func__.25766, 18
__func__.25766:
	.ascii	"moving_ratio_calc\000"
	.type	__func__.25797, %object
	.size	__func__.25797, 16
__func__.25797:
	.ascii	"calc_avbr_ratio\000"
	.type	__func__.25806, %object
	.size	__func__.25806, 16
__func__.25806:
	.ascii	"bits_model_init\000"
	.type	__func__.25816, %object
	.size	__func__.25816, 16
__func__.25816:
	.ascii	"bits_mode_reset\000"
	.type	__func__.25825, %object
	.size	__func__.25825, 18
__func__.25825:
	.ascii	"check_super_frame\000"
	.type	__func__.25838, %object
	.size	__func__.25838, 13
__func__.25838:
	.ascii	"check_re_enc\000"
	.type	__func__.25848, %object
	.size	__func__.25848, 17
__func__.25848:
	.ascii	"rc_model_v2_init\000"
	.type	__func__.25853, %object
	.size	__func__.25853, 19
__func__.25853:
	.ascii	"rc_model_v2_deinit\000"
	.type	__func__.25862, %object
	.size	__func__.25862, 18
__func__.25862:
	.ascii	"rc_model_v2_start\000"
	.type	__func__.25898, %object
	.size	__func__.25898, 22
__func__.25898:
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
	.type	__func__.25917, %object
	.size	__func__.25917, 20
__func__.25917:
	.ascii	"rc_model_v2_hal_end\000"
	.type	__func__.25926, %object
	.size	__func__.25926, 24
__func__.25926:
	.ascii	"rc_model_v2_check_reenc\000"
	.type	__func__.25940, %object
	.size	__func__.25940, 16
__func__.25940:
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
	.ascii	"\0013%d: i_bit init fail\012\000"
.LC3:
	.ascii	"\0013%d: vi_bit init fail\012\000"
.LC4:
	.ascii	"\0013%d: p_bit init fail\012\000"
.LC5:
	.ascii	"\0013%d: pre_p_bit init fail\012\000"
.LC6:
	.ascii	"\0013%d: pre_i_bit init fail\012\000"
.LC7:
	.ascii	"\0013%d: pre_i_mean_qp init fail\012\000"
.LC8:
	.ascii	"\0013%d: madi init fail\012\000"
.LC9:
	.ascii	"\0013%d: madp init fail\012\000"
.LC10:
	.ascii	"\0013%d: stat_rate init fail fps_out_num %d\012\000"
.LC11:
	.ascii	"\0013%d: stat_bits init fail stat_len %d\012\000"
.LC12:
	.ascii	"\0013%d: gop_bits init fail gop_len %d\012\000"
.LC13:
	.ascii	"\0013%d: motion_level init fail mad_len %d\012\000"
.LC14:
	.ascii	"\0013%d: complex_level init fail mad_len %d\012\000"
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
	.ascii	", vi_num %d, vi_scale %d\012\000"
.LC22:
	.ascii	"\0016%s:%d: gop_total_bits %lld, i_sumbits %d, p_su"
	.ascii	"mbits %d, vgop %d igop %d\012\000"
.LC23:
	.ascii	"\0016%s:%d: i_scale  %d, total_bits %lld\012\000"
.LC24:
	.ascii	"\0016%s:%d: alloc bits max then super_bit_thr %d\012"
	.ascii	"\000"
.LC25:
	.ascii	"alloc_bits\000"
.LC26:
	.ascii	"r_bits\000"
.LC27:
	.ascii	"\0016%s:%d: %10s|%10s|%10s|%10s|%10s|%8s\012\000"
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
	.ascii	"\0016%s:%d: %10d %10d %10d %10d %10d %8d\012\000"
.LC34:
	.ascii	"target_bps > 0\000"
.LC35:
	.ascii	"diff_ratio\000"
.LC36:
	.ascii	"\0016%s:%d: %10s|%10s|%10s|%10s|%10s|%10s\012\000"
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
	.ascii	"\0016%s:%d: %10d %10d %10d %10d %10d|%10d\012\000"
.LC43:
	.ascii	"max_bps\000"
.LC44:
	.ascii	"bps_ch\000"
.LC45:
	.ascii	"\0016%s:%d: %10d %10d %10d %10d %10d %10d\012\000"
.LC46:
	.ascii	"\0016%s:%d: %10s|%10s|%10s|%10s|%10s\012\000"
.LC47:
	.ascii	"\0016%s:%d: %10d %10d %10d %10d|%10d\012\000"
.LC48:
	.ascii	"\0016%s:%d: ctx->next_i_ratio %d\012\000"
.LC49:
	.ascii	"\0016%s:%d: i start_qp %d, qp_start_sum = %d, intra"
	.ascii	"_lv4_prop %d\012\000"
.LC50:
	.ascii	"\0016%s:%d: cbr target_bit %d real_bit %d reenc nex"
	.ascii	"t ratio %d\012\000"
.LC51:
	.ascii	"\0016%s:%d: vbr reenc next ratio %d\012\000"
.LC52:
	.ascii	"\0016%s:%d: pre_mean_qp = %d, ctx->pre_mean_qp %d\012"
	.ascii	"\000"
.LC53:
	.ascii	"\0016%s:%d: scale = %d, pre_I_bit %d\012\000"
.LC54:
	.ascii	"\0016%s:%d: pbit_sum %d,madi_sum = %d, madp_sum = %"
	.ascii	"d\012\000"
.LC55:
	.ascii	"\0016%s:%d: means qp percent %d min_still_percent %"
	.ascii	"d\012\000"
.LC56:
	.ascii	"\0016%s:%d: percent_a = %d percent_b %d\012\000"
.LC57:
	.ascii	"\0016%s:%d: mv_ratio = %d\000"
.LC58:
	.ascii	"\0016%s:%d: mad_ratio = %d\012\000"
.LC59:
	.ascii	"\0016%s:%d: moving_ratio = %d, motion_sensitivity ="
	.ascii	" %d\012\000"
.LC60:
	.ascii	"\0016%s:%d: percent %d mad_ratio %d hr_ratio %d, mo"
	.ascii	"ving_ratio %d\012\000"
.LC61:
	.ascii	"\0016%s:%d: final moving_ratio = %d\012\000"
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
	.ascii	"it_ip_ratio %d\012\000"
.LC70:
	.ascii	"\0016%s:%d: infinte gop, set default for rc bit cal"
	.ascii	"c\012\000"
.LC71:
	.ascii	"\0016%s:%d: all intra gop\012\000"
.LC72:
	.ascii	"\0016%s:%d: min_still_percent  %d\000"
.LC73:
	.ascii	"\0016%d: rc mode set error\000"
.LC74:
	.ascii	"\0016%s:%d: gop %d total bit %lld per_frame %d stat"
	.ascii	"istics time %d second\012\000"
.LC75:
	.ascii	"\0016%s:%d: super frame drop current frame\012\000"
.LC76:
	.ascii	"\0016%s:%d: reenc check target_bps %d last_ins_bps "
	.ascii	"%d ins_bps %d\012\000"
.LC77:
	.ascii	"\0016%s:%d: drop mode %d frame_type %d\012\000"
.LC78:
	.ascii	"\0016%s:%d: drop mode %d check max_bps %d bit_thr %"
	.ascii	"d ins_bps %d\012\000"
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
	.ascii	"%7s|%7s|%8s|%6s|%6s|%8s|%10s|%5s|%5s \012\000"
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
	.ascii	"%7d|%7u|%8u|%6u|%6u|%8s|%10s|%5u|%5u \012\000"
.LC112:
	.ascii	"N/A\000"
.LC113:
	.ascii	"%7d|%7u|%8u|%6u|%6u|%8s|%10u|%5s|%5s \012\000"
.LC114:
	.ascii	"\012---------RC base param 2-----------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC115:
	.ascii	"MinQp\000"
.LC116:
	.ascii	"%7s|%8s|%8s|%8s|%8s|%12s \012\000"
.LC117:
	.ascii	"EnableIdr\000"
.LC118:
	.ascii	"MaxIQp\000"
.LC119:
	.ascii	"MinIQp\000"
.LC120:
	.ascii	"MaxQp\000"
.LC121:
	.ascii	"%7d|%8d|%8d|%8d|%8d|%12s \012\000"
.LC122:
	.ascii	"Y\000"
.LC123:
	.ascii	"\012---------RC run comm param 1-------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC124:
	.ascii	"bLost\000"
.LC125:
	.ascii	"%7s|%8s|%12s|%12s|%10s \012\000"
.LC126:
	.ascii	"EncGap\000"
.LC127:
	.ascii	"LostFrmStr\000"
.LC128:
	.ascii	"LostThr\000"
.LC129:
	.ascii	"%7d|%8s|%12u|%12d|%10u \012\000"
.LC130:
	.ascii	"\012---------RC run comm param 2-------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC131:
	.ascii	"SprFrmMod\000"
.LC132:
	.ascii	"%7s|%12s|%12s|%12s|%12s \012\000"
.LC133:
	.ascii	"RCPriority\000"
.LC134:
	.ascii	"SprPFrm\000"
.LC135:
	.ascii	"SprIFrm\000"
.LC136:
	.ascii	"%7d|%12s|%12u|%12u|%12u \012\000"
.LC137:
	.ascii	"\012---------RC gop mode attr----------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC138:
	.ascii	"GopMode\000"
.LC139:
	.ascii	"%7s|%10s|%10s|%12s|%10s \012\000"
.LC140:
	.ascii	"ViQpDelta\000"
.LC141:
	.ascii	"BgInterval\000"
.LC142:
	.ascii	"IpQpDelta\000"
.LC143:
	.ascii	"%7d|%10s|%10d|%12u|%10d\012\000"
.LC144:
	.ascii	"%7d|%10s|%10d|%12s|%10s\012\000"
.LC145:
	.ascii	"\012---------RC run cbr param----------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC146:
	.ascii	"MinIprop\000"
.LC147:
	.ascii	"%7s|%10s|%10s|%8s|%8s|%8s|%8s|%10s|%10s|%10s|%10s|%"
	.ascii	"15s \012\000"
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
	.ascii	"%7d|%10u|%10u|%8u|%8u|%8u|%8u|%10d|%10d|%10d|%10d|%"
	.ascii	"15d\012\000"
.LC155:
	.ascii	"\012---------RC run vbr common param---------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC156:
	.ascii	"ChgPs\000"
.LC157:
	.ascii	"%7s|%8s|%10s|%10s|%8s|%8s|%8s|%8s|%10s|%10s|%10s|%1"
	.ascii	"0s|%15s \012\000"
.LC158:
	.ascii	"%7d|%8d|%10u|%10u|%8u|%8u|%8u|%8u|%10d|%10d|%10d|%1"
	.ascii	"0d|%15d\012\000"
.LC159:
	.ascii	"\012---------RC run avbr param---------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC160:
	.ascii	"MaxStillQP\000"
.LC161:
	.ascii	"%7s|%15s|%15s|%15s|%15s \012\000"
.LC162:
	.ascii	"MinQpDelta\000"
.LC163:
	.ascii	"MinPercent\000"
.LC164:
	.ascii	"MotionSensi\000"
.LC165:
	.ascii	"%7d|%15u|%15u|%15d|%15u \012\000"
.LC166:
	.ascii	"\012--------RC HierarchicalQp INFO-----------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC167:
	.ascii	"bEnable\000"
.LC168:
	.ascii	"%7s|%10s|%12s|%12s|%12s|%12s|%12s|%12s|%12s|%12s\012"
	.ascii	"\000"
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
	.ascii	"%7d|%10s|%12d|%12d|%12d|%12d|%12d|%12d|%12d|%12d\012"
	.ascii	"\000"
.LC178:
	.ascii	"\012--------RC debreath_effect info----------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC179:
	.ascii	"%7s|%10s|%10s|%18s\012\000"
.LC180:
	.ascii	"DeBrthEfctCnt\000"
.LC181:
	.ascii	"Strength0\000"
.LC182:
	.ascii	"%7d|%10s|%10d|%18u\012\000"
.LC183:
	.ascii	"%7d|%10s|%10s|%18u\012\000"
.LC184:
	.ascii	"\012--------RC run CBR/VBR info1-------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"--------\012\000"
.LC185:
	.ascii	"InsBr(kbps)\000"
.LC186:
	.ascii	"%7s|%14s|%8s|%8s|%12s|%12s|%10s|%10s|%8s|%8s\012\000"
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
	.ascii	"%7d|%14d|%8u|%8d|%12u|%12u|%10d|%10u|%8u|%8u\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
