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
	.file	"mpp_mem.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_osal_malloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_osal_malloc, %function
mpp_osal_malloc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r0, r1, #32
	mov	r2, #-1
	mov	r1, #3264
	bic	r0, r0, #31
	b	kvmalloc_node
	.fnend
	.size	mpp_osal_malloc, .-mpp_osal_malloc
	.align	1
	.global	mpp_osal_calloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_osal_calloc, %function
mpp_osal_calloc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r1
	bl	mpp_osal_malloc
	mov	r4, r0
	cbz	r0, .L2
	mov	r2, r5
	movs	r1, #0
	bl	memset
.L2:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_osal_calloc, .-mpp_osal_calloc
	.align	1
	.global	mpp_osal_realloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_osal_realloc, %function
mpp_osal_realloc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	cbnz	r1, .L8
	mov	r1, r2
	pop	{r3, lr}
	b	mpp_osal_malloc
.L8:
	cbnz	r2, .L7
	mov	r2, r1
	ldr	r0, .L11
	movs	r1, #78
	bl	printk
.L7:
	movs	r0, #0
	pop	{r3, pc}
.L12:
	.align	2
.L11:
	.word	.LC0
	.fnend
	.size	mpp_osal_realloc, .-mpp_osal_realloc
	.align	1
	.global	mpp_osal_free
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_osal_free, %function
mpp_osal_free:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbz	r1, .L13
	mov	r0, r1
	b	kvfree
.L13:
	bx	lr
	.fnend
	.size	mpp_osal_free, .-mpp_osal_free
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%d: warning: realloc %p to zero size\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
