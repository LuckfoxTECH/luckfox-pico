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
	.file	"mpp_bitwrite.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_writer_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_status, %function
mpp_writer_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r3, r2, [r0, #8]
	cmp	r2, r3
	ittte	hi
	movhi	r3, #1
	strhi	r3, [r0, #28]
	movhi	r0, #-1
	movls	r0, #0
	bx	lr
	.fnend
	.size	mpp_writer_status, .-mpp_writer_status
	.align	1
	.global	mpp_writer_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_reset, %function
mpp_writer_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0]
	mov	r3, r0
	str	r2, [r0, #4]
	movs	r0, #0
	strd	r0, r0, [r3, #12]
	strd	r0, r0, [r3, #20]
	strd	r0, r0, [r3, #28]
	bx	lr
	.fnend
	.size	mpp_writer_reset, .-mpp_writer_reset
	.align	1
	.global	mpp_writer_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_init, %function
mpp_writer_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	strd	r1, r1, [r0]
	str	r2, [r0, #8]
	movs	r0, #0
	str	r0, [r3, #12]
	str	r0, [r3, #28]
	strd	r0, r0, [r3, #16]
	str	r0, [r3, #24]
	str	r0, [r3, #32]
	bx	lr
	.fnend
	.size	mpp_writer_init, .-mpp_writer_init
	.align	1
	.global	mpp_writer_put_raw_bits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_put_raw_bits, %function
mpp_writer_put_raw_bits:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r6, r0
	mov	r5, r1
	mov	r4, r2
	ldr	r8, [r0, #16]
	ldr	r7, [r0, #4]
	bl	mpp_writer_status
	cbnz	r0, .L6
	movs	r3, #1
	lsls	r3, r3, r4
	cmp	r3, r1
	bgt	.L8
	movs	r1, #66
	ldr	r3, .L12
	str	r1, [sp]
	ldr	r2, .L12+4
	ldr	r0, .L12+8
	bl	printk
.L8:
	cmp	r4, #24
	ble	.L9
	movs	r1, #67
	ldr	r3, .L12
	str	r1, [sp]
	ldr	r2, .L12+12
	ldr	r0, .L12+8
	bl	printk
.L9:
	ldr	r2, [r6, #20]
	add	r2, r2, r4
	rsb	r1, r2, #32
	lsl	r1, r5, r1
	orr	r1, r1, r8
.L10:
	cmp	r2, #7
	bgt	.L11
	uxtb	r2, r2
	str	r1, [r6, #16]
	str	r2, [r6, #20]
	str	r7, [r6, #4]
.L6:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L11:
	lsrs	r3, r1, #24
	strb	r3, [r7], #1
	ldr	r3, [r6, #12]
	subs	r2, r2, #8
	lsls	r1, r1, #8
	adds	r3, r3, #1
	str	r3, [r6, #12]
	b	.L10
.L13:
	.align	2
.L12:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.fnend
	.size	mpp_writer_put_raw_bits, .-mpp_writer_put_raw_bits
	.align	1
	.global	mpp_writer_flush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_flush, %function
mpp_writer_flush:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r1, [r0, #16]
	ldr	r5, [r0, #4]
	bl	mpp_writer_status
	cbnz	r0, .L14
	ldr	r3, [r4, #20]
	cbz	r3, .L14
	lsrs	r1, r1, #24
	strb	r1, [r5]
.L14:
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_writer_flush, .-mpp_writer_flush
	.align	1
	.global	mpp_writer_put_bits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_put_bits, %function
mpp_writer_put_bits:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r4, r0
	mov	r5, r2
	ldr	r7, [r0, #4]
	ldr	r8, [r0, #16]
	mov	r6, r1
	cbz	r1, .L21
	movs	r3, #1
	lsls	r3, r3, r2
	cmp	r3, r1
	bgt	.L22
	movs	r1, #110
	ldr	r3, .L29
	str	r1, [sp]
	ldr	r2, .L29+4
	ldr	r0, .L29+8
	bl	printk
.L22:
	cmp	r5, #24
	ble	.L21
	movs	r1, #111
	ldr	r3, .L29
	str	r1, [sp]
	ldr	r2, .L29+12
	ldr	r0, .L29+8
	bl	printk
.L21:
	ldr	r3, [r4, #20]
	add	r5, r5, r3
	rsb	r1, r5, #32
	lsl	r1, r6, r1
	orr	r1, r1, r8
	mov	r8, #3
.L24:
	cmp	r5, #7
	bgt	.L28
	uxtb	r5, r5
	strd	r1, r5, [r4, #16]
.L19:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L28:
	mov	r0, r4
	ldr	r6, [r4, #24]
	ldr	r9, [r4, #12]
	bl	mpp_writer_status
	cmp	r0, #0
	bne	.L19
	lsrs	r3, r1, #24
	cmp	r6, #2
	add	ip, r9, #1
	uxtb	r2, r3
	strb	r2, [r7]
	bne	.L26
	cmp	r3, #3
	bhi	.L26
	mov	r3, r7
	add	ip, r9, #2
	strb	r8, [r3], #1
	mov	r6, r0
	strb	r2, [r7, #1]
	mov	r7, r3
	ldr	r2, [r4, #32]
	adds	r2, r2, #1
	str	r2, [r4, #32]
.L26:
	ldrb	r3, [r7]	@ zero_extendqisi2
	cbnz	r3, .L27
	adds	r0, r6, #1
.L27:
	adds	r7, r7, #1
	subs	r5, r5, #8
	lsls	r1, r1, #8
	str	r0, [r4, #24]
	str	ip, [r4, #12]
	str	r7, [r4, #4]
	b	.L24
.L30:
	.align	2
.L29:
	.word	.LANCHOR0+24
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.fnend
	.size	mpp_writer_put_bits, .-mpp_writer_put_bits
	.align	1
	.global	mpp_writer_align_zero
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_align_zero, %function
mpp_writer_align_zero:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #20]
	cbz	r2, .L31
	rsb	r2, r2, #8
	movs	r1, #0
	b	mpp_writer_put_raw_bits
.L31:
	bx	lr
	.fnend
	.size	mpp_writer_align_zero, .-mpp_writer_align_zero
	.align	1
	.global	mpp_writer_align_one
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_align_one, %function
mpp_writer_align_one:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #20]
	cbz	r2, .L33
	rsb	r2, r2, #8
	movs	r1, #1
	lsls	r1, r1, r2
	subs	r1, r1, #1
	b	mpp_writer_put_raw_bits
.L33:
	bx	lr
	.fnend
	.size	mpp_writer_align_one, .-mpp_writer_align_one
	.align	1
	.global	mpp_writer_trailing
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_trailing, %function
mpp_writer_trailing:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #1
	push	{r4, lr}
	.save {r4, lr}
	mov	r1, r2
	mov	r4, r0
	bl	mpp_writer_put_bits
	ldr	r2, [r4, #20]
	cbz	r2, .L35
	mov	r0, r4
	rsb	r2, r2, #8
	movs	r1, #0
	pop	{r4, lr}
	b	mpp_writer_put_bits
.L35:
	pop	{r4, pc}
	.fnend
	.size	mpp_writer_trailing, .-mpp_writer_trailing
	.align	1
	.global	mpp_writer_put_ue
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_put_ue, %function
mpp_writer_put_ue:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	adds	r5, r1, #1
	mov	r7, r0
	movs	r6, #0
.L38:
	adds	r4, r6, #1
	lsrs	r1, r5, r4
	bne	.L42
	cmp	r4, #12
	bls	.L39
	cmp	r6, #24
	bls	.L43
	movs	r2, #24
	mov	r0, r7
	sub	r8, r6, #24
	bl	mpp_writer_put_bits
.L40:
	mov	r2, r8
	movs	r1, #0
	mov	r0, r7
	bl	mpp_writer_put_bits
	cmp	r4, #24
	bls	.L41
	sub	r4, r6, #23
	movs	r2, #24
	lsrs	r5, r5, r4
	mov	r0, r7
	mov	r1, r5
	bl	mpp_writer_put_bits
.L41:
	mov	r2, r4
.L44:
	mov	r1, r5
	mov	r0, r7
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpp_writer_put_bits
.L42:
	mov	r6, r4
	b	.L38
.L43:
	mov	r8, r6
	b	.L40
.L39:
	lsls	r2, r4, #1
	subs	r2, r2, #1
	b	.L44
	.fnend
	.size	mpp_writer_put_ue, .-mpp_writer_put_ue
	.align	1
	.global	mpp_writer_put_se
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_put_se, %function
mpp_writer_put_se:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r1, #0
	itete	gt
	lslgt	r1, r1, #1
	mvnle	r3, #1
	addgt	r1, r1, #-1
	mulle	r1, r3, r1
	b	mpp_writer_put_ue
	.fnend
	.size	mpp_writer_put_se, .-mpp_writer_put_se
	.align	1
	.global	mpp_writer_bytes
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_bytes, %function
mpp_writer_bytes:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #20]
	ldr	r0, [r0, #12]
	cmp	r2, #0
	it	ne
	addne	r0, r0, #1
	bx	lr
	.fnend
	.size	mpp_writer_bytes, .-mpp_writer_bytes
	.align	1
	.global	mpp_writer_bits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_writer_bits, %function
mpp_writer_bits:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #12]
	ldr	r0, [r0, #20]
	add	r0, r0, r2, lsl #3
	bx	lr
	.fnend
	.size	mpp_writer_bits, .-mpp_writer_bits
	.align	1
	.global	mpp_exp_golomb_signed
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_exp_golomb_signed, %function
mpp_exp_golomb_signed:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	subs	r3, r0, #0
	mov	r0, #0
	itett	le
	mvnle	r2, #1
	lslgt	r3, r3, #1
	mulle	r3, r2, r3
	addle	r3, r3, #1
.L53:
	adds	r0, r0, #1
	asrs	r2, r3, r0
	bne	.L53
	lsls	r0, r0, #1
	subs	r0, r0, #1
	bx	lr
	.fnend
	.size	mpp_exp_golomb_signed, .-mpp_exp_golomb_signed
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.7247, %object
	.size	__FUNCTION__.7247, 24
__FUNCTION__.7247:
	.ascii	"mpp_writer_put_raw_bits\000"
	.type	__FUNCTION__.7268, %object
	.size	__FUNCTION__.7268, 20
__FUNCTION__.7268:
	.ascii	"mpp_writer_put_bits\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"val < (1 << len)\000"
.LC1:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC2:
	.ascii	"len < 25\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
