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
	.file	"vepu541_common.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	vepu541_set_fmt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu541_set_fmt, %function
vepu541_set_fmt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	ubfx	r4, r1, #0, #20
	ands	r1, r1, #983040
	mov	r5, r0
	bne	.L2
	cmp	r4, #17
	bhi	.L3
	ldr	r3, .L12
	movs	r2, #28
	mla	r2, r2, r4, r3
.L4:
	ldr	r3, [r2]
	cmp	r3, #16
	beq	.L5
	mov	r3, r2
	mov	r2, r5
	add	r5, r3, #24
.L6:
	mov	r4, r3
	adds	r2, r2, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, r5
	str	r0, [r2, #-8]	@ unaligned
	mov	r3, r4
	str	r1, [r2, #-4]	@ unaligned
	bne	.L6
	ldr	r0, [r4]
	str	r0, [r2]	@ unaligned
	movs	r0, #0
.L1:
	pop	{r3, r4, r5, pc}
.L2:
	cmp	r1, #65536
	bne	.L3
	ldr	r3, .L12+4
	cmp	r4, r3
	bhi	.L3
	ldr	r3, .L12+8
	sub	r2, r4, #65536
	movs	r1, #28
	mla	r2, r1, r2, r3
	b	.L4
.L3:
	movs	r2, #24
	movs	r1, #0
	adds	r0, r5, #4
	bl	memset
	movs	r3, #16
	str	r3, [r5]
.L5:
	mov	r3, r4
	movw	r2, #359
	ldr	r1, .L12+12
	ldr	r0, .L12+16
	bl	printk
	movs	r3, #16
	mov	r0, #-1
	str	r3, [r5]
	b	.L1
.L13:
	.align	2
.L12:
	.word	.LANCHOR0
	.word	65551
	.word	.LANCHOR0+504
	.word	.LANCHOR1
	.word	.LC0
	.fnend
	.size	vepu541_set_fmt, .-vepu541_set_fmt
	.align	1
	.global	vepu541_get_roi_buf_size
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu541_get_roi_buf_size, %function
vepu541_get_roi_buf_size:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	adds	r0, r0, #63
	adds	r1, r1, #63
	bics	r0, r0, #63
	it	mi
	addmi	r0, r0, #15
	bics	r1, r1, #63
	it	mi
	addmi	r1, r1, #15
	asrs	r3, r0, #4
	asrs	r0, r1, #4
	muls	r0, r3, r0
	lsls	r0, r0, #1
	adds	r0, r0, #32
	bx	lr
	.fnend
	.size	vepu541_get_roi_buf_size, .-vepu541_get_roi_buf_size
	.align	1
	.global	vepu541_set_roi
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu541_set_roi, %function
vepu541_set_roi:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r7, r3
	.pad #68
	sub	sp, sp, #68
	add	r3, r2, #15
	mov	r8, r1
	mov	r5, r2
	asrs	r3, r3, #4
	str	r3, [sp, #32]
	add	r3, r7, #15
	add	r4, r1, #8
	mov	r9, r0
	asrs	r3, r3, #4
	str	r3, [sp, #36]
	ldr	r3, [sp, #32]
	adds	r3, r3, #3
	bic	r3, r3, #3
	str	r3, [sp, #40]
	ldr	r3, [sp, #36]
	adds	r3, r3, #3
	bic	r3, r3, #3
	cbz	r0, .L18
	cbnz	r1, .L19
.L18:
	ldr	r1, .L54
	mov	r3, r9
	ldr	r0, .L54+4
	mov	r2, #390
	str	r8, [sp]
.L53:
	bl	printk
	b	.L52
.L19:
	ldr	r2, [sp, #40]
	mov	r6, #128	@ movhi
	movs	r1, #128
	muls	r3, r2, r3
	movs	r2, #0
.L21:
	cmp	r3, r2
	bgt	.L22
	cmp	r5, #0
	ble	.L23
	cmp	r7, #0
	bgt	.L24
.L23:
	str	r7, [sp]
	mov	r3, r5
	mov	r2, #406
	ldr	r1, .L54
	ldr	r0, .L54+8
	b	.L53
.L22:
	strh	r1, [r9, r2, lsl #1]	@ unaligned
	adds	r2, r2, #1
	b	.L21
.L24:
	ldr	r0, [r8, #4]
	cmp	r0, #8
	bhi	.L25
	movs	r3, #0
.L26:
	cmp	r3, r0
	bne	.L33
	ldr	r3, [sp, #40]
	add	r5, r8, #24
	ldr	fp, .L54+48
	ldr	r10, .L54+52
	lsls	r3, r3, #1
	str	r3, [sp, #52]
	movs	r3, #0
	str	r3, [sp, #28]
.L34:
	ldr	r3, [r8, #4]
	ldr	r2, [sp, #28]
	cmp	r3, r2
	bgt	.L42
.L20:
	movs	r0, #0
	b	.L17
.L25:
	mov	r3, r0
	movw	r2, #411
	ldr	r1, .L54
	ldr	r0, .L54+12
	bl	printk
.L52:
	mov	r0, #-1
.L17:
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L33:
	ldrh	r2, [r4]
	ldrh	r1, [r4, #4]
	add	r2, r2, r1
	cmp	r2, r5
	bgt	.L43
	ldrh	r2, [r4, #2]
	ldrh	r1, [r4, #6]
	add	r2, r2, r1
	cmp	r2, r7
	mov	r2, #-1
	it	le
	movle	r2, #0
.L27:
	ldrh	r1, [r4, #8]
	ldrb	ip, [r4, #15]	@ zero_extendqisi2
	cmp	r1, #1
	bhi	.L46
	ldrh	r1, [r4, #12]
	cmp	r1, #7
	bhi	.L46
	ldrb	r1, [r4, #14]	@ zero_extendqisi2
	cmp	r1, #1
	bhi	.L46
	cmp	ip, #2
	it	cs
	movcs	r2, #-1
.L28:
	ldrsh	r1, [r4, #10]
	cmp	ip, #0
	beq	.L29
	cmp	r1, #51
	ble	.L31
.L30:
	mov	r2, #432
	ldr	r1, .L54
	ldr	r0, .L54+16
	bl	printk
	movw	r2, #435
	ldr	r1, .L54
	ldr	r0, .L54+20
	strd	r5, r7, [sp, #12]
	ldrh	r3, [r4, #6]
	str	r3, [sp, #8]
	ldrh	r3, [r4, #4]
	str	r3, [sp, #4]
	ldrh	r3, [r4, #2]
	str	r3, [sp]
	ldrh	r3, [r4]
	bl	printk
	ldrh	r3, [r4, #12]
	movw	r2, #437
	ldr	r1, .L54
	ldr	r0, .L54+24
	str	r3, [sp]
	ldrh	r3, [r4, #8]
	bl	printk
	ldrsh	r3, [r4, #10]
	movw	r2, #439
	ldr	r1, .L54
	ldr	r0, .L54+28
	str	r3, [sp]
	ldrb	r3, [r4, #15]	@ zero_extendqisi2
	b	.L53
.L43:
	mov	r2, #-1
	b	.L27
.L46:
	mov	r2, #-1
	b	.L28
.L29:
	adds	r1, r1, #51
	uxth	r1, r1
	cmp	r1, #102
	bhi	.L30
.L31:
	cmp	r2, #0
	bne	.L30
	adds	r3, r3, #1
	adds	r4, r4, #16
	b	.L26
.L42:
	ldrh	r3, [r5, #-12]
	ldrh	r7, [r5, #-16]
	adds	r3, r3, #15
	ldrh	r4, [r5, #-14]
	adds	r7, r7, #15
	asrs	r3, r3, #4
	str	r3, [sp, #44]
	ldrh	r3, [r5, #-10]
	asrs	r7, r7, #4
	adds	r4, r4, #15
	adds	r3, r3, #15
	asrs	r4, r4, #4
	asrs	r3, r3, #4
	str	r3, [sp, #48]
	ldr	r3, [sp, #44]
	add	r3, r3, r7
	str	r3, [sp, #56]
	ldr	r3, [sp, #48]
	add	r3, r3, r4
	str	r3, [sp, #60]
	ldr	r3, [sp, #32]
	cmp	r3, r7
	bgt	.L35
	movw	r1, #455
	mov	r3, fp
	str	r1, [sp]
	mov	r0, r10
	ldr	r2, .L54+32
	bl	printk
.L35:
	ldr	r3, [sp, #32]
	ldr	r2, [sp, #56]
	cmp	r3, r2
	bge	.L36
	mov	r1, #456
	mov	r3, fp
	str	r1, [sp]
	mov	r0, r10
	ldr	r2, .L54+36
	bl	printk
.L36:
	ldr	r3, [sp, #36]
	cmp	r3, r4
	bgt	.L37
	movw	r1, #457
	mov	r3, fp
	str	r1, [sp]
	mov	r0, r10
	ldr	r2, .L54+40
	bl	printk
.L37:
	ldr	r3, [sp, #36]
	ldr	r2, [sp, #60]
	cmp	r3, r2
	bge	.L38
	mov	r1, #458
	mov	r3, fp
	str	r1, [sp]
	mov	r0, r10
	ldr	r2, .L54+44
	bl	printk
.L38:
	ldrh	r3, [r5, #-8]
	mvn	r0, #1
	bfi	r6, r3, #0, #1
	ldrh	r3, [r5, #-4]
	bfi	r6, r3, #4, #3
	and	r3, r6, #113
	orn	r3, r3, #127
	bfi	r6, r3, #0, #8
	ldrh	r3, [r5, #-6]
	bfi	r6, r3, #8, #7
	ldrb	r3, [r5, #-1]	@ zero_extendqisi2
	bfi	r6, r3, #15, #1
	ldr	r3, [sp, #40]
	uxth	r1, r6
	mla	r4, r4, r3, r7
	ldr	r3, [sp, #44]
	add	r4, r4, r3
	muls	r0, r3, r0
	add	r4, r9, r4, lsl #1
	movs	r3, #0
.L39:
	ldr	r7, [sp, #48]
	adds	r2, r0, r4
	cmp	r3, r7
	bne	.L41
	ldr	r3, [sp, #28]
	adds	r5, r5, #16
	adds	r3, r3, #1
	str	r3, [sp, #28]
	b	.L34
.L40:
	strh	r1, [r2], #2	@ unaligned
.L41:
	cmp	r2, r4
	bne	.L40
	ldr	r2, [sp, #52]
	adds	r3, r3, #1
	add	r4, r4, r2
	b	.L39
.L55:
	.align	2
.L54:
	.word	.LANCHOR1+16
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LANCHOR1+32
	.word	.LC9
	.fnend
	.size	vepu541_set_roi, .-vepu541_set_roi
	.align	1
	.global	vepu541_set_osd
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu541_set_osd, %function
vepu541_set_osd:
	.fnstart
	@ args = 0, pretend = 0, frame = 264
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #292
	sub	sp, sp, #292
	ldrd	r5, r1, [r0, #8]
	ldr	r4, [r0]
	cbz	r1, .L57
	ldr	r0, [r1, #4]
	movs	r6, #28
	add	r3, sp, #28
	mov	r2, r1
	str	r0, [sp, #28]
	mla	r0, r6, r0, r1
.L58:
	cmp	r2, r0
	bne	.L59
.L60:
	ldr	r6, [sp, #28]
	cbnz	r6, .L62
.L114:
	movs	r0, #0
.L56:
	add	sp, sp, #292
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L59:
	ldr	r6, [r2, #8]
	adds	r2, r2, #28
	str	r6, [r3, #4]
	ldr	r6, [r2, #-16]
	str	r6, [r3, #8]
	ldr	r6, [r2, #-12]
	str	r6, [r3, #12]
	ldr	r6, [r2, #-8]
	str	r6, [r3, #16]
	ldr	r6, [r2, #-4]
	str	r6, [r3, #20]
	ldr	r6, [r2]
	str	r6, [r3, #24]
	ldr	r6, [r2, #4]
	str	r6, [r3, #28]
	ldr	r6, [r1]
	str	r6, [r3, #32]!
	b	.L58
.L57:
	ldr	r1, [r0, #16]
	cbz	r1, .L78
	mov	r2, #260
	add	r0, sp, #28
	bl	memcpy
	b	.L60
.L62:
	cmp	r6, #8
	bls	.L63
	mov	r3, r6
	mov	r2, #620
	ldr	r1, .L115
	ldr	r0, .L115+4
	bl	printk
	movw	r1, #621
	ldr	r3, .L115+8
	ldr	r2, .L115+12
	ldr	r0, .L115+16
	str	r1, [sp]
	bl	printk
.L78:
	mov	r0, #-1
	b	.L56
.L63:
	ldr	r3, [r5, #4]
	add	r7, sp, #28
	ldr	r9, .L115
	add	r8, r4, #496
	cmp	r3, #1
	ldr	r10, .L115+20
	ldrb	r3, [r4, #450]	@ zero_extendqisi2
	mov	r5, #0
	strh	r5, [r4, #448]	@ movhi
	bic	r3, r3, #3
	ite	eq
	orreq	r3, r3, #1
	orrne	r3, r3, #2
	strb	r3, [r4, #450]
.L69:
	ldr	r2, [r7, #4]
	ldrb	r3, [r4, #448]	@ zero_extendqisi2
	lsl	r1, r2, r5
	orrs	r3, r3, r1
	strb	r3, [r4, #448]
	ldr	r3, [r7, #8]
	ldrb	r1, [r4, #449]	@ zero_extendqisi2
	lsls	r3, r3, r5
	orrs	r3, r3, r1
	strb	r3, [r4, #449]
	cbz	r2, .L67
	ldr	r2, [r7, #20]
	cbz	r2, .L67
	ldr	r1, [r7, #24]
	cbz	r1, .L67
	ldr	ip, [r7, #12]
	add	r0, r4, r5, lsl #2
	ldr	lr, [r7, #16]
	uxtb	fp, ip
	strb	fp, [r0, #464]
	uxtb	r3, lr
	add	fp, fp, r2
	strb	r3, [r0, #465]
	add	r3, r3, r1
	add	fp, fp, #-1
	subs	r3, r3, #1
	strb	fp, [r0, #466]
	strb	r3, [r0, #467]
	movs	r3, #0
	mul	r0, r1, r2
	str	r3, [r8, r5, lsl #2]
	ldr	r3, [r7, #28]
	and	fp, r3, #15
	add	r0, r3, r0, lsl #8
	orrs	r0, r0, fp
	beq	.L67
	strd	r1, r3, [sp, #12]
	mov	r0, r10
	strd	lr, r2, [sp, #4]
	mov	r3, r5
	str	ip, [sp]
	mov	r2, #688
	mov	r1, r9
	bl	printk
.L67:
	adds	r5, r5, #1
	adds	r7, r7, #32
	cmp	r6, r5
	bne	.L69
	ldr	r3, [sp, #36]
	cbz	r3, .L70
	ldrb	r3, [r4, #452]	@ zero_extendqisi2
	orr	r3, r3, #15
	strb	r3, [r4, #452]
.L70:
	ldr	r3, [sp, #68]
	cbz	r3, .L71
	ldrb	r3, [r4, #452]	@ zero_extendqisi2
	orr	r3, r3, #240
	strb	r3, [r4, #452]
.L71:
	ldr	r3, [sp, #100]
	cbz	r3, .L72
	ldrb	r3, [r4, #453]	@ zero_extendqisi2
	orr	r3, r3, #15
	strb	r3, [r4, #453]
.L72:
	ldr	r3, [sp, #132]
	cbz	r3, .L73
	ldrb	r3, [r4, #453]	@ zero_extendqisi2
	orr	r3, r3, #240
	strb	r3, [r4, #453]
.L73:
	ldr	r3, [sp, #164]
	cbz	r3, .L74
	ldrb	r3, [r4, #454]	@ zero_extendqisi2
	orr	r3, r3, #15
	strb	r3, [r4, #454]
.L74:
	ldr	r3, [sp, #196]
	cbz	r3, .L75
	ldrb	r3, [r4, #454]	@ zero_extendqisi2
	orr	r3, r3, #240
	strb	r3, [r4, #454]
.L75:
	ldr	r3, [sp, #228]
	cbz	r3, .L76
	ldrb	r3, [r4, #455]	@ zero_extendqisi2
	orr	r3, r3, #15
	strb	r3, [r4, #455]
.L76:
	ldr	r3, [sp, #260]
	cmp	r3, #0
	beq	.L114
	ldrb	r3, [r4, #455]	@ zero_extendqisi2
	orr	r3, r3, #240
	strb	r3, [r4, #455]
	b	.L114
.L116:
	.align	2
.L115:
	.word	.LANCHOR1+48
	.word	.LC13
	.word	.LANCHOR1+64
	.word	.LC14
	.word	.LC9
	.word	.LC15
	.fnend
	.size	vepu541_set_osd, .-vepu541_set_osd
	.align	1
	.global	vepu540_set_osd
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540_set_osd, %function
vepu540_set_osd:
	.fnstart
	@ args = 0, pretend = 0, frame = 264
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #272
	sub	sp, sp, #272
	ldrd	r5, r1, [r0, #8]
	ldr	r4, [r0]
	cbz	r1, .L118
	ldr	r0, [r1, #4]
	movs	r6, #28
	add	r3, sp, #12
	mov	r2, r1
	str	r0, [sp, #12]
	mla	r0, r6, r0, r1
.L119:
	cmp	r0, r2
	bne	.L120
.L121:
	ldr	r3, [sp, #12]
	cbnz	r3, .L123
.L175:
	movs	r0, #0
.L117:
	add	sp, sp, #272
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L120:
	ldr	r6, [r2, #8]
	adds	r2, r2, #28
	str	r6, [r3, #4]
	ldr	r6, [r2, #-16]
	str	r6, [r3, #8]
	ldr	r6, [r2, #-12]
	str	r6, [r3, #12]
	ldr	r6, [r2, #-8]
	str	r6, [r3, #16]
	ldr	r6, [r2, #-4]
	str	r6, [r3, #20]
	ldr	r6, [r2]
	str	r6, [r3, #24]
	ldr	r6, [r2, #4]
	str	r6, [r3, #28]
	ldr	r6, [r1]
	str	r6, [r3, #32]!
	b	.L119
.L118:
	ldr	r1, [r0, #16]
	cbz	r1, .L138
	mov	r2, #260
	add	r0, sp, #12
	bl	memcpy
	b	.L121
.L123:
	cmp	r3, #8
	bls	.L124
	mov	r2, #832
	ldr	r1, .L176
	ldr	r0, .L176+4
	bl	printk
	movw	r1, #833
	ldr	r3, .L176+8
	ldr	r2, .L176+12
	ldr	r0, .L176+16
	str	r1, [sp]
	bl	printk
.L138:
	mov	r0, #-1
	b	.L117
.L124:
	ldr	r2, [r5, #4]
	movs	r1, #0
	add	r6, sp, #12
	mov	r5, r1
	cmp	r2, #1
	ldrb	r2, [r4, #450]	@ zero_extendqisi2
	mov	r7, r1
	mov	lr, #1
	bic	r2, r2, #3
	strh	r1, [r4, #448]	@ movhi
	ite	eq
	orreq	r2, r2, #1
	orrne	r2, r2, #2
	strb	r2, [r4, #450]
	ldr	r2, [r4, #376]
	and	r2, r2, #-16711936
	str	r2, [r4, #376]
	mov	r2, r1
.L129:
	ldr	r8, [r6, #4]
	add	ip, r6, #4
	sxtb	r5, r5
	lsl	r0, r8, r2
	orrs	r7, r7, r0
	ldr	r0, [r6, #8]
	uxtb	r7, r7
	cbz	r0, .L127
	lsl	r0, lr, r2
	sxtb	r0, r0
.L127:
	orrs	r5, r5, r0
	orrs	r1, r1, r0
	uxtb	r5, r5
	uxtb	r1, r1
	cmp	r8, #0
	beq	.L128
	ldr	r8, [r6, #20]
	cmp	r8, #0
	beq	.L128
	ldr	r0, [r6, #24]
	cbz	r0, .L128
	ldrb	r3, [ip, #12]	@ zero_extendqisi2
	add	r8, r8, #-1
	strb	r1, [r4, #376]
	subs	r0, r0, #1
	ldrb	r1, [ip, #8]	@ zero_extendqisi2
	strb	r7, [r4, #448]
	strb	r5, [r4, #449]
	add	r4, r4, r2, lsl #2
	mov	r2, #880
	strb	r1, [r4, #464]
	add	r1, r1, r8
	strb	r3, [r4, #465]
	add	r3, r3, r0
	strb	r1, [r4, #466]
	strb	r3, [r4, #467]
	mov	r3, #-1
	ldr	r1, .L176
	ldr	r0, .L176+20
	bl	printk
	b	.L138
.L128:
	adds	r2, r2, #1
	adds	r6, r6, #32
	cmp	r3, r2
	bne	.L129
	ldr	r3, [sp, #20]
	strb	r7, [r4, #448]
	strb	r5, [r4, #449]
	strb	r1, [r4, #376]
	cbz	r3, .L130
	ldrb	r3, [r4, #452]	@ zero_extendqisi2
	orr	r3, r3, #15
	strb	r3, [r4, #452]
.L130:
	ldr	r3, [sp, #52]
	cbz	r3, .L131
	ldrb	r3, [r4, #452]	@ zero_extendqisi2
	orr	r3, r3, #240
	strb	r3, [r4, #452]
.L131:
	ldr	r3, [sp, #84]
	cbz	r3, .L132
	ldrb	r3, [r4, #453]	@ zero_extendqisi2
	orr	r3, r3, #15
	strb	r3, [r4, #453]
.L132:
	ldr	r3, [sp, #116]
	cbz	r3, .L133
	ldrb	r3, [r4, #453]	@ zero_extendqisi2
	orr	r3, r3, #240
	strb	r3, [r4, #453]
.L133:
	ldr	r3, [sp, #148]
	cbz	r3, .L134
	ldrb	r3, [r4, #454]	@ zero_extendqisi2
	orr	r3, r3, #15
	strb	r3, [r4, #454]
.L134:
	ldr	r3, [sp, #180]
	cbz	r3, .L135
	ldrb	r3, [r4, #454]	@ zero_extendqisi2
	orr	r3, r3, #240
	strb	r3, [r4, #454]
.L135:
	ldr	r3, [sp, #212]
	cbz	r3, .L136
	ldrb	r3, [r4, #455]	@ zero_extendqisi2
	orr	r3, r3, #15
	strb	r3, [r4, #455]
.L136:
	ldr	r3, [sp, #244]
	cmp	r3, #0
	beq	.L175
	ldrb	r3, [r4, #455]	@ zero_extendqisi2
	orr	r3, r3, #240
	strb	r3, [r4, #455]
	b	.L175
.L177:
	.align	2
.L176:
	.word	.LANCHOR1+80
	.word	.LC13
	.word	.LANCHOR1+96
	.word	.LC14
	.word	.LC9
	.word	.LC16
	.fnend
	.size	vepu540_set_osd, .-vepu540_set_osd
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__func__.8822, %object
	.size	__func__.8822, 16
__func__.8822:
	.ascii	"vepu541_set_fmt\000"
	.type	__func__.8845, %object
	.size	__func__.8845, 16
__func__.8845:
	.ascii	"vepu541_set_roi\000"
	.type	__FUNCTION__.8861, %object
	.size	__FUNCTION__.8861, 16
__FUNCTION__.8861:
	.ascii	"vepu541_set_roi\000"
	.type	__func__.8929, %object
	.size	__func__.8929, 16
__func__.8929:
	.ascii	"vepu541_set_osd\000"
	.type	__FUNCTION__.8930, %object
	.size	__FUNCTION__.8930, 16
__FUNCTION__.8930:
	.ascii	"vepu541_set_osd\000"
	.type	__func__.8983, %object
	.size	__func__.8983, 16
__func__.8983:
	.ascii	"vepu540_set_osd\000"
	.type	__FUNCTION__.8984, %object
	.size	__FUNCTION__.8984, 16
__FUNCTION__.8984:
	.ascii	"vepu540_set_osd\000"
	.type	zeros, %object
	.size	zeros, 36
zeros:
	.space	36
	.data
	.align	2
	.set	.LANCHOR0,. + 0
	.type	vepu541_yuv_cfg, %object
	.size	vepu541_yuv_cfg, 504
vepu541_yuv_cfg:
	.word	6
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	4
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	7
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	6
	.word	0
	.word	1
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	5
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	8
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	8
	.word	0
	.word	1
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	9
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	9
	.word	0
	.word	1
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	10
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	12
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	13
	.word	0
	.word	1
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	11
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.type	vepu541_rgb_cfg, %object
	.size	vepu541_rgb_cfg, 448
vepu541_rgb_cfg:
	.word	2
	.word	0
	.word	1
	.word	0
	.word	1
	.word	zeros
	.word	zeros
	.word	2
	.word	0
	.word	0
	.word	0
	.word	1
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	1
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	1
	.word	0
	.word	1
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	16
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	0
	.word	1
	.word	1
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	0
	.word	1
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	0
	.word	0
	.word	1
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	14
	.word	0
	.word	1
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.word	3
	.word	0
	.word	1
	.word	0
	.word	0
	.word	zeros
	.word	zeros
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: unsupport frame format %x\012\000"
.LC1:
	.ascii	"\0013%s:%d: invalid buf %p roi %p\012\000"
.LC2:
	.ascii	"\0013%s:%d: invalid size [%d:%d]\012\000"
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
	.ascii	"pos_x_init >= 0 && pos_x_init < mb_w\000"
.LC9:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC10:
	.ascii	"pos_x_end >= 0 && pos_x_end <= mb_w\000"
.LC11:
	.ascii	"pos_y_init >= 0 && pos_y_init < mb_h\000"
.LC12:
	.ascii	"pos_y_end >= 0 && pos_y_end <= mb_h\000"
.LC13:
	.ascii	"\0013%s:%d: do NOT support more than 8 regions inva"
	.ascii	"lid num %d\012\000"
.LC14:
	.ascii	"osd.num_region <= 8\000"
.LC15:
	.ascii	"\0013%s:%d: invalid osd cfg: %d x:y:w:h:off %d:%d:%"
	.ascii	"d:%d:%x\012\000"
.LC16:
	.ascii	"\0013%s:%d: invalid osd buffer fd %d\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
