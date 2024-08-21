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
	ldr	r2, .L12
	ldr	r3, [r2]
.L11:
	subs	r3, r3, #36
	add	r1, r3, #36
	cmp	r1, r2
	bne	.L7
	movs	r3, #0
.L3:
	mov	r0, r3
	bx	lr
.L7:
	ldr	r1, [r3, #12]
	lsls	r1, r1, #30
	bpl	.L5
	ldr	r1, [r3, #44]
	cmp	r1, r0
	beq	.L3
.L5:
	ldr	r3, [r3, #36]
	b	.L11
.L13:
	.align	2
.L12:
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
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r7, r0
	.pad #20
	sub	sp, sp, #20
	mov	r6, r1
	mov	r4, r2
	mov	r5, r3
	cbnz	r3, .L15
.L25:
	mov	r0, #-1
.L14:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L15:
	strd	r1, r2, [sp, #8]
	movw	r3, #697
	str	r0, [sp, #4]
	movs	r1, #5
	str	r3, [sp]
	movs	r0, #0
	ldr	r3, .L28
	subs	r7, r7, #4
	ldr	r2, .L28+4
	bl	vlog
	cmp	r7, #19
	bhi	.L26
	tbb	[pc, r7]
.L18:
	.byte	(.L24-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L23-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L22-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L21-.L18)/2
	.byte	(.L20-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L19-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L26-.L18)/2
	.byte	(.L17-.L18)/2
	.p2align 1
.L21:
	cmp	r6, #1
	bhi	.L25
	cmp	r4, #3
	bhi	.L25
	add	r4, r4, r6, lsl #2
	add	r4, r4, #1792
.L27:
	ldr	r3, .L28+8
	add	r4, r3, r4, lsl #2
	str	r5, [r4, #4]
	b	.L26
.L19:
	cmp	r6, #0
	blt	.L25
	cmp	r4, #31
	bhi	.L25
	add	r4, r4, r6, lsl #5
	ldr	r3, .L28+8
	add	r4, r3, r4, lsl #2
	str	r5, [r4, #900]
.L26:
	movs	r0, #0
	b	.L14
.L20:
	cmp	r6, #0
	blt	.L25
	cmp	r4, #63
	bhi	.L25
	add	r4, r4, r6, lsl #6
	ldr	r3, .L28+8
	add	r4, r3, r4, lsl #2
	str	r5, [r4, #388]
	b	.L26
.L24:
	cmp	r6, #0
	blt	.L25
	cmp	r4, #63
	bhi	.L25
	add	r4, r4, r6, lsl #6
	ldr	r3, .L28+8
	add	r4, r3, r4, lsl #2
	str	r5, [r4, #644]
	b	.L26
.L22:
	cmp	r6, #7
	bhi	.L25
	cmp	r4, #5
	bhi	.L25
	movs	r3, #6
	mla	r4, r3, r6, r4
	ldr	r6, .L28+8
	add	r4, r6, r4, lsl #2
	str	r5, [r4, #196]
	b	.L26
.L23:
	cmp	r6, #255
	bhi	.L25
	cmp	r4, #5
	bhi	.L25
	movs	r3, #6
	mla	r4, r3, r6, r4
	ldr	r6, .L28+8
	add	r4, r6, r4, lsl #2
	str	r5, [r4, #1028]
	b	.L26
.L17:
	cmp	r6, #0
	blt	.L25
	cmp	r4, #255
	bhi	.L25
	add	r4, r4, r6, lsl #8
	add	r4, r4, #1800
	b	.L27
.L29:
	.align	2
.L28:
	.word	.LANCHOR0
	.word	.LC0
	.word	g_vsys_dev
	.fnend
	.size	vsys_add_node_to_map, .-vsys_add_node_to_map
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
	movw	r3, #747
	str	r0, [sp, #4]
	mov	r5, r1
	str	r3, [sp]
	mov	r4, r2
	ldr	r3, .L56
	movs	r1, #5
	ldr	r2, .L56+4
	movs	r0, #0
	subs	r6, r6, #4
	bl	vlog
	cmp	r6, #19
	bhi	.L54
	tbb	[pc, r6]
.L33:
	.byte	(.L39-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L38-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L37-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L36-.L33)/2
	.byte	(.L35-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L34-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L54-.L33)/2
	.byte	(.L32-.L33)/2
	.p2align 1
.L36:
	cmp	r5, #1
	bhi	.L54
	cmp	r4, #3
	bhi	.L54
	add	r4, r4, r5, lsl #2
	add	r4, r4, #1792
.L55:
	ldr	r3, .L56+8
	add	r4, r3, r4, lsl #2
	ldr	r0, [r4, #4]
	b	.L30
.L34:
	cmp	r5, #0
	blt	.L54
	cmp	r4, #31
	bhi	.L54
	add	r4, r4, r5, lsl #5
	ldr	r3, .L56+8
	add	r4, r3, r4, lsl #2
	ldr	r0, [r4, #900]
.L30:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L35:
	cmp	r5, #0
	blt	.L54
	cmp	r4, #63
	bhi	.L54
	add	r4, r4, r5, lsl #6
	ldr	r3, .L56+8
	add	r4, r3, r4, lsl #2
	ldr	r0, [r4, #388]
	b	.L30
.L39:
	cmp	r5, #0
	blt	.L54
	cmp	r4, #63
	bhi	.L54
	add	r4, r4, r5, lsl #6
	ldr	r3, .L56+8
	add	r4, r3, r4, lsl #2
	ldr	r0, [r4, #644]
	b	.L30
.L37:
	cmp	r5, #7
	bhi	.L54
	cmp	r4, #5
	bhi	.L54
	movs	r3, #6
	mla	r4, r3, r5, r4
	ldr	r5, .L56+8
	add	r4, r5, r4, lsl #2
	ldr	r0, [r4, #196]
	b	.L30
.L38:
	cmp	r5, #255
	bhi	.L54
	cmp	r4, #5
	bhi	.L54
	movs	r3, #6
	mla	r4, r3, r5, r4
	ldr	r5, .L56+8
	add	r4, r5, r4, lsl #2
	ldr	r0, [r4, #1028]
	b	.L30
.L32:
	cmp	r5, #0
	blt	.L54
	cmp	r4, #255
	bhi	.L54
	add	r4, r4, r5, lsl #8
	add	r4, r4, #1800
	b	.L55
.L54:
	movs	r0, #0
	b	.L30
.L57:
	.align	2
.L56:
	.word	.LANCHOR0+21
	.word	.LC0
	.word	g_vsys_dev
	.fnend
	.size	vsys_get_node_from_map, .-vsys_get_node_from_map
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
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r6, r0
	strd	r1, r2, [sp, #8]
	movw	r3, #797
	str	r0, [sp, #4]
	mov	r5, r1
	str	r3, [sp]
	mov	r4, r2
	ldr	r3, .L86
	movs	r1, #5
	ldr	r2, .L86+4
	movs	r0, #0
	subs	r6, r6, #4
	bl	vlog
	cmp	r6, #19
	bhi	.L68
	tbb	[pc, r6]
.L61:
	.byte	(.L67-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L66-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L65-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L64-.L61)/2
	.byte	(.L63-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L62-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L68-.L61)/2
	.byte	(.L60-.L61)/2
	.p2align 1
.L64:
	cmp	r5, #1
	bhi	.L82
	cmp	r4, #3
	bhi	.L82
	add	r4, r4, r5, lsl #2
	add	r4, r4, #1792
.L83:
	ldr	r3, .L86+8
	add	r4, r3, r4, lsl #2
.L84:
	movs	r0, #0
	str	r0, [r4, #4]
.L58:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L62:
	cmp	r5, #0
	blt	.L82
	cmp	r4, #31
	bhi	.L82
	add	r4, r4, r5, lsl #5
	adds	r4, r4, #224
	b	.L83
.L63:
	cmp	r5, #0
	blt	.L82
	cmp	r4, #63
	bhi	.L82
	add	r4, r4, r5, lsl #6
	adds	r4, r4, #96
	b	.L83
.L67:
	cmp	r5, #0
	blt	.L82
	cmp	r4, #63
	bhi	.L82
	add	r4, r4, r5, lsl #6
	adds	r4, r4, #160
	b	.L83
.L65:
	cmp	r5, #7
	bhi	.L82
	cmp	r4, #5
	bhi	.L82
	movs	r3, #6
	mla	r4, r3, r5, r4
	adds	r4, r4, #48
.L85:
	ldr	r5, .L86+8
	add	r4, r5, r4, lsl #2
	b	.L84
.L66:
	cmp	r5, #255
	bhi	.L82
	cmp	r4, #5
	bhi	.L82
	movs	r3, #6
	mla	r4, r3, r5, r4
	add	r4, r4, #256
	b	.L85
.L60:
	cmp	r5, #0
	blt	.L82
	cmp	r4, #255
	bhi	.L82
	add	r4, r4, r5, lsl #8
	add	r4, r4, #1800
	b	.L83
.L68:
	movs	r0, #0
	b	.L58
.L82:
	mov	r0, #-1
	b	.L58
.L87:
	.align	2
.L86:
	.word	.LANCHOR0+44
	.word	.LC0
	.word	g_vsys_dev
	.fnend
	.size	vsys_remove_node_from_map, .-vsys_remove_node_from_map
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
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r3, [r3]
	ldr	r6, [r3, #160]
	ldr	r3, [r1, #32]	@ unaligned
	cmp	r3, #76
	beq	.L89
	movs	r3, #234
	ldr	r2, .L95
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L95+4
	movs	r0, #0
	bl	vlog
.L89:
	movs	r5, #0
	movs	r3, #1
	strb	r5, [r4, #1]
	strb	r3, [r4]
	strb	r5, [r4, #2]
	strb	r5, [r4, #3]
	strb	r5, [r4, #4]
	strb	r5, [r4, #5]
	strb	r5, [r4, #6]
	strb	r5, [r4, #7]
	strb	r5, [r4, #8]
	strb	r5, [r4, #9]
	strb	r5, [r4, #10]
	strb	r5, [r4, #11]
	ldr	r0, [r6]
	bl	strlen
	cmp	r0, #16
	ite	ls
	strls	r0, [r4, #12]	@ unaligned
	strbhi	r5, [r4, #13]
	add	r0, r4, #16
	itttt	hi
	strbhi	r5, [r4, #14]
	movhi	r3, #16
	strbhi	r5, [r4, #15]
	strbhi	r3, [r4, #12]
	ldr	r2, [r4, #12]	@ unaligned
	ldr	r1, [r6]
	bl	strncpy
	movs	r0, #0
	strb	r0, [r4, #36]
	strb	r0, [r4, #37]
	strb	r0, [r4, #38]
	strb	r0, [r4, #39]
	strb	r0, [r4, #56]
	strb	r0, [r4, #57]
	strb	r0, [r4, #58]
	strb	r0, [r4, #59]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L96:
	.align	2
.L95:
	.word	.LC1
	.word	.LANCHOR0+70
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
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r1
	ldr	r3, [r1, #12]	@ unaligned
	.pad #32
	sub	sp, sp, #32
	cmp	r3, #28
	beq	.L98
	mov	r3, #260
	ldr	r2, .L115
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L115+4
	movs	r0, #0
	bl	vlog
.L98:
	ldr	r0, [r4]	@ unaligned
	ldr	r3, [r4, #8]	@ unaligned
	cmp	r0, #6
	bne	.L99
	adds	r3, r3, #1
.L112:
	str	r3, [r4, #8]	@ unaligned
.L100:
	ldr	r2, [r4, #8]	@ unaligned
	ldr	r1, [r4, #4]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r6, r0
	cbnz	r0, .L101
	mov	r3, #272
	ldr	r2, .L115+8
	str	r3, [sp]
	ldr	r3, .L115+4
.L114:
	movs	r1, #2
	bl	vlog
	mov	r0, #-1
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, pc}
.L99:
	cmp	r0, #17
	bne	.L100
	adds	r3, r3, #2
	b	.L112
.L101:
	ldr	r0, [r4, #16]	@ unaligned
	cmp	r0, #6
	bne	.L103
	movs	r2, #5
	ldr	r1, [r4, #20]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbz	r0, .L103
	ldr	r3, [r4, #20]	@ unaligned
	movs	r1, #4
	ldr	r2, .L115+12
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #281
	str	r3, [sp]
	ldr	r3, .L115+4
	bl	vlog
	movs	r3, #5
	str	r3, [sp, #24]
	ldr	r3, [r4, #20]	@ unaligned
	str	r3, [sp, #20]
	ldr	r3, [r4, #16]	@ unaligned
	str	r3, [sp, #16]
	ldr	r3, [r4, #8]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #284
.L113:
.L104:
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L115+4
	movs	r0, #0
	ldr	r2, .L115+16
	bl	vlog
	mov	r1, r5
	mov	r0, r6
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	mpi_node_bind
.L103:
	ldr	r2, [r4, #24]	@ unaligned
	ldr	r1, [r4, #20]	@ unaligned
	ldr	r0, [r4, #16]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbnz	r0, .L105
	mov	r3, #294
	ldr	r2, .L115+20
	str	r3, [sp]
	ldr	r3, .L115+4
	b	.L114
.L105:
	ldr	r3, [r4, #24]	@ unaligned
	str	r3, [sp, #24]
	ldr	r3, [r4, #20]	@ unaligned
	str	r3, [sp, #20]
	ldr	r3, [r4, #16]	@ unaligned
	str	r3, [sp, #16]
	ldr	r3, [r4, #8]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #300
	b	.L113
.L116:
	.align	2
.L115:
	.word	.LC2
	.word	.LANCHOR0+83
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
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
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r1
	ldr	r3, [r1, #12]	@ unaligned
	.pad #32
	sub	sp, sp, #32
	cmp	r3, #28
	beq	.L118
	mov	r3, #318
	ldr	r2, .L135
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L135+4
	movs	r0, #0
	bl	vlog
.L118:
	ldr	r0, [r4]	@ unaligned
	ldr	r3, [r4, #8]	@ unaligned
	cmp	r0, #6
	bne	.L119
	adds	r3, r3, #1
.L132:
	str	r3, [r4, #8]	@ unaligned
.L120:
	ldr	r2, [r4, #8]	@ unaligned
	ldr	r1, [r4, #4]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r6, r0
	cbnz	r0, .L121
	mov	r3, #330
	ldr	r2, .L135+8
	str	r3, [sp]
	ldr	r3, .L135+4
.L134:
	movs	r1, #2
	bl	vlog
	mov	r0, #-1
.L117:
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, pc}
.L119:
	cmp	r0, #17
	bne	.L120
	adds	r3, r3, #2
	b	.L132
.L121:
	ldr	r0, [r4, #16]	@ unaligned
	cmp	r0, #6
	bne	.L123
	movs	r2, #5
	ldr	r1, [r4, #20]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbz	r0, .L123
	ldr	r3, [r4, #20]	@ unaligned
	movs	r1, #4
	ldr	r2, .L135+12
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #339
	str	r3, [sp]
	ldr	r3, .L135+4
	bl	vlog
	movs	r3, #5
	str	r3, [sp, #24]
	ldr	r3, [r4, #20]	@ unaligned
	str	r3, [sp, #20]
	ldr	r3, [r4, #16]	@ unaligned
	str	r3, [sp, #16]
	ldr	r3, [r4, #8]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #342
.L133:
.L124:
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L135+4
	movs	r0, #0
	ldr	r2, .L135+16
	bl	vlog
	mov	r0, r6
	mov	r1, r5
	bl	mpi_node_unbind
	movs	r0, #0
	b	.L117
.L123:
	ldr	r2, [r4, #24]	@ unaligned
	ldr	r1, [r4, #20]	@ unaligned
	ldr	r0, [r4, #16]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbnz	r0, .L125
	movw	r3, #351
	ldr	r2, .L135+20
	str	r3, [sp]
	ldr	r3, .L135+4
	b	.L134
.L125:
	ldr	r3, [r4, #24]	@ unaligned
	str	r3, [sp, #24]
	ldr	r3, [r4, #20]	@ unaligned
	str	r3, [sp, #20]
	ldr	r3, [r4, #16]	@ unaligned
	str	r3, [sp, #16]
	ldr	r3, [r4, #8]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #4]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #357
	b	.L133
.L136:
	.align	2
.L135:
	.word	.LC2
	.word	.LANCHOR0+93
	.word	.LC3
	.word	.LC7
	.word	.LC8
	.word	.LC6
	.fnend
	.size	vsys_unbind, .-vsys_unbind
	.align	1
	.global	vsys_main
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_main, %function
vsys_main:
	.fnstart
	@ args = 0, pretend = 0, frame = 472
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L148
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #492
	sub	sp, sp, #492
	ldr	r3, [r3]
	movs	r4, #0
	strd	r0, r1, [sp, #16]
	movs	r1, #109
	strb	r1, [sp, #168]
	add	r9, sp, #32
	str	r3, [sp, #484]
	mov	fp, #1
	ldr	r3, .L148+4
	add	r8, sp, #36
	ldr	r1, .L148+8
	add	r7, sp, #40
	str	r4, [sp, #32]
	add	r6, sp, #48
	str	r3, [sp, #52]
	movs	r3, #2
	str	r3, [sp, #100]
	add	r10, sp, #44
	ldr	r3, .L148+12
	add	r2, sp, #56
	strd	r1, r9, [sp, #172]
	add	r5, sp, #52
	ldr	r1, .L148+16
	add	r0, sp, #60
	strd	r3, r4, [sp, #108]
	ldr	r3, .L148+20
	str	r4, [sp, #36]
	str	r4, [sp, #40]
	str	r3, [sp, #116]
	ldr	r3, .L148+24
	str	r4, [sp, #44]
	str	fp, [sp, #48]
	strd	r3, r4, [sp, #120]
	ldr	r3, .L148+28
	str	r4, [sp, #56]
	strb	r4, [sp, #104]
	strd	r3, r4, [sp, #148]
	movs	r3, #4
	strd	fp, fp, [sp, #128]
	str	r3, [sp, #164]
	strb	r4, [sp, #136]
	strd	r4, r4, [sp, #140]
	strd	r4, r4, [sp, #156]
	str	r1, [sp, #180]
	ldr	r1, .L148+32
	strd	r4, r3, [sp, #192]
	str	r3, [sp, #228]
	strd	r1, r4, [sp, #184]
	movs	r1, #100
	strb	r1, [sp, #200]
	ldr	r1, .L148+36
	str	r3, [sp, #260]
	str	r3, [sp, #292]
	strd	r1, r8, [sp, #204]
	ldr	r1, .L148+40
	strd	r4, r4, [sp, #220]
	strd	r4, r4, [sp, #252]
	strd	r1, r4, [sp, #212]
	movs	r1, #99
	strb	r1, [sp, #232]
	ldr	r1, .L148+44
	strd	r4, r4, [sp, #284]
	strd	r1, r7, [sp, #236]
	ldr	r1, .L148+48
	strd	r1, r4, [sp, #244]
	movs	r1, #67
	strb	r1, [sp, #264]
	ldr	r1, .L148+52
	strd	r1, r6, [sp, #268]
	ldr	r1, .L148+56
	strd	r1, r4, [sp, #276]
	movs	r1, #105
	strb	r1, [sp, #296]
	ldr	r1, .L148+60
	strd	r1, r10, [sp, #300]
	ldr	r1, .L148+64
	strd	r1, r4, [sp, #308]
	add	r1, sp, #100
	str	r3, [sp, #324]
	ldr	r3, .L148+68
	strd	r4, r4, [sp, #316]
	strb	r4, [sp, #328]
	strd	r3, r2, [sp, #332]
	ldr	r3, .L148+72
	ldr	r2, .L148+76
	strd	r4, r4, [sp, #348]
	strd	r3, r4, [sp, #340]
	movs	r3, #6
	str	r3, [sp, #356]
	movs	r3, #111
	strb	r3, [sp, #360]
	ldr	r3, .L148+80
	strd	r4, r4, [sp, #380]
	str	r4, [sp, #388]
	strd	r3, r5, [sp, #364]
	ldr	r3, .L148+84
	strb	r4, [sp, #392]
	strd	r4, r4, [sp, #396]
	strd	r3, r4, [sp, #372]
	mov	r3, r4
	strd	r4, r4, [sp, #404]
	strd	r4, r4, [sp, #412]
	bl	argparse_init
	ldr	r2, .L148+88
	ldr	r1, .L148+92
	add	r0, sp, #60
	bl	argparse_describe
	ldrd	r1, r2, [sp, #16]
	add	r0, sp, #60
	bl	argparse_parse
	ldr	r3, [r5]
	ldr	r0, .L148+96
	str	r3, [sp, #8]
	ldr	r3, [r6]
	str	r3, [sp, #4]
	ldr	r3, [r10]
	str	r3, [sp]
	ldr	r3, [r7]
	ldr	r2, [r8]
	ldr	r1, [r9]
	bl	printk
	ldr	r3, [r10]
	ldr	r9, [r9]
	ldr	r8, [r8]
	ldr	r7, [r7]
	mov	r0, r9
	mov	r1, r8
	mov	r2, r7
	str	r3, [sp, #16]
	ldr	r3, [r6]
	str	r3, [sp, #20]
	ldr	r3, [r5]
	str	r3, [sp, #24]
	bl	vsys_get_node_from_map
	ldr	r3, .L148
	mov	r6, r0
	str	r3, [sp, #28]
	cbnz	r0, .L138
	movs	r3, #97
	strd	r8, r7, [sp, #8]
	str	r3, [sp]
	movs	r1, #2
	str	r9, [sp, #4]
	ldr	r3, .L148+100
	ldr	r2, .L148+104
	bl	vlog
.L139:
	ldr	r3, .L148
	movs	r0, #0
	ldr	r2, [sp, #484]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L146
	bl	__stack_chk_fail
.L138:
	ldr	r3, .L148+108
	mov	r1, #3264
	ldr	r0, [r3, #24]
	bl	kmem_cache_alloc
	mov	r5, r0
	cbnz	r0, .L140
	movs	r3, #102
	ldr	r2, .L148+112
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L148+100
	bl	vlog
	b	.L139
.L140:
	ldr	r3, .L148+116
	add	r10, sp, #420
	ldr	r0, [sp, #16]
	ldr	r1, .L148+120
	ldr	r2, [r3, r9, lsl #2]
	ldr	r3, .L148+124
	strd	r2, r8, [sp]
	cmp	r0, #0
	it	eq
	moveq	r3, r1
	ldr	r2, .L148+128
	movs	r1, #64
	strd	r7, r3, [sp, #8]
	mov	r0, r10
	ldr	r3, [sp, #24]
	bl	snprintf
	mov	r2, #420
	movw	r1, #577
	mov	r0, r10
	bl	filp_open
	cmn	r0, #4096
	str	r0, [r5, #36]
	bls	.L142
	movs	r3, #108
	str	r10, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L148+100
	mov	r0, r4
	ldr	r2, .L148+132
	bl	vlog
	ldr	r0, [r5, #36]
	cmn	r0, #4096
	bhi	.L144
	mov	r1, r4
	bl	filp_close
.L144:
	mov	r0, r5
	bl	kfree
	b	.L139
.L142:
	ldr	r3, [sp, #16]
	movw	r1, #8240
	ldr	r0, .L148+136
	movs	r2, #0
	strd	r9, r8, [r5, #16]
	str	r3, [r5, #32]
	ldr	r3, [sp, #20]
	str	r7, [r5, #24]
	str	r6, [r5, #12]
	str	r3, [r5, #28]
	movs	r3, #0
	strd	r2, [r5, #40]
	adds	r3, r5, #4
	ldr	r2, [r0, r1]
	str	r3, [r0, r1]
	ldr	r1, .L148+140
	strd	r1, r2, [r5, #4]
	str	r3, [r2]
	ldr	r2, [sp, #16]
	ldr	r3, .L148+144
	cbz	r2, .L145
	ldr	r2, [r6, #332]
	str	r2, [r5]
	strd	fp, r3, [r6, #328]
	b	.L139
.L145:
	ldr	r2, [r6, #344]
	str	r2, [r5]
	strd	fp, r3, [r6, #340]
	b	.L139
.L146:
	add	sp, sp, #492
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L149:
	.align	2
.L148:
	.word	__stack_chk_guard
	.word	.LC11
	.word	.LC15
	.word	.LC12
	.word	.LC16
	.word	.LC13
	.word	argparse_help_cb
	.word	.LC14
	.word	m_callback
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.word	.LANCHOR0+108
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LANCHOR0+116
	.word	.LC32
	.word	kmalloc_caches
	.word	.LC33
	.word	module_name
	.word	.LC10
	.word	.LC9
	.word	.LC34
	.word	.LC35
	.word	g_vsys_dev
	.word	g_vsys_dev+8236
	.word	dumpsys_callback
	.fnend
	.size	vsys_main, .-vsys_main
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_write, %function
vsys_write:
	.fnstart
	@ args = 0, pretend = 0, frame = 520
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #520
	sub	sp, sp, #520
	ldr	r5, .L152
	cmp	r2, #510
	mov	r4, r2
	mov	r6, r2
	it	cs
	movcs	r4, #510
	ldr	r3, [r5]
	mov	r2, r4
	str	r3, [sp, #516]
	add	r3, sp, #4
	mov	r0, r3
	bl	memcpy
	movs	r2, #0
	ldr	r1, .L152+4
	strb	r2, [r0, r4]
	add	r4, r4, r0
	strb	r2, [r4, #-1]
	bl	argparse_excute_main
	mov	r0, r6
	ldr	r2, [sp, #516]
	ldr	r3, [r5]
	cmp	r2, r3
	beq	.L151
	bl	__stack_chk_fail
.L151:
	add	sp, sp, #520
	@ sp needed
	pop	{r4, r5, r6, pc}
.L153:
	.align	2
.L152:
	.word	__stack_chk_guard
	.word	vsys_main
	.fnend
	.size	vsys_write, .-vsys_write
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
	.pad #100
	sub	sp, sp, #100
	ldr	r5, [r0, #128]
	mov	r9, r2
	str	r1, [sp, #92]
	adds	r5, r5, #4
	mov	r0, r5
	bl	mpi_fs_buf_read
	adds	r3, r0, #1
	mov	r4, r0
	bne	.L155
	ldr	r3, .L186
	mov	r0, r5
	ldr	r2, .L186+4
	ldr	r1, .L186+8
	str	r3, [sp]
	ldr	r7, .L186+12
	ldr	r3, .L186+16
	ldr	r2, [r2]
	bl	mpi_fs_buf_add
	ldr	r1, .L186+20
	mov	r0, r5
	ldr	r10, .L186+52
	bl	mpi_fs_buf_add
	ldr	r1, .L186+24
	mov	r0, r5
	ldr	fp, .L186+44
	bl	mpi_fs_buf_add
	ldr	r4, [r7]
	subs	r4, r4, #36
.L156:
	add	r3, r4, #36
	cmp	r3, r7
	beq	.L164
	ldr	r1, [r4, #12]
	mov	r0, r5
	ldr	r2, [r4, #16]
	ldr	r3, [r4, #44]
	str	r1, [sp, #80]
	ldr	r1, [r4, #284]
	str	r1, [sp, #76]
	ldr	r1, [r4, #280]
	str	r1, [sp, #72]
	ldr	r1, [r4, #384]
	str	r1, [sp, #68]
	ldr	r1, [r4, #392]
	str	r1, [sp, #64]
	ldr	r1, [r4, #400]
	str	r1, [sp, #60]
	ldr	r1, [r4, #204]
	str	r1, [sp, #56]
	ldr	r1, [r4, #196]
	str	r1, [sp, #52]
	ldr	r1, [r4, #192]
	str	r1, [sp, #48]
	ldr	r1, [r4, #184]
	str	r1, [sp, #44]
	ldr	r1, [r4, #380]
	str	r1, [sp, #40]
	ldr	r1, [r4, #388]
	str	r1, [sp, #36]
	ldr	r1, [r4, #396]
	str	r1, [sp, #32]
	ldr	r1, [r4, #144]
	str	r1, [sp, #28]
	ldr	r1, [r4, #136]
	str	r1, [sp, #24]
	ldr	r1, [r4, #132]
	str	r1, [sp, #20]
	ldr	r1, [r4, #124]
	strd	r2, r1, [sp, #12]
	ldr	r2, [r4, #76]
	ldr	r1, .L186+28
	str	r2, [sp, #8]
	ldr	r2, [r4, #56]
	strd	r4, r2, [sp]
	ldr	r3, [r3, #44]
	ldr	r2, [r4]
	bl	mpi_fs_buf_add
	cbz	r0, .L157
.L164:
	ldr	r1, .L186+32
	mov	r0, r5
	bl	mpi_fs_buf_add
	ldr	r1, .L186+36
	mov	r0, r5
	ldr	r8, .L186+56
	bl	mpi_fs_buf_add
	ldr	r4, [r7]
	ldr	r10, .L186+60
	subs	r4, r4, #36
.L158:
	add	r3, r4, #36
	cmp	r3, r7
	bne	.L174
.L166:
	mov	r2, r9
	ldr	r1, [sp, #92]
	mov	r0, r5
	add	sp, sp, #100
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	mpi_fs_buf_read
.L157:
	ldr	r6, [r4, #300]
	add	r8, r4, #300
	subs	r6, r6, #4
.L159:
	adds	r2, r6, #4
	cmp	r2, r8
	beq	.L162
	ldr	r2, [r6]
	mov	r1, r10
	mov	r0, r5
	ldr	r2, [r2]
	bl	mpi_fs_buf_add
	cbz	r0, .L160
.L162:
	mov	r1, fp
	mov	r0, r5
	bl	mpi_fs_buf_add
	ldr	r4, [r4, #36]
	subs	r4, r4, #36
	b	.L156
.L160:
	ldr	r6, [r6, #4]
	subs	r6, r6, #4
	b	.L159
.L174:
	ldr	r3, [r4, #144]
	cbz	r3, .L165
	ldr	r3, [r4, #44]
	mov	r1, r8
	ldr	r2, .L186+40
	mov	r0, r5
	str	r2, [sp]
	ldr	r3, [r3, #44]
	ldr	r2, [r4]
	bl	mpi_fs_buf_add
	cmp	r0, #0
	bne	.L166
	mvn	fp, #151
	add	r6, r4, #152
	sub	fp, fp, r4
.L167:
	ldr	r1, [r4, #144]
	add	r2, fp, r6
	cmp	r1, r2
	ble	.L169
	ldrb	r2, [r6], #1	@ zero_extendqisi2
	mov	r1, r10
	mov	r0, r5
	bl	mpi_fs_buf_add
	cmp	r0, #0
	beq	.L167
.L169:
	ldr	r1, .L186+44
	mov	r0, r5
	bl	mpi_fs_buf_add
.L165:
	ldr	r3, [r4, #204]
	cbz	r3, .L170
	ldr	r3, [r4, #44]
	mov	r1, r8
	ldr	r2, .L186+48
	mov	r0, r5
	str	r2, [sp]
	ldr	r3, [r3, #44]
	ldr	r2, [r4]
	bl	mpi_fs_buf_add
	cmp	r0, #0
	bne	.L166
	mvn	fp, #211
	add	r6, r4, #212
	sub	fp, fp, r4
.L171:
	ldr	r1, [r4, #204]
	add	r2, fp, r6
	cmp	r1, r2
	ble	.L173
	ldrb	r2, [r6], #1	@ zero_extendqisi2
	mov	r1, r10
	mov	r0, r5
	bl	mpi_fs_buf_add
	cmp	r0, #0
	beq	.L171
.L173:
	ldr	r1, .L186+44
	mov	r0, r5
	bl	mpi_fs_buf_add
.L170:
	ldr	r4, [r4, #36]
	subs	r4, r4, #36
	b	.L158
.L155:
	cbnz	r0, .L154
	mov	r0, r5
	bl	mpi_fs_buf_reset
.L154:
	mov	r0, r4
	add	sp, sp, #100
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L187:
	.align	2
.L186:
	.word	.LC38
	.word	g_vsys_dev
	.word	.LC37
	.word	mpi_node_list
	.word	.LC36
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC42
	.word	.LC43
	.word	.LC9
	.word	.LC45
	.word	.LC10
	.word	.LC44
	.word	.LC46
	.word	.LC47
	.fnend
	.size	vsys_read, .-vsys_read
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_force_lost_frame, %function
vsys_force_lost_frame:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r1, #12]	@ unaligned
	cmp	r3, #20
	beq	.L189
	movw	r3, #558
	ldr	r2, .L200
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L200+4
	movs	r0, #0
	bl	vlog
.L189:
	ldr	r0, [r4]	@ unaligned
	ldr	r3, [r4, #8]	@ unaligned
	cmp	r0, #6
	bne	.L190
	adds	r3, r3, #1
.L198:
	str	r3, [r4, #8]	@ unaligned
.L191:
	ldr	r2, [r4, #8]	@ unaligned
	ldr	r1, [r4, #4]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r3, r0
	cbnz	r0, .L192
	movw	r3, #569
	ldr	r2, .L200+8
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L200+4
	bl	vlog
	mov	r0, #-1
.L188:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L190:
	cmp	r0, #17
	bne	.L191
	adds	r3, r3, #2
	b	.L198
.L192:
	ldr	r2, [r4]	@ unaligned
	ldr	r1, [r4, #16]	@ unaligned
	cmp	r2, #8
	ite	eq
	addeq	r0, r0, #208
	addne	r0, r3, #148
	bl	mpi_node_force_lost_frame
	movs	r0, #0
	b	.L188
.L201:
	.align	2
.L200:
	.word	.LC48
	.word	.LANCHOR0+130
	.word	.LC49
	.fnend
	.size	vsys_force_lost_frame, .-vsys_force_lost_frame
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_free_all_cma, %function
vsys_free_all_cma:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	free_all_cma
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	vsys_free_all_cma, .-vsys_free_all_cma
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsys_wait_rcu, %function
vsys_wait_rcu:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	synchronize_rcu
	bl	flush_delayed_fput
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	vsys_wait_rcu, .-vsys_wait_rcu
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
	mov	r4, r1
	ldr	r3, [r1, #16]	@ unaligned
	ldr	r6, [r0, #128]
	cmp	r3, #536
	beq	.L205
	mov	r3, #458
	ldr	r2, .L222
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L222+4
	movs	r0, #0
	bl	vlog
.L205:
	ldr	r0, .L222+8
	bl	mpi_allocator_find_by_name
	mov	r7, r0
	cbnz	r0, .L206
	movw	r3, #463
	ldr	r2, .L222+12
	str	r3, [sp]
	ldr	r3, .L222+4
	str	r0, [sp, #4]
.L221:
	movs	r1, #2
	bl	vlog
	mov	r0, #-1
.L204:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L206:
	mov	r2, sp
	bic	r3, r2, #8160
	bic	r3, r3, #31
	movs	r1, #1
	ldr	r3, [r3, #12]
	add	r3, r3, #736
	str	r3, [r2]
	movs	r3, #0
	ldr	r2, [r4, #12]	@ unaligned
	bl	mpi_buf_pool_create
	mov	r5, r0
	cbnz	r0, .L208
	movw	r3, #469
	str	r7, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L222+16
	ldr	r3, .L222+4
	b	.L221
.L208:
	ldr	r2, [r6, #28]
	add	r3, r0, #48
	adds	r6, r6, #24
	str	r3, [r6, #4]
	ldr	r9, .L222+4
	strd	r6, r2, [r0, #48]
	movs	r6, #0
	str	r3, [r2]
	ldr	r3, [r4, #20]	@ unaligned
	ldr	r10, .L222+20
	str	r3, [r0, #92]
	ldr	r3, [r4]	@ unaligned
	str	r3, [r0, #44]
.L209:
	ldr	r3, [r4, #4]	@ unaligned
	cmp	r3, r6
	bgt	.L214
	movs	r0, #0
	b	.L204
.L214:
	add	fp, r4, r6, lsl #2
	ldr	r8, [r4, #8]	@ unaligned
	ldr	r0, [fp, #24]	@ unaligned
	cmp	r8, #0
	beq	.L210
	bl	dma_buf_get
	mov	r7, r0
	cbz	r0, .L212
	mov	r1, r0
	movs	r2, #1
	mov	r0, r5
	bl	mpi_buf_pool_import_dmabuf
	mov	r0, r7
	bl	dma_buf_put
.L212:
	adds	r6, r6, #1
	b	.L209
.L210:
	movs	r1, #4
	bl	mpi_obj_from_id
	ldr	r3, [fp, #24]	@ unaligned
	mov	r2, r10
	movs	r1, #4
	mov	r7, r0
	str	r3, [sp, #4]
	mov	r0, r8
	movw	r3, #485
	str	r3, [sp]
	mov	r3, r9
	bl	vlog
	cmp	r7, #0
	beq	.L212
	mov	r1, r7
	mov	r0, r5
	bl	mpi_buf_pool_import
	mov	r0, r7
	bl	mpi_obj_unref
	b	.L212
.L223:
	.align	2
.L222:
	.word	.LC50
	.word	.LANCHOR0+152
	.word	.LC51
	.word	.LC52
	.word	.LC53
	.word	.LC54
	.fnend
	.size	vsys_pool_create, .-vsys_pool_create
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	m_callback, %function
m_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movs	r4, #0
	ldr	r5, .L227
	ldr	r6, .L227+4
.L225:
	mov	r1, r4
	ldr	r2, [r5], #4
	mov	r0, r6
	adds	r4, r4, #1
	bl	printk
	cmp	r4, #24
	bne	.L225
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L228:
	.align	2
.L227:
	.word	module_name
	.word	.LC55
	.fnend
	.size	m_callback, .-m_callback
	.global	__aeabi_idivmod
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	dumpsys_callback, %function
dumpsys_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r9, r0
	ldr	r4, .L238
	mov	r8, r1
	ldr	r3, [r4, #184]
	cbnz	r3, .L229
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movw	r3, #8244
	movw	r2, #8248
	ldr	r3, [r4, r3]
	ldr	r2, [r4, r2]
	subs	r3, r3, r2
	cmp	r3, #19
	bgt	.L231
	cbz	r1, .L232
	mov	r0, r1
	bl	mpi_obj_ref
.L232:
	movw	r6, #8244
	movs	r1, #20
	ldr	r5, [r4, r6]
	mov	r0, r5
	bl	__aeabi_idivmod
	adds	r0, r5, #1
	cmp	r0, #65536
	addw	r3, r1, #2062
	addw	r1, r1, #2082
	add	r3, r4, r3, lsl #2
	add	r1, r4, r1, lsl #2
	str	r9, [r3, #4]
	str	r8, [r1, #4]
	bgt	.L233
	str	r0, [r4, r6]
.L234:
	ldr	r0, .L238+4
	bl	up
.L231:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L229:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L233:
	movs	r1, #20
	movw	r5, #8248
	bl	__aeabi_idivmod
	ldr	r0, [r4, r5]
	str	r1, [r4, r6]
	movs	r1, #20
	bl	__aeabi_idivmod
	str	r1, [r4, r5]
	b	.L234
.L239:
	.align	2
.L238:
	.word	g_vsys_dev
	.word	g_vsys_dev+168
	.fnend
	.size	dumpsys_callback, .-dumpsys_callback
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
	ldr	r3, .L244
	mov	r6, r0
	mov	r5, r1
	mov	r7, r2
	adds	r3, r3, #4
	ldr	r2, .L244+4
	ldr	r8, [r3, #160]
	movs	r1, #5
	strd	r4, r3, [sp, #8]
	movs	r0, #0
	movw	r3, #606
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r3, .L244+8
	bl	vlog
	ldr	r0, [r8, #192]
	cmp	r0, r4
	bhi	.L241
	movw	r3, #609
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L244+8
	movs	r0, #0
	ldr	r2, .L244+12
	bl	vlog
.L242:
.L240:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L241:
	.syntax unified
@ 87 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
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
	cbnz	r0, .L243
	mov	r3, #616
	ldr	r2, .L244+16
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L244+8
	bl	vlog
	b	.L240
.L243:
	mov	r1, r0
	mov	r3, r7
	mov	r2, r5
	mov	r0, r6
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L245:
	.align	2
.L244:
	.word	g_vsys_dev
	.word	.LC56
	.word	.LANCHOR0+169
	.word	.LC57
	.word	.LC58
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
	bgt	.L312
	ldr	r4, [r3, #160]
	mov	r9, #0
	add	r5, r4, #196
	add	r6, r4, #388
.L248:
	mov	r8, r5
	movs	r7, #6
.L250:
	ldr	r0, [r8], #4
	cbz	r0, .L249
	bl	mpi_node_destroy
	str	r9, [r8, #-4]
.L249:
	subs	r7, r7, #1
	bne	.L250
	adds	r5, r5, #24
	cmp	r5, r6
	bne	.L248
	add	r8, r4, #644
	add	r5, r4, #900
	mov	r9, r8
.L253:
	ldr	r0, [r8], #4
	cbz	r0, .L252
	bl	mpi_node_destroy
	str	r7, [r8, #-4]
.L252:
	cmp	r8, r5
	bne	.L253
	add	fp, r4, #7168
	add	r8, r4, #1032
	add	fp, fp, #8
.L254:
	mov	r10, r8
	movs	r7, #5
.L256:
	ldr	r1, [r10], #4
	cbz	r1, .L255
	ldr	r0, [r8, #-4]
	bl	mpi_node_unbind
	movs	r3, #0
	str	r3, [r10, #-4]
.L255:
	subs	r7, r7, #1
	bne	.L256
	str	r7, [r8, #-4]
	add	r8, r8, #24
	cmp	r8, fp
	bne	.L254
.L259:
	ldr	r0, [r6], #4
	cbz	r0, .L258
	bl	mpi_node_destroy
	str	r7, [r6, #-4]
.L258:
	cmp	r9, r6
	bne	.L259
	addw	r6, r4, #1028
	movs	r7, #0
.L261:
	ldr	r0, [r5], #4
	cbz	r0, .L260
	bl	mpi_node_destroy
	str	r7, [r5, #-4]
.L260:
	cmp	r5, r6
	bne	.L261
	add	r5, r4, #7168
	add	r6, r4, #7168
	adds	r5, r5, #4
	adds	r6, r6, #20
	movs	r7, #0
.L263:
	ldr	r0, [r5], #4
	cbnz	r0, .L262
.L265:
	cmp	r6, r5
	bne	.L263
	add	r5, r4, #7200
	movs	r7, #0
	adds	r5, r5, #4
.L267:
	ldr	r0, [r6], #4
	cbnz	r0, .L266
.L269:
	cmp	r5, r6
	bne	.L267
	add	r4, r4, #8192
	movs	r6, #0
	adds	r4, r4, #36
.L271:
	ldr	r0, [r5], #4
	cbz	r0, .L270
	bl	mpi_node_destroy
	str	r6, [r5, #-4]
.L270:
	cmp	r5, r4
	bne	.L271
	ldr	r3, .L315
	ldr	r9, .L315+8
	ldr	r10, .L315+12
	ldr	r4, [r3]
	mov	r8, r3
	ldr	r5, [r4], #-36
	subs	r5, r5, #36
.L272:
	add	r3, r4, #36
	cmp	r3, r8
	bne	.L279
	ldr	r5, .L315+4
	ldr	r0, [r5]
	ldr	r4, [r0], #-36
	subs	r4, r4, #36
.L276:
	add	r3, r0, #36
	cmp	r3, r5
	bne	.L277
.L312:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L262:
	bl	mpi_node_destroy
	str	r7, [r5, #-4]
	b	.L265
.L266:
	bl	mpi_node_destroy
	str	r7, [r6, #-4]
	b	.L269
.L274:
	ldr	r7, [r4, #4]
	mov	r1, r9
	mov	r0, r7
	bl	strcmp
	cbz	r0, .L273
	mov	r1, r10
	mov	r0, r7
	bl	strcmp
	cbz	r0, .L273
	mov	r0, r4
	bl	mpi_obj_unref
.L273:
	adds	r6, r6, #1
.L275:
	ldr	r3, [r4, #16]
	cmp	r6, r3
	blt	.L274
	ldr	r3, [r5, #36]
	mov	r4, r5
	sub	r5, r3, #36
	b	.L272
.L279:
	movs	r6, #0
	b	.L275
.L277:
	movs	r1, #1
	bl	mpi_buf_pool_destroy
	ldr	r3, [r4, #36]
	mov	r0, r4
	sub	r4, r3, #36
	b	.L276
.L316:
	.align	2
.L315:
	.word	mpi_buf_list
	.word	mpi_buf_pool_list
	.word	.LC59
	.word	.LC60
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
	ldr	r3, .L322
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r7, r1
	ldr	r0, [r3, #20]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbnz	r0, .L318
	mov	r3, #640
	ldr	r2, .L322+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L322+8
	bl	vlog
.L321:
	mov	r6, #-1
.L317:
	mov	r0, r6
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L318:
	mov	r1, #10240
	adds	r0, r0, #4
	bl	mpi_fs_buf_init
	mov	r6, r0
	cbz	r0, .L320
	mov	r0, r4
	bl	kfree
	b	.L321
.L320:
	ldr	r5, .L322+12
	add	r3, r4, #24
	str	r3, [r4, #24]
	str	r3, [r4, #28]
	mov	r0, r5
	bl	mpi_obj_ref
	ldr	r3, [r5, #16]
	str	r5, [r4]
	subs	r5, r5, #4
	cmp	r3, #2
	str	r4, [r7, #128]
	bne	.L317
	ldr	r1, [r5, #4]
	ldr	r0, .L322+16
	bl	printk
	b	.L317
.L323:
	.align	2
.L322:
	.word	kmalloc_caches
	.word	.LC61
	.word	.LANCHOR0+180
	.word	g_vsys_dev+4
	.word	.LC62
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
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	ldr	r5, [r1, #128]
	ldr	r8, .L334+20
	ldr	r9, .L334
	ldr	r0, [r5]
	sub	r10, r8, #4
	bl	mpi_obj_unref
	ldr	r4, [r5, #24]
	ldr	r6, [r4], #-48
	subs	r6, r6, #48
.L325:
	add	r7, r4, #48
	add	r3, r5, #24
	cmp	r7, r3
	bne	.L328
	ldr	r3, [r5]
	ldr	r2, [r3, #16]
	cmp	r2, #1
	bne	.L332
	ldr	r2, [r3]
	mov	r7, #256
	ldr	r1, .L334
	mov	r8, #290
	ldr	r0, .L334+4
	bl	printk
	ldr	r2, .L334+8
	movw	r3, #8228
	ldr	r6, .L334+12
	ldr	r0, [r2, r3]
	ldr	r4, [r0], #-48
	subs	r4, r4, #48
.L330:
	add	r3, r0, #48
	cmp	r3, r6
	bne	.L331
.L332:
	adds	r0, r5, #4
	bl	mpi_fs_buf_deinit
	mov	r0, r5
	bl	kfree
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L328:
	ldr	r3, [r4]
	movs	r1, #4
	ldr	r2, .L334+16
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #662
	str	r3, [sp]
	mov	r3, r9
	bl	vlog
	ldrd	r2, r3, [r4, #48]
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r3, #290
	mov	r2, #256
	strd	r2, r3, [r4, #48]
	ldr	r3, [r4, #92]
	cbnz	r3, .L326
	movs	r1, #1
	mov	r0, r4
	bl	mpi_buf_pool_destroy
.L327:
	ldr	r3, [r6, #48]
	mov	r4, r6
	sub	r6, r3, #48
	b	.L325
.L326:
	ldr	r3, [r8]
	str	r7, [r8]
	strd	r10, r3, [r4, #48]
	str	r7, [r3]
	b	.L327
.L331:
	ldrd	r2, r3, [r0, #48]
	movs	r1, #1
	str	r3, [r2, #4]
	str	r2, [r3]
	strd	r7, r8, [r0, #48]
	bl	mpi_buf_pool_destroy
	ldr	r3, [r4, #48]
	mov	r0, r4
	sub	r4, r3, #48
	b	.L330
.L335:
	.align	2
.L334:
	.word	.LANCHOR0+190
	.word	.LC64
	.word	g_vsys_dev
	.word	g_vsys_dev+8228
	.word	.LC63
	.word	g_vsys_dev+8232
	.fnend
	.size	vsys_release, .-vsys_release
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
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	add	r7, r0, #8192
	ldr	r9, .L362
	adds	r7, r7, #44
	mov	r5, r0
.L337:
	bl	kthread_should_stop
	cbz	r0, .L350
.L338:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movw	r3, #8236
	mov	r9, #256
	ldr	r4, [r5, r3]
	mov	r10, #290
	ldr	r6, [r4], #-4
	subs	r6, r6, #4
.L351:
	adds	r3, r4, #4
	cmp	r3, r7
	bne	.L352
	add	r4, r5, #8192
	add	r5, r5, #8320
	adds	r4, r4, #60
	adds	r5, r5, #12
.L354:
	ldr	r3, [r4], #4
	cbz	r3, .L353
	ldr	r0, [r4, #76]
	bl	mpi_obj_unref
.L353:
	cmp	r4, r5
	bne	.L354
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L350:
	ldr	r3, [r5, #184]
	cmp	r3, #0
	bne	.L338
	add	r0, r5, #168
	bl	down
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r1, #20
	ldr	r0, [r9]
	bl	__aeabi_idivmod
	movw	r3, #8332
	add	r8, r5, r1, lsl #2
	ldr	r6, [r8, r3]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r4, [r7]
	movw	r10, #8252
	ldr	r1, [r8, r10]
	ldr	r3, [r4], #-4
	subs	r3, r3, #4
.L339:
	adds	r2, r4, #4
	cmp	r2, r7
	beq	.L345
	ldr	r2, [r4, #12]
	cmp	r2, r1
	bne	.L340
	ldr	r3, [r4, #28]
	cmp	r3, #0
	ble	.L341
	ldr	r1, [r6, #604]
	cbz	r1, .L341
	ldr	r3, [r6, #640]
	ldr	r0, [r6, #596]
	cmp	r3, #0
	bne	.L342
	ldrd	r3, r2, [r6, #624]
	muls	r3, r2, r3
	add	r3, r3, r3, lsl #1
	lsrs	r2, r3, #1
.L343:
	cmp	r2, r0
	add	r3, r4, #40
	it	cs
	movcs	r2, r0
	ldr	r0, [r4, #36]
	bl	kernel_write
.L341:
	ldr	r3, [r4, #28]
	subs	r3, r3, #1
	str	r3, [r4, #28]
	cbnz	r3, .L345
	ldr	r2, [r4, #32]
	ldr	r1, [r4, #12]
	cbz	r2, .L346
	str	r3, [r1, #328]
	ldr	r2, [r4, #12]
	str	r3, [r2, #332]
.L347:
	movs	r1, #0
	ldr	r0, [r4, #36]
	bl	filp_close
	ldrd	r2, r3, [r4, #4]
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r2, #256
	mov	r3, #290
	strd	r2, r3, [r4, #4]
.L345:
	movs	r2, #1
	mov	r1, r6
	ldr	r0, [r8, r10]
	bl	mpi_node_buffer_in
	mov	r0, r6
	bl	mpi_obj_unref
	movs	r3, #0
	str	r3, [r8, r10]
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r9]
	adds	r3, r3, #1
	str	r3, [r9]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L337
.L342:
	mov	r2, r0
	b	.L343
.L346:
	str	r2, [r1, #340]
	ldr	r3, [r4, #12]
	str	r2, [r3, #344]
	b	.L347
.L340:
	ldr	r2, [r3, #4]
	mov	r4, r3
	subs	r3, r2, #4
	b	.L339
.L352:
	ldr	r0, [r4, #36]
	movs	r1, #0
	bl	filp_close
	ldrd	r2, r3, [r4, #4]
	str	r3, [r2, #4]
	str	r2, [r3]
	strd	r9, r10, [r4, #4]
	mov	r4, r6
	ldr	r3, [r6, #4]
	subs	r6, r3, #4
	b	.L351
.L363:
	.align	2
.L362:
	.word	g_vsys_dev+8248
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
	ldr	r3, [r1, #16]	@ unaligned
	ldr	r4, [r0, #128]
	cmp	r3, #536
	beq	.L365
	movw	r3, #509
	ldr	r2, .L373
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L373+4
	movs	r0, #0
	bl	vlog
.L365:
	ldr	r3, [r5]	@ unaligned
	movs	r1, #4
	ldr	r2, .L373+8
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #511
	str	r3, [sp]
	ldr	r3, .L373+4
	bl	vlog
	mov	r3, r4
	ldr	r0, [r3, #24]!
	ldr	r2, [r0], #-48
	subs	r2, r2, #48
.L366:
	add	r1, r0, #48
	cmp	r1, r3
	beq	.L368
	ldr	r1, [r5]	@ unaligned
	ldr	r4, [r0, #44]
	cmp	r4, r1
	bne	.L367
	ldrd	r2, r3, [r0, #48]
	movs	r1, #1
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r2, #256
	mov	r3, #290
	strd	r2, r3, [r0, #48]
	bl	mpi_buf_pool_destroy
.L368:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L367:
	ldr	r1, [r2, #48]
	mov	r0, r2
	sub	r2, r1, #48
	b	.L366
.L374:
	.align	2
.L373:
	.word	.LC50
	.word	.LANCHOR0+203
	.word	.LC65
	.fnend
	.size	vsys_pool_delete, .-vsys_pool_delete
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
	movw	r2, #8412
	ldr	r4, .L377
	mov	r5, r0
	movs	r1, #0
	ldr	r6, .L377+4
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r5
	bl	kstrdup
	mov	r2, r5
	movs	r1, #8
	movs	r5, #0
	movs	r3, #10
	str	r4, [r4, #164]
	str	r3, [r4, #192]
	str	r6, [r4, #188]
	str	r0, [r4]
	adds	r0, r4, #4
	bl	mpi_dev_register
	mov	r2, r5
	add	r0, r4, #172
	add	r1, r6, #200
	strd	r0, r0, [r4, #172]
	adds	r0, r4, #4
	str	r5, [r4, #72]
	strd	r5, r5, [r4, #80]
	str	r5, [r4, #168]
	bl	mpi_sysfs_dev_add
	movw	r3, #8244
	mov	r2, #-1
	str	r5, [r4, r3]
	movw	r3, #8248
	str	r5, [r4, r3]
	mov	r1, r4
	ldr	r3, .L377+8
	ldr	r0, .L377+12
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L376
	bl	wake_up_process
.L376:
	ldr	r3, .L377+16
	movw	r2, #8228
	str	r5, [r4, #180]
	str	r3, [r4, r2]
	movw	r2, #8232
	str	r3, [r4, r2]
	movw	r2, #8236
	adds	r3, r3, #8
	str	r3, [r4, r2]
	movw	r2, #8240
	str	r3, [r4, r2]
	pop	{r4, r5, r6, pc}
.L378:
	.align	2
.L377:
	.word	g_vsys_dev
	.word	.LANCHOR0+220
	.word	.LC66
	.word	vsys_thread_fn
	.word	g_vsys_dev+8228
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
	push	{r4, lr}
	.save {r4, lr}
	movs	r3, #1
	ldr	r4, .L380
	ldr	r0, [r4, #180]
	str	r3, [r4, #184]
	bl	kthread_stop
	adds	r0, r4, #4
	bl	mpi_dev_unregister
	ldr	r0, [r4]
	pop	{r4, lr}
	b	kfree
.L381:
	.align	2
.L380:
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
	ldr	r0, .L383
	bl	vsys_probe
	movs	r0, #0
	pop	{r3, pc}
.L384:
	.align	2
.L383:
	.word	.LC66
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
	ldr	r7, .L431
.L387:
	adds	r6, r7, r4
	movs	r5, #0
.L389:
	ldr	r0, [r6], #4
	cbz	r0, .L388
	bl	mpi_node_stream_pause
.L388:
	adds	r5, r5, #1
	cmp	r5, #6
	bne	.L389
	adds	r4, r4, #24
	cmp	r4, #192
	bne	.L387
	ldr	r5, .L431+4
	movs	r4, #0
.L392:
	ldr	r0, [r5], #4
	cbz	r0, .L391
	bl	mpi_node_stream_pause
.L391:
	adds	r4, r4, #1
	cmp	r4, #64
	bne	.L392
	ldr	r5, .L431+8
	movs	r4, #0
.L394:
	ldr	r0, [r5], #4
	cbz	r0, .L393
	bl	mpi_node_stream_pause
.L393:
	adds	r4, r4, #1
	cmp	r4, #64
	bne	.L394
	ldr	r5, .L431+12
	movs	r4, #0
.L396:
	ldr	r0, [r5], #4
	cbz	r0, .L395
	bl	mpi_node_stream_pause
.L395:
	adds	r4, r4, #1
	cmp	r4, #32
	bne	.L396
	ldr	r5, .L431+16
	movs	r4, #0
.L398:
	ldr	r0, [r5], #4
	cbnz	r0, .L397
.L400:
	adds	r4, r4, #1
	cmp	r4, #4
	bne	.L398
	ldr	r5, .L431+20
	movs	r4, #0
.L402:
	ldr	r0, [r5], #4
	cbnz	r0, .L401
.L404:
	adds	r4, r4, #1
	cmp	r4, #4
	bne	.L402
	ldr	r5, .L431+24
	movs	r4, #0
.L406:
	ldr	r0, [r5], #4
	cbz	r0, .L405
	bl	mpi_node_stream_pause
.L405:
	adds	r4, r4, #1
	cmp	r4, #256
	bne	.L406
	movs	r0, #100
	pop	{r3, r4, r5, r6, r7, lr}
	b	msleep
.L397:
	bl	mpi_node_stream_pause
	b	.L400
.L401:
	bl	mpi_node_stream_pause
	b	.L404
.L432:
	.align	2
.L431:
	.word	g_vsys_dev+196
	.word	g_vsys_dev+644
	.word	g_vsys_dev+388
	.word	g_vsys_dev+900
	.word	g_vsys_dev+7172
	.word	g_vsys_dev+7188
	.word	g_vsys_dev+7204
	.fnend
	.size	vsys_close, .-vsys_close
	.global	g_vsys_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.36364, %object
	.size	__func__.36364, 21
__func__.36364:
	.ascii	"vsys_add_node_to_map\000"
	.type	__func__.36387, %object
	.size	__func__.36387, 23
__func__.36387:
	.ascii	"vsys_get_node_from_map\000"
	.type	__func__.36409, %object
	.size	__func__.36409, 26
__func__.36409:
	.ascii	"vsys_remove_node_from_map\000"
	.type	__func__.36113, %object
	.size	__func__.36113, 13
__func__.36113:
	.ascii	"vsys_version\000"
	.type	__func__.36121, %object
	.size	__func__.36121, 10
__func__.36121:
	.ascii	"vsys_bind\000"
	.type	__func__.36130, %object
	.size	__func__.36130, 12
__func__.36130:
	.ascii	"vsys_unbind\000"
	.space	3
	.type	usages, %object
	.size	usages, 8
usages:
	.word	.LC67
	.word	0
	.type	__func__.35978, %object
	.size	__func__.35978, 14
__func__.35978:
	.ascii	"set_dump_info\000"
	.type	__func__.36247, %object
	.size	__func__.36247, 22
__func__.36247:
	.ascii	"vsys_force_lost_frame\000"
	.type	__func__.36202, %object
	.size	__func__.36202, 17
__func__.36202:
	.ascii	"vsys_pool_create\000"
	.type	__func__.36260, %object
	.size	__func__.36260, 11
__func__.36260:
	.ascii	"vsys_ioctl\000"
	.type	__func__.36282, %object
	.size	__func__.36282, 10
__func__.36282:
	.ascii	"vsys_open\000"
	.type	__func__.36310, %object
	.size	__func__.36310, 13
__func__.36310:
	.ascii	"vsys_release\000"
	.type	__func__.36214, %object
	.size	__func__.36214, 17
__func__.36214:
	.ascii	"vsys_pool_delete\000"
	.type	vsys_ioctls, %object
	.size	vsys_ioctls, 200
vsys_ioctls:
	.word	-1068731648
	.word	0
	.word	vsys_version
	.word	.LC68
	.word	0
	.word	-1071877375
	.word	0
	.word	vsys_bind
	.word	.LC69
	.word	0
	.word	-1071877374
	.word	0
	.word	vsys_unbind
	.word	.LC70
	.word	0
	.word	29443
	.word	0
	.word	vsys_init
	.word	.LC71
	.word	0
	.word	29444
	.word	0
	.word	vsys_deinit
	.word	.LC72
	.word	0
	.word	-1038585083
	.word	0
	.word	vsys_pool_create
	.word	.LC73
	.word	0
	.word	-1038585082
	.word	0
	.word	vsys_pool_delete
	.word	.LC74
	.word	0
	.word	29447
	.word	0
	.word	vsys_wait_rcu
	.word	.LC75
	.word	0
	.word	29448
	.word	0
	.word	vsys_free_all_cma
	.word	.LC76
	.word	0
	.word	-1072401655
	.word	0
	.word	vsys_force_lost_frame
	.word	.LC77
	.word	0
	.type	vsys_driver_fops, %object
	.size	vsys_driver_fops, 128
vsys_driver_fops:
	.word	0
	.space	4
	.word	vsys_read
	.word	vsys_write
	.space	20
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
	.size	g_vsys_dev, 8412
g_vsys_dev:
	.space	8412
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"mod = %d dev_id = %d, chn_id = %d\000"
.LC1:
	.ascii	"\033[1;31mstruct vsys_version mismatching\033[0m\000"
.LC2:
	.ascii	"\033[1;31mstruct vsys_bind mismatching\033[0m\000"
.LC3:
	.ascii	"\033[1;31mget node from map [src chn] fail\033[0m\000"
.LC4:
	.ascii	"\033[1;32mVPSS GRP %d bind on AIISP node\033[0m\000"
.LC5:
	.ascii	"\033[1;32mSRC mod,dev,chn(%d-%d-%d) bind DEST mod,d"
	.ascii	"ev,chn(%d-%d-%d) success!\033[0m\000"
.LC6:
	.ascii	"\033[1;31mget node from map [dst chn] fail\033[0m\000"
.LC7:
	.ascii	"\033[1;32mVPSS GRP %d unbind on AIISP node\033[0m\000"
.LC8:
	.ascii	"\033[1;32mSRC mod,dev,chn(%d-%d-%d) unbind DEST mod"
	.ascii	",dev,chn(%d-%d-%d) success!\033[0m\000"
.LC9:
	.ascii	"in\000"
.LC10:
	.ascii	"out\000"
.LC11:
	.ascii	"/tmp\000"
.LC12:
	.ascii	"help\000"
.LC13:
	.ascii	"show this help message and exit\000"
.LC14:
	.ascii	"basic options:\000"
.LC15:
	.ascii	"module_id\000"
.LC16:
	.ascii	"MODULE id. default(0)\000"
.LC17:
	.ascii	"device_id\000"
.LC18:
	.ascii	"device id. default(0)\000"
.LC19:
	.ascii	"channel_id\000"
.LC20:
	.ascii	"channel id default(0)\000"
.LC21:
	.ascii	"frame_cnt\000"
.LC22:
	.ascii	"save frame counnter(default 1)\000"
.LC23:
	.ascii	"input\000"
.LC24:
	.ascii	"save input frame(default 0)\000"
.LC25:
	.ascii	"one_file\000"
.LC26:
	.ascii	"all frame save a file(default 0)\000"
.LC27:
	.ascii	"output_path\000"
.LC28:
	.ascii	"save frame path, (default /tmp)\000"
.LC29:
	.ascii	"\012use --help for details.\000"
.LC30:
	.ascii	"\012select a test case to run.\000"
.LC31:
	.ascii	"modId = %d, devId = %d, chId = %d, is_in %d, cnt = "
	.ascii	"%d, path = %s\000"
.LC32:
	.ascii	"\033[1;31mnode invalid %d, %d, %d\033[0m\000"
.LC33:
	.ascii	"\033[1;31mmalloc fail\033[0m\000"
.LC34:
	.ascii	"%s/%s_%d_%d_%s.bin\000"
.LC35:
	.ascii	"\033[1;31m%s open fail\033[0m\000"
.LC36:
	.ascii	"vmpi:28ee389dbc33c2c7db812cd08e54fd3a0f2f992b-v1.4."
	.ascii	"81\000"
.LC37:
	.ascii	"module: %s\012version: %s\012build: %s\012\000"
.LC38:
	.ascii	"-2024-05-24-16:21:01\000"
.LC39:
	.ascii	"***********************************************mpi "
	.ascii	"node list******************************************"
	.ascii	"*********\012\000"
.LC40:
	.ascii	"id---name--------handle-----nid-uid-ref-infa_cnt-lc"
	.ascii	"nt-frate-fbase-wcnt-depth-timeout-onfa_cnt-lcnt-fra"
	.ascii	"te-fbase-wcnt-depth-timeout-itime-otime-state-next_"
	.ascii	"node-------------\012\000"
.LC41:
	.ascii	"%-#4d %-#11s %-#10x %-#3d %-#3d %-#3d %-#8d %-#4d %"
	.ascii	"-#5d %-#5d %-#4d %-#5d %-#7d %-#8d %-#4d %-#5d %-#5"
	.ascii	"d %-#4d %-#5d %-#7d %-#5ld %-#5ld %-#5x \000"
.LC42:
	.ascii	"*********************************************mpi no"
	.ascii	"de frame control***********************************"
	.ascii	"*********\012\000"
.LC43:
	.ascii	"id---name--------table-----------------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"---------\012\000"
.LC44:
	.ascii	"%-#4d  \000"
.LC45:
	.ascii	"\012\000"
.LC46:
	.ascii	"%-#4d %-#11s %-#5s\000"
.LC47:
	.ascii	"%d \000"
.LC48:
	.ascii	"\033[1;31mstruct vsys_chn_attr mismatching\033[0m\000"
.LC49:
	.ascii	"\033[1;31mget node from map fail\033[0m\000"
.LC50:
	.ascii	"\033[1;31mstruct vsys_pool mismatching\033[0m\000"
.LC51:
	.ascii	"valloc\000"
.LC52:
	.ascii	"\033[1;31merror getting vdev  %p\033[0m\000"
.LC53:
	.ascii	"\033[1;31mcreate pool fail  %p\033[0m\000"
.LC54:
	.ascii	"\033[1;32mmpi_buf id =  %d\033[0m\000"
.LC55:
	.ascii	"%d : %s\000"
.LC56:
	.ascii	"vsys_ioctl: nr = %d, index = %d dev = %p\000"
.LC57:
	.ascii	"\033[1;31merror: index = %d\033[0m\000"
.LC58:
	.ascii	"\033[1;31m error: ioctl is null\033[0m\000"
.LC59:
	.ascii	"mcu\000"
.LC60:
	.ascii	"venc\000"
.LC61:
	.ascii	"\033[1;31mvalloc open fail\033[0m\000"
.LC62:
	.ascii	"vsys dev open %d\000"
.LC63:
	.ascii	"\033[1;32mpool id = %d, user process exit\033[0m\000"
.LC64:
	.ascii	"%s, %d\000"
.LC65:
	.ascii	"\033[1;32mpool->id =  %d\033[0m\000"
.LC66:
	.ascii	"vsys\000"
.LC67:
	.ascii	"echo cap -m 8 -d 0 -c 0 -C 1 -o /tmp > /dev/mpi/vsy"
	.ascii	"s\000"
.LC68:
	.ascii	"VSYS_IOCTL_VERSION\000"
.LC69:
	.ascii	"VSYS_IOCTL_SYS_BIND\000"
.LC70:
	.ascii	"VSYS_IOCTL_SYS_UNBIND\000"
.LC71:
	.ascii	"VSYS_IOCTL_SYS_INIT\000"
.LC72:
	.ascii	"VSYS_IOCTL_SYS_DEINIT\000"
.LC73:
	.ascii	"VSYS_IOCTL_SYS_POOL_CREATE\000"
.LC74:
	.ascii	"VSYS_IOCTL_SYS_POOL_DELETE\000"
.LC75:
	.ascii	"VSYS_IOCTL_WAIT_MMZ_FREE\000"
.LC76:
	.ascii	"VSYS_IOCTL_RELEASE_PHY_MEMORY\000"
.LC77:
	.ascii	"VSYS_IOCTL_FORCE_LOST_FRAME\000"
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
