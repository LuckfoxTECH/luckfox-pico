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
	.file	"hal_jpege_hdr.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	jpege_bits_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_bits_init, %function
jpege_bits_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r1, #28
	mov	r4, r0
	ldr	r0, .L2
	bl	mpp_osal_malloc
	str	r0, [r4]
	pop	{r4, pc}
.L3:
	.align	2
.L2:
	.word	.LANCHOR0
	.fnend
	.size	jpege_bits_init, .-jpege_bits_init
	.align	1
	.global	jpege_bits_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_bits_deinit, %function
jpege_bits_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r1, r0
	cbz	r0, .L4
	ldr	r0, .L6
	b	mpp_osal_free
.L4:
	bx	lr
.L7:
	.align	2
.L6:
	.word	.LANCHOR0+16
	.fnend
	.size	jpege_bits_deinit, .-jpege_bits_deinit
	.align	1
	.global	jpege_bits_setup
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_bits_setup, %function
jpege_bits_setup:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	strd	r1, r1, [r0]
	str	r2, [r0, #8]
	strd	r3, r3, [r0, #12]
	strd	r3, r3, [r0, #20]
	bx	lr
	.fnend
	.size	jpege_bits_setup, .-jpege_bits_setup
	.align	1
	.global	jpege_bits_put
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_bits_put, %function
jpege_bits_put:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, #1
	lsls	r3, r3, r2
	cmp	r3, r1
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r4, r0
	mov	r5, r1
	mov	r7, r2
	ldr	r6, [r0, #4]
	bhi	.L10
	mov	r1, #600
	ldr	r3, .L14
	str	r1, [sp]
	ldr	r2, .L14+4
	ldr	r0, .L14+8
	bl	printk
.L10:
	cmp	r7, #24
	ble	.L11
	movw	r1, #601
	ldr	r3, .L14
	str	r1, [sp]
	ldr	r2, .L14+12
	ldr	r0, .L14+8
	bl	printk
.L11:
	ldr	r3, [r4, #24]
	ldrb	r2, [r6]	@ zero_extendqisi2
	add	r3, r3, r7
	rsb	r1, r3, #32
	lsl	r1, r5, r1
	lsrs	r0, r3, #3
	orr	r1, r1, r2, lsl #24
	adds	r5, r6, r0
.L12:
	cmp	r6, r5
	lsr	r2, r1, #24
	bne	.L13
	strb	r2, [r6]
	ldr	r2, [r4, #16]
	str	r6, [r4, #4]
	add	r7, r7, r2
	mvn	r2, #7
	mla	r3, r2, r0, r3
	strd	r7, r1, [r4, #16]
	str	r3, [r4, #24]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L13:
	strb	r2, [r6], #1
	lsls	r1, r1, #8
	ldr	r2, [r4, #12]
	adds	r2, r2, #1
	str	r2, [r4, #12]
	b	.L12
.L15:
	.align	2
.L14:
	.word	.LANCHOR0+34
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.fnend
	.size	jpege_bits_put, .-jpege_bits_put
	.align	1
	.global	jpege_seek_bits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_seek_bits, %function
jpege_seek_bits:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r0
	ldrd	r6, r3, [r0, #4]
	mov	r5, r1
	cmp	r3, r1
	bhi	.L17
	movw	r1, #629
	ldr	r3, .L20
	str	r1, [sp]
	ldr	r2, .L20+4
	ldr	r0, .L20+8
	bl	printk
.L17:
	mov	r3, r5
.L18:
	cmp	r3, #7
	bhi	.L19
	lsrs	r3, r5, #3
	adds	r2, r6, r3
	str	r2, [r4, #4]
	ldr	r2, [r4, #16]
	add	r2, r2, r5
	str	r2, [r4, #16]
	ldrb	r2, [r6, r3]	@ zero_extendqisi2
	str	r2, [r4, #20]
	mvn	r2, #7
	mla	r3, r2, r3, r5
	str	r3, [r4, #24]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L19:
	ldr	r2, [r4, #12]
	subs	r3, r3, #8
	adds	r2, r2, #1
	str	r2, [r4, #12]
	b	.L18
.L21:
	.align	2
.L20:
	.word	.LANCHOR0+49
	.word	.LC3
	.word	.LC1
	.fnend
	.size	jpege_seek_bits, .-jpege_seek_bits
	.align	1
	.global	jpege_bits_align_byte
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_bits_align_byte, %function
jpege_bits_align_byte:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #16]
	ands	r2, r2, #7
	beq	.L22
	rsb	r2, r2, #8
	movs	r1, #0
	b	jpege_bits_put
.L22:
	bx	lr
	.fnend
	.size	jpege_bits_align_byte, .-jpege_bits_align_byte
	.align	1
	.global	jpege_bits_get_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_bits_get_buf, %function
jpege_bits_get_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0]
	bx	lr
	.fnend
	.size	jpege_bits_get_buf, .-jpege_bits_get_buf
	.align	1
	.global	jpege_bits_get_bitpos
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_bits_get_bitpos, %function
jpege_bits_get_bitpos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #16]
	bx	lr
	.fnend
	.size	jpege_bits_get_bitpos, .-jpege_bits_get_bitpos
	.align	1
	.global	jpege_bits_get_bytepos
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_bits_get_bytepos, %function
jpege_bits_get_bytepos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #12]
	bx	lr
	.fnend
	.size	jpege_bits_get_bytepos, .-jpege_bits_get_bytepos
	.align	1
	.global	write_jpeg_RestartInterval
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	write_jpeg_RestartInterval, %function
write_jpeg_RestartInterval:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r3, [r1, #136]
	mov	r5, r1
	cbz	r3, .L27
	movs	r2, #16
	movw	r1, #65501
	bl	jpege_bits_put
	mov	r0, r4
	movs	r2, #16
	movs	r1, #4
	bl	jpege_bits_put
	ldr	r1, [r5, #136]
	mov	r0, r4
	movs	r2, #16
	pop	{r3, r4, r5, lr}
	b	jpege_bits_put
.L27:
	pop	{r3, r4, r5, pc}
	.fnend
	.size	write_jpeg_RestartInterval, .-write_jpeg_RestartInterval
	.align	1
	.global	write_jpeg_header
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	write_jpeg_header, %function
write_jpeg_header:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r8, [r1, #120]
	mov	r6, r1
	mov	r5, r2
	cmp	r8, #0
	bne	.L30
.L35:
	ldr	r3, [r5]
	cbnz	r3, .L32
	ldr	r3, [r6, #64]
	cmp	r3, #0
	beq	.L36
.L65:
	str	r3, [r5]
.L32:
	ldr	r3, [r5, #4]
	cbnz	r3, .L37
	ldr	r3, [r6, #68]
	cmp	r3, #0
	beq	.L38
.L66:
	str	r3, [r5, #4]
.L37:
	movs	r2, #16
	movw	r1, #65499
	mov	r0, r4
	ldr	r10, [r5]
	bl	jpege_bits_put
	movs	r2, #16
	movs	r1, #67
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #0
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #0
	mov	r0, r4
	bl	jpege_bits_put
	ldr	r7, .L67
	mov	r8, #0
	mov	r9, r7
.L39:
	ldrb	r3, [r9], #1	@ zero_extendqisi2
	movs	r2, #8
	mov	r0, r4
	add	r8, r8, #1
	ldrb	r1, [r10, r3]	@ zero_extendqisi2
	bl	jpege_bits_put
	cmp	r8, #64
	bne	.L39
	movs	r2, #16
	movw	r1, #65499
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #16
	movs	r1, #67
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #0
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #1
	mov	r0, r4
	bl	jpege_bits_put
	ldr	r8, [r5, #4]
	movs	r5, #0
.L40:
	ldrb	r3, [r7], #1	@ zero_extendqisi2
	movs	r2, #8
	mov	r0, r4
	adds	r5, r5, #1
	ldrb	r1, [r8, r3]	@ zero_extendqisi2
	bl	jpege_bits_put
	cmp	r5, #64
	bne	.L40
	movs	r2, #16
	movw	r1, #65472
	mov	r0, r4
	ldrd	r5, r7, [r6]
	bl	jpege_bits_put
	movs	r2, #16
	movs	r1, #17
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #8
	mov	r1, r2
	mov	r0, r4
	bl	jpege_bits_put
	mov	r1, r7
	movs	r2, #16
	mov	r0, r4
	bl	jpege_bits_put
	mov	r1, r5
	movs	r2, #16
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #8
	movs	r1, #3
	mov	r0, r4
	bl	jpege_bits_put
	ldr	r7, .L67+4
	movs	r5, #3
.L41:
	ldr	r1, [r7], #4
	movs	r2, #8
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	ldr	r1, [r7, #8]
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	ldr	r1, [r7, #20]
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #8
	ldr	r1, [r7, #32]
	mov	r0, r4
	bl	jpege_bits_put
	subs	r5, r5, #1
	bne	.L41
	mov	r1, r6
	mov	r0, r4
	bl	write_jpeg_RestartInterval
	movs	r2, #16
	movw	r1, #65476
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #16
	movs	r1, #31
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	mov	r1, r5
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	mov	r1, r5
	mov	r0, r4
	bl	jpege_bits_put
	ldr	r7, .L67+8
	mov	r6, r5
	mov	r10, r7
.L42:
	ldr	r1, [r7, r6, lsl #3]
	movs	r2, #8
	mov	r0, r4
	adds	r6, r6, #1
	bl	jpege_bits_put
	cmp	r6, #16
	bne	.L42
	ldr	r7, .L67+12
	movs	r6, #0
	mov	r9, r7
.L43:
	ldr	r1, [r7, r6, lsl #3]
	movs	r2, #8
	mov	r0, r4
	adds	r6, r6, #1
	bl	jpege_bits_put
	cmp	r6, #12
	bne	.L43
	movs	r2, #16
	movw	r1, #65476
	mov	r0, r4
	ldr	r7, .L67+16
	bl	jpege_bits_put
	movs	r2, #16
	movs	r1, #181
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #1
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #0
	mov	r0, r4
	bl	jpege_bits_put
	movs	r6, #0
	mov	r8, r7
.L44:
	ldr	r1, [r7, r6, lsl #3]
	movs	r2, #8
	mov	r0, r4
	adds	r6, r6, #1
	bl	jpege_bits_put
	cmp	r6, #16
	bne	.L44
	ldr	fp, .L67+28
	movs	r6, #0
	mov	r7, fp
.L45:
	ldr	r1, [fp, r6, lsl #3]
	movs	r2, #8
	mov	r0, r4
	adds	r6, r6, #1
	bl	jpege_bits_put
	cmp	r6, #162
	bne	.L45
	movs	r2, #16
	movw	r1, #65476
	mov	r0, r4
	movs	r6, #0
	bl	jpege_bits_put
	movs	r2, #16
	movs	r1, #31
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #0
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #1
	mov	r0, r4
	bl	jpege_bits_put
.L46:
	add	r3, r10, r6, lsl #3
	movs	r2, #8
	mov	r0, r4
	adds	r6, r6, #1
	ldr	r1, [r3, #4]
	bl	jpege_bits_put
	cmp	r6, #16
	bne	.L46
	movs	r6, #0
.L47:
	add	r3, r9, r6, lsl #3
	movs	r2, #8
	mov	r0, r4
	adds	r6, r6, #1
	ldr	r1, [r3, #4]
	bl	jpege_bits_put
	cmp	r6, #12
	bne	.L47
	movs	r2, #16
	movw	r1, #65476
	mov	r0, r4
	movs	r6, #0
	bl	jpege_bits_put
	movs	r2, #16
	movs	r1, #181
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #1
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #1
	mov	r0, r4
	bl	jpege_bits_put
.L48:
	add	r3, r8, r6, lsl #3
	movs	r2, #8
	mov	r0, r4
	adds	r6, r6, #1
	ldr	r1, [r3, #4]
	bl	jpege_bits_put
	cmp	r6, #16
	bne	.L48
	movs	r6, #0
.L49:
	add	r3, r7, r6, lsl #3
	movs	r2, #8
	mov	r0, r4
	adds	r6, r6, #1
	ldr	r1, [r3, #4]
	bl	jpege_bits_put
	cmp	r6, #162
	bne	.L49
	movs	r2, #16
	movw	r1, #65498
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #16
	movs	r1, #12
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #8
	movs	r1, #3
	mov	r0, r4
	bl	jpege_bits_put
.L50:
	adds	r6, r5, #1
	movs	r2, #8
	mov	r0, r4
	mov	r1, r6
	bl	jpege_bits_put
	movs	r2, #4
	cbnz	r5, .L51
	mov	r1, r5
	mov	r0, r4
	bl	jpege_bits_put
	mov	r1, r5
	movs	r2, #4
	mov	r0, r4
	bl	jpege_bits_put
	movs	r5, #1
	b	.L50
.L30:
	ldr	r7, [r1, #124]
	movs	r2, #16
	movw	r1, #65534
	bl	jpege_bits_put
	add	r1, r8, #2
	movs	r2, #16
	mov	r0, r4
	bl	jpege_bits_put
	add	r8, r8, r7
.L33:
	cmp	r8, r7
	beq	.L35
	movs	r2, #8
	ldrb	r1, [r7], #1	@ zero_extendqisi2
	mov	r0, r4
	bl	jpege_bits_put
	b	.L33
.L36:
	ldr	r2, [r6, #48]
	ldr	r3, .L67+20
	add	r3, r3, r2, lsl #6
	b	.L65
.L38:
	ldr	r2, [r6, #48]
	ldr	r3, .L67+24
	add	r3, r3, r2, lsl #6
	b	.L66
.L53:
	movs	r5, #2
	b	.L50
.L51:
	movs	r1, #1
	mov	r0, r4
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #1
	mov	r0, r4
	bl	jpege_bits_put
	cmp	r6, #3
	bne	.L53
	mov	r0, r4
	movs	r2, #8
	movs	r1, #0
	bl	jpege_bits_put
	mov	r0, r4
	movs	r2, #8
	movs	r1, #63
	bl	jpege_bits_put
	mov	r0, r4
	movs	r2, #4
	movs	r1, #0
	bl	jpege_bits_put
	movs	r2, #4
	movs	r1, #0
	mov	r0, r4
	bl	jpege_bits_put
	mov	r0, r4
	bl	jpege_bits_align_byte
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L68:
	.align	2
.L67:
	.word	.LANCHOR0+1473
	.word	.LANCHOR0+1540
	.word	.LANCHOR0+1636
	.word	.LANCHOR0+1764
	.word	.LANCHOR0+1860
	.word	.LANCHOR0+65
	.word	.LANCHOR0+769
	.word	.LANCHOR0+1988
	.fnend
	.size	write_jpeg_header, .-write_jpeg_header
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.8468, %object
	.size	__FUNCTION__.8468, 16
__FUNCTION__.8468:
	.ascii	"jpege_bits_init\000"
	.type	__FUNCTION__.8472, %object
	.size	__FUNCTION__.8472, 18
__FUNCTION__.8472:
	.ascii	"jpege_bits_deinit\000"
	.type	__FUNCTION__.8488, %object
	.size	__FUNCTION__.8488, 15
__FUNCTION__.8488:
	.ascii	"jpege_bits_put\000"
	.type	__FUNCTION__.8503, %object
	.size	__FUNCTION__.8503, 16
__FUNCTION__.8503:
	.ascii	"jpege_seek_bits\000"
	.type	qtable_y, %object
	.size	qtable_y, 704
qtable_y:
	.byte	80
	.byte	56
	.byte	50
	.byte	80
	.byte	120
	.byte	-56
	.byte	-8
	.byte	-8
	.byte	60
	.byte	60
	.byte	72
	.byte	96
	.byte	-120
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	72
	.byte	68
	.byte	80
	.byte	120
	.byte	-56
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	72
	.byte	88
	.byte	112
	.byte	-104
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	92
	.byte	112
	.byte	-64
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	120
	.byte	-80
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-104
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	40
	.byte	28
	.byte	25
	.byte	40
	.byte	60
	.byte	100
	.byte	-128
	.byte	-96
	.byte	30
	.byte	30
	.byte	36
	.byte	48
	.byte	68
	.byte	-104
	.byte	-104
	.byte	-112
	.byte	36
	.byte	34
	.byte	40
	.byte	60
	.byte	100
	.byte	-112
	.byte	-80
	.byte	-112
	.byte	36
	.byte	44
	.byte	56
	.byte	76
	.byte	-128
	.byte	-32
	.byte	-56
	.byte	-96
	.byte	46
	.byte	56
	.byte	96
	.byte	-112
	.byte	-80
	.byte	-8
	.byte	-8
	.byte	-56
	.byte	60
	.byte	88
	.byte	-112
	.byte	-96
	.byte	-48
	.byte	-8
	.byte	-8
	.byte	-24
	.byte	124
	.byte	-96
	.byte	-56
	.byte	-32
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-72
	.byte	-24
	.byte	-16
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	27
	.byte	18
	.byte	17
	.byte	27
	.byte	40
	.byte	68
	.byte	88
	.byte	104
	.byte	20
	.byte	20
	.byte	23
	.byte	32
	.byte	44
	.byte	96
	.byte	100
	.byte	92
	.byte	23
	.byte	22
	.byte	27
	.byte	40
	.byte	68
	.byte	96
	.byte	116
	.byte	96
	.byte	23
	.byte	28
	.byte	38
	.byte	48
	.byte	88
	.byte	-112
	.byte	-120
	.byte	104
	.byte	30
	.byte	38
	.byte	62
	.byte	96
	.byte	116
	.byte	-72
	.byte	-80
	.byte	-128
	.byte	40
	.byte	58
	.byte	92
	.byte	108
	.byte	-120
	.byte	-80
	.byte	-64
	.byte	-96
	.byte	84
	.byte	108
	.byte	-120
	.byte	-112
	.byte	-80
	.byte	-48
	.byte	-56
	.byte	-88
	.byte	120
	.byte	-96
	.byte	-96
	.byte	-88
	.byte	-64
	.byte	-88
	.byte	-80
	.byte	-88
	.byte	20
	.byte	14
	.byte	13
	.byte	20
	.byte	30
	.byte	50
	.byte	64
	.byte	76
	.byte	15
	.byte	15
	.byte	18
	.byte	24
	.byte	34
	.byte	76
	.byte	76
	.byte	72
	.byte	18
	.byte	16
	.byte	20
	.byte	30
	.byte	50
	.byte	72
	.byte	88
	.byte	72
	.byte	18
	.byte	21
	.byte	28
	.byte	36
	.byte	64
	.byte	112
	.byte	100
	.byte	80
	.byte	23
	.byte	28
	.byte	46
	.byte	72
	.byte	88
	.byte	-120
	.byte	-120
	.byte	96
	.byte	30
	.byte	44
	.byte	72
	.byte	80
	.byte	104
	.byte	-120
	.byte	-112
	.byte	116
	.byte	62
	.byte	80
	.byte	100
	.byte	112
	.byte	-120
	.byte	-104
	.byte	-104
	.byte	-128
	.byte	92
	.byte	116
	.byte	120
	.byte	124
	.byte	-112
	.byte	-128
	.byte	-120
	.byte	124
	.byte	16
	.byte	11
	.byte	10
	.byte	16
	.byte	24
	.byte	40
	.byte	52
	.byte	62
	.byte	12
	.byte	12
	.byte	14
	.byte	19
	.byte	26
	.byte	58
	.byte	60
	.byte	56
	.byte	14
	.byte	13
	.byte	16
	.byte	24
	.byte	40
	.byte	58
	.byte	72
	.byte	56
	.byte	14
	.byte	17
	.byte	22
	.byte	29
	.byte	52
	.byte	88
	.byte	80
	.byte	62
	.byte	18
	.byte	22
	.byte	38
	.byte	56
	.byte	68
	.byte	112
	.byte	104
	.byte	80
	.byte	24
	.byte	36
	.byte	56
	.byte	64
	.byte	84
	.byte	104
	.byte	116
	.byte	92
	.byte	50
	.byte	64
	.byte	80
	.byte	88
	.byte	104
	.byte	124
	.byte	120
	.byte	104
	.byte	72
	.byte	92
	.byte	96
	.byte	100
	.byte	124
	.byte	100
	.byte	104
	.byte	100
	.byte	13
	.byte	9
	.byte	8
	.byte	13
	.byte	19
	.byte	32
	.byte	42
	.byte	50
	.byte	10
	.byte	10
	.byte	11
	.byte	15
	.byte	21
	.byte	46
	.byte	48
	.byte	44
	.byte	11
	.byte	10
	.byte	13
	.byte	19
	.byte	32
	.byte	46
	.byte	56
	.byte	46
	.byte	11
	.byte	14
	.byte	18
	.byte	23
	.byte	42
	.byte	72
	.byte	64
	.byte	50
	.byte	14
	.byte	18
	.byte	30
	.byte	46
	.byte	54
	.byte	88
	.byte	84
	.byte	62
	.byte	19
	.byte	28
	.byte	44
	.byte	52
	.byte	68
	.byte	84
	.byte	92
	.byte	76
	.byte	40
	.byte	52
	.byte	62
	.byte	72
	.byte	84
	.byte	100
	.byte	96
	.byte	84
	.byte	58
	.byte	76
	.byte	76
	.byte	80
	.byte	100
	.byte	80
	.byte	84
	.byte	80
	.byte	10
	.byte	7
	.byte	6
	.byte	10
	.byte	14
	.byte	24
	.byte	31
	.byte	38
	.byte	7
	.byte	7
	.byte	8
	.byte	11
	.byte	16
	.byte	36
	.byte	36
	.byte	34
	.byte	8
	.byte	8
	.byte	10
	.byte	14
	.byte	24
	.byte	34
	.byte	42
	.byte	34
	.byte	8
	.byte	10
	.byte	13
	.byte	17
	.byte	32
	.byte	52
	.byte	48
	.byte	38
	.byte	11
	.byte	13
	.byte	22
	.byte	34
	.byte	42
	.byte	68
	.byte	62
	.byte	46
	.byte	14
	.byte	21
	.byte	34
	.byte	38
	.byte	50
	.byte	62
	.byte	68
	.byte	56
	.byte	29
	.byte	38
	.byte	48
	.byte	52
	.byte	62
	.byte	76
	.byte	72
	.byte	62
	.byte	44
	.byte	56
	.byte	58
	.byte	60
	.byte	68
	.byte	60
	.byte	62
	.byte	60
	.byte	6
	.byte	4
	.byte	4
	.byte	6
	.byte	10
	.byte	16
	.byte	20
	.byte	24
	.byte	5
	.byte	5
	.byte	6
	.byte	8
	.byte	10
	.byte	23
	.byte	24
	.byte	22
	.byte	6
	.byte	5
	.byte	6
	.byte	10
	.byte	16
	.byte	23
	.byte	28
	.byte	22
	.byte	6
	.byte	7
	.byte	9
	.byte	12
	.byte	20
	.byte	36
	.byte	32
	.byte	25
	.byte	7
	.byte	9
	.byte	15
	.byte	22
	.byte	27
	.byte	44
	.byte	42
	.byte	31
	.byte	10
	.byte	14
	.byte	22
	.byte	26
	.byte	32
	.byte	42
	.byte	46
	.byte	38
	.byte	20
	.byte	26
	.byte	31
	.byte	36
	.byte	42
	.byte	48
	.byte	48
	.byte	40
	.byte	29
	.byte	38
	.byte	38
	.byte	40
	.byte	46
	.byte	40
	.byte	42
	.byte	40
	.byte	3
	.byte	2
	.byte	2
	.byte	3
	.byte	5
	.byte	8
	.byte	10
	.byte	12
	.byte	2
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	12
	.byte	12
	.byte	11
	.byte	3
	.byte	3
	.byte	3
	.byte	5
	.byte	8
	.byte	11
	.byte	14
	.byte	11
	.byte	3
	.byte	3
	.byte	4
	.byte	6
	.byte	10
	.byte	17
	.byte	16
	.byte	12
	.byte	4
	.byte	4
	.byte	7
	.byte	11
	.byte	14
	.byte	22
	.byte	21
	.byte	15
	.byte	5
	.byte	7
	.byte	11
	.byte	13
	.byte	16
	.byte	21
	.byte	23
	.byte	18
	.byte	10
	.byte	13
	.byte	16
	.byte	17
	.byte	21
	.byte	24
	.byte	24
	.byte	20
	.byte	14
	.byte	18
	.byte	19
	.byte	20
	.byte	22
	.byte	20
	.byte	21
	.byte	20
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	3
	.byte	3
	.byte	4
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	4
	.byte	4
	.byte	4
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	4
	.byte	1
	.byte	1
	.byte	1
	.byte	2
	.byte	3
	.byte	6
	.byte	5
	.byte	4
	.byte	1
	.byte	1
	.byte	2
	.byte	4
	.byte	5
	.byte	7
	.byte	7
	.byte	5
	.byte	2
	.byte	2
	.byte	4
	.byte	4
	.byte	5
	.byte	7
	.byte	8
	.byte	6
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	8
	.byte	7
	.byte	5
	.byte	6
	.byte	6
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.type	qtable_c, %object
	.size	qtable_c, 704
qtable_c:
	.byte	88
	.byte	92
	.byte	120
	.byte	-16
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	92
	.byte	108
	.byte	-120
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	120
	.byte	-120
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-16
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	44
	.byte	46
	.byte	60
	.byte	120
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	46
	.byte	54
	.byte	68
	.byte	-88
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	60
	.byte	66
	.byte	-112
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	120
	.byte	-88
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	28
	.byte	30
	.byte	40
	.byte	80
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	30
	.byte	36
	.byte	44
	.byte	112
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	40
	.byte	44
	.byte	96
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	80
	.byte	112
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	-88
	.byte	21
	.byte	23
	.byte	30
	.byte	60
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	23
	.byte	26
	.byte	34
	.byte	84
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	30
	.byte	34
	.byte	72
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	60
	.byte	84
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	124
	.byte	17
	.byte	18
	.byte	24
	.byte	48
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	18
	.byte	21
	.byte	26
	.byte	68
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	24
	.byte	26
	.byte	56
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	48
	.byte	68
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	100
	.byte	14
	.byte	14
	.byte	19
	.byte	38
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	14
	.byte	17
	.byte	21
	.byte	54
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	19
	.byte	21
	.byte	46
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	38
	.byte	54
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	80
	.byte	10
	.byte	11
	.byte	14
	.byte	28
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	11
	.byte	13
	.byte	16
	.byte	40
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	14
	.byte	16
	.byte	34
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	28
	.byte	40
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	60
	.byte	7
	.byte	7
	.byte	10
	.byte	19
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	7
	.byte	8
	.byte	10
	.byte	26
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	10
	.byte	10
	.byte	22
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	19
	.byte	26
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	40
	.byte	3
	.byte	4
	.byte	5
	.byte	9
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	4
	.byte	4
	.byte	5
	.byte	13
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	5
	.byte	5
	.byte	11
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	9
	.byte	13
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	20
	.byte	1
	.byte	1
	.byte	2
	.byte	3
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	1
	.byte	1
	.byte	2
	.byte	4
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	2
	.byte	2
	.byte	4
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	3
	.byte	4
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.byte	1
	.type	zigzag, %object
	.size	zigzag, 64
zigzag:
	.byte	0
	.byte	1
	.byte	8
	.byte	16
	.byte	9
	.byte	2
	.byte	3
	.byte	10
	.byte	17
	.byte	24
	.byte	32
	.byte	25
	.byte	18
	.byte	11
	.byte	4
	.byte	5
	.byte	12
	.byte	19
	.byte	26
	.byte	33
	.byte	40
	.byte	48
	.byte	41
	.byte	34
	.byte	27
	.byte	20
	.byte	13
	.byte	6
	.byte	7
	.byte	14
	.byte	21
	.byte	28
	.byte	35
	.byte	42
	.byte	49
	.byte	56
	.byte	57
	.byte	50
	.byte	43
	.byte	36
	.byte	29
	.byte	22
	.byte	15
	.byte	23
	.byte	30
	.byte	37
	.byte	44
	.byte	51
	.byte	58
	.byte	59
	.byte	52
	.byte	45
	.byte	38
	.byte	31
	.byte	39
	.byte	46
	.byte	53
	.byte	60
	.byte	61
	.byte	54
	.byte	47
	.byte	55
	.byte	62
	.byte	63
	.space	3
	.type	color_info, %object
	.size	color_info, 96
color_info:
	.word	1
	.word	2
	.word	3
	.word	2
	.word	1
	.word	1
	.word	2
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.word	1
	.word	2
	.word	3
	.word	2
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	1
	.type	dc_li, %object
	.size	dc_li, 128
dc_li:
	.word	0
	.word	0
	.word	1
	.word	3
	.word	5
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	1
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.type	dc_vij, %object
	.size	dc_vij, 96
dc_vij:
	.word	0
	.word	0
	.word	1
	.word	1
	.word	2
	.word	2
	.word	3
	.word	3
	.word	4
	.word	4
	.word	5
	.word	5
	.word	6
	.word	6
	.word	7
	.word	7
	.word	8
	.word	8
	.word	9
	.word	9
	.word	10
	.word	10
	.word	11
	.word	11
	.type	ac_li, %object
	.size	ac_li, 128
ac_li:
	.word	0
	.word	0
	.word	2
	.word	2
	.word	1
	.word	1
	.word	3
	.word	2
	.word	3
	.word	4
	.word	2
	.word	4
	.word	4
	.word	3
	.word	3
	.word	4
	.word	5
	.word	7
	.word	5
	.word	5
	.word	4
	.word	4
	.word	4
	.word	4
	.word	0
	.word	0
	.word	0
	.word	1
	.word	1
	.word	2
	.word	125
	.word	119
	.type	ac_vij, %object
	.size	ac_vij, 1296
ac_vij:
	.word	1
	.word	0
	.word	2
	.word	1
	.word	3
	.word	2
	.word	0
	.word	3
	.word	4
	.word	17
	.word	17
	.word	4
	.word	5
	.word	5
	.word	18
	.word	33
	.word	33
	.word	49
	.word	49
	.word	6
	.word	65
	.word	18
	.word	6
	.word	65
	.word	19
	.word	81
	.word	81
	.word	7
	.word	97
	.word	97
	.word	7
	.word	113
	.word	34
	.word	19
	.word	113
	.word	34
	.word	20
	.word	50
	.word	50
	.word	129
	.word	129
	.word	8
	.word	145
	.word	20
	.word	161
	.word	66
	.word	8
	.word	145
	.word	35
	.word	161
	.word	66
	.word	177
	.word	177
	.word	193
	.word	193
	.word	9
	.word	21
	.word	35
	.word	82
	.word	51
	.word	209
	.word	82
	.word	240
	.word	240
	.word	36
	.word	21
	.word	51
	.word	98
	.word	98
	.word	114
	.word	114
	.word	209
	.word	130
	.word	10
	.word	9
	.word	22
	.word	10
	.word	36
	.word	22
	.word	52
	.word	23
	.word	225
	.word	24
	.word	37
	.word	25
	.word	241
	.word	26
	.word	23
	.word	37
	.word	24
	.word	38
	.word	25
	.word	39
	.word	26
	.word	40
	.word	38
	.word	41
	.word	39
	.word	42
	.word	40
	.word	52
	.word	41
	.word	53
	.word	42
	.word	54
	.word	53
	.word	55
	.word	54
	.word	56
	.word	55
	.word	57
	.word	56
	.word	58
	.word	57
	.word	67
	.word	58
	.word	68
	.word	67
	.word	69
	.word	68
	.word	70
	.word	69
	.word	71
	.word	70
	.word	72
	.word	71
	.word	73
	.word	72
	.word	74
	.word	73
	.word	83
	.word	74
	.word	84
	.word	83
	.word	85
	.word	84
	.word	86
	.word	85
	.word	87
	.word	86
	.word	88
	.word	87
	.word	89
	.word	88
	.word	90
	.word	89
	.word	99
	.word	90
	.word	100
	.word	99
	.word	101
	.word	100
	.word	102
	.word	101
	.word	103
	.word	102
	.word	104
	.word	103
	.word	105
	.word	104
	.word	106
	.word	105
	.word	115
	.word	106
	.word	116
	.word	115
	.word	117
	.word	116
	.word	118
	.word	117
	.word	119
	.word	118
	.word	120
	.word	119
	.word	121
	.word	120
	.word	122
	.word	121
	.word	131
	.word	122
	.word	132
	.word	130
	.word	133
	.word	131
	.word	134
	.word	132
	.word	135
	.word	133
	.word	136
	.word	134
	.word	137
	.word	135
	.word	138
	.word	136
	.word	146
	.word	137
	.word	147
	.word	138
	.word	148
	.word	146
	.word	149
	.word	147
	.word	150
	.word	148
	.word	151
	.word	149
	.word	152
	.word	150
	.word	153
	.word	151
	.word	154
	.word	152
	.word	162
	.word	153
	.word	163
	.word	154
	.word	164
	.word	162
	.word	165
	.word	163
	.word	166
	.word	164
	.word	167
	.word	165
	.word	168
	.word	166
	.word	169
	.word	167
	.word	170
	.word	168
	.word	178
	.word	169
	.word	179
	.word	170
	.word	180
	.word	178
	.word	181
	.word	179
	.word	182
	.word	180
	.word	183
	.word	181
	.word	184
	.word	182
	.word	185
	.word	183
	.word	186
	.word	184
	.word	194
	.word	185
	.word	195
	.word	186
	.word	196
	.word	194
	.word	197
	.word	195
	.word	198
	.word	196
	.word	199
	.word	197
	.word	200
	.word	198
	.word	201
	.word	199
	.word	202
	.word	200
	.word	210
	.word	201
	.word	211
	.word	202
	.word	212
	.word	210
	.word	213
	.word	211
	.word	214
	.word	212
	.word	215
	.word	213
	.word	216
	.word	214
	.word	217
	.word	215
	.word	218
	.word	216
	.word	225
	.word	217
	.word	226
	.word	218
	.word	227
	.word	226
	.word	228
	.word	227
	.word	229
	.word	228
	.word	230
	.word	229
	.word	231
	.word	230
	.word	232
	.word	231
	.word	233
	.word	232
	.word	234
	.word	233
	.word	241
	.word	234
	.word	242
	.word	242
	.word	243
	.word	243
	.word	244
	.word	244
	.word	245
	.word	245
	.word	246
	.word	246
	.word	247
	.word	247
	.word	248
	.word	248
	.word	249
	.word	249
	.word	250
	.word	250
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"value < ((RK_U32) 1 << number)\000"
.LC1:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC2:
	.ascii	"number < 25\000"
.LC3:
	.ascii	"bits < impl->size\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
