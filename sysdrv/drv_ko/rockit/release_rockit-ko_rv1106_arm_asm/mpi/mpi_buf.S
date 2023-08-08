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
	.file	"mpi_buf.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_create:					
	.asciz 	"mpi_buf_create"					
__kstrtabns_mpi_buf_create:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_pool_import:					
	.asciz 	"mpi_buf_pool_import"					
__kstrtabns_mpi_buf_pool_import:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_pool_import_dmabuf:					
	.asciz 	"mpi_buf_pool_import_dmabuf"					
__kstrtabns_mpi_buf_pool_import_dmabuf:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_pool_request_buf:					
	.asciz 	"mpi_buf_pool_request_buf"					
__kstrtabns_mpi_buf_pool_request_buf:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_pool_add_user:					
	.asciz 	"mpi_buf_pool_add_user"					
__kstrtabns_mpi_buf_pool_add_user:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_pool_del_user:					
	.asciz 	"mpi_buf_pool_del_user"					
__kstrtabns_mpi_buf_pool_del_user:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_pool_destroy:					
	.asciz 	"mpi_buf_pool_destroy"					
__kstrtabns_mpi_buf_pool_destroy:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_pool_create:					
	.asciz 	"mpi_buf_pool_create"					
__kstrtabns_mpi_buf_pool_create:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_allocator_alloc:					
	.asciz 	"mpi_allocator_alloc"					
__kstrtabns_mpi_allocator_alloc:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_allocator_unregister:					
	.asciz 	"mpi_allocator_unregister"					
__kstrtabns_mpi_allocator_unregister:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_allocator_register:					
	.asciz 	"mpi_allocator_register"					
__kstrtabns_mpi_allocator_register:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.global	mpi_buf_pool_import
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_import, %function
mpi_buf_pool_import:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r0
	.pad #20
	sub	sp, sp, #20
	mov	r6, r1
	cbz	r0, .L2
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L3
.L2:
	mov	r0, #-1
.L1:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L3:
	ldr	r5, [r1, #60]
	cbnz	r5, .L7
	ldr	r2, [r1, #212]
	ldr	r3, [r0, #56]
	cmp	r2, r3
	bcc	.L7
	str	r0, [r6, #60]
	ldr	r3, [r0, #56]
	str	r3, [r1, #212]
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r0, #80]
	add	r7, r0, #76
	add	r3, r6, #52
	strd	r7, r2, [r6, #52]
	str	r3, [r2]
	str	r3, [r0, #80]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	bl	mpi_obj_ref
	ldr	r3, [r6]
	ldr	r2, .L11
	movs	r1, #5
	mov	r0, r5
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movs	r3, #192
	str	r3, [sp]
	ldr	r3, .L11+4
	bl	vlog
	mov	r0, r5
.L5:
	b	.L1
.L7:
	mov	r0, #-1
	b	.L5
.L12:
	.align	2
.L11:
	.word	.LC0
	.word	.LANCHOR0
	.fnend
	.size	mpi_buf_pool_import, .-mpi_buf_pool_import
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_destroy_rcu, %function
mpi_buf_destroy_rcu:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	mov	r4, r0
	sub	r5, r0, #28
	ldr	r0, [r0, #36]
	.pad #20
	sub	sp, sp, #20
	cbz	r0, .L14
	bl	dma_buf_put
.L14:
	ldr	r3, [r4, #20]
	movs	r1, #5
	ldr	r2, .L31
	movs	r0, #0
	str	r3, [sp, #12]
	ldr	r3, [r4, #16]
	strd	r5, r3, [sp, #4]
	movs	r3, #39
	str	r3, [sp]
	ldr	r3, .L31+4
	bl	vlog
	ldr	r2, [r4, #16]
	cbz	r2, .L15
	ldr	r3, [r4, #20]
	cbz	r3, .L15
	cmp	r2, #256
	beq	.L15
	cmp	r3, #290
	beq	.L15
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r2, #256
	mov	r3, #290
	strd	r2, r3, [r4, #16]
.L15:
	mov	r0, r5
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, lr}
	b	mpi_obj_unregister
.L32:
	.align	2
.L31:
	.word	.LC1
	.word	.LANCHOR0+20
	.fnend
	.size	mpi_buf_destroy_rcu, .-mpi_buf_destroy_rcu
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_release, %function
mpi_buf_pool_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r0
	ldr	r3, [r0]
	movs	r1, #5
	ldr	r2, .L35
	movs	r0, #0
	str	r3, [sp, #4]
	mov	r3, #342
	str	r3, [sp]
	ldr	r3, .L35+4
	bl	vlog
	ldr	r3, [r4, #76]
	ldr	r3, [r4, #12]
	lsls	r3, r3, #29
	bpl	.L33
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, lr}
	b	mpi_obj_unregister
.L33:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L36:
	.align	2
.L35:
	.word	.LC2
	.word	.LANCHOR0+40
	.fnend
	.size	mpi_buf_pool_release, .-mpi_buf_pool_release
	.align	1
	.global	mpi_allocator_register
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_allocator_register, %function
mpi_allocator_register:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	mov	r5, r1
	mov	r0, r2
	mov	r1, #3264
	bl	kstrdup
	orr	r2, r5, #64
	movs	r3, #0
	movs	r1, #7
	str	r0, [r4, #44]
	mov	r0, r4
	bl	mpi_obj_register
	ldr	r3, [r4, #44]
	ldr	r2, .L38
	movs	r1, #5
	movs	r0, #0
	str	r3, [sp, #4]
	mov	r3, #552
	str	r3, [sp]
	ldr	r3, .L38+4
	bl	vlog
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L39:
	.align	2
.L38:
	.word	.LC3
	.word	.LANCHOR0+61
	.fnend
	.size	mpi_allocator_register, .-mpi_allocator_register
	.align	1
	.global	mpi_allocator_alloc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_allocator_alloc, %function
mpi_allocator_alloc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r0
	mov	r5, r1
	mov	r6, r2
	bl	_cond_resched
	cbz	r4, .L41
	ldr	r3, [r4, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L42
.L41:
	movs	r5, #0
.L40:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L42:
	mov	r0, r4
	bl	mpi_obj_ref
	mov	r1, r5
	ldr	r3, [r4, #52]
	mov	r2, r6
	mov	r0, r4
	blx	r3
	mov	r5, r0
	cbnz	r0, .L44
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L48
	str	r3, [sp, #4]
	movw	r3, #499
	str	r3, [sp]
	ldr	r3, .L48+4
	bl	vlog
.L44:
	mov	r0, r4
	bl	mpi_obj_unref
	b	.L40
.L49:
	.align	2
.L48:
	.word	.LC4
	.word	.LANCHOR0+84
	.fnend
	.size	mpi_allocator_alloc, .-mpi_allocator_alloc
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_sub.constprop.3, %function
atomic_sub.constprop.3:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	pld	[r0]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r3, [r0]
	sub	r3, r3, #1
	strex	r2, r3, [r0]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	atomic_sub.constprop.3, .-atomic_sub.constprop.3
	.align	1
	.global	mpi_buf_pool_request_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_request_buf, %function
mpi_buf_pool_request_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	.pad #24
	sub	sp, sp, #24
	mov	r4, r1
	cbz	r0, .L67
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L53
.L67:
	movs	r4, #0
.L54:
	mov	r0, r4
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L53:
	ldr	r2, [r0]
	cbz	r1, .L62
	ldr	r3, [r1]
.L55:
	ldr	r1, [r5, #68]
	ldr	r0, [r5, #16]
	strd	r2, r3, [sp, #4]
	movs	r3, #241
	ldr	r2, .L68
	strd	r1, r0, [sp, #12]
	movs	r1, #5
	str	r3, [sp]
	movs	r0, #0
	ldr	r3, .L68+4
	bl	vlog
	cbnz	r4, .L56
.L58:
	mov	r6, r5
	ldr	r4, [r6, #76]!
.L66:
	subs	r4, r4, #52
	add	r3, r4, #52
	cmp	r3, r6
	beq	.L67
	mov	r0, r4
	bl	mpi_obj_ref_if_unused
	cbnz	r0, .L59
	ldr	r4, [r4, #52]
	b	.L66
.L62:
	mov	r3, #-1
	b	.L55
.L56:
	ldr	r3, [r4, #60]
	cmp	r3, r5
	bne	.L58
	mov	r0, r4
	bl	mpi_obj_ref_if_unused
	cmp	r0, #0
	beq	.L58
.L59:
	add	r0, r5, #68
	bl	atomic_sub.constprop.3
	ldr	r3, [r5, #68]
	ldr	r2, [r5, #72]
	movs	r1, #5
	movs	r0, #0
	cmp	r2, r3
	ldr	r2, .L68+8
	itt	gt
	ldrgt	r3, [r5, #68]
	strgt	r3, [r5, #72]
	ldr	r3, [r5, #64]
	adds	r3, r3, #1
	str	r3, [r5, #64]
	ldr	r3, [r4, #12]
	bic	r3, r3, #1024
	str	r3, [r4, #12]
	ldr	r3, [r4]
	str	r3, [sp, #8]
	ldr	r3, [r5]
	str	r3, [sp, #4]
	movw	r3, #267
	str	r3, [sp]
	ldr	r3, .L68+4
	bl	vlog
	b	.L54
.L69:
	.align	2
.L68:
	.word	.LC5
	.word	.LANCHOR0+104
	.word	.LC6
	.fnend
	.size	mpi_buf_pool_request_buf, .-mpi_buf_pool_request_buf
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_add.constprop.4, %function
atomic_add.constprop.4:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	pld	[r0]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r3, [r0]
	add	r3, r3, #1
	strex	r2, r3, [r0]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	atomic_add.constprop.4, .-atomic_add.constprop.4
	.align	1
	.global	mpi_buf_pool_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_destroy, %function
mpi_buf_pool_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r0
	.pad #20
	sub	sp, sp, #20
	mov	r7, r1
	cbz	r0, .L72
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L73
.L72:
	mov	r5, #-1
.L71:
	mov	r0, r5
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L73:
	bl	mpi_obj_ref
	str	r7, [sp, #8]
	movs	r1, #5
	ldr	r3, [r5]
	movs	r0, #0
	ldr	r2, .L81
	str	r3, [sp, #4]
	movw	r3, #389
	str	r3, [sp]
	ldr	r3, .L81+4
	bl	vlog
	ldr	r4, [r5, #24]
	ldr	r3, [r5, #12]
	adds	r6, r4, #4
	bic	r3, r3, #2
	mov	r0, r4
	str	r3, [r5, #12]
	bl	atomic_add.constprop.4
	mov	r0, r6
	bl	atomic_add.constprop.4
	ldr	r3, [r5, #16]
	cbnz	r3, .L75
	add	r0, r4, #8
	bl	complete_all
.L75:
	ldr	r1, .L81+8
	add	r0, r5, #28
	bl	call_rcu
	cbnz	r7, .L76
	add	r0, r4, #8
	bl	wait_for_completion
.L76:
	mov	r0, r6
	bl	atomic_sub.constprop.3
	pld	[r4]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub_return
1:	ldrex	r5, [r4]
	sub	r5, r5, #1
	strex	r3, r5, [r4]
	teq	r3, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r5, .L77
	mov	r0, r4
	bl	kfree
	b	.L71
.L77:
	movs	r5, #0
	b	.L71
.L82:
	.align	2
.L81:
	.word	.LC7
	.word	.LANCHOR0+129
	.word	mpi_buf_pool_destroy_rcu
	.fnend
	.size	mpi_buf_pool_destroy, .-mpi_buf_pool_destroy
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_destroy, %function
mpi_buf_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	ldr	r3, [r0]
	movs	r1, #5
	ldr	r5, [r0, #60]
	movs	r0, #0
	ldr	r2, .L92
	str	r3, [sp, #4]
	movs	r3, #52
	str	r3, [sp]
	ldr	r3, .L92+4
	bl	vlog
	ldr	r3, [r4, #196]
	cbz	r3, .L84
	mov	r0, r4
	blx	r3
.L84:
	ldr	r3, [r4, #12]
	orr	r3, r3, #4
	str	r3, [r4, #12]
	cbz	r5, .L85
	ldrd	r2, r3, [r4, #52]
	mov	r0, r5
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r3, #290
	str	r3, [r4, #56]
	bl	mpi_obj_unref
.L85:
	ldr	r1, .L92+8
	add	r0, r4, #28
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, lr}
	b	call_rcu
.L93:
	.align	2
.L92:
	.word	.LC8
	.word	.LANCHOR0+150
	.word	mpi_buf_destroy_rcu
	.fnend
	.size	mpi_buf_destroy, .-mpi_buf_destroy
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_release, %function
mpi_buf_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r4, r0
	ldr	r3, [r0]
	movs	r1, #5
	ldr	r5, [r0, #60]
	movs	r0, #0
	ldr	r2, .L114
	str	r3, [sp, #4]
	movs	r3, #76
	str	r3, [sp]
	ldr	r3, .L114+4
	bl	vlog
	cbnz	r5, .L95
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	b	mpi_buf_destroy
.L95:
	ldr	r3, [r5, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L96
	mov	r0, r5
	bl	mpi_obj_ref
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r4
	bl	mpi_buf_destroy
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	b	mpi_obj_unref
.L96:
	movs	r2, #128
	movs	r1, #0
	add	r0, r4, #68
	add	r7, r5, #84
	bl	memset
	add	r0, r5, #68
	bl	atomic_add.constprop.4
	ldr	r3, [r4, #12]
	orr	r3, r3, #1024
	str	r3, [r4, #12]
	ldr	r3, [r5, #84]
	cmp	r7, r3
	bne	.L97
.L105:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L97:
	ldr	r3, [r5, #84]
	movs	r6, #0
	subs	r3, r3, #64
.L99:
	add	r2, r3, #64
	cmp	r7, r2
	bne	.L101
	cbz	r6, .L102
	mov	r1, r4
	mov	r0, r6
	bl	mpi_node_buffer_available
.L102:
	ldr	r5, [r5, #84]
.L113:
	subs	r5, r5, #64
	add	r3, r5, #64
	cmp	r7, r3
	beq	.L105
	cmp	r5, r6
	beq	.L104
	ldr	r3, [r4, #16]
	cmp	r3, #0
	bne	.L105
	mov	r1, r4
	mov	r0, r5
	bl	mpi_node_buffer_available
.L104:
	ldr	r5, [r5, #64]
	b	.L113
.L101:
	cbz	r6, .L107
	ldr	r1, [r6, #72]
	ldr	r2, [r3, #72]
	cmp	r1, r2
	it	hi
	movhi	r6, r3
.L100:
	ldr	r3, [r3, #64]
	subs	r3, r3, #64
	b	.L99
.L107:
	mov	r6, r3
	b	.L100
.L115:
	.align	2
.L114:
	.word	.LC9
	.word	.LANCHOR0+166
	.fnend
	.size	mpi_buf_release, .-mpi_buf_release
	.align	1
	.global	mpi_buf_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_create, %function
mpi_buf_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L123
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r0
	mov	r6, r1
	ldr	r0, [r3, #36]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L116
	cbz	r5, .L118
	ldr	r0, [r5, #4]
	adds	r0, r0, #28
	bl	atomic_add.constprop.4
	ldr	r3, [r5]
	str	r3, [r4, #212]
.L119:
	ldr	r3, .L123+4
	movs	r2, #0
	movs	r1, #4
	str	r6, [r4, #216]
	str	r5, [r4, #64]
	mov	r0, r4
	bl	mpi_obj_register
	mov	r0, r4
	bl	mpi_obj_ref
	ldr	r3, [r4]
	ldr	r2, .L123+8
	movs	r1, #5
	movs	r0, #0
	str	r3, [sp, #4]
	movs	r3, #160
	str	r3, [sp]
	ldr	r3, .L123+12
	bl	vlog
.L116:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L118:
	str	r5, [r0, #212]
	b	.L119
.L124:
	.align	2
.L123:
	.word	kmalloc_caches
	.word	mpi_buf_release
	.word	.LC10
	.word	.LANCHOR0+182
	.fnend
	.size	mpi_buf_create, .-mpi_buf_create
	.align	1
	.global	mpi_buf_pool_import_dmabuf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_import_dmabuf, %function
mpi_buf_pool_import_dmabuf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	mov	r0, r1
	mov	r1, r2
	bl	mpi_buf_create
	mov	r4, r0
	cbz	r0, .L127
	ldr	r3, [r5, #4]
	mov	r1, r0
	str	r3, [r0, #4]
	mov	r0, r5
	bl	mpi_buf_pool_import
	mov	r5, r0
	mov	r0, r4
	bl	mpi_obj_unref
.L125:
	mov	r0, r5
	pop	{r3, r4, r5, pc}
.L127:
	mov	r5, #-1
	b	.L125
	.fnend
	.size	mpi_buf_pool_import_dmabuf, .-mpi_buf_pool_import_dmabuf
	.align	1
	.global	mpi_buf_pool_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_create, %function
mpi_buf_pool_create:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r7, r3
	ldr	r3, .L164
	mov	r9, r0
	mov	r6, r1
	mov	r1, #3520
	mov	r8, r2
	ldr	r0, [r3, #4]
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L128
	add	r3, r0, #76
	movs	r5, #0
	str	r3, [r0, #80]
	movs	r2, #64
	str	r5, [r0, #68]
	movs	r1, #5
	str	r3, [r0, #76]
	add	r3, r0, #84
	strd	r8, r6, [r0, #56]
	str	r3, [r0, #84]
	str	r3, [r0, #88]
	str	r7, [r0, #72]
	ldr	r3, .L164+4
	bl	mpi_obj_register
	cmp	r6, r5
	ldr	r3, [sp, #48]
	str	r3, [r4, #4]
	blt	.L130
.L132:
	bic	r10, r6, #-2147483648
.L131:
	cbnz	r7, .L139
	cmp	r6, #0
	bge	.L140
	cbz	r5, .L140
	mov	r0, r5
	bl	dma_buf_put
.L140:
	ldr	r3, [r4, #16]
	movs	r1, #5
	ldr	r2, .L164+8
	movs	r0, #0
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #467
	str	r3, [sp]
	ldr	r3, .L164+12
	bl	vlog
.L128:
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L141:
	mov	r5, r8
	b	.L132
.L130:
	cmp	r8, #0
	beq	.L141
	mov	r2, r8
	mov	r1, r6
	mov	r0, r9
	bl	mpi_allocator_alloc
	mov	r5, r0
	cmp	r0, #0
	bne	.L132
.L137:
	mov	r0, r4
	movs	r1, #0
	bl	mpi_buf_pool_destroy
	movs	r4, #0
	b	.L128
.L139:
	cmp	r6, #0
	blt	.L134
	cmp	r8, #0
	beq	.L134
	mov	r2, r8
	mov	r1, r6
	mov	r0, r9
	bl	mpi_allocator_alloc
	mov	r5, r0
	cmp	r0, #0
	beq	.L137
.L134:
	ldr	r3, [r4, #72]
	mov	r1, r5
	mov	r0, r4
	cmp	r7, r3
	ite	ne
	movne	r2, r6
	moveq	r2, r10
	bl	mpi_buf_pool_import_dmabuf
	cmp	r0, #0
	bge	.L136
	cmp	r5, #0
	beq	.L137
	mov	r0, r5
	bl	dma_buf_put
	b	.L137
.L136:
	cmp	r6, #0
	blt	.L138
	cbz	r5, .L138
	mov	r0, r5
	bl	dma_buf_put
.L138:
	subs	r7, r7, #1
	b	.L131
.L165:
	.align	2
.L164:
	.word	kmalloc_caches
	.word	mpi_buf_pool_release
	.word	.LC11
	.word	.LANCHOR0+197
	.fnend
	.size	mpi_buf_pool_create, .-mpi_buf_pool_create
	.align	1
	.global	mpi_buf_pool_del_user
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_del_user, %function
mpi_buf_pool_del_user:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	mov	r4, r1
	ldr	r3, [r1]
	ldr	r2, .L167
	str	r3, [sp, #12]
	ldr	r3, [r1, #44]
	movs	r1, #5
	ldr	r3, [r3, #44]
	str	r3, [sp, #8]
	ldr	r3, [r0]
	movs	r0, #0
	str	r3, [sp, #4]
	mov	r3, #326
	str	r3, [sp]
	ldr	r3, .L167+4
	bl	vlog
	movs	r1, #1
	mov	r0, r4
	bl	mpi_node_flush
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldrd	r2, r3, [r4, #64]
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r3, #290
	str	r3, [r4, #68]
	movs	r3, #0
	str	r3, [r4, #60]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L168:
	.align	2
.L167:
	.word	.LC12
	.word	.LANCHOR0+217
	.fnend
	.size	mpi_buf_pool_del_user, .-mpi_buf_pool_del_user
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_destroy_rcu, %function
mpi_buf_pool_destroy_rcu:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r7, r0
	ldr	r1, [r7, #56]!
	sub	r6, r0, #28
	mov	r4, r0
	ldr	r5, [r1], #-64
	subs	r5, r5, #64
.L170:
	add	r3, r1, #64
	cmp	r3, r7
	bne	.L171
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r7, r4
	ldr	r0, [r7, #48]!
	ldr	r5, [r0], #-52
	subs	r5, r5, #52
.L172:
	add	r3, r0, #52
	cmp	r3, r7
	bne	.L174
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #-16]
	mov	r0, r6
	orr	r3, r3, #4
	str	r3, [r4, #-16]
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_obj_unref
.L171:
	mov	r0, r6
	bl	mpi_buf_pool_del_user
	ldr	r3, [r5, #64]
	mov	r1, r5
	sub	r5, r3, #64
	b	.L170
.L174:
	ldr	r3, [r0, #12]
	lsls	r3, r3, #21
	bpl	.L173
	bl	mpi_buf_destroy
.L173:
	ldr	r3, [r5, #52]
	mov	r0, r5
	sub	r5, r3, #52
	b	.L172
	.fnend
	.size	mpi_buf_pool_destroy_rcu, .-mpi_buf_pool_destroy_rcu
	.align	1
	.global	mpi_buf_pool_add_user
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_add_user, %function
mpi_buf_pool_add_user:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r4, r0
	ldr	r3, [r0, #12]
	mov	r6, r1
	lsls	r3, r3, #30
	bpl	.L178
	ldr	r3, [r1]
	ldr	r2, .L199
	str	r3, [sp, #12]
	ldr	r3, [r1, #44]
	movs	r1, #5
	ldr	r3, [r3, #44]
	str	r3, [sp, #8]
	ldr	r3, [r0]
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #291
	str	r3, [sp]
	ldr	r3, .L199+4
	bl	vlog
	ldr	r0, [r6, #60]
	cbz	r0, .L180
	mov	r1, r6
	bl	mpi_buf_pool_del_user
.L180:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	str	r4, [r6, #60]
	add	r0, r4, #84
	ldr	r2, [r4, #88]
	add	r3, r6, #64
	strd	r0, r2, [r6, #64]
	str	r3, [r2]
	str	r3, [r4, #88]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cbz	r4, .L181
	ldr	r3, [r4, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L182
.L181:
.L182:
	ldr	r5, [r4, #76]!
.L198:
	subs	r5, r5, #52
	add	r3, r5, #52
	cmp	r3, r4
	bne	.L185
.L178:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L185:
	ldr	r3, [r5, #16]
	cbnz	r3, .L184
	mov	r1, r5
	mov	r0, r6
	bl	mpi_node_buffer_available
.L184:
	ldr	r5, [r5, #52]
	b	.L198
.L200:
	.align	2
.L199:
	.word	.LC13
	.word	.LANCHOR0+239
	.fnend
	.size	mpi_buf_pool_add_user, .-mpi_buf_pool_add_user
	.align	1
	.global	mpi_allocator_unregister
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_allocator_unregister, %function
mpi_allocator_unregister:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r4, r0
	ldr	r3, [r0, #44]
	movs	r1, #5
	ldr	r2, .L208
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #517
	str	r3, [sp]
	ldr	r3, .L208+4
	bl	vlog
	cbz	r4, .L202
	mov	r0, r4
	bl	mpi_obj_ref
.L202:
	ldr	r3, [r4, #12]
	bic	r3, r3, #2
	str	r3, [r4, #12]
	bl	synchronize_rcu
	ldr	r5, [r4, #24]
	ldr	r3, [r4, #12]
	adds	r6, r5, #4
	add	r7, r5, #8
	orr	r3, r3, #4
	mov	r0, r5
	str	r3, [r4, #12]
	bl	atomic_add.constprop.4
	mov	r0, r6
	bl	atomic_add.constprop.4
	ldr	r3, [r4, #16]
	cbnz	r3, .L203
	mov	r0, r7
	bl	complete_all
.L203:
	mov	r0, r4
	bl	mpi_obj_unref
	mov	r0, r7
	bl	wait_for_completion
	mov	r0, r6
	bl	atomic_sub.constprop.3
	pld	[r5]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub_return
1:	ldrex	r3, [r5]
	sub	r3, r3, #1
	strex	r2, r3, [r5]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L204
	mov	r0, r5
	bl	kfree
.L204:
	ldr	r0, [r4, #44]
	bl	kfree
	mov	r0, r4
	bl	mpi_obj_unregister
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L209:
	.align	2
.L208:
	.word	.LC14
	.word	.LANCHOR0+261
	.fnend
	.size	mpi_allocator_unregister, .-mpi_allocator_unregister
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.36404, %object
	.size	__func__.36404, 20
__func__.36404:
	.ascii	"mpi_buf_pool_import\000"
	.type	__func__.36264, %object
	.size	__func__.36264, 20
__func__.36264:
	.ascii	"mpi_buf_destroy_rcu\000"
	.type	__func__.36586, %object
	.size	__func__.36586, 21
__func__.36586:
	.ascii	"mpi_buf_pool_release\000"
	.type	__func__.36755, %object
	.size	__func__.36755, 23
__func__.36755:
	.ascii	"mpi_allocator_register\000"
	.type	__func__.36723, %object
	.size	__func__.36723, 20
__func__.36723:
	.ascii	"mpi_allocator_alloc\000"
	.type	__func__.36431, %object
	.size	__func__.36431, 25
__func__.36431:
	.ascii	"mpi_buf_pool_request_buf\000"
	.type	__func__.36668, %object
	.size	__func__.36668, 21
__func__.36668:
	.ascii	"mpi_buf_pool_destroy\000"
	.type	__func__.36269, %object
	.size	__func__.36269, 16
__func__.36269:
	.ascii	"mpi_buf_destroy\000"
	.type	__func__.36282, %object
	.size	__func__.36282, 16
__func__.36282:
	.ascii	"mpi_buf_release\000"
	.type	__func__.36379, %object
	.size	__func__.36379, 15
__func__.36379:
	.ascii	"mpi_buf_create\000"
	.type	__func__.36700, %object
	.size	__func__.36700, 20
__func__.36700:
	.ascii	"mpi_buf_pool_create\000"
	.type	__func__.36564, %object
	.size	__func__.36564, 22
__func__.36564:
	.ascii	"mpi_buf_pool_del_user\000"
	.type	__func__.36503, %object
	.size	__func__.36503, 22
__func__.36503:
	.ascii	"mpi_buf_pool_add_user\000"
	.type	__func__.36735, %object
	.size	__func__.36735, 25
__func__.36735:
	.ascii	"mpi_allocator_unregister\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"%s,%d:buf pool(%d) imported buf(%d)\000"
.LC1:
	.ascii	"%s,%d:buf release = %p, next = %p, prev = %p\000"
.LC2:
	.ascii	"%s,%d:buf pool(%d) released\000"
.LC3:
	.ascii	"%s,%d:allocator(%s) registered\000"
.LC4:
	.ascii	"%s,%d:alloc(%d) unable to alloc\000"
.LC5:
	.ascii	"%s,%d:buf pool(%d) request buf(%d), status: %d/%d\000"
.LC6:
	.ascii	"%s,%d:buf pool(%d) return buf(%d)\000"
.LC7:
	.ascii	"%s,%d:buf pool(%d) destroying, async: %d\000"
.LC8:
	.ascii	"%s,%d:buf(%d) destroying\000"
.LC9:
	.ascii	"%s,%d:buf(%d) released\000"
.LC10:
	.ascii	"%s,%d:buf(%d) created\000"
.LC11:
	.ascii	"%s,%d:buf pool(%d) created with %d buffers\000"
.LC12:
	.ascii	"%s,%d:buf pool(%d) del user(%s-%d)\000"
.LC13:
	.ascii	"%s,%d:buf pool(%d) add user(%s-%d)\000"
.LC14:
	.ascii	"%s,%d:allocator(%s) unregistering\000"
	.section	___ksymtab_gpl+mpi_allocator_alloc,"a"
	.align	2
	.type	__ksymtab_mpi_allocator_alloc, %object
	.size	__ksymtab_mpi_allocator_alloc, 12
__ksymtab_mpi_allocator_alloc:
	.word	mpi_allocator_alloc
	.word	__kstrtab_mpi_allocator_alloc
	.word	__kstrtabns_mpi_allocator_alloc
	.section	___ksymtab_gpl+mpi_allocator_register,"a"
	.align	2
	.type	__ksymtab_mpi_allocator_register, %object
	.size	__ksymtab_mpi_allocator_register, 12
__ksymtab_mpi_allocator_register:
	.word	mpi_allocator_register
	.word	__kstrtab_mpi_allocator_register
	.word	__kstrtabns_mpi_allocator_register
	.section	___ksymtab_gpl+mpi_allocator_unregister,"a"
	.align	2
	.type	__ksymtab_mpi_allocator_unregister, %object
	.size	__ksymtab_mpi_allocator_unregister, 12
__ksymtab_mpi_allocator_unregister:
	.word	mpi_allocator_unregister
	.word	__kstrtab_mpi_allocator_unregister
	.word	__kstrtabns_mpi_allocator_unregister
	.section	___ksymtab_gpl+mpi_buf_create,"a"
	.align	2
	.type	__ksymtab_mpi_buf_create, %object
	.size	__ksymtab_mpi_buf_create, 12
__ksymtab_mpi_buf_create:
	.word	mpi_buf_create
	.word	__kstrtab_mpi_buf_create
	.word	__kstrtabns_mpi_buf_create
	.section	___ksymtab_gpl+mpi_buf_pool_add_user,"a"
	.align	2
	.type	__ksymtab_mpi_buf_pool_add_user, %object
	.size	__ksymtab_mpi_buf_pool_add_user, 12
__ksymtab_mpi_buf_pool_add_user:
	.word	mpi_buf_pool_add_user
	.word	__kstrtab_mpi_buf_pool_add_user
	.word	__kstrtabns_mpi_buf_pool_add_user
	.section	___ksymtab_gpl+mpi_buf_pool_create,"a"
	.align	2
	.type	__ksymtab_mpi_buf_pool_create, %object
	.size	__ksymtab_mpi_buf_pool_create, 12
__ksymtab_mpi_buf_pool_create:
	.word	mpi_buf_pool_create
	.word	__kstrtab_mpi_buf_pool_create
	.word	__kstrtabns_mpi_buf_pool_create
	.section	___ksymtab_gpl+mpi_buf_pool_del_user,"a"
	.align	2
	.type	__ksymtab_mpi_buf_pool_del_user, %object
	.size	__ksymtab_mpi_buf_pool_del_user, 12
__ksymtab_mpi_buf_pool_del_user:
	.word	mpi_buf_pool_del_user
	.word	__kstrtab_mpi_buf_pool_del_user
	.word	__kstrtabns_mpi_buf_pool_del_user
	.section	___ksymtab_gpl+mpi_buf_pool_destroy,"a"
	.align	2
	.type	__ksymtab_mpi_buf_pool_destroy, %object
	.size	__ksymtab_mpi_buf_pool_destroy, 12
__ksymtab_mpi_buf_pool_destroy:
	.word	mpi_buf_pool_destroy
	.word	__kstrtab_mpi_buf_pool_destroy
	.word	__kstrtabns_mpi_buf_pool_destroy
	.section	___ksymtab_gpl+mpi_buf_pool_import,"a"
	.align	2
	.type	__ksymtab_mpi_buf_pool_import, %object
	.size	__ksymtab_mpi_buf_pool_import, 12
__ksymtab_mpi_buf_pool_import:
	.word	mpi_buf_pool_import
	.word	__kstrtab_mpi_buf_pool_import
	.word	__kstrtabns_mpi_buf_pool_import
	.section	___ksymtab_gpl+mpi_buf_pool_import_dmabuf,"a"
	.align	2
	.type	__ksymtab_mpi_buf_pool_import_dmabuf, %object
	.size	__ksymtab_mpi_buf_pool_import_dmabuf, 12
__ksymtab_mpi_buf_pool_import_dmabuf:
	.word	mpi_buf_pool_import_dmabuf
	.word	__kstrtab_mpi_buf_pool_import_dmabuf
	.word	__kstrtabns_mpi_buf_pool_import_dmabuf
	.section	___ksymtab_gpl+mpi_buf_pool_request_buf,"a"
	.align	2
	.type	__ksymtab_mpi_buf_pool_request_buf, %object
	.size	__ksymtab_mpi_buf_pool_request_buf, 12
__ksymtab_mpi_buf_pool_request_buf:
	.word	mpi_buf_pool_request_buf
	.word	__kstrtab_mpi_buf_pool_request_buf
	.word	__kstrtabns_mpi_buf_pool_request_buf
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
