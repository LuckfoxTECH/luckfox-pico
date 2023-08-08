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
	.file	"mpp_enc_ref.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	_check_is_mpp_enc_ref_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_check_is_mpp_enc_ref_cfg, %function
_check_is_mpp_enc_ref_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	mov	r3, r1
	mov	r2, r0
	cbnz	r1, .L2
	movs	r1, #30
	ldr	r0, .L6
	bl	printk
.L5:
	mov	r0, #-1
.L1:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L2:
	ldr	r1, [r1]
	ldr	r0, .L6+4
	cmp	r1, r0
	beq	.L4
	str	r1, [sp]
	movs	r1, #37
	ldr	r0, .L6+8
	bl	printk
	b	.L5
.L4:
	movs	r0, #0
	b	.L1
.L7:
	.align	2
.L6:
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.fnend
	.size	_check_is_mpp_enc_ref_cfg, .-_check_is_mpp_enc_ref_cfg
	.align	1
	.global	mpp_enc_ref_cfg_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_init, %function
mpp_enc_ref_cfg_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbnz	r0, .L9
	ldr	r1, .L13
	movs	r2, #48
	ldr	r0, .L13+4
.L12:
	bl	printk
	mvn	r0, #2
.L8:
	pop	{r4, pc}
.L9:
	movs	r1, #68
	ldr	r0, .L13+8
	bl	mpp_osal_calloc
	str	r0, [r4]
	cbnz	r0, .L11
	movs	r2, #55
	ldr	r1, .L13
	ldr	r0, .L13+12
	b	.L12
.L11:
	ldr	r3, .L13+16
	str	r3, [r0]
	movs	r0, #0
	b	.L8
.L14:
	.align	2
.L13:
	.word	.LANCHOR0
	.word	.LC3
	.word	.LANCHOR0+21
	.word	.LC4
	.word	.LC1
	.fnend
	.size	mpp_enc_ref_cfg_init, .-mpp_enc_ref_cfg_init
	.align	1
	.global	mpp_enc_ref_cfg_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_deinit, %function
mpp_enc_ref_cfg_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	cbnz	r0, .L16
.L18:
	mov	r3, r4
	movs	r2, #69
	ldr	r1, .L30
	mvn	r6, #5
	ldr	r0, .L30+4
	bl	printk
.L15:
	mov	r0, r6
	pop	{r4, r5, r6, pc}
.L16:
	ldr	r1, [r0]
	ldr	r0, .L30+8
	bl	_check_is_mpp_enc_ref_cfg
	mov	r6, r0
	cmp	r0, #0
	bne	.L18
	ldr	r4, [r4]
	ldr	r1, [r4, #32]
	cbz	r1, .L19
	ldr	r0, .L30+8
	bl	mpp_osal_free
.L19:
	ldr	r1, [r4, #36]
	movs	r5, #0
	str	r5, [r4, #32]
	cbz	r1, .L20
	ldr	r0, .L30+8
	bl	mpp_osal_free
.L20:
	str	r5, [r4, #36]
	cmp	r4, #0
	beq	.L15
	mov	r1, r4
	ldr	r0, .L30+8
	bl	mpp_osal_free
	b	.L15
.L31:
	.align	2
.L30:
	.word	.LANCHOR0+42
	.word	.LC5
	.word	.LANCHOR0+65
	.fnend
	.size	mpp_enc_ref_cfg_deinit, .-mpp_enc_ref_cfg_deinit
	.align	1
	.global	mpp_enc_ref_cfg_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_reset, %function
mpp_enc_ref_cfg_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r1, r0
	mov	r4, r0
	ldr	r0, .L43
	bl	_check_is_mpp_enc_ref_cfg
	mov	r5, r0
	cbnz	r0, .L36
	ldr	r1, [r4, #32]
	cbz	r1, .L34
	ldr	r0, .L43
	bl	mpp_osal_free
.L34:
	ldr	r1, [r4, #36]
	movs	r3, #0
	str	r3, [r4, #32]
	cbz	r1, .L35
	ldr	r0, .L43
	bl	mpp_osal_free
.L35:
	movs	r2, #64
	movs	r1, #0
	adds	r0, r4, #4
	bl	memset
	ldr	r3, .L43+4
	str	r3, [r4]
.L32:
	mov	r0, r5
	pop	{r3, r4, r5, pc}
.L36:
	mvn	r5, #5
	b	.L32
.L44:
	.align	2
.L43:
	.word	.LANCHOR0+88
	.word	.LC1
	.fnend
	.size	mpp_enc_ref_cfg_reset, .-mpp_enc_ref_cfg_reset
	.align	1
	.global	mpp_enc_ref_cfg_set_cfg_cnt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_set_cfg_cnt, %function
mpp_enc_ref_cfg_set_cfg_cnt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	mov	r5, r1
	mov	r1, r0
	ldr	r0, .L62
	mov	r6, r2
	bl	_check_is_mpp_enc_ref_cfg
	mov	r7, r0
	cmp	r0, #0
	bne	.L53
	ldrd	r9, r8, [r4, #32]
	cmp	r9, #0
	bne	.L47
	cmp	r8, #0
	beq	.L48
.L47:
	movs	r2, #111
	ldr	r1, .L62+4
	ldr	r0, .L62+8
	bl	printk
	cmp	r9, #0
	beq	.L49
	mov	r1, r9
	ldr	r0, .L62
	bl	mpp_osal_free
.L49:
	cmp	r8, #0
	beq	.L48
	mov	r1, r8
	ldr	r0, .L62
	bl	mpp_osal_free
.L48:
	cbz	r5, .L54
	movs	r1, #24
	ldr	r0, .L62
	muls	r1, r5, r1
	bl	mpp_osal_calloc
	mov	r9, r0
	cbnz	r0, .L51
	mov	r3, r5
	movs	r2, #119
	ldr	r1, .L62+4
	ldr	r0, .L62+12
	bl	printk
.L51:
	cbz	r6, .L55
	movs	r1, #20
	ldr	r0, .L62
	muls	r1, r6, r1
	bl	mpp_osal_calloc
	mov	r8, r0
	cbnz	r0, .L52
	mov	r3, r5
	movs	r2, #125
	ldr	r1, .L62+4
	ldr	r0, .L62+16
	bl	printk
.L52:
	movs	r3, #0
	strd	r5, r6, [r4, #16]
	strd	r3, r3, [r4, #24]
	strd	r9, r8, [r4, #32]
.L45:
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L54:
	mov	r9, r5
	b	.L51
.L55:
	mov	r8, r6
	b	.L52
.L53:
	mvn	r7, #2
	b	.L45
.L63:
	.align	2
.L62:
	.word	.LANCHOR0+110
	.word	.LANCHOR0+138
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.fnend
	.size	mpp_enc_ref_cfg_set_cfg_cnt, .-mpp_enc_ref_cfg_set_cfg_cnt
	.align	1
	.global	mpp_enc_ref_cfg_add_lt_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_add_lt_cfg, %function
mpp_enc_ref_cfg_add_lt_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r0
	.pad #28
	sub	sp, sp, #28
	mov	r7, r1
	mov	r1, r0
	ldr	r0, .L71
	mov	r5, r2
	bl	_check_is_mpp_enc_ref_cfg
	mov	r6, r0
	cbnz	r0, .L67
	ldr	r3, [r4, #8]
	cbz	r3, .L66
	ldr	r3, [r5, #8]
	mov	r2, r4
	movs	r1, #151
	ldr	r0, .L71+4
	str	r3, [sp, #16]
	ldr	r3, [r5, #20]
	str	r3, [sp, #12]
	ldr	r3, [r5, #16]
	str	r3, [sp, #8]
	ldr	r3, [r5, #4]
	str	r3, [sp, #4]
	ldr	r3, [r5]
	str	r3, [sp]
	ldr	r3, [r4, #24]
	bl	printk
.L66:
	ldr	r0, [r4, #24]
	mov	ip, #24
	ldr	r3, [r4, #32]
	mov	r1, r5
	mul	r2, ip, r7
	mla	r0, ip, r0, r3
	bl	memcpy
	ldr	r1, [r4, #24]
	add	r1, r1, r7
	str	r1, [r4, #24]
.L64:
	mov	r0, r6
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L67:
	mvn	r6, #5
	b	.L64
.L72:
	.align	2
.L71:
	.word	.LANCHOR0+166
	.word	.LC9
	.fnend
	.size	mpp_enc_ref_cfg_add_lt_cfg, .-mpp_enc_ref_cfg_add_lt_cfg
	.align	1
	.global	mpp_enc_ref_cfg_add_st_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_add_st_cfg, %function
mpp_enc_ref_cfg_add_st_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r0
	.pad #28
	sub	sp, sp, #28
	mov	r7, r1
	mov	r1, r0
	ldr	r0, .L80
	mov	r5, r2
	bl	_check_is_mpp_enc_ref_cfg
	mov	r6, r0
	cbz	r0, .L74
	mov	r3, r4
	movs	r2, #164
	ldr	r1, .L80+4
	mvn	r6, #5
	ldr	r0, .L80+8
	bl	printk
.L73:
	mov	r0, r6
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L74:
	ldr	r3, [r4, #8]
	cbz	r3, .L76
	ldr	r3, [r5, #12]
	mov	r2, r4
	movs	r1, #174
	ldr	r0, .L80+12
	str	r3, [sp, #16]
	ldr	r3, [r5, #8]
	str	r3, [sp, #12]
	ldr	r3, [r5, #16]
	str	r3, [sp, #8]
	ldr	r3, [r5, #4]
	str	r3, [sp, #4]
	ldr	r3, [r5]
	str	r3, [sp]
	ldr	r3, [r4, #28]
	bl	printk
.L76:
	ldr	r0, [r4, #28]
	mov	ip, #20
	ldr	r3, [r4, #36]
	mov	r1, r5
	mul	r2, ip, r7
	mla	r0, ip, r0, r3
	bl	memcpy
	ldr	r1, [r4, #28]
	add	r1, r1, r7
	str	r1, [r4, #28]
	b	.L73
.L81:
	.align	2
.L80:
	.word	.LANCHOR0+193
	.word	.LANCHOR0+220
	.word	.LC5
	.word	.LC10
	.fnend
	.size	mpp_enc_ref_cfg_add_st_cfg, .-mpp_enc_ref_cfg_add_st_cfg
	.align	1
	.global	mpp_enc_ref_cfg_check
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_check, %function
mpp_enc_ref_cfg_check:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r1, r0
	.pad #52
	sub	sp, sp, #52
	mov	r4, r0
	ldr	r0, .L139
	bl	_check_is_mpp_enc_ref_cfg
	mov	r5, r0
	cmp	r0, #0
	bne	.L112
	ldr	r3, [r4, #24]
	str	r3, [sp, #28]
	ldr	r3, [r4, #28]
	str	r3, [sp, #24]
	ldr	r3, [sp, #28]
	cmp	r3, #0
	beq	.L113
	ldr	r7, [r4, #32]
	mov	r6, r0
	mov	r10, r0
	str	r0, [sp, #20]
	strd	r0, r0, [sp, #12]
.L85:
	ldr	r3, [sp, #28]
	cmp	r6, r3
	bge	.L84
	ldrd	r3, r8, [r7, #4]
	ldr	r9, [r7]
	cmp	r9, #15
	str	r3, [sp, #32]
	mov	r3, #1
	lsl	fp, r3, r9
	ble	.L86
	mov	r3, r4
	strd	r6, r9, [sp]
	movs	r2, #220
	ldr	r1, .L139+4
	ldr	r0, .L139+8
	bl	printk
	movs	r3, #0
.L86:
	tst	r10, fp
	beq	.L87
	mov	r3, r4
	strd	r6, r9, [sp]
	movs	r2, #227
	ldr	r1, .L139+4
	ldr	r0, .L139+12
	bl	printk
	movs	r3, #0
.L110:
	ldr	r2, [sp, #16]
	cmp	r2, r9
	it	lt
	movlt	r2, r9
	str	r2, [sp, #16]
	ldr	r2, [sp, #32]
	cbz	r2, .L88
	strd	r6, r2, [sp]
	mov	r3, r4
	movs	r2, #243
	ldr	r1, .L139+4
	ldr	r0, .L139+16
	bl	printk
	cmp	r8, #6
	bhi	.L89
.L92:
	mov	r3, r4
	mov	r2, #360
	ldr	r1, .L139+4
	ldr	r0, .L139+20
	bl	printk
	movs	r3, #0
	mov	r0, #-1
	str	r3, [r4, #4]
	b	.L82
.L88:
	cmp	r8, #6
	bls	.L91
	cmp	r8, #25
	beq	.L91
.L111:
	ldr	r1, .L139+4
	mov	r3, r4
	ldr	r0, .L139+24
	movs	r2, #252
	strd	r6, r8, [sp]
.L138:
	bl	printk
	b	.L92
.L91:
	cmp	r3, #0
	beq	.L92
	ldr	r3, [r7, #16]
	cbz	r3, .L93
	ldr	r2, [r7, #20]
	add	r3, r3, r2
	ldr	r2, [sp, #20]
	cmp	r2, r3
	it	lt
	movlt	r2, r3
	str	r2, [sp, #20]
.L93:
	adds	r6, r6, #1
	adds	r7, r7, #24
	b	.L85
.L109:
	ldr	r7, [r4, #36]
	movs	r6, #0
	mov	r8, r6
	mov	r10, r6
	adds	r7, r7, #20
	mov	fp, r6
	str	r6, [sp, #28]
.L95:
	ldr	r3, [sp, #24]
	cmp	r6, r3
	blt	.L104
.L105:
	movs	r3, #0
	str	r3, [sp, #44]
	str	r3, [r4, #40]
	add	r0, sp, #44
	ldr	r3, [sp, #12]
	add	r7, r4, #40
	strd	r3, fp, [r4, #44]
	ldr	r3, [sp, #16]
	strd	r3, r10, [r4, #52]
	ldr	r3, [sp, #20]
	str	r3, [r4, #60]
	add	r3, r8, #-1
	str	r3, [r4, #64]
	bl	mpp_enc_refs_init
	mov	r1, r4
	mov	r6, r0
	ldr	r0, [sp, #44]
	bl	mpp_enc_refs_set_cfg
	orrs	r3, r6, r0
	ldr	r0, [sp, #44]
	ite	eq
	moveq	r6, #1
	movne	r6, #0
	bl	mpp_enc_refs_dryrun
	mov	r1, r7
	cmp	r0, #0
	ldr	r0, [sp, #44]
	it	ne
	movne	r6, #0
	bl	mpp_enc_refs_get_cpb_info
	cmp	r0, #0
	add	r0, sp, #44
	it	ne
	movne	r6, #0
	bl	mpp_enc_refs_deinit
	cmp	r0, #0
	it	eq
	moveq	r5, r6
	str	r5, [r4, #4]
	eor	r0, r5, #1
	rsbs	r0, r0, #0
	b	.L82
.L104:
	ldr	r9, [r7, #-16]
	ldr	r3, [r7, #-12]
	cmp	r9, #3
	str	r3, [sp, #36]
	ble	.L115
	mov	r3, r4
	strd	r6, r9, [sp]
	mov	r2, #280
	ldr	r1, .L139+4
	ldr	r0, .L139+28
	bl	printk
	movs	r3, #0
.L137:
	str	r3, [sp, #32]
	ldr	r3, [sp, #36]
	cmp	r3, #6
	bls	.L97
	cmp	r3, #29
	beq	.L97
	strd	r6, r3, [sp]
	movw	r2, #289
	mov	r3, r4
	ldr	r1, .L139+4
	ldr	r0, .L139+32
	bl	printk
	movs	r3, #0
	str	r3, [sp, #32]
.L97:
	ldr	r3, [r7, #-4]
	cmp	r3, #0
	bge	.L98
	strd	r6, r3, [sp]
	mov	r2, #296
	mov	r3, r4
	ldr	r1, .L139+4
	ldr	r0, .L139+36
	bl	printk
	movs	r3, #0
	str	r3, [r7, #-4]
.L98:
	cbz	r6, .L99
	ldr	r3, [sp, #24]
	subs	r3, r3, #1
	cmp	r3, r6
	bne	.L100
.L99:
	ldr	r3, [r7, #-20]
	cbz	r3, .L101
	str	r6, [sp]
	mov	r3, r4
	movw	r2, #305
	ldr	r1, .L139+4
	ldr	r0, .L139+40
	bl	printk
	cmp	r9, #0
	ble	.L92
.L102:
	strd	r6, r9, [sp]
	mov	r3, r4
	mov	r2, #312
	ldr	r1, .L139+4
	ldr	r0, .L139+44
	b	.L138
.L115:
	movs	r3, #1
	b	.L137
.L101:
	cmp	r9, #0
	bgt	.L102
.L100:
	ldr	r3, [sp, #32]
	cmp	r3, #0
	beq	.L92
	ldr	r3, [r7, #-20]
	cbnz	r3, .L103
	ldr	r2, [sp, #28]
	movs	r3, #1
	lsl	r3, r3, r9
	tst	r2, r3
	ittt	eq
	addeq	fp, fp, #1
	orreq	r2, r2, r3
	streq	r2, [sp, #28]
.L103:
	ldr	r3, [r7, #-4]
	cmp	r10, r9
	add	r6, r6, #1
	add	r7, r7, #20
	add	r3, r3, #1
	it	lt
	movlt	r10, r9
	add	r8, r8, r3
	b	.L95
.L112:
	mvn	r0, #5
.L82:
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L113:
	strd	r0, r0, [sp, #16]
	str	r0, [sp, #12]
.L84:
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne	.L109
	mov	r8, r3
	mov	r10, r3
	mov	fp, r3
	b	.L105
.L87:
	ldr	r2, [sp, #12]
	orr	r10, r10, fp
	adds	r2, r2, #1
	str	r2, [sp, #12]
	b	.L110
.L89:
	cmp	r8, #25
	beq	.L92
	b	.L111
.L140:
	.align	2
.L139:
	.word	.LANCHOR0+247
	.word	.LANCHOR0+269
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC20
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.fnend
	.size	mpp_enc_ref_cfg_check, .-mpp_enc_ref_cfg_check
	.align	1
	.global	mpp_enc_ref_cfg_set_keep_cpb
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_set_keep_cpb, %function
mpp_enc_ref_cfg_set_keep_cpb:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	mov	r1, r0
	ldr	r0, .L144
	bl	_check_is_mpp_enc_ref_cfg
	cbnz	r0, .L143
	str	r5, [r4, #12]
.L141:
	pop	{r3, r4, r5, pc}
.L143:
	mvn	r0, #5
	b	.L141
.L145:
	.align	2
.L144:
	.word	.LANCHOR0+291
	.fnend
	.size	mpp_enc_ref_cfg_set_keep_cpb, .-mpp_enc_ref_cfg_set_keep_cpb
	.align	1
	.global	mpp_enc_ref_cfg_show
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_show, %function
mpp_enc_ref_cfg_show:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r1, r0
	ldr	r0, .L150
	push	{r3, lr}
	.save {r3, lr}
	bl	_check_is_mpp_enc_ref_cfg
	cmp	r0, #0
	it	ne
	mvnne	r0, #5
	pop	{r3, pc}
.L151:
	.align	2
.L150:
	.word	.LANCHOR0+320
	.fnend
	.size	mpp_enc_ref_cfg_show, .-mpp_enc_ref_cfg_show
	.align	1
	.global	mpp_enc_ref_cfg_copy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_copy, %function
mpp_enc_ref_cfg_copy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r4, r0
	mov	r5, r1
	mov	r1, r0
	ldr	r0, .L175
	bl	_check_is_mpp_enc_ref_cfg
	cbz	r0, .L153
.L155:
	mvn	r7, #5
.L152:
	mov	r0, r7
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L153:
	mov	r1, r5
	ldr	r0, .L175
	bl	_check_is_mpp_enc_ref_cfg
	mov	r7, r0
	cmp	r0, #0
	bne	.L155
	ldr	r1, [r4, #32]
	ldrd	r6, r8, [r5, #16]
	cbz	r1, .L156
	ldr	r0, .L175
	bl	mpp_osal_free
.L156:
	ldr	r1, [r4, #36]
	movs	r3, #0
	mov	r9, r3
	str	r3, [r4, #32]
	cbz	r1, .L157
	ldr	r0, .L175
	bl	mpp_osal_free
.L157:
	str	r9, [r4, #36]
	movs	r2, #68
	mov	r1, r5
	mov	r0, r4
	bl	memcpy
	cmp	r6, #0
	beq	.L158
	mov	r9, #24
	ldr	r0, .L175
	mul	r1, r9, r6
	bl	mpp_osal_calloc
	mov	r10, r0
	cbnz	r0, .L159
	mov	r3, r6
	mov	r2, #416
	ldr	r1, .L175+4
	mov	r6, #-1
	ldr	r0, .L175+8
	bl	printk
.L160:
	str	r10, [r4, #32]
	cmp	r8, #0
	beq	.L161
.L164:
	mov	r10, #20
	ldr	r0, .L175
	mul	r1, r10, r8
	bl	mpp_osal_calloc
	mov	r9, r0
	cbnz	r0, .L162
	mov	r3, r8
	movw	r2, #431
	ldr	r1, .L175+4
	ldr	r0, .L175+12
	bl	printk
	str	r9, [r4, #36]
.L163:
	mov	r0, r4
	mov	r7, #-1
	bl	mpp_enc_ref_cfg_reset
	b	.L152
.L159:
	ldr	r2, [r5, #16]
	movs	r6, #0
	ldr	r1, [r5, #32]
	mul	r2, r9, r2
	bl	memcpy
	b	.L160
.L162:
	ldr	r2, [r5, #20]
	ldr	r1, [r5, #36]
	mul	r2, r10, r2
	bl	memcpy
	str	r9, [r4, #36]
.L161:
	cmp	r6, #0
	bne	.L163
	b	.L152
.L158:
	cmp	r8, #0
	beq	.L152
	b	.L164
.L176:
	.align	2
.L175:
	.word	.LANCHOR0+341
	.word	.LANCHOR0+362
	.word	.LC7
	.word	.LC8
	.fnend
	.size	mpp_enc_ref_cfg_copy, .-mpp_enc_ref_cfg_copy
	.align	1
	.global	mpp_enc_ref_cfg_get_cpb_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_cfg_get_cpb_info, %function
mpp_enc_ref_cfg_get_cpb_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r1, r0
	mov	r4, r0
	ldr	r0, .L180
	bl	_check_is_mpp_enc_ref_cfg
	cbnz	r0, .L179
	add	r0, r4, #40
.L177:
	pop	{r4, pc}
.L179:
	movs	r0, #0
	b	.L177
.L181:
	.align	2
.L180:
	.word	.LANCHOR0+383
	.fnend
	.size	mpp_enc_ref_cfg_get_cpb_info, .-mpp_enc_ref_cfg_get_cpb_info
	.align	1
	.global	mpp_enc_ref_default
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_enc_ref_default, %function
mpp_enc_ref_default:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L183
	bx	lr
.L184:
	.align	2
.L183:
	.word	.LANCHOR0+412
	.fnend
	.size	mpp_enc_ref_default, .-mpp_enc_ref_default
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.8856, %object
	.size	__func__.8856, 21
__func__.8856:
	.ascii	"mpp_enc_ref_cfg_init\000"
	.type	__FUNCTION__.8857, %object
	.size	__FUNCTION__.8857, 21
__FUNCTION__.8857:
	.ascii	"mpp_enc_ref_cfg_init\000"
	.type	__func__.8863, %object
	.size	__func__.8863, 23
__func__.8863:
	.ascii	"mpp_enc_ref_cfg_deinit\000"
	.type	__FUNCTION__.8862, %object
	.size	__FUNCTION__.8862, 23
__FUNCTION__.8862:
	.ascii	"mpp_enc_ref_cfg_deinit\000"
	.type	__FUNCTION__.8868, %object
	.size	__FUNCTION__.8868, 22
__FUNCTION__.8868:
	.ascii	"mpp_enc_ref_cfg_reset\000"
	.type	__FUNCTION__.8877, %object
	.size	__FUNCTION__.8877, 28
__FUNCTION__.8877:
	.ascii	"mpp_enc_ref_cfg_set_cfg_cnt\000"
	.type	__func__.8878, %object
	.size	__func__.8878, 28
__func__.8878:
	.ascii	"mpp_enc_ref_cfg_set_cfg_cnt\000"
	.type	__FUNCTION__.8885, %object
	.size	__FUNCTION__.8885, 27
__FUNCTION__.8885:
	.ascii	"mpp_enc_ref_cfg_add_lt_cfg\000"
	.type	__FUNCTION__.8892, %object
	.size	__FUNCTION__.8892, 27
__FUNCTION__.8892:
	.ascii	"mpp_enc_ref_cfg_add_st_cfg\000"
	.type	__func__.8893, %object
	.size	__func__.8893, 27
__func__.8893:
	.ascii	"mpp_enc_ref_cfg_add_st_cfg\000"
	.type	__FUNCTION__.8909, %object
	.size	__FUNCTION__.8909, 22
__FUNCTION__.8909:
	.ascii	"mpp_enc_ref_cfg_check\000"
	.type	__func__.8916, %object
	.size	__func__.8916, 22
__func__.8916:
	.ascii	"mpp_enc_ref_cfg_check\000"
	.type	__FUNCTION__.8936, %object
	.size	__FUNCTION__.8936, 29
__FUNCTION__.8936:
	.ascii	"mpp_enc_ref_cfg_set_keep_cpb\000"
	.type	__FUNCTION__.8940, %object
	.size	__FUNCTION__.8940, 21
__FUNCTION__.8940:
	.ascii	"mpp_enc_ref_cfg_show\000"
	.type	__FUNCTION__.8950, %object
	.size	__FUNCTION__.8950, 21
__FUNCTION__.8950:
	.ascii	"mpp_enc_ref_cfg_copy\000"
	.type	__func__.8952, %object
	.size	__func__.8952, 21
__func__.8952:
	.ascii	"mpp_enc_ref_cfg_copy\000"
	.type	__FUNCTION__.8958, %object
	.size	__FUNCTION__.8958, 29
__FUNCTION__.8958:
	.ascii	"mpp_enc_ref_cfg_get_cpb_info\000"
	.type	default_ref_cfg, %object
	.size	default_ref_cfg, 68
default_ref_cfg:
	.word	.LC1
	.word	1
	.word	0
	.word	0
	.word	0
	.word	1
	.word	0
	.word	1
	.word	0
	.word	default_st_ref_cfg
	.word	1
	.word	0
	.word	1
	.word	0
	.word	0
	.word	0
	.word	0
	.bss
	.align	2
	.type	default_st_ref_cfg, %object
	.size	default_st_ref_cfg, 20
default_st_ref_cfg:
	.space	20
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%d: %s input ref check NULL failed\012\000"
.LC1:
	.ascii	"mpp_enc_ref\000"
.LC2:
	.ascii	"\0013%d: %s input ref %p check %p failed\012\000"
.LC3:
	.ascii	"\0013%s:%d: invalid NULL input ref_cfg\012\000"
.LC4:
	.ascii	"\0013%s:%d: malloc failed\012\000"
.LC5:
	.ascii	"\0013%s:%d: input %p check failed\012\000"
.LC6:
	.ascii	"\0013%s:%d: do call reset before setup new cnout\012"
	.ascii	"\000"
.LC7:
	.ascii	"\0016%s:%d: failed to create %d lt ref cfg\012\000"
.LC8:
	.ascii	"\0016%s:%d: failed to create %d st ref cfg\012\000"
.LC9:
	.ascii	"\0016%d: ref %p add lt %d cfg idx %d tid %d gap %d "
	.ascii	"delay %d ref mode %x\012\000"
.LC10:
	.ascii	"\0016%d: ref %p add lt %d cfg non %d tid %d gap rep"
	.ascii	"eat %d ref mode %x arg %d\012\000"
.LC11:
	.ascii	"\0013%s:%d: ref cfg %p lt cfg %d with invalid lt_id"
	.ascii	"x %d larger than MPP_ENC_MAX_LT_REF_NUM\012\000"
.LC12:
	.ascii	"\0013%s:%d: ref cfg %p lt cfg %d with redefined lt_"
	.ascii	"idx %d config\012\000"
.LC13:
	.ascii	"\0013%s:%d: ref cfg %p lt cfg %d with invalid tempo"
	.ascii	"ral_id %d is non-zero\012\000"
.LC14:
	.ascii	"\0013%s:%d: ref cfg %p lt cfg %d with invalid ref m"
	.ascii	"ode %x\012\000"
.LC15:
	.ascii	"\0013%s:%d: ref cfg %p st cfg %d with invalid tempo"
	.ascii	"ral_id %d larger than MPP_ENC_MAX_TEMPORAL_LAYER_NU"
	.ascii	"M\012\000"
.LC16:
	.ascii	"\0013%s:%d: ref cfg %p st cfg %d with invalid ref m"
	.ascii	"ode %x\012\000"
.LC17:
	.ascii	"\0013%s:%d: ref cfg %p st cfg %d with negative repe"
	.ascii	"at %d set to zero\012\000"
.LC18:
	.ascii	"\0013%s:%d: ref cfg %p st cfg %d with invalid non-r"
	.ascii	"ef frame on head/tail frame\012\000"
.LC19:
	.ascii	"\0013%s:%d: ref cfg %p st cfg %d with invalid non-z"
	.ascii	"ero temporal id %d on head/tail frame\012\000"
.LC20:
	.ascii	"\0013%s:%d: check ref cfg %p failed\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
