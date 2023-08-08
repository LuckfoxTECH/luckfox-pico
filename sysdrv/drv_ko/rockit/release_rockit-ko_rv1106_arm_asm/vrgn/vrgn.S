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
	.file	"vrgn.c"
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
	.type	vrgn_compat_ioctl, %function
vrgn_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vrgn_compat_ioctl, .-vrgn_compat_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	osd_sync_callback, %function
osd_sync_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, [r5, #84]!
	movs	r6, #0
.L3:
	cmp	r4, r5
	bne	.L5
	pop	{r4, r5, r6, pc}
.L5:
	ldr	r0, [r4, #8]
	ldr	r3, [r0, #52]
	cmp	r3, #1
	bne	.L4
	ldr	r3, [r0, #8]
	cbz	r3, .L4
	adds	r0, r0, #16
	bl	complete_all
	ldr	r3, [r4, #8]
	str	r6, [r3, #8]
.L4:
	ldr	r4, [r4]
	b	.L3
	.fnend
	.size	osd_sync_callback, .-osd_sync_callback
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_ioctl, %function
vrgn_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	uxtb	r4, r1
	ldr	r3, [r0, #128]
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	movs	r1, #5
	ldr	r2, .L13
	ldr	r3, [r3]
	movs	r0, #3
	ldr	r8, [r3, #160]
	strd	r4, r3, [sp, #8]
	movw	r3, #722
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r3, .L13+4
	bl	vlog
	ldr	r1, [r8, #192]
	cmp	r1, r4
	bhi	.L10
	movw	r3, #725
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L13+4
	movs	r0, #3
	ldr	r2, .L13+8
	bl	vlog
.L11:
.L9:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L10:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
	cmp	r4, r1
	sbc	r1, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r4, r4, r1
	ldr	r1, [r8, #188]
	movs	r3, #20
	mla	r1, r3, r4, r1
	cbnz	r1, .L12
	mov	r3, #732
	ldr	r2, .L13+12
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L13+4
	movs	r0, #3
	bl	vlog
	b	.L9
.L12:
	mov	r3, r7
	mov	r2, r6
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L14:
	.align	2
.L13:
	.word	.LC0
	.word	.LANCHOR0
	.word	.LC1
	.word	.LC2
	.fnend
	.size	vrgn_ioctl, .-vrgn_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_read, %function
vrgn_read:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #36
	sub	sp, sp, #36
	ldr	r4, [r0, #128]
	mov	fp, r1
	str	r2, [sp, #28]
	ldr	r3, [r4], #4
	mov	r0, r4
	ldr	r7, [r3, #160]
	bl	mpi_fs_buf_read
	adds	r3, r0, #1
	mov	r5, r0
	bne	.L16
	ldr	r3, .L47
	mov	r0, r4
	ldr	r2, .L47+4
	ldr	r1, .L47+8
	str	r3, [sp]
	ldr	r3, .L47+12
	ldr	r2, [r2]
	bl	mpi_fs_buf_add
	ldr	r1, .L47+16
	mov	r0, r4
	ldr	r6, .L47+20
	bl	mpi_fs_buf_add
	ldr	r1, .L47+24
	mov	r0, r4
	ldr	r8, .L47+52
	bl	mpi_fs_buf_add
	ldr	r5, [r7, #196]
	add	r9, r6, #52
.L17:
	add	r3, r7, #196
	cmp	r5, r3
	beq	.L26
	ldr	r3, [r5, #28]
	str	r3, [sp, #24]
	ldr	r3, [r5, #36]
	cbz	r3, .L18
	cmp	r3, #1
	beq	.L18
	cmp	r3, #2
	beq	.L18
	cmp	r3, #3
	beq	.L18
	cmp	r3, #4
	bne	.L40
.L18:
	add	r3, r6, r3, lsl #3
	ldr	r3, [r3, #16]
.L19:
	ldrd	r10, ip, [r5, #44]
	movs	r1, #0
	ldr	r0, [r5, #40]
.L22:
	ldr	r2, [r9, r1, lsl #3]
	cmp	r0, r2
	bne	.L20
	add	r1, r6, r1, lsl #3
	ldr	lr, [r1, #56]
.L21:
	ldr	r0, [r5, #1084]
	cmp	r0, #0
	beq	.L41
	ldr	r1, [r0]
	ldr	r0, [r0, #16]
.L23:
	strd	r1, r0, [sp, #12]
	mov	r0, r4
	strd	ip, lr, [sp, #4]
	str	r10, [sp]
	ldr	r2, [sp, #24]
	ldr	r1, .L47+28
	bl	mpi_fs_buf_add
	cbz	r0, .L24
.L26:
	ldr	r7, .L47+32
	mov	r0, r4
	ldr	r1, .L47+36
	bl	mpi_fs_buf_add
	ldr	r1, .L47+40
	mov	r0, r4
	ldr	r9, .L47+56
	bl	mpi_fs_buf_add
	ldr	r5, [r7]
	ldr	r10, .L47+60
	subs	r5, r5, #36
.L25:
	add	r3, r5, #36
	cmp	r3, r7
	beq	.L34
	ldr	r3, [r5, #44]
	mov	r1, r9
	ldr	r2, [r5]
	mov	r0, r4
	ldr	r3, [r3, #44]
	bl	mpi_fs_buf_add
	cbz	r0, .L28
.L34:
	ldr	r1, .L47+44
	mov	r0, r4
	bl	mpi_fs_buf_add
	mov	r1, fp
	mov	r0, r4
	ldr	r2, [sp, #28]
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	mpi_fs_buf_read
.L40:
	mov	r3, r8
	b	.L19
.L20:
	adds	r1, r1, #1
	cmp	r1, #13
	bne	.L22
	mov	lr, r8
	b	.L21
.L41:
	mov	r1, #-1
	mov	r0, r1
	b	.L23
.L24:
	ldr	r2, [r5, #32]
	mov	r0, r4
	ldr	r1, .L47+48
	bl	mpi_fs_buf_add
	cmp	r0, #0
	bne	.L26
	ldr	r1, .L47+44
	mov	r0, r4
	bl	mpi_fs_buf_add
	ldr	r5, [r5]
	b	.L17
.L28:
	mov	r8, r5
	ldr	r6, [r8, #84]!
.L29:
	cmp	r6, r8
	beq	.L32
	ldr	r2, [r6, #8]
	mov	r1, r10
	ldr	r3, [r6, #28]
	mov	r0, r4
	str	r3, [sp]
	ldr	r3, [r6, #24]
	ldr	r2, [r2, #28]
	bl	mpi_fs_buf_add
	cbz	r0, .L30
.L32:
	ldr	r1, .L47+44
	mov	r0, r4
	bl	mpi_fs_buf_add
	ldr	r5, [r5, #36]
	subs	r5, r5, #36
	b	.L25
.L30:
	ldr	r6, [r6]
	b	.L29
.L16:
	cbnz	r0, .L15
	mov	r0, r4
	bl	mpi_fs_buf_reset
.L15:
	mov	r0, r5
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L48:
	.align	2
.L47:
	.word	.LC6
	.word	.LANCHOR1
	.word	.LC5
	.word	.LC4
	.word	.LC7
	.word	.LANCHOR0
	.word	.LC8
	.word	.LC9
	.word	mpi_node_list
	.word	.LC10
	.word	.LC11
	.word	.LC13
	.word	.LC12
	.word	.LC3
	.word	.LC14
	.word	.LC15
	.fnend
	.size	vrgn_read, .-vrgn_read
	.align	1
	.global	vrgn_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_version, %function
vrgn_version:
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
	beq	.L50
	movw	r3, #521
	ldr	r2, .L54
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L54+4
	movs	r0, #3
	bl	vlog
	mov	r0, #-1
.L49:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L50:
	movs	r3, #0
	movs	r2, #1
	str	r3, [r1, #8]
	strd	r2, r3, [r1]
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
	b	.L49
.L55:
	.align	2
.L54:
	.word	.LC16
	.word	.LANCHOR0+156
	.fnend
	.size	vrgn_version, .-vrgn_version
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_open, %function
vrgn_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L61
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r7, r1
	ldr	r0, [r3, #20]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbnz	r0, .L57
	mov	r3, #756
	ldr	r2, .L61+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L61+8
	movs	r0, #3
	bl	vlog
.L60:
	mov	r5, #-1
.L56:
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L57:
	mov	r1, #5120
	adds	r0, r0, #4
	bl	mpi_fs_buf_init
	mov	r5, r0
	cbz	r0, .L59
	mov	r0, r4
	bl	kfree
	b	.L60
.L59:
	ldr	r6, .L61+12
	mov	r0, r6
	bl	mpi_obj_ref
	str	r6, [r4]
	str	r4, [r7, #128]
	b	.L56
.L62:
	.align	2
.L61:
	.word	kmalloc_caches
	.word	.LC17
	.word	.LANCHOR0+169
	.word	.LANCHOR1+4
	.fnend
	.size	vrgn_open, .-vrgn_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	out_filter_callback, %function
out_filter_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	.pad #24
	sub	sp, sp, #24
	mov	r7, r1
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L76
	mov	r1, #3520
	ldr	r0, [r3, #16]
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L63
	cbz	r7, .L65
	mov	r0, r7
	bl	mpi_obj_ref
.L65:
	str	r7, [r4, #12]
	cbz	r6, .L66
	mov	r0, r6
	bl	mpi_obj_ref
.L66:
	ldr	r5, .L76+4
	movs	r1, #5
	str	r6, [r4, #8]
	movs	r0, #3
	add	r2, r5, #204
	ldr	r3, [r5, #208]
	str	r4, [r5, #208]
	strd	r2, r3, [r4]
	str	r4, [r3]
	ldr	r3, [r7]
	ldr	r2, .L76+8
	str	r3, [sp, #16]
	ldr	r3, [r6]
	strd	r4, r3, [sp, #8]
	ldr	r3, [r5, #48]
	str	r3, [sp, #4]
	movw	r3, #341
	str	r3, [sp]
	ldr	r3, .L76+12
	bl	vlog
	add	r0, r5, #168
	bl	up
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L63:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L77:
	.align	2
.L76:
	.word	kmalloc_caches
	.word	.LANCHOR1
	.word	.LC18
	.word	.LANCHOR0+179
	.fnend
	.size	out_filter_callback, .-out_filter_callback
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_release, %function
vrgn_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	ldr	r6, [r1, #128]
	ldr	r4, [r6]
	mov	r0, r4
	ldr	r5, [r4, #160]
	bl	mpi_obj_unref
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bne	.L83
	ldr	r4, [r5, #196]!
	ldr	r8, .L88
	ldr	r9, .L88+4
	ldr	r7, [r4]
.L80:
	cmp	r4, r5
	bne	.L82
.L83:
	adds	r0, r6, #4
	bl	mpi_fs_buf_deinit
	mov	r0, r6
	bl	kfree
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L82:
	cbz	r4, .L81
	ldr	r3, [r4, #28]
	mov	r2, r9
	movs	r1, #4
	movs	r0, #3
	str	r3, [sp, #4]
	movw	r3, #782
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
	ldrd	r2, r3, [r4]
	mov	r0, r4
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r2, #256
	mov	r3, #290
	strd	r2, r3, [r4]
	bl	kfree
.L81:
	mov	r4, r7
	ldr	r7, [r7]
	b	.L80
.L89:
	.align	2
.L88:
	.word	.LANCHOR0+199
	.word	.LC19
	.fnend
	.size	vrgn_release, .-vrgn_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_thread_fn, %function
vrgn_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #100
	sub	sp, sp, #100
	ldr	r10, .L181+24
	mov	r9, r0
.L91:
	ldr	r3, [r9, #184]
	cbz	r3, .L123
	movs	r0, #0
	add	sp, sp, #100
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L123:
	add	r0, r9, #168
	bl	down
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, r9
	ldr	r5, [r3, #204]!
	cmp	r5, r3
	beq	.L92
	cbnz	r5, .L93
.L92:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L91
.L93:
	ldrd	r1, r3, [r5]
	str	r3, [r1, #4]
	str	r1, [r3]
	mov	r1, #256
	mov	r3, #290
	strd	r1, r3, [r5]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L181
	movs	r0, #3
	str	r3, [sp, #16]
	ldr	r3, [r6]
	strd	r5, r3, [sp, #8]
	ldr	r3, [r9, #48]
	str	r3, [sp, #4]
	movw	r3, #267
	str	r3, [sp]
	ldr	r3, .L181+4
	bl	vlog
	ldrd	r6, r4, [r5, #8]
	ldr	r3, [r6, #92]
	cmp	r3, #0
	beq	.L95
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	str	r3, [sp, #60]
	ldr	r8, [r6, #84]
.L96:
	add	r3, r6, #84
	cmp	r8, r3
	bne	.L110
	ldr	r3, [sp, #60]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cbz	r6, .L177
	ldr	r3, [r6, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L112
.L177:
	mov	r0, r6
	bl	mpi_obj_unref
	mov	r0, r4
.L179:
	bl	mpi_obj_unref
	mov	r0, r5
	bl	kfree
	b	.L91
.L110:
	ldr	r3, [r8, #8]
	ldr	r3, [r3, #1084]
	cmp	r3, #0
	beq	.L97
	mov	r1, #3520
	ldr	r0, [r10, #20]
	bl	kmem_cache_alloc
	mov	r1, #3520
	str	r0, [sp, #76]
	ldr	r0, [r10, #20]
	bl	kmem_cache_alloc
	mov	r1, #3520
	str	r0, [sp, #80]
	ldr	r0, [r10, #20]
	bl	kmem_cache_alloc
	ldr	r3, [sp, #76]
	str	r0, [sp, #84]
	cmp	r3, #0
	beq	.L104
	ldr	r2, [sp, #80]
	cmp	r2, #0
	beq	.L104
	cmp	r0, #0
	beq	.L104
	cmp	r4, #0
	beq	.L99
	ldr	r2, [r8, #8]
	ldr	r7, [r2, #1084]
	cmp	r7, #0
	beq	.L99
	ldr	r1, [r7, #64]
	mov	fp, #0
	ldr	r2, [r4, #64]
	strd	r1, r2, [sp, #68]
	str	r2, [sp, #64]
	ldr	r2, [r8, #8]
	ldr	r2, [r2, #1088]
	ldr	r2, [r2, #56]
	str	r2, [sp, #92]
	ldr	r2, [r8, #24]
	str	r2, [r3]
	ldr	r3, [sp, #76]
	ldr	r2, [r8, #28]
	str	r2, [r3, #4]
	ldr	r2, [r7, #236]
	ldr	r3, [sp, #76]
	str	r2, [r3, #8]
	ldr	r2, [r7, #240]
	ldr	r3, [sp, #76]
	str	r2, [r3, #12]
	ldr	r3, [sp, #76]
	ldr	r2, [r4, #244]
	str	r2, [r3, #20]
	ldr	r2, [r4, #248]
	str	r2, [r3, #24]
	ldr	r0, [r4, #256]
	bl	pixel_format_rk_to_rga
	ldr	r3, [sp, #76]
	str	r0, [r3, #16]
	ldr	r0, [r4, #264]
	bl	compress_mode_rk_to_rga
	movs	r1, #5
	ldr	r3, [sp, #76]
	str	r0, [r3, #28]
	str	r0, [sp, #44]
	movs	r0, #3
	ldr	r2, [r3, #16]
	str	r2, [sp, #40]
	ldr	r2, [r3, #24]
	str	r2, [sp, #36]
	ldr	r2, [r3, #20]
	str	r2, [sp, #32]
	ldr	r2, [r3, #12]
	str	r2, [sp, #28]
	ldr	r2, [r3, #8]
	str	r2, [sp, #24]
	ldr	r2, [r3, #4]
	str	r2, [sp, #20]
	ldr	r3, [r3]
	ldr	r2, .L181+8
	str	r3, [sp, #16]
	ldr	r3, [sp, #92]
	str	r3, [sp, #12]
	ldr	r3, [r4, #64]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movs	r3, #134
	str	r3, [sp]
	ldr	r3, .L181+12
	bl	vlog
	ldr	r3, [sp, #80]
	str	fp, [r3]
	ldr	r3, [sp, #80]
	str	fp, [r3, #4]
	ldr	r2, [r7, #236]
	ldr	r3, [sp, #80]
	str	r2, [r3, #8]
	ldr	r2, [r7, #240]
	ldr	r3, [sp, #80]
	str	r2, [r3, #12]
	ldr	r2, [r7, #244]
	ldr	r3, [sp, #80]
	str	r2, [r3, #20]
	ldr	r2, [r7, #248]
	str	r2, [r3, #24]
	ldr	r0, [r7, #256]
	bl	pixel_format_rk_to_rga
	movs	r2, #1
	movs	r1, #5
	ldr	r3, [sp, #80]
	str	r0, [r3, #16]
	movs	r0, #3
	ldr	r3, [sp, #80]
	str	r2, [r3, #28]
	str	r2, [sp, #44]
	ldr	r2, [r3, #16]
	str	r2, [sp, #40]
	ldr	r2, [r3, #24]
	str	r2, [sp, #36]
	ldr	r2, [r3, #20]
	str	r2, [sp, #32]
	ldr	r2, [r3, #12]
	str	r2, [sp, #28]
	ldr	r2, [r3, #8]
	str	r2, [sp, #24]
	ldr	r2, [r3, #4]
	str	r2, [sp, #20]
	ldr	r3, [r3]
	ldr	r2, .L181+16
	str	r3, [sp, #16]
	ldr	r3, [sp, #92]
	str	r3, [sp, #12]
	ldr	r3, [r7, #64]
	str	r3, [sp, #8]
	ldr	r3, [r7]
	str	r3, [sp, #4]
	movs	r3, #151
	str	r3, [sp]
	ldr	r3, .L181+12
	bl	vlog
	ldr	r2, [r8, #24]
	ldr	r3, [sp, #84]
	str	r2, [r3]
	ldr	r3, [sp, #84]
	ldr	r2, [r8, #28]
	str	r2, [r3, #4]
	ldr	r2, [r7, #236]
	ldr	r3, [sp, #84]
	str	r2, [r3, #8]
	ldr	r2, [r7, #240]
	ldr	r3, [sp, #84]
	str	r2, [r3, #12]
	ldr	r3, [sp, #84]
	ldr	r2, [r4, #244]
	str	r2, [r3, #20]
	ldr	r3, [sp, #84]
	ldr	r2, [r4, #248]
	str	r2, [r3, #24]
	ldr	r0, [r4, #256]
	bl	pixel_format_rk_to_rga
	ldr	r3, [sp, #84]
	str	r0, [r3, #16]
	ldr	r0, [r4, #264]
	bl	compress_mode_rk_to_rga
	movs	r1, #5
	ldr	r3, [sp, #84]
	str	r0, [r3, #28]
	str	r0, [sp, #52]
	movs	r0, #3
	ldr	r2, [r3, #16]
	str	r2, [sp, #48]
	ldr	r2, [r3, #24]
	str	r2, [sp, #44]
	ldr	r2, [r3, #20]
	str	r2, [sp, #40]
	ldr	r2, [r3, #12]
	str	r2, [sp, #36]
	ldr	r2, [r3, #8]
	str	r2, [sp, #32]
	ldr	r2, [r3, #4]
	str	r2, [sp, #28]
	ldr	r3, [r3]
	ldr	r2, .L181+12
	str	r3, [sp, #24]
	ldr	r3, [sp, #92]
	str	r3, [sp, #20]
	ldr	r3, [r4, #64]
	str	r3, [sp, #16]
	ldr	r3, [r4]
	str	r3, [sp, #12]
	movs	r3, #169
	strd	r3, r2, [sp]
	str	r3, [sp, #8]
	mov	r3, r2
	ldr	r2, .L181+20
	bl	vlog
	add	r0, sp, #64
	bl	rga_mpi_commit
	mov	r7, r0
	cmp	r0, #0
	beq	.L100
	ldr	r0, [sp, #76]
	cbz	r0, .L108
	bl	kfree
	str	fp, [sp, #76]
.L108:
	ldr	r0, [sp, #80]
	cbz	r0, .L109
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #80]
	b	.L109
.L182:
	.align	2
.L181:
	.word	.LC18
	.word	.LANCHOR0+212
	.word	.LC20
	.word	.LANCHOR0+227
	.word	.LC21
	.word	.LC22
	.word	kmalloc_caches
.L99:
	mov	r0, r3
	movs	r7, #0
	bl	kfree
	ldr	r0, [sp, #80]
	str	r7, [sp, #76]
	cbz	r0, .L109
	bl	kfree
	str	r7, [sp, #80]
.L109:
	ldr	r0, [sp, #84]
	cbz	r0, .L104
	bl	kfree
.L104:
	ldr	r3, [r8, #8]
	movs	r0, #3
	ldr	r1, [r4, #64]
	ldr	r2, [r3, #1084]
	str	r1, [sp, #28]
	ldr	r1, [r4]
	str	r1, [sp, #24]
	ldr	r1, [r2, #64]
	str	r1, [sp, #20]
	movs	r1, #2
	ldr	r2, [r2]
	str	r2, [sp, #16]
	ldr	r3, [r3, #1088]
	ldr	r2, .L183
	ldr	r3, [r3]
	str	r3, [sp, #12]
	mov	r3, #284
	strd	r3, r2, [sp]
	str	r3, [sp, #8]
	mov	r3, r2
	ldr	r2, .L183+4
	bl	vlog
.L95:
	ldr	r3, [r5, #8]
	cbz	r3, .L178
	ldr	r3, [r3, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L118
.L178:
	ldr	r0, [r5, #8]
	bl	mpi_obj_unref
	ldr	r0, [r5, #12]
	b	.L179
.L100:
	ldr	r2, [r8, #8]
	movs	r1, #5
	movs	r0, #3
	ldr	r3, [r2, #1084]
	ldr	r2, [r2, #1088]
	ldr	r2, [r2, #56]
	str	r2, [sp, #20]
	ldr	r2, [r4, #64]
	str	r2, [sp, #16]
	ldr	r2, [r4]
	str	r2, [sp, #12]
	ldr	r2, [r3, #64]
	str	r2, [sp, #8]
	ldr	r3, [r3]
	ldr	r2, .L183+8
	str	r3, [sp, #4]
	movs	r3, #206
	str	r3, [sp]
	ldr	r3, .L183+12
	bl	vlog
	ldr	r0, [sp, #76]
	cbz	r0, .L105
	bl	kfree
	str	r7, [sp, #76]
.L105:
	ldr	r0, [sp, #80]
	cbz	r0, .L106
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #80]
.L106:
	ldr	r0, [sp, #84]
	cbz	r0, .L97
	bl	kfree
.L97:
	ldr	r8, [r8]
	b	.L96
.L112:
	cbz	r4, .L114
	ldr	r3, [r6, #76]
	cmp	r3, #0
	itttt	ge
	addge	r3, r3, #16
	movge	r2, #0
	addge	r3, r4, r3, lsl #2
	strge	r2, [r3, #4]
.L114:
	ldr	r7, [r6, #260]
	add	r8, r6, #260
	subs	r7, r7, #4
.L115:
	adds	r3, r7, #4
	cmp	r3, r8
	beq	.L177
	ldr	r0, [r7]
	movs	r2, #0
	mov	r1, r4
	bl	mpi_node_buffer_in
	ldr	r7, [r7, #4]
	subs	r7, r7, #4
	b	.L115
.L118:
	ldr	r3, [r5, #12]
	cbz	r3, .L120
	ldr	r2, [r6, #76]
	cmp	r2, #0
	itttt	ge
	addge	r2, r2, #16
	addge	r3, r3, r2, lsl #2
	movge	r2, #0
	strge	r2, [r3, #4]
.L120:
	ldr	r7, [r6, #260]
	add	r8, r6, #260
	subs	r7, r7, #4
.L121:
	adds	r3, r7, #4
	cmp	r3, r8
	beq	.L178
	ldr	r0, [r7]
	movs	r2, #0
	ldr	r1, [r5, #12]
	bl	mpi_node_buffer_in
	ldr	r7, [r7, #4]
	subs	r7, r7, #4
	b	.L121
.L184:
	.align	2
.L183:
	.word	.LANCHOR0+212
	.word	.LC24
	.word	.LC23
	.word	.LANCHOR0+251
	.fnend
	.size	vrgn_thread_fn, .-vrgn_thread_fn
	.align	1
	.global	list_rgn_find_by_handle
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	list_rgn_find_by_handle, %function
list_rgn_find_by_handle:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L189
	ldr	r3, [r2, #196]!
.L186:
	cmp	r3, r2
	bne	.L188
	movs	r3, #0
.L185:
	mov	r0, r3
	bx	lr
.L188:
	ldr	r1, [r3, #28]
	cmp	r1, r0
	beq	.L185
	ldr	r3, [r3]
	b	.L186
.L190:
	.align	2
.L189:
	.word	.LANCHOR1
	.fnend
	.size	list_rgn_find_by_handle, .-list_rgn_find_by_handle
	.align	1
	.global	vrgn_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_create, %function
vrgn_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r6, r1
	ldr	r0, [r1]
	bl	list_rgn_find_by_handle
	ldr	r3, [r6, #1052]
	cmp	r3, #1144
	mov	r5, r0
	beq	.L192
	movw	r3, #545
	ldr	r2, .L199
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L199+4
	movs	r0, #3
	bl	vlog
.L198:
	mov	r0, #-1
	b	.L191
.L192:
	cbz	r0, .L194
	mov	r2, #1048
	adds	r1, r6, #4
	adds	r0, r0, #36
	movs	r4, #0
	bl	memcpy
	str	r4, [r5, #1084]
	str	r4, [r5, #32]
	movs	r1, #4
	ldr	r3, [r6]
	movs	r0, #3
	ldr	r2, .L199+8
	str	r3, [sp, #4]
	mov	r3, #552
	str	r3, [sp]
	ldr	r3, .L199+4
	bl	vlog
	mov	r0, r4
.L191:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L194:
	ldr	r3, .L199+12
	mov	r1, #3520
	ldr	r7, [r6, #1140]
	ldr	r0, [r3, #44]
	bl	kmem_cache_alloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L198
	ldr	r3, [r6, #4]
	cmp	r3, #1
	bne	.L195
	and	r7, r7, #63
	mov	r1, r5
	movs	r0, #18
	mov	r2, r7
	bl	vsys_get_node_from_map
	cbnz	r0, .L196
	mov	r3, r7
	movw	r2, #561
	ldr	r1, .L199+4
	ldr	r0, .L199+16
	bl	printk
	b	.L198
.L196:
	str	r0, [r4, #1088]
.L195:
	ldr	r3, [r6]
	str	r3, [r4, #28]
	ldr	r3, [r6, #20]
	cmp	r3, #1
	bne	.L197
	movs	r5, #0
	ldr	r2, .L199+20
	str	r5, [r4, #16]
	add	r0, r4, #20
	ldr	r1, .L199+24
	bl	__init_swait_queue_head
	mov	r3, #1000
	str	r5, [r4, #8]
	str	r3, [r4, #12]
.L197:
	mov	r2, #1048
	adds	r1, r6, #4
	add	r0, r4, #36
	movs	r5, #0
	bl	memcpy
	ldr	r3, .L199+28
	str	r5, [r4, #1084]
	movs	r1, #4
	str	r5, [r4, #32]
	adds	r3, r3, #196
	ldr	r2, [r3, #4]
	movs	r0, #3
	str	r4, [r3, #4]
	strd	r3, r2, [r4]
	str	r4, [r2]
	ldr	r3, [r6]
	ldr	r2, .L199+32
	str	r3, [sp, #4]
	movw	r3, #578
	str	r3, [sp]
	ldr	r3, .L199+4
	bl	vlog
	mov	r0, r5
	b	.L191
.L200:
	.align	2
.L199:
	.word	.LC25
	.word	.LANCHOR0+261
	.word	.LC26
	.word	kmalloc_caches
	.word	.LC27
	.word	.LANCHOR1+212
	.word	.LC28
	.word	.LANCHOR1
	.word	.LC29
	.fnend
	.size	vrgn_create, .-vrgn_create
	.align	1
	.global	vrgn_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_destroy, %function
vrgn_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r6, r1
	ldr	r5, [r1]
	mov	r0, r5
	bl	list_rgn_find_by_handle
	ldr	r3, [r6, #1052]
	cmp	r3, #1144
	mov	r4, r0
	beq	.L202
	mov	r3, #660
	ldr	r2, .L205
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L205+4
	movs	r0, #3
	bl	vlog
	mov	r0, #-1
.L201:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L202:
	cmp	r0, #0
	beq	.L201
	ldr	r3, [r0, #28]
	movs	r1, #4
	ldr	r2, .L205+8
	movs	r0, #3
	strd	r5, r3, [sp, #4]
	movw	r3, #666
	str	r3, [sp]
	ldr	r3, .L205+4
	bl	vlog
	ldrd	r2, r3, [r4]
	mov	r0, r4
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r2, #256
	mov	r3, #290
	strd	r2, r3, [r4]
	bl	kfree
	movs	r0, #0
	b	.L201
.L206:
	.align	2
.L205:
	.word	.LC25
	.word	.LANCHOR0+273
	.word	.LC30
	.fnend
	.size	vrgn_destroy, .-vrgn_destroy
	.align	1
	.global	vrgn_upate_canvas_sync
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_upate_canvas_sync, %function
vrgn_upate_canvas_sync:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r1
	ldr	r0, [r1, #4]
	bl	list_rgn_find_by_handle
	ldr	r3, [r4, #8]
	cmp	r3, #184
	beq	.L208
	movw	r3, #679
	ldr	r2, .L212
	str	r3, [sp]
	ldr	r3, .L212+4
.L211:
	movs	r1, #2
	movs	r0, #3
	bl	vlog
	mov	r0, #-1
.L207:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L208:
	cbnz	r0, .L210
	mov	r3, #684
	ldr	r2, .L212+8
	str	r3, [sp]
	ldr	r3, .L212+4
	b	.L211
.L210:
	ldr	r2, [r0, #12]
	movs	r3, #1
	str	r3, [r0, #8]
	movs	r3, #0
	str	r3, [r0, #16]
	sub	r3, r3, #1431655765
	adds	r0, r0, #16
	umull	r2, r3, r2, r3
	lsrs	r1, r3, #1
	bl	wait_for_completion_interruptible_timeout
	cmp	r0, #0
	ite	gt
	movgt	r0, #0
	movle	r0, #1
	rsbs	r0, r0, #0
	b	.L207
.L213:
	.align	2
.L212:
	.word	.LC31
	.word	.LANCHOR0+286
	.word	.LC32
	.fnend
	.size	vrgn_upate_canvas_sync, .-vrgn_upate_canvas_sync
	.global	__aeabi_uidiv
	.align	1
	.global	set_osd_invert
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_osd_invert, %function
set_osd_invert:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r6, [r0, #8]
	ldr	r1, [r0, #68]
	ldr	r0, [r6, #44]
	bl	__aeabi_uidiv
	ldr	r1, [r4, #72]
	subs	r3, r1, #1
	adds	r0, r0, #7
	lsrs	r5, r0, #3
	ldr	r0, [r6, #48]
	add	r3, r3, r0
	rsbs	r0, r1, #0
	ands	r0, r0, r3
	bl	__aeabi_uidiv
	mov	r6, r0
	ldr	r0, .L215
	bl	mpi_allocator_find_by_name
	mul	r2, r6, r5
	movs	r1, #0
	bl	mpi_allocator_alloc
	movs	r1, #0
	mov	r6, r0
	bl	mpi_buf_create
	mov	r5, r0
	mov	r0, r6
	bl	dma_buf_put
	str	r5, [r4, #12]
	pop	{r4, r5, r6, pc}
.L216:
	.align	2
.L215:
	.word	.LC33
	.fnend
	.size	set_osd_invert, .-set_osd_invert
	.align	1
	.global	get_list_node_rgn_from_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_list_node_rgn_from_node, %function
get_list_node_rgn_from_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r1, #84]!
.L218:
	cmp	r3, r1
	bne	.L220
	movs	r3, #0
.L217:
	mov	r0, r3
	bx	lr
.L220:
	ldr	r2, [r3, #8]
	ldr	r2, [r2, #28]
	cmp	r2, r0
	beq	.L217
	ldr	r3, [r3]
	b	.L218
	.fnend
	.size	get_list_node_rgn_from_node, .-get_list_node_rgn_from_node
	.align	1
	.global	rgn_attach_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rgn_attach_node, %function
rgn_attach_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r5, r0
	add	r0, r1, #1128
	mov	r6, r1
	ldm	r0, {r0, r1, r2}
	bl	vsys_get_node_from_map
	mov	r4, r0
	cbnz	r0, .L222
	ldr	r3, [r6, #1136]
	movs	r1, #2
	ldr	r2, .L226
	movs	r0, #3
	str	r3, [sp, #12]
	ldr	r3, [r6, #1132]
	str	r3, [sp, #8]
	ldr	r3, [r6, #1128]
	str	r3, [sp, #4]
	mov	r3, #418
	str	r3, [sp]
	ldr	r3, .L226+4
	bl	vlog
	mov	r0, #-1
.L221:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L222:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0, #88]
	add	r2, r0, #84
	str	r5, [r0, #88]
	strd	r2, r3, [r5]
	str	r5, [r3]
	ldr	r3, [r0, #92]
	adds	r3, r3, #1
	str	r3, [r0, #92]
	ldr	r3, [r5, #8]
	ldr	r3, [r3, #36]
	cmp	r3, #1
	bne	.L224
	ldr	r1, .L226+8
	bl	mpi_node_add_out_filter
	ldr	r3, .L226+12
	str	r3, [r4, #304]
.L224:
	ldr	r3, [r5, #8]
	ldr	r3, [r3, #52]
	cmp	r3, #1
	itt	eq
	ldreq	r3, .L226+16
	streq	r3, [r4, #348]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	b	.L221
.L227:
	.align	2
.L226:
	.word	.LC34
	.word	.LANCHOR0+309
	.word	.LC35
	.word	out_filter_callback
	.word	osd_sync_callback
	.fnend
	.size	rgn_attach_node, .-rgn_attach_node
	.align	1
	.global	vrgn_attach_chn
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_attach_chn, %function
vrgn_attach_chn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #24
	sub	sp, sp, #24
	ldr	r0, [r1]
	mov	r4, r1
	bl	list_rgn_find_by_handle
	mov	r6, r0
	cbz	r0, .L233
	ldr	r3, .L240
	mov	r1, #3520
	ldr	r0, [r3, #4]
	bl	kmem_cache_alloc
	ldr	r3, [r4, #1052]
	cmp	r3, #1144
	mov	r5, r0
	beq	.L230
	movw	r3, #594
	ldr	r2, .L240+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L240+8
	movs	r0, #3
	bl	vlog
.L233:
	mov	r5, #-1
	b	.L228
.L230:
	str	r6, [r0, #8]
	movs	r2, #72
	add	r1, r4, #1056
	adds	r0, r0, #16
	bl	memcpy
	ldr	r3, [r5, #84]
	cbz	r3, .L231
	mov	r0, r5
	bl	set_osd_invert
.L231:
	mov	r0, r5
	mov	r1, r4
	bl	rgn_attach_node
	mov	r5, r0
	cbz	r0, .L232
	ldr	r3, [r4, #1136]
	movs	r1, #2
	ldr	r2, .L240+12
	movs	r0, #3
	str	r3, [sp, #16]
	ldr	r3, [r4, #1132]
	str	r3, [sp, #12]
	ldr	r3, [r4, #1128]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #605
	str	r3, [sp]
	ldr	r3, .L240+8
	bl	vlog
.L232:
	ldr	r3, [r6, #32]
	movs	r1, #4
	ldr	r2, .L240+16
	movs	r0, #3
	adds	r3, r3, #1
	str	r3, [r6, #32]
	ldr	r3, [r4, #1136]
	str	r3, [sp, #16]
	ldr	r3, [r4, #1132]
	str	r3, [sp, #12]
	ldr	r3, [r4, #1128]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #609
	str	r3, [sp]
	ldr	r3, .L240+8
	bl	vlog
.L228:
	mov	r0, r5
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L241:
	.align	2
.L240:
	.word	kmalloc_caches
	.word	.LC25
	.word	.LANCHOR0+325
	.word	.LC36
	.word	.LC37
	.fnend
	.size	vrgn_attach_chn, .-vrgn_attach_chn
	.align	1
	.global	rgn_dettach_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rgn_dettach_node, %function
rgn_dettach_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r4, r0
	add	r0, r0, #1128
	ldm	r0, {r0, r1, r2}
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbnz	r0, .L243
	ldr	r3, [r4, #1136]
	movs	r1, #2
	ldr	r2, .L252
	movs	r0, #3
	str	r3, [sp, #12]
	ldr	r3, [r4, #1132]
	str	r3, [sp, #8]
	ldr	r3, [r4, #1128]
	str	r3, [sp, #4]
	movw	r3, #445
	str	r3, [sp]
	ldr	r3, .L252+4
	bl	vlog
	mov	r0, #-1
.L242:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L243:
	mov	r1, r0
	ldr	r0, [r4]
	bl	get_list_node_rgn_from_node
	mov	r4, r0
	cmp	r0, #0
	beq	.L242
	movw	r3, #451
	ldr	r2, .L252+8
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L252+4
	movs	r0, #3
	bl	vlog
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldrd	r2, r3, [r4]
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r3, #290
	mov	r2, #256
	strd	r2, r3, [r4]
	ldr	r3, [r5, #92]
	subs	r3, r3, #1
	str	r3, [r5, #92]
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #36]
	cmp	r3, #1
	bne	.L245
	ldr	r1, .L252+12
	mov	r0, r5
	bl	mpi_node_del_out_filter
.L245:
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #52]
	cmp	r3, #1
	itt	eq
	moveq	r3, #0
	streq	r3, [r5, #348]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, [r4, #12]
	cbz	r0, .L247
	bl	mpi_obj_unref
.L247:
	mov	r0, r4
	bl	kfree
	movs	r0, #0
	b	.L242
.L253:
	.align	2
.L252:
	.word	.LC34
	.word	.LANCHOR0+341
	.word	.LC38
	.word	.LC35
	.fnend
	.size	rgn_dettach_node, .-rgn_dettach_node
	.align	1
	.global	vrgn_dettach_chn
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_dettach_chn, %function
vrgn_dettach_chn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #24
	sub	sp, sp, #24
	ldr	r0, [r1]
	mov	r4, r1
	bl	list_rgn_find_by_handle
	mov	r6, r0
	cbz	r0, .L258
	ldr	r3, [r4, #1052]
	cmp	r3, #1144
	beq	.L256
	movw	r3, #623
	ldr	r2, .L262
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L262+4
	movs	r0, #3
	bl	vlog
.L258:
	mov	r5, #-1
	b	.L254
.L256:
	mov	r0, r4
	bl	rgn_dettach_node
	mov	r5, r0
	cbz	r0, .L257
	ldr	r3, [r4, #1136]
	movs	r1, #2
	ldr	r2, .L262+8
	movs	r0, #3
	str	r3, [sp, #16]
	ldr	r3, [r4, #1132]
	str	r3, [sp, #12]
	ldr	r3, [r4, #1128]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #630
	str	r3, [sp]
	ldr	r3, .L262+4
	bl	vlog
.L257:
	ldr	r3, [r6, #32]
	movs	r1, #4
	ldr	r2, .L262+12
	movs	r0, #3
	subs	r3, r3, #1
	str	r3, [r6, #32]
	ldr	r3, [r4, #1136]
	str	r3, [sp, #16]
	ldr	r3, [r4, #1132]
	str	r3, [sp, #12]
	ldr	r3, [r4, #1128]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #634
	str	r3, [sp]
	ldr	r3, .L262+4
	bl	vlog
.L254:
	mov	r0, r5
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L263:
	.align	2
.L262:
	.word	.LC25
	.word	.LANCHOR0+358
	.word	.LC39
	.word	.LC40
	.fnend
	.size	vrgn_dettach_chn, .-vrgn_dettach_chn
	.align	1
	.global	update_canvas_to_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	update_canvas_to_node, %function
update_canvas_to_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r4, r0
	ldrb	r5, [r0]	@ zero_extendqisi2
	cmp	r5, #0
	beq	.L265
	ldr	r0, [r0, #4]
	bl	list_rgn_find_by_handle
	mov	r6, r0
	cbnz	r0, .L266
.L281:
	mov	r0, #-1
	b	.L264
.L266:
	ldr	r0, [r4, #12]
	bl	get_mpi_buf
	mov	r5, r0
	cmp	r0, #0
	beq	.L281
	ldr	r3, [r0, #16]
	cbnz	r3, .L269
	mov	r3, #488
	ldr	r2, .L282
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L282+4
	movs	r0, #3
	bl	vlog
.L269:
	ldr	r3, [r6, #1084]
	cmp	r3, r5
	bne	.L270
	ldr	r3, [r6, #52]
	cmp	r3, #1
	bls	.L270
	ldr	r3, [r4, #4]
	movs	r1, #2
	ldr	r2, .L282+8
	movs	r0, #3
	str	r3, [sp, #4]
	mov	r3, #492
	str	r3, [sp]
	ldr	r3, .L282+4
	bl	vlog
.L270:
	movs	r2, #80
	add	r1, r4, #104
	add	r0, r5, #232
	bl	memcpy
	movs	r0, #0
	str	r5, [r6, #1084]
.L264:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L265:
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L281
	ldrd	r1, r2, [r0, #20]
	ldr	r0, [r0, #16]
	bl	vsys_get_node_from_map
	mov	r1, r0
	cbnz	r0, .L271
	ldr	r3, [r4, #24]
	movs	r1, #2
	ldr	r2, .L282+12
	movs	r0, #3
	str	r3, [sp, #12]
	ldr	r3, [r4, #20]
	str	r3, [sp, #8]
	ldr	r3, [r4, #16]
	str	r3, [sp, #4]
	movw	r3, #501
	str	r3, [sp]
	ldr	r3, .L282+4
	bl	vlog
	b	.L281
.L271:
	ldr	r0, [r4, #4]
	bl	get_list_node_rgn_from_node
	mov	r6, r0
	cmp	r0, #0
	beq	.L281
	mov	r3, #506
	ldr	r2, .L282+16
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L282+4
	movs	r0, #3
	bl	vlog
	movs	r2, #72
	add	r1, r4, #28
	add	r0, r6, #16
	bl	memcpy
	mov	r0, r5
	b	.L264
.L283:
	.align	2
.L282:
	.word	.LC41
	.word	.LANCHOR0+375
	.word	.LC42
	.word	.LC34
	.word	.LC43
	.fnend
	.size	update_canvas_to_node, .-update_canvas_to_node
	.align	1
	.global	vrgn_update_canvas_to_chns
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_update_canvas_to_chns, %function
vrgn_update_canvas_to_chns:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r5, r1
	ldr	r3, [r1, #8]
	cmp	r3, #184
	beq	.L285
	movw	r3, #643
	ldr	r2, .L290
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L290+4
	movs	r0, #3
	bl	vlog
	mov	r4, #-1
.L284:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L285:
	mov	r0, r1
	bl	update_canvas_to_node
	mov	r4, r0
	cmp	r0, #0
	beq	.L284
	ldr	r3, [r5, #4]
	movs	r1, #2
	ldr	r2, .L290+8
	movs	r0, #3
	str	r3, [sp, #4]
	movw	r3, #649
	str	r3, [sp]
	ldr	r3, .L290+4
	bl	vlog
	b	.L284
.L291:
	.align	2
.L290:
	.word	.LC31
	.word	.LANCHOR0+397
	.word	.LC44
	.fnend
	.size	vrgn_update_canvas_to_chns, .-vrgn_update_canvas_to_chns
	.align	1
	.global	vrgn_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_probe, %function
vrgn_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movs	r2, #212
	ldr	r4, .L294
	mov	r6, r0
	movs	r1, #0
	ldr	r5, .L294+4
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
	add	r0, r4, #196
	add	r1, r5, #140
	str	r0, [r4, #196]
	str	r0, [r4, #200]
	adds	r0, r4, #4
	movs	r3, #0
	add	r2, r4, #204
	str	r3, [r4, #72]
	strd	r3, r3, [r4, #80]
	str	r2, [r4, #204]
	str	r2, [r4, #208]
	str	r3, [r4, #168]
	add	r3, r4, #172
	strd	r3, r3, [r4, #172]
	bl	mpi_sysfs_dev_add
	ldr	r3, .L294+8
	mov	r2, #-1
	mov	r1, r4
	ldr	r0, .L294+12
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L293
	bl	wake_up_process
.L293:
	str	r5, [r4, #180]
	pop	{r4, r5, r6, pc}
.L295:
	.align	2
.L294:
	.word	.LANCHOR1
	.word	.LANCHOR0+424
	.word	.LC45
	.word	vrgn_thread_fn
	.fnend
	.size	vrgn_probe, .-vrgn_probe
	.align	1
	.global	vrgn_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_shutdown, %function
vrgn_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r3, #1
	ldr	r4, .L297
	ldr	r0, [r4, #180]
	str	r3, [r4, #184]
	bl	kthread_stop
	adds	r0, r4, #4
	pop	{r4, lr}
	b	mpi_dev_unregister
.L298:
	.align	2
.L297:
	.word	.LANCHOR1
	.fnend
	.size	vrgn_shutdown, .-vrgn_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	vrgn_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_init, %function
vrgn_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L300
	bl	vrgn_probe
	movs	r0, #0
	pop	{r3, pc}
.L301:
	.align	2
.L300:
	.word	.LC45
	.fnend
	.size	vrgn_init, .-vrgn_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	vrgn_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_exit, %function
vrgn_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	vrgn_shutdown
	.fnend
	.size	vrgn_exit, .-vrgn_exit
	.global	g_vrgn_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.37104, %object
	.size	__func__.37104, 11
__func__.37104:
	.ascii	"vrgn_ioctl\000"
	.space	1
	.type	tp, %object
	.size	tp, 40
tp:
	.word	0
	.word	.LC53
	.word	1
	.word	.LC54
	.word	2
	.word	.LC55
	.word	3
	.word	.LC56
	.word	4
	.word	.LC57
	.type	fmt, %object
	.size	fmt, 104
fmt:
	.word	65552
	.word	.LC58
	.word	65553
	.word	.LC59
	.word	65554
	.word	.LC58
	.word	65555
	.word	.LC58
	.word	65548
	.word	.LC60
	.word	65549
	.word	.LC61
	.word	65558
	.word	.LC60
	.word	65559
	.word	.LC60
	.word	65546
	.word	.LC62
	.word	65547
	.word	.LC63
	.word	65557
	.word	.LC64
	.word	65556
	.word	.LC64
	.word	65562
	.word	.LC65
	.type	__func__.37042, %object
	.size	__func__.37042, 13
__func__.37042:
	.ascii	"vrgn_version\000"
	.type	__func__.37126, %object
	.size	__func__.37126, 10
__func__.37126:
	.ascii	"vrgn_open\000"
	.type	__func__.36932, %object
	.size	__func__.36932, 20
__func__.36932:
	.ascii	"out_filter_callback\000"
	.type	__func__.37156, %object
	.size	__func__.37156, 13
__func__.37156:
	.ascii	"vrgn_release\000"
	.type	__func__.36785, %object
	.size	__func__.36785, 15
__func__.36785:
	.ascii	"vrgn_thread_fn\000"
	.type	__func__.36765, %object
	.size	__func__.36765, 24
__func__.36765:
	.ascii	"vrgn_set_osd_frame_info\000"
	.type	__func__.36772, %object
	.size	__func__.36772, 10
__func__.36772:
	.ascii	"do_ex_osd\000"
	.type	__func__.37051, %object
	.size	__func__.37051, 12
__func__.37051:
	.ascii	"vrgn_create\000"
	.type	__func__.37082, %object
	.size	__func__.37082, 13
__func__.37082:
	.ascii	"vrgn_destroy\000"
	.type	__func__.37090, %object
	.size	__func__.37090, 23
__func__.37090:
	.ascii	"vrgn_upate_canvas_sync\000"
	.type	__func__.37003, %object
	.size	__func__.37003, 16
__func__.37003:
	.ascii	"rgn_attach_node\000"
	.type	__func__.37060, %object
	.size	__func__.37060, 16
__func__.37060:
	.ascii	"vrgn_attach_chn\000"
	.type	__func__.37016, %object
	.size	__func__.37016, 17
__func__.37016:
	.ascii	"rgn_dettach_node\000"
	.type	__func__.37068, %object
	.size	__func__.37068, 17
__func__.37068:
	.ascii	"vrgn_dettach_chn\000"
	.type	__func__.37034, %object
	.size	__func__.37034, 22
__func__.37034:
	.ascii	"update_canvas_to_node\000"
	.type	__func__.37075, %object
	.size	__func__.37075, 27
__func__.37075:
	.ascii	"vrgn_update_canvas_to_chns\000"
	.type	vrgn_ioctls, %object
	.size	vrgn_ioctls, 140
vrgn_ioctls:
	.word	-1068732928
	.word	0
	.word	vrgn_version
	.word	.LC46
	.word	0
	.word	-998740479
	.word	0
	.word	vrgn_create
	.word	.LC47
	.word	0
	.word	-998740478
	.word	0
	.word	vrgn_destroy
	.word	.LC48
	.word	0
	.word	-998740477
	.word	0
	.word	vrgn_attach_chn
	.word	.LC49
	.word	0
	.word	-998740476
	.word	0
	.word	vrgn_dettach_chn
	.word	.LC50
	.word	0
	.word	-1061655035
	.word	0
	.word	vrgn_update_canvas_to_chns
	.word	.LC51
	.word	0
	.word	-1061655034
	.word	0
	.word	vrgn_upate_canvas_sync
	.word	.LC52
	.word	0
	.type	vrgn_driver_fops, %object
	.size	vrgn_driver_fops, 128
vrgn_driver_fops:
	.word	0
	.space	4
	.word	vrgn_read
	.space	24
	.word	0
	.word	vrgn_ioctl
	.word	vrgn_compat_ioctl
	.word	0
	.space	4
	.word	vrgn_open
	.space	4
	.word	vrgn_release
	.space	60
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	g_vrgn_dev, %object
	.size	g_vrgn_dev, 212
g_vrgn_dev:
	.space	212
	.type	__key.13543, %object
	.size	__key.13543, 0
__key.13543:
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"%s,%d:vrgn_ioctl: nr = %d, index = %d dev = %p\000"
.LC1:
	.ascii	"\033[31m\033[1m%s,%d:error: index = %d\033[0m\000"
.LC2:
	.ascii	"\033[31m\033[1m%s,%d:error: ioctl is null\033[0m\000"
.LC3:
	.ascii	"-1\000"
.LC4:
	.ascii	"vmpi:9093280df536e902dd5125cdc9953509ebc59a74-v1.4."
	.ascii	"32\000"
.LC5:
	.ascii	"module: %s\012version: %s\012build: %s\012\000"
.LC6:
	.ascii	"-2023-02-02-15:30:51\000"
.LC7:
	.ascii	"*****************************************rgn list**"
	.ascii	"*********************************************\012\000"
.LC8:
	.ascii	"handle---type------width---height---format----buf_i"
	.ascii	"d---refcnt---channel_num---------------------\012\000"
.LC9:
	.ascii	"%-#8d %-#9s %-#7d %-#8d %-#9s %-#8d %-#8d \000"
.LC10:
	.ascii	"****************************************node_rgn li"
	.ascii	"st*******************************************\012\000"
.LC11:
	.ascii	"id---name-------h-x---y---h-x---y---h-x---y---h-x--"
	.ascii	"-y---h-x---y---h-x---y---h-x---y---h-x---y---\012\000"
.LC12:
	.ascii	"%-#8d \000"
.LC13:
	.ascii	"\012\000"
.LC14:
	.ascii	"%-#4d %-#10s \000"
.LC15:
	.ascii	"%-#1d %-#3d %-#4d\000"
.LC16:
	.ascii	"\033[31m\033[1m%s,%d:struct vrgn_version mismatchin"
	.ascii	"g\033[0m\000"
.LC17:
	.ascii	"\033[31m\033[1m%s,%d:vrgn open fail\033[0m\000"
.LC18:
	.ascii	"%s,%d:%s ctx(%p) ready for node(%d) buf(%d)\012\000"
.LC19:
	.ascii	"\033[32m\033[1m%s,%d:release list_rgn handle = %d s"
	.ascii	"uccess\033[0m\000"
.LC20:
	.ascii	"%s,%d:src buf(%d: %p), rga: ctx_id(%d), x(%d), y(%d"
	.ascii	"), w(%d), h(%d), vw(%d), vh(%d), pf(0x%x), cp(%d)\000"
.LC21:
	.ascii	"%s,%d:pat buf(%d: %p), rga: ctx_id(%d), x(%d), y(%d"
	.ascii	"), w(%d), h(%d), vw(%d), vh(%d), pf(0x%x), cp(%d)\000"
.LC22:
	.ascii	"%s,%d:dst buf(%d: %p), rga: ctx_id(%d), x(%d), y(%d"
	.ascii	"), w(%d), h(%d), vw(%d), vh(%d), pf(0x%x), cp(%d)\000"
.LC23:
	.ascii	"%s,%d:vrga done buf(%d: %p->%d: %p) nick_id(%d)\000"
.LC24:
	.ascii	"\033[31m\033[1m%s,%d:rga(%d) do buf(%d: %p->%d: %p)"
	.ascii	" err!\033[0m\000"
.LC25:
	.ascii	"\033[31m\033[1m%s,%d:struct vrgn_info mismatching\033"
	.ascii	"[0m\000"
.LC26:
	.ascii	"\033[32m\033[1m%s,%d:rgn handle = %d reduce osd siz"
	.ascii	"e success\033[0m\000"
.LC27:
	.ascii	"\0013%s[%d]: vrgn get rga node fail channel_id = %d"
	.ascii	"\012\000"
.LC28:
	.ascii	"&x->wait\000"
.LC29:
	.ascii	"\033[32m\033[1m%s,%d:rgn handle = %d create success"
	.ascii	"\033[0m\000"
.LC30:
	.ascii	"\033[32m\033[1m%s,%d:destroy rgn handle = %d, delet"
	.ascii	"e list_rgn handle = %d success\033[0m\000"
.LC31:
	.ascii	"\033[31m\033[1m%s,%d:struct canvas_info mismatching"
	.ascii	"\033[0m\000"
.LC32:
	.ascii	"\033[31m\033[1m%s,%d:rgn handle = %d not find list_"
	.ascii	"rgn\033[0m\000"
.LC33:
	.ascii	"valloc\000"
.LC34:
	.ascii	"\033[31m\033[1m%s,%d:get node [%d %d %d] fail\033[0"
	.ascii	"m\000"
.LC35:
	.ascii	"vrgn_out_filter\000"
.LC36:
	.ascii	"\033[31m\033[1m%s,%d:rgn handle = %d attach node [%"
	.ascii	"d %d %d] fail\033[0m\000"
.LC37:
	.ascii	"\033[32m\033[1m%s,%d:rgn handle = %d attach node [%"
	.ascii	"d %d %d] success\033[0m\000"
.LC38:
	.ascii	"%s,%d:unref list_node_rgn\000"
.LC39:
	.ascii	"\033[31m\033[1m%s,%d:rgn handle = %d dettach node ["
	.ascii	"%d %d %d] fail\033[0m\000"
.LC40:
	.ascii	"\033[32m\033[1m%s,%d:rgn handle = %d dettach from ["
	.ascii	"%d, %d, %d] success\033[0m\000"
.LC41:
	.ascii	"\033[31m\033[1m%s,%d:mpi_buf refcnt err\033[0m\000"
.LC42:
	.ascii	"\033[31m\033[1m%s,%d:rgn handle = %d update frame e"
	.ascii	"rror\033[0m\000"
.LC43:
	.ascii	"%s,%d:update list_node_rgn chn attr success\000"
.LC44:
	.ascii	"\033[31m\033[1m%s,%d:rgn handle = %d update canvas "
	.ascii	"to node fail\033[0m\000"
.LC45:
	.ascii	"vrgn\000"
.LC46:
	.ascii	"VRGN_IOCTL_VERSION\000"
.LC47:
	.ascii	"VRGN_IOCTL_CREATE\000"
.LC48:
	.ascii	"VRGN_IOCTL_DESTORY\000"
.LC49:
	.ascii	"VRGN_IOCTL_ATTACH_CH\000"
.LC50:
	.ascii	"VRGN_IOCTL_DETTACH_CH\000"
.LC51:
	.ascii	"VRGN_IOCTL_UPDATE_CANVAS\000"
.LC52:
	.ascii	"VRGN_IOCTL_UPDATE_SYNC\000"
.LC53:
	.ascii	"OVERLAY\000"
.LC54:
	.ascii	"OVERLAY_EX\000"
.LC55:
	.ascii	"COVER\000"
.LC56:
	.ascii	"MOSAIC\000"
.LC57:
	.ascii	"LINE\000"
.LC58:
	.ascii	"ARGB8888\000"
.LC59:
	.ascii	"ABGR8888\000"
.LC60:
	.ascii	"ARGB4444\000"
.LC61:
	.ascii	"ABGR4444\000"
.LC62:
	.ascii	"ARGB1555\000"
.LC63:
	.ascii	"ABGR1555\000"
.LC64:
	.ascii	"ARGB5551\000"
.LC65:
	.ascii	"2BPP\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
