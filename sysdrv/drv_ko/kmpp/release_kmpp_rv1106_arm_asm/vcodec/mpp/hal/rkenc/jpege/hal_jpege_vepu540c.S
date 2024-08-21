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
	movw	r3, #495
	ldr	r1, .L24+4
	mov	r2, r3
	ldr	r0, .L24+8
	bl	printk
.L2:
	ldr	r3, [r4, #80]
	cbz	r3, .L3
	movw	r2, #499
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
	movw	r2, #509
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
	movw	r2, #519
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
	movw	r2, #529
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
	movw	r2, #539
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
	movw	r2, #549
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
	movw	r2, #559
	b	.L23
.L10:
	mov	r2, r0
	movs	r1, #5
	ldr	r0, [r5, #64]
	bl	mpp_dev_ioctl
	mov	r4, r0
	cbz	r0, .L11
	mov	r3, r0
	movw	r2, #565
	ldr	r1, .L24+4
	ldr	r0, .L24+24
	bl	printk
.L11:
	movs	r3, #1
	str	r3, [r5, #304]
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L1
	movw	r3, #567
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
	mov	r7, r1
	ldr	r4, .L103
	mov	r5, r0
	ldr	r8, [r0, #72]
	ldr	r3, [r4]
	lsls	r1, r3, #31
	bpl	.L27
	movw	r3, #623
	ldr	r1, .L103+4
	mov	r2, r3
	ldr	r0, .L103+8
	bl	printk
.L27:
	ldr	r3, [r7, #80]
	cbz	r3, .L28
	movw	r2, #627
	ldr	r1, .L103+4
	ldr	r0, .L103+12
	mov	r6, #-1
	bl	printk
.L26:
	mov	r0, r6
	pop	{r4, r5, r6, r7, r8, pc}
.L28:
	mov	r2, r3
	movs	r1, #6
	ldr	r0, [r5, #64]
	bl	mpp_dev_ioctl
	mov	r6, r0
	cbz	r0, .L30
	mov	r3, r0
	movw	r2, #633
	ldr	r1, .L103+4
	mvn	r6, #1008
	ldr	r0, .L103+16
	bl	printk
.L31:
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L26
	movw	r3, #642
	ldr	r1, .L103+4
	mov	r2, r3
	ldr	r0, .L103+20
	bl	printk
	b	.L26
.L30:
	ldr	r3, [r5, #72]
	ldr	r5, [r3]
	lsls	r2, r5, #30
	bpl	.L32
	ldr	r3, [r4]
	lsls	r3, r3, #29
	bpl	.L32
	mov	r2, #580
	ldr	r1, .L103+24
	ldr	r0, .L103+28
	bl	printk
.L32:
	lsls	r0, r5, #31
	bpl	.L33
	ldr	r3, [r4]
	lsls	r1, r3, #29
	bpl	.L33
	movw	r2, #583
	ldr	r1, .L103+24
	ldr	r0, .L103+32
	bl	printk
.L33:
	lsls	r2, r5, #28
	bpl	.L34
	ldr	r3, [r4]
	lsls	r3, r3, #29
	bpl	.L34
	movw	r2, #586
	ldr	r1, .L103+24
	ldr	r0, .L103+36
	bl	printk
.L34:
	lsls	r0, r5, #29
	bpl	.L35
	ldr	r3, [r4]
	lsls	r1, r3, #29
	bpl	.L35
	movw	r2, #589
	ldr	r1, .L103+24
	ldr	r0, .L103+40
	bl	printk
.L35:
	lsls	r2, r5, #27
	ldr	r3, [r4]
	bpl	.L36
	lsls	r0, r3, #22
	bmi	.L37
.L101:
	mvn	r6, #100
	b	.L26
.L37:
	ldr	r1, .L103+24
	mov	r2, #592
	ldr	r0, .L103+44
.L102:
	bl	printk
	b	.L101
.L36:
	lsls	r1, r5, #26
	bpl	.L38
	lsls	r2, r3, #22
	bpl	.L38
	movw	r2, #597
	ldr	r1, .L103+24
	ldr	r0, .L103+48
	bl	printk
.L38:
	lsls	r3, r5, #25
	bpl	.L39
	ldr	r3, [r4]
	lsls	r0, r3, #22
	bpl	.L39
	mov	r2, #600
	ldr	r1, .L103+24
	ldr	r0, .L103+52
	bl	printk
.L39:
	lsls	r1, r5, #24
	bpl	.L40
	ldr	r3, [r4]
	lsls	r2, r3, #22
	bpl	.L40
	movw	r2, #603
	ldr	r1, .L103+24
	ldr	r0, .L103+56
	bl	printk
.L40:
	lsls	r3, r5, #23
	bpl	.L41
	ldr	r3, [r4]
	lsls	r0, r3, #22
	bpl	.L41
	movw	r2, #606
	ldr	r1, .L103+24
	ldr	r0, .L103+60
	bl	printk
.L41:
	lsls	r1, r5, #18
	bpl	.L42
	ldr	r3, [r4]
	lsls	r2, r3, #22
	bpl	.L101
	movw	r2, #609
	ldr	r1, .L103+24
	ldr	r0, .L103+64
	b	.L102
.L42:
	ldr	r3, [r7, #44]
	ldr	r2, [r8, #104]
	add	r3, r3, r2
	str	r3, [r7, #44]
	b	.L31
.L104:
	.align	2
.L103:
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
	ldr	r7, .L121
	mov	r4, r1
	ldr	r6, [r1, #16]
	ldr	r3, [r7]
	lsls	r2, r3, #31
	bpl	.L106
	mov	r3, #652
	ldr	r1, .L121+4
	mov	r2, r3
	ldr	r0, .L121+8
	bl	printk
.L106:
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
	beq	.L107
	ldr	r3, [r5, #308]
	ldr	r2, [r4, #4]
	cbnz	r3, .L108
	ldr	r3, [r6, #52]
	cbz	r3, .L111
	rsb	r3, r3, #100
.L109:
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
.L107:
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L110
	mov	r3, #672
	ldr	r1, .L121+4
	mov	r2, r3
	ldr	r0, .L121+12
	bl	printk
.L110:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L111:
	movs	r3, #80
	b	.L109
.L108:
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
	b	.L107
.L122:
	.align	2
.L121:
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
	ldr	r5, .L131
	mov	r4, r1
	ldr	r6, [r1, #4]
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L124
	mov	r3, #684
	ldr	r1, .L131+4
	mov	r2, r3
	ldr	r0, .L131+8
	bl	printk
.L124:
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
	ldr	r4, [r4, #96]
	cbz	r4, .L125
	movw	r1, #693
	ldr	r0, .L131+12
	bl	printk
	mov	r0, #-1
.L123:
	pop	{r3, r4, r5, r6, r7, pc}
.L125:
	ldr	r0, [r5]
	str	r4, [r7, #304]
	ands	r0, r0, #1
	beq	.L123
	movw	r3, #698
	ldr	r1, .L131+4
	mov	r2, r3
	ldr	r0, .L131+16
	bl	printk
	mov	r0, r4
	b	.L123
.L132:
	.align	2
.L131:
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
	ldr	r5, .L142
	ldr	r6, [r0, #128]
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L134
	mov	r3, r0
	movs	r2, #199
	ldr	r1, .L142+4
	ldr	r0, .L142+8
	bl	printk
.L134:
	ldr	r3, [r4, #108]
	mov	r0, r6
	ldr	r1, [r3, #36]
	bl	vepu541_set_fmt
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L135
	mov	r3, r4
	movs	r2, #201
	ldr	r1, .L142+4
	ldr	r0, .L142+12
	bl	printk
.L135:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L143:
	.align	2
.L142:
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
	beq	.L145
	ldr	r1, .L158
	rsb	r3, r3, #100
	str	r3, [r0, #308]
	ldr	r1, [r1]
	lsls	r1, r1, #27
	bpl	.L146
	ldr	r2, [r2, #172]
	ldr	r1, .L158+4
	ldr	r0, .L158+8
	str	r2, [sp]
	movs	r2, #90
	bl	printk
.L146:
	ldr	r1, [r4, #308]
	cmp	r1, #49
	bgt	.L147
	movw	r0, #5000
	bl	__aeabi_idiv
.L148:
	ldr	r5, .L158+12
	movs	r2, #0
	ldr	r1, .L158+16
	ldr	ip, .L158+20
.L151:
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
	ble	.L152
	cmp	lr, #255
	it	ge
	movge	lr, #255
	uxtb	lr, lr
.L149:
	ldr	r6, [r4, #312]
	cmp	r3, #0
	strb	lr, [r6, r2]
	ble	.L153
	cmp	r3, #255
	it	ge
	movge	r3, #255
	uxtb	r3, r3
.L150:
	ldr	r6, [r4, #316]
	strb	r3, [r6, r2]
	adds	r2, r2, #1
	cmp	r2, #64
	bne	.L151
.L145:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L147:
	lsls	r0, r1, #1
	rsb	r0, r0, #200
	b	.L148
.L152:
	mov	lr, #1
	b	.L149
.L153:
	movs	r3, #1
	b	.L150
.L159:
	.align	2
.L158:
	.word	hal_jpege_debug
	.word	.LANCHOR0+152
	.word	.LC21
	.word	jpege_luma_quantizer
	.word	jpege_chroma_quantizer
	.word	1374389535
	.fnend
	.size	hal_jpege_vepu_rc, .-hal_jpege_vepu_rc
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
	ldr	r0, .L167
	bl	mpp_osal_malloc
	movs	r1, #64
	str	r0, [r4, #4]
	ldr	r0, .L167
	bl	mpp_osal_malloc
	ldr	r3, [r4, #4]
	str	r0, [r4, #8]
	cbz	r3, .L161
	cbnz	r0, .L163
.L161:
	movs	r2, #118
	ldr	r1, .L167+4
	ldr	r0, .L167+8
	bl	printk
	mvn	r0, #3
.L160:
	pop	{r4, pc}
.L163:
	movs	r0, #0
	b	.L160
.L168:
	.align	2
.L167:
	.word	.LANCHOR0+170
	.word	.LANCHOR0+193
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
	ldr	r7, .L181
	mov	r4, r0
	ldr	r3, [r7]
	lsls	r1, r3, #31
	bpl	.L170
	movs	r3, #140
	ldr	r1, .L181+4
	mov	r2, r3
	ldr	r0, .L181+8
	bl	printk
.L170:
	mov	r1, #596
	ldr	r0, .L181+12
	bl	mpp_osal_calloc
	mov	r1, #5408
	str	r0, [r4, #72]
	ldr	r0, .L181+12
	bl	mpp_osal_calloc
	movs	r1, #28
	str	r0, [r4, #68]
	ldr	r0, .L181+12
	bl	mpp_osal_calloc
	movs	r1, #16
	str	r0, [r4, #128]
	add	r0, r5, #32
	ldr	r3, [r5, #4]
	str	r3, [r4, #108]
	movs	r3, #0
	str	r3, [r4, #88]
	movs	r3, #1
	str	r3, [r4, #112]
	ldr	r3, [r5, #8]
	str	r3, [r4, #300]
	str	r1, [r5, #28]
	bl	mpp_dev_init
	mov	r6, r0
	cbz	r0, .L171
	mov	r3, r0
	ldr	r1, .L181+4
	ldr	r0, .L181+16
	movs	r2, #153
.L180:
	bl	printk
.L169:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L171:
	ldr	r2, [r5, #32]
	ldr	r3, [r4, #68]
	str	r0, [r4, #100]
	add	r0, r4, #156
	add	r3, r3, #4224
	str	r2, [r4, #64]
	adds	r3, r3, #16
	strd	r3, r2, [r4, #92]
	bl	jpege_bits_init
	ldr	r3, [r4, #156]
	cbnz	r3, .L173
	movs	r1, #162
	ldr	r3, .L181+12
	str	r1, [sp]
	ldr	r2, .L181+20
	ldr	r0, .L181+24
	bl	printk
.L173:
	add	r0, r4, #308
	bl	hal_jpege_vepu_init_rc
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L169
	movs	r3, #164
	ldr	r1, .L181+4
	mov	r2, r3
	ldr	r0, .L181+28
	b	.L180
.L182:
	.align	2
.L181:
	.word	hal_jpege_debug
	.word	.LANCHOR0+216
	.word	.LC0
	.word	.LANCHOR0+237
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
	cbz	r1, .L184
	ldr	r0, .L192
	bl	mpp_osal_free
.L184:
	ldr	r1, [r4, #8]
	movs	r5, #0
	str	r5, [r4, #4]
	cbz	r1, .L185
	ldr	r0, .L192
	bl	mpp_osal_free
.L185:
	str	r5, [r4, #8]
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L193:
	.align	2
.L192:
	.word	.LANCHOR0+258
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
	ldr	r6, .L219
	ldr	r2, [r6]
	lsls	r2, r2, #31
	bpl	.L195
	movs	r3, #173
	ldr	r1, .L219+4
	mov	r2, r3
	ldr	r0, .L219+8
	bl	printk
.L195:
	ldr	r0, [r4, #64]
	cbz	r0, .L196
	bl	mpp_dev_deinit
	movs	r3, #0
	str	r3, [r4, #64]
.L196:
	ldr	r0, [r4, #156]
	bl	jpege_bits_deinit
	add	r0, r4, #308
	bl	hal_jpege_vepu_deinit_rc
	ldr	r1, [r4, #68]
	cbz	r1, .L197
	ldr	r0, .L219+12
	bl	mpp_osal_free
.L197:
	ldr	r1, [r4, #72]
	movs	r5, #0
	str	r5, [r4, #68]
	cbz	r1, .L198
	ldr	r0, .L219+12
	bl	mpp_osal_free
.L198:
	ldr	r1, [r4, #128]
	str	r5, [r4, #72]
	cbz	r1, .L199
	ldr	r0, .L219+12
	bl	mpp_osal_free
.L199:
	movs	r3, #0
	str	r3, [r4, #128]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L200
	movs	r3, #189
	ldr	r1, .L219+4
	mov	r2, r3
	ldr	r0, .L219+16
	bl	printk
.L200:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L220:
	.align	2
.L219:
	.word	hal_jpege_debug
	.word	.LANCHOR0+283
	.word	.LC0
	.word	.LANCHOR0+306
	.word	.LC5
	.fnend
	.size	hal_jpege_v540c_deinit, .-hal_jpege_v540c_deinit
	.global	__aeabi_uidiv
	.align	1
	.global	hal_jpege_vepu540c_set_osd
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_vepu540c_set_osd, %function
hal_jpege_vepu540c_set_osd:
	.fnstart
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r6, r0
	ldr	r3, [r0, #336]
	.pad #68
	sub	sp, sp, #68
	ldr	r5, [r0, #92]
	ldr	r4, [r0, #100]
	ldr	r8, [r3, #16]
	ldr	r0, [r3, #52]
	ldrd	r7, r9, [r8]
	bl	mpp_frame_get_height
	cmp	r0, r9
	bcs	.L233
	ldr	r3, [r8, #136]
	adds	r3, r3, #0
	it	ne
	movne	r3, #1
.L250:
	str	r3, [sp, #12]
	cmp	r4, #0
	beq	.L251
	ldr	fp, [r4, #4]
	cmp	fp, #0
	beq	.L251
	cmp	fp, #8
	bls	.L224
	mov	r3, fp
	movw	r2, #291
	ldr	r1, .L252
	ldr	r0, .L252+4
	bl	printk
	ldr	r3, [r4, #4]
	cmp	r3, #8
	bls	.L236
	mov	r1, #292
	ldr	r3, .L252+8
	str	r1, [sp]
	ldr	r2, .L252+12
	ldr	r0, .L252+16
	bl	printk
.L236:
	mov	r0, #-1
	b	.L221
.L233:
	movs	r3, #0
	b	.L250
.L224:
	ldr	r3, [sp, #12]
	cbz	r3, .L225
	adds	r7, r7, #15
	ldr	r0, [r8, #136]
	lsrs	r1, r7, #4
	bl	__aeabi_uidiv
	ldr	r3, [r6, #348]
	lsls	r0, r0, #4
	muls	r3, r0, r3
	str	r3, [sp, #8]
	bic	r3, r7, #15
	str	r3, [sp, #20]
	ldr	r3, [sp, #8]
	adds	r3, r0, r3
	str	r3, [sp, #24]
.L225:
	adds	r4, r4, #148
	add	r10, r5, #24
	mov	r9, #0
.L232:
	ldrd	r8, r3, [r4, #-116]
	ldr	r7, [r4, #-108]
	str	r3, [sp, #16]
	ldr	r3, [sp, #12]
	cbz	r3, .L226
	ldr	r3, [sp, #8]
	cmp	r3, r8
	bhi	.L229
	ldrd	r2, r3, [sp, #16]
	cmp	r3, r2
	bcc	.L229
	ldr	r3, [sp, #24]
	cmp	r3, r7
	bcc	.L229
	ldr	r3, [sp, #8]
	sub	r8, r8, r3
	subs	r7, r7, r3
.L226:
	ldr	r3, [r4, #-120]
	add	r0, sp, #36
	ldr	r1, [r4, #-128]
	str	r3, [sp, #28]
	bl	vepu541_set_fmt
	movs	r3, #36
	ldr	r2, [r4, #-140]
	mul	r1, r3, r9
	adds	r3, r5, r1
	add	lr, r3, #8
	ldrb	r0, [r5, r1]	@ zero_extendqisi2
	bfi	r0, r2, #0, #1
	strb	r0, [r5, r1]
	ldr	r2, [r4, #-136]
	bfi	r0, r2, #6, #1
	strb	r0, [r5, r1]
	ldr	r2, [r4, #-132]
	bfi	r0, r2, #7, #1
	strb	r0, [r5, r1]
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	ldrh	ip, [r3, #8]
	ldr	r2, [sp, #36]
	bfi	r0, r2, #0, #4
	strb	r0, [r3, #1]
	ldr	r2, [r4, #-124]
	bfi	r0, r2, #5, #1
	ldrh	r2, [r3, #4]
	strb	r0, [r3, #1]
	ldr	r0, [sp, #28]
	bfi	r2, r0, #0, #14
	strh	r2, [r3, #4]	@ movhi
	ldr	r2, [r3, #4]
	bfi	r2, r8, #14, #14
	str	r2, [r3, #4]
	ldr	r0, [sp, #16]
	bfi	ip, r0, #0, #14
	strh	ip, [r3, #8]	@ movhi
	ldr	ip, [r3, #8]
	bfi	ip, r7, #14, #14
	ubfx	r7, r2, #24, #8
	str	ip, [r3, #8]
	ldr	r2, [r4, #-96]
	bfi	r7, r2, #4, #4
	strb	r7, [r3, #7]
	ldr	r7, [r3, #20]
	ldr	r0, [r4, #-104]
	bfi	r7, r0, #0, #17
	str	r7, [r3, #20]
	ubfx	ip, r7, #24, #8
	ldr	r7, [r4, #-100]
	bfi	ip, r7, #1, #1
	strb	ip, [r3, #23]
	ldrb	ip, [r5, r1]	@ zero_extendqisi2
	ldr	r7, [r4, #-92]
	bfi	ip, r7, #1, #1
	strb	ip, [r5, r1]
	ldr	r7, [r4, #-88]
	bfi	ip, r7, #2, #1
	strb	ip, [r5, r1]
	ldr	r7, [r4, #-84]
	bfi	ip, r7, #3, #1
	strb	ip, [r5, r1]
	ldr	r7, [r4, #-80]
	bfi	ip, r7, #4, #1
	strb	ip, [r5, r1]
	ldr	r7, [r4, #-76]
	ldrb	ip, [r3, #11]	@ zero_extendqisi2
	bfi	ip, r7, #4, #1
	strb	ip, [r3, #11]
	ldr	r7, [r4, #-72]
	bfi	ip, r7, #5, #2
	ldrh	r7, [r3, #22]
	strb	ip, [r3, #11]
	ldr	r0, [r4, #-68]
	bfi	r7, r0, #1, #8
	ldrb	r0, [r3, #1]	@ zero_extendqisi2
	strh	r7, [r3, #22]	@ movhi
	ldr	r2, [r4, #-56]
	bfi	r0, r2, #4, #1
	strb	r0, [r3, #1]
	ldr	r2, [r4, #-52]
	ldr	r0, [r5, r1]
	bfi	r0, r2, #14, #8
	str	r0, [r5, r1]
	ldr	r2, [r4, #-48]
	ldrh	r0, [r3, #2]
	bfi	r0, r2, #6, #8
	strh	r0, [r3, #2]	@ movhi
	ldr	r2, [r4, #-44]
	ubfx	r0, r0, #8, #8
	bfi	r0, r2, #6, #2
	strb	r0, [r3, #3]
	ldrb	r0, [r5, r1]	@ zero_extendqisi2
	ldr	r2, [r4, #-40]
	bfi	r0, r2, #5, #1
	strb	r0, [r5, r1]
	ldrb	r1, [r3, #32]	@ zero_extendqisi2
	add	r0, r3, #32
	ldr	r2, [r4, #-36]
	bfi	r1, r2, #0, #1
	strb	r1, [r3, #32]
	ldr	r2, [r4, #-32]
	bfi	r1, r2, #1, #7
	strb	r1, [r3, #32]
	ldrb	r1, [r3, #33]	@ zero_extendqisi2
	ldr	r2, [r4, #-28]
	bfi	r1, r2, #0, #6
	strb	r1, [r3, #33]
	ldr	r2, [r3, #32]
	ldr	r1, [r4, #-24]
	bfi	r2, r1, #14, #6
	str	r2, [r3, #32]
	ldr	r3, [r4, #-20]
	ubfx	r2, r2, #16, #16
	bfi	r2, r3, #4, #5
	strh	r2, [r0, #2]	@ movhi
	ldr	r1, [r4, #-60]
	cbz	r1, .L230
	movs	r2, #0
	ldr	r0, [r6, #324]
	bl	mpp_dev_get_mpi_ioaddress
	str	r0, [r10, #-8]
.L230:
	ldr	r1, [r4, #-12]
	cbz	r1, .L231
	movs	r2, #0
	ldr	r0, [r6, #324]
	bl	mpp_dev_get_mpi_ioaddress
	str	r0, [r10, #-12]
.L231:
	mov	r2, r4
	ldr	r3, [r2, #-8]!	@ unaligned
	str	r3, [r10]	@ unaligned
	ldr	r3, [r2, #4]	@ unaligned
	str	r3, [r10, #4]	@ unaligned
.L229:
	add	r9, r9, #1
	adds	r4, r4, #140
	cmp	fp, r9
	add	r10, r10, #36
	bne	.L232
	ldr	r3, [r5, #288]
	and	r2, r3, #-134217728
	ldr	r3, .L252+20
	orrs	r3, r3, r2
	str	r3, [r5, #288]
	ldr	r3, [r5, #292]
	and	r2, r3, #-134217728
	ldr	r3, .L252+24
	orrs	r3, r3, r2
	str	r3, [r5, #292]
	ldr	r3, [r5, #296]
	and	r2, r3, #-134217728
	ldr	r3, .L252+28
	orrs	r3, r3, r2
	str	r3, [r5, #296]
	ldr	r3, [r5, #300]
	and	r3, r3, #-16777216
	orr	r3, r3, #8388608
	orr	r3, r3, #32768
	str	r3, [r5, #300]
	mov	r3, #-16711936
	str	r3, [r5, #304]
	mov	r3, #65280
	strh	r3, [r5, #308]	@ movhi
.L251:
	movs	r0, #0
.L221:
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L253:
	.align	2
.L252:
	.word	.LANCHOR0+329
	.word	.LC26
	.word	.LANCHOR0+356
	.word	.LC27
	.word	.LC25
	.word	7679053
	.word	33773525
	.word	128920192
	.fnend
	.size	hal_jpege_vepu540c_set_osd, .-hal_jpege_vepu540c_set_osd
	.align	1
	.global	hal_jpege_v540c_gen_regs
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	hal_jpege_v540c_gen_regs, %function
hal_jpege_v540c_gen_regs:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	mov	r5, r0
	ldr	r4, [r0, #68]
	ldr	r7, [r0, #156]
	mov	r6, r1
	ldr	r0, [r1, #28]
	movs	r3, #0
	strd	r3, r3, [sp, #8]
	bl	mpp_packet_get_length
	ldr	r3, [r6, #32]
	add	r9, r5, #160
	ldr	r1, .L284
	add	fp, r5, #324
	mov	r10, r0
	ldr	r0, [r3]
	bl	mpp_buffer_get_ptr_with_caller
	ldr	r3, [r6, #32]
	ldr	r2, [r3, #12]
	ldr	r3, [r3, #24]
	str	r3, [sp]
	add	r8, r0, r2
	ldr	r3, .L284+4
	ldr	r2, [r3]
	str	r3, [sp, #4]
	tst	r2, #1
	beq	.L255
	movw	r3, #407
	ldr	r1, .L284+8
	mov	r2, r3
	ldr	r0, .L284+12
	bl	printk
.L255:
	add	r3, r4, #692
	str	r3, [r5, #328]
	ldr	r3, [r5, #64]
	mov	r2, #5408
	movs	r1, #0
	str	r6, [r5, #336]
	mov	r0, r4
	str	r3, [r5, #324]
	ldr	r3, [r5, #128]
	str	r3, [r5, #340]
	ldr	r3, [r5, #300]
	str	r3, [r5, #344]
	bl	memset
	mov	r1, r8
	mov	r0, r7
	ldr	r2, [sp]
	bl	jpege_bits_setup
	lsl	r1, r10, #3
	mov	r0, r7
	bl	jpege_seek_bits
	ldr	r3, [r5, #108]
	ldr	r3, [r3, #108]
	cmp	r3, #2
	beq	.L256
	mov	r1, r6
	mov	r0, r5
	bl	hal_jpege_vepu_rc
	ldr	r3, [r5, #312]
	str	r3, [sp, #8]
	ldr	r3, [r5, #316]
.L283:
	add	r2, sp, #8
	mov	r1, r9
	mov	r0, r7
	str	r3, [sp, #12]
	bl	write_jpeg_header
	mov	r0, r7
	bl	jpege_bits_get_bitpos
	adds	r1, r0, #7
	ldr	r0, [r6, #28]
	asrs	r1, r1, #3
	str	r1, [r6, #48]
	bl	mpp_packet_set_length
	ldr	r3, [r6, #32]
	ldr	r2, [r3]
	cmp	r2, #0
	beq	.L258
	ldr	r2, [r6, #48]
	ldr	r1, .L284
	str	r2, [r3, #20]
	ldr	r0, [r6, #32]
	bl	mpp_buffer_flush_for_device_with_caller
.L259:
	movs	r2, #0
	ldrb	r3, [r4, #17]	@ zero_extendqisi2
	strb	r2, [r4, #16]
	ldr	r1, [r5, #112]
	bfi	r3, r1, #0, #3
	strb	r3, [r4, #17]
	ldrb	r3, [r4, #20]	@ zero_extendqisi2
	movs	r1, #2
	bic	r3, r3, #3
	strb	r3, [r4, #20]
	ldrh	r3, [r4, #32]
	and	r3, r3, #56320
	orr	r3, r3, #8640
	orr	r3, r3, #63
	strh	r3, [r4, #32]	@ movhi
	ldrb	r3, [r4, #436]	@ zero_extendqisi2
	bfi	r3, r1, #0, #2
	strb	r3, [r4, #436]
	movs	r3, #7
	str	r3, [r4, #48]
	ldrh	r3, [r4, #54]
	bfi	r3, r2, #0, #10
	strh	r3, [r4, #54]	@ movhi
	mvn	r3, #14680064
	str	r3, [r4, #56]
	ldr	r3, [r5, #300]
	cbz	r3, .L260
	ldrb	r3, [r4, #96]	@ zero_extendqisi2
	and	r3, r3, #8
	orn	r3, r3, #124
	strb	r3, [r4, #96]
	ldrb	r3, [r4, #27]	@ zero_extendqisi2
	bic	r3, r3, #7
	orr	r3, r3, #7
	strb	r3, [r4, #27]
	ldr	r3, [r4, #444]
	bic	r3, r3, #458752
	bic	r3, r3, #255
	orr	r3, r3, #327680
	str	r3, [r4, #444]
.L260:
	mov	r0, r5
	addw	r10, r4, #3542
	bl	hal_jpege_vepu540c_set_osd
	mov	r0, fp
	bl	vepu540c_set_jpeg_reg
	movs	r7, #0
	addw	r3, r4, #3544
	str	r3, [sp]
.L261:
	add	r8, r7, #64
	mov	fp, r10
	mov	r9, r7
.L262:
	ldr	r2, [sp, #8]
	mov	r0, #32768
	ldrb	r1, [r2, r9]	@ zero_extendqisi2
	add	r9, r9, #8
	bl	__aeabi_idiv
	cmp	r8, r9
	strh	r0, [fp, #2]!	@ movhi
	bne	.L262
	adds	r7, r7, #1
	add	r10, r10, #16
	cmp	r7, #8
	bne	.L261
	addw	r10, r4, #3670
	movs	r7, #0
.L264:
	add	r8, r7, #64
	mov	fp, r10
	mov	r9, r7
.L265:
	ldr	r2, [sp, #12]
	mov	r0, #32768
	ldrb	r1, [r2, r9]	@ zero_extendqisi2
	add	r9, r9, #8
	bl	__aeabi_idiv
	cmp	r8, r9
	strh	r0, [fp, #2]!	@ movhi
	bne	.L265
	adds	r7, r7, #1
	add	r10, r10, #16
	cmp	r7, #8
	bne	.L264
	addw	r10, r4, #3798
	movs	r7, #0
.L267:
	add	r8, r7, #64
	mov	fp, r10
	mov	r9, r7
.L268:
	ldr	r2, [sp, #12]
	mov	r0, #32768
	ldrb	r1, [r2, r9]	@ zero_extendqisi2
	add	r9, r9, #8
	bl	__aeabi_idiv
	cmp	r8, r9
	strh	r0, [fp, #2]!	@ movhi
	bne	.L268
	adds	r7, r7, #1
	add	r10, r10, #16
	cmp	r7, #8
	bne	.L267
	ldr	r3, [sp]
	add	r4, r4, #4224
	adds	r4, r4, #16
	str	r4, [r6, #88]
	str	r3, [r6, #92]
	ldr	r3, [r5, #144]
	adds	r3, r3, #1
	str	r3, [r5, #144]
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L270
	movw	r3, #481
	ldr	r1, .L284+8
	mov	r2, r3
	ldr	r0, .L284+16
	bl	printk
.L270:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L256:
	movs	r3, #0
	str	r3, [sp, #8]
	b	.L283
.L258:
	ldr	r0, [r5, #64]
	bl	mpp_get_dev
	ldr	r1, [r6, #32]
	movs	r3, #1
	ldr	r2, [r6, #48]
	ldr	r1, [r1, #4]
	bl	dma_sync_single_for_device
	b	.L259
.L285:
	.align	2
.L284:
	.word	.LANCHOR0+383
	.word	hal_jpege_debug
	.word	.LANCHOR0+408
	.word	.LC0
	.word	.LC5
	.fnend
	.size	hal_jpege_v540c_gen_regs, .-hal_jpege_v540c_gen_regs
	.global	hal_jpege_vepu540c
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.36591, %object
	.size	__func__.36591, 22
__func__.36591:
	.ascii	"hal_jpege_v540c_start\000"
	.type	__func__.36607, %object
	.size	__func__.36607, 21
__func__.36607:
	.ascii	"hal_jpege_v540c_wait\000"
	.type	__func__.36598, %object
	.size	__func__.36598, 32
__func__.36598:
	.ascii	"hal_jpege_vepu540c_status_check\000"
	.type	__func__.36614, %object
	.size	__func__.36614, 25
__func__.36614:
	.ascii	"hal_jpege_v540c_get_task\000"
	.type	__func__.36621, %object
	.size	__func__.36621, 25
__func__.36621:
	.ascii	"hal_jpege_v540c_ret_task\000"
	.type	__func__.36494, %object
	.size	__func__.36494, 27
__func__.36494:
	.ascii	"hal_jpege_vepu540c_prepare\000"
	.type	__func__.36457, %object
	.size	__func__.36457, 18
__func__.36457:
	.ascii	"hal_jpege_vepu_rc\000"
	.type	__FUNCTION__.36466, %object
	.size	__FUNCTION__.36466, 23
__FUNCTION__.36466:
	.ascii	"hal_jpege_vepu_init_rc\000"
	.type	__func__.36467, %object
	.size	__func__.36467, 23
__func__.36467:
	.ascii	"hal_jpege_vepu_init_rc\000"
	.type	__func__.36479, %object
	.size	__func__.36479, 21
__func__.36479:
	.ascii	"hal_jpege_v540c_init\000"
	.type	__FUNCTION__.36480, %object
	.size	__FUNCTION__.36480, 21
__FUNCTION__.36480:
	.ascii	"hal_jpege_v540c_init\000"
	.type	__FUNCTION__.36471, %object
	.size	__FUNCTION__.36471, 25
__FUNCTION__.36471:
	.ascii	"hal_jpege_vepu_deinit_rc\000"
	.type	__func__.36487, %object
	.size	__func__.36487, 23
__func__.36487:
	.ascii	"hal_jpege_v540c_deinit\000"
	.type	__FUNCTION__.36488, %object
	.size	__FUNCTION__.36488, 23
__FUNCTION__.36488:
	.ascii	"hal_jpege_v540c_deinit\000"
	.type	__func__.36544, %object
	.size	__func__.36544, 27
__func__.36544:
	.ascii	"hal_jpege_vepu540c_set_osd\000"
	.type	__FUNCTION__.36545, %object
	.size	__FUNCTION__.36545, 27
__FUNCTION__.36545:
	.ascii	"hal_jpege_vepu540c_set_osd\000"
	.type	__FUNCTION__.36574, %object
	.size	__FUNCTION__.36574, 25
__FUNCTION__.36574:
	.ascii	"hal_jpege_v540c_gen_regs\000"
	.type	__func__.36579, %object
	.size	__func__.36579, 25
__func__.36579:
	.ascii	"hal_jpege_v540c_gen_regs\000"
	.space	3
	.type	hal_jpege_vepu540c, %object
	.size	hal_jpege_vepu540c, 64
hal_jpege_vepu540c:
	.word	.LC28
	.word	8
	.word	352
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
	.ascii	"\0016hal_jpege_v540c:%s:%d: RKV_ENC_INT_BIT_STREAM_"
	.ascii	"OVERFLOW\000"
.LC13:
	.ascii	"\0016hal_jpege_v540c:%s:%d: RKV_ENC_INT_BUS_WRITE_F"
	.ascii	"ULL\000"
.LC14:
	.ascii	"\0016hal_jpege_v540c:%s:%d: RKV_ENC_INT_BUS_WRITE_E"
	.ascii	"RROR\000"
.LC15:
	.ascii	"\0016hal_jpege_v540c:%s:%d: RKV_ENC_INT_BUS_READ_ER"
	.ascii	"ROR\000"
.LC16:
	.ascii	"\0016hal_jpege_v540c:%s:%d: RKV_ENC_INT_TIMEOUT_ERR"
	.ascii	"OR\000"
.LC17:
	.ascii	"\0016hal_jpege_v540c:%s:%d: JPEG BIT_STREAM_OVERFLO"
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
	.ascii	"\0013hal_jpege_v540c:%s:%d: do NOT support more tha"
	.ascii	"n 8 regions invalid num %d\012\000"
.LC27:
	.ascii	"osd->num_region <= 8\000"
.LC28:
	.ascii	"hal_jpege_v540c\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
