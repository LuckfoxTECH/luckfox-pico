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
	.file	"hal_h265e_vepu540c.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	hal_h265e_v540c_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_v540c_init, %function
hal_h265e_v540c_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r4, r0
	ldr	r6, .L30
	mov	r7, r1
	ldr	r3, [r6]
	lsls	r2, r3, #29
	bpl	.L2
	movw	r3, #1084
	ldr	r1, .L30+4
	mov	r2, r3
	ldr	r0, .L30+8
	bl	printk
.L2:
	mov	r1, #596
	ldr	r0, .L30+12
	bl	mpp_osal_calloc
	mov	r1, #596
	str	r0, [r4, #72]
	ldr	r0, .L30+12
	bl	mpp_osal_calloc
	mov	r1, #6656
	str	r0, [r4, #76]
	ldr	r0, .L30+12
	bl	mpp_osal_calloc
	movs	r1, #28
	str	r0, [r4, #68]
	ldr	r0, .L30+12
	bl	mpp_osal_calloc
	movs	r1, #16
	str	r0, [r4, #484]
	add	r0, r7, #36
	ldr	r3, [r7, #4]
	str	r3, [r4, #444]
	ldr	r3, [r7, #8]
	str	r3, [r4, #516]
	ldr	r3, [r7, #12]
	str	r3, [r4, #520]
	ldr	r3, [r7, #40]
	str	r3, [r4, #636]
	ldr	r3, [r7, #16]
	str	r3, [r4, #524]
	ldr	r3, [r7, #20]
	str	r3, [r4, #528]
	ldr	r3, [r7, #24]
	str	r3, [r4, #532]
	ldr	r3, [r7, #28]
	str	r3, [r4, #652]
	movs	r3, #0
	str	r3, [r4, #424]
	str	r3, [r4, #388]
	movs	r3, #1
	str	r3, [r4, #464]
	str	r1, [r7, #32]
	bl	mpp_dev_init
	mov	r5, r0
	cbz	r0, .L3
	mov	r3, r0
	ldr	r1, .L30+4
	ldr	r0, .L30+16
	movw	r2, #1108
.L29:
	bl	printk
.L1:
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, pc}
.L3:
	ldr	r2, [r7, #36]
	ldr	r3, [r4, #68]
	str	r0, [r4, #436]
	add	r3, r3, #5152
	str	r2, [r4, #64]
	adds	r3, r3, #24
	strd	r3, r2, [r4, #428]
	movs	r2, #2
	str	r2, [r4, #392]
	ldr	r3, [r7, #4]
	strd	r2, r2, [r3, #304]
	add	r2, r3, #440
	ldr	r1, [r4, #528]
	cmp	r1, #0
	beq	.L5
	ldr	r7, .L30+20
	add	ip, r7, #64
	mov	lr, ip
.L6:
	mov	r4, r7
	adds	r2, r2, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, ip
	str	r0, [r2, #-8]	@ unaligned
	mov	r7, r4
	str	r1, [r2, #-4]	@ unaligned
	bne	.L6
	ldr	r7, .L30+20
	add	r4, r3, #504
.L7:
	mov	r2, r7
	adds	r4, r4, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r4, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r4, #-4]	@ unaligned
	bne	.L7
	ldr	r7, .L30+24
	add	r4, r3, #312
	add	ip, r7, #64
	mov	lr, ip
.L8:
	mov	r2, r7
	adds	r4, r4, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, ip
	str	r0, [r4, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r4, #-4]	@ unaligned
	bne	.L8
	ldr	r4, .L30+24
	add	r3, r3, #376
.L9:
	mov	r2, r4
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r3, #-8]	@ unaligned
	mov	r4, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L9
.L10:
	ldr	r3, [r6]
	lsls	r3, r3, #29
	bpl	.L1
	movw	r3, #1138
	ldr	r1, .L30+4
	mov	r2, r3
	ldr	r0, .L30+28
	b	.L29
.L5:
	ldr	r7, .L30+32
	add	ip, r7, #64
	mov	lr, ip
.L11:
	mov	r4, r7
	adds	r2, r2, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, ip
	str	r0, [r2, #-8]	@ unaligned
	mov	r7, r4
	str	r1, [r2, #-4]	@ unaligned
	bne	.L11
	ldr	r7, .L30+32
	add	r4, r3, #504
.L12:
	mov	r2, r7
	adds	r4, r4, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r4, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r4, #-4]	@ unaligned
	bne	.L12
	ldr	r7, .L30+36
	add	r4, r3, #312
	add	ip, r7, #64
	mov	lr, ip
.L13:
	mov	r2, r7
	adds	r4, r4, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, ip
	str	r0, [r4, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r4, #-4]	@ unaligned
	bne	.L13
	ldr	r4, .L30+36
	add	r3, r3, #376
.L14:
	mov	r2, r4
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r3, #-8]	@ unaligned
	mov	r4, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L14
	b	.L10
.L31:
	.align	2
.L30:
	.word	hal_h265e_debug
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR0+21
	.word	.LC1
	.word	.LANCHOR1
	.word	.LANCHOR1+64
	.word	.LC2
	.word	.LANCHOR1+128
	.word	.LANCHOR1+192
	.fnend
	.size	hal_h265e_v540c_init, .-hal_h265e_v540c_init
	.global	__aeabi_uidiv
	.global	__aeabi_idiv
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_h265_set_feedback, %function
vepu540c_h265_set_feedback:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #36
	sub	sp, sp, #36
	ldr	r3, [r0, #444]
	mov	r4, r0
	ldr	r2, [r0, #68]
	ldr	r6, [r1, #4]
	ldr	r5, [r4, #72]
	str	r2, [sp]
	ldrd	r1, r2, [r3, #12]
	ldr	r10, [r5]
	adds	r3, r1, #63
	it	mi
	addmi	r3, r1, #126
	adds	r0, r2, #63
	it	mi
	addmi	r0, r2, #126
	asrs	r3, r3, #6
	asrs	r0, r0, #6
	muls	r3, r0, r3
	adds	r0, r1, #15
	it	mi
	addmi	r0, r1, #30
	ldrh	r1, [r5, #196]
	asrs	r0, r0, #4
	str	r3, [sp, #4]
	adds	r3, r2, #15
	it	mi
	addmi	r3, r2, #30
	asrs	r3, r3, #4
	muls	r0, r3, r0
	ldr	r3, [sp, #4]
	lsls	r3, r3, #8
	str	r3, [sp, #20]
	ldrh	r3, [r5, #204]
	add	r1, r1, r3
	ldrh	r3, [r5, #212]
	add	r1, r1, r3
	ldrh	r3, [r5, #220]
	adds	r3, r1, r3
	ldrh	r1, [r5, #198]
	str	r3, [sp, #24]
	ldrh	r3, [r5, #206]
	add	r1, r1, r3
	ldrh	r3, [r5, #214]
	add	r1, r1, r3
	ldrh	r3, [r5, #222]
	adds	r3, r1, r3
	ldrh	r1, [r5, #200]
	str	r3, [sp, #8]
	ldrh	r3, [r5, #208]
	add	r1, r1, r3
	ldrh	r3, [r5, #216]
	add	r1, r1, r3
	ldrh	r3, [r5, #224]
	adds	r3, r1, r3
	ldrh	r1, [r5, #202]
	str	r3, [sp, #12]
	ldrh	r3, [r5, #210]
	add	r1, r1, r3
	ldrh	r3, [r5, #218]
	add	r1, r1, r3
	ldrh	r3, [r5, #226]
	adds	r3, r1, r3
	str	r3, [sp, #16]
	ldrh	r8, [r5, #228]
	ldrh	r3, [r5, #236]
	ldrh	r9, [r5, #244]
	add	r8, r8, r3
	ldrh	r1, [r5, #254]
	add	r8, r8, r9
	ldrh	r9, [r5, #252]
	ldrh	r7, [r5, #234]
	add	r3, r8, r9
	ldrh	r8, [r5, #230]
	str	r3, [sp, #28]
	ldrh	r3, [r5, #238]
	ldrh	r9, [r5, #232]
	add	r8, r8, r3
	ldrh	r3, [r5, #246]
	add	r8, r8, r3
	ldrh	r3, [r5, #240]
	add	r8, r8, r1
	ldrh	r1, [r5, #248]
	add	r9, r9, r3
	ldr	r3, [r4, #528]
	add	r9, r9, r1
	ldrh	r1, [r5, #256]
	add	r9, r9, r1
	ldrh	r1, [r5, #242]
	add	r7, r7, r1
	ldrh	r1, [r5, #250]
	add	r7, r7, r1
	ldrh	r1, [r5, #258]
	add	r7, r7, r1
	cmp	r3, #0
	bne	.L37
	add	r1, r8, r8, lsl #4
	movs	r2, #22
	mla	r3, r2, r9, r1
	movs	r1, #24
	mla	r1, r1, r7, r3
.L89:
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #16]
	mov	ip, #6
	lsrs	r1, r1, #2
	add	r3, r3, r3, lsl #2
	mla	r3, ip, r2, r3
	ldr	r2, [sp, #8]
	rsb	ip, r0, r0, lsl #4
	add	r3, r3, r2, lsl #2
	lsrs	r2, r3, #2
	movs	r3, #100
	muls	r1, r3, r1
	cmp	r1, ip
	bls	.L39
	movs	r3, #200
.L90:
	str	r3, [r6, #204]
	movs	r3, #100
	muls	r3, r2, r3
	movs	r2, #30
	muls	r2, r0, r2
	cmp	r3, r2
	bhi	.L63
	movs	r2, #13
	muls	r0, r2, r0
	cmp	r3, r0
	ite	ls
	movls	r3, #0
	movhi	r3, #1
.L43:
	str	r3, [r6, #208]
	ldr	r3, .L94
	ldr	r3, [r3]
	lsls	r2, r3, #29
	bpl	.L44
	movw	r3, #2461
	ldr	r1, .L94+4
	mov	r2, r3
	ldr	r0, .L94+8
	bl	printk
.L44:
	ldr	r2, [r5, #16]
	ldr	r3, [r4, #84]
	add	r3, r3, r2
	str	r3, [r4, #84]
	ldr	r3, [r4, #88]
	ldr	r2, [r5, #4]
	add	r3, r3, r2
	str	r3, [r4, #88]
	ldr	r3, [r5, #12]
	str	r10, [r4, #80]
	lsls	r1, r3, #16
	ldrd	r2, [r4, #96]
	adds	fp, r2, r1
	adc	ip, r3, #0
	tst	r10, #2
	mov	r2, fp
	mov	r3, ip
	strd	r2, [r4, #96]
	beq	.L45
	movw	r2, #2472
	ldr	r1, .L94+4
	ldr	r0, .L94+12
	bl	printk
.L45:
	tst	r10, #1
	beq	.L46
	ldr	r3, .L94
	ldr	r3, [r3]
	lsls	r3, r3, #28
	bpl	.L46
	movw	r2, #2475
	ldr	r1, .L94+4
	ldr	r0, .L94+16
	bl	printk
.L46:
	tst	r10, #8
	beq	.L47
	movw	r2, #2478
	ldr	r1, .L94+4
	ldr	r0, .L94+20
	bl	printk
.L47:
	tst	r10, #4
	beq	.L48
	movw	r2, #2481
	ldr	r1, .L94+4
	ldr	r0, .L94+24
	bl	printk
.L48:
	tst	r10, #16
	beq	.L49
	ldr	r1, .L94+4
	movw	r2, #2484
	ldr	r0, .L94+28
.L93:
	bl	printk
	mov	r0, #-1
.L32:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L37:
	movs	r1, #11
	mul	r3, r1, r9
	movs	r1, #12
	mla	r1, r1, r7, r3
	add	r1, r1, r8, lsl #3
	b	.L89
.L39:
	add	ip, r0, r0, lsl #2
	cmp	r1, ip
	bhi	.L90
	cmp	r1, r0, asr #2
	ite	ls
	movls	r3, #0
	movhi	r3, #1
	b	.L90
.L63:
	movs	r3, #2
	b	.L43
.L49:
	tst	r10, #32
	beq	.L51
	movw	r2, #2489
	ldr	r1, .L94+4
	ldr	r0, .L94+32
	b	.L93
.L51:
	tst	r10, #64
	beq	.L52
	movw	r2, #2494
	ldr	r1, .L94+4
	ldr	r0, .L94+36
	b	.L93
.L52:
	tst	r10, #128
	beq	.L53
	movw	r2, #2499
	ldr	r1, .L94+4
	ldr	r0, .L94+40
	b	.L93
.L53:
	tst	r10, #256
	beq	.L54
	movw	r2, #2504
	ldr	r1, .L94+4
	ldr	r0, .L94+44
	b	.L93
.L54:
	ldr	r2, [sp]
	ldr	r3, [sp]
	ldr	r1, [sp, #8]
	ldrb	ip, [r2, #925]	@ zero_extendqisi2
	ldrb	r3, [r3, #924]	@ zero_extendqisi2
	ldrb	r2, [r2, #926]	@ zero_extendqisi2
	add	r0, r3, ip
	mul	lr, r1, r0
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #24]
	muls	r0, r2, r0
	mla	r0, r1, r3, r0
	add	r3, r2, ip
	ldr	r2, [sp, #12]
	add	r0, r0, lr, lsr #1
	muls	r3, r2, r3
	ldr	r2, [sp, #8]
	add	r1, r1, r2
	add	r0, r0, r3, lsr #1
	ldr	r3, [sp, #12]
	add	r1, r1, r3
	ldr	r3, [sp, #16]
	adds	r1, r1, r3
	bne	.L55
.L91:
	ldr	r2, [sp]
	ldr	r3, [sp]
	str	r0, [r4, #348]
	ldrh	r1, [r2, #930]
	ldrh	r3, [r3, #928]
	ubfx	r1, r1, #0, #12
	ldrh	r2, [r2, #932]
	ubfx	r3, r3, #0, #12
	adds	r0, r3, r1
	ubfx	r2, r2, #0, #12
	mul	ip, r8, r0
	ldr	r0, [sp, #28]
	mul	lr, r7, r2
	mla	r0, r0, r3, lr
	adds	r3, r2, r1
	mul	r3, r9, r3
	add	r0, r0, ip, lsr #1
	add	r0, r0, r3, lsr #1
	ldr	r3, [sp, #28]
	add	r8, r8, r3
	add	r9, r9, r8
	adds	r1, r9, r7
	bne	.L57
.L92:
	str	r0, [r4, #344]
	add	r1, r5, #388
	ldr	r3, [r5, #164]
	add	r0, r4, #136
	ubfx	r2, r3, #0, #23
	ldr	r3, [r4, #352]
	add	r3, r3, r2
	str	r3, [r4, #352]
	ldr	r3, [r5, #132]
	ubfx	r2, r3, #0, #17
	ldr	r3, [r4, #104]
	add	r3, r3, r2
	str	r3, [r4, #104]
	ldr	r3, [r5, #136]
	ubfx	r2, r3, #0, #19
	ldr	r3, [r4, #108]
	add	r3, r3, r2
	str	r3, [r4, #108]
	ldr	r3, [r5, #148]
	ubfx	r2, r3, #0, #19
	ldr	r3, [r4, #120]
	add	r3, r3, r2
	str	r3, [r4, #120]
	ldr	r3, [r5, #140]
	ubfx	r2, r3, #0, #21
	ldr	r3, [r4, #112]
	add	r3, r3, r2
	str	r3, [r4, #112]
	ldr	r3, [r5, #152]
	ubfx	r2, r3, #0, #21
	ldr	r3, [r4, #124]
	add	r3, r3, r2
	str	r3, [r4, #124]
	ldr	r3, [r5, #144]
	ubfx	r2, r3, #0, #23
	ldr	r3, [r4, #116]
	add	r3, r3, r2
	str	r3, [r4, #116]
	ldr	r3, [r5, #156]
	ubfx	r2, r3, #0, #23
	ldr	r3, [r4, #128]
	add	r3, r3, r2
	str	r3, [r4, #128]
	ldr	r3, [r5, #160]
	ubfx	r2, r3, #0, #23
	ldr	r3, [r4, #132]
	add	r3, r3, r2
	movs	r2, #208
	str	r3, [r4, #132]
	bl	memcpy
	ldrb	r1, [r5, #168]	@ zero_extendqisi2
	movs	r3, #4
	smulbb	r2, r1, r3
	str	r2, [r4, #360]
	ldrb	r0, [r5, #169]	@ zero_extendqisi2
	lsls	r0, r0, #2
	str	r0, [r4, #364]
	ldrb	r2, [r5, #170]	@ zero_extendqisi2
	smlabb	r1, r3, r1, r0
	smulbb	r7, r2, r3
	smlabb	r2, r3, r2, r1
	str	r7, [r4, #368]
	ldrb	r5, [r5, #171]	@ zero_extendqisi2
	smulbb	r7, r5, r3
	smlabb	r3, r3, r5, r2
	ldr	r2, [r4, #88]
	str	r7, [r4, #372]
	str	r3, [r4, #376]
	ldr	r3, [r6, #184]
	add	r3, r3, r2, lsl #3
	str	r3, [r6, #184]
	ldr	r1, [r4, #352]
	cbz	r1, .L59
	ldr	r0, [r4, #348]
	bl	__aeabi_uidiv
	mov	r1, r0
.L59:
	str	r1, [r4, #348]
	ldr	r1, [r4, #356]
	cbz	r1, .L60
	ldr	r0, [r4, #344]
	bl	__aeabi_uidiv
	mov	r1, r0
.L60:
	ldr	r3, [sp, #20]
	str	r1, [r4, #344]
	cmp	r3, #0
	ble	.L61
	ldrd	r3, r2, [r4, #120]
	ldr	r1, [sp, #20]
	lsls	r0, r3, #6
	add	r0, r0, r2, lsl #4
	ldrd	r2, r3, [r4, #128]
	add	r3, r3, r2
	add	r0, r0, r3, lsl #2
	lsls	r0, r0, #8
	bl	__aeabi_uidiv
	str	r0, [r6, #200]
.L61:
	ldr	r3, [sp, #4]
	cmp	r3, #0
	ble	.L62
	ldr	r0, [r4, #84]
	lsls	r1, r3, #6
	bl	__aeabi_uidiv
	str	r0, [r6, #188]
.L62:
	ldr	r3, [r4, #348]
	str	r3, [r6, #192]
	ldr	r3, [r4, #344]
	str	r3, [r6, #196]
	ldr	r3, .L94
	ldr	r0, [r3]
	ands	r0, r0, #4
	beq	.L32
	movw	r3, #2585
	ldr	r1, .L94+4
	mov	r2, r3
	ldr	r0, .L94+48
	bl	printk
	movs	r0, #0
	b	.L32
.L55:
	bl	__aeabi_uidiv
	b	.L91
.L95:
	.align	2
.L94:
	.word	hal_h265e_debug
	.word	.LANCHOR0+42
	.word	.LC0
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC2
.L57:
	bl	__aeabi_uidiv
	b	.L92
	.fnend
	.size	vepu540c_h265_set_feedback, .-vepu540c_h265_set_feedback
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_v540c_ret_comb_task, %function
hal_h265e_v540c_ret_comb_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r5, r0
	ldr	r4, .L115
	mov	r7, r1
	ldr	r10, [r0, #72]
	mov	r6, r2
	ldr	r9, [r2, #4]
	ldr	r3, [r4]
	lsls	r0, r3, #29
	bpl	.L97
	movw	r3, #2763
	ldr	r1, .L115+4
	mov	r2, r3
	ldr	r0, .L115+8
	bl	printk
.L97:
	mov	r1, r7
	mov	r0, r5
	bl	vepu540c_h265_set_feedback
	mov	r8, r0
	cmp	r0, #0
	bne	.L96
	ldr	r3, [r5, #88]
	str	r3, [r7, #44]
	ldr	r3, [r7, #48]
	ldr	r2, [r5, #88]
	add	r3, r3, r2
	str	r3, [r7, #48]
	ldr	r3, [r10]
	lsls	r1, r3, #18
	itt	mi
	movmi	r3, #1
	strmi	r3, [r6, #108]
	ldr	r2, [r10, #104]
	ldr	r3, [r6, #48]
	str	r2, [r6, #44]
	add	r3, r3, r2
	str	r3, [r6, #48]
	ldr	r3, [r9, #184]
	add	r3, r3, r2, lsl #3
	str	r3, [r9, #184]
	ldr	r3, [r4]
	lsls	r2, r3, #28
	bpl	.L101
	ldr	r3, [r5, #88]
	movw	r2, #2778
	ldr	r1, .L115+4
	ldr	r0, .L115+12
	bl	printk
.L101:
	ldr	r3, [r4]
	lsls	r3, r3, #29
	bpl	.L96
	movw	r3, #2780
	ldr	r1, .L115+4
	mov	r2, r3
	ldr	r0, .L115+16
	bl	printk
.L96:
	mov	r0, r8
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L116:
	.align	2
.L115:
	.word	hal_h265e_debug
	.word	.LANCHOR0+69
	.word	.LC0
	.word	.LC12
	.word	.LC2
	.fnend
	.size	hal_h265e_v540c_ret_comb_task, .-hal_h265e_v540c_ret_comb_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_v540c_wait, %function
hal_h265e_v540c_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r6, r0
	ldr	r5, .L131
	mov	r4, r1
	ldr	r2, [r5]
	lsls	r2, r2, #29
	bpl	.L118
	movw	r3, #2596
	ldr	r1, .L131+4
	mov	r2, r3
	ldr	r0, .L131+8
	bl	printk
.L118:
	ldr	r3, [r4, #92]
	cbz	r3, .L119
	movw	r2, #2600
	ldr	r1, .L131+4
	ldr	r0, .L131+12
	mov	r4, #-1
	bl	printk
.L117:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L119:
	mov	r2, r3
	movs	r1, #6
	ldr	r0, [r6, #64]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L121
	ldr	r3, [r6, #72]
	movw	r2, #2662
	ldr	r1, .L131+4
	str	r3, [sp]
	mov	r3, r0
	ldr	r0, .L131+16
	bl	printk
.L121:
	ldr	r3, [r5]
	lsls	r3, r3, #29
	bpl	.L117
	movw	r3, #2664
	ldr	r1, .L131+4
	mov	r2, r3
	ldr	r0, .L131+20
	bl	printk
	b	.L117
.L132:
	.align	2
.L131:
	.word	hal_h265e_debug
	.word	.LANCHOR0+99
	.word	.LC0
	.word	.LC13
	.word	.LC14
	.word	.LC2
	.fnend
	.size	hal_h265e_v540c_wait, .-hal_h265e_v540c_wait
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_v540c_start, %function
hal_h265e_v540c_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	ldr	r6, .L205
	.pad #36
	sub	sp, sp, #36
	mov	r7, r0
	ldrd	r5, r9, [r0, #68]
	ldr	r3, [r6]
	lsls	r1, r3, #29
	bpl	.L134
	movw	r3, #2239
	ldr	r1, .L205+4
	mov	r2, r3
	ldr	r0, .L205+8
	bl	printk
.L134:
	ldr	r8, [r4, #92]
	cmp	r8, #0
	beq	.L135
	mov	r3, r8
	movw	r2, #2243
	ldr	r1, .L205+4
	mov	r4, #-1
	ldr	r0, .L205+12
	bl	printk
.L133:
	mov	r0, r4
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L135:
	add	r2, sp, #8
	mov	r1, r8
	ldr	r0, [r7, #64]
	mov	r3, #292
	str	r5, [sp, #8]
	strd	r3, r8, [sp, #12]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L137
	mov	r3, r0
	movw	r2, #2253
.L203:
	ldr	r1, .L205+4
	ldr	r0, .L205+16
.L202:
	bl	printk
	b	.L133
.L137:
	ldr	r3, [r6]
	lsls	r2, r3, #26
	bpl	.L138
	ldr	r4, .L205+4
	ldr	r10, .L205+32
.L140:
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L139
	ldr	r3, [r5, r8, lsl #2]
	movw	r2, #2261
	mov	r1, r4
	mov	r0, r10
	str	r3, [sp]
	lsl	r3, r8, #2
	bl	printk
.L139:
	add	r8, r8, #1
	cmp	r8, #73
	bne	.L140
.L138:
	add	r3, r5, #292
	mov	r0, #532
	str	r3, [sp, #8]
	add	r2, sp, #8
	mov	r3, #624
	movs	r1, #0
	strd	r0, r3, [sp, #12]
	ldr	r0, [r7, #64]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L141
	mov	r3, r0
	movw	r2, #2271
	b	.L203
.L141:
	ldr	r3, [r6]
	lsls	r1, r3, #27
	bpl	.L142
	ldr	r10, .L205+4
	movs	r4, #32
	ldr	fp, .L205+36
	mvn	r8, #3
.L144:
	ldr	r3, [r6]
	lsls	r2, r3, #27
	bpl	.L143
	mul	r3, r8, r4
	add	r2, r5, #420
	mov	r1, r10
	mov	r0, fp
	ldr	r2, [r2, r3]
	adds	r3, r3, #128
	str	r2, [sp]
	movw	r2, #2279
	bl	printk
.L143:
	subs	r4, r4, #1
	bne	.L144
	ldr	r10, .L205+4
	movs	r4, #101
	ldr	fp, .L205+40
	mvn	r8, #3
.L146:
	ldr	r3, [r6]
	lsls	r3, r3, #27
	bpl	.L145
	mul	r3, r8, r4
	add	r2, r5, #824
	mov	r1, r10
	mov	r0, fp
	ldr	r2, [r2, r3]
	add	r3, r3, #404
	str	r2, [sp]
	movw	r2, #2284
	bl	printk
.L145:
	subs	r4, r4, #1
	bne	.L146
.L142:
	add	r2, sp, #8
	movs	r1, #0
	ldr	r0, [r7, #64]
	add	r3, r5, #824
	mov	ip, #272
	str	r3, [sp, #8]
	mov	r3, #4096
	strd	ip, r3, [sp, #12]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L147
	mov	r3, r0
	movw	r2, #2293
	b	.L203
.L147:
	ldr	r3, [r6]
	lsls	r4, r3, #25
	bpl	.L148
	ldr	r10, .L205+4
	movs	r4, #68
	ldr	fp, .L205+40
	mvn	r8, #3
.L150:
	ldr	r3, [r6]
	lsls	r0, r3, #25
	bpl	.L149
	mul	r3, r8, r4
	add	r2, r5, #1096
	mov	r1, r10
	mov	r0, fp
	ldr	r2, [r2, r3]
	add	r3, r3, #272
	str	r2, [sp]
	movw	r2, #2301
	bl	printk
.L149:
	subs	r4, r4, #1
	bne	.L150
.L148:
	add	r3, r5, #1096
	mov	r1, #720
	str	r3, [sp, #8]
	add	r2, sp, #8
	mov	r3, #5888
	ldr	r0, [r7, #64]
	strd	r1, r3, [sp, #12]
	movs	r1, #0
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L151
	mov	r3, r0
	movw	r2, #2311
	b	.L203
.L151:
	ldr	r3, [r6]
	lsls	r2, r3, #24
	bpl	.L152
	ldr	r10, .L205+4
	movs	r4, #180
	ldr	fp, .L205+40
	mvn	r8, #3
.L154:
	ldr	r3, [r6]
	lsls	r3, r3, #24
	bpl	.L153
	mul	r3, r8, r4
	add	r2, r5, #1816
	mov	r1, r10
	mov	r0, fp
	ldr	r2, [r2, r3]
	add	r3, r3, #720
	str	r2, [sp]
	movw	r2, #2319
	bl	printk
.L153:
	subs	r4, r4, #1
	bne	.L154
.L152:
	add	r2, sp, #8
	movs	r1, #0
	ldr	r0, [r7, #64]
	add	r3, r5, #1816
	mov	lr, #256
	str	r3, [sp, #8]
	mov	r3, #8192
	strd	lr, r3, [sp, #12]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L155
	mov	r3, r0
	movw	r2, #2329
	b	.L203
.L155:
	mov	r1, r0
	add	r3, r5, #5152
	add	r2, sp, #8
	ldr	r0, [r7, #64]
	adds	r3, r3, #24
	mov	ip, #624
	str	r3, [sp, #8]
	mov	r3, #12288
	strd	ip, r3, [sp, #12]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L156
	mov	r3, r0
	movw	r2, #2339
	b	.L203
.L156:
	mov	r0, #3104
	movw	r3, #8672
	mov	r1, r4
	strd	r0, r3, [sp, #12]
	add	r2, sp, #8
	ldr	r0, [r7, #64]
	addw	r5, r5, #2072
	str	r5, [sp, #8]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L157
	mov	r3, r0
	movw	r2, #2349
	b	.L203
.L157:
	movs	r1, #4
	movs	r3, #44
	add	r2, sp, #20
	strd	r1, r3, [sp, #24]
	ldr	r0, [r7, #64]
	movs	r1, #1
	str	r9, [sp, #20]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L158
	mov	r3, r0
	movw	r2, #2359
.L204:
	ldr	r1, .L205+4
	ldr	r0, .L205+20
	b	.L202
.L158:
	add	r3, r9, #4
	mov	r2, #592
	str	r3, [sp, #20]
	movs	r1, #1
	mov	r3, #16384
	ldr	r0, [r7, #64]
	strd	r2, r3, [sp, #24]
	add	r2, sp, #20
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L159
	mov	r3, r0
	movw	r2, #2369
	b	.L204
.L159:
	mov	r2, r0
	movs	r1, #5
	ldr	r0, [r7, #64]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L160
	mov	r3, r0
	movw	r2, #2375
	ldr	r1, .L205+4
	ldr	r0, .L205+24
	bl	printk
.L160:
	ldr	r3, [r6]
	lsls	r3, r3, #29
	bpl	.L133
	movw	r3, #2376
	ldr	r1, .L205+4
	mov	r2, r3
	ldr	r0, .L205+28
	b	.L202
.L206:
	.align	2
.L205:
	.word	hal_h265e_debug
	.word	.LANCHOR0+120
	.word	.LC0
	.word	.LC15
	.word	.LC16
	.word	.LC20
	.word	.LC21
	.word	.LC2
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.fnend
	.size	hal_h265e_v540c_start, .-hal_h265e_v540c_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_v540c_comb_start, %function
hal_h265e_v540c_comb_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r8, r2
	ldr	r6, .L224
	.pad #28
	sub	sp, sp, #28
	mov	r4, r0
	mov	r9, r1
	ldr	r5, [r0, #68]
	ldr	r3, [r6]
	lsls	r2, r3, #29
	bpl	.L208
	movw	r3, #2733
	ldr	r1, .L224+4
	mov	r2, r3
	ldr	r0, .L224+8
	bl	printk
.L208:
	ldrb	r3, [r5, #48]	@ zero_extendqisi2
	movs	r2, #7
	add	r7, r5, #4768
	mov	r0, sp
	bfi	r3, r2, #0, #4
	strb	r3, [r5, #48]
	ldr	r3, [r4, #64]
	adds	r7, r7, #24
	strd	r7, r8, [sp, #8]
	str	r3, [sp]
	add	r3, r5, #692
	str	r3, [sp, #4]
	ldr	r3, [r4, #484]
	str	r3, [sp, #16]
	ldr	r3, [r4, #516]
	str	r3, [sp, #20]
	bl	vepu540c_set_jpeg_reg
	ldr	r1, [r8, #104]
	cbz	r1, .L209
	mov	r2, #384
	mov	r0, r7
	bl	memcpy
.L209:
	ldr	r1, [r8, #100]
	cbz	r1, .L210
	add	r0, r5, #5472
	mov	r2, #312
	adds	r0, r0, #16
	bl	memcpy
.L210:
	ldr	r3, [r6]
	lsls	r3, r3, #29
	bpl	.L211
	movw	r3, #2748
	ldr	r1, .L224+4
	mov	r2, r3
	ldr	r0, .L224+12
	bl	printk
.L211:
	mov	r1, r9
	mov	r0, r4
	bl	hal_h265e_v540c_start
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L225:
	.align	2
.L224:
	.word	hal_h265e_debug
	.word	.LANCHOR0+142
	.word	.LC0
	.word	.LC2
	.fnend
	.size	hal_h265e_v540c_comb_start, .-hal_h265e_v540c_comb_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_v540c_get_task, %function
hal_h265e_v540c_get_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #52
	sub	sp, sp, #52
	ldr	r3, [r1, #4]
	mov	r4, r0
	ldr	r7, .L314
	mov	r9, r1
	str	r3, [sp, #16]
	ldr	r3, [r7]
	lsls	r5, r3, #29
	bpl	.L227
	movw	r3, #2674
	ldr	r1, .L314+4
	mov	r2, r3
	ldr	r0, .L314+8
	bl	printk
.L227:
	ldr	r6, [r4, #444]
	movw	r3, #4700
	ldr	r5, [r4, #484]
	ldr	fp, [r4, #476]
	ldr	r8, [r6, r3]
	ldr	r3, [r6, #28]
	add	r10, r3, #63
	ldr	r3, [r7]
	bic	r10, r10, #63
	lsls	r0, r3, #29
	bpl	.L228
	mov	r3, #450
	ldr	r1, .L314+12
	mov	r2, r3
	ldr	r0, .L314+8
	bl	printk
.L228:
	ldr	r3, [r6, #28]
	mov	r0, r5
	adds	r7, r3, #63
	it	mi
	addmi	r7, r3, #126
	adds	r3, r3, #15
	bic	r3, r3, #15
	asrs	r2, r7, #6
	str	r2, [sp, #20]
	ldr	r2, [r6, #32]
	adds	r7, r2, #63
	it	mi
	addmi	r7, r2, #126
	adds	r2, r2, #15
	bic	r2, r2, #15
	asrs	r7, r7, #6
	muls	r2, r3, r2
	ldr	r3, [r4, #444]
	ldr	r1, [r3, #36]
	str	r2, [sp, #28]
	bl	vepu541_set_fmt
	ldr	r3, [r5]
	ldr	r2, [sp, #28]
	cmp	r3, #13
	bhi	.L231
	tbb	[pc, r3]
.L233:
	.byte	(.L236-.L233)/2
	.byte	(.L232-.L233)/2
	.byte	(.L234-.L233)/2
	.byte	(.L231-.L233)/2
	.byte	(.L234-.L233)/2
	.byte	(.L234-.L233)/2
	.byte	(.L235-.L233)/2
	.byte	(.L235-.L233)/2
	.byte	(.L234-.L233)/2
	.byte	(.L234-.L233)/2
	.byte	(.L231-.L233)/2
	.byte	(.L231-.L233)/2
	.byte	(.L232-.L233)/2
	.byte	(.L232-.L233)/2
	.p2align 1
.L235:
	add	r5, r2, r2, lsl #1
	lsrs	r5, r5, #1
.L237:
	cmp	r8, #0
	beq	.L272
	mov	r0, r8
	bl	mpp_enc_ref_cfg_get_cpb_info
	ldr	r3, [r0]
	add	r8, r3, #1
	ldr	r3, [r0, #4]
	cmp	r8, #2
	it	lt
	movlt	r8, #2
	str	r3, [sp, #24]
.L239:
	ldrd	r3, r1, [r6, #28]
	addw	r6, r3, #511
	add	r0, r1, #31
	addw	r1, r1, #511
	adds	r3, r3, #31
	asrs	r2, r6, #9
	bic	r3, r3, #31
	bic	r6, r0, #31
	asrs	r1, r1, #9
	muls	r6, r2, r6
	it	mi
	addmi	r6, r6, #31
	muls	r3, r1, r3
	it	mi
	addmi	r3, r3, #31
	asrs	r6, r6, #5
	asrs	r3, r3, #5
	lsls	r2, r6, #4
	lsls	r6, r3, #4
	cmp	r6, r2
	it	lt
	movlt	r6, r2
	cmp	r10, #3072
	ble	.L242
	asr	r10, r10, #5
	mov	r3, #416
	mul	r10, r3, r10
	ldr	r3, [r4, #636]
	ldr	r3, [r3, #8]
	sub	r10, r10, #37632
	sub	r10, r10, #224
	cmp	r3, #0
	bne	.L243
	ldrd	r2, r0, [r4, #644]
	str	r3, [sp, #28]
	cmp	r10, r2
	beq	.L244
	ldr	r1, .L314+16
	bl	mpp_buffer_put_with_caller
	ldr	r3, [sp, #28]
	str	r3, [r4, #648]
.L245:
	ldr	r3, .L314+16
	mov	r2, r10
	add	r1, r4, #648
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L314+20
	bl	mpp_buffer_get_with_tag
	b	.L246
.L234:
	lsls	r5, r2, #1
	b	.L237
.L232:
	add	r5, r2, r2, lsl #1
	b	.L237
.L236:
	lsls	r5, r2, #2
	b	.L237
.L231:
	mov	r2, #486
	ldr	r1, .L314+12
	ldr	r0, .L314+24
	bl	printk
	movw	r2, #2678
	ldr	r1, .L314+4
	ldr	r0, .L314+28
	bl	printk
	ldr	r3, [r9, #92]
	mvn	r0, #3
	orr	r3, r3, #16
	str	r3, [r9, #92]
.L226:
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L272:
	mov	r8, #2
	b	.L239
.L315:
	.align	2
.L314:
	.word	hal_h265e_debug
	.word	.LANCHOR0+169
	.word	.LC0
	.word	.LANCHOR0+194
	.word	.LANCHOR0+223
	.word	.LC24
	.word	.LC22
	.word	.LC23
.L244:
	cmp	r0, #0
	beq	.L245
.L246:
	str	r10, [r4, #644]
.L247:
	ldr	r3, [r4, #468]
	ldr	r10, .L316+32
	cmp	r3, r5
	bcc	.L249
	cmp	fp, r8
	blt	.L249
	ldr	r3, [r4, #472]
	cmp	r3, r6
	bcs	.L250
.L249:
	ldr	r3, [r4, #636]
	ldr	r3, [r3]
	cbnz	r3, .L251
	ldr	r0, [r4, #504]
	cbz	r0, .L252
	bl	hal_bufs_deinit
.L252:
	add	r0, r4, #504
	bl	hal_bufs_init
.L251:
	ldr	r3, [sp, #20]
	cmp	r8, fp
	ldr	r2, [r4, #444]
	it	lt
	movlt	r8, fp
	muls	r7, r3, r7
	ldr	r3, [r4, #520]
	lsls	r1, r7, #8
	cmp	r3, #0
	beq	.L253
	movs	r3, #0
	str	r1, [sp, #32]
	strd	r3, r3, [sp, #36]
	str	r3, [sp, #44]
	ldr	r3, [r2, #596]
	cbz	r3, .L254
	lsls	r7, r7, #2
	adds	r7, r7, #255
	bic	r7, r7, #255
	lsls	r7, r7, #4
	str	r7, [sp, #36]
.L254:
	ldr	r3, [r10]
	str	r6, [sp, #40]
	lsls	r1, r3, #28
	bpl	.L255
	strd	fp, r8, [sp, #4]
	movw	r2, #555
	str	r5, [sp]
	ldr	r3, [r4, #468]
	ldr	r1, .L316
	ldr	r0, .L316+4
	bl	printk
.L255:
	ldr	r3, [r4, #444]
	movs	r0, #48
	movs	r2, #240
	ldr	r7, .L316+8
	ldr	r1, [r3, #28]
	ldr	r3, [r3, #32]
	adds	r1, r1, #63
	adds	r3, r3, #63
	bic	r1, r1, #63
	bic	r3, r3, #63
	cmp	r1, r3
	mul	ip, r3, r1
	it	lt
	movlt	r1, r3
	muls	r0, r1, r0
	muls	r2, r1, r2
	add	r3, ip, ip, lsl #1
	cmp	ip, #0
	it	lt
	addlt	ip, ip, #63
	addw	r0, r0, #4095
	add	r0, r0, r3, asr #1
	asrs	r3, r1, #1
	add	ip, r3, ip, asr #6
	add	r3, r1, r1, lsl #1
	ldr	r1, [sp, #24]
	addw	r2, r2, #4095
	addw	ip, ip, #4095
	addw	r3, r3, #4095
	ands	r0, r0, r7
	and	ip, ip, r7
	ands	r2, r2, r7
	ands	r3, r3, r7
	cmp	r1, #0
	add	r2, r2, r0
	add	r3, r3, ip
	str	r0, [r4, #628]
	str	r2, [r4, #632]
	add	r7, r2, r3
	str	ip, [r4, #580]
	str	r3, [r4, #584]
	ble	.L257
	ldr	r1, [r4, #652]
	strd	r0, r2, [r4, #604]
	strd	ip, r3, [r4, #556]
	cbz	r1, .L258
	str	r0, [r4, #608]
	str	ip, [r4, #560]
.L258:
	ldr	r0, [r4, #560]
	ldr	r1, [r4, #608]
	add	r3, r3, r0
	str	r3, [r4, #588]
	add	ip, r1, r0
	add	r1, r1, r3
	add	r7, r7, ip
	add	r2, r2, r1
	mov	ip, #0
	str	r0, [r4, #564]
	strd	ip, r0, [r4, #540]
	str	ip, [r4, #548]
	strd	r3, r0, [r4, #568]
	strd	r1, r3, [r4, #592]
	str	r1, [r4, #612]
	strd	r2, r1, [r4, #616]
.L271:
	ldr	r3, [r4, #636]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne	.L259
	ldr	r0, [r4, #536]
	cbz	r0, .L260
	ldr	r1, .L316+12
	bl	mpp_buffer_put_with_caller
.L260:
	ldr	r3, .L316+12
	mov	r2, r7
	add	r1, r4, #536
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L316+16
	bl	mpp_buffer_get_with_tag
.L265:
	ldr	r3, [r4, #636]
	ldr	r3, [r3]
	cbnz	r3, .L263
	add	r3, sp, #32
	movs	r2, #4
	mov	r1, r8
	ldr	r0, [r4, #504]
	bl	hal_bufs_setup
.L263:
	ldr	r3, [r4, #636]
	ldr	r3, [r3]
	cbz	r3, .L267
	str	r3, [r4, #504]
.L267:
	str	r5, [r4, #468]
	strd	r6, r8, [r4, #472]
.L250:
	ldr	r3, [r10]
	lsls	r3, r3, #29
	bpl	.L268
	mov	r3, #588
	ldr	r1, .L316
	mov	r2, r3
	ldr	r0, .L316+20
	bl	printk
.L268:
	ldr	r3, [r4, #392]
	str	r3, [r4, #396]
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	tst	r3, #16
	ite	ne
	movne	r3, #2
	moveq	r3, #0
	str	r3, [r4, #392]
	ldr	r0, [r9, #52]
	bl	mpp_frame_get_roi
	str	r0, [r4, #440]
	ldr	r0, [r9, #52]
	bl	mpp_frame_get_osd
	ldr	r3, [r4, #352]
	add	r2, r4, #404
	add	r1, r4, #380
	str	r3, [r4, #400]
	add	r3, r4, #360
	str	r0, [r4, #436]
.L270:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L270
	mov	r2, #304
	movs	r1, #0
	add	r0, r4, #80
	bl	memset
	ldr	r0, [r10]
	ands	r0, r0, #4
	beq	.L226
	movw	r3, #2700
	ldr	r1, .L316+24
	mov	r2, r3
	ldr	r0, .L316+20
	bl	printk
	movs	r0, #0
	b	.L226
.L243:
	str	r3, [r4, #648]
	b	.L246
.L242:
	ldr	r0, [r4, #648]
	cbz	r0, .L248
	ldr	r3, [r4, #636]
	ldr	r10, [r3, #8]
	cmp	r10, #0
	bne	.L248
	ldr	r1, .L316+28
	bl	mpp_buffer_put_with_caller
	str	r10, [r4, #648]
.L248:
	movs	r3, #0
	str	r3, [r4, #644]
	b	.L247
.L259:
	str	r3, [r4, #536]
	b	.L265
.L253:
	strd	r3, r3, [sp, #36]
	str	r3, [sp, #44]
	movw	r3, #8191
	add	r3, r3, r7, lsl #6
	str	r1, [sp, #32]
	bic	r3, r3, #8160
	bic	r3, r3, #31
	str	r3, [r4, #508]
	ldr	r2, [r2, #596]
	cbz	r2, .L264
	lsls	r2, r7, #2
	adds	r2, r2, #255
	bic	r2, r2, #255
	lsls	r2, r2, #4
	str	r2, [sp, #36]
.L264:
	lsls	r2, r7, #13
	str	r6, [sp, #40]
	add	r7, r2, r7, lsl #12
	add	r3, r3, r7, asr #1
	str	r3, [sp, #44]
	ldr	r3, [r10]
	lsls	r2, r3, #28
	bpl	.L265
	strd	fp, r8, [sp, #4]
	movw	r2, #575
	str	r5, [sp]
	ldr	r3, [r4, #468]
	ldr	r1, .L316
	ldr	r0, .L316+4
	bl	printk
	b	.L265
.L257:
	movs	r2, #0
	strd	r3, r7, [r4, #612]
	strd	r2, r3, [r4, #564]
	str	r2, [r4, #572]
	str	r3, [r4, #620]
	b	.L271
.L317:
	.align	2
.L316:
	.word	.LANCHOR0+194
	.word	.LC25
	.word	-4096
	.word	.LANCHOR0+252
	.word	.LC24
	.word	.LC2
	.word	.LANCHOR0+169
	.word	.LANCHOR0+223
	.word	hal_h265e_debug
	.fnend
	.size	hal_h265e_v540c_get_task, .-hal_h265e_v540c_get_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_vepu540c_prepare, %function
hal_h265e_vepu540c_prepare:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, .L331
	ldr	r6, [r0, #444]
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L319
	mov	r3, r0
	movw	r2, #1196
	ldr	r1, .L331+4
	ldr	r0, .L331+8
	bl	printk
.L319:
	ldr	r3, [r6, #8]
	tst	r3, #5
	itt	ne
	movne	r3, #0
	strne	r3, [r6, #8]
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L321
	mov	r3, r5
	movw	r2, #1210
	ldr	r1, .L331+4
	ldr	r0, .L331+12
	bl	printk
.L321:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L332:
	.align	2
.L331:
	.word	hal_h265e_debug
	.word	.LANCHOR0+265
	.word	.LC26
	.word	.LC27
	.fnend
	.size	hal_h265e_vepu540c_prepare, .-hal_h265e_vepu540c_prepare
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_v540c_ret_task, %function
hal_h265e_v540c_ret_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r1
	ldr	r4, .L346
	mov	r5, r0
	ldr	r3, [r4]
	lsls	r1, r3, #29
	bpl	.L334
	movw	r3, #2711
	ldr	r1, .L346+4
	mov	r2, r3
	ldr	r0, .L346+8
	bl	printk
.L334:
	mov	r1, r6
	mov	r0, r5
	bl	vepu540c_h265_set_feedback
	mov	r7, r0
	cbnz	r0, .L333
	ldr	r3, [r5, #88]
	str	r3, [r6, #44]
	ldr	r3, [r6, #48]
	ldr	r2, [r5, #88]
	add	r3, r3, r2
	str	r3, [r6, #48]
	ldr	r3, [r4]
	lsls	r2, r3, #28
	bpl	.L336
	ldr	r3, [r5, #88]
	mov	r2, #2720
	ldr	r1, .L346+4
	ldr	r0, .L346+12
	bl	printk
.L336:
	ldr	r3, [r4]
	lsls	r3, r3, #29
	bpl	.L333
	movw	r3, #2722
	ldr	r1, .L346+4
	mov	r2, r3
	ldr	r0, .L346+16
	bl	printk
.L333:
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, pc}
.L347:
	.align	2
.L346:
	.word	hal_h265e_debug
	.word	.LANCHOR0+292
	.word	.LC0
	.word	.LC12
	.word	.LC2
	.fnend
	.size	hal_h265e_v540c_ret_task, .-hal_h265e_v540c_ret_task
	.align	1
	.global	hal_h265e_v540c_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_v540c_deinit, %function
hal_h265e_v540c_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L401
	ldr	r3, [r5]
	lsls	r2, r3, #29
	bpl	.L349
	movw	r3, #1147
	ldr	r1, .L401+4
	mov	r2, r3
	ldr	r0, .L401+8
	bl	printk
.L349:
	ldr	r1, [r4, #68]
	cbz	r1, .L350
	ldr	r0, .L401+12
	bl	mpp_osal_free
.L350:
	ldr	r1, [r4, #72]
	movs	r6, #0
	str	r6, [r4, #68]
	cbz	r1, .L351
	ldr	r0, .L401+12
	bl	mpp_osal_free
.L351:
	ldr	r1, [r4, #76]
	str	r6, [r4, #72]
	cbz	r1, .L352
	ldr	r0, .L401+12
	bl	mpp_osal_free
.L352:
	ldr	r1, [r4, #484]
	movs	r6, #0
	str	r6, [r4, #76]
	cbz	r1, .L353
	ldr	r0, .L401+12
	bl	mpp_osal_free
.L353:
	ldr	r3, [r4, #636]
	str	r6, [r4, #484]
	ldr	r6, [r3, #8]
	cbnz	r6, .L354
	ldr	r0, [r4, #648]
	cbz	r0, .L354
	ldr	r1, .L401+12
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #648]
.L354:
	ldr	r3, [r4, #636]
	ldr	r3, [r3]
	cbnz	r3, .L355
	ldr	r0, [r4, #504]
	cbz	r0, .L355
	bl	hal_bufs_deinit
.L355:
	ldr	r0, [r4, #452]
	cbz	r0, .L356
	ldr	r1, .L401+12
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #452]
.L356:
	ldr	r0, [r4, #456]
	cbz	r0, .L357
	ldr	r1, .L401+12
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #456]
.L357:
	ldr	r3, [r4, #636]
	ldr	r6, [r3, #4]
	cbnz	r6, .L358
	ldr	r0, [r4, #536]
	cbz	r0, .L358
	ldr	r1, .L401+12
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #536]
.L358:
	ldr	r0, [r4, #460]
	cbz	r0, .L359
	ldr	r1, .L401+12
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #460]
.L359:
	ldr	r0, [r4, #64]
	cbz	r0, .L360
	bl	mpp_dev_deinit
	movs	r3, #0
	str	r3, [r4, #64]
.L360:
	ldr	r3, [r5]
	lsls	r3, r3, #29
	bpl	.L361
	movw	r3, #1187
	ldr	r1, .L401+4
	mov	r2, r3
	ldr	r0, .L401+16
	bl	printk
.L361:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L402:
	.align	2
.L401:
	.word	hal_h265e_debug
	.word	.LANCHOR0+317
	.word	.LC0
	.word	.LANCHOR0+340
	.word	.LC2
	.fnend
	.size	hal_h265e_v540c_deinit, .-hal_h265e_v540c_deinit
	.global	__aeabi_uidivmod
	.align	1
	.global	vepu540c_h265_set_hw_address
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu540c_h265_set_hw_address, %function
vepu540c_h265_set_hw_address:
	.fnstart
	@ args = 0, pretend = 0, frame = 80
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #100
	sub	sp, sp, #100
	ldr	r3, [r2, #64]
	mov	r4, r0
	ldr	r7, [r0, #484]
	mov	r6, r2
	ldr	r0, [r2, #28]
	mov	r5, r1
	str	r3, [sp, #52]
	ldr	r3, [r2, #16]
	str	r3, [sp, #20]
	bl	mpp_packet_get_length
	str	r0, [sp, #40]
	ldr	r0, [r6, #52]
	bl	mpp_frame_get_is_full
	mov	r8, r0
	ldr	r0, [r6, #52]
	bl	mpp_frame_get_ppinfo
	ldr	r3, .L491
	ldr	r2, [r3]
	tst	r2, #4
	str	r3, [sp, #64]
	beq	.L404
	movw	r3, #1761
	ldr	r1, .L491+4
	mov	r2, r3
	ldr	r0, .L491+8
	bl	printk
.L404:
	ldr	r3, [r4, #516]
	orrs	r3, r8, r3
	bne	.L405
	movs	r2, #160
	ldr	r1, [r6, #56]
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	ldr	r3, [sp, #20]
	str	r0, [r5, #24]
	strd	r0, r0, [r5, #16]
	ldr	r8, [r7]
	ldrh	r7, [r3, #10]
	ldrh	r9, [r3, #8]
	cbnz	r7, .L407
	ldrh	r7, [r3, #6]
.L407:
	ldr	r0, [r6, #52]
	bl	mpp_frame_get_fmt
	ands	r0, r0, #15728640
	beq	.L408
	ldr	r0, [r6, #52]
	bl	mpp_frame_get_fbc_offset
	mov	r7, r0
	movs	r0, #0
.L409:
	ldr	r3, [r5, #20]
	add	r7, r7, r3
	ldr	r3, [r5, #24]
	str	r7, [r5, #20]
	add	r0, r0, r3
	str	r0, [r5, #24]
.L405:
	ldr	r3, [sp, #20]
	ldr	r0, [r4, #504]
	ldrb	r1, [r3, #232]	@ zero_extendqisi2
	bl	hal_bufs_get_buf
	ldr	r3, [sp, #20]
	str	r0, [sp, #56]
	ldr	r0, [r4, #504]
	ldrb	r1, [r3, #234]	@ zero_extendqisi2
	bl	hal_bufs_get_buf
	ldr	r3, [r4, #520]
	mov	r8, r0
	cmp	r3, #0
	beq	.L416
	ldr	r3, [r6, #16]
	ldr	r1, [r4, #536]
	ldr	r0, [r4, #64]
	ldr	r2, [r3, #292]
	ldrb	r10, [r3, #233]	@ zero_extendqisi2
	ldrb	r9, [r3, #235]	@ zero_extendqisi2
	str	r2, [sp, #60]
	movs	r2, #163
	str	r3, [sp, #68]
	bl	mpp_dev_get_iova_address
	ldr	r2, [r4, #392]
	cmp	r2, #2
	mov	r7, r0
	ldr	r3, [sp, #68]
	bne	.L417
	ldrb	r2, [r3, #232]	@ zero_extendqisi2
	ldrb	r3, [r3, #234]	@ zero_extendqisi2
	cmp	r2, r3
	bne	.L417
	ldr	r3, .L491
	ldr	r3, [r3]
	lsls	r3, r3, #23
	bpl	.L418
	mov	r3, r10
	movw	r2, #305
	ldr	r1, .L491+12
	ldr	r0, .L491+16
	bl	printk
.L418:
	ldr	r3, [r4, #564]
	str	r3, [sp, #28]
	ldr	r3, [r4, #568]
	str	r3, [sp, #24]
	ldr	r3, [r4, #612]
	str	r3, [sp, #36]
	ldr	r3, [r4, #616]
	str	r3, [sp, #32]
	cmp	r10, #0
	beq	.L419
	ldr	r3, [r4, #548]
	ldr	r2, [sp, #28]
	ldr	fp, [r4, #596]
	str	r3, [sp, #16]
	ldr	r3, [r4, #540]
	str	fp, [sp, #48]
	cmp	r2, r3
	it	cs
	movcs	r2, r3
	ldr	r3, [r4, #544]
	str	r2, [sp, #28]
	ldr	r2, [sp, #24]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [r4, #588]
	str	r2, [sp, #24]
	ldr	r2, [sp, #36]
	cmp	r2, r3
	it	cs
	movcs	r2, r3
	ldr	r3, [r4, #592]
	str	r2, [sp, #36]
	ldr	r2, [sp, #32]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [sp, #16]
	str	r2, [sp, #32]
.L488:
	str	r3, [sp, #44]
.L420:
	ldr	r3, [sp, #16]
	ldrb	r2, [r5, #144]	@ zero_extendqisi2
	add	r3, r3, r7
	str	r3, [r5, #28]
	add	r3, r7, fp
	str	r3, [r5, #32]
	ldr	r3, [sp, #44]
	add	r3, r3, r7
	str	r3, [r5, #36]
	ldr	r3, [sp, #48]
	add	r3, r3, r7
	str	r3, [r5, #40]
	ldr	r3, [sp, #24]
	add	r3, r3, r7
	str	r3, [r5, #96]
	ldr	r3, [sp, #28]
	add	r3, r3, r7
	str	r3, [r5, #100]
	ldr	r3, [sp, #32]
	add	r3, r3, r7
	str	r3, [r5, #104]
	ldr	r3, [sp, #36]
	add	r7, r7, r3
	ldr	r3, [sp, #60]
	str	r7, [r5, #108]
	clz	r3, r3
	ldr	r7, .L491
	lsrs	r3, r3, #5
	bfi	r2, r3, #2, #1
	strb	r2, [r5, #144]
	ldr	r2, [r7]
	lsls	r1, r2, #23
	bpl	.L452
	mov	r2, #414
	ldr	r1, .L491+12
	ldr	r0, .L491+20
	bl	printk
	ldr	r3, [r7]
	lsls	r2, r3, #23
	bpl	.L452
	ldr	r3, [r4, #576]
	mov	r2, #416
	ldr	r1, .L491+12
	ldr	r0, .L491+24
	str	r3, [sp, #12]
	ldr	r3, [r4, #572]
	str	r3, [sp, #8]
	ldr	r3, [r4, #564]
	str	r3, [sp, #4]
	ldr	r3, [r4, #568]
	str	r3, [sp]
	ldr	r3, [r4, #580]
	bl	printk
	ldr	r3, [r7]
	lsls	r3, r3, #23
	bpl	.L452
	ldr	r3, [r4, #624]
	mov	r2, #418
	ldr	r1, .L491+12
	ldr	r0, .L491+28
	str	r3, [sp, #12]
	ldr	r3, [r4, #620]
	str	r3, [sp, #8]
	ldr	r3, [r4, #612]
	str	r3, [sp, #4]
	ldr	r3, [r4, #616]
	str	r3, [sp]
	ldr	r3, [r4, #628]
	bl	printk
.L452:
	ldr	r3, [sp, #60]
	cbnz	r3, .L436
	ldr	r3, [r4, #572]
	ldr	r2, [r4, #580]
	str	r3, [r4, #576]
	add	r3, r3, r2
	ldr	r2, [r4, #568]
	cmp	r3, r2
	ittt	cs
	subcs	r3, r3, r2
	ldrcs	r2, [r4, #564]
	addcs	r3, r3, r2
	str	r3, [r4, #572]
	ldr	r3, [r4, #620]
	ldr	r2, [r4, #628]
	str	r3, [r4, #624]
	add	r3, r3, r2
	ldr	r2, [r4, #616]
	cmp	r3, r2
	ittt	cs
	subcs	r3, r3, r2
	ldrcs	r2, [r4, #612]
	addcs	r3, r3, r2
	str	r3, [r4, #620]
.L436:
	ldr	r3, [sp, #56]
	movs	r2, #185
	ldr	r0, [r4, #64]
	ldr	r3, [r3, #4]
	ldr	r1, [r3, #8]
	bl	mpp_dev_get_iova_address
	movs	r2, #184
	str	r0, [r5, #116]
	ldr	r3, [r8, #4]
	ldr	r0, [r4, #64]
	ldr	r1, [r3, #8]
	bl	mpp_dev_get_iova_address
	str	r0, [r5, #112]
	ldr	r3, [r4, #444]
	ldr	r3, [r3, #596]
	cbz	r3, .L440
	ldr	r3, [sp, #56]
	movs	r2, #167
	ldr	r0, [r4, #64]
	ldr	r3, [r3, #4]
	ldr	r1, [r3, #4]
	bl	mpp_dev_get_iova_address
	movs	r2, #168
	str	r0, [r5, #44]
	ldr	r3, [r8, #4]
	ldr	r0, [r4, #64]
	ldr	r1, [r3, #4]
	bl	mpp_dev_get_iova_address
	str	r0, [r5, #48]
.L440:
	ldr	r3, [sp, #56]
	movs	r2, #169
	ldr	r0, [r4, #64]
	ldr	r3, [r3, #4]
	ldr	r1, [r3]
	bl	mpp_dev_get_iova_address
	movs	r2, #170
	str	r0, [r5, #52]
	ldr	r3, [r8, #4]
	ldr	r0, [r4, #64]
	ldr	r1, [r3]
	bl	mpp_dev_get_iova_address
	ldr	r3, [sp, #20]
	str	r0, [r5, #56]
	ldrsb	r3, [r3, #40]
	cmp	r3, #0
	bge	.L441
	ldr	r0, [r4, #452]
	cbnz	r0, .L442
	ldr	r3, .L491+32
	mov	r2, #131072
	add	r1, r4, #452
	str	r3, [sp]
	ldr	r3, .L491+36
	bl	mpp_buffer_get_with_tag
	b	.L492
.L493:
	.align	2
.L491:
	.word	hal_h265e_debug
	.word	.LANCHOR0+363
	.word	.LC0
	.word	.LANCHOR0+417
	.word	.LC29
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LANCHOR0+456
	.word	.LC24
.L492:
.L442:
	ldr	r0, [r4, #456]
	cbnz	r0, .L443
	ldr	r3, .L494
	mov	r2, #131072
	add	r1, r4, #456
	str	r3, [sp]
	ldr	r3, .L494+4
	bl	mpp_buffer_get_with_tag
.L443:
	movs	r2, #176
	ldr	r1, [r4, #452]
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	movs	r2, #177
	str	r0, [r5, #80]
	ldr	r1, [r4, #456]
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	str	r0, [r5, #84]
.L441:
	ldr	r2, [sp, #52]
	ldrb	r3, [r5, #144]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L444
	orr	r3, r3, #8
	movs	r2, #171
	strb	r3, [r5, #144]
	ldr	r1, [sp, #52]
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	str	r0, [r5, #60]
.L445:
	ldr	r3, [r6, #32]
	ldr	r2, [r3, #28]
	ldr	r0, [r3]
	cmp	r2, #0
	bne	.L446
	cmp	r0, #0
	beq	.L447
	mov	r1, r0
	movs	r2, #174
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	ldr	r3, [r6, #32]
	ldr	r3, [r3, #12]
	add	r0, r0, r3
	str	r0, [r5, #72]
.L448:
	ldr	r3, [r5, #72]
	subs	r1, r3, #1
	strd	r3, r3, [r5, #64]
	str	r3, [r5, #76]
	ldr	r2, [r6, #32]
	ldr	r2, [r2, #24]
	add	r2, r2, r1
	str	r2, [r5, #64]
	ldr	r2, [sp, #40]
	add	r3, r3, r2
	str	r3, [r5, #72]
.L449:
	ldr	r3, [sp, #40]
	cbz	r3, .L450
	ldr	r3, [r6, #32]
	ldr	r0, [r3]
	cmp	r0, #0
	beq	.L451
	ldr	r1, .L494
	bl	mpp_buffer_get_dma_with_caller
	ldr	r2, [r6, #32]
	movs	r1, #1
	ldr	r2, [r2, #12]
	ldr	r3, [sp, #40]
	bl	dma_buf_end_cpu_access_partial
.L450:
	ldr	r0, [r6, #52]
	bl	mpp_frame_get_offset_y
	ldrh	r3, [r5, #194]
	bfi	r3, r0, #0, #14
	strh	r3, [r5, #194]	@ movhi
	ldr	r0, [r6, #52]
	bl	mpp_frame_get_offset_x
	ldrh	r3, [r5, #192]
	bfi	r3, r0, #0, #14
	strh	r3, [r5, #192]	@ movhi
	add	sp, sp, #100
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L408:
	mul	r7, r7, r9
	cmp	r8, #13
	bhi	.L410
	tbb	[pc, r8]
.L412:
	.byte	(.L413-.L412)/2
	.byte	(.L413-.L412)/2
	.byte	(.L413-.L412)/2
	.byte	(.L410-.L412)/2
	.byte	(.L453-.L412)/2
	.byte	(.L415-.L412)/2
	.byte	(.L453-.L412)/2
	.byte	(.L487-.L412)/2
	.byte	(.L413-.L412)/2
	.byte	(.L413-.L412)/2
	.byte	(.L410-.L412)/2
	.byte	(.L410-.L412)/2
	.byte	(.L453-.L412)/2
	.byte	(.L411-.L412)/2
	.p2align 1
.L413:
	movs	r7, #0
	b	.L409
.L415:
	add	r0, r7, r7, lsl #1
	lsrs	r0, r0, #1
	b	.L409
.L411:
	lsls	r0, r7, #1
	b	.L409
.L410:
	mov	r3, r8
	movw	r2, #1287
	ldr	r1, .L494+8
	ldr	r0, .L494+12
	bl	printk
.L487:
	add	r0, r7, r7, lsl #2
	lsrs	r0, r0, #2
	b	.L409
.L453:
	mov	r0, r7
	b	.L409
.L419:
	ldr	r3, [r4, #572]
	ldr	fp, [r4, #620]
	str	r3, [sp, #16]
	str	fp, [sp, #48]
	b	.L488
.L417:
	ldr	r3, [sp, #64]
	add	r9, r9, r10, lsl #1
	ldr	r1, .L494+16
	ldr	r3, [r3]
	add	r9, r1, r9, lsl #2
	lsls	r0, r3, #23
	ldr	r9, [r9, #440]
	bpl	.L421
	mov	r3, r9
	mov	r2, #330
	addw	r1, r1, #417
	ldr	r0, .L494+20
	bl	printk
.L421:
	cmp	r9, #3
	bhi	.L420
	tbb	[pc, r9]
.L423:
	.byte	(.L426-.L423)/2
	.byte	(.L425-.L423)/2
	.byte	(.L424-.L423)/2
	.byte	(.L422-.L423)/2
	.p2align 1
.L426:
	ldr	r3, [r4, #576]
	ldr	fp, [r4, #620]
	str	r3, [sp, #44]
	ldr	r3, [r4, #624]
	str	r3, [sp, #48]
	ldr	r3, [r4, #572]
	str	r3, [sp, #16]
	ldr	r3, [r4, #564]
	str	r3, [sp, #28]
	ldr	r3, [r4, #568]
	str	r3, [sp, #24]
	ldr	r3, [r4, #612]
	str	r3, [sp, #36]
	ldr	r3, [r4, #616]
.L489:
	str	r3, [sp, #32]
	b	.L420
.L425:
	ldr	r3, [r4, #548]
	ldr	fp, [r4, #612]
	str	r3, [sp, #44]
	ldr	r3, [r4, #596]
	str	fp, [r4, #620]
	str	r3, [sp, #48]
	ldr	r3, [r4, #564]
	str	r3, [sp, #16]
	ldr	r2, [sp, #16]
	str	r3, [r4, #572]
	ldr	r3, [r4, #540]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	ldr	r2, [r4, #544]
	str	r3, [sp, #28]
	ldr	r3, [r4, #568]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [r4, #588]
	str	r2, [sp, #24]
	cmp	r3, fp
	ldr	r2, [r4, #592]
	it	cs
	movcs	r3, fp
	str	r3, [sp, #36]
	ldr	r3, [r4, #616]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	mov	r3, r2
	b	.L489
.L424:
	ldr	r0, .L494+24
	mov	r1, #362
.L490:
	bl	printk
	b	.L420
.L495:
	.align	2
.L494:
	.word	.LANCHOR0+456
	.word	.LC24
	.word	.LANCHOR0+392
	.word	.LC28
	.word	.LANCHOR0
	.word	.LC30
	.word	.LC31
.L422:
	ldr	r3, [r4, #652]
	cmp	r3, #0
	beq	.L427
	add	r1, r4, #564
	str	r1, [sp, #64]
	ldr	r1, [r4, #548]
	add	r3, r4, #588
	ldr	fp, [r4, #612]
	add	r2, r4, #540
	ldr	r9, [sp, #64]
	add	ip, r4, #612
	str	r1, [sp, #44]
	add	lr, sp, #72
	ldr	r1, [r4, #596]
	str	fp, [r4, #620]
	str	r1, [sp, #48]
	ldr	r1, [r4, #564]
	str	r1, [sp, #16]
	ldr	r0, [sp, #16]
	str	r1, [r4, #572]
	ldr	r1, [r4, #540]
	cmp	r1, r0
	it	cs
	movcs	r1, r0
	ldr	r0, [r4, #544]
	str	r1, [sp, #28]
	ldr	r1, [r4, #568]
	cmp	r0, r1
	it	cc
	movcc	r0, r1
	ldr	r1, [r4, #588]
	str	r0, [sp, #24]
	cmp	r1, fp
	ldr	r0, [r4, #592]
	it	cs
	movcs	r1, fp
	str	r1, [sp, #36]
	ldr	r1, [r4, #616]
	cmp	r0, r1
	it	cc
	movcc	r0, r1
	str	r0, [sp, #32]
.L428:
	ldr	r0, [r9]	@ unaligned
	add	r9, r9, #8
	ldr	r1, [r9, #-4]	@ unaligned
	cmp	r9, r3
	mov	r10, lr
	stmia	r10!, {r0, r1}
	mov	lr, r10
	bne	.L428
	ldr	lr, [sp, #64]
	add	r0, r2, #24
	mov	r1, r2
.L429:
	ldr	r9, [r1], #4	@ unaligned
	cmp	r1, r0
	str	r9, [lr], #4	@ unaligned
	bne	.L429
	add	r9, sp, #72
	add	r10, sp, #96
.L430:
	mov	lr, r9
	adds	r2, r2, #8
	ldmia	lr!, {r0, r1}
	cmp	lr, r10
	str	r0, [r2, #-8]	@ unaligned
	mov	r9, lr
	str	r1, [r2, #-4]	@ unaligned
	bne	.L430
	add	r9, sp, #72
	add	r10, ip, #24
	mov	r2, ip
.L431:
	ldr	r0, [r2]	@ unaligned
	adds	r2, r2, #8
	ldr	r1, [r2, #-4]	@ unaligned
	cmp	r2, r10
	mov	lr, r9
	stmia	lr!, {r0, r1}
	mov	r9, lr
	bne	.L431
	add	r1, r3, #24
	mov	r2, r3
.L432:
	ldr	r0, [r2], #4	@ unaligned
	cmp	r2, r1
	str	r0, [ip], #4	@ unaligned
	bne	.L432
	add	ip, sp, #72
	add	lr, sp, #96
.L433:
	mov	r2, ip
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r3, #-8]	@ unaligned
	mov	ip, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L433
	b	.L420
.L427:
	mov	r1, #390
	ldr	r0, .L496
	b	.L490
.L416:
	ldr	r3, [sp, #20]
	ldr	r3, [r3, #292]
	cbnz	r3, .L439
	ldr	r3, [sp, #56]
	movs	r2, #163
	ldr	r0, [r4, #64]
	ldr	r3, [r3, #4]
	ldr	r1, [r3, #12]
	bl	mpp_dev_get_iova_address
	str	r0, [r5, #28]
	ldr	r3, [r4, #508]
	add	r3, r3, r0
	str	r3, [r5, #32]
.L439:
	ldr	r3, [r8, #4]
	movs	r2, #165
	ldr	r0, [r4, #64]
	ldr	r1, [r3, #12]
	bl	mpp_dev_get_iova_address
	mov	r2, #-1
	str	r0, [r5, #36]
	ldr	r3, [r4, #508]
	add	r3, r3, r0
	str	r3, [r5, #40]
	movs	r3, #0
	strd	r2, r3, [r5, #96]
	strd	r2, r3, [r5, #104]
	b	.L436
.L444:
	ldr	r2, [sp, #52]
	bfi	r3, r2, #3, #1
	strb	r3, [r5, #144]
	str	r2, [r5, #60]
	b	.L445
.L447:
	ldr	r2, [r3, #4]
	ldr	r3, [r3, #12]
	add	r3, r3, r2
	str	r3, [r5, #72]
	b	.L448
.L446:
	ldr	r1, .L496+4
	bl	mpp_buffer_get_size_with_caller
	ldr	r3, [r6, #32]
	movs	r2, #173
	ldr	r1, [r3]
	mov	r8, r0
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	mov	r1, r8
	str	r0, [r5, #68]
	mov	r7, r0
	ldr	r3, [r6, #32]
	ldr	r0, [r3, #12]
	ldr	r3, [sp, #40]
	add	r0, r0, r3
	bl	__aeabi_uidivmod
	add	r1, r1, r7
	str	r1, [r5, #72]
	ldr	r3, [r6, #32]
	ldr	r3, [r3, #16]
	add	r3, r3, r7
	add	r7, r7, r8
	str	r3, [r5, #76]
	str	r7, [r5, #64]
	b	.L449
.L451:
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L450
	ldr	r0, [r4, #64]
	bl	mpp_get_dev
	ldr	r1, [r6, #32]
	movs	r3, #1
	ldr	r1, [r1, #4]
	ldr	r2, [sp, #40]
	bl	dma_sync_single_for_device
	b	.L450
.L497:
	.align	2
.L496:
	.word	.LC32
	.word	.LANCHOR0+456
	.fnend
	.size	vepu540c_h265_set_hw_address, .-vepu540c_h265_set_hw_address
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265e_v540c_gen_regs, %function
hal_h265e_v540c_gen_regs:
	.fnstart
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #92
	sub	sp, sp, #92
	ldr	r4, [r0, #68]
	mov	r5, r0
	ldr	r6, [r0, #484]
	mov	r10, r1
	add	r3, r4, #292
	str	r3, [sp, #36]
	ldr	r3, [r0, #444]
	ldr	r0, [r1, #52]
	ldr	r9, [r1, #16]
	str	r3, [sp, #28]
	ldr	r3, [r1, #4]
	str	r3, [sp, #32]
	bl	mpp_frame_get_is_full
	ldr	r3, .L723
	ldr	r3, [r3]
	lsls	r2, r3, #29
	str	r0, [sp, #40]
	bpl	.L499
	movw	r3, #2020
	ldr	r1, .L723+4
	mov	r2, r3
	ldr	r0, .L723+8
	bl	printk
.L499:
	ldrh	r3, [r9, #4]
	ldrh	fp, [r9, #6]
	add	r8, r3, #7
	adds	r3, r3, #31
	add	r7, fp, #7
	add	fp, fp, #31
	asrs	r3, r3, #5
	str	r3, [sp, #44]
	ldr	r3, .L723
	asr	fp, fp, #5
	ldr	r3, [r3]
	lsls	r3, r3, #30
	bpl	.L500
	ldr	r3, [r5, #392]
	movw	r2, #2027
	ldr	r1, .L723+4
	ldr	r0, .L723+12
	str	r3, [sp]
	ldr	r3, [r5, #424]
	bl	printk
.L500:
	mov	r2, #6656
	movs	r1, #0
	mov	r0, r4
	lsr	r8, r8, #3
	bl	memset
	movs	r2, #0
	strb	r2, [r4, #16]
	mov	r3, #0
	ldr	r1, [r5, #464]
	bfi	r3, r1, #0, #3
	strb	r3, [r4, #17]
	ldrb	r3, [r4, #20]	@ zero_extendqisi2
	bic	r3, r3, #3
	strb	r3, [r4, #20]
	movw	r3, #32255
	strh	r3, [r4, #32]	@ movhi
	mov	r3, #28672
	str	r3, [r4, #48]
	ldrh	r3, [r4, #54]
	bfi	r3, r2, #0, #10
	strh	r3, [r4, #54]	@ movhi
	mvn	r3, #14680064
	str	r3, [r4, #56]
	ldrb	r3, [r4, #84]	@ zero_extendqisi2
	bic	r3, r3, #7
	orr	r3, r3, #7
	strb	r3, [r4, #84]
	mov	r3, #-1
	str	r3, [r4, #88]
	add	r8, r8, r3
	ldrh	r3, [r4, #452]
	bfi	r3, r8, #0, #11
	strh	r3, [r4, #452]	@ movhi
	ldrh	r3, [r9, #4]
	and	r3, r3, #7
	uxth	r2, r3
	cmp	r2, #0
	beq	.L626
	rsb	r3, r3, #8
	and	r3, r3, #63
.L501:
	ldrb	r2, [r4, #456]	@ zero_extendqisi2
	lsrs	r7, r7, #3
	subs	r7, r7, #1
	bfi	r2, r3, #0, #6
	ldrh	r3, [r4, #454]
	strb	r2, [r4, #456]
	bfi	r3, r7, #0, #11
	strh	r3, [r4, #454]	@ movhi
	ldrh	r3, [r9, #6]
	and	r3, r3, #7
	uxth	r2, r3
	cmp	r2, #0
	beq	.L627
	rsb	r3, r3, #8
	and	r3, r3, #63
.L502:
	ldrb	r2, [r4, #458]	@ zero_extendqisi2
	bfi	r2, r3, #0, #6
	ldrb	r3, [r4, #436]	@ zero_extendqisi2
	strb	r2, [r4, #458]
	movs	r2, #1
	bfi	r3, r2, #0, #2
	strb	r3, [r4, #436]
	ldr	r3, [r9, #292]
	ldrb	r2, [r4, #436]	@ zero_extendqisi2
	clz	r3, r3
	lsrs	r3, r3, #5
	bfi	r2, r3, #2, #1
	ldr	r3, [sp, #44]
	orr	r2, r2, #16
	strb	r2, [r4, #436]
	mul	fp, fp, r3
	add	r0, fp, #-1
	lsls	r0, r0, #1
	bl	mpp_log2
	ldrb	r3, [r4, #438]	@ zero_extendqisi2
	bfi	r3, r0, #3, #5
	strb	r3, [r4, #438]
	ldrb	r3, [r4, #483]	@ zero_extendqisi2
	bic	r3, r3, #28
	strb	r3, [r4, #483]
	ldr	r3, [r5, #392]
	cmp	r3, #2
	beq	.L628
	ldr	r2, [r5, #444]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	cmp	r3, #1
	ite	ne
	movne	r2, #6
	moveq	r2, #9
.L503:
	ldrb	r3, [r4, #948]	@ zero_extendqisi2
	bfi	r3, r2, #0, #4
	strb	r3, [r4, #948]
	ldrb	r3, [r4, #949]	@ zero_extendqisi2
	movs	r2, #4
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #949]
	ldrb	r3, [r9, #228]	@ zero_extendqisi2
	ldrb	r2, [r4, #532]	@ zero_extendqisi2
	ubfx	r3, r3, #1, #1
	bfi	r2, r3, #1, #1
	strb	r2, [r4, #532]
	ldrb	r3, [r9, #228]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #2, #1
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #532]
	ldrb	r2, [r9, #228]	@ zero_extendqisi2
	ldrb	r3, [r4, #532]	@ zero_extendqisi2
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #532]
	ldrb	r3, [r9, #228]	@ zero_extendqisi2
	ldrb	r2, [r4, #533]	@ zero_extendqisi2
	ubfx	r3, r3, #3, #1
	bfi	r2, r3, #7, #1
	ldrh	r3, [r4, #534]
	strb	r2, [r4, #533]
	ldrh	r2, [r9, #260]
	bfi	r3, r2, #0, #15
	strh	r3, [r4, #534]	@ movhi
	ldr	r3, [r4, #540]
	ldrh	r2, [r9, #262]
	bfi	r3, r2, #0, #20
	str	r3, [r4, #540]
	ldr	r3, [r4, #536]
	ldr	r2, [r9, #284]
	bfi	r3, r2, #0, #20
	str	r3, [r4, #536]
	ldr	r2, [r5, #444]
	ldrb	r3, [r4, #660]	@ zero_extendqisi2
	ldr	r2, [r2, #764]
	bfi	r3, r2, #0, #3
	strb	r3, [r4, #660]
	ldrh	r3, [r4, #452]
	ldr	r2, [r4, #552]
	ubfx	r3, r3, #0, #11
	and	r2, r2, #-2013265920
	lsls	r3, r3, #3
	orr	r2, r2, #15859712
	adds	r3, r3, #39
	orr	r2, r2, #72
	str	r2, [r4, #552]
	asrs	r1, r3, #5
	ldr	r3, .L723+16
	str	r3, [r4, #548]
	ldrb	r3, [r9, #38]	@ zero_extendqisi2
	lsls	r0, r3, #28
	bpl	.L504
	ldr	r3, [r5, #392]
	cmp	r3, #2
	beq	.L504
	ldr	r3, [r5, #396]
	ldrb	r2, [r4, #559]	@ zero_extendqisi2
	subs	r3, r3, #2
	it	ne
	movne	r3, #1
	bfi	r2, r3, #7, #1
	orr	r2, r2, #64
	strb	r2, [r4, #559]
.L504:
	adds	r2, r1, #2
	subs	r3, r1, #1
	cmp	r3, r2
	mov	r0, #0
	mov	r1, #35
	mov	lr, #13
	it	ge
	movge	r3, r2
	adds	r3, r3, #1
	ldrh	r2, [r4, #558]
	movw	r8, #1795
	lsl	fp, r3, #2
	bfi	r2, r3, #2, #10
	strh	r2, [r4, #558]	@ movhi
	movs	r2, #4
.L505:
	mla	r7, lr, r1, r0
	sub	ip, r2, #4
	cmp	r7, r8
	bhi	.L507
	cmp	r2, #36
	bne	.L508
.L509:
	subs	r1, r2, #4
	lsrs	r2, r2, #1
	muls	r3, r1, r3
	ldrh	r1, [r4, #556]
	adds	r3, r3, #52
	lsrs	r3, r3, #2
	bfi	r1, r3, #0, #10
	ldrb	r3, [r4, #559]	@ zero_extendqisi2
	strh	r1, [r4, #556]	@ movhi
	ubfx	r1, r1, #8, #8
	bfi	r1, r2, #3, #5
	movs	r2, #1
	bfi	r3, r2, #4, #2
	ldr	r2, .L723+20
	strb	r3, [r4, #559]
	ldr	r3, [r4, #596]
	strb	r1, [r4, #557]
	ands	r2, r2, r3
	ldr	r3, .L723+24
	orrs	r3, r3, r2
	str	r3, [r4, #596]
	ldr	r2, [r5, #444]
	movw	r3, #4708
	ldr	r2, [r2, r3]
	ldrb	r3, [r4, #599]	@ zero_extendqisi2
	subs	r1, r2, #1
	rsbs	r2, r1, #0
	adcs	r2, r2, r1
	bfi	r3, r2, #4, #1
	orr	r3, r3, #32
	strb	r3, [r4, #599]
	ldrb	r2, [r9, #30]	@ zero_extendqisi2
	ldrb	r3, [r4, #596]	@ zero_extendqisi2
	bic	r3, r3, #3
	cbz	r2, .L510
	orr	r3, r3, #2
	b	.L724
.L725:
	.align	2
.L723:
	.word	hal_h265e_debug
	.word	.LANCHOR0+485
	.word	.LC0
	.word	.LC36
	.word	117999
	.word	-209223677
	.word	209192668
.L724:
.L510:
	strb	r3, [r4, #596]
	ldrb	r3, [r4, #598]	@ zero_extendqisi2
	orr	r3, r3, #128
	strb	r3, [r4, #598]
	ldrb	r3, [r4, #599]	@ zero_extendqisi2
	ldrb	r2, [r9, #36]	@ zero_extendqisi2
	bfi	r3, r2, #0, #2
	strb	r3, [r4, #599]
	ldrh	r3, [r4, #600]
	bfc	r3, #0, #9
	strh	r3, [r4, #600]	@ movhi
	ldr	r3, [r5, #392]
	cmp	r3, #2
	ldrb	r3, [r4, #612]	@ zero_extendqisi2
	ite	eq
	moveq	r2, #19
	movne	r2, #1
	bfi	r3, r2, #0, #6
	ldr	r2, [sp, #40]
	strb	r3, [r4, #612]
	ldr	r3, [r5, #516]
	orrs	r3, r2, r3
	beq	.L513
	ldr	r7, [r10, #52]
	mov	r0, r7
	bl	mpp_frame_get_is_full
	cmp	r0, #0
	bne	.L514
	add	r2, sp, #52
	movs	r1, #19
	bl	rk_dvbm_ctrl
	ldrb	r3, [r4, #96]	@ zero_extendqisi2
	ldrh	r2, [r4, #24]
	and	r3, r3, #8
	orn	r3, r3, #120
	strb	r3, [r4, #96]
	ldr	r3, [sp, #80]
	bfi	r2, r3, #0, #14
	strh	r2, [r4, #24]	@ movhi
	ldr	r2, [sp, #76]
	ldr	r1, [r4, #24]
	uxtb	r0, r2
	strb	r0, [r4, #444]
	bfi	r1, r0, #14, #8
	str	r1, [r4, #24]
	lsrs	r1, r1, #24
	bic	r1, r1, #7
	orr	r1, r1, #7
	strb	r1, [r4, #27]
	ldrb	r1, [r4, #446]	@ zero_extendqisi2
	bic	r1, r1, #7
	orr	r1, r1, #5
	strb	r1, [r4, #446]
	ldr	r1, [sp, #52]
	str	r1, [r4, #292]
	ldr	r1, [sp, #64]
	str	r1, [r4, #296]
	ldr	r1, [sp, #56]
	str	r1, [r4, #300]
	ldr	r1, [sp, #68]
	str	r1, [r4, #304]
	ldr	r1, [sp, #60]
	str	r1, [r4, #308]
	ldr	r1, [sp, #72]
	strd	r1, r1, [r4, #312]
	ldr	r1, [sp, #84]
	cbz	r1, .L513
	movw	r1, #1972
	ldr	r0, .L726
	bl	printk
	mov	r0, #-1
.L498:
	add	sp, sp, #92
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L626:
	mov	r3, r2
	b	.L501
.L627:
	mov	r3, r2
	b	.L502
.L628:
	movs	r2, #6
	b	.L503
.L508:
	cmp	r1, #8
	add	r2, r2, #4
	it	hi
	subhi	r1, r1, #4
	add	r0, r0, fp
	it	ls
	movls	r1, #4
	b	.L505
.L507:
	movw	r1, #1796
	cmp	r7, r1
	it	ne
	movne	r2, ip
	b	.L509
.L514:
	mov	r0, r7
	bl	mpp_frame_get_phy_addr
	mov	r8, r0
	mov	r0, r7
	bl	mpp_frame_get_hor_stride
	mov	fp, r0
	mov	r0, r7
	bl	mpp_frame_get_ver_stride
	mul	r0, r0, fp
	cmp	r8, #0
	beq	.L517
	add	r0, r0, r8
	str	r8, [r4, #308]
	strd	r0, r0, [r4, #312]
.L513:
	mov	r2, r10
	ldr	r1, [sp, #36]
	mov	r0, r5
	bl	vepu540c_h265_set_hw_address
	ldr	r2, [r6, #16]
	ldrb	r3, [r4, #48]	@ zero_extendqisi2
	bfi	r3, r2, #4, #4
	strb	r3, [r4, #48]
	ldr	r2, [r6]
	ldrb	r3, [r4, #460]	@ zero_extendqisi2
	and	r2, r2, #15
	bfi	r3, r2, #2, #4
	strb	r3, [r4, #460]
	ldr	r1, [r6, #4]
	bfi	r3, r1, #0, #1
	strb	r3, [r4, #460]
	ldr	r1, [r6, #8]
	bfi	r3, r1, #1, #1
	orn	r3, r3, #127
	strb	r3, [r4, #460]
	ldr	r3, [sp, #28]
	ldr	r3, [r3, #60]
	cmp	r3, #0
	ldrb	r3, [r4, #483]	@ zero_extendqisi2
	ite	le
	movle	r1, #0
	movgt	r1, #1
	bfi	r3, r1, #2, #1
	strb	r3, [r4, #483]
	ldr	r1, [sp, #28]
	ldr	r1, [r1, #56]
	bfi	r3, r1, #3, #2
	strb	r3, [r4, #483]
	ldr	r3, [sp, #28]
	ldr	r1, [r3, #20]
	ldr	r3, [r6]
	cbnz	r1, .L518
	ldr	r1, [sp, #28]
	ldr	r1, [r1, #12]
	cbnz	r3, .L519
	lsls	r1, r1, #2
.L518:
	subs	r3, r3, #4
	cmp	r3, #9
	bhi	.L522
	tbh	[pc, r3, lsl #1]
.L524:
	.2byte	(.L632-.L524)/2
	.2byte	(.L522-.L524)/2
	.2byte	(.L632-.L524)/2
	.2byte	(.L522-.L524)/2
	.2byte	(.L522-.L524)/2
	.2byte	(.L522-.L524)/2
	.2byte	(.L522-.L524)/2
	.2byte	(.L522-.L524)/2
	.2byte	(.L525-.L524)/2
	.2byte	(.L632-.L524)/2
	.p2align 1
.L517:
	movw	r1, #1989
	ldr	r0, .L726+4
	bl	printk
	b	.L513
.L519:
	cmp	r3, #1
	bne	.L520
	add	r1, r1, r1, lsl #1
	b	.L518
.L520:
	cmp	r3, #2
	beq	.L521
	sub	r0, r3, #8
	cmp	r0, #1
	bhi	.L518
.L521:
	lsls	r1, r1, #1
	b	.L518
.L727:
	.align	2
.L726:
	.word	.LC37
	.word	.LC38
.L525:
	lsls	r0, r1, #1
.L523:
	cmp	r2, #2
	bhi	.L526
	ldr	r3, [r4, #464]
	and	r2, r3, #-134217728
	ldr	r3, .L728
	orrs	r3, r3, r2
	str	r3, [r4, #464]
	ldr	r3, [r4, #468]
	and	r2, r3, #-134217728
	ldr	r3, .L728+4
	orrs	r3, r3, r2
	str	r3, [r4, #468]
	ldr	r3, [r4, #472]
	and	r2, r3, #-134217728
	ldr	r3, .L728+8
	orrs	r3, r3, r2
	str	r3, [r4, #472]
	ldr	r3, [r4, #476]
	lsrs	r3, r3, #21
	lsls	r3, r3, #21
	orr	r3, r3, #32768
	orr	r3, r3, #128
	str	r3, [r4, #476]
.L526:
	ldr	r3, [r4, #488]
	strh	r0, [r4, #492]	@ movhi
	bfi	r3, r1, #0, #17
	str	r3, [r4, #488]
	ldr	r7, [r5, #444]
	ldr	r6, [r10, #4]
	ldr	r3, [r7, #108]
	cmp	r3, #2
	bne	.L527
	ldr	r2, [r6, #172]
	ldrb	r3, [r4, #437]	@ zero_extendqisi2
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #437]
	ldrb	r3, [r4, #630]	@ zero_extendqisi2
	ldr	r2, [r6, #172]
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #630]
	ldrh	r3, [r4, #522]
	ldr	r2, [r6, #172]
	bfi	r3, r2, #4, #6
	strh	r3, [r4, #522]	@ movhi
	ldr	r2, [r6, #172]
	ubfx	r3, r3, #8, #8
	bfi	r3, r2, #2, #6
	strb	r3, [r4, #523]
	b	.L729
.L730:
	.align	2
.L728:
	.word	20261917
	.word	123164288
	.word	33762283
.L729:
.L528:
	ldrb	r2, [r9, #36]	@ zero_extendqisi2
	ldrb	r3, [r4, #616]	@ zero_extendqisi2
	ubfx	r2, r2, #3, #1
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #616]
	ldrb	r2, [r9, #29]	@ zero_extendqisi2
	bfi	r3, r2, #1, #7
	strb	r3, [r4, #616]
	ldrb	r3, [r4, #617]	@ zero_extendqisi2
	ldrb	r2, [r9, #30]	@ zero_extendqisi2
	bfi	r3, r2, #1, #6
	strb	r3, [r4, #617]
	ldrb	r3, [r9, #38]	@ zero_extendqisi2
	ldrb	r2, [r4, #617]	@ zero_extendqisi2
	ubfx	r3, r3, #2, #1
	bfi	r2, r3, #0, #1
	strb	r2, [r4, #617]
	ldrb	r3, [r9, #38]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #3, #1
	bfi	r2, r3, #7, #1
	strb	r2, [r4, #617]
	ldrb	r3, [r9, #21]	@ zero_extendqisi2
	ldrb	r2, [r4, #618]	@ zero_extendqisi2
	ubfx	r3, r3, #1, #4
	bfi	r2, r3, #0, #4
	strb	r2, [r4, #618]
	ldrb	r3, [r9, #38]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #4, #1
	bfi	r2, r3, #4, #1
	strb	r2, [r4, #618]
	ldrb	r3, [r9, #38]	@ zero_extendqisi2
	ldrb	r2, [r4, #620]	@ zero_extendqisi2
	ubfx	r3, r3, #5, #1
	bfi	r2, r3, #0, #1
	strb	r2, [r4, #620]
	ldrb	r3, [r9, #38]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #6, #1
	bfi	r2, r3, #1, #1
	strb	r2, [r4, #620]
	ldrh	r3, [r9, #38]
	uxtb	r2, r2
	ubfx	r3, r3, #7, #3
	bfi	r2, r3, #2, #3
	strb	r2, [r4, #620]
	ldrb	r3, [r9, #39]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #2, #1
	bfi	r2, r3, #5, #1
	strb	r2, [r4, #620]
	ldrb	r3, [r9, #39]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #3, #1
	bfi	r2, r3, #6, #1
	strb	r2, [r4, #620]
	ldrb	r3, [r9, #33]	@ zero_extendqisi2
	ldrh	r2, [r4, #620]
	adds	r3, r3, #26
	bfi	r2, r3, #7, #6
	strh	r2, [r4, #620]	@ movhi
	ldrb	r3, [r9, #40]	@ zero_extendqisi2
	ldrb	r2, [r4, #621]	@ zero_extendqisi2
	ubfx	r3, r3, #2, #1
	bfi	r2, r3, #5, #1
	strb	r2, [r4, #621]
	ldrb	r3, [r9, #40]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #3, #1
	bfi	r2, r3, #6, #1
	strb	r2, [r4, #621]
	ldrb	r3, [r9, #41]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #3, #1
	bfi	r2, r3, #7, #1
	strb	r2, [r4, #621]
	ldrb	r3, [r9, #41]	@ zero_extendqisi2
	ldrb	r2, [r4, #622]	@ zero_extendqisi2
	ubfx	r3, r3, #4, #1
	bfi	r2, r3, #0, #1
	strb	r2, [r4, #622]
	ldrb	r3, [r9, #41]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #6, #1
	bfi	r2, r3, #1, #1
	strb	r2, [r4, #622]
	ldrb	r2, [r9, #41]	@ zero_extendqisi2
	ldrb	r3, [r4, #622]	@ zero_extendqisi2
	lsrs	r2, r2, #7
	bfi	r3, r2, #2, #1
	strb	r3, [r4, #622]
	ldrb	r2, [r9, #208]	@ zero_extendqisi2
	bfi	r3, r2, #3, #2
	strb	r3, [r4, #622]
	ldrb	r3, [r9, #41]	@ zero_extendqisi2
	ldrb	r2, [r4, #622]	@ zero_extendqisi2
	ubfx	r3, r3, #2, #1
	bfi	r2, r3, #5, #1
	strb	r2, [r4, #622]
	ldrb	r3, [r9, #228]	@ zero_extendqisi2
	ldrb	r2, [r4, #624]	@ zero_extendqisi2
	ubfx	r3, r3, #4, #1
	bfi	r2, r3, #0, #1
	strb	r2, [r4, #624]
	ldrb	r3, [r9, #228]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #5, #1
	bfi	r2, r3, #1, #1
	strb	r2, [r4, #624]
	ldrb	r3, [r9, #228]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #6, #1
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #624]
	ldrb	r3, [r9, #228]	@ zero_extendqisi2
	uxtb	r2, r2
	lsrs	r3, r3, #7
	bfi	r2, r3, #3, #1
	strb	r2, [r4, #624]
	ldrb	r2, [r9, #229]	@ zero_extendqisi2
	ldrb	r3, [r4, #624]	@ zero_extendqisi2
	bfi	r3, r2, #5, #1
	strb	r3, [r4, #624]
	ldrb	r2, [r9, #242]	@ zero_extendqisi2
	bfi	r3, r2, #6, #2
	strb	r3, [r4, #624]
	ldrb	r3, [r4, #625]	@ zero_extendqisi2
	ldrb	r2, [r9, #243]	@ zero_extendqisi2
	bfi	r3, r2, #0, #2
	strb	r3, [r4, #625]
	ldrb	r3, [r9, #229]	@ zero_extendqisi2
	ldrb	r2, [r4, #625]	@ zero_extendqisi2
	ubfx	r3, r3, #1, #1
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #625]
	ldrb	r3, [r9, #229]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #2, #1
	bfi	r2, r3, #3, #1
	strb	r2, [r4, #625]
	ldrb	r3, [r9, #229]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #3, #1
	bfi	r2, r3, #4, #1
	strb	r2, [r4, #625]
	ldrb	r3, [r9, #229]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #4, #1
	bfi	r2, r3, #5, #1
	ldr	r3, [r4, #436]
	strb	r2, [r4, #625]
	ldr	r2, [r9, #288]
	bfi	r3, r2, #14, #5
	str	r3, [r4, #436]
	ldrb	r3, [r9, #229]	@ zero_extendqisi2
	ldrb	r2, [r4, #625]	@ zero_extendqisi2
	ubfx	r3, r3, #5, #1
	bfi	r2, r3, #6, #1
	strb	r2, [r4, #625]
	ldrb	r2, [r9, #244]	@ zero_extendqisi2
	ldr	r3, [r4, #624]
	bfi	r3, r2, #15, #2
	str	r3, [r4, #624]
	ldrb	r2, [r9, #245]	@ zero_extendqisi2
	ubfx	r3, r3, #16, #8
	bfi	r3, r2, #1, #7
	strb	r3, [r4, #626]
	ldrb	r2, [r9, #229]	@ zero_extendqisi2
	ldrb	r3, [r4, #627]	@ zero_extendqisi2
	ubfx	r2, r2, #6, #1
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #627]
	ldrb	r2, [r9, #246]	@ zero_extendqisi2
	bfi	r3, r2, #1, #6
	strb	r3, [r4, #627]
	ldrb	r3, [r9, #229]	@ zero_extendqisi2
	ldrb	r2, [r4, #627]	@ zero_extendqisi2
	lsrs	r3, r3, #7
	bfi	r2, r3, #7, #1
	strb	r2, [r4, #627]
	ldrb	r2, [r9, #236]	@ zero_extendqisi2
	ldrb	r3, [r4, #628]	@ zero_extendqisi2
	bfi	r3, r2, #0, #4
	strb	r3, [r4, #628]
	ldrb	r2, [r9, #237]	@ zero_extendqisi2
	bfi	r3, r2, #4, #4
	strb	r3, [r4, #628]
	ldrb	r3, [r4, #629]	@ zero_extendqisi2
	ldrb	r2, [r9, #230]	@ zero_extendqisi2
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #629]
	ldrb	r3, [r9, #230]	@ zero_extendqisi2
	ldrb	r2, [r4, #629]	@ zero_extendqisi2
	ubfx	r3, r3, #1, #1
	bfi	r2, r3, #1, #1
	strb	r2, [r4, #629]
	ldrb	r3, [r9, #230]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #2, #1
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #629]
	ldrb	r3, [r9, #40]	@ zero_extendqisi2
	lsls	r2, r3, #28
	ldrb	r2, [r4, #629]	@ zero_extendqisi2
	ite	mi
	ldrbmi	r3, [r9, #238]	@ zero_extendqisi2
	ldrbpl	r3, [r9, #44]	@ zero_extendqisi2
	and	r3, r3, #31
	bfi	r2, r3, #3, #5
	ldrb	r3, [r4, #630]	@ zero_extendqisi2
	strb	r2, [r4, #629]
	ldrb	r2, [r9, #240]	@ zero_extendqisi2
	bfi	r3, r2, #6, #2
	strb	r3, [r4, #630]
	ldrb	r3, [r9, #230]	@ zero_extendqisi2
	ldrb	r2, [r4, #631]	@ zero_extendqisi2
	ubfx	r3, r3, #3, #1
	bfi	r2, r3, #1, #1
	strb	r2, [r4, #631]
	ldrb	r3, [r9, #230]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #4, #1
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #631]
	ldrh	r3, [r9, #254]
	strh	r3, [r4, #632]	@ movhi
	ldrh	r3, [r4, #634]
	ldrh	r2, [r9, #256]
	bfi	r3, r2, #0, #9
	strh	r3, [r4, #634]	@ movhi
	ldrb	r3, [r9, #230]	@ zero_extendqisi2
	ldrb	r2, [r4, #636]	@ zero_extendqisi2
	ubfx	r3, r3, #5, #1
	bfi	r2, r3, #0, #1
	strb	r2, [r4, #636]
	ldrh	r2, [r9, #258]
	ldr	r3, [r4, #636]
	bfi	r3, r2, #1, #16
	str	r3, [r4, #636]
	ldrb	r2, [r9, #248]	@ zero_extendqisi2
	ubfx	r3, r3, #16, #8
	bfi	r3, r2, #6, #2
	strb	r3, [r4, #638]
	ldrb	r3, [r9, #230]	@ zero_extendqisi2
	ldrb	r2, [r4, #643]	@ zero_extendqisi2
	lsrs	r3, r3, #7
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #643]
	ldrh	r2, [r9, #264]
	ldr	r3, [r4, #640]
	bfi	r3, r2, #10, #16
	str	r3, [r4, #640]
	ldrb	r3, [r9, #231]	@ zero_extendqisi2
	ldrb	r2, [r4, #643]	@ zero_extendqisi2
	ubfx	r3, r3, #2, #1
	bfi	r2, r3, #5, #1
	strb	r2, [r4, #643]
	ldrb	r3, [r9, #231]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #3, #1
	bfi	r2, r3, #6, #1
	strb	r2, [r4, #643]
	ldrb	r3, [r9, #231]	@ zero_extendqisi2
	uxtb	r2, r2
	ubfx	r3, r3, #4, #1
	bfi	r2, r3, #7, #1
	strb	r2, [r4, #643]
	ldrb	r3, [r9, #230]	@ zero_extendqisi2
	uxtb	r2, r2
	lsrs	r3, r3, #7
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #643]
	ldrh	r2, [r9, #264]
	ldr	r3, [r4, #640]
	bfi	r3, r2, #10, #16
	str	r3, [r4, #640]
	ldrb	r2, [r9, #231]	@ zero_extendqisi2
	ldrb	r3, [r4, #643]	@ zero_extendqisi2
	bfi	r3, r2, #3, #1
	strb	r3, [r4, #643]
	ldrh	r3, [r4, #640]
	ldrb	r2, [r9, #252]	@ zero_extendqisi2
	bfi	r3, r2, #5, #5
	strh	r3, [r4, #640]	@ movhi
	ldrb	r2, [r9, #230]	@ zero_extendqisi2
	ldrb	r3, [r4, #640]	@ zero_extendqisi2
	ubfx	r2, r2, #6, #1
	bfi	r3, r2, #4, #1
	strb	r3, [r4, #640]
	ldrb	r2, [r9, #251]	@ zero_extendqisi2
	bfi	r3, r2, #0, #4
	strb	r3, [r4, #640]
	ldrh	r3, [r9, #266]
	ldrb	r2, [r4, #643]	@ zero_extendqisi2
	strh	r3, [r4, #644]	@ movhi
	ldrh	r3, [r9, #268]
	strh	r3, [r4, #646]	@ movhi
	ldrh	r3, [r9, #270]
	strh	r3, [r4, #648]	@ movhi
	ldrh	r3, [r9, #272]
	strh	r3, [r4, #650]	@ movhi
	ldrh	r3, [r9, #274]
	strh	r3, [r4, #652]	@ movhi
	ldrh	r3, [r9, #278]
	strh	r3, [r4, #656]	@ movhi
	ldrh	r3, [r9, #276]
	strh	r3, [r4, #654]	@ movhi
	ldrb	r3, [r9, #231]	@ zero_extendqisi2
	ubfx	r3, r3, #1, #1
	bfi	r2, r3, #4, #1
	strb	r2, [r4, #643]
	ldrh	r3, [r9, #280]
	strh	r3, [r4, #658]	@ movhi
	ldrb	r3, [r4, #631]	@ zero_extendqisi2
	ldrb	r2, [r9, #241]	@ zero_extendqisi2
	bfi	r3, r2, #3, #4
	strb	r3, [r4, #631]
	ldrb	r3, [r4, #624]	@ zero_extendqisi2
	ldrb	r2, [r9, #229]	@ zero_extendqisi2
	bfi	r3, r2, #5, #1
	strb	r3, [r4, #624]
	ldr	r1, [r5, #648]
	cmp	r1, #0
	beq	.L567
	movs	r2, #179
	ldr	r0, [r5, #64]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #384]
	ldr	r3, [r5, #644]
	add	r3, r3, r0
	str	r3, [r4, #380]
.L568:
	ldr	r3, [r5, #436]
	cbz	r3, .L569
	add	r0, r5, #428
	bl	vepu540c_set_osd
.L569:
	ldr	r3, [r5, #524]
	cbz	r3, .L571
	ldr	r2, [r5, #444]
	movw	r3, #4708
	ldr	ip, [r2, r3]
	cmp	ip, #1
	bne	.L571
	ldr	r7, [sp, #28]
	add	r3, r10, #72
	str	r3, [sp, #36]
	add	r0, r4, #952
	ldr	fp, [r10, #84]
	ldrd	lr, r8, [r7, #12]
	ldr	r7, [r5, #392]
	ldrb	r6, [r4, #437]	@ zero_extendqisi2
	subs	r3, r7, #2
	ldrd	r1, r2, [r10, #64]
	rsbs	r7, r3, #0
	and	r6, r6, #63
	adcs	r7, r7, r3
	ldr	r3, [r5, #528]
	strd	ip, r7, [sp, #16]
	strd	lr, r8, [sp, #8]
	strd	fp, r6, [sp]
	cmp	r3, #0
	beq	.L573
	ldr	r3, [sp, #36]
	bl	vepu540c_set_qpmap_smart
.L574:
	cbnz	r0, .L571
	movs	r2, #186
	ldr	r1, [r10, #68]
	ldr	r0, [r5, #64]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #412]
.L571:
	ldr	r1, [r5, #440]
	cbz	r1, .L575
	ldr	r3, [sp, #28]
	add	r0, r4, #952
	ldr	r2, [sp, #28]
	ldr	r3, [r3, #16]
	ldr	r2, [r2, #12]
	bl	vepu540c_set_roi
.L575:
	ldr	r0, [r10, #52]
	ldr	r6, [r5, #444]
	ldr	r7, [r5, #400]
	bl	mpp_frame_get_ppinfo
	ldr	r2, [r5, #444]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	ldr	r2, [r4, #1832]
	cmp	r3, #1
	ldrh	r3, [r4, #1836]
	and	r2, r2, #65024
	bne	.L576
	ldr	r1, .L731
	and	r3, r3, #32512
	orr	r3, r3, #32768
	orr	r3, r3, #9
	orrs	r1, r1, r2
	strh	r3, [r4, #1836]	@ movhi
	str	r1, [r4, #1832]
	ldr	r3, [r5, #528]
	cbz	r3, .L577
	ldr	r3, .L731+4
	and	r1, r1, #65280
	orrs	r3, r3, r1
	str	r3, [r4, #1832]
	movs	r3, #16
	strb	r3, [r4, #1836]
.L577:
	ldr	r1, [r5, #420]
	ldrd	r3, r2, [r5, #412]
	add	r3, r3, r2
	cmp	r3, r1, lsr #1
	ite	hi
	movhi	r3, #1
	movls	r3, #0
	cmp	r1, r7, lsr #8
	bcs	.L578
	subs	r3, r3, #8
.L579:
	ldrb	r2, [r4, #1837]	@ zero_extendqisi2
	bfi	r2, r3, #0, #4
	mov	r3, r2
	bfc	r3, #4, #1
	strb	r3, [r4, #1837]
	ldr	r3, [r5, #392]
	cmp	r3, #2
	ldrb	r3, [r4, #1837]	@ zero_extendqisi2
	bne	.L582
.L721:
	movs	r2, #1
.L711:
	bfi	r3, r2, #5, #2
	strb	r3, [r4, #1837]
	ldrh	r3, [r4, #1816]
	ldr	r7, .L731+8
	bic	r3, r3, #1016
	orr	r2, r3, #256
	ldr	r3, [r4, #1820]
	strh	r2, [r4, #1816]	@ movhi
	mov	r8, r7
	and	r1, r7, r3
	ldr	r3, .L731+12
	orrs	r3, r3, r1
	ldr	r1, [r4, #1824]
	str	r3, [r4, #1820]
	ldr	r3, .L731+16
	ands	r3, r3, r1
	str	r3, [r4, #1824]
	ldrh	r3, [r4, #1838]
	and	r3, r3, #14
	orr	r3, r3, #1
	strh	r3, [r4, #1838]	@ movhi
	cmp	r0, #0
	bne	.L585
.L586:
	ldr	r3, [r4, #1840]
	and	r3, r3, #-268374016
	orr	r3, r3, #1572864
	str	r3, [r4, #1840]
	ldr	r3, [r4, #1844]
	and	r3, r3, #-268374016
	orr	r3, r3, #4194304
	orr	r3, r3, #48
	str	r3, [r4, #1844]
	ldr	r3, [r4, #1848]
	and	r3, r3, #-268374016
	orr	r3, r3, #2097152
	orr	r3, r3, #16
	str	r3, [r4, #1848]
	ldr	r3, [r4, #1852]
	and	r3, r3, #-268374016
	orr	r3, r3, #6291456
	orr	r3, r3, #48
	str	r3, [r4, #1852]
	ldr	r3, [r4, #1856]
	and	r3, r3, #-268374016
	orr	r3, r3, #1572864
	orr	r3, r3, #48
	str	r3, [r4, #1856]
	ldr	r3, [r4, #1860]
	and	r3, r3, #-268374016
	orr	r3, r3, #3145728
	orr	r3, r3, #96
	str	r3, [r4, #1860]
	ldr	r3, [r4, #1864]
	and	r3, r3, #-252645136
	orr	r3, r3, #16843009
	orr	r3, r3, #33554944
	str	r3, [r4, #1864]
	ldr	r3, [r4, #1868]
	and	r3, r3, #-252645136
	orr	r3, r3, #67109888
	orr	r3, r3, #65537
	str	r3, [r4, #1868]
	ldr	r3, [r4, #1872]
	and	r3, r3, #-252645136
	orr	r3, r3, #50332416
	str	r3, [r4, #1872]
	ldrh	r3, [r4, #1876]
	bic	r3, r3, #3840
	bic	r3, r3, #15
	orr	r3, r3, #768
	orr	r3, r3, #1
	strh	r3, [r4, #1876]	@ movhi
	ldr	r3, [r4, #1880]
	and	r3, r3, #-1061109568
	orr	r3, r3, #150997248
	orr	r3, r3, #393222
	str	r3, [r4, #1880]
	ldr	r3, [r4, #1884]
	and	r2, r3, #-1061109568
	ldr	r3, .L731+20
	orrs	r3, r3, r2
	str	r3, [r4, #1884]
	ldr	r3, [r4, #1888]
	and	r2, r3, #-1061109568
	ldr	r3, .L731+24
	orrs	r3, r3, r2
	str	r3, [r4, #1888]
	ldrh	r3, [r4, #1892]
	bic	r3, r3, #16128
	bic	r3, r3, #63
	orr	r3, r3, #4096
	orr	r3, r3, #7
	strh	r3, [r4, #1892]	@ movhi
	ldr	r3, [r4, #1896]
	and	r3, r3, #-16711936
	orr	r3, r3, #393216
	orr	r3, r3, #9
	str	r3, [r4, #1896]
	ldr	r3, [r4, #1900]
	and	r2, r3, #-16777216
	ldr	r3, .L731+28
	orrs	r3, r3, r2
	str	r3, [r4, #1900]
	ldr	r3, [r4, #1912]
	ldr	r2, [r4, #1916]
	and	r3, r3, #-268374016
	and	r2, r2, #-268374016
	orr	r3, r3, #655360
	orr	r2, r2, #4718592
	orr	r3, r3, #5
	orr	r2, r2, #15
	str	r3, [r4, #1912]
	str	r2, [r4, #1916]
	movw	r2, #4708
	ldr	r1, [r5, #444]
	ldr	r2, [r1, r2]
	cmp	r2, #1
	bne	.L587
	ldr	r2, .L731+32
	str	r2, [r4, #1920]
	movs	r2, #24
	strb	r2, [r4, #1924]
	ldr	r2, [r5, #528]
	cbz	r2, .L588
	and	r3, r3, #-268374016
	orr	r3, r3, #458752
	orr	r3, r3, #1
	str	r3, [r4, #1912]
	movs	r3, #10
	strb	r3, [r4, #1921]
.L588:
	ldr	r3, [r4, #1944]
	movs	r1, #72
	and	r2, r3, #-268374016
	ldr	r3, .L731+36
	orrs	r2, r2, r3
	str	r2, [r4, #1944]
	ldrh	r2, [r4, #1948]
	bfi	r2, r1, #0, #12
	strh	r2, [r4, #1948]	@ movhi
	ldr	r1, [r5, #444]
	movw	r2, #4708
	ldr	r2, [r1, r2]
	cmp	r2, #1
	mov	r2, r3
	ite	eq
	ldreq	r3, .L731+40
	movne	r3, #269488144
	str	r3, [r4, #1952]
	ldr	r3, [r4, #1968]
	and	r3, r3, #-268374016
	orrs	r3, r3, r2
	str	r3, [r4, #1968]
	ldrh	r3, [r4, #1972]
	movs	r2, #72
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #1972]	@ movhi
	ldr	r2, [r5, #444]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	cmp	r3, #1
	ite	eq
	ldreq	r3, .L731+44
	movne	r3, #269488144
	str	r3, [r4, #1976]
	ldr	r3, [r4, #1928]
	and	r3, r3, #-268374016
	orr	r3, r3, #655360
	orr	r3, r3, #1
	str	r3, [r4, #1928]
	ldr	r3, [r4, #1932]
	and	r3, r3, #-268374016
	orr	r3, r3, #1638400
	orr	r3, r3, #15
	str	r3, [r4, #1932]
	ldr	r2, [r5, #444]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	cmp	r3, #1
	mov	r3, #16
	bne	.L593
	ldr	r2, .L731+32
	strb	r3, [r4, #1940]
	str	r2, [r4, #1936]
	ldr	r3, [r5, #528]
	cbz	r3, .L594
	ldrh	r3, [r4, #1930]
	movs	r2, #7
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #1930]	@ movhi
	movs	r3, #10
	strb	r3, [r4, #1937]
.L594:
	ldr	r3, [r4, #1956]
	movs	r1, #72
	ldr	r0, .L731+36
	and	r2, r3, #-268374016
	ldrh	r3, [r4, #1960]
	orrs	r2, r2, r0
	str	r2, [r4, #1956]
	bfi	r3, r1, #0, #12
	strh	r3, [r4, #1960]	@ movhi
	ldr	r3, [r4, #1980]
	mov	r2, #269488144
	str	r2, [r4, #1964]
	and	r3, r3, #-268374016
	orrs	r3, r3, r0
	str	r3, [r4, #1980]
	ldrh	r3, [r4, #1984]
	bfi	r3, r1, #0, #12
	strh	r3, [r4, #1984]	@ movhi
	ldr	r1, [r5, #444]
	movw	r3, #4708
	ldr	r3, [r1, r3]
	ldr	r1, .L731+48
	cmp	r3, #1
	itee	ne
	strne	r2, [r4, #1988]
	ldreq	r3, .L731+52
	streq	r3, [r4, #1988]
	ldr	r3, [r4, #1992]
	and	r2, r3, #-1061109568
	ldr	r3, [r4, #1996]
	orrs	r2, r2, r1
	str	r2, [r4, #1992]
	and	r3, r3, #-252645136
	orrs	r3, r3, r1
	str	r3, [r4, #1996]
	ldr	r3, [r4, #2000]
	ldr	r1, .L731+56
	and	r2, r3, #-1061109568
	ldr	r3, .L731+60
	str	r1, [r4, #2008]
	orrs	r3, r3, r2
	b	.L732
.L733:
	.align	2
.L731:
	.word	134742300
	.word	269484050
	.word	-536346112
	.word	1049601
	.word	-267648256
	.word	134613766
	.word	335942665
	.word	2175313
	.word	269488148
	.word	2621460
	.word	403705872
	.word	403970331
	.word	67175425
	.word	269752603
	.word	100860677
	.word	134746128
.L732:
	movw	r2, #2567
	str	r3, [r4, #2000]
	strh	r2, [r4, #2012]	@ movhi
	movw	r2, #4708
	ldr	r3, [r5, #444]
	ldr	r2, [r3, r2]
	ldr	r3, [r4, #2024]
	cmp	r2, #1
	bne	.L599
	ldr	r2, [r5, #528]
	cmp	r2, #0
	bne	.L599
	sub	r2, r2, #-320017172
	sub	r2, r2, #16777472
	str	r2, [r4, #2016]
	sub	r2, r2, #329728
	sub	r2, r2, #270
.L714:
	lsrs	r3, r3, #19
	str	r2, [r4, #2020]
	ldr	r2, .L734
	lsls	r3, r3, #19
	str	r1, [r4, #2028]
	orrs	r2, r2, r3
	movw	r3, #1797
	str	r2, [r4, #2024]
	strh	r3, [r4, #2032]	@ movhi
	movw	r3, #4708
	ldr	r2, [r5, #444]
	ldr	r3, [r2, r3]
	cmp	r3, #1
	ldr	r3, [r4, #2044]
	bne	.L601
	ldr	r2, [r5, #528]
	cmp	r2, #0
	bne	.L601
	sub	r2, r2, #-320017172
	sub	r2, r2, #16777472
	str	r2, [r4, #2036]
	sub	r2, r2, #329728
	sub	r2, r2, #270
.L715:
	lsrs	r3, r3, #19
	str	r2, [r4, #2040]
	ldr	r2, .L734
	lsls	r3, r3, #19
	ldr	r7, .L734+4
	orrs	r2, r2, r3
	ldr	r3, [r4, #2056]
	str	r2, [r4, #2044]
	ldr	r2, .L734+8
	ands	r2, r2, r3
	ldr	r3, .L734+12
	orrs	r3, r3, r2
	str	r3, [r4, #2056]
	ldr	r3, [r4, #2060]
	and	r2, r3, #-16777216
	ldr	r3, .L734+16
	orrs	r3, r3, r2
	ldr	r2, [r4, #2064]
	str	r3, [r4, #2060]
	ldr	r3, .L734+20
	ands	r3, r3, r2
	orr	r3, r3, #1572864
	orr	r3, r3, #5120
	str	r3, [r4, #2064]
	ldr	r3, [r4, #2068]
	and	r2, r3, #-16777216
	ldr	r3, .L734+24
	orrs	r3, r3, r2
	str	r3, [r4, #2068]
	ldr	r3, [r7]
	ldr	fp, [r10, #16]
	lsls	r0, r3, #31
	bpl	.L603
	movw	r2, #595
	ldr	r1, .L734+28
	ldr	r0, .L734+32
	bl	printk
.L603:
	ldrb	r3, [fp, #36]	@ zero_extendqisi2
	addw	r0, r4, #2072
	mov	r2, #2720
	and	r3, r3, #3
	cmp	r3, #1
	ite	ls
	ldrls	r1, .L734+36
	ldrhi	r1, .L734+40
	bl	memcpy
	ldr	r3, [r7]
	lsls	r1, r3, #31
	bpl	.L606
	movw	r2, #601
	ldr	r1, .L734+28
	ldr	r0, .L734+44
	bl	printk
.L606:
	ldr	r3, [r5, #392]
	add	r1, r6, #376
	cmp	r3, #2
	addw	r3, r4, #891
	bne	.L607
	add	r6, r6, #312
.L608:
	ldr	r2, [r6], #4
	cmp	r1, r6
	strb	r2, [r3, #1]!
	ldr	r2, [r6, #124]
	and	r2, r2, #63
	strb	r2, [r3, #16]
	bne	.L608
	ldrb	r3, [r4, #1352]	@ zero_extendqisi2
	movs	r2, #11
	add	r0, r4, #1608
	bfi	r3, r2, #0, #5
	strb	r3, [r4, #1352]
	ldr	r3, [r5, #528]
	movs	r2, #208
	cmp	r3, #0
	beq	.L609
.L722:
	ldr	r1, .L734+48
	b	.L717
.L522:
	add	r0, r1, r1, lsr #31
	asrs	r0, r0, #1
	b	.L523
.L632:
	mov	r0, r1
	b	.L523
.L735:
	.align	2
.L734:
	.word	332049
	.word	hal_h265e_debug
	.word	-7829312
	.word	5522204
	.word	1647363
	.word	-16776992
	.word	1574404
	.word	.LANCHOR0+510
	.word	.LC39
	.word	vepu540c_h265_jvt_scl_tab
	.word	vepu540c_h265_customer_scl_tab
	.word	.LC40
	.word	.LANCHOR1+256
.L527:
	ldr	r3, [r10, #16]
	ldr	r0, [r6, #160]
	ldrh	r8, [r3, #4]
	ldrh	r1, [r3, #6]
	lsls	r0, r0, #4
	add	r8, r8, #31
	adds	r1, r1, #31
	asr	r8, r8, #5
	asrs	r1, r1, #5
	mul	r1, r8, r1
	bl	__aeabi_idiv
	ldr	r2, [r6, #172]
	ldrb	ip, [r4, #437]	@ zero_extendqisi2
	bfi	ip, r2, #0, #6
	strb	ip, [r4, #437]
	ldrb	ip, [r4, #630]	@ zero_extendqisi2
	ldr	r2, [r6, #172]
	bfi	ip, r2, #0, #6
	ldrb	r2, [r4, #516]	@ zero_extendqisi2
	strb	ip, [r4, #630]
	bic	r2, r2, #7
	ldrb	ip, [r4, #522]	@ zero_extendqisi2
	orr	r2, r2, #7
	strb	r2, [r4, #516]
	ldr	r2, [r4, #516]
	bfi	r2, r8, #12, #20
	str	r2, [r4, #516]
	ldr	r2, [r5, #392]
	cmp	r0, #1048576
	it	cs
	movcs	r0, #327680
	cmp	r2, #2
	mul	r3, r0, r8
	ite	eq
	ldreq	r2, [r7, #308]
	ldrne	r2, [r7, #304]
	and	r2, r2, #15
	bfi	ip, r2, #0, #4
	strb	ip, [r4, #522]
	lsrs	r3, r3, #4
	ldr	r2, [r5, #528]
	add	r3, r3, r3, lsl #2
	lsrs	r3, r3, #4
	rsbs	r1, r3, #0
	cbz	r2, .L532
	uxtb	r2, ip
	bfc	r2, #0, #4
	strb	r2, [r4, #522]
.L532:
	ldr	r2, [r6, #176]
	ldrh	ip, [r4, #522]
	bfi	ip, r2, #4, #6
	strh	ip, [r4, #522]	@ movhi
	ldr	r2, [r6, #180]
	ubfx	ip, ip, #8, #8
	str	r3, [r4, #840]
	bfi	ip, r2, #2, #6
	ldr	r2, [r4, #524]
	strb	ip, [r4, #523]
	bfi	r2, r0, #0, #20
	str	r2, [r4, #524]
	mvn	r2, #3
	muls	r2, r3, r2
	lsls	r3, r3, #2
	str	r3, [r4, #844]
	mvn	r3, #-2147483648
	strd	r3, r3, [r4, #848]
	strd	r3, r3, [r4, #856]
	str	r3, [r4, #864]
	ldr	r3, [r4, #824]
	strd	r2, r1, [r4, #832]
	and	r2, r3, #-33554432
	ldr	r3, .L736
	orrs	r3, r3, r2
	str	r3, [r4, #824]
	ldr	r3, [r4, #828]
	lsrs	r3, r3, #20
	lsls	r3, r3, #20
	str	r3, [r4, #828]
	ldrb	r3, [r7, #648]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L533
.L694:
	ldrb	r2, [r4, #872]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #872]
	ldrb	r3, [r7, #640]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L535
.L695:
	ldrh	r2, [r4, #872]
	and	r3, r3, #63
	bfi	r2, r3, #6, #6
	strh	r2, [r4, #872]	@ movhi
	ldrb	r3, [r7, #649]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L537
.L696:
	ldr	r2, [r4, #872]
	and	r3, r3, #63
	bfi	r2, r3, #12, #6
	str	r2, [r4, #872]
	ldrb	r3, [r7, #641]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L539
.L697:
	ldrb	r2, [r4, #874]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #2, #6
	strb	r2, [r4, #874]
	ldrb	r3, [r7, #650]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L541
.L698:
	ldrb	r2, [r4, #875]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #875]
	ldrb	r3, [r7, #642]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L543
.L699:
	ldrb	r2, [r4, #876]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #876]
	ldrb	r3, [r7, #651]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L545
.L700:
	ldrh	r2, [r4, #876]
	and	r3, r3, #63
	bfi	r2, r3, #6, #6
	strh	r2, [r4, #876]	@ movhi
	ldrb	r3, [r7, #643]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L547
.L701:
	ldr	r2, [r4, #876]
	and	r3, r3, #63
	bfi	r2, r3, #12, #6
	str	r2, [r4, #876]
	ldrb	r3, [r7, #652]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L549
.L702:
	ldrb	r2, [r4, #878]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #2, #6
	strb	r2, [r4, #878]
	ldrb	r3, [r7, #644]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L551
.L703:
	ldrb	r2, [r4, #879]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #879]
	ldrb	r3, [r7, #653]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L553
.L704:
	ldrb	r2, [r4, #880]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #880]
	ldrb	r3, [r7, #645]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L555
.L705:
	ldrh	r2, [r4, #880]
	and	r3, r3, #63
	bfi	r2, r3, #6, #6
	strh	r2, [r4, #880]	@ movhi
	ldrb	r3, [r7, #654]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L557
.L706:
	ldr	r2, [r4, #880]
	and	r3, r3, #63
	bfi	r2, r3, #12, #6
	str	r2, [r4, #880]
	ldrb	r3, [r7, #646]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L559
.L707:
	ldrb	r2, [r4, #882]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #2, #6
	strb	r2, [r4, #882]
	ldrb	r3, [r7, #655]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L561
.L708:
	ldrb	r2, [r4, #883]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #883]
	ldrb	r3, [r7, #647]	@ zero_extendqisi2
	cbz	r3, .L563
.L709:
	ldrb	r2, [r4, #884]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #884]
	ldrb	r3, [r4, #887]	@ zero_extendqisi2
	ldr	r2, [r7, #656]
	bfi	r3, r2, #6, #2
	strb	r3, [r4, #887]
	b	.L528
.L533:
	ldr	r3, [r6, #180]
	b	.L694
.L535:
	ldr	r3, [r6, #176]
	b	.L695
.L537:
	ldr	r3, [r6, #180]
	b	.L696
.L539:
	ldr	r3, [r6, #176]
	b	.L697
.L541:
	ldr	r3, [r6, #180]
	b	.L698
.L543:
	ldr	r3, [r6, #176]
	b	.L699
.L545:
	ldr	r3, [r6, #180]
	b	.L700
.L547:
	ldr	r3, [r6, #176]
	b	.L701
.L549:
	ldr	r3, [r6, #180]
	b	.L702
.L551:
	ldr	r3, [r6, #176]
	b	.L703
.L553:
	ldr	r3, [r6, #180]
	b	.L704
.L555:
	ldr	r3, [r6, #176]
	b	.L705
.L557:
	ldr	r3, [r6, #180]
	b	.L706
.L559:
	ldr	r3, [r6, #176]
	b	.L707
.L561:
	ldr	r3, [r6, #180]
	b	.L708
.L563:
	ldr	r3, [r6, #176]
	b	.L709
.L567:
	strd	r1, r1, [r4, #380]
	b	.L568
.L573:
	ldr	r3, [sp, #36]
	bl	vepu540c_set_qpmap_normal
	b	.L574
.L576:
	orr	r2, r2, #268435456
	and	r3, r3, #32512
	orr	r2, r2, #1048592
	orr	r3, r3, #16
	str	r2, [r4, #1832]
	strh	r3, [r4, #1836]	@ movhi
	b	.L577
.L578:
	cmp	r1, r7, lsr #7
	bcs	.L580
	subs	r3, r3, #6
	b	.L579
.L580:
	cmp	r1, r7, lsr #6
	ite	cc
	subcc	r3, r3, #4
	addcs	r3, r3, #-1
	b	.L579
.L582:
	ldr	r2, [r5, #396]
	cmp	r2, #2
	beq	.L721
	movs	r2, #2
	b	.L711
.L585:
	ldr	r3, [r0, #8]
	lsls	r7, r3, #31
	bpl	.L586
	orr	r3, r2, #640
	strh	r3, [r4, #1816]	@ movhi
	ldr	r3, [r0, #8]
	ldrb	r2, [r4, #1816]	@ zero_extendqisi2
	asrs	r3, r3, #1
	bfi	r2, r3, #3, #3
	strb	r2, [r4, #1816]
	ldr	r3, [r0, #8]
	ldrh	r2, [r4, #1822]
	asrs	r3, r3, #4
	bfi	r2, r3, #4, #9
	ldrh	r3, [r4, #1826]
	strh	r2, [r4, #1822]	@ movhi
	ldrsh	r2, [r0, #10]
	bfi	r3, r2, #4, #8
	strh	r3, [r4, #1826]	@ movhi
	b	.L586
.L587:
	mov	r3, #269488144
	str	r3, [r4, #1920]
	movs	r3, #16
	strb	r3, [r4, #1924]
	b	.L588
.L593:
	mov	r2, #269488144
	strb	r3, [r4, #1940]
	str	r2, [r4, #1936]
	b	.L594
.L599:
	mov	r2, #286331153
	str	r2, [r4, #2016]
	sub	r2, r2, #131072
	sub	r2, r2, #772
	b	.L714
.L601:
	mov	r2, #286331153
	str	r2, [r4, #2036]
	sub	r2, r2, #131072
	sub	r2, r2, #772
	b	.L715
.L737:
	.align	2
.L736:
	.word	2130942
.L609:
	ldr	r1, .L738
.L717:
	bl	memcpy
	ldrh	r3, [r4, #1144]
	movs	r2, #171
	bfi	r3, r2, #0, #10
	strh	r3, [r4, #1144]	@ movhi
	ldr	r2, [r5, #528]
	cbz	r2, .L613
	ldr	r2, [r5, #532]
	cmp	r2, #0
	beq	.L614
	movs	r2, #85
.L718:
	bfi	r3, r2, #0, #10
	strh	r3, [r4, #1144]	@ movhi
.L613:
	ldr	r3, [r4, #1144]
	movs	r2, #85
	ldrh	r1, [r4, #1200]
	bfi	r3, r2, #10, #10
	str	r3, [r4, #1144]
	ldrh	r3, [r4, #1192]
	ldr	r2, [r4, #1208]
	bic	r3, r3, #32640
	bic	r3, r3, #119
	orr	r3, r3, #15360
	orr	r3, r3, #35
	strh	r3, [r4, #1192]	@ movhi
	ldr	r3, [r4, #1196]
	and	r8, r8, r3
	ldr	r3, .L738+4
	orr	r3, r8, r3
	str	r3, [r4, #1196]
	movs	r3, #16
	bfi	r1, r3, #0, #12
	ldr	r3, .L738+8
	strh	r1, [r4, #1200]	@ movhi
	str	r3, [r4, #1204]
	ldr	r3, .L738+12
	ands	r3, r3, r2
	ldr	r2, [r4, #1212]
	orr	r3, r3, #33
	str	r3, [r4, #1208]
	ldr	r3, [r4, #1216]
	and	r2, r2, #-268374016
	orr	r0, r2, #1048576
	and	r2, r3, #-1073741824
	ldr	r3, .L738+16
	orr	r0, r0, #8
	str	r0, [r4, #1212]
	orrs	r3, r3, r2
	ldr	r2, [r4, #1220]
	str	r3, [r4, #1216]
	and	r6, r2, #-16777216
	ldr	r2, .L738+20
	orrs	r2, r2, r6
	str	r2, [r4, #1220]
	ldr	r2, [r4, #924]
	and	r6, r2, #-16777216
	ldr	r2, .L738+24
	orrs	r2, r2, r6
	str	r2, [r4, #924]
	ldr	r2, [r4, #928]
	movs	r6, #240
	and	r2, r2, #-268374016
	orr	r2, r2, #9437184
	orr	r2, r2, #64
	str	r2, [r4, #928]
	ldrh	r2, [r4, #932]
	bfi	r2, r6, #0, #12
	strh	r2, [r4, #932]	@ movhi
	ldr	r2, [r5, #444]
	movw	r6, #4708
	ldr	r2, [r2, r6]
	cmp	r2, #1
	beq	.L615
	bic	r3, r3, #1069547520
	bfc	r1, #0, #12
	bic	r3, r3, #4177920
	mov	r2, #67372036
	bic	r3, r3, #15360
	and	r0, r0, #-268374016
	orr	r3, r3, #4194304
	strh	r1, [r4, #1200]	@ movhi
	orr	r3, r3, #4096
	str	r2, [r4, #1204]
	str	r0, [r4, #1212]
.L719:
	str	r3, [r4, #1216]
.L616:
	ldr	r0, [r10, #52]
	bl	mpp_frame_get_is_gray
	ldr	r3, [r5, #640]
	cmp	r3, r0
	beq	.L617
	cbnz	r3, .L619
	ldrb	r2, [r4, #1837]	@ zero_extendqisi2
	str	r3, [r4, #1988]
	bfi	r2, r3, #7, #1
	str	r3, [r4, #1976]
	ldrb	r3, [r4, #437]	@ zero_extendqisi2
	strb	r2, [r4, #1837]
	and	r2, r3, #63
	cmp	r2, #29
	bhi	.L620
	movs	r2, #30
.L720:
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #437]
	ldrb	r3, [r4, #630]	@ zero_extendqisi2
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #630]
.L619:
	str	r0, [r5, #640]
.L617:
	ldr	r3, [sp, #32]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsls	r2, r3, #29
	bpl	.L623
	ldr	r0, [r5, #460]
	cmp	r0, #0
	bne	.L624
	ldr	r1, [r5, #444]
	ldr	r2, [r1, #12]
	adds	r2, r2, #31
	bic	r3, r2, #31
	ldr	r2, [r1, #16]
	add	r1, r5, #460
	adds	r2, r2, #31
	bic	r2, r2, #31
	muls	r2, r3, r2
	ldr	r3, .L738+28
	str	r3, [sp]
	ldr	r3, .L738+32
	add	r2, r2, r2, lsl #1
	asrs	r2, r2, #1
	bl	mpp_buffer_get_with_tag
	ldr	r3, [r5, #460]
	cmp	r3, #0
	bne	.L624
	movw	r1, #1884
	ldr	r0, .L738+36
	bl	printk
.L623:
	ldr	r3, [sp, #32]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsls	r3, r3, #28
	bpl	.L625
	ldr	r1, [r5, #484]
	ldrb	r2, [r4, #48]	@ zero_extendqisi2
	ldrh	r3, [r9, #4]
	ldr	r1, [r1, #16]
	adds	r3, r3, #31
	bfi	r2, r1, #4, #4
	strb	r2, [r4, #48]
	ldrb	r2, [r4, #460]	@ zero_extendqisi2
	bic	r3, r3, #31
	and	r2, r2, #3
	lsls	r6, r3, #1
	orn	r2, r2, #39
	strb	r2, [r4, #460]
	ldr	r2, [r4, #488]
	bfi	r2, r3, #0, #17
	add	r3, r3, r6
	str	r2, [r4, #488]
	movs	r2, #160
	strh	r3, [r4, #492]	@ movhi
	ldr	r1, [r5, #460]
	ldr	r0, [r5, #64]
	bl	mpp_dev_get_iova_address
	movs	r3, #0
	str	r3, [r4, #316]
	add	r6, r6, r0
	str	r0, [r4, #308]
	str	r6, [r4, #312]
.L625:
	ldr	r0, [r7]
	ldr	r3, [r5, #500]
	ands	r0, r0, #4
	add	r3, r3, #1
	str	r3, [r5, #500]
	beq	.L498
	mov	r3, #2224
	ldr	r1, .L738+40
	mov	r2, r3
	ldr	r0, .L738+44
	bl	printk
	movs	r0, #0
	b	.L498
.L607:
	add	r6, r6, #440
.L611:
	ldr	r2, [r1], #4
	cmp	r6, r1
	strb	r2, [r3, #1]!
	ldr	r2, [r1, #124]
	and	r2, r2, #63
	strb	r2, [r3, #16]
	bne	.L611
	ldrb	r3, [r4, #1352]	@ zero_extendqisi2
	movs	r2, #11
	add	r0, r4, #1608
	bfi	r3, r2, #0, #5
	strb	r3, [r4, #1352]
	ldr	r2, [r5, #444]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	movs	r2, #208
	cmp	r3, #1
	beq	.L722
	ldr	r1, .L738+48
	b	.L717
.L614:
	movs	r2, #128
	b	.L718
.L615:
	ldr	r2, [r5, #528]
	cmp	r2, #0
	beq	.L616
	bic	r3, r3, #1069547520
	ldr	r2, .L738+52
	bic	r3, r3, #4177920
	bic	r3, r3, #15360
	orr	r3, r3, #8388608
	str	r2, [r4, #1204]
	orr	r3, r3, #6144
	b	.L719
.L620:
	cmp	r2, #31
	bhi	.L621
	movs	r2, #32
	b	.L720
.L621:
	cmp	r2, #33
	bhi	.L619
	movs	r2, #34
	b	.L720
.L624:
	ldrb	r3, [r4, #436]	@ zero_extendqisi2
	movs	r2, #163
	orr	r3, r3, #4
	strb	r3, [r4, #436]
	movs	r3, #0
	str	r3, [r4, #320]
	ldr	r1, [r5, #460]
	ldr	r0, [r5, #64]
	bl	mpp_dev_get_iova_address
	ldrb	r3, [r4, #439]	@ zero_extendqisi2
	orr	r3, r3, #128
	strb	r3, [r4, #439]
	str	r0, [r4, #324]
	b	.L623
.L739:
	.align	2
.L738:
	.word	.LANCHOR1+464
	.word	33574920
	.word	336595976
	.word	-268370040
	.word	12591108
	.word	8413248
	.word	1313797
	.word	.LANCHOR0+538
	.word	.LC24
	.word	.LC41
	.word	.LANCHOR0+485
	.word	.LC2
	.word	.LANCHOR1+672
	.word	201852420
	.fnend
	.size	hal_h265e_v540c_gen_regs, .-hal_h265e_v540c_gen_regs
	.global	hal_h265e_vepu540c
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.36911, %object
	.size	__func__.36911, 21
__func__.36911:
	.ascii	"hal_h265e_v540c_init\000"
	.type	__FUNCTION__.36912, %object
	.size	__FUNCTION__.36912, 21
__FUNCTION__.36912:
	.ascii	"hal_h265e_v540c_init\000"
	.type	__func__.37156, %object
	.size	__func__.37156, 27
__func__.37156:
	.ascii	"vepu540c_h265_set_feedback\000"
	.type	__func__.37202, %object
	.size	__func__.37202, 30
__func__.37202:
	.ascii	"hal_h265e_v540c_ret_comb_task\000"
	.type	__func__.37165, %object
	.size	__func__.37165, 21
__func__.37165:
	.ascii	"hal_h265e_v540c_wait\000"
	.type	__func__.37114, %object
	.size	__func__.37114, 22
__func__.37114:
	.ascii	"hal_h265e_v540c_start\000"
	.type	__func__.37190, %object
	.size	__func__.37190, 27
__func__.37190:
	.ascii	"hal_h265e_v540c_comb_start\000"
	.type	__func__.37172, %object
	.size	__func__.37172, 25
__func__.37172:
	.ascii	"hal_h265e_v540c_get_task\000"
	.type	__func__.36845, %object
	.size	__func__.36845, 29
__func__.36845:
	.ascii	"vepu540c_h265_setup_hal_bufs\000"
	.type	__FUNCTION__.36861, %object
	.size	__FUNCTION__.36861, 29
__FUNCTION__.36861:
	.ascii	"vepu540c_h265_setup_hal_bufs\000"
	.type	__FUNCTION__.36792, %object
	.size	__FUNCTION__.36792, 13
__FUNCTION__.36792:
	.ascii	"get_wrap_buf\000"
	.type	__func__.36932, %object
	.size	__func__.36932, 27
__func__.36932:
	.ascii	"hal_h265e_vepu540c_prepare\000"
	.type	__func__.37181, %object
	.size	__func__.37181, 25
__func__.37181:
	.ascii	"hal_h265e_v540c_ret_task\000"
	.type	__func__.36922, %object
	.size	__func__.36922, 23
__func__.36922:
	.ascii	"hal_h265e_v540c_deinit\000"
	.type	__FUNCTION__.36923, %object
	.size	__FUNCTION__.36923, 23
__FUNCTION__.36923:
	.ascii	"hal_h265e_v540c_deinit\000"
	.type	__func__.37045, %object
	.size	__func__.37045, 29
__func__.37045:
	.ascii	"vepu540c_h265_set_hw_address\000"
	.type	__func__.36962, %object
	.size	__func__.36962, 25
__func__.36962:
	.ascii	"vepu540c_h265_uv_address\000"
	.type	__func__.36817, %object
	.size	__func__.36817, 21
__func__.36817:
	.ascii	"setup_recn_refr_wrap\000"
	.space	2
	.type	ref_type_map, %object
	.size	ref_type_map, 16
ref_type_map:
	.word	0
	.word	1
	.word	2
	.word	3
	.type	__FUNCTION__.37046, %object
	.size	__FUNCTION__.37046, 29
__FUNCTION__.37046:
	.ascii	"vepu540c_h265_set_hw_address\000"
	.type	__func__.37098, %object
	.size	__func__.37098, 25
__func__.37098:
	.ascii	"hal_h265e_v540c_gen_regs\000"
	.type	__func__.36869, %object
	.size	__func__.36869, 28
__func__.36869:
	.ascii	"setup_vepu540c_hevc_scl_cfg\000"
	.type	__FUNCTION__.37056, %object
	.size	__FUNCTION__.37056, 32
__FUNCTION__.37056:
	.ascii	"vepu540c_h265e_save_pass1_patch\000"
	.space	2
	.type	hal_h265e_vepu540c, %object
	.size	hal_h265e_vepu540c, 64
hal_h265e_vepu540c:
	.word	.LC24
	.word	16777220
	.word	656
	.word	0
	.word	hal_h265e_v540c_init
	.word	hal_h265e_v540c_deinit
	.word	hal_h265e_vepu540c_prepare
	.word	hal_h265e_v540c_get_task
	.word	hal_h265e_v540c_gen_regs
	.word	hal_h265e_v540c_start
	.word	hal_h265e_v540c_wait
	.word	0
	.word	0
	.word	hal_h265e_v540c_ret_task
	.word	hal_h265e_v540c_comb_start
	.word	hal_h265e_v540c_ret_comb_task
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	aq_qp_dealt_smart, %object
	.size	aq_qp_dealt_smart, 64
aq_qp_dealt_smart:
	.word	-8
	.word	-7
	.word	-6
	.word	-5
	.word	-4
	.word	-3
	.word	-2
	.word	-1
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	6
	.word	7
	.word	9
	.type	aq_thd_smart, %object
	.size	aq_thd_smart, 64
aq_thd_smart:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	3
	.word	3
	.word	5
	.word	5
	.word	8
	.word	8
	.word	8
	.word	15
	.word	15
	.word	20
	.word	25
	.word	25
	.type	aq_qp_dealt_default, %object
	.size	aq_qp_dealt_default, 64
aq_qp_dealt_default:
	.word	-8
	.word	-7
	.word	-6
	.word	-5
	.word	-4
	.word	-3
	.word	-2
	.word	-1
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.word	8
	.type	aq_thd_default, %object
	.size	aq_thd_default, 64
aq_thd_default:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	3
	.word	3
	.word	5
	.word	5
	.word	8
	.word	8
	.word	8
	.word	15
	.word	15
	.word	20
	.word	25
	.word	25
	.type	lamd_modb_qp, %object
	.size	lamd_modb_qp, 208
lamd_modb_qp:
	.word	112
	.word	137
	.word	176
	.word	224
	.word	274
	.word	352
	.word	448
	.word	548
	.word	704
	.word	896
	.word	1096
	.word	1408
	.word	1792
	.word	2192
	.word	2816
	.word	3584
	.word	4384
	.word	5632
	.word	7168
	.word	8768
	.word	11264
	.word	14336
	.word	17536
	.word	22528
	.word	28672
	.word	35072
	.word	45056
	.word	57344
	.word	70144
	.word	90112
	.word	114688
	.word	140288
	.word	180224
	.word	229376
	.word	280576
	.word	360448
	.word	458752
	.word	561152
	.word	720896
	.word	917504
	.word	1122304
	.word	1441792
	.word	1835008
	.word	2244608
	.word	2883584
	.word	3670016
	.word	4489216
	.word	5767168
	.word	7340032
	.word	8978432
	.word	11534336
	.word	14680064
	.type	lamd_moda_qp, %object
	.size	lamd_moda_qp, 208
lamd_moda_qp:
	.word	73
	.word	92
	.word	116
	.word	146
	.word	184
	.word	232
	.word	292
	.word	368
	.word	463
	.word	584
	.word	735
	.word	927
	.word	1167
	.word	1471
	.word	1853
	.word	2335
	.word	2942
	.word	3706
	.word	4669
	.word	5883
	.word	7412
	.word	9339
	.word	11766
	.word	14825
	.word	18678
	.word	23533
	.word	29649
	.word	37356
	.word	47065
	.word	59298
	.word	74711
	.word	94130
	.word	118596
	.word	149422
	.word	188260
	.word	237193
	.word	298844
	.word	376520
	.word	474386
	.word	597688
	.word	753040
	.word	948771
	.word	1195377
	.word	1506080
	.word	1897542
	.word	2390753
	.word	3012160
	.word	3795084
	.word	4781507
	.word	6024321
	.word	7590169
	.word	9563013
	.type	lamd_modb_qp_cvr, %object
	.size	lamd_modb_qp_cvr, 208
lamd_modb_qp_cvr:
	.word	112
	.word	136
	.word	176
	.word	216
	.word	272
	.word	344
	.word	432
	.word	552
	.word	688
	.word	872
	.word	1096
	.word	1384
	.word	1744
	.word	2192
	.word	2760
	.word	3480
	.word	4384
	.word	5528
	.word	6960
	.word	8776
	.word	11056
	.word	13928
	.word	17544
	.word	22104
	.word	27856
	.word	40456
	.word	50976
	.word	64224
	.word	80920
	.word	101952
	.word	128448
	.word	161840
	.word	203904
	.word	290976
	.word	366608
	.word	461904
	.word	581960
	.word	733224
	.word	923800
	.word	1300232
	.word	1638192
	.word	2063992
	.word	2600472
	.word	3619880
	.word	4560760
	.word	5746200
	.word	7926736
	.word	9987064
	.word	12582912
	.word	15853472
	.word	19974128
	.word	25165824
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: (%d) enter\012\000"
.LC1:
	.ascii	"\0013%s:%d: mpp_dev_init failed. ret: %d\012\000"
.LC2:
	.ascii	"\0016%s:%d: (%d) leave\012\000"
.LC3:
	.ascii	"\0013%s:%d: RKV_ENC_INT_LINKTABLE_FINISH\000"
.LC4:
	.ascii	"\0016%s:%d: RKV_ENC_INT_ONE_FRAME_FINISH\000"
.LC5:
	.ascii	"\0013%s:%d: RKV_ENC_INT_ONE_SLICE_FINISH\000"
.LC6:
	.ascii	"\0013%s:%d: RKV_ENC_INT_SAFE_CLEAR_FINISH\000"
.LC7:
	.ascii	"\0013%s:%d: RKV_ENC_INT_BIT_STREAM_OVERFLOW\000"
.LC8:
	.ascii	"\0013%s:%d: RKV_ENC_INT_BUS_WRITE_FULL\000"
.LC9:
	.ascii	"\0013%s:%d: RKV_ENC_INT_BUS_WRITE_ERROR\000"
.LC10:
	.ascii	"\0013%s:%d: RKV_ENC_INT_BUS_READ_ERROR\000"
.LC11:
	.ascii	"\0013%s:%d: RKV_ENC_INT_TIMEOUT_ERROR\000"
.LC12:
	.ascii	"\0016%s:%d: output stream size %d\012\000"
.LC13:
	.ascii	"\0013%s:%d: enc_task->flags.err %08x, return early\000"
.LC14:
	.ascii	"\0013%s:%d: poll cmd failed %d status %d \012\000"
.LC15:
	.ascii	"\0013%s:%d: enc_task->flags.err %08x, return e arly"
	.ascii	"\000"
.LC16:
	.ascii	"\0013%s:%d: set register write failed %d\012\000"
.LC17:
	.ascii	"\0016%s:%d: ctl reg[%04x]: 0%08x\012\000"
.LC18:
	.ascii	"\0016%s:%d: hw add cfg reg[%04x]: 0%08x\012\000"
.LC19:
	.ascii	"\0016%s:%d: set reg[%04x]: 0%08x\012\000"
.LC20:
	.ascii	"\0013%s:%d: set register read failed %d\012\000"
.LC21:
	.ascii	"\0013%s:%d: send cmd failed %d\012\000"
.LC22:
	.ascii	"\0013%s:%d: invalid src color space: %d\012\000"
.LC23:
	.ascii	"\0013%s:%d: vepu541_h265_allocate_buffers failed, f"
	.ascii	"ree buffers and return\012\000"
.LC24:
	.ascii	"hal_h265e_v540c\000"
.LC25:
	.ascii	"\0016%s:%d: frame size %d -> %d max count %d -> %d\012"
	.ascii	"\000"
.LC26:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC27:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC28:
	.ascii	"\0013%s:%d: unknown color space: %d\012\000"
.LC29:
	.ascii	"\0016%s:%d: cur is idr  lt %d\012\000"
.LC30:
	.ascii	"\0016%s:%d: ref type %d\012\000"
.LC31:
	.ascii	"\0013%d: WARNING: not support lt ref to st when buf"
	.ascii	" is wrap\000"
.LC32:
	.ascii	"\0013%d: WARNING: not support lt ref to lt when buf"
	.ascii	" is wrap\000"
.LC33:
	.ascii	"\0016%s:%d: cur_is_ref %d\012\000"
.LC34:
	.ascii	"\0016%s:%d: hdr[size %d top %d bot %d cur %d pre %d"
	.ascii	"]\012\000"
.LC35:
	.ascii	"\0016%s:%d: bdy [size %d top %d bot %d cur %d pre %"
	.ascii	"d]\012\000"
.LC36:
	.ascii	"\0016%s:%d: frame %d | type %d | start gen regs\000"
.LC37:
	.ascii	"\0013%d: cur frame already overflow [%d %d]!\012\000"
.LC38:
	.ascii	"\0013%d: online case set full frame err\000"
.LC39:
	.ascii	"\0016%s:%d: enter\012\000"
.LC40:
	.ascii	"\0016%s:%d: leave\012\000"
.LC41:
	.ascii	"\0013%d: buf_pass1 malloc fail, debreath invaild\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
