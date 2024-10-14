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
	.file	"mpp_enc_impl.c"
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
	.type	check_hal_task_pkt_len, %function
check_hal_task_pkt_len:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r5, r1
	ldr	r4, [r0, #48]
	ldr	r0, [r0, #28]
	bl	mpp_packet_get_length
	cmp	r4, r0
	beq	.L1
	strd	r4, r0, [sp]
	mov	r3, r5
	movs	r2, #181
	ldr	r1, .L3
	ldr	r0, .L3+4
	bl	printk
.L1:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L4:
	.align	2
.L3:
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	check_hal_task_pkt_len, .-check_hal_task_pkt_len
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_check_frm_pkt, %function
mpp_enc_check_frm_pkt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	movs	r3, #0
	str	r3, [r0, #392]
	mov	r4, r0
	ldr	r3, [r0, #372]
	cbz	r3, .L6
.L10:
	ldr	r0, [r4, #368]
	cbnz	r0, .L7
.L8:
	mov	r0, #-1
	b	.L5
.L6:
	add	r0, r0, #372
	bl	mpp_packet_new
	cmp	r0, #0
	beq	.L10
.L5:
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L7:
	bl	mpp_frame_get_pts
	mov	r6, r0
	ldr	r0, [r4, #368]
	mov	r7, r1
	bl	mpp_frame_get_dts
	mov	r8, r0
	ldr	r0, [r4, #368]
	mov	r9, r1
	bl	mpp_frame_get_buffer
	mov	r5, r0
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_hor_stride
	mov	r10, r0
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_ver_stride
	ldr	r3, [r4, #620]
	cmp	r3, r10
	bne	.L11
	ldr	r3, [r4, #624]
	cmp	r3, r0
	beq	.L12
.L11:
	movw	r1, #1235
	ldr	r0, .L18
	bl	printk
.L12:
	mov	r2, r6
	mov	r3, r7
	ldr	r0, [r4, #372]
	strd	r6, [r4, #384]
	str	r5, [r4, #392]
	bl	mpp_packet_set_pts
	ldr	r0, [r4, #372]
	mov	r2, r8
	mov	r3, r9
	bl	mpp_packet_set_dts
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_eos
	cbz	r0, .L13
	ldr	r0, [r4, #372]
	bl	mpp_packet_set_eos
.L14:
	ldr	r3, [r4, #368]
	cmp	r3, #0
	beq	.L8
	ldr	r0, [r4, #392]
	clz	r0, r0
	lsrs	r0, r0, #5
	rsbs	r0, r0, #0
	b	.L5
.L13:
	ldr	r0, [r4, #372]
	bl	mpp_packet_clr_eos
	b	.L14
.L19:
	.align	2
.L18:
	.word	.LC1
	.fnend
	.size	mpp_enc_check_frm_pkt, .-mpp_enc_check_frm_pkt
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_proc_two_pass, %function
mpp_enc_proc_two_pass:
	.fnstart
	@ args = 0, pretend = 0, frame = 128
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	.pad #140
	sub	sp, sp, #140
	ldr	r0, [r0, #460]
	mov	r7, r1
	bl	mpp_enc_refs_next_frm_is_intra
	mov	r5, r0
	cmp	r0, #0
	beq	.L20
	ldrd	r2, [r4, #216]
	add	r1, r4, #224
	add	r0, sp, #32
	add	r5, r4, #64
	strd	r2, [sp, #24]
	movs	r2, #104
	bl	memcpy
	ldr	r3, [r7, #68]
	ldr	r10, [r7, #48]
	ldrd	fp, r8, [r4, #4]
	mov	r0, r10
	str	r3, [sp, #12]
	ldr	r3, [r7, #64]
	str	r3, [sp, #20]
	bl	mpp_packet_get_length
	ldr	r3, .L70
	ldr	r3, [r3]
	lsls	r6, r3, #26
	str	r0, [sp, #16]
	bpl	.L22
	ldrh	r3, [r4, #222]
	movw	r2, #1409
	ldr	r1, .L70+4
	ldr	r0, .L70+8
	bl	printk
.L22:
	movs	r2, #104
	add	r1, r4, #496
	add	r0, r4, #224
	bl	memcpy
	ldr	r3, .L70
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #32
	beq	.L23
	ldrh	r3, [r4, #222]
	movw	r2, #1412
	ldr	r1, .L70+4
	ldr	r0, .L70+12
	bl	printk
.L23:
	mov	r1, r5
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_get_cpb_pass1
	ldr	r3, [r9]
	lsls	r5, r3, #15
	bpl	.L24
	ldrh	r3, [r4, #78]
	movw	r2, #1415
	ldr	r1, .L70+4
	ldr	r0, .L70+16
	bl	printk
.L24:
	add	r6, r7, #20
	mov	r0, fp
	mov	r1, r6
	bl	enc_impl_proc_dpb
	mov	r5, r0
	cbz	r0, .L25
	str	r0, [sp]
	mov	r3, #1416
	ldr	r0, .L70+20
	mov	r2, r4
	mov	r1, r3
.L69:
	bl	printk
.L21:
.L20:
	mov	r0, r5
	add	sp, sp, #140
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L25:
	ldr	r3, [r9]
	lsls	r0, r3, #26
	bpl	.L27
	ldrh	r3, [r4, #222]
	movw	r2, #1418
	ldr	r1, .L70+4
	ldr	r0, .L70+24
	bl	printk
.L27:
	mov	r1, r6
	mov	r0, fp
	bl	enc_impl_proc_hal
	mov	r5, r0
	cbz	r0, .L28
	movw	r3, #1419
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L70+28
	b	.L69
.L28:
	ldr	r3, .L70
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #32
	beq	.L29
	ldrh	r3, [r4, #222]
	movw	r2, #1421
	ldr	r1, .L70+4
	ldr	r0, .L70+32
	bl	printk
.L29:
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_get_task
	mov	r5, r0
	cbz	r0, .L30
	movw	r3, #1422
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L70+36
	b	.L69
.L30:
	ldr	r3, [r9]
	lsls	r1, r3, #26
	bpl	.L31
	ldrh	r3, [r4, #222]
	mov	r2, #1424
	ldr	r1, .L70+4
	ldr	r0, .L70+40
	bl	printk
.L31:
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_gen_regs
	mov	r5, r0
	cbz	r0, .L32
	movw	r3, #1425
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L70+44
	b	.L69
.L32:
	ldr	r3, [r9]
	lsls	r2, r3, #26
	bpl	.L33
	ldrh	r3, [r4, #222]
	movw	r2, #1427
	ldr	r1, .L70+4
	ldr	r0, .L70+48
	bl	printk
.L33:
	movs	r2, #0
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_start
	mov	r5, r0
	cbz	r0, .L34
	movw	r3, #1429
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L70+52
	b	.L69
.L34:
	ldr	r3, .L70
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #32
	beq	.L35
	ldrh	r3, [r4, #222]
	movw	r2, #1431
	ldr	r1, .L70+4
	ldr	r0, .L70+56
	bl	printk
.L35:
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_wait
	mov	r5, r0
	cbz	r0, .L36
	mov	r3, #1432
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L70+60
	b	.L69
.L36:
	mov	r2, r0
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_ret_task
	mov	r5, r0
	cbz	r0, .L37
	movw	r3, #1436
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L70+64
	b	.L69
.L37:
	ldr	r1, [sp, #16]
	mov	r0, r10
	bl	mpp_packet_set_length
	add	r0, r4, #224
	ldr	r3, [sp, #20]
	add	r1, sp, #32
	str	r3, [r7, #64]
	ldr	r3, [sp, #12]
	str	r3, [r7, #68]
	ldrd	r2, [sp, #24]
	strd	r2, [r4, #216]
	movs	r2, #104
	bl	memcpy
	ldr	r3, [r9]
	lsls	r3, r3, #26
	bpl	.L20
	ldrh	r3, [r4, #222]
	movw	r2, #1446
	ldr	r1, .L70+4
	ldr	r0, .L70+68
	bl	printk
	b	.L20
.L71:
	.align	2
.L70:
	.word	mpp_enc_debug
	.word	.LANCHOR0+23
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.fnend
	.size	mpp_enc_proc_two_pass, .-mpp_enc_proc_two_pass
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	update_rc_cfg_log.constprop.1, %function
update_rc_cfg_log.constprop.1:
	.fnstart
	@ args = 4, pretend = 12, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 1
	push	{r1, r2, r3}
	.save {r1, r2, r3}
	push	{r0, r1, r2, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #12
	mov	r5, r0
	ldrd	r4, r6, [r0, #444]
	add	r3, sp, #32
	ldr	r0, [r0, #436]
	subs	r1, r6, r4
	ldr	r2, .L74
	add	r0, r0, r4
	str	r3, [sp, #4]
	bl	vsnprintf
	add	r4, r4, r0
	cmp	r6, r4
	bgt	.L73
	movw	r2, #993
	ldr	r1, .L74+4
	ldr	r0, .L74+8
	bl	printk
.L73:
	str	r4, [r5, #444]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, lr}
	add	sp, sp, #12
	bx	lr
.L75:
	.align	2
.L74:
	.word	.LC18
	.word	.LANCHOR0+45
	.word	.LC19
	.fnend
	.size	update_rc_cfg_log.constprop.1, .-update_rc_cfg_log.constprop.1
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_terminate_task, %function
mpp_enc_terminate_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	movs	r1, #0
	str	r1, [r0, #368]
	ldr	r0, [r0, #372]
	cbz	r0, .L77
	bl	mpp_packet_set_length
	movw	r3, #10828
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	add	r0, r4, #372
	bl	mpp_packet_deinit
.L77:
	ldrb	r2, [r4, #416]	@ zero_extendqisi2
	movs	r3, #0
	strd	r3, r3, [r4, #368]
	strd	r3, r3, [r4, #392]
	and	r2, r2, #1
	str	r2, [r4, #416]
	str	r3, [r5, #8]
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_enc_terminate_task, .-mpp_enc_terminate_task
	.global	__aeabi_idiv
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_comb_end_jpeg, %function
mpp_enc_comb_end_jpeg:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #36
	sub	sp, sp, #36
	ldr	r5, [r0, #456]
	mov	r4, r0
	ldr	r6, [r0, #8]
	add	r7, r0, #64
	str	r1, [sp, #28]
	bl	get_mpidev_ops
	ldrd	r2, r3, [r5, #64]
	add	r1, r5, #20
	ldr	r8, .L135+40
	subs	r3, r3, r2
	movs	r2, #0
	str	r3, [r5, #68]
	mov	r9, r0
	mov	r0, r6
	bl	mpp_enc_hal_ret_task
	mov	r6, r0
	cmp	r0, #0
	beq	.L82
	str	r0, [sp]
	mov	r3, #1960
	ldr	r0, .L135
	mov	r2, r4
	mov	r1, r3
.L133:
	bl	printk
	ldr	r3, [r4, #52]
	movs	r1, #0
	ldr	r0, [r4, #372]
	adds	r3, r3, #1
	str	r3, [r4, #52]
	bl	mpp_packet_set_length
	movw	r3, #10828
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	add	r0, r4, #372
	bl	mpp_packet_deinit
.L93:
	ldr	r3, [r4, #372]
	ldr	r2, [sp, #28]
	str	r3, [r2]
	movs	r3, #0
	str	r3, [r4, #372]
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L94
	ldrd	r2, [r4, #384]
	ldr	r1, .L135+4
	ldr	r0, .L135+8
	strd	r2, [sp]
	movw	r2, #2010
	ldrh	r3, [r4, #222]
	bl	printk
.L94:
	ldr	r3, [r4, #368]
	cbz	r3, .L95
	add	r0, r4, #368
	bl	mpp_frame_deinit
.L95:
	ldrb	r2, [r4, #416]	@ zero_extendqisi2
	movs	r3, #0
	mov	r0, r6
	strd	r3, r3, [r4, #368]
	strd	r3, r3, [r4, #392]
	and	r2, r2, #1
	str	r2, [r4, #416]
	str	r3, [r5, #8]
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L82:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_hal_end
	mov	r6, r0
	cbz	r0, .L84
	movw	r3, #1961
	str	r0, [sp]
.L134:
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L135+12
	b	.L133
.L84:
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L85
	ldrh	r3, [r4, #222]
	movw	r2, #1962
	ldr	r1, .L135+4
	ldr	r0, .L135+16
	bl	printk
.L85:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_hal_end
	mov	r6, r0
	cbz	r0, .L86
	str	r0, [sp]
	movw	r3, #1963
	b	.L134
.L86:
	ldr	r3, [r8]
	lsls	r1, r3, #26
	bpl	.L87
	ldrh	r3, [r4, #222]
	movw	r2, #1964
	ldr	r1, .L135+4
	ldr	r0, .L135+20
	bl	printk
.L87:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_frm_end
	mov	r6, r0
	cbz	r0, .L88
	movw	r3, #1965
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L135+24
	b	.L133
.L88:
	ldr	r2, [r4, #364]
	movw	r3, #10824
	str	r2, [r4, r3]
	bl	mpp_time
	ldr	r3, [r4, #48]
	adds	r3, r3, #1
	str	r3, [r4, #48]
	strd	r0, [r4, #32]
	cmp	r9, #0
	beq	.L89
	ldr	r3, [r9, #24]
	cbz	r3, .L89
	ldr	r0, [r5, #72]
	bl	mpp_frame_get_dts
	mov	r6, r0
	ldr	r0, [r5, #72]
	mov	r7, r1
	bl	mpp_frame_get_pts
	movs	r3, #1
	mov	r2, r6
	strd	r0, [sp]
	movw	r0, #10828
	str	r3, [sp, #8]
	mov	r3, r7
	ldr	r1, [r9, #24]
	ldr	r0, [r4, r0]
	blx	r1
.L89:
	ldr	r3, [r4, #12]
	cbz	r3, .L90
	ldrd	r2, [r4, #24]
	strd	r2, [sp, #16]
	orrs	r3, r2, r3
	beq	.L90
	ldrd	r6, [r4, #32]
	orrs	r3, r6, r7
	beq	.L90
	ldr	r3, [sp, #16]
	movs	r1, #0
	ldr	r0, .L135+28
	subs	r10, r6, r3
	ldr	r3, [sp, #20]
	sbc	fp, r7, r3
	cmp	r0, r10
	sbcs	r3, r1, fp
	bge	.L90
	ldr	r0, .L135+32
	mov	r2, r10
	ldr	r3, [r4, #48]
	asrs	r2, r2, #31
	smull	r0, r1, r10, r0
	movw	r0, #10000
	muls	r0, r3, r0
	rsb	r1, r2, r1, asr #6
	bl	__aeabi_idiv
	movw	r3, #5364
	str	r0, [r4, r3]
	movs	r3, #0
	strd	r6, [r4, #24]
	str	r3, [r4, #48]
.L90:
	ldrh	r3, [r4, #220]
	ldr	r0, [r4, #372]
	and	r3, r3, #251
	strh	r3, [r4, #220]	@ movhi
	ldr	r1, [r5, #68]
	bl	mpp_packet_set_length
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	lsls	r2, r3, #27
	bpl	.L91
	movs	r1, #16
	ldr	r0, [r4, #372]
	bl	mpp_packet_set_flag
.L91:
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_eos
	cbz	r0, .L92
	movs	r1, #1
	ldr	r0, [r4, #372]
	bl	mpp_packet_set_flag
.L92:
	ldrb	r1, [r4, #217]	@ zero_extendqisi2
	ldr	r0, [r4, #372]
	lsrs	r1, r1, #4
	bl	mpp_packet_set_temporal_id
	movw	r3, #10828
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	mov	r6, r0
	cmp	r0, #0
	beq	.L93
	movw	r2, #1998
	ldr	r1, .L135+4
	ldr	r0, .L135+36
	movs	r6, #0
	bl	printk
	b	.L93
.L136:
	.align	2
.L135:
	.word	.LC16
	.word	.LANCHOR0+63
	.word	.LC24
	.word	.LC20
	.word	.LC14
	.word	.LC21
	.word	.LC22
	.word	999999
	.word	274877907
	.word	.LC23
	.word	mpp_enc_debug
	.fnend
	.size	mpp_enc_comb_end_jpeg, .-mpp_enc_comb_end_jpeg
	.align	1
	.global	mpp_enc_proc_rc_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_proc_rc_cfg, %function
mpp_enc_proc_rc_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 216
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r6, [r1]
	.pad #236
	sub	sp, sp, #236
	mov	r5, r1
	cmp	r6, #0
	beq	.L180
	mov	r1, r0
	movs	r2, #200
	add	r0, sp, #32
	bl	memcpy
	ldrd	r8, r3, [r4, #112]
	lsls	r7, r6, #31
	ldrd	r9, r10, [r4, #128]
	ldr	fp, [r4, #140]
	str	r3, [sp, #16]
	ldr	r3, [r4, #120]
	str	r3, [sp, #20]
	ldr	r3, [r4, #124]
	str	r3, [sp, #24]
	ldr	r3, [r4, #136]
	str	r3, [sp, #28]
	itt	mi
	ldrmi	r3, [r5, #4]
	strmi	r3, [r4, #4]
	lsls	r0, r6, #30
	itt	mi
	ldrmi	r3, [r5, #8]
	strmi	r3, [r4, #8]
	lsls	r1, r6, #29
	bpl	.L141
	ldr	r3, [r5, #12]
	str	r3, [r4, #12]
	ldr	r3, [r5, #16]
	str	r3, [r4, #16]
	ldr	r3, [r5, #20]
	str	r3, [r4, #20]
.L141:
	lsls	r2, r6, #26
	bpl	.L142
	ldr	r3, [r5, #24]
	str	r3, [r4, #24]
	ldr	r3, [r5, #28]
	str	r3, [r4, #28]
	ldr	r3, [r5, #32]
	str	r3, [r4, #32]
.L142:
	lsls	r3, r6, #25
	bpl	.L143
	ldr	r3, [r5, #36]
	str	r3, [r4, #36]
	ldr	r3, [r5, #40]
	str	r3, [r4, #40]
	ldr	r3, [r5, #44]
	str	r3, [r4, #44]
.L143:
	lsls	r7, r6, #24
	itt	mi
	ldrmi	r3, [r5, #48]
	strmi	r3, [r4, #48]
	lsls	r0, r6, #22
	itt	mi
	ldrmi	r3, [r5, #56]
	strmi	r3, [r4, #56]
	lsls	r1, r6, #21
	bpl	.L146
	ldr	r3, [r5, #64]
	str	r3, [r4, #64]
	ldr	r3, [r5, #68]
	str	r3, [r4, #68]
	ldr	r3, [r5, #72]
	str	r3, [r4, #72]
.L146:
	ands	r7, r6, #16384
	beq	.L147
	ldr	r2, [r5, #88]
	cmp	r2, #1
	bls	.L182
	movs	r3, #2
	mov	r1, #376
	str	r3, [sp]
	movs	r3, #0
	ldr	r0, .L257
	mvn	r7, #5
	bl	printk
.L148:
	ldr	r3, [r5, #88]
	str	r3, [r4, #88]
.L147:
	lsls	r2, r6, #16
	bpl	.L149
	ldr	r2, [r5, #76]
	cmp	r2, #2
	bls	.L150
	movs	r3, #3
	movw	r1, #387
	str	r3, [sp]
	movs	r3, #0
	ldr	r0, .L257+4
	mvn	r7, #5
	bl	printk
.L150:
	ldr	r3, [r5, #76]
	str	r3, [r4, #76]
	ldr	r3, [r5, #80]
	str	r3, [r4, #80]
	ldr	r3, [r5, #84]
	str	r3, [r4, #84]
.L149:
	lsls	r3, r6, #7
	bpl	.L151
	ldr	r2, [r5, #92]
	str	r2, [r4, #92]
	ldr	r3, [r5, #96]
	str	r3, [r4, #96]
	cbz	r2, .L151
	cmp	r3, #35
	bls	.L151
	movs	r3, #35
	movs	r2, #0
	mov	r1, #400
	ldr	r0, .L257+8
	bl	printk
	mvn	r7, #5
.L151:
	lsls	r0, r6, #20
	itt	mi
	ldrmi	r3, [r5, #100]
	strmi	r3, [r4, #100]
	lsls	r1, r6, #19
	itt	mi
	ldrmi	r3, [r5, #104]
	strmi	r3, [r4, #104]
	lsls	r2, r6, #18
	itt	mi
	ldrmi	r3, [r5, #108]
	strmi	r3, [r4, #108]
	lsls	r3, r6, #15
	itt	mi
	ldrmi	r3, [r5, #112]
	strmi	r3, [r4, #112]
	lsls	r0, r6, #14
	itttt	mi
	ldrmi	r3, [r5, #124]
	strmi	r3, [r4, #124]
	ldrmi	r3, [r5, #116]
	strmi	r3, [r4, #116]
	lsls	r1, r6, #13
	itttt	mi
	ldrmi	r3, [r5, #128]
	strmi	r3, [r4, #128]
	ldrmi	r3, [r5, #120]
	strmi	r3, [r4, #120]
	lsls	r2, r6, #12
	itt	mi
	ldrmi	r3, [r5, #132]
	strmi	r3, [r4, #132]
	lsls	r3, r6, #11
	itt	mi
	ldrmi	r3, [r5, #136]
	strmi	r3, [r4, #136]
	lsls	r0, r6, #10
	itt	mi
	ldrmi	r3, [r5, #140]
	strmi	r3, [r4, #140]
	lsls	r1, r6, #4
	bpl	.L161
	ldr	r3, [r5, #144]
	str	r3, [r4, #144]
	ldr	r3, [r5, #148]
	str	r3, [r4, #148]
	ldr	r3, [r5, #152]
	str	r3, [r4, #152]
	ldr	r3, [r5, #156]
	str	r3, [r4, #156]
	ldr	r3, [r5, #160]
	str	r3, [r4, #160]
.L161:
	lsls	r2, r6, #6
	bpl	.L162
	ldr	r3, [r5, #164]
	add	r2, r4, #168
	add	r1, r5, #184
	str	r3, [r4, #164]
	add	r3, r5, #168
.L163:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L163
	add	r3, r5, #184
	add	r2, r4, #184
	add	r1, r5, #200
.L164:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L164
.L162:
	lsls	r3, r6, #5
	itt	mi
	ldrmi	r3, [r5, #60]
	strmi	r3, [r4, #60]
	ldr	r3, [r4, #4]
	cmp	r3, #4
	bls	.L166
	ldr	r2, [r5, #4]
	movw	r1, #459
	ldr	r0, .L257+12
	mvn	r7, #5
	bl	printk
.L166:
	ldr	r2, [r4, #8]
	cmp	r2, #6
	bls	.L167
	mov	r1, #464
	ldr	r0, .L257+16
	bl	printk
	mvn	r7, #5
.L167:
	ldr	r3, [r4, #4]
	cmp	r3, #2
	beq	.L168
	ldrd	r2, r0, [r4, #12]
	mvn	lr, #1024
	ldr	r1, .L257+20
	add	r5, r2, lr
	ldr	r3, [r4, #20]
	cmp	r5, r1
	bhi	.L169
	mvn	ip, #1024
	add	r5, r0, ip
	cmp	r5, r1
	bhi	.L169
	mvn	r5, #1024
	adds	r5, r3, r5
	cmp	r5, r1
	bls	.L168
.L169:
	str	r0, [sp]
	mov	r1, #476
	ldr	r0, .L257+24
	mvn	r7, #5
	bl	printk
.L168:
	ldrd	r0, r3, [r4, #124]
	ldr	r2, [r4, #112]
	cmp	r3, #0
	ldrd	r5, r3, [r4, #116]
	it	le
	strle	r0, [r4, #128]
	cmp	r3, #0
	ldr	r3, [r4, #128]
	it	le
	strle	r5, [r4, #120]
	cmp	r0, #0
	ldr	r1, [r4, #120]
	blt	.L172
	cmp	r5, #0
	blt	.L172
	cmp	r5, r0
	blt	.L172
	cmp	r3, #0
	blt	.L172
	cmp	r1, #0
	blt	.L172
	cmp	r1, r3
	blt	.L172
	cmp	r2, #0
	ble	.L173
	cmp	r1, r2
	blt	.L172
	cmp	r3, r2
	ble	.L173
.L172:
	strd	r0, r5, [sp, #4]
	str	r1, [sp]
	mov	r1, #492
	ldr	r0, .L257+28
	bl	printk
	str	r8, [r4, #112]
	str	r9, [r4, #128]
	mov	r2, r8
	mov	r1, #502
	ldr	r0, .L257+32
	ldr	r3, [sp, #20]
	str	r3, [r4, #120]
	ldr	r3, [sp, #24]
	str	r3, [r4, #124]
	ldr	r3, [sp, #16]
	str	r3, [r4, #116]
	str	r3, [sp, #8]
	ldr	r3, [sp, #24]
	str	r3, [sp, #4]
	ldr	r3, [sp, #20]
	str	r3, [sp]
	mov	r3, r9
	bl	printk
.L173:
	ldr	r2, [r4, #136]
	cmp	r2, #0
	bge	.L174
	ldr	r3, [sp, #28]
	mov	r1, #506
	ldr	r0, .L257+36
	bl	printk
	ldr	r3, [sp, #28]
	str	r3, [r4, #136]
.L174:
	ldr	r2, [r4, #140]
	cmp	r2, #0
	bge	.L175
	mov	r3, fp
	movw	r1, #511
	ldr	r0, .L257+40
	bl	printk
	str	fp, [r4, #140]
.L175:
	ldr	r2, [r4, #132]
	cmp	r2, #0
	bge	.L176
	mov	r3, r10
	mov	r1, #516
	ldr	r0, .L257+44
	bl	printk
	str	r10, [r4, #132]
.L176:
	ldr	r2, [r4, #60]
	cmp	r2, #60
	ble	.L177
	movw	r1, #521
	ldr	r0, .L257+48
	bl	printk
.L177:
	ldr	r3, [r4]
	orrs	r6, r6, r3
	str	r6, [r4]
	cbz	r7, .L178
	movw	r2, #527
	ldr	r1, .L257+52
	ldr	r0, .L257+56
	bl	printk
	movs	r2, #200
	mov	r0, r4
	add	r1, sp, #32
	bl	memcpy
.L137:
	mov	r0, r7
	add	sp, sp, #236
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L182:
	movs	r7, #0
	b	.L148
.L178:
	ldr	r3, [r4, #48]
	mov	r1, #532
	ldr	r2, [r4, #12]
	ldr	r0, .L257+60
	str	r3, [sp, #12]
	ldr	r3, [r4, #40]
	str	r3, [sp, #8]
	ldr	r3, [r4, #28]
	str	r3, [sp, #4]
	ldr	r3, [r4, #16]
	str	r3, [sp]
	ldr	r3, [r4, #20]
	bl	printk
	b	.L137
.L180:
	mov	r7, r6
	b	.L137
.L258:
	.align	2
.L257:
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	104856574
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	.LANCHOR0+85
	.word	.LC37
	.word	.LC38
	.fnend
	.size	mpp_enc_proc_rc_cfg, .-mpp_enc_proc_rc_cfg
	.align	1
	.global	mpp_enc_proc_hw_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_proc_hw_cfg, %function
mpp_enc_proc_hw_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 288
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r6, [r1]
	.pad #288
	sub	sp, sp, #288
	mov	r5, r1
	cmp	r6, #0
	beq	.L284
	mov	r1, r0
	mov	r2, #288
	mov	r0, sp
	bl	memcpy
	lsls	r1, r6, #31
	itt	mi
	ldrmi	r3, [r5, #4]
	strmi	r3, [r4, #4]
	lsls	r2, r6, #30
	itt	mi
	ldrmi	r3, [r5, #8]
	strmi	r3, [r4, #8]
	lsls	r3, r6, #25
	itt	mi
	ldrmi	r3, [r5, #12]
	strmi	r3, [r4, #12]
	lsls	r0, r6, #24
	itt	mi
	ldrmi	r3, [r5, #16]
	strmi	r3, [r4, #16]
	lsls	r1, r6, #23
	itt	mi
	ldrmi	r3, [r5, #20]
	strmi	r3, [r4, #20]
	lsls	r2, r6, #22
	itt	mi
	ldrmi	r3, [r5, #24]
	strmi	r3, [r4, #24]
	lsls	r3, r6, #21
	itt	mi
	ldrmi	r3, [r5, #28]
	strmi	r3, [r4, #28]
	lsls	r0, r6, #29
	bpl	.L268
	add	r3, r5, #32
	add	r2, r4, #32
	add	r1, r5, #96
.L269:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L269
.L268:
	lsls	r1, r6, #28
	bpl	.L270
	add	r3, r5, #96
	add	r2, r4, #96
	add	r1, r5, #160
.L271:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L271
.L270:
	lsls	r2, r6, #27
	bpl	.L272
	add	r3, r5, #160
	add	r2, r4, #160
	add	r1, r5, #224
.L273:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L273
.L272:
	lsls	r3, r6, #26
	bpl	.L274
	add	r3, r5, #224
	add	r2, r4, #224
	add	r5, r5, #288
.L275:
	ldr	r1, [r3], #4	@ unaligned
	cmp	r3, r5
	str	r1, [r2], #4	@ unaligned
	bne	.L275
.L274:
	ldrd	r3, r2, [r4, #4]
	cmp	r3, #0
	blt	.L276
	cmp	r2, #0
	bge	.L285
.L276:
	movw	r1, #586
	ldr	r0, .L326
	bl	printk
	mvn	r5, #5
.L277:
	ldr	r2, [r4, #20]
	cmp	r2, #0
	bge	.L278
	movw	r1, #591
	ldr	r0, .L326+4
	bl	printk
	mvn	r5, #5
.L278:
	ldrd	r2, r3, [r4, #12]
	cmp	r2, #0
	blt	.L279
	cmp	r3, #0
	bge	.L280
.L279:
	movw	r1, #597
	ldr	r0, .L326+8
	bl	printk
	mvn	r5, #5
.L280:
	ldrd	r2, r3, [r4, #24]
	cmp	r2, #0
	blt	.L281
	cmp	r3, #0
	bge	.L282
.L281:
	movw	r1, #603
	ldr	r0, .L326+12
	bl	printk
	mvn	r5, #5
.L282:
	ldr	r3, [r4]
	orrs	r6, r6, r3
	str	r6, [r4]
	cbz	r5, .L259
	movw	r2, #610
	ldr	r1, .L326+16
	ldr	r0, .L326+20
	bl	printk
	mov	r2, #288
	mov	r0, r4
	mov	r1, sp
	bl	memcpy
.L259:
	mov	r0, r5
	add	sp, sp, #288
	@ sp needed
	pop	{r4, r5, r6, pc}
.L285:
	movs	r5, #0
	b	.L277
.L284:
	mov	r5, r6
	b	.L259
.L327:
	.align	2
.L326:
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC42
	.word	.LANCHOR0+105
	.word	.LC43
	.fnend
	.size	mpp_enc_proc_hw_cfg, .-mpp_enc_proc_hw_cfg
	.align	1
	.global	mpp_enc_unref_osd_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_unref_osd_buf, %function
mpp_enc_unref_osd_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	cbz	r0, .L341
	ldr	r3, [r0]
	cbz	r3, .L341
	ldr	r7, .L345
	mov	r5, r0
	ldr	r8, .L345+4
	movs	r6, #0
.L331:
	ldr	r3, [r4, #4]
	cmp	r6, r3
	bcc	.L334
.L341:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
.L334:
	ldr	r0, [r5, #136]
	cbz	r0, .L332
	mov	r2, r7
	mov	r1, r8
	bl	mpi_buf_unref_with_tag
.L332:
	ldr	r0, [r5, #88]
	cbz	r0, .L333
	mov	r2, r7
	mov	r1, r8
	bl	mpi_buf_unref_with_tag
.L333:
	adds	r6, r6, #1
	adds	r5, r5, #140
	b	.L331
.L346:
	.align	2
.L345:
	.word	.LANCHOR0+125
	.word	.LC44
	.fnend
	.size	mpp_enc_unref_osd_buf, .-mpp_enc_unref_osd_buf
	.align	1
	.global	mpp_enc_proc_export_osd_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_proc_export_osd_buf, %function
mpp_enc_proc_export_osd_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 104
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #104
	sub	sp, sp, #104
	mov	r7, r0
	bl	get_mpibuf_ops
	movs	r2, #104
	movs	r1, #0
	mov	r5, r0
	mov	r0, sp
	bl	memset
	cbz	r5, .L348
	ldr	r3, [r5, #48]
	cbz	r3, .L348
	ldr	r9, .L362+8
	mov	r4, r7
	ldr	r10, .L362+12
	movs	r6, #0
.L349:
	ldr	r3, [r7, #4]
	cmp	r3, r6
	bhi	.L355
	movs	r0, #0
	b	.L347
.L348:
	mov	r2, #724
	ldr	r1, .L362
	ldr	r0, .L362+4
	bl	printk
	mov	r0, #-1
.L347:
	add	sp, sp, #104
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L355:
	ldr	r0, [r4, #132]
	cmp	r0, #0
	ble	.L351
	bl	dma_buf_get
	cmn	r0, #4096
	mov	r8, r0
	bhi	.L352
	ldr	r3, [r5, #48]
	mov	r2, #-1
	mov	r1, sp
	blx	r3
	str	r0, [r4, #136]
	mov	r0, r8
	bl	dma_buf_put
.L351:
	ldr	r0, [r4, #84]
	cmp	r0, #0
	ble	.L353
	bl	dma_buf_get
	cmn	r0, #4096
	mov	r8, r0
	bhi	.L354
	ldr	r3, [r5, #48]
	mov	r2, #-1
	mov	r1, sp
	blx	r3
	str	r0, [r4, #88]
	mov	r0, r8
	bl	dma_buf_put
.L353:
	adds	r6, r6, #1
	adds	r4, r4, #140
	b	.L349
.L352:
	ldr	r2, [r4, #132]
	movw	r1, #739
	mov	r0, r9
	bl	printk
	b	.L351
.L354:
	ldr	r2, [r4, #84]
	movw	r1, #750
	mov	r0, r10
	bl	printk
	b	.L353
.L363:
	.align	2
.L362:
	.word	.LANCHOR0+147
	.word	.LC45
	.word	.LC46
	.word	.LC47
	.fnend
	.size	mpp_enc_proc_export_osd_buf, .-mpp_enc_proc_export_osd_buf
	.align	1
	.global	mpp_enc_proc_tune_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_proc_tune_cfg, %function
mpp_enc_proc_tune_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r6, [r1]
	.pad #40
	sub	sp, sp, #40
	mov	r8, r1
	cmp	r6, #0
	beq	.L379
	mov	r7, r0
	add	r5, sp, #12
	ldmia	r7!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	lsls	r3, r6, #31
	ldm	r7, {r0, r1, r2}
	it	mi
	ldrmi	r3, [r8, #4]
	stm	r5, {r0, r1, r2}
	it	mi
	strmi	r3, [r4, #4]
	ldr	r2, [r4, #4]
	cmp	r2, #2
	bls	.L380
	movs	r3, #2
	movw	r1, #770
	str	r3, [sp]
	movs	r3, #0
	ldr	r0, .L402
	mvn	r5, #5
	bl	printk
.L367:
	lsls	r7, r6, #30
	itt	mi
	ldrmi	r3, [r8, #8]
	strmi	r3, [r4, #8]
	ldr	r3, [r4, #8]
	cmp	r3, #1
	bls	.L369
	movs	r3, #1
	movs	r2, #0
	movw	r1, #778
	ldr	r0, .L402+4
	bl	printk
	mvn	r5, #5
.L369:
	lsls	r0, r6, #29
	itt	mi
	ldrmi	r3, [r8, #12]
	strmi	r3, [r4, #12]
	ldr	r3, [r4, #12]
	cmp	r3, #7
	bls	.L371
	movw	r1, #786
	ldr	r0, .L402+8
	bl	printk
	mvn	r5, #5
.L371:
	lsls	r1, r6, #28
	itt	mi
	ldrmi	r3, [r8, #16]
	strmi	r3, [r4, #16]
	ldr	r3, [r4, #16]
	cmp	r3, #3
	bls	.L373
	movw	r1, #794
	ldr	r0, .L402+12
	bl	printk
	mvn	r5, #5
.L373:
	lsls	r2, r6, #27
	itt	mi
	ldrmi	r3, [r8, #20]
	strmi	r3, [r4, #20]
	ldr	r3, [r4, #20]
	cmp	r3, #3
	bls	.L375
	movw	r1, #802
	ldr	r0, .L402+16
	bl	printk
	mvn	r5, #5
.L375:
	lsls	r3, r6, #26
	itt	mi
	ldrmi	r3, [r8, #24]
	strmi	r3, [r4, #24]
	ldr	r3, [r4, #24]
	cmp	r3, #8
	bls	.L377
	movw	r1, #810
	ldr	r0, .L402+20
	bl	printk
	mvn	r5, #5
.L377:
	ldr	r3, [r4]
	orrs	r6, r6, r3
	str	r6, [r4]
	cbz	r5, .L364
	movw	r2, #817
	ldr	r1, .L402+24
	ldr	r0, .L402+28
	bl	printk
	add	r6, sp, #12
	ldmia	r6!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldm	r6, {r0, r1, r2}
	stm	r4, {r0, r1, r2}
.L364:
	mov	r0, r5
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L380:
	movs	r5, #0
	b	.L367
.L379:
	mov	r5, r6
	b	.L364
.L403:
	.align	2
.L402:
	.word	.LC48
	.word	.LC49
	.word	.LC50
	.word	.LC51
	.word	.LC52
	.word	.LC53
	.word	.LANCHOR0+175
	.word	.LC54
	.fnend
	.size	mpp_enc_proc_tune_cfg, .-mpp_enc_proc_tune_cfg
	.align	1
	.global	mpp_enc_proc_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_proc_cfg, %function
mpp_enc_proc_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L537
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	cmp	r1, r3
	mov	r4, r0
	mov	r7, r1
	mov	r5, r2
	beq	.L405
	bhi	.L406
	subs	r3, r3, #6
	cmp	r1, r3
	beq	.L407
	bhi	.L408
	subs	r3, r3, #8
	cmp	r1, r3
	beq	.L409
	adds	r3, r3, #4
	cmp	r1, r3
	beq	.L410
.L411:
	mov	r2, r5
	mov	r1, r7
	b	.L530
.L408:
	ldr	r3, .L537+4
	cmp	r1, r3
	bcc	.L411
	ldrb	r3, [r0, #416]	@ zero_extendqisi2
	lsls	r6, r3, #31
	bmi	.L427
	ldr	r1, [r0, #404]
	ldr	r0, [r0, #4]
	bl	enc_impl_gen_hdr
	ldr	r0, [r4, #404]
	bl	mpp_packet_get_length
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r4, #416]
	str	r0, [r4, #412]
.L427:
	ldr	r3, .L537+8
	cmp	r7, r3
	bne	.L428
	movw	r1, #893
	ldr	r0, .L537+12
	bl	printk
	movw	r1, #894
	ldr	r0, .L537+16
	bl	printk
	ldr	r3, [r4, #404]
	str	r3, [r5]
.L429:
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r4, #416]
	b	.L528
.L406:
	ldr	r3, .L537+20
	cmp	r1, r3
	beq	.L413
	bhi	.L414
	ldr	r3, .L537+24
	cmp	r1, r3
	beq	.L415
	adds	r3, r3, #2
	cmp	r1, r3
	bne	.L411
	movs	r2, #136
	mov	r1, r5
	addw	r0, r0, #4040
	bl	memcpy
	b	.L528
.L414:
	ldr	r3, .L537+28
	cmp	r1, r3
	beq	.L417
	adds	r3, r3, #253
	cmp	r1, r3
	beq	.L418
	subw	r3, r3, #767
	cmp	r1, r3
	bne	.L411
	movs	r3, #0
	str	r3, [sp]
	movw	r3, #5328
	ldr	r3, [r0, r3]
	str	r3, [sp, #4]
	ldr	r3, [r2]
	cbz	r3, .L435
	mov	r0, sp
	bl	mpp_enc_ref_cfg_init
.L435:
	ldr	r3, [sp, #4]
	cbnz	r3, .L436
	add	r0, sp, #4
	bl	mpp_enc_ref_cfg_init
	movw	r3, #5328
	ldr	r2, [sp, #4]
	str	r2, [r4, r3]
.L436:
	ldr	r1, [r5]
	cmp	r1, #5
	bhi	.L437
	tbh	[pc, r1, lsl #1]
.L439:
	.2byte	(.L442-.L439)/2
	.2byte	(.L441-.L439)/2
	.2byte	(.L441-.L439)/2
	.2byte	(.L441-.L439)/2
	.2byte	(.L440-.L439)/2
	.2byte	(.L438-.L439)/2
	.p2align 1
.L409:
	ldr	r6, [r2]
	mov	r1, #836
	ldr	r0, .L537+32
	bl	printk
	cbz	r6, .L420
	lsls	r3, r6, #31
	itt	mi
	ldrmi	r3, [r5, #4]
	strmi	r3, [r4, #604]
	movs	r3, #0
	str	r3, [r5]
.L420:
	ldr	r3, [r5, #104]
	cbz	r3, .L422
	add	r1, r5, #104
	add	r0, r4, #704
	bl	mpp_enc_proc_rc_cfg
	movs	r3, #0
	str	r3, [r5, #104]
.L422:
	ldr	r3, [r5, #304]
	cbz	r3, .L423
	add	r1, r5, #304
	add	r0, r4, #904
	bl	mpp_enc_proc_hw_cfg
	movs	r3, #0
	str	r3, [r5, #304]
.L423:
	movw	r6, #4732
	adds	r1, r5, r6
	ldr	r3, [r5, r6]
	cbz	r3, .L424
	add	r0, r4, #5312
	adds	r0, r0, #20
	bl	mpp_enc_proc_tune_cfg
	movs	r3, #0
	str	r3, [r5, r6]
.L424:
	ldr	r1, .L537+36
	mov	r2, r5
.L530:
	ldr	r0, [r4, #4]
	bl	enc_impl_proc_cfg
	b	.L531
.L410:
	cbnz	r2, .L426
.L528:
	movs	r6, #0
	b	.L425
.L426:
	mov	r1, r2
	add	r0, r0, #704
	bl	mpp_enc_proc_rc_cfg
.L531:
	mov	r6, r0
	b	.L425
.L407:
	ldr	r3, [r0, #468]
	adds	r3, r3, #1
	str	r3, [r0, #468]
	b	.L528
.L428:
	ldr	r1, [r4, #404]
	mov	r0, r5
	bl	mpp_packet_copy
	b	.L429
.L415:
	mov	r1, #904
	ldr	r0, .L537+40
	bl	printk
	b	.L528
.L413:
	cbz	r2, .L430
	ldr	r3, [r2]
	cmp	r3, #1
	bhi	.L431
	ldr	r2, .L537+44
	str	r3, [r0, #420]
	ldr	r2, [r2]
	lsls	r0, r2, #30
	bpl	.L528
	ldr	r1, .L537+48
	mov	r2, #912
	ldr	r0, .L537+52
.L535:
	bl	printk
	b	.L528
.L431:
	ldr	r1, .L537+48
	movw	r2, #914
	ldr	r0, .L537+56
.L532:
	bl	printk
.L529:
	mov	r6, #-1
.L425:
	ldr	r3, [r4, #1192]
	bic	r8, r7, #4
	sub	r2, r3, #8
	cmp	r2, #1
	bls	.L453
	ldr	r2, .L537+60
	cmp	r8, r2
	beq	.L475
	adds	r2, r2, #6
	cmp	r7, r2
	beq	.L475
	subs	r2, r2, #4
	cmp	r7, r2
	bne	.L455
	ldr	r3, [r5]
	tst	r3, #225
	beq	.L456
	movs	r3, #2
	b	.L454
.L430:
	movw	r2, #918
.L536:
	ldr	r1, .L537+48
	ldr	r0, .L537+64
.L534:
	bl	printk
	b	.L529
.L405:
	cbz	r2, .L433
	ldr	r3, [r2]
	cmp	r3, #2
	bhi	.L434
	ldr	r2, .L537+44
	str	r3, [r0, #424]
	ldr	r2, [r2]
	lsls	r1, r2, #30
	bpl	.L528
	mov	r2, #928
	ldr	r1, .L537+48
	ldr	r0, .L537+68
	b	.L535
.L434:
	movw	r2, #930
	ldr	r1, .L537+48
	ldr	r0, .L537+72
	b	.L532
.L433:
	movw	r2, #934
	b	.L536
.L442:
	bl	mpp_enc_ref_default
	str	r0, [sp]
.L443:
	ldr	r1, [sp]
	cmp	r1, #0
	beq	.L472
	ldr	r0, [sp, #4]
	bl	mpp_enc_ref_cfg_copy
	mov	r3, r0
	cbz	r0, .L445
	movw	r2, #681
	ldr	r1, .L537+76
	ldr	r0, .L537+80
	bl	printk
.L445:
	ldr	r1, [sp, #4]
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_set_cfg
	mov	r6, r0
	cbz	r0, .L446
	mov	r3, r0
	movw	r2, #685
	ldr	r1, .L537+76
	ldr	r0, .L537+84
	bl	printk
.L446:
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_update_hdr
	cbz	r0, .L447
	movs	r3, #0
	str	r3, [r4, #416]
.L447:
	ldr	r3, [r5]
	cbz	r3, .L444
	ldr	r3, [sp]
	cbz	r3, .L444
	mov	r0, sp
	bl	mpp_enc_ref_cfg_deinit
.L444:
	add	r3, r4, #5280
	add	r1, r5, #24
	adds	r3, r3, #24
	mov	r2, r5
.L448:
	ldr	r0, [r2], #4	@ unaligned
	cmp	r2, r1
	str	r0, [r3], #4	@ unaligned
	bne	.L448
	b	.L425
.L538:
	.align	2
.L537:
	.word	3276815
	.word	3276813
	.word	3276814
	.word	.LC56
	.word	.LC57
	.word	3309569
	.word	3276817
	.word	3310595
	.word	.LC55
	.word	3276801
	.word	.LC58
	.word	mpp_enc_debug
	.word	.LANCHOR0+197
	.word	.LC59
	.word	.LC60
	.word	3276803
	.word	.LC61
	.word	.LC62
	.word	.LC63
	.word	.LANCHOR0+214
	.word	.LC65
	.word	.LC66
.L441:
	ldr	r0, [sp]
	bl	mpi_enc_gen_ref_cfg
	b	.L443
.L440:
	mov	r1, r5
	ldr	r0, [sp]
	bl	mpi_enc_gen_smart_gop_ref_cfg
	b	.L443
.L438:
	mov	r1, r5
	ldr	r0, [sp]
	bl	mpi_enc_gen_hir_skip_ref
	b	.L443
.L437:
	movw	r1, #675
	ldr	r0, .L539
	bl	printk
	b	.L443
.L472:
	mov	r6, r1
	b	.L444
.L417:
	add	r6, r0, #4160
	adds	r6, r6, #16
	mov	r0, r6
	bl	mpp_enc_unref_osd_buf
	mov	r2, #1128
	mov	r1, r5
	mov	r0, r6
	bl	memcpy
	mov	r0, r6
	bl	mpp_enc_proc_export_osd_buf
	b	.L528
.L418:
	movw	r3, #6674
	ldrb	r3, [r0, r3]	@ zero_extendqisi2
	cbnz	r3, .L449
	mov	r1, #624
	ldr	r0, .L539+4
	bl	printk
	b	.L529
.L449:
	ldr	r6, [r2]
	movw	r3, #6673
	ldrb	r8, [r0, r3]	@ zero_extendqisi2
	cmp	r6, #1024
	and	r8, r8, #3
	bls	.L450
	mov	r2, r6
	movw	r1, #631
	ldr	r0, .L539+8
	b	.L534
.L450:
	mov	r3, sp
	bic	r3, r3, #8160
	add	r9, r0, r8, lsl #10
	bic	r3, r3, #31
	add	r9, r9, #6656
	ldr	r1, [r2, #4]
	ldr	r3, [r3, #8]
	add	r9, r9, #23
	.syntax unified
@ 157 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/include/linux/uaccess.h" 1
	.syntax unified
adds r2, r1, r6; sbcscc r2, r2, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r2, r6
	cbnz	r3, .L451
	mov	r0, r9
	bl	arm_copy_from_user
	mov	r2, r0
.L451:
	cmp	r2, #0
	beq	.L452
	subs	r0, r6, r2
	movs	r1, #0
	add	r0, r0, r9
	mvn	r6, #13
	bl	memset
	b	.L425
.L455:
	ldr	r2, .L539+12
	cmp	r7, r2
	bne	.L457
	ldr	r2, [r4, #608]
	tst	r2, #21
	bne	.L477
	ldr	r2, [r4, #704]
	tst	r2, #225
	bne	.L478
	cmp	r3, #7
	beq	.L459
	ldr	r2, .L539+16
	cmp	r3, r2
	beq	.L459
.L453:
	ldr	r3, .L539+20
	cmp	r7, r3
	beq	.L456
	ldr	r3, .L539+24
	cmp	r7, r3
	beq	.L456
.L457:
	ldr	r3, .L539+28
	cmp	r7, r3
	beq	.L456
	sub	r3, r3, #33280
	cmp	r7, r3
	bne	.L526
.L461:
	ldr	r3, [r4, #608]
	tst	r3, #5
	beq	.L524
.L456:
	movs	r3, #1
	str	r3, [r4, #452]
	ldr	r3, .L539+20
	cmp	r7, r3
	beq	.L463
	ldr	r3, .L539+12
	cmp	r7, r3
	beq	.L463
.L466:
	ldr	r3, .L539+24
	cmp	r8, r3
	bne	.L527
.L469:
	movs	r3, #0
	str	r3, [r4, #56]
.L404:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L459:
	ldr	r3, [r4, #1196]
	cmp	r3, #0
	beq	.L461
	movs	r3, #5
	b	.L454
.L475:
	movs	r3, #1
.L454:
	ldr	r2, .L539+32
	mov	r1, #270
	ldr	r0, .L539+36
	add	r3, r2, r3, lsl #2
	ldr	r2, [r3, #236]
	bl	printk
	ldr	r3, [r4, #464]
	orr	r3, r3, #1
	str	r3, [r4, #464]
	movs	r3, #0
	str	r3, [r4, #416]
	b	.L453
.L477:
	movs	r3, #3
	b	.L454
.L478:
	movs	r3, #4
	b	.L454
.L524:
	ldr	r3, [r4, #704]
	bics	r3, r3, #2
	bne	.L456
	movw	r3, #5332
	ldr	r3, [r4, r3]
	lsls	r2, r3, #29
	bne	.L456
.L467:
	ldr	r3, .L539+12
	cmp	r8, r3
	bne	.L466
	b	.L469
.L463:
	ldr	r3, [r4, #704]
	lsls	r3, r3, #24
	bpl	.L467
	ldr	r1, [r4, #752]
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_set_rc_igop
	b	.L467
.L527:
	ldr	r3, .L539+28
	cmp	r7, r3
.L533:
	bne	.L404
	b	.L469
.L452:
	ldr	r0, [r5]
	add	r1, r4, r8
	movw	r3, #6675
	add	r8, r8, #1
	strb	r0, [r1, r3]
	movw	r3, #6673
	movw	r1, #6674
	strb	r8, [r4, r3]
	ldrb	r3, [r4, r1]	@ zero_extendqisi2
	subs	r3, r3, #1
	strb	r3, [r4, r1]
	b	.L528
.L526:
	ldr	r3, .L539+24
	cmp	r8, r3
	b	.L533
.L540:
	.align	2
.L539:
	.word	.LC64
	.word	.LC67
	.word	.LC68
	.word	3276801
	.word	16777220
	.word	3276805
	.word	3276803
	.word	3310081
	.word	.LANCHOR0
	.word	.LC69
	.fnend
	.size	mpp_enc_proc_cfg, .-mpp_enc_proc_cfg
	.align	1
	.global	mpp_enc_proc_rc_update
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_proc_rc_update, %function
mpp_enc_proc_rc_update:
	.fnstart
	@ args = 0, pretend = 0, frame = 280
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	ldr	r5, [r0, #452]
	.pad #324
	sub	sp, sp, #324
	cmp	r5, #0
	beq	.L541
	ldr	r6, .L602
	ldr	r3, [r6]
	lsls	r1, r3, #26
	bpl	.L543
	movw	r2, #1170
	ldr	r1, .L602+4
	ldr	r0, .L602+8
	bl	printk
.L543:
	mov	r2, #276
	movs	r1, #0
	add	r0, sp, #44
	bl	memset
	ldrd	r9, r8, [r4, #732]
	movw	r3, #5328
	ldr	r7, [r4, r3]
	cmp	r8, #0
	beq	.L575
	mov	r1, r8
	mov	r0, r9
	bl	__aeabi_idiv
	mov	r1, r0
	cbz	r0, .L576
.L544:
	ldr	r0, [r4, #752]
	bl	__aeabi_idiv
	lsls	r5, r0, #2
	cmp	r5, #8
	it	ge
	movge	r5, #8
.L545:
	ldr	r3, [r4, #612]
	str	r3, [sp, #44]
	ldr	r3, [r4, #616]
	str	r3, [sp, #48]
	ldr	r3, [r4, #708]
	cmp	r3, #4
	bhi	.L546
	tbh	[pc, r3, lsl #1]
.L548:
	.2byte	(.L551-.L548)/2
	.2byte	(.L550-.L548)/2
	.2byte	(.L549-.L548)/2
	.2byte	(.L546-.L548)/2
	.2byte	(.L547-.L548)/2
	.p2align 1
.L575:
	movs	r1, #1
	b	.L544
.L576:
	movs	r5, #8
	b	.L545
.L603:
	.align	2
.L602:
	.word	mpp_enc_debug
	.word	.LANCHOR0+260
	.word	.LC72
.L550:
	movs	r3, #1
.L599:
	str	r3, [sp, #52]
.L551:
	ldr	r3, [r4, #728]
	add	ip, sp, #276
	ldr	lr, [r4, #744]
	str	r8, [sp, #64]
	strd	r3, r9, [sp, #56]
	add	r9, r4, #904
	ldr	r3, [r4, #740]
	str	lr, [sp, #72]
	str	r3, [sp, #68]
	ldr	r3, [r4, #748]
	str	r3, [sp, #76]
	ldr	r3, [r4, #752]
	str	r3, [sp, #84]
	ldr	r3, [r4, #804]
	str	r3, [sp, #108]
	ldr	r3, [r4, #808]
	str	r3, [sp, #112]
	ldr	r3, [r4, #812]
	str	r3, [sp, #116]
	ldr	r3, [r4, #852]
	str	r3, [sp, #164]
	ldr	r3, [r4, #848]
	str	r3, [sp, #168]
	ldr	r3, [r4, #860]
	str	r3, [sp, #172]
	ldr	r3, [r4, #856]
	str	r3, [sp, #176]
	ldr	r3, [r4, #864]
	str	r3, [sp, #180]
	ldr	r3, [r4, #716]
	str	r3, [sp, #96]
	ldr	r3, [r4, #720]
	str	r3, [sp, #100]
	ldr	r3, [r4, #724]
	str	r3, [sp, #92]
	movw	r3, #5336
	ldr	r3, [r4, r3]
	str	r3, [sp, #296]
	movw	r3, #5340
	ldr	r3, [r4, r3]
	str	r3, [sp, #300]
	add	r3, r4, #5344
	ldr	r3, [r3]
	str	r3, [sp, #304]
	movw	r3, #5348
	ldr	r3, [r4, r3]
	str	r3, [sp, #308]
	movw	r3, #5352
	ldr	r3, [r4, r3]
	str	r3, [sp, #312]
	movw	r3, #5356
	ldr	r3, [r4, r3]
	str	r3, [sp, #316]
	ldr	r3, [r4, #868]
	str	r3, [sp, #256]
	add	r3, r4, #888
	mov	r8, r3
.L552:
	ldr	r0, [r3]	@ unaligned
	adds	r3, r3, #8
	ldr	r1, [r3, #-4]	@ unaligned
	cmp	r3, r9
	mov	r2, ip
	stmia	r2!, {r0, r1}
	mov	ip, r2
	bne	.L552
	add	r3, r4, #872
	add	ip, sp, #260
.L553:
	ldr	r0, [r3]	@ unaligned
	adds	r3, r3, #8
	ldr	r1, [r3, #-4]	@ unaligned
	cmp	r3, r8
	mov	r2, ip
	stmia	r2!, {r0, r1}
	mov	ip, r2
	bne	.L553
	cmp	lr, #0
	bne	.L554
	mov	r1, #1088
	ldr	r3, .L604
	str	r1, [sp]
	ldr	r2, .L604+4
	ldr	r0, .L604+8
	bl	printk
.L554:
	ldr	r3, [r4, #764]
	cbz	r3, .L600
	cmp	r3, #60
	it	ge
	movge	r3, #60
	mov	r5, r3
.L600:
	cmp	r5, #1
	ldr	r3, [r4, #1192]
	it	lt
	movlt	r5, #1
	cmp	r3, #8
	str	r5, [sp, #104]
	beq	.L556
	bhi	.L557
	cmp	r3, #7
.L601:
	bne	.L559
.L558:
	ldr	r3, [r4, #816]
	ldr	r1, [r4, #820]
	ldr	r2, [r4, #828]
	str	r3, [sp, #136]
	ldr	r3, [r4, #824]
	str	r1, [sp, #140]
	cmp	r3, #0
	it	eq
	moveq	r3, r1
	str	r3, [sp, #148]
	ldr	r3, [r4, #832]
	str	r2, [sp, #144]
	cmp	r3, #0
	it	eq
	moveq	r3, r2
	str	r3, [sp, #152]
	ldr	r3, [r4, #840]
	str	r3, [sp, #156]
	ldr	r3, [r4, #844]
	str	r3, [sp, #160]
.L562:
	movs	r3, #0
	mov	r2, #256
	strd	r3, r3, [sp, #128]
	strd	r2, r3, [sp, #120]
	ldr	r3, [r4, #760]
	str	r3, [sp, #200]
	ldr	r3, [r4, #768]
	str	r3, [sp, #220]
	ldr	r3, [r4, #772]
	str	r3, [sp, #224]
	ldr	r3, [r4, #776]
	str	r3, [sp, #228]
	ldr	r3, [r4, #792]
	str	r3, [sp, #244]
	ldr	r3, [r4, #780]
	str	r3, [sp, #232]
	ldr	r3, [r4, #784]
	str	r3, [sp, #236]
	ldr	r3, [r4, #788]
	str	r3, [sp, #240]
	ldr	r3, [r4, #796]
	str	r3, [sp, #248]
	ldr	r3, [r4, #800]
	str	r3, [sp, #252]
	ldr	r5, [r7, #64]
	cbz	r5, .L564
	ldrd	r0, r1, [r4, #744]
	str	r5, [sp, #88]
	bl	__aeabi_idiv
	cmp	r5, r0
	blt	.L564
	ldr	r3, [sp, #84]
	cmp	r5, r3
	bge	.L564
	movs	r3, #1
	str	r3, [sp, #80]
	ldr	r3, [sp, #160]
	cbnz	r3, .L564
	movs	r3, #2
	str	r3, [sp, #160]
.L564:
	ldr	r3, [r4, #1192]
	ldr	r7, .L604+12
	cmp	r3, #7
	beq	.L566
	ldr	r2, .L604+16
	cmp	r3, r2
	bne	.L567
.L566:
	ldr	r2, .L604+20
	ldr	r3, .L604+24
	ldr	r0, [sp, #68]
	ldr	r1, [sp, #56]
	cmp	r1, #0
	ite	ne
	movne	r1, r3
	moveq	r1, r2
	cmp	r0, #0
	it	eq
	moveq	r3, r2
	ldr	r0, [sp, #88]
	ldr	r2, [sp, #52]
	str	r0, [sp, #36]
	ldr	r0, [sp, #84]
	add	r2, r7, r2, lsl #2
	ldr	r2, [r2, #308]
	str	r0, [sp, #32]
	ldr	r0, [sp, #76]
	str	r0, [sp, #28]
	ldr	r0, [sp, #72]
	strd	r3, r0, [sp, #20]
	ldr	r3, [sp, #64]
	ldr	r0, .L604+28
	str	r3, [sp, #16]
	ldr	r3, [sp, #60]
	strd	r1, r3, [sp, #8]
	movw	r1, #1155
	ldr	r3, [r4, #720]
	str	r3, [sp, #4]
	ldr	r3, [r4, #716]
	str	r3, [sp]
	ldr	r3, [r4, #724]
	bl	printk
.L567:
	movw	r3, #5380
	ldr	r3, [r4, r3]
	cbnz	r3, .L570
	movw	r3, #5384
	ldr	r3, [r4, r3]
	cbz	r3, .L571
.L570:
	movs	r3, #1
	str	r3, [sp, #292]
.L571:
	add	r1, sp, #44
	ldr	r0, [r4, #60]
	bl	rc_update_usr_cfg
	movs	r3, #0
	str	r3, [r4, #704]
	str	r3, [r4, #608]
	ldr	r3, [r6]
	lsls	r3, r3, #26
	mov	r5, r0
	bpl	.L572
	movw	r2, #1182
	ldr	r1, .L604+32
	ldr	r0, .L604+36
	bl	printk
.L572:
	movs	r3, #0
	str	r3, [r4, #452]
	ldr	r3, [r4, #440]
	mov	r0, r4
	ldr	r2, [sp, #80]
	ldr	r1, .L604+40
	str	r3, [r4, #444]
	movw	r3, #5360
	str	r2, [r4, r3]
	ldr	r3, [sp, #156]
	ldr	r2, [sp, #52]
	str	r3, [sp, #32]
	ldr	r3, [sp, #148]
	add	r2, r7, r2, lsl #2
	ldr	r2, [r2, #308]
	str	r3, [sp, #28]
	ldr	r3, [sp, #152]
	str	r3, [sp, #24]
	ldr	r3, [sp, #140]
	str	r3, [sp, #20]
	ldr	r3, [sp, #144]
	str	r3, [sp, #16]
	ldr	r3, [sp, #136]
	str	r3, [sp, #12]
	ldr	r3, [sp, #84]
	str	r3, [sp, #8]
	ldr	r3, [sp, #100]
	str	r3, [sp, #4]
	ldr	r3, [sp, #92]
	str	r3, [sp]
	ldr	r3, [sp, #96]
	bl	update_rc_cfg_log.constprop.1
.L541:
	mov	r0, r5
	add	sp, sp, #324
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L546:
	movs	r3, #3
	b	.L599
.L549:
	movs	r3, #2
	b	.L599
.L547:
	movs	r3, #4
	b	.L599
.L557:
	cmp	r3, #9
	beq	.L558
	ldr	r2, .L604+16
	cmp	r3, r2
	b	.L601
.L556:
	ldr	r3, [r4, #1204]
	ldr	r2, [r4, #1208]
	str	r3, [sp, #136]
	ldr	r3, [r4, #1212]
	str	r2, [sp, #140]
	strd	r3, r2, [sp, #144]
	str	r3, [sp, #152]
	b	.L562
.L559:
	movw	r2, #1115
	ldr	r1, .L604+44
	ldr	r0, .L604+48
	bl	printk
	b	.L562
.L605:
	.align	2
.L604:
	.word	.LANCHOR0+283
	.word	.LC73
	.word	.LC74
	.word	.LANCHOR0
	.word	16777220
	.word	.LC71
	.word	.LC70
	.word	.LC76
	.word	.LANCHOR0+260
	.word	.LC77
	.word	.LC18
	.word	.LANCHOR0+294
	.word	.LC75
	.fnend
	.size	mpp_enc_proc_rc_update, .-mpp_enc_proc_rc_update
	.align	1
	.global	mpp_enc_alloc_output_from_bufpool
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_alloc_output_from_bufpool, %function
mpp_enc_alloc_output_from_bufpool:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #56
	sub	sp, sp, #56
	mov	r5, r0
	bl	mpp_enc_check_frm_pkt
	mov	r6, r0
	cmp	r0, #0
	bne	.L606
	mov	r1, r0
	ldr	r0, [r5, #372]
	bl	mpp_packet_set_length
	ldr	r8, [r5, #396]
	ldr	r7, .L640
	cmp	r8, #0
	bne	.L608
	ldr	r4, [r5, #612]
	ldr	r8, [r5, #372]
	adds	r4, r4, #15
	bic	r3, r4, #15
	ldr	r4, [r5, #616]
	adds	r4, r4, #15
	bic	r4, r4, #15
	muls	r4, r3, r4
	ldr	r3, [r5]
	cmp	r3, #8
	mov	r3, #0
	str	r3, [sp, #20]
	itt	eq
	addeq	r4, r4, r4, lsl #1
	lsreq	r4, r4, #1
	bl	get_mpibuf_ops
	mov	r9, r0
	cbnz	r4, .L610
	movw	r1, #1269
	ldr	r3, .L640+4
	str	r1, [sp]
	ldr	r2, .L640+8
	ldr	r0, .L640+12
	bl	printk
.L610:
	cmp	r9, #0
	beq	.L612
	movw	r10, #5404
	ldr	r3, [r5, r10]
	cbnz	r3, .L613
	ldr	r3, [r9, #56]
	cbz	r3, .L613
	movs	r1, #2
	mov	r0, r4
	blx	r3
	str	r0, [r5, r10]
.L613:
	movw	r3, #5404
	ldr	r0, [r5, r3]
	cbz	r0, .L612
	ldr	r3, [r9, #64]
	cbz	r3, .L612
	blx	r3
	movs	r2, #32
	movs	r1, #0
	mov	r10, r0
	add	r0, sp, #24
	bl	memset
	cmp	r10, #0
	beq	.L612
	ldr	r3, .L640+4
	add	r2, sp, #20
	add	r1, sp, #24
	movs	r0, #0
	str	r10, [sp, #36]
	str	r3, [sp]
	ldr	r3, .L640+16
	str	r4, [sp, #28]
	bl	mpp_buffer_import_with_tag
	ldr	r3, [r9, #16]
	cbz	r3, .L612
	mov	r0, r10
	blx	r3
.L612:
	ldr	r0, [sp, #20]
	cbnz	r0, .L619
	ldr	r9, .L640+4
	mov	r2, r4
	ldr	r3, .L640+16
	add	r1, sp, #20
	str	r9, [sp]
	bl	mpp_buffer_get_with_tag
	ldr	r3, [sp, #20]
	cbnz	r3, .L619
	movw	r1, #1293
	mov	r3, r9
	str	r1, [sp]
	ldr	r2, .L640+20
	ldr	r0, .L640+12
	bl	printk
.L619:
	ldr	r0, [sp, #20]
	add	r3, r8, #72
	ldr	r1, .L640+4
	movs	r4, #0
	str	r3, [r5, #396]
	str	r0, [r8, #72]
	bl	mpp_buffer_get_mpi_buf_id_with_caller
	ldr	r1, .L640+4
	str	r4, [r8, #84]
	str	r0, [r8, #76]
	ldr	r0, [sp, #20]
	bl	mpp_buffer_get_size_with_caller
	ldr	r1, .L640+4
	str	r0, [r8, #96]
	ldr	r0, [sp, #20]
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L640+4
	strd	r0, r0, [r8, #24]
	ldr	r0, [sp, #20]
	bl	mpp_buffer_get_size_with_caller
	ldr	r2, [r7]
	lsls	r2, r2, #26
	ldr	r3, [sp, #20]
	strd	r0, r4, [r8, #32]
	str	r3, [r8, #68]
	bpl	.L606
	str	r3, [sp]
	movw	r2, #1307
	ldr	r3, [r5, #372]
	ldr	r1, .L640+24
	ldr	r0, .L640+28
	bl	printk
.L606:
	mov	r0, r6
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L608:
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L606
	ldr	r4, [r5, #372]
	mov	r0, r4
	bl	mpp_packet_get_pos
	mov	r7, r0
	ldr	r0, [r5, #372]
	bl	mpp_packet_get_length
	mov	r3, r4
	mov	r2, #1312
	ldr	r1, .L640+24
	strd	r7, r0, [sp, #4]
	str	r8, [sp]
	ldr	r0, .L640+32
	bl	printk
	b	.L606
.L641:
	.align	2
.L640:
	.word	mpp_enc_debug
	.word	.LANCHOR0+328
	.word	.LC78
	.word	.LC74
	.word	.LC44
	.word	.LC79
	.word	.LANCHOR0+362
	.word	.LC80
	.word	.LC81
	.fnend
	.size	mpp_enc_alloc_output_from_bufpool, .-mpp_enc_alloc_output_from_bufpool
	.align	1
	.global	mpp_enc_alloc_output_from_ringbuf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_alloc_output_from_ringbuf, %function
mpp_enc_alloc_output_from_ringbuf:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	movs	r3, #0
	ldr	r5, [r0, #612]
	mov	r4, r0
	str	r3, [sp, #12]
	adds	r5, r5, #15
	bic	r3, r5, #15
	ldr	r5, [r0, #616]
	adds	r5, r5, #15
	bic	r5, r5, #15
	muls	r5, r3, r5
	ldr	r3, [r0]
	ldrd	r6, r0, [r0, #456]
	cmp	r3, #8
	it	ne
	lsrne	r5, r5, #1
	bl	mpp_enc_refs_next_frm_is_intra
	cmp	r0, #0
	bne	.L652
	ldr	r6, [r6, #4]
	cmp	r6, #1
	beq	.L644
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_next_frm_is_kpfrm
	adds	r6, r0, #0
	it	ne
	movne	r6, #1
.L644:
	movw	r3, #10776
	ldr	r3, [r4, r3]
	cmp	r3, #0
	beq	.L645
	ldr	r3, [r3, #28]
	cmp	r3, #0
	bne	.L645
	bl	get_vsm_ops
	cmp	r0, #0
	bne	.L645
	movw	r3, #10780
	ldr	r2, [r4, r3]
	cbnz	r2, .L646
	str	r5, [r4, r3]
.L646:
	ldr	r2, [r4, r3]
	cmp	r2, #16384
	it	cc
	movcc	r2, #16384
	addw	r2, r2, #4095
	bic	r2, r2, #4080
	bic	r2, r2, #15
	str	r2, [r4, r3]
	movw	r3, #10808
	ldr	r3, [r4, r3]
	ldr	r0, [r3, #12]
	cbz	r0, .L647
	str	r0, [sp, #12]
.L648:
	movw	r3, #10784
	ldr	r1, [sp, #12]
	ldr	r2, [r4, r3]
	movw	r3, #10776
	ldr	r0, [r4, r3]
	bl	ring_buf_init
	cbz	r0, .L645
	movw	r3, #10780
	movw	r2, #1357
	ldr	r3, [r4, r3]
	ldr	r1, .L665
	ldr	r0, .L665+4
	bl	printk
	b	.L664
.L652:
	movs	r6, #1
	b	.L644
.L647:
	ldr	r3, .L665+8
	add	r1, sp, #12
	str	r3, [sp]
	ldr	r3, .L665+12
	bl	mpp_ring_buffer_get_with_tag
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne	.L648
	movw	r1, #1351
	ldr	r0, .L665+16
	bl	printk
.L664:
	mov	r0, #-1
.L642:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L645:
	movw	r3, #10828
	movw	r1, #10776
	ldr	r3, [r4, r3]
	movs	r2, #0
	ldr	r1, [r4, r1]
	add	r0, r4, #372
	str	r3, [sp]
	mov	r3, r6
	bl	mpp_packet_new_ring_buf
	cbz	r0, .L650
	adds	r3, r0, #3
	ite	eq
	movweq	r2, #10788
	movwne	r2, #10792
	ldr	r3, [r4, r2]
	adds	r3, r3, #1
	str	r3, [r4, r2]
	b	.L642
.L650:
	ldr	r3, [r4, #372]
	mov	r0, r4
	adds	r3, r3, #72
	str	r3, [r4, #396]
	bl	mpp_enc_check_frm_pkt
	b	.L642
.L666:
	.align	2
.L665:
	.word	.LANCHOR0+430
	.word	.LC83
	.word	.LANCHOR0+396
	.word	.LC44
	.word	.LC82
	.fnend
	.size	mpp_enc_alloc_output_from_ringbuf, .-mpp_enc_alloc_output_from_ringbuf
	.align	1
	.global	mpp_enc_impl_alloc_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_impl_alloc_task, %function
mpp_enc_impl_alloc_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r1, #124
	mov	r4, r0
	ldr	r0, .L670
	bl	mpp_osal_calloc
	str	r0, [r4, #456]
	cbz	r0, .L669
	bl	mpp_time
	strd	r0, [r4, #40]
	bl	mpp_time
	strd	r0, [r4, #24]
	bl	mpp_time
	strd	r0, [r4, #32]
	movs	r0, #0
.L667:
	pop	{r4, pc}
.L669:
	mov	r0, #-1
	b	.L667
.L671:
	.align	2
.L670:
	.word	.LANCHOR0+464
	.fnend
	.size	mpp_enc_impl_alloc_task, .-mpp_enc_impl_alloc_task
	.align	1
	.global	mpp_enc_impl_free_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_impl_free_task, %function
mpp_enc_impl_free_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r1, [r0, #456]
	cbz	r1, .L673
	ldr	r0, .L677
	bl	mpp_osal_free
	movs	r3, #0
	str	r3, [r4, #456]
.L673:
	movs	r0, #0
	pop	{r4, pc}
.L678:
	.align	2
.L677:
	.word	.LANCHOR0+488
	.fnend
	.size	mpp_enc_impl_free_task, .-mpp_enc_impl_free_task
	.align	1
	.global	mpp_enc_impl_get_roi_osd
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_impl_get_roi_osd, %function
mpp_enc_impl_get_roi_osd:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r3, [r0, #4040]
	mov	r6, r1
	cbz	r3, .L680
	addw	r1, r0, #4040
	movs	r2, #136
	add	r0, r0, #5408
	bl	memcpy
	movs	r3, #0
	str	r3, [r4, #4040]
.L680:
	movw	r7, #4176
	add	r8, r4, r7
	ldr	r3, [r4, r7]
	cbz	r3, .L681
	add	r5, r4, #5536
	adds	r5, r5, #8
	mov	r0, r5
	bl	mpp_enc_unref_osd_buf
	mov	r2, #1128
	mov	r1, r8
	mov	r0, r5
	bl	memcpy
	movs	r3, #0
	str	r3, [r4, r7]
.L681:
	cbz	r6, .L683
	add	r1, r4, #5408
	ldr	r3, [r1]
	cbz	r3, .L684
	mov	r0, r6
	bl	mpp_frame_add_roi
.L684:
	movw	r3, #5544
	adds	r1, r4, r3
	ldr	r3, [r4, r3]
	cbz	r3, .L683
	mov	r0, r6
	bl	mpp_frame_add_osd
.L683:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
	.fnend
	.size	mpp_enc_impl_get_roi_osd, .-mpp_enc_impl_get_roi_osd
	.align	1
	.global	mpp_enc_impl_reg_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_impl_reg_cfg, %function
mpp_enc_impl_reg_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r6, [r0, #456]
	.pad #52
	sub	sp, sp, #52
	mov	r8, r1
	add	r7, r0, #64
	add	r5, r6, #20
	ldrb	r3, [r6, #11]	@ zero_extendqisi2
	ands	r3, r3, #1
	beq	.L699
	ldr	r6, .L876
	ldr	r8, [r0, #8]
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L700
	movw	r2, #1629
	ldr	r1, .L876+4
	ldr	r0, .L876+8
	bl	printk
.L700:
	ldr	r3, [r4, #224]
	movs	r1, #0
	movs	r2, #104
	add	r0, r4, #224
	ldrd	fp, r10, [r4, #228]
	str	r3, [sp, #36]
	ldrd	r9, r3, [r4, #236]
	str	r3, [sp, #28]
	ldr	r3, [r4, #244]
	str	r3, [sp, #32]
	bl	memset
	strd	r10, r9, [r4, #232]
	ldr	r3, [sp, #36]
	strd	r3, fp, [r4, #224]
	ldr	r3, [sp, #28]
	str	r3, [r4, #240]
	ldr	r3, [sp, #32]
	str	r3, [r4, #244]
	ldr	r3, [r6]
	lsls	r1, r3, #26
	bpl	.L701
	ldrh	r3, [r4, #222]
	mov	r2, #1632
	ldr	r1, .L876+4
	ldr	r0, .L876+12
	bl	printk
.L701:
	mov	r1, r5
	ldr	r0, [r4, #4]
	bl	enc_impl_proc_hal
	mov	r9, r0
	cbz	r0, .L702
	str	r0, [sp]
	movw	r3, #1633
	ldr	r0, .L876+16
	mov	r2, r4
	mov	r1, r3
.L875:
	bl	printk
	mov	r9, #0
	b	.L698
.L702:
	ldr	r3, [r6]
	lsls	r2, r3, #26
	bpl	.L704
	ldrh	r3, [r4, #222]
	movw	r2, #1635
	ldr	r1, .L876+4
	ldr	r0, .L876+20
	bl	printk
.L704:
	mov	r1, r5
	mov	r0, r8
	bl	mpp_enc_hal_get_task
	cbz	r0, .L705
	movw	r3, #1636
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L876+24
	bl	printk
.L698:
	mov	r0, r9
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L705:
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L706
	ldrh	r3, [r4, #222]
	movw	r2, #1638
	ldr	r1, .L876+4
	ldr	r0, .L876+28
	bl	printk
.L706:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_hal_start
	cbz	r0, .L707
	movw	r3, #1639
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L876+32
	b	.L875
.L707:
	ldr	r3, [r6]
	lsls	r7, r3, #26
	bpl	.L708
	ldrh	r3, [r4, #222]
	movw	r2, #1641
	ldr	r1, .L876+4
	ldr	r0, .L876+36
	bl	printk
.L708:
	mov	r1, r5
	mov	r0, r8
	bl	mpp_enc_hal_gen_regs
	mov	r9, r0
	cbz	r0, .L709
	movw	r3, #1642
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L876+40
	b	.L875
.L709:
	ldr	r3, [r6]
	lsls	r5, r3, #26
	bpl	.L710
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	movw	r2, #1645
	ldr	r1, .L876+4
	ldr	r0, .L876+44
	str	r3, [sp, #4]
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	ubfx	r3, r3, #2, #1
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
.L710:
	ldr	r3, [r6]
	lsls	r4, r3, #31
	bpl	.L698
	movw	r2, #1646
	ldr	r1, .L876+4
	ldr	r0, .L876+48
	bl	printk
	b	.L698
.L699:
	strd	r1, r3, [r0, #368]
	cbz	r1, .L765
	ldr	r3, [r0, #4032]
	cbnz	r3, .L712
	mov	r0, r1
	bl	mpp_frame_get_hor_stride
	mov	r10, r0
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_ver_stride
	mov	fp, r0
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_width
	mov	r9, r0
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_height
	ldr	r2, [r4, #656]
	bic	r2, r2, #2
	cmp	r2, #1
	it	eq
	moveq	r2, r9
	mov	r3, r0
	it	eq
	moveq	r9, r0
	ldrd	r0, ip, [r4, #620]
	it	eq
	moveq	r3, r2
	ldrd	r2, r1, [r4, #612]
	cmp	r10, r0
	bne	.L714
	cmp	fp, ip
	bne	.L714
	cmp	r2, r9
	bhi	.L714
	cmp	r1, r3
	bls	.L712
.L714:
	strd	r2, r1, [sp, #8]
	mov	r2, r9
	strd	r0, ip, [sp, #16]
	movw	r1, #1795
	strd	r10, fp, [sp]
	ldr	r0, .L876+52
	bl	printk
.L765:
	mov	r9, #-1
.L711:
	mov	r1, r6
	mov	r0, r4
	bl	mpp_enc_terminate_task
	b	.L698
.L712:
	mov	r0, r8
	bl	mpp_frame_get_idr_request
	cbz	r0, .L715
	ldr	r3, [r4, #464]
	orr	r3, r3, #1
	str	r3, [r4, #464]
	movs	r3, #0
	str	r3, [r4, #416]
.L715:
	movs	r2, #100
	movs	r1, #0
	mov	r0, r5
	add	r10, r4, #464
	bl	memset
	mov	r2, #304
	movs	r1, #0
	mov	r0, r7
	bl	memset
	ldr	r3, [r6, #4]
	adds	r2, r3, #1
	str	r2, [r6, #4]
	strh	r3, [r4, #222]	@ movhi
	movs	r3, #0
	strd	r7, r10, [r6, #24]
	str	r3, [r6, #80]
	ldr	r3, [r4, #368]
	str	r3, [r4, #360]
	ldr	r3, .L876
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #32
	beq	.L716
	ldrh	r3, [r4, #222]
	movw	r2, #1841
	ldr	r1, .L876+56
	ldr	r0, .L876+60
	bl	printk
.L716:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_frm_check_drop
	mov	r9, r0
	cbz	r0, .L717
	str	r0, [sp]
	movw	r3, #1842
	ldr	r0, .L876+64
	mov	r2, r4
	mov	r1, r3
.L873:
	bl	printk
	b	.L711
.L717:
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	orr	r3, r3, #16
	strb	r3, [r6, #8]
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L718
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	movw	r2, #1844
	ldr	r1, .L876+56
	and	r3, r3, #1
	ldr	r0, .L876+68
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
.L718:
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	lsls	r1, r3, #31
	bpl	.L719
	movs	r3, #0
	str	r3, [r6, #20]
	str	r3, [r6, #68]
	b	.L765
.L877:
	.align	2
.L876:
	.word	mpp_enc_debug
	.word	.LANCHOR0+511
	.word	.LC84
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC85
	.word	.LC86
	.word	.LC10
	.word	.LC11
	.word	.LC87
	.word	.LC88
	.word	.LC89
	.word	.LANCHOR0+532
	.word	.LC90
	.word	.LC91
	.word	.LC92
.L719:
	ldr	r3, [r4, #464]
	cbz	r3, .L720
	mov	r1, r10
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_set_usr_cfg
.L720:
	movs	r3, #1
	mov	r0, r4
	str	r3, [r6, #20]
	bl	mpp_enc_alloc_output_from_ringbuf
	mov	r9, r0
	cmp	r0, #0
	bne	.L711
	ldr	r3, [r4, #368]
	str	r3, [r6, #72]
	ldr	r3, [r4, #392]
	str	r3, [r6, #76]
	ldr	r3, [r4, #372]
	str	r3, [r6, #48]
	ldr	r3, [r4, #396]
	str	r3, [r6, #52]
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	orr	r3, r3, #32
	strb	r3, [r6, #8]
	ldr	r3, [r4, #396]
	str	r3, [r6, #52]
	ldr	r3, [r4, #372]
	cbnz	r3, .L721
	movw	r1, #1870
	ldr	r3, .L878
	str	r1, [sp]
	ldr	r2, .L878+4
	ldr	r0, .L878+8
	bl	printk
.L721:
	ldr	r1, [r4, #368]
	mov	r0, r4
	bl	mpp_enc_impl_get_roi_osd
	ldr	r3, [r4, #56]
	cbnz	r3, .L722
	movs	r3, #1
	str	r3, [r4, #56]
.L722:
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bmi	.L723
	ldr	r1, [r4, #404]
	ldr	r0, [r4, #4]
	bl	enc_impl_gen_hdr
	ldr	r0, [r4, #404]
	bl	mpp_packet_get_length
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r4, #416]
	ldr	r3, [r8]
	lsls	r3, r3, #26
	str	r0, [r4, #412]
	bpl	.L724
	str	r0, [sp]
	movw	r2, #1886
	ldrh	r3, [r4, #222]
	ldr	r1, .L878+12
	ldr	r0, .L878+16
	bl	printk
.L724:
	ldr	r1, [r4, #404]
	ldr	r0, [r4, #372]
	bl	mpp_packet_append
	ldr	r3, [r4, #412]
	str	r3, [r6, #56]
	ldr	r3, [r6, #68]
	ldr	r2, [r4, #412]
	add	r3, r3, r2
	str	r3, [r6, #68]
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	orr	r3, r3, #8
	strb	r3, [r4, #416]
	ldr	r3, [r8]
	lsls	r1, r3, #26
	bpl	.L726
	movw	r2, #1893
	ldr	r1, .L878+12
	ldr	r0, .L878+20
	bl	printk
.L723:
	ldr	r3, [r8]
	lsls	r2, r3, #26
	bpl	.L726
	movw	r2, #1895
	ldr	r1, .L878+12
	ldr	r0, .L878+24
	bl	printk
.L726:
	ldr	r1, .L878+28
	mov	r0, r5
	bl	check_hal_task_pkt_len
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L728
	ldrh	r3, [r4, #222]
	movw	r2, #1899
	ldr	r1, .L878+12
	ldr	r0, .L878+32
	bl	printk
.L728:
	mov	r1, r5
	ldr	r0, [r4, #4]
	bl	enc_impl_start
	mov	r9, r0
	cbz	r0, .L729
	movw	r3, #1900
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L878+36
	b	.L873
.L729:
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_stash
	ldrd	r10, r3, [r4, #4]
	ldr	fp, [r6, #48]
	str	r3, [sp, #32]
	movw	r3, #5336
	ldr	r2, [r4, r3]
	movw	r3, #10832
	str	r2, [r4, #280]
	ldr	r1, [r4, r3]
	str	r1, [r4, #284]
	str	r2, [r4, r3]
	movw	r3, #5388
	ldr	r3, [r4, r3]
	cbz	r3, .L730
	movw	r3, #10812
	ldr	r3, [r4, r3]
	str	r3, [r6, #84]
	add	r3, r4, #10816
	ldr	r3, [r3]
	str	r3, [r6, #88]
	movw	r3, #10820
	ldr	r3, [r4, r3]
	str	r3, [r6, #92]
	movw	r3, #10824
	ldr	r3, [r4, r3]
	str	r3, [r6, #96]
.L730:
	movw	r3, #5380
	ldr	r3, [r4, r3]
	cbz	r3, .L731
.L734:
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L733
	ldrh	r3, [r4, #222]
	mov	r2, #1488
	ldr	r1, .L878+40
	ldr	r0, .L878+44
	bl	printk
.L733:
	mov	r1, r7
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_get_cpb
	ldr	r3, [r8]
	lsls	r1, r3, #15
	bpl	.L736
	ldrh	r3, [r4, #78]
	movw	r2, #1492
	ldr	r1, .L878+40
	ldr	r0, .L878+48
	bl	printk
.L736:
	mov	r1, r5
	mov	r0, r10
	bl	enc_impl_proc_dpb
	mov	r9, r0
	cmp	r0, #0
	beq	.L737
	str	r0, [sp]
	movw	r3, #1493
	ldr	r0, .L878+52
	mov	r2, r4
	mov	r1, r3
.L874:
	bl	printk
	movw	r2, #10796
	ldr	r3, [r4, r2]
	adds	r3, r3, #1
	str	r3, [r4, r2]
	b	.L735
.L731:
	ldr	r3, [r4, #796]
	cmp	r3, #0
	beq	.L734
	movw	r3, #5384
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bne	.L734
	ldr	r3, [r4, #464]
	cmp	r3, #0
	bne	.L734
	mov	r1, r6
	mov	r0, r4
	bl	mpp_enc_proc_two_pass
	mov	r9, r0
	cmp	r0, #0
	beq	.L734
.L735:
	movw	r3, #1903
	str	r9, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L878+56
	b	.L873
.L879:
	.align	2
.L878:
	.word	.LANCHOR0+553
	.word	.LC93
	.word	.LC74
	.word	.LANCHOR0+532
	.word	.LC94
	.word	.LC95
	.word	.LC96
	.word	.LC97
	.word	.LC98
	.word	.LC99
	.word	.LANCHOR0+574
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC115
.L737:
	ldr	r3, [r8]
	lsls	r2, r3, #15
	bpl	.L764
	ldrh	r3, [r4, #78]
	movw	r2, #1495
	ldr	r1, .L880
	ldr	r0, .L880+4
	bl	printk
	ldr	r3, [r8]
	lsls	r3, r3, #15
	bpl	.L764
	ldrh	r3, [r4, #78]
	movw	r2, #1497
	ldr	r1, .L880
	ldr	r0, .L880+8
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
	ldr	r3, [r8]
	lsls	r0, r3, #15
	bpl	.L764
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	ubfx	r3, r3, #5, #1
	ldr	r1, .L880
	ubfx	r2, r2, #5, #1
	ldr	r0, .L880+12
	str	r2, [sp]
	movw	r2, #1499
	bl	printk
	ldr	r3, [r8]
	lsls	r1, r3, #15
	bpl	.L764
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	ubfx	r3, r3, #4, #1
	ldr	r1, .L880
	ubfx	r2, r2, #4, #1
	ldr	r0, .L880+16
	str	r2, [sp]
	movw	r2, #1501
	bl	printk
	ldr	r3, [r8]
	lsls	r2, r3, #15
	bpl	.L764
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	ubfx	r3, r3, #6, #1
	ldr	r1, .L880
	ubfx	r2, r2, #6, #1
	ldr	r0, .L880+20
	str	r2, [sp]
	movw	r2, #1503
	bl	printk
	ldr	r3, [r8]
	lsls	r3, r3, #15
	bpl	.L764
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	ldr	r1, .L880
	lsrs	r3, r3, #7
	ldr	r0, .L880+24
	lsrs	r2, r2, #7
	str	r2, [sp]
	movw	r2, #1505
	bl	printk
	ldr	r3, [r8]
	lsls	r0, r3, #15
	bpl	.L764
	ldrb	r3, [r4, #217]	@ zero_extendqisi2
	ldrb	r2, [r4, #73]	@ zero_extendqisi2
	and	r3, r3, #15
	ldr	r1, .L880
	and	r2, r2, #15
	ldr	r0, .L880+28
	str	r2, [sp]
	movw	r2, #1507
	bl	printk
	ldr	r3, [r8]
	lsls	r1, r3, #15
	bpl	.L764
	ldrb	r3, [r4, #217]	@ zero_extendqisi2
	ldrb	r2, [r4, #73]	@ zero_extendqisi2
	ldr	r1, .L880
	lsrs	r3, r3, #4
	ldr	r0, .L880+32
	lsrs	r2, r2, #4
	str	r2, [sp]
	movw	r2, #1509
	bl	printk
	ldr	r3, [r8]
	lsls	r2, r3, #15
	bpl	.L764
	ldrh	r3, [r4, #78]
	movw	r2, #1511
	ldr	r1, .L880
	ldr	r0, .L880+36
	bl	printk
.L764:
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L747
	ldrh	r3, [r4, #222]
	movw	r2, #1513
	ldr	r1, .L880
	ldr	r0, .L880+40
	bl	printk
.L747:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_frm_start
	mov	r9, r0
	cbz	r0, .L748
	movw	r3, #1514
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L880+44
	b	.L874
.L748:
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L749
	ldrh	r3, [r4, #222]
	movw	r2, #1515
	ldr	r1, .L880
	ldr	r0, .L880+48
	bl	printk
.L749:
	ldr	r3, [r4, #420]
	cmp	r3, #1
	bne	.L750
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	lsls	r1, r3, #27
	bpl	.L750
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	tst	r3, #14
	bne	.L750
	ldr	r3, [r8]
	lsls	r2, r3, #26
	bpl	.L751
	ldr	r3, [r4, #412]
	movw	r2, #1522
	ldr	r1, .L880
	ldr	r0, .L880+52
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
.L751:
	ldr	r1, [r4, #404]
	mov	r0, fp
	bl	mpp_packet_append
	ldr	r3, [r4, #412]
	str	r3, [r6, #56]
	ldr	r3, [r6, #68]
	ldr	r2, [r4, #412]
	add	r3, r3, r2
	str	r3, [r6, #68]
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	orr	r3, r3, #4
	strb	r3, [r4, #416]
.L750:
	ldr	r1, .L880+56
	mov	r0, r5
	bl	check_hal_task_pkt_len
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	lsls	r3, r3, #26
	bpl	.L752
	ldr	r3, [r4, #424]
	cbz	r3, .L752
	ldr	r3, [r4, #432]
	add	r2, sp, #44
	mov	r1, fp
	mov	r0, r10
	mov	r9, #0
	str	r9, [sp, #44]
	str	r3, [sp, #4]
	ldr	r3, [r4, #428]
	str	r3, [sp]
	ldr	r3, .L880+60
	bl	enc_impl_add_prefix
	ldr	r2, [r6, #60]
	ldr	r3, [r6, #68]
	mov	r0, r10
	ldr	r1, [sp, #44]
	add	r2, r2, r1
	add	r3, r3, r1
	str	r2, [r6, #60]
	mov	r1, fp
	str	r3, [r6, #68]
	add	r2, sp, #44
	ldr	r3, [r4, #444]
	str	r9, [sp, #44]
	str	r3, [sp, #4]
	ldr	r3, [r4, #436]
	str	r3, [sp]
	ldr	r3, .L880+64
	bl	enc_impl_add_prefix
	ldr	r2, [r6, #60]
	ldr	r3, [r6, #68]
	ldr	r1, [sp, #44]
	add	r2, r2, r1
	add	r3, r3, r1
	str	r2, [r6, #60]
	str	r3, [r6, #68]
.L752:
	movw	r3, #6674
	ldrb	r2, [r4, r3]	@ zero_extendqisi2
	cmp	r2, #3
	bhi	.L756
	add	r9, r4, #6656
	adds	r3, r4, r3
	add	r9, r9, #16
	str	r3, [sp, #36]
.L753:
	ldrb	r3, [r9]	@ zero_extendqisi2
	mov	r1, fp
	and	r3, r3, #3
	str	r3, [sp, #28]
	movs	r3, #0
	str	r3, [sp, #44]
	ldr	r3, [sp, #28]
	adds	r2, r4, r3
	movw	r3, #6675
	ldrb	r3, [r2, r3]	@ zero_extendqisi2
	add	r2, sp, #44
	str	r3, [sp, #4]
	ldr	r3, [sp, #28]
	add	r3, r4, r3, lsl #10
	add	r3, r3, #6656
	adds	r3, r3, #23
	str	r3, [sp]
	ldr	r3, .L880+68
	ldr	r0, [r4, #4]
	bl	enc_impl_add_prefix
	ldr	r3, [r6, #68]
	ldr	r2, [r6, #60]
	ldr	r1, [sp, #44]
	add	r3, r3, r1
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	add	r2, r2, r1
	str	r2, [r6, #60]
	ldr	r2, [sp, #36]
	ldrb	r3, [r3]	@ zero_extendqisi2
	adds	r3, r3, #1
	uxtb	r3, r3
	strb	r3, [r2]
	cmp	r3, #3
	ldr	r2, [sp, #28]
	add	r2, r2, #1
	strb	r2, [r9]
	bls	.L753
.L756:
	mov	r0, r5
	ldr	r1, .L880+72
	bl	check_hal_task_pkt_len
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L755
	ldrh	r3, [r4, #222]
	movw	r2, #1556
	ldr	r1, .L880
	ldr	r0, .L880+76
	bl	printk
.L755:
	mov	r1, r5
	mov	r0, r10
	bl	enc_impl_proc_hal
	mov	r9, r0
	cbz	r0, .L757
	movw	r3, #1557
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L880+80
	b	.L874
.L881:
	.align	2
.L880:
	.word	.LANCHOR0+574
	.word	.LC100
	.word	.LC101
	.word	.LC102
	.word	.LC103
	.word	.LC104
	.word	.LC105
	.word	.LC106
	.word	.LC107
	.word	.LC108
	.word	.LC109
	.word	.LC110
	.word	.LC111
	.word	.LC112
	.word	.LC113
	.word	.LANCHOR1
	.word	.LANCHOR1+16
	.word	.LANCHOR1+32
	.word	.LC114
	.word	.LC6
	.word	.LC7
.L757:
	ldr	r3, [r8]
	lsls	r1, r3, #26
	bpl	.L758
	ldrh	r3, [r4, #222]
	movw	r2, #1559
	ldr	r1, .L882
	ldr	r0, .L882+4
	bl	printk
.L758:
	mov	r1, r5
	ldr	r0, [sp, #32]
	bl	mpp_enc_hal_get_task
	mov	r9, r0
	cbz	r0, .L759
	mov	r3, #1560
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L882+8
	b	.L874
.L759:
	ldr	r3, [r8]
	lsls	r2, r3, #26
	bpl	.L760
	ldrh	r3, [r4, #222]
	movw	r2, #1562
	ldr	r1, .L882
	ldr	r0, .L882+12
	bl	printk
.L760:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_hal_start
	mov	r9, r0
	cbz	r0, .L761
	movw	r3, #1563
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L882+16
	b	.L874
.L761:
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L762
	ldrh	r3, [r4, #222]
	movw	r2, #1565
	ldr	r1, .L882
	ldr	r0, .L882+20
	bl	printk
.L762:
	mov	r1, r5
	ldr	r0, [sp, #32]
	bl	mpp_enc_hal_gen_regs
	mov	r9, r0
	cbz	r0, .L763
	movw	r3, #1566
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L882+24
	b	.L874
.L763:
	str	r0, [r4, #464]
	b	.L698
.L883:
	.align	2
.L882:
	.word	.LANCHOR0+574
	.word	.LC8
	.word	.LC9
	.word	.LC85
	.word	.LC86
	.word	.LC10
	.word	.LC11
	.fnend
	.size	mpp_enc_impl_reg_cfg, .-mpp_enc_impl_reg_cfg
	.align	1
	.global	mpp_enc_impl_hw_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_impl_hw_start, %function
mpp_enc_impl_hw_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r4, r0
	ldr	r7, [r0, #456]
	mov	r5, r1
	ldr	r8, [r0, #8]
	add	r9, r7, #20
	cbz	r1, .L889
	ldr	r6, [r1, #456]
	adds	r6, r6, #20
.L885:
	ldr	r3, .L899
	ldr	r3, [r3]
	lsls	r3, r3, #26
	bpl	.L886
	ldrh	r3, [r4, #222]
	movw	r2, #1931
	ldr	r1, .L899+4
	ldr	r0, .L899+8
	bl	printk
.L886:
	mov	r2, r6
	mov	r1, r9
	mov	r0, r8
	bl	mpp_enc_hal_start
	mov	r6, r0
	cbz	r0, .L884
	movw	r3, #1932
	str	r0, [sp]
	mov	r1, r3
	mov	r2, r4
	ldr	r0, .L899+12
	bl	printk
	mov	r1, r7
	mov	r0, r4
	bl	mpp_enc_terminate_task
	cbz	r5, .L888
	ldr	r1, [r5, #456]
	mov	r0, r5
	bl	mpp_enc_terminate_task
.L888:
	movw	r2, #10796
	ldr	r3, [r4, r2]
	adds	r3, r3, #1
	str	r3, [r4, r2]
.L884:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L889:
	mov	r6, r1
	b	.L885
.L900:
	.align	2
.L899:
	.word	mpp_enc_debug
	.word	.LANCHOR0+593
	.word	.LC12
	.word	.LC13
	.fnend
	.size	mpp_enc_impl_hw_start, .-mpp_enc_impl_hw_start
	.align	1
	.global	mpp_enc_impl_int
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_impl_int, %function
mpp_enc_impl_int:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #52
	sub	sp, sp, #52
	ldr	fp, [r0, #456]
	mov	r4, r0
	strd	r3, r2, [sp, #32]
	ldr	r5, [r0, #8]
	add	r9, fp, #20
	str	r1, [sp, #20]
	cmp	r1, #0
	beq	.L950
	ldr	r7, [r1, #456]
.L902:
	ldr	r3, .L1054
	ldr	r2, [r3]
	mov	r10, r3
	tst	r2, #32
	beq	.L903
	ldrh	r3, [r4, #222]
	movw	r2, #2037
	ldr	r1, .L1054+4
	ldr	r0, .L1054+8
	bl	printk
.L903:
	mov	r0, r5
	mov	r1, r9
	bl	mpp_enc_hal_wait
	mov	r5, r0
	cbnz	r0, .L904
	ldr	r8, [r4, #8]
	cbz	r7, .L905
	adds	r7, r7, #20
.L905:
	ldr	r3, [r10]
	lsls	r5, r3, #26
	bpl	.L906
	ldrh	r3, [r4, #222]
	movw	r2, #1586
	ldr	r1, .L1054+12
	ldr	r0, .L1054+16
	bl	printk
.L906:
	add	r6, r4, #64
	ldr	r0, [r4, #60]
	mov	r1, r6
	bl	rc_hal_end
	mov	r5, r0
	cmp	r0, #0
	beq	.L907
	str	r0, [sp]
	movw	r3, #1587
	ldr	r0, .L1054+20
	mov	r2, r4
	mov	r1, r3
.L1050:
	bl	printk
.L904:
	ldr	r3, [r4, #52]
	movs	r1, #0
	ldr	r0, [r4, #372]
	adds	r3, r3, #1
	str	r3, [r4, #52]
	ldr	r3, [r4, #464]
	str	r1, [r4, #416]
	orr	r3, r3, #1
	str	r3, [r4, #464]
	bl	mpp_packet_set_length
	movw	r3, #10828
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	add	r0, r4, #372
	bl	mpp_packet_deinit
.L938:
	ldr	r3, [r4, #372]
	ldr	r2, [sp, #36]
	str	r3, [r2]
	movs	r3, #0
	str	r3, [r4, #372]
	ldr	r3, [r10]
	lsls	r2, r3, #26
	bpl	.L940
	ldrd	r2, [r4, #384]
	ldr	r1, .L1054+4
	ldr	r0, .L1054+24
	strd	r2, [sp]
	movw	r2, #2117
	ldrh	r3, [r4, #222]
	bl	printk
.L940:
	bl	get_mpidev_ops
	ldr	r3, [r4, #1192]
	cmp	r3, #8
	itet	ne
	ldrbne	r8, [r4, #216]	@ zero_extendqisi2
	moveq	r8, #1
	ubfxne	r8, r8, #4, #1
	mov	r9, r0
	ldr	r0, [fp, #72]
	bl	mpp_frame_get_dts
	strd	r0, [sp, #40]
	ldr	r0, [fp, #72]
	bl	mpp_frame_get_pts
	cmn	r5, #102
	mov	r6, r0
	mov	r7, r1
	bne	.L942
	cmp	r9, #0
	beq	.L944
	ldr	r3, [r9, #32]
	cbz	r3, .L945
	movw	r0, #10828
	add	r2, sp, #40
	movs	r1, #3
	ldr	r0, [r4, r0]
	blx	r3
.L945:
	ldr	r1, [r9, #24]
	cmp	r1, #0
	beq	.L944
	movw	r0, #10828
	str	r8, [sp, #8]
	strd	r6, [sp]
	ldrd	r2, [sp, #40]
	ldr	r0, [r4, r0]
	blx	r1
	b	.L944
.L950:
	ldr	r7, [sp, #20]
	b	.L902
.L907:
	ldr	r3, [r10]
	lsls	r0, r3, #26
	bpl	.L908
	ldrh	r3, [r4, #222]
	movw	r2, #1589
	ldr	r1, .L1054+12
	ldr	r0, .L1054+28
	bl	printk
.L908:
	mov	r2, r7
	mov	r1, r9
	mov	r0, r8
	bl	mpp_enc_hal_ret_task
	mov	r5, r0
	cmp	r0, #0
	bne	.L904
	ldr	r3, [r10]
	lsls	r1, r3, #26
	bpl	.L909
	ldrh	r3, [r4, #222]
	movw	r2, #1594
	ldr	r1, .L1054+12
	ldr	r0, .L1054+32
	bl	printk
.L909:
	mov	r1, r6
	ldr	r0, [r4, #60]
	bl	rc_frm_check_reenc
	mov	r5, r0
	cmp	r0, #0
	beq	.L910
	movw	r3, #1595
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L1054+36
	b	.L1050
.L949:
	ldrb	r2, [r4, #221]	@ zero_extendqisi2
	ldr	r3, [r4, #760]
	cmp	r2, r3
	bcs	.L911
	ldrd	r1, r2, [fp, #64]
	ldr	r3, [r10]
	subs	r2, r2, r1
	strd	r5, r2, [fp, #64]
	ldrb	r2, [fp, #11]	@ zero_extendqisi2
	and	r3, r3, #32
	orr	r2, r2, #1
	strb	r2, [fp, #11]
	movw	r2, #5380
	ldr	r2, [r4, r2]
	cbnz	r2, .L912
	movw	r2, #5384
	ldr	r2, [r4, r2]
	cbz	r2, .L913
.L912:
	cbz	r3, .L951
	movw	r2, #2052
	ldr	r1, .L1054+4
	ldr	r0, .L1054+40
	bl	printk
.L951:
	mov	r5, #-1
	b	.L904
.L1055:
	.align	2
.L1054:
	.word	mpp_enc_debug
	.word	.LANCHOR0+615
	.word	.LC14
	.word	.LANCHOR0+632
	.word	.LC116
	.word	.LC20
	.word	.LC24
	.word	.LC117
	.word	.LC118
	.word	.LC119
	.word	.LC120
.L913:
	cbz	r3, .L914
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	movw	r2, #2058
	ldr	r1, .L1056
	ldr	r0, .L1056+4
	str	r3, [sp, #4]
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	ubfx	r3, r3, #2, #1
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
.L914:
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bpl	.L915
	ldr	r3, [r10]
	lsls	r3, r3, #31
	bpl	.L916
	movw	r2, #1661
	ldr	r1, .L1056+8
	ldr	r0, .L1056+12
	bl	printk
.L916:
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_rollback
	ldr	r3, [fp, #68]
	str	r3, [r4, #248]
	ldr	r3, [r4, #236]
	str	r3, [r4, #252]
	ldr	r3, [r10]
	lsls	r7, r3, #26
	bpl	.L917
	ldrh	r3, [r4, #222]
	movw	r2, #1667
	ldr	r1, .L1056+8
	ldr	r0, .L1056+16
	bl	printk
.L917:
	mov	r1, r6
	ldr	r0, [r4, #60]
	bl	rc_frm_end
	cmp	r0, #0
	beq	.L918
	movw	r3, #1668
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L1056+20
	bl	printk
.L919:
	ldr	r3, [r10]
	lsls	r0, r3, #31
	bpl	.L920
	mov	r2, #1672
	ldr	r1, .L1056+8
	ldr	r0, .L1056+24
	bl	printk
.L920:
	ldrb	r3, [fp, #11]	@ zero_extendqisi2
	bfc	r3, #0, #1
	strb	r3, [fp, #11]
.L915:
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	lsls	r1, r3, #27
	bpl	.L921
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	tst	r3, #160
	bne	.L921
	ldr	r3, [r10]
	add	r8, r4, #464
	ldr	r7, [r4, #4]
	lsls	r2, r3, #31
	bpl	.L922
	movw	r2, #1687
	ldr	r1, .L1056+28
	ldr	r0, .L1056+12
	bl	printk
.L922:
	ldr	r3, [r4, #472]
	ldr	r0, [r4, #460]
	adds	r3, r3, #1
	str	r3, [r4, #472]
	ldr	r3, [r4, #464]
	orr	r3, r3, #2
	str	r3, [r4, #464]
	bl	mpp_enc_refs_rollback
	mov	r1, r8
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_set_usr_cfg
	ldr	r3, [r10]
	lsls	r3, r3, #26
	bpl	.L923
	ldrh	r3, [r4, #222]
	movw	r2, #1697
	ldr	r1, .L1056+28
	ldr	r0, .L1056+32
	bl	printk
.L923:
	mov	r1, r6
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_get_cpb
	ldr	r3, [r10]
	lsls	r0, r3, #15
	bpl	.L924
	ldrh	r3, [r4, #78]
	movw	r2, #1701
	ldr	r1, .L1056+28
	ldr	r0, .L1056+36
	bl	printk
.L924:
	mov	r1, r9
	mov	r0, r7
	bl	enc_impl_proc_dpb
	cbz	r0, .L925
	str	r0, [sp]
	movw	r3, #1702
	ldr	r0, .L1056+40
	mov	r2, r4
	mov	r1, r3
.L1049:
	bl	printk
.L926:
	ldr	r3, [r10]
	lsls	r3, r3, #31
	bpl	.L931
	mov	r2, #1712
	ldr	r1, .L1056+28
	ldr	r0, .L1056+24
	bl	printk
.L931:
	ldrb	r3, [fp, #11]	@ zero_extendqisi2
	bfc	r3, #0, #1
	strb	r3, [fp, #11]
.L921:
	ldrb	r3, [fp, #11]	@ zero_extendqisi2
	lsls	r7, r3, #31
	bpl	.L911
.L1053:
	ldr	r3, [sp, #20]
	cbz	r3, .L901
	ldr	r1, [sp, #32]
	mov	r0, r3
	bl	mpp_enc_comb_end_jpeg
.L901:
	mov	r0, r5
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L918:
	ldr	r2, [r4, #364]
	movw	r3, #10824
	str	r2, [r4, r3]
	b	.L919
.L925:
	ldr	r3, [r10]
	lsls	r1, r3, #26
	bpl	.L927
	ldrh	r3, [r4, #222]
	mov	r2, #1704
	ldr	r1, .L1056+28
	ldr	r0, .L1056+44
	bl	printk
.L927:
	mov	r1, r9
	mov	r0, r7
	bl	enc_impl_sw_enc
	cbz	r0, .L928
	movw	r3, #1705
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L1056+48
	b	.L1049
.L928:
	ldr	r3, [r10]
	lsls	r2, r3, #26
	bpl	.L929
	ldrh	r3, [r4, #222]
	movw	r2, #1707
	ldr	r1, .L1056+28
	ldr	r0, .L1056+16
	bl	printk
.L929:
	mov	r1, r6
	ldr	r0, [r4, #60]
	bl	rc_frm_end
	cbz	r0, .L930
	movw	r3, #1708
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L1056+20
	b	.L1049
.L930:
	ldr	r2, [r4, #364]
	movw	r3, #10824
	str	r2, [r4, r3]
	b	.L926
.L934:
	ldr	r2, [r4, #364]
	movw	r3, #10824
	str	r2, [r4, r3]
	bl	mpp_time
	ldr	r3, [r4, #12]
	mov	r6, r0
	ldr	r0, [r4, #48]
	mov	r7, r1
	strd	r6, [r4, #32]
	add	ip, r0, #1
	str	ip, [r4, #48]
	cbz	r3, .L935
	ldrd	r2, [r4, #24]
	orrs	r1, r2, r3
	beq	.L935
	orrs	r1, r6, r7
	beq	.L935
	subs	r1, r6, r2
	str	r1, [sp, #24]
	sbc	r1, r7, r3
	str	r1, [sp, #28]
	ldrd	r0, [sp, #24]
	mov	r9, #0
	ldr	r8, .L1056+60
	cmp	r8, r0
	sbcs	r1, r9, r1
	bge	.L935
	ldr	r0, .L1056+52
	subs	r2, r6, r2
	smull	r0, r1, r2, r0
	asrs	r2, r2, #31
	movw	r0, #10000
	mul	r0, r0, ip
	rsb	r1, r2, r1, asr #6
	bl	__aeabi_idiv
	movw	r3, #5364
	str	r0, [r4, r3]
	strd	r6, [r4, #24]
	str	r5, [r4, #48]
.L935:
	ldrh	r3, [r4, #220]
	ldr	r0, [r4, #372]
	and	r3, r3, #251
	strh	r3, [r4, #220]	@ movhi
	ldr	r1, [fp, #68]
	bl	mpp_packet_set_length
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	lsls	r1, r3, #27
	bpl	.L937
	movs	r1, #16
	ldr	r0, [r4, #372]
	bl	mpp_packet_set_flag
.L937:
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_eos
	cbz	r0, .L939
	movs	r1, #1
	ldr	r0, [r4, #372]
	bl	mpp_packet_set_flag
.L939:
	ldrb	r1, [r4, #217]	@ zero_extendqisi2
	ldr	r0, [r4, #372]
	lsrs	r1, r1, #4
	bl	mpp_packet_set_temporal_id
	movw	r3, #10828
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	mov	r5, r0
	cmp	r0, #0
	beq	.L938
	movw	r2, #2107
	ldr	r1, .L1056
	ldr	r0, .L1056+56
	movs	r5, #0
	bl	printk
	b	.L938
.L1057:
	.align	2
.L1056:
	.word	.LANCHOR0+615
	.word	.LC87
	.word	.LANCHOR0+644
	.word	.LC84
	.word	.LC121
	.word	.LC22
	.word	.LC88
	.word	.LANCHOR0+663
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC122
	.word	.LC123
	.word	274877907
	.word	.LC23
	.word	999999
.L942:
	cmp	r9, #0
	bne	.L945
.L944:
	ldr	r3, [r4, #368]
	cbz	r3, .L947
	add	r0, r4, #368
	bl	mpp_frame_deinit
.L947:
	movw	r3, #5380
	ldr	r3, [r4, r3]
	cbnz	r3, .L948
	ldr	r3, [r4, #796]
	cbz	r3, .L948
	movw	r3, #5384
	ldr	r3, [r4, r3]
	cbnz	r3, .L948
	movs	r2, #104
	add	r1, r4, #224
	add	r0, r4, #496
	bl	memcpy
.L948:
	ldrb	r2, [r4, #416]	@ zero_extendqisi2
	movs	r3, #0
	strd	r3, r3, [r4, #368]
	strd	r3, r3, [r4, #392]
	and	r2, r2, #1
	str	r2, [r4, #416]
	str	r3, [fp, #8]
	b	.L1053
.L910:
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	lsls	r3, r3, #29
	bmi	.L949
.L911:
	ldr	r3, [r10]
	lsls	r0, r3, #26
	bpl	.L933
	ldrh	r3, [r4, #222]
	movw	r2, #2075
	ldr	r1, .L1058
	ldr	r0, .L1058+4
	bl	printk
.L933:
	mov	r1, r6
	ldr	r0, [r4, #60]
	bl	rc_frm_end
	mov	r5, r0
	cmp	r0, #0
	beq	.L934
	movw	r3, #2076
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L1058+8
	b	.L1050
.L1059:
	.align	2
.L1058:
	.word	.LANCHOR0+615
	.word	.LC21
	.word	.LC22
	.fnend
	.size	mpp_enc_impl_int, .-mpp_enc_impl_int
	.align	1
	.global	mpp_enc_impl_pkt_full_inc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_impl_pkt_full_inc, %function
mpp_enc_impl_pkt_full_inc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movw	r2, #10804
	ldr	r3, [r0, r2]
	adds	r3, r3, #1
	str	r3, [r0, r2]
	bx	lr
	.fnend
	.size	mpp_enc_impl_pkt_full_inc, .-mpp_enc_impl_pkt_full_inc
	.align	1
	.global	mpp_enc_impl_poc_debug_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_impl_poc_debug_info, %function
mpp_enc_impl_poc_debug_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	ldr	r7, [r1, #456]
	.pad #76
	sub	sp, sp, #76
	ldr	r1, .L1075
	mov	r5, r0
	mov	r6, r2
	bl	seq_puts
	ldr	r3, .L1075+4
	mov	r0, r5
	ldr	r2, .L1075+8
	ldr	r1, .L1075+12
	str	r3, [sp, #36]
	ldr	r3, .L1075+16
	str	r3, [sp, #32]
	ldr	r3, .L1075+20
	str	r3, [sp, #28]
	ldr	r3, .L1075+24
	str	r3, [sp, #24]
	ldr	r3, .L1075+28
	str	r3, [sp, #20]
	ldr	r3, .L1075+32
	str	r3, [sp, #16]
	ldr	r3, .L1075+36
	str	r3, [sp, #12]
	ldr	r3, .L1075+40
	str	r3, [sp, #8]
	ldr	r3, .L1075+44
	str	r3, [sp, #4]
	ldr	r3, .L1075+48
	str	r3, [sp]
	ldr	r3, .L1075+52
	bl	seq_printf
	ldr	r0, [r4, #1192]
	ldrd	r8, r9, [r4, #612]
	bl	strof_coding_type
	movw	r3, #5360
	ldr	r7, [r7, #4]
	mov	r10, r0
	ldr	r0, [r4, r3]
	bl	strof_gop_mode
	movw	r3, #5384
	ldr	r3, [r4, r3]
	mov	r2, r6
	ldr	r1, .L1075+56
	str	r3, [sp, #36]
	movw	r3, #5380
	ldr	r3, [r4, r3]
	str	r7, [sp, #12]
	ldr	r7, .L1075+60
	str	r3, [sp, #32]
	ldr	r3, [r4, #632]
	strd	r10, r7, [sp, #4]
	str	r9, [sp]
	str	r3, [sp, #28]
	ldr	r3, [r4, #628]
	str	r3, [sp, #24]
	movs	r3, #0
	strd	r0, r3, [sp, #16]
	mov	r3, r8
	mov	r0, r5
	bl	seq_printf
	ldrd	r0, r1, [r4, #732]
	bl	__aeabi_idiv
	mov	r8, r0
	ldrd	r0, r1, [r4, #744]
	bl	__aeabi_idiv
	ldr	r1, .L1075+64
	mov	r9, r0
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L1075+68
	ldr	r1, .L1075+72
	mov	r0, r5
	ldr	r2, .L1075+8
	str	r3, [sp, #24]
	ldr	r3, .L1075+76
	str	r3, [sp, #20]
	ldr	r3, .L1075+80
	str	r3, [sp, #16]
	ldr	r3, .L1075+84
	str	r3, [sp, #12]
	ldr	r3, .L1075+88
	str	r3, [sp, #8]
	ldr	r3, .L1075+92
	str	r3, [sp, #4]
	ldr	r3, .L1075+96
	str	r3, [sp]
	ldr	r3, .L1075+100
	bl	seq_printf
	ldr	r2, [r4, #40]
	ldr	r0, [r4, #636]
	str	r2, [sp, #68]
	bl	strof_pixel_fmt
	movw	r3, #5364
	ldr	r3, [r4, r3]
	mov	fp, r0
	ldr	r0, [r4, #656]
	str	r3, [sp, #64]
	bl	strof_rotation
	mov	r10, r0
	ldr	r0, [r4, #660]
	bl	strof_bool
	ldr	r1, .L1075+104
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #68]
	strd	r10, r0, [sp, #20]
	mov	r0, r5
	strd	fp, r3, [sp, #12]
	mov	r3, r7
	strd	r9, r2, [sp, #4]
	mov	r2, r6
	str	r8, [sp]
	movw	r8, #10776
	bl	seq_printf
	ldr	r3, [r4, r8]
	cmp	r3, #0
	beq	.L1062
	ldr	r1, .L1075+108
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L1075+112
	ldr	r2, .L1075+8
	mov	r0, r5
	ldr	r1, .L1075+116
	str	r3, [sp, #20]
	ldr	r3, .L1075+120
	str	r3, [sp, #16]
	ldr	r3, .L1075+124
	str	r3, [sp, #12]
	ldr	r3, .L1075+128
	str	r3, [sp, #8]
	ldr	r3, .L1075+132
	str	r3, [sp, #4]
	ldr	r3, .L1075+136
	str	r3, [sp]
	ldr	r3, .L1075+140
	bl	seq_printf
	ldr	r2, [r4, r8]
	ldr	r1, .L1075+144
	mov	r0, r5
	ldr	r3, [r2, #36]
	str	r3, [sp, #20]
	ldr	r3, [r2, #40]
	str	r3, [sp, #16]
	ldr	r3, [r2, #32]
	str	r3, [sp, #12]
	ldr	r3, [r2, #8]
	str	r3, [sp, #8]
	ldr	r3, [r2, #12]
	str	r3, [sp, #4]
	ldr	r3, [r2]
	str	r3, [sp]
	ldr	r3, [r2, #4]
	mov	r2, r6
	bl	seq_printf
.L1062:
	ldr	r1, .L1075+148
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L1075+152
	ldr	r2, .L1075+8
	mov	r0, r5
	ldr	r1, .L1075+156
	str	r3, [sp, #20]
	ldr	r3, .L1075+160
	str	r3, [sp, #16]
	ldr	r3, .L1075+164
	str	r3, [sp, #12]
	ldr	r3, .L1075+168
	str	r3, [sp, #8]
	ldr	r3, .L1075+172
	str	r3, [sp, #4]
	ldr	r3, .L1075+176
	str	r3, [sp]
	ldr	r3, .L1075+180
	bl	seq_printf
	movw	r2, #10804
	ldr	r2, [r4, r2]
	movw	r3, #5372
	ldr	r3, [r4, r3]
	mov	r0, r5
	ldr	r1, .L1075+184
	str	r2, [sp, #20]
	movw	r2, #10800
	ldr	r2, [r4, r2]
	str	r2, [sp, #16]
	movw	r2, #10796
	ldr	r2, [r4, r2]
	str	r2, [sp, #12]
	movw	r2, #10792
	ldr	r2, [r4, r2]
	str	r2, [sp, #8]
	movw	r2, #10788
	ldr	r2, [r4, r2]
	str	r2, [sp, #4]
	add	r2, r4, #5376
	ldr	r2, [r2]
	str	r2, [sp]
	mov	r2, r6
	bl	seq_printf
	ldr	r3, [r4, #1192]
	cmp	r3, #7
	beq	.L1063
	ldr	r2, .L1075+188
	cmp	r3, r2
	bne	.L1064
.L1063:
	ldr	r1, .L1075+192
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L1075+196
	ldr	r2, .L1075+8
	mov	r0, r5
	ldr	r1, .L1075+200
	str	r3, [sp, #20]
	ldr	r3, .L1075+204
	str	r3, [sp, #16]
	ldr	r3, .L1075+208
	str	r3, [sp, #12]
	ldr	r3, .L1075+212
	str	r3, [sp, #8]
	ldr	r3, .L1075+216
	str	r3, [sp, #4]
	ldr	r3, .L1075+220
	str	r3, [sp]
	ldr	r3, .L1075+224
	bl	seq_printf
	ldr	r3, [r4, #932]
	mov	r2, r6
	ldr	r1, .L1075+228
	mov	r0, r5
	str	r3, [sp, #20]
	ldr	r3, [r4, #928]
	str	r3, [sp, #16]
	ldr	r3, [r4, #920]
	str	r3, [sp, #12]
	ldr	r3, [r4, #916]
	str	r3, [sp, #8]
	ldr	r3, [r4, #924]
	str	r3, [sp, #4]
	ldr	r3, [r4, #908]
	str	r3, [sp]
	ldr	r3, [r4, #912]
	bl	seq_printf
	ldr	r3, .L1075+232
	ldr	r2, .L1075+8
	mov	r0, r5
	ldr	r1, .L1075+236
	bl	seq_printf
	ldr	r3, [r4, #996]
	mov	r2, r6
	ldr	r1, .L1075+240
	mov	r0, r5
	str	r3, [sp, #56]
	ldr	r3, [r4, #992]
	str	r3, [sp, #52]
	ldr	r3, [r4, #988]
	str	r3, [sp, #48]
	ldr	r3, [r4, #984]
	str	r3, [sp, #44]
	ldr	r3, [r4, #980]
	str	r3, [sp, #40]
	ldr	r3, [r4, #976]
	str	r3, [sp, #36]
	ldr	r3, [r4, #972]
	str	r3, [sp, #32]
	ldr	r3, [r4, #968]
	str	r3, [sp, #28]
	ldr	r3, [r4, #964]
	str	r3, [sp, #24]
	ldr	r3, [r4, #960]
	str	r3, [sp, #20]
	ldr	r3, [r4, #956]
	str	r3, [sp, #16]
	ldr	r3, [r4, #952]
	str	r3, [sp, #12]
	b	.L1076
.L1077:
	.align	2
.L1075:
	.word	.LC124
	.word	.LC128
	.word	.LC126
	.word	.LC127
	.word	.LC129
	.word	.LC130
	.word	.LC131
	.word	.LC132
	.word	.LC133
	.word	.LC134
	.word	.LC135
	.word	.LC136
	.word	.LC137
	.word	.LC125
	.word	.LC138
	.word	.LC139
	.word	.LC140
	.word	.LC143
	.word	.LC142
	.word	.LC144
	.word	.LC145
	.word	.LC146
	.word	.LC147
	.word	.LC148
	.word	.LC149
	.word	.LC141
	.word	.LC150
	.word	.LC151
	.word	.LC154
	.word	.LC153
	.word	.LC155
	.word	.LC156
	.word	.LC157
	.word	.LC158
	.word	.LC159
	.word	.LC152
	.word	.LC160
	.word	.LC161
	.word	.LC164
	.word	.LC163
	.word	.LC165
	.word	.LC166
	.word	.LC167
	.word	.LC168
	.word	.LC169
	.word	.LC162
	.word	.LC170
	.word	16777220
	.word	.LC171
	.word	.LC174
	.word	.LC173
	.word	.LC175
	.word	.LC176
	.word	.LC177
	.word	.LC178
	.word	.LC179
	.word	.LC172
	.word	.LC180
	.word	.LC181
	.word	.LC182
	.word	.LC183
.L1076:
	ldr	r3, [r4, #948]
	str	r3, [sp, #8]
	ldr	r3, [r4, #944]
	str	r3, [sp, #4]
	ldr	r3, [r4, #940]
	str	r3, [sp]
	ldr	r3, [r4, #936]
	bl	seq_printf
	ldr	r3, .L1078
	ldr	r2, .L1078+4
	mov	r0, r5
	ldr	r1, .L1078+8
	bl	seq_printf
	ldr	r3, [r4, #1060]
	mov	r2, r6
	ldr	r1, .L1078+12
	mov	r0, r5
	str	r3, [sp, #56]
	ldr	r3, [r4, #1056]
	str	r3, [sp, #52]
	ldr	r3, [r4, #1052]
	str	r3, [sp, #48]
	ldr	r3, [r4, #1048]
	str	r3, [sp, #44]
	ldr	r3, [r4, #1044]
	str	r3, [sp, #40]
	ldr	r3, [r4, #1040]
	str	r3, [sp, #36]
	ldr	r3, [r4, #1036]
	str	r3, [sp, #32]
	ldr	r3, [r4, #1032]
	str	r3, [sp, #28]
	ldr	r3, [r4, #1028]
	str	r3, [sp, #24]
	ldr	r3, [r4, #1024]
	str	r3, [sp, #20]
	ldr	r3, [r4, #1020]
	str	r3, [sp, #16]
	ldr	r3, [r4, #1016]
	str	r3, [sp, #12]
	ldr	r3, [r4, #1012]
	str	r3, [sp, #8]
	ldr	r3, [r4, #1008]
	str	r3, [sp, #4]
	ldr	r3, [r4, #1004]
	str	r3, [sp]
	ldr	r3, [r4, #1000]
	bl	seq_printf
	ldr	r3, .L1078+16
	ldr	r2, .L1078+4
	mov	r0, r5
	ldr	r1, .L1078+8
	bl	seq_printf
	ldr	r3, [r4, #1124]
	mov	r2, r6
	ldr	r1, .L1078+20
	mov	r0, r5
	str	r3, [sp, #56]
	ldr	r3, [r4, #1120]
	str	r3, [sp, #52]
	ldr	r3, [r4, #1116]
	str	r3, [sp, #48]
	ldr	r3, [r4, #1112]
	str	r3, [sp, #44]
	ldr	r3, [r4, #1108]
	str	r3, [sp, #40]
	ldr	r3, [r4, #1104]
	str	r3, [sp, #36]
	ldr	r3, [r4, #1100]
	str	r3, [sp, #32]
	ldr	r3, [r4, #1096]
	str	r3, [sp, #28]
	ldr	r3, [r4, #1092]
	str	r3, [sp, #24]
	ldr	r3, [r4, #1088]
	str	r3, [sp, #20]
	ldr	r3, [r4, #1084]
	str	r3, [sp, #16]
	ldr	r3, [r4, #1080]
	str	r3, [sp, #12]
	ldr	r3, [r4, #1076]
	str	r3, [sp, #8]
	ldr	r3, [r4, #1072]
	str	r3, [sp, #4]
	ldr	r3, [r4, #1068]
	str	r3, [sp]
	ldr	r3, [r4, #1064]
	bl	seq_printf
	ldr	r3, .L1078+24
	ldr	r2, .L1078+4
	mov	r0, r5
	ldr	r1, .L1078+8
	bl	seq_printf
	ldr	r3, [r4, #1188]
	mov	r2, r6
	ldr	r1, .L1078+20
	mov	r0, r5
	str	r3, [sp, #56]
	ldr	r3, [r4, #1184]
	str	r3, [sp, #52]
	ldr	r3, [r4, #1180]
	str	r3, [sp, #48]
	ldr	r3, [r4, #1176]
	str	r3, [sp, #44]
	ldr	r3, [r4, #1172]
	str	r3, [sp, #40]
	ldr	r3, [r4, #1168]
	str	r3, [sp, #36]
	ldr	r3, [r4, #1164]
	str	r3, [sp, #32]
	ldr	r3, [r4, #1160]
	str	r3, [sp, #28]
	ldr	r3, [r4, #1156]
	str	r3, [sp, #24]
	ldr	r3, [r4, #1152]
	str	r3, [sp, #20]
	ldr	r3, [r4, #1148]
	str	r3, [sp, #16]
	ldr	r3, [r4, #1144]
	str	r3, [sp, #12]
	ldr	r3, [r4, #1140]
	str	r3, [sp, #8]
	ldr	r3, [r4, #1136]
	str	r3, [sp, #4]
	ldr	r3, [r4, #1132]
	str	r3, [sp]
	ldr	r3, [r4, #1128]
	bl	seq_printf
	ldr	r1, .L1078+28
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L1078+32
	ldr	r2, .L1078+4
	mov	r0, r5
	ldr	r1, .L1078+36
	str	r3, [sp, #16]
	ldr	r3, .L1078+40
	str	r3, [sp, #12]
	ldr	r3, .L1078+44
	str	r3, [sp, #8]
	ldr	r3, .L1078+48
	str	r3, [sp, #4]
	ldr	r3, .L1078+52
	str	r3, [sp]
	ldr	r3, .L1078+56
	bl	seq_printf
	add	r2, r4, #5312
	ldr	r1, .L1078+60
	mov	r0, r5
	ldr	r3, [r2, #44]
	str	r3, [sp, #16]
	ldr	r3, [r2, #40]
	str	r3, [sp, #12]
	ldr	r3, [r2, #36]
	str	r3, [sp, #8]
	ldr	r3, [r2, #32]
	str	r3, [sp, #4]
	ldr	r3, [r2, #28]
	str	r3, [sp]
	ldr	r3, [r2, #24]
	mov	r2, r6
	bl	seq_printf
.L1064:
	ldr	r3, [r4, #4044]
	cbnz	r3, .L1065
.L1068:
	mov	r1, r6
	mov	r0, r5
	bl	mpp_show_session_info
	mov	r2, r6
	ldr	r1, [r4, #4]
	mov	r0, r5
	bl	enc_impl_proc_debug
	mov	r2, r6
	ldr	r1, [r4, #60]
	mov	r0, r5
	add	sp, sp, #76
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	rc_proc_show
.L1065:
	ldr	r1, .L1078+64
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L1078+68
	ldr	r2, .L1078+4
	mov	r0, r5
	ldr	r1, .L1078+72
	mov	r8, #0
	ldr	r9, .L1078+100
	str	r3, [sp, #24]
	ldr	r3, .L1078+76
	str	r3, [sp, #20]
	ldr	r3, .L1078+80
	str	r3, [sp, #16]
	ldr	r3, .L1078+84
	str	r3, [sp, #12]
	ldr	r3, .L1078+88
	strd	r7, r3, [sp, #4]
	add	r7, r4, #4048
	ldr	r3, .L1078+92
	str	r3, [sp]
	ldr	r3, .L1078+96
	bl	seq_printf
.L1066:
	ldr	r3, [r4, #4044]
	adds	r7, r7, #16
	cmp	r3, r8
	bls	.L1068
	ldrb	r3, [r7, #-1]	@ zero_extendqisi2
	mov	r2, r6
	mov	r1, r9
	mov	r0, r5
	str	r3, [sp, #24]
	ldrh	r3, [r7, #-8]
	str	r3, [sp, #20]
	ldrsh	r3, [r7, #-6]
	str	r3, [sp, #16]
	ldrh	r3, [r7, #-10]
	str	r3, [sp, #12]
	ldrh	r3, [r7, #-12]
	str	r3, [sp, #8]
	ldrh	r3, [r7, #-14]
	str	r3, [sp, #4]
	ldrh	r3, [r7, #-16]
	str	r3, [sp]
	mov	r3, r8
	bl	seq_printf
	add	r8, r8, #1
	b	.L1066
.L1079:
	.align	2
.L1078:
	.word	.LC184
	.word	.LC126
	.word	.LC182
	.word	.LC183
	.word	.LC185
	.word	.LC186
	.word	.LC187
	.word	.LC188
	.word	.LC191
	.word	.LC190
	.word	.LC192
	.word	.LC193
	.word	.LC194
	.word	.LC195
	.word	.LC189
	.word	.LC196
	.word	.LC197
	.word	.LC200
	.word	.LC199
	.word	.LC201
	.word	.LC202
	.word	.LC203
	.word	.LC204
	.word	.LC205
	.word	.LC198
	.word	.LC206
	.fnend
	.size	mpp_enc_impl_poc_debug_info, .-mpp_enc_impl_poc_debug_info
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.37071, %object
	.size	__func__.37071, 23
__func__.37071:
	.ascii	"check_hal_task_pkt_len\000"
	.type	__func__.37331, %object
	.size	__func__.37331, 22
__func__.37331:
	.ascii	"mpp_enc_proc_two_pass\000"
	.type	__func__.37214, %object
	.size	__func__.37214, 18
__func__.37214:
	.ascii	"update_rc_cfg_log\000"
	.type	__func__.37488, %object
	.size	__func__.37488, 22
__func__.37488:
	.ascii	"mpp_enc_comb_end_jpeg\000"
	.type	__func__.37113, %object
	.size	__func__.37113, 20
__func__.37113:
	.ascii	"mpp_enc_proc_rc_cfg\000"
	.type	__func__.37121, %object
	.size	__func__.37121, 20
__func__.37121:
	.ascii	"mpp_enc_proc_hw_cfg\000"
	.type	__FUNCTION__.37150, %object
	.size	__FUNCTION__.37150, 22
__FUNCTION__.37150:
	.ascii	"mpp_enc_unref_osd_buf\000"
	.type	__func__.37162, %object
	.size	__func__.37162, 28
__func__.37162:
	.ascii	"mpp_enc_proc_export_osd_buf\000"
	.type	__func__.37174, %object
	.size	__func__.37174, 22
__func__.37174:
	.ascii	"mpp_enc_proc_tune_cfg\000"
	.type	__func__.37195, %object
	.size	__func__.37195, 17
__func__.37195:
	.ascii	"mpp_enc_proc_cfg\000"
	.type	__func__.37144, %object
	.size	__func__.37144, 21
__func__.37144:
	.ascii	"mpp_enc_proc_ref_cfg\000"
	.space	1
	.type	resend_reason.37087, %object
	.size	resend_reason.37087, 24
resend_reason.37087:
	.word	.LC207
	.word	.LC208
	.word	.LC209
	.word	.LC210
	.word	.LC211
	.word	.LC212
	.type	__func__.37262, %object
	.size	__func__.37262, 23
__func__.37262:
	.ascii	"mpp_enc_proc_rc_update\000"
	.type	__FUNCTION__.37243, %object
	.size	__FUNCTION__.37243, 11
__FUNCTION__.37243:
	.ascii	"set_rc_cfg\000"
	.type	__func__.37253, %object
	.size	__func__.37253, 11
__func__.37253:
	.ascii	"set_rc_cfg\000"
	.space	3
	.type	name_of_rc_mode, %object
	.size	name_of_rc_mode, 20
name_of_rc_mode:
	.word	.LC213
	.word	.LC214
	.word	.LC215
	.word	.LC216
	.word	.LC217
	.type	__FUNCTION__.37284, %object
	.size	__FUNCTION__.37284, 34
__FUNCTION__.37284:
	.ascii	"mpp_enc_alloc_output_from_bufpool\000"
	.type	__func__.37291, %object
	.size	__func__.37291, 34
__func__.37291:
	.ascii	"mpp_enc_alloc_output_from_bufpool\000"
	.type	__FUNCTION__.37307, %object
	.size	__FUNCTION__.37307, 34
__FUNCTION__.37307:
	.ascii	"mpp_enc_alloc_output_from_ringbuf\000"
	.type	__func__.37308, %object
	.size	__func__.37308, 34
__func__.37308:
	.ascii	"mpp_enc_alloc_output_from_ringbuf\000"
	.type	__FUNCTION__.37418, %object
	.size	__FUNCTION__.37418, 24
__FUNCTION__.37418:
	.ascii	"mpp_enc_impl_alloc_task\000"
	.type	__FUNCTION__.37422, %object
	.size	__FUNCTION__.37422, 23
__FUNCTION__.37422:
	.ascii	"mpp_enc_impl_free_task\000"
	.type	__func__.37385, %object
	.size	__func__.37385, 21
__func__.37385:
	.ascii	"mpp_enc_reenc_simple\000"
	.type	__func__.37451, %object
	.size	__func__.37451, 21
__func__.37451:
	.ascii	"mpp_enc_impl_reg_cfg\000"
	.type	__FUNCTION__.37452, %object
	.size	__FUNCTION__.37452, 21
__FUNCTION__.37452:
	.ascii	"mpp_enc_impl_reg_cfg\000"
	.type	__func__.37350, %object
	.size	__func__.37350, 19
__func__.37350:
	.ascii	"mpp_enc_normal_cfg\000"
	.type	__func__.37471, %object
	.size	__func__.37471, 22
__func__.37471:
	.ascii	"mpp_enc_impl_hw_start\000"
	.type	__func__.37507, %object
	.size	__func__.37507, 17
__func__.37507:
	.ascii	"mpp_enc_impl_int\000"
	.type	__func__.37364, %object
	.size	__func__.37364, 12
__func__.37364:
	.ascii	"mpp_enc_end\000"
	.type	__func__.37396, %object
	.size	__func__.37396, 19
__func__.37396:
	.ascii	"mpp_enc_reenc_drop\000"
	.type	__func__.37409, %object
	.size	__func__.37409, 26
__func__.37409:
	.ascii	"mpp_enc_reenc_force_pskip\000"
	.data
	.set	.LANCHOR1,. + 0
	.type	uuid_version, %object
	.size	uuid_version, 16
uuid_version:
	.byte	61
	.byte	7
	.byte	109
	.byte	69
	.byte	115
	.byte	15
	.byte	65
	.byte	-88
	.byte	-79
	.byte	-60
	.byte	37
	.byte	-41
	.byte	-105
	.byte	107
	.byte	-15
	.byte	-84
	.type	uuid_rc_cfg, %object
	.size	uuid_rc_cfg, 16
uuid_rc_cfg:
	.byte	-41
	.byte	-36
	.byte	3
	.byte	-61
	.byte	-59
	.byte	111
	.byte	64
	.byte	-32
	.byte	-114
	.byte	-87
	.byte	23
	.byte	26
	.byte	-46
	.byte	-17
	.byte	94
	.byte	35
	.type	uuid_usr_data, %object
	.size	uuid_usr_data, 16
uuid_usr_data:
	.byte	-2
	.byte	57
	.byte	-84
	.byte	76
	.byte	74
	.byte	-114
	.byte	75
	.byte	75
	.byte	-123
	.byte	-39
	.byte	-78
	.byte	-94
	.byte	79
	.byte	-95
	.byte	25
	.byte	91
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: %s check failed: task length is not mat"
	.ascii	"ch to packet length %d vs %d\012\000"
.LC1:
	.ascii	"\0013%d: frame stride set equal cfg stride\000"
.LC2:
	.ascii	"\0016%s:%d: task %d two pass mode enter\012\000"
.LC3:
	.ascii	"\0016%s:%d: task %d enc proc dpb\012\000"
.LC4:
	.ascii	"\0016%s:%d: frm %d start **************************"
	.ascii	"*********\012\000"
.LC5:
	.ascii	"\0013%d: enc %p enc_impl_proc_dpb:%-4d failed retur"
	.ascii	"n %d\000"
.LC6:
	.ascii	"\0016%s:%d: task %d enc proc hal\012\000"
.LC7:
	.ascii	"\0013%d: enc %p enc_impl_proc_hal:%-4d failed retur"
	.ascii	"n %d\000"
.LC8:
	.ascii	"\0016%s:%d: task %d hal get task\012\000"
.LC9:
	.ascii	"\0013%d: enc %p mpp_enc_hal_get_task:%-4d failed re"
	.ascii	"turn %d\000"
.LC10:
	.ascii	"\0016%s:%d: task %d hal generate reg\012\000"
.LC11:
	.ascii	"\0013%d: enc %p mpp_enc_hal_gen_regs:%-4d failed re"
	.ascii	"turn %d\000"
.LC12:
	.ascii	"\0016%s:%d: task %d hal start\012\000"
.LC13:
	.ascii	"\0013%d: enc %p mpp_enc_hal_start:%-4d failed retur"
	.ascii	"n %d\000"
.LC14:
	.ascii	"\0016%s:%d: task %d hal wait\012\000"
.LC15:
	.ascii	"\0013%d: enc %p mpp_enc_hal_wait:%-4d failed return"
	.ascii	" %d\000"
.LC16:
	.ascii	"\0013%d: enc %p mpp_enc_hal_ret_task:%-4d failed re"
	.ascii	"turn %d\000"
.LC17:
	.ascii	"\0016%s:%d: task %d two pass mode leave\012\000"
.LC18:
	.ascii	"%s-b:%d[%d:%d]-g:%d-q:%d:[%d:%d]:[%d:%d]:%d\012\000"
.LC19:
	.ascii	"\0016%s:%d: rc cfg log is full\012\000"
.LC20:
	.ascii	"\0013%d: enc %p rc_hal_end:%-4d failed return %d\000"
.LC21:
	.ascii	"\0016%s:%d: task %d rc enc->frame end\012\000"
.LC22:
	.ascii	"\0013%d: enc %p rc_frm_end:%-4d failed return %d\000"
.LC23:
	.ascii	"\0013%s:%d: ring_buf_put_used fail \012\000"
.LC24:
	.ascii	"\0016%s:%d: task %d enqueue packet pts %lld\012\000"
.LC25:
	.ascii	"\0013%d: invalid rc_priority %d should be[%d, %d] \012"
	.ascii	"\000"
.LC26:
	.ascii	"\0013%d: invalid super_mode %d should be[%d, %d] \012"
	.ascii	"\000"
.LC27:
	.ascii	"\0013%d: invalid debre_strength should be[%d, %d] \012"
	.ascii	"\000"
.LC28:
	.ascii	"\0013%d: invalid rc mode %d should be RC_MODE_VBR o"
	.ascii	"r RC_MODE_CBR\012\000"
.LC29:
	.ascii	"\0013%d: invalid quality %d should be from QUALITY_"
	.ascii	"WORST to QUALITY_BEST\012\000"
.LC30:
	.ascii	"\0013%d: invalid bit per second %d [%d:%d] out of r"
	.ascii	"ange 1K~100M\012\000"
.LC31:
	.ascii	"\0013%d: invalid qp range: init %d i [%d:%d] p [%d:"
	.ascii	"%d]\012\000"
.LC32:
	.ascii	"\0013%d: restore qp range: init %d i [%d:%d] p [%d:"
	.ascii	"%d]\012\000"
.LC33:
	.ascii	"\0013%d: invalid qp delta ip %d restore to %d\012\000"
.LC34:
	.ascii	"\0013%d: invalid qp delta vi %d restore to %d\012\000"
.LC35:
	.ascii	"\0013%d: invalid qp max step %d restore to %d\012\000"
.LC36:
	.ascii	"\0013%d: warning: bitrate statistic time %d is larg"
	.ascii	"er than 60s\012\000"
.LC37:
	.ascii	"\0013%s:%d: failed to accept new rc config\012\000"
.LC38:
	.ascii	"\0016%d: MPP_ENC_SET_RC_CFG bps %d [%d : %d] fps [%"
	.ascii	"d:%d] gop %d\012\000"
.LC39:
	.ascii	"\0013%d: invalid hw qp delta row [%d:%d]\012\000"
.LC40:
	.ascii	"\0013%d: invalid hw qbias_en [%d]\012\000"
.LC41:
	.ascii	"\0013%d: invalid hw qp bias [%d:%d]\012\000"
.LC42:
	.ascii	"\0013%d: invalid filter strength [%d:%d]\012\000"
.LC43:
	.ascii	"\0013%s:%d: failed to accept new hw config\012\000"
.LC44:
	.ascii	"mpp_enc\000"
.LC45:
	.ascii	"\0013%s:%d: mpibuf_ops get fail\000"
.LC46:
	.ascii	"\0013%d: osd buf dma_buf_get fd %d failed\012\000"
.LC47:
	.ascii	"\0013%d: osd inv buf dma_buf_get fd %d failed\012\000"
.LC48:
	.ascii	"\0013%d: invalid scene mode %d not in range [%d:%d]"
	.ascii	"\012\000"
.LC49:
	.ascii	"\0013%d: invalid motion static switch enable not in"
	.ascii	" range [%d : %d]\012\000"
.LC50:
	.ascii	"\0013%d: invalid deblur strength not in range [0 : "
	.ascii	"7]\012\000"
.LC51:
	.ascii	"\0013%d: invalid anti ring strength not in range [0"
	.ascii	" : 3]\012\000"
.LC52:
	.ascii	"\0013%d: invalid anti line strength not in range [0"
	.ascii	" : 3]\012\000"
.LC53:
	.ascii	"\0013%d: invalid lambda idx not in range [0 : 8]\012"
	.ascii	"\000"
.LC54:
	.ascii	"\0013%s:%d: failed to accept new tuning config\012\000"
.LC55:
	.ascii	"\0016%d: MPP_ENC_SET_CFG in \012\000"
.LC56:
	.ascii	"\0013%d: Please use MPP_ENC_GET_HDR_SYNC instead of"
	.ascii	" unsafe MPP_ENC_GET_EXTRA_INFO\012\000"
.LC57:
	.ascii	"\0013%d: NOTE: MPP_ENC_GET_HDR_SYNC needs MppPacket"
	.ascii	" input\012\000"
.LC58:
	.ascii	"\0016%d: deprecated MPP_ENC_PRE_ALLOC_BUFF control\012"
	.ascii	"\000"
.LC59:
	.ascii	"\0016%s:%d: header mode set to %d\012\000"
.LC60:
	.ascii	"\0013%s:%d: invalid header mode %d\012\000"
.LC61:
	.ascii	"\0013%s:%d: invalid NULL ptr on setting header mode"
	.ascii	"\012\000"
.LC62:
	.ascii	"\0016%s:%d: sei mode set to %d\012\000"
.LC63:
	.ascii	"\0013%s:%d: invalid sei mode %d\012\000"
.LC64:
	.ascii	"\0013%d: ref param_error\000"
.LC65:
	.ascii	"\0013%s:%d: failed to copy ref cfg ret %d\012\000"
.LC66:
	.ascii	"\0013%s:%d: failed to set ref cfg ret %d\012\000"
.LC67:
	.ascii	"\0013%d: user data is overflow\000"
.LC68:
	.ascii	"\0013%d: usr data is big then 1k byte len %d\000"
.LC69:
	.ascii	"\0016%d: send header for %s\012\000"
.LC70:
	.ascii	"flex\000"
.LC71:
	.ascii	"fix\000"
.LC72:
	.ascii	"\0016%s:%d: rc update cfg start\012\000"
.LC73:
	.ascii	"rc->fps_out_num\000"
.LC74:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC75:
	.ascii	"\0013%s:%d: unsupport coding type %d\012\000"
.LC76:
	.ascii	"\0016%d: mode %s bps [%d:%d:%d] fps %s [%d/%d] -> %"
	.ascii	"s [%d/%d] gop i [%d] v [%d]\012\000"
.LC77:
	.ascii	"\0016%s:%d: rc update cfg done\012\000"
.LC78:
	.ascii	"size\000"
.LC79:
	.ascii	"buffer\000"
.LC80:
	.ascii	"\0016%s:%d: create output pkt %p buf %p\012\000"
.LC81:
	.ascii	"\0016%s:%d: output to pkt %p buf %p pos %p length %"
	.ascii	"d\012\000"
.LC82:
	.ascii	"\0013%d: ring buf get mpp_buf fail \012\000"
.LC83:
	.ascii	"\0013%s:%d: ring buffer %d init failed\012\000"
.LC84:
	.ascii	"\0016%s:%d: enter\012\000"
.LC85:
	.ascii	"\0016%s:%d: task %d rc hal start\012\000"
.LC86:
	.ascii	"\0013%d: enc %p rc_hal_start:%-4d failed return %d\000"
.LC87:
	.ascii	"\0016%s:%d: task %d reenc %d times %d\012\000"
.LC88:
	.ascii	"\0016%s:%d: leave\012\000"
.LC89:
	.ascii	"\0016%d: frame info no equal set drop: frame [%d, %"
	.ascii	"d, %d, %d], prep [%d, %d, %d, %d]\000"
.LC90:
	.ascii	"\0016%s:%d: task seq idx %d start\012\000"
.LC91:
	.ascii	"\0013%d: enc %p rc_frm_check_drop:%-4d failed retur"
	.ascii	"n %d\000"
.LC92:
	.ascii	"\0016%s:%d: task %d drop %d\012\000"
.LC93:
	.ascii	"enc->packet\000"
.LC94:
	.ascii	"\0016%s:%d: task %d update header length %d\012\000"
.LC95:
	.ascii	"\0016%s:%d: added_by_change \012\000"
.LC96:
	.ascii	"\0016%s:%d: check_hal_task_pkt_len \012\000"
.LC97:
	.ascii	"gen_hdr and adding\000"
.LC98:
	.ascii	"\0016%s:%d: task %d enc start\012\000"
.LC99:
	.ascii	"\0013%d: enc %p enc_impl_start:%-4d failed return %"
	.ascii	"d\000"
.LC100:
	.ascii	"\0016%s:%d: frm %d compare\012\000"
.LC101:
	.ascii	"\0016%s:%d: seq_idx      %d vs %d\012\000"
.LC102:
	.ascii	"\0016%s:%d: is_idr       %d vs %d\012\000"
.LC103:
	.ascii	"\0016%s:%d: is_intra     %d vs %d\012\000"
.LC104:
	.ascii	"\0016%s:%d: is_non_ref   %d vs %d\012\000"
.LC105:
	.ascii	"\0016%s:%d: is_lt_ref    %d vs %d\012\000"
.LC106:
	.ascii	"\0016%s:%d: lt_idx       %d vs %d\012\000"
.LC107:
	.ascii	"\0016%s:%d: temporal_id  %d vs %d\012\000"
.LC108:
	.ascii	"\0016%s:%d: frm %d done  **************************"
	.ascii	"*********\012\000"
.LC109:
	.ascii	"\0016%s:%d: task %d rc frame start\012\000"
.LC110:
	.ascii	"\0013%d: enc %p rc_frm_start:%-4d failed return %d\000"
.LC111:
	.ascii	"\0016%s:%d: task %d rc frame start ok \012\000"
.LC112:
	.ascii	"\0016%s:%d: task %d IDR header length %d\012\000"
.LC113:
	.ascii	"header adding\000"
.LC114:
	.ascii	"user data adding\000"
.LC115:
	.ascii	"\0013%d: enc %p mpp_enc_normal_cfg:%-4d failed retu"
	.ascii	"rn %d\000"
.LC116:
	.ascii	"\0016%s:%d: task %d rc hal end\012\000"
.LC117:
	.ascii	"\0016%s:%d: task %d hal ret task\012\000"
.LC118:
	.ascii	"\0016%s:%d: task %d rc frame check reenc\012\000"
.LC119:
	.ascii	"\0013%d: enc %p rc_frm_check_reenc:%-4d failed retu"
	.ascii	"rn %d\000"
.LC120:
	.ascii	"\0016%s:%d: shared status can't reenc drop request "
	.ascii	"idr\012\000"
.LC121:
	.ascii	"\0016%s:%d: task %d rc frame end\012\000"
.LC122:
	.ascii	"\0016%s:%d: task %d enc sw enc start\012\000"
.LC123:
	.ascii	"\0013%d: enc %p enc_impl_sw_enc:%-4d failed return "
	.ascii	"%d\000"
.LC124:
	.ascii	"\012--------venc chn attr 1------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC125:
	.ascii	"Width\000"
.LC126:
	.ascii	"ID\000"
.LC127:
	.ascii	"%8s|%8s|%8s|%6s|%9s|%10s|%10s|%6s|%10s|%11s|%8s|%10"
	.ascii	"s\012\000"
.LC128:
	.ascii	"RefShare\000"
.LC129:
	.ascii	"Online\000"
.LC130:
	.ascii	"MaxHeight\000"
.LC131:
	.ascii	"MaxWidth\000"
.LC132:
	.ascii	"Prio\000"
.LC133:
	.ascii	"GopMode\000"
.LC134:
	.ascii	"Sequence\000"
.LC135:
	.ascii	"ByFrame\000"
.LC136:
	.ascii	"Type\000"
.LC137:
	.ascii	"Height\000"
.LC138:
	.ascii	"%8d|%8u|%8u|%6s|%9s|%10u|%10s|%6d|%10d|%11d|%8d|%10"
	.ascii	"d\012\000"
.LC139:
	.ascii	"y\000"
.LC140:
	.ascii	"\012--------venc chn attr 2------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC141:
	.ascii	"VeStr\000"
.LC142:
	.ascii	"%8s|%8s|%8s|%8s|%12s|%12s|%12s|%12s|%10s\012\000"
.LC143:
	.ascii	"mirror\000"
.LC144:
	.ascii	"rotation\000"
.LC145:
	.ascii	"RealFps*10\000"
.LC146:
	.ascii	"PixFmt\000"
.LC147:
	.ascii	"Timeref\000"
.LC148:
	.ascii	"TarFr\000"
.LC149:
	.ascii	"SrcFr\000"
.LC150:
	.ascii	"%8d|%8s|%8d|%8d|%12x|%12s|%12u|%12s|%10s\012\000"
.LC151:
	.ascii	"\012--------ring buf status------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC152:
	.ascii	"w_pos\000"
.LC153:
	.ascii	"%8s|%8s|%8s|%8s|%10s|%10s|%10s|%10s\012\000"
.LC154:
	.ascii	"l_r_pos\000"
.LC155:
	.ascii	"l_w_pos\000"
.LC156:
	.ascii	"min_size\000"
.LC157:
	.ascii	"total_len\000"
.LC158:
	.ascii	"usd_len\000"
.LC159:
	.ascii	"r_pos\000"
.LC160:
	.ascii	"%8d|%8d|%8d|%8d|%10d|%10d|%10d|%10d\012\000"
.LC161:
	.ascii	"\012--------hw status------------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC162:
	.ascii	"hw_run\000"
.LC163:
	.ascii	"%8s|%8s|%12s|%14s|%14s|%14s|%16s|%14s\012\000"
.LC164:
	.ascii	"pkt_full_cnt\000"
.LC165:
	.ascii	"start_fail_cnt\000"
.LC166:
	.ascii	"cfg_fail_cnt\000"
.LC167:
	.ascii	"ring_fail_cnt\000"
.LC168:
	.ascii	"pkt_fail_cnt\000"
.LC169:
	.ascii	"enc_status\000"
.LC170:
	.ascii	"%8d|%8d|%12d|%14u|%14u|%14u|%16u|%14u\012\000"
.LC171:
	.ascii	"\012--------hw param-------------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC172:
	.ascii	"qp_dlt_row_i\000"
.LC173:
	.ascii	"%8s|%14s|%12s|%12s|%12s|%12s|%12s|%12s|\012\000"
.LC174:
	.ascii	"flt_str_p\000"
.LC175:
	.ascii	"flt_str_i\000"
.LC176:
	.ascii	"qbias_p\000"
.LC177:
	.ascii	"qbias_i\000"
.LC178:
	.ascii	"qbias_en\000"
.LC179:
	.ascii	"qp_dlt_row\000"
.LC180:
	.ascii	"%8u|%14d|%12d|%12d|%12d|%12d|%12d|%12d\012\000"
.LC181:
	.ascii	"thrd_i[0] ~ thrd_i[15]\000"
.LC182:
	.ascii	"%8s|%24s\012\000"
.LC183:
	.ascii	"%8u|%4u|%4u|%4u|%4u|%4u|%4u|%4u|%4u|%4u|%4u|%4u|%4u"
	.ascii	"|%4u|%4u|%4u|%4u\012\000"
.LC184:
	.ascii	"thrd_p[0] ~ thrd_p[15]\000"
.LC185:
	.ascii	"step_i[0] ~ step_i[15]\000"
.LC186:
	.ascii	"%8u|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d|%4d"
	.ascii	"|%4d|%4d|%4d|%4d\012\000"
.LC187:
	.ascii	"step_p[0] ~ step_p[15]\000"
.LC188:
	.ascii	"\012--------fine tuning param----------------------"
	.ascii	"--------------------------------------------------\012"
	.ascii	"\000"
.LC189:
	.ascii	"scene_mode\000"
.LC190:
	.ascii	"%8s|%12s|%12s|%12s|%10s|%10s|%12s|\012\000"
.LC191:
	.ascii	"lambda_idx\000"
.LC192:
	.ascii	"atl_str\000"
.LC193:
	.ascii	"atr_str\000"
.LC194:
	.ascii	"deblur_str\000"
.LC195:
	.ascii	"md_swth_en\000"
.LC196:
	.ascii	"%8u|%12d|%12d|%12d|%10d|%10d|%12d\012\000"
.LC197:
	.ascii	"\012--------venc roi attr -------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC198:
	.ascii	"roi\000"
.LC199:
	.ascii	"%8s|%8s|%8s|%8s|%8s|%8s|%8s|%8s|%8s\012\000"
.LC200:
	.ascii	"abs_qp\000"
.LC201:
	.ascii	"intra\000"
.LC202:
	.ascii	"quality\000"
.LC203:
	.ascii	"h\000"
.LC204:
	.ascii	"w\000"
.LC205:
	.ascii	"x\000"
.LC206:
	.ascii	"%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d|%8d\012\000"
.LC207:
	.ascii	"unchanged\000"
.LC208:
	.ascii	"codec/prep cfg change\000"
.LC209:
	.ascii	"rc cfg change rc_mode/fps/gop\000"
.LC210:
	.ascii	"set cfg change input/format \000"
.LC211:
	.ascii	"set cfg change rc_mode/fps/gop\000"
.LC212:
	.ascii	"set cfg change codec\000"
.LC213:
	.ascii	"vbr\000"
.LC214:
	.ascii	"cbr\000"
.LC215:
	.ascii	"fixqp\000"
.LC216:
	.ascii	"avbr\000"
.LC217:
	.ascii	"smtrc\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
