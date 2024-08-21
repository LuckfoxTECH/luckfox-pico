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
	lsls	r0, r3, #29
	bpl	.L2
	movw	r3, #1154
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
	str	r0, [r4, #792]
	add	r0, r7, #32
	ldr	r3, [r7, #4]
	str	r3, [r4, #752]
	ldr	r3, [r7, #8]
	str	r3, [r4, #824]
	ldr	r3, [r7, #12]
	str	r3, [r4, #828]
	ldr	r3, [r7, #36]
	str	r3, [r4, #940]
	ldr	r3, [r7, #16]
	str	r3, [r4, #832]
	ldr	r3, [r7, #20]
	str	r3, [r4, #836]
	ldr	r3, [r7, #24]
	str	r3, [r4, #956]
	movs	r3, #0
	str	r3, [r4, #732]
	str	r3, [r4, #724]
	movs	r3, #1
	str	r3, [r4, #772]
	str	r1, [r7, #28]
	bl	mpp_dev_init
	mov	r5, r0
	cbz	r0, .L3
	mov	r3, r0
	ldr	r1, .L30+4
	ldr	r0, .L30+16
	movw	r2, #1177
.L29:
	bl	printk
.L1:
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, pc}
.L3:
	ldr	r2, [r7, #32]
	movs	r1, #171
	ldr	r3, [r4, #68]
	str	r0, [r4, #744]
	add	r3, r3, #5152
	str	r2, [r4, #64]
	adds	r3, r3, #24
	strd	r3, r2, [r4, #736]
	movs	r2, #2
	str	r2, [r4, #728]
	ldr	r3, [r7, #4]
	strd	r2, r2, [r3, #308]
	movs	r2, #85
	strd	r0, r0, [r3, #324]
	strd	r1, r2, [r3, #316]
	add	r2, r3, #464
	str	r0, [r3, #332]
	ldr	r1, [r4, #836]
	cmp	r1, #0
	beq	.L5
	ldr	r7, .L30+20
	add	ip, r7, #64
.L6:
	mov	r4, r7
	adds	r2, r2, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, ip
	str	r0, [r2, #-8]	@ unaligned
	mov	r7, r4
	str	r1, [r2, #-4]	@ unaligned
	bne	.L6
	ldr	r7, .L30+24
	add	r4, r3, #528
	add	ip, r7, #64
.L7:
	mov	r2, r7
	adds	r4, r4, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, ip
	str	r0, [r4, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r4, #-4]	@ unaligned
	bne	.L7
	ldr	r7, .L30+28
	add	r4, r3, #336
	add	ip, r7, #64
.L8:
	mov	r2, r7
	adds	r4, r4, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, ip
	str	r0, [r4, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r4, #-4]	@ unaligned
	bne	.L8
	ldr	r4, .L30+32
	add	r3, r3, #400
	add	r7, r4, #64
.L9:
	mov	r2, r4
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, r7
	str	r0, [r3, #-8]	@ unaligned
	mov	r4, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L9
.L10:
	ldr	r3, [r6]
	lsls	r3, r3, #29
	bpl	.L1
	movw	r3, #1212
	ldr	r1, .L30+4
	mov	r2, r3
	ldr	r0, .L30+36
	b	.L29
.L5:
	ldr	r7, .L30+40
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
	ldr	r7, .L30+40
	add	r4, r3, #528
.L12:
	mov	r2, r7
	adds	r4, r4, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r4, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r4, #-4]	@ unaligned
	bne	.L12
	ldr	r7, .L30+44
	add	r4, r3, #336
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
	ldr	r4, .L30+44
	add	r3, r3, #400
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
	.word	.LC2
	.word	.LANCHOR0+21
	.word	.LC3
	.word	.LANCHOR1
	.word	.LANCHOR1+64
	.word	.LANCHOR1+128
	.word	.LANCHOR1+192
	.word	.LC4
	.word	.LANCHOR1+256
	.word	.LANCHOR1+320
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
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #68
	sub	sp, sp, #68
	ldr	r3, [r0, #752]
	mov	r4, r0
	ldr	fp, [r0, #68]
	ldr	r5, [r4, #72]
	ldr	r2, [r3, #12]
	ldr	r3, [r3, #16]
	adds	r0, r2, #15
	str	r1, [sp, #28]
	it	mi
	addmi	r0, r2, #30
	ldr	r8, [r1, #4]
	str	r3, [sp, #4]
	ldr	r3, [sp, #4]
	asrs	r0, r0, #4
	ldrh	r1, [r5, #198]
	adds	r3, r3, #15
	str	r2, [sp]
	ldr	r9, [r5]
	itt	mi
	ldrmi	r3, [sp, #4]
	addmi	r3, r3, #30
	asrs	r3, r3, #4
	muls	r0, r3, r0
	ldrh	r3, [r5, #196]
	str	r3, [sp, #32]
	ldrh	r3, [r5, #204]
	str	r3, [sp, #36]
	ldrh	r3, [r5, #212]
	str	r3, [sp, #40]
	ldrh	r3, [r5, #220]
	str	r3, [sp, #44]
	ldrh	r3, [r5, #206]
	add	r1, r1, r3
	ldrh	r3, [r5, #214]
	add	r1, r1, r3
	ldrh	r3, [r5, #222]
	adds	r3, r1, r3
	ldrh	r1, [r5, #200]
	str	r3, [sp, #16]
	ldrh	r3, [r5, #208]
	add	r1, r1, r3
	ldrh	r3, [r5, #216]
	add	r1, r1, r3
	ldrh	r3, [r5, #224]
	adds	r3, r1, r3
	ldrh	r1, [r5, #202]
	str	r3, [sp, #20]
	ldrh	r3, [r5, #210]
	ldrh	r7, [r5, #230]
	add	r1, r1, r3
	ldrh	r3, [r5, #218]
	ldrh	r10, [r5, #232]
	add	r1, r1, r3
	ldrh	r3, [r5, #226]
	adds	r3, r1, r3
	str	r3, [sp, #24]
	ldrh	r3, [r5, #228]
	ldrh	r1, [r5, #254]
	str	r3, [sp, #48]
	ldrh	r3, [r5, #236]
	str	r3, [sp, #52]
	ldrh	r3, [r5, #244]
	str	r3, [sp, #56]
	ldrh	r3, [r5, #252]
	str	r3, [sp, #60]
	ldrh	r3, [r5, #238]
	add	r7, r7, r3
	ldrh	r3, [r5, #246]
	add	r7, r7, r3
	adds	r3, r7, r1
	str	r3, [sp, #8]
	ldrh	r3, [r5, #240]
	ldrh	r1, [r5, #256]
	add	r10, r10, r3
	ldrh	r3, [r5, #248]
	add	r10, r10, r3
	add	r3, r10, r1
	ldrh	r10, [r5, #234]
	str	r3, [sp, #12]
	ldrh	r3, [r5, #242]
	ldrh	r1, [r5, #250]
	add	r10, r10, r3
	ldr	r3, [r4, #836]
	add	r10, r10, r1
	ldrh	r1, [r5, #258]
	add	r10, r10, r1
	cmp	r3, #0
	bne	.L35
	ldr	r3, [sp, #8]
	movs	r2, #22
	add	r1, r3, r3, lsl #4
	ldr	r3, [sp, #12]
	mla	r3, r2, r3, r1
	movs	r1, #24
	mla	r1, r1, r10, r3
.L126:
	ldr	r3, [sp, #20]
	ldr	r2, [sp, #24]
	movs	r6, #6
	lsrs	r1, r1, #2
	add	r3, r3, r3, lsl #2
	mla	r3, r6, r2, r3
	ldr	r2, [sp, #16]
	rsb	r6, r0, r0, lsl #4
	add	r3, r3, r2, lsl #2
	lsrs	r2, r3, #2
	movs	r3, #100
	muls	r1, r3, r1
	cmp	r1, r6
	bls	.L37
	movs	r3, #200
.L127:
	str	r3, [r8, #204]
	movs	r3, #100
	muls	r3, r2, r3
	movs	r2, #30
	muls	r2, r0, r2
	cmp	r3, r2
	bhi	.L82
	movs	r2, #13
	muls	r0, r2, r0
	cmp	r3, r0
	ite	ls
	movls	r3, #0
	movhi	r3, #1
.L41:
	str	r3, [r8, #208]
	ldr	r3, .L133
	ldr	r3, [r3]
	lsls	r0, r3, #29
	bpl	.L42
	movw	r3, #2535
	ldr	r1, .L133+4
	mov	r2, r3
	ldr	r0, .L133+8
	bl	printk
.L42:
	ldr	r2, [r5, #16]
	ldr	r3, [r4, #88]
	add	r3, r3, r2
	str	r3, [r4, #88]
	ldr	r3, [r4, #92]
	ldr	r2, [r5, #4]
	add	r3, r3, r2
	str	r3, [r4, #92]
	ldr	r3, [r5, #12]
	str	r9, [r4, #80]
	lsls	r1, r3, #16
	ldrd	r2, [r4, #104]
	adds	r6, r2, r1
	adc	r7, r3, #0
	tst	r9, #2
	mov	r2, r6
	mov	r3, r7
	strd	r2, [r4, #104]
	beq	.L43
	ldr	r3, .L133
	ldr	r3, [r3]
	lsls	r1, r3, #28
	bpl	.L43
	movw	r2, #2546
	ldr	r1, .L133+4
	ldr	r0, .L133+12
	bl	printk
.L43:
	tst	r9, #1
	beq	.L44
	ldr	r3, .L133
	ldr	r3, [r3]
	lsls	r2, r3, #28
	bpl	.L44
	movw	r2, #2549
	ldr	r1, .L133+4
	ldr	r0, .L133+16
	bl	printk
.L44:
	tst	r9, #8
	beq	.L45
	ldr	r3, .L133
	ldr	r3, [r3]
	lsls	r3, r3, #28
	bpl	.L45
	movw	r2, #2552
	ldr	r1, .L133+4
	ldr	r0, .L133+20
	bl	printk
.L45:
	tst	r9, #4
	beq	.L46
	ldr	r3, .L133
	ldr	r3, [r3]
	lsls	r7, r3, #28
	bpl	.L46
	movw	r2, #2555
	ldr	r1, .L133+4
	ldr	r0, .L133+24
	bl	printk
.L46:
	ldr	r3, .L133
	tst	r9, #16
	ldr	r6, [r3]
	beq	.L47
	lsls	r6, r6, #22
	bpl	.L83
	movw	r2, #2558
	ldr	r1, .L133+4
	ldr	r0, .L133+28
	bl	printk
.L83:
	mvn	r0, #100
	b	.L32
.L35:
	ldr	r3, [sp, #12]
	movs	r1, #11
	muls	r3, r1, r3
	movs	r1, #12
	mla	r1, r1, r10, r3
	ldr	r3, [sp, #8]
	add	r1, r1, r3, lsl #3
	b	.L126
.L37:
	add	r6, r0, r0, lsl #2
	cmp	r1, r6
	bhi	.L127
	cmp	r1, r0, asr #2
	ite	ls
	movls	r3, #0
	movhi	r3, #1
	b	.L127
.L82:
	movs	r3, #2
	b	.L41
.L47:
	tst	r9, #32
	beq	.L49
	lsls	r5, r6, #22
	bpl	.L87
	ldr	r1, .L133+4
	movw	r2, #2563
	ldr	r0, .L133+32
.L132:
	bl	printk
.L87:
	mov	r0, #-1
	b	.L32
.L49:
	tst	r9, #64
	beq	.L50
	lsls	r4, r6, #22
	bpl	.L87
	movw	r2, #2568
	ldr	r1, .L133+4
	ldr	r0, .L133+36
	b	.L132
.L50:
	tst	r9, #128
	beq	.L51
	lsls	r0, r6, #22
	bpl	.L87
	movw	r2, #2573
	ldr	r1, .L133+4
	ldr	r0, .L133+40
	b	.L132
.L51:
	tst	r9, #256
	beq	.L52
	lsls	r1, r6, #22
	bpl	.L87
	movw	r2, #2578
	ldr	r1, .L133+4
	ldr	r0, .L133+44
	b	.L132
.L52:
	tst	r9, #65536
	beq	.L53
	lsls	r2, r6, #22
	bpl	.L88
	movw	r2, #2583
	ldr	r1, .L133+4
	ldr	r0, .L133+48
	bl	printk
.L88:
	mvn	r0, #101
	b	.L32
.L134:
	.align	2
.L133:
	.word	hal_h265e_debug
	.word	.LANCHOR0+42
	.word	.LC2
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
.L53:
	ldrd	r3, r2, [sp, #40]
	ldrb	ip, [fp, #925]	@ zero_extendqisi2
	add	r3, r3, r2
	ldr	r0, [sp, #16]
	mov	r1, r3
	ldr	r3, [sp, #36]
	add	r1, r1, r3
	ldr	r3, [sp, #32]
	add	r3, r3, r1
	ldrb	r1, [fp, #926]	@ zero_extendqisi2
	mov	r2, r3
	ldrb	r3, [fp, #924]	@ zero_extendqisi2
	add	lr, r3, ip
	mul	lr, r0, lr
	ldr	r0, [sp, #24]
	muls	r0, r1, r0
	mla	r0, r2, r3, r0
	add	r3, r1, ip
	ldr	r1, [sp, #20]
	add	r0, r0, lr, lsr #1
	muls	r3, r1, r3
	add	r0, r0, r3, lsr #1
	ldr	r3, [sp, #16]
	adds	r1, r3, r1
	ldr	r3, [sp, #24]
	add	r1, r1, r3
	adds	r1, r1, r2
	bne	.L54
.L128:
	ldrd	r3, r2, [sp, #56]
	str	r0, [r4, #356]
	add	r3, r3, r2
	ldrh	ip, [fp, #930]
	ldrh	r2, [fp, #932]
	mov	r1, r3
	ldr	r3, [sp, #52]
	ubfx	ip, ip, #0, #12
	ldr	r7, [sp, #8]
	add	r1, r1, r3
	ldr	r3, [sp, #48]
	ubfx	r2, r2, #0, #12
	add	r1, r1, r3
	ldrh	r3, [fp, #928]
	ubfx	r3, r3, #0, #12
	add	r0, r3, ip
	mul	lr, r7, r0
	mul	r0, r10, r2
	mla	r0, r1, r3, r0
	add	r3, r2, ip
	ldr	r2, [sp, #12]
	add	r0, r0, lr, lsr #1
	muls	r3, r2, r3
	add	r0, r0, r3, lsr #1
	mov	r3, r2
	mov	r2, r7
	add	r2, r2, r3
	add	r10, r10, r2
	adds	r1, r10, r1
	bne	.L56
.L129:
	str	r0, [r4, #352]
	add	r1, r5, #388
	ldr	r3, [r5, #164]
	add	r0, r4, #144
	ubfx	r2, r3, #0, #23
	ldr	r3, [r4, #360]
	add	r3, r3, r2
	str	r3, [r4, #360]
	ldr	r3, [r5, #132]
	ubfx	r2, r3, #0, #17
	ldr	r3, [r4, #112]
	add	r3, r3, r2
	str	r3, [r4, #112]
	ldr	r3, [r5, #136]
	ubfx	r2, r3, #0, #19
	ldr	r3, [r4, #116]
	add	r3, r3, r2
	str	r3, [r4, #116]
	ldr	r3, [r5, #148]
	ubfx	r2, r3, #0, #19
	ldr	r3, [r4, #128]
	add	r3, r3, r2
	str	r3, [r4, #128]
	ldr	r3, [r5, #140]
	ubfx	r2, r3, #0, #21
	ldr	r3, [r4, #120]
	add	r3, r3, r2
	str	r3, [r4, #120]
	ldr	r3, [r5, #152]
	ubfx	r2, r3, #0, #21
	ldr	r3, [r4, #132]
	add	r3, r3, r2
	str	r3, [r4, #132]
	ldr	r3, [r5, #144]
	ubfx	r2, r3, #0, #23
	ldr	r3, [r4, #124]
	add	r3, r3, r2
	str	r3, [r4, #124]
	ldr	r3, [r5, #156]
	ubfx	r2, r3, #0, #23
	ldr	r3, [r4, #136]
	add	r3, r3, r2
	str	r3, [r4, #136]
	ldr	r3, [r5, #160]
	ubfx	r2, r3, #0, #23
	ldr	r3, [r4, #140]
	add	r3, r3, r2
	movs	r2, #208
	str	r3, [r4, #140]
	bl	memcpy
	ldrb	r1, [r5, #168]	@ zero_extendqisi2
	movs	r3, #4
	smulbb	r2, r1, r3
	str	r2, [r4, #368]
	ldrb	r0, [r5, #169]	@ zero_extendqisi2
	lsls	r0, r0, #2
	str	r0, [r4, #372]
	ldrb	r2, [r5, #170]	@ zero_extendqisi2
	smlabb	r1, r3, r1, r0
	smulbb	r7, r2, r3
	smlabb	r2, r3, r2, r1
	str	r7, [r4, #376]
	ldrb	r5, [r5, #171]	@ zero_extendqisi2
	smulbb	r7, r5, r3
	smlabb	r3, r3, r5, r2
	ldr	r2, [r4, #92]
	str	r7, [r4, #380]
	str	r3, [r4, #384]
	ldr	r3, [sp, #28]
	ldr	r3, [r3, #4]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsrs	r3, r3, #3
	and	r3, r3, #2
	str	r3, [r4, #84]
	ldr	r3, [r8, #184]
	add	r3, r3, r2, lsl #3
	str	r3, [r8, #184]
	ldr	r2, [r4, #708]	@ unaligned
	ldrh	r3, [r4, #712]	@ unaligned
	str	r2, [r4, #388]	@ unaligned
	ldrsb	r0, [r4, #391]
	ldrsb	r7, [r4, #390]
	ldrsb	r1, [r4, #389]
	ldrsb	ip, [r4, #388]
	strh	r3, [r4, #392]	@ unaligned
	strb	r7, [r4, #391]
	strb	r0, [r4, #392]
	strb	r1, [r4, #390]
	strb	ip, [r4, #389]
	ldr	r2, [r8, #160]
	ldr	r3, [r8, #184]
	cmp	r2, r3
	ble	.L58
	movs	r5, #6
	muls	r5, r3, r5
	it	mi
	addmi	r5, r5, #3
	cmp	r2, r5, asr #2
	ble	.L59
	movs	r3, #3
.L130:
	strb	r3, [r4, #388]
	add	r1, r1, ip
	ldrsb	r3, [r4, #388]
	add	r1, r1, r7
	add	r1, r1, r3
	ldr	r3, [sp, #28]
	add	r1, r1, r0
	ldr	r3, [r3, #4]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsls	r3, r3, #27
	itt	mi
	movmi	r3, #0
	strbmi	r3, [r4, #393]
	cmn	r1, #9
	bge	.L74
	movs	r3, #2
.L131:
	strb	r3, [r4, #393]
.L75:
	ldr	r1, [r4, #360]
	cbz	r1, .L76
	ldr	r0, [r4, #356]
	bl	__aeabi_uidiv
	mov	r1, r0
.L76:
	ldr	r0, [r4, #364]
	str	r1, [r4, #356]
	cbz	r0, .L77
	mov	r1, r0
	ldr	r0, [r4, #352]
	bl	__aeabi_uidiv
.L77:
	ldr	r3, [sp]
	str	r0, [r4, #352]
	adds	r5, r3, #63
	ldr	r3, [sp, #4]
	it	mi
	addmi	r5, r5, #63
	adds	r3, r3, #63
	asr	r5, r5, #6
	itt	mi
	ldrmi	r3, [sp, #4]
	addmi	r3, r3, #126
	asrs	r3, r3, #6
	muls	r5, r3, r5
	lsls	r1, r5, #8
	cmp	r1, #0
	ble	.L80
	ldrd	r3, r2, [r4, #128]
	lsls	r0, r3, #6
	add	r0, r0, r2, lsl #4
	ldrd	r2, r3, [r4, #136]
	add	r3, r3, r2
	add	r0, r0, r3, lsl #2
	lsls	r0, r0, #8
	bl	__aeabi_uidiv
	str	r0, [r8, #200]
.L80:
	cmp	r5, #0
	ble	.L81
	ldr	r0, [r4, #88]
	lsls	r1, r5, #6
	bl	__aeabi_uidiv
	str	r0, [r8, #188]
.L81:
	ldr	r3, [r4, #356]
	ands	r0, r6, #4
	str	r3, [r8, #192]
	ldr	r3, [r4, #352]
	str	r3, [r8, #196]
	beq	.L32
	movw	r3, #2698
	ldr	r1, .L135
	mov	r2, r3
	ldr	r0, .L135+4
	bl	printk
	movs	r0, #0
.L32:
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L54:
	bl	__aeabi_uidiv
	b	.L128
.L56:
	bl	__aeabi_uidiv
	b	.L129
.L59:
	adds	r5, r3, r3, lsl #2
	it	mi
	addmi	r5, r5, #3
	cmp	r2, r5, asr #2
	ble	.L62
	movs	r3, #2
	b	.L130
.L62:
	adds	r3, r3, r3, lsl #3
	it	mi
	addmi	r3, r3, #7
	cmp	r2, r3, asr #3
	ite	gt
	movgt	r3, #1
	movle	r3, #0
	b	.L130
.L58:
	cmp	r3, r2, lsl #1
	ble	.L66
	movs	r3, #251
	b	.L130
.L66:
	rsbs	r5, r2, r2, lsl #3
	it	mi
	addmi	r5, r5, #3
	cmp	r3, r5, asr #2
	ble	.L67
	movs	r3, #252
	b	.L130
.L67:
	movs	r5, #6
	muls	r5, r2, r5
	it	mi
	addmi	r5, r5, #3
	cmp	r3, r5, asr #2
	ble	.L69
	movs	r3, #253
	b	.L130
.L69:
	adds	r2, r2, r2, lsl #2
	it	mi
	addmi	r2, r2, #3
	cmp	r3, r2, asr #2
	ite	gt
	movgt	r3, #254
	movle	r3, #255
	b	.L130
.L74:
	adds	r1, r1, #2
	bge	.L75
	ldrsb	r3, [r4, #393]
	cmp	r3, #1
	bgt	.L75
	movs	r3, #1
	b	.L131
.L136:
	.align	2
.L135:
	.word	.LANCHOR0+42
	.word	.LC4
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
	ldr	r4, .L156
	mov	r7, r1
	ldr	r10, [r0, #72]
	mov	r6, r2
	ldr	r9, [r2, #4]
	ldr	r3, [r4]
	lsls	r0, r3, #29
	bpl	.L138
	movw	r3, #2872
	ldr	r1, .L156+4
	mov	r2, r3
	ldr	r0, .L156+8
	bl	printk
.L138:
	mov	r1, r7
	mov	r0, r5
	bl	vepu540c_h265_set_feedback
	mov	r8, r0
	cmp	r0, #0
	bne	.L137
	ldr	r3, [r5, #92]
	str	r3, [r7, #44]
	ldr	r3, [r7, #48]
	ldr	r2, [r5, #92]
	add	r3, r3, r2
	str	r3, [r7, #48]
	ldr	r3, [r10]
	lsls	r1, r3, #18
	itt	mi
	movmi	r3, #1
	strmi	r3, [r6, #96]
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
	bpl	.L142
	ldr	r3, [r5, #92]
	movw	r2, #2887
	ldr	r1, .L156+4
	ldr	r0, .L156+12
	bl	printk
.L142:
	ldr	r3, [r4]
	lsls	r3, r3, #29
	bpl	.L137
	movw	r3, #2889
	ldr	r1, .L156+4
	mov	r2, r3
	ldr	r0, .L156+16
	bl	printk
.L137:
	mov	r0, r8
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L157:
	.align	2
.L156:
	.word	hal_h265e_debug
	.word	.LANCHOR0+69
	.word	.LC2
	.word	.LC15
	.word	.LC4
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
	ldr	r5, .L172
	mov	r4, r1
	ldr	r2, [r5]
	lsls	r2, r2, #29
	bpl	.L159
	movw	r3, #2711
	ldr	r1, .L172+4
	mov	r2, r3
	ldr	r0, .L172+8
	bl	printk
.L159:
	ldr	r3, [r4, #80]
	cbz	r3, .L160
	movw	r2, #2715
	ldr	r1, .L172+4
	ldr	r0, .L172+12
	mov	r4, #-1
	bl	printk
.L158:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L160:
	mov	r2, r3
	movs	r1, #6
	ldr	r0, [r6, #64]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L162
	ldr	r3, [r6, #72]
	movw	r2, #2777
	ldr	r1, .L172+4
	str	r3, [sp]
	mov	r3, r0
	ldr	r0, .L172+16
	bl	printk
.L162:
	ldr	r3, [r5]
	lsls	r3, r3, #29
	bpl	.L158
	movw	r3, #2779
	ldr	r1, .L172+4
	mov	r2, r3
	ldr	r0, .L172+20
	bl	printk
	b	.L158
.L173:
	.align	2
.L172:
	.word	hal_h265e_debug
	.word	.LANCHOR0+99
	.word	.LC2
	.word	.LC16
	.word	.LC17
	.word	.LC4
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
	ldr	r6, .L246
	.pad #36
	sub	sp, sp, #36
	mov	r7, r0
	ldrd	r5, r9, [r0, #68]
	ldr	r3, [r6]
	lsls	r1, r3, #29
	bpl	.L175
	movw	r3, #2310
	ldr	r1, .L246+4
	mov	r2, r3
	ldr	r0, .L246+8
	bl	printk
.L175:
	ldr	r8, [r4, #80]
	cmp	r8, #0
	beq	.L176
	mov	r3, r8
	movw	r2, #2314
	ldr	r1, .L246+4
	mov	r4, #-1
	ldr	r0, .L246+12
	bl	printk
.L174:
	mov	r0, r4
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L176:
	add	r2, sp, #8
	mov	r1, r8
	ldr	r0, [r7, #64]
	mov	r3, #292
	str	r5, [sp, #8]
	strd	r3, r8, [sp, #12]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L178
	mov	r3, r0
	movw	r2, #2324
.L244:
	ldr	r1, .L246+4
	ldr	r0, .L246+16
.L243:
	bl	printk
	b	.L174
.L178:
	ldr	r3, [r6]
	lsls	r2, r3, #26
	bpl	.L179
	ldr	r4, .L246+4
	ldr	r10, .L246+32
.L181:
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L180
	ldr	r3, [r5, r8, lsl #2]
	movw	r2, #2332
	mov	r1, r4
	mov	r0, r10
	str	r3, [sp]
	lsl	r3, r8, #2
	bl	printk
.L180:
	add	r8, r8, #1
	cmp	r8, #73
	bne	.L181
.L179:
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
	cbz	r0, .L182
	mov	r3, r0
	movw	r2, #2342
	b	.L244
.L182:
	ldr	r3, [r6]
	lsls	r1, r3, #27
	bpl	.L183
	ldr	r10, .L246+4
	movs	r4, #32
	ldr	fp, .L246+36
	mvn	r8, #3
.L185:
	ldr	r3, [r6]
	lsls	r2, r3, #27
	bpl	.L184
	mul	r3, r8, r4
	add	r2, r5, #420
	mov	r1, r10
	mov	r0, fp
	ldr	r2, [r2, r3]
	adds	r3, r3, #128
	str	r2, [sp]
	movw	r2, #2350
	bl	printk
.L184:
	subs	r4, r4, #1
	bne	.L185
	ldr	r10, .L246+4
	movs	r4, #101
	ldr	fp, .L246+40
	mvn	r8, #3
.L187:
	ldr	r3, [r6]
	lsls	r3, r3, #27
	bpl	.L186
	mul	r3, r8, r4
	add	r2, r5, #824
	mov	r1, r10
	mov	r0, fp
	ldr	r2, [r2, r3]
	add	r3, r3, #404
	str	r2, [sp]
	movw	r2, #2355
	bl	printk
.L186:
	subs	r4, r4, #1
	bne	.L187
.L183:
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
	cbz	r0, .L188
	mov	r3, r0
	movw	r2, #2364
	b	.L244
.L188:
	ldr	r3, [r6]
	lsls	r4, r3, #25
	bpl	.L189
	ldr	r10, .L246+4
	movs	r4, #68
	ldr	fp, .L246+40
	mvn	r8, #3
.L191:
	ldr	r3, [r6]
	lsls	r0, r3, #25
	bpl	.L190
	mul	r3, r8, r4
	add	r2, r5, #1096
	mov	r1, r10
	mov	r0, fp
	ldr	r2, [r2, r3]
	add	r3, r3, #272
	str	r2, [sp]
	movw	r2, #2372
	bl	printk
.L190:
	subs	r4, r4, #1
	bne	.L191
.L189:
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
	cbz	r0, .L192
	mov	r3, r0
	movw	r2, #2382
	b	.L244
.L192:
	ldr	r3, [r6]
	lsls	r2, r3, #24
	bpl	.L193
	ldr	r10, .L246+4
	movs	r4, #180
	ldr	fp, .L246+40
	mvn	r8, #3
.L195:
	ldr	r3, [r6]
	lsls	r3, r3, #24
	bpl	.L194
	mul	r3, r8, r4
	add	r2, r5, #1816
	mov	r1, r10
	mov	r0, fp
	ldr	r2, [r2, r3]
	add	r3, r3, #720
	str	r2, [sp]
	movw	r2, #2390
	bl	printk
.L194:
	subs	r4, r4, #1
	bne	.L195
.L193:
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
	cbz	r0, .L196
	mov	r3, r0
	mov	r2, #2400
	b	.L244
.L196:
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
	cbz	r0, .L197
	mov	r3, r0
	movw	r2, #2410
	b	.L244
.L197:
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
	cbz	r0, .L198
	mov	r3, r0
	movw	r2, #2420
	b	.L244
.L198:
	movs	r1, #4
	movs	r3, #44
	add	r2, sp, #20
	strd	r1, r3, [sp, #24]
	ldr	r0, [r7, #64]
	movs	r1, #1
	str	r9, [sp, #20]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L199
	mov	r3, r0
	movw	r2, #2430
.L245:
	ldr	r1, .L246+4
	ldr	r0, .L246+20
	b	.L243
.L199:
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
	cbz	r0, .L200
	mov	r3, r0
	movw	r2, #2440
	b	.L245
.L200:
	mov	r2, r0
	movs	r1, #5
	ldr	r0, [r7, #64]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L201
	mov	r3, r0
	movw	r2, #2446
	ldr	r1, .L246+4
	ldr	r0, .L246+24
	bl	printk
.L201:
	ldr	r3, [r6]
	lsls	r3, r3, #29
	bpl	.L174
	movw	r3, #2447
	ldr	r1, .L246+4
	mov	r2, r3
	ldr	r0, .L246+28
	b	.L243
.L247:
	.align	2
.L246:
	.word	hal_h265e_debug
	.word	.LANCHOR0+120
	.word	.LC2
	.word	.LC18
	.word	.LC19
	.word	.LC23
	.word	.LC24
	.word	.LC4
	.word	.LC20
	.word	.LC21
	.word	.LC22
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
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r8, r2
	ldr	r6, .L265
	.pad #36
	sub	sp, sp, #36
	mov	r4, r0
	mov	r9, r1
	ldr	r5, [r0, #68]
	ldr	r3, [r6]
	lsls	r2, r3, #29
	bpl	.L249
	movw	r3, #2842
	ldr	r1, .L265+4
	mov	r2, r3
	ldr	r0, .L265+8
	bl	printk
.L249:
	ldrb	r3, [r5, #48]	@ zero_extendqisi2
	movs	r2, #7
	add	r7, r5, #4768
	add	r0, sp, #4
	bfi	r3, r2, #0, #4
	strb	r3, [r5, #48]
	ldr	r3, [r4, #64]
	adds	r7, r7, #24
	strd	r7, r8, [sp, #12]
	str	r3, [sp, #4]
	add	r3, r5, #692
	str	r3, [sp, #8]
	ldr	r3, [r4, #792]
	str	r3, [sp, #20]
	ldr	r3, [r4, #824]
	str	r3, [sp, #24]
	bl	vepu540c_set_jpeg_reg
	ldr	r1, [r8, #92]
	cbz	r1, .L250
	mov	r2, #384
	mov	r0, r7
	bl	memcpy
.L250:
	ldr	r1, [r8, #88]
	cbz	r1, .L251
	add	r0, r5, #5472
	mov	r2, #312
	adds	r0, r0, #16
	bl	memcpy
.L251:
	ldr	r3, [r6]
	lsls	r3, r3, #29
	bpl	.L252
	movw	r3, #2857
	ldr	r1, .L265+4
	mov	r2, r3
	ldr	r0, .L265+12
	bl	printk
.L252:
	mov	r1, r9
	mov	r0, r4
	bl	hal_h265e_v540c_start
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L266:
	.align	2
.L265:
	.word	hal_h265e_debug
	.word	.LANCHOR0+142
	.word	.LC2
	.word	.LC4
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
	mov	r4, r0
	ldr	r5, .L352
	.pad #52
	sub	sp, sp, #52
	mov	r8, r1
	ldr	r6, [r1, #4]
	ldr	r3, [r5]
	lsls	r7, r3, #29
	bpl	.L268
	movw	r3, #2789
	ldr	r1, .L352+4
	mov	r2, r3
	ldr	r0, .L352+8
	bl	printk
.L268:
	ldr	r0, [r8, #52]
	bl	mpp_frame_get_roi
	str	r0, [r4, #748]
	ldr	r0, [r8, #52]
	bl	mpp_frame_get_osd
	str	r0, [r4, #744]
	ldrb	r3, [r6, #152]	@ zero_extendqisi2
	lsrs	r3, r3, #3
	and	r3, r3, #2
	str	r3, [r4, #728]
	ldr	r3, [r8, #4]
	ldrb	r3, [r3, #156]	@ zero_extendqisi2
	lsls	r6, r3, #29
	bmi	.L269
	ldr	r7, [r4, #752]
	movw	r3, #4728
	ldr	fp, [r4, #792]
	ldr	r9, [r4, #784]
	ldr	r10, [r7, r3]
	ldr	r3, [r5]
	ldr	r6, [r7, #28]
	lsls	r0, r3, #29
	add	r6, r6, #63
	bic	r6, r6, #63
	bpl	.L270
	movw	r3, #473
	ldr	r1, .L352+12
	mov	r2, r3
	ldr	r0, .L352+8
	bl	printk
.L270:
	ldr	r3, [r7, #28]
	mov	r0, fp
	adds	r2, r3, #63
	it	mi
	addmi	r2, r3, #126
	adds	r3, r3, #15
	bic	r3, r3, #15
	asrs	r2, r2, #6
	str	r2, [sp, #16]
	ldr	r2, [r7, #32]
	adds	r1, r2, #63
	it	mi
	addmi	r1, r2, #126
	adds	r2, r2, #15
	bic	r2, r2, #15
	asrs	r1, r1, #6
	str	r1, [sp, #20]
	muls	r2, r3, r2
	ldr	r3, [r4, #752]
	ldr	r1, [r3, #36]
	str	r2, [sp, #28]
	bl	vepu541_set_fmt
	ldr	r3, [fp]
	ldr	r2, [sp, #28]
	cmp	r3, #13
	bhi	.L273
	tbb	[pc, r3]
.L275:
	.byte	(.L278-.L275)/2
	.byte	(.L274-.L275)/2
	.byte	(.L276-.L275)/2
	.byte	(.L273-.L275)/2
	.byte	(.L276-.L275)/2
	.byte	(.L276-.L275)/2
	.byte	(.L277-.L275)/2
	.byte	(.L277-.L275)/2
	.byte	(.L276-.L275)/2
	.byte	(.L276-.L275)/2
	.byte	(.L273-.L275)/2
	.byte	(.L273-.L275)/2
	.byte	(.L274-.L275)/2
	.byte	(.L274-.L275)/2
	.p2align 1
.L277:
	add	r8, r2, r2, lsl #1
	lsr	r8, r8, #1
.L279:
	cmp	r10, #0
	beq	.L312
	mov	r0, r10
	bl	mpp_enc_ref_cfg_get_cpb_info
	ldr	r3, [r0]
	add	r10, r3, #1
	ldr	r3, [r0, #4]
	cmp	r10, #2
	it	lt
	movlt	r10, #2
	str	r3, [sp, #24]
.L281:
	ldrd	r3, r1, [r7, #28]
	addw	r7, r3, #511
	add	r0, r1, #31
	addw	r1, r1, #511
	adds	r3, r3, #31
	asrs	r2, r7, #9
	bic	r3, r3, #31
	bic	r7, r0, #31
	asrs	r1, r1, #9
	muls	r7, r2, r7
	it	mi
	addmi	r7, r7, #31
	muls	r3, r1, r3
	it	mi
	addmi	r3, r3, #31
	asrs	r7, r7, #5
	asrs	r3, r3, #5
	lsls	r2, r7, #4
	lsls	r7, r3, #4
	cmp	r7, r2
	it	lt
	movlt	r7, r2
	cmp	r6, #3072
	ble	.L284
	asrs	r6, r6, #5
	mov	r3, #416
	muls	r6, r3, r6
	ldr	r3, [r4, #940]
	ldr	fp, [r3, #8]
	sub	r6, r6, #37632
	subs	r6, r6, #224
	cmp	fp, #0
	bne	.L285
	ldrd	r3, r0, [r4, #948]
	cmp	r6, r3
	beq	.L286
	ldr	r1, .L352+16
	bl	mpp_buffer_put_with_caller
	str	fp, [r4, #952]
.L287:
	ldr	r3, .L352+16
	mov	r2, r6
	add	r1, r4, #952
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L352+20
	bl	mpp_buffer_get_with_tag
	b	.L288
.L276:
	lsl	r8, r2, #1
	b	.L279
.L274:
	add	r8, r2, r2, lsl #1
	b	.L279
.L278:
	lsl	r8, r2, #2
	b	.L279
.L273:
	movw	r2, #509
	ldr	r1, .L352+12
	ldr	r0, .L352+24
	bl	printk
	movw	r2, #2798
	ldr	r1, .L352+4
	ldr	r0, .L352+28
	bl	printk
	ldr	r3, [r8, #80]
	mvn	r0, #3
	orr	r3, r3, #16
	str	r3, [r8, #80]
.L267:
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L312:
	mov	r10, #2
	b	.L281
.L353:
	.align	2
.L352:
	.word	hal_h265e_debug
	.word	.LANCHOR0+169
	.word	.LC2
	.word	.LANCHOR0+194
	.word	.LANCHOR0+223
	.word	.LC27
	.word	.LC25
	.word	.LC26
.L286:
	cmp	r0, #0
	beq	.L287
.L288:
	str	r6, [r4, #948]
.L289:
	ldr	r3, [r4, #776]
	cmp	r3, r8
	bcc	.L291
	cmp	r9, r10
	blt	.L291
	ldr	r3, [r4, #780]
	cmp	r3, r7
	bcs	.L292
.L291:
	ldr	r3, [r4, #940]
	ldr	r3, [r3]
	cbnz	r3, .L293
	ldr	r0, [r4, #812]
	cbz	r0, .L294
	bl	hal_bufs_deinit
.L294:
	add	r0, r4, #812
	bl	hal_bufs_init
.L293:
	ldrd	r3, r1, [sp, #16]
	cmp	r10, r9
	ldr	r2, [r4, #828]
	mov	r6, r10
	ldr	r0, [r4, #752]
	it	lt
	movlt	r6, r9
	muls	r1, r3, r1
	lsl	ip, r1, #8
	cmp	r2, #0
	beq	.L295
	movs	r3, #0
	str	ip, [sp, #32]
	strd	r3, r3, [sp, #36]
	str	r3, [sp, #44]
	ldr	r3, [r0, #620]
	cbz	r3, .L296
	lsls	r3, r1, #2
	adds	r3, r3, #255
	bic	r3, r3, #255
	lsls	r3, r3, #4
	str	r3, [sp, #36]
.L296:
	ldr	r3, [r5]
	str	r7, [sp, #40]
	lsls	r1, r3, #28
	bpl	.L297
	strd	r9, r6, [sp, #4]
	movw	r2, #578
	str	r8, [sp]
	ldr	r3, [r4, #776]
	ldr	r1, .L354
	ldr	r0, .L354+4
	bl	printk
.L297:
	ldr	r3, [r4, #752]
	movs	r0, #48
	movs	r1, #240
	ldr	r2, [r3, #28]
	ldr	r3, [r3, #32]
	adds	r2, r2, #63
	adds	r3, r3, #63
	bic	r2, r2, #63
	bic	r3, r3, #63
	cmp	r2, r3
	mul	ip, r3, r2
	it	lt
	movlt	r2, r3
	muls	r0, r2, r0
	muls	r1, r2, r1
	add	r3, ip, ip, lsl #1
	addw	r0, r0, #4095
	cmp	ip, #0
	add	r0, r0, r3, asr #1
	ldr	r3, .L354+8
	it	lt
	addlt	ip, ip, #63
	addw	r1, r1, #4095
	ands	r0, r0, r3
	ands	r1, r1, r3
	mov	lr, r3
	asrs	r3, r2, #1
	add	r2, r2, r2, lsl #1
	add	ip, r3, ip, asr #6
	addw	r3, r2, #4095
	ldr	r2, [sp, #24]
	addw	ip, ip, #4095
	and	r3, r3, lr
	and	ip, ip, lr
	cmp	r2, #0
	add	r1, r1, r0
	add	r3, r3, ip
	str	r0, [r4, #932]
	add	r9, r1, r3
	str	r1, [r4, #936]
	str	ip, [r4, #884]
	str	r3, [r4, #888]
	ble	.L299
	ldr	r2, [r4, #956]
	strd	r0, r1, [r4, #908]
	strd	ip, r3, [r4, #860]
	cbz	r2, .L300
	str	r0, [r4, #912]
	str	ip, [r4, #864]
.L300:
	ldr	r0, [r4, #864]
	ldr	r2, [r4, #912]
	add	r3, r3, r0
	str	r3, [r4, #892]
	add	ip, r2, r0
	add	r2, r2, r3
	add	r9, r9, ip
	add	r1, r1, r2
	mov	ip, #0
	str	r0, [r4, #868]
	strd	ip, r0, [r4, #844]
	str	ip, [r4, #852]
	strd	r3, r0, [r4, #872]
	strd	r2, r3, [r4, #896]
	str	r2, [r4, #916]
	strd	r1, r2, [r4, #920]
.L311:
	ldr	r3, [r4, #940]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne	.L301
	ldr	r0, [r4, #840]
	cbz	r0, .L302
	ldr	r1, .L354+12
	bl	mpp_buffer_put_with_caller
.L302:
	ldr	r3, .L354+12
	mov	r2, r9
	add	r1, r4, #840
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L354+16
	bl	mpp_buffer_get_with_tag
.L307:
	ldr	r3, [r4, #940]
	ldr	r3, [r3]
	cbnz	r3, .L305
	add	r3, sp, #32
	movs	r2, #4
	mov	r1, r6
	ldr	r0, [r4, #812]
	bl	hal_bufs_setup
.L305:
	ldr	r3, [r4, #940]
	ldr	r3, [r3]
	cbz	r3, .L309
	str	r3, [r4, #812]
.L309:
	str	r8, [r4, #776]
	strd	r7, r6, [r4, #780]
.L292:
	ldr	r3, [r5]
	lsls	r3, r3, #29
	bpl	.L310
	movw	r3, #611
	ldr	r1, .L354
	mov	r2, r3
	ldr	r0, .L354+20
	bl	printk
.L310:
	mov	r2, #320
	add	r1, r4, #80
	add	r0, r4, #400
	bl	memcpy
.L269:
	mov	r2, #320
	movs	r1, #0
	add	r0, r4, #80
	bl	memset
	ldr	r0, [r5]
	ands	r0, r0, #4
	beq	.L267
	movw	r3, #2808
	ldr	r1, .L354+24
	mov	r2, r3
	ldr	r0, .L354+20
	bl	printk
	movs	r0, #0
	b	.L267
.L285:
	str	fp, [r4, #952]
	b	.L288
.L284:
	ldr	r0, [r4, #952]
	cbz	r0, .L290
	ldr	r3, [r4, #940]
	ldr	r6, [r3, #8]
	cbnz	r6, .L290
	ldr	r1, .L354+28
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #952]
.L290:
	movs	r3, #0
	str	r3, [r4, #948]
	b	.L289
.L301:
	str	r3, [r4, #840]
	b	.L307
.L295:
	movw	r3, #8191
	add	r3, r3, r1, lsl #6
	strd	r2, r2, [sp, #36]
	str	r2, [sp, #44]
	bic	r2, r3, #8160
	bic	r2, r2, #31
	str	ip, [sp, #32]
	str	r2, [r4, #816]
	ldr	r3, [r0, #620]
	cbz	r3, .L306
	lsls	r3, r1, #2
	adds	r3, r3, #255
	bic	r3, r3, #255
	lsls	r3, r3, #4
	str	r3, [sp, #36]
.L306:
	lsls	r3, r1, #13
	str	r7, [sp, #40]
	add	r3, r3, r1, lsl #12
	add	r3, r2, r3, asr #1
	str	r3, [sp, #44]
	ldr	r3, [r5]
	lsls	r2, r3, #28
	bpl	.L307
	strd	r9, r6, [sp, #4]
	movw	r2, #598
	str	r8, [sp]
	ldr	r3, [r4, #776]
	ldr	r1, .L354
	ldr	r0, .L354+4
	bl	printk
	b	.L307
.L299:
	movs	r2, #0
	strd	r3, r9, [r4, #916]
	strd	r2, r3, [r4, #868]
	str	r2, [r4, #876]
	str	r3, [r4, #924]
	b	.L311
.L355:
	.align	2
.L354:
	.word	.LANCHOR0+194
	.word	.LC28
	.word	-4096
	.word	.LANCHOR0+252
	.word	.LC27
	.word	.LC4
	.word	.LANCHOR0+169
	.word	.LANCHOR0+223
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
	ldr	r4, .L369
	ldr	r6, [r0, #752]
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L357
	mov	r3, r0
	movw	r2, #1271
	ldr	r1, .L369+4
	ldr	r0, .L369+8
	bl	printk
.L357:
	ldr	r3, [r6, #8]
	tst	r3, #5
	itt	ne
	movne	r3, #0
	strne	r3, [r6, #8]
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L359
	mov	r3, r5
	movw	r2, #1285
	ldr	r1, .L369+4
	ldr	r0, .L369+12
	bl	printk
.L359:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L370:
	.align	2
.L369:
	.word	hal_h265e_debug
	.word	.LANCHOR0+265
	.word	.LC29
	.word	.LC30
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
	ldr	r4, .L384
	mov	r5, r0
	ldr	r3, [r4]
	lsls	r1, r3, #29
	bpl	.L372
	movw	r3, #2819
	ldr	r1, .L384+4
	mov	r2, r3
	ldr	r0, .L384+8
	bl	printk
.L372:
	mov	r1, r6
	mov	r0, r5
	bl	vepu540c_h265_set_feedback
	mov	r7, r0
	cbnz	r0, .L371
	ldr	r3, [r5, #92]
	str	r3, [r6, #44]
	ldr	r3, [r6, #48]
	ldr	r2, [r5, #92]
	add	r3, r3, r2
	str	r3, [r6, #48]
	ldr	r3, [r4]
	lsls	r2, r3, #28
	bpl	.L374
	ldr	r3, [r5, #92]
	movw	r2, #2828
	ldr	r1, .L384+4
	ldr	r0, .L384+12
	bl	printk
.L374:
	ldr	r3, [r4]
	lsls	r3, r3, #29
	bpl	.L371
	movw	r3, #2830
	ldr	r1, .L384+4
	mov	r2, r3
	ldr	r0, .L384+16
	bl	printk
.L371:
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, pc}
.L385:
	.align	2
.L384:
	.word	hal_h265e_debug
	.word	.LANCHOR0+292
	.word	.LC2
	.word	.LC15
	.word	.LC4
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
	ldr	r5, .L439
	ldr	r3, [r5]
	lsls	r2, r3, #29
	bpl	.L387
	movw	r3, #1221
	ldr	r1, .L439+4
	mov	r2, r3
	ldr	r0, .L439+8
	bl	printk
.L387:
	ldr	r0, [r4, #64]
	cbz	r0, .L388
	bl	mpp_dev_deinit
	movs	r3, #0
	str	r3, [r4, #64]
.L388:
	ldr	r1, [r4, #68]
	cbz	r1, .L389
	ldr	r0, .L439+12
	bl	mpp_osal_free
.L389:
	ldr	r1, [r4, #72]
	movs	r6, #0
	str	r6, [r4, #68]
	cbz	r1, .L390
	ldr	r0, .L439+12
	bl	mpp_osal_free
.L390:
	ldr	r1, [r4, #76]
	str	r6, [r4, #72]
	cbz	r1, .L391
	ldr	r0, .L439+12
	bl	mpp_osal_free
.L391:
	ldr	r1, [r4, #792]
	movs	r6, #0
	str	r6, [r4, #76]
	cbz	r1, .L392
	ldr	r0, .L439+12
	bl	mpp_osal_free
.L392:
	ldr	r3, [r4, #940]
	str	r6, [r4, #792]
	ldr	r6, [r3, #8]
	cbnz	r6, .L393
	ldr	r0, [r4, #952]
	cbz	r0, .L393
	ldr	r1, .L439+12
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #952]
.L393:
	ldr	r3, [r4, #940]
	ldr	r3, [r3]
	cbnz	r3, .L394
	ldr	r0, [r4, #812]
	cbz	r0, .L394
	bl	hal_bufs_deinit
.L394:
	ldr	r0, [r4, #760]
	cbz	r0, .L395
	ldr	r1, .L439+12
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #760]
.L395:
	ldr	r0, [r4, #764]
	cbz	r0, .L396
	ldr	r1, .L439+12
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #764]
.L396:
	ldr	r3, [r4, #940]
	ldr	r6, [r3, #4]
	cbnz	r6, .L397
	ldr	r0, [r4, #840]
	cbz	r0, .L397
	ldr	r1, .L439+12
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #840]
.L397:
	ldr	r0, [r4, #768]
	cbz	r0, .L398
	ldr	r1, .L439+12
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #768]
.L398:
	ldr	r3, [r5]
	lsls	r3, r3, #29
	bpl	.L399
	movw	r3, #1262
	ldr	r1, .L439+4
	mov	r2, r3
	ldr	r0, .L439+16
	bl	printk
.L399:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L440:
	.align	2
.L439:
	.word	hal_h265e_debug
	.word	.LANCHOR0+317
	.word	.LC2
	.word	.LANCHOR0+340
	.word	.LC4
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
	ldr	r7, [r0, #792]
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
	ldr	r3, .L529
	ldr	r2, [r3]
	tst	r2, #4
	str	r3, [sp, #64]
	beq	.L442
	movw	r3, #1850
	ldr	r1, .L529+4
	mov	r2, r3
	ldr	r0, .L529+8
	bl	printk
.L442:
	ldr	r3, [r4, #824]
	orrs	r3, r8, r3
	bne	.L443
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
	cbnz	r7, .L445
	ldrh	r7, [r3, #6]
.L445:
	ldr	r0, [r6, #52]
	bl	mpp_frame_get_fmt
	ands	r0, r0, #15728640
	beq	.L446
	ldr	r0, [r6, #52]
	bl	mpp_frame_get_fbc_offset
	mov	r7, r0
	movs	r0, #0
.L447:
	ldr	r3, [r5, #20]
	add	r7, r7, r3
	ldr	r3, [r5, #24]
	str	r7, [r5, #20]
	add	r0, r0, r3
	str	r0, [r5, #24]
.L443:
	ldr	r3, [sp, #20]
	ldr	r0, [r4, #812]
	ldrb	r1, [r3, #232]	@ zero_extendqisi2
	bl	hal_bufs_get_buf
	ldr	r3, [sp, #20]
	str	r0, [sp, #56]
	ldr	r0, [r4, #812]
	ldrb	r1, [r3, #234]	@ zero_extendqisi2
	bl	hal_bufs_get_buf
	ldr	r3, [r4, #828]
	mov	r8, r0
	cmp	r3, #0
	beq	.L454
	ldr	r3, [r6, #16]
	ldr	r1, [r4, #840]
	ldr	r0, [r4, #64]
	ldr	r2, [r3, #292]
	ldrb	r10, [r3, #233]	@ zero_extendqisi2
	ldrb	r9, [r3, #235]	@ zero_extendqisi2
	str	r2, [sp, #60]
	movs	r2, #163
	str	r3, [sp, #68]
	bl	mpp_dev_get_iova_address
	ldr	r2, [r4, #728]
	cmp	r2, #2
	mov	r7, r0
	ldr	r3, [sp, #68]
	bne	.L455
	ldrb	r2, [r3, #232]	@ zero_extendqisi2
	ldrb	r3, [r3, #234]	@ zero_extendqisi2
	cmp	r2, r3
	bne	.L455
	ldr	r3, .L529
	ldr	r3, [r3]
	lsls	r3, r3, #23
	bpl	.L456
	mov	r3, r10
	mov	r2, #328
	ldr	r1, .L529+12
	ldr	r0, .L529+16
	bl	printk
.L456:
	ldr	r3, [r4, #868]
	str	r3, [sp, #28]
	ldr	r3, [r4, #872]
	str	r3, [sp, #24]
	ldr	r3, [r4, #916]
	str	r3, [sp, #36]
	ldr	r3, [r4, #920]
	str	r3, [sp, #32]
	cmp	r10, #0
	beq	.L457
	ldr	r3, [r4, #852]
	ldr	r2, [sp, #28]
	ldr	fp, [r4, #900]
	str	r3, [sp, #16]
	ldr	r3, [r4, #844]
	str	fp, [sp, #48]
	cmp	r2, r3
	it	cs
	movcs	r2, r3
	ldr	r3, [r4, #848]
	str	r2, [sp, #28]
	ldr	r2, [sp, #24]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [r4, #892]
	str	r2, [sp, #24]
	ldr	r2, [sp, #36]
	cmp	r2, r3
	it	cs
	movcs	r2, r3
	ldr	r3, [r4, #896]
	str	r2, [sp, #36]
	ldr	r2, [sp, #32]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [sp, #16]
	str	r2, [sp, #32]
.L526:
	str	r3, [sp, #44]
.L458:
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
	ldr	r7, .L529
	lsrs	r3, r3, #5
	bfi	r2, r3, #2, #1
	strb	r2, [r5, #144]
	ldr	r2, [r7]
	lsls	r1, r2, #23
	bpl	.L490
	movw	r2, #437
	ldr	r1, .L529+12
	ldr	r0, .L529+20
	bl	printk
	ldr	r3, [r7]
	lsls	r2, r3, #23
	bpl	.L490
	ldr	r3, [r4, #880]
	movw	r2, #439
	ldr	r1, .L529+12
	ldr	r0, .L529+24
	str	r3, [sp, #12]
	ldr	r3, [r4, #876]
	str	r3, [sp, #8]
	ldr	r3, [r4, #868]
	str	r3, [sp, #4]
	ldr	r3, [r4, #872]
	str	r3, [sp]
	ldr	r3, [r4, #884]
	bl	printk
	ldr	r3, [r7]
	lsls	r3, r3, #23
	bpl	.L490
	ldr	r3, [r4, #928]
	movw	r2, #441
	ldr	r1, .L529+12
	ldr	r0, .L529+28
	str	r3, [sp, #12]
	ldr	r3, [r4, #924]
	str	r3, [sp, #8]
	ldr	r3, [r4, #916]
	str	r3, [sp, #4]
	ldr	r3, [r4, #920]
	str	r3, [sp]
	ldr	r3, [r4, #932]
	bl	printk
.L490:
	ldr	r3, [sp, #60]
	cbnz	r3, .L474
	ldr	r3, [r4, #876]
	ldr	r2, [r4, #884]
	str	r3, [r4, #880]
	add	r3, r3, r2
	ldr	r2, [r4, #872]
	cmp	r3, r2
	ittt	cs
	subcs	r3, r3, r2
	ldrcs	r2, [r4, #868]
	addcs	r3, r3, r2
	str	r3, [r4, #876]
	ldr	r3, [r4, #924]
	ldr	r2, [r4, #932]
	str	r3, [r4, #928]
	add	r3, r3, r2
	ldr	r2, [r4, #920]
	cmp	r3, r2
	ittt	cs
	subcs	r3, r3, r2
	ldrcs	r2, [r4, #916]
	addcs	r3, r3, r2
	str	r3, [r4, #924]
.L474:
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
	ldr	r3, [r4, #752]
	ldr	r3, [r3, #620]
	cbz	r3, .L478
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
.L478:
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
	bge	.L479
	ldr	r0, [r4, #760]
	cbnz	r0, .L480
	ldr	r3, .L529+32
	mov	r2, #131072
	add	r1, r4, #760
	str	r3, [sp]
	ldr	r3, .L529+36
	bl	mpp_buffer_get_with_tag
	b	.L530
.L531:
	.align	2
.L529:
	.word	hal_h265e_debug
	.word	.LANCHOR0+363
	.word	.LC2
	.word	.LANCHOR0+417
	.word	.LC32
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LANCHOR0+456
	.word	.LC27
.L530:
.L480:
	ldr	r0, [r4, #764]
	cbnz	r0, .L481
	ldr	r3, .L532
	mov	r2, #131072
	add	r1, r4, #764
	str	r3, [sp]
	ldr	r3, .L532+4
	bl	mpp_buffer_get_with_tag
.L481:
	movs	r2, #176
	ldr	r1, [r4, #760]
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	movs	r2, #177
	str	r0, [r5, #80]
	ldr	r1, [r4, #764]
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	str	r0, [r5, #84]
.L479:
	ldr	r2, [sp, #52]
	ldrb	r3, [r5, #144]	@ zero_extendqisi2
	cmp	r2, #0
	beq	.L482
	orr	r3, r3, #8
	movs	r2, #171
	strb	r3, [r5, #144]
	ldr	r1, [sp, #52]
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	str	r0, [r5, #60]
.L483:
	ldr	r3, [r6, #32]
	ldr	r2, [r3, #28]
	ldr	r0, [r3]
	cmp	r2, #0
	bne	.L484
	cmp	r0, #0
	beq	.L485
	mov	r1, r0
	movs	r2, #174
	ldr	r0, [r4, #64]
	bl	mpp_dev_get_iova_address
	ldr	r3, [r6, #32]
	ldr	r3, [r3, #12]
	add	r0, r0, r3
	str	r0, [r5, #72]
.L486:
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
.L487:
	ldr	r3, [sp, #40]
	cbz	r3, .L488
	ldr	r3, [r6, #32]
	ldr	r2, [r3]
	cmp	r2, #0
	beq	.L489
	ldr	r2, [sp, #40]
	ldr	r1, .L532
	str	r2, [r3, #20]
	ldr	r0, [r6, #32]
	bl	mpp_buffer_flush_for_device_with_caller
.L488:
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
.L446:
	mul	r7, r7, r9
	cmp	r8, #13
	bhi	.L448
	tbb	[pc, r8]
.L450:
	.byte	(.L451-.L450)/2
	.byte	(.L451-.L450)/2
	.byte	(.L451-.L450)/2
	.byte	(.L448-.L450)/2
	.byte	(.L491-.L450)/2
	.byte	(.L453-.L450)/2
	.byte	(.L491-.L450)/2
	.byte	(.L525-.L450)/2
	.byte	(.L451-.L450)/2
	.byte	(.L451-.L450)/2
	.byte	(.L448-.L450)/2
	.byte	(.L448-.L450)/2
	.byte	(.L491-.L450)/2
	.byte	(.L449-.L450)/2
	.p2align 1
.L451:
	movs	r7, #0
	b	.L447
.L453:
	add	r0, r7, r7, lsl #1
	lsrs	r0, r0, #1
	b	.L447
.L449:
	lsls	r0, r7, #1
	b	.L447
.L448:
	mov	r3, r8
	movw	r2, #1362
	ldr	r1, .L532+8
	ldr	r0, .L532+12
	bl	printk
.L525:
	add	r0, r7, r7, lsl #2
	lsrs	r0, r0, #2
	b	.L447
.L491:
	mov	r0, r7
	b	.L447
.L457:
	ldr	r3, [r4, #876]
	ldr	fp, [r4, #924]
	str	r3, [sp, #16]
	str	fp, [sp, #48]
	b	.L526
.L455:
	ldr	r3, [sp, #64]
	add	r9, r9, r10, lsl #1
	ldr	r1, .L532+16
	ldr	r3, [r3]
	add	r9, r1, r9, lsl #2
	lsls	r0, r3, #23
	ldr	r9, [r9, #440]
	bpl	.L459
	mov	r3, r9
	movw	r2, #353
	addw	r1, r1, #417
	ldr	r0, .L532+20
	bl	printk
.L459:
	cmp	r9, #3
	bhi	.L458
	tbb	[pc, r9]
.L461:
	.byte	(.L464-.L461)/2
	.byte	(.L463-.L461)/2
	.byte	(.L462-.L461)/2
	.byte	(.L460-.L461)/2
	.p2align 1
.L464:
	ldr	r3, [r4, #880]
	ldr	fp, [r4, #924]
	str	r3, [sp, #44]
	ldr	r3, [r4, #928]
	str	r3, [sp, #48]
	ldr	r3, [r4, #876]
	str	r3, [sp, #16]
	ldr	r3, [r4, #868]
	str	r3, [sp, #28]
	ldr	r3, [r4, #872]
	str	r3, [sp, #24]
	ldr	r3, [r4, #916]
	str	r3, [sp, #36]
	ldr	r3, [r4, #920]
.L527:
	str	r3, [sp, #32]
	b	.L458
.L463:
	ldr	r3, [r4, #852]
	ldr	fp, [r4, #916]
	str	r3, [sp, #44]
	ldr	r3, [r4, #900]
	str	fp, [r4, #924]
	str	r3, [sp, #48]
	ldr	r3, [r4, #868]
	str	r3, [sp, #16]
	ldr	r2, [sp, #16]
	str	r3, [r4, #876]
	ldr	r3, [r4, #844]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	ldr	r2, [r4, #848]
	str	r3, [sp, #28]
	ldr	r3, [r4, #872]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [r4, #892]
	str	r2, [sp, #24]
	cmp	r3, fp
	ldr	r2, [r4, #896]
	it	cs
	movcs	r3, fp
	str	r3, [sp, #36]
	ldr	r3, [r4, #920]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	mov	r3, r2
	b	.L527
.L462:
	ldr	r0, .L532+24
	movw	r1, #385
.L528:
	bl	printk
	b	.L458
.L533:
	.align	2
.L532:
	.word	.LANCHOR0+456
	.word	.LC27
	.word	.LANCHOR0+392
	.word	.LC31
	.word	.LANCHOR0
	.word	.LC33
	.word	.LC34
.L460:
	ldr	r3, [r4, #956]
	cmp	r3, #0
	beq	.L465
	add	r1, r4, #868
	str	r1, [sp, #64]
	ldr	r1, [r4, #852]
	add	r3, r4, #892
	ldr	fp, [r4, #916]
	add	r2, r4, #844
	ldr	r9, [sp, #64]
	add	ip, r4, #916
	str	r1, [sp, #44]
	add	lr, sp, #72
	ldr	r1, [r4, #900]
	str	fp, [r4, #924]
	str	r1, [sp, #48]
	ldr	r1, [r4, #868]
	str	r1, [sp, #16]
	ldr	r0, [sp, #16]
	str	r1, [r4, #876]
	ldr	r1, [r4, #844]
	cmp	r1, r0
	it	cs
	movcs	r1, r0
	ldr	r0, [r4, #848]
	str	r1, [sp, #28]
	ldr	r1, [r4, #872]
	cmp	r0, r1
	it	cc
	movcc	r0, r1
	ldr	r1, [r4, #892]
	str	r0, [sp, #24]
	cmp	r1, fp
	ldr	r0, [r4, #896]
	it	cs
	movcs	r1, fp
	str	r1, [sp, #36]
	ldr	r1, [r4, #920]
	cmp	r0, r1
	it	cc
	movcc	r0, r1
	str	r0, [sp, #32]
.L466:
	ldr	r0, [r9]	@ unaligned
	add	r9, r9, #8
	ldr	r1, [r9, #-4]	@ unaligned
	cmp	r9, r3
	mov	r10, lr
	stmia	r10!, {r0, r1}
	mov	lr, r10
	bne	.L466
	ldr	lr, [sp, #64]
	add	r0, r2, #24
	mov	r1, r2
.L467:
	ldr	r9, [r1], #4	@ unaligned
	cmp	r1, r0
	str	r9, [lr], #4	@ unaligned
	bne	.L467
	add	r9, sp, #72
	add	r10, sp, #96
.L468:
	mov	lr, r9
	adds	r2, r2, #8
	ldmia	lr!, {r0, r1}
	cmp	lr, r10
	str	r0, [r2, #-8]	@ unaligned
	mov	r9, lr
	str	r1, [r2, #-4]	@ unaligned
	bne	.L468
	add	r9, sp, #72
	add	r10, ip, #24
	mov	r2, ip
.L469:
	ldr	r0, [r2]	@ unaligned
	adds	r2, r2, #8
	ldr	r1, [r2, #-4]	@ unaligned
	cmp	r2, r10
	mov	lr, r9
	stmia	lr!, {r0, r1}
	mov	r9, lr
	bne	.L469
	add	r1, r3, #24
	mov	r2, r3
.L470:
	ldr	r0, [r2], #4	@ unaligned
	cmp	r2, r1
	str	r0, [ip], #4	@ unaligned
	bne	.L470
	add	ip, sp, #72
	add	lr, sp, #96
.L471:
	mov	r2, ip
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r3, #-8]	@ unaligned
	mov	ip, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L471
	b	.L458
.L465:
	movw	r1, #413
	ldr	r0, .L534
	b	.L528
.L454:
	ldr	r3, [sp, #20]
	ldr	r3, [r3, #292]
	cbnz	r3, .L477
	ldr	r3, [sp, #56]
	movs	r2, #163
	ldr	r0, [r4, #64]
	ldr	r3, [r3, #4]
	ldr	r1, [r3, #12]
	bl	mpp_dev_get_iova_address
	str	r0, [r5, #28]
	ldr	r3, [r4, #816]
	add	r3, r3, r0
	str	r3, [r5, #32]
.L477:
	ldr	r3, [r8, #4]
	movs	r2, #165
	ldr	r0, [r4, #64]
	ldr	r1, [r3, #12]
	bl	mpp_dev_get_iova_address
	mov	r2, #-1
	str	r0, [r5, #36]
	ldr	r3, [r4, #816]
	add	r3, r3, r0
	str	r3, [r5, #40]
	movs	r3, #0
	strd	r2, r3, [r5, #96]
	strd	r2, r3, [r5, #104]
	b	.L474
.L482:
	ldr	r2, [sp, #52]
	bfi	r3, r2, #3, #1
	strb	r3, [r5, #144]
	str	r2, [r5, #60]
	b	.L483
.L485:
	ldr	r2, [r3, #4]
	ldr	r3, [r3, #12]
	add	r3, r3, r2
	str	r3, [r5, #72]
	b	.L486
.L484:
	ldr	r1, .L534+4
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
	b	.L487
.L489:
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L488
	ldr	r0, [r4, #64]
	bl	mpp_get_dev
	ldr	r1, [r6, #32]
	movs	r3, #1
	ldr	r1, [r1, #4]
	ldr	r2, [sp, #40]
	bl	dma_sync_single_for_device
	b	.L488
.L535:
	.align	2
.L534:
	.word	.LC35
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
	.pad #100
	sub	sp, sp, #100
	ldr	r4, [r0, #68]
	mov	r5, r0
	ldr	r7, [r0, #792]
	add	r3, r4, #292
	str	r3, [sp, #44]
	ldr	r3, [r1, #4]
	ldr	r10, [r0, #752]
	ldr	r0, [r1, #52]
	str	r3, [sp, #40]
	str	r1, [sp, #36]
	ldr	r9, [r1, #16]
	bl	mpp_frame_get_is_full
	ldr	r3, .L862
	ldr	r3, [r3]
	lsls	r6, r3, #29
	str	r0, [sp, #48]
	bpl	.L537
	movw	r3, #2086
	ldr	r1, .L862+4
	mov	r2, r3
	ldr	r0, .L862+8
	bl	printk
.L537:
	ldrh	r3, [r9, #4]
	ldrh	r6, [r9, #6]
	add	fp, r3, #7
	adds	r3, r3, #31
	add	r8, r6, #7
	adds	r6, r6, #31
	asrs	r3, r3, #5
	str	r3, [sp, #52]
	ldr	r3, .L862
	asrs	r6, r6, #5
	ldr	r3, [r3]
	lsls	r0, r3, #30
	bpl	.L538
	ldr	r3, [r5, #728]
	movw	r2, #2093
	ldr	r1, .L862+4
	ldr	r0, .L862+12
	str	r3, [sp]
	ldr	r3, [r5, #732]
	bl	printk
.L538:
	mov	r2, #6656
	movs	r1, #0
	mov	r0, r4
	lsr	fp, fp, #3
	bl	memset
	movs	r2, #0
	strb	r2, [r4, #16]
	mov	r3, #0
	ldr	r1, [r5, #772]
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
	add	fp, fp, r3
	ldrh	r3, [r4, #452]
	bfi	r3, fp, #0, #11
	strh	r3, [r4, #452]	@ movhi
	ldrh	r3, [r9, #4]
	and	r3, r3, #7
	uxth	r2, r3
	cmp	r2, #0
	beq	.L705
	rsb	r3, r3, #8
	and	r3, r3, #63
.L539:
	ldrb	r2, [r4, #456]	@ zero_extendqisi2
	lsr	r8, r8, #3
	add	r8, r8, #-1
	bfi	r2, r3, #0, #6
	ldrh	r3, [r4, #454]
	strb	r2, [r4, #456]
	bfi	r3, r8, #0, #11
	strh	r3, [r4, #454]	@ movhi
	ldrh	r3, [r9, #6]
	and	r3, r3, #7
	uxth	r2, r3
	cmp	r2, #0
	beq	.L706
	rsb	r3, r3, #8
	and	r3, r3, #63
.L540:
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
	ldr	r3, [sp, #52]
	orr	r2, r2, #16
	strb	r2, [r4, #436]
	muls	r6, r3, r6
	subs	r0, r6, #1
	lsls	r0, r0, #1
	bl	mpp_log2
	ldrb	r3, [r4, #438]	@ zero_extendqisi2
	bfi	r3, r0, #3, #5
	strb	r3, [r4, #438]
	ldrb	r3, [r4, #483]	@ zero_extendqisi2
	bic	r3, r3, #28
	strb	r3, [r4, #483]
	ldr	r3, [r5, #728]
	cmp	r3, #2
	beq	.L707
	ldr	r3, [r5, #752]
	add	r3, r3, #4736
	ldr	r3, [r3]
	cmp	r3, #1
	ite	ne
	movne	r2, #6
	moveq	r2, #9
.L541:
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
	ldr	r2, [r5, #752]
	ldrb	r3, [r4, #660]	@ zero_extendqisi2
	ldr	r2, [r2, #792]
	bfi	r3, r2, #0, #3
	strb	r3, [r4, #660]
	ldr	r3, .L862+16
	ldrh	r2, [r4, #452]
	str	r3, [r4, #548]
	ubfx	r2, r2, #0, #11
	ldr	r3, [r4, #552]
	lsls	r2, r2, #3
	and	r3, r3, #-2013265920
	adds	r2, r2, #39
	orr	r3, r3, #15859712
	orr	r3, r3, #72
	str	r3, [r4, #552]
	ldrb	r3, [r9, #38]	@ zero_extendqisi2
	asrs	r2, r2, #5
	lsls	r1, r3, #28
	bpl	.L542
	ldr	r3, [r5, #728]
	cmp	r3, #2
	beq	.L542
	ldr	r3, [r5, #404]
	subs	r1, r3, #2
	ldrb	r3, [r4, #559]	@ zero_extendqisi2
	it	ne
	movne	r1, #1
	bfi	r3, r1, #7, #1
	orr	r3, r3, #64
	strb	r3, [r4, #559]
.L542:
	adds	r3, r2, #2
	subs	r2, r2, #1
	cmp	r2, r3
	mov	r0, #0
	mov	r1, #35
	mov	r8, #13
	it	ge
	movge	r2, r3
	adds	r3, r2, #1
	ldrh	r2, [r4, #558]
	movw	fp, #1795
	lsl	lr, r3, #2
	bfi	r2, r3, #2, #10
	strh	r2, [r4, #558]	@ movhi
	movs	r2, #4
.L543:
	mla	r6, r8, r1, r0
	sub	ip, r2, #4
	cmp	r6, fp
	bhi	.L545
	cmp	r2, #36
	bne	.L546
.L547:
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
	ldr	r2, .L862+20
	strb	r3, [r4, #559]
	ldr	r3, [r4, #596]
	strb	r1, [r4, #557]
	ands	r2, r2, r3
	ldr	r3, .L862+24
	orrs	r3, r3, r2
	str	r3, [r4, #596]
	ldr	r3, [r5, #752]
	add	r3, r3, #4736
	ldr	r3, [r3]
	subs	r2, r3, #1
	rsbs	r3, r2, #0
	adcs	r3, r3, r2
	ldrb	r2, [r4, #599]	@ zero_extendqisi2
	bfi	r2, r3, #4, #1
	strb	r2, [r4, #599]
	ldr	r2, [r5, #752]
	movw	r3, #4748
	ldr	r3, [r2, r3]
	ldrb	r2, [r4, #599]	@ zero_extendqisi2
	cmp	r3, #0
	ite	le
	movle	r3, #0
	movgt	r3, #1
	bfi	r2, r3, #5, #1
	strb	r2, [r4, #599]
	ldrb	r2, [r9, #30]	@ zero_extendqisi2
	ldrb	r3, [r4, #596]	@ zero_extendqisi2
	bic	r3, r3, #3
	cbz	r2, .L548
	orr	r3, r3, #2
	b	.L863
.L864:
	.align	2
.L862:
	.word	hal_h265e_debug
	.word	.LANCHOR0+485
	.word	.LC2
	.word	.LC39
	.word	117999
	.word	-209223677
	.word	209192668
.L863:
.L548:
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
	ldr	r3, [r5, #728]
	cmp	r3, #2
	ldrb	r3, [r4, #612]	@ zero_extendqisi2
	ite	eq
	moveq	r2, #19
	movne	r2, #1
	bfi	r3, r2, #0, #6
	ldr	r2, [sp, #48]
	strb	r3, [r4, #612]
	ldr	r3, [r5, #824]
	orrs	r3, r2, r3
	beq	.L551
	ldr	r3, [sp, #36]
	ldr	r6, [r3, #52]
	mov	r0, r6
	bl	mpp_frame_get_is_full
	cmp	r0, #0
	bne	.L552
	ldrb	r3, [r4, #96]	@ zero_extendqisi2
	and	r3, r3, #8
	orn	r3, r3, #124
	strb	r3, [r4, #96]
	ldrb	r3, [r4, #27]	@ zero_extendqisi2
	bic	r3, r3, #7
	orr	r3, r3, #7
	strb	r3, [r4, #27]
	ldr	r3, [r4, #444]
	bic	r3, r3, #458752
	bic	r3, r3, #255
	orr	r3, r3, #327680
	str	r3, [r4, #444]
.L551:
	ldr	r2, [sp, #36]
	mov	r0, r5
	ldr	r1, [sp, #44]
	bl	vepu540c_h265_set_hw_address
	ldr	r2, [r7, #16]
	ldrb	r3, [r4, #48]	@ zero_extendqisi2
	bfi	r3, r2, #4, #4
	strb	r3, [r4, #48]
	ldr	r2, [r7]
	ldrb	r3, [r4, #460]	@ zero_extendqisi2
	and	r2, r2, #15
	bfi	r3, r2, #2, #4
	strb	r3, [r4, #460]
	ldr	r1, [r7, #4]
	bfi	r3, r1, #0, #1
	strb	r3, [r4, #460]
	ldr	r1, [r7, #8]
	bfi	r3, r1, #1, #1
	orn	r3, r3, #127
	strb	r3, [r4, #460]
	ldr	r3, [r10, #60]
	cmp	r3, #0
	ldrb	r3, [r4, #483]	@ zero_extendqisi2
	ite	le
	movle	r1, #0
	movgt	r1, #1
	bfi	r3, r1, #2, #1
	strb	r3, [r4, #483]
	ldr	r1, [r10, #56]
	bfi	r3, r1, #3, #2
	strb	r3, [r4, #483]
	ldr	r1, [r10, #20]
	ldr	r3, [r7]
	cbnz	r1, .L555
	ldr	r1, [r10, #12]
	cmp	r3, #0
	bne	.L556
	lsls	r1, r1, #2
.L555:
	subs	r3, r3, #4
	cmp	r3, #9
	bhi	.L559
	tbh	[pc, r3, lsl #1]
.L561:
	.2byte	(.L711-.L561)/2
	.2byte	(.L559-.L561)/2
	.2byte	(.L711-.L561)/2
	.2byte	(.L559-.L561)/2
	.2byte	(.L559-.L561)/2
	.2byte	(.L559-.L561)/2
	.2byte	(.L559-.L561)/2
	.2byte	(.L559-.L561)/2
	.2byte	(.L562-.L561)/2
	.2byte	(.L711-.L561)/2
	.p2align 1
.L705:
	mov	r3, r2
	b	.L539
.L706:
	mov	r3, r2
	b	.L540
.L707:
	movs	r2, #6
	b	.L541
.L546:
	cmp	r1, #8
	add	r2, r2, #4
	it	hi
	subhi	r1, r1, #4
	add	r0, r0, lr
	it	ls
	movls	r1, #4
	b	.L543
.L545:
	movw	r1, #1796
	cmp	r6, r1
	it	ne
	movne	r2, ip
	b	.L547
.L552:
	mov	r0, r6
	bl	mpp_frame_get_phy_addr
	mov	fp, r0
	mov	r0, r6
	bl	mpp_frame_get_hor_stride
	mov	r8, r0
	mov	r0, r6
	bl	mpp_frame_get_ver_stride
	cmp	fp, #0
	bne	.L553
	movw	r1, #2053
	ldr	r0, .L865
	bl	printk
	mov	r0, #-1
.L536:
	add	sp, sp, #100
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L553:
	mla	r0, r0, r8, fp
	str	fp, [r4, #308]
	strd	r0, r0, [r4, #312]
	b	.L551
.L556:
	cmp	r3, #1
	bne	.L557
	add	r1, r1, r1, lsl #1
	b	.L555
.L557:
	cmp	r3, #2
	beq	.L558
	sub	r0, r3, #8
	cmp	r0, #1
	bhi	.L555
.L558:
	lsls	r1, r1, #1
	b	.L555
.L866:
	.align	2
.L865:
	.word	.LC40
.L562:
	lsls	r0, r1, #1
.L560:
	cmp	r2, #2
	bhi	.L563
	ldr	r3, [r4, #464]
	and	r2, r3, #-134217728
	ldr	r3, .L867
	orrs	r3, r3, r2
	str	r3, [r4, #464]
	ldr	r3, [r4, #468]
	and	r2, r3, #-134217728
	ldr	r3, .L867+4
	orrs	r3, r3, r2
	str	r3, [r4, #468]
	ldr	r3, [r4, #472]
	and	r2, r3, #-134217728
	ldr	r3, .L867+8
	orrs	r3, r3, r2
	str	r3, [r4, #472]
	ldr	r3, [r4, #476]
	lsrs	r3, r3, #21
	lsls	r3, r3, #21
	orr	r3, r3, #32768
	orr	r3, r3, #128
	str	r3, [r4, #476]
.L563:
	ldr	r3, [r4, #488]
	strh	r0, [r4, #492]	@ movhi
	bfi	r3, r1, #0, #17
	str	r3, [r4, #488]
	ldr	r3, [sp, #36]
	ldr	r6, [r5, #752]
	ldr	r7, [r3, #4]
	ldr	r3, [r6, #108]
	cmp	r3, #2
	bne	.L564
	ldr	r2, [r7, #172]
	ldrb	r3, [r4, #437]	@ zero_extendqisi2
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #437]
	ldr	r2, [r7, #172]
	ldrb	r3, [r4, #630]	@ zero_extendqisi2
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #630]
	ldr	r2, [r7, #172]
	ldrh	r3, [r4, #522]
	bfi	r3, r2, #4, #6
	strh	r3, [r4, #522]	@ movhi
	ldr	r2, [r7, #172]
	ubfx	r3, r3, #8, #8
	bfi	r3, r2, #2, #6
	strb	r3, [r4, #523]
	b	.L868
.L869:
	.align	2
.L867:
	.word	20261917
	.word	123164288
	.word	33762283
.L868:
.L565:
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
	ldrb	r2, [r4, #629]	@ zero_extendqisi2
	lsls	r3, r3, #28
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
	ldr	r1, [r5, #952]
	cmp	r1, #0
	beq	.L606
	movs	r2, #179
	ldr	r0, [r5, #64]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #384]
	ldr	r3, [r5, #948]
	add	r3, r3, r0
	str	r3, [r4, #380]
.L607:
	ldr	r3, [r5, #744]
	cbz	r3, .L608
	add	r0, r5, #736
	bl	vepu540c_set_osd
.L608:
	ldr	r3, [sp, #36]
	ldr	r3, [r3, #4]
	ldr	r3, [r3, #212]
	cmp	r3, #0
	bne	.L610
	ldr	r3, [r5, #832]
	cmp	r3, #0
	beq	.L610
	ldr	r3, [r5, #752]
	movw	r2, #4744
	ldr	ip, [r3, r2]
	cmp	ip, #3
	bgt	.L610
	add	r3, r3, #4736
	ldr	r7, [r3]
	cmp	r7, #1
	bne	.L610
	ldrb	r6, [r4, #437]	@ zero_extendqisi2
	add	r0, r4, #952
	ldr	r3, [sp, #36]
	and	lr, r6, #63
	ldrd	fp, r6, [r10, #12]
	ldrd	r1, r2, [r3, #64]
	ldr	r3, [r3, #72]
	str	r6, [sp, #44]
	ldr	r6, [r5, #728]
	str	r3, [sp, #48]
	ldr	r3, [sp, #40]
	sub	r8, r6, #2
	rsbs	r6, r8, #0
	adc	r6, r6, r8
	ldrb	r8, [r3, #156]	@ zero_extendqisi2
	ldr	r3, [r5, #836]
	ubfx	r8, r8, #2, #1
	strd	r7, r6, [sp, #12]
	strd	r8, ip, [sp, #20]
	cmp	r3, #0
	beq	.L612
	ldr	r3, [sp, #44]
	str	lr, [sp]
	strd	fp, r3, [sp, #4]
	ldr	r3, [sp, #48]
	bl	vepu540c_set_qpmap_smart
.L613:
	cbnz	r0, .L610
	ldr	r3, [sp, #36]
	movs	r2, #186
	ldr	r0, [r5, #64]
	ldr	r1, [r3, #68]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #412]
.L610:
	ldr	r1, [r5, #748]
	cbz	r1, .L614
	ldrd	r2, r3, [r10, #12]
	add	r0, r4, #952
	bl	vepu540c_set_roi
.L614:
	ldr	r6, [r5, #752]
	movw	r7, #4744
	ldr	r3, [r6, r7]
	str	r3, [sp, #60]
	movw	r3, #4756
	ldr	r3, [r6, r3]
	str	r3, [sp, #48]
	ldr	r3, [sp, #36]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_ppinfo
	ldr	r3, [r5, #704]
	ldr	ip, [r5, #680]
	ldr	lr, [r5, #752]
	ldr	r10, [lr, r7]
	str	r3, [sp, #44]
	lsr	r3, ip, #6
	str	r3, [sp, #52]
	movw	r3, #4752
	ldr	r7, [lr, r3]
	movw	r3, #4748
	ldr	r3, [lr, r3]
	mov	fp, r0
	add	r8, sp, #64
	str	r3, [sp, #56]
	ldr	r3, .L870
	ldm	r3, {r0, r1, r2, r3}
	stm	r8, {r0, r1, r2, r3}
	ldr	r3, .L870
	add	r8, r3, #16
	ldm	r8, {r0, r1, r2, r3}
	add	r8, sp, #96
	stmdb	r8, {r0, r1, r2, r3}
	ldr	r3, [r5, #728]
	cmp	r3, #2
	beq	.L615
	cmp	r7, #3
	it	eq
	moveq	r7, #0
.L615:
	ldr	r0, [r5, #700]
	add	r3, ip, ip, lsl #2
	cmp	r0, r3, lsr #10
	bcc	.L712
	ldrd	r3, r2, [r5, #688]
	ldr	r1, [r5, #696]
	cmp	r3, r2
	it	cc
	movcc	r3, r2
	cmp	r1, r3
	it	cc
	movcc	r1, r3
	movw	r3, #1126
	muls	r1, r3, r1
	cmp	r0, r1, lsr #10
	ite	cs
	movcs	r8, #1
	movcc	r8, #0
.L616:
	sub	r3, r10, #6
	cmp	r3, #1
	bls	.L617
	cmp	r8, #0
	bne	.L713
.L617:
	movs	r3, #102
	ldr	r2, [sp, #44]
	mul	r3, r3, ip
	cmp	r2, r3, lsr #10
	bhi	.L714
	movs	r3, #66
	mov	r8, #0
	mul	r3, r3, ip
	cmp	r2, r3, lsr #10
	ite	hi
	movhi	r1, #1
	movls	r1, #0
.L618:
	add	lr, lr, #4736
	ldr	r3, [lr]
	cmp	r3, #1
	bne	.L619
	ldrb	r3, [r4, #1833]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r4, #1833]
	ldrb	r3, [r4, #1837]	@ zero_extendqisi2
	orr	r3, r3, #128
	strb	r3, [r4, #1837]
	ldr	r3, [r5, #832]
	cmp	r3, #0
	beq	.L620
	cmp	r10, #3
	ble	.L620
	cmp	r8, #0
	ite	ne
	movne	r3, #48
	moveq	r3, #28
	strb	r3, [r4, #1832]
	add	r3, sp, #96
	add	r3, r3, r1, lsl #2
	ldr	r2, [r3, #-32]
	ldrb	r3, [r3, #-16]	@ zero_extendqisi2
	strb	r2, [r4, #1836]
.L850:
	strb	r3, [r4, #1835]
	strb	r3, [r4, #1834]
.L622:
	ldr	r0, [r5, #832]
	ldrd	r2, r3, [r5, #696]
	add	r3, r3, r2
	ldr	r2, [r5, #704]
	cmp	r0, #0
	beq	.L625
	cmp	r10, #3
	ble	.L625
	add	r0, r2, r2, lsl #1
	cmp	r3, r0, lsr #2
	ite	hi
	movhi	r3, #1
	movls	r3, #0
	cmp	r2, ip, lsr #4
	bcs	.L626
	subs	r3, r3, #8
.L627:
	cmp	r1, #2
	beq	.L718
	cmp	r1, #1
	bne	.L719
	mvn	r3, #1
.L629:
	ldrb	r2, [r4, #1837]	@ zero_extendqisi2
	bfi	r2, r3, #0, #4
	bfc	r2, #4, #1
	strb	r2, [r4, #1837]
	ldr	r3, [r5, #728]
	uxtb	r2, r2
	cmp	r3, #2
	bne	.L633
.L861:
	movs	r3, #1
.L852:
	bfi	r2, r3, #5, #2
	ldr	r0, .L870+4
	ldr	r3, [r4, #1820]
	strb	r2, [r4, #1837]
	ands	r0, r0, r3
	ldr	r3, .L870+8
	ldrh	r2, [r4, #1816]
	orrs	r3, r3, r0
	ldr	r0, .L870+12
	str	r3, [r4, #1820]
	bic	r2, r2, #1016
	ldr	r3, [r4, #1824]
	orr	r2, r2, #256
	strh	r2, [r4, #1816]	@ movhi
	ands	r0, r0, r3
	ldrh	r3, [r4, #1838]
	str	r0, [r4, #1824]
	and	r3, r3, #14
	orr	r3, r3, #1
	strh	r3, [r4, #1838]	@ movhi
	cmp	fp, #0
	bne	.L636
.L637:
	ldrh	r3, [r4, #1840]
	cmp	r1, #0
	bfc	r3, #0, #12
	strh	r3, [r4, #1840]	@ movhi
	ldrh	r3, [r4, #1842]
	itet	ne
	movne	r2, #48
	moveq	r2, #24
	movne	r0, #0
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #1842]	@ movhi
	ldrh	r3, [r4, #1844]
	itee	ne
	movne	r2, #64
	moveq	r2, #48
	moveq	r0, #96
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #1844]	@ movhi
	ldrh	r3, [r4, #1846]
	ite	ne
	movne	r2, #72
	moveq	r2, #64
	bfi	r3, r2, #0, #12
	ldrh	r2, [r4, #1848]
	strh	r3, [r4, #1846]	@ movhi
	movw	r3, #4095
	it	eq
	moveq	r3, #16
	bfi	r2, r3, #0, #12
	ldrh	r3, [r4, #1850]
	strh	r2, [r4, #1848]	@ movhi
	mov	r2, #32
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #1850]	@ movhi
	ldrh	r3, [r4, #1852]
	mov	r2, #48
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #1852]	@ movhi
	ldrh	r3, [r4, #1854]
	bfi	r3, r0, #0, #12
	strh	r3, [r4, #1854]	@ movhi
	ldr	r3, [r4, #1856]
	ite	ne
	movne	r0, #64
	moveq	r0, #96
	and	r3, r3, #-268374016
	orr	r3, r3, #1572864
	orr	r3, r3, r2
	str	r3, [r4, #1856]
	ldrh	r3, [r4, #1860]
	bfi	r3, r0, #0, #12
	strh	r3, [r4, #1860]	@ movhi
	ldrh	r3, [r4, #1862]
	ldr	r0, .L870+16
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #1862]	@ movhi
	ldrb	r3, [r4, #1864]	@ zero_extendqisi2
	ite	ne
	movne	r2, #2
	moveq	r2, #1
	bfi	r3, r2, #0, #4
	strb	r3, [r4, #1864]
	ldrb	r3, [r4, #1865]	@ zero_extendqisi2
	ite	ne
	movne	r2, #5
	moveq	r2, #3
	bfi	r3, r2, #0, #4
	strb	r3, [r4, #1865]
	ldrh	r3, [r4, #1866]
	movw	r2, #769
	and	r3, r3, r0
	orr	r3, r3, r2
	strh	r3, [r4, #1866]	@ movhi
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
	and	r0, r0, r3
	ldr	r3, [r4, #1880]
	orr	r2, r2, r0
	strh	r2, [r4, #1876]	@ movhi
	and	r3, r3, #-1061109568
	ldr	r2, .L870+20
	orr	r3, r3, #150997248
	it	ne
	movne	r0, #0
	orr	r3, r3, #393222
	str	r3, [r4, #1880]
	ldr	r3, [r4, #1884]
	it	eq
	moveq	r0, #5
	and	r3, r3, #-1061109568
	orr	r2, r2, r3
	ldr	r3, [r4, #1888]
	str	r2, [r4, #1884]
	ldr	r2, .L870+24
	and	r3, r3, #-1061109568
	orr	r2, r2, r3
	ldrh	r3, [r4, #1892]
	str	r2, [r4, #1888]
	bic	r3, r3, #16128
	ldrb	r2, [r4, #1900]	@ zero_extendqisi2
	bic	r3, r3, #63
	orr	r3, r3, #4096
	orr	r3, r3, #7
	strh	r3, [r4, #1892]	@ movhi
	ite	ne
	movne	r3, #0
	moveq	r3, #9
	strb	r3, [r4, #1896]
	ite	ne
	movne	r3, #0
	moveq	r3, #6
	strb	r3, [r4, #1898]
	eor	r3, r1, #1
	bfi	r2, r3, #0, #4
	bfi	r2, r0, #4, #4
	strb	r2, [r4, #1900]
	ldrb	r2, [r4, #1901]	@ zero_extendqisi2
	ite	ne
	movne	r0, #0
	moveq	r0, #3
	bfi	r2, r3, #0, #4
	bfi	r2, r0, #4, #4
	strb	r2, [r4, #1901]
	ldrb	r2, [r4, #1902]	@ zero_extendqisi2
	bfi	r2, r3, #0, #4
	ite	ne
	movne	r3, #0
	moveq	r3, #2
	bfi	r2, r3, #4, #4
	ldr	r3, [r4, #1916]
	b	.L871
.L872:
	.align	2
.L870:
	.word	.LANCHOR0+512
	.word	-536346112
	.word	1049601
	.word	-267648256
	.word	-3856
	.word	134613766
	.word	335942665
.L871:
	strb	r2, [r4, #1902]
	ldr	r2, [r4, #1912]
	and	r3, r3, #-268374016
	orr	r3, r3, #4718592
	and	r2, r2, #-268374016
	orr	r3, r3, #15
	orr	r2, r2, #655360
	str	r3, [r4, #1916]
	orr	r2, r2, #5
	str	r2, [r4, #1912]
	ldr	r3, [r5, #752]
	add	r3, r3, #4736
	ldr	r3, [r3]
	cmp	r3, #1
	mov	r3, #16
	bne	.L651
	ldr	r0, .L873
	strb	r3, [r4, #1924]
	str	r0, [r4, #1920]
	ldr	r3, [r5, #836]
	cbz	r3, .L652
	and	r2, r2, #-268374016
	movs	r3, #10
	orr	r2, r2, #458752
	strb	r3, [r4, #1921]
	orr	r2, r2, #1
	str	r2, [r4, #1912]
.L652:
	ldr	r3, [r4, #1944]
	movs	r0, #72
	ldr	ip, .L873+56
	mov	r2, #269488144
	and	r3, r3, #-268374016
	str	r2, [r4, #1952]
	orr	r3, r3, ip
	str	r3, [r4, #1944]
	ldrh	r3, [r4, #1948]
	bfi	r3, r0, #0, #12
	strh	r3, [r4, #1948]	@ movhi
	ldr	r3, [r4, #1968]
	and	r3, r3, #-268374016
	orr	r3, r3, ip
	str	r3, [r4, #1968]
	ldrh	r3, [r4, #1972]
	bfi	r3, r0, #0, #12
	strh	r3, [r4, #1972]	@ movhi
	ldr	r3, [r5, #752]
	add	r3, r3, #4736
	ldr	r3, [r3]
	cmp	r3, #1
	bne	.L655
	and	r3, r8, r1
	cmp	r3, #0
	ite	ne
	movne	r3, #28
	moveq	r3, #27
	strb	r3, [r4, #1976]
	ite	ne
	movne	r3, #26
	moveq	r3, #25
	strb	r3, [r4, #1977]
	ite	ne
	movne	r3, #24
	moveq	r3, #20
	strb	r3, [r4, #1978]
	ite	ne
	movne	r3, #16
	moveq	r3, #19
	strb	r3, [r4, #1979]
.L660:
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
	ldr	r3, [r5, #752]
	add	r3, r3, #4736
	ldr	r3, [r3]
	cmp	r3, #1
	mov	r3, #16
	bne	.L661
	ldr	r2, .L873
	strb	r3, [r4, #1940]
	str	r2, [r4, #1936]
	ldr	r3, [r5, #836]
	cbz	r3, .L662
	ldrh	r3, [r4, #1930]
	movs	r2, #7
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #1930]	@ movhi
	movs	r3, #10
	strb	r3, [r4, #1937]
.L662:
	ldr	r3, [r4, #1956]
	movs	r0, #72
	ldr	ip, .L873+56
	mov	r2, #269488144
	and	r3, r3, #-268374016
	str	r2, [r4, #1964]
	orr	r3, r3, ip
	str	r3, [r4, #1956]
	ldrh	r3, [r4, #1960]
	bfi	r3, r0, #0, #12
	strh	r3, [r4, #1960]	@ movhi
	ldr	r3, [r4, #1980]
	and	r3, r3, #-268374016
	orr	r3, r3, ip
	str	r3, [r4, #1980]
	ldrh	r3, [r4, #1984]
	bfi	r3, r0, #0, #12
	strh	r3, [r4, #1984]	@ movhi
	ldr	r3, [r5, #752]
	add	r3, r3, #4736
	ldr	r3, [r3]
	cmp	r3, #1
	bne	.L665
	and	r1, r8, r1
	cmp	r1, #0
	ite	ne
	movne	r3, #28
	moveq	r3, #27
	strb	r3, [r4, #1988]
	ite	ne
	movne	r3, #26
	moveq	r3, #25
	strb	r3, [r4, #1989]
	ite	ne
	movne	r3, #24
	moveq	r3, #20
	strb	r3, [r4, #1990]
	movs	r3, #16
	strb	r3, [r4, #1991]
.L669:
	ldr	r3, [r4, #1992]
	ldr	r1, .L873+4
	and	r3, r3, #-1061109568
	orrs	r3, r3, r1
	str	r3, [r4, #1992]
	ldr	r3, [r4, #1996]
	and	r3, r3, #-252645136
	orrs	r3, r3, r1
	str	r3, [r4, #1996]
	ldr	r3, [r4, #2000]
	and	r2, r3, #-1061109568
	ldr	r3, .L873+8
	orrs	r3, r3, r2
	str	r3, [r4, #2000]
	ldr	r3, .L873+12
	movw	r2, #2567
	strh	r2, [r4, #2012]	@ movhi
	str	r3, [r4, #2008]
	mov	fp, r3
	cmp	r7, #0
	beq	.L740
	cmp	r7, #2
	ite	lt
	movlt	r8, #17
	movge	r8, #20
.L670:
	strb	r8, [r4, #2016]
	cmp	r7, #0
	beq	.L742
	cmp	r7, #2
	ite	lt
	movlt	r3, #17
	movge	r3, #18
.L671:
	strb	r3, [r4, #2017]
	cmp	r7, #0
	beq	.L744
	cmp	r7, #2
	ite	lt
	movlt	lr, #17
	movge	lr, #19
.L672:
	strb	lr, [r4, #2018]
	strb	r3, [r4, #2019]
	cmp	r7, #0
	beq	.L746
	cmp	r7, #2
	ite	lt
	movlt	ip, #13
	movge	ip, #6
.L673:
	strb	ip, [r4, #2020]
	cmp	r7, #0
	beq	.L748
	cmp	r7, #2
	ite	lt
	movlt	r0, #14
	movge	r0, #9
.L674:
	strb	r0, [r4, #2021]
	cmp	r7, #0
	beq	.L750
	cmp	r7, #2
	ite	lt
	movlt	r1, #15
	movge	r1, #14
.L675:
	cmp	r7, #0
	strb	r3, [r4, #2023]
	strb	r3, [r4, #2037]
	mov	r7, #5
	strb	r3, [r4, #2039]
	it	ne
	movne	r2, #17
	strb	r3, [r4, #2043]
	it	eq
	moveq	r2, #16
	ldrb	r3, [r4, #2046]	@ zero_extendqisi2
	strb	r2, [r4, #2024]
	bfi	r3, r7, #0, #3
	strb	r2, [r4, #2025]
	strb	r3, [r4, #2046]
	strb	r2, [r4, #2044]
	ldr	r3, [r4, #2056]
	strb	r2, [r4, #2045]
	ldr	r2, .L873+16
	ldrb	r10, [r4, #2026]	@ zero_extendqisi2
	ands	r2, r2, r3
	ldr	r3, .L873+20
	bfi	r10, r7, #0, #3
	strb	r10, [r4, #2026]
	orrs	r3, r3, r2
	str	r3, [r4, #2056]
	ldr	r3, [sp, #56]
	movw	r10, #1797
	strb	r1, [r4, #2022]
	strb	r1, [r4, #2042]
	ldr	r1, .L873+24
	strb	r0, [r4, #2041]
	add	r3, r1, r3, lsl #2
	ldrb	r0, [r4, #2064]	@ zero_extendqisi2
	strb	r8, [r4, #2036]
	ldr	r2, [r3, #544]
	str	fp, [r4, #2028]
	strh	r10, [r4, #2032]	@ movhi
	strb	r2, [r4, #2060]
	ldr	r2, [r3, #560]
	strb	lr, [r4, #2038]
	strb	ip, [r4, #2040]
	strb	r2, [r4, #2061]
	ldr	r2, [r3, #576]
	ldr	r7, .L873+28
	strb	r2, [r4, #2062]
	ldr	r2, [r3, #592]
	bfi	r0, r2, #0, #5
	strb	r0, [r4, #2064]
	ldr	r2, [r3, #608]
	strb	r2, [r4, #2065]
	ldr	r2, [r3, #624]
	strb	r2, [r4, #2066]
	ldr	r2, [r3, #640]
	ldr	r3, [r3, #656]
	strb	r2, [r4, #2068]
	strb	r3, [r4, #2069]
	ldr	r2, [sp, #56]
	ldr	r3, .L873+32
	ldr	r3, [r3, r2, lsl #2]
	strb	r3, [r4, #2070]
	ldr	r3, [sp, #36]
	ldr	r8, [r3, #16]
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L677
	movw	r2, #618
	add	r1, r1, #672
	ldr	r0, .L873+36
	bl	printk
.L677:
	ldrb	r3, [r8, #36]	@ zero_extendqisi2
	addw	r0, r4, #2072
	mov	r2, #2720
	and	r3, r3, #3
	cmp	r3, #1
	ite	ls
	ldrls	r1, .L873+40
	ldrhi	r1, .L873+44
	bl	memcpy
	ldr	r3, [r7]
	lsls	r0, r3, #31
	bpl	.L680
	mov	r2, #624
	ldr	r1, .L873+48
	ldr	r0, .L873+52
	bl	printk
	b	.L874
.L875:
	.align	2
.L873:
	.word	269488148
	.word	67175425
	.word	134746128
	.word	100860677
	.word	-7829312
	.word	5522204
	.word	.LANCHOR0
	.word	hal_h265e_debug
	.word	cu16_madi_cost_multi
	.word	.LC41
	.word	vepu540c_h265_jvt_scl_tab
	.word	vepu540c_h265_customer_scl_tab
	.word	.LANCHOR0+672
	.word	.LC42
	.word	2621460
.L874:
.L680:
	ldr	r3, [r5, #728]
	add	r2, r6, #400
	cmp	r3, #2
	addw	r3, r4, #891
	bne	.L681
	add	r1, r6, #336
.L682:
	ldr	r0, [r1], #4
	cmp	r2, r1
	strb	r0, [r3, #1]!
	ldr	r0, [r1, #124]
	and	r0, r0, #63
	strb	r0, [r3, #16]
	bne	.L682
	ldrb	r3, [r4, #1352]	@ zero_extendqisi2
	movs	r2, #11
	ldr	r1, .L876
	add	r0, r4, #1608
	bfi	r3, r2, #0, #5
	strb	r3, [r4, #1352]
	ldr	r3, [sp, #48]
	movs	r2, #208
	add	r1, r1, r3, lsl #2
.L854:
	bl	memcpy
	ldrh	r3, [r4, #1144]
	movs	r2, #171
	bfi	r3, r2, #0, #10
	strh	r3, [r4, #1144]	@ movhi
	ldr	r2, [r5, #836]
	cbz	r2, .L686
	ldr	r1, [r5, #752]
	movw	r2, #4740
	ldr	r2, [r1, r2]
	cmp	r2, #0
	beq	.L687
	movs	r2, #85
.L856:
	bfi	r3, r2, #0, #10
	strh	r3, [r4, #1144]	@ movhi
.L686:
	ldr	r3, [r4, #1144]
	movs	r2, #85
	bfi	r3, r2, #10, #10
	str	r3, [r4, #1144]
	ldr	r3, [r6, #324]
	cbz	r3, .L688
	ldr	r2, [r6, #316]
	ldrh	r3, [r4, #1144]
	bfi	r3, r2, #0, #10
	strh	r3, [r4, #1144]	@ movhi
	ldr	r3, [r4, #1144]
	ldr	r2, [r6, #320]
	bfi	r3, r2, #10, #10
	str	r3, [r4, #1144]
.L688:
	ldr	r3, [r5, #832]
	cmp	r3, #0
	beq	.L689
	ldr	r3, [sp, #60]
	lsls	r1, r3, #31
	bmi	.L689
	ldr	r3, [r6, #328]
	cmp	r3, #0
	bne	.L689
	ldr	r3, [r6, #332]
	cmp	r3, #0
	bne	.L689
	ldrsb	r3, [r5, #713]
	cmp	r3, #2
	bne	.L690
	ldr	r3, [r5, #728]
	cmp	r3, #2
	beq	.L691
	ldrh	r3, [r4, #496]
	bic	r3, r3, #816
	bic	r3, r3, #3
	orr	r3, r3, #544
	orr	r3, r3, #2
.L857:
	strh	r3, [r4, #496]	@ movhi
.L692:
	ldrh	r3, [r4, #1192]
	ldr	r2, .L876+4
	bic	r3, r3, #32640
	ldrh	r1, [r4, #1200]
	bic	r3, r3, #119
	orr	r3, r3, #15360
	orr	r3, r3, #35
	strh	r3, [r4, #1192]	@ movhi
	ldr	r3, [r4, #1196]
	ands	r2, r2, r3
	ldr	r3, .L876+8
	orrs	r3, r3, r2
	str	r3, [r4, #1196]
	movs	r3, #16
	bfi	r1, r3, #0, #12
	ldr	r3, .L876+12
	ldr	r2, [r4, #1208]
	strh	r1, [r4, #1200]	@ movhi
	str	r3, [r4, #1204]
	ldr	r3, .L876+16
	ands	r3, r3, r2
	ldr	r2, [r4, #1212]
	orr	r3, r3, #33
	str	r3, [r4, #1208]
	ldr	r3, [r4, #1216]
	and	r2, r2, #-268374016
	orr	r0, r2, #1048576
	and	r2, r3, #-1073741824
	ldr	r3, .L876+20
	orr	r0, r0, #8
	str	r0, [r4, #1212]
	orrs	r3, r3, r2
	ldr	r2, [r4, #1220]
	str	r3, [r4, #1216]
	and	r6, r2, #-16777216
	ldr	r2, .L876+24
	orrs	r2, r2, r6
	str	r2, [r4, #1220]
	ldr	r2, [r4, #924]
	and	r6, r2, #-16777216
	ldr	r2, .L876+28
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
	ldr	r2, [r5, #752]
	add	r2, r2, #4736
	ldr	r2, [r2]
	cmp	r2, #1
	beq	.L694
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
.L859:
	str	r3, [r4, #1216]
.L695:
	ldr	r3, [sp, #36]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_is_gray
	ldr	r3, [r5, #944]
	cmp	r3, r0
	beq	.L696
	cbnz	r3, .L698
	ldrb	r2, [r4, #1837]	@ zero_extendqisi2
	str	r3, [r4, #1988]
	bfi	r2, r3, #7, #1
	str	r3, [r4, #1976]
	ldrb	r3, [r4, #437]	@ zero_extendqisi2
	strb	r2, [r4, #1837]
	and	r2, r3, #63
	cmp	r2, #29
	bhi	.L699
	movs	r2, #30
.L860:
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #437]
	ldrb	r3, [r4, #630]	@ zero_extendqisi2
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #630]
.L698:
	str	r0, [r5, #944]
.L696:
	ldr	r3, [sp, #40]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsls	r2, r3, #29
	bpl	.L702
	ldr	r0, [r5, #768]
	cmp	r0, #0
	bne	.L703
	ldr	r1, [r5, #752]
	ldr	r2, [r1, #12]
	adds	r2, r2, #31
	bic	r3, r2, #31
	ldr	r2, [r1, #16]
	add	r1, r5, #768
	adds	r2, r2, #31
	bic	r2, r2, #31
	muls	r2, r3, r2
	ldr	r3, .L876+32
	str	r3, [sp]
	ldr	r3, .L876+36
	add	r2, r2, r2, lsl #1
	asrs	r2, r2, #1
	bl	mpp_buffer_get_with_tag
	ldr	r3, [r5, #768]
	cmp	r3, #0
	bne	.L703
	movw	r1, #1973
	ldr	r0, .L876+40
	bl	printk
.L702:
	ldr	r3, [sp, #40]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsls	r3, r3, #28
	bpl	.L704
	ldr	r1, [r5, #792]
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
	ldr	r1, [r5, #768]
	ldr	r0, [r5, #64]
	bl	mpp_dev_get_iova_address
	movs	r3, #0
	str	r3, [r4, #316]
	add	r6, r6, r0
	str	r0, [r4, #308]
	str	r6, [r4, #312]
.L704:
	ldr	r0, [r7]
	ldr	r3, [r5, #808]
	ands	r0, r0, #4
	add	r3, r3, #1
	str	r3, [r5, #808]
	beq	.L536
	movw	r3, #2294
	ldr	r1, .L876+44
	mov	r2, r3
	ldr	r0, .L876+48
	bl	printk
	movs	r0, #0
	b	.L536
.L559:
	add	r0, r1, r1, lsr #31
	asrs	r0, r0, #1
	b	.L560
.L711:
	mov	r0, r1
	b	.L560
.L877:
	.align	2
.L876:
	.word	.LANCHOR1+384
	.word	-536346112
	.word	33574920
	.word	336595976
	.word	-268370040
	.word	12591108
	.word	8413248
	.word	1313797
	.word	.LANCHOR0+700
	.word	.LC27
	.word	.LC43
	.word	.LANCHOR0+485
	.word	.LC4
.L564:
	ldr	r3, [sp, #36]
	ldr	r0, [r7, #160]
	ldr	r3, [r3, #16]
	lsls	r0, r0, #4
	ldrh	r8, [r3, #4]
	ldrh	r1, [r3, #6]
	add	r8, r8, #31
	adds	r1, r1, #31
	asr	r8, r8, #5
	asrs	r1, r1, #5
	mul	r1, r8, r1
	bl	__aeabi_idiv
	ldr	r2, [r7, #172]
	ldrb	ip, [r4, #437]	@ zero_extendqisi2
	bfi	ip, r2, #0, #6
	strb	ip, [r4, #437]
	ldr	r2, [r7, #172]
	ldrb	ip, [r4, #630]	@ zero_extendqisi2
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
	ldr	r2, [r5, #728]
	cmp	r0, #1048576
	it	cs
	movcs	r0, #327680
	cmp	r2, #2
	mul	r3, r0, r8
	ite	eq
	ldreq	r2, [r6, #312]
	ldrne	r2, [r6, #308]
	and	r2, r2, #15
	bfi	ip, r2, #0, #4
	strb	ip, [r4, #522]
	lsrs	r3, r3, #4
	ldr	r2, [r5, #836]
	add	r3, r3, r3, lsl #2
	lsrs	r3, r3, #4
	rsbs	r1, r3, #0
	cbz	r2, .L569
	uxtb	r2, ip
	bfc	r2, #0, #4
	strb	r2, [r4, #522]
.L569:
	ldr	r2, [r7, #176]
	ldrh	ip, [r4, #522]
	bfi	ip, r2, #4, #6
	strh	ip, [r4, #522]	@ movhi
	ldr	r2, [r7, #180]
	ubfx	ip, ip, #8, #8
	bfi	ip, r2, #2, #6
	ldr	r2, [r4, #524]
	strb	ip, [r4, #523]
	bfi	r2, r0, #0, #20
	str	r2, [r4, #524]
	ldr	r2, [r5, #728]
	ldrb	r0, [r4, #630]	@ zero_extendqisi2
	cmp	r2, #2
	bne	.L570
	ldr	ip, [r6, #248]
	ldr	r2, [r6, #256]
	cmp	ip, r2
	bne	.L571
	ldrb	r2, [r4, #437]	@ zero_extendqisi2
	bfi	r2, ip, #0, #6
	strb	r2, [r4, #437]
	ldr	r2, [r6, #248]
.L832:
	bfi	r0, r2, #0, #6
	ldrb	r2, [r4, #522]	@ zero_extendqisi2
	strb	r0, [r4, #630]
	bfc	r2, #0, #4
	strb	r2, [r4, #522]
.L571:
	mvn	r2, #3
	str	r3, [r4, #840]
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
	ldr	r3, .L878
	orrs	r3, r3, r2
	str	r3, [r4, #824]
	ldr	r3, [r4, #828]
	lsrs	r3, r3, #20
	lsls	r3, r3, #20
	str	r3, [r4, #828]
	ldrb	r3, [r6, #672]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L572
.L833:
	ldrb	r2, [r4, #872]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #872]
	ldrb	r3, [r6, #664]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L574
.L834:
	ldrh	r2, [r4, #872]
	and	r3, r3, #63
	bfi	r2, r3, #6, #6
	strh	r2, [r4, #872]	@ movhi
	ldrb	r3, [r6, #673]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L576
.L835:
	ldr	r2, [r4, #872]
	and	r3, r3, #63
	bfi	r2, r3, #12, #6
	str	r2, [r4, #872]
	ldrb	r3, [r6, #665]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L578
.L836:
	ldrb	r2, [r4, #874]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #2, #6
	strb	r2, [r4, #874]
	ldrb	r3, [r6, #674]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L580
.L837:
	ldrb	r2, [r4, #875]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #875]
	ldrb	r3, [r6, #666]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L582
.L838:
	ldrb	r2, [r4, #876]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #876]
	ldrb	r3, [r6, #675]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L584
.L839:
	ldrh	r2, [r4, #876]
	and	r3, r3, #63
	bfi	r2, r3, #6, #6
	strh	r2, [r4, #876]	@ movhi
	ldrb	r3, [r6, #667]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L586
.L840:
	ldr	r2, [r4, #876]
	and	r3, r3, #63
	bfi	r2, r3, #12, #6
	str	r2, [r4, #876]
	ldrb	r3, [r6, #676]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L588
.L841:
	ldrb	r2, [r4, #878]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #2, #6
	strb	r2, [r4, #878]
	ldrb	r3, [r6, #668]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L590
.L842:
	ldrb	r2, [r4, #879]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #879]
	ldrb	r3, [r6, #677]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L592
.L843:
	ldrb	r2, [r4, #880]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #880]
	ldrb	r3, [r6, #669]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L594
.L844:
	ldrh	r2, [r4, #880]
	and	r3, r3, #63
	bfi	r2, r3, #6, #6
	strh	r2, [r4, #880]	@ movhi
	ldrb	r3, [r6, #678]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L596
.L845:
	ldr	r2, [r4, #880]
	and	r3, r3, #63
	bfi	r2, r3, #12, #6
	str	r2, [r4, #880]
	ldrb	r3, [r6, #670]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L598
.L846:
	ldrb	r2, [r4, #882]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #2, #6
	strb	r2, [r4, #882]
	ldrb	r3, [r6, #679]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L600
.L847:
	ldrb	r2, [r4, #883]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #883]
	ldrb	r3, [r6, #671]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L602
.L848:
	ldrb	r2, [r4, #884]	@ zero_extendqisi2
	and	r3, r3, #63
	bfi	r2, r3, #0, #6
	ldrb	r3, [r4, #887]	@ zero_extendqisi2
	strb	r2, [r4, #884]
	ldr	r2, [r6, #680]
	bfi	r3, r2, #6, #2
	strb	r3, [r4, #887]
	b	.L565
.L570:
	ldr	ip, [r6, #252]
	ldr	r2, [r6, #260]
	cmp	ip, r2
	bne	.L571
	ldrb	r2, [r4, #437]	@ zero_extendqisi2
	bfi	r2, ip, #0, #6
	strb	r2, [r4, #437]
	ldr	r2, [r6, #252]
	b	.L832
.L572:
	ldr	r3, [r7, #180]
	b	.L833
.L574:
	ldr	r3, [r7, #176]
	b	.L834
.L576:
	ldr	r3, [r7, #180]
	b	.L835
.L578:
	ldr	r3, [r7, #176]
	b	.L836
.L580:
	ldr	r3, [r7, #180]
	b	.L837
.L582:
	ldr	r3, [r7, #176]
	b	.L838
.L584:
	ldr	r3, [r7, #180]
	b	.L839
.L586:
	ldr	r3, [r7, #176]
	b	.L840
.L588:
	ldr	r3, [r7, #180]
	b	.L841
.L590:
	ldr	r3, [r7, #176]
	b	.L842
.L592:
	ldr	r3, [r7, #180]
	b	.L843
.L594:
	ldr	r3, [r7, #176]
	b	.L844
.L596:
	ldr	r3, [r7, #180]
	b	.L845
.L598:
	ldr	r3, [r7, #176]
	b	.L846
.L600:
	ldr	r3, [r7, #180]
	b	.L847
.L602:
	ldr	r3, [r7, #176]
	b	.L848
.L606:
	strd	r1, r1, [r4, #380]
	b	.L607
.L612:
	ldr	r3, [sp, #44]
	str	lr, [sp]
	strd	fp, r3, [sp, #4]
	ldr	r3, [sp, #48]
	bl	vepu540c_set_qpmap_normal
	b	.L613
.L712:
	mov	r8, #0
	b	.L616
.L713:
	movs	r1, #3
	b	.L618
.L714:
	mov	r8, #0
	movs	r1, #2
	b	.L618
.L620:
	movs	r3, #28
	ldr	r2, [sp, #52]
	strb	r3, [r4, #1832]
	ldr	r3, [sp, #44]
	cmp	r3, r2
	ite	cc
	movcc	r3, #9
	movcs	r3, #12
	strb	r3, [r4, #1836]
	ite	cc
	movcc	r3, #8
	movcs	r3, #12
	b	.L850
.L619:
	ldr	r3, [r4, #1832]
	and	r3, r3, #65024
	orr	r3, r3, #268435456
	orr	r3, r3, #1048592
	str	r3, [r4, #1832]
	ldrh	r3, [r4, #1836]
	and	r3, r3, #32512
	orr	r3, r3, #16
	strh	r3, [r4, #1836]	@ movhi
	b	.L622
.L626:
	add	ip, ip, ip, lsl #1
	cmp	r2, ip, lsr #5
	ite	cc
	subcc	r3, r3, #7
	subcs	r3, r3, #6
	b	.L627
.L625:
	cmp	r3, r2, lsr #1
	ite	hi
	movhi	r3, #1
	movls	r3, #0
	cmp	r2, ip, lsr #8
	bcs	.L630
	subs	r3, r3, #8
.L851:
	movs	r1, #0
	b	.L629
.L630:
	cmp	r2, ip, lsr #7
	bcs	.L631
	subs	r3, r3, #6
	b	.L851
.L631:
	ldr	r1, [sp, #52]
	cmp	r1, r2
	ite	hi
	subhi	r3, r3, #4
	addls	r3, r3, #-1
	b	.L851
.L718:
	movs	r1, #1
	movs	r3, #0
	b	.L629
.L719:
	movs	r1, #1
	b	.L629
.L633:
	ldr	r3, [r5, #404]
	cmp	r3, #2
	beq	.L861
	movs	r3, #2
	b	.L852
.L636:
	ldr	r3, [fp, #8]
	lsls	r3, r3, #31
	bpl	.L637
	orr	r2, r2, #640
	strh	r2, [r4, #1816]	@ movhi
	ldr	r3, [fp, #8]
	ldrb	r2, [r4, #1816]	@ zero_extendqisi2
	asrs	r3, r3, #1
	bfi	r2, r3, #3, #3
	strb	r2, [r4, #1816]
	ldr	r3, [fp, #8]
	ldrh	r2, [r4, #1822]
	asrs	r3, r3, #4
	bfi	r2, r3, #4, #9
	ldrh	r3, [r4, #1826]
	strh	r2, [r4, #1822]	@ movhi
	ldrsh	r2, [fp, #10]
	bfi	r3, r2, #4, #8
	strh	r3, [r4, #1826]	@ movhi
	b	.L637
.L651:
	mov	r2, #269488144
	strb	r3, [r4, #1924]
	str	r2, [r4, #1920]
	b	.L652
.L879:
	.align	2
.L878:
	.word	2130942
.L655:
	str	r2, [r4, #1976]
	b	.L660
.L661:
	mov	r2, #269488144
	strb	r3, [r4, #1940]
	str	r2, [r4, #1936]
	b	.L662
.L665:
	str	r2, [r4, #1988]
	b	.L669
.L740:
	mov	r8, #16
	b	.L670
.L742:
	movs	r3, #16
	b	.L671
.L744:
	mov	lr, #16
	b	.L672
.L746:
	mov	ip, #16
	b	.L673
.L748:
	movs	r0, #16
	b	.L674
.L750:
	movs	r1, #16
	b	.L675
.L681:
	add	r0, r6, #464
.L684:
	ldr	r1, [r2], #4
	cmp	r0, r2
	strb	r1, [r3, #1]!
	ldr	r1, [r2, #124]
	and	r1, r1, #63
	strb	r1, [r3, #16]
	bne	.L684
	ldrb	r3, [r4, #1352]	@ zero_extendqisi2
	movs	r2, #11
	add	r0, r4, #1608
	bfi	r3, r2, #0, #5
	strb	r3, [r4, #1352]
	ldr	r3, [sp, #48]
	movs	r2, #208
	lsls	r1, r3, #2
	ldr	r3, [r5, #752]
	add	r3, r3, #4736
	ldr	r3, [r3]
	subs	r3, r3, #1
	cmp	r3, #1
	ite	ls
	ldrls	r3, .L880
	ldrhi	r3, .L880+4
	add	r1, r1, r3
	b	.L854
.L687:
	movs	r2, #144
	b	.L856
.L690:
	cmp	r3, #0
	ble	.L692
.L691:
	ldrh	r3, [r4, #496]
	bic	r3, r3, #816
	bic	r3, r3, #3
	orr	r3, r3, #272
	orr	r3, r3, #1
	b	.L857
.L689:
	ldr	r3, [r5, #728]
	ldrb	r2, [r4, #497]	@ zero_extendqisi2
	cmp	r3, #2
	ldrb	r3, [r4, #496]	@ zero_extendqisi2
	bne	.L693
	ldr	r1, [r6, #328]
	bfi	r3, r1, #0, #2
	strb	r3, [r4, #496]
	ldr	r1, [r6, #328]
	bfi	r3, r1, #4, #2
	strb	r3, [r4, #496]
	mov	r3, r2
	ldr	r2, [r6, #328]
.L858:
	bfi	r3, r2, #0, #2
	strb	r3, [r4, #497]
	b	.L692
.L693:
	ldr	r1, [r6, #332]
	bfi	r3, r1, #0, #2
	strb	r3, [r4, #496]
	ldr	r1, [r6, #332]
	bfi	r3, r1, #4, #2
	strb	r3, [r4, #496]
	mov	r3, r2
	ldr	r2, [r6, #332]
	b	.L858
.L694:
	ldr	r2, [r5, #836]
	cmp	r2, #0
	beq	.L695
	bic	r3, r3, #1069547520
	ldr	r2, .L880+8
	bic	r3, r3, #4177920
	bic	r3, r3, #15360
	orr	r3, r3, #8388608
	str	r2, [r4, #1204]
	orr	r3, r3, #6144
	b	.L859
.L699:
	cmp	r2, #31
	bhi	.L700
	movs	r2, #32
	b	.L860
.L700:
	cmp	r2, #33
	bhi	.L698
	movs	r2, #34
	b	.L860
.L703:
	ldrb	r3, [r4, #436]	@ zero_extendqisi2
	movs	r2, #163
	orr	r3, r3, #4
	strb	r3, [r4, #436]
	movs	r3, #0
	str	r3, [r4, #320]
	ldr	r1, [r5, #768]
	ldr	r0, [r5, #64]
	bl	mpp_dev_get_iova_address
	ldrb	r3, [r4, #439]	@ zero_extendqisi2
	orr	r3, r3, #128
	strb	r3, [r4, #439]
	str	r0, [r4, #324]
	b	.L702
.L881:
	.align	2
.L880:
	.word	.LANCHOR1+384
	.word	.LANCHOR1+624
	.word	201852420
	.fnend
	.size	hal_h265e_v540c_gen_regs, .-hal_h265e_v540c_gen_regs
	.global	hal_h265e_vepu540c
	.set	cu16_madi_cost_multi,cu32_madi_cost_multi
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.37085, %object
	.size	__func__.37085, 21
__func__.37085:
	.ascii	"hal_h265e_v540c_init\000"
	.type	__FUNCTION__.37086, %object
	.size	__FUNCTION__.37086, 21
__FUNCTION__.37086:
	.ascii	"hal_h265e_v540c_init\000"
	.type	__func__.37331, %object
	.size	__func__.37331, 27
__func__.37331:
	.ascii	"vepu540c_h265_set_feedback\000"
	.type	__func__.37383, %object
	.size	__func__.37383, 30
__func__.37383:
	.ascii	"hal_h265e_v540c_ret_comb_task\000"
	.type	__func__.37346, %object
	.size	__func__.37346, 21
__func__.37346:
	.ascii	"hal_h265e_v540c_wait\000"
	.type	__func__.37287, %object
	.size	__func__.37287, 22
__func__.37287:
	.ascii	"hal_h265e_v540c_start\000"
	.type	__func__.37371, %object
	.size	__func__.37371, 27
__func__.37371:
	.ascii	"hal_h265e_v540c_comb_start\000"
	.type	__func__.37353, %object
	.size	__func__.37353, 25
__func__.37353:
	.ascii	"hal_h265e_v540c_get_task\000"
	.type	__func__.36986, %object
	.size	__func__.36986, 29
__func__.36986:
	.ascii	"vepu540c_h265_setup_hal_bufs\000"
	.type	__FUNCTION__.37002, %object
	.size	__FUNCTION__.37002, 29
__FUNCTION__.37002:
	.ascii	"vepu540c_h265_setup_hal_bufs\000"
	.type	__FUNCTION__.36933, %object
	.size	__FUNCTION__.36933, 13
__FUNCTION__.36933:
	.ascii	"get_wrap_buf\000"
	.type	__func__.37106, %object
	.size	__func__.37106, 27
__func__.37106:
	.ascii	"hal_h265e_vepu540c_prepare\000"
	.type	__func__.37362, %object
	.size	__func__.37362, 25
__func__.37362:
	.ascii	"hal_h265e_v540c_ret_task\000"
	.type	__func__.37096, %object
	.size	__func__.37096, 23
__func__.37096:
	.ascii	"hal_h265e_v540c_deinit\000"
	.type	__FUNCTION__.37097, %object
	.size	__FUNCTION__.37097, 23
__FUNCTION__.37097:
	.ascii	"hal_h265e_v540c_deinit\000"
	.type	__func__.37219, %object
	.size	__func__.37219, 29
__func__.37219:
	.ascii	"vepu540c_h265_set_hw_address\000"
	.type	__func__.37136, %object
	.size	__func__.37136, 25
__func__.37136:
	.ascii	"vepu540c_h265_uv_address\000"
	.type	__func__.36958, %object
	.size	__func__.36958, 21
__func__.36958:
	.ascii	"setup_recn_refr_wrap\000"
	.space	2
	.type	ref_type_map, %object
	.size	ref_type_map, 16
ref_type_map:
	.word	0
	.word	1
	.word	2
	.word	3
	.type	__FUNCTION__.37220, %object
	.size	__FUNCTION__.37220, 29
__FUNCTION__.37220:
	.ascii	"vepu540c_h265_set_hw_address\000"
	.type	__func__.37271, %object
	.size	__func__.37271, 25
__func__.37271:
	.ascii	"hal_h265e_v540c_gen_regs\000"
	.space	2
.LC0:
	.word	9
	.word	12
	.word	16
	.word	16
.LC1:
	.word	8
	.word	12
	.word	16
	.word	16
	.type	cu32_madi_thd0, %object
	.size	cu32_madi_thd0, 16
cu32_madi_thd0:
	.word	3
	.word	3
	.word	5
	.word	6
	.type	cu32_madi_thd1, %object
	.size	cu32_madi_thd1, 16
cu32_madi_thd1:
	.word	3
	.word	35
	.word	4
	.word	5
	.type	cu32_madi_thd2, %object
	.size	cu32_madi_thd2, 16
cu32_madi_thd2:
	.word	20
	.word	25
	.word	5
	.word	4
	.type	cu32_madi_cnt_thd3, %object
	.size	cu32_madi_cnt_thd3, 16
cu32_madi_cnt_thd3:
	.word	3
	.word	0
	.word	0
	.word	0
	.type	cu32_madi_thd4, %object
	.size	cu32_madi_thd4, 16
cu32_madi_thd4:
	.word	30
	.word	20
	.word	10
	.word	8
	.type	cu32_madi_cost_multi, %object
	.size	cu32_madi_cost_multi, 16
cu32_madi_cost_multi:
	.word	16
	.word	24
	.word	28
	.word	32
	.type	cu16_madi_thd0, %object
	.size	cu16_madi_thd0, 16
cu16_madi_thd0:
	.word	2
	.word	4
	.word	5
	.word	6
	.type	cu16_madi_thd1, %object
	.size	cu16_madi_thd1, 16
cu16_madi_thd1:
	.word	20
	.word	6
	.word	5
	.word	5
	.type	__func__.37010, %object
	.size	__func__.37010, 28
__func__.37010:
	.ascii	"setup_vepu540c_hevc_scl_cfg\000"
	.type	__FUNCTION__.37230, %object
	.size	__FUNCTION__.37230, 32
__FUNCTION__.37230:
	.ascii	"vepu540c_h265e_save_pass1_patch\000"
	.type	hal_h265e_vepu540c, %object
	.size	hal_h265e_vepu540c, 64
hal_h265e_vepu540c:
	.word	.LC27
	.word	16777220
	.word	960
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
	.type	aq_I_qp_dealt_smart, %object
	.size	aq_I_qp_dealt_smart, 64
aq_I_qp_dealt_smart:
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
	.word	5
	.word	7
	.word	8
	.word	9
	.type	aq_P_qp_dealt_smart, %object
	.size	aq_P_qp_dealt_smart, 64
aq_P_qp_dealt_smart:
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
	.type	aq_I_thd_smart, %object
	.size	aq_I_thd_smart, 64
aq_I_thd_smart:
	.word	1
	.word	2
	.word	3
	.word	3
	.word	3
	.word	3
	.word	5
	.word	5
	.word	8
	.word	8
	.word	8
	.word	13
	.word	15
	.word	20
	.word	25
	.word	25
	.type	aq_P_thd_smart, %object
	.size	aq_P_thd_smart, 64
aq_P_thd_smart:
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
	.word	7
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
	.size	lamd_modb_qp, 240
lamd_modb_qp:
	.word	44
	.word	56
	.word	68
	.word	88
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
	.word	17956864
	.word	23068672
	.word	29360128
	.word	35913728
	.type	lamd_modb_qp_cvr, %object
	.size	lamd_modb_qp_cvr, 240
lamd_modb_qp_cvr:
	.word	44
	.word	56
	.word	68
	.word	88
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
	.word	31706944
	.word	39948256
	.word	50331648
	.word	63413888
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC2:
	.ascii	"\0016%s:%d: (%d) enter\012\000"
.LC3:
	.ascii	"\0013%s:%d: mpp_dev_init failed. ret: %d\012\000"
.LC4:
	.ascii	"\0016%s:%d: (%d) leave\012\000"
.LC5:
	.ascii	"\0016%s:%d: RKV_ENC_INT_LINKTABLE_FINISH\000"
.LC6:
	.ascii	"\0016%s:%d: RKV_ENC_INT_ONE_FRAME_FINISH\000"
.LC7:
	.ascii	"\0016%s:%d: RKV_ENC_INT_ONE_SLICE_FINISH\000"
.LC8:
	.ascii	"\0016%s:%d: RKV_ENC_INT_SAFE_CLEAR_FINISH\000"
.LC9:
	.ascii	"\0016%s:%d: RKV_ENC_INT_BIT_STREAM_OVERFLOW\000"
.LC10:
	.ascii	"\0016%s:%d: RKV_ENC_INT_BUS_WRITE_FULL\000"
.LC11:
	.ascii	"\0016%s:%d: RKV_ENC_INT_BUS_WRITE_ERROR\000"
.LC12:
	.ascii	"\0016%s:%d: RKV_ENC_INT_BUS_READ_ERROR\000"
.LC13:
	.ascii	"\0016%s:%d: RKV_ENC_INT_TIMEOUT_ERROR\000"
.LC14:
	.ascii	"\0016%s:%d: source id mismatch\012\000"
.LC15:
	.ascii	"\0016%s:%d: output stream size %d\012\000"
.LC16:
	.ascii	"\0013%s:%d: enc_task->flags.err %08x, return early\000"
.LC17:
	.ascii	"\0013%s:%d: poll cmd failed %d status %d \012\000"
.LC18:
	.ascii	"\0013%s:%d: enc_task->flags.err %08x, return e arly"
	.ascii	"\000"
.LC19:
	.ascii	"\0013%s:%d: set register write failed %d\012\000"
.LC20:
	.ascii	"\0016%s:%d: ctl reg[%04x]: 0%08x\012\000"
.LC21:
	.ascii	"\0016%s:%d: hw add cfg reg[%04x]: 0%08x\012\000"
.LC22:
	.ascii	"\0016%s:%d: set reg[%04x]: 0%08x\012\000"
.LC23:
	.ascii	"\0013%s:%d: set register read failed %d\012\000"
.LC24:
	.ascii	"\0013%s:%d: send cmd failed %d\012\000"
.LC25:
	.ascii	"\0013%s:%d: invalid src color space: %d\012\000"
.LC26:
	.ascii	"\0013%s:%d: setup hal bufs failed, free buffers and"
	.ascii	" return\012\000"
.LC27:
	.ascii	"hal_h265e_v540c\000"
.LC28:
	.ascii	"\0016%s:%d: frame size %d -> %d max count %d -> %d\012"
	.ascii	"\000"
.LC29:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC30:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC31:
	.ascii	"\0013%s:%d: unknown color space: %d\012\000"
.LC32:
	.ascii	"\0016%s:%d: cur is idr  lt %d\012\000"
.LC33:
	.ascii	"\0016%s:%d: ref type %d\012\000"
.LC34:
	.ascii	"\0013%d: WARNING: not support lt ref to st when buf"
	.ascii	" is wrap\000"
.LC35:
	.ascii	"\0013%d: WARNING: not support lt ref to lt when buf"
	.ascii	" is wrap\000"
.LC36:
	.ascii	"\0016%s:%d: cur_is_ref %d\012\000"
.LC37:
	.ascii	"\0016%s:%d: hdr[size %d top %d bot %d cur %d pre %d"
	.ascii	"]\012\000"
.LC38:
	.ascii	"\0016%s:%d: bdy [size %d top %d bot %d cur %d pre %"
	.ascii	"d]\012\000"
.LC39:
	.ascii	"\0016%s:%d: frame %d | type %d | start gen regs\000"
.LC40:
	.ascii	"\0013%d: online case set full frame err\000"
.LC41:
	.ascii	"\0016%s:%d: enter\012\000"
.LC42:
	.ascii	"\0016%s:%d: leave\012\000"
.LC43:
	.ascii	"\0013%d: buf_pass1 malloc fail, debreath invaild\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
