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
	ldr	r0, [r0, #248]
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
	cbz	r0, .L5
	ldr	r0, [r0, #64]
.L5:
	bx	lr
	.fnend
	.size	mpi_buf_get_dmabuf, .-mpi_buf_get_dmabuf
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
	cbz	r0, .L10
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L11
.L10:
	movs	r0, #1
	bx	lr
.L11:
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
	push	{r4, lr}
	.save {r4, lr}
	mov	r2, r1
	ldr	r4, [r0, #128]
	cbz	r1, .L18
	ldr	r3, [r1]
	cbz	r3, .L18
	addw	r1, r4, #4036
	cbz	r1, .L18
	blx	r3
.L18:
	movw	r3, #4108
	ldr	r3, [r4, r3]
	cmp	r3, #0
	ite	ne
	movne	r0, #65
	moveq	r0, #0
	pop	{r4, pc}
	.fnend
	.size	venc_poll, .-venc_poll
	.section	.init.text,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rk_cam_phy_addr1_setup, %function
rk_cam_phy_addr1_setup:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #0
	push	{r3, lr}
	.save {r3, lr}
	mov	r1, r2
	bl	simple_strtoul
	ldr	r3, .L31
	str	r0, [r3]
	movs	r0, #1
	pop	{r3, pc}
.L32:
	.align	2
.L31:
	.word	.LANCHOR0
	.fnend
	.size	rk_cam_phy_addr1_setup, .-rk_cam_phy_addr1_setup
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rk_cam_phy_addr2_setup, %function
rk_cam_phy_addr2_setup:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #0
	push	{r3, lr}
	.save {r3, lr}
	mov	r1, r2
	bl	simple_strtoul
	ldr	r3, .L34
	str	r0, [r3, #4]
	movs	r0, #1
	pop	{r3, pc}
.L35:
	.align	2
.L34:
	.word	.LANCHOR0
	.fnend
	.size	rk_cam_phy_addr2_setup, .-rk_cam_phy_addr2_setup
	.text
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
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbz	r0, .L37
	ldr	r0, [r0, #64]
	bl	dma_buf_vmap
	str	r0, [r4, #220]
	mov	r4, r0
.L37:
	mov	r0, r4
	pop	{r4, pc}
	.fnend
	.size	mpi_buf_map, .-mpi_buf_map
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
	cbz	r0, .L41
	b	mpi_obj_ref
.L41:
	bx	lr
	.fnend
	.size	mpi_buf_ref, .-mpi_buf_ref
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
	ldr	r0, .L44
	push	{r3, lr}
	.save {r3, lr}
	str	r0, [r1, #128]
	bl	mpi_obj_ref
	movs	r0, #0
	pop	{r3, pc}
.L45:
	.align	2
.L44:
	.word	g_venc_dev+4
	.fnend
	.size	venc_open, .-venc_open
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
	ldr	r2, [r1]
	ldr	r3, [r0, #128]
	cmp	r2, #15
	ble	.L47
	movs	r3, #16
	movs	r1, #2
	strd	r2, r3, [sp, #4]
	movs	r0, #4
	movw	r3, #1845
	ldr	r2, .L49
	str	r3, [sp]
	ldr	r3, .L49+4
	bl	vlog
	mov	r0, #-1
.L46:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L47:
	add	r3, r3, r2
	movs	r0, #0
	add	r3, r3, #4896
	movs	r2, #1
	strb	r2, [r3]
	b	.L46
.L50:
	.align	2
.L49:
	.word	.LC0
	.word	.LANCHOR1
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
	ldr	r3, [r0, #128]
	ldrd	r0, r2, [r1]
	cmp	r0, #15
	bgt	.L52
	cmp	r2, #15
	ble	.L53
.L52:
	movs	r3, #16
	str	r0, [sp, #4]
	strd	r2, r3, [sp, #8]
	movs	r1, #2
	movw	r3, #1861
	ldr	r2, .L55
	str	r3, [sp]
	movs	r0, #4
	ldr	r3, .L55+4
	bl	vlog
	mov	r0, #-1
.L51:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L53:
	add	r3, r3, r0
	movw	r1, #4912
	movs	r0, #0
	strb	r2, [r3, r1]
	b	.L51
.L56:
	.align	2
.L55:
	.word	.LC1
	.word	.LANCHOR1+20
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
	ldr	r0, [r0, #128]
	cmp	r3, #15
	ble	.L58
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1876
	ldr	r2, .L60
	str	r3, [sp]
	ldr	r3, .L60+4
	bl	vlog
	mov	r0, #-1
.L57:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L58:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r3, r3, #1232
	movs	r1, #1
	str	r1, [r0, r3, lsl #2]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	b	.L57
.L61:
	.align	2
.L60:
	.word	.LC0
	.word	.LANCHOR1+38
	.fnend
	.size	venc_node_tb_request, .-venc_node_tb_request
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
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r0
	ldr	r3, [r0, #60]
	cbz	r3, .L63
	ldr	r3, [r0]
	movs	r1, #5
	ldr	r2, .L67
	movs	r0, #4
	str	r3, [sp, #4]
	movw	r3, #927
	str	r3, [sp]
	ldr	r3, .L67+4
	bl	vlog
.L63:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, lr}
	b	mpi_obj_unref
.L68:
	.align	2
.L67:
	.word	.LC2
	.word	.LANCHOR1+59
	.fnend
	.size	mpi_buf_unref, .-mpi_buf_unref
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
	mov	r7, r0
	ldr	r3, [r0, #8]
	.pad #28
	sub	sp, sp, #28
	mov	r6, r1
	mov	r5, r2
	cmp	r3, #1
	beq	.L103
.L134:
	movs	r6, #0
.L112:
	mov	r0, r6
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L103:
	mov	r10, #0
.L73:
	mov	r0, r10
	bl	get_venc_node
	cmp	r7, r0
	beq	.L75
	add	r10, r10, #1
	cmp	r10, #16
	bne	.L73
	mov	r3, #620
	mov	r2, #-1
	strd	r3, r2, [sp]
	ldr	r3, .L136
	ldr	r2, .L136+4
.L135:
	movs	r1, #2
	movs	r0, #4
	bl	vlog
	b	.L134
.L75:
	ldr	r3, [r7, #248]
	ldr	r8, [r3]
	cmp	r8, #0
	bne	.L133
	movw	r3, #626
	str	r10, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L136+8
	ldr	r3, .L136
	b	.L135
.L133:
	cmp	r6, #1
	bne	.L134
	cmp	r5, #0
	beq	.L134
	ldr	r4, .L136+12
	add	r9, r10, #52
	add	r3, r4, r9, lsl #2
	ldr	r3, [r3, #4]
	cbz	r3, .L80
	ldr	r3, [r4, #320]
	ldr	r2, [r5, #284]
	cmp	r3, r2
	beq	.L81
	movs	r1, #4
	strd	r3, r2, [sp, #4]
	mov	r0, r1
	movw	r3, #641
	ldr	r2, .L136+16
	str	r3, [sp]
	ldr	r3, .L136
	bl	vlog
.L81:
	ldr	r3, [r5, #284]
	adds	r3, r3, #1
	str	r3, [r4, #320]
.L80:
	ldr	r3, [r4, #316]
	cmp	r3, #0
	ble	.L82
	subs	r3, r3, #1
	str	r3, [r4, #316]
	b	.L134
.L82:
	add	r3, r10, #7648
	adds	r3, r3, #2
	ldr	r3, [r4, r3, lsl #2]
	cmp	r3, #9
	beq	.L83
	cmp	r3, #15
	bne	.L84
.L83:
	add	r3, r10, #237
	lsls	r3, r3, #4
	adds	r2, r4, r3
	ldr	r3, [r4, r3]
	cmp	r3, #0
	beq	.L134
	ble	.L84
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne	.L134
	movs	r3, #1
	str	r3, [r2, #4]
.L84:
	add	r2, r4, r10
	movw	r3, #4900
	ldrb	r3, [r2, r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L85
	ldr	r3, .L136+20
	movs	r2, #0
.L87:
	ldrb	r1, [r3], #1	@ zero_extendqisi2
	cmp	r1, r10
	beq	.L86
	adds	r2, r2, #1
	cmp	r2, #16
	bne	.L87
	b	.L134
.L88:
	add	lr, lr, #1
	cmp	lr, #100
	bne	.L89
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, ip	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L134
.L102:
	ldr	r0, [r5, #312]
	cmp	r0, #0
	bne	.L90
	ldr	r0, .L136+24
	ldrd	ip, r0, [r0, #8]
	orrs	r0, ip, r0
	beq	.L90
	strd	r3, r1, [sp, #8]
	movs	r1, #4
	movw	r3, #666
	str	r2, [sp, #4]
	str	r3, [sp]
	mov	r0, r1
	ldr	r3, .L136
	ldr	r2, .L136+28
	bl	vlog
	b	.L134
.L93:
	str	r5, [r4, #324]
	b	.L95
.L92:
	mov	r0, r8
	bl	mpi_queue_pop_obj
	mov	r10, r0
	cbnz	r0, .L96
	ldrd	r2, [r5, #288]
	movs	r1, #4
	mov	r0, r1
	strd	r2, [sp, #8]
	movw	r3, #690
	ldr	r2, .L136+32
	str	r3, [sp]
	ldr	r3, .L136
	bl	vlog
	mov	r0, r5
	bl	mpi_obj_unref
	b	.L134
.L96:
	ldr	r1, .L136+24
	ldr	r3, [r1, #20]
	cmp	r3, #19
	add	r2, r3, #1
	str	r2, [r1, #20]
	bgt	.L97
	ldr	r3, [r0]
	movs	r1, #4
	ldr	r2, .L136+28
	mov	r0, r1
	str	r3, [sp, #12]
	ldr	r3, [r7]
	str	r3, [sp, #8]
	ldr	r3, [r7, #44]
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	mov	r3, #696
	str	r3, [sp]
	ldr	r3, .L136
	bl	vlog
.L97:
	mov	r0, r10
	bl	mpi_obj_unref
	b	.L91
.L86:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	ip, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, #800
	mov	lr, #0
	mla	r3, r3, r2, r4
	add	r3, r3, #17792
	adds	r3, r3, #8
.L89:
	ldrd	r0, [r3], #8
	orrs	fp, r0, r1
	beq	.L88
	movs	r3, #100
	strd	r0, [r5, #288]
	mla	r3, r3, r2, lr
	movs	r0, #0
	movs	r1, #0
	addw	r2, r3, #625
	addw	r3, r3, #2225
	add	r2, r4, r2, lsl #3
	add	r3, r4, r3, lsl #3
	strd	r0, [r2]
	strd	r0, [r3]
.L85:
	ldr	r3, [r7, #44]
	ldr	r1, [r5]
	ldr	r2, [r3, #44]
	ldr	r3, [r7]
	cmp	r10, #0
	beq	.L102
.L90:
	ldr	r0, [r8, #16]
	add	fp, r4, r9, lsl #2
	strd	r2, r3, [sp, #4]
	mov	r3, #672
	ldr	r2, .L136+36
	strd	r1, r0, [sp, #12]
	movs	r1, #5
	str	r3, [sp]
	movs	r0, #4
	ldr	r3, .L136
	bl	vlog
	mov	r0, r5
	bl	mpi_obj_ref
.L91:
	mov	r1, r5
	mov	r0, r8
	bl	mpi_queue_push_obj
	cbnz	r0, .L95
	ldr	r3, [fp, #4]
	cmp	r3, #0
	beq	.L92
	ldr	r3, [r4, #324]
	cmp	r3, #0
	beq	.L93
	ldr	r2, .L136+24
	ldr	r3, [r2, #16]
	cmp	r3, #19
	add	r1, r3, #1
	str	r1, [r2, #16]
	bgt	.L94
	ldr	r3, [r5, #284]
	movs	r1, #2
	ldr	r2, .L136+40
	movs	r0, #4
	str	r3, [sp, #4]
	movw	r3, #679
	str	r3, [sp]
	ldr	r3, .L136
	bl	vlog
.L94:
	mov	r0, r5
	bl	mpi_obj_unref
.L95:
	ldr	r3, [r4, #312]
	cbz	r3, .L100
	bl	mcu_get_wrap_status
	cmp	r0, #2
	itt	ne
	movne	r3, #0
	strne	r3, [r4, #312]
.L100:
	ldr	r3, [r5, #312]
	cmp	r3, #0
	bne	.L112
	add	r9, r4, r9, lsl #2
	ldr	r3, [r9, #4]
	cmp	r3, #0
	beq	.L112
	ldr	r3, [r4, #312]
	cmp	r3, #0
	bne	.L112
	bl	mcu_wrap_start
	movs	r3, #1
	str	r3, [r4, #312]
	movs	r1, #4
	ldr	r3, [r5, #284]
	mov	r0, r1
	ldr	r2, .L136+44
	str	r3, [sp, #20]
	ldr	r3, [r5, #248]
	str	r3, [sp, #16]
	ldr	r3, [r5, #244]
	str	r3, [sp, #12]
	ldr	r3, [r5, #240]
	str	r3, [sp, #8]
	ldr	r3, [r5, #236]
	str	r3, [sp, #4]
	movw	r3, #709
	str	r3, [sp]
	ldr	r3, .L136
	bl	vlog
	b	.L112
.L137:
	.align	2
.L136:
	.word	.LANCHOR1+73
	.word	.LC3
	.word	.LC4
	.word	g_venc_dev
	.word	.LC5
	.word	g_venc_dev+4916
	.word	.LANCHOR0
	.word	.LC6
	.word	.LC9
	.word	.LC7
	.word	.LC8
	.word	.LC10
	.fnend
	.size	vcodec_handle_message, .-vcodec_handle_message
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
	cbz	r0, .L141
	bl	mpi_dev_unregister
	mov	r0, r4
	bl	kfree
	movs	r0, #0
.L139:
	pop	{r4, pc}
.L141:
	mov	r0, #-1
	b	.L139
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
	ldr	r0, .L148
	bl	mpi_allocator_find_by_name
	mov	r2, r4
	movs	r1, #1
	bl	mpi_allocator_alloc
	mov	r5, r0
	cbz	r0, .L144
	movs	r1, #0
	bl	mpi_buf_create
	mov	r4, r0
	cbz	r0, .L142
	ldr	r3, .L148+4
	str	r3, [r0, #4]
	mov	r0, r5
	bl	dma_buf_put
.L142:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L144:
	mov	r4, r0
	b	.L142
.L149:
	.align	2
.L148:
	.word	.LC11
	.word	.LC12
	.fnend
	.size	mpi_buf_alloc, .-mpi_buf_alloc
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
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbz	r0, .L151
	ldr	r1, [r0, #220]
	cbz	r1, .L152
	ldr	r0, [r0, #64]
	cbz	r0, .L152
	bl	dma_buf_vunmap
.L152:
	movs	r3, #0
	str	r3, [r4, #220]
.L151:
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	mpi_buf_unmap, .-mpi_buf_unmap
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
	ldr	r2, .L163
	movs	r1, #4
	b	mpi_queue_create
.L164:
	.align	2
.L163:
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
	cbz	r0, .L167
	movs	r1, #0
	bl	mpi_queue_destroy
.L167:
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
	ldr	r0, .L176
	mov	r6, r1
	bl	mpi_allocator_find_by_name
	mov	r4, r0
	cbz	r0, .L171
	ldr	r3, .L176+4
	mov	r2, r5
	movs	r1, #1
	str	r3, [sp]
	mov	r3, r6
	bl	mpi_buf_pool_create
	mov	r4, r0
.L171:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L177:
	.align	2
.L176:
	.word	.LC11
	.word	.LC12
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
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r5, r0
	ldr	r0, .L188
	mov	r4, r1
	bl	mpi_dev_find_by_name
	ldr	r2, [r4, #8]
	ldr	r5, [r5, #128]
	cmp	r2, #16
	mov	r6, r0
	beq	.L181
	movw	r3, #1334
	ldr	r2, .L188+4
	str	r3, [sp]
	ldr	r3, .L188+8
.L187:
	movs	r1, #2
	movs	r0, #4
	bl	vlog
	b	.L186
.L181:
	ldr	r3, [r4, #4]
	cmp	r3, #15
	ble	.L183
	strd	r3, r2, [sp, #4]
	movs	r1, #2
	movw	r3, #1338
	ldr	r2, .L188+12
	str	r3, [sp]
	movs	r0, #4
	ldr	r3, .L188+8
	bl	vlog
.L186:
	mov	r0, #-1
.L180:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L183:
	ldr	r0, .L188+16
	bl	mpi_allocator_find_by_name
	cbnz	r0, .L184
	mov	r3, #1344
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L188+8
	movs	r0, #4
	ldr	r2, .L188+20
	bl	vlog
	b	.L186
.L184:
	mov	r0, r6
	movs	r3, #0
	movs	r2, #1
	ldr	r1, [r4, #4]
	bl	mpi_dev_create_node_with_params
	mov	r6, r0
	cbnz	r0, .L185
	movw	r3, #1350
	ldr	r2, .L188+24
	str	r3, [sp]
	ldr	r3, .L188+8
	b	.L187
.L185:
	ldr	r3, .L188+28
	movs	r1, #1
	str	r3, [r0, #4]
	bl	mpi_node_enable_auto_streamon
	mov	r3, r6
	ldrd	r1, r2, [r4]
	movs	r0, #4
	bl	vsys_add_node_to_map
	ldr	r3, [r4, #4]
	ldr	r2, [r4, #12]
	movs	r0, #0
	add	r3, r3, #7648
	adds	r3, r3, #2
	add	r3, r5, r3, lsl #2
	str	r2, [r3, #-4]
	b	.L180
.L189:
	.align	2
.L188:
	.word	.LC13
	.word	.LC14
	.word	.LANCHOR1+98
	.word	.LC0
	.word	.LC11
	.word	.LC15
	.word	.LC16
	.word	.LC12
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
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r4, [r1]
	ldr	r6, [r0, #128]
	cmp	r4, #15
	ble	.L191
	movs	r3, #16
	ldr	r2, .L220
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1369
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L220+4
	bl	vlog
	mov	r0, #-1
.L190:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L191:
	movs	r3, #192
	add	r8, r6, #516
	muls	r3, r4, r3
	mov	r9, #0
	add	r5, r3, #324
	adds	r7, r6, r3
	add	r5, r5, r6
	add	r8, r8, r3
.L195:
	ldr	r0, [r7, #328]
	cbz	r0, .L193
	bl	kfree
.L193:
	ldr	r0, [r7, #332]
	cbz	r0, .L194
	bl	kfree
.L194:
	str	r9, [r5]	@ unaligned
	adds	r5, r5, #12
	str	r9, [r5, #-8]	@ unaligned
	adds	r7, r7, #12
	str	r9, [r5, #-4]	@ unaligned
	cmp	r5, r8
	bne	.L195
	mov	r0, r4
	sub	r8, r6, #4
	bl	get_venc_node
	mov	r5, r0
	cbz	r0, .L196
	mov	r2, r4
	movs	r1, #0
	movs	r0, #4
	bl	vsys_remove_node_from_map
	mov	r0, r5
	bl	mpi_node_destroy
.L196:
	adds	r3, r6, r4
	add	r9, r3, #4896
	ldrb	r2, [r9]	@ zero_extendqisi2
	cbz	r2, .L197
	movw	r2, #4912
	movs	r1, #255
	strb	r1, [r3, r2]
	add	r3, r4, #1232
	movs	r7, #0
	str	r7, [r6, r3, lsl #2]
	mov	r6, #800
	mul	r5, r6, r4
	mov	r2, r6
	mov	r1, r7
	add	r0, r5, #4992
	adds	r0, r0, #8
	add	r0, r0, r8
	bl	memset
	add	r0, r5, #17792
	adds	r0, r0, #8
	mov	r2, r6
	mov	r1, r7
	add	r0, r0, r8
	bl	memset
	strb	r7, [r9]
.L197:
	cbnz	r4, .L198
	ldr	r5, .L220+8
	ldr	r3, [r5, #212]
	cbz	r3, .L198
	bl	mcu_wrap_end
	str	r4, [r5, #212]
.L198:
	movs	r5, #0
.L199:
	mov	r0, r5
	bl	get_venc_node
	mov	r4, r0
	cbnz	r0, .L201
	adds	r5, r5, #1
	cmp	r5, #16
	bne	.L199
	bl	mpp_vcodec_clear_buf_resource
	mov	r0, r4
	b	.L190
.L201:
	movs	r0, #0
	b	.L190
.L221:
	.align	2
.L220:
	.word	.LC0
	.word	.LANCHOR1+115
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
	cmp	r4, #0
	ble	.L223
	movs	r3, #1
	ldr	r2, .L226
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1710
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L226+4
	bl	vlog
	mov	r0, #-1
.L222:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L223:
	mov	r0, r4
	bl	get_pp_node
	cmp	r0, #0
	beq	.L222
	mov	r0, r4
	bl	get_pp_node
	bl	mpi_node_streamon
	movs	r0, #0
	b	.L222
.L227:
	.align	2
.L226:
	.word	.LC0
	.word	.LANCHOR1+132
	.fnend
	.size	venc_node_pp_start, .-venc_node_pp_start
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
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r3, [r1, #8]
	ldr	r2, [r0, #128]
	cmp	r3, #15
	bls	.L229
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1521
	ldr	r2, .L233
	str	r3, [sp]
	ldr	r3, .L233+4
	bl	vlog
	mov	r0, #-1
.L228:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L229:
	movs	r0, #60
	mla	r3, r0, r3, r2
	adds	r2, r1, r0
	add	r3, r3, #30592
	adds	r3, r3, #68
.L231:
	ldr	r0, [r1], #4	@ unaligned
	cmp	r1, r2
	str	r0, [r3], #4	@ unaligned
	bne	.L231
	movs	r0, #0
	b	.L228
.L234:
	.align	2
.L233:
	.word	.LC0
	.word	.LANCHOR1+151
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
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r3, [r1, #8]
	ldr	r2, [r0, #128]
	cmp	r3, #15
	bls	.L236
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1534
	ldr	r2, .L240
	str	r3, [sp]
	ldr	r3, .L240+4
	bl	vlog
	mov	r0, #-1
.L235:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L236:
	movs	r0, #60
	mla	r3, r0, r3, r2
	add	r3, r3, #30592
	adds	r3, r3, #68
	adds	r2, r3, r0
.L238:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r2
	str	r0, [r1], #4	@ unaligned
	bne	.L238
	movs	r0, #0
	b	.L235
.L241:
	.align	2
.L240:
	.word	.LC0
	.word	.LANCHOR1+177
	.fnend
	.size	venc_node_get_vcodec_attr, .-venc_node_get_vcodec_attr
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
	ldr	r0, .L257
	mov	r4, r1
	bl	mpi_dev_find_by_name
	ldr	r3, [r4, #8]
	cmp	r3, #96
	mov	r8, r0
	beq	.L243
	movw	r3, #1450
	ldr	r2, .L257+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L257+8
	movs	r0, #4
	bl	vlog
.L256:
	mov	r0, #-1
	b	.L242
.L243:
	ldr	r5, [r4]
	cmp	r5, #15
	ble	.L245
	movs	r3, #16
	ldr	r2, .L257+12
	strd	r5, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1455
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L257+8
	bl	vlog
	b	.L256
.L245:
	mov	r0, r5
	bl	get_venc_node
	mov	r7, r0
	cbz	r0, .L246
	ldr	r0, [r4, #4]
	cmp	r8, #0
	beq	.L246
	cmp	r0, #0
	blt	.L246
	bl	get_mpi_buf
	mov	r6, r0
	cbz	r0, .L246
	movs	r2, #80
	add	r1, r4, #16
	adds	r0, r0, #232
	bl	memcpy
	movs	r3, #4
	mov	r2, r6
	str	r3, [r6, #200]
	movs	r1, #1
	ldr	r3, [r8, #80]
	mov	r0, r7
	blx	r3
	movs	r0, #0
.L242:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L246:
	ldr	r3, [r4, #4]
	movs	r1, #2
	strd	r5, r7, [sp, #4]
	movs	r0, #4
	ldr	r2, .L257+16
	strd	r8, r3, [sp, #12]
	mov	r3, #1472
	str	r3, [sp]
	ldr	r3, .L257+8
	bl	vlog
	b	.L256
.L258:
	.align	2
.L257:
	.word	.LC13
	.word	.LC17
	.word	.LANCHOR1+203
	.word	.LC0
	.word	.LC18
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
	ldr	r0, .L274
	mov	r4, r1
	bl	mpi_dev_find_by_name
	ldr	r3, [r4, #8]
	cmp	r3, #96
	mov	r7, r0
	beq	.L260
	movw	r3, #1727
	ldr	r2, .L274+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L274+8
	movs	r0, #4
	bl	vlog
.L273:
	mov	r0, #-1
	b	.L259
.L260:
	ldr	r5, [r4]
	cmp	r5, #0
	ble	.L262
	movs	r3, #1
	ldr	r2, .L274+12
	strd	r5, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1732
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L274+8
	bl	vlog
	b	.L273
.L262:
	mov	r0, r5
	bl	get_pp_node
	mov	r6, r0
	cbz	r0, .L263
	ldr	r0, [r4, #4]
	cbz	r7, .L263
	cmp	r0, #0
	blt	.L263
	bl	get_mpi_buf
	mov	r8, r0
	cbz	r0, .L263
	movs	r2, #80
	add	r1, r4, #16
	adds	r0, r0, #232
	bl	memcpy
	ldr	r3, [r7, #80]
	mov	r2, r8
	movs	r1, #1
	mov	r0, r6
	blx	r3
	movs	r0, #0
.L259:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L263:
	ldr	r3, [r4, #4]
	movs	r1, #2
	strd	r5, r6, [sp, #4]
	movs	r0, #4
	ldr	r2, .L274+16
	strd	r7, r3, [sp, #12]
	movw	r3, #1748
	str	r3, [sp]
	ldr	r3, .L274+8
	bl	vlog
	b	.L273
.L275:
	.align	2
.L274:
	.word	.LC12
	.word	.LC17
	.word	.LANCHOR1+224
	.word	.LC0
	.word	.LC19
	.fnend
	.size	venc_node_pp_send_frame, .-venc_node_pp_send_frame
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_thread_fn, %function
venc_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 456
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #472
	sub	sp, sp, #472
	adds	r3, r0, #4
	mov	r4, r0
	str	r3, [sp, #8]
	movs	r1, #5
	ldr	r3, [r0, #48]
	movs	r0, #4
	ldr	r2, .L337
	add	r8, r4, #4096
	add	r8, r8, #12
	str	r3, [sp, #4]
	mov	r3, #2080
	str	r3, [sp]
	ldr	r3, .L337+4
	bl	vlog
.L277:
	ldr	r3, [r4, #180]
	cbz	r3, .L302
	movs	r0, #0
	add	sp, sp, #472
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L302:
	add	r0, r4, #196
	bl	down
	ldr	r5, [r4, #92]
.L336:
	subs	r5, r5, #48
	add	r2, r4, #92
	add	r3, r5, #48
	cmp	r3, r2
	beq	.L277
	ldr	r3, [r5, #248]
	ldr	r0, [r3]
	bl	mpi_queue_pop_obj
	mov	r7, r0
	cmp	r0, #0
	beq	.L279
	ldr	r2, .L337+8
	movs	r3, #6
	str	r3, [sp, #212]
	ldr	r3, [r2, #24]
	adds	r1, r3, #1
	str	r1, [r2, #24]
	str	r3, [sp, #208]
	add	r2, sp, #208
	ldr	r3, [r0, #64]
	movs	r1, #16
	movs	r0, #0
	str	r3, [sp, #228]
	movw	r3, #4196
	ldr	r3, [r4, r3]
	str	r3, [sp, #216]
	movw	r3, #4200
	ldr	r3, [r4, r3]
	str	r3, [sp, #220]
	movw	r3, #4204
	ldr	r3, [r4, r3]
	str	r3, [sp, #224]
	bl	vepu_pp_control
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r6, [r8]
	adds	r6, r6, #1
	cmp	r6, #5
	it	ge
	movge	r6, #0
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #4064]
	cbz	r3, .L281
	addw	r3, r6, #1054
	add	r3, r4, r3, lsl #2
	ldr	r3, [r3, #4]
	ldr	r3, [r3, #64]
	str	r3, [sp, #152]
	movs	r3, #2
	str	r3, [sp, #156]
	movw	r3, #31636
	ldr	r3, [r4, r3]
	cmp	r3, #0
	ble	.L282
	movw	r2, #31632
	ldr	r2, [r4, r2]
	cmp	r2, #0
	ble	.L282
	movw	r1, #4095
	cmp	r2, r1
	ite	le
	strle	r2, [sp, #160]
	strgt	r1, [sp, #160]
	cmp	r3, #4
	it	ge
	movge	r3, #4
.L332:
	str	r3, [sp, #164]
	movw	r3, #4208
	ldr	r3, [r4, r3]
	movs	r0, #0
	add	r2, sp, #152
	movs	r1, #32
	str	r0, [sp, #172]
	str	r3, [sp, #168]
	bl	vepu_pp_control
.L281:
	ldr	r3, [r4, #4068]
	cbz	r3, .L286
	ldr	r3, [r4, #4048]
	movs	r1, #12
	ldr	r2, [r4, #4052]
	movs	r0, #1
	strd	r0, r1, [sp, #60]
	ldr	r1, [r4, #4072]
	muls	r2, r3, r2
	movw	r3, #4212
	ldr	r3, [r4, r3]
	cmp	r1, #0
	beq	.L287
	asrs	r2, r2, #8
.L333:
	muls	r3, r2, r3
	movs	r1, #48
	add	r2, sp, #60
	movs	r0, #0
	asrs	r3, r3, #3
	str	r3, [sp, #68]
	bl	vepu_pp_control
.L286:
	ldr	r3, [r4, #4056]
	cbz	r3, .L289
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r3, r4, #4896
	movw	r1, #4764
	movs	r0, #0
	ldr	r3, [r3]
	add	r3, r4, r3, lsl #3
	ldr	r1, [r3, r1]
	add	r3, r3, #4768
	ldr	r1, [r1, #64]
	str	r0, [r3]
	str	r1, [sp, #340]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r1, #64
	add	r2, sp, #340
	bl	vepu_pp_control
.L289:
	ldr	r3, [r4, #4060]
	cbz	r3, .L290
	add	r2, sp, #272
	movs	r1, #80
	movs	r0, #0
	bl	vepu_pp_control
.L290:
	ldr	r3, [r4, #4064]
	cbnz	r3, .L291
	ldr	r3, [r4, #4068]
	cbnz	r3, .L291
	ldr	r3, [r4, #4056]
	cbnz	r3, .L291
	ldr	r3, [r4, #4060]
	cmp	r3, #0
	beq	.L292
.L291:
	add	r2, sp, #208
	movs	r1, #96
	movs	r0, #0
	bl	vepu_pp_control
	ldr	r3, [r4, #4068]
	cbz	r3, .L293
	add	r2, sp, #20
	movs	r1, #112
	movs	r0, #0
	bl	vepu_pp_control
.L293:
	ldr	r3, [r4, #4060]
	cbz	r3, .L292
	add	r2, sp, #104
	movs	r1, #128
	movs	r0, #0
	bl	vepu_pp_control
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	lr, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movw	ip, #4760
	movs	r2, #52
	ldr	r3, [r4, ip]
	add	r10, sp, #152
	mla	r3, r2, r3, r4
	add	r2, sp, #104
	add	r3, r3, #4224
	adds	r3, r3, #16
.L294:
	mov	r9, r2
	adds	r3, r3, #8
	ldmia	r9!, {r0, r1}
	cmp	r9, r10
	str	r0, [r3, #-8]	@ unaligned
	mov	r2, r9
	str	r1, [r3, #-4]	@ unaligned
	bne	.L294
	ldr	r3, [r4, ip]
	movs	r1, #52
	ldr	r2, [r7, #284]
	mla	r1, r1, r3, r4
	adds	r3, r3, #1
	cmp	r3, #9
	add	r1, r1, #4288
	it	gt
	movgt	r3, #0
	str	r2, [r1]
	str	r3, [r4, ip]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, lr	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L292:
	ldr	r3, [r4, #4056]
	cbz	r3, .L297
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	ip, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r2, r4, #4896
	ldr	r0, [r7, #284]
	ldr	r3, [r2]
	addw	r1, r3, #595
	adds	r3, r3, #1
	cmp	r3, #1
	add	r1, r4, r1, lsl #3
	it	gt
	movgt	r3, #0
	str	r0, [r1, #8]
	str	r3, [r2]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, ip	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L297:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	ip, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movw	r3, #4112
	str	r6, [r8]
	movs	r2, #1
	str	r2, [r4, r3]
	add	r3, r4, #4096
	add	r6, sp, #20
	adds	r3, r3, #24
	add	lr, sp, #60
.L300:
	mov	r2, r6
	adds	r3, r3, #8
	ldmia	r2!, {r0, r1}
	cmp	r2, lr
	str	r0, [r3, #-8]	@ unaligned
	mov	r6, r2
	str	r1, [r3, #-4]	@ unaligned
	bne	.L300
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, ip	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r3, #0
	addw	r0, r4, #4040
	movs	r2, #1
	movs	r1, #3
	bl	__wake_up
	mov	r0, r7
	bl	mpi_buf_unref
.L279:
	ldr	r5, [r5, #48]
	b	.L336
.L282:
	movw	r3, #4216
	ldr	r3, [r4, r3]
	cmp	r3, #1
	bne	.L284
	movs	r3, #70
	str	r3, [sp, #160]
	movs	r3, #3
	b	.L332
.L284:
	cmp	r3, #2
	itett	ne
	movne	r3, #30
	moveq	r2, #50
	strne	r3, [sp, #160]
	movne	r3, #1
	it	eq
	streq	r2, [sp, #160]
	b	.L332
.L287:
	asrs	r2, r2, #4
	b	.L333
.L338:
	.align	2
.L337:
	.word	.LC20
	.word	.LANCHOR1+248
	.word	.LANCHOR0
	.fnend
	.size	venc_thread_fn, .-venc_thread_fn
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
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r4, r0
	ldr	r3, [r0, #8]
	.pad #24
	sub	sp, sp, #24
	mov	r5, r2
	cmp	r3, #1
	bne	.L340
	cmp	r1, #1
	bne	.L340
	cbz	r2, .L340
	ldr	r3, [r0, #248]
	movs	r1, #5
	ldr	r9, .L347+4
	movw	r8, #750
	ldr	r10, .L347+12
	ldr	r6, [r3]
	ldr	r3, [r6, #16]
	str	r3, [sp, #16]
	ldr	r3, [r2]
	ldr	r2, .L347
	str	r3, [sp, #12]
	ldr	r3, [r0]
	str	r3, [sp, #8]
	ldr	r3, [r0, #44]
	movs	r0, #4
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	movw	r3, #738
	str	r3, [sp]
	ldr	r3, .L347+4
	bl	vlog
	mov	r0, r5
	bl	mpi_obj_ref
.L341:
	mov	r1, r5
	mov	r0, r6
	bl	mpi_queue_push_obj
	cbz	r0, .L343
	ldr	r0, .L347+8
	bl	up
	b	.L340
.L343:
	mov	r0, r6
	bl	mpi_queue_pop_obj
	mov	r7, r0
	cbnz	r0, .L342
	mov	r0, r5
	bl	mpi_obj_unref
.L340:
	movs	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L342:
	ldr	r3, [r0]
	mov	r2, r10
	movs	r1, #5
	movs	r0, #4
	str	r3, [sp, #12]
	ldr	r3, [r4]
	str	r3, [sp, #8]
	ldr	r3, [r4, #44]
	ldr	r3, [r3, #44]
	strd	r8, r3, [sp]
	mov	r3, r9
	bl	vlog
	mov	r0, r7
	bl	mpi_obj_unref
	b	.L341
.L348:
	.align	2
.L347:
	.word	.LC21
	.word	.LANCHOR1+263
	.word	g_venc_dev+196
	.word	.LC22
	.fnend
	.size	venc_handle_message, .-venc_handle_message
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
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	ldr	r6, [r1]
	ldr	r4, [r0, #128]
	cmp	r6, #0
	ble	.L350
	movs	r3, #1
	ldr	r2, .L371
	strd	r6, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1671
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L371+4
	bl	vlog
	mov	r0, #-1
.L349:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L350:
	ldr	r3, [r4, #164]
	cmn	r3, #4096
	bhi	.L352
	movs	r3, #1
	add	r0, r4, #192
	str	r3, [r4, #176]
	bl	up
	ldr	r0, [r4, #164]
	bl	kthread_stop
	movs	r3, #0
	str	r3, [r4, #164]
.L352:
	add	r5, r4, #4192
	add	r7, r4, #4224
	adds	r5, r5, #24
	adds	r7, r7, #12
	mov	r8, #0
.L354:
	ldr	r0, [r5], #4
	cbz	r0, .L353
	bl	mpi_buf_unmap
	ldr	r0, [r5, #-4]
	bl	mpi_obj_unref
	str	r8, [r5, #-4]
.L353:
	cmp	r7, r5
	bne	.L354
	movw	r5, #4760
	ldr	r0, [r4, r5]
	cbz	r0, .L355
	bl	mpi_obj_unref
	movs	r3, #0
	str	r3, [r4, r5]
.L355:
	add	r5, r4, #4768
	ldr	r0, [r5]
	cbz	r0, .L356
	bl	mpi_obj_unref
	movs	r3, #0
	str	r3, [r5]
.L356:
	mov	r0, r6
	bl	vepu_pp_destroy_chn
	mov	r0, r6
	bl	get_pp_node
	mov	r5, r0
	cbz	r0, .L357
	mov	r2, r6
	movs	r1, #0
	movs	r0, #20
	bl	vsys_remove_node_from_map
	mov	r0, r5
	bl	mpi_node_destroy
.L357:
	add	r0, r4, #31616
	movs	r2, #16
	movs	r1, #0
	adds	r0, r0, #4
	bl	memset
	movs	r0, #0
	b	.L349
.L372:
	.align	2
.L371:
	.word	.LC0
	.word	.LANCHOR1+291
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
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	ldr	r4, [r1, #128]
	mov	r0, r4
	bl	mpi_obj_unref
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bne	.L374
	ldr	r5, .L397
	ldr	r3, [r5, #212]
	cbz	r3, .L375
	bl	mcu_wrap_end
	movs	r3, #0
	str	r3, [r5, #212]
.L375:
	add	r5, r4, #4192
	add	r6, r4, #4224
	adds	r5, r5, #24
	adds	r6, r6, #12
	movs	r7, #0
.L377:
	ldr	r0, [r5], #4
	cbz	r0, .L376
	bl	mpi_buf_unmap
	ldr	r0, [r5, #-4]
	bl	mpi_obj_unref
	str	r7, [r5, #-4]
.L376:
	cmp	r5, r6
	bne	.L377
	movw	r5, #4760
	ldr	r0, [r4, r5]
	cbz	r0, .L378
	bl	mpi_obj_unref
	movs	r3, #0
	str	r3, [r4, r5]
.L378:
	add	r5, r4, #4768
	ldr	r0, [r5]
	cbz	r0, .L379
	bl	mpi_obj_unref
	movs	r3, #0
	str	r3, [r5]
.L379:
	movs	r0, #0
	bl	get_pp_node
	cbz	r0, .L380
	movs	r0, #0
	bl	vepu_pp_destroy_chn
.L380:
	mov	r2, #3072
	movs	r1, #0
	add	r0, r4, #324
	subs	r5, r4, #4
	bl	memset
	mov	r2, #384
	movs	r1, #0
	addw	r0, r4, #3396
	bl	memset
	mov	r2, #256
	movs	r1, #0
	addw	r0, r4, #3780
	bl	memset
	movs	r2, #172
	movs	r1, #0
	addw	r0, r4, #4044
	bl	memset
	add	r0, r5, #31616
	movs	r2, #16
	movs	r1, #0
	adds	r0, r0, #8
	bl	memset
	add	r0, r5, #4896
	movs	r2, #16
	mov	r1, #-1
	adds	r0, r0, #20
	bl	memset
	movs	r2, #64
	movs	r1, #0
	add	r0, r4, #4928
	bl	memset
	add	r0, r5, #4992
	mov	r2, #12800
	movs	r1, #0
	adds	r0, r0, #8
	bl	memset
	add	r0, r5, #17792
	mov	r2, #12800
	movs	r1, #0
	adds	r0, r0, #8
	bl	memset
	movs	r2, #16
	movs	r1, #0
	add	r0, r4, #4896
	bl	memset
	bl	mpp_vcodec_clear_buf_resource
.L374:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L398:
	.align	2
.L397:
	.word	g_venc_dev
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
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r3, r2
	ldr	r2, [r0, #128]
	uxtb	r4, r1
	ldr	r6, [r2, #160]
	ldr	r5, [r6, #192]
	cmp	r5, r4
	bhi	.L400
	movw	r3, #1927
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L403
	movs	r0, #4
	ldr	r2, .L403+4
	bl	vlog
.L401:
	movw	r3, #1943
	ldr	r2, .L403+8
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L403
	movs	r0, #4
	bl	vlog
	mvn	r0, #21
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L400:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
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
	cbnz	r4, .L402
	movw	r3, #1934
	ldr	r2, .L403+12
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L403
	movs	r0, #4
	bl	vlog
	b	.L401
.L402:
	mov	r2, r1
	mov	r1, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	mpi_ioctl_kernel
.L404:
	.align	2
.L403:
	.word	.LANCHOR1+311
	.word	.LC23
	.word	.LC25
	.word	.LC24
	.fnend
	.size	venc_ioctl, .-venc_ioctl
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
	bls	.L406
	movs	r3, #16
	ldr	r2, .L414
	strd	r0, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #857
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L414+4
	bl	vlog
	mov	r0, #-1
.L405:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L406:
	ldr	r3, .L414+8
	add	r2, r0, #7648
	adds	r2, r2, #2
	ldr	r2, [r3, r2, lsl #2]
	cmp	r2, #9
	beq	.L408
	cmp	r2, #15
	bne	.L409
.L408:
	adds	r0, r0, #237
	lsls	r0, r0, #4
	adds	r2, r3, r0
	ldr	r3, [r3, r0]
	movs	r0, #0
	cmp	r3, #0
	ble	.L405
	str	r0, [r2, #4]
	b	.L405
.L409:
	movs	r0, #0
	b	.L405
.L415:
	.align	2
.L414:
	.word	.LC0
	.word	.LANCHOR1+322
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
	.save {lr}
	.pad #20
	ldr	r2, [r1, #8]
	ldr	r3, [r0, #128]
	cmp	r2, #12
	beq	.L417
	movw	r3, #1547
	ldr	r2, .L421
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L421+4
	movs	r0, #4
	bl	vlog
.L420:
	mov	r0, #-1
.L416:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L417:
	ldr	r2, [r1]
	cmp	r2, #15
	ble	.L419
	movs	r3, #16
	movs	r1, #2
	strd	r2, r3, [sp, #4]
	movs	r0, #4
	mov	r3, #1552
	ldr	r2, .L421+8
	str	r3, [sp]
	ldr	r3, .L421+4
	bl	vlog
	b	.L420
.L419:
	add	r3, r3, r2, lsl #4
	movs	r2, #1
	movs	r0, #0
	str	r2, [r3, #3780]
	ldr	r2, [r1, #4]
	str	r2, [r3, #3784]
	b	.L416
.L422:
	.align	2
.L421:
	.word	.LC26
	.word	.LANCHOR1+345
	.word	.LC0
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
	ldr	r3, [r1, #8]
	ldr	r2, [r0, #128]
	cmp	r3, #12
	beq	.L424
	movw	r3, #1569
	ldr	r2, .L429
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L429+4
	movs	r0, #4
	bl	vlog
.L428:
	mov	r0, #-1
.L423:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L424:
	ldr	r3, [r1]
	cmp	r3, #15
	ble	.L426
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1574
	ldr	r2, .L429+8
	str	r3, [sp]
	ldr	r3, .L429+4
	bl	vlog
	b	.L428
.L426:
	ldr	r4, [r1, #4]
	add	r0, r2, r3, lsl #4
	str	r4, [r0, #3788]
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r0, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r1, #4]
	cmp	r1, #0
	itt	gt
	addgt	r3, r3, #1232
	strgt	r1, [r2, r3, lsl #2]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r0	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	b	.L423
.L430:
	.align	2
.L429:
	.word	.LC27
	.word	.LANCHOR1+365
	.word	.LC0
	.fnend
	.size	venc_node_set_snap, .-venc_node_set_snap
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
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r2, [r1, #4]
	ldr	r3, [r0, #128]
	cmp	r2, #16
	beq	.L432
	movw	r3, #1812
	ldr	r2, .L440
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L440+4
	movs	r0, #4
	bl	vlog
.L439:
	mov	r0, #-1
.L431:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L432:
	ldr	r2, [r1]
	cmp	r2, #0
	ble	.L434
	movs	r3, #1
	movs	r1, #2
	strd	r2, r3, [sp, #4]
	movs	r0, #4
	movw	r3, #1817
	ldr	r2, .L440+8
	str	r3, [sp]
	ldr	r3, .L440+4
	bl	vlog
	b	.L439
.L434:
	movw	r2, #31632
	ldr	r0, [r3, r2]
	cmp	r0, #0
	ble	.L435
	movw	r0, #31628
	ldr	r0, [r3, r0]
	cmp	r0, #0
	ble	.L435
	str	r0, [r1, #8]
	ldr	r3, [r3, r2]
.L438:
	str	r3, [r1, #12]
	movs	r0, #0
	b	.L431
.L435:
	movw	r2, #4212
	ldr	r3, [r3, r2]
	cmp	r3, #1
	bne	.L436
	movs	r3, #70
	str	r3, [r1, #8]
	movs	r3, #3
	b	.L438
.L436:
	cmp	r3, #2
	itett	ne
	movne	r3, #30
	moveq	r2, #50
	strne	r3, [r1, #8]
	movne	r3, #1
	it	eq
	streq	r2, [r1, #8]
	b	.L438
.L441:
	.align	2
.L440:
	.word	.LC28
	.word	.LANCHOR1+384
	.word	.LC0
	.fnend
	.size	venc_node_pp_get_md_attr, .-venc_node_pp_get_md_attr
	.align	1
	.global	vcodec_set_intra_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_set_intra_info, %function
vcodec_set_intra_info:
	.fnstart
	@ args = 12, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	cmp	r0, #15
	.pad #20
	sub	sp, sp, #20
	mov	fp, r0
	mov	r8, r2
	mov	r9, r3
	ldrd	r6, [sp, #56]
	ldr	r4, [sp, #64]
	bls	.L443
	movs	r3, #16
	ldr	r2, .L469
	strd	r0, r3, [sp, #4]
	movs	r1, #2
	mov	r3, #812
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L469+4
	bl	vlog
	mov	r0, #-1
.L442:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L443:
	bl	get_venc_node
	cbz	r0, .L445
	ldr	r3, [r0, #348]
	cbz	r3, .L445
	blx	r3
.L445:
	cmp	r4, #0
	beq	.L468
	ldr	r4, .L469+8
	add	r3, fp, #7648
	adds	r3, r3, #2
	ldr	r3, [r4, r3, lsl #2]
	cmp	r3, #9
	beq	.L446
	cmp	r3, #15
	bne	.L447
.L446:
	add	r3, fp, #237
	lsls	r3, r3, #4
	adds	r1, r4, r3
	ldr	r2, [r4, r3]
	cmp	r2, #0
	itttt	gt
	addgt	r2, r2, #-1
	strgt	r2, [r4, r3]
	movgt	r3, #0
	strgt	r3, [r1, #4]
.L447:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r10, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r2, r4, fp
	movw	r3, #4916
	ldrb	r3, [r2, r3]	@ zero_extendqisi2
	movw	r2, #4900
	add	r3, r3, r4
	ldrb	r3, [r3, r2]	@ zero_extendqisi2
	cbz	r3, .L448
	add	r5, fp, #1232
	add	r3, r4, r5, lsl #2
	ldr	r3, [r3, #4]
	cmp	r3, #0
	ble	.L448
	mov	r3, #800
	movs	r2, #0
	mla	r3, r3, fp, r4
	add	r3, r3, #17792
	adds	r3, r3, #8
.L451:
	ldrd	r0, [r3], #8
	cmp	r7, r1
	it	eq
	cmpeq	r6, r0
	beq	.L449
	orrs	r1, r0, r1
	bne	.L450
	movs	r3, #100
	mla	fp, r3, fp, r2
	addw	r3, fp, #625
	addw	fp, fp, #2225
	add	r3, r4, r3, lsl #3
	add	fp, r4, fp, lsl #3
	strd	r8, [r3]
	strd	r6, [fp]
.L449:
	add	r4, r4, r5, lsl #2
	ldr	r3, [r4, #4]
	subs	r3, r3, #1
	str	r3, [r4, #4]
.L448:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r10	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L468:
	movs	r0, #0
	b	.L442
.L450:
	adds	r2, r2, #1
	cmp	r2, #100
	bne	.L451
	mov	r3, #844
	str	r2, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L469+4
	movs	r0, #4
	ldr	r2, .L469+12
	bl	vlog
	b	.L449
.L470:
	.align	2
.L469:
	.word	.LC0
	.word	.LANCHOR1+409
	.word	g_venc_dev
	.word	.LC29
	.fnend
	.size	vcodec_set_intra_info, .-vcodec_set_intra_info
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
	ldr	r3, [r1, #8]
	cmp	r3, #16
	beq	.L472
	movw	r3, #1310
	ldr	r2, .L476
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L476+4
	movs	r0, #4
	bl	vlog
.L475:
	mov	r0, #-1
.L471:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L472:
	ldr	r0, [r1, #4]
	cmp	r0, #15
	ble	.L474
	strd	r0, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1314
	ldr	r2, .L476+8
	str	r3, [sp]
	movs	r0, #4
	ldr	r3, .L476+4
	bl	vlog
	b	.L475
.L474:
	bl	get_venc_node
	clz	r0, r0
	lsrs	r0, r0, #5
	rsbs	r0, r0, #0
	b	.L471
.L477:
	.align	2
.L476:
	.word	.LC14
	.word	.LANCHOR1+431
	.word	.LC0
	.fnend
	.size	venc_node_check, .-venc_node_check
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	release_phy_buf, %function
release_phy_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	mov	r4, r0
	ldr	r2, [r0, #312]
	cbz	r2, .L478
	ldr	r3, .L484
	movs	r1, #4
	mov	r0, r1
	strd	r3, r2, [sp, #4]
	mov	r2, #568
	str	r2, [sp]
	ldr	r2, .L484+4
	bl	vlog
	ldr	r3, .L484+8
	ldr	r2, [r4, #312]
	ldr	r1, [r3]
	cmp	r2, r1
	itt	eq
	moveq	r1, #0
	streq	r1, [r3]
	ldr	r1, [r3, #4]
	cmp	r2, r1
	itt	eq
	moveq	r2, #0
	streq	r2, [r3, #4]
	ldrd	r2, r3, [r3]
	orrs	r3, r2, r3
	bne	.L478
	add	sp, sp, #16
	@ sp needed
	pop	{r4, lr}
	b	rockit_free_tb_stream_buf
.L478:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L485:
	.align	2
.L484:
	.word	.LANCHOR1+447
	.word	.LC30
	.word	.LANCHOR0
	.fnend
	.size	release_phy_buf, .-release_phy_buf
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
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r5, [r3, #160]
	ldr	r3, [r1, #32]
	cmp	r3, #76
	beq	.L487
	movw	r3, #1289
	ldr	r2, .L491
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L491+4
	movs	r0, #4
	bl	vlog
	mov	r0, #-1
.L486:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L487:
	movs	r3, #0
	movs	r2, #1
	str	r3, [r1, #8]
	strd	r2, r3, [r1]
	str	r3, [r1, #36]
	str	r3, [r1, #56]
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
	b	.L486
.L492:
	.align	2
.L491:
	.word	.LC31
	.word	.LANCHOR1+463
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
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r2, [r1, #12]
	ldr	r0, [r0, #128]
	cmp	r2, #24
	beq	.L494
	movw	r3, #1483
	ldr	r2, .L500
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L500+4
	movs	r0, #4
	bl	vlog
.L499:
	mov	r0, #-1
.L493:
	add	sp, sp, #20
	@ sp needed
	ldr	pc, [sp], #4
.L494:
	ldr	r3, [r1]
	cmp	r3, #15
	ble	.L496
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	mov	r3, #1488
	ldr	r2, .L500+8
	str	r3, [sp]
	ldr	r3, .L500+4
	bl	vlog
	b	.L499
.L496:
	mla	r3, r2, r3, r0
	add	r2, r1, #24
	addw	r3, r3, #3396
.L497:
	ldr	r0, [r1], #4	@ unaligned
	cmp	r1, r2
	str	r0, [r3], #4	@ unaligned
	bne	.L497
	movs	r0, #0
	b	.L493
.L501:
	.align	2
.L500:
	.word	.LC32
	.word	.LANCHOR1+480
	.word	.LC0
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
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r2, [r1, #12]
	ldr	r0, [r0, #128]
	cmp	r2, #24
	beq	.L503
	movw	r3, #1502
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
	ldr	pc, [sp], #4
.L503:
	ldr	r3, [r1]
	cmp	r3, #15
	ble	.L505
	movs	r2, #16
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1507
	ldr	r2, .L509+8
	str	r3, [sp]
	ldr	r3, .L509+4
	bl	vlog
	b	.L508
.L505:
	mla	r3, r2, r3, r0
	addw	r3, r3, #3396
	add	r2, r3, #24
.L506:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r2
	str	r0, [r1], #4	@ unaligned
	bne	.L506
	movs	r0, #0
	b	.L502
.L510:
	.align	2
.L509:
	.word	.LC32
	.word	.LANCHOR1+499
	.word	.LC0
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
	push	{r0, r1, r2, r3, r4, lr}
	.save {lr}
	.pad #20
	ldr	r2, [r1, #4]
	ldr	r3, [r0, #128]
	cmp	r2, #16
	beq	.L512
	movw	r3, #1793
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
	ldr	pc, [sp], #4
.L512:
	ldr	r2, [r1]
	cmp	r2, #0
	ble	.L514
	movs	r3, #1
	movs	r1, #2
	strd	r2, r3, [sp, #4]
	movs	r0, #4
	movw	r3, #1798
	ldr	r2, .L518+8
	str	r3, [sp]
	ldr	r3, .L518+4
	bl	vlog
	b	.L517
.L514:
	add	r3, r3, #31616
	add	r2, r1, #16
	adds	r3, r3, #4
.L515:
	ldr	r0, [r1], #4	@ unaligned
	cmp	r1, r2
	str	r0, [r3], #4	@ unaligned
	bne	.L515
	movs	r0, #0
	b	.L511
.L519:
	.align	2
.L518:
	.word	.LC28
	.word	.LANCHOR1+518
	.word	.LC0
	.fnend
	.size	venc_node_pp_set_md_attr, .-venc_node_pp_set_md_attr
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
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r6, r1
	ldr	r3, [r1, #8]
	.pad #80
	sub	sp, sp, #80
	cmp	r3, #60
	beq	.L521
	movw	r3, #1599
	ldr	r2, .L546
	str	r3, [sp]
	ldr	r3, .L546+4
.L543:
	movs	r1, #2
	movs	r0, #4
	bl	vlog
	b	.L544
.L521:
	ldr	r3, [r1, #4]
	cmp	r3, #0
	ble	.L523
	movs	r2, #1
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1603
	ldr	r2, .L546+8
	str	r3, [sp]
	ldr	r3, .L546+4
	bl	vlog
.L544:
	mov	r8, #-1
.L520:
	mov	r0, r8
	add	sp, sp, #80
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L523:
	ldr	r3, [r1, #16]
	movw	r2, #4116
	ldr	r5, .L546+12
	movs	r4, #5
	sbfx	r3, r3, #1, #29
	ldr	r7, .L546+16
	adds	r3, r3, #15
	sub	r8, r5, #104
	asrs	r3, r3, #4
	lsls	r3, r3, #8
	str	r3, [r7, r2]
.L527:
	ldr	r0, [r8]
	bl	mpi_buf_alloc
	str	r0, [r5], #4
	cbnz	r0, .L524
	movw	r3, #1611
.L545:
	str	r3, [sp]
	ldr	r2, .L546+20
	ldr	r3, .L546+4
	b	.L543
.L524:
	bl	mpi_buf_map
	cbnz	r0, .L526
	movw	r3, #1615
	b	.L545
.L526:
	ldr	r3, [r5, #-4]
	movs	r1, #0
	ldr	r2, [r8]
	ldr	r0, [r3, #220]
	bl	memset
	subs	r4, r4, #1
	bne	.L527
	ldr	r3, [r6, #20]
	cbz	r3, .L528
	ldr	r3, [r6, #12]
	sbfx	r4, r3, #1, #29
	ldr	r3, [r6, #16]
	adds	r4, r4, #3
	asrs	r4, r4, #2
	adds	r4, r4, #7
	asrs	r2, r4, #3
	sbfx	r4, r3, #1, #29
	adds	r4, r4, #15
	asrs	r4, r4, #4
	muls	r4, r2, r4
	lsls	r4, r4, #6
	mov	r0, r4
	bl	mpi_buf_alloc
	movw	r3, #4764
	str	r0, [r7, r3]
	cbnz	r0, .L529
.L530:
	movw	r3, #1627
	b	.L545
.L529:
	mov	r0, r4
	bl	mpi_buf_alloc
	movw	r3, #4772
	str	r0, [r7, r3]
	cmp	r0, #0
	beq	.L530
.L528:
	ldr	r3, [r6, #12]
	add	r1, sp, #20
	ldr	r0, [r6, #4]
	str	r3, [sp, #20]
	ldr	r3, [r6, #16]
	str	r3, [sp, #24]
	ldr	r3, [r6, #28]
	str	r3, [sp, #36]
	ldr	r3, [r6, #32]
	str	r3, [sp, #40]
	ldr	r3, [r6, #20]
	str	r3, [sp, #28]
	ldr	r3, [r6, #24]
	str	r3, [sp, #32]
	movs	r3, #0
	str	r3, [sp, #44]
	bl	vepu_pp_create_chn
	mov	r8, r0
	cbz	r0, .L531
	movw	r3, #1642
	b	.L545
.L531:
	mov	r3, r0
	movs	r2, #1
	ldr	r1, [r6, #4]
	ldr	r0, .L546+24
	bl	mpi_dev_create_node_with_params
	mov	ip, r0
	cbnz	r0, .L532
	mov	r3, #1648
	ldr	r2, .L546+28
	str	r3, [sp]
	ldr	r3, .L546+4
	b	.L543
.L532:
	add	r5, sp, #20
	ldr	r4, .L546+32
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	mov	r3, r6
	ldm	r5, {r0, r1, r2}
	add	r5, r6, #56
	stm	r4, {r0, r1, r2}
	adds	r4, r4, #64
.L533:
	ldr	r0, [r3]	@ unaligned
	adds	r3, r3, #8
	ldr	r1, [r3, #-4]	@ unaligned
	cmp	r3, r5
	mov	r2, r4
	stmia	r2!, {r0, r1}
	mov	r4, r2
	bne	.L533
	ldr	r0, [r3]	@ unaligned
	mov	r3, ip
	str	r0, [r2]
	movs	r0, #20
	ldrd	r1, r2, [r6]
	bl	vsys_add_node_to_map
	movs	r3, #0
	mov	r2, #-1
	str	r3, [r7, #180]
	ldr	r1, .L546+16
	ldr	r3, .L546+36
	ldr	r0, .L546+40
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r4, r0
	bhi	.L534
	bl	wake_up_process
.L534:
	str	r4, [r7, #168]
	b	.L520
.L547:
	.align	2
.L546:
	.word	.LC33
	.word	.LANCHOR1+543
	.word	.LC0
	.word	g_venc_dev+4220
	.word	g_venc_dev
	.word	.LC34
	.word	g_venc_dev+4
	.word	.LC35
	.word	g_venc_dev+4048
	.word	.LC12
	.word	venc_thread_fn
	.fnend
	.size	venc_node_pp_create, .-venc_node_pp_create
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	send_phy_buf.constprop.14, %function
send_phy_buf.constprop.14:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r0
	.pad #36
	sub	sp, sp, #36
	ldr	r0, .L555
	bl	mpi_dev_find_by_name
	mov	r7, r0
	movs	r0, #0
	bl	get_venc_node
	mov	r6, r0
	cmp	r5, #0
	beq	.L548
	movs	r1, #0
	mov	r0, r1
	bl	mpi_buf_create
	mov	r4, r0
	cmp	r0, #0
	beq	.L548
	ldr	r3, .L555+4
	movs	r1, #0
	str	r3, [r0, #4]
	ldr	r3, .L555+8
	ldr	r2, [r3, #288]
	str	r2, [r0, #236]
	ldr	r2, [r3, #292]
	str	r2, [r0, #240]
	ldr	r2, [r3, #300]
	str	r2, [r0, #244]
	ldr	r2, [r3, #304]
	str	r2, [r0, #248]
	mov	r2, #1000
	ldr	r3, [r3, #308]
	str	r3, [r0, #256]
	add	r3, sp, #28
	movs	r0, #0
	bl	div_s64_rem
	movs	r3, #0
	ldr	r2, .L555+12
	str	r3, [r4, #284]
	movw	r3, #599
	strd	r0, [r4, #288]
	movs	r1, #4
	str	r3, [sp]
	mov	r0, r1
	ldr	r3, .L555+16
	str	r5, [sp, #4]
	bl	vlog
	ldr	r3, .L555+20
	str	r5, [r4, #312]
	mov	r2, r4
	movs	r1, #1
	mov	r0, r6
	str	r3, [r4, #196]
	ldr	r3, [r7, #80]
	blx	r3
	ldr	r3, [r4, #248]
	movs	r1, #4
	ldr	r2, .L555+24
	mov	r0, r1
	str	r3, [sp, #16]
	ldr	r3, [r4, #244]
	str	r3, [sp, #12]
	ldr	r3, [r4, #240]
	str	r3, [sp, #8]
	ldr	r3, [r4, #236]
	str	r3, [sp, #4]
	movw	r3, #606
	str	r3, [sp]
	ldr	r3, .L555+16
	bl	vlog
	mov	r0, r4
	bl	mpi_obj_unref
.L548:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L556:
	.align	2
.L555:
	.word	.LC13
	.word	.LC12
	.word	g_venc_dev
	.word	.LC36
	.word	.LANCHOR1+563
	.word	release_phy_buf
	.word	.LC37
	.fnend
	.size	send_phy_buf.constprop.14, .-send_phy_buf.constprop.14
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
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r1
	ldr	r3, [r1, #20]
	.pad #20
	sub	sp, sp, #20
	ldr	r4, [r1]
	cmp	r3, #36
	beq	.L558
	movw	r2, #1410
	ldr	r1, .L586
	ldr	r0, .L586+4
	bl	printk
.L585:
	mov	r4, #-1
.L557:
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L558:
	cmp	r4, #15
	ble	.L560
	movs	r3, #16
	ldr	r2, .L586+8
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #1415
	movs	r0, #4
	str	r3, [sp]
	ldr	r3, .L586
	bl	vlog
	b	.L585
.L560:
	ldr	r3, [r1, #8]
	movs	r0, #4
	ldr	r2, .L586+12
	str	r3, [sp, #12]
	ldr	r3, [r1, #4]
	movs	r1, #5
	strd	r4, r3, [sp, #4]
	movw	r3, #1418
	str	r3, [sp]
	ldr	r3, .L586
	bl	vlog
	ldr	r3, .L586+16
	add	r2, r3, r4, lsl #2
	ldr	r2, [r2, #212]
	cbnz	r2, .L561
	ldr	r2, [r5, #4]
	cbz	r2, .L562
.L561:
	cbnz	r4, .L564
	strd	r4, r4, [r3, #312]
	strd	r4, r4, [r3, #320]
.L565:
	ldr	r2, [r5, #4]
	cbz	r2, .L564
	str	r2, [r3, #212]
	b	.L564
.L562:
	cmp	r4, #0
	beq	.L565
.L564:
	mov	r0, r4
	bl	get_venc_node
	cbz	r0, .L567
	mov	r0, r4
	bl	get_venc_node
	bl	mpi_node_streamon
.L567:
	cbnz	r4, .L571
	ldr	r3, .L586+20
	ldr	r6, [r3]
	cbz	r6, .L568
	movs	r2, #1
	str	r2, [r3, #8]
.L568:
	ldr	r2, [r3, #4]
	cbz	r2, .L569
	movs	r2, #1
	str	r2, [r3, #12]
.L569:
	ldr	r3, .L586+24
	add	r7, r5, #32
.L570:
	ldr	r0, [r5]	@ unaligned
	adds	r5, r5, #8
	ldr	r1, [r5, #-4]	@ unaligned
	cmp	r5, r7
	mov	r2, r3
	stmia	r2!, {r0, r1}
	mov	r3, r2
	bne	.L570
	ldr	r0, [r5]	@ unaligned
	str	r0, [r2]
	mov	r0, r6
	bl	send_phy_buf.constprop.14
	b	.L557
.L571:
	movs	r4, #0
	b	.L557
.L587:
	.align	2
.L586:
	.word	.LANCHOR1+576
	.word	.LC38
	.word	.LC0
	.word	.LC39
	.word	g_venc_dev
	.word	.LANCHOR0
	.word	g_venc_dev+276
	.fnend
	.size	venc_node_start, .-venc_node_start
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
	ldr	r3, [r1, #4]
	.pad #20
	sub	sp, sp, #20
	ldr	r4, [r0, #128]
	cmp	r3, #32
	beq	.L589
	movw	r3, #1761
	ldr	r2, .L607
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L607+4
	movs	r0, #4
	bl	vlog
.L606:
	mov	r0, #-1
.L588:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L589:
	ldr	r3, [r1]
	cmp	r3, #0
	ble	.L591
	movs	r2, #1
	movs	r1, #2
	strd	r3, r2, [sp, #4]
	movs	r0, #4
	movw	r3, #1766
	ldr	r2, .L607+8
	str	r3, [sp]
	ldr	r3, .L607+4
	bl	vlog
	b	.L606
.L591:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movw	r2, #4104
	ldr	r6, [r4, r2]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #4060]
	cbz	r3, .L592
	ldr	r3, [r1, #8]
	cbz	r3, .L592
	movw	r3, #4112
	add	r6, r4, r6, lsl #2
	ldr	r3, [r4, r3]
	movw	r7, #4216
	str	r3, [r1, #20]
	movs	r1, #0
	ldr	r3, [r6, r7]
	ldr	r0, [r3, #64]
	bl	dma_buf_fd
	str	r0, [r5, #16]
	ldr	r3, [r6, r7]
	ldr	r3, [r3, #64]
	ldr	r3, [r3, #4]
	add	r2, r3, #28
	pld	[r2]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r1, [r2]
	add	r1, r1, #1
	strex	r0, r1, [r2]
	teq	r0, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
.L592:
	ldr	r3, [r4, #4064]
	cbz	r3, .L593
	ldr	r3, [r5, #12]
	cbz	r3, .L593
	movw	r3, #4116
	ldr	r3, [r4, r3]
	str	r3, [r5, #24]
	movw	r3, #4120
	ldr	r3, [r4, r3]
	str	r3, [r5, #28]
.L593:
	movw	r3, #4108
	movs	r0, #0
	str	r0, [r4, r3]
	b	.L588
.L608:
	.align	2
.L607:
	.word	.LC40
	.word	.LANCHOR1+592
	.word	.LC0
	.fnend
	.size	venc_node_pp_get_result, .-venc_node_pp_get_result
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
	ldr	r3, .L614
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	mov	r5, r1
	ldr	r0, [r3, #8]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L609
	ldr	r3, [r5]
	mov	r2, r6
	movs	r1, #8
	str	r3, [r0, #68]
	ldr	r3, [r5, #8]
	str	r3, [r0, #80]
	ldr	r3, [r5, #4]
	str	r3, [r0, #76]
	bl	mpi_dev_register
.L609:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L615:
	.align	2
.L614:
	.word	kmalloc_caches
	.fnend
	.size	vcodec_create_dev, .-vcodec_create_dev
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
	mov	r1, #3520
	ldr	r3, .L621
	ldr	r0, [r3, #12]
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L616
	ldr	r2, .L621+4
	movs	r1, #4
	movs	r0, #1
	bl	mpi_queue_create
	mov	r5, r0
	str	r0, [r4]
	cbnz	r0, .L616
	mov	r0, r4
	mov	r4, r5
	bl	kfree
.L616:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L622:
	.align	2
.L621:
	.word	kmalloc_caches
	.word	mpi_obj_unref
	.fnend
	.size	venc_create_ctx, .-venc_create_ctx
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
	bhi	.L629
	movs	r2, #1
	movw	r3, #15375
	lsl	r0, r2, r0
	ands	r3, r0, r3
	bne	.L627
	tst	r0, #960
	bne	.L628
	lsls	r3, r0, #15
	bpl	.L629
	movs	r0, #2
	mov	r3, r2
.L625:
	muls	r1, r3, r1
	asr	r0, r1, r0
	bx	lr
.L627:
	movs	r0, #0
	movs	r3, #2
	b	.L625
.L628:
	mov	r0, r3
	movs	r3, #4
	b	.L625
.L629:
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
	mvn	r5, #42
	ldr	r2, [r1]
	mvn	r4, #84
	ldr	r3, [r1, #4]
	mvn	r8, #106
	ubfx	ip, r2, #16, #8
	ubfx	r7, r2, #8, #8
	uxtb	r2, r2
	mvn	r9, #20
	mov	r10, #150
	mov	fp, #77
	mul	r1, r5, r2
	ubfx	lr, r3, #16, #8
	ubfx	r6, r3, #8, #8
	uxtb	r3, r3
	add	r1, r1, #32768
	muls	r5, r3, r5
	mla	r1, r4, r7, r1
	add	r5, r5, #32768
	add	r1, r1, ip, lsl #7
	mla	r4, r4, r6, r5
	asrs	r1, r1, #8
	strb	r1, [r0]
	lsls	r1, r2, #7
	add	r1, r1, #32768
	add	r4, r4, lr, lsl #7
	mla	r1, r8, r7, r1
	asrs	r4, r4, #8
	strb	r4, [r0, #3]
	mla	r1, r9, ip, r1
	asrs	r1, r1, #8
	strb	r1, [r0, #1]
	movs	r1, #29
	mul	ip, r1, ip
	mla	r7, r10, r7, ip
	mla	r2, fp, r2, r7
	asrs	r2, r2, #8
	strb	r2, [r0, #2]
	lsls	r2, r3, #7
	add	r2, r2, #32768
	mla	r8, r8, r6, r2
	mla	r9, r9, lr, r8
	mul	lr, r1, lr
	mla	r6, r10, r6, lr
	asr	r9, r9, #8
	strb	r9, [r0, #4]
	mla	r3, fp, r3, r6
	asrs	r3, r3, #8
	strb	r3, [r0, #5]
	movs	r3, #0
	strb	r3, [r0, #6]
	movs	r3, #255
	strb	r3, [r0, #7]
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r1
	mov	r5, r2
	mov	r4, r0
	cbz	r0, .L632
	ldr	r0, [r0, #64]
	bl	dma_buf_vmap
	mul	r2, r6, r5
	movs	r1, #255
	str	r0, [r4, #220]
	bl	memset
.L632:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
	.fnend
	.size	set_invert_buffer, .-set_invert_buffer
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
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r5, r1
	ldr	r8, [r1, #8]
	mov	r7, r0
	ldr	r9, [r1, #68]
	mov	r4, r2
	ldr	r0, [r8, #44]
	mov	r1, r9
	bl	__aeabi_uidiv
	ldr	r1, [r5, #72]
	subs	r3, r1, #1
	adds	r0, r0, #7
	lsrs	r6, r0, #3
	ldr	r0, [r8, #48]
	add	r3, r3, r0
	rsbs	r0, r1, #0
	ands	r0, r0, r3
	bl	__aeabi_uidiv
	cmp	r9, #32
	mov	r3, #140
	mov	r8, r0
	beq	.L637
	cmp	r9, #64
	beq	.L638
	cmp	r9, #16
	bne	.L639
	mla	r3, r3, r4, r7
	movs	r2, #0
.L644:
	str	r2, [r3, #76]
.L640:
	movs	r2, #140
	movs	r3, #0
	mla	r4, r2, r4, r7
	movs	r2, #1
	str	r3, [r4, #64]
	strd	r2, r2, [r4, #56]
	strd	r3, r3, [r4, #68]
	str	r6, [r4, #80]
	ldr	r3, [r5, #80]
	cbz	r3, .L641
	cmp	r3, r2
	beq	.L642
.L636:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L637:
	mla	r3, r3, r4, r7
	movs	r2, #1
	b	.L644
.L638:
	mla	r3, r3, r4, r7
	movs	r2, #3
	b	.L644
.L639:
	ldr	r0, .L646
	bl	printk
	b	.L640
.L641:
	ldr	r3, [r5, #76]
	cmp	r3, #80
	bls	.L636
.L645:
	mov	r2, r6
	mov	r1, r8
	ldr	r0, [r5, #12]
	bl	set_invert_buffer
	str	r0, [r4, #88]
	b	.L636
.L642:
	ldr	r3, [r5, #76]
	cmp	r3, #79
	bhi	.L636
	b	.L645
.L647:
	.align	2
.L646:
	.word	.LC41
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
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r3, [r1, #92]
	mov	r6, r1
	cbnz	r3, .L649
	str	r3, [r0, #4]
.L648:
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L649:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r10, .L685+12
	movs	r5, #0
	ldr	r9, [r1, #84]
	ldr	fp, .L685+16
.L651:
	add	r2, r6, #84
	cmp	r9, r2
	bne	.L664
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	str	r5, [r4, #4]
	b	.L648
.L664:
	ldr	r2, [r9, #8]
	ldr	r2, [r2, #1084]
	cmp	r2, #0
	beq	.L652
	movs	r2, #140
	ldr	r1, [r9, #16]
	mla	r2, r2, r5, r4
	adds	r1, r1, #0
	it	ne
	movne	r1, #1
	str	r1, [r2, #8]
	ldr	r1, [r9, #24]
	str	r1, [r2, #28]
	ldr	r1, [r9, #28]
	str	r1, [r2, #32]
	ldr	r1, [r9, #8]
	ldr	r0, [r9, #24]
	ldr	r1, [r1, #44]
	add	r1, r1, r0
	subs	r1, r1, #1
	str	r1, [r2, #36]
	ldr	r1, [r9, #8]
	ldr	r0, [r9, #28]
	ldr	r1, [r1, #48]
	add	r1, r1, r0
	subs	r1, r1, #1
	str	r1, [r2, #40]
	ldr	r1, [r9, #8]
	ldr	r0, [r1, #1084]
	str	r0, [r2, #136]
	cbz	r0, .L653
	bl	mpi_obj_ref
.L653:
	ldr	r2, [r9, #8]
	ldrd	r0, r1, [r2, #40]
	bl	to_osd_stride
	movs	r2, #140
	mla	r2, r2, r5, r4
	str	r0, [r2, #44]
	movs	r0, #1
	ldr	r2, [r9, #8]
	ldr	r2, [r2, #40]
	sub	r2, r2, #65536
	subs	r2, r2, #10
	cmp	r2, #16
	itett	ls
	ldrls	r1, .L685
	movhi	r1, #-1
	addls	r2, r1, r2, lsl #2
	ldrls	r1, [r2, #616]
	movs	r2, #140
	mla	r2, r2, r5, r4
	str	r1, [r2, #20]
	movs	r1, #0
	strd	r1, r0, [r2, #12]
	ldr	r2, [r9, #8]
	ldr	r2, [r2, #40]
	sub	r2, r2, #65536
	subs	r2, r2, #10
	cmp	r2, #16
	bhi	.L655
	lsl	r2, r0, r2
	movw	r1, #9797
	ands	r1, r2, r1
	bne	.L656
	ldr	r0, .L685+4
	tst	r2, r0
	bne	.L667
.L655:
	mov	r0, fp
	bl	printk
	mov	r0, #-1
.L656:
	movs	r1, #140
	mla	r1, r1, r5, r4
	str	r0, [r1, #24]
	ldr	r2, [r9, #8]
	ldr	r2, [r2, #40]
	sub	r2, r2, #65536
	subs	r2, r2, #10
	cmp	r2, #16
	bhi	.L657
	movs	r0, #1
	lsl	r2, r0, r2
	ands	r1, r2, #207
	bne	.L668
	tst	r2, r10
	bne	.L669
.L657:
	ldr	r0, .L685+8
	bl	printk
	mov	r2, #-1
.L658:
	movs	r0, #140
	adds	r7, r5, #1
	mla	r1, r0, r5, r4
	str	r2, [r1, #92]
	ldr	r2, [r9, #36]
	str	r2, [r1, #96]
	ldr	r2, [r9, #32]
	str	r2, [r1, #100]
	clz	r2, r2
	lsrs	r2, r2, #5
	str	r2, [r1, #104]
	ldr	r2, [r1, #20]
	cmp	r2, #17
	bne	.L659
	add	r1, r9, #60
	mla	r0, r0, r7, r4
	bl	set_2bpp_color_lut
.L659:
	ldr	r2, [r9, #84]
	cbz	r2, .L660
	ldr	r0, [r9, #12]
	cbz	r0, .L661
	bl	mpi_obj_ref
.L661:
	mov	r2, r5
	mov	r1, r9
	mov	r0, r4
	bl	to_osd_invert_cfg
.L660:
	ldr	r2, [r9, #44]
	cbz	r2, .L662
	movs	r2, #140
	movs	r1, #1
	mla	r5, r2, r5, r4
	movs	r3, #51
	movs	r2, #0
	str	r1, [r5, #108]
	strd	r3, r2, [r5, #120]
	ldr	r2, [r9, #56]
	str	r2, [r5, #116]
	ldr	r2, [r9, #48]
	cmp	r2, #0
	it	ne
	movne	r2, r1
	str	r2, [r5, #112]
.L662:
	mov	r5, r7
.L652:
	ldr	r9, [r9]
	b	.L651
.L667:
	mov	r0, r1
	b	.L656
.L668:
	mov	r2, r0
	b	.L658
.L669:
	mov	r2, r1
	b	.L658
.L686:
	.align	2
.L685:
	.word	.LANCHOR1
	.word	72074
	.word	.LC43
	.word	81664
	.word	.LC42
	.fnend
	.size	set_osd_data, .-set_osd_data
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
	bhi	.L688
	cmp	r3, #65536
	bcs	.L691
	cmp	r3, #8
	beq	.L691
	cmp	r3, #10
	ite	eq
	moveq	r1, #2
	movne	r1, #1
.L689:
	bl	__aeabi_idiv
	pop	{r3, pc}
.L688:
	ldr	r2, .L696
	cmp	r3, r2
	bcc	.L693
	adds	r2, r2, #1
	cmp	r3, r2
	bls	.L694
	sub	r3, r3, #65536
	subs	r3, r3, #10
	cmp	r3, #3
	ite	hi
	movhi	r1, #1
	movls	r1, #4
	b	.L689
.L691:
	movs	r1, #2
	b	.L689
.L693:
	movs	r1, #1
	b	.L689
.L694:
	movs	r1, #3
	b	.L689
.L697:
	.align	2
.L696:
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
	beq	.L698
	ldr	r3, .L707
	str	r3, [r0, #4]
	ldr	r3, [r5]
	str	r3, [r0, #236]
	ldr	r3, [r5, #4]
	str	r3, [r0, #240]
	ldr	r1, [r5, #8]
	ldr	r0, [r5, #28]
	bl	to_frame_pixel_stride
	ldr	r2, .L707+4
	add	r7, r2, #684
	str	r0, [r4, #244]
	ldr	r3, [r5, #12]
	str	r3, [r4, #248]
	movs	r3, #0
	ldr	r0, [r5, #28]
.L702:
	lsls	r1, r3, #3
	add	ip, r1, r7
	ldr	ip, [ip, #4]
	cmp	r0, ip
	bne	.L700
	add	r2, r2, r1
	ldr	r3, [r2, #684]
.L701:
	str	r3, [r4, #256]
	cmp	r6, #0
	ldrd	r2, [r5, #40]
	strd	r2, [r4, #288]
	mov	r3, #4
	str	r3, [r4, #200]
	blt	.L698
	ldr	r2, [r5, #48]
	cmp	r2, #15
	bhi	.L698
	ldr	r3, .L707+8
	movs	r1, #1
	add	r2, r3, r2, lsl #4
	str	r1, [r2, #3784]
	ldr	r2, [r5, #48]
	add	r2, r3, r2, lsl #4
	str	r6, [r2, #3788]
	ldr	r2, [r5, #48]
	add	r3, r3, r2, lsl #4
	str	r1, [r3, #3792]
.L698:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L700:
	adds	r3, r3, #1
	cmp	r3, #31
	bne	.L702
	mov	r3, #-1
	b	.L701
.L708:
	.align	2
.L707:
	.word	.LC12
	.word	.LANCHOR1
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
	bhi	.L710
	cmp	r0, #65536
	bcs	.L712
	subs	r0, r0, #9
	cmp	r0, #1
	ite	hi
	movhi	r0, #1
	movls	r0, #2
.L711:
	muls	r0, r1, r0
	bx	lr
.L710:
	ldr	r3, .L717
	cmp	r0, r3
	bcc	.L714
	adds	r3, r3, #1
	cmp	r0, r3
	bls	.L715
	sub	r0, r0, #65536
	subs	r0, r0, #16
	cmp	r0, #3
	ite	hi
	movhi	r0, #1
	movls	r0, #4
	b	.L711
.L712:
	movs	r0, #2
	b	.L711
.L714:
	movs	r0, #1
	b	.L711
.L715:
	movs	r0, #3
	b	.L711
.L718:
	.align	2
.L717:
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
	@ args = 0, pretend = 0, frame = 120
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L823
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #148
	sub	sp, sp, #148
	mov	r6, r0
	mov	r4, r1
	mov	r7, r2
	mov	r9, #0
	mov	r5, r3
.L722:
	ldr	r2, [r3, #3784]
	cmp	r2, #0
	beq	.L720
	ldr	r2, [r3, #3788]
	cmp	r7, r2
	bne	.L720
	add	r3, r5, r9, lsl #4
	ldr	r3, [r3, #3792]
	adds	r2, r3, #1
	beq	.L721
	cmp	r3, #0
	it	le
	movle	r9, #-1
.L721:
	cmp	r7, #15
	bhi	.L765
	movs	r3, #192
	mov	r8, #0
	mla	r3, r3, r7, r5
	mov	r2, r3
.L725:
	ldr	r1, [r2, #328]
	cmp	r1, r6
	beq	.L724
	add	r8, r8, #1
	adds	r2, r2, #12
	cmp	r8, #16
	bne	.L725
	mov	r8, #0
.L727:
	ldr	r2, [r3, #328]
	cmp	r2, #0
	bne	.L726
	movs	r3, #192
	mul	r2, r3, r7
	movs	r3, #12
	mla	r3, r3, r8, r2
	add	r3, r3, r5
	str	r6, [r3, #328]
.L724:
	mov	r10, #192
	mul	r3, r10, r7
	mov	r10, #12
	mla	r10, r10, r8, r3
	add	r10, r10, r5
	ldr	r3, [r10, #332]
	cbnz	r3, .L763
	ldr	r3, .L823+4
	mov	r1, #3520
	ldr	r0, [r3, #44]
	bl	kmem_cache_alloc
	str	r0, [r10, #332]
.L763:
	cmp	r9, #-1
	beq	.L723
	mov	r10, #192
	mul	r3, r10, r7
	mov	r10, #12
	mla	r10, r10, r8, r3
	add	r10, r10, r5
	ldr	r3, [r10, #336]
	cbnz	r3, .L723
	ldr	r3, .L823+4
	mov	r1, #3520
	ldr	r0, [r3, #44]
	bl	kmem_cache_alloc
	str	r0, [r10, #336]
.L723:
	cbnz	r6, .L729
.L822:
	movs	r0, #0
.L719:
	add	sp, sp, #148
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L720:
	add	r9, r9, #1
	adds	r3, r3, #16
	cmp	r9, #16
	bne	.L722
	mov	r9, #-1
	b	.L721
.L726:
	add	r8, r8, #1
	adds	r3, r3, #12
	cmp	r8, #16
	bne	.L727
	movw	r3, #1143
	str	r6, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L823+8
	movs	r0, #4
	ldr	r2, .L823+12
	bl	vlog
	b	.L723
.L765:
	mov	r8, #16
	b	.L723
.L824:
	.align	2
.L823:
	.word	g_venc_dev
	.word	kmalloc_caches
	.word	.LANCHOR1+932
	.word	.LC46
.L729:
	cmp	r4, #0
	beq	.L822
	ldr	r3, [r6, #236]
	str	r3, [r4]
	ldr	r3, [r6, #240]
	str	r3, [r4, #4]
	ldr	r1, [r6, #244]
	ldr	r0, [r6, #256]
	bl	to_frame_hor_stride
	cmp	r7, #0
	str	r0, [r4, #8]
	ldr	r3, [r6, #248]
	str	r3, [r4, #12]
	blt	.L733
	ldr	r10, [r6, #312]
	cmp	r10, #0
	beq	.L734
	ldr	r3, .L825
	ldr	r2, [r3]
	mov	fp, r3
	cmp	r10, r2
	bne	.L735
	ldr	r0, [fp, #4]
	movs	r3, #0
	str	r3, [fp, #8]
	bl	send_phy_buf.constprop.14
.L735:
	ldr	r3, [fp, #4]
	ldr	r2, [r6, #312]
	cmp	r2, r3
	mov	r3, #0
	it	eq
	streq	r3, [fp, #12]
	add	r2, sp, #144
	ite	eq
	moveq	r10, #1
	movne	r10, r3
	add	r3, r10, #2
	movw	fp, #1001
	add	r3, r2, r3, lsl #4
	str	r3, [sp, #28]
.L738:
	movs	r2, #104
	movs	r1, #0
	add	r0, sp, #40
	bl	memset
	add	r0, sp, #40
	bl	rockit_get_tb_stream_info
	mov	r2, #1000
	ldr	r1, [sp, #28]
	add	r3, sp, #36
	ldrd	r0, [r1, #-104]
	bl	div_s64_rem
	add	r3, sp, #144
	strd	r0, [r4, #40]
	add	r3, r3, r10, lsl #4
	ldr	r0, .L825+4
	ldr	r3, [r3, #-76]
	str	r3, [r4, #80]
	movs	r3, #0
	str	r3, [r4, #84]
	ldr	r3, .L825+8
	ldr	r3, [r3, #4]
	blx	r3
	ldrd	r0, [r4, #40]
	orrs	r3, r0, r1
	bne	.L737
	subs	fp, fp, #1
	bne	.L738
.L737:
	ldrd	r2, [r4, #80]
	strd	r0, [sp, #8]
	movs	r1, #4
	str	r10, [sp, #4]
	mov	r0, r1
	strd	r2, [sp, #16]
	mov	r3, #1176
	ldr	r2, .L825+12
	str	r3, [sp]
	ldr	r3, .L825+16
	bl	vlog
.L733:
	cmp	r7, #15
	mov	r3, #0
	strd	r3, r3, [r4, #20]
	bhi	.L741
	movs	r3, #24
	mla	r3, r3, r7, r5
	ldr	r2, [r3, #3400]
	cmp	r2, r7
	itttt	eq
	ldreq	r2, [r3, #3404]
	streq	r2, [r4, #20]
	ldreq	r3, [r3, #3408]
	streq	r3, [r4, #24]
.L741:
	cmp	r8, #16
	mov	r3, #0
	str	r9, [r4, #48]
	str	r3, [r4, #52]
	str	r3, [r4, #60]
	beq	.L743
	movs	r3, #192
	mov	r0, r7
	mul	r2, r3, r7
	movs	r3, #12
	mla	r3, r3, r8, r2
	add	r3, r3, r5
	ldr	r10, [r3, #332]
	bl	get_venc_node
	mov	r1, r0
	cmp	r0, #0
	bne	.L744
.L746:
	movs	r3, #0
	str	r3, [r4, #52]
.L745:
	cmp	r9, #-1
	beq	.L743
	movs	r2, #192
	movs	r3, #12
	muls	r2, r7, r2
	mov	r0, r9
	mla	r8, r3, r8, r2
	add	r8, r8, r5
	ldr	r8, [r8, #336]
	bl	get_venc_node
	mov	r1, r0
	cmp	r0, #0
	bne	.L748
.L749:
	movs	r3, #0
	str	r3, [r4, #60]
.L743:
	ldr	r3, [r6]
	ldr	r2, .L825+20
	str	r3, [r4, #56]
	add	ip, r2, #684
	ldr	r1, [r6, #256]
	movs	r3, #0
.L752:
	ldr	r0, [ip, r3, lsl #3]
	cmp	r1, r0
	bne	.L750
	add	r3, r2, r3, lsl #3
	ldr	r3, [r3, #688]
.L751:
	str	r3, [r4, #28]
	add	r3, r5, r7, lsl #2
	ldr	r3, [r3, #212]
	cmp	r3, #0
	bne	.L768
	ldr	r3, [r6, #200]
	tst	r3, #4
	ite	eq
	moveq	r3, #1
	movne	r3, #0
.L753:
	str	r3, [r4, #64]
	ldr	r3, [r6, #312]
	cbz	r3, .L754
	movs	r3, #1
	str	r3, [r4, #68]
	ldr	r3, [r6, #312]
	str	r3, [r4, #72]
.L754:
	movs	r3, #0
	str	r3, [r4, #88]
	cmp	r7, #0
	bne	.L822
	ldr	r2, [r5, #4056]
	ldr	r3, [r5, #4060]
	orrs	r3, r2, r3
	beq	.L755
	movw	r1, #5500
	movw	r0, #4500
	bl	usleep_range
.L755:
	movs	r2, #112
	movs	r1, #0
	ldr	r0, .L825+24
	bl	memset
	ldr	r3, [r5, #4056]
	cbz	r3, .L756
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r0, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, .L825+28
	ldr	r3, [r6, #284]
	ldr	r2, [r2]
	cmp	r3, r2
	beq	.L769
	movw	r2, #4776
	ldr	r2, [r5, r2]
	cmp	r2, r3
	bne	.L758
	movs	r2, #1
.L757:
	addw	r3, r2, #595
	movw	r1, #4780
	add	r3, r5, r3, lsl #3
	ldr	r3, [r3, #4]
	ldr	ip, [r3, #64]
	movs	r3, #56
	muls	r3, r2, r3
	adds	r7, r5, r3
	add	r3, r3, r1
	add	r3, r3, r5
	str	ip, [r7, r1]
	str	r3, [r4, #88]
	movw	r3, #4892
	str	r2, [r5, r3]
.L758:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r0	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L756:
	ldr	r3, [r5, #4060]
	cmp	r3, #0
	beq	.L822
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r0, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, .L825+32
	movs	r3, #0
	ldr	r6, [r6, #284]
	mov	ip, #52
.L761:
	mul	r2, ip, r3
	ldr	r7, [r2, r1]
	cmp	r6, r7
	bne	.L759
	ldr	r1, [r1, #604]
	movs	r3, #56
	add	r2, r2, r5
	movw	r6, #4240
	muls	r3, r1, r3
	ldr	r6, [r2, r6]
	movw	r2, #4788
	adds	r1, r5, r3
	add	r3, r3, #4768
	adds	r3, r3, #12
	str	r6, [r1, r2]
	add	r5, r5, r3
	str	r5, [r4, #88]
.L760:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r0	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L822
.L734:
	ldrd	r2, [r6, #288]
	strd	r2, [r4, #40]
	ldr	r3, [r6, #284]
	strd	r3, r10, [r4, #80]
	add	r3, r5, r7, lsl #2
	ldr	r3, [r3, #212]
	cmp	r3, #0
	beq	.L733
	bl	mcu_get_wrap_status
	cmp	r0, #2
	mov	fp, r0
	bne	.L767
	ldr	r3, [r5, #324]
	cmp	r3, #0
	beq	.L733
	mov	r0, r7
	bl	get_venc_node
	ldr	r1, [r5, #324]
	ldr	r3, [r0, #248]
	ldr	r0, [r3]
	bl	mpi_queue_push_obj
	cbz	r0, .L740
	str	r10, [r5, #324]
	b	.L733
.L740:
	ldr	r1, [r5, #324]
	movs	r0, #4
	ldrd	r2, [r1, #288]
	strd	r2, [sp, #8]
	ldr	r3, [r1, #284]
	mov	r1, fp
	ldr	r2, .L825+36
	str	r3, [sp, #4]
	movw	r3, #1191
	str	r3, [sp]
	ldr	r3, .L825+16
	bl	vlog
	b	.L733
.L744:
	cmp	r10, #0
	beq	.L746
	mov	r0, r10
	bl	set_osd_data
	ldr	r3, [r10, #4]
	cmp	r3, #0
	beq	.L746
	str	r10, [r4, #52]
	b	.L745
.L748:
	cmp	r8, #0
	beq	.L749
	mov	r0, r8
	bl	set_osd_data
	ldr	r3, [r8, #4]
	cmp	r3, #0
	beq	.L749
	str	r8, [r4, #60]
	b	.L743
.L750:
	adds	r3, r3, #1
	cmp	r3, #31
	bne	.L752
	mov	r3, #-1
	b	.L751
.L826:
	.align	2
.L825:
	.word	.LANCHOR0
	.word	64424500
	.word	arm_delay_ops
	.word	.LC44
	.word	.LANCHOR1+932
	.word	.LANCHOR1
	.word	g_venc_dev+4780
	.word	g_venc_dev+4768
	.word	g_venc_dev+4288
	.word	.LC45
.L768:
	movs	r3, #0
	b	.L753
.L769:
	movs	r2, #0
	b	.L757
.L759:
	adds	r3, r3, #1
	cmp	r3, #10
	bne	.L761
	b	.L760
.L767:
	mov	r0, #-1
	b	.L719
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
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	movw	r2, #31640
	ldr	r4, .L828
	mov	r7, r0
	movs	r1, #0
	ldr	r6, .L828+4
	adds	r5, r4, #4
	mov	r0, r4
	bl	memset
	movs	r2, #16
	mov	r1, #-1
	ldr	r0, .L828+8
	bl	memset
	ldr	r2, .L828+12
	ldr	r1, .L828+16
	addw	r0, r4, #4040
	bl	__init_waitqueue_head
	mov	r1, #3264
	mov	r0, r7
	bl	kstrdup
	mov	r2, r7
	movs	r1, #8
	add	r3, r6, #956
	str	r4, [r4, #164]
	str	r3, [r4, #188]
	movs	r3, #22
	str	r3, [r4, #192]
	str	r0, [r4]
	mov	r0, r5
	bl	mpi_dev_register
	ldr	r3, .L828+20
	addw	r1, r6, #1396
	mov	r0, r5
	str	r3, [r4, #72]
	ldr	r3, .L828+24
	str	r3, [r4, #80]
	ldr	r3, .L828+28
	str	r3, [r4, #84]
	bl	mpi_sysfs_dev_add
	add	r3, r4, #172
	str	r3, [r4, #172]
	str	r3, [r4, #176]
	movs	r3, #0
	str	r3, [r4, #196]
	add	r3, r4, #200
	strd	r3, r3, [r4, #200]
	pop	{r3, r4, r5, r6, r7, pc}
.L829:
	.align	2
.L828:
	.word	g_venc_dev
	.word	.LANCHOR1
	.word	g_venc_dev+4916
	.word	.LANCHOR0+28
	.word	.LC47
	.word	venc_create_ctx
	.word	venc_destroy_ctx
	.word	venc_handle_message
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
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L831
	mov	r0, r4
	bl	mpi_sysfs_dev_remove
	mov	r0, r4
	bl	mpi_dev_unregister
	ldr	r0, [r4, #-4]
	pop	{r4, lr}
	b	kfree
.L832:
	.align	2
.L831:
	.word	g_venc_dev+4
	.fnend
	.size	venc_shutdown, .-venc_shutdown
	.section	.init.text
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
	ldr	r4, .L834
	ldr	r0, .L834+4
	bl	venc_probe
	add	r0, r4, #64
	mov	r1, r4
	bl	vmpi_register_fn2vcocdec
	mov	r0, r4
	bl	register_vmpibuf_func_to_pp
	movs	r0, #0
	pop	{r4, pc}
.L835:
	.align	2
.L834:
	.word	.LANCHOR2
	.word	.LC12
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
	.global	rk_cam_phy_addr2
	.global	rk_cam_phy_addr1
	.global	g_venc_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__func__.39480, %object
	.size	__func__.39480, 20
__func__.39480:
	.ascii	"venc_node_enable_tb\000"
	.type	__func__.39493, %object
	.size	__func__.39493, 18
__func__.39493:
	.ascii	"venc_node_tb_bind\000"
	.type	__func__.39507, %object
	.size	__func__.39507, 21
__func__.39507:
	.ascii	"venc_node_tb_request\000"
	.type	__func__.39048, %object
	.size	__func__.39048, 14
__func__.39048:
	.ascii	"mpi_buf_unref\000"
	.type	__func__.38921, %object
	.size	__func__.38921, 25
__func__.38921:
	.ascii	"venc_handle_node_message\000"
	.type	__func__.39226, %object
	.size	__func__.39226, 17
__func__.39226:
	.ascii	"venc_node_create\000"
	.type	__func__.39241, %object
	.size	__func__.39241, 17
__func__.39241:
	.ascii	"venc_node_delete\000"
	.type	__func__.39406, %object
	.size	__func__.39406, 19
__func__.39406:
	.ascii	"venc_node_pp_start\000"
	.type	__func__.39307, %object
	.size	__func__.39307, 26
__func__.39307:
	.ascii	"venc_node_set_vcodec_attr\000"
	.type	__func__.39321, %object
	.size	__func__.39321, 26
__func__.39321:
	.ascii	"venc_node_get_vcodec_attr\000"
	.type	__func__.39265, %object
	.size	__func__.39265, 21
__func__.39265:
	.ascii	"venc_node_send_frame\000"
	.type	__func__.39417, %object
	.size	__func__.39417, 24
__func__.39417:
	.ascii	"venc_node_pp_send_frame\000"
	.type	__func__.39617, %object
	.size	__func__.39617, 15
__func__.39617:
	.ascii	"venc_thread_fn\000"
	.type	__func__.38960, %object
	.size	__func__.38960, 28
__func__.38960:
	.ascii	"venc_handle_node_pp_message\000"
	.type	__func__.39394, %object
	.size	__func__.39394, 20
__func__.39394:
	.ascii	"venc_node_pp_delete\000"
	.type	__func__.39526, %object
	.size	__func__.39526, 11
__func__.39526:
	.ascii	"venc_ioctl\000"
	.type	__func__.39027, %object
	.size	__func__.39027, 23
__func__.39027:
	.ascii	"vcodec_notify_drop_frm\000"
	.type	__func__.39335, %object
	.size	__func__.39335, 20
__func__.39335:
	.ascii	"venc_node_set_combo\000"
	.type	__func__.39350, %object
	.size	__func__.39350, 19
__func__.39350:
	.ascii	"venc_node_set_snap\000"
	.type	__func__.39467, %object
	.size	__func__.39467, 25
__func__.39467:
	.ascii	"venc_node_pp_get_md_attr\000"
	.type	__func__.39013, %object
	.size	__func__.39013, 22
__func__.39013:
	.ascii	"vcodec_set_intra_info\000"
	.type	__func__.39209, %object
	.size	__func__.39209, 16
__func__.39209:
	.ascii	"venc_node_check\000"
	.type	__func__.38902, %object
	.size	__func__.38902, 16
__func__.38902:
	.ascii	"release_phy_buf\000"
	.type	__func__.39202, %object
	.size	__func__.39202, 17
__func__.39202:
	.ascii	"venc_get_version\000"
	.type	__func__.39279, %object
	.size	__func__.39279, 19
__func__.39279:
	.ascii	"venc_node_set_crop\000"
	.type	__func__.39293, %object
	.size	__func__.39293, 19
__func__.39293:
	.ascii	"venc_node_get_crop\000"
	.type	__func__.39453, %object
	.size	__func__.39453, 25
__func__.39453:
	.ascii	"venc_node_pp_set_md_attr\000"
	.type	__func__.39368, %object
	.size	__func__.39368, 20
__func__.39368:
	.ascii	"venc_node_pp_create\000"
	.type	__func__.38912, %object
	.size	__func__.38912, 13
__func__.38912:
	.ascii	"send_phy_buf\000"
	.type	__func__.39254, %object
	.size	__func__.39254, 16
__func__.39254:
	.ascii	"venc_node_start\000"
	.type	__func__.39433, %object
	.size	__func__.39433, 24
__func__.39433:
	.ascii	"venc_node_pp_get_result\000"
	.type	CSWTCH.112, %object
	.size	CSWTCH.112, 68
CSWTCH.112:
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
	.type	__func__.39154, %object
	.size	__func__.39154, 21
__func__.39154:
	.ascii	"get_mpi_buf_frm_info\000"
	.space	3
	.type	venc_ioctls, %object
	.size	venc_ioctls, 440
venc_ioctls:
	.word	-1068735232
	.word	0
	.word	venc_get_version
	.word	.LC48
	.word	0
	.word	-1072667391
	.word	0
	.word	venc_node_create
	.word	.LC49
	.word	0
	.word	1074029826
	.word	0
	.word	venc_node_delete
	.word	.LC50
	.word	0
	.word	1076126979
	.word	0
	.word	venc_node_start
	.word	.LC51
	.word	0
	.word	1080059140
	.word	0
	.word	venc_node_send_frame
	.word	.LC52
	.word	0
	.word	1075340549
	.word	0
	.word	venc_node_set_crop
	.word	.LC53
	.word	0
	.word	1074554118
	.word	0
	.word	venc_node_set_combo
	.word	.LC54
	.word	0
	.word	1074554119
	.word	0
	.word	venc_node_set_snap
	.word	.LC55
	.word	0
	.word	-1069783800
	.word	0
	.word	venc_node_pp_create
	.word	.LC56
	.word	0
	.word	1074029833
	.word	0
	.word	venc_node_pp_delete
	.word	.LC57
	.word	0
	.word	1074029834
	.word	0
	.word	venc_node_pp_start
	.word	.LC58
	.word	0
	.word	1080059147
	.word	0
	.word	venc_node_pp_send_frame
	.word	.LC59
	.word	0
	.word	-1071618804
	.word	0
	.word	venc_node_pp_get_result
	.word	.LC60
	.word	0
	.word	1074029837
	.word	0
	.word	venc_node_enable_tb
	.word	.LC61
	.word	0
	.word	1074291982
	.word	0
	.word	venc_node_tb_bind
	.word	.LC62
	.word	0
	.word	1074029839
	.word	0
	.word	venc_node_tb_request
	.word	.LC63
	.word	0
	.word	-1072143088
	.word	0
	.word	venc_node_get_crop
	.word	.LC64
	.word	0
	.word	1077699857
	.word	0
	.word	venc_node_set_vcodec_attr
	.word	.LC65
	.word	0
	.word	-1069783790
	.word	0
	.word	venc_node_get_vcodec_attr
	.word	.LC66
	.word	0
	.word	-1072667373
	.word	0
	.word	venc_node_pp_set_md_attr
	.word	.LC67
	.word	0
	.word	-1072667372
	.word	0
	.word	venc_node_pp_get_md_attr
	.word	.LC68
	.word	0
	.word	1074816277
	.word	0
	.word	venc_node_check
	.word	.LC69
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
	.type	__param_str_rk_cam_phy_addr2, %object
	.size	__param_str_rk_cam_phy_addr2, 24
__param_str_rk_cam_phy_addr2:
	.ascii	"rockit.rk_cam_phy_addr2\000"
	.type	__param_str_rk_cam_phy_addr1, %object
	.size	__param_str_rk_cam_phy_addr1, 24
__param_str_rk_cam_phy_addr1:
	.ascii	"rockit.rk_cam_phy_addr1\000"
	.data
	.align	2
	.set	.LANCHOR2,. + 0
	.type	gmpi_buf_ops, %object
	.size	gmpi_buf_ops, 64
gmpi_buf_ops:
	.word	mpi_buf_alloc
	.word	mpi_buf_map
	.word	mpi_buf_unmap
	.word	mpi_buf_ref
	.word	mpi_buf_unref
	.word	mpi_buf_get_dmabuf
	.word	mpi_buffer_queue_create
	.word	mpi_buffer_queue_destroy
	.word	mpi_buffer_queue_push
	.word	mpi_buffer_queue_pop
	.word	dma_buf_import_mpi_buf
	.word	get_mpi_buf_frm_info
	.word	mpi_buffer_pool_create
	.word	mpi_buffer_pool_destroy
	.word	mpi_buffer_pool_request_buf
	.word	mpi_buffer_pool_get_free_num
	.type	gmpidev_ops, %object
	.size	gmpidev_ops, 32
gmpidev_ops:
	.word	vcodec_create_dev
	.word	vcodec_destory_dev
	.word	vcodec_handle_message
	.word	vcodec_get_chnl_ctx
	.word	vcodec_get_chnl_id
	.word	vcodec_get_chnl_type
	.word	vcodec_set_intra_info
	.word	vcodec_notify_drop_frm
	.bss
	.align	3
	.set	.LANCHOR0,. + 0
	.type	rk_cam_phy_addr1, %object
	.size	rk_cam_phy_addr1, 4
rk_cam_phy_addr1:
	.space	4
	.type	rk_cam_phy_addr2, %object
	.size	rk_cam_phy_addr2, 4
rk_cam_phy_addr2:
	.space	4
	.type	phy_addr1_active, %object
	.size	phy_addr1_active, 4
phy_addr1_active:
	.space	4
	.type	phy_addr2_active, %object
	.size	phy_addr2_active, 4
phy_addr2_active:
	.space	4
	.type	__func____LINE__.38933, %object
	.size	__func____LINE__.38933, 4
__func____LINE__.38933:
	.space	4
	.type	__func____LINE__.38943, %object
	.size	__func____LINE__.38943, 4
__func____LINE__.38943:
	.space	4
	.type	frm_cnt.39616, %object
	.size	frm_cnt.39616, 4
frm_cnt.39616:
	.space	4
	.type	__key.39689, %object
	.size	__key.39689, 0
__key.39689:
	.space	4
	.type	g_venc_dev, %object
	.size	g_venc_dev, 31640
g_venc_dev:
	.space	31640
	.section	.init.rodata,"a"
	.type	__setup_str_rk_cam_phy_addr2_setup, %object
	.size	__setup_str_rk_cam_phy_addr2_setup, 18
__setup_str_rk_cam_phy_addr2_setup:
	.ascii	"rk_cam_phy_addr2=\000"
	.type	__setup_str_rk_cam_phy_addr1_setup, %object
	.size	__setup_str_rk_cam_phy_addr1_setup, 18
__setup_str_rk_cam_phy_addr1_setup:
	.ascii	"rk_cam_phy_addr1=\000"
	.section	.init.setup,"aw"
	.align	2
	.type	__setup_rk_cam_phy_addr2_setup, %object
	.size	__setup_rk_cam_phy_addr2_setup, 12
__setup_rk_cam_phy_addr2_setup:
	.word	__setup_str_rk_cam_phy_addr2_setup
	.word	rk_cam_phy_addr2_setup
	.word	0
	.type	__setup_rk_cam_phy_addr1_setup, %object
	.size	__setup_rk_cam_phy_addr1_setup, 12
__setup_rk_cam_phy_addr1_setup:
	.word	__setup_str_rk_cam_phy_addr1_setup
	.word	rk_cam_phy_addr1_setup
	.word	0
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_rk_cam_phy_addr2type229, %object
	.size	__UNIQUE_ID_rk_cam_phy_addr2type229, 38
__UNIQUE_ID_rk_cam_phy_addr2type229:
	.ascii	"rockit.parmtype=rk_cam_phy_addr2:uint\000"
	.type	__UNIQUE_ID_rk_cam_phy_addr1type228, %object
	.size	__UNIQUE_ID_rk_cam_phy_addr1type228, 38
__UNIQUE_ID_rk_cam_phy_addr1type228:
	.ascii	"rockit.parmtype=rk_cam_phy_addr1:uint\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[31m\033[1m%s,%d:chnId %d need less than %d\033"
	.ascii	"[0m\000"
.LC1:
	.ascii	"\033[31m\033[1m%s,%d:chnId %d and %d need less than"
	.ascii	" %d\033[0m\000"
.LC2:
	.ascii	"%s,%d:buf(%d)\000"
.LC3:
	.ascii	"\033[31m\033[1m%s,%d:chn %d error!\033[0m\000"
.LC4:
	.ascii	"\033[31m\033[1m%s,%d:chn %d queue is NULL!\033[0m\000"
.LC5:
	.ascii	"\033[32m\033[1m%s,%d:seq lost = %d, %d\033[0m\000"
.LC6:
	.ascii	"\033[32m\033[1m%s,%d:venc(%s-%d) drop buf(%d)\033[0"
	.ascii	"m\000"
.LC7:
	.ascii	"%s,%d:venc(%s-%d) buf(%d) ready, pending: %d\000"
.LC8:
	.ascii	"\033[31m\033[1m%s,%d:wrap mode lost buf, seq = %d\033"
	.ascii	"[0m\000"
.LC9:
	.ascii	"\033[32m\033[1m%s,%d:venc lost a buf pts = %d\033[0"
	.ascii	"m\000"
.LC10:
	.ascii	"\033[32m\033[1m%s,%d:start wrap buf: %d, %d, %d ,%d"
	.ascii	", seq_id = %d\033[0m\000"
.LC11:
	.ascii	"valloc\000"
.LC12:
	.ascii	"venc\000"
.LC13:
	.ascii	"rkv_enc\000"
.LC14:
	.ascii	"\033[31m\033[1m%s,%d:struct venc_info mismatching\033"
	.ascii	"[0m\000"
.LC15:
	.ascii	"\033[31m\033[1m%s,%d:error getting vdev  %p\033[0m\000"
.LC16:
	.ascii	"\033[31m\033[1m%s,%d:====== error create venc node\033"
	.ascii	"[0m\000"
.LC17:
	.ascii	"\033[31m\033[1m%s,%d:struct venc_frame mismatching\033"
	.ascii	"[0m\000"
.LC18:
	.ascii	"\033[31m\033[1m%s,%d:chnId %d send frame fail: node"
	.ascii	" %px, enc_dev %px, frameFd %d\033[0m\000"
.LC19:
	.ascii	"\033[31m\033[1m%s,%d:chnId %d send frame fail: pp_n"
	.ascii	"ode %px, enc_dev %px, frameFd %d\033[0m\000"
.LC20:
	.ascii	"%s,%d:%s register ok = %p\000"
.LC21:
	.ascii	"%s,%d:venc_pp(%s-%d) buf(%d) ready, pending: %d\000"
.LC22:
	.ascii	"%s,%d:venc(%s-%d) drop buf(%d)\000"
.LC23:
	.ascii	"%s,%d:======error: index = %d\000"
.LC24:
	.ascii	"%s,%d:======error: ioctl is null\000"
.LC25:
	.ascii	"%s,%d:======error venc ioctl\000"
.LC26:
	.ascii	"\033[31m\033[1m%s,%d:struct venc_combo mismatching\033"
	.ascii	"[0m\000"
.LC27:
	.ascii	"\033[31m\033[1m%s,%d:struct venc_snap mismatching\033"
	.ascii	"[0m\000"
.LC28:
	.ascii	"\033[31m\033[1m%s,%d:struct venc_pp_md_attr mismatc"
	.ascii	"hing\033[0m\000"
.LC29:
	.ascii	"\033[31m\033[1m%s,%d:request thumbnail more than %d"
	.ascii	"\033[0m\000"
.LC30:
	.ascii	"\033[32m\033[1m%s,%d:%s: 0x%08x\033[0m\000"
.LC31:
	.ascii	"\033[31m\033[1m%s,%d:struct venc_version mismatchin"
	.ascii	"g\033[0m\000"
.LC32:
	.ascii	"\033[31m\033[1m%s,%d:struct venc_crop mismatching\033"
	.ascii	"[0m\000"
.LC33:
	.ascii	"\033[31m\033[1m%s,%d:struct venc_pp_info mismatchin"
	.ascii	"g\033[0m\000"
.LC34:
	.ascii	"\033[31m\033[1m%s,%d:error\033[0m\000"
.LC35:
	.ascii	"\033[31m\033[1m%s,%d:====== error create venc pp no"
	.ascii	"de\033[0m\000"
.LC36:
	.ascii	"\033[32m\033[1m%s,%d:phy_addr: 0x%08x\033[0m\000"
.LC37:
	.ascii	"\033[32m\033[1m%s,%d:send a buf to venc %d, %d, %d "
	.ascii	",%d\033[0m\000"
.LC38:
	.ascii	"\0013%s[%d] struct venc_frame mismatching\000"
.LC39:
	.ascii	"%s,%d:ch = %d, warp enable = %d, wrap line = %d\000"
.LC40:
	.ascii	"\033[31m\033[1m%s,%d:struct venc_pp_result mismatch"
	.ascii	"ing\033[0m\000"
.LC41:
	.ascii	"Inverted cells do not support size\012\000"
.LC42:
	.ascii	"rbuv_swap not support\000"
.LC43:
	.ascii	"alpha_swap not support\000"
.LC44:
	.ascii	"\033[32m\033[1m%s,%d:frame cnt = %d get pts = %lld,"
	.ascii	" dts = %lld\033[0m\000"
.LC45:
	.ascii	"\033[31m\033[1m%s,%d:re send a buf fail seq = %d, p"
	.ascii	"ts = %d\033[0m\000"
.LC46:
	.ascii	"\033[31m\033[1m%s,%d:buf %px get osd data failed!\033"
	.ascii	"[0m\000"
.LC47:
	.ascii	"&vdev->wait\000"
.LC48:
	.ascii	"VENC_IOCTL_VERSION\000"
.LC49:
	.ascii	"VENC_IOCTL_NODE_CREATE\000"
.LC50:
	.ascii	"VENC_IOCTL_NODE_DELETE\000"
.LC51:
	.ascii	"VENC_IOCTL_NODE_START\000"
.LC52:
	.ascii	"VENC_IOCTL_NODE_SEND_FRAME\000"
.LC53:
	.ascii	"VENC_IOCTL_NODE_SET_CROP\000"
.LC54:
	.ascii	"VENC_IOCTL_NODE_SET_COMBO\000"
.LC55:
	.ascii	"VENC_IOCTL_NODE_SET_SNAP\000"
.LC56:
	.ascii	"VENC_IOCTL_NODE_PP_CREATE\000"
.LC57:
	.ascii	"VENC_IOCTL_NODE_PP_DELETE\000"
.LC58:
	.ascii	"VENC_IOCTL_NODE_PP_START\000"
.LC59:
	.ascii	"VENC_IOCTL_NODE_PP_SEND_FRAME\000"
.LC60:
	.ascii	"VENC_IOCTL_NODE_PP_GET_RESULT\000"
.LC61:
	.ascii	"VENC_IOCTL_NODE_ENABLE_TB\000"
.LC62:
	.ascii	"VENC_IOCTL_NODE_TB_BIND\000"
.LC63:
	.ascii	"VENC_IOCTL_NODE_TB_REQUEST\000"
.LC64:
	.ascii	"VENC_IOCTL_NODE_GET_CROP\000"
.LC65:
	.ascii	"VENC_IOCTL_NODE_SET_VCODEC_ATTR\000"
.LC66:
	.ascii	"VENC_IOCTL_NODE_GET_VCODEC_ATTR\000"
.LC67:
	.ascii	"VENC_IOCTL_NODE_PP_SET_MD_ATTR\000"
.LC68:
	.ascii	"VENC_IOCTL_NODE_PP_GET_MD_ATTR\000"
.LC69:
	.ascii	"VENC_IOCTL_NODE_CHECK\000"
	.section	__param,"a"
	.align	2
	.type	__param_rk_cam_phy_addr2, %object
	.size	__param_rk_cam_phy_addr2, 20
__param_rk_cam_phy_addr2:
	.word	__param_str_rk_cam_phy_addr2
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	rk_cam_phy_addr2
	.type	__param_rk_cam_phy_addr1, %object
	.size	__param_rk_cam_phy_addr1, 20
__param_rk_cam_phy_addr1:
	.word	__param_str_rk_cam_phy_addr1
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	rk_cam_phy_addr1
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
