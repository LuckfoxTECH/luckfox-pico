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
	ldrd	r4, r9, [r0]
	.pad #60
	sub	sp, sp, #60
	ldr	r7, [r0, #8]
	ldrd	fp, r10, [r0, #12]
	mov	r1, r4
	mov	r0, r7
	bl	mpp_vcodec_get_chan_entry
	mov	r6, r0
	mov	r0, r4
	bl	mpp_vcodec_get_chan_num
	movs	r1, #44
	mov	r2, r0
	ldr	r0, .L16
	bl	printk
	ldr	r3, [r6]
	cmp	r3, r7
	beq	.L3
	movs	r1, #45
	ldr	r3, .L16+4
	str	r1, [sp]
	ldr	r2, .L16+8
	ldr	r0, .L16+12
	bl	printk
.L3:
	ldr	r2, [r6, #40]
	ldr	r3, [r5, #48]
	cmp	r2, #0
	beq	.L4
	cmp	r3, #0
	bne	.L13
	mov	r0, r4
	bl	mpp_vcodec_get_free_chan
	subs	r7, r0, #0
	bge	.L6
.L15:
	mov	r4, #-1
.L2:
	mov	r0, r4
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L6:
	mov	r3, r7
	ldr	r2, [r5, #8]
	movs	r1, #58
	ldr	r0, .L16+16
	bl	printk
	str	r7, [r5, #8]
	mov	r1, r4
	mov	r0, r7
	bl	mpp_vcodec_get_chan_entry
	mov	r6, r0
.L7:
	mov	r8, #0
	cmp	r4, #0
	beq	.L2
	cmp	r4, #1
	bne	.L9
	ldr	r3, [r5, #20]
	add	r2, r6, #164
	strd	r8, r9, [sp, #8]
	strd	fp, r10, [sp, #16]
	str	r3, [sp, #24]
	ldr	r3, [r5, #24]
	str	r2, [sp, #36]
	str	r3, [sp, #28]
	ldr	r3, [r5, #28]
	str	r3, [sp, #32]
	cbnz	r3, .L10
	ldr	r4, [r5, #44]
.L10:
	ldr	r3, [r5, #52]
	add	r1, sp, #12
	add	r0, sp, #8
	strd	r4, r7, [sp, #40]
	str	r3, [sp, #48]
	ldr	r3, [r5, #56]
	str	r3, [sp, #52]
	bl	mpp_enc_init
	cmp	r0, #0
	bne	.L13
	mov	r2, r7
	ldr	r1, .L16+20
	ldr	r0, [sp, #8]
	bl	mpp_enc_register_chl
	mov	r2, r9
	movs	r1, #1
	mov	r0, r6
	ldr	r3, [sp, #8]
	bl	mpp_vcodec_chan_entry_init
	mov	r1, r5
	mov	r0, r6
	bl	mpp_vcodec_chan_setup_hal_bufs
	mov	r4, r0
	cbz	r0, .L12
	ldr	r0, [r6, #40]
	bl	mpp_enc_deinit
	mov	r0, r6
	bl	mpp_vcodec_chan_entry_deinit
	b	.L15
.L4:
	cmp	r3, #0
	beq	.L7
	b	.L15
.L12:
	movs	r0, #1
	bl	mpp_vcodec_inc_chan_num
	ldr	r2, [r6, #40]
	movs	r1, #103
	ldr	r0, .L16+24
	ldr	r3, [sp, #8]
	bl	printk
	bl	mpp_time
	strd	r0, [r6, #120]
	bl	mpp_time
	strd	r0, [r6, #128]
	bl	mpp_time
	strd	r0, [r6, #136]
	b	.L2
.L9:
	mov	r2, r4
	movs	r1, #111
	ldr	r0, .L16+28
	bl	printk
.L13:
	movs	r4, #0
	b	.L2
.L17:
	.align	2
.L16:
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
	ldr	r3, [r0, #40]
	mov	r5, r0
	cbnz	r3, .L19
	movs	r1, #160
	ldr	r3, .L25
	str	r1, [sp]
	ldr	r2, .L25+4
	ldr	r0, .L25+8
	bl	printk
.L19:
	cbz	r4, .L20
	cmp	r4, #1
	bne	.L21
	ldr	r0, [r5, #40]
	bl	mpp_enc_start
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #3
	str	r2, [r5, #24]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	bl	enc_chan_update_chan_prior_tab
.L20:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L21:
	mov	r2, r4
	movs	r1, #176
	ldr	r0, .L25+12
	bl	printk
	b	.L20
.L26:
	.align	2
.L25:
	.word	.LANCHOR0+23
	.word	.LC6
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
	ldr	r3, [r0, #40]
	mov	r5, r0
	cbnz	r3, .L28
	movs	r1, #187
	ldr	r3, .L34
	str	r1, [sp]
	ldr	r2, .L34+4
	ldr	r0, .L34+8
	bl	printk
.L28:
	cbz	r4, .L32
	cmp	r4, #1
	bne	.L30
	ldr	r0, [r5, #40]
	bl	mpp_enc_stop
	mov	r6, r0
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r5, #24]
	cmp	r1, #3
	beq	.L31
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L33:
	movs	r6, #0
	b	.L27
.L31:
	movs	r2, #2
	str	r2, [r5, #24]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r3, #0
	mov	r2, r4
	add	r0, r5, #48
	bl	__wake_up
	bl	enc_chan_update_chan_prior_tab
.L27:
	mov	r0, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L30:
	mov	r2, r4
	movs	r1, #210
	ldr	r0, .L34+12
	bl	printk
	b	.L33
.L32:
	mov	r6, r4
	b	.L27
.L35:
	.align	2
.L34:
	.word	.LANCHOR0+45
	.word	.LC6
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
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #24
	sub	sp, sp, #24
	mov	r6, r0
	mov	r5, r1
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #40]
	mov	r4, r0
	cbz	r3, .L54
	cbz	r5, .L54
	cmp	r5, #1
	bne	.L38
	mov	r1, r5
	mov	r0, r6
	bl	mpp_vcodec_chan_stop
	bl	_cond_resched
	ldr	r3, [r4, #32]
	cbz	r3, .L39
	movs	r1, #0
	add	r0, sp, #4
	bl	init_wait_entry
	add	r6, r4, #56
	mov	r5, #660
.L42:
	movs	r2, #2
	add	r1, sp, #4
	mov	r0, r6
	bl	prepare_to_wait_event
	ldr	r3, [r4, #32]
	cbz	r3, .L40
	cbnz	r5, .L61
.L40:
	add	r1, sp, #4
	mov	r0, r6
	bl	finish_wait
.L39:
	mov	r0, r4
	bl	mpp_vcodec_stream_clear
	ldr	r0, [r4, #40]
	bl	mpp_enc_deinit
	movs	r0, #1
	bl	mpp_vcodec_dec_chan_num
	mov	r0, r4
	bl	mpp_vcodec_chan_entry_deinit
.L54:
	movs	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L61:
	mov	r0, r5
	bl	schedule_timeout
	mov	r5, r0
	b	.L42
.L38:
	mov	r2, r5
	movs	r1, #150
	ldr	r0, .L62
	bl	printk
	b	.L54
.L63:
	.align	2
.L62:
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
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r6, r2
	bl	mpp_vcodec_get_chan_entry
	ldr	r4, [r0, #104]
	mov	r5, r0
	cmp	r4, #0
	beq	.L67
	add	r8, r0, #64
	mov	r0, r8
	bl	mutex_lock
	mov	r3, r5
	ldr	r4, [r3, #88]!
	mov	r0, r8
	cmp	r4, r3
	ite	ne
	subne	r4, r4, #12
	moveq	r4, #0
	bl	mutex_unlock
	add	r3, r5, #104
	pld	[r3]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
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
	add	r9, r5, #76
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
	ldr	r1, .L71
	asrs	r3, r2, #31
	strd	r2, [r6]
	ldr	r3, [r4, #84]
	strd	r4, r7, [r6, #8]
	str	r3, [r6, #48]
	ldr	r0, [r4, #72]
	bl	mpp_buffer_get_size_with_caller
	str	r0, [r6, #20]
	mov	r0, r8
	bl	mutex_lock
	ldrd	r2, r3, [r4, #12]
	add	r6, r4, #12
	mov	r0, r9
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r6, [r4, #12]
	str	r6, [r4, #16]
	bl	mutex_lock
	ldrd	r2, r3, [r4, #12]
	mov	r0, r9
	str	r3, [r2, #4]
	str	r2, [r3]
	add	r2, r5, #96
	ldr	r3, [r5, #100]
	str	r6, [r5, #100]
	strd	r2, r3, [r4, #12]
	str	r6, [r3]
	bl	mutex_unlock
	mov	r0, r8
	bl	mutex_unlock
	add	r3, r5, #108
	pld	[r3]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r2, [r3]
	add	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r7
.L66:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L67:
	movw	r1, #261
	ldr	r0, .L71+4
	bl	printk
	movs	r2, #184
	mov	r1, r4
	mov	r0, r6
	bl	memset
	mov	r0, #-1
	b	.L66
.L72:
	.align	2
.L71:
	.word	.LANCHOR0+66
	.word	.LC7
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
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r8, r2
	bl	mpp_vcodec_get_chan_entry
	add	r6, r0, #76
	mov	r4, r0
	mov	r5, r4
	mov	r0, r6
	bl	mutex_lock
	ldr	r3, [r5, #96]!
	ldrd	r0, [r8, #8]
	ldr	r7, [r3], #-12
	subs	r7, r7, #12
.L74:
	add	r2, r3, #12
	cmp	r2, r5
	bne	.L79
	mov	r2, r0
	mov	r3, r1
	ldr	r0, .L82
	mov	r1, #290
	bl	printk
	ldr	r2, [r4, #96]
	ldr	r7, .L82+4
	ldr	r4, [r2], #-12
	subs	r4, r4, #12
.L80:
	add	r3, r2, #12
	cmp	r5, r3
	bne	.L81
	mov	r1, #296
	ldr	r3, .L82+8
	str	r1, [sp]
	ldr	r2, .L82+12
	ldr	r0, .L82+16
	bl	printk
	b	.L78
.L79:
	mov	r9, #0
	cmp	r9, r1
	it	eq
	cmpeq	r3, r0
	bne	.L75
	ldrd	r0, r1, [r3, #12]
	str	r1, [r0, #4]
	str	r0, [r1]
	add	r0, r3, #20
	str	r2, [r3, #12]
	str	r2, [r3, #16]
	pld	[r0]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_fetch_sub
1:	ldrex	r2, [r0]
	sub	r1, r2, #1
	strex	r5, r1, [r0]
	teq	r5, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r2, #1
	bne	.L76
	bl	stream_packet_free
.L77:
	add	r3, r4, #108
	pld	[r3]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r2, [r3]
	sub	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bl	mpp_vcodec_get_enc_module_entry
	ldr	r0, [r0, #1568]
	bl	vcodec_thread_trigger
.L78:
	mov	r0, r6
	bl	mutex_unlock
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L76:
	cmp	r2, #0
	bgt	.L77
	movs	r1, #3
	bl	refcount_warn_saturate
	b	.L77
.L75:
	ldr	r2, [r7, #12]
	mov	r3, r7
	sub	r7, r2, #12
	b	.L74
.L81:
	movs	r3, #0
	mov	r1, #294
	mov	r0, r7
	bl	printk
	ldr	r3, [r4, #12]
	mov	r2, r4
	sub	r4, r3, #12
	b	.L80
.L83:
	.align	2
.L82:
	.word	.LC8
	.word	.LC9
	.word	.LANCHOR0+93
	.word	.LC10
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
	cbnz	r0, .L85
	ldr	r1, .L102
	mov	r2, #314
	ldr	r0, .L102+4
.L101:
	bl	printk
	mov	r0, #-1
.L84:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L85:
	movs	r1, #1
	mov	r0, r9
	bl	mpp_vcodec_get_chan_entry
	mov	r8, r0
	bl	mpp_vcodec_get_enc_module_entry
	ldr	r3, [r5, #40]
	ldr	r7, [r0, #1568]
	cbz	r3, .L88
	ldr	r0, [r6, #32]
	bl	dma_buf_get
	movs	r1, #1
	mov	r4, r0
	bl	dma_buf_end_cpu_access
	cmn	r4, #4096
	bls	.L89
	ldr	r2, [r6, #32]
	movw	r1, #327
	ldr	r0, .L102+8
	b	.L101
.L89:
	mov	r1, r6
	ldr	r3, [r5, #40]
	mov	r2, r9
	mov	r0, r4
	blx	r3
	mov	r6, r0
	mov	r0, r4
	bl	dma_buf_put
	cbz	r6, .L90
	ldr	r3, [r5, #32]
	cbnz	r3, .L91
.L100:
	movs	r0, #0
	b	.L84
.L91:
	mov	r1, r6
	ldr	r0, [r8, #44]
	blx	r3
	cmp	r0, #0
	beq	.L100
	mov	r0, r7
	bl	vcodec_thread_trigger
	b	.L100
.L90:
	cbz	r4, .L88
	mov	r0, r4
	bl	dma_buf_put
.L88:
	mov	r1, #346
	ldr	r0, .L102+12
	bl	printk
	b	.L100
.L103:
	.align	2
.L102:
	.word	.LANCHOR0+120
	.word	.LC11
	.word	.LC12
	.word	.LC13
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
	mov	r7, r3
	mov	r6, r0
	mov	r4, r1
	mov	r8, r2
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #40]
	mov	r5, r0
	cbnz	r3, .L105
	movw	r1, #403
	ldr	r3, .L132
	str	r1, [sp]
	ldr	r2, .L132+4
	ldr	r0, .L132+8
	bl	printk
.L105:
	cbz	r4, .L125
	cmp	r4, #1
	bne	.L107
	ldr	r3, .L132+12
	cmp	r8, r3
	bne	.L108
	mov	r1, r4
	mov	r0, r6
	bl	mpp_vcodec_get_chan_entry
	movw	r1, #4720
	mov	r4, r0
	ldr	r0, .L132+16
	bl	mpp_osal_malloc
	mov	r5, r0
	cbnz	r0, .L109
	movw	r1, #359
	ldr	r0, .L132+20
	bl	printk
.L125:
	movs	r0, #0
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L109:
	mov	r2, r0
	ldr	r1, .L132+24
	ldr	r0, [r4, #40]
	bl	mpp_enc_control
	ldr	r3, [r4, #40]
	cbnz	r3, .L110
	mov	r1, #364
	ldr	r3, .L132+16
	str	r1, [sp]
	ldr	r2, .L132+28
	ldr	r0, .L132+8
	bl	printk
.L110:
	ldr	r3, [r7, #8]
	cmp	r6, r3
	beq	.L111
	movw	r1, #365
	ldr	r3, .L132+16
	str	r1, [sp]
	ldr	r2, .L132+32
	ldr	r0, .L132+8
	bl	printk
.L111:
	movs	r1, #1
	mov	r0, r6
	bl	mpp_vcodec_chan_stop
	bl	_cond_resched
	ldr	r3, [r4, #32]
	cbz	r3, .L112
	movs	r1, #0
	add	r0, sp, #12
	bl	init_wait_entry
	add	r9, r4, #56
	mov	r8, #660
.L115:
	movs	r2, #2
	add	r1, sp, #12
	mov	r0, r9
	bl	prepare_to_wait_event
	ldr	r3, [r4, #32]
	cbz	r3, .L113
	cmp	r8, #0
	bne	.L131
.L113:
	add	r1, sp, #12
	mov	r0, r9
	bl	finish_wait
.L112:
	mov	r0, r4
	mov	r8, #-1
	bl	mpp_vcodec_stream_clear
	ldr	r0, [r4, #40]
	bl	mpp_enc_deinit
	movs	r0, #1
	bl	mpp_vcodec_dec_chan_num
	movs	r3, #0
	str	r8, [r4, #4]
	mov	r0, r7
	str	r3, [r4, #40]
	str	r3, [r4, #24]
	str	r3, [r4, #36]
	bl	mpp_vcodec_chan_create
	movs	r1, #1
	mov	r0, r6
	bl	mpp_vcodec_get_chan_entry
	add	r2, r5, #8
	ldr	r1, .L132+36
	mov	r4, r0
	ldr	r0, [r0, #40]
	bl	mpp_enc_control
	mov	r2, r5
	str	r8, [r2, #104]!
	ldr	r1, .L132+40
	ldr	r0, [r4, #40]
	bl	mpp_enc_control
	add	r2, r5, #4672
	adds	r2, r2, #4
	ldr	r1, .L132+44
	ldr	r0, [r4, #40]
	bl	mpp_enc_control
	mov	r2, r5
	ldr	r1, .L132+24
	ldr	r0, [r4, #40]
	bl	mpp_enc_control
	str	r8, [r5, #8]
	mov	r2, r5
	ldr	r1, .L132+48
	ldr	r0, [r4, #40]
	bl	mpp_enc_control
	movs	r1, #1
	mov	r0, r6
	bl	mpp_vcodec_chan_start
	mov	r1, r5
	ldr	r0, .L132+16
	bl	mpp_osal_free
	b	.L125
.L131:
	mov	r0, r8
	bl	schedule_timeout
	mov	r8, r0
	b	.L115
.L108:
	mov	r2, r7
	mov	r1, r8
	ldr	r0, [r5, #40]
	bl	mpp_enc_control
	b	.L125
.L107:
	mov	r2, r4
	mov	r1, #418
	ldr	r0, .L132+52
	bl	printk
	b	.L125
.L133:
	.align	2
.L132:
	.word	.LANCHOR0+145
	.word	.LC6
	.word	.LC2
	.word	3276822
	.word	.LANCHOR0+169
	.word	.LC14
	.word	3276802
	.word	.LC15
	.word	.LC16
	.word	3276803
	.word	3276805
	.word	3310081
	.word	3276801
	.word	.LC17
	.fnend
	.size	mpp_vcodec_chan_control, .-mpp_vcodec_chan_control
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.36204, %object
	.size	__FUNCTION__.36204, 23
__FUNCTION__.36204:
	.ascii	"mpp_vcodec_chan_create\000"
	.type	__FUNCTION__.36252, %object
	.size	__FUNCTION__.36252, 22
__FUNCTION__.36252:
	.ascii	"mpp_vcodec_chan_start\000"
	.type	__FUNCTION__.36272, %object
	.size	__FUNCTION__.36272, 21
__FUNCTION__.36272:
	.ascii	"mpp_vcodec_chan_stop\000"
	.type	__FUNCTION__.36318, %object
	.size	__FUNCTION__.36318, 27
__FUNCTION__.36318:
	.ascii	"mpp_vcodec_chan_get_stream\000"
	.type	__FUNCTION__.36366, %object
	.size	__FUNCTION__.36366, 27
__FUNCTION__.36366:
	.ascii	"mpp_vcodec_chan_put_stream\000"
	.type	__func__.36381, %object
	.size	__func__.36381, 25
__func__.36381:
	.ascii	"mpp_vcodec_chan_push_frm\000"
	.type	__FUNCTION__.36423, %object
	.size	__FUNCTION__.36423, 24
__FUNCTION__.36423:
	.ascii	"mpp_vcodec_chan_control\000"
	.type	__FUNCTION__.36390, %object
	.size	__FUNCTION__.36390, 35
__FUNCTION__.36390:
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
	.ascii	"\0016mpp_vcodec: %d: chan_entry->handle %p, enc %p "
	.ascii	"\000"
.LC5:
	.ascii	"\0013mpp_vcodec: %d: create chan error type %d\012\000"
.LC6:
	.ascii	"chan_entry->handle != ((void *)0)\000"
.LC7:
	.ascii	"\0013mpp_vcodec: %d: no stream count found in list "
	.ascii	"\012\000"
.LC8:
	.ascii	"\0013mpp_vcodec: %d: release packet fail %llx \012\000"
.LC9:
	.ascii	"\0013mpp_vcodec: %d: dump packet out list %llx \012"
	.ascii	"\000"
.LC10:
	.ascii	"found\000"
.LC11:
	.ascii	"\0013mpp_vcodec: %s:%d: mpibuf_ops get fail\000"
.LC12:
	.ascii	"\0013mpp_vcodec: %d: dma_buf_get fd %d failed\012\000"
.LC13:
	.ascii	"\0013mpp_vcodec: %d: import dma buf to mpi buf fail"
	.ascii	" \012\000"
.LC14:
	.ascii	"\0013mpp_vcodec: %d: change_coding_type malloc fail"
	.ascii	"\000"
.LC15:
	.ascii	"entry->handle != ((void *)0)\000"
.LC16:
	.ascii	"chan_id == attr->chan_id\000"
.LC17:
	.ascii	"\0013mpp_vcodec: %d: control type %d error\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
