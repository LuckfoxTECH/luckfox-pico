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
	.file	"hal_h264e_vepu540c.c"
	.text
	.syntax unified
	.syntax unified
	.global	__aeabi_uidiv
	.global	__aeabi_idiv
	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_ret_task, %function
hal_h264e_vepu540c_ret_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #60
	sub	sp, sp, #60
	ldr	r3, [r0, #160]
	mov	r10, r1
	ldr	r8, [r0, #308]
	mov	r4, r0
	ldr	r6, [r1, #4]
	ldrd	r2, r3, [r3, #60]
	mul	r9, r3, r2
	add	r3, r8, #5600
	adds	r3, r3, #8
	ldrh	r2, [r3, #192]
	ldrh	r1, [r3, #194]
	str	r2, [sp, #24]
	ldrh	r2, [r3, #200]
	str	r2, [sp, #28]
	ldrh	r2, [r3, #208]
	str	r2, [sp, #32]
	ldrh	r2, [r3, #216]
	str	r2, [sp, #36]
	ldrh	r2, [r3, #202]
	add	r1, r1, r2
	ldrh	r2, [r3, #210]
	add	r1, r1, r2
	ldrh	r2, [r3, #218]
	adds	r2, r1, r2
	ldrh	r1, [r3, #196]
	str	r2, [sp, #12]
	ldrh	r2, [r3, #204]
	add	r1, r1, r2
	ldrh	r2, [r3, #212]
	add	r1, r1, r2
	ldrh	r2, [r3, #220]
	adds	r2, r1, r2
	ldrh	r1, [r3, #198]
	str	r2, [sp, #16]
	ldrh	r2, [r3, #206]
	add	r1, r1, r2
	ldrh	r2, [r3, #214]
	add	r1, r1, r2
	ldrh	r2, [r3, #222]
	adds	r2, r1, r2
	str	r2, [sp, #20]
	ldrh	r2, [r3, #224]
	str	r2, [sp, #40]
	ldrh	r2, [r3, #232]
	str	r2, [sp, #44]
	ldrh	r2, [r3, #240]
	ldrh	r7, [r3, #226]
	ldrh	r1, [r3, #250]
	str	r2, [sp, #48]
	ldrh	r2, [r3, #248]
	str	r2, [sp, #52]
	ldrh	r2, [r3, #234]
	add	r7, r7, r2
	ldrh	r2, [r3, #242]
	add	r7, r7, r2
	ldrh	r2, [r3, #236]
	add	r7, r7, r1
	ldrh	r1, [r3, #228]
	add	r1, r1, r2
	ldrh	r2, [r3, #244]
	add	r1, r1, r2
	ldrh	r2, [r3, #252]
	adds	r2, r1, r2
	ldrh	r1, [r3, #230]
	str	r2, [sp, #4]
	ldrh	r2, [r3, #238]
	add	r1, r1, r2
	ldrh	r2, [r3, #246]
	ldrh	r3, [r3, #254]
	add	r1, r1, r2
	adds	r3, r1, r3
	str	r3, [sp, #8]
	ldr	r3, [r0, #968]
	cmp	r3, #0
	bne	.L2
	ldr	r3, [sp, #4]
	add	r5, r7, r7, lsl #4
	movs	r2, #22
	mla	r3, r2, r3, r5
	ldr	r2, [sp, #8]
	movs	r5, #24
	mla	r5, r5, r2, r3
.L80:
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #20]
	lsrs	r5, r5, #2
	add	fp, r3, r3, lsl #2
	movs	r3, #6
	mla	fp, r3, r2, fp
	ldr	r3, [sp, #12]
	add	fp, fp, r3, lsl #2
	ldr	r3, .L88
	lsr	fp, fp, #2
	ldr	r3, [r3]
	lsls	r1, r3, #30
	bpl	.L4
	mov	r3, r4
	movw	r2, #2755
	ldr	r1, .L88+4
	ldr	r0, .L88+8
	bl	printk
.L4:
	movs	r3, #100
	rsb	r2, r9, r9, lsl #4
	muls	r5, r3, r5
	cmp	r5, r2
	bls	.L5
	movs	r3, #200
.L81:
	str	r3, [r6, #204]
	movs	r3, #100
	mul	fp, r3, fp
	movs	r3, #30
	mul	r3, r3, r9
	cmp	fp, r3
	bls	.L9
	movs	r3, #2
.L82:
	str	r3, [r6, #208]
	ldr	fp, [r4, #308]
	ldrb	r3, [fp, #44]	@ zero_extendqisi2
	lsls	r2, r3, #30
	bpl	.L12
	ldr	r3, .L88
	ldr	r3, [r3]
	lsls	r3, r3, #28
	bpl	.L12
	mov	r2, #2640
	ldr	r1, .L88+12
	ldr	r0, .L88+16
	bl	printk
.L12:
	ldrb	r3, [fp, #44]	@ zero_extendqisi2
	lsls	r5, r3, #31
	bpl	.L13
	ldr	r3, .L88
	ldr	r3, [r3]
	lsls	r0, r3, #28
	bpl	.L13
	movw	r2, #2643
	ldr	r1, .L88+12
	ldr	r0, .L88+20
	bl	printk
.L13:
	ldrb	r3, [fp, #44]	@ zero_extendqisi2
	lsls	r1, r3, #28
	bpl	.L14
	ldr	r3, .L88
	ldr	r3, [r3]
	lsls	r2, r3, #28
	bpl	.L14
	movw	r2, #2646
	ldr	r1, .L88+12
	ldr	r0, .L88+24
	bl	printk
.L14:
	ldrb	r3, [fp, #44]	@ zero_extendqisi2
	lsls	r3, r3, #29
	bpl	.L15
	ldr	r3, .L88
	ldr	r3, [r3]
	lsls	r5, r3, #28
	bpl	.L15
	movw	r2, #2649
	ldr	r1, .L88+12
	ldr	r0, .L88+28
	bl	printk
.L15:
	ldrb	r3, [fp, #44]	@ zero_extendqisi2
	ldr	r2, .L88
	lsls	r0, r3, #27
	ldr	r5, [r2]
	bpl	.L16
	lsls	r1, r5, #22
	bpl	.L44
	movw	r2, #2652
	ldr	r1, .L88+12
	ldr	r0, .L88+32
	bl	printk
.L44:
	mvn	r0, #100
	b	.L1
.L2:
	ldr	r3, [sp, #4]
	movs	r5, #11
	ldr	r2, [sp, #8]
	muls	r3, r5, r3
	movs	r5, #12
	mla	r5, r5, r2, r3
	add	r5, r5, r7, lsl #3
	b	.L80
.L5:
	add	r2, r9, r9, lsl #2
	cmp	r5, r2
	bhi	.L81
	cmp	r5, r9, lsr #2
	ite	ls
	movls	r3, #0
	movhi	r3, #1
	b	.L81
.L9:
	movs	r3, #13
	mul	r3, r3, r9
	cmp	fp, r3
	ite	ls
	movls	r3, #0
	movhi	r3, #1
	b	.L82
.L16:
	lsls	r2, r3, #26
	bpl	.L18
	lsls	r0, r5, #22
	bpl	.L47
	ldr	r1, .L88+12
	movw	r2, #2657
	ldr	r0, .L88+36
.L87:
	bl	printk
.L47:
	mov	r0, #-1
	b	.L1
.L18:
	lsls	r1, r3, #25
	bpl	.L19
	lsls	r2, r5, #22
	bpl	.L47
	movw	r2, #2662
	ldr	r1, .L88+12
	ldr	r0, .L88+40
	b	.L87
.L19:
	ldrb	r3, [fp, #45]	@ zero_extendqisi2
	lsls	r3, r3, #31
	bpl	.L20
	lsls	r0, r5, #22
	bpl	.L47
	movw	r2, #2667
	ldr	r1, .L88+12
	ldr	r0, .L88+44
	b	.L87
.L20:
	ldrb	r3, [fp, #46]	@ zero_extendqisi2
	lsls	r1, r3, #31
	bpl	.L21
	lsls	r2, r5, #22
	bpl	.L48
	mov	r2, #2672
	ldr	r1, .L88+12
	ldr	r0, .L88+48
	bl	printk
.L48:
	mvn	r0, #101
	b	.L1
.L89:
	.align	2
.L88:
	.word	hal_h264e_debug
	.word	.LANCHOR0
	.word	.LC1
	.word	.LANCHOR0+28
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
.L21:
	movw	r2, #5608
	ldr	r3, [r10, #44]
	ldr	r2, [r8, r2]
	mov	r1, r9
	add	r3, r3, r2
	ldr	r2, [r10, #48]
	str	r3, [r10, #44]
	add	r2, r2, r3
	lsls	r3, r3, #3
	str	r2, [r10, #48]
	str	r3, [r6, #184]
	movw	r3, #5620
	ldr	r0, [r8, r3]
	bl	__aeabi_uidiv
	str	r0, [r6, #188]
	ldr	r2, [r4, #940]	@ unaligned
	ldrh	r3, [r4, #944]	@ unaligned
	str	r2, [r4, #620]	@ unaligned
	ldrsb	ip, [r4, #623]
	ldrsb	lr, [r4, #622]
	ldrsb	r1, [r4, #621]
	ldrsb	fp, [r4, #620]
	strh	r3, [r4, #624]	@ unaligned
	strb	lr, [r4, #623]
	strb	ip, [r4, #624]
	strb	r1, [r4, #622]
	strb	fp, [r4, #621]
	ldr	r2, [r6, #160]
	ldr	r3, [r6, #184]
	cmp	r2, r3
	ble	.L22
	movs	r0, #6
	muls	r0, r3, r0
	it	mi
	addmi	r0, r0, #3
	cmp	r2, r0, asr #2
	ble	.L23
	movs	r3, #3
.L83:
	strb	r3, [r4, #620]
	add	r1, r1, fp
	ldr	r3, [r10, #4]
	add	lr, lr, r1
	add	ip, ip, lr
	ldrsb	lr, [r4, #620]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	add	ip, ip, lr
	lsls	r3, r3, #27
	itt	mi
	movmi	r3, #0
	strbmi	r3, [r4, #625]
	cmn	ip, #9
	bge	.L38
	movs	r3, #2
.L84:
	strb	r3, [r4, #625]
.L39:
	ldrd	r3, r2, [sp, #32]
	ldrb	ip, [r8, #733]	@ zero_extendqisi2
	add	r3, r3, r2
	ldr	r0, [sp, #12]
	mov	r1, r3
	ldr	r3, [sp, #28]
	add	r1, r1, r3
	ldr	r3, [sp, #24]
	add	r3, r3, r1
	ldrb	r1, [r8, #734]	@ zero_extendqisi2
	mov	r2, r3
	ldrb	r3, [r8, #732]	@ zero_extendqisi2
	add	lr, r3, ip
	mul	lr, r0, lr
	ldr	r0, [sp, #20]
	muls	r0, r1, r0
	add	r1, r1, ip
	mla	r0, r2, r3, r0
	ldr	r3, [sp, #16]
	muls	r1, r3, r1
	add	r0, r0, lr, lsr #1
	add	r0, r0, r1, lsr #1
	ldrd	r3, r1, [sp, #12]
	adds	r1, r3, r1
	ldr	r3, [sp, #20]
	add	r1, r1, r3
	adds	r1, r1, r2
	bne	.L40
.L85:
	ldrd	r3, r2, [sp, #48]
	str	r0, [r6, #192]
	add	r3, r3, r2
	ldrh	r2, [r8, #738]
	mov	r1, r3
	ldr	r3, [sp, #44]
	ubfx	ip, r2, #0, #12
	ldrh	r2, [r8, #740]
	add	r1, r1, r3
	ldr	r3, [sp, #40]
	ubfx	r2, r2, #0, #12
	add	r1, r1, r3
	ldrh	r3, [r8, #736]
	ubfx	r3, r3, #0, #12
	add	r0, r3, ip
	mul	lr, r7, r0
	ldr	r0, [sp, #8]
	muls	r0, r2, r0
	add	r2, r2, ip
	mla	r0, r1, r3, r0
	ldr	r3, [sp, #4]
	add	r7, r7, r3
	muls	r2, r3, r2
	ldr	r3, [sp, #8]
	add	r0, r0, lr, lsr #1
	add	r7, r7, r3
	adds	r1, r7, r1
	add	r0, r0, r2, lsr #1
	bne	.L42
.L86:
	movw	r3, #5764
	str	r0, [r6, #196]
	ldr	r0, [r8, r3]
	add	r3, r8, #5760
	mov	r1, r9
	ldr	r3, [r3]
	ubfx	r0, r0, #0, #23
	ubfx	r3, r3, #0, #23
	add	r0, r0, r3
	movw	r3, #5756
	ldr	r3, [r8, r3]
	ubfx	r3, r3, #0, #21
	add	r0, r0, r3
	lsls	r0, r0, #8
	bl	__aeabi_uidiv
	ldr	r3, [r6, #184]
	str	r0, [r6, #200]
	str	r3, [r4, #212]
	ldr	r3, [r6, #188]
	str	r3, [r4, #216]
	ldr	r3, [r6, #200]
	str	r9, [r4, #592]
	str	r3, [r4, #228]
	movw	r3, #5772
	ldrb	r1, [r8, r3]	@ zero_extendqisi2
	movs	r3, #4
	smulbb	r2, r1, r3
	str	r2, [r4, #600]
	movw	r2, #5773
	ldrb	r0, [r8, r2]	@ zero_extendqisi2
	movw	r2, #5774
	lsls	r0, r0, #2
	str	r0, [r4, #604]
	ldrb	r2, [r8, r2]	@ zero_extendqisi2
	smlabb	r1, r3, r1, r0
	ands	r0, r5, #2
	smulbb	r6, r2, r3
	smlabb	r2, r3, r2, r1
	str	r6, [r4, #608]
	movw	r6, #5775
	ldrb	r6, [r8, r6]	@ zero_extendqisi2
	smulbb	r7, r6, r3
	smlabb	r3, r3, r6, r2
	str	r7, [r4, #612]
	str	r3, [r4, #616]
	ldr	r3, [r4, #168]
	ldr	r3, [r3, #44]
	str	r3, [r4, #316]
	add	r3, r4, #188
	str	r3, [r10, #24]
	mov	r3, #1
	str	r3, [r10, #20]
	beq	.L1
	mov	r3, r4
	movw	r2, #2867
	ldr	r1, .L90
	ldr	r0, .L90+4
	bl	printk
	movs	r0, #0
.L1:
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L23:
	adds	r0, r3, r3, lsl #2
	it	mi
	addmi	r0, r0, #3
	cmp	r2, r0, asr #2
	ble	.L26
	movs	r3, #2
	b	.L83
.L26:
	adds	r3, r3, r3, lsl #3
	it	mi
	addmi	r3, r3, #7
	cmp	r2, r3, asr #3
	ite	gt
	movgt	r3, #1
	movle	r3, #0
	b	.L83
.L22:
	cmp	r3, r2, lsl #1
	ble	.L30
	movs	r3, #251
	b	.L83
.L30:
	rsbs	r0, r2, r2, lsl #3
	it	mi
	addmi	r0, r0, #3
	cmp	r3, r0, asr #2
	ble	.L31
	movs	r3, #252
	b	.L83
.L31:
	movs	r0, #6
	muls	r0, r2, r0
	it	mi
	addmi	r0, r0, #3
	cmp	r3, r0, asr #2
	ble	.L33
	movs	r3, #253
	b	.L83
.L33:
	adds	r2, r2, r2, lsl #2
	it	mi
	addmi	r2, r2, #3
	cmp	r3, r2, asr #2
	ite	gt
	movgt	r3, #254
	movle	r3, #255
	b	.L83
.L38:
	cmn	ip, #2
	bge	.L39
	ldrsb	r3, [r4, #625]
	cmp	r3, #1
	bgt	.L39
	movs	r3, #1
	b	.L84
.L40:
	bl	__aeabi_uidiv
	b	.L85
.L42:
	bl	__aeabi_uidiv
	b	.L86
.L91:
	.align	2
.L90:
	.word	.LANCHOR0
	.word	.LC11
	.fnend
	.size	hal_h264e_vepu540c_ret_task, .-hal_h264e_vepu540c_ret_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_ret_comb_task, %function
hal_h264e_vepu540c_ret_comb_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r7, r1
	ldr	r6, .L107
	mov	r5, r0
	mov	r4, r2
	ldr	r8, [r2, #4]
	ldr	r9, [r0, #308]
	ldr	r3, [r6]
	lsls	r1, r3, #30
	bpl	.L93
	mov	r3, r0
	movw	r2, #2910
	ldr	r1, .L107+4
	ldr	r0, .L107+8
	bl	printk
.L93:
	mov	r1, r7
	mov	r0, r5
	bl	hal_h264e_vepu540c_ret_task
	mov	r7, r0
	cbnz	r0, .L92
	ldrb	r3, [r9, #45]	@ zero_extendqisi2
	lsls	r2, r3, #26
	movw	r2, #5708
	itt	mi
	movmi	r3, #1
	strmi	r3, [r4, #96]
	ldr	r3, [r4, #44]
	ldr	r2, [r9, r2]
	add	r3, r3, r2
	ldr	r2, [r4, #48]
	str	r3, [r4, #44]
	add	r2, r2, r3
	lsls	r3, r3, #3
	str	r2, [r4, #48]
	str	r3, [r8, #184]
	ldr	r3, [r6]
	lsls	r3, r3, #30
	bpl	.L92
	mov	r3, r5
	movw	r2, #2924
	ldr	r1, .L107+4
	ldr	r0, .L107+12
	bl	printk
.L92:
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L108:
	.align	2
.L107:
	.word	hal_h264e_debug
	.word	.LANCHOR0+60
	.word	.LC1
	.word	.LC11
	.fnend
	.size	hal_h264e_vepu540c_ret_comb_task, .-hal_h264e_vepu540c_ret_comb_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_start, %function
hal_h264e_vepu540c_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r0
	ldr	r6, .L135
	.pad #28
	sub	sp, sp, #28
	ldr	r3, [r6]
	lsls	r1, r3, #30
	bpl	.L110
	mov	r3, r0
	movw	r2, #2516
	ldr	r1, .L135+4
	ldr	r0, .L135+8
	bl	printk
.L110:
	ldr	r3, [r5, #308]
	movs	r1, #0
	mov	r2, sp
	ldr	r0, [r5, #4]
	str	r3, [sp]
	movs	r3, #100
	strd	r3, r1, [sp, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L111
	mov	r3, r0
	movw	r2, #2541
.L134:
	ldr	r1, .L135+4
	ldr	r0, .L135+12
.L133:
	bl	printk
.L122:
	ldr	r3, [r6]
	lsls	r3, r3, #30
	bpl	.L109
	mov	r3, r5
	movw	r2, #2629
	ldr	r1, .L135+4
	ldr	r0, .L135+16
	bl	printk
.L109:
	mov	r0, r4
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L111:
	ldr	r3, [r5, #308]
	mov	r1, r0
	mov	r2, sp
	ldr	r0, [r5, #4]
	adds	r3, r3, #100
	mov	r7, #624
	str	r3, [sp]
	mov	r3, #532
	strd	r3, r7, [sp, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L113
	mov	r3, r0
	movw	r2, #2550
	b	.L134
.L113:
	ldr	r3, [r5, #308]
	mov	r2, #272
	mov	r1, r0
	ldr	r0, [r5, #4]
	add	r3, r3, #632
	str	r3, [sp]
	mov	r3, #4096
	strd	r2, r3, [sp, #4]
	mov	r2, sp
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L114
	mov	r3, r0
	movw	r2, #2559
	b	.L134
.L114:
	ldr	r3, [r5, #308]
	mov	r1, r0
	mov	r2, sp
	ldr	r0, [r5, #4]
	add	r3, r3, #904
	mov	lr, #720
	str	r3, [sp]
	mov	r3, #5888
	strd	lr, r3, [sp, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L115
	mov	r3, r0
	movw	r2, #2568
	b	.L134
.L115:
	ldr	r3, [r5, #308]
	mov	r1, r0
	mov	r2, sp
	ldr	r0, [r5, #4]
	add	r3, r3, #1624
	mov	ip, #256
	str	r3, [sp]
	mov	r3, #8192
	strd	ip, r3, [sp, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L116
	mov	r3, r0
	movw	r2, #2577
	b	.L134
.L116:
	ldr	r3, [r5, #308]
	mov	r0, #2720
	mov	r1, r4
	mov	r2, sp
	add	r3, r3, #1880
	str	r3, [sp]
	movw	r3, #8672
	strd	r0, r3, [sp, #4]
	ldr	r0, [r5, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L117
	mov	r3, r0
	movw	r2, #2587
	b	.L134
.L117:
	ldr	r3, [r5, #308]
	mov	r1, #384
	mov	r2, sp
	add	r3, r3, #4576
	adds	r3, r3, #24
	str	r3, [sp]
	mov	r3, #11392
	strd	r1, r3, [sp, #4]
	mov	r1, r0
	ldr	r0, [r5, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L118
	mov	r3, r0
	movw	r2, #2597
	b	.L134
.L118:
	ldr	r3, [r5, #308]
	mov	r1, r0
	mov	r2, sp
	ldr	r0, [r5, #4]
	add	r3, r3, #4960
	adds	r3, r3, #24
	strd	r3, r7, [sp]
	mov	r3, #12288
	str	r3, [sp, #8]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L119
	mov	r3, r0
	movw	r2, #2607
	b	.L134
.L119:
	ldr	r3, [r5, #308]
	mov	r2, #592
	movs	r1, #1
	ldr	r0, [r5, #4]
	add	r3, r3, #5600
	adds	r3, r3, #8
	str	r3, [sp, #12]
	mov	r3, #16384
	strd	r2, r3, [sp, #16]
	add	r2, sp, #12
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L120
	mov	r3, r0
	movw	r2, #2617
	ldr	r1, .L135+4
	ldr	r0, .L135+20
	b	.L133
.L120:
	mov	r2, r0
	movs	r1, #5
	ldr	r0, [r5, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cmp	r0, #0
	beq	.L122
	mov	r3, r0
	mov	r2, #2624
	ldr	r1, .L135+4
	ldr	r0, .L135+24
	b	.L133
.L136:
	.align	2
.L135:
	.word	hal_h264e_debug
	.word	.LANCHOR0+93
	.word	.LC1
	.word	.LC12
	.word	.LC11
	.word	.LC13
	.word	.LC14
	.fnend
	.size	hal_h264e_vepu540c_start, .-hal_h264e_vepu540c_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_comb_start, %function
hal_h264e_vepu540c_comb_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r7, r2
	ldr	r3, [r0]
	.pad #56
	sub	sp, sp, #56
	ldr	r5, .L154
	mov	r4, r0
	mov	r8, r1
	ldr	r6, [r0, #308]
	ldr	r9, [r3, #36]
	ldr	r3, [r5]
	lsls	r2, r3, #30
	bpl	.L138
	mov	r3, r0
	movw	r2, #2881
	ldr	r1, .L154+4
	ldr	r0, .L154+8
	bl	printk
.L138:
	ldrb	r3, [r6, #48]	@ zero_extendqisi2
	add	r10, sp, #28
	mov	r1, r9
	movs	r2, #7
	mov	r0, r10
	bfi	r3, r2, #0, #4
	strb	r3, [r6, #48]
	movw	r9, #4600
	bl	vepu541_set_fmt
	ldr	r3, [r4, #4]
	str	r3, [sp]
	mov	r0, sp
	ldr	r3, [r4, #308]
	str	r10, [sp, #16]
	add	r2, r3, #500
	add	r3, r3, r9
	strd	r3, r7, [sp, #8]
	ldr	r3, [r4, #956]
	str	r2, [sp, #4]
	str	r3, [sp, #20]
	bl	vepu540c_set_jpeg_reg
	ldr	r1, [r7, #92]
	cbz	r1, .L139
	mov	r2, #384
	add	r0, r6, r9
	bl	memcpy
.L139:
	ldr	r1, [r7, #88]
	cbz	r1, .L140
	add	r0, r6, #5280
	mov	r2, #312
	adds	r0, r0, #16
	bl	memcpy
.L140:
	ldr	r3, [r5]
	lsls	r3, r3, #30
	bpl	.L141
	mov	r3, r4
	movw	r2, #2897
	ldr	r1, .L154+4
	ldr	r0, .L154+12
	bl	printk
.L141:
	mov	r1, r8
	mov	r0, r4
	bl	hal_h264e_vepu540c_start
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L155:
	.align	2
.L154:
	.word	hal_h264e_debug
	.word	.LANCHOR0+118
	.word	.LC1
	.word	.LC11
	.fnend
	.size	hal_h264e_vepu540c_comb_start, .-hal_h264e_vepu540c_comb_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_get_task, %function
hal_h264e_vepu540c_get_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r9, #0
	ldr	r5, [r1, #16]
	.pad #52
	sub	sp, sp, #52
	ldr	r3, [r1, #12]
	mov	r4, r0
	ldr	r6, .L249
	adds	r5, r5, #8
	ldr	fp, .L249+52
	mov	r8, r1
	ldr	r7, .L249+4
	mov	r10, r9
	str	r3, [sp, #16]
.L157:
	ldr	r3, [sp, #16]
	cmp	r3, r9
	bgt	.L173
	ldr	r3, .L249+4
	ldr	r5, [r8, #4]
	ldr	r3, [r3]
	lsls	r0, r3, #30
	bpl	.L174
	mov	r3, r4
	movw	r2, #706
	ldr	r1, .L249+8
	ldr	r0, .L249+12
	bl	printk
.L174:
	ldr	r3, [r4, #308]
	add	r3, r3, #4960
	adds	r3, r3, #24
	str	r3, [r4, #296]
	ldr	r0, [r8, #52]
	bl	mpp_frame_get_roi
	str	r0, [r4, #292]
	ldr	r0, [r8, #52]
	bl	mpp_frame_get_osd
	str	r0, [r4, #304]
	ldrb	r3, [r5, #156]	@ zero_extendqisi2
	lsls	r1, r3, #29
	bmi	.L175
	tst	r10, #1
	beq	.L177
	ldr	r2, [r4]
	movw	r9, #8191
	ldr	r3, [r2, #28]
	ldr	r7, [r2, #32]
	add	r10, r3, #63
	adds	r7, r7, #63
	bic	r6, r10, #63
	bic	r7, r7, #63
	asr	r10, r10, #6
	mul	r3, r7, r6
	cmp	r3, #0
	mov	r1, r3
	it	lt
	addlt	r1, r3, #63
	add	r3, r3, r3, lsl #1
	add	r9, r9, r1, asr #6
	ldr	r1, .L249+16
	asrs	r3, r3, #1
	str	r3, [sp, #16]
	and	r9, r9, r1
	add	fp, r9, r3
	mov	r3, r10
	muls	r3, r7, r3
	it	mi
	addmi	r3, r3, #63
	asrs	r5, r3, #6
	ldr	r3, [r4, #40]
	lsls	r5, r5, #8
	str	r3, [sp, #20]
	movw	r3, #4728
	ldr	r0, [r2, r3]
	add	r5, r5, #8160
	adds	r5, r5, #31
	ands	r5, r5, r1
	cmp	r0, #0
	beq	.L208
	bl	mpp_enc_ref_cfg_get_cpb_info
	ldr	r3, [r0]
	add	r8, r3, #1
	ldr	r3, [r0, #4]
	cmp	r8, #2
	it	lt
	movlt	r8, #2
	str	r3, [sp, #24]
.L180:
	cmp	r6, #3072
	ble	.L182
	mov	r3, #896
	mul	r10, r3, r10
	ldr	r3, [r4, #960]
	ldr	r3, [r3, #8]
	sub	r10, r10, #32256
	cmp	r3, #0
	bne	.L183
	ldr	r2, [r4, #148]
	str	r3, [sp, #28]
	cmp	r10, r2
	ldr	r0, [r4, #952]
	beq	.L184
	ldr	r1, .L249+20
	bl	mpp_buffer_put_with_caller
	ldr	r3, [sp, #28]
	str	r3, [r4, #952]
.L185:
	ldr	r3, .L249+20
	mov	r2, r10
	add	r1, r4, #952
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L249+24
	bl	mpp_buffer_get_with_tag
	b	.L186
.L173:
	ldr	r3, [r5, #-8]
	cmp	r3, #6
	bhi	.L158
	tbb	[pc, r3]
.L160:
	.byte	(.L165-.L160)/2
	.byte	(.L164-.L160)/2
	.byte	(.L163-.L160)/2
	.byte	(.L158-.L160)/2
	.byte	(.L162-.L160)/2
	.byte	(.L161-.L160)/2
	.byte	(.L159-.L160)/2
	.p2align 1
.L165:
	ldr	r3, [r7]
	lsls	r2, r3, #28
	bpl	.L166
	movw	r2, #658
	mov	r1, r6
	ldr	r0, .L249+28
	bl	printk
.L166:
	ldr	r3, [r5, #-4]
	str	r3, [r4]
.L167:
	ldr	r2, [r5, #-8]
	movs	r3, #1
	add	r9, r9, #1
	adds	r5, r5, #8
	lsls	r3, r3, r2
	orr	r10, r10, r3
	b	.L157
.L164:
	ldr	r3, [r7]
	lsls	r3, r3, #28
	bpl	.L168
	movw	r2, #663
	mov	r1, r6
	ldr	r0, .L249+32
	bl	printk
.L168:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #160]
	b	.L167
.L163:
	ldr	r3, [r7]
	lsls	r0, r3, #28
	bpl	.L169
	mov	r2, #668
	mov	r1, r6
	ldr	r0, .L249+36
	bl	printk
.L169:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #164]
	b	.L167
.L162:
	ldr	r3, [r7]
	lsls	r1, r3, #28
	bpl	.L170
	movw	r2, #673
	mov	r1, r6
	ldr	r0, .L249+40
	bl	printk
.L170:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #168]
	b	.L167
.L161:
	ldr	r3, [r7]
	lsls	r2, r3, #28
	bpl	.L171
	movw	r2, #678
	mov	r1, r6
	ldr	r0, .L249+44
	bl	printk
.L171:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #172]
	b	.L167
.L159:
	ldr	r3, [r7]
	lsls	r3, r3, #28
	bpl	.L172
	movw	r2, #683
	mov	r1, r6
	ldr	r0, .L249+48
	bl	printk
.L172:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #184]
	b	.L167
.L158:
	movw	r2, #689
	mov	r1, r6
	mov	r0, fp
	bl	printk
	b	.L167
.L208:
	mov	r8, #2
	b	.L180
.L250:
	.align	2
.L249:
	.word	.LANCHOR0+148
	.word	hal_h264e_debug
	.word	.LANCHOR0+171
	.word	.LC1
	.word	-8192
	.word	.LANCHOR0+199
	.word	.LC22
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC21
.L184:
	cmp	r0, #0
	beq	.L185
.L186:
	str	r10, [r4, #148]
.L187:
	addw	r3, r6, #1023
	asrs	r3, r3, #10
	muls	r3, r7, r3
	addw	r7, r7, #1023
	it	mi
	addmi	r3, r3, #15
	bic	r3, r3, #15
	asrs	r7, r7, #10
	muls	r6, r7, r6
	it	mi
	addmi	r6, r6, #15
	bic	r6, r6, #15
	cmp	r6, r3
	it	cc
	movcc	r6, r3
	ldr	r3, [r4, #16]
	cmp	r9, r3
	bne	.L191
	ldr	r3, [r4, #20]
	ldr	r2, [sp, #16]
	cmp	r2, r3
	bne	.L191
	ldr	r3, [r4, #28]
	cmp	fp, r3
	bne	.L191
	ldr	r3, [r4, #32]
	cmp	r5, r3
	bne	.L191
	ldr	r3, [r4, #36]
	cmp	r6, r3
	bne	.L191
	ldr	r3, [sp, #20]
	cmp	r3, r8
	bge	.L177
.L191:
	ldr	r3, .L251
	ldr	r3, [r3]
	lsls	r2, r3, #28
	bpl	.L193
	ldr	r3, [sp, #20]
	movw	r2, #587
	str	r8, [sp, #8]
	ldr	r1, .L251+4
	strd	fp, r3, [sp]
	ldr	r0, .L251+8
	ldr	r3, [r4, #28]
	bl	printk
.L193:
	ldr	r3, [sp, #20]
	str	r9, [r4, #16]
	cmp	r3, r8
	it	lt
	movlt	r3, r8
	mov	r8, r3
	ldr	r3, [sp, #16]
	str	r3, [r4, #20]
	ldr	r3, [r4, #960]
	ldr	r3, [r3]
	cbnz	r3, .L194
	add	r0, r4, #12
	bl	hal_bufs_init
.L194:
	ldr	r2, [r4, #44]
	ldr	r3, [r4, #960]
	str	r5, [sp, #32]
	cmp	r2, #0
	beq	.L195
	movs	r2, #0
	strd	r2, r6, [sp, #36]
	str	r2, [sp, #44]
	ldr	r3, [r3]
	cbnz	r3, .L196
	add	r3, sp, #32
	movs	r2, #4
	mov	r1, r8
	ldr	r0, [r4, #12]
	bl	hal_bufs_setup
.L196:
	ldr	r3, [r4]
	movs	r0, #48
	movs	r1, #240
	ldr	r7, .L251+12
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
	cmp	ip, #0
	it	lt
	addlt	ip, ip, #63
	addw	r0, r0, #4095
	add	r0, r0, r3, asr #1
	asrs	r3, r2, #1
	add	r2, r2, r2, lsl #1
	add	ip, r3, ip, asr #6
	addw	r3, r2, #4095
	ldr	r2, [sp, #24]
	addw	r1, r1, #4095
	addw	ip, ip, #4095
	ands	r0, r0, r7
	and	ip, ip, r7
	ands	r1, r1, r7
	ands	r3, r3, r7
	cmp	r2, #0
	add	r1, r1, r0
	add	r3, r3, ip
	str	r0, [r4, #140]
	str	r1, [r4, #144]
	add	r7, r1, r3
	str	ip, [r4, #92]
	str	r3, [r4, #96]
	ble	.L198
	ldr	r2, [r4, #976]
	strd	r0, r1, [r4, #116]
	strd	ip, r3, [r4, #68]
	cbz	r2, .L199
	str	r0, [r4, #120]
	str	ip, [r4, #72]
.L199:
	ldr	r0, [r4, #72]
	ldr	r2, [r4, #120]
	add	r3, r3, r0
	str	r3, [r4, #100]
	add	ip, r2, r0
	add	r2, r2, r3
	add	r7, r7, ip
	add	r1, r1, r2
	mov	ip, #0
	str	r0, [r4, #76]
	strd	ip, r0, [r4, #52]
	str	ip, [r4, #60]
	strd	r3, r0, [r4, #80]
	strd	r2, r3, [r4, #104]
	str	r2, [r4, #124]
	strd	r1, r2, [r4, #128]
.L207:
	ldr	r3, [r4, #960]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne	.L200
	ldr	r0, [r4, #48]
	cbz	r0, .L201
	ldr	r1, .L251+16
	bl	mpp_buffer_put_with_caller
.L201:
	ldr	r3, .L251+16
	mov	r2, r7
	add	r1, r4, #48
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L251+20
	bl	mpp_buffer_get_with_tag
.L203:
	ldr	r3, [r4, #960]
	ldr	r3, [r3]
	cbz	r3, .L205
	str	r3, [r4, #12]
.L205:
	strd	fp, r5, [r4, #28]
	strd	r6, r8, [r4, #36]
.L177:
	mov	r2, #320
	add	r1, r4, #312
	add	r0, r4, #632
	bl	memcpy
.L175:
	ldr	r3, .L251
	ldr	r3, [r3]
	lsls	r3, r3, #30
	bpl	.L235
	mov	r3, r4
	movw	r2, #719
	ldr	r1, .L251+24
	ldr	r0, .L251+28
	bl	printk
.L235:
	movs	r0, #0
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L183:
	str	r3, [r4, #952]
	b	.L186
.L182:
	ldr	r0, [r4, #952]
	cbz	r0, .L188
	ldr	r3, [r4, #960]
	ldr	r10, [r3, #8]
	cmp	r10, #0
	bne	.L188
	ldr	r1, .L251+32
	bl	mpp_buffer_put_with_caller
	str	r10, [r4, #952]
.L188:
	movs	r3, #0
	str	r3, [r4, #148]
	b	.L187
.L200:
	str	r3, [r4, #48]
	b	.L203
.L195:
	strd	r2, r6, [sp, #36]
	str	fp, [sp, #44]
	ldr	r3, [r3]
	cbnz	r3, .L204
	add	r3, sp, #32
	movs	r2, #4
	mov	r1, r8
	ldr	r0, [r4, #12]
	bl	hal_bufs_setup
.L204:
	strd	r9, fp, [r4, #24]
	b	.L203
.L198:
	movs	r2, #0
	strd	r3, r7, [r4, #124]
	strd	r2, r3, [r4, #76]
	str	r2, [r4, #84]
	str	r3, [r4, #132]
	b	.L207
.L252:
	.align	2
.L251:
	.word	hal_h264e_debug
	.word	.LANCHOR0+214
	.word	.LC23
	.word	-4096
	.word	.LANCHOR0+229
	.word	.LC22
	.word	.LANCHOR0+171
	.word	.LC11
	.word	.LANCHOR0+199
	.fnend
	.size	hal_h264e_vepu540c_get_task, .-hal_h264e_vepu540c_get_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_deinit, %function
hal_h264e_vepu540c_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L286
	ldr	r3, [r5]
	lsls	r2, r3, #30
	bpl	.L254
	mov	r3, r0
	movs	r2, #185
	ldr	r1, .L286+4
	ldr	r0, .L286+8
	bl	printk
.L254:
	ldr	r0, [r4, #4]
	cbz	r0, .L255
	bl	mpp_dev_deinit
	movs	r3, #0
	str	r3, [r4, #4]
.L255:
	ldr	r3, [r4, #960]
	ldr	r6, [r3, #8]
	cbnz	r6, .L256
	ldr	r0, [r4, #952]
	cbz	r0, .L256
	ldr	r1, .L286+12
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #952]
.L256:
	ldr	r3, [r4, #960]
	ldr	r6, [r3]
	cbnz	r6, .L257
	ldr	r0, [r4, #12]
	cbz	r0, .L257
	bl	hal_bufs_deinit
	str	r6, [r4, #12]
.L257:
	ldr	r0, [r4, #152]
	cbz	r0, .L258
	ldr	r1, .L286+12
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #152]
.L258:
	ldr	r3, [r4, #960]
	ldr	r6, [r3, #4]
	cbnz	r6, .L259
	ldr	r0, [r4, #48]
	cbz	r0, .L259
	ldr	r1, .L286+12
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #48]
.L259:
	ldr	r1, [r4, #308]
	cbz	r1, .L260
	ldr	r0, .L286+12
	bl	mpp_osal_free
.L260:
	movs	r3, #0
	str	r3, [r4, #308]
	ldr	r3, [r5]
	lsls	r3, r3, #30
	bpl	.L261
	mov	r3, r4
	movs	r2, #213
	ldr	r1, .L286+4
	ldr	r0, .L286+16
	bl	printk
.L261:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L287:
	.align	2
.L286:
	.word	hal_h264e_debug
	.word	.LANCHOR0+242
	.word	.LC1
	.word	.LANCHOR0+268
	.word	.LC11
	.fnend
	.size	hal_h264e_vepu540c_deinit, .-hal_h264e_vepu540c_deinit
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_init, %function
hal_h264e_vepu540c_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r6, .L317
	mov	r7, r1
	ldr	r3, [r6]
	lsls	r5, r3, #30
	bpl	.L289
	mov	r3, r0
	movs	r2, #222
	ldr	r1, .L317+4
	ldr	r0, .L317+8
	bl	printk
.L289:
	ldr	r3, [r7, #4]
	movs	r1, #16
	add	r0, r7, #32
	str	r3, [r4]
	ldr	r3, [r7, #8]
	str	r3, [r4, #956]
	ldr	r3, [r7, #12]
	str	r3, [r4, #44]
	ldr	r3, [r7, #36]
	str	r3, [r4, #960]
	ldr	r3, [r7, #16]
	str	r3, [r4, #964]
	ldr	r3, [r7, #20]
	str	r3, [r4, #968]
	ldr	r3, [r7, #24]
	str	r3, [r4, #976]
	str	r1, [r7, #28]
	bl	mpp_dev_init
	mov	r5, r0
	cbz	r0, .L290
	mov	r3, r0
	movs	r2, #235
	ldr	r1, .L317+4
	ldr	r0, .L317+12
	bl	printk
	mov	r0, r4
	bl	hal_h264e_vepu540c_deinit
.L291:
	ldr	r3, [r6]
	lsls	r3, r3, #30
	bpl	.L288
	mov	r3, r4
	movw	r2, #281
	ldr	r1, .L317+4
	ldr	r0, .L317+16
	bl	printk
	b	.L288
.L290:
	ldr	r3, [r7, #32]
	movs	r2, #2
	movs	r1, #1
	movw	r0, #683
	str	r3, [r4, #4]
	str	r3, [r4, #300]
	ldr	r3, [r7, #4]
	strd	r2, r1, [r3, #308]
	movw	r2, #341
	strd	r5, r5, [r3, #324]
	strd	r0, r2, [r3, #316]
	add	r2, r3, #336
	str	r5, [r3, #332]
	ldr	r1, [r4, #968]
	cmp	r1, #0
	beq	.L292
	ldr	ip, .L317+36
	add	lr, ip, #64
	mov	r8, lr
.L293:
	mov	r7, ip
	adds	r2, r2, #8
	ldmia	r7!, {r0, r1}
	cmp	r7, lr
	str	r0, [r2, #-8]	@ unaligned
	mov	ip, r7
	str	r1, [r2, #-4]	@ unaligned
	bne	.L293
	ldr	ip, .L317+36
	add	r7, r3, #400
.L294:
	mov	r2, ip
	adds	r7, r7, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, r8
	str	r0, [r7, #-8]	@ unaligned
	mov	ip, r2
	str	r1, [r7, #-4]	@ unaligned
	bne	.L294
	ldr	ip, .L317+40
	add	r7, r3, #464
	add	lr, ip, #64
.L295:
	mov	r2, ip
	adds	r7, r7, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r7, #-8]	@ unaligned
	mov	ip, r2
	str	r1, [r7, #-4]	@ unaligned
	bne	.L295
	ldr	r7, .L317+20
	add	r3, r3, #528
	add	ip, r7, #64
.L296:
	mov	r2, r7
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, ip
	str	r0, [r3, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L296
.L297:
	movw	r1, #6200
	ldr	r0, .L317+24
	bl	mpp_osal_calloc
	str	r0, [r4, #308]
	cmp	r0, #0
	bne	.L291
	mov	r1, #274
	ldr	r0, .L317+28
	bl	printk
	mvn	r5, #3
.L288:
	mov	r0, r5
	pop	{r4, r5, r6, r7, r8, pc}
.L292:
	ldr	ip, .L317+44
	add	lr, ip, #64
	mov	r8, lr
.L298:
	mov	r7, ip
	adds	r2, r2, #8
	ldmia	r7!, {r0, r1}
	cmp	r7, lr
	str	r0, [r2, #-8]	@ unaligned
	mov	ip, r7
	str	r1, [r2, #-4]	@ unaligned
	bne	.L298
	ldr	ip, .L317+44
	add	r7, r3, #400
.L299:
	mov	r2, ip
	adds	r7, r7, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, r8
	str	r0, [r7, #-8]	@ unaligned
	mov	ip, r2
	str	r1, [r7, #-4]	@ unaligned
	bne	.L299
	ldr	ip, .L317+48
	add	r7, r3, #464
	add	lr, ip, #64
.L300:
	mov	r2, ip
	adds	r7, r7, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r7, #-8]	@ unaligned
	mov	ip, r2
	str	r1, [r7, #-4]	@ unaligned
	bne	.L300
	ldr	r7, .L317+32
	add	r3, r3, #528
	add	ip, r7, #64
.L301:
	mov	r2, r7
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, ip
	str	r0, [r3, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L301
	b	.L297
.L318:
	.align	2
.L317:
	.word	hal_h264e_debug
	.word	.LANCHOR0+294
	.word	.LC1
	.word	.LC24
	.word	.LC11
	.word	.LANCHOR1+128
	.word	.LANCHOR0+318
	.word	.LC25
	.word	.LANCHOR1+320
	.word	.LANCHOR1
	.word	.LANCHOR1+64
	.word	.LANCHOR1+192
	.word	.LANCHOR1+256
	.fnend
	.size	hal_h264e_vepu540c_init, .-hal_h264e_vepu540c_init
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_prepare, %function
hal_h264e_vepu540c_prepare:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, .L332
	ldr	r6, [r0]
	ldr	r2, [r4]
	lsls	r2, r2, #30
	bpl	.L320
	mov	r3, r0
	movw	r2, #627
	ldr	r1, .L332+4
	ldr	r0, .L332+8
	bl	printk
.L320:
	ldr	r3, [r6, #8]
	tst	r3, #5
	itt	ne
	movne	r3, #0
	strne	r3, [r6, #8]
	ldr	r3, [r4]
	lsls	r3, r3, #30
	bpl	.L322
	mov	r3, r5
	movw	r2, #642
	ldr	r1, .L332+4
	ldr	r0, .L332+12
	bl	printk
.L322:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L333:
	.align	2
.L332:
	.word	hal_h264e_debug
	.word	.LANCHOR0+342
	.word	.LC1
	.word	.LC11
	.fnend
	.size	hal_h264e_vepu540c_prepare, .-hal_h264e_vepu540c_prepare
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_wait, %function
hal_h264e_vepu540c_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r5, .L347
	ldr	r2, [r5]
	lsls	r2, r2, #30
	bpl	.L335
	mov	r3, r0
	movw	r2, #2684
	ldr	r1, .L347+4
	ldr	r0, .L347+8
	bl	printk
.L335:
	movs	r2, #0
	movs	r1, #6
	ldr	r0, [r6, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L336
	mov	r3, r0
	movw	r2, #2687
	ldr	r1, .L347+4
	mvn	r4, #1008
	ldr	r0, .L347+12
	bl	printk
.L336:
	ldr	r3, [r5]
	lsls	r3, r3, #30
	bpl	.L334
	mov	r3, r6
	movw	r2, #2690
	ldr	r1, .L347+4
	ldr	r0, .L347+16
	bl	printk
.L334:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L348:
	.align	2
.L347:
	.word	hal_h264e_debug
	.word	.LANCHOR0+369
	.word	.LC1
	.word	.LC26
	.word	.LC11
	.fnend
	.size	hal_h264e_vepu540c_wait, .-hal_h264e_vepu540c_wait
	.global	__aeabi_uidivmod
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h264e_vepu540c_gen_regs, %function
hal_h264e_vepu540c_gen_regs:
	.fnstart
	@ args = 0, pretend = 0, frame = 112
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #148
	sub	sp, sp, #148
	ldr	r3, [r1, #4]
	mov	r5, r0
	ldr	r4, [r0, #308]
	ldr	r10, [r0]
	ldrd	fp, r7, [r0, #160]
	ldr	r9, [r0, #168]
	ldr	r0, [r1, #52]
	str	r3, [sp, #80]
	str	r1, [sp, #32]
	bl	mpp_frame_get_is_full
	ldr	r6, .L834
	ldr	r3, [r6]
	lsls	r2, r3, #30
	str	r0, [sp, #92]
	bpl	.L350
	mov	r3, r5
	movw	r2, #2396
	ldr	r1, .L834+4
	ldr	r0, .L834+8
	bl	printk
.L350:
	ldr	r3, [r6]
	lsls	r3, r3, #28
	bpl	.L351
	ldr	r3, [r5, #172]
	movw	r2, #2397
	ldr	r1, .L834+4
	ldr	r0, .L834+12
	ldr	r3, [r3]
	bl	printk
.L351:
	movw	r2, #6200
	movs	r1, #0
	mov	r0, r4
	bl	memset
	ldr	r3, [r6]
	lsls	r0, r3, #30
	bpl	.L352
	movw	r2, #726
	ldr	r1, .L834+16
	ldr	r0, .L834+20
	bl	printk
.L352:
	ldrh	r3, [r4, #16]
	movs	r2, #0
	strh	r2, [r4, #36]	@ movhi
	and	r3, r3, #64512
	orr	r3, r3, #256
	strh	r3, [r4, #16]	@ movhi
	ldrb	r3, [r4, #20]	@ zero_extendqisi2
	bic	r3, r3, #3
	strb	r3, [r4, #20]
	movw	r3, #32767
	strh	r3, [r4, #32]	@ movhi
	mov	r3, #28672
	str	r3, [r4, #48]
	ldrh	r3, [r4, #54]
	bfi	r3, r2, #0, #10
	strh	r3, [r4, #54]	@ movhi
	movw	r3, #65535
	str	r3, [r4, #56]
	ldrb	r3, [r4, #84]	@ zero_extendqisi2
	bic	r3, r3, #7
	orr	r3, r3, #7
	strb	r3, [r4, #84]
	mov	r3, #-1
	str	r3, [r4, #88]
	ldr	r3, [r6]
	lsls	r1, r3, #30
	bpl	.L353
	movw	r2, #803
	ldr	r1, .L834+16
	ldr	r0, .L834+24
	bl	printk
.L353:
	ldr	r3, [r10, #36]
	add	r0, sp, #116
	mov	r1, r3
	str	r3, [sp, #44]
	bl	vepu541_set_fmt
	ldr	r3, [r6]
	lsls	r2, r3, #30
	str	r0, [sp, #88]
	ldr	r8, [sp, #116]
	bpl	.L354
	mov	r2, #816
	ldr	r1, .L834+28
	ldr	r0, .L834+20
	bl	printk
.L354:
	ldr	r3, [sp, #88]
	cmp	r3, #0
	bne	.L349
	ldr	r3, [r10, #12]
	ldrh	r2, [r4, #260]
	adds	r3, r3, #15
	bics	r3, r3, #15
	it	mi
	addmi	r3, r3, #7
	asrs	r3, r3, #3
	subs	r3, r3, #1
	bfi	r2, r3, #0, #11
	strh	r2, [r4, #260]	@ movhi
	ldrsb	r2, [r10, #12]
	add	r3, r2, #15
	bic	r3, r3, #15
	subs	r3, r3, r2
	ldrb	r2, [r4, #264]	@ zero_extendqisi2
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #264]
	ldr	r3, [r10, #16]
	ldrh	r2, [r4, #262]
	adds	r3, r3, #15
	bics	r3, r3, #15
	it	mi
	addmi	r3, r3, #7
	asrs	r3, r3, #3
	subs	r3, r3, #1
	bfi	r2, r3, #0, #11
	strh	r2, [r4, #262]	@ movhi
	ldrsb	r2, [r10, #16]
	add	r3, r2, #15
	bic	r3, r3, #15
	subs	r3, r3, r2
	ldrb	r2, [r4, #266]	@ zero_extendqisi2
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #266]
	ldr	r2, [sp, #132]
	ldrb	r3, [r4, #48]	@ zero_extendqisi2
	bfi	r3, r2, #4, #4
	strb	r3, [r4, #48]
	ldrb	r3, [r4, #268]	@ zero_extendqisi2
	bfi	r3, r8, #2, #4
	strb	r3, [r4, #268]
	ldr	r2, [sp, #120]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #268]
	ldr	r2, [sp, #124]
	bfi	r3, r2, #1, #1
	orn	r3, r3, #127
	strb	r3, [r4, #268]
	ldr	r3, [sp, #44]
	tst	r3, #15728640
	beq	.L358
	ldr	r3, [r10, #12]
	adds	r3, r3, #15
	bic	r3, r3, #15
.L359:
	sub	r2, r8, #4
	cmp	r2, #9
	bhi	.L361
	tbh	[pc, r2, lsl #1]
.L369:
	.2byte	(.L600-.L369)/2
	.2byte	(.L361-.L369)/2
	.2byte	(.L600-.L369)/2
	.2byte	(.L361-.L369)/2
	.2byte	(.L361-.L369)/2
	.2byte	(.L361-.L369)/2
	.2byte	(.L361-.L369)/2
	.2byte	(.L361-.L369)/2
	.2byte	(.L367-.L369)/2
	.2byte	(.L600-.L369)/2
	.p2align 1
.L835:
	.align	2
.L834:
	.word	hal_h264e_debug
	.word	.LANCHOR0+393
	.word	.LC1
	.word	.LC27
	.word	.LANCHOR0+421
	.word	.LC28
	.word	.LC29
	.word	.LANCHOR0+443
.L358:
	ldr	r3, [r10, #20]
	cmp	r3, #0
	bne	.L359
	ldr	r3, [r10, #12]
	cmp	r8, #0
	bne	.L360
	lsls	r3, r3, #2
.L361:
	add	r2, r3, r3, lsr #31
	mov	r1, r3
	asrs	r3, r2, #1
.L368:
	cmp	r8, #2
	bhi	.L601
	ldr	r2, [r4, #272]
	and	r0, r2, #-134217728
	ldr	r2, .L836
	orrs	r2, r2, r0
	str	r2, [r4, #272]
	ldr	r2, [r4, #276]
	and	r0, r2, #-134217728
	ldr	r2, .L836+4
	orrs	r2, r2, r0
	str	r2, [r4, #276]
	ldr	r2, [r4, #280]
	and	r0, r2, #-134217728
	ldr	r2, .L836+8
	orrs	r2, r2, r0
	str	r2, [r4, #280]
	ldr	r2, [r4, #284]
	lsrs	r2, r2, #21
	lsls	r2, r2, #21
	orr	r2, r2, #32768
	orr	r2, r2, #128
	str	r2, [r4, #284]
.L370:
	ldr	r2, [r4, #296]
	strh	r3, [r4, #300]	@ movhi
	bfi	r2, r1, #0, #17
	str	r2, [r4, #296]
	ldr	r3, [r10, #60]
	ldr	r6, .L836+12
	cmp	r3, #0
	ldrb	r3, [r4, #291]	@ zero_extendqisi2
	ite	le
	movle	r2, #0
	movgt	r2, #1
	bfi	r3, r2, #2, #1
	strb	r3, [r4, #291]
	ldr	r2, [r10, #56]
	bfi	r3, r2, #3, #2
	strb	r3, [r4, #291]
	ldrh	r3, [r4, #484]
	ldr	r2, [r6]
	and	r3, r3, #16384
	strh	r3, [r4, #484]	@ movhi
	ldr	r3, [r4, #292]
	and	r3, r3, #-1073692672
	str	r3, [r4, #292]
	lsls	r3, r2, #30
	bpl	.L371
	movw	r2, #917
	ldr	r1, .L836+16
	ldr	r0, .L836+20
	bl	printk
.L371:
	ldr	r3, [r6]
	lsls	r3, r3, #30
	bpl	.L597
	movw	r2, #925
	ldr	r1, .L836+24
	ldr	r0, .L836+28
	bl	printk
.L597:
	ldrb	r3, [r4, #244]	@ zero_extendqisi2
	bfc	r3, #0, #2
	strb	r3, [r4, #244]
	ldr	r3, [r9, #32]
	cmp	r3, #0
	ldrb	r3, [r4, #244]	@ zero_extendqisi2
	ite	le
	movle	r2, #0
	movgt	r2, #1
	bfi	r3, r2, #2, #1
	orr	r3, r3, #16
	strb	r3, [r4, #244]
	ldrb	r3, [r4, #420]	@ zero_extendqisi2
	ldr	r2, [r9, #32]
	bfi	r3, r2, #0, #2
	strb	r3, [r4, #420]
	ldr	r2, [r9, #36]
	bfi	r3, r2, #2, #5
	strb	r3, [r4, #420]
	ldrb	r3, [r4, #424]	@ zero_extendqisi2
	ldr	r2, [fp, #40]
	bfi	r3, r2, #0, #4
	strb	r3, [r4, #424]
	ldr	r2, [fp, #72]
	bfi	r3, r2, #4, #1
	strb	r3, [r4, #424]
	ldr	r2, [fp, #48]
	ldrh	r3, [r4, #424]
	bfi	r3, r2, #5, #4
	strh	r3, [r4, #424]	@ movhi
	ldr	r2, [r7, #8]
	ldrb	r3, [r4, #428]	@ zero_extendqisi2
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #428]
	ldr	r2, [r7, #68]
	bfi	r3, r2, #1, #1
	strb	r3, [r4, #428]
	ldr	r2, [r7, #60]
	bfi	r3, r2, #2, #1
	strb	r3, [r4, #428]
	ldr	r3, [r7, #20]
	ldrb	r2, [r4, #428]	@ zero_extendqisi2
	subs	r3, r3, #1
	bfi	r2, r3, #3, #2
	strb	r2, [r4, #428]
	ldr	r3, [r7, #24]
	uxtb	r2, r2
	subs	r3, r3, #1
	bfi	r2, r3, #5, #2
	strb	r2, [r4, #428]
	ldr	r2, [r7, #36]
	ldrh	r3, [r4, #428]
	bfi	r3, r2, #7, #6
	strh	r3, [r4, #428]	@ movhi
	ldr	r2, [r7, #44]
	ldr	r3, [r4, #428]
	bfi	r3, r2, #13, #5
	str	r3, [r4, #428]
	ldr	r2, [r7, #52]
	ubfx	r3, r3, #16, #8
	bfi	r3, r2, #2, #5
	strb	r3, [r4, #430]
	ldr	r2, [r7, #56]
	ldrb	r3, [r4, #431]	@ zero_extendqisi2
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #431]
	ldr	r3, [r9, #44]
	cmp	r3, #2
	ldrb	r3, [r4, #432]	@ zero_extendqisi2
	ite	eq
	moveq	r2, #2
	movne	r2, #0
	bfi	r3, r2, #0, #2
	strb	r3, [r4, #432]
	ldr	r2, [r9, #52]
	ldrh	r3, [r4, #432]
	bfi	r3, r2, #2, #8
	strh	r3, [r4, #432]	@ movhi
	ubfx	r3, r3, #8, #8
	bfc	r3, #2, #1
	strb	r3, [r4, #433]
	ldr	r2, [r9, #60]
	bfi	r3, r2, #3, #1
	strb	r3, [r4, #433]
	ldr	r2, [r9, #68]
	bfi	r3, r2, #4, #2
	strb	r3, [r4, #433]
	ldr	r3, [r9, #56]
	strh	r3, [r4, #434]	@ movhi
	ldr	r3, [r9, #44]
	cmp	r3, #2
	ite	eq
	ldrheq	r3, [r9, #100]
	movwne	r3, #65535
	strh	r3, [r4, #436]	@ movhi
	ldr	r3, [r9, #108]
	strh	r3, [r4, #438]	@ movhi
	ldrb	r3, [r4, #441]	@ zero_extendqisi2
	ldr	r2, [r9, #72]
	bfi	r3, r2, #3, #2
	strb	r3, [r4, #441]
	ldr	r2, [r9, #76]
	ldrh	r3, [r4, #440]
	bfi	r3, r2, #7, #4
	strh	r3, [r4, #440]	@ movhi
	ldr	r0, [r9, #88]
	bl	h264e_reorder_rd_rewind
	ldr	r0, [r9, #88]
	add	r1, sp, #116
	bl	h264e_reorder_rd_op
	cmp	r0, #0
	bne	.L374
	ldrb	r3, [r4, #440]	@ zero_extendqisi2
	orr	r3, r3, #4
	strb	r3, [r4, #440]
	ldr	r3, [sp, #116]
	ldrb	r2, [r4, #440]	@ zero_extendqisi2
	cmp	r3, #0
	bfi	r2, r3, #0, #2
	strb	r2, [r4, #440]
	blt	.L375
	cmp	r3, #1
	ble	.L376
	cmp	r3, #2
	beq	.L377
.L375:
	movw	r2, #998
	ldr	r1, .L836+24
	ldr	r0, .L836+32
	bl	printk
	b	.L378
.L360:
	cmp	r8, #1
	bne	.L362
	add	r3, r3, r3, lsl #1
	b	.L361
.L362:
	cmp	r8, #2
	beq	.L363
	sub	r2, r8, #8
	cmp	r2, #1
	bhi	.L364
.L363:
	lsls	r3, r3, #1
	b	.L359
.L364:
	sub	r2, r8, #4
	cmp	r2, #9
	bhi	.L361
	adr	r1, .L366
	ldr	pc, [r1, r2, lsl #2]
	.p2align 2
.L366:
	.word	.L599+1
	.word	.L361+1
	.word	.L599+1
	.word	.L361+1
	.word	.L361+1
	.word	.L361+1
	.word	.L361+1
	.word	.L361+1
	.word	.L367+1
	.word	.L599+1
	.p2align 1
.L367:
	lsls	r6, r3, #1
.L365:
	ldr	r2, [sp, #136]
	ldrh	r0, [r4, #272]
	ldr	r1, [r2]
	bfi	r0, r1, #0, #9
	strh	r0, [r4, #272]	@ movhi
	ldr	r1, [r2, #4]
	ldr	r0, [r4, #272]
	bfi	r0, r1, #9, #9
	str	r0, [r4, #272]
	ldr	r1, [r2, #8]
	ubfx	r0, r0, #16, #16
	bfi	r0, r1, #2, #9
	strh	r0, [r4, #274]	@ movhi
	ldrh	r0, [r4, #276]
	ldr	r1, [r2, #12]
	bfi	r0, r1, #0, #9
	strh	r0, [r4, #276]	@ movhi
	ldr	r1, [r2, #16]
	ldr	r0, [r4, #276]
	bfi	r0, r1, #9, #9
	str	r0, [r4, #276]
	ldr	r1, [r2, #20]
	ubfx	r0, r0, #16, #16
	bfi	r0, r1, #2, #9
	strh	r0, [r4, #278]	@ movhi
	ldrh	r0, [r4, #280]
	ldr	r1, [r2, #24]
	bfi	r0, r1, #0, #9
	strh	r0, [r4, #280]	@ movhi
	ldr	r0, [r2, #28]
	ldr	r1, [r4, #280]
	bfi	r1, r0, #9, #9
	str	r1, [r4, #280]
	ldr	r2, [r2, #32]
	ubfx	r1, r1, #16, #16
	ldrb	r0, [r4, #286]	@ zero_extendqisi2
	bfi	r1, r2, #2, #9
	strh	r1, [r4, #282]	@ movhi
	ldr	r2, [sp, #140]
	ldr	r1, [r2]
	bfi	r0, r1, #0, #5
	strb	r0, [r4, #286]
	ldr	r1, [r2, #4]
	strb	r1, [r4, #285]
	mov	r1, r3
	ldr	r2, [r2, #8]
	mov	r3, r6
	strb	r2, [r4, #284]
	b	.L370
.L600:
	mov	r1, r3
	b	.L368
.L837:
	.align	2
.L836:
	.word	20261917
	.word	123164288
	.word	33762283
	.word	hal_h264e_debug
	.word	.LANCHOR0+443
	.word	.LC29
	.word	.LANCHOR0+463
	.word	.LC28
	.word	.LC30
.L599:
	mov	r6, r3
	b	.L365
.L601:
	mov	r6, r3
	mov	r3, r1
	b	.L365
.L376:
	ldr	r3, [sp, #120]
.L811:
	strh	r3, [r4, #442]	@ movhi
.L378:
	ldr	r3, [r4, #444]
	and	r3, r3, #-536870912
	str	r3, [r4, #444]
	movs	r3, #0
	str	r3, [r4, #448]
	ldrh	r3, [r4, #452]
	and	r3, r3, #61440
	strh	r3, [r4, #452]	@ movhi
	ldr	r0, [r9, #92]
	bl	h264e_marking_rd_rewind
	ldr	r3, [r9, #44]
	cmp	r3, #2
	bne	.L379
	ldr	r2, [r9, #116]
	ldrb	r3, [r4, #444]	@ zero_extendqisi2
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #444]
	ldr	r2, [r9, #120]
	bfi	r3, r2, #1, #1
	strb	r3, [r4, #444]
.L380:
	ldr	r3, .L838
	ldr	r2, [r3]
	str	r3, [sp, #44]
	tst	r2, #2
	beq	.L408
	movw	r2, #1207
	ldr	r1, .L838+4
	ldr	r0, .L838+8
	bl	printk
.L408:
	ldr	r3, [r5]
	ldr	r6, [r5, #308]
	add	r3, r3, #4736
	ldr	r8, [r3]
	ldr	r3, [sp, #44]
	ldr	r3, [r3]
	lsls	r0, r3, #30
	bpl	.L409
	movw	r2, #1217
	ldr	r1, .L838+12
	ldr	r0, .L838+16
	bl	printk
.L409:
	ldr	r3, [r9, #44]
	cmp	r3, #2
	ldrb	r3, [r6, #756]	@ zero_extendqisi2
	bne	.L410
	movs	r2, #6
.L812:
	bfi	r3, r2, #0, #4
	strb	r3, [r6, #756]
	ldr	r3, [fp]
	cmp	r3, #66
	bne	.L608
	ldr	r3, [fp, #28]
	cmp	r3, #30
	ite	gt
	movgt	r3, #0
	movle	r3, #1
.L413:
	ldrb	r2, [r6, #404]	@ zero_extendqisi2
	add	r8, r8, #-1
	bfi	r2, r3, #0, #1
	strb	r2, [r6, #404]
	ldr	r3, [fp]
	ldrb	r2, [r6, #404]	@ zero_extendqisi2
	cmp	r3, #76
	itett	le
	ldrle	r3, [r7, #8]
	movgt	r3, #0
	clzle	r3, r3
	lsrle	r3, r3, #5
	cmp	r8, #1
	bfi	r2, r3, #3, #1
	strb	r2, [r6, #404]
	ldrh	r3, [r6, #404]
	ite	hi
	movhi	r8, #0
	movls	r8, #1
	orr	r3, r3, #8192
	orr	r3, r3, #16
	strh	r3, [r6, #404]	@ movhi
	ldr	r2, [r7, #72]
	ldrb	r3, [r6, #406]	@ zero_extendqisi2
	bfi	r3, r2, #4, #2
	strb	r3, [r6, #406]
	ldrb	r3, [r6, #407]	@ zero_extendqisi2
	bfi	r3, r8, #4, #1
	strb	r3, [r6, #407]
	ldr	r2, [r5]
	movw	r3, #4748
	ldr	r3, [r2, r3]
	ldrb	r2, [r6, #407]	@ zero_extendqisi2
	cmp	r3, #0
	ite	le
	movle	r3, #0
	movgt	r3, #1
	bfi	r2, r3, #5, #1
	ldrb	r3, [r6, #405]	@ zero_extendqisi2
	strb	r2, [r6, #407]
	orr	r3, r3, #128
	strb	r3, [r6, #405]
	ldrh	r3, [r6, #408]
	bfc	r3, #0, #9
	strh	r3, [r6, #408]	@ movhi
	ldr	r3, .L838
	ldr	r3, [r3]
	lsls	r1, r3, #30
	bpl	.L415
	movw	r2, #1235
	ldr	r1, .L838+12
	ldr	r0, .L838+8
	bl	printk
.L415:
	ldr	r3, [r5, #168]
	add	ip, sp, #116
	ldr	r2, [r5]
	ldr	r7, [r5, #912]
	str	r3, [sp, #44]
	movw	r3, #4744
	ldr	r8, [r2, r3]
	ldr	r3, [r5, #936]
	ldr	r6, [r5, #308]
	str	r3, [sp, #64]
	lsrs	r3, r7, #6
	str	r3, [sp, #68]
	ldr	r3, .L838+20
	ldm	r3, {r0, r1, r2, r3}
	stm	ip, {r0, r1, r2, r3}
	ldr	r3, [sp, #32]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_ppinfo
	ldr	r2, .L838
	ldr	r2, [r2]
	lsls	r2, r2, #30
	mov	r3, r0
	bpl	.L416
	ldr	r3, .L838+20
	mov	r2, #1256
	str	r0, [sp, #84]
	add	r1, r3, #16
	ldr	r0, .L838+16
	bl	printk
	ldr	r3, [sp, #84]
.L416:
	ldr	r1, [r5, #932]
	add	r2, r7, r7, lsl #2
	cmp	r1, r2, lsr #10
	bcc	.L610
	ldrd	r2, r0, [r5, #920]
	cmp	r2, r0
	it	cc
	movcc	r2, r0
	ldr	r0, [r5, #928]
	cmp	r2, r0
	it	cc
	movcc	r2, r0
	movw	r0, #1126
	muls	r2, r0, r2
	cmp	r1, r2, lsr #10
	ite	cs
	movcs	r2, #1
	movcc	r2, #0
.L417:
	sub	r1, r8, #6
	cmp	r1, #1
	bls	.L418
	cmp	r2, #0
	bne	.L611
.L418:
	movs	r2, #102
	ldr	r1, [r5, #936]
	muls	r2, r7, r2
	cmp	r1, r2, lsr #10
	bhi	.L612
	movs	r2, #66
	muls	r2, r7, r2
	cmp	r1, r2, lsr #10
	ite	hi
	movhi	r1, #1
	movls	r1, #0
.L419:
	ldr	r2, [r5]
	add	r2, r2, #4736
	ldr	r2, [r2]
	cmp	r2, #1
	bne	.L420
	ldrb	r2, [r6, #1645]	@ zero_extendqisi2
	orr	r2, r2, #128
	strb	r2, [r6, #1645]
	ldr	r2, [r5, #964]
	cmp	r2, #0
	beq	.L421
	cmp	r8, #3
	ble	.L421
	add	r2, sp, #144
	add	r2, r2, r1, lsl #2
	ldr	r2, [r2, #-28]
.L813:
	strb	r2, [r6, #1644]
.L422:
	ldrd	r2, r0, [r5, #928]
	add	ip, r2, r0
	ldr	r2, [r5, #964]
	ldr	r0, [r5, #936]
	cmp	r2, #0
	beq	.L424
	cmp	r8, #3
	ble	.L424
	add	r2, r0, r0, lsl #1
	cmp	ip, r2, lsr #2
	ite	hi
	movhi	r2, #1
	movls	r2, #0
	cmp	r0, r7, lsr #4
	bcs	.L425
	subs	r2, r2, #8
.L426:
	cmp	r1, #2
	beq	.L614
	cmp	r1, #1
	bne	.L615
	mvn	r2, #1
.L428:
	ldrb	r0, [r6, #1645]	@ zero_extendqisi2
	bfi	r0, r2, #0, #4
	mov	r2, r0
	bfc	r2, #4, #1
	strb	r2, [r6, #1645]
	ldr	r2, [sp, #44]
	ldr	r2, [r2, #44]
	cmp	r2, #2
	ldrb	r2, [r6, #1645]	@ zero_extendqisi2
	bne	.L435
.L831:
	movs	r0, #1
.L815:
	bfi	r2, r0, #5, #2
	strb	r2, [r6, #1645]
	ldrh	r2, [r6, #1624]
	ldr	r7, .L838+24
	bic	r2, r2, #1016
	orr	r0, r2, #256
	ldr	r2, [r6, #1628]
	strh	r0, [r6, #1624]	@ movhi
	ands	r7, r7, r2
	ldr	r2, .L838+28
	orrs	r2, r2, r7
	ldr	r7, .L838+32
	str	r2, [r6, #1628]
	ldr	r2, [r6, #1632]
	ands	r7, r7, r2
	ldrh	r2, [r6, #1646]
	str	r7, [r6, #1632]
	and	r2, r2, #14
	orr	r2, r2, #1
	strh	r2, [r6, #1646]	@ movhi
	cmp	r3, #0
	bne	.L438
	b	.L839
.L840:
	.align	2
.L838:
	.word	hal_h264e_debug
	.word	.LANCHOR0+463
	.word	.LC29
	.word	.LANCHOR0+484
	.word	.LC28
	.word	.LANCHOR0+508
	.word	-536346112
	.word	1049601
	.word	-267648256
.L839:
.L439:
	ldrh	r3, [r6, #1648]
	cmp	r1, #0
	eor	r1, r1, #1
	bfc	r3, #0, #12
	strh	r3, [r6, #1648]	@ movhi
	ldrh	r3, [r6, #1650]
	itet	ne
	movne	r2, #48
	moveq	r2, #24
	movne	r0, #0
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #1650]	@ movhi
	ldrh	r3, [r6, #1652]
	itee	ne
	movne	r2, #64
	moveq	r2, #48
	moveq	r0, #96
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #1652]	@ movhi
	ldrh	r3, [r6, #1654]
	ite	ne
	movne	r2, #72
	moveq	r2, #64
	bfi	r3, r2, #0, #12
	ldrh	r2, [r6, #1656]
	strh	r3, [r6, #1654]	@ movhi
	movw	r3, #4095
	it	eq
	moveq	r3, #16
	bfi	r2, r3, #0, #12
	ldrh	r3, [r6, #1658]
	strh	r2, [r6, #1656]	@ movhi
	mov	r2, #32
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #1658]	@ movhi
	ldrh	r3, [r6, #1660]
	mov	r2, #48
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #1660]	@ movhi
	ldrh	r3, [r6, #1662]
	bfi	r3, r0, #0, #12
	strh	r3, [r6, #1662]	@ movhi
	ldr	r3, [r6, #1664]
	ite	ne
	movne	r0, #64
	moveq	r0, #96
	and	r3, r3, #-268374016
	orr	r3, r3, #1572864
	orr	r3, r3, r2
	str	r3, [r6, #1664]
	ldrh	r3, [r6, #1668]
	bfi	r3, r0, #0, #12
	strh	r3, [r6, #1668]	@ movhi
	ldrh	r3, [r6, #1670]
	ldr	r0, .L841
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #1670]	@ movhi
	ldrb	r3, [r6, #1672]	@ zero_extendqisi2
	ite	ne
	movne	r2, #2
	moveq	r2, #1
	bfi	r3, r2, #0, #4
	strb	r3, [r6, #1672]
	ldrb	r3, [r6, #1673]	@ zero_extendqisi2
	ite	ne
	movne	r2, #5
	moveq	r2, #3
	bfi	r3, r2, #0, #4
	strb	r3, [r6, #1673]
	ldrh	r3, [r6, #1674]
	movw	r2, #769
	and	r3, r3, r0
	orr	r3, r3, r2
	strh	r3, [r6, #1674]	@ movhi
	ldr	r3, [r6, #1676]
	and	r3, r3, #-252645136
	orr	r3, r3, #67109888
	orr	r3, r3, #65537
	str	r3, [r6, #1676]
	ldr	r3, [r6, #1680]
	and	r3, r3, #-252645136
	orr	r3, r3, #50332416
	str	r3, [r6, #1680]
	ldrh	r3, [r6, #1684]
	and	r0, r0, r3
	ldr	r3, [r6, #1688]
	orr	r2, r2, r0
	strh	r2, [r6, #1684]	@ movhi
	and	r3, r3, #-1061109568
	it	ne
	movne	r0, #0
	orr	r3, r3, #150997248
	it	eq
	moveq	r0, #5
	orr	r3, r3, #393222
	str	r3, [r6, #1688]
	ldr	r3, [r6, #1692]
	and	r2, r3, #-1061109568
	ldr	r3, .L841+4
	orr	r3, r3, r2
	str	r3, [r6, #1692]
	ldr	r3, [r6, #1696]
	and	r2, r3, #-1061109568
	ldr	r3, .L841+8
	orr	r3, r3, r2
	str	r3, [r6, #1696]
	ldrh	r3, [r6, #1700]
	ite	ne
	movne	r2, #0
	moveq	r2, #10
	strb	r2, [r6, #1704]
	bic	r3, r3, #16128
	bic	r3, r3, #63
	orr	r3, r3, #4096
	orr	r3, r3, #7
	strh	r3, [r6, #1700]	@ movhi
	ite	ne
	movne	r3, #0
	moveq	r3, #6
	strb	r3, [r6, #1706]
	ldrb	r3, [r6, #1708]	@ zero_extendqisi2
	bfi	r3, r1, #0, #4
	bfi	r3, r0, #4, #4
	strb	r3, [r6, #1708]
	ldrb	r3, [r6, #1709]	@ zero_extendqisi2
	bfi	r3, r1, #0, #4
	ite	ne
	movne	r1, #0
	moveq	r1, #3
	bfi	r3, r1, #4, #4
	strb	r3, [r6, #1709]
	it	ne
	movne	r1, #0
	ldrb	r3, [r6, #1710]	@ zero_extendqisi2
	it	eq
	moveq	r1, #9
	bfi	r3, r1, #0, #4
	bfi	r3, r2, #4, #4
	ldr	r2, [r6, #1740]
	strb	r3, [r6, #1710]
	and	r2, r2, #-268374016
	ldr	r3, [r6, #1736]
	orr	r2, r2, #1638400
	orr	r2, r2, #15
	str	r2, [r6, #1740]
	ldr	r2, .L841+12
	and	r3, r3, #-268374016
	orr	r3, r3, #655360
	orr	r3, r3, #1
	str	r3, [r6, #1736]
	str	r2, [r6, #1744]
	movs	r2, #16
	strb	r2, [r6, #1748]
	ldr	r2, [r5, #968]
	cbz	r2, .L454
	and	r3, r3, #-268374016
	orr	r3, r3, #458752
	str	r3, [r6, #1736]
	mov	r3, #3600
	strh	r3, [r6, #1744]	@ movhi
.L454:
	ldr	r3, [r6, #1764]
	movs	r0, #72
	ldr	r1, .L841+16
	and	r2, r3, #-268374016
	ldrh	r3, [r6, #1768]
	orrs	r2, r2, r1
	str	r2, [r6, #1764]
	bfi	r3, r0, #0, #12
	strh	r3, [r6, #1768]	@ movhi
	ldr	r3, [r6, #1788]
	mov	r2, #269488144
	str	r2, [r6, #1772]
	and	r3, r3, #-268374016
	orrs	r3, r3, r1
	str	r3, [r6, #1788]
	ldrh	r3, [r6, #1792]
	bfi	r3, r0, #0, #12
	strh	r3, [r6, #1792]	@ movhi
	ldr	r3, .L841+20
	str	r3, [r6, #1796]
	ldr	r3, [r6, #1804]
	and	r3, r3, #-252645136
	orr	r3, r3, #67109888
	orr	r3, r3, #65537
	str	r3, [r6, #1804]
	ldr	r3, [r6, #1808]
	and	r1, r3, #-1061109568
	ldr	r3, .L841+24
	orrs	r3, r3, r1
	str	r3, [r6, #1808]
	ldr	r3, [r5]
	add	r3, r3, #4736
	ldr	r3, [r3]
	cmp	r3, #1
	ittt	ne
	strne	r2, [r6, #1796]
	movne	r3, #16
	strbne	r3, [r6, #1744]
	ldr	r6, .L841+28
	ldr	r3, [r6]
	lsls	r7, r3, #30
	bpl	.L456
	movw	r2, #1479
	ldr	r1, .L841+32
	ldr	r0, .L841+36
	bl	printk
.L456:
	ldr	r3, [r6]
	ldr	r7, [r5, #308]
	lsls	r0, r3, #30
	bpl	.L457
	movw	r2, #1543
	ldr	r1, .L841+40
	ldr	r0, .L841+44
	bl	printk
.L457:
	ldr	r3, [r5, #164]
	add	r0, r7, #1912
	mov	r2, #2304
	ldr	r3, [r3, #72]
	cmp	r3, #2
	bne	.L458
	ldr	r1, .L841+48
.L816:
	bl	memcpy
	ldr	r3, [r6]
	lsls	r1, r3, #30
	bpl	.L461
	mov	r2, #1552
	ldr	r1, .L841+40
	ldr	r0, .L841+36
	bl	printk
.L461:
	ldr	r3, [sp, #32]
	ldr	r7, [fp, #60]
	ldr	r1, [fp, #64]
	ldr	r3, [r3, #4]
	muls	r1, r7, r1
	ldr	r2, [r3, #172]
	str	r2, [sp, #44]
	ldr	r2, [r3, #180]
	str	r2, [sp, #64]
	ldr	r2, [r3, #176]
	ldr	r3, [r3, #160]
	str	r2, [sp, #68]
	lsls	r0, r3, #4
	str	r3, [sp, #84]
	bl	__aeabi_idiv
	ldr	r2, .L841+28
	ldr	r6, [r5, #308]
	ldr	fp, [r5]
	ldr	r2, [r2]
	tst	r2, #256
	mov	r8, r0
	ldr	r3, [sp, #84]
	beq	.L462
	ldr	r2, [sp, #68]
	ldr	r1, .L841+52
	ldr	r0, .L841+56
	str	r2, [sp, #8]
	ldr	r2, [sp, #44]
	str	r2, [sp, #4]
	ldr	r2, [sp, #64]
	str	r2, [sp]
	movw	r2, #1575
	bl	printk
.L462:
	cmp	r8, #1048576
	it	ge
	movge	r8, #327680
	mul	r3, r8, r7
	asrs	r3, r3, #4
	add	r3, r3, r3, lsl #2
	cmp	r3, #0
	mov	r2, r3
	it	lt
	addlt	r2, r3, #15
	cmp	r3, #0
	asr	r2, r2, #4
	it	lt
	addlt	r3, r3, #15
	rsbs	r2, r2, #0
	str	r2, [sp, #84]
	ldr	r2, .L841+28
	asrs	r3, r3, #4
	ldr	r2, [r2]
	lsls	r2, r2, #30
	bpl	.L466
	mov	r2, #1584
	ldr	r1, .L841+52
	ldr	r0, .L841+44
	str	r3, [sp, #96]
	bl	printk
	ldr	r3, [sp, #96]
.L466:
	ldr	r1, [sp, #44]
	ldrb	r2, [r6, #245]	@ zero_extendqisi2
	bfi	r2, r1, #0, #6
	strb	r2, [r6, #245]
	ldrb	r2, [r6, #324]	@ zero_extendqisi2
	ldrb	r1, [r6, #330]	@ zero_extendqisi2
	bic	r2, r2, #7
	orr	r2, r2, #3
	strb	r2, [r6, #324]
	ldr	r2, [r6, #324]
	bfi	r2, r7, #12, #20
	str	r2, [r6, #324]
	ldr	r2, [r9, #44]
	cmp	r2, #2
	ite	eq
	ldreq	r2, [r10, #312]
	ldrne	r2, [r10, #308]
	and	r2, r2, #15
	bfi	r1, r2, #0, #4
	strb	r1, [r6, #330]
	ldr	r2, [r5, #968]
	cbz	r2, .L469
	uxtb	r2, r1
	bfc	r2, #0, #4
	strb	r2, [r6, #330]
	b	.L842
.L843:
	.align	2
.L841:
	.word	-3856
	.word	134613766
	.word	335942665
	.word	269488148
	.word	2621460
	.word	269752603
	.word	134746128
	.word	hal_h264e_debug
	.word	.LANCHOR0+524
	.word	.LC29
	.word	.LANCHOR0+547
	.word	.LC28
	.word	vepu540c_h264_customer_scl_tab
	.word	.LANCHOR0+570
	.word	.LC32
.L842:
.L469:
	ldr	r2, [sp, #68]
	ldrh	r1, [r6, #330]
	and	r7, r2, #63
	bfi	r1, r7, #4, #6
	strh	r1, [r6, #330]	@ movhi
	ldr	r2, [sp, #64]
	ubfx	r1, r1, #8, #8
	and	r2, r2, #63
	bfi	r1, r2, #2, #6
	strb	r1, [r6, #331]
	ldr	r1, [r9, #44]
	cmp	r1, #2
	ittee	eq
	ldreq	r0, [fp, #248]
	ldreq	r1, [fp, #256]
	ldrne	r0, [fp, #252]
	ldrne	r1, [fp, #260]
	cmp	r0, r1
	bne	.L471
	ldrb	r1, [r6, #245]	@ zero_extendqisi2
	bfi	r1, r0, #0, #6
	strb	r1, [r6, #245]
	ldrb	r1, [r6, #330]	@ zero_extendqisi2
	bfc	r1, #0, #4
	strb	r1, [r6, #330]
.L471:
	ldr	r1, [r6, #332]
	bfi	r1, r8, #0, #20
	str	r1, [r6, #332]
	ldr	r1, [r6, #632]
	and	r0, r1, #-33554432
	ldr	r1, .L844
	orrs	r1, r1, r0
	str	r1, [r6, #632]
	ldr	r1, [r6, #636]
	lsrs	r1, r1, #20
	lsls	r1, r1, #20
	str	r1, [r6, #636]
	ldr	r1, [sp, #84]
	lsls	r1, r1, #2
	str	r1, [r6, #640]
	ldr	r1, [sp, #84]
	strd	r1, r3, [r6, #644]
	lsls	r3, r3, #2
	str	r3, [r6, #652]
	mvn	r3, #-2147483648
	strd	r3, r3, [r6, #656]
	strd	r3, r3, [r6, #664]
	str	r3, [r6, #672]
	ldrb	r3, [r6, #680]	@ zero_extendqisi2
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #680]
	ldrh	r3, [r6, #680]
	bfi	r3, r7, #6, #6
	strh	r3, [r6, #680]	@ movhi
	ldr	r3, [r6, #680]
	bfi	r3, r2, #12, #6
	str	r3, [r6, #680]
	ubfx	r1, r3, #16, #8
	ubfx	r3, r3, #24, #8
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #683]
	ldrb	r3, [r6, #684]	@ zero_extendqisi2
	bfi	r1, r7, #2, #6
	strb	r1, [r6, #682]
	bfi	r3, r7, #0, #6
	strb	r3, [r6, #684]
	ldrh	r3, [r6, #684]
	bfi	r3, r2, #6, #6
	strh	r3, [r6, #684]	@ movhi
	ldr	r3, [r6, #684]
	bfi	r3, r7, #12, #6
	str	r3, [r6, #684]
	ubfx	r1, r3, #16, #8
	ubfx	r3, r3, #24, #8
	bfi	r1, r2, #2, #6
	bfi	r3, r7, #0, #6
	strb	r1, [r6, #686]
	strb	r3, [r6, #687]
	ldrb	r3, [r6, #688]	@ zero_extendqisi2
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #688]
	ldrh	r3, [r6, #688]
	bfi	r3, r7, #6, #6
	strh	r3, [r6, #688]	@ movhi
	ldr	r3, [r6, #688]
	bfi	r3, r2, #12, #6
	str	r3, [r6, #688]
	ubfx	r1, r3, #16, #8
	ubfx	r3, r3, #24, #8
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #691]
	ldrb	r3, [r6, #692]	@ zero_extendqisi2
	movs	r2, #1
	bfi	r1, r7, #2, #6
	strb	r1, [r6, #690]
	bfi	r3, r7, #0, #6
	strb	r3, [r6, #692]
	ldrb	r3, [r6, #695]	@ zero_extendqisi2
	bfi	r3, r2, #6, #2
	strb	r3, [r6, #695]
	ldr	r3, .L844+4
	ldr	r3, [r3]
	lsls	r6, r3, #30
	bpl	.L472
	movw	r2, #1655
	ldr	r1, .L844+8
	ldr	r0, .L844+12
	bl	printk
.L472:
	ldr	r3, [r5, #4]
	ldr	r6, [r5, #308]
	str	r3, [sp, #84]
	ldr	r3, [sp, #32]
	ldr	fp, [r3, #52]
	ldr	r8, [r3, #28]
	mov	r0, fp
	bl	mpp_frame_get_buffer
	ldr	r3, [sp, #32]
	str	r0, [sp, #96]
	mov	r0, fp
	ldr	r7, [r3, #32]
	bl	mpp_frame_get_fmt
	str	r0, [sp, #108]
	mov	r0, fp
	bl	mpp_frame_get_hor_stride
	str	r0, [sp, #44]
	mov	r0, fp
	bl	mpp_frame_get_ver_stride
	str	r0, [sp, #64]
	mov	r0, r8
	bl	mpp_packet_get_length
	ldr	r3, [r7, #24]
	str	r3, [sp, #100]
	ldr	r3, [sp, #32]
	str	r0, [sp, #68]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_is_full
	ldr	r2, .L844+4
	ldr	r2, [r2]
	tst	r2, #2
	str	r0, [sp, #104]
	ldr	r3, [sp, #108]
	beq	.L473
	movw	r2, #1675
	ldr	r1, .L844+16
	ldr	r0, .L844+20
	bl	printk
	ldr	r3, [sp, #108]
.L473:
	ands	r8, r3, #15728640
	beq	.L474
	mov	r0, fp
	bl	mpp_frame_get_fbc_offset
	mov	r8, r0
.L819:
	mov	fp, #0
.L475:
	ldr	r3, [r5, #956]
	ldr	r2, [sp, #104]
	orrs	r3, r2, r3
	beq	.L484
	movs	r3, #0
	strd	r3, r3, [r6, #116]
	str	r3, [r6, #124]
.L485:
	ldr	r3, [r7, #28]
	ldr	r0, [r7]
	cmp	r3, #0
	bne	.L486
	cmp	r0, #0
	beq	.L487
	mov	r1, r0
	movs	r2, #173
	ldr	r0, [sp, #84]
	bl	mpp_dev_get_iova_address
	str	r0, [r6, #168]
.L488:
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #168]
	add	r3, r3, r2
	ldr	r2, [sp, #100]
	str	r3, [r6, #168]
	subs	r2, r2, #1
	str	r3, [r6, #176]
	add	r2, r2, r3
	str	r2, [r6, #164]
	ldr	r2, [sp, #68]
	add	r3, r3, r2
	str	r3, [r6, #172]
.L489:
	ldr	r3, [sp, #68]
	cbz	r3, .L490
	ldr	r3, [sp, #32]
	ldr	r3, [r3, #32]
	ldr	r2, [r3]
	cmp	r2, #0
	beq	.L491
	ldr	r2, [sp, #68]
	ldr	r1, .L844+24
	str	r2, [r3, #20]
	ldr	r3, [sp, #32]
	ldr	r0, [r3, #32]
	bl	mpp_buffer_flush_for_device_with_caller
.L490:
	ldr	r3, .L844+4
	ldr	r3, [r3]
	lsls	r0, r3, #30
	bpl	.L492
	movw	r2, #1771
	ldr	r1, .L844+16
	ldr	r0, .L844+12
	bl	printk
.L492:
	ldr	r3, [r5, #956]
	ldr	r2, [sp, #92]
	orrs	r3, r2, r3
	beq	.L493
	ldr	r3, [sp, #32]
	ldr	r6, [r3, #52]
	mov	r0, r6
	bl	mpp_frame_get_is_full
	cmp	r0, #0
	bne	.L494
	ldrb	r3, [r4, #96]	@ zero_extendqisi2
	and	r3, r3, #8
	orn	r3, r3, #124
	strb	r3, [r4, #96]
	ldrb	r3, [r4, #27]	@ zero_extendqisi2
	bic	r3, r3, #7
	orr	r3, r3, #7
	strb	r3, [r4, #27]
	ldr	r3, [r4, #252]
	bic	r3, r3, #458752
	bic	r3, r3, #255
	orr	r3, r3, #327680
	str	r3, [r4, #252]
.L493:
	ldr	r3, [r5, #4]
	ldr	r8, [r5, #172]
	ldr	r7, [r5, #44]
	str	r3, [sp, #44]
	ldrd	r6, r3, [r5, #12]
	ldr	r1, [r8, #4]
	mov	r0, r6
	str	r3, [sp, #64]
	bl	hal_bufs_get_buf
	ldr	r1, [r8, #20]
	mov	fp, r0
	mov	r0, r6
	bl	hal_bufs_get_buf
	ldr	r3, [sp, #32]
	mov	r8, r0
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_ppinfo
	ldr	r3, .L844+4
	ldr	r3, [r3]
	lsls	r1, r3, #30
	bpl	.L497
	movw	r2, #1787
	ldr	r1, .L844+28
	ldr	r0, .L844+20
	bl	printk
.L497:
	cmp	fp, #0
	beq	.L499
	ldr	r3, [fp]
	cmp	r3, #0
	beq	.L499
	ldr	r3, [fp, #4]
	ldr	r6, [r3]
	cbnz	r6, .L501
	movw	r1, #1791
	ldr	r3, .L844+32
	str	r1, [sp]
	ldr	r2, .L844+36
	ldr	r0, .L844+40
	bl	printk
.L501:
	movs	r2, #169
	mov	r1, r6
	ldr	r0, [sp, #44]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #152]
	cmp	r7, #0
	bne	.L499
	ldr	r3, [fp, #4]
	ldr	r3, [r3, #12]
	str	r3, [sp, #68]
	cbnz	r3, .L502
	movw	r1, #1796
	ldr	r3, .L844+32
	str	r1, [sp]
	ldr	r2, .L844+44
	ldr	r0, .L844+40
	bl	printk
.L502:
	movs	r2, #163
	ldr	r1, [sp, #68]
	ldr	r0, [sp, #44]
	bl	mpp_dev_get_iova_address
	movs	r2, #169
	mov	r1, r6
	ldr	r3, [sp, #64]
	str	r0, [r4, #128]
	add	r0, r0, r3
	str	r0, [r4, #132]
	ldr	r0, [sp, #44]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #152]
	b	.L845
.L846:
	.align	2
.L844:
	.word	2130942
	.word	hal_h264e_debug
	.word	.LANCHOR0+570
	.word	.LC29
	.word	.LANCHOR0+593
	.word	.LC28
	.word	.LANCHOR0+615
	.word	.LANCHOR0+637
	.word	.LANCHOR0+662
	.word	.LC34
	.word	.LC35
	.word	.LC36
.L845:
.L499:
	cmp	r8, #0
	beq	.L504
	ldr	r3, [r8]
	cbz	r3, .L504
	ldr	r3, [r8, #4]
	ldr	r6, [r3]
	cbnz	r6, .L506
	movw	r1, #1809
	ldr	r3, .L847
	str	r1, [sp]
	ldr	r2, .L847+4
	ldr	r0, .L847+8
	bl	printk
.L506:
	movs	r2, #170
	mov	r1, r6
	ldr	r0, [sp, #44]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #156]
	cbnz	r7, .L504
	ldr	r3, [r8, #4]
	ldr	r6, [r3, #12]
	cbnz	r6, .L507
	movw	r1, #1814
	ldr	r3, .L847
	str	r1, [sp]
	ldr	r2, .L847+12
	ldr	r0, .L847+8
	bl	printk
.L507:
	movs	r2, #165
	mov	r1, r6
	ldr	r0, [sp, #44]
	bl	mpp_dev_get_iova_address
	ldr	r3, [sp, #64]
	str	r0, [r4, #136]
	add	r3, r3, r0
	str	r3, [r4, #140]
.L504:
	ldr	r3, [r5, #44]
	cmp	r3, #0
	beq	.L508
	ldr	r7, [r5, #172]
	movs	r2, #163
	ldr	r1, [r5, #48]
	ldr	r0, [r5, #4]
	ldr	r3, [r7, #8]
	str	r3, [sp, #64]
	ldr	r3, [r7, #24]
	str	r3, [sp, #84]
	ldr	r3, [r7, #12]
	str	r3, [sp, #68]
	bl	mpp_dev_get_iova_address
	ldr	r3, [r7, #16]
	mov	r6, r0
	cmp	r3, #0
	beq	.L509
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #20]
	cmp	r2, r3
	bne	.L509
	ldr	r3, .L847+16
	ldr	r3, [r3]
	lsls	r2, r3, #24
	bpl	.L510
	ldr	r3, [sp, #64]
	mov	r2, #390
	ldr	r1, .L847+20
	ldr	r0, .L847+24
	bl	printk
.L510:
	ldr	r3, [r5, #76]
	str	r3, [sp, #52]
	ldr	r3, [r5, #80]
	str	r3, [sp, #48]
	ldr	r3, [r5, #124]
	str	r3, [sp, #60]
	ldr	r3, [r5, #128]
	str	r3, [sp, #56]
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq	.L511
	ldr	r3, [r5, #60]
	ldr	r2, [sp, #52]
	str	r3, [sp, #36]
	ldr	r3, [r5, #108]
	str	r3, [sp, #40]
	ldr	r3, [r5, #52]
	cmp	r2, r3
	it	cs
	movcs	r2, r3
	ldr	r3, [r5, #56]
	str	r2, [sp, #52]
	ldr	r2, [sp, #48]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [r5, #100]
	str	r2, [sp, #48]
	ldr	r2, [sp, #60]
	cmp	r2, r3
	it	cs
	movcs	r2, r3
	ldr	r3, [r5, #104]
	str	r2, [sp, #60]
	ldr	r2, [sp, #56]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [sp, #40]
	str	r2, [sp, #56]
.L820:
	str	r3, [sp, #76]
	ldr	r3, [sp, #36]
	str	r3, [sp, #72]
.L512:
	ldr	r3, [sp, #36]
	ldrb	r2, [r4, #244]	@ zero_extendqisi2
	add	r3, r3, r6
	str	r3, [r4, #128]
	ldr	r3, [sp, #40]
	add	r3, r3, r6
	str	r3, [r4, #132]
	ldr	r3, [sp, #72]
	add	r3, r3, r6
	str	r3, [r4, #136]
	ldr	r3, [sp, #76]
	add	r3, r3, r6
	str	r3, [r4, #140]
	ldr	r3, [sp, #48]
	add	r3, r3, r6
	str	r3, [r4, #196]
	ldr	r3, [sp, #52]
	add	r3, r3, r6
	str	r3, [r4, #200]
	ldr	r3, [sp, #56]
	add	r3, r3, r6
	str	r3, [r4, #204]
	ldr	r3, [sp, #60]
	add	r6, r6, r3
	ldr	r3, [sp, #68]
	str	r6, [r4, #208]
	clz	r3, r3
	ldr	r6, .L847+16
	lsrs	r3, r3, #5
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #244]
	ldr	r2, [r6]
	lsls	r7, r2, #24
	bpl	.L598
	mov	r2, #500
	ldr	r1, .L847+20
	ldr	r0, .L847+28
	bl	printk
	ldr	r3, [r6]
	lsls	r0, r3, #24
	bpl	.L598
	ldr	r3, [r5, #88]
	mov	r2, #502
	ldr	r1, .L847+20
	ldr	r0, .L847+32
	str	r3, [sp, #12]
	ldr	r3, [r5, #84]
	str	r3, [sp, #8]
	ldr	r3, [r5, #76]
	str	r3, [sp, #4]
	ldr	r3, [r5, #80]
	str	r3, [sp]
	ldr	r3, [r5, #92]
	bl	printk
	ldr	r3, [r6]
	lsls	r1, r3, #24
	bpl	.L598
	ldr	r3, [r5, #136]
	mov	r2, #504
	ldr	r1, .L847+20
	ldr	r0, .L847+36
	str	r3, [sp, #12]
	ldr	r3, [r5, #132]
	str	r3, [sp, #8]
	ldr	r3, [r5, #124]
	str	r3, [sp, #4]
	ldr	r3, [r5, #128]
	str	r3, [sp]
	ldr	r3, [r5, #140]
	bl	printk
.L598:
	ldr	r3, [sp, #68]
	cbnz	r3, .L528
	ldr	r3, [r5, #84]
	ldr	r2, [r5, #92]
	str	r3, [r5, #88]
	add	r3, r3, r2
	ldr	r2, [r5, #80]
	cmp	r3, r2
	ittt	cs
	subcs	r3, r3, r2
	ldrcs	r2, [r5, #76]
	addcs	r3, r3, r2
	str	r3, [r5, #84]
	ldr	r3, [r5, #132]
	ldr	r2, [r5, #140]
	str	r3, [r5, #136]
	add	r3, r3, r2
	ldr	r2, [r5, #128]
	cmp	r3, r2
	ittt	cs
	subcs	r3, r3, r2
	ldrcs	r2, [r5, #124]
	addcs	r3, r3, r2
	str	r3, [r5, #132]
.L528:
	ldr	r3, [fp, #4]
	movs	r2, #185
	ldr	r0, [sp, #44]
	ldr	r6, .L847+16
	ldr	r1, [r3, #8]
	bl	mpp_dev_get_iova_address
	movs	r2, #184
	str	r0, [r4, #216]
	ldr	r3, [r8, #4]
	ldr	r0, [sp, #44]
	ldr	r1, [r3, #8]
	bl	mpp_dev_get_iova_address
	ldr	r3, [r6]
	lsls	r2, r3, #30
	str	r0, [r4, #212]
	bpl	.L531
	movw	r2, #1836
	ldr	r1, .L847+40
	ldr	r0, .L847+44
	bl	printk
.L531:
	ldr	r3, [sp, #32]
	ldr	r1, [r3, #64]
	cmp	r1, #0
	beq	.L636
	movs	r2, #171
	ldr	r0, [r5, #4]
	bl	mpp_dev_get_iova_address
.L532:
	ldr	r3, [sp, #32]
	str	r0, [r4, #160]
	ldrb	r2, [r4, #244]	@ zero_extendqisi2
	ldr	r3, [r3, #64]
	adds	r3, r3, #0
	it	ne
	movne	r3, #1
	bfi	r2, r3, #3, #1
	ldr	r3, [sp, #32]
	strb	r2, [r4, #244]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_offset_y
	ldrh	r3, [r4, #294]
	bfi	r3, r0, #0, #14
	strh	r3, [r4, #294]	@ movhi
	ldr	r3, [sp, #32]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_offset_x
	ldrh	r3, [r4, #292]
	bfi	r3, r0, #0, #14
	strh	r3, [r4, #292]	@ movhi
	ldr	r3, [r6]
	lsls	r3, r3, #30
	bpl	.L533
	movw	r2, #1842
	ldr	r1, .L847+48
	ldr	r0, .L847+52
	bl	printk
.L533:
	ldr	r3, [r10, #3432]
	cmp	r3, #1
	beq	.L534
	cmp	r3, #0
	beq	.L535
	cmp	r3, #2
	beq	.L536
	movw	r2, #1884
	ldr	r1, .L847+48
	ldr	r0, .L847+56
	bl	printk
	b	.L538
.L377:
	ldr	r3, [sp, #124]
	b	.L811
.L374:
	ldr	r3, [r4, #440]
	bic	r3, r3, #7
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	str	r3, [r4, #440]
	b	.L378
.L848:
	.align	2
.L847:
	.word	.LANCHOR0+662
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	hal_h264e_debug
	.word	.LANCHOR0+687
	.word	.LC37
	.word	.LC41
	.word	.LC42
	.word	.LC43
	.word	.LANCHOR0+637
	.word	.LC29
	.word	.LANCHOR0+724
	.word	.LC28
	.word	.LC44
.L379:
	ldr	r0, [r9, #92]
	bl	h264e_marking_is_empty
	mov	r6, r0
	cmp	r0, #0
	bne	.L380
	ldrb	r3, [r4, #444]	@ zero_extendqisi2
	add	r1, sp, #116
	orr	r3, r3, #4
	strb	r3, [r4, #444]
	ldr	r0, [r9, #92]
	bl	h264e_marking_rd_op
	ldr	r3, [sp, #116]
	subs	r2, r3, #1
	cmp	r2, #5
	bhi	.L381
	tbb	[pc, r2]
.L383:
	.byte	(.L388-.L383)/2
	.byte	(.L387-.L383)/2
	.byte	(.L386-.L383)/2
	.byte	(.L385-.L383)/2
	.byte	(.L604-.L383)/2
	.byte	(.L382-.L383)/2
	.p2align 1
.L388:
	ldr	r2, [sp, #120]
.L384:
	ldrb	r1, [r4, #444]	@ zero_extendqisi2
	bfi	r1, r3, #4, #3
	strb	r1, [r4, #444]
	ldr	r3, [r4, #444]
	bfi	r3, r2, #7, #16
	str	r3, [r4, #444]
	ldrb	r3, [r4, #452]	@ zero_extendqisi2
	bfi	r3, r6, #0, #4
	strb	r3, [r4, #452]
	ldr	r0, [r9, #92]
	bl	h264e_marking_is_empty
	mov	r6, r0
	cmp	r0, #0
	bne	.L380
	add	r1, sp, #116
	ldr	r0, [r9, #92]
	bl	h264e_marking_rd_op
	ldr	r3, [sp, #116]
	subs	r2, r3, #1
	cmp	r2, #5
	bhi	.L391
	tbb	[pc, r2]
.L393:
	.byte	(.L398-.L393)/2
	.byte	(.L397-.L393)/2
	.byte	(.L396-.L393)/2
	.byte	(.L395-.L393)/2
	.byte	(.L605-.L393)/2
	.byte	(.L392-.L393)/2
	.p2align 1
.L387:
	ldr	r2, [sp, #124]
	b	.L384
.L386:
	ldr	r2, [sp, #120]
	ldr	r6, [sp, #128]
	b	.L384
.L385:
	ldr	r2, [sp, #132]
	b	.L384
.L382:
	ldr	r2, [sp, #128]
	b	.L384
.L381:
	movw	r2, #1084
	ldr	r1, .L849
	ldr	r0, .L849+4
	bl	printk
	movs	r3, #0
.L604:
	movs	r2, #0
	b	.L384
.L398:
	ldr	r2, [sp, #120]
.L394:
	ldrh	r1, [r4, #446]
	strh	r2, [r4, #448]	@ movhi
	bfi	r1, r3, #7, #3
	ldrb	r3, [r4, #452]	@ zero_extendqisi2
	strh	r1, [r4, #446]	@ movhi
	bfi	r3, r6, #4, #4
	strb	r3, [r4, #452]
	ldr	r0, [r9, #92]
	bl	h264e_marking_is_empty
	mov	r6, r0
	cmp	r0, #0
	bne	.L380
	add	r1, sp, #116
	ldr	r0, [r9, #92]
	bl	h264e_marking_rd_op
	ldr	r3, [sp, #116]
	subs	r2, r3, #1
	cmp	r2, #5
	bhi	.L400
	tbb	[pc, r2]
.L402:
	.byte	(.L407-.L402)/2
	.byte	(.L406-.L402)/2
	.byte	(.L405-.L402)/2
	.byte	(.L404-.L402)/2
	.byte	(.L606-.L402)/2
	.byte	(.L401-.L402)/2
	.p2align 1
.L397:
	ldr	r2, [sp, #124]
	b	.L394
.L396:
	ldr	r2, [sp, #120]
	ldr	r6, [sp, #128]
	b	.L394
.L395:
	ldr	r2, [sp, #132]
	b	.L394
.L392:
	ldr	r2, [sp, #128]
	b	.L394
.L391:
	movw	r2, #1139
	ldr	r1, .L849
	ldr	r0, .L849+4
	bl	printk
	movs	r3, #0
.L605:
	movs	r2, #0
	b	.L394
.L407:
	ldr	r2, [sp, #120]
.L403:
	ldrb	r1, [r4, #447]	@ zero_extendqisi2
	strh	r2, [r4, #450]	@ movhi
	bfi	r1, r3, #2, #3
	ldrb	r3, [r4, #453]	@ zero_extendqisi2
	strb	r1, [r4, #447]
	bfi	r3, r6, #0, #4
	strb	r3, [r4, #453]
	b	.L380
.L406:
	ldr	r2, [sp, #124]
	b	.L403
.L405:
	ldr	r2, [sp, #120]
	ldr	r6, [sp, #128]
	b	.L403
.L404:
	ldr	r2, [sp, #132]
	b	.L403
.L401:
	ldr	r2, [sp, #128]
	b	.L403
.L400:
	movw	r2, #1193
	ldr	r1, .L849
	ldr	r0, .L849+4
	bl	printk
	movs	r3, #0
.L606:
	movs	r2, #0
	b	.L403
.L410:
	cmp	r8, #1
	ite	eq
	moveq	r2, #9
	movne	r2, #6
	b	.L812
.L608:
	movs	r3, #0
	b	.L413
.L610:
	movs	r2, #0
	b	.L417
.L611:
	movs	r1, #3
	b	.L419
.L612:
	movs	r1, #2
	b	.L419
.L421:
	ldr	r2, [sp, #64]
	lsrs	r0, r7, #6
	cmp	r2, r0
	ite	cc
	movcc	r2, #9
	movcs	r2, #12
	b	.L813
.L420:
	ldrh	r2, [r6, #1644]
	and	r2, r2, #32512
	orr	r2, r2, #16
	strh	r2, [r6, #1644]	@ movhi
	b	.L422
.L425:
	add	r7, r7, r7, lsl #1
	cmp	r0, r7, lsr #5
	ite	cc
	subcc	r2, r2, #7
	subcs	r2, r2, #6
	b	.L426
.L424:
	cmp	ip, r0, lsr #1
	ite	hi
	movhi	r2, #1
	movls	r2, #0
	cmp	r0, r7, lsr #8
	bcs	.L429
	cmp	r8, #2
	ite	lt
	movlt	r1, #6
	movge	r1, #8
.L830:
	subs	r2, r2, r1
.L814:
	movs	r1, #0
	b	.L428
.L429:
	cmp	r0, r7, lsr #7
	bcs	.L431
	cmp	r8, #2
	ite	lt
	movlt	r1, #5
	movge	r1, #6
	b	.L830
.L431:
	ldr	r1, [sp, #68]
	cmp	r0, r1
	bcs	.L433
	cmp	r8, #2
	ite	lt
	movlt	r1, #3
	movge	r1, #4
	b	.L830
.L433:
	subs	r2, r2, #1
	b	.L814
.L614:
	movs	r2, #0
.L615:
	movs	r1, #1
	b	.L428
.L435:
	ldr	r0, [r5, #636]
	cmp	r0, #2
	beq	.L831
	movs	r0, #2
	b	.L815
.L438:
	ldr	r2, [r3, #8]
	lsls	r7, r2, #31
	bpl	.L439
	orr	r2, r0, #640
	strh	r2, [r6, #1624]	@ movhi
	ldr	r2, [r3, #8]
	ldrb	r0, [r6, #1624]	@ zero_extendqisi2
	asrs	r2, r2, #1
	bfi	r0, r2, #3, #3
	strb	r0, [r6, #1624]
	ldr	r2, [r3, #8]
	ldrh	r0, [r6, #1630]
	asrs	r2, r2, #4
	bfi	r0, r2, #4, #9
	strh	r0, [r6, #1630]	@ movhi
	ldrsh	r2, [r3, #10]
	ldrh	r3, [r6, #1634]
	bfi	r3, r2, #4, #8
	strh	r3, [r6, #1634]	@ movhi
	b	.L439
.L458:
	cmp	r3, #1
	ite	eq
	ldreq	r1, .L849+8
	ldrne	r1, .L849+12
	b	.L816
.L474:
	ands	fp, r3, #983040
	bne	.L819
	ubfx	r2, r3, #0, #20
	cmp	r2, #17
	bhi	.L634
	mov	r1, r3
	add	r0, sp, #116
	bl	vepu541_set_fmt
	ldr	r2, [sp, #116]
	subs	r2, r2, #4
	cmp	r2, #9
	bhi	.L635
	tbb	[pc, r2]
.L478:
	.byte	(.L479-.L478)/2
	.byte	(.L482-.L478)/2
	.byte	(.L479-.L478)/2
	.byte	(.L480-.L478)/2
	.byte	(.L635-.L478)/2
	.byte	(.L635-.L478)/2
	.byte	(.L635-.L478)/2
	.byte	(.L635-.L478)/2
	.byte	(.L479-.L478)/2
	.byte	(.L477-.L478)/2
	.p2align 1
.L482:
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #44]
	mul	fp, r3, r2
	add	r3, fp, fp, lsl #1
	add	r3, r3, r3, lsr #31
	asrs	r3, r3, #1
.L476:
	mov	r8, fp
	mov	fp, r3
	b	.L475
.L480:
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #44]
	mul	fp, r3, r2
	adds	r3, fp, fp, lsl #2
	it	mi
	addmi	r3, r3, #3
	asrs	r3, r3, #2
	b	.L476
.L479:
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #44]
	mul	fp, r3, r2
	mov	r3, fp
	b	.L476
.L477:
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #44]
	mul	fp, r3, r2
	lsl	r3, fp, #1
	b	.L476
.L635:
	movs	r3, #0
	b	.L476
.L634:
	mov	r8, fp
	b	.L475
.L484:
	movs	r2, #160
	ldr	r1, [sp, #96]
	ldr	r0, [sp, #84]
	bl	mpp_dev_get_iova_address
	add	r3, r0, r8
	str	r0, [r6, #116]
	str	r3, [r6, #120]
	add	r0, r0, fp
	str	r0, [r6, #124]
	b	.L485
.L487:
	ldr	r3, [r7, #4]
	str	r3, [r6, #168]
	b	.L488
.L486:
	ldr	r1, .L849+16
	bl	mpp_buffer_get_size_with_caller
	movs	r2, #173
	ldr	r1, [r7]
	mov	r8, r0
	ldr	r0, [sp, #84]
	bl	mpp_dev_get_iova_address
	mov	r1, r8
	str	r0, [r6, #168]
	mov	fp, r0
	ldr	r3, [sp, #68]
	ldr	r0, [r7, #12]
	add	r0, r0, r3
	bl	__aeabi_uidivmod
	add	r0, r8, fp
	add	r1, r1, fp
	str	r1, [r6, #172]
	ldr	r3, [r7, #16]
	str	r0, [r6, #164]
	add	r3, r3, fp
	str	r3, [r6, #176]
	b	.L489
.L491:
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L490
	ldr	r0, [r5, #4]
	bl	mpp_get_dev
	ldr	r3, [sp, #32]
	ldr	r2, [sp, #68]
	ldr	r1, [r3, #32]
	movs	r3, #1
	ldr	r1, [r1, #4]
	bl	dma_sync_single_for_device
	b	.L490
.L850:
	.align	2
.L849:
	.word	.LANCHOR0+463
	.word	.LC31
	.word	vepu540c_h264_jvt_scl_tab
	.word	vepu540c_h264_flat_scl_tab
	.word	.LANCHOR0+615
.L494:
	mov	r0, r6
	bl	mpp_frame_get_phy_addr
	mov	r8, r0
	mov	r0, r6
	bl	mpp_frame_get_hor_stride
	mov	r7, r0
	mov	r0, r6
	bl	mpp_frame_get_ver_stride
	cmp	r8, #0
	bne	.L495
	movw	r1, #2365
	ldr	r0, .L851
	bl	printk
	mov	r3, #-1
	str	r3, [sp, #88]
.L349:
	ldr	r0, [sp, #88]
	add	sp, sp, #148
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L495:
	mla	r0, r0, r7, r8
	str	r8, [r4, #116]
	strd	r0, r0, [r4, #120]
	b	.L493
.L511:
	ldr	r3, [r5, #84]
	str	r3, [sp, #36]
	ldr	r3, [r5, #132]
	str	r3, [sp, #40]
	b	.L820
.L509:
	ldr	r2, [sp, #64]
	ldr	r3, [sp, #84]
	ldr	r1, .L851+4
	add	r3, r3, r2, lsl #1
	add	r3, r1, r3, lsl #2
	ldr	r7, [r3, #708]
	ldr	r3, .L851+8
	ldr	r3, [r3]
	lsls	r3, r3, #24
	bpl	.L513
	mov	r3, r7
	movw	r2, #415
	addw	r1, r1, #687
	ldr	r0, .L851+12
	bl	printk
.L513:
	cmp	r7, #3
	bhi	.L512
	tbb	[pc, r7]
.L515:
	.byte	(.L518-.L515)/2
	.byte	(.L517-.L515)/2
	.byte	(.L516-.L515)/2
	.byte	(.L514-.L515)/2
	.p2align 1
.L518:
	ldr	r3, [r5, #88]
	str	r3, [sp, #72]
	ldr	r3, [r5, #136]
	str	r3, [sp, #76]
	ldr	r3, [r5, #84]
	str	r3, [sp, #36]
	ldr	r3, [r5, #132]
	str	r3, [sp, #40]
	ldr	r3, [r5, #76]
	str	r3, [sp, #52]
	ldr	r3, [r5, #80]
	str	r3, [sp, #48]
	ldr	r3, [r5, #124]
	str	r3, [sp, #60]
	ldr	r3, [r5, #128]
.L821:
	str	r3, [sp, #56]
	b	.L512
.L517:
	ldr	r3, [r5, #60]
	str	r3, [sp, #72]
	ldr	r3, [r5, #108]
	str	r3, [sp, #76]
	ldr	r3, [r5, #76]
	str	r3, [sp, #36]
	str	r3, [r5, #84]
	ldr	r3, [r5, #124]
	ldr	r2, [sp, #36]
	str	r3, [sp, #40]
	str	r3, [r5, #132]
	ldr	r3, [r5, #52]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	ldr	r2, [r5, #56]
	str	r3, [sp, #52]
	ldr	r3, [r5, #80]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [r5, #100]
	str	r2, [sp, #48]
	ldr	r2, [sp, #40]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	ldr	r2, [r5, #104]
	str	r3, [sp, #60]
	ldr	r3, [r5, #128]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	mov	r3, r2
	b	.L821
.L516:
	ldr	r0, .L851+16
	movw	r1, #447
.L822:
	bl	printk
	b	.L512
.L514:
	ldr	r3, [r5, #976]
	cmp	r3, #0
	bne	.L519
	add	r1, r5, #76
	str	r1, [sp, #84]
	ldr	r1, [r5, #60]
	add	r3, r5, #100
	ldr	lr, [sp, #84]
	add	r2, r5, #52
	add	r7, r5, #124
	str	r1, [sp, #72]
	ldr	r1, [r5, #108]
	str	r1, [sp, #76]
	ldr	r1, [r5, #76]
	str	r1, [sp, #36]
	str	r1, [r5, #84]
	ldr	r1, [r5, #124]
	ldr	r0, [sp, #36]
	str	r1, [sp, #40]
	str	r1, [r5, #132]
	ldr	r1, [r5, #52]
	cmp	r1, r0
	it	cs
	movcs	r1, r0
	ldr	r0, [r5, #56]
	str	r1, [sp, #52]
	ldr	r1, [r5, #80]
	cmp	r0, r1
	it	cc
	movcc	r0, r1
	ldr	r1, [r5, #100]
	str	r0, [sp, #48]
	ldr	r0, [sp, #40]
	cmp	r1, r0
	it	cs
	movcs	r1, r0
	ldr	r0, [r5, #104]
	str	r1, [sp, #60]
	ldr	r1, [r5, #128]
	cmp	r0, r1
	it	cc
	movcc	r0, r1
	add	r1, sp, #116
	str	r0, [sp, #56]
	str	r1, [sp, #64]
.L520:
	ldr	r1, [lr, #4]	@ unaligned
	add	lr, lr, #8
	ldr	r0, [lr, #-8]	@ unaligned
	cmp	lr, r3
	ldr	ip, [sp, #64]
	stmia	ip!, {r0, r1}
	ldr	r1, [sp, #64]
	add	r1, r1, #8
	str	r1, [sp, #64]
	bne	.L520
	ldr	ip, [sp, #84]
	add	r0, r2, #24
	mov	r1, r2
.L521:
	ldr	lr, [r1], #4	@ unaligned
	cmp	r1, r0
	str	lr, [ip], #4	@ unaligned
	bne	.L521
	add	lr, sp, #116
.L522:
	mov	ip, lr
	adds	r2, r2, #8
	ldmia	ip!, {r0, r1}
	str	r1, [r2, #-4]	@ unaligned
	add	r1, sp, #140
	cmp	ip, r1
	str	r0, [r2, #-8]	@ unaligned
	mov	lr, ip
	bne	.L522
	add	lr, sp, #116
	mov	r2, r7
	add	r1, r7, #24
	str	r1, [sp, #64]
.L523:
	ldr	r1, [r2, #4]	@ unaligned
	mov	ip, lr
	ldr	r0, [r2]	@ unaligned
	adds	r2, r2, #8
	stmia	ip!, {r0, r1}
	mov	lr, ip
	ldr	r1, [sp, #64]
	cmp	r2, r1
	bne	.L523
	add	r1, r3, #24
	mov	r2, r3
.L524:
	ldr	r0, [r2], #4	@ unaligned
	cmp	r2, r1
	str	r0, [r7], #4	@ unaligned
	bne	.L524
	add	r7, sp, #116
	add	ip, sp, #140
.L525:
	mov	r2, r7
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, ip
	str	r0, [r3, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L525
	b	.L512
.L519:
	movw	r1, #475
	ldr	r0, .L851+20
	b	.L822
.L508:
	mov	r2, #-1
	strd	r2, r3, [r4, #196]
	strd	r2, r3, [r4, #204]
	b	.L528
.L636:
	mov	r0, r1
	b	.L532
.L852:
	.align	2
.L851:
	.word	.LC33
	.word	.LANCHOR0
	.word	hal_h264e_debug
	.word	.LC38
	.word	.LC39
	.word	.LC40
.L535:
	ldr	r1, [r4, #340]
	ldr	r2, .L853
	ands	r2, r2, r1
	str	r2, [r4, #340]
	ldr	r2, [r4, #344]
	bfi	r2, r3, #0, #20
	str	r2, [r4, #344]
	ldr	r2, [r4, #348]
	bfi	r2, r3, #0, #20
	str	r2, [r4, #348]
	ldrb	r2, [r4, #247]	@ zero_extendqisi2
	bfi	r2, r3, #6, #1
	strb	r2, [r4, #247]
.L538:
	movs	r3, #0
	str	r3, [r10, #3428]
	ldr	r3, [r6]
	lsls	r7, r3, #30
	bpl	.L539
	movw	r2, #1891
	ldr	r1, .L853+4
	ldr	r0, .L853+8
	bl	printk
.L539:
	ldrh	r2, [r4, #260]
	movw	r3, #52463
	str	r3, [r4, #356]
	movs	r0, #0
	ubfx	r2, r2, #0, #11
	ldr	r3, [r4, #360]
	movs	r1, #31
	mov	r8, #14
	lsls	r2, r2, #3
	and	r3, r3, #-2013265920
	adds	r2, r2, #71
	orr	r3, r3, #15859712
	orr	r3, r3, #72
	str	r3, [r4, #360]
	asrs	r2, r2, #6
	movw	fp, #1795
	lsls	r2, r2, #1
	adds	r3, r2, #2
	subs	r2, r2, #1
	cmp	r2, r3
	it	ge
	movge	r2, r3
	adds	r3, r2, #1
	ldrh	r2, [r4, #366]
	lsl	lr, r3, #1
	bfi	r2, r3, #2, #10
	strh	r2, [r4, #366]	@ movhi
	movs	r2, #2
.L540:
	mla	r7, r8, r1, r0
	sub	ip, r2, #2
	cmp	r7, fp
	bhi	.L542
	cmp	r2, #32
	bne	.L543
.L544:
	subs	r1, r2, #2
	lsrs	r2, r2, #1
	muls	r3, r1, r3
	ldrh	r1, [r4, #364]
	adds	r3, r3, #28
	lsrs	r3, r3, #1
	bfi	r1, r3, #0, #10
	ldrb	r3, [r4, #367]	@ zero_extendqisi2
	strh	r1, [r4, #364]	@ movhi
	ubfx	r1, r1, #8, #8
	bfc	r3, #4, #2
	bfi	r1, r2, #3, #5
	strb	r3, [r4, #367]
	strb	r1, [r4, #365]
	ldr	r3, [r6]
	lsls	r0, r3, #30
	bpl	.L545
	movw	r2, #2015
	ldr	r1, .L853+12
	ldr	r0, .L853+8
	bl	printk
.L545:
	ldr	r1, [r5, #952]
	cmp	r1, #0
	beq	.L546
	movs	r2, #179
	ldr	r0, [r5, #4]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #192]
	ldr	r3, [r5, #148]
	add	r3, r3, r0
	str	r3, [r4, #188]
.L547:
	ldr	r3, [r5, #304]
	cbz	r3, .L548
	add	r0, r5, #296
	bl	vepu540c_set_osd
.L548:
	ldr	r3, [sp, #32]
	ldr	r3, [r3, #4]
	ldr	fp, [r3, #212]
	cmp	fp, #0
	bne	.L550
	ldr	r3, [r5, #964]
	cmp	r3, #0
	beq	.L550
	ldr	r3, [r5]
	movw	r2, #4744
	ldr	ip, [r3, r2]
	cmp	ip, #3
	bgt	.L550
	add	r3, r3, #4736
	ldr	r3, [r3]
	cmp	r3, #1
	bne	.L550
	ldr	r3, [sp, #32]
	ldr	r7, [r9, #96]
	ldrb	r6, [r4, #245]	@ zero_extendqisi2
	ldrd	r1, r2, [r3, #64]
	ldr	r3, [r3, #72]
	and	r6, r6, #63
	str	r7, [sp, #36]
	ldr	r7, [sp, #80]
	str	r3, [sp, #40]
	ldr	r3, [r5, #968]
	ldrb	r7, [r7, #156]	@ zero_extendqisi2
	ldr	r0, [r5, #308]
	ldrd	lr, r8, [r10, #12]
	ubfx	r7, r7, #2, #1
	add	r0, r0, #760
	str	r3, [sp, #44]
	strd	r7, ip, [sp, #20]
	cmp	r3, #0
	beq	.L552
	ldr	r3, [sp, #36]
	strd	lr, r8, [sp, #4]
	str	r6, [sp]
	strd	fp, r3, [sp, #12]
	ldr	r3, [sp, #40]
	bl	vepu540c_set_qpmap_smart
.L553:
	cbnz	r0, .L550
	ldr	r3, [sp, #32]
	movs	r2, #186
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #68]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #220]
.L550:
	ldr	r1, [r5, #292]
	cbz	r1, .L554
	ldr	r0, [r5, #308]
	ldrd	r2, r3, [r10, #12]
	add	r0, r0, #760
	bl	vepu540c_set_roi
.L554:
	ldr	r3, [sp, #32]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_is_gray
	ldr	r3, [r5, #972]
	cmp	r3, r0
	beq	.L555
	cbnz	r3, .L557
	ldrb	r2, [r4, #1645]	@ zero_extendqisi2
	str	r3, [r4, #1796]
	bfi	r2, r3, #7, #1
	ldrb	r3, [r4, #245]	@ zero_extendqisi2
	strb	r2, [r4, #1645]
	and	r2, r3, #63
	cmp	r2, #29
	bhi	.L558
	movs	r2, #30
.L824:
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #245]
.L557:
	str	r0, [r5, #972]
.L555:
	ldr	r3, [r5]
	movw	r2, #4744
	ldr	r7, .L853+16
	ldr	r6, [r5, #308]
	ldr	fp, [r3, r2]
	movw	r2, #4752
	ldr	r8, [r3, r2]
	movw	r2, #4756
	ldr	r3, [r3, r2]
	str	r3, [sp, #32]
	ldr	r3, [r7]
	lsls	r1, r3, #30
	bpl	.L561
	movw	r2, #2065
	ldr	r1, .L853+20
	ldr	r0, .L853+24
	bl	printk
.L561:
	ldr	r3, [r6, #952]
	add	r0, r6, #1416
	ldr	r2, .L853+28
	lsrs	r3, r3, #20
	lsls	r3, r3, #20
	orrs	r2, r2, r3
	ldr	r3, [sp, #32]
	str	r2, [r6, #952]
	movs	r2, #208
	lsl	ip, r3, #2
	ldr	r3, [r5]
	add	r3, r3, #4736
	ldr	r3, [r3]
	cmp	r3, #1
	ite	eq
	ldreq	r1, .L853+32
	ldrne	r1, .L853+36
	add	r1, r1, ip
	bl	memcpy
	ldr	r3, [r10, #324]
	cbz	r3, .L564
	ldr	r2, [r5]
	ldrh	r3, [r6, #952]
	ldr	r2, [r2, #316]
	bfi	r3, r2, #0, #10
	strh	r3, [r6, #952]	@ movhi
	ldr	r2, [r5]
	ldr	r3, [r6, #952]
	ldr	r2, [r2, #320]
	bfi	r3, r2, #10, #10
	str	r3, [r6, #952]
.L564:
	ldr	r3, [r5, #964]
	cmp	r3, #0
	beq	.L565
	tst	fp, #1
	bne	.L565
	ldr	r3, [r10, #328]
	cmp	r3, #0
	bne	.L565
	ldr	r3, [r10, #332]
	cmp	r3, #0
	bne	.L565
	ldrsb	r3, [r5, #945]
	cmp	r3, #2
	bne	.L566
	ldr	r3, [r9, #44]
	cmp	r3, #2
	beq	.L567
	ldrh	r3, [r6, #304]
	bic	r3, r3, #816
	bic	r3, r3, #3
	orr	r3, r3, #544
	orr	r3, r3, #2
.L826:
	strh	r3, [r6, #304]	@ movhi
.L568:
	ldr	r3, [r6, #904]
	cmp	r8, #0
	ldr	r0, .L853+40
	and	r3, r3, #-268374016
	ldr	r1, .L853+44
	orr	r3, r3, r0
	str	r3, [r6, #904]
	ldr	r3, [r6, #908]
	ldr	r2, [r6, #920]
	and	r3, r3, #-268374016
	orr	r3, r3, r1
	str	r3, [r6, #908]
	ldr	r3, [r6, #912]
	and	r2, r2, #-268374016
	orr	r2, r2, r0
	str	r2, [r6, #920]
	and	r3, r3, #-268374016
	it	ne
	movne	r2, #4
	orr	r3, r3, r0
	str	r3, [r6, #912]
	ldr	r3, [r6, #916]
	it	eq
	moveq	r2, #255
	and	r3, r3, #-268374016
	orr	r3, r3, r1
	str	r3, [r6, #916]
	ldr	r3, [r6, #924]
	and	r3, r3, #-268374016
	orr	r3, r3, r1
	str	r3, [r6, #924]
	ldrh	r3, [r6, #928]
	it	ne
	movne	r1, #26
	b	.L854
.L855:
	.align	2
.L853:
	.word	-2147450888
	.word	.LANCHOR0+724
	.word	.LC29
	.word	.LANCHOR0+745
	.word	hal_h264e_debug
	.word	.LANCHOR0+763
	.word	.LC28
	.word	349867
	.word	.LANCHOR1+384
	.word	.LANCHOR1+624
	.word	196609
	.word	524294
.L854:
	it	eq
	moveq	r1, #16
	strb	r1, [r6, #937]
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #928]	@ movhi
	ite	ne
	movne	r3, #22
	moveq	r3, #16
	strb	r3, [r6, #932]
	it	ne
	movne	r2, #23
	strb	r3, [r6, #935]
	it	eq
	moveq	r2, #16
	strb	r3, [r6, #936]
	strb	r3, [r6, #939]
	ldr	r3, .L856
	strb	r2, [r6, #933]
	ite	ne
	movne	r2, #20
	moveq	r2, #16
	strb	r2, [r6, #934]
	str	r3, [r6, #940]
	add	r3, r3, #-50331648
	strb	r2, [r6, #938]
	subw	r3, r3, #1284
	str	r3, [r6, #944]
	ldr	r3, [r9, #44]
	ldrh	r2, [r6, #972]
	cmp	r3, #2
	ldr	r3, [r6, #968]
	and	r3, r3, #-268374016
	bne	.L575
	orr	r3, r3, #131072
	orr	r3, r3, #1
	str	r3, [r6, #968]
	mov	r3, r2	@ movhi
	movs	r2, #6
.L828:
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #972]	@ movhi
	ldrb	r3, [r6, #974]	@ zero_extendqisi2
	movs	r2, #32
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #974]
	ldr	r3, [r9, #44]
	ldr	r2, [r6, #992]
	cmp	r3, #2
	ldrd	r3, r1, [r6, #984]
	and	r3, r3, #-16777216
	bne	.L577
	orr	r3, r3, #1048592
	and	r1, r1, #-16777216
	orr	r3, r3, #4096
	str	r3, [r6, #984]
	ldr	r3, .L856+4
	and	r2, r2, #-16777216
	orrs	r3, r3, r1
	str	r3, [r6, #988]
	ldr	r3, .L856+8
	orrs	r3, r3, r2
	str	r3, [r6, #992]
.L578:
	ldrh	r3, [r6, #1000]
	add	ip, r10, #400
	ldr	r2, .L856+12
	bic	r3, r3, #32640
	bic	r3, r3, #119
	orr	r3, r3, #3
	strh	r3, [r6, #1000]	@ movhi
	ldr	r3, [r6, #1004]
	ands	r2, r2, r3
	ldr	r3, .L856+16
	orrs	r3, r3, r2
	str	r3, [r6, #1004]
	ldrh	r3, [r6, #1008]
	movs	r2, #16
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #1008]	@ movhi
	ldr	r3, .L856+20
	ldr	r2, [r6, #1016]
	str	r3, [r6, #1012]
	ldr	r3, .L856+24
	ands	r3, r3, r2
	orr	r3, r3, #33
	str	r3, [r6, #1016]
	ldr	r3, [r6, #1020]
	and	r3, r3, #-268374016
	orr	r3, r3, #1048576
	orr	r3, r3, #8
	str	r3, [r6, #1020]
	ldr	r3, [r6, #1024]
	and	r2, r3, #-1073741824
	ldr	r3, .L856+28
	orrs	r3, r3, r2
	str	r3, [r6, #1024]
	ldr	r3, [r6, #1028]
	and	r2, r3, #-16777216
	ldr	r3, .L856+32
	orrs	r3, r3, r2
	str	r3, [r6, #1028]
	ldr	r3, [r6, #732]
	and	r2, r3, #-16777216
	ldr	r3, .L856+36
	orrs	r3, r3, r2
	str	r3, [r6, #732]
	ldr	r3, [r6, #736]
	movs	r2, #240
	and	r3, r3, #-268374016
	orr	r3, r3, #9437184
	orr	r3, r3, #64
	str	r3, [r6, #736]
	ldrh	r3, [r6, #740]
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #740]	@ movhi
	ldr	r3, [r9, #44]
	cmp	r3, #2
	addw	r3, r6, #699
	bne	.L579
	add	r2, r10, #336
.L580:
	ldr	r1, [r2], #4
	cmp	ip, r2
	strb	r1, [r3, #1]!
	ldr	r0, [r2, #124]
	and	r1, r0, #63
	strb	r1, [r3, #16]
	bne	.L580
.L584:
	ldr	r3, [r5]
	add	r3, r3, #4736
	ldr	r3, [r3]
	cmp	r3, #1
	bne	.L581
.L582:
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L587
	movw	r2, #2298
	ldr	r1, .L856+40
	ldr	r0, .L856+44
	bl	printk
.L587:
	ldr	r1, [r5, #952]
	cmp	r1, #0
	beq	.L588
	movs	r2, #179
	ldr	r0, [r5, #4]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #192]
	ldr	r3, [r5, #148]
	add	r3, r3, r0
	str	r3, [r4, #188]
.L589:
	ldr	r3, [sp, #80]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsls	r3, r3, #29
	bpl	.L590
	ldr	r0, [r5, #152]
	cmp	r0, #0
	bne	.L591
	mov	r1, r5
	ldr	r6, [r1], #152
	ldr	r2, [r6, #12]
	adds	r2, r2, #15
	bic	r3, r2, #15
	ldr	r2, [r6, #16]
	adds	r2, r2, #15
	bic	r2, r2, #15
	muls	r2, r3, r2
	ldr	r3, .L856+48
	str	r3, [sp]
	ldr	r3, .L856+52
	add	r2, r2, r2, lsl #1
	asrs	r2, r2, #1
	bl	mpp_buffer_get_with_tag
	ldr	r3, [r5, #152]
	cmp	r3, #0
	bne	.L591
	movw	r1, #1490
	ldr	r0, .L856+56
	bl	printk
.L590:
	ldr	r3, [sp, #80]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsls	r6, r3, #28
	bpl	.L593
	ldr	r3, [r7]
	ldr	r6, [r5]
	lsls	r0, r3, #30
	bpl	.L594
	movw	r2, #1509
	ldr	r1, .L856+60
	ldr	r0, .L856+64
	bl	printk
.L594:
	movs	r3, #216
	strb	r3, [r4, #268]
	ldr	r3, [r6, #12]
	ldr	r2, [r4, #296]
	adds	r3, r3, #15
	bic	r3, r3, #15
	bfi	r2, r3, #0, #17
	str	r2, [r4, #296]
	lsls	r6, r3, #1
	movs	r2, #160
	add	r3, r3, r6
	strh	r3, [r4, #300]	@ movhi
	ldrb	r3, [r4, #291]	@ zero_extendqisi2
	bic	r3, r3, #28
	strb	r3, [r4, #291]
	ldr	r3, [r4, #292]
	and	r3, r3, #-1073692672
	str	r3, [r4, #292]
	ldr	r1, [r5, #152]
	ldr	r0, [r5, #4]
	bl	mpp_dev_get_iova_address
	movs	r3, #0
	str	r3, [r4, #124]
	ldr	r3, [r7]
	lsls	r1, r3, #30
	str	r0, [r4, #116]
	add	r0, r0, r6
	str	r0, [r4, #120]
	bpl	.L593
	movw	r2, #1534
	ldr	r1, .L856+60
	ldr	r0, .L856+44
	bl	printk
.L593:
	ldr	r3, [r5, #8]
	adds	r3, r3, #1
	str	r3, [r5, #8]
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L349
	mov	r3, r5
	movw	r2, #2505
	ldr	r1, .L856+68
	ldr	r0, .L856+72
	bl	printk
	b	.L349
.L534:
	ldr	r3, [r4, #340]
	ldr	r2, .L856+76
	ands	r2, r2, r3
	ldr	r3, .L856+80
	orrs	r3, r3, r2
	str	r3, [r4, #340]
	ldr	r3, [r4, #348]
	bfc	r3, #0, #20
	str	r3, [r4, #348]
	ldr	r3, [r4, #344]
	ldr	r2, [r10, #3436]
	bfi	r3, r2, #0, #20
.L823:
	str	r3, [r4, #344]
	ldrb	r3, [r4, #247]	@ zero_extendqisi2
	bfc	r3, #6, #1
	strb	r3, [r4, #247]
	b	.L538
.L536:
	ldr	r3, [r4, #340]
	ldr	r2, .L856+76
	ands	r2, r2, r3
	ldr	r3, .L856+84
	orrs	r3, r3, r2
	str	r3, [r4, #340]
	ldr	r3, [r10, #3436]
	ldr	r2, [r4, #348]
	subs	r3, r3, #1
	bfi	r2, r3, #0, #20
	ldr	r3, [r4, #344]
	str	r2, [r4, #348]
	bfc	r3, #0, #20
	b	.L823
.L543:
	cmp	r1, #4
	add	r2, r2, #2
	it	hi
	subhi	r1, r1, #2
	add	r0, r0, lr
	it	ls
	movls	r1, #2
	b	.L540
.L542:
	movw	r1, #1796
	cmp	r7, r1
	it	ne
	movne	r2, ip
	b	.L544
.L546:
	str	r1, [r4, #192]
	b	.L547
.L552:
	ldr	r3, [sp, #36]
	strd	r6, lr, [sp]
	str	r3, [sp, #16]
	ldr	r3, [sp, #44]
	strd	r8, r3, [sp, #8]
	ldr	r3, [sp, #40]
	bl	vepu540c_set_qpmap_normal
	b	.L553
.L558:
	cmp	r2, #31
	bhi	.L559
	movs	r2, #32
	b	.L824
.L559:
	cmp	r2, #33
	bhi	.L557
	movs	r2, #34
	b	.L824
.L857:
	.align	2
.L856:
	.word	370416150
	.word	1316118
	.word	1053204
	.word	-536346112
	.word	83935248
	.word	336595976
	.word	-268370040
	.word	12591108
	.word	8413248
	.word	1313797
	.word	.LANCHOR0+763
	.word	.LC29
	.word	.LANCHOR0+781
	.word	.LC22
	.word	.LC45
	.word	.LANCHOR0+813
	.word	.LC28
	.word	.LANCHOR0+393
	.word	.LC11
	.word	-2147450888
	.word	32800769
	.word	32800771
.L566:
	cmp	r3, #0
	ble	.L568
.L567:
	ldrh	r3, [r6, #304]
	bic	r3, r3, #816
	bic	r3, r3, #3
	orr	r3, r3, #272
	orr	r3, r3, #1
	b	.L826
.L565:
	ldr	r3, [r9, #44]
	ldrb	r2, [r6, #305]	@ zero_extendqisi2
	cmp	r3, #2
	ldrb	r3, [r6, #304]	@ zero_extendqisi2
	bne	.L569
	ldr	r1, [r10, #328]
	bfi	r3, r1, #0, #2
	strb	r3, [r6, #304]
	ldr	r1, [r10, #328]
	bfi	r3, r1, #4, #2
	strb	r3, [r6, #304]
	ldr	r3, [r10, #328]
.L827:
	bfi	r2, r3, #0, #2
	strb	r2, [r6, #305]
	b	.L568
.L569:
	ldr	r1, [r10, #332]
	bfi	r3, r1, #0, #2
	strb	r3, [r6, #304]
	ldr	r1, [r10, #332]
	bfi	r3, r1, #4, #2
	strb	r3, [r6, #304]
	ldr	r3, [r10, #332]
	b	.L827
.L575:
	orr	r3, r3, #262144
	orr	r3, r3, #2
	str	r3, [r6, #968]
	mov	r3, r2	@ movhi
	movs	r2, #9
	b	.L828
.L577:
	ldr	r0, .L858
	and	r1, r1, #-16777216
	and	r2, r2, #-16777216
	orrs	r3, r3, r0
	str	r3, [r6, #984]
	ldr	r3, .L858+4
	orrs	r2, r2, r0
	str	r2, [r6, #992]
	orrs	r3, r3, r1
	str	r3, [r6, #988]
	b	.L578
.L579:
	add	r0, r10, #464
	mov	r1, ip
.L583:
	ldr	r2, [r1], #4
	cmp	r0, r1
	strb	r2, [r3, #1]!
	ldr	r2, [r1, #124]
	and	r2, r2, #63
	strb	r2, [r3, #16]
	bne	.L583
	b	.L584
.L581:
	ldrb	r3, [r6, #974]	@ zero_extendqisi2
	movs	r2, #45
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #974]
	ldr	r3, [r9, #44]
	ldr	r2, [r6, #992]
	cmp	r3, #2
	ldrd	r3, r0, [r6, #984]
	bne	.L585
	ldr	r1, .L858+8
	and	r0, r0, #-16777216
	and	r3, r3, #-16777216
	orr	r0, r0, #1179666
	orrs	r3, r3, r1
	orr	r0, r0, #4352
	str	r3, [r6, #984]
	str	r0, [r6, #988]
.L829:
	ldrh	r3, [r6, #928]
	and	r2, r2, #-16777216
	orrs	r2, r2, r1
	str	r2, [r6, #992]
	movw	r2, #4095
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #928]	@ movhi
	ldrh	r3, [r6, #1008]
	bfc	r3, #0, #12
	strh	r3, [r6, #1008]	@ movhi
	mov	r3, #67372036
	str	r3, [r6, #1012]
	ldr	r3, [r6, #1020]
	and	r3, r3, #-268374016
	str	r3, [r6, #1020]
	ldr	r3, [r6, #1024]
	and	r2, r3, #-1073741824
	ldr	r3, .L858+12
	orrs	r3, r3, r2
	str	r3, [r6, #1024]
	b	.L582
.L585:
	ldr	r1, [r6, #968]
	mov	ip, #7
	and	r3, r3, #-16777216
	and	r1, r1, #-268374016
	orr	r1, r1, #131072
	orr	r1, r1, #1
	str	r1, [r6, #968]
	ldrh	r1, [r6, #972]
	bfi	r1, ip, #0, #12
	strh	r1, [r6, #972]	@ movhi
	ldr	r1, .L858+16
	orrs	r3, r3, r1
	str	r3, [r6, #984]
	and	r3, r0, #-16777216
	orr	r3, r3, #1572888
	orr	r3, r3, #6144
	str	r3, [r6, #988]
	b	.L829
.L588:
	str	r1, [r4, #192]
	b	.L589
.L591:
	ldrb	r3, [r4, #244]	@ zero_extendqisi2
	movs	r2, #163
	orr	r3, r3, #4
	strb	r3, [r4, #244]
	movs	r3, #0
	str	r3, [r4, #128]
	ldr	r1, [r5, #152]
	ldr	r0, [r5, #4]
	bl	mpp_dev_get_iova_address
	ldrb	r3, [r4, #247]	@ zero_extendqisi2
	orr	r3, r3, #128
	strb	r3, [r4, #247]
	str	r0, [r4, #132]
	b	.L590
.L859:
	.align	2
.L858:
	.word	1053721
	.word	1184793
	.word	1052688
	.word	4198404
	.word	1316375
	.fnend
	.size	hal_h264e_vepu540c_gen_regs, .-hal_h264e_vepu540c_gen_regs
	.global	hal_h264e_vepu540c
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.37631, %object
	.size	__func__.37631, 28
__func__.37631:
	.ascii	"hal_h264e_vepu540c_ret_task\000"
	.type	__func__.37597, %object
	.size	__func__.37597, 32
__func__.37597:
	.ascii	"hal_h264e_vepu540c_status_check\000"
	.type	__func__.37659, %object
	.size	__func__.37659, 33
__func__.37659:
	.ascii	"hal_h264e_vepu540c_ret_comb_task\000"
	.type	__func__.37588, %object
	.size	__func__.37588, 25
__func__.37588:
	.ascii	"hal_h264e_vepu540c_start\000"
	.type	__func__.37649, %object
	.size	__func__.37649, 30
__func__.37649:
	.ascii	"hal_h264e_vepu540c_comb_start\000"
	.type	__func__.37238, %object
	.size	__func__.37238, 23
__func__.37238:
	.ascii	"update_vepu540c_syntax\000"
	.type	__func__.37256, %object
	.size	__func__.37256, 28
__func__.37256:
	.ascii	"hal_h264e_vepu540c_get_task\000"
	.type	__FUNCTION__.37219, %object
	.size	__FUNCTION__.37219, 15
__FUNCTION__.37219:
	.ascii	"setup_hal_bufs\000"
	.type	__func__.37220, %object
	.size	__func__.37220, 15
__func__.37220:
	.ascii	"setup_hal_bufs\000"
	.type	__FUNCTION__.37164, %object
	.size	__FUNCTION__.37164, 13
__FUNCTION__.37164:
	.ascii	"get_wrap_buf\000"
	.type	__func__.37136, %object
	.size	__func__.37136, 26
__func__.37136:
	.ascii	"hal_h264e_vepu540c_deinit\000"
	.type	__FUNCTION__.37137, %object
	.size	__FUNCTION__.37137, 26
__FUNCTION__.37137:
	.ascii	"hal_h264e_vepu540c_deinit\000"
	.type	__func__.37144, %object
	.size	__func__.37144, 24
__func__.37144:
	.ascii	"hal_h264e_vepu540c_init\000"
	.type	__FUNCTION__.37147, %object
	.size	__FUNCTION__.37147, 24
__FUNCTION__.37147:
	.ascii	"hal_h264e_vepu540c_init\000"
	.type	__func__.37228, %object
	.size	__func__.37228, 27
__func__.37228:
	.ascii	"hal_h264e_vepu540c_prepare\000"
	.type	__func__.37604, %object
	.size	__func__.37604, 24
__func__.37604:
	.ascii	"hal_h264e_vepu540c_wait\000"
	.type	__func__.37574, %object
	.size	__func__.37574, 28
__func__.37574:
	.ascii	"hal_h264e_vepu540c_gen_regs\000"
	.type	__func__.37260, %object
	.size	__func__.37260, 22
__func__.37260:
	.ascii	"setup_vepu540c_normal\000"
	.type	__func__.37272, %object
	.size	__func__.37272, 20
__func__.37272:
	.ascii	"setup_vepu540c_prep\000"
	.type	__func__.37285, %object
	.size	__func__.37285, 21
__func__.37285:
	.ascii	"setup_vepu540c_codec\000"
	.type	__func__.37332, %object
	.size	__func__.37332, 24
__func__.37332:
	.ascii	"setup_vepu540c_rdo_pred\000"
.LC0:
	.word	9
	.word	12
	.word	16
	.word	16
	.type	__func__.37352, %object
	.size	__func__.37352, 23
__func__.37352:
	.ascii	"setup_vepu540c_rdo_cfg\000"
	.type	__func__.37394, %object
	.size	__func__.37394, 23
__func__.37394:
	.ascii	"setup_vepu540c_scl_cfg\000"
	.type	__func__.37415, %object
	.size	__func__.37415, 23
__func__.37415:
	.ascii	"setup_vepu540c_rc_base\000"
	.type	__func__.37433, %object
	.size	__func__.37433, 22
__func__.37433:
	.ascii	"setup_vepu540c_io_buf\000"
	.type	__FUNCTION__.37450, %object
	.size	__FUNCTION__.37450, 22
__FUNCTION__.37450:
	.ascii	"setup_vepu540c_io_buf\000"
	.type	__func__.37465, %object
	.size	__func__.37465, 25
__func__.37465:
	.ascii	"setup_vepu540c_recn_refr\000"
	.type	__FUNCTION__.37467, %object
	.size	__FUNCTION__.37467, 25
__FUNCTION__.37467:
	.ascii	"setup_vepu540c_recn_refr\000"
	.type	__func__.37188, %object
	.size	__func__.37188, 21
__func__.37188:
	.ascii	"setup_recn_refr_wrap\000"
	.type	ref_type_map, %object
	.size	ref_type_map, 16
ref_type_map:
	.word	0
	.word	1
	.word	2
	.word	3
	.type	__func__.37483, %object
	.size	__func__.37483, 21
__func__.37483:
	.ascii	"setup_vepu540c_split\000"
	.type	__func__.37517, %object
	.size	__func__.37517, 18
__func__.37517:
	.ascii	"setup_vepu540c_me\000"
	.type	__func__.37530, %object
	.size	__func__.37530, 18
__func__.37530:
	.ascii	"setup_vepu540c_l2\000"
	.type	__FUNCTION__.37381, %object
	.size	__FUNCTION__.37381, 32
__FUNCTION__.37381:
	.ascii	"vepu540c_h264e_save_pass1_patch\000"
	.type	__func__.37389, %object
	.size	__func__.37389, 31
__func__.37389:
	.ascii	"vepu540c_h264e_use_pass1_patch\000"
	.type	hal_h264e_vepu540c, %object
	.size	hal_h264e_vepu540c, 64
hal_h264e_vepu540c:
	.word	.LC22
	.word	7
	.word	984
	.word	0
	.word	hal_h264e_vepu540c_init
	.word	hal_h264e_vepu540c_deinit
	.word	hal_h264e_vepu540c_prepare
	.word	hal_h264e_vepu540c_get_task
	.word	hal_h264e_vepu540c_gen_regs
	.word	hal_h264e_vepu540c_start
	.word	hal_h264e_vepu540c_wait
	.word	0
	.word	0
	.word	hal_h264e_vepu540c_ret_task
	.word	hal_h264e_vepu540c_comb_start
	.word	hal_h264e_vepu540c_ret_comb_task
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	h264_aq_tthd_smart, %object
	.size	h264_aq_tthd_smart, 64
h264_aq_tthd_smart:
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
	.word	28
	.type	h264_I_aq_step_smart, %object
	.size	h264_I_aq_step_smart, 64
h264_I_aq_step_smart:
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
	.word	8
	.word	10
	.type	h264_P_aq_step_smart, %object
	.size	h264_P_aq_step_smart, 64
h264_P_aq_step_smart:
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
	.word	8
	.word	10
	.type	h264_aq_tthd_default, %object
	.size	h264_aq_tthd_default, 64
h264_aq_tthd_default:
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
	.type	h264_I_aq_step_default, %object
	.size	h264_I_aq_step_default, 64
h264_I_aq_step_default:
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
	.type	h264_P_aq_step_default, %object
	.size	h264_P_aq_step_default, 64
h264_P_aq_step_default:
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
	.type	h264e_lambda_default, %object
	.size	h264e_lambda_default, 240
h264e_lambda_default:
	.word	5
	.word	6
	.word	7
	.word	9
	.word	11
	.word	14
	.word	18
	.word	22
	.word	28
	.word	36
	.word	45
	.word	57
	.word	72
	.word	91
	.word	115
	.word	145
	.word	182
	.word	230
	.word	290
	.word	365
	.word	460
	.word	580
	.word	731
	.word	921
	.word	1161
	.word	1462
	.word	1843
	.word	2322
	.word	2925
	.word	3686
	.word	4644
	.word	5851
	.word	7372
	.word	9289
	.word	11703
	.word	14745
	.word	18578
	.word	23407
	.word	29491
	.word	37156
	.word	46814
	.word	58982
	.word	74313
	.word	93628
	.word	117964
	.word	148626
	.word	187257
	.word	235929
	.word	297252
	.word	374514
	.word	471859
	.word	594505
	.word	749029
	.word	943718
	.word	1189010
	.word	1498059
	.word	1887436
	.word	2378020
	.word	2996118
	.word	3774872
	.type	h264e_lambda_cvr, %object
	.size	h264e_lambda_cvr, 240
h264e_lambda_cvr:
	.word	9
	.word	11
	.word	14
	.word	17
	.word	22
	.word	27
	.word	34
	.word	43
	.word	54
	.word	69
	.word	86
	.word	109
	.word	137
	.word	173
	.word	218
	.word	274
	.word	345
	.word	435
	.word	548
	.word	691
	.word	870
	.word	1097
	.word	1382
	.word	1741
	.word	2193
	.word	2763
	.word	3482
	.word	5057
	.word	6372
	.word	8028
	.word	10115
	.word	12744
	.word	16056
	.word	20230
	.word	25488
	.word	36372
	.word	45826
	.word	57738
	.word	72745
	.word	91653
	.word	115475
	.word	162529
	.word	204774
	.word	257999
	.word	325059
	.word	452485
	.word	570095
	.word	718275
	.word	990842
	.word	1248383
	.word	1572864
	.word	1981684
	.word	2496766
	.word	3145728
	.word	3963368
	.word	4993532
	.word	6291456
	.word	7926736
	.word	9987064
	.word	201326592
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC1:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC2:
	.ascii	"\0016%s:%d: lkt_done finish\000"
.LC3:
	.ascii	"\0016%s:%d: enc_done finish\000"
.LC4:
	.ascii	"\0016%s:%d: enc_slice finsh\000"
.LC5:
	.ascii	"\0016%s:%d: safe clear finsh\000"
.LC6:
	.ascii	"\0016%s:%d: bit stream overflow\000"
.LC7:
	.ascii	"\0016%s:%d: bus write full\000"
.LC8:
	.ascii	"\0016%s:%d: bus error\000"
.LC9:
	.ascii	"\0016%s:%d: wdg timeout\000"
.LC10:
	.ascii	"\0016%s:%d: source id mismatch\012\000"
.LC11:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC12:
	.ascii	"\0013%s:%d: set register write failed %d\012\000"
.LC13:
	.ascii	"\0013%s:%d: set register read failed %d\012\000"
.LC14:
	.ascii	"\0013%s:%d: send cmd failed %d\012\000"
.LC15:
	.ascii	"\0016%s:%d: update cfg\000"
.LC16:
	.ascii	"\0016%s:%d: update sps\000"
.LC17:
	.ascii	"\0016%s:%d: update pps\000"
.LC18:
	.ascii	"\0016%s:%d: update slice\000"
.LC19:
	.ascii	"\0016%s:%d: update frames\000"
.LC20:
	.ascii	"\0016%s:%d: update prefix nal\000"
.LC21:
	.ascii	"\0016%s:%d: invalid syntax type %d\012\000"
.LC22:
	.ascii	"hal_h264e_vepu540c\000"
.LC23:
	.ascii	"\0016%s:%d: frame size %d -> %d max count %d -> %d\012"
	.ascii	"\000"
.LC24:
	.ascii	"\0013%s:%d: mpp_dev_init failed. ret: %d\012\000"
.LC25:
	.ascii	"\0013%d: HalVepu540cRegSet alloc fail\000"
.LC26:
	.ascii	"\0013%s:%d: poll cmd failed %d\012\000"
.LC27:
	.ascii	"\0016%s:%d: frame %d generate regs now\000"
.LC28:
	.ascii	"\0016%s:%d: enter\012\000"
.LC29:
	.ascii	"\0016%s:%d: leave\012\000"
.LC30:
	.ascii	"\0013%s:%d: invalid modification_of_pic_nums_idc %d"
	.ascii	"\012\000"
.LC31:
	.ascii	"\0013%s:%d: unsupported mmco 0 %d\012\000"
.LC32:
	.ascii	"\0016%s:%d: bittarget %d qp [%d %d %d]\012\000"
.LC33:
	.ascii	"\0013%d: online case set full frame err\000"
.LC34:
	.ascii	"buf_thumb\000"
.LC35:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC36:
	.ascii	"buf_pixel\000"
.LC37:
	.ascii	"\0016%s:%d: cur is idr  lt %d\012\000"
.LC38:
	.ascii	"\0016%s:%d: ref type %d\012\000"
.LC39:
	.ascii	"\0013%d: WARNING: not support lt ref to st when buf"
	.ascii	" is wrap\000"
.LC40:
	.ascii	"\0013%d: WARNING: not support lt ref to lt when buf"
	.ascii	" is wrap\000"
.LC41:
	.ascii	"\0016%s:%d: cur_is_ref %d\012\000"
.LC42:
	.ascii	"\0016%s:%d: hdr[size %d top %d bot %d cur %d pre %d"
	.ascii	"]\012\000"
.LC43:
	.ascii	"\0016%s:%d: bdy [size %d top %d bot %d cur %d pre %"
	.ascii	"d]\012\000"
.LC44:
	.ascii	"\0016%s:%d: invalide slice split mode %d\012\000"
.LC45:
	.ascii	"\0013%d: buf_pass1 maSlloc fail, debreath invaild\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
