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
	.file	"rc_model_v2_smt.c"
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
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_coef, %function
set_coef:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r7, r0
	mov	r5, r1
	ldr	r0, [r0, #344]
	movs	r1, #0
	mov	r4, r2
	bl	mpp_data_get_pre_val_v2
	movs	r1, #1
	mov	r6, r0
	ldr	r0, [r7, #344]
	bl	mpp_data_get_pre_val_v2
	mov	r8, r0
	ldr	r0, [r7, #344]
	bl	mpp_data_sum_v2
	cbnz	r0, .L5
.L28:
	str	r4, [r5]
	pop	{r4, r5, r6, r7, r8, pc}
.L5:
	cmp	r0, #1
	bne	.L7
	cbnz	r6, .L8
	adds	r4, r4, #10
	b	.L28
.L8:
	adds	r4, r4, #25
	b	.L28
.L7:
	cmp	r0, #2
	bne	.L9
	cmp	r6, #0
	beq	.L8
.L11:
	adds	r4, r4, #35
	b	.L28
.L9:
	cmp	r0, #3
	bne	.L10
	cmp	r6, #0
	beq	.L11
.L14:
	adds	r4, r4, #51
	b	.L28
.L10:
	subs	r3, r0, #4
	cmp	r3, #2
	bhi	.L12
	cbnz	r6, .L13
	cmp	r8, #0
	beq	.L11
	b	.L14
.L13:
	adds	r4, r4, #64
	b	.L28
.L12:
	subs	r0, r0, #7
	cmp	r0, #2
	bhi	.L15
	orrs	r3, r6, r8
	beq	.L13
	adds	r4, r4, #72
	b	.L28
.L15:
	adds	r4, r4, #80
	b	.L28
	.fnend
	.size	set_coef, .-set_coef
	.global	__aeabi_idiv
	.global	__aeabi_uidiv
	.align	1
	.global	rc_model_v2_smt_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_smt_start, %function
rc_model_v2_smt_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #84
	sub	sp, sp, #84
	ldr	r3, [r0, #48]
	mov	r4, r0
	ldr	r6, [r0, #580]
	mov	r7, r1
	ldr	fp, [r0, #128]
	str	r3, [sp, #32]
	ldr	r3, [r0, #56]
	str	r3, [sp, #36]
	ldr	r3, [r0, #28]
	str	r3, [sp, #28]
	ldr	r3, [r0, #32]
	str	r3, [sp, #40]
	ldr	r3, [r0, #124]
	str	r3, [sp, #16]
	ldr	r3, [r0, #120]
	str	r3, [sp, #12]
	ldr	r3, [r0, #132]
	ldr	r0, [r0, #340]
	str	r3, [sp, #20]
	bl	mpp_data_sum_v2
	movs	r1, #0
	str	r0, [sp, #44]
	ldr	r0, [r4, #340]
	bl	mpp_data_get_pre_val_v2
	str	r0, [sp, #48]
	ldr	r0, [r4, #344]
	bl	mpp_data_sum_v2
	ldrb	r3, [r7, #156]	@ zero_extendqisi2
	lsls	r1, r3, #29
	str	r0, [sp, #64]
	bmi	.L192
	ldr	r0, [r7, #296]
	bl	mpp_frame_get_ppinfo
	mov	r8, r0
	ldrd	r0, r1, [r4, #28]
	bl	__aeabi_idiv
	ldr	r3, [r4, #48]
	ldr	r10, [r4, #56]
	mov	r5, r0
	str	r3, [sp, #24]
	cmp	r8, #0
	beq	.L32
	ldr	r3, [r8, #8]
	lsls	r2, r3, #31
	bpl	.L32
	ubfx	r2, r3, #4, #9
	asrs	r3, r3, #16
	tst	r3, #254
	uxtb	r1, r3
	bne	.L33
	cmp	r1, #0
	bne	.L144
	movs	r3, #30
	cmp	r2, #0
	bne	.L205
	str	r3, [sp, #12]
	b	.L32
.L33:
	movs	r3, #33
	str	r3, [sp, #12]
	orrs	r3, r2, r1
	beq	.L32
.L34:
	cmp	fp, #37
	it	ge
	movge	fp, #37
.L32:
	ldr	r0, [r4, #340]
	bl	mpp_data_sum_v2
	cbnz	r0, .L36
	ldr	r3, [r4, #256]
	cbz	r3, .L36
	ldr	r3, [r4, #136]
	strd	r3, r3, [sp, #12]
.L36:
	ldr	r3, [r7, #216]
	cmp	r3, #2
	bne	.L37
	ldr	r0, [r4, #344]
	bl	mpp_data_sum_v2
	cmp	r0, #15
	ldr	r3, [sp, #20]
	bgt	.L38
	cmp	r3, #34
	it	ge
	movge	r3, #34
	cmp	fp, #37
	str	r3, [sp, #20]
	it	ge
	movge	fp, #37
.L37:
	ldrb	r3, [r7, #152]	@ zero_extendqisi2
	tst	r3, #16
	ite	ne
	movne	r3, #2
	moveq	r3, #0
	str	r3, [r4, #276]
	ldrb	r2, [r7, #154]	@ zero_extendqisi2
	and	r2, r2, #63
	cmp	r2, #3
	it	eq
	moveq	r3, r2
	str	r3, [r4, #276]
	ldr	r3, [r4, #620]
	cbnz	r3, .L41
	ldr	r2, [r7, #216]
	cmp	r2, #1
	bne	.L42
	ldr	r2, [r7, #220]
	cmp	r2, #2
	bne	.L42
.L41:
	ldr	r2, [r4, #276]
	cmp	r2, #2
	ittte	ne
	addne	r3, r3, #1
	strne	r3, [r4, #620]
	movne	fp, #28
	moveq	r3, #0
	itte	ne
	strne	fp, [sp, #20]
	strdne	fp, fp, [sp, #12]
	streq	r3, [r4, #620]
.L42:
	ldr	r3, [r4, #304]
	cbz	r3, .L44
	cmp	r3, #1
	beq	.L45
	ldr	r3, [r4, #276]
	cmp	r3, #2
	bne	.L49
	add	r8, r4, #352
	mov	r0, r8
	bl	mpp_pid_calc
	ldr	r3, [r4, #356]
	mov	r9, r0
	subs	r3, r3, r0
	mov	r0, r8
	str	r3, [r4, #332]
	bl	mpp_pid_reset
	ldrd	r2, [r4, #312]
	ldr	r8, [r4, #396]
	orrs	r3, r2, r3
	beq	.L50
	ldr	r3, [r4, #404]
	add	r0, r8, r9
	str	r0, [r4, #388]
	add	r3, r3, r9
	b	.L206
.L144:
	movs	r3, #32
.L205:
	str	r3, [sp, #12]
	b	.L34
.L38:
	cmp	r3, #35
	it	ge
	movge	r3, #35
	cmp	fp, #38
	str	r3, [sp, #20]
	it	ge
	movge	fp, #38
	b	.L37
.L44:
	ldr	r3, [r4, #276]
	cmp	r3, #2
	bne	.L47
	ldr	r3, [r4, #396]
	str	r3, [r4, #388]
	ldr	r3, [r4, #404]
.L206:
	str	r3, [r4, #392]
.L48:
	ldr	r3, [r4, #336]
	cbnz	r3, .L55
	cmp	r5, #14
	bgt	.L56
	lsls	r1, r5, #2
.L57:
	ldr	r5, [r4, #420]
	add	r0, r4, #336
	cmp	r1, r5
	it	lt
	movlt	r1, r5
	cmp	r1, #50
	it	ge
	movge	r1, #50
	cmp	r1, #20
	it	lt
	movlt	r1, #20
	bl	mpp_data_init
.L55:
	ldrd	r3, r2, [r4, #388]
	ldr	r1, [sp, #40]
	add	r3, r3, r2
	ldr	r0, [sp, #28]
	cmp	r3, #199
	ite	gt
	asrgt	r3, r3, #1
	movle	r3, #100
	str	r3, [r7, #164]
	bl	__aeabi_idiv
	movs	r3, #1
	mov	r2, r3
	mov	r1, #-1
	ldr	r8, [r7, #164]
	str	r0, [sp, #28]
	ldr	r0, [r4, #336]
	bl	mpp_data_avg
	ldrd	r9, [r4, #288]
	orrs	r3, r9, r10
	str	r0, [sp, #52]
	bne	.L61
	ldr	r0, [r4, #92]
	cmp	r0, #0
	bge	.L62
	ldr	r2, .L221
	ldr	r3, [sp, #28]
	ldr	r1, [sp, #28]
	ldr	r0, [r4]
	smull	r2, r3, r3, r2
	asrs	r1, r1, #31
	adds	r0, r0, #15
	ldr	r5, .L221+4
	rsb	r1, r1, r3, asr #2
	ldr	r3, [r4, #392]
	adds	r1, r1, #1
	cmp	r1, #3
	it	ge
	movge	r1, #3
	cmp	r1, #1
	it	lt
	movlt	r1, #1
	muls	r1, r3, r1
	asrs	r3, r0, #4
	ldr	r0, [r4, #4]
	adds	r0, r0, #15
	asrs	r0, r0, #4
	muls	r0, r3, r0
	movs	r3, #0
.L63:
	adds	r3, r3, #1
	cmp	r3, #8
	beq	.L64
	ldr	r2, [r5, r3, lsl #2]
	cmp	r0, r2
	bcs	.L63
.L64:
	add	r3, r5, r3, lsl #2
	ldr	r3, [r3, #132]
	muls	r0, r3, r0
	sub	r0, r0, #350
	bl	__aeabi_uidiv
	ldr	r2, [r4, #300]
	cmp	r0, #95
	ldr	r3, [sp, #16]
	it	ge
	movge	r0, #95
	cmp	r0, #4
	ite	ge
	addge	r5, r5, r0
	addlt	r5, r5, #4
	cmp	r3, #31
	ldrb	r0, [r5, #36]	@ zero_extendqisi2
	ite	gt
	movgt	r1, r3
	movle	r1, #31
	bl	mpp_clip
.L62:
	str	r0, [r4, #576]
.L61:
	ldrd	r3, r2, [sp, #32]
	add	r3, r3, r2
	add	r0, r3, r3, lsr #31
	asrs	r3, r0, #1
	str	r3, [sp, #24]
	ldr	r3, [r4, #276]
	cmp	r3, #2
	bne	.L67
	cmp	r9, #1
	sbcs	r3, r10, #0
	blt	.L69
	ldrd	r1, r2, [r4, #296]
	ldr	r0, [sp, #52]
	bl	mpp_clip
	ldrd	r3, r5, [r4, #584]
	cmp	r3, r8
	str	r0, [sp, #32]
	blt	.L70
	add	r2, r8, r8, lsl #2
	cmp	r3, r2
	ble	.L71
	add	r8, r5, #3
.L72:
	ldr	r3, [r4, #604]
	cmp	r3, #0
	bne	.L77
	ldr	r3, [r4, #204]
	cmp	r3, #0
	beq	.L77
	ldrd	r10, r3, [r4, #592]
	str	r3, [sp, #36]
	ldr	r3, [r4, #600]
	asr	r9, r3, #5
	ldr	r3, .L221+8
	cmp	r9, #7
	ldr	r3, [r3]
	it	ge
	movge	r9, #7
	lsls	r3, r3, #31
	bpl	.L79
	mov	r3, r4
	movw	r2, #463
	ldr	r1, .L221+12
	ldr	r0, .L221+16
	bl	printk
.L79:
	mov	r0, r10
	ldr	r1, [sp, #36]
	bl	__aeabi_idiv
	ldr	r3, .L221+8
	ldr	r3, [r3]
	cmp	r0, #35
	it	ge
	movge	r0, #35
	mov	r10, r0
	lsls	r0, r3, #24
	bpl	.L80
	ldr	r3, [r4, #600]
	mov	r2, #468
	ldr	r1, .L221+12
	ldr	r0, .L221+20
	strd	r10, r3, [sp]
	ldr	r3, [r4, #576]
	bl	printk
.L80:
	ldr	r2, .L221+4
	ldr	r3, [r4, #208]
	uxtab	r9, r2, r9
	add	r3, r3, r2
	ldrsb	r2, [r9, #225]
	ldrsb	r3, [r3, #189]
	subs	r1, r3, r2
	cmp	r10, r1
	itt	gt
	subgt	r10, r10, r3
	addgt	r10, r10, r2
	ldrd	r2, r1, [r4, #104]
	mov	r0, r10
	bl	mpp_clip
	movs	r3, #0
	strd	r3, r3, [r4, #592]
	ldr	r3, .L221+8
	ldr	r3, [r3]
	lsls	r1, r3, #31
	str	r0, [r4, #576]
	bpl	.L77
	mov	r3, r4
	movw	r2, #479
	ldr	r1, .L221+12
	ldr	r0, .L221+24
	bl	printk
.L77:
	ldr	r3, .L221+4
	mov	r0, r8
	ldr	r2, [sp, #32]
	add	r2, r2, r3
	mov	r1, r2
	ldrb	r2, [r2, #233]	@ zero_extendqisi2
	ldrb	r1, [r1, #285]	@ zero_extendqisi2
	bl	mpp_clip
	add	r6, r6, r3
	ldrb	r2, [r6, #233]	@ zero_extendqisi2
	ldrb	r1, [r6, #285]	@ zero_extendqisi2
	bl	mpp_clip
	add	r5, r5, r3
	cmp	r0, #27
	mov	r1, r0
	ldrb	r2, [r5, #337]	@ zero_extendqisi2
	ble	.L82
	ldrb	r1, [r5, #389]	@ zero_extendqisi2
.L210:
	bl	mpp_clip
	ldr	r6, [r4, #332]
	cmp	r6, #0
	str	r0, [r4, #576]
	bge	.L69
	ldr	r1, [sp, #28]
	ldr	r0, [r4, #420]
	bl	__aeabi_idiv
	ldr	r5, [r4, #576]
	ldr	r3, [sp, #24]
	muls	r3, r0, r3
	mov	r0, r3
	mvn	r3, #4
	muls	r3, r6, r3
	cmp	r3, r0
	ble	.L86
	cmp	r5, #51
	it	ge
	movge	r5, #51
	cmp	r5, #20
	it	lt
	movlt	r5, #20
.L215:
	str	r5, [r4, #576]
.L69:
	ldr	r3, [sp, #44]
	cmp	r3, #700
	bge	.L161
	ldr	r3, [sp, #48]
	cmp	r3, #200
	beq	.L161
	ldr	r3, [sp, #44]
	cmp	r3, #400
	bge	.L162
	ldr	r3, [sp, #48]
	movs	r1, #4
	cmp	r3, #100
	bne	.L163
.L217:
	movs	r3, #5
	b	.L131
.L47:
	ldr	r0, [r4, #568]
	ldr	r8, [r4, #400]
	bl	mpp_data_mean_v2
	sub	r0, r8, r0
	str	r0, [r4, #388]
	ldr	r0, [r4, #572]
	ldr	r8, [r4, #408]
	bl	mpp_data_mean_v2
.L219:
	sub	r0, r8, r0
.L207:
	str	r0, [r4, #392]
	b	.L48
.L45:
	ldr	r8, [r4, #396]
.L50:
	add	r0, r4, #424
	bl	mpp_pid_calc
	sub	r0, r8, r0
	str	r0, [r4, #388]
	add	r0, r4, #460
	ldr	r8, [r4, #404]
	bl	mpp_pid_calc
	b	.L219
.L49:
	ldr	r3, [r4, #280]
	cmp	r3, #2
	bne	.L51
	ldr	r3, [r4, #420]
	ldr	r0, [sp, #24]
	ldr	r2, [r4, #584]
	add	r9, r3, #-1
	ldr	r1, [r4, #332]
	muls	r0, r3, r0
	sub	r8, r1, r2
	mov	r1, r5
	str	r3, [sp, #24]
	bl	__aeabi_idiv
	mov	r1, r9
	add	r0, r0, r8
	bl	__aeabi_idiv
	mov	r1, r5
	ldr	r3, [sp, #24]
	str	r0, [r4, #400]
	str	r0, [r4, #388]
	mul	r0, r3, r10
	bl	__aeabi_idiv
	mov	r1, r9
	add	r0, r0, r8
	bl	__aeabi_idiv
	str	r0, [r4, #408]
	b	.L207
.L51:
	ldr	r0, [r4, #568]
	bl	mpp_data_mean_v2
	str	r0, [sp, #52]
	ldr	r0, [r4, #572]
	bl	mpp_data_mean_v2
	ldrd	r2, r1, [r4, #28]
	mov	r8, r0
	ldr	r0, [sp, #24]
	bl	axb_div_c
	ldrd	r2, r1, [r4, #28]
	mov	r9, r0
	mov	r0, r10
	bl	axb_div_c
	ldr	r3, [r4, #400]
	lsl	r9, r9, #1
	ldr	r2, [sp, #52]
	lsls	r0, r0, #1
	subs	r3, r3, r2
	cmp	r3, r9
	ite	le
	strle	r3, [r4, #388]
	strgt	r9, [r4, #388]
	ldr	r3, [r4, #408]
	sub	r3, r3, r8
	cmp	r3, r0
	ite	le
	strle	r3, [r4, #392]
	strgt	r0, [r4, #392]
	b	.L48
.L56:
	cmp	r5, #24
	lsl	r1, r5, #1
	it	le
	addle	r1, r1, r5
	b	.L57
.L71:
	lsl	r2, r8, #1
	cmp	r3, r2
	ble	.L73
	add	r8, r5, #2
	b	.L72
.L73:
	add	r8, r8, r2
	cmp	r8, r3, lsl #1
	bge	.L148
	add	r8, r5, #1
	b	.L72
.L70:
	lsls	r2, r3, #1
	add	r3, r3, r2
	cmp	r3, r8
	bge	.L74
	sub	r8, r5, #3
	b	.L72
.L74:
	cmp	r2, r8
	bge	.L75
	sub	r8, r5, #2
	b	.L72
.L222:
	.align	2
.L221:
	.word	1717986919
	.word	.LANCHOR0
	.word	rc_debug
	.word	.LANCHOR0+168
	.word	.LC1
	.word	.LC2
	.word	.LC3
.L75:
	cmp	r3, r8, lsl #1
	bge	.L148
	add	r8, r5, #-1
	b	.L72
.L148:
	mov	r8, r5
	b	.L72
.L82:
	cmp	r0, #22
	ite	gt
	ldrbgt	r1, [r5, #441]	@ zero_extendqisi2
	ldrble	r1, [r5, #493]	@ zero_extendqisi2
	b	.L210
.L86:
	mvn	r3, #19
	muls	r3, r6, r3
	cmp	r0, r3
	bge	.L69
	cmp	r5, #51
	it	ge
	movge	r5, #51
	cmp	r5, #15
	it	lt
	movlt	r5, #15
	b	.L215
.L67:
	ldr	r3, [r4, #280]
	cmp	r3, #2
	bne	.L87
	cmp	r6, #32
	ble	.L149
	cmp	r6, #35
	ite	lt
	movlt	r3, #2
	movge	r3, #1
.L88:
	add	r6, r6, r3
	str	r6, [r4, #576]
	b	.L69
.L149:
	movs	r3, #3
	b	.L88
.L87:
	ldr	r0, [r4, #340]
	mov	r3, #1024
	str	r3, [sp, #76]
	bl	mpp_data_sum_v2
	movs	r1, #0
	mov	r5, r0
	ldr	r0, [r4, #340]
	bl	mpp_data_get_pre_val_v2
	movs	r1, #1
	mov	r8, r0
	ldr	r0, [r4, #340]
	bl	mpp_data_get_pre_val_v2
	cmp	r5, #99
	bgt	.L89
	movs	r2, #0
.L211:
	mov	r0, r4
	add	r1, sp, #76
	bl	set_coef
	ldr	r3, [sp, #76]
	cmp	r3, #1024
	itt	gt
	movgt	r3, #1024
	strgt	r3, [sp, #76]
	ldr	r0, [sp, #76]
	cmp	r0, #900
	bge	.L151
	cmp	r0, #306
	ble	.L109
	mvn	r2, #306
	add	r0, r0, r2
	ldr	r3, .L223
	lsls	r0, r0, #9
	umull	r0, r1, r0, r3
	lsrs	r3, r1, #7
	add	r0, r3, #512
.L110:
	cmp	r0, #1024
	it	ge
	movge	r0, #1024
	mov	r9, r0
.L108:
	ldrd	r5, r2, [r4, #388]
	ldr	r0, [r4, #536]
	subs	r3, r2, r5
	ldrd	r1, r2, [r4, #412]
	str	r3, [sp, #56]
	subs	r3, r2, r1
	ldr	r2, [r4, #500]
	str	r3, [sp, #60]
	lsls	r1, r1, #10
	add	r2, r2, r0
	str	r1, [sp, #36]
	ldr	r1, [sp, #60]
	asrs	r3, r2, #1
	lsls	r2, r5, #10
	str	r2, [sp, #32]
	ldr	r0, [sp, #32]
	ldr	r2, [sp, #56]
	str	r3, [sp, #68]
	ldr	r3, [r4, #576]
	mla	r2, r9, r2, r0
	ldr	r0, [sp, #36]
	mla	r5, r9, r1, r0
	movs	r0, #100
	asrs	r2, r2, #10
	cmp	r2, #100
	it	lt
	movlt	r2, #100
	asrs	r5, r5, #10
	lsl	ip, r2, #1
	add	r8, ip, r2
	eor	r1, r5, r5, asr #31
	sub	r1, r1, r5, asr #31
	mul	r10, r0, r1
	cmp	r10, r8
	bgt	.L111
.L118:
	subs	r0, r6, #1
	b	.L112
.L89:
	cmp	r5, #199
	bgt	.L91
	cmp	r8, #99
	bgt	.L92
	movs	r2, #102
	b	.L211
.L92:
	movs	r2, #154
	b	.L211
.L91:
	cmp	r5, #300
	bge	.L93
	cmp	r8, #99
	ble	.L92
	cmp	r8, #100
	bne	.L94
	cmp	r0, #99
	bgt	.L95
	movs	r2, #205
	b	.L211
.L95:
	cmp	r0, #100
	bne	.L94
	mov	r2, #256
	b	.L211
.L94:
	movw	r2, #307
	b	.L211
.L93:
	cmp	r5, #600
	bge	.L96
	cmp	r8, #99
	bgt	.L97
	cmp	r0, #99
	ble	.L94
	cmp	r0, #100
	bne	.L98
.L100:
	mov	r2, #358
	b	.L211
.L98:
	mov	r2, #410
	b	.L211
.L97:
	cmp	r8, #100
	bne	.L99
	cmp	r0, #99
	ble	.L100
	cmp	r0, #100
	beq	.L98
.L99:
	movw	r2, #461
	b	.L211
.L96:
	cmp	r5, #900
	bge	.L101
	cmp	r8, #99
	bgt	.L102
	cmp	r0, #99
	ble	.L98
	cmp	r0, #100
	beq	.L99
.L104:
	mov	r2, #512
	b	.L211
.L102:
	cmp	r8, #100
	bne	.L103
	cmp	r0, #99
	ble	.L104
	cmp	r0, #100
	bne	.L103
	movw	r2, #563
	b	.L211
.L103:
	movw	r2, #614
	b	.L211
.L101:
	movw	r3, #1499
	cmp	r5, r3
	bgt	.L105
	movw	r2, #666
	b	.L211
.L105:
	movw	r3, #1899
	cmp	r5, r3
	ite	le
	movle	r2, #768
	movgt	r2, #900
	b	.L211
.L109:
	movw	r1, #307
	lsls	r0, r0, #9
	bl	__aeabi_idiv
	b	.L110
.L151:
	mov	r9, #1024
	b	.L108
.L111:
	muls	r0, r5, r0
	cmp	r8, r0
	bge	.L113
	cmp	r5, r2
	blt	.L114
	cmp	r3, #29
	ble	.L213
	subs	r0, r6, #4
.L112:
	ldrd	r10, r3, [r4, #296]
	mov	r2, r3
	mov	r1, r10
	str	r3, [sp, #40]
	bl	mpp_clip
	cmp	r0, #34
	mov	r8, r0
	ble	.L121
	ldrd	r0, r1, [sp, #24]
	bl	__aeabi_idiv
	ldr	r2, .L223+4
	rsbs	r0, r0, #0
	smull	r2, r3, r0, r2
	asrs	r0, r0, #31
	rsb	r0, r0, r3, asr #1
	cmp	r5, r0, lsl #1
	ble	.L153
	cmp	r5, r0
	ite	le
	movle	r5, #102
	movgt	r5, #51
.L122:
	add	r3, r5, r9
	cmp	r3, #1024
	bge	.L155
	cmp	r8, #35
	it	ne
	movne	r3, #1024
.L123:
	ldr	r2, [sp, #60]
	ldr	r1, [sp, #36]
	mla	r9, r2, r3, r1
	ldr	r2, [sp, #56]
	ldr	r1, [sp, #32]
	mla	r3, r2, r3, r1
	asr	r9, r9, #10
	movs	r2, #100
	eor	r8, r9, r9, asr #31
	sub	r8, r8, r9, asr #31
	asrs	r3, r3, #10
	cmp	r3, #100
	mul	r5, r2, r8
	it	lt
	movlt	r3, #100
	lsls	r1, r3, #1
	adds	r0, r1, r3
	cmp	r5, r0
	ble	.L157
	mul	r2, r2, r9
	cmp	r0, r2
	bge	.L124
	cmp	r9, r3
	blt	.L125
	sub	r8, r6, #3
.L121:
	ldr	r2, [sp, #40]
	mov	r1, r10
	mov	r0, r8
	bl	mpp_clip
	cmp	r0, #36
	bgt	.L127
	cmp	r0, #33
	bgt	.L158
	cmp	r0, #30
	mov	r1, #1
	ite	gt
	movgt	r2, #3
	movle	r2, #4
.L128:
	add	r2, r2, r6
	subs	r1, r6, r1
	bl	mpp_clip
	ldr	r3, [sp, #68]
	str	r0, [r4, #576]
	cmp	r3, #0
	ble	.L69
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #24]
	add	r3, r3, r3, lsl #2
	cmp	r3, r2
	ble	.L130
	cmp	r0, #51
	it	ge
	movge	r0, #51
	cmp	r0, #25
	it	lt
	movlt	r0, #25
.L216:
	str	r0, [r4, #576]
	b	.L69
.L114:
	cmp	r2, r5, lsl #2
	bgt	.L116
	cmp	r3, #29
	ble	.L214
.L213:
	subs	r0, r6, #3
	b	.L112
.L116:
	movs	r1, #10
	muls	r1, r5, r1
	cmp	r1, r2
	ble	.L118
.L214:
	subs	r0, r6, #2
	b	.L112
.L113:
	cmp	r6, #35
	ite	gt
	movgt	r0, #0
	movle	r0, #1
	cmp	ip, r1
	bgt	.L119
	adds	r2, r6, #2
.L212:
	add	r0, r0, r2
	b	.L112
.L119:
	add	lr, r1, r1, lsl #1
	cmp	ip, lr
	bgt	.L120
	adds	r2, r6, #1
	b	.L212
.L120:
	add	r1, r1, r1, lsl #2
	cmp	r1, r2
	ite	gt
	addgt	r0, r6, #1
	movle	r0, r6
	b	.L112
.L153:
	movs	r5, #205
	b	.L122
.L155:
	mov	r3, #1024
	b	.L123
.L125:
	cmp	r3, r9, lsl #2
	bgt	.L126
	sub	r8, r6, #2
	b	.L121
.L126:
	movs	r2, #10
	mul	r9, r2, r9
	cmp	r9, r3
	ble	.L157
	add	r8, r6, #-1
	b	.L121
.L124:
	add	r8, r8, r8, lsl #1
	cmp	r8, r1
	ite	lt
	movlt	r8, r6
	addge	r8, r6, #1
	b	.L121
.L157:
	mov	r8, r6
	b	.L121
.L158:
	movs	r2, #2
	mov	r1, r2
	b	.L128
.L130:
	ldr	r2, [sp, #24]
	movs	r3, #20
	muls	r3, r2, r3
	cmp	r3, r2
	ble	.L69
	cmp	r0, #51
	it	ge
	movge	r0, #51
	cmp	r0, #21
	it	lt
	movlt	r0, #21
	b	.L216
.L224:
	.align	2
.L223:
	.word	1340867839
	.word	1717986919
.L161:
	movs	r1, #5
	movs	r3, #6
.L131:
	ldr	r2, [sp, #64]
	ldr	r5, [r4, #276]
	cmp	r2, #11
	ldr	r2, [r4, #620]
	itt	gt
	addgt	r3, r3, #1
	addgt	r1, r1, #1
	cmp	r2, #0
	ldr	r0, [r4, #576]
	it	ne
	movne	r3, #0
	cmp	r5, #2
	bne	.L134
	ldrd	r2, r1, [sp, #16]
	cmp	r2, r1
	beq	.L164
	mov	r2, r1
	ldr	r1, [sp, #16]
	adds	r1, r3, r1
.L220:
	bl	mpp_clip
	b	.L142
.L162:
	movs	r1, #4
	b	.L217
.L163:
	mov	r3, r1
	b	.L131
.L164:
	ldr	r0, [sp, #16]
.L142:
	ldrd	r1, r2, [r4, #296]
	str	r0, [r4, #576]
	ldr	r0, [r4, #576]
	bl	mpp_clip
	movs	r3, #0
	str	r0, [r4, #576]
	str	r0, [r7, #172]
	str	r3, [r7, #212]
	ldr	r3, [r4, #276]
	cbnz	r3, .L139
	ldr	r2, [sp, #52]
	add	r3, fp, #-1
	cmp	r3, r2
	bgt	.L139
	ldr	r3, [r4, #576]
	cmp	r3, fp
	bne	.L139
	ldr	r3, [r4, #580]
	cmp	r3, fp
	bne	.L139
	movs	r3, #1
	str	r3, [r7, #212]
	ldr	r3, [r4, #276]
	cmp	r3, #2
	bne	.L140
	ldrd	r3, r2, [sp, #16]
	subs	r3, r3, r2
.L218:
	it	ne
	movne	r3, #1
	str	r3, [r7, #212]
.L139:
	ldr	r3, [r4, #96]
	str	r3, [r7, #176]
	ldr	r3, [r4, #100]
	str	r3, [r7, #180]
	ldr	r3, [r4, #392]
	str	r3, [r7, #160]
	ldrd	r3, r2, [r4, #288]
	adds	r3, r3, #1
	adc	r2, r2, #0
	strd	r3, r2, [r4, #288]
	movs	r3, #0
	str	r3, [r4, #604]
	ldr	r3, .L225
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L192
	mov	r3, r4
	movw	r2, #930
	ldr	r1, .L225+4
	ldr	r0, .L225+8
	bl	printk
.L192:
	movs	r0, #0
	add	sp, sp, #84
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L134:
	ldr	r2, [sp, #12]
	cmp	r2, fp
	beq	.L137
	ldr	r6, [sp, #12]
	mov	r2, fp
	add	r1, r1, r6
	bl	mpp_clip
	cmp	r5, #3
	bne	.L142
	ldr	r2, [sp, #12]
	subs	r0, r0, #1
	subs	r1, r2, #1
	mov	r2, fp
	add	r1, r1, r3
	b	.L220
.L140:
	ldr	r3, [sp, #12]
	subs	r3, r3, fp
	b	.L218
.L127:
	cmp	r0, #41
	mov	r2, #1
	ite	lt
	movlt	r1, #3
	movge	r1, #4
	b	.L128
.L137:
	ldr	r0, [sp, #12]
	b	.L142
.L226:
	.align	2
.L225:
	.word	rc_debug
	.word	.LANCHOR0+545
	.word	.LC3
	.fnend
	.size	rc_model_v2_smt_start, .-rc_model_v2_smt_start
	.align	1
	.global	rc_model_v2_smt_proc_show
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_smt_proc_show, %function
rc_model_v2_smt_proc_show:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	.pad #44
	sub	sp, sp, #44
	ldr	r1, .L242
	mov	r5, r0
	mov	r6, r2
	bl	seq_puts
	ldr	r3, .L242+4
	ldr	r2, .L242+8
	mov	r0, r5
	ldr	r1, .L242+12
	ldr	r7, .L242+16
	ldr	r8, .L242+288
	str	r3, [sp, #28]
	ldr	r3, .L242+20
	str	r3, [sp, #24]
	ldr	r3, .L242+24
	str	r3, [sp, #20]
	ldr	r3, .L242+28
	str	r3, [sp, #16]
	ldr	r3, .L242+32
	str	r3, [sp, #12]
	ldr	r3, .L242+36
	str	r3, [sp, #8]
	ldr	r3, .L242+40
	str	r3, [sp, #4]
	ldr	r3, .L242+44
	str	r3, [sp]
	ldr	r3, .L242+48
	bl	seq_printf
	ldr	r10, [r4, #8]
	ldr	r9, [r4, #60]
	cmp	r10, #2
	bne	.L228
	ldrd	r0, r1, [r4, #16]
	bl	__aeabi_idiv
	ldr	fp, [r4, #40]
	str	r0, [sp, #36]
	ldrd	r0, r1, [r4, #28]
	bl	__aeabi_idiv
	str	r0, [sp, #32]
	mov	r0, r10
	bl	strof_rc_mode
	ldr	r2, [r4, #92]
	ldr	r1, [sp, #32]
	ldr	r3, [sp, #36]
	strd	r2, r2, [sp, #24]
	mov	r2, r6
	str	r1, [sp, #8]
	ldr	r1, .L242+52
	strd	r9, r3, [sp]
	mov	r3, fp
	strd	r7, r7, [sp, #16]
	str	r0, [sp, #12]
.L239:
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L242+56
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L242+60
	ldr	r2, .L242+8
	mov	r0, r5
	ldr	r1, .L242+64
	str	r3, [sp, #8]
	ldr	r3, .L242+68
	str	r3, [sp, #4]
	ldr	r3, .L242+72
	str	r3, [sp]
	ldr	r3, .L242+76
	bl	seq_printf
	ldr	r0, [r4, #176]
	bl	strof_drop
	ldr	r3, [r4, #184]
	mov	r2, r6
	ldr	r1, .L242+80
	str	r3, [sp, #8]
	ldr	r3, [r4, #608]
	str	r3, [sp, #4]
	ldr	r3, [r4, #180]
	str	r3, [sp]
	mov	r3, r0
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L242+84
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L242+88
	ldr	r2, .L242+8
	mov	r0, r5
	ldr	r1, .L242+92
	str	r3, [sp, #8]
	ldr	r3, .L242+96
	str	r3, [sp, #4]
	ldr	r3, .L242+100
	str	r3, [sp]
	ldr	r3, .L242+104
	bl	seq_printf
	ldr	r0, [r4, #188]
	bl	strof_suprmode
	ldr	r3, [r4, #200]
	mov	r2, r6
	ldr	r1, .L242+108
	str	r3, [sp, #8]
	ldr	r3, [r4, #196]
	str	r3, [sp, #4]
	ldr	r3, [r4, #192]
	str	r3, [sp]
	mov	r3, r0
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L242+112
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L242+116
	mov	r0, r5
	ldr	r2, .L242+8
	ldr	r1, .L242+120
	str	r3, [sp, #8]
	ldr	r3, .L242+124
	str	r3, [sp, #4]
	ldr	r3, .L242+128
	str	r3, [sp]
	ldr	r3, .L242+132
	bl	seq_printf
	ldr	r0, [r4, #36]
	cmp	r0, #1
	bne	.L230
	bl	strof_gop_mode
	ldr	r3, [r4, #112]
	ldr	r2, [r4, #44]
	ldr	r1, .L242+136
	strd	r3, r2, [sp]
	mov	r2, r6
	str	r3, [sp, #8]
	mov	r3, r0
.L240:
	mov	r0, r5
	bl	seq_printf
	ldr	r3, [r4, #8]
	cbz	r3, .L232
	cmp	r3, #4
	bne	.L233
.L232:
	ldr	r1, .L242+140
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L242+144
	ldr	r2, .L242+8
	mov	r0, r5
	ldr	r1, .L242+148
	str	r3, [sp, #28]
	ldr	r3, .L242+152
	str	r3, [sp, #24]
	ldr	r3, .L242+156
	str	r3, [sp, #20]
	ldr	r3, .L242+160
	str	r3, [sp, #16]
	ldr	r3, .L242+164
	str	r3, [sp, #12]
	ldr	r3, .L242+168
	str	r3, [sp, #8]
	ldr	r3, .L242+172
	str	r3, [sp, #4]
	ldr	r3, .L242+176
	str	r3, [sp]
	ldr	r3, .L242+180
	bl	seq_printf
	ldr	r3, [r4, #156]
	mov	r2, r6
	ldr	r1, .L242+184
	mov	r0, r5
	str	r3, [sp, #28]
	ldr	r3, [r4, #124]
	str	r3, [sp, #24]
	ldr	r3, [r4, #132]
	str	r3, [sp, #20]
	ldr	r3, [r4, #120]
	str	r3, [sp, #16]
	ldr	r3, [r4, #128]
	str	r3, [sp, #12]
	ldr	r3, [r4, #108]
	str	r3, [sp, #8]
	ldr	r3, [r4, #104]
	str	r3, [sp, #4]
	ldr	r3, [r4, #100]
	str	r3, [sp]
	ldr	r3, [r4, #96]
	bl	seq_printf
.L233:
	ldr	r1, .L242+188
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L242+192
	ldr	r2, .L242+8
	mov	r0, r5
	ldr	r1, .L242+196
	str	r3, [sp, #28]
	ldr	r3, .L242+200
	str	r3, [sp, #24]
	ldr	r3, .L242+204
	str	r3, [sp, #20]
	ldr	r3, .L242+208
	str	r3, [sp, #16]
	ldr	r3, .L242+212
	str	r3, [sp, #12]
	ldr	r3, .L242+216
	str	r3, [sp, #8]
	ldr	r3, .L242+220
	str	r3, [sp, #4]
	ldr	r3, .L242+224
	str	r3, [sp]
	ldr	r3, .L242+228
	bl	seq_printf
	ldr	r0, [r4, #212]
	bl	strof_bool
	ldr	r3, [r4, #228]
	mov	r2, r6
	ldr	r1, .L242+232
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
	ldr	r1, .L242+236
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L242+240
	mov	r0, r5
	ldr	r2, .L242+8
	ldr	r1, .L242+244
	str	r3, [sp, #4]
	ldr	r3, .L242+248
	str	r3, [sp]
	ldr	r3, .L242+228
	bl	seq_printf
	ldr	r9, [r4, #204]
	mov	r0, r9
	cmp	r9, #0
	beq	.L234
	bl	strof_bool
	movs	r3, #0
	ldr	r1, .L242+252
	mov	r2, r6
	str	r3, [sp, #4]
	ldr	r3, [r4, #208]
	str	r3, [sp]
	mov	r3, r0
.L241:
	mov	r0, r5
	bl	seq_printf
	mov	r0, r5
	ldr	r1, .L242+256
	bl	seq_puts
	ldr	r3, .L242+260
	mov	r0, r5
	ldr	r2, .L242+8
	ldr	r1, .L242+264
	str	r3, [sp, #16]
	ldr	r3, .L242+268
	str	r3, [sp, #12]
	ldr	r3, .L242+272
	str	r3, [sp, #8]
	ldr	r3, .L242+276
	str	r3, [sp, #4]
	ldr	r3, .L242+280
	str	r3, [sp]
	ldr	r3, .L242+284
	bl	seq_printf
	b	.L243
.L244:
	.align	2
.L242:
	.word	.LC4
	.word	.LC8
	.word	.LC6
	.word	.LC7
	.word	.LC17
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC5
	.word	.LC16
	.word	.LC20
	.word	.LC23
	.word	.LC22
	.word	.LC24
	.word	.LC25
	.word	.LC21
	.word	.LC26
	.word	.LC27
	.word	.LC30
	.word	.LC29
	.word	.LC31
	.word	.LC32
	.word	.LC28
	.word	.LC33
	.word	.LC34
	.word	.LC37
	.word	.LC36
	.word	.LC38
	.word	.LC39
	.word	.LC35
	.word	.LC40
	.word	.LC42
	.word	.LC45
	.word	.LC44
	.word	.LC46
	.word	.LC47
	.word	.LC48
	.word	.LC49
	.word	.LC50
	.word	.LC51
	.word	.LC52
	.word	.LC43
	.word	.LC53
	.word	.LC54
	.word	.LC57
	.word	.LC56
	.word	.LC58
	.word	.LC59
	.word	.LC60
	.word	.LC61
	.word	.LC62
	.word	.LC63
	.word	.LC64
	.word	.LC55
	.word	.LC65
	.word	.LC66
	.word	.LC68
	.word	.LC67
	.word	.LC69
	.word	.LC70
	.word	.LC72
	.word	.LC75
	.word	.LC74
	.word	.LC76
	.word	.LC77
	.word	.LC78
	.word	.LC79
	.word	.LC73
	.word	274877907
.L243:
	ldr	r2, [r4, #252]
	ldr	r3, [r4, #328]
	mov	r0, r5
	ldr	r1, .L245
	smull	r8, r9, r3, r8
	asrs	r3, r3, #31
	rsb	r3, r3, r9, asr #6
	str	r2, [sp, #16]
	ldr	r2, [r4, #136]
	str	r2, [sp, #12]
	ldr	r2, [r4, #256]
	str	r2, [sp, #8]
	ldr	r2, [r4, #92]
	str	r2, [sp, #4]
	ldr	r2, [r4, #72]
	str	r2, [sp]
	mov	r2, r6
	bl	seq_printf
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L228:
	ldr	r3, [r4, #56]
	strd	r7, r7, [sp, #24]
	smull	r0, r1, r3, r8
	asrs	r3, r3, #31
	rsb	r3, r3, r1, asr #6
	str	r3, [sp, #20]
	ldr	r3, [r4, #48]
	smull	r0, r1, r3, r8
	asrs	r3, r3, #31
	rsb	r3, r3, r1, asr #6
	str	r3, [sp, #16]
	ldr	r3, .L245+4
	ldrd	r0, r1, [r4, #28]
	str	r3, [sp, #12]
	bl	__aeabi_idiv
	str	r0, [sp, #8]
	ldrd	r0, r1, [r4, #16]
	bl	__aeabi_idiv
	ldr	r3, [r4, #40]
	mov	r2, r6
	ldr	r1, .L245+8
	strd	r9, r0, [sp]
	b	.L239
.L230:
	bl	strof_gop_mode
	ldr	r3, [r4, #112]
	mov	r2, r6
	ldr	r1, .L245+12
	str	r3, [sp]
	mov	r3, r0
	strd	r7, r7, [sp, #4]
	b	.L240
.L234:
	bl	strof_bool
	mov	r2, r6
	ldr	r1, .L245+16
	strd	r7, r9, [sp]
	mov	r3, r0
	b	.L241
.L246:
	.align	2
.L245:
	.word	.LC80
	.word	.LC19
	.word	.LC18
	.word	.LC41
	.word	.LC71
	.fnend
	.size	rc_model_v2_smt_proc_show, .-rc_model_v2_smt_proc_show
	.align	1
	.global	rc_model_v2_smt_hal_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_smt_hal_start, %function
rc_model_v2_smt_hal_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	mov	r3, r0
	ldr	r2, .L252
	ldr	r2, [r2]
	lsls	r2, r2, #31
	bpl	.L248
	str	r1, [sp]
	movw	r2, #1126
	ldr	r1, .L252+4
	ldr	r0, .L252+8
	bl	printk
.L248:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L253:
	.align	2
.L252:
	.word	rc_debug
	.word	.LANCHOR0+567
	.word	.LC81
	.fnend
	.size	rc_model_v2_smt_hal_start, .-rc_model_v2_smt_hal_start
	.align	1
	.global	rc_model_v2_smt_hal_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_smt_hal_end, %function
rc_model_v2_smt_hal_end:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	ldr	r5, .L261
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L256
	mov	r3, r0
	str	r1, [sp]
	movw	r2, #1132
	ldr	r1, .L261+4
	ldr	r0, .L261+8
	bl	printk
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L256
	mov	r3, r4
	movw	r2, #1133
	ldr	r1, .L261+4
	ldr	r0, .L261+12
	bl	printk
.L256:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L262:
	.align	2
.L261:
	.word	rc_debug
	.word	.LANCHOR0+593
	.word	.LC82
	.word	.LC3
	.fnend
	.size	rc_model_v2_smt_hal_end, .-rc_model_v2_smt_hal_end
	.align	1
	.global	bits_model_smt_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_model_smt_deinit, %function
bits_model_smt_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r5, .L296
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L264
	mov	r3, r0
	movs	r2, #83
	ldr	r1, .L296+4
	ldr	r0, .L296+8
	bl	printk
.L264:
	ldr	r0, [r4, #336]
	cbz	r0, .L265
	bl	mpp_data_deinit
	movs	r3, #0
	str	r3, [r4, #336]
.L265:
	ldr	r0, [r4, #340]
	cbz	r0, .L266
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #340]
.L266:
	ldr	r0, [r4, #344]
	cbz	r0, .L267
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #344]
.L267:
	ldr	r0, [r4, #348]
	cbz	r0, .L268
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #348]
.L268:
	ldr	r0, [r4, #568]
	cbz	r0, .L269
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #568]
.L269:
	ldr	r0, [r4, #572]
	cbz	r0, .L270
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #572]
.L270:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L271
	mov	r3, r4
	movs	r2, #115
	ldr	r1, .L296+4
	ldr	r0, .L296+12
	bl	printk
.L271:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L297:
	.align	2
.L296:
	.word	rc_debug
	.word	.LANCHOR0+617
	.word	.LC1
	.word	.LC3
	.fnend
	.size	bits_model_smt_deinit, .-bits_model_smt_deinit
	.align	1
	.global	rc_model_v2_smt_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_smt_deinit, %function
rc_model_v2_smt_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	ldr	r4, .L307
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L299
	mov	r3, r0
	mov	r2, #294
	ldr	r1, .L307+4
	ldr	r0, .L307+8
	bl	printk
.L299:
	mov	r0, r5
	bl	bits_model_smt_deinit
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L300
	mov	r3, r5
	mov	r2, #296
	ldr	r1, .L307+4
	ldr	r0, .L307+12
	bl	printk
.L300:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L308:
	.align	2
.L307:
	.word	rc_debug
	.word	.LANCHOR0+639
	.word	.LC1
	.word	.LC3
	.fnend
	.size	rc_model_v2_smt_deinit, .-rc_model_v2_smt_deinit
	.align	1
	.global	bits_model_smt_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_model_smt_init, %function
bits_model_smt_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r7, .L351
	.pad #44
	sub	sp, sp, #44
	ldr	r5, [r0, #40]
	add	r6, sp, #20
	ldrd	r9, r10, [r4, #28]
	ldmia	r7!, {r0, r1, r2, r3}
	stmia	r6!, {r0, r1, r2, r3}
	mov	r1, r10
	ldr	r3, [r7]
	mov	r0, r9
	str	r3, [r6]
	bl	__aeabi_idiv
	cmp	r5, #10
	mov	r1, #1
	mov	r6, r0
	mov	r0, r5
	it	lt
	movlt	r0, #10
	cmp	r0, r6
	mov	r2, r6
	it	lt
	movlt	r0, r6
	bl	mpp_clip
	mov	r1, r10
	mov	r7, r0
	ldr	r0, [r4, #60]
	mul	r0, r0, r9
	bl	__aeabi_idiv
	mov	r8, r0
	cbnz	r0, .L310
	mov	r1, r10
	lsl	r0, r9, #3
	bl	__aeabi_idiv
	mov	r8, r0
.L310:
	ldr	r3, .L351+4
	ldr	r2, [r3]
	str	r3, [sp, #12]
	tst	r2, #1
	beq	.L311
	mov	r3, r4
	movs	r2, #132
	ldr	r1, .L351+8
	ldr	r0, .L351+12
	bl	printk
.L311:
	ldr	r0, [r4, #340]
	movs	r3, #0
	movs	r2, #0
	movs	r1, #10
	strd	r2, [r4, #288]
	movs	r3, #1
	str	r5, [r4, #420]
	str	r3, [r4, #284]
	movs	r3, #0
	strd	r3, r3, [r4, #592]
	movs	r3, #51
	strd	r1, r3, [r4, #296]
	cbz	r0, .L312
	bl	mpp_data_deinit_v2
.L312:
	movs	r2, #0
	movs	r1, #10
	add	r0, r4, #340
	bl	mpp_data_init_v2
	ldr	r0, [r4, #344]
	cbz	r0, .L313
	bl	mpp_data_deinit_v2
.L313:
	movs	r2, #0
	movs	r1, #10
	add	r0, r4, #344
	bl	mpp_data_init_v2
	ldr	r0, [r4, #348]
	cbz	r0, .L314
	bl	mpp_data_deinit_v2
.L314:
	ldr	r2, [r4, #408]
	mov	r1, r8
	add	r0, r4, #348
	bl	mpp_data_init_v2
	ldr	r0, [r4, #568]
	cbz	r0, .L315
	bl	mpp_data_deinit_v2
.L315:
	movs	r2, #0
	mov	r1, r8
	add	r0, r4, #568
	bl	mpp_data_init_v2
	ldr	r0, [r4, #572]
	cbz	r0, .L316
	bl	mpp_data_deinit_v2
.L316:
	mov	r1, r8
	add	r8, r4, #352
	movs	r2, #0
	add	fp, r4, #424
	add	r0, r4, #572
	add	r10, r4, #460
	bl	mpp_data_init_v2
	mov	r0, r8
	bl	mpp_pid_reset
	add	r9, r4, #496
	mov	r0, fp
	bl	mpp_pid_reset
	mov	r0, r10
	bl	mpp_pid_reset
	mov	r0, r9
	bl	mpp_pid_reset
	add	r3, r4, #532
	mov	r0, r3
	str	r3, [sp, #8]
	bl	mpp_pid_reset
	mov	r0, r8
	movs	r3, #90
	movs	r2, #6
	movs	r1, #4
	mov	r8, #100
	str	r3, [sp]
	movs	r3, #0
	str	r7, [sp, #4]
	bl	mpp_pid_set_param
	movs	r3, #0
	movs	r2, #6
	movs	r1, #4
	mov	r0, fp
	str	r8, [sp]
	str	r7, [sp, #4]
	bl	mpp_pid_set_param
	movs	r3, #0
	movs	r2, #6
	movs	r1, #4
	mov	r0, r10
	strd	r8, r7, [sp]
	bl	mpp_pid_set_param
	mov	r0, r9
	movs	r3, #0
	movs	r2, #6
	movs	r1, #4
	strd	r8, r5, [sp]
	bl	mpp_pid_set_param
	movs	r3, #0
	movs	r2, #6
	movs	r1, #4
	strd	r8, r5, [sp]
	ldr	r0, [sp, #8]
	bl	mpp_pid_set_param
	ldrd	r2, r1, [r4, #28]
	ldr	r0, [r4, #48]
	bl	axb_div_c
	ldrd	r2, r1, [r4, #28]
	mov	r8, r0
	ldr	r0, [r4, #56]
	bl	axb_div_c
	movs	r3, #0
	movs	r2, #0
	strd	r2, [r4, #312]
	strd	r2, [r4, #320]
	movs	r3, #0
	str	r3, [r4, #328]
	mov	r9, r0
	cbnz	r5, .L317
	movs	r3, #10
	str	r5, [r4, #304]
	mul	r0, r3, r8
	str	r8, [r4, #400]
	mul	r3, r3, r9
	str	r9, [r4, #408]
	str	r0, [r4, #396]
	str	r3, [r4, #404]
.L318:
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L322
	mov	r3, r4
	movs	r2, #213
	ldr	r1, .L351+8
	ldr	r0, .L351+16
	bl	printk
.L322:
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L317:
	cmp	r5, #1
	bne	.L319
	ldr	r2, [r4, #204]
	str	r5, [r4, #304]
	strd	r8, r3, [r4, #396]
	strd	r0, r3, [r4, #404]
	cmp	r2, #0
	beq	.L318
	str	r3, [r4, #204]
	b	.L318
.L319:
	cmp	r5, r7
	bge	.L320
	movs	r3, #2
	adds	r5, r5, #1
	asrs	r0, r0, #1
	str	r3, [r4, #304]
	asr	r3, r8, #1
	str	r0, [r4, #408]
	str	r3, [r4, #400]
	muls	r0, r5, r0
	muls	r3, r5, r3
	str	r0, [r4, #404]
	str	r3, [r4, #396]
	b	.L318
.L320:
	cmp	r5, #50
	ble	.L321
	cmp	r5, #100
	ble	.L324
	cmp	r5, #200
	ble	.L325
	cmp	r5, #300
	ite	le
	movle	r3, #3
	movgt	r3, #4
.L321:
	movs	r2, #3
	str	r2, [r4, #304]
	add	r2, sp, #40
	rsb	r6, r6, #1
	add	r3, r2, r3, lsl #2
	mov	r1, r6
	ldr	r5, [r3, #-20]
	mul	r0, r8, r5
	mul	r5, r9, r5
	add	r0, r0, r0, lsr #31
	add	r5, r5, r5, lsr #31
	asrs	r0, r0, #1
	str	r0, [r4, #396]
	asrs	r5, r5, #1
	str	r5, [r4, #404]
	bl	__aeabi_idiv
	mov	r1, r6
	add	r0, r0, r8
	str	r0, [r4, #400]
	mov	r0, r5
	bl	__aeabi_idiv
	add	r0, r0, r9
	str	r0, [r4, #408]
	b	.L318
.L324:
	movs	r3, #1
	b	.L321
.L325:
	movs	r3, #2
	b	.L321
.L352:
	.align	2
.L351:
	.word	.LANCHOR0+664
	.word	rc_debug
	.word	.LANCHOR0+684
	.word	.LC1
	.word	.LC3
	.fnend
	.size	bits_model_smt_init, .-bits_model_smt_init
	.align	1
	.global	rc_model_v2_smt_h265_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_smt_h265_init, %function
rc_model_v2_smt_h265_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L362
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L354
	mov	r3, r0
	movw	r2, #269
	ldr	r1, .L362+4
	ldr	r0, .L362+8
	bl	printk
.L354:
	mov	r2, #276
	mov	r1, r6
	mov	r0, r4
	bl	memcpy
	mov	r0, r4
	bl	bits_model_smt_init
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L355
	mov	r3, r4
	mov	r2, #274
	ldr	r1, .L362+4
	ldr	r0, .L362+12
	bl	printk
.L355:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L363:
	.align	2
.L362:
	.word	rc_debug
	.word	.LANCHOR0+704
	.word	.LC1
	.word	.LC3
	.fnend
	.size	rc_model_v2_smt_h265_init, .-rc_model_v2_smt_h265_init
	.align	1
	.global	rc_model_v2_smt_h264_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_smt_h264_init, %function
rc_model_v2_smt_h264_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L373
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L365
	mov	r3, r0
	mov	r2, #282
	ldr	r1, .L373+4
	ldr	r0, .L373+8
	bl	printk
.L365:
	mov	r2, #276
	mov	r1, r6
	mov	r0, r4
	bl	memcpy
	mov	r0, r4
	bl	bits_model_smt_init
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L366
	mov	r3, r4
	movw	r2, #287
	ldr	r1, .L373+4
	ldr	r0, .L373+12
	bl	printk
.L366:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L374:
	.align	2
.L373:
	.word	rc_debug
	.word	.LANCHOR0+730
	.word	.LC1
	.word	.LC3
	.fnend
	.size	rc_model_v2_smt_h264_init, .-rc_model_v2_smt_h264_init
	.global	__aeabi_idivmod
	.align	1
	.global	bits_model_update_smt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	bits_model_update_smt, %function
bits_model_update_smt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r6, .L390
	mov	r5, r1
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L376
	mov	r3, r0
	movs	r2, #224
	ldr	r1, .L390+4
	ldr	r0, .L390+8
	bl	printk
.L376:
	mov	r1, r5
	ldr	r0, [r4, #348]
	bl	mpp_data_update_v2
	ldr	r1, [r4, #388]
	subs	r3, r1, r5
	str	r3, [r4, #412]
	ldr	r3, [r4, #392]
	subs	r1, r5, r1
	subs	r3, r3, r5
	str	r3, [r4, #416]
	ldr	r3, [r4, #276]
	cmp	r3, #2
	bne	.L377
	ldrd	r3, r2, [r4, #312]
	add	r0, r4, #424
	adds	r3, r3, #1
	adc	r2, r2, #0
	strd	r3, r2, [r4, #312]
	movs	r2, #1
	bl	mpp_pid_update
	ldr	r1, [r4, #392]
	movs	r2, #1
	add	r0, r4, #460
	subs	r1, r5, r1
	bl	mpp_pid_update
.L378:
	ldr	r1, [r4, #388]
	movs	r2, #1
	add	r0, r4, #496
	subs	r1, r5, r1
	bl	mpp_pid_update
	ldr	r1, [r4, #392]
	movs	r2, #1
	add	r0, r4, #532
	subs	r1, r5, r1
	bl	mpp_pid_update
	ldr	r3, [r4, #328]
	ldr	r0, [r4, #312]
	add	r5, r5, r3
	ldr	r3, [r4, #320]
	str	r5, [r4, #328]
	ldr	r1, [r4, #28]
	add	r0, r0, r3
	bl	__aeabi_idivmod
	mov	r2, r1
	cbnz	r1, .L379
	ldr	r3, [r4, #48]
	add	r0, r4, #352
	ldr	r7, [r4, #56]
	adds	r1, r3, r7
	asrs	r1, r1, #1
	add	ip, r1, r1, lsl #1
	cmp	ip, r5, lsl #1
	ble	.L380
.L389:
	subs	r1, r1, r5
	bl	mpp_pid_update
	movs	r3, #0
	str	r3, [r4, #328]
.L379:
	ldr	r3, [r4, #576]
	str	r3, [r4, #580]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L382
	mov	r3, r4
	mov	r2, #260
	ldr	r1, .L390+4
	ldr	r0, .L390+12
	bl	printk
.L382:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L377:
	ldrd	r3, r2, [r4, #320]
	ldr	r0, [r4, #568]
	adds	r3, r3, #1
	adc	r2, r2, #0
	strd	r3, r2, [r4, #320]
	bl	mpp_data_update_v2
	ldr	r1, [r4, #392]
	ldr	r0, [r4, #572]
	subs	r1, r5, r1
	bl	mpp_data_update_v2
	b	.L378
.L380:
	movs	r1, #6
	ldr	r8, .L390+16
	muls	r1, r7, r1
	lsls	r3, r3, #2
	smull	r10, fp, r3, r8
	asrs	r3, r3, #31
	smull	r8, r9, r1, r8
	asrs	r1, r1, #31
	rsb	r3, r3, fp, asr #2
	rsb	r1, r1, r9, asr #2
	add	r1, r1, r3
	b	.L389
.L391:
	.align	2
.L390:
	.word	rc_debug
	.word	.LANCHOR0+756
	.word	.LC1
	.word	.LC3
	.word	1717986919
	.fnend
	.size	bits_model_update_smt, .-bits_model_update_smt
	.align	1
	.global	rc_model_v2_smt_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_smt_end, %function
rc_model_v2_smt_end:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r4, r0
	ldr	r3, [r0, #620]
	mov	r5, r1
	ldr	r7, [r1, #184]
	cmp	r3, #0
	beq	.L393
	ldr	r2, .L419
	smull	r2, r3, r7, r2
	sub	r7, r3, r7, asr #31
	itt	gt
	movgt	r3, #0
	strgt	r3, [r0, #620]
.L393:
	ldr	r6, .L419+4
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L394
	add	r3, r5, #160
	movw	r2, #1098
	str	r3, [sp]
	mov	r3, r4
	ldr	r1, .L419+8
	ldr	r0, .L419+12
	bl	printk
.L394:
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L395
	ldr	r3, [r5, #208]
	movw	r2, #1099
	ldr	r1, .L419+8
	ldr	r0, .L419+16
	str	r3, [sp]
	ldr	r3, [r5, #204]
	bl	printk
.L395:
	ldr	r1, [r5, #204]
	ldr	r0, [r4, #340]
	bl	mpp_data_update_v2
	ldr	r1, [r5, #208]
	ldr	r0, [r4, #344]
	bl	mpp_data_update_v2
	movs	r3, #0
	str	r3, [r4, #284]
	ldr	r3, [r4, #276]
	cbz	r3, .L396
	ldr	r3, [r4, #304]
	cmp	r3, #1
	bne	.L397
.L396:
	ldr	r1, [r4, #576]
	ldr	r0, [r4, #336]
	bl	mpp_data_update
.L398:
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L399
	mov	r3, r7
	mov	r2, #1112
	ldr	r1, .L419+8
	ldr	r0, .L419+20
	bl	printk
.L399:
	mov	r1, r7
	mov	r0, r4
	bl	bits_model_update_smt
	ldr	r1, [r4, #576]
	ldr	r3, [r4, #276]
	ldr	r2, [r4, #596]
	str	r1, [r4, #580]
	str	r3, [r4, #280]
	adds	r2, r2, #1
	ldr	r3, [r5, #200]
	str	r2, [r4, #596]
	str	r3, [r4, #600]
	ldr	r3, [r4, #592]
	add	r3, r3, r1
	str	r3, [r4, #592]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L400
	mov	r3, r4
	mov	r2, #1120
	ldr	r1, .L419+8
	ldr	r0, .L419+24
	bl	printk
.L400:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L397:
	ldr	r3, [r4, #576]
	strd	r7, r3, [r4, #584]
	b	.L398
.L420:
	.align	2
.L419:
	.word	1431655766
	.word	rc_debug
	.word	.LANCHOR0+778
	.word	.LC83
	.word	.LC84
	.word	.LC85
	.word	.LC3
	.fnend
	.size	rc_model_v2_smt_end, .-rc_model_v2_smt_end
	.align	1
	.global	check_super_frame_smt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_super_frame_smt, %function
check_super_frame_smt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r5, r0
	ldr	r6, .L439
	mov	r7, r1
	ldr	r8, [r0, #276]
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L422
	mov	r3, r5
	movw	r2, #942
	ldr	r1, .L439+4
	ldr	r0, .L439+8
	bl	printk
.L422:
	ldr	r4, [r5, #188]
	cbz	r4, .L423
	cmp	r8, #2
	ldr	r3, [r5, #196]
	ldr	r2, [r7, #24]
	it	eq
	ldreq	r3, [r5, #192]
	cmp	r2, r3
	bcc	.L428
	cmp	r4, #1
	bne	.L429
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L425
	movw	r2, #950
	ldr	r1, .L439+4
	ldr	r0, .L439+12
	bl	printk
.L425:
	movs	r3, #1
	str	r3, [r5, #176]
	movs	r3, #0
	str	r3, [r5, #184]
.L429:
	mov	r4, #-1
	b	.L423
.L428:
	movs	r4, #0
.L423:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L421
	mov	r3, r5
	movw	r2, #957
	ldr	r1, .L439+4
	ldr	r0, .L439+16
	bl	printk
.L421:
	mov	r0, r4
	pop	{r4, r5, r6, r7, r8, pc}
.L440:
	.align	2
.L439:
	.word	rc_debug
	.word	.LANCHOR0+798
	.word	.LC1
	.word	.LC86
	.word	.LC3
	.fnend
	.size	check_super_frame_smt, .-check_super_frame_smt
	.align	1
	.global	check_re_enc_smt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_re_enc_smt, %function
check_re_enc_smt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r4, r0
	ldr	r5, [r0, #60]
	mov	r7, r1
	ldr	r10, [r0, #276]
	ldr	r0, [r0, #348]
	bl	mpp_data_sum_v2
	mov	r1, r5
	bl	__aeabi_idiv
	mov	r1, #-1
	mul	r6, r0, r5
	mov	r8, r0
	ldr	r0, [r4, #348]
	bl	mpp_data_get_pre_val_v2
	ldr	r3, [r7, #24]
	mov	r1, r5
	subs	r0, r3, r0
	add	r0, r0, r6
	bl	__aeabi_idiv
	ldr	r3, .L480
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #1
	mov	r6, r0
	beq	.L442
	mov	r3, r4
	movw	r2, #973
	ldr	r1, .L480+4
	ldr	r0, .L480+8
	bl	printk
.L442:
	ldr	r3, [r9]
	lsls	r0, r3, #25
	bpl	.L443
	strd	r8, r6, [sp]
	movw	r2, #975
	ldr	r3, [r4, #52]
	ldr	r1, .L480+4
	ldr	r0, .L480+12
	bl	printk
.L443:
	ldr	r2, [r4, #604]
	ldr	r3, [r4, #156]
	cmp	r2, r3
	blt	.L444
.L446:
	movs	r5, #0
.L441:
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L444:
	ldr	r3, [r4, #248]
	cmp	r3, #0
	bne	.L446
	mov	r1, r7
	mov	r0, r4
	bl	check_super_frame_smt
	mov	r5, r0
	cmp	r0, #0
	bne	.L454
	ldr	r3, [r4, #204]
	cbz	r3, .L447
	ldr	r3, [r4, #284]
	cmp	r3, #0
	beq	.L446
.L447:
	ldr	r3, [r9]
	lsls	r1, r3, #22
	bpl	.L448
	str	r10, [sp]
	movw	r2, #989
	ldr	r3, [r4, #176]
	ldr	r1, .L480+4
	ldr	r0, .L480+16
	bl	printk
.L448:
	ldr	r3, [r4, #176]
	ldr	r2, [r9]
	cbz	r3, .L449
	cmp	r10, #0
	bne	.L449
	ldr	r0, [r4, #180]
	lsls	r2, r2, #22
	add	r1, r0, #100
	ldr	r0, [r4, #56]
	mul	r0, r0, r1
	ldr	r1, .L480+20
	umull	r0, r1, r0, r1
	lsr	r5, r1, #5
	bpl	.L450
	ldr	r2, [r4, #52]
	strd	r5, r6, [sp, #4]
	ldr	r1, .L480+4
	str	r2, [sp]
	movw	r2, #993
	ldr	r0, .L480+24
	bl	printk
.L450:
	cmp	r6, r5
	mov	r5, #-1
	it	le
	movle	r5, #0
	b	.L441
.L449:
	cmp	r10, #0
	beq	.L451
	cmp	r10, #2
	bne	.L455
	ldr	r3, [r7, #4]
	add	r3, r3, r3, lsl #1
	add	r3, r3, r3, lsr #31
	asrs	r3, r3, #1
.L452:
	ldr	r1, [r7, #24]
	cmp	r1, r3
	ble	.L453
	ldr	r3, [r4, #56]
	sub	r1, r3, r3, asr #3
	cmp	r1, r6
	bge	.L453
	ldr	r0, .L480+28
	asrs	r5, r3, #31
	sub	r6, r6, r8
	smull	r0, r1, r3, r0
	rsb	r5, r5, r1, asr #3
	cmp	r5, r6
	mov	r5, #-1
	it	ge
	movge	r5, #0
.L453:
	lsls	r3, r2, #31
	bpl	.L441
	str	r5, [sp]
	mov	r3, r4
	movw	r2, #1014
	ldr	r1, .L480+4
	ldr	r0, .L480+32
	bl	printk
	b	.L441
.L451:
	ldr	r3, [r7, #4]
	add	r3, r3, r3, lsl #1
	b	.L452
.L455:
	movs	r3, #0
	b	.L452
.L454:
	mov	r5, #-1
	b	.L441
.L481:
	.align	2
.L480:
	.word	rc_debug
	.word	.LANCHOR0+820
	.word	.LC1
	.word	.LC87
	.word	.LC88
	.word	1374389535
	.word	.LC89
	.word	1717986919
	.word	.LC90
	.fnend
	.size	check_re_enc_smt, .-check_re_enc_smt
	.align	1
	.global	rc_model_v2_smt_check_reenc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_smt_check_reenc, %function
rc_model_v2_smt_check_reenc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r5, r1
	ldr	r7, .L524
	add	r6, r1, #160
	mov	r4, r0
	ldr	r3, [r7]
	lsls	r1, r3, #31
	bpl	.L483
	mov	r3, r0
	str	r6, [sp]
	movw	r2, #1025
	ldr	r1, .L524+4
	ldr	r0, .L524+8
	bl	printk
.L483:
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	bfc	r3, #2, #1
	strb	r3, [r5, #156]
	ldr	r3, [r4, #8]
	cmp	r3, #2
	beq	.L485
	ldr	r3, [r5, #264]
	lsls	r2, r3, #31
	bmi	.L485
	ldr	r3, [r4, #612]
	cmp	r3, #0
	bne	.L485
	ldr	r3, [r4, #616]
	cmp	r3, #0
	bne	.L485
	mov	r1, r6
	mov	r0, r4
	bl	check_re_enc_smt
	cmp	r0, #0
	beq	.L485
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	ldr	r6, [r4, #176]
	tst	r3, #16
	ldr	r3, [r4, #184]
	it	ne
	movne	r6, #0
	cbz	r3, .L488
	ldr	r2, [r4, #608]
	cmp	r3, r2
	it	ls
	movls	r6, #0
.L488:
	ldr	r3, [r7]
	lsls	r3, r3, #22
	bpl	.L489
	ldr	r3, [r4, #608]
	movw	r2, #1043
	ldr	r1, .L524+4
	ldr	r0, .L524+12
	str	r3, [sp]
	mov	r3, r6
	bl	printk
.L489:
	ldr	r1, [r7]
	cmp	r6, #1
	and	r1, r1, #512
	beq	.L490
	cmp	r6, #2
	beq	.L491
	ldr	r3, [r4, #276]
	ldr	r0, [r4, #196]
	cmp	r3, #2
	ldr	r6, [r5, #184]
	ldr	r3, [r5, #172]
	it	eq
	ldreq	r0, [r4, #192]
	lsls	r2, r0, #1
	cmp	r6, r2
	ble	.L494
	adds	r3, r3, #3
.L522:
	str	r3, [r5, #172]
.L495:
	ldrd	r2, r3, [r5, #172]
	cmp	r2, r3
	bge	.L497
	ldr	r3, [r4, #604]
	adds	r3, r3, #1
	str	r3, [r4, #604]
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	orr	r3, r3, #4
	strb	r3, [r5, #156]
.L497:
	movs	r3, #0
	str	r3, [r4, #608]
	cbz	r1, .L485
	movw	r2, #1078
	ldr	r1, .L524+4
	ldr	r0, .L524+16
	b	.L523
.L490:
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	bic	r3, r3, #5
	orr	r3, r3, #5
	strb	r3, [r5, #156]
	ldr	r3, [r4, #608]
	str	r6, [r4, #612]
	adds	r3, r3, #1
	str	r3, [r4, #608]
	cbz	r1, .L485
	ldr	r1, .L524+4
	movw	r2, #1051
	ldr	r0, .L524+20
.L523:
	bl	printk
.L485:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L491:
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	bic	r3, r3, #20
	orr	r3, r3, #20
	strb	r3, [r5, #156]
	movs	r3, #1
	str	r3, [r4, #616]
	ldr	r3, [r4, #608]
	adds	r3, r3, #1
	str	r3, [r4, #608]
	cmp	r1, #0
	beq	.L485
	movw	r2, #1058
	ldr	r1, .L524+4
	ldr	r0, .L524+24
	b	.L523
.L494:
	add	r2, r2, r0
	add	r2, r2, r2, lsr #31
	cmp	r6, r2, asr #1
	ble	.L496
	adds	r3, r3, #2
	b	.L522
.L496:
	cmp	r6, r0
	ble	.L495
	adds	r3, r3, #1
	b	.L522
.L525:
	.align	2
.L524:
	.word	rc_debug
	.word	.LANCHOR0+837
	.word	.LC83
	.word	.LC91
	.word	.LC94
	.word	.LC92
	.word	.LC93
	.fnend
	.size	rc_model_v2_smt_check_reenc, .-rc_model_v2_smt_check_reenc
	.global	smt_h265e
	.global	smt_h264e
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
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
	.type	__func__.25739, %object
	.size	__func__.25739, 21
__func__.25739:
	.ascii	"calc_smt_debreath_qp\000"
	.type	strength_map.25738, %object
	.size	strength_map.25738, 36
strength_map.25738:
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
	.type	intra_qp_map.25736, %object
	.size	intra_qp_map.25736, 8
intra_qp_map.25736:
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.type	inter_pqp1, %object
	.size	inter_pqp1, 52
inter_pqp1:
	.byte	1
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
	.byte	16
	.byte	17
	.byte	18
	.byte	19
	.byte	20
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	24
	.byte	25
	.byte	26
	.byte	26
	.byte	27
	.byte	28
	.byte	29
	.byte	29
	.byte	30
	.byte	31
	.byte	31
	.byte	32
	.byte	33
	.byte	34
	.byte	35
	.byte	36
	.byte	37
	.byte	38
	.byte	39
	.byte	40
	.byte	41
	.byte	42
	.byte	42
	.byte	42
	.byte	43
	.byte	43
	.byte	44
	.type	inter_pqp0, %object
	.size	inter_pqp0, 52
inter_pqp0:
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
	.byte	17
	.byte	18
	.byte	19
	.byte	20
	.byte	21
	.byte	21
	.byte	21
	.byte	22
	.byte	23
	.byte	24
	.byte	25
	.byte	26
	.byte	26
	.byte	27
	.byte	28
	.byte	28
	.byte	29
	.byte	29
	.byte	29
	.byte	30
	.byte	31
	.byte	31
	.byte	32
	.byte	32
	.byte	33
	.byte	33
	.byte	34
	.byte	35
	.byte	35
	.byte	35
	.byte	36
	.byte	36
	.byte	36
	.type	intra_pqp1, %object
	.size	intra_pqp1, 52
intra_pqp1:
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
	.byte	16
	.byte	17
	.byte	18
	.byte	19
	.byte	20
	.byte	22
	.byte	23
	.byte	24
	.byte	25
	.byte	26
	.byte	27
	.byte	28
	.byte	29
	.byte	30
	.byte	31
	.byte	32
	.byte	33
	.byte	34
	.byte	35
	.byte	36
	.byte	37
	.byte	38
	.byte	39
	.byte	40
	.byte	41
	.byte	42
	.byte	43
	.byte	44
	.byte	45
	.byte	46
	.byte	47
	.byte	48
	.byte	49
	.byte	50
	.byte	51
	.byte	51
	.byte	51
	.byte	51
	.type	intra_pqp0, %object
	.size	intra_pqp0, 156
intra_pqp0:
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
	.byte	16
	.byte	17
	.byte	18
	.byte	19
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	23
	.byte	24
	.byte	25
	.byte	26
	.byte	27
	.byte	27
	.byte	28
	.byte	28
	.byte	29
	.byte	30
	.byte	31
	.byte	32
	.byte	32
	.byte	33
	.byte	34
	.byte	34
	.byte	34
	.byte	35
	.byte	35
	.byte	36
	.byte	36
	.byte	36
	.byte	36
	.byte	37
	.byte	37
	.byte	37
	.byte	38
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
	.byte	16
	.byte	17
	.byte	17
	.byte	18
	.byte	18
	.byte	19
	.byte	19
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	24
	.byte	25
	.byte	26
	.byte	27
	.byte	28
	.byte	29
	.byte	30
	.byte	31
	.byte	32
	.byte	32
	.byte	33
	.byte	34
	.byte	34
	.byte	34
	.byte	35
	.byte	35
	.byte	36
	.byte	36
	.byte	36
	.byte	36
	.byte	37
	.byte	37
	.byte	37
	.byte	38
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	14
	.byte	15
	.byte	15
	.byte	16
	.byte	16
	.byte	17
	.byte	17
	.byte	18
	.byte	16
	.byte	16
	.byte	16
	.byte	17
	.byte	18
	.byte	19
	.byte	20
	.byte	21
	.byte	23
	.byte	24
	.byte	26
	.byte	28
	.byte	30
	.byte	31
	.byte	32
	.byte	33
	.byte	34
	.byte	34
	.byte	35
	.byte	35
	.byte	36
	.byte	36
	.byte	36
	.byte	36
	.byte	37
	.byte	37
	.byte	37
	.byte	38
	.type	__func__.25960, %object
	.size	__func__.25960, 22
__func__.25960:
	.ascii	"rc_model_v2_smt_start\000"
	.type	__func__.26015, %object
	.size	__func__.26015, 26
__func__.26015:
	.ascii	"rc_model_v2_smt_hal_start\000"
	.type	__func__.26020, %object
	.size	__func__.26020, 24
__func__.26020:
	.ascii	"rc_model_v2_smt_hal_end\000"
	.type	__func__.25660, %object
	.size	__func__.25660, 22
__func__.25660:
	.ascii	"bits_model_smt_deinit\000"
	.type	__func__.25700, %object
	.size	__func__.25700, 23
__func__.25700:
	.ascii	"rc_model_v2_smt_deinit\000"
	.space	2
.LC0:
	.word	7
	.word	8
	.word	9
	.word	10
	.word	11
	.type	__func__.25673, %object
	.size	__func__.25673, 20
__func__.25673:
	.ascii	"bits_model_smt_init\000"
	.type	__func__.25689, %object
	.size	__func__.25689, 26
__func__.25689:
	.ascii	"rc_model_v2_smt_h265_init\000"
	.type	__func__.25695, %object
	.size	__func__.25695, 26
__func__.25695:
	.ascii	"rc_model_v2_smt_h264_init\000"
	.type	__func__.25683, %object
	.size	__func__.25683, 22
__func__.25683:
	.ascii	"bits_model_update_smt\000"
	.type	__func__.26010, %object
	.size	__func__.26010, 20
__func__.26010:
	.ascii	"rc_model_v2_smt_end\000"
	.type	__func__.25969, %object
	.size	__func__.25969, 22
__func__.25969:
	.ascii	"check_super_frame_smt\000"
	.type	__func__.25982, %object
	.size	__func__.25982, 17
__func__.25982:
	.ascii	"check_re_enc_smt\000"
	.type	__func__.25995, %object
	.size	__func__.25995, 28
__func__.25995:
	.ascii	"rc_model_v2_smt_check_reenc\000"
	.space	3
	.type	smt_h265e, %object
	.size	smt_h265e, 48
smt_h265e:
	.word	.LC19
	.word	16777220
	.word	624
	.word	rc_model_v2_smt_h265_init
	.word	rc_model_v2_smt_deinit
	.word	0
	.word	rc_model_v2_smt_check_reenc
	.word	rc_model_v2_smt_start
	.word	rc_model_v2_smt_end
	.word	rc_model_v2_smt_hal_start
	.word	rc_model_v2_smt_hal_end
	.word	rc_model_v2_smt_proc_show
	.type	smt_h264e, %object
	.size	smt_h264e, 48
smt_h264e:
	.word	.LC19
	.word	7
	.word	624
	.word	rc_model_v2_smt_h264_init
	.word	rc_model_v2_smt_deinit
	.word	0
	.word	rc_model_v2_smt_check_reenc
	.word	rc_model_v2_smt_start
	.word	rc_model_v2_smt_end
	.word	rc_model_v2_smt_hal_start
	.word	rc_model_v2_smt_hal_end
	.word	rc_model_v2_smt_proc_show
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC1:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC2:
	.ascii	"\0016%s:%d: i qp_out %d, qp_start_sum = %d, intra_l"
	.ascii	"v4_prop %d\000"
.LC3:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC4:
	.ascii	"\012--------RC base param--------------------------"
	.ascii	"--------------------------------------------------\012"
	.ascii	"\000"
.LC5:
	.ascii	"Gop\000"
.LC6:
	.ascii	"ChnId\000"
.LC7:
	.ascii	"%7s|%7s|%8s|%6s|%6s|%8s|%13s|%13s|%5s|%5s \012\000"
.LC8:
	.ascii	"PQp\000"
.LC9:
	.ascii	"IQp\000"
.LC10:
	.ascii	"MaxBr(kbps)\000"
.LC11:
	.ascii	"MinBr(kbps)\000"
.LC12:
	.ascii	"RcMode\000"
.LC13:
	.ascii	"TrgFr\000"
.LC14:
	.ascii	"ViFr\000"
.LC15:
	.ascii	"StatTm\000"
.LC16:
	.ascii	"%7d|%7u|%8u|%6u|%6u|%8s|%13s|%13s|%5u|%5u \012\000"
.LC17:
	.ascii	"N/A\000"
.LC18:
	.ascii	"%7d|%7u|%8u|%6u|%6u|%8s|%13u|%13u|%5s|%5s \012\000"
.LC19:
	.ascii	"smart\000"
.LC20:
	.ascii	"\012--------RC run comm param 1--------------------"
	.ascii	"--------------------------------------------------\012"
	.ascii	"\000"
.LC21:
	.ascii	"bLost\000"
.LC22:
	.ascii	"%7s|%8s|%12s|%12s|%10s \012\000"
.LC23:
	.ascii	"EncGap\000"
.LC24:
	.ascii	"LostFrmStr\000"
.LC25:
	.ascii	"LostThr\000"
.LC26:
	.ascii	"%7d|%8s|%12u|%12d|%10u \012\000"
.LC27:
	.ascii	"\012--------RC run comm param 2--------------------"
	.ascii	"--------------------------------------------------\012"
	.ascii	"\000"
.LC28:
	.ascii	"SprFrmMod\000"
.LC29:
	.ascii	"%7s|%12s|%12s|%12s|%12s \012\000"
.LC30:
	.ascii	"RCPriority\000"
.LC31:
	.ascii	"SprPFrm\000"
.LC32:
	.ascii	"SprIFrm\000"
.LC33:
	.ascii	"%7d|%12s|%12u|%12u|%12u \012\000"
.LC34:
	.ascii	"\012--------RC gop mode attr-----------------------"
	.ascii	"--------------------------------------------------\012"
	.ascii	"\000"
.LC35:
	.ascii	"GopMode\000"
.LC36:
	.ascii	"%7s|%10s|%10s|%12s|%10s \012\000"
.LC37:
	.ascii	"ViQpDelta\000"
.LC38:
	.ascii	"BgInterval\000"
.LC39:
	.ascii	"IpQpDelta\000"
.LC40:
	.ascii	"%7d|%10s|%10d|%12u|%10d\012\000"
.LC41:
	.ascii	"%7d|%10s|%10d|%12s|%10s\012\000"
.LC42:
	.ascii	"\012--------RC run smart common param--------------"
	.ascii	"--------------------------------------------------\012"
	.ascii	"\000"
.LC43:
	.ascii	"MaxQp\000"
.LC44:
	.ascii	"%7s|%8s|%8s|%8s|%8s|%10s|%10s|%10s|%10s|%15s\012\000"
.LC45:
	.ascii	"MaxReEncTimes\000"
.LC46:
	.ascii	"FrmMinIQp\000"
.LC47:
	.ascii	"FrmMaxIQp\000"
.LC48:
	.ascii	"FrmMinQp\000"
.LC49:
	.ascii	"FrmMaxQp\000"
.LC50:
	.ascii	"MinIQp\000"
.LC51:
	.ascii	"MaxIQp\000"
.LC52:
	.ascii	"MinQp\000"
.LC53:
	.ascii	"%7d|%8u|%8u|%8u|%8u|%10d|%10d|%10d|%10d|%15d\012\000"
.LC54:
	.ascii	"\012--------RC HierarchicalQp INFO-----------------"
	.ascii	"--------------------------------------------------\012"
	.ascii	"\000"
.LC55:
	.ascii	"bEnable\000"
.LC56:
	.ascii	"%7s|%10s|%12s|%12s|%12s|%12s|%12s|%12s|%12s|%12s\012"
	.ascii	"\000"
.LC57:
	.ascii	"QpDelta[3]\000"
.LC58:
	.ascii	"QpDelta[2]\000"
.LC59:
	.ascii	"QpDelta[1]\000"
.LC60:
	.ascii	"QpDelta[0]\000"
.LC61:
	.ascii	"FrameNum[3]\000"
.LC62:
	.ascii	"FrameNum[2]\000"
.LC63:
	.ascii	"FrameNum[1]\000"
.LC64:
	.ascii	"FrameNum[0]\000"
.LC65:
	.ascii	"%7d|%10s|%12d|%12d|%12d|%12d|%12d|%12d|%12d|%12d\012"
	.ascii	"\000"
.LC66:
	.ascii	"\012--------RC debreath_effect info----------------"
	.ascii	"--------------------------------------------------\012"
	.ascii	"\000"
.LC67:
	.ascii	"%7s|%10s|%10s|%18s\012\000"
.LC68:
	.ascii	"DeBrthEfctCnt\000"
.LC69:
	.ascii	"Strength0\000"
.LC70:
	.ascii	"%7d|%10s|%10d|%18u\012\000"
.LC71:
	.ascii	"%7d|%10s|%10s|%18u\012\000"
.LC72:
	.ascii	"\012--------RC run smart info1---------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"-----\012\000"
.LC73:
	.ascii	"RealBt(kb)\000"
.LC74:
	.ascii	"%7s|%12s|%10s|%10s|%15s|%15s|%12s\012\000"
.LC75:
	.ascii	"scene_mode\000"
.LC76:
	.ascii	"md_switch_qp\000"
.LC77:
	.ascii	"md_switch_en\000"
.LC78:
	.ascii	"StartQp\000"
.LC79:
	.ascii	"IPRatio\000"
.LC80:
	.ascii	"%7d|%12u|%10d|%10u|%15d|%15d|%12d\012\000"
.LC81:
	.ascii	"\0016%s:%d: smt_hal_start enter ctx %p task %p\012\000"
.LC82:
	.ascii	"\0016%s:%d: smt_hal_end enter ctx %p task %p\012\000"
.LC83:
	.ascii	"\0016%s:%d: enter ctx %p cfg %p\012\000"
.LC84:
	.ascii	"\0016%s:%d: motion_level %u, complex_level %u\012\000"
.LC85:
	.ascii	"\0016%s:%d: bits_mode_update real_bit %d\000"
.LC86:
	.ascii	"\0016%s:%d: super frame drop current frame\000"
.LC87:
	.ascii	"\0016%s:%d: reenc check target_bps %d last_ins_bps "
	.ascii	"%d ins_bps %d\000"
.LC88:
	.ascii	"\0016%s:%d: drop mode %d frame_type %d\012\000"
.LC89:
	.ascii	"\0016%s:%d: drop mode %d check max_bps %d bit_thr %"
	.ascii	"d ins_bps %d\000"
.LC90:
	.ascii	"\0016%s:%d: leave %p ret %d\012\000"
.LC91:
	.ascii	"\0016%s:%d: reenc drop_mode %d drop_cnt %d\012\000"
.LC92:
	.ascii	"\0016%s:%d: drop\012\000"
.LC93:
	.ascii	"\0016%s:%d: force_pskip\012\000"
.LC94:
	.ascii	"\0016%s:%d: drop disable\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
