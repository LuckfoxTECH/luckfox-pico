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
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	bl	get_mpibuf_ops
	add	r8, r4, #5344
	mov	r7, r0
	mov	r0, r8
	bl	down
	cbnz	r4, .L2
	movs	r2, #168
	ldr	r1, .L70
	ldr	r0, .L70+4
	bl	printk
	mvn	r0, #2
.L1:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L2:
	mov	r0, r4
	bl	mpp_enc_impl_free_task
	ldr	r0, [r4, #4]
	cbz	r0, .L4
	bl	enc_impl_deinit
	movs	r3, #0
	str	r3, [r4, #4]
.L4:
	ldr	r0, [r4, #8]
	cbz	r0, .L5
	bl	mpp_enc_hal_deinit
	movs	r3, #0
	str	r3, [r4, #8]
.L5:
	ldr	r3, [r4, #404]
	cbz	r3, .L6
	add	r0, r4, #404
	bl	mpp_packet_deinit
.L6:
	ldr	r1, [r4, #408]
	cbz	r1, .L7
	ldr	r0, .L70+8
	bl	mpp_osal_free
.L7:
	movw	r5, #5300
	movs	r6, #0
	str	r6, [r4, #408]
	adds	r0, r4, r5
	ldr	r3, [r4, r5]
	cbz	r3, .L8
	bl	mpp_enc_ref_cfg_deinit
	str	r6, [r4, r5]
.L8:
	ldr	r3, [r4, #460]
	cbz	r3, .L9
	add	r0, r4, #460
	bl	mpp_enc_refs_deinit
	movs	r3, #0
	str	r3, [r4, #460]
.L9:
	ldr	r0, [r4, #60]
	cbz	r0, .L10
	bl	rc_deinit
	movs	r3, #0
	str	r3, [r4, #60]
.L10:
	movw	r3, #10728
	ldr	r3, [r4, r3]
	cbz	r3, .L11
	movw	r2, #10756
	ldr	r2, [r4, r2]
	ldr	r2, [r2, #12]
	cbnz	r2, .L12
	ldr	r0, [r3, #20]
	cbz	r0, .L12
	ldr	r1, .L70+8
	bl	mpp_buffer_put_with_caller
.L12:
	movw	r3, #10728
	ldr	r1, [r4, r3]
	mov	r5, r3
	cbz	r1, .L13
	ldr	r0, .L70+8
	bl	mpp_osal_free
.L13:
	movs	r3, #0
	str	r3, [r4, r5]
.L11:
	add	r0, r4, #5472
	adds	r0, r0, #24
	bl	mpp_enc_unref_osd_buf
	movw	r3, #5340
	ldr	r3, [r4, r3]
	cbz	r3, .L14
	movw	r3, #10760
	ldr	r0, [r4, r3]
	cbz	r0, .L15
	ldr	r1, .L70+8
	bl	mpp_buffer_put_with_caller
.L15:
	movw	r3, #10764
	ldr	r0, [r4, r3]
	cbz	r0, .L16
	ldr	r1, .L70+8
	bl	mpp_buffer_put_with_caller
.L16:
	add	r5, r4, #10752
	add	r6, r4, #10752
	ldr	r9, .L70+8
	adds	r5, r5, #20
	adds	r6, r6, #32
.L18:
	ldr	r1, [r5], #4
	cbz	r1, .L17
	mov	r0, r9
	bl	mpp_osal_free
.L17:
	cmp	r5, r6
	bne	.L18
.L14:
	ldr	r1, [r4, #436]
	cbz	r1, .L19
	ldr	r0, .L70+8
	bl	mpp_osal_free
.L19:
	movs	r5, #0
	mov	r0, r8
	str	r5, [r4, #436]
	movw	r6, #5356
	strd	r5, r5, [r4, #444]
	bl	up
	ldr	r3, [r4, r6]
	cbz	r3, .L20
	movs	r1, #234
	ldr	r0, .L70+12
	bl	printk
	ldr	r3, [r7, #52]
	ldr	r0, [r4, r6]
	blx	r3
	movs	r1, #236
	ldr	r0, .L70+16
	bl	printk
	str	r5, [r4, r6]
.L20:
	ldr	r0, .L70+8
	mov	r1, r4
	bl	mpp_osal_free
	movs	r0, #0
	b	.L1
.L71:
	.align	2
.L70:
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
	@ args = 0, pretend = 0, frame = 80
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r2, .L86
	movs	r3, #0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #80
	sub	sp, sp, #80
	mov	r5, r1
	ldr	r7, [r1]
	strd	r3, r3, [sp, #4]
	mov	r8, r0
	str	r2, [sp, #12]
	cbnz	r0, .L73
	movs	r2, #43
	ldr	r1, .L86+4
	ldr	r0, .L86+8
	mvn	r7, #2
	bl	printk
.L72:
	mov	r0, r7
	add	sp, sp, #80
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L73:
	str	r3, [r0]
	movw	r1, #10800
	ldr	r0, .L86+12
	bl	mpp_osal_calloc
	mov	r4, r0
	cbnz	r0, .L75
	movs	r2, #51
	ldr	r1, .L86+4
	ldr	r0, .L86+8
	mvn	r7, #3
	bl	printk
	b	.L72
.L75:
	add	r0, r0, #460
	bl	mpp_enc_refs_init
	mov	r6, r0
	cbz	r0, .L76
	ldr	r1, .L86+4
	movs	r2, #57
	ldr	r0, .L86+16
.L85:
	bl	printk
.L77:
	mov	r0, r4
	mov	r7, r6
	bl	mpp_enc_deinit
	b	.L72
.L76:
	ldr	r1, [r5, #4]
	movs	r2, #29
	strd	r2, r0, [sp, #68]
	add	r3, r4, #600
	ldr	r0, [r5, #8]
	str	r1, [sp, #44]
	ldr	r1, [r5, #16]
	str	r3, [sp, #40]
	str	r3, [sp, #24]
	str	r1, [sp, #48]
	ldr	r1, [r5, #24]
	str	r7, [sp, #36]
	strd	r7, r2, [sp, #16]
	str	r1, [sp, #76]
	ldr	r1, [r5, #28]
	str	r1, [sp, #52]
	ldr	r1, [r5, #20]
	str	r1, [sp, #56]
	ldr	r1, [r5, #36]
	str	r1, [sp, #60]
	ldr	r1, [r5, #40]
	str	r1, [sp, #64]
	movw	r1, #10732
	str	r0, [r4, r1]
	movw	r1, #10736
	ldr	r0, [r5, #12]
	str	r0, [r4, r1]
	movw	r1, #10792
	ldr	r0, [r5, #36]
	str	r0, [r4, r1]
	add	r1, sp, #36
	ldr	r3, [r4, #460]
	add	r0, sp, #8
	str	r3, [sp, #28]
	movs	r3, #2
	str	r3, [sp, #32]
	bl	mpp_enc_hal_init
	mov	r6, r0
	cbz	r0, .L78
	movs	r2, #84
	ldr	r1, .L86+4
	ldr	r0, .L86+20
	b	.L85
.L78:
	ldr	r3, [sp, #68]
	add	r1, sp, #16
	add	r0, sp, #4
	str	r3, [sp, #20]
	mov	r3, #-1
	str	r3, [sp, #32]
	bl	enc_impl_init
	mov	r6, r0
	cbz	r0, .L79
	movs	r2, #93
	ldr	r1, .L86+4
	ldr	r0, .L86+24
	b	.L85
.L79:
	mov	r0, r4
	bl	mpp_enc_impl_alloc_task
	ldr	r2, [r5, #20]
	add	r0, r4, #60
	cbz	r2, .L80
	add	r2, sp, #12
.L80:
	mov	r1, r7
	mov	r6, #1024
	bl	rc_init
	str	r7, [r4]
	mov	r1, r6
	str	r6, [r4, #448]
	ldr	r0, .L86+12
	ldr	r3, [sp, #4]
	str	r3, [r4, #4]
	ldr	r3, [sp, #8]
	str	r3, [r4, #8]
	ldr	r3, [sp, #72]
	str	r3, [r4, #12]
	movs	r3, #0
	str	r3, [r4, #424]
	ldr	r3, .L86+28
	str	r3, [r4, #428]
	movs	r3, #81
	str	r3, [r4, #432]
	bl	mpp_osal_calloc
	mov	r1, r6
	str	r0, [r4, #436]
	ldr	r0, .L86+12
	bl	mpp_osal_calloc
	mov	r2, r6
	mov	r1, r0
	str	r0, [r4, #408]
	add	r0, r4, #404
	bl	mpp_packet_init
	mov	r6, r0
	cmp	r0, #0
	bne	.L77
	mov	r1, r0
	ldr	r0, [r4, #404]
	bl	mpp_packet_set_length
	str	r7, [r4, #1168]
	movw	r7, #5300
	adds	r0, r4, r7
	bl	mpp_enc_ref_cfg_init
	cmp	r0, #0
	bne	.L77
	ldr	r7, [r4, r7]
	bl	mpp_enc_ref_default
	mov	r1, r0
	mov	r0, r7
	bl	mpp_enc_ref_cfg_copy
	cmp	r0, #0
	bne	.L77
	ldr	r7, [r4, #460]
	bl	mpp_enc_ref_default
	mov	r1, r0
	mov	r0, r7
	bl	mpp_enc_refs_set_cfg
	cmp	r0, #0
	bne	.L77
	ldr	r1, [r4, #752]
	ldr	r0, [r4, #460]
	bl	mpp_enc_refs_set_rc_igop
	mov	r7, r0
	cmp	r0, #0
	bne	.L77
	add	r2, r4, #5344
	movs	r1, #1
	mov	r3, r2
	str	r1, [r3], #4
	strd	r3, r3, [r2, #4]
	movw	r3, #5320
	str	r1, [r4, r3]
	movw	r3, #6626
	movs	r2, #4
	strb	r2, [r4, r3]
	bl	get_vsm_ops
	cbnz	r0, .L83
	movs	r1, #44
	ldr	r0, .L86+12
	bl	mpp_osal_calloc
	movw	r3, #10728
	str	r0, [r4, r3]
.L83:
	ldr	r2, [r5, #4]
	movw	r3, #5332
	str	r2, [r4, r3]
	movw	r3, #10756
	ldr	r2, [r5, #24]
	str	r2, [r4, r3]
	movw	r3, #5340
	ldr	r2, [r5, #28]
	str	r2, [r4, r3]
	movw	r3, #10788
	ldr	r2, [r5, #32]
	str	r2, [r4, r3]
	movw	r3, #5336
	ldr	r2, [r5, #16]
	str	r2, [r4, r3]
	str	r4, [r8]
	b	.L72
.L87:
	.align	2
.L86:
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
	ldr	r5, .L97
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L89
	mov	r3, r0
	movs	r2, #247
	ldr	r1, .L97+4
	ldr	r0, .L97+8
	bl	printk
.L89:
	add	r6, r4, #5344
	mov	r0, r6
	bl	down
	movw	r3, #5320
	movs	r2, #0
	mov	r0, r6
	str	r2, [r4, r3]
	bl	up
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L90
	mov	r3, r4
	movs	r2, #254
	ldr	r1, .L97+4
	ldr	r0, .L97+12
	bl	printk
.L90:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L98:
	.align	2
.L97:
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
	add	r6, r0, #5344
	ldr	r5, .L108
	mov	r4, r0
	mov	r0, r6
	bl	down
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L100
	mov	r3, r4
	mov	r2, #264
	ldr	r1, .L108+4
	ldr	r0, .L108+8
	bl	printk
.L100:
	movw	r3, #5320
	movs	r2, #1
	str	r2, [r4, r3]
	movw	r3, #5324
	ldr	r7, [r4, r3]
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L101
	mov	r3, r4
	movw	r2, #267
	ldr	r1, .L108+4
	ldr	r0, .L108+12
	bl	printk
.L101:
	mov	r0, r6
	bl	up
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, pc}
.L109:
	.align	2
.L108:
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
	cbz	r0, .L110
	ldr	r3, [r0, #60]
	cbz	r3, .L110
	movw	r2, #5356
	ldr	r0, [r4, r2]
	pop	{r4, lr}
	bx	r3
.L110:
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
	ldr	r3, .L123
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L117
	mov	r3, r0
	mov	r2, #288
	ldr	r1, .L123+4
	ldr	r0, .L123+8
	bl	printk
.L117:
	cbnz	r4, .L119
	mov	r2, #290
	ldr	r1, .L123+4
	ldr	r0, .L123+12
	bl	printk
	mvn	r0, #2
.L116:
	pop	{r4, pc}
.L119:
	movs	r0, #0
	b	.L116
.L124:
	.align	2
.L123:
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
	cbnz	r0, .L126
	movw	r2, #303
	ldr	r1, .L134
	ldr	r0, .L134+4
	bl	printk
	mvn	r0, #2
.L125:
	pop	{r3, r4, r5, pc}
.L126:
	ldr	r5, .L134+8
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bmi	.L128
.L133:
	movs	r0, #0
	b	.L125
.L128:
	mov	r3, r0
	movw	r2, #307
	ldr	r1, .L134
	ldr	r0, .L134+12
	bl	printk
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L133
	mov	r3, r4
	movw	r2, #309
	ldr	r1, .L134
	ldr	r0, .L134+16
	bl	printk
	b	.L133
.L135:
	.align	2
.L134:
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
	mov	r7, r1
	mov	r4, r0
	cbnz	r0, .L137
	mov	r2, #320
	ldr	r1, .L159
	ldr	r0, .L159+4
	mvn	r7, #2
	bl	printk
.L136:
	mov	r0, r7
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L137:
	ldr	r5, .L159+8
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L139
	mov	r3, r0
	mov	r2, #324
	ldr	r1, .L159
	ldr	r0, .L159+12
	bl	printk
.L139:
	add	r6, r4, #5344
	mov	r0, r6
	bl	down
	movw	r3, #5320
	ldr	r8, [r4, r3]
	cmp	r8, #0
	beq	.L140
	mov	r0, r6
	mov	r7, #-1
	bl	up
	b	.L136
.L140:
	mov	r0, r4
	bl	mpp_enc_proc_rc_update
	movw	r3, #5328
	str	r8, [r4, r3]
	movw	r3, #5340
	ldr	r3, [r4, r3]
	cbnz	r3, .L141
.L143:
	mov	r1, r7
	mov	r0, r4
	bl	mpp_enc_impl_reg_cfg
	movw	r3, #5328
	movs	r2, #1
	str	r2, [r4, r3]
	mov	r7, r0
	mov	r0, r6
	bl	up
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L136
	mov	r3, r4
	mov	r2, #356
	ldr	r1, .L159
	ldr	r0, .L159+16
	bl	printk
	b	.L136
.L141:
	movw	r3, #10760
	ldr	r3, [r4, r3]
	cmp	r3, #0
	bne	.L143
	ldr	r2, [r4, #1168]
	ldrd	r1, r3, [r4, #628]
	cmp	r2, #7
	bne	.L144
	adds	r1, r1, #63
	adds	r3, r3, #63
	bics	r1, r1, #63
	it	mi
	addmi	r1, r1, #15
	bics	r3, r3, #63
	asr	r1, r1, #4
.L158:
	it	mi
	addmi	r3, r3, #15
	ldr	r10, .L159+24
	movs	r0, #0
	asrs	r3, r3, #4
	str	r10, [sp]
	mul	r8, r3, r1
	add	r1, r4, #10752
	ldr	r3, .L159+20
	adds	r1, r1, #8
	lsl	r9, r8, #2
	mov	r2, r9
	bl	mpp_buffer_get_with_tag
	movw	r3, #10764
	ldr	r0, [r4, r3]
	adds	r1, r4, r3
	cbnz	r0, .L150
	str	r10, [sp]
	mov	r2, r9
	ldr	r3, .L159+20
	bl	mpp_buffer_get_with_tag
.L150:
	add	r9, r4, #10752
	add	r10, r4, #10752
	ldr	fp, .L159+28
	add	r9, r9, #20
	add	r10, r10, #32
.L152:
	mov	r1, r8
	ldr	r0, .L159+24
	bl	mpp_osal_calloc
	str	r0, [r9], #4
	cbnz	r0, .L151
	mov	r1, #350
	mov	r0, fp
	bl	printk
.L151:
	cmp	r9, r10
	bne	.L152
	b	.L143
.L144:
	adds	r1, r1, #31
	adds	r3, r3, #31
	bics	r1, r1, #31
	it	mi
	addmi	r1, r1, #15
	bics	r3, r3, #31
	asr	r1, r1, #4
	b	.L158
.L160:
	.align	2
.L159:
	.word	.LANCHOR0+114
	.word	.LC11
	.word	.LANCHOR1
	.word	.LC9
	.word	.LC10
	.word	.LC12
	.word	.LANCHOR0+130
	.word	.LC13
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r8, r1
	mov	r4, r0
	cbnz	r0, .L162
	mov	r2, #366
	ldr	r1, .L173
	ldr	r0, .L173+4
	mvn	r5, #2
	bl	printk
.L161:
	mov	r0, r5
	pop	{r4, r5, r6, r7, r8, pc}
.L162:
	ldr	r7, .L173+8
	ldr	r2, [r7]
	lsls	r2, r2, #31
	bpl	.L164
	mov	r3, r0
	mov	r2, #370
	ldr	r1, .L173
	ldr	r0, .L173+12
	bl	printk
.L164:
	add	r6, r4, #5344
	mov	r0, r6
	bl	down
	movw	r3, #5320
	ldr	r3, [r4, r3]
	cbz	r3, .L165
	mov	r0, r6
	mov	r5, #-1
	bl	up
	b	.L161
.L165:
	movw	r5, #5328
	movs	r3, #2
	mov	r1, r8
	str	r3, [r4, r5]
	mov	r0, r4
	bl	mpp_enc_impl_hw_start
	movs	r3, #3
	str	r3, [r4, r5]
	mov	r5, r0
	cbnz	r0, .L166
	movw	r3, #5324
	movs	r2, #1
	str	r2, [r4, r3]
.L166:
	mov	r0, r6
	bl	up
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L161
	mov	r3, r4
	mov	r2, #382
	ldr	r1, .L173
	ldr	r0, .L173+16
	bl	printk
	b	.L161
.L174:
	.align	2
.L173:
	.word	.LANCHOR0+146
	.word	.LC11
	.word	.LANCHOR1
	.word	.LC9
	.word	.LC10
	.fnend
	.size	mpp_enc_hw_start, .-mpp_enc_hw_start
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
	mov	r7, r1
	mov	r8, r2
	mov	r9, r3
	mov	r4, r0
	cbnz	r0, .L176
	mov	r2, #394
	ldr	r1, .L185
	ldr	r0, .L185+4
	mvn	r7, #2
	bl	printk
.L175:
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L176:
	ldr	r6, .L185+8
	ldr	r2, [r6]
	lsls	r2, r2, #31
	bpl	.L178
	mov	r3, r0
	mov	r2, #398
	ldr	r1, .L185
	ldr	r0, .L185+12
	bl	printk
.L178:
	movw	r5, #5328
	mov	r1, r7
	mov	r2, r8
	movs	r3, #4
	mov	r0, r4
	str	r3, [r4, r5]
	mov	r3, r9
	bl	mpp_enc_impl_int
	movs	r3, #5
	str	r3, [r4, r5]
	add	r5, r4, #5344
	mov	r7, r0
	mov	r0, r5
	bl	down
	movw	r3, #5324
	movs	r2, #0
	mov	r0, r5
	str	r2, [r4, r3]
	bl	up
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L175
	mov	r3, r4
	movw	r2, #405
	ldr	r1, .L185
	ldr	r0, .L185+16
	bl	printk
	b	.L175
.L186:
	.align	2
.L185:
	.word	.LANCHOR0+163
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
	cbnz	r1, .L188
	movw	r2, #413
	ldr	r1, .L194
	ldr	r0, .L194+4
	pop	{r4, r5, r6, r7, r8, lr}
	b	printk
.L188:
	ldr	r5, .L194+8
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L189
	mov	r3, r1
	movw	r2, #417
	ldr	r1, .L194
	ldr	r0, .L194+12
	bl	printk
.L189:
	mov	r2, r7
	mov	r1, r4
	mov	r0, r6
	bl	mpp_enc_impl_poc_debug_info
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L187
	mov	r3, r4
	movw	r2, #421
	ldr	r1, .L194
	ldr	r0, .L194+16
	pop	{r4, r5, r6, r7, r8, lr}
	b	printk
.L187:
	pop	{r4, r5, r6, r7, r8, pc}
.L195:
	.align	2
.L194:
	.word	.LANCHOR0+183
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
	cbnz	r0, .L197
	movw	r2, #433
	ldr	r1, .L199
	ldr	r0, .L199+4
	bl	printk
	mvn	r0, #2
.L196:
	pop	{r3, pc}
.L197:
	ldr	r0, [r0, #12]
	bl	mpp_dev_chnl_register
	movs	r0, #0
	b	.L196
.L200:
	.align	2
.L199:
	.word	.LANCHOR0+202
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
	ldr	r5, .L208
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L203
	mov	r3, r0
	str	r1, [sp]
	mov	r2, #446
	ldr	r1, .L208+4
	ldr	r0, .L208+8
	bl	printk
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L203
	mov	r3, r4
	mov	r2, #448
	ldr	r1, .L208+4
	ldr	r0, .L208+12
	bl	printk
.L203:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L209:
	.align	2
.L208:
	.word	.LANCHOR1
	.word	.LANCHOR0+223
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
	cbnz	r0, .L211
	mov	r2, #464
	ldr	r1, .L278
	ldr	r0, .L278+4
	bl	printk
.L275:
	mvn	r0, #2
.L210:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L211:
	cbnz	r2, .L213
	ldr	r3, .L278+8
	cmp	r1, r3
	beq	.L213
	mov	r3, r0
	str	r1, [sp]
	movw	r2, #469
	ldr	r1, .L278
	ldr	r0, .L278+12
	bl	printk
	b	.L275
.L213:
	ldr	r7, .L278+16
	ldr	r3, [r7]
	lsls	r3, r3, #30
	bpl	.L214
	str	r4, [sp]
	mov	r3, r6
	movw	r2, #473
	ldr	r1, .L278
	ldr	r0, .L278+20
	bl	printk
.L214:
	ldr	r3, .L278+24
	cmp	r6, r3
	beq	.L215
	bhi	.L216
	subs	r3, r3, #4
	cmp	r6, r3
	beq	.L217
	bhi	.L218
	subs	r3, r3, #1
	cmp	r6, r3
	beq	.L219
.L220:
	add	r8, r5, #5344
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
	bpl	.L274
	mov	r3, r6
	mov	r2, #532
	ldr	r1, .L278
	ldr	r0, .L278+28
	bl	printk
	b	.L274
.L218:
	ldr	r3, .L278+32
	cmp	r6, r3
	beq	.L221
	adds	r3, r3, #2
	cmp	r6, r3
	bne	.L220
	ldr	r3, [r7]
	lsls	r7, r3, #30
	bpl	.L233
	mov	r2, #504
	ldr	r1, .L278
	ldr	r0, .L278+36
	bl	printk
.L233:
	movs	r2, #196
	add	r1, r5, #704
	b	.L276
.L216:
	ldr	r3, .L278+40
	cmp	r6, r3
	beq	.L223
	bhi	.L224
	subs	r3, r3, #12
	cmp	r6, r3
	bne	.L220
	ldr	r3, [r7]
	lsls	r6, r3, #30
	bpl	.L234
	movw	r2, #509
	ldr	r1, .L278
	ldr	r0, .L278+44
	bl	printk
.L234:
	mov	r2, #2832
	add	r1, r5, #1168
	b	.L276
.L224:
	ldr	r3, .L278+48
	cmp	r6, r3
	beq	.L226
	add	r3, r3, #512
	cmp	r6, r3
	bne	.L220
	ldr	r3, [r7]
	lsls	r1, r3, #30
	bpl	.L236
	movw	r2, #519
	ldr	r1, .L278
	ldr	r0, .L278+52
	bl	printk
.L236:
	add	r5, r5, #5248
	adds	r5, r5, #28
	add	r3, r5, #24
.L237:
	ldr	r2, [r5], #4	@ unaligned
	cmp	r5, r3
	str	r2, [r4], #4	@ unaligned
	bne	.L237
	b	.L274
.L219:
	ldr	r3, [r7]
	lsls	r0, r3, #30
	bpl	.L228
	movw	r2, #479
	ldr	r1, .L278
	ldr	r0, .L278+56
	bl	printk
.L228:
	movw	r2, #4712
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
.L274:
	movs	r0, #0
	b	.L210
.L217:
	ldr	r3, [r7]
	lsls	r1, r3, #30
	bpl	.L230
	movw	r2, #489
	ldr	r1, .L278
	ldr	r0, .L278+60
	bl	printk
.L230:
	add	r0, r5, #608
	movs	r2, #96
	mov	r1, r4
.L277:
	bl	memcpy
	b	.L274
.L215:
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L231
	mov	r2, #494
	ldr	r1, .L278
	ldr	r0, .L278+64
	bl	printk
.L231:
	mov	r2, #2832
	mov	r1, r4
	add	r0, r5, #1168
	b	.L277
.L221:
	ldr	r3, [r7]
	lsls	r3, r3, #30
	bpl	.L232
	movw	r2, #499
	ldr	r1, .L278
	ldr	r0, .L278+68
	bl	printk
.L232:
	add	r1, r5, #608
	movs	r2, #96
.L276:
	mov	r0, r4
	b	.L277
.L226:
	ldr	r3, [r7]
	lsls	r0, r3, #30
	bpl	.L235
	movw	r2, #514
	ldr	r1, .L278
	ldr	r0, .L278+72
	bl	printk
.L235:
	ldr	r3, [r5, #420]	@ unaligned
	str	r3, [r4]	@ unaligned
	b	.L274
.L223:
	ldr	r3, [r7]
	lsls	r2, r3, #30
	bpl	.L238
	mov	r2, #524
	ldr	r1, .L278
	ldr	r0, .L278+76
	bl	printk
.L238:
	movs	r2, #136
	addw	r1, r5, #4012
	b	.L276
.L279:
	.align	2
.L278:
	.word	.LANCHOR0+238
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
	.global	mpp_enc_debug
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.22357, %object
	.size	__func__.22357, 15
__func__.22357:
	.ascii	"mpp_enc_deinit\000"
	.type	__FUNCTION__.22358, %object
	.size	__FUNCTION__.22358, 15
__FUNCTION__.22358:
	.ascii	"mpp_enc_deinit\000"
	.type	__func__.22347, %object
	.size	__func__.22347, 13
__func__.22347:
	.ascii	"mpp_enc_init\000"
	.type	__FUNCTION__.22348, %object
	.size	__FUNCTION__.22348, 13
__FUNCTION__.22348:
	.ascii	"mpp_enc_init\000"
	.type	__func__.22367, %object
	.size	__func__.22367, 14
__func__.22367:
	.ascii	"mpp_enc_start\000"
	.type	__func__.22373, %object
	.size	__func__.22373, 13
__func__.22373:
	.ascii	"mpp_enc_stop\000"
	.type	__func__.22384, %object
	.size	__func__.22384, 14
__func__.22384:
	.ascii	"mpp_enc_reset\000"
	.type	__func__.22392, %object
	.size	__func__.22392, 17
__func__.22392:
	.ascii	"mpp_enc_oneframe\000"
	.type	__func__.22400, %object
	.size	__func__.22400, 16
__func__.22400:
	.ascii	"mpp_enc_cfg_reg\000"
	.type	__FUNCTION__.22404, %object
	.size	__FUNCTION__.22404, 16
__FUNCTION__.22404:
	.ascii	"mpp_enc_cfg_reg\000"
	.type	__func__.22414, %object
	.size	__func__.22414, 17
__func__.22414:
	.ascii	"mpp_enc_hw_start\000"
	.type	__func__.22423, %object
	.size	__func__.22423, 20
__func__.22423:
	.ascii	"mpp_enc_int_process\000"
	.type	__func__.22430, %object
	.size	__func__.22430, 19
__func__.22430:
	.ascii	"mpp_enc_proc_debug\000"
	.type	__func__.22438, %object
	.size	__func__.22438, 21
__func__.22438:
	.ascii	"mpp_enc_register_chl\000"
	.type	__func__.22444, %object
	.size	__func__.22444, 15
__func__.22444:
	.ascii	"mpp_enc_notify\000"
	.type	__func__.22452, %object
	.size	__func__.22452, 16
__func__.22452:
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
	.ascii	"0cf529e0 author: Zhihua Wang 2022-12-30 [vcodec]: a"
	.ascii	"dd packet num module parameter\000"
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
