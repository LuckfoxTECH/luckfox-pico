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
	.file	"mpp_frame.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_frame_pool_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_pool_init, %function
mpp_frame_pool_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r3, r0
	ldr	r4, .L3
	ldr	r2, [r4]
	cbnz	r2, .L2
	movs	r2, #184
	ldr	r1, .L3+4
	ldr	r0, .L3+8
	bl	mpp_mem_get_pool_f
	str	r0, [r4]
.L2:
	movs	r0, #0
	pop	{r4, pc}
.L4:
	.align	2
.L3:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR1
	.fnend
	.size	mpp_frame_pool_init, .-mpp_frame_pool_init
	.align	1
	.global	mpp_frame_pool_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_pool_deinit, %function
mpp_frame_pool_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L10
	ldr	r1, [r4]
	cbz	r1, .L6
	ldr	r0, .L10+4
	bl	mpp_mem_put_pool_f
	movs	r3, #0
	str	r3, [r4]
.L6:
	movs	r0, #0
	pop	{r4, pc}
.L11:
	.align	2
.L10:
	.word	.LANCHOR0
	.word	.LANCHOR1+20
	.fnend
	.size	mpp_frame_pool_deinit, .-mpp_frame_pool_deinit
	.align	1
	.global	mpp_frame_pool_info_show
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_pool_info_show, %function
mpp_frame_pool_info_show:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L13
	ldr	r1, [r3]
	b	mpp_mem_pool_info_show
.L14:
	.align	2
.L13:
	.word	.LANCHOR0
	.fnend
	.size	mpp_frame_pool_info_show, .-mpp_frame_pool_info_show
	.align	1
	.global	check_is_mpp_frame
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_is_mpp_frame, %function
check_is_mpp_frame:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r3, r0
	cbz	r0, .L16
	ldr	r1, [r0]
	ldr	r2, .L22
	cmp	r1, r2
	beq	.L18
.L16:
	movs	r2, #58
	ldr	r1, .L22+4
	ldr	r0, .L22+8
	bl	printk
	mov	r0, #-1
.L15:
	pop	{r3, pc}
.L18:
	movs	r0, #0
	b	.L15
.L23:
	.align	2
.L22:
	.word	.LC0
	.word	.LANCHOR1+42
	.word	.LC1
	.fnend
	.size	check_is_mpp_frame, .-check_is_mpp_frame
	.align	1
	.global	mpp_frame_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_init, %function
mpp_frame_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbnz	r0, .L25
	ldr	r1, .L29
	movs	r2, #69
	ldr	r0, .L29+4
.L28:
	bl	printk
	mvn	r0, #2
.L24:
	pop	{r4, pc}
.L25:
	ldr	r3, .L29+8
	ldr	r0, .L29+12
	ldr	r1, [r3]
	bl	mpp_mem_pool_get_f
	cbnz	r0, .L27
	movs	r2, #75
	ldr	r1, .L29
	ldr	r0, .L29+16
	b	.L28
.L27:
	ldr	r3, .L29+20
	str	r3, [r0]
	str	r0, [r4]
	movs	r0, #0
	b	.L24
.L30:
	.align	2
.L29:
	.word	.LANCHOR1+61
	.word	.LC2
	.word	.LANCHOR0
	.word	.LANCHOR1+76
	.word	.LC3
	.word	.LC0
	.fnend
	.size	mpp_frame_init, .-mpp_frame_init
	.align	1
	.global	mpp_frame_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_deinit, %function
mpp_frame_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	cbnz	r0, .L32
.L34:
	movs	r2, #90
	ldr	r1, .L53
	ldr	r0, .L53+4
	mvn	r6, #2
	bl	printk
.L31:
	mov	r0, r6
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L32:
	ldr	r0, [r0]
	bl	check_is_mpp_frame
	mov	r6, r0
	cmp	r0, #0
	bne	.L34
	ldr	r7, [r4]
	ldr	r0, [r7, #136]
	cbz	r0, .L35
	ldr	r1, .L53+8
	bl	mpp_buffer_put_with_caller
.L35:
	ldr	r9, [r7, #152]
	cmp	r9, #0
	beq	.L36
	ldr	r10, .L53+8
	mov	r5, r9
	ldr	fp, .L53+16
	mov	r8, #0
.L37:
	ldr	r3, [r9, #4]
	cmp	r3, r8
	bhi	.L40
.L36:
	ldr	r3, .L53+12
	mov	r2, r7
	ldr	r0, .L53+8
	ldr	r1, [r3]
	bl	mpp_mem_pool_put_f
	movs	r3, #0
	str	r3, [r4]
	b	.L31
.L40:
	ldr	r0, [r5, #136]
	cbz	r0, .L38
	mov	r2, r10
	mov	r1, fp
	bl	mpi_buf_unref_with_tag
.L38:
	ldr	r0, [r5, #88]
	cbz	r0, .L39
	mov	r2, r10
	mov	r1, fp
	bl	mpi_buf_unref_with_tag
.L39:
	add	r8, r8, #1
	adds	r5, r5, #140
	b	.L37
.L54:
	.align	2
.L53:
	.word	.LANCHOR1+91
	.word	.LC2
	.word	.LANCHOR1+108
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	mpp_frame_deinit, .-mpp_frame_deinit
	.align	1
	.global	mpp_frame_get_next
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_next, %function
mpp_frame_get_next:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	cbnz	r0, .L57
	ldr	r0, [r4, #172]
.L55:
	pop	{r4, pc}
.L57:
	movs	r0, #0
	b	.L55
	.fnend
	.size	mpp_frame_get_next, .-mpp_frame_get_next
	.align	1
	.global	mpp_frame_set_next
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_next, %function
mpp_frame_set_next:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	cbnz	r0, .L60
	str	r5, [r4, #172]
.L58:
	pop	{r3, r4, r5, pc}
.L60:
	mvn	r0, #1
	b	.L58
	.fnend
	.size	mpp_frame_set_next, .-mpp_frame_set_next
	.align	1
	.global	mpp_frame_get_buffer
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_buffer, %function
mpp_frame_get_buffer:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	cbnz	r0, .L63
	ldr	r0, [r4, #136]
.L61:
	pop	{r4, pc}
.L63:
	movs	r0, #0
	b	.L61
	.fnend
	.size	mpp_frame_get_buffer, .-mpp_frame_get_buffer
	.align	1
	.global	mpp_frame_set_buffer
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_buffer, %function
mpp_frame_set_buffer:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	mov	r4, r1
	bl	check_is_mpp_frame
	cbnz	r0, .L64
	ldr	r3, [r5, #136]
	cmp	r3, r4
	beq	.L64
	cbz	r4, .L66
	ldr	r1, .L74
	mov	r0, r4
	bl	mpp_buffer_inc_ref_with_caller
.L66:
	ldr	r0, [r5, #136]
	cbz	r0, .L67
	ldr	r1, .L74
	bl	mpp_buffer_put_with_caller
.L67:
	str	r4, [r5, #136]
.L64:
	pop	{r3, r4, r5, pc}
.L75:
	.align	2
.L74:
	.word	.LANCHOR1+125
	.fnend
	.size	mpp_frame_set_buffer, .-mpp_frame_set_buffer
	.align	1
	.global	mpp_frame_has_meta
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_has_meta, %function
mpp_frame_has_meta:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	check_is_mpp_frame
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	mpp_frame_has_meta, .-mpp_frame_has_meta
	.align	1
	.global	mpp_frame_add_roi
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_add_roi, %function
mpp_frame_add_roi:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	mov	r4, r1
	bl	check_is_mpp_frame
	cbnz	r0, .L80
	cbz	r4, .L80
	str	r4, [r5, #148]
.L77:
	pop	{r3, r4, r5, pc}
.L80:
	mvn	r0, #2
	b	.L77
	.fnend
	.size	mpp_frame_add_roi, .-mpp_frame_add_roi
	.align	1
	.global	mpp_frame_get_roi
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_roi, %function
mpp_frame_get_roi:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	cbnz	r0, .L83
	ldr	r0, [r4, #148]
.L81:
	pop	{r4, pc}
.L83:
	movs	r0, #0
	b	.L81
	.fnend
	.size	mpp_frame_get_roi, .-mpp_frame_get_roi
	.align	1
	.global	mpp_frame_add_osd
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_add_osd, %function
mpp_frame_add_osd:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r6, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	mov	r4, r0
	cbnz	r0, .L91
	cbz	r5, .L91
	ldr	r8, .L98
	mov	r7, r0
	ldr	r9, .L98+4
	str	r5, [r6, #152]
	mov	r6, r5
.L86:
	ldr	r3, [r5, #4]
	cmp	r3, r7
	bhi	.L89
.L84:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L89:
	ldr	r0, [r6, #136]
	cbz	r0, .L87
	mov	r2, r8
	mov	r1, r9
	bl	mpi_buf_ref_with_tag
.L87:
	ldr	r0, [r6, #88]
	cbz	r0, .L88
	mov	r2, r8
	mov	r1, r9
	bl	mpi_buf_ref_with_tag
.L88:
	adds	r7, r7, #1
	adds	r6, r6, #140
	b	.L86
.L91:
	mvn	r4, #2
	b	.L84
.L99:
	.align	2
.L98:
	.word	.LANCHOR1+146
	.word	.LC0
	.fnend
	.size	mpp_frame_add_osd, .-mpp_frame_add_osd
	.align	1
	.global	mpp_frame_get_osd
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_osd, %function
mpp_frame_get_osd:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	cbnz	r0, .L102
	ldr	r0, [r4, #152]
.L100:
	pop	{r4, pc}
.L102:
	movs	r0, #0
	b	.L100
	.fnend
	.size	mpp_frame_get_osd, .-mpp_frame_get_osd
	.align	1
	.global	mpp_frame_add_ppinfo
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_add_ppinfo, %function
mpp_frame_add_ppinfo:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	mov	r4, r1
	bl	check_is_mpp_frame
	cbnz	r0, .L106
	cbz	r4, .L106
	str	r4, [r5, #168]
.L103:
	pop	{r3, r4, r5, pc}
.L106:
	mvn	r0, #2
	b	.L103
	.fnend
	.size	mpp_frame_add_ppinfo, .-mpp_frame_add_ppinfo
	.align	1
	.global	mpp_frame_get_ppinfo
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_ppinfo, %function
mpp_frame_get_ppinfo:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	cbnz	r0, .L109
	ldr	r0, [r4, #168]
.L107:
	pop	{r4, pc}
.L109:
	movs	r0, #0
	b	.L107
	.fnend
	.size	mpp_frame_get_ppinfo, .-mpp_frame_get_ppinfo
	.align	1
	.global	mpp_frame_set_stopwatch_enable
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_stopwatch_enable, %function
mpp_frame_set_stopwatch_enable:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	check_is_mpp_frame
	.fnend
	.size	mpp_frame_set_stopwatch_enable, .-mpp_frame_set_stopwatch_enable
	.align	1
	.global	mpp_frame_get_stopwatch
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_stopwatch, %function
mpp_frame_get_stopwatch:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	check_is_mpp_frame
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	mpp_frame_get_stopwatch, .-mpp_frame_get_stopwatch
	.align	1
	.global	mpp_frame_copy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_copy, %function
mpp_frame_copy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r6, r1
	mov	r5, r0
	cbnz	r0, .L113
.L115:
	str	r6, [sp]
	mov	r3, r5
	movw	r2, #303
	ldr	r1, .L116
	ldr	r0, .L116+4
	mvn	r4, #1
	bl	printk
.L112:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L113:
	mov	r0, r1
	bl	check_is_mpp_frame
	mov	r4, r0
	cmp	r0, #0
	bne	.L115
	movs	r2, #184
	mov	r1, r6
	mov	r0, r5
	bl	memcpy
	str	r4, [r5, #152]
	b	.L112
.L117:
	.align	2
.L116:
	.word	.LANCHOR1+164
	.word	.LC4
	.fnend
	.size	mpp_frame_copy, .-mpp_frame_copy
	.align	1
	.global	mpp_frame_info_cmp
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_info_cmp, %function
mpp_frame_info_cmp:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	cbz	r0, .L119
.L121:
	mov	r2, #338
	ldr	r1, .L127
	ldr	r0, .L127+4
	bl	printk
	mvn	r0, #2
.L118:
	pop	{r3, r4, r5, pc}
.L119:
	mov	r0, r4
	bl	check_is_mpp_frame
	cmp	r0, #0
	bne	.L121
	ldr	r2, [r4, #4]
	ldr	r3, [r5, #4]
	cmp	r2, r3
	bne	.L126
	ldr	r2, [r4, #8]
	ldr	r3, [r5, #8]
	cmp	r2, r3
	bne	.L126
	ldr	r2, [r4, #12]
	ldr	r3, [r5, #12]
	cmp	r2, r3
	bne	.L126
	ldr	r2, [r4, #16]
	ldr	r3, [r5, #16]
	cmp	r2, r3
	bne	.L126
	ldr	r2, [r4, #96]
	ldr	r3, [r5, #96]
	cmp	r2, r3
	bne	.L126
	ldr	r0, [r4, #140]
	ldr	r3, [r5, #140]
	subs	r0, r0, r3
	it	ne
	movne	r0, #-1
	b	.L118
.L126:
	mov	r0, #-1
	b	.L118
.L128:
	.align	2
.L127:
	.word	.LANCHOR1+179
	.word	.LC2
	.fnend
	.size	mpp_frame_info_cmp, .-mpp_frame_info_cmp
	.align	1
	.global	mpp_frame_get_fbc_offset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_fbc_offset, %function
mpp_frame_get_fbc_offset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	cbnz	r0, .L133
	ldr	r3, [r4, #96]
	ands	r3, r3, #15728640
	beq	.L131
	cmp	r3, #1048576
	bne	.L132
	ldr	r0, [r4, #4]
	ldr	r3, [r4, #8]
	adds	r0, r0, #15
	bic	r2, r0, #15
	add	r0, r3, #15
	bic	r0, r0, #15
	muls	r0, r2, r0
	lsrs	r0, r0, #4
	addw	r0, r0, #4095
	bic	r0, r0, #4080
	bic	r0, r0, #15
.L132:
	str	r0, [r4, #144]
.L131:
	ldr	r0, [r4, #144]
.L129:
	pop	{r4, pc}
.L133:
	movs	r0, #0
	b	.L129
	.fnend
	.size	mpp_frame_get_fbc_offset, .-mpp_frame_get_fbc_offset
	.align	1
	.global	mpp_frame_get_fbc_stride
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_fbc_stride, %function
mpp_frame_get_fbc_stride:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	cbnz	r0, .L140
	ldr	r0, [r4, #4]
	adds	r0, r0, #15
	bic	r0, r0, #15
.L138:
	pop	{r4, pc}
.L140:
	movs	r0, #0
	b	.L138
	.fnend
	.size	mpp_frame_get_fbc_stride, .-mpp_frame_get_fbc_stride
	.align	1
	.global	mpp_frame_get_width
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_width, %function
mpp_frame_get_width:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #4]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_width, .-mpp_frame_get_width
	.align	1
	.global	mpp_frame_set_width
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_width, %function
mpp_frame_set_width:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #4]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_width, .-mpp_frame_set_width
	.align	1
	.global	mpp_frame_get_height
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_height, %function
mpp_frame_get_height:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #8]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_height, .-mpp_frame_get_height
	.align	1
	.global	mpp_frame_set_height
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_height, %function
mpp_frame_set_height:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #8]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_height, .-mpp_frame_set_height
	.align	1
	.global	mpp_frame_get_hor_stride_pixel
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_hor_stride_pixel, %function
mpp_frame_get_hor_stride_pixel:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #20]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_hor_stride_pixel, .-mpp_frame_get_hor_stride_pixel
	.align	1
	.global	mpp_frame_set_hor_stride_pixel
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_hor_stride_pixel, %function
mpp_frame_set_hor_stride_pixel:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #20]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_hor_stride_pixel, .-mpp_frame_set_hor_stride_pixel
	.align	1
	.global	mpp_frame_get_hor_stride
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_hor_stride, %function
mpp_frame_get_hor_stride:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #12]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_hor_stride, .-mpp_frame_get_hor_stride
	.align	1
	.global	mpp_frame_set_hor_stride
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_hor_stride, %function
mpp_frame_set_hor_stride:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #12]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_hor_stride, .-mpp_frame_set_hor_stride
	.align	1
	.global	mpp_frame_get_ver_stride
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_ver_stride, %function
mpp_frame_get_ver_stride:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #16]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_ver_stride, .-mpp_frame_get_ver_stride
	.align	1
	.global	mpp_frame_set_ver_stride
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_ver_stride, %function
mpp_frame_set_ver_stride:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #16]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_ver_stride, .-mpp_frame_set_ver_stride
	.align	1
	.global	mpp_frame_get_offset_x
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_offset_x, %function
mpp_frame_get_offset_x:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #24]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_offset_x, .-mpp_frame_get_offset_x
	.align	1
	.global	mpp_frame_set_offset_x
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_offset_x, %function
mpp_frame_set_offset_x:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #24]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_offset_x, .-mpp_frame_set_offset_x
	.align	1
	.global	mpp_frame_get_offset_y
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_offset_y, %function
mpp_frame_get_offset_y:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #28]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_offset_y, .-mpp_frame_get_offset_y
	.align	1
	.global	mpp_frame_set_offset_y
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_offset_y, %function
mpp_frame_set_offset_y:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #28]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_offset_y, .-mpp_frame_set_offset_y
	.align	1
	.global	mpp_frame_get_mode
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_mode, %function
mpp_frame_get_mode:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #32]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_mode, .-mpp_frame_get_mode
	.align	1
	.global	mpp_frame_set_mode
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_mode, %function
mpp_frame_set_mode:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #32]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_mode, .-mpp_frame_set_mode
	.align	1
	.global	mpp_frame_get_discard
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_discard, %function
mpp_frame_get_discard:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #36]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_discard, .-mpp_frame_get_discard
	.align	1
	.global	mpp_frame_set_discard
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_discard, %function
mpp_frame_set_discard:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #36]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_discard, .-mpp_frame_set_discard
	.align	1
	.global	mpp_frame_get_viewid
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_viewid, %function
mpp_frame_get_viewid:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #40]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_viewid, .-mpp_frame_get_viewid
	.align	1
	.global	mpp_frame_set_viewid
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_viewid, %function
mpp_frame_set_viewid:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #40]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_viewid, .-mpp_frame_set_viewid
	.align	1
	.global	mpp_frame_get_poc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_poc, %function
mpp_frame_get_poc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #44]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_poc, .-mpp_frame_get_poc
	.align	1
	.global	mpp_frame_set_poc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_poc, %function
mpp_frame_set_poc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #44]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_poc, .-mpp_frame_set_poc
	.align	1
	.global	mpp_frame_get_pts
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_pts, %function
mpp_frame_get_pts:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldrd	r0, [r4, #48]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_pts, .-mpp_frame_get_pts
	.align	1
	.global	mpp_frame_set_pts
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_pts, %function
mpp_frame_set_pts:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r6, r7, lr}
	.save {r4, r6, r7, lr}
	mov	r4, r0
	mov	r6, r2
	mov	r7, r3
	bl	check_is_mpp_frame
	strd	r6, [r4, #48]
	pop	{r4, r6, r7, pc}
	.fnend
	.size	mpp_frame_set_pts, .-mpp_frame_set_pts
	.align	1
	.global	mpp_frame_get_dts
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_dts, %function
mpp_frame_get_dts:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldrd	r0, [r4, #56]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_dts, .-mpp_frame_get_dts
	.align	1
	.global	mpp_frame_set_dts
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_dts, %function
mpp_frame_set_dts:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r6, r7, lr}
	.save {r4, r6, r7, lr}
	mov	r4, r0
	mov	r6, r2
	mov	r7, r3
	bl	check_is_mpp_frame
	strd	r6, [r4, #56]
	pop	{r4, r6, r7, pc}
	.fnend
	.size	mpp_frame_set_dts, .-mpp_frame_set_dts
	.align	1
	.global	mpp_frame_get_eos
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_eos, %function
mpp_frame_get_eos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #64]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_eos, .-mpp_frame_get_eos
	.align	1
	.global	mpp_frame_set_eos
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_eos, %function
mpp_frame_set_eos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #64]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_eos, .-mpp_frame_set_eos
	.align	1
	.global	mpp_frame_get_info_change
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_info_change, %function
mpp_frame_get_info_change:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #68]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_info_change, .-mpp_frame_get_info_change
	.align	1
	.global	mpp_frame_set_info_change
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_info_change, %function
mpp_frame_set_info_change:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #68]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_info_change, .-mpp_frame_set_info_change
	.align	1
	.global	mpp_frame_get_color_range
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_color_range, %function
mpp_frame_get_color_range:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #76]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_color_range, .-mpp_frame_get_color_range
	.align	1
	.global	mpp_frame_set_color_range
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_color_range, %function
mpp_frame_set_color_range:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #76]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_color_range, .-mpp_frame_set_color_range
	.align	1
	.global	mpp_frame_get_color_primaries
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_color_primaries, %function
mpp_frame_get_color_primaries:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #80]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_color_primaries, .-mpp_frame_get_color_primaries
	.align	1
	.global	mpp_frame_set_color_primaries
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_color_primaries, %function
mpp_frame_set_color_primaries:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #80]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_color_primaries, .-mpp_frame_set_color_primaries
	.align	1
	.global	mpp_frame_get_color_trc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_color_trc, %function
mpp_frame_get_color_trc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #84]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_color_trc, .-mpp_frame_get_color_trc
	.align	1
	.global	mpp_frame_set_color_trc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_color_trc, %function
mpp_frame_set_color_trc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #84]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_color_trc, .-mpp_frame_set_color_trc
	.align	1
	.global	mpp_frame_get_colorspace
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_colorspace, %function
mpp_frame_get_colorspace:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #88]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_colorspace, .-mpp_frame_get_colorspace
	.align	1
	.global	mpp_frame_set_colorspace
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_colorspace, %function
mpp_frame_set_colorspace:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #88]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_colorspace, .-mpp_frame_set_colorspace
	.align	1
	.global	mpp_frame_get_chroma_location
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_chroma_location, %function
mpp_frame_get_chroma_location:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #92]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_chroma_location, .-mpp_frame_get_chroma_location
	.align	1
	.global	mpp_frame_set_chroma_location
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_chroma_location, %function
mpp_frame_set_chroma_location:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #92]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_chroma_location, .-mpp_frame_set_chroma_location
	.align	1
	.global	mpp_frame_get_fmt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_fmt, %function
mpp_frame_get_fmt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #96]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_fmt, .-mpp_frame_get_fmt
	.align	1
	.global	mpp_frame_set_fmt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_fmt, %function
mpp_frame_set_fmt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #96]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_fmt, .-mpp_frame_set_fmt
	.align	1
	.global	mpp_frame_get_sar
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_sar, %function
mpp_frame_get_sar:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	mov	r0, r1
	mov	r4, r1
	bl	check_is_mpp_frame
	adds	r4, r4, #100
	ldm	r4, {r0, r1}
	stm	r5, {r0, r1}
	mov	r0, r5
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_get_sar, .-mpp_frame_get_sar
	.align	1
	.global	mpp_frame_set_sar
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_sar, %function
mpp_frame_set_sar:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r5, sp
	mov	r4, r0
	stm	r5, {r1, r2}
	bl	check_is_mpp_frame
	ldm	r5, {r0, r1}
	add	r3, r4, #100
	stm	r3, {r0, r1}
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
	.fnend
	.size	mpp_frame_set_sar, .-mpp_frame_set_sar
	.align	1
	.global	mpp_frame_get_mastering_display
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_mastering_display, %function
mpp_frame_get_mastering_display:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	mov	r0, r1
	mov	r4, r1
	bl	check_is_mpp_frame
	adds	r4, r4, #108
	mov	r5, r6
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldm	r4, {r0, r1}
	stm	r5, {r0, r1}
	mov	r0, r6
	pop	{r4, r5, r6, pc}
	.fnend
	.size	mpp_frame_get_mastering_display, .-mpp_frame_get_mastering_display
	.align	1
	.global	mpp_frame_set_mastering_display
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_mastering_display, %function
mpp_frame_set_mastering_display:
	.fnstart
	@ args = 28, pretend = 16, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.pad #16
	sub	sp, sp, #16
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	add	r4, sp, #20
	mov	r5, r0
	adds	r5, r5, #108
	stm	r4, {r1, r2, r3}
	bl	check_is_mpp_frame
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldm	r4, {r0, r1}
	stm	r5, {r0, r1}
	pop	{r3, r4, r5, lr}
	add	sp, sp, #16
	bx	lr
	.fnend
	.size	mpp_frame_set_mastering_display, .-mpp_frame_set_mastering_display
	.align	1
	.global	mpp_frame_get_content_light
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_content_light, %function
mpp_frame_get_content_light:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r3, [r4, #132]
	movs	r0, #0
	uxth	r2, r3
	lsrs	r3, r3, #16
	bfi	r0, r2, #0, #16
	bfi	r0, r3, #16, #16
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_content_light, .-mpp_frame_get_content_light
	.align	1
	.global	mpp_frame_set_content_light
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_content_light, %function
mpp_frame_set_content_light:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r0
	str	r1, [sp, #4]
	bl	check_is_mpp_frame
	ldr	r3, [sp, #4]
	str	r3, [r4, #132]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_set_content_light, .-mpp_frame_set_content_light
	.align	1
	.global	mpp_frame_get_buf_size
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_buf_size, %function
mpp_frame_get_buf_size:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #140]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_buf_size, .-mpp_frame_get_buf_size
	.align	1
	.global	mpp_frame_set_buf_size
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_buf_size, %function
mpp_frame_set_buf_size:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #140]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_buf_size, .-mpp_frame_set_buf_size
	.align	1
	.global	mpp_frame_get_errinfo
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_errinfo, %function
mpp_frame_get_errinfo:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #72]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_errinfo, .-mpp_frame_get_errinfo
	.align	1
	.global	mpp_frame_set_errinfo
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_errinfo, %function
mpp_frame_set_errinfo:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #72]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_errinfo, .-mpp_frame_set_errinfo
	.align	1
	.global	mpp_frame_get_is_gray
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_is_gray, %function
mpp_frame_get_is_gray:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #156]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_is_gray, .-mpp_frame_get_is_gray
	.align	1
	.global	mpp_frame_set_is_gray
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_is_gray, %function
mpp_frame_set_is_gray:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #156]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_is_gray, .-mpp_frame_set_is_gray
	.align	1
	.global	mpp_frame_get_is_full
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_is_full, %function
mpp_frame_get_is_full:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #160]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_is_full, .-mpp_frame_get_is_full
	.align	1
	.global	mpp_frame_set_is_full
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_is_full, %function
mpp_frame_set_is_full:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #160]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_is_full, .-mpp_frame_set_is_full
	.align	1
	.global	mpp_frame_get_phy_addr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_phy_addr, %function
mpp_frame_get_phy_addr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #164]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_phy_addr, .-mpp_frame_get_phy_addr
	.align	1
	.global	mpp_frame_set_phy_addr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_phy_addr, %function
mpp_frame_set_phy_addr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #164]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_phy_addr, .-mpp_frame_set_phy_addr
	.align	1
	.global	mpp_frame_get_idr_request
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_get_idr_request, %function
mpp_frame_get_idr_request:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_frame
	ldr	r0, [r4, #176]
	pop	{r4, pc}
	.fnend
	.size	mpp_frame_get_idr_request, .-mpp_frame_get_idr_request
	.align	1
	.global	mpp_frame_set_idr_request
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_frame_set_idr_request, %function
mpp_frame_set_idr_request:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_frame
	str	r5, [r4, #176]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_frame_set_idr_request, .-mpp_frame_set_idr_request
	.section	.rodata
	.set	.LANCHOR1,. + 0
	.type	__FUNCTION__.8743, %object
	.size	__FUNCTION__.8743, 20
__FUNCTION__.8743:
	.ascii	"mpp_frame_pool_init\000"
	.type	__FUNCTION__.8747, %object
	.size	__FUNCTION__.8747, 22
__FUNCTION__.8747:
	.ascii	"mpp_frame_pool_deinit\000"
	.type	__func__.8757, %object
	.size	__func__.8757, 19
__func__.8757:
	.ascii	"check_is_mpp_frame\000"
	.type	__func__.8764, %object
	.size	__func__.8764, 15
__func__.8764:
	.ascii	"mpp_frame_init\000"
	.type	__FUNCTION__.8765, %object
	.size	__FUNCTION__.8765, 15
__FUNCTION__.8765:
	.ascii	"mpp_frame_init\000"
	.type	__func__.8770, %object
	.size	__func__.8770, 17
__func__.8770:
	.ascii	"mpp_frame_deinit\000"
	.type	__FUNCTION__.8771, %object
	.size	__FUNCTION__.8771, 17
__FUNCTION__.8771:
	.ascii	"mpp_frame_deinit\000"
	.type	__FUNCTION__.8795, %object
	.size	__FUNCTION__.8795, 21
__FUNCTION__.8795:
	.ascii	"mpp_frame_set_buffer\000"
	.type	__FUNCTION__.8816, %object
	.size	__FUNCTION__.8816, 18
__FUNCTION__.8816:
	.ascii	"mpp_frame_add_osd\000"
	.type	__func__.8845, %object
	.size	__func__.8845, 15
__func__.8845:
	.ascii	"mpp_frame_copy\000"
	.type	__func__.8852, %object
	.size	__func__.8852, 19
__func__.8852:
	.ascii	"mpp_frame_info_cmp\000"
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_frame_pool, %object
	.size	g_frame_pool, 4
g_frame_pool:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"mpp_frame\000"
.LC1:
	.ascii	"\0013%s:%d: pointer %p failed on check\012\000"
.LC2:
	.ascii	"\0013%s:%d: invalid NULL pointer input\012\000"
.LC3:
	.ascii	"\0013%s:%d: malloc failed\012\000"
.LC4:
	.ascii	"\0013%s:%d: invalid input dst %p src %p\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
