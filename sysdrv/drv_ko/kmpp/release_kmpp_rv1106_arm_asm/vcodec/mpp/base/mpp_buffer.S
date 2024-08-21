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
	.file	"mpp_buffer.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_buffer_pool_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_pool_init, %function
mpp_buffer_pool_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r3, r0
	ldr	r4, .L3
	ldr	r2, [r4]
	cbnz	r2, .L2
	movs	r2, #52
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
	.size	mpp_buffer_pool_init, .-mpp_buffer_pool_init
	.align	1
	.global	mpp_buffer_pool_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_pool_deinit, %function
mpp_buffer_pool_deinit:
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
	.word	.LANCHOR1+21
	.fnend
	.size	mpp_buffer_pool_deinit, .-mpp_buffer_pool_deinit
	.align	1
	.global	mpp_buf_pool_info_show
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buf_pool_info_show, %function
mpp_buf_pool_info_show:
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
	.size	mpp_buf_pool_info_show, .-mpp_buf_pool_info_show
	.align	1
	.global	mpp_buffer_import_with_tag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_import_with_tag, %function
mpp_buffer_import_with_tag:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r1
	mov	r7, r2
	bl	get_mpibuf_ops
	mov	r8, r0
	cbnz	r0, .L16
	movs	r2, #73
	ldr	r1, .L30
	ldr	r0, .L30+4
	bl	printk
	mov	r0, #-1
.L15:
	pop	{r4, r5, r6, r7, r8, pc}
.L16:
	cbnz	r4, .L18
	ldr	r2, [sp, #24]
	movs	r1, #79
	ldr	r0, .L30+8
.L29:
	bl	printk
	mvn	r0, #2
	b	.L15
.L18:
	cbz	r7, .L22
	ldr	r3, .L30+12
	ldr	r0, .L30+16
	ldr	r1, [r3]
	bl	mpp_mem_pool_get_f
	mov	r5, r0
	cbnz	r0, .L19
	ldr	r2, [sp, #24]
	movs	r1, #87
	ldr	r0, .L30+20
	b	.L29
.L19:
	mov	r6, r0
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r6!, {r0, r1, r2, r3}
	ldm	r4, {r0, r1, r2, r3}
	stm	r6, {r0, r1, r2, r3}
	ldr	r0, [r5, #12]
	str	r0, [r5, #32]
	ldr	r3, [r8, #20]
	cbz	r3, .L20
	blx	r3
	str	r0, [r5, #36]
.L20:
	ldr	r3, [r8, #12]
	cbz	r3, .L21
	ldr	r0, [r5, #32]
	blx	r3
.L21:
	ldr	r3, [r5, #48]
	movs	r0, #0
	adds	r3, r3, #1
	str	r3, [r5, #48]
	mov	r3, #-1
	str	r3, [r5, #16]
	str	r5, [r7]
	b	.L15
.L22:
	mov	r0, r7
	b	.L15
.L31:
	.align	2
.L30:
	.word	.LANCHOR1+44
	.word	.LC1
	.word	.LC2
	.word	.LANCHOR0
	.word	.LANCHOR1+71
	.word	.LC3
	.fnend
	.size	mpp_buffer_import_with_tag, .-mpp_buffer_import_with_tag
	.align	1
	.global	mpi_buf_alloc_with_tag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_alloc_with_tag, %function
mpi_buf_alloc_with_tag:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	get_mpibuf_ops
	cbnz	r0, .L33
	movs	r2, #113
	ldr	r1, .L37
	ldr	r0, .L37+4
	bl	printk
.L32:
	movs	r0, #0
	pop	{r4, pc}
.L33:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L32
	mov	r0, r4
	pop	{r4, lr}
	bx	r3
.L38:
	.align	2
.L37:
	.word	.LANCHOR1+98
	.word	.LC1
	.fnend
	.size	mpi_buf_alloc_with_tag, .-mpi_buf_alloc_with_tag
	.align	1
	.global	mpi_buf_ref_with_tag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_ref_with_tag, %function
mpi_buf_ref_with_tag:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	get_mpibuf_ops
	cbnz	r0, .L40
	movs	r2, #128
	ldr	r1, .L45
	ldr	r0, .L45+4
	bl	printk
	mov	r0, #-1
.L39:
	pop	{r4, pc}
.L40:
	cbz	r4, .L44
	ldr	r3, [r0, #12]
	cbz	r3, .L43
	mov	r0, r4
	blx	r3
.L44:
	movs	r0, #0
	b	.L39
.L43:
	mov	r0, r3
	b	.L39
.L46:
	.align	2
.L45:
	.word	.LANCHOR1+121
	.word	.LC1
	.fnend
	.size	mpi_buf_ref_with_tag, .-mpi_buf_ref_with_tag
	.align	1
	.global	mpi_buf_unref_with_tag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_unref_with_tag, %function
mpi_buf_unref_with_tag:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	get_mpibuf_ops
	cbnz	r0, .L48
	movs	r2, #145
	ldr	r1, .L53
	ldr	r0, .L53+4
	bl	printk
	mov	r0, #-1
.L47:
	pop	{r4, pc}
.L48:
	cbz	r4, .L52
	ldr	r3, [r0, #16]
	cbz	r3, .L51
	mov	r0, r4
	blx	r3
.L52:
	movs	r0, #0
	b	.L47
.L51:
	mov	r0, r3
	b	.L47
.L54:
	.align	2
.L53:
	.word	.LANCHOR1+142
	.word	.LC1
	.fnend
	.size	mpi_buf_unref_with_tag, .-mpi_buf_unref_with_tag
	.align	1
	.global	mpi_buf_get_dma_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_get_dma_with_caller, %function
mpi_buf_get_dma_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	get_mpibuf_ops
	cbnz	r0, .L56
	movs	r2, #162
	ldr	r1, .L63
	ldr	r0, .L63+4
	bl	printk
.L57:
	movs	r0, #0
	pop	{r4, pc}
.L56:
	cmp	r4, #0
	beq	.L57
	ldr	r3, [r0, #20]
	cmp	r3, #0
	beq	.L57
	mov	r0, r4
	pop	{r4, lr}
	bx	r3
.L64:
	.align	2
.L63:
	.word	.LANCHOR1+165
	.word	.LC1
	.fnend
	.size	mpi_buf_get_dma_with_caller, .-mpi_buf_get_dma_with_caller
	.align	1
	.global	mpp_buffer_get_with_tag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_get_with_tag, %function
mpp_buffer_get_with_tag:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r10, r0
	mov	r7, r1
	mov	r5, r2
	ldr	fp, [sp, #48]
	bl	get_mpibuf_ops
	mov	r8, r0
	cbnz	r0, .L66
	movs	r2, #184
	ldr	r1, .L82
	ldr	r0, .L82+4
	bl	printk
	mov	r0, #-1
.L65:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L66:
	ldr	r6, .L82+8
	ldr	r0, .L82+12
	ldr	r1, [r6]
	bl	mpp_mem_pool_get_f
	mov	r4, r0
	cbnz	r0, .L68
	strd	r5, fp, [sp]
	mov	r3, r7
	mov	r2, r10
	movs	r1, #190
	ldr	r0, .L82+16
	bl	printk
.L81:
	mvn	r0, #1
	b	.L65
.L68:
	ldr	r3, [r8]
	cbnz	r3, .L69
.L70:
	mov	r2, r10
	strd	r5, fp, [sp]
	mov	r3, r7
	movs	r1, #199
	ldr	r0, .L82+20
	bl	printk
	mov	r2, r4
	ldr	r1, [r6]
	ldr	r0, .L82+12
	bl	mpp_mem_pool_put_f
	b	.L81
.L69:
	mov	r0, r5
	blx	r3
	mov	r9, r0
	cmp	r0, #0
	beq	.L70
	cmp	r5, #0
	beq	.L70
	ldr	r3, [r8, #20]
	cbz	r3, .L71
	blx	r3
	str	r0, [r4, #36]
.L71:
	mov	r3, #-1
	str	r3, [r4, #16]
	ldr	r3, [r4, #48]
	movs	r0, #0
	str	r9, [r4, #32]
	str	r5, [r4, #4]
	adds	r3, r3, #1
	str	r9, [r4, #12]
	str	r3, [r4, #48]
	str	r4, [r7]
	b	.L65
.L83:
	.align	2
.L82:
	.word	.LANCHOR1+193
	.word	.LC1
	.word	.LANCHOR0
	.word	.LANCHOR1+217
	.word	.LC4
	.word	.LC5
	.fnend
	.size	mpp_buffer_get_with_tag, .-mpp_buffer_get_with_tag
	.align	1
	.global	mpp_ring_buffer_get_with_tag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_ring_buffer_get_with_tag, %function
mpp_ring_buffer_get_with_tag:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r10, r0
	mov	r7, r1
	mov	r5, r2
	ldr	fp, [sp, #48]
	bl	get_mpibuf_ops
	mov	r8, r0
	cbnz	r0, .L85
	movs	r2, #224
	ldr	r1, .L101
	ldr	r0, .L101+4
	bl	printk
	mov	r0, #-1
.L84:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L85:
	ldr	r6, .L101+8
	ldr	r0, .L101+12
	ldr	r1, [r6]
	bl	mpp_mem_pool_get_f
	mov	r4, r0
	cbnz	r0, .L87
	strd	r5, fp, [sp]
	mov	r3, r7
	mov	r2, r10
	movs	r1, #230
	ldr	r0, .L101+16
	bl	printk
.L100:
	mvn	r0, #1
	b	.L84
.L87:
	ldr	r3, [r8]
	cbnz	r3, .L88
.L89:
	mov	r2, r10
	strd	r5, fp, [sp]
	mov	r3, r7
	movs	r1, #239
	ldr	r0, .L101+20
	bl	printk
	mov	r2, r4
	ldr	r1, [r6]
	ldr	r0, .L101+12
	bl	mpp_mem_pool_put_f
	b	.L100
.L88:
	mov	r0, r5
	blx	r3
	mov	r9, r0
	cmp	r0, #0
	beq	.L89
	cmp	r5, #0
	beq	.L89
	ldr	r3, [r8, #20]
	cbz	r3, .L90
	blx	r3
	str	r0, [r4, #36]
.L90:
	mov	r3, #-1
	str	r3, [r4, #16]
	ldr	r3, [r4, #48]
	movs	r0, #0
	str	r9, [r4, #32]
	adds	r3, r3, #1
	str	r5, [r4, #4]
	str	r3, [r4, #48]
	movs	r3, #1
	str	r9, [r4, #12]
	str	r3, [r4, #44]
	str	r4, [r7]
	b	.L84
.L102:
	.align	2
.L101:
	.word	.LANCHOR1+241
	.word	.LC1
	.word	.LANCHOR0
	.word	.LANCHOR1+270
	.word	.LC4
	.word	.LC5
	.fnend
	.size	mpp_ring_buffer_get_with_tag, .-mpp_ring_buffer_get_with_tag
	.align	1
	.global	mpp_buffer_put_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_put_with_caller, %function
mpp_buffer_put_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	mov	r4, r1
	bl	get_mpibuf_ops
	mov	r6, r0
	cbnz	r0, .L104
	movw	r2, #263
	ldr	r1, .L117
	ldr	r0, .L117+4
	mov	r4, #-1
	bl	printk
.L103:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L104:
	cbnz	r5, .L106
	mov	r2, r4
	movw	r1, #269
	ldr	r0, .L117+8
	mvn	r4, #1
	bl	printk
	b	.L103
.L106:
	ldr	r4, [r5, #48]
	subs	r4, r4, #1
	str	r4, [r5, #48]
	cbnz	r4, .L110
	ldr	r3, [r5, #44]
	cbz	r3, .L107
	ldr	r0, [r5, #8]
	bl	vunmap
.L108:
	ldr	r3, [r6, #16]
	cbz	r3, .L109
	ldr	r0, [r5, #32]
	blx	r3
.L109:
	ldr	r3, .L117+12
	mov	r2, r5
	ldr	r0, .L117+16
	ldr	r1, [r3]
	bl	mpp_mem_pool_put_f
	b	.L103
.L107:
	ldr	r3, [r0, #8]
	cmp	r3, #0
	beq	.L108
	ldr	r0, [r5, #32]
	blx	r3
	b	.L108
.L110:
	movs	r4, #0
	b	.L103
.L118:
	.align	2
.L117:
	.word	.LANCHOR1+299
	.word	.LC1
	.word	.LC6
	.word	.LANCHOR0
	.word	.LANCHOR1+326
	.fnend
	.size	mpp_buffer_put_with_caller, .-mpp_buffer_put_with_caller
	.align	1
	.global	mpp_buffer_inc_ref_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_inc_ref_with_caller, %function
mpp_buffer_inc_ref_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r2, r1
	cbnz	r0, .L120
	mov	r1, #362
	ldr	r0, .L122
	bl	printk
	mvn	r0, #1
.L119:
	pop	{r3, pc}
.L120:
	ldr	r3, [r0, #48]
	adds	r3, r3, #1
	str	r3, [r0, #48]
	movs	r0, #0
	b	.L119
.L123:
	.align	2
.L122:
	.word	.LC7
	.fnend
	.size	mpp_buffer_inc_ref_with_caller, .-mpp_buffer_inc_ref_with_caller
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.global	mpp_buffer_get_fd_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_get_fd_with_caller, %function
mpp_buffer_get_fd_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r0
	ldr	r2, [r0, #16]
	mov	r6, r1
	ldr	r0, .L128
	movw	r1, #457
	bl	printk
	ldr	r4, [r5, #16]
	cmp	r4, #0
	bgt	.L124
	cbnz	r5, .L126
	mov	r2, r6
	mov	r1, #462
	ldr	r0, .L128+4
	mov	r4, #-1
	bl	printk
.L124:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L126:
	movs	r1, #0
	ldr	r0, [r5, #36]
	bl	dma_buf_fd
	subs	r4, r0, #0
	bge	.L127
	mov	r1, #466
	ldr	r3, .L128+8
	str	r1, [sp]
	ldr	r2, .L128+12
	ldr	r0, .L128+16
	bl	printk
	mov	r3, r4
	mov	r2, r5
	movw	r1, #469
	ldr	r0, .L128+20
	str	r6, [sp]
	bl	printk
.L127:
	mov	r2, r4
	movw	r1, #471
	ldr	r0, .L128+24
	bl	printk
	str	r4, [r5, #16]
	b	.L124
.L129:
	.align	2
.L128:
	.word	.LC8
	.word	.LC9
	.word	.LANCHOR1+353
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.fnend
	.size	mpp_buffer_get_fd_with_caller, .-mpp_buffer_get_fd_with_caller
	.text
	.align	1
	.global	mpp_buffer_get_dma_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_get_dma_with_caller, %function
mpp_buffer_get_dma_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r3, r1
	mov	r4, r0
	cbnz	r0, .L131
	mov	r2, r1
	ldr	r0, .L134
	movw	r1, #483
	bl	printk
.L132:
	mov	r0, r4
	pop	{r4, pc}
.L131:
	ldr	r2, [r0, #36]
	cbnz	r2, .L133
	mov	r2, r0
	mov	r1, #488
	ldr	r0, .L134+4
	bl	printk
.L133:
	ldr	r4, [r4, #36]
	b	.L132
.L135:
	.align	2
.L134:
	.word	.LC14
	.word	.LC15
	.fnend
	.size	mpp_buffer_get_dma_with_caller, .-mpp_buffer_get_dma_with_caller
	.align	1
	.global	mpp_buffer_get_size_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_get_size_with_caller, %function
mpp_buffer_get_size_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r3, r1
	mov	r4, r0
	cbnz	r0, .L137
	mov	r2, r1
	ldr	r0, .L140
	mov	r1, #498
	bl	printk
	mov	r0, r4
.L136:
	pop	{r4, pc}
.L137:
	ldr	r2, [r0, #4]
	cbnz	r2, .L139
	mov	r2, r0
	movw	r1, #503
	ldr	r0, .L140+4
	bl	printk
.L139:
	ldr	r0, [r4, #4]
	b	.L136
.L141:
	.align	2
.L140:
	.word	.LC16
	.word	.LC17
	.fnend
	.size	mpp_buffer_get_size_with_caller, .-mpp_buffer_get_size_with_caller
	.align	1
	.global	mpp_buffer_get_index_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_get_index_with_caller, %function
mpp_buffer_get_index_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r2, r1
	cbnz	r0, .L143
	movw	r1, #514
	ldr	r0, .L145
	bl	printk
	mov	r0, #-1
.L142:
	pop	{r3, pc}
.L143:
	ldr	r0, [r0, #20]
	b	.L142
.L146:
	.align	2
.L145:
	.word	.LC18
	.fnend
	.size	mpp_buffer_get_index_with_caller, .-mpp_buffer_get_index_with_caller
	.align	1
	.global	mpp_buffer_set_index_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_set_index_with_caller, %function
mpp_buffer_set_index_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	cbnz	r0, .L148
	mov	r1, #528
	ldr	r0, .L150
	bl	printk
	mvn	r0, #1
.L147:
	pop	{r3, pc}
.L148:
	str	r1, [r0, #20]
	movs	r0, #0
	b	.L147
.L151:
	.align	2
.L150:
	.word	.LC19
	.fnend
	.size	mpp_buffer_set_index_with_caller, .-mpp_buffer_set_index_with_caller
	.align	1
	.global	mpp_buffer_get_offset_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_get_offset_with_caller, %function
mpp_buffer_get_offset_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r2, r1
	cbnz	r0, .L153
	movw	r1, #543
	ldr	r0, .L155
	bl	printk
	mov	r0, #-1
.L152:
	pop	{r3, pc}
.L153:
	ldr	r0, [r0, #40]
	b	.L152
.L156:
	.align	2
.L155:
	.word	.LC20
	.fnend
	.size	mpp_buffer_get_offset_with_caller, .-mpp_buffer_get_offset_with_caller
	.align	1
	.global	mpp_buffer_set_offset_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_set_offset_with_caller, %function
mpp_buffer_set_offset_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	cbnz	r0, .L158
	movw	r1, #557
	ldr	r0, .L160
	bl	printk
	mvn	r0, #1
.L157:
	pop	{r3, pc}
.L158:
	str	r1, [r0, #40]
	movs	r0, #0
	b	.L157
.L161:
	.align	2
.L160:
	.word	.LC21
	.fnend
	.size	mpp_buffer_set_offset_with_caller, .-mpp_buffer_set_offset_with_caller
	.align	1
	.global	mpp_buffer_info_get_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_info_get_with_caller, %function
mpp_buffer_info_get_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r0
	mov	r5, r1
	mov	r6, r2
	bl	get_mpibuf_ops
	cbnz	r0, .L163
	movw	r2, #573
	ldr	r1, .L174
	ldr	r0, .L174+4
	bl	printk
	mov	r0, #-1
.L162:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L163:
	cbz	r4, .L165
	cbnz	r5, .L166
.L165:
	str	r6, [sp]
	mov	r3, r5
	mov	r2, r4
	movw	r1, #578
	ldr	r0, .L174+8
	bl	printk
	mvn	r0, #1
	b	.L162
.L166:
	ldr	r3, [r4, #8]
	cbnz	r3, .L167
	ldr	r3, [r0, #4]
	cbz	r3, .L167
	ldr	r0, [r4, #32]
	blx	r3
	str	r0, [r4, #8]
.L167:
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldm	r4, {r0, r1, r2, r3}
	stm	r5, {r0, r1, r2, r3}
	movs	r0, #0
	b	.L162
.L175:
	.align	2
.L174:
	.word	.LANCHOR1+383
	.word	.LC1
	.word	.LC22
	.fnend
	.size	mpp_buffer_info_get_with_caller, .-mpp_buffer_info_get_with_caller
	.align	1
	.global	mpp_buffer_flush_for_cpu_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_flush_for_cpu_with_caller, %function
mpp_buffer_flush_for_cpu_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r2, r1
	ldr	r4, [r0]
	mov	r5, r0
	cbnz	r4, .L177
	movw	r1, #595
	ldr	r0, .L181
	bl	printk
	mvn	r0, #1
.L176:
	pop	{r4, r5, r6, pc}
.L177:
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #20]
	ldr	r1, [r4, #4]
	adds	r6, r2, r3
	ldr	r0, [r4, #36]
	cmp	r6, r1
	bcc	.L179
	subs	r3, r1, r2
	movs	r1, #2
	bl	dma_buf_begin_cpu_access_partial
	ldr	r2, [r5, #20]
	ldr	r3, [r5, #12]
	movs	r1, #2
	ldr	r0, [r4, #36]
	add	r3, r3, r2
	ldr	r2, [r4, #4]
	subs	r3, r3, r2
	movs	r2, #0
.L180:
	bl	dma_buf_begin_cpu_access_partial
	movs	r0, #0
	b	.L176
.L179:
	movs	r1, #2
	b	.L180
.L182:
	.align	2
.L181:
	.word	.LC21
	.fnend
	.size	mpp_buffer_flush_for_cpu_with_caller, .-mpp_buffer_flush_for_cpu_with_caller
	.align	1
	.global	mpp_buffer_flush_for_device_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_flush_for_device_with_caller, %function
mpp_buffer_flush_for_device_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r2, r1
	ldr	r4, [r0]
	mov	r5, r0
	cbnz	r4, .L184
	mov	r1, #616
	ldr	r0, .L188
	bl	printk
	mvn	r0, #1
.L183:
	pop	{r4, r5, r6, pc}
.L184:
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #20]
	ldr	r1, [r4, #4]
	adds	r6, r2, r3
	ldr	r0, [r4, #36]
	cmp	r6, r1
	bcc	.L186
	subs	r3, r1, r2
	movs	r1, #1
	bl	dma_buf_end_cpu_access_partial
	ldr	r2, [r5, #20]
	ldr	r3, [r5, #12]
	movs	r1, #1
	ldr	r0, [r4, #36]
	add	r3, r3, r2
	ldr	r2, [r4, #4]
	subs	r3, r3, r2
	movs	r2, #0
.L187:
	bl	dma_buf_end_cpu_access_partial
	movs	r0, #0
	b	.L183
.L186:
	movs	r1, #1
	b	.L187
.L189:
	.align	2
.L188:
	.word	.LC21
	.fnend
	.size	mpp_buffer_flush_for_device_with_caller, .-mpp_buffer_flush_for_device_with_caller
	.align	1
	.global	mpp_buffer_get_mpi_buf_id_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_get_mpi_buf_id_with_caller, %function
mpp_buffer_get_mpi_buf_id_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 104
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #104
	sub	sp, sp, #104
	mov	r4, r0
	mov	r6, r1
	bl	get_mpibuf_ops
	mov	r5, r0
	cbnz	r0, .L191
	movw	r2, #641
	ldr	r1, .L198
	ldr	r0, .L198+4
	bl	printk
.L196:
	mov	r0, #-1
	b	.L190
.L191:
	cbnz	r4, .L193
	mov	r3, r6
	mov	r2, r4
	movw	r1, #647
	ldr	r0, .L198+8
	bl	printk
	mvn	r0, #1
.L190:
	add	sp, sp, #104
	@ sp needed
	pop	{r4, r5, r6, pc}
.L193:
	movs	r2, #104
	movs	r1, #0
	mov	r0, sp
	bl	memset
	ldr	r3, [r5, #52]
	cbz	r3, .L194
	mov	r2, #-1
	mov	r1, sp
	ldr	r0, [r4, #32]
	blx	r3
	cmp	r0, #0
	bne	.L196
	ldr	r0, [sp, #56]
	b	.L190
.L194:
	movw	r1, #655
	ldr	r0, .L198+12
	bl	printk
	b	.L196
.L199:
	.align	2
.L198:
	.word	.LANCHOR1+415
	.word	.LC1
	.word	.LC23
	.word	.LC24
	.fnend
	.size	mpp_buffer_get_mpi_buf_id_with_caller, .-mpp_buffer_get_mpi_buf_id_with_caller
	.align	1
	.global	mpp_buffer_set_phy_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_set_phy_caller, %function
mpp_buffer_set_phy_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbnz	r0, .L201
	mov	r1, #668
	ldr	r0, .L202
	b	printk
.L201:
	movs	r3, #1
	strd	r3, r1, [r0, #24]
	bx	lr
.L203:
	.align	2
.L202:
	.word	.LC20
	.fnend
	.size	mpp_buffer_set_phy_caller, .-mpp_buffer_set_phy_caller
	.align	1
	.global	mpp_buffer_get_phy_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_get_phy_caller, %function
mpp_buffer_get_phy_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	get_mpibuf_ops
	cbnz	r4, .L205
	ldr	r0, .L214
	mov	r2, r5
	movw	r1, #683
.L213:
	bl	printk
.L209:
	mov	r0, #-1
	b	.L204
.L205:
	cbnz	r0, .L207
	mov	r2, #688
	ldr	r1, .L214+4
	ldr	r0, .L214+8
	b	.L213
.L207:
	ldr	r3, [r4, #24]
	cbz	r3, .L208
	ldr	r0, [r4, #28]
.L204:
	pop	{r3, r4, r5, pc}
.L208:
	ldr	r3, [r0, #24]
	cmp	r3, #0
	beq	.L209
	ldr	r0, [r4, #32]
	blx	r3
	adds	r3, r0, #1
	beq	.L204
	movs	r3, #1
	str	r0, [r4, #28]
	str	r3, [r4, #24]
	b	.L204
.L215:
	.align	2
.L214:
	.word	.LC20
	.word	.LANCHOR1+453
	.word	.LC1
	.fnend
	.size	mpp_buffer_get_phy_caller, .-mpp_buffer_get_phy_caller
	.align	1
	.global	mpp_buffer_map_ring_ptr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_map_ring_ptr, %function
mpp_buffer_map_ring_ptr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r8, r0
	ldr	r1, .L229
	bl	mpp_buffer_get_phy_caller
	adds	r3, r0, #1
	bne	.L217
	ldr	r0, [r8, #36]
	bl	mpp_srv_get_phy
.L217:
	ldr	r5, [r8, #4]
	ubfx	r4, r0, #0, #12
	mov	r7, r0
	subs	r5, r5, #1
	add	r5, r5, r0
	mvn	r5, r5, lsr #12
	mvn	r5, r5, lsl #12
	adds	r5, r5, #1
	cbz	r4, .L218
	movw	r1, #307
	ldr	r0, .L229+4
	bl	printk
	movs	r6, #0
.L216:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L218:
	subs	r0, r5, r0
	mov	r1, #3264
	lsrs	r0, r0, #12
	adds	r0, r0, #1
	lsl	r10, r0, #1
	lsls	r0, r0, #3
	bl	__kmalloc
	mov	r6, r0
	cmp	r0, #0
	beq	.L216
	ldr	fp, .L229+24
	mov	r9, r7
.L220:
	cmp	r9, r5
	bcc	.L222
	ldr	r9, .L229+12
	ldr	fp, .L229+24
.L223:
	cmp	r7, r5
	bcc	.L225
	ldr	r3, .L229+8
	movs	r2, #4
	mov	r1, r4
	mov	r0, r6
	ldr	r3, [r3]
	orr	r3, r3, #512
	bl	vmap
	str	r0, [r8, #8]
	mov	r0, r6
	bl	kfree
	ldr	r6, [r8, #8]
	b	.L216
.L222:
	cmp	r4, r10
	blt	.L221
	mov	r1, #318
	ldr	r3, .L229
	str	r1, [sp]
	mov	r0, fp
	ldr	r2, .L229+12
	bl	printk
.L221:
	ldr	r3, .L229+16
	ldr	r2, [r3]
	ldr	r3, .L229+20
	rsb	r2, r2, r9, lsr #12
	add	r9, r9, #4096
	ldr	r3, [r3]
	add	r3, r3, r2, lsl #5
	str	r3, [r6, r4, lsl #2]
	adds	r4, r4, #1
	b	.L220
.L225:
	cmp	r4, r10
	blt	.L224
	movw	r1, #325
	ldr	r3, .L229
	str	r1, [sp]
	mov	r2, r9
	mov	r0, fp
	bl	printk
.L224:
	ldr	r3, .L229+16
	ldr	r2, [r3]
	ldr	r3, .L229+20
	rsb	r2, r2, r7, lsr #12
	add	r7, r7, #4096
	ldr	r3, [r3]
	add	r3, r3, r2, lsl #5
	str	r3, [r6, r4, lsl #2]
	adds	r4, r4, #1
	b	.L223
.L230:
	.align	2
.L229:
	.word	.LANCHOR1+479
	.word	.LC25
	.word	pgprot_kernel
	.word	.LC26
	.word	__pv_phys_pfn_offset
	.word	mem_map
	.word	.LC11
	.fnend
	.size	mpp_buffer_map_ring_ptr, .-mpp_buffer_map_ring_ptr
	.align	1
	.global	mpp_buffer_map
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_map, %function
mpp_buffer_map:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	bl	get_mpibuf_ops
	cbnz	r0, .L232
	movw	r2, #341
	ldr	r1, .L238
	ldr	r0, .L238+4
	bl	printk
	mov	r0, #-1
.L231:
	pop	{r4, r5, r6, pc}
.L232:
	ldr	r6, [r4, #8]
	cbnz	r6, .L235
	ldr	r3, [r0, #4]
	cbz	r3, .L236
	ldr	r5, [r4, #44]
	cbz	r5, .L234
	mov	r0, r4
	bl	mpp_buffer_map_ring_ptr
.L237:
	str	r0, [r4, #8]
.L235:
	movs	r0, #0
	b	.L231
.L234:
	ldr	r0, [r4, #32]
	blx	r3
	b	.L237
.L236:
	mov	r0, r3
	b	.L231
.L239:
	.align	2
.L238:
	.word	.LANCHOR1+503
	.word	.LC1
	.fnend
	.size	mpp_buffer_map, .-mpp_buffer_map
	.align	1
	.global	mpp_buffer_read_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_read_with_caller, %function
mpp_buffer_read_with_caller:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r8, r1
	mov	r5, r2
	mov	r6, r3
	mov	r7, r0
	cbz	r0, .L241
	cbnz	r2, .L242
.L241:
	ldr	r3, [sp, #32]
	mov	r2, r7
	mov	r1, #378
	ldr	r0, .L250
	mvn	r4, #1
	str	r3, [sp]
	mov	r3, r5
	bl	printk
.L240:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L242:
	cbz	r3, .L245
	bl	mpp_buffer_map
	mov	r4, r0
	cbnz	r0, .L246
	ldr	r1, [r7, #8]
	cbnz	r1, .L244
	movw	r1, #389
	ldr	r3, .L250+4
	str	r1, [sp]
	ldr	r2, .L250+8
	ldr	r0, .L250+12
	bl	printk
	b	.L240
.L245:
	mov	r4, r3
	b	.L240
.L246:
	mov	r4, #-1
	b	.L240
.L244:
	mov	r2, r6
	add	r1, r1, r8
	mov	r0, r5
	bl	memcpy
	b	.L240
.L251:
	.align	2
.L250:
	.word	.LC27
	.word	.LANCHOR1+518
	.word	.LC28
	.word	.LC11
	.fnend
	.size	mpp_buffer_read_with_caller, .-mpp_buffer_read_with_caller
	.align	1
	.global	mpp_buffer_write_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_write_with_caller, %function
mpp_buffer_write_with_caller:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r8, r1
	mov	r6, r2
	mov	r5, r3
	mov	r7, r0
	cbz	r0, .L253
	cbnz	r2, .L254
.L253:
	ldr	r3, [sp, #32]
	mov	r2, r7
	movw	r1, #405
	ldr	r0, .L263
	mvn	r4, #1
	str	r3, [sp]
	mov	r3, r6
	bl	printk
.L252:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L254:
	cbz	r3, .L257
	ldr	r2, [r0, #4]
	add	r3, r3, r1
	cmp	r3, r2
	bhi	.L258
	bl	mpp_buffer_map
	mov	r4, r0
	cbnz	r0, .L259
	ldr	r0, [r7, #8]
	cbnz	r0, .L256
	movw	r1, #419
	ldr	r3, .L263+4
	str	r1, [sp]
	ldr	r2, .L263+8
	ldr	r0, .L263+12
	bl	printk
	b	.L252
.L257:
	mov	r4, r3
	b	.L252
.L258:
	mvn	r4, #5
	b	.L252
.L259:
	mov	r4, #-1
	b	.L252
.L256:
	mov	r2, r5
	mov	r1, r6
	add	r0, r0, r8
	bl	memcpy
	b	.L252
.L264:
	.align	2
.L263:
	.word	.LC29
	.word	.LANCHOR1+546
	.word	.LC30
	.word	.LC11
	.fnend
	.size	mpp_buffer_write_with_caller, .-mpp_buffer_write_with_caller
	.align	1
	.global	mpp_buffer_get_ptr_with_caller
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_buffer_get_ptr_with_caller, %function
mpp_buffer_get_ptr_with_caller:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r0
	mov	r6, r1
	bl	get_mpibuf_ops
	mov	r5, r0
	cbnz	r0, .L266
	mov	r2, #432
	ldr	r1, .L273
	ldr	r0, .L273+4
	bl	printk
.L265:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L266:
	cbnz	r4, .L268
	mov	r2, r6
	movw	r1, #437
	ldr	r0, .L273+8
	bl	printk
.L272:
	movs	r5, #0
	b	.L265
.L268:
	mov	r0, r4
	bl	mpp_buffer_map
	cmp	r0, #0
	bne	.L272
	ldr	r3, [r4, #8]
	cbnz	r3, .L270
	mov	r1, #444
	ldr	r3, .L273+12
	str	r1, [sp]
	ldr	r2, .L273+16
	ldr	r0, .L273+20
	bl	printk
	ldr	r3, [r4, #8]
	cbnz	r3, .L270
	mov	r3, r6
	mov	r2, r4
	movw	r1, #447
	ldr	r0, .L273+24
	bl	printk
.L270:
	ldr	r5, [r4, #8]
	b	.L265
.L274:
	.align	2
.L273:
	.word	.LANCHOR1+575
	.word	.LC1
	.word	.LC31
	.word	.LANCHOR1+606
	.word	.LC32
	.word	.LC11
	.word	.LC33
	.fnend
	.size	mpp_buffer_get_ptr_with_caller, .-mpp_buffer_get_ptr_with_caller
	.section	.rodata
	.set	.LANCHOR1,. + 0
	.type	__FUNCTION__.34728, %object
	.size	__FUNCTION__.34728, 21
__FUNCTION__.34728:
	.ascii	"mpp_buffer_pool_init\000"
	.type	__FUNCTION__.34732, %object
	.size	__FUNCTION__.34732, 23
__FUNCTION__.34732:
	.ascii	"mpp_buffer_pool_deinit\000"
	.type	__func__.34745, %object
	.size	__func__.34745, 27
__func__.34745:
	.ascii	"mpp_buffer_import_with_tag\000"
	.type	__FUNCTION__.34747, %object
	.size	__FUNCTION__.34747, 27
__FUNCTION__.34747:
	.ascii	"mpp_buffer_import_with_tag\000"
	.type	__func__.34755, %object
	.size	__func__.34755, 23
__func__.34755:
	.ascii	"mpi_buf_alloc_with_tag\000"
	.type	__func__.34762, %object
	.size	__func__.34762, 21
__func__.34762:
	.ascii	"mpi_buf_ref_with_tag\000"
	.type	__func__.34769, %object
	.size	__func__.34769, 23
__func__.34769:
	.ascii	"mpi_buf_unref_with_tag\000"
	.type	__func__.34776, %object
	.size	__func__.34776, 28
__func__.34776:
	.ascii	"mpi_buf_get_dma_with_caller\000"
	.type	__func__.34787, %object
	.size	__func__.34787, 24
__func__.34787:
	.ascii	"mpp_buffer_get_with_tag\000"
	.type	__FUNCTION__.34788, %object
	.size	__FUNCTION__.34788, 24
__FUNCTION__.34788:
	.ascii	"mpp_buffer_get_with_tag\000"
	.type	__func__.34799, %object
	.size	__func__.34799, 29
__func__.34799:
	.ascii	"mpp_ring_buffer_get_with_tag\000"
	.type	__FUNCTION__.34800, %object
	.size	__FUNCTION__.34800, 29
__FUNCTION__.34800:
	.ascii	"mpp_ring_buffer_get_with_tag\000"
	.type	__func__.34807, %object
	.size	__func__.34807, 27
__func__.34807:
	.ascii	"mpp_buffer_put_with_caller\000"
	.type	__FUNCTION__.34808, %object
	.size	__FUNCTION__.34808, 27
__FUNCTION__.34808:
	.ascii	"mpp_buffer_put_with_caller\000"
	.type	__FUNCTION__.34879, %object
	.size	__FUNCTION__.34879, 30
__FUNCTION__.34879:
	.ascii	"mpp_buffer_get_fd_with_caller\000"
	.type	__func__.34921, %object
	.size	__func__.34921, 32
__func__.34921:
	.ascii	"mpp_buffer_info_get_with_caller\000"
	.type	__func__.34939, %object
	.size	__func__.34939, 38
__func__.34939:
	.ascii	"mpp_buffer_get_mpi_buf_id_with_caller\000"
	.type	__func__.34953, %object
	.size	__func__.34953, 26
__func__.34953:
	.ascii	"mpp_buffer_get_phy_caller\000"
	.type	__FUNCTION__.34818, %object
	.size	__FUNCTION__.34818, 24
__FUNCTION__.34818:
	.ascii	"mpp_buffer_map_ring_ptr\000"
	.type	__func__.34833, %object
	.size	__func__.34833, 15
__func__.34833:
	.ascii	"mpp_buffer_map\000"
	.type	__FUNCTION__.34848, %object
	.size	__FUNCTION__.34848, 28
__FUNCTION__.34848:
	.ascii	"mpp_buffer_read_with_caller\000"
	.type	__FUNCTION__.34860, %object
	.size	__FUNCTION__.34860, 29
__FUNCTION__.34860:
	.ascii	"mpp_buffer_write_with_caller\000"
	.type	__func__.34869, %object
	.size	__func__.34869, 31
__func__.34869:
	.ascii	"mpp_buffer_get_ptr_with_caller\000"
	.type	__FUNCTION__.34870, %object
	.size	__FUNCTION__.34870, 31
__FUNCTION__.34870:
	.ascii	"mpp_buffer_get_ptr_with_caller\000"
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_mppbuf_pool, %object
	.size	g_mppbuf_pool, 4
g_mppbuf_pool:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"mpp_buffer\000"
.LC1:
	.ascii	"\0013%s:%d: mpibuf_ops get fail\000"
.LC2:
	.ascii	"\0013%d: mpp_buffer_commit invalid input: info NULL"
	.ascii	" from %s\012\000"
.LC3:
	.ascii	"\0013%d: mpp_buffer_import fail %s\012\000"
.LC4:
	.ascii	"\0013%d: buf impl malloc fail : group %p buffer %p "
	.ascii	"size %u from %s\012\000"
.LC5:
	.ascii	"\0013%d: mpp_buffer_get invalid input: group %p buf"
	.ascii	"fer %p size %u from %s\012\000"
.LC6:
	.ascii	"\0013%d: mpp_buffer_put invalid input: buffer NULL "
	.ascii	"from %s\012\000"
.LC7:
	.ascii	"\0013%d: mpp_buffer_inc_ref invalid input: buffer N"
	.ascii	"ULL from %s\012\000"
.LC8:
	.ascii	"\0016%d: p->info.fd %d\000"
.LC9:
	.ascii	"\0013%d: mpp_buffer_get_fd invalid NULL input from "
	.ascii	"%s\012\000"
.LC10:
	.ascii	"fd >= 0\000"
.LC11:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC12:
	.ascii	"\0013%d: mpp_buffer_get_fd buffer %p fd %d from %s\012"
	.ascii	"\000"
.LC13:
	.ascii	"\0016%d: dma_buf_fd fd %d\000"
.LC14:
	.ascii	"\0013%d: mpp_buffer_get_dma invalid NULL input from"
	.ascii	" %s\012\000"
.LC15:
	.ascii	"\0013%d: mpp_buffer_get_fd buffer %p from %s\012\000"
.LC16:
	.ascii	"\0013%d: mpp_buffer_get_size invalid NULL input fro"
	.ascii	"m %s\012\000"
.LC17:
	.ascii	"\0013%d: mpp_buffer_get_size buffer %p ret zero siz"
	.ascii	"e from %s\012\000"
.LC18:
	.ascii	"\0013%d: mpp_buffer_get_index invalid NULL input fr"
	.ascii	"om %s\012\000"
.LC19:
	.ascii	"\0013%d: mpp_buffer_set_index invalid NULL input fr"
	.ascii	"om %s\012\000"
.LC20:
	.ascii	"\0013%d: mpp_buffer_get_offset invalid NULL input f"
	.ascii	"rom %s\012\000"
.LC21:
	.ascii	"\0013%d: mpp_buffer_set_offset invalid NULL input f"
	.ascii	"rom %s\012\000"
.LC22:
	.ascii	"\0013%d: mpp_buffer_info_get invalid input buffer %"
	.ascii	"p info %p from %s\012\000"
.LC23:
	.ascii	"\0013%d: mpp_buffer_get_mpi_buf_id invalid input bu"
	.ascii	"ffer %p from %s\012\000"
.LC24:
	.ascii	"\0013%d: get buf info fail\000"
.LC25:
	.ascii	"\0013%d: alloc buf start is no 4k align\000"
.LC26:
	.ascii	"i < page_count\000"
.LC27:
	.ascii	"\0013%d: mpp_buffer_read invalid input: buffer %p d"
	.ascii	"ata %p from %s\012\000"
.LC28:
	.ascii	"src != ((void *)0)\000"
.LC29:
	.ascii	"\0013%d: mpp_buffer_write invalid input: buffer %p "
	.ascii	"data %p from %s\012\000"
.LC30:
	.ascii	"dst != ((void *)0)\000"
.LC31:
	.ascii	"\0013%d: mpp_buffer_get_ptr invalid NULL input from"
	.ascii	" %s\012\000"
.LC32:
	.ascii	"p->info.ptr != ((void *)0)\000"
.LC33:
	.ascii	"\0013%d: mpp_buffer_get_ptr buffer %p ret NULL from"
	.ascii	" %s\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
