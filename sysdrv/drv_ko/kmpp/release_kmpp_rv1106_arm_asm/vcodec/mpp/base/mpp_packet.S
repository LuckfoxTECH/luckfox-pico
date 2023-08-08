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
	cbz	r0, .L18
	ldr	r1, [r0]
	ldr	r2, .L24
	cmp	r1, r2
	beq	.L20
.L18:
	movs	r2, #38
	ldr	r1, .L24+4
	ldr	r0, .L24+8
	bl	printk
	mov	r0, #-1
.L17:
	pop	{r3, pc}
.L20:
	movs	r0, #0
	b	.L17
.L25:
	.align	2
.L24:
	.word	.LC0
	.word	.LANCHOR0
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
	cbnz	r0, .L27
	ldr	r1, .L31
	movs	r2, #48
	ldr	r0, .L31+4
.L30:
	bl	printk
	mvn	r0, #2
.L26:
	pop	{r4, pc}
.L27:
	bl	mpp_packet_mem_alloc
	cbnz	r0, .L29
	movs	r2, #54
	ldr	r1, .L31
	ldr	r0, .L31+8
	b	.L30
.L29:
	str	r0, [r4]
	add	r3, r0, #12
	str	r3, [r0, #12]
	str	r3, [r0, #16]
	movs	r3, #1
	str	r3, [r0, #20]
	ldr	r3, .L31+12
	str	r3, [r0]
	movs	r0, #0
	b	.L26
.L32:
	.align	2
.L31:
	.word	.LANCHOR0+20
	.word	.LC2
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
	@ args = 4, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r6, r1
	.pad #28
	sub	sp, sp, #28
	mov	r5, r2
	mov	r7, r3
	mov	fp, r0
	cbnz	r0, .L34
	movs	r2, #156
	ldr	r1, .L53
	ldr	r0, .L53+4
	bl	printk
.L45:
	mvn	r0, #2
	b	.L33
.L34:
	bl	mpp_packet_mem_alloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L45
	add	r3, r0, #12
	str	r3, [r0, #12]
	str	r3, [r0, #16]
	movs	r3, #1
	str	r3, [r0, #20]
	cbz	r6, .L36
	str	r3, [sp]
	add	r1, r0, #72
	ubfx	r3, r5, #0, #10
	movs	r2, #128
	mov	r0, r6
	bl	ring_buf_get_free
	cmp	r0, #0
	beq	.L37
.L39:
	mov	r0, r4
	bl	mpp_packet_mem_free
	mvn	r0, #3
.L33:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L36:
	bl	get_vsm_ops
	strd	r6, r6, [sp, #12]
	mov	r9, r0
	str	r6, [sp, #20]
	cbnz	r0, .L38
	movs	r1, #77
	ldr	r0, .L53+8
	bl	printk
	b	.L39
.L38:
	ldr	r5, [r0]
	cmp	r5, #0
	beq	.L39
	ldr	r3, [sp, #64]
	mov	r2, r6
	mov	r1, r7
	add	r0, sp, #12
	blx	r5
	cmp	r0, #0
	blt	.L39
	ldr	r7, [sp, #12]
	mov	r1, #3264
	ldr	r3, [sp, #20]
	subs	r5, r7, #1
	str	r6, [r4, #84]
	add	r5, r5, r3
	mvn	r5, r5, lsr #12
	mvn	r5, r5, lsl #12
	adds	r5, r5, #1
	sub	r8, r5, r7
	lsr	r8, r8, #12
	add	r8, r8, #1
	lsl	r0, r8, #2
	bl	__kmalloc
	mov	r10, r0
	cmp	r0, #0
	bne	.L46
	ldr	r3, [r9, #8]
	cmp	r3, #0
	beq	.L39
	ldr	r1, [sp, #64]
	add	r0, sp, #12
	blx	r3
	b	.L39
.L44:
	cmp	r8, r9
	bgt	.L43
	movs	r1, #102
	ldr	r3, .L53+12
	str	r1, [sp]
	ldr	r2, .L53+16
	ldr	r0, .L53+20
	bl	printk
.L43:
	ldr	r3, .L53+24
	ldr	r2, [r3]
	ldr	r3, .L53+28
	rsb	r2, r2, r7, lsr #12
	add	r7, r7, #4096
	ldr	r3, [r3]
	add	r3, r3, r2, lsl #5
	str	r3, [r10, r9, lsl #2]
	add	r9, r9, #1
.L41:
	cmp	r5, r7
	bhi	.L44
	ldr	r3, .L53+32
	movs	r2, #4
	mov	r1, r9
	mov	r0, r10
	ldr	r3, [r3]
	orr	r3, r3, #512
	bl	vmap
	ldr	r2, [sp, #12]
	ubfx	r3, r2, #0, #12
	add	r0, r0, r3
	ldr	r3, [sp, #20]
	strd	r2, r0, [r4, #76]
	mov	r0, r10
	str	r3, [r4, #96]
	ldr	r3, [r4, #60]
	orr	r3, r3, #8
	str	r3, [r4, #60]
	bl	kfree
.L37:
	ldr	r3, [r4, #80]
	movs	r0, #0
	str	r6, [r4, #104]
	str	r0, [r4, #36]
	strd	r3, r3, [r4, #24]
	ldr	r3, [r4, #96]
	str	r3, [r4, #32]
	ldr	r3, .L53+36
	str	r3, [r4]
	str	r4, [fp]
	b	.L33
.L46:
	mov	r9, r6
	b	.L41
.L54:
	.align	2
.L53:
	.word	.LANCHOR0+35
	.word	.LC2
	.word	.LC4
	.word	.LANCHOR0+59
	.word	.LC5
	.word	.LC6
	.word	__pv_phys_pfn_offset
	.word	mem_map
	.word	pgprot_kernel
	.word	.LC0
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
	cbz	r0, .L56
	ldr	r2, [r4, #96]
	cmp	r3, r2
	bls	.L57
	movs	r1, #200
	ldr	r0, .L71
	bl	printk
	mov	r0, #-1
.L55:
	pop	{r4, r5, r6, pc}
.L57:
	add	r1, r4, #72
	bl	ring_buf_put_use
.L56:
	ldr	r3, [r4, #60]
	lsls	r3, r3, #28
	bpl	.L59
	ldr	r3, [r4, #36]
	cbz	r3, .L60
	mov	r0, r6
	bl	mpp_get_dev
	movs	r3, #2
	ldr	r2, [r4, #36]
	ldr	r1, [r4, #76]
	bl	dma_sync_single_for_device
.L60:
	mov	r1, r5
	mov	r0, r4
	bl	mpp_packet_vsm_buf_free
.L59:
	ldr	r0, [r4, #72]
	cmp	r0, #0
	beq	.L55
	ldr	r1, .L71+4
	add	r0, r4, #72
	bl	mpp_buffer_flush_for_cpu_with_caller
	movs	r0, #0
	b	.L55
.L72:
	.align	2
.L71:
	.word	.LC7
	.word	.LANCHOR0+84
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
	cbnz	r0, .L74
	movs	r2, #225
	ldr	r1, .L77
	ldr	r0, .L77+4
	mvn	r4, #2
	bl	printk
.L73:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L74:
	bl	mpp_packet_new
	mov	r4, r0
	cbz	r0, .L76
	movs	r2, #231
	ldr	r1, .L77
	ldr	r0, .L77+8
	bl	printk
	b	.L73
.L76:
	ldr	r3, [r7]
	strd	r6, r6, [r3, #24]
	strd	r5, r5, [r3, #32]
	b	.L73
.L78:
	.align	2
.L77:
	.word	.LANCHOR0+113
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
	cbz	r0, .L80
	cbnz	r1, .L81
.L80:
	str	r4, [sp]
	mov	r3, r5
	movs	r2, #248
	ldr	r1, .L87
	ldr	r0, .L87+4
	mvn	r6, #2
	bl	printk
.L79:
	mov	r0, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L81:
	bl	mpp_packet_new
	mov	r6, r0
	cbz	r0, .L83
	movs	r2, #254
	ldr	r1, .L87
	ldr	r0, .L87+8
	bl	printk
	b	.L79
.L83:
	ldr	r1, .L87+12
	mov	r0, r4
	ldr	r5, [r5]
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L87+12
	strd	r0, r0, [r5, #24]
	mov	r0, r4
	bl	mpp_buffer_get_size_with_caller
	str	r4, [r5, #68]
	ldr	r1, .L87+12
	strd	r0, r0, [r5, #32]
	mov	r0, r4
	bl	mpp_buffer_inc_ref_with_caller
	b	.L79
.L88:
	.align	2
.L87:
	.word	.LANCHOR0+129
	.word	.LC10
	.word	.LC9
	.word	.LANCHOR0+157
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
	cbnz	r0, .L90
.L92:
	mov	r2, #272
	ldr	r1, .L109
	ldr	r0, .L109+4
	mvn	r6, #2
	bl	printk
.L89:
	mov	r0, r6
	pop	{r4, r5, r6, pc}
.L90:
	ldr	r0, [r0]
	bl	check_is_mpp_packet
	mov	r6, r0
	cmp	r0, #0
	bne	.L92
	ldr	r4, [r5]
	ldr	r0, [r4, #68]
	cbz	r0, .L93
	ldr	r1, .L109+8
	bl	mpp_buffer_put_with_caller
.L93:
	ldr	r3, [r4, #60]
	lsls	r2, r3, #29
	bpl	.L94
	ldr	r1, [r4, #24]
	ldr	r0, .L109+8
	bl	mpp_osal_free
.L94:
	ldr	r0, [r4, #104]
	cbz	r0, .L95
	add	r1, r4, #72
	bl	ring_buf_put_free
	movs	r3, #0
	str	r3, [r4, #104]
.L95:
	ldr	r3, [r4, #60]
	lsls	r3, r3, #28
	bpl	.L96
	ldr	r0, [r4, #80]
	bl	vunmap
.L96:
	mov	r0, r4
	bl	mpp_packet_mem_free
	movs	r3, #0
	str	r3, [r5]
	b	.L89
.L110:
	.align	2
.L109:
	.word	.LANCHOR0+185
	.word	.LC11
	.word	.LANCHOR0+203
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
	cbnz	r0, .L111
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
	bls	.L116
	movw	r1, #323
	ldr	r3, .L118
	str	r1, [sp]
	ldr	r2, .L118+4
	ldr	r0, .L118+8
	bl	printk
.L116:
	ldrd	r2, r3, [r4, #32]
	cmp	r2, r3
	bcs	.L111
	mov	r1, #324
	ldr	r3, .L118
	str	r1, [sp]
	ldr	r2, .L118+12
	ldr	r0, .L118+8
	bl	printk
.L111:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L119:
	.align	2
.L118:
	.word	.LANCHOR0+221
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
	cbnz	r0, .L122
	ldr	r0, [r4, #28]
.L120:
	pop	{r4, pc}
.L122:
	movs	r0, #0
	b	.L120
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
	cbnz	r0, .L125
	ldr	r3, [r4, #60]
	orr	r3, r3, #1
	str	r3, [r4, #60]
.L123:
	pop	{r4, pc}
.L125:
	mvn	r0, #1
	b	.L123
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
	cbnz	r0, .L128
	ldr	r3, [r4, #60]
	bic	r3, r3, #1
	str	r3, [r4, #60]
.L126:
	pop	{r4, pc}
.L128:
	mvn	r0, #1
	b	.L126
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
	cbnz	r0, .L131
	ldr	r0, [r4, #60]
	and	r0, r0, #1
.L129:
	pop	{r4, pc}
.L131:
	movs	r0, #0
	b	.L129
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
	cbnz	r0, .L134
	mov	r1, r0
	movs	r2, #108
	adds	r0, r4, #4
	ldr	r6, [r4, #24]
	ldr	r7, [r4, #32]
	bl	memset
	ldr	r3, .L135
	strd	r6, r6, [r4, #24]
	str	r7, [r4, #32]
	str	r3, [r4]
.L132:
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, pc}
.L134:
	mvn	r5, #1
	b	.L132
.L136:
	.align	2
.L135:
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
	cbnz	r0, .L140
	ldr	r0, [r4, #68]
.L138:
	pop	{r4, pc}
.L140:
	movs	r0, #0
	b	.L138
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
	cbnz	r0, .L143
	str	r5, [r4, #56]
.L141:
	pop	{r3, r4, r5, pc}
.L143:
	mvn	r0, #1
	b	.L141
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
	cbnz	r0, .L146
	ldr	r3, [r5, #56]
.L145:
	str	r3, [r4]
	pop	{r3, r4, r5, pc}
.L146:
	movs	r3, #0
	mvn	r0, #1
	b	.L145
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
	cbnz	r0, .L149
	ldrb	r0, [r4, #56]	@ zero_extendqisi2
	ubfx	r0, r0, #4, #1
.L147:
	pop	{r4, pc}
.L149:
	movs	r0, #0
	b	.L147
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
	cbnz	r0, .L152
	ldrb	r0, [r4, #56]	@ zero_extendqisi2
	ubfx	r0, r0, #5, #1
.L150:
	pop	{r4, pc}
.L152:
	movs	r0, #0
	b	.L150
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
	cbnz	r0, .L155
	ldrb	r0, [r4, #56]	@ zero_extendqisi2
	ubfx	r0, r0, #6, #1
.L153:
	pop	{r4, pc}
.L155:
	movs	r0, #0
	b	.L153
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
	cbnz	r0, .L157
	cbnz	r5, .L158
.L157:
	str	r5, [sp]
	mov	r3, r6
	mov	r2, #484
	ldr	r1, .L164
	ldr	r0, .L164+4
	mvn	r4, #1
	bl	printk
.L156:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L158:
	cmp	r7, #0
	beq	.L156
	mov	r0, r6
	bl	check_is_mpp_packet
	ldr	r6, [r6, #24]
	cbnz	r6, .L160
	mov	r1, #492
	ldr	r3, .L164+8
	str	r1, [sp]
	ldr	r2, .L164+12
	ldr	r0, .L164+16
	bl	printk
.L160:
	mov	r2, r7
	add	r1, r6, r8
	mov	r0, r5
	bl	memcpy
	b	.L156
.L165:
	.align	2
.L164:
	.word	.LANCHOR0+240
	.word	.LC14
	.word	.LANCHOR0+256
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
	cbnz	r0, .L167
	cbnz	r5, .L168
.L167:
	str	r5, [sp]
	mov	r3, r6
	mov	r2, #502
	ldr	r1, .L174
	ldr	r0, .L174+4
	mvn	r4, #1
	bl	printk
.L166:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L168:
	cmp	r7, #0
	beq	.L166
	mov	r0, r6
	bl	check_is_mpp_packet
	ldr	r6, [r6, #24]
	cbnz	r6, .L170
	mov	r1, #510
	ldr	r3, .L174+8
	str	r1, [sp]
	ldr	r2, .L174+12
	ldr	r0, .L174+16
	bl	printk
.L170:
	mov	r2, r7
	mov	r1, r5
	add	r0, r6, r8
	bl	memcpy
	b	.L166
.L175:
	.align	2
.L174:
	.word	.LANCHOR0+272
	.word	.LC14
	.word	.LANCHOR0+289
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
	cbz	r0, .L177
.L179:
	str	r4, [sp]
	mov	r3, r6
	movw	r2, #521
	ldr	r1, .L180
	ldr	r0, .L180+4
	mvn	r5, #1
	bl	printk
.L176:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L177:
	mov	r0, r4
	bl	check_is_mpp_packet
	mov	r5, r0
	cmp	r0, #0
	bne	.L179
	ldr	r2, [r4, #36]
	ldr	r1, [r4, #28]
	ldr	r0, [r6, #28]
	bl	memcpy
	ldr	r3, [r4, #36]
	str	r3, [r6, #36]
	b	.L176
.L181:
	.align	2
.L180:
	.word	.LANCHOR0+306
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
	cbz	r0, .L183
.L185:
	str	r5, [sp]
	mov	r3, r4
	movw	r2, #539
	ldr	r1, .L186
	ldr	r0, .L186+4
	mvn	r6, #1
	bl	printk
.L182:
	mov	r0, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L183:
	mov	r0, r5
	bl	check_is_mpp_packet
	mov	r6, r0
	cmp	r0, #0
	bne	.L185
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
	b	.L182
.L187:
	.align	2
.L186:
	.word	.LANCHOR0+322
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
	cbz	r0, .L189
	mov	r3, r4
	mov	r2, #556
	ldr	r1, .L190
	ldr	r0, .L190+4
	pop	{r4, r5, r6, lr}
	b	printk
.L189:
	ldr	r3, [r4, #60]
	orrs	r3, r3, r5
	str	r3, [r4, #60]
	pop	{r4, r5, r6, pc}
.L191:
	.align	2
.L190:
	.word	.LANCHOR0+340
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
	cbz	r0, .L193
	mov	r3, r4
	mov	r2, #568
	ldr	r1, .L195
	ldr	r0, .L195+4
	bl	printk
	movs	r0, #0
.L192:
	pop	{r4, pc}
.L193:
	ldr	r0, [r4, #60]
	b	.L192
.L196:
	.align	2
.L195:
	.word	.LANCHOR0+360
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
	.set	.LANCHOR0,. + 0
	.type	__func__.34500, %object
	.size	__func__.34500, 20
__func__.34500:
	.ascii	"check_is_mpp_packet\000"
	.type	__func__.34507, %object
	.size	__func__.34507, 15
__func__.34507:
	.ascii	"mpp_packet_new\000"
	.type	__func__.34541, %object
	.size	__func__.34541, 24
__func__.34541:
	.ascii	"mpp_packet_new_ring_buf\000"
	.type	__FUNCTION__.34520, %object
	.size	__FUNCTION__.34520, 25
__FUNCTION__.34520:
	.ascii	"mpp_packet_vsm_buf_alloc\000"
	.type	__FUNCTION__.34549, %object
	.size	__FUNCTION__.34549, 29
__FUNCTION__.34549:
	.ascii	"mpp_packet_ring_buf_put_used\000"
	.type	__func__.34557, %object
	.size	__func__.34557, 16
__func__.34557:
	.ascii	"mpp_packet_init\000"
	.type	__func__.34564, %object
	.size	__func__.34564, 28
__func__.34564:
	.ascii	"mpp_packet_init_with_buffer\000"
	.type	__FUNCTION__.34565, %object
	.size	__FUNCTION__.34565, 28
__FUNCTION__.34565:
	.ascii	"mpp_packet_init_with_buffer\000"
	.type	__func__.34570, %object
	.size	__func__.34570, 18
__func__.34570:
	.ascii	"mpp_packet_deinit\000"
	.type	__FUNCTION__.34571, %object
	.size	__FUNCTION__.34571, 18
__FUNCTION__.34571:
	.ascii	"mpp_packet_deinit\000"
	.type	__FUNCTION__.34579, %object
	.size	__FUNCTION__.34579, 19
__FUNCTION__.34579:
	.ascii	"mpp_packet_set_pos\000"
	.type	__func__.34642, %object
	.size	__func__.34642, 16
__func__.34642:
	.ascii	"mpp_packet_read\000"
	.type	__FUNCTION__.34643, %object
	.size	__FUNCTION__.34643, 16
__FUNCTION__.34643:
	.ascii	"mpp_packet_read\000"
	.type	__func__.34653, %object
	.size	__func__.34653, 17
__func__.34653:
	.ascii	"mpp_packet_write\000"
	.type	__FUNCTION__.34654, %object
	.size	__FUNCTION__.34654, 17
__FUNCTION__.34654:
	.ascii	"mpp_packet_write\000"
	.type	__func__.34663, %object
	.size	__func__.34663, 16
__func__.34663:
	.ascii	"mpp_packet_copy\000"
	.type	__func__.34670, %object
	.size	__func__.34670, 18
__func__.34670:
	.ascii	"mpp_packet_append\000"
	.type	__func__.34676, %object
	.size	__func__.34676, 20
__func__.34676:
	.ascii	"mpp_packet_set_flag\000"
	.type	__func__.34681, %object
	.size	__func__.34681, 20
__func__.34681:
	.ascii	"mpp_packet_get_flag\000"
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
