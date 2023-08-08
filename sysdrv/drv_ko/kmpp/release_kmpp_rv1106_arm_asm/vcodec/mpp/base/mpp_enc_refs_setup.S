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
	.file	"mpp_enc_refs_setup.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpi_enc_gen_ref_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_enc_gen_ref_cfg, %function
mpi_enc_gen_ref_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 416
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #416
	sub	sp, sp, #416
	mov	r5, r1
	movs	r2, #96
	movs	r1, #0
	mov	r6, r0
	mov	r0, sp
	bl	memset
	mov	r2, #320
	movs	r1, #0
	add	r0, sp, #96
	bl	memset
	cmp	r5, #2
	beq	.L2
	cmp	r5, #3
	beq	.L3
	cmp	r5, #1
	beq	.L4
	mov	r3, r5
	movs	r2, #175
	ldr	r1, .L21
	movs	r5, #0
	ldr	r0, .L21+4
	mov	r4, r5
	bl	printk
	b	.L6
.L3:
	movs	r4, #1
	strd	r4, r5, [sp, #116]
	strd	r4, r5, [sp, #156]
	movs	r3, #2
	strd	r4, r5, [sp, #196]
	movs	r2, #8
	strd	r4, r5, [sp, #236]
	movs	r5, #9
	str	r2, [sp, #16]
	movs	r2, #4
	str	r3, [sp, #8]
	str	r2, [sp, #104]
	str	r3, [sp, #140]
	strd	r4, r3, [sp, #180]
	str	r3, [sp, #220]
	str	r2, [sp, #264]
.L6:
	orrs	r0, r4, r5
	beq	.L1
	mov	r2, r5
	mov	r1, r4
	mov	r0, r6
	bl	mpp_enc_ref_cfg_set_cfg_cnt
	cbz	r4, .L8
	mov	r2, sp
	movs	r1, #1
	mov	r0, r6
	bl	mpp_enc_ref_cfg_add_lt_cfg
.L8:
	cbz	r5, .L9
	add	r2, sp, #96
	mov	r1, r5
	mov	r0, r6
	bl	mpp_enc_ref_cfg_add_st_cfg
.L9:
	mov	r0, r6
	bl	mpp_enc_ref_cfg_check
.L1:
	add	sp, sp, #416
	@ sp needed
	pop	{r4, r5, r6, pc}
.L2:
	movs	r3, #1
	strd	r3, r5, [sp, #116]
	strd	r3, r5, [sp, #156]
	movs	r5, #5
	movs	r2, #4
	str	r3, [sp, #140]
	str	r2, [sp, #104]
	str	r2, [sp, #184]
.L20:
	movs	r4, #0
	b	.L6
.L4:
	movs	r3, #4
	strd	r5, r5, [sp, #116]
	str	r3, [sp, #104]
	movs	r5, #3
	b	.L20
.L22:
	.align	2
.L21:
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	mpi_enc_gen_ref_cfg, .-mpi_enc_gen_ref_cfg
	.align	1
	.global	mpi_enc_gen_smart_gop_ref_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_enc_gen_smart_gop_ref_cfg, %function
mpi_enc_gen_smart_gop_ref_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 416
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #420
	sub	sp, sp, #420
	ldrd	r9, r7, [r1, #4]
	mov	r6, r0
	movs	r2, #96
	movs	r1, #0
	mov	r0, sp
	movs	r5, #0
	bl	memset
	mov	r2, #320
	movs	r1, #0
	mov	r8, #2
	add	r4, sp, #96
	mov	r0, r4
	bl	memset
	movs	r2, #8
	movs	r1, #1
	mov	r0, r6
	bl	mpp_enc_ref_cfg_set_cfg_cnt
	movs	r1, #1
	mov	r0, r6
	mov	r2, sp
	strd	r5, r5, [sp]
	str	r8, [sp, #8]
	strd	r9, r5, [sp, #16]
	bl	mpp_enc_ref_cfg_add_lt_cfg
	cmp	r7, #1
	mov	r3, #3
	strd	r5, r5, [r4]
	mov	r1, #1
	strd	r3, r5, [r4, #8]
	str	r5, [r4, #16]
	ble	.L24
	strd	r5, r1, [r4, #20]
	mov	r1, r8
	subs	r7, r7, #2
	strd	r5, r5, [r4, #28]
	str	r7, [r4, #36]
.L24:
	movs	r0, #20
	movs	r2, #0
	muls	r0, r1, r0
	adds	r1, r1, #1
	adds	r3, r4, r0
	str	r2, [r4, r0]
	movs	r0, #3
	str	r2, [r3, #4]
	strd	r0, r2, [r3, #8]
	mov	r0, r6
	str	r2, [r3, #16]
	mov	r2, r4
	bl	mpp_enc_ref_cfg_add_st_cfg
	mov	r0, r6
	bl	mpp_enc_ref_cfg_check
	add	sp, sp, #420
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
	.fnend
	.size	mpi_enc_gen_smart_gop_ref_cfg, .-mpi_enc_gen_smart_gop_ref_cfg
	.align	1
	.global	mpi_enc_gen_hir_skip_ref
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_enc_gen_hir_skip_ref, %function
mpi_enc_gen_hir_skip_ref:
	.fnstart
	@ args = 0, pretend = 0, frame = 416
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #416
	sub	sp, sp, #416
	ldr	r7, [r1, #4]
	movs	r2, #96
	ldrd	r6, r4, [r1, #12]
	mov	r5, r0
	ldr	r8, [r1, #20]
	mov	r0, sp
	movs	r1, #0
	bl	memset
	mov	r2, #320
	movs	r1, #0
	add	r0, sp, #96
	bl	memset
	movs	r3, #2
	str	r3, [sp, #8]
	cmp	r8, #0
	beq	.L27
	cmp	r4, #1
	add	r2, sp, #96
	mov	r1, #1
	itt	hi
	strhi	r1, [sp, #120]
	movhi	r1, r3
	mov	r3, #20
	mla	r6, r4, r6, r6
	it	hi
	addhi	r4, r4, #-1
	mla	r3, r3, r1, r2
	it	hi
	strhi	r4, [sp, #132]
	adds	r4, r1, #1
	movs	r2, #0
	movs	r0, #2
	str	r6, [sp, #16]
	strd	r2, r0, [r3, #4]
	str	r2, [r3, #16]
.L29:
	movs	r2, #8
	movs	r1, #1
	mov	r0, r5
	bl	mpp_enc_ref_cfg_set_cfg_cnt
	movs	r1, #1
	mov	r0, r5
	mov	r2, sp
	bl	mpp_enc_ref_cfg_add_lt_cfg
	mov	r1, r4
	mov	r0, r5
	add	r2, sp, #96
	bl	mpp_enc_ref_cfg_add_st_cfg
	mov	r0, r5
	bl	mpp_enc_ref_cfg_check
	add	sp, sp, #416
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L27:
	subs	r4, r4, #1
	str	r3, [sp, #104]
	str	r4, [sp, #132]
	movs	r3, #1
	movs	r4, #3
	str	r7, [sp, #16]
	str	r3, [sp, #120]
	str	r4, [sp, #144]
	b	.L29
	.fnend
	.size	mpi_enc_gen_hir_skip_ref, .-mpi_enc_gen_hir_skip_ref
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.7562, %object
	.size	__func__.7562, 20
__func__.7562:
	.ascii	"mpi_enc_gen_ref_cfg\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: unsupport gop mode %d\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
