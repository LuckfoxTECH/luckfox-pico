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
	.file	"h265e_stream.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	h265e_stream_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_init, %function
h265e_stream_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r1, #1536
	mov	r4, r0
	ldr	r0, .L2
	bl	mpp_osal_calloc
	mov	r2, #1536
	str	r2, [r4, #4]
	mov	r1, r0
	str	r0, [r4]
	add	r0, r4, #8
	bl	mpp_writer_init
	movs	r0, #0
	pop	{r4, pc}
.L3:
	.align	2
.L2:
	.word	.LANCHOR0
	.fnend
	.size	h265e_stream_init, .-h265e_stream_init
	.align	1
	.global	h265e_stream_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_deinit, %function
h265e_stream_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r1, [r0]
	cbz	r1, .L5
	ldr	r0, .L9
	bl	mpp_osal_free
.L5:
	mov	r0, r4
	movs	r5, #0
	str	r5, [r0], #8
	bl	mpp_writer_reset
	mov	r0, r5
	pop	{r3, r4, r5, pc}
.L10:
	.align	2
.L9:
	.word	.LANCHOR0+18
	.fnend
	.size	h265e_stream_deinit, .-h265e_stream_deinit
	.align	1
	.global	h265e_stream_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_reset, %function
h265e_stream_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	adds	r0, r0, #8
	bl	mpp_writer_reset
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	h265e_stream_reset, .-h265e_stream_reset
	.align	1
	.global	h265e_stream_realign
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_realign, %function
h265e_stream_realign:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r3, [r0, #28]
	cbz	r3, .L13
	adds	r0, r0, #8
	bl	mpp_writer_trailing
.L13:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	h265e_stream_realign, .-h265e_stream_realign
	.align	1
	.global	h265e_stream_write_with_log
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_write_with_log, %function
h265e_stream_write_with_log:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r2
	ldr	r2, .L22
	mov	r6, r0
	mov	r4, r1
	ldr	r2, [r2]
	lsls	r2, r2, #22
	bpl	.L18
	strd	r5, r1, [sp]
	movs	r2, #69
	ldr	r1, .L22+4
	ldr	r0, .L22+8
	bl	printk
.L18:
	add	r0, r6, #8
	mov	r2, r5
	mov	r1, r4
	bl	mpp_writer_put_bits
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L23:
	.align	2
.L22:
	.word	h265e_debug
	.word	.LANCHOR0+38
	.word	.LC0
	.fnend
	.size	h265e_stream_write_with_log, .-h265e_stream_write_with_log
	.align	1
	.global	h265e_stream_write1_with_log
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_write1_with_log, %function
h265e_stream_write1_with_log:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r3, r2
	ldr	r2, .L29
	mov	r5, r0
	mov	r4, r1
	ldr	r2, [r2]
	lsls	r2, r2, #22
	bpl	.L25
	str	r1, [sp]
	movs	r2, #77
	ldr	r1, .L29+4
	ldr	r0, .L29+8
	bl	printk
.L25:
	add	r0, r5, #8
	movs	r2, #1
	mov	r1, r4
	bl	mpp_writer_put_bits
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L30:
	.align	2
.L29:
	.word	h265e_debug
	.word	.LANCHOR0+66
	.word	.LC1
	.fnend
	.size	h265e_stream_write1_with_log, .-h265e_stream_write1_with_log
	.align	1
	.global	h265e_stream_write_ue_with_log
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_write_ue_with_log, %function
h265e_stream_write_ue_with_log:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r7, r2
	ldr	r5, .L43
	mov	r8, r0
	mov	r4, r1
	ldr	r2, [r5]
	lsls	r2, r2, #22
	bpl	.L32
	str	r1, [sp]
	mov	r3, r7
	movs	r2, #88
	ldr	r1, .L43+4
	ldr	r0, .L43+8
	bl	printk
.L32:
	ldr	r3, [r5]
	adds	r4, r4, #1
	lsls	r3, r3, #22
	mov	r6, r4
	bpl	.L33
	str	r4, [sp]
	mov	r3, r7
	movs	r2, #95
	ldr	r1, .L43+4
	ldr	r0, .L43+8
	bl	printk
.L33:
	cmp	r4, #65536
	ldr	r2, .L43+12
	it	ge
	asrge	r6, r4, #16
	mov	r1, #0
	ite	ge
	movge	r3, #32
	movlt	r3, #0
	cmp	r6, #255
	mov	r0, r8
	itt	gt
	asrgt	r6, r6, #8
	addgt	r3, r3, #16
	add	r6, r6, r2
	ldrb	r5, [r6, #126]	@ zero_extendqisi2
	add	r5, r5, r3
	mov	r3, r7
	asrs	r5, r5, #1
	mov	r2, r5
	bl	h265e_stream_write_with_log
	mov	r3, r7
	adds	r2, r5, #1
	mov	r1, r4
	mov	r0, r8
	bl	h265e_stream_write_with_log
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L44:
	.align	2
.L43:
	.word	h265e_debug
	.word	.LANCHOR0+95
	.word	.LC2
	.word	.LANCHOR0
	.fnend
	.size	h265e_stream_write_ue_with_log, .-h265e_stream_write_ue_with_log
	.align	1
	.global	h265e_stream_write_se_with_log
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_write_se_with_log, %function
h265e_stream_write_se_with_log:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mvn	r4, #1
	muls	r4, r1, r4
	mov	r6, r2
	mov	r7, r0
	adds	r4, r4, #1
	it	mi
	lslmi	r4, r1, #1
	ldr	r1, .L53
	cmp	r4, #255
	ite	gt
	asrgt	r3, r4, #8
	movle	r3, r4
	add	r3, r3, r1
	ite	gt
	movgt	r2, #16
	movle	r2, #0
	ldrb	r5, [r3, #126]	@ zero_extendqisi2
	ldr	r3, .L53+4
	add	r5, r5, r2
	ldr	r3, [r3]
	lsls	r3, r3, #22
	bpl	.L48
	str	r4, [sp]
	mov	r3, r6
	movs	r2, #130
	add	r1, r1, #382
	ldr	r0, .L53+8
	bl	printk
.L48:
	mov	r3, r6
	mov	r2, r5
	mov	r1, r4
	mov	r0, r7
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	b	h265e_stream_write_with_log
.L54:
	.align	2
.L53:
	.word	.LANCHOR0
	.word	h265e_debug
	.word	.LC3
	.fnend
	.size	h265e_stream_write_se_with_log, .-h265e_stream_write_se_with_log
	.align	1
	.global	h265e_stream_write32
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_write32, %function
h265e_stream_write32:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r3, r2
	mov	r6, r0
	mov	r4, r1
	mov	r5, r2
	movs	r2, #16
	lsrs	r1, r1, r2
	bl	h265e_stream_write_with_log
	mov	r3, r5
	movs	r2, #16
	mov	r1, r4
	mov	r0, r6
	bl	h265e_stream_write_with_log
	movs	r0, #0
	pop	{r4, r5, r6, pc}
	.fnend
	.size	h265e_stream_write32, .-h265e_stream_write32
	.align	1
	.global	h265e_stream_size_se
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_size_se, %function
h265e_stream_size_se:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mvn	r3, #1
	ldr	r2, .L60
	muls	r3, r0, r3
	adds	r3, r3, #1
	it	mi
	lslmi	r3, r0, #1
	cmp	r3, #255
	itete	le
	addle	r3, r3, r2
	addgt	r3, r2, r3, asr #8
	ldrble	r0, [r3, #126]	@ zero_extendqisi2
	ldrbgt	r0, [r3, #126]	@ zero_extendqisi2
	it	gt
	addgt	r0, r0, #16
	bx	lr
.L61:
	.align	2
.L60:
	.word	.LANCHOR0
	.fnend
	.size	h265e_stream_size_se, .-h265e_stream_size_se
	.align	1
	.global	h265e_stream_rbsp_trailing
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_rbsp_trailing, %function
h265e_stream_rbsp_trailing:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	adds	r0, r0, #8
	bl	mpp_writer_trailing
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	h265e_stream_rbsp_trailing, .-h265e_stream_rbsp_trailing
	.align	1
	.global	h265e_stream_flush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_stream_flush, %function
h265e_stream_flush:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	h265e_stream_realign
	.fnend
	.size	h265e_stream_flush, .-h265e_stream_flush
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.9282, %object
	.size	__FUNCTION__.9282, 18
__FUNCTION__.9282:
	.ascii	"h265e_stream_init\000"
	.type	__FUNCTION__.9286, %object
	.size	__FUNCTION__.9286, 20
__FUNCTION__.9286:
	.ascii	"h265e_stream_deinit\000"
	.type	__func__.9299, %object
	.size	__func__.9299, 28
__func__.9299:
	.ascii	"h265e_stream_write_with_log\000"
	.type	__func__.9305, %object
	.size	__func__.9305, 29
__func__.9305:
	.ascii	"h265e_stream_write1_with_log\000"
	.type	__func__.9311, %object
	.size	__func__.9311, 31
__func__.9311:
	.ascii	"h265e_stream_write_ue_with_log\000"
	.type	ue_size_tab, %object
	.size	ue_size_tab, 256
ue_size_tab:
	.byte	1
	.byte	1
	.byte	3
	.byte	3
	.byte	5
	.byte	5
	.byte	5
	.byte	5
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	7
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	9
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	11
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	13
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.byte	15
	.type	__func__.9321, %object
	.size	__func__.9321, 31
__func__.9321:
	.ascii	"h265e_stream_write_se_with_log\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: write bits name %s, count %d, val %d\000"
.LC1:
	.ascii	"\0016%s:%d: write 1 bit name %s, val %d\000"
.LC2:
	.ascii	"\0016%s:%d: write UE bits name %s, val %d \000"
.LC3:
	.ascii	"\0016%s:%d: write SE bits name %s, val %d \000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
