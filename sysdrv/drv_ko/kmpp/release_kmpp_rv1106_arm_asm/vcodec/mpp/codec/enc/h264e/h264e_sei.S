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
	.file	"h264e_sei.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	h264e_sei_to_packet
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_sei_to_packet, %function
h264e_sei_to_packet:
	.fnstart
	@ args = 8, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #76
	sub	sp, sp, #76
	mov	r9, r1
	mov	r10, r2
	mov	r7, r3
	mov	r6, r0
	bl	mpp_packet_get_pos
	ldr	r5, .L22
	str	r0, [sp, #28]
	mov	r0, r6
	bl	mpp_packet_get_data
	str	r0, [sp, #24]
	mov	r0, r6
	bl	mpp_packet_get_size
	str	r0, [sp, #20]
	mov	r0, r6
	bl	mpp_packet_get_length
	ldr	r1, [sp, #28]
	mov	r4, r0
	ldrd	r2, r3, [sp, #20]
	add	fp, r1, r0
	ldr	r0, [sp, #116]
	add	r8, r3, r2
	adds	r0, r0, #16
	str	r0, [sp, #20]
	ldr	r0, [r5]
	sub	r8, r8, fp
	lsls	r0, r0, #24
	bpl	.L2
	strd	r1, r4, [sp, #4]
	str	r2, [sp]
	movs	r2, #39
	ldr	r1, .L22+4
	ldr	r0, .L22+8
	bl	printk
.L2:
	mov	r2, r8
	mov	r1, fp
	add	r0, sp, #36
	sub	fp, r10, #255
	bl	mpp_writer_init
	movs	r2, #24
	movs	r1, #0
	mov	r8, #0
	add	r0, sp, #36
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	movs	r1, #1
	add	r0, sp, #36
	bl	mpp_writer_put_raw_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #36
	bl	mpp_writer_put_raw_bits
	movs	r2, #2
	movs	r1, #0
	add	r0, sp, #36
	bl	mpp_writer_put_raw_bits
	movs	r2, #5
	movs	r1, #6
	add	r0, sp, #36
	bl	mpp_writer_put_raw_bits
.L3:
	cmp	fp, r8
	mov	r2, #8
	bge	.L4
	sub	r1, r10, r8
	add	r0, sp, #36
	bl	mpp_writer_put_bits
	mov	r8, #0
	ldr	r3, [sp, #116]
	sub	r10, r3, #239
.L5:
	cmp	r10, r8
	mov	r2, #8
	bge	.L6
	ldr	r3, [sp, #20]
	add	r0, sp, #36
	sub	r1, r3, r8
	add	r8, r7, #-1
	bl	mpp_writer_put_bits
	adds	r7, r7, #15
.L7:
	ldrb	r1, [r8, #1]!	@ zero_extendqisi2
	movs	r2, #8
	add	r0, sp, #36
	bl	mpp_writer_put_bits
	cmp	r8, r7
	bne	.L7
	movs	r7, #0
.L8:
	ldr	r3, [sp, #116]
	cmp	r7, r3
	blt	.L9
	add	r0, sp, #36
	bl	mpp_writer_trailing
	add	r0, sp, #36
	bl	mpp_writer_bytes
	mov	r7, r0
	cmp	r9, #0
	beq	.L10
	str	r0, [r9]
.L10:
	add	r8, r7, r4
	mov	r0, r6
	mov	r1, r8
	bl	mpp_packet_set_length
	ldr	r3, [r5]
	lsls	r3, r3, #24
	bpl	.L11
	strd	r4, r8, [sp]
	mov	r3, r7
	movs	r2, #84
	ldr	r1, .L22+4
	ldr	r0, .L22+12
	bl	printk
.L11:
	movs	r0, #0
	add	sp, sp, #76
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L4:
	movs	r1, #255
	add	r0, sp, #36
	bl	mpp_writer_put_bits
	add	r8, r8, #255
	b	.L3
.L6:
	movs	r1, #255
	add	r0, sp, #36
	bl	mpp_writer_put_bits
	add	r8, r8, #255
	b	.L5
.L9:
	ldr	r3, [sp, #112]
	movs	r2, #8
	add	r0, sp, #36
	ldrb	r1, [r3, r7]	@ zero_extendqisi2
	adds	r7, r7, #1
	bl	mpp_writer_put_bits
	b	.L8
.L23:
	.align	2
.L22:
	.word	h264e_debug
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.fnend
	.size	h264e_sei_to_packet, .-h264e_sei_to_packet
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.7931, %object
	.size	__func__.7931, 20
__func__.7931:
	.ascii	"h264e_sei_to_packet\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: write sei to pkt [%p:%zu] [%p:%zu]\012\000"
.LC1:
	.ascii	"\0016%s:%d: sei data length %u pkt len %zu -> %zu\012"
	.ascii	"\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
