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
	.file	"mpi_job.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_job_set_ready:					
	.asciz 	"mpi_job_set_ready"					
__kstrtabns_mpi_job_set_ready:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_job_cancel:					
	.asciz 	"mpi_job_cancel"					
__kstrtabns_mpi_job_cancel:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_job_done:					
	.asciz 	"mpi_job_done"					
__kstrtabns_mpi_job_done:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_job_wait:					
	.asciz 	"mpi_job_wait"					
__kstrtabns_mpi_job_wait:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_job_pool_flush:					
	.asciz 	"mpi_job_pool_flush"					
__kstrtabns_mpi_job_pool_flush:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_set_if, %function
atomic_set_if:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	pld	[r0]
.L2:
	.syntax unified
@ 114 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_cmpxchg
ldrex	r3, [r0]
mov	r4, #0
teq	r3, r2
strexeq r4, r1, [r0]

@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r4, #0
	bne	.L2
	subs	r3, r2, r3
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	pop	{r4, pc}
	.fnend
	.size	atomic_set_if, .-atomic_set_if
	.align	1
	.global	mpi_job_set_ready
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_set_ready, %function
mpi_job_set_ready:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	mov	r4, r0
	movs	r2, #1
	movs	r1, #2
	adds	r0, r0, #68
	bl	atomic_set_if
	cbz	r0, .L4
	ldr	r3, [r4, #44]
	movs	r1, #5
	ldr	r2, .L9
	movs	r0, #0
	ldr	r3, [r3, #44]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movs	r3, #35
	str	r3, [sp]
	ldr	r3, .L9+4
	bl	vlog
.L4:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L10:
	.align	2
.L9:
	.word	.LC0
	.word	.LANCHOR0
	.fnend
	.size	mpi_job_set_ready, .-mpi_job_set_ready
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_release, %function
mpi_job_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	ldr	r3, [r0, #68]
	movs	r5, #0
	ldr	r0, [r0, #44]
	bl	mpi_obj_unref
	ldr	r0, [r4, #48]
	str	r5, [r4, #44]
	bl	mpi_obj_unref
	ldr	r0, [r4, #60]
	str	r5, [r4, #48]
	bl	mpi_obj_unref
	ldr	r0, [r4, #64]
	str	r5, [r4, #60]
	bl	mpi_obj_unref
	ldr	r3, [r4, #12]
	str	r5, [r4, #64]
	movs	r1, #5
	bic	r3, r3, #1024
	str	r3, [r4, #12]
	ldr	r3, [r4]
	mov	r0, r5
	str	r5, [r4, #68]
	ldr	r2, .L12
	str	r3, [sp, #4]
	movs	r3, #183
	str	r3, [sp]
	ldr	r3, .L12+4
	bl	vlog
	ldr	r0, [r4, #52]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, lr}
	b	mpi_obj_unref
.L13:
	.align	2
.L12:
	.word	.LC1
	.word	.LANCHOR0+18
	.fnend
	.size	mpi_job_release, .-mpi_job_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_try_complete, %function
mpi_job_try_complete:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #68]
	lsls	r2, r2, #21
	bpl	.L14
	subs	r3, r3, #4
	cmp	r3, #2
	bhi	.L14
	adds	r0, r0, #72
	b	complete_all
.L14:
	bx	lr
	.fnend
	.size	mpi_job_try_complete, .-mpi_job_try_complete
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_do_cancel, %function
mpi_job_do_cancel:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	movs	r2, #2
	mov	r4, r0
	movs	r1, #4
	adds	r0, r0, #68
	bl	atomic_set_if
	cbz	r0, .L21
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L22
	movs	r0, #0
	str	r3, [sp, #4]
	movs	r3, #69
	str	r3, [sp]
	ldr	r3, .L22+4
	bl	vlog
	mov	r0, r4
	bl	mpi_job_try_complete
	mov	r0, r4
	bl	mpi_obj_unref
	movs	r0, #0
.L19:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L21:
	mov	r0, #-1
	b	.L19
.L23:
	.align	2
.L22:
	.word	.LC2
	.word	.LANCHOR0+34
	.fnend
	.size	mpi_job_do_cancel, .-mpi_job_do_cancel
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_cancel_rcu, %function
mpi_job_cancel_rcu:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	subs	r0, r0, #28
	b	mpi_job_do_cancel
	.fnend
	.size	mpi_job_cancel_rcu, .-mpi_job_cancel_rcu
	.align	1
	.global	mpi_job_done
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_done, %function
mpi_job_done:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	subs	r6, r1, #0
	mov	r4, r0
	mov	r2, #3
	ite	eq
	moveq	r5, #6
	movne	r5, #5
	mov	r1, r5
	adds	r0, r0, #68
	bl	atomic_set_if
	cmp	r0, #0
	beq	.L36
	cmp	r5, #5
	mov	r5, #0
	bne	.L28
	ldr	r0, [r4, #64]
	bl	mpi_obj_unref
	str	r5, [r4, #64]
	ldr	r0, [r4, #60]
	bl	mpi_obj_unref
	str	r5, [r4, #60]
.L29:
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L45
	movs	r0, #0
	str	r6, [sp, #8]
	str	r3, [sp, #4]
	movs	r3, #151
	str	r3, [sp]
	ldr	r3, .L45+4
	bl	vlog
	mov	r0, r4
	bl	mpi_job_try_complete
	mov	r0, r4
	bl	mpi_obj_unref
	movs	r0, #0
.L25:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L28:
	ldr	r0, [r4, #60]
	bl	mpi_obj_unref
	str	r5, [r4, #60]
	cbz	r4, .L44
	ldr	r3, [r4, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L31
.L44:
	b	.L29
.L31:
	ldr	r0, [r4, #48]
	cbz	r0, .L43
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L33
.L43:
	b	.L44
.L33:
	mov	r2, r5
	ldr	r1, [r4, #64]
	bl	mpi_node_buffer_out
	b	.L43
.L36:
	mov	r0, #-1
	b	.L25
.L46:
	.align	2
.L45:
	.word	.LC3
	.word	.LANCHOR0+52
	.fnend
	.size	mpi_job_done, .-mpi_job_done
	.align	1
	.global	mpi_job_wait
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_wait, %function
mpi_job_wait:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r3, [r0, #16]
	cbz	r0, .L48
	bl	mpi_obj_ref
.L48:
	ldr	r3, [r4, #12]
	mov	r0, r4
	orr	r3, r3, #1024
	str	r3, [r4, #12]
	bl	mpi_job_try_complete
	add	r0, r4, #72
	bl	wait_for_completion
	mov	r0, r4
	bl	mpi_obj_unref
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	mpi_job_wait, .-mpi_job_wait
	.align	1
	.global	mpi_job_cancel
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_cancel, %function
mpi_job_cancel:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r3, [r0, #12]
	mov	r6, r1
	lsls	r2, r3, #30
	bpl	.L55
	bic	r3, r3, #2
	str	r3, [r0, #12]
	ldr	r0, [r0, #60]
	movs	r5, #0
	bl	mpi_obj_unref
	str	r5, [r4, #60]
	ldr	r0, [r4, #64]
	bl	mpi_obj_unref
	str	r5, [r4, #64]
	cbz	r6, .L54
	add	r0, r4, #28
	ldr	r1, .L56
	bl	call_rcu
	mov	r0, r5
.L52:
	pop	{r4, r5, r6, pc}
.L54:
	bl	synchronize_rcu
	mov	r0, r4
	pop	{r4, r5, r6, lr}
	b	mpi_job_do_cancel
.L55:
	mov	r0, #-1
	b	.L52
.L57:
	.align	2
.L56:
	.word	mpi_job_cancel_rcu
	.fnend
	.size	mpi_job_cancel, .-mpi_job_cancel
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_pool_release, %function
mpi_job_pool_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r0
	ldr	r3, [r0]
	movs	r1, #5
	ldr	r2, .L62
	movs	r0, #0
	str	r3, [sp, #4]
	mov	r3, #298
	str	r3, [sp]
	ldr	r3, .L62+4
	bl	vlog
	ldr	r3, [r4, #12]
	bic	r2, r3, #2048
	lsls	r3, r3, #29
	str	r2, [r4, #12]
	bpl	.L58
	mov	r6, r4
	ldr	r0, [r6, #40]!
	ldr	r5, [r0], #-36
	subs	r5, r5, #36
.L60:
	add	r3, r0, #36
	cmp	r3, r6
	bne	.L61
	ldr	r0, [r4, #36]
	bl	mpi_obj_unref
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	mpi_obj_unregister
.L61:
	ldr	r3, [r0, #68]
	bl	mpi_obj_unregister
	ldr	r3, [r5, #36]
	mov	r0, r5
	sub	r5, r3, #36
	b	.L60
.L58:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L63:
	.align	2
.L62:
	.word	.LC4
	.word	.LANCHOR0+65
	.fnend
	.size	mpi_job_pool_release, .-mpi_job_pool_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_pool_alloc, %function
mpi_job_pool_alloc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L67
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r0
	.pad #20
	sub	sp, sp, #20
	mov	r7, r1
	ldr	r0, [r3, #28]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbnz	r0, .L65
	ldr	r3, [r5]
	movs	r1, #5
	ldr	r2, .L67+4
	str	r3, [sp, #4]
	movs	r3, #215
	str	r3, [sp]
	ldr	r3, .L67+8
	bl	vlog
.L64:
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L65:
	movs	r6, #0
	ldr	r2, .L67+12
	str	r6, [r0, #72]
	adds	r0, r0, #76
	ldr	r1, .L67+16
	bl	__init_swait_queue_head
	str	r7, [r4, #68]
	str	r5, [r4, #52]
	mov	r2, r6
	ldr	r3, .L67+20
	movs	r1, #2
	mov	r0, r4
	bl	mpi_obj_register
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r5, #44]
	add	r0, r5, #40
	add	r3, r4, #36
	strd	r0, r2, [r4, #36]
	str	r3, [r2]
	str	r3, [r5, #44]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L67+24
	mov	r0, r6
	str	r3, [sp, #8]
	ldr	r3, [r5]
	str	r3, [sp, #4]
	movs	r3, #229
	str	r3, [sp]
	ldr	r3, .L67+8
	bl	vlog
	b	.L64
.L68:
	.align	2
.L67:
	.word	kmalloc_caches
	.word	.LC5
	.word	.LANCHOR0+86
	.word	.LANCHOR1
	.word	.LC6
	.word	mpi_job_release
	.word	.LC7
	.fnend
	.size	mpi_job_pool_alloc, .-mpi_job_pool_alloc
	.align	1
	.global	mpi_job_pool_flush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_pool_flush, %function
mpi_job_pool_flush:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r5, r0
	ldr	r3, [r0]
	mov	r7, r1
	mov	r4, r2
	movs	r1, #5
	ldr	r2, .L96
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #347
	str	r3, [sp]
	ldr	r3, .L96+4
	bl	vlog
	cbz	r5, .L70
	mov	r0, r5
	bl	mpi_obj_ref
.L70:
	ldr	r3, [r5, #12]
	orr	r3, r3, #2048
	str	r3, [r5, #12]
	bl	synchronize_rcu
	cbz	r4, .L71
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r6, r5
	ldr	r4, [r6, #40]!
.L95:
	subs	r4, r4, #36
	add	r3, r4, #36
	cmp	r3, r6
	bne	.L76
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L71:
	ldr	r3, [r5, #12]
	ldr	r4, [r5, #24]
	lsls	r2, r3, #21
	itt	mi
	orrmi	r3, r3, #4
	strmi	r3, [r5, #12]
	pld	[r4]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r3, [r4]
	add	r3, r3, #1
	strex	r2, r3, [r4]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	adds	r6, r4, #4
	pld	[r6]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r3, [r6]
	add	r3, r3, #1
	strex	r2, r3, [r6]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #16]
	add	r7, r4, #8
	cbnz	r3, .L78
	mov	r0, r7
	bl	complete_all
.L78:
	mov	r0, r5
	pld	[r6]
	bl	mpi_obj_unref
	mov	r0, r7
	bl	wait_for_completion
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r3, [r6]
	sub	r3, r3, #1
	strex	r2, r3, [r6]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	pld	[r4]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub_return
1:	ldrex	r3, [r4]
	sub	r3, r3, #1
	strex	r2, r3, [r4]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L79
	mov	r0, r4
	bl	kfree
.L79:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L76:
	mov	r0, r4
	bl	mpi_obj_ref_if_used
	cbz	r0, .L73
	cbz	r7, .L74
	ldr	r3, [r4, #48]
	cmp	r7, r3
	bne	.L75
.L74:
	movs	r1, #0
	mov	r0, r4
	bl	mpi_job_cancel
.L75:
	mov	r0, r4
	bl	mpi_obj_unref
.L73:
	ldr	r4, [r4, #36]
	b	.L95
.L97:
	.align	2
.L96:
	.word	.LC8
	.word	.LANCHOR0+105
	.fnend
	.size	mpi_job_pool_flush, .-mpi_job_pool_flush
	.align	1
	.global	mpi_job_set_running
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_set_running, %function
mpi_job_set_running:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	mov	r4, r0
	movs	r2, #2
	ldr	r3, [r0, #68]!
	movs	r1, #3
	bl	atomic_set_if
	cbz	r0, .L100
	ldr	r3, [r4, #44]
	movs	r1, #5
	ldr	r2, .L101
	movs	r0, #0
	ldr	r3, [r3, #44]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movs	r3, #48
	str	r3, [sp]
	ldr	r3, .L101+4
	bl	vlog
	movs	r0, #0
.L98:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L100:
	mov	r0, #-1
	b	.L98
.L102:
	.align	2
.L101:
	.word	.LC9
	.word	.LANCHOR0+124
	.fnend
	.size	mpi_job_set_running, .-mpi_job_set_running
	.align	1
	.global	mpi_job_pool_request_job
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_pool_request_job, %function
mpi_job_pool_request_job:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r5, r0
	cbz	r0, .L104
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L105
.L104:
	movs	r4, #0
.L103:
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L105:
	ldr	r3, [r0, #16]
	movs	r1, #5
	ldr	r2, .L124
	str	r3, [sp, #8]
	ldr	r3, [r0]
	movs	r0, #0
	str	r3, [sp, #4]
	movs	r3, #241
	str	r3, [sp]
	ldr	r3, .L124+4
	bl	vlog
	ldr	r3, [r5, #12]
	lsls	r3, r3, #20
	bpl	.L107
.L112:
	movs	r4, #0
.L108:
	b	.L103
.L107:
	mov	r6, r5
	ldr	r4, [r6, #40]!
.L123:
	subs	r4, r4, #36
	add	r3, r4, #36
	cmp	r3, r6
	bne	.L111
	movs	r1, #1
	mov	r0, r5
	bl	mpi_job_pool_alloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L112
.L110:
	mov	r0, r5
	bl	mpi_obj_ref
	movs	r3, #0
	str	r3, [r4, #72]
	ldr	r3, [r4, #12]
	orr	r3, r3, #2
	str	r3, [r4, #12]
	cbz	r4, .L113
	mov	r0, r4
	bl	mpi_obj_ref
.L113:
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L124+8
	movs	r0, #0
	str	r3, [sp, #8]
	ldr	r3, [r5]
	str	r3, [sp, #4]
	mov	r3, #262
	str	r3, [sp]
	ldr	r3, .L124+4
	bl	vlog
	b	.L108
.L111:
	movs	r2, #0
	movs	r1, #1
	add	r0, r4, #68
	bl	atomic_set_if
	cmp	r0, #0
	bne	.L110
	ldr	r4, [r4, #36]
	b	.L123
.L125:
	.align	2
.L124:
	.word	.LC10
	.word	.LANCHOR0+144
	.word	.LC11
	.fnend
	.size	mpi_job_pool_request_job, .-mpi_job_pool_request_job
	.align	1
	.global	mpi_job_pool_choose_job
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_pool_choose_job, %function
mpi_job_pool_choose_job:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r7, r1
	mov	r8, r2
	cbz	r0, .L127
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L128
.L127:
	movs	r5, #0
.L126:
	mov	r0, r5
	pop	{r4, r5, r6, r7, r8, pc}
.L128:
	mov	r6, r0
	movs	r5, #0
	ldr	r4, [r6, #40]!
	subs	r4, r4, #36
.L130:
	add	r3, r4, #36
	cmp	r3, r6
	bne	.L132
	b	.L126
.L132:
	ldr	r3, [r4, #12]
	lsls	r3, r3, #30
	bpl	.L131
	ldr	r3, [r4, #48]
	cmp	r3, r7
	bne	.L131
	ldr	r3, [r4, #68]
	cmp	r3, #2
	bne	.L131
	cbz	r5, .L133
	cmp	r8, #0
	beq	.L131
	mov	r0, r5
	mov	r1, r4
	blx	r8
	bics	r5, r5, r0, asr #32
	it	cs
	movcs	r5, r4
.L131:
	ldr	r4, [r4, #36]
	subs	r4, r4, #36
	b	.L130
.L133:
	mov	r5, r4
	b	.L131
	.fnend
	.size	mpi_job_pool_choose_job, .-mpi_job_pool_choose_job
	.align	1
	.global	mpi_job_pool_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_pool_destroy, %function
mpi_job_pool_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r0
	ldr	r3, [r0]
	movs	r1, #5
	ldr	r2, .L144
	movs	r0, #0
	str	r3, [sp, #4]
	mov	r3, #370
	str	r3, [sp]
	ldr	r3, .L144+4
	bl	vlog
	ldr	r3, [r4, #12]
	movs	r2, #1
	movs	r1, #0
	orr	r3, r3, #1024
	mov	r0, r4
	str	r3, [r4, #12]
	bl	mpi_job_pool_flush
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L145:
	.align	2
.L144:
	.word	.LC12
	.word	.LANCHOR0+169
	.fnend
	.size	mpi_job_pool_destroy, .-mpi_job_pool_destroy
	.align	1
	.global	mpi_job_pool_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_job_pool_create, %function
mpi_job_pool_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L157
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r6, r0
	mov	r5, r1
	ldr	r0, [r3, #24]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L146
	cbz	r6, .L148
	mov	r0, r6
	bl	mpi_obj_ref
.L148:
	add	r3, r4, #40
	str	r6, [r4, #36]
	str	r3, [r4, #40]
	movs	r2, #64
	str	r3, [r4, #44]
	movs	r1, #3
	ldr	r3, .L157+4
	mov	r0, r4
	bl	mpi_obj_register
.L149:
	adds	r5, r5, #-1
	bcs	.L150
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L157+8
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #397
	str	r3, [sp]
	ldr	r3, .L157+12
	bl	vlog
.L146:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L150:
	movs	r1, #0
	mov	r0, r4
	bl	mpi_job_pool_alloc
	b	.L149
.L158:
	.align	2
.L157:
	.word	kmalloc_caches
	.word	mpi_job_pool_release
	.word	.LC13
	.word	.LANCHOR0+190
	.fnend
	.size	mpi_job_pool_create, .-mpi_job_pool_create
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.36167, %object
	.size	__func__.36167, 18
__func__.36167:
	.ascii	"mpi_job_set_ready\000"
	.type	__func__.36268, %object
	.size	__func__.36268, 16
__func__.36268:
	.ascii	"mpi_job_release\000"
	.type	__func__.36195, %object
	.size	__func__.36195, 18
__func__.36195:
	.ascii	"mpi_job_do_cancel\000"
	.type	__func__.36242, %object
	.size	__func__.36242, 13
__func__.36242:
	.ascii	"mpi_job_done\000"
	.type	__func__.36416, %object
	.size	__func__.36416, 21
__func__.36416:
	.ascii	"mpi_job_pool_release\000"
	.type	__func__.36290, %object
	.size	__func__.36290, 19
__func__.36290:
	.ascii	"mpi_job_pool_alloc\000"
	.type	__func__.36473, %object
	.size	__func__.36473, 19
__func__.36473:
	.ascii	"mpi_job_pool_flush\000"
	.type	__func__.36183, %object
	.size	__func__.36183, 20
__func__.36183:
	.ascii	"mpi_job_set_running\000"
	.type	__func__.36306, %object
	.size	__func__.36306, 25
__func__.36306:
	.ascii	"mpi_job_pool_request_job\000"
	.type	__func__.36493, %object
	.size	__func__.36493, 21
__func__.36493:
	.ascii	"mpi_job_pool_destroy\000"
	.type	__func__.36504, %object
	.size	__func__.36504, 20
__func__.36504:
	.ascii	"mpi_job_pool_create\000"
	.bss
	.set	.LANCHOR1,. + 0
	.type	__key.10560, %object
	.size	__key.10560, 0
__key.10560:
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"%s,%d:job(%d) ready for dev(%s)\000"
.LC1:
	.ascii	"%s,%d:job(%d) release\000"
.LC2:
	.ascii	"%s,%d:job(%d) canceled\000"
.LC3:
	.ascii	"%s,%d:job(%d) done(%d)\000"
.LC4:
	.ascii	"%s,%d:job pool(%d) released\000"
.LC5:
	.ascii	"%s,%d:job pool(%d) failed to alloc job\000"
.LC6:
	.ascii	"&x->wait\000"
.LC7:
	.ascii	"%s,%d:job pool(%d) allocated job(%d)\000"
.LC8:
	.ascii	"%s,%d:job pool(%d) flushing\000"
.LC9:
	.ascii	"%s,%d:job(%d) run for dev(%s)\000"
.LC10:
	.ascii	"%s,%d:job pool(%d) request job, status: %d\000"
.LC11:
	.ascii	"%s,%d:job pool(%d) return job(%d)\000"
.LC12:
	.ascii	"%s,%d:job pool(%d) destroying\000"
.LC13:
	.ascii	"%s,%d:job pool(%d) registered\000"
	.section	___ksymtab_gpl+mpi_job_cancel,"a"
	.align	2
	.type	__ksymtab_mpi_job_cancel, %object
	.size	__ksymtab_mpi_job_cancel, 12
__ksymtab_mpi_job_cancel:
	.word	mpi_job_cancel
	.word	__kstrtab_mpi_job_cancel
	.word	__kstrtabns_mpi_job_cancel
	.section	___ksymtab_gpl+mpi_job_done,"a"
	.align	2
	.type	__ksymtab_mpi_job_done, %object
	.size	__ksymtab_mpi_job_done, 12
__ksymtab_mpi_job_done:
	.word	mpi_job_done
	.word	__kstrtab_mpi_job_done
	.word	__kstrtabns_mpi_job_done
	.section	___ksymtab_gpl+mpi_job_pool_flush,"a"
	.align	2
	.type	__ksymtab_mpi_job_pool_flush, %object
	.size	__ksymtab_mpi_job_pool_flush, 12
__ksymtab_mpi_job_pool_flush:
	.word	mpi_job_pool_flush
	.word	__kstrtab_mpi_job_pool_flush
	.word	__kstrtabns_mpi_job_pool_flush
	.section	___ksymtab_gpl+mpi_job_set_ready,"a"
	.align	2
	.type	__ksymtab_mpi_job_set_ready, %object
	.size	__ksymtab_mpi_job_set_ready, 12
__ksymtab_mpi_job_set_ready:
	.word	mpi_job_set_ready
	.word	__kstrtab_mpi_job_set_ready
	.word	__kstrtabns_mpi_job_set_ready
	.section	___ksymtab_gpl+mpi_job_wait,"a"
	.align	2
	.type	__ksymtab_mpi_job_wait, %object
	.size	__ksymtab_mpi_job_wait, 12
__ksymtab_mpi_job_wait:
	.word	mpi_job_wait
	.word	__kstrtab_mpi_job_wait
	.word	__kstrtabns_mpi_job_wait
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
