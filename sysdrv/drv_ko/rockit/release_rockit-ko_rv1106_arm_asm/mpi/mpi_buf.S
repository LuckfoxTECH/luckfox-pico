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
__kstrtab_mpi_buf_pool_dup:					
	.asciz 	"mpi_buf_pool_dup"					
__kstrtabns_mpi_buf_pool_dup:					
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
	ldr	r5, [r1, #444]
	cbnz	r5, .L7
	ldr	r2, [r1, #596]
	ldr	r3, [r0, #56]
	cmp	r2, r3
	bcc	.L7
	str	r0, [r6, #444]
	ldr	r3, [r0, #56]
	str	r3, [r1, #596]
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r0, #80]
	add	r7, r0, #76
	add	r3, r6, #436
	strd	r7, r2, [r6, #436]
	str	r3, [r2]
	str	r3, [r0, #80]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
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
	movs	r3, #204
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
	ldr	r5, [r0, #444]
	movs	r0, #0
	ldr	r2, .L22
	str	r3, [sp, #4]
	movs	r3, #56
	str	r3, [sp]
	ldr	r3, .L22+4
	bl	vlog
	ldr	r3, [r4, #580]
	cbz	r3, .L14
	mov	r0, r4
	blx	r3
.L14:
	ldr	r3, [r4, #12]
	orr	r3, r3, #4
	str	r3, [r4, #12]
	cbz	r5, .L15
	ldrd	r2, r3, [r4, #436]
	mov	r0, r5
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r3, #290
	str	r3, [r4, #440]
	bl	mpi_obj_unref
.L15:
	mov	r0, r4
	bl	valloc_munmap
	ldr	r1, .L22+8
	add	r0, r4, #28
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, lr}
	b	call_rcu
.L23:
	.align	2
.L22:
	.word	.LC1
	.word	.LANCHOR0+20
	.word	mpi_buf_destroy_rcu
	.fnend
	.size	mpi_buf_destroy, .-mpi_buf_destroy
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
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r0, [r0, #420]
	cbz	r0, .L25
	bl	dma_buf_put
.L25:
	sub	r0, r4, #28
	pop	{r4, lr}
	b	mpi_obj_unregister
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
	ldr	r2, .L31
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #355
	str	r3, [sp]
	ldr	r3, .L31+4
	bl	vlog
	ldr	r3, [r4, #76]
	ldr	r3, [r4, #12]
	lsls	r3, r3, #29
	bpl	.L29
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, lr}
	b	mpi_obj_unregister
.L29:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L32:
	.align	2
.L31:
	.word	.LC2
	.word	.LANCHOR0+36
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
	ldr	r2, .L34
	movs	r1, #5
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #634
	str	r3, [sp]
	ldr	r3, .L34+4
	bl	vlog
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L35:
	.align	2
.L34:
	.word	.LC3
	.word	.LANCHOR0+57
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
	cbz	r0, .L37
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L38
.L37:
	movs	r5, #0
.L36:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L38:
	bl	mpi_obj_ref
	mov	r1, r5
	ldr	r3, [r4, #52]
	mov	r2, r6
	mov	r0, r4
	blx	r3
	mov	r5, r0
	cbnz	r0, .L40
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L44
	str	r3, [sp, #4]
	movw	r3, #581
	str	r3, [sp]
	ldr	r3, .L44+4
	bl	vlog
.L40:
	mov	r0, r4
	bl	mpi_obj_unref
	b	.L36
.L45:
	.align	2
.L44:
	.word	.LC4
	.word	.LANCHOR0+80
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
@ 224 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
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
	cbz	r0, .L63
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L49
.L63:
	movs	r4, #0
.L50:
	mov	r0, r4
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L49:
	ldr	r2, [r0]
	cbz	r1, .L58
	ldr	r3, [r1]
.L51:
	ldr	r1, [r5, #68]
	ldr	r0, [r5, #16]
	strd	r2, r3, [sp, #4]
	movs	r3, #253
	ldr	r2, .L64
	strd	r1, r0, [sp, #12]
	movs	r1, #5
	str	r3, [sp]
	movs	r0, #0
	ldr	r3, .L64+4
	bl	vlog
	cbnz	r4, .L52
.L54:
	mov	r6, r5
	ldr	r4, [r6, #76]!
.L62:
	sub	r4, r4, #436
	add	r3, r4, #436
	cmp	r3, r6
	beq	.L63
	mov	r0, r4
	bl	mpi_obj_ref_if_unused
	cbnz	r0, .L55
	ldr	r4, [r4, #436]
	b	.L62
.L58:
	mov	r3, #-1
	b	.L51
.L52:
	ldr	r3, [r4, #444]
	cmp	r3, r5
	bne	.L54
	mov	r0, r4
	bl	mpi_obj_ref_if_unused
	cmp	r0, #0
	beq	.L54
.L55:
	add	r0, r5, #68
	bl	atomic_sub.constprop.3
	ldr	r3, [r5, #68]
	ldr	r2, [r5, #72]
	movs	r1, #5
	movs	r0, #0
	cmp	r2, r3
	ldr	r2, .L64+8
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
	movw	r3, #279
	str	r3, [sp]
	ldr	r3, .L64+4
	bl	vlog
	b	.L50
.L65:
	.align	2
.L64:
	.word	.LC5
	.word	.LANCHOR0+100
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
@ 223 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
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
	cbz	r0, .L68
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L69
.L68:
	mov	r5, #-1
.L67:
	mov	r0, r5
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L69:
	bl	mpi_obj_ref
	str	r7, [sp, #8]
	movs	r1, #5
	ldr	r3, [r5]
	movs	r0, #0
	ldr	r2, .L77
	str	r3, [sp, #4]
	mov	r3, #402
	str	r3, [sp]
	ldr	r3, .L77+4
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
	cbnz	r3, .L71
	add	r0, r4, #8
	bl	complete_all
.L71:
	ldr	r1, .L77+8
	add	r0, r5, #28
	bl	call_rcu
	cbnz	r7, .L72
	add	r0, r4, #8
	bl	wait_for_completion
.L72:
	mov	r0, r6
	bl	atomic_sub.constprop.3
	pld	[r4]
	.syntax unified
@ 224 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_sub_return
1:	ldrex	r5, [r4]
	sub	r5, r5, #1
	strex	r3, r5, [r4]
	teq	r3, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r5, .L73
	mov	r0, r4
	bl	kfree
	b	.L67
.L73:
	movs	r5, #0
	b	.L67
.L78:
	.align	2
.L77:
	.word	.LC7
	.word	.LANCHOR0+125
	.word	mpi_buf_pool_destroy_rcu
	.fnend
	.size	mpi_buf_pool_destroy, .-mpi_buf_pool_destroy
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
	ldr	r5, [r0, #444]
	movs	r0, #0
	ldr	r2, .L105
	str	r3, [sp, #4]
	movs	r3, #81
	str	r3, [sp]
	ldr	r3, .L105+4
	bl	vlog
	cbnz	r5, .L80
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	b	mpi_buf_destroy
.L80:
	ldr	r3, [r5, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L81
	mov	r0, r5
	bl	mpi_obj_ref
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r4
	bl	mpi_buf_destroy
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	b	mpi_obj_unref
.L81:
	add	r3, r4, #452
	add	r2, r4, #516
	movs	r1, #0
.L83:
	ldr	r0, [r3], #4
	adds	r0, r0, #1
	it	ne
	strne	r1, [r3, #-4]
	cmp	r2, r3
	bne	.L83
	add	r0, r5, #68
	add	r7, r5, #84
	bl	atomic_add.constprop.4
	ldr	r3, [r4, #12]
	orr	r3, r3, #1024
	str	r3, [r4, #12]
	ldr	r3, [r5, #84]
	cmp	r7, r3
	bne	.L84
.L92:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L84:
	ldr	r3, [r5, #84]
	movs	r6, #0
	subs	r3, r3, #64
.L86:
	add	r2, r3, #64
	cmp	r7, r2
	bne	.L88
	cbz	r6, .L89
	mov	r1, r4
	mov	r0, r6
	bl	mpi_node_buffer_available
.L89:
	ldr	r5, [r5, #84]
.L104:
	subs	r5, r5, #64
	add	r3, r5, #64
	cmp	r7, r3
	beq	.L92
	cmp	r5, r6
	beq	.L91
	ldr	r3, [r4, #16]
	cmp	r3, #0
	bne	.L92
	mov	r1, r4
	mov	r0, r5
	bl	mpi_node_buffer_available
.L91:
	ldr	r5, [r5, #64]
	b	.L104
.L88:
	cbz	r6, .L94
	ldr	r1, [r6, #72]
	ldr	r2, [r3, #72]
	cmp	r1, r2
	it	hi
	movhi	r6, r3
.L87:
	ldr	r3, [r3, #64]
	subs	r3, r3, #64
	b	.L86
.L94:
	mov	r6, r3
	b	.L87
.L106:
	.align	2
.L105:
	.word	.LC8
	.word	.LANCHOR0+146
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
	ldr	r3, .L116
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r0
	mov	r6, r1
	ldr	r0, [r3, #40]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L107
	cbz	r5, .L109
	ldr	r0, [r5, #4]
	adds	r0, r0, #28
	bl	atomic_add.constprop.4
	ldr	r3, [r5]
	str	r3, [r4, #596]
.L110:
	mov	r0, r4
	str	r6, [r4, #600]
	str	r5, [r4, #448]
	bl	valloc_mmap
	ldr	r3, .L116+4
	movs	r2, #0
	movs	r1, #4
	mov	r0, r4
	bl	mpi_obj_register
	mov	r0, r4
	bl	mpi_obj_ref
	add	r3, r4, #452
	add	r2, r4, #516
	mov	r1, #-1
.L111:
	str	r1, [r3], #4
	cmp	r3, r2
	bne	.L111
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L116+8
	movs	r0, #0
	str	r3, [sp, #4]
	movs	r3, #172
	str	r3, [sp]
	ldr	r3, .L116+12
	bl	vlog
.L107:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L109:
	str	r5, [r0, #596]
	b	.L110
.L117:
	.align	2
.L116:
	.word	kmalloc_caches
	.word	mpi_buf_release
	.word	.LC9
	.word	.LANCHOR0+162
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
	cbz	r0, .L120
	ldr	r3, [r5, #4]
	mov	r1, r0
	str	r3, [r0, #4]
	mov	r0, r5
	bl	mpi_buf_pool_import
	mov	r5, r0
	mov	r0, r4
	bl	mpi_obj_unref
.L118:
	mov	r0, r5
	pop	{r3, r4, r5, pc}
.L120:
	mov	r5, #-1
	b	.L118
	.fnend
	.size	mpi_buf_pool_import_dmabuf, .-mpi_buf_pool_import_dmabuf
	.align	1
	.global	mpi_buf_pool_dup
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_buf_pool_dup, %function
mpi_buf_pool_dup:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r5, r0
	ldr	r3, .L142
	mov	r10, r1
	ldr	r8, [r0, #16]
	mov	r1, #3520
	ldrd	r9, r7, [r0, #56]
	ldr	r0, [r3, #4]
	bl	kmem_cache_alloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L121
	add	r3, r0, #76
	movs	r6, #0
	str	r3, [r0, #80]
	movs	r2, #64
	str	r6, [r0, #68]
	movs	r1, #5
	str	r3, [r0, #76]
	add	r3, r0, #84
	strd	r9, r7, [r0, #56]
	str	r3, [r0, #84]
	str	r3, [r0, #88]
	str	r8, [r0, #72]
	ldr	r3, .L142+4
	bl	mpi_obj_register
	cmp	r7, r6
	str	r10, [r4, #4]
	bge	.L123
	cmp	r9, #0
	beq	.L131
	mov	r3, r5
	ldr	r2, [r3, #76]!
	cmp	r2, r3
	beq	.L132
	ldr	r5, [r2, #12]
	cbnz	r5, .L124
.L125:
	mov	r0, r4
	movs	r1, #0
	bl	mpi_buf_pool_destroy
	movs	r4, #0
	b	.L121
.L127:
	mov	r2, r7
	mov	r1, r5
	mov	r0, r4
	subs	r6, r6, #1
	bl	mpi_buf_pool_import_dmabuf
	cmp	r0, #0
	blt	.L125
.L126:
	cmp	r6, #0
	bne	.L127
.L130:
	ldr	r3, [r4, #16]
	movs	r1, #5
	ldr	r2, .L142+8
	movs	r0, #0
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #549
	str	r3, [sp]
	ldr	r3, .L142+12
	bl	vlog
.L121:
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L131:
	mov	r5, r9
.L124:
	mov	r6, r8
	b	.L126
.L132:
	mov	r5, r6
	b	.L124
.L123:
	ldr	r6, [r5, #76]!
	orr	r7, r7, #-2147483648
	sub	r6, r6, #436
.L128:
	add	r3, r6, #436
	cmp	r3, r5
	beq	.L130
	ldr	r1, [r6, #448]
	cmp	r1, #0
	beq	.L125
	mov	r2, r7
	mov	r0, r4
	bl	mpi_buf_pool_import_dmabuf
	cmp	r0, #0
	blt	.L125
	ldr	r6, [r6, #436]
	sub	r6, r6, #436
	b	.L128
.L143:
	.align	2
.L142:
	.word	kmalloc_caches
	.word	mpi_buf_pool_release
	.word	.LC10
	.word	.LANCHOR0+177
	.fnend
	.size	mpi_buf_pool_dup, .-mpi_buf_pool_dup
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
	ldr	r3, .L180
	mov	r9, r0
	mov	r6, r1
	mov	r1, #3520
	mov	r8, r2
	ldr	r0, [r3, #4]
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L144
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
	ldr	r3, .L180+4
	bl	mpi_obj_register
	cmp	r6, r5
	ldr	r3, [sp, #48]
	str	r3, [r4, #4]
	blt	.L146
.L148:
	bic	r10, r6, #-2147483648
.L147:
	cbnz	r7, .L155
	cmp	r6, #0
	bge	.L156
	cbz	r5, .L156
	mov	r0, r5
	bl	dma_buf_put
.L156:
	ldr	r3, [r4, #16]
	movs	r1, #5
	ldr	r2, .L180+8
	movs	r0, #0
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	mov	r3, #480
	str	r3, [sp]
	ldr	r3, .L180+12
	bl	vlog
.L144:
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L157:
	mov	r5, r8
	b	.L148
.L146:
	cmp	r8, #0
	beq	.L157
	mov	r2, r8
	mov	r1, r6
	mov	r0, r9
	bl	mpi_allocator_alloc
	mov	r5, r0
	cmp	r0, #0
	bne	.L148
.L153:
	mov	r0, r4
	movs	r1, #0
	bl	mpi_buf_pool_destroy
	movs	r4, #0
	b	.L144
.L155:
	cmp	r6, #0
	blt	.L150
	cmp	r8, #0
	beq	.L150
	mov	r2, r8
	mov	r1, r6
	mov	r0, r9
	bl	mpi_allocator_alloc
	mov	r5, r0
	cmp	r0, #0
	beq	.L153
.L150:
	ldr	r3, [r4, #72]
	mov	r1, r5
	mov	r0, r4
	cmp	r7, r3
	ite	ne
	movne	r2, r6
	moveq	r2, r10
	bl	mpi_buf_pool_import_dmabuf
	cmp	r0, #0
	bge	.L152
	cmp	r5, #0
	beq	.L153
	mov	r0, r5
	bl	dma_buf_put
	b	.L153
.L152:
	cmp	r6, #0
	blt	.L154
	cbz	r5, .L154
	mov	r0, r5
	bl	dma_buf_put
.L154:
	subs	r7, r7, #1
	b	.L147
.L181:
	.align	2
.L180:
	.word	kmalloc_caches
	.word	mpi_buf_pool_release
	.word	.LC10
	.word	.LANCHOR0+194
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
	ldr	r2, .L183
	str	r3, [sp, #12]
	ldr	r3, [r1, #44]
	movs	r1, #5
	ldr	r3, [r3, #44]
	str	r3, [sp, #8]
	ldr	r3, [r0]
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #339
	str	r3, [sp]
	ldr	r3, .L183+4
	bl	vlog
	movs	r1, #1
	mov	r0, r4
	bl	mpi_node_flush
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
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
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L184:
	.align	2
.L183:
	.word	.LC11
	.word	.LANCHOR0+214
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
.L186:
	add	r3, r1, #64
	cmp	r3, r7
	bne	.L187
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r7, r4
	ldr	r0, [r7, #48]!
	sub	r0, r0, #436
	ldr	r5, [r0, #436]
	sub	r5, r5, #436
.L188:
	add	r3, r0, #436
	cmp	r3, r7
	bne	.L190
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
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
.L187:
	mov	r0, r6
	bl	mpi_buf_pool_del_user
	ldr	r3, [r5, #64]
	mov	r1, r5
	sub	r5, r3, #64
	b	.L186
.L190:
	ldr	r3, [r0, #12]
	lsls	r3, r3, #21
	bpl	.L189
	bl	mpi_buf_destroy
.L189:
	ldr	r3, [r5, #436]
	mov	r0, r5
	sub	r5, r3, #436
	b	.L188
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
	bpl	.L194
	ldr	r3, [r1]
	ldr	r2, .L215
	str	r3, [sp, #12]
	ldr	r3, [r1, #44]
	movs	r1, #5
	ldr	r3, [r3, #44]
	str	r3, [sp, #8]
	ldr	r3, [r0]
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #303
	str	r3, [sp]
	ldr	r3, .L215+4
	bl	vlog
	ldr	r0, [r6, #60]
	cbz	r0, .L196
	mov	r1, r6
	bl	mpi_buf_pool_del_user
.L196:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
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
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cbz	r4, .L197
	ldr	r3, [r4, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L198
.L197:
.L198:
	ldr	r5, [r4, #76]!
.L214:
	sub	r5, r5, #436
	add	r3, r5, #436
	cmp	r3, r4
	bne	.L201
.L194:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L201:
	ldr	r3, [r5, #16]
	cbnz	r3, .L200
	mov	r1, r5
	mov	r0, r6
	bl	mpi_node_buffer_available
.L200:
	ldr	r5, [r5, #436]
	b	.L214
.L216:
	.align	2
.L215:
	.word	.LC12
	.word	.LANCHOR0+236
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
	ldr	r2, .L224
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #599
	str	r3, [sp]
	ldr	r3, .L224+4
	bl	vlog
	cbz	r4, .L218
	mov	r0, r4
	bl	mpi_obj_ref
.L218:
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
	cbnz	r3, .L219
	mov	r0, r7
	bl	complete_all
.L219:
	mov	r0, r4
	bl	mpi_obj_unref
	mov	r0, r7
	bl	wait_for_completion
	mov	r0, r6
	bl	atomic_sub.constprop.3
	pld	[r5]
	.syntax unified
@ 224 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_sub_return
1:	ldrex	r3, [r5]
	sub	r3, r3, #1
	strex	r2, r3, [r5]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L220
	mov	r0, r5
	bl	kfree
.L220:
	ldr	r0, [r4, #44]
	bl	kfree
	mov	r0, r4
	bl	mpi_obj_unregister
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L225:
	.align	2
.L224:
	.word	.LC13
	.word	.LANCHOR0+258
	.fnend
	.size	mpi_allocator_unregister, .-mpi_allocator_unregister
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.35921, %object
	.size	__func__.35921, 20
__func__.35921:
	.ascii	"mpi_buf_pool_import\000"
	.type	__func__.35777, %object
	.size	__func__.35777, 16
__func__.35777:
	.ascii	"mpi_buf_destroy\000"
	.type	__func__.36103, %object
	.size	__func__.36103, 21
__func__.36103:
	.ascii	"mpi_buf_pool_release\000"
	.type	__func__.36331, %object
	.size	__func__.36331, 23
__func__.36331:
	.ascii	"mpi_allocator_register\000"
	.type	__func__.36299, %object
	.size	__func__.36299, 20
__func__.36299:
	.ascii	"mpi_allocator_alloc\000"
	.type	__func__.35948, %object
	.size	__func__.35948, 25
__func__.35948:
	.ascii	"mpi_buf_pool_request_buf\000"
	.type	__func__.36185, %object
	.size	__func__.36185, 21
__func__.36185:
	.ascii	"mpi_buf_pool_destroy\000"
	.type	__func__.35791, %object
	.size	__func__.35791, 16
__func__.35791:
	.ascii	"mpi_buf_release\000"
	.type	__func__.35896, %object
	.size	__func__.35896, 15
__func__.35896:
	.ascii	"mpi_buf_create\000"
	.type	__func__.36279, %object
	.size	__func__.36279, 17
__func__.36279:
	.ascii	"mpi_buf_pool_dup\000"
	.type	__func__.36217, %object
	.size	__func__.36217, 20
__func__.36217:
	.ascii	"mpi_buf_pool_create\000"
	.type	__func__.36081, %object
	.size	__func__.36081, 22
__func__.36081:
	.ascii	"mpi_buf_pool_del_user\000"
	.type	__func__.36020, %object
	.size	__func__.36020, 22
__func__.36020:
	.ascii	"mpi_buf_pool_add_user\000"
	.type	__func__.36311, %object
	.size	__func__.36311, 25
__func__.36311:
	.ascii	"mpi_allocator_unregister\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"buf pool(%d) imported buf(%d)\000"
.LC1:
	.ascii	"buf(%d) destroying\000"
.LC2:
	.ascii	"buf pool(%d) released\000"
.LC3:
	.ascii	"allocator(%s) registered\000"
.LC4:
	.ascii	"alloc(%d) unable to alloc\000"
.LC5:
	.ascii	"buf pool(%d) request buf(%d), status: %d/%d\000"
.LC6:
	.ascii	"buf pool(%d) return buf(%d)\000"
.LC7:
	.ascii	"buf pool(%d) destroying, async: %d\000"
.LC8:
	.ascii	"buf(%d) released\000"
.LC9:
	.ascii	"buf(%d) created\000"
.LC10:
	.ascii	"buf pool(%d) created with %d buffers\000"
.LC11:
	.ascii	"buf pool(%d) del user(%s-%d)\000"
.LC12:
	.ascii	"buf pool(%d) add user(%s-%d)\000"
.LC13:
	.ascii	"allocator(%s) unregistering\000"
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
	.section	___ksymtab_gpl+mpi_buf_pool_dup,"a"
	.align	2
	.type	__ksymtab_mpi_buf_pool_dup, %object
	.size	__ksymtab_mpi_buf_pool_dup, 12
__ksymtab_mpi_buf_pool_dup:
	.word	mpi_buf_pool_dup
	.word	__kstrtab_mpi_buf_pool_dup
	.word	__kstrtabns_mpi_buf_pool_dup
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
