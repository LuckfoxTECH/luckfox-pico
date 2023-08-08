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
	.file	"hal_bufs.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_bufs_clear, %function
hal_bufs_clear:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r4, r0
	ldr	r5, [r0, #20]
	cbz	r5, .L2
	ldr	r5, [r0, #12]
	cmp	r5, #0
	bne	.L11
.L2:
	movs	r6, #0
	movs	r2, #32
	mov	r1, r6
	strd	r6, r6, [r4, #4]
	str	r6, [r4, #12]
	add	r0, r4, #24
	str	r6, [r4, #20]
	bl	memset
	ldr	r1, [r4, #56]
	cbz	r1, .L9
	ldr	r0, .L24
	bl	mpp_osal_free
.L9:
	mov	r0, r5
	str	r6, [r4, #56]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L8:
	mov	r8, #1
	lsl	r8, r8, r6
	tst	r8, r3
	beq	.L4
	ldr	r2, [r4, #16]
	movs	r7, #0
	ldr	r3, [r4, #56]
	mla	r9, r2, r6, r3
.L5:
	ldr	r3, [r4, #8]
	cmp	r3, r7
	bgt	.L7
	ldr	r3, [r4, #20]
	bic	r3, r3, r8
	str	r3, [r4, #20]
.L4:
	adds	r6, r6, #1
.L3:
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #20]
	cmp	r2, r6
	bgt	.L8
	cmp	r3, #0
	beq	.L2
	movs	r1, #80
	ldr	r3, .L24
	str	r1, [sp]
	ldr	r2, .L24+4
	ldr	r0, .L24+8
	bl	printk
	b	.L2
.L7:
	ldr	r3, [r9, #4]
	ldr	r0, [r3, r7, lsl #2]
	cbz	r0, .L6
	mov	r1, r10
	bl	mpp_buffer_put_with_caller
	ldr	r3, [r9, #4]
	str	fp, [r3, r7, lsl #2]
	orrs	r5, r5, r0
.L6:
	adds	r7, r7, #1
	b	.L5
.L11:
	movs	r6, #0
	ldr	r10, .L24
	mov	r5, r6
	mov	fp, r6
	b	.L3
.L25:
	.align	2
.L24:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.fnend
	.size	hal_bufs_clear, .-hal_bufs_clear
	.align	1
	.global	hal_bufs_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_bufs_init, %function
hal_bufs_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	cbnz	r0, .L27
	movs	r2, #99
	ldr	r1, .L31
	ldr	r0, .L31+4
	bl	printk
	mvn	r0, #2
.L26:
	pop	{r3, r4, r5, pc}
.L27:
	movs	r1, #60
	ldr	r0, .L31+8
	bl	mpp_osal_calloc
	mov	r5, r0
	cbnz	r0, .L30
	movs	r2, #110
	ldr	r1, .L31
	ldr	r0, .L31+12
	bl	printk
	mvn	r0, #3
.L29:
	str	r5, [r4]
	b	.L26
.L30:
	movs	r0, #0
	b	.L29
.L32:
	.align	2
.L31:
	.word	.LANCHOR0+15
	.word	.LC2
	.word	.LANCHOR0+29
	.word	.LC3
	.fnend
	.size	hal_bufs_init, .-hal_bufs_init
	.align	1
	.global	hal_bufs_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_bufs_deinit, %function
hal_bufs_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	cbnz	r0, .L34
	movs	r2, #127
	ldr	r1, .L36
	ldr	r0, .L36+4
	mvn	r4, #2
	bl	printk
.L33:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L34:
	bl	hal_bufs_clear
	movs	r2, #60
	movs	r1, #0
	mov	r4, r0
	mov	r0, r5
	bl	memset
	mov	r1, r5
	ldr	r0, .L36+8
	bl	mpp_osal_free
	b	.L33
.L37:
	.align	2
.L36:
	.word	.LANCHOR0+43
	.word	.LC2
	.word	.LANCHOR0+59
	.fnend
	.size	hal_bufs_deinit, .-hal_bufs_deinit
	.align	1
	.global	hal_bufs_setup
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_bufs_setup, %function
hal_bufs_setup:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r6, r1
	mov	r5, r2
	mov	r7, r3
	mov	r4, r0
	cbz	r0, .L39
	cbnz	r3, .L40
.L39:
	str	r7, [sp]
	mov	r3, r4
	movs	r2, #157
	ldr	r1, .L52
	ldr	r0, .L52+4
	bl	printk
	mvn	r0, #2
.L38:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L40:
	subs	r3, r1, #1
	cmp	r3, #39
	bhi	.L42
	subs	r3, r2, #1
	cmp	r3, #7
	bls	.L43
.L42:
	str	r5, [sp]
	mov	r3, r6
	movs	r2, #164
	ldr	r1, .L52
	ldr	r0, .L52+8
	bl	printk
	mvn	r0, #5
	b	.L38
.L43:
	bl	hal_bufs_clear
	adds	r2, r5, #2
	ldr	r0, .L52+12
	lsls	r2, r2, #2
	str	r2, [r4, #16]
	mul	r8, r2, r6
	mov	r1, r8
	bl	mpp_osal_calloc
	str	r0, [r4, #56]
	cbz	r0, .L44
	movs	r2, #0
	add	r3, r4, #24
	mov	r1, r2
.L45:
	ldr	r0, [r7, r2, lsl #2]
	adds	r2, r2, #1
	cmp	r5, r2
	add	r1, r1, r0
	str	r0, [r3], #4
	bgt	.L45
	movs	r3, #0
	str	r1, [r4, #12]
.L46:
	cmp	r3, r6
	blt	.L47
	strd	r6, r5, [r4, #4]
	movs	r0, #0
	b	.L38
.L47:
	ldr	r2, [r4, #16]
	ldr	r0, [r4, #56]
	muls	r2, r3, r2
	adds	r3, r3, #1
	adds	r1, r0, r2
	str	r5, [r0, r2]
	add	r2, r1, #8
	str	r2, [r1, #4]
	b	.L46
.L44:
	mov	r3, r8
	movs	r2, #205
	ldr	r1, .L52
	ldr	r0, .L52+16
	bl	printk
	mvn	r0, #3
	b	.L38
.L53:
	.align	2
.L52:
	.word	.LANCHOR0+75
	.word	.LC4
	.word	.LC5
	.word	.LANCHOR0+90
	.word	.LC6
	.fnend
	.size	hal_bufs_setup, .-hal_bufs_setup
	.align	1
	.global	hal_bufs_get_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_bufs_get_buf, %function
hal_bufs_get_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	.pad #20
	sub	sp, sp, #20
	cbz	r0, .L55
	cmp	r1, #0
	blt	.L55
	ldr	r3, [r0, #4]
	cmp	r3, r1
	bgt	.L56
.L55:
	str	r1, [sp]
	mov	r3, r4
	movs	r2, #222
	ldr	r1, .L70
	ldr	r0, .L70+4
	bl	printk
.L69:
	movs	r6, #0
.L54:
	mov	r0, r6
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L56:
	movs	r7, #1
	ldr	r5, [r0, #20]
	ldr	r6, [r0, #16]
	lsls	r7, r7, r1
	ldr	r3, [r0, #56]
	ands	r5, r7, r5
	mla	r6, r6, r1, r3
	bne	.L54
	ldr	r10, .L70+20
	add	r9, r0, #24
.L58:
	ldr	r3, [r4, #8]
	cmp	r3, r5
	bgt	.L62
	ldr	r1, [r4, #20]
	orrs	r1, r1, r7
	str	r1, [r4, #20]
	b	.L54
.L62:
	ldr	r3, [r6, #4]
	lsl	fp, r5, #2
	ldr	r2, [r9], #4
	ldr	r8, [r3, r5, lsl #2]
	str	r8, [sp, #12]
	cbz	r2, .L59
	cmp	r8, #0
	bne	.L59
	str	r10, [sp]
	add	r1, sp, #12
	ldr	r3, .L70+8
	mov	r0, r8
	bl	mpp_buffer_get_with_tag
	cmp	r0, #0
	bne	.L69
	ldr	r3, [sp, #12]
	cbnz	r3, .L61
	movs	r1, #243
	mov	r3, r10
	str	r1, [sp]
	ldr	r2, .L70+12
	ldr	r0, .L70+16
	bl	printk
.L61:
	ldr	r3, [r6, #4]
	ldr	r2, [sp, #12]
	str	r2, [r3, fp]
.L59:
	adds	r5, r5, #1
	b	.L58
.L71:
	.align	2
.L70:
	.word	.LANCHOR0+105
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC1
	.word	.LANCHOR0+122
	.fnend
	.size	hal_bufs_get_buf, .-hal_bufs_get_buf
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.7709, %object
	.size	__FUNCTION__.7709, 15
__FUNCTION__.7709:
	.ascii	"hal_bufs_clear\000"
	.type	__func__.7723, %object
	.size	__func__.7723, 14
__func__.7723:
	.ascii	"hal_bufs_init\000"
	.type	__FUNCTION__.7724, %object
	.size	__FUNCTION__.7724, 14
__FUNCTION__.7724:
	.ascii	"hal_bufs_init\000"
	.type	__func__.7730, %object
	.size	__func__.7730, 16
__func__.7730:
	.ascii	"hal_bufs_deinit\000"
	.type	__FUNCTION__.7731, %object
	.size	__FUNCTION__.7731, 16
__FUNCTION__.7731:
	.ascii	"hal_bufs_deinit\000"
	.type	__func__.7742, %object
	.size	__func__.7742, 15
__func__.7742:
	.ascii	"hal_bufs_setup\000"
	.type	__FUNCTION__.7743, %object
	.size	__FUNCTION__.7743, 15
__FUNCTION__.7743:
	.ascii	"hal_bufs_setup\000"
	.type	__func__.7761, %object
	.size	__func__.7761, 17
__func__.7761:
	.ascii	"hal_bufs_get_buf\000"
	.type	__FUNCTION__.7765, %object
	.size	__FUNCTION__.7765, 17
__FUNCTION__.7765:
	.ascii	"hal_bufs_get_buf\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"impl->valid == 0\000"
.LC1:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC2:
	.ascii	"\0013%s:%d: invalid NULL input\012\000"
.LC3:
	.ascii	"\0013%s:%d: failed to malloc HalBufs\012\000"
.LC4:
	.ascii	"\0013%s:%d: invalid NULL input bufs %p sizes %p\012"
	.ascii	"\000"
.LC5:
	.ascii	"\0013%s:%d: invalid max cnt %d size cnt %d\012\000"
.LC6:
	.ascii	"\0013%s:%d: failed to malloc size %d for impl\012\000"
.LC7:
	.ascii	"\0013%s:%d: invalid input impl %p buf_idx %d\012\000"
.LC8:
	.ascii	"hal_bufs\000"
.LC9:
	.ascii	"buf\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
