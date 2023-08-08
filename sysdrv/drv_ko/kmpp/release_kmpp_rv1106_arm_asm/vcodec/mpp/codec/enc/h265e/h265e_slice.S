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
	.file	"h265e_slice.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	get_ref_pic
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_ref_pic, %function
get_ref_pic:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L17
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L2
	movs	r2, #20
	ldr	r1, .L17+4
	ldr	r0, .L17+8
	bl	printk
.L2:
	add	r3, r4, #1408
	mov	r0, r4
.L5:
	ldr	r2, [r0, #36]
	mov	r4, r0
	cbz	r2, .L3
	ldr	r2, [r0, #52]
	cmp	r2, r6
	beq	.L4
.L3:
	adds	r0, r0, #88
	cmp	r0, r3
	bne	.L5
.L4:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L1
	movs	r2, #26
	ldr	r1, .L17+4
	ldr	r0, .L17+12
	bl	printk
.L1:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L18:
	.align	2
.L17:
	.word	h265e_debug
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.fnend
	.size	get_ref_pic, .-get_ref_pic
	.global	__aeabi_idivmod
	.align	1
	.global	get_lt_ref_pic
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_lt_ref_pic, %function
get_lt_ref_pic:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	fp, r3
	ldr	r3, [r1, #372]
	movs	r7, #1
	ldr	r6, .L40
	mov	r8, r2
	mov	r9, r0
	mov	r10, r1
	ldr	r3, [r3, #160]
	add	r5, r0, #1320
	lsls	r7, r7, r3
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L20
	movs	r2, #37
	ldr	r1, .L40+4
	ldr	r0, .L40+8
	bl	printk
.L20:
	cmp	fp, #0
	bne	.L21
	mov	r0, r8
	mov	r1, r7
	bl	__aeabi_idivmod
	mov	r8, r1
.L21:
	mov	r4, r5
.L25:
	ldr	r3, [r4, #32]
	cbz	r3, .L22
	ldr	r0, [r4, #52]
	ldr	r3, [r10, #16]
	cmp	r0, r3
	beq	.L22
	ldr	r3, [r4, #56]
	ldr	r3, [r3, #328]
	cbz	r3, .L22
	cmp	fp, #0
	bne	.L23
	mov	r1, r7
	bl	__aeabi_idivmod
	mov	r0, r1
.L23:
	cmp	r8, r0
	bne	.L22
	ldr	r3, [r4, #40]
	mov	r5, r4
	cbnz	r3, .L19
.L22:
	cmp	r4, r9
	sub	r3, r4, #88
	bne	.L27
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L19
	movs	r2, #56
	ldr	r1, .L40+4
	ldr	r0, .L40+12
	bl	printk
.L19:
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L27:
	mov	r4, r3
	b	.L25
.L41:
	.align	2
.L40:
	.word	h265e_debug
	.word	.LANCHOR0+12
	.word	.LC0
	.word	.LC1
	.fnend
	.size	get_lt_ref_pic, .-get_lt_ref_pic
	.global	__aeabi_uidivmod
	.align	1
	.global	h265e_slice_set_ref_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_slice_set_ref_list, %function
h265e_slice_set_ref_list:
	.fnstart
	@ args = 0, pretend = 0, frame = 336
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L102
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #348
	sub	sp, sp, #348
	ldr	r2, [r3]
	mov	fp, r0
	mov	r4, r1
	ldr	r5, [r1, #520]
	tst	r2, #1
	str	r3, [sp, #12]
	beq	.L43
	movs	r2, #75
	ldr	r1, .L102+4
	ldr	r0, .L102+8
	bl	printk
.L43:
	ldr	r3, [r4, #368]
	cmp	r3, #2
	beq	.L44
	movs	r7, #0
	add	r6, r5, #412
	add	r10, sp, #16
	mov	r8, r7
	mov	r0, r7
	mov	r9, r7
.L45:
	ldr	r3, [r5, #340]
	cmp	r3, r7
	bgt	.L48
	add	r9, r7, #103
	movs	r6, #0
	mov	r10, r6
	add	r9, r5, r9, lsl #2
.L49:
	ldrd	r2, r1, [r5, #340]
	add	r2, r2, r1
	cmp	r2, r7
	bgt	.L51
	ldr	r3, [r5, #672]
	movs	r7, #0
	subs	r3, r3, #1
	add	r10, r3, r2
	add	r9, r10, #20
	add	r9, r5, r9, lsl #2
.L52:
	ldrd	r3, r2, [r5, #340]
	add	r3, r3, r2
	subs	r3, r3, #1
	cmp	r3, r10
	blt	.L55
	add	r3, r8, r6
	add	r1, sp, #16
	str	r3, [sp, #8]
	add	r9, r3, r7
	add	r3, sp, #208
	movs	r2, #0
.L56:
	cmp	r8, r2
	bgt	.L57
	bic	r5, r8, r8, asr #31
	add	ip, sp, #80
	movs	r2, #0
	add	r0, r3, r5, lsl #2
.L58:
	cmp	r6, r2
	bgt	.L59
	bic	r10, r6, r6, asr #31
	add	r0, sp, #144
	add	r5, r5, r10
	movs	r2, #0
	lsl	fp, r5, #2
	add	r3, r3, fp
.L60:
	cmp	r7, r2
	bgt	.L61
	cmp	r7, #0
	it	ge
	addge	r5, r5, r7
	cmp	r9, r5
	beq	.L62
	movs	r1, #131
	ldr	r3, .L102+12
	str	r1, [sp]
	ldr	r2, .L102+16
	ldr	r0, .L102+20
	bl	printk
.L62:
	ldr	r3, [r4, #368]
	cmp	r3, #0
	beq	.L86
.L64:
	add	r5, r4, #192
	movs	r2, #136
	movs	r1, #0
	ldr	fp, .L102+12
	mov	r0, r5
	ldr	r8, .L102+28
	bl	memset
	ldr	r10, .L102+20
	movs	r7, #0
.L71:
	ldr	r3, [r4, #40]
	cmp	r3, r7
	bgt	.L76
	ldr	r5, [r4, #368]
	movs	r1, #0
	cmp	r5, #0
	bne	.L77
	ldr	fp, .L102+12
	add	r7, r4, #260
	ldr	r8, .L102+28
	ldr	r10, .L102+20
.L78:
	ldr	r3, [r4, #44]
	cmp	r3, r5
	ble	.L79
	ldr	r3, [r4, #1204]
	cmp	r3, #0
	beq	.L80
	ldr	r6, [r7, #1076]
.L81:
	cmp	r6, #0
	blt	.L82
	cmp	r6, r9
	blt	.L83
.L82:
	movs	r1, #164
	mov	r3, fp
	str	r1, [sp]
	mov	r2, r8
	mov	r0, r10
	bl	printk
.L83:
	add	r3, sp, #344
	adds	r5, r5, #1
	add	r3, r3, r6, lsl #2
	ldr	r3, [r3, #-68]
	str	r3, [r7, #192]
	ldr	r3, [sp, #8]
	cmp	r3, r6
	ite	gt
	movgt	r6, #0
	movle	r6, #1
	str	r6, [r7], #4
	b	.L78
.L44:
	movs	r2, #136
	movs	r1, #0
	add	r0, r4, #384
	bl	memset
	movs	r3, #0
	str	r3, [r4, #40]	@ unaligned
	str	r3, [r4, #44]	@ unaligned
.L42:
	add	sp, sp, #348
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L48:
	ldr	r3, [r6], #4
	cbz	r3, .L47
	ldr	r1, [r4, #16]
	mov	r0, fp
	ldr	r3, [r6, #-68]
	add	r1, r1, r3
	bl	get_ref_pic
	str	r9, [r0, #40]
	str	r0, [r10, r8, lsl #2]
	add	r8, r8, #1
	str	r9, [r0, #48]
.L47:
	adds	r7, r7, #1
	b	.L45
.L51:
	ldr	r2, [r9], #4
	cbz	r2, .L50
	ldr	r1, [r4, #16]
	mov	r0, fp
	ldr	r2, [r9, #-68]
	add	r1, r1, r2
	bl	get_ref_pic
	add	r3, sp, #80
	str	r10, [r0, #40]
	str	r0, [r3, r6, lsl #2]
	adds	r6, r6, #1
	str	r10, [r0, #48]
.L50:
	adds	r7, r7, #1
	b	.L49
.L55:
	ldr	r3, [r9, #332]
	cbz	r3, .L53
	ldr	r3, [r9]
	mov	r1, r4
	ldr	r2, [r9, #524]
	mov	r0, fp
	bl	get_lt_ref_pic
	movs	r3, #1
	str	r3, [r0, #40]
	add	r3, sp, #344
	add	r3, r3, r7, lsl #2
	adds	r7, r7, #1
	str	r0, [r3, #-200]
.L53:
	cbnz	r0, .L54
	ldr	r3, [r9]
	mov	r1, r4
	ldr	r2, [r9, #524]
	mov	r0, fp
	bl	get_lt_ref_pic
.L54:
	ldr	r3, [r9], #-4
	add	r10, r10, #-1
	str	r3, [r0, #48]
	b	.L52
.L57:
	ldr	r0, [r1, r2, lsl #2]
	str	r0, [r3, r2, lsl #2]
	adds	r2, r2, #1
	b	.L56
.L59:
	ldr	r1, [ip, r2, lsl #2]
	adds	r2, r2, #1
	str	r1, [r0], #4
	b	.L58
.L61:
	ldr	r1, [r0, r2, lsl #2]
	adds	r2, r2, #1
	str	r1, [r3], #4
	b	.L60
.L65:
	ldr	r0, [r1, r3, lsl #2]
	str	r0, [r2, r3, lsl #2]
	adds	r3, r3, #1
.L63:
	cmp	r6, r3
	bgt	.L65
	add	r10, r2, r10, lsl #2
	add	r0, sp, #16
	movs	r3, #0
.L66:
	cmp	r8, r3
	bgt	.L67
	add	fp, fp, r2
	add	r2, sp, #144
	movs	r3, #0
.L68:
	cmp	r7, r3
	bgt	.L69
	cmp	r9, r5
	beq	.L64
	movs	r1, #140
	ldr	r3, .L102+12
	str	r1, [sp]
	ldr	r2, .L102+16
	ldr	r0, .L102+20
	bl	printk
	b	.L64
.L86:
	add	r2, sp, #276
	add	r1, sp, #80
	b	.L63
.L67:
	ldr	r1, [r0, r3, lsl #2]
	adds	r3, r3, #1
	str	r1, [r10], #4
	b	.L66
.L69:
	ldr	r1, [r2, r3, lsl #2]
	adds	r3, r3, #1
	str	r1, [fp], #4
	b	.L68
.L76:
	ldr	r3, [r4, #1200]
	cbz	r3, .L72
	ldr	r6, [r5, #1016]
.L73:
	cmp	r6, #0
	blt	.L74
	cmp	r6, r9
	blt	.L75
.L74:
	movs	r1, #148
	mov	r3, fp
	str	r1, [sp]
	mov	r2, r8
	mov	r0, r10
	bl	printk
.L75:
	add	r3, sp, #344
	adds	r7, r7, #1
	add	r3, r3, r6, lsl #2
	ldr	r3, [r3, #-136]
	str	r3, [r5, #192]
	ldr	r3, [sp, #8]
	cmp	r3, r6
	ite	gt
	movgt	r6, #0
	movle	r6, #1
	str	r6, [r5], #4
	b	.L71
.L72:
	mov	r1, r9
	mov	r0, r7
	bl	__aeabi_uidivmod
	mov	r6, r1
	b	.L73
.L77:
	str	r1, [r4, #44]
	movs	r2, #68
	add	r0, r4, #452
	bl	memset
.L79:
	ldr	r3, [sp, #12]
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L42
	movs	r2, #170
	ldr	r1, .L102+4
	ldr	r0, .L102+24
	bl	printk
	b	.L42
.L80:
	mov	r1, r9
	mov	r0, r5
	bl	__aeabi_uidivmod
	mov	r6, r1
	b	.L81
.L103:
	.align	2
.L102:
	.word	h265e_debug
	.word	.LANCHOR0+27
	.word	.LC0
	.word	.LANCHOR0+52
	.word	.LC2
	.word	.LC3
	.word	.LC1
	.word	.LC4
	.fnend
	.size	h265e_slice_set_ref_list, .-h265e_slice_set_ref_list
	.align	1
	.global	h265e_slice_set_ref_poc_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_slice_set_ref_poc_list, %function
h265e_slice_set_ref_poc_list:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r5, .L115
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L105
	movs	r2, #176
	ldr	r1, .L115+4
	ldr	r0, .L115+8
	bl	printk
.L105:
	ldr	r0, [r4, #40]
	add	r2, r4, #56
	movs	r3, #0
.L108:
	cmp	r3, r0
	blt	.L106
	ldr	r1, [r4, #44]
	adds	r4, r4, #124
	movs	r3, #0
.L107:
	cmp	r3, r1
	bge	.L109
	ldr	r2, [r4, #328]
	adds	r3, r3, #1
	ldr	r2, [r2, #52]
	str	r2, [r4], #4
	b	.L107
.L106:
	ldr	r1, [r2, #328]
	adds	r3, r3, #1
	ldr	r1, [r1, #52]
	str	r1, [r2], #4
	b	.L108
.L109:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L104
	movs	r2, #184
	ldr	r1, .L115+4
	ldr	r0, .L115+12
	pop	{r3, r4, r5, lr}
	b	printk
.L104:
	pop	{r3, r4, r5, pc}
.L116:
	.align	2
.L115:
	.word	h265e_debug
	.word	.LANCHOR0+77
	.word	.LC0
	.word	.LC1
	.fnend
	.size	h265e_slice_set_ref_poc_list, .-h265e_slice_set_ref_poc_list
	.align	1
	.global	h265e_slice_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_slice_init, %function
h265e_slice_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r10, r2
	movw	r2, #19000
	ldr	r7, .L143
	ldr	r2, [r0, r2]
	add	r9, r0, #4608
	ldr	r8, [r0]
	add	r6, r0, #18176
	.pad #20
	sub	sp, sp, #20
	mov	r5, r0
	ldr	r2, [r2, #876]
	mov	fp, r3
	add	r3, r0, #12
	add	r9, r9, #8
	str	r3, [sp, #12]
	adds	r6, r6, #92
	ldr	r4, [r2, #56]
	movw	r2, #18996
	str	r4, [r0, r2]
	ldr	r2, [r7]
	lsls	r0, r2, #31
	bpl	.L118
	movs	r2, #197
	ldr	r1, .L143+4
	ldr	r0, .L143+8
	bl	printk
.L118:
	mov	r2, #1520
	movs	r1, #0
	mov	r0, r4
	bl	memset
	str	r9, [r4, #372]
	movs	r2, #5
	str	r2, [r4, #336]
	movs	r2, #1
	strh	r2, [r4, #12]	@ movhi
	movs	r0, #0
	ldr	r3, [sp, #12]
	strd	r6, r3, [r4, #376]
	ldrb	r3, [r9, #2]	@ zero_extendqisi2
	uxtb	r9, r10
	strb	r2, [r4, #2]
	strb	r3, [r4, #9]
	movw	r3, #19000
	ldr	r1, [r5, r3]
	ldr	r1, [r1, #876]
	str	r0, [r1, #80]
	ands	r1, r9, #32
	beq	.L119
	movs	r1, #2
	str	r1, [r4, #368]
	ldr	r1, [r5, r3]
	ldr	r1, [r1, #876]
	str	r2, [r1, #80]
	ldr	r2, [r5, r3]
	ldr	r1, [r2, #876]
	ldrb	r2, [r1, #24]	@ zero_extendqisi2
	orr	r2, r2, #16
	strb	r2, [r1, #24]
	ldr	r3, [r5, r3]
	str	r0, [r3, #4]
.L120:
	movw	r3, #19000
	ldr	r3, [r5, r3]
	ldr	r3, [r3, #876]
	strd	r10, fp, [r3, #24]
	ldr	r3, [r4, #368]
	cbz	r3, .L121
	tst	r9, #2
	itt	eq
	moveq	r3, #1
	streq	r3, [r4, #328]
.L121:
	ldr	r3, [r4, #376]
	ldrb	r3, [r3, #6]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L122
	ldr	r3, [r7]
	lsls	r1, r3, #23
	bpl	.L123
	movs	r2, #226
	ldr	r1, .L143+4
	ldr	r0, .L143+12
	bl	printk
.L123:
	ldr	r3, [r8, #756]
	clz	r3, r3
	lsrs	r3, r3, #5
	strb	r3, [r4]
	ldr	r3, [r8, #760]
	clz	r3, r3
	lsrs	r3, r3, #5
	strb	r3, [r4, #1]
	ldr	r3, [r8, #784]
	str	r3, [r4, #336]
	ldr	r3, [r8, #732]
	strb	r3, [r4, #13]
	movs	r3, #1
	strb	r3, [r4, #2]
	ldr	r3, [r6, #24]
	strb	r3, [r4, #4]
	ldr	r3, [r4, #376]
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	cbz	r3, .L124
	ldr	r3, [r8, #748]
	str	r3, [r4, #48]
	ldr	r3, [r8, #752]
	str	r3, [r4, #52]
.L124:
	movw	r3, #19000
	tst	r9, #128
	ldr	r2, [r5, r3]
	ldr	r2, [r2, #876]
	ldr	r2, [r2, #8]
	str	r2, [r4, #16]
	ldr	r2, [r5, r3]
	ldr	r2, [r2, #4]
	str	r2, [r4, #20]
	ldr	r1, [r5, r3]
	ldr	r2, [r1, #4]
	add	r0, r2, #1
	str	r0, [r1, #4]
	ldr	r1, [r5, r3]
	ldr	r1, [r1, #876]
	str	r2, [r1, #12]
	ldr	r2, [r5, r3]
	ldr	r1, [r4, #16]
	ldr	r2, [r2, #876]
	str	r1, [r2, #52]
	itttt	ne
	movne	r2, #1
	ldrne	r3, [r5, r3]
	ldrne	r3, [r3, #876]
	strne	r2, [r3, #40]
	ldr	r3, [r7]
	lsls	r2, r3, #23
	bpl	.L126
	ldr	r3, [r4, #328]
	movs	r2, #253
	ldr	r1, .L143+4
	ldr	r0, .L143+16
	str	r3, [sp]
	ldr	r3, [r4, #368]
	bl	printk
.L126:
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L117
	movs	r2, #254
	ldr	r1, .L143+4
	ldr	r0, .L143+20
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	printk
.L119:
	str	r2, [r4, #368]
	ldr	r3, [r5, r3]
	ldr	r3, [r3, #876]
	ldrb	r2, [r3, #24]	@ zero_extendqisi2
	bfi	r2, r1, #4, #1
	strb	r2, [r3, #24]
	b	.L120
.L122:
	ldrb	r3, [r6, #7]	@ zero_extendqisi2
	strb	r3, [r4, #6]
	ldr	r3, [r6, #708]
	str	r3, [r4, #32]
	ldr	r3, [r6, #712]
	str	r3, [r4, #36]
	b	.L123
.L117:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L144:
	.align	2
.L143:
	.word	h265e_debug
	.word	.LANCHOR0+106
	.word	.LC0
	.word	.LC5
	.word	.LC6
	.word	.LC1
	.fnend
	.size	h265e_slice_init, .-h265e_slice_init
	.align	1
	.global	code_st_refpic_set
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	code_st_refpic_set, %function
code_st_refpic_set:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r2, #0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r5, r0
	mov	r4, r1
	ble	.L146
	movs	r2, #1
	ldr	r1, [r1, #668]
	bl	mpp_writer_put_bits
.L146:
	ldr	r6, [r4, #668]
	cbz	r6, .L147
	ldr	r7, [r4, #4]
	mov	r6, r4
	ldr	r1, [r6], #12
	mov	r0, r5
	bl	mpp_writer_put_ue
	movs	r2, #1
	lsrs	r1, r7, #31
	mov	r0, r5
	bl	mpp_writer_put_bits
	eor	r1, r7, r7, asr #31
	sub	r1, r1, r7, asr #31
	mov	r0, r5
	subs	r1, r1, #1
	movs	r7, #0
	bl	mpp_writer_put_ue
.L148:
	ldr	r3, [r4, #8]
	cmp	r3, r7
	bgt	.L150
.L145:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L150:
	ldr	r8, [r6], #4
	movs	r2, #1
	mov	r0, r5
	sub	r3, r8, r2
	rsbs	r1, r3, #0
	adcs	r1, r1, r3
	bl	mpp_writer_put_bits
	cmp	r8, #1
	beq	.L149
	movs	r2, #1
	mov	r0, r5
	asr	r1, r8, r2
	bl	mpp_writer_put_bits
.L149:
	adds	r7, r7, #1
	b	.L148
.L147:
	ldr	r1, [r4, #340]
	mov	r0, r5
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #344]
	mov	r0, r5
	add	r8, r4, #348
	bl	mpp_writer_put_ue
	mov	r9, r6
.L151:
	ldr	r7, [r4, #340]
	cmp	r7, r6
	bgt	.L152
	add	r6, r7, #87
	mov	r8, #0
	add	r6, r4, r6, lsl #2
.L153:
	ldrd	r3, r2, [r4, #340]
	add	r3, r3, r2
	cmp	r3, r7
	ble	.L145
	ldr	r1, [r6]
	mvn	r8, r8
	mov	r0, r5
	adds	r7, r7, #1
	add	r1, r1, r8
	bl	mpp_writer_put_ue
	ldr	r8, [r6], #4
	movs	r2, #1
	mov	r0, r5
	ldr	r1, [r6, #60]
	bl	mpp_writer_put_bits
	b	.L153
.L152:
	ldr	r1, [r8]
	mov	r0, r5
	adds	r6, r6, #1
	mvns	r1, r1
	add	r1, r1, r9
	bl	mpp_writer_put_ue
	ldr	r9, [r8], #4
	movs	r2, #1
	mov	r0, r5
	ldr	r1, [r8, #60]
	bl	mpp_writer_put_bits
	b	.L151
	.fnend
	.size	code_st_refpic_set, .-code_st_refpic_set
	.align	1
	.global	find_matching_ltrp
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	find_matching_ltrp, %function
find_matching_ltrp:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r3
	ldr	r4, [r0, #372]
	mov	r6, r1
	movs	r1, #1
	mov	r0, r2
	ldr	r3, [r4, #160]
	lsls	r1, r1, r3
	bl	__aeabi_idivmod
	ldr	r5, [r4, #164]
	add	r0, r4, #168
	adds	r4, r4, #12
	movs	r2, #0
.L160:
	cmp	r5, r2
	bne	.L163
	movs	r0, #0
	b	.L162
.L163:
	ldr	r3, [r0], #4
	cmp	r3, r1
	bne	.L161
	ldrb	r3, [r4, r2]	@ zero_extendqisi2
	cmp	r3, r7
	bne	.L161
	movs	r0, #1
	str	r2, [r6]
.L162:
	pop	{r3, r4, r5, r6, r7, pc}
.L161:
	adds	r2, r2, #1
	b	.L160
	.fnend
	.size	find_matching_ltrp, .-find_matching_ltrp
	.align	1
	.global	get_num_rps_cur_templist
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_num_rps_cur_templist, %function
get_num_rps_cur_templist:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r1, #0
	ldrd	r3, r2, [r0, #340]
	add	r3, r3, r2
	ldr	r2, [r0, #672]
	add	r2, r2, r3
	add	r3, r0, #412
	mov	r0, r1
.L165:
	cmp	r2, r1
	bgt	.L167
	pop	{r4, pc}
.L167:
	ldr	r4, [r3], #4
	cbz	r4, .L166
	adds	r0, r0, #1
.L166:
	adds	r1, r1, #1
	b	.L165
	.fnend
	.size	get_num_rps_cur_templist, .-get_num_rps_cur_templist
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.9504, %object
	.size	__func__.9504, 12
__func__.9504:
	.ascii	"get_ref_pic\000"
	.type	__func__.9518, %object
	.size	__func__.9518, 15
__func__.9518:
	.ascii	"get_lt_ref_pic\000"
	.type	__func__.9541, %object
	.size	__func__.9541, 25
__func__.9541:
	.ascii	"h265e_slice_set_ref_list\000"
	.type	__FUNCTION__.9560, %object
	.size	__FUNCTION__.9560, 25
__FUNCTION__.9560:
	.ascii	"h265e_slice_set_ref_list\000"
	.type	__func__.9589, %object
	.size	__func__.9589, 29
__func__.9589:
	.ascii	"h265e_slice_set_ref_poc_list\000"
	.type	__func__.9607, %object
	.size	__func__.9607, 17
__func__.9607:
	.ascii	"h265e_slice_init\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: enter\012\000"
.LC1:
	.ascii	"\0016%s:%d: leave\012\000"
.LC2:
	.ascii	"cIdx == numPocTotalCurr\000"
.LC3:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC4:
	.ascii	"cIdx >= 0 && cIdx < numPocTotalCurr\000"
.LC5:
	.ascii	"\0016%s:%d: to do in this case\000"
.LC6:
	.ascii	"\0016%s:%d: slice->slice_type = %d slice->is_refere"
	.ascii	"nced = %d \012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
