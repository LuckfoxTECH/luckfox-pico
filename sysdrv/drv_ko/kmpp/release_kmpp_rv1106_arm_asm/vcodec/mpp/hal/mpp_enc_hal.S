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
	.file	"mpp_enc_hal.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_enc_hal_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_init, %function
mpp_enc_hal_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r6, r1
	mov	r7, r0
	cbz	r0, .L2
	cbnz	r1, .L3
.L2:
	str	r6, [sp]
	mov	r3, r7
	movs	r2, #54
	ldr	r1, .L15
	ldr	r0, .L15+4
	bl	printk
	mvn	r0, #2
.L1:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L3:
	movs	r1, #20
	movs	r5, #0
	str	r5, [r0]
	ldr	r0, .L15+8
	bl	mpp_osal_calloc
	ldr	r1, .L15
	mov	r4, r0
	cbz	r0, .L5
	ldr	r2, .L15+12
	ldr	r3, [r6]
	ldr	r2, [r2, #4]
	cmp	r3, r2
	beq	.L9
	ldr	r2, .L15+16
	ldr	r2, [r2, #4]
	cmp	r3, r2
	beq	.L10
	ldr	r2, .L15+20
	ldr	r2, [r2, #4]
	cmp	r3, r2
	beq	.L11
.L7:
	ldr	r3, [r6]
	movs	r2, #92
	ldr	r1, .L15
	ldr	r0, .L15+24
	bl	printk
	ldr	r1, [r4, #4]
	ldr	r0, .L15+8
	bl	mpp_osal_free
	ldr	r0, .L15+8
	mov	r1, r4
	bl	mpp_osal_free
	mov	r0, #-1
	b	.L1
.L5:
	movs	r2, #61
	ldr	r0, .L15+28
	bl	printk
	mvn	r0, #3
	b	.L1
.L9:
	mov	r2, r5
.L6:
	add	r1, r1, r2, lsl #2
	str	r3, [r4]
	ldr	r0, .L15+8
	ldr	r5, [r1, #36]
	str	r5, [r4, #8]
	ldr	r1, [r5, #8]
	bl	mpp_osal_calloc
	ldr	r3, [r4, #8]
	mov	r1, r6
	str	r0, [r4, #4]
	ldr	r3, [r3, #16]
	blx	r3
	cbz	r0, .L8
	str	r0, [sp]
	movs	r2, #76
	ldr	r3, [r5]
	ldr	r1, .L15
	ldr	r0, .L15+32
	bl	printk
	b	.L7
.L10:
	movs	r2, #1
	b	.L6
.L11:
	movs	r2, #2
	b	.L6
.L8:
	str	r4, [r7]
	b	.L1
.L16:
	.align	2
.L15:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR0+17
	.word	hal_api_h265e_v2
	.word	hal_api_h264e_v2
	.word	hal_api_jpege_v2
	.word	.LC3
	.word	.LC1
	.word	.LC2
	.fnend
	.size	mpp_enc_hal_init, .-mpp_enc_hal_init
	.align	1
	.global	mpp_enc_hal_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_deinit, %function
mpp_enc_hal_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbnz	r0, .L18
	movs	r2, #104
	ldr	r1, .L20
	ldr	r0, .L20+4
	bl	printk
	mvn	r0, #2
.L17:
	pop	{r4, pc}
.L18:
	ldr	r3, [r0, #8]
	ldr	r0, [r0, #4]
	ldr	r3, [r3, #20]
	blx	r3
	ldr	r1, [r4, #4]
	ldr	r0, .L20+8
	bl	mpp_osal_free
	ldr	r0, .L20+8
	mov	r1, r4
	bl	mpp_osal_free
	movs	r0, #0
	b	.L17
.L21:
	.align	2
.L20:
	.word	.LANCHOR0+48
	.word	.LC4
	.word	.LANCHOR0+67
	.fnend
	.size	mpp_enc_hal_deinit, .-mpp_enc_hal_deinit
	.align	1
	.global	mpp_enc_hal_prepare
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_prepare, %function
mpp_enc_hal_prepare:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r3, r0
	cbnz	r0, .L23
	movs	r2, #124
	ldr	r1, .L27
	ldr	r0, .L27+4
	bl	printk
	mvn	r0, #2
.L22:
	pop	{r4, pc}
.L23:
	ldr	r0, [r0, #8]
	cmp	r0, #0
	beq	.L22
	ldr	r2, [r0, #24]
	cbz	r2, .L26
	ldr	r0, [r3, #4]
	pop	{r4, lr}
	bx	r2
.L26:
	mov	r0, r2
	b	.L22
.L28:
	.align	2
.L27:
	.word	.LANCHOR0+86
	.word	.LC5
	.fnend
	.size	mpp_enc_hal_prepare, .-mpp_enc_hal_prepare
	.align	1
	.global	mpp_enc_hal_check_part_mode
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_check_part_mode, %function
mpp_enc_hal_check_part_mode:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbz	r0, .L33
	ldr	r3, [r0, #8]
	cbz	r3, .L33
	ldr	r2, [r3, #44]
	cbz	r2, .L33
	ldr	r0, [r3, #48]
	clz	r0, r0
	lsrs	r0, r0, #5
	rsbs	r0, r0, #0
	bx	lr
.L33:
	mov	r0, #-1
	bx	lr
	.fnend
	.size	mpp_enc_hal_check_part_mode, .-mpp_enc_hal_check_part_mode
	.align	1
	.global	mpp_enc_hal_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_start, %function
mpp_enc_hal_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r3, r0
	mov	r6, r1
	cbz	r0, .L35
	cbnz	r1, .L36
.L35:
	str	r6, [sp]
	movs	r2, #150
	ldr	r1, .L50
	ldr	r0, .L50+4
	bl	printk
	mvn	r0, #2
.L34:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L36:
	ldr	r5, [r0, #8]
	cbz	r5, .L39
	ldr	r4, [r5, #36]
	cbz	r4, .L40
	ldr	r0, [r0, #4]
	cbz	r2, .L38
	ldr	r3, [r5, #56]
	cbz	r3, .L38
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	bx	r3
.L38:
	mov	r1, r6
	mov	r3, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	bx	r3
.L39:
	mov	r0, r5
	b	.L34
.L40:
	mov	r0, r4
	b	.L34
.L51:
	.align	2
.L50:
	.word	.LANCHOR0+106
	.word	.LC6
	.fnend
	.size	mpp_enc_hal_start, .-mpp_enc_hal_start
	.align	1
	.global	mpp_enc_hal_ret_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_ret_task, %function
mpp_enc_hal_ret_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r3, r0
	mov	r6, r1
	cbz	r0, .L53
	cbnz	r1, .L54
.L53:
	str	r6, [sp]
	movs	r2, #167
	ldr	r1, .L68
	ldr	r0, .L68+4
	bl	printk
	mvn	r0, #2
.L52:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L54:
	ldr	r5, [r0, #8]
	cbz	r5, .L57
	ldr	r4, [r5, #52]
	cbz	r4, .L58
	ldr	r0, [r0, #4]
	cbz	r2, .L56
	ldr	r3, [r5, #60]
	cbz	r3, .L56
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	bx	r3
.L56:
	mov	r1, r6
	mov	r3, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	bx	r3
.L57:
	mov	r0, r5
	b	.L52
.L58:
	mov	r0, r4
	b	.L52
.L69:
	.align	2
.L68:
	.word	.LANCHOR0+124
	.word	.LC6
	.fnend
	.size	mpp_enc_hal_ret_task, .-mpp_enc_hal_ret_task
	.align	1
	.global	mpp_enc_hal_get_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_get_task, %function
mpp_enc_hal_get_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	mov	r3, r0
	mov	r2, r1
	cbz	r0, .L71
	cbnz	r1, .L72
.L71:
	str	r2, [sp]
	movs	r2, #195
	ldr	r1, .L79
	ldr	r0, .L79+4
	bl	printk
	mvn	r0, #2
.L70:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L72:
	ldr	r0, [r0, #8]
	cmp	r0, #0
	beq	.L70
	ldr	r2, [r0, #28]
	cbz	r2, .L75
	ldr	r0, [r3, #4]
	add	sp, sp, #12
	@ sp needed
	ldr	lr, [sp], #4
	bx	r2
.L75:
	mov	r0, r2
	b	.L70
.L80:
	.align	2
.L79:
	.word	.LANCHOR0+145
	.word	.LC6
	.fnend
	.size	mpp_enc_hal_get_task, .-mpp_enc_hal_get_task
	.align	1
	.global	mpp_enc_hal_gen_regs
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_gen_regs, %function
mpp_enc_hal_gen_regs:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	mov	r3, r0
	mov	r2, r1
	cbz	r0, .L82
	cbnz	r1, .L83
.L82:
	str	r2, [sp]
	movs	r2, #196
	ldr	r1, .L90
	ldr	r0, .L90+4
	bl	printk
	mvn	r0, #2
.L81:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L83:
	ldr	r0, [r0, #8]
	cmp	r0, #0
	beq	.L81
	ldr	r2, [r0, #32]
	cbz	r2, .L86
	ldr	r0, [r3, #4]
	add	sp, sp, #12
	@ sp needed
	ldr	lr, [sp], #4
	bx	r2
.L86:
	mov	r0, r2
	b	.L81
.L91:
	.align	2
.L90:
	.word	.LANCHOR0+166
	.word	.LC6
	.fnend
	.size	mpp_enc_hal_gen_regs, .-mpp_enc_hal_gen_regs
	.align	1
	.global	mpp_enc_hal_wait
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_wait, %function
mpp_enc_hal_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	mov	r3, r0
	mov	r2, r1
	cbz	r0, .L93
	cbnz	r1, .L94
.L93:
	str	r2, [sp]
	movs	r2, #198
	ldr	r1, .L101
	ldr	r0, .L101+4
	bl	printk
	mvn	r0, #2
.L92:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L94:
	ldr	r0, [r0, #8]
	cmp	r0, #0
	beq	.L92
	ldr	r2, [r0, #40]
	cbz	r2, .L97
	ldr	r0, [r3, #4]
	add	sp, sp, #12
	@ sp needed
	ldr	lr, [sp], #4
	bx	r2
.L97:
	mov	r0, r2
	b	.L92
.L102:
	.align	2
.L101:
	.word	.LANCHOR0+187
	.word	.LC6
	.fnend
	.size	mpp_enc_hal_wait, .-mpp_enc_hal_wait
	.align	1
	.global	mpp_enc_hal_part_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_part_start, %function
mpp_enc_hal_part_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	mov	r3, r0
	mov	r2, r1
	cbz	r0, .L104
	cbnz	r1, .L105
.L104:
	str	r2, [sp]
	movs	r2, #199
	ldr	r1, .L112
	ldr	r0, .L112+4
	bl	printk
	mvn	r0, #2
.L103:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L105:
	ldr	r0, [r0, #8]
	cmp	r0, #0
	beq	.L103
	ldr	r2, [r0, #44]
	cbz	r2, .L108
	ldr	r0, [r3, #4]
	add	sp, sp, #12
	@ sp needed
	ldr	lr, [sp], #4
	bx	r2
.L108:
	mov	r0, r2
	b	.L103
.L113:
	.align	2
.L112:
	.word	.LANCHOR0+204
	.word	.LC6
	.fnend
	.size	mpp_enc_hal_part_start, .-mpp_enc_hal_part_start
	.align	1
	.global	mpp_enc_hal_part_wait
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hal_part_wait, %function
mpp_enc_hal_part_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	mov	r3, r0
	mov	r2, r1
	cbz	r0, .L115
	cbnz	r1, .L116
.L115:
	str	r2, [sp]
	movs	r2, #200
	ldr	r1, .L123
	ldr	r0, .L123+4
	bl	printk
	mvn	r0, #2
.L114:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L116:
	ldr	r0, [r0, #8]
	cmp	r0, #0
	beq	.L114
	ldr	r2, [r0, #48]
	cbz	r2, .L119
	ldr	r0, [r3, #4]
	add	sp, sp, #12
	@ sp needed
	ldr	lr, [sp], #4
	bx	r2
.L119:
	mov	r0, r2
	b	.L114
.L124:
	.align	2
.L123:
	.word	.LANCHOR0+227
	.word	.LC6
	.fnend
	.size	mpp_enc_hal_part_wait, .-mpp_enc_hal_part_wait
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.8983, %object
	.size	__func__.8983, 17
__func__.8983:
	.ascii	"mpp_enc_hal_init\000"
	.type	__FUNCTION__.8984, %object
	.size	__FUNCTION__.8984, 17
__FUNCTION__.8984:
	.ascii	"mpp_enc_hal_init\000"
	.space	2
	.type	hw_enc_apis, %object
	.size	hw_enc_apis, 12
hw_enc_apis:
	.word	hal_api_h265e_v2
	.word	hal_api_h264e_v2
	.word	hal_api_jpege_v2
	.type	__func__.8993, %object
	.size	__func__.8993, 19
__func__.8993:
	.ascii	"mpp_enc_hal_deinit\000"
	.type	__FUNCTION__.8994, %object
	.size	__FUNCTION__.8994, 19
__FUNCTION__.8994:
	.ascii	"mpp_enc_hal_deinit\000"
	.type	__func__.8999, %object
	.size	__func__.8999, 20
__func__.8999:
	.ascii	"mpp_enc_hal_prepare\000"
	.type	__func__.9010, %object
	.size	__func__.9010, 18
__func__.9010:
	.ascii	"mpp_enc_hal_start\000"
	.type	__func__.9017, %object
	.size	__func__.9017, 21
__func__.9017:
	.ascii	"mpp_enc_hal_ret_task\000"
	.type	__func__.9023, %object
	.size	__func__.9023, 21
__func__.9023:
	.ascii	"mpp_enc_hal_get_task\000"
	.type	__func__.9029, %object
	.size	__func__.9029, 21
__func__.9029:
	.ascii	"mpp_enc_hal_gen_regs\000"
	.type	__func__.9035, %object
	.size	__func__.9035, 17
__func__.9035:
	.ascii	"mpp_enc_hal_wait\000"
	.type	__func__.9041, %object
	.size	__func__.9041, 23
__func__.9041:
	.ascii	"mpp_enc_hal_part_start\000"
	.type	__func__.9047, %object
	.size	__func__.9047, 22
__func__.9047:
	.ascii	"mpp_enc_hal_part_wait\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: found NULL input ctx %p cfg %p\012\000"
.LC1:
	.ascii	"\0013%s:%d: malloc failed\012\000"
.LC2:
	.ascii	"\0013%s:%d: hal %s init failed ret %d\012\000"
.LC3:
	.ascii	"\0013%s:%d: could not found coding type %d\012\000"
.LC4:
	.ascii	"\0013%s:%d: found NULL input\012\000"
.LC5:
	.ascii	"\0013%s:%d: found NULL input ctx %p\012\000"
.LC6:
	.ascii	"\0013%s:%d: found NULL input ctx %p task %p\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
