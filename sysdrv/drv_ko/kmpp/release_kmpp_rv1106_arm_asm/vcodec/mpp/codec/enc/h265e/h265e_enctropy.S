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
	.file	"h265e_enctropy.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	sbacInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sbacInit, %function
sbacInit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	asrs	r3, r1, #4
	lsls	r1, r1, #3
	and	r1, r1, #120
	add	r3, r3, r3, lsl #2
	subs	r1, r1, #16
	subs	r3, r3, #45
	muls	r0, r3, r0
	add	r0, r1, r0, asr #4
	cmp	r0, #126
	bgt	.L5
	cmp	r0, #1
	mov	r3, r0
	it	lt
	movlt	r3, #1
	cmp	r0, #63
	ble	.L3
.L2:
	sub	r0, r3, #64
	movs	r3, #1
	lsls	r0, r0, #1
	uxtb	r0, r0
.L4:
	add	r0, r0, r3
	uxtb	r0, r0
	bx	lr
.L5:
	movs	r3, #126
	b	.L2
.L3:
	rsb	r0, r3, #63
	movs	r3, #0
	lsls	r0, r0, #1
	uxtb	r0, r0
	b	.L4
	.fnend
	.size	sbacInit, .-sbacInit
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	initBuffer, %function
initBuffer:
	.fnstart
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r7, r3
	ldr	r3, .L18
	mov	r8, r2
	mov	r4, r0
	mov	r5, r1
	ldr	r6, [sp, #28]
	ldr	r3, [r3]
	lsls	r2, r3, #20
	bpl	.L7
	mov	r3, r1
	movs	r2, #164
	ldr	r1, .L18+4
	ldr	r0, .L18+8
	bl	printk
.L7:
	cmp	r5, #2
	beq	.L11
	cmp	r5, #1
	bne	.L9
	cmp	r8, #0
	ite	eq
	moveq	r3, #1
	movne	r3, #2
.L8:
	ldr	r5, [sp, #24]
	rsb	r3, r3, #2
	mov	r2, r4
	mla	r5, r6, r3, r5
	add	r6, r4, r6, lsl #1
	movs	r4, #0
.L10:
	ldrb	r1, [r5], #1	@ zero_extendqisi2
	mov	r0, r7
	bl	sbacInit
	strb	r0, [r2]
	adds	r2, r2, #2
	strb	r4, [r2, #-1]
	cmp	r6, r2
	bne	.L10
	pop	{r4, r5, r6, r7, r8, pc}
.L9:
	cmp	r8, #0
	ite	eq
	moveq	r3, #2
	movne	r3, #1
	b	.L8
.L11:
	movs	r3, #0
	b	.L8
.L19:
	.align	2
.L18:
	.word	h265e_debug
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	initBuffer, .-initBuffer
	.align	1
	.global	h265e_reset_enctropy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_reset_enctropy, %function
h265e_reset_enctropy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L26
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r4, r0
	ldr	r2, [r3]
	mov	r10, r3
	ldrb	r8, [r0, #13]	@ zero_extendqisi2
	tst	r2, #1
	ldr	r9, [r0, #28]
	ldr	r7, [r0, #368]
	beq	.L21
	movs	r2, #188
	ldr	r1, .L26+4
	ldr	r0, .L26+8
	bl	printk
.L21:
	ldr	r5, .L26+12
	movs	r6, #3
	mov	r3, r9
	str	r6, [sp, #4]
	mov	r2, r8
	mov	r1, r7
	add	r0, r4, #1464
	str	r5, [sp]
	bl	initBuffer
	add	r3, r5, #9
	mov	r2, r8
	mov	r1, r7
	addw	r0, r4, #1470
	str	r6, [sp, #4]
	movs	r6, #1
	str	r3, [sp]
	mov	r3, r9
	bl	initBuffer
	add	r3, r5, #18
	mov	r2, r8
	mov	r1, r7
	addw	r0, r4, #1476
	str	r3, [sp]
	mov	r3, r9
	str	r6, [sp, #4]
	bl	initBuffer
	add	r3, r5, #21
	mov	r2, r8
	mov	r1, r7
	addw	r0, r4, #1478
	str	r3, [sp]
	mov	r3, r9
	str	r6, [sp, #4]
	bl	initBuffer
	ldr	r3, [r10]
	lsls	r3, r3, #31
	bpl	.L20
	movs	r2, #201
	sub	r1, r5, #21
	ldr	r0, .L26+16
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	b	printk
.L20:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L27:
	.align	2
.L26:
	.word	h265e_debug
	.word	.LANCHOR0+11
	.word	.LC1
	.word	.LANCHOR0+32
	.word	.LC2
	.fnend
	.size	h265e_reset_enctropy, .-h265e_reset_enctropy
	.align	1
	.global	h265e_cabac_resetBits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_cabac_resetBits, %function
h265e_cabac_resetBits:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r5, .L34
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L29
	movs	r2, #204
	ldr	r1, .L34+4
	ldr	r0, .L34+8
	bl	printk
.L29:
	movs	r3, #0
	mvn	r2, #11
	str	r3, [r4, #4]
	strd	r3, r2, [r4, #16]
	movs	r3, #255
	movs	r2, #0
	str	r3, [r4, #12]
	movs	r3, #0
	strd	r2, [r4, #24]
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L28
	movs	r2, #210
	ldr	r1, .L34+4
	ldr	r0, .L34+12
	pop	{r3, r4, r5, lr}
	b	printk
.L28:
	pop	{r3, r4, r5, pc}
.L35:
	.align	2
.L34:
	.word	h265e_debug
	.word	.LANCHOR0+56
	.word	.LC1
	.word	.LC2
	.fnend
	.size	h265e_cabac_resetBits, .-h265e_cabac_resetBits
	.align	1
	.global	h265e_cabac_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_cabac_start, %function
h265e_cabac_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L41
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L37
	movs	r2, #213
	ldr	r1, .L41+4
	ldr	r0, .L41+8
	bl	printk
.L37:
	movs	r3, #0
	mov	r2, #510
	strd	r3, r2, [r4, #4]
	mvn	r2, #11
	strd	r3, r2, [r4, #16]
	movs	r3, #255
	str	r3, [r4, #12]
	pop	{r4, pc}
.L42:
	.align	2
.L41:
	.word	h265e_debug
	.word	.LANCHOR0+78
	.word	.LC1
	.fnend
	.size	h265e_cabac_start, .-h265e_cabac_start
	.align	1
	.global	h265e_cabac_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_cabac_init, %function
h265e_cabac_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, .L49
	mov	r6, r1
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L44
	movs	r2, #221
	ldr	r1, .L49+4
	ldr	r0, .L49+8
	bl	printk
.L44:
	str	r6, [r5]
	mov	r0, r5
	bl	h265e_cabac_start
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L43
	movs	r2, #224
	ldr	r1, .L49+4
	ldr	r0, .L49+12
	pop	{r4, r5, r6, lr}
	b	printk
.L43:
	pop	{r4, r5, r6, pc}
.L50:
	.align	2
.L49:
	.word	h265e_debug
	.word	.LANCHOR0+96
	.word	.LC1
	.word	.LC2
	.fnend
	.size	h265e_cabac_init, .-h265e_cabac_init
	.align	1
	.global	h265e_cabac_writeOut
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_cabac_writeOut, %function
h265e_cabac_writeOut:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r3, [r0, #20]
	ldr	r6, [r0, #4]
	add	r2, r3, #13
	rsb	r3, r3, #19
	ldr	r9, [r0]
	lsrs	r6, r6, r2
	mov	r2, #-1
	lsr	r5, r2, r3
	ldr	r3, .L71
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #1
	beq	.L52
	movs	r2, #230
	ldr	r1, .L71+4
	ldr	r0, .L71+8
	bl	printk
.L52:
	ldr	r3, [r4, #20]
	cmp	r6, #255
	ldr	r2, [r4, #4]
	sub	r3, r3, #8
	str	r3, [r4, #20]
	and	r2, r2, r5
	ldr	r5, [r4, #16]
	str	r2, [r4, #4]
	bne	.L53
	adds	r5, r5, #1
	str	r5, [r4, #16]
.L54:
	ldr	r3, [r8]
	lsls	r3, r3, #31
	bpl	.L51
	movs	r2, #254
	ldr	r1, .L71+4
	ldr	r0, .L71+12
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	printk
.L53:
	cbnz	r5, .L55
.L60:
	movs	r3, #1
	uxtb	r6, r6
	str	r3, [r4, #16]
	str	r6, [r4, #12]
	b	.L54
.L55:
	ldr	r3, [r4, #12]
	lsrs	r7, r6, #8
	movs	r2, #8
	mov	r0, r9
	add	r10, r7, r3
	mov	r1, r10
	bl	mpp_writer_put_bits
	ldr	r3, [r8]
	lsls	r1, r3, #20
	bpl	.L56
	mov	r3, r10
	movs	r2, #242
	ldr	r1, .L71+4
	ldr	r0, .L71+16
	bl	printk
.L56:
	adds	r7, r7, #255
	ldr	r10, .L71+4
	ldr	fp, .L71+16
	uxtb	r7, r7
.L57:
	cmp	r5, #1
	ldr	r3, [r8]
	beq	.L60
	lsls	r2, r3, #20
	bpl	.L58
	mov	r3, r7
	movs	r2, #246
	mov	r1, r10
	mov	r0, fp
	bl	printk
.L58:
	movs	r2, #8
	mov	r1, r7
	mov	r0, r9
	subs	r5, r5, #1
	bl	mpp_writer_put_bits
	b	.L57
.L51:
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L72:
	.align	2
.L71:
	.word	h265e_debug
	.word	.LANCHOR0+113
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.fnend
	.size	h265e_cabac_writeOut, .-h265e_cabac_writeOut
	.align	1
	.global	h265e_cabac_encodeBin
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_cabac_encodeBin, %function
h265e_cabac_encodeBin:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r5, .L84
	mov	r8, r1
	mov	r7, r2
	ldrb	r6, [r1]	@ zero_extendqisi2
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L74
	movw	r2, #273
	ldr	r1, .L84+4
	ldr	r0, .L84+8
	bl	printk
.L74:
	ldr	r3, .L84+12
	add	r2, r3, r6, lsl #1
	add	r2, r2, r7
	ldrb	r2, [r2, #156]	@ zero_extendqisi2
	strb	r2, [r8]
	movs	r2, #1
	strb	r2, [r8, #1]
	lsr	ip, r6, r2
	ldr	r0, [r4, #8]
	eors	r6, r6, r7
	add	r3, r3, ip, lsl #2
	lsls	r6, r6, #31
	ubfx	r2, r0, #6, #2
	add	r3, r3, r2
	ldr	r2, [r4, #4]
	ldrb	r1, [r3, #412]	@ zero_extendqisi2
	sub	r0, r0, r1
	bmi	.L75
	sub	r3, r0, #256
	mov	r1, r0
	lsrs	r3, r3, #31
.L76:
	lsls	r2, r2, r3
	str	r2, [r4, #4]
	ldr	r2, [r4, #20]
	lsls	r1, r1, r3
	str	r1, [r4, #8]
	add	r3, r3, r2
	str	r3, [r4, #20]
	cmp	r3, #0
	blt	.L78
	mov	r0, r4
	bl	h265e_cabac_writeOut
.L78:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L73
	mov	r2, #310
	ldr	r1, .L84+4
	ldr	r0, .L84+16
	pop	{r4, r5, r6, r7, r8, lr}
	b	printk
.L75:
	cmp	ip, #62
	add	r2, r2, r0
	ittet	ls
	clzls	r3, r1
	eorls	r3, r3, #31
	movhi	r3, #6
	rsbls	r3, r3, #8
	b	.L76
.L73:
	pop	{r4, r5, r6, r7, r8, pc}
.L85:
	.align	2
.L84:
	.word	h265e_debug
	.word	.LANCHOR0+134
	.word	.LC1
	.word	.LANCHOR0
	.word	.LC2
	.fnend
	.size	h265e_cabac_encodeBin, .-h265e_cabac_encodeBin
	.align	1
	.global	h265e_cabac_encodeBinTrm
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_cabac_encodeBinTrm, %function
h265e_cabac_encodeBinTrm:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L101
	mov	r6, r1
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L87
	movw	r2, #315
	ldr	r1, .L101+4
	ldr	r0, .L101+8
	bl	printk
.L87:
	ldr	r3, [r4, #8]
	subs	r3, r3, #2
	str	r3, [r4, #8]
	cbz	r6, .L88
	ldr	r2, [r4, #4]
	add	r3, r3, r2
	lsls	r3, r3, #7
	str	r3, [r4, #4]
	mov	r3, #256
	str	r3, [r4, #8]
	ldr	r3, [r4, #20]
	adds	r3, r3, #7
.L100:
	str	r3, [r4, #20]
	ldr	r3, [r4, #20]
	cmp	r3, #0
	blt	.L92
	mov	r0, r4
	bl	h265e_cabac_writeOut
.L92:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L86
	mov	r2, #334
	ldr	r1, .L101+4
	ldr	r0, .L101+12
	pop	{r4, r5, r6, lr}
	b	printk
.L88:
	cmp	r3, #255
	bhi	.L86
	ldr	r2, [r4, #4]
	lsls	r3, r3, #1
	str	r3, [r4, #8]
	ldr	r3, [r4, #20]
	lsls	r2, r2, #1
	str	r2, [r4, #4]
	adds	r3, r3, #1
	b	.L100
.L86:
	pop	{r4, r5, r6, pc}
.L102:
	.align	2
.L101:
	.word	h265e_debug
	.word	.LANCHOR0+668
	.word	.LC1
	.word	.LC2
	.fnend
	.size	h265e_cabac_encodeBinTrm, .-h265e_cabac_encodeBinTrm
	.align	1
	.global	h265e_cabac_finish
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_cabac_finish, %function
h265e_cabac_finish:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r6, .L116
	ldr	r5, [r0]
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L104
	mov	r2, #340
	ldr	r1, .L116+4
	ldr	r0, .L116+8
	bl	printk
.L104:
	ldr	r3, [r4, #20]
	ldr	r2, [r4, #4]
	adds	r3, r3, #21
	lsrs	r3, r2, r3
	beq	.L105
	ldr	r1, [r4, #12]
	movs	r2, #8
	mov	r0, r5
	adds	r1, r1, #1
	bl	mpp_writer_put_bits
.L106:
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bgt	.L107
	ldr	r3, [r4, #20]
	add	r2, r3, #21
	movs	r3, #1
	lsl	r2, r3, r2
	ldr	r3, [r4, #4]
	subs	r3, r3, r2
	str	r3, [r4, #4]
.L108:
	ldr	r1, [r4, #4]
	mov	r0, r5
	ldr	r2, [r4, #20]
	lsrs	r1, r1, #8
	adds	r2, r2, #13
	bl	mpp_writer_put_bits
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L103
	mov	r2, #358
	ldr	r1, .L116+4
	ldr	r0, .L116+12
	pop	{r4, r5, r6, lr}
	b	printk
.L107:
	movs	r2, #8
	movs	r1, #0
	mov	r0, r5
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #16]
	subs	r3, r3, #1
	str	r3, [r4, #16]
	b	.L106
.L105:
	ldr	r3, [r4, #16]
	cmp	r3, #0
	ble	.L110
	movs	r2, #8
	ldr	r1, [r4, #12]
	mov	r0, r5
	bl	mpp_writer_put_bits
.L110:
	ldr	r3, [r4, #16]
	cmp	r3, #1
	ble	.L108
	movs	r2, #8
	movs	r1, #255
	mov	r0, r5
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #16]
	subs	r3, r3, #1
	str	r3, [r4, #16]
	b	.L110
.L103:
	pop	{r4, r5, r6, pc}
.L117:
	.align	2
.L116:
	.word	h265e_debug
	.word	.LANCHOR0+693
	.word	.LC1
	.word	.LC2
	.fnend
	.size	h265e_cabac_finish, .-h265e_cabac_finish
	.align	1
	.global	h265e_cabac_flush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_cabac_flush, %function
h265e_cabac_flush:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, .L124
	ldr	r6, [r0]
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L119
	mov	r2, #364
	ldr	r1, .L124+4
	ldr	r0, .L124+8
	bl	printk
.L119:
	movs	r1, #1
	mov	r0, r5
	bl	h265e_cabac_encodeBinTrm
	mov	r0, r5
	bl	h265e_cabac_finish
	movs	r2, #1
	mov	r1, r2
	mov	r0, r6
	bl	mpp_writer_put_bits
	mov	r0, r6
	bl	mpp_writer_align_zero
	mov	r0, r5
	bl	h265e_cabac_start
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L118
	mov	r2, #370
	ldr	r1, .L124+4
	ldr	r0, .L124+12
	pop	{r4, r5, r6, lr}
	b	printk
.L118:
	pop	{r4, r5, r6, pc}
.L125:
	.align	2
.L124:
	.word	h265e_debug
	.word	.LANCHOR0+712
	.word	.LC1
	.word	.LC2
	.fnend
	.size	h265e_cabac_flush, .-h265e_cabac_flush
	.global	g_nextState
	.global	g_lpsTable
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.9259, %object
	.size	__func__.9259, 11
__func__.9259:
	.ascii	"initBuffer\000"
	.type	__func__.9270, %object
	.size	__func__.9270, 21
__func__.9270:
	.ascii	"h265e_reset_enctropy\000"
	.type	INIT_SPLIT_FLAG, %object
	.size	INIT_SPLIT_FLAG, 9
INIT_SPLIT_FLAG:
	.byte	107
	.byte	-117
	.byte	126
	.byte	107
	.byte	-117
	.byte	126
	.byte	-117
	.byte	-115
	.byte	-99
	.type	INIT_SKIP_FLAG, %object
	.size	INIT_SKIP_FLAG, 9
INIT_SKIP_FLAG:
	.byte	-59
	.byte	-71
	.byte	-55
	.byte	-59
	.byte	-71
	.byte	-55
	.byte	-102
	.byte	-102
	.byte	-102
	.type	INIT_MERGE_FLAG_EXT, %object
	.size	INIT_MERGE_FLAG_EXT, 3
INIT_MERGE_FLAG_EXT:
	.byte	-102
	.byte	110
	.byte	-102
	.type	INIT_MERGE_IDX_EXT, %object
	.size	INIT_MERGE_IDX_EXT, 3
INIT_MERGE_IDX_EXT:
	.byte	-119
	.byte	122
	.byte	-102
	.type	__func__.9274, %object
	.size	__func__.9274, 22
__func__.9274:
	.ascii	"h265e_cabac_resetBits\000"
	.type	__func__.9278, %object
	.size	__func__.9278, 18
__func__.9278:
	.ascii	"h265e_cabac_start\000"
	.type	__func__.9283, %object
	.size	__func__.9283, 17
__func__.9283:
	.ascii	"h265e_cabac_init\000"
	.type	__func__.9290, %object
	.size	__func__.9290, 21
__func__.9290:
	.ascii	"h265e_cabac_writeOut\000"
	.type	__func__.9308, %object
	.size	__func__.9308, 22
__func__.9308:
	.ascii	"h265e_cabac_encodeBin\000"
	.type	g_nextState, %object
	.size	g_nextState, 256
g_nextState:
	.byte	2
	.byte	1
	.byte	0
	.byte	3
	.byte	4
	.byte	0
	.byte	1
	.byte	5
	.byte	6
	.byte	2
	.byte	3
	.byte	7
	.byte	8
	.byte	4
	.byte	5
	.byte	9
	.byte	10
	.byte	4
	.byte	5
	.byte	11
	.byte	12
	.byte	8
	.byte	9
	.byte	13
	.byte	14
	.byte	8
	.byte	9
	.byte	15
	.byte	16
	.byte	10
	.byte	11
	.byte	17
	.byte	18
	.byte	12
	.byte	13
	.byte	19
	.byte	20
	.byte	14
	.byte	15
	.byte	21
	.byte	22
	.byte	16
	.byte	17
	.byte	23
	.byte	24
	.byte	18
	.byte	19
	.byte	25
	.byte	26
	.byte	18
	.byte	19
	.byte	27
	.byte	28
	.byte	22
	.byte	23
	.byte	29
	.byte	30
	.byte	22
	.byte	23
	.byte	31
	.byte	32
	.byte	24
	.byte	25
	.byte	33
	.byte	34
	.byte	26
	.byte	27
	.byte	35
	.byte	36
	.byte	26
	.byte	27
	.byte	37
	.byte	38
	.byte	30
	.byte	31
	.byte	39
	.byte	40
	.byte	30
	.byte	31
	.byte	41
	.byte	42
	.byte	32
	.byte	33
	.byte	43
	.byte	44
	.byte	32
	.byte	33
	.byte	45
	.byte	46
	.byte	36
	.byte	37
	.byte	47
	.byte	48
	.byte	36
	.byte	37
	.byte	49
	.byte	50
	.byte	38
	.byte	39
	.byte	51
	.byte	52
	.byte	38
	.byte	39
	.byte	53
	.byte	54
	.byte	42
	.byte	43
	.byte	55
	.byte	56
	.byte	42
	.byte	43
	.byte	57
	.byte	58
	.byte	44
	.byte	45
	.byte	59
	.byte	60
	.byte	44
	.byte	45
	.byte	61
	.byte	62
	.byte	46
	.byte	47
	.byte	63
	.byte	64
	.byte	48
	.byte	49
	.byte	65
	.byte	66
	.byte	48
	.byte	49
	.byte	67
	.byte	68
	.byte	50
	.byte	51
	.byte	69
	.byte	70
	.byte	52
	.byte	53
	.byte	71
	.byte	72
	.byte	52
	.byte	53
	.byte	73
	.byte	74
	.byte	54
	.byte	55
	.byte	75
	.byte	76
	.byte	54
	.byte	55
	.byte	77
	.byte	78
	.byte	56
	.byte	57
	.byte	79
	.byte	80
	.byte	58
	.byte	59
	.byte	81
	.byte	82
	.byte	58
	.byte	59
	.byte	83
	.byte	84
	.byte	60
	.byte	61
	.byte	85
	.byte	86
	.byte	60
	.byte	61
	.byte	87
	.byte	88
	.byte	60
	.byte	61
	.byte	89
	.byte	90
	.byte	62
	.byte	63
	.byte	91
	.byte	92
	.byte	64
	.byte	65
	.byte	93
	.byte	94
	.byte	64
	.byte	65
	.byte	95
	.byte	96
	.byte	66
	.byte	67
	.byte	97
	.byte	98
	.byte	66
	.byte	67
	.byte	99
	.byte	100
	.byte	66
	.byte	67
	.byte	101
	.byte	102
	.byte	68
	.byte	69
	.byte	103
	.byte	104
	.byte	68
	.byte	69
	.byte	105
	.byte	106
	.byte	70
	.byte	71
	.byte	107
	.byte	108
	.byte	70
	.byte	71
	.byte	109
	.byte	110
	.byte	70
	.byte	71
	.byte	111
	.byte	112
	.byte	72
	.byte	73
	.byte	113
	.byte	114
	.byte	72
	.byte	73
	.byte	115
	.byte	116
	.byte	72
	.byte	73
	.byte	117
	.byte	118
	.byte	74
	.byte	75
	.byte	119
	.byte	120
	.byte	74
	.byte	75
	.byte	121
	.byte	122
	.byte	74
	.byte	75
	.byte	123
	.byte	124
	.byte	76
	.byte	77
	.byte	125
	.byte	124
	.byte	76
	.byte	77
	.byte	125
	.byte	126
	.byte	126
	.byte	127
	.byte	127
	.type	g_lpsTable, %object
	.size	g_lpsTable, 256
g_lpsTable:
	.byte	-128
	.byte	-80
	.byte	-48
	.byte	-16
	.byte	-128
	.byte	-89
	.byte	-59
	.byte	-29
	.byte	-128
	.byte	-98
	.byte	-69
	.byte	-40
	.byte	123
	.byte	-106
	.byte	-78
	.byte	-51
	.byte	116
	.byte	-114
	.byte	-87
	.byte	-61
	.byte	111
	.byte	-121
	.byte	-96
	.byte	-71
	.byte	105
	.byte	-128
	.byte	-104
	.byte	-81
	.byte	100
	.byte	122
	.byte	-112
	.byte	-90
	.byte	95
	.byte	116
	.byte	-119
	.byte	-98
	.byte	90
	.byte	110
	.byte	-126
	.byte	-106
	.byte	85
	.byte	104
	.byte	123
	.byte	-114
	.byte	81
	.byte	99
	.byte	117
	.byte	-121
	.byte	77
	.byte	94
	.byte	111
	.byte	-128
	.byte	73
	.byte	89
	.byte	105
	.byte	122
	.byte	69
	.byte	85
	.byte	100
	.byte	116
	.byte	66
	.byte	80
	.byte	95
	.byte	110
	.byte	62
	.byte	76
	.byte	90
	.byte	104
	.byte	59
	.byte	72
	.byte	86
	.byte	99
	.byte	56
	.byte	69
	.byte	81
	.byte	94
	.byte	53
	.byte	65
	.byte	77
	.byte	89
	.byte	51
	.byte	62
	.byte	73
	.byte	85
	.byte	48
	.byte	59
	.byte	69
	.byte	80
	.byte	46
	.byte	56
	.byte	66
	.byte	76
	.byte	43
	.byte	53
	.byte	63
	.byte	72
	.byte	41
	.byte	50
	.byte	59
	.byte	69
	.byte	39
	.byte	48
	.byte	56
	.byte	65
	.byte	37
	.byte	45
	.byte	54
	.byte	62
	.byte	35
	.byte	43
	.byte	51
	.byte	59
	.byte	33
	.byte	41
	.byte	48
	.byte	56
	.byte	32
	.byte	39
	.byte	46
	.byte	53
	.byte	30
	.byte	37
	.byte	43
	.byte	50
	.byte	29
	.byte	35
	.byte	41
	.byte	48
	.byte	27
	.byte	33
	.byte	39
	.byte	45
	.byte	26
	.byte	31
	.byte	37
	.byte	43
	.byte	24
	.byte	30
	.byte	35
	.byte	41
	.byte	23
	.byte	28
	.byte	33
	.byte	39
	.byte	22
	.byte	27
	.byte	32
	.byte	37
	.byte	21
	.byte	26
	.byte	30
	.byte	35
	.byte	20
	.byte	24
	.byte	29
	.byte	33
	.byte	19
	.byte	23
	.byte	27
	.byte	31
	.byte	18
	.byte	22
	.byte	26
	.byte	30
	.byte	17
	.byte	21
	.byte	25
	.byte	28
	.byte	16
	.byte	20
	.byte	23
	.byte	27
	.byte	15
	.byte	19
	.byte	22
	.byte	25
	.byte	14
	.byte	18
	.byte	21
	.byte	24
	.byte	14
	.byte	17
	.byte	20
	.byte	23
	.byte	13
	.byte	16
	.byte	19
	.byte	22
	.byte	12
	.byte	15
	.byte	18
	.byte	21
	.byte	12
	.byte	14
	.byte	17
	.byte	20
	.byte	11
	.byte	14
	.byte	16
	.byte	19
	.byte	11
	.byte	13
	.byte	15
	.byte	18
	.byte	10
	.byte	12
	.byte	15
	.byte	17
	.byte	10
	.byte	12
	.byte	14
	.byte	16
	.byte	9
	.byte	11
	.byte	13
	.byte	15
	.byte	9
	.byte	11
	.byte	12
	.byte	14
	.byte	8
	.byte	10
	.byte	12
	.byte	14
	.byte	8
	.byte	9
	.byte	11
	.byte	13
	.byte	7
	.byte	9
	.byte	11
	.byte	12
	.byte	7
	.byte	9
	.byte	10
	.byte	12
	.byte	7
	.byte	8
	.byte	10
	.byte	11
	.byte	6
	.byte	8
	.byte	9
	.byte	11
	.byte	6
	.byte	7
	.byte	9
	.byte	10
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.type	__func__.9314, %object
	.size	__func__.9314, 25
__func__.9314:
	.ascii	"h265e_cabac_encodeBinTrm\000"
	.type	__func__.9319, %object
	.size	__func__.9319, 19
__func__.9319:
	.ascii	"h265e_cabac_finish\000"
	.type	__func__.9330, %object
	.size	__func__.9330, 18
__func__.9330:
	.ascii	"h265e_cabac_flush\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: sliceType = %d\000"
.LC1:
	.ascii	"\0016%s:%d: enter\012\000"
.LC2:
	.ascii	"\0016%s:%d: leave\012\000"
.LC3:
	.ascii	"\0016%s:%d: byteTowrite = %x\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
