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
	.file	"valloc.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_get_dma_buf_phy_addr:					
	.asciz 	"get_dma_buf_phy_addr"					
__kstrtabns_get_dma_buf_phy_addr:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_get_mpi_buf:					
	.asciz 	"get_mpi_buf"					
__kstrtabns_get_mpi_buf:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_compat_ioctl, %function
valloc_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	valloc_compat_ioctl, .-valloc_compat_ioctl
	.align	1
	.global	get_dma_buf_phy_addr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_dma_buf_phy_addr, %function
get_dma_buf_phy_addr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L8
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r8, r0
	ldr	r1, [r3, #160]
	bl	dma_buf_attach
	cmn	r0, #4096
	mov	r6, r0
	bls	.L3
	movs	r3, #69
	movs	r0, #1
	ldr	r2, .L8+4
	movs	r1, #2
	strd	r3, r0, [sp]
	ldr	r3, .L8+8
	bl	vlog
	mov	r0, r6
.L2:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L3:
	movs	r1, #0
	bl	dma_buf_map_attachment
	cmn	r0, #4096
	mov	r5, r0
	bls	.L5
	movs	r3, #74
	movs	r0, #1
	ldr	r2, .L8+12
	movs	r1, #2
	strd	r3, r0, [sp]
	ldr	r3, .L8+8
	bl	vlog
	mov	r0, r5
	b	.L2
.L5:
	ldr	r0, [r0]
	movs	r7, #0
	ldr	r9, .L8+16
	ldr	r10, .L8+20
.L6:
	ldr	r3, [r5, #8]
	cmp	r3, r7
	bhi	.L7
	movs	r2, #0
	mov	r1, r5
	mov	r0, r6
	bl	dma_buf_unmap_attachment
	mov	r0, r8
	mov	r1, r6
	bl	dma_buf_detach
	mov	r0, r4
	b	.L2
.L7:
	ldr	r4, [r0]
	adds	r7, r7, #1
	ldr	r3, [r9]
	bic	r4, r4, #3
	ldr	r2, [r0, #4]
	subs	r4, r4, r3
	ldr	r3, [r10]
	add	r4, r3, r4, asr #5
	add	r4, r2, r4, lsl #12
	bl	sg_next
	b	.L6
.L9:
	.align	2
.L8:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC1
	.word	mem_map
	.word	__pv_phys_pfn_offset
	.fnend
	.size	get_dma_buf_phy_addr, .-get_dma_buf_phy_addr
	.align	1
	.global	get_mpi_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_mpi_buf, %function
get_mpi_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	bl	dma_buf_get
	cbnz	r0, .L11
.L16:
	movs	r4, #0
.L10:
	mov	r0, r4
	pop	{r4, pc}
.L11:
	ldr	r3, .L24
	ldr	r4, [r3]
.L23:
	subs	r4, r4, #36
	add	r2, r4, #36
	cmp	r2, r3
	bne	.L15
	movs	r4, #0
.L14:
	bl	dma_buf_put
	cmp	r4, #0
	beq	.L16
	ldr	r0, [r4, #60]
	cmp	r0, #0
	beq	.L10
	ldr	r3, [r4, #16]
	cmp	r3, #0
	bne	.L10
	mov	r1, r4
	bl	mpi_buf_pool_request_buf
	movs	r3, #1
	str	r3, [r4, #204]
	b	.L10
.L15:
	ldr	r2, [r4, #64]
	cmp	r2, r0
	beq	.L14
	ldr	r4, [r4, #36]
	b	.L23
.L25:
	.align	2
.L24:
	.word	mpi_buf_list
	.fnend
	.size	get_mpi_buf, .-get_mpi_buf
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_read, %function
valloc_read:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #36
	sub	sp, sp, #36
	ldr	r4, [r0, #128]
	strd	r1, r2, [sp, #24]
	adds	r4, r4, #12
	mov	r0, r4
	bl	mpi_fs_buf_read
	adds	r3, r0, #1
	mov	r5, r0
	bne	.L27
	ldr	r3, .L58
	mov	r0, r4
	ldr	r2, .L58+4
	movs	r7, #0
	ldr	r1, .L58+8
	str	r3, [sp]
	ldr	r3, .L58+12
	ldr	r2, [r2]
	bl	mpi_fs_buf_add
	ldr	r1, .L58+16
	mov	r0, r4
	ldr	fp, .L58+48
	bl	mpi_fs_buf_add
	ldr	r1, .L58+20
	mov	r0, r4
	ldr	r10, .L58+52
	bl	mpi_fs_buf_add
	ldr	r3, .L58+24
	ldr	r5, [r3]
	mov	r9, r3
	subs	r5, r5, #36
.L28:
	add	r3, r5, #36
	cmp	r3, r9
	beq	.L40
	ldr	r3, [r5, #60]
	ldr	ip, [r5]
	cmp	r3, #0
	beq	.L49
	ldr	r3, [r3]
.L29:
	ldr	r1, [r5, #64]
	ldr	r0, [r5, #16]
	cbz	r1, .L30
	ldr	r1, [r1, #4]
	ldr	r1, [r1, #28]
.L30:
	ldr	r6, [r5, #216]
	ldr	r2, [r5, #4]
	cmp	r6, #0
	ite	ge
	ldrge	r6, [r5, #212]
	movlt	r6, #0
	strd	r5, r2, [sp, #12]
	mov	r2, ip
	str	r0, [sp]
	mov	r0, r4
	strd	r1, r6, [sp, #4]
	mov	r1, fp
	bl	mpi_fs_buf_add
	mov	r6, r0
	cmp	r0, #0
	beq	.L32
.L40:
	mov	r2, r7
	ldr	r1, .L58+28
	mov	r0, r4
	ldr	r7, .L58+32
	bl	mpi_fs_buf_add
	ldr	r1, .L58+36
	mov	r0, r4
	ldr	r9, .L58+56
	bl	mpi_fs_buf_add
	ldr	r1, .L58+40
	mov	r0, r4
	ldr	r10, .L58+60
	bl	mpi_fs_buf_add
	ldr	r5, [r7]
	ldr	fp, .L58+44
	subs	r5, r5, #36
.L33:
	add	r3, r5, #36
	cmp	r3, r7
	beq	.L47
	ldr	r1, [r5, #4]
	mov	r0, r4
	ldr	r3, [r5, #16]
	ldr	r2, [r5, #68]
	str	r1, [sp, #12]
	ldr	r1, [r5, #56]
	str	r1, [sp, #8]
	ldr	r1, [r5, #72]
	strd	r2, r1, [sp]
	mov	r1, r9
	ldr	r2, [r5]
	bl	mpi_fs_buf_add
	cbz	r0, .L41
.L47:
	ldr	r1, .L58+44
	mov	r0, r4
	bl	mpi_fs_buf_add
	mov	r0, r4
	ldrd	r1, r2, [sp, #24]
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	mpi_fs_buf_read
.L49:
	mov	r3, #-1
	b	.L29
.L32:
	add	r8, r5, #68
.L35:
	ldr	r3, [r8], #4
	cmp	r3, #1
	bhi	.L34
.L37:
	adds	r6, r6, #1
	cmp	r6, #32
	bne	.L35
.L36:
	ldr	r1, .L58+44
	mov	r0, r4
	bl	mpi_fs_buf_add
	ldr	r3, [r5, #216]
	cmp	r3, #0
	ite	ge
	ldrge	r3, [r5, #212]
	movlt	r3, #0
	ldr	r5, [r5, #36]
	add	r7, r7, r3
	subs	r5, r5, #36
	b	.L28
.L34:
	ldr	r2, [r3, #44]
	mov	r1, r10
	str	r6, [sp]
	mov	r0, r4
	ldr	r3, [r3]
	ldr	r2, [r2, #44]
	bl	mpi_fs_buf_add
	cmp	r0, #0
	beq	.L37
	b	.L36
.L41:
	mov	r8, r5
	ldr	r6, [r8, #84]!
.L57:
	subs	r6, r6, #64
	add	r3, r6, #64
	cmp	r3, r8
	beq	.L45
	ldr	r2, [r6, #44]
	mov	r1, r10
	ldr	r3, [r6]
	mov	r0, r4
	ldr	r2, [r2, #44]
	bl	mpi_fs_buf_add
	cbz	r0, .L43
.L45:
	mov	r1, fp
	mov	r0, r4
	bl	mpi_fs_buf_add
	ldr	r5, [r5, #36]
	subs	r5, r5, #36
	b	.L33
.L43:
	ldr	r6, [r6, #64]
	b	.L57
.L27:
	cbnz	r0, .L26
	mov	r0, r4
	bl	mpi_fs_buf_reset
.L26:
	mov	r0, r5
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L59:
	.align	2
.L58:
	.word	.LC4
	.word	.LANCHOR0
	.word	.LC3
	.word	.LC2
	.word	.LC5
	.word	.LC6
	.word	mpi_buf_list
	.word	.LC8
	.word	mpi_buf_pool_list
	.word	.LC9
	.word	.LC10
	.word	.LC12
	.word	.LC7
	.word	.LC11
	.word	.LC13
	.word	.LC14
	.fnend
	.size	valloc_read, .-valloc_read
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_mb_create, %function
valloc_mb_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r6, r1
	ldr	r3, [r1, #12]
	.pad #20
	sub	sp, sp, #20
	ldr	r7, [r0, #128]
	cmp	r3, #28
	beq	.L61
	movw	r3, #305
	ldr	r2, .L66
	str	r3, [sp]
	ldr	r3, .L66+4
.L65:
	movs	r1, #2
	movs	r0, #1
	bl	vlog
	mov	r0, #-1
.L60:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L61:
	ldrd	r2, r1, [r1, #16]
	ldr	r0, .L66+8
	bl	mpi_allocator_alloc
	mov	r5, r0
	cbnz	r0, .L63
	mov	r3, #310
	ldr	r2, .L66+12
	str	r3, [sp]
	ldr	r3, .L66+4
	b	.L65
.L63:
	movs	r1, #0
	bl	mpi_buf_create
	mov	r4, r0
	cbnz	r0, .L64
	movw	r3, #315
	ldr	r2, .L66+16
	str	r3, [sp]
	ldr	r3, .L66+4
	b	.L65
.L64:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r3, [r3, #12]
	add	r3, r3, #736
	str	r3, [r0, #4]
	mov	r0, r5
	mov	r5, r4
	bl	dma_buf_put
	ldr	r3, [r5], #44
	ldr	r2, .L66+20
	movs	r1, #5
	movs	r0, #1
	str	r3, [r6, #4]
	str	r3, [sp, #4]
	movw	r3, #321
	str	r3, [sp]
	ldr	r3, .L66+4
	bl	vlog
	ldr	r3, [r7, #4]
	str	r5, [r7, #4]
	movs	r1, #5
	ldr	r2, .L66+24
	movs	r0, #1
	strd	r7, r3, [r4, #44]
	str	r5, [r3]
	ldr	r3, [r4, #40]
	str	r3, [sp, #8]
	ldr	r3, [r4, #36]
	str	r3, [sp, #4]
	movw	r3, #323
	str	r3, [sp]
	ldr	r3, .L66+4
	bl	vlog
	movs	r0, #0
	b	.L60
.L67:
	.align	2
.L66:
	.word	.LC15
	.word	.LANCHOR1+21
	.word	.LANCHOR0+4
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.fnend
	.size	valloc_mb_create, .-valloc_mb_create
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	dma_buf_alloc, %function
dma_buf_alloc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L76
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r7, r2
	ldr	r5, [r3, #60]
	.pad #20
	sub	sp, sp, #20
	cbz	r5, .L68
	str	r2, [sp, #4]
	movs	r3, #175
	ldr	r2, .L76+4
	movs	r1, #5
	str	r3, [sp]
	movs	r0, #1
	ldr	r3, .L76+8
	movs	r6, #4
	bl	vlog
	ldr	r8, .L76+20
	ldr	r9, .L76+24
	movs	r4, #0
.L70:
	subs	r6, r6, #1
	bne	.L72
	cmn	r4, #4096
	bls	.L71
	movs	r3, #186
	str	r7, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L76+8
	movs	r0, #1
	ldr	r2, .L76+12
	mov	r5, r6
	bl	vlog
.L68:
	mov	r0, r5
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L72:
	str	r8, [sp]
	movs	r3, #0
	mov	r2, r9
	mov	r1, r7
	mov	r0, r5
	bl	rk_dma_heap_buffer_alloc
	cmn	r0, #4096
	mov	r4, r0
	bls	.L71
	movs	r0, #10
	bl	msleep
	b	.L70
.L71:
	ldr	r3, [r4, #4]
	movs	r1, #5
	ldr	r2, .L76+16
	movs	r0, #1
	mov	r5, r4
	ldr	r3, [r3, #28]
	str	r3, [sp, #12]
	ldr	r3, [r4]
	strd	r4, r3, [sp, #4]
	movs	r3, #189
	str	r3, [sp]
	ldr	r3, .L76+8
	bl	vlog
	b	.L68
.L77:
	.align	2
.L76:
	.word	.LANCHOR0
	.word	.LC20
	.word	.LANCHOR1+38
	.word	.LC22
	.word	.LC23
	.word	.LC21
	.word	524290
	.fnend
	.size	dma_buf_alloc, .-dma_buf_alloc
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_mb_get_id, %function
valloc_mb_get_id:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r1, #12]
	cmp	r3, #28
	beq	.L79
	mov	r3, #282
	ldr	r2, .L82
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L82+4
	movs	r0, #1
	bl	vlog
.L81:
	mov	r0, #-1
	b	.L78
.L79:
	ldr	r0, [r1, #8]
	bl	get_mpi_buf
	cmp	r0, #0
	beq	.L81
	ldr	r3, [r0]
	movs	r0, #0
	str	r3, [r4, #4]
.L78:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L83:
	.align	2
.L82:
	.word	.LC15
	.word	.LANCHOR1+52
	.fnend
	.size	valloc_mb_get_id, .-valloc_mb_get_id
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_ioctl, %function
valloc_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #20
	sub	sp, sp, #20
	uxtb	r4, r1
	movw	r3, #373
	mov	r6, r0
	str	r3, [sp]
	mov	r5, r1
	ldr	r3, .L88
	movs	r1, #5
	mov	r7, r2
	strd	r4, r4, [sp, #4]
	movs	r0, #1
	ldr	r2, .L88+4
	bl	vlog
	ldr	r3, .L88+8
	ldr	r1, [r3, #232]
	cmp	r1, r4
	bhi	.L85
	mov	r3, #376
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L88
	movs	r0, #1
	ldr	r2, .L88+12
	bl	vlog
.L86:
.L84:
	mvn	r0, #21
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L85:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
	cmp	r4, r1
	sbc	r1, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r4, r4, r1
	ldr	r1, [r3, #228]
	movs	r3, #20
	mla	r1, r3, r4, r1
	cbnz	r1, .L87
	movw	r3, #383
	ldr	r2, .L88+16
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L88
	movs	r0, #1
	bl	vlog
	b	.L84
.L87:
	ldr	r3, .L88+20
	mov	r2, r5
	mov	r0, r6
	str	r3, [r1, #16]
	mov	r3, r7
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	b	mpi_ioctl_kernel
.L89:
	.align	2
.L88:
	.word	.LANCHOR1+69
	.word	.LC24
	.word	.LANCHOR0
	.word	.LC25
	.word	.LC26
	.word	.LANCHOR2
	.fnend
	.size	valloc_ioctl, .-valloc_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_mb_get_phy_addr, %function
valloc_mb_get_phy_addr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	mov	r4, r1
	ldr	r3, [r1, #12]
	.pad #20
	sub	sp, sp, #20
	cmp	r3, #28
	beq	.L91
	mov	r3, #256
	ldr	r2, .L96
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L96+4
	movs	r0, #1
	bl	vlog
.L95:
	mov	r0, #-1
.L90:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L91:
	movs	r1, #4
	ldr	r0, [r4, #4]
	bl	mpi_obj_from_id
	mov	r5, r0
	cbnz	r0, .L93
	ldr	r3, [r4, #4]
	movs	r1, #2
	ldr	r2, .L96+8
	movs	r0, #1
	str	r3, [sp, #4]
	movw	r3, #261
	str	r3, [sp]
	ldr	r3, .L96+4
	bl	vlog
	b	.L95
.L93:
	ldr	r3, [r0, #224]
	cbnz	r3, .L94
	ldr	r0, [r0, #64]
	bl	get_dma_buf_phy_addr
	str	r0, [r5, #224]
.L94:
	ldr	r3, [r5, #224]
	movs	r1, #5
	ldr	r2, [r4, #4]
	movs	r0, #1
	str	r3, [r4, #24]
	strd	r3, r2, [sp, #4]
	movw	r3, #267
	ldr	r2, .L96+12
	str	r3, [sp]
	ldr	r3, .L96+4
	bl	vlog
	movs	r0, #0
	b	.L90
.L97:
	.align	2
.L96:
	.word	.LC15
	.word	.LANCHOR1+82
	.word	.LC27
	.word	.LC28
	.fnend
	.size	valloc_mb_get_phy_addr, .-valloc_mb_get_phy_addr
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_mb_delete, %function
valloc_mb_delete:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	mov	r3, r1
	ldr	r2, [r1, #12]
	cmp	r2, #28
	beq	.L99
	mov	r3, #338
	ldr	r2, .L111
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L111+4
	movs	r0, #1
	bl	vlog
	mov	r0, #-1
.L98:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L99:
	movs	r1, #4
	ldr	r0, [r3, #4]
	bl	mpi_obj_from_id
	mov	r4, r0
	cbnz	r0, .L101
.L110:
	movs	r0, #0
	b	.L98
.L101:
	ldr	r3, [r0, #16]
	movs	r1, #5
	ldr	r2, .L111+8
	strd	r0, r3, [sp, #4]
	movw	r3, #343
	movs	r0, #1
	str	r3, [sp]
	ldr	r3, .L111+4
	bl	vlog
	ldrd	r2, r3, [r4, #44]
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r3, #290
	mov	r2, #256
	strd	r2, r3, [r4, #44]
	ldr	r3, [r4, #60]
	cbz	r3, .L102
	ldr	r3, [r4, #204]
	cmp	r3, #0
	beq	.L110
.L102:
	mov	r0, r4
	bl	mpi_obj_unref
	b	.L110
.L112:
	.align	2
.L111:
	.word	.LC15
	.word	.LANCHOR1+105
	.word	.LC29
	.fnend
	.size	valloc_mb_delete, .-valloc_mb_delete
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_version, %function
valloc_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r1, #32]
	cmp	r3, #76
	beq	.L114
	movs	r3, #205
	ldr	r2, .L118
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L118+4
	movs	r0, #1
	bl	vlog
	mov	r0, #-1
.L113:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L114:
	movs	r2, #1
	movs	r3, #0
	strd	r2, r3, [r1]
	str	r3, [r1, #8]
	ldr	r3, .L118+8
	ldr	r5, [r3]
	mov	r0, r5
	bl	strlen
	mov	r1, r5
	cmp	r0, #16
	it	ls
	strls	r0, [r4, #12]
	add	r0, r4, #16
	itt	hi
	movhi	r3, #16
	strhi	r3, [r4, #12]
	ldr	r2, [r4, #12]
	bl	strncpy
	movs	r0, #0
	str	r0, [r4, #36]
	str	r0, [r4, #56]
	b	.L113
.L119:
	.align	2
.L118:
	.word	.LC30
	.word	.LANCHOR1+122
	.word	.LANCHOR0
	.fnend
	.size	valloc_version, .-valloc_version
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_open, %function
valloc_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L125
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r8, r1
	ldr	r0, [r3, #20]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbnz	r0, .L121
	movw	r3, #407
	ldr	r2, .L125+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L125+8
	movs	r0, #1
	bl	vlog
.L124:
	mov	r5, #-1
.L120:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L121:
	str	r0, [r4]
	mov	r1, #10240
	str	r0, [r4, #4]
	adds	r0, r0, #12
	bl	mpi_fs_buf_init
	mov	r5, r0
	cbz	r0, .L123
	mov	r0, r4
	bl	kfree
	b	.L124
.L123:
	ldr	r6, .L125+12
	add	r7, r6, #64
	mov	r0, r7
	bl	mpi_obj_ref
	ldr	r3, [r6, #64]
	str	r7, [r4, #8]
	movs	r1, #5
	str	r4, [r8, #128]
	movs	r0, #1
	ldr	r2, .L125+16
	str	r3, [sp, #4]
	movw	r3, #419
	str	r3, [sp]
	ldr	r3, .L125+8
	bl	vlog
	b	.L120
.L126:
	.align	2
.L125:
	.word	kmalloc_caches
	.word	.LC31
	.word	.LANCHOR1+137
	.word	.LANCHOR0
	.word	.LC32
	.fnend
	.size	valloc_open, .-valloc_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_mb_get_fd, %function
valloc_mb_get_fd:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	mov	r4, r1
	ldr	r3, [r1, #12]
	.pad #20
	sub	sp, sp, #20
	cmp	r3, #28
	beq	.L128
	movs	r3, #231
	ldr	r2, .L132
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L132+4
	movs	r0, #1
	bl	vlog
.L131:
	mov	r0, #-1
.L127:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L128:
	movs	r1, #4
	ldr	r0, [r4, #4]
	bl	mpi_obj_from_id
	mov	r5, r0
	cbnz	r0, .L130
	ldr	r3, [r4, #4]
	movs	r1, #2
	ldr	r2, .L132+8
	movs	r0, #1
	str	r3, [sp, #4]
	movs	r3, #236
	str	r3, [sp]
	ldr	r3, .L132+4
	bl	vlog
	b	.L131
.L130:
	movs	r1, #0
	ldr	r0, [r0, #64]
	bl	dma_buf_fd
	str	r0, [r4, #8]
	ldr	r3, [r5, #64]
	ldr	r3, [r3, #4]
	add	r2, r3, #28
	pld	[r2]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r1, [r2]
	add	r1, r1, #1
	strex	r0, r1, [r2]
	teq	r0, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #4]
	movs	r1, #5
	ldr	r2, .L132+12
	movs	r0, #1
	str	r3, [sp, #8]
	ldr	r3, [r4, #8]
	str	r3, [sp, #4]
	movs	r3, #241
	str	r3, [sp]
	ldr	r3, .L132+4
	bl	vlog
	movs	r0, #0
	b	.L127
.L133:
	.align	2
.L132:
	.word	.LC15
	.word	.LANCHOR1+149
	.word	.LC27
	.word	.LC33
	.fnend
	.size	valloc_mb_get_fd, .-valloc_mb_get_fd
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_release, %function
valloc_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r7, #430
	ldr	r5, [r1, #128]
	ldr	r8, .L137+4
	ldr	r9, .L137+8
	ldr	r0, [r5, #8]
	bl	mpi_obj_unref
	ldr	r3, [r5, #8]
	ldr	r2, .L137
	movs	r1, #5
	movs	r0, #1
	ldr	r3, [r3]
	str	r3, [sp, #4]
	mov	r3, #428
	str	r3, [sp]
	ldr	r3, .L137+4
	bl	vlog
	ldr	r4, [r5]
	ldr	r6, [r4], #-44
	subs	r6, r6, #44
.L135:
	add	r3, r4, #44
	cmp	r5, r3
	bne	.L136
	add	r0, r4, #56
	bl	mpi_fs_buf_deinit
	mov	r0, r5
	bl	kfree
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L136:
	ldr	r3, [r4]
	mov	r2, r9
	movs	r1, #4
	movs	r0, #1
	strd	r7, r3, [sp]
	mov	r3, r8
	bl	vlog
	ldrd	r2, r3, [r4, #44]
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r3, #290
	mov	r2, #256
	strd	r2, r3, [r4, #44]
	ldr	r3, [r6, #44]
	mov	r4, r6
	sub	r6, r3, #44
	b	.L135
.L138:
	.align	2
.L137:
	.word	.LC32
	.word	.LANCHOR1+166
	.word	.LC34
	.fnend
	.size	valloc_release, .-valloc_release
	.align	1
	.global	valloc_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_probe, %function
valloc_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r5, r0
	ldr	r0, .L142
	bl	rk_dma_heap_find
	ldr	r4, .L142+4
	str	r0, [r4, #60]
	cbnz	r0, .L140
	movw	r3, #530
	ldr	r2, .L142+8
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L142+12
	movs	r0, #1
	bl	vlog
	mov	r0, #-1
.L139:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L140:
	ldr	r3, .L142+16
	mov	r1, #3264
	mov	r0, r5
	str	r3, [r4, #56]
	bl	kstrdup
	mov	r2, r5
	movs	r1, #8
	mov	r3, r0
	mov	r0, r4
	str	r3, [r0], #4
	bl	mpi_allocator_register
	ldr	r3, .L142+20
	mov	r2, r5
	movs	r1, #8
	add	r0, r4, #64
	str	r4, [r4, #224]
	str	r3, [r4, #228]
	movs	r3, #6
	str	r3, [r4, #232]
	bl	mpi_dev_register
	ldr	r1, .L142+24
	add	r0, r4, #64
	bl	mpi_sysfs_dev_add
	ldr	r0, [r4, #160]
	bl	rk_dma_heap_set_dev
	movs	r0, #0
	b	.L139
.L143:
	.align	2
.L142:
	.word	.LC35
	.word	.LANCHOR0
	.word	.LC36
	.word	.LANCHOR1+181
	.word	dma_buf_alloc
	.word	.LANCHOR2+12
	.word	.LANCHOR1+196
	.fnend
	.size	valloc_probe, .-valloc_probe
	.section	.init.text,"ax",%progbits
	.align	1
	.global	valloc_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_init, %function
valloc_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L145
	b	valloc_probe
.L146:
	.align	2
.L145:
	.word	.LC37
	.fnend
	.size	valloc_init, .-valloc_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	valloc_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	valloc_exit, %function
valloc_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.fnend
	.size	valloc_exit, .-valloc_exit
	.global	valloc_mutex
	.global	g_valloc_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__func__.36549, %object
	.size	__func__.36549, 21
__func__.36549:
	.ascii	"get_dma_buf_phy_addr\000"
	.type	__func__.36634, %object
	.size	__func__.36634, 17
__func__.36634:
	.ascii	"valloc_mb_create\000"
	.type	__func__.36594, %object
	.size	__func__.36594, 14
__func__.36594:
	.ascii	"dma_buf_alloc\000"
	.type	__func__.36625, %object
	.size	__func__.36625, 17
__func__.36625:
	.ascii	"valloc_mb_get_id\000"
	.type	__func__.36659, %object
	.size	__func__.36659, 13
__func__.36659:
	.ascii	"valloc_ioctl\000"
	.type	__func__.36618, %object
	.size	__func__.36618, 23
__func__.36618:
	.ascii	"valloc_mb_get_phy_addr\000"
	.type	__func__.36641, %object
	.size	__func__.36641, 17
__func__.36641:
	.ascii	"valloc_mb_delete\000"
	.type	__func__.36604, %object
	.size	__func__.36604, 15
__func__.36604:
	.ascii	"valloc_version\000"
	.type	__func__.36681, %object
	.size	__func__.36681, 12
__func__.36681:
	.ascii	"valloc_open\000"
	.type	__func__.36611, %object
	.size	__func__.36611, 17
__func__.36611:
	.ascii	"valloc_mb_get_fd\000"
	.type	__func__.36690, %object
	.size	__func__.36690, 15
__func__.36690:
	.ascii	"valloc_release\000"
	.type	__func__.36783, %object
	.size	__func__.36783, 13
__func__.36783:
	.ascii	"valloc_probe\000"
	.space	2
	.type	valloc_driver_fops, %object
	.size	valloc_driver_fops, 128
valloc_driver_fops:
	.word	0
	.space	4
	.word	valloc_read
	.space	24
	.word	0
	.word	valloc_ioctl
	.word	valloc_compat_ioctl
	.word	0
	.space	4
	.word	valloc_open
	.space	4
	.word	valloc_release
	.space	60
	.data
	.align	2
	.set	.LANCHOR2,. + 0
	.type	valloc_mutex, %object
	.size	valloc_mutex, 12
valloc_mutex:
	.word	0
	.word	valloc_mutex+4
	.word	valloc_mutex+4
	.type	valloc_ioctls, %object
	.size	valloc_ioctls, 120
valloc_ioctls:
	.word	-1068736256
	.word	0
	.word	valloc_version
	.word	.LC38
	.word	0
	.word	-1071881983
	.word	0
	.word	valloc_mb_get_fd
	.word	.LC39
	.word	0
	.word	-1071881982
	.word	0
	.word	valloc_mb_create
	.word	.LC40
	.word	0
	.word	-1071881981
	.word	0
	.word	valloc_mb_delete
	.word	.LC41
	.word	0
	.word	-1071881980
	.word	0
	.word	valloc_mb_get_phy_addr
	.word	.LC42
	.word	0
	.word	-1071881979
	.word	0
	.word	valloc_mb_get_id
	.word	.LC43
	.word	0
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_valloc_dev, %object
	.size	g_valloc_dev, 236
g_valloc_dev:
	.space	236
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[31m\033[1m%s,%d:attach fail = %d\033[0m\000"
.LC1:
	.ascii	"\033[31m\033[1m%s,%d:map fail = %d\033[0m\000"
.LC2:
	.ascii	"vmpi:9093280df536e902dd5125cdc9953509ebc59a74-v1.4."
	.ascii	"32\000"
.LC3:
	.ascii	"module: %s\012version: %s\012build: %s\012\000"
.LC4:
	.ascii	"-2023-02-02-15:30:51\000"
.LC5:
	.ascii	"*******************************************mpi buf "
	.ascii	"list***************************************\012\000"
.LC6:
	.ascii	"buf_id-pool_id-refcnt-dmacnt-size------handle-----c"
	.ascii	"reate-----------user-------------------------\012\000"
.LC7:
	.ascii	"%-#6d %-#7d %-#6d %-#6d %-#9d %-#10x %-#16s \000"
.LC8:
	.ascii	"total:%-#16d\012\000"
.LC9:
	.ascii	"*************************************mpi pool list*"
	.ascii	"**********************************\012\000"
.LC10:
	.ascii	"poid-total-free-min-size-------------create--------"
	.ascii	"---user---------------------------\012\000"
.LC11:
	.ascii	"%s_%d(%d) \000"
.LC12:
	.ascii	"\012\000"
.LC13:
	.ascii	"%-#4d %-#5d %-#4d %-#3d %-#16d %-#16s \000"
.LC14:
	.ascii	"%s_%d\000"
.LC15:
	.ascii	"\033[31m\033[1m%s,%d:struct valloc_mb mismatching\033"
	.ascii	"[0m\000"
.LC16:
	.ascii	"\033[31m\033[1m%s,%d:dma buf create fail\033[0m\000"
.LC17:
	.ascii	"\033[31m\033[1m%s,%d:mpi buf create fail\033[0m\000"
.LC18:
	.ascii	"%s,%d:mpi buf buf_id(%d)\000"
.LC19:
	.ascii	"%s,%d:buf->list->next = %08x, %08x\000"
.LC20:
	.ascii	"%s,%d:alloc(%d)\000"
.LC21:
	.ascii	"vmpi\000"
.LC22:
	.ascii	"\033[31m\033[1m%s,%d:alloc(%d) fail\033[0m\000"
.LC23:
	.ascii	"%s,%d:cma buf = %p, size = %d, refcnt = %d\000"
.LC24:
	.ascii	"%s,%d:valloc_ioctl: nr = %d, index = %d\000"
.LC25:
	.ascii	"\033[31m\033[1m%s,%d:error: index = %d\033[0m\000"
.LC26:
	.ascii	"\033[31m\033[1m%s,%d:error: ioctl is null\033[0m\000"
.LC27:
	.ascii	"\033[31m\033[1m%s,%d:mpi buf get fail buf_id(%d)\033"
	.ascii	"[0m\000"
.LC28:
	.ascii	"%s,%d:mpi buf get phys (%08x) buf_id(%d)\000"
.LC29:
	.ascii	"%s,%d:unref %x buffer refcnt = %d\000"
.LC30:
	.ascii	"\033[31m\033[1m%s,%d:struct valloc_version mismatch"
	.ascii	"ing\033[0m\000"
.LC31:
	.ascii	"\033[31m\033[1m%s,%d:valloc open fail\033[0m\000"
.LC32:
	.ascii	"%s,%d:%d\000"
.LC33:
	.ascii	"%s,%d:mpi buf get fd (%d) buf_id(%d)\000"
.LC34:
	.ascii	"\033[32m\033[1m%s,%d:buf id = %d, user process exit"
	.ascii	"\033[0m\000"
.LC35:
	.ascii	"rk-dma-heap-cma\000"
.LC36:
	.ascii	"\033[31m\033[1m%s,%d:valloc get rk_dma_heap fail\033"
	.ascii	"[0m\000"
.LC37:
	.ascii	"valloc\000"
.LC38:
	.ascii	"VALLOC_IOCTL_VERSION\000"
.LC39:
	.ascii	"VALLOC_IOCTL_MB_GET_FD\000"
.LC40:
	.ascii	"VALLOC_IOCTL_MB_CREATE\000"
.LC41:
	.ascii	"VALLOC_IOCTL_MB_DELETE\000"
.LC42:
	.ascii	"VALLOC_IOCTL_MB_GET_PHY_ADDR\000"
.LC43:
	.ascii	"VALLOC_IOCTL_MB_GET_ID\000"
	.section	___ksymtab_gpl+get_dma_buf_phy_addr,"a"
	.align	2
	.type	__ksymtab_get_dma_buf_phy_addr, %object
	.size	__ksymtab_get_dma_buf_phy_addr, 12
__ksymtab_get_dma_buf_phy_addr:
	.word	get_dma_buf_phy_addr
	.word	__kstrtab_get_dma_buf_phy_addr
	.word	__kstrtabns_get_dma_buf_phy_addr
	.section	___ksymtab_gpl+get_mpi_buf,"a"
	.align	2
	.type	__ksymtab_get_mpi_buf, %object
	.size	__ksymtab_get_mpi_buf, 12
__ksymtab_get_mpi_buf:
	.word	get_mpi_buf
	.word	__kstrtab_get_mpi_buf
	.word	__kstrtabns_get_mpi_buf
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
