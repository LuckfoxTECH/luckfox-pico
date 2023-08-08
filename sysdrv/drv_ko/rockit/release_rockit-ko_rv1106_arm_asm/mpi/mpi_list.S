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
	.file	"mpi_list.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_list_init:					
	.asciz 	"mpi_list_init"					
__kstrtabns_mpi_list_init:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_list_deinit:					
	.asciz 	"mpi_list_deinit"					
__kstrtabns_mpi_list_deinit:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_list_free_rcu, %function
mpi_list_free_rcu:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L2
	sub	r1, r0, #12
	ldr	r0, [r3]
	b	kmem_cache_free
.L3:
	.align	2
.L2:
	.word	.LANCHOR0
	.fnend
	.size	mpi_list_free_rcu, .-mpi_list_free_rcu
	.align	1
	.global	mpi_list_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_list_init, %function
mpi_list_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	movs	r3, #0
	movs	r2, #4
	str	r3, [sp]
	movs	r1, #20
	ldr	r0, .L5
	bl	kmem_cache_create
	ldr	r3, .L5+4
	str	r0, [r3]
	clz	r0, r0
	lsrs	r0, r0, #5
	rsbs	r0, r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L6:
	.align	2
.L5:
	.word	.LC0
	.word	.LANCHOR0
	.fnend
	.size	mpi_list_init, .-mpi_list_init
	.align	1
	.global	mpi_list_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_list_deinit, %function
mpi_list_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L8
	ldr	r0, [r3]
	b	kmem_cache_destroy
.L9:
	.align	2
.L8:
	.word	.LANCHOR0
	.fnend
	.size	mpi_list_deinit, .-mpi_list_deinit
	.align	1
	.global	mpi_list_del_obj_rcu
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_list_del_obj_rcu, %function
mpi_list_del_obj_rcu:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0]
.L15:
	subs	r3, r3, #4
	adds	r2, r3, #4
	cmp	r0, r2
	bne	.L14
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, #-1
	b	.L10
.L14:
	ldr	r2, [r3]
	cmp	r2, r1
	bne	.L12
	ldrd	r1, r2, [r3, #4]
	add	r0, r3, #12
	str	r2, [r1, #4]
	str	r1, [r2]
	mov	r2, #290
	ldr	r1, .L16
	str	r2, [r3, #8]
	bl	call_rcu
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
.L10:
	pop	{r4, pc}
.L12:
	ldr	r3, [r3, #4]
	b	.L15
.L17:
	.align	2
.L16:
	.word	mpi_list_free_rcu
	.fnend
	.size	mpi_list_del_obj_rcu, .-mpi_list_del_obj_rcu
	.align	1
	.global	mpi_list_add_tail_obj_rcu
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_list_add_tail_obj_rcu, %function
mpi_list_add_tail_obj_rcu:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r3, .L21
	mov	r5, r1
	mov	r1, #3264
	ldr	r0, [r3]
	bl	kmem_cache_alloc
	cbz	r0, .L20
	mov	r3, r0
	str	r5, [r3], #4
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r4, #4]
	strd	r4, r2, [r0, #4]
	str	r3, [r2]
	str	r3, [r4, #4]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
.L18:
	pop	{r3, r4, r5, pc}
.L20:
	mov	r0, #-1
	b	.L18
.L22:
	.align	2
.L21:
	.word	.LANCHOR0
	.fnend
	.size	mpi_list_add_tail_obj_rcu, .-mpi_list_add_tail_obj_rcu
	.align	1
	.global	mpi_list_pop_obj_rcu
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_list_pop_obj_rcu, %function
mpi_list_pop_obj_rcu:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0]
	cmp	r0, r3
	str	r3, [sp, #4]
	beq	.L25
	ldr	r0, [sp, #4]
	cmp	r0, #4
	bne	.L26
.L25:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r4, #0
.L23:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L26:
	ldrd	r2, r3, [r0]
	adds	r0, r0, #8
	ldr	r4, [r0, #-12]
	ldr	r1, .L28
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r3, #290
	str	r3, [r0, #-4]
	bl	call_rcu
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L23
.L29:
	.align	2
.L28:
	.word	mpi_list_free_rcu
	.fnend
	.size	mpi_list_pop_obj_rcu, .-mpi_list_pop_obj_rcu
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	mpi_entry_cache, %object
	.size	mpi_entry_cache, 4
mpi_entry_cache:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"mpi_list\000"
	.section	___ksymtab_gpl+mpi_list_deinit,"a"
	.align	2
	.type	__ksymtab_mpi_list_deinit, %object
	.size	__ksymtab_mpi_list_deinit, 12
__ksymtab_mpi_list_deinit:
	.word	mpi_list_deinit
	.word	__kstrtab_mpi_list_deinit
	.word	__kstrtabns_mpi_list_deinit
	.section	___ksymtab_gpl+mpi_list_init,"a"
	.align	2
	.type	__ksymtab_mpi_list_init, %object
	.size	__ksymtab_mpi_list_init, 12
__ksymtab_mpi_list_init:
	.word	mpi_list_init
	.word	__kstrtab_mpi_list_init
	.word	__kstrtabns_mpi_list_init
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
