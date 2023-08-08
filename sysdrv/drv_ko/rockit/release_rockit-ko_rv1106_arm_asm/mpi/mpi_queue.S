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
	.file	"mpi_queue.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_queue_pop_obj:					
	.asciz 	"mpi_queue_pop_obj"					
__kstrtabns_mpi_queue_pop_obj:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_queue_push_obj:					
	.asciz 	"mpi_queue_push_obj"					
__kstrtabns_mpi_queue_push_obj:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_queue_front_obj:					
	.asciz 	"mpi_queue_front_obj"					
__kstrtabns_mpi_queue_front_obj:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_queue_flush:					
	.asciz 	"mpi_queue_flush"					
__kstrtabns_mpi_queue_flush:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_queue_destroy:					
	.asciz 	"mpi_queue_destroy"					
__kstrtabns_mpi_queue_destroy:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_queue_create:					
	.asciz 	"mpi_queue_create"					
__kstrtabns_mpi_queue_create:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_mpi_unref_rcu, %function
_mpi_unref_rcu:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	subs	r0, r0, #28
	b	mpi_obj_unref
	.fnend
	.size	_mpi_unref_rcu, .-_mpi_unref_rcu
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_cmpxchg_relaxed, %function
atomic_cmpxchg_relaxed:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	pld	[r0]
	mov	r3, r0
	push	{r4, lr}
	.save {r4, lr}
.L3:
	.syntax unified
@ 114 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_cmpxchg
ldrex	r0, [r3]
mov	r4, #0
teq	r0, r1
strexeq r4, r2, [r3]

@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r4, #0
	bne	.L3
	pop	{r4, pc}
	.fnend
	.size	atomic_cmpxchg_relaxed, .-atomic_cmpxchg_relaxed
	.align	1
	.global	mpi_queue_front_obj
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_front_obj, %function
mpi_queue_front_obj:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r2, r0
	cbz	r0, .L6
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	bne	.L6
	movs	r4, #12
.L7:
	ldr	r1, [r2, #40]
	cbz	r1, .L11
	subs	r3, r1, #1
	ldr	r0, [r2, #48]
	uxth	r3, r3
	mla	r3, r4, r3, r0
.L9:
	ldr	r0, [r3, #4]
	cbz	r0, .L10
	subs	r3, r0, #1
	ldr	r0, [r2, #48]
	uxth	r3, r3
	mla	r0, r4, r3, r0
.L10:
	ldr	r3, [r2, #40]
	cmp	r1, r3
	bne	.L7
	cbz	r0, .L5
	ldr	r0, [r0]
	b	.L5
.L6:
	movs	r0, #0
.L5:
	pop	{r4, pc}
.L11:
	mov	r3, r1
	b	.L9
	.fnend
	.size	mpi_queue_front_obj, .-mpi_queue_front_obj
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_release, %function
mpi_queue_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r0
	ldr	r3, [r0]
	movs	r1, #5
	ldr	r2, .L27
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #269
	str	r3, [sp]
	ldr	r3, .L27+4
	bl	vlog
	ldr	r3, [r4, #12]
	lsls	r2, r3, #20
	ittt	mi
	bicmi	r3, r3, #2048
	bicmi	r3, r3, #2
	strmi	r3, [r4, #12]
	ldr	r3, [r4, #12]
	lsls	r3, r3, #29
	bpl	.L21
	ldr	r0, [r4, #48]
	bl	kfree
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, lr}
	b	mpi_obj_unregister
.L21:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L28:
	.align	2
.L27:
	.word	.LC0
	.word	.LANCHOR0
	.fnend
	.size	mpi_queue_release, .-mpi_queue_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_do_try_pop_obj, %function
mpi_queue_do_try_pop_obj:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	.pad #28
	sub	sp, sp, #28
	mov	r7, r1
	cmp	r0, #0
	beq	.L37
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	bne	.L37
	mov	r8, #12
.L36:
	ldr	r6, [r4, #40]
	ldr	r1, [r4, #44]
	cbz	r6, .L41
	subs	r5, r6, #1
	ldr	r3, [r4, #48]
	uxth	r5, r5
	mla	r5, r8, r5, r3
.L33:
	cbz	r1, .L42
	subs	r0, r1, #1
	ldr	r3, [r4, #48]
	uxth	r0, r0
	mla	r0, r8, r0, r3
.L34:
	ldr	r2, [r5, #4]
	cbz	r2, .L43
	subs	r3, r2, #1
	uxth	ip, r3
	ldr	r3, [r4, #48]
	mla	r3, r8, ip, r3
.L35:
	ldr	r9, [r4, #40]
	cmp	r6, r9
	bne	.L36
	cmp	r3, #0
	beq	.L37
	cmp	r5, r0
	bne	.L38
	add	r0, r4, #44
	bl	atomic_cmpxchg_relaxed
	mov	r6, r7
.L39:
	mov	r7, r6
	b	.L36
.L41:
	mov	r5, r6
	b	.L33
.L42:
	mov	r0, r1
	b	.L34
.L43:
	mov	r3, r2
	b	.L35
.L38:
	ldr	r6, [r3]
	cbz	r7, .L40
	cmp	r7, r6
	bne	.L37
.L40:
	mov	r1, r9
	add	r0, r4, #40
	bl	atomic_cmpxchg_relaxed
	cmp	r9, r0
	bne	.L39
	add	r3, r5, #8
	movs	r7, #0
	str	r7, [r5]
	str	r7, [r5, #4]
	pld	[r3]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r2, [r3]
	sub	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r4
	bl	mpi_obj_unref
	ldr	r3, [r4, #16]
	ldr	r2, [r4, #52]
	movs	r1, #5
	mov	r0, r7
	strd	r3, r2, [sp, #12]
	ldr	r3, [r6]
	ldr	r2, .L55
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movs	r3, #107
	str	r3, [sp]
	ldr	r3, .L55+4
	bl	vlog
.L29:
	mov	r0, r6
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L37:
	movs	r6, #0
	b	.L29
.L56:
	.align	2
.L55:
	.word	.LC1
	.word	.LANCHOR0+18
	.fnend
	.size	mpi_queue_do_try_pop_obj, .-mpi_queue_do_try_pop_obj
	.align	1
	.global	mpi_queue_pop_obj
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_pop_obj, %function
mpi_queue_pop_obj:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #0
	b	mpi_queue_do_try_pop_obj
	.fnend
	.size	mpi_queue_pop_obj, .-mpi_queue_pop_obj
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_unused_node, %function
mpi_queue_unused_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r0
	movs	r5, #0
	movs	r7, #12
.L59:
	ldr	r3, [r6, #52]
	adds	r3, r3, #1
	cmp	r3, r5
	bhi	.L61
	movs	r4, #0
.L58:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L61:
	ldr	r4, [r6, #48]
	mla	r4, r7, r5, r4
	movs	r2, #1
	movs	r1, #0
	add	r0, r4, #8
	bl	atomic_cmpxchg_relaxed
	cmp	r0, #0
	beq	.L58
	adds	r5, r5, #1
	b	.L59
	.fnend
	.size	mpi_queue_unused_node, .-mpi_queue_unused_node
	.align	1
	.global	mpi_queue_push_obj
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_push_obj, %function
mpi_queue_push_obj:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r4, r0
	.pad #24
	sub	sp, sp, #24
	mov	r7, r1
	cbz	r0, .L85
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L67
.L85:
	movs	r0, #0
	b	.L68
.L67:
	bl	mpi_obj_ref
	ldr	r2, [r7, #8]
	ldr	r3, [r4, #36]
	cmp	r2, r3
	bne	.L69
	mov	r0, r4
	bl	mpi_queue_unused_node
	cmp	r0, #0
	beq	.L69
	str	r7, [r0]
	add	r3, r4, #56
	pld	[r3]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add_return
1:	ldrex	r2, [r3]
	add	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r6, [r4, #48]
	mov	r9, #12
	subs	r0, r0, r6
	ldr	r6, .L86
	asrs	r0, r0, #2
	muls	r6, r0, r6
	adds	r6, r6, #1
	orr	r6, r6, r2, lsl #16
.L73:
	ldr	r8, [r4, #44]
	cmp	r8, #0
	beq	.L76
	add	r0, r8, #-1
	ldr	r3, [r4, #48]
	uxth	r0, r0
	mla	r0, r9, r0, r3
.L71:
	ldr	r2, [r0, #4]
	cbz	r2, .L77
	subs	r5, r2, #1
	ldr	r3, [r4, #48]
	uxth	r5, r5
	mla	r5, r9, r5, r3
.L72:
	ldr	r3, [r4, #44]
	cmp	r8, r3
	bne	.L73
	add	r10, r4, #44
	cbz	r5, .L74
	mov	r1, r8
	mov	r0, r10
	bl	atomic_cmpxchg_relaxed
	b	.L73
.L76:
	mov	r0, r8
	b	.L71
.L77:
	mov	r5, r2
	b	.L72
.L74:
	mov	r2, r6
	mov	r1, r5
	adds	r0, r0, #4
	bl	atomic_cmpxchg_relaxed
	cmp	r0, #0
	bne	.L73
	mov	r1, r8
	mov	r0, r10
	bl	atomic_cmpxchg_relaxed
	ldr	r3, [r4, #16]
	movs	r1, #5
	ldr	r2, [r4, #52]
	mov	r0, r5
	strd	r3, r2, [sp, #12]
	ldr	r3, [r7]
	ldr	r2, .L86+4
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movs	r3, #182
	str	r3, [sp]
	ldr	r3, .L86+8
	bl	vlog
	movs	r0, #1
.L68:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L69:
	mov	r0, r4
	bl	mpi_obj_unref
	b	.L85
.L87:
	.align	2
.L86:
	.word	-1431655765
	.word	.LC2
	.word	.LANCHOR0+43
	.fnend
	.size	mpi_queue_push_obj, .-mpi_queue_push_obj
	.align	1
	.global	mpi_queue_flush
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_flush, %function
mpi_queue_flush:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r1
	mov	r7, r2
	mov	r5, r0
	cbz	r0, .L89
	bl	mpi_obj_ref
.L89:
	ldr	r3, [r5]
	movs	r1, #5
	ldr	r2, .L105
	movs	r0, #0
	str	r3, [sp, #4]
	movs	r3, #239
	str	r3, [sp]
	ldr	r3, .L105+4
	bl	vlog
	ldr	r3, [r5, #12]
	and	r8, r3, #2048
	orr	r3, r3, #2048
	str	r3, [r5, #12]
	cmp	r4, #0
	bne	.L90
.L94:
	ldr	r3, [r5, #12]
	lsls	r2, r3, #21
	bmi	.L91
.L92:
	ldr	r4, [r5, #24]
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
	cbnz	r3, .L96
	add	r0, r4, #8
	bl	complete_all
.L96:
	cmp	r8, #0
	beq	.L97
	mov	r0, r5
	bl	mpi_obj_unref
.L98:
	cbnz	r7, .L99
	add	r0, r4, #8
	bl	wait_for_completion
.L99:
	pld	[r6]
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
	cbnz	r3, .L100
	mov	r0, r4
	bl	kfree
.L100:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L90:
	ldr	r3, [r5, #60]
	cmp	r3, #0
	beq	.L94
.L93:
	mov	r0, r5
	bl	mpi_queue_pop_obj
	cmp	r0, #0
	beq	.L94
	ldr	r3, [r5, #60]
	blx	r3
	b	.L93
.L91:
	orr	r3, r3, #4
	str	r3, [r5, #12]
	b	.L92
.L97:
	ldr	r1, .L105+8
	add	r0, r5, #28
	bl	call_rcu
	b	.L98
.L106:
	.align	2
.L105:
	.word	.LC3
	.word	.LANCHOR0+62
	.word	_mpi_unref_rcu
	.fnend
	.size	mpi_queue_flush, .-mpi_queue_flush
	.align	1
	.global	mpi_queue_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_destroy, %function
mpi_queue_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	ldr	r3, [r0]
	mov	r5, r1
	ldr	r2, .L108
	movs	r1, #5
	movs	r0, #0
	str	r3, [sp, #4]
	movw	r3, #293
	str	r3, [sp]
	ldr	r3, .L108+4
	bl	vlog
	ldr	r3, [r4, #12]
	mov	r2, r5
	movs	r1, #1
	orr	r3, r3, #1024
	mov	r0, r4
	str	r3, [r4, #12]
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, lr}
	b	mpi_queue_flush
.L109:
	.align	2
.L108:
	.word	.LC4
	.word	.LANCHOR0+78
	.fnend
	.size	mpi_queue_destroy, .-mpi_queue_destroy
	.align	1
	.global	mpi_queue_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_create, %function
mpi_queue_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r8, r2
	subs	r2, r0, #1
	movw	r3, #65533
	cmp	r2, r3
	.pad #20
	sub	sp, sp, #20
	mov	r6, r0
	mov	r9, r1
	bls	.L111
.L113:
	movs	r4, #0
.L110:
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L111:
	ldr	r3, .L118
	mov	r1, #3520
	ldr	r0, [r3, #24]
	bl	kmem_cache_alloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L113
	movs	r0, #12
	mov	r1, #3520
	mla	r0, r6, r0, r0
	bl	__kmalloc
	movs	r5, #0
	str	r0, [r4, #48]
	cbnz	r0, .L114
	mov	r0, r4
	bl	kfree
	b	.L113
.L114:
	str	r5, [r4, #56]
	mov	r0, r4
	str	r6, [r4, #52]
	str	r9, [r4, #36]
	str	r8, [r4, #60]
	bl	mpi_queue_unused_node
	add	r2, r4, #56
	pld	[r2]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add_return
1:	ldrex	r1, [r2]
	add	r1, r1, #1
	strex	r3, r1, [r2]
	teq	r3, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #48]
	movs	r2, #64
	subs	r3, r0, r3
	asrs	r0, r3, #2
	ldr	r3, .L118+4
	muls	r3, r0, r3
	mov	r0, r4
	adds	r3, r3, #1
	orr	r3, r3, r1, lsl #16
	str	r3, [r4, #40]
	ldr	r3, [r4, #40]
	movs	r1, #6
	str	r3, [r4, #44]
	ldr	r3, .L118+8
	bl	mpi_obj_register
	ldr	r3, [r4, #52]
	ldr	r2, .L118+12
	movs	r1, #5
	mov	r0, r5
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #341
	str	r3, [sp]
	ldr	r3, .L118+16
	bl	vlog
	b	.L110
.L119:
	.align	2
.L118:
	.word	kmalloc_caches
	.word	-1431655765
	.word	mpi_queue_release
	.word	.LC5
	.word	.LANCHOR0+96
	.fnend
	.size	mpi_queue_create, .-mpi_queue_create
	.align	1
	.global	mpi_queue_try_pop_obj
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_queue_try_pop_obj, %function
mpi_queue_try_pop_obj:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r1
	bl	mpi_queue_do_try_pop_obj
	subs	r3, r4, r0
	rsbs	r0, r3, #0
	adcs	r0, r0, r3
	pop	{r4, pc}
	.fnend
	.size	mpi_queue_try_pop_obj, .-mpi_queue_try_pop_obj
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.36033, %object
	.size	__func__.36033, 18
__func__.36033:
	.ascii	"mpi_queue_release\000"
	.type	__func__.35909, %object
	.size	__func__.35909, 25
__func__.35909:
	.ascii	"mpi_queue_do_try_pop_obj\000"
	.type	__func__.35957, %object
	.size	__func__.35957, 19
__func__.35957:
	.ascii	"mpi_queue_push_obj\000"
	.type	__func__.36005, %object
	.size	__func__.36005, 16
__func__.36005:
	.ascii	"mpi_queue_flush\000"
	.type	__func__.36038, %object
	.size	__func__.36038, 18
__func__.36038:
	.ascii	"mpi_queue_destroy\000"
	.type	__func__.36066, %object
	.size	__func__.36066, 17
__func__.36066:
	.ascii	"mpi_queue_create\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"%s,%d:queue(%d) released\000"
.LC1:
	.ascii	"%s,%d:queue(%d) pop obj: %d, status: %d/%d\000"
.LC2:
	.ascii	"%s,%d:queue(%d) push obj: %d, status: %d/%d\000"
.LC3:
	.ascii	"%s,%d:queue(%d) flushing\000"
.LC4:
	.ascii	"%s,%d:queue(%d) destroying\000"
.LC5:
	.ascii	"%s,%d:queue(%d) inited for %d nodes\000"
	.section	___ksymtab_gpl+mpi_queue_create,"a"
	.align	2
	.type	__ksymtab_mpi_queue_create, %object
	.size	__ksymtab_mpi_queue_create, 12
__ksymtab_mpi_queue_create:
	.word	mpi_queue_create
	.word	__kstrtab_mpi_queue_create
	.word	__kstrtabns_mpi_queue_create
	.section	___ksymtab_gpl+mpi_queue_destroy,"a"
	.align	2
	.type	__ksymtab_mpi_queue_destroy, %object
	.size	__ksymtab_mpi_queue_destroy, 12
__ksymtab_mpi_queue_destroy:
	.word	mpi_queue_destroy
	.word	__kstrtab_mpi_queue_destroy
	.word	__kstrtabns_mpi_queue_destroy
	.section	___ksymtab_gpl+mpi_queue_flush,"a"
	.align	2
	.type	__ksymtab_mpi_queue_flush, %object
	.size	__ksymtab_mpi_queue_flush, 12
__ksymtab_mpi_queue_flush:
	.word	mpi_queue_flush
	.word	__kstrtab_mpi_queue_flush
	.word	__kstrtabns_mpi_queue_flush
	.section	___ksymtab_gpl+mpi_queue_front_obj,"a"
	.align	2
	.type	__ksymtab_mpi_queue_front_obj, %object
	.size	__ksymtab_mpi_queue_front_obj, 12
__ksymtab_mpi_queue_front_obj:
	.word	mpi_queue_front_obj
	.word	__kstrtab_mpi_queue_front_obj
	.word	__kstrtabns_mpi_queue_front_obj
	.section	___ksymtab_gpl+mpi_queue_pop_obj,"a"
	.align	2
	.type	__ksymtab_mpi_queue_pop_obj, %object
	.size	__ksymtab_mpi_queue_pop_obj, 12
__ksymtab_mpi_queue_pop_obj:
	.word	mpi_queue_pop_obj
	.word	__kstrtab_mpi_queue_pop_obj
	.word	__kstrtabns_mpi_queue_pop_obj
	.section	___ksymtab_gpl+mpi_queue_push_obj,"a"
	.align	2
	.type	__ksymtab_mpi_queue_push_obj, %object
	.size	__ksymtab_mpi_queue_push_obj, 12
__ksymtab_mpi_queue_push_obj:
	.word	mpi_queue_push_obj
	.word	__kstrtab_mpi_queue_push_obj
	.word	__kstrtabns_mpi_queue_push_obj
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
