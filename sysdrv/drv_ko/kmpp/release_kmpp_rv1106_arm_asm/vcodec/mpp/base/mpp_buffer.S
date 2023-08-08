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
	cbnz	r0, .L2
	movs	r2, #43
	ldr	r1, .L16
	ldr	r0, .L16+4
	bl	printk
	mov	r0, #-1
.L1:
	pop	{r4, r5, r6, r7, r8, pc}
.L2:
	cbnz	r4, .L4
	ldr	r2, [sp, #24]
	movs	r1, #49
	ldr	r0, .L16+8
.L15:
	bl	printk
	mvn	r0, #2
	b	.L1
.L4:
	cbz	r7, .L8
	movs	r1, #52
	ldr	r0, .L16+12
	bl	mpp_osal_calloc
	mov	r5, r0
	cbnz	r0, .L5
	ldr	r2, [sp, #24]
	movs	r1, #56
	ldr	r0, .L16+16
	b	.L15
.L5:
	mov	r6, r0
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r6!, {r0, r1, r2, r3}
	ldm	r4, {r0, r1, r2, r3}
	stm	r6, {r0, r1, r2, r3}
	ldr	r0, [r5, #12]
	str	r0, [r5, #32]
	ldr	r3, [r8, #20]
	cbz	r3, .L6
	blx	r3
	str	r0, [r5, #36]
.L6:
	ldr	r3, [r8, #12]
	cbz	r3, .L7
	ldr	r0, [r5, #32]
	blx	r3
.L7:
	ldr	r3, [r5, #48]
	movs	r0, #0
	adds	r3, r3, #1
	str	r3, [r5, #48]
	mov	r3, #-1
	str	r3, [r5, #16]
	str	r5, [r7]
	b	.L1
.L8:
	mov	r0, r7
	b	.L1
.L17:
	.align	2
.L16:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LANCHOR0+27
	.word	.LC2
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
	cbnz	r0, .L19
	movs	r2, #80
	ldr	r1, .L23
	ldr	r0, .L23+4
	bl	printk
.L18:
	movs	r0, #0
	pop	{r4, pc}
.L19:
	ldr	r3, [r0]
	cmp	r3, #0
	beq	.L18
	mov	r0, r4
	pop	{r4, lr}
	bx	r3
.L24:
	.align	2
.L23:
	.word	.LANCHOR0+54
	.word	.LC0
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
	cbnz	r0, .L26
	movs	r2, #94
	ldr	r1, .L31
	ldr	r0, .L31+4
	bl	printk
	mov	r0, #-1
.L25:
	pop	{r4, pc}
.L26:
	cbz	r4, .L30
	ldr	r3, [r0, #12]
	cbz	r3, .L29
	mov	r0, r4
	blx	r3
.L30:
	movs	r0, #0
	b	.L25
.L29:
	mov	r0, r3
	b	.L25
.L32:
	.align	2
.L31:
	.word	.LANCHOR0+77
	.word	.LC0
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
	cbnz	r0, .L34
	movs	r2, #109
	ldr	r1, .L39
	ldr	r0, .L39+4
	bl	printk
	mov	r0, #-1
.L33:
	pop	{r4, pc}
.L34:
	cbz	r4, .L38
	ldr	r3, [r0, #16]
	cbz	r3, .L37
	mov	r0, r4
	blx	r3
.L38:
	movs	r0, #0
	b	.L33
.L37:
	mov	r0, r3
	b	.L33
.L40:
	.align	2
.L39:
	.word	.LANCHOR0+98
	.word	.LC0
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
	cbnz	r0, .L42
	movs	r2, #125
	ldr	r1, .L49
	ldr	r0, .L49+4
	bl	printk
.L43:
	movs	r0, #0
	pop	{r4, pc}
.L42:
	cmp	r4, #0
	beq	.L43
	ldr	r3, [r0, #20]
	cmp	r3, #0
	beq	.L43
	mov	r0, r4
	pop	{r4, lr}
	bx	r3
.L50:
	.align	2
.L49:
	.word	.LANCHOR0+121
	.word	.LC0
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
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r9, r0
	mov	r6, r1
	mov	r5, r2
	ldr	r10, [sp, #40]
	bl	get_mpibuf_ops
	mov	r7, r0
	cbnz	r0, .L52
	movs	r2, #147
	ldr	r1, .L68
	ldr	r0, .L68+4
	bl	printk
	mov	r0, #-1
.L51:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L52:
	movs	r1, #52
	ldr	r0, .L68+8
	bl	mpp_osal_calloc
	mov	r4, r0
	cbnz	r0, .L54
	strd	r5, r10, [sp]
	mov	r3, r6
	mov	r2, r9
	movs	r1, #153
	ldr	r0, .L68+12
	bl	printk
.L67:
	mvn	r0, #1
	b	.L51
.L54:
	ldr	r3, [r7]
	cbnz	r3, .L55
.L56:
	strd	r5, r10, [sp]
	mov	r3, r6
	mov	r2, r9
	movs	r1, #162
	ldr	r0, .L68+16
	bl	printk
	mov	r1, r4
	ldr	r0, .L68+8
	bl	mpp_osal_free
	b	.L67
.L55:
	mov	r0, r5
	blx	r3
	mov	r8, r0
	cmp	r0, #0
	beq	.L56
	cmp	r5, #0
	beq	.L56
	ldr	r3, [r7, #20]
	cbz	r3, .L57
	blx	r3
	str	r0, [r4, #36]
.L57:
	mov	r3, #-1
	str	r3, [r4, #16]
	ldr	r3, [r4, #48]
	movs	r0, #0
	str	r8, [r4, #32]
	str	r5, [r4, #4]
	adds	r3, r3, #1
	str	r8, [r4, #12]
	str	r3, [r4, #48]
	str	r4, [r6]
	b	.L51
.L69:
	.align	2
.L68:
	.word	.LANCHOR0+149
	.word	.LC0
	.word	.LANCHOR0+173
	.word	.LC3
	.word	.LC4
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
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r9, r0
	mov	r6, r1
	mov	r5, r2
	ldr	r10, [sp, #40]
	bl	get_mpibuf_ops
	mov	r7, r0
	cbnz	r0, .L71
	movs	r2, #187
	ldr	r1, .L87
	ldr	r0, .L87+4
	bl	printk
	mov	r0, #-1
.L70:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L71:
	movs	r1, #52
	ldr	r0, .L87+8
	bl	mpp_osal_calloc
	mov	r4, r0
	cbnz	r0, .L73
	strd	r5, r10, [sp]
	mov	r3, r6
	mov	r2, r9
	movs	r1, #193
	ldr	r0, .L87+12
	bl	printk
.L86:
	mvn	r0, #1
	b	.L70
.L73:
	ldr	r3, [r7]
	cbnz	r3, .L74
.L75:
	strd	r5, r10, [sp]
	mov	r3, r6
	mov	r2, r9
	movs	r1, #202
	ldr	r0, .L87+16
	bl	printk
	mov	r1, r4
	ldr	r0, .L87+8
	bl	mpp_osal_free
	b	.L86
.L74:
	mov	r0, r5
	blx	r3
	mov	r8, r0
	cmp	r0, #0
	beq	.L75
	cmp	r5, #0
	beq	.L75
	ldr	r3, [r7, #20]
	cbz	r3, .L76
	blx	r3
	str	r0, [r4, #36]
.L76:
	mov	r3, #-1
	str	r3, [r4, #16]
	ldr	r3, [r4, #48]
	movs	r0, #0
	str	r8, [r4, #32]
	adds	r3, r3, #1
	str	r5, [r4, #4]
	str	r3, [r4, #48]
	movs	r3, #1
	str	r8, [r4, #12]
	str	r3, [r4, #44]
	str	r4, [r6]
	b	.L70
.L88:
	.align	2
.L87:
	.word	.LANCHOR0+197
	.word	.LC0
	.word	.LANCHOR0+226
	.word	.LC3
	.word	.LC4
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
	cbnz	r0, .L90
	movs	r2, #225
	ldr	r1, .L103
	ldr	r0, .L103+4
	mov	r4, #-1
	bl	printk
.L89:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L90:
	cbnz	r5, .L92
	mov	r2, r4
	movs	r1, #231
	ldr	r0, .L103+8
	mvn	r4, #1
	bl	printk
	b	.L89
.L92:
	ldr	r4, [r5, #48]
	subs	r4, r4, #1
	str	r4, [r5, #48]
	cbnz	r4, .L96
	ldr	r3, [r5, #44]
	cbz	r3, .L93
	ldr	r0, [r5, #8]
	bl	vunmap
.L94:
	ldr	r3, [r6, #16]
	cbz	r3, .L95
	ldr	r0, [r5, #32]
	blx	r3
.L95:
	mov	r1, r5
	ldr	r0, .L103+12
	bl	mpp_osal_free
	b	.L89
.L93:
	ldr	r3, [r0, #8]
	cmp	r3, #0
	beq	.L94
	ldr	r0, [r5, #32]
	blx	r3
	b	.L94
.L96:
	movs	r4, #0
	b	.L89
.L104:
	.align	2
.L103:
	.word	.LANCHOR0+255
	.word	.LC0
	.word	.LC5
	.word	.LANCHOR0+282
	.fnend
	.size	mpp_buffer_put_with_caller, .-mpp_buffer_put_with_caller
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
	ldr	r0, [r0, #36]
	bl	mpp_srv_get_phy
	ldr	r4, [r8, #4]
	subs	r4, r4, #1
	add	r4, r4, r0
	ubfx	r5, r0, #0, #12
	mov	r7, r0
	mvn	r4, r4, lsr #12
	mvn	r4, r4, lsl #12
	adds	r4, r4, #1
	cbz	r5, .L106
	movw	r1, #265
	ldr	r0, .L117
	bl	printk
	movs	r6, #0
.L105:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L106:
	subs	r0, r4, r0
	mov	r1, #3264
	lsrs	r0, r0, #12
	adds	r0, r0, #1
	lsl	r10, r0, #1
	lsls	r0, r0, #3
	bl	__kmalloc
	mov	r6, r0
	cmp	r0, #0
	beq	.L105
	ldr	fp, .L117+24
	mov	r9, r7
.L108:
	cmp	r9, r4
	bcc	.L110
	ldr	r9, .L117+12
	ldr	fp, .L117+24
.L111:
	cmp	r7, r4
	bcc	.L113
	ldr	r3, .L117+4
	movs	r2, #4
	mov	r1, r5
	mov	r0, r6
	ldr	r3, [r3]
	orr	r3, r3, #512
	bl	vmap
	str	r0, [r8, #8]
	mov	r0, r6
	bl	kfree
	ldr	r6, [r8, #8]
	b	.L105
.L110:
	cmp	r5, r10
	blt	.L109
	mov	r1, #276
	ldr	r3, .L117+8
	str	r1, [sp]
	mov	r0, fp
	ldr	r2, .L117+12
	bl	printk
.L109:
	ldr	r3, .L117+16
	ldr	r2, [r3]
	ldr	r3, .L117+20
	rsb	r2, r2, r9, lsr #12
	add	r9, r9, #4096
	ldr	r3, [r3]
	add	r3, r3, r2, lsl #5
	str	r3, [r6, r5, lsl #2]
	adds	r5, r5, #1
	b	.L108
.L113:
	cmp	r5, r10
	blt	.L112
	movw	r1, #283
	ldr	r3, .L117+8
	str	r1, [sp]
	mov	r2, r9
	mov	r0, fp
	bl	printk
.L112:
	ldr	r3, .L117+16
	ldr	r2, [r3]
	ldr	r3, .L117+20
	rsb	r2, r2, r7, lsr #12
	add	r7, r7, #4096
	ldr	r3, [r3]
	add	r3, r3, r2, lsl #5
	str	r3, [r6, r5, lsl #2]
	adds	r5, r5, #1
	b	.L111
.L118:
	.align	2
.L117:
	.word	.LC6
	.word	pgprot_kernel
	.word	.LANCHOR0+309
	.word	.LC7
	.word	__pv_phys_pfn_offset
	.word	mem_map
	.word	.LC8
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
	cbnz	r0, .L120
	movw	r2, #299
	ldr	r1, .L126
	ldr	r0, .L126+4
	bl	printk
	mov	r0, #-1
.L119:
	pop	{r4, r5, r6, pc}
.L120:
	ldr	r6, [r4, #8]
	cbnz	r6, .L123
	ldr	r3, [r0, #4]
	cbz	r3, .L124
	ldr	r5, [r4, #44]
	cbz	r5, .L122
	mov	r0, r4
	bl	mpp_buffer_map_ring_ptr
.L125:
	str	r0, [r4, #8]
.L123:
	movs	r0, #0
	b	.L119
.L122:
	ldr	r0, [r4, #32]
	blx	r3
	b	.L125
.L124:
	mov	r0, r3
	b	.L119
.L127:
	.align	2
.L126:
	.word	.LANCHOR0+333
	.word	.LC0
	.fnend
	.size	mpp_buffer_map, .-mpp_buffer_map
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
	cbnz	r0, .L129
	movw	r1, #319
	ldr	r0, .L131
	bl	printk
	mvn	r0, #1
.L128:
	pop	{r3, pc}
.L129:
	ldr	r3, [r0, #48]
	adds	r3, r3, #1
	str	r3, [r0, #48]
	movs	r0, #0
	b	.L128
.L132:
	.align	2
.L131:
	.word	.LC9
	.fnend
	.size	mpp_buffer_inc_ref_with_caller, .-mpp_buffer_inc_ref_with_caller
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
	cbz	r0, .L134
	cbnz	r2, .L135
.L134:
	ldr	r3, [sp, #32]
	mov	r2, r7
	mov	r1, #334
	ldr	r0, .L143
	mvn	r4, #1
	str	r3, [sp]
	mov	r3, r5
	bl	printk
.L133:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L135:
	cbz	r3, .L138
	bl	mpp_buffer_map
	mov	r4, r0
	cbnz	r0, .L139
	ldr	r1, [r7, #8]
	cbnz	r1, .L137
	movw	r1, #345
	ldr	r3, .L143+4
	str	r1, [sp]
	ldr	r2, .L143+8
	ldr	r0, .L143+12
	bl	printk
	b	.L133
.L138:
	mov	r4, r3
	b	.L133
.L139:
	mov	r4, #-1
	b	.L133
.L137:
	mov	r2, r6
	add	r1, r1, r8
	mov	r0, r5
	bl	memcpy
	b	.L133
.L144:
	.align	2
.L143:
	.word	.LC10
	.word	.LANCHOR0+348
	.word	.LC11
	.word	.LC8
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
	cbz	r0, .L146
	cbnz	r2, .L147
.L146:
	ldr	r3, [sp, #32]
	mov	r2, r7
	movw	r1, #361
	ldr	r0, .L156
	mvn	r4, #1
	str	r3, [sp]
	mov	r3, r6
	bl	printk
.L145:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L147:
	cbz	r3, .L150
	ldr	r2, [r0, #4]
	add	r3, r3, r1
	cmp	r3, r2
	bhi	.L151
	bl	mpp_buffer_map
	mov	r4, r0
	cbnz	r0, .L152
	ldr	r0, [r7, #8]
	cbnz	r0, .L149
	movw	r1, #375
	ldr	r3, .L156+4
	str	r1, [sp]
	ldr	r2, .L156+8
	ldr	r0, .L156+12
	bl	printk
	b	.L145
.L150:
	mov	r4, r3
	b	.L145
.L151:
	mvn	r4, #5
	b	.L145
.L152:
	mov	r4, #-1
	b	.L145
.L149:
	mov	r2, r5
	mov	r1, r6
	add	r0, r0, r8
	bl	memcpy
	b	.L145
.L157:
	.align	2
.L156:
	.word	.LC12
	.word	.LANCHOR0+376
	.word	.LC13
	.word	.LC8
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
	cbnz	r0, .L159
	mov	r2, #388
	ldr	r1, .L166
	ldr	r0, .L166+4
	bl	printk
.L158:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L159:
	cbnz	r4, .L161
	mov	r2, r6
	movw	r1, #393
	ldr	r0, .L166+8
	bl	printk
.L165:
	movs	r5, #0
	b	.L158
.L161:
	mov	r0, r4
	bl	mpp_buffer_map
	cmp	r0, #0
	bne	.L165
	ldr	r3, [r4, #8]
	cbnz	r3, .L163
	mov	r1, #400
	ldr	r3, .L166+12
	str	r1, [sp]
	ldr	r2, .L166+16
	ldr	r0, .L166+20
	bl	printk
	ldr	r3, [r4, #8]
	cbnz	r3, .L163
	mov	r3, r6
	mov	r2, r4
	movw	r1, #403
	ldr	r0, .L166+24
	bl	printk
.L163:
	ldr	r5, [r4, #8]
	b	.L158
.L167:
	.align	2
.L166:
	.word	.LANCHOR0+405
	.word	.LC0
	.word	.LC14
	.word	.LANCHOR0+436
	.word	.LC15
	.word	.LC8
	.word	.LC16
	.fnend
	.size	mpp_buffer_get_ptr_with_caller, .-mpp_buffer_get_ptr_with_caller
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
	ldr	r0, .L172
	mov	r1, #412
	bl	printk
	ldr	r4, [r5, #16]
	cmp	r4, #0
	bgt	.L168
	cbnz	r5, .L170
	mov	r2, r6
	movw	r1, #417
	ldr	r0, .L172+4
	mov	r4, #-1
	bl	printk
.L168:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L170:
	movs	r1, #0
	ldr	r0, [r5, #36]
	bl	dma_buf_fd
	subs	r4, r0, #0
	bge	.L171
	movw	r1, #421
	ldr	r3, .L172+8
	str	r1, [sp]
	ldr	r2, .L172+12
	ldr	r0, .L172+16
	bl	printk
	mov	r3, r4
	mov	r2, r5
	mov	r1, #424
	ldr	r0, .L172+20
	str	r6, [sp]
	bl	printk
.L171:
	mov	r2, r4
	mov	r1, #426
	ldr	r0, .L172+24
	bl	printk
	str	r4, [r5, #16]
	b	.L168
.L173:
	.align	2
.L172:
	.word	.LC17
	.word	.LC18
	.word	.LANCHOR0+467
	.word	.LC19
	.word	.LC8
	.word	.LC20
	.word	.LC21
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
	cbnz	r0, .L175
	mov	r2, r1
	ldr	r0, .L178
	movw	r1, #437
	bl	printk
.L176:
	mov	r0, r4
	pop	{r4, pc}
.L175:
	ldr	r2, [r0, #36]
	cbnz	r2, .L177
	mov	r2, r0
	mov	r1, #442
	ldr	r0, .L178+4
	bl	printk
.L177:
	ldr	r4, [r4, #36]
	b	.L176
.L179:
	.align	2
.L178:
	.word	.LC22
	.word	.LC23
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
	cbnz	r0, .L181
	mov	r2, r1
	ldr	r0, .L184
	mov	r1, #452
	bl	printk
	mov	r0, r4
.L180:
	pop	{r4, pc}
.L181:
	ldr	r2, [r0, #4]
	cbnz	r2, .L183
	mov	r2, r0
	movw	r1, #457
	ldr	r0, .L184+4
	bl	printk
.L183:
	ldr	r0, [r4, #4]
	b	.L180
.L185:
	.align	2
.L184:
	.word	.LC24
	.word	.LC25
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
	cbnz	r0, .L187
	movw	r1, #467
	ldr	r0, .L189
	bl	printk
	mov	r0, #-1
.L186:
	pop	{r3, pc}
.L187:
	ldr	r0, [r0, #20]
	b	.L186
.L190:
	.align	2
.L189:
	.word	.LC26
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
	cbnz	r0, .L192
	mov	r1, #480
	ldr	r0, .L194
	bl	printk
	mvn	r0, #1
.L191:
	pop	{r3, pc}
.L192:
	str	r1, [r0, #20]
	movs	r0, #0
	b	.L191
.L195:
	.align	2
.L194:
	.word	.LC27
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
	cbnz	r0, .L197
	mov	r1, #494
	ldr	r0, .L199
	bl	printk
	mov	r0, #-1
.L196:
	pop	{r3, pc}
.L197:
	ldr	r0, [r0, #40]
	b	.L196
.L200:
	.align	2
.L199:
	.word	.LC28
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
	cbnz	r0, .L202
	movw	r1, #507
	ldr	r0, .L204
	bl	printk
	mvn	r0, #1
.L201:
	pop	{r3, pc}
.L202:
	str	r1, [r0, #40]
	movs	r0, #0
	b	.L201
.L205:
	.align	2
.L204:
	.word	.LC29
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
	cbnz	r0, .L207
	movw	r2, #522
	ldr	r1, .L218
	ldr	r0, .L218+4
	bl	printk
	mov	r0, #-1
.L206:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L207:
	cbz	r4, .L209
	cbnz	r5, .L210
.L209:
	str	r6, [sp]
	mov	r3, r5
	mov	r2, r4
	movw	r1, #527
	ldr	r0, .L218+8
	bl	printk
	mvn	r0, #1
	b	.L206
.L210:
	ldr	r3, [r4, #8]
	cbnz	r3, .L211
	ldr	r3, [r0, #4]
	cbz	r3, .L211
	ldr	r0, [r4, #32]
	blx	r3
	str	r0, [r4, #8]
.L211:
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldm	r4, {r0, r1, r2, r3}
	stm	r5, {r0, r1, r2, r3}
	movs	r0, #0
	b	.L206
.L219:
	.align	2
.L218:
	.word	.LANCHOR0+497
	.word	.LC0
	.word	.LC30
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
	cbnz	r4, .L221
	mov	r1, #544
	ldr	r0, .L225
	bl	printk
	mvn	r0, #1
.L220:
	pop	{r4, r5, r6, pc}
.L221:
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #20]
	ldr	r1, [r4, #4]
	adds	r6, r2, r3
	ldr	r0, [r4, #36]
	cmp	r6, r1
	bcc	.L223
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
.L224:
	bl	dma_buf_begin_cpu_access_partial
	movs	r0, #0
	b	.L220
.L223:
	movs	r1, #2
	b	.L224
.L226:
	.align	2
.L225:
	.word	.LC29
	.fnend
	.size	mpp_buffer_flush_for_cpu_with_caller, .-mpp_buffer_flush_for_cpu_with_caller
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
	cbnz	r0, .L228
	movw	r2, #567
	ldr	r1, .L235
	ldr	r0, .L235+4
	bl	printk
.L233:
	mov	r0, #-1
	b	.L227
.L228:
	cbnz	r4, .L230
	mov	r3, r6
	mov	r2, r4
	movw	r1, #573
	ldr	r0, .L235+8
	bl	printk
	mvn	r0, #1
.L227:
	add	sp, sp, #104
	@ sp needed
	pop	{r4, r5, r6, pc}
.L230:
	movs	r2, #104
	movs	r1, #0
	mov	r0, sp
	bl	memset
	ldr	r3, [r5, #44]
	cbz	r3, .L231
	mov	r2, #-1
	mov	r1, sp
	ldr	r0, [r4, #32]
	blx	r3
	cmp	r0, #0
	bne	.L233
	ldr	r0, [sp, #56]
	b	.L227
.L231:
	movw	r1, #581
	ldr	r0, .L235+12
	bl	printk
	b	.L233
.L236:
	.align	2
.L235:
	.word	.LANCHOR0+529
	.word	.LC0
	.word	.LC31
	.word	.LC32
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
	cbnz	r0, .L238
	movw	r1, #593
	ldr	r0, .L239
	b	printk
.L238:
	movs	r3, #1
	strd	r3, r1, [r0, #24]
	bx	lr
.L240:
	.align	2
.L239:
	.word	.LC28
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
	push	{r3, lr}
	.save {r3, lr}
	mov	r2, r1
	cbnz	r0, .L242
	movw	r1, #606
	ldr	r0, .L245
	bl	printk
.L244:
	mov	r0, #-1
	b	.L241
.L242:
	ldr	r3, [r0, #24]
	cmp	r3, #0
	beq	.L244
	ldr	r0, [r0, #28]
.L241:
	pop	{r3, pc}
.L246:
	.align	2
.L245:
	.word	.LC28
	.fnend
	.size	mpp_buffer_get_phy_caller, .-mpp_buffer_get_phy_caller
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.34598, %object
	.size	__func__.34598, 27
__func__.34598:
	.ascii	"mpp_buffer_import_with_tag\000"
	.type	__FUNCTION__.34600, %object
	.size	__FUNCTION__.34600, 27
__FUNCTION__.34600:
	.ascii	"mpp_buffer_import_with_tag\000"
	.type	__func__.34608, %object
	.size	__func__.34608, 23
__func__.34608:
	.ascii	"mpi_buf_alloc_with_tag\000"
	.type	__func__.34615, %object
	.size	__func__.34615, 21
__func__.34615:
	.ascii	"mpi_buf_ref_with_tag\000"
	.type	__func__.34622, %object
	.size	__func__.34622, 23
__func__.34622:
	.ascii	"mpi_buf_unref_with_tag\000"
	.type	__func__.34629, %object
	.size	__func__.34629, 28
__func__.34629:
	.ascii	"mpi_buf_get_dma_with_caller\000"
	.type	__func__.34640, %object
	.size	__func__.34640, 24
__func__.34640:
	.ascii	"mpp_buffer_get_with_tag\000"
	.type	__FUNCTION__.34641, %object
	.size	__FUNCTION__.34641, 24
__FUNCTION__.34641:
	.ascii	"mpp_buffer_get_with_tag\000"
	.type	__func__.34652, %object
	.size	__func__.34652, 29
__func__.34652:
	.ascii	"mpp_ring_buffer_get_with_tag\000"
	.type	__FUNCTION__.34653, %object
	.size	__FUNCTION__.34653, 29
__FUNCTION__.34653:
	.ascii	"mpp_ring_buffer_get_with_tag\000"
	.type	__func__.34660, %object
	.size	__func__.34660, 27
__func__.34660:
	.ascii	"mpp_buffer_put_with_caller\000"
	.type	__FUNCTION__.34661, %object
	.size	__FUNCTION__.34661, 27
__FUNCTION__.34661:
	.ascii	"mpp_buffer_put_with_caller\000"
	.type	__FUNCTION__.34671, %object
	.size	__FUNCTION__.34671, 24
__FUNCTION__.34671:
	.ascii	"mpp_buffer_map_ring_ptr\000"
	.type	__func__.34686, %object
	.size	__func__.34686, 15
__func__.34686:
	.ascii	"mpp_buffer_map\000"
	.type	__FUNCTION__.34701, %object
	.size	__FUNCTION__.34701, 28
__FUNCTION__.34701:
	.ascii	"mpp_buffer_read_with_caller\000"
	.type	__FUNCTION__.34713, %object
	.size	__FUNCTION__.34713, 29
__FUNCTION__.34713:
	.ascii	"mpp_buffer_write_with_caller\000"
	.type	__func__.34722, %object
	.size	__func__.34722, 31
__func__.34722:
	.ascii	"mpp_buffer_get_ptr_with_caller\000"
	.type	__FUNCTION__.34723, %object
	.size	__FUNCTION__.34723, 31
__FUNCTION__.34723:
	.ascii	"mpp_buffer_get_ptr_with_caller\000"
	.type	__FUNCTION__.34732, %object
	.size	__FUNCTION__.34732, 30
__FUNCTION__.34732:
	.ascii	"mpp_buffer_get_fd_with_caller\000"
	.type	__func__.34774, %object
	.size	__func__.34774, 32
__func__.34774:
	.ascii	"mpp_buffer_info_get_with_caller\000"
	.type	__func__.34787, %object
	.size	__func__.34787, 38
__func__.34787:
	.ascii	"mpp_buffer_get_mpi_buf_id_with_caller\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: mpibuf_ops get fail\000"
.LC1:
	.ascii	"\0013%d: mpp_buffer_commit invalid input: info NULL"
	.ascii	" from %s\012\000"
.LC2:
	.ascii	"\0013%d: mpp_buffer_import fail %s\012\000"
.LC3:
	.ascii	"\0013%d: buf impl malloc fail : group %p buffer %p "
	.ascii	"size %u from %s\012\000"
.LC4:
	.ascii	"\0013%d: mpp_buffer_get invalid input: group %p buf"
	.ascii	"fer %p size %u from %s\012\000"
.LC5:
	.ascii	"\0013%d: mpp_buffer_put invalid input: buffer NULL "
	.ascii	"from %s\012\000"
.LC6:
	.ascii	"\0013%d: alloc buf start is no 4k align\000"
.LC7:
	.ascii	"i < page_count\000"
.LC8:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC9:
	.ascii	"\0013%d: mpp_buffer_inc_ref invalid input: buffer N"
	.ascii	"ULL from %s\012\000"
.LC10:
	.ascii	"\0013%d: mpp_buffer_read invalid input: buffer %p d"
	.ascii	"ata %p from %s\012\000"
.LC11:
	.ascii	"src != ((void *)0)\000"
.LC12:
	.ascii	"\0013%d: mpp_buffer_write invalid input: buffer %p "
	.ascii	"data %p from %s\012\000"
.LC13:
	.ascii	"dst != ((void *)0)\000"
.LC14:
	.ascii	"\0013%d: mpp_buffer_get_ptr invalid NULL input from"
	.ascii	" %s\012\000"
.LC15:
	.ascii	"p->info.ptr != ((void *)0)\000"
.LC16:
	.ascii	"\0013%d: mpp_buffer_get_ptr buffer %p ret NULL from"
	.ascii	" %s\012\000"
.LC17:
	.ascii	"\0016%d: p->info.fd %d\000"
.LC18:
	.ascii	"\0013%d: mpp_buffer_get_fd invalid NULL input from "
	.ascii	"%s\012\000"
.LC19:
	.ascii	"fd >= 0\000"
.LC20:
	.ascii	"\0013%d: mpp_buffer_get_fd buffer %p fd %d from %s\012"
	.ascii	"\000"
.LC21:
	.ascii	"\0016%d: dma_buf_fd fd %d\000"
.LC22:
	.ascii	"\0013%d: mpp_buffer_get_dma invalid NULL input from"
	.ascii	" %s\012\000"
.LC23:
	.ascii	"\0013%d: mpp_buffer_get_fd buffer %p from %s\012\000"
.LC24:
	.ascii	"\0013%d: mpp_buffer_get_size invalid NULL input fro"
	.ascii	"m %s\012\000"
.LC25:
	.ascii	"\0013%d: mpp_buffer_get_size buffer %p ret zero siz"
	.ascii	"e from %s\012\000"
.LC26:
	.ascii	"\0013%d: mpp_buffer_get_index invalid NULL input fr"
	.ascii	"om %s\012\000"
.LC27:
	.ascii	"\0013%d: mpp_buffer_set_index invalid NULL input fr"
	.ascii	"om %s\012\000"
.LC28:
	.ascii	"\0013%d: mpp_buffer_get_offset invalid NULL input f"
	.ascii	"rom %s\012\000"
.LC29:
	.ascii	"\0013%d: mpp_buffer_set_offset invalid NULL input f"
	.ascii	"rom %s\012\000"
.LC30:
	.ascii	"\0013%d: mpp_buffer_info_get invalid input buffer %"
	.ascii	"p info %p from %s\012\000"
.LC31:
	.ascii	"\0013%d: mpp_buffer_get_mpi_buf_id invalid input bu"
	.ascii	"ffer %p from %s\012\000"
.LC32:
	.ascii	"\0013%d: get buf info fail\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
