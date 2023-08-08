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
	.file	"h264e_slice.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	h264e_slice_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_slice_init, %function
h264e_slice_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r1
	mov	r4, r2
	movs	r1, #0
	movs	r2, #132
	bl	memset
	movs	r2, #1
	strd	r5, r4, [r0, #88]
	str	r2, [r0, #112]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	h264e_slice_init, .-h264e_slice_init
	.align	1
	.global	h264e_slice_update
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_slice_update, %function
h264e_slice_update:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	ldr	r5, [sp, #16]
	ldr	r6, [r2, #60]
	ldrb	r4, [r5, #16]	@ zero_extendqisi2
	str	r6, [r0]
	ldr	r6, [r2, #64]
	ubfx	r4, r4, #5, #1
	str	r6, [r0, #4]
	ldr	r6, [r2, #52]
	str	r6, [r0, #8]
	ldr	r6, [r2, #40]
	adds	r6, r6, #4
	str	r6, [r0, #16]
	ldr	r6, [r2, #48]
	adds	r6, r6, #4
	str	r6, [r0, #20]
	ldr	r6, [r1, #604]
	str	r6, [r0, #12]
	ldr	r2, [r2, #44]
	str	r2, [r0, #24]
	ldr	r3, [r3, #36]
	str	r3, [r0, #28]
	ldrb	r3, [r5, #16]	@ zero_extendqisi2
	lsls	r6, r3, #25
	bmi	.L12
	cmp	r4, #0
	ite	ne
	movne	r3, #3
	moveq	r3, #2
.L3:
	cmp	r4, #0
	str	r3, [r0, #32]
	ite	ne
	movne	r3, #5
	moveq	r3, #1
	str	r3, [r0, #36]
	mov	r3, #0
	it	eq
	moveq	r2, r3
	str	r3, [r0, #40]
	it	ne
	movne	r2, #2
	str	r3, [r0, #52]
	str	r2, [r0, #44]
	ldr	r2, [r5, #24]
	str	r3, [r0, #64]
	strd	r2, r3, [r0, #56]
	ldr	r3, [r1, #604]
	cbz	r3, .L16
	ldr	r3, [r1, #608]
.L6:
	str	r3, [r0, #68]
	ldr	r3, [r1, #632]
	str	r3, [r0, #72]
	ldr	r3, [r1, #636]
	str	r3, [r0, #76]
	ldr	r3, [r1, #640]
	str	r4, [r0, #96]
	str	r3, [r0, #80]
	cbz	r4, .L8
	ldr	r3, [r0, #104]
	str	r3, [r0, #100]
	adds	r3, r3, #1
	cmp	r3, #15
	it	hi
	movhi	r3, #0
	str	r3, [r0, #104]
.L8:
	ldr	r3, [r5, #32]
	movs	r2, #1
	str	r3, [r0, #108]
	movs	r3, #0
	strd	r2, r3, [r0, #112]
	cbz	r4, .L11
	ldrb	r4, [r5, #16]	@ zero_extendqisi2
	lsrs	r4, r4, #7
.L11:
	str	r4, [r0, #120]
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L12:
	movs	r3, #0
	b	.L3
.L16:
	mov	r3, #-1
	b	.L6
	.fnend
	.size	h264e_slice_update, .-h264e_slice_update
	.align	1
	.global	h264e_reorder_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_reorder_init, %function
h264e_reorder_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #16
	str	r3, [r0, #8]
	movs	r3, #0
	strd	r3, r3, [r0]
	mov	r0, r3
	bx	lr
	.fnend
	.size	h264e_reorder_init, .-h264e_reorder_init
	.align	1
	.global	h264e_reorder_wr_rewind
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_reorder_wr_rewind, %function
h264e_reorder_wr_rewind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	str	r3, [r0, #4]
	mov	r0, r3
	bx	lr
	.fnend
	.size	h264e_reorder_wr_rewind, .-h264e_reorder_wr_rewind
	.align	1
	.global	h264e_reorder_rd_rewind
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_reorder_rd_rewind, %function
h264e_reorder_rd_rewind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	str	r3, [r0]
	mov	r0, r3
	bx	lr
	.fnend
	.size	h264e_reorder_rd_rewind, .-h264e_reorder_rd_rewind
	.align	1
	.global	h264e_reorder_wr_op
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_reorder_wr_op, %function
h264e_reorder_wr_op:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	ldrd	r3, r2, [r0, #4]
	cmp	r3, r2
	blt	.L25
	str	r2, [sp]
	movs	r2, #110
	ldr	r1, .L27
	ldr	r0, .L27+4
	bl	printk
	mov	r0, #-1
.L24:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L25:
	adds	r2, r3, #1
	str	r2, [r0, #4]
	add	r0, r0, r3, lsl #4
	add	r4, r0, #12
	ldm	r1, {r0, r1, r2, r3}
	stm	r4, {r0, r1, r2, r3}
	movs	r0, #0
	b	.L24
.L28:
	.align	2
.L27:
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	h264e_reorder_wr_op, .-h264e_reorder_wr_op
	.align	1
	.global	h264e_reorder_rd_op
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_reorder_rd_op, %function
h264e_reorder_rd_op:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r1
	ldrd	r3, r2, [r0]
	cmp	r3, r2
	ittte	lt
	addlt	r2, r3, #1
	strlt	r2, [r0]
	addlt	r0, r0, r3, lsl #4
	movge	r0, #-1
	itttt	lt
	addlt	r0, r0, #12
	ldmlt	r0, {r0, r1, r2, r3}
	stmlt	r4, {r0, r1, r2, r3}
	movlt	r0, #0
	pop	{r4, pc}
	.fnend
	.size	h264e_reorder_rd_op, .-h264e_reorder_rd_op
	.align	1
	.global	h264e_marking_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_marking_init, %function
h264e_marking_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	movs	r2, #8
	strd	r3, r3, [r0]
	strd	r3, r3, [r0, #8]
	strd	r3, r2, [r0, #20]
	str	r3, [r0, #16]
	mov	r0, r3
	bx	lr
	.fnend
	.size	h264e_marking_init, .-h264e_marking_init
	.align	1
	.global	h264e_marking_is_empty
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_marking_is_empty, %function
h264e_marking_is_empty:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r2, r0, [r0, #16]
	cmp	r2, r0
	ite	lt
	movlt	r0, #0
	movge	r0, #1
	bx	lr
	.fnend
	.size	h264e_marking_is_empty, .-h264e_marking_is_empty
	.align	1
	.global	h264e_marking_wr_rewind
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_marking_wr_rewind, %function
h264e_marking_wr_rewind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	str	r3, [r0, #20]
	mov	r0, r3
	bx	lr
	.fnend
	.size	h264e_marking_wr_rewind, .-h264e_marking_wr_rewind
	.align	1
	.global	h264e_marking_rd_rewind
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_marking_rd_rewind, %function
h264e_marking_rd_rewind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	str	r3, [r0, #16]
	mov	r0, r3
	bx	lr
	.fnend
	.size	h264e_marking_rd_rewind, .-h264e_marking_rd_rewind
	.align	1
	.global	h264e_marking_wr_op
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_marking_wr_op, %function
h264e_marking_wr_op:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	ldrd	r3, r2, [r0, #20]
	cmp	r3, r2
	blt	.L37
	str	r2, [sp]
	movs	r2, #161
	ldr	r1, .L39
	ldr	r0, .L39+4
	bl	printk
	mov	r0, #-1
.L36:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L37:
	movs	r4, #20
	adds	r2, r3, #1
	mla	r3, r4, r3, r0
	mov	r5, r1
	str	r2, [r0, #20]
	add	r4, r3, #28
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	movs	r0, #0
	ldr	r3, [r5]
	str	r3, [r4]
	b	.L36
.L40:
	.align	2
.L39:
	.word	.LANCHOR0+20
	.word	.LC1
	.fnend
	.size	h264e_marking_wr_op, .-h264e_marking_wr_op
	.align	1
	.global	h264e_marking_rd_op
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_marking_rd_op, %function
h264e_marking_rd_op:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	ldrd	r4, r3, [r0, #16]
	cmp	r4, r3
	bge	.L43
	adds	r3, r4, #1
	str	r3, [r0, #16]
	movs	r3, #20
	mov	r5, r1
	mla	r4, r3, r4, r0
	adds	r4, r4, #28
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	movs	r0, #0
	ldr	r3, [r4]
	str	r3, [r5]
.L41:
	pop	{r4, r5, pc}
.L43:
	mov	r0, #-1
	b	.L41
	.fnend
	.size	h264e_marking_rd_op, .-h264e_marking_rd_op
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.9739, %object
	.size	__func__.9739, 20
__func__.9739:
	.ascii	"h264e_reorder_wr_op\000"
	.type	__func__.9760, %object
	.size	__func__.9760, 20
__func__.9760:
	.ascii	"h264e_marking_wr_op\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: write too many reorder op %d vs %d\012\000"
.LC1:
	.ascii	"\0013%s:%d: write too many mmco op %d vs %d\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
