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
	.file	"mpp_enc.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_enc_deinit_frame
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_deinit_frame, %function
mpp_enc_deinit_frame:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbz	r0, .L1
	ldr	r3, [r0, #368]
	cbz	r3, .L1
	ldr	r0, [r0, #372]
	cbz	r0, .L1
	movw	r3, #5372
	movs	r2, #0
	str	r2, [r4, r3]
	movw	r3, #10828
	ldr	r2, [r4, #12]
	ldr	r1, [r4, r3]
	bl	mpp_packet_ring_buf_put_used
	add	r0, r4, #372
	bl	mpp_packet_deinit
	add	r0, r4, #368
	pop	{r4, lr}
	b	mpp_frame_deinit
.L1:
	pop	{r4, pc}
	.fnend
	.size	mpp_enc_deinit_frame, .-mpp_enc_deinit_frame
	.align	1
	.global	mpp_enc_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_deinit, %function
mpp_enc_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	bl	get_mpibuf_ops
	add	r6, r4, #5376
	adds	r6, r6, #16
	mov	r7, r0
	mov	r0, r6
	bl	down
	cbnz	r4, .L13
	movs	r2, #181
	ldr	r1, .L81
	ldr	r0, .L81+4
	bl	printk
	mvn	r0, #2
.L12:
	pop	{r4, r5, r6, r7, r8, pc}
.L13:
	movw	r3, #5380
	ldr	r3, [r4, r3]
	cbz	r3, .L15
	mov	r0, r4
	bl	mpp_enc_deinit_frame
.L15:
	mov	r0, r4
	bl	mpp_enc_impl_free_task
	ldr	r0, [r4, #4]
	cbz	r0, .L16
	bl	enc_impl_deinit
	movs	r3, #0
	str	r3, [r4, #4]
.L16:
	ldr	r0, [r4, #8]
	cbz	r0, .L17
	bl	mpp_enc_hal_deinit
	movs	r3, #0
	str	r3, [r4, #8]
.L17:
	ldr	r3, [r4, #404]
	cbz	r3, .L18
	add	r0, r4, #404
	bl	mpp_packet_deinit
.L18:
	ldr	r1, [r4, #408]
	cbz	r1, .L19
	ldr	r0, .L81+8
	bl	mpp_osal_free
.L19:
	movw	r5, #5328
	mov	r8, #0
	str	r8, [r4, #408]
	adds	r0, r4, r5
	ldr	r3, [r4, r5]
	cbz	r3, .L20
	bl	mpp_enc_ref_cfg_deinit
	str	r8, [r4, r5]
.L20:
	ldr	r3, [r4, #460]
	cbz	r3, .L21
	add	r0, r4, #460
	bl	mpp_enc_refs_deinit
	movs	r3, #0
	str	r3, [r4, #460]
.L21:
	ldr	r0, [r4, #60]
	cbz	r0, .L22
	bl	rc_deinit
	movs	r3, #0
	str	r3, [r4, #60]
.L22:
	movw	r3, #10776
	ldr	r3, [r4, r3]
	cbz	r3, .L23
	movw	r2, #10808
	ldr	r2, [r4, r2]
	ldr	r2, [r2, #12]
	cbnz	r2, .L24
	ldr	r0, [r3, #20]
	cbz	r0, .L24
	ldr	r1, .L81+8
	bl	mpp_buffer_put_with_caller
.L24:
	movw	r3, #10776
	ldr	r1, [r4, r3]
	mov	r5, r3
	cbz	r1, .L25
	ldr	r0, .L81+8
	bl	mpp_osal_free
.L25:
	movs	r3, #0
	str	r3, [r4, r5]
.L23:
	add	r0, r4, #5536
	adds	r0, r0, #8
	bl	mpp_enc_unref_osd_buf
	movw	r3, #5388
	ldr	r3, [r4, r3]
	cbz	r3, .L27
	movw	r3, #10812
	ldr	r0, [r4, r3]
	cbz	r0, .L28
	ldr	r1, .L81+8
	bl	mpp_buffer_put_with_caller
.L28:
	add	r3, r4, #10816
	ldr	r0, [r3]
	cbz	r0, .L29
	ldr	r1, .L81+8
	bl	mpp_buffer_put_with_caller
.L29:
	movw	r3, #10820
	ldr	r1, [r4, r3]
	cbz	r1, .L27
	ldr	r0, .L81+8
	bl	mpp_osal_free
.L27:
	ldr	r1, [r4, #436]
	cbz	r1, .L31
	ldr	r0, .L81+8
	bl	mpp_osal_free
.L31:
	movs	r5, #0
	mov	r0, r6
	str	r5, [r4, #436]
	movw	r6, #5404
	strd	r5, r5, [r4, #444]
	bl	up
	ldr	r3, [r4, r6]
	cbz	r3, .L32
	movs	r1, #245
	ldr	r0, .L81+12
	bl	printk
	ldr	r3, [r7, #60]
	ldr	r0, [r4, r6]
	blx	r3
	movs	r1, #247
	ldr	r0, .L81+16
	bl	printk
	str	r5, [r4, r6]
.L32:
	ldr	r0, .L81+8
	mov	r1, r4
	bl	mpp_osal_free
	movs	r0, #0
	b	.L12
.L82:
	.align	2
.L81:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR0+15
	.word	.LC1
	.word	.LC2
	.fnend
	.size	mpp_enc_deinit, .-mpp_enc_deinit
	.align	1
	.global	mpp_enc_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_init, %function
mpp_enc_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r2, .L95
	movs	r3, #0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #72
	sub	sp, sp, #72
	mov	r6, r1
	ldr	r7, [r1]
	strd	r3, r3, [sp]
	mov	r8, r0
	str	r2, [sp, #8]
	cbnz	r0, .L84
	movs	r2, #44
	ldr	r1, .L95+4
	ldr	r0, .L95+8
	mvn	r5, #2
	bl	printk
.L83:
	mov	r0, r5
	add	sp, sp, #72
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L84:
	str	r3, [r0]
	movw	r1, #10840
	ldr	r0, .L95+12
	bl	mpp_osal_calloc
	mov	r4, r0
	cbnz	r0, .L86
	movs	r2, #52
	ldr	r1, .L95+4
	ldr	r0, .L95+8
	mvn	r5, #3
	bl	printk
	b	.L83
.L86:
	add	r0, r0, #460
	bl	mpp_enc_refs_init
	mov	r5, r0
	cbz	r0, .L87
	ldr	r1, .L95+4
	movs	r2, #58
	ldr	r0, .L95+16
.L94:
	bl	printk
.L88:
	mov	r0, r4
	bl	mpp_enc_deinit
	b	.L83
.L87:
	ldr	r1, [r6, #4]
	movs	r2, #29
	strd	r2, r0, [sp, #60]
	add	r3, r4, #600
	ldr	r0, [r6, #8]
	str	r1, [sp, #40]
	ldr	r1, [r6, #16]
	str	r3, [sp, #36]
	str	r3, [sp, #20]
	str	r1, [sp, #44]
	ldr	r1, [r6, #24]
	str	r7, [sp, #32]
	strd	r7, r2, [sp, #12]
	str	r1, [sp, #68]
	ldr	r1, [r6, #28]
	str	r1, [sp, #48]
	ldr	r1, [r6, #20]
	str	r1, [sp, #52]
	ldr	r1, [r6, #36]
	str	r1, [sp, #56]
	movw	r1, #10780
	str	r0, [r4, r1]
	movw	r1, #10784
	ldr	r0, [r6, #12]
	str	r0, [r4, r1]
	add	r1, sp, #32
	ldr	r3, [r4, #460]
	add	r0, sp, #4
	str	r3, [sp, #24]
	movs	r3, #2
	str	r3, [sp, #28]
	bl	mpp_enc_hal_init
	mov	r5, r0
	cbz	r0, .L89
	movs	r2, #83
	ldr	r1, .L95+4
	ldr	r0, .L95+20
	b	.L94
.L89:
	ldr	r3, [sp, #60]
	add	r1, sp, #12
	mov	r0, sp
	str	r3, [sp, #16]
	mov	r3, #-1
	str	r3, [sp, #28]
	bl	enc_impl_init
	mov	r5, r0
	cbz	r0, .L90
	movs	r2, #92
	ldr	r1, .L95+4
	ldr	r0, .L95+24
	b	.L94
.L90:
	mov	r0, r4
	bl	mpp_enc_impl_alloc_task
	ldr	r2, [r6, #20]
	add	r0, r4, #60
	cbz	r2, .L91
	add	r2, sp, #8
.L91:
	mov	r1, r7
	mov	r5, #1024
	bl	rc_init
	str	r7, [r4]
	ldr	r2, .L95+28
	mov	r1, r5
	str	r5, [r4, #448]
	ldr	r0, .L95+12
	ldr	r3, [sp]
	str	r3, [r4, #4]
	ldr	r3, [sp, #4]
	str	r3, [r4, #8]
	ldr	r3, [sp, #64]
	str	r3, [r4, #12]
	movs	r3, #0
	strd	r2, r3, [r4, #428]
	str	r3, [r4, #424]
	bl	mpp_osal_calloc
	mov	r1, r5
	str	r0, [r4, #436]
	ldr	r0, .L95+12
	bl	mpp_osal_calloc
	mov	r2, r5
	mov	r1, r0
	str	r0, [r4, #408]
	add	r0, r4, #404
	bl	mpp_packet_init
	mov	r5, r0
	cmp	r0, #0
	bne	.L88
	mov	r1, r0
	ldr	r0, [r4, #404]
	bl	mpp_packet_set_length
	str	r7, [r4, #1192]
	movw	r7, #5328
	adds	r0, r4, r7
	bl	mpp_enc_ref_cfg_init
	mov	r5, r0
	cmp	r0, #0
	bne	.L88
	ldr	r5, [r4, r7]
	bl	mpp_enc_ref_default
	mov	r1, r0
	mov	r0, r5
	bl	mpp_enc_ref_cfg_copy
	mov	r5, r0
	cmp	r0, #0
	bne	.L88
	ldr	r5, [r4, #460]
	bl	mpp_enc_ref_default
	mov	r1, r0
	mov	r0, r5
	bl	mpp_enc_refs_set_cfg
	mov	r5, r0
	cmp	r0, #0
	bne	.L88
	ldr	r1, [r4, #752]
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_set_rc_igop
	mov	r5, r0
	cmp	r0, #0
	bne	.L88
	movw	r0, #5392
	adds	r3, r4, r0
	movs	r2, #1
	adds	r1, r3, #4
	str	r2, [r4, r0]
	strd	r1, r1, [r3, #4]
	movw	r3, #5368
	str	r2, [r4, r3]
	movw	r3, #5372
	str	r5, [r4, r3]
	movw	r3, #6674
	movs	r2, #4
	strb	r2, [r4, r3]
	bl	get_vsm_ops
	cbnz	r0, .L92
	movs	r1, #44
	ldr	r0, .L95+12
	bl	mpp_osal_calloc
	movw	r3, #10776
	str	r0, [r4, r3]
.L92:
	ldr	r2, [r6, #4]
	movw	r3, #5380
	str	r2, [r4, r3]
	movw	r3, #10808
	ldr	r2, [r6, #24]
	str	r2, [r4, r3]
	movw	r3, #5388
	ldr	r2, [r6, #28]
	str	r2, [r4, r3]
	movw	r3, #10828
	ldr	r2, [r6, #32]
	str	r2, [r4, r3]
	movw	r3, #5384
	ldr	r2, [r6, #16]
	str	r2, [r4, r3]
	str	r4, [r8]
	b	.L83
.L96:
	.align	2
.L95:
	.word	.LC3
	.word	.LANCHOR0+30
	.word	.LC4
	.word	.LANCHOR0+43
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.fnend
	.size	mpp_enc_init, .-mpp_enc_init
	.align	1
	.global	mpp_enc_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_start, %function
mpp_enc_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L106
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L98
	mov	r3, r0
	movw	r2, #259
	ldr	r1, .L106+4
	ldr	r0, .L106+8
	bl	printk
.L98:
	add	r6, r4, #5376
	adds	r6, r6, #16
	mov	r0, r6
	bl	down
	movw	r3, #5368
	movs	r2, #0
	mov	r0, r6
	str	r2, [r4, r3]
	bl	up
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L99
	mov	r3, r4
	mov	r2, #266
	ldr	r1, .L106+4
	ldr	r0, .L106+12
	bl	printk
.L99:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L107:
	.align	2
.L106:
	.word	.LANCHOR1
	.word	.LANCHOR0+56
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mpp_enc_start, .-mpp_enc_start
	.align	1
	.global	mpp_enc_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_stop, %function
mpp_enc_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	add	r5, r0, #5376
	adds	r5, r5, #16
	ldr	r6, .L117
	mov	r4, r0
	mov	r0, r5
	bl	down
	ldr	r2, [r6]
	lsls	r2, r2, #31
	bpl	.L109
	mov	r3, r4
	movw	r2, #277
	ldr	r1, .L117+4
	ldr	r0, .L117+8
	bl	printk
.L109:
	movw	r3, #5368
	movs	r2, #1
	str	r2, [r4, r3]
	movw	r3, #5372
	ldr	r7, [r4, r3]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L110
	mov	r3, r4
	mov	r2, #280
	ldr	r1, .L117+4
	ldr	r0, .L117+12
	bl	printk
.L110:
	mov	r0, r5
	bl	up
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, pc}
.L118:
	.align	2
.L117:
	.word	.LANCHOR1
	.word	.LANCHOR0+70
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mpp_enc_stop, .-mpp_enc_stop
	.align	1
	.global	mpp_enc_check_pkt_pool
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_check_pkt_pool, %function
mpp_enc_check_pkt_pool:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	bl	get_mpibuf_ops
	cbz	r0, .L119
	ldr	r3, [r0, #68]
	cbz	r3, .L119
	movw	r2, #5404
	ldr	r0, [r4, r2]
	pop	{r4, lr}
	bx	r3
.L119:
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	mpp_enc_check_pkt_pool, .-mpp_enc_check_pkt_pool
	.align	1
	.global	mpp_enc_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_reset, %function
mpp_enc_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L132
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L126
	mov	r3, r0
	mov	r2, #304
	ldr	r1, .L132+4
	ldr	r0, .L132+8
	bl	printk
.L126:
	cbnz	r4, .L128
	mov	r2, #306
	ldr	r1, .L132+4
	ldr	r0, .L132+12
	bl	printk
	mvn	r0, #2
.L125:
	pop	{r4, pc}
.L128:
	movs	r0, #0
	b	.L125
.L133:
	.align	2
.L132:
	.word	.LANCHOR1
	.word	.LANCHOR0+83
	.word	.LC9
	.word	.LC11
	.fnend
	.size	mpp_enc_reset, .-mpp_enc_reset
	.align	1
	.global	mpp_enc_oneframe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_oneframe, %function
mpp_enc_oneframe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	cbnz	r0, .L135
	movw	r2, #319
	ldr	r1, .L143
	ldr	r0, .L143+4
	bl	printk
	mvn	r0, #2
.L134:
	pop	{r3, r4, r5, pc}
.L135:
	ldr	r5, .L143+8
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bmi	.L137
.L142:
	movs	r0, #0
	b	.L134
.L137:
	mov	r3, r0
	movw	r2, #323
	ldr	r1, .L143
	ldr	r0, .L143+12
	bl	printk
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L142
	mov	r3, r4
	movw	r2, #325
	ldr	r1, .L143
	ldr	r0, .L143+16
	bl	printk
	b	.L142
.L144:
	.align	2
.L143:
	.word	.LANCHOR0+97
	.word	.LC11
	.word	.LANCHOR1
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mpp_enc_oneframe, .-mpp_enc_oneframe
	.align	1
	.global	mpp_enc_cfg_reg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_cfg_reg, %function
mpp_enc_cfg_reg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r8, r1
	mov	r4, r0
	cbnz	r0, .L146
	movw	r2, #337
	ldr	r1, .L168
	ldr	r0, .L168+4
	mvn	r8, #2
	bl	printk
.L145:
	mov	r0, r8
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L146:
	ldr	r6, .L168+8
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L148
	mov	r3, r0
	movw	r2, #341
	ldr	r1, .L168
	ldr	r0, .L168+12
	bl	printk
.L148:
	add	r5, r4, #5376
	adds	r5, r5, #16
	mov	r0, r5
	bl	down
	movw	r3, #5368
	ldr	r9, [r4, r3]
	cmp	r9, #0
	beq	.L149
	mov	r0, r5
	mov	r8, #-1
	bl	up
	b	.L145
.L149:
	mov	r0, r4
	add	r7, r4, #5376
	bl	mpp_enc_proc_rc_update
	movw	r3, #5388
	str	r9, [r7]
	ldr	r3, [r4, r3]
	cmp	r3, #0
	beq	.L151
	movw	r3, #10812
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bne	.L151
	ldr	r2, [r4, #1192]
	ldrd	r1, r3, [r4, #628]
	cmp	r2, #7
	bne	.L153
	adds	r1, r1, #63
	adds	r3, r3, #63
	bics	r1, r1, #63
	it	mi
	addmi	r1, r1, #15
	bics	r3, r3, #63
	asr	r1, r1, #4
.L167:
	it	mi
	addmi	r3, r3, #15
	ldr	fp, .L168+20
	movs	r0, #0
	asrs	r3, r3, #4
	str	fp, [sp]
	mul	r9, r3, r1
	add	r1, r4, #10752
	adds	r1, r1, #60
	ldr	r3, .L168+16
	lsl	r10, r9, #2
	mov	r2, r10
	bl	mpp_buffer_get_with_tag
	add	r1, r4, #10816
	ldr	r0, [r1]
	cbnz	r0, .L159
	str	fp, [sp]
	mov	r2, r10
	ldr	r3, .L168+16
	bl	mpp_buffer_get_with_tag
.L159:
	mov	r1, r9
	ldr	r0, .L168+20
	bl	mpp_osal_calloc
	movw	r3, #10820
	str	r0, [r4, r3]
	cbnz	r0, .L151
	mov	r1, #366
	ldr	r0, .L168+24
	bl	printk
.L151:
	mov	r1, r8
	mov	r0, r4
	bl	mpp_enc_impl_reg_cfg
	movs	r3, #1
	str	r3, [r7]
	mov	r8, r0
	mov	r0, r5
	bl	up
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L145
	mov	r3, r4
	movw	r2, #371
	ldr	r1, .L168
	ldr	r0, .L168+28
	bl	printk
	b	.L145
.L153:
	adds	r1, r1, #31
	adds	r3, r3, #31
	bics	r1, r1, #31
	it	mi
	addmi	r1, r1, #15
	bics	r3, r3, #31
	asr	r1, r1, #4
	b	.L167
.L169:
	.align	2
.L168:
	.word	.LANCHOR0+114
	.word	.LC11
	.word	.LANCHOR1
	.word	.LC9
	.word	.LC12
	.word	.LANCHOR0+130
	.word	.LC13
	.word	.LC10
	.fnend
	.size	mpp_enc_cfg_reg, .-mpp_enc_cfg_reg
	.align	1
	.global	mpp_enc_hw_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_hw_start, %function
mpp_enc_hw_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r5, r1
	mov	r4, r0
	cbnz	r0, .L171
	movw	r2, #383
	ldr	r1, .L201
	ldr	r0, .L201+4
	mvn	r6, #2
	bl	printk
.L170:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L171:
	ldr	r3, .L201+8
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #1
	beq	.L173
	mov	r3, r0
	movw	r2, #387
	ldr	r1, .L201
	ldr	r0, .L201+12
	bl	printk
.L173:
	add	r7, r4, #5376
	adds	r7, r7, #16
	mov	r0, r7
	bl	down
	movw	r3, #5368
	ldr	r3, [r4, r3]
	cbz	r3, .L174
	mov	r0, r7
	mov	r6, #-1
	bl	up
	b	.L170
.L174:
	add	r9, r4, #5376
	movs	r3, #2
	str	r3, [r9]
	cbz	r5, .L175
	add	r2, r5, #5376
	str	r3, [r2]
.L175:
	mov	r1, r5
	mov	r0, r4
	bl	mpp_enc_impl_hw_start
	movs	r3, #3
	str	r3, [r9]
	mov	r6, r0
	cbz	r5, .L176
	add	r5, r5, #5376
	str	r3, [r5]
.L176:
	cbnz	r6, .L177
	bl	get_mpidev_ops
	mov	r5, r0
	cbz	r0, .L178
	ldr	r3, [r0, #32]
	cbz	r3, .L178
	ldr	r0, [r4, #368]
	bl	mpp_frame_get_dts
	movw	r3, #5380
	ldr	r3, [r4, r3]
	strd	r0, [sp]
	cbz	r3, .L178
	movw	r0, #10828
	ldr	r3, [r5, #32]
	mov	r2, sp
	mov	r1, r6
	ldr	r0, [r4, r0]
	blx	r3
.L178:
	movw	r3, #5372
	movs	r2, #1
	str	r2, [r4, r3]
.L177:
	mov	r0, r7
	bl	up
	ldr	r3, [r8]
	lsls	r3, r3, #31
	bpl	.L170
	mov	r3, r4
	movw	r2, #413
	ldr	r1, .L201
	ldr	r0, .L201+16
	bl	printk
	b	.L170
.L202:
	.align	2
.L201:
	.word	.LANCHOR0+146
	.word	.LC11
	.word	.LANCHOR1
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mpp_enc_hw_start, .-mpp_enc_hw_start
	.align	1
	.global	mpp_enc_run_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_run_task, %function
mpp_enc_run_task:
	.fnstart
	@ args = 8, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r9, r3
	ldr	r3, [r0]
	mov	r4, r0
	mov	r8, r2
	cmp	r3, #7
	ite	eq
	moveq	r10, #16
	movne	r10, #8
	cbnz	r0, .L205
	mov	r2, #426
	ldr	r1, .L232
	ldr	r0, .L232+4
	mvn	r8, #2
	bl	printk
.L203:
	mov	r0, r8
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L205:
	ldr	r5, .L232+8
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L207
	mov	r3, r0
	mov	r2, #430
	ldr	r1, .L232
	ldr	r0, .L232+12
	bl	printk
.L207:
	add	r7, r4, #5376
	ldr	r6, [r7]
	cmp	r6, #3
	bne	.L213
	movs	r3, #4
	str	r3, [r7]
	ldr	r0, [r4, #372]
	cbz	r0, .L208
	mov	r2, r8
	mov	r3, r9
	bl	mpp_packet_set_pts
	ldr	r0, [r4, #372]
	ldrd	r2, [sp, #48]
	bl	mpp_packet_set_dts
.L208:
	ldr	r0, [r4, #368]
	cbz	r0, .L209
	mov	r2, r8
	mov	r3, r9
	bl	mpp_frame_set_pts
	ldr	r0, [r4, #368]
	ldrd	r2, [sp, #48]
	bl	mpp_frame_set_dts
.L209:
	bl	get_mpidev_ops
	mov	r8, r0
	cbz	r0, .L210
	ldr	r3, [r0, #32]
	cbz	r3, .L210
	movw	r2, #10828
	movs	r1, #1
	ldr	r9, [r4, r2]
	mov	r2, sp
	mov	r0, r9
	blx	r3
	ldr	r3, [r8, #32]
	movs	r1, #2
	mov	r0, r9
	add	r2, sp, #4
	blx	r3
.L210:
	ldr	r2, [r4, #612]
	rsb	r1, r10, #0
	ldr	r3, [r4, #616]
	subs	r2, r2, #1
	ldr	r0, [r4, #12]
	subs	r3, r3, #1
	add	r2, r2, r10
	add	r3, r3, r10
	ands	r2, r2, r1
	ands	r3, r3, r1
	str	r2, [sp, #8]
	movs	r1, #7
	mov	r2, sp
	str	r3, [sp, #12]
	bl	mpp_dev_ioctl
	ldr	r3, [r5]
	subs	r8, r0, #0
	it	eq
	moveq	r6, #5
	lsls	r3, r3, #31
	str	r6, [r7]
	bpl	.L203
	mov	r3, r4
	movw	r2, #467
	ldr	r1, .L232
	ldr	r0, .L232+16
	bl	printk
	b	.L203
.L213:
	mov	r8, #-1
	b	.L203
.L233:
	.align	2
.L232:
	.word	.LANCHOR0+163
	.word	.LC11
	.word	.LANCHOR1
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mpp_enc_run_task, .-mpp_enc_run_task
	.align	1
	.global	mpp_enc_check_hw_running
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_check_hw_running, %function
mpp_enc_check_hw_running:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	cbz	r0, .L238
	ldr	r0, [r0, #12]
	pop	{r3, lr}
	b	mpp_dev_chnl_check_running
.L238:
	movw	r2, #477
	ldr	r1, .L239
	ldr	r0, .L239+4
	bl	printk
	mvn	r0, #2
	pop	{r3, pc}
.L240:
	.align	2
.L239:
	.word	.LANCHOR0+180
	.word	.LC11
	.fnend
	.size	mpp_enc_check_hw_running, .-mpp_enc_check_hw_running
	.align	1
	.global	mpp_enc_unbind_jpeg_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_unbind_jpeg_task, %function
mpp_enc_unbind_jpeg_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	cbz	r0, .L245
	ldr	r0, [r0, #12]
	pop	{r3, lr}
	b	mpp_dev_chnl_unbind_jpeg_task
.L245:
	movw	r2, #489
	ldr	r1, .L246
	ldr	r0, .L246+4
	bl	printk
	mvn	r0, #2
	pop	{r3, pc}
.L247:
	.align	2
.L246:
	.word	.LANCHOR0+205
	.word	.LC11
	.fnend
	.size	mpp_enc_unbind_jpeg_task, .-mpp_enc_unbind_jpeg_task
	.align	1
	.global	mpp_enc_check_is_int_process
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_check_is_int_process, %function
mpp_enc_check_is_int_process:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	cbnz	r0, .L249
	mov	r2, #502
	ldr	r1, .L251
	ldr	r0, .L251+4
	bl	printk
.L250:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L249:
	add	r5, r0, #5376
	add	r4, r4, #5376
	adds	r5, r5, #16
	mov	r0, r5
	bl	down
	ldr	r4, [r4]
	mov	r0, r5
	subs	r3, r4, #6
	rsbs	r4, r3, #0
	adcs	r4, r4, r3
	bl	up
	b	.L250
.L252:
	.align	2
.L251:
	.word	.LANCHOR0+230
	.word	.LC11
	.fnend
	.size	mpp_enc_check_is_int_process, .-mpp_enc_check_is_int_process
	.align	1
	.global	mpp_enc_int_process
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_int_process, %function
mpp_enc_int_process:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r1
	mov	r6, r2
	mov	r9, r3
	mov	r5, r0
	cbnz	r0, .L254
	movw	r2, #522
	ldr	r1, .L271
	ldr	r0, .L271+4
	mvn	r6, #2
	bl	printk
.L253:
	mov	r0, r6
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L254:
	ldr	r3, .L271+8
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #1
	beq	.L256
	mov	r3, r0
	movw	r2, #526
	ldr	r1, .L271
	ldr	r0, .L271+12
	bl	printk
.L256:
	add	r7, r5, #5376
	movs	r3, #6
	str	r3, [r7]
	cbz	r4, .L257
	add	r2, r4, #5376
	str	r3, [r2]
.L257:
	mov	r3, r9
	mov	r2, r6
	mov	r1, r4
	mov	r0, r5
	bl	mpp_enc_impl_int
	movs	r3, #7
	str	r3, [r7]
	mov	r6, r0
	cbz	r4, .L258
	add	r4, r4, #5376
	str	r3, [r4]
.L258:
	movw	r3, #5372
	movs	r2, #0
	str	r2, [r5, r3]
	ldr	r3, [r8]
	lsls	r3, r3, #31
	bpl	.L253
	mov	r3, r5
	movw	r2, #535
	ldr	r1, .L271
	ldr	r0, .L271+16
	bl	printk
	b	.L253
.L272:
	.align	2
.L271:
	.word	.LANCHOR0+259
	.word	.LC11
	.word	.LANCHOR1
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mpp_enc_int_process, .-mpp_enc_int_process
	.align	1
	.global	mpp_enc_proc_debug
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_proc_debug, %function
mpp_enc_proc_debug:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	mov	r7, r2
	mov	r4, r1
	cbnz	r1, .L274
	mov	r2, #544
	ldr	r1, .L280
	ldr	r0, .L280+4
	pop	{r4, r5, r6, r7, r8, lr}
	b	printk
.L274:
	ldr	r5, .L280+8
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L275
	mov	r3, r1
	mov	r2, #548
	ldr	r1, .L280
	ldr	r0, .L280+12
	bl	printk
.L275:
	mov	r2, r7
	mov	r1, r4
	mov	r0, r6
	bl	mpp_enc_impl_poc_debug_info
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L273
	mov	r3, r4
	mov	r2, #552
	ldr	r1, .L280
	ldr	r0, .L280+16
	pop	{r4, r5, r6, r7, r8, lr}
	b	printk
.L273:
	pop	{r4, r5, r6, r7, r8, pc}
.L281:
	.align	2
.L280:
	.word	.LANCHOR0+279
	.word	.LC11
	.word	.LANCHOR1
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mpp_enc_proc_debug, .-mpp_enc_proc_debug
	.align	1
	.global	mpp_enc_register_chl
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_register_chl, %function
mpp_enc_register_chl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	cbnz	r0, .L283
	mov	r2, #564
	ldr	r1, .L285
	ldr	r0, .L285+4
	bl	printk
	mvn	r0, #2
.L282:
	pop	{r3, pc}
.L283:
	movw	r3, #5380
	ldr	r3, [r0, r3]
	ldr	r0, [r0, #12]
	orr	r2, r2, r3, lsl #16
	bl	mpp_dev_chnl_register
	movs	r0, #0
	b	.L282
.L286:
	.align	2
.L285:
	.word	.LANCHOR0+298
	.word	.LC11
	.fnend
	.size	mpp_enc_register_chl, .-mpp_enc_register_chl
	.align	1
	.global	mpp_enc_notify
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_notify, %function
mpp_enc_notify:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	ldr	r5, .L294
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L289
	mov	r3, r0
	str	r1, [sp]
	movw	r2, #579
	ldr	r1, .L294+4
	ldr	r0, .L294+8
	bl	printk
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L289
	mov	r3, r4
	movw	r2, #581
	ldr	r1, .L294+4
	ldr	r0, .L294+12
	bl	printk
.L289:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L295:
	.align	2
.L294:
	.word	.LANCHOR1
	.word	.LANCHOR0+319
	.word	.LC14
	.word	.LC10
	.fnend
	.size	mpp_enc_notify, .-mpp_enc_notify
	.align	1
	.global	mpp_enc_control
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_control, %function
mpp_enc_control:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r6, r1
	mov	r4, r2
	mov	r5, r0
	cbnz	r0, .L297
	movw	r2, #597
	ldr	r1, .L364
	ldr	r0, .L364+4
	bl	printk
.L361:
	mvn	r0, #2
.L296:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L297:
	cbnz	r2, .L299
	ldr	r3, .L364+8
	cmp	r1, r3
	beq	.L299
	mov	r3, r0
	str	r1, [sp]
	movw	r2, #602
	ldr	r1, .L364
	ldr	r0, .L364+12
	bl	printk
	b	.L361
.L299:
	ldr	r7, .L364+16
	ldr	r3, [r7]
	lsls	r3, r3, #30
	bpl	.L300
	str	r4, [sp]
	mov	r3, r6
	movw	r2, #606
	ldr	r1, .L364
	ldr	r0, .L364+20
	bl	printk
.L300:
	ldr	r3, .L364+24
	cmp	r6, r3
	beq	.L301
	bhi	.L302
	subs	r3, r3, #4
	cmp	r6, r3
	beq	.L303
	bhi	.L304
	subs	r3, r3, #1
	cmp	r6, r3
	beq	.L305
.L306:
	add	r8, r5, #5376
	add	r8, r8, #16
	mov	r0, r8
	bl	down
	mov	r2, r4
	mov	r1, r6
	mov	r0, r5
	bl	mpp_enc_proc_cfg
	ldr	r0, [r5, #8]
	bl	mpp_enc_hal_prepare
	mov	r0, r8
	bl	up
	ldr	r3, [r7]
	lsls	r3, r3, #30
	bpl	.L360
	mov	r3, r6
	movw	r2, #665
	ldr	r1, .L364
	ldr	r0, .L364+28
	bl	printk
	b	.L360
.L304:
	ldr	r3, .L364+32
	cmp	r6, r3
	beq	.L307
	adds	r3, r3, #2
	cmp	r6, r3
	bne	.L306
	ldr	r3, [r7]
	lsls	r7, r3, #30
	bpl	.L319
	movw	r2, #637
	ldr	r1, .L364
	ldr	r0, .L364+36
	bl	printk
.L319:
	movs	r2, #200
	add	r1, r5, #704
	b	.L362
.L302:
	ldr	r3, .L364+40
	cmp	r6, r3
	beq	.L309
	bhi	.L310
	subs	r3, r3, #12
	cmp	r6, r3
	bne	.L306
	ldr	r3, [r7]
	lsls	r6, r3, #30
	bpl	.L320
	movw	r2, #642
	ldr	r1, .L364
	ldr	r0, .L364+44
	bl	printk
.L320:
	movw	r2, #2836
	add	r1, r5, #1192
	b	.L362
.L310:
	ldr	r3, .L364+48
	cmp	r6, r3
	beq	.L312
	add	r3, r3, #512
	cmp	r6, r3
	bne	.L306
	ldr	r3, [r7]
	lsls	r1, r3, #30
	bpl	.L322
	mov	r2, #652
	ldr	r1, .L364
	ldr	r0, .L364+52
	bl	printk
.L322:
	add	r5, r5, #5280
	adds	r5, r5, #24
	add	r3, r5, #24
.L323:
	ldr	r2, [r5], #4	@ unaligned
	cmp	r5, r3
	str	r2, [r4], #4	@ unaligned
	bne	.L323
	b	.L360
.L305:
	ldr	r3, [r7]
	lsls	r0, r3, #30
	bpl	.L314
	mov	r2, #612
	ldr	r1, .L364
	ldr	r0, .L364+56
	bl	printk
.L314:
	movw	r2, #4760
	add	r1, r5, #600
	mov	r0, r4
	bl	memcpy
	ldr	r3, [r4, #56]
	bic	r3, r3, #2
	cmp	r3, #1
	itttt	eq
	ldrdeq	r2, r3, [r4, #12]
	strdeq	r3, r2, [r4, #12]
	ldrdeq	r2, r3, [r4, #28]
	strdeq	r3, r2, [r4, #28]
.L360:
	movs	r0, #0
	b	.L296
.L303:
	ldr	r3, [r7]
	lsls	r1, r3, #30
	bpl	.L316
	movw	r2, #622
	ldr	r1, .L364
	ldr	r0, .L364+60
	bl	printk
.L316:
	add	r0, r5, #608
	movs	r2, #96
	mov	r1, r4
.L363:
	bl	memcpy
	b	.L360
.L301:
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L317
	movw	r2, #627
	ldr	r1, .L364
	ldr	r0, .L364+64
	bl	printk
.L317:
	movw	r2, #2836
	mov	r1, r4
	add	r0, r5, #1192
	b	.L363
.L307:
	ldr	r3, [r7]
	lsls	r3, r3, #30
	bpl	.L318
	mov	r2, #632
	ldr	r1, .L364
	ldr	r0, .L364+68
	bl	printk
.L318:
	add	r1, r5, #608
	movs	r2, #96
.L362:
	mov	r0, r4
	b	.L363
.L312:
	ldr	r3, [r7]
	lsls	r0, r3, #30
	bpl	.L321
	movw	r2, #647
	ldr	r1, .L364
	ldr	r0, .L364+72
	bl	printk
.L321:
	ldr	r3, [r5, #420]	@ unaligned
	str	r3, [r4]	@ unaligned
	b	.L360
.L309:
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L324
	movw	r2, #657
	ldr	r1, .L364
	ldr	r0, .L364+76
	bl	printk
.L324:
	movs	r2, #136
	addw	r1, r5, #4040
	b	.L362
.L365:
	.align	2
.L364:
	.word	.LANCHOR0+334
	.word	.LC15
	.word	3276809
	.word	.LC16
	.word	.LANCHOR1
	.word	.LC17
	.word	3276807
	.word	.LC27
	.word	3276804
	.word	.LC22
	.word	3276820
	.word	.LC23
	.word	3309570
	.word	.LC25
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.word	.LC24
	.word	.LC26
	.fnend
	.size	mpp_enc_control, .-mpp_enc_control
	.align	1
	.global	mpp_enc_pkt_full_inc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_pkt_full_inc, %function
mpp_enc_pkt_full_inc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	mpp_enc_impl_pkt_full_inc
	.fnend
	.size	mpp_enc_pkt_full_inc, .-mpp_enc_pkt_full_inc
	.global	mpp_enc_debug
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.22584, %object
	.size	__func__.22584, 15
__func__.22584:
	.ascii	"mpp_enc_deinit\000"
	.type	__FUNCTION__.22585, %object
	.size	__FUNCTION__.22585, 15
__FUNCTION__.22585:
	.ascii	"mpp_enc_deinit\000"
	.type	__func__.22564, %object
	.size	__func__.22564, 13
__func__.22564:
	.ascii	"mpp_enc_init\000"
	.type	__FUNCTION__.22565, %object
	.size	__FUNCTION__.22565, 13
__FUNCTION__.22565:
	.ascii	"mpp_enc_init\000"
	.type	__func__.22590, %object
	.size	__func__.22590, 14
__func__.22590:
	.ascii	"mpp_enc_start\000"
	.type	__func__.22596, %object
	.size	__func__.22596, 13
__func__.22596:
	.ascii	"mpp_enc_stop\000"
	.type	__func__.22611, %object
	.size	__func__.22611, 14
__func__.22611:
	.ascii	"mpp_enc_reset\000"
	.type	__func__.22619, %object
	.size	__func__.22619, 17
__func__.22619:
	.ascii	"mpp_enc_oneframe\000"
	.type	__func__.22627, %object
	.size	__func__.22627, 16
__func__.22627:
	.ascii	"mpp_enc_cfg_reg\000"
	.type	__FUNCTION__.22630, %object
	.size	__FUNCTION__.22630, 16
__FUNCTION__.22630:
	.ascii	"mpp_enc_cfg_reg\000"
	.type	__func__.22638, %object
	.size	__func__.22638, 17
__func__.22638:
	.ascii	"mpp_enc_hw_start\000"
	.type	__func__.22653, %object
	.size	__func__.22653, 17
__func__.22653:
	.ascii	"mpp_enc_run_task\000"
	.type	__func__.22660, %object
	.size	__func__.22660, 25
__func__.22660:
	.ascii	"mpp_enc_check_hw_running\000"
	.type	__func__.22665, %object
	.size	__func__.22665, 25
__func__.22665:
	.ascii	"mpp_enc_unbind_jpeg_task\000"
	.type	__func__.22671, %object
	.size	__func__.22671, 29
__func__.22671:
	.ascii	"mpp_enc_check_is_int_process\000"
	.type	__func__.22681, %object
	.size	__func__.22681, 20
__func__.22681:
	.ascii	"mpp_enc_int_process\000"
	.type	__func__.22691, %object
	.size	__func__.22691, 19
__func__.22691:
	.ascii	"mpp_enc_proc_debug\000"
	.type	__func__.22699, %object
	.size	__func__.22699, 21
__func__.22699:
	.ascii	"mpp_enc_register_chl\000"
	.type	__func__.22705, %object
	.size	__func__.22705, 15
__func__.22705:
	.ascii	"mpp_enc_notify\000"
	.type	__func__.22713, %object
	.size	__func__.22713, 16
__func__.22713:
	.ascii	"mpp_enc_control\000"
	.type	__param_str_mpp_enc_debug, %object
	.size	__param_str_mpp_enc_debug, 25
__param_str_mpp_enc_debug:
	.ascii	"mpp_vcodec.mpp_enc_debug\000"
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	mpp_enc_debug, %object
	.size	mpp_enc_debug, 4
mpp_enc_debug:
	.space	4
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_mpp_enc_debug163, %object
	.size	__UNIQUE_ID_mpp_enc_debug163, 61
__UNIQUE_ID_mpp_enc_debug163:
	.ascii	"mpp_vcodec.parm=mpp_enc_debug:bits mpp_enc debug in"
	.ascii	"formation\000"
	.type	__UNIQUE_ID_mpp_enc_debugtype162, %object
	.size	__UNIQUE_ID_mpp_enc_debugtype162, 39
__UNIQUE_ID_mpp_enc_debugtype162:
	.ascii	"mpp_vcodec.parmtype=mpp_enc_debug:uint\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: found NULL input\012\000"
.LC1:
	.ascii	"\0016%d: buf_pool_destroy in\000"
.LC2:
	.ascii	"\0016%d: buf_pool_destroy out\000"
.LC3:
	.ascii	"smart\000"
.LC4:
	.ascii	"\0013%s:%d: failed to malloc context\012\000"
.LC5:
	.ascii	"\0013%s:%d: could not init enc refs\012\000"
.LC6:
	.ascii	"\0013%s:%d: could not init enc hal\012\000"
.LC7:
	.ascii	"\0013%s:%d: could not init impl\012\000"
.LC8:
	.ascii	"\000"
.LC9:
	.ascii	"\0016%s:%d: %p in\012\000"
.LC10:
	.ascii	"\0016%s:%d: %p out\012\000"
.LC11:
	.ascii	"\0013%s:%d: found NULL input enc\012\000"
.LC12:
	.ascii	"mpp_enc\000"
.LC13:
	.ascii	"\0016%d: alloc mv_flag failed!\012\000"
.LC14:
	.ascii	"\0016%s:%d: %p in flag %08x\012\000"
.LC15:
	.ascii	"\0013%s:%d: found NULL enc\012\000"
.LC16:
	.ascii	"\0013%s:%d: found NULL param enc %p cmd %x\012\000"
.LC17:
	.ascii	"\0016%s:%d: sending cmd %d param %p\012\000"
.LC18:
	.ascii	"\0016%s:%d: get all config\012\000"
.LC19:
	.ascii	"\0016%s:%d: set prep config\012\000"
.LC20:
	.ascii	"\0016%s:%d: set codec config\012\000"
.LC21:
	.ascii	"\0016%s:%d: get prep config\012\000"
.LC22:
	.ascii	"\0016%s:%d: get rc config\012\000"
.LC23:
	.ascii	"\0016%s:%d: get codec config\012\000"
.LC24:
	.ascii	"\0016%s:%d: get header mode\012\000"
.LC25:
	.ascii	"\0016%s:%d: get ref config\012\000"
.LC26:
	.ascii	"\0016%s:%d: get roi config\012\000"
.LC27:
	.ascii	"\0016%s:%d: sending cmd %d done\012\000"
	.section	__param,"a"
	.align	2
	.type	__param_mpp_enc_debug, %object
	.size	__param_mpp_enc_debug, 20
__param_mpp_enc_debug:
	.word	__param_str_mpp_enc_debug
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	mpp_enc_debug
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
