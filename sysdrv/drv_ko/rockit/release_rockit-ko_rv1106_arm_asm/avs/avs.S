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
	.file	"avs.c"
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
	.type	avs_compat_ioctl, %function
avs_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	avs_compat_ioctl, .-avs_compat_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_handle_message, %function
avs_handle_message:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	avs_handle_message, .-avs_handle_message
	.align	1
	.global	avs_get_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_get_version, %function
avs_get_version:
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
	bls	.L5
	movs	r3, #15
	strb	r5, [r4, #17]
	strb	r3, [r4, #16]
	strb	r5, [r4, #18]
	strb	r5, [r4, #19]
.L5:
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
	ldr	r1, .L6
	add	r0, r4, #40
	bl	strncpy
	ldr	r2, [r4, #56]	@ unaligned
	ldr	r1, .L6+4
	add	r0, r4, #60
	bl	strncpy
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L7:
	.align	2
.L6:
	.word	.LC0
	.word	.LC1
	.fnend
	.size	avs_get_version, .-avs_get_version
	.align	1
	.global	avs_node_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_node_reset, %function
avs_node_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	movs	r0, #17
	ldr	r3, [r1, #4]	@ unaligned
	movs	r1, #4
	ldr	r2, .L9
	str	r3, [sp, #4]
	mov	r3, #402
	str	r3, [sp]
	ldr	r3, .L9+4
	bl	vlog
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L10:
	.align	2
.L9:
	.word	.LC2
	.word	.LANCHOR0
	.fnend
	.size	avs_node_reset, .-avs_node_reset
	.align	1
	.global	avs_node_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_node_create, %function
avs_node_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #44
	sub	sp, sp, #44
	ldr	r3, [r0, #128]
	movs	r5, #0
	mov	r4, r1
	strd	r5, r5, [sp, #24]
	strd	r5, r5, [sp, #32]
	str	r3, [sp, #20]
	ldr	r3, [r1]	@ unaligned
	cmp	r3, #48
	beq	.L12
	movs	r3, #94
	ldr	r2, .L37
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L37+4
	movs	r0, #17
	bl	vlog
.L36:
	mov	r0, #-1
	b	.L11
.L12:
	add	r8, sp, #24
	ldr	r10, .L37+4
	ldr	fp, .L37+36
	mov	r9, r8
.L18:
	ldr	r1, [r4, #4]	@ unaligned
	lsls	r7, r5, #8
	mov	r2, r5
	movs	r0, #17
	orr	r7, r7, r1, lsl #16
	bl	vsys_get_node_from_map
	add	r7, r7, #1024
	mov	r3, r0
	str	r0, [r9], #4
	cmp	r0, #0
	beq	.L14
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	strd	r5, r7, [sp, #8]
	movs	r0, #17
	ldr	r2, .L37+8
	str	r3, [sp, #4]
	movs	r3, #104
	str	r3, [sp]
	mov	r3, r10
	bl	vlog
.L15:
	cbz	r5, .L26
	ldr	r9, .L37+4
	movs	r6, #0
	ldr	fp, .L37+40
.L23:
	ldr	r1, [r4, #4]	@ unaligned
	add	r10, r6, #2
	movs	r0, #17
	mov	r2, r10
	orr	r7, r6, r1, lsl #16
	bl	vsys_get_node_from_map
	mov	r3, r0
	cmp	r0, #0
	beq	.L19
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	strd	r6, r7, [sp, #8]
	movs	r0, #17
	ldr	r2, .L37+12
	str	r3, [sp, #4]
	movs	r3, #128
	str	r3, [sp]
	mov	r3, r9
	bl	vlog
.L20:
	cmp	r6, #0
	beq	.L27
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	ldr	r2, .L37+16
	movs	r0, #17
	str	r3, [sp, #4]
	movs	r3, #146
	str	r3, [sp]
	ldr	r3, .L37+4
	bl	vlog
	movs	r0, #0
.L11:
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L26:
	movs	r5, #1
	b	.L18
.L14:
	mov	r2, r0
	mov	r1, r7
	ldr	r0, [sp, #20]
	bl	mpi_dev_create_node_with_params
	str	r0, [r9, #-4]
	mov	r6, r0
	ldr	r3, [r4, #4]	@ unaligned
	strd	r5, r7, [sp, #8]
	str	r3, [sp, #4]
	cbnz	r0, .L16
	movs	r3, #110
	ldr	r2, .L37+20
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L37+4
	movs	r0, #17
	bl	vlog
	ldr	r8, .L37+4
	ldr	r9, .L37+44
	add	r5, sp, #32
.L17:
	ldr	r7, [r5], #4
	cbz	r7, .L25
	adds	r2, r6, #2
	ldr	r1, [r4, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_remove_node_from_map
	mov	r0, r7
	bl	mpi_node_destroy
	movs	r3, #0
	str	r3, [r5, #-4]
	mov	r2, r9
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp, #4]
	movs	r3, #167
	str	r6, [sp, #8]
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
.L25:
	cmp	r6, #0
	bne	.L36
	movs	r6, #1
	b	.L17
.L16:
	mov	r2, fp
	movs	r3, #113
	movs	r1, #4
	str	r3, [sp]
	movs	r0, #17
	mov	r3, r10
	bl	vlog
	ldr	r3, .L37+24
	mov	r0, r6
	movs	r1, #2
	str	r3, [r6, #4]
	bl	mpi_node_add_in_watcher
	mov	r3, r6
	mov	r2, r5
	ldr	r1, [r4, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_add_node_to_map
	b	.L15
.L27:
	movs	r6, #1
	b	.L23
.L19:
	mov	r2, r0
	mov	r1, r7
	ldr	r0, [sp, #20]
	bl	mpi_dev_create_node_with_params
	ldr	r3, [r4, #4]	@ unaligned
	mov	r5, r0
	strd	r6, r7, [sp, #8]
	str	r3, [sp, #4]
	cbnz	r0, .L21
	movs	r3, #134
	ldr	r2, .L37+28
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L37+4
	movs	r0, #17
	bl	vlog
	ldr	r7, .L37+4
	ldr	r9, .L37+48
.L22:
	ldr	r6, [r8], #4
	cbz	r6, .L24
	mov	r0, r6
	bl	mpi_node_del_in_watcher
	mov	r2, r5
	ldr	r1, [r4, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_remove_node_from_map
	mov	r0, r6
	bl	mpi_node_destroy
	movs	r3, #0
	str	r3, [r8, #-4]
	mov	r2, r9
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp, #4]
	movs	r3, #157
	str	r5, [sp, #8]
	str	r3, [sp]
	mov	r3, r7
	bl	vlog
.L24:
	cbz	r5, .L28
	mvn	r0, #1
	b	.L11
.L21:
	mov	r2, fp
	movs	r3, #138
	movs	r1, #4
	str	r3, [sp]
	movs	r0, #17
	mov	r3, r9
	bl	vlog
	ldr	r3, .L37+32
	mov	r2, r10
	movs	r0, #17
	str	r3, [r5, #4]
	mov	r3, r5
	ldr	r1, [r4, #4]	@ unaligned
	bl	vsys_add_node_to_map
	b	.L20
.L28:
	movs	r5, #1
	b	.L22
.L38:
	.align	2
.L37:
	.word	.LC3
	.word	.LANCHOR0+15
	.word	.LC4
	.word	.LC8
	.word	.LC12
	.word	.LC5
	.word	.LC7
	.word	.LC9
	.word	.LC11
	.word	.LC6
	.word	.LC10
	.word	.LC14
	.word	.LC13
	.fnend
	.size	avs_node_create, .-avs_node_create
	.align	1
	.global	avs_node_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_node_start, %function
avs_node_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #60
	sub	sp, sp, #60
	ldr	r3, [r1]	@ unaligned
	add	r7, sp, #48
	movs	r4, #0
	mov	r5, r1
	cmp	r3, #48
	strd	r4, r4, [sp, #40]
	str	r4, [sp, #48]
	str	r4, [r7, #4]
	beq	.L40
	mov	r3, #256
	ldr	r2, .L83
	str	r3, [sp]
	ldr	r3, .L83+4
.L82:
	movs	r1, #2
	movs	r0, #17
	bl	vlog
.L81:
	mov	r0, #-1
	b	.L39
.L40:
	ldr	r0, .L83+8
	bl	mpi_allocator_find_by_name
	str	r0, [sp, #32]
	cbnz	r0, .L42
	movw	r3, #263
	ldr	r2, .L83+12
	str	r3, [sp]
	ldr	r3, .L83+4
	b	.L82
.L42:
	add	r8, sp, #40
	ldr	fp, .L83+4
	mov	r10, r8
.L50:
	adds	r2, r4, #2
	ldr	r1, [r5, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r9, r0
	str	r0, [r10], #4
	cbnz	r0, .L43
	str	r4, [sp, #8]
	ldr	r3, [r5, #4]	@ unaligned
	ldr	r2, .L83+16
	str	r3, [sp, #4]
	movw	r3, #271
	str	r3, [sp]
	ldr	r3, .L83+4
.L80:
	movs	r1, #2
	movs	r0, #17
	bl	vlog
	movs	r6, #0
.L58:
	ldr	r5, [r7], #4
	ldr	r4, [r8], #4
	cbz	r5, .L55
	cbz	r4, .L56
	mov	r1, r4
	mov	r0, r5
	bl	mpi_buf_pool_del_user
.L56:
	movs	r1, #1
	mov	r0, r5
	bl	mpi_buf_pool_destroy
.L55:
	cbz	r4, .L57
	mov	r0, r4
	bl	mpi_node_del_out_watcher
.L57:
	cmp	r6, #0
	bne	.L81
	movs	r6, #1
	b	.L58
.L59:
	movs	r4, #1
	b	.L50
.L43:
	add	r6, r5, r4, lsl #2
	ldr	r3, [r6, #32]	@ unaligned
	cmp	r3, #2
	bne	.L45
	ldr	r2, [r6, #24]	@ unaligned
	cbnz	r2, .L46
	movs	r3, #1
	str	r3, [r6, #16]	@ unaligned
.L46:
	ldr	r3, .L83+20
	movs	r1, #1
	ldr	r0, [sp, #32]
	str	r3, [sp]
	ldr	r3, [r6, #16]	@ unaligned
	bl	mpi_buf_pool_create
	str	r0, [r7, r4, lsl #2]
	str	r0, [sp, #36]
	cmp	r0, #0
	beq	.L47
	mov	r1, r9
	bl	mpi_buf_pool_add_user
	ldr	r2, [r6, #24]	@ unaligned
	movs	r1, #4
	movs	r0, #17
	str	r2, [sp, #24]
	ldr	r3, [sp, #36]
	ldr	r2, [r6, #16]	@ unaligned
	str	r2, [sp, #20]
	ldr	r2, [r3]
	str	r4, [sp, #8]
	strd	r3, r2, [sp, #12]
	ldr	r3, [r5, #4]	@ unaligned
	ldr	r2, .L83+24
	str	r3, [sp, #4]
	movw	r3, #289
	str	r3, [sp]
	mov	r3, fp
	bl	vlog
.L48:
	ldr	r1, [r6, #8]	@ unaligned
	mov	r0, r9
	bl	mpi_node_add_out_watcher
	cmp	r4, #0
	beq	.L59
	ldr	r0, [sp, #40]
	cbz	r0, .L51
	bl	mpi_node_streamon
	movs	r3, #0
	ldr	r2, .L83+28
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp, #8]
	ldr	r3, [r5, #4]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #314
	str	r3, [sp]
	ldr	r3, .L83+4
	bl	vlog
.L51:
	ldr	r0, [sp, #44]
	cbz	r0, .L52
	bl	mpi_node_streamon
	movs	r3, #1
	ldr	r2, .L83+28
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp, #8]
	ldr	r3, [r5, #4]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #314
	str	r3, [sp]
	ldr	r3, .L83+4
	bl	vlog
.L52:
	ldr	r6, .L83+4
	movs	r4, #0
	ldr	r7, .L83+32
.L54:
	mov	r2, r4
	ldr	r1, [r5, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_get_node_from_map
	cbz	r0, .L53
	bl	mpi_node_streamon
	mov	r2, r7
	movs	r1, #4
	movs	r0, #17
	str	r4, [sp, #8]
	ldr	r3, [r5, #4]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #323
	str	r3, [sp]
	mov	r3, r6
	bl	vlog
.L53:
	cmp	r4, #0
	beq	.L60
	ldr	r3, [r5, #4]	@ unaligned
	movs	r1, #4
	ldr	r2, .L83+36
	movs	r0, #17
	str	r3, [sp, #4]
	mov	r3, #328
	str	r3, [sp]
	ldr	r3, .L83+4
	bl	vlog
	movs	r0, #0
.L39:
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L47:
	str	r4, [sp, #8]
	ldr	r3, [r5, #4]	@ unaligned
	ldr	r2, .L83+40
	str	r3, [sp, #4]
	movw	r3, #291
	str	r3, [sp]
	ldr	r3, .L83+4
	b	.L80
.L45:
	cmp	r3, #3
	bne	.L48
	ldr	r0, [r6, #40]	@ unaligned
	bl	vsys_get_pool
	str	r0, [r7, r4, lsl #2]
	str	r0, [sp, #36]
	cbz	r0, .L49
	mov	r1, r9
	bl	mpi_buf_pool_add_user
	ldr	r2, [r6, #40]	@ unaligned
	movs	r1, #4
	movs	r0, #17
	ldr	r3, [sp, #36]
	str	r2, [sp, #16]
	ldr	r2, .L83+44
	strd	r4, r3, [sp, #8]
	ldr	r3, [r5, #4]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #301
	str	r3, [sp]
	mov	r3, fp
	bl	vlog
	b	.L48
.L49:
	str	r4, [sp, #8]
	ldr	r3, [r5, #4]	@ unaligned
	ldr	r2, .L83+48
	str	r3, [sp, #4]
	movw	r3, #303
	str	r3, [sp]
	ldr	r3, .L83+4
	b	.L80
.L60:
	movs	r4, #1
	b	.L54
.L84:
	.align	2
.L83:
	.word	.LC3
	.word	.LANCHOR0+31
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.fnend
	.size	avs_node_start, .-avs_node_start
	.align	1
	.global	avs_mb_get
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_mb_get, %function
avs_mb_get:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r8, r1
	ldr	r3, [r1]	@ unaligned
	.pad #36
	sub	sp, sp, #36
	cmp	r3, #116
	beq	.L86
	mov	r3, #420
	ldr	r2, .L121
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L121+4
	movs	r0, #17
	bl	vlog
.L95:
	mov	r5, #-1
	b	.L85
.L86:
	ldr	r5, [r1, #12]	@ unaligned
	ldr	r9, [r1, #4]	@ unaligned
	adds	r4, r5, #1
	ldr	r7, [r1, #8]	@ unaligned
	bne	.L102
	mov	r2, r7
	mov	r1, r9
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r6, r0
	cbnz	r0, .L88
	mov	r3, #432
	strd	r9, r7, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L121+4
	movs	r0, #17
	ldr	r2, .L121+8
	bl	vlog
.L85:
	mov	r0, r5
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L102:
	movs	r6, #0
.L88:
	adds	r0, r7, #1
	bne	.L89
	adds	r2, r5, #2
	mov	r1, r9
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r6, r0
	cbnz	r0, .L89
	movw	r3, #441
	strd	r9, r5, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L121+4
	movs	r0, #17
	ldr	r2, .L121+12
	bl	vlog
	b	.L95
.L89:
	ldr	r1, [r8, #16]	@ unaligned
	cmp	r1, #0
	bne	.L90
	ldr	r3, [r8, #20]	@ unaligned
	mov	r0, r6
	str	r3, [r6, #380]
	bl	mpi_node_get_in_buffer
	mov	r4, r0
	cmp	r0, #0
	beq	.L95
	adds	r1, r7, #1
	ldr	r1, [r0, #16]
	ittee	eq
	ldreq	r3, .L121+16
	moveq	r2, r5
	ldrne	r3, .L121+20
	movne	r2, r7
	str	r1, [sp, #24]
	ldr	r1, [r0]
	strd	r3, r2, [sp, #8]
	mov	r3, #452
	ldr	r2, .L121+24
	str	r3, [sp]
	ldr	r3, .L121+4
	strd	r0, r1, [sp, #16]
	str	r9, [sp, #4]
.L120:
	movs	r1, #5
	movs	r0, #17
	bl	vlog
	ldr	r3, [r4, #448]
	cmp	r3, #0
	beq	.L95
	ldr	r3, [r6, #288]
	cbz	r3, .L98
	ldr	r2, [r8, #16]	@ unaligned
	cmp	r2, #0
	bne	.L99
	ldr	r2, [r6, #76]
	add	r3, r3, r7, lsl #3
	ldr	r0, [r3, #4]
	add	r1, r4, r2, lsl #3
	add	r2, r4, r2, lsl #3
	adds	r1, r1, #52
	str	r1, [r3, #4]
	strd	r3, r0, [r2, #52]
.L119:
	str	r1, [r0]
.L98:
	movs	r2, #80
	add	r1, r4, #616
	add	r0, r8, #36
	movs	r5, #0
	bl	memcpy
	ldr	r3, [r4, #448]
	ldr	r3, [r3]
	str	r3, [r8, #32]	@ unaligned
	ldr	r3, [r4]
	str	r3, [r8, #24]	@ unaligned
	b	.L85
.L90:
	cmp	r1, #1
	bne	.L94
	ldr	r3, [r8, #20]	@ unaligned
	mov	r0, r6
	str	r3, [r6, #384]
	bl	mpi_node_get_out_buffer
	mov	r4, r0
	cmp	r0, #0
	beq	.L95
	ldr	r1, [r0, #16]
	adds	r2, r7, #1
	ittee	ne
	ldrne	r3, .L121+20
	movne	r2, r7
	ldreq	r3, .L121+16
	moveq	r2, r5
	str	r1, [sp, #24]
	ldr	r1, [r0]
	strd	r3, r2, [sp, #8]
	movw	r3, #459
	str	r9, [sp, #4]
	str	r3, [sp]
	strd	r0, r1, [sp, #16]
	ldr	r3, .L121+4
	ldr	r2, .L121+28
	b	.L120
.L94:
	cmp	r1, #2
	bne	.L95
	movs	r1, #0
	mov	r0, r6
	bl	mpi_node_request_buf
	mov	r4, r0
	cmp	r0, #0
	beq	.L95
	ldr	r1, [r0, #16]
	adds	r3, r7, #1
	ittee	ne
	ldrne	r3, .L121+20
	movne	r2, r7
	ldreq	r3, .L121+16
	moveq	r2, r5
	str	r1, [sp, #24]
	ldr	r1, [r0]
	strd	r3, r2, [sp, #8]
	movw	r3, #465
	str	r9, [sp, #4]
	str	r3, [sp]
	strd	r0, r1, [sp, #16]
	ldr	r3, .L121+4
	ldr	r2, .L121+32
	b	.L120
.L99:
	cmp	r2, #1
	bne	.L100
	ldr	r2, [r6, #76]
	add	r6, r3, r5, lsl #3
	adds	r5, r5, #4
	ldr	r0, [r6, #36]
	add	r1, r4, r2, lsl #3
	add	r2, r4, r2, lsl #3
	adds	r1, r1, #180
	add	r3, r3, r5, lsl #3
	str	r1, [r6, #36]
	strd	r3, r0, [r2, #180]
	b	.L119
.L100:
	cmp	r2, #2
	bne	.L98
	ldr	r2, [r6, #76]
	add	r6, r3, r5, lsl #3
	adds	r5, r5, #2
	ldr	r0, [r6, #20]
	add	r1, r4, r2, lsl #3
	add	r2, r4, r2, lsl #3
	add	r1, r1, #308
	add	r3, r3, r5, lsl #3
	str	r1, [r6, #20]
	strd	r3, r0, [r2, #308]
	b	.L119
.L122:
	.align	2
.L121:
	.word	.LC28
	.word	.LANCHOR0+46
	.word	.LC29
	.word	.LC17
	.word	.LC27
	.word	.LC26
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.fnend
	.size	avs_mb_get, .-avs_mb_get
	.align	1
	.global	avs_mb_send
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_mb_send, %function
avs_mb_send:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r7, r1
	ldr	r3, [r1]	@ unaligned
	.pad #44
	sub	sp, sp, #44
	cmp	r3, #116
	beq	.L124
	mov	r3, #502
	ldr	r2, .L137
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L137+4
	movs	r0, #17
	bl	vlog
.L135:
	mov	r4, #-1
	b	.L123
.L124:
	ldr	r4, [r1, #12]	@ unaligned
	ldr	r6, [r1, #4]	@ unaligned
	ldr	r5, [r1, #8]	@ unaligned
	adds	r1, r4, #1
	bne	.L133
	mov	r2, r5
	mov	r1, r6
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r9, r0
	cbnz	r0, .L126
	movw	r3, #514
	strd	r6, r5, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L137+4
	movs	r0, #17
	ldr	r2, .L137+8
	bl	vlog
.L123:
	mov	r0, r4
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L133:
	mov	r9, #0
.L126:
	adds	r2, r5, #1
	bne	.L127
	adds	r2, r4, #2
	mov	r1, r6
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r9, r0
	cbnz	r0, .L127
	movw	r3, #523
	strd	r6, r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L137+4
	movs	r0, #17
	ldr	r2, .L137+12
	bl	vlog
	b	.L135
.L127:
	ldr	r0, [r7, #28]	@ unaligned
	cmp	r0, #0
	bge	.L128
.L136:
	movs	r4, #0
	b	.L123
.L128:
	bl	get_mpi_buf
	ldr	r2, [r7, #28]	@ unaligned
	mov	r8, r0
	cbnz	r0, .L129
	movw	r3, #531
	str	r2, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L137+4
	movs	r0, #17
	ldr	r2, .L137+16
	bl	vlog
	b	.L135
.L129:
	ldr	r1, .L137+20
	adds	r3, r5, #1
	ldr	r3, .L137+24
	itt	ne
	movne	r4, r5
	movne	r3, r1
	ldr	r1, [r0, #16]
	ldr	r0, [r7, #16]	@ unaligned
	strd	r2, r0, [sp, #32]
	movs	r0, #17
	ldr	r2, [r8, #448]
	strd	r1, r2, [sp, #24]
	movs	r1, #5
	ldr	r2, [r8]
	strd	r3, r4, [sp, #8]
	movw	r3, #538
	str	r6, [sp, #4]
	strd	r8, r2, [sp, #16]
	str	r3, [sp]
	ldr	r2, .L137+28
	ldr	r3, .L137+4
	bl	vlog
	movs	r2, #80
	add	r1, r7, #36
	add	r0, r8, #616
	bl	memcpy
	ldr	r4, [r7, #16]	@ unaligned
	cbnz	r4, .L131
	mov	r2, r4
	mov	r1, r8
	mov	r0, r9
	bl	mpi_node_buffer_in
	b	.L123
.L131:
	cmp	r4, #1
	bne	.L136
	movs	r2, #0
	mov	r1, r8
	mov	r0, r9
	bl	mpi_node_buffer_out
	b	.L136
.L138:
	.align	2
.L137:
	.word	.LC28
	.word	.LANCHOR0+57
	.word	.LC33
	.word	.LC17
	.word	.LC34
	.word	.LC26
	.word	.LC27
	.word	.LC35
	.fnend
	.size	avs_mb_send, .-avs_mb_send
	.align	1
	.global	avs_mb_release
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_mb_release, %function
avs_mb_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r9, r1
	ldr	r3, [r1]	@ unaligned
	.pad #44
	sub	sp, sp, #44
	cmp	r3, #116
	beq	.L140
	movw	r3, #566
	ldr	r2, .L153
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L153+4
	movs	r0, #17
	bl	vlog
.L151:
	mov	r4, #-1
.L139:
	mov	r0, r4
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L140:
	ldr	r4, [r1, #12]	@ unaligned
	ldr	r6, [r1, #4]	@ unaligned
	ldr	r5, [r1, #8]	@ unaligned
	adds	r1, r4, #1
	bne	.L148
	mov	r2, r5
	mov	r1, r6
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r7, r0
	cbnz	r0, .L142
	movw	r3, #578
	strd	r6, r5, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L153+4
	movs	r0, #17
	ldr	r2, .L153+8
	bl	vlog
	b	.L139
.L148:
	movs	r7, #0
.L142:
	adds	r2, r5, #1
	bne	.L143
	adds	r2, r4, #2
	mov	r1, r6
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r7, r0
	cbnz	r0, .L143
	movw	r3, #587
	strd	r6, r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L153+4
	movs	r0, #17
	ldr	r2, .L153+12
	bl	vlog
	b	.L151
.L143:
	movs	r1, #4
	ldr	r0, [r9, #24]	@ unaligned
	bl	mpi_obj_from_id
	mov	r8, r0
	cmp	r0, #0
	beq	.L144
	ldr	r2, .L153+16
	adds	r3, r5, #1
	ldr	r1, [r9, #16]	@ unaligned
	ldr	r3, .L153+20
	itt	ne
	movne	r4, r5
	movne	r3, r2
	ldr	r2, [r0, #16]
	strd	r2, r1, [sp, #28]
	movs	r1, #5
	ldr	r2, [r9, #24]	@ unaligned
	str	r2, [sp, #24]
	ldr	r2, [r0, #448]
	strd	r3, r4, [sp, #8]
	movw	r3, #598
	str	r6, [sp, #4]
	strd	r0, r2, [sp, #16]
	movs	r0, #17
	str	r3, [sp]
	ldr	r2, .L153+24
	ldr	r3, .L153+4
	bl	vlog
	ldr	r4, [r9, #16]	@ unaligned
	cbnz	r4, .L146
	mov	r0, r7
	add	r2, r7, #396
	mov	r1, r8
	bl	mpi_node_release_buffer_flag
	ldr	r0, [r7, #76]
	add	r0, r8, r0, lsl #3
	adds	r0, r0, #52
	bl	list_del
	b	.L139
.L146:
	cmp	r4, #1
	bne	.L147
	mov	r0, r7
	add	r2, r7, #400
	mov	r1, r8
	bl	mpi_node_release_buffer_flag
	ldr	r0, [r7, #76]
	add	r0, r8, r0, lsl #3
	adds	r0, r0, #180
.L152:
	bl	list_del
.L150:
	movs	r4, #0
	b	.L139
.L147:
	cmp	r4, #2
	bne	.L150
	mov	r0, r7
	movs	r2, #0
	mov	r1, r8
	bl	mpi_node_release_buffer_flag
	ldr	r0, [r7, #76]
	add	r0, r8, r0, lsl #3
	add	r0, r0, #308
	b	.L152
.L144:
	movw	r3, #610
	ldr	r2, .L153+28
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L153+4
	movs	r0, #17
	bl	vlog
	b	.L150
.L154:
	.align	2
.L153:
	.word	.LC28
	.word	.LANCHOR0+69
	.word	.LC29
	.word	.LC17
	.word	.LC26
	.word	.LC27
	.word	.LC36
	.word	.LC37
	.fnend
	.size	avs_mb_release, .-avs_mb_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_dev_open, %function
avs_dev_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L156
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	adds	r0, r3, #4
	str	r0, [r1, #128]
	movs	r2, #9
	str	r2, [r3, #172]
	bl	mpi_obj_ref
	movw	r3, #678
	ldr	r2, .L156+4
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp]
	ldr	r3, .L156+8
	bl	vlog
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L157:
	.align	2
.L156:
	.word	.LANCHOR1
	.word	.LC38
	.word	.LANCHOR0+84
	.fnend
	.size	avs_dev_open, .-avs_dev_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_dev_destroy_ctx, %function
avs_dev_destroy_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	cbz	r0, .L159
	bl	kfree
.L159:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	avs_dev_destroy_ctx, .-avs_dev_destroy_ctx
	.align	1
	.global	avs_node_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_node_destroy, %function
avs_node_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	ldr	r3, [r1]	@ unaligned
	.pad #28
	sub	sp, sp, #28
	cmp	r3, #48
	beq	.L182
	movs	r3, #186
	ldr	r2, .L200
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L200+4
	movs	r0, #17
	bl	vlog
	mov	r0, #-1
.L163:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L182:
	ldr	r8, .L200+4
	movs	r5, #0
	mvn	r7, #43
.L164:
	mov	r2, r5
	ldr	r1, [r4, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r6, r0
	cbz	r0, .L166
	ldr	r0, [r0, #288]
	cmp	r0, #0
	bne	.L167
.L171:
	mov	r2, r5
	ldr	r1, [r4, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_remove_node_from_map
	mov	r0, r6
	bl	mpi_node_destroy
	ldr	r3, [r4, #4]	@ unaligned
	ldr	r2, .L200+8
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp, #4]
	movs	r3, #207
	str	r5, [sp, #8]
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
.L166:
	cbz	r5, .L183
	movs	r6, #2
	mvn	r8, #43
.L181:
	mov	r2, r6
	ldr	r1, [r4, #4]	@ unaligned
	movs	r0, #17
	subs	r3, r6, #2
	str	r3, [sp, #16]
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbz	r0, .L172
	ldr	r9, [r0, #288]
	cmp	r9, #0
	bne	.L173
.L180:
	mov	r2, r6
	ldr	r1, [r4, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_remove_node_from_map
	mov	r0, r5
	bl	mpi_node_destroy
	ldr	r2, .L200+12
	movs	r1, #4
	movs	r0, #17
	ldr	r3, [sp, #16]
	str	r3, [sp, #8]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #4]
	movs	r3, #235
	str	r3, [sp]
	ldr	r3, .L200+4
	bl	vlog
.L172:
	cmp	r6, #3
	bne	.L184
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	ldr	r2, .L200+16
	movs	r0, #17
	str	r3, [sp, #4]
	movs	r3, #238
	str	r3, [sp]
	ldr	r3, .L200+4
	bl	vlog
	movs	r0, #0
	b	.L163
.L183:
	movs	r5, #1
	b	.L164
.L167:
	ldr	r2, [r6, #76]
	add	r9, r0, r5, lsl #3
	ldr	r1, [r0, r5, lsl #3]
	add	r10, r2, #1
	sub	r3, r7, r10, lsl #3
	add	r1, r1, r3
	add	r2, r1, r2, lsl #3
	ldr	r2, [r2, #52]
	add	r10, r2, r3
.L168:
	ldr	r0, [r6, #76]
	add	r0, r1, r0, lsl #3
	adds	r0, r0, #52
	cmp	r0, r9
	beq	.L171
	cbz	r1, .L169
	bl	list_del
	mov	r0, r1
	bl	mpi_obj_unref
.L169:
	ldr	r3, [r6, #76]
	mov	r1, r10
	adds	r2, r3, #1
	add	r3, r10, r3, lsl #3
	sub	r2, r7, r2, lsl #3
	ldr	r3, [r3, #52]
	add	r10, r3, r2
	b	.L168
.L184:
	movs	r6, #3
	b	.L181
.L173:
	lsl	fp, r6, #3
	ldr	r0, [r0, #76]
	add	r10, r9, fp
	add	r7, r0, #17
	ldr	r1, [r10, #16]
	sub	r3, r8, r7, lsl #3
	add	r1, r1, r3
	add	r0, r1, r0, lsl #3
	ldr	r7, [r0, #180]
	add	r7, r7, r3
	add	r3, fp, #16
	add	r3, r3, r9
.L174:
	ldr	r2, [r5, #76]
	add	r0, r1, r2, lsl #3
	adds	r0, r0, #180
	cmp	r0, r3
	bne	.L176
	add	r7, r2, #33
	ldr	r1, [r9, fp]
	sub	r3, r8, r7, lsl #3
	add	r1, r1, r3
	add	ip, r1, r2, lsl #3
	ldr	r7, [ip, #308]
	add	r7, r7, r3
.L177:
	ldr	r0, [r5, #76]
	add	r0, r1, r0, lsl #3
	add	r0, r0, #308
	cmp	r0, r10
	beq	.L180
	cbz	r1, .L178
	bl	list_del
	mov	r0, r1
	bl	mpi_obj_unref
.L178:
	ldr	r3, [r5, #76]
	mov	r1, r7
	add	r0, r3, #33
	add	r3, r7, r3, lsl #3
	sub	r0, r8, r0, lsl #3
	ldr	r3, [r3, #308]
	adds	r7, r3, r0
	b	.L177
.L176:
	cbz	r1, .L175
	str	r3, [sp, #20]
	bl	list_del
	mov	r0, r1
	bl	mpi_obj_unref
	ldr	r3, [sp, #20]
.L175:
	ldr	r1, [r5, #76]
	add	r0, r1, #17
	add	r1, r7, r1, lsl #3
	sub	r0, r8, r0, lsl #3
	ldr	r2, [r1, #180]
	mov	r1, r7
	adds	r7, r2, r0
	b	.L174
.L201:
	.align	2
.L200:
	.word	.LC3
	.word	.LANCHOR0+97
	.word	.LC13
	.word	.LC14
	.word	.LC39
	.fnend
	.size	avs_node_destroy, .-avs_node_destroy
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_dev_release, %function
avs_dev_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #44
	sub	sp, sp, #44
	ldr	r4, [r1, #128]
	mov	r0, r4
	bl	mpi_obj_unref
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bne	.L203
	ldr	r8, .L244+24
	movs	r4, #0
.L204:
	movs	r7, #0
	b	.L215
.L224:
	movs	r7, #1
.L215:
	adds	r3, r7, #2
	mov	r1, r4
	movs	r0, #17
	str	r3, [sp, #32]
	mov	r2, r3
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbz	r0, .L205
	ldr	r6, [r0, #60]
	cbz	r6, .L206
	ldr	r3, [r6]
	movs	r1, #4
	ldr	r2, .L244
	movs	r0, #17
	strd	r4, r7, [sp, #4]
	strd	r6, r3, [sp, #12]
	movw	r3, #710
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
	mov	r1, r5
	mov	r0, r6
	bl	mpi_buf_pool_del_user
	movs	r1, #1
	mov	r0, r6
	bl	mpi_buf_pool_destroy
.L206:
	ldr	r10, [r5, #288]
	cmp	r10, #0
	bne	.L207
.L214:
	ldr	r2, [sp, #32]
	mov	r1, r4
	movs	r0, #17
	bl	vsys_remove_node_from_map
	mov	r0, r5
	bl	mpi_node_destroy
	movw	r3, #739
	ldr	r2, .L244+4
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp]
	mov	r3, r8
	strd	r4, r7, [sp, #4]
	bl	vlog
.L205:
	cmp	r7, #0
	beq	.L224
	movs	r5, #0
	mvn	r7, #43
.L222:
	mov	r2, r5
	mov	r1, r4
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r6, r0
	cbz	r0, .L216
	ldr	r0, [r0, #288]
	cmp	r0, #0
	bne	.L217
.L221:
	mov	r2, r5
	mov	r1, r4
	movs	r0, #17
	bl	vsys_remove_node_from_map
	mov	r0, r6
	bl	mpi_node_destroy
	mov	r3, #760
	ldr	r2, .L244+8
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp]
	mov	r3, r8
	strd	r4, r5, [sp, #4]
	bl	vlog
.L216:
	cmp	r5, #0
	beq	.L225
	cmp	r4, #0
	bne	.L223
	mov	r4, r5
	b	.L204
.L207:
	lsls	r3, r7, #3
	ldr	r2, [r5, #76]
	add	fp, r10, r3
	str	r3, [sp, #36]
	add	r3, r2, #17
	mvn	r9, #43
	ldr	r6, [fp, #32]
	sub	r3, r9, r3, lsl #3
	add	r6, r6, r3
	add	r2, r6, r2, lsl #3
	ldr	r2, [r2, #180]
	add	r9, r2, r3
	lsls	r3, r7, #3
	adds	r3, r3, #32
	add	r3, r10, r3
	str	r3, [sp, #28]
.L208:
	ldr	r3, [r5, #76]
	ldr	r2, [sp, #28]
	add	r0, r6, r3, lsl #3
	adds	r0, r0, #180
	cmp	r0, r2
	bne	.L210
	add	r2, r3, #33
	ldr	r6, [fp, #16]
	mvn	r9, #43
	movw	fp, #730
	sub	r2, r9, r2, lsl #3
	add	r6, r6, r2
	add	r3, r6, r3, lsl #3
	ldr	r3, [r3, #308]
	add	r9, r3, r2
	ldr	r3, [sp, #36]
	adds	r3, r3, #16
	add	r10, r10, r3
.L211:
	ldr	r0, [r5, #76]
	add	r0, r6, r0, lsl #3
	add	r0, r0, #308
	cmp	r0, r10
	beq	.L214
	cbz	r6, .L212
	bl	list_del
	ldr	r3, [r6, #16]
	ldr	r2, .L244+12
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp, #20]
	ldr	r3, [r6]
	strd	r4, r7, [sp, #4]
	str	fp, [sp]
	strd	r6, r3, [sp, #12]
	mov	r3, r8
	bl	vlog
	mov	r0, r6
	bl	mpi_obj_unref
.L212:
	ldr	r3, [r5, #76]
	mvn	r2, #43
	mov	r6, r9
	add	r1, r3, #33
	add	r3, r9, r3, lsl #3
	sub	r2, r2, r1, lsl #3
	ldr	r3, [r3, #308]
	add	r9, r3, r2
	b	.L211
.L210:
	cbz	r6, .L209
	bl	list_del
	ldr	r3, [r6, #16]
	ldr	r2, .L244+16
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp, #20]
	ldr	r3, [r6]
	strd	r4, r7, [sp, #4]
	strd	r6, r3, [sp, #12]
	movw	r3, #721
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
	mov	r0, r6
	bl	mpi_obj_unref
.L209:
	ldr	r3, [r5, #76]
	mvn	r2, #43
	mov	r6, r9
	add	r1, r3, #17
	add	r3, r9, r3, lsl #3
	sub	r2, r2, r1, lsl #3
	ldr	r3, [r3, #180]
	add	r9, r3, r2
	b	.L208
.L225:
	movs	r5, #1
	b	.L222
.L217:
	ldr	r2, [r6, #76]
	add	r9, r0, r5, lsl #3
	ldr	r1, [r0, r5, lsl #3]
	add	r10, r2, #1
	sub	r3, r7, r10, lsl #3
	add	r1, r1, r3
	add	r2, r1, r2, lsl #3
	ldr	r2, [r2, #52]
	add	r10, r2, r3
.L218:
	ldr	r0, [r6, #76]
	add	r0, r1, r0, lsl #3
	adds	r0, r0, #52
	cmp	r0, r9
	beq	.L221
	cbz	r1, .L219
	bl	list_del
	mov	r0, r1
	bl	mpi_obj_unref
.L219:
	ldr	r3, [r6, #76]
	mov	r1, r10
	adds	r2, r3, #1
	add	r3, r10, r3, lsl #3
	sub	r2, r7, r2, lsl #3
	ldr	r3, [r3, #52]
	add	r10, r3, r2
	b	.L218
.L223:
	movw	r3, #766
	ldr	r2, .L244+20
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L244+24
	movs	r0, #17
	bl	vlog
.L203:
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L245:
	.align	2
.L244:
	.word	.LC40
	.word	.LC14
	.word	.LC13
	.word	.LC42
	.word	.LC41
	.word	.LC43
	.word	.LANCHOR0+114
	.fnend
	.size	avs_dev_release, .-avs_dev_release
	.align	1
	.global	avs_node_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_node_stop, %function
avs_node_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r1
	ldr	r3, [r1]	@ unaligned
	.pad #20
	sub	sp, sp, #20
	cmp	r3, #48
	beq	.L253
	movw	r3, #359
	ldr	r2, .L265
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L265+4
	movs	r0, #17
	bl	vlog
	mov	r0, #-1
.L246:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L253:
	ldr	r6, .L265+4
	movs	r5, #0
	ldr	r7, .L265+8
.L247:
	mov	r2, r5
	ldr	r1, [r4, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_get_node_from_map
	cbz	r0, .L249
	bl	mpi_node_streamoff
	ldr	r3, [r4, #4]	@ unaligned
	mov	r2, r7
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp, #4]
	mov	r3, #368
	str	r5, [sp, #8]
	str	r3, [sp]
	mov	r3, r6
	bl	vlog
.L249:
	cbz	r5, .L254
	ldr	r8, .L265+4
	movs	r5, #0
	ldr	r9, .L265+16
.L252:
	adds	r2, r5, #2
	ldr	r1, [r4, #4]	@ unaligned
	movs	r0, #17
	bl	vsys_get_node_from_map
	mov	r6, r0
	cbz	r0, .L250
	bl	mpi_node_del_out_watcher
	ldr	r7, [r6, #60]
	cbz	r7, .L251
	mov	r1, r6
	mov	r0, r7
	bl	mpi_buf_pool_del_user
	movs	r1, #1
	mov	r0, r7
	bl	mpi_buf_pool_destroy
.L251:
	mov	r0, r6
	bl	mpi_node_streamoff
	ldr	r3, [r4, #4]	@ unaligned
	mov	r2, r9
	movs	r1, #4
	movs	r0, #17
	str	r3, [sp, #4]
	mov	r3, #386
	str	r5, [sp, #8]
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
.L250:
	cbz	r5, .L255
	ldr	r3, [r4, #4]	@ unaligned
	movs	r1, #4
	ldr	r2, .L265+12
	movs	r0, #17
	str	r3, [sp, #4]
	movw	r3, #391
	str	r3, [sp]
	ldr	r3, .L265+4
	bl	vlog
	movs	r0, #0
	b	.L246
.L254:
	movs	r5, #1
	b	.L247
.L255:
	movs	r5, #1
	b	.L252
.L266:
	.align	2
.L265:
	.word	.LC3
	.word	.LANCHOR0+130
	.word	.LC44
	.word	.LC46
	.word	.LC45
	.fnend
	.size	avs_node_stop, .-avs_node_stop
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_dev_ioctl, %function
avs_dev_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	uxtb	r4, r1
	ldr	r8, [r0, #128]
	movw	r3, #641
	mov	r5, r0
	str	r3, [sp]
	mov	r6, r1
	mov	r7, r2
	movs	r1, #5
	strd	r4, r8, [sp, #8]
	str	r4, [sp, #4]
	movs	r0, #17
	ldr	r3, .L271
	ldr	r2, .L271+4
	bl	vlog
	ldr	r1, [r8, #168]
	cmp	r1, r4
	bhi	.L268
	mov	r3, #644
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L271
	movs	r0, #17
	ldr	r2, .L271+8
	bl	vlog
.L269:
.L267:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L268:
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
	cbnz	r1, .L270
	movw	r3, #651
	ldr	r2, .L271+12
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L271
	movs	r0, #17
	bl	vlog
	b	.L267
.L270:
	mov	r3, r7
	mov	r2, r6
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L272:
	.align	2
.L271:
	.word	.LANCHOR0+144
	.word	.LC47
	.word	.LC48
	.word	.LC49
	.fnend
	.size	avs_dev_ioctl, .-avs_dev_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_dev_create_ctx, %function
avs_dev_create_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r1, #3520
	ldr	r3, .L278
	ldr	r0, [r3, #24]
	bl	kmem_cache_alloc
	cbz	r0, .L273
	str	r0, [r0]
	add	r3, r0, #8
	str	r0, [r0, #4]
	str	r3, [r0, #8]
	str	r3, [r0, #12]
	add	r3, r0, #32
	str	r3, [r0, #32]
	str	r3, [r0, #36]
	add	r3, r0, #16
	str	r3, [r0, #16]
	str	r3, [r0, #20]
	add	r3, r0, #40
	str	r3, [r0, #40]
	str	r3, [r0, #44]
	add	r3, r0, #24
	str	r3, [r0, #24]
	str	r3, [r0, #28]
.L273:
	pop	{r3, pc}
.L279:
	.align	2
.L278:
	.word	kmalloc_caches
	.fnend
	.size	avs_dev_create_ctx, .-avs_dev_create_ctx
	.align	1
	.global	avs_dev_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_dev_probe, %function
avs_dev_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movs	r2, #184
	ldr	r4, .L281
	mov	r6, r0
	movs	r1, #0
	ldr	r5, .L281+4
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r6
	bl	kstrdup
	movs	r3, #1
	str	r3, [r4, #180]
	add	r3, r5, #160
	str	r3, [r4, #168]
	movs	r3, #9
	str	r3, [r4, #172]
	mov	r2, r6
	ldr	r3, .L281+8
	movs	r1, #8
	str	r4, [r4, #164]
	adds	r4, r4, #4
	str	r3, [r4, #68]
	ldr	r3, .L281+12
	str	r3, [r4, #76]
	ldr	r3, .L281+16
	str	r3, [r4, #80]
	str	r0, [r4, #172]
	mov	r0, r4
	bl	mpi_dev_register
	add	r1, r5, #340
	mov	r0, r4
	movs	r2, #0
	pop	{r4, r5, r6, lr}
	b	mpi_sysfs_dev_add
.L282:
	.align	2
.L281:
	.word	.LANCHOR1
	.word	.LANCHOR0
	.word	avs_dev_create_ctx
	.word	avs_dev_destroy_ctx
	.word	avs_handle_message
	.fnend
	.size	avs_dev_probe, .-avs_dev_probe
	.align	1
	.global	avs_dev_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_dev_shutdown, %function
avs_dev_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r3, #1
	ldr	r4, .L284
	ldr	r0, [r4, #176]
	str	r3, [r4]
	bl	kfree
	adds	r0, r4, #4
	pop	{r4, lr}
	b	mpi_dev_unregister
.L285:
	.align	2
.L284:
	.word	.LANCHOR1
	.fnend
	.size	avs_dev_shutdown, .-avs_dev_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	avs_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_init, %function
avs_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L287
	bl	avs_dev_probe
	movs	r0, #0
	pop	{r3, pc}
.L288:
	.align	2
.L287:
	.word	.LC18
	.fnend
	.size	avs_init, .-avs_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	avs_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	avs_exit, %function
avs_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	avs_dev_shutdown
	.fnend
	.size	avs_exit, .-avs_exit
	.global	g_avs_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.36018, %object
	.size	__func__.36018, 15
__func__.36018:
	.ascii	"avs_node_reset\000"
	.type	__func__.35884, %object
	.size	__func__.35884, 16
__func__.35884:
	.ascii	"avs_node_create\000"
	.type	__func__.35983, %object
	.size	__func__.35983, 15
__func__.35983:
	.ascii	"avs_node_start\000"
	.type	__func__.36030, %object
	.size	__func__.36030, 11
__func__.36030:
	.ascii	"avs_mb_get\000"
	.type	__func__.36053, %object
	.size	__func__.36053, 12
__func__.36053:
	.ascii	"avs_mb_send\000"
	.type	__func__.36068, %object
	.size	__func__.36068, 15
__func__.36068:
	.ascii	"avs_mb_release\000"
	.type	__func__.36117, %object
	.size	__func__.36117, 13
__func__.36117:
	.ascii	"avs_dev_open\000"
	.type	__func__.35910, %object
	.size	__func__.35910, 17
__func__.35910:
	.ascii	"avs_node_destroy\000"
	.type	__func__.36135, %object
	.size	__func__.36135, 16
__func__.36135:
	.ascii	"avs_dev_release\000"
	.type	__func__.36006, %object
	.size	__func__.36006, 14
__func__.36006:
	.ascii	"avs_node_stop\000"
	.type	__func__.36090, %object
	.size	__func__.36090, 14
__func__.36090:
	.ascii	"avs_dev_ioctl\000"
	.space	2
	.type	avs_ioctls, %object
	.size	avs_ioctls, 180
avs_ioctls:
	.word	-1068731648
	.word	0
	.word	avs_get_version
	.word	.LC50
	.word	0
	.word	-1070566655
	.word	0
	.word	avs_node_create
	.word	.LC51
	.word	0
	.word	-1070566654
	.word	0
	.word	avs_node_destroy
	.word	.LC52
	.word	0
	.word	-1070566653
	.word	0
	.word	avs_node_start
	.word	.LC53
	.word	0
	.word	-1070566652
	.word	0
	.word	avs_node_stop
	.word	.LC54
	.word	0
	.word	-1070566651
	.word	0
	.word	avs_node_reset
	.word	.LC55
	.word	0
	.word	-1066110202
	.word	16
	.word	avs_mb_get
	.word	.LC56
	.word	0
	.word	-1066110201
	.word	0
	.word	avs_mb_send
	.word	.LC57
	.word	0
	.word	-1066110200
	.word	0
	.word	avs_mb_release
	.word	.LC58
	.word	0
	.type	avs_driver_fops, %object
	.size	avs_driver_fops, 128
avs_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	0
	.word	avs_dev_ioctl
	.word	avs_compat_ioctl
	.word	0
	.space	4
	.word	avs_dev_open
	.space	4
	.word	avs_dev_release
	.space	60
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	g_avs_dev, %object
	.size	g_avs_dev, 184
g_avs_dev:
	.space	184
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"-2024-05-24-16:21:01\000"
.LC1:
	.ascii	"vmpi:28ee389dbc33c2c7db812cd08e54fd3a0f2f992b-v1.4."
	.ascii	"81\000"
.LC2:
	.ascii	"\033[1;32mAVS Grp %d reset node done!\033[0m\000"
.LC3:
	.ascii	"\033[1;31merror struct avs_dev_info mismatching\033"
	.ascii	"[0m\000"
.LC4:
	.ascii	"\033[1;32mAVS Grp %d Pipe %d node have exist. nick_"
	.ascii	"id %d\033[0m\000"
.LC5:
	.ascii	"\033[1;31merror AVS Grp %d Pipe %d create avs pipe "
	.ascii	"node failed! nick_id %d\033[0m\000"
.LC6:
	.ascii	"\033[1;32mAVS Grp %d Pipe %d node create success! n"
	.ascii	"ick_id %d\033[0m\000"
.LC7:
	.ascii	"avs-pipe\000"
.LC8:
	.ascii	"\033[1;32mAVS Grp %d Chn %d node have exist. nick_i"
	.ascii	"d %d\033[0m\000"
.LC9:
	.ascii	"\033[1;31merror AVS Grp %d Chn %d create avs chn no"
	.ascii	"de failed! nick_id %d\033[0m\000"
.LC10:
	.ascii	"\033[1;32mAVS Grp %d Chn %d node create success! ni"
	.ascii	"ck_id %d\033[0m\000"
.LC11:
	.ascii	"avs-chn\000"
.LC12:
	.ascii	"\033[1;32mAVS Grp %d create node done!\033[0m\000"
.LC13:
	.ascii	"\033[1;32mAVS Grp %d Pipe %d node destroy success!\033"
	.ascii	"[0m\000"
.LC14:
	.ascii	"\033[1;32mAVS Grp %d Chn %d node destroy success!\033"
	.ascii	"[0m\000"
.LC15:
	.ascii	"valloc\000"
.LC16:
	.ascii	"\033[1;31merror getting allocator failed!\033[0m\000"
.LC17:
	.ascii	"\033[1;31merror AVS Grp %d Chn %d get node failed!\033"
	.ascii	"[0m\000"
.LC18:
	.ascii	"avs\000"
.LC19:
	.ascii	"\033[1;32mAVS Grp %d Chn %d create MB_SOURCE_PRIVAT"
	.ascii	"E success! pool %p pool_id %d bufffer_cnt %d bufffe"
	.ascii	"r_size %d\033[0m\000"
.LC20:
	.ascii	"\033[1;31merror AVS Grp %d Chn %d pool create faile"
	.ascii	"d!\033[0m\000"
.LC21:
	.ascii	"\033[1;32mAVS Grp %d Chn %d attach MB_SOURCE_USER s"
	.ascii	"uccess! pool %p pool_id %d\033[0m\000"
.LC22:
	.ascii	"\033[1;31merror AVS Grp %d Chn %d pool attach faile"
	.ascii	"d!\033[0m\000"
.LC23:
	.ascii	"\033[1;32mAVS Grp %d Chn %d node stream on success!"
	.ascii	"\033[0m\000"
.LC24:
	.ascii	"\033[1;32mAVS Grp %d Pipe %d node stream on success"
	.ascii	"!\033[0m\000"
.LC25:
	.ascii	"\033[1;32mAVS Grp %d start node done!\033[0m\000"
.LC26:
	.ascii	"Pipe\000"
.LC27:
	.ascii	"Chn \000"
.LC28:
	.ascii	"\033[1;31mstruct avs_buff_info mismatching\033[0m\000"
.LC29:
	.ascii	"\033[1;31merror AVS Grp %d Pipe %d get node failed!"
	.ascii	"\033[0m\000"
.LC30:
	.ascii	"AVS Grp %d %s %d outof mpi_node_get_current_in_buff"
	.ascii	"er mpi_buf(%p), buf_id(%d), buf_refcnt(%d).\000"
.LC31:
	.ascii	"AVS Grp %d %s %d outof mpi_node_get_next_out_buffer"
	.ascii	" mpi_buf(%p), buf_id(%d), buf_refcnt(%d).\000"
.LC32:
	.ascii	"AVS Grp %d %s %d outof mpi_node_request_buf mpi_buf"
	.ascii	"(%p), buf_id(%d), buf_refcnt(%d).\000"
.LC33:
	.ascii	"\033[1;31mAVS Grp %d Pipe %d get node failed!\033[0"
	.ascii	"m\000"
.LC34:
	.ascii	"\033[1;31merror cannot find mpi_buf fr fd %d\033[0m"
	.ascii	"\000"
.LC35:
	.ascii	"AVS Grp %d %s %d mpi_buf(%p), buf_id(%d), buf_refcn"
	.ascii	"t(%d), dma_buf(%p), fd(%d), access(%d).\000"
.LC36:
	.ascii	"AVS Grp %d %s %d put mpi_buf(%p), dma_buf(%p), buf_"
	.ascii	"id(%ld), buf_refcnt(%d), access(%d).\000"
.LC37:
	.ascii	"\033[1;31mbuf null\033[0m\000"
.LC38:
	.ascii	"\033[1;32m--------- avs dev open ----------\033[0m\000"
.LC39:
	.ascii	"\033[1;32mAVS Grp %d destroy node done!\033[0m\000"
.LC40:
	.ascii	"\033[1;32mAVS Grp %d Chn %d destroy MB_SOURCE_PRIVA"
	.ascii	"TE success! pool %p pool_id %d.\033[0m\000"
.LC41:
	.ascii	"\033[1;32mAVS Grp %d Chn %d destroy buf %p buf_id %"
	.ascii	"d buf_refcnt %d form BUF_ACCESS_NODE_OUT success!\033"
	.ascii	"[0m\000"
.LC42:
	.ascii	"\033[1;32mAVS Grp %d Chn %d destroy buf %p buf_id %"
	.ascii	"d buf_refcnt %d form BUF_ACCESS_NODE_POOL success!\033"
	.ascii	"[0m\000"
.LC43:
	.ascii	"\033[1;32m--------- avs dev release ----------\033["
	.ascii	"0m\000"
.LC44:
	.ascii	"\033[1;32mAVS Grp %d Pipe %d node stream off succes"
	.ascii	"s!\033[0m\000"
.LC45:
	.ascii	"\033[1;32mAVS Grp %d Chn %d node stream off success"
	.ascii	"!\033[0m\000"
.LC46:
	.ascii	"\033[1;32mAVS Grp %d stop node done!\033[0m\000"
.LC47:
	.ascii	"nr = %d, index = %d dev = %p\000"
.LC48:
	.ascii	"\033[1;31merror: index = %d\033[0m\000"
.LC49:
	.ascii	"\033[1;31merror: ioctl is null\033[0m\000"
.LC50:
	.ascii	"AVS_IOCTL_VERSION\000"
.LC51:
	.ascii	"AVS_IOCTL_CREATE\000"
.LC52:
	.ascii	"AVS_IOCTL_DESTROY\000"
.LC53:
	.ascii	"AVS_IOCTL_START\000"
.LC54:
	.ascii	"AVS_IOCTL_STOP\000"
.LC55:
	.ascii	"AVS_IOCTL_RESET\000"
.LC56:
	.ascii	"AVS_IOCTL_MB_GET\000"
.LC57:
	.ascii	"AVS_IOCTL_MB_SEND\000"
.LC58:
	.ascii	"AVS_IOCTL_MB_RELEASE\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
