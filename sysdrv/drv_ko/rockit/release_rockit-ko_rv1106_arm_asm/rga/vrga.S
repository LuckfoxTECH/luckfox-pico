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
	.file	"vrga.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	vrga_node_get_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_get_status, %function
vrga_node_get_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vrga_node_get_status, .-vrga_node_get_status
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_compat_ioctl, %function
vrga_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vrga_compat_ioctl, .-vrga_compat_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_open, %function
vrga_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L4
	movs	r0, #0
	add	r2, r3, #8
	str	r2, [r1, #128]
	movs	r2, #18
	str	r2, [r3, #184]
	bx	lr
.L5:
	.align	2
.L4:
	.word	.LANCHOR0
	.fnend
	.size	vrga_open, .-vrga_open
	.align	1
	.global	vrga_node_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_create, %function
vrga_node_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r5, [r1]
	ldr	r6, [r3, #160]
	and	r5, r5, #63
	ldr	r3, [r1, #12]
	cmp	r3, #32
	beq	.L7
	movs	r3, #86
	ldr	r2, .L18
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L18+4
	movs	r0, #0
	bl	vlog
.L10:
	mov	r0, #-1
.L6:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L7:
	ldr	r0, .L18+8
	bl	mpi_allocator_find_by_name
	mov	r7, r0
	cmp	r0, #0
	beq	.L10
	ldr	r2, [r4, #20]
	add	r3, r6, r5, lsl #2
	add	r0, r6, #8
	str	r2, [r3, #204]
	movs	r3, #0
	movs	r2, #1
	ldr	r1, [r4]
	bl	mpi_dev_create_node_with_params
	mov	r6, r0
	cmp	r0, #0
	beq	.L10
	ldr	r8, .L18+16
	mov	r3, r0
	mov	r2, r5
	movs	r1, #0
	str	r8, [r0, #4]
	movs	r0, #18
	bl	vsys_add_node_to_map
	ldrd	r1, r2, [r4, #24]
	mov	r0, r6
	bl	mpi_node_set_in_frame_rate
	ldr	r3, [r4, #20]
	cbnz	r3, .L11
	ldr	r1, [r4, #8]
	ldr	r0, .L18+12
	bl	printk
	ldr	r3, [r4, #16]
	ldr	r2, [r4, #8]
	movs	r1, #1
	mov	r0, r7
	str	r8, [sp]
	bl	mpi_buf_pool_create
	cmp	r0, #0
	beq	.L10
	mov	r1, r6
	bl	mpi_buf_pool_add_user
.L11:
	str	r5, [r4, #4]
	movs	r0, #0
	b	.L6
.L19:
	.align	2
.L18:
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC1
	.word	.LC3
	.word	.LC2
	.fnend
	.size	vrga_node_create, .-vrga_node_create
	.global	__aeabi_uidivmod
	.align	1
	.global	vrga_node_set_crop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_set_crop, %function
vrga_node_set_crop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	mov	r4, r1
	ldr	r3, [r1, #28]
	.pad #44
	sub	sp, sp, #44
	ldr	r2, [r1]
	cmp	r3, #32
	and	r2, r2, #63
	beq	.L21
	movw	r3, #299
	ldr	r2, .L32
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L32+4
	movs	r0, #0
.L31:
	bl	vlog
	mov	r0, #-1
.L20:
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, pc}
.L21:
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	cbnz	r0, .L23
	movw	r3, #305
	ldr	r2, .L32+8
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L32+4
	b	.L31
.L23:
	ldr	r5, [r0, #248]
	cbz	r5, .L24
	ldrd	r0, r2, [r5, #8]
	subs	r3, r0, r2
	cmp	r3, #0
	it	lt
	rsblt	r3, r3, #0
	cmp	r3, #4
	ble	.L25
	mov	r1, r0
	ldr	r0, .L32+12
	bl	printk
.L24:
	ldr	r3, [r5, #12]
	movs	r1, #5
	ldr	r2, .L32+16
	movs	r0, #0
	str	r3, [sp, #36]
	ldr	r3, [r5, #8]
	str	r3, [sp, #32]
	ldr	r3, [r4, #24]
	str	r3, [sp, #28]
	ldr	r3, [r4, #20]
	str	r3, [sp, #24]
	ldr	r3, [r4, #16]
	str	r3, [sp, #20]
	ldr	r3, [r4, #12]
	str	r3, [sp, #16]
	ldr	r3, [r4, #8]
	str	r3, [sp, #12]
	ldr	r3, [r4, #4]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	mov	r3, #324
	str	r3, [sp]
	ldr	r3, .L32+4
	bl	vlog
	movs	r0, #0
	b	.L20
.L25:
	movs	r1, #5
	bl	__aeabi_uidivmod
	add	r2, r4, #32
	mov	r3, r4
	add	r1, r5, r1, lsl #5
	adds	r1, r1, #16
.L26:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r2
	str	r0, [r1], #4	@ unaligned
	bne	.L26
	ldr	r3, [r5, #8]
	adds	r3, r3, #1
	str	r3, [r5, #8]
	b	.L24
.L33:
	.align	2
.L32:
	.word	.LC4
	.word	.LANCHOR1+17
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.fnend
	.size	vrga_node_set_crop, .-vrga_node_set_crop
	.align	1
	.global	vrga_mb_release
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_mb_release, %function
vrga_mb_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r1
	ldr	r3, [r1, #12]
	.pad #24
	sub	sp, sp, #24
	ldr	r2, [r1]
	cmp	r3, #104
	and	r2, r2, #63
	beq	.L35
	movs	r3, #206
	ldr	r2, .L40
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L40+4
	movs	r0, #0
	bl	vlog
.L38:
	mov	r0, #-1
	b	.L34
.L35:
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	mov	r6, r0
	cmp	r0, #0
	beq	.L38
	movs	r1, #4
	ldr	r0, [r5, #88]
	bl	mpi_obj_from_id
	mov	r4, r0
	cbz	r0, .L37
	ldrd	r2, [r5, #88]
	movs	r1, #5
	strd	r2, [sp, #16]
	ldr	r3, [r0, #64]
	ldr	r2, .L40+8
	strd	r0, r3, [sp, #4]
	movs	r3, #217
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L40+4
	bl	vlog
	mov	r1, r4
	mov	r0, r6
	bl	mpi_node_release_buffer
.L39:
	movs	r0, #0
.L34:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L37:
	movs	r3, #220
	ldr	r2, .L40+12
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L40+4
	bl	vlog
	b	.L39
.L41:
	.align	2
.L40:
	.word	.LC8
	.word	.LANCHOR1+36
	.word	.LC9
	.word	.LC10
	.fnend
	.size	vrga_mb_release, .-vrga_mb_release
	.align	1
	.global	vrga_mb_send
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_mb_send, %function
vrga_mb_send:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, fp, lr}
	.save {r4, r5, r6, fp, lr}
	.pad #12
	mov	r5, r1
	ldr	r3, [r0, #128]
	ldr	r2, [r1]
	ldr	r4, [r3, #160]
	and	r2, r2, #63
	ldr	r3, [r1, #12]
	adds	r4, r4, #8
	cmp	r3, #104
	beq	.L43
	movs	r3, #235
	ldr	r2, .L57
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L57+4
	movs	r0, #0
	bl	vlog
.L49:
	mov	r0, #-1
.L42:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, fp, pc}
.L43:
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	mov	r6, r0
	cmp	r0, #0
	beq	.L49
	cbnz	r4, .L46
.L56:
	movs	r0, #0
	b	.L42
.L46:
	ldrd	fp, [r5, #88]
	orrs	r3, fp, ip
	beq	.L47
	movs	r1, #4
	mov	r0, fp
	bl	mpi_obj_from_id
	mov	r1, r0
	cmp	r0, #0
	beq	.L56
	movs	r2, #1
	mov	r0, r6
	bl	mpi_node_buffer_in
	b	.L56
.L47:
	ldr	r0, [r5, #16]
	cmp	r0, #0
	blt	.L56
	bl	get_mpi_buf
	mov	r4, r0
	cmp	r0, #0
	beq	.L49
	ldr	r3, .L57+8
	add	r1, r5, #24
	movs	r2, #80
	adds	r0, r0, #232
	str	r3, [r0, #-228]
	ldr	r3, [r5, #28]
	str	r3, [r0, #4]
	ldr	r3, [r5, #32]
	str	r3, [r0, #8]
	ldr	r3, [r5, #36]
	str	r3, [r0, #12]
	ldr	r3, [r5, #40]
	str	r3, [r0, #16]
	bl	memcpy
	mov	r0, r6
	movs	r2, #1
	mov	r1, r4
	bl	mpi_node_buffer_in
	mov	r0, r4
	bl	mpi_obj_unref
	b	.L56
.L58:
	.align	2
.L57:
	.word	.LC8
	.word	.LANCHOR1+52
	.word	.LC11
	.fnend
	.size	vrga_mb_send, .-vrga_mb_send
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_destroy_ctx, %function
vrga_destroy_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	kfree
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	vrga_destroy_ctx, .-vrga_destroy_ctx
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_ioctl, %function
vrga_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r3, r2
	ldr	r2, [r0, #128]
	uxtb	r4, r1
	ldr	r6, [r2, #160]
	ldr	r5, [r6, #184]
	cmp	r5, r4
	bls	.L61
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
	cmp	r4, r5
	sbc	r5, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r6, #180]
	ands	r4, r4, r5
	movs	r5, #20
	mla	r4, r5, r4, r2
	cbz	r4, .L61
	mov	r2, r1
	mov	r1, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	mpi_ioctl_kernel
.L61:
	movw	r3, #389
	ldr	r2, .L65
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L65+4
	movs	r0, #0
	bl	vlog
	mvn	r0, #21
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L66:
	.align	2
.L65:
	.word	.LC12
	.word	.LANCHOR1+65
	.fnend
	.size	vrga_ioctl, .-vrga_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_handle_message, %function
vrga_handle_message:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r7, r0
	ldr	r3, [r0, #8]
	.pad #36
	sub	sp, sp, #36
	mov	r5, r2
	cmp	r3, #1
	bne	.L84
	subs	r3, r1, #2
	cmp	r3, #1
	bhi	.L69
	ldr	r4, [r0, #248]
	cmp	r4, #0
	beq	.L67
	ldr	r4, [r4]
	cmp	r4, #0
	beq	.L67
	mov	r0, r4
	bl	mpi_obj_ref
	ldr	r3, [r4, #48]
	cmp	r7, r3
	bne	.L70
	mov	r0, r4
	bl	mpi_job_wait
.L70:
	mov	r0, r4
	bl	mpi_obj_unref
.L84:
	movs	r4, #0
	b	.L67
.L69:
	ldr	r3, .L96
	ldr	r4, [r3, #172]
	cmp	r4, #0
	bne	.L84
	cmp	r1, #1
	bne	.L67
	ldr	r3, [r2]
	movs	r1, #5
	ldr	r2, .L96+4
	str	r3, [sp, #12]
	ldr	r3, [r0]
	str	r3, [sp, #8]
	ldr	r3, [r0, #44]
	mov	r0, r4
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	movw	r3, #727
	str	r3, [sp]
	ldr	r3, .L96+8
	bl	vlog
	ldr	r9, [r7, #248]
	ldr	r10, [r7, #44]
	cmp	r9, #0
	beq	.L86
	cmp	r5, #0
	beq	.L86
	mov	r0, r5
	bl	mpi_obj_ref
	ldr	r3, [r9, #4]
	cbnz	r3, .L87
	mov	r1, r4
	mov	r0, r7
	bl	mpi_node_request_buf
	mov	r6, r0
	cbnz	r0, .L71
.L72:
	mov	r0, r5
	b	.L95
.L87:
	mov	r6, r4
.L71:
	mov	r1, r7
	mov	r0, r10
	bl	mpi_dev_request_job_for_node
	mov	r8, r0
	cbz	r0, .L73
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L74
	bl	mpi_obj_unref
.L73:
	mov	r0, r6
	bl	mpi_obj_unref
	b	.L72
.L74:
	str	r5, [r0, #60]
	ldr	r3, [r9, #4]
	cmp	r3, #0
	beq	.L75
	str	r5, [r0, #64]
	mov	r0, r5
	bl	mpi_obj_ref
.L76:
	ldr	r3, [r8, #64]
	movs	r1, #5
	ldr	r2, .L96+12
	movs	r0, #0
	ldr	r3, [r3]
	str	r3, [sp, #20]
	ldr	r3, [r8, #60]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	ldr	r3, [r7]
	str	r3, [sp, #12]
	ldr	r3, [r10, #44]
	str	r3, [sp, #8]
	ldr	r3, [r8]
	str	r3, [sp, #4]
	movw	r3, #661
	str	r3, [sp]
	ldr	r3, .L96+16
	bl	vlog
	mov	r0, r8
	bl	mpi_job_set_ready
	mov	r0, r8
	ldr	r10, .L96+16
	bl	mpi_obj_ref
	ldr	fp, .L96+24
	movw	r9, #681
.L77:
	mov	r1, r8
	ldr	r0, [r7, #268]
	bl	mpi_queue_push_obj
	mov	r6, r0
	cbz	r0, .L94
	ldr	r0, .L96+20
	bl	up
.L67:
	mov	r0, r4
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L75:
	str	r6, [r0, #64]
	b	.L76
.L94:
	ldr	r0, [r7, #268]
	bl	mpi_queue_pop_obj
	mov	r5, r0
	cbnz	r0, .L78
	movs	r1, #1
	mov	r0, r8
	bl	mpi_job_cancel
	mov	r0, r8
.L95:
	bl	mpi_obj_unref
.L86:
	mov	r4, #-1
	b	.L67
.L78:
	ldr	r3, [r0, #60]
	ldr	r2, [r0, #16]
	ldr	r1, [r3, #16]
	str	r1, [sp, #24]
	movs	r1, #5
	ldr	r3, [r3]
	strd	r2, r3, [sp, #16]
	mov	r2, fp
	ldr	r3, [r0]
	mov	r0, r6
	str	r3, [sp, #12]
	ldr	r3, [r7]
	str	r3, [sp, #8]
	ldr	r3, [r7, #44]
	ldr	r3, [r3, #44]
	strd	r9, r3, [sp]
	mov	r3, r10
	bl	vlog
	movs	r1, #1
	mov	r0, r5
	bl	mpi_job_cancel
	mov	r0, r5
	bl	mpi_obj_unref
	b	.L77
.L97:
	.align	2
.L96:
	.word	.LANCHOR0
	.word	.LC13
	.word	.LANCHOR1+76
	.word	.LC14
	.word	.LANCHOR1+101
	.word	.LANCHOR0+188
	.word	.LC15
	.fnend
	.size	vrga_handle_message, .-vrga_handle_message
	.align	1
	.global	vrga_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_version, %function
vrga_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r5, [r3, #160]
	ldr	r3, [r1, #32]
	cmp	r3, #76
	beq	.L99
	movs	r3, #58
	ldr	r2, .L103
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L103+4
	movs	r0, #0
	bl	vlog
	mov	r0, #-1
.L98:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L99:
	movs	r3, #0
	movs	r2, #1
	str	r3, [r1, #8]
	strd	r2, r3, [r1]
	ldr	r0, [r5]
	bl	strlen
	cmp	r0, #16
	it	ls
	strls	r0, [r4, #12]
	add	r0, r4, #16
	itt	hi
	movhi	r3, #16
	strhi	r3, [r4, #12]
	ldr	r2, [r4, #12]
	ldr	r1, [r5]
	bl	strncpy
	movs	r0, #0
	str	r0, [r4, #36]
	str	r0, [r4, #56]
	b	.L98
.L104:
	.align	2
.L103:
	.word	.LC16
	.word	.LANCHOR1+114
	.fnend
	.size	vrga_version, .-vrga_version
	.align	1
	.global	vrga_node_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_stop, %function
vrga_node_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	ldr	r3, [r1, #12]
	ldr	r2, [r1]
	cmp	r3, #32
	and	r2, r2, #63
	beq	.L106
	movs	r3, #164
	ldr	r2, .L108
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L108+4
	movs	r0, #0
	bl	vlog
	mov	r0, #-1
.L105:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L106:
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	clz	r0, r0
	lsrs	r0, r0, #5
	rsbs	r0, r0, #0
	b	.L105
.L109:
	.align	2
.L108:
	.word	.LC0
	.word	.LANCHOR1+127
	.fnend
	.size	vrga_node_stop, .-vrga_node_stop
	.align	1
	.global	vrga_node_set_bypass
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_set_bypass, %function
vrga_node_set_bypass:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r5, r1
	ldr	r3, [r1, #12]
	ldr	r4, [r1]
	cmp	r3, #32
	and	r4, r4, #63
	beq	.L111
	mov	r3, #276
	ldr	r2, .L115
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L115+4
	movs	r0, #0
	bl	vlog
.L113:
	mov	r0, #-1
	b	.L110
.L111:
	mov	r2, r4
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L113
	ldr	r3, .L115+8
	ldr	r0, [r0, #248]
	ldr	r2, [r5, #20]
	add	r4, r3, r4, lsl #2
	str	r2, [r4, #204]
	cbz	r0, .L110
	str	r2, [r0, #4]
	movs	r0, #0
.L110:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L116:
	.align	2
.L115:
	.word	.LC0
	.word	.LANCHOR1+142
	.word	.LANCHOR0
	.fnend
	.size	vrga_node_set_bypass, .-vrga_node_set_bypass
	.align	1
	.global	vrga_node_frame_rate_ctl
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_frame_rate_ctl, %function
vrga_node_frame_rate_ctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r1, #12]
	ldr	r2, [r1]
	cmp	r3, #32
	and	r2, r2, #63
	beq	.L118
	mov	r3, #336
	ldr	r2, .L121
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L121+4
	movs	r0, #0
	bl	vlog
.L120:
	mov	r0, #-1
	b	.L117
.L118:
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L120
	ldrd	r1, r2, [r4, #24]
	bl	mpi_node_set_in_frame_rate
	movs	r0, #0
.L117:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L122:
	.align	2
.L121:
	.word	.LC0
	.word	.LANCHOR1+163
	.fnend
	.size	vrga_node_frame_rate_ctl, .-vrga_node_frame_rate_ctl
	.align	1
	.global	vrga_node_delete
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_delete, %function
vrga_node_delete:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	ldr	r3, [r1, #12]
	ldr	r5, [r1]
	cmp	r3, #32
	and	r5, r5, #63
	beq	.L124
	movs	r3, #123
	ldr	r2, .L128
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L128+4
	movs	r0, #0
	bl	vlog
.L126:
	mov	r0, #-1
	b	.L123
.L124:
	mov	r2, r5
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	mov	r6, r0
	cmp	r0, #0
	beq	.L126
	ldr	r4, [r0, #60]
	mov	r2, r5
	movs	r1, #0
	movs	r0, #18
	bl	vsys_remove_node_from_map
	mov	r0, r6
	bl	mpi_node_destroy
	cbz	r4, .L127
	movs	r1, #1
	mov	r0, r4
	bl	mpi_buf_pool_destroy
	movs	r0, #0
.L123:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L127:
	mov	r0, r4
	b	.L123
.L129:
	.align	2
.L128:
	.word	.LC0
	.word	.LANCHOR1+188
	.fnend
	.size	vrga_node_delete, .-vrga_node_delete
	.align	1
	.global	vrga_node_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_start, %function
vrga_node_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	ldr	r3, [r1, #12]
	ldr	r2, [r1]
	cmp	r3, #32
	and	r2, r2, #63
	beq	.L131
	movs	r3, #148
	ldr	r2, .L134
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L134+4
	movs	r0, #0
	bl	vlog
.L133:
	mov	r0, #-1
	b	.L130
.L131:
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L133
	bl	mpi_node_streamon
	movs	r0, #0
.L130:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L135:
	.align	2
.L134:
	.word	.LC0
	.word	.LANCHOR1+205
	.fnend
	.size	vrga_node_start, .-vrga_node_start
	.align	1
	.global	vrga_mb_get
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_mb_get, %function
vrga_mb_get:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r1, #12]
	ldr	r2, [r1]
	cmp	r3, #104
	and	r2, r2, #63
	beq	.L137
	movs	r3, #181
	ldr	r2, .L144
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L144+4
	movs	r0, #0
	bl	vlog
.L140:
	mov	r0, #-1
.L136:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L137:
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L140
	ldr	r2, [r4, #8]
	str	r2, [r0, #344]
	bl	mpi_node_pick_out_buffer
	mov	r5, r0
	cmp	r0, #0
	beq	.L140
	movs	r2, #80
	add	r1, r0, #232
	add	r0, r4, #24
	bl	memcpy
	ldr	r3, [r5, #64]
	movs	r0, #0
	ldr	r3, [r3]
	str	r3, [r4, #4]
	ldr	r2, [r5]
	asrs	r3, r2, #31
	strd	r2, [r4, #88]
	b	.L136
.L145:
	.align	2
.L144:
	.word	.LC8
	.word	.LANCHOR1+221
	.fnend
	.size	vrga_mb_get, .-vrga_mb_get
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_create_ctx, %function
vrga_create_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L151
	mov	r1, #3520
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r0, [r3, #8]
	bl	kmem_cache_alloc
	cbz	r0, .L146
	ldr	r2, [r4, #56]
	ldr	r3, .L151+4
	and	r2, r2, #63
	add	r3, r3, r2, lsl #2
	ldr	r3, [r3, #204]
	str	r3, [r0, #4]
	movs	r3, #1
	str	r3, [r0, #8]
.L146:
	pop	{r4, pc}
.L152:
	.align	2
.L151:
	.word	kmalloc_caches
	.word	.LANCHOR0
	.fnend
	.size	vrga_create_ctx, .-vrga_create_ctx
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_thread_fn, %function
vrga_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 344
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #396
	sub	sp, sp, #396
	ldr	r5, .L221
	movs	r4, #0
	add	r9, sp, #132
	mov	r7, r0
	movs	r2, #252
	mov	r1, r4
	ldr	r3, [r5]
	mov	r10, r5
	str	r4, [r9]
	str	r3, [sp, #388]
	add	r3, r0, #8
	add	r0, sp, #136
	str	r3, [sp, #52]
	bl	memset
	ldr	r3, .L221+4
	mov	r1, #3520
	ldr	r0, [r3, #20]
	bl	kmem_cache_alloc
	ldr	r3, [r7, #8]
	ldr	r2, .L221+8
	movs	r1, #5
	str	r3, [sp, #8]
	ldr	r3, [r7, #52]
	str	r0, [sp, #124]
	mov	r0, r4
	str	r3, [sp, #4]
	movw	r3, #546
	str	r3, [sp]
	ldr	r3, .L221+12
	bl	vlog
	add	r3, r7, #188
	str	r3, [sp, #56]
.L155:
	ldr	r5, [r7, #172]
	cbz	r5, .L170
	ldr	r0, [sp, #124]
	cbz	r0, .L171
	bl	kfree
.L171:
	ldr	r2, [sp, #388]
	movs	r0, #0
	ldr	r3, [r10]
	cmp	r2, r3
	beq	.L172
	bl	__stack_chk_fail
.L170:
	ldr	r0, [sp, #56]
	bl	down
	mov	r1, r5
	ldr	r0, [sp, #52]
	bl	mpi_dev_run_job
	mov	r4, r0
	cmp	r0, #0
	beq	.L155
	ldr	fp, [r0, #48]
	cmp	fp, #0
	beq	.L156
	ldr	r6, [fp, #248]
	cmp	r6, #0
	beq	.L156
	str	r0, [r6]
	mov	r1, #256
	ldr	r3, [r0, #48]
	mov	r0, r9
	ldr	r2, .L221+16
	ldr	r3, [r3]
	str	r3, [sp]
	ldr	r3, [r7, #52]
	bl	snprintf
	ldr	r1, [fp, #56]
	ldrd	r3, r2, [r4, #60]
	mov	r0, r5
	strd	r6, r1, [sp, #32]
	ldr	r1, [r6, #4]
	str	r1, [sp, #28]
	ldr	r1, [r2, #64]
	str	r1, [sp, #24]
	movs	r1, #5
	ldr	r2, [r2]
	str	r2, [sp, #20]
	ldr	r2, [r3, #64]
	str	r2, [sp, #16]
	ldr	r3, [r3]
	ldr	r2, .L221+20
	strd	r9, r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #567
	str	r3, [sp]
	ldr	r3, .L221+12
	bl	vlog
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bne	.L157
	ldrd	r2, r3, [r6, #8]
	cmp	r2, r3
	bne	.L158
.L161:
	ldr	r2, [r4, #48]
	ldr	r3, [sp, #112]
	cbnz	r2, .L217
.L159:
	cbz	r3, .L156
	mov	r0, r3
.L219:
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #112]
	b	.L156
.L158:
	ldr	r3, .L221+4
	mov	r1, #3520
	ldr	r0, [r3, #20]
	bl	kmem_cache_alloc
	str	r0, [sp, #112]
	cmp	r0, #0
	bne	.L161
.L156:
	movs	r3, #0
	mov	r1, #-1
	str	r3, [r6]
	b	.L220
.L222:
	.align	2
.L221:
	.word	__stack_chk_guard
	.word	kmalloc_caches
	.word	.LC17
	.word	.LANCHOR1+233
	.word	.LC18
	.word	.LC19
.L217:
	ldr	r8, [r2, #248]
	cmp	r8, #0
	beq	.L159
	ldr	r5, [r4, #60]
	cmp	r5, #0
	beq	.L159
	ldr	r1, [r4, #64]
	cmp	r1, #0
	beq	.L159
	ldr	r0, [r5, #64]
	ldr	r1, [r1, #64]
	ldr	r2, [r2, #56]
	str	r0, [sp, #100]
	movs	r0, #0
	str	r1, [sp, #108]
	str	r0, [sp, #104]
	str	r2, [sp, #128]
	str	r3, [sp, #60]
	cmp	r3, #0
	beq	.L162
	movs	r1, #5
	ldr	r0, [r8, #12]
	bl	__aeabi_uidivmod
	add	r1, r8, r1, lsl #5
	ldr	r3, [sp, #60]
	ldr	r2, [r1, #20]
	cmp	r2, #0
	beq	.L163
	ldrd	r0, r2, [r1, #24]
	cmp	r0, #0
	bne	.L164
	ldr	r3, [r1, #32]
	add	r0, sp, #68
	str	r2, [sp, #84]
	str	r3, [sp, #88]
	ldr	r3, [r1, #36]
	str	r3, [sp, #92]
	ldr	r3, [r1, #40]
	add	r1, sp, #84
	str	r3, [sp, #96]
	ldrd	r2, r3, [r5, #236]
	bl	crop_rect_ratio_to_abs
	ldr	r3, [sp, #112]
	ldr	ip, [sp, #68]
	ldrd	r0, r1, [sp, #72]
	ldr	r2, [sp, #80]
	str	ip, [r3]
	ldr	r3, [sp, #112]
	str	r0, [r3, #4]
	ldr	r3, [sp, #112]
	str	r1, [r3, #8]
	ldr	r3, [sp, #112]
.L218:
	str	r2, [r3, #12]
	ldr	r3, [sp, #112]
	ldr	r2, [r5, #244]
	str	r2, [r3, #20]
	ldr	r2, [r5, #248]
	str	r2, [r3, #24]
	ldr	r0, [r5, #256]
	bl	pixel_format_rk_to_rga
	ldr	r3, [sp, #112]
	str	r0, [r3, #16]
	ldr	r0, [r5, #264]
	bl	compress_mode_rk_to_rga
	movs	r1, #5
	ldr	r3, [sp, #112]
	str	r0, [r3, #28]
	movs	r0, #0
	ldr	r3, [r8, #12]
	adds	r3, r3, #1
	str	r3, [r8, #12]
	ldr	r3, [sp, #112]
	ldr	r2, [r3, #28]
	str	r2, [sp, #44]
	ldr	r2, [r3, #16]
	str	r2, [sp, #40]
	ldr	r2, [r3, #24]
	str	r2, [sp, #36]
	ldr	r2, [r3, #20]
	str	r2, [sp, #32]
	ldr	r2, [r3, #12]
	str	r2, [sp, #28]
	ldr	r2, [r3, #8]
	str	r2, [sp, #24]
	ldr	r2, [r3, #4]
	str	r2, [sp, #20]
	ldr	r3, [r3]
	ldr	r2, .L223
	str	r3, [sp, #16]
	ldr	r3, [sp, #128]
	str	r3, [sp, #12]
	ldr	r3, [r5, #64]
	str	r3, [sp, #8]
	ldr	r3, [r5]
	str	r3, [sp, #4]
	mov	r3, #484
	str	r3, [sp]
	ldr	r3, .L223+4
	bl	vlog
.L162:
	add	r0, sp, #100
	bl	rga_mpi_commit
	mov	r8, r0
	cmp	r0, #0
	bne	.L166
	ldrd	r3, r2, [r4, #60]
	ldr	r1, [fp, #56]
	str	r1, [sp, #28]
	ldr	r1, [r2, #64]
	str	r1, [sp, #24]
	movs	r1, #5
	ldr	r2, [r2]
	str	r2, [sp, #20]
	ldr	r2, [r3, #64]
	str	r2, [sp, #16]
	ldr	r3, [r3]
	ldr	r2, .L223+8
	str	r3, [sp, #12]
	add	r3, sp, #132
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #591
	str	r3, [sp]
	ldr	r3, .L223+12
	bl	vlog
	ldr	r3, [r4, #48]
	cmp	r3, #0
	beq	.L175
	ldr	fp, [r4, #60]
	cmp	fp, #0
	beq	.L175
	ldr	r5, [r4, #64]
	cmp	r5, #0
	beq	.L175
	ldr	r3, [sp, #124]
	ldr	r2, [r3, #8]
	str	r2, [r5, #236]
	ldr	r2, [r3, #12]
	str	r2, [r5, #240]
	ldr	r2, [r3, #20]
	str	r2, [r5, #244]
	ldr	r2, [r3, #24]
	str	r2, [r5, #248]
	ldr	r0, [r3, #16]
	bl	pixel_format_rga_to_rk
	ldr	r3, [sp, #124]
	str	r0, [r5, #256]
	ldr	r0, [r3, #28]
	bl	compress_mode_rga_to_rk
	str	r0, [r5, #264]
	ldr	r2, [fp, #284]
	ldr	r3, [sp, #124]
	str	r2, [r5, #284]
	ldrd	r0, [fp, #288]
	strd	r0, [r5, #288]
	strd	r0, [sp, #40]
	movs	r1, #5
	str	r2, [sp, #36]
	mov	r0, r8
	ldr	r2, [r3, #28]
	str	r2, [sp, #32]
	ldr	r2, [r3, #16]
	str	r2, [sp, #28]
	ldr	r2, [r3, #24]
	str	r2, [sp, #24]
	ldr	r2, [r3, #20]
	str	r2, [sp, #20]
	ldr	r2, [r3, #12]
	str	r2, [sp, #16]
	ldr	r3, [r3, #8]
	ldr	r2, .L223+16
	str	r3, [sp, #12]
	ldr	r3, [r5, #64]
	str	r3, [sp, #8]
	ldr	r3, [r5]
	str	r3, [sp, #4]
	mov	r3, #524
	str	r3, [sp]
	ldr	r3, .L223+20
	bl	vlog
	ldr	r3, [fp, #16]
	ldr	r2, [r5, #16]
	movs	r1, #5
	mov	r0, r8
	strd	r3, r2, [sp, #4]
	movw	r3, #526
	ldr	r2, .L223+24
	str	r3, [sp]
	ldr	r3, .L223+20
	bl	vlog
.L167:
	ldr	r0, [sp, #112]
	cbz	r0, .L168
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #112]
.L168:
	cmp	r8, #-1
	beq	.L156
.L157:
	movs	r1, #0
	str	r1, [r6]
	ldr	r3, [r7, #176]
	adds	r3, r3, #1
	str	r3, [r7, #176]
.L220:
	mov	r0, r4
	bl	mpi_job_done
	b	.L155
.L164:
	str	r2, [r3]
	ldr	r3, [sp, #112]
	ldr	r2, [r1, #32]
	str	r2, [r3, #4]
	ldr	r3, [sp, #112]
	ldr	r2, [r1, #36]
	str	r2, [r3, #8]
	ldr	r3, [sp, #112]
	ldr	r2, [r1, #40]
	b	.L218
.L163:
	str	r2, [r3]
	ldr	r3, [sp, #112]
	str	r2, [r3, #4]
	ldr	r3, [sp, #112]
	ldr	r2, [r5, #236]
	str	r2, [r3, #8]
	ldr	r3, [sp, #112]
	ldr	r2, [r5, #240]
	b	.L218
.L175:
	mov	r8, #-1
	b	.L167
.L166:
	ldr	r0, [sp, #112]
	cmp	r0, #0
	bne	.L219
	b	.L156
.L172:
	add	sp, sp, #396
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L224:
	.align	2
.L223:
	.word	.LC20
	.word	.LANCHOR1+248
	.word	.LC21
	.word	.LANCHOR1+233
	.word	.LC22
	.word	.LANCHOR1+272
	.word	.LC23
	.fnend
	.size	vrga_thread_fn, .-vrga_thread_fn
	.align	1
	.global	vrga_user_request
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_user_request, %function
vrga_user_request:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r5, r0
	ldr	r0, .L231
	mov	r6, r1
	bl	mpi_dev_request_job
	mov	r4, r0
	cbz	r0, .L225
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L227
	bl	mpi_obj_unref
	movs	r4, #0
.L225:
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L227:
	strd	r5, r6, [r0, #60]
	movs	r1, #5
	ldr	r3, [r6]
	ldr	r2, .L231+4
	str	r3, [sp, #12]
	ldr	r3, [r5]
	str	r3, [sp, #8]
	ldr	r3, [r0]
	movs	r0, #0
	str	r3, [sp, #4]
	mov	r3, #792
	str	r3, [sp]
	ldr	r3, .L231+8
	bl	vlog
	mov	r0, r4
	bl	mpi_job_set_ready
	mov	r0, r4
	bl	mpi_obj_ref
	b	.L225
.L232:
	.align	2
.L231:
	.word	.LANCHOR0+8
	.word	.LC24
	.word	.LANCHOR1+296
	.fnend
	.size	vrga_user_request, .-vrga_user_request
	.align	1
	.global	vrga_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_probe, %function
vrga_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r2, #460
	ldr	r4, .L235
	mov	r6, r0
	movs	r1, #0
	ldr	r5, .L235+4
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r6
	bl	kstrdup
	mov	r2, r6
	movs	r1, #8
	movs	r3, #1
	str	r5, [r4, #180]
	str	r3, [r4, #4]
	movs	r3, #18
	str	r4, [r4, #168]
	str	r3, [r4, #184]
	str	r0, [r4]
	adds	r0, r4, r1
	bl	mpi_dev_register
	ldr	r3, .L235+8
	add	r1, r5, #360
	add	r0, r4, #8
	str	r3, [r4, #76]
	ldr	r3, .L235+12
	str	r3, [r4, #84]
	ldr	r3, .L235+16
	str	r3, [r4, #88]
	bl	mpi_sysfs_dev_add
	movs	r3, #0
	mov	r2, #-1
	str	r3, [r4, #188]
	mov	r1, r4
	add	r3, r4, #192
	ldr	r0, .L235+20
	strd	r3, r3, [r4, #192]
	ldr	r3, .L235+24
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L234
	bl	wake_up_process
.L234:
	str	r5, [r4, #200]
	pop	{r4, r5, r6, pc}
.L236:
	.align	2
.L235:
	.word	.LANCHOR0
	.word	.LANCHOR1+316
	.word	vrga_create_ctx
	.word	vrga_destroy_ctx
	.word	vrga_handle_message
	.word	vrga_thread_fn
	.word	.LC2
	.fnend
	.size	vrga_probe, .-vrga_probe
	.align	1
	.global	vrga_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_shutdown, %function
vrga_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r3, #1
	ldr	r4, .L238
	ldr	r0, [r4, #200]
	str	r3, [r4, #172]
	bl	kthread_stop
	add	r0, r4, #8
	bl	mpi_dev_unregister
	ldr	r0, [r4]
	pop	{r4, lr}
	b	kfree
.L239:
	.align	2
.L238:
	.word	.LANCHOR0
	.fnend
	.size	vrga_shutdown, .-vrga_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	vrga_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_init, %function
vrga_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L241
	bl	vrga_probe
	movs	r0, #0
	pop	{r3, pc}
.L242:
	.align	2
.L241:
	.word	.LC2
	.fnend
	.size	vrga_init, .-vrga_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	vrga_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_exit, %function
vrga_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	vrga_shutdown
	.fnend
	.size	vrga_exit, .-vrga_exit
	.global	g_vrga_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__func__.36842, %object
	.size	__func__.36842, 17
__func__.36842:
	.ascii	"vrga_node_create\000"
	.type	__func__.36919, %object
	.size	__func__.36919, 19
__func__.36919:
	.ascii	"vrga_node_set_crop\000"
	.type	__func__.36889, %object
	.size	__func__.36889, 16
__func__.36889:
	.ascii	"vrga_mb_release\000"
	.type	__func__.36901, %object
	.size	__func__.36901, 13
__func__.36901:
	.ascii	"vrga_mb_send\000"
	.type	__func__.36963, %object
	.size	__func__.36963, 11
__func__.36963:
	.ascii	"vrga_ioctl\000"
	.type	__func__.37073, %object
	.size	__func__.37073, 25
__func__.37073:
	.ascii	"vrga_handle_node_message\000"
	.type	__func__.37037, %object
	.size	__func__.37037, 13
__func__.37037:
	.ascii	"vrga_new_job\000"
	.type	__func__.36830, %object
	.size	__func__.36830, 13
__func__.36830:
	.ascii	"vrga_version\000"
	.type	__func__.36871, %object
	.size	__func__.36871, 15
__func__.36871:
	.ascii	"vrga_node_stop\000"
	.type	__func__.36910, %object
	.size	__func__.36910, 21
__func__.36910:
	.ascii	"vrga_node_set_bypass\000"
	.type	__func__.36939, %object
	.size	__func__.36939, 25
__func__.36939:
	.ascii	"vrga_node_frame_rate_ctl\000"
	.type	__func__.36851, %object
	.size	__func__.36851, 17
__func__.36851:
	.ascii	"vrga_node_delete\000"
	.type	__func__.36863, %object
	.size	__func__.36863, 16
__func__.36863:
	.ascii	"vrga_node_start\000"
	.type	__func__.36880, %object
	.size	__func__.36880, 12
__func__.36880:
	.ascii	"vrga_mb_get\000"
	.type	__func__.37017, %object
	.size	__func__.37017, 15
__func__.37017:
	.ascii	"vrga_thread_fn\000"
	.type	__func__.36989, %object
	.size	__func__.36989, 24
__func__.36989:
	.ascii	"vrga_set_src_frame_info\000"
	.type	__func__.36997, %object
	.size	__func__.36997, 24
__func__.36997:
	.ascii	"vrga_set_dst_frame_info\000"
	.type	__func__.37103, %object
	.size	__func__.37103, 18
__func__.37103:
	.ascii	"vrga_user_request\000"
	.space	2
	.type	vrga_ioctls, %object
	.size	vrga_ioctls, 360
vrga_ioctls:
	.word	-1068731904
	.word	0
	.word	vrga_version
	.word	.LC25
	.word	0
	.word	-1071615487
	.word	0
	.word	vrga_node_create
	.word	.LC26
	.word	0
	.word	-1071615486
	.word	0
	.word	vrga_node_delete
	.word	.LC27
	.word	0
	.word	-1071615485
	.word	0
	.word	vrga_node_get_status
	.word	.LC28
	.word	0
	.word	-1071615484
	.word	0
	.word	vrga_node_start
	.word	.LC29
	.word	0
	.word	-1071615483
	.word	0
	.word	vrga_node_stop
	.word	.LC30
	.word	0
	.word	-1066896890
	.word	16
	.word	vrga_mb_get
	.word	.LC31
	.word	0
	.word	-1066896889
	.word	0
	.word	vrga_mb_send
	.word	.LC32
	.word	0
	.word	-1066896888
	.word	0
	.word	vrga_mb_release
	.word	.LC33
	.word	0
	.word	-1071615479
	.word	0
	.word	vrga_node_set_bypass
	.word	.LC34
	.word	0
	.space	120
	.word	-1071615472
	.word	0
	.word	vrga_node_set_crop
	.word	.LC35
	.word	0
	.word	-1071615471
	.word	0
	.word	vrga_node_frame_rate_ctl
	.word	.LC36
	.word	0
	.type	vrga_driver_fops, %object
	.size	vrga_driver_fops, 128
vrga_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	0
	.word	vrga_ioctl
	.word	vrga_compat_ioctl
	.word	0
	.space	4
	.word	vrga_open
	.space	4
	.word	0
	.space	60
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_vrga_dev, %object
	.size	g_vrga_dev, 460
g_vrga_dev:
	.space	460
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[31m\033[1m%s,%d:struct vrga_node_info mismatch"
	.ascii	"ing\033[0m\000"
.LC1:
	.ascii	"valloc\000"
.LC2:
	.ascii	"vrga\000"
.LC3:
	.ascii	"create vpss node = %d\000"
.LC4:
	.ascii	"\033[31m\033[1m%s,%d:struct vrga_crop_info mismatch"
	.ascii	"ing\033[0m\000"
.LC5:
	.ascii	"\033[31m\033[1m%s,%d:not found vrga_node\033[0m\000"
.LC6:
	.ascii	"\0013exceed max rga buf cnt(0x%x-0x%x)\000"
.LC7:
	.ascii	"%s,%d:ctx_id(%d), enable(%d), ration(0)/abs(1)(%d),"
	.ascii	" x(%d), y(%d), w(%d), h(%d), s/g(0x%x-0x%x)\000"
.LC8:
	.ascii	"\033[31m\033[1m%s,%d:struct vrga_buff_info mismatch"
	.ascii	"ing\033[0m\000"
.LC9:
	.ascii	"%s,%d:put buf(%p), dma_buf(%p), buff id(%lld)\000"
.LC10:
	.ascii	"\033[31m\033[1m%s,%d:buf null\033[0m\000"
.LC11:
	.ascii	"vrga-fr-user\000"
.LC12:
	.ascii	"\033[31m\033[1m%s,%d:vrga: error vrga ioctl\033[0m\000"
.LC13:
	.ascii	"%s,%d:vrga(%s-%d) buf(%d) ready\000"
.LC14:
	.ascii	"%s,%d:vrga job(%d) ready for (%s-%d) buf(%d->%d)\000"
.LC15:
	.ascii	"%s,%d:vrga(%s-%d) drop job(%d)->refcnt(%d), src_buf"
	.ascii	"(%d)->refcnt(%d)\000"
.LC16:
	.ascii	"\033[31m\033[1m%s,%d:struct vrga_version mismatchin"
	.ascii	"g\033[0m\000"
.LC17:
	.ascii	"%s,%d:dev(%s-%d) register ok\000"
.LC18:
	.ascii	"%s-%d\000"
.LC19:
	.ascii	"%s,%d:vrga job(%d) run for (%s) buf(%d: %p->%d: %p)"
	.ascii	", vctx(%d: %p), nick_id(%d)\000"
.LC20:
	.ascii	"%s,%d:src buf(%d: %p), rga: ctx_id(%d), x(%d), y(%d"
	.ascii	"), w(%d), h(%d), vw(%d), vh(%d), pf(0x%x), cp(%d)\000"
.LC21:
	.ascii	"%s,%d:vrga job(%d) done for (%s) buf(%d: %p->%d: %p"
	.ascii	"), nick_id(%d)\000"
.LC22:
	.ascii	"%s,%d:dst buf(%d: %p), rga: w(%d), h(%d), vw(%d), v"
	.ascii	"h(%d), pf(0x%x), cp(%d), seq(%d), pts(%lld)\000"
.LC23:
	.ascii	"%s,%d:src_ref(%d), dst_ref(%d)\000"
.LC24:
	.ascii	"%s,%d:vrga job(%d) ready for user, buf(%d->%d)\000"
.LC25:
	.ascii	"VRGA_IOCTL_VERSION\000"
.LC26:
	.ascii	"VRGA_IOCTL_NODE_CREATE\000"
.LC27:
	.ascii	"VRGA_IOCTL_NODE_DELETE\000"
.LC28:
	.ascii	"VRGA_IOCTL_NODE_GET_STATUS\000"
.LC29:
	.ascii	"VRGA_IOCTL_NODE_START\000"
.LC30:
	.ascii	"VRGA_IOCTL_NODE_STOP\000"
.LC31:
	.ascii	"VRGA_IOCTL_MB_GET\000"
.LC32:
	.ascii	"VRGA_IOCTL_MB_SEND\000"
.LC33:
	.ascii	"VRGA_IOCTL_MB_RELEASE\000"
.LC34:
	.ascii	"VRGA_IOCTL_SET_BYPASS\000"
.LC35:
	.ascii	"VRGA_IOCTL_SET_CROP\000"
.LC36:
	.ascii	"VRGA_IOCTL_FRAME_RATE_CTL\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
