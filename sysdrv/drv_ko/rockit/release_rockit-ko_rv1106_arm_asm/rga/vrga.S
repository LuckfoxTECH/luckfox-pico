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
	ldr	r3, .L4
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	add	r0, r3, #12
	str	r0, [r1, #128]
	movs	r2, #21
	str	r2, [r3, #184]
	bl	mpi_obj_ref
	mov	r3, #608
	ldr	r2, .L4+4
	movs	r1, #4
	movs	r0, #0
	str	r3, [sp]
	ldr	r3, .L4+8
	bl	vlog
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L5:
	.align	2
.L4:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR1
	.fnend
	.size	vrga_open, .-vrga_open
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
	bls	.L7
	.syntax unified
@ 87 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
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
	cbz	r4, .L7
	mov	r2, r1
	mov	r1, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	mpi_ioctl_kernel
.L7:
	movw	r3, #589
	ldr	r2, .L11
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L11+4
	movs	r0, #0
	bl	vlog
	mvn	r0, #21
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L12:
	.align	2
.L11:
	.word	.LC1
	.word	.LANCHOR1+10
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
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r7, r2
	.pad #32
	sub	sp, sp, #32
	mov	r6, r0
	cbnz	r0, .L14
	ldr	r3, [r0, #8]
	cmp	r3, #1
	bne	.L32
.L14:
	subs	r3, r1, #2
	ldr	r5, [r6, #288]
	cmp	r3, #1
	bhi	.L16
	cbz	r5, .L32
	ldr	r4, [r5]
	cbz	r4, .L13
	mov	r0, r4
	bl	mpi_obj_ref
	ldr	r3, [r4, #48]
	cmp	r6, r3
	bne	.L17
	mov	r0, r4
	bl	mpi_job_wait
.L17:
	mov	r0, r4
	bl	mpi_obj_unref
.L32:
	movs	r4, #0
	b	.L13
.L16:
	ldr	r3, .L44
	ldr	r4, [r3, #8]
	cmp	r4, #0
	bne	.L32
	cmp	r1, #1
	bne	.L13
	ldr	r3, [r7]
	movs	r1, #5
	ldr	r2, .L44+4
	mov	r0, r4
	str	r3, [sp, #12]
	ldr	r3, [r6]
	str	r3, [sp, #8]
	ldr	r3, [r6, #44]
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	movw	r3, #1073
	str	r3, [sp]
	ldr	r3, .L44+8
	bl	vlog
	ldr	r5, [r5, #4]
	cbz	r5, .L18
	mov	r2, r4
	mov	r1, r7
	mov	r0, r6
	bl	mpi_node_buffer_out
.L13:
	mov	r0, r4
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L18:
	ldr	r8, [r6, #288]
	ldr	r9, [r6, #44]
	cmp	r8, #0
	beq	.L34
	cmp	r7, #0
	beq	.L34
	mov	r0, r7
	bl	mpi_obj_ref
	ldr	r1, [r8, #4]
	cbnz	r1, .L35
	mov	r0, r6
	bl	mpi_node_request_buf
	mov	r4, r0
	cbnz	r0, .L19
.L20:
	mov	r0, r7
	b	.L43
.L35:
	mov	r4, r5
.L19:
	mov	r1, r6
	mov	r0, r9
	bl	mpi_dev_request_job_for_node
	mov	r5, r0
	cbz	r0, .L21
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L22
	bl	mpi_obj_unref
.L21:
	mov	r0, r4
	bl	mpi_obj_unref
	b	.L20
.L22:
	str	r7, [r0, #60]
	ldr	r3, [r8, #4]
	cmp	r3, #0
	beq	.L23
	str	r7, [r0, #64]
	mov	r0, r7
	bl	mpi_obj_ref
.L24:
	ldr	r3, [r5, #64]
	movs	r1, #5
	ldr	r2, .L44+12
	movs	r0, #0
	ldr	r3, [r3]
	str	r3, [sp, #20]
	ldr	r3, [r5, #60]
	ldr	r3, [r3]
	str	r3, [sp, #16]
	ldr	r3, [r6]
	str	r3, [sp, #12]
	ldr	r3, [r9, #44]
	str	r3, [sp, #8]
	ldr	r3, [r5]
	str	r3, [sp, #4]
	movw	r3, #1007
	str	r3, [sp]
	ldr	r3, .L44+16
	bl	vlog
	mov	r0, r5
	bl	mpi_job_set_ready
	mov	r0, r5
	ldr	r9, .L44+16
	bl	mpi_obj_ref
	ldr	r10, .L44+24
	movw	r8, #1027
.L25:
	mov	r1, r5
	ldr	r0, [r6, #308]
	bl	mpi_queue_push_obj
	mov	r7, r0
	cbz	r0, .L42
	ldr	r0, .L44+20
	bl	up
	b	.L32
.L23:
	str	r4, [r0, #64]
	b	.L24
.L42:
	ldr	r0, [r6, #308]
	bl	mpi_queue_pop_obj
	mov	r4, r0
	cbnz	r0, .L26
	movs	r1, #1
	mov	r0, r5
	bl	mpi_job_cancel
	mov	r0, r5
.L43:
	bl	mpi_obj_unref
.L34:
	mov	r4, #-1
	b	.L13
.L26:
	ldr	r3, [r0, #60]
	ldr	r2, [r0, #16]
	ldr	r1, [r3, #16]
	str	r1, [sp, #24]
	movs	r1, #5
	ldr	r3, [r3]
	strd	r2, r3, [sp, #16]
	mov	r2, r10
	ldr	r3, [r0]
	mov	r0, r7
	str	r3, [sp, #12]
	ldr	r3, [r6]
	str	r3, [sp, #8]
	ldr	r3, [r6, #44]
	ldr	r3, [r3, #44]
	strd	r8, r3, [sp]
	mov	r3, r9
	bl	vlog
	movs	r1, #1
	mov	r0, r4
	bl	mpi_job_cancel
	mov	r0, r4
	bl	mpi_obj_unref
	b	.L25
.L45:
	.align	2
.L44:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LANCHOR1+21
	.word	.LC3
	.word	.LANCHOR1+46
	.word	.LANCHOR0+188
	.word	.LC4
	.fnend
	.size	vrga_handle_message, .-vrga_handle_message
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
	cbz	r0, .L47
	bl	kfree
.L47:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	vrga_destroy_ctx, .-vrga_destroy_ctx
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
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r6, [r3, #160]
	ldr	r3, [r1, #32]	@ unaligned
	cmp	r3, #76
	beq	.L52
	movs	r3, #104
	ldr	r2, .L56
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L56+4
	movs	r0, #0
	bl	vlog
	mov	r0, #-1
.L51:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L52:
	movs	r5, #0
	movs	r3, #1
	strb	r5, [r1, #1]
	strb	r3, [r1]
	strb	r5, [r1, #2]
	strb	r5, [r1, #3]
	strb	r5, [r1, #4]
	strb	r5, [r1, #5]
	strb	r5, [r1, #6]
	strb	r5, [r1, #7]
	strb	r5, [r1, #8]
	strb	r5, [r1, #9]
	strb	r5, [r1, #10]
	strb	r5, [r1, #11]
	ldr	r0, [r6]
	bl	strlen
	cmp	r0, #16
	ite	ls
	strls	r0, [r4, #12]	@ unaligned
	strbhi	r5, [r4, #13]
	add	r0, r4, #16
	itttt	hi
	strbhi	r5, [r4, #14]
	movhi	r3, #16
	strbhi	r5, [r4, #15]
	strbhi	r3, [r4, #12]
	ldr	r2, [r4, #12]	@ unaligned
	ldr	r1, [r6]
	bl	strncpy
	movs	r0, #0
	strb	r0, [r4, #36]
	strb	r0, [r4, #37]
	strb	r0, [r4, #38]
	strb	r0, [r4, #39]
	strb	r0, [r4, #56]
	strb	r0, [r4, #57]
	strb	r0, [r4, #58]
	strb	r0, [r4, #59]
	b	.L51
.L57:
	.align	2
.L56:
	.word	.LC5
	.word	.LANCHOR1+59
	.fnend
	.size	vrga_version, .-vrga_version
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_release, %function
vrga_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	ldr	r4, [r1, #128]
	mov	r0, r4
	bl	mpi_obj_unref
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bne	.L59
	movs	r5, #0
	mov	r8, #256
.L67:
	mov	r2, r5
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	mov	r4, r0
	cbz	r0, .L60
	ldr	r6, [r0, #60]
	cbz	r6, .L61
	mov	r1, r0
	mov	r0, r6
	bl	mpi_buf_pool_del_user
	movs	r1, #1
	mov	r0, r6
	bl	mpi_buf_pool_destroy
.L61:
	ldr	r6, [r4, #288]
	cbnz	r6, .L62
.L66:
	mov	r2, r5
	movs	r1, #0
	movs	r0, #18
	bl	vsys_remove_node_from_map
	mov	r0, r4
	bl	mpi_node_del_out_watcher
	mov	r0, r4
	bl	mpi_node_destroy
.L60:
	adds	r5, r5, #1
	cmp	r5, #64
	bne	.L67
	movw	r3, #654
	ldr	r2, .L78
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L78+4
	movs	r0, #0
	bl	vlog
.L59:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L62:
	ldr	r2, [r4, #76]
	mvn	r7, #43
	ldr	r0, [r6, #324]
	add	r6, r6, #324
	add	r3, r2, #33
	mov	r9, #290
	sub	r3, r7, r3, lsl #3
	add	r0, r0, r3
	add	r2, r0, r2, lsl #3
	ldr	r7, [r2, #308]
	add	r7, r7, r3
.L63:
	ldr	r3, [r4, #76]
	add	r2, r0, r3, lsl #3
	add	r2, r2, #308
	cmp	r2, r6
	beq	.L66
	cbz	r0, .L64
	add	r3, r0, r3, lsl #3
	ldrd	r1, r2, [r3, #308]
	str	r2, [r1, #4]
	str	r1, [r2]
	strd	r8, r9, [r3, #308]
	bl	mpi_obj_unref
.L64:
	ldr	r3, [r4, #76]
	mvn	r2, #43
	mov	r0, r7
	add	r1, r3, #33
	add	r3, r7, r3, lsl #3
	sub	r2, r2, r1, lsl #3
	ldr	r3, [r3, #308]
	adds	r7, r3, r2
	b	.L63
.L79:
	.align	2
.L78:
	.word	.LC6
	.word	.LANCHOR1+72
	.fnend
	.size	vrga_release, .-vrga_release
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
	push	{r3, lr}
	.save {r3, lr}
	mov	r1, #3520
	ldr	r3, .L85
	ldr	r0, [r3, #36]
	bl	kmem_cache_alloc
	cbz	r0, .L80
	movs	r3, #0
	movs	r2, #1
	str	r3, [r0, #16]
	strd	r3, r2, [r0, #4]
	add	r3, r0, #324
	str	r3, [r0, #324]
	str	r3, [r0, #328]
.L80:
	pop	{r3, pc}
.L86:
	.align	2
.L85:
	.word	kmalloc_caches
	.fnend
	.size	vrga_create_ctx, .-vrga_create_ctx
	.global	__aeabi_uidivmod
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
	ldr	r4, .L200
	movs	r5, #0
	mov	r6, r0
	movs	r2, #252
	mov	r1, r5
	str	r5, [sp, #132]
	ldr	r3, [r4]
	ldr	fp, .L200+12
	str	r3, [sp, #388]
	add	r3, r0, #12
	add	r0, sp, #136
	str	r3, [sp, #48]
	bl	memset
	movs	r2, #32
	mov	r1, r5
	add	r0, sp, #100
	bl	memset
	ldr	r3, .L200+4
	mov	r1, #3520
	ldr	r0, [r3, #20]
	bl	kmem_cache_alloc
	ldr	r3, [r6, #12]
	ldr	r2, .L200+8
	movs	r1, #5
	str	r3, [sp, #8]
	ldr	r3, [r6, #56]
	str	r0, [sp, #124]
	mov	r0, r5
	str	r3, [sp, #4]
	movw	r3, #854
	str	r3, [sp]
	ldr	r3, .L200+12
	bl	vlog
	str	r4, [sp, #52]
.L88:
	bl	kthread_should_stop
	cbz	r0, .L111
	ldr	r0, [sp, #124]
	cbz	r0, .L112
	bl	kfree
.L112:
	ldr	r3, [sp, #52]
	movs	r0, #0
	ldr	r2, [sp, #388]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L113
	bl	__stack_chk_fail
.L111:
	ldr	r7, [r6, #8]
	cbz	r7, .L89
	movs	r0, #1
	bl	msleep
	b	.L88
.L201:
	.align	2
.L200:
	.word	__stack_chk_guard
	.word	kmalloc_caches
	.word	.LC7
	.word	.LANCHOR1+85
.L89:
	add	r0, r6, #188
	bl	down
	mov	r1, r7
	ldr	r0, [sp, #48]
	bl	mpi_dev_run_job
	mov	r4, r0
	cmp	r0, #0
	beq	.L88
	ldr	r8, [r0, #48]
	cmp	r8, #0
	beq	.L91
	ldr	r5, [r8, #288]
	cmp	r5, #0
	beq	.L91
	str	r0, [r5]
	add	r9, sp, #132
	ldr	r3, [r0, #48]
	mov	r1, #256
	ldr	r2, .L202
	mov	r0, r9
	ldr	r3, [r3]
	str	r3, [sp]
	ldr	r3, [r6, #56]
	bl	snprintf
	ldr	r0, [r8, #56]
	ldrd	r3, r2, [r4, #60]
	ldr	r1, [r4, #16]
	strd	r5, r0, [sp, #36]
	ldr	r0, [r5, #4]
	str	r0, [sp, #32]
	ldr	r0, [r2, #448]
	str	r0, [sp, #28]
	mov	r0, r7
	ldr	r2, [r2]
	str	r2, [sp, #24]
	ldr	r2, [r3, #448]
	str	r2, [sp, #20]
	ldr	r3, [r3]
	str	r1, [sp, #8]
	movs	r1, #5
	ldr	r2, .L202+4
	strd	r9, r3, [sp, #12]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #879
	str	r3, [sp]
	mov	r3, fp
	bl	vlog
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne	.L92
	ldrd	r2, r3, [r5, #8]
	cmp	r2, r3
	bne	.L93
.L96:
	ldrd	r2, r3, [r5, #16]
	cmp	r2, r3
	bne	.L94
.L95:
	ldr	r3, [r4, #48]
	ldr	r10, [sp, #112]
	cmp	r3, #0
	beq	.L97
	ldr	r9, [r3, #288]
	cmp	r9, #0
	beq	.L97
	ldr	r7, [r4, #60]
	cmp	r7, #0
	beq	.L97
	ldr	r2, [r4, #64]
	cmp	r2, #0
	beq	.L97
	ldr	r1, [r7, #448]
	ldr	r2, [r2, #448]
	ldr	r3, [r3, #56]
	str	r1, [sp, #100]
	movs	r1, #0
	str	r2, [sp, #108]
	str	r1, [sp, #104]
	str	r3, [sp, #128]
	cmp	r10, #0
	beq	.L98
	movs	r1, #5
	ldr	r0, [r9, #12]
	bl	__aeabi_uidivmod
	add	r1, r9, r1, lsl #5
	ldr	r3, [r1, #28]
	cmp	r3, #0
	beq	.L99
	ldrd	r2, r3, [r1, #32]
	cmp	r2, #0
	bne	.L100
	str	r3, [sp, #84]
	add	r0, sp, #68
	ldr	r3, [r1, #40]
	str	r3, [sp, #88]
	ldr	r3, [r1, #44]
	str	r3, [sp, #92]
	ldr	r3, [r1, #48]
	add	r1, sp, #84
	str	r3, [sp, #96]
	ldrd	r2, r3, [r7, #620]
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
.L192:
	str	r2, [r3, #12]
	ldr	r3, [sp, #112]
	ldr	r2, [r7, #628]
	str	r2, [r3, #20]
	ldr	r2, [r7, #632]
	str	r2, [r3, #24]
	ldr	r0, [r7, #640]
	bl	pixel_format_rk_to_rga
	ldr	r3, [sp, #112]
	str	r0, [r3, #16]
	ldr	r0, [r7, #648]
	bl	compress_mode_rk_to_rga
	movs	r1, #5
	ldr	r3, [sp, #112]
	str	r0, [r3, #28]
	movs	r0, #0
	ldr	r3, [r9, #12]
	adds	r3, r3, #1
	str	r3, [r9, #12]
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
	ldr	r2, .L202+8
	str	r3, [sp, #16]
	ldr	r3, [sp, #128]
	str	r3, [sp, #12]
	ldr	r3, [r7, #448]
	str	r3, [sp, #8]
	ldr	r3, [r7]
	str	r3, [sp, #4]
	movw	r3, #733
	str	r3, [sp]
	ldr	r3, .L202+12
	bl	vlog
.L98:
	ldr	r2, [r4, #48]
	cmp	r2, #0
	beq	.L106
	ldr	r7, [r2, #288]
	cmp	r7, #0
	beq	.L106
	ldr	r3, [r4, #60]
	cmp	r3, #0
	beq	.L106
	ldr	r10, [r4, #64]
	cmp	r10, #0
	beq	.L106
	ldr	r1, [r3, #448]
	mov	r9, #0
	ldr	r2, [r2, #56]
	str	r3, [sp, #60]
	str	r1, [sp, #100]
	str	r2, [sp, #128]
	ldr	r1, [r10, #448]
	ldr	r2, [sp, #120]
	str	r9, [sp, #104]
	str	r1, [sp, #108]
	str	r2, [sp, #56]
	cmp	r2, #0
	beq	.L104
	ldr	r0, [r7, #20]
	movs	r1, #5
	bl	__aeabi_uidivmod
	movs	r0, #28
	ldr	r2, [sp, #56]
	mla	r1, r0, r1, r7
	str	r9, [r2]
	ldr	r2, [sp, #120]
	str	r9, [r2, #4]
	ldr	r2, [sp, #120]
	ldr	r0, [r1, #188]
	str	r0, [r2, #8]
	ldr	r2, [sp, #120]
	ldr	r0, [r1, #192]
	str	r0, [r2, #12]
	ldr	r2, [sp, #120]
	ldr	r0, [r1, #196]
	str	r0, [r2, #20]
	ldr	r2, [sp, #120]
	ldr	r0, [r1, #200]
	str	r0, [r2, #24]
	ldr	r0, [r1, #204]
	bl	pixel_format_rk_to_rga
	ldr	r2, [sp, #120]
	ldr	r3, [sp, #60]
	str	r0, [r2, #16]
	ldr	r0, [r3, #648]
	bl	compress_mode_rk_to_rga
	movs	r1, #5
	ldr	r3, [sp, #120]
	str	r0, [r3, #28]
	mov	r0, r9
	ldr	r3, [r7, #20]
	adds	r3, r3, #1
	str	r3, [r7, #20]
	ldr	r3, [sp, #120]
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
	ldr	r2, .L202+16
	str	r3, [sp, #16]
	ldr	r3, [sp, #128]
	str	r3, [sp, #12]
	ldr	r3, [r10, #448]
	str	r3, [sp, #8]
	ldr	r3, [r10]
	str	r3, [sp, #4]
	movw	r3, #791
	str	r3, [sp]
	ldr	r3, .L202+20
	bl	vlog
.L104:
	add	r0, sp, #100
	bl	rga_mpi_commit
	mov	r9, r0
	cmp	r0, #0
	bne	.L106
	ldrd	r3, r2, [r4, #60]
	ldr	r1, [r8, #56]
	str	r1, [sp, #28]
	ldr	r1, [r2, #448]
	str	r1, [sp, #24]
	movs	r1, #5
	ldr	r2, [r2]
	str	r2, [sp, #20]
	ldr	r2, [r3, #448]
	str	r2, [sp, #16]
	ldr	r3, [r3]
	ldr	r2, .L202+24
	str	r3, [sp, #12]
	add	r3, sp, #132
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #929
	str	r3, [sp]
	mov	r3, fp
	bl	vlog
	ldr	r3, [r4, #48]
	cmp	r3, #0
	beq	.L116
	ldr	r8, [r4, #60]
	cmp	r8, #0
	beq	.L116
	ldr	r7, [r4, #64]
	cmp	r7, #0
	beq	.L116
	b	.L203
.L204:
	.align	2
.L202:
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LANCHOR1+100
	.word	.LC11
	.word	.LANCHOR1+126
	.word	.LC12
.L203:
	ldr	r3, [sp, #124]
	ldr	r2, [r3, #8]
	str	r2, [r7, #620]
	ldr	r2, [r3, #12]
	str	r2, [r7, #624]
	ldr	r2, [r3, #20]
	str	r2, [r7, #628]
	ldr	r2, [r3, #24]
	str	r2, [r7, #632]
	ldr	r0, [r3, #16]
	bl	pixel_format_rga_to_rk
	ldr	r3, [sp, #124]
	str	r0, [r7, #640]
	ldr	r0, [r3, #28]
	bl	compress_mode_rga_to_rk
	str	r0, [r7, #648]
	ldr	r2, [r8, #668]
	ldr	r3, [sp, #124]
	str	r2, [r7, #668]
	ldrd	r0, [r8, #672]
	strd	r0, [r7, #672]
	strd	r0, [sp, #40]
	movs	r1, #5
	str	r2, [sp, #36]
	mov	r0, r9
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
	ldr	r2, .L205
	str	r3, [sp, #12]
	ldr	r3, [r7, #448]
	str	r3, [sp, #8]
	ldr	r3, [r7]
	str	r3, [sp, #4]
	movw	r3, #831
	str	r3, [sp]
	ldr	r3, .L205+4
	bl	vlog
	ldr	r3, [r8, #16]
	ldr	r2, [r7, #16]
	movs	r1, #5
	mov	r0, r9
	strd	r3, r2, [sp, #4]
	movw	r3, #833
	ldr	r2, .L205+8
	str	r3, [sp]
	ldr	r3, .L205+4
	bl	vlog
.L107:
	ldr	r0, [sp, #112]
	cbz	r0, .L108
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #112]
.L108:
	ldr	r0, [sp, #120]
	cbz	r0, .L109
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #120]
.L109:
	cmp	r9, #-1
	beq	.L91
.L92:
	movs	r1, #0
	str	r1, [r5]
	ldr	r3, [r6, #176]
	adds	r3, r3, #1
	str	r3, [r6, #176]
.L195:
	mov	r0, r4
	bl	mpi_job_done
	b	.L88
.L93:
	ldr	r3, .L205+12
	mov	r1, #3520
	ldr	r0, [r3, #20]
	bl	kmem_cache_alloc
	str	r0, [sp, #112]
	cmp	r0, #0
	bne	.L96
.L91:
	movs	r3, #0
	mov	r1, #-1
	str	r3, [r5]
	b	.L195
.L94:
	ldr	r3, .L205+12
	mov	r1, #3520
	ldr	r0, [r3, #20]
	bl	kmem_cache_alloc
	mov	r7, r0
	str	r0, [sp, #120]
	cmp	r0, #0
	bne	.L95
	ldr	r0, [sp, #112]
	cmp	r0, #0
	beq	.L91
	bl	kfree
	str	r7, [sp, #112]
	b	.L91
.L100:
	str	r3, [r10]
	ldr	r3, [sp, #112]
	ldr	r2, [r1, #40]
	str	r2, [r3, #4]
	ldr	r3, [sp, #112]
	ldr	r2, [r1, #44]
	str	r2, [r3, #8]
	ldr	r3, [sp, #112]
	ldr	r2, [r1, #48]
	b	.L192
.L99:
	str	r3, [r10]
	ldr	r2, [sp, #112]
	str	r3, [r2, #4]
	ldr	r3, [sp, #112]
	ldr	r2, [r7, #620]
	str	r2, [r3, #8]
	ldr	r3, [sp, #112]
	ldr	r2, [r7, #624]
	b	.L192
.L97:
	cmp	r10, #0
	beq	.L110
	mov	r0, r10
.L198:
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #112]
	b	.L110
.L116:
	mov	r9, #-1
	b	.L107
.L106:
	ldr	r0, [sp, #112]
	cmp	r0, #0
	bne	.L198
.L110:
	ldr	r0, [sp, #120]
	cmp	r0, #0
	beq	.L91
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #120]
	b	.L91
.L113:
	add	sp, sp, #396
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L206:
	.align	2
.L205:
	.word	.LC13
	.word	.LANCHOR1+152
	.word	.LC14
	.word	kmalloc_caches
	.fnend
	.size	vrga_thread_fn, .-vrga_thread_fn
	.align	1
	.global	new_rga_chn_id_by_ctx_id
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	new_rga_chn_id_by_ctx_id, %function
new_rga_chn_id_by_ctx_id:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	and	r4, r0, #63
	movs	r1, #0
	movs	r0, #18
	mov	r2, r4
	bl	vsys_get_node_from_map
	cbz	r0, .L207
	movs	r4, #0
.L210:
	mov	r2, r4
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	cbz	r0, .L207
	adds	r4, r4, #1
	cmp	r4, #64
	bne	.L210
.L207:
	mov	r0, r4
	pop	{r4, pc}
	.fnend
	.size	new_rga_chn_id_by_ctx_id, .-new_rga_chn_id_by_ctx_id
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
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #32
	sub	sp, sp, #32
	ldr	r3, [r0, #128]
	mov	r4, r1
	ldr	r0, [r1]	@ unaligned
	ldr	r7, [r3, #160]
	bl	new_rga_chn_id_by_ctx_id
	ldr	r3, [r4, #12]	@ unaligned
	cmp	r3, #36
	mov	r6, r0
	beq	.L216
	movs	r3, #133
	ldr	r2, .L230
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L230+4
	movs	r0, #0
	bl	vlog
.L229:
	mov	r0, #-1
.L215:
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L216:
	ldr	r0, .L230+8
	bl	mpi_allocator_find_by_name
	mov	r9, r0
	cmp	r0, #0
	beq	.L229
	movs	r3, #0
	movs	r2, #1
	ldr	r1, [r4]	@ unaligned
	add	r0, r7, #12
	bl	mpi_dev_create_node_with_params
	mov	r5, r0
	cmp	r0, #0
	beq	.L229
	ldr	r3, [r4, #20]	@ unaligned
	add	r2, r6, #52
	str	r3, [r7, r2, lsl #2]
	ldr	r7, [r0, #288]
	cbz	r7, .L220
	str	r3, [r7, #4]
.L220:
	ldr	r3, [r4]	@ unaligned
	movs	r1, #4
	ldr	r10, .L230+24
	movs	r0, #0
	ldr	r2, .L230+12
	strd	r6, r3, [sp, #4]
	movs	r3, #151
	str	r3, [sp]
	ldr	r3, .L230+4
	bl	vlog
	str	r10, [r5, #4]
	mov	r0, r5
	ldr	r1, [r4, #24]	@ unaligned
	bl	mpi_node_add_out_watcher
	mov	r3, r5
	mov	r2, r6
	movs	r1, #0
	movs	r0, #18
	bl	vsys_add_node_to_map
	add	r3, r5, #144
	add	r2, r5, #152
	ldr	r1, [r4, #32]	@ unaligned
	ldr	r0, [r4, #28]	@ unaligned
	bl	mpi_node_set_frame_rate
	ldr	r8, [r7, #4]
	cmp	r8, #0
	bne	.L221
	str	r10, [sp]
	movs	r1, #1
	ldr	r3, [r4, #16]	@ unaligned
	mov	r0, r9
	ldr	r2, [r4, #8]	@ unaligned
	bl	mpi_buf_pool_create
	mov	r7, r0
	cbnz	r0, .L222
	mov	r1, r0
	mov	r2, r6
	movs	r0, #18
	bl	vsys_remove_node_from_map
	mov	r0, r5
	bl	mpi_node_del_out_watcher
	mov	r0, r5
	bl	mpi_node_destroy
	b	.L229
.L222:
	ldr	r3, [r4, #8]	@ unaligned
	movs	r1, #4
	ldr	r2, .L230+16
	str	r3, [sp, #24]
	ldr	r3, [r4, #16]	@ unaligned
	str	r3, [sp, #20]
	ldr	r3, [r0]
	strd	r0, r3, [sp, #12]
	mov	r0, r8
	ldr	r3, [r4]	@ unaligned
	strd	r6, r3, [sp, #4]
	movs	r3, #167
	str	r3, [sp]
	ldr	r3, .L230+4
	bl	vlog
	mov	r1, r5
	mov	r0, r7
	bl	mpi_buf_pool_add_user
.L221:
	ldr	r3, [r4]	@ unaligned
	movs	r1, #4
	str	r6, [r4, #4]	@ unaligned
	movs	r0, #0
	ldr	r2, .L230+20
	strd	r6, r3, [sp, #4]
	movs	r3, #172
	str	r3, [sp]
	ldr	r3, .L230+4
	bl	vlog
	movs	r0, #0
	b	.L215
.L231:
	.align	2
.L230:
	.word	.LC15
	.word	.LANCHOR1+176
	.word	.LC16
	.word	.LC17
	.word	.LC19
	.word	.LC20
	.word	.LC18
	.fnend
	.size	vrga_node_create, .-vrga_node_create
	.align	1
	.global	vrga_get_node_site_from_map_by_nick_id
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_get_node_site_from_map_by_nick_id, %function
vrga_get_node_site_from_map_by_nick_id:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #20
	sub	sp, sp, #20
	mov	r4, r0
	mov	r5, r1
	strd	r0, r1, [sp, #4]
	mov	r6, r2
	mov	r7, r3
	ldr	r2, .L241
	movs	r3, #78
	movs	r1, #5
	str	r3, [sp]
	movs	r0, #0
	ldr	r3, .L241+4
	bl	vlog
	cmp	r4, #18
	bne	.L233
	movs	r4, #0
.L236:
	mov	r2, r4
	movs	r1, #0
	movs	r0, #18
	bl	vsys_get_node_from_map
	cbz	r0, .L234
	ldr	r3, [r0, #56]
	cmp	r5, r3
	bne	.L234
	movs	r0, #0
	str	r0, [r6]
	str	r4, [r7]
.L232:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L234:
	adds	r4, r4, #1
	cmp	r4, #64
	bne	.L236
.L233:
	mov	r0, #-1
	b	.L232
.L242:
	.align	2
.L241:
	.word	.LC21
	.word	.LANCHOR1+193
	.fnend
	.size	vrga_get_node_site_from_map_by_nick_id, .-vrga_get_node_site_from_map_by_nick_id
	.align	1
	.global	vrga_node_delete
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_delete, %function
vrga_node_delete:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #24
	sub	sp, sp, #24
	movs	r3, #64
	movs	r4, #0
	strd	r3, r4, [sp, #16]
	mov	r8, r1
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #36
	beq	.L244
	movs	r3, #194
	ldr	r2, .L260
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L260+4
	mov	r0, r4
	bl	vlog
.L253:
	mov	r5, #-1
	b	.L243
.L244:
	add	r3, sp, #16
	add	r2, sp, #20
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r5, r0
	cbz	r0, .L246
	ldr	r3, [r8]	@ unaligned
	movs	r1, #2
	ldr	r2, .L260+8
	mov	r0, r4
	str	r3, [sp, #8]
	ldr	r3, [sp, #16]
	str	r3, [sp, #4]
	movs	r3, #199
	str	r3, [sp]
	ldr	r3, .L260+4
	bl	vlog
	b	.L253
.L246:
	mov	r1, r0
	ldr	r2, [sp, #16]
	movs	r0, #18
	bl	vsys_get_node_from_map
	mov	r4, r0
	cmp	r0, #0
	beq	.L253
	ldr	r6, [r0, #60]
	cbz	r6, .L247
	mov	r1, r0
	mov	r0, r6
	bl	mpi_buf_pool_del_user
	movs	r1, #1
	mov	r0, r6
	bl	mpi_buf_pool_destroy
.L247:
	ldr	r6, [r4, #288]
	cbnz	r6, .L248
.L252:
	ldr	r2, [sp, #16]
	movs	r1, #0
	movs	r0, #18
	bl	vsys_remove_node_from_map
	mov	r0, r4
	bl	mpi_node_del_out_watcher
	mov	r0, r4
	bl	mpi_node_destroy
	ldr	r3, [r8]	@ unaligned
	ldr	r2, .L260+12
	movs	r1, #4
	movs	r0, #0
	str	r3, [sp, #8]
	ldr	r3, [sp, #16]
	str	r3, [sp, #4]
	movs	r3, #228
	str	r3, [sp]
	ldr	r3, .L260+4
	bl	vlog
.L243:
	mov	r0, r5
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L248:
	ldr	r2, [r4, #76]
	mvn	r7, #43
	ldr	r0, [r6, #324]
	add	r6, r6, #324
	add	r3, r2, #33
	mov	r9, #256
	mov	r10, #290
	sub	r3, r7, r3, lsl #3
	add	r0, r0, r3
	add	r2, r0, r2, lsl #3
	ldr	r7, [r2, #308]
	add	r7, r7, r3
.L249:
	ldr	r3, [r4, #76]
	add	r2, r0, r3, lsl #3
	add	r2, r2, #308
	cmp	r2, r6
	beq	.L252
	cbz	r0, .L250
	add	r3, r0, r3, lsl #3
	ldrd	r1, r2, [r3, #308]
	str	r2, [r1, #4]
	str	r1, [r2]
	strd	r9, r10, [r3, #308]
	bl	mpi_obj_unref
.L250:
	ldr	r3, [r4, #76]
	mvn	r2, #43
	mov	r0, r7
	add	r1, r3, #33
	add	r3, r7, r3, lsl #3
	sub	r2, r2, r1, lsl #3
	ldr	r3, [r3, #308]
	adds	r7, r3, r2
	b	.L249
.L261:
	.align	2
.L260:
	.word	.LC15
	.word	.LANCHOR1+232
	.word	.LC22
	.word	.LC23
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
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #24
	sub	sp, sp, #24
	movs	r3, #64
	movs	r6, #0
	strd	r3, r6, [sp, #16]
	mov	r5, r1
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #36
	beq	.L263
	movs	r3, #246
	ldr	r2, .L267
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L267+4
	mov	r0, r6
	bl	vlog
.L266:
	mov	r4, #-1
	b	.L262
.L263:
	add	r3, sp, #16
	add	r2, sp, #20
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r4, r0
	cbz	r0, .L265
	ldr	r3, [r5]	@ unaligned
	movs	r1, #2
	ldr	r2, .L267+8
	mov	r0, r6
	str	r3, [sp, #8]
	ldr	r3, [sp, #16]
	str	r3, [sp, #4]
	movs	r3, #251
	str	r3, [sp]
	ldr	r3, .L267+4
	bl	vlog
	b	.L266
.L265:
	mov	r1, r0
	ldr	r2, [sp, #16]
	movs	r0, #18
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L266
	bl	mpi_node_streamon
	ldr	r3, [r5]	@ unaligned
	ldr	r2, .L267+12
	movs	r1, #4
	mov	r0, r4
	str	r3, [sp, #8]
	ldr	r3, [sp, #16]
	str	r3, [sp, #4]
	movw	r3, #259
	str	r3, [sp]
	ldr	r3, .L267+4
	bl	vlog
.L262:
	mov	r0, r4
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L268:
	.align	2
.L267:
	.word	.LC15
	.word	.LANCHOR1+249
	.word	.LC22
	.word	.LC24
	.fnend
	.size	vrga_node_start, .-vrga_node_start
	.align	1
	.global	vrga_node_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_stop, %function
vrga_node_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #24
	sub	sp, sp, #24
	movs	r3, #64
	movs	r6, #0
	strd	r3, r6, [sp, #16]
	mov	r5, r1
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #36
	beq	.L270
	movw	r3, #271
	ldr	r2, .L274
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L274+4
	mov	r0, r6
	bl	vlog
.L273:
	mov	r4, #-1
	b	.L269
.L270:
	add	r3, sp, #16
	add	r2, sp, #20
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r4, r0
	cbz	r0, .L272
	ldr	r3, [r5]	@ unaligned
	movs	r1, #2
	ldr	r2, .L274+8
	mov	r0, r6
	str	r3, [sp, #8]
	ldr	r3, [sp, #16]
	str	r3, [sp, #4]
	mov	r3, #276
	str	r3, [sp]
	ldr	r3, .L274+4
	bl	vlog
	b	.L273
.L272:
	mov	r1, r0
	ldr	r2, [sp, #16]
	movs	r0, #18
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L273
	movs	r1, #1
	bl	mpi_node_flush
	ldr	r3, [r5]	@ unaligned
	ldr	r2, .L274+12
	movs	r1, #4
	mov	r0, r4
	str	r3, [sp, #8]
	ldr	r3, [sp, #16]
	str	r3, [sp, #4]
	movw	r3, #285
	str	r3, [sp]
	ldr	r3, .L274+4
	bl	vlog
.L269:
	mov	r0, r4
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L275:
	.align	2
.L274:
	.word	.LC15
	.word	.LANCHOR1+265
	.word	.LC22
	.word	.LC25
	.fnend
	.size	vrga_node_stop, .-vrga_node_stop
	.align	1
	.global	vrga_mb_get
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_mb_get, %function
vrga_mb_get:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #28
	sub	sp, sp, #28
	movs	r3, #64
	movs	r6, #0
	strd	r3, r6, [sp, #16]
	mov	r5, r1
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #104
	beq	.L277
	mov	r3, #300
	ldr	r2, .L292
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L292+4
	mov	r0, r6
	bl	vlog
.L281:
	mov	r4, #-1
.L276:
	mov	r0, r4
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L277:
	add	r3, sp, #16
	add	r2, sp, #20
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r4, r0
	cbz	r0, .L279
	ldr	r3, [r5]	@ unaligned
	movs	r1, #2
	ldr	r2, .L292+8
	mov	r0, r6
	str	r3, [sp, #8]
	ldr	r3, [sp, #16]
	str	r3, [sp, #4]
	movw	r3, #305
	str	r3, [sp]
	ldr	r3, .L292+4
	bl	vlog
	b	.L281
.L279:
	mov	r1, r0
	ldr	r2, [sp, #16]
	movs	r0, #18
	bl	vsys_get_node_from_map
	mov	r7, r0
	cmp	r0, #0
	beq	.L281
	ldr	r3, [r5, #8]	@ unaligned
	mov	r1, r4
	str	r3, [r0, #384]
	bl	mpi_node_get_out_buffer
	mov	r6, r0
	cmp	r0, #0
	beq	.L281
	ldr	r3, [r0, #448]
	cmp	r3, #0
	beq	.L281
	ldr	r3, [r7, #288]
	cbz	r3, .L282
	ldr	r2, [r7, #76]
	add	r3, r3, #324
	add	r1, r0, r2, lsl #3
	ldr	r0, [r3, #4]
	add	r2, r6, r2, lsl #3
	add	r1, r1, #308
	str	r1, [r3, #4]
	strd	r3, r0, [r2, #308]
	str	r1, [r0]
.L282:
	movs	r2, #80
	add	r1, r6, #616
	add	r0, r5, #24
	bl	memcpy
	ldr	r3, [r6, #448]
	ldr	r3, [r3]
	str	r3, [r5, #4]	@ unaligned
	ldr	r3, [r6]
	str	r3, [r5, #20]	@ unaligned
	b	.L276
.L293:
	.align	2
.L292:
	.word	.LC26
	.word	.LANCHOR1+280
	.word	.LC22
	.fnend
	.size	vrga_mb_get, .-vrga_mb_get
	.align	1
	.global	vrga_mb_release
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_mb_release, %function
vrga_mb_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #44
	sub	sp, sp, #44
	movs	r3, #64
	movs	r6, #0
	strd	r3, r6, [sp, #32]
	mov	r5, r1
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #104
	beq	.L295
	mov	r3, #338
	ldr	r2, .L300
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L300+4
	mov	r0, r6
	bl	vlog
.L299:
	mov	r4, #-1
	b	.L294
.L295:
	add	r3, sp, #32
	add	r2, sp, #36
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r4, r0
	cbz	r0, .L297
	ldr	r3, [r5]	@ unaligned
	movs	r1, #2
	ldr	r2, .L300+8
	mov	r0, r6
	str	r3, [sp, #8]
	ldr	r3, [sp, #32]
	str	r3, [sp, #4]
	movw	r3, #343
	str	r3, [sp]
	ldr	r3, .L300+4
	bl	vlog
	b	.L299
.L297:
	mov	r1, r0
	ldr	r2, [sp, #32]
	movs	r0, #18
	bl	vsys_get_node_from_map
	mov	r7, r0
	cmp	r0, #0
	beq	.L299
	movs	r1, #4
	ldr	r0, [r5, #20]	@ unaligned
	bl	mpi_obj_from_id
	mov	r6, r0
	cbz	r0, .L298
	ldr	r3, [r0, #16]
	movs	r1, #5
	ldr	r2, .L300+12
	str	r3, [sp, #24]
	ldr	r3, [r5, #20]	@ unaligned
	str	r3, [sp, #20]
	ldr	r3, [r0, #448]
	strd	r0, r3, [sp, #12]
	mov	r0, r4
	ldr	r3, [r5]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [sp, #32]
	str	r3, [sp, #4]
	mov	r3, #356
	str	r3, [sp]
	ldr	r3, .L300+4
	bl	vlog
	ldr	r3, [r7, #76]
	mov	r0, r7
	add	r3, r6, r3, lsl #3
	ldrd	r1, r2, [r3, #308]
	str	r2, [r1, #4]
	str	r1, [r2]
	mov	r1, #256
	mov	r2, #290
	strd	r1, r2, [r3, #308]
	add	r2, r7, #400
	mov	r1, r6
	bl	mpi_node_release_buffer_flag
.L294:
	mov	r0, r4
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L298:
	mov	r3, #360
	ldr	r2, .L300+16
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L300+4
	bl	vlog
	b	.L294
.L301:
	.align	2
.L300:
	.word	.LC26
	.word	.LANCHOR1+292
	.word	.LC22
	.word	.LC27
	.word	.LC28
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
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #44
	sub	sp, sp, #44
	movs	r3, #64
	movs	r6, #0
	strd	r3, r6, [sp, #32]
	mov	r4, r1
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #104
	beq	.L303
	movw	r3, #373
	ldr	r2, .L308
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L308+4
	mov	r0, r6
	bl	vlog
.L307:
	mov	r5, #-1
	b	.L302
.L303:
	add	r3, sp, #32
	add	r2, sp, #36
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r5, r0
	cbz	r0, .L305
	ldr	r3, [r4]	@ unaligned
	movs	r1, #2
	ldr	r2, .L308+8
	mov	r0, r6
	str	r3, [sp, #8]
	ldr	r3, [sp, #32]
	str	r3, [sp, #4]
	mov	r3, #378
	str	r3, [sp]
	ldr	r3, .L308+4
	bl	vlog
	b	.L307
.L305:
	mov	r1, r0
	ldr	r2, [sp, #32]
	movs	r0, #18
	bl	vsys_get_node_from_map
	mov	r7, r0
	cmp	r0, #0
	beq	.L307
	ldr	r0, [r4, #16]	@ unaligned
	cmp	r0, #0
	blt	.L302
	bl	get_mpi_buf
	ldr	r3, [r4, #16]	@ unaligned
	mov	r6, r0
	cbnz	r0, .L306
	str	r3, [sp, #4]
	movs	r1, #2
	mov	r3, #390
	ldr	r2, .L308+12
	str	r3, [sp]
	ldr	r3, .L308+4
	bl	vlog
	b	.L307
.L306:
	ldr	r2, [r0, #16]
	movs	r1, #5
	str	r3, [sp, #28]
	ldr	r3, [r0, #448]
	strd	r2, r3, [sp, #20]
	ldr	r3, [r0]
	ldr	r2, .L308+16
	strd	r0, r3, [sp, #12]
	mov	r0, r5
	ldr	r3, [r4], #24	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [sp, #32]
	str	r3, [sp, #4]
	movw	r3, #397
	str	r3, [sp]
	ldr	r3, .L308+4
	bl	vlog
	movs	r2, #80
	mov	r1, r4
	add	r0, r6, #616
	bl	memcpy
	mov	r2, r5
	mov	r1, r6
	mov	r0, r7
	bl	mpi_node_buffer_in
.L302:
	mov	r0, r5
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L309:
	.align	2
.L308:
	.word	.LC26
	.word	.LANCHOR1+308
	.word	.LC22
	.word	.LC29
	.word	.LC30
	.fnend
	.size	vrga_mb_send, .-vrga_mb_send
	.align	1
	.global	vrga_node_set_bypass
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_set_bypass, %function
vrga_node_set_bypass:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	movs	r3, #64
	movs	r0, #0
	strd	r3, r0, [sp, #8]
	mov	r5, r1
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #36
	beq	.L311
	movw	r3, #415
	ldr	r2, .L321
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L321+4
	bl	vlog
.L314:
	mov	r4, #-1
.L310:
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L311:
	add	r3, sp, #8
	add	r2, sp, #12
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r4, r0
	cmp	r0, #0
	bne	.L314
	mov	r1, r0
	ldr	r2, [sp, #8]
	movs	r0, #18
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L314
	ldr	r3, [sp, #8]
	ldr	r2, [r0, #288]
	ldr	r1, [r5, #20]	@ unaligned
	adds	r3, r3, #52
	ldr	r0, .L321+8
	str	r1, [r0, r3, lsl #2]
	cmp	r2, #0
	beq	.L310
	str	r1, [r2, #4]
	b	.L310
.L322:
	.align	2
.L321:
	.word	.LC15
	.word	.LANCHOR1+321
	.word	.LANCHOR0
	.fnend
	.size	vrga_node_set_bypass, .-vrga_node_set_bypass
	.align	1
	.global	vrga_node_set_crop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_set_crop, %function
vrga_node_set_crop:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #56
	sub	sp, sp, #56
	movs	r3, #64
	movs	r5, #0
	strd	r3, r5, [sp, #48]
	mov	r4, r1
	ldr	r3, [r1, #28]	@ unaligned
	cmp	r3, #32
	beq	.L324
	mov	r3, #444
	ldr	r2, .L337
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L337+4
	mov	r0, r5
.L336:
	bl	vlog
	b	.L335
.L324:
	add	r3, sp, #48
	add	r2, sp, #52
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r6, r0
	cbz	r0, .L326
	ldr	r3, [r4]	@ unaligned
	movs	r1, #2
	ldr	r2, .L337+8
	mov	r0, r5
	str	r3, [sp, #8]
	ldr	r3, [sp, #48]
	str	r3, [sp, #4]
	movw	r3, #449
	str	r3, [sp]
	ldr	r3, .L337+4
	bl	vlog
.L335:
	mov	r6, #-1
.L323:
	mov	r0, r6
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, pc}
.L326:
	mov	r1, r0
	ldr	r2, [sp, #48]
	movs	r0, #18
	bl	vsys_get_node_from_map
	cbnz	r0, .L327
	movw	r3, #455
	ldr	r2, .L337+12
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L337+4
	b	.L336
.L327:
	ldr	r5, [r0, #288]
	cbz	r5, .L328
	ldrd	r0, r2, [r5, #8]
	subs	r3, r0, r2
	cmp	r3, #0
	it	lt
	rsblt	r3, r3, #0
	cmp	r3, #4
	ble	.L329
	strd	r0, r2, [sp, #4]
	mov	r3, #462
	ldr	r2, .L337+16
	movs	r1, #2
	str	r3, [sp]
	mov	r0, r6
	ldr	r3, .L337+4
	bl	vlog
.L328:
	ldr	r3, [r5, #12]
	movs	r1, #4
	ldr	r2, .L337+20
	movs	r0, #0
	str	r3, [sp, #40]
	ldr	r3, [r5, #8]
	str	r3, [sp, #36]
	ldr	r3, [r4, #24]	@ unaligned
	str	r3, [sp, #32]
	ldr	r3, [r4, #20]	@ unaligned
	str	r3, [sp, #28]
	ldr	r3, [r4, #16]	@ unaligned
	str	r3, [sp, #24]
	ldr	r3, [r4, #12]	@ unaligned
	str	r3, [sp, #20]
	ldr	r3, [r4, #8]	@ unaligned
	str	r3, [sp, #16]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [sp, #48]
	str	r3, [sp, #4]
	mov	r3, #474
	str	r3, [sp]
	ldr	r3, .L337+4
	bl	vlog
	b	.L323
.L329:
	movs	r1, #5
	bl	__aeabi_uidivmod
	add	r2, r4, #32
	mov	r3, r4
	add	r1, r5, r1, lsl #5
	adds	r1, r1, #24
.L330:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r2
	str	r0, [r1], #4	@ unaligned
	bne	.L330
	ldr	r3, [r5, #8]
	adds	r3, r3, #1
	str	r3, [r5, #8]
	b	.L328
.L338:
	.align	2
.L337:
	.word	.LC31
	.word	.LANCHOR1+342
	.word	.LC22
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.fnend
	.size	vrga_node_set_crop, .-vrga_node_set_crop
	.align	1
	.global	vrga_node_set_scale
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_set_scale, %function
vrga_node_set_scale:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #52
	sub	sp, sp, #52
	movs	r3, #64
	movs	r0, #0
	strd	r3, r0, [sp, #40]
	mov	r4, r1
	ldr	r7, [r1, #24]	@ unaligned
	cmp	r7, #28
	beq	.L340
	mov	r3, #488
	ldr	r2, .L351
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L351+4
	bl	vlog
.L346:
	mov	r6, #-1
	b	.L339
.L340:
	add	r3, sp, #40
	add	r2, sp, #44
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r6, r0
	cmp	r0, #0
	bne	.L346
	mov	r1, r0
	ldr	r2, [sp, #40]
	movs	r0, #18
	bl	vsys_get_node_from_map
	cbnz	r0, .L342
	ldr	r3, [r4]	@ unaligned
	movs	r1, #2
	ldr	r2, .L351+8
	str	r3, [sp, #8]
	ldr	r3, [sp, #40]
	str	r3, [sp, #4]
	mov	r3, #498
	str	r3, [sp]
	ldr	r3, .L351+4
	bl	vlog
	b	.L346
.L342:
	ldr	r5, [r0, #288]
	cbz	r5, .L343
	ldrd	r0, r2, [r5, #16]
	subs	r3, r0, r2
	cmp	r3, #0
	it	lt
	rsblt	r3, r3, #0
	cmp	r3, #4
	ble	.L344
	strd	r0, r2, [sp, #4]
	movw	r3, #505
	ldr	r2, .L351+12
	movs	r1, #2
	str	r3, [sp]
	mov	r0, r6
	ldr	r3, .L351+4
	bl	vlog
.L343:
	ldr	r3, [r5, #20]
	movs	r1, #4
	ldr	r2, .L351+16
	movs	r0, #0
	str	r3, [sp, #36]
	ldr	r3, [r5, #16]
	str	r3, [sp, #32]
	ldr	r3, [r4, #20]	@ unaligned
	str	r3, [sp, #28]
	ldr	r3, [r4, #16]	@ unaligned
	str	r3, [sp, #24]
	ldr	r3, [r4, #8]	@ unaligned
	str	r3, [sp, #20]
	ldr	r3, [r4, #12]	@ unaligned
	str	r3, [sp, #16]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [sp, #40]
	str	r3, [sp, #4]
	movw	r3, #518
	str	r3, [sp]
	ldr	r3, .L351+4
	bl	vlog
.L339:
	mov	r0, r6
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L344:
	movs	r1, #5
	bl	__aeabi_uidivmod
	add	r2, r4, #28
	mov	r3, r4
	mla	r1, r7, r1, r5
	adds	r1, r1, #184
.L345:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r2
	str	r0, [r1], #4	@ unaligned
	bne	.L345
	ldr	r3, [r5, #16]
	adds	r3, r3, #1
	str	r3, [r5, #16]
	b	.L343
.L352:
	.align	2
.L351:
	.word	.LC35
	.word	.LANCHOR1+361
	.word	.LC22
	.word	.LC33
	.word	.LC36
	.fnend
	.size	vrga_node_set_scale, .-vrga_node_set_scale
	.align	1
	.global	vrga_node_frame_rate_ctl
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrga_node_frame_rate_ctl, %function
vrga_node_frame_rate_ctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	movs	r3, #64
	movs	r0, #0
	strd	r3, r0, [sp, #8]
	mov	r5, r1
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #36
	beq	.L354
	movw	r3, #531
	ldr	r2, .L361
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L361+4
	bl	vlog
.L357:
	mov	r4, #-1
.L353:
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L354:
	add	r3, sp, #8
	add	r2, sp, #12
	ldr	r1, [r1]	@ unaligned
	movs	r0, #18
	bl	vrga_get_node_site_from_map_by_nick_id
	mov	r4, r0
	cmp	r0, #0
	bne	.L357
	mov	r1, r0
	ldr	r2, [sp, #8]
	movs	r0, #18
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L357
	add	r3, r0, #144
	add	r2, r0, #152
	ldr	r1, [r5, #32]	@ unaligned
	ldr	r0, [r5, #28]	@ unaligned
	bl	mpi_node_set_frame_rate
	b	.L353
.L362:
	.align	2
.L361:
	.word	.LC15
	.word	.LANCHOR1+381
	.fnend
	.size	vrga_node_frame_rate_ctl, .-vrga_node_frame_rate_ctl
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
	ldr	r0, .L369
	mov	r6, r1
	bl	mpi_dev_request_job
	mov	r4, r0
	cbz	r0, .L363
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L365
	bl	mpi_obj_unref
	movs	r4, #0
.L363:
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L365:
	strd	r5, r6, [r0, #60]
	movs	r1, #5
	ldr	r3, [r6]
	ldr	r2, .L369+4
	str	r3, [sp, #12]
	ldr	r3, [r5]
	str	r3, [sp, #8]
	ldr	r3, [r0]
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #1145
	str	r3, [sp]
	ldr	r3, .L369+8
	bl	vlog
	mov	r0, r4
	bl	mpi_job_set_ready
	mov	r0, r4
	bl	mpi_obj_ref
	b	.L363
.L370:
	.align	2
.L369:
	.word	.LANCHOR0+12
	.word	.LC37
	.word	.LANCHOR1+406
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
	mov	r2, #464
	ldr	r4, .L374
	mov	r5, r0
	movs	r1, #0
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r5
	bl	kstrdup
	movs	r3, #1
	str	r3, [r4, #4]
	movs	r2, #21
	ldr	r3, .L374+4
	mov	r1, r4
	str	r3, [r4, #180]
	movs	r3, #0
	strd	r2, r3, [r4, #184]
	add	r3, r4, #192
	mov	r2, #-1
	strd	r3, r3, [r4, #192]
	ldr	r3, .L374+8
	str	r0, [r4]
	ldr	r0, .L374+12
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r6, r0
	bhi	.L372
	bl	wake_up_process
.L372:
	ldr	r3, .L374+16
	mov	r2, #-1
	ldr	r1, .L374
	ldr	r0, .L374+12
	str	r6, [r4, #200]
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r6, r0
	bhi	.L373
	bl	wake_up_process
.L373:
	ldr	r3, .L374+20
	mov	r2, r5
	movs	r1, #8
	ldr	r0, .L374+24
	str	r6, [r4, #204]
	str	r3, [r4, #80]
	ldr	r3, .L374+28
	str	r4, [r4, #172]
	str	r3, [r4, #88]
	ldr	r3, .L374+32
	str	r3, [r4, #92]
	bl	mpi_dev_register
	movs	r2, #0
	ldr	r1, .L374+36
	ldr	r0, .L374+24
	pop	{r4, r5, r6, lr}
	b	mpi_sysfs_dev_add
.L375:
	.align	2
.L374:
	.word	.LANCHOR0
	.word	.LANCHOR1+424
	.word	.LC38
	.word	vrga_thread_fn
	.word	.LC39
	.word	vrga_create_ctx
	.word	.LANCHOR0+12
	.word	vrga_destroy_ctx
	.word	vrga_handle_message
	.word	.LANCHOR1+844
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
	ldr	r4, .L377
	ldr	r0, [r4, #200]
	str	r3, [r4, #8]
	bl	kthread_stop
	ldr	r0, [r4, #204]
	bl	kthread_stop
	ldr	r0, [r4], #12
	bl	kfree
	mov	r0, r4
	pop	{r4, lr}
	b	mpi_dev_unregister
.L378:
	.align	2
.L377:
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
	ldr	r0, .L380
	bl	vrga_probe
	movs	r0, #0
	pop	{r3, pc}
.L381:
	.align	2
.L380:
	.word	.LC18
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
	.type	__func__.36644, %object
	.size	__func__.36644, 10
__func__.36644:
	.ascii	"vrga_open\000"
	.type	__func__.36628, %object
	.size	__func__.36628, 11
__func__.36628:
	.ascii	"vrga_ioctl\000"
	.type	__func__.36791, %object
	.size	__func__.36791, 25
__func__.36791:
	.ascii	"vrga_handle_node_message\000"
	.type	__func__.36755, %object
	.size	__func__.36755, 13
__func__.36755:
	.ascii	"vrga_new_job\000"
	.type	__func__.36435, %object
	.size	__func__.36435, 13
__func__.36435:
	.ascii	"vrga_version\000"
	.type	__func__.36681, %object
	.size	__func__.36681, 13
__func__.36681:
	.ascii	"vrga_release\000"
	.type	__func__.36731, %object
	.size	__func__.36731, 15
__func__.36731:
	.ascii	"vrga_thread_fn\000"
	.type	__func__.36693, %object
	.size	__func__.36693, 26
__func__.36693:
	.ascii	"vrga_set_rga_job_src_info\000"
	.type	__func__.36703, %object
	.size	__func__.36703, 26
__func__.36703:
	.ascii	"vrga_set_rga_job_dst_info\000"
	.type	__func__.36711, %object
	.size	__func__.36711, 24
__func__.36711:
	.ascii	"vrga_set_dst_frame_info\000"
	.type	__func__.36448, %object
	.size	__func__.36448, 17
__func__.36448:
	.ascii	"vrga_node_create\000"
	.type	__func__.36421, %object
	.size	__func__.36421, 39
__func__.36421:
	.ascii	"vrga_get_node_site_from_map_by_nick_id\000"
	.type	__func__.36462, %object
	.size	__func__.36462, 17
__func__.36462:
	.ascii	"vrga_node_delete\000"
	.type	__func__.36493, %object
	.size	__func__.36493, 16
__func__.36493:
	.ascii	"vrga_node_start\000"
	.type	__func__.36502, %object
	.size	__func__.36502, 15
__func__.36502:
	.ascii	"vrga_node_stop\000"
	.type	__func__.36513, %object
	.size	__func__.36513, 12
__func__.36513:
	.ascii	"vrga_mb_get\000"
	.type	__func__.36523, %object
	.size	__func__.36523, 16
__func__.36523:
	.ascii	"vrga_mb_release\000"
	.type	__func__.36537, %object
	.size	__func__.36537, 13
__func__.36537:
	.ascii	"vrga_mb_send\000"
	.type	__func__.36551, %object
	.size	__func__.36551, 21
__func__.36551:
	.ascii	"vrga_node_set_bypass\000"
	.type	__func__.36561, %object
	.size	__func__.36561, 19
__func__.36561:
	.ascii	"vrga_node_set_crop\000"
	.type	__func__.36583, %object
	.size	__func__.36583, 20
__func__.36583:
	.ascii	"vrga_node_set_scale\000"
	.type	__func__.36604, %object
	.size	__func__.36604, 25
__func__.36604:
	.ascii	"vrga_node_frame_rate_ctl\000"
	.type	__func__.36821, %object
	.size	__func__.36821, 18
__func__.36821:
	.ascii	"vrga_user_request\000"
	.type	vrga_ioctls, %object
	.size	vrga_ioctls, 420
vrga_ioctls:
	.word	-1068731904
	.word	0
	.word	vrga_version
	.word	.LC40
	.word	0
	.word	-1071353343
	.word	0
	.word	vrga_node_create
	.word	.LC41
	.word	0
	.word	-1071353342
	.word	0
	.word	vrga_node_delete
	.word	.LC42
	.word	0
	.word	-1071353341
	.word	0
	.word	vrga_node_get_status
	.word	.LC43
	.word	0
	.word	-1071353340
	.word	0
	.word	vrga_node_start
	.word	.LC44
	.word	0
	.word	-1071353339
	.word	0
	.word	vrga_node_stop
	.word	.LC45
	.word	0
	.word	-1066896890
	.word	16
	.word	vrga_mb_get
	.word	.LC46
	.word	0
	.word	-1066896889
	.word	0
	.word	vrga_mb_send
	.word	.LC47
	.word	0
	.word	-1066896888
	.word	0
	.word	vrga_mb_release
	.word	.LC48
	.word	0
	.word	-1071353335
	.word	0
	.word	vrga_node_set_bypass
	.word	.LC49
	.word	0
	.space	120
	.word	-1071615472
	.word	0
	.word	vrga_node_set_crop
	.word	.LC50
	.word	0
	.word	-1071353327
	.word	0
	.word	vrga_node_frame_rate_ctl
	.word	.LC51
	.word	0
	.space	40
	.word	-1071877612
	.word	0
	.word	vrga_node_set_scale
	.word	.LC52
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
	.word	vrga_release
	.space	60
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_vrga_dev, %object
	.size	g_vrga_dev, 464
g_vrga_dev:
	.space	464
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[1;32m--------- vrga open ----------\033[0m\000"
.LC1:
	.ascii	"\033[1;31mvrga: error vrga ioctl\033[0m\000"
.LC2:
	.ascii	"vrga(%s-%d) buf(%d) ready\000"
.LC3:
	.ascii	"vrga job(%d) ready for (%s-%d) buf(%d->%d)\000"
.LC4:
	.ascii	"vrga(%s-%d) drop job(%d)->refcnt(%d), src_buf(%d)->"
	.ascii	"refcnt(%d)\000"
.LC5:
	.ascii	"\033[1;31mstruct vrga_version mismatching\033[0m\000"
.LC6:
	.ascii	"\033[1;32m--------- vrga release ----------\033[0m\000"
.LC7:
	.ascii	"dev(%s-%d) register ok\000"
.LC8:
	.ascii	"%s-%d\000"
.LC9:
	.ascii	"vrga job(%d-%d) run for (%s) buf(%d: %p->%d: %p), v"
	.ascii	"ctx(%d: %p), nick_id(%d)\000"
.LC10:
	.ascii	"src buf(%d: %p), rga: ctx_id(%d), x(%d), y(%d), w(%"
	.ascii	"d), h(%d), vw(%d), vh(%d), pf(0x%x), cp(%d)\000"
.LC11:
	.ascii	"dst buf(%d: %p), rga: ctx_id(%d), x(%d), y(%d), w(%"
	.ascii	"d), h(%d), vw(%d), vh(%d), pf(0x%x), cp(%d)\000"
.LC12:
	.ascii	"vrga job(%d) done for (%s) buf(%d: %p->%d: %p), nic"
	.ascii	"k_id(%d)\000"
.LC13:
	.ascii	"dst buf(%d: %p), rga: w(%d), h(%d), vw(%d), vh(%d),"
	.ascii	" pf(0x%x), cp(%d), seq(%d), pts(%lld)\000"
.LC14:
	.ascii	"src_ref(%d), dst_ref(%d)\000"
.LC15:
	.ascii	"\033[1;31mstruct vrga_node_info mismatching\033[0m\000"
.LC16:
	.ascii	"valloc\000"
.LC17:
	.ascii	"\033[1;32mRGA %d(%d) node create success!\033[0m\000"
.LC18:
	.ascii	"vrga\000"
.LC19:
	.ascii	"\033[1;32mRGA %d(%d) create MB_SOURCE_PRIVATE succe"
	.ascii	"ss! pool %p pool_id %d bufffer_cnt %d bufffer_size "
	.ascii	"%d\033[0m\000"
.LC20:
	.ascii	"\033[1;32mRGA %d(%d) node create done!\033[0m\000"
.LC21:
	.ascii	"mod = %d nick_id = %d\000"
.LC22:
	.ascii	"\033[1;31mRGA (%d)%d node not find!\033[0m\000"
.LC23:
	.ascii	"\033[1;32mRGA %d(%d) node destroy done!\033[0m\000"
.LC24:
	.ascii	"\033[1;32mRGA %d(%d) node start done!\033[0m\000"
.LC25:
	.ascii	"\033[1;32mRGA %d(%d) node stop done!\033[0m\000"
.LC26:
	.ascii	"\033[1;31mstruct vrga_buff_info mismatching\033[0m\000"
.LC27:
	.ascii	"RGA %d(%d) node release mpi_buf(%p), dma_buf(%p), b"
	.ascii	"uf_id(%ld), buf_refcnt(%d).\000"
.LC28:
	.ascii	"\033[1;31mbuf null\033[0m\000"
.LC29:
	.ascii	"\033[1;31merror cannot find mpi_buf fr fd %d\033[0m"
	.ascii	"\000"
.LC30:
	.ascii	"RGA %d(%d) node mpi_buf(%p), buf_id(%d), buf_refcnt"
	.ascii	"(%d), dma_buf(%p), fd(%d).\000"
.LC31:
	.ascii	"\033[1;31mstruct vrga_crop_info mismatching\033[0m\000"
.LC32:
	.ascii	"\033[1;31mnot found vrga_node\033[0m\000"
.LC33:
	.ascii	"\033[1;31mexceed max rga buf cnt(0x%x-0x%x)\033[0m\000"
.LC34:
	.ascii	"\033[1;32mRGA %d(%d) node enable(%d), ration(0)/abs"
	.ascii	"(1)(%d), x(%d), y(%d), w(%d), h(%d), s/g(0x%x-0x%x)"
	.ascii	"\033[0m\000"
.LC35:
	.ascii	"\033[1;31mstruct vrga_scale_info mismatching\033[0m"
	.ascii	"\000"
.LC36:
	.ascii	"\033[1;32mRGA %d(%d) node w(%d), h(%d), vw(%d), vh("
	.ascii	"%d), fmt(%d), s/g(0x%x-0x%x)\033[0m\000"
.LC37:
	.ascii	"vrga job(%d) ready for user, buf(%d->%d)\000"
.LC38:
	.ascii	"vrga_0\000"
.LC39:
	.ascii	"vrga_1\000"
.LC40:
	.ascii	"VRGA_IOCTL_VERSION\000"
.LC41:
	.ascii	"VRGA_IOCTL_NODE_CREATE\000"
.LC42:
	.ascii	"VRGA_IOCTL_NODE_DELETE\000"
.LC43:
	.ascii	"VRGA_IOCTL_NODE_GET_STATUS\000"
.LC44:
	.ascii	"VRGA_IOCTL_NODE_START\000"
.LC45:
	.ascii	"VRGA_IOCTL_NODE_STOP\000"
.LC46:
	.ascii	"VRGA_IOCTL_MB_GET\000"
.LC47:
	.ascii	"VRGA_IOCTL_MB_SEND\000"
.LC48:
	.ascii	"VRGA_IOCTL_MB_RELEASE\000"
.LC49:
	.ascii	"VRGA_IOCTL_SET_BYPASS\000"
.LC50:
	.ascii	"VRGA_IOCTL_SET_CROP\000"
.LC51:
	.ascii	"VRGA_IOCTL_FRAME_RATE_CTL\000"
.LC52:
	.ascii	"VRGA_IOCTL_SET_SCALE\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
