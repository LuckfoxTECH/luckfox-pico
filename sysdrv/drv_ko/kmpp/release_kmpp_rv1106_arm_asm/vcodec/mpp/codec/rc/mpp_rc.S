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
	.file	"mpp_rc.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_data_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_init, %function
mpp_data_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r1
	mov	r6, r0
	cbz	r0, .L2
	cmp	r1, #0
	bgt	.L3
.L2:
	str	r5, [sp]
	mov	r3, r6
	movs	r2, #50
	ldr	r1, .L9
	ldr	r0, .L9+4
	bl	printk
	mvn	r0, #2
.L1:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L3:
	adds	r1, r1, #4
	movs	r4, #0
	str	r4, [r0]
	lsls	r1, r1, #2
	ldr	r0, .L9+8
	bl	mpp_osal_malloc
	cbnz	r0, .L5
	mov	r3, r5
	movs	r2, #57
	ldr	r1, .L9
	ldr	r0, .L9+12
	bl	printk
	mvn	r0, #3
	b	.L1
.L5:
	strd	r5, r4, [r0]
	add	r3, r0, #16
	str	r4, [r0, #8]
	str	r3, [r0, #12]
	str	r0, [r6]
	mov	r0, r4
	b	.L1
.L10:
	.align	2
.L9:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR0+14
	.word	.LC1
	.fnend
	.size	mpp_data_init, .-mpp_data_init
	.align	1
	.global	mpp_data_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_deinit, %function
mpp_data_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r1, r0
	cbz	r0, .L11
	ldr	r0, .L13
	b	mpp_osal_free
.L11:
	bx	lr
.L14:
	.align	2
.L13:
	.word	.LANCHOR0+28
	.fnend
	.size	mpp_data_deinit, .-mpp_data_deinit
	.align	1
	.global	mpp_data_update
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_update, %function
mpp_data_update:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r5, r1
	mov	r4, r0
	cbnz	r0, .L16
	movs	r1, #77
	ldr	r3, .L19
	str	r1, [sp]
	ldr	r2, .L19+4
	ldr	r0, .L19+8
	bl	printk
.L16:
	ldrd	r2, r3, [r4, #8]
	str	r5, [r3, r2, lsl #2]
	ldr	r3, [r4, #8]
	ldr	r2, [r4]
	adds	r3, r3, #1
	str	r3, [r4, #8]
	cmp	r3, r2
	itt	ge
	movge	r3, #0
	strge	r3, [r4, #8]
	ldr	r3, [r4, #4]
	cmp	r2, r3
	itt	gt
	addgt	r3, r3, #1
	strgt	r3, [r4, #4]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L20:
	.align	2
.L19:
	.word	.LANCHOR0+44
	.word	.LC2
	.word	.LC3
	.fnend
	.size	mpp_data_update, .-mpp_data_update
	.global	__aeabi_idiv
	.align	1
	.global	mpp_data_avg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_avg, %function
mpp_data_avg:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r1
	.pad #20
	sub	sp, sp, #20
	mov	r8, r2
	mov	r9, r3
	mov	r7, r0
	cbnz	r0, .L22
	movs	r1, #94
	ldr	r3, .L45
	str	r1, [sp]
	ldr	r2, .L45+4
	ldr	r0, .L45+8
	bl	printk
.L22:
	ldr	r0, [r7, #4]
	cbz	r0, .L21
	cmp	r5, #0
	blt	.L37
	cmp	r5, r0
	it	ge
	movge	r5, r0
.L24:
	cmp	r8, r9
	ldr	r4, [r7, #8]
	bne	.L43
	mov	r3, r5
	movs	r6, #0
	b	.L25
.L37:
	mov	r5, r0
	b	.L24
.L29:
	cbz	r4, .L27
	subs	r4, r4, #1
.L28:
	ldr	r2, [r7, #12]
	ldr	r2, [r2, r4, lsl #2]
	add	r6, r6, r2
.L25:
	adds	r3, r3, #-1
	bcs	.L29
.L30:
	cmp	r6, #0
	mov	r1, r5
	ite	lt
	movlt	r0, #-1
	movge	r0, #1
	muls	r0, r5, r0
	add	r0, r0, r0, lsr #31
	add	r0, r6, r0, asr #1
	bl	__aeabi_idiv
.L21:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L27:
	subs	r4, r0, #1
	b	.L28
.L43:
	bgt	.L31
	movs	r1, #117
	ldr	r3, .L45
	str	r1, [sp]
	ldr	r2, .L45+12
	ldr	r0, .L45+8
	bl	printk
.L31:
	ldr	r3, [r7, #12]
	subs	r4, r4, #1
	add	r10, r5, #-1
	mov	r2, r9
	mov	fp, r8
	ldr	r6, [r3, r4, lsl #2]
.L32:
	adds	r10, r10, #-1
	bcc	.L30
	cbz	r4, .L33
.L44:
	subs	r4, r4, #1
	mov	r1, r2
	str	r3, [sp, #12]
	ldr	r0, [r3, r4, lsl #2]
	str	r2, [sp, #8]
	mul	r0, r0, fp
	bl	__aeabi_idiv
	mul	fp, r8, fp
	ldr	r2, [sp, #8]
	add	r6, r6, r0
	ldr	r3, [sp, #12]
	mul	r2, r9, r2
	b	.L32
.L33:
	ldr	r4, [r7, #4]
	b	.L44
.L46:
	.align	2
.L45:
	.word	.LANCHOR0+60
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.fnend
	.size	mpp_data_avg, .-mpp_data_avg
	.align	1
	.global	mpp_pid_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_pid_reset, %function
mpp_pid_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	strd	r3, r3, [r0]
	str	r3, [r0, #8]
	str	r3, [r0, #32]
	bx	lr
	.fnend
	.size	mpp_pid_reset, .-mpp_pid_reset
	.align	1
	.global	mpp_pid_set_param
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_pid_set_param, %function
mpp_pid_set_param:
	.fnstart
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	str	r3, [r0, #20]
	ldr	r3, [sp]
	strd	r1, r2, [r0, #12]
	str	r3, [r0, #24]
	ldr	r3, [sp, #4]
	str	r3, [r0, #28]
	movs	r3, #0
	str	r3, [r0, #32]
	bx	lr
	.fnend
	.size	mpp_pid_set_param, .-mpp_pid_set_param
	.align	1
	.global	mpp_pid_update
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_pid_update, %function
mpp_pid_update:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0]
	ldr	r2, [r0, #28]
	subs	r3, r1, r3
	str	r3, [r0, #8]
	ldr	r3, [r0, #4]
	add	r3, r3, r1
	strd	r1, r3, [r0]
	ldr	r3, [r0, #32]
	adds	r3, r3, #1
	str	r3, [r0, #32]
	cmp	r3, r2
	itttt	ge
	movge	r3, #0
	strdge	r3, r3, [r0]
	strge	r3, [r0, #8]
	strge	r3, [r0, #32]
	bx	lr
	.fnend
	.size	mpp_pid_update, .-mpp_pid_update
	.align	1
	.global	mpp_pid_calc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_pid_calc, %function
mpp_pid_calc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r1, [r0, #16]
	ldr	r3, [r0, #4]
	ldr	r2, [r0]
	ldr	r4, [r0, #12]
	muls	r3, r1, r3
	ldr	r1, [r0, #20]
	mla	r3, r4, r2, r3
	ldr	r2, [r0, #8]
	mla	r2, r1, r2, r3
	ldr	r1, [r0, #24]
	cmp	r2, #0
	ite	lt
	movlt	r3, #-1
	movge	r3, #1
	muls	r3, r1, r3
	add	r3, r3, r3, lsr #31
	add	r0, r2, r3, asr #1
	bl	__aeabi_idiv
	pop	{r4, pc}
	.fnend
	.size	mpp_pid_calc, .-mpp_pid_calc
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.8407, %object
	.size	__func__.8407, 14
__func__.8407:
	.ascii	"mpp_data_init\000"
	.type	__FUNCTION__.8408, %object
	.size	__FUNCTION__.8408, 14
__FUNCTION__.8408:
	.ascii	"mpp_data_init\000"
	.type	__FUNCTION__.8412, %object
	.size	__FUNCTION__.8412, 16
__FUNCTION__.8412:
	.ascii	"mpp_data_deinit\000"
	.type	__FUNCTION__.8417, %object
	.size	__FUNCTION__.8417, 16
__FUNCTION__.8417:
	.ascii	"mpp_data_update\000"
	.type	__FUNCTION__.8429, %object
	.size	__FUNCTION__.8429, 13
__FUNCTION__.8429:
	.ascii	"mpp_data_avg\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: invalid data %p size %d\012\000"
.LC1:
	.ascii	"\0013%s:%d: malloc size %d failed\012\000"
.LC2:
	.ascii	"p\000"
.LC3:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC4:
	.ascii	"num > denorm\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
