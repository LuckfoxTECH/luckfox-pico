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
	.file	"mpi.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_dev_list:					
	.asciz 	"mpi_dev_list"					
__kstrtabns_mpi_dev_list:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_node_list:					
	.asciz 	"mpi_node_list"					
__kstrtabns_mpi_node_list:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_list:					
	.asciz 	"mpi_buf_list"					
__kstrtabns_mpi_buf_list:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_buf_pool_list:					
	.asciz 	"mpi_buf_pool_list"					
__kstrtabns_mpi_buf_pool_list:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_alloc_list:					
	.asciz 	"mpi_alloc_list"					
__kstrtabns_mpi_alloc_list:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_obj_ref:					
	.asciz 	"mpi_obj_ref"					
__kstrtabns_mpi_obj_ref:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_obj_unref:					
	.asciz 	"mpi_obj_unref"					
__kstrtabns_mpi_obj_unref:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_obj_from_id:					
	.asciz 	"mpi_obj_from_id"					
__kstrtabns_mpi_obj_from_id:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_dev_find_by_name:					
	.asciz 	"mpi_dev_find_by_name"					
__kstrtabns_mpi_dev_find_by_name:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_allocator_find_by_name:					
	.asciz 	"mpi_allocator_find_by_name"					
__kstrtabns_mpi_allocator_find_by_name:					
	.asciz 	""					
	.previous						

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
	.type	mpi_obj_referenced, %function
mpi_obj_referenced:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r5, [r0, #16]
	mov	r4, r0
	ldr	r3, .L7
	movs	r1, #5
	subs	r2, r5, #1
	str	r2, [sp, #12]
	ldr	r2, [r0]
	str	r2, [sp, #8]
	ldr	r2, [r0, #8]
	movs	r0, #0
	add	r2, r3, r2, lsl #2
	ldr	r2, [r2, #20]
	str	r2, [sp, #4]
	movs	r2, #184
	str	r2, [sp]
	ldr	r2, .L7+4
	bl	vlog
	cmp	r5, #1
	bne	.L3
	ldr	r3, [r4, #24]
	cbz	r3, .L3
	movs	r2, #0
	str	r2, [r3, #8]
.L3:
	ldr	r3, [r4, #16]
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L8:
	.align	2
.L7:
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	mpi_obj_referenced, .-mpi_obj_referenced
	.align	1
	.global	mpi_obj_from_id
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_obj_from_id, %function
mpi_obj_from_id:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r1
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r1, r0
	ldr	r0, .L15
	bl	idr_find
	cbz	r0, .L10
	ldr	r3, [r0, #8]
	cmp	r3, r5
	it	ne
	movne	r0, #0
.L10:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r3, r4, r5, pc}
.L16:
	.align	2
.L15:
	.word	.LANCHOR1
	.fnend
	.size	mpi_obj_from_id, .-mpi_obj_from_id
	.align	1
	.global	mpi_dev_find_by_name
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_dev_find_by_name, %function
mpi_dev_find_by_name:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r0
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r5, .L23
	ldr	r4, [r5, #16]!
.L22:
	subs	r4, r4, #36
	add	r3, r4, #36
	cmp	r3, r5
	bne	.L21
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r4, #0
	b	.L17
.L21:
	mov	r1, r7
	ldr	r0, [r4, #44]
	bl	strcmp
	cbnz	r0, .L19
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L17:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L19:
	ldr	r4, [r4, #36]
	b	.L22
.L24:
	.align	2
.L23:
	.word	.LANCHOR1
	.fnend
	.size	mpi_dev_find_by_name, .-mpi_dev_find_by_name
	.align	1
	.global	mpi_allocator_find_by_name
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_allocator_find_by_name, %function
mpi_allocator_find_by_name:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r0
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r5, .L31
	ldr	r4, [r5, #24]!
.L30:
	subs	r4, r4, #36
	add	r3, r4, #36
	cmp	r3, r5
	bne	.L29
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r4, #0
	b	.L25
.L29:
	mov	r1, r7
	ldr	r0, [r4, #44]
	bl	strcmp
	cbnz	r0, .L27
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L25:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L27:
	ldr	r4, [r4, #36]
	b	.L30
.L32:
	.align	2
.L31:
	.word	.LANCHOR1
	.fnend
	.size	mpi_allocator_find_by_name, .-mpi_allocator_find_by_name
	.align	1
	.global	mpi_obj_ref
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_obj_ref, %function
mpi_obj_ref:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbz	r0, .L34
	add	r3, r0, #16
	pld	[r3]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r2, [r3]
	add	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bl	mpi_obj_referenced
.L34:
	mov	r0, r4
	pop	{r4, pc}
	.fnend
	.size	mpi_obj_ref, .-mpi_obj_ref
	.align	1
	.global	mpi_obj_unref
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_obj_unref, %function
mpi_obj_unref:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	mov	r5, r0
	.pad #20
	sub	sp, sp, #20
	cbz	r0, .L38
	ldr	r0, [r0, #16]
	ldr	r2, [r5]
	cbnz	r0, .L41
	movs	r3, #243
	str	r2, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L58
	ldr	r2, .L58+4
	bl	vlog
.L38:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L41:
	ldr	r1, [r5, #16]
	movs	r0, #0
	ldr	r3, .L58+8
	strd	r2, r1, [sp, #8]
	movs	r1, #5
	ldr	r2, [r5, #8]
	add	r2, r3, r2, lsl #2
	adds	r3, r3, #52
	ldr	r2, [r2, #20]
	str	r2, [sp, #4]
	movs	r2, #248
	str	r2, [sp]
	ldr	r2, .L58+12
	bl	vlog
	add	r3, r5, #16
	pld	[r3]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_sub_return
1:	ldrex	r2, [r3]
	sub	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r2, #0
	bne	.L38
	ldr	r4, [r5, #24]
	cbz	r4, .L43
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
.L43:
	ldr	r3, [r5, #20]
	cbz	r3, .L44
	mov	r0, r5
	blx	r3
.L44:
	cmp	r4, #0
	beq	.L38
	ldr	r3, [r4, #4]
	cbz	r3, .L45
	add	r0, r4, #8
	bl	complete_all
.L45:
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
	cmp	r3, #0
	bne	.L38
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, lr}
	b	kfree
.L59:
	.align	2
.L58:
	.word	.LANCHOR0+52
	.word	.LC1
	.word	.LANCHOR0
	.word	.LC2
	.fnend
	.size	mpi_obj_unref, .-mpi_obj_unref
	.align	1
	.global	mpi_obj_register
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_obj_register, %function
mpi_obj_register:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	mov	r4, r0
	str	r3, [r4, #20]
	lsls	r3, r2, #25
	mov	r0, #-1
	str	r1, [r4, #8]
	mov	r5, #0
	orr	r1, r2, #3
	.pad #20
	sub	sp, sp, #20
	str	r0, [r4]
	str	r1, [r4, #12]
	str	r5, [r4, #16]
	bpl	.L61
	ldr	r3, .L76
	mov	r1, #3520
	ldr	r0, [r3, #20]
	bl	kmem_cache_alloc
	ldr	r2, .L76+4
	ldr	r1, .L76+8
	str	r0, [r4, #24]
	adds	r0, r0, #12
	str	r5, [r0, #-4]
	bl	__init_swait_queue_head
	ldr	r3, [r4, #24]
	movs	r2, #1
	str	r2, [r3]
.L61:
	ldr	r3, [r4, #12]
	ands	r3, r3, #16
	bne	.L62
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r2, #2592
	mov	r1, r4
	str	r2, [sp]
	movs	r2, #1
	ldr	r0, .L76+12
	bl	idr_alloc
	cmp	r0, #0
	str	r0, [r4]
	bge	.L63
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, [r4]
.L60:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L63:
	ldr	r3, [r4, #8]
	cmp	r3, #7
	bhi	.L65
	tbb	[pc, r3]
.L67:
	.byte	(.L71-.L67)/2
	.byte	(.L70-.L67)/2
	.byte	(.L65-.L67)/2
	.byte	(.L65-.L67)/2
	.byte	(.L69-.L67)/2
	.byte	(.L68-.L67)/2
	.byte	(.L65-.L67)/2
	.byte	(.L66-.L67)/2
	.p2align 1
.L71:
	ldr	r3, .L76+12
	add	r2, r4, #36
	adds	r3, r3, #16
	ldr	r1, [r3, #4]
	str	r2, [r3, #4]
.L75:
	strd	r3, r1, [r4, #36]
	str	r2, [r1]
.L65:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L62:
	ldr	r2, [r4]
	movs	r1, #5
	ldr	r3, .L76+16
	movs	r0, #0
	str	r4, [sp, #12]
	str	r2, [sp, #8]
	ldr	r2, [r4, #8]
	add	r2, r3, r2, lsl #2
	adds	r3, r3, #66
	ldr	r2, [r2, #20]
	str	r2, [sp, #4]
	movs	r2, #141
	str	r2, [sp]
	ldr	r2, .L76+20
	bl	vlog
	movs	r0, #0
	b	.L60
.L70:
	ldr	r3, .L76+12
	add	r2, r4, #36
	adds	r3, r3, #32
	ldr	r1, [r3, #4]
	str	r2, [r3, #4]
	b	.L75
.L69:
	ldr	r3, .L76+12
	add	r2, r4, #36
	adds	r3, r3, #40
	ldr	r1, [r3, #4]
	str	r2, [r3, #4]
	b	.L75
.L68:
	ldr	r3, .L76+12
	add	r2, r4, #36
	adds	r3, r3, #48
	ldr	r1, [r3, #4]
	str	r2, [r3, #4]
	b	.L75
.L66:
	ldr	r3, .L76+12
	add	r2, r4, #36
	adds	r3, r3, #24
	ldr	r1, [r3, #4]
	str	r2, [r3, #4]
	b	.L75
.L77:
	.align	2
.L76:
	.word	kmalloc_caches
	.word	.LANCHOR2
	.word	.LC3
	.word	.LANCHOR1
	.word	.LANCHOR0
	.word	.LC4
	.fnend
	.size	mpi_obj_register, .-mpi_obj_register
	.align	1
	.global	mpi_obj_unregister
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_obj_unregister, %function
mpi_obj_unregister:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r4, r0
	ldr	r3, [r0, #16]
	ldr	r3, [r0, #12]
	ldr	r5, [r0, #24]
	bic	r2, r3, #3
	str	r2, [r0, #12]
	lsls	r2, r3, #27
	bmi	.L79
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0, #8]
	cmp	r3, #7
	bhi	.L80
	tbb	[pc, r3]
.L82:
	.byte	(.L81-.L82)/2
	.byte	(.L81-.L82)/2
	.byte	(.L80-.L82)/2
	.byte	(.L80-.L82)/2
	.byte	(.L81-.L82)/2
	.byte	(.L81-.L82)/2
	.byte	(.L80-.L82)/2
	.byte	(.L81-.L82)/2
	.p2align 1
.L81:
	adds	r0, r0, #36
	bl	list_del
.L80:
	ldr	r3, [r4, #12]
	ldr	r2, [r4]
	lsls	r3, r3, #26
	bpl	.L83
	movs	r1, #0
	ldr	r0, .L94
	bl	idr_replace
.L84:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L79:
	cbz	r5, .L86
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
	cbnz	r3, .L86
	mov	r0, r5
	bl	kfree
.L86:
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r5, .L94+4
	movs	r0, #0
	ldr	r2, .L94+8
	str	r3, [sp, #8]
	ldr	r3, [r4, #8]
	add	r3, r5, r3, lsl #2
	ldr	r3, [r3, #20]
	str	r3, [sp, #4]
	movs	r3, #171
	str	r3, [sp]
	add	r3, r5, #83
	bl	vlog
	ldr	r0, [r4, #12]
	ands	r0, r0, #8
	bne	.L78
	ldr	r3, [r4]
	movs	r1, #5
	ldr	r2, .L94+12
	str	r3, [sp, #8]
	ldr	r3, [r4, #8]
	add	r3, r5, r3, lsl #2
	ldr	r3, [r3, #20]
	str	r3, [sp, #4]
	movs	r3, #174
	str	r3, [sp]
	add	r3, r5, #83
	bl	vlog
	cbz	r4, .L78
	movs	r1, #28
	adds	r0, r4, r1
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	call_rcu
.L83:
	mov	r1, r2
	ldr	r0, .L94
	bl	idr_remove
	b	.L84
.L78:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L95:
	.align	2
.L94:
	.word	.LANCHOR1
	.word	.LANCHOR0
	.word	.LC5
	.word	.LC6
	.fnend
	.size	mpi_obj_unregister, .-mpi_obj_unregister
	.align	1
	.global	mpi_obj_ref_if_unused
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_obj_ref_if_unused, %function
mpi_obj_ref_if_unused:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	add	r3, r0, #16
	pld	[r3]
	movs	r1, #1
.L97:
	.syntax unified
@ 114 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_cmpxchg
ldrex	r4, [r3]
mov	r2, #0
teq	r4, #0
strexeq r2, r1, [r3]

@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r2, #0
	bne	.L97
	cbnz	r4, .L99
	bl	mpi_obj_referenced
	movs	r0, #1
.L98:
	pop	{r4, pc}
.L99:
	mov	r0, r2
	b	.L98
	.fnend
	.size	mpi_obj_ref_if_unused, .-mpi_obj_ref_if_unused
	.align	1
	.global	mpi_obj_ref_if_used
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_obj_ref_if_used, %function
mpi_obj_ref_if_used:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	add	r1, r0, #16
	movs	r4, #1
	pld	[r1]
	movs	r5, #0
	.syntax unified
@ 136 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add_unless
1:	ldrex	r2, [r1]
	teq	r2, r5
	beq	2f
	add	r6, r2, r4
	strex	r7, r6, [r1]
	teq	r7, #0
	bne	1b
2:
@ 0 "" 2
	.thumb
	.syntax unified
	cbz	r2, .L103
	bl	mpi_obj_referenced
	mov	r0, r4
.L102:
	pop	{r3, r4, r5, r6, r7, pc}
.L103:
	mov	r0, r2
	b	.L102
	.fnend
	.size	mpi_obj_ref_if_used, .-mpi_obj_ref_if_used
	.align	1
	.global	mpi_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_init, %function
mpi_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	mpi_list_init
	cmp	r0, #0
	blt	.L104
	bl	mpi_fs_init
	ldr	r0, .L106
	bl	vlog_probe
	movs	r0, #0
.L104:
	pop	{r3, pc}
.L107:
	.align	2
.L106:
	.word	.LC7
	.fnend
	.size	mpi_init, .-mpi_init
	.align	1
	.global	mpi_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_deinit, %function
mpi_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	mpi_fs_exit
	pop	{r3, lr}
	b	mpi_list_deinit
	.fnend
	.size	mpi_deinit, .-mpi_deinit
	.global	mpi_alloc_list
	.global	mpi_buf_pool_list
	.global	mpi_buf_list
	.global	mpi_node_list
	.global	mpi_dev_list
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.36325, %object
	.size	__func__.36325, 19
__func__.36325:
	.ascii	"mpi_obj_referenced\000"
	.space	1
	.type	names.36271, %object
	.size	names.36271, 32
names.36271:
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.type	__func__.36354, %object
	.size	__func__.36354, 14
__func__.36354:
	.ascii	"mpi_obj_unref\000"
	.type	__func__.36292, %object
	.size	__func__.36292, 17
__func__.36292:
	.ascii	"mpi_obj_register\000"
	.type	__func__.36312, %object
	.size	__func__.36312, 19
__func__.36312:
	.ascii	"mpi_obj_unregister\000"
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	mpi_obj_idr, %object
	.size	mpi_obj_idr, 16
mpi_obj_idr:
	.word	67108868
	.word	0
	.word	0
	.word	0
	.type	mpi_dev_list, %object
	.size	mpi_dev_list, 8
mpi_dev_list:
	.word	mpi_dev_list
	.word	mpi_dev_list
	.type	mpi_alloc_list, %object
	.size	mpi_alloc_list, 8
mpi_alloc_list:
	.word	mpi_alloc_list
	.word	mpi_alloc_list
	.type	mpi_node_list, %object
	.size	mpi_node_list, 8
mpi_node_list:
	.word	mpi_node_list
	.word	mpi_node_list
	.type	mpi_buf_list, %object
	.size	mpi_buf_list, 8
mpi_buf_list:
	.word	mpi_buf_list
	.word	mpi_buf_list
	.type	mpi_buf_pool_list, %object
	.size	mpi_buf_pool_list, 8
mpi_buf_pool_list:
	.word	mpi_buf_pool_list
	.word	mpi_buf_pool_list
	.bss
	.set	.LANCHOR2,. + 0
	.type	__key.10560, %object
	.size	__key.10560, 0
__key.10560:
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"%s,%d:%s(%d) ref from %d\000"
.LC1:
	.ascii	"\033[31m\033[1m%s,%d:mpi buf multiple unref buf id "
	.ascii	"= %d\033[0m\000"
.LC2:
	.ascii	"%s,%d:%s(%d) unref from %d\000"
.LC3:
	.ascii	"&x->wait\000"
.LC4:
	.ascii	"%s,%d:%s(%d|%p) registered\000"
.LC5:
	.ascii	"%s,%d:%s(%d) unregistered\000"
.LC6:
	.ascii	"%s,%d:%s(%d) freed\000"
.LC7:
	.ascii	"vlog\000"
.LC8:
	.ascii	"dev\000"
.LC9:
	.ascii	"node\000"
.LC10:
	.ascii	"job\000"
.LC11:
	.ascii	"job_pool\000"
.LC12:
	.ascii	"buf\000"
.LC13:
	.ascii	"buf_pool\000"
.LC14:
	.ascii	"queue\000"
.LC15:
	.ascii	"alloc\000"
	.section	___ksymtab_gpl+mpi_alloc_list,"a"
	.align	2
	.type	__ksymtab_mpi_alloc_list, %object
	.size	__ksymtab_mpi_alloc_list, 12
__ksymtab_mpi_alloc_list:
	.word	mpi_alloc_list
	.word	__kstrtab_mpi_alloc_list
	.word	__kstrtabns_mpi_alloc_list
	.section	___ksymtab_gpl+mpi_allocator_find_by_name,"a"
	.align	2
	.type	__ksymtab_mpi_allocator_find_by_name, %object
	.size	__ksymtab_mpi_allocator_find_by_name, 12
__ksymtab_mpi_allocator_find_by_name:
	.word	mpi_allocator_find_by_name
	.word	__kstrtab_mpi_allocator_find_by_name
	.word	__kstrtabns_mpi_allocator_find_by_name
	.section	___ksymtab_gpl+mpi_buf_list,"a"
	.align	2
	.type	__ksymtab_mpi_buf_list, %object
	.size	__ksymtab_mpi_buf_list, 12
__ksymtab_mpi_buf_list:
	.word	mpi_buf_list
	.word	__kstrtab_mpi_buf_list
	.word	__kstrtabns_mpi_buf_list
	.section	___ksymtab_gpl+mpi_buf_pool_list,"a"
	.align	2
	.type	__ksymtab_mpi_buf_pool_list, %object
	.size	__ksymtab_mpi_buf_pool_list, 12
__ksymtab_mpi_buf_pool_list:
	.word	mpi_buf_pool_list
	.word	__kstrtab_mpi_buf_pool_list
	.word	__kstrtabns_mpi_buf_pool_list
	.section	___ksymtab_gpl+mpi_dev_find_by_name,"a"
	.align	2
	.type	__ksymtab_mpi_dev_find_by_name, %object
	.size	__ksymtab_mpi_dev_find_by_name, 12
__ksymtab_mpi_dev_find_by_name:
	.word	mpi_dev_find_by_name
	.word	__kstrtab_mpi_dev_find_by_name
	.word	__kstrtabns_mpi_dev_find_by_name
	.section	___ksymtab_gpl+mpi_dev_list,"a"
	.align	2
	.type	__ksymtab_mpi_dev_list, %object
	.size	__ksymtab_mpi_dev_list, 12
__ksymtab_mpi_dev_list:
	.word	mpi_dev_list
	.word	__kstrtab_mpi_dev_list
	.word	__kstrtabns_mpi_dev_list
	.section	___ksymtab_gpl+mpi_node_list,"a"
	.align	2
	.type	__ksymtab_mpi_node_list, %object
	.size	__ksymtab_mpi_node_list, 12
__ksymtab_mpi_node_list:
	.word	mpi_node_list
	.word	__kstrtab_mpi_node_list
	.word	__kstrtabns_mpi_node_list
	.section	___ksymtab_gpl+mpi_obj_from_id,"a"
	.align	2
	.type	__ksymtab_mpi_obj_from_id, %object
	.size	__ksymtab_mpi_obj_from_id, 12
__ksymtab_mpi_obj_from_id:
	.word	mpi_obj_from_id
	.word	__kstrtab_mpi_obj_from_id
	.word	__kstrtabns_mpi_obj_from_id
	.section	___ksymtab_gpl+mpi_obj_ref,"a"
	.align	2
	.type	__ksymtab_mpi_obj_ref, %object
	.size	__ksymtab_mpi_obj_ref, 12
__ksymtab_mpi_obj_ref:
	.word	mpi_obj_ref
	.word	__kstrtab_mpi_obj_ref
	.word	__kstrtabns_mpi_obj_ref
	.section	___ksymtab_gpl+mpi_obj_unref,"a"
	.align	2
	.type	__ksymtab_mpi_obj_unref, %object
	.size	__ksymtab_mpi_obj_unref, 12
__ksymtab_mpi_obj_unref:
	.word	mpi_obj_unref
	.word	__kstrtab_mpi_obj_unref
	.word	__kstrtabns_mpi_obj_unref
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
