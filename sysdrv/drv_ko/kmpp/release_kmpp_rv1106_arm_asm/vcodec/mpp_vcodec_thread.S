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
	.file	"mpp_vcodec_thread.c"
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
	.type	check_vcodec_threads, %function
check_vcodec_threads:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r1
	cbnz	r0, .L2
	ldr	r0, .L7
	bl	printk
.L6:
	mvn	r0, #21
.L1:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L2:
	ldrd	r2, r3, [r0]
	cmp	r2, r0
	bne	.L4
	cbnz	r3, .L5
.L4:
	mov	r1, r0
	str	r4, [sp]
	ldr	r0, .L7+4
	bl	printk
	b	.L6
.L5:
	movs	r0, #0
	b	.L1
.L8:
	.align	2
.L7:
	.word	.LC0
	.word	.LC1
	.fnend
	.size	check_vcodec_threads, .-check_vcodec_threads
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_reset, %function
vcodec_thread_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r7, r0
	ldr	r6, .L31
	ldr	r3, [r6]
	lsls	r1, r3, #31
	bpl	.L10
	ldr	r0, .L31+4
	bl	printk
.L10:
	ldr	r3, [r7, #64]
	cbz	r3, .L11
	ldr	r3, [r7, #16]
	cbnz	r3, .L18
.L16:
	ldr	r0, [r7, #64]
	bl	kfree
.L11:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L9
	ldr	r0, .L31+8
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	b	printk
.L15:
	ldr	r3, [r6]
	mla	r5, r8, r4, r5
	lsls	r2, r3, #31
	bpl	.L13
	mov	r1, r4
	mov	r0, r9
	bl	printk
.L13:
	ldr	r3, [r5, #100]
	cbz	r3, .L14
	add	r0, r5, #72
	bl	kthread_flush_worker
	ldr	r0, [r5, #100]
	bl	kthread_stop
	str	r10, [r5, #100]
.L14:
	adds	r4, r4, #1
.L12:
	ldr	r3, [r7, #16]
	ldr	r5, [r7, #64]
	cmp	r3, r4
	bgt	.L15
	b	.L16
.L18:
	movs	r4, #0
	mov	r8, #104
	ldr	r9, .L31+12
	mov	r10, r4
	b	.L12
.L9:
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L32:
	.align	2
.L31:
	.word	.LANCHOR0
	.word	.LC2
	.word	.LC4
	.word	.LC3
	.fnend
	.size	vcodec_thread_reset, .-vcodec_thread_reset
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_worker, %function
vcodec_thread_worker:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	ldr	r7, [r0, #20]
	bl	ktime_get_raw
	ldr	r3, .L43
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #1
	mov	r5, r0
	mov	r6, r1
	beq	.L34
	ldr	r0, .L43+4
	bl	printk
.L34:
	add	r8, r7, #40
.L37:
	ldrd	r3, r2, [r4, #64]
	ldrd	r1, r0, [r4, #32]
	strd	r5, r6, [r4, #32]
	subs	r3, r3, r1
	sbc	r2, r2, r0
	adds	r3, r3, r5
	adc	r2, r6, r2
	strd	r3, r2, [r4, #64]
	ldr	r3, [r7, #20]
	cbz	r3, .L35
	mov	r1, r8
	ldr	r0, [r4, #24]
	bl	_set_bit
	ldrd	r3, r0, [r7, #20]
	blx	r3
	mov	r1, r8
	str	r0, [r4, #28]
	ldr	r0, [r4, #24]
	bl	_clear_bit
.L36:
	bl	ktime_get_raw
	ldrd	r3, r2, [r4, #56]
	mov	r5, r0
	mov	r6, r1
	ldrd	r1, r0, [r4, #32]
	strd	r5, r6, [r4, #32]
	subs	r3, r3, r1
	sbc	r2, r2, r0
	adds	r3, r3, r5
	adc	r2, r6, r2
	strd	r3, r2, [r4, #56]
	ldrd	r3, r2, [r4, #48]
	adds	r3, r3, #1
	adc	r2, r2, #0
	strd	r3, r2, [r4, #48]
	ldr	r3, [r4, #28]
	cmp	r3, #0
	bgt	.L37
	ldr	r3, [r9]
	lsls	r3, r3, #31
	bpl	.L33
	ldr	r0, .L43+8
	pop	{r3, r4, r5, r6, r7, r8, r9, lr}
	b	printk
.L35:
	str	r3, [r4, #28]
	b	.L36
.L33:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L44:
	.align	2
.L43:
	.word	.LANCHOR0
	.word	.LC5
	.word	.LC6
	.fnend
	.size	vcodec_thread_worker, .-vcodec_thread_worker
	.align	1
	.global	vcodec_thread_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_create, %function
vcodec_thread_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L50
	mov	r1, #3520
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r0, [r3, #4]
	bl	kmem_cache_alloc
	cbz	r0, .L45
	movs	r3, #1
	strd	r0, r4, [r0]
	str	r3, [r0, #28]
	movs	r3, #3
	str	r3, [r0, #12]
	movs	r3, #0
	str	r3, [r0, #8]
.L45:
	pop	{r4, pc}
.L51:
	.align	2
.L50:
	.word	kmalloc_caches
	.fnend
	.size	vcodec_thread_create, .-vcodec_thread_create
	.align	1
	.global	vcodec_thread_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_destroy, %function
vcodec_thread_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r1, .L55
	bl	check_vcodec_threads
	mov	r4, r0
	cbnz	r0, .L54
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	str	r0, [r5, #8]
	mov	r0, r5
	bl	vcodec_thread_reset
	strd	r4, r4, [r5]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r5
	bl	kfree
.L52:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L54:
	mvn	r4, #21
	b	.L52
.L56:
	.align	2
.L55:
	.word	.LC7
	.fnend
	.size	vcodec_thread_destroy, .-vcodec_thread_destroy
	.align	1
	.global	vcodec_thread_set_count
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_set_count, %function
vcodec_thread_set_count:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r1
	ldr	r1, .L68
	mov	r5, r0
	bl	check_vcodec_threads
	mov	r6, r0
	cbnz	r0, .L61
	ldr	r4, .L68+4
	ldr	r2, [r4]
	lsls	r2, r2, #31
	bpl	.L59
	mov	r1, r7
	ldr	r0, .L68+8
	bl	printk
.L59:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #28]
	cmp	r3, r7
	itttt	ne
	strne	r7, [r5, #28]
	ldrne	r3, [r5, #12]
	orrne	r3, r3, #1
	strne	r3, [r5, #12]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L57
	ldr	r0, .L68+12
	bl	printk
.L57:
	mov	r0, r6
	pop	{r3, r4, r5, r6, r7, pc}
.L61:
	mvn	r6, #21
	b	.L57
.L69:
	.align	2
.L68:
	.word	.LC8
	.word	.LANCHOR0
	.word	.LC9
	.word	.LC10
	.fnend
	.size	vcodec_thread_set_count, .-vcodec_thread_set_count
	.align	1
	.global	vcodec_thread_set_callback
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_set_callback, %function
vcodec_thread_set_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r8, r1
	ldr	r1, .L82
	mov	r4, r0
	mov	r7, r2
	bl	check_vcodec_threads
	mov	r6, r0
	cbnz	r0, .L75
	ldr	r5, .L82+4
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L72
	ldr	r0, .L82+8
	bl	printk
.L72:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #32]
	cmp	r3, r8
	bne	.L73
	ldr	r3, [r4, #36]
	cmp	r3, r7
	beq	.L74
.L73:
	ldr	r3, [r4, #12]
	strd	r8, r7, [r4, #32]
	orr	r3, r3, #2
	str	r3, [r4, #12]
.L74:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L70
	ldr	r0, .L82+12
	bl	printk
.L70:
	mov	r0, r6
	pop	{r4, r5, r6, r7, r8, pc}
.L75:
	mvn	r6, #21
	b	.L70
.L83:
	.align	2
.L82:
	.word	.LC11
	.word	.LANCHOR0
	.word	.LC12
	.word	.LC13
	.fnend
	.size	vcodec_thread_set_callback, .-vcodec_thread_set_callback
	.align	1
	.global	vcodec_thread_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_start, %function
vcodec_thread_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #28
	sub	sp, sp, #28
	ldr	r1, .L140
	mov	r4, r0
	bl	check_vcodec_threads
	mov	r9, r0
	cbnz	r0, .L104
	ldr	r6, .L140+4
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L86
	ldr	r1, [r4, #12]
	ldr	r0, .L140+8
	bl	printk
.L86:
	ldr	r3, [r4, #8]
	cmp	r3, #1
	bls	.L87
	cmp	r3, #3
	mov	r1, r4
	ldr	r0, .L140+12
	itet	ls
	ldrls	r2, .L140+16
	movhi	r2, #0
	ldrls	r2, [r2, r3, lsl #2]
	bl	printk
.L104:
	mvn	r5, #21
	b	.L84
.L87:
	ldr	r5, [r4, #12]
	cbz	r5, .L89
	mov	r0, r4
	bl	vcodec_thread_reset
	lsls	r3, r5, #31
	itt	mi
	ldrmi	r3, [r4, #28]
	strmi	r3, [r4, #16]
	lsls	r7, r5, #30
	ittt	mi
	ldrmi	r3, [r4, #32]
	strmi	r3, [r4, #20]
	ldrmi	r3, [r4, #36]
	ldrd	r8, r2, [r4, #16]
	it	mi
	strmi	r3, [r4, #24]
	movs	r3, #0
	str	r3, [r4, #12]
	movs	r3, #90
	str	r3, [sp, #20]
	cmp	r8, #0
	beq	.L92
	cmp	r8, #32
	bgt	.L92
	cbnz	r2, .L93
.L92:
	mov	r1, r8
	ldr	r0, .L140+20
	bl	printk
	mvn	r9, #21
.L94:
	mov	r5, r9
.L89:
	movs	r3, #2
	str	r3, [r4, #8]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L84
	ldr	r0, .L140+24
	bl	printk
.L84:
	mov	r0, r5
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L93:
	ldr	r3, [r6]
	lsls	r5, r3, #31
	bpl	.L95
	ldr	r0, .L140+28
	bl	printk
.L95:
	movs	r0, #104
	umull	r2, r3, r8, r0
	cbnz	r3, .L138
	mov	r1, #3520
	mov	r0, r2
	bl	__kmalloc
	str	r0, [r4, #64]
	cbnz	r0, .L139
.L106:
	mvn	r9, #11
	b	.L94
.L138:
	movs	r3, #0
	str	r3, [r4, #64]
	b	.L106
.L139:
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L99
	mov	r1, r8
	ldr	r0, .L140+32
	bl	printk
.L99:
	movs	r7, #0
.L100:
	cmp	r8, r7
	bgt	.L103
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L94
	ldr	r0, .L140+36
	bl	printk
	b	.L94
.L103:
	movs	r3, #104
	mul	r10, r3, r7
	ldr	r3, [r4, #64]
	add	r5, r3, r10
	str	r3, [sp, #12]
	add	fp, r5, #72
	strd	r4, r7, [r5, #20]
	bl	ktime_get_raw
	ldr	r2, .L140+40
	strd	r0, [r5, #32]
	mov	r0, fp
	ldr	r1, .L140+44
	bl	__kthread_init_worker
	mov	r1, fp
	ldr	r3, .L140+48
	mov	r2, #-1
	ldr	r0, .L140+52
	str	r7, [sp]
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	fp, r0
	bhi	.L101
	bl	wake_up_process
.L101:
	movs	r2, #20
	movs	r1, #0
	str	fp, [r5, #100]
	mov	r0, r5
	bl	memset
	movs	r1, #1
	mov	r0, fp
	ldr	r3, [sp, #12]
	add	r2, sp, #20
	str	r5, [r3, r10]
	ldr	r3, .L140+56
	str	r5, [r5, #4]
	str	r3, [r5, #8]
	bl	sched_setscheduler
	ldr	r3, [r6]
	lsls	r1, r3, #31
	bpl	.L102
	mov	r1, r7
	ldr	r0, .L140+60
	bl	printk
.L102:
	adds	r7, r7, #1
	b	.L100
.L141:
	.align	2
.L140:
	.word	.LC14
	.word	.LANCHOR0
	.word	.LC15
	.word	.LC16
	.word	.LANCHOR1
	.word	.LC17
	.word	.LC24
	.word	.LC18
	.word	.LC19
	.word	.LC23
	.word	.LANCHOR0+4
	.word	.LC20
	.word	.LC21
	.word	kthread_worker_fn
	.word	vcodec_thread_worker
	.word	.LC22
	.fnend
	.size	vcodec_thread_start, .-vcodec_thread_start
	.align	1
	.global	vcodec_thread_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_stop, %function
vcodec_thread_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r1, .L158
	bl	check_vcodec_threads
	mov	r5, r0
	cbnz	r0, .L152
	ldr	r7, .L158+4
	ldr	r2, [r7]
	lsls	r2, r2, #31
	bpl	.L144
	ldr	r0, .L158+8
	bl	printk
.L144:
	ldr	r3, [r4, #8]
	subs	r2, r3, #2
	cmp	r2, #1
	bls	.L145
	cmp	r3, #3
	mov	r1, r4
	ldr	r0, .L158+12
	itet	ls
	ldrls	r2, .L158+16
	movhi	r2, #0
	ldrls	r2, [r2, r3, lsl #2]
	bl	printk
.L152:
	mvn	r5, #21
	b	.L142
.L145:
	ldr	r3, [r4, #16]
	cbnz	r3, .L147
.L150:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #1
	str	r2, [r4, #8]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L142
	ldr	r0, .L158+20
	bl	printk
.L142:
	mov	r0, r5
	pop	{r4, r5, r6, r7, r8, pc}
.L147:
	ldr	r3, [r4, #64]
	cmp	r3, #0
	beq	.L150
	movs	r6, #0
	mov	r8, #104
	b	.L149
.L151:
	ldr	r0, [r4, #64]
	mla	r0, r8, r6, r0
	adds	r6, r6, #1
	adds	r0, r0, #72
	bl	kthread_flush_worker
.L149:
	ldr	r3, [r4, #16]
	cmp	r3, r6
	bgt	.L151
	b	.L150
.L159:
	.align	2
.L158:
	.word	.LC25
	.word	.LANCHOR0
	.word	.LC26
	.word	.LC27
	.word	.LANCHOR1
	.word	.LC28
	.fnend
	.size	vcodec_thread_stop, .-vcodec_thread_stop
	.align	1
	.global	vcodec_thread_pause
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_pause, %function
vcodec_thread_pause:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r1, .L166
	bl	check_vcodec_threads
	cbnz	r0, .L164
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #8]
	cmp	r3, #2
	beq	.L162
	cmp	r3, #3
	mov	r1, r4
	ite	ls
	ldrls	r2, .L166+4
	movhi	r2, r0
	ldr	r0, .L166+8
	it	ls
	ldrls	r2, [r2, r3, lsl #2]
	bl	printk
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L164:
	mvn	r0, #21
	b	.L160
.L162:
	movs	r3, #3
	str	r3, [r4, #8]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L160:
	pop	{r3, r4, r5, pc}
.L167:
	.align	2
.L166:
	.word	.LC29
	.word	.LANCHOR1
	.word	.LC30
	.fnend
	.size	vcodec_thread_pause, .-vcodec_thread_pause
	.align	1
	.global	vcodec_thread_resume
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_resume, %function
vcodec_thread_resume:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r1, .L174
	bl	check_vcodec_threads
	cbnz	r0, .L172
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #8]
	cmp	r3, #3
	beq	.L170
	ite	ls
	ldrls	r2, .L174+4
	movhi	r2, r0
	mov	r1, r4
	ldr	r0, .L174+8
	it	ls
	ldrls	r2, [r2, r3, lsl #2]
	bl	printk
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L172:
	mvn	r0, #21
	b	.L168
.L170:
	movs	r3, #2
	str	r3, [r4, #8]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L168:
	pop	{r3, r4, r5, pc}
.L175:
	.align	2
.L174:
	.word	.LC31
	.word	.LANCHOR1
	.word	.LC32
	.fnend
	.size	vcodec_thread_resume, .-vcodec_thread_resume
	.align	1
	.global	vcodec_thread_trigger
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_thread_trigger, %function
vcodec_thread_trigger:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r4, r0
	ldr	r1, .L215
	bl	check_vcodec_threads
	mov	r7, r0
	cbnz	r0, .L190
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r9, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r6, .L215+4
	ldr	r3, [r6]
	lsls	r5, r3, #31
	bpl	.L178
	ldrd	r2, [r4, #48]
	ldr	r0, .L215+8
	bl	printk
.L178:
	ldr	r3, [r4, #8]
	cmp	r3, #2
	beq	.L179
	cmp	r3, #3
	mov	r1, r4
	ldr	r0, .L215+12
	itet	ls
	ldrls	r2, .L215+16
	movhi	r2, #0
	ldrls	r2, [r2, r3, lsl #2]
	bl	printk
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r9	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L190:
	mvn	r7, #21
	b	.L176
.L179:
	ldr	r3, [r4, #16]
	cmp	r3, #0
	beq	.L181
	ldr	r5, [r4, #64]
	cmp	r5, #0
	beq	.L181
	cmp	r3, #1
	ble	.L182
	add	r1, r4, #40
	mov	r10, #0
.L186:
	asr	r2, r10, #5
	and	r0, r10, #31
	ldr	r2, [r1, r2, lsl #2]
	lsrs	r2, r2, r0
	lsls	r0, r2, #31
	bmi	.L183
	movs	r3, #104
	mla	r5, r3, r10, r5
	add	r0, r5, #72
	mov	r1, r5
	bl	kthread_queue_work
	ldr	r3, [r6]
	lsls	r1, r3, #31
	mov	r8, r0
	bpl	.L188
	mov	r2, r0
	mov	r1, r10
.L214:
	ldr	r0, .L215+20
	bl	printk
.L188:
	ldrd	r3, r2, [r5, #40]
	uxtb	r0, r8
	movs	r1, #0
	adds	r3, r3, r0
	adc	r2, r1, r2
	strd	r3, r2, [r5, #40]
	cmp	r8, #0
	beq	.L187
	ldrd	r3, r2, [r4, #48]
	adds	r3, r3, #1
	adc	r2, r2, #0
	strd	r3, r2, [r4, #48]
	movs	r2, #0
	movs	r3, #0
	strd	r2, [r4, #56]
.L181:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L189
	ldrd	r2, [r4, #56]
	ldr	r0, .L215+24
	strd	r2, [sp]
	ldrd	r2, [r4, #48]
	bl	printk
.L189:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r9	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L176:
	mov	r0, r7
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L183:
	add	r10, r10, #1
	cmp	r3, r10
	bne	.L186
.L187:
	ldrd	r3, r2, [r4, #56]
	adds	r3, r3, #1
	adc	r2, r2, #0
	strd	r3, r2, [r4, #56]
	b	.L181
.L182:
	mov	r1, r5
	add	r0, r5, #72
	bl	kthread_queue_work
	ldr	r3, [r6]
	lsls	r2, r3, #31
	mov	r8, r0
	bpl	.L188
	mov	r2, r0
	movs	r1, #0
	b	.L214
.L216:
	.align	2
.L215:
	.word	.LC25
	.word	.LANCHOR0
	.word	.LC33
	.word	.LC34
	.word	.LANCHOR1
	.word	.LC35
	.word	.LC36
	.fnend
	.size	vcodec_thread_trigger, .-vcodec_thread_trigger
	.global	thread_debug
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	str_thd_state, %object
	.size	str_thd_state, 16
str_thd_state:
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.type	__param_str_thread_debug, %object
	.size	__param_str_thread_debug, 24
__param_str_thread_debug:
	.ascii	"mpp_vcodec.thread_debug\000"
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	thread_debug, %object
	.size	thread_debug, 4
thread_debug:
	.space	4
	.type	__key.20987, %object
	.size	__key.20987, 0
__key.20987:
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_thread_debug166, %object
	.size	__UNIQUE_ID_thread_debug166, 76
__UNIQUE_ID_thread_debug166:
	.ascii	"mpp_vcodec.parm=thread_debug:bit switch for vcodec "
	.ascii	"thread debug information\000"
	.type	__UNIQUE_ID_thread_debugtype165, %object
	.size	__UNIQUE_ID_thread_debugtype165, 38
__UNIQUE_ID_thread_debugtype165:
	.ascii	"mpp_vcodec.parmtype=thread_debug:uint\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013vcodec_thread: NULL invalid vcodec threads on "
	.ascii	"%s\012\000"
.LC1:
	.ascii	"\0013vcodec_thread: %p invalid vcodec threads check"
	.ascii	" %p module %p at %s\012\000"
.LC2:
	.ascii	"\0016vcodec_thread: enter reset\012\000"
.LC3:
	.ascii	"\0016vcodec_thread: stop thread %d\012\000"
.LC4:
	.ascii	"\0016vcodec_thread: leave reset\012\000"
.LC5:
	.ascii	"\0016vcodec_thread: enter worker\012\000"
.LC6:
	.ascii	"\0016vcodec_thread: leave worker\012\000"
.LC7:
	.ascii	"destroy\000"
.LC8:
	.ascii	"set count\000"
.LC9:
	.ascii	"\0016vcodec_thread: enter set count %d\012\000"
.LC10:
	.ascii	"\0016vcodec_thread: leave set count\012\000"
.LC11:
	.ascii	"set callback\000"
.LC12:
	.ascii	"\0016vcodec_thread: enter set callback\012\000"
.LC13:
	.ascii	"\0016vcodec_thread: leave set callback\012\000"
.LC14:
	.ascii	"start\000"
.LC15:
	.ascii	"\0016vcodec_thread: enter start with change %x\012\000"
.LC16:
	.ascii	"\0013vcodec_thread: %p can not start at status %s\000"
.LC17:
	.ascii	"\0013vcodec_thread: invalid count %d callback %p\000"
.LC18:
	.ascii	"\0016vcodec_thread: enter prepare\012\000"
.LC19:
	.ascii	"\0016vcodec_thread: prepare count %d\012\000"
.LC20:
	.ascii	"(&thd->worker)->lock\000"
.LC21:
	.ascii	"vcodec_thread_%d\000"
.LC22:
	.ascii	"\0016vcodec_thread: prepare thread %d done\012\000"
.LC23:
	.ascii	"\0016vcodec_thread: leave prepare\012\000"
.LC24:
	.ascii	"\0016vcodec_thread: leave start\012\000"
.LC25:
	.ascii	"stop\000"
.LC26:
	.ascii	"\0016vcodec_thread: enter stop\012\000"
.LC27:
	.ascii	"\0013vcodec_thread: %p can not stop at status %s\000"
.LC28:
	.ascii	"\0016vcodec_thread: leave stop\012\000"
.LC29:
	.ascii	"pause\000"
.LC30:
	.ascii	"\0013vcodec_thread: %p can not pause at status %s\000"
.LC31:
	.ascii	"resume\000"
.LC32:
	.ascii	"\0013vcodec_thread: %p can not resume at status %s\000"
.LC33:
	.ascii	"\0016vcodec_thread: enter trigger %llu\012\000"
.LC34:
	.ascii	"\0013vcodec_thread: %p can not trigger at status %s"
	.ascii	"\000"
.LC35:
	.ascii	"\0016vcodec_thread: queue work %d ret %d\012\000"
.LC36:
	.ascii	"\0016vcodec_thread: leave trigger %llu:%llu\012\000"
.LC37:
	.ascii	"invalid\000"
.LC38:
	.ascii	"ready\000"
.LC39:
	.ascii	"running\000"
.LC40:
	.ascii	"paused\000"
	.section	__param,"a"
	.align	2
	.type	__param_thread_debug, %object
	.size	__param_thread_debug, 20
__param_thread_debug:
	.word	__param_str_thread_debug
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	thread_debug
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
