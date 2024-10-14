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
	.file	"aiisp.c"
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
	.type	list_del, %function
list_del:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r2, r3, [r0]
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r2, #256
	mov	r3, #290
	strd	r2, r3, [r0]
	bx	lr
	.fnend
	.size	list_del, .-list_del
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_compat_ioctl, %function
aiisp_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	aiisp_compat_ioctl, .-aiisp_compat_ioctl
	.align	1
	.global	aiisp_get_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_get_version, %function
aiisp_get_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r1
	ldr	r5, [r0, #128]
	movs	r2, #76
	movs	r1, #0
	mov	r0, r4
	ldr	r6, [r5, #160]
	bl	memset
	ldr	r3, [r6, #180]
	str	r3, [r4, #4]	@ unaligned
	ldr	r3, [r5]
	movs	r5, #0
	strb	r5, [r4, #12]
	strb	r5, [r4, #13]
	str	r3, [r4, #8]	@ unaligned
	strb	r5, [r4, #14]
	strb	r5, [r4, #15]
	ldr	r0, [r6, #176]
	bl	strlen
	movs	r3, #20
	strb	r5, [r4, #37]
	strb	r3, [r4, #36]
	movs	r3, #17
	strb	r5, [r4, #38]
	strb	r5, [r4, #39]
	strb	r3, [r4, #56]
	strb	r5, [r4, #57]
	strb	r5, [r4, #58]
	strb	r5, [r4, #59]
	cmp	r0, #15
	str	r0, [r4, #16]	@ unaligned
	bls	.L4
	movs	r3, #15
	strb	r5, [r4, #17]
	strb	r3, [r4, #16]
	strb	r5, [r4, #18]
	strb	r5, [r4, #19]
.L4:
	movs	r5, #0
	movs	r3, #16
	strb	r5, [r4, #37]
	add	r0, r4, #20
	strb	r3, [r4, #36]
	strb	r3, [r4, #56]
	strb	r5, [r4, #38]
	strb	r5, [r4, #39]
	strb	r5, [r4, #57]
	strb	r5, [r4, #58]
	strb	r5, [r4, #59]
	ldr	r2, [r4, #16]	@ unaligned
	ldr	r1, [r6, #176]
	bl	strncpy
	ldr	r2, [r4, #36]	@ unaligned
	ldr	r1, .L5
	add	r0, r4, #40
	bl	strncpy
	ldr	r2, [r4, #56]	@ unaligned
	ldr	r1, .L5+4
	add	r0, r4, #60
	bl	strncpy
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L6:
	.align	2
.L5:
	.word	.LC0
	.word	.LC1
	.fnend
	.size	aiisp_get_version, .-aiisp_get_version
	.align	1
	.global	aiisp_node_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_node_reset, %function
aiisp_node_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	movs	r0, #23
	ldr	r3, [r1, #4]	@ unaligned
	movs	r1, #4
	ldr	r2, .L8
	str	r3, [sp, #4]
	mov	r3, #304
	str	r3, [sp]
	ldr	r3, .L8+4
	bl	vlog
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L9:
	.align	2
.L8:
	.word	.LC2
	.word	.LANCHOR0
	.fnend
	.size	aiisp_node_reset, .-aiisp_node_reset
	.align	1
	.global	aiisp_node_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_node_create, %function
aiisp_node_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r1
	ldr	r3, [r1]	@ unaligned
	.pad #28
	sub	sp, sp, #28
	ldr	r5, [r0, #128]
	cmp	r3, #32
	beq	.L11
	movs	r3, #100
	ldr	r2, .L27
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L27+4
	movs	r0, #23
	bl	vlog
.L25:
	mov	r0, #-1
.L10:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L11:
	ldr	r0, .L27+8
	bl	mpi_allocator_find_by_name
	ldr	r6, [r4, #4]	@ unaligned
	mov	r8, r0
	cbnz	r0, .L13
	movs	r3, #106
	ldr	r2, .L27+12
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L27+4
	str	r6, [sp, #4]
.L26:
	movs	r0, #23
	bl	vlog
	b	.L25
.L13:
	mov	r2, r6
	movs	r1, #0
	movs	r0, #23
	bl	vsys_get_node_from_map
	mov	r7, r0
	cbz	r0, .L14
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	str	r6, [sp, #8]
	movs	r0, #23
	ldr	r2, .L27+16
	str	r3, [sp, #4]
	movs	r3, #115
	str	r3, [sp]
	ldr	r3, .L27+4
	bl	vlog
.L15:
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	ldr	r2, .L27+20
	movs	r0, #23
	str	r3, [sp, #4]
	movs	r3, #157
	str	r3, [sp]
	ldr	r3, .L27+4
	bl	vlog
	movs	r0, #0
	b	.L10
.L14:
	mov	r3, r0
	mov	r2, r0
	mov	r1, r6
	mov	r0, r5
	bl	mpi_dev_create_node_with_params
	ldr	r3, [r4, #4]	@ unaligned
	mov	r5, r0
	strd	r3, r6, [sp, #4]
	cbnz	r0, .L16
	movs	r3, #121
	ldr	r2, .L27+24
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L27+4
	movs	r0, #23
	bl	vlog
	b	.L25
.L16:
	ldr	r9, .L27+48
	movs	r3, #124
	ldr	r2, .L27+28
	movs	r1, #4
	str	r3, [sp]
	movs	r0, #23
	ldr	r3, .L27+4
	bl	vlog
	str	r9, [r5, #4]
	mov	r3, r5
	ldr	r2, [r4, #4]	@ unaligned
	mov	r1, r7
	movs	r0, #23
	bl	vsys_add_node_to_map
	ldr	r6, [r4, #24]	@ unaligned
	cmp	r6, #2
	bne	.L17
	str	r9, [sp]
	movs	r1, #1
	ldr	r3, [r4, #16]	@ unaligned
	mov	r0, r8
	ldr	r2, [r4, #20]	@ unaligned
	bl	mpi_buf_pool_create
	mov	r7, r0
	cbz	r0, .L18
	mov	r1, r5
	bl	mpi_buf_pool_add_user
	ldr	r3, [r4, #20]	@ unaligned
	ldr	r2, .L27+32
	movs	r1, #4
	movs	r0, #23
	str	r3, [sp, #20]
	ldr	r3, [r4, #16]	@ unaligned
	str	r3, [sp, #16]
	ldr	r3, [r7]
	strd	r7, r3, [sp, #8]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #4]
	movs	r3, #139
	str	r3, [sp]
	ldr	r3, .L27+4
	bl	vlog
.L19:
	ldr	r3, [r5, #288]
	ldr	r2, [r4, #24]	@ unaligned
	str	r2, [r3, #20]
	b	.L15
.L18:
	ldr	r3, [r4, #4]	@ unaligned
	mov	r1, r6
	ldr	r2, .L27+36
	movs	r0, #23
	str	r3, [sp, #4]
	movs	r3, #141
	str	r3, [sp]
	ldr	r3, .L27+4
	bl	vlog
	ldr	r2, [r4, #4]	@ unaligned
	mov	r1, r7
	movs	r0, #23
	bl	vsys_remove_node_from_map
	mov	r0, r5
	bl	mpi_node_destroy
	ldr	r3, [r4, #4]	@ unaligned
	ldr	r2, .L27+40
	movs	r1, #4
	str	r3, [sp, #4]
	movs	r3, #175
	str	r3, [sp]
	ldr	r3, .L27+4
	b	.L26
.L17:
	cmp	r6, #3
	bne	.L19
	ldr	r0, [r4, #28]	@ unaligned
	bl	vsys_get_pool
	mov	r6, r0
	cmp	r0, #0
	beq	.L19
	mov	r1, r5
	bl	mpi_buf_pool_add_user
	ldr	r3, [r4, #28]	@ unaligned
	ldr	r2, .L27+44
	movs	r1, #4
	movs	r0, #23
	strd	r6, r3, [sp, #4]
	movs	r3, #150
	str	r3, [sp]
	ldr	r3, .L27+4
	bl	vlog
	b	.L19
.L28:
	.align	2
.L27:
	.word	.LC3
	.word	.LANCHOR0+17
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC14
	.word	.LC7
	.word	.LC8
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC9
	.fnend
	.size	aiisp_node_create, .-aiisp_node_create
	.align	1
	.global	aiisp_node_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_node_destroy, %function
aiisp_node_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r6, r1
	ldr	r3, [r1]	@ unaligned
	cmp	r3, #32
	beq	.L30
	movs	r3, #192
	ldr	r2, .L54
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L54+4
	movs	r0, #23
	bl	vlog
.L53:
	mov	r0, #-1
.L29:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L30:
	ldr	r2, [r1, #4]	@ unaligned
	movs	r0, #23
	movs	r1, #0
	bl	vsys_get_node_from_map
	mov	r4, r0
	cbnz	r0, .L32
	ldr	r3, [r6, #4]	@ unaligned
	movs	r1, #2
	ldr	r2, .L54+8
	movs	r0, #23
	str	r3, [sp, #4]
	movs	r3, #199
	str	r3, [sp]
	ldr	r3, .L54+4
	bl	vlog
	b	.L53
.L32:
	ldr	r7, [r0, #60]
	ldr	r5, [r0, #288]
	cmp	r7, #0
	beq	.L33
	mov	r1, r0
	mov	r0, r7
	bl	mpi_buf_pool_del_user
	cbnz	r5, .L34
.L40:
	ldr	r2, [r6, #4]	@ unaligned
	movs	r1, #0
	movs	r0, #23
	bl	vsys_remove_node_from_map
	ldr	r3, [r4, #324]
	cbz	r3, .L36
	mov	r0, r4
	bl	mpi_node_del_in_watcher
.L36:
	mov	r0, r4
	bl	mpi_node_destroy
	ldr	r3, [r6, #4]	@ unaligned
	ldr	r2, .L54+12
	movs	r1, #4
	movs	r0, #23
	str	r3, [sp, #4]
	movs	r3, #234
	str	r3, [sp]
	ldr	r3, .L54+4
	bl	vlog
	movs	r0, #0
	b	.L29
.L34:
	ldr	r3, [r5, #20]
	cmp	r3, #2
	bne	.L38
	movs	r1, #1
	mov	r0, r7
	bl	mpi_buf_pool_destroy
.L38:
	ldr	r2, [r4, #76]
	mvn	r8, #43
	ldr	r1, [r5]
	adds	r7, r2, #1
	sub	r3, r8, r7, lsl #3
	add	r1, r1, r3
	add	r2, r1, r2, lsl #3
	ldr	r7, [r2, #52]
	add	r7, r7, r3
.L41:
	ldr	r3, [r4, #76]
	add	r0, r1, r3, lsl #3
	adds	r0, r0, #52
	cmp	r0, r5
	bne	.L43
	add	r7, r3, #33
	ldr	r1, [r5, #8]!
	mvn	r8, #43
	sub	r2, r8, r7, lsl #3
	add	r1, r1, r2
	add	r3, r1, r3, lsl #3
	ldr	r7, [r3, #308]
	add	r7, r7, r2
.L44:
	ldr	r0, [r4, #76]
	add	r0, r1, r0, lsl #3
	add	r0, r0, #308
	cmp	r0, r5
	beq	.L40
	cbz	r1, .L45
	bl	list_del
	mov	r0, r1
	bl	mpi_obj_unref
.L45:
	ldr	r3, [r4, #76]
	mov	r1, r7
	add	r2, r3, #33
	add	r3, r7, r3, lsl #3
	sub	r2, r8, r2, lsl #3
	ldr	r3, [r3, #308]
	adds	r7, r3, r2
	b	.L44
.L33:
	cmp	r5, #0
	bne	.L38
	b	.L40
.L43:
	cbz	r1, .L42
	bl	list_del
	mov	r0, r1
	bl	mpi_obj_unref
.L42:
	ldr	r3, [r4, #76]
	mov	r1, r7
	adds	r2, r3, #1
	add	r3, r7, r3, lsl #3
	sub	r2, r8, r2, lsl #3
	ldr	r3, [r3, #52]
	adds	r7, r3, r2
	b	.L41
.L55:
	.align	2
.L54:
	.word	.LC3
	.word	.LANCHOR0+35
	.word	.LC15
	.word	.LC16
	.fnend
	.size	aiisp_node_destroy, .-aiisp_node_destroy
	.align	1
	.global	aiisp_node_bypass
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_node_bypass, %function
aiisp_node_bypass:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	mov	r4, r1
	ldr	r3, [r1]	@ unaligned
	cmp	r3, #32
	beq	.L57
	mov	r3, #318
	ldr	r2, .L65
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L65+4
	movs	r0, #23
	bl	vlog
	mov	r0, #-1
.L56:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L57:
	ldr	r2, [r1, #4]	@ unaligned
	movs	r0, #23
	movs	r1, #0
	bl	vsys_get_node_from_map
	cbz	r0, .L59
	ldr	r3, [r4, #8]	@ unaligned
	ldr	r2, [r0, #288]
	cmp	r3, #1
	str	r3, [r2, #16]
	bne	.L60
	bl	mpi_node_del_in_watcher
.L61:
	ldr	r3, [r4, #8]	@ unaligned
	movs	r1, #4
	ldr	r2, .L65+8
	movs	r0, #23
	str	r3, [sp, #8]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #331
	str	r3, [sp]
	ldr	r3, .L65+4
	bl	vlog
.L59:
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	ldr	r2, .L65+12
	movs	r0, #23
	str	r3, [sp, #4]
	mov	r3, #334
	str	r3, [sp]
	ldr	r3, .L65+4
	bl	vlog
	movs	r0, #0
	b	.L56
.L60:
	movs	r1, #2
	bl	mpi_node_add_in_watcher
	b	.L61
.L66:
	.align	2
.L65:
	.word	.LC3
	.word	.LANCHOR0+54
	.word	.LC17
	.word	.LC18
	.fnend
	.size	aiisp_node_bypass, .-aiisp_node_bypass
	.align	1
	.global	aiisp_mb_get
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_mb_get, %function
aiisp_mb_get:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r1
	ldr	r3, [r1]	@ unaligned
	.pad #28
	sub	sp, sp, #28
	cmp	r3, #108
	beq	.L68
	mov	r3, #368
	ldr	r2, .L90
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L90+4
	movs	r0, #23
	bl	vlog
.L73:
	mov	r0, #-1
.L67:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L68:
	ldr	r7, [r1, #4]	@ unaligned
	movs	r0, #23
	movs	r1, #0
	mov	r2, r7
	bl	vsys_get_node_from_map
	mov	r6, r0
	cbnz	r0, .L70
	movw	r3, #377
	str	r7, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L90+4
	movs	r0, #23
	ldr	r2, .L90+8
	bl	vlog
	b	.L73
.L70:
	ldr	r1, [r5, #8]	@ unaligned
	cmp	r1, #0
	bne	.L71
	ldr	r3, [r5, #12]	@ unaligned
	str	r3, [r0, #380]
	bl	mpi_node_get_in_buffer
	mov	r4, r0
	cmp	r0, #0
	beq	.L73
	ldr	r3, [r0, #16]
	movs	r1, #5
	ldr	r2, .L90+12
	str	r3, [sp, #16]
	ldr	r3, [r0]
	str	r7, [sp, #4]
	strd	r0, r3, [sp, #8]
	mov	r3, #386
	movs	r0, #23
	str	r3, [sp]
	ldr	r3, .L90+4
	bl	vlog
	ldr	r3, [r4, #448]
	cmp	r3, #0
	beq	.L73
.L74:
	ldr	r3, [r6, #288]
	cbz	r3, .L75
	ldr	r2, [r5, #8]	@ unaligned
	cmp	r2, #0
	bne	.L76
	ldr	r2, [r6, #76]
	ldr	r0, [r3, #4]
	add	r1, r4, r2, lsl #3
	add	r2, r4, r2, lsl #3
	adds	r1, r1, #52
	str	r1, [r3, #4]
	strd	r3, r0, [r2, #52]
.L89:
	str	r1, [r0]
.L75:
	movs	r2, #80
	add	r1, r4, #616
	add	r0, r5, #28
	bl	memcpy
	ldr	r3, [r4, #448]
	cbz	r3, .L77
	ldr	r3, [r3]
.L77:
	str	r3, [r5, #24]	@ unaligned
	movs	r0, #0
	ldr	r3, [r4]
	str	r3, [r5, #16]	@ unaligned
	b	.L67
.L71:
	cmp	r1, #2
	bne	.L78
	movs	r1, #0
	bl	mpi_node_request_buf
	mov	r4, r0
	cmp	r0, #0
	beq	.L73
	ldr	r3, [r0, #16]
	movs	r1, #5
	ldr	r2, .L90+16
	str	r3, [sp, #16]
	ldr	r3, [r0]
	str	r7, [sp, #4]
	strd	r0, r3, [sp, #8]
	movw	r3, #393
	movs	r0, #23
	str	r3, [sp]
	ldr	r3, .L90+4
	bl	vlog
	b	.L74
.L78:
	movs	r4, #0
	b	.L74
.L76:
	cmp	r2, #2
	bne	.L75
	ldr	r2, [r6, #76]
	adds	r3, r3, #8
	ldr	r0, [r3, #4]
	add	r1, r4, r2, lsl #3
	add	r2, r4, r2, lsl #3
	add	r1, r1, #308
	str	r1, [r3, #4]
	strd	r3, r0, [r2, #308]
	b	.L89
.L91:
	.align	2
.L90:
	.word	.LC19
	.word	.LANCHOR0+72
	.word	.LC15
	.word	.LC20
	.word	.LC21
	.fnend
	.size	aiisp_mb_get, .-aiisp_mb_get
	.align	1
	.global	aiisp_mb_send
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_mb_send, %function
aiisp_mb_send:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r1
	ldr	r3, [r1]	@ unaligned
	.pad #36
	sub	sp, sp, #36
	cmp	r3, #108
	beq	.L93
	mov	r3, #428
	ldr	r2, .L102
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L102+4
	movs	r0, #23
	bl	vlog
.L100:
	mov	r0, #-1
.L92:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L93:
	ldr	r6, [r1, #4]	@ unaligned
	movs	r0, #23
	movs	r1, #0
	mov	r2, r6
	bl	vsys_get_node_from_map
	mov	r7, r0
	cbnz	r0, .L95
	movw	r3, #437
	str	r6, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L102+4
	movs	r0, #23
	ldr	r2, .L102+8
	bl	vlog
	b	.L100
.L95:
	ldr	r0, [r4, #20]	@ unaligned
	cmp	r0, #0
	bge	.L96
.L101:
	movs	r0, #0
	b	.L92
.L96:
	bl	get_mpi_buf
	ldr	r3, [r4, #20]	@ unaligned
	mov	r5, r0
	cbnz	r0, .L97
	strd	r6, r3, [sp, #4]
	movs	r1, #2
	mov	r3, #444
	ldr	r2, .L102+12
	str	r3, [sp]
	movs	r0, #23
	ldr	r3, .L102+4
	bl	vlog
	b	.L100
.L97:
	ldr	r1, [r4, #8]	@ unaligned
	ldr	r2, [r0, #16]
	strd	r3, r1, [sp, #24]
	movs	r1, #5
	ldr	r3, [r0, #448]
	strd	r2, r3, [sp, #16]
	ldr	r3, [r0]
	ldr	r2, .L102+16
	str	r6, [sp, #4]
	strd	r0, r3, [sp, #8]
	mov	r3, #450
	movs	r0, #23
	str	r3, [sp]
	ldr	r3, .L102+4
	bl	vlog
	add	r1, r4, #28
	movs	r2, #80
	add	r0, r5, #616
	bl	memcpy
	ldr	r4, [r4, #8]	@ unaligned
	cbnz	r4, .L98
	mov	r2, r4
	mov	r1, r5
	mov	r0, r7
	bl	mpi_node_buffer_in
	b	.L101
.L98:
	cmp	r4, #1
	bne	.L101
	movs	r2, #0
	mov	r1, r5
	mov	r0, r7
	bl	mpi_node_buffer_out
	b	.L101
.L103:
	.align	2
.L102:
	.word	.LC19
	.word	.LANCHOR0+85
	.word	.LC15
	.word	.LC22
	.word	.LC23
	.fnend
	.size	aiisp_mb_send, .-aiisp_mb_send
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_handle_message, %function
aiisp_handle_message:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r0
	ldr	r3, [r0, #8]
	.pad #20
	sub	sp, sp, #20
	mov	r5, r2
	cmp	r3, #1
	bne	.L106
	subs	r3, r1, #2
	cmp	r3, #1
	bls	.L106
	ldr	r3, .L111
	ldr	r6, [r3]
	cbnz	r6, .L106
	cmp	r1, #1
	bne	.L106
	ldr	r3, [r2]
	movs	r1, #5
	ldr	r7, [r0, #288]
	ldr	r2, .L111+4
	str	r3, [sp, #12]
	ldr	r3, [r0]
	str	r3, [sp, #8]
	ldr	r3, [r0, #44]
	movs	r0, #23
	ldr	r3, [r3, #44]
	str	r3, [sp, #4]
	mov	r3, #692
	str	r3, [sp]
	ldr	r3, .L111+8
	bl	vlog
	ldr	r3, [r7, #16]
	cbz	r3, .L106
	mov	r2, r6
	mov	r1, r5
	mov	r0, r4
	bl	mpi_node_buffer_out
.L106:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L112:
	.align	2
.L111:
	.word	.LANCHOR1
	.word	.LC24
	.word	.LANCHOR0+99
	.fnend
	.size	aiisp_handle_message, .-aiisp_handle_message
	.align	1
	.global	aiisp_mb_release
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_mb_release, %function
aiisp_mb_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r6, r1
	ldr	r3, [r1]	@ unaligned
	.pad #36
	sub	sp, sp, #36
	cmp	r3, #108
	beq	.L114
	mov	r3, #474
	ldr	r2, .L123
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L123+4
	movs	r0, #23
	bl	vlog
.L121:
	mov	r0, #-1
.L113:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L114:
	ldr	r7, [r1, #4]	@ unaligned
	movs	r0, #23
	movs	r1, #0
	mov	r2, r7
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbnz	r0, .L116
	movw	r3, #483
	str	r7, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L123+4
	movs	r0, #23
	ldr	r2, .L123+8
	bl	vlog
	b	.L121
.L116:
	movs	r1, #4
	ldr	r0, [r6, #16]	@ unaligned
	bl	mpi_obj_from_id
	mov	r4, r0
	cmp	r0, #0
	beq	.L117
	ldr	r3, [r0, #16]
	movs	r1, #5
	ldr	r2, [r6, #8]	@ unaligned
	strd	r3, r2, [sp, #20]
	ldr	r3, [r6, #16]	@ unaligned
	ldr	r2, .L123+12
	str	r3, [sp, #16]
	ldr	r3, [r0, #448]
	str	r7, [sp, #4]
	strd	r0, r3, [sp, #8]
	mov	r3, #492
	movs	r0, #23
	str	r3, [sp]
	ldr	r3, .L123+4
	bl	vlog
	ldr	r6, [r6, #8]	@ unaligned
	cbnz	r6, .L118
	mov	r0, r5
	add	r2, r5, #396
	mov	r1, r4
	bl	mpi_node_release_buffer_flag
	ldr	r0, [r5, #76]
	add	r0, r4, r0, lsl #3
	adds	r0, r0, #52
.L122:
	bl	list_del
.L120:
	movs	r0, #0
	b	.L113
.L118:
	cmp	r6, #1
	bne	.L119
	mov	r0, r5
	add	r2, r5, #400
	mov	r1, r4
	bl	mpi_node_release_buffer_flag
	ldr	r0, [r5, #76]
	add	r0, r4, r0, lsl #3
	adds	r0, r0, #180
	b	.L122
.L119:
	cmp	r6, #2
	bne	.L120
	mov	r0, r5
	movs	r2, #0
	mov	r1, r4
	bl	mpi_node_release_buffer_flag
	ldr	r0, [r5, #76]
	add	r0, r4, r0, lsl #3
	add	r0, r0, #308
	b	.L122
.L117:
	mov	r3, #504
	ldr	r2, .L123+16
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L123+4
	movs	r0, #23
	bl	vlog
	b	.L120
.L124:
	.align	2
.L123:
	.word	.LC19
	.word	.LANCHOR0+125
	.word	.LC15
	.word	.LC25
	.word	.LC26
	.fnend
	.size	aiisp_mb_release, .-aiisp_mb_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_dev_open, %function
aiisp_dev_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L126
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	adds	r0, r3, #4
	str	r0, [r1, #128]
	movs	r2, #12
	str	r2, [r3, #172]
	bl	mpi_obj_ref
	mov	r3, #604
	ldr	r2, .L126+4
	movs	r1, #4
	movs	r0, #23
	str	r3, [sp]
	ldr	r3, .L126+8
	bl	vlog
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L127:
	.align	2
.L126:
	.word	.LANCHOR1
	.word	.LC27
	.word	.LANCHOR0+142
	.fnend
	.size	aiisp_dev_open, .-aiisp_dev_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_dev_destroy_ctx, %function
aiisp_dev_destroy_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	cbz	r0, .L129
	bl	kfree
.L129:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	aiisp_dev_destroy_ctx, .-aiisp_dev_destroy_ctx
	.align	1
	.global	aiisp_node_exist
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_node_exist, %function
aiisp_node_exist:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	ldr	r3, [r1]	@ unaligned
	cmp	r3, #32
	beq	.L134
	movw	r3, #345
	ldr	r2, .L136
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L136+4
	movs	r0, #23
	bl	vlog
	mov	r0, #-1
.L133:
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L134:
	ldr	r2, [r1, #4]	@ unaligned
	movs	r0, #23
	movs	r1, #0
	bl	vsys_get_node_from_map
	clz	r0, r0
	lsrs	r0, r0, #5
	rsbs	r0, r0, #0
	b	.L133
.L137:
	.align	2
.L136:
	.word	.LC3
	.word	.LANCHOR0+157
	.fnend
	.size	aiisp_node_exist, .-aiisp_node_exist
	.align	1
	.global	aiisp_pool_attach
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_pool_attach, %function
aiisp_pool_attach:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r4, r1
	ldr	r3, [r1]	@ unaligned
	cmp	r3, #32
	beq	.L139
	mov	r3, #516
	ldr	r2, .L148
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L148+4
	movs	r0, #23
	bl	vlog
.L143:
	mov	r0, #-1
	b	.L138
.L139:
	ldr	r2, [r1, #4]	@ unaligned
	movs	r0, #23
	movs	r1, #0
	bl	vsys_get_node_from_map
	mov	r5, r0
	cmp	r0, #0
	beq	.L143
	ldr	r3, [r4, #24]	@ unaligned
	cmp	r3, #3
	beq	.L141
.L147:
	movs	r0, #0
.L138:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L141:
	ldr	r0, [r4, #28]	@ unaligned
	bl	vsys_get_pool
	mov	r6, r0
	cmp	r0, #0
	beq	.L147
	mov	r1, r5
	bl	mpi_buf_pool_add_user
	ldr	r3, [r4, #28]	@ unaligned
	ldr	r2, .L148+8
	movs	r1, #4
	movs	r0, #23
	strd	r6, r3, [sp, #8]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #531
	str	r3, [sp]
	ldr	r3, .L148+4
	bl	vlog
	b	.L147
.L149:
	.align	2
.L148:
	.word	.LC3
	.word	.LANCHOR0+174
	.word	.LC28
	.fnend
	.size	aiisp_pool_attach, .-aiisp_pool_attach
	.align	1
	.global	aiisp_node_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_node_start, %function
aiisp_node_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r1]	@ unaligned
	cmp	r3, #32
	beq	.L151
	movs	r3, #247
	ldr	r2, .L155
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L155+4
	movs	r0, #23
	bl	vlog
.L154:
	mov	r0, #-1
.L150:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L151:
	ldr	r2, [r1, #4]	@ unaligned
	movs	r0, #23
	movs	r1, #0
	bl	vsys_get_node_from_map
	cbnz	r0, .L153
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #2
	ldr	r2, .L155+8
	movs	r0, #23
	str	r3, [sp, #4]
	movs	r3, #254
	str	r3, [sp]
	ldr	r3, .L155+4
	bl	vlog
	b	.L154
.L153:
	bl	mpi_node_streamon
	ldr	r3, [r4, #4]	@ unaligned
	ldr	r2, .L155+12
	movs	r1, #4
	movs	r0, #23
	str	r3, [sp, #4]
	movw	r3, #263
	str	r3, [sp]
	ldr	r3, .L155+4
	bl	vlog
	ldr	r3, [r4, #4]	@ unaligned
	ldr	r2, .L155+16
	movs	r1, #4
	movs	r0, #23
	str	r3, [sp, #4]
	mov	r3, #266
	str	r3, [sp]
	ldr	r3, .L155+4
	bl	vlog
	movs	r0, #0
	b	.L150
.L156:
	.align	2
.L155:
	.word	.LC3
	.word	.LANCHOR0+192
	.word	.LC15
	.word	.LC29
	.word	.LC30
	.fnend
	.size	aiisp_node_start, .-aiisp_node_start
	.align	1
	.global	aiisp_node_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_node_stop, %function
aiisp_node_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	mov	r4, r1
	ldr	r3, [r1]	@ unaligned
	cmp	r3, #32
	beq	.L158
	mov	r3, #282
	ldr	r2, .L164
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L164+4
	movs	r0, #23
	bl	vlog
	mov	r0, #-1
.L157:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L158:
	ldr	r2, [r1, #4]	@ unaligned
	movs	r0, #23
	movs	r1, #0
	bl	vsys_get_node_from_map
	cbz	r0, .L160
	bl	mpi_node_streamoff
	movs	r3, #0
	ldr	r2, .L164+8
	movs	r1, #4
	movs	r0, #23
	str	r3, [sp, #8]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #290
	str	r3, [sp]
	ldr	r3, .L164+4
	bl	vlog
.L160:
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	ldr	r2, .L164+12
	movs	r0, #23
	str	r3, [sp, #4]
	movw	r3, #293
	str	r3, [sp]
	ldr	r3, .L164+4
	bl	vlog
	movs	r0, #0
	b	.L157
.L165:
	.align	2
.L164:
	.word	.LC3
	.word	.LANCHOR0+209
	.word	.LC31
	.word	.LC32
	.fnend
	.size	aiisp_node_stop, .-aiisp_node_stop
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_dev_ioctl, %function
aiisp_dev_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	uxtb	r4, r1
	ldr	r8, [r0, #128]
	movw	r3, #567
	mov	r5, r0
	str	r3, [sp]
	mov	r6, r1
	mov	r7, r2
	movs	r1, #5
	strd	r4, r8, [sp, #8]
	str	r4, [sp, #4]
	movs	r0, #23
	ldr	r3, .L170
	ldr	r2, .L170+4
	bl	vlog
	ldr	r1, [r8, #168]
	cmp	r1, r4
	bhi	.L167
	movw	r3, #570
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L170
	movs	r0, #23
	ldr	r2, .L170+8
	bl	vlog
.L168:
.L166:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L167:
	.syntax unified
@ 87 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
	cmp	r4, r1
	sbc	r1, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r4, r4, r1
	ldr	r1, [r8, #164]
	movs	r3, #20
	mla	r1, r3, r4, r1
	cbnz	r1, .L169
	movw	r3, #577
	ldr	r2, .L170+12
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L170
	movs	r0, #23
	bl	vlog
	b	.L166
.L169:
	mov	r3, r7
	mov	r2, r6
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L171:
	.align	2
.L170:
	.word	.LANCHOR0+225
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.fnend
	.size	aiisp_dev_ioctl, .-aiisp_dev_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_dev_create_ctx, %function
aiisp_dev_create_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r1, #3520
	ldr	r3, .L177
	ldr	r0, [r3, #20]
	bl	kmem_cache_alloc
	cbz	r0, .L172
	add	r3, r0, #8
	str	r0, [r0]
	str	r3, [r0, #12]
	str	r3, [r0, #8]
	movs	r3, #0
	str	r0, [r0, #4]
	strd	r3, r3, [r0, #16]
.L172:
	pop	{r3, pc}
.L178:
	.align	2
.L177:
	.word	kmalloc_caches
	.fnend
	.size	aiisp_dev_create_ctx, .-aiisp_dev_create_ctx
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_dev_release, %function
aiisp_dev_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r4, [r1, #128]
	mov	r0, r4
	bl	mpi_obj_unref
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bne	.L180
	ldr	r7, .L207
	movs	r5, #0
	mov	r6, #256
.L196:
	mov	r2, r5
	movs	r1, #0
	movs	r0, #23
	bl	vsys_get_node_from_map
	mov	r4, r0
	cbz	r0, .L181
	ldr	r9, [r0, #60]
	ldr	r8, [r0, #288]
	cmp	r9, #0
	beq	.L182
	ldr	r3, [r9]
	movs	r1, #4
	str	r5, [sp, #4]
	movs	r0, #23
	ldr	r2, .L207+4
	strd	r9, r3, [sp, #8]
	movw	r3, #635
	str	r3, [sp]
	mov	r3, r7
	bl	vlog
	mov	r1, r4
	mov	r0, r9
	bl	mpi_buf_pool_del_user
	cmp	r8, #0
	bne	.L183
.L189:
	mov	r2, r5
	movs	r1, #0
	movs	r0, #23
	bl	vsys_remove_node_from_map
	ldr	r3, [r4, #324]
	cbz	r3, .L185
	mov	r0, r4
	bl	mpi_node_del_in_watcher
.L185:
	mov	r0, r4
	bl	mpi_node_destroy
	movw	r3, #663
	ldr	r2, .L207+8
	movs	r1, #4
	movs	r0, #23
	str	r3, [sp]
	mov	r3, r7
	str	r5, [sp, #4]
	bl	vlog
.L181:
	adds	r5, r5, #1
	cmp	r5, #64
	bne	.L196
	movw	r3, #667
	ldr	r2, .L207+12
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L207
	movs	r0, #23
	bl	vlog
.L180:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L183:
	ldr	r3, [r8, #20]
	cmp	r3, #2
	bne	.L187
	movs	r1, #1
	mov	r0, r9
	bl	mpi_buf_pool_destroy
.L187:
	ldr	r2, [r4, #76]
	mvn	r10, #43
	ldr	r0, [r8]
	mov	fp, #290
	add	r9, r2, #1
	sub	r3, r10, r9, lsl #3
	add	r0, r0, r3
	add	r2, r0, r2, lsl #3
	ldr	r2, [r2, #52]
	add	r9, r2, r3
.L190:
	ldr	r3, [r4, #76]
	add	r2, r0, r3, lsl #3
	adds	r2, r2, #52
	cmp	r8, r2
	bne	.L192
	add	r1, r3, #33
	ldr	r0, [r8, #8]!
	mvn	r2, #43
	mov	r10, #290
	sub	r2, r2, r1, lsl #3
	add	r0, r0, r2
	add	r3, r0, r3, lsl #3
	ldr	r3, [r3, #308]
	add	r9, r3, r2
.L193:
	ldr	r3, [r4, #76]
	add	r2, r0, r3, lsl #3
	add	r2, r2, #308
	cmp	r2, r8
	beq	.L189
	cbz	r0, .L194
	add	r3, r0, r3, lsl #3
	ldrd	r1, r2, [r3, #308]
	str	r2, [r1, #4]
	str	r1, [r2]
	strd	r6, r10, [r3, #308]
	bl	mpi_obj_unref
.L194:
	ldr	r3, [r4, #76]
	mvn	r2, #43
	mov	r0, r9
	add	r1, r3, #33
	add	r3, r9, r3, lsl #3
	sub	r2, r2, r1, lsl #3
	ldr	r3, [r3, #308]
	add	r9, r3, r2
	b	.L193
.L182:
	cmp	r8, #0
	beq	.L189
	b	.L187
.L192:
	cbz	r0, .L191
	add	r3, r0, r3, lsl #3
	ldrd	r1, r2, [r3, #52]
	str	r2, [r1, #4]
	str	r1, [r2]
	strd	r6, fp, [r3, #52]
	bl	mpi_obj_unref
.L191:
	ldr	r3, [r4, #76]
	mov	r0, r9
	adds	r2, r3, #1
	add	r3, r9, r3, lsl #3
	sub	r2, r10, r2, lsl #3
	ldr	r3, [r3, #52]
	add	r9, r3, r2
	b	.L190
.L208:
	.align	2
.L207:
	.word	.LANCHOR0+241
	.word	.LC36
	.word	.LC12
	.word	.LC37
	.fnend
	.size	aiisp_dev_release, .-aiisp_dev_release
	.align	1
	.global	aiisp_dev_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_dev_probe, %function
aiisp_dev_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movs	r2, #184
	ldr	r4, .L210
	mov	r6, r0
	movs	r1, #0
	ldr	r5, .L210+4
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r6
	bl	kstrdup
	movs	r3, #1
	str	r3, [r4, #180]
	add	r3, r5, #260
	str	r3, [r4, #168]
	movs	r3, #12
	str	r3, [r4, #172]
	mov	r2, r6
	ldr	r3, .L210+8
	movs	r1, #8
	str	r4, [r4, #164]
	adds	r4, r4, #4
	str	r3, [r4, #68]
	ldr	r3, .L210+12
	str	r3, [r4, #76]
	ldr	r3, .L210+16
	str	r3, [r4, #80]
	str	r0, [r4, #172]
	mov	r0, r4
	bl	mpi_dev_register
	add	r1, r5, #500
	mov	r0, r4
	movs	r2, #0
	pop	{r4, r5, r6, lr}
	b	mpi_sysfs_dev_add
.L211:
	.align	2
.L210:
	.word	.LANCHOR1
	.word	.LANCHOR0
	.word	aiisp_dev_create_ctx
	.word	aiisp_dev_destroy_ctx
	.word	aiisp_handle_message
	.fnend
	.size	aiisp_dev_probe, .-aiisp_dev_probe
	.align	1
	.global	aiisp_dev_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_dev_shutdown, %function
aiisp_dev_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r3, #1
	ldr	r4, .L213
	ldr	r0, [r4, #176]
	str	r3, [r4]
	bl	kfree
	adds	r0, r4, #4
	pop	{r4, lr}
	b	mpi_dev_unregister
.L214:
	.align	2
.L213:
	.word	.LANCHOR1
	.fnend
	.size	aiisp_dev_shutdown, .-aiisp_dev_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	aiisp_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_init, %function
aiisp_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L216
	bl	aiisp_dev_probe
	movs	r0, #0
	pop	{r3, pc}
.L217:
	.align	2
.L216:
	.word	.LC9
	.fnend
	.size	aiisp_init, .-aiisp_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	aiisp_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	aiisp_exit, %function
aiisp_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	aiisp_dev_shutdown
	.fnend
	.size	aiisp_exit, .-aiisp_exit
	.global	g_aiisp_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.35955, %object
	.size	__func__.35955, 17
__func__.35955:
	.ascii	"aiisp_node_reset\000"
	.type	__func__.35885, %object
	.size	__func__.35885, 18
__func__.35885:
	.ascii	"aiisp_node_create\000"
	.type	__func__.35898, %object
	.size	__func__.35898, 19
__func__.35898:
	.ascii	"aiisp_node_destroy\000"
	.type	__func__.35963, %object
	.size	__func__.35963, 18
__func__.35963:
	.ascii	"aiisp_node_bypass\000"
	.type	__func__.35980, %object
	.size	__func__.35980, 13
__func__.35980:
	.ascii	"aiisp_mb_get\000"
	.type	__func__.35997, %object
	.size	__func__.35997, 14
__func__.35997:
	.ascii	"aiisp_mb_send\000"
	.type	__func__.36136, %object
	.size	__func__.36136, 26
__func__.36136:
	.ascii	"aiisp_handle_node_message\000"
	.type	__func__.36010, %object
	.size	__func__.36010, 17
__func__.36010:
	.ascii	"aiisp_mb_release\000"
	.type	__func__.36067, %object
	.size	__func__.36067, 15
__func__.36067:
	.ascii	"aiisp_dev_open\000"
	.type	__func__.35970, %object
	.size	__func__.35970, 17
__func__.35970:
	.ascii	"aiisp_node_exist\000"
	.type	__func__.36022, %object
	.size	__func__.36022, 18
__func__.36022:
	.ascii	"aiisp_pool_attach\000"
	.type	__func__.35941, %object
	.size	__func__.35941, 17
__func__.35941:
	.ascii	"aiisp_node_start\000"
	.type	__func__.35949, %object
	.size	__func__.35949, 16
__func__.35949:
	.ascii	"aiisp_node_stop\000"
	.type	__func__.36040, %object
	.size	__func__.36040, 16
__func__.36040:
	.ascii	"aiisp_dev_ioctl\000"
	.type	__func__.36083, %object
	.size	__func__.36083, 18
__func__.36083:
	.ascii	"aiisp_dev_release\000"
	.space	1
	.type	aiisp_ioctls, %object
	.size	aiisp_ioctls, 240
aiisp_ioctls:
	.word	-1068729856
	.word	0
	.word	aiisp_get_version
	.word	.LC38
	.word	0
	.word	-1071613439
	.word	0
	.word	aiisp_node_create
	.word	.LC39
	.word	0
	.word	-1071613438
	.word	0
	.word	aiisp_node_destroy
	.word	.LC40
	.word	0
	.word	-1071613437
	.word	0
	.word	aiisp_node_start
	.word	.LC41
	.word	0
	.word	-1071613436
	.word	0
	.word	aiisp_node_stop
	.word	.LC42
	.word	0
	.word	-1071613435
	.word	0
	.word	aiisp_node_reset
	.word	.LC43
	.word	0
	.word	-1071613434
	.word	0
	.word	aiisp_node_bypass
	.word	.LC44
	.word	0
	.word	-1071613433
	.word	0
	.word	aiisp_node_exist
	.word	.LC45
	.word	0
	.word	-1066632696
	.word	16
	.word	aiisp_mb_get
	.word	.LC46
	.word	0
	.word	-1066632695
	.word	0
	.word	aiisp_mb_send
	.word	.LC47
	.word	0
	.word	-1066632694
	.word	0
	.word	aiisp_mb_release
	.word	.LC48
	.word	0
	.word	-1071613429
	.word	0
	.word	aiisp_pool_attach
	.word	.LC49
	.word	0
	.type	aiisp_driver_fops, %object
	.size	aiisp_driver_fops, 128
aiisp_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	0
	.word	aiisp_dev_ioctl
	.word	aiisp_compat_ioctl
	.word	0
	.space	4
	.word	aiisp_dev_open
	.space	4
	.word	aiisp_dev_release
	.space	60
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	g_aiisp_dev, %object
	.size	g_aiisp_dev, 184
g_aiisp_dev:
	.space	184
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"-2024-05-24-16:21:01\000"
.LC1:
	.ascii	"vmpi:28ee389dbc33c2c7db812cd08e54fd3a0f2f992b-v1.4."
	.ascii	"81\000"
.LC2:
	.ascii	"\033[1;32mAIISP Dev %d reset node done!\033[0m\000"
.LC3:
	.ascii	"\033[1;31mstruct aiisp_node_info mismatching\033[0m"
	.ascii	"\000"
.LC4:
	.ascii	"valloc\000"
.LC5:
	.ascii	"\033[1;31mAIISP Dev %d getting allocator failed!\033"
	.ascii	"[0m\000"
.LC6:
	.ascii	"\033[1;32mAIISP Dev %d node have exist. nick_id %d\033"
	.ascii	"[0m\000"
.LC7:
	.ascii	"\033[1;31mAIISP Dev %d create aiisp node failed! ni"
	.ascii	"ck_id %d\033[0m\000"
.LC8:
	.ascii	"\033[1;32mAIISP Dev %d node create success! nick_id"
	.ascii	" %d\033[0m\000"
.LC9:
	.ascii	"aiisp\000"
.LC10:
	.ascii	"\033[1;32mAIISP Dev %d create MB_SOURCE_PRIVATE suc"
	.ascii	"cess! pool %p pool_id %d bufffer_cnt %d bufffer_siz"
	.ascii	"e %d\033[0m\000"
.LC11:
	.ascii	"\033[1;31mAIISP Dev %d pool create failed!\033[0m\000"
.LC12:
	.ascii	"\033[1;32mAIISP Dev %d node destroy success!\033[0m"
	.ascii	"\000"
.LC13:
	.ascii	"\033[1;32mAIISP Dev %d attach MB_SOURCE_USER succes"
	.ascii	"s! pool %p pool_id %d\033[0m\000"
.LC14:
	.ascii	"\033[1;32mAIISP Dev %d create node done!\033[0m\000"
.LC15:
	.ascii	"\033[1;31mAIISP Dev %d get node failed!\033[0m\000"
.LC16:
	.ascii	"\033[1;32mAIISP Dev %d destroy node done!\033[0m\000"
.LC17:
	.ascii	"\033[1;32mAIISP Dev %d bypass %d node success!\033["
	.ascii	"0m\000"
.LC18:
	.ascii	"\033[1;32mAIISP Dev %d bypass node done!\033[0m\000"
.LC19:
	.ascii	"\033[1;31mstruct aiisp_buff_info mismatching\033[0m"
	.ascii	"\000"
.LC20:
	.ascii	"AIISP Dev %d outof mpi_node_get_current_in_buffer m"
	.ascii	"pi_buf(%p), buf_id(%d), buf_refcnt(%d).\000"
.LC21:
	.ascii	"AIISP Dev %d outof mpi_node_request_buf mpi_buf(%p)"
	.ascii	", buf_id(%d), buf_refcnt(%d).\000"
.LC22:
	.ascii	"\033[1;31mAIISP Dev %d cannot find mpi_buf fr fd %d"
	.ascii	"\033[0m\000"
.LC23:
	.ascii	"AIISP Dev %d mpi_buf(%p), buf_id(%d), buf_refcnt(%d"
	.ascii	"), dma_buf(%p), fd(%d), access(%d).\000"
.LC24:
	.ascii	"aiisp(%s-%d) buf(%d) ready\000"
.LC25:
	.ascii	"AIISP Dev %d put mpi_buf(%p), dma_buf(%p), buf_id(%"
	.ascii	"ld), buf_refcnt(%d), access(%d).\000"
.LC26:
	.ascii	"\033[1;31mbuf null\033[0m\000"
.LC27:
	.ascii	"\033[1;32m--------- aiisp dev open ----------\033[0"
	.ascii	"m\000"
.LC28:
	.ascii	"\033[1;32mAIISP Dev %d attach MB_SOURCE_USER succes"
	.ascii	"s! pool %p pool_id %llu\033[0m\000"
.LC29:
	.ascii	"\033[1;32mAIISP Dev %d node stream on success!\033["
	.ascii	"0m\000"
.LC30:
	.ascii	"\033[1;32mAIISP Dev %d start node done!\033[0m\000"
.LC31:
	.ascii	"\033[1;32mAIISP Dev %d node stream off success!\033"
	.ascii	"[0m\000"
.LC32:
	.ascii	"\033[1;32mAIISP Dev %d stop node done!\033[0m\000"
.LC33:
	.ascii	"nr = %d, index = %d dev = %p\000"
.LC34:
	.ascii	"\033[1;31merror: index = %d\033[0m\000"
.LC35:
	.ascii	"\033[1;31merror: ioctl is null\033[0m\000"
.LC36:
	.ascii	"\033[1;32mAIISP DEV %d destroy MB_SOURCE_PRIVATE su"
	.ascii	"ccess! pool %p pool_id %d.\033[0m\000"
.LC37:
	.ascii	"\033[1;32m--------- aiisp dev release ----------\033"
	.ascii	"[0m\000"
.LC38:
	.ascii	"AIISP_IOCTL_VERSION\000"
.LC39:
	.ascii	"AIISP_IOCTL_NODE_CREATE\000"
.LC40:
	.ascii	"AIISP_IOCTL_NODE_DESTROY\000"
.LC41:
	.ascii	"AIISP_IOCTL_NODE_START\000"
.LC42:
	.ascii	"AIISP_IOCTL_NODE_STOP\000"
.LC43:
	.ascii	"AIISP_IOCTL_NODE_RESET\000"
.LC44:
	.ascii	"AIISP_IOCTL_NODE_BYPASS\000"
.LC45:
	.ascii	"AIISP_IOCTL_NODE_EXIST\000"
.LC46:
	.ascii	"AIISP_IOCTL_MB_GET\000"
.LC47:
	.ascii	"AIISP_IOCTL_MB_SEND\000"
.LC48:
	.ascii	"AIISP_IOCTL_MB_RELEASE\000"
.LC49:
	.ascii	"AIISP_IOCTL_POOL_ATTACH\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
