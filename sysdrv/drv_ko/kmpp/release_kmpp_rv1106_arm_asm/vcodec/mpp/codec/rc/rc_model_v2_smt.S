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
	.global	__aeabi_idiv
	.thumb
	.syntax unified
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
	ldr	r1, .L12
	mov	r5, r0
	mov	r6, r2
	bl	seq_puts
	ldr	r3, .L12+4
	ldr	r2, .L12+8
	mov	r0, r5
	ldr	r1, .L12+12
	ldr	r7, .L12+16
	str	r3, [sp, #28]
	ldr	r3, .L12+20
	str	r3, [sp, #24]
	ldr	r3, .L12+24
	str	r3, [sp, #20]
	ldr	r3, .L12+28
	str	r3, [sp, #16]
	ldr	r3, .L12+32
	str	r3, [sp, #12]
	ldr	r3, .L12+36
	str	r3, [sp, #8]
	ldr	r3, .L12+40
	str	r3, [sp, #4]
	ldr	r3, .L12+44
	str	r3, [sp]
	ldr	r3, .L12+48
	bl	seq_printf
	ldr	r9, [r4, #8]
	ldr	r8, [r4, #60]
	cmp	r9, #2
	bne	.L2
	ldrd	r0, r1, [r4, #16]
	bl	__aeabi_idiv
	ldr	r10, [r4, #40]
	mov	fp, r0
	ldrd	r0, r1, [r4, #28]
	bl	__aeabi_idiv
	str	r0, [sp, #36]
	mov	r0, r9
	bl	strof_rc_mode
	ldr	r2, [r4, #92]
	ldr	r1, .L12+52
	ldr	r3, [sp, #36]
	strd	r2, r2, [sp, #24]
	mov	r2, r6
	strd	r7, r7, [sp, #16]
	strd	fp, r3, [sp, #4]
	mov	r3, r10
	str	r0, [sp, #12]
	str	r8, [sp]
.L9:
	mov	r0, r5
	bl	seq_printf
	ldr	r1, .L12+56
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L12+60
	mov	r0, r5
	ldr	r2, .L12+8
	ldr	r1, .L12+64
	str	r3, [sp, #8]
	ldr	r3, .L12+68
	str	r3, [sp, #4]
	ldr	r3, .L12+72
	str	r3, [sp]
	ldr	r3, .L12+76
	bl	seq_printf
	ldr	r0, [r4, #36]
	cmp	r0, #1
	bne	.L4
	bl	strof_gop_mode
	ldr	r3, [r4, #112]
	ldr	r2, [r4, #44]
	ldr	r1, .L12+80
	strd	r3, r2, [sp]
	mov	r2, r6
	str	r3, [sp, #8]
	mov	r3, r0
.L10:
	mov	r0, r5
	bl	seq_printf
	ldr	r3, [r4, #8]
	cmp	r3, #0
	bne	.L6
	ldr	r1, .L12+84
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L12+88
	ldr	r2, .L12+8
	mov	r0, r5
	ldr	r1, .L12+92
	str	r3, [sp, #28]
	ldr	r3, .L12+96
	str	r3, [sp, #24]
	ldr	r3, .L12+100
	str	r3, [sp, #20]
	ldr	r3, .L12+104
	str	r3, [sp, #16]
	ldr	r3, .L12+108
	str	r3, [sp, #12]
	ldr	r3, .L12+112
	str	r3, [sp, #8]
	ldr	r3, .L12+116
	str	r3, [sp, #4]
	ldr	r3, .L12+120
	str	r3, [sp]
	ldr	r3, .L12+124
	bl	seq_printf
	ldr	r3, [r4, #152]
	mov	r2, r6
	ldr	r1, .L12+128
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
.L6:
	ldr	r1, .L12+132
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L12+136
	ldr	r2, .L12+8
	mov	r0, r5
	ldr	r1, .L12+140
	str	r3, [sp, #28]
	ldr	r3, .L12+144
	str	r3, [sp, #24]
	ldr	r3, .L12+148
	str	r3, [sp, #20]
	ldr	r3, .L12+152
	str	r3, [sp, #16]
	ldr	r3, .L12+156
	str	r3, [sp, #12]
	ldr	r3, .L12+160
	str	r3, [sp, #8]
	ldr	r3, .L12+164
	str	r3, [sp, #4]
	ldr	r3, .L12+168
	str	r3, [sp]
	ldr	r3, .L12+172
	bl	seq_printf
	ldr	r0, [r4, #208]
	bl	strof_bool
	ldr	r3, [r4, #224]
	mov	r2, r6
	ldr	r1, .L12+176
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
	ldr	r1, .L12+180
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L12+184
	mov	r0, r5
	ldr	r2, .L12+8
	ldr	r1, .L12+188
	str	r3, [sp, #4]
	ldr	r3, .L12+192
	str	r3, [sp]
	ldr	r3, .L12+172
	bl	seq_printf
	ldr	r8, [r4, #200]
	mov	r0, r8
	cmp	r8, #0
	beq	.L7
	bl	strof_bool
	movs	r3, #0
	ldr	r1, .L12+196
	mov	r2, r6
	str	r3, [sp, #4]
	ldr	r3, [r4, #204]
	str	r3, [sp]
	mov	r3, r0
.L11:
	mov	r0, r5
	bl	seq_printf
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L2:
	ldr	r3, [r4, #56]
	ldr	r0, .L12+200
	strd	r7, r7, [sp, #24]
	smull	r10, fp, r3, r0
	asrs	r3, r3, #31
	rsb	r3, r3, fp, asr #6
	str	r3, [sp, #20]
	ldr	r3, [r4, #48]
	smull	r0, r1, r3, r0
	asrs	r3, r3, #31
	rsb	r3, r3, r1, asr #6
	str	r3, [sp, #16]
	ldr	r3, .L12+204
	ldrd	r0, r1, [r4, #28]
	str	r3, [sp, #12]
	bl	__aeabi_idiv
	str	r0, [sp, #8]
	ldrd	r0, r1, [r4, #16]
	bl	__aeabi_idiv
	ldr	r3, [r4, #40]
	mov	r2, r6
	ldr	r1, .L12+208
	strd	r8, r0, [sp]
	b	.L9
.L4:
	bl	strof_gop_mode
	ldr	r3, [r4, #112]
	mov	r2, r6
	ldr	r1, .L12+212
	str	r3, [sp]
	mov	r3, r0
	strd	r7, r7, [sp, #4]
	b	.L10
.L7:
	bl	strof_bool
	mov	r2, r6
	ldr	r1, .L12+216
	strd	r7, r8, [sp]
	mov	r3, r0
	b	.L11
.L13:
	.align	2
.L12:
	.word	.LC1
	.word	.LC5
	.word	.LC3
	.word	.LC4
	.word	.LC14
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC2
	.word	.LC13
	.word	.LC17
	.word	.LC20
	.word	.LC19
	.word	.LC21
	.word	.LC22
	.word	.LC18
	.word	.LC23
	.word	.LC25
	.word	.LC28
	.word	.LC27
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LC26
	.word	.LC36
	.word	.LC37
	.word	.LC40
	.word	.LC39
	.word	.LC41
	.word	.LC42
	.word	.LC43
	.word	.LC44
	.word	.LC45
	.word	.LC46
	.word	.LC47
	.word	.LC38
	.word	.LC48
	.word	.LC49
	.word	.LC51
	.word	.LC50
	.word	.LC52
	.word	.LC53
	.word	274877907
	.word	.LC16
	.word	.LC15
	.word	.LC24
	.word	.LC54
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
	ldr	r2, .L19
	ldr	r2, [r2]
	lsls	r2, r2, #31
	bpl	.L15
	str	r1, [sp]
	movw	r2, #1319
	ldr	r1, .L19+4
	ldr	r0, .L19+8
	bl	printk
.L15:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L20:
	.align	2
.L19:
	.word	rc_debug
	.word	.LANCHOR0
	.word	.LC55
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
	ldr	r5, .L28
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L23
	mov	r3, r0
	str	r1, [sp]
	movw	r2, #1325
	ldr	r1, .L28+4
	ldr	r0, .L28+8
	bl	printk
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L23
	mov	r3, r4
	movw	r2, #1326
	ldr	r1, .L28+4
	ldr	r0, .L28+12
	bl	printk
.L23:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L29:
	.align	2
.L28:
	.word	rc_debug
	.word	.LANCHOR0+26
	.word	.LC56
	.word	.LC57
	.fnend
	.size	rc_model_v2_smt_hal_end, .-rc_model_v2_smt_hal_end
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
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r0, [r0, #604]
	mov	r5, r1
	mov	r4, r2
	bl	mpp_data_sum_v2
	cbnz	r0, .L31
.L56:
	str	r4, [r5]
	pop	{r4, r5, r6, pc}
.L31:
	ldr	r0, [r6, #604]
	bl	mpp_data_sum_v2
	cmp	r0, #1
	bne	.L33
	movs	r1, #0
	ldr	r0, [r6, #604]
	bl	mpp_data_get_pre_val_v2
	cbnz	r0, .L34
	adds	r4, r4, #10
	b	.L56
.L34:
	adds	r4, r4, #25
	b	.L56
.L33:
	ldr	r0, [r6, #604]
	bl	mpp_data_sum_v2
	cmp	r0, #2
	bne	.L35
	movs	r1, #0
	ldr	r0, [r6, #604]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #0
	beq	.L34
.L37:
	adds	r4, r4, #35
	b	.L56
.L35:
	ldr	r0, [r6, #604]
	bl	mpp_data_sum_v2
	cmp	r0, #3
	bne	.L36
	movs	r1, #0
.L55:
	ldr	r0, [r6, #604]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #0
	beq	.L37
	adds	r4, r4, #51
	b	.L56
.L36:
	ldr	r0, [r6, #604]
	bl	mpp_data_sum_v2
	cmp	r0, #3
	ble	.L38
	ldr	r0, [r6, #604]
	bl	mpp_data_sum_v2
	cmp	r0, #6
	bgt	.L38
	movs	r1, #0
	ldr	r0, [r6, #604]
	bl	mpp_data_get_pre_val_v2
	cbnz	r0, .L39
	movs	r1, #1
	b	.L55
.L39:
	adds	r4, r4, #64
	b	.L56
.L38:
	ldr	r0, [r6, #604]
	bl	mpp_data_sum_v2
	cmp	r0, #6
	ble	.L41
	ldr	r0, [r6, #604]
	bl	mpp_data_sum_v2
	cmp	r0, #9
	bgt	.L41
	movs	r1, #0
	ldr	r0, [r6, #604]
	bl	mpp_data_get_pre_val_v2
	cbnz	r0, .L42
	movs	r1, #1
	ldr	r0, [r6, #604]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #0
	beq	.L39
.L42:
	adds	r4, r4, #72
	b	.L56
.L41:
	adds	r4, r4, #80
	b	.L56
	.fnend
	.size	set_coef, .-set_coef
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
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #60
	sub	sp, sp, #60
	ldr	r3, [r0, #124]
	mov	r4, r0
	ldr	fp, [r0, #120]
	str	r1, [sp, #16]
	str	r3, [sp, #20]
	ldr	r3, [r0, #132]
	str	r3, [sp, #44]
	ldr	r3, [r0, #128]
	ldr	r0, [r1, #296]
	str	r3, [sp, #24]
	bl	mpp_frame_get_ppinfo
	cbz	r0, .L58
	ldr	r3, [r0, #8]
	lsls	r1, r3, #31
	bpl	.L58
	ubfx	r2, r3, #4, #9
	asrs	r3, r3, #16
	tst	r3, #254
	uxtb	r1, r3
	bne	.L59
	cmp	r1, #0
	bne	.L183
	mov	fp, #30
	cbnz	r2, .L60
	b	.L58
.L59:
	orrs	r3, r2, r1
	mov	fp, #33
	beq	.L58
.L60:
	ldr	r3, [sp, #24]
	cmp	r3, #37
	it	ge
	movge	r3, #37
	str	r3, [sp, #24]
.L58:
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	cbnz	r0, .L62
	ldr	r3, [r4, #248]
	cbz	r3, .L62
	mov	fp, #32
	str	fp, [sp, #20]
.L62:
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #156]	@ zero_extendqisi2
	ands	r2, r3, #4
	bne	.L231
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	ands	r3, r3, #16
	beq	.L65
	movs	r3, #2
	movs	r0, #0
	movs	r1, #0
	str	r3, [r4, #356]
	strd	r0, [r4, #544]
	str	r2, [r4, #528]
.L66:
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #154]	@ zero_extendqisi2
	and	r3, r3, #63
	cmp	r3, #3
	it	eq
	streq	r3, [r4, #356]
	ldr	r3, [r4, #516]
	cbz	r3, .L68
	cmp	r3, #1
	beq	.L69
	ldr	r3, [r4, #356]
	cmp	r3, #2
	bne	.L73
	add	r5, r4, #608
	mov	r0, r5
	bl	mpp_pid_calc
	ldr	r3, [r4, #612]
	mov	r6, r0
	subs	r3, r3, r0
	mov	r0, r5
	str	r3, [r4, #572]
	bl	mpp_pid_reset
	ldrd	r2, [r4, #552]
	ldr	r5, [r4, #664]
	orrs	r3, r2, r3
	beq	.L74
	ldr	r0, [r4, #676]
	add	r5, r5, r6
	str	r5, [r4, #652]
	add	r0, r0, r6
.L250:
	str	r0, [r4, #656]
	b	.L72
.L183:
	mov	fp, #32
	b	.L60
.L65:
	str	r3, [r4, #356]
	b	.L66
.L68:
	ldr	r3, [r4, #356]
	cmp	r3, #2
	bne	.L71
	ldr	r3, [r4, #664]
	str	r3, [r4, #652]
	ldr	r3, [r4, #676]
	str	r3, [r4, #656]
.L72:
	ldr	r3, [r4, #576]
	cbnz	r3, .L77
	ldr	r1, [r4, #692]
	add	r0, r4, #576
	cmp	r1, #15
	it	ge
	movge	r1, #15
	bl	mpp_data_init
.L77:
	ldr	r3, [r4, #580]
	cbnz	r3, .L78
	ldr	r1, [r4, #692]
	add	r0, r4, #580
	cmp	r1, #15
	it	ge
	movge	r1, #15
	bl	mpp_data_init
.L78:
	ldrd	r6, [r4, #488]
	orrs	r3, r6, r7
	bne	.L79
	ldr	r3, [r4]
	ldr	r0, [r4, #4]
	ldr	r2, [r4, #28]
	adds	r3, r3, #15
	adds	r0, r0, #15
	cmp	r2, #9
	asr	r3, r3, #4
	asr	r0, r0, #4
	ble	.L185
	cmp	r2, #20
	ite	lt
	movlt	r5, #2
	movge	r5, #3
.L80:
	ldrd	r2, r1, [r4, #652]
	muls	r0, r3, r0
	sub	ip, r1, r2
	muls	r1, r5, r1
	add	ip, ip, ip, lsr #31
	ldr	r5, .L260
	movs	r3, #0
	add	r2, r2, ip, asr #1
	movw	ip, #4856
	str	r2, [r4, ip]
.L81:
	adds	r3, r3, #1
	cmp	r3, #8
	beq	.L82
	ldr	r2, [r5, r3, lsl #2]
	cmp	r0, r2
	bcs	.L81
.L82:
	ldr	r5, .L260+4
	add	r3, r5, r3, lsl #2
	ldr	r3, [r3, #184]
	muls	r0, r3, r0
	sub	r0, r0, #350
	bl	__aeabi_uidiv
	movw	r2, #4860
	cmp	r0, #95
	it	ge
	movge	r0, #95
	cmp	r0, #4
	ite	ge
	addge	r5, r5, r0
	addlt	r5, r5, #4
	ldr	r0, [sp, #20]
	ldrb	r3, [r5, #88]	@ zero_extendqisi2
	cmp	r0, #31
	str	r3, [r4, r2]
	ldr	r1, [r4, #508]
	ble	.L84
	cmp	r0, r3
	bgt	.L187
.L258:
	cmp	r3, r1
	it	ge
	movge	r3, r1
	b	.L87
.L71:
	add	r0, r4, #768
	ldr	r5, [r4, #668]
	bl	mpp_pid_calc
	subs	r0, r5, r0
	ldr	r5, [r4, #680]
	str	r0, [r4, #652]
	add	r0, r4, #804
.L249:
	bl	mpp_pid_calc
	subs	r0, r5, r0
	b	.L250
.L69:
	ldr	r5, [r4, #664]
.L74:
	add	r0, r4, #696
	bl	mpp_pid_calc
	subs	r0, r5, r0
	ldr	r5, [r4, #676]
	str	r0, [r4, #652]
	add	r0, r4, #732
	b	.L249
.L73:
	ldr	r3, [r4, #360]
	cmp	r3, #2
	bne	.L75
	add	r0, r4, #608
	bl	mpp_pid_calc
	movs	r3, #1
	mov	r1, r3
	mov	r2, r3
	mov	r8, r0
	ldr	r0, [r4, #584]
	bl	mpp_data_avg
	ldr	r9, [r4, #692]
	ldr	r10, [r4, #28]
	ldr	r5, [r4, #572]
	add	r7, r9, #-1
	mov	r1, r10
	mov	r6, r0
	ldr	r0, [r4, #644]
	mul	r0, r0, r9
	bl	__aeabi_idiv
	mov	r1, r7
	add	r0, r0, r5
	add	r0, r0, r8
	subs	r0, r0, r6
	bl	__aeabi_idiv
	mov	r1, r10
	str	r0, [r4, #668]
	str	r0, [r4, #652]
	ldr	r0, [r4, #648]
	mul	r0, r0, r9
	bl	__aeabi_idiv
	mov	r1, r7
	add	r0, r0, r5
	add	r0, r0, r8
	subs	r0, r0, r6
	bl	__aeabi_idiv
	str	r0, [r4, #680]
	b	.L250
.L75:
	add	r0, r4, #768
	bl	mpp_pid_calc
	mov	r5, r0
	add	r0, r4, #804
	bl	mpp_pid_calc
	ldr	r2, [r4, #660]
	ldr	r3, [r4, #668]
	lsls	r2, r2, #1
	subs	r3, r3, r5
	cmp	r2, r3
	ite	le
	strle	r2, [r4, #652]
	strgt	r3, [r4, #652]
	ldr	r3, [r4, #680]
	subs	r0, r3, r0
	ldr	r3, [r4, #672]
	lsls	r3, r3, #1
	cmp	r3, r0
	ite	le
	strle	r3, [r4, #656]
	strgt	r0, [r4, #656]
	b	.L72
.L185:
	movs	r5, #1
	b	.L80
.L187:
	ldr	r3, [sp, #20]
.L87:
	str	r3, [r4, r2]
	movw	r3, #4868
	movs	r2, #0
	str	r2, [r4, r3]
.L79:
	ldr	r3, [r4, #356]
	ldr	r9, .L260+20
	cmp	r3, #2
	bne	.L88
	cmp	r6, #1
	sbcs	r3, r7, #0
	blt	.L90
	ldrd	r6, r3, [r4, #652]
	mov	r1, #-1
	ldr	r0, [r4, #576]
	add	r6, r6, r3
	movs	r3, #1
	mov	r2, r3
	bl	mpp_data_avg
	ldr	r3, [sp, #20]
	cmp	r0, r3
	blt	.L189
	ldr	r3, [sp, #44]
	cmp	r3, r0
	it	ge
	movge	r3, r0
	mov	r5, r3
.L91:
	cmp	r6, #199
	movw	r3, #4856
	it	gt
	asrgt	r6, r6, #1
	movw	r1, #4860
	it	le
	movle	r6, #100
	str	r6, [r4, r3]
	movw	r3, #4876
	movw	r2, #4872
	ldr	r3, [r4, r3]
	str	r3, [r4, r1]
	ldr	r2, [r4, r2]
	cmp	r2, r6
	blt	.L93
	add	r0, r6, r6, lsl #2
	cmp	r2, r0
	ble	.L94
	adds	r3, r3, #3
.L251:
	str	r3, [r4, r1]
.L95:
	add	r3, r4, #4864
	ldr	r3, [r3]
	cmp	r3, #31
	ble	.L191
	cmp	r3, #45
	mov	r8, #1
	bgt	.L192
	cmp	r3, #40
	ite	gt
	movgt	r10, #3
	movle	r10, #2
.L99:
	ldr	r3, [r4, #496]
	cmp	r3, #0
	bne	.L101
	ldr	r3, [r4, #200]
	cmp	r3, #0
	beq	.L101
	movw	r3, #4900
	ldr	r6, [r4, r3]
	ldr	r3, [r9]
	asrs	r6, r6, #5
	cmp	r6, #7
	it	ge
	movge	r6, #7
	lsls	r2, r3, #31
	bpl	.L103
	mov	r3, r4
	movw	r2, #794
	ldr	r1, .L260+8
	ldr	r0, .L260+12
	bl	printk
.L103:
	add	r3, r4, #4896
	str	r3, [sp, #28]
	ldr	r1, [r3]
	movw	r3, #4892
	ldr	r0, [r4, r3]
	bl	__aeabi_idiv
	ldr	r3, [r9]
	cmp	r0, #35
	it	ge
	movge	r0, #35
	lsls	r3, r3, #24
	mov	r7, r0
	bpl	.L104
	movw	r3, #4900
	movw	r2, #799
	ldr	r3, [r4, r3]
	ldr	r1, .L260+8
	strd	r0, r3, [sp]
	movw	r3, #4860
	ldr	r3, [r4, r3]
	ldr	r0, .L260+16
	bl	printk
.L104:
	ldr	r2, .L260+4
	ldr	r3, [r4, #204]
	uxtab	r6, r2, r6
	add	r3, r3, r2
	ldrsb	r2, [r6, #277]
	ldrsb	r3, [r3, #241]
	subs	r1, r3, r2
	cmp	r7, r1
	itt	gt
	subgt	r7, r7, r3
	addgt	r7, r7, r2
	ldrd	r2, r3, [r4, #104]
	cmp	r3, r7
	bgt	.L106
	cmp	r2, r7
	mov	r3, r2
	it	ge
	movge	r3, r7
.L106:
	movw	r2, #4860
	str	r3, [r4, r2]
	movs	r3, #0
	ldr	r2, [sp, #28]
	str	r3, [r2]
	movw	r2, #4892
	str	r3, [r4, r2]
	ldr	r3, .L260+20
	ldr	r3, [r3]
	lsls	r0, r3, #31
	bpl	.L101
	mov	r3, r4
	movw	r2, #810
	ldr	r1, .L260+8
	ldr	r0, .L260+24
	bl	printk
.L101:
	movw	r3, #4860
	subs	r0, r5, #6
	ldr	r3, [r4, r3]
	cmp	r3, r0
	blt	.L107
	subs	r5, r5, #1
	cmp	r5, r3
	mov	r0, r5
	it	ge
	movge	r0, r3
.L107:
	add	r3, r4, #4864
	ldr	r2, [r3]
	subs	r3, r2, #4
	sub	r3, r3, r10
	cmp	r3, r0
	bgt	.L163
	add	r10, r2, r8
	cmp	r10, r0
	mov	r3, r10
	it	ge
	movge	r3, r0
.L163:
	movw	r2, #4860
	str	r3, [r4, r2]
	b	.L90
.L84:
	cmp	r3, #30
	bgt	.L258
	movs	r3, #31
	b	.L87
.L189:
	ldr	r5, [sp, #20]
	b	.L91
.L94:
	lsls	r0, r6, #1
	cmp	r2, r0
	ble	.L96
	adds	r3, r3, #2
	b	.L251
.L96:
	add	r6, r6, r0
	cmp	r6, r2, lsl #1
	bge	.L95
	adds	r3, r3, #1
	b	.L251
.L93:
	lsls	r0, r2, #1
	add	r2, r2, r0
	cmp	r2, r6
	bge	.L97
	subs	r3, r3, #3
	b	.L251
.L97:
	cmp	r0, r6
	bge	.L98
	subs	r3, r3, #2
	b	.L251
.L98:
	cmp	r2, r6, lsl #1
	bge	.L95
	subs	r3, r3, #1
	b	.L251
.L261:
	.align	2
.L260:
	.word	.LANCHOR0+52
	.word	.LANCHOR0
	.word	.LANCHOR0+220
	.word	.LC58
	.word	.LC59
	.word	rc_debug
	.word	.LC57
.L191:
	mov	r8, #2
	mov	r10, r8
	b	.L99
.L192:
	mov	r10, #4
	b	.L99
.L88:
	ldrd	r2, r3, [r4, #652]
	movw	r1, #4856
	add	r2, r2, r3
	add	r3, r2, r2, lsr #31
	asrs	r3, r3, #1
	str	r3, [r4, r1]
	movw	r3, #4868
	ldr	r1, [r4, r3]
	movw	r3, #4860
	str	r1, [r4, r3]
	ldr	r3, [r4, #360]
	cmp	r3, #2
	bne	.L109
	add	r3, r4, #4864
	ldr	r3, [r3]
	cmp	r3, #32
	ble	.L194
	cmp	r3, #34
	ite	gt
	movgt	r0, #1
	movle	r0, #2
.L110:
	add	r3, r3, r0
	cmp	r1, r3
	blt	.L111
	adds	r3, r3, #4
	cmp	r3, r1
	it	ge
	movge	r3, r1
.L111:
	cmp	r2, #199
	movw	r1, #4860
	str	r3, [r4, r1]
	bgt	.L90
	movw	r3, #4856
	movs	r2, #100
	str	r2, [r4, r3]
.L90:
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	cmp	r0, #700
	blt	.L164
.L167:
	ldr	r0, [r4, #604]
	bl	mpp_data_sum_v2
	cmp	r0, #14
	bgt	.L203
.L166:
	movs	r3, #6
.L165:
	ldr	r0, [r4, #356]
	movw	r2, #4860
	ldr	r1, [r4, r2]
	cmp	r0, #2
	bne	.L175
	ldr	r2, [sp, #20]
	add	r3, r3, r2
	cmp	r3, r1
	bgt	.L180
	ldr	r3, [sp, #44]
	cmp	r3, r1
	it	ge
	movge	r3, r1
.L180:
	movw	r2, #4860
	str	r3, [r4, r2]
	b	.L177
.L194:
	movs	r0, #3
	b	.L110
.L109:
	mov	r3, #1024
	str	r3, [sp, #52]
	ldr	r3, [r9]
	ldr	r0, [r4, #600]
	lsls	r1, r3, #25
	bpl	.L113
	movs	r1, #0
	bl	mpp_data_get_pre_val_v2
	mov	r5, r0
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	movs	r1, #0
	mov	r6, r0
	ldr	r0, [r4, #604]
	bl	mpp_data_get_pre_val_v2
	mov	r7, r0
	ldr	r0, [r4, #604]
	bl	mpp_data_sum_v2
	mov	r3, r5
	movw	r2, #1025
	ldr	r1, .L262
	strd	r7, r0, [sp, #4]
	str	r6, [sp]
	ldr	r0, .L262+4
	bl	printk
.L113:
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	cmp	r0, #99
	bgt	.L114
	movs	r2, #0
.L252:
	mov	r0, r4
	add	r1, sp, #52
	bl	set_coef
	ldr	r3, [sp, #52]
	cmp	r3, #1024
	itt	gt
	movgt	r3, #1024
	strgt	r3, [sp, #52]
	ldr	r0, [sp, #52]
	cmp	r0, #900
	bge	.L196
	cmp	r0, #306
	ble	.L134
	mvn	r2, #306
	add	r0, r0, r2
	ldr	r3, .L262+8
	lsls	r0, r0, #9
	umull	r0, r1, r0, r3
	lsrs	r0, r1, #7
	add	r0, r0, #512
.L135:
	cmp	r0, #1024
	it	ge
	movge	r0, #1024
	mov	r6, r0
.L133:
	ldrd	r3, r2, [r4, #652]
	subs	r2, r2, r3
	lsl	r8, r3, #10
	str	r2, [sp, #28]
	mla	r3, r6, r2, r8
	ldrd	r1, r2, [r4, #684]
	subs	r2, r2, r1
	str	r2, [sp, #32]
	lsls	r2, r1, #10
	str	r2, [sp, #36]
	ldrd	r2, r1, [sp, #32]
	asrs	r3, r3, #10
	cmp	r3, #100
	mla	r9, r6, r2, r1
	movw	r2, #4856
	it	lt
	movlt	r3, #100
	add	r1, r4, #4864
	str	r3, [r4, r2]
	lsls	r0, r3, #1
	ldr	r5, [r1]
	movs	r1, #100
	adds	r7, r0, r3
	asr	r9, r9, #10
	eor	r2, r9, r9, asr #31
	sub	r2, r2, r9, asr #31
	mul	ip, r1, r2
	cmp	ip, r7
	bgt	.L136
	subs	r3, r5, #1
.L144:
	movw	r2, #4860
.L253:
	str	r3, [r4, r2]
.L137:
	movw	r3, #4860
	ldrd	r7, r1, [r4, #504]
	ldr	r2, [r4, r3]
	cmp	r2, r7
	str	r1, [sp, #40]
	blt	.L197
	cmp	r1, r2
	it	ge
	movge	r1, r2
	mov	r10, r1
.L149:
	cmp	r10, #34
	str	r10, [r4, r3]
	ble	.L150
	ldrd	r0, r3, [r4, #644]
	ldr	r1, [r4, #28]
	add	r0, r0, r3
	add	r0, r0, r0, lsr #31
	asrs	r0, r0, #1
	bl	__aeabi_idiv
	ldr	r2, .L262+12
	rsbs	r0, r0, #0
	smull	r2, r3, r0, r2
	asrs	r0, r0, #31
	rsb	r0, r0, r3, asr #1
	cmp	r9, r0, lsl #1
	bgt	.L151
	adds	r6, r6, #205
.L152:
	cmp	r10, #35
	bne	.L198
	cmp	r6, #1024
	it	ge
	movge	r6, #1024
.L154:
	ldrd	r3, r2, [sp, #32]
	movs	r1, #100
	mla	r3, r6, r3, r2
	ldr	r2, [sp, #28]
	mla	r6, r6, r2, r8
	movw	r2, #4856
	asrs	r3, r3, #10
	asrs	r6, r6, #10
	cmp	r6, #100
	it	lt
	movlt	r6, #100
	str	r6, [r4, r2]
	eor	r2, r3, r3, asr #31
	lsls	r0, r6, #1
	sub	r2, r2, r3, asr #31
	add	ip, r0, r6
	mul	lr, r1, r2
	cmp	lr, ip
	bgt	.L155
	movw	r3, #4860
.L160:
	str	r5, [r4, r3]
	b	.L150
.L114:
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	cmp	r0, #199
	bgt	.L116
	movs	r1, #0
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #99
	bgt	.L117
	movs	r2, #102
	b	.L252
.L117:
	movs	r2, #154
	b	.L252
.L116:
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	cmp	r0, #300
	bge	.L118
	movs	r1, #0
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #99
	ble	.L117
	movs	r1, #0
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #100
	bne	.L119
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #99
	bgt	.L120
	movs	r2, #205
	b	.L252
.L120:
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #100
	bne	.L119
	mov	r2, #256
	b	.L252
.L119:
	movw	r2, #307
	b	.L252
.L118:
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	cmp	r0, #600
	bge	.L121
	movs	r1, #0
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #99
	bgt	.L122
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #99
	ble	.L119
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #100
	bne	.L123
.L125:
	mov	r2, #358
	b	.L252
.L123:
	mov	r2, #410
	b	.L252
.L122:
	movs	r1, #0
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #100
	bne	.L124
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #99
	ble	.L125
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #100
	beq	.L123
.L124:
	movw	r2, #461
	b	.L252
.L121:
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	cmp	r0, #900
	bge	.L126
	movs	r1, #0
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #99
	bgt	.L127
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #99
	ble	.L123
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #100
	beq	.L124
.L129:
	mov	r2, #512
	b	.L252
.L127:
	movs	r1, #0
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #100
	bne	.L128
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #99
	ble	.L129
	movs	r1, #1
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #100
	bne	.L128
	movw	r2, #563
	b	.L252
.L128:
	movw	r2, #614
	b	.L252
.L126:
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	movw	r3, #1499
	cmp	r0, r3
	bgt	.L130
	movw	r2, #666
	b	.L252
.L130:
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	movw	r3, #1899
	cmp	r0, r3
	ite	le
	movle	r2, #768
	movgt	r2, #900
	b	.L252
.L134:
	movw	r1, #307
	lsls	r0, r0, #9
	bl	__aeabi_idiv
	b	.L135
.L196:
	mov	r6, #1024
	b	.L133
.L136:
	mul	r1, r1, r9
	cmp	r7, r1
	bge	.L138
	cmp	r9, r3
	movw	r2, #4860
	blt	.L139
	ldr	r3, [r4, r2]
	cmp	r3, #29
	ble	.L259
	subs	r3, r5, #4
	b	.L144
.L139:
	cmp	r3, r9, lsl #2
	bgt	.L142
	ldr	r3, [r4, r2]
	cmp	r3, #29
	ble	.L143
.L259:
	subs	r3, r5, #3
	b	.L144
.L143:
	subs	r3, r5, #2
	b	.L144
.L142:
	movs	r1, #10
	mul	r1, r1, r9
	cmp	r1, r3
	ite	gt
	subgt	r3, r5, #2
	addle	r3, r5, #-1
	b	.L253
.L263:
	.align	2
.L262:
	.word	.LANCHOR0+285
	.word	.LC60
	.word	1340867839
	.word	1717986919
.L138:
	cmp	r5, #35
	ite	gt
	movgt	r1, #0
	movle	r1, #1
	cmp	r0, r2
	bgt	.L146
	adds	r3, r5, #2
.L254:
	add	r1, r1, r3
	movw	r3, #4860
	str	r1, [r4, r3]
	b	.L137
.L146:
	add	r7, r2, r2, lsl #1
	cmp	r0, r7
	bgt	.L147
	adds	r3, r5, #1
	b	.L254
.L147:
	add	r2, r2, r2, lsl #2
	cmp	r2, r3
	movw	r3, #4860
	itet	gt
	addgt	r2, r5, #1
	strle	r5, [r4, r3]
	strgt	r2, [r4, r3]
	b	.L137
.L197:
	mov	r10, r7
	b	.L149
.L151:
	cmp	r9, r0
	ite	le
	addle	r6, r6, #102
	addgt	r6, r6, #51
	b	.L152
.L198:
	mov	r6, #1024
	b	.L154
.L155:
	muls	r1, r3, r1
	cmp	ip, r1
	bge	.L156
	cmp	r6, r3
	movw	r2, #4860
	bgt	.L157
	subs	r3, r5, #3
.L255:
	str	r3, [r4, r2]
.L150:
	movw	r3, #4860
	ldr	r3, [r4, r3]
	cmp	r7, r3
	bgt	.L161
	ldr	r2, [sp, #40]
	cmp	r2, r3
	it	ge
	movge	r2, r3
	mov	r7, r2
.L161:
	cmp	r7, #40
	bgt	.L199
	cmp	r7, #36
	bgt	.L200
	cmp	r7, #33
	bgt	.L201
	cmp	r7, #30
	mov	r3, #1
	ite	gt
	movgt	r2, #3
	movle	r2, #4
.L162:
	subs	r3, r5, r3
	cmp	r3, r7
	bgt	.L163
	adds	r3, r2, r5
	cmp	r3, r7
	it	ge
	movge	r3, r7
	b	.L163
.L157:
	cmp	r6, r3, lsl #2
	bgt	.L158
	subs	r3, r5, #2
	b	.L255
.L158:
	movs	r1, #10
	muls	r3, r1, r3
	cmp	r6, r3
	bge	.L159
	subs	r3, r5, #1
	b	.L255
.L159:
	str	r5, [r4, r2]
	b	.L150
.L156:
	add	r2, r2, r2, lsl #1
	movw	r3, #4860
	cmp	r2, r0
	blt	.L160
	adds	r2, r5, #1
	str	r2, [r4, r3]
	b	.L150
.L199:
	movs	r3, #4
.L256:
	movs	r2, #1
	b	.L162
.L200:
	movs	r3, #3
	b	.L256
.L201:
	movs	r3, #2
	mov	r2, r3
	b	.L162
.L164:
	movs	r1, #0
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #200
	beq	.L167
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	cmp	r0, #400
	blt	.L169
.L171:
	ldr	r0, [r4, #604]
	bl	mpp_data_sum_v2
	cmp	r0, #14
	bgt	.L166
.L170:
	movs	r3, #5
	b	.L165
.L169:
	movs	r1, #0
	ldr	r0, [r4, #600]
	bl	mpp_data_get_pre_val_v2
	cmp	r0, #100
	beq	.L171
	ldr	r0, [r4, #600]
	bl	mpp_data_sum_v2
	cmp	r0, #99
	ldr	r0, [r4, #604]
	ble	.L173
	bl	mpp_data_sum_v2
	cmp	r0, #14
.L257:
	bgt	.L170
	movs	r3, #4
	b	.L165
.L173:
	bl	mpp_data_sum_v2
	cmp	r0, #11
	b	.L257
.L203:
	movs	r3, #7
	b	.L165
.L175:
	add	r2, r3, fp
	cmp	r2, r1
	bgt	.L178
	ldr	r2, [sp, #24]
	cmp	r2, r1
	it	ge
	movge	r2, r1
.L178:
	cmp	r0, #3
	beq	.L179
	movw	r3, #4860
	str	r2, [r4, r3]
.L177:
	movw	r3, #4860
	ldrd	r2, r0, [r4, #504]
	ldr	r1, [r4, r3]
	cmp	r1, r2
	blt	.L181
	cmp	r1, r0
	mov	r2, r1
	it	ge
	movge	r2, r0
.L181:
	str	r2, [r4, r3]
	movw	r2, #4856
	ldr	r2, [r4, r2]
	ldr	r1, [sp, #16]
	str	r2, [r1, #160]
	ldr	r3, [r4, r3]
	str	r3, [r1, #172]
	ldr	r3, [r4, #96]
	str	r3, [r1, #176]
	ldr	r3, [r4, #100]
	str	r3, [r1, #180]
	ldrd	r3, r2, [r4, #488]
	adds	r3, r3, #1
	adc	r2, r2, #0
	strd	r3, r2, [r4, #488]
	movs	r3, #0
	str	r3, [r4, #496]
	ldr	r3, .L264
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L231
	mov	r3, r4
	movw	r2, #1242
	ldr	r1, .L264+4
	ldr	r0, .L264+8
	bl	printk
.L231:
	movs	r0, #0
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L179:
	add	r1, fp, #-1
	subs	r2, r2, #1
	add	r3, r3, r1
	cmp	r2, r3
	blt	.L180
	ldr	r3, [sp, #24]
	cmp	r3, r2
	it	ge
	movge	r3, r2
	b	.L180
.L265:
	.align	2
.L264:
	.word	rc_debug
	.word	.LANCHOR0+285
	.word	.LC57
	.fnend
	.size	rc_model_v2_smt_start, .-rc_model_v2_smt_start
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
	ldr	r5, .L327
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L267
	mov	r3, r0
	movs	r2, #151
	ldr	r1, .L327+4
	ldr	r0, .L327+8
	bl	printk
.L267:
	ldr	r0, [r4, #576]
	cbz	r0, .L268
	bl	mpp_data_deinit
	movs	r3, #0
	str	r3, [r4, #576]
.L268:
	ldr	r0, [r4, #580]
	cbz	r0, .L269
	bl	mpp_data_deinit
	movs	r3, #0
	str	r3, [r4, #580]
.L269:
	ldr	r0, [r4, #584]
	cbz	r0, .L270
	bl	mpp_data_deinit
	movs	r3, #0
	str	r3, [r4, #584]
.L270:
	ldr	r0, [r4, #588]
	cbz	r0, .L271
	bl	mpp_data_deinit
	movs	r3, #0
	str	r3, [r4, #588]
.L271:
	ldr	r0, [r4, #592]
	cbz	r0, .L272
	bl	mpp_data_deinit
	movs	r3, #0
	str	r3, [r4, #592]
.L272:
	ldr	r0, [r4, #596]
	cbz	r0, .L273
	bl	mpp_data_deinit
	movs	r3, #0
	str	r3, [r4, #596]
.L273:
	ldr	r0, [r4, #372]
	cbz	r0, .L274
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #372]
.L274:
	ldr	r0, [r4, #396]
	cbz	r0, .L275
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #396]
.L275:
	ldr	r0, [r4, #408]
	cbz	r0, .L276
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #408]
.L276:
	ldr	r0, [r4, #444]
	cbz	r0, .L277
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #444]
.L277:
	ldr	r0, [r4, #440]
	cbz	r0, .L278
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #440]
.L278:
	ldr	r0, [r4, #600]
	cbz	r0, .L279
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #600]
.L279:
	ldr	r0, [r4, #604]
	cbz	r0, .L280
	bl	mpp_data_deinit_v2
	movs	r3, #0
	str	r3, [r4, #604]
.L280:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L281
	mov	r3, r4
	movs	r2, #218
	ldr	r1, .L327+4
	ldr	r0, .L327+12
	bl	printk
.L281:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L328:
	.align	2
.L327:
	.word	rc_debug
	.word	.LANCHOR0+307
	.word	.LC58
	.word	.LC57
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
	ldr	r4, .L338
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L330
	mov	r3, r0
	movw	r2, #686
	ldr	r1, .L338+4
	ldr	r0, .L338+8
	bl	printk
.L330:
	mov	r0, r5
	bl	bits_model_smt_deinit
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L331
	mov	r3, r5
	mov	r2, #688
	ldr	r1, .L338+4
	ldr	r0, .L338+12
	bl	printk
.L331:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L339:
	.align	2
.L338:
	.word	rc_debug
	.word	.LANCHOR0+329
	.word	.LC58
	.word	.LC57
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
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r3, [r0, #28]
	.pad #20
	sub	sp, sp, #20
	ldr	r5, [r0, #40]
	ldr	r8, [r0, #412]
	ldr	r0, [r0, #60]
	ldr	r1, [r4, #32]
	muls	r0, r3, r0
	bl	__aeabi_idiv
	ldr	r3, .L396
	ldr	r2, [r3]
	subs	r7, r0, #0
	str	r3, [sp, #12]
	it	eq
	moveq	r7, #8
	tst	r2, #1
	beq	.L342
	mov	r3, r4
	movs	r2, #235
	ldr	r1, .L396+4
	ldr	r0, .L396+8
	bl	printk
.L342:
	movs	r2, #0
	movs	r3, #0
	strd	r2, [r4, #488]
	add	r2, r4, #4896
	movs	r3, #1
	str	r3, [r4, #368]
	movs	r3, #0
	str	r3, [r2]
	movw	r2, #4892
	str	r3, [r4, r2]
	movw	r2, #4852
	str	r3, [r4, r2]
	movw	r2, #4840
	str	r3, [r4, r2]
	movs	r2, #18
	ldrd	r6, r0, [r4, #28]
	movs	r3, #51
	str	r5, [r4, #692]
	strd	r2, r3, [r4, #504]
	movs	r3, #4
	mov	r1, r6
	str	r3, [r4, #512]
	mul	r0, r0, r8
	bl	__aeabi_idiv
	cmp	r6, r5
	mov	r3, r6
	it	lt
	movlt	r3, r5
	cmp	r3, #9
	it	le
	movle	r3, #10
	str	r3, [r4, #520]
	ldr	r3, [r4, #520]
	cmp	r6, r3
	it	lt
	strlt	r6, [r4, #520]
	str	r0, [r4, #364]
	ldr	r0, [r4, #584]
	cbz	r0, .L346
	bl	mpp_data_deinit
.L346:
	mov	r1, r5
	add	r0, r4, #584
	bl	mpp_data_init
	ldr	r0, [r4, #588]
	cbz	r0, .L347
	bl	mpp_data_deinit
.L347:
	ldr	r1, [r4, #28]
	add	r0, r4, #588
	bl	mpp_data_init
	ldr	r0, [r4, #592]
	cbz	r0, .L348
	bl	mpp_data_deinit
.L348:
	mov	r1, r5
	add	r0, r4, #592
	bl	mpp_data_init
	ldr	r0, [r4, #596]
	cbz	r0, .L349
	bl	mpp_data_deinit
.L349:
	mov	r1, r5
	add	r0, r4, #596
	bl	mpp_data_init
	ldr	r0, [r4, #600]
	cbz	r0, .L350
	bl	mpp_data_deinit_v2
.L350:
	movs	r2, #0
	movs	r1, #10
	add	r0, r4, #600
	bl	mpp_data_init_v2
	ldr	r0, [r4, #604]
	cbz	r0, .L351
	bl	mpp_data_deinit_v2
.L351:
	movs	r2, #0
	movs	r1, #10
	add	r0, r4, #604
	bl	mpp_data_init_v2
	ldr	r0, [r4, #440]
	cbz	r0, .L352
	bl	mpp_data_deinit_v2
.L352:
	add	fp, r4, #608
	mov	r1, r7
	ldr	r2, [r4, #364]
	add	r10, r4, #696
	add	r0, r4, #440
	add	r9, r4, #732
	bl	mpp_data_init_v2
	mov	r0, fp
	bl	mpp_pid_reset
	add	r8, r4, #768
	mov	r0, r10
	add	r7, r4, #804
	bl	mpp_pid_reset
	mov	r0, r9
	bl	mpp_pid_reset
	mov	r0, r8
	bl	mpp_pid_reset
	mov	r0, r7
	bl	mpp_pid_reset
	ldr	r3, [r4, #520]
	movs	r6, #100
	movs	r2, #6
	movs	r1, #4
	mov	r0, fp
	str	r3, [sp, #4]
	movs	r3, #0
	str	r6, [sp]
	bl	mpp_pid_set_param
	ldr	r3, [r4, #520]
	movs	r2, #6
	movs	r1, #4
	mov	r0, r10
	strd	r6, r3, [sp]
	movs	r3, #0
	bl	mpp_pid_set_param
	ldr	r3, [r4, #520]
	movs	r2, #6
	movs	r1, #4
	mov	r0, r9
	strd	r6, r3, [sp]
	movs	r3, #0
	bl	mpp_pid_set_param
	ldr	r3, [r4, #520]
	movs	r2, #6
	movs	r1, #4
	mov	r0, r8
	strd	r6, r3, [sp]
	movs	r3, #0
	bl	mpp_pid_set_param
	ldr	r3, [r4, #520]
	mov	r0, r7
	movs	r2, #6
	movs	r1, #4
	strd	r6, r3, [sp]
	movs	r3, #0
	bl	mpp_pid_set_param
	ldr	r3, [r4, #56]
	ldr	r0, [r4, #48]
	ldrd	r2, r1, [r4, #28]
	str	r3, [r4, #648]
	str	r0, [r4, #644]
	bl	axb_div_c
	ldrd	r2, r1, [r4, #28]
	str	r0, [r4, #660]
	ldr	r0, [r4, #648]
	bl	axb_div_c
	movs	r3, #0
	movs	r1, #0
	strd	r3, r3, [r4, #528]
	str	r3, [r4, #536]
	str	r3, [r4, #568]
	ldr	r6, [r4, #660]
	mov	r7, r0
	str	r0, [r4, #672]
	movs	r0, #0
	strd	r0, [r4, #552]
	strd	r0, [r4, #560]
	cbnz	r5, .L353
	movs	r0, #10
	str	r6, [r4, #668]
	muls	r6, r0, r6
	str	r5, [r4, #516]
	muls	r0, r7, r0
	str	r7, [r4, #680]
	str	r6, [r4, #664]
	str	r0, [r4, #676]
.L354:
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L362
	mov	r3, r4
	mov	r2, #380
	ldr	r1, .L396+4
	ldr	r0, .L396+12
	bl	printk
.L362:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L353:
	cmp	r5, #1
	bne	.L355
	ldr	r2, [r4, #200]
	str	r5, [r4, #516]
	strd	r6, r3, [r4, #664]
	strd	r7, r3, [r4, #676]
	str	r3, [r4, #524]
	cmp	r2, #0
	beq	.L354
	str	r3, [r4, #200]
	b	.L354
.L355:
	ldr	r2, [r4, #520]
	adds	r3, r5, #1
	cmp	r2, r5
	ble	.L356
	asrs	r6, r6, #1
	asrs	r7, r7, #1
	str	r3, [r4, #524]
	movs	r2, #2
	str	r6, [r4, #668]
	muls	r6, r3, r6
	str	r2, [r4, #516]
	muls	r3, r7, r3
	str	r7, [r4, #680]
	str	r6, [r4, #664]
	str	r3, [r4, #676]
	b	.L354
.L356:
	cmp	r5, #50
	mov	r2, #3
	str	r3, [r4, #524]
	str	r2, [r4, #516]
	bgt	.L357
	rsb	r3, r6, r6, lsl #3
	add	r3, r3, r3, lsr #31
	asrs	r3, r3, #1
	str	r3, [r4, #664]
	rsb	r3, r7, r7, lsl #3
.L394:
	add	r3, r3, r3, lsr #31
	b	.L395
.L357:
	cmp	r5, #100
	bgt	.L359
	lsls	r3, r6, #3
	asrs	r3, r3, #1
	str	r3, [r4, #664]
	lsls	r3, r7, #3
.L395:
	ldr	r5, [r4, #28]
	asrs	r3, r3, #1
	ldr	r0, [r4, #664]
	rsb	r5, r5, #1
	str	r3, [r4, #676]
	mov	r1, r5
	bl	__aeabi_idiv
	mov	r1, r5
	add	r0, r0, r6
	str	r0, [r4, #668]
	ldr	r0, [r4, #676]
	bl	__aeabi_idiv
	add	r0, r0, r7
	str	r0, [r4, #680]
	b	.L354
.L359:
	cmp	r5, #200
	bgt	.L360
	add	r3, r6, r6, lsl #3
	add	r3, r3, r3, lsr #31
	asrs	r3, r3, #1
	str	r3, [r4, #664]
	add	r3, r7, r7, lsl #3
	b	.L394
.L360:
	cmp	r5, #300
	bgt	.L361
	movs	r3, #10
	mul	r2, r3, r6
	muls	r3, r7, r3
	asrs	r2, r2, #1
	str	r2, [r4, #664]
	b	.L395
.L361:
	movs	r3, #11
	mul	r2, r3, r6
	muls	r3, r7, r3
	add	r2, r2, r2, lsr #31
	asrs	r2, r2, #1
	str	r2, [r4, #664]
	b	.L394
.L397:
	.align	2
.L396:
	.word	rc_debug
	.word	.LANCHOR0+352
	.word	.LC58
	.word	.LC57
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
	ldr	r5, .L407
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L399
	mov	r3, r0
	movw	r2, #659
	ldr	r1, .L407+4
	ldr	r0, .L407+8
	bl	printk
.L399:
	movs	r3, #1
	movs	r2, #252
	str	r3, [r4, #500]
	mov	r1, r6
	mov	r0, r4
	bl	memcpy
	mov	r0, r4
	bl	bits_model_smt_init
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L400
	mov	r3, r4
	movw	r2, #665
	ldr	r1, .L407+4
	ldr	r0, .L407+12
	bl	printk
.L400:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L408:
	.align	2
.L407:
	.word	rc_debug
	.word	.LANCHOR0+372
	.word	.LC58
	.word	.LC57
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
	ldr	r5, .L418
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L410
	mov	r3, r0
	movw	r2, #673
	ldr	r1, .L418+4
	ldr	r0, .L418+8
	bl	printk
.L410:
	movs	r3, #0
	movs	r2, #252
	str	r3, [r4, #500]
	mov	r1, r6
	mov	r0, r4
	bl	memcpy
	mov	r0, r4
	bl	bits_model_smt_init
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L411
	mov	r3, r4
	movw	r2, #679
	ldr	r1, .L418+4
	ldr	r0, .L418+12
	bl	printk
.L411:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L419:
	.align	2
.L418:
	.word	rc_debug
	.word	.LANCHOR0+398
	.word	.LC58
	.word	.LC57
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
	ldr	r6, .L440
	mov	r5, r1
	ldr	r8, [r0, #40]
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L421
	mov	r3, r0
	mov	r2, #392
	ldr	r1, .L440+4
	ldr	r0, .L440+8
	bl	printk
.L421:
	mov	r1, r5
	ldr	r0, [r4, #440]
	bl	mpp_data_update_v2
	ldr	r3, [r4, #652]
	movw	r9, #4852
	mov	r1, #1000
	subs	r3, r3, r5
	str	r3, [r4, #684]
	ldr	r3, [r4, #656]
	subs	r3, r3, r5
	str	r3, [r4, #688]
	ldr	r7, [r4, r9]
	mov	r0, r7
	adds	r7, r7, #1
	bl	__aeabi_idivmod
	cmp	r7, r8
	add	r1, r1, #210
	str	r5, [r4, r1, lsl #2]
	beq	.L422
	str	r7, [r4, r9]
.L423:
	ldr	r3, [r4, #356]
	cmp	r3, #2
	bne	.L426
	ldrd	r3, r2, [r4, #552]
	mov	r1, r5
	ldr	r0, [r4, #584]
	adds	r3, r3, #1
	adc	r2, r2, #0
	strd	r3, r2, [r4, #552]
	ldr	r3, [r4, #528]
	add	r3, r3, r5
	str	r3, [r4, #528]
	bl	mpp_data_update
	mov	r1, r5
	ldr	r0, [r4, #592]
	bl	mpp_data_update
	ldr	r1, [r4, #652]
	add	r0, r4, #696
	subs	r1, r5, r1
	bl	mpp_pid_update
	ldr	r1, [r4, #656]
	add	r0, r4, #732
	subs	r1, r5, r1
.L438:
	bl	mpp_pid_update
	ldrd	r3, r2, [r4, #544]
	ldr	r0, [r4, #552]
	adds	r3, r3, #1
	ldr	r1, [r4, #28]
	adc	r2, r2, #0
	strd	r3, r2, [r4, #544]
	ldr	r3, [r4, #568]
	add	r5, r5, r3
	ldr	r3, [r4, #560]
	str	r5, [r4, #568]
	add	r0, r0, r3
	bl	__aeabi_idivmod
	cbnz	r1, .L428
	ldrd	r3, r2, [r4, #644]
	add	r0, r4, #608
	adds	r1, r3, r2
	asrs	r1, r1, #1
	add	r7, r1, r1, lsl #1
	cmp	r7, r5, lsl #1
	ble	.L429
.L439:
	subs	r1, r1, r5
	bl	mpp_pid_update
	movs	r3, #0
	strd	r3, r3, [r4, #528]
	str	r3, [r4, #568]
.L428:
	movw	r2, #4860
	add	r3, r4, #4864
	ldr	r2, [r4, r2]
	str	r2, [r3]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L431
	mov	r3, r4
	mov	r2, #458
	ldr	r1, .L440+4
	ldr	r0, .L440+12
	bl	printk
.L431:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L422:
	movw	r2, #4840
	movs	r3, #0
	movs	r1, #1
	str	r3, [r4, r9]
	cmp	r8, #1000
	str	r1, [r4, r2]
	movw	r2, #4844
	add	r1, r4, #840
	str	r3, [r4, r2]
	add	r2, r2, r4
	mov	r7, r8
	it	ge
	movge	r7, #1000
.L424:
	cmp	r7, r3
	ldr	r9, [r2]
	bgt	.L425
	ldr	r1, [r4, #28]
	ldr	r0, [r4, #648]
	bl	__aeabi_idiv
	mov	r1, r7
	mov	r8, r0
	mov	r0, r9
	bl	__aeabi_idiv
	movw	r3, #4848
	sub	r0, r8, r0
	str	r0, [r4, r3]
	b	.L423
.L425:
	ldr	r0, [r1], #4
	adds	r3, r3, #1
	add	r0, r0, r9
	str	r0, [r2]
	b	.L424
.L426:
	ldrd	r3, r2, [r4, #560]
	mov	r1, r5
	ldr	r0, [r4, #588]
	adds	r3, r3, #1
	adc	r2, r2, #0
	strd	r3, r2, [r4, #560]
	ldr	r3, [r4, #532]
	add	r3, r3, r5
	str	r3, [r4, #532]
	bl	mpp_data_update
	mov	r1, r5
	ldr	r0, [r4, #592]
	bl	mpp_data_update
	ldr	r1, [r4, #652]
	add	r0, r4, #768
	subs	r1, r5, r1
	bl	mpp_pid_update
	ldr	r1, [r4, #656]
	add	r0, r4, #804
	subs	r1, r5, r1
	b	.L438
.L429:
	movs	r1, #6
	ldr	r7, .L440+16
	muls	r1, r2, r1
	lsls	r3, r3, #2
	smull	r10, fp, r3, r7
	asrs	r3, r3, #31
	smull	r8, r9, r1, r7
	asrs	r1, r1, #31
	rsb	r3, r3, fp, asr #2
	rsb	r1, r1, r9, asr #2
	add	r1, r1, r3
	b	.L439
.L441:
	.align	2
.L440:
	.word	rc_debug
	.word	.LANCHOR0+424
	.word	.LC58
	.word	.LC57
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
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r0
	ldr	r6, .L465
	mov	r5, r1
	ldr	r7, [r1, #184]
	ldr	r8, [r1, #192]
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L443
	add	r3, r1, #160
	movw	r2, #1283
	str	r3, [sp]
	mov	r3, r4
	ldr	r1, .L465+4
	ldr	r0, .L465+8
	bl	printk
.L443:
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L444
	ldr	r3, [r5, #208]
	movw	r2, #1284
	ldr	r1, .L465+4
	ldr	r0, .L465+12
	str	r3, [sp]
	ldr	r3, [r5, #204]
	bl	printk
.L444:
	ldr	r1, [r5, #204]
	ldr	r0, [r4, #600]
	bl	mpp_data_update_v2
	ldr	r1, [r5, #208]
	ldr	r0, [r4, #604]
	bl	mpp_data_update_v2
	movs	r3, #0
	str	r3, [r4, #368]
	movw	r3, #4860
	ldr	r1, [r4, r3]
	movw	r3, #4868
	str	r1, [r4, r3]
	ldr	r3, [r4, #356]
	cbz	r3, .L445
	ldr	r3, [r4, #516]
	cmp	r3, #1
	bne	.L446
.L445:
	ldr	r0, [r4, #576]
	bl	mpp_data_update
	ldr	r1, [r5, #196]
	ldr	r0, [r4, #580]
	bl	mpp_data_update
.L447:
	ldr	r2, [r5, #192]
	movw	r3, #4888
	str	r2, [r4, r3]
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L448
	mov	r3, r7
	movw	r2, #1301
	ldr	r1, .L465+4
	ldr	r0, .L465+16
	bl	printk
.L448:
	mov	r1, r7
	mov	r0, r4
	bl	bits_model_update_smt
	ldr	r3, [r5, #160]
	add	r2, r4, #4864
	strd	r3, r7, [r4, #416]
	movw	r3, #4860
	ldr	r3, [r4, r3]
	str	r3, [r2]
	ldr	r2, [r4, #428]
	str	r2, [r4, #432]
	ldr	r2, [r4, #356]
	str	r2, [r4, #360]
	movw	r2, #4900
	ldr	r1, [r5, #200]
	str	r1, [r4, r2]
	add	r1, r4, #4896
	ldr	r2, [r1]
	adds	r2, r2, #1
	str	r2, [r1]
	movw	r1, #4892
	ldr	r2, [r4, r1]
	add	r2, r2, r3
	str	r2, [r4, r1]
	str	r3, [r5, #300]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L449
	mov	r3, r4
	movw	r2, #1313
	ldr	r1, .L465+4
	ldr	r0, .L465+20
	bl	printk
.L449:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L446:
	movw	r3, #4876
	str	r1, [r4, r3]
	movw	r3, #4880
	ldr	r2, [r5, #196]
	str	r2, [r4, r3]
	movw	r3, #4884
	str	r8, [r4, r3]
	movw	r3, #4872
	str	r7, [r4, r3]
	b	.L447
.L466:
	.align	2
.L465:
	.word	rc_debug
	.word	.LANCHOR0+446
	.word	.LC61
	.word	.LC62
	.word	.LC63
	.word	.LC57
	.fnend
	.size	rc_model_v2_smt_end, .-rc_model_v2_smt_end
	.align	1
	.global	reenc_calc_cbr_ratio_smt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	reenc_calc_cbr_ratio_smt, %function
reenc_calc_cbr_ratio_smt:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r4, r0
	ldr	r5, [r0, #60]
	mov	r7, r1
	ldr	r0, [r0, #440]
	bl	mpp_data_sum_v2
	mov	r1, r5
	bl	__aeabi_idiv
	mov	r1, #-1
	mul	r9, r0, r5
	mov	r8, r0
	ldr	r0, [r4, #440]
	bl	mpp_data_get_pre_val_v2
	ldr	r6, [r7, #24]
	mov	r1, r5
	subs	r0, r6, r0
	add	r0, r0, r9
	bl	__aeabi_idiv
	ldr	r3, .L492
	ldr	fp, [r7]
	ldr	r7, [r4, #412]
	ldr	r2, [r3]
	mov	r10, r3
	tst	r2, #1
	mov	r9, r0
	beq	.L468
	mov	r3, r4
	movw	r2, #479
	ldr	r1, .L492+4
	ldr	r0, .L492+8
	bl	printk
.L468:
	ldr	r3, [r4, #356]
	ldr	r0, [r4, #448]
	ldr	r5, [r4, #364]
	str	r3, [sp]
	ldr	r3, [r4, #452]
	add	r3, r3, r6
	cmp	r3, r0
	it	le
	addle	r0, r0, r6
	subs	r5, r0, r5
	ldr	r0, [r4, #456]
	cmp	r5, r0
	it	lt
	movlt	r5, r0
	cmp	r6, fp
	sub	r0, r6, fp
	ite	lt
	movlt	r1, fp
	movge	r1, r6
	lsls	r0, r0, #5
	asrs	r6, r7, #5
	bl	__aeabi_idiv
	mov	r1, r6
	mov	fp, r0
	mov	r0, r9
	bl	__aeabi_idiv
	mov	r1, r6
	usat	r2, #6, r0
	mov	r0, r8
	str	r2, [sp, #4]
	bl	__aeabi_idiv
	ldr	r3, .L492+12
	mov	r1, r7
	ldr	r2, [sp, #4]
	usat	r0, #6, r0
	ldr	r2, [r3, r2, lsl #2]
	ldr	r3, [r3, r0, lsl #2]
	movs	r0, #96
	subs	r3, r2, r3
	sub	r2, r9, r7
	str	r3, [sp, #4]
	muls	r0, r2, r0
	bl	__aeabi_idiv
	ldr	r1, [r4, #436]
	lsrs	r1, r1, #3
	mov	r6, r0
	subs	r0, r5, r1
	lsls	r0, r0, #5
	bl	__aeabi_uidiv
	cmp	r8, r9
	ldr	r3, [sp, #4]
	bge	.L473
	cmp	r8, r7
	beq	.L473
	movs	r2, #6
	muls	r3, r2, r3
.L491:
	cmp	r3, #256
	mvn	r2, #191
	it	ge
	movge	r3, #256
	cmp	r3, r2
	it	lt
	movlt	r3, r2
.L474:
	cmn	fp, #128
	bge	.L475
	subs	r3, r3, #128
.L476:
	cmn	r6, #32
	bge	.L477
	sub	r6, r3, #32
.L478:
	cmn	r0, #32
	bge	.L479
	subs	r6, r6, #32
.L480:
	ldr	r3, [r10]
	str	r6, [r4, #464]
	lsls	r3, r3, #31
	bpl	.L481
	mov	r3, r4
	mov	r2, #544
	ldr	r1, .L492+4
	ldr	r0, .L492+16
	bl	printk
.L481:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L473:
	ldr	r2, [sp]
	cmp	r2, #2
	bne	.L482
	add	r3, r3, r3, lsl #1
	b	.L491
.L482:
	movs	r3, #0
	b	.L474
.L475:
	cmp	fp, #256
	ite	le
	addle	r3, r3, fp
	addgt	r3, r3, #256
	b	.L476
.L477:
	cmp	r6, #32
	ite	le
	addle	r6, r3, r6
	addgt	r6, r3, #32
	b	.L478
.L479:
	cmp	r0, #32
	ite	le
	addle	r6, r6, r0
	addgt	r6, r6, #32
	b	.L480
.L493:
	.align	2
.L492:
	.word	rc_debug
	.word	.LANCHOR0+466
	.word	.LC58
	.word	tab_lnx
	.word	.LC57
	.fnend
	.size	reenc_calc_cbr_ratio_smt, .-reenc_calc_cbr_ratio_smt
	.align	1
	.global	reenc_calc_vbr_ratio_smt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	reenc_calc_vbr_ratio_smt, %function
reenc_calc_vbr_ratio_smt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r0
	ldr	r4, [r0, #60]
	mov	r7, r1
	ldr	r0, [r0, #440]
	bl	mpp_data_sum_v2
	mov	r1, r4
	bl	__aeabi_idiv
	mov	r1, #-1
	mul	r6, r0, r4
	mov	fp, r0
	ldr	r0, [r5, #440]
	bl	mpp_data_get_pre_val_v2
	ldr	r8, [r7, #24]
	mov	r1, r4
	sub	r0, r8, r0
	add	r0, r0, r6
	bl	__aeabi_idiv
	ldr	r10, [r7]
	ldr	r7, .L513
	ldr	r9, [r5, #412]
	ldr	r4, [r5, #56]
	ldr	r3, [r7]
	lsls	r2, r3, #31
	mov	r6, r0
	bpl	.L495
	mov	r3, r5
	movw	r2, #563
	ldr	r1, .L513+4
	ldr	r0, .L513+8
	bl	printk
.L495:
	cmp	r8, r10
	sub	r0, r8, r10
	ite	ge
	movge	r1, r10
	movlt	r1, r8
	lsls	r0, r0, #5
	bl	__aeabi_idiv
	cmp	fp, r6
	mov	r10, r0
	bge	.L505
	cmp	r6, r9
	ble	.L505
	asr	r8, r4, #5
	mov	r0, r6
	mov	r1, r8
	bl	__aeabi_idiv
	mov	r1, r8
	usat	r4, #6, r0
	mov	r0, fp
	bl	__aeabi_idiv
	ldr	r3, .L513+12
	ldr	r4, [r3, r4, lsl #2]
	usat	r0, #6, r0
	ldr	r0, [r3, r0, lsl #2]
	mvn	r3, #191
	subs	r0, r4, r0
	movs	r4, #6
	muls	r4, r0, r4
	cmp	r4, #256
	it	ge
	movge	r4, #256
	cmp	r4, r3
	it	lt
	movlt	r4, r3
.L498:
	sub	r6, r6, r9
	movs	r0, #96
	mov	r1, r9
	muls	r0, r6, r0
	bl	__aeabi_idiv
	cmn	r10, #128
	bge	.L499
	subs	r4, r4, #128
.L500:
	cmn	r0, #32
	bge	.L501
	subs	r4, r4, #32
.L502:
	ldr	r3, [r7]
	str	r4, [r5, #464]
	lsls	r3, r3, #31
	bpl	.L503
	mov	r3, r5
	movw	r2, #598
	ldr	r1, .L513+4
	ldr	r0, .L513+16
	bl	printk
.L503:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L505:
	movs	r4, #0
	b	.L498
.L499:
	cmp	r10, #256
	ite	le
	addle	r4, r4, r10
	addgt	r4, r4, #256
	b	.L500
.L501:
	cmp	r0, #32
	ite	le
	addle	r4, r4, r0
	addgt	r4, r4, #32
	b	.L502
.L514:
	.align	2
.L513:
	.word	rc_debug
	.word	.LANCHOR0+491
	.word	.LC58
	.word	tab_lnx
	.word	.LC57
	.fnend
	.size	reenc_calc_vbr_ratio_smt, .-reenc_calc_vbr_ratio_smt
	.align	1
	.global	check_re_enc_smt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_re_enc_smt, %function
check_re_enc_smt:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r4, r0
	ldr	fp, [r0, #356]
	mov	r7, r1
	ldr	r8, [r0, #60]
	ldr	r0, [r0, #440]
	bl	mpp_data_sum_v2
	mov	r1, #-1
	ldr	r5, .L547
	mov	r9, r0
	ldr	r0, [r4, #440]
	bl	mpp_data_get_pre_val_v2
	ldr	r3, [r5]
	ldr	r10, [r7, #24]
	lsls	r2, r3, #31
	mov	r6, r0
	bpl	.L516
	mov	r3, r4
	movw	r2, #618
	ldr	r1, .L547+4
	ldr	r0, .L547+8
	bl	printk
.L516:
	ldrd	r1, r2, [r4, #52]
	ldr	r3, [r4, #8]
	ldr	r0, [r4, #496]
	cmp	r3, #1
	it	eq
	moveq	r2, r1
	ldr	r1, [r4, #152]
	cmp	r0, r1
	bge	.L539
	ldr	r1, [r4, #200]
	cbz	r1, .L519
	ldr	r1, [r4, #368]
	cmp	r1, #0
	beq	.L539
.L519:
	ldr	r1, [r7, #24]
	cmp	fp, #2
	ldr	r7, [r7]
	add	r7, r7, r7, lsl #1
	beq	.L520
	cmp	r1, r7
	ite	le
	movle	r7, #0
	movgt	r7, #1
.L523:
	mov	r1, r8
	mov	r0, r9
	strd	r3, r2, [sp]
	bl	__aeabi_idiv
	mov	r1, r8
	mov	r9, r0
	sub	r0, r10, r6
	mla	r0, r9, r8, r0
	bl	__aeabi_idiv
	ldr	r1, .L547+12
	ldr	r2, [sp, #4]
	sub	r6, r0, r9
	ldr	r3, [sp]
	smull	r8, r9, r2, r1
	asrs	r1, r2, #31
	rsb	ip, r1, r9, asr #3
	cmp	r6, ip
	ite	le
	movle	r6, #0
	movgt	r6, #1
	cmp	r3, #1
	bne	.L521
	asr	lr, r9, #2
	sub	ip, lr, r1
	add	ip, ip, r2
	cmp	ip, r0
	blt	.L522
	sub	r3, r1, lr
	add	r3, r3, r2
	cmp	r3, r0
	ite	le
	movle	r3, #0
	movgt	r3, #1
.L522:
	cbz	r7, .L539
	cbz	r6, .L539
	cbz	r3, .L539
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L539
	mov	r3, r4
	movw	r2, #651
	ldr	r1, .L547+4
	ldr	r0, .L547+16
	bl	printk
.L539:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L521:
	sub	r2, r2, r2, asr #3
	mov	r3, r6
	cmp	r2, r0
	ite	ge
	movge	r6, #0
	movlt	r6, #1
	b	.L522
.L520:
	add	r7, r7, r7, lsr #31
	cmp	r1, r7, asr #1
	ite	gt
	movgt	r7, #1
	movle	r7, #0
	b	.L523
.L548:
	.align	2
.L547:
	.word	rc_debug
	.word	.LANCHOR0+516
	.word	.LC58
	.word	1717986919
	.word	.LC57
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
	ldr	r6, .L568
	add	r7, r1, #160
	mov	r4, r0
	ldr	r3, [r6]
	lsls	r1, r3, #31
	bpl	.L550
	mov	r3, r0
	str	r7, [sp]
	movw	r2, #1253
	ldr	r1, .L568+4
	ldr	r0, .L568+8
	bl	printk
.L550:
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	bfc	r3, #2, #1
	strb	r3, [r5, #156]
	ldr	r3, [r4, #8]
	cmp	r3, #2
	beq	.L552
	ldr	r3, [r5, #264]
	lsls	r2, r3, #31
	bmi	.L552
	mov	r1, r7
	mov	r0, r4
	bl	check_re_enc_smt
	cbz	r0, .L555
	ldr	r3, [r4, #8]
	mov	r1, r7
	mov	r0, r4
	cmp	r3, #1
	bne	.L556
	bl	reenc_calc_cbr_ratio_smt
.L557:
	ldr	r3, [r4, #464]
	cbz	r3, .L555
	ldrd	r2, r3, [r5, #172]
	cmp	r2, r3
	bge	.L555
	ldr	r3, [r4, #496]
	adds	r3, r3, #1
	str	r3, [r4, #496]
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	orr	r3, r3, #4
	strb	r3, [r5, #156]
.L555:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L552
	mov	r3, r4
	mov	r2, #1272
	ldr	r1, .L568+4
	ldr	r0, .L568+12
	bl	printk
.L552:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L556:
	bl	reenc_calc_vbr_ratio_smt
	b	.L557
.L569:
	.align	2
.L568:
	.word	rc_debug
	.word	.LANCHOR0+533
	.word	.LC61
	.word	.LC57
	.fnend
	.size	rc_model_v2_smt_check_reenc, .-rc_model_v2_smt_check_reenc
	.global	smt_h265e
	.global	smt_h264e
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.25933, %object
	.size	__func__.25933, 26
__func__.25933:
	.ascii	"rc_model_v2_smt_hal_start\000"
	.type	__func__.25938, %object
	.size	__func__.25938, 24
__func__.25938:
	.ascii	"rc_model_v2_smt_hal_end\000"
	.space	2
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
	.type	__func__.25750, %object
	.size	__func__.25750, 21
__func__.25750:
	.ascii	"calc_smt_debreath_qp\000"
	.type	strength_map.25749, %object
	.size	strength_map.25749, 36
strength_map.25749:
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
	.type	intra_qp_map.25747, %object
	.size	intra_qp_map.25747, 8
intra_qp_map.25747:
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.type	__func__.25861, %object
	.size	__func__.25861, 22
__func__.25861:
	.ascii	"rc_model_v2_smt_start\000"
	.type	__func__.25626, %object
	.size	__func__.25626, 22
__func__.25626:
	.ascii	"bits_model_smt_deinit\000"
	.type	__func__.25720, %object
	.size	__func__.25720, 23
__func__.25720:
	.ascii	"rc_model_v2_smt_deinit\000"
	.type	__func__.25638, %object
	.size	__func__.25638, 20
__func__.25638:
	.ascii	"bits_model_smt_init\000"
	.type	__func__.25709, %object
	.size	__func__.25709, 26
__func__.25709:
	.ascii	"rc_model_v2_smt_h265_init\000"
	.type	__func__.25715, %object
	.size	__func__.25715, 26
__func__.25715:
	.ascii	"rc_model_v2_smt_h264_init\000"
	.type	__func__.25647, %object
	.size	__func__.25647, 22
__func__.25647:
	.ascii	"bits_model_update_smt\000"
	.type	__func__.25928, %object
	.size	__func__.25928, 20
__func__.25928:
	.ascii	"rc_model_v2_smt_end\000"
	.type	__func__.25671, %object
	.size	__func__.25671, 25
__func__.25671:
	.ascii	"reenc_calc_cbr_ratio_smt\000"
	.type	__func__.25688, %object
	.size	__func__.25688, 25
__func__.25688:
	.ascii	"reenc_calc_vbr_ratio_smt\000"
	.type	__func__.25703, %object
	.size	__func__.25703, 17
__func__.25703:
	.ascii	"check_re_enc_smt\000"
	.type	__func__.25919, %object
	.size	__func__.25919, 28
__func__.25919:
	.ascii	"rc_model_v2_smt_check_reenc\000"
	.space	3
	.type	smt_h265e, %object
	.size	smt_h265e, 48
smt_h265e:
	.word	.LC16
	.word	16777220
	.word	4904
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
	.word	.LC16
	.word	7
	.word	4904
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
	.ascii	"\012---------RC base param 1-----------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC2:
	.ascii	"Gop\000"
.LC3:
	.ascii	"ChnId\000"
.LC4:
	.ascii	"%7s%7s%8s%6s%6s%8s%13s%13s%5s%5s \012\000"
.LC5:
	.ascii	"PQp\000"
.LC6:
	.ascii	"IQp\000"
.LC7:
	.ascii	"MaxBr(kbps)\000"
.LC8:
	.ascii	"MinBr(kbps)\000"
.LC9:
	.ascii	"RcMode\000"
.LC10:
	.ascii	"TrgFr\000"
.LC11:
	.ascii	"ViFr\000"
.LC12:
	.ascii	"StatTm\000"
.LC13:
	.ascii	"%7d%7u%8u%6u%6u%8s%13s%13s%5u%5u \012\000"
.LC14:
	.ascii	"N/A\000"
.LC15:
	.ascii	"%7d%7u%8u%6u%6u%8s%13u%13u%5s%5s \012\000"
.LC16:
	.ascii	"smart\000"
.LC17:
	.ascii	"\012---------RC gop mode attr----------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC18:
	.ascii	"GopMode\000"
.LC19:
	.ascii	"%7s%10s%10s%12s%10s \012\000"
.LC20:
	.ascii	"ViQpDelta\000"
.LC21:
	.ascii	"BgInterval\000"
.LC22:
	.ascii	"IpQpDelta\000"
.LC23:
	.ascii	"%7d%10s%10d%12u%10d\012\000"
.LC24:
	.ascii	"%7d%10s%10d%12s%10s\012\000"
.LC25:
	.ascii	"\012---------RC run smart common param-------------"
	.ascii	"---------------------------------------------------"
	.ascii	"--\012\000"
.LC26:
	.ascii	"MaxQp\000"
.LC27:
	.ascii	"%7s%8s%8s%8s%8s%10s%10s%10s%10s%15s \012\000"
.LC28:
	.ascii	"MaxReEncTimes\000"
.LC29:
	.ascii	"FrmMinIQp\000"
.LC30:
	.ascii	"FrmMaxIQp\000"
.LC31:
	.ascii	"FrmMinQp\000"
.LC32:
	.ascii	"FrmMaxQp\000"
.LC33:
	.ascii	"MinIQp\000"
.LC34:
	.ascii	"MaxIQp\000"
.LC35:
	.ascii	"MinQp\000"
.LC36:
	.ascii	"%7d%8u%8u%8u%8u%10d%10d%10d%10d%15d\012\000"
.LC37:
	.ascii	"\012--------RC HierarchicalQp INFO-----------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC38:
	.ascii	"bEnable\000"
.LC39:
	.ascii	"%7s%10s%12s%12s%12s%12s%12s%12s%12s%12s\012\000"
.LC40:
	.ascii	"QpDelta[3]\000"
.LC41:
	.ascii	"QpDelta[2]\000"
.LC42:
	.ascii	"QpDelta[1]\000"
.LC43:
	.ascii	"QpDelta[0]\000"
.LC44:
	.ascii	"FrameNum[3]\000"
.LC45:
	.ascii	"FrameNum[2]\000"
.LC46:
	.ascii	"FrameNum[1]\000"
.LC47:
	.ascii	"FrameNum[0]\000"
.LC48:
	.ascii	"%7d%10s%12d%12d%12d%12d%12d%12d%12d%12d\012\000"
.LC49:
	.ascii	"\012--------RC debreath_effect info----------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC50:
	.ascii	"%7s%10s%10s%18s\012\000"
.LC51:
	.ascii	"DeBrthEfctCnt\000"
.LC52:
	.ascii	"Strength0\000"
.LC53:
	.ascii	"%7d%10s%10d%18u\012\000"
.LC54:
	.ascii	"%7d%10s%10s%18u\012\000"
.LC55:
	.ascii	"\0016%s:%d: smt_hal_start enter ctx %p task %p\012\000"
.LC56:
	.ascii	"\0016%s:%d: smt_hal_end enter ctx %p task %p\012\000"
.LC57:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC58:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC59:
	.ascii	"\0016%s:%d: i qp_out %d, qp_start_sum = %d, intra_l"
	.ascii	"v4_prop %d\000"
.LC60:
	.ascii	"\0016%s:%d: pre motion_level %u, sum %u, complex_le"
	.ascii	"vel %u, sum %u\012\000"
.LC61:
	.ascii	"\0016%s:%d: enter ctx %p cfg %p\012\000"
.LC62:
	.ascii	"\0016%s:%d: motion_level %u, complex_level %u\012\000"
.LC63:
	.ascii	"\0016%s:%d: bits_mode_update real_bit %d\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
