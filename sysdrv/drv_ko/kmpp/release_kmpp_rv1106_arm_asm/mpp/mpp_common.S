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
	.file	"mpp_common.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpp_show_session_info:					
	.asciz 	"mpp_show_session_info"					
__kstrtabns_mpp_show_session_info:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpp_srv_get_phy:					
	.asciz 	"mpp_srv_get_phy"					
__kstrtabns_mpp_srv_get_phy:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_get_mppdev_svr_ops:					
	.asciz 	"get_mppdev_svr_ops"					
__kstrtabns_get_mppdev_svr_ops:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	____atomic_set_bit, %function
____atomic_set_bit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	lsrs	r2, r0, #5
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r5, [r1, r2, lsl #2]
	and	r3, r0, #31
	movs	r0, #1
	lsls	r0, r0, r3
	orrs	r0, r0, r5
	str	r0, [r1, r2, lsl #2]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r4, r5, pc}
	.fnend
	.size	____atomic_set_bit, .-____atomic_set_bit
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	list_move_tail, %function
list_move_tail:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r2, r3, [r0]
	str	r3, [r2, #4]
	str	r2, [r3]
	ldr	r3, [r1, #4]
	str	r0, [r1, #4]
	strd	r1, r3, [r0]
	str	r0, [r3]
	bx	lr
	.fnend
	.size	list_move_tail, .-list_move_tail
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_taskqueue_pending_to_run, %function
mpp_taskqueue_pending_to_run:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r3, r1
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r1, r0, #60
	add	r0, r3, #20
	bl	list_move_tail
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	mpp_taskqueue_pending_to_run, .-mpp_taskqueue_pending_to_run
	.align	1
	.global	mpp_chnl_is_running
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_chnl_is_running, %function
mpp_chnl_is_running:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0, #20]!
	ldr	r2, [r3], #-4
	subs	r2, r2, #4
.L5:
	adds	r5, r3, #4
	cmp	r5, r0
	bne	.L7
	movs	r0, #0
.L6:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r4, r5, pc}
.L7:
	ldr	r3, [r3, #2200]
	lsls	r3, r3, #30
	bmi	.L8
	ldr	r5, [r2, #4]
	mov	r3, r2
	subs	r2, r5, #4
	b	.L5
.L8:
	movs	r0, #1
	b	.L6
	.fnend
	.size	mpp_chnl_is_running, .-mpp_chnl_is_running
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_check_cmd_v1, %function
mpp_check_cmd_v1:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	r3, r0, #512
	cmp	r3, #4
	bls	.L12
	cmp	r0, #768
	beq	.L14
	bic	r3, r0, #256
	cmp	r3, #2
	ite	hi
	movhi	r3, #0
	movls	r3, #1
.L10:
	sub	r0, r0, #1024
	cmp	r0, #4
	bls	.L14
	cmp	r3, #0
	ite	ne
	movne	r0, #0
	mvneq	r0, #21
	bx	lr
.L12:
	movs	r3, #1
	b	.L10
.L14:
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_check_cmd_v1, .-mpp_check_cmd_v1
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_chnl_get_dev, %function
mpp_chnl_get_dev:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #12]
	ldr	r0, [r3]
	bx	lr
	.fnend
	.size	mpp_chnl_get_dev, .-mpp_chnl_get_dev
	.align	1
	.global	get_mppdev_svr_ops
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_mppdev_svr_ops, %function
get_mppdev_svr_ops:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L18
	bx	lr
.L19:
	.align	2
.L18:
	.word	.LANCHOR0
	.fnend
	.size	get_mppdev_svr_ops, .-get_mppdev_svr_ops
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_get_idle_task, %function
mpp_session_get_idle_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r6, r0
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r4, [r0, #20]
	ldr	r8, .L35
	ldr	r9, .L35+4
	ldr	r5, [r4], #-4
	ldr	r10, .L35+8
	subs	r5, r5, #4
.L21:
	adds	r3, r4, #4
	add	r2, r6, #20
	cmp	r3, r2
	bne	.L24
	movs	r3, #0
.L26:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r3, #0
	it	eq
	moveq	r4, #0
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L24:
	cbz	r4, .L22
	ldr	r3, [r8]
	lsls	r2, r3, #22
	bpl	.L23
	ldr	r3, [r4]
	mov	r1, r9
	str	r6, [sp, #12]
	mov	r0, r10
	ldr	r2, [r3, #196]
	strd	r3, r2, [sp, #4]
	movs	r2, #149
	ldr	r3, [r4, #2200]
	str	r3, [sp]
	ldr	r3, [r4, #2276]
	bl	printk
.L23:
	ldr	r3, [r4, #2200]
	lsls	r3, r3, #30
	bmi	.L22
	movs	r3, #1
	b	.L26
.L22:
	ldr	r3, [r5, #4]
	mov	r4, r5
	subs	r5, r3, #4
	b	.L21
.L36:
	.align	2
.L35:
	.word	mpp_dev_debug
	.word	.LANCHOR1
	.word	.LC0
	.fnend
	.size	mpp_session_get_idle_task, .-mpp_session_get_idle_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_attach_workqueue, %function
mpp_session_attach_workqueue:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r3, .L42
	mov	r5, r1
	ldr	r3, [r3]
	lsls	r3, r3, #9
	bpl	.L38
	ldr	r2, [r0, #4]
	mov	r1, r0
	ldr	r0, .L42+4
	bl	printk
.L38:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #36]
	add	r2, r4, #176
	adds	r5, r5, #32
	str	r2, [r5, #4]
	strd	r5, r3, [r4, #176]
	str	r2, [r3]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r3, r4, r5, pc}
.L43:
	.align	2
.L42:
	.word	mpp_dev_debug
	.word	.LC1
	.fnend
	.size	mpp_session_attach_workqueue, .-mpp_session_attach_workqueue
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_process_task, %function
mpp_process_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r3, [r0, #208]
	cbz	r3, .L45
	pop	{r4, lr}
	bx	r3
.L45:
	ldr	r0, .L46
	bl	printk
	mvn	r0, #21
	pop	{r4, pc}
.L47:
	.align	2
.L46:
	.word	.LC2
	.fnend
	.size	mpp_process_task, .-mpp_process_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_wait_result, %function
mpp_wait_result:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r3, [r0, #212]
	cbz	r3, .L49
	pop	{r4, lr}
	bx	r3
.L49:
	ldr	r0, .L50
	bl	printk
	mvn	r0, #21
	pop	{r4, pc}
.L51:
	.align	2
.L50:
	.word	.LC3
	.fnend
	.size	mpp_wait_result, .-mpp_wait_result
	.align	1
	.global	mpp_show_session_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_show_session_info, %function
mpp_show_session_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L65
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r7, r0
	ldr	r4, [r3]
	mov	r8, r1
	add	r6, r4, #700
	add	r4, r4, #712
	mov	r0, r6
	bl	mutex_lock
	ldr	r0, [r4]
	ldr	r5, [r0], #-168
	subs	r5, r5, #168
.L53:
	add	r3, r0, #168
	cmp	r3, r4
	bne	.L55
	mov	r0, r6
	bl	mutex_unlock
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
.L55:
	ldr	r3, [r0, #196]
	cmp	r3, r8
	bne	.L54
	ldr	r3, [r0, #184]
	cbz	r3, .L54
	ldr	r3, [r0, #12]
	cbz	r3, .L54
	ldr	r3, [r3, #12]
	ldr	r3, [r3, #60]
	cbz	r3, .L54
	mov	r1, r7
	blx	r3
.L54:
	ldr	r3, [r5, #168]
	mov	r0, r5
	sub	r5, r3, #168
	b	.L53
.L66:
	.align	2
.L65:
	.word	.LANCHOR2
	.fnend
	.size	mpp_show_session_info, .-mpp_show_session_info
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_copy_from_user, %function
_copy_from_user:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r3, sp
	bic	r3, r3, #8160
	mov	r6, r0
	bic	r3, r3, #31
	mov	r4, r2
	ldr	r3, [r3, #8]
	.syntax unified
@ 157 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/include/linux/uaccess.h" 1
	.syntax unified
adds r5, r1, r2; sbcscc r5, r5, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L70
	bl	arm_copy_from_user
	mov	r5, r0
.L68:
	cbz	r5, .L67
	subs	r0, r4, r5
	mov	r2, r5
	movs	r1, #0
	add	r0, r0, r6
	bl	memset
.L67:
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L70:
	mov	r5, r2
	b	.L68
	.fnend
	.size	_copy_from_user, .-_copy_from_user
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	fops_write_u32, %function
fops_write_u32:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r2
	ldr	r3, [r0, #128]
	mov	r5, r1
	movs	r2, #0
	mov	r1, r4
	mov	r0, r5
	ldr	r3, [r3, #64]
	bl	kstrtouint_from_user
	cmp	r0, #0
	it	eq
	moveq	r0, r4
	pop	{r3, r4, r5, pc}
	.fnend
	.size	fops_write_u32, .-fops_write_u32
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	fops_open_u32, %function
fops_open_u32:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r1
	bl	PDE_DATA
	ldr	r1, .L77
	mov	r2, r0
	mov	r0, r4
	pop	{r4, lr}
	b	single_open
.L78:
	.align	2
.L77:
	.word	fops_show_u32
	.fnend
	.size	fops_open_u32, .-fops_open_u32
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	fops_show_u32, %function
fops_show_u32:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r3, [r0, #64]
	ldr	r1, .L80
	ldr	r2, [r3]
	bl	seq_printf
	movs	r0, #0
	pop	{r3, pc}
.L81:
	.align	2
.L80:
	.word	.LC4
	.fnend
	.size	fops_show_u32, .-fops_show_u32
	.align	1
	.global	mpp_srv_get_phy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_srv_get_phy, %function
mpp_srv_get_phy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L83
	ldr	r3, [r3]
	ldr	r1, [r3, #4]
	b	mpp_dma_get_iova
.L84:
	.align	2
.L83:
	.word	.LANCHOR2
	.fnend
	.size	mpp_srv_get_phy, .-mpp_srv_get_phy
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_chnl_get_iova_addr, %function
mpp_chnl_get_iova_addr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r3, r1
	cbz	r0, .L89
	ldr	r2, [r0, #12]
	mov	r0, r3
	pop	{r3, lr}
	ldr	r1, [r2]
	b	mpp_dma_get_iova
.L89:
	movw	r2, #2584
	ldr	r1, .L90
	ldr	r0, .L90+4
	bl	printk
	mov	r0, #-1
	pop	{r3, pc}
.L91:
	.align	2
.L90:
	.word	.LANCHOR1+26
	.word	.LC5
	.fnend
	.size	mpp_chnl_get_iova_addr, .-mpp_chnl_get_iova_addr
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_fetch_add_relaxed.constprop.19, %function
atomic_fetch_add_relaxed.constprop.19:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	pld	[r0]
	.syntax unified
@ 223 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_fetch_add
1:	ldrex	r0, [r3]
	add	r2, r0, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	atomic_fetch_add_relaxed.constprop.19, .-atomic_fetch_add_relaxed.constprop.19
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_add.constprop.23, %function
atomic_add.constprop.23:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	pld	[r0]
	.syntax unified
@ 223 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
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
	.size	atomic_add.constprop.23, .-atomic_add.constprop.23
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_process_task_default, %function
mpp_process_task_default:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r5, r0
	ldr	r3, [r0, #12]
	mov	r7, r1
	cbnz	r3, .L95
	ldr	r3, [r0]
	mov	r2, #664
	ldr	r1, .L115
	str	r3, [sp]
	ldr	r3, [r0, #28]
	ldr	r0, .L115+4
	bl	printk
	mvn	r0, #21
.L94:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L95:
	ldr	r3, [r3, #12]
	ldr	r3, [r3, #16]
	cbnz	r3, .L97
.L98:
	movw	r2, #671
	ldr	r1, .L115
	ldr	r0, .L115+8
	bl	printk
	mvn	r0, #11
	b	.L94
.L97:
	blx	r3
	mov	r4, r0
	cmp	r0, #0
	beq	.L98
	ldr	r6, [r0, #2292]
	cbnz	r6, .L99
	ldr	r6, [r5, #12]
.L99:
	movs	r3, #1
	ldr	r2, .L115+12
	str	r3, [r4, #2252]
	addw	r0, r4, #2284
	ldr	r1, .L115+16
	mov	r8, #0
	bl	__init_waitqueue_head
	str	r8, [r4, #2204]
	add	r0, r6, #84
	bl	atomic_fetch_add_relaxed.constprop.19
	mvn	r3, #31
	str	r3, [r4, #2208]
	addw	r3, r4, #2212
	str	r3, [r4, #2212]
	str	r3, [r4, #2216]
	mov	r2, #2097152
	ldr	r3, .L115+20
	str	r0, [r4, #2276]
	add	r0, r4, #2224
	ldr	r1, .L115+24
	str	r3, [r4, #2220]
	mov	r3, r8
	str	r8, [sp]
	bl	init_timer_key
	ldrb	r3, [r6, #36]	@ zero_extendqisi2
	cbz	r3, .L100
	ldr	r3, [r6, #8]
	ldr	r3, [r3, #16]
	cbz	r3, .L100
	mov	r1, r4
	mov	r0, r6
	blx	r3
.L100:
	ldr	r3, [r6, #96]
	add	r0, r5, #32
	str	r6, [r7, #28]
	addw	r6, r4, #2252
	strd	r3, r4, [r7, #20]
	bl	atomic_add.constprop.23
	mov	r0, r6
	bl	atomic_fetch_add_relaxed.constprop.19
	cmp	r0, #0
	bne	.L101
	movs	r1, #2
.L114:
	mov	r0, r6
	bl	refcount_warn_saturate
.L102:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r5, #24]
	adds	r3, r4, #4
	add	r0, r5, #20
	str	r3, [r5, #24]
	strd	r0, r2, [r4, #4]
	str	r3, [r2]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L115+28
	ldr	r0, [r3]
	ands	r0, r0, #512
	beq	.L94
	ldr	r3, [r4, #2200]
	movw	r2, #701
	ldr	r1, .L115
	ldr	r0, .L115+32
	str	r3, [sp, #8]
	ldr	r3, [r4, #2276]
	str	r3, [sp, #4]
	ldr	r3, [r5, #4]
	str	r3, [sp]
	ldr	r3, [r5]
	bl	printk
	movs	r0, #0
	b	.L94
.L101:
	blt	.L103
	cmn	r0, #1
	bpl	.L102
.L103:
	movs	r1, #1
	b	.L114
.L116:
	.align	2
.L115:
	.word	.LANCHOR1+49
	.word	.LC6
	.word	.LC7
	.word	.LANCHOR2+4
	.word	.LC8
	.word	mpp_task_timeout_work
	.word	delayed_work_timer_fn
	.word	mpp_dev_debug
	.word	.LC9
	.fnend
	.size	mpp_process_task_default, .-mpp_process_task_default
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	list_del_init, %function
list_del_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r2, r3, [r0]
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r0, [r0]
	str	r0, [r0, #4]
	bx	lr
	.fnend
	.size	list_del_init, .-list_del_init
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	put_task_msgs, %function
put_task_msgs:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, [r0, #16]
	cbnz	r5, .L119
	ldr	r0, .L125
	pop	{r4, r5, r6, lr}
	b	printk
.L119:
	ldr	r3, [r0, #32]
	cmp	r3, #0
	blt	.L120
	ldr	r3, [r0, #40]
	lsls	r3, r3, #31
	bpl	.L121
	ldr	r0, [r0, #36]
	bl	fput
.L121:
	mov	r3, #-1
	str	r3, [r4, #32]
.L120:
	mov	r0, r4
	bl	list_del_init
	movs	r3, #0
	strd	r3, r3, [r4, #44]
	strd	r3, r3, [r4, #52]
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r1, r5, #232
	add	r0, r4, #8
	bl	list_move_tail
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r4, r5, r6, pc}
.L126:
	.align	2
.L125:
	.word	.LC10
	.fnend
	.size	put_task_msgs, .-put_task_msgs
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	task_msgs_add, %function
task_msgs_add:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r3, [r0, #52]
	mov	r5, r1
	cbnz	r3, .L128
.L130:
	str	r4, [r4]
	str	r4, [r4, #4]
	ldr	r3, [r5, #4]
	str	r4, [r5, #4]
	strd	r5, r3, [r4]
	str	r4, [r3]
	pop	{r3, r4, r5, pc}
.L128:
	ldr	r0, [r0, #16]
	mov	r1, r4
	ldr	r3, [r4, #44]
	str	r3, [r0, #164]
	bl	mpp_process_task
	cmp	r0, #0
	beq	.L130
	mov	r0, r4
	pop	{r3, r4, r5, lr}
	b	put_task_msgs
	.fnend
	.size	task_msgs_add, .-task_msgs_add
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_detach_workqueue, %function
mpp_detach_workqueue:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r1, [r0, #96]
	cbz	r1, .L135
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0, #116]
	movs	r5, #0
	adds	r3, r3, #26
	add	r3, r1, r3, lsl #2
	adds	r1, r1, #124
	str	r5, [r3, #4]
	ldr	r0, [r1, #4]
	subs	r0, r0, #1
	str	r0, [r1, #4]
	bl	_clear_bit
	add	r0, r4, #108
	bl	list_del_init
	str	r5, [r4, #96]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L135:
	pop	{r4, r5, r6, pc}
	.fnend
	.size	mpp_detach_workqueue, .-mpp_detach_workqueue
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_init, %function
mpp_session_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L148
	mov	r1, #3520
	push	{r4, lr}
	.save {r4, lr}
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L140
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r3, [r3, #12]
	ldr	r3, [r3, #472]
	str	r3, [r0, #28]
	add	r3, r0, #20
	str	r3, [r0, #20]
	str	r3, [r0, #24]
	add	r3, r0, #168
	str	r3, [r0, #168]
	str	r3, [r0, #172]
	add	r3, r0, #176
	str	r3, [r0, #176]
	str	r3, [r0, #180]
	movs	r3, #0
	str	r3, [r0, #32]
	str	r3, [r0, #36]
	add	r3, r0, #224
	str	r3, [r0, #224]
	str	r3, [r0, #228]
	add	r3, r0, #232
	str	r3, [r0, #232]
	str	r3, [r0, #236]
	ldr	r3, .L148+4
	ldr	r3, [r3]
	lsls	r3, r3, #9
	bpl	.L140
	mov	r1, r0
	ldr	r0, .L148+8
	bl	printk
.L140:
	mov	r0, r4
	pop	{r4, pc}
.L149:
	.align	2
.L148:
	.word	kmalloc_caches
	.word	mpp_dev_debug
	.word	.LC11
	.fnend
	.size	mpp_session_init, .-mpp_session_init
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_chnl_open, %function
mpp_chnl_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r5, .L183
	ldr	r3, [r5]
	cbnz	r3, .L151
.L153:
	movs	r4, #0
.L150:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L151:
	bl	mpp_session_init
	mov	r4, r0
	cmp	r0, #0
	beq	.L153
	movs	r3, #1
	str	r3, [r4, #200]
	ldr	r3, .L183+4
	ldr	r0, [r5]
	str	r3, [r4, #208]
	ldr	r3, .L183+8
	str	r0, [r4, #8]
	str	r3, [r4, #212]
	ldr	r3, .L183+12
	str	r3, [r4, #216]
	cbz	r0, .L154
	add	r0, r0, #700
	bl	mutex_lock
	ldr	r3, [r5]
	add	r2, r4, #168
	add	r3, r3, #712
	ldr	r1, [r3, #4]
	str	r2, [r3, #4]
	strd	r3, r1, [r4, #168]
	str	r2, [r1]
	ldr	r0, [r5]
	add	r0, r0, #700
	bl	mutex_unlock
.L154:
	.syntax unified
@ 87 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
	cmp	r6, #29
	sbc	r3, r6, r6
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r6, r6, r3
	ldr	r3, [r5]
	add	r2, r6, #86
	ldr	r5, [r3, r2, lsl #2]
	cmp	r5, #0
	beq	.L153
	str	r6, [r4]
	str	r5, [r4, #12]
	ldr	r3, [r5, #12]
	cbz	r3, .L156
	ldr	r3, [r3]
	cbz	r3, .L157
	str	r3, [r4, #208]
.L157:
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #4]
	cbz	r3, .L158
	str	r3, [r4, #212]
.L158:
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #8]
	cbz	r3, .L156
	str	r3, [r4, #216]
.L156:
	add	r0, r5, #76
	bl	atomic_fetch_add_relaxed.constprop.19
	str	r0, [r4, #4]
	ldr	r3, [r5, #12]
	cbnz	r3, .L160
.L161:
	ldr	r1, [r5, #96]
	mov	r0, r4
	bl	mpp_session_attach_workqueue
	b	.L150
.L160:
	ldr	r3, [r3, #52]
	cmp	r3, #0
	beq	.L161
	mov	r0, r4
	blx	r3
	cmp	r0, #0
	beq	.L161
	b	.L153
.L184:
	.align	2
.L183:
	.word	.LANCHOR2
	.word	mpp_process_task_default
	.word	mpp_wait_result_default
	.word	mpp_session_deinit_default
	.fnend
	.size	mpp_chnl_open, .-mpp_chnl_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_open, %function
mpp_dev_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r7, r0
	ldr	r6, .L199
	mov	r8, r1
	ldr	r5, [r0, #320]
	ldr	r3, [r6]
	sub	r9, r5, #12
	lsls	r2, r3, #26
	bpl	.L186
	movw	r2, #1731
	ldr	r1, .L199+4
	ldr	r0, .L199+8
	bl	printk
.L186:
	bl	mpp_session_init
	mov	r4, r0
	cbz	r0, .L187
	str	r9, [r0, #8]
	cmp	r9, #0
	beq	.L188
	add	r9, r5, #688
	add	r5, r5, #700
	mov	r0, r9
	bl	mutex_lock
	ldr	r2, [r5, #4]
	add	r3, r4, #168
	mov	r0, r9
	str	r3, [r5, #4]
	strd	r5, r2, [r4, #168]
	str	r3, [r2]
	bl	mutex_unlock
.L188:
	ldr	r3, .L199+12
	str	r3, [r4, #208]
	ldr	r3, .L199+16
	str	r3, [r4, #212]
	ldr	r3, .L199+20
	str	r3, [r4, #216]
	ldr	r3, [r6]
	str	r4, [r8, #128]
	lsls	r3, r3, #26
	bpl	.L189
	movw	r2, #1749
	ldr	r1, .L199+4
	ldr	r0, .L199+24
	bl	printk
.L189:
	mov	r1, r8
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, r8, r9, lr}
	b	nonseekable_open
.L187:
	mvn	r0, #11
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L200:
	.align	2
.L199:
	.word	mpp_dev_debug
	.word	.LANCHOR1+74
	.word	.LC12
	.word	mpp_process_task_default
	.word	mpp_wait_result_default
	.word	mpp_session_deinit_default
	.word	.LC13
	.fnend
	.size	mpp_dev_open, .-mpp_dev_open
	.align	1
	.global	mpp_free_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_free_task, %function
mpp_free_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	subw	r6, r0, #2252
	ldr	r4, [r6]
	cbnz	r4, .L202
	mov	r3, r6
	mov	r2, #588
	ldr	r1, .L212
	ldr	r0, .L212+4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	printk
.L202:
	ldr	r3, .L212+8
	ldr	r3, [r3]
	lsls	r3, r3, #22
	bpl	.L203
	ldr	r3, [r6, #2204]
	mov	r2, #596
	ldr	r1, .L212
	ldr	r0, .L212+12
	str	r3, [sp, #12]
	ldr	r3, [r6, #2200]
	str	r3, [sp, #8]
	ldr	r3, [r6, #2276]
	str	r3, [sp, #4]
	ldr	r3, [r4, #4]
	str	r3, [sp]
	ldr	r3, [r4]
	bl	printk
.L203:
	ldr	r5, [r6, #2292]
	cbnz	r5, .L204
	ldr	r5, [r4, #12]
.L204:
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #44]
	cbz	r3, .L205
	mov	r1, r6
	mov	r0, r4
	blx	r3
.L205:
	add	r3, r4, #32
	pld	[r3]
	.syntax unified
@ 224 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r2, [r3]
	sub	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	add	r3, r5, #80
	pld	[r3]
	.syntax unified
@ 224 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_sub
1:	ldrex	r2, [r3]
	sub	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L213:
	.align	2
.L212:
	.word	.LANCHOR1+87
	.word	.LC14
	.word	mpp_dev_debug
	.word	.LC15
	.fnend
	.size	mpp_free_task, .-mpp_free_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	kref_put.constprop.24, %function
kref_put.constprop.24:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	pld	[r0]
	.syntax unified
@ 224 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_fetch_sub
1:	ldrex	r4, [r0]
	sub	r2, r4, #1
	strex	r1, r2, [r0]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r4, #1
	bne	.L215
	bl	mpp_free_task
.L214:
	mov	r0, r4
	pop	{r4, pc}
.L215:
	cmp	r4, #0
	bgt	.L217
	movs	r1, #3
	bl	refcount_warn_saturate
.L217:
	movs	r4, #0
	b	.L214
	.fnend
	.size	kref_put.constprop.24, .-kref_put.constprop.24
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_clear.constprop.10, %function
mpp_session_clear.constprop.10:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r4, r0
	ldr	r1, [r4, #20]!
	ldr	r5, [r1], #-4
	subs	r5, r5, #4
.L219:
	adds	r6, r1, #4
	cmp	r6, r4
	bne	.L220
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L220:
	addw	r0, r1, #2204
	bl	atomic_add.constprop.23
	mov	r0, r6
	bl	list_del_init
	addw	r0, r1, #2252
	bl	kref_put.constprop.24
	ldr	r3, [r5, #4]
	mov	r1, r5
	subs	r5, r3, #4
	b	.L219
	.fnend
	.size	mpp_session_clear.constprop.10, .-mpp_session_clear.constprop.10
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_process_request, %function
mpp_process_request:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r10, r3
	ldr	r7, .L340
	mov	r4, r1
	mov	r5, r0
	mov	r6, r2
	ldr	r3, [r7]
	lsls	r1, r3, #27
	bpl	.L222
	ldr	r1, [r2]
	ldr	r0, .L340+4
	bl	printk
.L222:
	ldr	r1, [r6]
	movw	r3, #257
	cmp	r1, r3
	beq	.L223
	cmp	r1, #258
	bcs	.L224
	cmp	r1, #1
	beq	.L225
	cmp	r1, #0
	beq	.L226
	cmp	r1, #2
	beq	.L227
	cmp	r1, #256
	beq	.L228
.L229:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	bne	.L266
	ldr	r3, [r5]
	movw	r2, #1455
	ldr	r1, .L340+8
	ldr	r0, .L340+12
	str	r3, [sp]
	ldr	r3, [r5, #28]
	bl	printk
	b	.L339
.L224:
	cmp	r1, #516
	bcs	.L230
	cmp	r1, #512
	bcs	.L231
	cmp	r1, #258
	bne	.L229
	cmp	r5, #0
	beq	.L338
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq	.L338
	cmp	r3, #120
	bls	.L260
	movs	r2, #120
	ldr	r1, .L340+8
	str	r2, [sp]
	movw	r2, #1407
	ldr	r0, .L340+16
	mvn	r4, #11
	bl	printk
	b	.L221
.L230:
	cmp	r1, #768
	beq	.L233
	cmp	r1, #1024
	bne	.L229
	bl	ktime_get
	ldr	r3, .L340+20
	adds	r8, r0, r3
	mov	r3, #0
	adc	r9, r1, r3
.L265:
	ldr	r3, [r5, #32]
	cmp	r3, #0
	beq	.L262
	bl	ktime_get
	cmp	r8, r0
	sbcs	r3, r9, r1
	bge	.L263
	ldr	r3, [r5, #32]
	cmp	r3, #0
	beq	.L262
	movw	r2, #1439
	ldr	r1, .L340+8
	ldr	r0, .L340+24
	mvn	r4, #109
	bl	printk
	b	.L221
.L226:
	ldr	r3, [r7]
	ldr	r4, [r4, #80]
	lsls	r2, r3, #27
	bpl	.L235
	mov	r1, r4
	ldr	r0, .L340+28
	bl	printk
.L235:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	mov	r2, r4
	ldr	r1, [r3, #8]
.L335:
	ldr	r0, [r6, #16]
.L337:
	subs	r1, r1, #1
	.syntax unified
@ 1311 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__put_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r0, #0
	beq	.L338
.L240:
	mvn	r4, #13
.L221:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L225:
	cbz	r5, .L238
	ldr	r3, [r5, #12]
	cmp	r3, #0
	bne	.L239
.L238:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r0, [r6, #16]
	ldr	r1, [r3, #8]
	subs	r1, r1, #1
	.syntax unified
@ 1324 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__get_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r5, r2
	cmp	r0, #0
	bne	.L240
	ldr	r3, [r7]
	lsls	r3, r3, #27
	bpl	.L241
	mov	r1, r2
	ldr	r0, .L340+32
	bl	printk
.L241:
	.syntax unified
@ 87 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
	cmp	r5, #29
	sbc	r2, r5, r5
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r2, r2, r5
	add	r1, r4, #80
	cmp	r2, #0
	mov	r3, r2
	it	lt
	addlt	r3, r2, #31
	asrs	r3, r3, #5
	ldr	r3, [r1, r3, lsl #2]
	and	r1, r2, #31
	lsrs	r3, r3, r1
	lsls	r0, r3, #31
	bmi	.L243
.L339:
	mvn	r4, #21
	b	.L221
.L243:
	adds	r2, r2, #86
	ldr	r3, [r4, r2, lsl #2]
	cmp	r3, #0
	beq	.L339
.L239:
	ldr	r3, [r3, #4]
	ldr	r4, [r3, #4]
	ldr	r3, [r7]
	lsls	r1, r3, #27
	bpl	.L245
	ldr	r1, [r4, #8]
	ldr	r0, .L340+36
	bl	printk
.L245:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r2, [r4, #8]
	ldr	r1, [r3, #8]
	b	.L335
.L341:
	.align	2
.L340:
	.word	mpp_dev_debug
	.word	.LC16
	.word	.LANCHOR1+118
	.word	.LC25
	.word	.LC22
	.word	500000000
	.word	.LC24
	.word	.LC17
	.word	.LC18
	.word	.LC19
.L227:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r0, [r6, #16]
	ldr	r1, [r3, #8]
	subs	r1, r1, #1
	.syntax unified
@ 1344 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__get_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, r2
	cmp	r0, #0
	bne	.L339
	cmp	r2, #512
	ldr	r4, [r6, #16]
	beq	.L268
	bhi	.L247
	cbz	r2, .L269
	cmp	r2, #256
	beq	.L270
.L248:
	movw	r2, #1289
	ldr	r1, .L342
	ldr	r0, .L342+4
	bl	printk
	movs	r2, #0
.L246:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	mov	r0, r4
	ldr	r1, [r3, #8]
	b	.L337
.L247:
	cmp	r2, #768
	beq	.L271
	cmp	r2, #1024
	bne	.L248
	movw	r2, #1029
	b	.L246
.L268:
	movw	r2, #517
	b	.L246
.L269:
	movs	r2, #3
	b	.L246
.L270:
	movw	r2, #259
	b	.L246
.L271:
	movw	r2, #769
	b	.L246
.L228:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r0, [r6, #16]
	ldr	r1, [r3, #8]
	subs	r1, r1, #1
	.syntax unified
@ 1353 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__get_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r6, r2
	cmp	r0, #0
	bne	.L240
	ldr	r3, [r7]
	lsls	r2, r3, #27
	bpl	.L249
	mov	r1, r6
	ldr	r0, .L342+8
	bl	printk
.L249:
	cmp	r6, #28
	bls	.L250
	movs	r3, #29
	movw	r2, #1359
	ldr	r1, .L342+12
	ldr	r0, .L342+16
	bl	printk
	b	.L339
.L250:
	.syntax unified
@ 87 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
	cmp	r6, #29
	sbc	r2, r6, r6
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r2, r2, r6
	add	r3, r2, #86
	ldr	r6, [r4, r3, lsl #2]
	cmp	r6, #0
	beq	.L339
	str	r2, [r5]
	str	r6, [r5, #12]
	ldr	r3, [r6, #12]
	cbz	r3, .L252
	ldr	r3, [r3]
	cbz	r3, .L253
	str	r3, [r5, #208]
.L253:
	ldr	r3, [r6, #12]
	ldr	r3, [r3, #4]
	cbz	r3, .L254
	str	r3, [r5, #212]
.L254:
	ldr	r3, [r6, #12]
	ldr	r3, [r3, #8]
	cbz	r3, .L252
	str	r3, [r5, #216]
.L252:
	add	r0, r6, #76
	bl	atomic_fetch_add_relaxed.constprop.19
	str	r0, [r5, #4]
	ldr	r3, [r6, #12]
	cbnz	r3, .L256
.L257:
	ldr	r1, [r6, #96]
	mov	r0, r5
	bl	mpp_session_attach_workqueue
.L338:
	movs	r4, #0
	b	.L221
.L256:
	ldr	r3, [r3, #52]
	cmp	r3, #0
	beq	.L257
	mov	r0, r5
	blx	r3
	mov	r4, r0
	cmp	r0, #0
	beq	.L257
	b	.L221
.L223:
	ldr	r5, [r5, #12]
	cmp	r5, #0
	beq	.L339
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r0, [r6, #16]
	ldr	r1, [r3, #8]
	subs	r1, r1, #1
	.syntax unified
@ 1396 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__get_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r4, r0
	cmp	r0, #0
	bne	.L240
	ldr	r3, [r5, #64]
	ldr	r0, [r3, #8]
	cmp	r0, #0
	beq	.L338
	mov	r1, #1496
	bl	regmap_write
	b	.L221
.L260:
	mov	r2, r3
	ldr	r1, [r6, #16]
	add	r0, r5, #44
	bl	_copy_from_user
	cbz	r0, .L261
	movw	r2, #1413
	ldr	r1, .L342+12
	ldr	r0, .L342+20
	bl	printk
	b	.L339
.L261:
	ldr	r3, [r6, #8]
	lsrs	r3, r3, #1
	str	r3, [r5, #40]
	b	.L338
.L231:
	ldr	r2, [r6, #4]
	ldr	r3, [r10, #44]
	orrs	r3, r3, r2
	str	r3, [r10, #44]
	ldr	r3, [r10, #52]
	adds	r3, r3, #1
	str	r3, [r10, #52]
	b	.L338
.L233:
	ldr	r2, [r6, #4]
	ldr	r3, [r10, #44]
	orrs	r3, r3, r2
	str	r3, [r10, #44]
	ldr	r3, [r10, #56]
	adds	r3, r3, #1
	str	r3, [r10, #56]
	b	.L338
.L263:
	mov	r1, #1000
	movs	r0, #251
	bl	usleep_range
	b	.L265
.L262:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	beq	.L339
	mov	r0, r5
	bl	mpp_session_clear.constprop.10
	b	.L338
.L266:
	ldr	r3, [r3, #12]
	ldr	r4, [r3, #48]
	cbz	r4, .L267
	mov	r1, r6
	mov	r0, r5
	mov	r3, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	bx	r3
.L267:
	ldr	r3, [r7]
	lsls	r3, r3, #27
	bpl	.L338
	ldr	r0, .L342+24
	bl	printk
	b	.L221
.L343:
	.align	2
.L342:
	.word	.LANCHOR1+101
	.word	.LC20
	.word	.LC18
	.word	.LANCHOR1+118
	.word	.LC21
	.word	.LC23
	.word	.LC26
	.fnend
	.size	mpp_process_request, .-mpp_process_request
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_deinit_default, %function
mpp_session_deinit_default:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r3, [r0, #12]
	cbz	r3, .L345
	ldr	r3, [r3, #12]
	ldr	r3, [r3, #56]
	cbz	r3, .L346
	blx	r3
.L346:
	mov	r0, r4
	bl	mpp_session_clear.constprop.10
.L345:
	ldr	r5, [r4, #8]
	cbz	r5, .L347
	add	r5, r5, #700
	mov	r0, r5
	bl	mutex_lock
	add	r0, r4, #168
	bl	list_del_init
	mov	r0, r5
	bl	mutex_unlock
.L347:
	add	r0, r4, #176
	pop	{r3, r4, r5, lr}
	b	list_del_init
	.fnend
	.size	mpp_session_deinit_default, .-mpp_session_deinit_default
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_task_msgs, %function
get_task_msgs:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r5, r0
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, [r0, #232]
	add	r3, r5, #232
	add	r6, r5, #224
	cmp	r0, r3
	beq	.L358
	subs	r4, r0, #8
	beq	.L358
	mov	r1, r6
	bl	list_move_tail
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L357:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L358:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L366
	mov	r1, #3520
	ldr	r0, [r3, #36]
	bl	kmem_cache_alloc
	movs	r1, #0
	mov	r3, #-1
	mov	r4, r0
	str	r0, [r4]
	str	r0, [r4, #4]
	str	r5, [r0, #16]
	str	r1, [r0, #28]
	str	r3, [r0, #32]
	strd	r1, r1, [r0, #20]
	bl	list_del_init
	strd	r1, r1, [r0, #44]
	strd	r1, r1, [r0, #52]
	adds	r0, r0, #8
	str	r0, [r4, #8]
	str	r0, [r4, #12]
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r1, r6
	bl	list_move_tail
	ldr	r3, [r5, #220]
	adds	r3, r3, #1
	str	r3, [r5, #220]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L366+4
	ldr	r3, [r3]
	lsls	r3, r3, #22
	bpl	.L357
	ldr	r3, [r5, #220]
	mov	r2, #308
	ldr	r1, .L366+8
	ldr	r0, .L366+12
	str	r3, [sp, #4]
	ldr	r3, [r5, #4]
	str	r3, [sp]
	mov	r3, r5
	bl	printk
	b	.L357
.L367:
	.align	2
.L366:
	.word	kmalloc_caches
	.word	mpp_dev_debug
	.word	.LANCHOR1+138
	.word	.LC27
	.fnend
	.size	get_task_msgs, .-get_task_msgs
	.align	1
	.global	mpp_taskqueue_get_pending_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_taskqueue_get_pending_task, %function
mpp_taskqueue_get_pending_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0, #52]!
	cmp	r3, r0
	ite	ne
	subne	r0, r3, #20
	moveq	r0, #0
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	mpp_taskqueue_get_pending_task, .-mpp_taskqueue_get_pending_task
	.align	1
	.global	mpp_taskqueue_get_running_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_taskqueue_get_running_task, %function
mpp_taskqueue_get_running_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0, #60]!
	cmp	r3, r0
	ite	ne
	subne	r0, r3, #20
	moveq	r0, #0
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	mpp_taskqueue_get_running_task, .-mpp_taskqueue_get_running_task
	.align	1
	.global	mpp_taskqueue_pop_running
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_taskqueue_pop_running, %function
mpp_taskqueue_pop_running:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, [r1]
	push	{r4, lr}
	.save {r4, lr}
	cbz	r3, .L377
	ldr	r3, [r3, #12]
	cbz	r3, .L377
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r0, r1, #20
	bl	list_del_init
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	addw	r0, r1, #2252
	bl	kref_put.constprop.24
	movs	r0, #0
.L374:
	pop	{r4, pc}
.L377:
	mvn	r0, #21
	b	.L374
	.fnend
	.size	mpp_taskqueue_pop_running, .-mpp_taskqueue_pop_running
	.thumb_set mpp_taskqueue_pop_pending,mpp_taskqueue_pop_running
	.align	1
	.global	mpp_taskqueue_trigger_work
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_taskqueue_trigger_work, %function
mpp_taskqueue_trigger_work:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r1, r0, #16
	ldr	r0, [r0, #96]
	b	kthread_queue_work
	.fnend
	.size	mpp_taskqueue_trigger_work, .-mpp_taskqueue_trigger_work
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_ioctl, %function
mpp_dev_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r1
	ldr	r7, .L472
	.pad #84
	sub	sp, sp, #84
	mov	r10, r2
	ldr	r6, [r0, #128]
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L380
	movw	r2, #1692
	ldr	r1, .L472+4
	ldr	r0, .L472+8
	bl	printk
.L380:
	cbz	r6, .L381
	ldr	r3, [r6, #8]
	cbnz	r3, .L382
.L381:
	mov	r3, r6
	movw	r2, #1695
	ldr	r1, .L472+4
	mvn	r4, #21
	ldr	r0, .L472+12
	bl	printk
.L379:
	mov	r0, r4
	add	sp, sp, #84
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L382:
	ldr	r2, [r6, #36]
	cmp	r2, #0
	ble	.L384
	ldr	r3, [r7]
	lsls	r6, r3, #27
	bpl	.L421
	ldr	r0, .L472+16
.L471:
	bl	printk
.L421:
	mvn	r4, #15
	b	.L379
.L384:
	ldr	r3, [r3, #84]
	cmp	r3, #0
	ble	.L385
	ldr	r3, [r7]
	lsls	r5, r3, #27
	bpl	.L421
	ldr	r0, .L472+20
	b	.L471
.L385:
	ldr	r3, .L472+24
	add	r9, sp, #32
	str	r9, [sp, #32]
	cmp	r4, r3
	str	r9, [sp, #36]
	beq	.L422
	mov	r3, r4
	movw	r2, #1497
	ldr	r1, .L472+28
	ldr	r0, .L472+32
	bl	printk
.L470:
	mvn	r4, #21
.L387:
	mov	r3, r4
	movw	r2, #1714
	ldr	r1, .L472+4
	ldr	r0, .L472+36
	bl	printk
	b	.L407
.L422:
	movs	r5, #0
.L386:
	movs	r2, #24
	mov	r1, r10
	add	r0, sp, #56
	bl	_copy_from_user
	cbnz	r0, .L393
	ldr	r3, [r7]
	add	r10, r10, #24
	lsls	r4, r3, #27
	bpl	.L389
	ldr	r3, [sp, #68]
	ldr	r1, [sp, #56]
	ldr	r0, .L472+40
	str	r3, [sp]
	ldrd	r2, r3, [sp, #60]
	bl	printk
.L389:
	ldr	r2, [sp, #56]
	mov	r0, r2
	bl	mpp_check_cmd_v1
	mov	r4, r0
	cbz	r0, .L390
	mov	r3, r2
	ldr	r1, .L472+28
	mov	r2, #1512
	ldr	r0, .L472+44
	bl	printk
.L393:
	mvn	r4, #13
	b	.L387
.L390:
	ldr	r3, [sp, #60]
	lsls	r0, r3, #31
	ite	mi
	ubfxmi	fp, r3, #1, #1
	movpl	fp, #1
	cmp	r2, #516
	bne	.L392
	ldr	r3, [sp, #72]
	movs	r2, #16
	add	r0, sp, #40
	mov	r1, r3
	str	r3, [sp, #16]
	bl	_copy_from_user
	cmp	r0, #0
	bne	.L393
	ldr	r3, [sp, #40]
	and	r3, r3, #1
	strd	r3, r0, [sp, #8]
	ldrd	r2, [sp, #8]
	orrs	r3, r2, r3
	bne	.L394
	ldr	r0, [sp, #48]
	bl	__fdget
	bics	r8, r0, #3
	and	r3, r0, #3
	str	r3, [sp, #20]
	bne	.L395
	mvn	r3, #8
	movw	r2, #1541
	str	r3, [sp, #28]
	ldr	r1, .L472+28
	ldr	r3, [sp, #48]
	ldr	r0, .L472+48
	bl	printk
	mov	r3, sp
	bic	r3, r3, #8160
	ldr	r0, [sp, #16]
	bic	r3, r3, #31
	adds	r0, r0, #12
	ldr	r3, [r3, #8]
	.syntax unified
@ 177 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/include/linux/uaccess.h" 1
	.syntax unified
adds r2, r0, #4; sbcscc r2, r2, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbz	r3, .L396
.L398:
	mov	r2, #1544
	ldr	r1, .L472+28
	ldr	r0, .L472+52
	bl	printk
.L394:
	cmp	fp, #0
	beq	.L386
.L407:
	ldr	r1, [sp, #32]
	mov	r10, #0
	mov	fp, r10
	ldr	r8, [r1]
.L408:
	cmp	r1, r9
	bne	.L413
	cmp	fp, #0
	beq	.L414
	cmp	r10, #0
	beq	.L414
	.syntax unified
@ 39 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		cpsie i			@ arch_local_irq_enable
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, fp
	bl	mpp_taskqueue_trigger_work
.L414:
	ldr	r5, [sp, #32]
	ldr	r10, .L472+76
	ldr	fp, .L472+80
	ldr	r6, [r5]
.L415:
	cmp	r5, r9
	bne	.L419
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L379
	mov	r2, #1720
	ldr	r1, .L472+4
	ldr	r0, .L472+56
	bl	printk
	b	.L379
.L396:
	movs	r2, #4
	add	r1, sp, #28
	bl	arm_copy_to_user
	cmp	r0, #0
	bne	.L398
	b	.L394
.L395:
	cbz	r5, .L399
	ldr	r3, [r5, #48]
	cbz	r3, .L400
	mov	r1, r9
	mov	r0, r5
	bl	task_msgs_add
.L399:
	ldr	r6, [r8, #128]
	mov	r0, r6
	bl	get_task_msgs
	ldr	r3, [r8, #128]
	cmp	r6, r3
	it	eq
	ldreq	r3, [sp, #48]
	mov	r5, r0
	str	r8, [r0, #36]
	it	eq
	streq	r3, [r0, #32]
	ldr	r3, [sp, #20]
	str	r3, [r0, #40]
	ldr	r3, [r7]
	lsls	r1, r3, #27
	bpl	.L394
	ldr	r3, [r6, #220]
	ldr	r2, [r6, #4]
	ldr	r1, [sp, #48]
	ldr	r0, .L472+60
	bl	printk
	b	.L394
.L400:
	mov	r0, r5
	bl	put_task_msgs
	b	.L399
.L392:
	cbnz	r5, .L404
	mov	r0, r6
	bl	get_task_msgs
	mov	r5, r0
	cbnz	r0, .L404
	ldr	r2, [r6, #4]
	mov	r1, r6
	ldr	r0, .L472+64
	bl	printk
	b	.L470
.L404:
	ldr	r2, [r5, #48]
	cmp	r2, #15
	bls	.L405
	movs	r3, #16
	ldr	r1, .L472+28
	strd	r2, r3, [sp]
	movw	r2, #1589
	ldr	r3, [r6, #4]
	ldr	r0, .L472+68
	bl	printk
	b	.L470
.L405:
	adds	r3, r2, #1
	str	r3, [r5, #48]
	movs	r3, #20
	ldr	r1, [sp, #56]
	muls	r2, r3, r2
	mov	r0, r6
	adds	r3, r5, r2
	adds	r2, r2, #60
	add	r8, r3, #60
	add	r2, r2, r5
	str	r1, [r3, #60]
	ldr	r1, [sp, #60]
	str	r1, [r3, #64]
	ldr	r1, [sp, #64]
	str	r1, [r3, #68]
	ldr	r1, [sp, #68]
	str	r1, [r3, #72]
	ldr	r1, [sp, #72]
	str	r1, [r3, #76]
	mov	r3, r5
	ldr	r1, [r6, #8]
	bl	mpp_process_request
	mov	r4, r0
	cbz	r0, .L406
	str	r0, [sp, #4]
	movw	r2, #1603
	ldr	r3, [r8]
	ldr	r1, .L472+28
	ldr	r0, .L472+72
	str	r3, [sp]
	ldr	r3, [r6, #4]
	bl	printk
	b	.L387
.L406:
	cmp	fp, #0
	beq	.L386
	mov	r1, r9
	mov	r0, r5
	bl	task_msgs_add
	b	.L407
.L473:
	.align	2
.L472:
	.word	mpp_dev_debug
	.word	.LANCHOR1+152
	.word	.LC12
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	1074034177
	.word	.LANCHOR1+166
	.word	.LC31
	.word	.LC40
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.word	.LC13
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	.LANCHOR1+183
	.word	.LC42
.L413:
	ldr	r0, [r1, #52]
	cbz	r0, .L409
	ldr	r5, [r1, #20]
	cbz	r5, .L409
	ldr	r3, [r1, #28]
	cmp	r5, r10
	ldr	r6, [r1, #24]
	str	r3, [sp, #16]
	beq	.L410
	cmp	r10, #0
	beq	.L411
	cmp	fp, #0
	beq	.L411
	.syntax unified
@ 39 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		cpsie i			@ arch_local_irq_enable
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, fp
	bl	mpp_taskqueue_trigger_work
.L411:
	.syntax unified
@ 49 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		cpsid i			@ arch_local_irq_disable
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	fp, [sp, #16]
	mov	r10, r5
.L410:
	ldr	r1, [r6, #2200]
	lsls	r2, r1, #22
	bpl	.L412
	ldr	r1, [r6, #2276]
	ldr	r0, .L474
	bl	printk
.L412:
	ldr	r0, [sp, #16]
	adds	r5, r5, #52
	adds	r0, r0, #80
	bl	atomic_add.constprop.23
	addw	r1, r6, #2200
	movs	r0, #0
	bl	____atomic_set_bit
	ldr	r1, [r5, #4]
	add	r0, r6, #20
	str	r0, [r5, #4]
	strd	r5, r1, [r6, #20]
	str	r0, [r1]
.L409:
	mov	r1, r8
	ldr	r8, [r8]
	b	.L408
.L419:
	ldr	r3, [r5, #56]
	cbz	r3, .L417
	ldr	r8, [r5, #16]
	mov	r1, r5
	mov	r0, r8
	bl	mpp_wait_result
	cbz	r0, .L417
	str	r0, [sp]
	movw	r2, #1676
	ldr	r3, [r8, #4]
	mov	r1, r10
	mov	r0, fp
	bl	printk
.L417:
	mov	r0, r5
	mov	r5, r6
	bl	put_task_msgs
	ldr	r6, [r6]
	b	.L415
.L475:
	.align	2
.L474:
	.word	.LC41
	.fnend
	.size	mpp_dev_ioctl, .-mpp_dev_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_chnl_add_req, %function
mpp_chnl_add_req:
	.fnstart
	@ args = 0, pretend = 0, frame = 392
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #404
	sub	sp, sp, #404
	mov	r4, r1
	mov	r6, r0
	mov	r2, #380
	movs	r1, #0
	add	r0, sp, #20
	ldr	fp, .L504+20
	bl	memset
	adds	r4, r4, #24
	mov	r10, #20
.L486:
	ldr	r7, [sp, #68]
	add	r3, sp, #20
	ldr	r1, [r4, #-24]
	mul	r0, r10, r7
	add	r5, r0, #60
	add	r0, r0, r3
	add	r9, r5, r3
	str	r1, [r0, #60]
	ldr	r2, [r4, #-20]
	str	r2, [r0, #64]
	ldr	r3, [r4, #-16]
	str	r3, [r0, #68]
	ldr	ip, [r4, #-12]
	str	ip, [r0, #72]
	ldr	r5, [r4, #-8]
	str	r5, [r0, #76]
	ldr	r0, [fp]
	lsls	r5, r0, #27
	bpl	.L477
	str	ip, [sp]
	ldr	r0, .L504
	bl	printk
.L477:
	add	r3, sp, #20
	mla	r8, r10, r7, r3
	ldr	r2, [r8, #60]
	mov	r0, r2
	bl	mpp_check_cmd_v1
	cmp	r0, #0
	beq	.L478
	mov	r3, r2
	ldr	r1, .L504+4
	movw	r2, #1250
	ldr	r0, .L504+8
	bl	printk
.L489:
	mvn	r5, #13
	b	.L476
.L503:
	add	r3, sp, #20
	mov	r2, r9
	ldr	r1, [r6, #8]
	mov	r0, r6
	bl	mpp_process_request
	mov	r5, r0
	cmp	r0, #0
	bne	.L489
	ldr	r3, [r8, #64]
	lsls	r0, r3, #31
	bpl	.L480
	lsls	r1, r3, #30
	bmi	.L480
.L485:
	add	r3, sp, #20
	mla	r7, r10, r7, r3
	ldr	r3, [r7, #64]
	lsls	r2, r3, #31
	bpl	.L476
	lsls	r3, r3, #30
	add	r4, r4, #24
	bpl	.L486
	b	.L476
.L480:
	ldr	r3, [sp, #64]
	str	r3, [r6, #164]
	ldr	r3, [sp, #72]
	cbz	r3, .L483
	add	r1, sp, #20
	mov	r0, r6
	bl	mpp_process_task
	cbnz	r0, .L490
	ldrd	r9, r2, [sp, #44]
	str	r0, [sp, #12]
	add	r0, r2, #80
	ldr	r8, [sp, #40]
	str	r2, [sp, #8]
	bl	atomic_add.constprop.23
	addw	r1, r9, #2200
	ldr	r3, [sp, #12]
	mov	r0, r3
	bl	____atomic_set_bit
	ldr	r3, [r6, #204]
	ldr	r2, [sp, #8]
	cbnz	r3, .L483
	ldr	r1, [r8, #56]
	add	r0, r9, #20
	add	r3, r8, #52
	str	r0, [r8, #56]
	strd	r3, r1, [r9, #20]
	str	r0, [r1]
	mov	r0, r2
	bl	mpp_taskqueue_trigger_work
.L483:
	ldr	r3, [sp, #76]
	cmp	r3, #0
	beq	.L485
	add	r1, sp, #20
	mov	r0, r6
	bl	mpp_wait_result
	cmp	r0, #0
	beq	.L485
	mov	r5, r0
	b	.L476
.L490:
	mov	r5, r0
	b	.L476
.L478:
	ldr	r3, [sp, #68]
	adds	r3, r3, #1
	str	r3, [sp, #68]
	cmp	r3, #16
	bls	.L503
	movs	r2, #16
	ldr	r1, .L504+12
	str	r2, [sp]
	movw	r2, #2537
	ldr	r0, .L504+16
	mvn	r5, #21
	bl	printk
.L476:
	mov	r0, r5
	add	sp, sp, #404
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L505:
	.align	2
.L504:
	.word	.LC43
	.word	.LANCHOR1+197
	.word	.LC44
	.word	.LANCHOR1+214
	.word	.LC45
	.word	mpp_dev_debug
	.fnend
	.size	mpp_chnl_add_req, .-mpp_chnl_add_req
	.align	1
	.global	mpp_power_on
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_power_on, %function
mpp_power_on:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	add	r3, r0, #124
	movs	r2, #1
	pld	[r3]
	.syntax unified
@ 66 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/cmpxchg.h" 1
	@	__xchg4
1:	ldrex	r1, [r3]
	strex	r0, r2, [r3]
	teq	r0, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r1, .L508
	movs	r1, #4
	ldr	r0, [r4]
	bl	__pm_runtime_resume
	ldr	r0, [r4]
	bl	mpp_pm_stay_awake
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #8]
	cbz	r3, .L508
	mov	r0, r4
	blx	r3
.L508:
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	mpp_power_on, .-mpp_power_on
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_chnl_register, %function
mpp_chnl_register:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	uxth	r3, r2
	lsrs	r2, r2, #16
	str	r1, [r0, #240]
	str	r3, [r0, #196]
	str	r2, [r0, #204]
	cbz	r2, .L514
	ldr	r3, [r0, #12]
	movs	r2, #1
	str	r2, [r3, #128]
	ldr	r0, [r0, #12]
	bl	mpp_power_on
.L514:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	mpp_chnl_register, .-mpp_chnl_register
	.align	1
	.global	mpp_power_off
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_power_off, %function
mpp_power_off:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r3, [r0, #128]
	cbnz	r3, .L520
	add	r2, r0, #124
	pld	[r2]
	.syntax unified
@ 66 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/cmpxchg.h" 1
	@	__xchg4
1:	ldrex	r1, [r2]
	strex	r5, r3, [r2]
	teq	r5, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cbz	r1, .L520
	ldr	r3, [r0, #8]
	ldr	r3, [r3, #12]
	cbz	r3, .L522
	blx	r3
.L522:
	ldr	r0, [r4]
	bl	mpp_pm_relax
	ldr	r0, [r4, #96]
	bl	mpp_taskqueue_get_pending_task
	cbz	r0, .L523
.L524:
	ldr	r5, [r4]
	bl	ktime_get_mono_fast_ns
	strd	r0, [r5, #248]
	movs	r1, #13
.L531:
	ldr	r0, [r4]
	bl	__pm_runtime_suspend
.L520:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L523:
	ldr	r0, [r4, #96]
	bl	mpp_taskqueue_get_running_task
	cmp	r0, #0
	bne	.L524
	movs	r1, #4
	b	.L531
	.fnend
	.size	mpp_power_off, .-mpp_power_off
	.align	1
	.global	mpp_session_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_deinit, %function
mpp_session_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r0
	ldr	r5, .L551
	ldr	r6, [r0, #32]
	ldr	r2, [r5]
	lsls	r2, r2, #9
	bpl	.L533
	ldr	r2, [r0, #4]
	mov	r1, r0
	mov	r3, r6
	ldr	r0, .L551+4
	bl	printk
.L533:
	cmp	r6, #0
	bne	.L544
	ldr	r3, [r4, #216]
	cbz	r3, .L535
	mov	r0, r4
	blx	r3
.L536:
	ldr	r3, [r5]
	lsls	r3, r3, #9
	bpl	.L537
	ldr	r2, [r4, #4]
	mov	r1, r4
	ldr	r0, .L551+8
	bl	printk
.L537:
	mov	r5, sp
	strd	r5, r5, [sp]
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r8, r4
	ldr	r0, [r8, #224]!
	ldr	r6, [r0], #-8
	subs	r6, r6, #8
.L538:
	adds	r0, r0, #8
	cmp	r0, r8
	bne	.L539
	mov	r8, r4
	ldr	r0, [r8, #232]!
	ldr	r6, [r0], #-8
	subs	r6, r6, #8
.L540:
	adds	r0, r0, #8
	cmp	r0, r8
	bne	.L541
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, [sp]
	ldr	r6, [r0], #-8
	subs	r6, r6, #8
.L542:
	add	r3, r0, #8
	cmp	r5, r3
	bne	.L543
	mov	r0, r4
	bl	kfree
	movs	r0, #0
.L532:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L535:
	ldr	r0, .L551+12
	bl	printk
	b	.L536
.L539:
	mov	r1, r5
	bl	list_move_tail
	ldr	r3, [r6, #8]
	mov	r0, r6
	sub	r6, r3, #8
	b	.L538
.L541:
	mov	r1, r5
	bl	list_move_tail
	ldr	r3, [r6, #8]
	mov	r0, r6
	sub	r6, r3, #8
	b	.L540
.L543:
	bl	kfree
	ldr	r3, [r6, #8]
	mov	r0, r6
	sub	r6, r3, #8
	b	.L542
.L544:
	mov	r0, #-1
	b	.L532
.L552:
	.align	2
.L551:
	.word	mpp_dev_debug
	.word	.LC46
	.word	.LC48
	.word	.LC47
	.fnend
	.size	mpp_session_deinit, .-mpp_session_deinit
	.align	1
	.global	mpp_session_pop_pending
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_pop_pending, %function
mpp_session_pop_pending:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	adds	r0, r1, #4
	bl	list_del_init
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	addw	r0, r1, #2252
	bl	kref_put.constprop.24
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	mpp_session_pop_pending, .-mpp_session_pop_pending
	.align	1
	.global	mpp_session_clean_detach
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_clean_detach, %function
mpp_session_clean_detach:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r6, r0
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r10, .L576
	add	r9, r0, #40
	ldr	fp, .L576+4
.L555:
	ldr	r3, [r6, #48]
	cbz	r3, .L557
	ldr	r5, [r6, #40]
	cmp	r5, r9
	bne	.L556
.L557:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L556:
	subs	r8, r5, #176
	beq	.L557
	ldr	r3, [r5, #28]
	cbz	r3, .L558
.L559:
	mov	r0, r8
	bl	mpp_session_get_idle_task
	mov	r7, r0
	cbz	r0, .L558
	addw	r0, r0, #2204
	bl	atomic_add.constprop.23
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r0, r7, #20
	bl	list_del_init
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	addw	r0, r7, #2252
	bl	kref_put.constprop.24
	mov	r1, r7
	mov	r0, r8
	bl	mpp_session_pop_pending
	b	.L559
.L558:
	ldr	r3, [r5, #-144]
	cmp	r3, #0
	bne	.L557
	mov	r0, r5
	bl	list_del_init
	ldr	r3, [r6, #48]
	subs	r3, r3, #1
	str	r3, [r6, #48]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r10]
	lsls	r3, r3, #9
	bpl	.L560
	ldr	r3, [r5, #-164]
	ldr	r3, [r3]
	ldr	r1, [r3, #44]
	cbnz	r1, .L561
	ldr	r1, [r3]
.L561:
	ldr	r2, [r6, #48]
	mov	r0, fp
	bl	printk
.L560:
	mov	r0, r8
	bl	mpp_session_deinit
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L555
.L577:
	.align	2
.L576:
	.word	mpp_dev_debug
	.word	.LC49
	.fnend
	.size	mpp_session_clean_detach, .-mpp_session_clean_detach
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_detach_workqueue, %function
mpp_session_detach_workqueue:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r3, [r0, #12]
	cbz	r3, .L578
	ldr	r3, [r3, #96]
	cbz	r3, .L578
	ldr	r3, .L590
	ldr	r3, [r3]
	lsls	r3, r3, #9
	bpl	.L580
	ldr	r2, [r0, #4]
	mov	r1, r0
	ldr	r0, .L590+4
	bl	printk
.L580:
	ldr	r3, [r4, #12]
	ldr	r1, [r3, #96]
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r5, r4, #176
	mov	r0, r5
	bl	list_del_init
	ldr	r3, [r1, #44]
	add	r2, r1, #40
	str	r5, [r1, #44]
	strd	r2, r3, [r4, #176]
	str	r5, [r3]
	ldr	r3, [r1, #48]
	adds	r3, r3, #1
	str	r3, [r1, #48]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r1
	pop	{r4, r5, r6, lr}
	b	mpp_session_clean_detach
.L578:
	pop	{r4, r5, r6, pc}
.L591:
	.align	2
.L590:
	.word	mpp_dev_debug
	.word	.LC50
	.fnend
	.size	mpp_session_detach_workqueue, .-mpp_session_detach_workqueue
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_release, %function
mpp_dev_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r1
	ldr	r5, .L602
	ldr	r4, [r1, #128]
	ldr	r2, [r5]
	lsls	r3, r2, #26
	bpl	.L593
	movw	r2, #1758
	ldr	r1, .L602+4
	ldr	r0, .L602+8
	bl	printk
.L593:
	cbnz	r4, .L594
	movw	r2, #1761
	ldr	r1, .L602+4
	ldr	r0, .L602+12
	bl	printk
	mvn	r0, #21
.L592:
	pop	{r4, r5, r6, pc}
.L594:
	add	r0, r4, #36
	bl	atomic_add.constprop.23
	ldr	r3, [r4, #12]
	mov	r0, r4
	cbz	r3, .L596
	bl	mpp_session_detach_workqueue
.L597:
	ldr	r0, [r5]
	movs	r4, #0
	str	r4, [r6, #128]
	ands	r0, r0, #32
	beq	.L592
	movw	r2, #1775
	ldr	r1, .L602+4
	ldr	r0, .L602+16
	bl	printk
	mov	r0, r4
	b	.L592
.L596:
	bl	mpp_session_deinit
	b	.L597
.L603:
	.align	2
.L602:
	.word	mpp_dev_debug
	.word	.LANCHOR1+231
	.word	.LC12
	.word	.LC51
	.word	.LC13
	.fnend
	.size	mpp_dev_release, .-mpp_dev_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_chnl_release, %function
mpp_chnl_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	ldr	r3, .L612
	ldr	r3, [r3]
	lsls	r3, r3, #26
	bpl	.L605
	mov	r2, #2496
	ldr	r1, .L612+4
	ldr	r0, .L612+8
	bl	printk
.L605:
	cbnz	r5, .L606
	movw	r2, #2498
	ldr	r1, .L612+4
	ldr	r0, .L612+12
	bl	printk
	mvn	r0, #21
.L604:
	pop	{r3, r4, r5, pc}
.L606:
	add	r0, r5, #36
	bl	atomic_add.constprop.23
	ldr	r4, [r5, #12]
	mov	r0, r5
	cbz	r4, .L608
	bl	mpp_session_detach_workqueue
	movs	r0, #0
	b	.L604
.L608:
	bl	mpp_session_deinit
	mov	r0, r4
	b	.L604
.L613:
	.align	2
.L612:
	.word	mpp_dev_debug
	.word	.LANCHOR1+247
	.word	.LC12
	.word	.LC51
	.fnend
	.size	mpp_chnl_release, .-mpp_chnl_release
	.align	1
	.global	mpp_session_get_pending_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_get_pending_task, %function
mpp_session_get_pending_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0, #20]!
	cmp	r3, r0
	ite	ne
	subne	r0, r3, #4
	moveq	r0, #0
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	bx	lr
	.fnend
	.size	mpp_session_get_pending_task, .-mpp_session_get_pending_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_wait_result_default, %function
mpp_wait_result_default:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #52
	sub	sp, sp, #52
	mov	r6, r0
	mov	r9, r1
	bl	mpp_session_get_pending_task
	mov	r4, r0
	cbnz	r0, .L618
	mov	r3, r6
	movw	r2, #1028
	ldr	r1, .L641
	mvn	r5, #4
	ldr	r0, .L641+4
	bl	printk
.L617:
	mov	r0, r5
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L618:
	ldr	r7, [r0, #2292]
	cbnz	r7, .L620
	ldr	r7, [r6, #12]
.L620:
	ldr	r1, [r4, #2200]
	ubfx	r1, r1, #7, #1
	cmp	r1, #0
	bne	.L630
	add	r0, sp, #28
	addw	r8, r4, #2284
	bl	init_wait_entry
	movs	r5, #200
.L625:
	movs	r2, #2
	add	r1, sp, #28
	mov	r0, r8
	bl	prepare_to_wait_event
	ldr	r3, [r4, #2200]
	lsls	r2, r3, #24
	bpl	.L622
	cmp	r5, #0
	it	eq
	moveq	r5, #1
.L624:
	add	r1, sp, #28
	mov	r0, r8
	bl	finish_wait
	cmp	r5, #0
	bgt	.L621
	addw	r0, r4, #2204
	bl	atomic_add.constprop.23
	addw	r1, r4, #2200
	movs	r0, #9
	bl	____atomic_set_bit
	ldr	r2, [r4, #2200]
	ldr	r3, [r6, #32]
	ldr	r1, .L641
	ldr	r0, .L641+8
	str	r2, [sp, #16]
	ldr	r2, [r4, #2276]
	strd	r3, r2, [sp, #8]
	movw	r2, #1044
	ldr	r3, [r6, #4]
	str	r3, [sp, #4]
	ldr	r3, [r6]
	str	r3, [sp]
	ldr	r3, [r6, #28]
	bl	printk
	b	.L627
.L630:
	movs	r5, #200
.L621:
	ldr	r3, [r7, #12]
	ldr	r3, [r3, #40]
	cbz	r3, .L627
	mov	r2, r9
	mov	r1, r4
	mov	r0, r7
	blx	r3
	mov	r5, r0
.L627:
	ldr	r3, .L641+12
	ldr	r3, [r3]
	lsls	r3, r3, #22
	bpl	.L628
	ldr	r3, [r4, #2252]
	movw	r2, #1051
	ldr	r1, .L641
	ldr	r0, .L641+16
	strd	r3, r5, [sp, #12]
	ldr	r3, [r4, #2200]
	str	r3, [sp, #8]
	ldr	r3, [r4, #2276]
	str	r3, [sp, #4]
	ldr	r3, [r6, #4]
	str	r3, [sp]
	ldr	r3, [r6]
	bl	printk
.L628:
	mov	r1, r4
	mov	r0, r6
	bl	mpp_session_pop_pending
	b	.L617
.L622:
	cmp	r5, #0
	beq	.L624
	mov	r0, r5
	bl	schedule_timeout
	mov	r5, r0
	b	.L625
.L642:
	.align	2
.L641:
	.word	.LANCHOR1+264
	.word	.LC52
	.word	.LC53
	.word	mpp_dev_debug
	.word	.LC54
	.fnend
	.size	mpp_wait_result_default, .-mpp_wait_result_default
	.align	1
	.global	mpp_reset_control_get
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_reset_control_get, %function
mpp_reset_control_get:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r2
	ldr	r7, .L657
	.pad #52
	sub	sp, sp, #52
	ldr	r2, .L657+4
	mov	r5, r0
	mov	r8, r1
	ldr	r3, [r7]
	ldr	r0, [r2]	@ unaligned
	ldr	r1, [r2, #4]	@ unaligned
	movs	r2, #24
	str	r3, [sp, #44]
	add	r3, sp, #12
	stmia	r3!, {r0, r1}
	movs	r1, #0
	add	r0, sp, #20
	bl	memset
	ldr	r0, [r5]
	bl	mpp_dev_of_node
	mov	r2, r4
	ldr	r1, .L657+8
	mov	r6, r0
	bl	of_property_match_string
	cmp	r0, #0
	blt	.L644
	movs	r3, #0
	movs	r2, #1
	mov	r1, r4
	strd	r3, r2, [sp]
	ldr	r0, [r5]
	mov	r2, r3
	bl	__devm_reset_control_get
	mov	r4, r0
	cbz	r0, .L643
	bl	reset_control_deassert
.L643:
	ldr	r2, [sp, #44]
	mov	r0, r4
	ldr	r3, [r7]
	cmp	r2, r3
	beq	.L650
	bl	__stack_chk_fail
.L644:
	add	r0, sp, #12
	bl	strlen
	mov	r1, r4
	rsb	r2, r0, #31
	add	r0, sp, #12
	bl	strncat
	ldr	r1, .L657+8
	mov	r0, r6
	add	r2, sp, #12
	bl	of_property_match_string
	cmp	r0, #0
	bge	.L646
	add	r2, sp, #12
	ldr	r1, .L657+12
	ldr	r0, [r5]
	movs	r4, #0
	bl	_dev_err
	b	.L643
.L646:
	ldr	r4, [r5, #100]
	cbnz	r4, .L647
	ldr	r1, .L657+16
	ldr	r0, [r5]
	bl	_dev_err
	b	.L643
.L647:
	add	r9, r4, #4
	add	r8, r4, r8, lsl #2
	mov	r0, r9
	bl	down_write
	ldr	r6, [r8, #20]
	cbnz	r6, .L648
	movs	r3, #1
	mov	r2, r6
	strd	r6, r3, [sp]
	add	r1, sp, #12
	mov	r3, r6
	ldr	r0, [r5]
	bl	__devm_reset_control_get
	mov	r6, r0
	cbz	r0, .L649
	bl	reset_control_deassert
.L649:
	str	r6, [r8, #20]
	ldr	r3, [r5, #96]
	str	r3, [r4, #48]
.L648:
	ldr	r1, [r4, #48]
	ldr	r3, [r5, #96]
	ldr	r2, [r4]
	cmp	r1, r3
	it	ne
	orrne	r2, r2, #1
	ldr	r1, .L657+20
	str	r2, [r4]
	mov	r4, r6
	ldr	r0, [r5]
	bl	_dev_info
	mov	r0, r9
	bl	up_write
	b	.L643
.L650:
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L658:
	.align	2
.L657:
	.word	__stack_chk_guard
	.word	.LANCHOR1+288
	.word	.LC56
	.word	.LC57
	.word	.LC58
	.word	.LC59
	.fnend
	.size	mpp_reset_control_get, .-mpp_reset_control_get
	.align	1
	.global	mpp_taskqueue_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_taskqueue_init, %function
mpp_taskqueue_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r2, #3520
	movs	r1, #132
	bl	devm_kmalloc
	mov	r4, r0
	cbz	r0, .L659
	ldr	r2, .L664
	adds	r0, r0, #72
	ldr	r1, .L664+4
	bl	__mutex_init
	add	r3, r4, #32
	str	r3, [r4, #32]
	str	r3, [r4, #36]
	add	r3, r4, #40
	str	r3, [r4, #40]
	str	r3, [r4, #44]
	add	r3, r4, #52
	str	r3, [r4, #52]
	str	r3, [r4, #56]
	add	r3, r4, #60
	str	r3, [r4, #60]
	str	r3, [r4, #64]
	add	r3, r4, #84
	str	r3, [r4, #84]
	str	r3, [r4, #88]
	add	r3, r4, #92
	str	r3, [r4, #92]
	str	r3, [r4, #96]
	movs	r3, #16
	str	r3, [r4, #100]
	movs	r3, #0
	str	r3, [r4, #104]
.L659:
	mov	r0, r4
	pop	{r4, pc}
.L665:
	.align	2
.L664:
	.word	.LANCHOR2+4
	.word	.LC60
	.fnend
	.size	mpp_taskqueue_init, .-mpp_taskqueue_init
	.align	1
	.global	mpp_check_req
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_check_req, %function
mpp_check_req:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r5, r0
	ldr	r4, [r5, #12]
	mov	r0, r3
	mov	r6, r2
	ldr	r3, [sp, #24]
	cmp	r4, r1
	bcs	.L667
	mov	r3, r1
	ldr	r0, .L675
	ldr	r1, .L675+4
	movw	r2, #1795
	str	r4, [sp]
.L673:
	bl	printk
	b	.L674
.L667:
	ldr	r2, [r5, #8]
	subs	r4, r4, r1
	adds	r1, r4, r2
	cmp	r1, r0
	bcs	.L669
	strd	r2, r0, [sp]
	mov	r3, r4
	movw	r2, #1801
	ldr	r1, .L675+4
	ldr	r0, .L675+8
	bl	printk
.L674:
	mvn	r0, #21
.L666:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L669:
	cmp	r6, r3
	bcs	.L670
	str	r6, [sp]
	movw	r2, #1806
	ldr	r1, .L675+4
	ldr	r0, .L675+12
	b	.L673
.L670:
	cmp	r4, r6
	ble	.L671
	str	r6, [sp]
	mov	r3, r4
	movw	r2, #1811
	ldr	r1, .L675+4
	ldr	r0, .L675+16
	b	.L673
.L671:
	cmp	r1, r6
	bls	.L672
	strd	r2, r6, [sp]
	mov	r3, r4
	ldr	r1, .L675+4
	mov	r2, #1816
	ldr	r0, .L675+20
	bl	printk
	ldr	r1, [r5, #8]
	subs	r1, r1, r6
	add	r4, r4, r1
	str	r4, [r5, #8]
.L672:
	movs	r0, #0
	b	.L666
.L676:
	.align	2
.L675:
	.word	.LC61
	.word	.LANCHOR1+320
	.word	.LC62
	.word	.LC63
	.word	.LC64
	.word	.LC65
	.fnend
	.size	mpp_check_req, .-mpp_check_req
	.align	1
	.global	mpp_extract_reg_offset_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_extract_reg_offset_info, %function
mpp_extract_reg_offset_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	ldr	r2, [r1, #8]
	ldr	r0, [r0]
	lsrs	r5, r2, #3
	adds	r3, r5, r0
	cmp	r3, #60
	bls	.L678
	movs	r3, #60
	movw	r2, #1831
	strd	r0, r3, [sp]
	mov	r3, r5
	ldr	r1, .L683
	ldr	r0, .L683+4
	bl	printk
.L682:
	mvn	r0, #21
.L677:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L678:
	cmp	r2, #0
	add	r0, r4, r0, lsl #3
	add	r0, r0, #4
	ldr	r1, [r1, #16]
	bge	.L680
.L681:
	movw	r2, #1836
	ldr	r1, .L683
	ldr	r0, .L683+8
	bl	printk
	b	.L682
.L680:
	bl	_copy_from_user
	cmp	r0, #0
	bne	.L681
	ldr	r2, [r4]
	add	r2, r2, r5
	str	r2, [r4]
	b	.L677
.L684:
	.align	2
.L683:
	.word	.LANCHOR1+334
	.word	.LC66
	.word	.LC23
	.fnend
	.size	mpp_extract_reg_offset_info, .-mpp_extract_reg_offset_info
	.align	1
	.global	mpp_query_reg_offset_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_query_reg_offset_info, %function
mpp_query_reg_offset_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L696
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #26
	bpl	.L686
	movw	r2, #1847
	ldr	r1, .L696+4
	ldr	r0, .L696+8
	bl	printk
	cbnz	r4, .L687
.L694:
	ldr	r3, [r5]
	lsls	r3, r3, #26
	bpl	.L695
	mov	r2, #1856
	ldr	r1, .L696+4
	ldr	r0, .L696+12
	bl	printk
.L695:
	movs	r0, #0
	b	.L685
.L690:
	adds	r3, r3, #1
.L692:
	cmp	r1, r3
	beq	.L694
	ldr	r0, [r2, r3, lsl #3]
	cmp	r0, r6
	bne	.L690
	add	r4, r4, r3, lsl #3
	ldr	r0, [r4, #8]
.L685:
	pop	{r4, r5, r6, pc}
.L686:
	cmp	r0, #0
	beq	.L695
.L687:
	mov	r2, r4
	movs	r3, #0
	ldr	r1, [r2], #4
	b	.L692
.L697:
	.align	2
.L696:
	.word	mpp_dev_debug
	.word	.LANCHOR1+362
	.word	.LC12
	.word	.LC13
	.fnend
	.size	mpp_query_reg_offset_info, .-mpp_query_reg_offset_info
	.align	1
	.global	mpp_translate_reg_offset_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_translate_reg_offset_info, %function
mpp_translate_reg_offset_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r7, r1
	ldr	r4, .L712
	mov	r8, r2
	ldr	r3, [r4]
	lsls	r1, r3, #26
	bpl	.L699
	movw	r2, #1865
	ldr	r1, .L712+4
	ldr	r0, .L712+8
	bl	printk
	cbnz	r7, .L700
.L706:
	ldr	r3, [r4]
	lsls	r2, r3, #26
	bpl	.L711
	movw	r2, #1877
	ldr	r1, .L712+4
	ldr	r0, .L712+12
	bl	printk
.L711:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L705:
	lsls	r3, r2, #28
	bpl	.L703
	ldrd	r1, r2, [r5, #4]
	mov	r0, r9
	bl	printk
.L703:
	ldr	r2, [r5, #4]
	adds	r6, r6, #1
	ldr	r1, [r5, #8]!
	ldr	r3, [r8, r2, lsl #2]
	add	r3, r3, r1
	str	r3, [r8, r2, lsl #2]
.L704:
	ldr	r3, [r7]
	ldr	r2, [r4]
	cmp	r3, r6
	bhi	.L705
	b	.L706
.L699:
	cmp	r7, #0
	beq	.L711
.L700:
	mov	r5, r7
	movs	r6, #0
	ldr	r9, .L712+16
	b	.L704
.L713:
	.align	2
.L712:
	.word	mpp_dev_debug
	.word	.LANCHOR1+388
	.word	.LC12
	.word	.LC13
	.word	.LC67
	.fnend
	.size	mpp_translate_reg_offset_info, .-mpp_translate_reg_offset_info
	.align	1
	.global	mpp_task_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_task_init, %function
mpp_task_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	adds	r3, r1, #4
	str	r3, [r1, #4]
	str	r3, [r1, #8]
	add	r3, r1, #20
	str	r3, [r1, #20]
	str	r3, [r1, #24]
	add	r3, r1, #28
	str	r3, [r1, #28]
	str	r3, [r1, #32]
	movs	r3, #0
	str	r0, [r1]
	mov	r0, r3
	str	r3, [r1, #2200]
	str	r3, [r1, #36]
	bx	lr
	.fnend
	.size	mpp_task_init, .-mpp_task_init
	.align	1
	.global	mpp_task_finalize
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_task_finalize, %function
mpp_task_finalize:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	str	r0, [r1, #2200]
	bx	lr
	.fnend
	.size	mpp_task_finalize, .-mpp_task_finalize
	.align	1
	.global	mpp_task_dump_mem_region
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_task_dump_mem_region, %function
mpp_task_dump_mem_region:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r6, r0
	mov	r4, r1
	cbz	r1, .L721
	movw	r2, #1934
	ldr	r1, .L723
	ldr	r0, .L723+4
	add	r5, r4, #28
	bl	printk
	ldr	r3, [r4, #28]
	cmp	r5, r3
	beq	.L718
	ldr	r3, [r4, #28]
	ldr	r6, .L723
	ldr	r7, .L723+8
	ldr	r4, [r3]
.L719:
	cmp	r5, r3
	bne	.L720
.L722:
	movs	r0, #0
.L716:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L720:
	ldr	r2, [r3, #12]
	mov	r1, r6
	mov	r0, r7
	str	r2, [sp, #4]
	add	r2, r3, #8
	str	r2, [sp]
	movw	r2, #1940
	ldr	r3, [r3, #16]
	bl	printk
	mov	r3, r4
	ldr	r4, [r4]
	b	.L719
.L718:
	ldr	r1, .L723+12
	ldr	r0, [r6]
	bl	_dev_err
	b	.L722
.L721:
	mvn	r0, #4
	b	.L716
.L724:
	.align	2
.L723:
	.word	.LANCHOR1+418
	.word	.LC68
	.word	.LC69
	.word	.LC70
	.fnend
	.size	mpp_task_dump_mem_region, .-mpp_task_dump_mem_region
	.align	1
	.global	mpp_task_dump_reg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_task_dump_reg, %function
mpp_task_dump_reg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r5, r1
	cbz	r1, .L731
	ldr	r3, .L735
	ldr	r3, [r3]
	lsls	r3, r3, #21
	bmi	.L727
.L728:
	movs	r0, #0
.L725:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L727:
	movw	r2, #1955
	ldr	r1, .L735+4
	ldr	r0, .L735+8
	bl	printk
	ldr	r3, [r5, #2280]
	cmp	r3, #0
	beq	.L728
	ldr	r3, [r5, #2272]
	ldr	r7, .L735+4
	ldr	r8, .L735+12
	ldrd	r4, r6, [r3, #12]
.L729:
	cmp	r4, r6
	bhi	.L728
	ldr	r2, [r5, #2280]
	lsls	r3, r4, #2
	mov	r1, r7
	mov	r0, r8
	ldr	r2, [r2, r4, lsl #2]
	strd	r3, r2, [sp]
	mov	r3, r4
	movw	r2, #1965
	adds	r4, r4, #1
	bl	printk
	b	.L729
.L731:
	mvn	r0, #4
	b	.L725
.L736:
	.align	2
.L735:
	.word	mpp_dev_debug
	.word	.LANCHOR1+443
	.word	.LC71
	.word	.LC72
	.fnend
	.size	mpp_task_dump_reg, .-mpp_task_dump_reg
	.align	1
	.global	mpp_task_dump_hw_reg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_task_dump_hw_reg, %function
mpp_task_dump_hw_reg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L743
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r5, r0
	ldr	r3, [r3]
	lsls	r3, r3, #21
	bpl	.L742
	ldr	r3, [r0, #4]
	movw	r2, #1980
	ldr	r1, .L743+4
	ldr	r0, .L743+8
	ldr	r3, [r3, #4]
	ldr	r7, .L743+4
	ldr	r8, .L743+12
	ldrd	r4, r6, [r3, #12]
	bl	printk
.L739:
	cmp	r4, r6
	bls	.L740
.L742:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L740:
	ldr	r3, [r5, #60]
	lsls	r2, r4, #2
	add	r3, r3, r2
	.syntax unified
@ 113 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/io.h" 1
	ldr r3, [r3]
@ 0 "" 2
	.thumb
	.syntax unified
	strd	r2, r3, [sp]
	mov	r1, r7
	mov	r3, r4
	movw	r2, #1985
	mov	r0, r8
	adds	r4, r4, #1
	bl	printk
	b	.L739
.L744:
	.align	2
.L743:
	.word	mpp_dev_debug
	.word	.LANCHOR1+461
	.word	.LC73
	.word	.LC72
	.fnend
	.size	mpp_task_dump_hw_reg, .-mpp_task_dump_hw_reg
	.align	1
	.global	mpp_dev_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_probe, %function
mpp_dev_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	add	r5, r1, #16
	ldr	r3, [r0, #4]
	movs	r7, #1
	movs	r6, #0
	.pad #20
	sub	sp, sp, #20
	mov	r4, r0
	mov	r9, r1
	ldr	r8, [r3, #4]
	adds	r0, r0, #132
	strh	r7, [r0, #-96]	@ movhi
	str	r7, [r0, #-92]
	str	r5, [r0, #-132]
	ldr	r2, [r3, #12]
	ldr	r1, .L801
	str	r2, [r0, #-124]
	ldr	r3, [r3, #16]
	ldr	r2, .L801+4
	str	r6, [r0, #-8]
	str	r3, [r0, #-120]
	str	r6, [r0, #-4]
	str	r6, [r0, #16]
	bl	__init_rwsem
	mov	r0, r5
	strd	r6, r6, [sp, #8]
	bl	mpp_dev_of_node
	mov	r2, r6
	ldr	r1, .L801+8
	bl	of_parse_phandle
	mov	r10, r0
	cbnz	r0, .L746
.L748:
	ldr	r1, .L801+12
.L798:
	mov	r0, r5
	bl	_dev_err
.L747:
	ldr	r1, .L801+16
	mov	r0, r5
	bl	_dev_err
	mvn	r6, #18
.L745:
	mov	r0, r6
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L746:
	bl	of_device_is_available
	cmp	r0, #0
	beq	.L748
	mov	r0, r10
	bl	of_find_device_by_node
	cbnz	r0, .L749
	ldr	r1, .L801+20
	b	.L798
.L749:
	ldr	r3, [r0, #80]
	str	r3, [r4, #104]
	bl	platform_device_put
	ldr	r3, [r4, #104]
	cbnz	r3, .L750
	ldr	r1, .L801+24
	b	.L798
.L750:
	mov	r0, r5
	bl	mpp_dev_of_node
	mov	r3, r7
	ldr	r1, .L801+28
	str	r6, [sp]
	add	r2, sp, #8
	bl	of_property_read_variable_u32_array
	cmp	r0, #0
	bge	.L751
	ldr	r1, .L801+32
	b	.L798
.L751:
	ldr	r1, [r4, #104]
	ldr	r2, [sp, #8]
	ldr	r3, [r1, #460]
	cmp	r3, r2
	bhi	.L752
	ldr	r1, .L801+36
.L799:
	mov	r0, r5
	bl	_dev_err
	b	.L747
.L752:
	add	r3, r2, #116
	ldr	r6, [r1, r3, lsl #2]
	cbnz	r6, .L753
	ldr	r1, .L801+40
	mov	r0, r5
	bl	_dev_err
	b	.L747
.L753:
	add	r10, r4, #108
	str	r10, [r4, #108]
	str	r10, [r4, #112]
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	fp, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r7, [r4, #116]
	cmp	r7, #0
	it	lt
	ldrlt	r7, [r6, #128]
	cmp	r7, #3
	bls	.L755
	ldr	r1, .L801+44
	mov	r2, r7
.L797:
	ldr	r0, [r4]
	bl	_dev_err
.L756:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, fp	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r5
	bl	mpp_dev_of_node
	movs	r3, #0
	ldr	r1, .L801+48
	str	r3, [sp]
	add	r2, sp, #12
	movs	r3, #1
	bl	of_property_read_variable_u32_array
	cmp	r0, #0
	blt	.L761
	ldr	r1, [r4, #104]
	ldr	r2, [sp, #12]
	ldr	r3, [r1, #580]
	cmp	r3, r2
	bhi	.L762
	ldr	r1, .L801+52
	b	.L799
.L755:
	add	r3, r6, r7, lsl #2
	ldr	r2, [r3, #108]
	cbz	r2, .L757
	mov	r2, r7
	ldr	r1, .L801+56
	b	.L797
.L757:
	str	r4, [r3, #108]
	add	r1, r6, #124
	ldr	r3, [r6, #128]
	mov	r0, r7
	adds	r3, r3, #1
	str	r3, [r6, #128]
	bl	_set_bit
	ldr	r3, [r6, #96]
	add	r2, r6, #92
	str	r10, [r6, #96]
	strd	r2, r3, [r4, #108]
	str	r10, [r3]
	ldr	r3, .L801+60
	str	r7, [r4, #116]
	str	r6, [r4, #96]
	ldr	r3, [r3]
	lsls	r2, r3, #6
	bpl	.L758
	ldr	r3, [r4]
	ldr	r1, [r3, #44]
	cbnz	r1, .L759
	ldr	r1, [r3]
.L759:
	mov	r2, r7
	ldr	r0, .L801+64
	bl	printk
.L758:
	ldr	r3, [r4, #40]
	ldr	r2, [r6, #100]
	cmp	r2, r3
	it	hi
	strhi	r3, [r6, #100]
	b	.L756
.L762:
	adds	r2, r2, #146
	ldr	r3, [r1, r2, lsl #2]
	str	r3, [r4, #100]
.L761:
	ldr	r6, [r4, #40]
	cmp	r6, #1
	bne	.L764
	mov	r1, #2000
	mov	r0, r5
	bl	pm_runtime_set_autosuspend_delay
	mov	r1, r6
	mov	r0, r5
	bl	__pm_runtime_use_autosuspend
.L765:
	add	r3, r4, #16
	movs	r2, #20
	movs	r1, #0
	movs	r6, #0
	mov	r0, r3
	bl	memset
	ldr	r3, .L801+68
	movs	r1, #1
	str	r3, [r4, #24]
	str	r0, [r4, #16]
	str	r0, [r4, #20]
	mov	r0, r5
	str	r6, [r4, #72]
	str	r6, [r4, #76]
	str	r6, [r4, #80]
	str	r6, [r4, #84]
	bl	mpp_device_init_wakeup
	mov	r0, r5
	bl	pm_runtime_enable
	mov	r1, r6
	mov	r0, r9
	bl	platform_get_irq
	cmp	r0, r6
	str	r0, [r4, #48]
	bge	.L766
	ldr	r1, .L801+72
.L800:
	mov	r0, r5
	mvn	r6, #18
	bl	_dev_err
.L767:
	mov	r0, r4
	bl	mpp_detach_workqueue
	movs	r1, #0
	mov	r0, r5
	bl	mpp_device_init_wakeup
	movs	r1, #1
	mov	r0, r5
	bl	__pm_runtime_disable
	b	.L745
.L764:
	mov	r2, r6
	ldr	r1, .L801+76
	mov	r0, r5
	bl	_dev_info
	b	.L765
.L766:
	mov	r2, r6
	mov	r1, #512
	mov	r0, r9
	bl	platform_get_resource
	mov	r7, r0
	cbnz	r0, .L768
	ldr	r1, .L801+80
	b	.L800
.L768:
	ldrd	r1, r2, [r0]
	mov	r0, r5
	adds	r2, r2, #1
	subs	r2, r2, r1
	bl	devm_ioremap
	str	r0, [r4, #60]
	cbnz	r0, .L769
	mov	r2, r7
	ldr	r1, .L801+84
	mov	r0, r5
	mvn	r6, #11
	bl	_dev_err
	b	.L767
.L769:
	movs	r1, #4
	mov	r0, r5
	bl	__pm_runtime_resume
	ldr	r3, [r4, #8]
	str	r6, [r4, #68]
	ldr	r3, [r3]
	cbz	r3, .L770
	mov	r0, r4
	blx	r3
	mov	r6, r0
	cbz	r0, .L770
	movs	r1, #4
	mov	r0, r5
	bl	__pm_runtime_idle
	b	.L767
.L770:
	ldr	r3, [r8, #4]
	cmp	r3, #0
	blt	.L772
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #8]
	cbz	r3, .L773
	mov	r0, r4
	blx	r3
.L773:
	ldr	r2, [r8, #4]
	ldr	r3, [r4, #60]
	add	r3, r3, r2
	.syntax unified
@ 113 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/io.h" 1
	ldr r6, [r3]
@ 0 "" 2
@ 747 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_common.h" 1
	dsb 
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L801+60
	ldr	r3, [r3]
	lsls	r3, r3, #18
	bpl	.L774
	mov	r3, r6
	lsrs	r1, r2, #2
	ldr	r0, .L801+88
	bl	printk
.L774:
	str	r6, [r8, #8]
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #12]
	cbz	r3, .L772
	mov	r0, r4
	blx	r3
.L772:
	movs	r1, #4
	mov	r0, r5
	bl	__pm_runtime_idle
	movs	r6, #0
	b	.L745
.L802:
	.align	2
.L801:
	.word	.LC74
	.word	.LANCHOR2+4
	.word	.LC75
	.word	.LC76
	.word	.LC88
	.word	.LC77
	.word	.LC78
	.word	.LC79
	.word	.LC80
	.word	.LC81
	.word	.LC82
	.word	.LC83
	.word	.LC86
	.word	.LC87
	.word	.LC84
	.word	mpp_dev_debug
	.word	.LC85
	.word	mpp_task_worker_default
	.word	.LC90
	.word	.LC89
	.word	.LC91
	.word	.LC92
	.word	.LC93
	.fnend
	.size	mpp_dev_probe, .-mpp_dev_probe
	.align	1
	.global	mpp_dev_remove
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_remove, %function
mpp_dev_remove:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r3, [r0, #8]
	ldr	r3, [r3, #4]
	cbz	r3, .L804
	blx	r3
.L804:
	ldr	r0, [r4, #68]
	bl	mpp_iommu_remove
	mov	r0, r4
	bl	mpp_detach_workqueue
	movs	r1, #0
	ldr	r0, [r4]
	bl	mpp_device_init_wakeup
	ldr	r0, [r4]
	movs	r1, #1
	bl	__pm_runtime_disable
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	mpp_dev_remove, .-mpp_dev_remove
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.global	mpp_dev_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_shutdown, %function
mpp_dev_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	add	r5, r0, #16
	ldr	r4, [r0, #80]
	ldr	r1, .L820
	mov	r0, r5
	bl	_dev_info
	ldr	r0, [r4, #104]
	adds	r0, r0, #84
	bl	atomic_add.constprop.23
	bl	ktime_get
	ldr	r3, .L820+4
	adds	r6, r0, r3
	mov	r3, #0
	adc	r7, r1, r3
.L812:
	ldr	r3, [r4, #80]
	cbz	r3, .L809
	bl	ktime_get
	cmp	r6, r0
	sbcs	r3, r7, r1
	bge	.L810
	ldr	r3, [r4, #80]
	cbz	r3, .L809
	ldr	r2, [r4, #80]
	mov	r0, r5
	ldr	r1, .L820+8
	pop	{r3, r4, r5, r6, r7, lr}
	b	_dev_err
.L810:
	movw	r1, #20000
	movw	r0, #5001
	bl	usleep_range
	b	.L812
.L809:
	mov	r0, r5
	ldr	r1, .L820+12
	pop	{r3, r4, r5, r6, r7, lr}
	b	_dev_info
.L821:
	.align	2
.L820:
	.word	.LC94
	.word	200000000
	.word	.LC95
	.word	.LC96
	.fnend
	.size	mpp_dev_shutdown, .-mpp_dev_shutdown
	.text
	.align	1
	.global	mpp_dev_register_srv
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_register_srv, %function
mpp_dev_register_srv:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r3, [r0, #4]
	ldr	r3, [r3]
	add	r2, r3, #86
	str	r0, [r1, r2, lsl #2]
	mov	r0, r3
	adds	r1, r1, #80
	bl	_set_bit
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	mpp_dev_register_srv, .-mpp_dev_register_srv
	.align	1
	.global	mpp_dev_irq
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_irq, %function
mpp_dev_irq:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r1
	ldr	r3, [r1, #12]
	ldr	r5, [r1, #88]
	ldr	r4, [r3, #28]
	cbz	r4, .L824
	mov	r0, r1
	blx	r4
	mov	r4, r0
	cbz	r5, .L825
	cmp	r0, #2
	bne	.L823
	addw	r7, r5, #2200
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #2200]
	orr	r1, r3, #8
	str	r1, [r5, #2200]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	lsls	r2, r3, #28
	bpl	.L827
	ldr	r3, [r6, #52]
	movw	r2, #2159
	ldr	r1, .L835
	movs	r4, #1
	ldr	r0, .L835+4
	bl	printk
.L823:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L827:
	add	r0, r5, #2208
	bl	cancel_delayed_work
	mov	r1, r7
	movs	r0, #4
	bl	____atomic_set_bit
	b	.L823
.L826:
.L824:
	cmp	r5, #0
	bne	.L823
	mov	r4, r5
.L825:
	ldr	r3, .L835+8
	ldr	r3, [r3]
	lsls	r3, r3, #16
	bpl	.L823
	ldr	r0, .L835+12
	bl	printk
	b	.L823
.L836:
	.align	2
.L835:
	.word	.LANCHOR1+482
	.word	.LC97
	.word	mpp_dev_debug
	.word	.LC98
	.fnend
	.size	mpp_dev_irq, .-mpp_dev_irq
	.align	1
	.global	mpp_dev_isr_sched
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_isr_sched, %function
mpp_dev_isr_sched:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r1
	ldrb	r3, [r1, #36]	@ zero_extendqisi2
	cbz	r3, .L838
	ldr	r3, [r1, #8]
	ldr	r2, [r3, #24]
	cbz	r2, .L838
	ldr	r3, [r1, #96]
	ldr	r1, [r3, #52]!
	cmp	r1, r3
	bne	.L838
	mov	r0, r4
	blx	r2
.L838:
	ldr	r3, [r4, #12]
	ldr	r5, [r3, #32]
	cbz	r5, .L839
	mov	r0, r4
	blx	r5
	mov	r5, r0
.L839:
	mov	r0, r4
	bl	mpp_taskqueue_trigger_work
	mov	r0, r5
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_dev_isr_sched, .-mpp_dev_isr_sched
	.align	1
	.global	mpp_get_grf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_get_grf, %function
mpp_get_grf:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	movs	r3, #0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	str	r3, [sp, #4]
	mov	r3, r0
	cbz	r0, .L848
	ldr	r0, [r0, #8]
	cbz	r0, .L848
	ldr	r2, [r3, #4]
	cbz	r2, .L848
	add	r2, sp, #4
	ldr	r1, [r3]
	bl	regmap_read
.L848:
	ldrh	r0, [sp, #4]
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
	.fnend
	.size	mpp_get_grf, .-mpp_get_grf
	.align	1
	.global	mpp_grf_is_changed
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_grf_is_changed, %function
mpp_grf_is_changed:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbz	r0, .L859
	ldr	r3, [r0, #8]
	cbz	r3, .L862
	ldr	r3, [r0, #4]
	cbz	r3, .L862
	bl	mpp_get_grf
	ldrh	r3, [r4, #4]
	subs	r0, r3, r0
	it	ne
	movne	r0, #1
.L859:
	pop	{r4, pc}
.L862:
	mov	r0, r3
	b	.L859
	.fnend
	.size	mpp_grf_is_changed, .-mpp_grf_is_changed
	.align	1
	.global	mpp_set_grf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_set_grf, %function
mpp_set_grf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r3, r0
	cbz	r0, .L864
	ldr	r0, [r0, #8]
	cbz	r0, .L864
	ldr	r2, [r3, #4]
	cbz	r2, .L864
	ldr	r1, [r3]
	bl	regmap_write
.L864:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	mpp_set_grf, .-mpp_set_grf
	.align	1
	.global	mpp_dev_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_reset, %function
mpp_dev_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r5, .L918
	ldr	r3, [r5]
	lsls	r2, r3, #13
	bpl	.L875
	ldr	r0, .L918+4
	bl	printk
.L875:
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #32]
	cmp	r3, #0
	beq	.L876
	mov	r0, r4
	blx	r3
.L877:
	ldrb	r3, [r4, #36]	@ zero_extendqisi2
	cbz	r3, .L878
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #24]
	cbz	r3, .L878
	mov	r0, r4
	blx	r3
.L878:
	ldr	r0, [r4, #68]
	cbz	r0, .L879
	bl	down_write
.L879:
	ldr	r0, [r4, #100]
	cbz	r0, .L880
	ldr	r3, [r0]
	cbz	r3, .L880
	adds	r0, r0, #4
	bl	down_write
.L880:
	movs	r3, #0
	str	r3, [r4, #72]
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #28]
	cbz	r3, .L881
	mov	r0, r4
	blx	r3
.L881:
	ldr	r1, [r4]
	ldr	r0, [r4, #68]
	bl	mpp_iommu_refresh
	ldr	r0, [r4, #100]
	cbz	r0, .L882
	ldr	r3, [r0]
	cbz	r3, .L882
	adds	r0, r0, #4
	bl	up_write
.L882:
	ldr	r0, [r4, #68]
	cbz	r0, .L883
	bl	up_write
.L883:
	ldr	r3, [r5]
	lsls	r3, r3, #13
	bpl	.L884
	ldr	r0, .L918+8
	bl	printk
.L884:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L876:
	ldr	r0, [r4, #64]
	bl	mpp_set_grf
	b	.L877
.L919:
	.align	2
.L918:
	.word	mpp_dev_debug
	.word	.LC99
	.word	.LC100
	.fnend
	.size	mpp_dev_reset, .-mpp_dev_reset
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_task_timeout_work, %function
mpp_task_timeout_work:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	sub	r5, r0, #2208
	mov	r7, r0
	ldr	r9, [r5, #2308]
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0, #-8]
	orr	r1, r3, #8
	str	r1, [r0, #-8]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	lsls	r3, r3, #28
	bpl	.L921
	movw	r2, #618
	ldr	r1, .L943
	ldr	r0, .L943+4
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	b	printk
.L921:
	ldr	r3, [r5, #2276]
	movw	r2, #622
	ldr	r1, .L943
	ldr	r0, .L943+8
	bl	printk
	ldr	r6, [r5]
	cbnz	r6, .L922
	ldr	r1, .L943
	mov	r3, r5
	ldr	r0, .L943+12
	movw	r2, #625
.L942:
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	b	printk
.L922:
	ldr	r3, [r6, #12]
	cbnz	r3, .L923
	mov	r3, r6
	movw	r2, #631
	ldr	r1, .L943
	ldr	r0, .L943+16
	b	.L942
.L923:
	ldr	r4, [r5, #2292]
	cmp	r4, #0
	it	eq
	moveq	r4, r3
	ldr	r3, [r4, #4]
	ldr	r3, [r3, #16]
	ldr	r3, [r3, #64]
	cbz	r3, .L929
	mov	r0, r4
	blx	r3
.L929:
	ldr	r0, [r4, #100]
	cbz	r0, .L926
	ldr	r3, [r0]
	cbz	r3, .L926
	adds	r0, r0, #4
	bl	up_read
.L926:
	sub	r8, r7, #8
	mov	r0, r4
	bl	mpp_dev_reset
	mov	r0, r4
	bl	mpp_power_off
	mov	r1, r8
	movs	r0, #6
	bl	____atomic_set_bit
	movs	r0, #7
	bl	____atomic_set_bit
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	add	r0, r7, #76
	bl	__wake_up
	mov	r1, r5
	ldr	r0, [r4, #96]
	bl	mpp_taskqueue_pop_running
	ldr	r3, [r6, #240]
	cbz	r3, .L920
	cmp	r9, #0
	beq	.L920
	ldr	r0, [r6, #196]
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	bx	r3	@ indirect register sibling call
.L920:
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L944:
	.align	2
.L943:
	.word	.LANCHOR1+494
	.word	.LC101
	.word	.LC102
	.word	.LC14
	.word	.LC103
	.fnend
	.size	mpp_task_timeout_work, .-mpp_task_timeout_work
	.align	1
	.global	mpp_task_finish
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_task_finish, %function
mpp_task_finish:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	ldr	r4, [r1, #2292]
	mov	r5, r1
	cbnz	r4, .L946
	ldr	r4, [r0, #12]
.L946:
	ldr	r3, [r4, #12]
	ldr	r8, [r5, #2308]
	ldr	r3, [r3, #36]
	cbz	r3, .L947
	mov	r1, r5
	mov	r0, r4
	blx	r3
.L947:
	ldr	r0, [r4, #100]
	cbz	r0, .L948
	ldr	r3, [r0]
	cbz	r3, .L948
	adds	r0, r0, #4
	bl	up_read
.L948:
	ldr	r3, [r4, #72]
	cmp	r3, #0
	ble	.L949
	mov	r0, r4
	bl	mpp_dev_reset
.L949:
	addw	r7, r5, #2200
	mov	r0, r4
	bl	mpp_power_off
	movs	r0, #5
	mov	r1, r7
	bl	____atomic_set_bit
	movs	r0, #7
	bl	____atomic_set_bit
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	addw	r0, r5, #2284
	bl	__wake_up
	mov	r1, r5
	ldr	r0, [r4, #96]
	bl	mpp_taskqueue_pop_running
	ldr	r3, [r6, #240]
	cbz	r3, .L950
	cmp	r8, #0
	beq	.L950
	ldr	r0, [r6, #196]
	blx	r3
.L950:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
	.fnend
	.size	mpp_task_finish, .-mpp_task_finish
	.align	1
	.global	mpp_time_record
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_time_record, %function
mpp_time_record:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r3, .L974
	ldr	r3, [r3]
	lsls	r3, r3, #23
	bpl	.L967
	cbz	r0, .L967
	add	r0, r0, #2256
	bl	ktime_get_real_ts64
.L967:
	movs	r0, #0
	pop	{r3, pc}
.L975:
	.align	2
.L974:
	.word	mpp_dev_debug
	.fnend
	.size	mpp_time_record, .-mpp_time_record
	.align	1
	.global	mpp_chnl_run_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_chnl_run_task, %function
mpp_chnl_run_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r0
	ldr	r10, [r0, #12]
	.pad #20
	sub	sp, sp, #20
	mov	r6, r1
	ldr	r9, [r10, #96]
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	fp, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L1022
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #512
	beq	.L985
	ldr	r3, [r0, #196]
	movw	r2, #941
	ldr	r1, .L1022+4
	ldr	r0, .L1022+8
	bl	printk
.L985:
	mov	r0, r5
	bl	mpp_session_get_idle_task
	mov	r4, r0
	cmp	r0, #0
	beq	.L995
	ldr	r7, [r10, #148]
	cmp	r7, #0
	bne	.L996
	cbz	r6, .L980
	ldr	r3, [r6, #8]
	ldr	r1, [r0, #2324]
	adds	r2, r3, #7
	bic	r2, r2, #7
	cmp	r2, r1
	bne	.L981
	ldr	r2, [r6, #12]
	ldr	r0, [r0, #2328]
	adds	r2, r2, #7
	bic	r2, r2, #7
	cmp	r2, r0
	beq	.L980
.L981:
	ldr	r2, [r8]
	lsls	r0, r2, #13
	bpl	.L983
	ldr	r2, [r4, #2328]
	ldr	r0, .L1022+12
	strd	r1, r2, [sp, #4]
	ldr	r2, [r6, #12]
	ldr	r1, [r5, #196]
	str	r2, [sp]
	ldr	r2, [r4, #2276]
	bl	printk
.L983:
	addw	r0, r4, #2204
	bl	atomic_add.constprop.23
	addw	r1, r4, #2200
	movs	r0, #7
	bl	____atomic_set_bit
	mov	r1, r4
	mov	r0, r9
	bl	mpp_taskqueue_pop_pending
	ldr	r3, [r5, #240]
	cbz	r3, .L980
	ldr	r0, [r5, #196]
	blx	r3
.L980:
	ldr	r3, [r4, #2204]
	cmp	r3, #0
	ble	.L984
	mov	r1, r4
	mov	r0, r9
	bl	mpp_taskqueue_pop_pending
	b	.L985
.L984:
	ldr	r3, [r10, #12]
	ldr	r3, [r3, #20]
	cbz	r3, .L986
	mov	r1, r4
	mov	r0, r10
	blx	r3
	mov	r4, r0
	cbnz	r0, .L987
.L989:
	ldr	r3, [r8]
	lsls	r2, r3, #22
	bmi	.L988
.L1021:
	mvn	r7, #15
.L978:
	mov	r0, r9
	bl	mpp_session_clean_detach
	ldr	r3, [r8]
	lsls	r3, r3, #22
	bpl	.L994
	ldr	r3, [r5, #196]
	movw	r2, #1014
	ldr	r1, .L1022+4
	ldr	r0, .L1022+16
	bl	printk
.L994:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, fp	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r7
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L986:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, r9
	ldr	r2, [r3, #60]!
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r2, r3
	bne	.L989
.L987:
	cbz	r6, .L990
	ldr	r3, [r6]
	str	r3, [r4, #2316]
	ldr	r3, [r6, #4]
	str	r3, [r4, #2320]
.L990:
	ldr	r6, [r4, #2292]
	cbnz	r6, .L991
	ldr	r3, [r4]
	ldr	r6, [r3, #12]
.L991:
	mov	r1, r4
	mov	r0, r9
	bl	mpp_taskqueue_pending_to_run
	ldr	r0, [r6, #100]
	cbz	r0, .L992
	ldr	r3, [r0]
	cbz	r3, .L992
	adds	r0, r0, #4
	bl	down_read
.L992:
	add	r0, r6, #132
	bl	down_read
	mov	r0, r4
	bl	mpp_time_record
	ldr	r3, [r6, #12]
	ldr	r3, [r3, #24]
	cbz	r3, .L993
	mov	r1, r4
	mov	r0, r6
	blx	r3
.L993:
	addw	r1, r4, #2200
	movs	r0, #2
	bl	____atomic_set_bit
	b	.L978
.L988:
	ldr	r3, [r5, #196]
	mov	r2, #992
	ldr	r1, .L1022+4
	ldr	r0, .L1022+20
	bl	printk
	b	.L1021
.L995:
	mvn	r7, #10
	b	.L978
.L996:
	movs	r7, #0
	b	.L978
.L1023:
	.align	2
.L1022:
	.word	mpp_dev_debug
	.word	.LANCHOR1+516
	.word	.LC104
	.word	.LC105
	.word	.LC107
	.word	.LC106
	.fnend
	.size	mpp_chnl_run_task, .-mpp_chnl_run_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_task_worker_default, %function
mpp_task_worker_default:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r7, .L1075
	ldr	r6, [r0, #80]
	ldr	r3, [r7]
	lsls	r0, r3, #26
	bpl	.L1025
	movw	r2, #858
	ldr	r1, .L1075+4
	ldr	r0, .L1075+8
	bl	printk
.L1025:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	fp, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r8, .L1075+28
.L1026:
	mov	r0, r6
	bl	mpp_taskqueue_get_pending_task
	mov	r4, r0
	cbz	r0, .L1036
	ldr	r3, [r0, #2204]
	cmp	r3, #0
	ble	.L1028
	mov	r1, r0
	mov	r0, r6
	bl	mpp_taskqueue_pop_pending
	b	.L1026
.L1028:
	ldr	r3, [r0]
	ldr	r0, [r3, #12]
	ldr	r3, [r0, #12]
	ldr	r3, [r3, #20]
	cbz	r3, .L1030
	mov	r1, r4
	blx	r3
	mov	r4, r0
	cbnz	r0, .L1031
.L1036:
	mov	r0, r6
	bl	mpp_session_clean_detach
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, fp	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L1030:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, r6
	ldr	r2, [r3, #60]!
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r2, r3
	bne	.L1036
.L1031:
	ldr	r5, [r4, #2292]
	cbnz	r5, .L1032
	ldr	r3, [r4]
	ldr	r5, [r3, #12]
.L1032:
	addw	r9, r4, #2200
	mov	r1, r4
	mov	r0, r6
	bl	mpp_taskqueue_pending_to_run
	mov	r1, r9
	movs	r0, #1
	bl	____atomic_set_bit
	ldr	r3, [r7]
	ldr	r10, [r4]
	lsls	r1, r3, #26
	bpl	.L1033
	movw	r2, #810
	mov	r1, r8
	ldr	r0, .L1075+8
	bl	printk
.L1033:
	ldr	r3, [r5, #8]
	ldr	r3, [r3, #32]
	cbz	r3, .L1034
	mov	r0, r5
	blx	r3
	cbz	r0, .L1035
	ldr	r0, [r5]
	ldr	r1, .L1075+12
	bl	_dev_err
	mov	r1, r4
	mov	r0, r6
	bl	mpp_taskqueue_pop_running
	b	.L1036
.L1034:
	ldr	r0, [r5, #64]
	bl	mpp_set_grf
.L1035:
	mov	r0, r5
	bl	mpp_power_on
	mov	r0, r4
	bl	mpp_time_record
	ldr	r3, [r7]
	lsls	r2, r3, #22
	bpl	.L1037
	ldr	r2, [r5]
	ldr	r3, [r2, #44]
	cbnz	r3, .L1038
	ldr	r3, [r2]
.L1038:
	ldr	r2, [r4, #2200]
	mov	r1, r8
	ldr	r0, .L1075+16
	str	r2, [sp, #12]
	ldr	r2, [r4, #2276]
	str	r2, [sp, #8]
	ldr	r2, [r10, #4]
	str	r2, [sp, #4]
	ldr	r2, [r10]
	str	r2, [sp]
	movw	r2, #830
	bl	printk
.L1037:
	ldrb	r3, [r5, #36]	@ zero_extendqisi2
	cbz	r3, .L1039
	ldr	r3, [r5, #8]
	ldr	r3, [r3, #20]
	cbz	r3, .L1039
	mov	r1, r4
	mov	r0, r5
	blx	r3
.L1039:
	ldr	r0, [r5, #100]
	cbz	r0, .L1040
	ldr	r3, [r0]
	cbz	r3, .L1040
	adds	r0, r0, #4
	bl	down_read
.L1040:
	mov	r1, r9
	movs	r0, #2
	bl	____atomic_set_bit
	ldr	r1, .L1075+20
	movs	r3, #20
	add	r2, r4, #2208
	movs	r0, #1
	ldr	r1, [r1]
	bl	queue_delayed_work_on
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #24]
	cbz	r3, .L1041
	mov	r1, r4
	mov	r0, r5
	blx	r3
.L1041:
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L1026
	movw	r2, #846
	mov	r1, r8
	ldr	r0, .L1075+24
	bl	printk
	b	.L1026
.L1076:
	.align	2
.L1075:
	.word	mpp_dev_debug
	.word	.LANCHOR1+534
	.word	.LC12
	.word	.LC108
	.word	.LC109
	.word	system_wq
	.word	.LC13
	.word	.LANCHOR1+558
	.fnend
	.size	mpp_task_worker_default, .-mpp_task_worker_default
	.align	1
	.global	mpp_time_diff
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_time_diff, %function
mpp_time_diff:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, fp, lr}
	.save {r4, r5, r6, r7, fp, lr}
	mov	r4, r0
	ldr	r6, [r0, #2292]
	.pad #32
	sub	sp, sp, #32
	ldr	r5, [r0]
	cbnz	r6, .L1078
	ldr	r6, [r5, #12]
.L1078:
	add	r0, sp, #16
	bl	ktime_get_real_ts64
	ldr	r3, .L1084
	ldr	r3, [r3]
	lsls	r3, r3, #23
	bpl	.L1079
	ldr	r3, [r6]
	ldr	r1, [r3, #44]
	cbnz	r1, .L1080
	ldr	r1, [r3]
.L1080:
	ldr	r6, [r4, #2256]
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #20]
	subs	r2, r2, r6
	ldr	r6, .L1084+4
	ldr	r0, [r4, #2260]
	umull	fp, ip, r2, r6
	sbc	r0, r3, r0
	mla	ip, r6, r0, ip
	ldr	r6, [r4, #2264]
	ldr	r0, [sp, #24]
	subs	r0, r0, r6
	ldr	r6, .L1084+8
	smull	r6, r7, r0, r6
	asrs	r0, r0, #31
	rsb	r0, r0, r7, asr #6
	adds	r2, fp, r0
	adc	r3, ip, r0, asr #31
	strd	r2, [sp, #8]
	ldr	r3, [r4, #2276]
	ldr	r0, .L1084+12
	str	r3, [sp]
	ldrd	r2, r3, [r5]
	bl	printk
.L1079:
	movs	r0, #0
	add	sp, sp, #32
	@ sp needed
	pop	{r4, r5, r6, r7, fp, pc}
.L1085:
	.align	2
.L1084:
	.word	mpp_dev_debug
	.word	1000000
	.word	274877907
	.word	.LC110
	.fnend
	.size	mpp_time_diff, .-mpp_time_diff
	.align	1
	.global	mpp_write_req
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_write_req, %function
mpp_write_req:
	.fnstart
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r8, r0
	ldr	r10, .L1094
	mov	r9, r1
	ldr	fp, .L1094+4
	mov	r7, r3
	mov	r4, r2
.L1087:
	cmp	r7, r4
	bhi	.L1090
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L1090:
	ldr	r3, [sp, #40]
	cmp	r3, r4
	beq	.L1088
	ldr	r3, [r10]
	lsls	r6, r4, #2
	ldr	r5, [r9, r4, lsl #2]
	lsls	r3, r3, #19
	bpl	.L1089
	mov	r3, r5
	mov	r2, r6
	bic	r1, r4, #-1073741824
	mov	r0, fp
	bl	printk
.L1089:
	ldr	r3, [r8, #60]
	add	r6, r6, r3
	.syntax unified
@ 95 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/io.h" 1
	str r5, [r6]
@ 0 "" 2
	.thumb
	.syntax unified
.L1088:
	adds	r4, r4, #1
	b	.L1087
.L1095:
	.align	2
.L1094:
	.word	mpp_dev_debug
	.word	.LC111
	.fnend
	.size	mpp_write_req, .-mpp_write_req
	.align	1
	.global	mpp_read_req
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_read_req, %function
mpp_read_req:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r6, r0
	ldr	r8, .L1103
	mov	r7, r1
	ldr	r9, .L1103+4
	mov	r5, r3
	mov	r4, r2
.L1097:
	cmp	r5, r4
	bhi	.L1099
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L1099:
	ldr	r3, [r6, #60]
	lsls	r2, r4, #2
	add	r3, r3, r2
	.syntax unified
@ 113 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/io.h" 1
	ldr r10, [r3]
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r8]
	lsls	r3, r3, #18
	bpl	.L1098
	mov	r3, r10
	bic	r1, r4, #-1073741824
	mov	r0, r9
	bl	printk
.L1098:
	str	r10, [r7, r4, lsl #2]
	adds	r4, r4, #1
	b	.L1097
.L1104:
	.align	2
.L1103:
	.word	mpp_dev_debug
	.word	.LC112
	.fnend
	.size	mpp_read_req, .-mpp_read_req
	.align	1
	.global	mpp_get_clk_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_get_clk_info, %function
mpp_get_clk_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r7, r0
	ldr	r0, [r0]
	mov	r6, r1
	mov	r8, r2
	bl	mpp_dev_of_node
	mov	r2, r8
	ldr	r1, .L1108
	mov	r4, r0
	bl	of_property_match_string
	subs	r5, r0, #0
	blt	.L1107
	mov	r1, r8
	ldr	r0, [r7]
	bl	devm_clk_get
	mov	r3, r6
	mov	r2, r5
	ldr	r1, .L1108+4
	str	r0, [r3], #8
	mov	r0, r4
	bl	of_property_read_u32_index
	add	r3, r6, #12
	mov	r2, r5
	ldr	r1, .L1108+8
	mov	r0, r4
	bl	of_property_read_u32_index
	movs	r0, #0
.L1105:
	pop	{r4, r5, r6, r7, r8, pc}
.L1107:
	mvn	r0, #21
	b	.L1105
.L1109:
	.align	2
.L1108:
	.word	.LC113
	.word	.LC114
	.word	.LC115
	.fnend
	.size	mpp_get_clk_info, .-mpp_get_clk_info
	.align	1
	.global	mpp_set_clk_info_rate_hz
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_set_clk_info_rate_hz, %function
mpp_set_clk_info_rate_hz:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	mov	r3, r1
	ldr	r1, [r0]
	cbz	r1, .L1111
	cbz	r2, .L1111
	cmp	r3, #4
	bhi	.L1112
	tbb	[pc, r3]
.L1114:
	.byte	(.L1118-.L1114)/2
	.byte	(.L1117-.L1114)/2
	.byte	(.L1116-.L1114)/2
	.byte	(.L1115-.L1114)/2
	.byte	(.L1113-.L1114)/2
	.p2align 1
.L1117:
	str	r2, [r0, #4]
.L1111:
	movs	r0, #0
	pop	{r3, pc}
.L1116:
	str	r2, [r0, #20]
	b	.L1111
.L1115:
	str	r2, [r0, #8]
	b	.L1111
.L1113:
	str	r2, [r0, #12]
	b	.L1111
.L1118:
	str	r2, [r0, #16]
	b	.L1111
.L1112:
	mov	r2, #2320
	ldr	r1, .L1125
	ldr	r0, .L1125+4
	bl	printk
	b	.L1111
.L1126:
	.align	2
.L1125:
	.word	.LANCHOR1+571
	.word	.LC116
	.fnend
	.size	mpp_set_clk_info_rate_hz, .-mpp_set_clk_info_rate_hz
	.align	1
	.global	mpp_get_clk_info_rate_hz
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_get_clk_info_rate_hz, %function
mpp_get_clk_info_rate_hz:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, r0
	ldr	r0, [r0]
	cbz	r0, .L1127
	ldr	r0, [r2, #4]
	cbnz	r0, .L1127
	cmp	r1, #3
	beq	.L1129
	cmp	r1, #4
	beq	.L1130
	cmp	r1, #2
	bne	.L1131
	ldr	r0, [r2, #20]
	ldr	r3, .L1136
	cmp	r0, #0
	it	eq
	moveq	r0, r3
	bx	lr
.L1130:
	ldr	r0, [r2, #12]
	cbnz	r0, .L1127
.L1129:
	ldr	r0, [r2, #8]
	cbnz	r0, .L1127
.L1131:
	ldr	r0, [r2, #16]
.L1127:
	bx	lr
.L1137:
	.align	2
.L1136:
	.word	50000000
	.fnend
	.size	mpp_get_clk_info_rate_hz, .-mpp_get_clk_info_rate_hz
	.align	1
	.global	mpp_clk_set_rate
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_clk_set_rate, %function
mpp_clk_set_rate:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	ldr	r4, [r0]
	cbz	r4, .L1140
	bl	mpp_get_clk_info_rate_hz
	cbz	r0, .L1138
	str	r0, [r5, #24]
	mov	r1, r0
	mov	r0, r4
	bl	clk_set_rate
	movs	r0, #0
.L1138:
	pop	{r3, r4, r5, pc}
.L1140:
	mvn	r0, #21
	b	.L1138
	.fnend
	.size	mpp_clk_set_rate, .-mpp_clk_set_rate
	.align	1
	.global	mpp_procfs_create_u32
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_procfs_create_u32, %function
mpp_procfs_create_u32:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	str	r3, [sp]
	ldr	r3, .L1143
	bl	proc_create_data
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L1144:
	.align	2
.L1143:
	.word	.LANCHOR1+596
	.fnend
	.size	mpp_procfs_create_u32, .-mpp_procfs_create_u32
	.global	g_mpp_svr_fn_ops
	.global	g_srv
	.global	rockchip_mpp_fops
	.global	enc_info_item_name
	.global	mpp_device_name
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__func__.40320, %object
	.size	__func__.40320, 26
__func__.40320:
	.ascii	"mpp_session_get_idle_task\000"
	.type	__func__.41626, %object
	.size	__func__.41626, 23
__func__.41626:
	.ascii	"mpp_chnl_get_iova_addr\000"
	.type	__func__.40718, %object
	.size	__func__.40718, 25
__func__.40718:
	.ascii	"mpp_process_task_default\000"
	.type	__func__.41284, %object
	.size	__func__.41284, 13
__func__.41284:
	.ascii	"mpp_dev_open\000"
	.type	__func__.40694, %object
	.size	__func__.40694, 14
__func__.40694:
	.ascii	"mpp_free_task\000"
	.type	__func__.40944, %object
	.size	__func__.40944, 17
__func__.40944:
	.ascii	"mpp_get_cmd_butt\000"
	.type	__func__.41123, %object
	.size	__func__.41123, 20
__func__.41123:
	.ascii	"mpp_process_request\000"
	.type	__func__.40429, %object
	.size	__func__.40429, 14
__func__.40429:
	.ascii	"get_task_msgs\000"
	.type	__func__.41264, %object
	.size	__func__.41264, 14
__func__.41264:
	.ascii	"mpp_dev_ioctl\000"
	.type	__func__.41193, %object
	.size	__func__.41193, 17
__func__.41193:
	.ascii	"mpp_collect_msgs\000"
	.type	__func__.41251, %object
	.size	__func__.41251, 14
__func__.41251:
	.ascii	"mpp_msgs_wait\000"
	.type	__func__.40928, %object
	.size	__func__.40928, 17
__func__.40928:
	.ascii	"mpp_parse_msg_v1\000"
	.type	__func__.41614, %object
	.size	__func__.41614, 17
__func__.41614:
	.ascii	"mpp_chnl_add_req\000"
	.type	__func__.41290, %object
	.size	__func__.41290, 16
__func__.41290:
	.ascii	"mpp_dev_release\000"
	.type	__func__.41604, %object
	.size	__func__.41604, 17
__func__.41604:
	.ascii	"mpp_chnl_release\000"
	.type	__func__.40851, %object
	.size	__func__.40851, 24
__func__.40851:
	.ascii	"mpp_wait_result_default\000"
.LC55:
	.ascii	"shared_\000"
	.space	24
	.type	__func__.41300, %object
	.size	__func__.41300, 14
__func__.41300:
	.ascii	"mpp_check_req\000"
	.type	__func__.41310, %object
	.size	__func__.41310, 28
__func__.41310:
	.ascii	"mpp_extract_reg_offset_info\000"
	.type	__func__.41315, %object
	.size	__func__.41315, 26
__func__.41315:
	.ascii	"mpp_query_reg_offset_info\000"
	.type	__func__.41325, %object
	.size	__func__.41325, 30
__func__.41325:
	.ascii	"mpp_translate_reg_offset_info\000"
	.type	__func__.41354, %object
	.size	__func__.41354, 25
__func__.41354:
	.ascii	"mpp_task_dump_mem_region\000"
	.type	__func__.41377, %object
	.size	__func__.41377, 18
__func__.41377:
	.ascii	"mpp_task_dump_reg\000"
	.type	__func__.41391, %object
	.size	__func__.41391, 21
__func__.41391:
	.ascii	"mpp_task_dump_hw_reg\000"
	.type	__func__.41467, %object
	.size	__func__.41467, 12
__func__.41467:
	.ascii	"mpp_dev_irq\000"
	.type	__func__.40711, %object
	.size	__func__.40711, 22
__func__.40711:
	.ascii	"mpp_task_timeout_work\000"
	.type	__func__.40833, %object
	.size	__func__.40833, 18
__func__.40833:
	.ascii	"mpp_chnl_run_task\000"
	.type	__func__.40763, %object
	.size	__func__.40763, 24
__func__.40763:
	.ascii	"mpp_task_worker_default\000"
	.type	__func__.40750, %object
	.size	__func__.40750, 13
__func__.40750:
	.ascii	"mpp_task_run\000"
	.type	__func__.41538, %object
	.size	__func__.41538, 25
__func__.41538:
	.ascii	"mpp_set_clk_info_rate_hz\000"
	.type	procfs_fops_u32, %object
	.size	procfs_fops_u32, 44
procfs_fops_u32:
	.space	4
	.word	fops_open_u32
	.word	seq_read
	.space	4
	.word	fops_write_u32
	.space	4
	.word	single_release
	.space	16
	.type	rockchip_mpp_fops, %object
	.size	rockchip_mpp_fops, 128
rockchip_mpp_fops:
	.space	40
	.word	mpp_dev_ioctl
	.space	12
	.word	mpp_dev_open
	.space	4
	.word	mpp_dev_release
	.space	60
	.data
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_mpp_svr_fn_ops, %object
	.size	g_mpp_svr_fn_ops, 36
g_mpp_svr_fn_ops:
	.word	mpp_chnl_open
	.word	mpp_chnl_register
	.word	mpp_chnl_release
	.word	mpp_chnl_add_req
	.word	mpp_chnl_get_iova_addr
	.word	0
	.word	mpp_chnl_get_dev
	.word	mpp_chnl_run_task
	.word	mpp_chnl_is_running
	.type	enc_info_item_name, %object
	.size	enc_info_item_name, 44
enc_info_item_name:
	.word	.LC117
	.word	.LC118
	.word	.LC119
	.word	.LC120
	.word	.LC121
	.word	.LC122
	.word	.LC123
	.word	.LC124
	.word	.LC125
	.word	.LC126
	.word	.LC127
	.type	mpp_device_name, %object
	.size	mpp_device_name, 116
mpp_device_name:
	.word	.LC128
	.word	.LC129
	.word	.LC130
	.word	.LC131
	.word	.LC132
	.space	12
	.word	.LC133
	.word	.LC134
	.space	8
	.word	.LC135
	.space	12
	.word	.LC136
	.word	.LC137
	.word	.LC138
	.space	20
	.word	.LC139
	.space	12
	.word	.LC140
	.bss
	.align	2
	.set	.LANCHOR2,. + 0
	.type	g_srv, %object
	.size	g_srv, 4
g_srv:
	.space	4
	.type	__key.40719, %object
	.size	__key.40719, 0
__key.40719:
	.type	__key.40890, %object
	.size	__key.40890, 0
__key.40890:
	.type	__key.41412, %object
	.size	__key.41412, 0
__key.41412:
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016rk_vcodec: %s:%d: task %d %#lx session %p chan"
	.ascii	" %d find session %p\012\000"
.LC1:
	.ascii	"\0016rk_vcodec: session %p:%d attach\012\000"
.LC2:
	.ascii	"\0013rk_vcodec: invalid NULL process task function\012"
	.ascii	"\000"
.LC3:
	.ascii	"\0013rk_vcodec: invalid NULL wait result function\012"
	.ascii	"\000"
.LC4:
	.ascii	"%d\012\000"
.LC5:
	.ascii	"\0013rk_vcodec: %s:%d: session is null\000"
.LC6:
	.ascii	"\0013rk_vcodec: %s:%d: pid %d client %d found inval"
	.ascii	"id process function\012\000"
.LC7:
	.ascii	"\0013rk_vcodec: %s:%d: alloc_task failed.\012\000"
.LC8:
	.ascii	"&task->wait\000"
.LC9:
	.ascii	"\0016rk_vcodec: %s:%d: session %d:%d task %d state "
	.ascii	"0x%lx\012\000"
.LC10:
	.ascii	"\0013rk_vcodec: invalid msgs without session\012\000"
.LC11:
	.ascii	"\0016rk_vcodec: session %p init\012\000"
.LC12:
	.ascii	"\0016rk_vcodec: %s:%d: enter\012\000"
.LC13:
	.ascii	"\0016rk_vcodec: %s:%d: leave\012\000"
.LC14:
	.ascii	"\0013rk_vcodec: %s:%d: task %p, task->session is nu"
	.ascii	"ll.\012\000"
.LC15:
	.ascii	"\0016rk_vcodec: %s:%d: session %d:%d task %d state "
	.ascii	"0x%lx abort_request %d\012\000"
.LC16:
	.ascii	"\0016rk_vcodec: cmd %x process\012\000"
.LC17:
	.ascii	"\0016rk_vcodec: hw_support %08x\012\000"
.LC18:
	.ascii	"\0016rk_vcodec: client %d\012\000"
.LC19:
	.ascii	"\0016rk_vcodec: hw_id %08x\012\000"
.LC20:
	.ascii	"\0013rk_vcodec: %s:%d: unknown dev cmd 0x%x\012\000"
.LC21:
	.ascii	"\0013rk_vcodec: %s:%d: client_type must less than %"
	.ascii	"d\012\000"
.LC22:
	.ascii	"\0013rk_vcodec: %s:%d: init table size %d more than"
	.ascii	" %d\012\000"
.LC23:
	.ascii	"\0013rk_vcodec: %s:%d: copy_from_user failed\012\000"
.LC24:
	.ascii	"\0013rk_vcodec: %s:%d: wait task running time out\012"
	.ascii	"\000"
.LC25:
	.ascii	"\0013rk_vcodec: %s:%d: pid %d not find client %d\012"
	.ascii	"\000"
.LC26:
	.ascii	"\0016rk_vcodec: unknown mpp ioctl cmd %x\012\000"
.LC27:
	.ascii	"\0016rk_vcodec: %s:%d: session %p:%d msgs cnt %d\012"
	.ascii	"\000"
.LC28:
	.ascii	"\0013rk_vcodec: %s:%d: session %p\012\000"
.LC29:
	.ascii	"\0016rk_vcodec: release session had request\012\000"
.LC30:
	.ascii	"\0016rk_vcodec: shutdown had request\012\000"
.LC31:
	.ascii	"\0013rk_vcodec: %s:%d: unknown ioctl cmd %x\012\000"
.LC32:
	.ascii	"\0016rk_vcodec: cmd %x collect flags %08x, size %d,"
	.ascii	" offset %x\012\000"
.LC33:
	.ascii	"\0013rk_vcodec: %s:%d: mpp cmd %x is not supported."
	.ascii	"\012\000"
.LC34:
	.ascii	"\0013rk_vcodec: %s:%d: fd %d get session failed\012"
	.ascii	"\000"
.LC35:
	.ascii	"\0013rk_vcodec: %s:%d: copy_to_user failed.\012\000"
.LC36:
	.ascii	"\0016rk_vcodec: fd %d, session %d msg_cnt %d\012\000"
.LC37:
	.ascii	"\0013rk_vcodec: session %p:%d failed to get task ms"
	.ascii	"gs\000"
.LC38:
	.ascii	"\0013rk_vcodec: %s:%d: session %d message count %d "
	.ascii	"more than %d.\012\000"
.LC39:
	.ascii	"\0013rk_vcodec: %s:%d: session %d process cmd %x re"
	.ascii	"t %d\012\000"
.LC40:
	.ascii	"\0013rk_vcodec: %s:%d: collect msgs failed %d\012\000"
.LC41:
	.ascii	"\0016rk_vcodec: try to trigger abort task %d\012\000"
.LC42:
	.ascii	"\0013rk_vcodec: %s:%d: session %d wait result ret %"
	.ascii	"d\012\000"
.LC43:
	.ascii	"\0016rk_vcodec: cmd %x, flags %08x, size %d, offset"
	.ascii	" %x\012\000"
.LC44:
	.ascii	"\0013rk_vcodec: %s:%d: mpp cmd %x is not supproted."
	.ascii	"\012\000"
.LC45:
	.ascii	"\0013rk_vcodec: %s:%d: fail, message count %d more "
	.ascii	"than %d.\012\000"
.LC46:
	.ascii	"\0016rk_vcodec: session %p:%d task %d release\012\000"
.LC47:
	.ascii	"\0013rk_vcodec: invalid NULL session deinit functio"
	.ascii	"n\012\000"
.LC48:
	.ascii	"\0016rk_vcodec: session %p:%d deinit\012\000"
.LC49:
	.ascii	"\0016rk_vcodec: %s detach count %d\012\000"
.LC50:
	.ascii	"\0016rk_vcodec: session %p:%d detach\012\000"
.LC51:
	.ascii	"\0013rk_vcodec: %s:%d: session is null\012\000"
.LC52:
	.ascii	"\0013rk_vcodec: %s:%d: session %p pending list is e"
	.ascii	"mpty!\012\000"
.LC53:
	.ascii	"\0013rk_vcodec: %s:%d: timeout, pid %d session %d:%"
	.ascii	"d count %d cur_task %d state %lx\012\000"
.LC54:
	.ascii	"\0016rk_vcodec: %s:%d: session %d:%d task %d state "
	.ascii	"0x%lx kref_rd %d ret %d\012\000"
.LC56:
	.ascii	"reset-names\000"
.LC57:
	.ascii	"%s is not found!\012\000"
.LC58:
	.ascii	"reset group is empty!\012\000"
.LC59:
	.ascii	"reset_group->rw_sem_on=%d\012\000"
.LC60:
	.ascii	"&queue->mmu_lock\000"
.LC61:
	.ascii	"\0013rk_vcodec: %s:%d: error: base %x, offset %x\012"
	.ascii	"\000"
.LC62:
	.ascii	"\0013rk_vcodec: %s:%d: error: req_off %x, req_size "
	.ascii	"%x, off_s %x\012\000"
.LC63:
	.ascii	"\0013rk_vcodec: %s:%d: error: off_e %x, max_size %x"
	.ascii	"\012\000"
.LC64:
	.ascii	"\0013rk_vcodec: %s:%d: error: req_off %x, max_size "
	.ascii	"%x\012\000"
.LC65:
	.ascii	"\0013rk_vcodec: %s:%d: error: req_off %x, req_size "
	.ascii	"%x, max_size %x\012\000"
.LC66:
	.ascii	"\0013rk_vcodec: %s:%d: count %d, total %d, max_size"
	.ascii	" %d\012\000"
.LC67:
	.ascii	"\0016rk_vcodec: reg[%d] + offset %d\012\000"
.LC68:
	.ascii	"\0013rk_vcodec: %s:%d: --- dump mem region ---\012\000"
.LC69:
	.ascii	"\0013rk_vcodec: %s:%d: reg[%3d]: %pad, size %lx\012"
	.ascii	"\000"
.LC70:
	.ascii	"no memory region mapped\012\000"
.LC71:
	.ascii	"\0013rk_vcodec: %s:%d: --- dump task register ---\012"
	.ascii	"\000"
.LC72:
	.ascii	"\0013rk_vcodec: %s:%d: reg[%03d]: %04x: 0x%08x\012\000"
.LC73:
	.ascii	"\0013rk_vcodec: %s:%d: --- dump hardware register -"
	.ascii	"--\012\000"
.LC74:
	.ascii	"&mpp->work_sem\000"
.LC75:
	.ascii	"rockchip,srv\000"
.LC76:
	.ascii	"failed to get the mpp service node\012\000"
.LC77:
	.ascii	"failed to get mpp service from node\012\000"
.LC78:
	.ascii	"failed attach service\012\000"
.LC79:
	.ascii	"rockchip,taskqueue-node\000"
.LC80:
	.ascii	"failed to get taskqueue-node\012\000"
.LC81:
	.ascii	"taskqueue-node %d must less than %d\012\000"
.LC82:
	.ascii	"taskqueue attach to invalid node %d\012\000"
.LC83:
	.ascii	"invalid core id %d\012\000"
.LC84:
	.ascii	"can not attach device with same id %d\000"
.LC85:
	.ascii	"\0016rk_vcodec: %s attach queue as core %d\012\000"
.LC86:
	.ascii	"rockchip,resetgroup-node\000"
.LC87:
	.ascii	"resetgroup-node %d must less than %d\012\000"
.LC88:
	.ascii	"failed to attach service\012\000"
.LC89:
	.ascii	"link mode task capacity %d\012\000"
.LC90:
	.ascii	"No interrupt resource found\012\000"
.LC91:
	.ascii	"no memory resource defined\012\000"
.LC92:
	.ascii	"ioremap failed for resource %pR\012\000"
.LC93:
	.ascii	"\0016rk_vcodec: read reg[%03d]: %04x: 0x%08x\012\000"
.LC94:
	.ascii	"shutdown device\012\000"
.LC95:
	.ascii	"wait total %d running time out\012\000"
.LC96:
	.ascii	"shutdown success\012\000"
.LC97:
	.ascii	"\0013rk_vcodec: %s:%d: error, task has been handled"
	.ascii	", irq_status %08x\012\000"
.LC98:
	.ascii	"\0016rk_vcodec: error, task is null\012\000"
.LC99:
	.ascii	"\0016rk_vcodec: resetting...\012\000"
.LC100:
	.ascii	"\0016rk_vcodec: reset done\012\000"
.LC101:
	.ascii	"\0013rk_vcodec: %s:%d: task has been handled\012\000"
.LC102:
	.ascii	"\0013rk_vcodec: %s:%d: task %d processing time out!"
	.ascii	"\012\000"
.LC103:
	.ascii	"\0013rk_vcodec: %s:%d: session %p, session->mpp is "
	.ascii	"null.\012\000"
.LC104:
	.ascii	"\0016rk_vcodec: %s:%d: chan_id %d ++\012\000"
.LC105:
	.ascii	"\0016rk_vcodec: chan %d task %d resolution not matc"
	.ascii	"h [%d %d] != [%d %d]\012\000"
.LC106:
	.ascii	"\0016rk_vcodec: %s:%d: chan_id %d device is busy no"
	.ascii	"w\012\000"
.LC107:
	.ascii	"\0016rk_vcodec: %s:%d: chan_id %d --\012\000"
.LC108:
	.ascii	"set grf failed\012\000"
.LC109:
	.ascii	"\0016rk_vcodec: %s:%d: %s session %d:%d task=%d sta"
	.ascii	"te=0x%lx\012\000"
.LC110:
	.ascii	"\0016rk_vcodec: %s: session %d:%d task %d time: %ll"
	.ascii	"d us\012\000"
.LC111:
	.ascii	"\0016rk_vcodec: write reg[%03d]: %04x: 0x%08x\012\000"
.LC112:
	.ascii	"\0016rk_vcodec: read reg[%03d] %04x: 0x%08x\012\000"
.LC113:
	.ascii	"clock-names\000"
.LC114:
	.ascii	"rockchip,normal-rates\000"
.LC115:
	.ascii	"rockchip,advanced-rates\000"
.LC116:
	.ascii	"\0013rk_vcodec: %s:%d: error mode %d\012\000"
.LC117:
	.ascii	"null\000"
.LC118:
	.ascii	"width\000"
.LC119:
	.ascii	"height\000"
.LC120:
	.ascii	"format\000"
.LC121:
	.ascii	"fps_in\000"
.LC122:
	.ascii	"fps_out\000"
.LC123:
	.ascii	"rc_mode\000"
.LC124:
	.ascii	"bitrate\000"
.LC125:
	.ascii	"gop_size\000"
.LC126:
	.ascii	"fps_calc\000"
.LC127:
	.ascii	"profile\000"
.LC128:
	.ascii	"VDPU1\000"
.LC129:
	.ascii	"VDPU2\000"
.LC130:
	.ascii	"VDPU1_PP\000"
.LC131:
	.ascii	"VDPU2_PP\000"
.LC132:
	.ascii	"AV1DEC\000"
.LC133:
	.ascii	"HEVC_DEC\000"
.LC134:
	.ascii	"RKVDEC\000"
.LC135:
	.ascii	"AVSPLUS_DEC\000"
.LC136:
	.ascii	"RKVENC\000"
.LC137:
	.ascii	"VEPU1\000"
.LC138:
	.ascii	"VEPU2\000"
.LC139:
	.ascii	"VEPU22\000"
.LC140:
	.ascii	"IEP2\000"
	.section	___ksymtab+get_mppdev_svr_ops,"a"
	.align	2
	.type	__ksymtab_get_mppdev_svr_ops, %object
	.size	__ksymtab_get_mppdev_svr_ops, 12
__ksymtab_get_mppdev_svr_ops:
	.word	get_mppdev_svr_ops
	.word	__kstrtab_get_mppdev_svr_ops
	.word	__kstrtabns_get_mppdev_svr_ops
	.section	___ksymtab+mpp_show_session_info,"a"
	.align	2
	.type	__ksymtab_mpp_show_session_info, %object
	.size	__ksymtab_mpp_show_session_info, 12
__ksymtab_mpp_show_session_info:
	.word	mpp_show_session_info
	.word	__kstrtab_mpp_show_session_info
	.word	__kstrtabns_mpp_show_session_info
	.section	___ksymtab+mpp_srv_get_phy,"a"
	.align	2
	.type	__ksymtab_mpp_srv_get_phy, %object
	.size	__ksymtab_mpp_srv_get_phy, 12
__ksymtab_mpp_srv_get_phy:
	.word	mpp_srv_get_phy
	.word	__kstrtab_mpp_srv_get_phy
	.word	__kstrtabns_mpp_srv_get_phy
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
