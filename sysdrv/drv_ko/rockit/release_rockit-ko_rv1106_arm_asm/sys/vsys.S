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
	.file	"vsys.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vsys_get_pool:					
	.asciz 	"vsys_get_pool"					
__kstrtabns_vsys_get_pool:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vsys_add_node_to_map:					
	.asciz 	"vsys_add_node_to_map"					
__kstrtabns_vsys_add_node_to_map:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vsys_get_node_from_map:					
	.asciz 	"vsys_get_node_from_map"					
__kstrtabns_vsys_get_node_from_map:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vsys_remove_node_from_map:					
	.asciz 	"vsys_remove_node_from_map"					
__kstrtabns_vsys_remove_node_from_map:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_init, %function
vsys_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vsys_init, .-vsys_init
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_compat_ioctl, %function
vsys_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vsys_compat_ioctl, .-vsys_compat_ioctl
	.align	1
	.global	vsys_get_pool
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_get_pool, %function
vsys_get_pool:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L8
	ldr	r3, [r2]
.L7:
	subs	r3, r3, #36
	add	r1, r3, #36
	cmp	r1, r2
	bne	.L6
	movs	r3, #0
.L3:
	mov	r0, r3
	bx	lr
.L6:
	ldr	r1, [r3, #44]
	cmp	r1, r0
	beq	.L3
	ldr	r3, [r3, #36]
	b	.L7
.L9:
	.align	2
.L8:
	.word	mpi_buf_pool_list
	.fnend
	.size	vsys_get_pool, .-vsys_get_pool
	.align	1
	.global	vsys_add_node_to_map
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_add_node_to_map, %function
vsys_add_node_to_map:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r3, #0
	beq	.L28
	cmp	r0, #8
	beq	.L12
	bhi	.L13
	cmp	r0, #4
	beq	.L14
	cmp	r0, #6
	beq	.L15
.L29:
	movs	r0, #0
	bx	lr
.L13:
	cmp	r0, #18
	beq	.L16
	cmp	r0, #20
	bne	.L29
	cmp	r1, #0
	blt	.L28
	cmp	r2, #31
	bhi	.L28
	add	r1, r2, r1, lsl #5
	ldr	r2, .L30
	add	r1, r2, r1, lsl #2
	str	r3, [r1, #900]
	b	.L29
.L16:
	cmp	r1, #0
	blt	.L28
	cmp	r2, #63
	bhi	.L28
	add	r1, r2, r1, lsl #6
	ldr	r2, .L30
	add	r1, r2, r1, lsl #2
	str	r3, [r1, #388]
	b	.L29
.L14:
	cmp	r1, #0
	blt	.L28
	cmp	r2, #63
	bhi	.L28
	add	r1, r2, r1, lsl #6
	ldr	r2, .L30
	add	r1, r2, r1, lsl #2
	str	r3, [r1, #644]
	b	.L29
.L12:
	cmp	r1, #7
	bhi	.L28
	cmp	r2, #5
	bhi	.L28
	movs	r0, #6
	mla	r1, r0, r1, r2
	ldr	r2, .L30
	add	r1, r2, r1, lsl #2
	str	r3, [r1, #196]
	b	.L29
.L15:
	cmp	r1, #255
	bhi	.L28
	cmp	r2, #4
	bhi	.L28
	add	r1, r1, r1, lsl #2
	add	r1, r1, r2
	ldr	r2, .L30
	add	r1, r2, r1, lsl #2
	str	r3, [r1, #1028]
	b	.L29
.L28:
	mov	r0, #-1
	bx	lr
.L31:
	.align	2
.L30:
	.word	g_vsys_dev
	.fnend
	.size	vsys_add_node_to_map, .-vsys_add_node_to_map
	.align	1
	.global	vsys_remove_node_from_map
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_remove_node_from_map, %function
vsys_remove_node_from_map:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #8
	beq	.L33
	bhi	.L34
	cmp	r0, #4
	beq	.L35
	cmp	r0, #6
	beq	.L36
.L39:
	movs	r0, #0
	bx	lr
.L34:
	cmp	r0, #18
	beq	.L38
	cmp	r0, #20
	bne	.L39
	cmp	r1, #0
	blt	.L49
	cmp	r2, #31
	bhi	.L49
	add	r2, r2, r1, lsl #5
	adds	r2, r2, #224
.L50:
	ldr	r1, .L51
	movs	r0, #0
	add	r2, r1, r2, lsl #2
	str	r0, [r2, #4]
	bx	lr
.L38:
	cmp	r1, #0
	blt	.L49
	cmp	r2, #63
	bhi	.L49
	add	r2, r2, r1, lsl #6
	adds	r2, r2, #96
	b	.L50
.L35:
	cmp	r1, #0
	blt	.L49
	cmp	r2, #63
	bhi	.L49
	add	r2, r2, r1, lsl #6
	adds	r2, r2, #160
	b	.L50
.L33:
	cmp	r1, #7
	bhi	.L49
	cmp	r2, #5
	bhi	.L49
	movs	r3, #6
	mla	r2, r3, r1, r2
	adds	r2, r2, #48
	b	.L50
.L36:
	cmp	r1, #255
	bhi	.L49
	cmp	r2, #4
	bhi	.L49
	add	r1, r1, r1, lsl #2
	movs	r0, #0
	add	r1, r1, r2
	ldr	r2, .L51
	add	r1, r1, #256
	add	r1, r2, r1, lsl #2
	str	r0, [r1, #4]
	bx	lr
.L49:
	mov	r0, #-1
	bx	lr
.L52:
	.align	2
.L51:
	.word	g_vsys_dev
	.fnend
	.size	vsys_remove_node_from_map, .-vsys_remove_node_from_map
	.align	1
	.global	vsys_get_node_from_map
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_get_node_from_map, %function
vsys_get_node_from_map:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r6, r0
	strd	r1, r2, [sp, #8]
	mov	r3, #456
	str	r0, [sp, #4]
	mov	r4, r1
	str	r3, [sp]
	mov	r5, r2
	ldr	r3, .L71
	movs	r1, #5
	ldr	r2, .L71+4
	movs	r0, #0
	bl	vlog
	cmp	r6, #8
	beq	.L54
	bhi	.L55
	cmp	r6, #4
	beq	.L56
	cmp	r6, #6
	beq	.L57
.L70:
	movs	r0, #0
	b	.L53
.L55:
	cmp	r6, #18
	beq	.L59
	cmp	r6, #20
	bne	.L70
	cmp	r4, #0
	blt	.L70
	cmp	r5, #31
	bhi	.L70
	add	r4, r5, r4, lsl #5
	ldr	r5, .L71+8
	add	r4, r5, r4, lsl #2
	ldr	r0, [r4, #900]
.L53:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L59:
	cmp	r4, #0
	blt	.L70
	cmp	r5, #63
	bhi	.L70
	add	r4, r5, r4, lsl #6
	ldr	r5, .L71+8
	add	r4, r5, r4, lsl #2
	ldr	r0, [r4, #388]
	b	.L53
.L56:
	cmp	r4, #0
	blt	.L70
	cmp	r5, #63
	bhi	.L70
	add	r4, r5, r4, lsl #6
	ldr	r5, .L71+8
	add	r4, r5, r4, lsl #2
	ldr	r0, [r4, #644]
	b	.L53
.L54:
	cmp	r4, #7
	bhi	.L70
	cmp	r5, #5
	bhi	.L70
	movs	r3, #6
	mla	r4, r3, r4, r5
	ldr	r3, .L71+8
	add	r4, r3, r4, lsl #2
	ldr	r0, [r4, #196]
	b	.L53
.L57:
	cmp	r4, #255
	bhi	.L70
	cmp	r5, #4
	bhi	.L70
	add	r4, r4, r4, lsl #2
	ldr	r3, .L71+8
	add	r4, r4, r5
	add	r4, r3, r4, lsl #2
	ldr	r0, [r4, #1028]
	b	.L53
.L72:
	.align	2
.L71:
	.word	.LANCHOR0
	.word	.LC0
	.word	g_vsys_dev
	.fnend
	.size	vsys_get_node_from_map, .-vsys_get_node_from_map
	.align	1
	.global	vsys_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_version, %function
vsys_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r3, [r3]
	ldr	r5, [r3, #160]
	ldr	r3, [r1, #32]
	cmp	r3, #76
	beq	.L74
	movs	r3, #83
	ldr	r2, .L80
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L80+4
	movs	r0, #0
	bl	vlog
.L74:
	movs	r3, #0
	movs	r2, #1
	str	r3, [r4, #8]
	strd	r2, r3, [r4]
	ldr	r0, [r5]
	bl	strlen
	cmp	r0, #16
	it	ls
	strls	r0, [r4, #12]
	add	r0, r4, #16
	itt	hi
	movhi	r3, #16
	strhi	r3, [r4, #12]
	ldr	r2, [r4, #12]
	ldr	r1, [r5]
	bl	strncpy
	movs	r0, #0
	str	r0, [r4, #36]
	str	r0, [r4, #56]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L81:
	.align	2
.L80:
	.word	.LC1
	.word	.LANCHOR0+23
	.fnend
	.size	vsys_version, .-vsys_version
	.align	1
	.global	vsys_bind
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_bind, %function
vsys_bind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r1, #12]
	cmp	r3, #28
	beq	.L83
	movs	r3, #109
	ldr	r2, .L92
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L92+4
	movs	r0, #0
	bl	vlog
.L83:
	ldr	r0, [r4]
	cmp	r0, #6
	ittt	eq
	ldreq	r3, [r4, #8]
	addeq	r3, r3, #1
	streq	r3, [r4, #8]
	ldrd	r1, r2, [r4, #4]
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbnz	r0, .L85
	movs	r3, #118
	ldr	r2, .L92+8
	str	r3, [sp]
	ldr	r3, .L92+4
.L91:
	movs	r1, #2
	bl	vlog
	mov	r0, #-1
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L85:
	ldrd	r1, r2, [r4, #20]
	ldr	r0, [r4, #16]
	bl	vsys_get_node_from_map
	mov	r1, r0
	cbnz	r0, .L87
	movs	r3, #126
	ldr	r2, .L92+12
	str	r3, [sp]
	ldr	r3, .L92+4
	b	.L91
.L87:
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, lr}
	b	mpi_node_bind
.L93:
	.align	2
.L92:
	.word	.LC2
	.word	.LANCHOR0+36
	.word	.LC3
	.word	.LC4
	.fnend
	.size	vsys_bind, .-vsys_bind
	.align	1
	.global	vsys_unbind
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_unbind, %function
vsys_unbind:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r1, #12]
	cmp	r3, #28
	beq	.L95
	movs	r3, #145
	ldr	r2, .L104
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L104+4
	movs	r0, #0
	bl	vlog
.L95:
	ldr	r0, [r4]
	cmp	r0, #6
	ittt	eq
	ldreq	r3, [r4, #8]
	addeq	r3, r3, #1
	streq	r3, [r4, #8]
	ldrd	r1, r2, [r4, #4]
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbnz	r0, .L97
	movs	r3, #154
	ldr	r2, .L104+8
	str	r3, [sp]
	ldr	r3, .L104+4
.L103:
	movs	r1, #2
	bl	vlog
	mov	r0, #-1
.L94:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L97:
	ldrd	r1, r2, [r4, #20]
	ldr	r0, [r4, #16]
	bl	vsys_get_node_from_map
	mov	r1, r0
	cbnz	r0, .L99
	movs	r3, #162
	ldr	r2, .L104+12
	str	r3, [sp]
	ldr	r3, .L104+4
	b	.L103
.L99:
	mov	r0, r5
	bl	mpi_node_unbind
	movs	r0, #0
	b	.L94
.L105:
	.align	2
.L104:
	.word	.LC2
	.word	.LANCHOR0+46
	.word	.LC3
	.word	.LC4
	.fnend
	.size	vsys_unbind, .-vsys_unbind
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_thread_fn, %function
vsys_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	add	r5, r0, #168
	mov	r4, r0
.L107:
	ldr	r3, [r4, #184]
	cbz	r3, .L108
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L108:
	mov	r0, r5
	bl	down
	b	.L107
	.fnend
	.size	vsys_thread_fn, .-vsys_thread_fn
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_pool_delete, %function
vsys_pool_delete:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r5, r1
	ldr	r3, [r1, #16]
	ldr	r4, [r0, #128]
	cmp	r3, #532
	beq	.L110
	movw	r3, #299
	ldr	r2, .L118
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L118+4
	movs	r0, #0
	bl	vlog
.L110:
	ldr	r3, [r5]
	movs	r1, #4
	ldr	r2, .L118+8
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #301
	str	r3, [sp]
	ldr	r3, .L118+4
	bl	vlog
	mov	r3, r4
	ldr	r0, [r3, #24]!
	ldr	r2, [r0], #-48
	subs	r2, r2, #48
.L111:
	add	r1, r0, #48
	cmp	r1, r3
	beq	.L113
	ldr	r4, [r0, #44]
	ldr	r1, [r5]
	cmp	r4, r1
	bne	.L112
	ldrd	r2, r3, [r0, #48]
	movs	r1, #1
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r2, #256
	mov	r3, #290
	strd	r2, r3, [r0, #48]
	bl	mpi_buf_pool_destroy
.L113:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L112:
	ldr	r1, [r2, #48]
	mov	r0, r2
	sub	r2, r1, #48
	b	.L111
.L119:
	.align	2
.L118:
	.word	.LC5
	.word	.LANCHOR0+58
	.word	.LC6
	.fnend
	.size	vsys_pool_delete, .-vsys_pool_delete
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_read, %function
vsys_read:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #68
	sub	sp, sp, #68
	ldr	r5, [r0, #128]
	mov	r9, r2
	str	r1, [sp, #60]
	adds	r5, r5, #4
	mov	r0, r5
	bl	mpi_fs_buf_read
	adds	r3, r0, #1
	mov	r4, r0
	bne	.L121
	ldr	r3, .L152
	mov	r0, r5
	ldr	r2, .L152+4
	ldr	r1, .L152+8
	str	r3, [sp]
	ldr	r7, .L152+12
	ldr	r3, .L152+16
	ldr	r2, [r2]
	bl	mpi_fs_buf_add
	ldr	r1, .L152+20
	mov	r0, r5
	ldr	r10, .L152+52
	bl	mpi_fs_buf_add
	ldr	r1, .L152+24
	mov	r0, r5
	ldr	fp, .L152+44
	bl	mpi_fs_buf_add
	ldr	r4, [r7]
	subs	r4, r4, #36
.L122:
	add	r3, r4, #36
	cmp	r3, r7
	beq	.L130
	ldr	r1, [r4, #12]
	mov	r0, r5
	ldr	r2, [r4, #16]
	ldr	r3, [r4, #44]
	str	r1, [sp, #48]
	ldr	r1, [r4, #244]
	str	r1, [sp, #44]
	ldr	r1, [r4, #240]
	str	r1, [sp, #40]
	ldr	r1, [r4, #172]
	str	r1, [sp, #36]
	ldr	r1, [r4, #164]
	str	r1, [sp, #32]
	ldr	r1, [r4, #160]
	str	r1, [sp, #28]
	ldr	r1, [r4, #152]
	str	r1, [sp, #24]
	ldr	r1, [r4, #116]
	str	r1, [sp, #20]
	ldr	r1, [r4, #108]
	str	r1, [sp, #16]
	ldr	r1, [r4, #104]
	str	r1, [sp, #12]
	ldr	r1, [r4, #96]
	strd	r2, r1, [sp, #4]
	ldr	r2, [r4, #56]
	ldr	r1, .L152+28
	str	r2, [sp]
	ldr	r3, [r3, #44]
	ldr	r2, [r4]
	bl	mpi_fs_buf_add
	cbz	r0, .L123
.L130:
	ldr	r1, .L152+32
	mov	r0, r5
	bl	mpi_fs_buf_add
	ldr	r1, .L152+36
	mov	r0, r5
	ldr	r8, .L152+56
	bl	mpi_fs_buf_add
	ldr	r4, [r7]
	ldr	r10, .L152+60
	subs	r4, r4, #36
.L124:
	add	r3, r4, #36
	cmp	r3, r7
	bne	.L140
.L132:
	mov	r2, r9
	ldr	r1, [sp, #60]
	mov	r0, r5
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	mpi_fs_buf_read
.L123:
	ldr	r6, [r4, #260]
	add	r8, r4, #260
	subs	r6, r6, #4
.L125:
	adds	r2, r6, #4
	cmp	r2, r8
	beq	.L128
	ldr	r2, [r6]
	mov	r1, r10
	mov	r0, r5
	ldr	r2, [r2]
	bl	mpi_fs_buf_add
	cbz	r0, .L126
.L128:
	mov	r1, fp
	mov	r0, r5
	bl	mpi_fs_buf_add
	ldr	r4, [r4, #36]
	subs	r4, r4, #36
	b	.L122
.L126:
	ldr	r6, [r6, #4]
	subs	r6, r6, #4
	b	.L125
.L140:
	ldr	r3, [r4, #116]
	cbz	r3, .L131
	ldr	r3, [r4, #44]
	mov	r1, r8
	ldr	r2, .L152+40
	mov	r0, r5
	str	r2, [sp]
	ldr	r3, [r3, #44]
	ldr	r2, [r4]
	bl	mpi_fs_buf_add
	cmp	r0, #0
	bne	.L132
	mvn	fp, #119
	add	r6, r4, #120
	sub	fp, fp, r4
.L133:
	ldr	r1, [r4, #116]
	add	r2, fp, r6
	cmp	r1, r2
	ble	.L135
	ldrb	r2, [r6], #1	@ zero_extendqisi2
	mov	r1, r10
	mov	r0, r5
	bl	mpi_fs_buf_add
	cmp	r0, #0
	beq	.L133
.L135:
	ldr	r1, .L152+44
	mov	r0, r5
	bl	mpi_fs_buf_add
.L131:
	ldr	r3, [r4, #172]
	cbz	r3, .L136
	ldr	r3, [r4, #44]
	mov	r1, r8
	ldr	r2, .L152+48
	mov	r0, r5
	str	r2, [sp]
	ldr	r3, [r3, #44]
	ldr	r2, [r4]
	bl	mpi_fs_buf_add
	cmp	r0, #0
	bne	.L132
	mvn	fp, #175
	add	r6, r4, #176
	sub	fp, fp, r4
.L137:
	ldr	r1, [r4, #172]
	add	r2, fp, r6
	cmp	r1, r2
	ble	.L139
	ldrb	r2, [r6], #1	@ zero_extendqisi2
	mov	r1, r10
	mov	r0, r5
	bl	mpi_fs_buf_add
	cmp	r0, #0
	beq	.L137
.L139:
	ldr	r1, .L152+44
	mov	r0, r5
	bl	mpi_fs_buf_add
.L136:
	ldr	r4, [r4, #36]
	subs	r4, r4, #36
	b	.L124
.L121:
	cbnz	r0, .L120
	mov	r0, r5
	bl	mpi_fs_buf_reset
.L120:
	mov	r0, r4
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L153:
	.align	2
.L152:
	.word	.LC9
	.word	g_vsys_dev
	.word	.LC8
	.word	mpi_node_list
	.word	.LC7
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC18
	.word	.LC16
	.word	.LC20
	.word	.LC15
	.word	.LC17
	.word	.LC19
	.fnend
	.size	vsys_read, .-vsys_read
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_pool_create, %function
vsys_pool_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r6, r1
	ldr	r3, [r1, #16]
	ldr	r4, [r0, #128]
	cmp	r3, #532
	beq	.L155
	movs	r3, #248
	ldr	r2, .L172
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L172+4
	movs	r0, #0
	bl	vlog
.L155:
	ldr	r0, .L172+8
	bl	mpi_allocator_find_by_name
	mov	r7, r0
	cbnz	r0, .L156
	movs	r3, #253
	ldr	r2, .L172+12
	str	r3, [sp]
	ldr	r3, .L172+4
	str	r0, [sp, #4]
.L171:
	movs	r1, #2
	bl	vlog
	mov	r0, #-1
.L154:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L156:
	mov	r2, sp
	bic	r3, r2, #8160
	bic	r3, r3, #31
	movs	r1, #1
	ldr	r3, [r3, #12]
	add	r3, r3, #736
	str	r3, [r2]
	movs	r3, #0
	ldr	r2, [r6, #12]
	bl	mpi_buf_pool_create
	mov	r5, r0
	cbnz	r0, .L158
	movw	r3, #259
	str	r7, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L172+16
	ldr	r3, .L172+4
	b	.L171
.L158:
	ldr	r2, [r4, #28]
	add	r3, r0, #48
	adds	r4, r4, #24
	str	r3, [r4, #4]
	ldr	r9, .L172+4
	movs	r7, #0
	strd	r4, r2, [r0, #48]
	mov	r4, r6
	str	r3, [r2]
	ldr	r10, .L172+20
	ldr	r3, [r4], #20
	str	r3, [r0, #44]
.L159:
	ldr	r3, [r6, #4]
	cmp	r3, r7
	bgt	.L164
	movs	r0, #0
	b	.L154
.L164:
	ldr	fp, [r6, #8]
	ldr	r0, [r4]
	cmp	fp, #0
	beq	.L160
	bl	dma_buf_get
	mov	r8, r0
	cbz	r0, .L162
	mov	r1, r0
	movs	r2, #1
	mov	r0, r5
	bl	mpi_buf_pool_import_dmabuf
	mov	r0, r8
	bl	dma_buf_put
.L162:
	adds	r7, r7, #1
	adds	r4, r4, #4
	b	.L159
.L160:
	movs	r1, #4
	bl	mpi_obj_from_id
	ldr	r3, [r4]
	mov	r2, r10
	movs	r1, #4
	mov	r8, r0
	str	r3, [sp, #4]
	mov	r0, fp
	movw	r3, #275
	str	r3, [sp]
	mov	r3, r9
	bl	vlog
	cmp	r8, #0
	beq	.L162
	mov	r1, r8
	mov	r0, r5
	bl	mpi_buf_pool_import
	mov	r0, r8
	bl	mpi_obj_unref
	b	.L162
.L173:
	.align	2
.L172:
	.word	.LC5
	.word	.LANCHOR0+75
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.fnend
	.size	vsys_pool_create, .-vsys_pool_create
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_ioctl, %function
vsys_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	uxtb	r4, r1
	ldr	r3, .L178
	mov	r6, r0
	mov	r5, r1
	mov	r7, r2
	adds	r3, r3, #4
	ldr	r2, .L178+4
	ldr	r8, [r3, #160]
	movs	r1, #5
	strd	r4, r3, [sp, #8]
	movs	r0, #0
	mov	r3, #336
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r3, .L178+8
	bl	vlog
	ldr	r0, [r8, #192]
	cmp	r0, r4
	bhi	.L175
	movw	r3, #339
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L178+8
	movs	r0, #0
	ldr	r2, .L178+12
	bl	vlog
.L176:
.L174:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L175:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
	cmp	r4, r0
	sbc	r0, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r4, r4, r0
	ldr	r0, [r8, #188]
	movs	r3, #20
	mla	r0, r3, r4, r0
	cbnz	r0, .L177
	mov	r3, #346
	ldr	r2, .L178+16
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L178+8
	bl	vlog
	b	.L174
.L177:
	mov	r1, r0
	mov	r3, r7
	mov	r2, r5
	mov	r0, r6
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L179:
	.align	2
.L178:
	.word	g_vsys_dev
	.word	.LC25
	.word	.LANCHOR0+92
	.word	.LC26
	.word	.LC27
	.fnend
	.size	vsys_ioctl, .-vsys_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_deinit, %function
vsys_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	ldr	r3, [r0, #128]
	ldr	r3, [r3]
	ldr	r2, [r3, #16]
	cmp	r2, #1
	bgt	.L230
	ldr	r4, [r3, #160]
	mov	r9, #0
	add	r5, r4, #196
	add	r6, r4, #388
.L182:
	mov	r8, r5
	movs	r7, #6
.L184:
	ldr	r0, [r8], #4
	cbz	r0, .L183
	bl	mpi_node_destroy
	str	r9, [r8, #-4]
.L183:
	subs	r7, r7, #1
	bne	.L184
	adds	r5, r5, #24
	cmp	r5, r6
	bne	.L182
	add	r8, r4, #644
	add	r5, r4, #900
	mov	r9, r8
.L187:
	ldr	r0, [r8], #4
	cbz	r0, .L186
	bl	mpi_node_destroy
	str	r7, [r8, #-4]
.L186:
	cmp	r8, r5
	bne	.L187
	add	fp, r4, #6144
	add	r8, r4, #1032
	add	fp, fp, #8
.L188:
	mov	r10, r8
	movs	r7, #4
.L190:
	ldr	r1, [r10], #4
	cbz	r1, .L189
	ldr	r0, [r8, #-4]
	bl	mpi_node_unbind
	movs	r3, #0
	str	r3, [r10, #-4]
.L189:
	subs	r7, r7, #1
	bne	.L190
	str	r7, [r8, #-4]
	add	r8, r8, #20
	cmp	fp, r8
	bne	.L188
.L193:
	ldr	r0, [r6], #4
	cbz	r0, .L192
	bl	mpi_node_destroy
	str	r7, [r6, #-4]
.L192:
	cmp	r9, r6
	bne	.L193
	addw	r4, r4, #1028
	movs	r6, #0
.L195:
	ldr	r0, [r5], #4
	cbz	r0, .L194
	bl	mpi_node_destroy
	str	r6, [r5, #-4]
.L194:
	cmp	r4, r5
	bne	.L195
	ldr	r3, .L231
	ldr	r9, .L231+8
	ldr	r10, .L231+12
	ldr	r4, [r3]
	mov	r8, r3
	ldr	r5, [r4], #-36
	subs	r5, r5, #36
.L196:
	add	r3, r4, #36
	cmp	r3, r8
	bne	.L203
	ldr	r5, .L231+4
	ldr	r0, [r5]
	ldr	r4, [r0], #-36
	subs	r4, r4, #36
.L200:
	add	r3, r0, #36
	cmp	r3, r5
	bne	.L201
.L230:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L198:
	ldr	r7, [r4, #4]
	mov	r1, r9
	mov	r0, r7
	bl	strcmp
	cbz	r0, .L197
	mov	r1, r10
	mov	r0, r7
	bl	strcmp
	cbz	r0, .L197
	mov	r0, r4
	bl	mpi_obj_unref
.L197:
	adds	r6, r6, #1
.L199:
	ldr	r3, [r4, #16]
	cmp	r6, r3
	blt	.L198
	ldr	r3, [r5, #36]
	mov	r4, r5
	sub	r5, r3, #36
	b	.L196
.L203:
	movs	r6, #0
	b	.L199
.L201:
	movs	r1, #1
	bl	mpi_buf_pool_destroy
	ldr	r3, [r4, #36]
	mov	r0, r4
	sub	r4, r3, #36
	b	.L200
.L232:
	.align	2
.L231:
	.word	mpi_buf_list
	.word	mpi_buf_pool_list
	.word	.LC28
	.word	.LC29
	.fnend
	.size	vsys_deinit, .-vsys_deinit
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_open, %function
vsys_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L238
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r7, r1
	ldr	r0, [r3, #20]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbnz	r0, .L234
	mov	r3, #370
	ldr	r2, .L238+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L238+8
	bl	vlog
.L237:
	mov	r5, #-1
.L233:
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L234:
	mov	r1, #10240
	adds	r0, r0, #4
	bl	mpi_fs_buf_init
	mov	r5, r0
	cbz	r0, .L236
	mov	r0, r4
	bl	kfree
	b	.L237
.L236:
	ldr	r6, .L238+12
	add	r3, r4, #24
	str	r3, [r4, #24]
	str	r3, [r4, #28]
	mov	r0, r6
	bl	mpi_obj_ref
	str	r6, [r4]
	str	r4, [r7, #128]
	ldr	r1, [r6]
	ldr	r0, .L238+16
	bl	printk
	b	.L233
.L239:
	.align	2
.L238:
	.word	kmalloc_caches
	.word	.LC30
	.word	.LANCHOR0+103
	.word	g_vsys_dev+4
	.word	.LC31
	.fnend
	.size	vsys_open, .-vsys_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_release, %function
vsys_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r4, r1
	ldr	r5, [r1, #128]
	ldr	r0, [r5]
	bl	mpi_obj_unref
	ldr	r3, [r5]
	ldr	r2, .L244
	movs	r1, #5
	movs	r0, #0
	ldr	r3, [r3]
	str	r3, [sp, #4]
	mov	r3, #390
	str	r3, [sp]
	ldr	r3, .L244+4
	bl	vlog
	ldr	r3, [r5]
	ldr	r3, [r3, #16]
	cmp	r3, #1
	bne	.L241
	movs	r1, #0
	mov	r0, r4
	bl	vsys_deinit
.L241:
	mov	r7, r5
	ldr	r8, .L244+4
	ldr	r4, [r7, #24]!
	ldr	r9, .L244+8
	ldr	r6, [r4], #-48
	subs	r6, r6, #48
.L242:
	add	r3, r4, #48
	cmp	r3, r7
	bne	.L243
	adds	r0, r5, #4
	bl	mpi_fs_buf_deinit
	mov	r0, r5
	bl	kfree
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L243:
	ldr	r3, [r4]
	mov	r2, r9
	movs	r1, #4
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #395
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
	ldrd	r2, r3, [r4, #48]
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r3, #290
	mov	r2, #256
	strd	r2, r3, [r4, #48]
	ldr	r3, [r6, #48]
	mov	r4, r6
	sub	r6, r3, #48
	b	.L242
.L245:
	.align	2
.L244:
	.word	.LC32
	.word	.LANCHOR0+113
	.word	.LC33
	.fnend
	.size	vsys_release, .-vsys_release
	.align	1
	.global	vsys_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_probe, %function
vsys_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movw	r2, #6148
	ldr	r4, .L248
	mov	r6, r0
	movs	r1, #0
	ldr	r5, .L248+4
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r6
	bl	kstrdup
	mov	r2, r6
	movs	r1, #8
	movs	r3, #7
	str	r5, [r4, #188]
	str	r3, [r4, #192]
	str	r4, [r4, #164]
	str	r0, [r4]
	adds	r0, r4, #4
	bl	mpi_dev_register
	add	r1, r5, #140
	add	r0, r4, #172
	strd	r0, r0, [r4, #172]
	adds	r0, r4, #4
	movs	r3, #0
	str	r3, [r4, #72]
	strd	r3, r3, [r4, #80]
	str	r3, [r4, #168]
	bl	mpi_sysfs_dev_add
	ldr	r3, .L248+8
	mov	r2, #-1
	mov	r1, r4
	ldr	r0, .L248+12
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L247
	bl	wake_up_process
.L247:
	str	r5, [r4, #180]
	pop	{r4, r5, r6, pc}
.L249:
	.align	2
.L248:
	.word	g_vsys_dev
	.word	.LANCHOR0+128
	.word	.LC34
	.word	vsys_thread_fn
	.fnend
	.size	vsys_probe, .-vsys_probe
	.align	1
	.global	vsys_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_shutdown, %function
vsys_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L251
	movs	r3, #1
	adds	r0, r0, #4
	str	r3, [r0, #180]
	b	mpi_dev_unregister
.L252:
	.align	2
.L251:
	.word	g_vsys_dev
	.fnend
	.size	vsys_shutdown, .-vsys_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	vsys_load
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_load, %function
vsys_load:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L254
	bl	vsys_probe
	movs	r0, #0
	pop	{r3, pc}
.L255:
	.align	2
.L254:
	.word	.LC34
	.fnend
	.size	vsys_load, .-vsys_load
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	vsys_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_exit, %function
vsys_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	vsys_shutdown
	.fnend
	.size	vsys_exit, .-vsys_exit
	.text
	.align	1
	.global	vsys_close
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_close, %function
vsys_close:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	movs	r4, #0
	ldr	r7, .L284
.L258:
	adds	r6, r7, r4
	movs	r5, #0
.L260:
	ldr	r0, [r6], #4
	cbz	r0, .L259
	bl	mpi_node_stream_pause
.L259:
	adds	r5, r5, #1
	cmp	r5, #6
	bne	.L260
	adds	r4, r4, #24
	cmp	r4, #192
	bne	.L258
	ldr	r5, .L284+4
	movs	r4, #0
.L263:
	ldr	r0, [r5], #4
	cbz	r0, .L262
	bl	mpi_node_stream_pause
.L262:
	adds	r4, r4, #1
	cmp	r4, #64
	bne	.L263
	ldr	r5, .L284+8
	movs	r4, #0
.L265:
	ldr	r0, [r5], #4
	cbz	r0, .L264
	bl	mpi_node_stream_pause
.L264:
	adds	r4, r4, #1
	cmp	r4, #64
	bne	.L265
	ldr	r5, .L284+12
	movs	r4, #0
.L267:
	ldr	r0, [r5], #4
	cbz	r0, .L266
	bl	mpi_node_stream_pause
.L266:
	adds	r4, r4, #1
	cmp	r4, #32
	bne	.L267
	movs	r0, #100
	pop	{r3, r4, r5, r6, r7, lr}
	b	msleep
.L285:
	.align	2
.L284:
	.word	g_vsys_dev+196
	.word	g_vsys_dev+644
	.word	g_vsys_dev+388
	.word	g_vsys_dev+900
	.fnend
	.size	vsys_close, .-vsys_close
	.global	g_vsys_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.36642, %object
	.size	__func__.36642, 23
__func__.36642:
	.ascii	"vsys_get_node_from_map\000"
	.type	__func__.36422, %object
	.size	__func__.36422, 13
__func__.36422:
	.ascii	"vsys_version\000"
	.type	__func__.36430, %object
	.size	__func__.36430, 10
__func__.36430:
	.ascii	"vsys_bind\000"
	.type	__func__.36438, %object
	.size	__func__.36438, 12
__func__.36438:
	.ascii	"vsys_unbind\000"
	.type	__func__.36509, %object
	.size	__func__.36509, 17
__func__.36509:
	.ascii	"vsys_pool_delete\000"
	.type	__func__.36497, %object
	.size	__func__.36497, 17
__func__.36497:
	.ascii	"vsys_pool_create\000"
	.type	__func__.36540, %object
	.size	__func__.36540, 11
__func__.36540:
	.ascii	"vsys_ioctl\000"
	.type	__func__.36562, %object
	.size	__func__.36562, 10
__func__.36562:
	.ascii	"vsys_open\000"
	.type	__func__.36571, %object
	.size	__func__.36571, 13
__func__.36571:
	.ascii	"vsys_release\000"
	.space	2
	.type	vsys_ioctls, %object
	.size	vsys_ioctls, 140
vsys_ioctls:
	.word	-1068731648
	.word	0
	.word	vsys_version
	.word	.LC35
	.word	0
	.word	-1071877375
	.word	0
	.word	vsys_bind
	.word	.LC36
	.word	0
	.word	-1071877374
	.word	0
	.word	vsys_unbind
	.word	.LC37
	.word	0
	.word	29443
	.word	0
	.word	vsys_init
	.word	.LC38
	.word	0
	.word	29444
	.word	0
	.word	vsys_deinit
	.word	.LC39
	.word	0
	.word	-1038847227
	.word	0
	.word	vsys_pool_create
	.word	.LC40
	.word	0
	.word	-1038847226
	.word	0
	.word	vsys_pool_delete
	.word	.LC41
	.word	0
	.type	vsys_driver_fops, %object
	.size	vsys_driver_fops, 128
vsys_driver_fops:
	.word	0
	.space	4
	.word	vsys_read
	.space	24
	.word	0
	.word	vsys_ioctl
	.word	vsys_compat_ioctl
	.word	0
	.space	4
	.word	vsys_open
	.space	4
	.word	vsys_release
	.space	60
	.bss
	.align	2
	.type	g_vsys_dev, %object
	.size	g_vsys_dev, 6148
g_vsys_dev:
	.space	6148
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"%s,%d:mod = %d dev_id = %d, chn_id = %d\000"
.LC1:
	.ascii	"\033[31m\033[1m%s,%d:struct vsys_version mismatchin"
	.ascii	"g\033[0m\000"
.LC2:
	.ascii	"\033[31m\033[1m%s,%d:struct vsys_bind mismatching\033"
	.ascii	"[0m\000"
.LC3:
	.ascii	"\033[31m\033[1m%s,%d:get node from map [src chn] fa"
	.ascii	"il\033[0m\000"
.LC4:
	.ascii	"\033[31m\033[1m%s,%d:get node from map [dst chn] fa"
	.ascii	"il\033[0m\000"
.LC5:
	.ascii	"\033[31m\033[1m%s,%d:struct vsys_pool mismatching\033"
	.ascii	"[0m\000"
.LC6:
	.ascii	"\033[32m\033[1m%s,%d:pool->id =  %d\033[0m\000"
.LC7:
	.ascii	"vmpi:9093280df536e902dd5125cdc9953509ebc59a74-v1.4."
	.ascii	"32\000"
.LC8:
	.ascii	"module: %s\012version: %s\012build: %s\012\000"
.LC9:
	.ascii	"-2023-02-02-15:30:51\000"
.LC10:
	.ascii	"***********************************************mpi "
	.ascii	"node list******************************************"
	.ascii	"*********\012\000"
.LC11:
	.ascii	"id---name--------nid-ref-infa_cnt-lcnt-frate-fbase-"
	.ascii	"onfa_cnt-lcnt-frate-fbase-itime-otime-state-next_no"
	.ascii	"de-------------\012\000"
.LC12:
	.ascii	"%-#4d %-#11s %-#3d %-#3d %-#8d %-#4d %-#5d %-#5d %-"
	.ascii	"#8d %-#4d %-#5d %-#5d %-#5ld %-#5ld %-#5x \000"
.LC13:
	.ascii	"*********************************************mpi no"
	.ascii	"de frame control***********************************"
	.ascii	"*********\012\000"
.LC14:
	.ascii	"id---name--------table-----------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"---------\012\000"
.LC15:
	.ascii	"%-#4d  \000"
.LC16:
	.ascii	"\012\000"
.LC17:
	.ascii	"%-#4d %-#11s %-#5s\000"
.LC18:
	.ascii	"in\000"
.LC19:
	.ascii	"%d \000"
.LC20:
	.ascii	"out\000"
.LC21:
	.ascii	"valloc\000"
.LC22:
	.ascii	"\033[31m\033[1m%s,%d:error getting vdev  %p\033[0m\000"
.LC23:
	.ascii	"\033[31m\033[1m%s,%d:create pool fail  %p\033[0m\000"
.LC24:
	.ascii	"\033[32m\033[1m%s,%d:mpi_buf id =  %d\033[0m\000"
.LC25:
	.ascii	"%s,%d:vsys_ioctl: nr = %d, index = %d dev = %p\000"
.LC26:
	.ascii	"\033[31m\033[1m%s,%d:error: index = %d\033[0m\000"
.LC27:
	.ascii	"\033[31m\033[1m%s,%d: error: ioctl is null\033[0m\000"
.LC28:
	.ascii	"mcu\000"
.LC29:
	.ascii	"venc\000"
.LC30:
	.ascii	"\033[31m\033[1m%s,%d:valloc open fail\033[0m\000"
.LC31:
	.ascii	"vsys dev open %d\000"
.LC32:
	.ascii	"%s,%d:%d\000"
.LC33:
	.ascii	"\033[32m\033[1m%s,%d:pool id = %d, user process exi"
	.ascii	"t\033[0m\000"
.LC34:
	.ascii	"vsys\000"
.LC35:
	.ascii	"VSYS_IOCTL_VERSION\000"
.LC36:
	.ascii	"VSYS_IOCTL_SYS_BIND\000"
.LC37:
	.ascii	"VSYS_IOCTL_SYS_UNBIND\000"
.LC38:
	.ascii	"VSYS_IOCTL_SYS_INIT\000"
.LC39:
	.ascii	"VSYS_IOCTL_SYS_DEINIT\000"
.LC40:
	.ascii	"VSYS_IOCTL_SYS_POOL_CREATE\000"
.LC41:
	.ascii	"VSYS_IOCTL_SYS_POOL_DELETE\000"
	.section	___ksymtab_gpl+vsys_add_node_to_map,"a"
	.align	2
	.type	__ksymtab_vsys_add_node_to_map, %object
	.size	__ksymtab_vsys_add_node_to_map, 12
__ksymtab_vsys_add_node_to_map:
	.word	vsys_add_node_to_map
	.word	__kstrtab_vsys_add_node_to_map
	.word	__kstrtabns_vsys_add_node_to_map
	.section	___ksymtab_gpl+vsys_get_node_from_map,"a"
	.align	2
	.type	__ksymtab_vsys_get_node_from_map, %object
	.size	__ksymtab_vsys_get_node_from_map, 12
__ksymtab_vsys_get_node_from_map:
	.word	vsys_get_node_from_map
	.word	__kstrtab_vsys_get_node_from_map
	.word	__kstrtabns_vsys_get_node_from_map
	.section	___ksymtab_gpl+vsys_get_pool,"a"
	.align	2
	.type	__ksymtab_vsys_get_pool, %object
	.size	__ksymtab_vsys_get_pool, 12
__ksymtab_vsys_get_pool:
	.word	vsys_get_pool
	.word	__kstrtab_vsys_get_pool
	.word	__kstrtabns_vsys_get_pool
	.section	___ksymtab_gpl+vsys_remove_node_from_map,"a"
	.align	2
	.type	__ksymtab_vsys_remove_node_from_map, %object
	.size	__ksymtab_vsys_remove_node_from_map, 12
__ksymtab_vsys_remove_node_from_map:
	.word	vsys_remove_node_from_map
	.word	__kstrtab_vsys_remove_node_from_map
	.word	__kstrtabns_vsys_remove_node_from_map
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
