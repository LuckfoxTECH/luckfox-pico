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
	.file	"mpp_vcodec_flow.c"
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
	.type	frame_add_osd, %function
frame_add_osd:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r1
	bl	mpp_frame_add_osd
	ldr	r7, .L12
	mov	r4, r6
	movs	r5, #0
.L2:
	ldr	r3, [r6, #4]
	cmp	r3, r5
	bhi	.L5
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L5:
	ldr	r0, [r4, #136]
	cbz	r0, .L3
	mov	r2, r7
	movs	r1, #0
	bl	mpi_buf_unref_with_tag
.L3:
	ldr	r0, [r4, #88]
	cbz	r0, .L4
	mov	r2, r7
	movs	r1, #0
	bl	mpi_buf_unref_with_tag
.L4:
	adds	r5, r5, #1
	adds	r4, r4, #140
	b	.L2
.L13:
	.align	2
.L12:
	.word	.LANCHOR0
	.fnend
	.size	frame_add_osd, .-frame_add_osd
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_sub.constprop.0, %function
atomic_sub.constprop.0:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	pld	[r0]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r3, [r0]
	sub	r3, r3, #1
	strex	r2, r3, [r0]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	atomic_sub.constprop.0, .-atomic_sub.constprop.0
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_add.constprop.1, %function
atomic_add.constprop.1:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	pld	[r0]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
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
	.size	atomic_add.constprop.1, .-atomic_add.constprop.1
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_chan_process_single_chan, %function
enc_chan_process_single_chan:
	.fnstart
	@ args = 0, pretend = 0, frame = 152
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	movs	r1, #1
	.pad #164
	sub	sp, sp, #164
	mov	r6, r0
	bl	mpp_vcodec_get_chan_entry
	movs	r7, #0
	mov	r4, r0
	strd	r7, r7, [sp, #12]
	str	r7, [sp, #20]
	bl	get_mpibuf_ops
	mov	r9, r0
	cbnz	r4, .L17
	ldr	r1, .L129
	movs	r2, #101
	ldr	r0, .L129+4
.L128:
	bl	printk
	mov	r7, #-1
.L16:
	mov	r0, r7
	add	sp, sp, #164
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L17:
	cbnz	r0, .L19
	movs	r2, #105
	ldr	r1, .L129
	ldr	r0, .L129+8
	b	.L128
.L19:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #24]
	cmp	r3, #3
	beq	.L20
	mov	r2, r6
	movs	r1, #111
	ldr	r0, .L129+12
	bl	printk
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L16
.L20:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #32]
	ldr	r8, .L129+44
	cmp	r3, #0
	ble	.L21
	ldr	r5, [r8]
	ands	r5, r5, #32
	beq	.L56
	mov	r3, r6
	movs	r2, #118
	ldr	r1, .L129
	ldr	r0, .L129+16
	bl	printk
	b	.L16
.L21:
	ldr	r3, [r8]
	lsls	r1, r3, #26
	bpl	.L22
	mov	r3, r6
	movs	r2, #122
	ldr	r1, .L129
	ldr	r0, .L129+20
	bl	printk
.L22:
	ldr	r10, [r4, #36]
	cmp	r10, #0
	beq	.L23
.L26:
	movs	r5, #0
.L24:
	ldr	r3, [sp, #12]
	cbnz	r3, .L39
	ldr	r3, [r4, #36]
	cmp	r3, #0
	beq	.L40
.L39:
	bl	mpp_time
	add	r9, r4, #32
	mov	r8, r0
	mov	r0, r9
	bl	atomic_add.constprop.1
	ldr	r3, [r4, #104]
	ldr	r2, [r4, #108]
	add	r3, r3, r2
	ldr	r2, .L129+24
	ldr	r2, [r2]
	cmp	r3, r2
	ble	.L41
	ldr	r3, [r4, #36]
	cmp	r3, #0
	bne	.L41
	add	r0, sp, #12
	bl	mpp_frame_deinit
.L42:
	bl	get_mpidev_ops
	mov	r7, r0
	bl	mpp_vcodec_get_enc_module_entry
	mov	r10, r0
	mov	r0, r9
	bl	atomic_sub.constprop.0
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	add	r0, r4, #56
	bl	__wake_up
	cbz	r7, .L52
	ldr	r3, [r7, #28]
	cbz	r3, .L52
	ldr	r0, [r4]
	blx	r3
.L52:
	ldr	r3, [sp, #16]
	cbz	r3, .L53
	cbz	r7, .L54
	ldr	r3, [r7, #28]
	cbz	r3, .L54
	cbz	r5, .L54
	ldr	r0, [r5]
	blx	r3
.L54:
	add	r0, sp, #16
	bl	mpp_frame_deinit
.L53:
	ldr	r0, [r10, #1568]
	bl	vcodec_thread_trigger
	b	.L47
.L23:
	ldr	r0, [r4, #40]
	bl	mpp_enc_check_pkt_pool
	mov	r7, r0
	cbnz	r0, .L25
	mov	r2, r6
	movs	r1, #125
	ldr	r0, .L129+28
	bl	printk
	b	.L16
.L25:
	ldr	r3, [r9, #36]
	cmp	r3, #0
	beq	.L26
	ldr	r0, [r4, #44]
	blx	r3
	mov	r7, r0
	cmp	r0, #0
	beq	.L26
	bl	mpp_time
	ldr	r3, [r4, #120]
	subs	r0, r0, r3
	str	r0, [r4, #144]
	bl	mpp_time
	ldr	r5, [r4]
	strd	r0, [r4, #120]
	bl	get_mpibuf_ops
	movs	r2, #104
	mov	r1, r10
	mov	fp, r0
	add	r0, sp, #56
	bl	memset
	ldr	r3, [fp, #44]
	cbz	r3, .L27
	mov	r2, r5
	add	r1, sp, #56
	mov	r0, r7
	blx	r3
	cbz	r0, .L126
.L28:
	ldr	r2, .L129+32
	movs	r1, #0
	mov	r0, r7
	bl	mpi_buf_unref_with_tag
.L127:
	movs	r7, #0
	b	.L16
.L27:
	movs	r1, #60
	ldr	r0, .L129+36
	bl	printk
	b	.L28
.L126:
	add	r0, sp, #12
	bl	mpp_frame_init
	ldr	r1, [sp, #56]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_width
	ldr	r1, [sp, #60]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_height
	ldr	r1, [sp, #64]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_hor_stride
	ldr	r1, [sp, #68]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_ver_stride
	ldrd	r2, [sp, #96]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_pts
	ldrd	r2, [sp, #136]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_dts
	ldr	r1, [sp, #84]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_fmt
	ldr	r1, [sp, #76]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_offset_x
	ldr	r1, [sp, #80]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_offset_y
	ldr	r1, [sp, #120]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_is_gray
	ldr	r1, [sp, #124]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_is_full
	ldr	r1, [sp, #128]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_phy_addr
	ldr	r1, [sp, #108]
	cbz	r1, .L30
	ldr	r0, [sp, #12]
	bl	frame_add_osd
.L30:
	ldr	r1, [sp, #144]
	cbz	r1, .L31
	ldr	r0, [sp, #12]
	bl	mpp_frame_add_ppinfo
.L31:
	movs	r2, #32
	movs	r1, #0
	add	r0, sp, #24
	ldr	r5, .L129+32
	bl	memset
	movs	r3, #0
	mov	r0, r3
	add	r2, sp, #20
	str	r5, [sp]
	add	r1, sp, #24
	str	r7, [sp, #36]
	bl	mpp_buffer_import_with_tag
	ldr	r3, [r8]
	lsls	r2, r3, #25
	bpl	.L32
	ldr	r3, [sp, #104]
	movs	r2, #145
	sub	r1, r5, #29
	ldr	r0, .L129+40
	bl	printk
.L32:
	ldr	r3, [sp, #104]
	cmp	r3, #0
	bgt	.L33
.L36:
	movs	r5, #0
.L34:
	ldr	r1, [sp, #20]
	ldr	r0, [sp, #12]
	bl	mpp_frame_set_buffer
	ldr	r3, [r9, #16]
	cmp	r3, #0
	beq	.L24
	mov	r0, r7
	blx	r3
	b	.L24
.L130:
	.align	2
.L129:
	.word	.LANCHOR0+14
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LANCHOR1
	.word	.LC5
	.word	.LANCHOR0+43
	.word	.LC6
	.word	.LC7
	.word	mpp_vcodec_debug
.L33:
	bl	mpp_time
	ldr	r3, [r4, #128]
	subs	r0, r0, r3
	str	r0, [r4, #148]
	bl	mpp_time
	ldr	r3, [r8]
	lsls	r3, r3, #25
	strd	r0, [r4, #128]
	bpl	.L35
	ldr	r3, [sp, #104]
	movs	r2, #150
	ldr	r1, .L131
	ldr	r0, .L131+4
	bl	printk
.L35:
	ldr	r0, [sp, #104]
	movs	r1, #1
	str	r0, [r4, #4]
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #24]
	mov	r5, r0
	cmp	r3, #3
	bne	.L36
	cmp	r0, #0
	beq	.L36
	ldr	r3, [r0, #40]
	cmp	r3, #0
	beq	.L34
	adds	r0, r0, #32
	bl	atomic_add.constprop.1
	add	r0, sp, #16
	bl	mpp_frame_init
	ldrd	r1, r0, [sp, #12]
	bl	mpp_frame_copy
	ldr	r1, [sp, #116]
	cbz	r1, .L37
	ldr	r0, [sp, #16]
	bl	frame_add_osd
.L37:
	ldrd	r0, r1, [sp, #16]
	bl	mpp_frame_set_buffer
	b	.L34
.L41:
	ldr	r1, [sp, #12]
	ldr	r0, [r4, #40]
	bl	mpp_enc_cfg_reg
	mov	fp, r0
	cmp	r0, #0
	bne	.L42
	cmp	r5, #0
	beq	.L44
	ldr	r3, [r5, #40]
	cmp	r3, #0
	beq	.L44
	add	r7, r4, #20
	mov	r0, r7
	bl	atomic_add.constprop.1
	ldr	r0, [r5, #40]
	ldr	r1, [sp, #16]
	bl	mpp_enc_cfg_reg
	mov	r10, r0
	cbnz	r0, .L45
	ldr	r1, [r5, #40]
	ldr	r0, [r4, #40]
	bl	mpp_enc_hw_start
	cbz	r0, .L47
	movs	r1, #195
	ldr	r0, .L131+8
	bl	printk
	mov	r0, r7
	bl	atomic_sub.constprop.0
	add	r0, r5, #32
	bl	atomic_sub.constprop.0
	mov	r3, r10
	movs	r2, #1
	movs	r1, #3
	add	r0, r5, #56
	bl	__wake_up
	b	.L42
.L45:
	bl	get_mpidev_ops
	movs	r1, #202
	mov	r10, r0
	ldr	r0, .L131+12
	bl	printk
	add	r0, r5, #32
	bl	atomic_sub.constprop.0
	mov	r0, r7
	bl	atomic_sub.constprop.0
	mov	r3, fp
	movs	r2, #1
	movs	r1, #3
	add	r0, r5, #56
	bl	__wake_up
	cmp	r10, #0
	beq	.L44
	ldr	r3, [r10, #28]
	cbz	r3, .L44
	ldr	r0, [r5]
	blx	r3
.L44:
	movs	r1, #0
	ldr	r0, [r4, #40]
	bl	mpp_enc_hw_start
	cmp	r0, #0
	bne	.L42
.L47:
	bl	mpp_time
	sub	r0, r0, r8
	str	r0, [r4, #112]
.L40:
	ldr	r0, [sp, #20]
	cbz	r0, .L55
	ldr	r1, .L131+16
	bl	mpp_buffer_put_with_caller
.L55:
	mov	r0, r6
	bl	enc_chan_update_tab_after_enc
	b	.L127
.L56:
	mov	r7, r5
	b	.L16
.L132:
	.align	2
.L131:
	.word	.LANCHOR0+14
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LANCHOR0+43
	.fnend
	.size	enc_chan_process_single_chan, .-enc_chan_process_single_chan
	.align	1
	.global	mpp_vcodec_enc_add_packet_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_enc_add_packet_list, %function
mpp_vcodec_enc_add_packet_list:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L140
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r4, r0
	ldr	r3, [r3]
	mov	r5, r1
	str	r1, [sp, #4]
	lsls	r3, r3, #26
	bpl	.L134
	mov	r0, r1
	bl	mpp_packet_get_length
	movs	r2, #248
	ldr	r1, .L140+4
	mov	r3, r0
	ldr	r0, .L140+8
	bl	printk
.L134:
	bl	get_vsm_ops
	mov	r6, r0
	cbnz	r0, .L135
	add	r7, r4, #64
	mov	r0, r7
	bl	mutex_lock
	ldr	r3, [r4, #92]
	add	r2, r5, #12
	add	r1, r4, #88
	str	r2, [r4, #92]
	add	r0, r4, #104
	strd	r1, r3, [r5, #12]
	str	r2, [r3]
	bl	atomic_add.constprop.1
	mov	r0, r7
	bl	mutex_unlock
	mov	r3, r6
	movs	r2, #1
	movs	r1, #3
	add	r0, r4, #48
	bl	__wake_up
.L136:
	movs	r3, #0
	str	r3, [r4, #36]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L135:
	add	r0, sp, #4
	bl	mpp_packet_deinit
	b	.L136
.L141:
	.align	2
.L140:
	.word	mpp_vcodec_debug
	.word	.LANCHOR0+72
	.word	.LC11
	.fnend
	.size	mpp_vcodec_enc_add_packet_list, .-mpp_vcodec_enc_add_packet_list
	.align	1
	.global	mpp_vcodec_enc_int_handle
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_enc_int_handle, %function
mpp_vcodec_enc_int_handle:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	movs	r1, #1
	movs	r3, #0
	mov	r8, r0
	strd	r3, r3, [sp]
	bl	mpp_vcodec_get_chan_entry
	movs	r5, #1
	str	r5, [r0, #36]
	mov	r4, r0
	bl	mpp_vcodec_get_enc_module_entry
	mov	r7, r0
	cbnz	r0, .L143
	movw	r2, #275
	ldr	r1, .L165
	ldr	r0, .L165+4
	bl	printk
.L142:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L143:
	ldr	r1, [r4, #20]
	cmp	r1, #0
	beq	.L145
	mov	r1, r5
	ldr	r0, [r4, #4]
	bl	mpp_vcodec_get_chan_entry
	mov	r5, r0
	cmp	r0, #0
	beq	.L151
	ldr	r6, [r0, #40]
	cbz	r6, .L146
	bl	get_mpidev_ops
	mov	r9, r0
	bl	mpp_time
	strd	r0, [r4, #136]
	add	r3, sp, #4
	ldr	r1, [r5, #40]
	mov	r2, sp
	ldr	r0, [r4, #40]
	bl	mpp_enc_int_process
	ldr	r1, [sp, #4]
	mov	r6, r0
	cbz	r1, .L147
	mov	r0, r5
	bl	mpp_vcodec_enc_add_packet_list
.L146:
	add	r0, r4, #20
	bl	atomic_sub.constprop.0
	add	r0, r5, #32
	bl	atomic_sub.constprop.0
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	add	r0, r5, #56
	bl	__wake_up
.L148:
	ldr	r1, [sp]
	cbz	r1, .L149
	mov	r0, r4
	bl	mpp_vcodec_enc_add_packet_list
.L149:
	cbz	r6, .L150
	mov	r2, r8
	movw	r1, #305
	ldr	r0, .L165+8
	bl	printk
	movs	r3, #0
	str	r3, [r4, #36]
.L150:
	add	r0, r4, #32
	bl	atomic_sub.constprop.0
	add	r0, r4, #56
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	bl	__wake_up
	ldr	r0, [r7, #1568]
	bl	vcodec_thread_trigger
	b	.L142
.L147:
	cmp	r9, #0
	beq	.L146
	ldr	r3, [r9, #28]
	cmp	r3, #0
	beq	.L146
	ldr	r0, [r5]
	blx	r3
	b	.L146
.L151:
	mov	r6, r0
	b	.L146
.L145:
	add	r3, sp, #4
	mov	r2, sp
	ldr	r0, [r4, #40]
	bl	mpp_enc_int_process
	mov	r6, r0
	b	.L148
.L166:
	.align	2
.L165:
	.word	.LANCHOR0+103
	.word	.LC12
	.word	.LC13
	.fnend
	.size	mpp_vcodec_enc_int_handle, .-mpp_vcodec_enc_int_handle
	.align	1
	.global	mpp_vcodec_enc_routine
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_enc_routine, %function
mpp_vcodec_enc_routine:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	bl	mpp_vcodec_get_enc_module_entry
	mov	r6, r0
	cbnz	r0, .L168
	ldr	r1, .L184
	mov	r2, #322
	ldr	r0, .L184+4
.L183:
	bl	printk
.L169:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L168:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #0
	ldr	r5, [r0, #1492]
	str	r2, [r0, #1496]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r4, .L184+8
	ldr	r2, [r4]
	lsls	r2, r2, #26
	bpl	.L171
	mov	r3, r5
	mov	r2, #330
	ldr	r1, .L184
	ldr	r0, .L184+12
	bl	printk
.L171:
	cbz	r5, .L176
	bl	enc_chan_get_high_prior_chan
	ldr	r0, [r6, #1500]
	cmp	r0, #7
	bls	.L172
.L173:
	subs	r5, r5, #1
	b	.L171
.L172:
	bl	enc_chan_process_single_chan
	cmp	r0, #0
	beq	.L173
.L176:
	ldr	r3, [r4]
	lsls	r3, r3, #26
	bpl	.L169
	movw	r2, #341
	ldr	r1, .L184
	ldr	r0, .L184+16
	b	.L183
.L185:
	.align	2
.L184:
	.word	.LANCHOR0+129
	.word	.LC12
	.word	mpp_vcodec_debug
	.word	.LC14
	.word	.LC15
	.fnend
	.size	mpp_vcodec_enc_routine, .-mpp_vcodec_enc_routine
	.align	1
	.global	mpp_vcodec_dec_routine
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_dec_routine, %function
mpp_vcodec_dec_routine:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_vcodec_dec_routine, .-mpp_vcodec_dec_routine
	.global	max_packet_num
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.26047, %object
	.size	__FUNCTION__.26047, 14
__FUNCTION__.26047:
	.ascii	"frame_add_osd\000"
	.type	__func__.26072, %object
	.size	__func__.26072, 29
__func__.26072:
	.ascii	"enc_chan_process_single_chan\000"
	.type	__FUNCTION__.26084, %object
	.size	__FUNCTION__.26084, 29
__FUNCTION__.26084:
	.ascii	"enc_chan_process_single_chan\000"
	.type	__func__.26102, %object
	.size	__func__.26102, 31
__func__.26102:
	.ascii	"mpp_vcodec_enc_add_packet_list\000"
	.type	__func__.26112, %object
	.size	__func__.26112, 26
__func__.26112:
	.ascii	"mpp_vcodec_enc_int_handle\000"
	.type	__func__.26125, %object
	.size	__func__.26125, 23
__func__.26125:
	.ascii	"mpp_vcodec_enc_routine\000"
	.type	__param_str_max_packet_num, %object
	.size	__param_str_max_packet_num, 26
__param_str_max_packet_num:
	.ascii	"mpp_vcodec.max_packet_num\000"
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	max_packet_num, %object
	.size	max_packet_num, 4
max_packet_num:
	.word	100
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_max_packet_numtype162, %object
	.size	__UNIQUE_ID_max_packet_numtype162, 39
__UNIQUE_ID_max_packet_numtype162:
	.ascii	"mpp_vcodec.parmtype=max_packet_num:int\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013mpp_vcodec: %s:%d: chan_entry is NULL\000"
.LC1:
	.ascii	"\0013mpp_vcodec: %s:%d: mpibuf_ops get fail\000"
.LC2:
	.ascii	"\0013mpp_vcodec: %d: cur chnl %d state is no runing"
	.ascii	"\000"
.LC3:
	.ascii	"\0016mpp_vcodec: %s:%d: cur chnl %d state is wating"
	.ascii	" irq\000"
.LC4:
	.ascii	"\0016mpp_vcodec: %s:%d: enc_chan_process_single_cha"
	.ascii	"n id %d\000"
.LC5:
	.ascii	"\0016mpp_vcodec: %d: cur chnl %d pkt pool non free "
	.ascii	"buf\000"
.LC6:
	.ascii	"\0013mpp_vcodec: %d: get buf info fail\000"
.LC7:
	.ascii	"\0016mpp_vcodec: %s:%d: attach jpeg id %d \012\000"
.LC8:
	.ascii	"\0016mpp_vcodec: %s:%d: attach jpeg id %d\000"
.LC9:
	.ascii	"\0013mpp_vcodec: %d: combo start fail \012\000"
.LC10:
	.ascii	"\0013mpp_vcodec: %d: combo cfg fail \012\000"
.LC11:
	.ascii	"\0016mpp_vcodec: %s:%d: packet size %zu\000"
.LC12:
	.ascii	"\0013mpp_vcodec: %s:%d: get_enc_module_entry fail\000"
.LC13:
	.ascii	"\0013mpp_vcodec: %d: enc %d handle int err\000"
.LC14:
	.ascii	"\0016mpp_vcodec: %s:%d: mpp_vcodec_enc_routine star"
	.ascii	"ted_chan_num %d\000"
.LC15:
	.ascii	"\0016mpp_vcodec: %s:%d: mpp_vcodec_enc_routine end "
	.ascii	"return \012\000"
	.section	__param,"a"
	.align	2
	.type	__param_max_packet_num, %object
	.size	__param_max_packet_num, 20
__param_max_packet_num:
	.word	__param_str_max_packet_num
	.word	0
	.word	param_ops_int
	.short	420
	.byte	-1
	.byte	0
	.word	max_packet_num
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
