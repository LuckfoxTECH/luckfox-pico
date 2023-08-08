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
	.file	"h265e_syntax.c"
	.text
	.syntax unified
	.syntax unified
	.global	__aeabi_idivmod
	.thumb
	.syntax unified
	.align	1
	.global	fill_ref_parameters
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	fill_ref_parameters, %function
fill_ref_parameters:
	.fnstart
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 0, uses_anonymous_args = 0
	movw	r3, #18996
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	ldr	r8, [r0, r3]
	movs	r3, #0
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	mov	r9, r0
	.pad #84
	sub	sp, sp, #84
	ldr	r5, [r8, #520]
	and	r2, r2, #95
	strh	r3, [r1, #36]	@ movhi
	strb	r2, [r1, #2]
	add	r0, r5, #348
	str	r3, [r1, #60]
	ldr	r2, [r5, #340]
	strb	r2, [r1, #24]
	ldrb	r2, [r1, #2]	@ zero_extendqisi2
	ldr	r1, [r5, #344]
	bfi	r2, r1, #6, #1
	strb	r2, [r4, #2]
	mov	r2, r3
.L2:
	ldr	r1, [r5, #340]
	cmp	r1, r2
	bgt	.L7
	add	r1, r5, #476
	movs	r3, #0
.L8:
	ldr	fp, [r5, #336]
	cmp	fp, r3
	bgt	.L10
	ldr	r6, [r8, #372]
	ldrb	r3, [r6, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L11
	ldr	r3, [r5, #672]
	add	r7, fp, #102
	add	r10, fp, #-1
	movs	r1, #0
	add	r7, r5, r7, lsl #2
	strd	r1, r1, [sp, #32]
	str	r3, [sp, #16]
	ldrd	r2, r3, [r5, #340]
	strd	r1, r1, [sp, #40]
	adds	r3, r2, r3
	str	r3, [sp, #20]
	ldr	r3, [r6, #164]
	strd	r1, r1, [sp, #48]
	strd	r1, r1, [sp, #56]
	str	r3, [sp, #12]
	ldr	r3, [sp, #16]
	strd	r1, r1, [sp, #64]
	sub	r3, r10, r3
	strd	r1, r1, [sp, #72]
	str	r3, [sp, #24]
	add	r3, r6, #12
	str	r1, [sp, #8]
	str	r3, [sp, #28]
.L12:
	ldr	r3, [sp, #24]
	cmp	r3, r10
	blt	.L17
	ldr	r3, [sp, #16]
	movs	r0, #1
	ldr	r2, [sp, #8]
	subs	r2, r3, r2
	movs	r3, #0
.L18:
	ldr	r6, [sp, #12]
	lsl	r1, r0, r3
	cmp	r1, r6
	bcc	.L19
	cbz	r6, .L20
	ldr	r3, [sp, #8]
	strb	r3, [r4, #22]
.L20:
	strb	r2, [r4, #20]
	movs	r3, #0
	ldrb	r2, [r4, #3]	@ zero_extendqisi2
	strh	r3, [r4, #30]	@ movhi
	bic	r2, r2, #31
	strh	r3, [r4, #46]	@ movhi
	strb	r2, [r4, #3]
	mov	r2, r3
	str	r3, [r4, #48]
	strh	r3, [r4, #52]	@ movhi
	ldr	r6, [r5, #336]
	add	r7, r6, #35
	add	r10, r6, #-1
	sub	r6, fp, r6
	add	r7, r5, r7, lsl #2
	lsls	r6, r6, #2
.L21:
	ldr	r3, [sp, #20]
	subs	r3, r3, #1
	cmp	r3, r10
	blt	.L26
	ldr	r3, [sp, #32]
	ldr	r2, [sp, #44]
	ldr	r1, [sp, #56]
	strh	r3, [r4, #30]	@ movhi
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	bfi	r3, r2, #2, #1
	ldrb	r2, [r4, #2]	@ zero_extendqisi2
	bfi	r2, r1, #7, #1
	strb	r2, [r4, #2]
	ldr	r2, [sp, #68]
	strh	r2, [r4, #36]	@ movhi
	ldr	r2, [sp, #36]
	strh	r2, [r4, #46]	@ movhi
	ldr	r2, [sp, #48]
	bfi	r3, r2, #3, #1
	ldr	r2, [sp, #60]
	bfi	r3, r2, #0, #1
	ldr	r2, [sp, #72]
	strh	r2, [r4, #50]	@ movhi
	ldr	r2, [sp, #40]
	strh	r2, [r4, #48]	@ movhi
	ldr	r2, [sp, #52]
	bfi	r3, r2, #4, #1
	ldr	r2, [sp, #64]
	bfi	r3, r2, #1, #1
	strb	r3, [r4, #3]
	ldr	r3, [sp, #76]
	strh	r3, [r4, #52]	@ movhi
.L11:
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	movs	r3, #0
	strb	r3, [r4, #13]
	bfi	r2, r3, #0, #1
	strb	r2, [r4, #1]
	ldr	r0, [r8, #368]
	cmp	r0, #2
	beq	.L27
	ldrd	r2, r1, [r5, #340]
	add	r5, r5, #412
	add	r2, r2, r1
	ldr	r1, [r5, #260]
	add	r2, r2, r1
	mov	r1, r3
.L28:
	cmp	r1, r2
	blt	.L30
.L27:
	ldr	r2, [r8, #376]
	ldrb	r2, [r2, #17]	@ zero_extendqisi2
	cbz	r2, .L32
	cmp	r3, #1
	bls	.L32
	cmp	r0, #2
	beq	.L32
	ldr	r3, [r8, #1200]
	adds	r2, r3, #0
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	it	ne
	movne	r2, #1
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #1]
	lsls	r3, r3, #31
	itt	mi
	ldrmi	r3, [r8, #1208]
	strbmi	r3, [r4, #13]
.L32:
	movw	r3, #19000
	ldr	r2, [r9, r3]
	ldr	r2, [r2, #876]
	ldr	r2, [r2, #4]
	strb	r2, [r4, #4]
	ldr	r2, [r9, r3]
	ldr	r2, [r2, #876]
	ldrb	r2, [r2, #24]	@ zero_extendqisi2
	ubfx	r2, r2, #7, #1
	strb	r2, [r4, #5]
	ldr	r2, [r8, #384]
	cmp	r2, #0
	beq	.L34
	ldr	r3, [r2, #4]
	strb	r3, [r4, #6]
	ldrb	r3, [r2, #24]	@ zero_extendqisi2
	ubfx	r3, r3, #7, #1
.L62:
	movs	r0, #0
	strb	r3, [r4, #7]
	add	sp, sp, #84
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L7:
	cbnz	r2, .L3
	ldr	r1, [r5, #348]
	mvns	r1, r1
	add	r3, r3, r1
	strh	r3, [r4, #38]	@ movhi
	ldr	r3, [r5, #476]
.L61:
	strb	r3, [r4, #23]
.L4:
	ldr	r3, [r0], #4
	adds	r2, r2, #1
	b	.L2
.L3:
	cmp	r2, #1
	bne	.L5
	ldr	r1, [r5, #352]
	mvns	r1, r1
	add	r3, r3, r1
	strh	r3, [r4, #40]	@ movhi
	ldr	r1, [r5, #480]
	ldrb	r3, [r4, #23]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #1
	b	.L61
.L5:
	cmp	r2, #2
	bne	.L6
	ldr	r1, [r5, #356]
	mvns	r1, r1
	add	r3, r3, r1
	strh	r3, [r4, #42]	@ movhi
	ldr	r1, [r5, #484]
	ldrb	r3, [r4, #23]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #2
	b	.L61
.L6:
	cmp	r2, #3
	bne	.L4
	ldr	r1, [r5, #360]
	mvns	r1, r1
	add	r3, r3, r1
	strh	r3, [r4, #44]	@ movhi
	ldr	r1, [r5, #488]
	ldrb	r3, [r4, #23]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #3
	b	.L61
.L10:
	ldr	r2, [r1], #4
	cbz	r2, .L9
	ldr	r2, [r4, #60]
	adds	r2, r2, #1
	str	r2, [r4, #60]
.L9:
	adds	r3, r3, #1
	b	.L8
.L17:
	ldr	r1, [r6, #160]
	movs	r3, #1
	ldr	r0, [r7, #128]
	lsl	r1, r3, r1
	bl	__aeabi_idivmod
	add	r0, r6, #168
	movs	r3, #0
.L13:
	ldr	r2, [sp, #12]
	cmp	r2, r3
	bgt	.L16
.L36:
	add	r10, r10, #-1
	subs	r7, r7, #4
	b	.L12
.L16:
	ldr	r2, [r0], #4
	cmp	r2, r1
	bne	.L14
	ldr	r2, [sp, #28]
	ldrb	ip, [r2, r3]	@ zero_extendqisi2
	ldr	r2, [r7]
	cmp	r2, ip
	beq	.L15
.L14:
	adds	r3, r3, #1
	b	.L13
.L19:
	adds	r3, r3, #1
	b	.L18
.L26:
	ldr	r3, [r5, #336]
	subs	r3, r3, #1
	cmp	r3, r10
	beq	.L37
	ldr	r1, [sp, #8]
	subs	r3, r3, r1
	sub	r1, r3, r10
	rsbs	r3, r1, #0
	adcs	r3, r3, r1
.L22:
	ldr	r0, [r7], #-4
	add	r1, sp, #32
	str	r0, [r6, r1]
	add	r1, sp, #44
	ldr	r0, [r7, #336]
	str	r0, [r6, r1]
	add	r0, sp, #56
	ldr	r1, [r7, #132]
	str	r1, [r6, r0]
	cbz	r1, .L23
	ldr	r1, [r7, #68]
	cbz	r3, .L24
	add	r3, sp, #68
	str	r1, [r6, r3]
.L23:
	ldr	r2, [r7, #68]
	add	r10, r10, #-1
	adds	r6, r6, #4
	b	.L21
.L37:
	movs	r3, #1
	b	.L22
.L24:
	subs	fp, r1, r2
	bpl	.L25
	movw	r1, #345
	ldr	r3, .L63
	str	r1, [sp]
	ldr	r2, .L63+4
	ldr	r0, .L63+8
	bl	printk
.L25:
	add	r3, sp, #68
	str	fp, [r6, r3]
	b	.L23
.L30:
	ldr	r6, [r5], #4
	cbz	r6, .L29
	adds	r3, r3, #1
.L29:
	adds	r1, r1, #1
	b	.L28
.L34:
	ldr	r2, [r9, r3]
	ldr	r2, [r2, #876]
	ldr	r2, [r2, #4]
	strb	r2, [r4, #6]
	ldr	r3, [r9, r3]
	ldr	r3, [r3, #876]
	ldr	r3, [r3, #40]
	b	.L62
.L15:
	ldr	r3, [sp, #8]
	adds	r3, r3, #1
	str	r3, [sp, #8]
	b	.L36
.L64:
	.align	2
.L63:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.fnend
	.size	fill_ref_parameters, .-fill_ref_parameters
	.align	1
	.global	h265e_syntax_fill
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_syntax_fill, %function
h265e_syntax_fill:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	add	r7, r0, #18944
	ldr	r8, [r0]
	adds	r7, r7, #76
	mov	r6, r0
	movs	r2, #224
	movs	r1, #0
	adds	r0, r7, #4
	bl	memset
	ldr	r2, [r8, #12]
	movw	r3, #19024
	adds	r5, r6, r3
	add	r4, r6, #18176
	movw	r1, #4616
	strh	r2, [r6, r3]	@ movhi
	adds	r4, r4, #92
	ldr	r3, [r8, #16]
	add	r9, r6, r1
	strh	r3, [r5, #2]	@ movhi
	ldr	r3, [r8, #20]
	strh	r3, [r5, #4]	@ movhi
	ldr	r3, [r8, #24]
	strh	r3, [r5, #6]	@ movhi
	movw	r3, #18996
	ldr	r3, [r6, r3]
	ldrb	r3, [r3, #4]	@ zero_extendqisi2
	str	r3, [r5, #212]
	ldr	r3, [r4, #28]
	str	r3, [r5, #216]
	ldr	r3, [r9, #52]
	str	r3, [r5, #208]
	ldr	r3, [r8, #36]
	str	r3, [r5, #12]
	ldrd	r2, r3, [r9, #144]
	lsls	r3, r3, #6
	orr	r3, r3, r2, lsl #3
	ldr	r2, [r9, #56]
	orrs	r3, r3, r2
	ldrb	r2, [r6, r1]	@ zero_extendqisi2
	orr	r3, r3, r2, lsl #2
	ldr	r2, [r9, #160]
	subs	r2, r2, #4
	orr	r3, r3, r2, lsl #9
	strh	r3, [r5, #16]	@ movhi
	ldr	r3, [r9, #60]
	add	r3, r9, r3, lsl #2
	ldr	r3, [r3, #300]
	strb	r3, [r5, #18]
	ldr	r3, [r9, #72]
	strb	r3, [r5, #19]
	ldr	r3, [r9, #76]
	strb	r3, [r5, #20]
	ldr	r3, [r9, #124]
	strb	r3, [r5, #21]
	ldr	r3, [r9, #120]
	strb	r3, [r5, #22]
	ldr	r3, [r9, #128]
	strb	r3, [r5, #23]
	ldr	r3, [r9, #132]
	strb	r3, [r5, #24]
	movw	r3, #4452
	ldr	r3, [r9, r3]
	strb	r3, [r5, #25]
	ldr	r3, [r9, #164]
	strb	r3, [r5, #26]
	ldr	r3, [r4, #48]
	ldrb	r8, [r9, #9]	@ zero_extendqisi2
	subs	r3, r3, #1
	strb	r3, [r5, #27]
	ldr	r3, [r4, #52]
	lsl	r8, r8, #3
	subs	r3, r3, #1
	strb	r3, [r5, #28]
	ldr	r3, [r4, #32]
	strb	r3, [r5, #29]
	ldrb	r2, [r9, #3]	@ zero_extendqisi2
	ldrb	r1, [r9, #4]	@ zero_extendqisi2
	orr	r8, r8, r2, lsl #2
	ldrb	r2, [r9, #10]	@ zero_extendqisi2
	orr	r8, r8, r2
	orr	r8, r8, r1, lsl #4
	cmp	r1, #0
	beq	.L82
	ldr	r3, [r9, #152]
	ldr	r2, [r9, #140]
	subs	r3, r3, #1
	ldr	r0, [r9, #136]
	lsl	lr, r3, #5
	ldr	r3, [r9, #156]
	subs	r0, r0, r2
	subs	r3, r3, #1
	lsls	r0, r0, #15
	lsl	ip, r3, #9
	subs	r3, r2, #3
	lsls	r1, r3, #13
.L66:
	ldrb	r2, [r9, #1]	@ zero_extendqisi2
	ldrb	r3, [r9, #5]	@ zero_extendqisi2
	lsls	r2, r2, #18
	orr	r3, r2, r3, lsl #17
	ldr	r2, [r4, #720]
	orr	r2, r3, r2, lsl #23
	ldrb	r3, [r9, #2]	@ zero_extendqisi2
	orr	r2, r2, r3, lsl #19
	ldrb	r3, [r9, #7]	@ zero_extendqisi2
	orr	r2, r2, r3, lsl #20
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	orr	r2, r2, r3, lsl #22
	ldrb	r3, [r4, #13]	@ zero_extendqisi2
	orr	r2, r2, r3, lsl #26
	ldrb	r3, [r4, #12]	@ zero_extendqisi2
	orr	r2, r2, r3, lsl #27
	orr	r3, r2, r8
	orr	r3, r3, lr
	orr	r3, r3, ip
	orrs	r3, r3, r1
	orrs	r3, r3, r0
	str	r3, [r5, #32]
	movw	r3, #18268
	ldrb	r1, [r4, #19]	@ zero_extendqisi2
	ldrb	r3, [r6, r3]	@ zero_extendqisi2
	ldrb	r2, [r4, #14]	@ zero_extendqisi2
	lsls	r3, r3, #2
	orr	r3, r3, r1, lsl #1
	ldrb	r1, [r4, #1]	@ zero_extendqisi2
	orrs	r3, r3, r1
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #3
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #4
	ldrb	r1, [r4, #9]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #5
	ldrb	r1, [r4, #18]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #6
	ldrb	r1, [r4, #20]	@ zero_extendqisi2
	orr	r3, r3, r2, lsl #7
	orr	r3, r3, r1, lsl #8
	ldrb	r1, [r4, #15]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #9
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #10
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #11
	ldrb	r1, [r4, #6]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #12
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #13
	ldrb	r1, [r4, #17]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #14
	ldrb	r1, [r4, #3]	@ zero_extendqisi2
	orr	r3, r3, r1, lsl #15
	str	r3, [r5, #36]
	ldr	r3, [r4, #40]
	strb	r3, [r5, #40]
	ldr	r3, [r4, #44]
	strb	r3, [r5, #41]
	ldr	r3, [r4, #36]
	strb	r3, [r5, #204]
	ldr	r3, [r4, #708]
	strb	r3, [r5, #205]
	ldr	r3, [r4, #712]
	strb	r3, [r5, #206]
	ldr	r3, [r4, #716]
	strb	r3, [r5, #207]
	cmp	r2, #0
	bne	.L67
.L76:
	ldr	r8, [r6]
	adds	r7, r7, #228
	movw	r3, #18996
	movs	r2, #68
	movs	r1, #0
	mov	r0, r7
	ldr	r5, [r6, r3]
	bl	memset
	ldr	r3, [r8, #716]
	add	r4, r6, #19200
	adds	r4, r4, #48
	cmp	r3, #0
	bne	.L68
.L69:
	movw	r2, #19248
	ldrb	r1, [r5, #13]	@ zero_extendqisi2
	ldrb	r3, [r6, r2]	@ zero_extendqisi2
	bfi	r3, r1, #4, #1
	strb	r3, [r6, r2]
	ldr	r1, [r5, #340]
	bfi	r3, r1, #5, #1
	strb	r3, [r6, r2]
	ldr	r1, [r8, #788]
	bfi	r3, r1, #6, #1
	strb	r3, [r6, r2]
	ldr	r1, [r8, #792]
	bfi	r3, r1, #7, #1
	strb	r3, [r6, r2]
	ldrb	r2, [r5, #14]	@ zero_extendqisi2
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	bfi	r3, r2, #0, #1
	movw	r2, #19262
	strb	r3, [r4, #1]
	movs	r3, #0
	strh	r3, [r6, r2]	@ movhi
	ldr	r2, [r5, #376]
	ldr	r0, [r5, #40]
	ldr	r1, [r2, #48]
	cmp	r0, r1
	bne	.L83
	ldr	r1, [r5, #368]
	cbnz	r1, .L79
	ldr	r3, [r5, #44]
	ldr	r2, [r2, #52]
	subs	r3, r3, r2
	it	ne
	movne	r3, #1
.L79:
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	bfi	r2, r3, #1, #1
	strb	r2, [r4, #1]
	ldr	r3, [r5, #372]
	ldrb	r3, [r3, #9]	@ zero_extendqisi2
	cbz	r3, .L80
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	adds	r3, r3, #0
	it	ne
	movne	r3, #1
.L80:
	ldrb	r2, [r4, #1]	@ zero_extendqisi2
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #1]
	ldr	r3, [r5, #372]
	ldrb	r2, [r3, #9]	@ zero_extendqisi2
	cbz	r2, .L81
	ldrb	r2, [r5]	@ zero_extendqisi2
	adds	r2, r2, #0
	it	ne
	movne	r2, #1
.L81:
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	mov	r8, #0
	movs	r1, #1
	bfi	r3, r2, #3, #1
	strb	r3, [r4, #1]
	ldrb	r2, [r5, #9]	@ zero_extendqisi2
	bfi	r3, r2, #4, #1
	strb	r3, [r4, #1]
	ldrb	r2, [r5, #2]	@ zero_extendqisi2
	bfi	r3, r2, #5, #1
	strb	r3, [r4, #1]
	ldr	r3, [r5, #368]
	strb	r8, [r4, #17]
	strb	r3, [r4, #16]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	bfi	r3, r8, #6, #1
	strb	r3, [r4, #1]
	ldr	r3, [r5, #376]
	ldr	r3, [r3, #24]
	strb	r3, [r4, #18]
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	ldrb	r2, [r5, #11]	@ zero_extendqisi2
	bfi	r3, r2, #7, #1
	strb	r3, [r4, #1]
	ldr	r3, [r5, #36]
	strb	r3, [r4, #8]
	ldr	r3, [r5, #32]
	strb	r3, [r4, #9]
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	ldrb	r2, [r5, #8]	@ zero_extendqisi2
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #2]
	ldrb	r2, [r5, #6]	@ zero_extendqisi2
	bfi	r3, r2, #1, #1
	strb	r3, [r4, #2]
	ldrb	r2, [r5, #5]	@ zero_extendqisi2
	bfi	r3, r2, #2, #1
	strb	r3, [r4, #2]
	ldr	r3, [r5, #48]
	strb	r3, [r4, #10]
	ldr	r3, [r5, #28]
	strb	r3, [r4, #11]
	ldr	r3, [r5, #336]
	strb	r3, [r4, #12]
	ldrb	r3, [r5, #7]	@ zero_extendqisi2
	str	r3, [r4, #64]
	ldrb	r3, [r4, #2]	@ zero_extendqisi2
	bfi	r3, r8, #3, #1
	strb	r3, [r4, #2]
	ldrb	r2, [r5, #12]	@ zero_extendqisi2
	bfi	r3, r2, #4, #1
	strb	r3, [r4, #2]
	ldr	r3, [r5, #372]
	ldr	r0, [r5, #16]
	ldr	r3, [r3, #160]
	lsls	r1, r1, r3
	ldr	r3, [r5, #24]
	subs	r0, r0, r3
	add	r0, r0, r1
	bl	__aeabi_idivmod
	mov	r0, r6
	strh	r1, [r4, #26]	@ movhi
	mov	r1, r7
	ldr	r3, [r5, #344]
	strh	r3, [r4, #28]	@ movhi
	bl	fill_ref_parameters
	mov	r0, r8
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L82:
	mov	ip, r1
	mov	lr, r1
	mov	r0, r1
	b	.L66
.L67:
	ldr	r3, [r4, #56]
	cmp	r3, #19
	ble	.L70
	movs	r1, #143
	ldr	r3, .L87
	str	r1, [sp]
	ldr	r2, .L87+4
	ldr	r0, .L87+8
	bl	printk
.L70:
	ldr	r3, [r4, #192]
	cmp	r3, #21
	ble	.L71
	movs	r1, #144
	ldr	r3, .L87
	str	r1, [sp]
	ldr	r2, .L87+12
	ldr	r0, .L87+8
	bl	printk
.L71:
	ldrb	r0, [r4, #56]	@ zero_extendqisi2
	add	r3, r6, #18304
	adds	r3, r3, #24
	movs	r2, #0
	strb	r0, [r5, #42]
	ldrb	r1, [r4, #192]	@ zero_extendqisi2
	strb	r1, [r5, #43]
.L72:
	uxtb	r4, r2
	cmp	r4, r0
	bcc	.L73
	add	r3, r6, #18432
	movs	r2, #0
	adds	r3, r3, #32
.L74:
	uxtb	r0, r2
	cmp	r0, r1
	bcs	.L76
	ldr	r0, [r3], #4
	adds	r2, r2, #1
	str	r0, [r3, #676]
	b	.L74
.L73:
	ldr	r4, [r3], #4
	adds	r2, r2, #1
	str	r4, [r3, #736]
	b	.L72
.L68:
	movw	r2, #19248
	ldrb	r3, [r6, r2]	@ zero_extendqisi2
	bic	r3, r3, #5
	orr	r3, r3, #5
	strb	r3, [r6, r2]
	ldr	r1, [r8, #720]
	bfi	r3, r1, #1, #1
	strb	r3, [r6, r2]
	ldrd	r3, r1, [r8, #720]
	cbz	r3, .L77
	subs	r1, r1, #1
	strh	r1, [r4, #34]	@ movhi
.L78:
	movs	r3, #50
	strh	r3, [r4, #32]	@ movhi
	ldrb	r3, [r6, r2]	@ zero_extendqisi2
	orr	r3, r3, #8
	strb	r3, [r6, r2]
	b	.L69
.L77:
	str	r1, [r4, #56]
	b	.L78
.L83:
	movs	r3, #1
	b	.L79
.L88:
	.align	2
.L87:
	.word	.LANCHOR0+20
	.word	.LC2
	.word	.LC1
	.word	.LC3
	.fnend
	.size	h265e_syntax_fill, .-h265e_syntax_fill
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.9566, %object
	.size	__FUNCTION__.9566, 20
__FUNCTION__.9566:
	.ascii	"fill_ref_parameters\000"
	.type	__FUNCTION__.9506, %object
	.size	__FUNCTION__.9506, 24
__FUNCTION__.9506:
	.ascii	"fill_picture_parameters\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"differenceInDeltaMSB >= 0\000"
.LC1:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC2:
	.ascii	"pps->num_tile_columns_minus1 <= 19\000"
.LC3:
	.ascii	"pps->num_tile_rows_minus1 <= 21\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
