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
	.file	"mpp_maths.c"
	.text
	.align	1
	.global	mpp_log2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_log2, %function
mpp_log2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	lsrs	r3, r0, #16
	lsls	r3, r3, #16
	cbz	r3, .L2
	lsrs	r0, r0, #16
	movs	r3, #16
.L2:
	tst	r0, #65280
	ldr	r2, .L8
	itt	ne
	lsrne	r0, r0, #8
	addne	r3, r3, #8
	ldrb	r0, [r2, r0]	@ zero_extendqisi2
	add	r0, r0, r3
	bx	lr
.L9:
	.align	2
.L8:
	.word	.LANCHOR0
	.fnend
	.size	mpp_log2, .-mpp_log2
	.align	1
	.global	mpp_log2_16bit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_log2_16bit, %function
mpp_log2_16bit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ands	r3, r0, #65280
	ldr	r2, .L13
	itt	ne
	lsrne	r0, r0, #8
	movne	r3, #8
	ldrb	r0, [r2, r0]	@ zero_extendqisi2
	add	r0, r0, r3
	bx	lr
.L14:
	.align	2
.L13:
	.word	.LANCHOR0
	.fnend
	.size	mpp_log2_16bit, .-mpp_log2_16bit
	.global	__aeabi_idiv
	.global	__aeabi_uidiv
	.align	1
	.global	axb_div_c
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	axb_div_c, %function
axb_div_c:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r5, r1
	mov	r8, r2
	mov	r4, r0
	cbz	r0, .L16
	cmp	r1, #0
	beq	.L27
	mul	r6, r1, r0
	mov	r0, r6
	bl	__aeabi_idiv
	cmp	r0, r4
	bne	.L17
	cmp	r8, #0
	beq	.L17
	mov	r1, r8
	mov	r0, r6
	bl	__aeabi_idiv
	mov	r4, r0
.L16:
	mov	r0, r4
	pop	{r4, r5, r6, r7, r8, pc}
.L17:
	cmp	r4, #0
	itte	lt
	rsblt	r4, r4, #0
	movlt	r6, #-1
	movge	r6, #1
	cmp	r5, #0
	itt	lt
	rsblt	r6, r6, #0
	rsblt	r5, r5, #0
	cmp	r8, #0
	bge	.L20
	rsbs	r6, r6, #0
	rsb	r8, r8, #0
.L21:
	cmp	r4, r5
	ittt	lt
	movlt	r3, r4
	movlt	r4, r5
	movlt	r5, r3
	movs	r3, #31
.L23:
	lsl	r0, r4, r3
	lsrs	r0, r0, r3
	subs	r3, r3, #1
	cmp	r4, r0
	bne	.L23
	movs	r4, #0
.L24:
	lsr	r2, r5, r4
	cmp	r2, r8
	bhi	.L25
	cmp	r3, r4
	bcc	.L36
	subs	r7, r3, r4
	mov	r1, r8
	lsls	r0, r0, r7
	bl	__aeabi_uidiv
	mul	r4, r5, r0
	lsrs	r4, r4, r7
	muls	r4, r6, r4
	b	.L16
.L20:
	bne	.L21
.L36:
	rsb	r4, r6, r6, lsl #31
	b	.L16
.L25:
	adds	r4, r4, #1
	b	.L24
.L27:
	mov	r4, r1
	b	.L16
	.fnend
	.size	axb_div_c, .-axb_div_c
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	log2_tab, %object
	.size	log2_tab, 256
log2_tab:
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	2
	.byte	2
	.byte	2
	.byte	2
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	3
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	4
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	6
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
