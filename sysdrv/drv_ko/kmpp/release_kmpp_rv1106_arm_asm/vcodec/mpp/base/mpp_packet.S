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
	.file	"mpp_packet.c"
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
	.type	mpp_packet_vsm_buf_free, %function
mpp_packet_vsm_buf_free:
	.fnstart
	@ args = 0, pretend = 0, frame = 200
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #200
	sub	sp, sp, #200
	mov	r4, r0
	mov	r6, r1
	bl	get_vsm_ops
	mov	r8, #0
	mov	r5, r0
	str	r8, [sp, #8]
	cmp	r0, #0
	beq	.L5
	ldr	r3, [r4, #96]
	ldr	r0, [r4, #76]
	str	r3, [sp, #12]
	ldr	r3, [r4, #80]
	str	r0, [sp, #4]
	ubfx	r0, r0, #0, #12
	subs	r0, r3, r0
	bl	vunmap
	ldr	r3, [r5, #4]
	cbz	r3, .L3
	ldr	r7, [r4, #36]
	cbz	r7, .L3
	movs	r2, #184
	mov	r1, r8
	add	r0, sp, #16
	bl	memset
	ldrd	r2, [r4, #40]
	strd	r2, [sp, #40]
	add	r1, sp, #16
	ldr	r3, [r4, #64]
	movs	r2, #0
	str	r7, [sp, #32]
	add	r0, sp, #4
	str	r3, [sp, #60]
	ldr	r3, [r4, #60]
	lsls	r3, r3, #27
	itt	mi
	movmi	r3, #1
	strmi	r3, [sp, #56]
	mov	r3, r6
	ldr	r4, [r5, #4]
	blx	r4
.L16:
	movs	r0, #0
.L1:
	add	sp, sp, #200
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L3:
	ldr	r3, [r5, #8]
	cbz	r3, .L6
	mov	r1, r6
	add	r0, sp, #4
	blx	r3
	b	.L16
.L5:
	mov	r0, #-1
	b	.L1
.L6:
	mov	r0, r3
	b	.L1
	.fnend
	.size	mpp_packet_vsm_buf_free, .-mpp_packet_vsm_buf_free
	.align	1
	.global	mpp_packet_pool_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_pool_init, %function
mpp_packet_pool_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r3, r0
	ldr	r4, .L19
	ldr	r2, [r4]
	cbnz	r2, .L18
	movs	r2, #112
	ldr	r1, .L19+4
	ldr	r0, .L19+8
	bl	mpp_mem_get_pool_f
	str	r0, [r4]
.L18:
	movs	r0, #0
	pop	{r4, pc}
.L20:
	.align	2
.L19:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR1
	.fnend
	.size	mpp_packet_pool_init, .-mpp_packet_pool_init
	.align	1
	.global	mpp_packet_pool_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_pool_deinit, %function
mpp_packet_pool_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L26
	ldr	r1, [r4]
	cbz	r1, .L22
	ldr	r0, .L26+4
	bl	mpp_mem_put_pool_f
	movs	r3, #0
	str	r3, [r4]
.L22:
	movs	r0, #0
	pop	{r4, pc}
.L27:
	.align	2
.L26:
	.word	.LANCHOR0
	.word	.LANCHOR1+21
	.fnend
	.size	mpp_packet_pool_deinit, .-mpp_packet_pool_deinit
	.align	1
	.global	mpp_packet_pool_info_show
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_pool_info_show, %function
mpp_packet_pool_info_show:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L29
	ldr	r1, [r3]
	b	mpp_mem_pool_info_show
.L30:
	.align	2
.L29:
	.word	.LANCHOR0
	.fnend
	.size	mpp_packet_pool_info_show, .-mpp_packet_pool_info_show
	.align	1
	.global	check_is_mpp_packet
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_is_mpp_packet, %function
check_is_mpp_packet:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r3, r0
	cbz	r0, .L32
	ldr	r1, [r0]
	ldr	r2, .L38
	cmp	r1, r2
	beq	.L34
.L32:
	movs	r2, #64
	ldr	r1, .L38+4
	ldr	r0, .L38+8
	bl	printk
	mov	r0, #-1
.L31:
	pop	{r3, pc}
.L34:
	movs	r0, #0
	b	.L31
.L39:
	.align	2
.L38:
	.word	.LC0
	.word	.LANCHOR1+44
	.word	.LC1
	.fnend
	.size	check_is_mpp_packet, .-check_is_mpp_packet
	.align	1
	.global	mpp_packet_new
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_new, %function
mpp_packet_new:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbnz	r0, .L41
	ldr	r1, .L45
	movs	r2, #74
	ldr	r0, .L45+4
.L44:
	bl	printk
	mvn	r0, #2
.L40:
	pop	{r4, pc}
.L41:
	ldr	r3, .L45+8
	ldr	r0, .L45+12
	ldr	r1, [r3]
	bl	mpp_mem_pool_get_f
	cbnz	r0, .L43
	movs	r2, #80
	ldr	r1, .L45
	ldr	r0, .L45+16
	b	.L44
.L43:
	str	r0, [r4]
	add	r3, r0, #12
	str	r3, [r0, #12]
	str	r3, [r0, #16]
	movs	r3, #1
	str	r3, [r0, #20]
	ldr	r3, .L45+20
	str	r3, [r0]
	movs	r0, #0
	b	.L40
.L46:
	.align	2
.L45:
	.word	.LANCHOR1+64
	.word	.LC2
	.word	.LANCHOR0
	.word	.LANCHOR1+79
	.word	.LC3
	.word	.LC0
	.fnend
	.size	mpp_packet_new, .-mpp_packet_new
	.align	1
	.global	mpp_packet_new_ring_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_new_ring_buf, %function
mpp_packet_new_ring_buf:
	.fnstart
	@ args = 4, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #36
	sub	sp, sp, #36
	mov	r5, r1
	mov	r8, r2
	mov	r7, r3
	str	r0, [sp, #12]
	cbnz	r0, .L48
	movs	r2, #182
	ldr	r1, .L68
	ldr	r0, .L68+4
	bl	printk
.L59:
	mvn	r0, #2
	b	.L47
.L48:
	ldr	r6, .L68+8
	ldr	r0, .L68+12
	ldr	r1, [r6]
	mov	fp, r6
	bl	mpp_mem_pool_get_f
	mov	r4, r0
	cmp	r0, #0
	beq	.L59
	add	r3, r0, #12
	str	r3, [r0, #12]
	str	r3, [r0, #16]
	movs	r3, #1
	str	r3, [r0, #20]
	cbz	r5, .L50
	str	r3, [sp]
	add	r1, r0, #72
	ubfx	r3, r8, #0, #10
	movs	r2, #128
	mov	r0, r5
	bl	ring_buf_get_free
	cmp	r0, #0
	beq	.L51
	ldr	r1, [r6]
	mov	r2, r4
.L67:
	ldr	r0, .L68+12
	bl	mpp_mem_pool_put_f
	mvn	r0, #3
.L47:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L50:
	bl	get_vsm_ops
	strd	r5, r5, [sp, #20]
	mov	r9, r0
	str	r5, [sp, #28]
	cbnz	r0, .L52
	movs	r1, #103
	ldr	r0, .L68+16
	bl	printk
.L53:
	mov	r2, r4
	ldr	r1, [fp]
	b	.L67
.L52:
	ldr	r6, [r0]
	cmp	r6, #0
	beq	.L53
	ldr	r3, [sp, #72]
	mov	r2, r5
	mov	r1, r7
	add	r0, sp, #20
	blx	r6
	cmp	r0, #0
	blt	.L53
	ldr	r6, [sp, #20]
	mov	r1, #3264
	ldr	r2, [sp, #28]
	subs	r3, r6, #1
	str	r5, [r4, #84]
	add	r3, r3, r2
	mvn	r3, r3, lsr #12
	mvn	r3, r3, lsl #12
	add	r10, r3, #1
	sub	r7, r10, r6
	lsrs	r7, r7, #12
	adds	r7, r7, #1
	lsls	r0, r7, #2
	bl	__kmalloc
	mov	r8, r0
	cmp	r0, #0
	bne	.L60
	ldr	r3, [r9, #8]
	cmp	r3, #0
	beq	.L53
	ldr	r1, [sp, #72]
	add	r0, sp, #20
	blx	r3
	b	.L53
.L58:
	cmp	r7, r9
	bgt	.L57
	movs	r1, #128
	ldr	r3, .L68+20
	str	r1, [sp]
	mov	r0, fp
	ldr	r2, .L68+24
	bl	printk
.L57:
	ldr	r3, .L68+28
	ldr	r2, [r3]
	ldr	r3, .L68+32
	rsb	r2, r2, r6, lsr #12
	add	r6, r6, #4096
	ldr	r3, [r3]
	add	r3, r3, r2, lsl #5
	str	r3, [r8, r9, lsl #2]
	add	r9, r9, #1
.L55:
	cmp	r10, r6
	bhi	.L58
	ldr	r3, .L68+36
	movs	r2, #4
	mov	r1, r9
	mov	r0, r8
	ldr	r3, [r3]
	orr	r3, r3, #512
	bl	vmap
	ldr	r2, [sp, #20]
	ubfx	r3, r2, #0, #12
	add	r0, r0, r3
	ldr	r3, [sp, #28]
	strd	r2, r0, [r4, #76]
	mov	r0, r8
	str	r3, [r4, #96]
	ldr	r3, [r4, #60]
	orr	r3, r3, #8
	str	r3, [r4, #60]
	bl	kfree
.L51:
	ldr	r3, [r4, #80]
	movs	r0, #0
	str	r5, [r4, #104]
	str	r0, [r4, #36]
	strd	r3, r3, [r4, #24]
	ldr	r3, [r4, #96]
	str	r3, [r4, #32]
	ldr	r3, .L68+40
	str	r3, [r4]
	ldr	r3, [sp, #12]
	str	r4, [r3]
	b	.L47
.L60:
	mov	r9, r5
	ldr	fp, .L68+44
	b	.L55
.L69:
	.align	2
.L68:
	.word	.LANCHOR1+94
	.word	.LC2
	.word	.LANCHOR0
	.word	.LANCHOR1+118
	.word	.LC4
	.word	.LANCHOR1+142
	.word	.LC5
	.word	__pv_phys_pfn_offset
	.word	mem_map
	.word	pgprot_kernel
	.word	.LC0
	.word	.LC6
	.fnend
	.size	mpp_packet_new_ring_buf, .-mpp_packet_new_ring_buf
	.align	1
	.global	mpp_packet_ring_buf_put_used
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_ring_buf_put_used, %function
mpp_packet_ring_buf_put_used:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r3, [r0, #36]
	mov	r5, r1
	mov	r6, r2
	str	r3, [r0, #92]
	ldr	r0, [r0, #104]
	cbz	r0, .L71
	ldr	r2, [r4, #96]
	cmp	r3, r2
	bls	.L72
	movs	r1, #225
	ldr	r0, .L86
	bl	printk
	mov	r0, #-1
.L70:
	pop	{r4, r5, r6, pc}
.L72:
	add	r1, r4, #72
	bl	ring_buf_put_use
.L71:
	ldr	r3, [r4, #60]
	lsls	r3, r3, #28
	bpl	.L74
	ldr	r3, [r4, #36]
	cbz	r3, .L75
	mov	r0, r6
	bl	mpp_get_dev
	movs	r3, #2
	ldr	r2, [r4, #36]
	ldr	r1, [r4, #76]
	bl	dma_sync_single_for_device
.L75:
	mov	r1, r5
	mov	r0, r4
	bl	mpp_packet_vsm_buf_free
.L74:
	ldr	r0, [r4, #72]
	cmp	r0, #0
	beq	.L70
	ldr	r1, .L86+4
	add	r0, r4, #72
	bl	mpp_buffer_flush_for_cpu_with_caller
	movs	r0, #0
	b	.L70
.L87:
	.align	2
.L86:
	.word	.LC7
	.word	.LANCHOR1+167
	.fnend
	.size	mpp_packet_ring_buf_put_used, .-mpp_packet_ring_buf_put_used
	.align	1
	.global	mpp_packet_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_init, %function
mpp_packet_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r1
	mov	r5, r2
	mov	r7, r0
	cbnz	r0, .L89
	movs	r2, #251
	ldr	r1, .L92
	ldr	r0, .L92+4
	mvn	r4, #2
	bl	printk
.L88:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L89:
	bl	mpp_packet_new
	mov	r4, r0
	cbz	r0, .L91
	movw	r2, #257
	ldr	r1, .L92
	ldr	r0, .L92+8
	bl	printk
	b	.L88
.L91:
	ldr	r3, [r7]
	strd	r6, r6, [r3, #24]
	strd	r5, r5, [r3, #32]
	b	.L88
.L93:
	.align	2
.L92:
	.word	.LANCHOR1+196
	.word	.LC8
	.word	.LC9
	.fnend
	.size	mpp_packet_init, .-mpp_packet_init
	.align	1
	.global	mpp_packet_init_with_buffer
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_init_with_buffer, %function
mpp_packet_init_with_buffer:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	mov	r5, r0
	cbz	r0, .L95
	cbnz	r1, .L96
.L95:
	str	r4, [sp]
	mov	r3, r5
	mov	r2, #274
	ldr	r1, .L102
	ldr	r0, .L102+4
	mvn	r6, #2
	bl	printk
.L94:
	mov	r0, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L96:
	bl	mpp_packet_new
	mov	r6, r0
	cbz	r0, .L98
	mov	r2, #280
	ldr	r1, .L102
	ldr	r0, .L102+8
	bl	printk
	b	.L94
.L98:
	ldr	r1, .L102+12
	mov	r0, r4
	ldr	r5, [r5]
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L102+12
	strd	r0, r0, [r5, #24]
	mov	r0, r4
	bl	mpp_buffer_get_size_with_caller
	str	r4, [r5, #68]
	ldr	r1, .L102+12
	strd	r0, r0, [r5, #32]
	mov	r0, r4
	bl	mpp_buffer_inc_ref_with_caller
	b	.L94
.L103:
	.align	2
.L102:
	.word	.LANCHOR1+212
	.word	.LC10
	.word	.LC9
	.word	.LANCHOR1+240
	.fnend
	.size	mpp_packet_init_with_buffer, .-mpp_packet_init_with_buffer
	.align	1
	.global	mpp_packet_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_deinit, %function
mpp_packet_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	cbnz	r0, .L105
.L107:
	mov	r2, #298
	ldr	r1, .L124
	ldr	r0, .L124+4
	mvn	r6, #2
	bl	printk
.L104:
	mov	r0, r6
	pop	{r4, r5, r6, pc}
.L105:
	ldr	r0, [r0]
	bl	check_is_mpp_packet
	mov	r6, r0
	cmp	r0, #0
	bne	.L107
	ldr	r4, [r5]
	ldr	r0, [r4, #68]
	cbz	r0, .L108
	ldr	r1, .L124+8
	bl	mpp_buffer_put_with_caller
.L108:
	ldr	r3, [r4, #60]
	lsls	r2, r3, #29
	bpl	.L109
	ldr	r1, [r4, #24]
	ldr	r0, .L124+8
	bl	mpp_osal_free
.L109:
	ldr	r0, [r4, #104]
	cbz	r0, .L110
	add	r1, r4, #72
	bl	ring_buf_put_free
	movs	r3, #0
	str	r3, [r4, #104]
.L110:
	ldr	r3, [r4, #60]
	lsls	r3, r3, #28
	bpl	.L111
	ldr	r0, [r4, #80]
	bl	vunmap
.L111:
	ldr	r3, .L124+12
	mov	r2, r4
	ldr	r0, .L124+8
	ldr	r1, [r3]
	bl	mpp_mem_pool_put_f
	movs	r3, #0
	str	r3, [r5]
	b	.L104
.L125:
	.align	2
.L124:
	.word	.LANCHOR1+268
	.word	.LC11
	.word	.LANCHOR1+286
	.word	.LANCHOR0
	.fnend
	.size	mpp_packet_deinit, .-mpp_packet_deinit
	.align	1
	.global	mpp_packet_set_pos
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_pos, %function
mpp_packet_set_pos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_packet
	cbnz	r0, .L126
	ldrd	r1, r3, [r4, #24]
	str	r5, [r4, #28]
	subs	r2, r5, r3
	ldr	r3, [r4, #36]
	cmp	r3, r2
	itett	cc
	subcc	r3, r5, r1
	subcs	r3, r3, r2
	ldrcc	r2, [r4, #32]
	subcc	r3, r2, r3
	cmp	r1, r5
	str	r3, [r4, #36]
	bls	.L131
	movw	r1, #349
	ldr	r3, .L133
	str	r1, [sp]
	ldr	r2, .L133+4
	ldr	r0, .L133+8
	bl	printk
.L131:
	ldrd	r2, r3, [r4, #32]
	cmp	r2, r3
	bcs	.L126
	mov	r1, #350
	ldr	r3, .L133
	str	r1, [sp]
	ldr	r2, .L133+12
	ldr	r0, .L133+8
	bl	printk
.L126:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L134:
	.align	2
.L133:
	.word	.LANCHOR1+304
	.word	.LC12
	.word	.LC6
	.word	.LC13
	.fnend
	.size	mpp_packet_set_pos, .-mpp_packet_set_pos
	.align	1
	.global	mpp_packet_get_pos
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_pos, %function
mpp_packet_get_pos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	cbnz	r0, .L137
	ldr	r0, [r4, #28]
.L135:
	pop	{r4, pc}
.L137:
	movs	r0, #0
	b	.L135
	.fnend
	.size	mpp_packet_get_pos, .-mpp_packet_get_pos
	.align	1
	.global	mpp_packet_set_eos
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_eos, %function
mpp_packet_set_eos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	cbnz	r0, .L140
	ldr	r3, [r4, #60]
	orr	r3, r3, #1
	str	r3, [r4, #60]
.L138:
	pop	{r4, pc}
.L140:
	mvn	r0, #1
	b	.L138
	.fnend
	.size	mpp_packet_set_eos, .-mpp_packet_set_eos
	.align	1
	.global	mpp_packet_clr_eos
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_clr_eos, %function
mpp_packet_clr_eos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	cbnz	r0, .L143
	ldr	r3, [r4, #60]
	bic	r3, r3, #1
	str	r3, [r4, #60]
.L141:
	pop	{r4, pc}
.L143:
	mvn	r0, #1
	b	.L141
	.fnend
	.size	mpp_packet_clr_eos, .-mpp_packet_clr_eos
	.align	1
	.global	mpp_packet_get_eos
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_eos, %function
mpp_packet_get_eos:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	cbnz	r0, .L146
	ldr	r0, [r4, #60]
	and	r0, r0, #1
.L144:
	pop	{r4, pc}
.L146:
	movs	r0, #0
	b	.L144
	.fnend
	.size	mpp_packet_get_eos, .-mpp_packet_get_eos
	.align	1
	.global	mpp_packet_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_reset, %function
mpp_packet_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	mov	r5, r0
	cbnz	r0, .L149
	mov	r1, r0
	movs	r2, #108
	adds	r0, r4, #4
	ldr	r6, [r4, #24]
	ldr	r7, [r4, #32]
	bl	memset
	ldr	r3, .L150
	strd	r6, r6, [r4, #24]
	str	r7, [r4, #32]
	str	r3, [r4]
.L147:
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, pc}
.L149:
	mvn	r5, #1
	b	.L147
.L151:
	.align	2
.L150:
	.word	.LC0
	.fnend
	.size	mpp_packet_reset, .-mpp_packet_reset
	.align	1
	.global	mpp_packet_set_buffer
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_buffer, %function
mpp_packet_set_buffer:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	check_is_mpp_packet
	.fnend
	.size	mpp_packet_set_buffer, .-mpp_packet_set_buffer
	.align	1
	.global	mpp_packet_get_buffer
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_buffer, %function
mpp_packet_get_buffer:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	cbnz	r0, .L155
	ldr	r0, [r4, #68]
.L153:
	pop	{r4, pc}
.L155:
	movs	r0, #0
	b	.L153
	.fnend
	.size	mpp_packet_get_buffer, .-mpp_packet_get_buffer
	.align	1
	.global	mpp_packet_set_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_status, %function
mpp_packet_set_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_packet
	cbnz	r0, .L158
	str	r5, [r4, #56]
.L156:
	pop	{r3, r4, r5, pc}
.L158:
	mvn	r0, #1
	b	.L156
	.fnend
	.size	mpp_packet_set_status, .-mpp_packet_set_status
	.align	1
	.global	mpp_packet_get_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_status, %function
mpp_packet_get_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	mov	r4, r1
	bl	check_is_mpp_packet
	cbnz	r0, .L161
	ldr	r3, [r5, #56]
.L160:
	str	r3, [r4]
	pop	{r3, r4, r5, pc}
.L161:
	movs	r3, #0
	mvn	r0, #1
	b	.L160
	.fnend
	.size	mpp_packet_get_status, .-mpp_packet_get_status
	.align	1
	.global	mpp_packet_is_partition
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_is_partition, %function
mpp_packet_is_partition:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	cbnz	r0, .L164
	ldrb	r0, [r4, #56]	@ zero_extendqisi2
	ubfx	r0, r0, #4, #1
.L162:
	pop	{r4, pc}
.L164:
	movs	r0, #0
	b	.L162
	.fnend
	.size	mpp_packet_is_partition, .-mpp_packet_is_partition
	.align	1
	.global	mpp_packet_is_soi
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_is_soi, %function
mpp_packet_is_soi:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	cbnz	r0, .L167
	ldrb	r0, [r4, #56]	@ zero_extendqisi2
	ubfx	r0, r0, #5, #1
.L165:
	pop	{r4, pc}
.L167:
	movs	r0, #0
	b	.L165
	.fnend
	.size	mpp_packet_is_soi, .-mpp_packet_is_soi
	.align	1
	.global	mpp_packet_is_eoi
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_is_eoi, %function
mpp_packet_is_eoi:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	cbnz	r0, .L170
	ldrb	r0, [r4, #56]	@ zero_extendqisi2
	ubfx	r0, r0, #6, #1
.L168:
	pop	{r4, pc}
.L170:
	movs	r0, #0
	b	.L168
	.fnend
	.size	mpp_packet_is_eoi, .-mpp_packet_is_eoi
	.align	1
	.global	mpp_packet_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_read, %function
mpp_packet_read:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r6, r0
	mov	r8, r1
	mov	r5, r2
	mov	r7, r3
	bl	check_is_mpp_packet
	mov	r4, r0
	cbnz	r0, .L172
	cbnz	r5, .L173
.L172:
	str	r5, [sp]
	mov	r3, r6
	mov	r2, #510
	ldr	r1, .L179
	ldr	r0, .L179+4
	mvn	r4, #1
	bl	printk
.L171:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L173:
	cmp	r7, #0
	beq	.L171
	mov	r0, r6
	bl	check_is_mpp_packet
	ldr	r6, [r6, #24]
	cbnz	r6, .L175
	movw	r1, #518
	ldr	r3, .L179+8
	str	r1, [sp]
	ldr	r2, .L179+12
	ldr	r0, .L179+16
	bl	printk
.L175:
	mov	r2, r7
	add	r1, r6, r8
	mov	r0, r5
	bl	memcpy
	b	.L171
.L180:
	.align	2
.L179:
	.word	.LANCHOR1+323
	.word	.LC14
	.word	.LANCHOR1+339
	.word	.LC15
	.word	.LC6
	.fnend
	.size	mpp_packet_read, .-mpp_packet_read
	.align	1
	.global	mpp_packet_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_write, %function
mpp_packet_write:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r6, r0
	mov	r8, r1
	mov	r5, r2
	mov	r7, r3
	bl	check_is_mpp_packet
	mov	r4, r0
	cbnz	r0, .L182
	cbnz	r5, .L183
.L182:
	str	r5, [sp]
	mov	r3, r6
	mov	r2, #528
	ldr	r1, .L189
	ldr	r0, .L189+4
	mvn	r4, #1
	bl	printk
.L181:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L183:
	cmp	r7, #0
	beq	.L181
	mov	r0, r6
	bl	check_is_mpp_packet
	ldr	r6, [r6, #24]
	cbnz	r6, .L185
	mov	r1, #536
	ldr	r3, .L189+8
	str	r1, [sp]
	ldr	r2, .L189+12
	ldr	r0, .L189+16
	bl	printk
.L185:
	mov	r2, r7
	mov	r1, r5
	add	r0, r6, r8
	bl	memcpy
	b	.L181
.L190:
	.align	2
.L189:
	.word	.LANCHOR1+355
	.word	.LC14
	.word	.LANCHOR1+372
	.word	.LC16
	.word	.LC6
	.fnend
	.size	mpp_packet_write, .-mpp_packet_write
	.align	1
	.global	mpp_packet_copy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_copy, %function
mpp_packet_copy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r6, r0
	mov	r4, r1
	bl	check_is_mpp_packet
	cbz	r0, .L192
.L194:
	str	r4, [sp]
	mov	r3, r6
	movw	r2, #547
	ldr	r1, .L195
	ldr	r0, .L195+4
	mvn	r5, #1
	bl	printk
.L191:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L192:
	mov	r0, r4
	bl	check_is_mpp_packet
	mov	r5, r0
	cmp	r0, #0
	bne	.L194
	ldr	r2, [r4, #36]
	ldr	r1, [r4, #28]
	ldr	r0, [r6, #28]
	bl	memcpy
	ldr	r3, [r4, #36]
	str	r3, [r6, #36]
	b	.L191
.L196:
	.align	2
.L195:
	.word	.LANCHOR1+389
	.word	.LC17
	.fnend
	.size	mpp_packet_copy, .-mpp_packet_copy
	.align	1
	.global	mpp_packet_append
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_append, %function
mpp_packet_append:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_packet
	cbz	r0, .L198
.L200:
	str	r5, [sp]
	mov	r3, r4
	movw	r2, #565
	ldr	r1, .L201
	ldr	r0, .L201+4
	mvn	r6, #1
	bl	printk
.L197:
	mov	r0, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L198:
	mov	r0, r5
	bl	check_is_mpp_packet
	mov	r6, r0
	cmp	r0, #0
	bne	.L200
	ldr	r3, [r4, #36]
	ldr	r0, [r4, #28]
	ldr	r2, [r5, #36]
	add	r0, r0, r3
	ldr	r1, [r5, #28]
	bl	memcpy
	ldr	r3, [r4, #36]
	ldr	r2, [r5, #36]
	add	r3, r3, r2
	str	r3, [r4, #36]
	b	.L197
.L202:
	.align	2
.L201:
	.word	.LANCHOR1+405
	.word	.LC17
	.fnend
	.size	mpp_packet_append, .-mpp_packet_append
	.align	1
	.global	mpp_packet_set_flag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_flag, %function
mpp_packet_set_flag:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_packet
	cbz	r0, .L204
	mov	r3, r4
	movw	r2, #582
	ldr	r1, .L205
	ldr	r0, .L205+4
	pop	{r4, r5, r6, lr}
	b	printk
.L204:
	ldr	r3, [r4, #60]
	orrs	r3, r3, r5
	str	r3, [r4, #60]
	pop	{r4, r5, r6, pc}
.L206:
	.align	2
.L205:
	.word	.LANCHOR1+423
	.word	.LC18
	.fnend
	.size	mpp_packet_set_flag, .-mpp_packet_set_flag
	.align	1
	.global	mpp_packet_get_flag
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_flag, %function
mpp_packet_get_flag:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	cbz	r0, .L208
	mov	r3, r4
	movw	r2, #594
	ldr	r1, .L210
	ldr	r0, .L210+4
	bl	printk
	movs	r0, #0
.L207:
	pop	{r4, pc}
.L208:
	ldr	r0, [r4, #60]
	b	.L207
.L211:
	.align	2
.L210:
	.word	.LANCHOR1+443
	.word	.LC18
	.fnend
	.size	mpp_packet_get_flag, .-mpp_packet_get_flag
	.align	1
	.global	mpp_packet_get_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_data, %function
mpp_packet_get_data:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	ldr	r0, [r4, #24]
	pop	{r4, pc}
	.fnend
	.size	mpp_packet_get_data, .-mpp_packet_get_data
	.align	1
	.global	mpp_packet_set_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_data, %function
mpp_packet_set_data:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_packet
	str	r5, [r4, #24]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_packet_set_data, .-mpp_packet_set_data
	.align	1
	.global	mpp_packet_get_size
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_size, %function
mpp_packet_get_size:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	ldr	r0, [r4, #32]
	pop	{r4, pc}
	.fnend
	.size	mpp_packet_get_size, .-mpp_packet_get_size
	.align	1
	.global	mpp_packet_set_size
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_size, %function
mpp_packet_set_size:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_packet
	str	r5, [r4, #32]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_packet_set_size, .-mpp_packet_set_size
	.align	1
	.global	mpp_packet_get_length
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_length, %function
mpp_packet_get_length:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	ldr	r0, [r4, #36]
	pop	{r4, pc}
	.fnend
	.size	mpp_packet_get_length, .-mpp_packet_get_length
	.align	1
	.global	mpp_packet_set_length
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_length, %function
mpp_packet_set_length:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_packet
	str	r5, [r4, #36]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_packet_set_length, .-mpp_packet_set_length
	.align	1
	.global	mpp_packet_get_pts
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_pts, %function
mpp_packet_get_pts:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	ldrd	r0, [r4, #40]
	pop	{r4, pc}
	.fnend
	.size	mpp_packet_get_pts, .-mpp_packet_get_pts
	.align	1
	.global	mpp_packet_set_pts
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_pts, %function
mpp_packet_set_pts:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r6, r7, lr}
	.save {r4, r6, r7, lr}
	mov	r4, r0
	mov	r6, r2
	mov	r7, r3
	bl	check_is_mpp_packet
	strd	r6, [r4, #40]
	pop	{r4, r6, r7, pc}
	.fnend
	.size	mpp_packet_set_pts, .-mpp_packet_set_pts
	.align	1
	.global	mpp_packet_get_dts
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_dts, %function
mpp_packet_get_dts:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	ldrd	r0, [r4, #48]
	pop	{r4, pc}
	.fnend
	.size	mpp_packet_get_dts, .-mpp_packet_get_dts
	.align	1
	.global	mpp_packet_set_dts
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_dts, %function
mpp_packet_set_dts:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r6, r7, lr}
	.save {r4, r6, r7, lr}
	mov	r4, r0
	mov	r6, r2
	mov	r7, r3
	bl	check_is_mpp_packet
	strd	r6, [r4, #48]
	pop	{r4, r6, r7, pc}
	.fnend
	.size	mpp_packet_set_dts, .-mpp_packet_set_dts
	.align	1
	.global	mpp_packet_get_temporal_id
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_get_temporal_id, %function
mpp_packet_get_temporal_id:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	check_is_mpp_packet
	ldr	r0, [r4, #64]
	pop	{r4, pc}
	.fnend
	.size	mpp_packet_get_temporal_id, .-mpp_packet_get_temporal_id
	.align	1
	.global	mpp_packet_set_temporal_id
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_packet_set_temporal_id, %function
mpp_packet_set_temporal_id:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	check_is_mpp_packet
	str	r5, [r4, #64]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_packet_set_temporal_id, .-mpp_packet_set_temporal_id
	.section	.rodata
	.set	.LANCHOR1,. + 0
	.type	__FUNCTION__.34634, %object
	.size	__FUNCTION__.34634, 21
__FUNCTION__.34634:
	.ascii	"mpp_packet_pool_init\000"
	.type	__FUNCTION__.34638, %object
	.size	__FUNCTION__.34638, 23
__FUNCTION__.34638:
	.ascii	"mpp_packet_pool_deinit\000"
	.type	__func__.34645, %object
	.size	__func__.34645, 20
__func__.34645:
	.ascii	"check_is_mpp_packet\000"
	.type	__func__.34652, %object
	.size	__func__.34652, 15
__func__.34652:
	.ascii	"mpp_packet_new\000"
	.type	__FUNCTION__.34653, %object
	.size	__FUNCTION__.34653, 15
__FUNCTION__.34653:
	.ascii	"mpp_packet_new\000"
	.type	__func__.34687, %object
	.size	__func__.34687, 24
__func__.34687:
	.ascii	"mpp_packet_new_ring_buf\000"
	.type	__FUNCTION__.34688, %object
	.size	__FUNCTION__.34688, 24
__FUNCTION__.34688:
	.ascii	"mpp_packet_new_ring_buf\000"
	.type	__FUNCTION__.34666, %object
	.size	__FUNCTION__.34666, 25
__FUNCTION__.34666:
	.ascii	"mpp_packet_vsm_buf_alloc\000"
	.type	__FUNCTION__.34696, %object
	.size	__FUNCTION__.34696, 29
__FUNCTION__.34696:
	.ascii	"mpp_packet_ring_buf_put_used\000"
	.type	__func__.34704, %object
	.size	__func__.34704, 16
__func__.34704:
	.ascii	"mpp_packet_init\000"
	.type	__func__.34711, %object
	.size	__func__.34711, 28
__func__.34711:
	.ascii	"mpp_packet_init_with_buffer\000"
	.type	__FUNCTION__.34712, %object
	.size	__FUNCTION__.34712, 28
__FUNCTION__.34712:
	.ascii	"mpp_packet_init_with_buffer\000"
	.type	__func__.34717, %object
	.size	__func__.34717, 18
__func__.34717:
	.ascii	"mpp_packet_deinit\000"
	.type	__FUNCTION__.34718, %object
	.size	__FUNCTION__.34718, 18
__FUNCTION__.34718:
	.ascii	"mpp_packet_deinit\000"
	.type	__FUNCTION__.34726, %object
	.size	__FUNCTION__.34726, 19
__FUNCTION__.34726:
	.ascii	"mpp_packet_set_pos\000"
	.type	__func__.34789, %object
	.size	__func__.34789, 16
__func__.34789:
	.ascii	"mpp_packet_read\000"
	.type	__FUNCTION__.34790, %object
	.size	__FUNCTION__.34790, 16
__FUNCTION__.34790:
	.ascii	"mpp_packet_read\000"
	.type	__func__.34800, %object
	.size	__func__.34800, 17
__func__.34800:
	.ascii	"mpp_packet_write\000"
	.type	__FUNCTION__.34801, %object
	.size	__FUNCTION__.34801, 17
__FUNCTION__.34801:
	.ascii	"mpp_packet_write\000"
	.type	__func__.34810, %object
	.size	__func__.34810, 16
__func__.34810:
	.ascii	"mpp_packet_copy\000"
	.type	__func__.34817, %object
	.size	__func__.34817, 18
__func__.34817:
	.ascii	"mpp_packet_append\000"
	.type	__func__.34823, %object
	.size	__func__.34823, 20
__func__.34823:
	.ascii	"mpp_packet_set_flag\000"
	.type	__func__.34828, %object
	.size	__func__.34828, 20
__func__.34828:
	.ascii	"mpp_packet_get_flag\000"
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_packet_pool, %object
	.size	g_packet_pool, 4
g_packet_pool:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"mpp_packet\000"
.LC1:
	.ascii	"\0013%s:%d: pointer %p failed on check\012\000"
.LC2:
	.ascii	"\0013%s:%d: invalid NULL input\012\000"
.LC3:
	.ascii	"\0013%s:%d: malloc failed\012\000"
.LC4:
	.ascii	"\0013%d: vcodec_vsm_buf_fn is NULL\000"
.LC5:
	.ascii	"i < page_count\000"
.LC6:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC7:
	.ascii	"\0013%d: ring_buf used may be error\000"
.LC8:
	.ascii	"\0013%s:%d: invalid NULL input packet\012\000"
.LC9:
	.ascii	"\0013%s:%d: new packet failed\012\000"
.LC10:
	.ascii	"\0013%s:%d: invalid input packet %p buffer %p\012\000"
.LC11:
	.ascii	"\0013%s:%d: found NULL input\012\000"
.LC12:
	.ascii	"p->data <= p->pos\000"
.LC13:
	.ascii	"p->size >= p->length\000"
.LC14:
	.ascii	"\0013%s:%d: invalid input: packet %p data %p\012\000"
.LC15:
	.ascii	"src != ((void *)0)\000"
.LC16:
	.ascii	"dst != ((void *)0)\000"
.LC17:
	.ascii	"\0013%s:%d: invalid input: dst %p src %p\012\000"
.LC18:
	.ascii	"\0013%s:%d: invalid input: dst %p\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
