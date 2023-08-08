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
	.file	"hal_jpege_api_v2.c"
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
	.type	hal_jpege_prepare, %function
hal_jpege_prepare:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L1
	ldr	r3, [r0]
	cbz	r3, .L1
	ldr	r3, [r3, #24]
	cbz	r3, .L1
	mov	r0, r2
	bx	r3
.L1:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_jpege_prepare, .-hal_jpege_prepare
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_get_task, %function
hal_jpege_get_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L9
	ldr	r3, [r0]
	cbz	r3, .L9
	ldr	r3, [r3, #28]
	cbz	r3, .L9
	mov	r0, r2
	bx	r3
.L9:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_jpege_get_task, .-hal_jpege_get_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_gen_regs, %function
hal_jpege_gen_regs:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L17
	ldr	r3, [r0]
	cbz	r3, .L17
	ldr	r3, [r3, #32]
	cbz	r3, .L17
	mov	r0, r2
	bx	r3
.L17:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_jpege_gen_regs, .-hal_jpege_gen_regs
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_start, %function
hal_jpege_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L25
	ldr	r3, [r0]
	cbz	r3, .L25
	ldr	r3, [r3, #36]
	cbz	r3, .L25
	mov	r0, r2
	bx	r3
.L25:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_jpege_start, .-hal_jpege_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_wait, %function
hal_jpege_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L33
	ldr	r3, [r0]
	cbz	r3, .L33
	ldr	r3, [r3, #40]
	cbz	r3, .L33
	mov	r0, r2
	bx	r3
.L33:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_jpege_wait, .-hal_jpege_wait
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_part_start, %function
hal_jpege_part_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L41
	ldr	r3, [r0]
	cbz	r3, .L41
	ldr	r3, [r3, #44]
	cbz	r3, .L41
	mov	r0, r2
	bx	r3
.L41:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_jpege_part_start, .-hal_jpege_part_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_part_wait, %function
hal_jpege_part_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L49
	ldr	r3, [r0]
	cbz	r3, .L49
	ldr	r3, [r3, #48]
	cbz	r3, .L49
	mov	r0, r2
	bx	r3
.L49:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_jpege_part_wait, .-hal_jpege_part_wait
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_ret_task, %function
hal_jpege_ret_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L57
	ldr	r3, [r0]
	cbz	r3, .L57
	ldr	r3, [r3, #52]
	cbz	r3, .L57
	mov	r0, r2
	bx	r3
.L57:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_jpege_ret_task, .-hal_jpege_ret_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_deinit, %function
hal_jpege_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	ldr	r5, [r0, #4]
	cbz	r5, .L67
	ldr	r4, [r0]
	cbz	r4, .L65
	ldr	r4, [r4, #20]
	cbz	r4, .L65
	mov	r0, r5
	blx	r4
	mov	r1, r5
	mov	r4, r0
	ldr	r0, .L70
	bl	mpp_osal_free
.L65:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L67:
	mov	r4, r5
	b	.L65
.L71:
	.align	2
.L70:
	.word	.LANCHOR0
	.fnend
	.size	hal_jpege_deinit, .-hal_jpege_deinit
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_init, %function
hal_jpege_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r5, r0
	ldr	r4, .L74
	mov	r6, r1
	cbnz	r4, .L73
	movs	r1, #59
	ldr	r3, .L74+4
	str	r1, [sp]
	ldr	r2, .L74+8
	ldr	r0, .L74+12
	bl	printk
.L73:
	ldr	r1, [r4, #8]
	ldr	r0, .L74+4
	bl	mpp_osal_calloc
	ldr	r3, [r4, #16]
	mov	r1, r6
	strd	r4, r0, [r5]
	str	r3, [sp, #12]
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, lr}
	bx	r3
.L75:
	.align	2
.L74:
	.word	hal_jpege_vepu540c
	.word	.LANCHOR0+17
	.word	.LC0
	.word	.LC1
	.fnend
	.size	hal_jpege_init, .-hal_jpege_init
	.global	hal_api_jpege_v2
	.global	hal_jpege_debug
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.9194, %object
	.size	__FUNCTION__.9194, 17
__FUNCTION__.9194:
	.ascii	"hal_jpege_deinit\000"
	.type	__FUNCTION__.9184, %object
	.size	__FUNCTION__.9184, 15
__FUNCTION__.9184:
	.ascii	"hal_jpege_init\000"
	.type	hal_api_jpege_v2, %object
	.size	hal_api_jpege_v2, 64
hal_api_jpege_v2:
	.word	.LC2
	.word	8
	.word	8
	.word	0
	.word	hal_jpege_init
	.word	hal_jpege_deinit
	.word	hal_jpege_prepare
	.word	hal_jpege_get_task
	.word	hal_jpege_gen_regs
	.word	hal_jpege_start
	.word	hal_jpege_wait
	.word	hal_jpege_part_start
	.word	hal_jpege_part_wait
	.word	hal_jpege_ret_task
	.space	8
	.bss
	.align	2
	.type	hal_jpege_debug, %object
	.size	hal_jpege_debug, 4
hal_jpege_debug:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"api\000"
.LC1:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC2:
	.ascii	"hal_jpege\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
