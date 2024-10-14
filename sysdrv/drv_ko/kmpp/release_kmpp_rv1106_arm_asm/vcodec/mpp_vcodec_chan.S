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
	.file	"mpp_vcodec_chan.c"
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
	.type	atomic_add.constprop.6, %function
atomic_add.constprop.6:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	pld	[r0]
	.syntax unified
@ 223 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r3, [r0]
	add	r3, r3, #1
	strex	r2, r3, [r0]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	atomic_add.constprop.6, .-atomic_add.constprop.6
	.align	1
	.global	mpp_vcodec_schedule
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_schedule, %function
mpp_vcodec_schedule:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_vcodec_schedule, .-mpp_vcodec_schedule
	.align	1
	.global	mpp_vcodec_chan_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_create, %function
mpp_vcodec_chan_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r0
	ldrd	r4, r10, [r0]
	.pad #60
	sub	sp, sp, #60
	ldr	r7, [r0, #8]
	ldrd	r8, fp, [r0, #12]
	mov	r1, r4
	mov	r0, r7
	bl	mpp_vcodec_get_chan_entry
	mov	r6, r0
	mov	r0, r4
	bl	mpp_vcodec_get_chan_num
	movs	r1, #45
	mov	r2, r0
	ldr	r0, .L17
	bl	printk
	ldr	r3, [r6]
	cmp	r3, r7
	beq	.L4
	movs	r1, #46
	ldr	r3, .L17+4
	str	r1, [sp]
	ldr	r2, .L17+8
	ldr	r0, .L17+12
	bl	printk
.L4:
	ldr	r2, [r6, #44]
	ldr	r3, [r5, #48]
	cmp	r2, #0
	beq	.L5
	cmp	r3, #0
	bne	.L14
	mov	r0, r4
	bl	mpp_vcodec_get_free_chan
	subs	r7, r0, #0
	bge	.L7
.L16:
	mov	r4, #-1
.L3:
	mov	r0, r4
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L7:
	mov	r3, r7
	ldr	r2, [r5, #8]
	movs	r1, #57
	ldr	r0, .L17+16
	bl	printk
	str	r7, [r5, #8]
	mov	r1, r4
	mov	r0, r7
	bl	mpp_vcodec_get_chan_entry
	mov	r6, r0
.L8:
	mov	r9, #0
	cmp	r4, #0
	beq	.L3
	cmp	r4, #1
	bne	.L10
	ldr	r3, [r5, #20]
	add	r2, r6, #188
	strd	r9, r10, [sp, #12]
	strd	r8, fp, [sp, #20]
	str	r3, [sp, #28]
	ldr	r3, [r5, #24]
	str	r2, [sp, #40]
	str	r3, [sp, #32]
	ldr	r3, [r5, #28]
	str	r3, [sp, #36]
	cbnz	r3, .L11
	ldr	r4, [r5, #44]
.L11:
	ldr	r3, [r5, #56]
	add	r1, sp, #16
	add	r0, sp, #12
	strd	r4, r7, [sp, #44]
	str	r3, [sp, #52]
	bl	mpp_enc_init
	cmp	r0, #0
	bne	.L14
	mov	r2, r7
	ldr	r1, .L17+20
	ldr	r0, [sp, #12]
	bl	mpp_enc_register_chl
	mov	r2, r10
	movs	r1, #1
	mov	r0, r6
	ldr	r3, [sp, #12]
	bl	mpp_vcodec_chan_entry_init
	mov	r1, r5
	mov	r0, r6
	bl	mpp_vcodec_chan_setup_hal_bufs
	mov	r4, r0
	cbz	r0, .L13
	ldr	r0, [r6, #44]
	bl	mpp_enc_deinit
	mov	r0, r6
	bl	mpp_vcodec_chan_entry_deinit
	b	.L16
.L5:
	cmp	r3, #0
	beq	.L8
	b	.L16
.L13:
	movs	r0, #1
	bl	mpp_vcodec_inc_chan_num
	strb	r8, [r6, #21]
	bl	mpp_time
	strd	r0, [r6, #144]
	bl	mpp_time
	strd	r0, [r6, #152]
	bl	mpp_time
	mov	r2, r7
	strd	r0, [r6, #160]
	movs	r1, #103
	str	r8, [sp]
	ldr	r0, .L17+24
	ldr	r3, [r6, #44]
	bl	printk
	b	.L3
.L10:
	mov	r2, r4
	movs	r1, #106
	ldr	r0, .L17+28
	bl	printk
.L14:
	movs	r4, #0
	b	.L3
.L18:
	.align	2
.L17:
	.word	.LC0
	.word	.LANCHOR0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	mpp_vcodec_enc_int_handle
	.word	.LC4
	.word	.LC5
	.fnend
	.size	mpp_vcodec_chan_create, .-mpp_vcodec_chan_create
	.align	1
	.global	mpp_vcodec_chan_unbind
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_unbind, %function
mpp_vcodec_chan_unbind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	ldr	r0, [r0, #4]
	adds	r1, r0, #1
	bne	.L20
	ldr	r0, [r4, #8]
	adds	r2, r0, #1
	beq	.L21
.L20:
	movs	r1, #1
	bl	mpp_vcodec_get_chan_entry
	mov	r5, r0
	cbnz	r0, .L22
	movs	r1, #125
	ldr	r3, .L27
	str	r1, [sp]
	ldr	r2, .L27+4
	ldr	r0, .L27+8
	bl	printk
.L22:
	ldr	r2, [r4, #4]
	adds	r3, r2, #1
	mov	r3, #0
	beq	.L23
	str	r3, [r4, #24]
	str	r3, [r5, #36]
	ldr	r0, [r5, #44]
	bl	mpp_enc_deinit_frame
	mov	r3, #-1
	str	r3, [r4, #4]
	str	r3, [r5, #8]
.L21:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L23:
	str	r3, [r5, #24]
	str	r3, [r4, #36]
	str	r2, [r4, #8]
	str	r2, [r5, #4]
	b	.L21
.L28:
	.align	2
.L27:
	.word	.LANCHOR0+23
	.word	.LC6
	.word	.LC2
	.fnend
	.size	mpp_vcodec_chan_unbind, .-mpp_vcodec_chan_unbind
	.align	1
	.global	mpp_vcodec_chan_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_start, %function
mpp_vcodec_chan_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #44]
	mov	r5, r0
	cbnz	r3, .L30
	movs	r1, #202
	ldr	r3, .L36
	str	r1, [sp]
	ldr	r2, .L36+4
	ldr	r0, .L36+8
	bl	printk
.L30:
	ldr	r0, [r5, #44]
	cbz	r0, .L33
	cbz	r4, .L34
	cmp	r4, #1
	bne	.L32
	bl	mpp_enc_start
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #3
	str	r2, [r5, #28]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	bl	enc_chan_update_chan_prior_tab
.L35:
	movs	r0, #0
.L29:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L32:
	mov	r2, r4
	movs	r1, #221
	ldr	r0, .L36+12
	bl	printk
	b	.L35
.L33:
	mov	r0, #-1
	b	.L29
.L34:
	mov	r0, r4
	b	.L29
.L37:
	.align	2
.L36:
	.word	.LANCHOR0+46
	.word	.LC7
	.word	.LC2
	.word	.LC5
	.fnend
	.size	mpp_vcodec_chan_start, .-mpp_vcodec_chan_start
	.align	1
	.global	mpp_vcodec_chan_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_stop, %function
mpp_vcodec_chan_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #44]
	mov	r5, r0
	cbnz	r3, .L39
	movs	r1, #233
	ldr	r3, .L45
	str	r1, [sp]
	ldr	r2, .L45+4
	ldr	r0, .L45+8
	bl	printk
.L39:
	cbz	r4, .L43
	cmp	r4, #1
	bne	.L41
	ldr	r0, [r5, #44]
	bl	mpp_enc_stop
	mov	r6, r0
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r5, #28]
	cmp	r1, #3
	beq	.L42
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L44:
	movs	r6, #0
	b	.L38
.L42:
	movs	r2, #2
	str	r2, [r5, #28]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r3, #0
	mov	r2, r4
	add	r0, r5, #52
	bl	__wake_up
	bl	enc_chan_update_chan_prior_tab
.L38:
	mov	r0, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L41:
	mov	r2, r4
	movs	r1, #255
	ldr	r0, .L45+12
	bl	printk
	b	.L44
.L43:
	mov	r6, r4
	b	.L38
.L46:
	.align	2
.L45:
	.word	.LANCHOR0+68
	.word	.LC7
	.word	.LC2
	.word	.LC5
	.fnend
	.size	mpp_vcodec_chan_stop, .-mpp_vcodec_chan_stop
	.align	1
	.global	mpp_vcodec_chan_destory
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_destory, %function
mpp_vcodec_chan_destory:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #44
	sub	sp, sp, #44
	mov	r6, r0
	mov	r5, r1
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #44]
	mov	r4, r0
	cmp	r3, #0
	beq	.L74
	cmp	r5, #0
	beq	.L74
	cmp	r5, #1
	bne	.L49
	ldr	r2, [r0, #8]
	movs	r1, #162
	str	r2, [sp, #8]
	ldr	r2, [r0, #4]
	str	r2, [sp, #4]
	ldrb	r2, [r0, #21]	@ zero_extendqisi2
	ldr	r0, .L85
	str	r2, [sp]
	mov	r2, r6
	bl	printk
	mov	r1, r5
	mov	r0, r6
	bl	mpp_vcodec_chan_stop
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cbnz	r3, .L50
.L54:
	ldr	r3, [r4, #36]
	cbz	r3, .L55
	movs	r1, #0
	add	r0, sp, #20
	bl	init_wait_entry
	add	r7, r4, #60
	movs	r5, #220
.L58:
	movs	r2, #2
	add	r1, sp, #20
	mov	r0, r7
	bl	prepare_to_wait_event
	ldr	r3, [r4, #36]
	cbz	r3, .L56
	cmp	r5, #0
	bne	.L83
.L56:
	add	r1, sp, #20
	mov	r0, r7
	bl	finish_wait
	b	.L55
.L50:
	ldr	r0, [r4, #8]
	adds	r3, r0, #1
	beq	.L61
	mov	r1, r5
	bl	mpp_vcodec_get_chan_entry
	mov	r5, r0
	ldr	r0, [r0, #44]
	bl	mpp_enc_unbind_jpeg_task
.L53:
	ldr	r0, [r5, #44]
	bl	mpp_enc_check_hw_running
	cmp	r0, #0
	bne	.L54
	ldr	r0, [r5, #44]
	bl	mpp_enc_check_is_int_process
	cmp	r0, #0
	bne	.L54
.L55:
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cbz	r3, .L59
	mov	r0, r4
	bl	mpp_vcodec_chan_unbind
.L59:
	ldr	r0, [r4, #44]
	bl	mpp_enc_deinit
	mov	r0, r4
	bl	mpp_vcodec_stream_clear
	movs	r0, #1
	bl	mpp_vcodec_dec_chan_num
	mov	r0, r4
	bl	mpp_vcodec_chan_entry_deinit
	ldr	r0, .L85+4
	mov	r2, r6
	movs	r1, #188
.L84:
	bl	printk
.L74:
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L61:
	mov	r5, r4
	b	.L53
.L83:
	mov	r0, r5
	bl	schedule_timeout
	mov	r5, r0
	b	.L58
.L49:
	mov	r2, r5
	movs	r1, #191
	ldr	r0, .L85+8
	b	.L84
.L86:
	.align	2
.L85:
	.word	.LC8
	.word	.LC9
	.word	.LC5
	.fnend
	.size	mpp_vcodec_chan_destory, .-mpp_vcodec_chan_destory
	.align	1
	.global	mpp_vcodec_chan_pause
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_pause, %function
mpp_vcodec_chan_pause:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_vcodec_chan_pause, .-mpp_vcodec_chan_pause
	.align	1
	.global	mpp_vcodec_chan_resume
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_resume, %function
mpp_vcodec_chan_resume:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_vcodec_chan_resume, .-mpp_vcodec_chan_resume
	.align	1
	.global	mpp_vcodec_chan_get_stream
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_get_stream, %function
mpp_vcodec_chan_get_stream:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r2
	bl	mpp_vcodec_get_chan_entry
	ldr	r4, [r0, #84]
	mov	r5, r0
	cbnz	r4, .L90
	movw	r1, #281
	ldr	r0, .L94
	bl	printk
	movs	r2, #184
	mov	r1, r4
	mov	r0, r6
	bl	memset
	mov	r0, #-1
.L89:
	pop	{r3, r4, r5, r6, r7, pc}
.L90:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r0, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, r5
	ldr	r4, [r3, #68]!
	cmp	r4, r3
	ite	ne
	subne	r4, r4, #12
	moveq	r4, #0
	add	r2, r4, #12
	ldrd	r1, r3, [r4, #12]
	str	r3, [r1, #4]
	str	r1, [r3]
	add	r1, r5, #76
	ldr	r3, [r5, #80]
	str	r2, [r5, #80]
	strd	r1, r3, [r4, #12]
	str	r2, [r3]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r0	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	add	r3, r5, #84
	pld	[r3]
	.syntax unified
@ 224 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r2, [r3]
	sub	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r4
	movs	r7, #0
	bl	mpp_packet_get_flag
	str	r0, [r6, #40]
	mov	r0, r4
	bl	mpp_packet_get_length
	str	r0, [r6, #16]
	mov	r0, r4
	bl	mpp_packet_get_temporal_id
	str	r0, [r6, #44]
	mov	r0, r4
	bl	mpp_packet_get_pts
	strd	r0, [r6, #24]
	mov	r0, r4
	bl	mpp_packet_get_dts
	movs	r3, #1
	str	r3, [r6, #52]
	strd	r0, [r6, #32]
	ldr	r2, [r4, #76]
	ldr	r1, .L94+4
	asrs	r3, r2, #31
	strd	r2, [r6]
	ldr	r3, [r4, #84]
	strd	r4, r7, [r6, #8]
	str	r3, [r6, #48]
	ldr	r0, [r4, #72]
	bl	mpp_buffer_get_size_with_caller
	str	r0, [r6, #20]
	mov	r0, r4
	bl	mpp_packet_get_dts
	strd	r0, [r5, #104]
	add	r0, r5, #88
	bl	atomic_add.constprop.6
	add	r0, r5, #96
	bl	atomic_add.constprop.6
	mov	r0, r7
	b	.L89
.L95:
	.align	2
.L94:
	.word	.LC10
	.word	.LANCHOR0+89
	.fnend
	.size	mpp_vcodec_chan_get_stream, .-mpp_vcodec_chan_get_stream
	.align	1
	.global	mpp_vcodec_chan_put_stream
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_put_stream, %function
mpp_vcodec_chan_put_stream:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r8, r2
	bl	mpp_vcodec_get_chan_entry
	mov	r5, r0
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r6, r0
	ldr	r4, [r6, #76]!
	ldr	r3, [r4], #-12
	sub	ip, r3, #12
	ldrd	r2, [r8, #8]
.L97:
	add	lr, r4, #12
	cmp	lr, r6
	bne	.L102
	movw	r1, #337
	ldr	r0, .L105
	bl	printk
	ldr	r2, [r5, #76]
	ldr	r5, .L105+4
	ldr	r4, [r2], #-12
	subs	r4, r4, #12
.L103:
	add	r3, r2, #12
	cmp	r6, r3
	bne	.L104
	movw	r1, #343
	ldr	r3, .L105+8
	str	r1, [sp]
	ldr	r2, .L105+12
	ldr	r0, .L105+16
	bl	printk
	b	.L101
.L102:
	movs	r1, #0
	cmp	r1, r3
	it	eq
	cmpeq	r4, r2
	bne	.L98
	ldrd	r2, r3, [r4, #12]
	add	r0, r4, #20
	str	r3, [r2, #4]
	str	r2, [r3]
	str	lr, [r4, #12]
	str	lr, [r4, #16]
	pld	[r0]
	.syntax unified
@ 224 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_fetch_sub
1:	ldrex	r3, [r0]
	sub	r2, r3, #1
	strex	r1, r2, [r0]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r3, #1
	bne	.L99
	bl	stream_packet_free
.L100:
	add	r3, r5, #88
	pld	[r3]
	.syntax unified
@ 224 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r2, [r3]
	sub	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	add	r0, r5, #100
	bl	atomic_add.constprop.6
	mov	r0, r4
	bl	mpp_packet_get_dts
	strd	r0, [r5, #112]
	bl	mpp_vcodec_get_enc_module_entry
	movw	r3, #13856
	ldr	r0, [r0, r3]
	bl	vcodec_thread_trigger
.L101:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L99:
	cmp	r3, #0
	bgt	.L100
	movs	r1, #3
	bl	refcount_warn_saturate
	b	.L100
.L98:
	ldr	r1, [ip, #12]
	mov	r4, ip
	sub	ip, r1, #12
	b	.L97
.L104:
	movs	r3, #0
	movw	r1, #341
	mov	r0, r5
	bl	printk
	ldr	r3, [r4, #12]
	mov	r2, r4
	sub	r4, r3, #12
	b	.L103
.L106:
	.align	2
.L105:
	.word	.LC11
	.word	.LC12
	.word	.LANCHOR0+116
	.word	.LC13
	.word	.LC2
	.fnend
	.size	mpp_vcodec_chan_put_stream, .-mpp_vcodec_chan_put_stream
	.align	1
	.global	mpp_vcodec_chan_push_frm
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_push_frm, %function
mpp_vcodec_chan_push_frm:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r9, r0
	mov	r6, r1
	bl	get_mpibuf_ops
	mov	r5, r0
	cbnz	r0, .L108
	ldr	r1, .L125
	mov	r2, #362
	ldr	r0, .L125+4
.L124:
	bl	printk
	mov	r0, #-1
.L107:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L108:
	movs	r1, #1
	mov	r0, r9
	bl	mpp_vcodec_get_chan_entry
	mov	r8, r0
	bl	mpp_vcodec_get_enc_module_entry
	movw	r3, #13856
	ldr	r7, [r0, r3]
	ldr	r3, [r5, #48]
	cbz	r3, .L111
	ldr	r0, [r6, #32]
	bl	dma_buf_get
	movs	r1, #1
	mov	r4, r0
	bl	dma_buf_end_cpu_access
	cmn	r4, #4096
	bls	.L112
	ldr	r2, [r6, #32]
	movw	r1, #375
	ldr	r0, .L125+8
	b	.L124
.L112:
	mov	r1, r6
	ldr	r3, [r5, #48]
	mov	r2, r9
	mov	r0, r4
	blx	r3
	mov	r6, r0
	mov	r0, r4
	bl	dma_buf_put
	cbz	r6, .L113
	ldr	r3, [r5, #36]
	cbnz	r3, .L114
.L123:
	movs	r0, #0
	b	.L107
.L114:
	mov	r1, r6
	ldr	r0, [r8, #48]
	blx	r3
	cmp	r0, #0
	beq	.L123
	mov	r0, r7
	bl	vcodec_thread_trigger
	b	.L123
.L113:
	cbz	r4, .L111
	mov	r0, r4
	bl	dma_buf_put
.L111:
	mov	r1, #390
	ldr	r0, .L125+12
	bl	printk
	b	.L123
.L126:
	.align	2
.L125:
	.word	.LANCHOR0+143
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.fnend
	.size	mpp_vcodec_chan_push_frm, .-mpp_vcodec_chan_push_frm
	.align	1
	.global	mpp_vcodec_chan_control
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_chan_control, %function
mpp_vcodec_chan_control:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #36
	sub	sp, sp, #36
	mov	r6, r3
	mov	r7, r0
	mov	r5, r1
	mov	r8, r2
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #44]
	mov	r4, r0
	cbnz	r3, .L128
	mov	r1, #464
	ldr	r3, .L191
	str	r1, [sp]
	ldr	r2, .L191+4
	ldr	r0, .L191+8
	bl	printk
.L128:
	cbz	r5, .L175
	cmp	r5, #1
	bne	.L130
	ldr	r3, .L191+12
	cmp	r8, r3
	bne	.L131
	mov	r1, r5
	mov	r0, r7
	bl	mpp_vcodec_get_chan_entry
	mov	r1, #4768
	mov	r4, r0
	ldr	r0, .L191+16
	bl	mpp_osal_malloc
	mov	r5, r0
	cbnz	r0, .L132
	movw	r1, #405
	ldr	r0, .L191+20
	bl	printk
.L175:
	movs	r0, #0
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L132:
	mov	r2, r0
	ldr	r1, .L191+24
	ldr	r0, [r4, #44]
	bl	mpp_enc_control
	ldr	r3, [r4, #44]
	cbnz	r3, .L133
	mov	r1, #410
	ldr	r3, .L191+16
	str	r1, [sp]
	ldr	r2, .L191+28
	ldr	r0, .L191+8
	bl	printk
.L133:
	ldr	r3, [r6, #8]
	cmp	r7, r3
	beq	.L134
	movw	r1, #411
	ldr	r3, .L191+16
	str	r1, [sp]
	ldr	r2, .L191+32
	ldr	r0, .L191+8
	bl	printk
.L134:
	movs	r1, #1
	mov	r0, r7
	bl	mpp_vcodec_chan_stop
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cbnz	r3, .L135
.L139:
	ldr	r3, [r4, #36]
	cbz	r3, .L140
	movs	r1, #0
	add	r0, sp, #12
	bl	init_wait_entry
	add	r9, r4, #60
	mov	r8, #220
.L143:
	movs	r2, #2
	add	r1, sp, #12
	mov	r0, r9
	bl	prepare_to_wait_event
	ldr	r3, [r4, #36]
	cbz	r3, .L141
	cmp	r8, #0
	bne	.L189
.L141:
	add	r1, sp, #12
	mov	r0, r9
	bl	finish_wait
	b	.L140
.L135:
	ldr	r0, [r4, #8]
	adds	r3, r0, #1
	beq	.L153
	movs	r1, #1
	bl	mpp_vcodec_get_chan_entry
	mov	r8, r0
	ldr	r0, [r0, #44]
	bl	mpp_enc_unbind_jpeg_task
.L138:
	ldr	r0, [r8, #44]
	bl	mpp_enc_check_hw_running
	cmp	r0, #0
	bne	.L139
	ldr	r0, [r8, #44]
	bl	mpp_enc_check_is_int_process
	cmp	r0, #0
	bne	.L139
.L140:
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cbz	r3, .L144
	mov	r0, r4
	bl	mpp_vcodec_chan_unbind
.L144:
	ldr	r0, [r4, #44]
	mov	r8, #-1
	bl	mpp_enc_deinit
	mov	r0, r4
	bl	mpp_vcodec_stream_clear
	movs	r0, #1
	bl	mpp_vcodec_dec_chan_num
	movs	r3, #0
	strd	r8, r8, [r4, #4]
	mov	r0, r6
	str	r3, [r4, #44]
	str	r3, [r4, #28]
	str	r3, [r4, #40]
	bl	mpp_vcodec_chan_create
	movs	r1, #1
	mov	r0, r7
	bl	mpp_vcodec_get_chan_entry
	add	r2, r5, #8
	ldr	r1, .L191+36
	mov	r4, r0
	ldr	r0, [r0, #44]
	bl	mpp_enc_control
	mov	r2, r5
	str	r8, [r2, #104]!
	ldr	r1, .L191+40
	ldr	r0, [r4, #44]
	bl	mpp_enc_control
	add	r2, r5, #4704
	ldr	r1, .L191+44
	ldr	r0, [r4, #44]
	bl	mpp_enc_control
	mov	r2, r5
	ldr	r1, .L191+24
	ldr	r0, [r4, #44]
	bl	mpp_enc_control
	str	r8, [r5, #8]
	mov	r2, r5
	ldr	r1, .L191+48
	ldr	r0, [r4, #44]
	bl	mpp_enc_control
	movs	r1, #1
	mov	r0, r7
	bl	mpp_vcodec_chan_start
	mov	r1, r5
	ldr	r0, .L191+16
	bl	mpp_osal_free
	b	.L175
.L153:
	mov	r8, r4
	b	.L138
.L189:
	mov	r0, r8
	bl	schedule_timeout
	mov	r8, r0
	b	.L143
.L131:
	cbz	r6, .L154
	ldr	r5, [r6, #8]
	adds	r5, r5, #0
	it	ne
	movne	r5, #1
.L145:
	mov	r2, r6
	mov	r1, r8
	ldr	r0, [r4, #44]
	bl	mpp_enc_control
	ldrb	r3, [r4, #21]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L175
	cmp	r5, #0
	beq	.L175
	ldr	r0, [r4, #44]
	bl	mpp_enc_check_hw_running
	cmp	r0, #0
	beq	.L175
	ldr	r3, [r4, #36]
	cmp	r3, #0
	beq	.L175
	movs	r1, #0
	add	r0, sp, #12
	bl	init_wait_entry
	add	r6, r4, #60
	movs	r5, #20
.L150:
	movs	r2, #2
	add	r1, sp, #12
	mov	r0, r6
	bl	prepare_to_wait_event
	ldr	r3, [r4, #36]
	cbz	r3, .L148
	cbnz	r5, .L190
.L148:
	add	r1, sp, #12
	mov	r0, r6
	bl	finish_wait
	b	.L175
.L154:
	mov	r5, r6
	b	.L145
.L190:
	mov	r0, r5
	bl	schedule_timeout
	mov	r5, r0
	b	.L150
.L130:
	mov	r2, r5
	mov	r1, #492
	ldr	r0, .L191+52
	bl	printk
	b	.L175
.L192:
	.align	2
.L191:
	.word	.LANCHOR0+168
	.word	.LC7
	.word	.LC2
	.word	3276822
	.word	.LANCHOR0+192
	.word	.LC17
	.word	3276802
	.word	.LC18
	.word	.LC19
	.word	3276803
	.word	3276805
	.word	3310081
	.word	3276801
	.word	.LC20
	.fnend
	.size	mpp_vcodec_chan_control, .-mpp_vcodec_chan_control
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.36387, %object
	.size	__FUNCTION__.36387, 23
__FUNCTION__.36387:
	.ascii	"mpp_vcodec_chan_create\000"
	.type	__FUNCTION__.36401, %object
	.size	__FUNCTION__.36401, 23
__FUNCTION__.36401:
	.ascii	"mpp_vcodec_chan_unbind\000"
	.type	__FUNCTION__.36457, %object
	.size	__FUNCTION__.36457, 22
__FUNCTION__.36457:
	.ascii	"mpp_vcodec_chan_start\000"
	.type	__FUNCTION__.36477, %object
	.size	__FUNCTION__.36477, 21
__FUNCTION__.36477:
	.ascii	"mpp_vcodec_chan_stop\000"
	.type	__FUNCTION__.36530, %object
	.size	__FUNCTION__.36530, 27
__FUNCTION__.36530:
	.ascii	"mpp_vcodec_chan_get_stream\000"
	.type	__FUNCTION__.36585, %object
	.size	__FUNCTION__.36585, 27
__FUNCTION__.36585:
	.ascii	"mpp_vcodec_chan_put_stream\000"
	.type	__func__.36600, %object
	.size	__func__.36600, 25
__func__.36600:
	.ascii	"mpp_vcodec_chan_push_frm\000"
	.type	__FUNCTION__.36644, %object
	.size	__FUNCTION__.36644, 24
__FUNCTION__.36644:
	.ascii	"mpp_vcodec_chan_control\000"
	.type	__FUNCTION__.36609, %object
	.size	__FUNCTION__.36609, 35
__FUNCTION__.36609:
	.ascii	"mpp_vcodec_chan_change_coding_type\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016mpp_vcodec: %d: num_chan = %d\000"
.LC1:
	.ascii	"chan_entry->chan_id == chan_id\000"
.LC2:
	.ascii	"\0013mpp_vcodec: %d: Assertion %s failed at %s:%d\012"
	.ascii	"\000"
.LC3:
	.ascii	"\0016mpp_vcodec: %d: current chan %d already create"
	.ascii	"d get new chan_id %d \012\000"
.LC4:
	.ascii	"\0016mpp_vcodec: %d: create channel %d handle %p on"
	.ascii	"line %d\012\000"
.LC5:
	.ascii	"\0013mpp_vcodec: %d: create chan error type %d\012\000"
.LC6:
	.ascii	"bind_chan\000"
.LC7:
	.ascii	"chan_entry->handle != ((void *)0)\000"
.LC8:
	.ascii	"\0016mpp_vcodec: %d: destroy chan %d hnd %p online "
	.ascii	"%d combo %d mst %d\012\000"
.LC9:
	.ascii	"\0016mpp_vcodec: %d: destroy chan %d done\012\000"
.LC10:
	.ascii	"\0013mpp_vcodec: %d: no stream count found in list "
	.ascii	"\012\000"
.LC11:
	.ascii	"\0013mpp_vcodec: %d: release packet fail %llx \012\000"
.LC12:
	.ascii	"\0013mpp_vcodec: %d: dump packet out list %llx \012"
	.ascii	"\000"
.LC13:
	.ascii	"found\000"
.LC14:
	.ascii	"\0013mpp_vcodec: %s:%d: mpibuf_ops get fail\000"
.LC15:
	.ascii	"\0013mpp_vcodec: %d: dma_buf_get fd %d failed\012\000"
.LC16:
	.ascii	"\0013mpp_vcodec: %d: import dma buf to mpi buf fail"
	.ascii	" \012\000"
.LC17:
	.ascii	"\0013mpp_vcodec: %d: change_coding_type malloc fail"
	.ascii	"\000"
.LC18:
	.ascii	"entry->handle != ((void *)0)\000"
.LC19:
	.ascii	"chan_id == attr->chan_id\000"
.LC20:
	.ascii	"\0013mpp_vcodec: %d: control type %d error\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
