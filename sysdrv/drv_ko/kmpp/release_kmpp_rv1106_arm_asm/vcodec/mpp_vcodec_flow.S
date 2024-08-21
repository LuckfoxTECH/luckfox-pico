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
@ 224 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
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
	.size	atomic_add.constprop.1, .-atomic_add.constprop.1
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_chan_process_single_chan, %function
enc_chan_process_single_chan:
	.fnstart
	@ args = 0, pretend = 0, frame = 160
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	movs	r1, #1
	.pad #172
	sub	sp, sp, #172
	mov	r6, r0
	bl	mpp_vcodec_get_chan_entry
	movs	r5, #0
	mov	r4, r0
	strd	r5, r5, [sp, #20]
	str	r5, [sp, #28]
	bl	get_mpibuf_ops
	mov	r8, r0
	cbnz	r4, .L17
	ldr	r1, .L141
	movs	r2, #104
	ldr	r0, .L141+4
.L140:
	bl	printk
	mov	r0, #-1
.L16:
	add	sp, sp, #172
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L17:
	cbnz	r0, .L19
	movs	r2, #108
	ldr	r1, .L141
	ldr	r0, .L141+8
	b	.L140
.L19:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #28]
	cmp	r3, #3
	beq	.L20
	mov	r2, r6
	movs	r1, #114
	ldr	r0, .L141+12
	bl	printk
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L139:
	movs	r0, #0
	b	.L16
.L20:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #36]
	ldr	r9, .L141+36
	cmp	r3, #0
	ble	.L21
	ldr	r0, [r9]
	ands	r0, r0, #32
	beq	.L16
	mov	r3, r6
	movs	r2, #121
	ldr	r1, .L141
	ldr	r0, .L141+16
	bl	printk
	b	.L139
.L21:
	ldr	r3, [r9]
	lsls	r2, r3, #26
	bpl	.L22
	mov	r3, r6
	movs	r2, #125
	ldr	r1, .L141
	ldr	r0, .L141+20
	bl	printk
.L22:
	ldr	r5, [r4, #40]
	cmp	r5, #0
	bne	.L60
	ldr	r0, [r4, #44]
	bl	mpp_enc_check_pkt_pool
	cbnz	r0, .L24
	mov	r2, r6
	movs	r1, #130
	ldr	r0, .L141+24
	bl	printk
	b	.L139
.L24:
	ldr	r3, [r8, #40]
	cmp	r3, #0
	beq	.L139
	ldr	r0, [r4, #48]
	blx	r3
	mov	r7, r0
	cmp	r0, #0
	beq	.L139
	bl	mpp_time
	ldr	r3, [r4, #144]
	subs	r0, r0, r3
	str	r0, [r4, #168]
	bl	mpp_time
	ldr	fp, [r4]
	strd	r0, [r4, #144]
	bl	get_mpibuf_ops
	movs	r2, #104
	mov	r1, r5
	mov	r10, r0
	add	r0, sp, #64
	bl	memset
	cmp	r10, #0
	beq	.L27
	ldr	r3, [r10, #52]
	cbnz	r3, .L28
.L27:
	movs	r1, #59
	ldr	r0, .L141+28
	bl	printk
.L29:
	ldr	r2, .L141+32
	movs	r1, #0
	mov	r0, r7
	bl	mpi_buf_unref_with_tag
	b	.L139
.L142:
	.align	2
.L141:
	.word	.LANCHOR0+14
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LANCHOR0+43
	.word	mpp_vcodec_debug
.L28:
	mov	r2, fp
	add	r1, sp, #64
	mov	r0, r7
	blx	r3
	cmp	r0, #0
	bne	.L29
	add	r0, sp, #20
	bl	mpp_frame_init
	ldr	r1, [sp, #64]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_width
	ldr	r1, [sp, #68]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_height
	ldr	r1, [sp, #72]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_hor_stride
	ldr	r1, [sp, #76]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_ver_stride
	ldrd	r2, [sp, #104]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_pts
	ldrd	r2, [sp, #144]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_dts
	ldr	r1, [sp, #92]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_fmt
	ldr	r1, [sp, #84]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_offset_x
	ldr	r1, [sp, #88]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_offset_y
	ldr	r1, [sp, #128]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_is_gray
	ldr	r1, [sp, #132]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_is_full
	ldr	r1, [sp, #136]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_phy_addr
	ldr	r1, [sp, #156]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_idr_request
	ldrb	r1, [sp, #160]	@ zero_extendqisi2
	ldr	r0, [sp, #20]
	and	r1, r1, #1
	bl	mpp_frame_set_eos
	ldr	r1, [sp, #116]
	cbz	r1, .L30
	ldr	r0, [sp, #20]
	bl	frame_add_osd
.L30:
	ldr	r1, [sp, #152]
	cbz	r1, .L31
	ldr	r0, [sp, #20]
	bl	mpp_frame_add_ppinfo
.L31:
	movs	r2, #32
	movs	r1, #0
	add	r0, sp, r2
	ldr	r5, .L143
	bl	memset
	movs	r3, #0
	mov	r0, r3
	str	r5, [sp]
	add	r2, sp, #28
	add	r1, sp, #32
	str	r7, [sp, #44]
	bl	mpp_buffer_import_with_tag
	ldr	r3, [r9]
	lsls	r3, r3, #25
	bpl	.L32
	ldr	r3, [sp, #112]
	movs	r2, #149
	sub	r1, r5, #29
	ldr	r0, .L143+4
	bl	printk
.L32:
	ldr	r0, [sp, #112]
	cmp	r0, #0
	bgt	.L33
.L138:
	movs	r5, #0
.L34:
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #20]
	bl	mpp_frame_set_buffer
	ldr	r3, [r8, #16]
	cbz	r3, .L23
	mov	r0, r7
	blx	r3
.L23:
	ldr	r3, [sp, #20]
	cbnz	r3, .L40
	ldr	r3, [r4, #40]
	cmp	r3, #0
	beq	.L41
.L40:
	bl	mpp_time
	add	r10, r4, #36
	str	r0, [sp, #12]
	mov	r0, r10
	bl	atomic_add.constprop.1
	ldr	r3, [r4, #84]
	ldr	r2, [r4, #88]
	add	r3, r3, r2
	ldr	r2, .L143+8
	ldr	r2, [r2]
	cmp	r3, r2
	ble	.L42
	ldr	r3, [r4, #40]
	cmp	r3, #0
	bne	.L42
	add	r0, sp, #20
	mov	r7, #-1
	bl	mpp_frame_deinit
	ldr	r0, [r4, #44]
	bl	mpp_enc_pkt_full_inc
.L43:
	ldrd	r2, [sp, #144]
	strd	r2, [r4, #128]
	cbnz	r7, .L45
	cmp	r5, #0
	beq	.L46
	ldr	r3, [r5, #44]
	cmp	r3, #0
	beq	.L46
	add	r9, r5, #36
	add	r8, r4, #24
	mov	r0, r9
	bl	atomic_add.constprop.1
	mov	r0, r8
	bl	atomic_add.constprop.1
	ldr	r0, [r5, #44]
	ldr	r1, [sp, #24]
	bl	mpp_enc_cfg_reg
	cmp	r0, #0
	bne	.L47
	ldr	r1, [r5, #44]
	ldr	r0, [r4, #44]
	bl	mpp_enc_hw_start
	cmp	r0, #0
	beq	.L48
	movs	r1, #202
	ldr	r0, .L143+12
	bl	printk
	mov	r0, r8
	bl	atomic_sub.constprop.0
	mov	r0, r9
	bl	atomic_sub.constprop.0
	mov	r3, r7
	movs	r2, #1
	movs	r1, #3
	add	r0, r5, #60
	bl	__wake_up
.L45:
	bl	get_mpidev_ops
	mov	r7, r0
	bl	mpp_vcodec_get_enc_module_entry
	mov	r8, r0
	mov	r0, r10
	bl	atomic_sub.constprop.0
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	add	r0, r4, #60
	bl	__wake_up
	ldr	r3, [sp, #20]
	cbz	r3, .L54
	cbz	r7, .L55
	ldr	r3, [r7, #28]
	cbz	r3, .L55
	ldr	r0, [r4]
	blx	r3
.L55:
	add	r0, sp, #20
	bl	mpp_frame_deinit
.L54:
	ldr	r3, [sp, #24]
	cbz	r3, .L56
	cbz	r7, .L57
	ldr	r3, [r7, #28]
	cbz	r3, .L57
	cbz	r5, .L57
	ldr	r0, [r5]
	blx	r3
.L57:
	add	r0, sp, #24
	bl	mpp_frame_deinit
.L56:
	movw	r3, #13856
	ldr	r0, [r8, r3]
	bl	vcodec_thread_trigger
	b	.L49
.L33:
	movs	r1, #1
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #28]
	mov	r5, r0
	cmp	r3, #3
	it	ne
	movne	r5, #0
	ldr	r3, [r5, #36]
	cmp	r3, #0
	ble	.L36
	ldr	r3, [sp, #112]
	movs	r2, #157
	ldr	r1, .L143+16
	ldr	r0, .L143+20
	str	r3, [sp]
	mov	r3, r6
	bl	printk
	b	.L138
.L36:
	cmp	r5, #0
	beq	.L138
	ldr	r3, [r5, #44]
	cmp	r3, #0
	beq	.L34
	bl	mpp_time
	ldr	r3, [r4, #152]
	subs	r0, r0, r3
	str	r0, [r4, #172]
	bl	mpp_time
	ldr	r3, [sp, #112]
	strd	r0, [r4, #152]
	add	r0, sp, #24
	str	r3, [r4, #4]
	bl	mpp_frame_init
	ldrd	r1, r0, [sp, #20]
	bl	mpp_frame_copy
	ldr	r1, [sp, #124]
	cbz	r1, .L38
	ldr	r0, [sp, #24]
	bl	frame_add_osd
.L38:
	ldrd	r0, r1, [sp, #24]
	bl	mpp_frame_set_buffer
	b	.L34
.L60:
	movs	r5, #0
	b	.L23
.L42:
	ldr	r1, [sp, #20]
	ldr	r0, [r4, #44]
	bl	mpp_enc_cfg_reg
	mov	r7, r0
	b	.L43
.L48:
	ldr	r3, [r4]
	str	r3, [r5, #8]
.L49:
	bl	mpp_time
	ldr	r3, [sp, #12]
	subs	r0, r0, r3
	str	r0, [r4, #136]
.L41:
	ldr	r0, [sp, #28]
	cbz	r0, .L58
	ldr	r1, .L143
	bl	mpp_buffer_put_with_caller
.L58:
	mov	r0, r6
	bl	enc_chan_update_tab_after_enc
	b	.L139
.L47:
	bl	get_mpidev_ops
	movs	r1, #212
	mov	fp, r0
	ldr	r0, .L143+24
	bl	printk
	mov	r0, r9
	bl	atomic_sub.constprop.0
	mov	r0, r8
	bl	atomic_sub.constprop.0
	mov	r3, r7
	movs	r2, #1
	movs	r1, #3
	add	r0, r5, #60
	bl	__wake_up
	cmp	fp, #0
	beq	.L50
	ldr	r3, [fp, #28]
	cbz	r3, .L50
	ldr	r0, [r5]
	blx	r3
.L50:
	ldr	r3, [sp, #24]
	cbz	r3, .L46
	add	r0, sp, #24
	bl	mpp_frame_deinit
.L46:
	movs	r1, #0
	ldr	r0, [r4, #44]
	bl	mpp_enc_hw_start
	cmp	r0, #0
	beq	.L49
	b	.L45
.L144:
	.align	2
.L143:
	.word	.LANCHOR0+43
	.word	.LC7
	.word	.LANCHOR1
	.word	.LC9
	.word	.LANCHOR0+14
	.word	.LC8
	.word	.LC10
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
	ldr	r3, .L152
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r4, r0
	ldr	r3, [r3]
	mov	r5, r1
	str	r1, [sp, #4]
	lsls	r3, r3, #26
	bpl	.L146
	mov	r0, r1
	bl	mpp_packet_get_length
	mov	r2, #262
	ldr	r1, .L152+4
	mov	r3, r0
	ldr	r0, .L152+8
	bl	printk
.L146:
	bl	get_vsm_ops
	mov	r6, r0
	cbnz	r0, .L147
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #72]
	add	r2, r5, #12
	add	r1, r4, #68
	str	r2, [r4, #72]
	add	r0, r4, #84
	strd	r1, r3, [r5, #12]
	str	r2, [r3]
	bl	atomic_add.constprop.1
	add	r0, r4, #92
	bl	atomic_add.constprop.1
	ldr	r0, [sp, #4]
	bl	mpp_packet_get_dts
	strd	r0, [r4, #120]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #1
	mov	r3, r6
	movs	r1, #3
	add	r0, r4, #52
	bl	__wake_up
.L148:
	movs	r3, #0
	str	r3, [r4, #40]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L147:
	add	r0, sp, #4
	bl	mpp_packet_deinit
	b	.L148
.L153:
	.align	2
.L152:
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
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	movs	r1, #1
	movs	r3, #0
	strd	r3, r3, [sp]
	bl	mpp_vcodec_get_chan_entry
	movs	r5, #1
	str	r5, [r0, #40]
	mov	r4, r0
	bl	mpp_vcodec_get_enc_module_entry
	mov	r7, r0
	cbnz	r0, .L155
	movw	r2, #289
	ldr	r1, .L177
	ldr	r0, .L177+4
	bl	printk
.L154:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L155:
	ldr	r1, [r4, #24]
	cmp	r1, #0
	beq	.L157
	mov	r1, r5
	ldr	r0, [r4, #4]
	bl	mpp_vcodec_get_chan_entry
	mov	r5, r0
	cmp	r0, #0
	beq	.L163
	ldr	r6, [r0, #44]
	cbz	r6, .L158
	bl	get_mpidev_ops
	mov	r8, r0
	bl	mpp_time
	strd	r0, [r4, #160]
	add	r3, sp, #4
	ldr	r1, [r5, #44]
	mov	r2, sp
	ldr	r0, [r4, #44]
	bl	mpp_enc_int_process
	ldr	r1, [sp, #4]
	mov	r6, r0
	cbz	r1, .L159
	mov	r0, r5
	bl	mpp_vcodec_enc_add_packet_list
.L158:
	add	r0, r4, #24
	bl	atomic_sub.constprop.0
	add	r0, r5, #36
	bl	atomic_sub.constprop.0
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	add	r0, r5, #60
	bl	__wake_up
	mov	r3, #-1
	str	r3, [r4, #4]
	str	r3, [r5, #8]
.L160:
	ldr	r1, [sp]
	cbz	r1, .L161
	mov	r0, r4
	bl	mpp_vcodec_enc_add_packet_list
.L161:
	cbz	r6, .L162
	movs	r3, #0
	str	r3, [r4, #40]
.L162:
	add	r0, r4, #36
	bl	atomic_sub.constprop.0
	add	r0, r4, #60
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	bl	__wake_up
	movw	r3, #13856
	ldr	r0, [r7, r3]
	bl	vcodec_thread_trigger
	b	.L154
.L159:
	cmp	r8, #0
	beq	.L158
	ldr	r3, [r8, #28]
	cmp	r3, #0
	beq	.L158
	ldr	r0, [r5]
	blx	r3
	b	.L158
.L163:
	mov	r6, r0
	b	.L158
.L157:
	add	r3, sp, #4
	mov	r2, sp
	ldr	r0, [r4, #44]
	bl	mpp_enc_int_process
	mov	r6, r0
	b	.L160
.L178:
	.align	2
.L177:
	.word	.LANCHOR0+103
	.word	.LC12
	.fnend
	.size	mpp_vcodec_enc_int_handle, .-mpp_vcodec_enc_int_handle
	.align	1
	.global	mpp_vcodec_enc_run_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_enc_run_task, %function
mpp_vcodec_enc_run_task:
	.fnstart
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	movs	r1, #1
	mov	r4, r2
	mov	r5, r3
	ldrd	r6, [sp, #24]
	bl	mpp_vcodec_get_chan_entry
	cbz	r0, .L182
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r0, #28]
	cmp	r2, #3
	beq	.L181
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, #-1
.L179:
	pop	{r4, r5, r6, r7, r8, pc}
.L181:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	strd	r6, [sp, #24]
	mov	r2, r4
	mov	r3, r5
	ldr	r0, [r0, #44]
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpp_enc_run_task
.L182:
	mvn	r0, #21
	b	.L179
	.fnend
	.size	mpp_vcodec_enc_run_task, .-mpp_vcodec_enc_run_task
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
	mov	r4, r0
	cbnz	r0, .L184
	mov	r2, #356
	ldr	r1, .L199
	ldr	r0, .L199+4
	bl	printk
	mvn	r0, #5
.L183:
	pop	{r4, r5, r6, pc}
.L184:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movw	r2, #13332
	movs	r1, #0
	ldr	r6, [r0, r2]
	movw	r2, #13336
	str	r1, [r0, r2]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r5, .L199+8
	ldr	r2, [r5]
	lsls	r3, r2, #26
	bpl	.L186
	mov	r3, r6
	mov	r2, #364
	ldr	r1, .L199
	ldr	r0, .L199+12
	bl	printk
.L186:
	add	r4, r4, #13312
	adds	r4, r4, #28
.L187:
	cbz	r6, .L192
	bl	enc_chan_get_high_prior_chan
	ldr	r0, [r4]
	cmp	r0, #63
	bls	.L188
.L189:
	subs	r6, r6, #1
	b	.L187
.L188:
	bl	enc_chan_process_single_chan
	cmp	r0, #0
	beq	.L189
.L192:
	ldr	r0, [r5]
	ands	r0, r0, #32
	beq	.L183
	mov	r2, #374
	ldr	r1, .L199
	ldr	r0, .L199+16
	bl	printk
	movs	r0, #0
	b	.L183
.L200:
	.align	2
.L199:
	.word	.LANCHOR0+129
	.word	.LC13
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
	.type	__FUNCTION__.26200, %object
	.size	__FUNCTION__.26200, 14
__FUNCTION__.26200:
	.ascii	"frame_add_osd\000"
	.type	__func__.26225, %object
	.size	__func__.26225, 29
__func__.26225:
	.ascii	"enc_chan_process_single_chan\000"
	.type	__FUNCTION__.26237, %object
	.size	__FUNCTION__.26237, 29
__FUNCTION__.26237:
	.ascii	"enc_chan_process_single_chan\000"
	.type	__func__.26260, %object
	.size	__func__.26260, 31
__func__.26260:
	.ascii	"mpp_vcodec_enc_add_packet_list\000"
	.type	__func__.26276, %object
	.size	__func__.26276, 26
__func__.26276:
	.ascii	"mpp_vcodec_enc_int_handle\000"
	.type	__func__.26302, %object
	.size	__func__.26302, 23
__func__.26302:
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
	.ascii	"\0013mpp_vcodec: %s:%d: chan_entry is NULL\012\000"
.LC1:
	.ascii	"\0013mpp_vcodec: %s:%d: mpibuf_ops get fail\012\000"
.LC2:
	.ascii	"\0013mpp_vcodec: %d: cur chnl %d state is no runing"
	.ascii	"\012\000"
.LC3:
	.ascii	"\0016mpp_vcodec: %s:%d: cur chnl %d state is wating"
	.ascii	" irq\012\000"
.LC4:
	.ascii	"\0016mpp_vcodec: %s:%d: enc_chan_process_single_cha"
	.ascii	"n id %d\012\000"
.LC5:
	.ascii	"\0016mpp_vcodec: %d: cur chnl %d pkt pool non free "
	.ascii	"buf\012\000"
.LC6:
	.ascii	"\0013mpp_vcodec: %d: get buf info fail\000"
.LC7:
	.ascii	"\0016mpp_vcodec: %s:%d: attach jpeg id %d\012\000"
.LC8:
	.ascii	"\0013mpp_vcodec: %s:%d: chan %d combo chan %d state"
	.ascii	" is wating irq\012\000"
.LC9:
	.ascii	"\0013mpp_vcodec: %d: combo start fail \012\000"
.LC10:
	.ascii	"\0013mpp_vcodec: %d: combo cfg fail \012\000"
.LC11:
	.ascii	"\0016mpp_vcodec: %s:%d: packet size %zu\012\000"
.LC12:
	.ascii	"\0013mpp_vcodec: %s:%d: get_enc_module_entry fail\000"
.LC13:
	.ascii	"\0013mpp_vcodec: %s:%d: get_enc_module_entry fail\012"
	.ascii	"\000"
.LC14:
	.ascii	"\0016mpp_vcodec: %s:%d: mpp_vcodec_enc_routine star"
	.ascii	"ted_chan_num %d\012\000"
.LC15:
	.ascii	"\0016mpp_vcodec: %s:%d: mpp_vcodec_enc_routine end\012"
	.ascii	"\000"
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
