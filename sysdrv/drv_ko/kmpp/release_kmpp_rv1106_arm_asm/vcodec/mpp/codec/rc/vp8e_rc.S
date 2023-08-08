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
	ldr	r7, .L58
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
	ldr	r1, .L58+4
	ldr	r0, .L58+8
	bl	printk
.L2:
	ldr	r3, [r7]
	lsls	r2, r3, #25
	bpl	.L3
	ldrb	r3, [r5, #152]	@ zero_extendqisi2
	movs	r2, #63
	ldr	r1, .L58+4
	ubfx	r3, r3, #4, #1
	ldr	r0, .L58+12
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
	ldr	r3, [r4, #380]
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
	ldr	r3, .L58+16
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
	ldr	r3, .L58+20
	mov	r2, #-1
.L12:
	ldr	r1, [r3], #4
	adds	r2, r2, #1
	cmp	r0, r1
	bgt	.L12
	ldr	r3, .L58+4
	add	r3, r3, r2, lsl #2
	ldr	r3, [r3, #76]
.L8:
	str	r3, [r4, #584]
	lsls	r3, r3, #6
	b	.L57
.L28:
	movs	r3, #10
	b	.L8
.L9:
	movs	r1, #85
	ldr	r0, .L58+24
	bl	printk
	movs	r3, #40
	str	r3, [r5, #172]
	str	r3, [r4, #584]
	mov	r3, #2560
.L57:
	str	r3, [r4, #580]
	ldr	r3, [r4, #608]
	cmp	r3, #0
	ble	.L14
	ldr	r2, [r4, #580]
	ldr	r0, [r7]
	ldr	r1, [r4, #556]
	lsls	r6, r0, #25
	add	r2, r2, r1
	str	r2, [r4, #580]
	asr	r3, r2, #6
	str	r3, [r4, #584]
	bpl	.L15
	strd	r2, r1, [sp]
	movs	r2, #100
	ldr	r1, .L58+4
	ldr	r0, .L58+28
	bl	printk
.L15:
	ldr	r3, [r5, #180]
	ldr	r2, [r4, #580]
	ldr	r1, [r5, #176]
	lsls	r3, r3, #6
	cmp	r2, r3
	blt	.L16
	lsls	r3, r1, #6
	cmp	r3, r2
	it	ge
	movge	r3, r2
.L16:
	str	r3, [r4, #580]
	asrs	r3, r3, #6
	strd	r3, r3, [r4, #560]
.L17:
	ldr	r2, [r4, #584]
	ldrd	r1, r3, [r5, #176]
	cmp	r2, r3
	blt	.L23
	cmp	r2, r1
	mov	r3, r2
	it	ge
	movge	r3, r1
.L23:
	ldr	r6, .L58
	str	r3, [r4, #584]
	str	r3, [r5, #172]
	ldr	r3, [r6]
	lsls	r1, r3, #25
	bpl	.L25
	ldr	r3, [r5, #164]
	movs	r2, #151
	ldr	r1, .L58+4
	ldr	r0, .L58+32
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
	ldr	r1, .L58+4
	ldr	r0, .L58+36
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
	ldr	r1, .L58+4
	ldr	r0, .L58+40
	bl	printk
	b	.L5
.L14:
	ldr	r3, [r4, #584]
	ldr	r2, [r4, #112]
	subs	r3, r3, r2
	str	r3, [r4, #584]
	b	.L15
.L7:
	ldr	r1, [r5, #180]
	ldr	r0, [r5, #176]
	ldr	r3, [r4, #556]
	ldr	r2, [r4, #580]
	lsls	r6, r0, #6
	add	r2, r2, r3
	lsls	r3, r1, #6
	cmp	ip, #0
	beq	.L18
	cmp	r2, r3
	blt	.L19
	cmp	r2, r6
	mov	r3, r2
	it	ge
	movge	r3, r6
.L19:
	ldr	r2, [r4, #552]
	adds	r6, r3, r2
	ldr	r2, [r4, #560]
	add	r2, r2, r6, asr #6
	asrs	r2, r2, #1
	cmp	r2, r1
	blt	.L20
	cmp	r2, r0
	it	ge
	movge	r2, r0
	mov	r1, r2
.L20:
	str	r1, [r4, #560]
	strd	r3, r1, [r4, #580]
.L21:
	ldr	r3, .L58
	ldr	r3, [r3]
	lsls	r0, r3, #25
	bpl	.L17
	ldr	r3, [r4, #116]
	movs	r2, #142
	ldr	r1, .L58+4
	ldr	r0, .L58+44
	str	r3, [sp]
	movs	r3, #0
	bl	printk
	b	.L17
.L18:
	cmp	r2, r3
	blt	.L22
	cmp	r2, r6
	mov	r3, r2
	it	ge
	movge	r3, r6
.L22:
	str	r3, [r4, #580]
	asrs	r3, r3, #6
	str	r3, [r4, #584]
	ldrb	r2, [r5, #154]	@ zero_extendqisi2
	and	r2, r2, #63
	cmp	r2, #3
	bne	.L21
	ldr	r2, [r4, #116]
	cmp	r2, #0
	beq	.L21
	subs	r3, r3, r2
	str	r3, [r4, #584]
	b	.L21
.L59:
	.align	2
.L58:
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
	.type	__func__.8798, %object
	.size	__func__.8798, 26
__func__.8798:
	.ascii	"rc_model_v2_vp8_hal_start\000"
	.space	2
	.type	qp_tbl.8777, %object
	.size	qp_tbl.8777, 96
qp_tbl.8777:
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
	.word	664
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
