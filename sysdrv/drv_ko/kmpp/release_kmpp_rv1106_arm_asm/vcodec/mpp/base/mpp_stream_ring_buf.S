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
	.file	"mpp_stream_ring_buf.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	ring_buf_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ring_buf_init, %function
ring_buf_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r7, r1
	mov	r6, r2
	mov	r4, r0
	cbz	r0, .L5
	ldr	r5, [r0, #28]
	cbnz	r5, .L5
	cmp	r2, #0
	strd	r5, r5, [r0]
	ldr	r1, .L7
	mov	r0, r7
	it	eq
	moveq	r6, #4
	bl	mpp_buffer_get_size_with_caller
	ldr	r1, .L7
	strd	r0, r5, [r4, #8]
	mov	r0, r7
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L7
	strd	r0, r7, [r4, #16]
	mov	r0, r7
	bl	mpp_buffer_get_mpi_buf_id_with_caller
	movs	r2, #1
	mov	r3, #10240
	strd	r2, r3, [r4, #28]
	ldr	r2, .L7+4
	str	r0, [r4, #24]
	ldr	r0, [r2]
	ands	r0, r0, #1
	beq	.L1
	strd	r6, r3, [sp]
	movs	r2, #44
	ldr	r3, [r4, #8]
	ldr	r1, .L7+8
	ldr	r0, .L7+12
	bl	printk
	mov	r0, r5
.L1:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L5:
	mov	r0, #-1
	b	.L1
.L8:
	.align	2
.L7:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	.LANCHOR0+14
	.word	.LC0
	.fnend
	.size	ring_buf_init, .-ring_buf_init
	.align	1
	.global	ring_buf_get_use_size
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ring_buf_get_use_size, %function
ring_buf_get_use_size:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbz	r0, .L9
	ldrd	r2, r3, [r0]
	cmp	r2, r3
	bls	.L11
	ldr	r0, [r0, #8]
	add	r0, r0, r3
	subs	r0, r0, r2
	bx	lr
.L11:
	subs	r0, r3, r2
.L9:
	bx	lr
	.fnend
	.size	ring_buf_get_use_size, .-ring_buf_get_use_size
	.global	__aeabi_uidivmod
	.align	1
	.global	ring_buf_put_use
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ring_buf_put_use, %function
ring_buf_put_use:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r10, r1
	.pad #20
	sub	sp, sp, #20
	mov	r4, r0
	cmp	r0, #0
	beq	.L28
	cmp	r1, #0
	beq	.L28
	ldr	r2, [r0, #20]
	ldr	r3, [r1]
	cmp	r2, r3
	bne	.L28
	ldr	fp, [r1, #20]
	cmp	fp, #0
	beq	.L28
	ldr	r8, [r1, #12]
	ldrd	r6, r7, [r0]
	ldr	r1, [r0, #8]
	add	r0, fp, r8
	bl	__aeabi_uidivmod
	cmp	r7, r6
	mov	r5, r1
	sub	r9, r1, r6
	bcc	.L15
	cmp	r7, r8
	bls	.L16
	cmp	r6, r8
	bcc	.L17
.L16:
	cmp	r6, r5
	bls	.L18
.L23:
	ldr	r3, [r4, #8]
	add	r3, r3, r9
.L19:
	ldr	r2, [r4, #12]
	str	r5, [r4, #4]
	cmp	r2, r3
	it	cc
	strcc	r3, [r4, #12]
	ldr	r3, .L31
	str	r5, [r4, #40]
	ldr	r0, [r3]
	ands	r0, r0, #1
	beq	.L13
	str	r5, [sp, #4]
	movs	r2, #97
	ldr	r3, [r4]
	ldr	r1, .L31+4
	ldr	r0, .L31+8
	str	r3, [sp]
	mov	r3, r4
	bl	printk
	movs	r0, #0
.L13:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L18:
	cmp	r7, r5
	bls	.L20
.L17:
	ldr	r3, [r10, #24]
	mov	r2, r6
	movs	r1, #75
	str	r8, [sp]
	strd	fp, r3, [sp, #4]
	mov	r3, r7
.L30:
	ldr	r0, .L31+12
	bl	printk
	cmp	r6, r5
	bhi	.L23
.L20:
	mov	r3, r9
	b	.L19
.L15:
	cmp	r6, r8
	bls	.L22
	cmp	r7, r8
	bhi	.L22
	cmp	r7, r1
	bhi	.L22
	cmp	r6, r1
	bhi	.L23
.L22:
	ldr	r3, [r10, #24]
	mov	r2, r6
	str	r8, [sp]
	movs	r1, #82
	strd	fp, r3, [sp, #4]
	mov	r3, r7
	b	.L30
.L28:
	mov	r0, #-1
	b	.L13
.L32:
	.align	2
.L31:
	.word	.LANCHOR1
	.word	.LANCHOR0+28
	.word	.LC2
	.word	.LC1
	.fnend
	.size	ring_buf_put_use, .-ring_buf_put_use
	.align	1
	.global	ring_buf_put_free
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ring_buf_put_free, %function
ring_buf_put_free:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r4, r0
	cmp	r0, #0
	beq	.L43
	cmp	r1, #0
	beq	.L43
	ldr	r2, [r0, #20]
	ldr	r3, [r1]
	cmp	r2, r3
	bne	.L43
	ldr	r9, [r1, #20]
	cmp	r9, #0
	beq	.L43
	ldr	r8, [r1, #12]
	ldrd	r6, r7, [r0]
	ldr	r1, [r0, #8]
	add	r0, r9, r8
	bl	__aeabi_uidivmod
	cmp	r7, r6
	mov	r5, r1
	bhi	.L35
	cmp	r6, r1
	bls	.L36
	cmp	r7, r1
	bcc	.L37
.L36:
	cmp	r7, r8
	bhi	.L38
	cmp	r6, r8
	bls	.L38
.L37:
	mov	r3, r7
	mov	r2, r6
	movs	r1, #117
	strd	r8, r9, [sp]
.L45:
	ldr	r0, .L46
	bl	printk
.L38:
	ldr	r3, .L46+4
	str	r5, [r4]
	str	r5, [r4, #36]
	ldr	r0, [r3]
	ands	r0, r0, #1
	beq	.L33
	ldr	r3, [r4, #4]
	movs	r2, #129
	ldr	r1, .L46+8
	ldr	r0, .L46+12
	strd	r5, r3, [sp]
	mov	r3, r4
	bl	printk
	movs	r0, #0
.L33:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L35:
	cmp	r6, r8
	bhi	.L39
	cmp	r7, r8
	bls	.L39
	cmp	r6, r1
	bhi	.L39
	cmp	r7, r1
	bcs	.L38
.L39:
	strd	r8, r9, [sp]
	mov	r3, r7
	mov	r2, r6
	movs	r1, #123
	b	.L45
.L43:
	mov	r0, #-1
	b	.L33
.L47:
	.align	2
.L46:
	.word	.LC3
	.word	.LANCHOR1
	.word	.LANCHOR0+45
	.word	.LC4
	.fnend
	.size	ring_buf_put_free, .-ring_buf_put_free
	.align	1
	.global	ring_buf_get_free
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ring_buf_get_free, %function
ring_buf_get_free:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r5, r1
	mov	r8, r2
	mov	r4, r0
	cbnz	r0, .L49
.L51:
	mov	r0, #-1
.L48:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L49:
	cmp	r1, #0
	beq	.L51
	ldr	r2, [r0, #32]
	ldrd	r7, r6, [r0]
	cmp	r3, r2
	it	cc
	movcc	r3, r2
	mov	r9, r3
	ldr	r3, [r0, #24]
	str	r3, [r1, #4]
	movs	r3, #1
	str	r3, [r1, #28]
	ldr	r3, .L61
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L52
	ldr	r3, [r0, #4]
	movs	r2, #149
	ldr	r1, .L61+4
	str	r3, [sp, #4]
	ldr	r3, [r0]
	str	r3, [sp]
	mov	r3, r0
	ldr	r0, .L61+8
	bl	printk
.L52:
	add	r0, r8, #-1
	cmp	r6, r7
	and	r0, r0, r6
	rsb	r1, r8, #0
	sub	r2, r8, r0
	and	r1, r1, r7
	bcs	.L53
	add	r9, r9, r6
	add	r9, r9, r2
	cmp	r9, r7
	bcs	.L51
	add	r2, r2, r6
	str	r2, [r5, #12]
	ldr	r3, [r4, #16]
	sub	r7, r7, r8
	str	r1, [r5, #16]
	add	r7, r7, r0
	add	r2, r2, r3
	str	r2, [r5, #8]
	ldr	r3, [r4, #20]
	subs	r6, r7, r6
	str	r6, [r5, #24]
	str	r3, [r5]
.L60:
	movs	r0, #0
	b	.L48
.L53:
	ldr	r3, [r4, #8]
	add	r6, r6, r2
	add	r3, r3, r7
	subs	r3, r3, r6
	cmp	r9, r3
	bcs	.L51
	str	r6, [r5, #12]
	ldr	r3, [r4, #16]
	add	r3, r3, r6
	str	r3, [r5, #8]
	ldr	r3, [r4, #8]
	str	r1, [r5, #16]
	add	r7, r7, r3
	subs	r6, r7, r6
	str	r6, [r5, #24]
	ldr	r3, [r4, #20]
	str	r3, [r5]
	b	.L60
.L62:
	.align	2
.L61:
	.word	.LANCHOR1
	.word	.LANCHOR0+63
	.word	.LC5
	.fnend
	.size	ring_buf_get_free, .-ring_buf_get_free
	.global	ring_buf_debug
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.19853, %object
	.size	__FUNCTION__.19853, 14
__FUNCTION__.19853:
	.ascii	"ring_buf_init\000"
	.type	__func__.19854, %object
	.size	__func__.19854, 14
__func__.19854:
	.ascii	"ring_buf_init\000"
	.type	__func__.19867, %object
	.size	__func__.19867, 17
__func__.19867:
	.ascii	"ring_buf_put_use\000"
	.type	__func__.19876, %object
	.size	__func__.19876, 18
__func__.19876:
	.ascii	"ring_buf_put_free\000"
	.type	__func__.19889, %object
	.size	__func__.19889, 18
__func__.19889:
	.ascii	"ring_buf_get_free\000"
	.type	__param_str_ring_buf_debug, %object
	.size	__param_str_ring_buf_debug, 26
__param_str_ring_buf_debug:
	.ascii	"mpp_vcodec.ring_buf_debug\000"
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	ring_buf_debug, %object
	.size	ring_buf_debug, 4
ring_buf_debug:
	.space	4
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_ring_buf_debug163, %object
	.size	__UNIQUE_ID_ring_buf_debug163, 63
__UNIQUE_ID_ring_buf_debug163:
	.ascii	"mpp_vcodec.parm=ring_buf_debug:bits ring_buf debug "
	.ascii	"information\000"
	.type	__UNIQUE_ID_ring_buf_debugtype162, %object
	.size	__UNIQUE_ID_ring_buf_debugtype162, 40
__UNIQUE_ID_ring_buf_debugtype162:
	.ascii	"mpp_vcodec.parmtype=ring_buf_debug:uint\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: ctx->len = %d, max_strm_cnt = %d, ctx->"
	.ascii	"min_buf_size = %d\000"
.LC1:
	.ascii	"\0013%d: INVALID param: r_pos=%x, w_pos=%x, start=%"
	.ascii	"x, size=%x, \011\011\011    buf_len = %x\012\000"
.LC2:
	.ascii	"\0016%s:%d:  pool %p use update ctx->r_pos %d ctx->"
	.ascii	"w_pos %d\012\000"
.LC3:
	.ascii	"\0013%d: INVALID param: r_pos=%x, w_pos=%x, start=%"
	.ascii	"x, size=%x\012\000"
.LC4:
	.ascii	"\0016%s:%d:  pool %p free update ctx->r_pos %d ctx-"
	.ascii	">w_pos %d\012\000"
.LC5:
	.ascii	"\0016%s:%d: get free pool %p ctx->r_pos %d ctx->w_p"
	.ascii	"os %d\012\000"
	.section	__param,"a"
	.align	2
	.type	__param_ring_buf_debug, %object
	.size	__param_ring_buf_debug, 20
__param_ring_buf_debug:
	.word	__param_str_ring_buf_debug
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	ring_buf_debug
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
