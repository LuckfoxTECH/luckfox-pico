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
	.file	"rc_base.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_data_deinit_v2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_deinit_v2, %function
mpp_data_deinit_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r1, r0
	cbz	r0, .L1
	ldr	r0, .L3
	b	mpp_osal_free
.L1:
	bx	lr
.L4:
	.align	2
.L3:
	.word	.LANCHOR0
	.fnend
	.size	mpp_data_deinit_v2, .-mpp_data_deinit_v2
	.align	1
	.global	mpp_data_reset_v2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_reset_v2, %function
mpp_data_reset_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movs	r3, #0
	ldr	r4, [r0]
	ldr	r6, [r0, #16]
	str	r3, [r0, #8]
	str	r4, [r0, #12]
	muls	r4, r1, r4
	asrs	r5, r4, #31
	strd	r4, [r0, #24]
.L6:
	ldr	r2, [r0]
	cmp	r2, r3
	bgt	.L7
	pop	{r4, r5, r6, pc}
.L7:
	str	r1, [r6, r3, lsl #2]
	adds	r3, r3, #1
	b	.L6
	.fnend
	.size	mpp_data_reset_v2, .-mpp_data_reset_v2
	.align	1
	.global	mpp_data_init_v2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_init_v2, %function
mpp_data_init_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r5, r1
	mov	r7, r2
	mov	r6, r0
	cbz	r0, .L9
	cmp	r1, #0
	bgt	.L10
.L9:
	str	r5, [sp]
	mov	r3, r6
	movs	r2, #41
	ldr	r1, .L16
	ldr	r0, .L16+4
	bl	printk
	mvn	r0, #2
.L8:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L10:
	adds	r1, r1, #8
	movs	r4, #0
	str	r4, [r0]
	lsls	r1, r1, #2
	ldr	r0, .L16+8
	bl	mpp_osal_malloc
	cbnz	r0, .L12
	mov	r3, r5
	movs	r2, #49
	ldr	r1, .L16
	ldr	r0, .L16+12
	bl	printk
	mvn	r0, #3
	b	.L8
.L12:
	add	r3, r0, #32
	strd	r5, r4, [r0]
	strd	r4, r3, [r0, #12]
	movs	r2, #0
	str	r4, [r0, #8]
	movs	r3, #0
	mov	r1, r7
	strd	r2, [r0, #24]
	str	r0, [r6]
	bl	mpp_data_reset_v2
	mov	r0, r4
	b	.L8
.L17:
	.align	2
.L16:
	.word	.LANCHOR0+19
	.word	.LC0
	.word	.LANCHOR0+36
	.word	.LC1
	.fnend
	.size	mpp_data_init_v2, .-mpp_data_init_v2
	.align	1
	.global	mpp_data_update_v2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_update_v2, %function
mpp_data_update_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, fp, lr}
	.save {r4, r5, fp, lr}
	.pad #8
	mov	r5, r1
	mov	r4, r0
	cbnz	r0, .L19
	movs	r1, #84
	ldr	r3, .L23
	str	r1, [sp]
	ldr	r2, .L23+4
	ldr	r0, .L23+8
	bl	printk
.L19:
	ldr	r2, [r4, #12]
	ldr	r3, [r4]
	cmp	r2, r3
	bne	.L20
	subs	r2, r3, #1
	ldr	r1, [r4, #8]
	str	r2, [r4, #12]
	ldr	r2, [r4, #16]
	ldr	r2, [r2, r1, lsl #2]
	ldrd	r0, [r4, #24]
	subs	fp, r0, r2
	sbc	ip, r1, r2, asr #31
	strd	fp, [r4, #24]
.L20:
	ldr	r2, [r4, #12]
	cmp	r3, r2
	bgt	.L21
	movs	r1, #89
	ldr	r3, .L23
	str	r1, [sp]
	ldr	r2, .L23+12
	ldr	r0, .L23+8
	bl	printk
.L21:
	ldr	r2, [r4, #8]
	ldr	r3, [r4, #16]
	str	r5, [r3, r2, lsl #2]
	ldr	r3, [r4, #8]
	ldr	r2, [r4, #16]
	ldrd	r0, [r4, #24]
	ldr	r2, [r2, r3, lsl #2]
	adds	r3, r3, #1
	str	r3, [r4, #8]
	adds	fp, r0, r2
	adc	ip, r1, r2, asr #31
	ldr	r2, [r4, #12]
	strd	fp, [r4, #24]
	adds	r2, r2, #1
	str	r2, [r4, #12]
	ldr	r2, [r4]
	cmp	r3, r2
	itt	ge
	movge	r3, #0
	strge	r3, [r4, #8]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, fp, pc}
.L24:
	.align	2
.L23:
	.word	.LANCHOR0+53
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.fnend
	.size	mpp_data_update_v2, .-mpp_data_update_v2
	.align	1
	.global	mpp_data_get_pre_val_v2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_get_pre_val_v2, %function
mpp_data_get_pre_val_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	subs	r5, r1, #0
	ldr	r3, [r0]
	mov	r6, r0
	ldr	r2, [r0, #8]
	it	lt
	addlt	r5, r5, r3
	cmp	r2, r3
	blt	.L27
	movs	r1, #103
	ldr	r3, .L32
	str	r1, [sp]
	ldr	r2, .L32+4
	ldr	r0, .L32+8
	bl	printk
.L27:
	ldr	r3, [r6]
	cmp	r3, r5
	bgt	.L28
	movs	r1, #104
	ldr	r3, .L32
	str	r1, [sp]
	ldr	r2, .L32+12
	ldr	r0, .L32+8
	bl	printk
.L28:
	ldr	r4, [r6, #8]
	subs	r4, r4, #1
	subs	r4, r4, r5
	bpl	.L29
	ldr	r2, [r6, #12]
	ldr	r3, [r6]
	cmp	r2, r3
	beq	.L30
	movs	r1, #108
	ldr	r3, .L32
	str	r1, [sp]
	ldr	r2, .L32+16
	ldr	r0, .L32+8
	bl	printk
.L30:
	ldr	r3, [r6]
	add	r4, r4, r3
.L29:
	ldr	r3, [r6]
	cmp	r3, r4
	bgt	.L31
	movs	r1, #113
	ldr	r3, .L32
	str	r1, [sp]
	ldr	r2, .L32+20
	ldr	r0, .L32+8
	bl	printk
.L31:
	ldr	r3, [r6, #16]
	ldr	r0, [r3, r4, lsl #2]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L33:
	.align	2
.L32:
	.word	.LANCHOR0+72
	.word	.LC5
	.word	.LC3
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.fnend
	.size	mpp_data_get_pre_val_v2, .-mpp_data_get_pre_val_v2
	.align	1
	.global	mpp_data_sum_v2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_sum_v2, %function
mpp_data_sum_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #24]
	bx	lr
	.fnend
	.size	mpp_data_sum_v2, .-mpp_data_sum_v2
	.align	1
	.global	mpp_data_mean_v2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_mean_v2, %function
mpp_data_mean_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	add	r3, sp, #4
	ldr	r2, [r0]
	ldrd	r0, [r0, #24]
	bl	div_s64_rem
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
	.fnend
	.size	mpp_data_mean_v2, .-mpp_data_mean_v2
	.align	1
	.global	mpp_data_sum_with_ratio_v2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_data_sum_with_ratio_v2, %function
mpp_data_sum_with_ratio_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #52
	sub	sp, sp, #52
	mov	r6, r1
	mov	r9, r2
	str	r3, [sp, #20]
	mov	r8, r0
	cbnz	r0, .L37
	movs	r1, #135
	ldr	r3, .L45
	str	r1, [sp]
	ldr	r2, .L45+4
	ldr	r0, .L45+8
	bl	printk
.L37:
	ldr	r3, [r8]
	ldr	r4, [r8, #16]
	cmp	r3, r6
	bge	.L38
	movs	r1, #137
	ldr	r3, .L45
	str	r1, [sp]
	ldr	r2, .L45+12
	ldr	r0, .L45+8
	bl	printk
.L38:
	ldr	r3, [sp, #20]
	cmp	r9, r3
	beq	.L44
	movs	r2, #0
	movs	r3, #0
	strd	r2, [sp, #8]
	mov	r2, r9
	asrs	r3, r2, #31
	strd	r2, [sp, #24]
	ldr	r3, [sp, #20]
	mov	r10, #1
	mov	fp, #0
	mov	r4, r10
	mov	r5, fp
	movs	r7, #0
	mov	r1, r3
	asrs	r2, r3, #31
	strd	r1, [sp, #32]
.L40:
	cmp	r7, r6
	bge	.L42
	ldr	r3, [r8, #16]
	ldr	r0, [r3, r7, lsl #2]
	adds	r7, r7, #1
	asrs	r2, r0, #31
	mul	r3, r0, r5
	mla	r3, r4, r2, r3
	mov	r2, r10
	umull	r0, r1, r0, r4
	add	r1, r1, r3
	add	r3, sp, #44
	bl	div_s64_rem
	ldrd	r2, [sp, #8]
	adds	r2, r2, r0
	adc	r3, r3, r0, asr #31
	strd	r2, [sp, #8]
	ldr	r3, [sp, #28]
	ldr	r2, [sp, #20]
	muls	r3, r4, r3
	mla	r3, r9, r5, r3
	umull	r4, r5, r4, r9
	add	r5, r5, r3
	ldr	r3, [sp, #36]
	mul	r3, r10, r3
	mla	r3, r2, fp, r3
	umull	r10, fp, r10, r2
	add	fp, fp, r3
	b	.L40
.L41:
	ldr	r2, [r4, r3, lsl #2]
	adds	r3, r3, #1
	ldrd	r0, [sp, #8]
	adds	r0, r0, r2
	adc	r1, r1, r2, asr #31
	strd	r0, [sp, #8]
.L39:
	cmp	r3, r6
	blt	.L41
.L42:
	add	r3, sp, #44
	mov	r2, r6
	ldrd	r0, [sp, #8]
	bl	div_s64_rem
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L44:
	movs	r3, #0
	movs	r2, #0
	strd	r2, [sp, #8]
	movs	r3, #0
	b	.L39
.L46:
	.align	2
.L45:
	.word	.LANCHOR0+96
	.word	.LC2
	.word	.LC3
	.word	.LC9
	.fnend
	.size	mpp_data_sum_with_ratio_v2, .-mpp_data_sum_with_ratio_v2
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.8621, %object
	.size	__FUNCTION__.8621, 19
__FUNCTION__.8621:
	.ascii	"mpp_data_deinit_v2\000"
	.type	__func__.8616, %object
	.size	__func__.8616, 17
__func__.8616:
	.ascii	"mpp_data_init_v2\000"
	.type	__FUNCTION__.8617, %object
	.size	__FUNCTION__.8617, 17
__FUNCTION__.8617:
	.ascii	"mpp_data_init_v2\000"
	.type	__FUNCTION__.8635, %object
	.size	__FUNCTION__.8635, 19
__FUNCTION__.8635:
	.ascii	"mpp_data_update_v2\000"
	.type	__FUNCTION__.8645, %object
	.size	__FUNCTION__.8645, 24
__FUNCTION__.8645:
	.ascii	"mpp_data_get_pre_val_v2\000"
	.type	__FUNCTION__.8671, %object
	.size	__FUNCTION__.8671, 27
__FUNCTION__.8671:
	.ascii	"mpp_data_sum_with_ratio_v2\000"
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
	.ascii	"p->pos_r < p->size\000"
.LC5:
	.ascii	"p->pos_w < p->size\000"
.LC6:
	.ascii	"idx < p->size\000"
.LC7:
	.ascii	"p->pos_r == p->size\000"
.LC8:
	.ascii	"pos < p->size\000"
.LC9:
	.ascii	"len <= p->size\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
