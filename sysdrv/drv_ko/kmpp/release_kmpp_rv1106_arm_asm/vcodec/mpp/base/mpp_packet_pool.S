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
	.file	"mpp_packet_pool.c"
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
	.type	atomic_sub.constprop.0, %function
atomic_sub.constprop.0:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	pld	[r0]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r3, [r0]
	sub	r3, r3, #1
	strex	r2, r3, [r0]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	atomic_sub.constprop.0, .-atomic_sub.constprop.0
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_add.constprop.1, %function
atomic_add.constprop.1:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	pld	[r0]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r3, [r0]
	add	r3, r3, #1
	strex	r2, r3, [r0]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	atomic_add.constprop.1, .-atomic_add.constprop.1
	.align	1
	.global	mpp_packet_pool_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_pool_init, %function
mpp_packet_pool_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r2, .L4
	push	{r4, lr}
	.save {r4, lr}
	adds	r3, r2, #4
	movs	r4, #0
	str	r0, [r2]
	str	r4, [r2, #40]
	add	r1, r2, #16
	str	r4, [r2, #36]
	mov	r0, r3
	str	r4, [r2, #32]
	str	r1, [r2, #20]
	str	r1, [r2, #16]
	add	r1, r2, #24
	strd	r4, r4, [r2, #4]
	adds	r2, r2, #44
	str	r1, [r2, #-20]
	str	r1, [r2, #-16]
	str	r4, [r2, #-32]
	ldr	r1, .L4+4
	bl	__mutex_init
	mov	r0, r4
	pop	{r4, pc}
.L5:
	.align	2
.L4:
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	mpp_packet_pool_init, .-mpp_packet_pool_init
	.align	1
	.global	mpp_packet_mem_alloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_mem_alloc, %function
mpp_packet_mem_alloc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	ldr	r5, .L13
	adds	r0, r5, #4
	bl	mutex_lock
	ldr	r3, [r5, #40]
	cmp	r3, #0
	ble	.L7
	mov	r3, r5
	add	r0, r5, #40
	ldr	r4, [r3, #24]!
	cmp	r4, r3
	it	eq
	moveq	r4, #0
	bl	atomic_sub.constprop.0
	add	r0, r5, #36
	ldrd	r2, r3, [r4]
	str	r3, [r2, #4]
	str	r2, [r3]
	add	r2, r5, #16
	ldr	r3, [r5, #20]
	str	r4, [r5, #20]
	strd	r2, r3, [r4]
	str	r4, [r3]
	bl	atomic_add.constprop.1
	movs	r2, #112
	movs	r1, #0
	add	r0, r4, #8
	bl	memset
.L9:
	ldr	r0, .L13+4
	adds	r4, r4, #8
	bl	mutex_unlock
	b	.L6
.L7:
	ldr	r2, [r5, #32]
	ldr	r3, [r5]
	cmp	r2, r3
	bcc	.L10
	adds	r0, r5, #4
	movs	r4, #0
	bl	mutex_unlock
.L6:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L10:
	movs	r1, #120
	ldr	r0, .L13+8
	bl	mpp_osal_calloc
	mov	r4, r0
	cbnz	r0, .L12
	movs	r2, #81
	ldr	r1, .L13+12
	ldr	r0, .L13+16
	bl	printk
	adds	r0, r5, #4
	bl	mutex_unlock
	b	.L6
.L12:
	str	r0, [r4]
	add	r2, r5, #16
	str	r0, [r4, #4]
	ldr	r3, [r5, #20]
	str	r0, [r5, #20]
	strd	r2, r3, [r0]
	str	r0, [r3]
	add	r0, r5, #36
	bl	atomic_add.constprop.1
	add	r0, r5, #32
	bl	atomic_add.constprop.1
	b	.L9
.L14:
	.align	2
.L13:
	.word	.LANCHOR0
	.word	.LANCHOR0+4
	.word	.LANCHOR1
	.word	.LANCHOR1+21
	.word	.LC1
	.fnend
	.size	mpp_packet_mem_alloc, .-mpp_packet_mem_alloc
	.align	1
	.global	mpp_packet_mem_free
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_mem_free, %function
mpp_packet_mem_free:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	ldr	r4, .L20
	adds	r0, r4, #4
	bl	mutex_lock
	ldr	r1, [r4, #16]
	add	r2, r4, #16
	ldr	r3, [r1]
.L16:
	cmp	r1, r2
	beq	.L18
	add	r0, r1, #8
	cmp	r5, r0
	bne	.L17
	ldr	r0, .L20+4
	bl	atomic_sub.constprop.0
	ldr	r0, .L20+8
	ldrd	r2, r3, [r1]
	str	r3, [r2, #4]
	str	r2, [r3]
	ldr	r3, [r4, #28]
	str	r1, [r4, #28]
	strd	r0, r3, [r1]
	adds	r0, r0, #16
	str	r1, [r3]
	bl	atomic_add.constprop.1
.L18:
	ldr	r0, .L20+12
	bl	mutex_unlock
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L17:
	mov	r1, r3
	ldr	r3, [r3]
	b	.L16
.L21:
	.align	2
.L20:
	.word	.LANCHOR0
	.word	.LANCHOR0+36
	.word	.LANCHOR0+24
	.word	.LANCHOR0+4
	.fnend
	.size	mpp_packet_mem_free, .-mpp_packet_mem_free
	.align	1
	.global	mpp_packet_pool_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_pool_deinit, %function
mpp_packet_pool_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	ldr	r4, .L35
	ldr	r7, .L35+4
	adds	r0, r4, #4
	add	r6, r4, #24
	bl	mutex_lock
	ldr	r1, [r4, #24]
	add	r8, r4, #40
	ldr	r5, [r1]
.L23:
	cmp	r1, r6
	bne	.L25
	ldr	r1, [r4, #16]
	ldr	r4, .L35+8
	ldr	r5, [r1]
	add	r6, r4, #20
	add	r7, r4, #16
.L26:
	cmp	r1, r4
	bne	.L28
	ldr	r0, .L35+12
	bl	mutex_unlock
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
.L25:
	ldrd	r2, r3, [r1]
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r1, [r1]
	str	r1, [r1, #4]
	cbz	r1, .L24
	mov	r0, r7
	bl	mpp_osal_free
.L24:
	mov	r0, r8
	bl	atomic_sub.constprop.0
	ldr	r0, .L35+16
	bl	atomic_sub.constprop.0
	mov	r1, r5
	ldr	r5, [r5]
	b	.L23
.L28:
	ldrd	r2, r3, [r1]
	mov	r0, r6
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r1, [r1]
	str	r1, [r1, #4]
	bl	atomic_sub.constprop.0
	mov	r0, r7
	bl	atomic_sub.constprop.0
	cbz	r1, .L27
	ldr	r0, .L35+4
	bl	mpp_osal_free
.L27:
	mov	r1, r5
	ldr	r5, [r5]
	b	.L26
.L36:
	.align	2
.L35:
	.word	.LANCHOR0
	.word	.LANCHOR1+42
	.word	.LANCHOR0+16
	.word	.LANCHOR0+4
	.word	.LANCHOR0+32
	.fnend
	.size	mpp_packet_pool_deinit, .-mpp_packet_pool_deinit
	.align	1
	.global	mpp_packet_pool_proc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_pool_proc, %function
mpp_packet_pool_proc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r0
	ldr	r1, .L38
	bl	seq_puts
	ldr	r3, .L38+4
	mov	r0, r4
	ldr	r2, .L38+8
	ldr	r1, .L38+12
	str	r3, [sp]
	ldr	r3, .L38+16
	bl	seq_printf
	ldr	r1, .L38+20
	mov	r0, r4
	ldr	r2, [r1, #40]
	ldr	r3, [r1, #36]
	ldr	r1, [r1, #32]
	str	r1, [sp]
	ldr	r1, .L38+24
	bl	seq_printf
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L39:
	.align	2
.L38:
	.word	.LC2
	.word	.LC6
	.word	.LC4
	.word	.LC5
	.word	.LC3
	.word	.LANCHOR0
	.word	.LC7
	.fnend
	.size	mpp_packet_pool_proc, .-mpp_packet_pool_proc
	.section	.rodata
	.set	.LANCHOR1,. + 0
	.type	__FUNCTION__.29425, %object
	.size	__FUNCTION__.29425, 21
__FUNCTION__.29425:
	.ascii	"mpp_packet_mem_alloc\000"
	.type	__func__.29426, %object
	.size	__func__.29426, 21
__func__.29426:
	.ascii	"mpp_packet_mem_alloc\000"
	.type	__FUNCTION__.29472, %object
	.size	__FUNCTION__.29472, 23
__FUNCTION__.29472:
	.ascii	"mpp_packet_pool_deinit\000"
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	mpp_packet_pool, %object
	.size	mpp_packet_pool, 44
mpp_packet_pool:
	.space	44
	.type	__key.29399, %object
	.size	__key.29399, 0
__key.29399:
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"&ctx->mem_lock\000"
.LC1:
	.ascii	"\0013%s:%d: malloc failed\012\000"
.LC2:
	.ascii	"\012--------packet_pool----------------------------"
	.ascii	"-----------------------------------------------\012"
	.ascii	"\000"
.LC3:
	.ascii	"used_cnt\000"
.LC4:
	.ascii	"unused_cnt\000"
.LC5:
	.ascii	"%12s%12s%12s\012\000"
.LC6:
	.ascii	"total_cnt\000"
.LC7:
	.ascii	"%12u%12u%12u\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
