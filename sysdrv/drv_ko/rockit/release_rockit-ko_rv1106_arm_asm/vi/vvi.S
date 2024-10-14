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
	.file	"vvi.c"
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
	.type	vi_compat_ioctl, %function
vi_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vi_compat_ioctl, .-vi_compat_ioctl
	.align	1
	.global	vvi_get_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_get_version, %function
vvi_get_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movs	r2, #112
	mov	r4, r1
	movs	r1, #0
	add	r0, r4, #12
	movs	r5, #0
	bl	memset
	movs	r3, #1
	strb	r3, [r4]
	movs	r3, #22
	strb	r3, [r4, #4]
	ldr	r3, .L4
	strb	r5, [r4, #1]
	strb	r5, [r4, #2]
	strb	r5, [r4, #3]
	ldr	r6, [r3]
	strb	r5, [r4, #5]
	strb	r5, [r4, #6]
	strb	r5, [r4, #7]
	mov	r0, r6
	strb	r5, [r4, #8]
	strb	r5, [r4, #9]
	strb	r5, [r4, #10]
	strb	r5, [r4, #11]
	bl	strlen
	movs	r3, #20
	strb	r5, [r4, #53]
	strb	r3, [r4, #52]
	movs	r3, #17
	strb	r5, [r4, #54]
	strb	r5, [r4, #55]
	strb	r3, [r4, #88]
	strb	r5, [r4, #89]
	strb	r5, [r4, #90]
	strb	r5, [r4, #91]
	cmp	r0, #31
	str	r0, [r4, #12]	@ unaligned
	bls	.L3
	movs	r3, #31
	strb	r5, [r4, #13]
	strb	r3, [r4, #12]
	strb	r5, [r4, #14]
	strb	r5, [r4, #15]
.L3:
	ldr	r2, [r4, #12]	@ unaligned
	mov	r1, r6
	add	r0, r4, #16
	bl	strncpy
	ldr	r2, [r4, #52]	@ unaligned
	ldr	r1, .L4+4
	add	r0, r4, #56
	bl	strncpy
	ldr	r2, [r4, #88]	@ unaligned
	ldr	r1, .L4+8
	add	r0, r4, #92
	bl	strncpy
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L5:
	.align	2
.L4:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.fnend
	.size	vvi_get_version, .-vvi_get_version
	.global	__aeabi_uidiv
	.global	__aeabi_uidivmod
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_new_wrap_line, %function
get_new_wrap_line:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r5, r1
	mov	r7, r0
	mov	r1, r0
	.pad #24
	sub	sp, sp, #24
	mov	r8, r2
	mul	r0, r3, r2
	mov	r6, r3
	bl	__aeabi_uidiv
	movw	r3, #275
	ldr	r2, .L13
	movs	r1, #4
	mov	r4, r0
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r0, #8
	strd	r5, r8, [sp, #16]
	strd	r6, r7, [sp, #8]
	ldr	r3, .L13+4
	bl	vlog
	cmp	r4, r5
	bcs	.L9
.L8:
	mov	r1, r4
	mov	r0, r5
	bl	__aeabi_uidivmod
	and	r3, r4, #1
	orrs	r3, r1, r3
	beq	.L6
	subs	r4, r4, #1
	b	.L8
.L9:
	mov	r4, r5
.L6:
	mov	r0, r4
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L14:
	.align	2
.L13:
	.word	.LC2
	.word	.LANCHOR1
	.fnend
	.size	get_new_wrap_line, .-get_new_wrap_line
	.align	1
	.global	vvi_node_get_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_get_status, %function
vvi_node_get_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r7, r1
	ldr	r3, [r1, #16]	@ unaligned
	.pad #20
	sub	sp, sp, #20
	cmp	r3, #136
	beq	.L16
	movw	r3, #431
	ldr	r2, .L22
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L22+4
	movs	r0, #8
	bl	vlog
.L21:
	mov	r0, #-1
	b	.L15
.L16:
	mov	r3, #434
	ldr	r2, .L22+8
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L22+4
	movs	r0, #8
	bl	vlog
	ldr	r2, [r7, #4]	@ unaligned
	ldr	r1, [r7]	@ unaligned
	movs	r0, #8
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L21
	ldr	r4, [r0, #60]
	movs	r1, #5
	ldr	r2, .L22+12
	movs	r0, #8
	ldr	r8, .L22+4
	movs	r6, #0
	ldr	r3, [r4]
	ldr	r9, .L22+20
	str	r3, [sp, #4]
	movw	r3, #439
	str	r3, [sp]
	ldr	r3, .L22+4
	bl	vlog
	ldr	r3, [r4, #76]!
	ldr	r5, [r3], #-36
	subs	r5, r5, #36
.L18:
	add	r2, r3, #36
	cmp	r2, r4
	bne	.L20
	movw	r3, #449
	ldr	r2, .L22+16
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L22+4
	movs	r0, #8
	bl	vlog
	movs	r0, #0
.L15:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L20:
	ldr	r2, [r3, #16]
	cmp	r6, #7
	it	le
	addle	r1, r7, r6, lsl #2
	mov	r0, #8
	itt	le
	addle	r6, r6, #1
	strle	r2, [r1, #20]	@ unaligned
	movs	r1, #5
	strd	r3, r2, [sp, #4]
	mov	r3, #446
	mov	r2, r9
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
	ldr	r2, [r5, #36]
	mov	r3, r5
	sub	r5, r2, #36
	b	.L18
.L23:
	.align	2
.L22:
	.word	.LC3
	.word	.LANCHOR1+18
	.word	.LC4
	.word	.LC5
	.word	.LC7
	.word	.LC6
	.fnend
	.size	vvi_node_get_status, .-vvi_node_get_status
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vi_poll, %function
vi_poll:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r2, r1
	ldr	r3, [r0, #128]
	.pad #20
	sub	sp, sp, #20
	ldrd	r4, r5, [r3, #4]
	cbz	r1, .L25
	ldr	r7, [r1]
	cbz	r7, .L25
	add	r3, r5, #76
	movs	r6, #6
	mla	r1, r6, r4, r3
	ldr	r3, .L38
	add	r1, r3, r1, lsl #3
	adds	r1, r1, #4
	blx	r7
.L25:
	movw	r3, #970
	ldr	r2, .L38+4
	str	r3, [sp]
	movs	r1, #4
	strd	r4, r5, [sp, #4]
	movs	r0, #8
	ldr	r3, .L38+8
	bl	vlog
	mov	r2, r5
	mov	r1, r4
	movs	r0, #8
	bl	vsys_get_node_from_map
	cbz	r0, .L26
	ldr	r3, [r0, #352]
	movs	r1, #4
	ldr	r2, .L38+12
	movs	r0, #8
	ldr	r4, [r3, #16]
	movw	r3, #974
	str	r3, [sp]
	ldr	r3, .L38+8
	str	r4, [sp, #4]
	bl	vlog
	cmp	r4, #0
	bgt	.L28
.L26:
	movs	r0, #0
.L24:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L28:
	movs	r0, #65
	b	.L24
.L39:
	.align	2
.L38:
	.word	.LANCHOR0
	.word	.LC8
	.word	.LANCHOR1+38
	.word	.LC9
	.fnend
	.size	vi_poll, .-vi_poll
	.align	1
	.global	vvi_node_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_stop, %function
vvi_node_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r5, r1
	ldr	r3, [r1, #28]	@ unaligned
	cmp	r3, #228
	beq	.L41
	mov	r3, #400
	ldr	r2, .L51
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L51+4
	movs	r0, #8
	bl	vlog
	mov	r0, #-1
.L40:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L41:
	ldr	r2, [r1, #8]	@ unaligned
	movs	r0, #8
	ldr	r1, [r1]	@ unaligned
	bl	vsys_get_node_from_map
	ldr	r6, .L51+8
	mov	r4, r0
	cbz	r0, .L43
	ldr	r3, [r5, #8]	@ unaligned
	movs	r1, #4
	ldr	r2, .L51+12
	movs	r0, #8
	str	r3, [sp, #8]
	ldr	r3, [r5]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #406
	str	r3, [sp]
	ldr	r3, .L51+4
	bl	vlog
	movs	r1, #1
	mov	r0, r4
	bl	mpi_node_flush
	movs	r3, #0
	str	r3, [r6, #4]
	movs	r0, #6
	ldr	r2, [r5]	@ unaligned
	ldr	r1, [r5, #8]	@ unaligned
	mla	r2, r0, r2, r1
	add	r2, r6, r2, lsl #2
	ldr	r2, [r2, #324]
	strb	r3, [r2, #228]
	strb	r3, [r2, #229]
	strb	r3, [r2, #230]
	strb	r3, [r2, #231]
.L43:
	ldr	r3, [r5, #4]	@ unaligned
	adds	r3, r3, #56
	add	r3, r6, r3, lsl #2
	ldr	r0, [r3, #4]
	cmp	r0, #0
	beq	.L40
	ldr	r4, [r5, #8]	@ unaligned
	cbnz	r4, .L44
	str	r4, [r3, #4]
	ldr	r3, [r5, #4]	@ unaligned
	adds	r3, r3, #60
	str	r4, [r6, r3, lsl #2]
	bl	mcu_wrap_end
	movs	r0, #10
	bl	msleep
.L44:
	movs	r0, #0
	b	.L40
.L52:
	.align	2
.L51:
	.word	.LC10
	.word	.LANCHOR1+46
	.word	.LANCHOR0
	.word	.LC11
	.fnend
	.size	vvi_node_stop, .-vvi_node_stop
	.align	1
	.global	vvi_mb_get
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_mb_get, %function
vvi_mb_get:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r1
	ldr	r3, [r1, #16]	@ unaligned
	.pad #36
	sub	sp, sp, #36
	cmp	r3, #136
	beq	.L54
	movw	r3, #489
	ldr	r2, .L64
	str	r3, [sp]
	ldr	r3, .L64+4
.L63:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
.L62:
	mov	r0, #-1
.L53:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L54:
	ldr	r1, [r1]	@ unaligned
	ldr	r6, .L64+8
	ldr	r2, [r4, #4]	@ unaligned
	add	r3, r6, r1, lsl #2
	ldr	r3, [r3, #228]
	cbz	r3, .L56
	cbnz	r2, .L56
	mov	r3, #494
	ldr	r2, .L64+12
	str	r3, [sp]
	ldr	r3, .L64+4
	b	.L63
.L56:
	movs	r0, #8
	bl	vsys_get_node_from_map
	mov	r7, r0
	cbnz	r0, .L57
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #2
	ldr	r2, .L64+16
	movs	r0, #8
	str	r3, [sp, #8]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #500
	str	r3, [sp]
	ldr	r3, .L64+4
	bl	vlog
	b	.L62
.L57:
	ldr	r3, [r4, #12]	@ unaligned
	movs	r1, #0
	str	r3, [r0, #384]
	bl	mpi_node_get_out_buffer
	mov	r5, r0
	cbnz	r0, .L58
	ldr	r3, [r4, #12]	@ unaligned
	movs	r1, #2
	ldr	r2, .L64+20
	movs	r0, #8
	str	r3, [sp, #4]
	movw	r3, #507
	str	r3, [sp]
	ldr	r3, .L64+4
	bl	vlog
	b	.L62
.L58:
	ldr	r2, [r4]	@ unaligned
	ldr	r1, [r7, #76]
	movs	r7, #6
	ldr	r3, [r4, #4]	@ unaligned
	muls	r7, r2, r7
	add	r0, r0, r1, lsl #3
	add	r0, r0, #308
	adds	r2, r7, r3
	adds	r3, r3, #58
	adds	r2, r2, #58
	add	r3, r3, r7
	add	r2, r6, r2, lsl #3
	add	r6, r6, r3, lsl #3
	adds	r6, r6, #4
	ldr	ip, [r2, #8]
	str	r0, [r2, #8]
	add	r2, r5, r1, lsl #3
	add	r1, r5, #616
	strd	r6, ip, [r2, #308]
	movs	r2, #80
	str	r0, [ip]
	add	r0, r4, #56
	ldr	r3, [r5, #448]
	ldr	r3, [r3]
	str	r3, [r4, #52]	@ unaligned
	bl	memcpy
	ldr	r1, [r4, #112]	@ unaligned
	ldr	r2, [r4, #116]	@ unaligned
	movs	r0, #8
	ldr	r3, [r5]
	str	r3, [r4, #8]	@ unaligned
	strd	r1, r2, [sp, #16]
	movs	r1, #5
	ldr	r2, [r4, #108]	@ unaligned
	str	r5, [sp, #4]
	strd	r3, r2, [sp, #8]
	movw	r3, #517
	ldr	r2, .L64+24
	str	r3, [sp]
	ldr	r3, .L64+4
	bl	vlog
	ldrd	r2, [r5, #672]
	movs	r1, #5
	movs	r0, #8
	strd	r2, [sp, #24]
	ldr	r3, [r5, #624]
	ldr	r2, .L64+28
	str	r3, [sp, #16]
	ldr	r3, [r5, #620]
	str	r3, [sp, #12]
	ldr	r3, [r5, #448]
	ldr	r3, [r3]
	str	r3, [sp, #8]
	ldr	r3, [r4, #8]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #521
	str	r3, [sp]
	ldr	r3, .L64+4
	bl	vlog
	movs	r0, #0
	b	.L53
.L65:
	.align	2
.L64:
	.word	.LC3
	.word	.LANCHOR1+60
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.fnend
	.size	vvi_mb_get, .-vvi_mb_get
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vi_release, %function
vi_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #44
	sub	sp, sp, #44
	ldr	r4, .L100
	str	r1, [sp, #32]
	add	r0, r4, #32
	bl	mpi_obj_unref
	ldr	r3, [r4, #48]
	cmp	r3, #1
	bne	.L67
	ldr	r2, [r4, #32]
	add	r9, r4, #228
	ldr	r1, .L100+4
	movs	r5, #0
	ldr	r0, .L100+8
	bl	printk
	add	r3, r4, #324
	strd	r3, r4, [sp, #16]
.L68:
	mvn	r3, #47
	ldrd	r8, fp, [sp, #16]
	muls	r3, r5, r3
	movs	r7, #0
	str	r3, [sp, #28]
	movs	r3, #48
	muls	r3, r5, r3
	add	r3, r3, #468
	str	r3, [sp, #24]
.L77:
	mov	r2, r7
	mov	r1, r5
	movs	r0, #8
	bl	vsys_get_node_from_map
	mov	r6, r0
	cmp	r0, #0
	bne	.L69
.L76:
	ldr	r0, [r8]
	cbz	r0, .L71
	bl	kfree
	movs	r3, #0
	str	r3, [r8]
.L71:
	adds	r7, r7, #1
	add	r8, r8, #4
	cmp	r7, #6
	add	fp, fp, #8
	bne	.L77
	movs	r4, #0
	str	r4, [r9], #4
	ldr	r0, [r9, #80]
	str	r4, [r9, #8]
	cbz	r0, .L78
	bl	kfree
	str	r4, [r9, #80]
.L78:
	ldr	r3, [sp, #16]
	adds	r5, r5, #1
	cmp	r5, #3
	add	r3, r3, #24
	str	r3, [sp, #16]
	ldr	r3, [sp, #20]
	add	r3, r3, #48
	str	r3, [sp, #20]
	bne	.L68
	ldr	r5, .L100+12
	movs	r4, #6
	movs	r6, #0
.L81:
	ldr	r0, [r5], #4
	cbz	r0, .L80
	bl	kfree
	str	r6, [r5, #-4]
.L80:
	subs	r4, r4, #1
	bne	.L81
.L67:
	ldr	r3, [sp, #32]
	ldr	r0, [r3, #128]
	cbz	r0, .L82
	bl	kvfree
.L82:
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L69:
	ldr	r4, [r0, #60]
	mov	r2, r7
	mov	r1, r5
	movs	r0, #8
	bl	vsys_remove_node_from_map
	mov	r0, r6
	bl	mpi_node_destroy
	cbz	r4, .L72
	movs	r1, #1
	mov	r0, r4
	bl	mpi_buf_pool_destroy
.L72:
	movs	r3, #0
	ldr	r2, .L100+16
	str	r3, [r8, #72]
	movs	r1, #4
	mov	r3, #904
	strd	r5, r7, [sp, #4]
	str	r3, [sp]
	movs	r0, #8
	ldr	r3, .L100+4
	mvn	r10, #43
	bl	vlog
	ldr	r2, [r6, #76]
	add	r3, r2, #33
	sub	r3, r10, r3, lsl #3
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #24]
	add	r1, fp, r1
	ldr	r4, [r1, r0]
	add	r4, r4, r3
	add	r2, r4, r2, lsl #3
	ldr	r2, [r2, #308]
	add	r10, r2, r3
	add	r3, fp, #468
	str	r3, [sp, #36]
.L73:
	ldr	r3, [r6, #76]
	ldr	r2, [sp, #36]
	add	r3, r4, r3, lsl #3
	add	r3, r3, #308
	cmp	r3, r2
	beq	.L76
	cbz	r4, .L74
	ldr	r3, [r4]
	movs	r1, #4
	ldr	r2, .L100+20
	movs	r0, #8
	str	r3, [sp, #4]
	movw	r3, #907
	str	r3, [sp]
	ldr	r3, .L100+4
	bl	vlog
	ldr	r3, [r6, #76]
	mov	r0, r4
	add	r3, r4, r3, lsl #3
	ldrd	r1, r2, [r3, #308]
	str	r2, [r1, #4]
	str	r1, [r2]
	mov	r1, #256
	mov	r2, #290
	strd	r1, r2, [r3, #308]
	bl	mpi_obj_unref
.L74:
	ldr	r3, [r6, #76]
	mvn	r2, #43
	mov	r4, r10
	add	r1, r3, #33
	add	r3, r10, r3, lsl #3
	sub	r2, r2, r1, lsl #3
	ldr	r3, [r3, #308]
	add	r10, r3, r2
	b	.L73
.L101:
	.align	2
.L100:
	.word	.LANCHOR0
	.word	.LANCHOR1+71
	.word	.LC17
	.word	.LANCHOR0+264
	.word	.LC18
	.word	.LC19
	.fnend
	.size	vi_release, .-vi_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vi_ioctl, %function
vi_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	uxtb	r4, r1
	mov	r6, r1
	ldr	r1, .L108
	mov	r3, r2
	ldr	r5, [r1, #28]
	cmp	r5, r4
	bls	.L103
	.syntax unified
@ 87 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
	cmp	r4, r5
	sbc	r5, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r1, #24]
	ands	r4, r4, r5
	movs	r5, #20
	mla	r1, r5, r4, r1
	cbz	r1, .L103
	mov	r2, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	mpi_ioctl_kernel
.L103:
	str	r4, [sp, #4]
	mov	r3, #856
	ldr	r2, .L108+4
	movs	r1, #2
	str	r3, [sp]
	movs	r0, #8
	ldr	r3, .L108+8
	bl	vlog
	mvn	r0, #21
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L109:
	.align	2
.L108:
	.word	.LANCHOR0
	.word	.LC20
	.word	.LANCHOR1+82
	.fnend
	.size	vi_ioctl, .-vi_ioctl
	.align	1
	.global	vvi_node_get_chn_fr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_get_chn_fr, %function
vvi_node_get_chn_fr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r1, #8]	@ unaligned
	ldr	r5, [r1]	@ unaligned
	cmp	r3, #20
	ldr	r6, [r1, #4]	@ unaligned
	beq	.L111
	mov	r3, #796
	ldr	r2, .L114
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L114+4
	movs	r0, #8
	bl	vlog
.L113:
	mov	r0, #-1
	b	.L110
.L111:
	mov	r2, r6
	mov	r1, r5
	movs	r0, #8
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L113
	movs	r3, #6
	movs	r0, #0
	mla	r5, r3, r5, r6
	ldr	r3, .L114+8
	add	r5, r3, r5, lsl #2
	ldr	r3, [r5, #324]
	ldr	r2, [r3, #128]	@ unaligned
	str	r2, [r4, #12]	@ unaligned
	ldr	r3, [r3, #132]	@ unaligned
	str	r3, [r4, #16]	@ unaligned
.L110:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L115:
	.align	2
.L114:
	.word	.LC21
	.word	.LANCHOR1+91
	.word	.LANCHOR0
	.fnend
	.size	vvi_node_get_chn_fr, .-vvi_node_get_chn_fr
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vi_open, %function
vi_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r2, #-1
	mov	r4, r1
	movs	r0, #12
	mov	r1, #3520
	bl	kvmalloc_node
	str	r0, [r4, #128]
	cbnz	r0, .L117
	mov	r3, #872
	ldr	r2, .L119
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L119+4
	movs	r0, #8
	bl	vlog
	mov	r0, #-1
.L116:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L117:
	ldr	r0, .L119+8
	movs	r3, #17
	adds	r0, r0, #32
	str	r3, [r0, #-4]
	bl	mpi_obj_ref
	movw	r3, #878
	ldr	r2, .L119+12
	movs	r1, #4
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L119+4
	bl	vlog
	movs	r0, #0
	b	.L116
.L120:
	.align	2
.L119:
	.word	.LC22
	.word	.LANCHOR1+111
	.word	.LANCHOR0
	.word	.LC23
	.fnend
	.size	vi_open, .-vi_open
	.align	1
	.global	vvi_change_resolution
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_change_resolution, %function
vvi_change_resolution:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r4, r1
	ldr	r3, .L135
	mov	r1, #3520
	ldr	r0, [r3, #24]
	bl	kmem_cache_alloc
	mov	r5, r0
	cbnz	r0, .L122
	movw	r3, #535
	ldr	r2, .L135+4
	str	r3, [sp]
	ldr	r3, .L135+8
.L134:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
.L127:
	mov	r0, #-1
	b	.L121
.L122:
	ldr	r3, [r4, #16]	@ unaligned
	cmp	r3, #33
	beq	.L124
	mov	r3, #540
	ldr	r2, .L135+12
	str	r3, [sp]
	ldr	r3, .L135+8
	b	.L134
.L124:
	ldr	r2, [r4, #4]	@ unaligned
	movs	r0, #8
	ldr	r1, [r4]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r8, r0
	cmp	r0, #0
	beq	.L127
	ldr	r3, [r4]	@ unaligned
	ldr	r6, .L135+16
	adds	r3, r3, #60
	ldr	r7, [r6, r3, lsl #2]
	ldr	r3, [r4, #20]	@ unaligned
	str	r3, [r5, #28]
	ldr	r2, [r4, #24]	@ unaligned
	str	r2, [r5, #32]
	ldr	r1, [r0, #56]
	movs	r0, #8
	str	r3, [sp, #4]
	mov	r3, #552
	str	r3, [sp]
	strd	r2, r1, [sp, #8]
	movs	r1, #5
	ldr	r3, .L135+8
	ldr	r2, .L135+20
	bl	vlog
	add	r3, r6, #228
	ldr	r2, .L135+24
	movs	r1, #4
	movs	r0, #8
	str	r3, [sp, #4]
	movw	r3, #553
	str	r3, [sp]
	ldr	r3, .L135+8
	bl	vlog
	ldr	r3, [r4]	@ unaligned
	add	r2, r6, r3, lsl #2
	ldr	r2, [r2, #228]
	cbz	r2, .L125
	ldr	r2, [r4, #4]	@ unaligned
	cbnz	r2, .L125
	add	r3, r6, r3, lsl #2
	ldr	r2, [r4, #8]	@ unaligned
	ldrd	r0, r1, [r5, #28]
	ldr	r3, [r3, #252]
	bl	get_new_wrap_line
	mov	r7, r0
	bl	mcu_wrap_end
	movs	r0, #10
	bl	msleep
.L125:
	movs	r3, #2
	str	r7, [r5, #36]
	str	r3, [r5]
	mov	r1, r5
	mov	r0, r8
	bl	mpi_node_set_params
	ldr	r3, [r4]	@ unaligned
	add	r3, r6, r3, lsl #2
	ldr	r3, [r3, #228]
	cbz	r3, .L126
	ldr	r3, [r4, #4]	@ unaligned
	cbnz	r3, .L126
	mov	r0, r7
	bl	mcu_set_wrap_line
	mov	r1, r7
	ldr	r0, [r5, #32]
	bl	mcu_wrap_config
	ldr	r3, [r4]	@ unaligned
	adds	r3, r3, #60
	str	r7, [r6, r3, lsl #2]
.L126:
	movw	r3, #569
	ldr	r2, .L135+28
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L135+8
	movs	r0, #8
	bl	vlog
	movs	r0, #0
.L121:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L136:
	.align	2
.L135:
	.word	kmalloc_caches
	.word	.LC24
	.word	.LANCHOR1+119
	.word	.LC25
	.word	.LANCHOR0
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.fnend
	.size	vvi_change_resolution, .-vvi_change_resolution
	.align	1
	.global	vvi_node_set_pipe_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_set_pipe_status, %function
vvi_node_set_pipe_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r6, r1
	ldr	r3, [r1, #232]	@ unaligned
	ldr	r4, [r1, #4]	@ unaligned
	cmp	r3, #248
	beq	.L138
	movw	r3, #723
	ldr	r2, .L143
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L143+4
	movs	r0, #8
	bl	vlog
.L142:
	mov	r0, #-1
.L137:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L138:
	cmp	r4, #2
	bls	.L140
	movs	r3, #3
	ldr	r2, .L143+8
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	mov	r3, #728
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L143+4
	bl	vlog
	b	.L142
.L140:
	ldr	r5, .L143+12
	adds	r4, r4, #78
	ldr	r3, [r5, r4, lsl #2]
	cbnz	r3, .L141
	ldr	r3, .L143+16
	mov	r1, #3520
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	str	r0, [r5, r4, lsl #2]
.L141:
	ldr	r3, [r5, r4, lsl #2]
	movs	r0, #0
	ldr	r2, [r6, #236]	@ unaligned
	str	r2, [r3, #236]	@ unaligned
	b	.L137
.L144:
	.align	2
.L143:
	.word	.LC29
	.word	.LANCHOR1+141
	.word	.LC30
	.word	.LANCHOR0
	.word	kmalloc_caches
	.fnend
	.size	vvi_node_set_pipe_status, .-vvi_node_set_pipe_status
	.align	1
	.global	vvi_node_get_pipe_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_get_pipe_status, %function
vvi_node_get_pipe_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r6, r1
	ldr	r3, [r1, #232]	@ unaligned
	ldr	r4, [r1, #4]	@ unaligned
	cmp	r3, #248
	beq	.L146
	movw	r3, #743
	ldr	r2, .L151
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L151+4
	movs	r0, #8
	bl	vlog
.L150:
	mov	r0, #-1
.L145:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L146:
	cmp	r4, #2
	bls	.L148
	movs	r3, #3
	ldr	r2, .L151+8
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	mov	r3, #748
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L151+4
	bl	vlog
	b	.L150
.L148:
	ldr	r5, .L151+12
	adds	r4, r4, #78
	ldr	r3, [r5, r4, lsl #2]
	cbnz	r3, .L149
	ldr	r3, .L151+16
	mov	r1, #3520
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	str	r0, [r5, r4, lsl #2]
.L149:
	ldr	r3, [r5, r4, lsl #2]
	movs	r0, #0
	ldr	r3, [r3, #236]	@ unaligned
	str	r3, [r6, #236]	@ unaligned
	b	.L145
.L152:
	.align	2
.L151:
	.word	.LC29
	.word	.LANCHOR1+166
	.word	.LC30
	.word	.LANCHOR0
	.word	kmalloc_caches
	.fnend
	.size	vvi_node_get_pipe_status, .-vvi_node_get_pipe_status
	.align	1
	.global	vvi_node_delete
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_delete, %function
vvi_node_delete:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r10, r1
	ldr	r3, [r1, #28]	@ unaligned
	.pad #20
	sub	sp, sp, #20
	ldr	r4, [r1]	@ unaligned
	cmp	r3, #228
	ldr	r9, [r1, #8]	@ unaligned
	beq	.L154
	movs	r3, #228
	ldr	r2, .L172
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L172+4
	movs	r0, #8
	bl	vlog
	mov	r0, #-1
.L153:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L154:
	mov	r2, r9
	mov	r1, r4
	movs	r0, #8
	ldr	r6, .L172+8
	bl	vsys_get_node_from_map
	mov	r7, r0
	cmp	r0, #0
	beq	.L156
	movs	r3, #6
	add	r8, r9, #58
	muls	r3, r4, r3
	add	r8, r8, r3
	add	r3, r3, r9
	adds	r3, r3, #58
	add	r8, r6, r8, lsl #3
	add	r3, r6, r3, lsl #3
	add	r8, r8, #4
	ldr	r3, [r3, #4]
	cmp	r8, r3
	bne	.L157
	movs	r3, #235
	strd	r4, r9, [sp, #4]
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L172+4
	movs	r0, #8
	ldr	r2, .L172+12
	bl	vlog
.L157:
	ldr	r1, [r7, #76]
	mvn	fp, #43
	add	r3, r1, #33
	sub	r2, fp, r3, lsl #3
	movs	r3, #6
	mla	r3, r3, r4, r9
	add	r3, r6, r3, lsl #3
	ldr	r5, [r3, #468]
	add	r5, r5, r2
	add	r1, r5, r1, lsl #3
	ldr	r3, [r1, #308]
.L171:
	add	fp, r3, r2
	ldr	r3, [r7, #76]
	add	r3, r5, r3, lsl #3
	add	r3, r3, #308
	cmp	r8, r3
	bne	.L160
	ldr	r5, [r7, #60]
	mov	r0, r7
	bl	mpi_node_del_out_watcher
	ldr	r2, [r10, #8]	@ unaligned
	ldr	r1, [r10]	@ unaligned
	movs	r0, #8
	bl	vsys_remove_node_from_map
	mov	r0, r7
	bl	mpi_node_destroy
	cbz	r5, .L161
	movs	r1, #1
	mov	r0, r5
	bl	mpi_buf_pool_destroy
	ldr	r2, [r10, #8]	@ unaligned
	ldr	r3, [r10]	@ unaligned
	movs	r1, #6
	mla	r3, r1, r3, r2
	movs	r2, #0
	adds	r3, r3, #98
	add	r3, r6, r3, lsl #2
	str	r2, [r3, #4]
.L161:
	movs	r3, #253
	strd	r4, r9, [sp, #4]
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L172+4
	movs	r0, #8
	ldr	r2, .L172+16
	bl	vlog
.L156:
	movs	r3, #6
	movs	r2, #248
	mla	r4, r3, r4, r9
	movs	r1, #0
	adds	r4, r4, #80
	add	r4, r6, r4, lsl #2
	ldr	r0, [r4, #4]
	ldr	r5, [r0, #236]	@ unaligned
	bl	memset
	ldr	r3, [r4, #4]
	movs	r1, #5
	ldr	r2, .L172+20
	movs	r0, #8
	str	r5, [r3, #236]	@ unaligned
	movw	r3, #263
	str	r3, [sp]
	ldr	r3, .L172+4
	bl	vlog
	movs	r0, #0
	b	.L153
.L160:
	cbz	r5, .L159
	ldr	r3, [r5]
	movs	r1, #4
	ldr	r2, .L172+24
	movs	r0, #8
	str	r3, [sp, #4]
	movs	r3, #239
	str	r3, [sp]
	ldr	r3, .L172+4
	bl	vlog
	ldr	r3, [r7, #76]
	mov	r0, r5
	add	r3, r5, r3, lsl #3
	ldrd	r1, r2, [r3, #308]
	str	r2, [r1, #4]
	str	r1, [r2]
	mov	r1, #256
	mov	r2, #290
	strd	r1, r2, [r3, #308]
	bl	mpi_obj_unref
.L159:
	ldr	r3, [r7, #76]
	mvn	r2, #43
	mov	r5, fp
	add	r1, r3, #33
	add	r3, fp, r3, lsl #3
	sub	r2, r2, r1, lsl #3
	ldr	r3, [r3, #308]
	b	.L171
.L173:
	.align	2
.L172:
	.word	.LC10
	.word	.LANCHOR1+191
	.word	.LANCHOR0
	.word	.LC31
	.word	.LC18
	.word	.LC32
	.word	.LC19
	.fnend
	.size	vvi_node_delete, .-vvi_node_delete
	.align	1
	.global	vvi_node_set_chn_fr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_set_chn_fr, %function
vvi_node_set_chn_fr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r5, r1
	ldr	r3, [r1, #8]	@ unaligned
	ldr	r6, [r1]	@ unaligned
	cmp	r3, #20
	ldr	r8, [r1, #4]	@ unaligned
	beq	.L175
	movw	r3, #767
	ldr	r2, .L180
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L180+4
	movs	r0, #8
	bl	vlog
.L174:
	mov	r0, #-1
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L175:
	mov	r2, r8
	mov	r1, r6
	movs	r0, #8
	bl	vsys_get_node_from_map
	mov	r7, r0
	cmp	r0, #0
	beq	.L174
	movs	r4, #6
	ldr	r3, .L180+8
	mla	r4, r4, r6, r8
	mov	r9, r3
	adds	r4, r4, #80
	add	r4, r3, r4, lsl #2
	ldr	r2, [r4, #4]
	cbnz	r2, .L177
	ldr	r3, .L180+12
	mov	r1, #3520
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	str	r0, [r4, #4]
.L177:
	movs	r3, #6
	ldr	r2, [r5, #12]	@ unaligned
	mla	r6, r3, r6, r8
	add	r3, r9, r6, lsl #2
	ldr	r3, [r3, #324]
	str	r2, [r3, #128]	@ unaligned
	add	r2, r7, #212
	ldr	r1, [r5, #16]	@ unaligned
	str	r1, [r3, #132]	@ unaligned
	add	r3, r7, #204
	ldr	r0, [r5, #12]	@ unaligned
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, lr}
	b	mpi_node_set_frame_rate
.L181:
	.align	2
.L180:
	.word	.LC21
	.word	.LANCHOR1+207
	.word	.LANCHOR0
	.word	kmalloc_caches
	.fnend
	.size	vvi_node_set_chn_fr, .-vvi_node_set_chn_fr
	.align	1
	.global	vvi_node_get_chns_attr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_get_chns_attr, %function
vvi_node_get_chns_attr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r7, r1
	ldr	r3, [r1, #232]	@ unaligned
	ldr	r6, [r1]	@ unaligned
	cmp	r3, #248
	ldr	r8, [r1, #8]	@ unaligned
	beq	.L183
	movw	r3, #609
	ldr	r2, .L190
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L190+4
	movs	r0, #8
	bl	vlog
.L189:
	mov	r0, #-1
.L182:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L183:
	cmp	r6, #2
	bls	.L185
	movs	r3, #3
	ldr	r2, .L190+8
	strd	r6, r3, [sp, #4]
	movw	r3, #613
	str	r3, [sp]
	ldr	r3, .L190+4
.L188:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
	b	.L189
.L185:
	cmp	r8, #5
	mov	r4, #6
	bls	.L186
	movw	r3, #617
	strd	r8, r4, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L190+12
	ldr	r3, .L190+4
	b	.L188
.L186:
	mla	r4, r4, r6, r8
	ldr	r5, .L190+16
	adds	r4, r4, #80
	add	r4, r5, r4, lsl #2
	ldr	r2, [r4, #4]
	cbnz	r2, .L187
	ldr	r3, .L190+20
	mov	r1, #3520
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	str	r0, [r4, #4]
.L187:
	movs	r3, #6
	movs	r2, #248
	mla	r3, r3, r6, r8
	mov	r0, r7
	add	r3, r5, r3, lsl #2
	ldr	r1, [r3, #324]
	bl	memcpy
	movs	r0, #0
	b	.L182
.L191:
	.align	2
.L190:
	.word	.LC29
	.word	.LANCHOR1+227
	.word	.LC33
	.word	.LC34
	.word	.LANCHOR0
	.word	kmalloc_caches
	.fnend
	.size	vvi_node_get_chns_attr, .-vvi_node_get_chns_attr
	.align	1
	.global	vvi_node_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_create, %function
vvi_node_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	ldr	r3, [r1, #28]	@ unaligned
	.pad #36
	sub	sp, sp, #36
	ldr	r7, [r0, #128]
	cmp	r3, #228
	beq	.L193
	movs	r3, #116
	ldr	r2, .L225
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L225+4
	movs	r0, #8
	bl	vlog
.L224:
	mov	r0, #-1
	b	.L192
.L193:
	ldr	r0, .L225+8
	bl	mpi_allocator_find_by_name
	mov	r10, r0
	cbnz	r0, .L195
	movs	r3, #121
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L225+4
	movs	r0, #8
	ldr	r2, .L225+12
	bl	vlog
	b	.L224
.L195:
	ldr	r6, [r4, #8]	@ unaligned
	movs	r1, #4
	ldr	r5, [r4, #4]	@ unaligned
	movs	r0, #8
	ldr	r3, [r4, #136]	@ unaligned
	ldr	r2, .L225+16
	strd	r5, r5, [r7]
	str	r6, [r7, #8]
	str	r3, [sp, #24]
	movs	r3, #133
	strd	r5, r6, [sp, #4]
	str	r3, [sp]
	ldr	r3, .L225+4
	bl	vlog
	mov	r2, r6
	mov	r1, r5
	movs	r0, #8
	bl	vsys_get_node_from_map
	mov	r7, r0
	cbz	r0, .L196
	movs	r3, #137
	str	r6, [sp, #4]
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L225+4
	movs	r0, #8
	ldr	r2, .L225+20
	bl	vlog
	mvn	r0, #1
.L192:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L196:
	add	fp, r4, #60
	ldr	r8, .L225+64
	mov	r0, fp
	bl	mpi_dev_find_by_name
	add	r3, r5, #48
	ldr	r2, .L225+24
	movs	r1, #4
	add	r3, r8, r3, lsl #2
	str	r0, [r3, #4]
	str	r3, [sp, #28]
	movs	r3, #142
	strd	r0, fp, [sp, #8]
	movs	r0, #8
	str	r3, [sp]
	str	r5, [sp, #4]
	ldr	r3, .L225+4
	bl	vlog
	ldr	r3, .L225+28
	mov	r1, #3520
	ldr	r0, [r3, #24]
	bl	kmem_cache_alloc
	movs	r2, #44
	mov	r1, r7
	mov	r9, r0
	bl	memset
	ldr	ip, [r4, #24]	@ unaligned
	str	ip, [r9, #28]
	ldr	r2, [r4, #32]	@ unaligned
	str	r2, [r9, #32]
	ldr	r1, [r4, #12]	@ unaligned
	str	r1, [r9, #4]
	ldr	r0, [r4, #16]	@ unaligned
	str	r0, [r9, #8]
	ldr	r3, [sp, #24]
	strd	r2, r1, [sp, #8]
	movs	r1, #4
	str	ip, [sp, #4]
	strd	r0, r3, [sp, #16]
	movs	r3, #150
	ldr	r2, .L225+32
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L225+4
	bl	vlog
	ldr	r3, [r4, #100]	@ unaligned
	str	r3, [r9, #40]
	ldr	r3, [sp, #28]
	ldr	r0, [r3, #4]
	cbnz	r0, .L197
.L198:
	movs	r3, #155
	strd	r6, fp, [sp, #8]
	str	r5, [sp, #4]
	movs	r1, #2
	str	r3, [sp]
	movs	r0, #8
	ldr	r3, .L225+4
	ldr	r2, .L225+36
	bl	vlog
	b	.L224
.L197:
	mov	r3, r9
	mov	r2, r7
	mov	r1, r6
	bl	mpi_dev_create_node_with_params
	mov	r9, r0
	cmp	r0, #0
	beq	.L198
	movs	r3, #158
	ldr	r2, .L225+40
	str	r3, [sp]
	movs	r1, #4
	strd	r5, r6, [sp, #4]
	movs	r0, #8
	ldr	r3, .L225+4
	bl	vlog
	ldr	r0, .L225+44
	str	r0, [r9, #4]
	ldr	r2, [r4, #40]	@ unaligned
	cmp	r2, #0
	beq	.L199
	ldr	r1, [r4, #44]	@ unaligned
	cmp	r1, #1
	bne	.L200
	ldr	r3, [sp, #24]
	cmp	r3, #2
	bne	.L201
	ldr	r2, [r4, #224]	@ unaligned
	ldr	r3, [r4, #220]	@ unaligned
	adds	r1, r2, #1
	bne	.L202
	adds	r3, r3, #66
	ldr	r3, [r8, r3, lsl #2]
	ldr	r0, [r3, #48]	@ unaligned
	bl	vsys_get_pool
	ldr	r1, .L225+48
.L222:
	bl	mpi_buf_pool_dup
	mov	r7, r0
.L203:
	ldr	r3, [r4, #36]	@ unaligned
	movs	r1, #4
	ldr	r2, .L225+52
	movs	r0, #8
	str	r3, [sp, #8]
	ldr	r3, [r4, #8]	@ unaligned
	str	r3, [sp, #4]
	movs	r3, #180
	str	r3, [sp]
	ldr	r3, .L225+4
	bl	vlog
	cmp	r7, #0
	bne	.L206
.L208:
	ldr	r3, [r4, #8]	@ unaligned
	movs	r1, #2
	ldr	r2, .L225+56
	movs	r0, #8
	str	r3, [sp, #4]
	movs	r3, #193
	str	r3, [sp]
	ldr	r3, .L225+4
	bl	vlog
	mov	r0, r9
	bl	mpi_node_destroy
	b	.L224
.L202:
	movs	r1, #6
	mla	r3, r1, r3, r2
	ldr	r1, .L225+48
	add	r8, r8, r3, lsl #2
	ldr	r0, [r8, #396]
	b	.L222
.L201:
	ldr	r3, [r4, #36]	@ unaligned
	cmp	r3, #1
	itttt	eq
	strbeq	r7, [r4, #37]
	strbeq	r7, [r4, #38]
	moveq	r3, #4
	strbeq	r7, [r4, #39]
	itte	eq
	strbeq	r3, [r4, #36]
	streq	r0, [sp]
	strne	r0, [sp]
	mov	r0, r10
	it	eq
	ldreq	r1, .L225+60
	bl	mpi_buf_pool_create
	movs	r3, #6
	mla	r5, r3, r5, r6
	ldr	r3, .L225+64
	add	r5, r3, r5, lsl #2
	mov	r7, r0
	str	r0, [r5, #396]
	b	.L203
.L206:
	mov	r1, r9
	mov	r0, r7
.L223:
	bl	mpi_buf_pool_add_user
.L199:
	add	r3, r9, #204
	add	r2, r9, #212
	ldr	r1, [r4, #132]	@ unaligned
	ldr	r0, [r4, #128]	@ unaligned
	bl	mpi_node_set_frame_rate
	ldr	r3, [r4, #20]	@ unaligned
	ldr	r2, .L225+68
	movs	r1, #4
	movs	r0, #8
	strd	r6, r3, [sp, #4]
	movs	r3, #204
	str	r3, [sp]
	ldr	r3, .L225+4
	bl	vlog
	ldr	r1, [r4, #20]	@ unaligned
	mov	r0, r9
	bl	mpi_node_add_out_watcher
	mov	r3, r9
	ldr	r2, [r4, #8]	@ unaligned
	movs	r0, #8
	ldr	r1, [r4]	@ unaligned
	bl	vsys_add_node_to_map
	movs	r3, #209
	ldr	r2, .L225+72
	movs	r1, #4
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L225+4
	bl	vlog
	movs	r0, #0
	b	.L192
.L200:
	cmp	r1, #0
	bne	.L208
	ldr	r0, [r4, #48]	@ unaligned
	bl	vsys_get_pool
	ldr	r3, [r4, #48]	@ unaligned
	ldr	r2, .L225+76
	movs	r1, #4
	mov	r5, r0
	str	r3, [sp, #4]
	movs	r0, #8
	movs	r3, #187
	str	r3, [sp]
	ldr	r3, .L225+4
	bl	vlog
	cmp	r5, #0
	beq	.L208
	mov	r1, r9
	mov	r0, r5
	b	.L223
.L226:
	.align	2
.L225:
	.word	.LC10
	.word	.LANCHOR1+250
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	kmalloc_caches
	.word	.LC40
	.word	.LC41
	.word	.LC42
	.word	.LC43
	.word	.LC44
	.word	.LC45
	.word	.LC46
	.word	-2147483647
	.word	.LANCHOR0
	.word	.LC48
	.word	.LC49
	.word	.LC47
	.fnend
	.size	vvi_node_create, .-vvi_node_create
	.align	1
	.global	vvi_node_set_chns_attr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_set_chns_attr, %function
vvi_node_set_chns_attr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r6, r1
	ldr	r3, [r1, #232]	@ unaligned
	ldr	r5, [r1]	@ unaligned
	cmp	r3, #248
	ldr	r8, [r1, #8]	@ unaligned
	beq	.L228
	movw	r3, #581
	ldr	r2, .L235
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L235+4
	movs	r0, #8
	bl	vlog
.L234:
	mov	r0, #-1
.L227:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L228:
	cmp	r5, #2
	bls	.L230
	movs	r3, #3
	ldr	r2, .L235+8
	strd	r5, r3, [sp, #4]
	movw	r3, #585
	str	r3, [sp]
	ldr	r3, .L235+4
.L233:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
	b	.L234
.L230:
	cmp	r8, #5
	mov	r4, #6
	bls	.L231
	movw	r3, #589
	strd	r8, r4, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L235+12
	ldr	r3, .L235+4
	b	.L233
.L231:
	mla	r4, r4, r5, r8
	ldr	r7, .L235+16
	adds	r4, r4, #80
	add	r4, r7, r4, lsl #2
	ldr	r2, [r4, #4]
	cbnz	r2, .L232
	ldr	r3, .L235+20
	mov	r1, #3520
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	str	r0, [r4, #4]
.L232:
	movs	r3, #6
	movs	r2, #248
	mla	r5, r3, r5, r8
	mov	r1, r6
	adds	r5, r5, #80
	add	r5, r7, r5, lsl #2
	ldr	r0, [r5, #4]
	ldr	r4, [r0, #236]	@ unaligned
	bl	memcpy
	ldr	r3, [r5, #4]
	movs	r0, #0
	str	r4, [r3, #236]	@ unaligned
	b	.L227
.L236:
	.align	2
.L235:
	.word	.LC29
	.word	.LANCHOR1+266
	.word	.LC33
	.word	.LC34
	.word	.LANCHOR0
	.word	kmalloc_caches
	.fnend
	.size	vvi_node_set_chns_attr, .-vvi_node_set_chns_attr
	.align	1
	.global	vvi_mb_put
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_mb_put, %function
vvi_mb_put:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r5, r1
	ldr	r3, [r1, #16]	@ unaligned
	cmp	r3, #136
	beq	.L238
	movw	r3, #463
	ldr	r2, .L242
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L242+4
	movs	r0, #8
	bl	vlog
.L240:
	mov	r0, #-1
	b	.L237
.L238:
	ldr	r2, [r1, #4]	@ unaligned
	movs	r0, #8
	ldr	r1, [r1]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r6, r0
	cmp	r0, #0
	beq	.L240
	movs	r1, #4
	ldr	r0, [r5, #8]	@ unaligned
	bl	mpi_obj_from_id
	ldr	r2, .L242+8
	movs	r1, #5
	ldr	r3, [r0, #448]
	mov	r4, r0
	strd	r0, r3, [sp, #8]
	movs	r0, #8
	ldr	r3, [r5, #8]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #472
	str	r3, [sp]
	ldr	r3, .L242+4
	bl	vlog
	cbz	r4, .L241
	ldr	r3, [r6, #76]
	movs	r0, #8
	add	r3, r4, r3, lsl #3
	ldrd	r1, r2, [r3, #308]
	str	r2, [r1, #4]
	str	r1, [r2]
	mov	r1, #256
	mov	r2, #290
	strd	r1, r2, [r3, #308]
	movw	r3, #475
	ldr	r2, .L242+12
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L242+4
	bl	vlog
	mov	r0, r6
	add	r2, r6, #400
	mov	r1, r4
	bl	mpi_node_release_buffer_flag
	movs	r0, #0
.L237:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L241:
	mov	r0, r4
	b	.L237
.L243:
	.align	2
.L242:
	.word	.LC3
	.word	.LANCHOR1+289
	.word	.LC50
	.word	.LC51
	.fnend
	.size	vvi_mb_put, .-vvi_mb_put
	.align	1
	.global	vvi_node_set_dev_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_set_dev_status, %function
vvi_node_set_dev_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r5, r1
	ldr	r3, [r1, #232]	@ unaligned
	ldr	r4, [r1]	@ unaligned
	cmp	r3, #248
	beq	.L245
	movw	r3, #634
	ldr	r2, .L256
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L256+4
	movs	r0, #8
	bl	vlog
.L254:
	mov	r4, #-1
	b	.L244
.L245:
	adds	r3, r4, #1
	bne	.L247
	ldr	r7, [r1, #4]	@ unaligned
	cmp	r7, #2
	bls	.L248
	movs	r3, #3
	strd	r7, r3, [sp, #4]
	mov	r3, #640
.L255:
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L256+4
	movs	r0, #8
	ldr	r2, .L256+8
	bl	vlog
.L244:
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L248:
	ldr	r8, [r1, #8]	@ unaligned
	movs	r2, #6
	cmp	r8, #5
	bls	.L249
	strd	r7, r2, [sp, #4]
	movw	r3, #645
	b	.L255
.L249:
	mla	r4, r2, r7, r8
	ldr	r6, .L256+12
	adds	r4, r4, #80
	add	r4, r6, r4, lsl #2
	ldr	r2, [r4, #4]
	cbnz	r2, .L250
	ldr	r3, .L256+16
	mov	r1, #3520
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	str	r0, [r4, #4]
.L250:
	movs	r3, #6
	ldr	r1, [r5, #236]	@ unaligned
	mla	r3, r3, r7, r8
	adds	r3, r3, #80
	add	r3, r6, r3, lsl #2
	ldr	r2, [r3, #4]
	str	r1, [r2, #236]	@ unaligned
	ldr	r3, [r3, #4]
	ldr	r2, [r5, #228]	@ unaligned
	str	r2, [r3, #228]	@ unaligned
.L253:
	movs	r4, #0
	b	.L244
.L247:
	cmp	r4, #2
	bls	.L251
	movs	r3, #3
	ldr	r2, .L256+20
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #654
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L256+4
	bl	vlog
	b	.L254
.L251:
	ldr	r6, .L256+12
	adds	r4, r4, #66
	ldr	r3, [r6, r4, lsl #2]
	cbnz	r3, .L252
	ldr	r3, .L256+16
	mov	r1, #3520
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	str	r0, [r6, r4, lsl #2]
.L252:
	ldr	r3, [r6, r4, lsl #2]
	ldr	r2, [r5, #236]	@ unaligned
	str	r2, [r3, #236]	@ unaligned
	b	.L253
.L257:
	.align	2
.L256:
	.word	.LC29
	.word	.LANCHOR1+300
	.word	.LC30
	.word	.LANCHOR0
	.word	kmalloc_caches
	.word	.LC33
	.fnend
	.size	vvi_node_set_dev_status, .-vvi_node_set_dev_status
	.align	1
	.global	vvi_node_get_dev_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_get_dev_status, %function
vvi_node_get_dev_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r6, r1
	ldr	r3, [r1, #232]	@ unaligned
	.pad #20
	sub	sp, sp, #20
	ldr	r4, [r1]	@ unaligned
	cmp	r3, #248
	ldr	r9, [r1, #4]	@ unaligned
	ldr	r7, [r1, #8]	@ unaligned
	beq	.L259
	mov	r3, #672
	ldr	r2, .L280
	str	r3, [sp]
	ldr	r3, .L280+4
.L279:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
.L277:
	mov	r4, #-1
	b	.L258
.L259:
	ldr	r5, .L280+8
	ldr	r2, [r5, #756]
	cbnz	r2, .L272
.L262:
	adds	r3, r4, #1
	bne	.L266
	cmp	r9, #2
	bls	.L267
	movs	r3, #3
	strd	r9, r3, [sp, #4]
	movw	r3, #693
.L278:
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L280+4
	movs	r0, #8
	ldr	r2, .L280+12
	bl	vlog
.L258:
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L264:
	bl	mcu_get_wrap_status
	cbnz	r0, .L263
	movs	r0, #10
	bl	msleep
.L261:
	adds	r8, r8, #-1
	bcs	.L264
.L265:
	movw	r3, #686
	ldr	r2, .L280+16
	str	r3, [sp]
	ldr	r3, .L280+4
	b	.L279
.L272:
	mov	r8, #50
	b	.L261
.L263:
	cmp	r8, #0
	bne	.L262
	b	.L265
.L267:
	cmp	r7, #5
	mov	r2, #6
	bls	.L268
	strd	r9, r2, [sp, #4]
	movw	r3, #698
	b	.L278
.L268:
	mla	r4, r2, r9, r7
	adds	r4, r4, #80
	add	r4, r5, r4, lsl #2
	ldr	r3, [r4, #4]
	cbnz	r3, .L269
	ldr	r3, .L280+20
	mov	r1, #3520
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	str	r0, [r4, #4]
.L269:
	movs	r3, #6
	mla	r3, r3, r9, r7
	adds	r3, r3, #80
	add	r3, r5, r3, lsl #2
	ldr	r2, [r3, #4]
	ldr	r2, [r2, #236]	@ unaligned
	str	r2, [r6, #236]	@ unaligned
	ldr	r3, [r3, #4]
	ldr	r3, [r3, #228]	@ unaligned
	str	r3, [r6, #228]	@ unaligned
.L276:
	movs	r4, #0
	b	.L258
.L266:
	cmp	r4, #2
	bls	.L270
	movs	r3, #3
	ldr	r2, .L280+24
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #707
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L280+4
	bl	vlog
	b	.L277
.L270:
	adds	r4, r4, #66
	ldr	r3, [r5, r4, lsl #2]
	cbnz	r3, .L271
	ldr	r3, .L280+20
	mov	r1, #3520
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	str	r0, [r5, r4, lsl #2]
.L271:
	ldr	r3, [r5, r4, lsl #2]
	ldr	r3, [r3, #236]	@ unaligned
	str	r3, [r6, #236]	@ unaligned
	b	.L276
.L281:
	.align	2
.L280:
	.word	.LC29
	.word	.LANCHOR1+324
	.word	.LANCHOR0
	.word	.LC30
	.word	.LC52
	.word	kmalloc_caches
	.word	.LC33
	.fnend
	.size	vvi_node_get_dev_status, .-vvi_node_get_dev_status
	.align	1
	.global	vvi_node_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_start, %function
vvi_node_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	ldr	r3, [r1, #28]	@ unaligned
	.pad #60
	sub	sp, sp, #60
	cmp	r3, #228
	beq	.L283
	mov	r3, #310
	ldr	r2, .L313
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L313+4
	movs	r0, #8
	bl	vlog
.L312:
	mov	r8, #-1
.L282:
	mov	r0, r8
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L283:
	ldr	r0, .L313+8
	bl	mpi_allocator_find_by_name
	mov	fp, r0
	cbnz	r0, .L285
	mov	r3, #316
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L313+4
	movs	r0, #8
	ldr	r2, .L313+12
	bl	vlog
	b	.L312
.L285:
	ldr	r5, [r4, #4]	@ unaligned
	movs	r3, #6
	ldr	r9, [r4, #8]	@ unaligned
	movs	r0, #8
	ldr	r7, .L313+16
	mov	r1, r5
	mla	r3, r3, r5, r9
	add	r3, r7, r3, lsl #2
	ldr	r3, [r3, #324]
	ldr	r2, [r3, #136]	@ unaligned
	ldr	r10, [r3, #208]	@ unaligned
	str	r2, [sp, #52]
	ldr	r2, [r3, #200]	@ unaligned
	str	r2, [sp, #32]
	ldr	r2, [r3, #204]	@ unaligned
	str	r2, [sp, #44]
	ldr	r2, [r3, #212]	@ unaligned
	str	r2, [sp, #40]
	ldr	r2, [r3, #192]	@ unaligned
	ldr	r3, [r3, #196]	@ unaligned
	str	r2, [sp, #36]
	mov	r2, r9
	str	r3, [sp, #48]
	bl	vsys_get_node_from_map
	ldr	r8, [r4, #56]	@ unaligned
	ubfx	r8, r8, #2, #1
	mov	r6, r0
	cmp	r8, #0
	beq	.L286
	ldr	r3, [r4, #52]	@ unaligned
	add	r7, r7, r5, lsl #2
	str	r3, [r7, #252]
	ldr	r8, [r4, #32]	@ unaligned
	ldr	r7, [r4, #52]	@ unaligned
	mov	r0, r8
	mov	r1, r7
	bl	__aeabi_uidivmod
	cbz	r1, .L287
	mov	r3, r7
	ldr	r2, [r4, #12]	@ unaligned
	mov	r1, r8
	ldr	r0, [r4, #24]	@ unaligned
	bl	get_new_wrap_line
	mov	r7, r0
.L287:
	ldr	r3, .L313+16
	add	r3, r3, r5, lsl #2
	ldr	r3, [r3, #228]
	cmp	r3, #1
	beq	.L299
	ldr	r8, [r6, #80]
	cmp	r8, #0
	bne	.L289
	ldr	r3, .L313+20
	mov	r1, #3520
	ldr	r0, [r3, #24]
	bl	kmem_cache_alloc
	mov	r8, r0
.L289:
	movs	r1, #6
	movs	r3, #1
	mla	r2, r1, r5, r9
	str	r3, [r8]
	ldr	r3, .L313+16
	str	r7, [r8, #36]
	add	r0, r3, r2, lsl #2
	ldr	r0, [r0, #396]
	cbnz	r0, .L291
	ldr	r0, [sp, #52]
	cmp	r0, #2
	bne	.L292
	add	r2, r3, r2, lsl #2
	ldr	r0, [r2, #324]
	ldr	r2, [r0, #220]	@ unaligned
	ldr	r0, [r0, #224]	@ unaligned
	mla	r2, r1, r2, r0
	ldr	r1, .L313+24
	add	r3, r3, r2, lsl #2
	ldr	r0, [r3, #396]
	bl	mpi_buf_pool_dup
.L293:
	movs	r3, #6
	mla	r9, r3, r5, r9
	ldr	r3, .L313+16
	add	r9, r3, r9, lsl #2
	str	r0, [r9, #396]
	cbz	r0, .L291
	mov	r1, r6
	bl	mpi_buf_pool_add_user
.L291:
	mov	r1, r8
	mov	r0, r6
	bl	mpi_node_set_params
	mov	r8, r0
.L288:
	bl	mcu_wrap_end
	mov	r0, r7
	bl	mcu_set_wrap_line
	movs	r1, #0
	ldr	r0, [r4, #32]	@ unaligned
	bl	mcu_wrap_config
	ldr	r3, .L313+16
	movs	r1, #1
	add	r2, r3, r5, lsl #2
	adds	r5, r5, #60
	str	r1, [r2, #228]
	ldr	r2, [r4, #52]	@ unaligned
	str	r2, [r3, r5, lsl #2]
.L286:
	ldr	r3, [sp, #48]
	movs	r1, #4
	ldr	r2, .L313+28
	movs	r0, #8
	str	r3, [sp, #24]
	ldr	r3, [sp, #36]
	str	r3, [sp, #20]
	ldr	r3, [sp, #40]
	strd	r10, r3, [sp, #12]
	ldr	r3, [sp, #44]
	str	r3, [sp, #8]
	ldr	r3, [sp, #32]
	str	r3, [sp, #4]
	movw	r3, #367
	str	r3, [sp]
	ldr	r3, .L313+4
	bl	vlog
	cmp	r10, #0
	beq	.L295
	ldr	r1, [r6, #80]
	cbnz	r1, .L296
	ldr	r3, .L313+20
	mov	r1, #3520
	ldr	r0, [r3, #24]
	bl	kmem_cache_alloc
	mov	r1, r0
.L296:
	movs	r3, #3
	str	r3, [r1]
	ldr	r3, [sp, #32]
	mov	r0, r6
	str	r3, [r1, #12]
	ldr	r3, [sp, #44]
	strd	r3, r10, [r1, #16]
	ldr	r3, [sp, #40]
	str	r3, [r1, #24]
	ldr	r3, [sp, #36]
	str	r3, [r1, #4]
	ldr	r3, [sp, #48]
	str	r3, [r1, #8]
	bl	mpi_node_set_params
	mov	r8, r0
.L295:
	ldr	r3, .L313+16
	movs	r2, #1
	str	r2, [r3, #4]
	cbz	r6, .L297
	mov	r3, #384
	ldr	r2, .L313+32
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L313+4
	movs	r0, #8
	bl	vlog
	mov	r0, r6
	bl	mpi_node_streamon
	mov	r8, r0
.L297:
	mov	r3, #388
	ldr	r2, .L313+36
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L313+4
	movs	r0, #8
	bl	vlog
	b	.L282
.L292:
	ldr	r3, [r4, #12]	@ unaligned
	mov	r0, fp
	ldr	r2, [r4, #52]	@ unaligned
	ldr	r1, .L313+40
	muls	r2, r3, r2
	ldr	r3, .L313+44
	str	r3, [sp]
	movs	r3, #4
	add	r2, r2, r2, lsl #1
	lsrs	r2, r2, #1
	bl	mpi_buf_pool_create
	b	.L293
.L299:
	mov	r8, #0
	b	.L288
.L314:
	.align	2
.L313:
	.word	.LC10
	.word	.LANCHOR1+348
	.word	.LC35
	.word	.LC36
	.word	.LANCHOR0
	.word	kmalloc_caches
	.word	.LC44
	.word	.LC53
	.word	.LC54
	.word	.LC55
	.word	-2147483647
	.word	.LC43
	.fnend
	.size	vvi_node_start, .-vvi_node_start
	.align	1
	.global	vvi_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_probe, %function
vvi_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r2, #756
	ldr	r4, .L320
	mov	r5, r0
	movs	r1, #0
	ldr	r10, .L320+16
	add	r9, r4, #468
	mov	r8, #4
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r5
	bl	kstrdup
	movs	r3, #1
	str	r3, [r4, #224]
	mov	r2, r5
	ldr	r3, .L320+4
	movs	r1, #8
	str	r4, [r4, #192]
	movs	r5, #0
	str	r3, [r4, #24]
	movs	r3, #17
	str	r3, [r4, #28]
	str	r0, [r4]
	add	r0, r4, #32
	bl	mpi_dev_register
	str	r5, [r4, #100]
	strd	r5, r5, [r4, #108]
	add	r4, r4, #760
.L316:
	sub	r6, r9, r5
	movs	r7, #0
.L317:
	adds	r3, r5, r6
	str	r6, [r6]
	add	r0, r6, #144
	mov	r2, r4
	str	r6, [r3, r8]
	mov	r1, r10
	adds	r7, r7, #1
	bl	__init_waitqueue_head
	cmp	r7, #6
	add	r6, r6, #8
	bne	.L317
	subs	r5, r5, #48
	add	r8, r8, #48
	cmn	r5, #144
	bne	.L316
	movs	r2, #0
	ldr	r1, .L320+8
	ldr	r0, .L320+12
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	b	mpi_sysfs_dev_add
.L321:
	.align	2
.L320:
	.word	.LANCHOR0
	.word	.LANCHOR1+364
	.word	.LANCHOR1+704
	.word	.LANCHOR0+32
	.word	.LC56
	.fnend
	.size	vvi_probe, .-vvi_probe
	.align	1
	.global	vvi_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_shutdown, %function
vvi_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	movs	r3, #1
	ldr	r4, .L323
	add	r5, r4, #32
	str	r3, [r4, #12]
	mov	r0, r5
	bl	mpi_sysfs_dev_remove
	mov	r0, r5
	bl	mpi_dev_unregister
	ldr	r0, [r4]
	pop	{r3, r4, r5, lr}
	b	kfree
.L324:
	.align	2
.L323:
	.word	.LANCHOR0
	.fnend
	.size	vvi_shutdown, .-vvi_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	vvi_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_init, %function
vvi_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L326
	bl	vvi_probe
	movs	r0, #0
	pop	{r3, pc}
.L327:
	.align	2
.L326:
	.word	.LC43
	.fnend
	.size	vvi_init, .-vvi_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	vvi_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_exit, %function
vvi_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	vvi_shutdown
	.fnend
	.size	vvi_exit, .-vvi_exit
	.global	wait_mcu_flag
	.global	g_vvi_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__func__.38668, %object
	.size	__func__.38668, 18
__func__.38668:
	.ascii	"get_new_wrap_line\000"
	.type	__func__.38710, %object
	.size	__func__.38710, 20
__func__.38710:
	.ascii	"vvi_node_get_status\000"
	.type	__func__.38956, %object
	.size	__func__.38956, 8
__func__.38956:
	.ascii	"vi_poll\000"
	.type	__func__.38699, %object
	.size	__func__.38699, 14
__func__.38699:
	.ascii	"vvi_node_stop\000"
	.type	__func__.38750, %object
	.size	__func__.38750, 11
__func__.38750:
	.ascii	"vvi_mb_get\000"
	.type	__func__.38900, %object
	.size	__func__.38900, 11
__func__.38900:
	.ascii	"vi_release\000"
	.type	__func__.38866, %object
	.size	__func__.38866, 9
__func__.38866:
	.ascii	"vi_ioctl\000"
	.type	__func__.38843, %object
	.size	__func__.38843, 20
__func__.38843:
	.ascii	"vvi_node_get_chn_fr\000"
	.type	__func__.38878, %object
	.size	__func__.38878, 8
__func__.38878:
	.ascii	"vi_open\000"
	.type	__func__.38760, %object
	.size	__func__.38760, 22
__func__.38760:
	.ascii	"vvi_change_resolution\000"
	.type	__func__.38811, %object
	.size	__func__.38811, 25
__func__.38811:
	.ascii	"vvi_node_set_pipe_status\000"
	.type	__func__.38819, %object
	.size	__func__.38819, 25
__func__.38819:
	.ascii	"vvi_node_get_pipe_status\000"
	.type	__func__.38640, %object
	.size	__func__.38640, 16
__func__.38640:
	.ascii	"vvi_node_delete\000"
	.type	__func__.38832, %object
	.size	__func__.38832, 20
__func__.38832:
	.ascii	"vvi_node_set_chn_fr\000"
	.type	__func__.38779, %object
	.size	__func__.38779, 23
__func__.38779:
	.ascii	"vvi_node_get_chns_attr\000"
	.type	__func__.38626, %object
	.size	__func__.38626, 16
__func__.38626:
	.ascii	"vvi_node_create\000"
	.type	__func__.38770, %object
	.size	__func__.38770, 23
__func__.38770:
	.ascii	"vvi_node_set_chns_attr\000"
	.type	__func__.38741, %object
	.size	__func__.38741, 11
__func__.38741:
	.ascii	"vvi_mb_put\000"
	.type	__func__.38789, %object
	.size	__func__.38789, 24
__func__.38789:
	.ascii	"vvi_node_set_dev_status\000"
	.type	__func__.38799, %object
	.size	__func__.38799, 24
__func__.38799:
	.ascii	"vvi_node_get_dev_status\000"
	.type	__func__.38691, %object
	.size	__func__.38691, 15
__func__.38691:
	.ascii	"vvi_node_start\000"
	.space	1
	.type	vvi_ioctls, %object
	.size	vvi_ioctls, 340
vvi_ioctls:
	.word	-1065585152
	.word	0
	.word	vvi_get_version
	.word	.LC57
	.word	0
	.word	-1058769407
	.word	0
	.word	vvi_node_create
	.word	.LC58
	.word	0
	.word	-1058769406
	.word	0
	.word	vvi_node_delete
	.word	.LC59
	.word	0
	.word	-1064798717
	.word	0
	.word	vvi_node_get_status
	.word	.LC60
	.word	0
	.word	-1058769404
	.word	0
	.word	vvi_node_start
	.word	.LC61
	.word	0
	.word	-1058769403
	.word	0
	.word	vvi_node_stop
	.word	.LC62
	.word	0
	.word	-1064798714
	.word	0
	.word	vvi_mb_put
	.word	.LC63
	.word	0
	.word	-1064798713
	.word	16
	.word	vvi_mb_get
	.word	.LC64
	.word	0
	.word	-1071548920
	.word	0
	.word	vvi_change_resolution
	.word	.LC65
	.word	0
	.word	-1057458679
	.word	0
	.word	vvi_node_set_chns_attr
	.word	.LC66
	.word	0
	.word	-1057458678
	.word	0
	.word	vvi_node_get_chns_attr
	.word	.LC67
	.word	0
	.word	-1057458677
	.word	0
	.word	vvi_node_set_dev_status
	.word	.LC68
	.word	0
	.word	-1057458676
	.word	0
	.word	vvi_node_get_dev_status
	.word	.LC69
	.word	0
	.word	-1072400883
	.word	0
	.word	vvi_node_set_chn_fr
	.word	.LC70
	.word	0
	.word	-1072400882
	.word	0
	.word	vvi_node_get_chn_fr
	.word	.LC71
	.word	0
	.word	-1057458673
	.word	0
	.word	vvi_node_set_pipe_status
	.word	.LC72
	.word	0
	.word	-1057458672
	.word	0
	.word	vvi_node_get_pipe_status
	.word	.LC73
	.word	0
	.type	vvi_driver_fops, %object
	.size	vvi_driver_fops, 128
vvi_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	vi_poll
	.word	vi_ioctl
	.word	vi_compat_ioctl
	.word	0
	.space	4
	.word	vi_open
	.space	4
	.word	vi_release
	.space	60
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_vvi_dev, %object
	.size	g_vvi_dev, 756
g_vvi_dev:
	.space	756
	.type	wait_mcu_flag, %object
	.size	wait_mcu_flag, 4
wait_mcu_flag:
	.space	4
	.type	__key.38972, %object
	.size	__key.38972, 0
__key.38972:
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"-2024-05-24-16:21:01\000"
.LC1:
	.ascii	"vmpi:28ee389dbc33c2c7db812cd08e54fd3a0f2f992b-v1.4."
	.ascii	"81\000"
.LC2:
	.ascii	"\033[1;32mtmp = %d, vi_res->wrap_line = %d, cur_w ="
	.ascii	" %d, cur_h = %d, max_res_width = %d\033[0m\000"
.LC3:
	.ascii	"\033[1;31mstruct vvi_buff_info mismatching\033[0m\000"
.LC4:
	.ascii	"enter...\000"
.LC5:
	.ascii	"buf pool(%d) find\000"
.LC6:
	.ascii	"buf = %px ref cnt = %d\000"
.LC7:
	.ascii	"exit...\000"
.LC8:
	.ascii	"\033[1;32mvi_poll pipeId: %d, chnId: %d\033[0m\000"
.LC9:
	.ascii	"\033[1;32mvvi_node out_buf_queue: refcnt:%d\033[0m\000"
.LC10:
	.ascii	"\033[1;31mstruct vvi_node_info mismatching\033[0m\000"
.LC11:
	.ascii	"\033[1;32mdo vi vvi_node_stop dev:%d, chn:%d\033[0m"
	.ascii	"\000"
.LC12:
	.ascii	"\033[1;31mwrap mode not support get buffer from chn"
	.ascii	" 0\033[0m\000"
.LC13:
	.ascii	"\033[1;31mget node fail devid = %d, chnid = %d\033["
	.ascii	"0m\000"
.LC14:
	.ascii	"\033[1;31mget buf fail timeout = %d\033[0m\000"
.LC15:
	.ascii	"get buff: %px, dma_buf id = %d, frame seq=%d, pts:%"
	.ascii	"lld\000"
.LC16:
	.ascii	"buf id:%d size:%d w:%d h:%d pts:%llu\000"
.LC17:
	.ascii	"%s %d\000"
.LC18:
	.ascii	"\033[1;32mdelete vvi node: dev:%d, chn:%d\033[0m\000"
.LC19:
	.ascii	"\033[1;32mbuf id = %d, user process exit\033[0m\000"
.LC20:
	.ascii	"\033[1;31mvi_ioctl: error vvi ioctl, invalid ioctrl"
	.ascii	" index %d\033[0m\000"
.LC21:
	.ascii	"\033[1;31mstruct vi_chn_frame_rate mismatching\033["
	.ascii	"0m\000"
.LC22:
	.ascii	"\033[1;31mdata alloc failed!\012\033[0m\000"
.LC23:
	.ascii	"\033[1;32m--------- vi open ----------\033[0m\000"
.LC24:
	.ascii	"\033[1;31mno memory for rockit_cfg\033[0m\000"
.LC25:
	.ascii	"\033[1;31mstruct vvi_resolution mismatching\033[0m\000"
.LC26:
	.ascii	"change resolution w:%d, h:%d node: %d\000"
.LC27:
	.ascii	"\033[1;32mvi_res->wrap_mode = %d\033[0m\000"
.LC28:
	.ascii	"vvi_change_resolution done\000"
.LC29:
	.ascii	"\033[1;31mstruct vi_attr_kn_ctx mismatching\033[0m\000"
.LC30:
	.ascii	"\033[1;31mpipeId %d need less than %d\033[0m\000"
.LC31:
	.ascii	"\033[1;32mdev = %d, chn = %d is empty\033[0m\000"
.LC32:
	.ascii	"do vi vvi_node_delete\000"
.LC33:
	.ascii	"\033[1;31mdevId %d need less than %d\033[0m\000"
.LC34:
	.ascii	"\033[1;31mchnId %d need less than %d\033[0m\000"
.LC35:
	.ascii	"valloc\000"
.LC36:
	.ascii	"\033[1;31merror getting vdev  %p\033[0m\000"
.LC37:
	.ascii	"\033[1;32mvi_pri_data pipeId: %d chnId:%d--->>>\033"
	.ascii	"[0m\000"
.LC38:
	.ascii	"\033[1;32mvvi chn:%d node have exist!\033[0m\000"
.LC39:
	.ascii	"\033[1;32m>>>>>> videv %d: %p, driver: %s\033[0m\000"
.LC40:
	.ascii	"\033[1;32mwidth = %d, height = %d, max_res_width = "
	.ascii	"%d, max_res_height = %d, enAllocBufType = %d\033[0m"
	.ascii	"\000"
.LC41:
	.ascii	"\033[1;31merror create dev:%d, chn:%d %s vvi node\033"
	.ascii	"[0m\000"
.LC42:
	.ascii	"\033[1;32m vvi dev:%d, chn:%d node create success\033"
	.ascii	"[0m\000"
.LC43:
	.ascii	"vvi\000"
.LC44:
	.ascii	"vvi_dup\000"
.LC45:
	.ascii	"\033[1;32mvvi_node_create chn:%d pool create bufcnt"
	.ascii	":%d\033[0m\000"
.LC46:
	.ascii	"\033[1;31mchn:%d pool create fail\033[0m\000"
.LC47:
	.ascii	"\033[1;32mvvi_node_create vi_node_info->mbPool:%d\033"
	.ascii	"[0m\000"
.LC48:
	.ascii	"\033[1;32mchn:%d depth: %d\033[0m\000"
.LC49:
	.ascii	"\033[1;32mvvi_node_create done\033[0m\000"
.LC50:
	.ascii	"put buff: id:%d, buf:%p, dma_buf:%p\000"
.LC51:
	.ascii	"unref buffer\000"
.LC52:
	.ascii	"\033[1;31mwait mcu start fail\033[0m\000"
.LC53:
	.ascii	"\033[1;32mstWindow = %d, %d, %d, %d, stMaxsize = %d"
	.ascii	", %d\033[0m\000"
.LC54:
	.ascii	"mpi_node_streamon start\000"
.LC55:
	.ascii	"\033[1;32mvvi_node_start done\033[0m\000"
.LC56:
	.ascii	"&vdev->wait[devId][chnId]\000"
.LC57:
	.ascii	"VVI_IOCTL_VERSION\000"
.LC58:
	.ascii	"VVI_IOCTL_NODE_CREATE\000"
.LC59:
	.ascii	"VVI_IOCTL_NODE_DELETE\000"
.LC60:
	.ascii	"VVI_IOCTL_NODE_GET_STATUS\000"
.LC61:
	.ascii	"VVI_IOCTL_NODE_START\000"
.LC62:
	.ascii	"VVI_IOCTL_NODE_STOP\000"
.LC63:
	.ascii	"VVI_IOCTL_MB_PUT\000"
.LC64:
	.ascii	"VVI_IOCTL_MB_GET\000"
.LC65:
	.ascii	"VVI_IOCTL_RES_CHANGE\000"
.LC66:
	.ascii	"VVI_IOCTL_SET_ATTR\000"
.LC67:
	.ascii	"VVI_IOCTL_GET_ATTR\000"
.LC68:
	.ascii	"VVI_IOCTL_SET_DEV_STA\000"
.LC69:
	.ascii	"VVI_IOCTL_GET_DEV_STA\000"
.LC70:
	.ascii	"VVI_IOCTL_SET_FR\000"
.LC71:
	.ascii	"VVI_IOCTL_GET_FR\000"
.LC72:
	.ascii	"VVI_IOCTL_SET_PIPE_STA\000"
.LC73:
	.ascii	"VVI_IOCTL_GET_PIPE_STA\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
