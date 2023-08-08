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
	.file	"mpp_enc_refs.c"
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
	.type	get_ref_from_cpb, %function
get_ref_from_cpb:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #28
	sub	sp, sp, #28
	ldrb	r4, [r1]	@ zero_extendqisi2
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	ldrh	r2, [r1, #2]
	lsls	r4, r4, #27
	and	r3, r3, #63
	sbfx	r2, r2, #6, #8
	bmi	.L13
	cmp	r3, #6
	bhi	.L3
	tbb	[pc, r3]
.L5:
	.byte	(.L8-.L5)/2
	.byte	(.L8-.L5)/2
	.byte	(.L8-.L5)/2
	.byte	(.L8-.L5)/2
	.byte	(.L7-.L5)/2
	.byte	(.L6-.L5)/2
	.byte	(.L4-.L5)/2
	.p2align 1
.L8:
	add	r4, r3, #36
.L20:
	add	r4, r0, r4, lsl #3
	cbz	r4, .L1
	ldrb	r0, [r4]	@ zero_extendqisi2
	lsls	r5, r0, #31
	bpl	.L10
	ldr	r5, .L21
	ldr	r5, [r5]
	lsls	r5, r5, #30
	bpl	.L1
	lsls	r0, r0, #24
	bmi	.L11
	ldr	r5, .L21+4
	movs	r0, #0
.L12:
	strd	r5, r0, [sp, #12]
	ldrh	r0, [r4, #6]
	str	r3, [sp]
	strd	r2, r0, [sp, #4]
	movw	r2, #379
	ldrh	r3, [r1, #6]
	ldr	r0, .L21+8
	ldr	r1, .L21+12
	bl	printk
.L1:
	mov	r0, r4
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, pc}
.L7:
	add	r4, r2, #68
	b	.L20
.L6:
	add	r4, r2, #84
	b	.L20
.L4:
	adds	r4, r2, #4
	b	.L20
.L3:
	str	r3, [sp]
	mov	r2, #368
	ldrh	r3, [r1, #6]
	ldr	r0, .L21+16
	ldr	r1, .L21+12
	bl	printk
.L13:
	movs	r4, #0
	b	.L1
.L10:
	ldrh	r0, [r4, #6]
	str	r3, [sp]
	strd	r2, r0, [sp, #4]
	movw	r2, #383
	ldrh	r3, [r1, #6]
	ldr	r0, .L21+20
	ldr	r1, .L21+12
	bl	printk
	b	.L1
.L11:
	ldrb	r0, [r4, #1]	@ zero_extendqisi2
	ldr	r5, .L21+24
	and	r0, r0, #15
	b	.L12
.L22:
	.align	2
.L21:
	.word	.LANCHOR1
	.word	.LC1
	.word	.LC3
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC4
	.word	.LC0
	.fnend
	.size	get_ref_from_cpb, .-get_ref_from_cpb
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	cleanup_cpb_refs, %function
cleanup_cpb_refs:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r2, #256
	mov	r4, r0
	movs	r1, #0
	adds	r0, r0, #32
	bl	memset
	mov	r2, #256
	movs	r1, #0
	add	r0, r4, #288
	bl	memset
	movs	r2, #128
	movs	r1, #0
	add	r0, r4, #544
	bl	memset
	movs	r2, #128
	movs	r1, #0
	add	r0, r4, #672
	bl	memset
	movs	r2, #16
	movs	r1, #0
	add	r0, r4, #1376
	bl	memset
	movs	r2, #16
	movs	r1, #0
	add	r0, r4, #1392
	bl	memset
	movs	r2, #16
	movs	r1, #0
	add	r0, r4, #1408
	bl	memset
	ldr	r2, [r4, #1432]
	movs	r3, #0
	str	r3, [r4, #1428]
	adds	r2, r2, #1
	str	r2, [r4, #1432]
	add	r2, r4, #576
	str	r3, [r4, #1436]
	str	r3, [r4, #1440]
.L24:
	ldr	r1, [r4, #800]
	strd	r3, r3, [r4, #812]
	adds	r4, r4, #36
	str	r1, [r4, #768]
	cmp	r4, r2
	bne	.L24
	pop	{r4, pc}
	.fnend
	.size	cleanup_cpb_refs, .-cleanup_cpb_refs
	.align	1
	.global	_dump_frm
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_dump_frm, %function
_dump_frm:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r3, r2
	ldrb	r6, [r0]	@ zero_extendqisi2
	.pad #36
	sub	sp, sp, #36
	lsls	r2, r6, #31
	bpl	.L26
	tst	r6, #64
	ldrh	r7, [r0, #6]
	ldrh	r4, [r0, #2]
	and	r2, r6, #1
	ldrb	r5, [r0, #2]	@ zero_extendqisi2
	and	ip, r6, #16
	ldrb	r0, [r0, #1]	@ zero_extendqisi2
	beq	.L28
	ldr	r6, .L40
	lsrs	r0, r0, #4
	ldr	lr, .L40+16
	sbfx	r4, r4, #6, #8
	strd	r2, r7, [sp]
	mov	r2, r1
	cmp	ip, #0
	it	eq
	moveq	r6, lr
	strd	r6, r0, [sp, #8]
	ldr	r0, .L40+4
	movs	r1, #117
	and	r5, r5, #63
	str	r4, [sp, #20]
	str	r5, [sp, #16]
.L39:
	bl	printk
	b	.L26
.L28:
	tst	r6, #128
	ldr	lr, .L40+16
	ldr	r6, .L40
	beq	.L30
	cmp	ip, #0
	it	eq
	moveq	r6, lr
	sbfx	r4, r4, #6, #8
	and	ip, r0, #15
	and	r5, r5, #63
	lsrs	r0, r0, #4
	strd	r2, r7, [sp]
	strd	r6, r0, [sp, #8]
	mov	r2, r1
	str	ip, [sp, #24]
	movs	r1, #123
	str	r4, [sp, #20]
	str	r5, [sp, #16]
	ldr	r0, .L40+8
	bl	printk
.L26:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L30:
	lsrs	r0, r0, #4
	cmp	ip, #0
	it	eq
	moveq	r6, lr
	sbfx	r4, r4, #6, #8
	and	r5, r5, #63
	strd	r6, r0, [sp, #8]
	strd	r2, r7, [sp]
	mov	r2, r1
	str	r4, [sp, #20]
	movs	r1, #128
	str	r5, [sp, #16]
	ldr	r0, .L40+12
	b	.L39
.L41:
	.align	2
.L40:
	.word	.LC5
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC6
	.fnend
	.size	_dump_frm, .-_dump_frm
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_st_cfg_to_frm, %function
set_st_cfg_to_frm:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4, r5}
	.save {r4, r5}
	movs	r5, #0
	strh	r1, [r0, #6]	@ movhi
	clz	r1, r1
	str	r5, [r0]	@ unaligned
	ldrb	r3, [r0]	@ zero_extendqisi2
	lsrs	r1, r1, #5
	strh	r5, [r0, #4]	@ unaligned
	orr	r3, r3, #1
	bfi	r3, r1, #5, #1
	bfi	r3, r1, #4, #1
	strb	r3, [r0]
	ldr	r1, [r2]
	bfi	r3, r1, #6, #1
	bfi	r3, r5, #7, #1
	strb	r3, [r0]
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	ldr	r1, [r2, #4]
	bfi	r3, r1, #4, #4
	strb	r3, [r0, #1]
	ldrb	r3, [r0, #2]	@ zero_extendqisi2
	ldr	r1, [r2, #8]
	bfi	r3, r1, #0, #6
	strb	r3, [r0, #2]
	ldr	r2, [r2, #12]
	ldrh	r3, [r0, #2]
	bfi	r3, r2, #6, #8
	strh	r3, [r0, #2]	@ movhi
	ldr	r3, .L44
	ldr	r3, [r3]
	lsls	r3, r3, #29
	bpl	.L42
	mov	r2, #316
	ldr	r1, .L44+4
	pop	{r4, r5}
	b	_dump_frm
.L42:
	pop	{r4, r5}
	bx	lr
.L45:
	.align	2
.L44:
	.word	.LANCHOR1
	.word	.LANCHOR0+17
	.fnend
	.size	set_st_cfg_to_frm, .-set_st_cfg_to_frm
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	store_ref_to_cpb, %function
store_ref_to_cpb:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r5, r1
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	mov	r4, r0
	ldrh	r8, [r1, #6]
	and	r7, r3, #15
	lsrs	r6, r3, #4
	ldrb	r3, [r1]	@ zero_extendqisi2
	lsls	r1, r3, #31
	bmi	.L47
	movw	r1, #501
	ldr	r3, .L85
	str	r1, [sp]
	ldr	r2, .L85+4
	ldr	r0, .L85+8
	bl	printk
.L47:
	ldrb	r3, [r5]	@ zero_extendqisi2
	lsls	r2, r3, #25
	bmi	.L46
	lsls	r3, r3, #27
	bpl	.L49
	ldrd	r2, [r5]
	strd	r2, [r4, #312]
.L49:
	ldrsb	r3, [r5]
	ldrd	r0, [r5]
	cmp	r3, #0
	add	r3, r6, #68
	bge	.L50
	add	r2, r4, r7, lsl #3
	add	r3, r4, r3, lsl #3
	strd	r0, [r2, #672]
	ldrd	r0, [r5]
	strd	r0, [r3]
	movs	r3, #0
	strd	r0, [r4, #288]
	ldrd	r0, [r5]
	strd	r0, [r4, #304]
.L52:
	add	r2, r3, #20
	add	r5, r3, #16
	ldrb	r6, [r4, r2, lsl #3]	@ zero_extendqisi2
	lsls	r6, r6, #31
	bpl	.L63
	add	r2, r4, r2, lsl #3
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	and	r2, r2, #15
	cmp	r2, r7
	beq	.L64
	adds	r3, r3, #1
	cmp	r3, #16
	bne	.L52
	str	r7, [sp]
	mov	r3, r8
	movw	r2, #546
	ldr	r1, .L85+12
	ldr	r0, .L85+16
	bl	printk
	b	.L55
.L50:
	add	r3, r4, r3, lsl #3
	strd	r0, [r3]
	strd	r0, [r4, #288]
	add	r1, r4, #152
	ldrd	r2, [r5]
	add	r0, r4, #32
	strd	r2, [r4, #296]
.L56:
	ldrd	r2, [r1, #-8]
	strd	r2, [r1], #-8
	cmp	r0, r1
	bne	.L56
	ldrd	r2, [r5]
	strd	r2, [r4, #32]
	b	.L55
.L63:
	movs	r6, #1
.L51:
	add	r2, r4, r5, lsl #3
	strd	r0, [r2, #32]
	ldr	r2, .L85+20
	ldr	r2, [r2]
	lsls	r2, r2, #30
	bpl	.L46
	ldr	r1, .L85+24
	ldr	r2, .L85+28
	str	r7, [sp]
	cmp	r6, #1
	it	ne
	movne	r2, r1
	ldr	r0, .L85+32
	strd	r2, r3, [sp, #4]
	mov	r3, r8
	movw	r2, #542
	ldr	r1, .L85+12
	bl	printk
.L55:
	ldr	r5, .L85+20
	ldr	r3, [r5]
	lsls	r7, r3, #30
	bpl	.L46
	movw	r2, #562
	ldr	r1, .L85+12
	ldr	r0, .L85+36
	bl	printk
	ldr	r3, [r5]
	lsls	r6, r3, #30
	bpl	.L46
	ldr	r8, .L85
	add	r7, r4, #32
	movs	r6, #0
.L60:
	adds	r3, r6, #4
	ldrb	r3, [r4, r3, lsl #3]	@ zero_extendqisi2
	lsls	r0, r3, #31
	bpl	.L59
	movw	r2, #566
	mov	r1, r8
	add	r0, r7, r6, lsl #3
	bl	_dump_frm
.L59:
	adds	r6, r6, #1
	cmp	r6, #32
	bne	.L60
	ldr	r3, [r5]
	lsls	r1, r3, #30
	bpl	.L46
	mov	r2, #568
	ldr	r1, .L85+12
	ldr	r0, .L85+40
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	printk
.L64:
	movs	r6, #2
	b	.L51
.L46:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L86:
	.align	2
.L85:
	.word	.LANCHOR0+35
	.word	.LC12
	.word	.LC13
	.word	.LANCHOR0+52
	.word	.LC17
	.word	.LANCHOR1
	.word	.LC11
	.word	.LC10
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.fnend
	.size	store_ref_to_cpb, .-store_ref_to_cpb
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	save_cpb_status, %function
save_cpb_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r5, r0
	ldr	r6, [r5], #160
	mov	r8, #0
	ldr	r10, .L109
	mov	r7, r0
	ldr	fp, .L109+20
	mov	r9, r1
	mov	r4, r8
.L88:
	ldr	r3, [r7, #4]
	cmp	r3, r8
	bgt	.L91
	cmp	r4, r6
	bge	.L101
	ldr	r3, [r7, #8]
	subs	r6, r6, r4
	ldr	r10, .L109
	add	r8, r7, #32
	cmp	r6, r3
	mov	r7, #0
	mov	r5, r4
	it	lt
	movlt	r6, r3
	mov	fp, r6
	mov	r6, r7
.L93:
	cmp	fp, r7
	bgt	.L96
.L92:
	ldr	r7, .L109
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L87
	mov	r3, r5
	strd	r4, r6, [sp]
	mov	r2, #488
	ldr	r1, .L109+4
	ldr	r0, .L109+8
	bl	printk
	ldr	r3, [r7]
	lsls	r3, r3, #30
	bmi	.L102
.L87:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L91:
	ldrb	r3, [r5]	@ zero_extendqisi2
	and	r3, r3, #193
	cmp	r3, #129
	bne	.L89
	ldr	r3, [r10]
	lsls	r0, r3, #30
	bpl	.L90
	str	r4, [sp]
	movw	r2, #459
	ldrh	r3, [r5, #6]
	mov	r0, fp
	ldr	r1, .L109+4
	bl	printk
.L90:
	ldrd	r2, [r5]
	add	r1, r9, r4, lsl #3
	adds	r4, r4, #1
	strd	r2, [r1]
.L89:
	add	r8, r8, #1
	adds	r5, r5, #8
	b	.L88
.L96:
	ldrb	r3, [r8]	@ zero_extendqisi2
	and	r3, r3, #193
	cmp	r3, #1
	bne	.L94
	ldr	r3, [r10]
	lsls	r1, r3, #30
	bpl	.L95
	str	r5, [sp]
	movw	r2, #481
	ldrh	r3, [r8, #6]
	ldr	r1, .L109+4
	ldr	r0, .L109+12
	bl	printk
.L95:
	ldrd	r2, [r8]
	add	r1, r9, r5, lsl #3
	adds	r6, r6, #1
	adds	r5, r5, #1
	strd	r2, [r1]
.L94:
	adds	r7, r7, #1
	add	r8, r8, #8
	b	.L93
.L101:
	mov	r5, r4
	movs	r6, #0
	b	.L92
.L100:
	add	r0, r9, r4, lsl #3
	movw	r2, #491
	mov	r1, r6
	adds	r4, r4, #1
	bl	_dump_frm
.L98:
	cmp	r5, r4
	bgt	.L100
	b	.L87
.L102:
	movs	r4, #0
	ldr	r6, .L109+16
	b	.L98
.L110:
	.align	2
.L109:
	.word	.LANCHOR1
	.word	.LANCHOR0+69
	.word	.LC20
	.word	.LC19
	.word	.LANCHOR0+85
	.word	.LC18
	.fnend
	.size	save_cpb_status, .-save_cpb_status
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.global	_dump_cpb
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_dump_cpb, %function
_dump_cpb:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #20
	sub	sp, sp, #20
	mov	r5, r0
	mov	r3, r2
	str	r0, [sp]
	mov	r2, r1
	ldr	r0, .L120
	movs	r1, #139
	bl	printk
	ldr	r3, [r5, #8]
	movs	r1, #141
	ldr	r0, .L120+4
	ldr	r7, .L120+8
	add	r4, r5, #32
	add	r6, r5, #288
	str	r3, [sp]
	ldrd	r2, r3, [r5]
	bl	printk
	ldrd	r2, r3, [r5, #12]
	movs	r1, #143
	ldr	r0, .L120+12
	bl	printk
	ldrd	r2, r3, [r5, #20]
	movs	r1, #144
	ldr	r0, .L120+16
	bl	printk
	movs	r1, #146
	ldr	r0, .L120+20
	bl	printk
.L112:
	mov	r0, r4
	movs	r2, #148
	mov	r1, r7
	adds	r4, r4, #8
	bl	_dump_frm
	cmp	r4, r6
	bne	.L112
	movs	r1, #150
	ldr	r0, .L120+24
	bl	printk
	ldr	r7, .L120+8
	add	r6, r5, #544
.L113:
	mov	r0, r4
	movs	r2, #152
	mov	r1, r7
	adds	r4, r4, #8
	bl	_dump_frm
	cmp	r4, r6
	bne	.L113
	movs	r1, #154
	ldr	r0, .L120+28
	bl	printk
	ldr	r7, .L120+8
	add	r6, r5, #672
.L114:
	mov	r0, r4
	movs	r2, #156
	mov	r1, r7
	adds	r4, r4, #8
	bl	_dump_frm
	cmp	r4, r6
	bne	.L114
	movs	r1, #158
	ldr	r0, .L120+32
	bl	printk
	ldr	r7, .L120+8
	add	r6, r5, #800
.L115:
	mov	r0, r4
	movs	r2, #160
	mov	r1, r7
	adds	r4, r4, #8
	bl	_dump_frm
	cmp	r4, r6
	bne	.L115
	ldr	r3, [r5, #1440]
	movs	r1, #165
	ldr	r2, [r5, #1424]
	ldr	r0, .L120+36
	str	r3, [sp, #8]
	ldr	r3, [r5, #1436]
	str	r3, [sp, #4]
	ldr	r3, [r5, #1432]
	str	r3, [sp]
	ldr	r3, [r5, #1428]
	bl	printk
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L121:
	.align	2
.L120:
	.word	.LC21
	.word	.LC22
	.word	.LANCHOR0+101
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.fnend
	.size	_dump_cpb, .-_dump_cpb
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_ref_cpb_pos, %function
check_ref_cpb_pos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r5, r0
	ldrb	r3, [r1]	@ zero_extendqisi2
	ldrh	r6, [r1, #6]
	and	r2, r3, #65
	cmp	r2, #1
	beq	.L123
	ldr	r3, .L150
	ldr	r3, [r3]
	lsls	r2, r3, #30
	bpl	.L131
	mov	r3, r6
	movw	r2, #397
	ldr	r1, .L150+4
	ldr	r0, .L150+8
	bl	printk
.L131:
	mov	r4, #-1
	b	.L122
.L123:
	lsls	r3, r3, #24
	bpl	.L132
	movs	r3, #16
.L127:
	adds	r2, r3, #4
	mov	r4, r3
	ldrb	r0, [r5, r2, lsl #3]	@ zero_extendqisi2
	lsls	r7, r0, #31
	bpl	.L126
	add	r2, r5, r2, lsl #3
	ldrb	r0, [r1, #1]	@ zero_extendqisi2
	ldrb	r2, [r2, #1]	@ zero_extendqisi2
	eors	r2, r2, r0
	lsls	r0, r2, #28
	beq	.L122
.L126:
	adds	r3, r3, #1
	cmp	r3, #32
	bne	.L127
.L128:
	mov	r3, r6
	movw	r2, #431
	ldr	r1, .L150+4
	ldr	r0, .L150+12
	bl	printk
	movw	r2, #433
	ldr	r1, .L150+16
	mov	r0, r5
	bl	_dump_cpb
	b	.L131
.L132:
	ldr	r8, .L150
	movs	r4, #0
	ldr	r10, .L150+4
	mov	r9, r8
.L125:
	ldr	r3, [r8]
	adds	r7, r4, #4
	tst	r3, #2
	beq	.L129
	add	r3, r5, r7, lsl #3
	movw	r2, #419
	mov	r1, r10
	ldr	r0, .L150+20
	ldrh	r3, [r3, #6]
	strd	r4, r3, [sp]
	mov	r3, r6
	bl	printk
.L129:
	ldrb	r3, [r5, r7, lsl #3]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bpl	.L130
	add	r7, r5, r7, lsl #3
	ldrh	r3, [r7, #6]
	cmp	r3, r6
	bne	.L130
	ldr	r3, [r9]
	lsls	r3, r3, #30
	bpl	.L122
	str	r4, [sp]
	mov	r3, r6
	movw	r2, #423
	ldr	r1, .L150+4
	ldr	r0, .L150+24
	bl	printk
.L122:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L130:
	adds	r4, r4, #1
	cmp	r4, #16
	bne	.L125
	b	.L128
.L151:
	.align	2
.L150:
	.word	.LANCHOR1
	.word	.LANCHOR0+111
	.word	.LC30
	.word	.LC33
	.word	.LANCHOR0+129
	.word	.LC31
	.word	.LC32
	.fnend
	.size	check_ref_cpb_pos, .-check_ref_cpb_pos
	.align	1
	.global	mpp_enc_refs_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_init, %function
mpp_enc_refs_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	cbnz	r0, .L153
	ldr	r1, .L162
	movs	r2, #172
	ldr	r0, .L162+4
.L161:
	bl	printk
	mvn	r0, #2
.L152:
	pop	{r3, r4, r5, pc}
.L153:
	ldr	r4, .L162+8
	ldr	r2, [r4]
	lsls	r3, r2, #31
	bpl	.L155
	mov	r3, r0
	movs	r2, #176
	ldr	r1, .L162
	ldr	r0, .L162+12
	bl	printk
.L155:
	mov	r1, #2944
	ldr	r0, .L162+16
	bl	mpp_osal_calloc
	mov	r3, r0
	str	r0, [r5]
	cbnz	r0, .L156
	movs	r2, #181
	ldr	r1, .L162
	ldr	r0, .L162+20
	b	.L161
.L156:
	ldr	r2, [r4]
	ands	r2, r2, #1
	beq	.L157
	movs	r2, #186
	ldr	r1, .L162
	ldr	r0, .L162+24
	bl	printk
	movs	r0, #0
	b	.L152
.L157:
	mov	r0, r2
	b	.L152
.L163:
	.align	2
.L162:
	.word	.LANCHOR0+147
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LANCHOR0+165
	.word	.LC36
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_init, .-mpp_enc_refs_init
	.align	1
	.global	mpp_enc_refs_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_deinit, %function
mpp_enc_refs_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	cbnz	r0, .L165
	movs	r2, #194
	ldr	r1, .L176
	ldr	r0, .L176+4
	bl	printk
	mvn	r0, #5
.L164:
	pop	{r3, r4, r5, pc}
.L165:
	ldr	r5, .L176+8
	ldr	r2, [r5]
	lsls	r3, r2, #31
	bpl	.L167
	mov	r3, r0
	movs	r2, #198
	ldr	r1, .L176
	ldr	r0, .L176+12
	bl	printk
.L167:
	ldr	r1, [r4]
	cbz	r1, .L168
	ldr	r0, .L176+16
	bl	mpp_osal_free
.L168:
	ldr	r0, [r5]
	ands	r0, r0, #1
	beq	.L164
	mov	r3, r4
	movs	r2, #203
	ldr	r1, .L176
	ldr	r0, .L176+20
	bl	printk
	movs	r0, #0
	b	.L164
.L177:
	.align	2
.L176:
	.word	.LANCHOR0+183
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LANCHOR0+203
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_deinit, .-mpp_enc_refs_deinit
	.align	1
	.global	mpp_enc_refs_set_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_set_cfg, %function
mpp_enc_refs_set_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r5, r1
	mov	r4, r0
	cbnz	r0, .L179
.L182:
	str	r5, [sp]
	mov	r3, r4
	movs	r2, #214
	ldr	r1, .L206
	ldr	r0, .L206+4
	bl	printk
	mvn	r0, #5
.L178:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L179:
	cbz	r1, .L181
	ldr	r0, .L206+8
	bl	_check_is_mpp_enc_ref_cfg
	cmp	r0, #0
	bne	.L182
.L181:
	ldr	r6, .L206+12
	ldr	r3, [r6]
	mov	r8, r6
	tst	r3, #1
	beq	.L183
	str	r5, [sp]
	mov	r3, r4
	movs	r2, #218
	ldr	r1, .L206
	ldr	r0, .L206+16
	bl	printk
.L183:
	add	r7, r4, #48
	cbnz	r5, .L184
	bl	mpp_enc_ref_default
	mov	r5, r0
.L184:
	ldr	r3, [r4]
	str	r5, [r4, #4]
	orr	r3, r3, #1
	str	r3, [r4]
	movs	r3, #0
	str	r3, [r4, #44]
	ldr	r1, [r5, #12]
	cbnz	r1, .L185
	mov	r2, #1448
	mov	r0, r7
	bl	memset
	movs	r3, #1
	str	r3, [r4, #44]
.L185:
	ldr	r3, [r5, #24]
	cbnz	r3, .L186
.L192:
	ldr	r3, [r4, #48]
	cmp	r3, #0
	bne	.L187
.L188:
	add	r3, r5, #40
	add	r2, r5, #68
.L193:
	ldr	r1, [r3], #4	@ unaligned
	cmp	r3, r2
	str	r1, [r7], #4	@ unaligned
	bne	.L193
	ldr	r3, [r8]
	lsls	r3, r3, #30
	bpl	.L194
	ldr	r3, [r4, #64]
	mov	r2, #268
	ldr	r1, .L206
	ldr	r0, .L206+20
	str	r3, [sp, #8]
	ldr	r3, [r4, #60]
	str	r3, [sp, #4]
	ldr	r3, [r4, #56]
	str	r3, [sp]
	ldr	r3, [r4, #52]
	bl	printk
.L194:
	ldr	r0, [r8]
	ands	r0, r0, #1
	beq	.L178
	str	r5, [sp]
	mov	r3, r4
	mov	r2, #270
	ldr	r1, .L206
	ldr	r0, .L206+24
	bl	printk
	movs	r0, #0
	b	.L178
.L186:
	cmp	r3, #15
	ble	.L189
	movs	r1, #242
	ldr	r3, .L206+8
	str	r1, [sp]
	ldr	r2, .L206+28
	ldr	r0, .L206+32
	bl	printk
.L189:
	add	r3, r4, #848
	movs	r1, #0
	mov	ip, #24
.L190:
	ldr	r2, [r5, #24]
	adds	r3, r3, #36
	cmp	r2, r1
	ble	.L192
	mul	lr, ip, r1
	ldr	r0, [r5, #32]
	adds	r1, r1, #1
	add	r2, r0, lr
	ldr	r6, [r2, #20]
	str	r6, [r3, #-36]
	ldr	r6, [r2, #20]
	str	r6, [r3, #-32]
	ldr	r6, [r2, #16]
	str	r6, [r3, #-28]
	ldr	r0, [r0, lr]
	str	r0, [r3, #-16]
	ldr	r0, [r2, #4]
	str	r0, [r3, #-12]
	ldr	r0, [r2, #8]
	str	r0, [r3, #-8]
	ldr	r2, [r2, #12]
	str	r2, [r3, #-4]
	b	.L190
.L187:
	ldr	r2, [r5, #40]
	cmp	r3, r2
	itt	lt
	movlt	r3, #1
	strlt	r3, [r4, #44]
	b	.L188
.L207:
	.align	2
.L206:
	.word	.LANCHOR0+223
	.word	.LC38
	.word	.LANCHOR0+244
	.word	.LANCHOR1
	.word	.LC39
	.word	.LC41
	.word	.LC42
	.word	.LC40
	.word	.LC13
	.fnend
	.size	mpp_enc_refs_set_cfg, .-mpp_enc_refs_set_cfg
	.align	1
	.global	mpp_enc_refs_dryrun
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_dryrun, %function
mpp_enc_refs_dryrun:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r0
	.pad #44
	sub	sp, sp, #44
	cbnz	r0, .L209
	movw	r2, #585
	ldr	r1, .L253
	ldr	r0, .L253+4
	bl	printk
	mvn	r0, #5
.L208:
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L209:
	ldr	r4, .L253+8
	ldr	r2, [r4]
	lsls	r6, r2, #31
	bpl	.L211
	mov	r3, r0
	movw	r2, #589
	ldr	r1, .L253
	ldr	r0, .L253+12
	bl	printk
.L211:
	ldr	r3, [r5, #4]
	ldr	r2, [r3, #4]
	cbnz	r2, .L212
	ldr	r2, [r3, #36]
	add	r7, r5, #48
	mov	r0, r7
	str	r2, [sp, #12]
	ldr	r2, [r3, #24]
	ldr	r3, [r3, #28]
	str	r2, [sp, #16]
	str	r3, [sp, #20]
	bl	cleanup_cpb_refs
	ldr	r3, [r4]
	lsls	r4, r3, #30
	bpl	.L213
	ldr	r3, [sp, #20]
	movw	r2, #605
	ldr	r1, .L253
	ldr	r0, .L253+16
	str	r3, [sp]
	ldr	r3, [sp, #16]
	bl	printk
.L213:
	ldr	fp, .L253+8
	mov	r8, #0
	mov	r10, r8
	mov	r6, r8
.L214:
	ldr	r3, [sp, #20]
	cmp	r8, r3
	blt	.L228
	mov	r0, r7
	bl	cleanup_cpb_refs
	cmp	r6, #0
	it	eq
	moveq	r6, #1
	str	r6, [r5, #56]
.L212:
	ldr	r4, .L253+8
	ldrd	r3, r2, [r5, #52]
	ldr	r1, [r4]
	add	r3, r3, r2
	str	r3, [r5, #48]
	lsls	r2, r1, #28
	bpl	.L230
	movw	r2, #677
	ldr	r1, .L253
	ldr	r0, .L253+20
	bl	printk
.L230:
	ldr	r0, [r4]
	ands	r0, r0, #1
	beq	.L208
	mov	r3, r5
	movw	r2, #679
	ldr	r1, .L253
	ldr	r0, .L253+24
	bl	printk
	movs	r0, #0
	b	.L208
.L228:
	ldr	r3, [sp, #12]
	mov	r1, r10
	ldr	r9, [r3, #16]
	cmp	r9, #0
	it	eq
	moveq	r9, #1
.L216:
	add	r3, r10, r9
	subs	r3, r3, r1
	cmp	r3, #0
	bgt	.L227
	ldr	r3, [sp, #12]
	cmp	r9, #0
	it	ge
	addge	r10, r10, r9
	add	r8, r8, #1
	adds	r3, r3, #20
	str	r3, [sp, #12]
	b	.L214
.L227:
	ldr	r2, [sp, #12]
	add	r0, sp, #32
	adds	r3, r1, #1
	add	r4, r5, #848
	str	r3, [sp, #24]
	bl	set_st_cfg_to_frm
	movs	r3, #0
	mov	r1, r3
.L217:
	ldr	r2, [sp, #16]
	cmp	r3, r2
	blt	.L225
	add	r1, sp, #32
	mov	r0, r7
	bl	get_ref_from_cpb
	mov	r1, r0
	cbz	r0, .L226
	mov	r0, r7
	bl	check_ref_cpb_pos
	cmp	r0, #15
	bgt	.L226
	adds	r4, r0, #1
	cmp	r4, r6
	ble	.L226
	ldr	r3, [fp]
	lsls	r1, r3, #30
	bpl	.L232
	mov	r3, r4
	movw	r2, #661
	ldr	r1, .L253
	ldr	r0, .L253+28
	bl	printk
.L232:
	mov	r6, r4
.L226:
	add	r1, sp, #32
	mov	r0, r7
	bl	store_ref_to_cpb
	ldr	r1, [sp, #24]
	b	.L216
.L225:
	ldr	r2, [r4, #4]
	cbz	r2, .L218
	subs	r2, r2, #1
	str	r2, [r4, #4]
.L219:
	adds	r3, r3, #1
	adds	r4, r4, #36
	b	.L217
.L218:
	cbz	r1, .L220
.L223:
	movs	r1, #1
.L221:
	ldr	r2, [r4, #12]
	ldr	r0, [r4, #8]
	adds	r2, r2, #1
	str	r2, [r4, #12]
	cmp	r2, r0
	blt	.L219
	cmp	r0, #0
	beq	.L224
	movs	r2, #0
	str	r2, [r4, #12]
	ldr	r2, [r4, #16]
	adds	r2, r2, #1
.L252:
	str	r2, [r4, #16]
	b	.L219
.L220:
	ldr	r2, [r4, #12]
	cmp	r2, #0
	bne	.L221
	ldrb	r2, [sp, #32]	@ zero_extendqisi2
	ldr	r1, [r4, #24]
	and	r2, r2, #63
	orn	r2, r2, #127
	strb	r2, [sp, #32]
	ldrb	r2, [sp, #33]	@ zero_extendqisi2
	bfi	r2, r1, #4, #4
	ldr	r1, [r4, #20]
	bfi	r2, r1, #0, #4
	ldr	r1, [r4, #28]
	strb	r2, [sp, #33]
	cmp	r1, #25
	beq	.L222
	ldrb	r2, [sp, #34]	@ zero_extendqisi2
	bfi	r2, r1, #0, #6
	ldr	r1, [r4, #32]
	strb	r2, [sp, #34]
	ldrh	r2, [sp, #34]
	bfi	r2, r1, #6, #8
	strh	r2, [sp, #34]	@ movhi
.L222:
	ldr	r2, [fp]
	lsls	r0, r2, #29
	bpl	.L223
	ldr	r1, .L253+32
	mov	r2, #332
	add	r0, sp, #32
	str	r3, [sp, #28]
	bl	_dump_frm
	movs	r1, #1
	ldr	r3, [sp, #28]
	b	.L221
.L224:
	movs	r2, #1
	str	r2, [r4, #12]
	b	.L252
.L254:
	.align	2
.L253:
	.word	.LANCHOR0+265
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC43
	.word	.LC45
	.word	.LC37
	.word	.LC44
	.word	.LANCHOR0+285
	.fnend
	.size	mpp_enc_refs_dryrun, .-mpp_enc_refs_dryrun
	.align	1
	.global	mpp_enc_refs_set_usr_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_set_usr_cfg, %function
mpp_enc_refs_set_usr_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r1
	mov	r4, r0
	cbnz	r0, .L256
	mov	r2, #688
	ldr	r1, .L269
	ldr	r0, .L269+4
	bl	printk
	mvn	r0, #5
.L255:
	pop	{r4, r5, r6, pc}
.L256:
	ldr	r6, .L269+8
	ldr	r2, [r6]
	lsls	r3, r2, #31
	bpl	.L258
	mov	r3, r0
	mov	r2, #692
	ldr	r1, .L269
	ldr	r0, .L269+12
	bl	printk
.L258:
	add	r2, r4, #8
	add	r1, r5, #32
	mov	r3, r5
.L259:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L259
	ldr	r3, [r5]
	cbz	r3, .L260
	ldr	r3, [r4]
	orr	r3, r3, #2
	str	r3, [r4]
.L260:
	ldr	r0, [r6]
	ands	r0, r0, #1
	beq	.L255
	mov	r3, r4
	movw	r2, #699
	ldr	r1, .L269
	ldr	r0, .L269+16
	bl	printk
	movs	r0, #0
	b	.L255
.L270:
	.align	2
.L269:
	.word	.LANCHOR0+303
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_set_usr_cfg, .-mpp_enc_refs_set_usr_cfg
	.align	1
	.global	mpp_enc_refs_set_rc_igop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_set_rc_igop, %function
mpp_enc_refs_set_rc_igop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r1
	mov	r4, r0
	cbnz	r0, .L272
	movw	r2, #707
	ldr	r1, .L280
	ldr	r0, .L280+4
	bl	printk
	mvn	r0, #5
.L271:
	pop	{r4, r5, r6, pc}
.L272:
	ldr	r5, .L280+8
	ldr	r2, [r5]
	lsls	r3, r2, #31
	bpl	.L274
	mov	r3, r0
	movw	r2, #711
	ldr	r1, .L280
	ldr	r0, .L280+12
	bl	printk
.L274:
	ldr	r3, [r4, #40]
	cmp	r3, r6
	itttt	ne
	strne	r6, [r4, #40]
	ldrne	r3, [r4]
	orrne	r3, r3, #4
	strne	r3, [r4]
	ldr	r0, [r5]
	ands	r0, r0, #1
	beq	.L271
	mov	r3, r4
	mov	r2, #720
	ldr	r1, .L280
	ldr	r0, .L280+16
	bl	printk
	movs	r0, #0
	b	.L271
.L281:
	.align	2
.L280:
	.word	.LANCHOR0+328
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_set_rc_igop, .-mpp_enc_refs_set_rc_igop
	.align	1
	.global	mpp_enc_refs_update_hdr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_update_hdr, %function
mpp_enc_refs_update_hdr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	cbnz	r0, .L283
	movw	r2, #730
	ldr	r1, .L292
	ldr	r0, .L292+4
	mov	r6, r4
	bl	printk
.L282:
	mov	r0, r6
	pop	{r4, r5, r6, pc}
.L283:
	ldr	r5, .L292+8
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L285
	mov	r3, r0
	movw	r2, #734
	ldr	r1, .L292
	ldr	r0, .L292+12
	bl	printk
.L285:
	ldr	r3, [r5]
	ldr	r6, [r4, #44]
	lsls	r3, r3, #31
	bpl	.L282
	mov	r3, r4
	movw	r2, #739
	ldr	r1, .L292
	ldr	r0, .L292+16
	bl	printk
	b	.L282
.L293:
	.align	2
.L292:
	.word	.LANCHOR0+353
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_update_hdr, .-mpp_enc_refs_update_hdr
	.align	1
	.global	mpp_enc_refs_get_cpb_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_get_cpb_info, %function
mpp_enc_refs_get_cpb_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r1
	mov	r4, r0
	cbz	r0, .L295
	cbnz	r1, .L296
.L295:
	str	r5, [sp]
	mov	r3, r4
	movw	r2, #747
	ldr	r1, .L308
	ldr	r0, .L308+4
	bl	printk
	mvn	r0, #5
.L294:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L296:
	ldr	r6, .L308+8
	ldr	r2, [r6]
	lsls	r3, r2, #31
	bpl	.L298
	mov	r3, r0
	movw	r2, #751
	ldr	r1, .L308
	ldr	r0, .L308+12
	bl	printk
.L298:
	add	r3, r4, #48
	add	r2, r4, #76
.L299:
	ldr	r1, [r3], #4	@ unaligned
	cmp	r3, r2
	str	r1, [r5], #4	@ unaligned
	bne	.L299
	ldr	r0, [r6]
	ands	r0, r0, #1
	beq	.L294
	mov	r3, r4
	mov	r2, #756
	ldr	r1, .L308
	ldr	r0, .L308+16
	bl	printk
	movs	r0, #0
	b	.L294
.L309:
	.align	2
.L308:
	.word	.LANCHOR0+377
	.word	.LC46
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_get_cpb_info, .-mpp_enc_refs_get_cpb_info
	.align	1
	.global	mpp_enc_refs_get_cpb
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_get_cpb, %function
mpp_enc_refs_get_cpb:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r1
	.pad #20
	sub	sp, sp, #20
	mov	r4, r0
	cbnz	r0, .L311
	movw	r2, #789
	ldr	r1, .L379
	ldr	r0, .L379+4
	bl	printk
	mvn	r0, #5
.L310:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L311:
	ldr	r7, .L379+8
	ldr	r3, [r7]
	lsls	r6, r3, #31
	bpl	.L313
	mov	r3, r0
	movw	r2, #793
	ldr	r1, .L379
	ldr	r0, .L379+12
	bl	printk
.L313:
	ldr	r0, [r4, #40]
	add	r8, r4, #48
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	add	r9, r5, #8
	ldr	r1, [r4]
	add	r6, r4, #848
	ubfx	r3, r3, #2, #1
	ldr	fp, [r4, #4]
	str	r3, [sp, #12]
	ubfx	r3, r1, #2, #1
	cbz	r0, .L314
	ldr	r2, [r4, #1476]
	cmp	r0, r2
	it	le
	movle	r3, #1
.L314:
	ldr	r2, [r4, #8]
	lsls	r0, r2, #31
	bpl	.L315
	bic	r2, r2, #1
	str	r2, [r4, #8]
.L316:
	mov	r0, r8
	bl	cleanup_cpb_refs
.L318:
	movs	r3, #0
	str	r3, [r4]
	ldr	r3, [r4, #1472]
	adds	r3, r3, #1
	str	r3, [r4, #1472]
	ldr	r3, [r4, #1484]
	ldr	r2, [fp, #28]
	cmp	r3, r2
	blt	.L319
	cmp	r2, #1
	ite	le
	movle	r3, #0
	movgt	r3, #1
.L319:
	str	r3, [r4, #1484]
	movs	r1, #20
	ldr	r2, [fp, #36]
	mov	r0, r9
	mov	r10, #0
	mla	r3, r1, r3, r2
	ldr	r1, [r4, #1476]
	str	r3, [sp, #8]
	adds	r3, r1, #1
	ldr	r2, [sp, #8]
	str	r3, [r4, #1476]
	bl	set_st_cfg_to_frm
	mov	r1, r10
	movs	r3, #1
.L320:
	ldr	r2, [fp, #24]
	cmp	r2, r10
	bgt	.L328
	ldr	r3, [r4, #8]
	lsls	r0, r3, #28
	bpl	.L329
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	and	r3, r3, #63
	orn	r3, r3, #127
	strb	r3, [r5, #8]
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	ldr	r2, [r4, #24]
	bfi	r3, r2, #0, #4
	strb	r3, [r5, #9]
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	lsls	r1, r3, #26
	bpl	.L330
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	lsls	r2, r3, #28
	beq	.L330
	bfc	r3, #0, #4
	movw	r2, #871
	strb	r3, [r5, #9]
	ldr	r1, .L379
	ldr	r0, .L379+16
	bl	printk
.L330:
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	bfc	r3, #4, #4
	strb	r3, [r5, #9]
	movs	r3, #0
	str	r3, [r4, #1488]
	str	r3, [r4, #1484]
	ldr	r3, [r4, #8]
	bic	r3, r3, #8
	str	r3, [r4, #8]
.L329:
	ldr	r3, [sp, #12]
	cbz	r3, .L331
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	orr	r3, r3, #8
	strb	r3, [r5, #8]
.L331:
	ldr	r3, [r4, #8]
	lsls	r6, r3, #26
	bpl	.L332
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	ldr	r2, [r4, #32]
	bfi	r3, r2, #0, #6
	strb	r3, [r5, #10]
	ldrh	r3, [r5, #10]
	ldr	r2, [r4, #36]
	bfi	r3, r2, #6, #8
	strh	r3, [r5, #10]	@ movhi
	ldr	r3, [r4, #8]
	bic	r3, r3, #32
	str	r3, [r4, #8]
.L332:
	ldr	r3, [r4, #8]
	lsls	r0, r3, #30
	bpl	.L333
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	orr	r3, r3, #64
	strb	r3, [r5, #8]
	ldr	r3, [r4, #8]
	bic	r3, r3, #2
	str	r3, [r4, #8]
.L333:
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	lsls	r1, r3, #25
	bmi	.L340
	ldr	r3, [r4, #40]
	subs	r2, r3, #1
	rsbs	r3, r2, #0
	adcs	r3, r3, r2
.L334:
	ldrb	r2, [r5, #8]	@ zero_extendqisi2
	bfi	r2, r3, #1, #1
	strb	r2, [r5, #8]
	ldr	r2, [sp, #8]
	ldr	r3, [r4, #1488]
	adds	r3, r3, #1
	str	r3, [r4, #1488]
	ldr	r2, [r2, #16]
	cmp	r3, r2
	ble	.L335
	movs	r3, #0
	str	r3, [r4, #1488]
	ldr	r3, [r4, #1484]
	adds	r3, r3, #1
	str	r3, [r4, #1484]
.L335:
	mov	r1, r9
	mov	r0, r8
	bl	get_ref_from_cpb
	mov	r6, r0
	cmp	r0, #0
	beq	.L336
	mov	r1, r0
	mov	r0, r8
	bl	check_ref_cpb_pos
	cmp	r0, #0
	bge	.L337
	movw	r1, #913
	ldr	r3, .L379+20
	str	r1, [sp]
	ldr	r2, .L379+24
	ldr	r0, .L379+28
	bl	printk
.L337:
	ldrd	r2, [r5, #16]
	add	r1, r4, #1424
	strd	r2, [r1]
	ldrd	r2, [r6]
.L378:
	strd	r2, [r5, #16]
	ldr	r3, [r7]
	lsls	r3, r3, #29
	bpl	.L339
	mov	r2, #920
	ldr	r1, .L379
	ldr	r0, .L379+32
	bl	printk
	movw	r2, #921
	ldr	r1, .L379+20
	mov	r0, r9
	bl	_dump_frm
	movw	r2, #922
	ldr	r1, .L379
	ldr	r0, .L379+36
	bl	printk
	movw	r2, #923
	ldr	r1, .L379+20
	add	r0, r5, #16
	bl	_dump_frm
.L339:
	add	r3, r5, #24
	movs	r2, #64
	movs	r1, #0
	adds	r5, r5, #88
	mov	r0, r3
	bl	memset
	mov	r1, r0
	mov	r0, r8
	bl	save_cpb_status
	mov	r1, r9
	mov	r0, r8
	bl	store_ref_to_cpb
	movs	r2, #64
	movs	r1, #0
	mov	r0, r5
	bl	memset
	mov	r0, r8
	mov	r1, r5
	bl	save_cpb_status
	ldr	r0, [r7]
	ands	r0, r0, #1
	beq	.L310
	mov	r3, r4
	movw	r2, #938
	ldr	r1, .L379
	ldr	r0, .L379+40
	bl	printk
	movs	r0, #0
	b	.L310
.L315:
	cmp	r3, #0
	bne	.L316
	lsls	r1, r1, #31
	itt	mi
	strmi	r3, [r4, #1484]
	strmi	r3, [r4, #1488]
	b	.L318
.L328:
	ldr	r2, [r6, #4]
	cbz	r2, .L321
	subs	r2, r2, #1
	str	r2, [r6, #4]
.L322:
	add	r10, r10, #1
	adds	r6, r6, #36
	b	.L320
.L321:
	cbz	r1, .L323
.L326:
	movs	r1, #1
.L324:
	ldr	r2, [r6, #12]
	ldr	r0, [r6, #8]
	adds	r2, r2, #1
	str	r2, [r6, #12]
	cmp	r2, r0
	blt	.L322
	cmp	r0, #0
	beq	.L327
	movs	r2, #0
	str	r2, [r6, #12]
	ldr	r2, [r6, #16]
	adds	r2, r2, #1
	str	r2, [r6, #16]
	b	.L322
.L380:
	.align	2
.L379:
	.word	.LANCHOR0+403
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC47
	.word	.LANCHOR0+424
	.word	.LC48
	.word	.LC13
	.word	.LC49
	.word	.LC50
	.word	.LC37
.L323:
	ldr	r2, [r6, #12]
	cmp	r2, #0
	bne	.L324
	ldrb	r2, [r5, #8]	@ zero_extendqisi2
	and	r2, r2, #63
	orn	r2, r2, #127
	strb	r2, [r5, #8]
	ldrb	r2, [r5, #9]	@ zero_extendqisi2
	ldr	r1, [r6, #24]
	bfi	r2, r1, #4, #4
	strb	r2, [r5, #9]
	ldr	r1, [r6, #20]
	bfi	r2, r1, #0, #4
	strb	r2, [r5, #9]
	ldr	r1, [r6, #28]
	cmp	r1, #25
	beq	.L325
	ldrb	r2, [r5, #10]	@ zero_extendqisi2
	bfi	r2, r1, #0, #6
	strb	r2, [r5, #10]
	ldr	r1, [r6, #32]
	ldrh	r2, [r5, #10]
	bfi	r2, r1, #6, #8
	strh	r2, [r5, #10]	@ movhi
.L325:
	ldr	r2, [r7]
	lsls	r2, r2, #29
	bpl	.L326
	ldr	r1, .L381
	mov	r2, #332
	mov	r0, r9
	bl	_dump_frm
	movs	r1, #1
	mov	r3, r1
	b	.L324
.L327:
	strd	r3, r3, [r6, #12]
	b	.L322
.L340:
	movs	r3, #1
	b	.L334
.L336:
	movs	r2, #0
	movs	r3, #0
	b	.L378
.L382:
	.align	2
.L381:
	.word	.LANCHOR0+285
	.fnend
	.size	mpp_enc_refs_get_cpb, .-mpp_enc_refs_get_cpb
	.align	1
	.global	mpp_enc_refs_next_frm_is_intra
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_next_frm_is_intra, %function
mpp_enc_refs_next_frm_is_intra:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	cbnz	r0, .L384
	movw	r2, #949
	ldr	r1, .L401
	ldr	r0, .L401+4
	mvn	r4, #5
	bl	printk
.L383:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L384:
	ldr	r6, .L401+8
	ldr	r2, [r6]
	lsls	r2, r2, #31
	bpl	.L386
	mov	r3, r0
	movw	r2, #953
	ldr	r1, .L401
	ldr	r0, .L401+12
	bl	printk
.L386:
	ldr	r3, [r5, #40]
	ldr	r4, [r5]
	ubfx	r4, r4, #2, #1
	cbz	r3, .L387
	ldr	r2, [r5, #1476]
	cmp	r3, r2
	it	le
	movle	r4, #1
.L387:
	ldr	r3, [r5, #8]
	tst	r3, #1
	ldr	r3, [r5, #1472]
	it	ne
	movne	r4, #1
	cmp	r3, #0
	ldr	r3, [r6]
	it	eq
	moveq	r4, #0
	lsls	r3, r3, #31
	bpl	.L383
	mov	r3, r5
	movw	r2, #966
	ldr	r1, .L401
	ldr	r0, .L401+16
	bl	printk
	b	.L383
.L402:
	.align	2
.L401:
	.word	.LANCHOR0+445
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_next_frm_is_intra, .-mpp_enc_refs_next_frm_is_intra
	.global	__aeabi_idivmod
	.align	1
	.global	mpp_enc_refs_next_frm_is_kpfrm
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_next_frm_is_kpfrm, %function
mpp_enc_refs_next_frm_is_kpfrm:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	cbnz	r0, .L404
	movw	r2, #979
	ldr	r1, .L417
	ldr	r0, .L417+4
	mvn	r4, #5
	bl	printk
.L403:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L404:
	ldr	r6, .L417+8
	ldr	r2, [r6]
	lsls	r2, r2, #31
	bpl	.L406
	mov	r3, r0
	movw	r2, #983
	ldr	r1, .L417
	ldr	r0, .L417+12
	bl	printk
.L406:
	ldr	r4, [r5, #68]
	cbz	r4, .L407
	ldr	r0, [r5, #1476]
	ldr	r1, [r5, #72]
	adds	r0, r0, #1
	bl	__aeabi_idivmod
	clz	r4, r1
	lsrs	r4, r4, #5
.L407:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L403
	mov	r3, r5
	mov	r2, #1000
	ldr	r1, .L417
	ldr	r0, .L417+16
	bl	printk
	b	.L403
.L418:
	.align	2
.L417:
	.word	.LANCHOR0+476
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_next_frm_is_kpfrm, .-mpp_enc_refs_next_frm_is_kpfrm
	.align	1
	.global	mpp_enc_refs_get_cpb_pass1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_get_cpb_pass1, %function
mpp_enc_refs_get_cpb_pass1:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r5, r0
	mov	r4, r1
	add	r7, r0, #48
	add	r8, r1, #8
	cbnz	r0, .L420
	movw	r2, #1014
	ldr	r1, .L435
	ldr	r0, .L435+4
	bl	printk
	mvn	r0, #5
.L419:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L420:
	ldr	r6, .L435+8
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L422
	mov	r3, r0
	movw	r2, #1018
	ldr	r1, .L435
	ldr	r0, .L435+12
	bl	printk
.L422:
	ldr	r3, [r4, #8]
	mov	r1, r8
	mov	r0, r7
	bic	r3, r3, #1069547520
	bic	r3, r3, #4177920
	bic	r3, r3, #12480
	bic	r3, r3, #7
	orr	r3, r3, #69
	str	r3, [r4, #8]
	bl	get_ref_from_cpb
	mov	r9, r0
	cmp	r0, #0
	beq	.L423
	mov	r1, r0
	mov	r0, r7
	bl	check_ref_cpb_pos
	cmp	r0, #0
	bge	.L424
	movw	r1, #1033
	ldr	r3, .L435+16
	str	r1, [sp]
	ldr	r2, .L435+20
	ldr	r0, .L435+24
	bl	printk
.L424:
	ldrd	r2, [r4, #16]
	add	r1, r5, #1424
	strd	r2, [r1]
	ldrd	r2, [r9]
.L434:
	strd	r2, [r4, #16]
	ldr	r3, [r6]
	lsls	r3, r3, #29
	bpl	.L426
	mov	r2, #1040
	ldr	r1, .L435
	ldr	r0, .L435+28
	bl	printk
	movw	r2, #1041
	ldr	r1, .L435+16
	mov	r0, r8
	bl	_dump_frm
	movw	r2, #1042
	ldr	r1, .L435
	ldr	r0, .L435+32
	bl	printk
	movw	r2, #1043
	ldr	r1, .L435+16
	add	r0, r4, #16
	bl	_dump_frm
.L426:
	add	r3, r4, #24
	movs	r2, #64
	movs	r1, #0
	adds	r4, r4, #88
	mov	r0, r3
	bl	memset
	mov	r1, r0
	mov	r0, r7
	bl	save_cpb_status
	mov	r1, r8
	mov	r0, r7
	bl	store_ref_to_cpb
	movs	r2, #64
	movs	r1, #0
	mov	r0, r4
	bl	memset
	mov	r0, r7
	mov	r1, r4
	bl	save_cpb_status
	ldr	r0, [r6]
	ands	r0, r0, #1
	beq	.L419
	mov	r3, r5
	movw	r2, #1058
	ldr	r1, .L435
	ldr	r0, .L435+36
	bl	printk
	movs	r0, #0
	b	.L419
.L423:
	movs	r2, #0
	movs	r3, #0
	b	.L434
.L436:
	.align	2
.L435:
	.word	.LANCHOR0+507
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LANCHOR0+534
	.word	.LC48
	.word	.LC13
	.word	.LC49
	.word	.LC50
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_get_cpb_pass1, .-mpp_enc_refs_get_cpb_pass1
	.align	1
	.global	mpp_enc_refs_stash
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_stash, %function
mpp_enc_refs_stash:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	cbnz	r0, .L438
	movw	r2, #1067
	ldr	r1, .L445
	ldr	r0, .L445+4
	bl	printk
	mvn	r0, #5
.L437:
	pop	{r3, r4, r5, pc}
.L438:
	ldr	r5, .L445+8
	ldr	r2, [r5]
	lsls	r3, r2, #31
	bpl	.L440
	mov	r3, r0
	movw	r2, #1071
	ldr	r1, .L445
	ldr	r0, .L445+12
	bl	printk
.L440:
	mov	r2, #1448
	add	r1, r4, #48
	add	r0, r4, #1496
	bl	memcpy
	ldr	r0, [r5]
	ands	r0, r0, #1
	beq	.L437
	mov	r3, r4
	movw	r2, #1076
	ldr	r1, .L445
	ldr	r0, .L445+16
	bl	printk
	movs	r0, #0
	b	.L437
.L446:
	.align	2
.L445:
	.word	.LANCHOR0+561
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_stash, .-mpp_enc_refs_stash
	.align	1
	.global	mpp_enc_refs_rollback
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_refs_rollback, %function
mpp_enc_refs_rollback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	cbnz	r0, .L448
	movw	r2, #1084
	ldr	r1, .L455
	ldr	r0, .L455+4
	bl	printk
	mvn	r0, #5
.L447:
	pop	{r3, r4, r5, pc}
.L448:
	ldr	r5, .L455+8
	ldr	r2, [r5]
	lsls	r3, r2, #31
	bpl	.L450
	mov	r3, r0
	mov	r2, #1088
	ldr	r1, .L455
	ldr	r0, .L455+12
	bl	printk
.L450:
	mov	r2, #1448
	add	r1, r4, #1496
	add	r0, r4, #48
	bl	memcpy
	ldr	r0, [r5]
	ands	r0, r0, #1
	beq	.L447
	mov	r3, r4
	movw	r2, #1093
	ldr	r1, .L455
	ldr	r0, .L455+16
	bl	printk
	movs	r0, #0
	b	.L447
.L456:
	.align	2
.L455:
	.word	.LANCHOR0+580
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC37
	.fnend
	.size	mpp_enc_refs_rollback, .-mpp_enc_refs_rollback
	.global	enc_refs_debug
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.8991, %object
	.size	__func__.8991, 17
__func__.8991:
	.ascii	"get_ref_from_cpb\000"
	.type	__FUNCTION__.8968, %object
	.size	__FUNCTION__.8968, 18
__FUNCTION__.8968:
	.ascii	"set_st_cfg_to_frm\000"
	.type	__FUNCTION__.9055, %object
	.size	__FUNCTION__.9055, 17
__FUNCTION__.9055:
	.ascii	"store_ref_to_cpb\000"
	.type	__func__.9068, %object
	.size	__func__.9068, 17
__func__.9068:
	.ascii	"store_ref_to_cpb\000"
	.type	__func__.9029, %object
	.size	__func__.9029, 16
__func__.9029:
	.ascii	"save_cpb_status\000"
	.type	__FUNCTION__.9022, %object
	.size	__FUNCTION__.9022, 16
__FUNCTION__.9022:
	.ascii	"save_cpb_status\000"
	.type	__FUNCTION__.8912, %object
	.size	__FUNCTION__.8912, 10
__FUNCTION__.8912:
	.ascii	"_dump_cpb\000"
	.type	__func__.8999, %object
	.size	__func__.8999, 18
__func__.8999:
	.ascii	"check_ref_cpb_pos\000"
	.type	__FUNCTION__.9009, %object
	.size	__FUNCTION__.9009, 18
__FUNCTION__.9009:
	.ascii	"check_ref_cpb_pos\000"
	.type	__func__.8929, %object
	.size	__func__.8929, 18
__func__.8929:
	.ascii	"mpp_enc_refs_init\000"
	.type	__FUNCTION__.8930, %object
	.size	__FUNCTION__.8930, 18
__FUNCTION__.8930:
	.ascii	"mpp_enc_refs_init\000"
	.type	__func__.8935, %object
	.size	__func__.8935, 20
__func__.8935:
	.ascii	"mpp_enc_refs_deinit\000"
	.type	__FUNCTION__.8936, %object
	.size	__FUNCTION__.8936, 20
__FUNCTION__.8936:
	.ascii	"mpp_enc_refs_deinit\000"
	.type	__func__.8946, %object
	.size	__func__.8946, 21
__func__.8946:
	.ascii	"mpp_enc_refs_set_cfg\000"
	.type	__FUNCTION__.8945, %object
	.size	__FUNCTION__.8945, 21
__FUNCTION__.8945:
	.ascii	"mpp_enc_refs_set_cfg\000"
	.type	__func__.9088, %object
	.size	__func__.9088, 20
__func__.9088:
	.ascii	"mpp_enc_refs_dryrun\000"
	.type	__FUNCTION__.8973, %object
	.size	__FUNCTION__.8973, 18
__FUNCTION__.8973:
	.ascii	"set_lt_cfg_to_frm\000"
	.type	__func__.9112, %object
	.size	__func__.9112, 25
__func__.9112:
	.ascii	"mpp_enc_refs_set_usr_cfg\000"
	.type	__func__.9118, %object
	.size	__func__.9118, 25
__func__.9118:
	.ascii	"mpp_enc_refs_set_rc_igop\000"
	.type	__func__.9124, %object
	.size	__func__.9124, 24
__func__.9124:
	.ascii	"mpp_enc_refs_update_hdr\000"
	.type	__func__.9130, %object
	.size	__func__.9130, 26
__func__.9130:
	.ascii	"mpp_enc_refs_get_cpb_info\000"
	.type	__func__.9154, %object
	.size	__func__.9154, 21
__func__.9154:
	.ascii	"mpp_enc_refs_get_cpb\000"
	.type	__FUNCTION__.9160, %object
	.size	__FUNCTION__.9160, 21
__FUNCTION__.9160:
	.ascii	"mpp_enc_refs_get_cpb\000"
	.type	__func__.9170, %object
	.size	__func__.9170, 31
__func__.9170:
	.ascii	"mpp_enc_refs_next_frm_is_intra\000"
	.type	__func__.9178, %object
	.size	__func__.9178, 31
__func__.9178:
	.ascii	"mpp_enc_refs_next_frm_is_kpfrm\000"
	.type	__func__.9188, %object
	.size	__func__.9188, 27
__func__.9188:
	.ascii	"mpp_enc_refs_get_cpb_pass1\000"
	.type	__FUNCTION__.9190, %object
	.size	__FUNCTION__.9190, 27
__FUNCTION__.9190:
	.ascii	"mpp_enc_refs_get_cpb_pass1\000"
	.type	__func__.9197, %object
	.size	__func__.9197, 19
__func__.9197:
	.ascii	"mpp_enc_refs_stash\000"
	.type	__func__.9202, %object
	.size	__func__.9202, 22
__func__.9202:
	.ascii	"mpp_enc_refs_rollback\000"
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	enc_refs_debug, %object
	.size	enc_refs_debug, 4
enc_refs_debug:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"lt\000"
.LC1:
	.ascii	"st\000"
.LC2:
	.ascii	"\0013%s:%d: frm %d not supported ref mode 0x%x\012\000"
.LC3:
	.ascii	"\0016%s:%d: frm %d ref mode %d arg %d -> seq %d %s "
	.ascii	"idx %d\012\000"
.LC4:
	.ascii	"\0013%s:%d: frm %d found mode %d arg %d -> ref %d b"
	.ascii	"ut it is invalid\012\000"
.LC5:
	.ascii	"intra\000"
.LC6:
	.ascii	"inter\000"
.LC7:
	.ascii	"\0016%d: %s:%d valid %d frm %d %s tid %d non-ref ->"
	.ascii	" [%x:%d]\012\000"
.LC8:
	.ascii	"\0016%d: %s:%d valid %d frm %d %s tid %d lt-ref  ->"
	.ascii	" [%x:%d] lt_idx %d\012\000"
.LC9:
	.ascii	"\0016%d: %s:%d valid %d frm %d %s tid %d st-ref  ->"
	.ascii	" [%x:%d]\012\000"
.LC10:
	.ascii	"add\000"
.LC11:
	.ascii	"replace\000"
.LC12:
	.ascii	"frm->valid\000"
.LC13:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC14:
	.ascii	"\0016%s:%d: frm %d with lt idx %d %s to pos %d\012\000"
.LC15:
	.ascii	"\0016%s:%d: dumping cpb refs status start\012\000"
.LC16:
	.ascii	"\0016%s:%d: dumping cpb refs status done\012\000"
.LC17:
	.ascii	"\0013%s:%d: frm %d with lt idx %d found no place to"
	.ascii	" add or relace\012\000"
.LC18:
	.ascii	"\0016%s:%d: save lt ref %d to slot %d\012\000"
.LC19:
	.ascii	"\0016%s:%d: save st ref %d to slot %d\012\000"
.LC20:
	.ascii	"\0016%s:%d: save ref total %d lt %d st %d\012\000"
.LC21:
	.ascii	"\0016%d: %s:%d cpb %p status:\012\000"
.LC22:
	.ascii	"\0016%d: cpb info: dpb_size %d max_lt/st cnt [%d:%d"
	.ascii	"] \012\000"
.LC23:
	.ascii	"\0016%d: cpb info: max_lt_idx %d max_st_tid %d\012\000"
.LC24:
	.ascii	"\0016%d: cpb info: lt_gop %d st_gop %d\012\000"
.LC25:
	.ascii	"\0016%d: cpb cpb_refs:\012\000"
.LC26:
	.ascii	"\0016%d: cpb mode_refs:\012\000"
.LC27:
	.ascii	"\0016%d: cpb st_tid_refs:\012\000"
.LC28:
	.ascii	"\0016%d: cpb lt_idx_refs:\012\000"
.LC29:
	.ascii	"\0016%d: cpb runtime: frm_idx %d seq_idx %d seq_cnt"
	.ascii	" %d st_cfg [%d:%d]\012\000"
.LC30:
	.ascii	"\0016%s:%d: frm %d is not valid ref frm\012\000"
.LC31:
	.ascii	"\0016%s:%d: matching ref %d at pos %d %d\012\000"
.LC32:
	.ascii	"\0016%s:%d: found ref %d at pos %d\012\000"
.LC33:
	.ascii	"\0013%s:%d: frm %d can NOT be found in st refs!!\012"
	.ascii	"\000"
.LC34:
	.ascii	"\0013%s:%d: invalid NULL input refs\012\000"
.LC35:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC36:
	.ascii	"\0013%s:%d: create refs_impl failed\012\000"
.LC37:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC38:
	.ascii	"\0013%s:%d: invalid input refs %p ref_cfg %p\012\000"
.LC39:
	.ascii	"\0016%s:%d: enter %p cfg %p\012\000"
.LC40:
	.ascii	"cfg->lt_cfg_cnt < 16\000"
.LC41:
	.ascii	"\0016%s:%d: ref_cfg cpb size: lt %d st %d max lt_id"
	.ascii	"x %d tid %d\012\000"
.LC42:
	.ascii	"\0016%s:%d: leave %p cfg %p\012\000"
.LC43:
	.ascii	"\0016%s:%d: dryrun start: lt_cfg %d st_cfg %d\012\000"
.LC44:
	.ascii	"\0016%s:%d: cpb_st_used_size update to %d\012\000"
.LC45:
	.ascii	"\0016%s:%d: dryrun success: cpb size %d\012\000"
.LC46:
	.ascii	"\0013%s:%d: invalid input refs %p info %p\012\000"
.LC47:
	.ascii	"\0013%s:%d: can not set IDR to ltr with non-zero in"
	.ascii	"dex\012\000"
.LC48:
	.ascii	"cpb_idx >= 0\000"
.LC49:
	.ascii	"\0016%s:%d: frm status:\012\000"
.LC50:
	.ascii	"\0016%s:%d: ref status:\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
