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
	ldr	r6, [r0, #332]
	mov	r5, r0
	ldr	r7, [r1, #4]
	ldrd	r2, r3, [r3, #60]
	mul	r9, r3, r2
	add	r3, r6, #5600
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
	ldrh	r4, [r3, #226]
	ldrh	r1, [r3, #250]
	str	r2, [sp, #48]
	ldrh	r2, [r3, #248]
	str	r2, [sp, #52]
	ldrh	r2, [r3, #234]
	add	r4, r4, r2
	ldrh	r2, [r3, #242]
	add	r4, r4, r2
	ldrh	r2, [r3, #236]
	add	r4, r4, r1
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
	ldr	r3, [r0, #352]
	cmp	r3, #0
	bne	.L2
	ldr	r3, [sp, #4]
	add	r8, r4, r4, lsl #4
	movs	r2, #22
	mla	r3, r2, r3, r8
	ldr	r2, [sp, #8]
	mov	r8, #24
	mla	r8, r8, r2, r3
.L53:
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #20]
	lsr	r8, r8, #2
	add	fp, r3, r3, lsl #2
	movs	r3, #6
	mla	fp, r3, r2, fp
	ldr	r3, [sp, #12]
	add	fp, fp, r3, lsl #2
	ldr	r3, .L59
	lsr	fp, fp, #2
	ldr	r3, [r3]
	lsls	r3, r3, #30
	bpl	.L4
	mov	r3, r5
	movw	r2, #2674
	ldr	r1, .L59+4
	ldr	r0, .L59+8
	bl	printk
.L4:
	movs	r3, #100
	rsb	r2, r9, r9, lsl #4
	mul	r8, r3, r8
	cmp	r8, r2
	bls	.L5
	movs	r3, #200
.L54:
	str	r3, [r7, #204]
	movs	r3, #100
	mul	fp, r3, fp
	movs	r3, #30
	mul	r3, r3, r9
	cmp	fp, r3
	bls	.L9
	movs	r3, #2
.L55:
	str	r3, [r7, #208]
	ldr	r8, [r5, #332]
	ldrb	r3, [r8, #44]	@ zero_extendqisi2
	lsls	r0, r3, #30
	bpl	.L12
	ldr	r3, .L59
	ldr	r3, [r3]
	lsls	r1, r3, #28
	bpl	.L12
	movw	r2, #2563
	ldr	r1, .L59+12
	ldr	r0, .L59+16
	bl	printk
.L12:
	ldrb	r3, [r8, #44]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bpl	.L13
	ldr	r3, .L59
	ldr	r3, [r3]
	lsls	r3, r3, #28
	bpl	.L13
	movw	r2, #2566
	ldr	r1, .L59+12
	ldr	r0, .L59+20
	bl	printk
.L13:
	ldrb	r3, [r8, #44]	@ zero_extendqisi2
	lsls	r0, r3, #28
	bpl	.L14
	ldr	r3, .L59
	ldr	r3, [r3]
	lsls	r1, r3, #28
	bpl	.L14
	movw	r2, #2569
	ldr	r1, .L59+12
	ldr	r0, .L59+24
	bl	printk
.L14:
	ldrb	r3, [r8, #44]	@ zero_extendqisi2
	lsls	r2, r3, #29
	bpl	.L15
	ldr	r3, .L59
	ldr	r3, [r3]
	lsls	r3, r3, #28
	bpl	.L15
	movw	r2, #2572
	ldr	r1, .L59+12
	ldr	r0, .L59+28
	bl	printk
.L15:
	ldrb	r3, [r8, #44]	@ zero_extendqisi2
	lsls	r0, r3, #27
	bpl	.L16
	ldr	r1, .L59+12
	movw	r2, #2575
	ldr	r0, .L59+32
.L58:
	bl	printk
	mov	r0, #-1
.L1:
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L2:
	ldr	r3, [sp, #4]
	mov	r8, #11
	ldr	r2, [sp, #8]
	mul	r3, r8, r3
	mov	r8, #12
	mla	r8, r8, r2, r3
	add	r8, r8, r4, lsl #3
	b	.L53
.L5:
	add	r2, r9, r9, lsl #2
	cmp	r8, r2
	bhi	.L54
	cmp	r8, r9, lsr #2
	ite	ls
	movls	r3, #0
	movhi	r3, #1
	b	.L54
.L9:
	movs	r3, #13
	mul	r3, r3, r9
	cmp	fp, r3
	ite	ls
	movls	r3, #0
	movhi	r3, #1
	b	.L55
.L16:
	lsls	r1, r3, #26
	bpl	.L18
	movw	r2, #2580
	ldr	r1, .L59+12
	ldr	r0, .L59+36
	b	.L58
.L18:
	lsls	r2, r3, #25
	bpl	.L19
	movw	r2, #2585
	ldr	r1, .L59+12
	ldr	r0, .L59+40
	b	.L58
.L19:
	ldrb	r3, [r8, #45]	@ zero_extendqisi2
	lsls	r3, r3, #31
	bpl	.L20
	movw	r2, #2590
	ldr	r1, .L59+12
	ldr	r0, .L59+44
	b	.L58
.L20:
	ldrd	r3, r2, [sp, #32]
	add	r3, r3, r2
	movw	r2, #5608
	ldr	r2, [r6, r2]
	mov	r1, r3
	ldr	r3, [sp, #28]
	add	r1, r1, r3
	ldr	r3, [sp, #24]
	add	r3, r3, r1
	mov	r1, r9
	mov	fp, r3
	ldr	r3, [r10, #44]
	add	r3, r3, r2
	ldr	r2, [r10, #48]
	str	r3, [r10, #44]
	add	r2, r2, r3
	lsl	r8, r3, #3
	movw	r3, #5620
	str	r2, [r10, #48]
	str	r8, [r7, #184]
	ldr	r0, [r6, r3]
	bl	__aeabi_uidiv
	str	r0, [r7, #188]
	ldr	r0, [sp, #20]
	ldrb	r3, [r6, #734]	@ zero_extendqisi2
	ldrb	r2, [r6, #732]	@ zero_extendqisi2
	ldrb	r1, [r6, #733]	@ zero_extendqisi2
	muls	r0, r3, r0
	add	r3, r3, r1
	mla	ip, fp, r2, r0
	ldr	r0, [sp, #12]
	add	r2, r2, r1
	muls	r2, r0, r2
	add	r0, ip, r2, lsr #1
	ldr	r2, [sp, #16]
	muls	r3, r2, r3
	add	r0, r0, r3, lsr #1
	ldr	r3, [sp, #12]
	adds	r1, r3, r2
	ldr	r3, [sp, #20]
	add	r1, r1, r3
	adds	r1, r1, fp
	bne	.L21
.L56:
	ldrd	r3, r2, [sp, #48]
	str	r0, [r7, #192]
	add	r3, r3, r2
	ldrh	r2, [r6, #738]
	mov	r1, r3
	ldr	r3, [sp, #44]
	ubfx	ip, r2, #0, #12
	ldrh	r2, [r6, #740]
	add	r1, r1, r3
	ldr	r3, [sp, #40]
	ubfx	r2, r2, #0, #12
	add	r1, r1, r3
	ldrh	r3, [r6, #736]
	ubfx	r3, r3, #0, #12
	add	r0, r3, ip
	mul	lr, r4, r0
	ldr	r0, [sp, #8]
	muls	r0, r2, r0
	add	r2, r2, ip
	mla	r0, r1, r3, r0
	ldr	r3, [sp, #4]
	add	r4, r4, r3
	muls	r2, r3, r2
	ldr	r3, [sp, #8]
	add	r0, r0, lr, lsr #1
	add	r4, r4, r3
	adds	r1, r4, r1
	add	r0, r0, r2, lsr #1
	bne	.L23
.L57:
	movw	r3, #5764
	str	r0, [r7, #196]
	ldr	r0, [r6, r3]
	add	r3, r6, #5760
	mov	r1, r9
	ldr	r3, [r3]
	ubfx	r0, r0, #0, #23
	ubfx	r3, r3, #0, #23
	add	r0, r0, r3
	movw	r3, #5756
	ldr	r3, [r6, r3]
	ubfx	r3, r3, #0, #21
	add	r0, r0, r3
	lsls	r0, r0, #8
	bl	__aeabi_uidiv
	str	r0, [r7, #200]
	str	r8, [r5, #212]
	ldr	r3, [r7, #188]
	str	r3, [r5, #216]
	ldr	r3, [r7, #200]
	str	r3, [r5, #228]
	ldr	r3, [r5, #168]
	ldr	r2, [r3, #44]
	str	r2, [r3, #48]
	movw	r3, #5772
	str	r9, [r5, #292]
	ldrb	r1, [r6, r3]	@ zero_extendqisi2
	movs	r3, #4
	smulbb	r2, r1, r3
	str	r2, [r5, #296]
	movw	r2, #5773
	ldrb	r0, [r6, r2]	@ zero_extendqisi2
	movw	r2, #5774
	lsls	r0, r0, #2
	str	r0, [r5, #300]
	ldrb	r2, [r6, r2]	@ zero_extendqisi2
	smlabb	r1, r3, r1, r0
	smulbb	r4, r2, r3
	smlabb	r2, r3, r2, r1
	str	r4, [r5, #304]
	movw	r4, #5775
	ldrb	r4, [r6, r4]	@ zero_extendqisi2
	smulbb	r6, r4, r3
	smlabb	r3, r3, r4, r2
	str	r6, [r5, #308]
	str	r3, [r5, #312]
	add	r3, r5, #188
	str	r3, [r10, #24]
	movs	r3, #1
	str	r3, [r10, #20]
	ldr	r3, .L59
	ldr	r0, [r3]
	ands	r0, r0, #2
	beq	.L1
	mov	r3, r5
	mov	r2, #2752
	ldr	r1, .L59+4
	ldr	r0, .L59+48
	bl	printk
	movs	r0, #0
	b	.L1
.L21:
	bl	__aeabi_uidiv
	b	.L56
.L23:
	bl	__aeabi_uidiv
	b	.L57
.L60:
	.align	2
.L59:
	.word	hal_h264e_debug
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR0+28
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
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
	ldr	r6, .L76
	mov	r5, r0
	mov	r4, r2
	ldr	r8, [r2, #4]
	ldr	r9, [r0, #332]
	ldr	r3, [r6]
	lsls	r1, r3, #30
	bpl	.L62
	mov	r3, r0
	movw	r2, #2793
	ldr	r1, .L76+4
	ldr	r0, .L76+8
	bl	printk
.L62:
	mov	r1, r7
	mov	r0, r5
	bl	hal_h264e_vepu540c_ret_task
	mov	r7, r0
	cbnz	r0, .L61
	ldrb	r3, [r9, #45]	@ zero_extendqisi2
	lsls	r2, r3, #26
	movw	r2, #5708
	itt	mi
	movmi	r3, #1
	strmi	r3, [r4, #108]
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
	bpl	.L61
	mov	r3, r5
	movw	r2, #2807
	ldr	r1, .L76+4
	ldr	r0, .L76+12
	bl	printk
.L61:
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L77:
	.align	2
.L76:
	.word	hal_h264e_debug
	.word	.LANCHOR0+60
	.word	.LC0
	.word	.LC9
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
	ldr	r6, .L104
	.pad #28
	sub	sp, sp, #28
	ldr	r3, [r6]
	lsls	r1, r3, #30
	bpl	.L79
	mov	r3, r0
	movw	r2, #2439
	ldr	r1, .L104+4
	ldr	r0, .L104+8
	bl	printk
.L79:
	ldr	r3, [r5, #332]
	movs	r1, #0
	mov	r2, sp
	ldr	r0, [r5, #4]
	str	r3, [sp]
	movs	r3, #100
	strd	r3, r1, [sp, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L80
	mov	r3, r0
	mov	r2, #2464
.L103:
	ldr	r1, .L104+4
	ldr	r0, .L104+12
.L102:
	bl	printk
.L91:
	ldr	r3, [r6]
	lsls	r3, r3, #30
	bpl	.L78
	mov	r3, r5
	movw	r2, #2552
	ldr	r1, .L104+4
	ldr	r0, .L104+16
	bl	printk
.L78:
	mov	r0, r4
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L80:
	ldr	r3, [r5, #332]
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
	cbz	r0, .L82
	mov	r3, r0
	movw	r2, #2473
	b	.L103
.L82:
	ldr	r3, [r5, #332]
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
	cbz	r0, .L83
	mov	r3, r0
	movw	r2, #2482
	b	.L103
.L83:
	ldr	r3, [r5, #332]
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
	cbz	r0, .L84
	mov	r3, r0
	movw	r2, #2491
	b	.L103
.L84:
	ldr	r3, [r5, #332]
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
	cbz	r0, .L85
	mov	r3, r0
	movw	r2, #2500
	b	.L103
.L85:
	ldr	r3, [r5, #332]
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
	cbz	r0, .L86
	mov	r3, r0
	movw	r2, #2510
	b	.L103
.L86:
	ldr	r3, [r5, #332]
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
	cbz	r0, .L87
	mov	r3, r0
	movw	r2, #2520
	b	.L103
.L87:
	ldr	r3, [r5, #332]
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
	cbz	r0, .L88
	mov	r3, r0
	movw	r2, #2530
	b	.L103
.L88:
	ldr	r3, [r5, #332]
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
	cbz	r0, .L89
	mov	r3, r0
	movw	r2, #2540
	ldr	r1, .L104+4
	ldr	r0, .L104+20
	b	.L102
.L89:
	mov	r2, r0
	movs	r1, #5
	ldr	r0, [r5, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cmp	r0, #0
	beq	.L91
	mov	r3, r0
	movw	r2, #2547
	ldr	r1, .L104+4
	ldr	r0, .L104+24
	b	.L102
.L105:
	.align	2
.L104:
	.word	hal_h264e_debug
	.word	.LANCHOR0+93
	.word	.LC0
	.word	.LC10
	.word	.LC9
	.word	.LC11
	.word	.LC12
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
	ldr	r5, .L123
	mov	r4, r0
	mov	r8, r1
	ldr	r6, [r0, #332]
	ldr	r9, [r3, #36]
	ldr	r3, [r5]
	lsls	r2, r3, #30
	bpl	.L107
	mov	r3, r0
	movw	r2, #2765
	ldr	r1, .L123+4
	ldr	r0, .L123+8
	bl	printk
.L107:
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
	str	r3, [sp, #4]
	add	r0, sp, #4
	ldr	r3, [r4, #332]
	str	r10, [sp, #20]
	add	r2, r3, #500
	add	r3, r3, r9
	strd	r3, r7, [sp, #12]
	ldr	r3, [r4, #340]
	str	r2, [sp, #8]
	str	r3, [sp, #24]
	bl	vepu540c_set_jpeg_reg
	ldr	r1, [r7, #104]
	cbz	r1, .L108
	mov	r2, #384
	add	r0, r6, r9
	bl	memcpy
.L108:
	ldr	r1, [r7, #100]
	cbz	r1, .L109
	add	r0, r6, #5280
	mov	r2, #312
	adds	r0, r0, #16
	bl	memcpy
.L109:
	ldr	r3, [r5]
	lsls	r3, r3, #30
	bpl	.L110
	mov	r3, r4
	movw	r2, #2781
	ldr	r1, .L123+4
	ldr	r0, .L123+12
	bl	printk
.L110:
	mov	r1, r8
	mov	r0, r4
	bl	hal_h264e_vepu540c_start
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L124:
	.align	2
.L123:
	.word	hal_h264e_debug
	.word	.LANCHOR0+118
	.word	.LC0
	.word	.LC9
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
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r8, #0
	ldrd	fp, r5, [r1, #12]
	.pad #60
	sub	sp, sp, #60
	ldr	r6, .L217
	mov	r4, r0
	ldr	r10, .L217+52
	adds	r5, r5, #8
	ldr	r7, .L217+4
	mov	r9, r8
	str	r1, [sp, #20]
.L126:
	cmp	fp, r8
	bgt	.L142
	ldr	r3, .L217+4
	ldr	r3, [r3]
	lsls	r1, r3, #30
	bpl	.L143
	mov	r3, r4
	movw	r2, #706
	ldr	r1, .L217+8
	ldr	r0, .L217+12
	bl	printk
.L143:
	tst	r9, #1
	beq	.L145
	ldr	r1, [r4]
	movw	r8, #8191
	ldr	r3, [r1, #28]
	ldr	r7, [r1, #32]
	add	r10, r3, #63
	adds	r7, r7, #63
	bic	r6, r10, #63
	bic	r7, r7, #63
	asr	r10, r10, #6
	mul	r3, r7, r6
	cmp	r3, #0
	mov	r2, r3
	it	lt
	addlt	r2, r3, #63
	add	r3, r3, r3, lsl #1
	add	r8, r8, r2, asr #6
	ldr	r2, .L217+16
	asrs	r3, r3, #1
	str	r3, [sp, #24]
	and	r8, r8, r2
	add	fp, r8, r3
	mov	r3, r10
	muls	r3, r7, r3
	it	mi
	addmi	r3, r3, #63
	asrs	r5, r3, #6
	ldr	r3, [r4, #40]
	lsls	r5, r5, #8
	str	r3, [sp, #28]
	movw	r3, #4700
	ldr	r0, [r1, r3]
	add	r5, r5, #8160
	adds	r5, r5, #31
	ands	r5, r5, r2
	cmp	r0, #0
	beq	.L176
	bl	mpp_enc_ref_cfg_get_cpb_info
	ldr	r3, [r0]
	add	r9, r3, #1
	ldr	r3, [r0, #4]
	cmp	r9, #2
	it	lt
	movlt	r9, #2
	str	r3, [sp, #32]
.L148:
	cmp	r6, #3072
	ble	.L150
	mov	r3, #896
	mul	r10, r3, r10
	ldr	r3, [r4, #344]
	ldr	r3, [r3, #8]
	sub	r10, r10, #32256
	cmp	r3, #0
	bne	.L151
	ldr	r2, [r4, #148]
	str	r3, [sp, #36]
	cmp	r10, r2
	ldr	r0, [r4, #336]
	beq	.L152
	ldr	r1, .L217+20
	bl	mpp_buffer_put_with_caller
	ldr	r3, [sp, #36]
	str	r3, [r4, #336]
.L153:
	ldr	r3, .L217+20
	mov	r2, r10
	add	r1, r4, #336
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L217+24
	bl	mpp_buffer_get_with_tag
	b	.L154
.L142:
	ldr	r3, [r5, #-8]
	cmp	r3, #6
	bhi	.L127
	tbb	[pc, r3]
.L129:
	.byte	(.L134-.L129)/2
	.byte	(.L133-.L129)/2
	.byte	(.L132-.L129)/2
	.byte	(.L127-.L129)/2
	.byte	(.L131-.L129)/2
	.byte	(.L130-.L129)/2
	.byte	(.L128-.L129)/2
	.p2align 1
.L134:
	ldr	r3, [r7]
	lsls	r3, r3, #28
	bpl	.L135
	movw	r2, #659
	mov	r1, r6
	ldr	r0, .L217+28
	bl	printk
.L135:
	ldr	r3, [r5, #-4]
	str	r3, [r4]
.L136:
	ldr	r2, [r5, #-8]
	movs	r3, #1
	add	r8, r8, #1
	adds	r5, r5, #8
	lsls	r3, r3, r2
	orr	r9, r9, r3
	b	.L126
.L133:
	ldr	r3, [r7]
	lsls	r0, r3, #28
	bpl	.L137
	mov	r2, #664
	mov	r1, r6
	ldr	r0, .L217+32
	bl	printk
.L137:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #160]
	b	.L136
.L132:
	ldr	r3, [r7]
	lsls	r1, r3, #28
	bpl	.L138
	movw	r2, #669
	mov	r1, r6
	ldr	r0, .L217+36
	bl	printk
.L138:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #164]
	b	.L136
.L131:
	ldr	r3, [r7]
	lsls	r2, r3, #28
	bpl	.L139
	movw	r2, #674
	mov	r1, r6
	ldr	r0, .L217+40
	bl	printk
.L139:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #168]
	b	.L136
.L130:
	ldr	r3, [r7]
	lsls	r3, r3, #28
	bpl	.L140
	movw	r2, #679
	mov	r1, r6
	ldr	r0, .L217+44
	bl	printk
.L140:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #172]
	b	.L136
.L128:
	ldr	r3, [r7]
	lsls	r0, r3, #28
	bpl	.L141
	mov	r2, #684
	mov	r1, r6
	ldr	r0, .L217+48
	bl	printk
.L141:
	ldr	r3, [r5, #-4]
	str	r3, [r4, #184]
	b	.L136
.L127:
	movw	r2, #690
	mov	r1, r6
	mov	r0, r10
	bl	printk
	b	.L136
.L176:
	mov	r9, #2
	b	.L148
.L218:
	.align	2
.L217:
	.word	.LANCHOR0+148
	.word	hal_h264e_debug
	.word	.LANCHOR0+171
	.word	.LC0
	.word	-8192
	.word	.LANCHOR0+199
	.word	.LC20
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
.L152:
	cmp	r0, #0
	beq	.L153
.L154:
	str	r10, [r4, #148]
.L155:
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
	cmp	r8, r3
	bne	.L159
	ldr	r3, [r4, #20]
	ldr	r2, [sp, #24]
	cmp	r2, r3
	bne	.L159
	ldr	r3, [r4, #28]
	cmp	fp, r3
	bne	.L159
	ldr	r3, [r4, #32]
	cmp	r5, r3
	bne	.L159
	ldr	r3, [r4, #36]
	cmp	r6, r3
	bne	.L159
	ldr	r3, [sp, #28]
	cmp	r3, r9
	bge	.L145
.L159:
	ldr	r3, .L219
	ldr	r3, [r3]
	lsls	r2, r3, #28
	bpl	.L161
	ldr	r3, [sp, #28]
	mov	r2, #588
	str	r9, [sp, #8]
	ldr	r1, .L219+4
	strd	fp, r3, [sp]
	ldr	r0, .L219+8
	ldr	r3, [r4, #28]
	bl	printk
.L161:
	ldr	r3, [sp, #28]
	str	r8, [r4, #16]
	cmp	r3, r9
	it	lt
	movlt	r3, r9
	mov	r9, r3
	ldr	r3, [sp, #24]
	str	r3, [r4, #20]
	ldr	r3, [r4, #344]
	ldr	r3, [r3]
	cbnz	r3, .L162
	add	r0, r4, #12
	bl	hal_bufs_init
.L162:
	ldr	r2, [r4, #44]
	ldr	r3, [r4, #344]
	str	r5, [sp, #40]
	cmp	r2, #0
	beq	.L163
	movs	r2, #0
	strd	r2, r6, [sp, #44]
	str	r2, [sp, #52]
	ldr	r3, [r3]
	cbnz	r3, .L164
	add	r3, sp, #40
	movs	r2, #4
	mov	r1, r9
	ldr	r0, [r4, #12]
	bl	hal_bufs_setup
.L164:
	ldr	r3, [r4]
	movs	r0, #48
	movs	r1, #240
	ldr	r7, .L219+12
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
	ldr	r2, [sp, #32]
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
	ble	.L166
	ldr	r2, [r4, #364]
	strd	r0, r1, [r4, #116]
	strd	ip, r3, [r4, #68]
	cbz	r2, .L167
	str	r0, [r4, #120]
	str	ip, [r4, #72]
.L167:
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
.L175:
	ldr	r3, [r4, #344]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne	.L168
	ldr	r0, [r4, #48]
	cbz	r0, .L169
	ldr	r1, .L219+16
	bl	mpp_buffer_put_with_caller
.L169:
	ldr	r3, .L219+16
	mov	r2, r7
	add	r1, r4, #48
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L219+20
	bl	mpp_buffer_get_with_tag
.L171:
	ldr	r3, [r4, #344]
	ldr	r3, [r3]
	cbz	r3, .L173
	str	r3, [r4, #12]
.L173:
	strd	fp, r5, [r4, #28]
	strd	r6, r9, [r4, #36]
.L145:
	ldr	r3, [r4, #332]
	add	r3, r3, #4960
	adds	r3, r3, #24
	str	r3, [r4, #320]
	ldr	r3, [sp, #20]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_roi
	ldr	r3, [sp, #20]
	str	r0, [r4, #316]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_osd
	ldr	r3, .L219
	ldr	r3, [r3]
	lsls	r3, r3, #30
	str	r0, [r4, #328]
	bpl	.L203
	mov	r3, r4
	mov	r2, #716
	ldr	r1, .L219+24
	ldr	r0, .L219+28
	bl	printk
.L203:
	movs	r0, #0
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L151:
	str	r3, [r4, #336]
	b	.L154
.L150:
	ldr	r0, [r4, #336]
	cbz	r0, .L156
	ldr	r3, [r4, #344]
	ldr	r10, [r3, #8]
	cmp	r10, #0
	bne	.L156
	ldr	r1, .L219+32
	bl	mpp_buffer_put_with_caller
	str	r10, [r4, #336]
.L156:
	movs	r3, #0
	str	r3, [r4, #148]
	b	.L155
.L168:
	str	r3, [r4, #48]
	b	.L171
.L163:
	strd	r2, r6, [sp, #44]
	str	fp, [sp, #52]
	ldr	r3, [r3]
	cbnz	r3, .L172
	add	r3, sp, #40
	movs	r2, #4
	mov	r1, r9
	ldr	r0, [r4, #12]
	bl	hal_bufs_setup
.L172:
	strd	r8, fp, [r4, #24]
	b	.L171
.L166:
	movs	r2, #0
	strd	r3, r7, [r4, #124]
	strd	r2, r3, [r4, #76]
	str	r2, [r4, #84]
	str	r3, [r4, #132]
	b	.L175
.L220:
	.align	2
.L219:
	.word	hal_h264e_debug
	.word	.LANCHOR0+214
	.word	.LC21
	.word	-4096
	.word	.LANCHOR0+229
	.word	.LC20
	.word	.LANCHOR0+171
	.word	.LC9
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
	ldr	r5, .L254
	ldr	r3, [r5]
	lsls	r2, r3, #30
	bpl	.L222
	mov	r3, r0
	movs	r2, #183
	ldr	r1, .L254+4
	ldr	r0, .L254+8
	bl	printk
.L222:
	ldr	r0, [r4, #4]
	cbz	r0, .L223
	bl	mpp_dev_deinit
	movs	r3, #0
	str	r3, [r4, #4]
.L223:
	ldr	r3, [r4, #344]
	ldr	r6, [r3, #8]
	cbnz	r6, .L224
	ldr	r0, [r4, #336]
	cbz	r0, .L224
	ldr	r1, .L254+12
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #336]
.L224:
	ldr	r3, [r4, #344]
	ldr	r6, [r3]
	cbnz	r6, .L225
	ldr	r0, [r4, #12]
	cbz	r0, .L225
	bl	hal_bufs_deinit
	str	r6, [r4, #12]
.L225:
	ldr	r0, [r4, #152]
	cbz	r0, .L226
	ldr	r1, .L254+12
	bl	mpp_buffer_put_with_caller
	movs	r3, #0
	str	r3, [r4, #152]
.L226:
	ldr	r3, [r4, #344]
	ldr	r6, [r3, #4]
	cbnz	r6, .L227
	ldr	r0, [r4, #48]
	cbz	r0, .L227
	ldr	r1, .L254+12
	bl	mpp_buffer_put_with_caller
	str	r6, [r4, #48]
.L227:
	ldr	r1, [r4, #332]
	cbz	r1, .L228
	ldr	r0, .L254+12
	bl	mpp_osal_free
.L228:
	movs	r3, #0
	str	r3, [r4, #332]
	ldr	r3, [r5]
	lsls	r3, r3, #30
	bpl	.L229
	mov	r3, r4
	movs	r2, #211
	ldr	r1, .L254+4
	ldr	r0, .L254+16
	bl	printk
.L229:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L255:
	.align	2
.L254:
	.word	hal_h264e_debug
	.word	.LANCHOR0+242
	.word	.LC0
	.word	.LANCHOR0+268
	.word	.LC9
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
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r5, r0
	ldr	r7, .L294
	mov	r4, r1
	ldr	r3, [r7]
	lsls	r0, r3, #30
	bpl	.L257
	mov	r3, r5
	movs	r2, #220
	ldr	r1, .L294+4
	ldr	r0, .L294+8
	bl	printk
.L257:
	ldr	r3, [r4, #4]
	movs	r1, #16
	add	r0, r4, #36
	str	r3, [r5]
	ldr	r3, [r4, #8]
	str	r3, [r5, #340]
	ldr	r3, [r4, #12]
	str	r3, [r5, #44]
	ldr	r3, [r4, #40]
	str	r3, [r5, #344]
	ldr	r3, [r4, #16]
	str	r3, [r5, #348]
	ldr	r3, [r4, #20]
	str	r3, [r5, #352]
	ldr	r3, [r4, #24]
	str	r3, [r5, #360]
	ldr	r3, [r4, #28]
	str	r3, [r5, #364]
	str	r1, [r4, #32]
	bl	mpp_dev_init
	mov	r6, r0
	cbz	r0, .L258
	mov	r3, r0
	movs	r2, #234
	ldr	r1, .L294+4
	ldr	r0, .L294+12
	bl	printk
	mov	r0, r5
	bl	hal_h264e_vepu540c_deinit
.L259:
	ldr	r3, [r7]
	lsls	r3, r3, #30
	bpl	.L256
	mov	r3, r5
	mov	r2, #282
	ldr	r1, .L294+4
	ldr	r0, .L294+16
	bl	printk
	b	.L256
.L258:
	ldr	r3, [r4, #36]
	movs	r1, #1
	str	r3, [r5, #4]
	str	r3, [r5, #324]
	movs	r3, #2
	ldr	r2, [r4, #4]
	strd	r3, r1, [r2, #304]
	add	r3, r2, #312
	ldr	r1, [r5, #352]
	cmp	r1, #0
	beq	.L260
	ldr	ip, .L294+36
	add	lr, ip, #64
	mov	r8, lr
.L261:
	mov	r4, ip
	adds	r3, r3, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, lr
	str	r0, [r3, #-8]	@ unaligned
	mov	ip, r4
	str	r1, [r3, #-4]	@ unaligned
	bne	.L261
	ldr	ip, .L294+36
	add	r3, r2, #376
.L262:
	mov	r4, ip
	adds	r3, r3, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, r8
	str	r0, [r3, #-8]	@ unaligned
	mov	ip, r4
	str	r1, [r3, #-4]	@ unaligned
	bne	.L262
	ldr	ip, .L294+40
	add	r3, r2, #440
	add	lr, ip, #64
.L263:
	mov	r4, ip
	adds	r3, r3, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, lr
	str	r0, [r3, #-8]	@ unaligned
	mov	ip, r4
	str	r1, [r3, #-4]	@ unaligned
	bne	.L263
	ldr	r3, .L294+20
	add	r2, r2, #504
	add	ip, r3, #64
.L264:
	mov	r4, r3
	adds	r2, r2, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, ip
	str	r0, [r2, #-8]	@ unaligned
	mov	r3, r4
	str	r1, [r2, #-4]	@ unaligned
	bne	.L264
.L265:
	movw	r1, #6200
	ldr	r0, .L294+24
	bl	mpp_osal_calloc
	str	r0, [r5, #332]
	cmp	r0, #0
	bne	.L259
	movw	r1, #275
	ldr	r0, .L294+28
	bl	printk
	mvn	r6, #3
.L256:
	mov	r0, r6
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L260:
	ldr	r1, [r5]
	movw	r0, #4708
	add	r8, r2, #376
	add	r4, r2, #440
	add	r2, r2, #504
	ldr	r1, [r1, r0]
	cmp	r1, #1
	bne	.L266
	ldr	r9, .L294+44
	mov	lr, r3
	add	r3, r9, #64
	mov	r10, r3
.L267:
	mov	ip, r9
	add	lr, lr, #8
	ldmia	ip!, {r0, r1}
	cmp	ip, r3
	str	r0, [lr, #-8]	@ unaligned
	mov	r9, ip
	str	r1, [lr, #-4]	@ unaligned
	bne	.L267
	ldr	lr, .L294+44
	mov	r3, r8
.L268:
	mov	ip, lr
	adds	r3, r3, #8
	ldmia	ip!, {r0, r1}
	cmp	ip, r10
	str	r0, [r3, #-8]	@ unaligned
	mov	lr, ip
	str	r1, [r3, #-4]	@ unaligned
	bne	.L268
	ldr	lr, .L294+48
	mov	r3, r4
	add	r4, lr, #64
.L269:
	mov	ip, lr
	adds	r3, r3, #8
	ldmia	ip!, {r0, r1}
	cmp	ip, r4
	str	r0, [r3, #-8]	@ unaligned
	mov	lr, ip
	str	r1, [r3, #-4]	@ unaligned
	bne	.L269
	ldr	ip, .L294+52
	mov	r3, r2
	add	r2, ip, #64
.L270:
	mov	r4, ip
	adds	r3, r3, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, r2
	str	r0, [r3, #-8]	@ unaligned
	mov	ip, r4
	str	r1, [r3, #-4]	@ unaligned
	bne	.L270
	b	.L265
.L266:
	ldr	lr, .L294+56
	add	r10, lr, #64
	mov	r9, r10
.L271:
	mov	ip, lr
	adds	r3, r3, #8
	ldmia	ip!, {r0, r1}
	cmp	ip, r10
	str	r0, [r3, #-8]	@ unaligned
	mov	lr, ip
	str	r1, [r3, #-4]	@ unaligned
	bne	.L271
	ldr	lr, .L294+56
	mov	r3, r8
.L272:
	mov	ip, lr
	adds	r3, r3, #8
	ldmia	ip!, {r0, r1}
	cmp	ip, r9
	str	r0, [r3, #-8]	@ unaligned
	mov	lr, ip
	str	r1, [r3, #-4]	@ unaligned
	bne	.L272
	ldr	ip, .L294+60
	mov	r3, r4
	add	lr, ip, #64
.L273:
	mov	r4, ip
	adds	r3, r3, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, lr
	str	r0, [r3, #-8]	@ unaligned
	mov	ip, r4
	str	r1, [r3, #-4]	@ unaligned
	bne	.L273
	ldr	r4, .L294+32
	mov	r3, r2
	add	ip, r4, #64
.L274:
	mov	r2, r4
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, ip
	str	r0, [r3, #-8]	@ unaligned
	mov	r4, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L274
	b	.L265
.L295:
	.align	2
.L294:
	.word	hal_h264e_debug
	.word	.LANCHOR0+294
	.word	.LC0
	.word	.LC22
	.word	.LC9
	.word	.LANCHOR1+128
	.word	.LANCHOR0+318
	.word	.LC23
	.word	.LANCHOR1+512
	.word	.LANCHOR1
	.word	.LANCHOR1+64
	.word	.LANCHOR1+192
	.word	.LANCHOR1+256
	.word	.LANCHOR1+320
	.word	.LANCHOR1+384
	.word	.LANCHOR1+448
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
	ldr	r4, .L309
	ldr	r6, [r0]
	ldr	r2, [r4]
	lsls	r2, r2, #30
	bpl	.L297
	mov	r3, r0
	mov	r2, #628
	ldr	r1, .L309+4
	ldr	r0, .L309+8
	bl	printk
.L297:
	ldr	r3, [r6, #8]
	tst	r3, #5
	itt	ne
	movne	r3, #0
	strne	r3, [r6, #8]
	ldr	r3, [r4]
	lsls	r3, r3, #30
	bpl	.L299
	mov	r3, r5
	movw	r2, #643
	ldr	r1, .L309+4
	ldr	r0, .L309+12
	bl	printk
.L299:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L310:
	.align	2
.L309:
	.word	hal_h264e_debug
	.word	.LANCHOR0+342
	.word	.LC0
	.word	.LC9
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
	ldr	r5, .L324
	ldr	r2, [r5]
	lsls	r2, r2, #30
	bpl	.L312
	mov	r3, r0
	movw	r2, #2606
	ldr	r1, .L324+4
	ldr	r0, .L324+8
	bl	printk
.L312:
	movs	r2, #0
	movs	r1, #6
	ldr	r0, [r6, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L313
	mov	r3, r0
	movw	r2, #2609
	ldr	r1, .L324+4
	mvn	r4, #1008
	ldr	r0, .L324+12
	bl	printk
.L313:
	ldr	r3, [r5]
	lsls	r3, r3, #30
	bpl	.L311
	mov	r3, r6
	movw	r2, #2612
	ldr	r1, .L324+4
	ldr	r0, .L324+16
	bl	printk
.L311:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L325:
	.align	2
.L324:
	.word	hal_h264e_debug
	.word	.LANCHOR0+369
	.word	.LC0
	.word	.LC24
	.word	.LC9
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
	@ args = 0, pretend = 0, frame = 120
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #148
	sub	sp, sp, #148
	ldr	r3, [r0]
	mov	r5, r0
	ldr	r4, [r0, #332]
	ldrd	fp, r6, [r0, #160]
	str	r3, [sp, #24]
	ldr	r3, [r1, #4]
	ldr	r10, [r0, #168]
	ldr	r0, [r1, #52]
	str	r3, [sp, #80]
	str	r1, [sp, #28]
	bl	mpp_frame_get_is_full
	ldr	r7, .L725
	ldr	r3, [r7]
	str	r0, [sp, #84]
	lsls	r0, r3, #30
	bpl	.L327
	mov	r3, r5
	movw	r2, #2321
	ldr	r1, .L725+4
	ldr	r0, .L725+8
	bl	printk
.L327:
	ldr	r3, [r7]
	lsls	r1, r3, #28
	bpl	.L328
	ldr	r3, [r5, #172]
	movw	r2, #2322
	ldr	r1, .L725+4
	ldr	r0, .L725+12
	ldr	r3, [r3]
	bl	printk
.L328:
	movw	r2, #6200
	movs	r1, #0
	mov	r0, r4
	bl	memset
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L329
	movw	r2, #723
	ldr	r1, .L725+16
	ldr	r0, .L725+20
	bl	printk
.L329:
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
	ldr	r3, [r7]
	lsls	r3, r3, #30
	bpl	.L330
	mov	r2, #800
	ldr	r1, .L725+16
	ldr	r0, .L725+24
	bl	printk
.L330:
	ldr	r3, [sp, #24]
	add	r0, sp, #108
	ldr	r8, [r3, #36]
	mov	r1, r8
	bl	vepu541_set_fmt
	ldr	r3, [r7]
	str	r0, [sp, #76]
	lsls	r0, r3, #30
	ldr	r9, [sp, #108]
	bpl	.L331
	movw	r2, #813
	ldr	r1, .L725+28
	ldr	r0, .L725+20
	bl	printk
.L331:
	ldr	r3, [sp, #76]
	cmp	r3, #0
	bne	.L326
	ldr	r3, [sp, #24]
	ldrh	r2, [r4, #260]
	ldr	r3, [r3, #12]
	adds	r3, r3, #15
	bics	r3, r3, #15
	it	mi
	addmi	r3, r3, #7
	asrs	r3, r3, #3
	subs	r3, r3, #1
	bfi	r2, r3, #0, #11
	ldr	r3, [sp, #24]
	strh	r2, [r4, #260]	@ movhi
	ldrsb	r2, [r3, #12]
	add	r3, r2, #15
	bic	r3, r3, #15
	subs	r3, r3, r2
	ldrb	r2, [r4, #264]	@ zero_extendqisi2
	bfi	r2, r3, #0, #6
	ldr	r3, [sp, #24]
	strb	r2, [r4, #264]
	ldrh	r2, [r4, #262]
	ldr	r3, [r3, #16]
	adds	r3, r3, #15
	bics	r3, r3, #15
	it	mi
	addmi	r3, r3, #7
	tst	r8, #15728640
	asr	r3, r3, #3
	add	r3, r3, #-1
	bfi	r2, r3, #0, #11
	strh	r2, [r4, #262]	@ movhi
	ldr	r3, [sp, #24]
	ldrsb	r2, [r3, #16]
	add	r3, r2, #15
	bic	r3, r3, #15
	sub	r3, r3, r2
	ldrb	r2, [r4, #266]	@ zero_extendqisi2
	bfi	r2, r3, #0, #6
	strb	r2, [r4, #266]
	ldr	r2, [sp, #124]
	ldrb	r3, [r4, #48]	@ zero_extendqisi2
	bfi	r3, r2, #4, #4
	strb	r3, [r4, #48]
	ldrb	r3, [r4, #268]	@ zero_extendqisi2
	bfi	r3, r9, #2, #4
	strb	r3, [r4, #268]
	ldr	r2, [sp, #112]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #268]
	ldr	r2, [sp, #116]
	bfi	r3, r2, #1, #1
	orn	r3, r3, #127
	strb	r3, [r4, #268]
	ldr	r3, [sp, #24]
	beq	.L335
	ldr	r3, [r3, #12]
	adds	r3, r3, #15
	bic	r3, r3, #15
.L336:
	sub	r2, r9, #4
	cmp	r2, #9
	bhi	.L338
	tbh	[pc, r2, lsl #1]
.L346:
	.2byte	(.L541-.L346)/2
	.2byte	(.L338-.L346)/2
	.2byte	(.L541-.L346)/2
	.2byte	(.L338-.L346)/2
	.2byte	(.L338-.L346)/2
	.2byte	(.L338-.L346)/2
	.2byte	(.L338-.L346)/2
	.2byte	(.L338-.L346)/2
	.2byte	(.L344-.L346)/2
	.2byte	(.L541-.L346)/2
	.p2align 1
.L726:
	.align	2
.L725:
	.word	hal_h264e_debug
	.word	.LANCHOR0+393
	.word	.LC0
	.word	.LC25
	.word	.LANCHOR0+421
	.word	.LC26
	.word	.LC27
	.word	.LANCHOR0+443
.L335:
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne	.L336
	ldr	r3, [sp, #24]
	ldr	r3, [r3, #12]
	cmp	r9, #0
	bne	.L337
	lsls	r3, r3, #2
.L338:
	add	r2, r3, r3, lsr #31
	mov	r1, r3
	asrs	r3, r2, #1
.L345:
	cmp	r9, #2
	bhi	.L542
	ldr	r2, [r4, #272]
	and	r0, r2, #-134217728
	ldr	r2, .L727
	orrs	r2, r2, r0
	str	r2, [r4, #272]
	ldr	r2, [r4, #276]
	and	r0, r2, #-134217728
	ldr	r2, .L727+4
	orrs	r2, r2, r0
	str	r2, [r4, #276]
	ldr	r2, [r4, #280]
	and	r0, r2, #-134217728
	ldr	r2, .L727+8
	orrs	r2, r2, r0
	str	r2, [r4, #280]
	ldr	r2, [r4, #284]
	lsrs	r2, r2, #21
	lsls	r2, r2, #21
	orr	r2, r2, #32768
	orr	r2, r2, #128
	str	r2, [r4, #284]
.L347:
	ldr	r2, [r4, #296]
	strh	r3, [r4, #300]	@ movhi
	bfi	r2, r1, #0, #17
	str	r2, [r4, #296]
	ldr	r3, [sp, #24]
	ldr	r7, .L727+12
	ldr	r3, [r3, #60]
	cmp	r3, #0
	ldrb	r3, [r4, #291]	@ zero_extendqisi2
	ite	le
	movle	r2, #0
	movgt	r2, #1
	bfi	r3, r2, #2, #1
	strb	r3, [r4, #291]
	ldr	r2, [sp, #24]
	ldr	r2, [r2, #56]
	bfi	r3, r2, #3, #2
	strb	r3, [r4, #291]
	ldr	r2, [r7]
	ldrh	r3, [r4, #484]
	lsls	r1, r2, #30
	and	r3, r3, #16384
	strh	r3, [r4, #484]	@ movhi
	ldr	r3, [r4, #292]
	and	r3, r3, #-1073692672
	str	r3, [r4, #292]
	bpl	.L348
	movw	r2, #914
	ldr	r1, .L727+16
	ldr	r0, .L727+20
	bl	printk
.L348:
	ldr	r3, [r7]
	lsls	r3, r3, #30
	bpl	.L538
	movw	r2, #922
	ldr	r1, .L727+24
	ldr	r0, .L727+28
	bl	printk
.L538:
	ldrb	r3, [r4, #244]	@ zero_extendqisi2
	bfc	r3, #0, #2
	strb	r3, [r4, #244]
	ldr	r3, [r10, #32]
	cmp	r3, #0
	ldrb	r3, [r4, #244]	@ zero_extendqisi2
	ite	le
	movle	r2, #0
	movgt	r2, #1
	bfi	r3, r2, #2, #1
	orr	r3, r3, #16
	strb	r3, [r4, #244]
	ldrb	r3, [r4, #420]	@ zero_extendqisi2
	ldr	r2, [r10, #32]
	bfi	r3, r2, #0, #2
	strb	r3, [r4, #420]
	ldr	r2, [r10, #36]
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
	ldrb	r3, [r4, #428]	@ zero_extendqisi2
	ldr	r2, [r6, #8]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #428]
	ldr	r2, [r6, #68]
	bfi	r3, r2, #1, #1
	strb	r3, [r4, #428]
	ldr	r2, [r6, #60]
	bfi	r3, r2, #2, #1
	strb	r3, [r4, #428]
	ldr	r3, [r6, #20]
	ldrb	r2, [r4, #428]	@ zero_extendqisi2
	subs	r3, r3, #1
	bfi	r2, r3, #3, #2
	strb	r2, [r4, #428]
	ldr	r3, [r6, #24]
	uxtb	r2, r2
	subs	r3, r3, #1
	bfi	r2, r3, #5, #2
	strb	r2, [r4, #428]
	ldr	r2, [r6, #36]
	ldrh	r3, [r4, #428]
	bfi	r3, r2, #7, #6
	strh	r3, [r4, #428]	@ movhi
	ldr	r2, [r6, #44]
	ldr	r3, [r4, #428]
	bfi	r3, r2, #13, #5
	str	r3, [r4, #428]
	ldr	r2, [r6, #52]
	ubfx	r3, r3, #16, #8
	bfi	r3, r2, #2, #5
	strb	r3, [r4, #430]
	ldrb	r3, [r4, #431]	@ zero_extendqisi2
	ldr	r2, [r6, #56]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #431]
	ldr	r3, [r10, #44]
	cmp	r3, #2
	ldrb	r3, [r4, #432]	@ zero_extendqisi2
	ite	eq
	moveq	r2, #2
	movne	r2, #0
	bfi	r3, r2, #0, #2
	strb	r3, [r4, #432]
	ldr	r2, [r10, #52]
	ldrh	r3, [r4, #432]
	bfi	r3, r2, #2, #8
	strh	r3, [r4, #432]	@ movhi
	ubfx	r3, r3, #8, #8
	bfc	r3, #2, #1
	strb	r3, [r4, #433]
	ldr	r2, [r10, #60]
	bfi	r3, r2, #3, #1
	strb	r3, [r4, #433]
	ldr	r2, [r10, #68]
	bfi	r3, r2, #4, #2
	strb	r3, [r4, #433]
	ldr	r3, [r10, #56]
	strh	r3, [r4, #434]	@ movhi
	ldr	r3, [r10, #44]
	cmp	r3, #2
	ite	eq
	ldrheq	r3, [r10, #100]
	movwne	r3, #65535
	strh	r3, [r4, #436]	@ movhi
	ldr	r3, [r10, #108]
	strh	r3, [r4, #438]	@ movhi
	ldrb	r3, [r4, #441]	@ zero_extendqisi2
	ldr	r2, [r10, #72]
	bfi	r3, r2, #3, #2
	strb	r3, [r4, #441]
	ldr	r2, [r10, #76]
	ldrh	r3, [r4, #440]
	bfi	r3, r2, #7, #4
	strh	r3, [r4, #440]	@ movhi
	ldr	r0, [r10, #88]
	bl	h264e_reorder_rd_rewind
	ldr	r0, [r10, #88]
	add	r1, sp, #108
	bl	h264e_reorder_rd_op
	cmp	r0, #0
	bne	.L351
	ldrb	r3, [r4, #440]	@ zero_extendqisi2
	orr	r3, r3, #4
	strb	r3, [r4, #440]
	ldr	r3, [sp, #108]
	ldrb	r2, [r4, #440]	@ zero_extendqisi2
	cmp	r3, #0
	bfi	r2, r3, #0, #2
	strb	r2, [r4, #440]
	blt	.L352
	cmp	r3, #1
	ble	.L353
	cmp	r3, #2
	beq	.L354
.L352:
	movw	r2, #995
	ldr	r1, .L727+24
	ldr	r0, .L727+32
	bl	printk
	b	.L355
.L337:
	cmp	r9, #1
	bne	.L339
	add	r3, r3, r3, lsl #1
	b	.L338
.L339:
	cmp	r9, #2
	beq	.L340
	sub	r2, r9, #8
	cmp	r2, #1
	bhi	.L341
.L340:
	lsls	r3, r3, #1
	b	.L336
.L341:
	sub	r2, r9, #4
	cmp	r2, #9
	bhi	.L338
	adr	r1, .L343
	ldr	pc, [r1, r2, lsl #2]
	.p2align 2
.L343:
	.word	.L540+1
	.word	.L338+1
	.word	.L540+1
	.word	.L338+1
	.word	.L338+1
	.word	.L338+1
	.word	.L338+1
	.word	.L338+1
	.word	.L344+1
	.word	.L540+1
	.p2align 1
.L344:
	lsls	r7, r3, #1
.L342:
	ldr	r2, [sp, #128]
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
	ldr	r2, [sp, #132]
	ldr	r1, [r2]
	bfi	r0, r1, #0, #5
	strb	r0, [r4, #286]
	ldr	r1, [r2, #4]
	strb	r1, [r4, #285]
	mov	r1, r3
	ldr	r2, [r2, #8]
	mov	r3, r7
	strb	r2, [r4, #284]
	b	.L347
.L728:
	.align	2
.L727:
	.word	20261917
	.word	123164288
	.word	33762283
	.word	hal_h264e_debug
	.word	.LANCHOR0+443
	.word	.LC27
	.word	.LANCHOR0+463
	.word	.LC26
	.word	.LC28
.L541:
	mov	r1, r3
	b	.L345
.L540:
	mov	r7, r3
	b	.L342
.L542:
	mov	r7, r3
	mov	r3, r1
	b	.L342
.L353:
	ldr	r3, [sp, #112]
.L710:
	strh	r3, [r4, #442]	@ movhi
.L355:
	ldr	r3, [r4, #444]
	and	r3, r3, #-536870912
	str	r3, [r4, #444]
	movs	r3, #0
	str	r3, [r4, #448]
	ldrh	r3, [r4, #452]
	and	r3, r3, #61440
	strh	r3, [r4, #452]	@ movhi
	ldr	r0, [r10, #92]
	bl	h264e_marking_rd_rewind
	ldr	r3, [r10, #44]
	cmp	r3, #2
	bne	.L356
	ldr	r2, [r10, #116]
	ldrb	r3, [r4, #444]	@ zero_extendqisi2
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #444]
	ldr	r2, [r10, #120]
	bfi	r3, r2, #1, #1
	strb	r3, [r4, #444]
.L357:
	ldr	r3, .L729
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #2
	beq	.L385
	movw	r2, #1204
	ldr	r1, .L729+4
	ldr	r0, .L729+8
	bl	printk
.L385:
	ldr	r3, [r8]
	ldr	r7, [r5, #332]
	lsls	r3, r3, #30
	bpl	.L386
	movw	r2, #1211
	ldr	r1, .L729+12
	ldr	r0, .L729+16
	bl	printk
.L386:
	ldr	r3, [r10, #44]
	cmp	r3, #2
	bne	.L387
	ldrb	r3, [r7, #756]	@ zero_extendqisi2
	movs	r2, #6
.L711:
	bfi	r3, r2, #0, #4
	strb	r3, [r7, #756]
	ldr	r3, [fp]
	cmp	r3, #66
	bne	.L549
	ldr	r3, [fp, #28]
	cmp	r3, #30
	ite	gt
	movgt	r3, #0
	movle	r3, #1
.L390:
	ldrb	r2, [r7, #404]	@ zero_extendqisi2
	bfi	r2, r3, #0, #1
	strb	r2, [r7, #404]
	ldr	r3, [fp]
	ldrb	r2, [r7, #404]	@ zero_extendqisi2
	cmp	r3, #76
	itett	le
	ldrle	r3, [r6, #8]
	movgt	r3, #0
	clzle	r3, r3
	lsrle	r3, r3, #5
	bfi	r2, r3, #3, #1
	strb	r2, [r7, #404]
	ldrh	r3, [r7, #404]
	orr	r3, r3, #8192
	orr	r3, r3, #16
	strh	r3, [r7, #404]	@ movhi
	ldrb	r3, [r7, #406]	@ zero_extendqisi2
	ldr	r2, [r6, #72]
	bfi	r3, r2, #4, #2
	strb	r3, [r7, #406]
	ldr	r2, [r5]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	ldrb	r2, [r7, #407]	@ zero_extendqisi2
	subs	r1, r3, #1
	rsbs	r3, r1, #0
	adcs	r3, r3, r1
	bfi	r2, r3, #4, #1
	strb	r2, [r7, #407]
	ldr	r3, [r7, #404]
	orr	r3, r3, #536870912
	orr	r3, r3, #32768
	str	r3, [r7, #404]
	ldrh	r3, [r7, #408]
	bfc	r3, #0, #9
	strh	r3, [r7, #408]	@ movhi
	ldr	r3, [r8]
	lsls	r0, r3, #30
	bpl	.L392
	movw	r2, #1233
	ldr	r1, .L729+12
	ldr	r0, .L729+8
	bl	printk
.L392:
	ldr	r3, [sp, #28]
	ldr	r6, [r5, #332]
	ldr	r9, [r5, #168]
	ldr	r0, [r3, #52]
	ldr	r8, [r5, #292]
	bl	mpp_frame_get_ppinfo
	ldr	r3, .L729
	ldr	r3, [r3]
	lsls	r2, r3, #30
	mov	r7, r0
	bpl	.L393
	movw	r2, #1246
	ldr	r1, .L729+20
	ldr	r0, .L729+16
	bl	printk
.L393:
	ldr	r2, [r5]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	cmp	r3, #1
	ldrh	r3, [r6, #1644]
	and	r3, r3, #32512
	bne	.L394
	orr	r3, r3, #32768
	orr	r3, r3, #9
	strh	r3, [r6, #1644]	@ movhi
	ldr	r3, [r5, #352]
	cbz	r3, .L395
	movs	r3, #16
	strb	r3, [r6, #1644]
.L395:
	ldr	r2, [r5, #312]
	ldrd	r3, r1, [r5, #304]
	add	r3, r3, r1
	cmp	r3, r2, lsr #1
	ite	hi
	movhi	r3, #1
	movls	r3, #0
	cmp	r2, r8, lsr #8
	bcs	.L396
	subs	r3, r3, #8
.L397:
	ldrb	r2, [r6, #1645]	@ zero_extendqisi2
	bfi	r2, r3, #0, #4
	mov	r3, r2
	bfc	r3, #4, #1
	strb	r3, [r6, #1645]
	ldr	r3, [r9, #44]
	cmp	r3, #2
	ldrb	r3, [r6, #1645]	@ zero_extendqisi2
	bne	.L400
.L724:
	movs	r2, #1
.L712:
	bfi	r3, r2, #5, #2
	ldr	r1, .L729+24
	strb	r3, [r6, #1645]
	ldr	r3, [r6, #1628]
	ldrh	r2, [r6, #1624]
	ands	r1, r1, r3
	ldr	r3, .L729+28
	bic	r2, r2, #1016
	orrs	r3, r3, r1
	ldr	r1, [r6, #1632]
	str	r3, [r6, #1628]
	orr	r2, r2, #256
	ldr	r3, .L729+32
	strh	r2, [r6, #1624]	@ movhi
	ands	r3, r3, r1
	str	r3, [r6, #1632]
	ldrh	r3, [r6, #1646]
	and	r3, r3, #14
	orr	r3, r3, #1
	strh	r3, [r6, #1646]	@ movhi
	cmp	r7, #0
	bne	.L403
.L404:
	ldr	r3, [r6, #1648]
	and	r3, r3, #-268374016
	orr	r3, r3, #1572864
	str	r3, [r6, #1648]
	ldr	r3, [r6, #1652]
	and	r3, r3, #-268374016
	orr	r3, r3, #4194304
	orr	r3, r3, #48
	str	r3, [r6, #1652]
	ldr	r3, [r6, #1656]
	and	r3, r3, #-268374016
	orr	r3, r3, #2097152
	orr	r3, r3, #16
	str	r3, [r6, #1656]
	ldr	r3, [r6, #1660]
	and	r3, r3, #-268374016
	orr	r3, r3, #6291456
	orr	r3, r3, #48
	str	r3, [r6, #1660]
	ldr	r3, [r6, #1664]
	and	r3, r3, #-268374016
	orr	r3, r3, #1572864
	orr	r3, r3, #48
	str	r3, [r6, #1664]
	ldr	r3, [r6, #1668]
	and	r3, r3, #-268374016
	orr	r3, r3, #3145728
	orr	r3, r3, #96
	str	r3, [r6, #1668]
	ldr	r3, [r6, #1672]
	and	r3, r3, #-252645136
	orr	r3, r3, #16843009
	orr	r3, r3, #33554944
	str	r3, [r6, #1672]
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
	bic	r3, r3, #3840
	bic	r3, r3, #15
	orr	r3, r3, #768
	orr	r3, r3, #1
	strh	r3, [r6, #1684]	@ movhi
	ldr	r3, [r6, #1688]
	and	r3, r3, #-1061109568
	orr	r3, r3, #150997248
	orr	r3, r3, #393222
	str	r3, [r6, #1688]
	ldr	r3, [r6, #1692]
	and	r2, r3, #-1061109568
	ldr	r3, .L729+36
	orrs	r3, r3, r2
	str	r3, [r6, #1692]
	ldr	r3, [r6, #1696]
	and	r2, r3, #-1061109568
	ldr	r3, .L729+40
	orrs	r3, r3, r2
	str	r3, [r6, #1696]
	ldrh	r3, [r6, #1700]
	bic	r3, r3, #16128
	bic	r3, r3, #63
	orr	r3, r3, #4096
	orr	r3, r3, #7
	strh	r3, [r6, #1700]	@ movhi
	ldr	r3, [r6, #1704]
	and	r3, r3, #-16711936
	orr	r3, r3, #393216
	orr	r3, r3, #10
	str	r3, [r6, #1704]
	ldr	r3, [r6, #1708]
	and	r2, r3, #-16777216
	ldr	r3, .L729+44
	orrs	r3, r3, r2
	str	r3, [r6, #1708]
	ldr	r3, [r6, #1736]
	and	r3, r3, #-268374016
	orr	r3, r3, #655360
	orr	r3, r3, #1
	str	r3, [r6, #1736]
	ldr	r3, [r6, #1740]
	and	r3, r3, #-268374016
	orr	r3, r3, #1638400
	orr	r3, r3, #15
	str	r3, [r6, #1740]
	ldr	r3, .L729+48
	str	r3, [r6, #1744]
	movs	r3, #16
	strb	r3, [r6, #1748]
	ldr	r3, [r5, #352]
	cbz	r3, .L405
	ldrh	r3, [r6, #1738]
	movs	r2, #7
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #1738]	@ movhi
	movs	r3, #14
	strb	r3, [r6, #1745]
.L405:
	ldr	r3, [r6, #1764]
	movs	r1, #72
	ldr	r0, .L729+52
	mov	r2, #269488144
	and	r3, r3, #-268374016
	str	r2, [r6, #1772]
	orrs	r3, r3, r0
	str	r3, [r6, #1764]
	ldrh	r3, [r6, #1768]
	bfi	r3, r1, #0, #12
	strh	r3, [r6, #1768]	@ movhi
	ldr	r3, [r6, #1788]
	and	r3, r3, #-268374016
	orrs	r3, r3, r0
	str	r3, [r6, #1788]
	b	.L730
.L731:
	.align	2
.L729:
	.word	hal_h264e_debug
	.word	.LANCHOR0+463
	.word	.LC27
	.word	.LANCHOR0+484
	.word	.LC26
	.word	.LANCHOR0+508
	.word	-536346112
	.word	1049601
	.word	-267648256
	.word	134613766
	.word	335942665
	.word	11088209
	.word	269488148
	.word	2621460
.L730:
	ldrh	r3, [r6, #1792]
	bfi	r3, r1, #0, #12
	strh	r3, [r6, #1792]	@ movhi
	ldr	r3, .L732
	str	r3, [r6, #1796]
	ldr	r3, [r6, #1804]
	and	r3, r3, #-252645136
	orr	r3, r3, #67109888
	orr	r3, r3, #65537
	str	r3, [r6, #1804]
	ldr	r3, [r6, #1808]
	and	r1, r3, #-1061109568
	ldr	r3, .L732+4
	orrs	r3, r3, r1
	str	r3, [r6, #1808]
	ldr	r1, [r5]
	movw	r3, #4708
	ldr	r3, [r1, r3]
	cmp	r3, #1
	ittt	ne
	strne	r2, [r6, #1796]
	movne	r3, #16
	strbne	r3, [r6, #1744]
	ldr	r6, .L732+8
	ldr	r3, [r6]
	lsls	r7, r3, #30
	bpl	.L407
	movw	r2, #1418
	ldr	r1, .L732+12
	ldr	r0, .L732+16
	bl	printk
.L407:
	ldr	r3, [r6]
	ldr	r7, [r5, #332]
	lsls	r0, r3, #30
	bpl	.L408
	movw	r2, #1481
	ldr	r1, .L732+20
	ldr	r0, .L732+24
	bl	printk
.L408:
	ldr	r3, [r5, #164]
	add	r0, r7, #1912
	mov	r2, #2304
	ldr	r3, [r3, #72]
	cmp	r3, #2
	bne	.L409
	ldr	r1, .L732+28
.L713:
	bl	memcpy
	ldr	r3, [r6]
	lsls	r1, r3, #30
	bpl	.L412
	movw	r2, #1490
	ldr	r1, .L732+20
	ldr	r0, .L732+16
	bl	printk
.L412:
	ldr	r3, [sp, #28]
	ldr	r8, [fp, #60]
	ldr	r1, [fp, #64]
	ldr	r3, [r3, #4]
	mul	r1, r1, r8
	ldr	r2, [r3, #172]
	ldr	r7, [r3, #160]
	str	r2, [sp, #40]
	ldr	r2, [r3, #180]
	lsls	r0, r7, #4
	str	r2, [sp, #60]
	ldr	r2, [r3, #176]
	str	r2, [sp, #72]
	bl	__aeabi_idiv
	ldr	r3, .L732+8
	ldr	r6, [r5, #332]
	ldr	r3, [r3]
	lsls	r2, r3, #23
	mov	r9, r0
	bpl	.L413
	ldr	r3, [sp, #72]
	mov	r2, #1512
	ldr	r1, .L732+32
	ldr	r0, .L732+36
	str	r3, [sp, #8]
	ldr	r3, [sp, #40]
	str	r3, [sp, #4]
	ldr	r3, [sp, #60]
	str	r3, [sp]
	mov	r3, r7
	bl	printk
.L413:
	cmp	r9, #1048576
	ldr	r2, .L732+8
	it	ge
	movge	r9, #327680
	mul	r7, r9, r8
	ldr	r2, [r2]
	asrs	r7, r7, #4
	add	r7, r7, r7, lsl #2
	cmp	r7, #0
	mov	r3, r7
	it	lt
	addlt	r3, r7, #15
	cmp	r7, #0
	asr	r3, r3, #4
	it	lt
	addlt	r7, r7, #15
	rsb	fp, r3, #0
	lsls	r3, r2, #30
	asr	r7, r7, #4
	bpl	.L417
	movw	r2, #1521
	ldr	r1, .L732+32
	ldr	r0, .L732+24
	bl	printk
.L417:
	ldr	r3, [sp, #40]
	ldrb	r2, [r6, #245]	@ zero_extendqisi2
	ldrb	r1, [r6, #330]	@ zero_extendqisi2
	bfi	r2, r3, #0, #6
	strb	r2, [r6, #245]
	ldrb	r2, [r6, #324]	@ zero_extendqisi2
	bic	r2, r2, #7
	orr	r2, r2, #3
	strb	r2, [r6, #324]
	ldr	r2, [r6, #324]
	bfi	r2, r8, #12, #20
	str	r2, [r6, #324]
	ldr	r2, [r10, #44]
	ldr	r3, [sp, #24]
	cmp	r2, #2
	ite	eq
	ldreq	r2, [r3, #308]
	ldrne	r2, [r3, #304]
	and	r2, r2, #15
	bfi	r1, r2, #0, #4
	strb	r1, [r6, #330]
	ldr	r2, [r5, #352]
	cbz	r2, .L420
	uxtb	r2, r1
	bfc	r2, #0, #4
	strb	r2, [r6, #330]
.L420:
	ldr	r3, [sp, #72]
	ldrh	r1, [r6, #330]
	and	r8, r3, #63
	str	r7, [r6, #648]
	bfi	r1, r8, #4, #6
	strh	r1, [r6, #330]	@ movhi
	ldr	r3, [sp, #60]
	ubfx	r1, r1, #8, #8
	lsls	r7, r7, #2
	str	r7, [r6, #652]
	and	r2, r3, #63
	mvn	r3, #-2147483648
	bfi	r1, r2, #2, #6
	strb	r1, [r6, #331]
	ldr	r1, [r6, #332]
	strd	r3, r3, [r6, #656]
	bfi	r1, r9, #0, #20
	str	r1, [r6, #332]
	ldr	r1, [r6, #632]
	strd	r3, r3, [r6, #664]
	str	r3, [r6, #672]
	and	r0, r1, #-33554432
	ldrb	r3, [r6, #680]	@ zero_extendqisi2
	ldr	r1, .L732+40
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #680]
	orrs	r1, r1, r0
	ldrh	r3, [r6, #680]
	str	r1, [r6, #632]
	ldr	r1, [r6, #636]
	bfi	r3, r8, #6, #6
	strh	r3, [r6, #680]	@ movhi
	ldr	r3, [r6, #680]
	lsrs	r1, r1, #20
	bfi	r3, r2, #12, #6
	str	r3, [r6, #680]
	lsls	r1, r1, #20
	str	r1, [r6, #636]
	lsl	r1, fp, #2
	strd	r1, fp, [r6, #640]
	ubfx	r1, r3, #16, #8
	ubfx	r3, r3, #24, #8
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #683]
	ldrb	r3, [r6, #684]	@ zero_extendqisi2
	bfi	r1, r8, #2, #6
	strb	r1, [r6, #682]
	bfi	r3, r8, #0, #6
	strb	r3, [r6, #684]
	ldrh	r3, [r6, #684]
	bfi	r3, r2, #6, #6
	strh	r3, [r6, #684]	@ movhi
	ldr	r3, [r6, #684]
	bfi	r3, r8, #12, #6
	str	r3, [r6, #684]
	ubfx	r1, r3, #16, #8
	ubfx	r3, r3, #24, #8
	bfi	r3, r8, #0, #6
	strb	r3, [r6, #687]
	ldrb	r3, [r6, #688]	@ zero_extendqisi2
	bfi	r1, r2, #2, #6
	strb	r1, [r6, #686]
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #688]
	ldrh	r3, [r6, #688]
	bfi	r3, r8, #6, #6
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
	bfi	r1, r8, #2, #6
	strb	r1, [r6, #690]
	bfi	r3, r8, #0, #6
	strb	r3, [r6, #692]
	ldrb	r3, [r6, #695]	@ zero_extendqisi2
	bfi	r3, r2, #6, #2
	strb	r3, [r6, #695]
	ldr	r3, .L732+8
	ldr	r3, [r3]
	lsls	r6, r3, #30
	bpl	.L421
	movw	r2, #1580
	ldr	r1, .L732+32
	ldr	r0, .L732+16
	bl	printk
.L421:
	ldr	r3, [r5, #4]
	ldr	r6, [r5, #332]
	str	r3, [sp, #72]
	ldr	r3, [sp, #28]
	ldr	fp, [r3, #52]
	ldr	r8, [r3, #28]
	mov	r0, fp
	bl	mpp_frame_get_buffer
	ldr	r3, [sp, #28]
	str	r0, [sp, #88]
	mov	r0, fp
	ldr	r7, [r3, #32]
	bl	mpp_frame_get_fmt
	str	r0, [sp, #100]
	mov	r0, fp
	bl	mpp_frame_get_hor_stride
	mov	r9, r0
	mov	r0, fp
	bl	mpp_frame_get_ver_stride
	str	r0, [sp, #40]
	mov	r0, r8
	bl	mpp_packet_get_length
	ldr	r3, [r7, #24]
	str	r3, [sp, #92]
	ldr	r3, [sp, #28]
	str	r0, [sp, #60]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_is_full
	ldr	r2, .L732+8
	ldr	r2, [r2]
	tst	r2, #2
	str	r0, [sp, #96]
	ldr	r3, [sp, #100]
	beq	.L422
	mov	r2, #1600
	ldr	r1, .L732+44
	ldr	r0, .L732+24
	bl	printk
	ldr	r3, [sp, #100]
	b	.L733
.L734:
	.align	2
.L732:
	.word	269752603
	.word	134746128
	.word	hal_h264e_debug
	.word	.LANCHOR0+508
	.word	.LC27
	.word	.LANCHOR0+531
	.word	.LC26
	.word	vepu540c_h264_customer_scl_tab
	.word	.LANCHOR0+554
	.word	.LC30
	.word	2130942
	.word	.LANCHOR0+577
.L733:
.L422:
	ands	r8, r3, #15728640
	beq	.L423
	mov	r0, fp
	bl	mpp_frame_get_fbc_offset
	mov	r8, r0
.L715:
	mov	fp, #0
.L424:
	ldr	r3, [r5, #340]
	ldr	r2, [sp, #96]
	orrs	r3, r2, r3
	beq	.L433
	movs	r3, #0
	strd	r3, r3, [r6, #116]
	str	r3, [r6, #124]
.L434:
	ldr	r3, [r7, #28]
	ldr	r0, [r7]
	cmp	r3, #0
	bne	.L435
	cmp	r0, #0
	beq	.L436
	mov	r1, r0
	movs	r2, #173
	ldr	r0, [sp, #72]
	bl	mpp_dev_get_iova_address
	str	r0, [r6, #168]
.L437:
	ldr	r2, [r7, #12]
	ldr	r3, [r6, #168]
	add	r3, r3, r2
	ldr	r2, [sp, #92]
	str	r3, [r6, #168]
	subs	r2, r2, #1
	str	r3, [r6, #176]
	add	r2, r2, r3
	str	r2, [r6, #164]
	ldr	r2, [sp, #60]
	add	r3, r3, r2
	str	r3, [r6, #172]
.L438:
	ldr	r3, [sp, #60]
	cbz	r3, .L439
	ldr	r3, [sp, #28]
	ldr	r3, [r3, #32]
	ldr	r0, [r3]
	cmp	r0, #0
	beq	.L440
	ldr	r1, .L735
	bl	mpp_buffer_get_dma_with_caller
	movs	r1, #1
	ldr	r3, [sp, #28]
	ldr	r2, [r3, #32]
	ldr	r3, [sp, #60]
	ldr	r2, [r2, #12]
	bl	dma_buf_end_cpu_access_partial
.L439:
	ldr	r3, .L735+4
	ldr	r3, [r3]
	lsls	r0, r3, #30
	bpl	.L441
	mov	r2, #1696
	ldr	r1, .L735+8
	ldr	r0, .L735+12
	bl	printk
.L441:
	ldr	r3, [r5, #340]
	ldr	r2, [sp, #84]
	orrs	r3, r2, r3
	beq	.L442
	ldr	r3, [sp, #28]
	ldr	r6, [r3, #52]
	mov	r0, r6
	bl	mpp_frame_get_is_full
	cmp	r0, #0
	bne	.L443
	add	r2, sp, #108
	movs	r1, #19
	bl	rk_dvbm_ctrl
	ldr	r3, [sp, #140]
	cmp	r3, #0
	beq	.L444
	ldrd	r2, r3, [sp, #132]
	movw	r1, #2254
	ldr	r0, .L735+16
	bl	printk
	mov	r3, #-1
	str	r3, [sp, #76]
.L326:
	ldr	r0, [sp, #76]
	add	sp, sp, #148
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L354:
	ldr	r3, [sp, #116]
	b	.L710
.L351:
	ldr	r3, [r4, #440]
	bic	r3, r3, #7
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	str	r3, [r4, #440]
	b	.L355
.L356:
	ldr	r0, [r10, #92]
	bl	h264e_marking_is_empty
	mov	r7, r0
	cmp	r0, #0
	bne	.L357
	ldrb	r3, [r4, #444]	@ zero_extendqisi2
	add	r1, sp, #108
	orr	r3, r3, #4
	strb	r3, [r4, #444]
	ldr	r0, [r10, #92]
	bl	h264e_marking_rd_op
	ldr	r3, [sp, #108]
	subs	r2, r3, #1
	cmp	r2, #5
	bhi	.L358
	tbb	[pc, r2]
.L360:
	.byte	(.L365-.L360)/2
	.byte	(.L364-.L360)/2
	.byte	(.L363-.L360)/2
	.byte	(.L362-.L360)/2
	.byte	(.L545-.L360)/2
	.byte	(.L359-.L360)/2
	.p2align 1
.L365:
	ldr	r2, [sp, #112]
.L361:
	ldrb	r1, [r4, #444]	@ zero_extendqisi2
	bfi	r1, r3, #4, #3
	strb	r1, [r4, #444]
	ldr	r3, [r4, #444]
	bfi	r3, r2, #7, #16
	str	r3, [r4, #444]
	ldrb	r3, [r4, #452]	@ zero_extendqisi2
	bfi	r3, r7, #0, #4
	strb	r3, [r4, #452]
	ldr	r0, [r10, #92]
	bl	h264e_marking_is_empty
	mov	r7, r0
	cmp	r0, #0
	bne	.L357
	add	r1, sp, #108
	ldr	r0, [r10, #92]
	bl	h264e_marking_rd_op
	ldr	r3, [sp, #108]
	subs	r2, r3, #1
	cmp	r2, #5
	bhi	.L368
	tbb	[pc, r2]
.L370:
	.byte	(.L375-.L370)/2
	.byte	(.L374-.L370)/2
	.byte	(.L373-.L370)/2
	.byte	(.L372-.L370)/2
	.byte	(.L546-.L370)/2
	.byte	(.L369-.L370)/2
	.p2align 1
.L364:
	ldr	r2, [sp, #116]
	b	.L361
.L363:
	ldr	r2, [sp, #112]
	ldr	r7, [sp, #120]
	b	.L361
.L362:
	ldr	r2, [sp, #124]
	b	.L361
.L359:
	ldr	r2, [sp, #120]
	b	.L361
.L358:
	movw	r2, #1081
	ldr	r1, .L735+20
	ldr	r0, .L735+24
	bl	printk
	movs	r3, #0
.L545:
	movs	r2, #0
	b	.L361
.L375:
	ldr	r2, [sp, #112]
.L371:
	ldrh	r1, [r4, #446]
	strh	r2, [r4, #448]	@ movhi
	bfi	r1, r3, #7, #3
	ldrb	r3, [r4, #452]	@ zero_extendqisi2
	strh	r1, [r4, #446]	@ movhi
	bfi	r3, r7, #4, #4
	strb	r3, [r4, #452]
	ldr	r0, [r10, #92]
	bl	h264e_marking_is_empty
	mov	r7, r0
	cmp	r0, #0
	bne	.L357
	add	r1, sp, #108
	ldr	r0, [r10, #92]
	bl	h264e_marking_rd_op
	ldr	r3, [sp, #108]
	subs	r2, r3, #1
	cmp	r2, #5
	bhi	.L377
	tbb	[pc, r2]
.L379:
	.byte	(.L384-.L379)/2
	.byte	(.L383-.L379)/2
	.byte	(.L382-.L379)/2
	.byte	(.L381-.L379)/2
	.byte	(.L547-.L379)/2
	.byte	(.L378-.L379)/2
	.p2align 1
.L374:
	ldr	r2, [sp, #116]
	b	.L371
.L373:
	ldr	r2, [sp, #112]
	ldr	r7, [sp, #120]
	b	.L371
.L372:
	ldr	r2, [sp, #124]
	b	.L371
.L369:
	ldr	r2, [sp, #120]
	b	.L371
.L368:
	mov	r2, #1136
	ldr	r1, .L735+20
	ldr	r0, .L735+24
	bl	printk
	movs	r3, #0
.L546:
	movs	r2, #0
	b	.L371
.L384:
	ldr	r2, [sp, #112]
.L380:
	ldrb	r1, [r4, #447]	@ zero_extendqisi2
	strh	r2, [r4, #450]	@ movhi
	bfi	r1, r3, #2, #3
	ldrb	r3, [r4, #453]	@ zero_extendqisi2
	strb	r1, [r4, #447]
	bfi	r3, r7, #0, #4
	strb	r3, [r4, #453]
	b	.L357
.L383:
	ldr	r2, [sp, #116]
	b	.L380
.L382:
	ldr	r2, [sp, #112]
	ldr	r7, [sp, #120]
	b	.L380
.L381:
	ldr	r2, [sp, #124]
	b	.L380
.L378:
	ldr	r2, [sp, #120]
	b	.L380
.L377:
	movw	r2, #1190
	ldr	r1, .L735+20
	ldr	r0, .L735+24
	bl	printk
	movs	r3, #0
.L547:
	movs	r2, #0
	b	.L380
.L387:
	ldr	r2, [r5]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	cmp	r3, #1
	ldrb	r3, [r7, #756]	@ zero_extendqisi2
	ite	eq
	moveq	r2, #9
	movne	r2, #6
	b	.L711
.L549:
	movs	r3, #0
	b	.L390
.L394:
	orr	r3, r3, #16
	strh	r3, [r6, #1644]	@ movhi
	b	.L395
.L396:
	cmp	r2, r8, lsr #7
	bcs	.L398
	subs	r3, r3, #6
	b	.L397
.L398:
	cmp	r2, r8, lsr #6
	ite	cc
	subcc	r3, r3, #4
	addcs	r3, r3, #-1
	b	.L397
.L400:
	ldr	r2, [r9, #48]
	cmp	r2, #2
	beq	.L724
	movs	r2, #2
	b	.L712
.L403:
	ldr	r3, [r7, #8]
	lsls	r3, r3, #31
	bpl	.L404
	orr	r2, r2, #640
	strh	r2, [r6, #1624]	@ movhi
	ldr	r3, [r7, #8]
	ldrb	r2, [r6, #1624]	@ zero_extendqisi2
	asrs	r3, r3, #1
	bfi	r2, r3, #3, #3
	strb	r2, [r6, #1624]
	ldr	r3, [r7, #8]
	ldrh	r2, [r6, #1630]
	asrs	r3, r3, #4
	bfi	r2, r3, #4, #9
	ldrh	r3, [r6, #1634]
	strh	r2, [r6, #1630]	@ movhi
	ldrsh	r2, [r7, #10]
	bfi	r3, r2, #4, #8
	strh	r3, [r6, #1634]	@ movhi
	b	.L404
.L409:
	cmp	r3, #1
	ite	eq
	ldreq	r1, .L735+28
	ldrne	r1, .L735+32
	b	.L713
.L423:
	ands	fp, r3, #983040
	bne	.L715
	ubfx	r2, r3, #0, #20
	cmp	r2, #17
	bhi	.L552
	mov	r1, r3
	add	r0, sp, #108
	bl	vepu541_set_fmt
	ldr	r2, [sp, #108]
	subs	r2, r2, #4
	cmp	r2, #9
	bhi	.L553
	tbb	[pc, r2]
.L427:
	.byte	(.L428-.L427)/2
	.byte	(.L431-.L427)/2
	.byte	(.L428-.L427)/2
	.byte	(.L429-.L427)/2
	.byte	(.L553-.L427)/2
	.byte	(.L553-.L427)/2
	.byte	(.L553-.L427)/2
	.byte	(.L553-.L427)/2
	.byte	(.L428-.L427)/2
	.byte	(.L426-.L427)/2
	.p2align 1
.L736:
	.align	2
.L735:
	.word	.LANCHOR0+599
	.word	hal_h264e_debug
	.word	.LANCHOR0+577
	.word	.LC27
	.word	.LC31
	.word	.LANCHOR0+463
	.word	.LC29
	.word	vepu540c_h264_jvt_scl_tab
	.word	vepu540c_h264_flat_scl_tab
.L431:
	ldr	r3, [sp, #40]
	mul	fp, r3, r9
	add	r3, fp, fp, lsl #1
	add	r3, r3, r3, lsr #31
	asrs	r3, r3, #1
.L425:
	mov	r8, fp
	mov	fp, r3
	b	.L424
.L429:
	ldr	r3, [sp, #40]
	mul	fp, r3, r9
	adds	r3, fp, fp, lsl #2
	it	mi
	addmi	r3, r3, #3
	asrs	r3, r3, #2
	b	.L425
.L428:
	ldr	r3, [sp, #40]
	mul	fp, r3, r9
	mov	r3, fp
	b	.L425
.L426:
	ldr	r3, [sp, #40]
	mul	fp, r3, r9
	lsl	r3, fp, #1
	b	.L425
.L553:
	movs	r3, #0
	b	.L425
.L552:
	mov	r8, fp
	b	.L424
.L433:
	movs	r2, #160
	ldr	r1, [sp, #88]
	ldr	r0, [sp, #72]
	bl	mpp_dev_get_iova_address
	add	r3, r0, r8
	str	r0, [r6, #116]
	str	r3, [r6, #120]
	add	r0, r0, fp
	str	r0, [r6, #124]
	b	.L434
.L436:
	ldr	r3, [r7, #4]
	str	r3, [r6, #168]
	b	.L437
.L435:
	ldr	r1, .L737
	bl	mpp_buffer_get_size_with_caller
	movs	r2, #173
	ldr	r1, [r7]
	mov	r8, r0
	ldr	r0, [sp, #72]
	bl	mpp_dev_get_iova_address
	mov	r1, r8
	str	r0, [r6, #168]
	mov	r9, r0
	ldr	r3, [sp, #60]
	ldr	r0, [r7, #12]
	add	r0, r0, r3
	bl	__aeabi_uidivmod
	add	r0, r8, r9
	add	r1, r1, r9
	str	r1, [r6, #172]
	ldr	r3, [r7, #16]
	str	r0, [r6, #164]
	add	r3, r3, r9
	str	r3, [r6, #176]
	b	.L438
.L440:
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L439
	ldr	r0, [r5, #4]
	bl	mpp_get_dev
	ldr	r3, [sp, #28]
	ldr	r2, [sp, #60]
	ldr	r1, [r3, #32]
	movs	r3, #1
	ldr	r1, [r1, #4]
	bl	dma_sync_single_for_device
	b	.L439
.L738:
	.align	2
.L737:
	.word	.LANCHOR0+599
.L444:
	ldrb	r3, [r4, #96]	@ zero_extendqisi2
	and	r3, r3, #8
	orn	r3, r3, #120
	strb	r3, [r4, #96]
	ldr	r2, [sp, #136]
	ldrh	r3, [r4, #24]
	bfi	r3, r2, #0, #14
	strh	r3, [r4, #24]	@ movhi
	ldrb	r2, [sp, #132]	@ zero_extendqisi2
	ldr	r3, [r4, #24]
	bfi	r3, r2, #14, #8
	str	r3, [r4, #24]
	strb	r2, [r4, #252]
	lsrs	r3, r3, #24
	bic	r3, r3, #7
	orr	r3, r3, #7
	strb	r3, [r4, #27]
	ldrb	r3, [r4, #254]	@ zero_extendqisi2
	bic	r3, r3, #7
	orr	r3, r3, #5
	strb	r3, [r4, #254]
	ldr	r3, [sp, #108]
	str	r3, [r4, #100]
	ldr	r3, [sp, #120]
	str	r3, [r4, #104]
	ldr	r3, [sp, #112]
	str	r3, [r4, #108]
	ldr	r3, [sp, #124]
	str	r3, [r4, #112]
	ldr	r3, [sp, #116]
	str	r3, [r4, #116]
	ldr	r3, [sp, #128]
	strd	r3, r3, [r4, #120]
.L442:
	ldr	r8, [r5, #172]
	ldrd	r6, fp, [r5, #12]
	ldr	r3, [r5, #4]
	ldr	r1, [r8, #4]
	mov	r0, r6
	ldr	r7, [r5, #44]
	str	r3, [sp, #40]
	bl	hal_bufs_get_buf
	ldr	r1, [r8, #20]
	mov	r9, r0
	mov	r0, r6
	bl	hal_bufs_get_buf
	ldr	r3, [sp, #28]
	mov	r8, r0
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_ppinfo
	ldr	r3, .L739
	ldr	r3, [r3]
	lsls	r1, r3, #30
	bpl	.L447
	mov	r2, #1712
	ldr	r1, .L739+4
	ldr	r0, .L739+8
	bl	printk
.L447:
	cmp	r9, #0
	beq	.L449
	ldr	r3, [r9]
	cmp	r3, #0
	beq	.L449
	ldr	r3, [r9, #4]
	ldr	r6, [r3]
	cbnz	r6, .L451
	movw	r1, #1716
	ldr	r3, .L739+12
	str	r1, [sp]
	ldr	r2, .L739+16
	ldr	r0, .L739+20
	bl	printk
.L451:
	movs	r2, #169
	mov	r1, r6
	ldr	r0, [sp, #40]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #152]
	cbnz	r7, .L449
	ldr	r3, [r9, #4]
	ldr	r3, [r3, #12]
	str	r3, [sp, #60]
	cbnz	r3, .L452
	movw	r1, #1721
	ldr	r3, .L739+12
	str	r1, [sp]
	ldr	r2, .L739+24
	ldr	r0, .L739+20
	bl	printk
.L452:
	movs	r2, #163
	ldr	r1, [sp, #60]
	ldr	r0, [sp, #40]
	bl	mpp_dev_get_iova_address
	movs	r2, #169
	mov	r1, r6
	str	r0, [r4, #128]
	add	r0, r0, fp
	str	r0, [r4, #132]
	ldr	r0, [sp, #40]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #152]
.L449:
	cmp	r8, #0
	beq	.L454
	ldr	r3, [r8]
	cbz	r3, .L454
	ldr	r3, [r8, #4]
	ldr	r6, [r3]
	cbnz	r6, .L456
	movw	r1, #1734
	ldr	r3, .L739+12
	str	r1, [sp]
	ldr	r2, .L739+16
	ldr	r0, .L739+20
	bl	printk
.L456:
	movs	r2, #170
	mov	r1, r6
	ldr	r0, [sp, #40]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #156]
	cbnz	r7, .L454
	ldr	r3, [r8, #4]
	ldr	r6, [r3, #12]
	cbnz	r6, .L457
	movw	r1, #1739
	ldr	r3, .L739+12
	str	r1, [sp]
	ldr	r2, .L739+24
	ldr	r0, .L739+20
	bl	printk
.L457:
	movs	r2, #165
	mov	r1, r6
	ldr	r0, [sp, #40]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #136]
	add	r0, r0, fp
	str	r0, [r4, #140]
.L454:
	ldr	r3, [r5, #44]
	cmp	r3, #0
	beq	.L458
	ldr	r7, [r5, #172]
	movs	r2, #163
	ldr	r1, [r5, #48]
	ldr	r0, [r5, #4]
	ldr	r3, [r7, #24]
	ldr	fp, [r7, #8]
	str	r3, [sp, #72]
	ldr	r3, [r7, #12]
	str	r3, [sp, #60]
	bl	mpp_dev_get_iova_address
	ldr	r3, [r7, #16]
	mov	r6, r0
	cmp	r3, #0
	beq	.L459
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #20]
	cmp	r2, r3
	bne	.L459
	ldr	r3, .L739
	ldr	r3, [r3]
	lsls	r2, r3, #24
	bpl	.L460
	mov	r3, fp
	movw	r2, #391
	ldr	r1, .L739+28
	ldr	r0, .L739+32
	bl	printk
.L460:
	ldr	r3, [r5, #76]
	str	r3, [sp, #48]
	ldr	r3, [r5, #80]
	str	r3, [sp, #44]
	ldr	r3, [r5, #124]
	str	r3, [sp, #56]
	ldr	r3, [r5, #128]
	str	r3, [sp, #52]
	cmp	fp, #0
	beq	.L461
	ldr	r3, [r5, #60]
	ldr	r2, [sp, #48]
	str	r3, [sp, #32]
	ldr	r3, [r5, #108]
	str	r3, [sp, #36]
	ldr	r3, [r5, #52]
	cmp	r2, r3
	it	cs
	movcs	r2, r3
	ldr	r3, [r5, #56]
	str	r2, [sp, #48]
	ldr	r2, [sp, #44]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [r5, #100]
	str	r2, [sp, #44]
	ldr	r2, [sp, #56]
	cmp	r2, r3
	it	cs
	movcs	r2, r3
	ldr	r3, [r5, #104]
	str	r2, [sp, #56]
	ldr	r2, [sp, #52]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [sp, #36]
	str	r2, [sp, #52]
.L716:
	str	r3, [sp, #68]
	ldr	r3, [sp, #32]
	str	r3, [sp, #64]
.L462:
	ldr	r3, [sp, #32]
	ldrb	r2, [r4, #244]	@ zero_extendqisi2
	add	r3, r3, r6
	str	r3, [r4, #128]
	ldr	r3, [sp, #36]
	add	r3, r3, r6
	str	r3, [r4, #132]
	ldr	r3, [sp, #64]
	add	r3, r3, r6
	str	r3, [r4, #136]
	ldr	r3, [sp, #68]
	add	r3, r3, r6
	str	r3, [r4, #140]
	ldr	r3, [sp, #44]
	add	r3, r3, r6
	str	r3, [r4, #196]
	ldr	r3, [sp, #48]
	add	r3, r3, r6
	str	r3, [r4, #200]
	ldr	r3, [sp, #52]
	add	r3, r3, r6
	str	r3, [r4, #204]
	ldr	r3, [sp, #56]
	add	r6, r6, r3
	ldr	r3, [sp, #60]
	str	r6, [r4, #208]
	clz	r3, r3
	ldr	r6, .L739
	lsrs	r3, r3, #5
	bfi	r2, r3, #2, #1
	strb	r2, [r4, #244]
	ldr	r2, [r6]
	lsls	r7, r2, #24
	bpl	.L539
	movw	r2, #501
	ldr	r1, .L739+28
	ldr	r0, .L739+36
	bl	printk
	ldr	r3, [r6]
	lsls	r0, r3, #24
	bpl	.L539
	ldr	r3, [r5, #88]
	movw	r2, #503
	ldr	r1, .L739+28
	ldr	r0, .L739+40
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
	bpl	.L539
	ldr	r3, [r5, #136]
	movw	r2, #505
	ldr	r1, .L739+28
	ldr	r0, .L739+44
	str	r3, [sp, #12]
	ldr	r3, [r5, #132]
	str	r3, [sp, #8]
	ldr	r3, [r5, #124]
	str	r3, [sp, #4]
	ldr	r3, [r5, #128]
	str	r3, [sp]
	ldr	r3, [r5, #140]
	bl	printk
.L539:
	ldr	r3, [sp, #60]
	cbnz	r3, .L478
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
.L478:
	ldr	r3, [r9, #4]
	movs	r2, #185
	ldr	r0, [sp, #40]
	ldr	r6, .L739
	ldr	r1, [r3, #8]
	bl	mpp_dev_get_iova_address
	movs	r2, #184
	str	r0, [r4, #216]
	ldr	r3, [r8, #4]
	ldr	r0, [sp, #40]
	ldr	r1, [r3, #8]
	bl	mpp_dev_get_iova_address
	ldr	r3, [r6]
	lsls	r2, r3, #30
	str	r0, [r4, #212]
	bpl	.L481
	b	.L740
.L741:
	.align	2
.L739:
	.word	hal_h264e_debug
	.word	.LANCHOR0+621
	.word	.LC26
	.word	.LANCHOR0+646
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LANCHOR0+671
	.word	.LC36
	.word	.LC40
	.word	.LC41
	.word	.LC42
.L740:
	movw	r2, #1761
	ldr	r1, .L742
	ldr	r0, .L742+4
	bl	printk
.L481:
	ldr	r3, [sp, #28]
	ldr	r1, [r3, #64]
	cmp	r1, #0
	beq	.L554
	movs	r2, #171
	ldr	r0, [r5, #4]
	bl	mpp_dev_get_iova_address
.L482:
	ldr	r3, [sp, #28]
	str	r0, [r4, #160]
	ldrb	r2, [r4, #244]	@ zero_extendqisi2
	ldr	r3, [r3, #64]
	adds	r3, r3, #0
	it	ne
	movne	r3, #1
	bfi	r2, r3, #3, #1
	ldr	r3, [sp, #28]
	strb	r2, [r4, #244]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_offset_y
	ldrh	r3, [r4, #294]
	bfi	r3, r0, #0, #14
	strh	r3, [r4, #294]	@ movhi
	ldr	r3, [sp, #28]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_offset_x
	ldrh	r3, [r4, #292]
	bfi	r3, r0, #0, #14
	strh	r3, [r4, #292]	@ movhi
	ldr	r3, [r6]
	lsls	r3, r3, #30
	bpl	.L483
	movw	r2, #1767
	ldr	r1, .L742+8
	ldr	r0, .L742+12
	bl	printk
.L483:
	ldr	r3, [sp, #24]
	ldr	r3, [r3, #3404]
	cmp	r3, #1
	beq	.L484
	cmp	r3, #0
	beq	.L485
	cmp	r3, #2
	beq	.L486
	movw	r2, #1809
	ldr	r1, .L742+8
	ldr	r0, .L742+16
	bl	printk
	b	.L488
.L443:
	mov	r0, r6
	bl	mpp_frame_get_phy_addr
	mov	r7, r0
	mov	r0, r6
	bl	mpp_frame_get_hor_stride
	mov	r8, r0
	mov	r0, r6
	bl	mpp_frame_get_ver_stride
	mul	r0, r0, r8
	cbz	r7, .L446
	str	r7, [r4, #116]
	add	r7, r7, r0
	strd	r7, r7, [r4, #120]
	b	.L442
.L446:
	movw	r1, #2294
	ldr	r0, .L742+20
	bl	printk
	b	.L442
.L461:
	ldr	r3, [r5, #84]
	str	r3, [sp, #32]
	ldr	r3, [r5, #132]
	str	r3, [sp, #36]
	b	.L716
.L459:
	ldr	r3, [sp, #72]
	ldr	r1, .L742+24
	add	r3, r3, fp, lsl #1
	add	r3, r1, r3, lsl #2
	ldr	r7, [r3, #692]
	ldr	r3, .L742+28
	ldr	r3, [r3]
	lsls	r3, r3, #24
	bpl	.L463
	mov	r3, r7
	mov	r2, #416
	addw	r1, r1, #671
	ldr	r0, .L742+32
	bl	printk
.L463:
	cmp	r7, #3
	bhi	.L462
	tbb	[pc, r7]
.L465:
	.byte	(.L468-.L465)/2
	.byte	(.L467-.L465)/2
	.byte	(.L466-.L465)/2
	.byte	(.L464-.L465)/2
	.p2align 1
.L468:
	ldr	r3, [r5, #88]
	str	r3, [sp, #64]
	ldr	r3, [r5, #136]
	str	r3, [sp, #68]
	ldr	r3, [r5, #84]
	str	r3, [sp, #32]
	ldr	r3, [r5, #132]
	str	r3, [sp, #36]
	ldr	r3, [r5, #76]
	str	r3, [sp, #48]
	ldr	r3, [r5, #80]
	str	r3, [sp, #44]
	ldr	r3, [r5, #124]
	str	r3, [sp, #56]
	ldr	r3, [r5, #128]
.L717:
	str	r3, [sp, #52]
	b	.L462
.L467:
	ldr	r3, [r5, #60]
	str	r3, [sp, #64]
	ldr	r3, [r5, #108]
	str	r3, [sp, #68]
	ldr	r3, [r5, #76]
	str	r3, [sp, #32]
	str	r3, [r5, #84]
	ldr	r3, [r5, #124]
	ldr	r2, [sp, #32]
	str	r3, [sp, #36]
	str	r3, [r5, #132]
	ldr	r3, [r5, #52]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	ldr	r2, [r5, #56]
	str	r3, [sp, #48]
	ldr	r3, [r5, #80]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	ldr	r3, [r5, #100]
	str	r2, [sp, #44]
	ldr	r2, [sp, #36]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	ldr	r2, [r5, #104]
	str	r3, [sp, #56]
	ldr	r3, [r5, #128]
	cmp	r2, r3
	it	cc
	movcc	r2, r3
	mov	r3, r2
	b	.L717
.L466:
	ldr	r0, .L742+36
	mov	r1, #448
.L718:
	bl	printk
	b	.L462
.L464:
	ldr	r3, [r5, #364]
	cmp	r3, #0
	bne	.L469
	add	r1, r5, #76
	str	r1, [sp, #72]
	ldr	r1, [r5, #60]
	add	r3, r5, #100
	ldr	lr, [sp, #72]
	add	r2, r5, #52
	add	r7, r5, #124
	add	fp, sp, #108
	str	r1, [sp, #64]
	ldr	r1, [r5, #108]
	str	r1, [sp, #68]
	ldr	r1, [r5, #76]
	str	r1, [sp, #32]
	str	r1, [r5, #84]
	ldr	r1, [r5, #124]
	ldr	r0, [sp, #32]
	str	r1, [sp, #36]
	str	r1, [r5, #132]
	ldr	r1, [r5, #52]
	cmp	r1, r0
	it	cs
	movcs	r1, r0
	ldr	r0, [r5, #56]
	str	r1, [sp, #48]
	ldr	r1, [r5, #80]
	cmp	r0, r1
	it	cc
	movcc	r0, r1
	ldr	r1, [r5, #100]
	str	r0, [sp, #44]
	ldr	r0, [sp, #36]
	cmp	r1, r0
	it	cs
	movcs	r1, r0
	ldr	r0, [r5, #104]
	str	r1, [sp, #56]
	ldr	r1, [r5, #128]
	cmp	r0, r1
	it	cc
	movcc	r0, r1
	str	r0, [sp, #52]
.L470:
	ldr	r0, [lr]	@ unaligned
	add	lr, lr, #8
	ldr	r1, [lr, #-4]	@ unaligned
	cmp	lr, r3
	mov	ip, fp
	stmia	ip!, {r0, r1}
	mov	fp, ip
	bne	.L470
	ldr	ip, [sp, #72]
	add	r0, r2, #24
	mov	r1, r2
.L471:
	ldr	lr, [r1], #4	@ unaligned
	cmp	r1, r0
	str	lr, [ip], #4	@ unaligned
	bne	.L471
	add	lr, sp, #108
	add	fp, sp, #132
.L472:
	mov	ip, lr
	adds	r2, r2, #8
	ldmia	ip!, {r0, r1}
	cmp	ip, fp
	str	r0, [r2, #-8]	@ unaligned
	mov	lr, ip
	str	r1, [r2, #-4]	@ unaligned
	bne	.L472
	add	lr, sp, #108
	add	fp, r7, #24
	mov	r2, r7
.L473:
	ldr	r0, [r2]	@ unaligned
	adds	r2, r2, #8
	ldr	r1, [r2, #-4]	@ unaligned
	cmp	r2, fp
	mov	ip, lr
	stmia	ip!, {r0, r1}
	mov	lr, ip
	bne	.L473
	add	r1, r3, #24
	mov	r2, r3
.L474:
	ldr	r0, [r2], #4	@ unaligned
	cmp	r2, r1
	str	r0, [r7], #4	@ unaligned
	bne	.L474
	add	r7, sp, #108
	add	ip, sp, #132
.L475:
	mov	r2, r7
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, ip
	str	r0, [r3, #-8]	@ unaligned
	mov	r7, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L475
	b	.L462
.L469:
	mov	r1, #476
	ldr	r0, .L742+40
	b	.L718
.L458:
	mov	r2, #-1
	strd	r2, r3, [r4, #196]
	strd	r2, r3, [r4, #204]
	b	.L478
.L554:
	mov	r0, r1
	b	.L482
.L743:
	.align	2
.L742:
	.word	.LANCHOR0+621
	.word	.LC27
	.word	.LANCHOR0+708
	.word	.LC26
	.word	.LC43
	.word	.LC32
	.word	.LANCHOR0
	.word	hal_h264e_debug
	.word	.LC37
	.word	.LC38
	.word	.LC39
.L485:
	ldr	r1, [r4, #340]
	ldr	r2, .L744
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
.L488:
	ldr	r2, [sp, #24]
	movs	r3, #0
	str	r3, [r2, #3400]
	ldr	r3, [r6]
	lsls	r7, r3, #30
	bpl	.L489
	mov	r2, #1816
	ldr	r1, .L744+4
	ldr	r0, .L744+8
	bl	printk
.L489:
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
	movw	r9, #1795
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
.L490:
	mla	r7, r8, r1, r0
	sub	ip, r2, #2
	cmp	r7, r9
	bhi	.L492
	cmp	r2, #32
	bne	.L493
.L494:
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
	bpl	.L495
	movw	r2, #1940
	ldr	r1, .L744+12
	ldr	r0, .L744+8
	bl	printk
.L495:
	ldr	r1, [r5, #336]
	cmp	r1, #0
	beq	.L496
	movs	r2, #179
	ldr	r0, [r5, #4]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #192]
	ldr	r3, [r5, #148]
	add	r3, r3, r0
	str	r3, [r4, #188]
.L497:
	ldr	r3, [r5, #328]
	cbz	r3, .L498
	add	r0, r5, #320
	bl	vepu540c_set_osd
.L498:
	ldr	r3, [r5, #348]
	cbz	r3, .L500
	ldr	r2, [r5]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	cmp	r3, #1
	bne	.L500
	ldr	r3, [sp, #28]
	ldr	r9, [r10, #96]
	add	fp, r3, #72
	ldrb	r6, [r4, #245]	@ zero_extendqisi2
	ldrd	r1, r2, [r3, #64]
	ldr	r7, [r3, #84]
	and	r6, r6, #63
	ldr	r3, [sp, #24]
	ldr	r0, [r5, #332]
	ldr	r8, [r5, #352]
	ldrd	ip, lr, [r3, #12]
	add	r0, r0, #760
	str	r9, [sp, #20]
	cmp	r8, #0
	beq	.L502
	mov	r8, #0
	strd	r6, ip, [sp, #4]
	strd	lr, r8, [sp, #12]
	mov	r3, fp
	str	r7, [sp]
	bl	vepu540c_set_qpmap_smart
.L503:
	ldr	r3, [sp, #28]
	movs	r2, #186
	ldr	r0, [r5, #4]
	ldr	r1, [r3, #68]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #220]
.L500:
	ldr	r1, [r5, #316]
	cbz	r1, .L504
	ldr	r3, [sp, #24]
	ldr	r2, [sp, #24]
	ldr	r0, [r5, #332]
	ldr	r3, [r3, #16]
	ldr	r2, [r2, #12]
	add	r0, r0, #760
	bl	vepu540c_set_roi
.L504:
	ldr	r3, [sp, #28]
	ldr	r0, [r3, #52]
	bl	mpp_frame_get_is_gray
	ldr	r3, [r5, #356]
	cmp	r3, r0
	beq	.L505
	cbnz	r3, .L507
	ldrb	r2, [r4, #1645]	@ zero_extendqisi2
	str	r3, [r4, #1796]
	bfi	r2, r3, #7, #1
	ldrb	r3, [r4, #245]	@ zero_extendqisi2
	strb	r2, [r4, #1645]
	and	r2, r3, #63
	cmp	r2, #29
	bhi	.L508
	movs	r2, #30
.L720:
	bfi	r3, r2, #0, #6
	strb	r3, [r4, #245]
.L507:
	str	r0, [r5, #356]
.L505:
	ldr	r7, .L744+16
	ldr	r6, [r5, #332]
	ldr	r3, [r7]
	lsls	r1, r3, #30
	bpl	.L511
	movw	r2, #1986
	ldr	r1, .L744+20
	ldr	r0, .L744+24
	bl	printk
.L511:
	ldr	r3, [r6, #952]
	add	r0, r6, #1416
	ldr	r2, .L744+28
	lsrs	r3, r3, #20
	lsls	r3, r3, #20
	orrs	r2, r2, r3
	str	r2, [r6, #952]
	ldr	r2, [r5]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	cmp	r3, #1
	bne	.L512
	ldr	r3, [r5, #352]
	cmp	r3, #0
	beq	.L513
	ldr	r3, [r5, #360]
	ldrh	r2, [r6, #952]
	cmp	r3, #0
	ldr	r1, .L744+32
	movw	r3, #341
	it	eq
	moveq	r3, #500
	bfi	r2, r3, #0, #10
	strh	r2, [r6, #952]	@ movhi
	movs	r2, #208
.L721:
	bl	memcpy
	ldr	r3, [r6, #904]
	ldr	r0, .L744+36
	and	r3, r3, #-268374016
	ldr	r1, .L744+40
	orrs	r3, r3, r0
	str	r3, [r6, #904]
	ldr	r3, [r6, #908]
	ldr	r2, [r6, #920]
	and	r3, r3, #-268374016
	orrs	r3, r3, r1
	str	r3, [r6, #908]
	ldr	r3, [r6, #912]
	and	r2, r2, #-268374016
	orrs	r2, r2, r0
	str	r2, [r6, #920]
	and	r3, r3, #-268374016
	movs	r2, #4
	orrs	r3, r3, r0
	str	r3, [r6, #912]
	ldr	r3, [r6, #916]
	and	r3, r3, #-268374016
	orrs	r3, r3, r1
	str	r3, [r6, #916]
	ldr	r3, [r6, #924]
	and	r3, r3, #-268374016
	orrs	r3, r3, r1
	str	r3, [r6, #924]
	ldrh	r3, [r6, #928]
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #928]	@ movhi
	ldr	r3, .L744+44
	ldrh	r2, [r6, #972]
	str	r3, [r6, #932]
	add	r3, r3, #768
	strd	r3, r3, [r6, #936]
	add	r3, r3, #-50331648
	subw	r3, r3, #1284
	str	r3, [r6, #944]
	ldr	r3, [r10, #44]
	cmp	r3, #2
	ldr	r3, [r6, #968]
	and	r3, r3, #-268374016
	bne	.L516
	orr	r3, r3, #131072
	orr	r3, r3, #1
	str	r3, [r6, #968]
	mov	r3, r2	@ movhi
	movs	r2, #6
.L722:
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #972]	@ movhi
	ldrb	r3, [r6, #974]	@ zero_extendqisi2
	movs	r2, #32
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #974]
	ldr	r3, [r10, #44]
	ldr	r2, [r6, #992]
	cmp	r3, #2
	ldrd	r3, r1, [r6, #984]
	and	r3, r3, #-16777216
	bne	.L518
	orr	r3, r3, #1048592
	and	r1, r1, #-16777216
	orr	r3, r3, #4096
	str	r3, [r6, #984]
	ldr	r3, .L744+48
	and	r2, r2, #-16777216
	orrs	r3, r3, r1
	str	r3, [r6, #988]
	ldr	r3, .L744+52
	orrs	r3, r3, r2
	str	r3, [r6, #992]
.L519:
	ldrh	r3, [r6, #1000]
	ldr	r2, .L744+56
	bic	r3, r3, #32640
	ldr	r1, [sp, #24]
	bic	r3, r3, #119
	orr	r3, r3, #3
	strh	r3, [r6, #1000]	@ movhi
	ldr	r3, [r6, #1004]
	ands	r2, r2, r3
	ldr	r3, .L744+60
	orrs	r3, r3, r2
	str	r3, [r6, #1004]
	ldrh	r3, [r6, #1008]
	movs	r2, #16
	bfi	r3, r2, #0, #12
	strh	r3, [r6, #1008]	@ movhi
	ldr	r3, .L744+64
	ldr	r2, [r6, #1016]
	b	.L745
.L746:
	.align	2
.L744:
	.word	-2147450888
	.word	.LANCHOR0+708
	.word	.LC27
	.word	.LANCHOR0+729
	.word	hal_h264e_debug
	.word	.LANCHOR0+747
	.word	.LC26
	.word	349867
	.word	.LANCHOR1+604
	.word	196609
	.word	524294
	.word	370415382
	.word	1316118
	.word	1053204
	.word	-536346112
	.word	83935248
	.word	336595976
.L745:
	str	r3, [r6, #1012]
	ldr	r3, .L747
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
	ldr	r3, .L747+4
	orrs	r3, r3, r2
	str	r3, [r6, #1024]
	ldr	r3, [r6, #1028]
	and	r2, r3, #-16777216
	ldr	r3, .L747+8
	orrs	r3, r3, r2
	str	r3, [r6, #1028]
	ldr	r3, [r6, #732]
	and	r2, r3, #-16777216
	ldr	r3, .L747+12
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
	ldr	r3, [r10, #44]
	ldr	r2, [sp, #24]
	cmp	r3, #2
	addw	r3, r6, #699
	add	r2, r2, #376
	bne	.L520
	add	r1, r1, #312
.L521:
	ldr	r0, [r1], #4
	cmp	r2, r1
	strb	r0, [r3, #1]!
	ldr	r0, [r1, #124]
	and	r0, r0, #63
	strb	r0, [r3, #16]
	bne	.L521
.L525:
	ldr	r2, [r5]
	movw	r3, #4708
	ldr	r3, [r2, r3]
	cmp	r3, #1
	bne	.L522
.L523:
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L528
	movw	r2, #2200
	ldr	r1, .L747+16
	ldr	r0, .L747+20
	bl	printk
.L528:
	ldr	r1, [r5, #336]
	cmp	r1, #0
	beq	.L529
	movs	r2, #179
	ldr	r0, [r5, #4]
	bl	mpp_dev_get_iova_address
	str	r0, [r4, #192]
	ldr	r3, [r5, #148]
	add	r3, r3, r0
	str	r3, [r4, #188]
.L530:
	ldr	r3, [sp, #80]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsls	r3, r3, #29
	bpl	.L531
	ldr	r0, [r5, #152]
	cmp	r0, #0
	bne	.L532
	mov	r1, r5
	ldr	r6, [r1], #152
	ldr	r2, [r6, #12]
	adds	r2, r2, #15
	bic	r3, r2, #15
	ldr	r2, [r6, #16]
	adds	r2, r2, #15
	bic	r2, r2, #15
	muls	r2, r3, r2
	ldr	r3, .L747+24
	str	r3, [sp]
	ldr	r3, .L747+28
	add	r2, r2, r2, lsl #1
	asrs	r2, r2, #1
	bl	mpp_buffer_get_with_tag
	ldr	r3, [r5, #152]
	cmp	r3, #0
	bne	.L532
	movw	r1, #1429
	ldr	r0, .L747+32
	bl	printk
.L531:
	ldr	r3, [sp, #80]
	ldrb	r3, [r3, #152]	@ zero_extendqisi2
	lsls	r6, r3, #28
	bpl	.L534
	ldr	r3, [r7]
	ldr	r6, [r5]
	lsls	r0, r3, #30
	bpl	.L535
	mov	r2, #1448
	ldr	r1, .L747+36
	ldr	r0, .L747+40
	bl	printk
.L535:
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
	bpl	.L534
	movw	r2, #1473
	ldr	r1, .L747+36
	ldr	r0, .L747+20
	bl	printk
.L534:
	ldr	r3, [r5, #8]
	adds	r3, r3, #1
	str	r3, [r5, #8]
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L326
	mov	r3, r5
	movw	r2, #2428
	ldr	r1, .L747+44
	ldr	r0, .L747+48
	bl	printk
	b	.L326
.L484:
	ldr	r3, [r4, #340]
	ldr	r2, .L747+52
	ands	r2, r2, r3
	ldr	r3, .L747+56
	orrs	r3, r3, r2
	str	r3, [r4, #340]
	ldr	r3, [r4, #348]
	bfc	r3, #0, #20
	str	r3, [r4, #348]
	ldr	r2, [sp, #24]
	ldr	r3, [r4, #344]
	ldr	r2, [r2, #3408]
	bfi	r3, r2, #0, #20
.L719:
	str	r3, [r4, #344]
	ldrb	r3, [r4, #247]	@ zero_extendqisi2
	bfc	r3, #6, #1
	strb	r3, [r4, #247]
	b	.L488
.L486:
	ldr	r3, [r4, #340]
	ldr	r2, .L747+52
	ands	r2, r2, r3
	ldr	r3, .L747+60
	orrs	r3, r3, r2
	str	r3, [r4, #340]
	ldr	r3, [sp, #24]
	ldr	r2, [r4, #348]
	ldr	r3, [r3, #3408]
	subs	r3, r3, #1
	bfi	r2, r3, #0, #20
	ldr	r3, [r4, #344]
	str	r2, [r4, #348]
	bfc	r3, #0, #20
	b	.L719
.L493:
	cmp	r1, #4
	add	r2, r2, #2
	it	hi
	subhi	r1, r1, #2
	add	r0, r0, lr
	it	ls
	movls	r1, #2
	b	.L490
.L492:
	movw	r1, #1796
	cmp	r7, r1
	it	ne
	movne	r2, ip
	b	.L494
.L496:
	str	r1, [r4, #192]
	b	.L497
.L502:
	strd	lr, r8, [sp, #12]
	mov	r3, fp
	strd	r6, ip, [sp, #4]
	str	r7, [sp]
	bl	vepu540c_set_qpmap_normal
	b	.L503
.L508:
	cmp	r2, #31
	bhi	.L509
	movs	r2, #32
	b	.L720
.L509:
	cmp	r2, #33
	bhi	.L507
	movs	r2, #34
	b	.L720
.L513:
	movs	r2, #208
	ldr	r1, .L747+64
	b	.L721
.L512:
	movs	r2, #208
	ldr	r1, .L747+68
	b	.L721
.L516:
	orr	r3, r3, #262144
	orr	r3, r3, #2
	str	r3, [r6, #968]
	mov	r3, r2	@ movhi
	movs	r2, #9
	b	.L722
.L518:
	ldr	r0, .L747+72
	and	r1, r1, #-16777216
	and	r2, r2, #-16777216
	orrs	r3, r3, r0
	str	r3, [r6, #984]
	ldr	r3, .L747+76
	orrs	r2, r2, r0
	str	r2, [r6, #992]
	orrs	r3, r3, r1
	str	r3, [r6, #988]
	b	.L519
.L520:
	add	r0, r1, #440
.L524:
	ldr	r1, [r2], #4
	cmp	r0, r2
	strb	r1, [r3, #1]!
	ldr	r1, [r2, #124]
	and	r1, r1, #63
	strb	r1, [r3, #16]
	bne	.L524
	b	.L525
.L748:
	.align	2
.L747:
	.word	-268370040
	.word	12591108
	.word	8413248
	.word	1313797
	.word	.LANCHOR0+747
	.word	.LC27
	.word	.LANCHOR0+765
	.word	.LC20
	.word	.LC44
	.word	.LANCHOR0+797
	.word	.LC26
	.word	.LANCHOR0+393
	.word	.LC9
	.word	-2147450888
	.word	32800769
	.word	32800771
	.word	.LANCHOR1+600
	.word	.LANCHOR1+832
	.word	1053721
	.word	1184793
.L522:
	ldrb	r3, [r6, #974]	@ zero_extendqisi2
	movs	r2, #45
	bfi	r3, r2, #0, #6
	strb	r3, [r6, #974]
	ldr	r3, [r10, #44]
	ldr	r2, [r6, #992]
	cmp	r3, #2
	ldrd	r3, r0, [r6, #984]
	bne	.L526
	ldr	r1, .L749
	and	r0, r0, #-16777216
	and	r3, r3, #-16777216
	orr	r0, r0, #1179666
	orrs	r3, r3, r1
	orr	r0, r0, #4352
	str	r3, [r6, #984]
	str	r0, [r6, #988]
.L723:
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
	ldr	r3, .L749+4
	orrs	r3, r3, r2
	str	r3, [r6, #1024]
	b	.L523
.L526:
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
	ldr	r1, .L749+8
	orrs	r3, r3, r1
	str	r3, [r6, #984]
	and	r3, r0, #-16777216
	orr	r3, r3, #1572888
	orr	r3, r3, #6144
	str	r3, [r6, #988]
	b	.L723
.L529:
	str	r1, [r4, #192]
	b	.L530
.L532:
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
	b	.L531
.L750:
	.align	2
.L749:
	.word	1052688
	.word	4198404
	.word	1316375
	.fnend
	.size	hal_h264e_vepu540c_gen_regs, .-hal_h264e_vepu540c_gen_regs
	.global	hal_h264e_vepu540c
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.37434, %object
	.size	__func__.37434, 28
__func__.37434:
	.ascii	"hal_h264e_vepu540c_ret_task\000"
	.type	__func__.37403, %object
	.size	__func__.37403, 32
__func__.37403:
	.ascii	"hal_h264e_vepu540c_status_check\000"
	.type	__func__.37456, %object
	.size	__func__.37456, 33
__func__.37456:
	.ascii	"hal_h264e_vepu540c_ret_comb_task\000"
	.type	__func__.37394, %object
	.size	__func__.37394, 25
__func__.37394:
	.ascii	"hal_h264e_vepu540c_start\000"
	.type	__func__.37446, %object
	.size	__func__.37446, 30
__func__.37446:
	.ascii	"hal_h264e_vepu540c_comb_start\000"
	.type	__func__.37079, %object
	.size	__func__.37079, 23
__func__.37079:
	.ascii	"update_vepu540c_syntax\000"
	.type	__func__.37096, %object
	.size	__func__.37096, 28
__func__.37096:
	.ascii	"hal_h264e_vepu540c_get_task\000"
	.type	__FUNCTION__.37060, %object
	.size	__FUNCTION__.37060, 15
__FUNCTION__.37060:
	.ascii	"setup_hal_bufs\000"
	.type	__func__.37061, %object
	.size	__func__.37061, 15
__func__.37061:
	.ascii	"setup_hal_bufs\000"
	.type	__FUNCTION__.37005, %object
	.size	__FUNCTION__.37005, 13
__FUNCTION__.37005:
	.ascii	"get_wrap_buf\000"
	.type	__func__.36977, %object
	.size	__func__.36977, 26
__func__.36977:
	.ascii	"hal_h264e_vepu540c_deinit\000"
	.type	__FUNCTION__.36978, %object
	.size	__FUNCTION__.36978, 26
__FUNCTION__.36978:
	.ascii	"hal_h264e_vepu540c_deinit\000"
	.type	__func__.36985, %object
	.size	__func__.36985, 24
__func__.36985:
	.ascii	"hal_h264e_vepu540c_init\000"
	.type	__FUNCTION__.36988, %object
	.size	__FUNCTION__.36988, 24
__FUNCTION__.36988:
	.ascii	"hal_h264e_vepu540c_init\000"
	.type	__func__.37069, %object
	.size	__func__.37069, 27
__func__.37069:
	.ascii	"hal_h264e_vepu540c_prepare\000"
	.type	__func__.37410, %object
	.size	__func__.37410, 24
__func__.37410:
	.ascii	"hal_h264e_vepu540c_wait\000"
	.type	__func__.37380, %object
	.size	__func__.37380, 28
__func__.37380:
	.ascii	"hal_h264e_vepu540c_gen_regs\000"
	.type	__func__.37100, %object
	.size	__func__.37100, 22
__func__.37100:
	.ascii	"setup_vepu540c_normal\000"
	.type	__func__.37112, %object
	.size	__func__.37112, 20
__func__.37112:
	.ascii	"setup_vepu540c_prep\000"
	.type	__func__.37125, %object
	.size	__func__.37125, 21
__func__.37125:
	.ascii	"setup_vepu540c_codec\000"
	.type	__func__.37169, %object
	.size	__func__.37169, 24
__func__.37169:
	.ascii	"setup_vepu540c_rdo_pred\000"
	.type	__func__.37182, %object
	.size	__func__.37182, 23
__func__.37182:
	.ascii	"setup_vepu540c_rdo_cfg\000"
	.type	__func__.37203, %object
	.size	__func__.37203, 23
__func__.37203:
	.ascii	"setup_vepu540c_scl_cfg\000"
	.type	__func__.37223, %object
	.size	__func__.37223, 23
__func__.37223:
	.ascii	"setup_vepu540c_rc_base\000"
	.type	__func__.37241, %object
	.size	__func__.37241, 22
__func__.37241:
	.ascii	"setup_vepu540c_io_buf\000"
	.type	__FUNCTION__.37258, %object
	.size	__FUNCTION__.37258, 22
__FUNCTION__.37258:
	.ascii	"setup_vepu540c_io_buf\000"
	.type	__func__.37273, %object
	.size	__func__.37273, 25
__func__.37273:
	.ascii	"setup_vepu540c_recn_refr\000"
	.type	__FUNCTION__.37275, %object
	.size	__FUNCTION__.37275, 25
__FUNCTION__.37275:
	.ascii	"setup_vepu540c_recn_refr\000"
	.type	__func__.37029, %object
	.size	__func__.37029, 21
__func__.37029:
	.ascii	"setup_recn_refr_wrap\000"
	.type	ref_type_map, %object
	.size	ref_type_map, 16
ref_type_map:
	.word	0
	.word	1
	.word	2
	.word	3
	.type	__func__.37291, %object
	.size	__func__.37291, 21
__func__.37291:
	.ascii	"setup_vepu540c_split\000"
	.type	__func__.37325, %object
	.size	__func__.37325, 18
__func__.37325:
	.ascii	"setup_vepu540c_me\000"
	.type	__func__.37335, %object
	.size	__func__.37335, 18
__func__.37335:
	.ascii	"setup_vepu540c_l2\000"
	.type	__FUNCTION__.37190, %object
	.size	__FUNCTION__.37190, 32
__FUNCTION__.37190:
	.ascii	"vepu540c_h264e_save_pass1_patch\000"
	.type	__func__.37198, %object
	.size	__func__.37198, 31
__func__.37198:
	.ascii	"vepu540c_h264e_use_pass1_patch\000"
	.type	hal_h264e_vepu540c, %object
	.size	hal_h264e_vepu540c, 64
hal_h264e_vepu540c:
	.word	.LC20
	.word	7
	.word	368
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
	.word	8
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
	.type	h264_aq_tthd_cvr, %object
	.size	h264_aq_tthd_cvr, 64
h264_aq_tthd_cvr:
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
	.word	35
	.type	h264_I_aq_step_cvr, %object
	.size	h264_I_aq_step_cvr, 64
h264_I_aq_step_cvr:
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
	.word	8
	.type	h264_P_aq_step_cvr, %object
	.size	h264_P_aq_step_cvr, 64
h264_P_aq_step_cvr:
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
	.type	h264e_lambda_default, %object
	.size	h264e_lambda_default, 232
h264e_lambda_default:
	.word	3
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
	.type	h264e_lambda_cvr, %object
	.size	h264e_lambda_cvr, 232
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
	.space	16
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC1:
	.ascii	"\0016%s:%d: lkt_done finish\000"
.LC2:
	.ascii	"\0016%s:%d: enc_done finish\000"
.LC3:
	.ascii	"\0016%s:%d: enc_slice finsh\000"
.LC4:
	.ascii	"\0016%s:%d: safe clear finsh\000"
.LC5:
	.ascii	"\0013%s:%d: bit stream overflow\000"
.LC6:
	.ascii	"\0013%s:%d: bus write full\000"
.LC7:
	.ascii	"\0013%s:%d: bus error\000"
.LC8:
	.ascii	"\0013%s:%d: wdg timeout\000"
.LC9:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC10:
	.ascii	"\0013%s:%d: set register write failed %d\012\000"
.LC11:
	.ascii	"\0013%s:%d: set register read failed %d\012\000"
.LC12:
	.ascii	"\0013%s:%d: send cmd failed %d\012\000"
.LC13:
	.ascii	"\0016%s:%d: update cfg\000"
.LC14:
	.ascii	"\0016%s:%d: update sps\000"
.LC15:
	.ascii	"\0016%s:%d: update pps\000"
.LC16:
	.ascii	"\0016%s:%d: update slice\000"
.LC17:
	.ascii	"\0016%s:%d: update frames\000"
.LC18:
	.ascii	"\0016%s:%d: update prefix nal\000"
.LC19:
	.ascii	"\0016%s:%d: invalid syntax type %d\012\000"
.LC20:
	.ascii	"hal_h264e_vepu540c\000"
.LC21:
	.ascii	"\0016%s:%d: frame size %d -> %d max count %d -> %d\012"
	.ascii	"\000"
.LC22:
	.ascii	"\0013%s:%d: mpp_dev_init failed. ret: %d\012\000"
.LC23:
	.ascii	"\0013%d: HalVepu540cRegSet alloc fail\000"
.LC24:
	.ascii	"\0013%s:%d: poll cmd failed %d\012\000"
.LC25:
	.ascii	"\0016%s:%d: frame %d generate regs now\000"
.LC26:
	.ascii	"\0016%s:%d: enter\012\000"
.LC27:
	.ascii	"\0016%s:%d: leave\012\000"
.LC28:
	.ascii	"\0013%s:%d: invalid modification_of_pic_nums_idc %d"
	.ascii	"\012\000"
.LC29:
	.ascii	"\0013%s:%d: unsupported mmco 0 %d\012\000"
.LC30:
	.ascii	"\0016%s:%d: bittarget %d qp [%d %d %d]\012\000"
.LC31:
	.ascii	"\0013%d: cur frame already overflow [%d %d]!\012\000"
.LC32:
	.ascii	"\0013%d: online case set full frame err\000"
.LC33:
	.ascii	"buf_thumb\000"
.LC34:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC35:
	.ascii	"buf_pixel\000"
.LC36:
	.ascii	"\0016%s:%d: cur is idr  lt %d\012\000"
.LC37:
	.ascii	"\0016%s:%d: ref type %d\012\000"
.LC38:
	.ascii	"\0013%d: WARNING: not support lt ref to st when buf"
	.ascii	" is wrap\000"
.LC39:
	.ascii	"\0013%d: WARNING: not support lt ref to lt when buf"
	.ascii	" is wrap\000"
.LC40:
	.ascii	"\0016%s:%d: cur_is_ref %d\012\000"
.LC41:
	.ascii	"\0016%s:%d: hdr[size %d top %d bot %d cur %d pre %d"
	.ascii	"]\012\000"
.LC42:
	.ascii	"\0016%s:%d: bdy [size %d top %d bot %d cur %d pre %"
	.ascii	"d]\012\000"
.LC43:
	.ascii	"\0016%s:%d: invalide slice split mode %d\012\000"
.LC44:
	.ascii	"\0013%d: buf_pass1 maSlloc fail, debreath invaild\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
