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
	.file	"vp8e_rc.c"
	.text
	.syntax unified
	.syntax unified
	.global	__aeabi_idiv
	.thumb
	.syntax unified
	.align	1
	.global	rc_model_v2_vp8_hal_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_model_v2_vp8_hal_start, %function
rc_model_v2_vp8_hal_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #44
	sub	sp, sp, #44
	ldr	r3, [r1, #160]
	mov	r5, r1
	ldr	r7, .L63
	mov	r4, r0
	ldrd	fp, r10, [r1, #164]
	str	r3, [sp, #28]
	ldrd	r3, r9, [r1, #176]
	ldrd	r8, r6, [r0]
	str	r3, [sp, #32]
	ldr	r3, [r1, #172]
	str	r3, [sp, #36]
	ldr	r3, [r7]
	lsls	r1, r3, #31
	bpl	.L2
	mov	r3, r0
	str	r5, [sp]
	movs	r2, #61
	ldr	r1, .L63+4
	ldr	r0, .L63+8
	bl	printk
.L2:
	ldr	r3, [r7]
	lsls	r2, r3, #25
	bpl	.L3
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	movs	r2, #63
	ldr	r1, .L63+4
	ubfx	r3, r3, #4, #1
	ldr	r0, .L63+12
	str	r3, [sp]
	ldrh	r3, [r5, #158]
	bl	printk
.L3:
	ldr	r3, [r5, #264]
	lsls	r3, r3, #31
	bpl	.L4
	ldr	r3, [r5, #268]
	strd	r3, r3, [r5, #172]
	str	r3, [r5, #180]
.L5:
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L4:
	ldr	r3, [r4, #8]
	cmp	r3, #2
	beq	.L5
	ldr	r3, [r4, #404]
	ldrb	ip, [r5, #152]	@ zero_extendqisi2
	and	ip, ip, #16
	cmp	r3, #0
	beq	.L7
	cmp	ip, #0
	beq	.L7
	ldr	r3, [r5, #172]
	cmp	r3, #0
	bge	.L8
	ldr	r0, [r5, #160]
	cbz	r0, .L9
	ldr	r3, .L63+16
	cmp	r0, r3
	bgt	.L28
	add	r8, r8, #15
	adds	r6, r6, #15
	asrs	r0, r0, #5
	asr	r8, r8, #4
	asrs	r6, r6, #4
	mul	r6, r6, r8
	sbfx	r6, r6, #0, #24
	add	r3, r6, #250
	adds	r1, r6, r6, lsl #1
	it	mi
	addmi	r1, r1, #3
	muls	r0, r3, r0
	asrs	r1, r1, #2
	add	r1, r1, #350
	bl	__aeabi_idiv
	lsls	r2, r6, #6
	mov	r1, #8000
	bl	axb_div_c
	ldr	r3, .L63+20
	mov	r2, #-1
.L12:
	ldr	r1, [r3], #4
	adds	r2, r2, #1
	cmp	r0, r1
	bgt	.L12
	ldr	r3, .L63+4
	add	r3, r3, r2, lsl #2
	ldr	r3, [r3, #76]
.L8:
	str	r3, [r4, #608]
	lsls	r3, r3, #6
	b	.L62
.L28:
	movs	r3, #10
	b	.L8
.L9:
	movs	r1, #85
	ldr	r0, .L63+24
	bl	printk
	movs	r3, #40
	str	r3, [r5, #172]
	str	r3, [r4, #608]
	mov	r3, #2560
.L62:
	str	r3, [r4, #604]
	ldr	r3, [r4, #632]
	cmp	r3, #0
	ble	.L14
	ldr	r2, [r4, #604]
	ldr	r0, [r7]
	ldr	r1, [r4, #580]
	lsls	r6, r0, #25
	add	r2, r2, r1
	str	r2, [r4, #604]
	asr	r3, r2, #6
	str	r3, [r4, #608]
	bpl	.L15
	strd	r2, r1, [sp]
	movs	r2, #100
	ldr	r1, .L63+4
	ldr	r0, .L63+28
	bl	printk
.L15:
	ldr	r2, [r5, #180]
	ldr	r3, [r5, #176]
	ldr	r1, [r4, #604]
	lsls	r2, r2, #6
	lsls	r3, r3, #6
	cmp	r2, r3
	bgt	.L16
	cmp	r1, r2
	blt	.L29
	cmp	r3, r1
	it	ge
	movge	r3, r1
.L16:
	str	r3, [r4, #604]
	asrs	r3, r3, #6
	strd	r3, r3, [r4, #584]
.L17:
	ldrd	r3, r2, [r5, #176]
	ldr	r1, [r4, #608]
	cmp	r2, r3
	bgt	.L23
	cmp	r1, r2
	blt	.L33
	cmp	r3, r1
	it	ge
	movge	r3, r1
.L23:
	ldr	r6, .L63
	str	r3, [r4, #608]
	str	r3, [r5, #172]
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L25
	ldr	r3, [r5, #164]
	movs	r2, #151
	ldr	r1, .L63+4
	ldr	r0, .L63+32
	str	r3, [sp, #16]
	ldr	r3, [r5, #160]
	str	r3, [sp, #12]
	ldr	r3, [r5, #168]
	strd	fp, r3, [sp, #4]
	ldr	r3, [sp, #28]
	str	r3, [sp]
	mov	r3, r10
	bl	printk
	ldr	r3, [r6]
	lsls	r2, r3, #25
	bpl	.L25
	ldr	r3, [r5, #176]
	movs	r2, #154
	ldr	r1, .L63+4
	ldr	r0, .L63+36
	str	r3, [sp, #16]
	ldr	r3, [r5, #172]
	str	r3, [sp, #12]
	ldr	r3, [r5, #180]
	str	r3, [sp, #8]
	ldr	r3, [sp, #32]
	str	r3, [sp, #4]
	ldr	r3, [sp, #36]
	str	r3, [sp]
	mov	r3, r9
	bl	printk
.L25:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L5
	mov	r3, r4
	movs	r2, #156
	ldr	r1, .L63+4
	ldr	r0, .L63+40
	bl	printk
	b	.L5
.L14:
	ldr	r3, [r4, #608]
	ldr	r2, [r4, #112]
	subs	r3, r3, r2
	str	r3, [r4, #608]
	b	.L15
.L29:
	mov	r3, r2
	b	.L16
.L7:
	ldr	r6, [r5, #180]
	ldr	r2, [r5, #176]
	ldr	r3, [r4, #580]
	ldr	r0, [r4, #604]
	lsls	r1, r6, #6
	add	r0, r0, r3
	lsls	r3, r2, #6
	cmp	ip, #0
	beq	.L18
	cmp	r1, r3
	bgt	.L19
	cmp	r0, r1
	blt	.L30
	cmp	r3, r0
	it	ge
	movge	r3, r0
.L19:
	cmp	r2, r6
	ldr	r1, [r4, #584]
	ldr	r0, [r4, #576]
	blt	.L20
	add	r0, r0, r3
	add	r1, r1, r0, asr #6
	asrs	r1, r1, #1
	cmp	r1, r6
	blt	.L31
	cmp	r2, r1
	it	ge
	movge	r2, r1
.L20:
	str	r2, [r4, #584]
	strd	r3, r2, [r4, #604]
.L21:
	ldr	r3, .L63
	ldr	r3, [r3]
	lsls	r0, r3, #25
	bpl	.L17
	ldr	r3, [r4, #116]
	movs	r2, #142
	ldr	r1, .L63+4
	ldr	r0, .L63+44
	str	r3, [sp]
	movs	r3, #0
	bl	printk
	b	.L17
.L30:
	mov	r3, r1
	b	.L19
.L31:
	mov	r2, r6
	b	.L20
.L18:
	cmp	r1, r3
	bgt	.L22
	cmp	r0, r1
	blt	.L32
	cmp	r3, r0
	it	ge
	movge	r3, r0
.L22:
	str	r3, [r4, #604]
	asrs	r3, r3, #6
	str	r3, [r4, #608]
	ldrb	r2, [r5, #154]	@ zero_extendqisi2
	and	r2, r2, #63
	cmp	r2, #3
	bne	.L21
	ldr	r2, [r4, #116]
	cmp	r2, #0
	beq	.L21
	subs	r3, r3, r2
	str	r3, [r4, #608]
	b	.L21
.L32:
	mov	r3, r1
	b	.L22
.L33:
	mov	r3, r2
	b	.L23
.L64:
	.align	2
.L63:
	.word	rc_debug
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	1000000
	.word	.LANCHOR0+28
	.word	.LC2
	.word	.LC3
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC4
	.fnend
	.size	rc_model_v2_vp8_hal_start, .-rc_model_v2_vp8_hal_start
	.global	default_vp8e
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.8858, %object
	.size	__func__.8858, 26
__func__.8858:
	.ascii	"rc_model_v2_vp8_hal_start\000"
	.space	2
	.type	qp_tbl.8837, %object
	.size	qp_tbl.8837, 96
qp_tbl.8837:
	.word	47
	.word	57
	.word	73
	.word	93
	.word	122
	.word	155
	.word	214
	.word	294
	.word	373
	.word	506
	.word	781
	.word	2147483647
	.word	120
	.word	110
	.word	100
	.word	90
	.word	80
	.word	70
	.word	60
	.word	50
	.word	40
	.word	30
	.word	20
	.word	10
	.type	default_vp8e, %object
	.size	default_vp8e, 48
default_vp8e:
	.word	.LC8
	.word	9
	.word	688
	.word	rc_model_v2_init
	.word	rc_model_v2_deinit
	.word	0
	.word	rc_model_v2_check_reenc
	.word	rc_model_v2_start
	.word	rc_model_v2_end
	.word	rc_model_v2_vp8_hal_start
	.word	rc_model_v2_hal_end
	.word	rc_model_v2_proc_show
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: enter p %p task %p\012\000"
.LC1:
	.ascii	"\0016%s:%d: seq_idx %d intra %d\012\000"
.LC2:
	.ascii	"\0016%d: fix qp case but init qp no set\000"
.LC3:
	.ascii	"\0016%s:%d: p->start_qp = %d, p->cur_scale_qp %d,p-"
	.ascii	">next_ratio %d \000"
.LC4:
	.ascii	"\0016%s:%d: i_quality_delta %d, vi_quality_delta %d"
	.ascii	"\000"
.LC5:
	.ascii	"\0016%s:%d: bitrate [%d : %d : %d] -> [%d : %d : %d"
	.ascii	"]\012\000"
.LC6:
	.ascii	"\0016%s:%d: quality [%d : %d : %d] -> [%d : %d : %d"
	.ascii	"]\012\000"
.LC7:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC8:
	.ascii	"default\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
