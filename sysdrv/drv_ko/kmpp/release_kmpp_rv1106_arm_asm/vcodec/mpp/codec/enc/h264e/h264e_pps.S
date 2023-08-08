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
	.file	"h264e_pps.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	h264e_pps_update
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_pps_update, %function
h264e_pps_update:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	movs	r3, #0
	strd	r3, r3, [r0]
	mov	r5, r1
	ldr	r2, [r1, #604]
	movs	r1, #26
	str	r3, [r0, #32]
	mov	r4, r0
	strd	r1, r1, [r0, #36]
	strd	r2, r3, [r0, #8]
	movs	r2, #1
	strd	r2, r2, [r0, #16]
	strd	r2, r2, [r0, #24]
	ldr	r1, [r5, #624]
	str	r1, [r0, #44]
	strd	r1, r2, [r0, #52]
	ldr	r2, [r5, #616]
	strd	r2, r3, [r0, #60]
	ldr	r3, [r5, #612]
	str	r3, [r0, #68]
	ldr	r3, [r5, #620]
	cmp	r3, #1
	bls	.L2
	movs	r1, #76
	ldr	r3, .L16
	str	r1, [sp]
	ldr	r2, .L16+4
	ldr	r0, .L16+8
	bl	printk
.L2:
	ldr	r3, [r5, #620]
	str	r3, [r4, #72]
	cbz	r3, .L3
	movs	r3, #1
	strd	r3, r3, [r4, #76]
	strd	r3, r3, [r4, #84]
	strd	r3, r3, [r4, #92]
	strd	r3, r3, [r4, #100]
.L3:
	ldr	r3, [r5, #596]
	cmp	r3, #99
	bgt	.L4
	ldr	r2, [r4, #68]
	movs	r3, #0
	str	r3, [r4, #48]
	cbz	r2, .L5
	str	r3, [r4, #68]
	movs	r2, #95
	ldr	r3, [r5, #596]
	ldr	r1, .L16+12
	ldr	r0, .L16+16
	bl	printk
.L5:
	ldr	r3, [r5, #596]
	cmp	r3, #66
	bne	.L6
	ldr	r3, [r4, #8]
	cbz	r3, .L6
	movs	r2, #103
	ldr	r1, .L16+12
	ldr	r0, .L16+20
	bl	printk
	movs	r3, #0
	str	r3, [r4, #8]
.L6:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L4:
	movs	r3, #1
	str	r3, [r4, #48]
	ldr	r3, [r5, #628]
	str	r3, [r4, #52]
	b	.L5
.L17:
	.align	2
.L16:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LANCHOR0+17
	.word	.LC2
	.word	.LC3
	.fnend
	.size	h264e_pps_update, .-h264e_pps_update
	.align	1
	.global	h264e_pps_to_packet
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_pps_to_packet, %function
h264e_pps_to_packet:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	.pad #44
	sub	sp, sp, #44
	mov	r0, r1
	mov	r5, r1
	mov	r9, r2
	bl	mpp_packet_get_pos
	mov	r6, r0
	mov	r0, r5
	bl	mpp_packet_get_data
	mov	r7, r0
	mov	r0, r5
	bl	mpp_packet_get_size
	mov	r10, r0
	mov	r0, r5
	bl	mpp_packet_get_length
	add	r2, r7, r10
	adds	r1, r6, r0
	mov	r8, r0
	subs	r2, r2, r1
	add	r0, sp, #4
	bl	mpp_writer_init
	movs	r2, #24
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	movs	r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #2
	movs	r1, #3
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #5
	movs	r1, #8
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	ldr	r1, [r4]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #4]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	movs	r2, #1
	ldr	r1, [r4, #8]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #12]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r1, [r4, #16]
	subs	r1, r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #20]
	subs	r1, r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #24]
	subs	r1, r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	movs	r2, #1
	ldr	r1, [r4, #28]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #2
	ldr	r1, [r4, #32]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r1, [r4, #36]
	subs	r1, r1, #26
	add	r0, sp, #4
	bl	mpp_writer_put_se
	ldr	r1, [r4, #40]
	subs	r1, r1, #26
	add	r0, sp, #4
	bl	mpp_writer_put_se
	ldr	r1, [r4, #44]
	add	r0, sp, #4
	bl	mpp_writer_put_se
	movs	r2, #1
	ldr	r1, [r4, #56]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #60]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r1, [r4, #64]
	movs	r2, #1
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r1, [r4, #68]
	cbnz	r1, .L19
	ldr	r3, [r4, #48]
	cbnz	r3, .L19
	ldr	r3, [r4, #72]
	cmp	r3, #0
	beq	.L20
.L19:
	movs	r2, #1
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r1, [r4, #72]
	movs	r2, #1
	adds	r1, r1, #0
	it	ne
	movne	r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #72]
	cbz	r3, .L21
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
.L21:
	ldr	r3, [r4, #72]
	cmp	r3, #1
	bne	.L22
.L25:
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
.L23:
	ldr	r1, [r4, #52]
	add	r0, sp, #4
	bl	mpp_writer_put_se
.L20:
	add	r0, sp, #4
	bl	mpp_writer_trailing
	add	r0, sp, #4
	bl	mpp_writer_bytes
	mov	r4, r0
	cmp	r9, #0
	beq	.L31
	str	r0, [r9]
.L31:
	mov	r0, r5
	add	r1, r4, r8
	bl	mpp_packet_set_length
	mov	r0, r4
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L22:
	cmp	r3, #2
	bne	.L23
	ldr	r3, [r4, #68]
	cmp	r3, #0
	beq	.L25
	movs	r2, #1
	ldr	r6, .L48
	mov	r1, r2
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	sub	r3, r6, #34
	mov	fp, r6
	movs	r7, #0
	mov	r10, r3
.L27:
	ldrb	r2, [fp], #1	@ zero_extendqisi2
	add	r2, r2, r3
	ldrsb	r1, [r2, #98]
	cbz	r7, .L32
	ldrb	r2, [fp, #-2]	@ zero_extendqisi2
	add	r2, r2, r3
	ldrsb	r2, [r2, #98]
.L26:
	subs	r1, r1, r2
	add	r0, sp, #4
	adds	r7, r7, #1
	sxtb	r1, r1
	bl	mpp_writer_put_se
	cmp	r7, #64
	ldr	r3, .L48+4
	bne	.L27
	movs	r2, #1
	add	r0, sp, #4
	mov	r1, r2
	movs	r7, #0
	bl	mpp_writer_put_bits
.L29:
	ldrb	r3, [r6], #1	@ zero_extendqisi2
	add	r3, r3, r10
	ldrsb	r1, [r3, #162]
	cbz	r7, .L33
	ldrb	r3, [r6, #-2]	@ zero_extendqisi2
	add	r3, r3, r10
	ldrsb	r3, [r3, #162]
.L28:
	subs	r1, r1, r3
	add	r0, sp, #4
	adds	r7, r7, #1
	sxtb	r1, r1
	bl	mpp_writer_put_se
	cmp	r7, #64
	bne	.L29
	b	.L23
.L32:
	movs	r2, #8
	b	.L26
.L33:
	movs	r3, #8
	b	.L28
.L49:
	.align	2
.L48:
	.word	.LANCHOR0+34
	.word	.LANCHOR0
	.fnend
	.size	h264e_pps_to_packet, .-h264e_pps_to_packet
	.align	1
	.global	h264e_pps_dump
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_pps_dump, %function
h264e_pps_dump:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	h264e_pps_dump, .-h264e_pps_dump
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.8864, %object
	.size	__FUNCTION__.8864, 17
__FUNCTION__.8864:
	.ascii	"h264e_pps_update\000"
	.type	__func__.8867, %object
	.size	__func__.8867, 17
__func__.8867:
	.ascii	"h264e_pps_update\000"
	.type	zigzag, %object
	.size	zigzag, 64
zigzag:
	.byte	0
	.byte	8
	.byte	1
	.byte	2
	.byte	9
	.byte	16
	.byte	24
	.byte	17
	.byte	10
	.byte	3
	.byte	4
	.byte	11
	.byte	18
	.byte	25
	.byte	32
	.byte	40
	.byte	33
	.byte	26
	.byte	19
	.byte	12
	.byte	5
	.byte	6
	.byte	13
	.byte	20
	.byte	27
	.byte	34
	.byte	41
	.byte	48
	.byte	56
	.byte	49
	.byte	42
	.byte	35
	.byte	28
	.byte	21
	.byte	14
	.byte	7
	.byte	15
	.byte	22
	.byte	29
	.byte	36
	.byte	43
	.byte	50
	.byte	57
	.byte	58
	.byte	51
	.byte	44
	.byte	37
	.byte	30
	.byte	23
	.byte	31
	.byte	38
	.byte	45
	.byte	52
	.byte	59
	.byte	60
	.byte	53
	.byte	46
	.byte	39
	.byte	47
	.byte	54
	.byte	61
	.byte	62
	.byte	55
	.byte	63
	.type	intra_scl, %object
	.size	intra_scl, 64
intra_scl:
	.byte	10
	.byte	11
	.byte	14
	.byte	16
	.byte	17
	.byte	19
	.byte	21
	.byte	23
	.byte	11
	.byte	12
	.byte	16
	.byte	17
	.byte	19
	.byte	21
	.byte	23
	.byte	25
	.byte	14
	.byte	16
	.byte	17
	.byte	19
	.byte	21
	.byte	23
	.byte	25
	.byte	27
	.byte	16
	.byte	17
	.byte	19
	.byte	21
	.byte	23
	.byte	25
	.byte	27
	.byte	28
	.byte	17
	.byte	19
	.byte	21
	.byte	23
	.byte	25
	.byte	27
	.byte	28
	.byte	29
	.byte	19
	.byte	21
	.byte	23
	.byte	25
	.byte	27
	.byte	28
	.byte	29
	.byte	30
	.byte	21
	.byte	23
	.byte	25
	.byte	27
	.byte	28
	.byte	29
	.byte	30
	.byte	31
	.byte	23
	.byte	25
	.byte	27
	.byte	28
	.byte	29
	.byte	30
	.byte	31
	.byte	32
	.type	inter_scl, %object
	.size	inter_scl, 64
inter_scl:
	.byte	12
	.byte	13
	.byte	15
	.byte	16
	.byte	17
	.byte	19
	.byte	20
	.byte	21
	.byte	13
	.byte	14
	.byte	16
	.byte	17
	.byte	19
	.byte	20
	.byte	21
	.byte	22
	.byte	15
	.byte	16
	.byte	17
	.byte	19
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	16
	.byte	17
	.byte	19
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	25
	.byte	17
	.byte	19
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	25
	.byte	27
	.byte	19
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	25
	.byte	27
	.byte	28
	.byte	20
	.byte	21
	.byte	22
	.byte	23
	.byte	25
	.byte	27
	.byte	28
	.byte	29
	.byte	21
	.byte	22
	.byte	23
	.byte	25
	.byte	27
	.byte	28
	.byte	29
	.byte	30
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"codec->scaling_list_mode == 0 || codec->scaling_lis"
	.ascii	"t_mode == 1\000"
.LC1:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC2:
	.ascii	"\0016%s:%d: warning: for profile %d transform_8x8_m"
	.ascii	"ode should be 0\012\000"
.LC3:
	.ascii	"\0016%s:%d: warning: for baseline profile entropy_c"
	.ascii	"oding_mode should be 0\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
