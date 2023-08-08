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
	.file	"hal_h265e_api_v2.c"
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
	.type	hal_h265ev2_comb_start, %function
hal_h265ev2_comb_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4}
	.save {r4}
	mov	r3, r0
	ldr	r0, [r0, #4]
	cbz	r0, .L1
	ldr	r3, [r3]
	cbz	r3, .L1
	ldr	r4, [r3, #56]
	cbnz	r4, .L3
	ldr	r3, [r3, #36]
	pop	{r4}
	bx	r3
.L3:
	mov	r3, r4
	pop	{r4}
	bx	r3
.L1:
	movs	r0, #0
	pop	{r4}
	bx	lr
	.fnend
	.size	hal_h265ev2_comb_start, .-hal_h265ev2_comb_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_comb_ret_task, %function
hal_h265ev2_comb_ret_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4}
	.save {r4}
	mov	r3, r0
	ldr	r0, [r0, #4]
	cbz	r0, .L8
	ldr	r3, [r3]
	cbz	r3, .L8
	ldr	r4, [r3, #60]
	cbnz	r4, .L10
	ldr	r3, [r3, #52]
	pop	{r4}
	bx	r3
.L10:
	mov	r3, r4
	pop	{r4}
	bx	r3
.L8:
	movs	r0, #0
	pop	{r4}
	bx	lr
	.fnend
	.size	hal_h265ev2_comb_ret_task, .-hal_h265ev2_comb_ret_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_prepare, %function
hal_h265ev2_prepare:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L15
	ldr	r3, [r0]
	cbz	r3, .L15
	ldr	r3, [r3, #24]
	cbz	r3, .L15
	mov	r0, r2
	bx	r3
.L15:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_h265ev2_prepare, .-hal_h265ev2_prepare
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_get_task, %function
hal_h265ev2_get_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L23
	ldr	r3, [r0]
	cbz	r3, .L23
	ldr	r3, [r3, #28]
	cbz	r3, .L23
	mov	r0, r2
	bx	r3
.L23:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_h265ev2_get_task, .-hal_h265ev2_get_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_gen_regs, %function
hal_h265ev2_gen_regs:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L31
	ldr	r3, [r0]
	cbz	r3, .L31
	ldr	r3, [r3, #32]
	cbz	r3, .L31
	mov	r0, r2
	bx	r3
.L31:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_h265ev2_gen_regs, .-hal_h265ev2_gen_regs
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_start, %function
hal_h265ev2_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L39
	ldr	r3, [r0]
	cbz	r3, .L39
	ldr	r3, [r3, #36]
	cbz	r3, .L39
	mov	r0, r2
	bx	r3
.L39:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_h265ev2_start, .-hal_h265ev2_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_wait, %function
hal_h265ev2_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L47
	ldr	r3, [r0]
	cbz	r3, .L47
	ldr	r3, [r3, #40]
	cbz	r3, .L47
	mov	r0, r2
	bx	r3
.L47:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_h265ev2_wait, .-hal_h265ev2_wait
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_part_start, %function
hal_h265ev2_part_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L55
	ldr	r3, [r0]
	cbz	r3, .L55
	ldr	r3, [r3, #44]
	cbz	r3, .L55
	mov	r0, r2
	bx	r3
.L55:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_h265ev2_part_start, .-hal_h265ev2_part_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_part_wait, %function
hal_h265ev2_part_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L63
	ldr	r3, [r0]
	cbz	r3, .L63
	ldr	r3, [r3, #48]
	cbz	r3, .L63
	mov	r0, r2
	bx	r3
.L63:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_h265ev2_part_wait, .-hal_h265ev2_part_wait
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_ret_task, %function
hal_h265ev2_ret_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #4]
	cbz	r2, .L71
	ldr	r3, [r0]
	cbz	r3, .L71
	ldr	r3, [r3, #52]
	cbz	r3, .L71
	mov	r0, r2
	bx	r3
.L71:
	movs	r0, #0
	bx	lr
	.fnend
	.size	hal_h265ev2_ret_task, .-hal_h265ev2_ret_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_deinit, %function
hal_h265ev2_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	ldr	r5, [r0, #4]
	cbz	r5, .L81
	ldr	r4, [r0]
	cbz	r4, .L79
	ldr	r4, [r4, #20]
	cbz	r4, .L79
	mov	r0, r5
	blx	r4
	mov	r1, r5
	mov	r4, r0
	ldr	r0, .L84
	bl	mpp_osal_free
.L79:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L81:
	mov	r4, r5
	b	.L79
.L85:
	.align	2
.L84:
	.word	.LANCHOR0
	.fnend
	.size	hal_h265ev2_deinit, .-hal_h265ev2_deinit
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_h265ev2_init, %function
hal_h265ev2_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r5, r0
	ldr	r4, .L88
	mov	r6, r1
	cbnz	r4, .L87
	movs	r1, #57
	ldr	r3, .L88+4
	str	r1, [sp]
	ldr	r2, .L88+8
	ldr	r0, .L88+12
	bl	printk
.L87:
	ldr	r1, [r4, #8]
	ldr	r0, .L88+4
	bl	mpp_osal_calloc
	ldr	r3, [r4, #16]
	mov	r1, r6
	strd	r4, r0, [r5]
	str	r3, [sp, #12]
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, lr}
	bx	r3
.L89:
	.align	2
.L88:
	.word	hal_h265e_vepu540c
	.word	.LANCHOR0+19
	.word	.LC0
	.word	.LC1
	.fnend
	.size	hal_h265ev2_init, .-hal_h265ev2_init
	.global	hal_api_h265e_v2
	.global	hal_h265e_debug
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.19052, %object
	.size	__FUNCTION__.19052, 19
__FUNCTION__.19052:
	.ascii	"hal_h265ev2_deinit\000"
	.type	__FUNCTION__.19042, %object
	.size	__FUNCTION__.19042, 17
__FUNCTION__.19042:
	.ascii	"hal_h265ev2_init\000"
	.type	hal_api_h265e_v2, %object
	.size	hal_api_h265e_v2, 64
hal_api_h265e_v2:
	.word	.LC2
	.word	16777220
	.word	8
	.word	0
	.word	hal_h265ev2_init
	.word	hal_h265ev2_deinit
	.word	hal_h265ev2_prepare
	.word	hal_h265ev2_get_task
	.word	hal_h265ev2_gen_regs
	.word	hal_h265ev2_start
	.word	hal_h265ev2_wait
	.word	hal_h265ev2_part_start
	.word	hal_h265ev2_part_wait
	.word	hal_h265ev2_ret_task
	.word	hal_h265ev2_comb_start
	.word	hal_h265ev2_comb_ret_task
	.type	__param_str_hal_h265e_debug, %object
	.size	__param_str_hal_h265e_debug, 27
__param_str_hal_h265e_debug:
	.ascii	"mpp_vcodec.hal_h265e_debug\000"
	.bss
	.align	2
	.type	hal_h265e_debug, %object
	.size	hal_h265e_debug, 4
hal_h265e_debug:
	.space	4
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_hal_h265e_debug105, %object
	.size	__UNIQUE_ID_hal_h265e_debug105, 69
__UNIQUE_ID_hal_h265e_debug105:
	.ascii	"mpp_vcodec.parm=hal_h265e_debug:bits for hal_h265e "
	.ascii	"debug information\000"
	.type	__UNIQUE_ID_hal_h265e_debugtype104, %object
	.size	__UNIQUE_ID_hal_h265e_debugtype104, 41
__UNIQUE_ID_hal_h265e_debugtype104:
	.ascii	"mpp_vcodec.parmtype=hal_h265e_debug:uint\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"api\000"
.LC1:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC2:
	.ascii	"hal_h265e\000"
	.section	__param,"a"
	.align	2
	.type	__param_hal_h265e_debug, %object
	.size	__param_hal_h265e_debug, 20
__param_hal_h265e_debug:
	.word	__param_str_hal_h265e_debug
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	hal_h265e_debug
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
