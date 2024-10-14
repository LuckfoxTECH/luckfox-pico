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
	.file	"rockit_isp.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_rockit_get_tb_stream_info:					
	.asciz 	"rockit_get_tb_stream_info"					
__kstrtabns_rockit_get_tb_stream_info:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_rockit_free_tb_stream_buf:					
	.asciz 	"rockit_free_tb_stream_buf"					
__kstrtabns_rockit_free_tb_stream_buf:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_ioctl, %function
rockit_rkisp_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	rockit_rkisp_ioctl, .-rockit_rkisp_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_open, %function
rockit_rkisp_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	rockit_rkisp_open, .-rockit_rkisp_open
	.section	.init.text,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rk_cam_num_setup, %function
rk_cam_num_setup:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r2, #0
	push	{r3, lr}
	.save {r3, lr}
	mov	r1, r2
	bl	simple_strtoul
	ldr	r3, .L4
	str	r0, [r3]
	movs	r0, #1
	pop	{r3, pc}
.L5:
	.align	2
.L4:
	.word	.LANCHOR0
	.fnend
	.size	rk_cam_num_setup, .-rk_cam_num_setup
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	isp_destroy_ctx, %function
isp_destroy_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	kfree
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	isp_destroy_ctx, .-isp_destroy_ctx
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_buf_que, %function
rockit_rkisp_buf_que:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r8, r1
	.pad #28
	sub	sp, sp, #28
	mov	r7, r0
	cmp	r0, #0
	beq	.L8
	ldr	r4, [r0, #288]
	cmp	r4, #0
	beq	.L20
	ldrb	r0, [r4]	@ zero_extendqisi2
	cmp	r0, #0
	beq	.L8
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r9, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	fp, .L27+12
	movs	r5, #0
.L9:
	movs	r1, #0
	mov	r0, r7
	bl	mpi_node_request_buf
	mov	r6, r0
	cbz	r0, .L15
	ldr	r3, [r6]
	movs	r1, #5
	ldr	r2, .L27
	movs	r0, #8
	str	r3, [sp, #12]
	ldr	r3, [r7]
	str	r3, [sp, #8]
	ldr	r3, [r7, #44]
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	mov	r3, #290
	str	r3, [sp]
	ldr	r3, .L27+4
	bl	vlog
	add	r1, r4, #12
	movs	r3, #0
.L13:
	ldr	r2, [r1, r3, lsl #3]
	cbz	r2, .L10
	ldr	r0, [r2]
	ldr	r2, [r6]
	cmp	r0, r2
	bne	.L26
.L11:
	add	r10, r4, r3, lsl #3
	ldrb	r2, [r10, #16]	@ zero_extendqisi2
	cbz	r2, .L14
	ldr	r1, [r6]
	ldr	r0, .L27+8
	bl	printk
	movs	r3, #3
	mov	r0, r6
	str	r3, [r4, #4]
	bl	mpi_obj_unref
.L15:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r9	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	adds	r0, r5, #0
	it	ne
	movne	r0, #1
.L8:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L10:
	add	r2, r4, r3, lsl #3
	str	r6, [r2, #12]
	b	.L11
.L26:
	adds	r3, r3, #1
	cmp	r3, #20
	bne	.L13
	b	.L11
.L14:
	movs	r1, #1
	strb	r1, [r10, #16]
	ldr	r0, [r7, #56]
	strd	r7, r6, [r4, #224]
	strb	r1, [r4, #176]
	str	r0, [r4, #200]
	ldr	r1, [r6]
	str	r1, [r4, #192]
	ldr	r1, [r6, #448]
	str	r1, [r4, #244]
	ldr	r1, [r7, #44]
	ldr	r1, [r1, #44]
	str	r1, [r4, #180]
	ldr	r1, [fp]
	cbnz	r1, .L16
.L17:
	adds	r5, r5, #1
	cmp	r5, r8
	bne	.L9
	b	.L15
.L16:
	add	r0, r4, #176
	str	r2, [sp, #20]
	blx	r1
	cmp	r0, #0
	ldr	r2, [sp, #20]
	bge	.L17
	movs	r3, #3
	strb	r2, [r10, #16]
	mov	r0, r6
	str	r3, [r4, #4]
	bl	mpi_obj_unref
	ldr	r3, [r4, #244]
	cmp	r3, #0
	beq	.L17
	b	.L15
.L20:
	mov	r0, r4
	b	.L8
.L28:
	.align	2
.L27:
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC1
	.word	.LANCHOR2
	.fnend
	.size	rockit_rkisp_buf_que, .-rockit_rkisp_buf_que
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	visp_thread_fn, %function
visp_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	add	r6, r0, #164
	add	r7, r0, #88
	mov	r5, r0
.L30:
	bl	kthread_should_stop
	cbz	r0, .L34
.L31:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L34:
	ldr	r3, [r5, #180]
	cmp	r3, #0
	bne	.L31
	mov	r0, r6
	bl	down
	ldr	r4, [r5, #88]
.L35:
	subs	r4, r4, #48
	add	r3, r4, #48
	cmp	r3, r7
	beq	.L30
	mov	r0, r4
	movs	r1, #0
	bl	mpi_node_buffer_available
	ldr	r4, [r4, #48]
	b	.L35
	.fnend
	.size	visp_thread_fn, .-visp_thread_fn
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_handle_message, %function
rockit_rkisp_handle_message:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r7, r0
	ldr	r3, [r0, #8]
	mov	r5, r1
	mov	r6, r2
	cmp	r3, #1
	beq	.L37
.L65:
	movs	r5, #0
	b	.L38
.L37:
	cbnz	r0, .L39
	movw	r3, #347
	ldr	r2, .L89
	str	r3, [sp]
	ldr	r3, .L89+4
.L88:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
	b	.L65
.L39:
	ldr	r4, [r0, #288]
	cbz	r4, .L40
	add	r8, r4, #176
	cmp	r1, #6
	bhi	.L65
	adr	r3, .L42
	ldr	pc, [r3, r1, lsl #2]
	.p2align 2
.L42:
	.word	.L45+1
	.word	.L65+1
	.word	.L65+1
	.word	.L65+1
	.word	.L44+1
	.word	.L43+1
	.word	.L41+1
	.p2align 1
.L40:
	movw	r3, #359
	ldr	r2, .L89+8
	str	r3, [sp]
	ldr	r3, .L89+4
	b	.L88
.L44:
	cbnz	r2, .L46
.L85:
	mov	r5, #-1
.L38:
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L46:
	ldr	r3, [r0, #56]
	str	r3, [r4, #200]
	ldr	r3, [r0, #44]
	ldr	r3, [r3, #44]
	str	r3, [r4, #180]
	ldr	r3, [r2]
	cmp	r3, #2
	bne	.L47
	mov	r4, r3
.L51:
	mov	r0, r8
	bl	rkisp_rockit_pause_stream
	cbz	r0, .L48
	ldr	r1, .L89+4
	movw	r2, #373
	ldr	r0, .L89+12
.L84:
	bl	printk
	b	.L85
.L48:
	ldrd	r2, r3, [r6, #32]
	mov	r0, r8
	ldr	r1, [r6, #28]
	bl	rkisp_rockit_config_stream
	cbz	r0, .L49
	movw	r2, #379
	ldr	r1, .L89+4
	ldr	r0, .L89+16
	b	.L84
.L49:
	mov	r0, r8
	bl	rkisp_rockit_resume_stream
	cbz	r0, .L50
	movw	r2, #385
	ldr	r1, .L89+4
	ldr	r0, .L89+20
	b	.L84
.L50:
	cmp	r4, #1
	beq	.L65
	movs	r4, #1
	b	.L51
.L47:
	cmp	r3, #1
	bne	.L53
	ldr	r5, [r2, #36]
	movw	r2, #7556
	cbz	r5, .L54
	strb	r3, [r4, r2]
	b	.L65
.L54:
	strb	r5, [r4, r2]
	b	.L38
.L53:
	cmp	r3, #3
	bne	.L65
	ldr	r1, [r2, #12]
	ldr	r3, [r2, #4]
	ldr	r2, [r2, #20]
	add	r2, r2, r1
	cmp	r2, r3
	bgt	.L85
	ldr	r0, [r6, #16]
	ldr	r2, [r6, #24]
	ldr	r5, [r6, #8]
	add	r2, r2, r0
	cmp	r2, r5
	bgt	.L85
	mla	r3, r0, r3, r1
	movs	r5, #0
	str	r3, [r4, #208]
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #8]
	ldr	r1, [r6, #4]
	add	r2, r2, r2, lsr #31
	add	r3, r3, r2, asr #1
	ldr	r2, [r6, #12]
	str	r5, [r4, #216]
	mla	r3, r1, r3, r2
	str	r3, [r4, #212]
	ldr	r3, [r6, #4]
	str	r3, [r4, #220]
	b	.L38
.L45:
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L65
	ldr	r3, [r4, #4]
	movs	r1, #5
	ldr	r2, .L89+24
	movs	r0, #8
	strd	r4, r3, [sp, #4]
	mov	r3, #408
	str	r3, [sp]
	ldr	r3, .L89+4
	bl	vlog
	ldr	r1, [r4, #4]
	cmp	r1, #3
	bne	.L56
.L86:
	movs	r3, #0
	str	r3, [r4, #4]
	b	.L38
.L56:
	cmp	r1, #1
	bne	.L57
	mov	r0, r7
	bl	rockit_rkisp_buf_que
	cmp	r0, #0
	beq	.L65
	b	.L86
.L57:
	cmp	r1, #2
	bne	.L58
	ldr	r0, [r7, #44]
	movs	r2, #0
	movs	r3, #1
	strd	r2, r3, [r4, #4]
	adds	r0, r0, #164
	bl	up
	b	.L38
.L58:
	ldr	r1, [r4, #8]
	cmp	r1, #1
	bne	.L59
	mov	r0, r7
	bl	rockit_rkisp_buf_que
	cmp	r0, #0
	beq	.L65
	movs	r3, #0
	str	r3, [r4, #8]
	b	.L38
.L59:
	movw	r3, #7556
	ldrb	r3, [r4, r3]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L65
	movs	r1, #1
	mov	r0, r7
	bl	rockit_rkisp_buf_que
	b	.L38
.L43:
	movw	r3, #7556
	movs	r1, #1
	strb	r1, [r4]
	ldrb	r3, [r4, r3]	@ zero_extendqisi2
	cbz	r3, .L67
	bl	rockit_rkisp_buf_que
.L61:
	ldr	r0, .L89+28
.L87:
	bl	printk
	b	.L65
.L67:
	ldr	r5, .L89+32
	movs	r4, #10
.L60:
	movs	r1, #20
	mov	r0, r7
	bl	rockit_rkisp_buf_que
	cmp	r0, #0
	bne	.L61
	movs	r0, #100
	bl	msleep
	cbz	r4, .L62
	subs	r4, r4, #1
	ldr	r2, [r7, #56]
	mov	r0, r5
	mov	r1, r4
	bl	printk
	b	.L60
.L62:
	ldr	r1, [r7, #56]
	ldr	r0, .L89+36
	bl	printk
	b	.L61
.L41:
	movs	r5, #0
	mov	r0, r8
	strb	r5, [r4]
	add	r6, r4, #160
	bl	rkisp_rockit_free_stream_buf
.L64:
	ldr	r0, [r4, #12]
	cbz	r0, .L63
	ldrb	r3, [r4, #16]	@ zero_extendqisi2
	cbz	r3, .L63
	bl	mpi_obj_unref
	str	r5, [r4, #12]
	strb	r5, [r4, #16]
.L63:
	adds	r4, r4, #8
	cmp	r6, r4
	bne	.L64
	ldr	r0, .L89+40
	b	.L87
.L90:
	.align	2
.L89:
	.word	.LC2
	.word	.LANCHOR1+21
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC10
	.word	.LC8
	.word	.LC9
	.word	.LC11
	.fnend
	.size	rockit_rkisp_handle_message, .-rockit_rkisp_handle_message
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_compat_ioctl, %function
rockit_rkisp_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	rockit_rkisp_compat_ioctl, .-rockit_rkisp_compat_ioctl
	.align	1
	.global	rockit_rkisp_mpibuf_done
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_mpibuf_done, %function
rockit_rkisp_mpibuf_done:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r5, r0
	cbnz	r0, .L93
	movs	r3, #171
	ldr	r2, .L148
	str	r3, [sp]
	ldr	r3, .L148+4
.L146:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
.L134:
	movs	r0, #0
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L93:
	ldr	r7, [r0, #48]
	cbnz	r7, .L95
	ldr	r2, .L148+8
	ldr	r3, [r2, #4]
	cmp	r3, #9
	add	r1, r3, #1
	str	r1, [r2, #4]
	bgt	.L134
	movs	r3, #176
	ldr	r2, .L148+12
	str	r3, [sp]
	ldr	r3, .L148+4
	b	.L146
.L95:
	ldr	r3, [r0, #28]
	ldr	r6, [r7, #288]
	cmp	r3, #0
	beq	.L97
	movs	r1, #0
	mov	r0, r7
	bl	mpi_node_request_buf
	mov	r4, r0
.L98:
	cmp	r4, #0
	beq	.L134
	ldr	r3, [r7, #80]
	cmp	r3, #0
	beq	.L100
	ldr	r2, [r3, #20]
	cmp	r2, #0
	beq	.L100
	ldr	r2, [r3, #8]
	str	r2, [r4, #624]
	ldr	r2, [r3, #4]
	str	r2, [r4, #620]
	ldr	r2, [r3, #4]
	str	r2, [r4, #628]
	ldr	r3, [r3, #8]
.L145:
	str	r3, [r4, #632]
	mov	r8, #0
	ldr	r3, .L148+16
	ldr	r2, [r5, #96]
	sub	r9, r3, #72
.L103:
	ldr	r1, [r3, r8, lsl #3]
	cmp	r2, r1
	beq	.L102
	add	r8, r8, #1
	cmp	r8, #24
	bne	.L103
	movs	r3, #207
	ldr	r2, .L148+20
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L148+4
	movs	r0, #8
	bl	vlog
.L102:
	add	r8, r9, r8, lsl #3
	mov	r2, #1000
	ldr	r3, [r8, #76]
	str	r3, [r4, #640]
	add	r3, sp, #12
	ldrd	r0, [r5, #120]
	bl	div_s64_rem
	strd	r0, [r4, #672]
	ldr	r10, [r5, #116]
	str	r10, [r4, #668]
	ldr	r8, [r5, #28]
	str	r8, [r4, #584]
	ldr	r3, [r7, #44]
	ldr	r9, [r3, #44]
	mov	r0, r9
	bl	strlen
	add	r0, r0, r9
	ldrb	r0, [r0, #-1]	@ zero_extendqisi2
	subs	r0, r0, #48
	str	r0, [r4, #592]
	ldrb	r3, [r5, #3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L105
	orr	r3, r8, #4
.L106:
	str	r3, [r4, #584]
	movw	r3, #7556
	ldrb	r3, [r6, r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L107
	mov	r1, r10
	bl	mcu_wrap_set_id
.L108:
	movs	r2, #0
	mov	r1, r4
	mov	r0, r7
	bl	mpi_node_buffer_out
	movw	r3, #7556
	ldrb	r3, [r6, r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L109
	ldr	r3, [r7, #44]
	ldr	r0, [r3, #44]
	bl	rkisp_rockit_get_isp_mode
	cmp	r0, #3
	bne	.L147
	ldr	r7, .L148+24
	movs	r5, #0
	ldr	r8, .L148+32
	ldr	r9, .L148+36
.L110:
	add	r3, r6, #7552
	ldr	r3, [r3]
	ldr	r3, [r3]
	ubfx	r3, r3, #0, #11
	cbz	r3, .L112
	ldr	r3, [r7, #4]
	mov	r0, r8
	blx	r3
	ldr	r3, [r9, #4]
	adds	r5, r5, #1
	cmp	r3, r5
	bne	.L110
.L112:
	cbz	r5, .L147
	mov	r3, r5
	movs	r2, #237
	ldr	r1, .L148+4
	ldr	r0, .L148+28
	bl	printk
.L147:
	mov	r0, r4
	bl	mpi_obj_unref
	b	.L134
.L97:
	ldr	r4, [r0, #52]
	b	.L98
.L100:
	ldr	r3, [r5, #80]
	str	r3, [r4, #624]
	ldr	r3, [r5, #76]
	str	r3, [r4, #620]
	ldr	r3, [r5, #84]
	str	r3, [r4, #628]
	ldr	r3, [r5, #88]
	b	.L145
.L105:
	orr	r3, r8, #2
	b	.L106
.L107:
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L108
	movs	r3, #2
	str	r3, [r6, #4]
	b	.L108
.L109:
	cmp	r6, #0
	beq	.L147
	add	r1, r6, #12
.L115:
	ldr	r2, [r1, r3, lsl #3]
	adds	r3, r3, #1
	cbz	r2, .L116
	ldr	r0, [r4]
	ldr	r2, [r2]
	cmp	r0, r2
	bne	.L116
	add	r3, r6, r3, lsl #3
	movs	r2, #0
	strb	r2, [r3, #8]
.L117:
	mov	r0, r4
	bl	mpi_obj_unref
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L134
	movs	r3, #1
	str	r3, [r6, #4]
	b	.L134
.L116:
	cmp	r3, #20
	bne	.L115
	b	.L117
.L149:
	.align	2
.L148:
	.word	.LC12
	.word	.LANCHOR1+47
	.word	.LANCHOR2
	.word	.LC13
	.word	.LANCHOR1+72
	.word	.LC15
	.word	arm_delay_ops
	.word	.LC14
	.word	214748000
	.word	.LANCHOR0
	.fnend
	.size	rockit_rkisp_mpibuf_done, .-rockit_rkisp_mpibuf_done
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	isp_create_ctx, %function
isp_create_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L155
	mov	r1, #3520
	push	{r4, lr}
	.save {r4, lr}
	ldr	r0, [r3, #52]
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L150
	movs	r1, #4
	ldr	r0, .L155+4
	bl	ioremap
	add	r3, r4, #7552
	str	r0, [r3]
.L150:
	mov	r0, r4
	pop	{r4, pc}
.L156:
	.align	2
.L155:
	.word	kmalloc_caches
	.word	-5943292
	.fnend
	.size	isp_create_ctx, .-isp_create_ctx
	.align	1
	.global	rockit_free_tb_stream_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_free_tb_stream_buf, %function
rockit_free_tb_stream_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r1, #3520
	ldr	r3, .L160
	ldr	r0, [r3, #52]
	bl	kmem_cache_alloc
	mov	r5, r0
	cbnz	r0, .L158
	ldr	r0, .L160+4
	bl	printk
	mov	r0, #-1
.L157:
	pop	{r3, r4, r5, pc}
.L158:
	ldr	r3, .L160+8
	movs	r4, #0
	str	r4, [r0, #24]
	str	r3, [r0, #4]
	bl	rkisp_rockit_free_tb_stream_buf
	mov	r0, r5
	bl	kfree
	mov	r0, r4
	b	.L157
.L161:
	.align	2
.L160:
	.word	kmalloc_caches
	.word	.LC16
	.word	.LC17
	.fnend
	.size	rockit_free_tb_stream_buf, .-rockit_free_tb_stream_buf
	.align	1
	.global	rockit_get_tb_stream_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_get_tb_stream_info, %function
rockit_get_tb_stream_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L165
	mov	r1, #3520
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r0, [r3, #52]
	bl	kmem_cache_alloc
	mov	r5, r0
	cbnz	r0, .L163
	ldr	r0, .L165+4
	bl	printk
	mov	r0, #-1
.L162:
	pop	{r4, r5, r6, pc}
.L163:
	ldr	r3, .L165+8
	movs	r4, #0
	mov	r1, r6
	str	r4, [r0, #24]
	str	r3, [r0, #4]
	bl	rkisp_rockit_get_tb_stream_info
	mov	r0, r5
	bl	kfree
	mov	r0, r4
	b	.L162
.L166:
	.align	2
.L165:
	.word	kmalloc_caches
	.word	.LC16
	.word	.LC17
	.fnend
	.size	rockit_get_tb_stream_info, .-rockit_get_tb_stream_info
	.align	1
	.global	rockit_rkisp_dev_register
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_dev_register, %function
rockit_rkisp_dev_register:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r1, #3264
	ldr	r6, .L185
	mov	r8, r0
	mov	r9, #0
	mov	r10, #1
	mov	fp, r9
	ldr	r0, [r6, #8]
	bl	kmem_cache_alloc
	movs	r2, #184
	movs	r1, #0
	mov	r4, r0
	bl	memset
	ldr	r3, .L185+4
	add	r7, r4, #60
	str	r7, [r4, #60]
	str	r7, [r4, #64]
	str	r3, [r4, #68]
	ldr	r3, .L185+8
	str	r3, [r4, #76]
	movs	r3, #2
	strd	r3, r3, [r4, #48]
	movs	r3, #3
	str	r3, [r4, #56]
.L168:
	ldr	r2, [r4, #52]
	ldr	r0, [r6, #20]
	cmp	r2, fp
	bgt	.L172
	ldr	r2, [r4, #48]
	add	r9, r9, #1
	cmp	r2, r9
	bgt	.L178
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r5, r0
	cbz	r0, .L175
	mov	r3, #-1
	movs	r1, #2
	str	r3, [r0, #16]
	strd	r1, r3, [r0, #8]
	mov	r1, #3520
	ldr	r0, [r6, #20]
	bl	kmem_cache_alloc
	str	r0, [r5, #20]
	cbz	r0, .L175
	movs	r3, #16
	movs	r2, #1
	strd	r3, r3, [r0, #8]
	strd	r2, r3, [r0]
	str	r3, [r0, #16]
	ldr	r3, [r4, #64]
	str	r5, [r4, #64]
	strd	r7, r3, [r5]
	str	r5, [r3]
.L175:
	mov	r2, r8
	movs	r1, #8
	mov	r0, r4
	bl	mpi_dev_register
	ldr	r3, .L185+12
	movs	r2, #0
	ldr	r1, .L185+16
	mov	r0, r4
	str	r3, [r4, #80]
	bl	mpi_sysfs_dev_add
	add	r3, r4, #168
	movs	r2, #0
	str	r3, [r4, #172]
	mov	r1, r4
	strd	r2, r3, [r4, #164]
	mov	r3, r8
	mov	r2, #-1
	ldr	r0, .L185+20
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L177
	bl	wake_up_process
.L177:
	str	r5, [r4, #176]
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L172:
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r5, r0
	cmp	r0, #0
	beq	.L170
	strd	r10, r9, [r0, #8]
	mov	r1, #3520
	str	fp, [r0, #16]
	ldr	r0, [r6, #4]
	bl	kmem_cache_alloc
	str	r0, [r5, #20]
	cbz	r0, .L170
	mov	r2, #2160
	mov	r3, #3840
	strd	r3, r2, [r0, #8]
	movs	r2, #4
	str	r2, [r0, #36]
	ldr	r2, [r0, #64]
	str	r10, [r0, #32]
	orr	r2, r2, #3
	str	r2, [r0, #64]
	ldr	r2, [r0, #68]
	strd	r10, r10, [r0, #40]
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
.L170:
	add	fp, fp, #1
	b	.L168
.L178:
	mov	fp, #0
	b	.L168
.L186:
	.align	2
.L185:
	.word	kmalloc_caches
	.word	isp_create_ctx
	.word	isp_destroy_ctx
	.word	rockit_rkisp_handle_message
	.word	.LANCHOR1+264
	.word	visp_thread_fn
	.fnend
	.size	rockit_rkisp_dev_register, .-rockit_rkisp_dev_register
	.section	.init.text
	.align	1
	.global	rockit_rkisp_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_init, %function
rockit_rkisp_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	movs	r4, #0
	ldr	r5, .L194
	.pad #20
	sub	sp, sp, #20
	mov	r1, r4
	strd	r4, r4, [sp, #4]
	ldr	r0, [r5]
	str	r4, [sp, #12]
	bl	rkisp_rockit_function_register
	movs	r1, #1
	str	r0, [r5]
	ldr	r0, .L194+4
	bl	rkisp_rockit_function_register
	add	r0, sp, #4
	bl	rkisp_rockit_get_ispdev
	add	r5, sp, #4
.L189:
	ldr	r0, [r5], #4
	cbz	r0, .L188
	bl	rockit_rkisp_dev_register
.L188:
	adds	r4, r4, #1
	cmp	r4, #3
	bne	.L189
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L195:
	.align	2
.L194:
	.word	.LANCHOR2
	.word	rockit_rkisp_mpibuf_done
	.fnend
	.size	rockit_rkisp_init, .-rockit_rkisp_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	rockit_rkisp_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_rkisp_exit, %function
rockit_rkisp_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #20
	sub	sp, sp, #20
	add	r0, sp, #4
	movs	r3, #0
	strd	r3, r3, [sp, #4]
	str	r3, [sp, #12]
	bl	rkisp_rockit_get_ispdev
	mov	r5, r0
	cbnz	r0, .L196
	add	r4, sp, #4
	movs	r7, #1
.L201:
	ldr	r0, [r4], #4
	cbz	r0, .L199
	bl	mpi_dev_find_by_name
	mov	r6, r0
	cbz	r0, .L199
	str	r7, [r0, #180]
	ldr	r0, [r0, #176]
	bl	kthread_stop
	mov	r0, r6
	bl	mpi_dev_unregister
.L199:
	adds	r5, r5, #1
	cmp	r5, #3
	bne	.L201
.L196:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
	.fnend
	.size	rockit_rkisp_exit, .-rockit_rkisp_exit
	.global	rockit_buf_que
	.global	rk_cam_num
	.global	rk_virtual_vblank
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__func__.40406, %object
	.size	__func__.40406, 21
__func__.40406:
	.ascii	"rockit_rkisp_buf_que\000"
	.type	__func__.40431, %object
	.size	__func__.40431, 26
__func__.40431:
	.ascii	"rockit_rkisp_node_message\000"
	.type	__func__.40380, %object
	.size	__func__.40380, 25
__func__.40380:
	.ascii	"rockit_rkisp_mpibuf_done\000"
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
	.type	rockit_rkisp_driver_fops, %object
	.size	rockit_rkisp_driver_fops, 128
rockit_rkisp_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	0
	.word	rockit_rkisp_ioctl
	.word	rockit_rkisp_compat_ioctl
	.word	0
	.space	4
	.word	rockit_rkisp_open
	.space	4
	.word	0
	.space	60
	.type	__param_str_rk_cam_num, %object
	.size	__param_str_rk_cam_num, 18
__param_str_rk_cam_num:
	.ascii	"rockit.rk_cam_num\000"
	.type	__param_str_rk_virtual_vblank, %object
	.size	__param_str_rk_virtual_vblank, 25
__param_str_rk_virtual_vblank:
	.ascii	"rockit.rk_virtual_vblank\000"
	.data
	.align	2
	.set	.LANCHOR0,. + 0
	.type	rk_cam_num, %object
	.size	rk_cam_num, 4
rk_cam_num:
	.word	1
	.type	rk_virtual_vblank, %object
	.size	rk_virtual_vblank, 4
rk_virtual_vblank:
	.word	10
	.bss
	.align	2
	.set	.LANCHOR2,. + 0
	.type	rockit_buf_que, %object
	.size	rockit_buf_que, 4
rockit_buf_que:
	.space	4
	.type	__func____LINE__.40381, %object
	.size	__func____LINE__.40381, 4
__func____LINE__.40381:
	.space	4
	.section	.init.rodata,"a"
	.type	__setup_str_rk_cam_num_setup, %object
	.size	__setup_str_rk_cam_num_setup, 12
__setup_str_rk_cam_num_setup:
	.ascii	"rk_cam_num=\000"
	.section	.init.setup,"aw"
	.align	2
	.type	__setup_rk_cam_num_setup, %object
	.size	__setup_rk_cam_num_setup, 12
__setup_rk_cam_num_setup:
	.word	__setup_str_rk_cam_num_setup
	.word	rk_cam_num_setup
	.word	0
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_rk_cam_numtype229, %object
	.size	__UNIQUE_ID_rk_cam_numtype229, 32
__UNIQUE_ID_rk_cam_numtype229:
	.ascii	"rockit.parmtype=rk_cam_num:uint\000"
	.type	__UNIQUE_ID_rk_virtual_vblanktype228, %object
	.size	__UNIQUE_ID_rk_virtual_vblanktype228, 39
__UNIQUE_ID_rk_virtual_vblanktype228:
	.ascii	"rockit.parmtype=rk_virtual_vblank:uint\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"vvi(%s-%d) processing, bufs: %d\000"
.LC1:
	.ascii	"isp qbuf error: repeated qubuf %d\012\000"
.LC2:
	.ascii	"\033[1;31merr:node null\033[0m\000"
.LC3:
	.ascii	"\033[1;31merr: ctx is null\033[0m\000"
.LC4:
	.ascii	"\0013%s[%d]:rkisp_rockit_pause_stream fail\000"
.LC5:
	.ascii	"\0013%s[%d]:rkisp_rockit_config_stream fail\000"
.LC6:
	.ascii	"\0013%s[%d]:rkisp_rockit_resume_stream fail\000"
.LC7:
	.ascii	"ispctx = %08x, ispctx->qbuf_flag = %d\000"
.LC8:
	.ascii	"rkisp queue buf retry cnt = %d, id = %d\000"
.LC9:
	.ascii	"rkisp queue buf fail, id = %d\000"
.LC10:
	.ascii	"rkisp queue buf done\000"
.LC11:
	.ascii	"rockit isp stream off\000"
.LC12:
	.ascii	"\033[1;31merr, rockit_cfg null\033[0m\000"
.LC13:
	.ascii	"\033[1;31merr, node null\033[0m\000"
.LC14:
	.ascii	"%s, line %d: rockit hold cpu wait %d ms\000"
.LC15:
	.ascii	"\033[1;31mthe format is not find!\033[0m\000"
.LC16:
	.ascii	"\0013no memory for rockit_cfg\000"
.LC17:
	.ascii	"rkisp-vir0\000"
	.section	___ksymtab_gpl+rockit_free_tb_stream_buf,"a"
	.align	2
	.type	__ksymtab_rockit_free_tb_stream_buf, %object
	.size	__ksymtab_rockit_free_tb_stream_buf, 12
__ksymtab_rockit_free_tb_stream_buf:
	.word	rockit_free_tb_stream_buf
	.word	__kstrtab_rockit_free_tb_stream_buf
	.word	__kstrtabns_rockit_free_tb_stream_buf
	.section	___ksymtab_gpl+rockit_get_tb_stream_info,"a"
	.align	2
	.type	__ksymtab_rockit_get_tb_stream_info, %object
	.size	__ksymtab_rockit_get_tb_stream_info, 12
__ksymtab_rockit_get_tb_stream_info:
	.word	rockit_get_tb_stream_info
	.word	__kstrtab_rockit_get_tb_stream_info
	.word	__kstrtabns_rockit_get_tb_stream_info
	.section	__param,"a"
	.align	2
	.type	__param_rk_cam_num, %object
	.size	__param_rk_cam_num, 20
__param_rk_cam_num:
	.word	__param_str_rk_cam_num
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	rk_cam_num
	.type	__param_rk_virtual_vblank, %object
	.size	__param_rk_virtual_vblank, 20
__param_rk_virtual_vblank:
	.word	__param_str_rk_virtual_vblank
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	rk_virtual_vblank
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
