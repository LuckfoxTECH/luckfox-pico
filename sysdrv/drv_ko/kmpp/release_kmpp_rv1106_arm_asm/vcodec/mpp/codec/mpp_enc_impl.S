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
	movs	r2, #182
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
	movw	r1, #1145
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
	ldr	r3, .L66
	ldr	r3, [r3]
	lsls	r6, r3, #26
	str	r0, [sp, #16]
	bpl	.L22
	ldrh	r3, [r4, #222]
	movw	r2, #1315
	ldr	r1, .L66+4
	ldr	r0, .L66+8
	bl	printk
.L22:
	movs	r2, #104
	add	r1, r4, #496
	add	r0, r4, #224
	bl	memcpy
	ldr	r3, .L66
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #32
	beq	.L23
	ldrh	r3, [r4, #222]
	movw	r2, #1318
	ldr	r1, .L66+4
	ldr	r0, .L66+12
	bl	printk
.L23:
	mov	r1, r5
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_get_cpb_pass1
	ldr	r3, [r9]
	lsls	r5, r3, #15
	bpl	.L24
	ldrh	r3, [r4, #78]
	movw	r2, #1321
	ldr	r1, .L66+4
	ldr	r0, .L66+16
	bl	printk
.L24:
	add	r6, r7, #20
	mov	r0, fp
	mov	r1, r6
	bl	enc_impl_proc_dpb
	mov	r5, r0
	cbz	r0, .L25
	str	r0, [sp]
	movw	r3, #1322
	ldr	r0, .L66+20
	mov	r2, r4
	mov	r1, r3
.L65:
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
	movw	r2, #1324
	ldr	r1, .L66+4
	ldr	r0, .L66+24
	bl	printk
.L27:
	mov	r1, r6
	mov	r0, fp
	bl	enc_impl_proc_hal
	mov	r5, r0
	cbz	r0, .L28
	movw	r3, #1325
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L66+28
	b	.L65
.L28:
	ldr	r3, .L66
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #32
	beq	.L29
	ldrh	r3, [r4, #222]
	movw	r2, #1327
	ldr	r1, .L66+4
	ldr	r0, .L66+32
	bl	printk
.L29:
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_get_task
	mov	r5, r0
	cbz	r0, .L30
	mov	r3, #1328
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L66+36
	b	.L65
.L30:
	ldr	r3, [r9]
	lsls	r1, r3, #26
	bpl	.L31
	ldrh	r3, [r4, #222]
	movw	r2, #1330
	ldr	r1, .L66+4
	ldr	r0, .L66+40
	bl	printk
.L31:
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_gen_regs
	mov	r5, r0
	cbz	r0, .L32
	movw	r3, #1331
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L66+44
	b	.L65
.L32:
	ldr	r3, [r9]
	lsls	r2, r3, #26
	bpl	.L33
	ldrh	r3, [r4, #222]
	movw	r2, #1333
	ldr	r1, .L66+4
	ldr	r0, .L66+48
	bl	printk
.L33:
	movs	r2, #0
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_start
	mov	r5, r0
	cbz	r0, .L34
	movw	r3, #1335
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L66+52
	b	.L65
.L34:
	ldrh	r2, [r4, #222]
	movw	r1, #1337
	ldr	r0, .L66+56
	bl	printk
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_wait
	mov	r5, r0
	cbz	r0, .L35
	movw	r3, #1338
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L66+60
	b	.L65
.L35:
	mov	r2, r0
	mov	r1, r6
	mov	r0, r8
	bl	mpp_enc_hal_ret_task
	mov	r5, r0
	cbz	r0, .L36
	movw	r3, #1342
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L66+64
	b	.L65
.L36:
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
	ldr	r3, .L66
	ldr	r3, [r3]
	lsls	r3, r3, #26
	bpl	.L20
	ldrh	r3, [r4, #222]
	mov	r2, #1352
	ldr	r1, .L66+4
	ldr	r0, .L66+68
	bl	printk
	b	.L20
.L67:
	.align	2
.L66:
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
	ldr	r2, .L70
	add	r0, r0, r4
	str	r3, [sp, #4]
	bl	vsnprintf
	add	r4, r4, r0
	cmp	r6, r4
	bgt	.L69
	movw	r2, #911
	ldr	r1, .L70+4
	ldr	r0, .L70+8
	bl	printk
.L69:
	str	r4, [r5, #444]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, lr}
	add	sp, sp, #12
	bx	lr
.L71:
	.align	2
.L70:
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
	ldr	r3, [r0, #368]
	mov	r5, r1
	cbz	r3, .L73
	add	r0, r0, #368
	bl	mpp_frame_deinit
	movs	r3, #0
	str	r3, [r4, #368]
.L73:
	ldr	r0, [r4, #372]
	cbz	r0, .L74
	movs	r1, #0
	bl	mpp_packet_set_length
	movw	r3, #10788
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	add	r0, r4, #372
	bl	mpp_packet_deinit
.L74:
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
	ldr	r8, .L131+40
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
	movw	r3, #1876
	ldr	r0, .L131
	mov	r2, r4
	mov	r1, r3
.L129:
	bl	printk
	ldr	r3, [r4, #52]
	movs	r1, #0
	ldr	r0, [r4, #372]
	adds	r3, r3, #1
	str	r3, [r4, #52]
	bl	mpp_packet_set_length
	movw	r3, #10788
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	add	r0, r4, #372
	bl	mpp_packet_deinit
.L92:
	ldr	r3, [r4, #372]
	ldr	r2, [sp, #28]
	str	r3, [r2]
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L93
	ldrd	r2, [r4, #384]
	ldr	r1, .L131+4
	ldr	r0, .L131+8
	strd	r2, [sp]
	movw	r2, #1923
	ldrh	r3, [r4, #222]
	bl	printk
.L93:
	ldr	r3, [r4, #368]
	cbz	r3, .L94
	add	r0, r4, #368
	bl	mpp_frame_deinit
.L94:
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
	movw	r3, #1877
	str	r0, [sp]
.L130:
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L131+12
	b	.L129
.L84:
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L85
	ldrh	r3, [r4, #222]
	movw	r2, #1878
	ldr	r1, .L131+4
	ldr	r0, .L131+16
	bl	printk
.L85:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_hal_end
	mov	r6, r0
	cbz	r0, .L86
	str	r0, [sp]
	movw	r3, #1879
	b	.L130
.L86:
	ldr	r3, [r8]
	lsls	r1, r3, #26
	bpl	.L87
	ldrh	r3, [r4, #222]
	mov	r2, #1880
	ldr	r1, .L131+4
	ldr	r0, .L131+20
	bl	printk
.L87:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_frm_end
	mov	r6, r0
	cbz	r0, .L88
	movw	r3, #1881
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L131+24
	b	.L129
.L88:
	ldr	r2, [r4, #364]
	movw	r3, #10784
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
	movw	r0, #10788
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
	ldr	r0, .L131+28
	subs	r10, r6, r3
	ldr	r3, [sp, #20]
	sbc	fp, r7, r3
	cmp	r0, r10
	sbcs	r3, r1, fp
	bge	.L90
	ldr	r0, .L131+32
	mov	r2, r10
	ldr	r3, [r4, #48]
	asrs	r2, r2, #31
	smull	r0, r1, r10, r0
	movw	r0, #10000
	muls	r0, r3, r0
	rsb	r1, r2, r1, asr #6
	bl	__aeabi_idiv
	movw	r3, #5316
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
	ldrb	r1, [r4, #217]	@ zero_extendqisi2
	ldr	r0, [r4, #372]
	lsrs	r1, r1, #4
	bl	mpp_packet_set_temporal_id
	movw	r3, #10788
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	mov	r6, r0
	cmp	r0, #0
	beq	.L92
	mov	r2, #1912
	ldr	r1, .L131+4
	ldr	r0, .L131+36
	movs	r6, #0
	bl	printk
	b	.L92
.L132:
	.align	2
.L131:
	.word	.LC16
	.word	.LANCHOR0+63
	.word	.LC25
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	999999
	.word	274877907
	.word	.LC24
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
	beq	.L176
	mov	r1, r0
	movs	r2, #196
	add	r0, sp, #36
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
	bpl	.L137
	ldr	r3, [r5, #12]
	str	r3, [r4, #12]
	ldr	r3, [r5, #16]
	str	r3, [r4, #16]
	ldr	r3, [r5, #20]
	str	r3, [r4, #20]
.L137:
	lsls	r2, r6, #26
	bpl	.L138
	ldr	r3, [r5, #24]
	str	r3, [r4, #24]
	ldr	r3, [r5, #28]
	str	r3, [r4, #28]
	ldr	r3, [r5, #32]
	str	r3, [r4, #32]
.L138:
	lsls	r3, r6, #25
	bpl	.L139
	ldr	r3, [r5, #36]
	str	r3, [r4, #36]
	ldr	r3, [r5, #40]
	str	r3, [r4, #40]
	ldr	r3, [r5, #44]
	str	r3, [r4, #44]
.L139:
	lsls	r7, r6, #24
	itt	mi
	ldrmi	r3, [r5, #48]
	strmi	r3, [r4, #48]
	lsls	r0, r6, #22
	itt	mi
	ldrmi	r3, [r5, #56]
	strmi	r3, [r4, #56]
	lsls	r1, r6, #21
	bpl	.L142
	ldr	r3, [r5, #64]
	str	r3, [r4, #64]
	ldr	r3, [r5, #68]
	str	r3, [r4, #68]
	ldr	r3, [r5, #72]
	str	r3, [r4, #72]
.L142:
	ands	r7, r6, #16384
	beq	.L143
	ldr	r2, [r5, #88]
	cmp	r2, #1
	bls	.L178
	movs	r3, #2
	movw	r1, #369
	str	r3, [sp]
	movs	r3, #0
	ldr	r0, .L253
	mvn	r7, #5
	bl	printk
.L144:
	ldr	r3, [r5, #88]
	str	r3, [r4, #88]
.L143:
	lsls	r2, r6, #16
	bpl	.L145
	ldr	r2, [r5, #76]
	cmp	r2, #2
	bls	.L146
	movs	r3, #3
	mov	r1, #380
	str	r3, [sp]
	movs	r3, #0
	ldr	r0, .L253+4
	mvn	r7, #5
	bl	printk
.L146:
	ldr	r3, [r5, #76]
	str	r3, [r4, #76]
	ldr	r3, [r5, #80]
	str	r3, [r4, #80]
	ldr	r3, [r5, #84]
	str	r3, [r4, #84]
.L145:
	lsls	r3, r6, #7
	bpl	.L147
	ldr	r2, [r5, #92]
	str	r2, [r4, #92]
	ldr	r3, [r5, #96]
	str	r3, [r4, #96]
	cbz	r2, .L147
	cmp	r3, #35
	bls	.L147
	movs	r3, #35
	movs	r2, #0
	movw	r1, #393
	ldr	r0, .L253+8
	bl	printk
	mvn	r7, #5
.L147:
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
	bpl	.L157
	ldr	r3, [r5, #144]
	str	r3, [r4, #144]
	ldr	r3, [r5, #148]
	str	r3, [r4, #148]
	ldr	r3, [r5, #152]
	str	r3, [r4, #152]
	ldr	r3, [r5, #156]
	str	r3, [r4, #156]
.L157:
	lsls	r2, r6, #6
	bpl	.L158
	ldr	r3, [r5, #160]
	add	r2, r4, #164
	add	r1, r5, #180
	str	r3, [r4, #160]
	add	r3, r5, #164
.L159:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L159
	add	r3, r5, #180
	add	r2, r4, #180
	add	r1, r5, #196
.L160:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L160
.L158:
	lsls	r3, r6, #5
	itt	mi
	ldrmi	r3, [r5, #60]
	strmi	r3, [r4, #60]
	ldr	r3, [r4, #4]
	cmp	r3, #3
	bls	.L162
	ldr	r2, [r5, #4]
	movw	r1, #451
	ldr	r0, .L253+12
	mvn	r7, #5
	bl	printk
.L162:
	ldr	r2, [r4, #8]
	cmp	r2, #6
	bls	.L163
	mov	r1, #456
	ldr	r0, .L253+16
	bl	printk
	mvn	r7, #5
.L163:
	ldr	r3, [r4, #4]
	cmp	r3, #2
	beq	.L164
	ldrd	r2, r0, [r4, #12]
	mvn	lr, #1024
	ldr	r1, .L253+20
	add	r5, r2, lr
	ldr	r3, [r4, #20]
	cmp	r5, r1
	bhi	.L165
	mvn	ip, #1024
	add	r5, r0, ip
	cmp	r5, r1
	bhi	.L165
	mvn	r5, #1024
	adds	r5, r3, r5
	cmp	r5, r1
	bls	.L164
.L165:
	str	r0, [sp]
	mov	r1, #468
	ldr	r0, .L253+24
	mvn	r7, #5
	bl	printk
.L164:
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
	blt	.L168
	cmp	r5, #0
	blt	.L168
	cmp	r5, r0
	blt	.L168
	cmp	r3, #0
	blt	.L168
	cmp	r1, #0
	blt	.L168
	cmp	r1, r3
	blt	.L168
	cmp	r2, #0
	ble	.L169
	cmp	r1, r2
	blt	.L168
	cmp	r3, r2
	ble	.L169
.L168:
	strd	r0, r5, [sp, #4]
	str	r1, [sp]
	mov	r1, #484
	ldr	r0, .L253+28
	bl	printk
	str	r8, [r4, #112]
	str	r9, [r4, #128]
	mov	r2, r8
	mov	r1, #494
	ldr	r0, .L253+32
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
.L169:
	ldr	r2, [r4, #136]
	cmp	r2, #0
	bge	.L170
	ldr	r3, [sp, #28]
	mov	r1, #498
	ldr	r0, .L253+36
	bl	printk
	ldr	r3, [sp, #28]
	str	r3, [r4, #136]
.L170:
	ldr	r2, [r4, #140]
	cmp	r2, #0
	bge	.L171
	mov	r3, fp
	movw	r1, #503
	ldr	r0, .L253+40
	bl	printk
	str	fp, [r4, #140]
.L171:
	ldr	r2, [r4, #132]
	cmp	r2, #0
	bge	.L172
	mov	r3, r10
	mov	r1, #508
	ldr	r0, .L253+44
	bl	printk
	str	r10, [r4, #132]
.L172:
	ldr	r2, [r4, #60]
	cmp	r2, #60
	ble	.L173
	movw	r1, #513
	ldr	r0, .L253+48
	bl	printk
.L173:
	ldr	r3, [r4]
	orrs	r6, r6, r3
	str	r6, [r4]
	cbz	r7, .L174
	movw	r2, #519
	ldr	r1, .L253+52
	ldr	r0, .L253+56
	bl	printk
	movs	r2, #196
	mov	r0, r4
	add	r1, sp, #36
	bl	memcpy
.L133:
	mov	r0, r7
	add	sp, sp, #236
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L178:
	movs	r7, #0
	b	.L144
.L174:
	ldr	r3, [r4, #48]
	mov	r1, #524
	ldr	r2, [r4, #12]
	ldr	r0, .L253+60
	str	r3, [sp, #12]
	ldr	r3, [r4, #40]
	str	r3, [sp, #8]
	ldr	r3, [r4, #28]
	str	r3, [sp, #4]
	ldr	r3, [r4, #16]
	str	r3, [sp]
	ldr	r3, [r4, #20]
	bl	printk
	b	.L133
.L176:
	mov	r7, r6
	b	.L133
.L254:
	.align	2
.L253:
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	104856574
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LANCHOR0+85
	.word	.LC38
	.word	.LC39
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
	@ args = 0, pretend = 0, frame = 272
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r6, [r1]
	.pad #272
	sub	sp, sp, #272
	mov	r4, r1
	cmp	r6, #0
	beq	.L270
	mov	r2, #268
	mov	r1, r0
	add	r0, sp, #4
	bl	memcpy
	lsls	r2, r6, #31
	itt	mi
	ldrmi	r3, [r4, #4]
	strmi	r3, [r5, #4]
	lsls	r3, r6, #30
	itt	mi
	ldrmi	r3, [r4, #8]
	strmi	r3, [r5, #8]
	lsls	r0, r6, #29
	bpl	.L259
	add	r3, r4, #12
	add	r2, r5, #12
	add	r1, r4, #76
.L260:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L260
.L259:
	lsls	r1, r6, #28
	bpl	.L261
	add	r3, r4, #76
	add	r2, r5, #76
	add	r1, r4, #140
.L262:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L262
.L261:
	lsls	r2, r6, #27
	bpl	.L263
	add	r3, r4, #140
	add	r2, r5, #140
	add	r1, r4, #204
.L264:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L264
.L263:
	lsls	r3, r6, #26
	bpl	.L265
	add	r3, r4, #204
	add	r2, r5, #204
	add	r4, r4, #268
.L266:
	ldr	r1, [r3], #4	@ unaligned
	cmp	r3, r4
	str	r1, [r2], #4	@ unaligned
	bne	.L266
.L265:
	ldrd	r3, r2, [r5, #4]
	cmp	r3, #0
	blt	.L267
	cmp	r2, #0
	bge	.L271
.L267:
	movw	r1, #563
	ldr	r0, .L297
	bl	printk
	mvn	r4, #5
.L268:
	ldr	r3, [r5]
	orrs	r6, r6, r3
	str	r6, [r5]
	cbz	r4, .L255
	movw	r2, #570
	ldr	r1, .L297+4
	ldr	r0, .L297+8
	bl	printk
	mov	r2, #268
	mov	r0, r5
	add	r1, sp, #4
	bl	memcpy
.L255:
	mov	r0, r4
	add	sp, sp, #272
	@ sp needed
	pop	{r4, r5, r6, pc}
.L271:
	movs	r4, #0
	b	.L268
.L270:
	mov	r4, r6
	b	.L255
.L298:
	.align	2
.L297:
	.word	.LC40
	.word	.LANCHOR0+105
	.word	.LC41
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
	cbz	r0, .L312
	ldr	r3, [r0]
	cbz	r3, .L312
	ldr	r7, .L316
	mov	r5, r0
	ldr	r8, .L316+4
	movs	r6, #0
.L302:
	ldr	r3, [r4, #4]
	cmp	r6, r3
	bcc	.L305
.L312:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
.L305:
	ldr	r0, [r5, #136]
	cbz	r0, .L303
	mov	r2, r7
	mov	r1, r8
	bl	mpi_buf_unref_with_tag
.L303:
	ldr	r0, [r5, #88]
	cbz	r0, .L304
	mov	r2, r7
	mov	r1, r8
	bl	mpi_buf_unref_with_tag
.L304:
	adds	r6, r6, #1
	adds	r5, r5, #140
	b	.L302
.L317:
	.align	2
.L316:
	.word	.LANCHOR0+125
	.word	.LC42
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
	cbz	r5, .L319
	ldr	r3, [r5, #40]
	cbz	r3, .L319
	ldr	r9, .L333+8
	mov	r4, r7
	ldr	r10, .L333+12
	movs	r6, #0
.L320:
	ldr	r3, [r7, #4]
	cmp	r3, r6
	bhi	.L326
	movs	r0, #0
	b	.L318
.L319:
	mov	r2, #684
	ldr	r1, .L333
	ldr	r0, .L333+4
	bl	printk
	mov	r0, #-1
.L318:
	add	sp, sp, #104
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L326:
	ldr	r0, [r4, #132]
	cmp	r0, #0
	ble	.L322
	bl	dma_buf_get
	cmn	r0, #4096
	mov	r8, r0
	bhi	.L323
	ldr	r3, [r5, #40]
	mov	r2, #-1
	mov	r1, sp
	blx	r3
	str	r0, [r4, #136]
	mov	r0, r8
	bl	dma_buf_put
.L322:
	ldr	r0, [r4, #84]
	cmp	r0, #0
	ble	.L324
	bl	dma_buf_get
	cmn	r0, #4096
	mov	r8, r0
	bhi	.L325
	ldr	r3, [r5, #40]
	mov	r2, #-1
	mov	r1, sp
	blx	r3
	str	r0, [r4, #88]
	mov	r0, r8
	bl	dma_buf_put
.L324:
	adds	r6, r6, #1
	adds	r4, r4, #140
	b	.L320
.L323:
	ldr	r2, [r4, #132]
	movw	r1, #699
	mov	r0, r9
	bl	printk
	b	.L322
.L325:
	ldr	r2, [r4, #84]
	movw	r1, #710
	mov	r0, r10
	bl	printk
	b	.L324
.L334:
	.align	2
.L333:
	.word	.LANCHOR0+147
	.word	.LC43
	.word	.LC44
	.word	.LC45
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r0
	ldr	r6, [r1]
	cbz	r6, .L339
	lsls	r3, r6, #31
	ldrd	r8, r7, [r0]
	itt	mi
	ldrmi	r3, [r1, #4]
	strmi	r3, [r0, #4]
	ldr	r2, [r0, #4]
	cmp	r2, #1
	bls	.L340
	movs	r3, #1
	movw	r1, #730
	str	r3, [sp]
	movs	r3, #0
	ldr	r0, .L347
	mvn	r5, #5
	bl	printk
.L338:
	ldr	r3, [r4]
	orrs	r6, r6, r3
	str	r6, [r4]
	cbz	r5, .L335
	movw	r2, #737
	ldr	r1, .L347+4
	ldr	r0, .L347+8
	bl	printk
	strd	r8, r7, [r4]
.L335:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L340:
	movs	r5, #0
	b	.L338
.L339:
	mov	r5, r6
	b	.L335
.L348:
	.align	2
.L347:
	.word	.LC46
	.word	.LANCHOR0+175
	.word	.LC47
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
	ldr	r3, .L482
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	cmp	r1, r3
	mov	r4, r0
	mov	r7, r1
	mov	r6, r2
	beq	.L350
	bhi	.L351
	subs	r3, r3, #6
	cmp	r1, r3
	beq	.L352
	bhi	.L353
	subs	r3, r3, #8
	cmp	r1, r3
	beq	.L354
	adds	r3, r3, #4
	cmp	r1, r3
	beq	.L355
.L356:
	mov	r2, r6
	mov	r1, r7
	b	.L475
.L353:
	ldr	r3, .L482+4
	cmp	r1, r3
	bcc	.L356
	ldrb	r3, [r0, #416]	@ zero_extendqisi2
	lsls	r0, r3, #31
	bmi	.L372
	ldr	r1, [r4, #404]
	ldr	r0, [r4, #4]
	bl	enc_impl_gen_hdr
	ldr	r0, [r4, #404]
	bl	mpp_packet_get_length
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r4, #416]
	str	r0, [r4, #412]
.L372:
	ldr	r3, .L482+8
	cmp	r7, r3
	bne	.L373
	mov	r1, #812
	ldr	r0, .L482+12
	bl	printk
	movw	r1, #813
	ldr	r0, .L482+16
	bl	printk
	ldr	r3, [r4, #404]
	str	r3, [r6]
.L374:
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	orr	r3, r3, #2
	strb	r3, [r4, #416]
	b	.L473
.L351:
	ldr	r3, .L482+20
	cmp	r1, r3
	beq	.L358
	bhi	.L359
	ldr	r3, .L482+24
	cmp	r1, r3
	beq	.L360
	adds	r3, r3, #2
	cmp	r1, r3
	bne	.L356
	movs	r2, #136
	mov	r1, r6
	addw	r0, r0, #4012
	bl	memcpy
	b	.L473
.L359:
	ldr	r3, .L482+28
	cmp	r1, r3
	beq	.L362
	adds	r3, r3, #253
	cmp	r1, r3
	beq	.L363
	subw	r3, r3, #767
	cmp	r1, r3
	bne	.L356
	movs	r3, #0
	str	r3, [sp]
	movw	r3, #5300
	ldr	r3, [r0, r3]
	str	r3, [sp, #4]
	ldr	r3, [r2]
	cbz	r3, .L380
	mov	r0, sp
	bl	mpp_enc_ref_cfg_init
.L380:
	ldr	r3, [sp, #4]
	cbnz	r3, .L381
	add	r0, sp, #4
	bl	mpp_enc_ref_cfg_init
	movw	r3, #5300
	ldr	r2, [sp, #4]
	str	r2, [r4, r3]
.L381:
	ldr	r1, [r6]
	cmp	r1, #5
	bhi	.L382
	tbh	[pc, r1, lsl #1]
.L384:
	.2byte	(.L387-.L384)/2
	.2byte	(.L386-.L384)/2
	.2byte	(.L386-.L384)/2
	.2byte	(.L386-.L384)/2
	.2byte	(.L385-.L384)/2
	.2byte	(.L383-.L384)/2
	.p2align 1
.L354:
	ldr	r5, [r2]
	movw	r1, #755
	ldr	r0, .L482+32
	bl	printk
	cbz	r5, .L365
	lsls	r5, r5, #31
	itt	mi
	ldrmi	r3, [r6, #4]
	strmi	r3, [r4, #604]
	movs	r3, #0
	str	r3, [r6]
.L365:
	ldr	r3, [r6, #104]
	cbz	r3, .L367
	add	r1, r6, #104
	add	r0, r4, #704
	bl	mpp_enc_proc_rc_cfg
	movs	r3, #0
	str	r3, [r6, #104]
.L367:
	ldr	r3, [r6, #300]
	cbz	r3, .L368
	add	r1, r6, #300
	add	r0, r4, #900
	bl	mpp_enc_proc_hw_cfg
	movs	r3, #0
	str	r3, [r6, #300]
.L368:
	add	r5, r6, #4704
	ldr	r3, [r5]
	cbz	r3, .L369
	add	r0, r4, #5280
	mov	r1, r5
	adds	r0, r0, #24
	bl	mpp_enc_proc_tune_cfg
	movs	r3, #0
	str	r3, [r5]
.L369:
	ldr	r1, .L482+36
	mov	r2, r6
.L475:
	ldr	r0, [r4, #4]
	bl	enc_impl_proc_cfg
	b	.L476
.L355:
	cbnz	r2, .L371
.L473:
	movs	r5, #0
	b	.L370
.L371:
	mov	r1, r2
	add	r0, r0, #704
	bl	mpp_enc_proc_rc_cfg
.L476:
	mov	r5, r0
	b	.L370
.L352:
	ldr	r3, [r0, #468]
	adds	r3, r3, #1
	str	r3, [r0, #468]
	b	.L473
.L373:
	ldr	r1, [r4, #404]
	mov	r0, r6
	bl	mpp_packet_copy
	b	.L374
.L360:
	movw	r1, #823
	ldr	r0, .L482+40
	bl	printk
	b	.L473
.L358:
	cbz	r2, .L375
	ldr	r3, [r2]
	cmp	r3, #1
	bhi	.L376
	ldr	r2, .L482+44
	str	r3, [r0, #420]
	ldr	r2, [r2]
	lsls	r1, r2, #30
	bpl	.L473
	ldr	r1, .L482+48
	movw	r2, #831
	ldr	r0, .L482+52
.L480:
	bl	printk
	b	.L473
.L376:
	ldr	r1, .L482+48
	movw	r2, #833
	ldr	r0, .L482+56
.L477:
	bl	printk
.L474:
	mov	r5, #-1
.L370:
	ldr	r3, [r4, #1168]
	bic	r8, r7, #4
	sub	r2, r3, #8
	cmp	r2, #1
	bls	.L398
	ldr	r2, .L482+60
	cmp	r8, r2
	beq	.L420
	adds	r2, r2, #6
	cmp	r7, r2
	beq	.L420
	subs	r2, r2, #4
	cmp	r7, r2
	bne	.L400
	ldr	r3, [r6]
	tst	r3, #225
	beq	.L401
	movs	r3, #2
	b	.L399
.L375:
	movw	r2, #837
.L481:
	ldr	r1, .L482+48
	ldr	r0, .L482+64
.L479:
	bl	printk
	b	.L474
.L350:
	cbz	r2, .L378
	ldr	r3, [r2]
	cmp	r3, #2
	bhi	.L379
	ldr	r2, .L482+44
	str	r3, [r0, #424]
	ldr	r2, [r2]
	lsls	r2, r2, #30
	bpl	.L473
	movw	r2, #847
	ldr	r1, .L482+48
	ldr	r0, .L482+68
	b	.L480
.L379:
	movw	r2, #849
	ldr	r1, .L482+48
	ldr	r0, .L482+72
	b	.L477
.L378:
	movw	r2, #853
	b	.L481
.L387:
	bl	mpp_enc_ref_default
	str	r0, [sp]
.L388:
	ldr	r1, [sp]
	cmp	r1, #0
	beq	.L417
	ldr	r0, [sp, #4]
	bl	mpp_enc_ref_cfg_copy
	mov	r3, r0
	cbz	r0, .L390
	movw	r2, #641
	ldr	r1, .L482+76
	ldr	r0, .L482+80
	bl	printk
.L390:
	ldr	r1, [sp, #4]
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_set_cfg
	mov	r5, r0
	cbz	r0, .L391
	mov	r3, r0
	movw	r2, #645
	ldr	r1, .L482+76
	ldr	r0, .L482+84
	bl	printk
.L391:
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_update_hdr
	cbz	r0, .L392
	movs	r3, #0
	str	r3, [r4, #416]
.L392:
	ldr	r3, [r6]
	cbz	r3, .L389
	ldr	r3, [sp]
	cbz	r3, .L389
	mov	r0, sp
	bl	mpp_enc_ref_cfg_deinit
.L389:
	add	r3, r4, #5248
	add	r1, r6, #24
	adds	r3, r3, #28
	mov	r2, r6
.L393:
	ldr	r0, [r2], #4	@ unaligned
	cmp	r2, r1
	str	r0, [r3], #4	@ unaligned
	bne	.L393
	b	.L370
.L483:
	.align	2
.L482:
	.word	3276815
	.word	3276813
	.word	3276814
	.word	.LC49
	.word	.LC50
	.word	3309569
	.word	3276817
	.word	3310595
	.word	.LC48
	.word	3276801
	.word	.LC51
	.word	mpp_enc_debug
	.word	.LANCHOR0+197
	.word	.LC52
	.word	.LC53
	.word	3276803
	.word	.LC54
	.word	.LC55
	.word	.LC56
	.word	.LANCHOR0+214
	.word	.LC58
	.word	.LC59
.L386:
	ldr	r0, [sp]
	bl	mpi_enc_gen_ref_cfg
	b	.L388
.L385:
	mov	r1, r6
	ldr	r0, [sp]
	bl	mpi_enc_gen_smart_gop_ref_cfg
	b	.L388
.L383:
	mov	r1, r6
	ldr	r0, [sp]
	bl	mpi_enc_gen_hir_skip_ref
	b	.L388
.L382:
	movw	r1, #635
	ldr	r0, .L484
	bl	printk
	b	.L388
.L417:
	mov	r5, r1
	b	.L389
.L362:
	add	r5, r0, #4128
	adds	r5, r5, #20
	mov	r0, r5
	bl	mpp_enc_unref_osd_buf
	mov	r2, #1128
	mov	r1, r6
	mov	r0, r5
	bl	memcpy
	mov	r0, r5
	bl	mpp_enc_proc_export_osd_buf
	b	.L473
.L363:
	movw	r3, #6626
	ldrb	r3, [r0, r3]	@ zero_extendqisi2
	cbnz	r3, .L394
	mov	r1, #584
	ldr	r0, .L484+4
	bl	printk
	b	.L474
.L394:
	ldr	r5, [r2]
	movw	r3, #6625
	ldrb	r8, [r0, r3]	@ zero_extendqisi2
	cmp	r5, #1024
	and	r8, r8, #3
	bls	.L395
	mov	r2, r5
	movw	r1, #591
	ldr	r0, .L484+8
	b	.L479
.L395:
	mov	r3, sp
	bic	r3, r3, #8160
	add	r9, r0, r8, lsl #10
	bic	r3, r3, #31
	add	r9, r9, #6624
	ldr	r1, [r2, #4]
	ldr	r3, [r3, #8]
	add	r9, r9, #7
	.syntax unified
@ 157 "./include/linux/uaccess.h" 1
	.syntax unified
adds r2, r1, r5; sbcscc r2, r2, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r2, r5
	cbnz	r3, .L396
	mov	r0, r9
	bl	arm_copy_from_user
	mov	r2, r0
.L396:
	cmp	r2, #0
	beq	.L397
	subs	r0, r5, r2
	movs	r1, #0
	add	r0, r0, r9
	mvn	r5, #13
	bl	memset
	b	.L370
.L400:
	ldr	r2, .L484+12
	cmp	r7, r2
	bne	.L402
	ldr	r2, [r4, #608]
	tst	r2, #21
	bne	.L422
	ldr	r2, [r4, #704]
	tst	r2, #225
	bne	.L423
	cmp	r3, #7
	beq	.L404
	ldr	r2, .L484+16
	cmp	r3, r2
	beq	.L404
.L398:
	ldr	r3, .L484+20
	cmp	r7, r3
	beq	.L401
	ldr	r3, .L484+24
	cmp	r7, r3
	beq	.L401
.L402:
	ldr	r3, .L484+28
	cmp	r7, r3
	beq	.L401
	sub	r3, r3, #33280
	cmp	r7, r3
	bne	.L471
.L406:
	ldr	r3, [r4, #608]
	tst	r3, #5
	beq	.L469
.L401:
	movs	r3, #1
	str	r3, [r4, #452]
	ldr	r3, .L484+20
	cmp	r7, r3
	beq	.L408
	ldr	r3, .L484+12
	cmp	r7, r3
	beq	.L408
.L411:
	ldr	r3, .L484+24
	cmp	r8, r3
	bne	.L472
.L414:
	movs	r3, #0
	str	r3, [r4, #56]
.L349:
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L404:
	ldr	r3, [r4, #1172]
	cmp	r3, #0
	beq	.L406
	movs	r3, #5
	b	.L399
.L420:
	movs	r3, #1
.L399:
	ldr	r2, .L484+32
	movw	r1, #271
	ldr	r0, .L484+36
	add	r3, r2, r3, lsl #2
	ldr	r2, [r3, #236]
	bl	printk
	ldr	r3, [r4, #464]
	orr	r3, r3, #1
	str	r3, [r4, #464]
	movs	r3, #0
	str	r3, [r4, #416]
	b	.L398
.L422:
	movs	r3, #3
	b	.L399
.L423:
	movs	r3, #4
	b	.L399
.L469:
	ldr	r3, [r4, #704]
	bics	r3, r3, #2
	bne	.L401
.L408:
	ldr	r3, [r4, #704]
	lsls	r3, r3, #24
	bpl	.L413
	ldr	r1, [r4, #752]
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_set_rc_igop
.L413:
	ldr	r3, .L484+12
	cmp	r8, r3
	bne	.L411
	b	.L414
.L472:
	ldr	r3, .L484+28
	cmp	r7, r3
.L478:
	bne	.L349
	b	.L414
.L397:
	ldr	r0, [r6]
	add	r1, r4, r8
	movw	r3, #6627
	add	r8, r8, #1
	strb	r0, [r1, r3]
	movw	r3, #6625
	movw	r1, #6626
	strb	r8, [r4, r3]
	ldrb	r3, [r4, r1]	@ zero_extendqisi2
	subs	r3, r3, #1
	strb	r3, [r4, r1]
	b	.L473
.L471:
	ldr	r3, .L484+24
	cmp	r8, r3
	b	.L478
.L485:
	.align	2
.L484:
	.word	.LC57
	.word	.LC60
	.word	.LC61
	.word	3276801
	.word	16777220
	.word	3276805
	.word	3276803
	.word	3310081
	.word	.LANCHOR0
	.word	.LC62
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
	@ args = 0, pretend = 0, frame = 256
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	ldr	r5, [r0, #452]
	.pad #300
	sub	sp, sp, #300
	cmp	r5, #0
	beq	.L486
	ldr	r6, .L552
	ldr	r3, [r6]
	lsls	r1, r3, #26
	bpl	.L488
	movw	r2, #1077
	ldr	r1, .L552+4
	ldr	r0, .L552+8
	bl	printk
.L488:
	movs	r2, #244
	movs	r1, #0
	add	r0, sp, #52
	bl	memset
	ldrd	r9, r8, [r4, #732]
	movw	r3, #5300
	ldr	r7, [r4, r3]
	cmp	r8, #0
	beq	.L519
	mov	r1, r8
	mov	r0, r9
	bl	__aeabi_idiv
	mov	r1, r0
	cmp	r0, #0
	beq	.L520
.L489:
	ldr	r0, [r4, #752]
	bl	__aeabi_idiv
	lsls	r5, r0, #2
	cmp	r5, #8
	it	ge
	movge	r5, #8
.L490:
	ldr	r3, [r4, #612]
	str	r3, [sp, #44]
	ldr	r3, [r4, #616]
	str	r3, [sp, #48]
	ldr	r3, [r4, #708]
	cmp	r3, #1
	beq	.L493
	cbz	r3, .L492
	cmp	r3, #2
	it	ne
	movne	r3, #3
.L493:
	str	r3, [sp, #52]
.L492:
	ldr	r3, [r4, #728]
	add	ip, sp, #272
	ldr	lr, [r4, #744]
	str	r8, [sp, #64]
	strd	r3, r9, [sp, #56]
	add	r9, r4, #900
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
	ldr	r3, [r4, #716]
	str	r3, [sp, #96]
	ldr	r3, [r4, #720]
	str	r3, [sp, #100]
	ldr	r3, [r4, #724]
	str	r3, [sp, #92]
	ldr	r3, [r4, #864]
	str	r3, [sp, #252]
	add	r3, r4, #884
	mov	r8, r3
.L495:
	ldr	r0, [r3]	@ unaligned
	adds	r3, r3, #8
	ldr	r1, [r3, #-4]	@ unaligned
	cmp	r3, r9
	mov	r2, ip
	stmia	r2!, {r0, r1}
	mov	ip, r2
	bne	.L495
	add	r3, r4, #868
	add	ip, sp, #256
.L496:
	ldr	r0, [r3]	@ unaligned
	adds	r3, r3, #8
	ldr	r1, [r3, #-4]	@ unaligned
	cmp	r3, r8
	mov	r2, ip
	stmia	r2!, {r0, r1}
	mov	ip, r2
	bne	.L496
	cmp	lr, #0
	bne	.L497
	movw	r1, #995
	ldr	r3, .L552+12
	str	r1, [sp]
	ldr	r2, .L552+16
	ldr	r0, .L552+20
	bl	printk
.L497:
	ldr	r3, [r4, #764]
	cbz	r3, .L550
	cmp	r3, #60
	it	ge
	movge	r3, #60
	mov	r5, r3
.L550:
	cmp	r5, #1
	ldr	r3, [r4, #1168]
	it	lt
	movlt	r5, #1
	cmp	r3, #8
	str	r5, [sp, #104]
	beq	.L499
	bhi	.L500
	cmp	r3, #7
.L551:
	bne	.L502
.L501:
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
.L505:
	movs	r3, #0
	mov	r2, #256
	strd	r3, r3, [sp, #128]
	strd	r2, r3, [sp, #120]
	ldr	r3, [r4, #760]
	str	r3, [sp, #196]
	ldr	r3, [r4, #768]
	str	r3, [sp, #216]
	ldr	r3, [r4, #772]
	str	r3, [sp, #220]
	ldr	r3, [r4, #776]
	str	r3, [sp, #224]
	ldr	r3, [r4, #792]
	str	r3, [sp, #240]
	ldr	r3, [r4, #780]
	str	r3, [sp, #228]
	ldr	r3, [r4, #784]
	str	r3, [sp, #232]
	ldr	r3, [r4, #788]
	str	r3, [sp, #236]
	ldr	r3, [r4, #796]
	str	r3, [sp, #244]
	ldr	r3, [r4, #800]
	str	r3, [sp, #248]
	ldr	r5, [r7, #64]
	cbz	r5, .L507
	ldrd	r0, r1, [r4, #744]
	str	r5, [sp, #88]
	bl	__aeabi_idiv
	cmp	r5, r0
	blt	.L507
	ldr	r3, [sp, #84]
	cmp	r5, r3
	bge	.L507
	movs	r3, #1
	str	r3, [sp, #80]
	ldr	r3, [sp, #160]
	cbnz	r3, .L507
	movs	r3, #2
	str	r3, [sp, #160]
.L507:
	ldr	r3, [r4, #1168]
	ldr	r7, .L552+24
	cmp	r3, #7
	beq	.L509
	ldr	r2, .L552+28
	cmp	r3, r2
	bne	.L510
.L509:
	ldr	r2, .L552+32
	ldr	r3, .L552+36
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
	ldr	r0, .L552+40
	str	r3, [sp, #16]
	ldr	r3, [sp, #60]
	strd	r1, r3, [sp, #8]
	movw	r1, #1062
	ldr	r3, [r4, #720]
	str	r3, [sp, #4]
	ldr	r3, [r4, #716]
	str	r3, [sp]
	ldr	r3, [r4, #724]
	bl	printk
.L510:
	movw	r3, #5332
	ldr	r3, [r4, r3]
	cbnz	r3, .L513
	movw	r3, #5336
	ldr	r3, [r4, r3]
	cbz	r3, .L514
.L513:
	movs	r3, #1
	str	r3, [sp, #288]
.L514:
	movw	r3, #10792
	ldr	r3, [r4, r3]
	cbz	r3, .L515
	movs	r3, #1
	str	r3, [sp, #292]
.L515:
	add	r1, sp, #44
	ldr	r0, [r4, #60]
	bl	rc_update_usr_cfg
	movs	r3, #0
	str	r3, [r4, #704]
	str	r3, [r4, #608]
	ldr	r3, [r6]
	lsls	r3, r3, #26
	mov	r5, r0
	bpl	.L516
	movw	r2, #1091
	ldr	r1, .L552+4
	ldr	r0, .L552+44
	bl	printk
.L516:
	movs	r3, #0
	str	r3, [r4, #452]
	ldr	r3, [r4, #440]
	mov	r0, r4
	ldr	r2, [sp, #80]
	ldr	r1, .L552+48
	str	r3, [r4, #444]
	add	r3, r4, #5312
	str	r2, [r3]
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
	b	.L553
.L554:
	.align	2
.L552:
	.word	mpp_enc_debug
	.word	.LANCHOR0+260
	.word	.LC65
	.word	.LANCHOR0+283
	.word	.LC66
	.word	.LC67
	.word	.LANCHOR0
	.word	16777220
	.word	.LC64
	.word	.LC63
	.word	.LC69
	.word	.LC70
	.word	.LC18
.L553:
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
.L486:
	mov	r0, r5
	add	sp, sp, #300
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L519:
	movs	r1, #1
	b	.L489
.L520:
	movs	r5, #8
	b	.L490
.L500:
	cmp	r3, #9
	beq	.L501
	ldr	r2, .L555
	cmp	r3, r2
	b	.L551
.L499:
	ldr	r3, [r4, #1180]
	ldr	r2, [r4, #1184]
	str	r3, [sp, #136]
	ldr	r3, [r4, #1188]
	str	r2, [sp, #140]
	strd	r3, r2, [sp, #144]
	str	r3, [sp, #152]
	b	.L505
.L502:
	movw	r2, #1022
	ldr	r1, .L555+4
	ldr	r0, .L555+8
	bl	printk
	b	.L505
.L556:
	.align	2
.L555:
	.word	16777220
	.word	.LANCHOR0+294
	.word	.LC68
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
	bne	.L557
	mov	r1, r0
	ldr	r0, [r5, #372]
	bl	mpp_packet_set_length
	ldr	r8, [r5, #396]
	ldr	r7, .L591
	cmp	r8, #0
	bne	.L559
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
	cbnz	r4, .L561
	movw	r1, #1179
	ldr	r3, .L591+4
	str	r1, [sp]
	ldr	r2, .L591+8
	ldr	r0, .L591+12
	bl	printk
.L561:
	cmp	r9, #0
	beq	.L563
	movw	r10, #5356
	ldr	r3, [r5, r10]
	cbnz	r3, .L564
	ldr	r3, [r9, #48]
	cbz	r3, .L564
	movs	r1, #2
	mov	r0, r4
	blx	r3
	str	r0, [r5, r10]
.L564:
	movw	r3, #5356
	ldr	r0, [r5, r3]
	cbz	r0, .L563
	ldr	r3, [r9, #56]
	cbz	r3, .L563
	blx	r3
	movs	r2, #32
	movs	r1, #0
	mov	r10, r0
	add	r0, sp, #24
	bl	memset
	cmp	r10, #0
	beq	.L563
	ldr	r3, .L591+4
	add	r2, sp, #20
	add	r1, sp, #24
	movs	r0, #0
	str	r10, [sp, #36]
	str	r3, [sp]
	ldr	r3, .L591+16
	str	r4, [sp, #28]
	bl	mpp_buffer_import_with_tag
	ldr	r3, [r9, #16]
	cbz	r3, .L563
	mov	r0, r10
	blx	r3
.L563:
	ldr	r0, [sp, #20]
	cbnz	r0, .L570
	ldr	r9, .L591+4
	mov	r2, r4
	ldr	r3, .L591+16
	add	r1, sp, #20
	str	r9, [sp]
	bl	mpp_buffer_get_with_tag
	ldr	r3, [sp, #20]
	cbnz	r3, .L570
	movw	r1, #1203
	mov	r3, r9
	str	r1, [sp]
	ldr	r2, .L591+20
	ldr	r0, .L591+12
	bl	printk
.L570:
	ldr	r0, [sp, #20]
	add	r3, r8, #72
	ldr	r1, .L591+4
	movs	r4, #0
	str	r3, [r5, #396]
	str	r0, [r8, #72]
	bl	mpp_buffer_get_mpi_buf_id_with_caller
	ldr	r1, .L591+4
	str	r4, [r8, #84]
	str	r0, [r8, #76]
	ldr	r0, [sp, #20]
	bl	mpp_buffer_get_size_with_caller
	ldr	r1, .L591+4
	str	r0, [r8, #96]
	ldr	r0, [sp, #20]
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r1, .L591+4
	strd	r0, r0, [r8, #24]
	ldr	r0, [sp, #20]
	bl	mpp_buffer_get_size_with_caller
	ldr	r2, [r7]
	lsls	r2, r2, #26
	ldr	r3, [sp, #20]
	strd	r0, r4, [r8, #32]
	str	r3, [r8, #68]
	bpl	.L557
	str	r3, [sp]
	movw	r2, #1217
	ldr	r3, [r5, #372]
	ldr	r1, .L591+24
	ldr	r0, .L591+28
	bl	printk
.L557:
	mov	r0, r6
	add	sp, sp, #56
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L559:
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L557
	ldr	r4, [r5, #372]
	mov	r0, r4
	bl	mpp_packet_get_pos
	mov	r7, r0
	ldr	r0, [r5, #372]
	bl	mpp_packet_get_length
	mov	r3, r4
	movw	r2, #1222
	ldr	r1, .L591+24
	strd	r7, r0, [sp, #4]
	str	r8, [sp]
	ldr	r0, .L591+32
	bl	printk
	b	.L557
.L592:
	.align	2
.L591:
	.word	mpp_enc_debug
	.word	.LANCHOR0+324
	.word	.LC71
	.word	.LC67
	.word	.LC42
	.word	.LC72
	.word	.LANCHOR0+358
	.word	.LC73
	.word	.LC74
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
	bne	.L605
	ldr	r6, [r6, #4]
	cmp	r6, #1
	beq	.L595
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_next_frm_is_kpfrm
	adds	r6, r0, #0
	it	ne
	movne	r6, #1
.L595:
	movw	r3, #10728
	ldr	r3, [r4, r3]
	cbz	r3, .L597
	ldr	r3, [r3, #28]
	cbnz	r3, .L597
	bl	get_vsm_ops
	cbnz	r0, .L597
	movw	r3, #10732
	ldr	r2, [r4, r3]
	cbnz	r2, .L599
	str	r5, [r4, r3]
.L599:
	ldr	r2, [r4, r3]
	cmp	r2, #16384
	it	cc
	movcc	r2, #16384
	addw	r2, r2, #4095
	bic	r2, r2, #4080
	bic	r2, r2, #15
	str	r2, [r4, r3]
	movw	r3, #10756
	ldr	r3, [r4, r3]
	ldr	r0, [r3, #12]
	cbz	r0, .L600
	str	r0, [sp, #12]
.L601:
	movw	r3, #10736
	ldr	r1, [sp, #12]
	ldr	r2, [r4, r3]
	movw	r3, #10728
	ldr	r0, [r4, r3]
	bl	ring_buf_init
.L597:
	movw	r3, #10788
	movw	r1, #10728
	ldr	r3, [r4, r3]
	movs	r2, #0
	ldr	r1, [r4, r1]
	add	r0, r4, #372
	str	r3, [sp]
	mov	r3, r6
	bl	mpp_packet_new_ring_buf
	cbz	r0, .L603
	adds	r3, r0, #3
	ite	eq
	movweq	r2, #10740
	movwne	r2, #10744
	ldr	r3, [r4, r2]
	adds	r3, r3, #1
	str	r3, [r4, r2]
	b	.L593
.L605:
	movs	r6, #1
	b	.L595
.L600:
	ldr	r3, .L611
	add	r1, sp, #12
	str	r3, [sp]
	ldr	r3, .L611+4
	bl	mpp_ring_buffer_get_with_tag
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne	.L601
	movw	r1, #1262
	ldr	r0, .L611+8
	bl	printk
	mov	r0, #-1
.L593:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L603:
	ldr	r3, [r4, #372]
	mov	r0, r4
	adds	r3, r3, #72
	str	r3, [r4, #396]
	bl	mpp_enc_check_frm_pkt
	b	.L593
.L612:
	.align	2
.L611:
	.word	.LANCHOR0+392
	.word	.LC42
	.word	.LC75
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
	movs	r1, #132
	mov	r4, r0
	ldr	r0, .L616
	bl	mpp_osal_calloc
	str	r0, [r4, #456]
	cbz	r0, .L615
	bl	mpp_time
	strd	r0, [r4, #40]
	bl	mpp_time
	strd	r0, [r4, #24]
	bl	mpp_time
	strd	r0, [r4, #32]
	movs	r0, #0
.L613:
	pop	{r4, pc}
.L615:
	mov	r0, #-1
	b	.L613
.L617:
	.align	2
.L616:
	.word	.LANCHOR0+426
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
	cbz	r1, .L619
	ldr	r0, .L623
	bl	mpp_osal_free
	movs	r3, #0
	str	r3, [r4, #456]
.L619:
	movs	r0, #0
	pop	{r4, pc}
.L624:
	.align	2
.L623:
	.word	.LANCHOR0+450
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
	ldr	r3, [r0, #4012]
	mov	r6, r1
	cbz	r3, .L626
	addw	r1, r0, #4012
	add	r0, r0, #5344
	movs	r2, #136
	adds	r0, r0, #16
	bl	memcpy
	movs	r3, #0
	str	r3, [r4, #4012]
.L626:
	movw	r7, #4148
	add	r8, r4, r7
	ldr	r3, [r4, r7]
	cbz	r3, .L627
	add	r5, r4, #5472
	adds	r5, r5, #24
	mov	r0, r5
	bl	mpp_enc_unref_osd_buf
	mov	r2, #1128
	mov	r1, r8
	mov	r0, r5
	bl	memcpy
	movs	r3, #0
	str	r3, [r4, r7]
.L627:
	cbz	r6, .L629
	movw	r3, #5360
	adds	r1, r4, r3
	ldr	r3, [r4, r3]
	cbz	r3, .L630
	mov	r0, r6
	bl	mpp_frame_add_roi
.L630:
	movw	r3, #5496
	adds	r1, r4, r3
	ldr	r3, [r4, r3]
	cbz	r3, .L629
	mov	r0, r6
	bl	mpp_frame_add_osd
.L629:
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
	add	r7, r0, #64
	add	r5, r6, #20
	ldrb	r3, [r6, #11]	@ zero_extendqisi2
	ands	r3, r3, #1
	beq	.L645
	ldr	r6, .L821
	ldr	r8, [r0, #8]
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L646
	movw	r2, #1537
	ldr	r1, .L821+4
	ldr	r0, .L821+8
	bl	printk
.L646:
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
	bpl	.L647
	ldrh	r3, [r4, #222]
	movw	r2, #1540
	ldr	r1, .L821+4
	ldr	r0, .L821+12
	bl	printk
.L647:
	mov	r1, r5
	ldr	r0, [r4, #4]
	bl	enc_impl_proc_hal
	mov	r9, r0
	cbz	r0, .L648
	str	r0, [sp]
	movw	r3, #1541
	ldr	r0, .L821+16
	mov	r2, r4
	mov	r1, r3
.L820:
	bl	printk
	mov	r9, #0
	b	.L644
.L648:
	ldr	r3, [r6]
	lsls	r2, r3, #26
	bpl	.L650
	ldrh	r3, [r4, #222]
	movw	r2, #1543
	ldr	r1, .L821+4
	ldr	r0, .L821+20
	bl	printk
.L650:
	mov	r1, r5
	mov	r0, r8
	bl	mpp_enc_hal_get_task
	cbz	r0, .L651
	mov	r3, #1544
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L821+24
	bl	printk
.L644:
	mov	r0, r9
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L651:
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L652
	ldrh	r3, [r4, #222]
	movw	r2, #1546
	ldr	r1, .L821+4
	ldr	r0, .L821+28
	bl	printk
.L652:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_hal_start
	cbz	r0, .L653
	movw	r3, #1547
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L821+32
	b	.L820
.L653:
	ldr	r3, [r6]
	lsls	r7, r3, #26
	bpl	.L654
	ldrh	r3, [r4, #222]
	movw	r2, #1549
	ldr	r1, .L821+4
	ldr	r0, .L821+36
	bl	printk
.L654:
	mov	r1, r5
	mov	r0, r8
	bl	mpp_enc_hal_gen_regs
	mov	r9, r0
	cbz	r0, .L655
	movw	r3, #1550
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L821+40
	b	.L820
.L655:
	ldr	r3, [r6]
	lsls	r5, r3, #26
	bpl	.L656
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	movw	r2, #1553
	ldr	r1, .L821+4
	ldr	r0, .L821+44
	str	r3, [sp, #4]
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	ubfx	r3, r3, #2, #1
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
.L656:
	ldr	r3, [r6]
	lsls	r4, r3, #31
	bpl	.L644
	movw	r2, #1554
	ldr	r1, .L821+4
	ldr	r0, .L821+48
	bl	printk
	b	.L644
.L645:
	strd	r1, r3, [r0, #368]
	cbz	r1, .L657
	mov	r0, r1
	bl	mpp_frame_get_hor_stride
	mov	r9, r0
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_ver_stride
	mov	r10, r0
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_width
	mov	r8, r0
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_height
	ldr	r2, [r4, #656]
	bic	r2, r2, #2
	cmp	r2, #1
	it	eq
	moveq	r2, r8
	mov	r3, r0
	it	eq
	moveq	r8, r0
	ldrd	r0, ip, [r4, #620]
	it	eq
	moveq	r3, r2
	ldrd	r2, r1, [r4, #612]
	cmp	r9, r0
	bne	.L659
	cmp	r10, ip
	bne	.L659
	cmp	r2, r8
	bhi	.L659
	cmp	r1, r3
	bls	.L657
.L659:
	strd	r2, r1, [sp, #8]
	mov	r2, r8
	strd	r0, ip, [sp, #16]
	movw	r1, #1708
	strd	r9, r10, [sp]
	ldr	r0, .L821+52
	bl	printk
.L818:
	mov	r9, #-1
.L660:
	mov	r1, r6
	mov	r0, r4
	bl	mpp_enc_terminate_task
	b	.L644
.L657:
	movs	r2, #112
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
	ldr	r3, .L821
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #32
	beq	.L661
	ldrh	r3, [r4, #222]
	movw	r2, #1749
	ldr	r1, .L821+56
	ldr	r0, .L821+60
	bl	printk
.L661:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_frm_check_drop
	mov	r9, r0
	cbz	r0, .L662
	str	r0, [sp]
	movw	r3, #1750
	ldr	r0, .L821+64
	mov	r2, r4
	mov	r1, r3
.L817:
	bl	printk
	b	.L660
.L662:
	ldrb	r3, [r6, #8]	@ zero_extendqisi2
	orr	r3, r3, #16
	strb	r3, [r6, #8]
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L663
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	mov	r2, #1752
	ldr	r1, .L821+56
	and	r3, r3, #1
	ldr	r0, .L821+68
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
.L663:
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	lsls	r1, r3, #31
	bpl	.L664
	movs	r3, #0
	str	r3, [r6, #20]
	str	r3, [r6, #68]
	b	.L818
.L822:
	.align	2
.L821:
	.word	mpp_enc_debug
	.word	.LANCHOR0+473
	.word	.LC76
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC77
	.word	.LC78
	.word	.LC10
	.word	.LC11
	.word	.LC79
	.word	.LC80
	.word	.LC81
	.word	.LANCHOR0+494
	.word	.LC82
	.word	.LC83
	.word	.LC84
.L664:
	movs	r3, #1
	mov	r0, r4
	str	r3, [r6, #20]
	bl	mpp_enc_alloc_output_from_ringbuf
	mov	r9, r0
	cmp	r0, #0
	bne	.L660
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
	cbnz	r3, .L665
	movw	r1, #1774
	ldr	r3, .L823
	str	r1, [sp]
	ldr	r2, .L823+4
	ldr	r0, .L823+8
	bl	printk
.L665:
	ldr	r1, [r4, #368]
	mov	r0, r4
	bl	mpp_enc_impl_get_roi_osd
	ldr	r3, [r4, #56]
	cbnz	r3, .L666
	movs	r3, #1
	str	r3, [r4, #56]
.L666:
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bmi	.L667
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
	bpl	.L668
	str	r0, [sp]
	mov	r2, #1792
	ldrh	r3, [r4, #222]
	ldr	r1, .L823+12
	ldr	r0, .L823+16
	bl	printk
.L668:
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
	bpl	.L670
	movw	r2, #1799
	ldr	r1, .L823+12
	ldr	r0, .L823+20
	bl	printk
.L667:
	ldr	r3, [r8]
	lsls	r2, r3, #26
	bpl	.L670
	movw	r2, #1801
	ldr	r1, .L823+12
	ldr	r0, .L823+24
	bl	printk
.L670:
	ldr	r1, .L823+28
	mov	r0, r5
	bl	check_hal_task_pkt_len
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L672
	ldrh	r3, [r4, #222]
	movw	r2, #1805
	ldr	r1, .L823+12
	ldr	r0, .L823+32
	bl	printk
.L672:
	mov	r1, r5
	ldr	r0, [r4, #4]
	bl	enc_impl_start
	mov	r9, r0
	cbz	r0, .L673
	movw	r3, #1806
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L823+36
	b	.L817
.L673:
	ldr	r3, [r4, #464]
	cbz	r3, .L674
	mov	r1, r10
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_set_usr_cfg
.L674:
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_stash
	ldrd	r10, r3, [r4, #4]
	ldr	fp, [r6, #48]
	str	r3, [sp, #28]
	movw	r3, #5340
	ldr	r3, [r4, r3]
	cbz	r3, .L675
	movw	r3, #10760
	ldr	r3, [r4, r3]
	str	r3, [r6, #84]
	movw	r3, #10764
	ldr	r3, [r4, r3]
	str	r3, [r6, #88]
	movw	r3, #10772
	ldr	r3, [r4, r3]
	str	r3, [r6, #92]
	movw	r3, #10776
	ldr	r3, [r4, r3]
	str	r3, [r6, #96]
	movw	r3, #10780
	ldr	r3, [r4, r3]
	str	r3, [r6, #100]
	add	r3, r4, #10752
	adds	r3, r3, #16
	str	r3, [r6, #104]
	movw	r3, #10784
	ldr	r3, [r4, r3]
	str	r3, [r6, #108]
.L675:
	movw	r3, #5332
	ldr	r3, [r4, r3]
	cbz	r3, .L676
.L679:
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L678
	ldrh	r3, [r4, #222]
	movw	r2, #1394
	ldr	r1, .L823+40
	ldr	r0, .L823+44
	bl	printk
.L678:
	mov	r1, r7
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_get_cpb
	ldr	r3, [r8]
	lsls	r1, r3, #15
	bpl	.L681
	ldrh	r3, [r4, #78]
	movw	r2, #1398
	ldr	r1, .L823+40
	ldr	r0, .L823+48
	bl	printk
.L681:
	mov	r1, r5
	mov	r0, r10
	bl	enc_impl_proc_dpb
	mov	r9, r0
	cmp	r0, #0
	beq	.L682
	str	r0, [sp]
	movw	r3, #1399
	ldr	r0, .L823+52
	mov	r2, r4
	mov	r1, r3
.L819:
	bl	printk
	movw	r2, #10748
	ldr	r3, [r4, r2]
	adds	r3, r3, #1
	str	r3, [r4, r2]
	b	.L680
.L676:
	ldr	r3, [r4, #796]
	cmp	r3, #0
	beq	.L679
	movw	r3, #5336
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bne	.L679
	ldr	r3, [r4, #464]
	cmp	r3, #0
	bne	.L679
	mov	r1, r6
	mov	r0, r4
	bl	mpp_enc_proc_two_pass
	mov	r9, r0
	cmp	r0, #0
	beq	.L679
.L680:
	movw	r3, #1812
	str	r9, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L823+56
	b	.L817
.L824:
	.align	2
.L823:
	.word	.LANCHOR0+515
	.word	.LC85
	.word	.LC67
	.word	.LANCHOR0+494
	.word	.LC86
	.word	.LC87
	.word	.LC88
	.word	.LC89
	.word	.LC90
	.word	.LC91
	.word	.LANCHOR0+536
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC107
.L682:
	ldr	r3, [r8]
	lsls	r2, r3, #15
	bpl	.L709
	ldrh	r3, [r4, #78]
	movw	r2, #1401
	ldr	r1, .L825
	ldr	r0, .L825+4
	bl	printk
	ldr	r3, [r8]
	lsls	r3, r3, #15
	bpl	.L709
	ldrh	r3, [r4, #78]
	movw	r2, #1403
	ldr	r1, .L825
	ldr	r0, .L825+8
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
	ldr	r3, [r8]
	lsls	r0, r3, #15
	bpl	.L709
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	ubfx	r3, r3, #5, #1
	ldr	r1, .L825
	ubfx	r2, r2, #5, #1
	ldr	r0, .L825+12
	str	r2, [sp]
	movw	r2, #1405
	bl	printk
	ldr	r3, [r8]
	lsls	r1, r3, #15
	bpl	.L709
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	ubfx	r3, r3, #4, #1
	ldr	r1, .L825
	ubfx	r2, r2, #4, #1
	ldr	r0, .L825+16
	str	r2, [sp]
	movw	r2, #1407
	bl	printk
	ldr	r3, [r8]
	lsls	r2, r3, #15
	bpl	.L709
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	ubfx	r3, r3, #6, #1
	ldr	r1, .L825
	ubfx	r2, r2, #6, #1
	ldr	r0, .L825+20
	str	r2, [sp]
	movw	r2, #1409
	bl	printk
	ldr	r3, [r8]
	lsls	r3, r3, #15
	bpl	.L709
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	ldrb	r2, [r4, #72]	@ zero_extendqisi2
	ldr	r1, .L825
	lsrs	r3, r3, #7
	ldr	r0, .L825+24
	lsrs	r2, r2, #7
	str	r2, [sp]
	movw	r2, #1411
	bl	printk
	ldr	r3, [r8]
	lsls	r0, r3, #15
	bpl	.L709
	ldrb	r3, [r4, #217]	@ zero_extendqisi2
	ldrb	r2, [r4, #73]	@ zero_extendqisi2
	and	r3, r3, #15
	ldr	r1, .L825
	and	r2, r2, #15
	ldr	r0, .L825+28
	str	r2, [sp]
	movw	r2, #1413
	bl	printk
	ldr	r3, [r8]
	lsls	r1, r3, #15
	bpl	.L709
	ldrb	r3, [r4, #217]	@ zero_extendqisi2
	ldrb	r2, [r4, #73]	@ zero_extendqisi2
	ldr	r1, .L825
	lsrs	r3, r3, #4
	ldr	r0, .L825+32
	lsrs	r2, r2, #4
	str	r2, [sp]
	movw	r2, #1415
	bl	printk
	ldr	r3, [r8]
	lsls	r2, r3, #15
	bpl	.L709
	ldrh	r3, [r4, #78]
	movw	r2, #1417
	ldr	r1, .L825
	ldr	r0, .L825+36
	bl	printk
.L709:
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L692
	ldrh	r3, [r4, #222]
	movw	r2, #1419
	ldr	r1, .L825
	ldr	r0, .L825+40
	bl	printk
.L692:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_frm_start
	mov	r9, r0
	cbz	r0, .L693
	movw	r3, #1420
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L825+44
	b	.L819
.L693:
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L694
	ldrh	r3, [r4, #222]
	movw	r2, #1421
	ldr	r1, .L825
	ldr	r0, .L825+48
	bl	printk
.L694:
	ldr	r3, [r4, #420]
	cmp	r3, #1
	bne	.L695
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	lsls	r1, r3, #27
	bpl	.L695
	ldrb	r3, [r4, #416]	@ zero_extendqisi2
	tst	r3, #14
	bne	.L695
	ldr	r3, [r8]
	lsls	r2, r3, #26
	bpl	.L696
	ldr	r3, [r4, #412]
	movw	r2, #1428
	ldr	r1, .L825
	ldr	r0, .L825+52
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
.L696:
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
.L695:
	ldr	r1, .L825+56
	mov	r0, r5
	bl	check_hal_task_pkt_len
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	lsls	r3, r3, #26
	bpl	.L697
	ldr	r3, [r4, #424]
	cbz	r3, .L697
	ldr	r3, [r4, #432]
	add	r2, sp, #44
	mov	r1, fp
	mov	r0, r10
	mov	r9, #0
	str	r9, [sp, #44]
	str	r3, [sp, #4]
	ldr	r3, [r4, #428]
	str	r3, [sp]
	ldr	r3, .L825+60
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
	ldr	r3, .L825+64
	bl	enc_impl_add_prefix
	ldr	r2, [r6, #60]
	ldr	r3, [r6, #68]
	ldr	r1, [sp, #44]
	add	r2, r2, r1
	add	r3, r3, r1
	str	r2, [r6, #60]
	str	r3, [r6, #68]
.L697:
	movw	r3, #6626
	ldrb	r2, [r4, r3]	@ zero_extendqisi2
	cmp	r2, #3
	bhi	.L701
	add	r2, r4, #6624
	adds	r3, r4, r3
	str	r2, [sp, #32]
	str	r3, [sp, #36]
.L698:
	ldr	r3, [sp, #32]
	mov	r1, fp
	ldrb	r9, [r3]	@ zero_extendqisi2
	movs	r3, #0
	str	r3, [sp, #44]
	movw	r3, #6627
	and	r9, r9, #3
	add	r2, r4, r9
	ldrb	r3, [r2, r3]	@ zero_extendqisi2
	add	r2, sp, #44
	str	r3, [sp, #4]
	add	r3, r4, r9, lsl #10
	add	r3, r3, #6624
	add	r9, r9, #1
	adds	r3, r3, #7
	str	r3, [sp]
	ldr	r3, .L825+68
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
	ldr	r2, [sp, #32]
	cmp	r3, #3
	strb	r9, [r2]
	bls	.L698
.L701:
	mov	r0, r5
	ldr	r1, .L825+72
	bl	check_hal_task_pkt_len
	ldr	r3, [r8]
	lsls	r0, r3, #26
	bpl	.L700
	ldrh	r3, [r4, #222]
	movw	r2, #1462
	ldr	r1, .L825
	ldr	r0, .L825+76
	bl	printk
.L700:
	mov	r1, r5
	mov	r0, r10
	bl	enc_impl_proc_hal
	mov	r9, r0
	cbz	r0, .L702
	movw	r3, #1463
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L825+80
	b	.L819
.L826:
	.align	2
.L825:
	.word	.LANCHOR0+536
	.word	.LC92
	.word	.LC93
	.word	.LC94
	.word	.LC95
	.word	.LC96
	.word	.LC97
	.word	.LC98
	.word	.LC99
	.word	.LC100
	.word	.LC101
	.word	.LC102
	.word	.LC103
	.word	.LC104
	.word	.LC105
	.word	.LANCHOR1
	.word	.LANCHOR1+16
	.word	.LANCHOR1+32
	.word	.LC106
	.word	.LC6
	.word	.LC7
.L702:
	ldr	r3, [r8]
	lsls	r1, r3, #26
	bpl	.L703
	ldrh	r3, [r4, #222]
	movw	r2, #1465
	ldr	r1, .L827
	ldr	r0, .L827+4
	bl	printk
.L703:
	mov	r1, r5
	ldr	r0, [sp, #28]
	bl	mpp_enc_hal_get_task
	mov	r9, r0
	cbz	r0, .L704
	movw	r3, #1466
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L827+8
	b	.L819
.L704:
	ldr	r3, [r8]
	lsls	r2, r3, #26
	bpl	.L705
	ldrh	r3, [r4, #222]
	movw	r2, #1468
	ldr	r1, .L827
	ldr	r0, .L827+12
	bl	printk
.L705:
	mov	r1, r7
	ldr	r0, [r4, #60]
	bl	rc_hal_start
	mov	r9, r0
	cbz	r0, .L706
	movw	r3, #1469
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L827+16
	b	.L819
.L706:
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L707
	ldrh	r3, [r4, #222]
	movw	r2, #1471
	ldr	r1, .L827
	ldr	r0, .L827+20
	bl	printk
.L707:
	mov	r1, r5
	ldr	r0, [sp, #28]
	bl	mpp_enc_hal_gen_regs
	mov	r9, r0
	cbz	r0, .L708
	mov	r3, #1472
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L827+24
	b	.L819
.L708:
	str	r0, [r4, #464]
	b	.L644
.L828:
	.align	2
.L827:
	.word	.LANCHOR0+536
	.word	.LC8
	.word	.LC9
	.word	.LC77
	.word	.LC78
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
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r4, r0
	ldr	r10, [r0, #456]
	mov	r5, r1
	ldr	r8, [r0, #8]
	bl	get_mpidev_ops
	add	r9, r10, #20
	mov	r7, r0
	cbz	r5, .L837
	ldr	r6, [r5, #456]
	adds	r6, r6, #20
.L830:
	ldr	r3, .L848
	ldr	r3, [r3]
	lsls	r3, r3, #26
	bpl	.L831
	ldrh	r3, [r4, #222]
	mov	r2, #1840
	ldr	r1, .L848+4
	ldr	r0, .L848+8
	bl	printk
.L831:
	mov	r2, r6
	mov	r1, r9
	mov	r0, r8
	bl	mpp_enc_hal_start
	mov	r6, r0
	cbz	r0, .L832
	movw	r3, #1842
	str	r0, [sp]
	mov	r1, r3
	mov	r2, r4
	ldr	r0, .L848+12
	bl	printk
	mov	r1, r10
	mov	r0, r4
	bl	mpp_enc_terminate_task
	cbnz	r5, .L833
.L834:
	movw	r2, #10748
	ldr	r3, [r4, r2]
	adds	r3, r3, #1
	str	r3, [r4, r2]
	b	.L829
.L837:
	mov	r6, r5
	b	.L830
.L832:
	cbz	r7, .L829
	ldr	r3, [r7, #24]
	cbz	r3, .L829
	ldr	r0, [r10, #72]
	bl	mpp_frame_get_dts
	mov	r8, r0
	ldr	r0, [r10, #72]
	mov	r9, r1
	bl	mpp_frame_get_pts
	ldr	r3, [r4, #1168]
	mov	r2, r8
	cmp	r3, #8
	itet	ne
	ldrbne	r3, [r4, #216]	@ zero_extendqisi2
	moveq	r3, #1
	ubfxne	r3, r3, #4, #1
	strd	r0, [sp]
	movw	r0, #10788
	str	r3, [sp, #8]
	mov	r3, r9
	ldr	r1, [r7, #24]
	ldr	r0, [r4, r0]
	blx	r1
.L829:
	mov	r0, r6
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L833:
	ldr	r1, [r5, #456]
	mov	r0, r5
	bl	mpp_enc_terminate_task
	b	.L834
.L849:
	.align	2
.L848:
	.word	mpp_enc_debug
	.word	.LANCHOR0+555
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
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #36
	sub	sp, sp, #36
	ldr	fp, [r0, #456]
	mov	r4, r0
	strd	r3, r2, [sp, #24]
	add	r6, r0, #64
	ldr	r5, [r0, #8]
	add	r9, fp, #20
	mov	r8, r1
	cmp	r1, #0
	beq	.L895
	ldr	r7, [r1, #456]
.L851:
	ldr	r3, .L987
	ldr	r2, [r3]
	mov	r10, r3
	tst	r2, #32
	beq	.L852
	ldrh	r3, [r4, #222]
	movw	r2, #1947
	ldr	r1, .L987+4
	ldr	r0, .L987+8
	bl	printk
.L852:
	mov	r0, r5
	mov	r1, r9
	bl	mpp_enc_hal_wait
	mov	r5, r0
	cmp	r0, #0
	beq	.L853
	str	r0, [sp]
	movw	r3, #1948
	ldr	r0, .L987+12
	mov	r2, r4
	mov	r1, r3
.L982:
	bl	printk
.L854:
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
	movw	r3, #10788
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	add	r0, r4, #372
	bl	mpp_packet_deinit
.L890:
	ldr	r3, [r4, #372]
	ldr	r2, [sp, #28]
	str	r3, [r2]
	ldr	r3, [r10]
	lsls	r2, r3, #26
	bpl	.L891
	ldrd	r2, [r4, #384]
	ldr	r1, .L987+4
	ldr	r0, .L987+16
	strd	r2, [sp]
	movw	r2, #2019
	ldrh	r3, [r4, #222]
	bl	printk
.L891:
	ldr	r3, [r4, #368]
	cbz	r3, .L892
	add	r0, r4, #368
	bl	mpp_frame_deinit
.L892:
	movw	r3, #5332
	ldr	r3, [r4, r3]
	cbnz	r3, .L893
	ldr	r3, [r4, #796]
	cbz	r3, .L893
	movw	r3, #5336
	ldr	r3, [r4, r3]
	cbnz	r3, .L893
	movs	r2, #104
	add	r1, r4, #224
	add	r0, r4, #496
	bl	memcpy
.L893:
	ldrb	r2, [r4, #416]	@ zero_extendqisi2
	movs	r3, #0
	strd	r3, r3, [r4, #368]
	strd	r3, r3, [r4, #392]
	and	r2, r2, #1
	str	r2, [r4, #416]
	str	r3, [fp, #8]
	b	.L986
.L895:
	mov	r7, r1
	b	.L851
.L853:
	ldr	r3, [r4, #8]
	str	r3, [sp, #8]
	cbz	r7, .L855
	adds	r7, r7, #20
.L855:
	ldr	r3, [r10]
	lsls	r5, r3, #26
	bpl	.L856
	ldrh	r3, [r4, #222]
	mov	r2, #1496
	ldr	r1, .L987+20
	ldr	r0, .L987+24
	bl	printk
.L856:
	mov	r1, r6
	ldr	r0, [r4, #60]
	bl	rc_hal_end
	mov	r5, r0
	cbz	r0, .L857
	str	r0, [sp]
	movw	r3, #1497
	ldr	r0, .L987+28
	mov	r2, r4
	mov	r1, r3
.L983:
	bl	printk
	movw	r3, #1949
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L987+32
	str	r5, [sp]
	b	.L982
.L857:
	ldr	r3, [r10]
	lsls	r0, r3, #26
	bpl	.L859
	ldrh	r3, [r4, #222]
	movw	r2, #1499
	ldr	r1, .L987+20
	ldr	r0, .L987+36
	bl	printk
.L859:
	mov	r2, r7
	mov	r1, r9
	ldr	r0, [sp, #8]
	bl	mpp_enc_hal_ret_task
	mov	r5, r0
	cbz	r0, .L860
	movw	r3, #1501
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L987+40
	b	.L983
.L860:
	ldr	r3, [r10]
	lsls	r1, r3, #26
	bpl	.L861
	ldrh	r3, [r4, #222]
	movw	r2, #1503
	ldr	r1, .L987+20
	ldr	r0, .L987+44
	bl	printk
.L861:
	mov	r1, r6
	ldr	r0, [r4, #60]
	bl	rc_frm_check_reenc
	mov	r5, r0
	cmp	r0, #0
	beq	.L862
	mov	r3, #1504
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L987+48
	b	.L983
.L894:
	ldrb	r2, [r4, #221]	@ zero_extendqisi2
	ldr	r3, [r4, #760]
	cmp	r2, r3
	bcs	.L863
	ldrd	r1, r2, [fp, #64]
	ldr	r3, [r10]
	subs	r2, r2, r1
	strd	r5, r2, [fp, #64]
	ldrb	r2, [fp, #11]	@ zero_extendqisi2
	and	r3, r3, #32
	orr	r2, r2, #1
	strb	r2, [fp, #11]
	movw	r2, #5332
	ldr	r2, [r4, r2]
	cbnz	r2, .L864
	movw	r2, #5336
	ldr	r2, [r4, r2]
	cbz	r2, .L865
.L864:
	cbz	r3, .L896
	movw	r2, #1957
	ldr	r1, .L987+4
	ldr	r0, .L987+52
	bl	printk
.L896:
	mov	r5, #-1
	b	.L854
.L988:
	.align	2
.L987:
	.word	mpp_enc_debug
	.word	.LANCHOR0+577
	.word	.LC21
	.word	.LC15
	.word	.LC25
	.word	.LANCHOR0+594
	.word	.LC108
	.word	.LC20
	.word	.LC116
	.word	.LC109
	.word	.LC16
	.word	.LC110
	.word	.LC111
	.word	.LC112
.L865:
	cbz	r3, .L866
	ldrb	r3, [r4, #221]	@ zero_extendqisi2
	movw	r2, #1963
	ldr	r1, .L989
	ldr	r0, .L989+4
	str	r3, [sp, #4]
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	ubfx	r3, r3, #2, #1
	str	r3, [sp]
	ldrh	r3, [r4, #222]
	bl	printk
.L866:
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bpl	.L867
	ldr	r3, [r10]
	lsls	r3, r3, #31
	bpl	.L868
	movw	r2, #1569
	ldr	r1, .L989+8
	ldr	r0, .L989+12
	bl	printk
.L868:
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_rollback
	ldr	r3, [fp, #68]
	str	r3, [r4, #248]
	ldr	r3, [r4, #236]
	str	r3, [r4, #252]
	ldr	r3, [r10]
	lsls	r7, r3, #26
	bpl	.L869
	ldrh	r3, [r4, #222]
	movw	r2, #1575
	ldr	r1, .L989+8
	ldr	r0, .L989+16
	bl	printk
.L869:
	mov	r1, r6
	ldr	r0, [r4, #60]
	bl	rc_frm_end
	cmp	r0, #0
	beq	.L870
	mov	r3, #1576
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L989+20
	bl	printk
.L871:
	ldr	r3, [r10]
	lsls	r0, r3, #31
	bpl	.L872
	movw	r2, #1580
	ldr	r1, .L989+8
	ldr	r0, .L989+24
	bl	printk
.L872:
	ldrb	r3, [fp, #11]	@ zero_extendqisi2
	bfc	r3, #0, #1
	strb	r3, [fp, #11]
.L867:
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	lsls	r1, r3, #27
	bpl	.L873
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	tst	r3, #160
	bne	.L873
	add	r3, r4, #464
	str	r3, [sp, #8]
	ldr	r3, [r10]
	ldr	r7, [r4, #4]
	lsls	r2, r3, #31
	bpl	.L874
	movw	r2, #1595
	ldr	r1, .L989+28
	ldr	r0, .L989+12
	bl	printk
.L874:
	ldr	r3, [r4, #472]
	ldr	r0, [r4, #460]
	adds	r3, r3, #1
	str	r3, [r4, #472]
	ldr	r3, [r4, #464]
	orr	r3, r3, #2
	str	r3, [r4, #464]
	bl	mpp_enc_refs_rollback
	ldr	r0, [r4, #460]
	ldr	r1, [sp, #8]
	bl	mpp_enc_refs_set_usr_cfg
	ldr	r3, [r10]
	lsls	r3, r3, #26
	bpl	.L875
	ldrh	r3, [r4, #222]
	movw	r2, #1605
	ldr	r1, .L989+28
	ldr	r0, .L989+32
	bl	printk
.L875:
	mov	r1, r6
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_get_cpb
	ldr	r3, [r10]
	lsls	r0, r3, #15
	bpl	.L876
	ldrh	r3, [r4, #78]
	movw	r2, #1609
	ldr	r1, .L989+28
	ldr	r0, .L989+36
	bl	printk
.L876:
	mov	r1, r9
	mov	r0, r7
	bl	enc_impl_proc_dpb
	cbz	r0, .L877
	str	r0, [sp]
	movw	r3, #1610
	ldr	r0, .L989+40
	mov	r2, r4
	mov	r1, r3
.L981:
	bl	printk
.L878:
	ldr	r3, [r10]
	lsls	r3, r3, #31
	bpl	.L883
	movw	r2, #1620
	ldr	r1, .L989+28
	ldr	r0, .L989+24
	bl	printk
.L883:
	ldrb	r3, [fp, #11]	@ zero_extendqisi2
	bfc	r3, #0, #1
	strb	r3, [fp, #11]
.L873:
	ldrb	r3, [fp, #11]	@ zero_extendqisi2
	lsls	r7, r3, #31
	bpl	.L863
.L986:
	cmp	r8, #0
	beq	.L850
	ldr	r1, [sp, #24]
	mov	r0, r8
	bl	mpp_enc_comb_end_jpeg
.L850:
	mov	r0, r5
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L870:
	ldr	r2, [r4, #364]
	movw	r3, #10784
	str	r2, [r4, r3]
	b	.L871
.L877:
	ldr	r3, [r10]
	lsls	r1, r3, #26
	bpl	.L879
	ldrh	r3, [r4, #222]
	movw	r2, #1612
	ldr	r1, .L989+28
	ldr	r0, .L989+44
	bl	printk
.L879:
	mov	r1, r9
	mov	r0, r7
	bl	enc_impl_sw_enc
	cbz	r0, .L880
	movw	r3, #1613
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L989+48
	b	.L981
.L880:
	ldr	r3, [r10]
	lsls	r2, r3, #26
	bpl	.L881
	ldrh	r3, [r4, #222]
	movw	r2, #1615
	ldr	r1, .L989+28
	ldr	r0, .L989+16
	bl	printk
.L881:
	mov	r1, r6
	ldr	r0, [r4, #60]
	bl	rc_frm_end
	cbz	r0, .L882
	mov	r3, #1616
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L989+20
	b	.L981
.L882:
	ldr	r2, [r4, #364]
	movw	r3, #10784
	str	r2, [r4, r3]
	b	.L878
.L886:
	ldr	r2, [r4, #364]
	movw	r3, #10784
	str	r2, [r4, r3]
	bl	mpp_time
	ldr	r3, [r4, #12]
	mov	r6, r0
	ldr	r0, [r4, #48]
	mov	r7, r1
	strd	r6, [r4, #32]
	add	ip, r0, #1
	str	ip, [r4, #48]
	cbz	r3, .L887
	ldrd	r2, [r4, #24]
	strd	r2, [sp, #8]
	orrs	r3, r2, r3
	beq	.L887
	orrs	r3, r6, r7
	beq	.L887
	ldr	r3, [sp, #8]
	ldr	r0, .L989+52
	subs	r1, r6, r3
	ldr	r3, [sp, #12]
	str	r1, [sp, #16]
	sbc	r1, r7, r3
	str	r1, [sp, #20]
	ldrd	r2, [sp, #16]
	movs	r1, #0
	cmp	r0, r2
	sbcs	r3, r1, r3
	bge	.L887
	ldr	r3, [sp, #8]
	ldr	r0, .L989+56
	subs	r2, r6, r3
	smull	r0, r1, r2, r0
	asrs	r2, r2, #31
	movw	r0, #10000
	mul	r0, r0, ip
	rsb	r1, r2, r1, asr #6
	bl	__aeabi_idiv
	movw	r3, #5316
	str	r0, [r4, r3]
	strd	r6, [r4, #24]
	str	r5, [r4, #48]
.L887:
	ldrh	r3, [r4, #220]
	ldr	r0, [r4, #372]
	and	r3, r3, #251
	strh	r3, [r4, #220]	@ movhi
	ldr	r1, [fp, #68]
	bl	mpp_packet_set_length
	ldrb	r3, [r4, #216]	@ zero_extendqisi2
	lsls	r1, r3, #27
	bpl	.L889
	movs	r1, #16
	ldr	r0, [r4, #372]
	bl	mpp_packet_set_flag
.L889:
	ldrb	r1, [r4, #217]	@ zero_extendqisi2
	ldr	r0, [r4, #372]
	lsrs	r1, r1, #4
	bl	mpp_packet_set_temporal_id
	movw	r3, #10788
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	ldr	r0, [r4, #372]
	bl	mpp_packet_ring_buf_put_used
	mov	r5, r0
	cmp	r0, #0
	beq	.L890
	movw	r2, #2010
	ldr	r1, .L989
	ldr	r0, .L989+60
	movs	r5, #0
	bl	printk
	b	.L890
.L990:
	.align	2
.L989:
	.word	.LANCHOR0+577
	.word	.LC79
	.word	.LANCHOR0+606
	.word	.LC76
	.word	.LC113
	.word	.LC23
	.word	.LC80
	.word	.LANCHOR0+625
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC114
	.word	.LC115
	.word	999999
	.word	274877907
	.word	.LC24
.L862:
	ldrb	r3, [r4, #220]	@ zero_extendqisi2
	lsls	r3, r3, #29
	bmi	.L894
.L863:
	ldr	r3, [r10]
	lsls	r0, r3, #26
	bpl	.L885
	ldrh	r3, [r4, #222]
	movw	r2, #1980
	ldr	r1, .L991
	ldr	r0, .L991+4
	bl	printk
.L885:
	mov	r1, r6
	ldr	r0, [r4, #60]
	bl	rc_frm_end
	mov	r5, r0
	cmp	r0, #0
	beq	.L886
	movw	r3, #1981
	str	r0, [sp]
	mov	r2, r4
	mov	r1, r3
	ldr	r0, .L991+8
	b	.L982
.L992:
	.align	2
.L991:
	.word	.LANCHOR0+577
	.word	.LC22
	.word	.LC23
	.fnend
	.size	mpp_enc_impl_int, .-mpp_enc_impl_int
	.align	1
	.global	mpp_enc_impl_poc_debug_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_impl_poc_debug_info, %function
mpp_enc_impl_poc_debug_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	.pad #52
	sub	sp, sp, #52
	ldr	r8, [r1, #456]
	ldr	r1, .L994
	mov	r5, r0
	mov	r6, r2
	bl	seq_puts
	ldr	r3, .L994+4
	mov	r0, r5
	ldr	r7, .L994+8
	ldr	r1, .L994+12
	mov	r2, r7
	str	r3, [sp, #20]
	ldr	r3, .L994+16
	str	r3, [sp, #16]
	ldr	r3, .L994+20
	str	r3, [sp, #12]
	ldr	r3, .L994+24
	str	r3, [sp, #8]
	ldr	r3, .L994+28
	str	r3, [sp, #4]
	ldr	r3, .L994+32
	str	r3, [sp]
	ldr	r3, .L994+36
	bl	seq_printf
	ldr	r0, [r4, #1168]
	ldrd	r9, r10, [r4, #612]
	bl	strof_coding_type
	add	r3, r4, #5312
	ldr	r8, [r8, #4]
	mov	fp, r0
	ldr	r0, [r3]
	bl	strof_gop_mode
	mov	r2, r6
	movs	r3, #0
	ldr	r1, .L994+40
	str	r8, [sp, #12]
	ldr	r8, .L994+164
	strd	r0, r3, [sp, #16]
	mov	r3, r9
	mov	r0, r5
	str	r10, [sp]
	strd	fp, r8, [sp, #4]
	bl	seq_printf
	mov	r0, r5
	ldr	r1, .L994+44
	bl	seq_puts
	ldr	r3, .L994+48
	mov	r2, r7
	mov	r0, r5
	ldr	r1, .L994+52
	str	r3, [sp, #24]
	ldr	r3, .L994+56
	str	r3, [sp, #20]
	ldr	r3, .L994+60
	str	r3, [sp, #16]
	ldr	r3, .L994+64
	str	r3, [sp, #12]
	ldr	r3, .L994+68
	str	r3, [sp, #8]
	ldr	r3, .L994+72
	str	r3, [sp, #4]
	ldr	r3, .L994+76
	str	r3, [sp]
	ldr	r3, .L994+80
	bl	seq_printf
	ldrd	r0, r1, [r4, #732]
	bl	__aeabi_idiv
	mov	r9, r0
	ldrd	r0, r1, [r4, #744]
	bl	__aeabi_idiv
	ldr	r3, [r4, #40]
	mov	r10, r0
	ldr	r0, [r4, #636]
	str	r3, [sp, #44]
	bl	strof_pixel_fmt
	movw	r2, #5316
	ldr	r2, [r4, r2]
	mov	fp, r0
	ldr	r0, [r4, #656]
	str	r2, [sp, #40]
	bl	strof_rotation
	str	r0, [sp, #36]
	ldr	r0, [r4, #660]
	bl	strof_bool
	ldr	r1, [sp, #36]
	ldr	r2, [sp, #40]
	ldr	r3, [sp, #44]
	str	r0, [sp, #24]
	mov	r0, r5
	strd	fp, r2, [sp, #12]
	mov	r2, r6
	strd	r10, r3, [sp, #4]
	mov	r3, r8
	str	r1, [sp, #20]
	str	r9, [sp]
	ldr	r1, .L994+84
	bl	seq_printf
	mov	r0, r5
	ldr	r1, .L994+88
	bl	seq_puts
	ldr	r3, .L994+92
	mov	r2, r7
	mov	r0, r5
	ldr	r1, .L994+96
	str	r3, [sp, #20]
	ldr	r3, .L994+100
	str	r3, [sp, #16]
	ldr	r3, .L994+104
	str	r3, [sp, #12]
	ldr	r3, .L994+108
	str	r3, [sp, #8]
	ldr	r3, .L994+112
	str	r3, [sp, #4]
	ldr	r3, .L994+116
	str	r3, [sp]
	ldr	r3, .L994+120
	bl	seq_printf
	movw	r3, #10728
	ldr	r3, [r4, r3]
	mov	r0, r5
	ldr	r1, .L994+124
	ldr	r2, [r3, #36]
	str	r2, [sp, #20]
	ldr	r2, [r3, #40]
	str	r2, [sp, #16]
	ldr	r2, [r3, #32]
	str	r2, [sp, #12]
	ldr	r2, [r3, #8]
	str	r2, [sp, #8]
	ldr	r2, [r3, #12]
	str	r2, [sp, #4]
	ldr	r2, [r3]
	str	r2, [sp]
	mov	r2, r6
	ldr	r3, [r3, #4]
	bl	seq_printf
	mov	r0, r5
	ldr	r1, .L994+128
	bl	seq_puts
	ldr	r3, .L994+132
	mov	r2, r7
	mov	r0, r5
	ldr	r1, .L994+136
	str	r3, [sp, #16]
	ldr	r3, .L994+140
	str	r3, [sp, #12]
	ldr	r3, .L994+144
	str	r3, [sp, #8]
	ldr	r3, .L994+148
	str	r3, [sp, #4]
	ldr	r3, .L994+152
	str	r3, [sp]
	ldr	r3, .L994+156
	bl	seq_printf
	add	r3, r4, #10752
	mov	r2, r6
	mov	r0, r5
	ldr	r3, [r3]
	ldr	r1, .L994+160
	str	r3, [sp, #16]
	movw	r3, #10748
	ldr	r3, [r4, r3]
	str	r3, [sp, #12]
	movw	r3, #10744
	ldr	r3, [r4, r3]
	str	r3, [sp, #8]
	movw	r3, #10740
	ldr	r3, [r4, r3]
	str	r3, [sp, #4]
	movw	r3, #5328
	ldr	r3, [r4, r3]
	str	r3, [sp]
	movw	r3, #5324
	ldr	r3, [r4, r3]
	bl	seq_printf
	mov	r2, r6
	ldr	r1, [r4, #4]
	mov	r0, r5
	bl	enc_impl_proc_debug
	mov	r2, r6
	ldr	r1, [r4, #60]
	mov	r0, r5
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	rc_proc_show
.L995:
	.align	2
.L994:
	.word	.LC117
	.word	.LC121
	.word	.LC119
	.word	.LC120
	.word	.LC122
	.word	.LC123
	.word	.LC124
	.word	.LC125
	.word	.LC126
	.word	.LC118
	.word	.LC127
	.word	.LC129
	.word	.LC132
	.word	.LC131
	.word	.LC133
	.word	.LC134
	.word	.LC135
	.word	.LC136
	.word	.LC137
	.word	.LC138
	.word	.LC130
	.word	.LC139
	.word	.LC140
	.word	.LC143
	.word	.LC142
	.word	.LC144
	.word	.LC145
	.word	.LC146
	.word	.LC147
	.word	.LC148
	.word	.LC141
	.word	.LC149
	.word	.LC150
	.word	.LC153
	.word	.LC152
	.word	.LC154
	.word	.LC155
	.word	.LC156
	.word	.LC157
	.word	.LC151
	.word	.LC158
	.word	.LC128
	.fnend
	.size	mpp_enc_impl_poc_debug_info, .-mpp_enc_impl_poc_debug_info
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.36896, %object
	.size	__func__.36896, 23
__func__.36896:
	.ascii	"check_hal_task_pkt_len\000"
	.type	__func__.37154, %object
	.size	__func__.37154, 22
__func__.37154:
	.ascii	"mpp_enc_proc_two_pass\000"
	.type	__func__.37039, %object
	.size	__func__.37039, 18
__func__.37039:
	.ascii	"update_rc_cfg_log\000"
	.type	__func__.37320, %object
	.size	__func__.37320, 22
__func__.37320:
	.ascii	"mpp_enc_comb_end_jpeg\000"
	.type	__func__.36938, %object
	.size	__func__.36938, 20
__func__.36938:
	.ascii	"mpp_enc_proc_rc_cfg\000"
	.type	__func__.36946, %object
	.size	__func__.36946, 20
__func__.36946:
	.ascii	"mpp_enc_proc_hw_cfg\000"
	.type	__FUNCTION__.36975, %object
	.size	__FUNCTION__.36975, 22
__FUNCTION__.36975:
	.ascii	"mpp_enc_unref_osd_buf\000"
	.type	__func__.36987, %object
	.size	__func__.36987, 28
__func__.36987:
	.ascii	"mpp_enc_proc_export_osd_buf\000"
	.type	__func__.36999, %object
	.size	__func__.36999, 22
__func__.36999:
	.ascii	"mpp_enc_proc_tune_cfg\000"
	.type	__func__.37020, %object
	.size	__func__.37020, 17
__func__.37020:
	.ascii	"mpp_enc_proc_cfg\000"
	.type	__func__.36969, %object
	.size	__func__.36969, 21
__func__.36969:
	.ascii	"mpp_enc_proc_ref_cfg\000"
	.space	1
	.type	resend_reason.36912, %object
	.size	resend_reason.36912, 24
resend_reason.36912:
	.word	.LC159
	.word	.LC160
	.word	.LC161
	.word	.LC162
	.word	.LC163
	.word	.LC164
	.type	__func__.37086, %object
	.size	__func__.37086, 23
__func__.37086:
	.ascii	"mpp_enc_proc_rc_update\000"
	.type	__FUNCTION__.37067, %object
	.size	__FUNCTION__.37067, 11
__FUNCTION__.37067:
	.ascii	"set_rc_cfg\000"
	.type	__func__.37077, %object
	.size	__func__.37077, 11
__func__.37077:
	.ascii	"set_rc_cfg\000"
	.space	3
	.type	name_of_rc_mode, %object
	.size	name_of_rc_mode, 16
name_of_rc_mode:
	.word	.LC165
	.word	.LC166
	.word	.LC167
	.word	.LC168
	.type	__FUNCTION__.37108, %object
	.size	__FUNCTION__.37108, 34
__FUNCTION__.37108:
	.ascii	"mpp_enc_alloc_output_from_bufpool\000"
	.type	__func__.37115, %object
	.size	__func__.37115, 34
__func__.37115:
	.ascii	"mpp_enc_alloc_output_from_bufpool\000"
	.type	__FUNCTION__.37131, %object
	.size	__FUNCTION__.37131, 34
__FUNCTION__.37131:
	.ascii	"mpp_enc_alloc_output_from_ringbuf\000"
	.type	__FUNCTION__.37245, %object
	.size	__FUNCTION__.37245, 24
__FUNCTION__.37245:
	.ascii	"mpp_enc_impl_alloc_task\000"
	.type	__FUNCTION__.37249, %object
	.size	__FUNCTION__.37249, 23
__FUNCTION__.37249:
	.ascii	"mpp_enc_impl_free_task\000"
	.type	__func__.37212, %object
	.size	__func__.37212, 21
__func__.37212:
	.ascii	"mpp_enc_reenc_simple\000"
	.type	__func__.37278, %object
	.size	__func__.37278, 21
__func__.37278:
	.ascii	"mpp_enc_impl_reg_cfg\000"
	.type	__FUNCTION__.37279, %object
	.size	__FUNCTION__.37279, 21
__FUNCTION__.37279:
	.ascii	"mpp_enc_impl_reg_cfg\000"
	.type	__func__.37177, %object
	.size	__func__.37177, 19
__func__.37177:
	.ascii	"mpp_enc_normal_cfg\000"
	.type	__func__.37300, %object
	.size	__func__.37300, 22
__func__.37300:
	.ascii	"mpp_enc_impl_hw_start\000"
	.type	__func__.37339, %object
	.size	__func__.37339, 17
__func__.37339:
	.ascii	"mpp_enc_impl_int\000"
	.type	__func__.37191, %object
	.size	__func__.37191, 12
__func__.37191:
	.ascii	"mpp_enc_end\000"
	.type	__func__.37223, %object
	.size	__func__.37223, 19
__func__.37223:
	.ascii	"mpp_enc_reenc_drop\000"
	.type	__func__.37236, %object
	.size	__func__.37236, 26
__func__.37236:
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
	.ascii	"\0013%d: task %d hal wait\012\000"
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
	.ascii	"\0016%s:%d: task %d hal wait\012\000"
.LC22:
	.ascii	"\0016%s:%d: task %d rc enc->frame end\012\000"
.LC23:
	.ascii	"\0013%d: enc %p rc_frm_end:%-4d failed return %d\000"
.LC24:
	.ascii	"\0013%s:%d: ring_buf_put_used fail \012\000"
.LC25:
	.ascii	"\0016%s:%d: task %d enqueue packet pts %lld\012\000"
.LC26:
	.ascii	"\0013%d: invalid rc_priority %d should be[%d, %d] \012"
	.ascii	"\000"
.LC27:
	.ascii	"\0013%d: invalid super_mode %d should be[%d, %d] \012"
	.ascii	"\000"
.LC28:
	.ascii	"\0013%d: invalid debre_strength should be[%d, %d] \012"
	.ascii	"\000"
.LC29:
	.ascii	"\0013%d: invalid rc mode %d should be RC_MODE_VBR o"
	.ascii	"r RC_MODE_CBR\012\000"
.LC30:
	.ascii	"\0013%d: invalid quality %d should be from QUALITY_"
	.ascii	"WORST to QUALITY_BEST\012\000"
.LC31:
	.ascii	"\0013%d: invalid bit per second %d [%d:%d] out of r"
	.ascii	"ange 1K~100M\012\000"
.LC32:
	.ascii	"\0013%d: invalid qp range: init %d i [%d:%d] p [%d:"
	.ascii	"%d]\012\000"
.LC33:
	.ascii	"\0013%d: restore qp range: init %d i [%d:%d] p [%d:"
	.ascii	"%d]\012\000"
.LC34:
	.ascii	"\0013%d: invalid qp delta ip %d restore to %d\012\000"
.LC35:
	.ascii	"\0013%d: invalid qp delta vi %d restore to %d\012\000"
.LC36:
	.ascii	"\0013%d: invalid qp max step %d restore to %d\012\000"
.LC37:
	.ascii	"\0013%d: warning: bitrate statistic time %d is larg"
	.ascii	"er than 60s\012\000"
.LC38:
	.ascii	"\0013%s:%d: failed to accept new rc config\012\000"
.LC39:
	.ascii	"\0016%d: MPP_ENC_SET_RC_CFG bps %d [%d : %d] fps [%"
	.ascii	"d:%d] gop %d\012\000"
.LC40:
	.ascii	"\0013%d: invalid hw qp delta row [%d:%d]\012\000"
.LC41:
	.ascii	"\0013%s:%d: failed to accept new hw config\012\000"
.LC42:
	.ascii	"mpp_enc\000"
.LC43:
	.ascii	"\0013%s:%d: mpibuf_ops get fail\000"
.LC44:
	.ascii	"\0013%d: osd buf dma_buf_get fd %d failed\012\000"
.LC45:
	.ascii	"\0013%d: osd inv buf dma_buf_get fd %d failed\012\000"
.LC46:
	.ascii	"\0013%d: invalid scene mode %d not in range [%d:%d]"
	.ascii	"\012\000"
.LC47:
	.ascii	"\0013%s:%d: failed to accept new tuning config\012\000"
.LC48:
	.ascii	"\0016%d: MPP_ENC_SET_CFG in \012\000"
.LC49:
	.ascii	"\0013%d: Please use MPP_ENC_GET_HDR_SYNC instead of"
	.ascii	" unsafe MPP_ENC_GET_EXTRA_INFO\012\000"
.LC50:
	.ascii	"\0013%d: NOTE: MPP_ENC_GET_HDR_SYNC needs MppPacket"
	.ascii	" input\012\000"
.LC51:
	.ascii	"\0016%d: deprecated MPP_ENC_PRE_ALLOC_BUFF control\012"
	.ascii	"\000"
.LC52:
	.ascii	"\0016%s:%d: header mode set to %d\012\000"
.LC53:
	.ascii	"\0013%s:%d: invalid header mode %d\012\000"
.LC54:
	.ascii	"\0013%s:%d: invalid NULL ptr on setting header mode"
	.ascii	"\012\000"
.LC55:
	.ascii	"\0016%s:%d: sei mode set to %d\012\000"
.LC56:
	.ascii	"\0013%s:%d: invalid sei mode %d\012\000"
.LC57:
	.ascii	"\0013%d: ref param_error\000"
.LC58:
	.ascii	"\0013%s:%d: failed to copy ref cfg ret %d\012\000"
.LC59:
	.ascii	"\0013%s:%d: failed to set ref cfg ret %d\012\000"
.LC60:
	.ascii	"\0013%d: user data is overflow\000"
.LC61:
	.ascii	"\0013%d: usr data is big then 1k byte len %d\000"
.LC62:
	.ascii	"\0016%d: send header for %s\012\000"
.LC63:
	.ascii	"flex\000"
.LC64:
	.ascii	"fix\000"
.LC65:
	.ascii	"\0016%s:%d: rc update cfg start\012\000"
.LC66:
	.ascii	"rc->fps_out_num\000"
.LC67:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC68:
	.ascii	"\0013%s:%d: unsupport coding type %d\012\000"
.LC69:
	.ascii	"\0016%d: mode %s bps [%d:%d:%d] fps %s [%d/%d] -> %"
	.ascii	"s [%d/%d] gop i [%d] v [%d]\012\000"
.LC70:
	.ascii	"\0016%s:%d: rc update cfg done\012\000"
.LC71:
	.ascii	"size\000"
.LC72:
	.ascii	"buffer\000"
.LC73:
	.ascii	"\0016%s:%d: create output pkt %p buf %p\012\000"
.LC74:
	.ascii	"\0016%s:%d: output to pkt %p buf %p pos %p length %"
	.ascii	"d\012\000"
.LC75:
	.ascii	"\0013%d: ring buf get mpp_buf fail \012\000"
.LC76:
	.ascii	"\0016%s:%d: enter\012\000"
.LC77:
	.ascii	"\0016%s:%d: task %d rc hal start\012\000"
.LC78:
	.ascii	"\0013%d: enc %p rc_hal_start:%-4d failed return %d\000"
.LC79:
	.ascii	"\0016%s:%d: task %d reenc %d times %d\012\000"
.LC80:
	.ascii	"\0016%s:%d: leave\012\000"
.LC81:
	.ascii	"\0016%d: frame info no equal set drop: frame [%d, %"
	.ascii	"d, %d, %d], prep [%d, %d, %d, %d]\000"
.LC82:
	.ascii	"\0016%s:%d: task seq idx %d start\012\000"
.LC83:
	.ascii	"\0013%d: enc %p rc_frm_check_drop:%-4d failed retur"
	.ascii	"n %d\000"
.LC84:
	.ascii	"\0016%s:%d: task %d drop %d\012\000"
.LC85:
	.ascii	"enc->packet\000"
.LC86:
	.ascii	"\0016%s:%d: task %d update header length %d\012\000"
.LC87:
	.ascii	"\0016%s:%d: added_by_change \012\000"
.LC88:
	.ascii	"\0016%s:%d: check_hal_task_pkt_len \012\000"
.LC89:
	.ascii	"gen_hdr and adding\000"
.LC90:
	.ascii	"\0016%s:%d: task %d enc start\012\000"
.LC91:
	.ascii	"\0013%d: enc %p enc_impl_start:%-4d failed return %"
	.ascii	"d\000"
.LC92:
	.ascii	"\0016%s:%d: frm %d compare\012\000"
.LC93:
	.ascii	"\0016%s:%d: seq_idx      %d vs %d\012\000"
.LC94:
	.ascii	"\0016%s:%d: is_idr       %d vs %d\012\000"
.LC95:
	.ascii	"\0016%s:%d: is_intra     %d vs %d\012\000"
.LC96:
	.ascii	"\0016%s:%d: is_non_ref   %d vs %d\012\000"
.LC97:
	.ascii	"\0016%s:%d: is_lt_ref    %d vs %d\012\000"
.LC98:
	.ascii	"\0016%s:%d: lt_idx       %d vs %d\012\000"
.LC99:
	.ascii	"\0016%s:%d: temporal_id  %d vs %d\012\000"
.LC100:
	.ascii	"\0016%s:%d: frm %d done  **************************"
	.ascii	"*********\012\000"
.LC101:
	.ascii	"\0016%s:%d: task %d rc frame start\012\000"
.LC102:
	.ascii	"\0013%d: enc %p rc_frm_start:%-4d failed return %d\000"
.LC103:
	.ascii	"\0016%s:%d: task %d rc frame start ok \012\000"
.LC104:
	.ascii	"\0016%s:%d: task %d IDR header length %d\012\000"
.LC105:
	.ascii	"header adding\000"
.LC106:
	.ascii	"user data adding\000"
.LC107:
	.ascii	"\0013%d: enc %p mpp_enc_normal_cfg:%-4d failed retu"
	.ascii	"rn %d\000"
.LC108:
	.ascii	"\0016%s:%d: task %d rc hal end\012\000"
.LC109:
	.ascii	"\0016%s:%d: task %d hal ret task\012\000"
.LC110:
	.ascii	"\0016%s:%d: task %d rc frame check reenc\012\000"
.LC111:
	.ascii	"\0013%d: enc %p rc_frm_check_reenc:%-4d failed retu"
	.ascii	"rn %d\000"
.LC112:
	.ascii	"\0016%s:%d: shared status can't reenc drop request "
	.ascii	"idr\012\000"
.LC113:
	.ascii	"\0016%s:%d: task %d rc frame end\012\000"
.LC114:
	.ascii	"\0016%s:%d: task %d enc sw enc start\012\000"
.LC115:
	.ascii	"\0013%d: enc %p enc_impl_sw_enc:%-4d failed return "
	.ascii	"%d\000"
.LC116:
	.ascii	"\0013%d: enc %p mpp_enc_end:%-4d failed return %d\000"
.LC117:
	.ascii	"\012--------venc chn attr 1------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC118:
	.ascii	"Width\000"
.LC119:
	.ascii	"ID\000"
.LC120:
	.ascii	"%8s%8s%8s%6s%9s%10s%10s%6s\012\000"
.LC121:
	.ascii	"Prio\000"
.LC122:
	.ascii	"GopMode\000"
.LC123:
	.ascii	"Sequence\000"
.LC124:
	.ascii	"ByFrame\000"
.LC125:
	.ascii	"Type\000"
.LC126:
	.ascii	"Height\000"
.LC127:
	.ascii	"%8d%8u%8u%6s%9s%10u%10s%6d\012\000"
.LC128:
	.ascii	"y\000"
.LC129:
	.ascii	"\012--------venc chn attr 2------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC130:
	.ascii	"VeStr\000"
.LC131:
	.ascii	"%8s%8s%8s%8s%12s%12s%12s%12s%10s\012\000"
.LC132:
	.ascii	"mirror\000"
.LC133:
	.ascii	"rotation\000"
.LC134:
	.ascii	"RealFps*10\000"
.LC135:
	.ascii	"PixFmt\000"
.LC136:
	.ascii	"Timeref\000"
.LC137:
	.ascii	"TarFr\000"
.LC138:
	.ascii	"SrcFr\000"
.LC139:
	.ascii	"%8d%8s%8d%8d%12x%12s%12u%12s%10s\012\000"
.LC140:
	.ascii	"\012--------ring buf status------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC141:
	.ascii	"w_pos\000"
.LC142:
	.ascii	"%8s%8s%8s%8s%10s%10s%10s%10s\012\000"
.LC143:
	.ascii	"l_r_pos\000"
.LC144:
	.ascii	"l_w_pos\000"
.LC145:
	.ascii	"min_size\000"
.LC146:
	.ascii	"total_len\000"
.LC147:
	.ascii	"usd_len\000"
.LC148:
	.ascii	"r_pos\000"
.LC149:
	.ascii	"%8d%8d%8d%8d%10d%10d%10d%10d\012\000"
.LC150:
	.ascii	"\012--------hw status------------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC151:
	.ascii	"hw_run\000"
.LC152:
	.ascii	"%8s%8s%12s%14s%14s%14s%16s\012\000"
.LC153:
	.ascii	"start_fail_cnt\000"
.LC154:
	.ascii	"cfg_fail_cnt\000"
.LC155:
	.ascii	"ring_fail_cnt\000"
.LC156:
	.ascii	"pkt_fail_cnt\000"
.LC157:
	.ascii	"enc_status\000"
.LC158:
	.ascii	"%8d%8d%12d%14u%14u%14u%16u\012\000"
.LC159:
	.ascii	"unchanged\000"
.LC160:
	.ascii	"codec/prep cfg change\000"
.LC161:
	.ascii	"rc cfg change rc_mode/fps/gop\000"
.LC162:
	.ascii	"set cfg change input/format \000"
.LC163:
	.ascii	"set cfg change rc_mode/fps/gop\000"
.LC164:
	.ascii	"set cfg change codec\000"
.LC165:
	.ascii	"vbr\000"
.LC166:
	.ascii	"cbr\000"
.LC167:
	.ascii	"fixqp\000"
.LC168:
	.ascii	"avbr\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
