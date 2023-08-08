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
	.file	"h264e_dpb.c"
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
	.type	cmp_st_list, %function
cmp_st_list:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0]
	ldr	r2, [r3, #24]
	ldr	r3, [r1]
	ldr	r3, [r3, #24]
	cmp	r2, r3
	beq	.L3
	blt	.L4
	mov	r0, #-1
	bx	lr
.L3:
	movs	r0, #0
	bx	lr
.L4:
	movs	r0, #1
	bx	lr
	.fnend
	.size	cmp_st_list, .-cmp_st_list
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	cmp_lt_list, %function
cmp_lt_list:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0]
	ldr	r2, [r3, #28]
	ldr	r3, [r1]
	ldr	r3, [r3, #28]
	cmp	r2, r3
	beq	.L7
	bgt	.L8
	mov	r0, #-1
	bx	lr
.L7:
	movs	r0, #0
	bx	lr
.L8:
	movs	r0, #1
	bx	lr
	.fnend
	.size	cmp_lt_list, .-cmp_lt_list
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_dump_listX.part.0, %function
h264e_dpb_dump_listX.part.0:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #36
	sub	sp, sp, #36
	ldr	r7, .L20
	mov	r6, r0
	ldr	r8, .L20+24
	mov	r5, r1
	ldr	r9, .L20+28
	movs	r4, #0
.L10:
	cmp	r4, r5
	blt	.L15
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L15:
	ldr	r3, [r6, r4, lsl #2]
	ldrb	r1, [r3, #16]	@ zero_extendqisi2
	lsls	r2, r1, #27
	and	r0, r1, #64
	bpl	.L11
	ldr	ip, .L20+32
	tst	r1, #32
	ldr	r2, .L20+4
	it	ne
	movne	r2, ip
	cbz	r0, .L14
.L18:
	ldr	r1, .L20+8
	b	.L13
.L11:
	cbnz	r0, .L17
	ldr	r2, .L20+12
.L14:
	tst	r1, #128
	ite	ne
	movne	r1, r7
	moveq	r1, r8
.L13:
	ldrh	r0, [r3, #18]
	sbfx	r0, r0, #6, #8
	str	r0, [sp, #28]
	ldrb	r0, [r3, #18]	@ zero_extendqisi2
	and	r0, r0, #63
	str	r0, [sp, #24]
	ldrb	r0, [r3, #17]	@ zero_extendqisi2
	and	r0, r0, #15
	strd	r1, r0, [sp, #16]
	ldrb	r1, [r3, #17]	@ zero_extendqisi2
	ldr	r0, .L20+16
	lsrs	r1, r1, #4
	strd	r2, r1, [sp, #8]
	ldrh	r2, [r3, #22]
	ldr	r1, .L20+20
	str	r2, [sp, #4]
	movs	r2, #64
	ldr	r3, [r3, #8]
	str	r3, [sp]
	mov	r3, r4
	bl	printk
	adds	r4, r4, #1
	b	.L10
.L17:
	mov	r2, r9
	b	.L18
.L21:
	.align	2
.L20:
	.word	.LC5
	.word	.LC1
	.word	.LC3
	.word	.LC2
	.word	.LC7
	.word	.LANCHOR0
	.word	.LC6
	.word	.LC4
	.word	.LC0
	.fnend
	.size	h264e_dpb_dump_listX.part.0, .-h264e_dpb_dump_listX.part.0
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.global	h264e_dpb_dump_frm
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_dump_frm, %function
h264e_dpb_dump_frm:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #32
	sub	sp, sp, #32
	mov	r4, r0
	mov	r3, r1
	str	r2, [sp]
	movs	r2, #27
	ldr	r1, .L33
	movs	r5, #0
	ldr	r0, .L33+4
	bl	printk
	ldr	r3, [r4, #56]
	movs	r2, #29
	ldr	r1, .L33
	ldr	r0, .L33+8
	ldr	r6, .L33+12
	ldr	r7, .L33+16
	ldr	r8, .L33+44
	str	r3, [sp, #4]
	ldr	r3, [r4, #60]
	str	r3, [sp]
	mov	r3, r4
	bl	printk
	movs	r2, #30
	ldr	r1, .L33
	ldr	r0, .L33+20
	bl	printk
.L23:
	ldr	r3, [r4, #60]
	cmp	r3, r5
	bgt	.L28
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L28:
	movs	r3, #48
	mla	r3, r3, r5, r4
	ldrb	r2, [r3, #416]	@ zero_extendqisi2
	lsls	r3, r2, #27
	and	r1, r2, #64
	bpl	.L24
	ldr	r0, .L33+24
	tst	r2, #32
	ldr	r3, .L33+28
	it	ne
	movne	r3, r0
	cbnz	r1, .L31
.L27:
	tst	r2, #128
	ite	ne
	movne	r0, r6
	moveq	r0, r7
.L26:
	movs	r2, #48
	muls	r2, r5, r2
	add	lr, r4, r2
	ldrh	ip, [lr, #418]
	sbfx	ip, ip, #6, #8
	str	ip, [sp, #28]
	ldrb	ip, [lr, #418]	@ zero_extendqisi2
	and	ip, ip, #63
	str	ip, [sp, #24]
	ldrb	ip, [lr, #417]	@ zero_extendqisi2
	and	ip, ip, #15
	strd	r0, ip, [sp, #16]
	ldrb	r1, [lr, #417]	@ zero_extendqisi2
	ldr	r0, .L33+32
	lsrs	r1, r1, #4
	strd	r3, r1, [sp, #8]
	ldrh	r3, [lr, #422]
	ldr	r1, .L33
	str	r3, [sp, #4]
	add	r3, r4, #408
	ldr	r3, [r3, r2]
	movs	r2, #44
	str	r3, [sp]
	mov	r3, r5
	bl	printk
	adds	r5, r5, #1
	b	.L23
.L24:
	cbnz	r1, .L30
	ldr	r3, .L33+36
	b	.L27
.L30:
	mov	r3, r8
.L31:
	ldr	r0, .L33+40
	b	.L26
.L34:
	.align	2
.L33:
	.word	.LANCHOR0+21
	.word	.LC8
	.word	.LC9
	.word	.LC5
	.word	.LC6
	.word	.LC10
	.word	.LC0
	.word	.LC1
	.word	.LC7
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.fnend
	.size	h264e_dpb_dump_frm, .-h264e_dpb_dump_frm
	.text
	.align	1
	.global	h264e_dpb_dump_listX
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_dump_listX, %function
h264e_dpb_dump_listX:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	h264e_dpb_dump_listX.part.0
	.fnend
	.size	h264e_dpb_dump_listX, .-h264e_dpb_dump_listX
	.section	.text.unlikely
	.align	1
	.global	h264e_dpb_dump_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_dump_list, %function
h264e_dpb_dump_list:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	mov	r4, r0
	movs	r2, #70
	ldr	r1, .L38
	ldr	r0, .L38+4
	bl	printk
	ldr	r3, [r4, #48]
	movs	r2, #73
	ldr	r1, .L38
	ldr	r0, .L38+8
	str	r3, [sp, #8]
	ldr	r3, [r4, #44]
	str	r3, [sp, #4]
	ldr	r3, [r4, #52]
	str	r3, [sp]
	ldr	r3, [r4, #56]
	bl	printk
	ldrd	r3, r2, [r4, #44]
	cmn	r3, r2
	beq	.L36
	movs	r2, #76
	ldr	r1, .L38
	ldr	r0, .L38+12
	bl	printk
	ldrd	r1, r3, [r4, #44]
	add	r0, r4, #96
	add	r1, r1, r3
	add	sp, sp, #16
	@ sp needed
	pop	{r4, lr}
	b	h264e_dpb_dump_listX.part.0
.L36:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L39:
	.align	2
.L38:
	.word	.LANCHOR0+40
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.fnend
	.size	h264e_dpb_dump_list, .-h264e_dpb_dump_list
	.text
	.align	1
	.global	h264e_dpb_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_init, %function
h264e_dpb_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r2
	ldr	r5, .L51
	mov	r4, r0
	mov	r7, r1
	ldr	r2, [r5]
	lsls	r2, r2, #23
	bpl	.L41
	mov	r3, r0
	movs	r2, #86
	ldr	r1, .L51+4
	ldr	r0, .L51+8
	bl	printk
.L41:
	movs	r1, #0
	mov	r2, #2032
	mov	r0, r4
	bl	memset
	add	r1, r4, #400
	movs	r3, #0
	movs	r0, #48
	strd	r7, r6, [r4]
.L42:
	mul	r2, r0, r3
	str	r3, [r1, r2]
	adds	r3, r3, #1
	cmp	r3, #17
	bne	.L42
	ldr	r3, [r5]
	lsls	r3, r3, #23
	bpl	.L43
	mov	r3, r4
	movs	r2, #95
	ldr	r1, .L51+4
	ldr	r0, .L51+12
	bl	printk
.L43:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L52:
	.align	2
.L51:
	.word	h264e_debug
	.word	.LANCHOR0+60
	.word	.LC14
	.word	.LC15
	.fnend
	.size	h264e_dpb_init, .-h264e_dpb_init
	.align	1
	.global	h264e_dpb_setup
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_setup, %function
h264e_dpb_setup:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movw	r3, #4700
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r4, r0
	ldr	r0, [r1, r3]
	mov	r8, r2
	bl	mpp_enc_ref_cfg_get_cpb_info
	ldr	r5, .L76
	ldr	r7, [r8, #40]
	ldr	r6, [r8, #48]
	ldr	r3, [r5]
	adds	r7, r7, #4
	ldr	r9, [r8, #52]
	adds	r6, r6, #4
	lsls	r3, r3, #23
	mov	r10, r0
	bpl	.L54
	mov	r3, r4
	movs	r2, #109
	ldr	r1, .L76+4
	ldr	r0, .L76+8
	bl	printk
.L54:
	ldrd	r1, r2, [r4]
	mov	r0, r4
	bl	h264e_dpb_init
	add	r2, r4, #8
	add	r1, r10, #28
	mov	r3, r10
.L55:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L55
	ldr	r3, [r10]
	movs	r2, #1
	lsl	r1, r2, r7
	lsls	r2, r2, r6
	str	r1, [r4, #64]
	str	r3, [r4, #56]
	adds	r3, r3, #1
	str	r2, [r4, #68]
	str	r3, [r4, #60]
	ldr	r2, [r8, #44]
	str	r2, [r4, #72]
	ldr	r2, [r5]
	lsls	r0, r2, #23
	bpl	.L59
	str	r3, [sp]
	movs	r2, #122
	mov	r3, r9
	ldr	r1, .L76+4
	ldr	r0, .L76+12
	bl	printk
	ldr	r3, [r5]
	lsls	r1, r3, #23
	bpl	.L59
	ldr	r3, [r4, #64]
	movs	r2, #124
	ldr	r1, .L76+4
	ldr	r0, .L76+16
	str	r3, [sp]
	mov	r3, r7
	bl	printk
	ldr	r3, [r5]
	lsls	r2, r3, #23
	bpl	.L59
	ldr	r3, [r4, #68]
	movs	r2, #126
	ldr	r1, .L76+4
	ldr	r0, .L76+20
	str	r3, [sp]
	mov	r3, r6
	bl	printk
	ldr	r3, [r5]
	lsls	r3, r3, #23
	bpl	.L59
	mov	r3, r4
	movs	r2, #128
	ldr	r1, .L76+4
	ldr	r0, .L76+24
	bl	printk
.L59:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L77:
	.align	2
.L76:
	.word	h264e_debug
	.word	.LANCHOR0+75
	.word	.LC14
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC15
	.fnend
	.size	h264e_dpb_setup, .-h264e_dpb_setup
	.align	1
	.global	find_cpb_frame
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	find_cpb_frame, %function
find_cpb_frame:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r8, r0
	ldrb	r4, [r2]	@ zero_extendqisi2
	mov	r7, r1
	mov	r5, r2
	ldrh	r6, [r2, #6]
	ands	r4, r4, #1
	beq	.L78
	ldr	r3, .L107
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #256
	beq	.L80
	mov	r3, r6
	movs	r2, #141
	ldr	r1, .L107+4
	ldr	r0, .L107+8
	bl	printk
.L80:
	movs	r3, #0
	movs	r0, #48
.L81:
	cmp	r3, r7
	blt	.L87
	mov	r3, r6
	movs	r2, #158
	ldr	r1, .L107+4
	movs	r4, #0
	ldr	r0, .L107+12
	bl	printk
	b	.L78
.L87:
	mla	r4, r0, r3, r8
	ldrb	r1, [r4, #16]	@ zero_extendqisi2
	mov	r2, r1
	lsls	r1, r1, #31
	bpl	.L82
	ldrh	r1, [r4, #22]
	cmp	r6, r1
	bne	.L82
	ldr	r1, [r9]
	lsls	r0, r1, #23
	bpl	.L83
	and	r2, r2, #1
	ldr	r1, .L107+4
	strd	r3, r2, [sp]
	mov	r3, r6
	movs	r2, #148
	ldr	r0, .L107+16
	bl	printk
.L83:
	ldrb	r3, [r4, #16]	@ zero_extendqisi2
	ldrb	r2, [r5]	@ zero_extendqisi2
	eors	r3, r3, r2
	lsls	r1, r3, #25
	bpl	.L84
	movs	r1, #150
	ldr	r3, .L107+20
	str	r1, [sp]
	ldr	r2, .L107+24
	ldr	r0, .L107+28
	bl	printk
.L84:
	ldrb	r2, [r5]	@ zero_extendqisi2
	ldrb	r3, [r4, #16]	@ zero_extendqisi2
	eors	r3, r3, r2
	lsls	r2, r3, #24
	bpl	.L85
	movs	r1, #151
	ldr	r3, .L107+20
	str	r1, [sp]
	ldr	r2, .L107+32
	ldr	r0, .L107+28
	bl	printk
.L85:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	eors	r3, r3, r2
	lsls	r3, r3, #28
	beq	.L86
	movs	r1, #152
	ldr	r3, .L107+20
	str	r1, [sp]
	ldr	r2, .L107+36
	ldr	r0, .L107+28
	bl	printk
.L86:
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	ldrb	r2, [r5, #1]	@ zero_extendqisi2
	eors	r3, r3, r2
	tst	r3, #240
	beq	.L78
	movs	r1, #153
	ldr	r3, .L107+20
	str	r1, [sp]
	ldr	r2, .L107+40
	ldr	r0, .L107+28
	bl	printk
.L78:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L82:
	adds	r3, r3, #1
	b	.L81
.L108:
	.align	2
.L107:
	.word	h264e_debug
	.word	.LANCHOR0+91
	.word	.LC19
	.word	.LC26
	.word	.LC20
	.word	.LANCHOR0+106
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.fnend
	.size	find_cpb_frame, .-find_cpb_frame
	.align	1
	.global	h264e_dpb_build_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_build_list, %function
h264e_dpb_build_list:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r10, r1
	ldr	r5, .L197
	.pad #44
	sub	sp, sp, #44
	mov	r4, r0
	ldr	r3, [r5]
	lsls	r1, r3, #23
	bpl	.L110
	mov	r3, r0
	movs	r2, #203
	ldr	r1, .L197+4
	ldr	r0, .L197+8
	bl	printk
.L110:
	add	r3, r4, #96
	movs	r2, #64
	movs	r1, #0
	str	r3, [sp, #20]
	mov	r0, r3
	bl	memset
	ldrb	r2, [r10, #8]	@ zero_extendqisi2
	ldr	r3, [r5]
	lsls	r2, r2, #27
	bpl	.L111
	lsls	r7, r3, #23
	bpl	.L109
	mov	r3, r4
	movs	r2, #209
.L196:
	ldr	r1, .L197+4
	ldr	r0, .L197+12
	bl	printk
.L109:
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L111:
	lsls	r6, r3, #22
	bpl	.L114
	movs	r2, #215
	ldr	r1, .L197+4
	ldr	r0, .L197+16
	bl	printk
.L114:
	movs	r7, #0
	add	r9, r10, #24
	mov	r6, r7
	mov	r8, r7
.L119:
	add	fp, r8, #3
	ldrb	r3, [r10, fp, lsl #3]	@ zero_extendqisi2
	lsls	r0, r3, #31
	bpl	.L115
	lsls	r1, r3, #25
	bpl	.L116
	movs	r1, #224
	ldr	r3, .L197+20
	str	r1, [sp]
	ldr	r2, .L197+24
	ldr	r0, .L197+28
	bl	printk
.L116:
	ldr	r3, [r5]
	lsls	r2, r3, #22
	bpl	.L117
	ldrb	r3, [r10, fp, lsl #3]	@ zero_extendqisi2
	ldr	r1, .L197+4
	ldr	r0, .L197+32
	lsrs	r2, r3, #7
	str	r2, [sp, #12]
	ubfx	r2, r3, #6, #1
	and	r3, r3, #1
	str	r3, [sp, #4]
	add	r3, r10, fp, lsl #3
	str	r2, [sp, #8]
	movs	r2, #227
	ldrh	r3, [r3, #6]
	str	r3, [sp]
	mov	r3, r8
	bl	printk
.L117:
	mov	r2, r9
	ldr	r1, [r4, #60]
	add	r0, r4, #400
	bl	find_cpb_frame
	ldrsb	r3, [r10, fp, lsl #3]
	cmp	r3, #0
	blt	.L118
	add	fp, r6, #1
	adds	r6, r6, #40
	str	r0, [r4, r6, lsl #2]
	ldrd	r2, [r9]
	strd	r2, [r0, #16]
	ldr	r3, [r5]
	lsls	r3, r3, #22
	bpl	.L151
	strd	fp, r9, [sp]
	mov	r3, r8
	movs	r2, #233
	ldr	r1, .L197+4
	ldr	r0, .L197+36
	bl	printk
.L151:
	mov	r6, fp
.L115:
	add	r8, r8, #1
	add	r9, r9, #8
	cmp	r8, #8
	bne	.L119
	ldr	r3, [r5]
	lsls	r0, r3, #22
	bpl	.L120
	movs	r2, #241
	ldr	r1, .L197+4
	ldr	r0, .L197+40
	bl	printk
.L120:
	ldr	r3, [r5]
	lsls	r1, r3, #23
	bpl	.L121
	strd	r6, r7, [sp]
	movs	r2, #242
	ldr	r3, [r4, #56]
	ldr	r1, .L197+4
	ldr	r0, .L197+44
	bl	printk
.L121:
	cmp	r6, #1
	ble	.L122
	ldr	r3, [r5]
	add	r8, r4, #160
	lsls	r2, r3, #22
	bpl	.L123
	ldr	r1, .L197+4
	movs	r2, #247
	ldr	r0, .L197+48
	bl	printk
	mov	r1, r6
	mov	r0, r8
	bl	h264e_dpb_dump_listX.part.0
.L123:
	movs	r3, #0
	movs	r2, #4
	str	r3, [sp]
	mov	r1, r6
	ldr	r3, .L197+52
	mov	r0, r8
	bl	sort
	ldr	r3, [r5]
	lsls	r3, r3, #22
	bpl	.L124
	ldr	r1, .L197+4
	movs	r2, #254
	ldr	r0, .L197+56
	bl	printk
	mov	r1, r6
	mov	r0, r8
	bl	h264e_dpb_dump_listX.part.0
.L122:
	cmp	r7, #1
	ble	.L126
	ldr	r3, [r5]
	lsls	r0, r3, #22
	bpl	.L128
	ldr	r1, .L197+4
	movw	r2, #261
	ldr	r0, .L197+60
	bl	printk
	mov	r1, r7
	add	r0, r4, #224
	bl	h264e_dpb_dump_listX.part.0
.L128:
	add	r8, r4, #224
	movs	r3, #0
	mov	r1, r7
	str	r3, [sp]
	movs	r2, #4
	ldr	r3, .L197+64
	mov	r0, r8
	bl	sort
	ldr	r3, [r5]
	lsls	r1, r3, #22
	bpl	.L126
	ldr	r1, .L197+4
	mov	r2, #268
	ldr	r0, .L197+68
	bl	printk
	mov	r1, r7
	mov	r0, r8
	bl	h264e_dpb_dump_listX.part.0
.L126:
	movs	r2, #64
	movs	r1, #0
	ldr	r0, [sp, #20]
	bl	memset
	movs	r3, #0
	ldr	r2, [sp, #20]
.L130:
	cmp	r3, r6
	blt	.L131
	bic	r3, r6, r6, asr #31
	add	r1, r4, #224
	adds	r3, r3, #24
	movs	r2, #0
	add	r3, r4, r3, lsl #2
.L132:
	cmp	r2, r7
	blt	.L133
	ldr	r3, [r4, #56]
	strd	r6, r7, [r4, #44]
	add	r6, r6, r7
	cmp	r3, r6
	bge	.L134
	movw	r1, #285
	ldr	r3, .L197+20
	str	r1, [sp]
	ldr	r2, .L197+72
	ldr	r0, .L197+28
	bl	printk
.L134:
	ldr	r3, [r5]
	lsls	r2, r3, #22
	bpl	.L135
	mov	r0, r4
	bl	h264e_dpb_dump_list
.L135:
	ldr	r0, [r4]
	bl	h264e_reorder_wr_rewind
	ldrd	r3, r2, [r4, #44]
	cmn	r3, r2
	beq	.L136
	ldr	r3, [r4, #96]
	ldrd	r9, r6, [r4, #88]
	ldrh	r7, [r3, #22]
	ldr	r3, [r5]
	ldrh	r8, [r6, #22]
	lsls	r3, r3, #22
	bpl	.L137
	subs	r2, r8, r7
	ldrh	r3, [r9, #22]
	ldr	r1, .L197+4
	it	ne
	movne	r2, #1
	str	r7, [sp]
	strd	r8, r2, [sp, #4]
	mov	r2, #304
	ldr	r0, .L197+76
	bl	printk
	cmp	r8, r7
	beq	.L139
	ldr	r3, [r5]
	lsls	r0, r3, #22
	bpl	.L141
	ldrh	r3, [r6, #22]
	movw	r2, #309
	ldr	r1, .L197+4
	ldr	r0, .L197+80
	bl	printk
.L141:
	ldrb	r3, [r6, #16]	@ zero_extendqisi2
	lsls	r1, r3, #25
	bpl	.L142
	movw	r1, #311
	ldr	r3, .L197+20
	str	r1, [sp]
	ldr	r2, .L197+84
	ldr	r0, .L197+28
	bl	printk
.L142:
	ldrsb	r3, [r6, #16]
	ldr	r1, [r5]
	cmp	r3, #0
	and	r1, r1, #512
	bge	.L143
	movs	r3, #2
	str	r3, [sp, #24]
	ldrb	r3, [r6, #17]	@ zero_extendqisi2
	and	r3, r3, #15
	str	r3, [sp, #32]
	cbz	r1, .L145
	mov	r2, #318
	ldr	r1, .L197+4
	ldr	r0, .L197+88
	bl	printk
.L145:
	add	r1, sp, #24
	ldr	r0, [r4]
	bl	h264e_reorder_wr_op
.L139:
	ldr	r3, [r5]
	lsls	r3, r3, #23
	bpl	.L109
	mov	r3, r4
	mov	r2, #334
	b	.L196
.L198:
	.align	2
.L197:
	.word	h264e_debug
	.word	.LANCHOR0+121
	.word	.LC14
	.word	.LC15
	.word	.LC27
	.word	.LANCHOR0+142
	.word	.LC28
	.word	.LC22
	.word	.LC29
	.word	.LC30
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	cmp_st_list
	.word	.LC35
	.word	.LC36
	.word	cmp_lt_list
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC42
.L118:
	add	fp, r7, #1
	adds	r7, r7, #56
	str	r0, [r4, r7, lsl #2]
	ldrd	r2, [r9]
	strd	r2, [r0, #16]
	ldr	r3, [r5]
	lsls	r7, r3, #22
	bpl	.L152
	strd	fp, r9, [sp]
	mov	r3, r8
	movs	r2, #237
	ldr	r1, .L199
	ldr	r0, .L199+4
	bl	printk
.L152:
	mov	r7, fp
	b	.L115
.L131:
	ldr	r1, [r2, #64]
	adds	r3, r3, #1
	str	r1, [r2], #4
	b	.L130
.L133:
	ldr	r0, [r1], #4
	adds	r2, r2, #1
	str	r0, [r3], #4
	b	.L132
.L143:
	movs	r3, #0
	ldr	r0, [r6, #24]
	str	r3, [sp, #24]
	ldr	r3, [r9, #24]
	subs	r2, r3, r0
	cmp	r2, #0
	it	lt
	rsblt	r2, r2, #0
	subs	r2, r2, #1
	str	r2, [sp, #28]
	cmp	r1, #0
	beq	.L145
	strd	r0, r2, [sp]
	mov	r2, #326
	ldr	r1, .L199
	ldr	r0, .L199+8
	bl	printk
	b	.L145
.L136:
	ldr	r3, [r5]
	lsls	r2, r3, #22
	bpl	.L139
	mov	r2, #332
	ldr	r1, .L199
	ldr	r0, .L199+12
	bl	printk
	b	.L139
.L137:
	cmp	r8, r7
	bne	.L141
	b	.L139
.L124:
	cmp	r7, #1
	bgt	.L128
	b	.L126
.L200:
	.align	2
.L199:
	.word	.LANCHOR0+121
	.word	.LC31
	.word	.LC43
	.word	.LC44
	.fnend
	.size	h264e_dpb_build_list, .-h264e_dpb_build_list
	.align	1
	.global	h264e_dpb_build_marking
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_build_marking, %function
h264e_dpb_build_marking:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r7, .L227
	.pad #40
	sub	sp, sp, #40
	ldr	r5, [r0, #88]
	ldr	r6, [r0, #4]
	ldr	r3, [r7]
	lsls	r3, r3, #23
	bpl	.L202
	mov	r3, r0
	mov	r2, #342
	ldr	r1, .L227+4
	ldr	r0, .L227+8
	bl	printk
.L202:
	mov	r0, r6
	bl	h264e_marking_wr_rewind
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	ands	r3, r3, #32
	beq	.L203
	movs	r3, #0
	movs	r0, #1
	strd	r0, r3, [r6]
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	lsrs	r3, r3, #7
	str	r3, [r6, #8]
.L204:
	ldr	r3, [r7]
	lsls	r2, r3, #23
	bpl	.L201
	ldr	r3, [r4, #52]
	mov	r2, #392
	ldr	r1, .L227+4
	ldr	r0, .L227+12
	str	r3, [sp]
	ldr	r3, [r4, #56]
	bl	printk
	ldr	r3, [r7]
	lsls	r3, r3, #23
	bpl	.L201
	mov	r3, r4
	movw	r2, #393
	ldr	r1, .L227+4
	ldr	r0, .L227+16
	bl	printk
.L201:
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L203:
	str	r3, [r6]
	strd	r3, r3, [r6, #8]
	ldr	r3, [r7]
	lsls	r1, r3, #23
	bpl	.L205
	ldrb	r2, [r5, #17]	@ zero_extendqisi2
	ldrh	r3, [r5, #22]
	ldr	r1, .L227+4
	lsrs	r2, r2, #4
	str	r2, [sp, #8]
	ldrb	r2, [r5, #16]	@ zero_extendqisi2
	ldr	r0, .L227+20
	lsrs	r2, r2, #7
	str	r2, [sp, #4]
	ldrb	r2, [r5, #16]	@ zero_extendqisi2
	eor	r2, r2, #64
	ubfx	r2, r2, #6, #1
	str	r2, [sp]
	mov	r2, #360
	bl	printk
.L205:
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	lsls	r2, r3, #25
	bmi	.L204
	lsls	r3, r3, #24
	bpl	.L204
	ldr	r3, [r4, #20]
	ldr	r2, [r4, #40]
	cmp	r3, r2
	str	r3, [r4, #36]
	beq	.L206
	add	r1, sp, #20
	mov	r0, r6
	add	r8, r3, #1
	movs	r3, #4
	str	r8, [sp, #36]
	str	r3, [sp, #20]
	bl	h264e_marking_wr_op
	ldr	r3, [r7]
	lsls	r0, r3, #21
	bpl	.L207
	mov	r3, r8
	mov	r2, #378
	ldr	r1, .L227+4
	ldr	r0, .L227+24
	bl	printk
.L207:
	ldr	r3, [r4, #36]
	str	r3, [r4, #40]
.L206:
	movs	r3, #6
	str	r3, [sp, #20]
	ldr	r3, [r5, #28]
	add	r1, sp, #20
	mov	r0, r6
	str	r3, [sp, #32]
	bl	h264e_marking_wr_op
	ldr	r3, [r7]
	lsls	r1, r3, #21
	bpl	.L204
	ldr	r3, [r5, #28]
	movw	r2, #387
	ldr	r1, .L227+4
	ldr	r0, .L227+28
	bl	printk
	b	.L204
.L228:
	.align	2
.L227:
	.word	h264e_debug
	.word	.LANCHOR0+163
	.word	.LC14
	.word	.LC48
	.word	.LC15
	.word	.LC45
	.word	.LC46
	.word	.LC47
	.fnend
	.size	h264e_dpb_build_marking, .-h264e_dpb_build_marking
	.align	1
	.global	h264e_dpb_proc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_proc, %function
h264e_dpb_proc:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #44
	sub	sp, sp, #44
	add	r3, r0, #400
	ldr	r7, .L315
	str	r3, [sp, #20]
	mov	r4, r0
	ldrh	r3, [r1, #14]
	mov	r5, r1
	add	r6, r0, #356
	str	r3, [sp, #16]
	ldr	r3, [r7]
	lsls	r3, r3, #23
	bpl	.L230
	ldr	r3, [r0, #60]
	mov	r2, #408
	ldr	r1, .L315+4
	str	r3, [sp]
	mov	r3, r0
	ldr	r0, .L315+8
	bl	printk
.L230:
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	lsls	r0, r3, #26
	bpl	.L231
	ldr	r3, [sp, #20]
	add	r0, r4, #1216
	movs	r2, #0
.L232:
	ldrb	r1, [r3, #16]	@ zero_extendqisi2
	adds	r3, r3, #48
	str	r2, [r3, #-40]
	bfi	r1, r2, #0, #1
	strb	r1, [r3, #-32]
	cmp	r0, r3
	bne	.L232
	str	r2, [r4, #52]
	strd	r2, r2, [r4, #36]
.L233:
	ldr	r3, [r7]
	lsls	r2, r3, #23
	bpl	.L236
	movw	r2, #427
	ldr	r1, .L315+12
	mov	r0, r4
	bl	h264e_dpb_dump_frm
.L236:
	add	r3, r4, #288
	add	r9, r5, #24
	add	fp, r5, #88
	mov	r8, #0
	mov	r10, r3
	str	r3, [sp, #28]
.L239:
	mov	r2, r9
	ldr	r1, [r4, #60]
	ldr	r0, [sp, #20]
	bl	find_cpb_frame
	mov	r7, r0
	str	r0, [r10], #4
	cbz	r0, .L237
	ldr	r3, [r0, #8]
	cbnz	r3, .L238
	ldr	r3, [r0, #4]
	mov	r2, #438
	ldr	r1, .L315+4
	ldr	r0, .L315+16
	bl	printk
.L238:
	movs	r3, #1
	add	r8, r8, r3
	str	r3, [r7, #8]
.L237:
	add	r9, r9, #8
	cmp	r9, fp
	bne	.L239
	ldr	r3, [r4, #52]
	cmp	r3, r8
	beq	.L240
	mov	r1, #444
	ldr	r3, .L315+12
	str	r1, [sp]
	ldr	r2, .L315+20
	ldr	r0, .L315+24
	bl	printk
.L240:
	ldr	r7, .L315
	ldr	r3, [r7]
	lsls	r3, r3, #23
	bpl	.L241
	ldr	r3, [r4, #52]
	mov	r2, #446
	ldr	r1, .L315+4
	ldr	r0, .L315+28
	strd	r8, r3, [sp]
	ldr	r3, [sp, #16]
	bl	printk
.L241:
	add	r3, r4, #376
	add	r2, r6, #20
.L242:
	ldr	r1, [r6], #4	@ unaligned
	cmp	r6, r2
	str	r1, [r3], #4	@ unaligned
	bne	.L242
	mov	r2, #816
	ldr	r1, [sp, #20]
	add	r0, r4, #1216
	bl	memcpy
	movs	r3, #0
	str	r3, [r4, #88]
	ldr	r3, [r7]
	lsls	r6, r3, #23
	bpl	.L243
	ldr	r3, [sp, #16]
	movw	r2, #457
	ldr	r1, .L315+4
	ldr	r0, .L315+32
	bl	printk
.L243:
	ldr	r6, [sp, #20]
	movs	r7, #0
	ldr	r9, .L315
	mov	fp, r7
.L260:
	ldr	r3, [r4, #364]
	add	r2, r4, #320
	str	r3, [sp, #32]
	ldr	r3, [r4, #360]
	str	r3, [sp, #36]
	ldrd	r8, r3, [r4, #368]
	str	r3, [sp, #24]
	ldr	r3, [sp, #28]
.L245:
	ldr	r1, [r3], #4
	cmp	r1, r6
	beq	.L271
	cmp	r2, r3
	bne	.L245
	mov	r10, #0
.L244:
	ldr	r3, .L315
	ldr	r3, [r3]
	lsls	r0, r3, #23
	bpl	.L246
	str	r10, [sp]
	mov	r3, fp
	movw	r2, #473
	ldr	r1, .L315+4
	ldr	r0, .L315+36
	bl	printk
.L246:
	cmp	r10, #0
	beq	.L247
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq	.L248
	ldrb	r3, [r6, #16]	@ zero_extendqisi2
	lsls	r1, r3, #31
	bmi	.L249
.L251:
	mov	r1, #478
	ldr	r3, .L315+12
	str	r1, [sp]
	ldr	r2, .L315+40
	ldr	r0, .L315+24
	bl	printk
.L250:
	movw	r2, #479
	ldr	r1, .L315+12
	mov	r0, r4
	bl	h264e_dpb_dump_frm
.L249:
	add	fp, fp, #1
	adds	r6, r6, #48
	cmp	fp, #17
	bne	.L260
	ldr	r3, [r4, #88]
	cbnz	r3, .L262
	movw	r1, #534
	ldr	r3, .L315+12
	str	r1, [sp]
	ldr	r2, .L315+44
	ldr	r0, .L315+24
	bl	printk
	ldr	r3, [r4, #88]
	cbnz	r3, .L262
	ldr	r3, [sp, #16]
	movw	r2, #537
	ldr	r1, .L315+4
	ldr	r0, .L315+48
	bl	printk
.L262:
	ldr	r3, [r9]
	lsls	r6, r3, #23
	bpl	.L264
	ldrh	r3, [r5, #22]
	movw	r2, #539
	ldr	r1, .L315+4
	ldr	r0, .L315+52
	str	r3, [sp]
	ldr	r3, [sp, #16]
	bl	printk
.L264:
	add	r2, r5, #16
	ldr	r1, [r4, #60]
	ldr	r0, [sp, #20]
	bl	find_cpb_frame
	str	r0, [r4, #92]
	cbnz	r0, .L265
	ldr	r3, [r4, #88]
	str	r3, [r4, #92]
.L265:
	ldr	r3, [r9]
	lsls	r0, r3, #23
	bpl	.L266
	ldr	r3, [r4, #92]
	movw	r2, #545
	ldr	r1, .L315+4
	ldr	r0, .L315+56
	ldr	r3, [r3]
	str	r3, [sp, #4]
	ldr	r3, [r4, #88]
	ldr	r3, [r3]
	str	r3, [sp]
	ldr	r3, [sp, #16]
	bl	printk
.L266:
	mov	r1, r5
	mov	r0, r4
	bl	h264e_dpb_build_list
	ldr	r3, [r9]
	lsls	r1, r3, #23
	bpl	.L267
	movw	r2, #550
	ldr	r1, .L315+12
	mov	r0, r4
	bl	h264e_dpb_dump_frm
.L267:
	mov	r0, r4
	bl	h264e_dpb_build_marking
	ldr	r3, [r9]
	lsls	r2, r3, #23
	bpl	.L269
	movw	r2, #555
	ldr	r1, .L315+12
	mov	r0, r4
	bl	h264e_dpb_dump_frm
	ldr	r3, [r9]
	lsls	r3, r3, #23
	bpl	.L269
	mov	r3, r4
	movw	r2, #557
	ldr	r1, .L315+4
	ldr	r0, .L315+60
	bl	printk
.L269:
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L231:
	ldrh	r3, [r5, #14]
	ldr	r2, [r4, #356]
	cmp	r3, r2
	bne	.L233
	ldr	r2, [r7]
	lsls	r1, r2, #23
	bpl	.L234
	mov	r2, #420
	ldr	r1, .L315+4
	ldr	r0, .L315+64
	bl	printk
.L234:
	add	r3, r4, #376
	add	r1, r4, #396
	mov	r2, r6
.L235:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L235
	mov	r2, #816
	add	r1, r4, #1216
	ldr	r0, [sp, #20]
	bl	memcpy
	b	.L233
.L271:
	mov	r10, #1
	b	.L244
.L316:
	.align	2
.L315:
	.word	h264e_debug
	.word	.LANCHOR0+187
	.word	.LC49
	.word	.LANCHOR0+202
	.word	.LC51
	.word	.LC52
	.word	.LC22
	.word	.LC53
	.word	.LC54
	.word	.LC55
	.word	.LC56
	.word	.LC60
	.word	.LC61
	.word	.LC62
	.word	.LC63
	.word	.LC15
	.word	.LC50
.L248:
	movw	r1, #477
	ldr	r3, .L317
	ldr	r2, .L317+4
	str	r1, [sp]
	ldr	r0, .L317+8
	bl	printk
	ldrb	r3, [r6, #16]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bpl	.L251
	b	.L250
.L247:
	cbz	r7, .L252
	ldrb	r3, [r6, #16]	@ zero_extendqisi2
	str	r10, [r6, #8]
	bfi	r3, r10, #0, #1
	strb	r3, [r6, #16]
	b	.L249
.L252:
	str	r6, [r4, #88]
	ldrd	r2, [r5, #8]
	strd	r2, [r6, #16]
	ldrh	r3, [r5, #14]
	str	r3, [r6, #4]
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	lsls	r3, r3, #26
	bpl	.L253
	mov	r8, r7
	str	r7, [r6, #24]
	str	r7, [r6, #32]
	str	r7, [sp, #24]
.L254:
	ldrb	r3, [r5, #9]	@ zero_extendqisi2
	mov	r10, #1
	str	r10, [r6, #8]
	and	r3, r3, #15
	str	r3, [r6, #28]
	ldrh	r3, [r5, #14]
	str	r3, [r4, #356]
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	str	r8, [r4, #368]
	eor	r3, r3, #64
	ubfx	r3, r3, #6, #1
	strd	r3, r7, [r4, #360]
	ldr	r3, [sp, #24]
	str	r3, [r4, #372]
	ldr	r3, .L317+12
	ldr	r3, [r3]
	lsls	r7, r3, #23
	bpl	.L272
	ldr	r3, [r6, #32]
	movw	r2, #531
	ldr	r1, .L317+16
	ldr	r0, .L317+20
	str	r3, [sp, #8]
	ldr	r3, [r6, #24]
	strd	fp, r3, [sp]
	ldr	r3, [sp, #16]
	bl	printk
.L272:
	mov	r7, r10
	b	.L249
.L253:
	ldrd	r3, r2, [sp, #32]
	add	r3, r3, r2
	mov	r7, r3
	ldr	r3, [r4, #64]
	cmp	r3, r7
	it	le
	movle	r7, #0
	str	r7, [r6, #24]
	ldr	r3, [r4, #72]
	cbnz	r3, .L256
	add	r8, r8, #2
.L257:
	ldr	r3, [r4, #68]
	cmp	r3, r8
	itttt	le
	ldrle	r2, [sp, #24]
	movle	r8, #0
	addle	r2, r2, #1
	strle	r2, [sp, #24]
	ldr	r2, [sp, #24]
	mla	r3, r2, r3, r8
	str	r3, [r6, #32]
	b	.L254
.L256:
	cmp	r3, #2
	bne	.L258
	ldr	r3, [r4, #360]
	add	r8, r8, r3
	add	r8, r8, #1
	b	.L257
.L258:
	mov	r2, #510
	ldr	r1, .L317+16
	ldr	r0, .L317+24
	bl	printk
	b	.L257
.L318:
	.align	2
.L317:
	.word	.LANCHOR0+202
	.word	.LC57
	.word	.LC22
	.word	h264e_debug
	.word	.LANCHOR0+187
	.word	.LC59
	.word	.LC58
	.fnend
	.size	h264e_dpb_proc, .-h264e_dpb_proc
	.align	1
	.global	h264e_dpb_check
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_dpb_check, %function
h264e_dpb_check:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #36
	sub	sp, sp, #36
	ldr	r5, .L403
	mov	r4, r0
	str	r1, [sp, #28]
	ldr	r7, [r0, #88]
	ldr	r3, [r5]
	lsls	r6, r3, #23
	bpl	.L321
	mov	r3, r0
	ldr	r6, [r0, #92]
	movw	r2, #569
	ldr	r1, .L403+4
	ldr	r0, .L403+8
	bl	printk
	ldr	r3, [r5]
	lsls	r0, r3, #23
	bpl	.L321
	ldr	r3, [r6, #4]
	mov	r2, #572
	ldr	r1, .L403+4
	ldr	r0, .L403+12
	str	r3, [sp]
	ldr	r3, [r7, #4]
	bl	printk
.L321:
	ldrb	r3, [r7, #16]	@ zero_extendqisi2
	lsls	r1, r3, #25
	bpl	.L323
	movs	r2, #0
	bfi	r3, r2, #0, #1
	str	r2, [r7, #8]
	strb	r3, [r7, #16]
.L324:
	ldr	r3, [r5]
	lsls	r1, r3, #23
	bpl	.L345
	ldr	r3, [r4, #52]
	movw	r2, #679
	ldr	r1, .L403+4
	ldr	r0, .L403+16
	str	r3, [sp]
	ldr	r3, [r7, #4]
	bl	printk
.L345:
	ldr	r6, [sp, #28]
	add	r9, r4, #288
	add	r10, r4, #320
	add	fp, r4, #400
	adds	r6, r6, #88
	mov	r8, #0
.L347:
	mov	r2, r6
	ldr	r1, [r4, #60]
	mov	r0, fp
	bl	find_cpb_frame
	str	r0, [r9], #4
	cbz	r0, .L346
	add	r8, r8, #1
.L346:
	cmp	r10, r9
	add	r6, r6, #8
	bne	.L347
	ldr	r3, [r5]
	lsls	r2, r3, #23
	bpl	.L348
	ldr	r3, [r4, #52]
	mov	r2, #688
	ldr	r1, .L403+4
	ldr	r0, .L403+20
	strd	r8, r3, [sp]
	ldr	r3, [r7, #4]
	bl	printk
.L348:
	ldr	r3, [r4, #52]
	cmp	r3, r8
	beq	.L349
	movw	r1, #689
	ldr	r3, .L403+24
	str	r1, [sp]
	ldr	r2, .L403+28
	ldr	r0, .L403+32
	bl	printk
.L349:
	ldr	r3, [r5]
	lsls	r3, r3, #23
	bpl	.L319
	mov	r3, r4
	movw	r2, #691
	ldr	r1, .L403+4
	ldr	r0, .L403+36
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	printk
.L323:
	ldr	r2, [r5]
	ldrd	r6, r10, [r4, #52]
	lsls	r2, r2, #23
	bpl	.L325
	tst	r3, #128
	ldr	r2, .L403+40
	ldr	r3, .L403+44
	strd	r6, r10, [sp, #4]
	it	eq
	moveq	r3, r2
	ldr	r1, .L403+4
	str	r3, [sp]
	mov	r2, #584
	ldr	r3, [r7, #4]
	ldr	r0, .L403+48
	bl	printk
.L325:
	ldrsb	r3, [r7, #16]
	adds	r6, r6, #1
	cmp	r3, #0
	bge	.L327
	ldr	r3, [r7, #28]
	add	r8, r4, #400
	ldr	fp, .L403+4
	mov	r9, #0
	str	r3, [sp, #20]
	ldr	r3, [r4, #48]
	adds	r3, r3, #1
	str	r3, [r4, #48]
.L328:
	ldr	r3, [r4, #60]
	cmp	r3, r9
	bgt	.L332
.L333:
	cmp	r6, r10
	ble	.L334
	ldr	r3, [r4, #48]
	cmp	r3, r10
	ble	.L344
	mov	r1, #624
	ldr	r3, .L403+24
	str	r1, [sp]
	ldr	r2, .L403+52
	ldr	r0, .L403+32
	bl	printk
.L344:
	ldr	r3, [r5]
	lsls	r3, r3, #23
	bpl	.L336
	ldr	r3, [r4, #48]
	movw	r2, #631
	str	r10, [sp, #4]
	ldr	r1, .L403+4
	str	r3, [sp]
	ldr	r0, .L403+56
	ldr	r3, [r4, #44]
	bl	printk
.L336:
	mov	r9, #0
	add	r8, r4, #400
	mov	fp, r9
	mvn	r3, #-2147483648
	str	r3, [sp, #24]
.L337:
	ldr	r3, [r4, #60]
	cmp	r3, fp
	bgt	.L341
	cmp	r9, #0
	beq	.L334
	ldr	r3, [r5]
	lsls	r0, r3, #23
	bpl	.L342
	ldr	r3, [r9, #32]
	movw	r2, #661
	ldr	r1, .L403+4
	ldr	r0, .L403+60
	str	r3, [sp]
	ldr	r3, [r9, #4]
	bl	printk
.L342:
	movs	r3, #0
	str	r3, [r9, #8]
	ldr	r3, [r4, #44]
	subs	r3, r3, #1
	str	r3, [r4, #44]
	cmp	r3, #0
	bge	.L343
	movw	r1, #665
	ldr	r3, .L403+24
	str	r1, [sp]
	ldr	r2, .L403+64
	ldr	r0, .L403+32
	bl	printk
.L343:
	subs	r6, r6, #1
	cmp	r10, r6
	bne	.L344
	mov	r6, r10
.L334:
	cmp	r6, r10
	str	r6, [r4, #52]
	ble	.L324
	mov	r3, r6
	str	r10, [sp]
	movw	r2, #674
	ldr	r1, .L403+4
	ldr	r0, .L403+68
	bl	printk
	movw	r1, #675
	ldr	r3, .L403+24
	ldr	r2, .L403+72
	ldr	r0, .L403+32
	str	r1, [sp]
	bl	printk
	b	.L324
.L332:
	cmp	r8, r7
	beq	.L329
	ldr	r3, [r8, #8]
	cmp	r3, #0
	beq	.L329
	movs	r3, #48
	mla	r3, r3, r9, r4
	ldrb	r3, [r3, #416]	@ zero_extendqisi2
	lsls	r0, r3, #31
	bpl	.L329
	lsls	r1, r3, #25
	bpl	.L330
	movw	r1, #606
	ldr	r3, .L403+24
	str	r1, [sp]
	ldr	r2, .L403+76
	ldr	r0, .L403+32
	bl	printk
.L330:
	movs	r3, #48
	mla	r3, r3, r9, r4
	ldrsb	r2, [r3, #416]
	cmp	r2, #0
	bge	.L329
	ldr	r2, [r8, #28]
	ldr	r1, [sp, #20]
	cmp	r2, r1
	bne	.L329
	movs	r1, #0
	str	r1, [r8, #8]
	ldrb	r2, [r3, #416]	@ zero_extendqisi2
	bfi	r2, r1, #0, #1
	strb	r2, [r3, #416]
	ldr	r3, [r5]
	lsls	r2, r3, #23
	bpl	.L331
	ldr	r3, [r8]
	movw	r2, #615
	mov	r1, fp
	ldr	r0, .L403+80
	str	r3, [sp, #4]
	ldr	r3, [r7, #28]
	str	r3, [sp]
	ldr	r3, [r7, #4]
	bl	printk
.L331:
	ldr	r3, [r4, #48]
	subs	r6, r6, #1
	subs	r3, r3, #1
	str	r3, [r4, #48]
.L329:
	add	r9, r9, #1
	add	r8, r8, #48
	b	.L328
.L327:
	ldr	r3, [r4, #44]
	adds	r3, r3, #1
	str	r3, [r4, #44]
	b	.L333
.L404:
	.align	2
.L403:
	.word	h264e_debug
	.word	.LANCHOR0+217
	.word	.LC14
	.word	.LC64
	.word	.LC76
	.word	.LC77
	.word	.LANCHOR0+233
	.word	.LC52
	.word	.LC22
	.word	.LC15
	.word	.LC6
	.word	.LC5
	.word	.LC65
	.word	.LC68
	.word	.LC69
	.word	.LC72
	.word	.LC73
	.word	.LC74
	.word	.LC75
	.word	.LC66
	.word	.LC67
.L341:
	ldr	r3, [r5]
	lsls	r0, r3, #23
	bpl	.L338
	ldr	r3, [r8, #32]
	movw	r2, #637
	ldr	r1, .L405
	ldr	r0, .L405+4
	str	r3, [sp, #4]
	ldr	r3, [r8, #24]
	str	r3, [sp]
	ldr	r3, [r8, #4]
	bl	printk
.L338:
	ldr	r3, [r8, #8]
	cbz	r3, .L339
	movs	r3, #48
	mla	r3, r3, fp, r4
	ldrb	r3, [r3, #416]	@ zero_extendqisi2
	lsls	r1, r3, #31
	bpl	.L339
	lsls	r2, r3, #25
	bpl	.L340
	movw	r1, #645
	ldr	r3, .L405+8
	str	r1, [sp]
	ldr	r2, .L405+12
	ldr	r0, .L405+16
	bl	printk
.L340:
	movs	r3, #48
	mla	r3, r3, fp, r4
	ldrsb	r3, [r3, #416]
	cmp	r3, #0
	blt	.L339
	ldr	r3, [r8, #32]
	ldr	r2, [sp, #24]
	cmp	r3, r2
	str	r3, [sp, #20]
	bge	.L339
	ldr	r3, [r5]
	lsls	r3, r3, #23
	bpl	.L352
	ldr	r3, [sp, #20]
	movw	r2, #655
	ldr	r1, .L405
	ldr	r0, .L405+20
	str	r3, [sp]
	ldr	r3, [r8, #4]
	bl	printk
.L352:
	ldr	r3, [sp, #20]
	mov	r9, r8
	str	r3, [sp, #24]
.L339:
	add	fp, fp, #1
	add	r8, r8, #48
	b	.L337
.L319:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L406:
	.align	2
.L405:
	.word	.LANCHOR0+217
	.word	.LC70
	.word	.LANCHOR0+233
	.word	.LC66
	.word	.LC22
	.word	.LC71
	.fnend
	.size	h264e_dpb_check, .-h264e_dpb_check
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.9646, %object
	.size	__func__.9646, 21
__func__.9646:
	.ascii	"h264e_dpb_dump_listX\000"
	.type	__func__.9633, %object
	.size	__func__.9633, 19
__func__.9633:
	.ascii	"h264e_dpb_dump_frm\000"
	.type	__func__.9653, %object
	.size	__func__.9653, 20
__func__.9653:
	.ascii	"h264e_dpb_dump_list\000"
	.type	__func__.9660, %object
	.size	__func__.9660, 15
__func__.9660:
	.ascii	"h264e_dpb_init\000"
	.type	__func__.9674, %object
	.size	__func__.9674, 16
__func__.9674:
	.ascii	"h264e_dpb_setup\000"
	.type	__func__.9682, %object
	.size	__func__.9682, 15
__func__.9682:
	.ascii	"find_cpb_frame\000"
	.type	__FUNCTION__.9684, %object
	.size	__FUNCTION__.9684, 15
__FUNCTION__.9684:
	.ascii	"find_cpb_frame\000"
	.type	__func__.9716, %object
	.size	__func__.9716, 21
__func__.9716:
	.ascii	"h264e_dpb_build_list\000"
	.type	__FUNCTION__.9720, %object
	.size	__FUNCTION__.9720, 21
__FUNCTION__.9720:
	.ascii	"h264e_dpb_build_list\000"
	.type	__func__.9748, %object
	.size	__func__.9748, 24
__func__.9748:
	.ascii	"h264e_dpb_build_marking\000"
	.type	__func__.9765, %object
	.size	__func__.9765, 15
__func__.9765:
	.ascii	"h264e_dpb_proc\000"
	.type	__FUNCTION__.9769, %object
	.size	__FUNCTION__.9769, 15
__FUNCTION__.9769:
	.ascii	"h264e_dpb_proc\000"
	.type	__func__.9803, %object
	.size	__func__.9803, 16
__func__.9803:
	.ascii	"h264e_dpb_check\000"
	.type	__FUNCTION__.9809, %object
	.size	__FUNCTION__.9809, 16
__FUNCTION__.9809:
	.ascii	"h264e_dpb_check\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"I\000"
.LC1:
	.ascii	"i\000"
.LC2:
	.ascii	"P\000"
.LC3:
	.ascii	"non\000"
.LC4:
	.ascii	"p\000"
.LC5:
	.ascii	"lt\000"
.LC6:
	.ascii	"st\000"
.LC7:
	.ascii	"\0016%s:%d: frm  %2d   %d  %-3d %s    %-3d %-3s %-3"
	.ascii	"d %-4x %-3d\012\000"
.LC8:
	.ascii	"\0016%s:%d: dump dpb frame info in %s line %d\012\000"
.LC9:
	.ascii	"\0016%s:%d: dpb %p total count %d size %d\012\000"
.LC10:
	.ascii	"\0016%s:%d: dpb slot use seq type tid ref idx mode "
	.ascii	"arg\012\000"
.LC11:
	.ascii	"\0016%s:%d: dump dpb list info\012\000"
.LC12:
	.ascii	"\0016%s:%d: dpb  size %d used %d st %d lt %d\012\000"
.LC13:
	.ascii	"\0016%s:%d: list slot use seq type tid ref idx mode"
	.ascii	" arg \012\000"
.LC14:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC15:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC16:
	.ascii	"\0016%s:%d: max  ref frm num %d total slot %d\012\000"
.LC17:
	.ascii	"\0016%s:%d: log2 max frm num %d -> %d\012\000"
.LC18:
	.ascii	"\0016%s:%d: log2 max poc lsb %d -> %d\012\000"
.LC19:
	.ascii	"\0016%s:%d: frm %d start finding slot\012\000"
.LC20:
	.ascii	"\0016%s:%d: frm %d match slot %d valid %d\012\000"
.LC21:
	.ascii	"p->is_non_ref == frm->is_non_ref\000"
.LC22:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC23:
	.ascii	"p->is_lt_ref == frm->is_lt_ref\000"
.LC24:
	.ascii	"p->lt_idx == frm->lt_idx\000"
.LC25:
	.ascii	"p->temporal_id == frm->temporal_id\000"
.LC26:
	.ascii	"\0013%s:%d: can not find match frm %d\012\000"
.LC27:
	.ascii	"\0016%s:%d: cpb init scaning start\012\000"
.LC28:
	.ascii	"!frm->is_non_ref\000"
.LC29:
	.ascii	"\0016%s:%d: idx %d frm %d valid %d is_non_ref %d lt"
	.ascii	"_ref %d\012\000"
.LC30:
	.ascii	"\0016%s:%d: found st %d st_size %d %p\012\000"
.LC31:
	.ascii	"\0016%s:%d: found lt %d lt_size %d %p\012\000"
.LC32:
	.ascii	"\0016%s:%d: cpb init scaning done\012\000"
.LC33:
	.ascii	"\0016%s:%d: dpb_size %d st_size %d lt_size %d\012\000"
.LC34:
	.ascii	"\0016%s:%d: dpb st list before sort\012\000"
.LC35:
	.ascii	"\0016%s:%d: dpb st list after  sort\012\000"
.LC36:
	.ascii	"\0016%s:%d: dpb lt list before sort\012\000"
.LC37:
	.ascii	"\0016%s:%d: dpb lt list after  sort\012\000"
.LC38:
	.ascii	"dpb->dpb_size >= st_size + lt_size\000"
.LC39:
	.ascii	"\0016%s:%d: refer curr %d def %d set %d reorder %d\012"
	.ascii	"\000"
.LC40:
	.ascii	"\0016%s:%d: reorder to frm %d\012\000"
.LC41:
	.ascii	"!refr->status.is_non_ref\000"
.LC42:
	.ascii	"\0016%s:%d: reorder lt idx %d \012\000"
.LC43:
	.ascii	"\0016%s:%d: reorder st cur %d refr %d diff - 1 %d\012"
	.ascii	"\000"
.LC44:
	.ascii	"\0016%s:%d: refer NULL\012\000"
.LC45:
	.ascii	"\0016%s:%d: frm %d ref %d lt %d T%d\012\000"
.LC46:
	.ascii	"\0016%s:%d: add mmco 4 %d\012\000"
.LC47:
	.ascii	"\0016%s:%d: add mmco 6 %d\012\000"
.LC48:
	.ascii	"\0016%s:%d: dpb size %d used %d\012\000"
.LC49:
	.ascii	"\0016%s:%d: enter %p total %d\012\000"
.LC50:
	.ascii	"\0016%s:%d: NOTE: reenc found at %d\012\000"
.LC51:
	.ascii	"\0013%s:%d: warning frm %d is used by cpb but on no"
	.ascii	"t used status\012\000"
.LC52:
	.ascii	"dpb->used_size == used_size\000"
.LC53:
	.ascii	"\0016%s:%d: frm %d init cpb used size %d vs %d\012\000"
.LC54:
	.ascii	"\0016%s:%d: frm %d start finding slot for storage\012"
	.ascii	"\000"
.LC55:
	.ascii	"\0016%s:%d: slot %2d check in cpb init valid %d\012"
	.ascii	"\000"
.LC56:
	.ascii	"p->status.valid\000"
.LC57:
	.ascii	"p->on_used\000"
.LC58:
	.ascii	"\0013%s:%d: do not support poc type 1\012\000"
.LC59:
	.ascii	"\0016%s:%d: frm %d update slot %d with frame_num %d"
	.ascii	" poc %d\012\000"
.LC60:
	.ascii	"dpb->curr\000"
.LC61:
	.ascii	"\0013%s:%d: frm %d failed to find a slot for curr \012"
	.ascii	"\000"
.LC62:
	.ascii	"\0016%s:%d: frm %d start finding slot for refr %d\012"
	.ascii	"\000"
.LC63:
	.ascii	"\0016%s:%d: frm %d curr %d refr %d start building l"
	.ascii	"ist\012\000"
.LC64:
	.ascii	"\0016%s:%d: frm %d refr -> frm %d ready\012\000"
.LC65:
	.ascii	"\0016%s:%d: frm %d %s insert dpb used %d max %d\012"
	.ascii	"\000"
.LC66:
	.ascii	"!tmp->status.is_non_ref\000"
.LC67:
	.ascii	"\0016%s:%d: frm %d lt_idx %d replace %d\012\000"
.LC68:
	.ascii	"dpb->lt_size <= dpb_size\000"
.LC69:
	.ascii	"\0016%s:%d: sliding window cpb proc: st %d lt %d ma"
	.ascii	"x %d\012\000"
.LC70:
	.ascii	"\0016%s:%d: frm %d num %d poc %d\012\000"
.LC71:
	.ascii	"\0016%s:%d: frm %d update smallest poc to %d\012\000"
.LC72:
	.ascii	"\0016%s:%d: removing frm %d poc %d\012\000"
.LC73:
	.ascii	"dpb->st_size >= 0\000"
.LC74:
	.ascii	"\0013%s:%d: error found used_size %d > dpb_size %d\012"
	.ascii	"\000"
.LC75:
	.ascii	"used_size <= dpb_size\000"
.LC76:
	.ascii	"\0016%s:%d: curr %d done used_size %d\012\000"
.LC77:
	.ascii	"\0016%s:%d: curr %d cpb final used_size %d vs %d\012"
	.ascii	"\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
