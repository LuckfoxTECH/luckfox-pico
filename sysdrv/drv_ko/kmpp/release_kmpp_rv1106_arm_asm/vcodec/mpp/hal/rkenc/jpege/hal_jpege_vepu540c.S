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
	.file	"hal_jpege_vepu540c.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	hal_jpege_v540c_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_v540c_start, %function
hal_jpege_v540c_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r5, r0
	ldr	r7, .L24
	.pad #28
	sub	sp, sp, #28
	mov	r4, r1
	ldrd	r6, r8, [r0, #68]
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L2
	mov	r3, #368
	ldr	r1, .L24+4
	mov	r2, r3
	ldr	r0, .L24+8
	bl	printk
.L2:
	ldr	r3, [r4, #92]
	cbz	r3, .L3
	mov	r2, #372
	ldr	r1, .L24+4
	ldr	r0, .L24+12
	mov	r4, #-1
	bl	printk
.L1:
	mov	r0, r4
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L3:
	mov	r2, #292
	mov	r1, r3
	strd	r2, r3, [sp, #4]
	mov	r2, sp
	ldr	r0, [r5, #64]
	str	r6, [sp]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L5
	mov	r3, r0
	mov	r2, #382
.L22:
	ldr	r1, .L24+4
	ldr	r0, .L24+16
.L21:
	bl	printk
	b	.L1
.L5:
	add	r3, r6, #824
	mov	r0, #3104
	str	r3, [sp]
	mov	r1, r4
	movw	r3, #8672
	mov	r2, sp
	strd	r0, r3, [sp, #4]
	ldr	r0, [r5, #64]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L6
	mov	r3, r0
	mov	r2, #392
	b	.L22
.L6:
	mov	r1, r0
	mov	r2, sp
	ldr	r0, [r5, #64]
	add	r3, r6, #292
	mov	r9, #624
	str	r3, [sp]
	mov	r3, #532
	strd	r3, r9, [sp, #4]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L7
	mov	r3, r0
	mov	r2, #402
	b	.L22
.L7:
	mov	r1, r0
	mov	r2, sp
	ldr	r0, [r5, #64]
	mov	r3, #12288
	addw	r6, r6, #3928
	str	r3, [sp, #8]
	strd	r6, r9, [sp]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L8
	mov	r3, r0
	mov	r2, #412
	b	.L22
.L8:
	movs	r1, #4
	movs	r3, #44
	add	r2, sp, #12
	strd	r1, r3, [sp, #16]
	ldr	r0, [r5, #64]
	movs	r1, #1
	str	r8, [sp, #12]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L9
	mov	r3, r0
	mov	r2, #422
.L23:
	ldr	r1, .L24+4
	ldr	r0, .L24+20
	b	.L21
.L9:
	add	r3, r8, #4
	mov	r2, #592
	str	r3, [sp, #12]
	movs	r1, #1
	mov	r3, #16384
	ldr	r0, [r5, #64]
	strd	r2, r3, [sp, #16]
	add	r2, sp, #12
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L10
	mov	r3, r0
	mov	r2, #432
	b	.L23
.L10:
	mov	r2, r0
	movs	r1, #5
	ldr	r0, [r5, #64]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L11
	mov	r3, r0
	mov	r2, #438
	ldr	r1, .L24+4
	ldr	r0, .L24+24
	bl	printk
.L11:
	movs	r3, #1
	str	r3, [r5, #304]
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L1
	mov	r3, #440
	ldr	r1, .L24+4
	mov	r2, r3
	ldr	r0, .L24+28
	b	.L21
.L25:
	.align	2
.L24:
	.word	hal_jpege_debug
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.fnend
	.size	hal_jpege_v540c_start, .-hal_jpege_v540c_start
	.align	1
	.global	hal_jpege_v540c_wait
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_v540c_wait, %function
hal_jpege_v540c_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r5, .L88
	mov	r7, r1
	ldr	r8, [r0, #72]
	ldr	r3, [r5]
	lsls	r0, r3, #31
	bpl	.L27
	movw	r3, #493
	ldr	r1, .L88+4
	mov	r2, r3
	ldr	r0, .L88+8
	bl	printk
.L27:
	ldr	r3, [r7, #92]
	cbz	r3, .L28
	movw	r2, #497
	ldr	r1, .L88+4
	ldr	r0, .L88+12
	bl	printk
.L87:
	mov	r6, #-1
.L26:
	mov	r0, r6
	pop	{r4, r5, r6, r7, r8, pc}
.L28:
	mov	r2, r3
	movs	r1, #6
	ldr	r0, [r4, #64]
	bl	mpp_dev_ioctl
	mov	r6, r0
	cbz	r0, .L30
	mov	r3, r0
	movw	r2, #503
	ldr	r1, .L88+4
	mvn	r6, #1008
	ldr	r0, .L88+16
	bl	printk
.L31:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L26
	mov	r3, #512
	ldr	r1, .L88+4
	mov	r2, r3
	ldr	r0, .L88+20
	bl	printk
	b	.L26
.L30:
	ldr	r3, [r4, #72]
	ldr	r4, [r3]
	lsls	r1, r4, #30
	bpl	.L32
	ldr	r3, [r5]
	lsls	r2, r3, #29
	bpl	.L32
	movw	r2, #453
	ldr	r1, .L88+24
	ldr	r0, .L88+28
	bl	printk
.L32:
	lsls	r3, r4, #31
	bpl	.L33
	ldr	r3, [r5]
	lsls	r0, r3, #29
	bpl	.L33
	mov	r2, #456
	ldr	r1, .L88+24
	ldr	r0, .L88+32
	bl	printk
.L33:
	lsls	r1, r4, #28
	bpl	.L34
	ldr	r3, [r5]
	lsls	r2, r3, #29
	bpl	.L34
	movw	r2, #459
	ldr	r1, .L88+24
	ldr	r0, .L88+36
	bl	printk
.L34:
	lsls	r3, r4, #29
	bpl	.L35
	ldr	r3, [r5]
	lsls	r0, r3, #29
	bpl	.L35
	mov	r2, #462
	ldr	r1, .L88+24
	ldr	r0, .L88+40
	bl	printk
.L35:
	lsls	r1, r4, #27
	bpl	.L36
	movw	r2, #465
	ldr	r1, .L88+24
	ldr	r0, .L88+44
	bl	printk
.L36:
	lsls	r2, r4, #26
	bpl	.L37
	mov	r2, #468
	ldr	r1, .L88+24
	ldr	r0, .L88+48
	bl	printk
.L37:
	lsls	r3, r4, #25
	bpl	.L38
	movw	r2, #471
	ldr	r1, .L88+24
	ldr	r0, .L88+52
	bl	printk
.L38:
	lsls	r0, r4, #24
	bpl	.L39
	mov	r2, #474
	ldr	r1, .L88+24
	ldr	r0, .L88+56
	bl	printk
.L39:
	lsls	r1, r4, #23
	bpl	.L40
	movw	r2, #477
	ldr	r1, .L88+24
	ldr	r0, .L88+60
	bl	printk
.L40:
	lsls	r2, r4, #18
	bmi	.L41
	ldr	r3, [r7, #44]
	ldr	r2, [r8, #104]
	add	r3, r3, r2
	str	r3, [r7, #44]
	b	.L31
.L41:
	mov	r2, #480
	ldr	r1, .L88+24
	ldr	r0, .L88+64
	bl	printk
	b	.L87
.L89:
	.align	2
.L88:
	.word	hal_jpege_debug
	.word	.LANCHOR0+22
	.word	.LC0
	.word	.LC6
	.word	.LC7
	.word	.LC5
	.word	.LANCHOR0+43
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
	.size	hal_jpege_v540c_wait, .-hal_jpege_v540c_wait
	.align	1
	.global	hal_jpege_v540c_get_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_v540c_get_task, %function
hal_jpege_v540c_get_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r5, r0
	ldr	r7, .L106
	mov	r4, r1
	ldr	r6, [r1, #16]
	ldr	r3, [r7]
	lsls	r2, r3, #31
	bpl	.L91
	movw	r3, #522
	ldr	r1, .L106+4
	mov	r2, r3
	ldr	r0, .L106+8
	bl	printk
.L91:
	movs	r2, #140
	mov	r1, r6
	add	r0, r5, #160
	bl	memcpy
	ldr	r3, [r5, #80]
	str	r3, [r5, #84]
	ldr	r0, [r4, #52]
	bl	mpp_frame_get_osd
	ldr	r3, [r5, #108]
	str	r0, [r5, #100]
	ldr	r3, [r3, #108]
	cmp	r3, #2
	beq	.L92
	ldr	r3, [r5, #308]
	ldr	r2, [r4, #4]
	cbnz	r3, .L93
	ldr	r3, [r6, #52]
	cbz	r3, .L96
	rsb	r3, r3, #100
.L94:
	str	r3, [r2, #172]
	ldr	r2, [r4, #4]
	ldr	r3, [r6, #60]
	rsb	r3, r3, #100
	str	r3, [r2, #180]
	ldr	r2, [r4, #4]
	ldr	r3, [r6, #56]
	rsb	r3, r3, #100
	str	r3, [r2, #176]
	ldr	r2, [r4, #4]
	ldrb	r3, [r2, #152]	@ zero_extendqisi2
	orr	r3, r3, #16
	strb	r3, [r2, #152]
.L92:
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L95
	movw	r3, #542
	ldr	r1, .L106+4
	mov	r2, r3
	ldr	r0, .L106+12
	bl	printk
.L95:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L96:
	movs	r3, #80
	b	.L94
.L93:
	ldr	r3, [r5, #320]
	str	r3, [r2, #172]
	ldr	r2, [r4, #4]
	ldr	r3, [r6, #60]
	rsb	r3, r3, #100
	str	r3, [r2, #180]
	ldr	r2, [r4, #4]
	ldr	r3, [r6, #56]
	rsb	r3, r3, #100
	str	r3, [r2, #176]
	b	.L92
.L107:
	.align	2
.L106:
	.word	hal_jpege_debug
	.word	.LANCHOR0+75
	.word	.LC0
	.word	.LC5
	.fnend
	.size	hal_jpege_v540c_get_task, .-hal_jpege_v540c_get_task
	.align	1
	.global	hal_jpege_v540c_ret_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_v540c_ret_task, %function
hal_jpege_v540c_ret_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r0
	ldr	r5, .L116
	mov	r4, r1
	ldr	r6, [r1, #4]
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L109
	mov	r3, #552
	ldr	r1, .L116+4
	mov	r2, r3
	ldr	r0, .L116+8
	bl	printk
.L109:
	ldr	r3, [r4, #4]
	ldr	r3, [r3, #172]
	str	r3, [r7, #320]
	ldrd	r3, r2, [r4, #44]
	add	r2, r2, r3
	lsls	r3, r3, #3
	str	r2, [r4, #48]
	str	r3, [r6, #184]
	ldr	r3, [r6, #172]
	str	r3, [r6, #188]
	ldr	r4, [r4, #108]
	cbz	r4, .L110
	movw	r1, #563
	ldr	r0, .L116+12
	bl	printk
	mov	r0, #-1
.L108:
	pop	{r3, r4, r5, r6, r7, pc}
.L110:
	ldr	r0, [r5]
	str	r4, [r7, #304]
	ands	r0, r0, #1
	beq	.L108
	mov	r3, #568
	ldr	r1, .L116+4
	mov	r2, r3
	ldr	r0, .L116+16
	bl	printk
	mov	r0, r4
	b	.L108
.L117:
	.align	2
.L116:
	.word	hal_jpege_debug
	.word	.LANCHOR0+100
	.word	.LC0
	.word	.LC18
	.word	.LC5
	.fnend
	.size	hal_jpege_v540c_ret_task, .-hal_jpege_v540c_ret_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_vepu540c_prepare, %function
hal_jpege_vepu540c_prepare:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L127
	ldr	r6, [r0, #128]
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L119
	mov	r3, r0
	movs	r2, #194
	ldr	r1, .L127+4
	ldr	r0, .L127+8
	bl	printk
.L119:
	ldr	r3, [r4, #108]
	mov	r0, r6
	ldr	r1, [r3, #36]
	bl	vepu541_set_fmt
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L120
	mov	r3, r4
	movs	r2, #196
	ldr	r1, .L127+4
	ldr	r0, .L127+12
	bl	printk
.L120:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L128:
	.align	2
.L127:
	.word	hal_jpege_debug
	.word	.LANCHOR0+125
	.word	.LC19
	.word	.LC20
	.fnend
	.size	hal_jpege_vepu540c_prepare, .-hal_jpege_vepu540c_prepare
	.global	__aeabi_idiv
	.align	1
	.global	hal_jpege_vepu_rc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_vepu_rc, %function
hal_jpege_vepu_rc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r4, r0
	ldr	r2, [r1, #4]
	ldr	r1, [r0, #320]
	ldr	r3, [r2, #172]
	cmp	r3, r1
	beq	.L130
	ldr	r1, .L143
	rsb	r3, r3, #100
	str	r3, [r0, #308]
	ldr	r1, [r1]
	lsls	r1, r1, #27
	bpl	.L131
	ldr	r2, [r2, #172]
	ldr	r1, .L143+4
	ldr	r0, .L143+8
	str	r2, [sp]
	movs	r2, #89
	bl	printk
.L131:
	ldr	r1, [r4, #308]
	cmp	r1, #49
	bgt	.L132
	movw	r0, #5000
	bl	__aeabi_idiv
.L133:
	ldr	r5, .L143+12
	movs	r2, #0
	ldr	r1, .L143+16
	ldr	ip, .L143+20
.L136:
	ldrb	lr, [r5], #1	@ zero_extendqisi2
	ldrb	r3, [r1], #1	@ zero_extendqisi2
	mul	lr, r0, lr
	muls	r3, r0, r3
	add	lr, lr, #50
	smull	r6, r7, lr, ip
	adds	r3, r3, #50
	asr	lr, lr, #31
	rsb	lr, lr, r7, asr #5
	smull	r6, r7, r3, ip
	sxth	lr, lr
	asrs	r3, r3, #31
	cmp	lr, #0
	rsb	r3, r3, r7, asr #5
	sxth	r3, r3
	ble	.L137
	cmp	lr, #255
	it	ge
	movge	lr, #255
	uxtb	lr, lr
.L134:
	ldr	r6, [r4, #312]
	cmp	r3, #0
	strb	lr, [r6, r2]
	ble	.L138
	cmp	r3, #255
	it	ge
	movge	r3, #255
	uxtb	r3, r3
.L135:
	ldr	r6, [r4, #316]
	strb	r3, [r6, r2]
	adds	r2, r2, #1
	cmp	r2, #64
	bne	.L136
.L130:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L132:
	lsls	r0, r1, #1
	rsb	r0, r0, #200
	b	.L133
.L137:
	mov	lr, #1
	b	.L134
.L138:
	movs	r3, #1
	b	.L135
.L144:
	.align	2
.L143:
	.word	hal_jpege_debug
	.word	.LANCHOR0+152
	.word	.LC21
	.word	jpege_luma_quantizer
	.word	jpege_chroma_quantizer
	.word	1374389535
	.fnend
	.size	hal_jpege_vepu_rc, .-hal_jpege_vepu_rc
	.global	__aeabi_uidiv
	.align	1
	.global	hal_jpege_v540c_gen_regs
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_v540c_gen_regs, %function
hal_jpege_v540c_gen_regs:
	.fnstart
	@ args = 0, pretend = 0, frame = 80
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #84
	sub	sp, sp, #84
	mov	r5, r0
	ldr	r4, [r0, #68]
	ldr	r7, [r0, #156]
	mov	r6, r1
	ldr	r0, [r1, #28]
	movs	r3, #0
	strd	r3, r3, [sp, #12]
	bl	mpp_packet_get_length
	ldr	r3, [r6, #32]
	add	r9, r5, #160
	ldr	r1, .L175
	mov	r10, r0
	ldr	r0, [r3]
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r3, [r6, #32]
	ldr	r2, [r3, #12]
	ldr	fp, [r3, #24]
	ldr	r3, .L175+4
	add	r8, r0, r2
	ldr	r2, [r3]
	str	r3, [sp, #4]
	tst	r2, #1
	beq	.L146
	mov	r3, #264
	ldr	r1, .L175+8
	mov	r2, r3
	ldr	r0, .L175+12
	bl	printk
.L146:
	add	r3, r4, #692
	str	r3, [sp, #24]
	ldr	r3, [r5, #64]
	mov	r2, #5408
	movs	r1, #0
	mov	r0, r4
	str	r6, [sp, #32]
	str	r3, [sp, #20]
	ldr	r3, [r5, #128]
	str	r3, [sp, #36]
	ldr	r3, [r5, #300]
	str	r3, [sp, #40]
	bl	memset
	mov	r2, fp
	mov	r1, r8
	mov	r0, r7
	bl	jpege_bits_setup
	lsl	r1, r10, #3
	mov	r0, r7
	bl	jpege_seek_bits
	ldr	r3, [r5, #108]
	ldr	r3, [r3, #108]
	cmp	r3, #2
	beq	.L147
	mov	r1, r6
	mov	r0, r5
	bl	hal_jpege_vepu_rc
	ldr	r3, [r5, #312]
	str	r3, [sp, #12]
	ldr	r3, [r5, #316]
.L174:
	add	r2, sp, #12
	mov	r1, r9
	mov	r0, r7
	str	r3, [sp, #16]
	bl	write_jpeg_header
	mov	r0, r7
	bl	jpege_bits_get_bitpos
	adds	r1, r0, #7
	ldr	r0, [r6, #28]
	asrs	r1, r1, #3
	str	r1, [r6, #48]
	bl	mpp_packet_set_length
	ldr	r3, [r6, #32]
	ldr	r0, [r3]
	cmp	r0, #0
	beq	.L149
	ldr	r1, .L175
	bl	mpp_buffer_get_dma_with_caller
	ldr	r2, [r6, #32]
	ldr	r3, [r6, #48]
	movs	r1, #1
	ldr	r2, [r2, #12]
	bl	dma_buf_end_cpu_access_partial
.L150:
	movs	r0, #0
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	strb	r0, [r4, #16]
	ldr	r2, [r5, #112]
	bfi	r3, r2, #0, #3
	strb	r3, [r4, #17]
	ldrb	r3, [r4, #20]	@ zero_extendqisi2
	movs	r2, #2
	bic	r3, r3, #3
	strb	r3, [r4, #20]
	ldrh	r3, [r4, #32]
	and	r3, r3, #56320
	orr	r3, r3, #8640
	orr	r3, r3, #63
	strh	r3, [r4, #32]	@ movhi
	ldrb	r3, [r4, #436]	@ zero_extendqisi2
	bfi	r3, r2, #0, #2
	strb	r3, [r4, #436]
	movs	r3, #7
	str	r3, [r4, #48]
	ldrh	r3, [r4, #54]
	bfi	r3, r0, #0, #10
	strh	r3, [r4, #54]	@ movhi
	mvn	r3, #14680064
	str	r3, [r4, #56]
	ldr	r3, [r5, #300]
	cbz	r3, .L151
	add	r2, sp, #44
	movs	r1, #19
	bl	rk_dvbm_ctrl
	ldrb	r3, [r4, #96]	@ zero_extendqisi2
	and	r3, r3, #8
	orn	r3, r3, #120
	strb	r3, [r4, #96]
	ldrh	r3, [r4, #24]
	ldr	r2, [sp, #72]
	bfi	r3, r2, #0, #14
	strh	r3, [r4, #24]	@ movhi
	ldrb	r2, [sp, #68]	@ zero_extendqisi2
	ldr	r3, [r4, #24]
	bfi	r3, r2, #14, #8
	str	r3, [r4, #24]
	strb	r2, [r4, #444]
	lsrs	r3, r3, #24
	bic	r3, r3, #7
	orr	r3, r3, #7
	strb	r3, [r4, #27]
	ldrb	r3, [r4, #446]	@ zero_extendqisi2
	bic	r3, r3, #7
	orr	r3, r3, #5
	strb	r3, [r4, #446]
.L151:
	add	r0, sp, #20
	addw	r8, r4, #3544
	bl	vepu540c_set_jpeg_reg
	add	r0, r5, #92
	bl	vepu540c_set_osd
	addw	r10, r4, #3542
	movs	r7, #64
.L152:
	sub	r9, r7, #64
	mov	fp, r10
.L153:
	ldr	r3, [sp, #12]
	mov	r0, #32768
	ldrb	r1, [r3, r9]	@ zero_extendqisi2
	add	r9, r9, #8
	bl	__aeabi_idiv
	cmp	r7, r9
	strh	r0, [fp, #2]!	@ movhi
	bne	.L153
	adds	r7, r7, #1
	add	r10, r10, #16
	cmp	r7, #72
	bne	.L152
	addw	r10, r4, #3670
	movs	r7, #64
.L155:
	sub	r9, r7, #64
	mov	fp, r10
.L156:
	ldr	r3, [sp, #16]
	mov	r0, #32768
	ldrb	r1, [r3, r9]	@ zero_extendqisi2
	add	r9, r9, #8
	bl	__aeabi_idiv
	cmp	r7, r9
	strh	r0, [fp, #2]!	@ movhi
	bne	.L156
	adds	r7, r7, #1
	add	r10, r10, #16
	cmp	r7, #72
	bne	.L155
	addw	r10, r4, #3798
	movs	r7, #64
.L158:
	sub	r9, r7, #64
	mov	fp, r10
.L159:
	ldr	r3, [sp, #16]
	mov	r0, #32768
	ldrb	r1, [r3, r9]	@ zero_extendqisi2
	add	r9, r9, #8
	bl	__aeabi_idiv
	cmp	r7, r9
	strh	r0, [fp, #2]!	@ movhi
	bne	.L159
	adds	r7, r7, #1
	add	r10, r10, #16
	cmp	r7, #72
	bne	.L158
	add	r4, r4, #4224
	adds	r4, r4, #16
	strd	r4, r8, [r6, #100]
	ldr	r3, [r5, #144]
	adds	r3, r3, #1
	str	r3, [r5, #144]
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L161
	mov	r3, #356
	ldr	r1, .L175+8
	mov	r2, r3
	ldr	r0, .L175+16
	bl	printk
.L161:
	movs	r0, #0
	add	sp, sp, #84
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L147:
	movs	r3, #0
	str	r3, [sp, #12]
	b	.L174
.L149:
	ldr	r0, [r5, #64]
	bl	mpp_get_dev
	ldr	r1, [r6, #32]
	movs	r3, #1
	ldr	r2, [r6, #48]
	ldr	r1, [r1, #4]
	bl	dma_sync_single_for_device
	b	.L150
.L176:
	.align	2
.L175:
	.word	.LANCHOR0+170
	.word	hal_jpege_debug
	.word	.LANCHOR0+195
	.word	.LC0
	.word	.LC5
	.fnend
	.size	hal_jpege_v540c_gen_regs, .-hal_jpege_v540c_gen_regs
	.align	1
	.global	hal_jpege_vepu_init_rc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_vepu_init_rc, %function
hal_jpege_vepu_init_rc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r2, #16
	movs	r1, #0
	mov	r4, r0
	bl	memset
	movs	r1, #64
	ldr	r0, .L184
	bl	mpp_osal_malloc
	movs	r1, #64
	str	r0, [r4, #4]
	ldr	r0, .L184
	bl	mpp_osal_malloc
	ldr	r3, [r4, #4]
	str	r0, [r4, #8]
	cbz	r3, .L178
	cbnz	r0, .L180
.L178:
	movs	r2, #117
	ldr	r1, .L184+4
	ldr	r0, .L184+8
	bl	printk
	mvn	r0, #3
.L177:
	pop	{r4, pc}
.L180:
	movs	r0, #0
	b	.L177
.L185:
	.align	2
.L184:
	.word	.LANCHOR0+220
	.word	.LANCHOR0+243
	.word	.LC22
	.fnend
	.size	hal_jpege_vepu_init_rc, .-hal_jpege_vepu_init_rc
	.align	1
	.global	hal_jpege_v540c_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_v540c_init, %function
hal_jpege_v540c_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r5, r1
	ldr	r7, .L198
	mov	r4, r0
	ldr	r3, [r7]
	lsls	r1, r3, #31
	bpl	.L187
	movs	r3, #138
	ldr	r1, .L198+4
	mov	r2, r3
	ldr	r0, .L198+8
	bl	printk
.L187:
	mov	r1, #596
	ldr	r0, .L198+12
	bl	mpp_osal_calloc
	mov	r1, #5408
	str	r0, [r4, #72]
	ldr	r0, .L198+12
	bl	mpp_osal_calloc
	movs	r1, #28
	str	r0, [r4, #68]
	ldr	r0, .L198+12
	bl	mpp_osal_calloc
	movs	r1, #16
	str	r0, [r4, #128]
	add	r0, r5, #36
	ldr	r3, [r5, #4]
	str	r3, [r4, #108]
	movs	r3, #0
	str	r3, [r4, #88]
	movs	r3, #1
	str	r3, [r4, #112]
	ldr	r3, [r5, #8]
	str	r3, [r4, #300]
	str	r1, [r5, #32]
	bl	mpp_dev_init
	mov	r6, r0
	cbz	r0, .L188
	mov	r3, r0
	ldr	r1, .L198+4
	ldr	r0, .L198+16
	movs	r2, #151
.L197:
	bl	printk
.L186:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L188:
	ldr	r2, [r5, #36]
	ldr	r3, [r4, #68]
	str	r0, [r4, #100]
	add	r0, r4, #156
	add	r3, r3, #4224
	str	r2, [r4, #64]
	adds	r3, r3, #16
	strd	r3, r2, [r4, #92]
	bl	jpege_bits_init
	ldr	r3, [r4, #156]
	cbnz	r3, .L190
	movs	r1, #160
	ldr	r3, .L198+12
	str	r1, [sp]
	ldr	r2, .L198+20
	ldr	r0, .L198+24
	bl	printk
.L190:
	add	r0, r4, #308
	bl	hal_jpege_vepu_init_rc
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L186
	movs	r3, #162
	ldr	r1, .L198+4
	mov	r2, r3
	ldr	r0, .L198+28
	b	.L197
.L199:
	.align	2
.L198:
	.word	hal_jpege_debug
	.word	.LANCHOR0+266
	.word	.LC0
	.word	.LANCHOR0+287
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC5
	.fnend
	.size	hal_jpege_v540c_init, .-hal_jpege_v540c_init
	.align	1
	.global	hal_jpege_vepu_deinit_rc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_vepu_deinit_rc, %function
hal_jpege_vepu_deinit_rc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r1, [r0, #4]
	cbz	r1, .L201
	ldr	r0, .L209
	bl	mpp_osal_free
.L201:
	ldr	r1, [r4, #8]
	movs	r5, #0
	str	r5, [r4, #4]
	cbz	r1, .L202
	ldr	r0, .L209
	bl	mpp_osal_free
.L202:
	str	r5, [r4, #8]
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L210:
	.align	2
.L209:
	.word	.LANCHOR0+308
	.fnend
	.size	hal_jpege_vepu_deinit_rc, .-hal_jpege_vepu_deinit_rc
	.align	1
	.global	hal_jpege_v540c_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_v540c_deinit, %function
hal_jpege_v540c_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L236
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L212
	movs	r3, #170
	ldr	r1, .L236+4
	mov	r2, r3
	ldr	r0, .L236+8
	bl	printk
.L212:
	ldr	r0, [r4, #156]
	bl	jpege_bits_deinit
	add	r0, r4, #308
	bl	hal_jpege_vepu_deinit_rc
	ldr	r1, [r4, #68]
	cbz	r1, .L213
	ldr	r0, .L236+12
	bl	mpp_osal_free
.L213:
	ldr	r1, [r4, #72]
	movs	r6, #0
	str	r6, [r4, #68]
	cbz	r1, .L214
	ldr	r0, .L236+12
	bl	mpp_osal_free
.L214:
	ldr	r1, [r4, #128]
	str	r6, [r4, #72]
	cbz	r1, .L215
	ldr	r0, .L236+12
	bl	mpp_osal_free
.L215:
	ldr	r0, [r4, #64]
	movs	r6, #0
	str	r6, [r4, #128]
	cbz	r0, .L216
	bl	mpp_dev_deinit
	str	r6, [r4, #64]
.L216:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L217
	movs	r3, #185
	ldr	r1, .L236+4
	mov	r2, r3
	ldr	r0, .L236+16
	bl	printk
.L217:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L237:
	.align	2
.L236:
	.word	hal_jpege_debug
	.word	.LANCHOR0+333
	.word	.LC0
	.word	.LANCHOR0+356
	.word	.LC5
	.fnend
	.size	hal_jpege_v540c_deinit, .-hal_jpege_v540c_deinit
	.global	hal_jpege_vepu540c
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.36416, %object
	.size	__func__.36416, 22
__func__.36416:
	.ascii	"hal_jpege_v540c_start\000"
	.type	__func__.36432, %object
	.size	__func__.36432, 21
__func__.36432:
	.ascii	"hal_jpege_v540c_wait\000"
	.type	__func__.36423, %object
	.size	__func__.36423, 32
__func__.36423:
	.ascii	"hal_jpege_vepu540c_status_check\000"
	.type	__func__.36439, %object
	.size	__func__.36439, 25
__func__.36439:
	.ascii	"hal_jpege_v540c_get_task\000"
	.type	__func__.36446, %object
	.size	__func__.36446, 25
__func__.36446:
	.ascii	"hal_jpege_v540c_ret_task\000"
	.type	__func__.36361, %object
	.size	__func__.36361, 27
__func__.36361:
	.ascii	"hal_jpege_vepu540c_prepare\000"
	.type	__func__.36324, %object
	.size	__func__.36324, 18
__func__.36324:
	.ascii	"hal_jpege_vepu_rc\000"
	.type	__FUNCTION__.36378, %object
	.size	__FUNCTION__.36378, 25
__FUNCTION__.36378:
	.ascii	"hal_jpege_v540c_gen_regs\000"
	.type	__func__.36383, %object
	.size	__func__.36383, 25
__func__.36383:
	.ascii	"hal_jpege_v540c_gen_regs\000"
	.type	__FUNCTION__.36333, %object
	.size	__FUNCTION__.36333, 23
__FUNCTION__.36333:
	.ascii	"hal_jpege_vepu_init_rc\000"
	.type	__func__.36334, %object
	.size	__func__.36334, 23
__func__.36334:
	.ascii	"hal_jpege_vepu_init_rc\000"
	.type	__func__.36346, %object
	.size	__func__.36346, 21
__func__.36346:
	.ascii	"hal_jpege_v540c_init\000"
	.type	__FUNCTION__.36347, %object
	.size	__FUNCTION__.36347, 21
__FUNCTION__.36347:
	.ascii	"hal_jpege_v540c_init\000"
	.type	__FUNCTION__.36338, %object
	.size	__FUNCTION__.36338, 25
__FUNCTION__.36338:
	.ascii	"hal_jpege_vepu_deinit_rc\000"
	.type	__func__.36354, %object
	.size	__func__.36354, 23
__func__.36354:
	.ascii	"hal_jpege_v540c_deinit\000"
	.type	__FUNCTION__.36355, %object
	.size	__FUNCTION__.36355, 23
__FUNCTION__.36355:
	.ascii	"hal_jpege_v540c_deinit\000"
	.space	1
	.type	hal_jpege_vepu540c, %object
	.size	hal_jpege_vepu540c, 64
hal_jpege_vepu540c:
	.word	.LC26
	.word	8
	.word	324
	.word	0
	.word	hal_jpege_v540c_init
	.word	hal_jpege_v540c_deinit
	.word	hal_jpege_vepu540c_prepare
	.word	hal_jpege_v540c_get_task
	.word	hal_jpege_v540c_gen_regs
	.word	hal_jpege_v540c_start
	.word	hal_jpege_v540c_wait
	.word	0
	.word	0
	.word	hal_jpege_v540c_ret_task
	.space	8
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016hal_jpege_v540c:%s:%d: (%d) enter\012\000"
.LC1:
	.ascii	"\0013hal_jpege_v540c:%s:%d: enc_task->flags.err %08"
	.ascii	"x, return e arly\000"
.LC2:
	.ascii	"\0013hal_jpege_v540c:%s:%d: set register write fail"
	.ascii	"ed %d\012\000"
.LC3:
	.ascii	"\0013hal_jpege_v540c:%s:%d: set register read faile"
	.ascii	"d %d\012\000"
.LC4:
	.ascii	"\0013hal_jpege_v540c:%s:%d: send cmd failed %d\012\000"
.LC5:
	.ascii	"\0016hal_jpege_v540c:%s:%d: (%d) leave\012\000"
.LC6:
	.ascii	"\0013hal_jpege_v540c:%s:%d: enc_task->flags.err %08"
	.ascii	"x, return early\000"
.LC7:
	.ascii	"\0013hal_jpege_v540c:%s:%d: poll cmd failed %d\012\000"
.LC8:
	.ascii	"\0016hal_jpege_v540c:%s:%d: RKV_ENC_INT_LINKTABLE_F"
	.ascii	"INISH\000"
.LC9:
	.ascii	"\0016hal_jpege_v540c:%s:%d: RKV_ENC_INT_ONE_FRAME_F"
	.ascii	"INISH\000"
.LC10:
	.ascii	"\0016hal_jpege_v540c:%s:%d: RKV_ENC_INT_ONE_SLICE_F"
	.ascii	"INISH\000"
.LC11:
	.ascii	"\0016hal_jpege_v540c:%s:%d: RKV_ENC_INT_SAFE_CLEAR_"
	.ascii	"FINISH\000"
.LC12:
	.ascii	"\0013hal_jpege_v540c:%s:%d: RKV_ENC_INT_BIT_STREAM_"
	.ascii	"OVERFLOW\000"
.LC13:
	.ascii	"\0013hal_jpege_v540c:%s:%d: RKV_ENC_INT_BUS_WRITE_F"
	.ascii	"ULL\000"
.LC14:
	.ascii	"\0013hal_jpege_v540c:%s:%d: RKV_ENC_INT_BUS_WRITE_E"
	.ascii	"RROR\000"
.LC15:
	.ascii	"\0013hal_jpege_v540c:%s:%d: RKV_ENC_INT_BUS_READ_ER"
	.ascii	"ROR\000"
.LC16:
	.ascii	"\0013hal_jpege_v540c:%s:%d: RKV_ENC_INT_TIMEOUT_ERR"
	.ascii	"OR\000"
.LC17:
	.ascii	"\0013hal_jpege_v540c:%s:%d: JPEG BIT_STREAM_OVERFLO"
	.ascii	"W\000"
.LC18:
	.ascii	"\0013hal_jpege_v540c:%d: jpege bit stream overflow\000"
.LC19:
	.ascii	"\0016hal_jpege_v540c:%s:%d: enter %p\012\000"
.LC20:
	.ascii	"\0016hal_jpege_v540c:%s:%d: leave %p\012\000"
.LC21:
	.ascii	"\0016hal_jpege_v540c:%s:%d: use qfactor=%d, rc_info"
	.ascii	"->quality_target=%d\012\000"
.LC22:
	.ascii	"\0013hal_jpege_v540c:%s:%d: qtable is null, malloc "
	.ascii	"err\012\000"
.LC23:
	.ascii	"\0013hal_jpege_v540c:%s:%d: mpp_dev_init failed. re"
	.ascii	"t: %d\012\000"
.LC24:
	.ascii	"ctx->bits\000"
.LC25:
	.ascii	"\0013hal_jpege_v540c:%d: Assertion %s failed at %s:"
	.ascii	"%d\012\000"
.LC26:
	.ascii	"hal_jpege_v540c\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
