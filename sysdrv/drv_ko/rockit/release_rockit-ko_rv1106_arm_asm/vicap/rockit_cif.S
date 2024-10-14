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
	.file	"rockit_cif.c"
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
	.type	rockit_rkcif_ioctl, %function
rockit_rkcif_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	rockit_rkcif_ioctl, .-rockit_rkcif_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkcif_open, %function
rockit_rkcif_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	rockit_rkcif_open, .-rockit_rkcif_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkcif_buf_que, %function
rockit_rkcif_buf_que:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r8, r1
	.pad #20
	sub	sp, sp, #20
	mov	r6, r0
	cmp	r0, #0
	beq	.L4
	ldr	r5, [r0, #288]
	cmp	r5, #0
	beq	.L14
	ldrb	r0, [r5]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L4
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r9, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	fp, .L23+8
	movs	r7, #0
.L5:
	movs	r1, #0
	mov	r0, r6
	bl	mpi_node_request_buf
	mov	r4, r0
	cmp	r0, #0
	beq	.L11
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L23
	movs	r0, #8
	str	r3, [sp, #12]
	ldr	r3, [r6]
	str	r3, [sp, #8]
	ldr	r3, [r6, #44]
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	movs	r3, #210
	str	r3, [sp]
	ldr	r3, .L23+4
	bl	vlog
	adds	r1, r5, #4
	movs	r3, #0
.L9:
	ldr	r2, [r1, r3, lsl #3]
	cbz	r2, .L6
	ldr	r0, [r2]
	ldr	r2, [r4]
	cmp	r0, r2
	bne	.L22
.L7:
	add	r10, r5, r3, lsl #3
	movs	r2, #1
	strb	r2, [r10, #8]
	ldr	r1, [r6, #56]
	strd	r6, r4, [r5, #196]
	strb	r2, [r5, #168]
	str	r1, [r5, #184]
	ldr	r2, [r4]
	str	r2, [r5, #180]
	ldr	r2, [r4, #448]
	str	r2, [r5, #228]
	ldr	r2, [r6, #44]
	ldr	r2, [r2, #44]
	str	r2, [r5, #172]
	ldr	r2, [fp]
	cbz	r2, .L10
	add	r0, r5, #168
	blx	r2
	cmp	r0, #0
	bge	.L10
	movs	r2, #0
	mov	r0, r4
	strb	r2, [r10, #8]
	strb	r2, [r5, #1]
	bl	mpi_obj_unref
.L11:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r9	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	adds	r0, r4, #0
	it	ne
	movne	r0, #1
.L4:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L6:
	add	r2, r5, r3, lsl #3
	str	r4, [r2, #4]
	b	.L7
.L22:
	adds	r3, r3, #1
	cmp	r3, #20
	bne	.L9
	b	.L7
.L10:
	adds	r7, r7, #1
	cmp	r7, r8
	bne	.L5
	b	.L11
.L14:
	mov	r0, r5
	b	.L4
.L24:
	.align	2
.L23:
	.word	.LC0
	.word	.LANCHOR0
	.word	.LANCHOR1
	.fnend
	.size	rockit_rkcif_buf_que, .-rockit_rkcif_buf_que
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	cif_destroy_ctx, %function
cif_destroy_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	kfree
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	cif_destroy_ctx, .-cif_destroy_ctx
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkcif_handle_message, %function
rockit_rkcif_handle_message:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r6, r0
	ldr	r2, [r0, #8]
	mov	r5, r1
	cmp	r2, #1
	bne	.L53
	cbnz	r0, .L28
	movw	r3, #269
	ldr	r2, .L56
	str	r3, [sp]
	ldr	r3, .L56+4
.L55:
	movs	r1, #2
.L54:
	movs	r0, #8
	bl	vlog
	b	.L53
.L28:
	ldr	r4, [r0, #288]
	cbz	r4, .L29
	cmp	r1, #5
	beq	.L30
	cmp	r1, #6
	beq	.L31
	cbz	r1, .L32
.L53:
	movs	r5, #0
	b	.L27
.L29:
	movw	r3, #277
	ldr	r2, .L56+8
	str	r3, [sp]
	ldr	r3, .L56+4
	b	.L55
.L32:
	ldrb	r3, [r4]	@ zero_extendqisi2
	cbz	r3, .L27
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cbz	r3, .L27
	mov	r1, r2
	bl	rockit_rkcif_buf_que
.L27:
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L30:
	ldr	r3, [r0, #80]
	cmp	r3, #0
	beq	.L38
	ldrd	r8, r9, [r3, #28]
	ldr	r7, [r3, #40]
	ldr	r3, [r0, #44]
	movs	r0, #8
	ldr	r3, [r3, #44]
	str	r3, [r4, #172]
	mov	r3, #300
	strb	r2, [r4], #168
	str	r3, [sp]
	ldr	r2, .L56+12
	ldr	r3, .L56+4
	bl	vlog
	ldr	r2, .L56+16
	movs	r3, #0
.L34:
	add	r1, r2, r3, lsl #3
	ldr	r1, [r1, #4]
	cmp	r7, r1
	beq	.L33
	adds	r3, r3, #1
	cmp	r3, #24
	bne	.L34
.L33:
	ldr	r5, .L56+20
	mov	r2, r9
	mov	r1, r8
	mov	r0, r4
	add	r3, r5, r3, lsl #3
	ldr	r10, [r3, #48]
	mov	r3, r10
	bl	rkcif_rockit_config_stream
	mov	r3, #302
	ldr	r2, .L56+24
	movs	r1, #5
	movs	r0, #8
	str	r3, [sp]
	add	r3, r5, #21
	strd	r7, r10, [sp, #4]
	bl	vlog
	ldr	r2, .L56+28
	movw	r3, #303
	movs	r1, #5
	movs	r0, #8
	str	r3, [sp]
	add	r3, r5, #21
	bl	vlog
	movs	r1, #20
	mov	r0, r6
	bl	rockit_rkcif_buf_que
	ldr	r2, .L56+32
	movs	r1, #5
	movw	r3, #305
	movs	r0, #8
	str	r3, [sp]
	add	r3, r5, #21
	bl	vlog
	mov	r0, r4
	bl	rkcif_rockit_resume_stream
	movw	r3, #307
	ldr	r2, .L56+36
	movs	r1, #5
	str	r3, [sp]
	add	r3, r5, #21
	b	.L54
.L31:
	mov	r3, #312
	movs	r6, #0
	ldr	r2, .L56+40
	movs	r1, #5
	strb	r6, [r4]
	movs	r0, #8
	str	r3, [sp]
	add	r7, r4, #160
	ldr	r3, .L56+4
	mov	r5, r4
	bl	vlog
.L36:
	ldr	r0, [r5, #4]
	cbz	r0, .L35
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	cbz	r3, .L35
	bl	mpi_obj_unref
	str	r6, [r5, #4]
.L35:
	adds	r5, r5, #8
	cmp	r5, r7
	bne	.L36
	mov	r3, #320
	ldr	r2, .L56+44
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L56+4
	movs	r0, #8
	bl	vlog
	add	r0, r4, #168
	bl	rkcif_rockit_pause_stream
	b	.L53
.L38:
	mvn	r5, #21
	b	.L27
.L57:
	.align	2
.L56:
	.word	.LC1
	.word	.LANCHOR0+21
	.word	.LC2
	.word	.LC3
	.word	.LANCHOR0+48
	.word	.LANCHOR0
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.fnend
	.size	rockit_rkcif_handle_message, .-rockit_rkcif_handle_message
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkcif_compat_ioctl, %function
rockit_rkcif_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	rockit_rkcif_compat_ioctl, .-rockit_rkcif_compat_ioctl
	.align	1
	.global	rockit_rkcif_mpibuf_done
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkcif_mpibuf_done, %function
rockit_rkcif_mpibuf_done:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	.pad #44
	sub	sp, sp, #44
	cbnz	r0, .L60
	movs	r3, #104
	ldr	r2, .L90
	str	r3, [sp]
	ldr	r3, .L90+4
.L89:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
.L81:
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L60:
	ldr	r7, [r0, #28]
	cbnz	r7, .L62
	movs	r3, #109
	ldr	r2, .L90+8
	str	r3, [sp]
	ldr	r3, .L90+4
	b	.L89
.L62:
	ldr	r8, [r7, #288]
	cmp	r8, #0
	bne	.L63
	movs	r3, #117
	ldr	r2, .L90+12
	str	r3, [sp]
	ldr	r3, .L90+4
	b	.L89
.L63:
	ldrb	r6, [r0, #2]	@ zero_extendqisi2
	cbz	r6, .L64
	movs	r1, #1
	mov	r0, r7
	bl	rockit_rkcif_buf_que
	movs	r3, #0
	strb	r3, [r4, #2]
	b	.L81
.L64:
	ldr	r5, [r0, #32]
	cmp	r5, #0
	beq	.L81
	ldr	r3, [r0, #72]
	str	r3, [r5, #624]
	ldr	r3, [r0, #68]
	str	r3, [r5, #620]
	ldr	r3, .L90+16
	ldr	r2, [r0, #88]
	sub	r9, r3, #48
.L66:
	ldr	r1, [r3, r6, lsl #3]
	cmp	r2, r1
	beq	.L65
	adds	r6, r6, #1
	cmp	r6, #24
	bne	.L66
	movs	r3, #139
	ldr	r2, .L90+20
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L90+4
	movs	r0, #8
	bl	vlog
.L65:
	add	r6, r9, r6, lsl #3
	mov	r2, #1000
	ldr	r3, [r6, #52]
	movs	r6, #0
	str	r3, [r5, #640]
	ldr	r3, [r4, #76]
	str	r3, [r5, #628]
	ldr	r3, [r4, #80]
	str	r3, [r5, #632]
	add	r3, sp, #36
	ldrd	r0, [r4, #112]
	bl	div_s64_rem
	ldr	r2, .L90+24
	strd	r0, [r5, #672]
	ldr	r3, [r4, #108]
	str	r3, [r5, #668]
	ldr	r3, [r4, #20]
	str	r3, [r5, #584]
	strd	r0, [sp, #16]
	movs	r1, #5
	str	r3, [sp, #24]
	movs	r0, #8
	ldr	r3, [r4, #80]
	str	r3, [sp, #8]
	ldr	r3, [r4, #76]
	str	r3, [sp, #4]
	movs	r3, #151
	str	r3, [sp]
	ldr	r3, .L90+4
	bl	vlog
	str	r6, [r5, #608]
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	bl	mpi_node_buffer_out
	ldr	r3, [r5, #448]
	cbnz	r3, .L68
	mov	r0, r5
	bl	mpi_obj_unref
	b	.L81
.L68:
	ldr	r10, .L90+4
	mov	r9, r8
	ldr	fp, .L90+28
.L71:
	ldr	r3, [r9, #4]
	mov	r2, fp
	movs	r1, #5
	movs	r0, #8
	strd	r6, r3, [sp, #4]
	movs	r3, #162
	str	r3, [sp]
	mov	r3, r10
	bl	vlog
	ldr	r3, [r9, #4]
	cbz	r3, .L69
	ldr	r2, [r5]
	ldr	r3, [r3]
	cmp	r2, r3
	bne	.L69
	add	r6, r8, r6, lsl #3
	movs	r3, #0
	strb	r3, [r6, #8]
.L70:
	mov	r0, r5
	bl	mpi_obj_unref
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cbz	r3, .L72
	movs	r1, #1
	mov	r0, r7
	bl	rockit_rkcif_buf_que
	cbz	r0, .L72
	movs	r3, #0
	strb	r3, [r4, #1]
	b	.L81
.L69:
	adds	r6, r6, #1
	add	r9, r9, #8
	cmp	r6, #20
	bne	.L71
	b	.L70
.L72:
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	strb	r3, [r8, #1]
	b	.L81
.L91:
	.align	2
.L90:
	.word	.LC10
	.word	.LANCHOR0+240
	.word	.LC11
	.word	.LC12
	.word	.LANCHOR0+48
	.word	.LC15
	.word	.LC13
	.word	.LC14
	.fnend
	.size	rockit_rkcif_mpibuf_done, .-rockit_rkcif_mpibuf_done
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	cif_create_ctx, %function
cif_create_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r2, #2
	mov	r1, #3520
	movw	r0, #14184
	b	kmalloc_order
	.fnend
	.size	cif_create_ctx, .-cif_create_ctx
	.align	1
	.global	rockit_rkcif_dev_register
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkcif_dev_register, %function
rockit_rkcif_dev_register:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r8, r0
	ldr	r5, .L110
	mov	r1, #3264
	movs	r6, #1
	mov	fp, #0
	mov	r10, fp
	ldr	r0, [r5, #8]
	mov	r9, r5
	bl	kmem_cache_alloc
	movs	r2, #164
	movs	r1, #0
	mov	r4, r0
	bl	memset
	movw	r3, #363
	ldr	r2, .L110+4
	movs	r1, #5
	movs	r0, #8
	add	r7, r4, #60
	str	r7, [r4, #60]
	str	r7, [r4, #64]
	str	r3, [sp]
	str	r8, [sp, #4]
	ldr	r3, .L110+8
	bl	vlog
	ldr	r3, .L110+12
	str	r3, [r4, #68]
	ldr	r3, .L110+16
	str	r3, [r4, #76]
	movs	r3, #2
	strd	r3, r6, [r4, #48]
	ldr	r3, [r4, #56]
	orr	r3, r3, #3
	str	r3, [r4, #56]
.L94:
	ldr	r2, [r4, #52]
	ldr	r0, [r9, #20]
	cmp	r2, r10
	bgt	.L98
	ldr	r3, [r4, #48]
	add	fp, fp, #1
	cmp	r3, fp
	bgt	.L103
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r5, r0
	cbz	r0, .L101
	mov	r3, #-1
	movs	r1, #2
	str	r3, [r0, #16]
	strd	r1, r3, [r0, #8]
	mov	r1, #3520
	ldr	r0, [r9, #20]
	bl	kmem_cache_alloc
	str	r0, [r5, #20]
	cbz	r0, .L101
	movs	r3, #16
	movs	r2, #1
	strd	r3, r3, [r0, #8]
	strd	r2, r3, [r0]
	str	r3, [r0, #16]
	ldr	r3, [r4, #64]
	str	r5, [r4, #64]
	strd	r7, r3, [r5]
	str	r5, [r3]
.L101:
	mov	r2, r8
	movs	r1, #8
	mov	r0, r4
	bl	mpi_dev_register
	ldr	r3, .L110+20
	movs	r2, #0
	ldr	r1, .L110+24
	mov	r0, r4
	str	r3, [r4, #80]
	bl	mpi_sysfs_dev_add
	mov	r3, #440
	ldr	r2, .L110+28
	movs	r1, #5
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L110+8
	bl	vlog
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L98:
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r5, r0
	cmp	r0, #0
	beq	.L96
	strd	r6, fp, [r0, #8]
	mov	r1, #3520
	str	r10, [r0, #16]
	ldr	r0, [r9, #4]
	bl	kmem_cache_alloc
	str	r0, [r5, #20]
	cbz	r0, .L96
	mov	r2, #2160
	mov	r3, #3840
	strd	r3, r2, [r0, #8]
	movs	r2, #4
	str	r2, [r0, #36]
	ldr	r2, [r0, #64]
	str	r6, [r0, #32]
	orr	r2, r2, #3
	str	r2, [r0, #64]
	ldr	r2, [r0, #68]
	strd	r6, r6, [r0, #40]
	orr	r2, r2, #3
	str	r2, [r0, #68]
	ldr	r2, [r0, #56]
	orr	r2, r2, #3
	str	r2, [r0, #56]
	ldr	r2, [r0, #60]
	orr	r2, r2, #3
	str	r2, [r0, #60]
	ldr	r2, [r0, #72]
	orr	r2, r2, #3
	str	r2, [r0, #72]
	ldr	r2, [r0, #76]
	orr	r2, r2, #14
	str	r2, [r0, #76]
	ldr	r2, [r4, #64]
	str	r5, [r4, #64]
	strd	r7, r2, [r5]
	str	r5, [r2]
.L96:
	add	r10, r10, #1
	b	.L94
.L103:
	mov	r10, #0
	b	.L94
.L111:
	.align	2
.L110:
	.word	kmalloc_caches
	.word	.LC16
	.word	.LANCHOR0+265
	.word	cif_create_ctx
	.word	cif_destroy_ctx
	.word	rockit_rkcif_handle_message
	.word	.LANCHOR0+292
	.word	.LC17
	.fnend
	.size	rockit_rkcif_dev_register, .-rockit_rkcif_dev_register
	.section	.init.text,"ax",%progbits
	.align	1
	.global	rockit_rkcif_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkcif_init, %function
rockit_rkcif_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	movs	r4, #0
	ldr	r5, .L119
	.pad #32
	sub	sp, sp, #32
	mov	r1, r4
	ldr	r7, .L119+4
	strd	r4, r4, [sp, #20]
	movw	r6, #455
	ldr	r0, [r5]
	str	r4, [sp, #28]
	bl	rkcif_rockit_function_register
	movs	r1, #1
	ldr	r8, .L119+12
	str	r0, [r5]
	ldr	r0, .L119+8
	bl	rkcif_rockit_function_register
	add	r0, sp, #20
	bl	rkcif_rockit_get_cifdev
	add	r5, sp, #20
.L114:
	ldr	r0, [r5], #4
	cbz	r0, .L113
	bl	rockit_rkcif_dev_register
	ldr	r3, [r5, #-4]
	mov	r2, r8
	movs	r1, #5
	movs	r0, #8
	strd	r4, r3, [sp, #4]
	mov	r3, r7
	str	r6, [sp]
	bl	vlog
.L113:
	adds	r4, r4, #1
	cmp	r4, #3
	bne	.L114
	movs	r0, #0
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L120:
	.align	2
.L119:
	.word	.LANCHOR1
	.word	.LANCHOR0+420
	.word	rockit_rkcif_mpibuf_done
	.word	.LC18
	.fnend
	.size	rockit_rkcif_init, .-rockit_rkcif_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	rockit_rkcif_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkcif_exit, %function
rockit_rkcif_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	add	r0, sp, #4
	movs	r3, #0
	strd	r3, r3, [sp, #4]
	str	r3, [sp, #12]
	bl	rkcif_rockit_get_cifdev
	mov	r5, r0
	cbnz	r0, .L121
	add	r4, sp, #4
.L126:
	ldr	r0, [r4], #4
	cbz	r0, .L124
	bl	mpi_dev_find_by_name
	cbz	r0, .L124
	bl	mpi_dev_unregister
.L124:
	adds	r5, r5, #1
	cmp	r5, #3
	bne	.L126
.L121:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
	.fnend
	.size	rockit_rkcif_exit, .-rockit_rkcif_exit
	.global	rockit_cif_buf_que
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.40299, %object
	.size	__func__.40299, 21
__func__.40299:
	.ascii	"rockit_rkcif_buf_que\000"
	.type	__func__.40334, %object
	.size	__func__.40334, 26
__func__.40334:
	.ascii	"rockit_rkcif_node_message\000"
	.space	1
	.type	v4l2_rockit_fmt, %object
	.size	v4l2_rockit_fmt, 192
v4l2_rockit_fmt:
	.word	842094158
	.word	0
	.word	909203022
	.word	2
	.word	1345466932
	.word	7
	.word	1448695129
	.word	9
	.word	1498831189
	.word	10
	.word	875836505
	.word	14
	.word	1431918169
	.word	17
	.word	1498765654
	.word	18
	.word	1346520914
	.word	65536
	.word	1329743698
	.word	65538
	.word	875836498
	.word	65540
	.word	825770306
	.word	131072
	.word	1196573255
	.word	131073
	.word	1195528775
	.word	131074
	.word	1111967570
	.word	131075
	.word	808535874
	.word	131076
	.word	808534599
	.word	131077
	.word	808534338
	.word	131078
	.word	808535890
	.word	131079
	.word	842090306
	.word	131080
	.word	842089031
	.word	131081
	.word	842088770
	.word	131082
	.word	842090322
	.word	131083
	.word	844257602
	.word	131084
	.type	__func__.40281, %object
	.size	__func__.40281, 25
__func__.40281:
	.ascii	"rockit_rkcif_mpibuf_done\000"
	.type	__func__.40372, %object
	.size	__func__.40372, 26
__func__.40372:
	.ascii	"rockit_rkcif_dev_register\000"
	.space	1
	.type	rockit_rkcif_driver_fops, %object
	.size	rockit_rkcif_driver_fops, 128
rockit_rkcif_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	0
	.word	rockit_rkcif_ioctl
	.word	rockit_rkcif_compat_ioctl
	.word	0
	.space	4
	.word	rockit_rkcif_open
	.space	4
	.word	0
	.space	60
	.type	__func__.40384, %object
	.size	__func__.40384, 18
__func__.40384:
	.ascii	"rockit_rkcif_init\000"
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	rockit_cif_buf_que, %object
	.size	rockit_cif_buf_que, 4
rockit_cif_buf_que:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"vvi(%s-%d) processing, bufs: %d\000"
.LC1:
	.ascii	"\033[1;31merr:cif node null\033[0m\000"
.LC2:
	.ascii	"\033[1;31merr:cif node ctx null\033[0m\000"
.LC3:
	.ascii	"MPI_EVENT_STREAM_ON\000"
.LC4:
	.ascii	"fmt index:%d fmt:0x%08x\000"
.LC5:
	.ascii	"rkcif_rockit_config_stream done\000"
.LC6:
	.ascii	"rockit_rkcif_buf_que done\000"
.LC7:
	.ascii	"rkcif_rockit_resume_stream done\000"
.LC8:
	.ascii	"MPI_EVENT_STREAM_OFF\000"
.LC9:
	.ascii	"\033[1;32mrockit cif stream off\033[0m\000"
.LC10:
	.ascii	"\033[1;31merr, rockit_cfg null\033[0m\000"
.LC11:
	.ascii	"\033[1;31merr, node null\033[0m\000"
.LC12:
	.ascii	"\033[1;31mnode->ctx is null\033[0m\000"
.LC13:
	.ascii	"vw:%dvh:%d pts:%lld event:%d\000"
.LC14:
	.ascii	"  %d :%p\000"
.LC15:
	.ascii	"\033[1;31mthe format is not find!\033[0m\000"
.LC16:
	.ascii	"rockit_rkcif_dev_register name = %s\012\000"
.LC17:
	.ascii	"rockit_rkcif_dev_register done\012\000"
.LC18:
	.ascii	"rockit_rkcif_init %d:name = %s\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
