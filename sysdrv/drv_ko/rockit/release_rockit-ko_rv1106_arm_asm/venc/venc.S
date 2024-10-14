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
	.file	"venc.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	vcodec_get_chnl_ctx
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_get_chnl_ctx, %function
vcodec_get_chnl_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #288]
	bx	lr
	.fnend
	.size	vcodec_get_chnl_ctx, .-vcodec_get_chnl_ctx
	.align	1
	.global	vcodec_get_chnl_id
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_get_chnl_id, %function
vcodec_get_chnl_id:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #56]
	bx	lr
	.fnend
	.size	vcodec_get_chnl_id, .-vcodec_get_chnl_id
	.align	1
	.global	vcodec_get_chnl_type
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_get_chnl_type, %function
vcodec_get_chnl_type:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #1
	bx	lr
	.fnend
	.size	vcodec_get_chnl_type, .-vcodec_get_chnl_type
	.align	1
	.global	mpi_buf_map
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_map, %function
mpi_buf_map:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, [r0, #604]
	bx	lr
	.fnend
	.size	mpi_buf_map, .-mpi_buf_map
	.align	1
	.global	mpi_buf_unmap
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_unmap, %function
mpi_buf_unmap:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpi_buf_unmap, .-mpi_buf_unmap
	.align	1
	.global	mpi_buf_get_dmabuf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_get_dmabuf, %function
mpi_buf_get_dmabuf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbz	r0, .L7
	ldr	r0, [r0, #448]
.L7:
	bx	lr
	.fnend
	.size	mpi_buf_get_dmabuf, .-mpi_buf_get_dmabuf
	.align	1
	.global	mpi_buffer_queue_size
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buffer_queue_size, %function
mpi_buffer_queue_size:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L12
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #340]
	bx	lr
.L13:
	.align	2
.L12:
	.word	g_venc_dev
	.fnend
	.size	mpi_buffer_queue_size, .-mpi_buffer_queue_size
	.align	1
	.global	mpi_buffer_pool_get_free_num
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buffer_pool_get_free_num, %function
mpi_buffer_pool_get_free_num:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbz	r0, .L15
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L16
.L15:
	movs	r0, #1
	bx	lr
.L16:
	ldr	r0, [r0, #68]
	bx	lr
	.fnend
	.size	mpi_buffer_pool_get_free_num, .-mpi_buffer_pool_get_free_num
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_compat_ioctl, %function
venc_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	venc_compat_ioctl, .-venc_compat_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_poll, %function
venc_poll:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r2, r1
	ldr	r3, [r0, #128]
	ldr	r1, [r3]
	cmp	r1, #20
	beq	.L23
.L26:
	movs	r0, #0
.L22:
	pop	{r4, r5, r6, pc}
.L23:
	ldr	r4, [r3, #8]
	ldr	r5, .L36
	cbz	r2, .L25
	ldr	r6, [r2]
	cbz	r6, .L25
	mov	r3, #988
	mla	r3, r3, r4, r5
	addw	r1, r3, #3380
	blx	r6
.L25:
	mov	r3, #988
	mla	r4, r3, r4, r5
	ldr	r3, [r4, #2464]
	cmp	r3, #0
	beq	.L26
	movs	r0, #65
	b	.L22
.L37:
	.align	2
.L36:
	.word	g_venc_dev
	.fnend
	.size	venc_poll, .-venc_poll
	.align	1
	.global	venc_node_pp_get_md_attr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_pp_get_md_attr, %function
venc_node_pp_get_md_attr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r3, [r1, #4]	@ unaligned
	cmp	r3, #36
	beq	.L39
	movw	r3, #2091
	ldr	r2, .L48
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L48+4
	movs	r0, #4
	bl	vlog
.L47:
	mov	r0, #-1
.L38:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L39:
	ldr	r2, [r1]	@ unaligned
	cmp	r2, #7
	ble	.L41
	movs	r3, #8
	movs	r1, #2
	strd	r2, r3, [sp, #4]
	movs	r0, #4
	mov	r3, #2096
	ldr	r2, .L48+8
	str	r3, [sp]
	ldr	r3, .L48+4
	bl	vlog
	b	.L47
.L41:
	ldr	r0, .L48+12
	mov	r4, #988
	mla	r4, r4, r2, r0
	ldr	r3, [r4, #3272]
	cmp	r3, #0
	ble	.L42
	ldr	r3, [r4, #3268]
	cmp	r3, #0
	ble	.L42
	str	r3, [r1, #8]	@ unaligned
	ldr	r3, [r4, #3272]
	str	r3, [r1, #12]	@ unaligned
.L43:
	mov	r3, #988
	mla	r3, r3, r2, r0
	movs	r0, #0
	ldr	r2, [r3, #3276]
	str	r2, [r1, #16]	@ unaligned
	ldr	r2, [r3, #3280]
	str	r2, [r1, #20]	@ unaligned
	ldr	r2, [r3, #3284]
	str	r2, [r1, #24]	@ unaligned
	ldr	r2, [r3, #3288]
	str	r2, [r1, #28]	@ unaligned
	ldr	r3, [r3, #3292]
	str	r3, [r1, #32]	@ unaligned
	b	.L38
.L42:
	mov	r3, #988
	mla	r3, r3, r2, r0
	ldr	r4, [r3, #2576]
	movs	r3, #0
	cmp	r4, #1
	bne	.L44
	movs	r4, #70
	strb	r4, [r1, #8]
	movs	r4, #3
	strb	r3, [r1, #9]
	strb	r3, [r1, #10]
	strb	r3, [r1, #11]
.L46:
	strb	r4, [r1, #12]
	strb	r3, [r1, #13]
	strb	r3, [r1, #14]
	strb	r3, [r1, #15]
	b	.L43
.L44:
	cmp	r4, #2
	bne	.L45
	movs	r5, #50
	strb	r3, [r1, #9]
	strb	r5, [r1, #8]
	strb	r3, [r1, #10]
	strb	r3, [r1, #11]
	b	.L46
.L45:
	movs	r4, #30
	strb	r3, [r1, #9]
	strb	r4, [r1, #8]
	movs	r4, #1
	strb	r3, [r1, #10]
	strb	r3, [r1, #11]
	b	.L46
.L49:
	.align	2
.L48:
	.word	.LC0
	.word	.LANCHOR0
	.word	.LC1
	.word	g_venc_dev
	.fnend
	.size	venc_node_pp_get_md_attr, .-venc_node_pp_get_md_attr
	.align	1
	.global	venc_node_enable_tb
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_enable_tb, %function
venc_node_enable_tb:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r3, [r1]
	cmp	r3, #15
	ble	.L51
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #2129
	ldr	r2, .L53
	str	r3, [sp]
	ldr	r3, .L53+4
	bl	vlog
	mov	r0, #-1
.L50:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L51:
	ldr	r2, .L53+8
	movs	r1, #1
	movs	r0, #0
	strb	r1, [r3, r2]
	b	.L50
.L54:
	.align	2
.L53:
	.word	.LC1
	.word	.LANCHOR0+25
	.word	g_venc_dev+10304
	.fnend
	.size	venc_node_enable_tb, .-venc_node_enable_tb
	.align	1
	.global	venc_node_tb_bind
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_tb_bind, %function
venc_node_tb_bind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r0, [r1]	@ unaligned
	ldr	r2, [r1, #4]	@ unaligned
	cmp	r0, #15
	bgt	.L56
	cmp	r2, #15
	ble	.L57
.L56:
	movs	r3, #16
	str	r0, [sp, #4]
	strd	r2, r3, [sp, #8]
	movs	r1, #2
	mov	r3, #2144
	ldr	r2, .L59
	str	r3, [sp]
	movs	r0, #4
	ldr	r3, .L59+4
	bl	vlog
	mov	r0, #-1
.L55:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L57:
	ldr	r3, .L59+8
	movw	r1, #10320
	add	r3, r3, r0
	movs	r0, #0
	strb	r2, [r3, r1]
	b	.L55
.L60:
	.align	2
.L59:
	.word	.LC2
	.word	.LANCHOR0+45
	.word	g_venc_dev
	.fnend
	.size	venc_node_tb_bind, .-venc_node_tb_bind
	.align	1
	.global	venc_node_tb_request
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_tb_request, %function
venc_node_tb_request:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r3, [r1]
	cmp	r3, #15
	ble	.L62
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #2158
	ldr	r2, .L64
	str	r3, [sp]
	ldr	r3, .L64+4
	bl	vlog
	mov	r0, #-1
.L61:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L62:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, .L64+8
	addw	r3, r3, #2584
	movs	r0, #1
	str	r0, [r1, r3, lsl #2]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	b	.L61
.L65:
	.align	2
.L64:
	.word	.LC1
	.word	.LANCHOR0+63
	.word	g_venc_dev
	.fnend
	.size	venc_node_tb_request, .-venc_node_tb_request
	.align	1
	.global	mpi_buf_ref
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_ref, %function
mpi_buf_ref:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4, r5, r6}
	.save {r4, r5, r6}
	cbz	r0, .L66
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r5, .L72
	movs	r3, #0
	movs	r1, #12
.L70:
	mul	r2, r1, r3
	ldr	r6, [r2, r5]
	cmp	r6, r0
	bne	.L68
	ldr	r3, .L72+4
	movw	r1, #37028
	add	r3, r3, r2
	ldr	r2, [r3, r1]
	adds	r2, r2, #1
	str	r2, [r3, r1]
.L69:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r4, r5, r6}
	b	mpi_obj_ref
.L68:
	adds	r3, r3, #1
	cmp	r3, #512
	bne	.L70
	b	.L69
.L66:
	pop	{r4, r5, r6}
	bx	lr
.L73:
	.align	2
.L72:
	.word	g_venc_dev+37024
	.word	g_venc_dev
	.fnend
	.size	mpi_buf_ref, .-mpi_buf_ref
	.align	1
	.global	mpi_buffer_queue_push
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buffer_queue_push, %function
mpi_buffer_queue_push:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	mpi_queue_push_obj
	pop	{r3, pc}
	.fnend
	.size	mpi_buffer_queue_push, .-mpi_buffer_queue_push
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_venc_node, %function
get_venc_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, r0
	movs	r1, #0
	movs	r0, #4
	b	vsys_get_node_from_map
	.fnend
	.size	get_venc_node, .-get_venc_node
	.align	1
	.global	vcodec_set_intra_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_set_intra_info, %function
vcodec_set_intra_info:
	.fnstart
	@ args = 12, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	cmp	r0, #15
	.pad #28
	sub	sp, sp, #28
	mov	r4, r0
	strd	r2, [sp, #16]
	bls	.L77
	movs	r3, #16
	ldr	r2, .L114
	strd	r0, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #942
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L114+4
	bl	vlog
	mov	r0, #-1
.L76:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L77:
	bl	get_venc_node
	cbz	r0, .L79
	ldr	r3, [r0, #116]
	cbz	r3, .L79
	blx	r3
.L79:
	ldr	r5, .L114+8
	add	r3, r5, r4, lsl #2
	ldr	r3, [r3, #404]
	cmp	r3, #0
	bne	.L81
	add	r3, r5, r4, lsl #4
	ldr	r3, [r3, #2144]
	cmp	r3, #0
	bne	.L81
	bl	mcu_get_wrap_status
	cmp	r0, #2
	bne	.L81
	movw	r3, #43232
	ldr	r6, [r5, r3]
	cbnz	r6, .L81
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r9, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r10, .L114+4
	add	r7, r5, #404
	ldr	fp, .L114+16
	mov	r8, r5
.L85:
	ldr	r3, [r7], #4
	cbz	r3, .L83
	ldr	r3, [r7, #764]
	adds	r3, r3, #1
	bne	.L83
	movs	r1, #4
	movw	r3, #958
	mov	r2, fp
	str	r3, [sp]
	mov	r0, r1
	mov	r3, r10
	str	r6, [sp, #4]
	bl	vlog
	add	r3, r8, #1440
	add	r1, r8, #1312
	mov	r0, r6
	ldrd	r2, [r3]
	strd	r2, [sp]
	ldrd	r2, [r1]
	bl	mpp_vcodec_run_task
	cmp	r0, #0
	bne	.L84
	movs	r3, #1
.L112:
	str	r3, [r7, #764]
.L83:
	adds	r6, r6, #1
	add	r8, r8, #8
	cmp	r6, #16
	bne	.L85
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r9	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L81:
	ldr	r3, [sp, #72]
	cmp	r3, #0
	beq	.L113
	ldr	r6, .L114+8
	add	r3, r4, #8960
	adds	r3, r3, #40
	ldr	r3, [r6, r3, lsl #2]
	cmp	r3, #9
	beq	.L86
	cmp	r3, #15
	bne	.L87
.L86:
	add	r2, r6, r4, lsl #4
	ldr	r3, [r2, #2152]
	cmp	r3, #0
	itttt	gt
	addgt	r3, r3, #-1
	strgt	r3, [r2, #2152]
	movgt	r3, #0
	strgt	r3, [r2, #2156]
.L87:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	adds	r2, r6, r4
	movw	r3, #10320
	ldrb	r3, [r2, r3]	@ zero_extendqisi2
	add	r3, r3, r6
	add	r3, r3, #10304
	ldrb	r3, [r3]	@ zero_extendqisi2
	cbz	r3, .L88
	addw	r7, r4, #2584
	ldr	r3, [r6, r7, lsl #2]
	cmp	r3, #0
	ble	.L88
	mov	r3, #800
	movs	r2, #0
	mla	r3, r3, r4, r6
	add	r3, r3, #23168
	adds	r3, r3, #32
.L91:
	ldrd	r0, [r3], #8
	ldrd	r9, [sp, #64]
	cmp	r1, r10
	it	eq
	cmpeq	r0, r9
	beq	.L89
	orrs	r1, r0, r1
	bne	.L90
	movs	r3, #100
	mla	r4, r3, r4, r2
	ldrd	r1, [sp, #16]
	addw	r3, r4, #1300
	addw	r4, r4, #2900
	add	r3, r5, r3, lsl #3
	add	r5, r5, r4, lsl #3
	mov	r4, r10
	strd	r1, [r3]
	mov	r3, r9
	strd	r3, [r5]
.L89:
	ldr	r3, [r6, r7, lsl #2]
	subs	r3, r3, #1
	str	r3, [r6, r7, lsl #2]
.L88:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L113:
	movs	r0, #0
	b	.L76
.L84:
	movs	r3, #0
	b	.L112
.L90:
	adds	r2, r2, #1
	cmp	r2, #100
	bne	.L91
	mov	r3, #992
	str	r2, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L114+4
	movs	r0, #4
	ldr	r2, .L114+12
	bl	vlog
	b	.L89
.L115:
	.align	2
.L114:
	.word	.LC1
	.word	.LANCHOR0+84
	.word	g_venc_dev
	.word	.LC4
	.word	.LC3
	.fnend
	.size	vcodec_set_intra_info, .-vcodec_set_intra_info
	.align	1
	.global	venc_node_set_framerate
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_set_framerate, %function
venc_node_set_framerate:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r4, r1
	ldr	r3, [r1, #4]	@ unaligned
	cmp	r3, #16
	beq	.L117
	movw	r3, #1837
	ldr	r2, .L123
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L123+4
	movs	r0, #4
	bl	vlog
.L122:
	mov	r0, #-1
.L116:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L117:
	ldr	r6, [r1]	@ unaligned
	cmp	r6, #15
	ble	.L119
	strd	r6, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1842
	ldr	r2, .L123+8
	str	r3, [sp]
	movs	r0, #4
	ldr	r3, .L123+4
	bl	vlog
	b	.L122
.L119:
	ldr	r5, .L123+12
	ldr	r3, [r1, #8]	@ unaligned
	add	r2, r5, r6, lsl #2
	ldr	r2, [r2, #404]
	cbnz	r2, .L120
	ldr	r2, [r1, #12]	@ unaligned
	cmp	r2, r3
	bgt	.L120
.L121:
	movs	r0, #0
	b	.L116
.L120:
	add	r2, r5, r6, lsl #2
	add	r1, r5, r6, lsl #2
	str	r3, [r2, #212]
	ldr	r2, [r4, #12]	@ unaligned
	str	r2, [r1, #276]
	movs	r1, #4
	strd	r3, r2, [sp, #8]
	mov	r0, r1
	mov	r3, #1848
	ldr	r2, .L123+16
	str	r3, [sp]
	str	r6, [sp, #4]
	ldr	r3, .L123+4
	bl	vlog
	ldr	r2, [r4, #8]	@ unaligned
	ldr	r3, [r4, #12]	@ unaligned
	cmp	r2, r3
	bge	.L121
	mov	r0, r6
	bl	get_venc_node
	cmp	r0, #0
	bne	.L121
	ldr	r3, [r4, #12]	@ unaligned
	add	r5, r5, r6, lsl #2
	ldr	r2, [r4, #8]	@ unaligned
	adds	r3, r3, #1
	subs	r3, r3, r2
	str	r3, [r5, #340]
	b	.L116
.L124:
	.align	2
.L123:
	.word	.LC5
	.word	.LANCHOR0+106
	.word	.LC1
	.word	g_venc_dev
	.word	.LC6
	.fnend
	.size	venc_node_set_framerate, .-venc_node_set_framerate
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_pp_node, %function
get_pp_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, r0
	movs	r1, #0
	movs	r0, #20
	b	vsys_get_node_from_map
	.fnend
	.size	get_pp_node, .-get_pp_node
	.align	1
	.global	vcodec_destory_dev
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_destory_dev, %function
vcodec_destory_dev:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbz	r0, .L128
	bl	mpi_dev_unregister
	mov	r0, r4
	bl	kfree
	movs	r0, #0
.L126:
	pop	{r4, pc}
.L128:
	mov	r0, #-1
	b	.L126
	.fnend
	.size	vcodec_destory_dev, .-vcodec_destory_dev
	.align	1
	.global	mpi_buf_alloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_alloc, %function
mpi_buf_alloc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r0, .L135
	bl	mpi_allocator_find_by_name
	mov	r2, r4
	movs	r1, #1
	bl	mpi_allocator_alloc
	mov	r5, r0
	cbz	r0, .L131
	movs	r1, #0
	bl	mpi_buf_create
	mov	r4, r0
	cbz	r0, .L129
	ldr	r3, .L135+4
	str	r3, [r0, #4]
	mov	r0, r5
	bl	dma_buf_put
.L129:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L131:
	mov	r4, r0
	b	.L129
.L136:
	.align	2
.L135:
	.word	.LC7
	.word	.LC8
	.fnend
	.size	mpi_buf_alloc, .-mpi_buf_alloc
	.align	1
	.global	mpi_buf_unref
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_unref, %function
mpi_buf_unref:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r0
	cmp	r0, #0
	beq	.L137
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, .L149
	movs	r2, #0
	movs	r3, #12
.L142:
	mul	r1, r3, r2
	ldr	r6, [r1, r0]
	cmp	r6, r4
	bne	.L139
	ldr	r3, .L149+4
	add	r3, r3, r1
	movw	r1, #37028
	ldr	r2, [r3, r1]
	subs	r2, r2, #1
	str	r2, [r3, r1]
	cbnz	r2, .L140
	movw	r1, #37032
	movw	r6, #37024
	ldr	r0, [r3, r1]
	str	r2, [r3, r6]
	mov	r2, #-1
	str	r2, [r3, r1]
.L141:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r0, #0
	blt	.L143
	movs	r2, #0
	mov	r1, r4
	bl	remove_user_from_buf
.L143:
	ldr	r3, [r4, #444]
	cbz	r3, .L144
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L149+8
	movs	r0, #4
	str	r3, [sp, #4]
	movw	r3, #1167
	str	r3, [sp]
	ldr	r3, .L149+12
	bl	vlog
.L144:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	mpi_obj_unref
.L139:
	adds	r2, r2, #1
	cmp	r2, #512
	bne	.L142
.L140:
	mov	r0, #-1
	b	.L141
.L137:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L150:
	.align	2
.L149:
	.word	g_venc_dev+37024
	.word	g_venc_dev
	.word	.LC9
	.word	.LANCHOR0+130
	.fnend
	.size	mpi_buf_unref, .-mpi_buf_unref
	.align	1
	.global	mpi_buffer_queue_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buffer_queue_create, %function
mpi_buffer_queue_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L152
	movs	r1, #4
	b	mpi_queue_create
.L153:
	.align	2
.L152:
	.word	mpi_obj_unref
	.fnend
	.size	mpi_buffer_queue_create, .-mpi_buffer_queue_create
	.align	1
	.global	mpi_buffer_queue_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buffer_queue_destroy, %function
mpi_buffer_queue_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	movs	r1, #0
	bl	mpi_queue_destroy
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	mpi_buffer_queue_destroy, .-mpi_buffer_queue_destroy
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_destroy_ctx, %function
venc_destroy_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r0, [r0]
	cbz	r0, .L156
	movs	r1, #0
	bl	mpi_queue_destroy
.L156:
	mov	r0, r4
	bl	kfree
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	venc_destroy_ctx, .-venc_destroy_ctx
	.align	1
	.global	mpi_buffer_pool_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buffer_pool_create, %function
mpi_buffer_pool_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r0
	ldr	r0, .L165
	mov	r6, r1
	bl	mpi_allocator_find_by_name
	mov	r4, r0
	cbz	r0, .L160
	ldr	r3, .L165+4
	mov	r2, r5
	movs	r1, #1
	str	r3, [sp]
	mov	r3, r6
	bl	mpi_buf_pool_create
	mov	r4, r0
.L160:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L166:
	.align	2
.L165:
	.word	.LC7
	.word	.LC8
	.fnend
	.size	mpi_buffer_pool_create, .-mpi_buffer_pool_create
	.align	1
	.global	mpi_buffer_pool_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buffer_pool_destroy, %function
mpi_buffer_pool_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #0
	b	mpi_buf_pool_destroy
	.fnend
	.size	mpi_buffer_pool_destroy, .-mpi_buffer_pool_destroy
	.align	1
	.global	mpi_buffer_pool_request_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buffer_pool_request_buf, %function
mpi_buffer_pool_request_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #0
	b	mpi_buf_pool_request_buf
	.fnend
	.size	mpi_buffer_pool_request_buf, .-mpi_buffer_pool_request_buf
	.align	1
	.global	mpi_buffer_queue_pop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buffer_queue_pop, %function
mpi_buffer_queue_pop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	mpi_queue_pop_obj
	.fnend
	.size	mpi_buffer_queue_pop, .-mpi_buffer_queue_pop
	.align	1
	.global	venc_node_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_create, %function
venc_node_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r4, r0
	ldr	r0, .L179
	mov	r5, r1
	bl	mpi_dev_find_by_name
	ldr	r2, [r5, #8]	@ unaligned
	ldr	r6, [r4, #128]
	cmp	r2, #16
	mov	r8, r0
	beq	.L171
	movw	r3, #1561
	ldr	r2, .L179+4
	str	r3, [sp]
	ldr	r3, .L179+8
.L178:
	movs	r1, #2
	movs	r0, #4
	bl	vlog
.L177:
	mov	r0, #-1
.L170:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L171:
	ldr	r3, [r5, #4]	@ unaligned
	cmp	r3, #15
	ble	.L173
	strd	r3, r2, [sp, #4]
	movs	r1, #2
	movw	r3, #1565
	ldr	r2, .L179+12
	str	r3, [sp]
	movs	r0, #4
	ldr	r3, .L179+8
	bl	vlog
	b	.L177
.L173:
	ldr	r7, .L179+16
	add	r3, r3, #10752
	adds	r3, r3, #40
	ldr	r0, .L179+20
	mov	r4, #-1
	str	r4, [r7, r3, lsl #2]
	bl	mpi_allocator_find_by_name
	cbnz	r0, .L174
	movw	r3, #1572
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L179+8
	movs	r0, #4
	ldr	r2, .L179+24
	bl	vlog
	b	.L177
.L174:
	mov	r0, r8
	movs	r3, #0
	movs	r2, #1
	ldr	r1, [r5, #4]	@ unaligned
	bl	mpi_dev_create_node_with_params
	mov	r8, r0
	cbnz	r0, .L175
	movw	r3, #1578
	ldr	r2, .L179+28
	str	r3, [sp]
	ldr	r3, .L179+8
	b	.L178
.L175:
	ldr	r3, .L179+32
	movs	r1, #1
	str	r3, [r0, #4]
	bl	mpi_node_enable_auto_streamon
	mov	r3, r8
	ldr	r2, [r5, #4]	@ unaligned
	movs	r0, #4
	ldr	r1, [r5]	@ unaligned
	bl	vsys_add_node_to_map
	ldr	r3, [r5, #4]	@ unaligned
	ldr	r2, [r5, #12]	@ unaligned
	movs	r0, #0
	add	r3, r3, #8960
	adds	r3, r3, #40
	str	r2, [r7, r3, lsl #2]
	movs	r3, #4
	str	r3, [r6]
	ldr	r3, [r5]	@ unaligned
	str	r3, [r6, #4]
	ldr	r3, [r5, #4]	@ unaligned
	str	r3, [r6, #8]
	b	.L170
.L180:
	.align	2
.L179:
	.word	.LC10
	.word	.LC11
	.word	.LANCHOR0+144
	.word	.LC1
	.word	g_venc_dev
	.word	.LC7
	.word	.LC12
	.word	.LC13
	.word	.LC8
	.fnend
	.size	venc_node_create, .-venc_node_create
	.align	1
	.global	venc_node_delete
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_delete, %function
venc_node_delete:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	ldr	r5, [r1]
	cmp	r5, #15
	ble	.L182
	movs	r3, #16
	ldr	r2, .L208
	strd	r5, r3, [sp, #4]
	movs	r1, #2
	mov	r3, #1600
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L208+4
	bl	vlog
	mov	r0, #-1
.L181:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L182:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r4, .L208+8
	add	r7, r5, #100
	add	r3, r4, r7, lsl #2
	ldr	r3, [r3, #4]
	cbz	r3, .L184
	add	r6, r5, #392
	ldr	r0, [r4, r6, lsl #2]
	cbz	r0, .L184
	bl	mpi_buf_unref
	add	r2, r5, #312
	movs	r3, #0
	str	r3, [r4, r6, lsl #2]
	str	r3, [r4, r2, lsl #2]
.L184:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r5
	bl	get_venc_node
	mov	r6, r0
	cbz	r0, .L185
	mov	r2, r5
	movs	r1, #0
	movs	r0, #4
	bl	vsys_remove_node_from_map
	mov	r0, r6
	bl	mpi_node_destroy
.L185:
	adds	r3, r4, r5
	add	r10, r3, #10304
	ldrb	r2, [r10]	@ zero_extendqisi2
	cbz	r2, .L186
	mov	r9, #800
	movw	r2, #10320
	mul	r6, r9, r5
	mov	r8, #0
	movs	r1, #255
	strb	r1, [r3, r2]
	addw	r3, r5, #2584
	mov	r2, r9
	add	r0, r6, #10368
	mov	r1, r8
	adds	r0, r0, #32
	str	r8, [r4, r3, lsl #2]
	add	r0, r0, r4
	bl	memset
	add	r0, r6, #23168
	adds	r0, r0, #32
	mov	r2, r9
	mov	r1, r8
	add	r0, r0, r4
	bl	memset
	strb	r8, [r10]
.L186:
	add	r7, r4, r7, lsl #2
	ldr	r3, [r7, #4]
	cbz	r3, .L187
	bl	mcu_wrap_end
	movs	r3, #0
	str	r3, [r7, #4]
.L187:
	add	r2, r5, #52
	movs	r3, #0
	add	r2, r4, r2, lsl #2
	str	r3, [r2, #4]
	add	r2, r5, #68
	adds	r5, r5, #84
	add	r2, r4, r2, lsl #2
	add	r5, r4, r5, lsl #2
	mov	r4, r3
	str	r3, [r2, #4]
	str	r3, [r5, #4]
.L188:
	mov	r0, r4
	bl	get_venc_node
	mov	r5, r0
	cbnz	r0, .L190
	adds	r4, r4, #1
	cmp	r4, #16
	bne	.L188
	bl	mpp_vcodec_clear_buf_resource
	mov	r0, r5
	b	.L181
.L190:
	movs	r0, #0
	b	.L181
.L209:
	.align	2
.L208:
	.word	.LC1
	.word	.LANCHOR0+161
	.word	g_venc_dev
	.fnend
	.size	venc_node_delete, .-venc_node_delete
	.align	1
	.global	venc_node_pp_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_pp_start, %function
venc_node_pp_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	ldr	r4, [r1]
	cmp	r4, #7
	ble	.L211
	movs	r3, #8
	ldr	r2, .L214
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1987
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L214+4
	bl	vlog
	mov	r0, #-1
.L210:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L211:
	mov	r0, r4
	bl	get_pp_node
	cmp	r0, #0
	beq	.L210
	mov	r0, r4
	bl	get_pp_node
	bl	mpi_node_streamon
	movs	r0, #0
	b	.L210
.L215:
	.align	2
.L214:
	.word	.LC1
	.word	.LANCHOR0+178
	.fnend
	.size	venc_node_pp_start, .-venc_node_pp_start
	.align	1
	.global	venc_node_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_start, %function
venc_node_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r1
	ldr	r3, [r1, #20]	@ unaligned
	.pad #28
	sub	sp, sp, #28
	ldr	r7, [r1]	@ unaligned
	cmp	r3, #36
	beq	.L217
	movw	r2, #1646
	ldr	r1, .L257
	ldr	r0, .L257+4
	bl	printk
.L256:
	mov	r0, #-1
.L216:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L217:
	cmp	r7, #15
	ble	.L219
	movs	r3, #16
	ldr	r2, .L257+8
	strd	r7, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1651
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L257
	bl	vlog
	b	.L256
.L219:
	ldr	r3, [r1, #8]	@ unaligned
	add	r6, r7, #100
	ldr	r5, .L257+12
	movs	r0, #4
	ldr	r2, .L257+16
	str	r3, [sp, #12]
	ldr	r3, [r1, #4]	@ unaligned
	movs	r1, #5
	strd	r7, r3, [sp, #4]
	movw	r3, #1654
	str	r3, [sp]
	ldr	r3, .L257
	bl	vlog
	add	r3, r5, r6, lsl #2
	ldr	r3, [r3, #4]
	cbnz	r3, .L220
	ldr	r3, [r4, #4]	@ unaligned
	cbz	r3, .L221
.L220:
	add	r2, r7, #260
	movs	r3, #0
	add	r2, r5, r2, lsl #2
	str	r3, [r2, #4]
	add	r2, r7, #276
	add	r2, r5, r2, lsl #2
	str	r3, [r2, #4]
	add	r2, r7, #312
	str	r3, [r5, r2, lsl #2]
.L221:
	ldr	r3, [r4, #4]	@ unaligned
	cbz	r3, .L222
	add	r2, r5, r6, lsl #2
	str	r3, [r2, #4]
.L222:
	mov	r0, r7
	bl	get_venc_node
	cbz	r0, .L223
	mov	r0, r7
	bl	get_venc_node
	bl	mpi_node_streamon
.L223:
	ldr	r3, [r4, #4]	@ unaligned
	cbnz	r3, .L224
.L255:
	movs	r0, #0
	b	.L216
.L224:
	add	r3, r7, #8960
	adds	r3, r3, #40
	ldr	r3, [r5, r3, lsl #2]
	bic	r3, r3, #4
	cmp	r3, #8
	bne	.L255
	movs	r3, #36
	add	lr, r4, #32
	mla	r3, r3, r7, r5
	mov	r2, r4
	add	r3, r3, #468
.L226:
	ldr	r0, [r2]	@ unaligned
	adds	r2, r2, #8
	ldr	r1, [r2, #-4]	@ unaligned
	cmp	r2, lr
	mov	ip, r3
	stmia	ip!, {r0, r1}
	mov	r3, ip
	bne	.L226
	ldr	r0, [r2]	@ unaligned
	add	r6, r5, r6, lsl #2
	str	r0, [ip]
	ldr	r3, [r6, #4]
	cmp	r3, #0
	beq	.L255
	ldr	r0, .L257+20
	bl	mpi_dev_find_by_name
	mov	r9, r0
	mov	r0, r7
	bl	get_venc_node
	movs	r1, #0
	mov	r8, r0
	mov	r0, r1
	bl	mpi_buf_create
	mov	r6, r0
	cmp	r0, #0
	beq	.L255
	cmp	r9, #0
	beq	.L255
	cmp	r8, #0
	beq	.L255
	ldr	r3, .L257+24
	str	r3, [r0, #4]
	ldr	r3, [r4, #12]	@ unaligned
	str	r3, [r0, #620]
	ldr	r2, [r4, #16]	@ unaligned
	str	r2, [r0, #624]
	ldr	ip, [r4, #24]	@ unaligned
	str	ip, [r0, #628]
	ldr	lr, [r4, #28]	@ unaligned
	str	lr, [r0, #632]
	ldr	r1, [r4, #32]	@ unaligned
	str	r1, [r0, #640]
	movs	r0, #0
	movs	r1, #0
	strd	r0, [r6, #672]
	movs	r1, #0
	str	r1, [r6, #668]
	movs	r1, #4
	strd	r3, r2, [sp, #4]
	strd	ip, lr, [sp, #12]
	movw	r3, #690
	ldr	r2, .L257+28
	mov	r0, r1
	str	r3, [sp]
	ldr	r3, .L257+32
	bl	vlog
	ldr	r3, [r8, #288]
	ldr	r4, [r3]
	cbnz	r4, .L227
	movw	r3, #693
	ldr	r2, .L257+36
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L257+32
	movs	r0, #4
	bl	vlog
	b	.L255
.L227:
	mov	r0, r4
	bl	mpi_queue_pop_obj
	cbz	r0, .L228
	bl	mpi_buf_unref
.L228:
	add	r5, r5, r7, lsl #2
	mov	r1, r6
	mov	r0, r4
	movs	r3, #1
	str	r3, [r5, #1172]
	bl	mpi_queue_push_obj
	cbnz	r0, .L229
	movw	r3, #701
	ldr	r2, .L257+40
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L257+32
	movs	r0, #4
	bl	vlog
	mov	r0, r6
	bl	mpi_obj_unref
.L229:
	ldr	r3, [r9, #80]
	movs	r2, #0
	movs	r1, #7
	mov	r0, r8
	blx	r3
	b	.L255
.L258:
	.align	2
.L257:
	.word	.LANCHOR0+197
	.word	.LC14
	.word	.LC1
	.word	g_venc_dev
	.word	.LC15
	.word	.LC10
	.word	.LC8
	.word	.LC16
	.word	.LANCHOR0+213
	.word	.LC17
	.word	.LC18
	.fnend
	.size	venc_node_start, .-venc_node_start
	.align	1
	.global	venc_node_set_vcodec_attr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_set_vcodec_attr, %function
venc_node_set_vcodec_attr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r2, [r1, #8]
	cmp	r2, #15
	bls	.L260
	movs	r3, #16
	movs	r1, #2
	strd	r2, r3, [sp, #4]
	movs	r0, #4
	mov	r3, #1768
	ldr	r2, .L264
	str	r3, [sp]
	ldr	r3, .L264+4
	bl	vlog
	mov	r0, #-1
.L259:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L260:
	movs	r3, #60
	movw	r0, #36064
	mla	r0, r3, r2, r0
	ldr	r2, .L264+8
	add	r5, r1, #56
	mov	r3, r1
	add	r2, r2, r0
.L262:
	ldr	r0, [r3]	@ unaligned
	adds	r3, r3, #8
	ldr	r1, [r3, #-4]	@ unaligned
	cmp	r3, r5
	mov	r4, r2
	stmia	r4!, {r0, r1}
	mov	r2, r4
	bne	.L262
	ldr	r0, [r3]	@ unaligned
	str	r0, [r4]
	movs	r0, #0
	b	.L259
.L265:
	.align	2
.L264:
	.word	.LC1
	.word	.LANCHOR0+228
	.word	g_venc_dev
	.fnend
	.size	venc_node_set_vcodec_attr, .-venc_node_set_vcodec_attr
	.align	1
	.global	venc_node_get_vcodec_attr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_get_vcodec_attr, %function
venc_node_get_vcodec_attr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r3, [r1, #8]
	cmp	r3, #15
	bls	.L267
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1780
	ldr	r2, .L271
	str	r3, [sp]
	ldr	r3, .L271+4
	bl	vlog
	mov	r0, #-1
.L266:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L267:
	movw	r2, #36064
	movs	r0, #60
	mla	r3, r0, r3, r2
	ldr	r2, .L271+8
	add	r3, r3, r2
	mov	r2, r1
	add	r5, r3, #56
.L269:
	mov	r4, r3
	adds	r2, r2, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, r5
	str	r0, [r2, #-8]	@ unaligned
	mov	r3, r4
	str	r1, [r2, #-4]	@ unaligned
	bne	.L269
	ldr	r0, [r4]
	str	r0, [r2]	@ unaligned
	movs	r0, #0
	b	.L266
.L272:
	.align	2
.L271:
	.word	.LC1
	.word	.LANCHOR0+254
	.word	g_venc_dev
	.fnend
	.size	venc_node_get_vcodec_attr, .-venc_node_get_vcodec_attr
	.align	1
	.global	venc_node_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_stop, %function
venc_node_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	ldr	r4, [r1]
	cmp	r4, #15
	ble	.L274
	movs	r3, #16
	ldr	r2, .L280
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1679
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L280+4
	bl	vlog
	mov	r0, #-1
.L273:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L274:
	mov	r0, r4
	bl	get_venc_node
	cbz	r0, .L276
	mov	r0, r4
	bl	get_venc_node
	bl	mpi_node_streamoff
.L276:
	ldr	r3, .L280+8
	movs	r0, #0
	add	r4, r3, r4, lsl #4
	str	r0, [r4, #2152]
	b	.L273
.L281:
	.align	2
.L280:
	.word	.LC1
	.word	.LANCHOR0+280
	.word	g_venc_dev
	.fnend
	.size	venc_node_stop, .-venc_node_stop
	.align	1
	.global	venc_node_send_frame
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_send_frame, %function
venc_node_send_frame:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #24
	sub	sp, sp, #24
	ldr	r0, .L296
	mov	r4, r1
	bl	mpi_dev_find_by_name
	ldr	r3, [r4, #8]	@ unaligned
	cmp	r3, #92
	mov	r8, r0
	beq	.L283
	movw	r3, #1698
	ldr	r2, .L296+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L296+8
	movs	r0, #4
	bl	vlog
.L295:
	mov	r0, #-1
.L282:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L283:
	ldr	r6, [r4]	@ unaligned
	cmp	r6, #15
	ble	.L285
	movs	r3, #16
	ldr	r2, .L296+12
	strd	r6, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1703
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L296+8
	bl	vlog
	b	.L295
.L285:
	mov	r0, r6
	bl	get_venc_node
	mov	r7, r0
	cbnz	r0, .L286
.L287:
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #2
	strd	r6, r7, [sp, #4]
	movs	r0, #4
	ldr	r2, .L296+16
	strd	r8, r3, [sp, #12]
	movw	r3, #1722
	str	r3, [sp]
	ldr	r3, .L296+8
	bl	vlog
	b	.L295
.L286:
	ldr	r0, [r4, #4]	@ unaligned
	cmp	r8, #0
	beq	.L287
	cmp	r0, #0
	blt	.L287
	bl	get_mpi_buf
	mov	r5, r0
	cmp	r0, #0
	beq	.L287
	movs	r2, #80
	add	r1, r4, #12
	add	r0, r0, #616
	bl	memcpy
	movs	r3, #4
	str	r3, [r5, #584]
	movs	r2, #0
	ldr	r3, [r4, #84]	@ unaligned
	mov	r1, r5
	mov	r0, r7
	cmp	r3, #0
	itt	lt
	movlt	r3, #12
	strlt	r3, [r5, #584]
	bl	mpi_node_buffer_in
	cmp	r0, #0
	beq	.L282
	b	.L287
.L297:
	.align	2
.L296:
	.word	.LC10
	.word	.LC19
	.word	.LANCHOR0+295
	.word	.LC1
	.word	.LC20
	.fnend
	.size	venc_node_send_frame, .-venc_node_send_frame
	.align	1
	.global	venc_node_pp_send_frame
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_pp_send_frame, %function
venc_node_pp_send_frame:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #24
	sub	sp, sp, #24
	ldr	r0, .L313
	mov	r4, r1
	bl	mpi_dev_find_by_name
	ldr	r3, [r4, #8]	@ unaligned
	cmp	r3, #92
	mov	r8, r0
	beq	.L299
	movw	r3, #2004
	ldr	r2, .L313+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L313+8
	movs	r0, #4
	bl	vlog
.L312:
	mov	r0, #-1
	b	.L298
.L299:
	ldr	r5, [r4]	@ unaligned
	cmp	r5, #7
	ble	.L301
	movs	r3, #8
	ldr	r2, .L313+12
	strd	r5, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #2009
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L313+8
	bl	vlog
	b	.L312
.L301:
	mov	r0, r5
	bl	get_pp_node
	mov	r6, r0
	cbz	r0, .L302
	ldr	r0, [r4, #4]	@ unaligned
	cmp	r8, #0
	beq	.L302
	cmp	r0, #0
	blt	.L302
	bl	get_mpi_buf
	mov	r7, r0
	cbz	r0, .L302
	movs	r2, #80
	add	r1, r4, #12
	add	r0, r0, #616
	bl	memcpy
	movs	r2, #1
	mov	r1, r7
	mov	r0, r6
	bl	mpi_node_buffer_in
	movs	r0, #0
.L298:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L302:
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #2
	strd	r5, r6, [sp, #4]
	movs	r0, #4
	ldr	r2, .L313+16
	strd	r8, r3, [sp, #12]
	movw	r3, #2025
	str	r3, [sp]
	ldr	r3, .L313+8
	bl	vlog
	b	.L312
.L314:
	.align	2
.L313:
	.word	.LC8
	.word	.LC19
	.word	.LANCHOR0+316
	.word	.LC1
	.word	.LC21
	.fnend
	.size	venc_node_pp_send_frame, .-venc_node_pp_send_frame
	.align	1
	.global	venc_node_pp_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_pp_create, %function
venc_node_pp_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r6, r1
	ldr	r3, [r1, #8]	@ unaligned
	.pad #84
	sub	sp, sp, #84
	ldr	r10, [r0, #128]
	cmp	r3, #68
	ldr	r5, [r1, #4]	@ unaligned
	beq	.L316
	mov	r3, #1872
	ldr	r2, .L338
	str	r3, [sp]
	ldr	r3, .L338+4
.L335:
	movs	r1, #2
	movs	r0, #4
	bl	vlog
	b	.L336
.L316:
	cmp	r5, #7
	ble	.L318
	movs	r3, #8
	ldr	r2, .L338+8
	strd	r5, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1876
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L338+4
	bl	vlog
.L336:
	mov	r7, #-1
.L315:
	mov	r0, r7
	add	sp, sp, #84
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L318:
	mov	r0, r5
	bl	get_pp_node
	mov	r4, r0
	cbz	r0, .L319
	ldr	r3, [r6, #4]	@ unaligned
	movs	r1, #2
	ldr	r2, .L338+12
	movs	r0, #4
	str	r3, [sp, #4]
	mov	r3, #1880
	str	r3, [sp]
	ldr	r3, .L338+4
	bl	vlog
	b	.L336
.L319:
	mov	r8, #988
	ldr	r7, .L338+16
	mul	r8, r8, r5
	ldr	r2, .L338+20
	ldr	r1, .L338+24
	mov	r9, r7
	addw	r0, r8, #3380
	addw	fp, r8, #2580
	add	r0, r0, r7
	add	r5, r7, r8
	bl	__init_waitqueue_head
	ldr	r3, [r6, #16]	@ unaligned
	add	fp, fp, r7
	sbfx	r3, r3, #1, #29
	adds	r3, r3, #3
	asrs	r3, r3, #2
	lsls	r3, r3, #6
	str	r3, [r5, #2468]
.L322:
	ldr	r0, [r5, #2468]
	bl	mpi_buf_alloc
	str	r0, [fp], #4
	cbnz	r0, .L320
	movw	r3, #1893
.L337:
	str	r3, [sp]
	ldr	r2, .L338+28
	ldr	r3, .L338+4
	b	.L335
.L320:
	ldr	r0, [r0, #604]
	cbnz	r0, .L321
	movw	r3, #1897
	b	.L337
.L321:
	ldr	r2, [r5, #2468]
	movs	r1, #0
	adds	r4, r4, #1
	bl	memset
	cmp	r4, #5
	bne	.L322
	ldr	r3, [r6, #28]	@ unaligned
	cbz	r3, .L323
	ldr	r4, [r6, #12]	@ unaligned
	sbfx	r4, r4, #1, #29
	adds	r4, r4, #3
	asrs	r4, r4, #2
	adds	r4, r4, #7
	asrs	r3, r4, #3
	ldr	r4, [r6, #16]	@ unaligned
	sbfx	r4, r4, #1, #29
	adds	r4, r4, #15
	asrs	r4, r4, #4
	muls	r4, r3, r4
	lsls	r4, r4, #6
	mov	r0, r4
	bl	mpi_buf_alloc
	str	r0, [r5, #3124]
	cbnz	r0, .L324
.L325:
	movw	r3, #1909
	b	.L337
.L324:
	mov	r0, r4
	bl	mpi_buf_alloc
	str	r0, [r5, #3132]
	cmp	r0, #0
	beq	.L325
.L323:
	ldr	r3, [r6, #12]	@ unaligned
	add	r1, sp, #20
	ldr	r0, [r6, #4]	@ unaligned
	movs	r4, #1
	str	r4, [sp, #44]
	str	r3, [sp, #20]
	ldr	r3, [r6, #16]	@ unaligned
	str	r3, [sp, #24]
	ldr	r3, [r6, #20]	@ unaligned
	str	r3, [sp, #48]
	ldr	r3, [r6, #24]	@ unaligned
	str	r3, [sp, #52]
	ldr	r3, [r6, #36]	@ unaligned
	str	r3, [sp, #36]
	ldr	r3, [r6, #40]	@ unaligned
	str	r3, [sp, #40]
	ldr	r3, [r6, #28]	@ unaligned
	str	r3, [sp, #28]
	ldr	r3, [r6, #32]	@ unaligned
	str	r3, [sp, #32]
	bl	vepu_pp_create_chn
	mov	r7, r0
	cbz	r0, .L326
	movw	r3, #1926
	b	.L337
.L326:
	mov	r3, r0
	mov	r2, r4
	ldr	r1, [r6, #4]	@ unaligned
	ldr	r0, .L338+32
	bl	mpi_dev_create_node_with_params
	mov	fp, r0
	cbnz	r0, .L327
	movw	r3, #1932
	ldr	r2, .L338+36
	str	r3, [sp]
	ldr	r3, .L338+4
	b	.L335
.L327:
	add	r4, sp, #20
	ldr	r5, .L338+40
	ldmia	r4!, {r0, r1, r2, r3}
	add	r5, r5, r8
	stmia	r5!, {r0, r1, r2, r3}
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldm	r4, {r0, r1, r2}
	stm	r5, {r0, r1, r2}
	add	r0, r8, #2512
	movs	r2, #68
	mov	r1, r6
	add	r0, r0, r9
	bl	memcpy
	mov	r3, fp
	ldr	r2, [r6, #4]	@ unaligned
	movs	r0, #20
	ldr	r1, [r6]	@ unaligned
	bl	vsys_add_node_to_map
	movs	r3, #20
	str	r3, [r10]
	ldr	r3, [r6]	@ unaligned
	str	r3, [r10, #4]
	ldr	r3, [r6, #4]	@ unaligned
	str	r3, [r10, #8]
	b	.L315
.L339:
	.align	2
.L338:
	.word	.LC22
	.word	.LANCHOR0+340
	.word	.LC1
	.word	.LC23
	.word	g_venc_dev
	.word	.LANCHOR1
	.word	.LC24
	.word	.LC25
	.word	g_venc_dev+4
	.word	.LC26
	.word	g_venc_dev+2400
	.fnend
	.size	venc_node_pp_create, .-venc_node_pp_create
	.align	1
	.global	venc_node_pp_delete
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_pp_delete, %function
venc_node_pp_delete:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r4, [r1]
	cmp	r4, #7
	ble	.L341
	movs	r3, #8
	ldr	r2, .L361
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1955
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L361+4
	bl	vlog
	mov	r0, #-1
.L340:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L341:
	mov	r5, #988
	ldr	r6, .L361+8
	muls	r5, r4, r5
	mov	r8, #0
	mov	r9, r8
	addw	r7, r5, #2580
	add	r7, r7, r6
.L344:
	ldr	r0, [r7], #4
	cbz	r0, .L343
	bl	mpi_obj_unref
	str	r9, [r7, #-4]
.L343:
	add	r8, r8, #1
	cmp	r8, #5
	bne	.L344
	mov	r7, #988
	mla	r7, r7, r4, r6
	ldr	r0, [r7, #3124]
	cbz	r0, .L345
	bl	mpi_obj_unref
	movs	r3, #0
	str	r3, [r7, #3124]
.L345:
	mov	r7, #988
	mla	r7, r7, r4, r6
	ldr	r0, [r7, #3132]
	cbz	r0, .L346
	bl	mpi_obj_unref
	movs	r3, #0
	str	r3, [r7, #3132]
.L346:
	mov	r0, r4
	bl	vepu_pp_destroy_chn
	mov	r0, r4
	bl	get_pp_node
	mov	r7, r0
	cbz	r0, .L347
	mov	r2, r4
	movs	r1, #0
	movs	r0, #20
	bl	vsys_remove_node_from_map
	mov	r0, r7
	bl	mpi_node_destroy
.L347:
	addw	r0, r5, #3260
	movs	r2, #36
	movs	r1, #0
	add	r0, r0, r6
	bl	memset
	movs	r0, #0
	b	.L340
.L362:
	.align	2
.L361:
	.word	.LC1
	.word	.LANCHOR0+360
	.word	g_venc_dev
	.fnend
	.size	venc_node_pp_delete, .-venc_node_pp_delete
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_release, %function
venc_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r7, r1
	ldr	r5, .L408
	ldr	r0, .L408+4
	bl	mpi_obj_unref
	ldr	r3, [r5, #20]
	cmp	r3, #1
	bne	.L364
	ldr	r2, [r5, #4]
	add	r5, r5, #404
	ldr	r1, .L408+8
	movs	r4, #16
	ldr	r0, .L408+12
	movs	r6, #0
	bl	printk
.L367:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5], #4
	cbz	r3, .L365
	ldr	r0, [r5, #1160]
	cbz	r0, .L365
	bl	mpi_buf_unref
	str	r6, [r5, #1160]
	str	r6, [r5, #840]
.L365:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #-4]
	cbz	r3, .L366
	bl	mcu_wrap_end
	str	r6, [r5, #-4]
.L366:
	subs	r4, r4, #1
	bne	.L367
	ldr	r6, .L408
	mov	r9, r4
.L368:
	addw	r8, r6, #2580
	movs	r5, #5
.L370:
	ldr	r0, [r8], #4
	cbz	r0, .L369
	bl	mpi_obj_unref
	str	r9, [r8, #-4]
.L369:
	subs	r5, r5, #1
	bne	.L370
	ldr	r0, [r6, #3124]
	cbz	r0, .L371
	bl	mpi_obj_unref
	str	r5, [r6, #3124]
.L371:
	ldr	r0, [r6, #3132]
	cbz	r0, .L372
	bl	mpi_obj_unref
	str	r9, [r6, #3132]
.L372:
	mov	r0, r4
	bl	get_pp_node
	mov	r5, r0
	cbz	r0, .L373
	mov	r0, r4
	bl	vepu_pp_destroy_chn
	mov	r2, r4
	movs	r1, #0
	movs	r0, #20
	bl	vsys_remove_node_from_map
	mov	r0, r5
	bl	mpi_node_destroy
.L373:
	adds	r4, r4, #1
	add	r6, r6, #988
	cmp	r4, #8
	bne	.L368
	movs	r4, #0
.L374:
	mov	r0, r4
	bl	get_venc_node
	mov	r5, r0
	cbz	r0, .L375
	mov	r2, r4
	movs	r1, #0
	movs	r0, #4
	bl	vsys_remove_node_from_map
	mov	r0, r5
	bl	mpi_node_destroy
.L375:
	adds	r4, r4, #1
	cmp	r4, #16
	bne	.L374
	mov	r2, #384
	movs	r1, #0
	ldr	r0, .L408+16
	bl	memset
	mov	r2, #256
	movs	r1, #0
	ldr	r0, .L408+20
	bl	memset
	mov	r2, #7904
	movs	r1, #0
	ldr	r0, .L408+24
	bl	memset
	mov	r2, r4
	mov	r1, #-1
	ldr	r0, .L408+28
	bl	memset
	movs	r2, #64
	movs	r1, #0
	ldr	r0, .L408+32
	bl	memset
	mov	r2, #12800
	movs	r1, #0
	ldr	r0, .L408+36
	bl	memset
	mov	r2, #12800
	movs	r1, #0
	ldr	r0, .L408+40
	bl	memset
	mov	r2, r4
	movs	r1, #0
	ldr	r0, .L408+44
	bl	memset
	bl	mpp_vcodec_clear_buf_resource
.L364:
	ldr	r0, [r7, #128]
	cbz	r0, .L376
	bl	kvfree
.L376:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L409:
	.align	2
.L408:
	.word	g_venc_dev
	.word	g_venc_dev+4
	.word	.LANCHOR0+380
	.word	.LC27
	.word	g_venc_dev+1760
	.word	g_venc_dev+2144
	.word	g_venc_dev+2400
	.word	g_venc_dev+10320
	.word	g_venc_dev+10336
	.word	g_venc_dev+10400
	.word	g_venc_dev+23200
	.word	g_venc_dev+10304
	.fnend
	.size	venc_release, .-venc_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_ioctl, %function
venc_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r3, r2
	ldr	r2, .L414
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	uxtb	r4, r1
	ldr	r6, [r2, #164]
	ldr	r5, [r6, #192]
	cmp	r5, r4
	bhi	.L411
	movw	r3, #2211
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L414+4
	movs	r0, #4
	ldr	r2, .L414+8
	bl	vlog
.L412:
	movw	r3, #2227
	ldr	r2, .L414+12
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L414+4
	movs	r0, #4
	bl	vlog
	mvn	r0, #21
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L411:
	.syntax unified
@ 87 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
	cmp	r4, r5
	sbc	r5, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r6, #188]
	ands	r4, r4, r5
	movs	r5, #20
	mla	r4, r5, r4, r2
	cbnz	r4, .L413
	movw	r3, #2218
	ldr	r2, .L414+16
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L414+4
	movs	r0, #4
	bl	vlog
	b	.L412
.L413:
	mov	r2, r1
	mov	r1, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	mpi_ioctl_kernel
.L415:
	.align	2
.L414:
	.word	g_venc_dev
	.word	.LANCHOR0+393
	.word	.LC28
	.word	.LC30
	.word	.LC29
	.fnend
	.size	venc_ioctl, .-venc_ioctl
	.global	__aeabi_uidivmod
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_handle_message, %function
venc_handle_message:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r3, [r0, #8]
	.pad #28
	sub	sp, sp, #28
	mov	r7, r1
	mov	r6, r2
	cmp	r3, #1
	beq	.L427
.L435:
	movs	r5, #0
.L416:
	mov	r0, r5
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L427:
	movs	r5, #0
.L417:
	mov	r0, r5
	bl	get_pp_node
	cmp	r4, r0
	beq	.L419
	adds	r5, r5, #1
	cmp	r5, #16
	bne	.L417
	mov	r5, #-1
.L420:
	movw	r3, #841
	str	r5, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L436
	movs	r0, #4
	ldr	r2, .L436+4
	bl	vlog
	b	.L435
.L419:
	cmp	r5, #7
	bgt	.L420
	cmp	r7, #1
	bne	.L435
	cmp	r6, #0
	beq	.L435
	ldr	r3, .L436+8
	mov	r2, #988
	mla	r5, r2, r5, r3
	ldr	r1, [r5, #2560]
	cmp	r1, #1
	ble	.L423
	ldr	r0, [r6, #668]
	bl	__aeabi_uidivmod
	cbz	r1, .L423
	movs	r2, #0
	mov	r1, r6
	ldr	r0, [r4, #76]
	bl	remove_user_from_buf
	b	.L435
.L423:
	ldr	r3, [r4, #288]
	movs	r1, #5
	ldr	r2, .L436+12
	movs	r0, #4
	ldr	r10, .L436
	movs	r5, #0
	ldr	r7, [r3]
	movw	r9, #878
	ldr	fp, .L436+20
	ldr	r3, [r7, #16]
	str	r3, [sp, #16]
	ldr	r3, [r6]
	str	r3, [sp, #12]
	ldr	r3, [r4]
	str	r3, [sp, #8]
	ldr	r3, [r4, #44]
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	movw	r3, #865
	str	r3, [sp]
	ldr	r3, .L436
	bl	vlog
	mov	r0, r6
	bl	mpi_obj_ref
.L424:
	mov	r1, r6
	mov	r0, r7
	bl	mpi_queue_push_obj
	mov	r8, r0
	cbz	r0, .L426
	ldr	r0, .L436+16
	bl	up
	b	.L416
.L426:
	mov	r0, r7
	bl	mpi_queue_pop_obj
	mov	r5, r0
	cbnz	r0, .L425
	mov	r0, r6
	bl	mpi_obj_unref
	mov	r2, r5
	mov	r1, r6
	ldr	r0, [r4, #76]
	bl	remove_user_from_buf
	mov	r5, #-1
	b	.L416
.L425:
	ldr	r3, [r0]
	mov	r2, fp
	movs	r1, #5
	movs	r0, #4
	str	r3, [sp, #12]
	ldr	r3, [r4]
	str	r3, [sp, #8]
	ldr	r3, [r4, #44]
	ldr	r3, [r3, #44]
	strd	r9, r3, [sp]
	mov	r3, r10
	bl	vlog
	mov	r0, r5
	bl	mpi_obj_unref
	mov	r1, r5
	mov	r2, r8
	ldr	r0, [r4, #76]
	bl	remove_user_from_buf
	mov	r5, #-1
	b	.L424
.L437:
	.align	2
.L436:
	.word	.LANCHOR0+404
	.word	.LC31
	.word	g_venc_dev
	.word	.LC32
	.word	g_venc_dev+196
	.word	.LC33
	.fnend
	.size	venc_handle_message, .-venc_handle_message
	.align	1
	.global	vcodec_notify_drop_frm
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_notify_drop_frm, %function
vcodec_notify_drop_frm:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #15
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	bls	.L439
	movs	r3, #16
	ldr	r2, .L447
	strd	r0, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1005
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L447+4
	bl	vlog
	mov	r0, #-1
.L438:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L439:
	ldr	r3, .L447+8
	add	r2, r0, #8960
	adds	r2, r2, #40
	ldr	r2, [r3, r2, lsl #2]
	cmp	r2, #9
	beq	.L441
	cmp	r2, #15
	bne	.L442
.L441:
	add	r3, r3, r0, lsl #4
	movs	r0, #0
	ldr	r2, [r3, #2152]
	cmp	r2, #0
	ble	.L438
	str	r0, [r3, #2156]
	b	.L438
.L442:
	movs	r0, #0
	b	.L438
.L448:
	.align	2
.L447:
	.word	.LC1
	.word	.LANCHOR0+432
	.word	g_venc_dev
	.fnend
	.size	vcodec_notify_drop_frm, .-vcodec_notify_drop_frm
	.align	1
	.global	venc_node_set_combo
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_set_combo, %function
venc_node_set_combo:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	ldr	r3, [r1, #8]	@ unaligned
	cmp	r3, #12
	beq	.L450
	mov	r3, #1792
	ldr	r2, .L454
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L454+4
	movs	r0, #4
	bl	vlog
.L453:
	mov	r0, #-1
.L449:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L450:
	ldr	r3, [r1]	@ unaligned
	cmp	r3, #15
	ble	.L452
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1797
	ldr	r2, .L454+8
	str	r3, [sp]
	ldr	r3, .L454+4
	bl	vlog
	b	.L453
.L452:
	adds	r3, r3, #134
	ldr	r2, .L454+12
	movs	r4, #1
	lsls	r3, r3, #4
	adds	r0, r2, r3
	str	r4, [r2, r3]
	ldr	r3, [r1, #4]	@ unaligned
	str	r3, [r0, #4]
	movs	r0, #0
	b	.L449
.L455:
	.align	2
.L454:
	.word	.LC34
	.word	.LANCHOR0+455
	.word	.LC1
	.word	g_venc_dev
	.fnend
	.size	venc_node_set_combo, .-venc_node_set_combo
	.align	1
	.global	venc_node_set_snap
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_set_snap, %function
venc_node_set_snap:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	ldr	r3, [r1, #8]	@ unaligned
	cmp	r3, #12
	beq	.L457
	movw	r3, #1813
	ldr	r2, .L462
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L462+4
	movs	r0, #4
	bl	vlog
.L461:
	mov	r0, #-1
.L456:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L457:
	ldr	r3, [r1]	@ unaligned
	cmp	r3, #15
	ble	.L459
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1818
	ldr	r2, .L462+8
	str	r3, [sp]
	ldr	r3, .L462+4
	bl	vlog
	b	.L461
.L459:
	ldr	r2, .L462+12
	ldr	r4, [r1, #4]	@ unaligned
	add	r0, r2, r3, lsl #4
	str	r4, [r0, #2152]
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r0, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r1, #4]	@ unaligned
	cmp	r1, #0
	itt	gt
	addwgt	r3, r3, #2584
	strgt	r1, [r2, r3, lsl #2]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r0	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	b	.L456
.L463:
	.align	2
.L462:
	.word	.LC35
	.word	.LANCHOR0+475
	.word	.LC1
	.word	g_venc_dev
	.fnend
	.size	venc_node_set_snap, .-venc_node_set_snap
	.align	1
	.global	vcodec_notify
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_notify, %function
vcodec_notify:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #15
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	bhi	.L475
	cmp	r1, #3
	bhi	.L475
	tbb	[pc, r1]
.L467:
	.byte	(.L470-.L467)/2
	.byte	(.L469-.L467)/2
	.byte	(.L481-.L467)/2
	.byte	(.L466-.L467)/2
	.p2align 1
.L470:
	ldr	r5, .L483
	movw	r3, #43232
	ldr	r4, [r5, r3]
	cmp	r4, #0
	bne	.L465
	ldrd	r10, [r2]
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r9, .L483+8
	add	r6, r5, #404
	movw	r8, #1031
.L473:
	ldr	r3, [r6], #4
	cbz	r3, .L471
	ldr	r3, [r6, #764]
	adds	r3, r3, #1
	bne	.L471
	mov	r3, r9
	ldr	r2, .L483+4
	movs	r1, #5
	strd	r10, [sp, #8]
	strd	r8, r4, [sp]
	movs	r0, #4
	bl	vlog
	add	r3, r5, #1440
	add	r1, r5, #1312
	mov	r0, r4
	ldrd	r2, [r3]
	strd	r2, [sp]
	ldrd	r2, [r1]
	bl	mpp_vcodec_run_task
	cbnz	r0, .L472
	bl	mcu_get_wrap_status
	cmp	r0, #2
	bne	.L472
	movs	r3, #1
.L480:
	str	r3, [r6, #764]
.L471:
	adds	r4, r4, #1
	adds	r5, r5, #8
	cmp	r4, #16
	bne	.L473
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L482:
	b	.L481
.L472:
	movs	r3, #0
	b	.L480
.L469:
	add	r0, r0, #10752
	ldr	r3, .L483
	adds	r0, r0, #40
	ldr	r3, [r3, r0, lsl #2]
	str	r3, [r2]
.L481:
	movs	r1, #0
.L465:
	mov	r0, r1
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L466:
	ldr	r0, .L483
	movw	r3, #43232
	movs	r2, #1
	str	r2, [r0, r3]
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #64
	movs	r1, #0
	addw	r0, r0, #1172
	bl	memset
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L482
.L475:
	mov	r1, #-1
	b	.L465
.L484:
	.align	2
.L483:
	.word	g_venc_dev
	.word	.LC36
	.word	.LANCHOR0+494
	.fnend
	.size	vcodec_notify, .-vcodec_notify
	.align	1
	.global	venc_node_check
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_check, %function
venc_node_check:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r3, [r1, #8]	@ unaligned
	cmp	r3, #16
	beq	.L486
	movw	r3, #1537
	ldr	r2, .L490
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L490+4
	movs	r0, #4
	bl	vlog
.L489:
	mov	r0, #-1
.L485:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L486:
	ldr	r0, [r1, #4]	@ unaligned
	cmp	r0, #15
	ble	.L488
	strd	r0, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1541
	ldr	r2, .L490+8
	str	r3, [sp]
	movs	r0, #4
	ldr	r3, .L490+4
	bl	vlog
	b	.L489
.L488:
	bl	get_venc_node
	clz	r0, r0
	lsrs	r0, r0, #5
	rsbs	r0, r0, #0
	b	.L485
.L491:
	.align	2
.L490:
	.word	.LC11
	.word	.LANCHOR0+508
	.word	.LC1
	.fnend
	.size	venc_node_check, .-venc_node_check
	.align	1
	.global	mpi_buf_get_paddr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_get_paddr, %function
mpi_buf_get_paddr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbz	r0, .L494
	ldr	r0, [r0, #608]
	cmp	r0, #0
	bgt	.L492
	ldr	r0, [r4, #448]
	bl	get_dma_buf_phy_addr
	str	r0, [r4, #608]
.L492:
	pop	{r4, pc}
.L494:
	mov	r0, #-1
	b	.L492
	.fnend
	.size	mpi_buf_get_paddr, .-mpi_buf_get_paddr
	.align	1
	.global	venc_get_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_get_version, %function
venc_get_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L500
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	ldr	r6, [r3, #164]
	ldr	r3, [r1, #32]	@ unaligned
	cmp	r3, #76
	beq	.L496
	movw	r3, #1516
	ldr	r2, .L500+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L500+8
	movs	r0, #4
	bl	vlog
	mov	r0, #-1
.L495:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L496:
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
	strb	r5, [r1, #36]
	strb	r5, [r1, #37]
	strb	r5, [r1, #38]
	strb	r5, [r1, #39]
	strb	r5, [r1, #56]
	strb	r5, [r1, #57]
	strb	r5, [r1, #58]
	strb	r5, [r1, #59]
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
	b	.L495
.L501:
	.align	2
.L500:
	.word	g_venc_dev
	.word	.LC37
	.word	.LANCHOR0+524
	.fnend
	.size	venc_get_version, .-venc_get_version
	.align	1
	.global	venc_node_set_crop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_set_crop, %function
venc_node_set_crop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r0, [r1, #12]	@ unaligned
	cmp	r0, #24
	beq	.L503
	movw	r3, #1732
	ldr	r2, .L509
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L509+4
	movs	r0, #4
	bl	vlog
.L508:
	mov	r0, #-1
.L502:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L503:
	ldr	r2, [r1]	@ unaligned
	cmp	r2, #15
	ble	.L505
	movs	r3, #16
	movs	r1, #2
	strd	r2, r3, [sp, #4]
	movs	r0, #4
	movw	r3, #1737
	ldr	r2, .L509+8
	str	r3, [sp]
	ldr	r3, .L509+4
	bl	vlog
	b	.L508
.L505:
	mov	r3, r1
	ldr	r1, .L509+12
	add	r5, r3, #24
	mla	r2, r0, r2, r1
.L506:
	ldr	r0, [r3]	@ unaligned
	ldr	r1, [r3, #4]	@ unaligned
	adds	r3, r3, #8
	cmp	r3, r5
	mov	r4, r2
	stmia	r4!, {r0, r1}
	mov	r2, r4
	bne	.L506
	movs	r0, #0
	b	.L502
.L510:
	.align	2
.L509:
	.word	.LC38
	.word	.LANCHOR0+541
	.word	.LC1
	.word	g_venc_dev+1760
	.fnend
	.size	venc_node_set_crop, .-venc_node_set_crop
	.align	1
	.global	venc_node_get_crop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_get_crop, %function
venc_node_get_crop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #24
	beq	.L512
	movw	r3, #1750
	ldr	r2, .L518
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L518+4
	movs	r0, #4
	bl	vlog
.L517:
	mov	r0, #-1
.L511:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L512:
	ldr	r2, [r1]	@ unaligned
	cmp	r2, #15
	ble	.L514
	movs	r3, #16
	movs	r1, #2
	strd	r2, r3, [sp, #4]
	movs	r0, #4
	movw	r3, #1755
	ldr	r2, .L518+8
	str	r3, [sp]
	ldr	r3, .L518+4
	bl	vlog
	b	.L517
.L514:
	ldr	r0, .L518+12
	mla	r2, r3, r2, r0
	mov	r3, r1
	add	r5, r2, #24
.L515:
	mov	r4, r2
	adds	r3, r3, #8
	ldmia	r4!, {r0, r1}
	cmp	r4, r5
	str	r0, [r3, #-8]	@ unaligned
	mov	r2, r4
	str	r1, [r3, #-4]	@ unaligned
	bne	.L515
	movs	r0, #0
	b	.L511
.L519:
	.align	2
.L518:
	.word	.LC38
	.word	.LANCHOR0+560
	.word	.LC1
	.word	g_venc_dev+1760
	.fnend
	.size	venc_node_get_crop, .-venc_node_get_crop
	.align	1
	.global	venc_node_pp_set_md_attr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_pp_set_md_attr, %function
venc_node_pp_set_md_attr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r3, [r1, #4]	@ unaligned
	cmp	r3, #36
	beq	.L521
	movw	r3, #2071
	ldr	r2, .L527
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L527+4
	movs	r0, #4
	bl	vlog
.L526:
	mov	r0, #-1
.L520:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L521:
	ldr	r2, [r1]	@ unaligned
	cmp	r2, #7
	ble	.L523
	movs	r3, #8
	movs	r1, #2
	strd	r2, r3, [sp, #4]
	movs	r0, #4
	movw	r3, #2076
	ldr	r2, .L527+8
	str	r3, [sp]
	ldr	r3, .L527+4
	bl	vlog
	b	.L526
.L523:
	mov	r3, r1
	ldr	r1, .L527+12
	mov	r0, #988
	add	r5, r3, #32
	mla	r2, r0, r2, r1
.L524:
	ldr	r0, [r3]	@ unaligned
	ldr	r1, [r3, #4]	@ unaligned
	adds	r3, r3, #8
	cmp	r3, r5
	mov	r4, r2
	stmia	r4!, {r0, r1}
	mov	r2, r4
	bne	.L524
	ldr	r0, [r3]	@ unaligned
	str	r0, [r4]
	movs	r0, #0
	b	.L520
.L528:
	.align	2
.L527:
	.word	.LC0
	.word	.LANCHOR0+579
	.word	.LC1
	.word	g_venc_dev+3260
	.fnend
	.size	venc_node_pp_set_md_attr, .-venc_node_pp_set_md_attr
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_open, %function
venc_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r2, #-1
	mov	r4, r1
	movs	r0, #12
	mov	r1, #3520
	bl	kvmalloc_node
	str	r0, [r4, #128]
	cbnz	r0, .L530
	movw	r3, #2280
	ldr	r2, .L532
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L532+4
	movs	r0, #4
	bl	vlog
	mov	r0, #-1
.L529:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L530:
	ldr	r0, .L532+8
	bl	mpi_obj_ref
	movs	r0, #0
	b	.L529
.L533:
	.align	2
.L532:
	.word	.LC39
	.word	.LANCHOR0+604
	.word	g_venc_dev+4
	.fnend
	.size	venc_open, .-venc_open
	.align	1
	.global	venc_node_pp_get_result
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_node_pp_get_result, %function
venc_node_pp_get_result:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r1
	ldr	r3, [r1, #4]	@ unaligned
	.pad #20
	sub	sp, sp, #20
	cmp	r3, #32
	beq	.L535
	movw	r3, #2038
	ldr	r2, .L553
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L553+4
	movs	r0, #4
	bl	vlog
.L552:
	mov	r0, #-1
.L534:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L535:
	ldr	r4, [r1]	@ unaligned
	cmp	r4, #7
	ble	.L537
	movs	r3, #8
	ldr	r2, .L553+8
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #2043
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L553+4
	bl	vlog
	b	.L552
.L537:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r7, .L553+12
	mov	r3, #988
	mla	r3, r3, r4, r7
	ldr	r2, [r3, #2460]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r3, #2416]
	cbz	r1, .L538
	ldr	r1, [r5, #8]	@ unaligned
	cbz	r1, .L538
	movs	r6, #247
	ldr	r3, [r3, #2468]
	mla	r6, r6, r4, r2
	movs	r1, #0
	str	r3, [r5, #20]	@ unaligned
	add	r6, r6, #644
	add	r6, r7, r6, lsl #2
	ldr	r3, [r6, #4]
	ldr	r0, [r3, #448]
	bl	dma_buf_fd
	str	r0, [r5, #16]	@ unaligned
	ldr	r3, [r6, #4]
	ldr	r3, [r3, #448]
	ldr	r3, [r3, #4]
	add	r2, r3, #28
	pld	[r2]
	.syntax unified
@ 223 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r1, [r2]
	add	r1, r1, #1
	strex	r0, r1, [r2]
	teq	r0, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
.L538:
	mov	r3, #988
	mla	r3, r3, r4, r7
	ldr	r2, [r3, #2420]
	cbz	r2, .L539
	ldr	r2, [r5, #12]	@ unaligned
	cbz	r2, .L539
	ldr	r2, [r3, #2472]
	str	r2, [r5, #24]	@ unaligned
	ldr	r3, [r3, #2476]
	str	r3, [r5, #28]	@ unaligned
.L539:
	mov	r3, #988
	movs	r0, #0
	mla	r4, r3, r4, r7
	str	r0, [r4, #2464]
	b	.L534
.L554:
	.align	2
.L553:
	.word	.LC40
	.word	.LANCHOR0+614
	.word	.LC1
	.word	g_venc_dev
	.fnend
	.size	venc_node_pp_get_result, .-venc_node_pp_get_result
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_thread_fn, %function
venc_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 424
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #444
	sub	sp, sp, #444
	adds	r3, r0, #4
	mov	r5, r0
	str	r3, [sp, #8]
	movs	r1, #5
	ldr	r3, [r0, #48]
	movs	r0, #4
	ldr	r2, .L634
	str	r3, [sp, #4]
	movw	r3, #2394
	str	r3, [sp]
	ldr	r3, .L634+4
	bl	vlog
	add	r3, r5, #196
	str	r3, [sp, #20]
.L556:
	bl	kthread_should_stop
	cbz	r0, .L593
	movs	r1, #4
	movw	r3, #2550
	ldr	r2, .L634+8
	mov	r0, r1
	str	r3, [sp]
	ldr	r3, .L634+4
	bl	vlog
	movs	r0, #0
	add	sp, sp, #444
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L593:
	ldr	r3, [r5, #180]
	cbz	r3, .L557
	movs	r0, #1
	bl	msleep
	b	.L556
.L635:
	.align	2
.L634:
	.word	.LC41
	.word	.LANCHOR0+638
	.word	.LC42
.L557:
	ldr	r0, [sp, #20]
	mov	r10, r5
	bl	down
	ldr	r7, [r10, #92]!
.L633:
	subs	r7, r7, #48
	add	r3, r7, #48
	cmp	r3, r10
	beq	.L556
	movs	r4, #0
.L561:
	mov	r0, r4
	bl	get_pp_node
	cmp	r7, r0
	beq	.L560
	adds	r4, r4, #1
	cmp	r4, #16
	bne	.L561
	mov	r4, #-1
.L560:
	ldr	r3, [r7, #288]
	ldr	r0, [r3]
	bl	mpi_queue_pop_obj
	mov	r6, r0
	cmp	r0, #0
	beq	.L562
	mov	r3, #988
	ldrd	r2, r0, [r0, #620]
	mla	r3, r3, r4, r5
	ldr	r1, [r3, #2428]
	cmp	r2, r1
	bhi	.L563
	ldr	r1, [r3, #2432]
	cmp	r1, r0
	bcc	.L563
	ldr	r1, [r3, #2400]
	cmp	r2, r1
	bne	.L564
	ldr	r3, [r3, #2404]
	cmp	r3, r0
	beq	.L565
.L564:
	mov	r2, #988
	movs	r1, #0
	muls	r2, r4, r2
	mov	r0, r4
	adds	r3, r5, r2
	add	r2, r2, #2400
	add	r2, r2, r5
	str	r1, [r3, #3376]
	ldr	r1, [r6, #620]
	str	r1, [r3, #2400]
	ldr	r1, [r6, #624]
	str	r1, [r3, #2404]
	movs	r1, #17
	bl	vepu_pp_control
.L565:
	mov	r8, #988
	movs	r3, #6
	mla	r8, r8, r4, r5
	str	r3, [sp, #180]
	movs	r1, #16
	mov	r0, r4
	str	r6, [sp, #196]
	ldr	r3, [r8, #3376]
	adds	r2, r3, #1
	str	r2, [r8, #3376]
	str	r3, [sp, #176]
	add	r2, sp, #176
	ldr	r3, [r8, #2556]
	str	r3, [sp, #184]
	movs	r3, #1
	strd	r3, r3, [sp, #188]
	bl	vepu_pp_control
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r8, #2460]
	add	r9, r2, #1
	cmp	r9, #5
	it	ge
	movge	r9, #0
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r8, #2416]
	cmp	r3, #0
	beq	.L567
	movs	r3, #247
	mla	r3, r3, r4, r9
	add	r3, r5, r3, lsl #2
	ldr	r3, [r3, #2580]
	str	r3, [sp, #120]
	ldr	r3, [r8, #3276]
	usat	r3, #2, r3
	str	r3, [sp, #124]
	ldr	r3, [r8, #3272]
	cmp	r3, #0
	ble	.L568
	ldr	r2, [r8, #3268]
	cmp	r2, #0
	ble	.L568
	movw	r1, #4095
	cmp	r2, r1
	ite	le
	strle	r2, [sp, #128]
	strgt	r1, [sp, #128]
	cmp	r3, #4
	it	ge
	movge	r3, #4
.L629:
	str	r3, [sp, #132]
	mov	r3, #988
	mla	r3, r3, r4, r5
	movs	r1, #32
	mov	r0, r4
	ldr	r2, [r3, #2568]
	str	r2, [sp, #136]
	ldr	r2, [r3, #3280]
	str	r2, [sp, #140]
	ldr	r2, [r3, #3284]
	str	r2, [sp, #144]
	ldr	r2, [r3, #3288]
	ldr	r3, [r3, #3292]
	str	r2, [sp, #148]
	add	r2, sp, #120
	str	r3, [sp, #152]
	bl	vepu_pp_control
.L567:
	mov	r2, #988
	mla	r2, r2, r4, r5
	ldr	r3, [r2, #2420]
	cbz	r3, .L572
	ldr	r1, [r2, #2400]
	ldr	r3, [r2, #2404]
	ldr	r0, [r2, #2572]
	ldr	r2, [r2, #2424]
	muls	r3, r1, r3
	cmp	r2, #0
	beq	.L573
	asrs	r3, r3, #8
.L574:
	cmp	r0, #6
	bgt	.L575
	mov	r1, #988
	movs	r2, #230
	mla	r1, r1, r4, r5
	muls	r2, r3, r2
	ldr	r1, [r1, #3300]
	cmp	r1, r2
	it	gt
	addgt	r0, r0, #1
.L575:
	muls	r3, r0, r3
	movs	r1, #1
	movs	r2, #12
	mov	r0, r4
	strd	r1, r2, [sp, #28]
	add	r2, sp, #28
	movs	r1, #48
	asrs	r3, r3, #3
	str	r3, [sp, #36]
	bl	vepu_pp_control
.L572:
	mov	r3, #988
	muls	r3, r4, r3
	adds	r2, r5, r3
	ldr	r1, [r2, #2408]
	cbz	r1, .L576
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r2, #3256]
	adds	r2, r2, #90
	add	r3, r3, r2, lsl #3
	add	r3, r3, r5
	ldr	r2, [r3, #2404]
	str	r2, [sp, #308]
	movs	r2, #0
	str	r2, [r3, #2408]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r1, #64
	add	r2, sp, #308
	mov	r0, r4
	bl	vepu_pp_control
.L576:
	mov	r3, #988
	mla	r3, r3, r4, r5
	ldr	r3, [r3, #2412]
	cbz	r3, .L577
	add	r2, sp, #240
	movs	r1, #80
	mov	r0, r4
	bl	vepu_pp_control
.L577:
	mov	r3, #988
	mla	r3, r3, r4, r5
	ldr	r2, [r3, #2416]
	cbnz	r2, .L578
	ldr	r2, [r3, #2420]
	cbnz	r2, .L578
	ldr	r2, [r3, #2408]
	cbnz	r2, .L578
	ldr	r3, [r3, #2412]
	cmp	r3, #0
	beq	.L579
.L578:
	mov	r8, #988
	add	r2, sp, #176
	mul	r8, r8, r4
	movs	r1, #96
	mov	r0, r4
	bl	vepu_pp_control
	add	r3, r5, r8
	ldr	r3, [r3, #2420]
	cbz	r3, .L579
	add	r2, r8, #3296
	movs	r1, #112
	add	r2, r2, r5
	mov	r0, r4
	bl	vepu_pp_control
	addw	r3, r8, #3372
	add	r3, r3, r5
	movs	r2, #9
.L581:
	ldr	r1, [r3, #-4]
	subs	r2, r2, #1
	str	r1, [r3], #-4
	bne	.L581
	mov	r3, #988
	mla	r3, r3, r4, r5
	ldr	r2, [r3, #3300]
	str	r2, [r3, #3336]
	ldr	r3, [r3, #3296]
	cbz	r3, .L579
	addw	r8, r8, #3340
	movs	r1, #95
	movs	r0, #105
	add	r8, r8, r5
	muls	r1, r2, r1
	mov	ip, #100
	muls	r0, r2, r0
	movs	r2, #1
.L584:
	ldr	r3, [r8], #4
	mul	r3, ip, r3
	cmp	r1, r3
	bgt	.L582
	cmp	r3, r0
	ble	.L583
.L582:
	mov	r3, #988
	movs	r2, #0
	mla	r3, r3, r4, r5
	str	r2, [r3, #3296]
.L579:
	mov	r8, #988
	mul	r8, r8, r4
	add	fp, r5, r8
	ldr	r3, [fp, #2412]
	cbz	r3, .L585
	add	r2, sp, #72
	movs	r1, #128
	mov	r0, r4
	bl	vepu_pp_control
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	lr, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [fp, #3120]
	addw	r3, r8, #2600
	movs	r1, #52
	add	fp, sp, #120
	mla	r3, r1, r2, r3
	add	r2, sp, #72
	add	r3, r3, r5
.L586:
	mov	ip, r2
	adds	r3, r3, #8
	ldmia	ip!, {r0, r1}
	cmp	ip, fp
	str	r0, [r3, #-8]	@ unaligned
	mov	r2, ip
	str	r1, [r3, #-4]	@ unaligned
	bne	.L586
	mov	r1, #988
	movs	r0, #52
	muls	r1, r4, r1
	adds	r2, r5, r1
	ldr	r3, [r2, #3120]
	mla	r3, r0, r3, r1
	ldr	r1, [r6, #668]
	add	r3, r3, r5
	str	r1, [r3, #2648]
	ldr	r3, [r2, #3120]
	adds	r3, r3, #1
	cmp	r3, #9
	it	gt
	movgt	r3, #0
	str	r3, [r2, #3120]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, lr	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L585:
	mov	r3, #988
	muls	r3, r4, r3
	adds	r2, r5, r3
	ldr	r1, [r2, #2408]
	cbz	r1, .L589
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	ip, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, [r2, #3256]
	ldr	r1, [r6, #668]
	adds	r0, r0, #90
	add	r3, r3, r0, lsl #3
	add	r3, r3, r5
	str	r1, [r3, #2408]
	ldr	r3, [r2, #3256]
	adds	r3, r3, #1
	cmp	r3, #1
	it	gt
	movgt	r3, #0
	str	r3, [r2, #3256]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, ip	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L589:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, #988
	addw	r2, r8, #2472
	mla	r4, r3, r4, r5
	movs	r3, #1
	add	r2, r2, r5
	str	r3, [r4, #2464]
	add	r3, r8, #3296
	add	r3, r3, r5
	str	r9, [r4, #2460]
	add	r0, r3, #40
.L592:
	ldr	r4, [r3], #4	@ unaligned
	cmp	r3, r0
	str	r4, [r2], #4	@ unaligned
	bne	.L592
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	addw	r0, r8, #3380
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	add	r0, r0, r5
	bl	__wake_up
.L632:
	ldr	r0, [r7, #76]
	movs	r2, #0
	mov	r1, r6
	bl	remove_user_from_buf
	mov	r0, r6
	bl	mpi_buf_unref
.L562:
	ldr	r7, [r7, #48]
	b	.L633
.L568:
	mov	r3, #988
	mla	r3, r3, r4, r5
	ldr	r3, [r3, #2576]
	cmp	r3, #1
	bne	.L570
	movs	r3, #70
	str	r3, [sp, #128]
	movs	r3, #3
	b	.L629
.L570:
	cmp	r3, #2
	itett	ne
	movne	r3, #30
	moveq	r2, #50
	strne	r3, [sp, #128]
	movne	r3, #1
	it	eq
	streq	r2, [sp, #128]
	b	.L629
.L573:
	asrs	r3, r3, #4
	b	.L574
.L583:
	adds	r2, r2, #1
	cmp	r2, #10
	bne	.L584
	b	.L579
.L563:
	strd	r2, r0, [sp, #4]
	mov	r3, #2544
	ldr	r2, .L636
	movs	r1, #2
	str	r3, [sp]
	movs	r0, #4
	ldr	r3, .L636+4
	bl	vlog
	b	.L632
.L637:
	.align	2
.L636:
	.word	.LC43
	.word	.LANCHOR0+638
	.fnend
	.size	venc_thread_fn, .-venc_thread_fn
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	kzalloc.constprop.16, %function
kzalloc.constprop.16:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r1, #3520
	b	__kmalloc
	.fnend
	.size	kzalloc.constprop.16, .-kzalloc.constprop.16
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_create_ctx, %function
venc_create_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	movs	r0, #4
	bl	kzalloc.constprop.16
	mov	r4, r0
	cbz	r0, .L639
	ldr	r2, .L644
	movs	r1, #4
	movs	r0, #1
	bl	mpi_queue_create
	mov	r5, r0
	str	r0, [r4]
	cbnz	r0, .L639
	mov	r0, r4
	mov	r4, r5
	bl	kfree
.L639:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L645:
	.align	2
.L644:
	.word	mpi_obj_unref
	.fnend
	.size	venc_create_ctx, .-venc_create_ctx
	.align	1
	.global	vcodec_create_dev
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_create_dev, %function
vcodec_create_dev:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	movs	r0, #164
	mov	r5, r1
	bl	kzalloc.constprop.16
	mov	r4, r0
	cbz	r0, .L646
	ldr	r3, [r5]
	mov	r2, r6
	movs	r1, #8
	str	r3, [r0, #68]
	ldr	r3, [r5, #8]
	str	r3, [r0, #80]
	ldr	r3, [r5, #4]
	str	r3, [r0, #76]
	bl	mpi_dev_register
.L646:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
	.fnend
	.size	vcodec_create_dev, .-vcodec_create_dev
	.align	1
	.global	to_osd_stride
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	to_osd_stride, %function
to_osd_stride:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	r0, r0, #65536
	subs	r0, r0, #10
	cmp	r0, #16
	bhi	.L657
	movs	r2, #1
	movw	r3, #15375
	lsl	r0, r2, r0
	ands	r3, r0, r3
	bne	.L655
	tst	r0, #960
	bne	.L656
	lsls	r3, r0, #15
	bpl	.L657
	movs	r0, #2
	mov	r3, r2
.L653:
	muls	r1, r3, r1
	asr	r0, r1, r0
	bx	lr
.L655:
	movs	r0, #0
	movs	r3, #2
	b	.L653
.L656:
	mov	r0, r3
	movs	r3, #4
	b	.L653
.L657:
	mov	r0, #-1
	bx	lr
	.fnend
	.size	to_osd_stride, .-to_osd_stride
	.align	1
	.global	set_2bpp_color_lut
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_2bpp_color_lut, %function
set_2bpp_color_lut:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mvn	r4, #20
	ldr	r3, [r1]
	mvn	r7, #106
	ldr	r2, [r1, #4]
	.pad #68
	sub	sp, sp, #68
	ubfx	ip, r3, #16, #8
	ubfx	lr, r3, #8, #8
	uxtb	r3, r3
	ubfx	r8, r2, #16, #8
	ubfx	r9, r2, #8, #8
	uxtb	r2, r2
	mul	r1, r4, r3
	mvn	fp, #42
	muls	r4, r2, r4
	mvn	r6, #84
	add	r1, r1, #32768
	mov	r10, #150
	add	r1, r1, ip, lsl #7
	movs	r5, #77
	add	r4, r4, #32768
	mla	r1, r7, lr, r1
	add	r4, r4, r8, lsl #7
	mla	r7, r7, r9, r4
	lsls	r4, r2, #7
	add	r4, r4, #32768
	asrs	r1, r1, #8
	str	r1, [sp, #40]
	strb	r1, [r0]
	lsls	r1, r3, #7
	add	r1, r1, #32768
	asrs	r7, r7, #8
	mla	r1, fp, ip, r1
	mla	fp, fp, r8, r4
	mla	r1, r6, lr, r1
	mla	fp, r6, r9, fp
	movs	r6, #77
	asrs	r1, r1, #8
	str	r1, [sp, #36]
	strb	r1, [r0, #1]
	mul	r1, r10, lr
	strb	r7, [r0, #3]
	mul	r10, r10, r9
	asr	r4, fp, #8
	mla	r1, r5, ip, r1
	movs	r5, #29
	mla	r10, r6, r8, r10
	strb	r4, [r0, #4]
	mla	r1, r5, r3, r1
	mla	r5, r5, r2, r10
	asrs	r1, r1, #8
	strb	r1, [r0, #2]
	asrs	r5, r5, #8
	strb	r5, [r0, #5]
	strd	r4, r7, [sp, #52]
	str	r5, [sp, #48]
	ldrb	r4, [r0, #7]	@ zero_extendqisi2
	str	r1, [sp, #32]
	str	r4, [sp, #44]
	ldrb	r1, [r0, #6]	@ zero_extendqisi2
	movs	r0, #4
	strd	lr, r3, [sp, #8]
	mov	r3, #440
	strd	r8, r9, [sp, #16]
	strd	r2, r1, [sp, #24]
	movs	r1, #5
	str	ip, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L659
	ldr	r3, .L659+4
	bl	vlog
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L660:
	.align	2
.L659:
	.word	.LC44
	.word	.LANCHOR0+653
	.fnend
	.size	set_2bpp_color_lut, .-set_2bpp_color_lut
	.align	1
	.global	set_invert_buffer
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_invert_buffer, %function
set_invert_buffer:
	.fnstart
	@ args = 8, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	ldrd	r4, r6, [sp, #56]
	strd	r1, r2, [sp]
	str	r3, [sp, #8]
	cmp	r4, #0
	beq	.L662
	cmp	r0, #0
	beq	.L679
	cmp	r6, #0
	beq	.L680
	ldr	r3, [r4, #604]
	cmp	r3, #0
	beq	.L681
	ldr	r2, [r6, #12]
	cbz	r2, .L663
	cmp	r2, #1
	bne	.L664
	mov	ip, #0
	mov	lr, r0
	mov	r8, ip
	str	r2, [sp, #12]
.L665:
	ldr	r2, [sp, #4]
	cmp	r8, r2
	bge	.L664
	mov	r10, lr
	movs	r5, #0
	b	.L678
.L663:
	mov	lr, r0
	mov	r8, r2
.L666:
	ldr	r1, [sp, #4]
	cmp	r8, r1
	bge	.L664
	mov	r10, lr
	mov	ip, #0
	b	.L672
.L670:
	add	r9, r2, ip, lsr #3
	and	r5, ip, #7
	movs	r1, #1
	ldrb	fp, [r10], #1	@ zero_extendqisi2
	lsl	r5, r1, r5
	ldr	r7, [r6, #8]
	ldrsb	r1, [r3, r9]
	add	ip, ip, #1
	sxtb	r5, r5
	cmp	fp, r7
	ite	cc
	orrcc	r1, r1, r5
	biccs	r1, r1, r5
	strb	r1, [r3, r9]
.L672:
	ldr	r1, [sp]
	cmp	ip, r1
	blt	.L670
	add	lr, lr, r1
	ldr	r1, [sp, #8]
	add	r8, r8, #1
	add	r2, r2, r1
	b	.L666
.L676:
	ldr	r2, [sp, #12]
	add	r9, ip, r5, lsr #3
	and	r1, r5, #7
	ldrb	fp, [r10], #1	@ zero_extendqisi2
	ldr	r7, [r6, #8]
	adds	r5, r5, #1
	lsl	r1, r2, r1
	ldrsb	r2, [r3, r9]
	cmp	fp, r7
	sxtb	r1, r1
	ite	hi
	orrhi	r2, r2, r1
	bicls	r2, r2, r1
	strb	r2, [r3, r9]
.L678:
	ldr	r2, [sp]
	cmp	r5, r2
	blt	.L676
	add	lr, lr, r2
	ldr	r2, [sp, #8]
	add	r8, r8, #1
	add	ip, ip, r2
	b	.L665
.L664:
	bl	kfree
	ldr	r1, [r4, #604]
	ldr	r0, [r4, #448]
	bl	dma_buf_vunmap
	movs	r3, #0
	str	r3, [r4, #604]
.L662:
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L679:
	mov	r4, r0
	b	.L662
.L680:
	mov	r4, r6
	b	.L662
.L681:
	mov	r4, r3
	b	.L662
	.fnend
	.size	set_invert_buffer, .-set_invert_buffer
	.align	1
	.global	create_lingt_lut
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	create_lingt_lut, %function
create_lingt_lut:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	kzalloc.constprop.16
	cbnz	r0, .L691
.L687:
	pop	{r4, pc}
.L689:
	strb	r3, [r0, r3]
	adds	r3, r3, #1
.L688:
	cmp	r3, r4
	blt	.L689
	b	.L687
.L691:
	movs	r3, #0
	b	.L688
	.fnend
	.size	create_lingt_lut, .-create_lingt_lut
	.global	__aeabi_uidiv
	.align	1
	.global	to_osd_invert_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	to_osd_invert_cfg, %function
to_osd_invert_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r4, r0
	ldr	r7, [r1, #12]
	mov	r6, r1
	ldr	r5, [r1, #72]
	ldr	r0, [r7, #40]
	subs	r3, r5, #1
	mov	r1, r5
	add	r3, r3, r0
	rsbs	r0, r5, #0
	ands	r0, r0, r3
	bl	__aeabi_uidiv
	ldr	r1, [r6, #76]
	subs	r3, r1, #1
	mov	r8, r0
	ldr	r0, [r7, #44]
	add	r7, r8, #7
	add	r3, r3, r0
	rsbs	r0, r1, #0
	ands	r0, r0, r3
	asrs	r7, r7, #3
	bl	__aeabi_uidiv
	cmp	r5, #32
	mov	r9, r0
	mul	r10, r0, r8
	beq	.L693
	cmp	r5, #64
	beq	.L694
	cmp	r5, #16
	bne	.L695
	movs	r3, #0
.L699:
	str	r3, [r4, #20]
.L696:
	movs	r3, #0
	movs	r2, #1
	str	r3, [r4, #8]
	mov	r0, r10
	strd	r2, r2, [r4]
	strd	r3, r3, [r4, #12]
	str	r7, [r4, #24]
	bl	create_lingt_lut
	cbnz	r0, .L697
	movw	r3, #542
	ldr	r2, .L700
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L700+4
	movs	r0, #4
	bl	vlog
.L692:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L693:
	movs	r3, #1
	b	.L699
.L694:
	movs	r3, #3
	b	.L699
.L695:
	ldr	r0, .L700+8
	bl	printk
	b	.L696
.L697:
	add	r3, r6, #72
	str	r3, [sp, #4]
	ldr	r3, [r6, #16]
	mov	r2, r9
	mov	r1, r8
	str	r3, [sp]
	mov	r3, r7
	bl	set_invert_buffer
	str	r0, [r4, #32]
	b	.L692
.L701:
	.align	2
.L700:
	.word	.LC46
	.word	.LANCHOR0+672
	.word	.LC45
	.fnend
	.size	to_osd_invert_cfg, .-to_osd_invert_cfg
	.align	1
	.global	set_osd_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_osd_data, %function
set_osd_data:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r5, r0
	ldr	r3, [r1, #104]
	mov	r9, r1
	cbnz	r3, .L703
	str	r3, [r0, #4]
.L702:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L703:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	fp, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r4, [r1, #84]
	movs	r6, #0
	subs	r4, r4, #4
.L705:
	adds	r3, r4, #4
	add	r2, r9, #84
	cmp	r3, r2
	bne	.L721
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, fp	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	str	r6, [r5, #4]
	b	.L702
.L721:
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #1752]
	cmp	r3, #0
	beq	.L706
	movs	r3, #140
	ldr	r2, [r4, #20]
	mla	r3, r3, r6, r5
	adds	r2, r2, #0
	it	ne
	movne	r2, #1
	str	r2, [r3, #8]
	ldr	r2, [r4, #28]
	str	r2, [r3, #28]
	ldr	r2, [r4, #32]
	str	r2, [r3, #32]
	ldr	r2, [r4, #12]
	ldr	r1, [r4, #28]
	ldr	r2, [r2, #40]
	add	r2, r2, r1
	subs	r2, r2, #1
	str	r2, [r3, #36]
	ldr	r2, [r4, #12]
	ldr	r1, [r4, #32]
	ldr	r2, [r2, #44]
	add	r2, r2, r1
	subs	r2, r2, #1
	str	r2, [r3, #40]
	ldr	r2, [r4, #12]
	ldr	r0, [r2, #1752]
	str	r0, [r3, #136]
	cbz	r0, .L707
	bl	mpi_obj_ref
.L707:
	ldr	r3, [r4, #12]
	movs	r7, #140
	ldrd	r0, r1, [r3, #36]
	bl	to_osd_stride
	movs	r3, #140
	mla	r3, r3, r6, r5
	movs	r1, #1
	add	r10, r6, r1
	str	r0, [r3, #44]
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #36]
	sub	r3, r3, #65536
	subs	r3, r3, #10
	cmp	r3, #16
	itet	ls
	ldrls	r2, .L744
	movhi	r3, #-1
	addls	r3, r2, r3, lsl #2
	mla	r2, r7, r6, r5
	it	ls
	ldrls	r3, [r3, #692]
	mul	r7, r7, r10
	str	r3, [r2, #20]
	movs	r3, #0
	strd	r3, r1, [r2, #12]
	add	r8, r5, r7
	ldrd	r2, r0, [r4, #36]
	strb	r3, [r5, r7]
	strb	r3, [r8, #1]
	strb	r3, [r8, #2]
	strb	r3, [r8, #3]
	strb	r3, [r8, #4]
	strb	r3, [r8, #5]
	strb	r0, [r8, #6]
	strb	r2, [r8, #7]
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #36]
	sub	r3, r3, #65536
	subs	r3, r3, #10
	cmp	r3, #16
	bhi	.L709
	lsl	r3, r1, r3
	movw	r2, #9797
	ands	r2, r3, r2
	bne	.L710
	ldr	r1, .L744+4
	tst	r3, r1
	bne	.L724
.L709:
	ldr	r0, .L744+8
	bl	printk
	mov	r1, #-1
.L710:
	movs	r2, #140
	mla	r2, r2, r6, r5
	str	r1, [r2, #24]
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #36]
	sub	r3, r3, #65536
	subs	r3, r3, #10
	cmp	r3, #16
	bhi	.L711
	movs	r1, #1
	lsl	r3, r1, r3
	ands	r2, r3, #207
	bne	.L712
	ldr	r1, .L744+12
	tst	r3, r1
	bne	.L726
.L711:
	ldr	r0, .L744+16
	bl	printk
	mov	r1, #-1
.L712:
	movs	r2, #140
	mla	r2, r2, r6, r5
	str	r1, [r2, #92]
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #36]
	sub	r3, r3, #65536
	subs	r3, r3, #10
	cmp	r3, #16
	bhi	.L713
	movs	r2, #1
	movw	r1, #13260
	lsl	r3, r2, r3
	tst	r3, r1
	bne	.L727
	ldr	r1, .L744+20
	tst	r3, r1
	bne	.L714
.L713:
	movw	r3, #403
	ldr	r2, .L744+24
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L744+28
	movs	r0, #4
	bl	vlog
.L727:
	movs	r2, #0
.L714:
	movs	r3, #140
	mla	r3, r3, r6, r5
	str	r2, [r3, #104]
	ldr	r2, [r4, #40]
	str	r2, [r3, #96]
	ldr	r2, [r4, #36]
	str	r2, [r3, #100]
	ldr	r3, [r3, #20]
	cmp	r3, #17
	bne	.L715
	add	r1, r4, #64
	mov	r0, r8
	bl	set_2bpp_color_lut
.L715:
	ldr	r3, [r4, #88]
	cbz	r3, .L717
	ldr	r3, [r4, #16]
	cbz	r3, .L717
	sub	r0, r7, #84
	mov	r1, r4
	add	r0, r0, r5
	bl	to_osd_invert_cfg
	movs	r3, #140
	mla	r3, r3, r6, r5
	ldr	r0, [r3, #88]
	cbz	r0, .L717
	bl	mpi_obj_ref
.L717:
	ldr	r3, [r4, #48]
	cbz	r3, .L719
	movs	r3, #140
	movs	r2, #1
	mla	r6, r3, r6, r5
	movs	r1, #51
	movs	r3, #0
	str	r2, [r6, #108]
	strd	r1, r3, [r6, #120]
	ldr	r3, [r4, #60]
	str	r3, [r6, #116]
	ldr	r3, [r4, #52]
	cmp	r3, #0
	it	ne
	movne	r3, r2
	str	r3, [r6, #112]
.L719:
	mov	r6, r10
.L706:
	ldr	r4, [r4, #4]
	subs	r4, r4, #4
	b	.L705
.L724:
	mov	r1, r2
	b	.L710
.L726:
	mov	r1, r2
	b	.L712
.L745:
	.align	2
.L744:
	.word	.LANCHOR0
	.word	72074
	.word	.LC47
	.word	81664
	.word	.LC48
	.word	68611
	.word	.LC49
	.word	.LANCHOR0+760
	.fnend
	.size	set_osd_data, .-set_osd_data
	.align	1
	.global	mpi_buf_ref_with_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_ref_with_node, %function
mpi_buf_ref_with_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4, r5, r6, r7}
	.save {r4, r5, r6, r7}
	cbz	r0, .L746
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r6, .L752
	movs	r2, #0
	movs	r3, #12
.L750:
	mul	r4, r3, r2
	ldr	r7, [r4, r6]
	cbnz	r7, .L748
	ldr	r3, .L752+4
	movw	r2, #37024
	add	r3, r3, r4
	str	r0, [r3, r2]
	movw	r2, #37032
	ldr	r1, [r1, #76]
	str	r1, [r3, r2]
	movw	r1, #37028
	ldr	r2, [r3, r1]
	adds	r2, r2, #1
	str	r2, [r3, r1]
.L749:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r4, r5, r6, r7}
	b	mpi_obj_ref
.L748:
	adds	r2, r2, #1
	cmp	r2, #512
	bne	.L750
	b	.L749
.L746:
	pop	{r4, r5, r6, r7}
	bx	lr
.L753:
	.align	2
.L752:
	.word	g_venc_dev+37024
	.word	g_venc_dev
	.fnend
	.size	mpi_buf_ref_with_node, .-mpi_buf_ref_with_node
	.align	1
	.global	vcodec_handle_message
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_handle_message, %function
vcodec_handle_message:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r8, r0
	ldr	r3, [r0, #8]
	.pad #36
	sub	sp, sp, #36
	mov	r4, r1
	mov	r5, r2
	cmp	r3, #1
	beq	.L790
.L825:
	movs	r4, #0
.L801:
	mov	r0, r4
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L790:
	movs	r6, #0
.L755:
	mov	r0, r6
	bl	get_venc_node
	cmp	r8, r0
	beq	.L757
	adds	r6, r6, #1
	cmp	r6, #16
	bne	.L755
.L823:
	mov	r3, #716
	mov	r2, #-1
	strd	r3, r2, [sp]
	ldr	r3, .L827+8
	ldr	r2, .L827+12
.L826:
	movs	r1, #2
	movs	r0, #4
	bl	vlog
	b	.L825
.L757:
	cmp	r6, #15
	bhi	.L823
	ldr	r3, [r8, #288]
	ldr	r10, [r3]
	cmp	r10, #0
	bne	.L759
	movw	r3, #722
	str	r6, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L827+16
	ldr	r3, .L827+8
	b	.L826
.L759:
	cmp	r4, #1
	beq	.L760
	subs	r3, r4, #7
	rsbs	r4, r3, #0
	adcs	r4, r4, r3
	b	.L801
.L760:
	cmp	r5, #0
	beq	.L825
	ldr	r7, .L827+20
	add	fp, r6, #52
	add	fp, r7, fp, lsl #2
	ldr	r0, [fp, #4]
	cbz	r0, .L762
	add	r9, r6, #68
	add	r9, r7, r9, lsl #2
	ldr	r1, [r9, #4]
	cbz	r1, .L762
	add	r3, r8, #144
	add	r2, r8, #152
	bl	mpi_node_set_frame_rate
	movs	r3, #0
	str	r3, [fp, #4]
	str	r3, [r9, #4]
.L762:
	add	r9, r6, #100
	add	r3, r7, r9, lsl #2
	ldr	r3, [r3, #4]
	cbz	r3, .L763
	add	fp, r6, #312
	ldr	r2, [r5, #668]
	ldr	r3, [r7, fp, lsl #2]
	cmp	r3, r2
	beq	.L764
	movs	r1, #4
	strd	r3, r2, [sp, #4]
	mov	r0, r1
	movw	r3, #746
	ldr	r2, .L827+24
	str	r3, [sp]
	ldr	r3, .L827+8
	bl	vlog
.L764:
	ldr	r3, [r5, #668]
	adds	r3, r3, #1
	str	r3, [r7, fp, lsl #2]
.L763:
	add	r3, r6, #276
	add	r3, r7, r3, lsl #2
	ldr	r2, [r3, #4]
	cmp	r2, #0
	ble	.L765
	subs	r2, r2, #1
	str	r2, [r3, #4]
	b	.L825
.L765:
	add	r3, r6, #8960
	adds	r3, r3, #40
	ldr	r3, [r7, r3, lsl #2]
	cmp	r3, #9
	beq	.L766
	cmp	r3, #15
	bne	.L767
.L766:
	add	r3, r7, r6, lsl #4
	ldr	r2, [r3, #2152]
	cmp	r2, #0
	beq	.L825
	ble	.L767
	ldr	r2, [r3, #2156]
	cmp	r2, #0
	bne	.L825
	movs	r2, #1
	str	r2, [r3, #2156]
.L767:
	adds	r3, r7, r6
	add	r3, r3, #10304
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L768
	ldr	r3, .L827+28
	movs	r2, #0
.L770:
	ldrb	r1, [r3], #1	@ zero_extendqisi2
	cmp	r6, r1
	beq	.L769
	adds	r2, r2, #1
	cmp	r2, #16
	bne	.L770
	b	.L825
.L771:
	add	lr, lr, #1
	cmp	lr, #100
	bne	.L772
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, ip	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L825
.L775:
	str	r5, [r7, r3, lsl #2]
	b	.L777
.L774:
	ldrd	r2, [r5, #672]
	movs	r1, #4
	mov	r0, r1
	mov	r4, #-1
	strd	r2, [sp, #8]
	movw	r3, #793
	ldr	r2, .L827+32
	str	r3, [sp]
	ldr	r3, .L827+8
	bl	vlog
	mov	r0, r5
	bl	mpi_buf_unref
	b	.L801
.L781:
	bl	mcu_wrap_start
	movs	r3, #1
	str	r3, [r8, #4]
	movs	r1, #4
	ldr	r3, [r5, #668]
	mov	r0, r1
	ldr	r2, .L827+36
	str	r3, [sp, #24]
	ldr	r3, [r5, #632]
	str	r3, [sp, #20]
	ldr	r3, [r5, #628]
	str	r3, [sp, #16]
	ldr	r3, [r5, #624]
	str	r3, [sp, #12]
	ldr	r3, [r5, #620]
	strd	r6, r3, [sp, #4]
	movw	r3, #807
	str	r3, [sp]
	ldr	r3, .L827+8
	bl	vlog
	ldr	r3, [r9, #4]
	cmp	r3, #0
	beq	.L801
	b	.L783
.L784:
	add	r6, r7, r6, lsl #2
	movs	r2, #64
	mov	r1, r8
	ldr	r0, .L827+40
	bl	memset
	mov	r3, #-1
	str	r3, [r6, #4]
	bl	ktime_get
	cmp	r0, #0
	mov	r6, r0
	sbcs	r3, r1, #0
	mov	r7, r1
	bge	.L786
	rsbs	r6, r0, #0
	sbc	r7, r1, r1, lsl #1
.L786:
	adr	r9, .L827
	ldrd	r8, [r9]
	mov	ip, #0
	.syntax unified
@ 94 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/div64.h" 1
	umull	r2, r3, r8, r6
	cmn	r2, r8
	adcs	r3, r3, r9
	adc	r2, ip, #0
@ 0 "" 2
@ 112 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/div64.h" 1
	umlal	r3, r2, r9, r6
	umlal	r3, ip, r8, r7
	mov	r3, #0
	adds	r2, ip, r2
	adc	r3, r3, #0
	umlal	r2, r3, r9, r7
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r0, #0
	lsr	r6, r2, #9
	orr	r6, r6, r3, lsl #23
	lsr	r7, r3, #9
	sbcs	r3, r1, #0
	bge	.L787
	rsbs	r6, r6, #0
	sbc	r7, r7, r7, lsl #1
.L787:
	ldr	r3, .L827+44
	strd	r6, [r3]
	b	.L785
.L828:
	.align	3
.L827:
	.word	-1924145349
	.word	-2095944041
	.word	.LANCHOR0+780
	.word	.LC31
	.word	.LC50
	.word	g_venc_dev
	.word	.LC51
	.word	g_venc_dev+10320
	.word	.LC53
	.word	.LC54
	.word	g_venc_dev+1172
	.word	g_venc_dev+1240
.L769:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	ip, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, #800
	mov	lr, #0
	mla	r3, r3, r2, r7
	add	r3, r3, #23168
	adds	r3, r3, #32
.L772:
	ldrd	r0, [r3], #8
	orrs	fp, r0, r1
	beq	.L771
	movs	r3, #100
	strd	r0, [r5, #672]
	mla	r3, r3, r2, lr
	movs	r0, #0
	movs	r1, #0
	addw	r2, r3, #1300
	addw	r3, r3, #2900
	add	r2, r7, r2, lsl #3
	add	r3, r7, r3, lsl #3
	strd	r0, [r2]
	strd	r0, [r3]
.L768:
	ldr	r3, [r10, #16]
	movs	r1, #5
	ldr	r2, .L829
	movs	r0, #4
	str	r3, [sp, #16]
	ldr	r3, [r5]
	str	r3, [sp, #12]
	ldr	r3, [r8]
	str	r3, [sp, #8]
	ldr	r3, [r8, #44]
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	movw	r3, #771
	str	r3, [sp]
	ldr	r3, .L829+4
	bl	vlog
	add	r3, r6, #10752
	adds	r3, r3, #40
	ldr	r2, [r7, r3, lsl #2]
	adds	r2, r2, #1
	bne	.L789
	ldr	r2, [r5, #592]
	str	r2, [r7, r3, lsl #2]
.L789:
	mov	r1, r8
	mov	r0, r5
	mov	r2, r6
	bl	mpi_buf_ref_with_node
	mov	r1, r5
	mov	r0, r10
	bl	mpi_queue_push_obj
	cbnz	r0, .L773
	add	r3, r7, r9, lsl #2
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq	.L774
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r3, r6, #392
	ldr	r2, [r7, r3, lsl #2]
	cmp	r2, #0
	beq	.L775
	ldr	r2, .L829+8
	ldr	r3, [r2]
	cmp	r3, #19
	add	r1, r3, #1
	str	r1, [r2]
	bgt	.L776
	ldr	r3, [r5, #668]
	movs	r1, #2
	ldr	r2, .L829+12
	movs	r0, #4
	strd	r6, r3, [sp, #4]
	movw	r3, #783
	str	r3, [sp]
	ldr	r3, .L829+4
	bl	vlog
.L776:
	mov	r0, r5
	mov	r4, #-1
	bl	mpi_buf_unref
.L777:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L773:
	add	r8, r6, #260
	add	r10, r7, r8, lsl #2
	ldr	r3, [r10, #4]
	cbz	r3, .L779
	bl	mcu_get_wrap_status
	cmp	r0, #2
	itt	ne
	movne	r3, #0
	strne	r3, [r10, #4]
.L779:
	add	r9, r7, r9, lsl #2
	ldr	r3, [r9, #4]
	cmp	r3, #0
	beq	.L801
	add	r8, r7, r8, lsl #2
	ldr	r3, [r8, #4]
	cmp	r3, #0
	beq	.L781
.L783:
	bl	mcu_get_wrap_status
	cmp	r0, #2
	bne	.L801
	ldr	r3, [r5, #668]
	mov	r8, #0
	mov	r0, r6
	strd	r3, r8, [sp]
	ldrd	r2, [r5, #672]
	bl	mpp_vcodec_run_task
	add	r1, r6, #164
	ldrd	r2, [r5, #672]
	add	r1, r7, r1, lsl #3
	strd	r2, [r1]
	add	r3, r6, #180
	ldr	r1, [r5, #668]
	add	r2, r7, r3, lsl #3
	str	r1, [r7, r3, lsl #3]
	str	r8, [r2, #4]
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r6, r6, #292
	cmp	r0, #0
	bne	.L784
	add	r6, r7, r6, lsl #2
	movs	r3, #1
	str	r3, [r6, #4]
	movw	r3, #43232
	str	r0, [r7, r3]
.L785:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L801
.L830:
	.align	2
.L829:
	.word	.LC55
	.word	.LANCHOR0+780
	.word	.LANCHOR1
	.word	.LC52
	.fnend
	.size	vcodec_handle_message, .-vcodec_handle_message
	.global	__aeabi_idiv
	.align	1
	.global	to_frame_pixel_stride
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	to_frame_pixel_stride, %function
to_frame_pixel_stride:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r3, r0
	cmp	r3, #65537
	mov	r0, r1
	bhi	.L832
	cmp	r3, #65536
	bcs	.L835
	cmp	r3, #8
	beq	.L835
	cmp	r3, #10
	ite	eq
	moveq	r1, #2
	movne	r1, #1
.L833:
	bl	__aeabi_idiv
	pop	{r3, pc}
.L832:
	ldr	r2, .L840
	cmp	r3, r2
	bcc	.L837
	adds	r2, r2, #1
	cmp	r3, r2
	bls	.L838
	sub	r3, r3, #65536
	subs	r3, r3, #10
	cmp	r3, #3
	ite	hi
	movhi	r1, #1
	movls	r1, #4
	b	.L833
.L835:
	movs	r1, #2
	b	.L833
.L837:
	movs	r1, #1
	b	.L833
.L838:
	movs	r1, #3
	b	.L833
.L841:
	.align	2
.L840:
	.word	65542
	.fnend
	.size	to_frame_pixel_stride, .-to_frame_pixel_stride
	.align	1
	.global	dma_buf_import_mpi_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	dma_buf_import_mpi_buf, %function
dma_buf_import_mpi_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r5, r1
	movs	r1, #0
	mov	r6, r2
	bl	mpi_buf_create
	mov	r4, r0
	cmp	r0, #0
	beq	.L842
	ldr	r3, .L855
	str	r3, [r0, #4]
	ldr	r3, [r5]
	str	r3, [r0, #620]
	ldr	r3, [r5, #4]
	str	r3, [r0, #624]
	ldr	r1, [r5, #8]
	ldr	r0, [r5, #28]
	bl	to_frame_pixel_stride
	ldr	r2, .L855+4
	add	r7, r2, #808
	str	r0, [r4, #628]
	ldr	r3, [r5, #12]
	str	r3, [r4, #632]
	movs	r3, #0
	ldr	r0, [r5, #28]
.L846:
	lsls	r1, r3, #3
	add	ip, r1, r7
	ldr	ip, [ip, #4]
	cmp	r0, ip
	bne	.L844
	add	r2, r2, r1
	ldr	r3, [r2, #808]
.L845:
	str	r3, [r4, #640]
	ldrd	r2, [r5, #40]
	strd	r2, [r4, #672]
	movs	r3, #4
	str	r3, [r4, #584]
	ldrb	r3, [r5, #96]	@ zero_extendqisi2
	lsls	r3, r3, #31
	itt	mi
	movmi	r3, #12
	strmi	r3, [r4, #584]
	cmp	r6, #0
	blt	.L842
	ldr	r2, [r5, #48]
	cmp	r2, #15
	bhi	.L842
	ldr	r3, .L855+8
	movs	r1, #1
	add	r2, r3, r2, lsl #4
	str	r1, [r2, #2144]
	ldr	r2, [r5, #48]
	add	r2, r3, r2, lsl #4
	str	r6, [r2, #2148]
	ldr	r2, [r5, #48]
	add	r3, r3, r2, lsl #4
	str	r1, [r3, #2152]
.L842:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L844:
	adds	r3, r3, #1
	cmp	r3, #31
	bne	.L846
	mov	r3, #-1
	b	.L845
.L856:
	.align	2
.L855:
	.word	.LC8
	.word	.LANCHOR0
	.word	g_venc_dev
	.fnend
	.size	dma_buf_import_mpi_buf, .-dma_buf_import_mpi_buf
	.align	1
	.global	to_frame_hor_stride
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	to_frame_hor_stride, %function
to_frame_hor_stride:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #65537
	bhi	.L858
	cmp	r0, #65536
	bcs	.L860
	subs	r0, r0, #9
	cmp	r0, #1
	ite	hi
	movhi	r0, #1
	movls	r0, #2
.L859:
	muls	r0, r1, r0
	bx	lr
.L858:
	ldr	r3, .L865
	cmp	r0, r3
	bcc	.L862
	adds	r3, r3, #1
	cmp	r0, r3
	bls	.L863
	sub	r0, r0, #65536
	subs	r0, r0, #16
	cmp	r0, #3
	ite	hi
	movhi	r0, #1
	movls	r0, #4
	b	.L859
.L860:
	movs	r0, #2
	b	.L859
.L862:
	movs	r0, #1
	b	.L859
.L863:
	movs	r0, #3
	b	.L859
.L866:
	.align	2
.L865:
	.word	65542
	.fnend
	.size	to_frame_hor_stride, .-to_frame_hor_stride
	.align	1
	.global	get_mpi_buf_frm_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_mpi_buf_frm_info, %function
get_mpi_buf_frm_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L950
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #24
	sub	sp, sp, #24
	mov	r7, r0
	mov	r4, r1
	mov	r6, r2
	mov	r8, #0
	mov	r5, r3
.L870:
	ldr	r2, [r3, #2144]
	cbz	r2, .L868
	ldr	r2, [r3, #2148]
	cmp	r6, r2
	bne	.L868
	add	r3, r5, r8, lsl #4
	ldr	r3, [r3, #2152]
	adds	r2, r3, #1
	beq	.L869
	cmp	r3, #0
	it	le
	movle	r8, #-1
.L869:
	cbnz	r7, .L871
.L949:
	movs	r0, #0
.L867:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L868:
	add	r8, r8, #1
	adds	r3, r3, #16
	cmp	r8, #16
	bne	.L870
	mov	r8, #-1
	b	.L869
.L871:
	cmp	r4, #0
	beq	.L949
	ldr	r3, [r7, #620]
	str	r3, [r4]
	ldr	r3, [r7, #624]
	str	r3, [r4, #4]
	ldr	r1, [r7, #628]
	ldr	r0, [r7, #640]
	bl	to_frame_hor_stride
	ldrb	r2, [r4, #96]	@ zero_extendqisi2
	cmp	r6, #0
	str	r0, [r4, #8]
	ldr	r3, [r7, #632]
	str	r3, [r4, #12]
	ldr	r3, [r7, #584]
	asr	r3, r3, #3
	bfi	r2, r3, #0, #1
	strb	r2, [r4, #96]
	blt	.L875
	ldrd	r2, [r7, #672]
	strd	r2, [r4, #40]
	ldr	r3, [r7, #668]
	str	r3, [r4, #80]
	movs	r3, #0
	str	r3, [r4, #84]
	add	r3, r5, r6, lsl #2
	ldr	r3, [r3, #404]
	cbz	r3, .L875
	ldrd	r0, [r7, #672]
	ldr	r3, [r7, #668]
	orrs	r2, r0, r1
	bne	.L877
	cbz	r3, .L878
.L877:
	adds	r3, r3, #1
	movs	r1, #0
	mov	r0, r3
.L878:
	strd	r0, [r4, #80]
	ldrd	r2, [r7, #672]
	orrs	r3, r2, r3
	bne	.L879
	ldr	r3, [r7, #668]
	cmp	r3, #0
	bne	.L879
.L882:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r10, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r9, r6, #392
	ldr	r3, [r5, r9, lsl #2]
	cmp	r3, #0
	bne	.L880
.L881:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r10	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L875:
	cmp	r6, #15
	mov	r3, #0
	strd	r3, r3, [r4, #20]
	bhi	.L884
	movs	r3, #24
	mla	r3, r3, r6, r5
	ldr	r2, [r3, #1760]
	cmp	r2, r6
	itttt	eq
	ldreq	r2, [r3, #1764]
	streq	r2, [r4, #20]
	ldreq	r3, [r3, #1768]
	streq	r3, [r4, #24]
.L884:
	add	r9, r6, #204
	movs	r3, #0
	str	r8, [r4, #48]
	str	r3, [r4, #52]
	str	r3, [r4, #60]
	ldr	r10, [r5, r9, lsl #3]
	cmp	r10, #0
	beq	.L885
	mov	r0, r6
	bl	get_venc_node
	mov	r1, r0
	cmp	r0, #0
	bne	.L886
.L887:
	movs	r3, #0
	str	r3, [r4, #52]
.L885:
	cmp	r8, #-1
	beq	.L888
	add	r9, r5, r9, lsl #3
	ldr	r9, [r9, #4]
	cmp	r9, #0
	beq	.L888
	mov	r0, r8
	bl	get_venc_node
	mov	r1, r0
	cmp	r0, #0
	bne	.L889
.L890:
	movs	r3, #0
	str	r3, [r4, #60]
.L888:
	ldr	r3, [r7]
	ldr	r2, .L950+4
	str	r3, [r4, #56]
	add	ip, r2, #808
	ldr	r1, [r7, #640]
	movs	r3, #0
.L893:
	ldr	r0, [ip, r3, lsl #3]
	cmp	r1, r0
	bne	.L891
	add	r3, r2, r3, lsl #3
	ldr	r3, [r3, #812]
.L892:
	str	r3, [r4, #28]
	add	r3, r5, r6, lsl #2
	ldr	r3, [r3, #404]
	cmp	r3, #0
	bne	.L905
	ldr	r3, [r7, #584]
	tst	r3, #4
	ite	eq
	moveq	r3, #1
	movne	r3, #0
.L894:
	cmp	r6, #7
	str	r3, [r4, #64]
	mov	r3, #0
	str	r3, [r4, #88]
	bhi	.L949
	mov	r3, #988
	mla	r3, r3, r6, r5
	ldr	r2, [r3, #2408]
	cbnz	r2, .L895
	ldr	r3, [r3, #2412]
	cbz	r3, .L896
.L895:
	movw	r1, #5500
	movw	r0, #4500
	bl	usleep_range
.L896:
	mov	r8, #988
	movs	r2, #112
	mul	r8, r8, r6
	movs	r1, #0
	addw	r9, r8, #3140
	add	r0, r5, r9
	bl	memset
	add	r3, r5, r8
	ldr	r2, [r3, #2408]
	cbz	r2, .L897
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r0, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r7, #668]
	ldr	r2, [r3, #3128]
	cmp	r1, r2
	beq	.L906
	ldr	r3, [r3, #3136]
	cmp	r1, r3
	bne	.L899
	movs	r1, #1
.L898:
	mov	r3, #988
	add	r2, r1, #90
	muls	r3, r6, r3
	add	r2, r3, r2, lsl #3
	add	r2, r2, r5
	ldr	r2, [r2, #2404]
	ldr	lr, [r2, #448]
	movs	r2, #56
	muls	r2, r1, r2
	add	ip, r2, r3
	add	r3, r3, r5
	add	ip, ip, r5
	add	r2, r2, r9
	add	r2, r2, r5
	str	lr, [ip, #3140]
	str	r2, [r4, #88]
	str	r1, [r3, #3252]
.L899:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r0	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L897:
	mov	r3, #988
	mla	r3, r3, r6, r5
	ldr	r3, [r3, #2412]
	cmp	r3, #0
	beq	.L949
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r7, #668]
	add	r8, r8, r5
	movs	r0, #0
.L902:
	ldr	r3, [r8, #2648]
	cmp	r1, r3
	bne	.L900
	mov	r1, #988
	muls	r1, r6, r1
	adds	r3, r5, r1
	ldr	r6, [r3, #3252]
	movs	r3, #56
	muls	r3, r6, r3
	movs	r6, #52
	mla	r6, r6, r0, r1
	adds	r7, r3, r1
	add	r3, r3, r9
	add	r7, r7, r5
	add	r6, r6, r5
	add	r5, r5, r3
	ldr	r1, [r6, #2600]
	str	r1, [r7, #3148]
	str	r5, [r4, #88]
.L901:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L949
.L879:
	bl	mcu_get_wrap_status
	cmp	r0, #2
	beq	.L882
	mov	r0, #-1
	b	.L867
.L880:
	mov	r0, r6
	bl	get_venc_node
	ldr	r1, [r5, r9, lsl #2]
	ldr	r3, [r0, #288]
	ldr	r0, [r3]
	bl	mpi_queue_push_obj
	cbz	r0, .L883
	movs	r3, #0
	str	r3, [r5, r9, lsl #2]
	b	.L881
.L883:
	ldr	r1, [r5, r9, lsl #2]
	movs	r0, #4
	ldrd	r2, [r1, #672]
	strd	r2, [sp, #16]
	ldr	r3, [r1, #668]
	movs	r1, #2
	ldr	r2, .L950+8
	strd	r6, r3, [sp, #4]
	movw	r3, #1414
	str	r3, [sp]
	ldr	r3, .L950+12
	bl	vlog
	b	.L881
.L886:
	mov	r0, r10
	bl	set_osd_data
	ldr	r3, [r10, #4]
	cmp	r3, #0
	beq	.L887
	str	r10, [r4, #52]
	b	.L885
.L889:
	mov	r0, r9
	bl	set_osd_data
	ldr	r3, [r9, #4]
	cmp	r3, #0
	beq	.L890
	str	r9, [r4, #60]
	b	.L888
.L951:
	.align	2
.L950:
	.word	g_venc_dev
	.word	.LANCHOR0
	.word	.LC56
	.word	.LANCHOR0+1056
.L891:
	adds	r3, r3, #1
	cmp	r3, #31
	bne	.L893
	mov	r3, #-1
	b	.L892
.L905:
	movs	r3, #0
	b	.L894
.L906:
	movs	r1, #0
	b	.L898
.L900:
	adds	r0, r0, #1
	add	r8, r8, #52
	cmp	r0, #10
	bne	.L902
	b	.L901
	.fnend
	.size	get_mpi_buf_frm_info, .-get_mpi_buf_frm_info
	.align	1
	.global	venc_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_probe, %function
venc_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	movw	r2, #43240
	movs	r1, #0
	mov	r7, r0
	ldr	r0, .L958
	movs	r6, #0
	bl	memset
	movs	r2, #16
	mov	r1, #-1
	ldr	r0, .L958+4
	bl	memset
	ldr	r5, .L958
	ldr	r8, .L958+44
	ldr	r9, .L958+52
	mov	r4, r5
.L955:
	mov	r0, #1128
	bl	kzalloc.constprop.16
	str	r0, [r5, #1632]
	cbnz	r0, .L953
	movw	r3, #2567
	mov	r2, r9
	str	r3, [sp]
	movs	r1, #2
	mov	r3, r8
	movs	r0, #4
	bl	vlog
.L953:
	mov	r0, #1128
	bl	kzalloc.constprop.16
	str	r0, [r5, #1636]
	cbnz	r0, .L954
	movw	r3, #2570
	ldr	r2, .L958+8
	str	r3, [sp]
	movs	r1, #2
	mov	r3, r8
	movs	r0, #4
	bl	vlog
.L954:
	adds	r6, r6, #1
	adds	r5, r5, #8
	cmp	r6, #16
	bne	.L955
	mov	r1, #3264
	mov	r0, r7
	bl	kstrdup
	ldr	r5, .L958+12
	mov	r2, r7
	movs	r1, #8
	str	r4, [r4, #164]
	movs	r3, #24
	str	r5, [r4, #188]
	str	r3, [r4, #192]
	str	r0, [r4]
	ldr	r0, .L958+16
	bl	mpi_dev_register
	ldr	r3, .L958+20
	add	r1, r5, #480
	movs	r2, #0
	ldr	r0, .L958+16
	str	r3, [r4, #72]
	ldr	r3, .L958+24
	str	r3, [r4, #80]
	ldr	r3, .L958+28
	str	r3, [r4, #84]
	bl	mpi_sysfs_dev_add
	ldr	r1, .L958+32
	movs	r3, #0
	ldr	r0, .L958+36
	add	r2, r1, #28
	str	r3, [r4, #196]
	str	r1, [r4, #172]
	str	r1, [r4, #176]
	subs	r1, r1, #172
	strd	r2, r2, [r4, #200]
	mov	r2, #-1
	str	r3, [r4, #180]
	ldr	r3, .L958+40
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L956
	bl	wake_up_process
.L956:
	movs	r1, #4
	str	r5, [r4, #168]
	movw	r3, #2597
	str	r5, [sp, #4]
	str	r3, [sp]
	mov	r0, r1
	ldr	r3, .L958+44
	ldr	r2, .L958+48
	bl	vlog
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L959:
	.align	2
.L958:
	.word	g_venc_dev
	.word	g_venc_dev+10320
	.word	.LC58
	.word	.LANCHOR0+1088
	.word	g_venc_dev+4
	.word	venc_create_ctx
	.word	venc_destroy_ctx
	.word	venc_handle_message
	.word	g_venc_dev+172
	.word	venc_thread_fn
	.word	.LC8
	.word	.LANCHOR0+1077
	.word	.LC59
	.word	.LC57
	.fnend
	.size	venc_probe, .-venc_probe
	.align	1
	.global	venc_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_shutdown, %function
venc_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	ldr	r4, .L975
	ldr	r3, [r4, #168]
	cbz	r3, .L961
	cmn	r3, #4096
	bhi	.L961
	add	r0, r4, #196
	movs	r3, #1
	str	r3, [r4, #180]
	bl	up
	ldr	r3, [r4, #168]
	movs	r1, #4
	ldr	r2, .L975+4
	mov	r0, r1
	str	r3, [sp, #4]
	movw	r3, #2609
	str	r3, [sp]
	ldr	r3, .L975+8
	bl	vlog
	ldr	r0, [r4, #168]
	bl	kthread_stop
	ldr	r3, [r4, #168]
	movs	r1, #4
	ldr	r2, .L975+12
	mov	r0, r1
	str	r3, [sp, #4]
	movw	r3, #2611
	str	r3, [sp]
	ldr	r3, .L975+8
	bl	vlog
	movs	r3, #0
	str	r3, [r4, #168]
.L961:
	ldr	r4, .L975+16
	movs	r5, #0
	mov	r6, r5
	mov	r0, r4
	bl	mpi_sysfs_dev_remove
	mov	r0, r4
	bl	mpi_dev_unregister
	ldr	r0, [r4, #-4]!
	bl	kfree
.L964:
	ldr	r0, [r4, #1632]
	cbz	r0, .L962
	bl	kfree
	str	r6, [r4, #1632]
.L962:
	ldr	r0, [r4, #1636]
	cbz	r0, .L963
	bl	kfree
	str	r6, [r4, #1636]
.L963:
	adds	r5, r5, #1
	adds	r4, r4, #8
	cmp	r5, #16
	bne	.L964
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L976:
	.align	2
.L975:
	.word	g_venc_dev
	.word	.LC60
	.word	.LANCHOR0+1696
	.word	.LC61
	.word	g_venc_dev+4
	.fnend
	.size	venc_shutdown, .-venc_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	venc_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_init, %function
venc_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L978
	ldr	r0, .L978+4
	bl	venc_probe
	add	r0, r4, #72
	mov	r1, r4
	bl	vmpi_register_fn2vcocdec
	mov	r0, r4
	bl	register_vmpibuf_func_to_pp
	movs	r0, #0
	pop	{r4, pc}
.L979:
	.align	2
.L978:
	.word	.LANCHOR2
	.word	.LC8
	.fnend
	.size	venc_init, .-venc_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	venc_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_exit, %function
venc_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	vmpi_unregister_fn2vcocdec
	bl	unregister_vmpibuf_func_pp
	pop	{r3, lr}
	b	venc_shutdown
	.fnend
	.size	venc_exit, .-venc_exit
	.global	gmpi_buf_ops
	.global	gmpidev_ops
	.global	g_venc_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.37358, %object
	.size	__func__.37358, 25
__func__.37358:
	.ascii	"venc_node_pp_get_md_attr\000"
	.type	__func__.37365, %object
	.size	__func__.37365, 20
__func__.37365:
	.ascii	"venc_node_enable_tb\000"
	.type	__func__.37372, %object
	.size	__func__.37372, 18
__func__.37372:
	.ascii	"venc_node_tb_bind\000"
	.type	__func__.37380, %object
	.size	__func__.37380, 21
__func__.37380:
	.ascii	"venc_node_tb_request\000"
	.type	__func__.36857, %object
	.size	__func__.36857, 22
__func__.36857:
	.ascii	"vcodec_set_intra_info\000"
	.type	__func__.37264, %object
	.size	__func__.37264, 24
__func__.37264:
	.ascii	"venc_node_set_framerate\000"
	.type	__func__.36981, %object
	.size	__func__.36981, 14
__func__.36981:
	.ascii	"mpi_buf_unref\000"
	.type	__func__.37156, %object
	.size	__func__.37156, 17
__func__.37156:
	.ascii	"venc_node_create\000"
	.type	__func__.37166, %object
	.size	__func__.37166, 17
__func__.37166:
	.ascii	"venc_node_delete\000"
	.type	__func__.37312, %object
	.size	__func__.37312, 19
__func__.37312:
	.ascii	"venc_node_pp_start\000"
	.type	__func__.37183, %object
	.size	__func__.37183, 16
__func__.37183:
	.ascii	"venc_node_start\000"
	.type	__func__.36739, %object
	.size	__func__.36739, 15
__func__.36739:
	.ascii	"send_first_buf\000"
	.type	__func__.37225, %object
	.size	__func__.37225, 26
__func__.37225:
	.ascii	"venc_node_set_vcodec_attr\000"
	.type	__func__.37233, %object
	.size	__func__.37233, 26
__func__.37233:
	.ascii	"venc_node_get_vcodec_attr\000"
	.type	__func__.37190, %object
	.size	__func__.37190, 15
__func__.37190:
	.ascii	"venc_node_stop\000"
	.type	__func__.37201, %object
	.size	__func__.37201, 21
__func__.37201:
	.ascii	"venc_node_send_frame\000"
	.type	__func__.37323, %object
	.size	__func__.37323, 24
__func__.37323:
	.ascii	"venc_node_pp_send_frame\000"
	.type	__func__.37280, %object
	.size	__func__.37280, 20
__func__.37280:
	.ascii	"venc_node_pp_create\000"
	.type	__func__.37300, %object
	.size	__func__.37300, 20
__func__.37300:
	.ascii	"venc_node_pp_delete\000"
	.type	__func__.37455, %object
	.size	__func__.37455, 13
__func__.37455:
	.ascii	"venc_release\000"
	.type	__func__.37399, %object
	.size	__func__.37399, 11
__func__.37399:
	.ascii	"venc_ioctl\000"
	.type	__func__.36796, %object
	.size	__func__.36796, 28
__func__.36796:
	.ascii	"venc_handle_node_pp_message\000"
	.type	__func__.36883, %object
	.size	__func__.36883, 23
__func__.36883:
	.ascii	"vcodec_notify_drop_frm\000"
	.type	__func__.37241, %object
	.size	__func__.37241, 20
__func__.37241:
	.ascii	"venc_node_set_combo\000"
	.type	__func__.37250, %object
	.size	__func__.37250, 19
__func__.37250:
	.ascii	"venc_node_set_snap\000"
	.type	__func__.36901, %object
	.size	__func__.36901, 14
__func__.36901:
	.ascii	"vcodec_notify\000"
	.type	__func__.37144, %object
	.size	__func__.37144, 16
__func__.37144:
	.ascii	"venc_node_check\000"
	.type	__func__.37137, %object
	.size	__func__.37137, 17
__func__.37137:
	.ascii	"venc_get_version\000"
	.type	__func__.37209, %object
	.size	__func__.37209, 19
__func__.37209:
	.ascii	"venc_node_set_crop\000"
	.type	__func__.37217, %object
	.size	__func__.37217, 19
__func__.37217:
	.ascii	"venc_node_get_crop\000"
	.type	__func__.37349, %object
	.size	__func__.37349, 25
__func__.37349:
	.ascii	"venc_node_pp_set_md_attr\000"
	.type	__func__.37440, %object
	.size	__func__.37440, 10
__func__.37440:
	.ascii	"venc_open\000"
	.type	__func__.37334, %object
	.size	__func__.37334, 24
__func__.37334:
	.ascii	"venc_node_pp_get_result\000"
	.type	__func__.37497, %object
	.size	__func__.37497, 15
__func__.37497:
	.ascii	"venc_thread_fn\000"
	.type	__func__.36623, %object
	.size	__func__.36623, 19
__func__.36623:
	.ascii	"set_2bpp_color_lut\000"
	.type	__func__.36674, %object
	.size	__func__.36674, 18
__func__.36674:
	.ascii	"to_osd_invert_cfg\000"
	.space	2
	.type	CSWTCH.368, %object
	.size	CSWTCH.368, 68
CSWTCH.368:
	.word	65551
	.word	65551
	.word	65550
	.word	65550
	.word	-1
	.word	-1
	.word	65546
	.word	65546
	.word	65546
	.word	65546
	.word	65551
	.word	65551
	.word	65550
	.word	65550
	.word	-1
	.word	-1
	.word	17
	.type	__func__.36610, %object
	.size	__func__.36610, 20
__func__.36610:
	.ascii	"to_osd_alpha_source\000"
	.type	__func__.36755, %object
	.size	__func__.36755, 25
__func__.36755:
	.ascii	"venc_handle_node_message\000"
	.space	3
	.type	fmt, %object
	.size	fmt, 248
fmt:
	.word	0
	.word	0
	.word	1
	.word	1
	.word	2
	.word	2
	.word	3
	.word	3
	.word	4
	.word	4
	.word	5
	.word	5
	.word	6
	.word	5
	.word	7
	.word	6
	.word	8
	.word	7
	.word	9
	.word	8
	.word	10
	.word	10
	.word	12
	.word	13
	.word	13
	.word	14
	.word	15
	.word	15
	.word	16
	.word	16
	.word	17
	.word	9
	.word	18
	.word	11
	.word	65536
	.word	65536
	.word	65537
	.word	65537
	.word	65538
	.word	65538
	.word	65539
	.word	65539
	.word	65540
	.word	65540
	.word	65541
	.word	65541
	.word	65542
	.word	65542
	.word	65543
	.word	65543
	.word	65544
	.word	65544
	.word	65545
	.word	65545
	.word	65552
	.word	65546
	.word	65553
	.word	65547
	.word	65554
	.word	65548
	.word	65555
	.word	65549
	.type	__func__.37095, %object
	.size	__func__.37095, 21
__func__.37095:
	.ascii	"get_mpi_buf_frm_info\000"
	.type	__func__.37581, %object
	.size	__func__.37581, 11
__func__.37581:
	.ascii	"venc_probe\000"
	.type	venc_ioctls, %object
	.size	venc_ioctls, 480
venc_ioctls:
	.word	-1068735232
	.word	0
	.word	venc_get_version
	.word	.LC62
	.word	0
	.word	-1072667391
	.word	0
	.word	venc_node_create
	.word	.LC63
	.word	0
	.word	1074029826
	.word	0
	.word	venc_node_delete
	.word	.LC64
	.word	0
	.word	1076126979
	.word	0
	.word	venc_node_start
	.word	.LC65
	.word	0
	.word	1079796996
	.word	0
	.word	venc_node_send_frame
	.word	.LC66
	.word	0
	.word	1075340549
	.word	0
	.word	venc_node_set_crop
	.word	.LC67
	.word	0
	.word	1074554118
	.word	0
	.word	venc_node_set_combo
	.word	.LC68
	.word	0
	.word	1074554119
	.word	0
	.word	venc_node_set_snap
	.word	.LC69
	.word	0
	.word	-1069259512
	.word	0
	.word	venc_node_pp_create
	.word	.LC70
	.word	0
	.word	1074029833
	.word	0
	.word	venc_node_pp_delete
	.word	.LC71
	.word	0
	.word	1074029834
	.word	0
	.word	venc_node_pp_start
	.word	.LC72
	.word	0
	.word	1079797003
	.word	0
	.word	venc_node_pp_send_frame
	.word	.LC73
	.word	0
	.word	-1071618804
	.word	0
	.word	venc_node_pp_get_result
	.word	.LC74
	.word	0
	.word	1074029837
	.word	0
	.word	venc_node_enable_tb
	.word	.LC75
	.word	0
	.word	1074291982
	.word	0
	.word	venc_node_tb_bind
	.word	.LC76
	.word	0
	.word	1074029839
	.word	0
	.word	venc_node_tb_request
	.word	.LC77
	.word	0
	.word	-1072143088
	.word	0
	.word	venc_node_get_crop
	.word	.LC78
	.word	0
	.word	1077699857
	.word	0
	.word	venc_node_set_vcodec_attr
	.word	.LC79
	.word	0
	.word	-1069783790
	.word	0
	.word	venc_node_get_vcodec_attr
	.word	.LC80
	.word	0
	.word	-1071356653
	.word	0
	.word	venc_node_pp_set_md_attr
	.word	.LC81
	.word	0
	.word	-1071356652
	.word	0
	.word	venc_node_pp_get_md_attr
	.word	.LC82
	.word	0
	.word	1074816277
	.word	0
	.word	venc_node_check
	.word	.LC83
	.word	0
	.word	1074816278
	.word	0
	.word	venc_node_set_framerate
	.word	.LC84
	.word	0
	.word	1074029847
	.word	0
	.word	venc_node_stop
	.word	.LC85
	.word	0
	.type	venc_driver_fops, %object
	.size	venc_driver_fops, 128
venc_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	venc_poll
	.word	venc_ioctl
	.word	venc_compat_ioctl
	.word	0
	.space	4
	.word	venc_open
	.space	4
	.word	venc_release
	.space	60
	.type	__func__.37602, %object
	.size	__func__.37602, 14
__func__.37602:
	.ascii	"venc_shutdown\000"
	.data
	.align	2
	.set	.LANCHOR2,. + 0
	.type	gmpi_buf_ops, %object
	.size	gmpi_buf_ops, 72
gmpi_buf_ops:
	.word	mpi_buf_alloc
	.word	mpi_buf_map
	.word	mpi_buf_unmap
	.word	mpi_buf_ref
	.word	mpi_buf_unref
	.word	mpi_buf_get_dmabuf
	.word	mpi_buf_get_paddr
	.word	mpi_buffer_queue_create
	.word	mpi_buffer_queue_destroy
	.word	mpi_buffer_queue_push
	.word	mpi_buffer_queue_pop
	.word	mpi_buffer_queue_size
	.word	dma_buf_import_mpi_buf
	.word	get_mpi_buf_frm_info
	.word	mpi_buffer_pool_create
	.word	mpi_buffer_pool_destroy
	.word	mpi_buffer_pool_request_buf
	.word	mpi_buffer_pool_get_free_num
	.type	gmpidev_ops, %object
	.size	gmpidev_ops, 36
gmpidev_ops:
	.word	vcodec_create_dev
	.word	vcodec_destory_dev
	.word	vcodec_handle_message
	.word	vcodec_get_chnl_ctx
	.word	vcodec_get_chnl_id
	.word	vcodec_get_chnl_type
	.word	vcodec_set_intra_info
	.word	vcodec_notify_drop_frm
	.word	vcodec_notify
	.bss
	.align	3
	.set	.LANCHOR1,. + 0
	.type	__key.37281, %object
	.size	__key.37281, 0
__key.37281:
	.type	__func____LINE__.36775, %object
	.size	__func____LINE__.36775, 4
__func____LINE__.36775:
	.space	4
	.space	4
	.type	g_venc_dev, %object
	.size	g_venc_dev, 43240
g_venc_dev:
	.space	43240
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[1;31mstruct venc_pp_md_attr mismatching\033[0m"
	.ascii	"\000"
.LC1:
	.ascii	"\033[1;31mchnId %d need less than %d\033[0m\000"
.LC2:
	.ascii	"\033[1;31mchnId %d and %d need less than %d\033[0m\000"
.LC3:
	.ascii	"\033[1;32mchn %d wrap trigger again\033[0m\000"
.LC4:
	.ascii	"\033[1;31mrequest thumbnail more than %d\033[0m\000"
.LC5:
	.ascii	"\033[1;31mstruct venc_framerate mismatching\033[0m\000"
.LC6:
	.ascii	"\033[1;32mchn %d vdev->fps_in %d, vdev->fps_out %d\012"
	.ascii	"\033[0m\000"
.LC7:
	.ascii	"valloc\000"
.LC8:
	.ascii	"venc\000"
.LC9:
	.ascii	"buf(%d)\000"
.LC10:
	.ascii	"rkv_enc\000"
.LC11:
	.ascii	"\033[1;31mstruct venc_info mismatching\033[0m\000"
.LC12:
	.ascii	"\033[1;31merror getting vdev  %p\033[0m\000"
.LC13:
	.ascii	"\033[1;31m====== error create venc node\033[0m\000"
.LC14:
	.ascii	"\0013%s[%d] struct venc_frame mismatching\000"
.LC15:
	.ascii	"ch = %d, warp enable = %d, wrap line = %d\000"
.LC16:
	.ascii	"\033[1;32msend first buf to venc %d, %d, %d ,%d\033"
	.ascii	"[0m\000"
.LC17:
	.ascii	"\033[1;31msend first buf to venc fail: queue is NUL"
	.ascii	"L!\033[0m\000"
.LC18:
	.ascii	"\033[1;31msend first buf to venc fail: push queue e"
	.ascii	"rror!\033[0m\000"
.LC19:
	.ascii	"\033[1;31mstruct venc_frame mismatching\033[0m\000"
.LC20:
	.ascii	"\033[1;31mchnId %d send frame fail: node %px, enc_d"
	.ascii	"ev %px, frameFd %d\033[0m\000"
.LC21:
	.ascii	"\033[1;31mchnId %d send frame fail: pp_node %px, en"
	.ascii	"c_dev %px, frameFd %d\033[0m\000"
.LC22:
	.ascii	"\033[1;31mstruct venc_pp_info mismatching\033[0m\000"
.LC23:
	.ascii	"\033[1;31mchnId %d is exist!\033[0m\000"
.LC24:
	.ascii	"&pp->wait\000"
.LC25:
	.ascii	"\033[1;31merror\033[0m\000"
.LC26:
	.ascii	"\033[1;31m====== error create venc pp node\033[0m\000"
.LC27:
	.ascii	"%s %d\000"
.LC28:
	.ascii	"======error: index = %d\000"
.LC29:
	.ascii	"======error: ioctl is null\000"
.LC30:
	.ascii	"======error venc ioctl\000"
.LC31:
	.ascii	"\033[1;31mchn %d error!\033[0m\000"
.LC32:
	.ascii	"venc_pp(%s-%d) buf(%d) ready, pending: %d\000"
.LC33:
	.ascii	"venc(%s-%d) drop buf(%d)\000"
.LC34:
	.ascii	"\033[1;31mstruct venc_combo mismatching\033[0m\000"
.LC35:
	.ascii	"\033[1;31mstruct venc_snap mismatching\033[0m\000"
.LC36:
	.ascii	"chn %d wrap trigger again, seq: %llu\000"
.LC37:
	.ascii	"\033[1;31mstruct venc_version mismatching\033[0m\000"
.LC38:
	.ascii	"\033[1;31mstruct venc_crop mismatching\033[0m\000"
.LC39:
	.ascii	"\033[1;31mdata alloc failed!\012\033[0m\000"
.LC40:
	.ascii	"\033[1;31mstruct venc_pp_result mismatching\033[0m\000"
.LC41:
	.ascii	"%s register ok = %p\000"
.LC42:
	.ascii	"\033[1;32mthread exit\012\033[0m\000"
.LC43:
	.ascii	"\033[1;31mvepu pp drop buf(%dx%d)\033[0m\000"
.LC44:
	.ascii	"rgb[0]:(%d %d %d) rgb[1]:(%d %d %d) ayuv[0]:(%d %d "
	.ascii	"%d %d) ayuv[1]:(%d %d %d %d)\000"
.LC45:
	.ascii	"Inverted cells do not support size\012\000"
.LC46:
	.ascii	"\033[1;31mcreate light lut failed!\033[0m\000"
.LC47:
	.ascii	"rbuv_swap not support\000"
.LC48:
	.ascii	"alpha_swap not support\000"
.LC49:
	.ascii	"\033[1;31merror format!\033[0m\000"
.LC50:
	.ascii	"\033[1;31mchn %d queue is NULL!\033[0m\000"
.LC51:
	.ascii	"\033[1;32mseq lost = %d, %d\033[0m\000"
.LC52:
	.ascii	"\033[1;31mchn %d wrap mode lost buf, seq = %d\033[0"
	.ascii	"m\000"
.LC53:
	.ascii	"\033[1;32mvenc lost a buf pts = %llu\033[0m\000"
.LC54:
	.ascii	"\033[1;32mchn %d start wrap buf: %d, %d, %d ,%d, se"
	.ascii	"q_id = %d\033[0m\000"
.LC55:
	.ascii	"venc(%s-%d) buf(%d) ready, pending: %d\000"
.LC56:
	.ascii	"\033[1;31mchn %d re send a buf fail seq = %d, pts ="
	.ascii	" %d\033[0m\000"
.LC57:
	.ascii	"\033[1;31mosd kzalloc failed!\033[0m\000"
.LC58:
	.ascii	"\033[1;31mjpg_osd kzalloc failed!\033[0m\000"
.LC59:
	.ascii	"\033[1;32mcreate thread %p\012\033[0m\000"
.LC60:
	.ascii	"\033[1;32mstop thread %p\012\033[0m\000"
.LC61:
	.ascii	"\033[1;32mstop thread %p ok\012\033[0m\000"
.LC62:
	.ascii	"VENC_IOCTL_VERSION\000"
.LC63:
	.ascii	"VENC_IOCTL_NODE_CREATE\000"
.LC64:
	.ascii	"VENC_IOCTL_NODE_DELETE\000"
.LC65:
	.ascii	"VENC_IOCTL_NODE_START\000"
.LC66:
	.ascii	"VENC_IOCTL_NODE_SEND_FRAME\000"
.LC67:
	.ascii	"VENC_IOCTL_NODE_SET_CROP\000"
.LC68:
	.ascii	"VENC_IOCTL_NODE_SET_COMBO\000"
.LC69:
	.ascii	"VENC_IOCTL_NODE_SET_SNAP\000"
.LC70:
	.ascii	"VENC_IOCTL_NODE_PP_CREATE\000"
.LC71:
	.ascii	"VENC_IOCTL_NODE_PP_DELETE\000"
.LC72:
	.ascii	"VENC_IOCTL_NODE_PP_START\000"
.LC73:
	.ascii	"VENC_IOCTL_NODE_PP_SEND_FRAME\000"
.LC74:
	.ascii	"VENC_IOCTL_NODE_PP_GET_RESULT\000"
.LC75:
	.ascii	"VENC_IOCTL_NODE_ENABLE_TB\000"
.LC76:
	.ascii	"VENC_IOCTL_NODE_TB_BIND\000"
.LC77:
	.ascii	"VENC_IOCTL_NODE_TB_REQUEST\000"
.LC78:
	.ascii	"VENC_IOCTL_NODE_GET_CROP\000"
.LC79:
	.ascii	"VENC_IOCTL_NODE_SET_VCODEC_ATTR\000"
.LC80:
	.ascii	"VENC_IOCTL_NODE_GET_VCODEC_ATTR\000"
.LC81:
	.ascii	"VENC_IOCTL_NODE_PP_SET_MD_ATTR\000"
.LC82:
	.ascii	"VENC_IOCTL_NODE_PP_GET_MD_ATTR\000"
.LC83:
	.ascii	"VENC_IOCTL_NODE_CHECK\000"
.LC84:
	.ascii	"VENC_IOCTL_NODE_SET_FRAMERATE\000"
.LC85:
	.ascii	"VENC_IOCTL_NODE_STOP\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
