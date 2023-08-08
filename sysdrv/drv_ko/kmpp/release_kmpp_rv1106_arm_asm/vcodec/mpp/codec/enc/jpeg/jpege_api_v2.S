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
	.file	"jpege_api_v2.c"
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
	.type	jpege_proc_show, %function
jpege_proc_show:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #24
	sub	sp, sp, #24
	ldr	r4, [r1]
	mov	r5, r0
	ldr	r1, .L2
	mov	r6, r2
	bl	seq_puts
	ldr	r3, .L2+4
	mov	r0, r5
	ldr	r2, .L2+8
	ldr	r1, .L2+12
	str	r3, [sp, #16]
	ldr	r3, .L2+16
	str	r3, [sp, #12]
	ldr	r3, .L2+20
	str	r3, [sp, #8]
	ldr	r3, .L2+24
	str	r3, [sp, #4]
	ldr	r3, .L2+28
	str	r3, [sp]
	ldr	r3, .L2+32
	bl	seq_printf
	ldr	r3, [r4, #588]
	mov	r2, r6
	ldr	r1, .L2+36
	mov	r0, r5
	str	r3, [sp, #16]
	ldr	r3, [r4, #584]
	str	r3, [sp, #12]
	ldr	r3, [r4, #580]
	str	r3, [sp, #8]
	ldr	r3, [r4, #576]
	str	r3, [sp, #4]
	ldr	r3, [r4, #16]
	str	r3, [sp]
	ldr	r3, [r4, #12]
	bl	seq_printf
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L3:
	.align	2
.L2:
	.word	.LC0
	.word	.LC4
	.word	.LC2
	.word	.LC3
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC1
	.word	.LC9
	.fnend
	.size	jpege_proc_show, .-jpege_proc_show
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_proc_prep_cfg, %function
jpege_proc_prep_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 96
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, [r1]
	.pad #112
	sub	sp, sp, #112
	mov	r6, r1
	cbnz	r5, .L5
	movs	r1, #110
	ldr	r3, .L33
	str	r1, [sp]
	ldr	r2, .L33+4
	ldr	r0, .L33+8
	bl	printk
.L32:
	movs	r0, #0
	b	.L4
.L12:
	cmp	r1, r2
	blt	.L16
	cmp	r0, r3
	bge	.L14
.L16:
	strd	r1, r0, [sp]
	movs	r1, #160
	b	.L31
.L14:
	ldr	r4, .L33+12
	ldr	r5, [r4]
	ands	r5, r5, #64
	beq	.L32
	strd	r1, r0, [sp, #4]
	str	r3, [sp]
	mov	r3, r2
	ldr	r1, .L33+16
	movs	r2, #171
	ldr	r0, .L33+20
	bl	printk
	b	.L32
.L5:
	mov	r1, r0
	movs	r2, #96
	add	r0, sp, #16
	bl	memcpy
	lsls	r3, r5, #29
	bpl	.L19
	ldr	r3, [r6, #28]
	str	r3, [r4, #28]
.L19:
	lsls	r1, r5, #27
	itt	mi
	ldrmi	r3, [r6, #48]
	strmi	r3, [r4, #48]
	lsls	r2, r5, #26
	itt	mi
	ldrmi	r3, [r6, #52]
	strmi	r3, [r4, #52]
	tst	r5, #17
	ldr	r5, [r4, #48]
	beq	.L9
	bic	r3, r5, #2
	cmp	r3, #1
	itete	eq
	ldreq	r3, [r6, #8]
	ldrne	r3, [r6, #4]
	streq	r3, [r4, #4]
	strne	r3, [r4, #4]
	ite	eq
	ldreq	r3, [r6, #4]
	ldrne	r3, [r6, #8]
	str	r3, [r4, #8]
	ldr	r3, [r6, #12]
	str	r3, [r4, #12]
	ldr	r3, [r6, #16]
	str	r3, [r4, #16]
.L9:
	bic	r5, r5, #2
	movs	r3, #0
	cmp	r5, #1
	str	r3, [r4]
	ldr	r2, [r4, #4]
	ldrd	r3, r1, [r4, #8]
	ldr	r0, [r4, #16]
	bne	.L12
	cmp	r3, r1
	bgt	.L13
	cmp	r0, r2
	bge	.L14
.L13:
	strd	r1, r0, [sp]
	movs	r1, #154
.L31:
	ldr	r0, .L33+24
	bl	printk
	movs	r2, #166
	ldr	r1, .L33+16
	ldr	r0, .L33+28
	bl	printk
	movs	r2, #96
	mov	r0, r4
	add	r1, sp, #16
	bl	memcpy
	mvn	r0, #5
.L4:
	add	sp, sp, #112
	@ sp needed
	pop	{r4, r5, r6, pc}
.L34:
	.align	2
.L33:
	.word	.LANCHOR0
	.word	.LC10
	.word	.LC11
	.word	.LANCHOR1
	.word	.LANCHOR0+20
	.word	.LC14
	.word	.LC12
	.word	.LC13
	.fnend
	.size	jpege_proc_prep_cfg, .-jpege_proc_prep_cfg
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_add_Prefix, %function
jpege_add_Prefix:
	.fnstart
	@ args = 4, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #44
	sub	sp, sp, #44
	mov	r9, r1
	mov	r4, r3
	mov	r7, r0
	ldr	r5, [sp, #72]
	bl	mpp_packet_get_pos
	mov	r6, r0
	mov	r0, r7
	bl	mpp_packet_get_length
	mov	r8, r0
	mov	r0, r7
	bl	mpp_packet_get_size
	add	r1, r6, r8
	sub	r2, r0, r8
	add	r0, sp, #4
	bl	mpp_writer_init
	cmp	r5, #8
	ble	.L36
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #255
	bne	.L36
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cmp	r3, #225
	bne	.L37
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #69
	bne	.L36
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #120
	bne	.L36
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	cmp	r3, #105
	bne	.L36
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cmp	r3, #102
	bne	.L36
	ldr	r3, .L52
	ldr	r3, [r3]
	lsls	r2, r3, #31
	bpl	.L38
	ldr	r1, .L52+4
	mov	r3, r5
	ldr	r0, .L52+8
	mov	r2, #516
.L51:
	bl	printk
.L38:
	movs	r6, #0
.L39:
	cmp	r6, r5
	blt	.L40
	add	r0, sp, #4
	bl	mpp_writer_bytes
	str	r0, [r9]
	add	r1, r0, r8
	mov	r0, r7
	bl	mpp_packet_set_length
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L37:
	cmp	r3, #226
	bne	.L36
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	cmp	r3, #77
	bne	.L36
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cmp	r3, #80
	bne	.L36
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	cmp	r3, #70
	bne	.L36
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	cbnz	r3, .L36
	ldr	r3, .L52
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L38
	mov	r3, r5
	movw	r2, #521
	ldr	r1, .L52+4
	ldr	r0, .L52+12
	b	.L51
.L36:
	movs	r2, #16
	movw	r1, #65511
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #16
	adds	r1, r5, #2
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	b	.L38
.L40:
	ldrb	r1, [r4, r6]	@ zero_extendqisi2
	movs	r2, #8
	add	r0, sp, #4
	adds	r6, r6, #1
	bl	mpp_writer_put_raw_bits
	b	.L39
.L53:
	.align	2
.L52:
	.word	.LANCHOR1
	.word	.LANCHOR0+40
	.word	.LC15
	.word	.LC16
	.fnend
	.size	jpege_add_Prefix, .-jpege_add_Prefix
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_start, %function
jpege_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #44
	sub	sp, sp, #44
	ldr	r5, [r1, #28]
	mov	r4, r1
	ldrd	r8, r7, [r0, #112]
	ldr	r6, [r0, #120]
	mov	r0, r5
	bl	mpp_packet_get_pos
	mov	r9, r0
	mov	r0, r5
	bl	mpp_packet_get_size
	mov	r1, r9
	mov	r2, r0
	add	r0, sp, #4
	bl	mpp_writer_init
	movs	r2, #16
	movw	r1, #65496
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #16
	movw	r1, #65504
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #16
	mov	r1, r2
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #16
	movw	r1, #19014
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #16
	movw	r1, #18758
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #16
	mov	r1, #258
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	cbz	r7, .L55
	cbz	r6, .L55
	mov	r1, r8
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #16
	mov	r1, r7
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #16
	mov	r1, r6
.L63:
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	add	r0, sp, #4
	bl	mpp_writer_bytes
	mov	r6, r0
	mov	r1, r0
	mov	r0, r5
	bl	mpp_packet_set_length
	ldr	r0, [r4, #48]
	add	r0, r0, r6
	str	r0, [r4, #48]
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L55:
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	movs	r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #16
	movs	r1, #1
	b	.L63
	.fnend
	.size	jpege_start, .-jpege_start
	.global	__aeabi_uidiv
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_proc_hal, %function
jpege_proc_hal:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r8, r0
	ldr	r6, .L86
	mov	r4, r0
	mov	r7, r1
	ldr	r9, [r1, #52]
	ldr	r5, [r8], #4
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L65
	mov	r3, r0
	movw	r2, #441
	ldr	r1, .L86+4
	ldr	r0, .L86+8
	bl	printk
.L65:
	ldr	r3, [r5, #12]
	mov	r0, r9
	str	r3, [r4, #4]
	ldr	r3, [r5, #16]
	str	r3, [r4, #8]
	ldr	r3, [r5, #20]
	str	r3, [r4, #12]
	ldr	r3, [r5, #24]
	str	r3, [r4, #16]
	ldr	r3, [r5, #12]
	adds	r3, r3, #15
	asrs	r3, r3, #4
	str	r3, [r4, #20]
	ldr	r3, [r5, #16]
	adds	r3, r3, #15
	asrs	r3, r3, #4
	str	r3, [r4, #24]
	ldr	r3, [r5, #36]
	str	r3, [r4, #28]
	ldr	r3, [r5, #40]
	str	r3, [r4, #32]
	ldr	r3, [r5, #56]
	str	r3, [r4, #36]
	ldr	r3, [r5, #60]
	str	r3, [r4, #40]
	bl	mpp_frame_get_offset_x
	str	r0, [r4, #44]
	mov	r0, r9
	bl	mpp_frame_get_offset_y
	str	r0, [r4, #48]
	ldr	r3, [r5, #576]
	str	r3, [r4, #52]
	ldr	r3, [r5, #580]
	str	r3, [r4, #56]
	ldr	r3, [r5, #588]
	str	r3, [r4, #60]
	ldr	r3, [r5, #584]
	str	r3, [r4, #64]
	ldr	r3, [r5, #592]
	str	r3, [r4, #68]
	ldr	r3, [r5, #596]
	str	r3, [r4, #72]
	movs	r3, #0
	strd	r3, r3, [r4, #136]
	str	r3, [r4, #132]
	ldr	r2, [r5, #3404]
	cbz	r2, .L66
	ldr	r3, [r5, #16]
	cmp	r2, #2
	add	r3, r3, #15
	asr	r9, r3, #4
	bne	.L67
	ldr	r1, [r5, #12]
	ldr	r3, [r5, #3408]
	adds	r1, r1, #15
	asrs	r1, r1, #4
	mul	r2, r1, r9
	cbz	r3, .L68
	cmp	r3, r2
	bhi	.L68
	subs	r3, r3, #1
	adds	r0, r3, r1
	ite	cs
	movcs	r10, #1
	movcc	r10, #0
	bl	__aeabi_uidiv
	cmp	r9, r0
	bls	.L66
	cmp	r10, #0
	bne	.L66
	ldr	r3, [r4, #20]
	str	r0, [r4, #136]
	muls	r0, r3, r0
	str	r0, [r4, #140]
	ldr	r3, [r5, #4]
	adds	r3, r3, #0
	it	ne
	movne	r3, #1
	str	r3, [r4, #132]
	b	.L66
.L68:
	str	r2, [sp]
	mov	r2, #480
	ldr	r1, .L86+4
	ldr	r0, .L86+12
	bl	printk
.L66:
	movs	r3, #1
	str	r3, [r7]
	strd	r3, r8, [r7, #12]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L72
	mov	r3, r4
	mov	r2, #496
	ldr	r1, .L86+4
	ldr	r0, .L86+16
	bl	printk
.L72:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L67:
	movw	r2, #483
	ldr	r1, .L86+4
	ldr	r0, .L86+20
	bl	printk
	b	.L66
.L87:
	.align	2
.L86:
	.word	.LANCHOR1
	.word	.LANCHOR0+57
	.word	.LC17
	.word	.LC18
	.word	.LC20
	.word	.LC19
	.fnend
	.size	jpege_proc_hal, .-jpege_proc_hal
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_deinit_v2, %function
jpege_deinit_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r0
	ldr	r7, .L109
	ldr	r4, [r0]
	ldr	r2, [r7]
	lsls	r2, r2, #31
	bpl	.L89
	mov	r3, r0
	movs	r2, #95
	ldr	r1, .L109+4
	ldr	r0, .L109+8
	bl	printk
.L89:
	ldr	r1, [r4, #592]
	cbz	r1, .L90
	ldr	r0, .L109+12
	bl	mpp_osal_free
.L90:
	ldr	r1, [r4, #596]
	movs	r5, #0
	str	r5, [r4, #592]
	cbz	r1, .L91
	ldr	r0, .L109+12
	bl	mpp_osal_free
.L91:
	ldr	r1, [r4, #600]
	str	r5, [r4, #596]
	cbz	r1, .L92
	ldr	r0, .L109+12
	bl	mpp_osal_free
.L92:
	movs	r3, #0
	str	r3, [r4, #600]
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L93
	mov	r3, r6
	movs	r2, #101
	ldr	r1, .L109+4
	ldr	r0, .L109+16
	bl	printk
.L93:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L110:
	.align	2
.L109:
	.word	.LANCHOR1
	.word	.LANCHOR0+72
	.word	.LC17
	.word	.LANCHOR0+88
	.word	.LC20
	.fnend
	.size	jpege_deinit_v2, .-jpege_deinit_v2
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_init_v2, %function
jpege_init_v2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r6, .L120
	mov	r5, r1
	ldr	r2, [r6]
	lsls	r2, r2, #31
	bpl	.L112
	mov	r3, r0
	movs	r2, #63
	ldr	r1, .L120+4
	ldr	r0, .L120+8
	bl	printk
.L112:
	ldr	r3, [r5, #8]
	movs	r1, #1
	movs	r2, #0
	movs	r0, #30
	str	r3, [r4]
	movs	r3, #8
	str	r1, [r5, #16]
	str	r3, [r5]
	ldr	r3, [r4]
	strd	r2, r0, [r3, #128]
	strd	r1, r2, [r3, #136]
	str	r2, [r3, #108]
	movs	r2, #10
	strd	r0, r1, [r3, #144]
	str	r2, [r3, #576]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L113
	mov	r3, r4
	movs	r2, #86
	ldr	r1, .L120+4
	ldr	r0, .L120+12
	bl	printk
.L113:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L121:
	.align	2
.L120:
	.word	.LANCHOR1
	.word	.LANCHOR0+104
	.word	.LC17
	.word	.LC20
	.fnend
	.size	jpege_init_v2, .-jpege_init_v2
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_copy_from_user.constprop.1, %function
_copy_from_user.constprop.1:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r3, sp
	bic	r3, r3, #8160
	mov	r5, r0
	bic	r3, r3, #31
	ldr	r3, [r3, #8]
	.syntax unified
@ 157 "./include/linux/uaccess.h" 1
	.syntax unified
adds r2, r1, #64; sbcscc r2, r2, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L125
	movs	r2, #64
	bl	arm_copy_from_user
	mov	r4, r0
	cbz	r0, .L122
.L123:
	rsb	r0, r4, #64
	mov	r2, r4
	movs	r1, #0
	add	r0, r0, r5
	bl	memset
.L122:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L125:
	movs	r4, #64
	b	.L123
	.fnend
	.size	_copy_from_user.constprop.1, .-_copy_from_user.constprop.1
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_proc_jpeg_cfg, %function
jpege_proc_jpeg_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r4, r0
	ldr	r8, [r1]
	.pad #40
	sub	sp, sp, #40
	mov	r5, r1
	mov	r9, r2
	cmp	r8, #0
	beq	.L163
	mov	r7, r0
	add	r6, sp, #8
	ldmia	r7!, {r0, r1, r2, r3}
	tst	r8, #1
	stmia	r6!, {r0, r1, r2, r3}
	ldm	r7, {r0, r1, r2, r3}
	stm	r6, {r0, r1, r2, r3}
	beq	.L131
	ldr	r3, [r5, #4]
	cmp	r3, #10
	str	r3, [r4, #4]
	bls	.L132
	movs	r2, #226
	ldr	r1, .L198
	ldr	r0, .L198+4
	bl	printk
	movs	r3, #8
	str	r3, [r4, #4]
.L132:
	ldr	r3, [r9, #4]
	cmp	r3, #2
	beq	.L134
	movs	r3, #2
	str	r3, [r9, #4]
.L134:
	ldr	r3, .L198+8
	ldr	r6, [r3]
	ands	r6, r6, #64
	beq	.L162
	ldr	r3, [r4, #8]
	mov	r2, #296
	ldr	r1, .L198
	movs	r6, #0
	ldr	r0, .L198+12
	str	r3, [sp, #4]
	ldr	r3, [r4, #4]
	str	r3, [sp]
	mov	r3, r8
	bl	printk
	b	.L162
.L131:
	tst	r8, #4
	beq	.L135
	ldr	r3, [r5, #8]
	subs	r3, r3, #1
	cmp	r3, #98
	bls	.L136
	movs	r2, #237
	ldr	r1, .L198
	ldr	r0, .L198+16
	bl	printk
	movs	r3, #80
	str	r3, [r5, #8]
.L136:
	ldr	r7, [r5, #8]
	ldr	r3, [r4, #8]
	cmp	r3, r7
	bne	.L137
.L148:
	movs	r6, #0
.L138:
	ldr	r3, [r5, #8]
	str	r3, [r4, #8]
	ldr	r3, [r5, #16]
	subs	r3, r3, #1
	cmp	r3, #98
	bls	.L149
	movs	r2, #245
	ldr	r1, .L198
	ldr	r0, .L198+20
	bl	printk
	movs	r3, #1
	str	r3, [r5, #16]
.L149:
	ldr	r3, [r5, #16]
	str	r3, [r4, #16]
	ldr	r3, [r5, #12]
	subs	r3, r3, #1
	cmp	r3, #98
	bls	.L150
	movs	r2, #250
	ldr	r1, .L198
	ldr	r0, .L198+24
	bl	printk
	movs	r3, #99
	str	r3, [r5, #12]
.L150:
	ldr	r2, .L198+8
	ldr	r3, [r5, #12]
	ldr	r2, [r2]
	str	r3, [r4, #12]
	lsls	r2, r2, #27
	bpl	.L151
	str	r3, [sp, #4]
	movs	r2, #255
	ldr	r3, [r4, #16]
	ldr	r1, .L198
	ldr	r0, .L198+28
	str	r3, [sp]
	ldr	r3, [r4, #8]
	bl	printk
.L151:
	cmp	r6, #0
	beq	.L134
	b	.L161
.L137:
	ldr	r3, [r4, #20]
	cbnz	r3, .L139
	movs	r1, #64
	ldr	r0, .L198+32
	bl	mpp_osal_malloc
	str	r0, [r4, #20]
.L139:
	ldr	r3, [r4, #24]
	cbnz	r3, .L140
	movs	r1, #64
	ldr	r0, .L198+32
	bl	mpp_osal_malloc
	str	r0, [r4, #24]
.L140:
	ldr	r6, [r4, #20]
	cbz	r6, .L141
	ldr	r9, [r4, #24]
	cmp	r9, #0
	bne	.L142
.L141:
	movs	r2, #193
	ldr	r1, .L198+36
	ldr	r0, .L198+40
	mvn	r6, #3
	bl	printk
	b	.L138
.L142:
	cmp	r7, #49
	bhi	.L143
	mov	r1, r7
	movw	r0, #5000
	bl	__aeabi_uidiv
.L144:
	ldr	ip, .L198+60
	add	r7, r9, #-1
	add	r1, r9, #63
	ldr	r9, .L198+64
	add	lr, ip, #64
	subs	r6, r6, #1
.L147:
	ldrb	r2, [ip], #1	@ zero_extendqisi2
	muls	r2, r0, r2
	adds	r2, r2, #50
	umull	r2, r3, r2, r9
	ldrb	r2, [lr], #1	@ zero_extendqisi2
	sbfx	r10, r3, #5, #16
	muls	r2, r0, r2
	cmp	r10, #0
	add	r2, r2, #50
	umull	r2, r3, r2, r9
	sbfx	r3, r3, #5, #16
	ble	.L164
	cmp	r10, #255
	it	ge
	movge	r10, #255
	uxtb	r2, r10
.L145:
	cmp	r3, #0
	strb	r2, [r6, #1]!
	ble	.L165
	cmp	r3, #255
	it	ge
	movge	r3, #255
	uxtb	r3, r3
.L146:
	strb	r3, [r7, #1]!
	cmp	r1, r7
	bne	.L147
	b	.L148
.L143:
	lsls	r0, r7, #1
	rsb	r0, r0, #200
	b	.L144
.L164:
	movs	r2, #1
	b	.L145
.L165:
	movs	r3, #1
	b	.L146
.L135:
	tst	r8, #2
	beq	.L134
	ldrd	r3, r2, [r5, #20]
	ldr	r1, [r5, #28]
	cmp	r3, #0
	beq	.L152
	cmp	r2, #0
	beq	.L152
	cmp	r1, #0
	beq	.L152
	ldr	r3, [r4, #20]
	cbnz	r3, .L153
	movs	r1, #64
	ldr	r0, .L198+44
	bl	mpp_osal_malloc
	str	r0, [r4, #20]
.L153:
	ldr	r3, [r4, #24]
	cbnz	r3, .L154
	movs	r1, #64
	ldr	r0, .L198+44
	bl	mpp_osal_malloc
	str	r0, [r4, #24]
.L154:
	ldr	r3, [r4, #28]
	cbnz	r3, .L155
	movs	r1, #64
	ldr	r0, .L198+44
	bl	mpp_osal_malloc
	str	r0, [r4, #28]
.L155:
	ldr	r0, [r4, #20]
	cbz	r0, .L156
	ldr	r3, [r4, #24]
	cbz	r3, .L156
	ldr	r3, [r4, #28]
	cbnz	r3, .L157
.L156:
	movw	r2, #267
	ldr	r1, .L198
	ldr	r0, .L198+48
	mvn	r6, #3
	bl	printk
.L129:
	mov	r0, r6
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L157:
	movs	r2, #64
	ldr	r1, [r5, #20]
	bl	_copy_from_user.constprop.1
	cbnz	r0, .L159
	movs	r2, #64
	ldr	r1, [r5, #24]
	ldr	r0, [r4, #24]
	bl	_copy_from_user.constprop.1
	cbnz	r0, .L159
	movs	r2, #64
	ldr	r1, [r5, #28]
	ldr	r0, [r4, #28]
	bl	_copy_from_user.constprop.1
	cmp	r0, #0
	beq	.L132
.L159:
	mvn	r6, #13
	b	.L129
.L152:
	strd	r2, r1, [sp]
	mov	r2, #286
	ldr	r1, .L198
	mvn	r6, #2
	ldr	r0, .L198+52
	bl	printk
.L161:
	mov	r2, #292
	ldr	r1, .L198
	ldr	r0, .L198+56
	mov	r7, r4
	bl	printk
	add	ip, sp, #8
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	r7!, {r0, r1, r2, r3}
	ldm	ip, {r0, r1, r2, r3}
	stm	r7, {r0, r1, r2, r3}
.L162:
	movs	r3, #0
	str	r3, [r4]
	str	r3, [r5]
	b	.L129
.L163:
	mov	r6, r8
	b	.L129
.L199:
	.align	2
.L198:
	.word	.LANCHOR0+118
	.word	.LC21
	.word	.LANCHOR1
	.word	.LC30
	.word	.LC22
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.word	.LANCHOR0+138
	.word	.LANCHOR0+162
	.word	.LC23
	.word	.LANCHOR0+314
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LANCHOR0+186
	.word	1374389535
	.fnend
	.size	jpege_proc_jpeg_cfg, .-jpege_proc_jpeg_cfg
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	jpege_proc_cfg, %function
jpege_proc_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r5, r2
	ldr	r6, .L237
	mov	r3, r0
	ldr	r7, [r0]
	mov	r4, r1
	ldr	r2, [r6]
	lsls	r0, r2, #31
	bpl	.L201
	strd	r1, r5, [sp]
	mov	r2, #330
	ldr	r1, .L237+4
	ldr	r0, .L237+8
	bl	printk
.L201:
	ldr	r3, .L237+12
	cmp	r4, r3
	beq	.L216
	bhi	.L203
	subs	r3, r3, #6
	cmp	r4, r3
	beq	.L204
	adds	r3, r3, #4
	cmp	r4, r3
	beq	.L205
	subs	r3, r3, #6
	cmp	r4, r3
	beq	.L206
.L207:
	mov	r3, r4
	mov	r2, #364
	ldr	r1, .L237+4
	mov	r4, #-1
	ldr	r0, .L237+16
	bl	printk
	b	.L202
.L203:
	ldr	r3, .L237+20
	cmp	r4, r3
	beq	.L216
	bhi	.L208
	sub	r3, r4, #3276800
	subs	r3, r3, #15
	cmp	r3, #1
	bhi	.L207
.L216:
	movs	r4, #0
	b	.L202
.L208:
	ldr	r3, .L237+24
	cmp	r4, r3
	beq	.L216
	b	.L207
.L206:
	ldr	r4, [r5, #8]
	cbz	r4, .L209
	add	r1, r5, #8
	add	r0, r7, #8
	bl	jpege_proc_prep_cfg
	movs	r3, #0
	str	r3, [r5, #8]
	mov	r4, r0
.L209:
	ldr	r3, [r5, #572]
	cbz	r3, .L210
	add	r2, r7, #104
	add	r1, r5, #572
	add	r0, r7, #572
	bl	jpege_proc_jpeg_cfg
	movs	r3, #0
	str	r3, [r5, #572]
	orrs	r4, r4, r0
.L210:
	ldr	r3, [r5, #3400]
	cbz	r3, .L202
	lsls	r1, r3, #31
	itttt	mi
	ldrmi	r2, [r5, #3404]
	strmi	r2, [r7, #3404]
	ldrmi	r2, [r5, #3408]
	strmi	r2, [r7, #3408]
	lsls	r2, r3, #30
	itt	mi
	ldrmi	r3, [r5, #3408]
	strmi	r3, [r7, #3408]
	movs	r3, #0
	str	r3, [r7, #3400]
	str	r3, [r5, #3400]
.L202:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L200
	mov	r3, r4
	movw	r2, #369
	ldr	r1, .L237+4
	ldr	r0, .L237+28
	bl	printk
.L200:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L204:
	mov	r1, r5
	add	r0, r7, #8
	bl	jpege_proc_prep_cfg
.L236:
	mov	r4, r0
	b	.L202
.L205:
	add	r2, r7, #104
	adds	r1, r5, #4
	add	r0, r7, #572
	bl	jpege_proc_jpeg_cfg
	b	.L236
.L238:
	.align	2
.L237:
	.word	.LANCHOR1
	.word	.LANCHOR0+334
	.word	.LC31
	.word	3276809
	.word	.LC32
	.word	3310593
	.word	3310595
	.word	.LC33
	.fnend
	.size	jpege_proc_cfg, .-jpege_proc_cfg
	.global	api_jpege
	.global	jpege_debug
	.global	jpege_chroma_quantizer
	.global	jpege_luma_quantizer
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.25965, %object
	.size	__FUNCTION__.25965, 20
__FUNCTION__.25965:
	.ascii	"jpege_proc_prep_cfg\000"
	.type	__func__.25969, %object
	.size	__func__.25969, 20
__func__.25969:
	.ascii	"jpege_proc_prep_cfg\000"
	.type	__func__.26069, %object
	.size	__func__.26069, 17
__func__.26069:
	.ascii	"jpege_add_Prefix\000"
	.type	__func__.26048, %object
	.size	__func__.26048, 15
__func__.26048:
	.ascii	"jpege_proc_hal\000"
	.type	__func__.25957, %object
	.size	__func__.25957, 16
__func__.25957:
	.ascii	"jpege_deinit_v2\000"
	.type	__FUNCTION__.25958, %object
	.size	__FUNCTION__.25958, 16
__FUNCTION__.25958:
	.ascii	"jpege_deinit_v2\000"
	.type	__func__.25946, %object
	.size	__func__.25946, 14
__func__.25946:
	.ascii	"jpege_init_v2\000"
	.type	__func__.25995, %object
	.size	__func__.25995, 20
__func__.25995:
	.ascii	"jpege_proc_jpeg_cfg\000"
	.type	__FUNCTION__.25980, %object
	.size	__FUNCTION__.25980, 24
__FUNCTION__.25980:
	.ascii	"jpege_gen_qt_by_qfactor\000"
	.type	__func__.25981, %object
	.size	__func__.25981, 24
__func__.25981:
	.ascii	"jpege_gen_qt_by_qfactor\000"
	.type	jpege_luma_quantizer, %object
	.size	jpege_luma_quantizer, 64
jpege_luma_quantizer:
	.byte	16
	.byte	11
	.byte	10
	.byte	16
	.byte	24
	.byte	40
	.byte	51
	.byte	61
	.byte	12
	.byte	12
	.byte	14
	.byte	19
	.byte	26
	.byte	58
	.byte	60
	.byte	55
	.byte	14
	.byte	13
	.byte	16
	.byte	24
	.byte	40
	.byte	57
	.byte	69
	.byte	56
	.byte	14
	.byte	17
	.byte	22
	.byte	29
	.byte	51
	.byte	87
	.byte	80
	.byte	62
	.byte	18
	.byte	22
	.byte	37
	.byte	56
	.byte	68
	.byte	109
	.byte	103
	.byte	77
	.byte	24
	.byte	35
	.byte	55
	.byte	64
	.byte	81
	.byte	104
	.byte	113
	.byte	92
	.byte	49
	.byte	64
	.byte	78
	.byte	87
	.byte	103
	.byte	121
	.byte	120
	.byte	101
	.byte	72
	.byte	92
	.byte	95
	.byte	98
	.byte	112
	.byte	100
	.byte	103
	.byte	99
	.type	jpege_chroma_quantizer, %object
	.size	jpege_chroma_quantizer, 64
jpege_chroma_quantizer:
	.byte	17
	.byte	18
	.byte	24
	.byte	47
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	18
	.byte	21
	.byte	26
	.byte	66
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	24
	.byte	26
	.byte	56
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	47
	.byte	66
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.byte	99
	.type	__FUNCTION__.25996, %object
	.size	__FUNCTION__.25996, 20
__FUNCTION__.25996:
	.ascii	"jpege_proc_jpeg_cfg\000"
	.type	__func__.26011, %object
	.size	__func__.26011, 15
__func__.26011:
	.ascii	"jpege_proc_cfg\000"
	.space	3
	.type	api_jpege, %object
	.size	api_jpege, 56
api_jpege:
	.word	.LC34
	.word	8
	.word	144
	.word	0
	.word	jpege_init_v2
	.word	jpege_deinit_v2
	.word	jpege_proc_cfg
	.word	0
	.word	jpege_start
	.word	0
	.word	jpege_proc_hal
	.word	jpege_add_Prefix
	.word	0
	.word	jpege_proc_show
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	jpege_debug, %object
	.size	jpege_debug, 4
jpege_debug:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\012--------jpege chn attr-------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC1:
	.ascii	"Width\000"
.LC2:
	.ascii	"ID\000"
.LC3:
	.ascii	"%7s%10s%10s%10s%10s%10s%10s\012\000"
.LC4:
	.ascii	"qfmin\000"
.LC5:
	.ascii	"qfmax\000"
.LC6:
	.ascii	"qfactor\000"
.LC7:
	.ascii	"quant\000"
.LC8:
	.ascii	"Height\000"
.LC9:
	.ascii	"%7d%10u%10u%10u%10u%10u%10u\012\000"
.LC10:
	.ascii	"change\000"
.LC11:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC12:
	.ascii	"\0013%d: invalid size w:h [%d:%d] stride [%d:%d]\012"
	.ascii	"\000"
.LC13:
	.ascii	"\0013%s:%d: failed to accept new prep config\012\000"
.LC14:
	.ascii	"\0016%s:%d: MPP_ENC_SET_PREP_CFG w:h [%d:%d] stride"
	.ascii	" [%d:%d]\012\000"
.LC15:
	.ascii	"\0016%s:%d: write EXIF data, total length %d\012\000"
.LC16:
	.ascii	"\0016%s:%d: write MPF data, total length %d\012\000"
.LC17:
	.ascii	"\0016%s:%d: enter ctx %p\012\000"
.LC18:
	.ascii	"\0013%s:%d: warning: invalid split arg %d > max %d\012"
	.ascii	"\000"
.LC19:
	.ascii	"\0013%s:%d: warning: only mcu split is supported\012"
	.ascii	"\000"
.LC20:
	.ascii	"\0016%s:%d: leave ctx %p\012\000"
.LC21:
	.ascii	"\0013%s:%d: invalid quality level is not in range ["
	.ascii	"0..10] set to default 8\012\000"
.LC22:
	.ascii	"\0013%s:%d: q_factor out of range, default set 80\012"
	.ascii	"\000"
.LC23:
	.ascii	"\0013%s:%d: qtable is null, malloc err\000"
.LC24:
	.ascii	"\0013%s:%d: qf_min out of range, default set 1\012\000"
.LC25:
	.ascii	"\0013%s:%d: qf_max out of range, default set 99\012"
	.ascii	"\000"
.LC26:
	.ascii	"\0016%s:%d: q_factor %d, qf_min %d, qf_max %d\012\000"
.LC27:
	.ascii	"\0013%s:%d: qtable is null, malloc err\012\000"
.LC28:
	.ascii	"\0013%s:%d: invalid qtable y %p u %p v %p\012\000"
.LC29:
	.ascii	"\0013%s:%d: failed to accept new rc config\012\000"
.LC30:
	.ascii	"\0016%s:%d: MPP_ENC_SET_CODEC_CFG change 0x%x jpeg "
	.ascii	"quant %d q_factor %d\012\000"
.LC31:
	.ascii	"\0016%s:%d: enter ctx %p cmd %x param %p\012\000"
.LC32:
	.ascii	"\0013%s:%d: No correspond cmd(%08x) found, and can "
	.ascii	"not config!\000"
.LC33:
	.ascii	"\0016%s:%d: leave ret %d\012\000"
.LC34:
	.ascii	"jpege_control\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
