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
	.file	"h265e_ps.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	init_zscan2raster
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	init_zscan2raster, %function
init_zscan2raster:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	add	r10, r0, #-1
	mov	r9, #1
	mov	r7, r0
	lsl	r9, r9, r10
	mov	r4, r1
	mov	r6, r2
	mov	r5, r3
.L3:
	cmp	r7, r4
	bne	.L2
	ldr	r3, [r5]
	str	r6, [r3]
	ldr	r3, [r5]
	adds	r3, r3, #4
	str	r3, [r5]
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L2:
	asr	r8, r9, r4
	adds	r4, r4, #1
	mov	r2, r6
	mov	r3, r5
	mov	r1, r4
	mov	r0, r7
	bl	init_zscan2raster
	add	r2, r8, r6
	mov	r3, r5
	mov	r1, r4
	mov	r0, r7
	bl	init_zscan2raster
	lsl	r3, r8, r10
	add	r6, r6, r3
	mov	r1, r4
	mov	r3, r5
	mov	r0, r7
	mov	r2, r6
	add	r6, r6, r8
	bl	init_zscan2raster
	b	.L3
	.fnend
	.size	init_zscan2raster, .-init_zscan2raster
	.global	__aeabi_uidiv
	.align	1
	.global	init_raster2zscan
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	init_raster2zscan, %function
init_raster2zscan:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	subs	r1, r1, #1
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	lsr	r1, r0, r1
	mov	r4, r2
	mov	r5, r3
	bl	__aeabi_uidiv
	movs	r1, #0
	muls	r0, r0, r0
.L6:
	cmp	r0, r1
	bne	.L7
	pop	{r3, r4, r5, pc}
.L7:
	ldr	r3, [r5, r1, lsl #2]
	str	r1, [r4, r3, lsl #2]
	adds	r1, r1, #1
	b	.L6
	.fnend
	.size	init_raster2zscan, .-init_raster2zscan
	.align	1
	.global	init_raster2pelxy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	init_raster2pelxy, %function
init_raster2pelxy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	subs	r1, r1, #1
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	lsr	r6, r0, r1
	mov	r5, r2
	mov	r7, r3
	mov	r1, r6
	bl	__aeabi_uidiv
	mov	r3, r5
	movs	r1, #1
	movs	r2, #0
	str	r2, [r3], #4
	mov	r4, r0
.L9:
	cmp	r1, r4
	bcc	.L10
	lsl	r8, r4, #2
	movs	r5, #1
.L11:
	cmp	r5, r4
	bcc	.L12
	mul	r8, r4, r4
	movs	r5, #1
.L13:
	cmp	r8, r5
	bhi	.L14
	pop	{r4, r5, r6, r7, r8, pc}
.L10:
	str	r6, [r5, #4]
	adds	r7, r7, #4
	adds	r1, r1, #1
	b	.L9
.L12:
	sub	r1, r3, r8
	mov	r0, r3
	mov	r2, r8
	adds	r5, r5, #1
	bl	memcpy
	mov	r3, r0
	add	r3, r3, r8
	b	.L11
.L14:
	mov	r0, r5
	mov	r1, r4
	bl	__aeabi_uidiv
	muls	r0, r6, r0
	str	r0, [r7, r5, lsl #2]
	adds	r5, r5, #1
	b	.L13
	.fnend
	.size	init_raster2pelxy, .-init_raster2pelxy
	.align	1
	.global	h265e_set_vps
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_set_vps, %function
h265e_set_vps:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	movs	r5, #0
	ldr	r7, [r0]
	mov	r4, r1
	ldrd	r3, r2, [r7, #12]
	strd	r5, r5, [r1]
	strd	r5, r5, [r1, #96]
	mul	r8, r2, r3
	movs	r3, #1
	add	r2, r1, #12
	str	r3, [r1, #8]
	add	r3, r1, #4224
	adds	r1, r1, #40
	str	r5, [r3]
.L16:
	str	r5, [r2], #4
	ldr	r3, [r7, #608]
	str	r5, [r2, #52]
	cmp	r3, #3
	it	cc
	movcc	r3, #3
	cmp	r3, #16
	it	cs
	movcs	r3, #16
	cmp	r2, r1
	add	r3, r3, #-1
	str	r3, [r2, #24]
	bne	.L16
	add	r6, r4, #4224
	movs	r2, #32
	adds	r6, r6, #4
	movs	r1, #0
	adds	r0, r6, #1
	bl	memset
	movw	r3, #4592
	adds	r2, r4, r3
	str	r5, [r4, r3]	@ unaligned
	movw	r3, #4598
	strh	r5, [r2, #4]	@ unaligned
	adds	r2, r4, r3
	str	r5, [r4, r3]	@ unaligned
	movs	r3, #0
	strh	r5, [r2, #4]	@ unaligned
	movs	r5, #40
	ldr	r2, .L22
.L19:
	mul	r1, r5, r3
	ldr	r0, [r1, r2]
	cmp	r0, r8
	bcc	.L17
	add	r2, r2, r1
	ldr	r3, [r2, #28]
.L18:
	movs	r0, #0
	str	r0, [r6, #44]
	ldr	r2, [r7, #580]
	cmp	r2, r3
	ite	ge
	strge	r2, [r6, #48]
	strlt	r3, [r6, #48]
	movw	r2, #4228
	ldr	r3, [r7, #584]
	subs	r3, r3, r0
	it	ne
	movne	r3, #1
	strb	r3, [r4, r2]
	ldr	r3, [r7, #576]
	movs	r2, #1
	str	r3, [r6, #40]
	add	r3, r3, r6
	strb	r2, [r3, #1]
	strb	r2, [r6, #3]
	strb	r2, [r6, #33]
	strb	r0, [r6, #35]
	strb	r0, [r6, #36]
	pop	{r4, r5, r6, r7, r8, pc}
.L17:
	adds	r3, r3, #1
	cmp	r3, #14
	bne	.L19
	movs	r3, #0
	b	.L18
.L23:
	.align	2
.L22:
	.word	.LANCHOR0
	.fnend
	.size	h265e_set_vps, .-h265e_set_vps
	.global	__aeabi_idiv
	.global	__aeabi_uidivmod
	.align	1
	.global	h265e_set_sps
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_set_sps, %function
h265e_set_sps:
	.fnstart
	@ args = 0, pretend = 0, frame = 160
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L55
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #164
	sub	sp, sp, #164
	ldr	r5, [r0]
	mov	r4, r1
	ldr	r3, [r3]
	mov	r6, r2
	ldrd	r0, r1, [r5, #144]
	str	r3, [sp, #156]
	bl	__aeabi_idiv
	movw	r3, #4700
	str	r0, [sp, #12]
	ldr	r0, [r5, r3]
	bl	mpp_enc_ref_cfg_get_cpb_info
	movs	r2, #129
	mov	r1, #-1
	add	r3, r4, #356
	mov	fp, r0
	add	r0, sp, #27
	str	r3, [sp]
	str	r3, [sp, #20]
	bl	memset
	movs	r3, #0
	movs	r2, #4
.L25:
	add	r1, sp, #160
	add	r1, r1, r2
	lsls	r2, r2, #1
	strb	r3, [r1, #-133]
	adds	r3, r3, #1
	cmp	r3, #6
	bne	.L25
	ldr	r3, [r5, #592]
	add	r2, sp, #160
	add	r2, r2, r3
	ldrb	r7, [r2, #-133]	@ zero_extendqisi2
	movs	r2, #0
	add	r9, r7, #-1
	asr	r8, r3, r9
	asrs	r3, r3, r7
.L26:
	adds	r1, r2, #1
	movs	r0, #1
	str	r1, [sp, #4]
	adds	r1, r2, #2
	lsl	r1, r0, r1
	cmp	r3, r1
	bhi	.L41
	adds	r3, r2, r7
	mov	r1, r0
	add	r10, r3, r0
	str	r3, [sp, #8]
	movs	r2, #0
	add	r3, sp, #20
	mov	r0, r10
	bl	init_zscan2raster
	addw	r2, r4, #1380
	mov	r1, r10
	ldr	r0, [r5, #592]
	ldr	r3, [sp]
	bl	init_raster2zscan
	mov	r1, r10
	ldr	r0, [r5, #592]
	addw	r3, r4, #3428
	addw	r2, r4, #2404
	bl	init_raster2pelxy
	mov	r1, r8
	ldr	r0, [r5, #12]
	bl	__aeabi_uidivmod
	cmp	r1, #0
	beq	.L42
	movw	r3, #4469
	mov	r2, #1
	strb	r2, [r4, r3]
	movw	r3, #4520
	sub	r10, r8, r1
	str	r10, [r4, r3]
.L27:
	mov	r1, r8
	ldr	r0, [r5, #16]
	bl	__aeabi_uidivmod
	cbz	r1, .L28
	movw	r3, #4469
	movs	r2, #1
	strb	r2, [r4, r3]
	movw	r3, #4528
	sub	r1, r8, r1
	str	r1, [r4, r3]
.L28:
	movs	r3, #0
	movs	r2, #1
	strd	r3, r3, [r4, #48]
	strd	r2, r3, [r4, #56]
	ldr	r2, [r5, #12]
	add	r2, r2, r10
	str	r2, [r4, #64]
	ldr	r2, [r5, #16]
	add	r1, r1, r2
	str	r1, [r4, #68]
	ldr	r1, [sp, #8]
	mvn	r2, #2
	strd	r2, r3, [r4, #72]
	ldr	r2, [r5, #592]
	str	r1, [r4, #84]
	ldr	r1, [sp, #4]
	str	r2, [r4, #80]
	lsr	r2, r2, r9
	str	r1, [r4, #88]
	movw	r1, #4452
	str	r3, [r4, r1]
	movw	r1, #4456
	str	r3, [r4, r1]
.L29:
	cmp	r2, #1
	bgt	.L30
	movs	r2, #0
	subs	r3, r3, #3
	strb	r2, [r4, #4]
	subs	r7, r7, #1
	strd	r3, r9, [r4, #72]
	movs	r3, #5
	strb	r2, [r4, #1]
	str	r3, [r4, #136]
	movs	r3, #3
	strd	r7, r2, [r4, #120]
	str	r3, [r4, #140]
	movs	r3, #1
	strd	r3, r3, [r4, #128]
	ldr	r1, [r5, #600]
	strd	r2, r2, [r4, #144]
	strb	r1, [r4, #3]
	ldr	r2, [r5, #760]
	cbz	r2, .L31
	ldr	r3, [r5, #756]
	clz	r3, r3
	lsrs	r3, r3, #5
.L31:
	strb	r3, [r4, #9]
	movs	r1, #1
	strb	r1, [r4, #6]
	movs	r2, #0
	ldr	r3, [r6, #40]
	str	r3, [r4, #300]
	ldr	r3, [r6, #12]
	strb	r2, [r4, #5]
	str	r3, [r4, #92]
	movs	r3, #8
	strd	r3, r3, [r4, #152]
	ldr	r3, [r5, #744]
	strb	r3, [r4, #10]
	movs	r3, #16
	strd	r3, r2, [r4, #160]
	ldr	r3, [r5, #596]
	strb	r3, [r4, #2]
	ldr	r3, [r5, #712]
	clz	r3, r3
	lsrs	r3, r3, #5
	strb	r3, [r4, #7]
	ldr	r3, [fp, #4]
	cmp	r3, #0
	beq	.L32
	str	r3, [r4, #164]
	strb	r1, [r4, #1]
	strb	r2, [r4, #2]
	str	r2, [r5, #596]
.L33:
	add	r1, r4, #4480
	movw	r3, #13648
	movw	r0, #4484
	add	r6, r6, #4224
	movs	r2, #1
	adds	r6, r6, #4
	str	r6, [r4, r3]
	movs	r3, #0
	strb	r2, [r4, #8]
	str	r3, [r1]
	str	r3, [r4, r0]
	movw	r0, #4488
	str	r3, [r4, r0]
	movw	r0, #4460
	str	r3, [r4, r0]
	movw	r3, #4492
	movs	r0, #5
	str	r0, [r4, r3]
	ldr	r3, [r5, #52]
	cmp	r3, #2
	itttt	eq
	movweq	r3, #4464
	strbeq	r2, [r4, r3]
	movweq	r3, #4463
	strbeq	r2, [r4, r3]
	ldr	r3, [r5, #44]
	cmp	r3, #22
	bhi	.L35
	cmp	r3, #2
	bne	.L36
.L35:
	ldr	r3, [r5, #48]
	cmp	r3, #18
	bhi	.L37
	cmp	r3, #2
	bne	.L36
.L37:
	ldr	r3, [r5, #40]
	cmp	r3, #14
	bhi	.L38
	cmp	r3, #2
	beq	.L38
.L36:
	movw	r2, #4463
	movs	r3, #1
	strb	r3, [r4, r2]
	movw	r2, #4465
	strb	r3, [r4, r2]
	movw	r3, #4496
	ldr	r2, [r5, #44]
	str	r2, [r4, r3]
	movw	r3, #4500
	ldr	r2, [r5, #48]
	str	r2, [r4, r3]
	movw	r3, #4504
	ldr	r2, [r5, #40]
	str	r2, [r4, r3]
.L38:
	movw	r2, #4508
	movs	r3, #0
	str	r3, [r4, r2]
	add	r2, r4, #4512
	movw	r0, #257
	str	r3, [r2]
	movw	r2, #4466
	strh	r3, [r4, r2]	@ movhi
	movw	r2, #4468
	strb	r3, [r4, r2]
	movw	r2, #4472
	str	r3, [r4, r2]
	movw	r2, #4476
	strh	r0, [r4, r2]	@ movhi
	add	r2, r4, #4544
	movw	r0, #4556
	str	r3, [r2]
	movw	r3, #4548
	movs	r2, #2
	str	r2, [r4, r3]
	movw	r2, #4552
	movs	r3, #1
	str	r3, [r4, r2]
	movs	r2, #15
	str	r2, [r4, r0]
	movw	r0, #4560
	str	r2, [r4, r0]
	ldr	r2, [r5, #804]
	cbz	r2, .L39
	movw	r2, #4460
	strb	r3, [r4, r2]
	ldr	r3, [r5, #804]
	str	r3, [r1]
.L39:
	movw	r2, #4470
	movs	r3, #1
	strb	r3, [r4, r2]
	movw	r2, #4532
	str	r3, [r4, r2]
	movs	r5, #0
	ldr	r2, [sp, #12]
	movw	r3, #4536
	mov	r1, r5
	add	r0, r4, #168
	str	r2, [r4, r3]
	movs	r2, #132
	strd	r5, r5, [r4, #328]
	strd	r5, r5, [r4, #336]
	strd	r5, r5, [r4, #344]
	str	r5, [r4, #352]
	bl	memset
	movs	r2, #33
	mov	r1, r5
	add	r0, r4, #12
	bl	memset
	ldr	r3, .L55
	mov	r0, r5
	ldr	r2, [sp, #156]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L40
	bl	__stack_chk_fail
.L41:
	ldr	r2, [sp, #4]
	b	.L26
.L42:
	mov	r10, r1
	b	.L27
.L30:
	asrs	r2, r2, #1
	adds	r3, r3, #1
	b	.L29
.L32:
	ldr	r2, [fp, #16]
	cmp	r2, #0
	beq	.L33
	strb	r3, [r4, #2]
	b	.L33
.L40:
	add	sp, sp, #164
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L56:
	.align	2
.L55:
	.word	__stack_chk_guard
	.fnend
	.size	h265e_set_sps, .-h265e_set_sps
	.align	1
	.global	h265e_set_pps
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_set_pps, %function
h265e_set_pps:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	movs	r4, #1
	ldr	r3, [r0]
	movs	r0, #0
	str	r0, [r1, #32]
	strh	r0, [r1]	@ movhi
	strd	r0, r0, [r1, #24]
	ldr	r0, [r3, #108]
	str	r2, [r1, #724]
	movs	r2, #0
	cmp	r0, #2
	strb	r2, [r1, #2]
	ittt	ne
	movne	r0, #1
	strbne	r0, [r1]
	strne	r0, [r1, #36]
	ldr	r0, [r3, #748]
	strb	r2, [r1, #20]
	strh	r4, [r1, #8]	@ movhi
	strd	r0, r0, [r1, #40]
	strb	r2, [r1, #10]
	strb	r2, [r1, #13]
	ldr	r0, [r3, #628]
	subs	r0, r0, #26
	str	r0, [r1, #32]
	ldr	r0, [r3, #728]
	strb	r0, [r1, #5]
	ldr	r0, [r3, #768]
	cmp	r0, r2
	ite	eq
	moveq	r5, r4
	movne	r5, r2
	strb	r5, [r1, #4]
	bne	.L59
	strb	r0, [r1, #6]
	ldr	r2, [r3, #772]
	cbz	r2, .L60
	strb	r4, [r1, #7]
.L61:
	movs	r0, #0
	movs	r2, #1
	str	r0, [r1, #716]
	strb	r2, [r1, #17]
	strd	r2, r2, [r1, #48]
	ldr	r4, [r3, #736]
	strb	r4, [r1, #18]
	ldr	r4, [r3, #740]
	strb	r4, [r1, #19]
	ldr	r3, [r3, #732]
	strb	r0, [r1, #3]
	str	r0, [r1, #720]
	strb	r3, [r1, #12]
	strh	r0, [r1, #14]	@ movhi
	str	r0, [r1, #192]
	str	r0, [r1, #56]
	strb	r2, [r1, #11]
	pop	{r4, r5, pc}
.L60:
	ldr	r2, [r3, #776]
	cmp	r2, #0
	ite	ne
	movne	r2, #1
	moveq	r2, #0
	strb	r2, [r1, #7]
	bne	.L61
	ldr	r2, [r3, #772]
	str	r2, [r1, #708]
	ldr	r2, [r3, #776]
.L65:
	str	r2, [r1, #712]
	b	.L61
.L59:
	strh	r2, [r1, #6]	@ movhi
	str	r2, [r1, #708]
	b	.L65
	.fnend
	.size	h265e_set_pps, .-h265e_set_pps
	.global	levels
	.data
	.align	2
	.set	.LANCHOR0,. + 0
	.type	levels, %object
	.size	levels, 560
levels:
	.word	36864
	.word	552960
	.word	128
	.word	-1
	.word	350
	.word	-1
	.word	2
	.word	30
	.word	.LC0
	.word	10
	.word	122880
	.word	3686400
	.word	1500
	.word	-1
	.word	1500
	.word	-1
	.word	2
	.word	60
	.word	.LC1
	.word	20
	.word	245760
	.word	7372800
	.word	3000
	.word	-1
	.word	3000
	.word	-1
	.word	2
	.word	63
	.word	.LC2
	.word	21
	.word	552960
	.word	16588800
	.word	6000
	.word	-1
	.word	6000
	.word	-1
	.word	2
	.word	90
	.word	.LC3
	.word	30
	.word	983040
	.word	33177600
	.word	10000
	.word	-1
	.word	10000
	.word	-1
	.word	2
	.word	93
	.word	.LC4
	.word	31
	.word	2228224
	.word	66846720
	.word	12000
	.word	30000
	.word	12000
	.word	30000
	.word	4
	.word	120
	.word	.LC5
	.word	40
	.word	2228224
	.word	133693440
	.word	20000
	.word	50000
	.word	20000
	.word	50000
	.word	4
	.word	123
	.word	.LC6
	.word	41
	.word	8912896
	.word	267386880
	.word	25000
	.word	100000
	.word	25000
	.word	100000
	.word	6
	.word	150
	.word	.LC7
	.word	50
	.word	8912896
	.word	534773760
	.word	40000
	.word	160000
	.word	40000
	.word	160000
	.word	8
	.word	153
	.word	.LC8
	.word	51
	.word	8912896
	.word	1069547520
	.word	60000
	.word	240000
	.word	60000
	.word	240000
	.word	8
	.word	156
	.word	.LC9
	.word	52
	.word	35651584
	.word	1069547520
	.word	60000
	.word	240000
	.word	60000
	.word	240000
	.word	8
	.word	180
	.word	.LC10
	.word	60
	.word	35651584
	.word	2139095040
	.word	120000
	.word	480000
	.word	120000
	.word	480000
	.word	8
	.word	183
	.word	.LC11
	.word	61
	.word	35651584
	.word	-16777216
	.word	240000
	.word	800000
	.word	240000
	.word	800000
	.word	6
	.word	186
	.word	.LC12
	.word	62
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	-1
	.word	1
	.word	255
	.word	.LC13
	.word	85
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"1\000"
.LC1:
	.ascii	"2\000"
.LC2:
	.ascii	"2.1\000"
.LC3:
	.ascii	"3\000"
.LC4:
	.ascii	"3.1\000"
.LC5:
	.ascii	"4\000"
.LC6:
	.ascii	"4.1\000"
.LC7:
	.ascii	"5\000"
.LC8:
	.ascii	"5.1\000"
.LC9:
	.ascii	"5.2\000"
.LC10:
	.ascii	"6\000"
.LC11:
	.ascii	"6.1\000"
.LC12:
	.ascii	"6.2\000"
.LC13:
	.ascii	"8.5\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
