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
	.file	"h265e_api.c"
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
	.type	h265e_proc_enc_skip, %function
h265e_proc_enc_skip:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	h265e_proc_enc_skip, .-h265e_proc_enc_skip
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_proc_show, %function
h265e_proc_show:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #24
	sub	sp, sp, #24
	ldr	r4, [r1]
	mov	r5, r0
	ldr	r1, .L3
	mov	r6, r2
	bl	seq_puts
	ldr	r3, .L3+4
	ldr	r7, .L3+8
	mov	r0, r5
	ldr	r1, .L3+12
	mov	r2, r7
	str	r3, [sp, #4]
	ldr	r3, .L3+16
	str	r3, [sp]
	ldr	r3, .L3+20
	bl	seq_printf
	ldr	r1, [r4, #576]
	ldr	r0, .L3+24
	ldrd	r8, r9, [r4, #12]
	bl	strof_profle
	mov	r2, r6
	ldr	r1, .L3+28
	mov	r3, r8
	strd	r9, r0, [sp]
	mov	r0, r5
	bl	seq_printf
	mov	r0, r5
	ldr	r1, .L3+32
	bl	seq_puts
	ldr	r3, .L3+36
	mov	r2, r7
	ldr	r1, .L3+40
	mov	r0, r5
	str	r3, [sp, #16]
	ldr	r3, .L3+44
	str	r3, [sp, #12]
	ldr	r3, .L3+48
	str	r3, [sp, #8]
	ldr	r3, .L3+52
	str	r3, [sp, #4]
	ldr	r3, .L3+56
	str	r3, [sp]
	ldr	r3, .L3+60
	bl	seq_printf
	ldr	r0, [r4, #3404]
	bl	strof_bool
	ldr	r9, [r4, #3404]
	ldr	r10, [r4, #3408]
	mov	r8, r0
	movs	r0, #0
	bl	strof_bool
	mov	r3, r8
	movs	r2, #0
	ldr	r1, .L3+64
	strd	r10, r0, [sp, #4]
	mov	r0, r5
	strd	r2, r2, [sp, #12]
	mov	r2, r6
	str	r9, [sp]
	bl	seq_printf
	mov	r0, r5
	ldr	r1, .L3+68
	bl	seq_puts
	ldr	r3, .L3+72
	mov	r2, r7
	ldr	r1, .L3+76
	mov	r0, r5
	str	r3, [sp, #16]
	ldr	r3, .L3+80
	str	r3, [sp, #12]
	ldr	r3, .L3+84
	str	r3, [sp, #8]
	ldr	r3, .L3+88
	str	r3, [sp, #4]
	ldr	r3, .L3+92
	str	r3, [sp]
	ldr	r3, .L3+96
	bl	seq_printf
	ldr	r0, [r4, #768]
	rsb	r0, r0, #1
	bl	strof_bool
	ldr	r3, [r4, #712]
	mov	r2, r6
	ldr	r1, .L3+100
	str	r3, [sp, #16]
	ldr	r3, [r4, #760]
	str	r3, [sp, #12]
	ldr	r3, [r4, #756]
	str	r3, [sp, #8]
	ldr	r3, [r4, #772]
	str	r3, [sp, #4]
	ldr	r3, [r4, #776]
	str	r3, [sp]
	mov	r3, r0
	mov	r0, r5
	bl	seq_printf
	mov	r0, r5
	ldr	r1, .L3+104
	bl	seq_puts
	ldr	r3, .L3+108
	mov	r2, r7
	mov	r0, r5
	ldr	r1, .L3+112
	str	r3, [sp]
	ldr	r3, .L3+116
	bl	seq_printf
	ldr	r3, [r4, #752]
	mov	r2, r6
	ldr	r1, .L3+120
	mov	r0, r5
	str	r3, [sp]
	ldr	r3, [r4, #748]
	bl	seq_printf
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L4:
	.align	2
.L3:
	.word	.LC0
	.word	.LC4
	.word	.LC2
	.word	.LC3
	.word	.LC5
	.word	.LC1
	.word	16777220
	.word	.LC6
	.word	.LC7
	.word	.LC10
	.word	.LC9
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC8
	.word	.LC15
	.word	.LC16
	.word	.LC19
	.word	.LC18
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC17
	.word	.LC24
	.word	.LC25
	.word	.LC28
	.word	.LC27
	.word	.LC26
	.word	.LC29
	.fnend
	.size	h265e_proc_show, .-h265e_proc_show
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_add_sei, %function
h265e_add_sei:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r6, r1
	mov	r8, r2
	mov	r9, r3
	mov	r4, r0
	bl	mpp_packet_get_pos
	mov	r7, r0
	mov	r0, r4
	bl	mpp_packet_get_length
	mov	r2, r9
	mov	r1, r8
	mov	r5, r0
	ldr	r3, [sp, #32]
	adds	r0, r7, r0
	bl	h265e_data_to_sei
	str	r0, [r6]
	adds	r1, r5, r0
	mov	r0, r4
	bl	mpp_packet_set_length
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
	.fnend
	.size	h265e_add_sei, .-h265e_add_sei
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_proc_prep_cfg, %function
h265e_proc_prep_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 96
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #112
	sub	sp, sp, #112
	ldr	r8, [r1]
	mov	r4, r0
	mov	r5, r1
	movs	r2, #96
	mov	r1, r0
	add	r0, sp, #16
	bl	memcpy
	cmp	r8, #0
	bne	.L7
	mov	r1, #308
	ldr	r3, .L77
	str	r1, [sp]
	ldr	r2, .L77+4
	ldr	r0, .L77+8
	bl	printk
.L8:
	tst	r8, #524288
	itt	ne
	ldrne	r3, [r5, #40]
	strne	r3, [r4, #40]
	tst	r8, #16
	itt	ne
	ldrne	r3, [r5, #48]
	strne	r3, [r4, #48]
	tst	r8, #32
	itt	ne
	ldrne	r3, [r5, #52]
	strne	r3, [r4, #52]
	tst	r8, #256
	itt	ne
	ldrne	r3, [r5, #56]
	strne	r3, [r4, #56]
	tst	r8, #512
	beq	.L16
	add	r7, r5, #60
	add	r6, r4, #60
	ldmia	r7!, {r0, r1, r2, r3}
	stmia	r6!, {r0, r1, r2, r3}
	ldmia	r7!, {r0, r1, r2, r3}
	stmia	r6!, {r0, r1, r2, r3}
	ldr	r3, [r7]
	str	r3, [r6]
.L16:
	tst	r8, #17
	beq	.L18
	ldr	r1, [r4, #48]
	bic	r3, r1, #2
	cmp	r3, #1
	itete	eq
	ldreq	r3, [r5, #8]
	ldrne	r3, [r5, #4]
	streq	r3, [r4, #4]
	strne	r3, [r4, #4]
	ite	eq
	ldreq	r3, [r5, #4]
	ldrne	r3, [r5, #8]
	str	r3, [r4, #8]
	ldr	r3, [r5, #12]
	ldrd	r6, r0, [r4, #4]
	str	r3, [r4, #12]
	ldr	r3, [r5, #16]
	str	r3, [r4, #16]
	ldr	r2, [r5, #20]
	cmp	r2, #0
	beq	.L21
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq	.L21
	mul	r7, r3, r2
	mul	ip, r0, r6
	cmp	r7, ip
	bge	.L22
	strd	r6, r0, [sp]
	mov	r1, #352
	ldr	r0, .L77+12
	bl	printk
	ldr	r3, [r4, #4]
	str	r3, [r4, #20]
	ldr	r3, [r4, #8]
	str	r3, [r4, #24]
.L18:
	ldr	r5, [r4, #48]
	ldr	r3, [r4]
	bic	r5, r5, #2
	ldr	r2, [r4, #4]
	cmp	r5, #1
	orr	r3, r3, r8
	ldr	r0, [r4, #16]
	str	r3, [r4]
	ldrd	r3, r1, [r4, #8]
	bne	.L24
	cmp	r3, r1
	bgt	.L25
	cmp	r2, r0
	ble	.L35
.L25:
	strd	r1, r0, [sp]
	movw	r1, #377
.L76:
	ldr	r0, .L77+16
	bl	printk
	mvn	r1, #5
.L26:
	ldr	r3, [r4, #28]
	tst	r3, #15728640
	beq	.L28
	ldrd	r3, r2, [r4, #48]
	orrs	r0, r2, r3
	beq	.L28
	mov	r1, #394
	ldr	r0, .L77+20
	bl	printk
	mvn	r1, #5
.L28:
	ldr	r2, [r4, #44]
	ldrd	r3, r0, [r4, #32]
	cmp	r2, #2
	ldr	r5, [r4, #40]
	bhi	.L29
	cmp	r3, #14
	bhi	.L29
	cmp	r0, #22
	bhi	.L29
	cmp	r5, #18
	bls	.L30
.L29:
	strd	r0, r5, [sp]
	mov	r1, #404
	ldr	r0, .L77+24
	bl	printk
.L31:
	movw	r2, #409
	ldr	r1, .L77+28
	ldr	r0, .L77+32
	bl	printk
	movs	r2, #96
	mov	r0, r4
	add	r1, sp, #16
	bl	memcpy
.L33:
	movs	r0, #0
	add	sp, sp, #112
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L7:
	tst	r8, #4
	itt	ne
	ldrne	r3, [r5, #28]
	strne	r3, [r4, #28]
	tst	r8, #65536
	itt	ne
	ldrne	r3, [r5, #44]
	strne	r3, [r4, #44]
	tst	r8, #131072
	itt	ne
	ldrne	r3, [r5, #32]
	strne	r3, [r4, #32]
	tst	r8, #262144
	itt	ne
	ldrne	r3, [r5, #36]
	strne	r3, [r4, #36]
	b	.L8
.L22:
	str	r2, [r4, #20]
	bic	r1, r1, #2
	ldr	r3, [r5, #24]
	cmp	r1, #1
	str	r3, [r4, #24]
	bne	.L18
	strd	r3, r2, [r4, #20]
	b	.L18
.L21:
	strd	r6, r0, [r4, #20]
	b	.L18
.L24:
	cmp	r1, r2
	blt	.L27
	cmp	r3, r0
	ble	.L35
.L27:
	strd	r1, r0, [sp]
	movw	r1, #385
	b	.L76
.L35:
	movs	r1, #0
	b	.L26
.L30:
	cmp	r1, #0
	bne	.L31
	ldr	r3, [r4, #16]
	mov	r2, #414
	ldr	r1, .L77+28
	ldr	r0, .L77+36
	str	r3, [sp, #8]
	ldr	r3, [r4, #12]
	str	r3, [sp, #4]
	ldr	r3, [r4, #8]
	str	r3, [sp]
	ldr	r3, [r4, #4]
	bl	printk
	b	.L33
.L78:
	.align	2
.L77:
	.word	.LANCHOR0
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LANCHOR0+20
	.word	.LC36
	.word	.LC37
	.fnend
	.size	h265e_proc_prep_cfg, .-h265e_proc_prep_cfg
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_proc_dpb, %function
h265e_proc_dpb:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r7, .L89
	ldr	r8, [r1, #4]
	ldr	r2, [r7]
	lsls	r2, r2, #31
	bpl	.L80
	movs	r2, #229
	ldr	r1, .L89+4
	ldr	r0, .L89+8
	bl	printk
.L80:
	movw	r5, #19000
	mov	r1, r8
	ldr	r0, [r4, r5]
	bl	h265e_dpb_proc_cpb
	ldr	r0, [r4, r5]
	bl	h265e_dpb_get_curr
	ldrd	r2, [r8, #8]
	mov	r0, r4
	bl	h265e_slice_init
	mov	r1, r8
	ldr	r0, [r4, r5]
	bl	h265e_dpb_build_list
	mov	r6, r0
	cbnz	r0, .L82
	ldr	r3, [r4, r5]
	ldr	r3, [r3, #876]
	ldrd	r2, [r3, #24]
	strd	r2, [r8, #152]
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L79
	movs	r2, #238
	ldr	r1, .L89+4
	ldr	r0, .L89+12
	bl	printk
.L79:
	mov	r0, r6
	pop	{r4, r5, r6, r7, r8, pc}
.L82:
	mov	r6, #-1
	b	.L79
.L90:
	.align	2
.L89:
	.word	.LANCHOR1
	.word	.LANCHOR0+40
	.word	.LC38
	.word	.LC39
	.fnend
	.size	h265e_proc_dpb, .-h265e_proc_dpb
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_proc_h265_cfg, %function
h265e_proc_h265_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r5, r0
	ldr	r6, [r1]
	mov	r4, r1
	lsls	r7, r6, #31
	itttt	mi
	ldrmi	r3, [r1, #4]
	strmi	r3, [r0, #4]
	ldrmi	r3, [r1, #8]
	strmi	r3, [r0, #8]
	lsls	r0, r6, #20
	bpl	.L93
	add	r3, r1, #112
	add	r2, r5, #112
	adds	r1, r1, #136
.L94:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L94
.L93:
	lsls	r1, r6, #9
	bpl	.L95
	mov	r2, r4
	ldr	r3, [r2, #136]!	@ unaligned
	str	r3, [r5, #136]	@ unaligned
	ldr	r3, [r2, #4]	@ unaligned
	str	r3, [r5, #140]	@ unaligned
.L95:
	lsls	r2, r6, #19
	bpl	.L96
	mov	r1, r4
	ldr	r3, [r1, #196]!	@ unaligned
	str	r3, [r5, #196]	@ unaligned
	ldr	r3, [r1, #4]	@ unaligned
	str	r3, [r5, #200]	@ unaligned
	ldr	r3, [r1, #8]	@ unaligned
	str	r3, [r5, #204]	@ unaligned
.L96:
	ands	r7, r6, #8192
	beq	.L97
	mov	r1, r4
	ldr	r3, [r1, #184]!	@ unaligned
	str	r3, [r5, #184]	@ unaligned
	ldr	r3, [r1, #4]	@ unaligned
	str	r3, [r5, #188]	@ unaligned
	ldr	r3, [r1, #8]	@ unaligned
	str	r3, [r5, #192]	@ unaligned
.L97:
	lsls	r3, r6, #17
	bpl	.L98
	ldrd	r2, r3, [r4, #176]
	cmp	r2, r3
	beq	.L99
	mov	r1, #442
	ldr	r0, .L145
	bl	printk
	ldr	r3, [r4, #176]
	str	r3, [r4, #180]
.L99:
	add	r3, r4, #164
	add	r2, r5, #164
	add	r1, r4, #184
.L100:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L100
.L98:
	lsls	r0, r6, #16
	bpl	.L101
	add	r3, r4, #144
	add	r2, r5, #144
	add	r1, r4, #160
.L102:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L102
.L101:
	lsls	r1, r6, #15
	itt	mi
	ldrmi	r3, [r4, #160]	@ unaligned
	strmi	r3, [r5, #160]	@ unaligned
	lsls	r2, r6, #14
	bpl	.L104
	mov	r1, r4
	ldr	r3, [r1, #212]!	@ unaligned
	str	r3, [r5, #212]	@ unaligned
	ldr	r3, [r1, #4]	@ unaligned
	str	r3, [r5, #216]	@ unaligned
	ldr	r3, [r1, #8]	@ unaligned
	str	r3, [r5, #220]	@ unaligned
.L104:
	lsls	r3, r6, #13
	bpl	.L105
	add	r3, r4, #224
	add	r2, r5, #224
	add	r1, r4, #248
.L106:
	ldr	r0, [r3], #4	@ unaligned
	cmp	r3, r1
	str	r0, [r2], #4	@ unaligned
	bne	.L106
.L105:
	cbz	r7, .L107
	ldr	r3, [r4, #184]!	@ unaligned
	str	r3, [r5, #184]	@ unaligned
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [r5, #188]	@ unaligned
	ldr	r3, [r4, #8]	@ unaligned
	str	r3, [r5, #192]	@ unaligned
.L107:
	ldr	r3, [r5]
	movs	r0, #0
	orrs	r6, r6, r3
	str	r6, [r5]
	pop	{r3, r4, r5, r6, r7, pc}
.L146:
	.align	2
.L145:
	.word	.LC40
	.fnend
	.size	h265e_proc_h265_cfg, .-h265e_proc_h265_cfg
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_proc_cfg, %function
h265e_proc_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L180
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r5, r2
	ldr	r2, [r3]
	mov	r7, r0
	mov	r4, r1
	ldr	r6, [r0]
	tst	r2, #1
	mov	r8, r3
	beq	.L148
	str	r1, [sp]
	mov	r3, r0
	movw	r2, #499
	ldr	r1, .L180+4
	ldr	r0, .L180+8
	bl	printk
.L148:
	ldr	r3, .L180+12
	cmp	r4, r3
	beq	.L149
	bhi	.L150
	subs	r3, r3, #6
	cmp	r4, r3
	beq	.L151
	adds	r3, r3, #2
	cmp	r4, r3
	bne	.L153
	mov	r1, r5
	add	r0, r6, #8
	bl	h265e_proc_prep_cfg
.L179:
	mov	r4, r0
	b	.L154
.L150:
	ldr	r3, .L180+16
	cmp	r4, r3
	beq	.L165
	ldr	r3, .L180+20
	cmp	r4, r3
	beq	.L155
	ldr	r3, .L180+24
	cmp	r4, r3
	beq	.L156
.L153:
	mov	r2, r4
	movw	r1, #563
	ldr	r0, .L180+28
	mov	r4, #-1
	bl	printk
	b	.L154
.L151:
	ldr	r4, [r5, #8]
	cbz	r4, .L157
	add	r1, r5, #8
	add	r0, r6, #8
	bl	h265e_proc_prep_cfg
	movs	r3, #0
	str	r3, [r5, #8]
	mov	r4, r0
.L157:
	ldr	r3, [r5, #572]
	cbz	r3, .L158
	add	r1, r5, #572
	add	r0, r6, #572
	bl	h265e_proc_h265_cfg
	movs	r3, #0
	str	r3, [r5, #572]
	orrs	r4, r4, r0
.L158:
	ldr	r3, [r5, #3400]
	cbz	r3, .L154
	ldr	r3, [r5, #3404]
	cbz	r3, .L159
	movs	r3, #0
	movs	r2, #1
	strd	r2, r3, [r6, #716]
	ldr	r1, [r5, #3404]
	cmp	r1, #2
	it	eq
	moveq	r3, r2
	str	r3, [r6, #720]
	ldr	r3, [r5, #3408]
	str	r3, [r6, #724]
.L161:
	movs	r3, #0
	str	r3, [r5, #3400]
.L154:
	ldr	r3, [r8]
	lsls	r3, r3, #31
	bpl	.L147
	mov	r3, r7
	mov	r2, #568
	ldr	r1, .L180+4
	ldr	r0, .L180+32
	bl	printk
.L147:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L159:
	str	r3, [r6, #716]
	b	.L161
.L156:
	mov	r0, r7
	bl	h265e_set_extra_info
	mov	r1, r5
	mov	r0, r7
	bl	h265e_get_extra_info
.L165:
	movs	r4, #0
	b	.L154
.L149:
	adds	r1, r5, #4
	add	r0, r6, #572
	bl	h265e_proc_h265_cfg
	b	.L179
.L155:
	ldr	r4, [r5, #4]
	movs	r2, #0
	cbz	r4, .L162
	movs	r3, #1
	strd	r3, r2, [r6, #716]
	ldr	r1, [r5, #4]
	cmp	r1, #2
	it	ne
	movne	r3, r2
	str	r3, [r6, #720]
	ldr	r3, [r5, #8]
	str	r3, [r6, #724]
	b	.L165
.L162:
	str	r4, [r6, #716]
	b	.L154
.L181:
	.align	2
.L180:
	.word	.LANCHOR1
	.word	.LANCHOR0+55
	.word	.LC41
	.word	3276807
	.word	3276815
	.word	3309825
	.word	3276814
	.word	.LC42
	.word	.LC43
	.fnend
	.size	h265e_proc_cfg, .-h265e_proc_cfg
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_deinit, %function
h265e_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r6, .L202
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L183
	mov	r3, r0
	movs	r2, #162
	ldr	r1, .L202+4
	ldr	r0, .L202+8
	bl	printk
.L183:
	cbnz	r4, .L184
	movs	r2, #165
	ldr	r1, .L202+4
	ldr	r0, .L202+12
	bl	printk
	mvn	r0, #2
.L182:
	pop	{r4, r5, r6, pc}
.L184:
	movw	r5, #19008
	ldr	r0, [r4, r5]
	bl	h265e_deinit_extra_info
	ldr	r1, [r4, r5]
	cbz	r1, .L186
	ldr	r0, .L202+16
	bl	mpp_osal_free
.L186:
	movs	r3, #0
	str	r3, [r4, r5]
	movw	r3, #19012
	ldr	r1, [r4, r3]
	mov	r5, r3
	cbz	r1, .L187
	ldr	r0, .L202+16
	bl	mpp_osal_free
.L187:
	movs	r3, #0
	str	r3, [r4, r5]
	movw	r3, #19016
	adds	r0, r4, r3
	ldr	r3, [r4, r3]
	cbz	r3, .L188
	bl	mpp_packet_deinit
.L188:
	movw	r3, #19000
	ldr	r0, [r4, r3]
	bl	h265e_dpb_deinit
	ldr	r0, [r6]
	ands	r0, r0, #1
	beq	.L182
	mov	r3, r4
	movs	r2, #178
	ldr	r1, .L202+4
	ldr	r0, .L202+20
	bl	printk
	movs	r0, #0
	b	.L182
.L203:
	.align	2
.L202:
	.word	.LANCHOR1
	.word	.LANCHOR0+70
	.word	.LC44
	.word	.LC45
	.word	.LANCHOR0+83
	.word	.LC43
	.fnend
	.size	h265e_deinit, .-h265e_deinit
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_init, %function
h265e_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r5, r1
	ldr	r4, [r1, #8]
	mov	r7, r0
	cbnz	r0, .L205
	movs	r2, #38
	ldr	r1, .L215
	ldr	r0, .L215+4
	mvn	r5, #2
	bl	printk
.L204:
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L205:
	ldr	r3, .L215+8
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #1
	beq	.L207
	mov	r3, r0
	movs	r2, #42
	ldr	r1, .L215
	ldr	r0, .L215+12
	bl	printk
.L207:
	ldr	r2, [r5]
	ldr	r3, .L215+16
	cmp	r2, r3
	beq	.L208
	movs	r1, #44
	ldr	r3, .L215+20
	str	r1, [sp]
	ldr	r2, .L215+24
	ldr	r0, .L215+28
	bl	printk
.L208:
	ldr	r3, [r5, #8]
	add	r0, r7, #18944
	mov	r2, #296
	movs	r1, #0
	adds	r0, r0, #76
	movs	r6, #1
	str	r3, [r7]
	movw	r9, #19008
	bl	memset
	str	r6, [r5, #16]
	movs	r1, #204
	ldr	r0, .L215+20
	bl	mpp_osal_calloc
	mov	r1, #1536
	str	r0, [r7, r9]
	ldr	r0, .L215+20
	bl	mpp_osal_calloc
	movw	r3, #19012
	mov	r2, #1536
	mov	r1, r0
	str	r0, [r7, r3]
	add	r0, r7, #18944
	adds	r0, r0, #72
	bl	mpp_packet_init
	mov	r5, r0
	cmp	r0, #0
	bne	.L204
	ldr	r0, [r7, r9]
	bl	h265e_init_extra_info
	ldr	r3, [r7]
	mov	ip, #26
	movs	r1, #120
	movs	r2, #51
	movs	r0, #10
	str	r1, [r3, #580]
	movs	r1, #32
	str	ip, [r3, #628]
	movw	lr, #4708
	strd	r1, r1, [r3, #588]
	movs	r1, #5
	str	r5, [r3, #680]
	strd	r1, r5, [r3, #764]
	movs	r1, #2
	str	r6, [r3, #576]
	str	r5, [r3, #716]
	str	r5, [r3, #732]
	str	r5, [r3, #712]
	str	r1, [r3, #784]
	str	r5, [r3, #744]
	strd	r2, r0, [r3, #612]
	strd	r2, r0, [r3, #620]
	strd	r6, r5, [r3, #656]
	strd	r5, r5, [r3, #664]
	strd	r5, r5, [r3, #672]
	strd	r5, r5, [r3, #596]
	strd	r6, r6, [r3, #604]
	strd	r6, r6, [r3, #756]
	strd	r6, r6, [r3, #788]
	ldr	r3, [r7]
	ldr	r9, .L215+44
	str	r6, [r3, lr]
	mov	lr, #60
	ldr	r3, .L215+32
	str	r5, [r4, #8]
	strd	r5, r1, [r4, #36]
	strd	r9, r3, [r4, #116]
	ldr	r3, .L215+36
	strd	r1, r1, [r4, #44]
	strd	r5, r5, [r4, #52]
	strd	r3, r5, [r4, #124]
	movs	r3, #30
	strd	r5, r5, [r4, #60]
	strd	r3, r6, [r4, #132]
	strd	r5, r3, [r4, #140]
	strd	r3, r0, [r4, #204]
	movs	r3, #160
	str	r5, [r4, #104]
	strd	r3, ip, [r4, #212]
	movs	r3, #15
	str	r1, [r4, #112]
	str	r3, [r4, #232]
	movs	r3, #4
	str	r6, [r4, #148]
	str	lr, [r4, #152]
	str	r6, [r4, #160]
	str	r2, [r4, #220]
	strd	r2, r0, [r4, #224]
	strd	r3, r1, [r4, #240]
	movs	r3, #28
	strd	r3, r2, [r4, #252]
	ldr	r3, [r8]
	str	ip, [r4, #248]
	str	r2, [r4, #260]
	lsls	r4, r3, #31
	bpl	.L204
	mov	r3, r7
	movs	r2, #154
	ldr	r1, .L215
	ldr	r0, .L215+40
	bl	printk
	b	.L204
.L216:
	.align	2
.L215:
	.word	.LANCHOR0+96
	.word	.LC45
	.word	.LANCHOR1
	.word	.LC44
	.word	16777220
	.word	.LANCHOR0+107
	.word	.LC46
	.word	.LC31
	.word	2500000
	.word	1500000
	.word	.LC43
	.word	2000000
	.fnend
	.size	h265e_init, .-h265e_init
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_start, %function
h265e_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r1
	ldr	r4, .L227
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L218
	movs	r2, #203
	ldr	r1, .L227+4
	ldr	r0, .L227+8
	bl	printk
.L218:
	ldr	r3, [r5]
	cbnz	r3, .L219
	movs	r2, #213
	ldr	r1, .L227+4
	ldr	r0, .L227+12
	bl	printk
.L219:
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L220
	movs	r2, #219
	ldr	r1, .L227+4
	ldr	r0, .L227+16
	bl	printk
.L220:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L228:
	.align	2
.L227:
	.word	.LANCHOR1
	.word	.LANCHOR0+118
	.word	.LC38
	.word	.LC47
	.word	.LC39
	.fnend
	.size	h265e_start, .-h265e_start
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_proc_hal, %function
h265e_proc_hal:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r1
	mov	r4, r0
	cbnz	r0, .L230
	movs	r2, #251
	ldr	r1, .L237
	ldr	r0, .L237+4
	bl	printk
	mvn	r0, #2
.L229:
	pop	{r4, r5, r6, pc}
.L230:
	ldr	r5, .L237+8
	ldr	r2, [r5]
	lsls	r3, r2, #31
	bpl	.L232
	mov	r3, r0
	movs	r2, #255
	ldr	r1, .L237
	ldr	r0, .L237+12
	bl	printk
.L232:
	mov	r0, r4
	bl	h265e_syntax_fill
	movs	r3, #1
	str	r3, [r6]
	add	r3, r4, #18944
	ldr	r0, [r5]
	adds	r3, r3, #76
	str	r3, [r6, #16]
	ands	r0, r0, #1
	beq	.L229
	mov	r3, r4
	mov	r2, #260
	ldr	r1, .L237
	ldr	r0, .L237+16
	bl	printk
	movs	r0, #0
	b	.L229
.L238:
	.align	2
.L237:
	.word	.LANCHOR0+130
	.word	.LC45
	.word	.LANCHOR1
	.word	.LC48
	.word	.LC49
	.fnend
	.size	h265e_proc_hal, .-h265e_proc_hal
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_gen_hdr, %function
h265e_gen_hdr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L249
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L240
	mov	r3, r0
	movs	r2, #186
	ldr	r1, .L249+4
	ldr	r0, .L249+8
	bl	printk
.L240:
	mov	r0, r4
	bl	h265e_set_extra_info
	mov	r1, r6
	mov	r0, r4
	bl	h265e_get_extra_info
	movw	r3, #19000
	adds	r0, r4, r3
	ldr	r3, [r4, r3]
	cbnz	r3, .L241
	bl	h265e_dpb_init
.L241:
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L242
	mov	r3, r4
	movs	r2, #194
	ldr	r1, .L249+4
	ldr	r0, .L249+12
	bl	printk
.L242:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L250:
	.align	2
.L249:
	.word	.LANCHOR1
	.word	.LANCHOR0+145
	.word	.LC44
	.word	.LC43
	.fnend
	.size	h265e_gen_hdr, .-h265e_gen_hdr
	.global	api_h265e
	.global	h265e_debug
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.26928, %object
	.size	__FUNCTION__.26928, 20
__FUNCTION__.26928:
	.ascii	"h265e_proc_prep_cfg\000"
	.type	__func__.26932, %object
	.size	__func__.26932, 20
__func__.26932:
	.ascii	"h265e_proc_prep_cfg\000"
	.type	__func__.26899, %object
	.size	__func__.26899, 15
__func__.26899:
	.ascii	"h265e_proc_dpb\000"
	.type	__func__.26950, %object
	.size	__func__.26950, 15
__func__.26950:
	.ascii	"h265e_proc_cfg\000"
	.type	__func__.26878, %object
	.size	__func__.26878, 13
__func__.26878:
	.ascii	"h265e_deinit\000"
	.type	__FUNCTION__.26879, %object
	.size	__FUNCTION__.26879, 13
__FUNCTION__.26879:
	.ascii	"h265e_deinit\000"
	.type	__func__.26870, %object
	.size	__func__.26870, 11
__func__.26870:
	.ascii	"h265e_init\000"
	.type	__FUNCTION__.26871, %object
	.size	__FUNCTION__.26871, 11
__FUNCTION__.26871:
	.ascii	"h265e_init\000"
	.type	__func__.26891, %object
	.size	__func__.26891, 12
__func__.26891:
	.ascii	"h265e_start\000"
	.type	__func__.26906, %object
	.size	__func__.26906, 15
__func__.26906:
	.ascii	"h265e_proc_hal\000"
	.type	__func__.26885, %object
	.size	__func__.26885, 14
__func__.26885:
	.ascii	"h265e_gen_hdr\000"
	.space	1
	.type	api_h265e, %object
	.size	api_h265e, 56
api_h265e:
	.word	.LC50
	.word	16777220
	.word	19348
	.word	0
	.word	h265e_init
	.word	h265e_deinit
	.word	h265e_proc_cfg
	.word	h265e_gen_hdr
	.word	h265e_start
	.word	h265e_proc_dpb
	.word	h265e_proc_hal
	.word	h265e_add_sei
	.word	h265e_proc_enc_skip
	.word	h265e_proc_show
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	h265e_debug, %object
	.size	h265e_debug, 4
h265e_debug:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\012--------h265e chn attr-------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC1:
	.ascii	"Width\000"
.LC2:
	.ascii	"ID\000"
.LC3:
	.ascii	"%7s%10s%10s%10s\012\000"
.LC4:
	.ascii	"profile\000"
.LC5:
	.ascii	"Height\000"
.LC6:
	.ascii	"%7d%10u%10u%10s\012\000"
.LC7:
	.ascii	"\012--------Syntax INFO1---------------------------"
	.ascii	"--------------------------------------------------\012"
	.ascii	"\000"
.LC8:
	.ascii	"SlcspltEn\000"
.LC9:
	.ascii	"%7s%10s%10s%10s%15s%15s%15s\012\000"
.LC10:
	.ascii	"RefreshNum\000"
.LC11:
	.ascii	"RefreshMode\000"
.LC12:
	.ascii	"IntraRefresh\000"
.LC13:
	.ascii	"Slcsize\000"
.LC14:
	.ascii	"SplitMode\000"
.LC15:
	.ascii	"%7d%10s%10u%15u%15s%15u%15u\012\000"
.LC16:
	.ascii	"--------Syntax INFO2-------------------------------"
	.ascii	"-----------------------------------------------\012"
	.ascii	"\000"
.LC17:
	.ascii	"DblkEn\000"
.LC18:
	.ascii	"%7s%10s%8s%8s%10s%10s%15s\012\000"
.LC19:
	.ascii	"IntraSmoothing\000"
.LC20:
	.ascii	"Saochroma\000"
.LC21:
	.ascii	"Saoluma\000"
.LC22:
	.ascii	"Beta\000"
.LC23:
	.ascii	"Tc\000"
.LC24:
	.ascii	"%7d%10s%7d%8d%10d%10d%12d\012\000"
.LC25:
	.ascii	"------Trans INFO-----------------------------------"
	.ascii	"------------------------------------------------\012"
	.ascii	"\000"
.LC26:
	.ascii	"CbQpOffset\000"
.LC27:
	.ascii	"%7s%12s%12s\012\000"
.LC28:
	.ascii	"CrQpOffset\000"
.LC29:
	.ascii	"%7d%12d%12d\012\000"
.LC30:
	.ascii	"change\000"
.LC31:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC32:
	.ascii	"\0013%d: config maybe err should realloc buff max w"
	.ascii	":h [%d:%d] enc w:h[%d:%d]\000"
.LC33:
	.ascii	"\0013%d: invalid size w:h [%d:%d] stride [%d:%d]\012"
	.ascii	"\000"
.LC34:
	.ascii	"\0013%d: invalid cfg fbc data no support mirror %d "
	.ascii	"or rotaion %d\000"
.LC35:
	.ascii	"\0013%d: invalid color range %d colorspace %d prima"
	.ascii	"ries %d transfer characteristic %d\012\000"
.LC36:
	.ascii	"\0013%s:%d: failed to accept new prep config\012\000"
.LC37:
	.ascii	"\0016%s:%d: MPP_ENC_SET_PREP_CFG w:h [%d:%d] stride"
	.ascii	" [%d:%d]\012\000"
.LC38:
	.ascii	"\0016%s:%d: enter\012\000"
.LC39:
	.ascii	"\0016%s:%d: leave\012\000"
.LC40:
	.ascii	"\0016%d: cr_qp_offset %d MUST equal to cb_qp_offset"
	.ascii	" %d. FORCE to same value\012\000"
.LC41:
	.ascii	"\0016%s:%d: enter ctx %p cmd %08x\012\000"
.LC42:
	.ascii	"\0013%d: No correspond %08x found, and can not conf"
	.ascii	"ig!\012\000"
.LC43:
	.ascii	"\0016%s:%d: leave ctx %p\012\000"
.LC44:
	.ascii	"\0016%s:%d: enter ctx %p\012\000"
.LC45:
	.ascii	"\0013%s:%d: invalid NULL ctx\012\000"
.LC46:
	.ascii	"ctrlCfg->coding == MPP_VIDEO_CodingHEVC\000"
.LC47:
	.ascii	"\0016%s:%d: drop one frame\012\000"
.LC48:
	.ascii	"\0016%s:%d: enter ctx %p \012\000"
.LC49:
	.ascii	"\0016%s:%d: leave ctx %p \012\000"
.LC50:
	.ascii	"h265e_control\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
