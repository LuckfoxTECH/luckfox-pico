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
	cbz	r0, .L2
	ldr	r1, [r0]
	ldr	r2, .L8
	cmp	r1, r2
	beq	.L4
.L2:
	movs	r2, #30
	ldr	r1, .L8+4
	ldr	r0, .L8+8
	bl	printk
	mov	r0, #-1
.L1:
	pop	{r3, pc}
.L4:
	movs	r0, #0
	b	.L1
.L9:
	.align	2
.L8:
	.word	.LC0
	.word	.LANCHOR0
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
	cbnz	r0, .L11
	ldr	r1, .L15
	movs	r2, #40
	ldr	r0, .L15+4
.L14:
	bl	printk
	mvn	r0, #2
.L10:
	pop	{r4, pc}
.L11:
	movs	r1, #176
	ldr	r0, .L15+8
	bl	mpp_osal_calloc
	cbnz	r0, .L13
	movs	r2, #46
	ldr	r1, .L15
	ldr	r0, .L15+12
	b	.L14
.L13:
	ldr	r3, .L15+16
	str	r3, [r0]
	str	r0, [r4]
	movs	r0, #0
	b	.L10
.L16:
	.align	2
.L15:
	.word	.LANCHOR0+19
	.word	.LC2
	.word	.LANCHOR0+34
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
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r4, r0
	cbnz	r0, .L18
.L20:
	movs	r2, #61
	ldr	r1, .L39
	ldr	r0, .L39+4
	mvn	r6, #2
	bl	printk
.L17:
	mov	r0, r6
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L18:
	ldr	r0, [r0]
	bl	check_is_mpp_frame
	mov	r6, r0
	cmp	r0, #0
	bne	.L20
	ldr	r5, [r4]
	ldr	r0, [r5, #136]
	cbz	r0, .L21
	ldr	r1, .L39+8
	bl	mpp_buffer_put_with_caller
.L21:
	ldr	r8, [r5, #152]
	cmp	r8, #0
	beq	.L22
	ldr	r9, .L39+8
	mov	r5, r8
	ldr	r10, .L39+12
	movs	r7, #0
.L23:
	ldr	r3, [r8, #4]
	cmp	r3, r7
	bhi	.L26
.L22:
	ldr	r1, [r4]
	ldr	r0, .L39+8
	bl	mpp_osal_free
	movs	r3, #0
	str	r3, [r4]
	b	.L17
.L26:
	ldr	r0, [r5, #136]
	cbz	r0, .L24
	mov	r2, r9
	mov	r1, r10
	bl	mpi_buf_unref_with_tag
.L24:
	ldr	r0, [r5, #88]
	cbz	r0, .L25
	mov	r2, r9
	mov	r1, r10
	bl	mpi_buf_unref_with_tag
.L25:
	adds	r7, r7, #1
	adds	r5, r5, #140
	b	.L23
.L40:
	.align	2
.L39:
	.word	.LANCHOR0+49
	.word	.LC2
	.word	.LANCHOR0+66
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
	cbnz	r0, .L43
	ldr	r0, [r4, #172]
.L41:
	pop	{r4, pc}
.L43:
	movs	r0, #0
	b	.L41
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
	cbnz	r0, .L46
	str	r5, [r4, #172]
.L44:
	pop	{r3, r4, r5, pc}
.L46:
	mvn	r0, #1
	b	.L44
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
	cbnz	r0, .L49
	ldr	r0, [r4, #136]
.L47:
	pop	{r4, pc}
.L49:
	movs	r0, #0
	b	.L47
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
	cbnz	r0, .L50
	ldr	r3, [r5, #136]
	cmp	r3, r4
	beq	.L50
	cbz	r4, .L52
	ldr	r1, .L60
	mov	r0, r4
	bl	mpp_buffer_inc_ref_with_caller
.L52:
	ldr	r0, [r5, #136]
	cbz	r0, .L53
	ldr	r1, .L60
	bl	mpp_buffer_put_with_caller
.L53:
	str	r4, [r5, #136]
.L50:
	pop	{r3, r4, r5, pc}
.L61:
	.align	2
.L60:
	.word	.LANCHOR0+83
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
	cbnz	r0, .L66
	cbz	r4, .L66
	str	r4, [r5, #148]
.L63:
	pop	{r3, r4, r5, pc}
.L66:
	mvn	r0, #2
	b	.L63
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
	cbnz	r0, .L69
	ldr	r0, [r4, #148]
.L67:
	pop	{r4, pc}
.L69:
	movs	r0, #0
	b	.L67
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
	cbnz	r0, .L77
	cbz	r5, .L77
	ldr	r8, .L84
	mov	r7, r0
	ldr	r9, .L84+4
	str	r5, [r6, #152]
	mov	r6, r5
.L72:
	ldr	r3, [r5, #4]
	cmp	r3, r7
	bhi	.L75
.L70:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L75:
	ldr	r0, [r6, #136]
	cbz	r0, .L73
	mov	r2, r8
	mov	r1, r9
	bl	mpi_buf_ref_with_tag
.L73:
	ldr	r0, [r6, #88]
	cbz	r0, .L74
	mov	r2, r8
	mov	r1, r9
	bl	mpi_buf_ref_with_tag
.L74:
	adds	r7, r7, #1
	adds	r6, r6, #140
	b	.L72
.L77:
	mvn	r4, #2
	b	.L70
.L85:
	.align	2
.L84:
	.word	.LANCHOR0+104
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
	cbnz	r0, .L88
	ldr	r0, [r4, #152]
.L86:
	pop	{r4, pc}
.L88:
	movs	r0, #0
	b	.L86
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
	cbnz	r0, .L92
	cbz	r4, .L92
	str	r4, [r5, #168]
.L89:
	pop	{r3, r4, r5, pc}
.L92:
	mvn	r0, #2
	b	.L89
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
	cbnz	r0, .L95
	ldr	r0, [r4, #168]
.L93:
	pop	{r4, pc}
.L95:
	movs	r0, #0
	b	.L93
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
	cbnz	r0, .L99
.L101:
	str	r6, [sp]
	mov	r3, r5
	movw	r2, #261
	ldr	r1, .L102
	ldr	r0, .L102+4
	mvn	r4, #1
	bl	printk
.L98:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L99:
	mov	r0, r1
	bl	check_is_mpp_frame
	mov	r4, r0
	cmp	r0, #0
	bne	.L101
	movs	r2, #176
	mov	r1, r6
	mov	r0, r5
	bl	memcpy
	str	r4, [r5, #152]
	b	.L98
.L103:
	.align	2
.L102:
	.word	.LANCHOR0+122
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
	cbz	r0, .L105
.L107:
	movw	r2, #295
	ldr	r1, .L113
	ldr	r0, .L113+4
	bl	printk
	mvn	r0, #2
.L104:
	pop	{r3, r4, r5, pc}
.L105:
	mov	r0, r4
	bl	check_is_mpp_frame
	cmp	r0, #0
	bne	.L107
	ldr	r2, [r4, #4]
	ldr	r3, [r5, #4]
	cmp	r2, r3
	bne	.L112
	ldr	r2, [r4, #8]
	ldr	r3, [r5, #8]
	cmp	r2, r3
	bne	.L112
	ldr	r2, [r4, #12]
	ldr	r3, [r5, #12]
	cmp	r2, r3
	bne	.L112
	ldr	r2, [r4, #16]
	ldr	r3, [r5, #16]
	cmp	r2, r3
	bne	.L112
	ldr	r2, [r4, #96]
	ldr	r3, [r5, #96]
	cmp	r2, r3
	bne	.L112
	ldr	r0, [r4, #140]
	ldr	r3, [r5, #140]
	subs	r0, r0, r3
	it	ne
	movne	r0, #-1
	b	.L104
.L112:
	mov	r0, #-1
	b	.L104
.L114:
	.align	2
.L113:
	.word	.LANCHOR0+137
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
	cbnz	r0, .L119
	ldr	r3, [r4, #96]
	ands	r3, r3, #15728640
	beq	.L117
	cmp	r3, #1048576
	bne	.L118
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
.L118:
	str	r0, [r4, #144]
.L117:
	ldr	r0, [r4, #144]
.L115:
	pop	{r4, pc}
.L119:
	movs	r0, #0
	b	.L115
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
	cbnz	r0, .L126
	ldr	r0, [r4, #4]
	adds	r0, r0, #15
	bic	r0, r0, #15
.L124:
	pop	{r4, pc}
.L126:
	movs	r0, #0
	b	.L124
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
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.8675, %object
	.size	__func__.8675, 19
__func__.8675:
	.ascii	"check_is_mpp_frame\000"
	.type	__func__.8682, %object
	.size	__func__.8682, 15
__func__.8682:
	.ascii	"mpp_frame_init\000"
	.type	__FUNCTION__.8683, %object
	.size	__FUNCTION__.8683, 15
__FUNCTION__.8683:
	.ascii	"mpp_frame_init\000"
	.type	__func__.8688, %object
	.size	__func__.8688, 17
__func__.8688:
	.ascii	"mpp_frame_deinit\000"
	.type	__FUNCTION__.8689, %object
	.size	__FUNCTION__.8689, 17
__FUNCTION__.8689:
	.ascii	"mpp_frame_deinit\000"
	.type	__FUNCTION__.8713, %object
	.size	__FUNCTION__.8713, 21
__FUNCTION__.8713:
	.ascii	"mpp_frame_set_buffer\000"
	.type	__FUNCTION__.8734, %object
	.size	__FUNCTION__.8734, 18
__FUNCTION__.8734:
	.ascii	"mpp_frame_add_osd\000"
	.type	__func__.8763, %object
	.size	__func__.8763, 15
__func__.8763:
	.ascii	"mpp_frame_copy\000"
	.type	__func__.8770, %object
	.size	__func__.8770, 19
__func__.8770:
	.ascii	"mpp_frame_info_cmp\000"
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
