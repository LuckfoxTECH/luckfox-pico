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
@ 29 "./arch/arm/include/asm/irqflags.h" 1
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
@ 171 "./arch/arm/include/asm/irqflags.h" 1
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
	.type	mpp_check_cmd_v1, %function
mpp_check_cmd_v1:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	r3, r0, #512
	cmp	r3, #4
	bls	.L6
	cmp	r0, #768
	beq	.L8
	bic	r3, r0, #256
	cmp	r3, #2
	ite	hi
	movhi	r3, #0
	movls	r3, #1
.L4:
	sub	r0, r0, #1024
	cmp	r0, #3
	bls	.L8
	cmp	r3, #0
	ite	ne
	movne	r0, #0
	mvneq	r0, #21
	bx	lr
.L6:
	movs	r3, #1
	b	.L4
.L8:
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_check_cmd_v1, .-mpp_check_cmd_v1
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
	@ link register save eliminated.
	str	r1, [r0, #248]
	str	r2, [r0, #208]
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_chnl_register, .-mpp_chnl_register
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
	ldr	r0, .L13
	bx	lr
.L14:
	.align	2
.L13:
	.word	.LANCHOR0
	.fnend
	.size	get_mppdev_svr_ops, .-get_mppdev_svr_ops
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
	ldr	r3, .L20
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r3, [r3]
	mov	r4, r1
	lsls	r3, r3, #9
	bpl	.L16
	ldr	r2, [r0, #4]
	mov	r1, r0
	ldr	r0, .L20+4
	bl	printk
.L16:
	add	r6, r4, #32
	adds	r4, r4, #44
	mov	r0, r6
	bl	mutex_lock
	ldr	r3, [r4, #4]
	add	r2, r5, #188
	mov	r0, r6
	str	r2, [r4, #4]
	strd	r4, r3, [r5, #188]
	str	r2, [r3]
	pop	{r4, r5, r6, lr}
	b	mutex_unlock
.L21:
	.align	2
.L20:
	.word	mpp_dev_debug
	.word	.LC0
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
	ldr	r3, [r0, #216]
	cbz	r3, .L23
	pop	{r4, lr}
	bx	r3
.L23:
	ldr	r0, .L24
	bl	printk
	mvn	r0, #21
	pop	{r4, pc}
.L25:
	.align	2
.L24:
	.word	.LC1
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
	ldr	r3, [r0, #220]
	cbz	r3, .L27
	pop	{r4, lr}
	bx	r3
.L27:
	ldr	r0, .L28
	bl	printk
	mvn	r0, #21
	pop	{r4, pc}
.L29:
	.align	2
.L28:
	.word	.LC2
	.fnend
	.size	mpp_wait_result, .-mpp_wait_result
	.align	1
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
@ 157 "./include/linux/uaccess.h" 1
	.syntax unified
adds r5, r1, r2; sbcscc r5, r5, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L34
	bl	arm_copy_from_user
	mov	r5, r0
.L32:
	cbz	r5, .L31
	subs	r0, r4, r5
	mov	r2, r5
	movs	r1, #0
	add	r0, r0, r6
	bl	memset
.L31:
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L34:
	mov	r5, r2
	b	.L32
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
	ldr	r1, .L41
	mov	r2, r0
	mov	r0, r4
	pop	{r4, lr}
	b	single_open
.L42:
	.align	2
.L41:
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
	ldr	r1, .L44
	ldr	r2, [r3]
	bl	seq_printf
	movs	r0, #0
	pop	{r3, pc}
.L45:
	.align	2
.L44:
	.word	.LC3
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
	ldr	r3, .L47
	ldr	r3, [r3]
	ldr	r1, [r3, #4]
	b	mpp_dma_get_iova
.L48:
	.align	2
.L47:
	.word	.LANCHOR1
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
	cbz	r0, .L53
	ldr	r2, [r0, #12]
	mov	r0, r3
	pop	{r3, lr}
	ldr	r1, [r2]
	b	mpp_dma_get_iova
.L53:
	movw	r2, #2375
	ldr	r1, .L54
	ldr	r0, .L54+4
	bl	printk
	mov	r0, #-1
	pop	{r3, pc}
.L55:
	.align	2
.L54:
	.word	.LANCHOR2
	.word	.LC4
	.fnend
	.size	mpp_chnl_get_iova_addr, .-mpp_chnl_get_iova_addr
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_fetch_add_relaxed.constprop.23, %function
atomic_fetch_add_relaxed.constprop.23:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	pld	[r0]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
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
	.size	atomic_fetch_add_relaxed.constprop.23, .-atomic_fetch_add_relaxed.constprop.23
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_add.constprop.24, %function
atomic_add.constprop.24:
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
	.size	atomic_add.constprop.24, .-atomic_add.constprop.24
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
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r5, r0
	ldr	r3, [r0, #12]
	mov	r7, r1
	cbnz	r3, .L59
	ldr	r3, [r0]
	mov	r2, #576
	ldr	r1, .L78
	str	r3, [sp]
	ldr	r3, [r0, #40]
	ldr	r0, .L78+4
	bl	printk
	mvn	r0, #21
.L58:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L59:
	ldr	r3, [r3, #12]
	ldr	r3, [r3, #16]
	cbnz	r3, .L61
.L62:
	movw	r2, #583
	ldr	r1, .L78
	ldr	r0, .L78+8
	bl	printk
	mvn	r0, #11
	b	.L58
.L61:
	blx	r3
	mov	r4, r0
	cmp	r0, #0
	beq	.L62
	ldr	r6, [r0, #2292]
	cbnz	r6, .L63
	ldr	r6, [r5, #12]
.L63:
	movs	r3, #1
	ldr	r2, .L78+12
	str	r3, [r4, #2252]
	addw	r0, r4, #2284
	ldr	r1, .L78+16
	mov	r8, #0
	bl	__init_waitqueue_head
	str	r8, [r4, #2204]
	add	r0, r6, #84
	bl	atomic_fetch_add_relaxed.constprop.23
	mvn	r3, #31
	str	r3, [r4, #2208]
	addw	r3, r4, #2212
	str	r3, [r4, #2212]
	str	r3, [r4, #2216]
	mov	r2, #2097152
	ldr	r3, .L78+20
	str	r0, [r4, #2276]
	add	r0, r4, #2224
	ldr	r1, .L78+24
	str	r3, [r4, #2220]
	mov	r3, r8
	str	r8, [sp]
	bl	init_timer_key
	ldrb	r3, [r6, #36]	@ zero_extendqisi2
	cbz	r3, .L64
	ldr	r3, [r6, #8]
	ldr	r3, [r3, #16]
	cbz	r3, .L64
	mov	r1, r4
	mov	r0, r6
	blx	r3
.L64:
	ldr	r3, [r6, #96]
	add	r0, r5, #44
	str	r6, [r7, #28]
	addw	r6, r4, #2252
	strd	r3, r4, [r7, #20]
	bl	atomic_add.constprop.24
	mov	r0, r6
	bl	atomic_fetch_add_relaxed.constprop.23
	cmp	r0, #0
	bne	.L65
	movs	r1, #2
.L77:
	mov	r0, r6
	bl	refcount_warn_saturate
.L66:
	add	r6, r5, #20
	adds	r5, r5, #32
	mov	r0, r6
	bl	mutex_lock
	ldr	r3, [r5, #4]
	adds	r2, r4, #4
	mov	r0, r6
	str	r2, [r5, #4]
	strd	r5, r3, [r4, #4]
	str	r2, [r3]
	bl	mutex_unlock
	movs	r0, #0
	b	.L58
.L65:
	blt	.L67
	cmn	r0, #1
	bpl	.L66
.L67:
	movs	r1, #1
	b	.L77
.L79:
	.align	2
.L78:
	.word	.LANCHOR2+23
	.word	.LC5
	.word	.LC6
	.word	.LANCHOR1+4
	.word	.LC7
	.word	mpp_task_timeout_work
	.word	delayed_work_timer_fn
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
	cbnz	r5, .L82
	ldr	r0, .L88
	pop	{r4, r5, r6, lr}
	b	printk
.L82:
	ldr	r3, [r0, #32]
	cmp	r3, #0
	blt	.L83
	ldr	r3, [r0, #40]
	lsls	r3, r3, #31
	bpl	.L84
	ldr	r0, [r0, #36]
	bl	fput
.L84:
	mov	r3, #-1
	str	r3, [r4, #32]
.L83:
	mov	r0, r4
	bl	list_del_init
	movs	r3, #0
	strd	r3, r3, [r4, #44]
	strd	r3, r3, [r4, #52]
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r1, r5, #240
	add	r0, r4, #8
	bl	list_move_tail
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r4, r5, r6, pc}
.L89:
	.align	2
.L88:
	.word	.LC8
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
	cbnz	r3, .L91
.L93:
	str	r4, [r4]
	str	r4, [r4, #4]
	ldr	r3, [r5, #4]
	str	r4, [r5, #4]
	strd	r5, r3, [r4]
	str	r4, [r3]
	pop	{r3, r4, r5, pc}
.L91:
	ldr	r0, [r0, #16]
	mov	r1, r4
	ldr	r3, [r4, #44]
	str	r3, [r0, #176]
	bl	mpp_process_task
	cmp	r0, #0
	beq	.L93
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
	.type	mpp_session_detach_workqueue, %function
mpp_session_detach_workqueue:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r4, r0
	ldr	r3, [r0, #12]
	cbz	r3, .L98
	ldr	r3, [r3, #96]
	cbz	r3, .L98
	ldr	r3, .L110
	ldr	r3, [r3]
	lsls	r3, r3, #9
	bpl	.L100
	ldr	r2, [r0, #4]
	mov	r1, r0
	ldr	r0, .L110+4
	bl	printk
.L100:
	ldr	r6, [r4, #12]
	ldr	r5, [r6, #96]
	add	r7, r5, #32
	mov	r0, r7
	bl	mutex_lock
	add	r1, r4, #188
	mov	r0, r1
	bl	list_del_init
	ldr	r3, [r5, #56]
	add	r2, r5, #52
	mov	r0, r7
	str	r1, [r5, #56]
	strd	r2, r3, [r4, #188]
	str	r1, [r3]
	ldr	r3, [r5, #60]
	adds	r3, r3, #1
	str	r3, [r5, #60]
	bl	mutex_unlock
	mov	r0, r6
	pop	{r3, r4, r5, r6, r7, lr}
	b	mpp_taskqueue_trigger_work
.L98:
	pop	{r3, r4, r5, r6, r7, pc}
.L111:
	.align	2
.L110:
	.word	mpp_dev_debug
	.word	.LC9
	.fnend
	.size	mpp_session_detach_workqueue, .-mpp_session_detach_workqueue
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
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r5, r0
	ldr	r4, [r0, #96]
	cbz	r4, .L112
	add	r6, r4, #116
	movs	r7, #0
	mov	r0, r6
	bl	mutex_lock
	ldr	r3, [r5, #116]
	add	r1, r4, #160
	adds	r3, r3, #36
	str	r7, [r4, r3, lsl #2]
	ldr	r0, [r4, #164]
	subs	r0, r0, #1
	str	r0, [r4, #164]
	bl	_clear_bit
	add	r0, r5, #108
	bl	list_del_init
	str	r7, [r5, #96]
	mov	r0, r6
	pop	{r3, r4, r5, r6, r7, lr}
	b	mutex_unlock
.L112:
	pop	{r3, r4, r5, r6, r7, pc}
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
	ldr	r3, .L122
	mov	r1, #3520
	push	{r4, lr}
	.save {r4, lr}
	ldr	r0, [r3, #32]
	bl	kmem_cache_alloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L114
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r2, .L122+4
	ldr	r1, .L122+8
	adds	r0, r0, #20
	ldr	r3, [r3, #12]
	ldr	r3, [r3, #472]
	str	r3, [r0, #20]
	bl	__mutex_init
	add	r3, r4, #32
	str	r3, [r4, #32]
	str	r3, [r4, #36]
	add	r3, r4, #180
	str	r3, [r4, #180]
	str	r3, [r4, #184]
	add	r3, r4, #188
	str	r3, [r4, #188]
	str	r3, [r4, #192]
	movs	r3, #0
	str	r3, [r4, #44]
	str	r3, [r4, #48]
	add	r3, r4, #232
	str	r3, [r4, #232]
	str	r3, [r4, #236]
	add	r3, r4, #240
	str	r3, [r4, #240]
	str	r3, [r4, #244]
	ldr	r3, .L122+12
	ldr	r3, [r3]
	lsls	r3, r3, #9
	bpl	.L114
	mov	r1, r4
	ldr	r0, .L122+16
	bl	printk
.L114:
	mov	r0, r4
	pop	{r4, pc}
.L123:
	.align	2
.L122:
	.word	kmalloc_caches
	.word	.LANCHOR1+4
	.word	.LC10
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
	ldr	r5, .L157
	ldr	r3, [r5]
	cbnz	r3, .L125
.L127:
	movs	r4, #0
.L124:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L125:
	bl	mpp_session_init
	mov	r4, r0
	cmp	r0, #0
	beq	.L127
	movs	r3, #1
	str	r3, [r4, #212]
	ldr	r3, .L157+4
	ldr	r0, [r5]
	str	r3, [r4, #216]
	ldr	r3, .L157+8
	str	r0, [r4, #8]
	str	r3, [r4, #220]
	ldr	r3, .L157+12
	str	r3, [r4, #224]
	cbz	r0, .L128
	add	r0, r0, #700
	bl	mutex_lock
	ldr	r3, [r5]
	add	r2, r4, #180
	add	r3, r3, #712
	ldr	r1, [r3, #4]
	str	r2, [r3, #4]
	strd	r3, r1, [r4, #180]
	str	r2, [r1]
	ldr	r0, [r5]
	add	r0, r0, #700
	bl	mutex_unlock
.L128:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
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
	beq	.L127
	str	r6, [r4]
	str	r5, [r4, #12]
	ldr	r3, [r5, #12]
	cbz	r3, .L130
	ldr	r3, [r3]
	cbz	r3, .L131
	str	r3, [r4, #216]
.L131:
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #4]
	cbz	r3, .L132
	str	r3, [r4, #220]
.L132:
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #8]
	cbz	r3, .L130
	str	r3, [r4, #224]
.L130:
	add	r0, r5, #76
	bl	atomic_fetch_add_relaxed.constprop.23
	str	r0, [r4, #4]
	ldr	r3, [r5, #12]
	cbnz	r3, .L134
.L135:
	ldr	r1, [r5, #96]
	mov	r0, r4
	bl	mpp_session_attach_workqueue
	b	.L124
.L134:
	ldr	r3, [r3, #52]
	cmp	r3, #0
	beq	.L135
	mov	r0, r4
	blx	r3
	cmp	r0, #0
	beq	.L135
	b	.L127
.L158:
	.align	2
.L157:
	.word	.LANCHOR1
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
	ldr	r6, .L173
	mov	r8, r1
	ldr	r5, [r0, #320]
	ldr	r3, [r6]
	sub	r9, r5, #12
	lsls	r2, r3, #26
	bpl	.L160
	movw	r2, #1539
	ldr	r1, .L173+4
	ldr	r0, .L173+8
	bl	printk
.L160:
	bl	mpp_session_init
	mov	r4, r0
	cbz	r0, .L161
	str	r9, [r0, #8]
	cmp	r9, #0
	beq	.L162
	add	r9, r5, #688
	add	r5, r5, #700
	mov	r0, r9
	bl	mutex_lock
	ldr	r2, [r5, #4]
	add	r3, r4, #180
	mov	r0, r9
	str	r3, [r5, #4]
	strd	r5, r2, [r4, #180]
	str	r3, [r2]
	bl	mutex_unlock
.L162:
	ldr	r3, .L173+12
	str	r3, [r4, #216]
	ldr	r3, .L173+16
	str	r3, [r4, #220]
	ldr	r3, .L173+20
	str	r3, [r4, #224]
	ldr	r3, [r6]
	str	r4, [r8, #128]
	lsls	r3, r3, #26
	bpl	.L163
	movw	r2, #1557
	ldr	r1, .L173+4
	ldr	r0, .L173+24
	bl	printk
.L163:
	mov	r1, r8
	mov	r0, r7
	pop	{r3, r4, r5, r6, r7, r8, r9, lr}
	b	nonseekable_open
.L161:
	mvn	r0, #11
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L174:
	.align	2
.L173:
	.word	mpp_dev_debug
	.word	.LANCHOR2+48
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
	cbnz	r4, .L176
	mov	r3, r6
	movw	r2, #501
	ldr	r1, .L186
	ldr	r0, .L186+4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	printk
.L176:
	ldr	r3, .L186+8
	ldr	r3, [r3]
	lsls	r3, r3, #22
	bpl	.L177
	ldr	r3, [r6, #2204]
	mov	r2, #508
	ldr	r1, .L186
	ldr	r0, .L186+12
	str	r3, [sp, #8]
	ldr	r3, [r6, #2200]
	str	r3, [sp, #4]
	ldr	r3, [r6, #2276]
	str	r3, [sp]
	ldr	r3, [r4, #4]
	bl	printk
.L177:
	ldr	r5, [r6, #2292]
	cbnz	r5, .L178
	ldr	r5, [r4, #12]
.L178:
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #44]
	cbz	r3, .L179
	mov	r1, r6
	mov	r0, r4
	blx	r3
.L179:
	add	r3, r4, #44
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
	add	r3, r5, #80
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
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L187:
	.align	2
.L186:
	.word	.LANCHOR2+61
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
	.type	kref_put.constprop.14, %function
kref_put.constprop.14:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	pld	[r0]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
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
	bne	.L189
	bl	mpp_free_task
.L188:
	mov	r0, r4
	pop	{r4, pc}
.L189:
	cmp	r4, #0
	bgt	.L191
	movs	r1, #3
	bl	refcount_warn_saturate
.L191:
	movs	r4, #0
	b	.L188
	.fnend
	.size	kref_put.constprop.14, .-kref_put.constprop.14
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_taskqueue_pop_running.constprop.10, %function
mpp_taskqueue_pop_running.constprop.10:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r1, r0
	ldr	r3, [r0]
	cbz	r3, .L195
	ldr	r3, [r3, #12]
	cbz	r3, .L195
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r4, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	adds	r0, r0, #20
	bl	list_del_init
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r4	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	addw	r0, r1, #2252
	bl	kref_put.constprop.14
	movs	r0, #0
.L192:
	pop	{r4, pc}
.L195:
	mvn	r0, #21
	b	.L192
	.fnend
	.size	mpp_taskqueue_pop_running.constprop.10, .-mpp_taskqueue_pop_running.constprop.10
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_session_clear.constprop.9, %function
mpp_session_clear.constprop.9:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	add	r6, r0, #20
	mov	r4, r0
	mov	r0, r6
	bl	mutex_lock
	ldr	r1, [r4, #32]!
	ldr	r5, [r1], #-4
	subs	r5, r5, #4
.L197:
	adds	r7, r1, #4
	cmp	r7, r4
	bne	.L198
	mov	r0, r6
	bl	mutex_unlock
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L198:
	addw	r0, r1, #2204
	bl	atomic_add.constprop.24
	mov	r0, r7
	bl	list_del_init
	addw	r0, r1, #2252
	bl	kref_put.constprop.14
	ldr	r3, [r5, #4]
	mov	r1, r5
	subs	r5, r3, #4
	b	.L197
	.fnend
	.size	mpp_session_clear.constprop.9, .-mpp_session_clear.constprop.9
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
	ldr	r7, .L318
	mov	r4, r1
	mov	r5, r0
	mov	r6, r2
	ldr	r3, [r7]
	lsls	r1, r3, #27
	bpl	.L200
	ldr	r1, [r2]
	ldr	r0, .L318+4
	bl	printk
.L200:
	ldr	r1, [r6]
	movw	r3, #257
	cmp	r1, r3
	beq	.L201
	cmp	r1, #258
	bcs	.L202
	cmp	r1, #1
	beq	.L203
	cmp	r1, #0
	beq	.L204
	cmp	r1, #2
	beq	.L205
	cmp	r1, #256
	beq	.L206
.L207:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	bne	.L244
	ldr	r3, [r5]
	movw	r2, #1263
	ldr	r1, .L318+8
	ldr	r0, .L318+12
	str	r3, [sp]
	ldr	r3, [r5, #40]
	bl	printk
	b	.L317
.L202:
	cmp	r1, #516
	bcs	.L208
	cmp	r1, #512
	bcs	.L209
	cmp	r1, #258
	bne	.L207
	cmp	r5, #0
	beq	.L316
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq	.L316
	cmp	r3, #120
	bls	.L238
	movs	r2, #120
	ldr	r1, .L318+8
	str	r2, [sp]
	movw	r2, #1215
	ldr	r0, .L318+16
	mvn	r4, #11
	bl	printk
	b	.L199
.L208:
	cmp	r1, #768
	beq	.L211
	cmp	r1, #1024
	bne	.L207
	bl	ktime_get
	ldr	r3, .L318+20
	adds	r8, r0, r3
	mov	r3, #0
	adc	r9, r1, r3
	bl	_cond_resched
.L243:
	ldr	r3, [r5, #44]
	cmp	r3, #0
	beq	.L240
	bl	ktime_get
	cmp	r8, r0
	sbcs	r3, r9, r1
	bge	.L241
	ldr	r3, [r5, #44]
	cmp	r3, #0
	beq	.L240
	movw	r2, #1247
	ldr	r1, .L318+8
	ldr	r0, .L318+24
	mvn	r4, #109
	bl	printk
	b	.L199
.L204:
	ldr	r3, [r7]
	ldr	r4, [r4, #80]
	lsls	r2, r3, #27
	bpl	.L213
	mov	r1, r4
	ldr	r0, .L318+28
	bl	printk
.L213:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	mov	r2, r4
	ldr	r1, [r3, #8]
.L313:
	ldr	r0, [r6, #16]
.L315:
	subs	r1, r1, #1
	.syntax unified
@ 1119 "drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__put_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r0, #0
	beq	.L316
.L218:
	mvn	r4, #13
.L199:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L203:
	cbz	r5, .L216
	ldr	r3, [r5, #12]
	cmp	r3, #0
	bne	.L217
.L216:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r0, [r6, #16]
	ldr	r1, [r3, #8]
	subs	r1, r1, #1
	.syntax unified
@ 1132 "drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__get_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r5, r2
	cmp	r0, #0
	bne	.L218
	ldr	r3, [r7]
	lsls	r3, r3, #27
	bpl	.L219
	mov	r1, r2
	ldr	r0, .L318+32
	bl	printk
.L219:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
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
	bmi	.L221
.L317:
	mvn	r4, #21
	b	.L199
.L221:
	adds	r2, r2, #86
	ldr	r3, [r4, r2, lsl #2]
	cmp	r3, #0
	beq	.L317
.L217:
	ldr	r3, [r3, #4]
	ldr	r4, [r3, #4]
	ldr	r3, [r7]
	lsls	r1, r3, #27
	bpl	.L223
	ldr	r1, [r4, #8]
	ldr	r0, .L318+36
	bl	printk
.L223:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r2, [r4, #8]
	ldr	r1, [r3, #8]
	b	.L313
.L319:
	.align	2
.L318:
	.word	mpp_dev_debug
	.word	.LC16
	.word	.LANCHOR2+92
	.word	.LC25
	.word	.LC22
	.word	500000000
	.word	.LC24
	.word	.LC17
	.word	.LC18
	.word	.LC19
.L205:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r0, [r6, #16]
	ldr	r1, [r3, #8]
	subs	r1, r1, #1
	.syntax unified
@ 1152 "drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__get_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, r2
	cmp	r0, #0
	bne	.L317
	cmp	r2, #512
	ldr	r4, [r6, #16]
	beq	.L246
	bhi	.L225
	cbz	r2, .L247
	cmp	r2, #256
	beq	.L248
.L226:
	movw	r2, #1097
	ldr	r1, .L320
	ldr	r0, .L320+4
	bl	printk
	movs	r2, #0
.L224:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	mov	r0, r4
	ldr	r1, [r3, #8]
	b	.L315
.L225:
	cmp	r2, #768
	beq	.L249
	cmp	r2, #1024
	bne	.L226
	movw	r2, #1028
	b	.L224
.L246:
	movw	r2, #517
	b	.L224
.L247:
	movs	r2, #3
	b	.L224
.L248:
	movw	r2, #259
	b	.L224
.L249:
	movw	r2, #769
	b	.L224
.L206:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r0, [r6, #16]
	ldr	r1, [r3, #8]
	subs	r1, r1, #1
	.syntax unified
@ 1161 "drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__get_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r6, r2
	cmp	r0, #0
	bne	.L218
	ldr	r3, [r7]
	lsls	r2, r3, #27
	bpl	.L227
	mov	r1, r6
	ldr	r0, .L320+8
	bl	printk
.L227:
	cmp	r6, #28
	bls	.L228
	movs	r3, #29
	movw	r2, #1167
	ldr	r1, .L320+12
	ldr	r0, .L320+16
	bl	printk
	b	.L317
.L228:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
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
	beq	.L317
	str	r2, [r5]
	str	r6, [r5, #12]
	ldr	r3, [r6, #12]
	cbz	r3, .L230
	ldr	r3, [r3]
	cbz	r3, .L231
	str	r3, [r5, #216]
.L231:
	ldr	r3, [r6, #12]
	ldr	r3, [r3, #4]
	cbz	r3, .L232
	str	r3, [r5, #220]
.L232:
	ldr	r3, [r6, #12]
	ldr	r3, [r3, #8]
	cbz	r3, .L230
	str	r3, [r5, #224]
.L230:
	add	r0, r6, #76
	bl	atomic_fetch_add_relaxed.constprop.23
	str	r0, [r5, #4]
	ldr	r3, [r6, #12]
	cbnz	r3, .L234
.L235:
	ldr	r1, [r6, #96]
	mov	r0, r5
	bl	mpp_session_attach_workqueue
.L316:
	movs	r4, #0
	b	.L199
.L234:
	ldr	r3, [r3, #52]
	cmp	r3, #0
	beq	.L235
	mov	r0, r5
	blx	r3
	mov	r4, r0
	cmp	r0, #0
	beq	.L235
	b	.L199
.L201:
	ldr	r5, [r5, #12]
	cmp	r5, #0
	beq	.L317
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r0, [r6, #16]
	ldr	r1, [r3, #8]
	subs	r1, r1, #1
	.syntax unified
@ 1204 "drivers/kmpp/mpp/mpp_common.c" 1
	.ifnc r0,r0; .ifnc r0r0,fpr11; .ifnc r0r0,r11fp; .ifnc r0r0,ipr12; .ifnc r0r0,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r2,r2; .ifnc r2r2,fpr11; .ifnc r2r2,r11fp; .ifnc r2r2,ipr12; .ifnc r2r2,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	.ifnc r1,r1; .ifnc r1r1,fpr11; .ifnc r1r1,r11fp; .ifnc r1r1,ipr12; .ifnc r1r1,r12ip; .err; .endif; .endif; .endif; .endif; .endif
	bl	__get_user_4
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r4, r0
	cmp	r0, #0
	bne	.L218
	ldr	r3, [r5, #64]
	ldr	r0, [r3, #8]
	cmp	r0, #0
	beq	.L316
	mov	r1, #1496
	bl	regmap_write
	b	.L199
.L238:
	mov	r2, r3
	ldr	r1, [r6, #16]
	add	r0, r5, #56
	bl	_copy_from_user
	cbz	r0, .L239
	movw	r2, #1221
	ldr	r1, .L320+12
	ldr	r0, .L320+20
	bl	printk
	b	.L317
.L239:
	ldr	r3, [r6, #8]
	lsrs	r3, r3, #1
	str	r3, [r5, #52]
	b	.L316
.L209:
	ldr	r2, [r6, #4]
	ldr	r3, [r10, #44]
	orrs	r3, r3, r2
	str	r3, [r10, #44]
	ldr	r3, [r10, #52]
	adds	r3, r3, #1
	str	r3, [r10, #52]
	b	.L316
.L211:
	ldr	r2, [r6, #4]
	ldr	r3, [r10, #44]
	orrs	r3, r3, r2
	str	r3, [r10, #44]
	ldr	r3, [r10, #56]
	adds	r3, r3, #1
	str	r3, [r10, #56]
	b	.L316
.L241:
	mov	r1, #1000
	movs	r0, #251
	bl	usleep_range
	b	.L243
.L240:
	ldr	r3, [r5, #12]
	cmp	r3, #0
	beq	.L317
	mov	r0, r5
	bl	mpp_session_clear.constprop.9
	b	.L316
.L244:
	ldr	r3, [r3, #12]
	ldr	r4, [r3, #48]
	cbz	r4, .L245
	mov	r1, r6
	mov	r0, r5
	mov	r3, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	bx	r3
.L245:
	ldr	r3, [r7]
	lsls	r3, r3, #27
	bpl	.L316
	ldr	r0, .L320+24
	bl	printk
	b	.L199
.L321:
	.align	2
.L320:
	.word	.LANCHOR2+75
	.word	.LC20
	.word	.LC18
	.word	.LANCHOR2+92
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
	mov	r7, r0
	mov	r2, #380
	movs	r1, #0
	add	r0, sp, #20
	ldr	fp, .L351+20
	bl	memset
	adds	r4, r4, #24
	mov	r10, #20
.L330:
	ldr	r6, [sp, #68]
	add	r3, sp, #20
	ldr	r1, [r4, #-24]
	mul	r0, r10, r6
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
	bpl	.L323
	str	ip, [sp]
	ldr	r0, .L351
	bl	printk
.L323:
	add	r3, sp, #20
	mla	r8, r10, r6, r3
	ldr	r2, [r8, #60]
	mov	r0, r2
	bl	mpp_check_cmd_v1
	cmp	r0, #0
	beq	.L324
	mov	r3, r2
	ldr	r1, .L351+4
	movw	r2, #1058
	ldr	r0, .L351+8
	bl	printk
.L333:
	mvn	r5, #13
	b	.L322
.L350:
	add	r3, sp, #20
	mov	r2, r9
	ldr	r1, [r7, #8]
	mov	r0, r7
	bl	mpp_process_request
	mov	r5, r0
	cmp	r0, #0
	bne	.L333
	ldr	r3, [r8, #64]
	lsls	r0, r3, #31
	bpl	.L326
	lsls	r1, r3, #30
	bmi	.L326
.L329:
	add	r3, sp, #20
	mla	r6, r10, r6, r3
	ldr	r3, [r6, #64]
	lsls	r2, r3, #31
	bpl	.L322
	lsls	r3, r3, #30
	add	r4, r4, #24
	bpl	.L330
	b	.L322
.L326:
	ldr	r3, [sp, #64]
	str	r3, [r7, #176]
	ldr	r3, [sp, #72]
	cbz	r3, .L328
	add	r1, sp, #20
	mov	r0, r7
	bl	mpp_process_task
	cbnz	r0, .L334
	ldrd	r9, r2, [sp, #44]
	str	r0, [sp, #12]
	add	r0, r2, #80
	ldr	r8, [sp, #40]
	str	r2, [sp, #8]
	bl	atomic_add.constprop.24
	addw	r1, r9, #2200
	ldr	r3, [sp, #12]
	mov	r0, r3
	bl	____atomic_set_bit
	ldr	r1, [r8, #80]
	add	r0, r9, #20
	add	r3, r8, #76
	str	r0, [r8, #80]
	strd	r3, r1, [r9, #20]
	str	r0, [r1]
	ldr	r2, [sp, #8]
	mov	r0, r2
	bl	mpp_taskqueue_trigger_work
.L328:
	ldr	r3, [sp, #76]
	cmp	r3, #0
	beq	.L329
	add	r1, sp, #20
	mov	r0, r7
	bl	mpp_wait_result
	cmp	r0, #0
	beq	.L329
	mov	r5, r0
	b	.L322
.L334:
	mov	r5, r0
	b	.L322
.L324:
	ldr	r3, [sp, #68]
	adds	r3, r3, #1
	str	r3, [sp, #68]
	cmp	r3, #16
	bls	.L350
	movs	r2, #16
	ldr	r1, .L351+12
	str	r2, [sp]
	movw	r2, #2334
	ldr	r0, .L351+16
	mvn	r5, #21
	bl	printk
.L322:
	mov	r0, r5
	add	sp, sp, #404
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L352:
	.align	2
.L351:
	.word	.LC27
	.word	.LANCHOR2+112
	.word	.LC28
	.word	.LANCHOR2+129
	.word	.LC29
	.word	mpp_dev_debug
	.fnend
	.size	mpp_chnl_add_req, .-mpp_chnl_add_req
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
	cbz	r3, .L354
	ldr	r3, [r3, #12]
	ldr	r3, [r3, #56]
	cbz	r3, .L355
	blx	r3
.L355:
	mov	r0, r4
	bl	mpp_session_clear.constprop.9
.L354:
	ldr	r5, [r4, #8]
	cbz	r5, .L356
	add	r5, r5, #700
	mov	r0, r5
	bl	mutex_lock
	add	r0, r4, #180
	bl	list_del_init
	mov	r0, r5
	bl	mutex_unlock
.L356:
	add	r0, r4, #188
	pop	{r3, r4, r5, lr}
	b	list_del_init
	.fnend
	.size	mpp_session_deinit_default, .-mpp_session_deinit_default
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
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	add	r8, r0, #20
	.pad #48
	sub	sp, sp, #48
	mov	r6, r0
	mov	r0, r8
	mov	r9, r1
	bl	mutex_lock
	mov	r3, r6
	ldr	r4, [r3, #32]!
	mov	r0, r8
	cmp	r4, r3
	ite	ne
	subne	r4, r4, #4
	moveq	r4, #0
	bl	mutex_unlock
	cbnz	r4, .L368
	mov	r3, r6
	mov	r2, #840
	ldr	r1, .L392
	mvn	r5, #4
	ldr	r0, .L392+4
	bl	printk
.L366:
	mov	r0, r5
	add	sp, sp, #48
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L368:
	ldr	r7, [r4, #2292]
	cbnz	r7, .L370
	ldr	r7, [r6, #12]
.L370:
	bl	_cond_resched
	ldr	r1, [r4, #2200]
	ubfx	r1, r1, #7, #1
	cbnz	r1, .L381
	add	r0, sp, #28
	addw	r10, r4, #2284
	bl	init_wait_entry
	mov	r5, #600
.L375:
	movs	r2, #2
	add	r1, sp, #28
	mov	r0, r10
	bl	prepare_to_wait_event
	ldr	r3, [r4, #2200]
	lsls	r2, r3, #24
	bpl	.L372
	cmp	r5, #0
	it	eq
	moveq	r5, #1
.L374:
	add	r1, sp, #28
	mov	r0, r10
	bl	finish_wait
	cmp	r5, #0
	bgt	.L371
	addw	r0, r4, #2204
	bl	atomic_add.constprop.24
	addw	r1, r4, #2200
	movs	r0, #9
	bl	____atomic_set_bit
	ldr	r3, [r6, #44]
	ldr	r2, [r4, #2276]
	ldr	r1, .L392
	ldr	r0, .L392+8
	str	r3, [sp, #8]
	ldr	r3, [r6, #4]
	strd	r4, r2, [sp, #12]
	movw	r2, #857
	strd	r6, r3, [sp]
	ldr	r3, [r6, #40]
	bl	printk
	b	.L377
.L381:
	mov	r5, #600
.L371:
	ldr	r3, [r7, #12]
	ldr	r3, [r3, #40]
	cbz	r3, .L377
	mov	r2, r9
	mov	r1, r4
	mov	r0, r7
	blx	r3
	mov	r5, r0
.L377:
	ldr	r3, .L392+12
	ldr	r3, [r3]
	lsls	r3, r3, #22
	bpl	.L378
	ldr	r3, [r4, #2252]
	movw	r2, #861
	ldr	r1, .L392
	ldr	r0, .L392+16
	str	r3, [sp]
	ldr	r3, [r4, #2276]
	bl	printk
.L378:
	mov	r0, r8
	bl	mutex_lock
	adds	r0, r4, #4
	bl	list_del_init
	mov	r0, r8
	bl	mutex_unlock
	addw	r0, r4, #2252
	bl	kref_put.constprop.14
	b	.L366
.L372:
	cmp	r5, #0
	beq	.L374
	mov	r0, r5
	bl	schedule_timeout
	mov	r5, r0
	b	.L375
.L393:
	.align	2
.L392:
	.word	.LANCHOR2+146
	.word	.LC30
	.word	.LC31
	.word	mpp_dev_debug
	.word	.LC32
	.fnend
	.size	mpp_wait_result_default, .-mpp_wait_result_default
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
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, [r0, #240]
	add	r3, r5, #240
	add	r6, r5, #232
	cmp	r0, r3
	beq	.L395
	subs	r4, r0, #8
	beq	.L395
	mov	r1, r6
	bl	list_move_tail
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L394:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L395:
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L403
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
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r1, r6
	bl	list_move_tail
	ldr	r3, [r5, #228]
	adds	r3, r3, #1
	str	r3, [r5, #228]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L403+4
	ldr	r3, [r3]
	lsls	r3, r3, #22
	bpl	.L394
	ldr	r3, [r5, #228]
	mov	r2, #286
	ldr	r1, .L403+8
	ldr	r0, .L403+12
	str	r3, [sp, #4]
	ldr	r3, [r5, #4]
	str	r3, [sp]
	mov	r3, r5
	bl	printk
	b	.L394
.L404:
	.align	2
.L403:
	.word	kmalloc_caches
	.word	mpp_dev_debug
	.word	.LANCHOR2+170
	.word	.LC33
	.fnend
	.size	get_task_msgs, .-get_task_msgs
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
	ldr	r7, .L498
	.pad #84
	sub	sp, sp, #84
	mov	r10, r2
	ldr	r6, [r0, #128]
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L406
	movw	r2, #1500
	ldr	r1, .L498+4
	ldr	r0, .L498+8
	bl	printk
.L406:
	cbz	r6, .L407
	ldr	r3, [r6, #8]
	cbnz	r3, .L408
.L407:
	mov	r3, r6
	movw	r2, #1503
	ldr	r1, .L498+4
	mvn	r4, #21
	ldr	r0, .L498+12
	bl	printk
.L405:
	mov	r0, r4
	add	sp, sp, #84
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L408:
	ldr	r2, [r6, #48]
	cmp	r2, #0
	ble	.L410
	ldr	r3, [r7]
	lsls	r6, r3, #27
	bpl	.L447
	ldr	r0, .L498+16
.L497:
	bl	printk
.L447:
	mvn	r4, #15
	b	.L405
.L410:
	ldr	r3, [r3, #84]
	cmp	r3, #0
	ble	.L411
	ldr	r3, [r7]
	lsls	r5, r3, #27
	bpl	.L447
	ldr	r0, .L498+20
	b	.L497
.L411:
	ldr	r3, .L498+24
	add	r9, sp, #32
	str	r9, [sp, #32]
	cmp	r4, r3
	str	r9, [sp, #36]
	beq	.L448
	mov	r3, r4
	movw	r2, #1305
	ldr	r1, .L498+28
	ldr	r0, .L498+32
	bl	printk
.L496:
	mvn	r4, #21
.L413:
	mov	r3, r4
	movw	r2, #1522
	ldr	r1, .L498+4
	ldr	r0, .L498+36
	bl	printk
	b	.L433
.L448:
	movs	r5, #0
.L412:
	movs	r2, #24
	mov	r1, r10
	add	r0, sp, #56
	bl	_copy_from_user
	cbnz	r0, .L419
	ldr	r3, [r7]
	add	r10, r10, #24
	lsls	r4, r3, #27
	bpl	.L415
	ldr	r3, [sp, #68]
	ldr	r1, [sp, #56]
	ldr	r0, .L498+40
	str	r3, [sp]
	ldrd	r2, r3, [sp, #60]
	bl	printk
.L415:
	ldr	r2, [sp, #56]
	mov	r0, r2
	bl	mpp_check_cmd_v1
	mov	r4, r0
	cbz	r0, .L416
	mov	r3, r2
	ldr	r1, .L498+28
	mov	r2, #1320
	ldr	r0, .L498+44
	bl	printk
.L419:
	mvn	r4, #13
	b	.L413
.L416:
	ldr	r3, [sp, #60]
	lsls	r0, r3, #31
	ite	mi
	ubfxmi	fp, r3, #1, #1
	movpl	fp, #1
	cmp	r2, #516
	bne	.L418
	ldr	r3, [sp, #72]
	movs	r2, #16
	add	r0, sp, #40
	mov	r1, r3
	str	r3, [sp, #16]
	bl	_copy_from_user
	cmp	r0, #0
	bne	.L419
	ldr	r3, [sp, #40]
	and	r3, r3, #1
	strd	r3, r0, [sp, #8]
	ldrd	r2, [sp, #8]
	orrs	r3, r2, r3
	bne	.L420
	ldr	r0, [sp, #48]
	bl	__fdget
	bics	r8, r0, #3
	and	r3, r0, #3
	str	r3, [sp, #20]
	bne	.L421
	mvn	r3, #8
	movw	r2, #1349
	str	r3, [sp, #28]
	ldr	r1, .L498+28
	ldr	r3, [sp, #48]
	ldr	r0, .L498+48
	bl	printk
	mov	r3, sp
	bic	r3, r3, #8160
	ldr	r0, [sp, #16]
	bic	r3, r3, #31
	adds	r0, r0, #12
	ldr	r3, [r3, #8]
	.syntax unified
@ 177 "./include/linux/uaccess.h" 1
	.syntax unified
adds r2, r0, #4; sbcscc r2, r2, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbz	r3, .L422
.L424:
	mov	r2, #1352
	ldr	r1, .L498+28
	ldr	r0, .L498+52
	bl	printk
.L420:
	cmp	fp, #0
	beq	.L412
.L433:
	ldr	r1, [sp, #32]
	mov	fp, #0
	mov	r6, fp
	ldr	r8, [r1]
.L434:
	cmp	r1, r9
	bne	.L439
	cbz	r6, .L440
	cmp	fp, #0
	beq	.L440
	add	r0, fp, #64
	bl	mutex_unlock
	mov	r0, r6
	bl	mpp_taskqueue_trigger_work
.L440:
	ldr	r5, [sp, #32]
	ldr	r10, .L498+76
	ldr	fp, .L498+80
	ldr	r6, [r5]
.L441:
	cmp	r5, r9
	bne	.L445
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L405
	mov	r2, #1528
	ldr	r1, .L498+4
	ldr	r0, .L498+56
	bl	printk
	b	.L405
.L422:
	movs	r2, #4
	add	r1, sp, #28
	bl	arm_copy_to_user
	cmp	r0, #0
	bne	.L424
	b	.L420
.L421:
	cbz	r5, .L425
	ldr	r3, [r5, #48]
	cbz	r3, .L426
	mov	r1, r9
	mov	r0, r5
	bl	task_msgs_add
.L425:
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
	bpl	.L420
	ldr	r3, [r6, #228]
	ldr	r2, [r6, #4]
	ldr	r1, [sp, #48]
	ldr	r0, .L498+60
	bl	printk
	b	.L420
.L426:
	mov	r0, r5
	bl	put_task_msgs
	b	.L425
.L418:
	cbnz	r5, .L430
	mov	r0, r6
	bl	get_task_msgs
	mov	r5, r0
	cbnz	r0, .L430
	ldr	r2, [r6, #4]
	mov	r1, r6
	ldr	r0, .L498+64
	bl	printk
	b	.L496
.L430:
	ldr	r2, [r5, #48]
	cmp	r2, #15
	bls	.L431
	movs	r3, #16
	ldr	r1, .L498+28
	strd	r2, r3, [sp]
	movw	r2, #1397
	ldr	r3, [r6, #4]
	ldr	r0, .L498+68
	bl	printk
	b	.L496
.L431:
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
	cbz	r0, .L432
	str	r0, [sp, #4]
	movw	r2, #1411
	ldr	r3, [r8]
	ldr	r1, .L498+28
	ldr	r0, .L498+72
	str	r3, [sp]
	ldr	r3, [r6, #4]
	bl	printk
	b	.L413
.L432:
	cmp	fp, #0
	beq	.L412
	mov	r1, r9
	mov	r0, r5
	bl	task_msgs_add
	b	.L433
.L499:
	.align	2
.L498:
	.word	mpp_dev_debug
	.word	.LANCHOR2+184
	.word	.LC12
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	1074034177
	.word	.LANCHOR2+198
	.word	.LC37
	.word	.LC46
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC13
	.word	.LC42
	.word	.LC43
	.word	.LC44
	.word	.LC45
	.word	.LANCHOR2+215
	.word	.LC48
.L439:
	ldr	r3, [r1, #52]
	cmp	r3, #0
	beq	.L435
	ldr	r10, [r1, #20]
	cmp	r10, #0
	beq	.L435
	ldr	r3, [r1, #28]
	cmp	r10, fp
	ldr	r5, [r1, #24]
	str	r3, [sp, #16]
	beq	.L436
	cmp	fp, #0
	beq	.L437
	cbz	r6, .L437
	add	r0, fp, #64
	bl	mutex_unlock
	mov	r0, r6
	bl	mpp_taskqueue_trigger_work
.L437:
	add	r0, r10, #64
	mov	fp, r10
	bl	mutex_lock
	ldr	r6, [sp, #16]
.L436:
	ldr	r1, [r5, #2200]
	lsls	r2, r1, #22
	bpl	.L438
	ldr	r1, [r5, #2276]
	ldr	r0, .L500
	bl	printk
.L438:
	ldr	r0, [sp, #16]
	adds	r0, r0, #80
	bl	atomic_add.constprop.24
	addw	r1, r5, #2200
	movs	r0, #0
	bl	____atomic_set_bit
	ldr	r1, [r10, #80]
	add	r0, r5, #20
	add	r3, r10, #76
	str	r0, [r10, #80]
	strd	r3, r1, [r5, #20]
	str	r0, [r1]
.L435:
	mov	r1, r8
	ldr	r8, [r8]
	b	.L434
.L445:
	ldr	r3, [r5, #56]
	cbz	r3, .L443
	ldr	r8, [r5, #16]
	mov	r1, r5
	mov	r0, r8
	bl	mpp_wait_result
	cbz	r0, .L443
	str	r0, [sp]
	movw	r2, #1484
	ldr	r3, [r8, #4]
	mov	r1, r10
	mov	r0, fp
	bl	printk
.L443:
	mov	r0, r5
	mov	r5, r6
	bl	put_task_msgs
	ldr	r6, [r6]
	b	.L441
.L501:
	.align	2
.L500:
	.word	.LC47
	.fnend
	.size	mpp_dev_ioctl, .-mpp_dev_ioctl
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
@ 66 "./arch/arm/include/asm/cmpxchg.h" 1
	@	__xchg4
1:	ldrex	r1, [r3]
	strex	r0, r2, [r3]
	teq	r0, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r1, .L504
	movs	r1, #4
	ldr	r0, [r4]
	bl	__pm_runtime_resume
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #8]
	cbz	r3, .L504
	mov	r0, r4
	blx	r3
.L504:
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	mpp_power_on, .-mpp_power_on
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
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r3, [r0, #128]
	cbnz	r3, .L511
	add	r2, r0, #124
	pld	[r2]
	.syntax unified
@ 66 "./arch/arm/include/asm/cmpxchg.h" 1
	@	__xchg4
1:	ldrex	r1, [r2]
	strex	r5, r3, [r2]
	teq	r5, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cbz	r1, .L511
	ldr	r3, [r0, #8]
	ldr	r3, [r3, #12]
	cbz	r3, .L513
	blx	r3
.L513:
	ldr	r5, [r4, #96]
	add	r6, r5, #64
	mov	r0, r6
	bl	mutex_lock
	mov	r3, r5
	ldr	r5, [r3, #76]!
	mov	r0, r6
	cmp	r5, r3
	ite	ne
	subne	r5, r5, #20
	moveq	r5, #0
	bl	mutex_unlock
	cbz	r5, .L515
.L517:
	ldr	r5, [r4]
	bl	ktime_get_mono_fast_ns
	strd	r0, [r5, #248]
	movs	r1, #13
.L525:
	ldr	r0, [r4]
	bl	__pm_runtime_suspend
.L511:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L515:
	ldr	r3, [r4, #96]
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r3, #84]!
	cmp	r2, r3
	it	ne
	subne	r5, r2, #20
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r5, #0
	bne	.L517
	movs	r1, #4
	b	.L525
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
	ldr	r5, .L545
	ldr	r6, [r0, #44]
	ldr	r2, [r5]
	lsls	r2, r2, #9
	bpl	.L527
	ldr	r2, [r0, #4]
	mov	r1, r0
	mov	r3, r6
	ldr	r0, .L545+4
	bl	printk
.L527:
	cmp	r6, #0
	bne	.L538
	ldr	r3, [r4, #224]
	cbz	r3, .L529
	mov	r0, r4
	blx	r3
.L530:
	ldr	r3, [r5]
	lsls	r3, r3, #9
	bpl	.L531
	ldr	r2, [r4, #4]
	mov	r1, r4
	ldr	r0, .L545+8
	bl	printk
.L531:
	mov	r5, sp
	strd	r5, r5, [sp]
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r8, r4
	ldr	r0, [r8, #232]!
	ldr	r6, [r0], #-8
	subs	r6, r6, #8
.L532:
	adds	r0, r0, #8
	cmp	r0, r8
	bne	.L533
	mov	r8, r4
	ldr	r0, [r8, #240]!
	ldr	r6, [r0], #-8
	subs	r6, r6, #8
.L534:
	adds	r0, r0, #8
	cmp	r0, r8
	bne	.L535
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, [sp]
	ldr	r6, [r0], #-8
	subs	r6, r6, #8
.L536:
	add	r3, r0, #8
	cmp	r5, r3
	bne	.L537
	mov	r0, r4
	bl	kfree
	movs	r0, #0
.L526:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L529:
	ldr	r0, .L545+12
	bl	printk
	b	.L530
.L533:
	mov	r1, r5
	bl	list_move_tail
	ldr	r3, [r6, #8]
	mov	r0, r6
	sub	r6, r3, #8
	b	.L532
.L535:
	mov	r1, r5
	bl	list_move_tail
	ldr	r3, [r6, #8]
	mov	r0, r6
	sub	r6, r3, #8
	b	.L534
.L537:
	bl	kfree
	ldr	r3, [r6, #8]
	mov	r0, r6
	sub	r6, r3, #8
	b	.L536
.L538:
	mov	r0, #-1
	b	.L526
.L546:
	.align	2
.L545:
	.word	mpp_dev_debug
	.word	.LC49
	.word	.LC51
	.word	.LC50
	.fnend
	.size	mpp_session_deinit, .-mpp_session_deinit
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
	ldr	r5, .L557
	ldr	r4, [r1, #128]
	ldr	r2, [r5]
	lsls	r3, r2, #26
	bpl	.L548
	movw	r2, #1566
	ldr	r1, .L557+4
	ldr	r0, .L557+8
	bl	printk
.L548:
	cbnz	r4, .L549
	movw	r2, #1569
	ldr	r1, .L557+4
	ldr	r0, .L557+12
	bl	printk
	mvn	r0, #21
.L547:
	pop	{r4, r5, r6, pc}
.L549:
	add	r0, r4, #48
	bl	atomic_add.constprop.24
	ldr	r3, [r4, #12]
	mov	r0, r4
	cbz	r3, .L551
	bl	mpp_session_detach_workqueue
.L552:
	ldr	r0, [r5]
	movs	r4, #0
	str	r4, [r6, #128]
	ands	r0, r0, #32
	beq	.L547
	movw	r2, #1583
	ldr	r1, .L557+4
	ldr	r0, .L557+16
	bl	printk
	mov	r0, r4
	b	.L547
.L551:
	bl	mpp_session_deinit
	b	.L552
.L558:
	.align	2
.L557:
	.word	mpp_dev_debug
	.word	.LANCHOR2+229
	.word	.LC12
	.word	.LC52
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
	ldr	r3, .L567
	ldr	r3, [r3]
	lsls	r3, r3, #26
	bpl	.L560
	movw	r2, #2293
	ldr	r1, .L567+4
	ldr	r0, .L567+8
	bl	printk
.L560:
	cbnz	r5, .L561
	movw	r2, #2295
	ldr	r1, .L567+4
	ldr	r0, .L567+12
	bl	printk
	mvn	r0, #21
.L559:
	pop	{r3, r4, r5, pc}
.L561:
	add	r0, r5, #48
	bl	atomic_add.constprop.24
	ldr	r4, [r5, #12]
	mov	r0, r5
	cbz	r4, .L563
	bl	mpp_session_detach_workqueue
	movs	r0, #0
	b	.L559
.L563:
	bl	mpp_session_deinit
	mov	r0, r4
	b	.L559
.L568:
	.align	2
.L567:
	.word	mpp_dev_debug
	.word	.LANCHOR2+245
	.word	.LC12
	.word	.LC52
	.fnend
	.size	mpp_chnl_release, .-mpp_chnl_release
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
	ldr	r7, .L583
	.pad #52
	sub	sp, sp, #52
	ldr	r2, .L583+4
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
	ldr	r3, [r5]
	mov	r2, r4
	ldr	r1, .L583+8
	ldr	r0, [r3, #356]
	bl	of_property_match_string
	cmp	r0, #0
	blt	.L570
	movs	r3, #0
	movs	r2, #1
	mov	r1, r4
	strd	r3, r2, [sp]
	ldr	r0, [r5]
	mov	r2, r3
	bl	__devm_reset_control_get
	mov	r4, r0
	cbz	r0, .L569
	bl	reset_control_deassert
.L569:
	ldr	r2, [sp, #44]
	mov	r0, r4
	ldr	r3, [r7]
	cmp	r2, r3
	beq	.L576
	bl	__stack_chk_fail
.L570:
	add	r0, sp, #12
	bl	strlen
	mov	r1, r4
	rsb	r2, r0, #31
	add	r0, sp, #12
	bl	strncat
	ldr	r3, [r5]
	ldr	r1, .L583+8
	ldr	r0, [r3, #356]
	add	r2, sp, #12
	bl	of_property_match_string
	cmp	r0, #0
	bge	.L572
	add	r2, sp, #12
	ldr	r1, .L583+12
	ldr	r0, [r5]
	movs	r4, #0
	bl	_dev_err
	b	.L569
.L572:
	ldr	r4, [r5, #100]
	cbnz	r4, .L573
	ldr	r1, .L583+16
	ldr	r0, [r5]
	bl	_dev_err
	b	.L569
.L573:
	add	r9, r4, #4
	add	r8, r4, r8, lsl #2
	mov	r0, r9
	bl	down_write
	ldr	r6, [r8, #20]
	cbnz	r6, .L574
	movs	r3, #1
	mov	r2, r6
	strd	r6, r3, [sp]
	add	r1, sp, #12
	mov	r3, r6
	ldr	r0, [r5]
	bl	__devm_reset_control_get
	mov	r6, r0
	cbz	r0, .L575
	bl	reset_control_deassert
.L575:
	str	r6, [r8, #20]
	ldr	r3, [r5, #96]
	str	r3, [r4, #48]
.L574:
	ldr	r1, [r4, #48]
	ldr	r3, [r5, #96]
	ldr	r2, [r4]
	cmp	r1, r3
	it	ne
	orrne	r2, r2, #1
	ldr	r1, .L583+20
	str	r2, [r4]
	mov	r4, r6
	ldr	r0, [r5]
	bl	_dev_info
	mov	r0, r9
	bl	up_write
	b	.L569
.L576:
	add	sp, sp, #52
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L584:
	.align	2
.L583:
	.word	__stack_chk_guard
	.word	.LANCHOR2+262
	.word	.LC54
	.word	.LC55
	.word	.LC56
	.word	.LC57
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
	movs	r1, #168
	bl	devm_kmalloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L585
	ldr	r2, .L590
	adds	r0, r0, #32
	ldr	r1, .L590+4
	bl	__mutex_init
	ldr	r2, .L590
	ldr	r1, .L590+8
	add	r0, r4, #64
	bl	__mutex_init
	ldr	r2, .L590
	ldr	r1, .L590+12
	add	r0, r4, #96
	bl	__mutex_init
	ldr	r2, .L590
	ldr	r1, .L590+16
	add	r0, r4, #116
	bl	__mutex_init
	add	r3, r4, #44
	str	r3, [r4, #44]
	str	r3, [r4, #48]
	add	r3, r4, #52
	str	r3, [r4, #52]
	str	r3, [r4, #56]
	add	r3, r4, #76
	str	r3, [r4, #76]
	str	r3, [r4, #80]
	add	r3, r4, #84
	str	r3, [r4, #84]
	str	r3, [r4, #88]
	add	r3, r4, #108
	str	r3, [r4, #108]
	str	r3, [r4, #112]
	add	r3, r4, #128
	str	r3, [r4, #128]
	str	r3, [r4, #132]
	movs	r3, #16
	str	r3, [r4, #136]
	movs	r3, #0
	str	r3, [r4, #140]
.L585:
	mov	r0, r4
	pop	{r4, pc}
.L591:
	.align	2
.L590:
	.word	.LANCHOR1+4
	.word	.LC58
	.word	.LC59
	.word	.LC60
	.word	.LC61
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
	bcs	.L593
	mov	r3, r1
	ldr	r0, .L601
	ldr	r1, .L601+4
	movw	r2, #1603
	str	r4, [sp]
.L599:
	bl	printk
	b	.L600
.L593:
	ldr	r2, [r5, #8]
	subs	r4, r4, r1
	adds	r1, r4, r2
	cmp	r1, r0
	bcs	.L595
	strd	r2, r0, [sp]
	mov	r3, r4
	movw	r2, #1609
	ldr	r1, .L601+4
	ldr	r0, .L601+8
	bl	printk
.L600:
	mvn	r0, #21
.L592:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L595:
	cmp	r6, r3
	bcs	.L596
	str	r6, [sp]
	movw	r2, #1614
	ldr	r1, .L601+4
	ldr	r0, .L601+12
	b	.L599
.L596:
	cmp	r4, r6
	ble	.L597
	str	r6, [sp]
	mov	r3, r4
	movw	r2, #1619
	ldr	r1, .L601+4
	ldr	r0, .L601+16
	b	.L599
.L597:
	cmp	r1, r6
	bls	.L598
	strd	r2, r6, [sp]
	mov	r3, r4
	ldr	r1, .L601+4
	mov	r2, #1624
	ldr	r0, .L601+20
	bl	printk
	ldr	r1, [r5, #8]
	subs	r1, r1, r6
	add	r4, r4, r1
	str	r4, [r5, #8]
.L598:
	movs	r0, #0
	b	.L592
.L602:
	.align	2
.L601:
	.word	.LC62
	.word	.LANCHOR2+294
	.word	.LC63
	.word	.LC64
	.word	.LC65
	.word	.LC66
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
	bls	.L604
	movs	r3, #60
	movw	r2, #1639
	strd	r0, r3, [sp]
	mov	r3, r5
	ldr	r1, .L609
	ldr	r0, .L609+4
	bl	printk
.L608:
	mvn	r0, #21
.L603:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L604:
	cmp	r2, #0
	add	r0, r4, r0, lsl #3
	add	r0, r0, #4
	ldr	r1, [r1, #16]
	bge	.L606
.L607:
	movw	r2, #1644
	ldr	r1, .L609
	ldr	r0, .L609+8
	bl	printk
	b	.L608
.L606:
	bl	_copy_from_user
	cmp	r0, #0
	bne	.L607
	ldr	r2, [r4]
	add	r2, r2, r5
	str	r2, [r4]
	b	.L603
.L610:
	.align	2
.L609:
	.word	.LANCHOR2+308
	.word	.LC67
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
	ldr	r5, .L622
	mov	r6, r1
	ldr	r2, [r5]
	lsls	r2, r2, #26
	bpl	.L612
	movw	r2, #1655
	ldr	r1, .L622+4
	ldr	r0, .L622+8
	bl	printk
	cbnz	r4, .L613
.L620:
	ldr	r3, [r5]
	lsls	r3, r3, #26
	bpl	.L621
	mov	r2, #1664
	ldr	r1, .L622+4
	ldr	r0, .L622+12
	bl	printk
.L621:
	movs	r0, #0
	b	.L611
.L616:
	adds	r3, r3, #1
.L618:
	cmp	r1, r3
	beq	.L620
	ldr	r0, [r2, r3, lsl #3]
	cmp	r0, r6
	bne	.L616
	add	r4, r4, r3, lsl #3
	ldr	r0, [r4, #8]
.L611:
	pop	{r4, r5, r6, pc}
.L612:
	cmp	r0, #0
	beq	.L621
.L613:
	mov	r2, r4
	movs	r3, #0
	ldr	r1, [r2], #4
	b	.L618
.L623:
	.align	2
.L622:
	.word	mpp_dev_debug
	.word	.LANCHOR2+336
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
	ldr	r4, .L638
	mov	r8, r2
	ldr	r3, [r4]
	lsls	r1, r3, #26
	bpl	.L625
	movw	r2, #1673
	ldr	r1, .L638+4
	ldr	r0, .L638+8
	bl	printk
	cbnz	r7, .L626
.L632:
	ldr	r3, [r4]
	lsls	r2, r3, #26
	bpl	.L637
	movw	r2, #1685
	ldr	r1, .L638+4
	ldr	r0, .L638+12
	bl	printk
.L637:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L631:
	lsls	r3, r2, #28
	bpl	.L629
	ldrd	r1, r2, [r5, #4]
	mov	r0, r9
	bl	printk
.L629:
	ldr	r2, [r5, #4]
	adds	r6, r6, #1
	ldr	r1, [r5, #8]!
	ldr	r3, [r8, r2, lsl #2]
	add	r3, r3, r1
	str	r3, [r8, r2, lsl #2]
.L630:
	ldr	r3, [r7]
	ldr	r2, [r4]
	cmp	r3, r6
	bhi	.L631
	b	.L632
.L625:
	cmp	r7, #0
	beq	.L637
.L626:
	mov	r5, r7
	movs	r6, #0
	ldr	r9, .L638+16
	b	.L630
.L639:
	.align	2
.L638:
	.word	mpp_dev_debug
	.word	.LANCHOR2+362
	.word	.LC12
	.word	.LC13
	.word	.LC68
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
	cbz	r1, .L647
	movw	r2, #1742
	ldr	r1, .L649
	ldr	r0, .L649+4
	add	r5, r4, #28
	bl	printk
	ldr	r3, [r4, #28]
	cmp	r5, r3
	beq	.L644
	ldr	r3, [r4, #28]
	ldr	r6, .L649
	ldr	r7, .L649+8
	ldr	r4, [r3]
.L645:
	cmp	r5, r3
	bne	.L646
.L648:
	movs	r0, #0
.L642:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L646:
	ldr	r2, [r3, #12]
	mov	r1, r6
	mov	r0, r7
	str	r2, [sp, #4]
	add	r2, r3, #8
	str	r2, [sp]
	movw	r2, #1748
	ldr	r3, [r3, #16]
	bl	printk
	mov	r3, r4
	ldr	r4, [r4]
	b	.L645
.L644:
	ldr	r1, .L649+12
	ldr	r0, [r6]
	bl	_dev_err
	b	.L648
.L647:
	mvn	r0, #4
	b	.L642
.L650:
	.align	2
.L649:
	.word	.LANCHOR2+392
	.word	.LC69
	.word	.LC70
	.word	.LC71
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
	cbz	r1, .L657
	ldr	r3, .L661
	ldr	r3, [r3]
	lsls	r3, r3, #21
	bmi	.L653
.L654:
	movs	r0, #0
.L651:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L653:
	movw	r2, #1763
	ldr	r1, .L661+4
	ldr	r0, .L661+8
	bl	printk
	ldr	r3, [r5, #2280]
	cmp	r3, #0
	beq	.L654
	ldr	r3, [r5, #2272]
	ldr	r7, .L661+4
	ldr	r8, .L661+12
	ldrd	r4, r6, [r3, #12]
.L655:
	cmp	r4, r6
	bhi	.L654
	ldr	r2, [r5, #2280]
	lsls	r3, r4, #2
	mov	r1, r7
	mov	r0, r8
	ldr	r2, [r2, r4, lsl #2]
	strd	r3, r2, [sp]
	mov	r3, r4
	movw	r2, #1773
	adds	r4, r4, #1
	bl	printk
	b	.L655
.L657:
	mvn	r0, #4
	b	.L651
.L662:
	.align	2
.L661:
	.word	mpp_dev_debug
	.word	.LANCHOR2+417
	.word	.LC72
	.word	.LC73
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
	ldr	r3, .L669
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r5, r0
	ldr	r3, [r3]
	lsls	r3, r3, #21
	bpl	.L668
	ldr	r3, [r0, #4]
	movw	r2, #1788
	ldr	r1, .L669+4
	ldr	r0, .L669+8
	ldr	r3, [r3, #4]
	ldr	r7, .L669+4
	ldr	r8, .L669+12
	ldrd	r4, r6, [r3, #12]
	bl	printk
.L665:
	cmp	r4, r6
	bls	.L666
.L668:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L666:
	ldr	r3, [r5, #60]
	lsls	r2, r4, #2
	add	r3, r3, r2
	.syntax unified
@ 113 "./arch/arm/include/asm/io.h" 1
	ldr r3, [r3]
@ 0 "" 2
	.thumb
	.syntax unified
	strd	r2, r3, [sp]
	mov	r1, r7
	mov	r3, r4
	movw	r2, #1793
	mov	r0, r8
	adds	r4, r4, #1
	bl	printk
	b	.L665
.L670:
	.align	2
.L669:
	.word	mpp_dev_debug
	.word	.LANCHOR2+435
	.word	.LC74
	.word	.LC73
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
	mov	r8, #1
	movs	r7, #0
	.pad #20
	sub	sp, sp, #20
	mov	r6, r1
	mov	r4, r0
	ldr	r9, [r3, #4]
	strh	r8, [r0, #36]	@ movhi
	str	r8, [r0, #40]
	str	r5, [r0]
	ldr	r2, [r3, #12]
	ldr	r1, .L727
	strd	r7, r7, [sp, #8]
	str	r2, [r0, #8]
	mov	r2, r7
	ldr	r3, [r3, #16]
	str	r7, [r0, #124]
	str	r7, [r0, #128]
	str	r3, [r0, #12]
	ldr	r0, [r6, #372]
	bl	of_parse_phandle
	mov	r10, r0
	cbnz	r0, .L672
.L674:
	ldr	r1, .L727+4
.L724:
	mov	r0, r5
	bl	_dev_err
.L673:
	ldr	r1, .L727+8
	mov	r0, r5
	bl	_dev_err
	mvn	r7, #18
.L671:
	mov	r0, r7
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L672:
	bl	of_device_is_available
	cmp	r0, #0
	beq	.L674
	mov	r0, r10
	bl	of_find_device_by_node
	cbnz	r0, .L675
	ldr	r1, .L727+12
	b	.L724
.L675:
	ldr	r3, [r0, #80]
	str	r3, [r4, #104]
	bl	platform_device_put
	ldr	r3, [r4, #104]
	cbnz	r3, .L676
	ldr	r1, .L727+16
	b	.L724
.L676:
	str	r7, [sp]
	mov	r3, r8
	add	r2, sp, #8
	ldr	r1, .L727+20
	ldr	r0, [r6, #372]
	bl	of_property_read_variable_u32_array
	cmp	r0, #0
	bge	.L677
	ldr	r1, .L727+24
	b	.L724
.L677:
	ldr	r1, [r4, #104]
	ldr	r2, [sp, #8]
	ldr	r3, [r1, #460]
	cmp	r3, r2
	bhi	.L678
	ldr	r1, .L727+28
.L725:
	mov	r0, r5
	bl	_dev_err
	b	.L673
.L678:
	add	r3, r2, #116
	ldr	r7, [r1, r3, lsl #2]
	cbnz	r7, .L679
	ldr	r1, .L727+32
	mov	r0, r5
	bl	_dev_err
	b	.L673
.L679:
	add	fp, r7, #116
	add	r10, r4, #108
	str	r10, [r4, #108]
	str	r10, [r4, #112]
	mov	r0, fp
	bl	mutex_lock
	ldr	r8, [r4, #116]
	cmp	r8, #0
	it	lt
	ldrlt	r8, [r7, #164]
	cmp	r8, #3
	bls	.L681
	ldr	r1, .L727+36
	mov	r2, r8
.L723:
	ldr	r0, [r4]
	bl	_dev_err
.L682:
	mov	r0, fp
	bl	mutex_unlock
	movs	r3, #0
	ldr	r1, .L727+40
	str	r3, [sp]
	add	r2, sp, #12
	movs	r3, #1
	ldr	r0, [r6, #372]
	bl	of_property_read_variable_u32_array
	cmp	r0, #0
	blt	.L687
	ldr	r1, [r4, #104]
	ldr	r2, [sp, #12]
	ldr	r3, [r1, #580]
	cmp	r3, r2
	bhi	.L688
	ldr	r1, .L727+44
	b	.L725
.L681:
	add	r3, r7, r8, lsl #2
	ldr	r2, [r3, #144]
	cbz	r2, .L683
	mov	r2, r8
	ldr	r1, .L727+48
	b	.L723
.L683:
	str	r4, [r3, #144]
	add	r1, r7, #160
	ldr	r3, [r7, #164]
	mov	r0, r8
	adds	r3, r3, #1
	str	r3, [r7, #164]
	bl	_set_bit
	ldr	r3, [r7, #132]
	add	r2, r7, #128
	str	r10, [r7, #132]
	strd	r2, r3, [r4, #108]
	str	r10, [r3]
	ldr	r3, .L727+52
	str	r8, [r4, #116]
	str	r7, [r4, #96]
	ldr	r3, [r3]
	lsls	r2, r3, #6
	bpl	.L684
	ldr	r3, [r4]
	ldr	r1, [r3, #44]
	cbnz	r1, .L685
	ldr	r1, [r3]
.L685:
	mov	r2, r8
	ldr	r0, .L727+56
	bl	printk
.L684:
	ldr	r3, [r4, #40]
	ldr	r2, [r7, #136]
	cmp	r2, r3
	it	hi
	strhi	r3, [r7, #136]
	b	.L682
.L688:
	adds	r2, r2, #146
	ldr	r3, [r1, r2, lsl #2]
	str	r3, [r4, #100]
.L687:
	ldr	r7, [r4, #40]
	cmp	r7, #1
	bne	.L690
	mov	r1, #2000
	mov	r0, r5
	bl	pm_runtime_set_autosuspend_delay
	mov	r1, r7
	mov	r0, r5
	bl	__pm_runtime_use_autosuspend
.L691:
	add	r3, r4, #16
	movs	r2, #20
	movs	r1, #0
	movs	r7, #0
	mov	r0, r3
	bl	memset
	ldr	r3, .L727+60
	str	r3, [r4, #24]
	str	r0, [r4, #16]
	str	r0, [r4, #20]
	mov	r0, r5
	str	r7, [r4, #72]
	str	r7, [r4, #76]
	str	r7, [r4, #80]
	str	r7, [r4, #84]
	ldrb	r3, [r6, #132]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r6, #132]
	ldrb	r3, [r6, #140]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r6, #140]
	bl	pm_runtime_enable
	mov	r1, r7
	mov	r0, r6
	bl	platform_get_irq
	cmp	r0, r7
	str	r0, [r4, #48]
	bge	.L692
	ldr	r1, .L727+64
.L726:
	mov	r0, r5
	mvn	r7, #18
	bl	_dev_err
.L693:
	mov	r0, r4
	bl	mpp_detach_workqueue
	ldrb	r3, [r6, #132]	@ zero_extendqisi2
	movs	r1, #1
	mov	r0, r5
	bfc	r3, #0, #1
	strb	r3, [r6, #132]
	ldrb	r3, [r6, #140]	@ zero_extendqisi2
	bfc	r3, #0, #1
	strb	r3, [r6, #140]
	bl	__pm_runtime_disable
	b	.L671
.L690:
	mov	r2, r7
	ldr	r1, .L727+68
	mov	r0, r5
	bl	_dev_info
	b	.L691
.L692:
	mov	r2, r7
	mov	r1, #512
	mov	r0, r6
	bl	platform_get_resource
	mov	r8, r0
	cbnz	r0, .L694
	ldr	r1, .L727+72
	b	.L726
.L694:
	ldrd	r1, r2, [r0]
	mov	r0, r5
	adds	r2, r2, #1
	subs	r2, r2, r1
	bl	devm_ioremap
	str	r0, [r4, #60]
	cbnz	r0, .L695
	mov	r2, r8
	ldr	r1, .L727+76
	mov	r0, r5
	mvn	r7, #11
	bl	_dev_err
	b	.L693
.L695:
	movs	r1, #4
	mov	r0, r5
	bl	__pm_runtime_resume
	ldr	r3, [r4, #8]
	str	r7, [r4, #68]
	ldr	r3, [r3]
	cbz	r3, .L696
	mov	r0, r4
	blx	r3
	mov	r7, r0
	cbz	r0, .L696
	movs	r1, #4
	mov	r0, r5
	bl	__pm_runtime_idle
	b	.L693
.L696:
	ldr	r3, [r9, #4]
	cmp	r3, #0
	blt	.L698
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #8]
	cbz	r3, .L699
	mov	r0, r4
	blx	r3
.L699:
	ldr	r2, [r9, #4]
	ldr	r3, [r4, #60]
	add	r3, r3, r2
	.syntax unified
@ 113 "./arch/arm/include/asm/io.h" 1
	ldr r6, [r3]
@ 0 "" 2
@ 717 "drivers/kmpp/mpp/mpp_common.h" 1
	dsb 
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L727+52
	ldr	r3, [r3]
	lsls	r3, r3, #18
	bpl	.L700
	mov	r3, r6
	lsrs	r1, r2, #2
	ldr	r0, .L727+80
	bl	printk
.L700:
	str	r6, [r9, #8]
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #12]
	cbz	r3, .L698
	mov	r0, r4
	blx	r3
.L698:
	movs	r1, #4
	mov	r0, r5
	bl	__pm_runtime_idle
	movs	r7, #0
	b	.L671
.L728:
	.align	2
.L727:
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
	cbz	r3, .L730
	blx	r3
.L730:
	ldr	r0, [r4, #68]
	bl	mpp_iommu_remove
	mov	r0, r4
	bl	mpp_detach_workqueue
	ldr	r3, [r4]
	movs	r1, #1
	ldrb	r2, [r3, #116]	@ zero_extendqisi2
	bfc	r2, #0, #1
	strb	r2, [r3, #116]
	ldrb	r2, [r3, #124]	@ zero_extendqisi2
	bfc	r2, #0, #1
	strb	r2, [r3, #124]
	ldr	r0, [r4]
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
	ldr	r1, .L746
	mov	r0, r5
	bl	_dev_info
	ldr	r0, [r4, #104]
	adds	r0, r0, #84
	bl	atomic_add.constprop.24
	bl	ktime_get
	ldr	r3, .L746+4
	adds	r6, r0, r3
	mov	r3, #0
	adc	r7, r1, r3
	bl	_cond_resched
.L738:
	ldr	r3, [r4, #80]
	cbz	r3, .L735
	bl	ktime_get
	cmp	r6, r0
	sbcs	r3, r7, r1
	bge	.L736
	ldr	r3, [r4, #80]
	cbz	r3, .L735
	ldr	r2, [r4, #80]
	mov	r0, r5
	ldr	r1, .L746+8
	pop	{r3, r4, r5, r6, r7, lr}
	b	_dev_err
.L736:
	movw	r1, #20000
	movw	r0, #5001
	bl	usleep_range
	b	.L738
.L735:
	mov	r0, r5
	ldr	r1, .L746+12
	pop	{r3, r4, r5, r6, r7, lr}
	b	_dev_info
.L747:
	.align	2
.L746:
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
	cbz	r4, .L750
	mov	r0, r1
	blx	r4
	mov	r4, r0
	cbz	r5, .L751
	cmp	r0, #2
	bne	.L749
	addw	r7, r5, #2200
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #2200]
	orr	r1, r3, #8
	str	r1, [r5, #2200]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	lsls	r2, r3, #28
	bpl	.L753
	ldr	r3, [r6, #52]
	movw	r2, #1965
	ldr	r1, .L761
	movs	r4, #1
	ldr	r0, .L761+4
	bl	printk
.L749:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L753:
	add	r0, r5, #2208
	bl	cancel_delayed_work
	mov	r1, r7
	movs	r0, #4
	bl	____atomic_set_bit
	b	.L749
.L752:
.L750:
	cmp	r5, #0
	bne	.L749
	mov	r4, r5
.L751:
	ldr	r3, .L761+8
	ldr	r3, [r3]
	lsls	r3, r3, #16
	bpl	.L749
	ldr	r0, .L761+12
	bl	printk
	b	.L749
.L762:
	.align	2
.L761:
	.word	.LANCHOR2+456
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
	cbz	r3, .L764
	ldr	r3, [r1, #8]
	ldr	r2, [r3, #24]
	cbz	r2, .L764
	ldr	r3, [r1, #96]
	ldr	r1, [r3, #76]!
	cmp	r1, r3
	bne	.L764
	mov	r0, r4
	blx	r2
.L764:
	ldr	r3, [r4, #12]
	ldr	r5, [r3, #32]
	cbz	r5, .L765
	mov	r0, r4
	blx	r5
	mov	r5, r0
.L765:
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
	cbz	r0, .L774
	ldr	r0, [r0, #8]
	cbz	r0, .L774
	ldr	r2, [r3, #4]
	cbz	r2, .L774
	add	r2, sp, #4
	ldr	r1, [r3]
	bl	regmap_read
.L774:
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
	cbz	r0, .L785
	ldr	r3, [r0, #8]
	cbz	r3, .L788
	ldr	r3, [r0, #4]
	cbz	r3, .L788
	bl	mpp_get_grf
	ldrh	r3, [r4, #4]
	subs	r0, r3, r0
	it	ne
	movne	r0, #1
.L785:
	pop	{r4, pc}
.L788:
	mov	r0, r3
	b	.L785
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
	cbz	r0, .L790
	ldr	r0, [r0, #8]
	cbz	r0, .L790
	ldr	r2, [r3, #4]
	cbz	r2, .L790
	ldr	r1, [r3]
	bl	regmap_write
.L790:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	mpp_set_grf, .-mpp_set_grf
	.section	.text.unlikely
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
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r1, .L836
	ldr	r0, [r0]
	bl	_dev_info
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #32]
	cmp	r3, #0
	beq	.L801
	mov	r0, r4
	blx	r3
.L802:
	ldrb	r3, [r4, #36]	@ zero_extendqisi2
	cbz	r3, .L803
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #24]
	cbz	r3, .L803
	mov	r0, r4
	blx	r3
.L803:
	ldr	r0, [r4, #68]
	cbz	r0, .L804
	bl	down_write
.L804:
	ldr	r0, [r4, #100]
	cbz	r0, .L805
	ldr	r3, [r0]
	cbz	r3, .L805
	adds	r0, r0, #4
	bl	down_write
.L805:
	movs	r3, #0
	str	r3, [r4, #72]
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #28]
	cbz	r3, .L806
	mov	r0, r4
	blx	r3
.L806:
	ldr	r1, [r4]
	ldr	r0, [r4, #68]
	bl	mpp_iommu_refresh
	ldr	r0, [r4, #100]
	cbz	r0, .L807
	ldr	r3, [r0]
	cbz	r3, .L807
	adds	r0, r0, #4
	bl	up_write
.L807:
	ldr	r0, [r4, #68]
	cbz	r0, .L808
	bl	up_write
.L808:
	ldr	r0, [r4]
	ldr	r1, .L836+4
	bl	_dev_info
	movs	r0, #0
	pop	{r4, pc}
.L801:
	ldr	r0, [r4, #64]
	bl	mpp_set_grf
	b	.L802
.L837:
	.align	2
.L836:
	.word	.LC99
	.word	.LC100
	.fnend
	.size	mpp_dev_reset, .-mpp_dev_reset
	.text
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
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r0, #-8]
	orr	r1, r3, #8
	str	r1, [r0, #-8]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	lsls	r3, r3, #28
	bpl	.L839
	movw	r2, #531
	ldr	r1, .L858
	ldr	r0, .L858+4
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	b	printk
.L839:
	ldr	r3, [r5, #2276]
	movw	r2, #535
	ldr	r1, .L858
	ldr	r0, .L858+8
	bl	printk
	ldr	r6, [r5]
	cbnz	r6, .L840
	ldr	r1, .L858
	mov	r3, r5
	ldr	r0, .L858+12
	movw	r2, #538
.L857:
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	b	printk
.L840:
	ldr	r3, [r6, #12]
	cbnz	r3, .L841
	mov	r3, r6
	mov	r2, #544
	ldr	r1, .L858
	ldr	r0, .L858+16
	b	.L857
.L841:
	ldr	r4, [r5, #2292]
	cmp	r4, #0
	it	eq
	moveq	r4, r3
	mov	r0, r4
	bl	rkvenc_dump_dbg
	ldr	r0, [r4, #100]
	cbz	r0, .L843
	ldr	r3, [r0]
	cbz	r3, .L843
	adds	r0, r0, #4
	bl	up_read
.L843:
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
	mov	r0, r5
	bl	mpp_taskqueue_pop_running.constprop.10
	ldr	r3, [r6, #248]
	cbz	r3, .L838
	cmp	r9, #0
	beq	.L838
	ldr	r0, [r6, #208]
	pop	{r4, r5, r6, r7, r8, r9, r10, lr}
	bx	r3	@ indirect register sibling call
.L838:
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L859:
	.align	2
.L858:
	.word	.LANCHOR2+468
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
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r6, r0
	ldr	r4, [r1, #2292]
	mov	r5, r1
	cbnz	r4, .L861
	ldr	r4, [r0, #12]
.L861:
	ldr	r3, [r4, #12]
	ldr	r7, [r5, #2308]
	ldr	r3, [r3, #36]
	cbz	r3, .L862
	mov	r1, r5
	mov	r0, r4
	blx	r3
.L862:
	ldr	r0, [r4, #100]
	cbz	r0, .L863
	ldr	r3, [r0]
	cbz	r3, .L863
	adds	r0, r0, #4
	bl	up_read
.L863:
	ldr	r3, [r4, #72]
	cmp	r3, #0
	ble	.L864
	mov	r0, r4
	bl	mpp_dev_reset
.L864:
	mov	r0, r4
	addw	r4, r5, #2200
	bl	mpp_power_off
	movs	r0, #5
	mov	r1, r4
	bl	____atomic_set_bit
	movs	r0, #7
	bl	____atomic_set_bit
	movs	r3, #0
	movs	r2, #1
	movs	r1, #3
	addw	r0, r5, #2284
	bl	__wake_up
	mov	r0, r5
	bl	mpp_taskqueue_pop_running.constprop.10
	ldr	r3, [r6, #248]
	cbz	r3, .L865
	cbz	r7, .L865
	ldr	r0, [r6, #208]
	blx	r3
.L865:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
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
	ldr	r3, .L889
	ldr	r3, [r3]
	lsls	r3, r3, #23
	bpl	.L882
	cbz	r0, .L882
	add	r0, r0, #2256
	bl	ktime_get_real_ts64
.L882:
	movs	r0, #0
	pop	{r3, pc}
.L890:
	.align	2
.L889:
	.word	mpp_dev_debug
	.fnend
	.size	mpp_time_record, .-mpp_time_record
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
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	sub	r9, r0, #16
	ldr	r7, .L965
	ldr	r6, [r0, #80]
	ldr	r3, [r7]
	lsls	r4, r3, #26
	bpl	.L892
	movw	r2, #762
	ldr	r1, .L965+4
	ldr	r0, .L965+8
	bl	printk
.L892:
	ldr	r10, .L965+28
.L893:
	add	r8, r6, #64
	mov	r0, r8
	bl	mutex_lock
	mov	r3, r6
	ldr	r4, [r3, #76]!
	mov	r0, r8
	cmp	r4, r3
	ite	ne
	subne	r4, r4, #20
	moveq	r4, #0
	bl	mutex_unlock
	cbz	r4, .L895
	ldr	r2, [r4, #2204]
	ldr	r3, [r4]
	cmp	r2, #0
	ble	.L896
	cmp	r3, #0
	beq	.L893
	ldr	r3, [r3, #12]
	cmp	r3, #0
	beq	.L893
	mov	r0, r8
	bl	mutex_lock
	add	r0, r4, #20
	bl	list_del_init
	mov	r0, r8
	bl	mutex_unlock
	addw	r0, r4, #2252
	bl	kref_put.constprop.14
	b	.L893
.L896:
	ldr	r9, [r3, #12]
	ldr	r3, [r9, #12]
	ldr	r3, [r3, #20]
	cbz	r3, .L898
	mov	r1, r4
	mov	r0, r9
	blx	r3
	mov	r4, r0
	cbnz	r0, .L899
.L895:
	add	r4, r6, #32
	ldr	r10, .L965+32
	add	r8, r6, #52
	mov	r0, r4
	bl	mutex_lock
.L909:
	ldr	r3, [r6, #60]
	cmp	r3, #0
	bne	.L914
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	mutex_unlock
.L898:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, r6
	ldr	r2, [r3, #84]!
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r2, r3
	bne	.L895
.L899:
	ldr	r5, [r4, #2292]
	cbnz	r5, .L900
	ldr	r3, [r4]
	ldr	r5, [r3, #12]
.L900:
	mov	r0, r8
	bl	mutex_lock
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	fp, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r1, r6, #84
	add	r0, r4, #20
	bl	list_move_tail
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, fp	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r8
	addw	r8, r4, #2200
	bl	mutex_unlock
	movs	r0, #1
	mov	r1, r8
	bl	____atomic_set_bit
	ldr	r3, [r7]
	lsls	r0, r3, #26
	bpl	.L901
	movw	r2, #717
	mov	r1, r10
	ldr	r0, .L965+8
	bl	printk
.L901:
	ldr	r3, [r5, #8]
	ldr	r3, [r3, #32]
	cbz	r3, .L902
	mov	r0, r5
	blx	r3
	cbz	r0, .L903
	ldr	r0, [r5]
	ldr	r1, .L965+12
	bl	_dev_err
	mov	r0, r4
	bl	mpp_taskqueue_pop_running.constprop.10
	b	.L895
.L902:
	ldr	r0, [r5, #64]
	bl	mpp_set_grf
.L903:
	mov	r0, r5
	bl	mpp_power_on
	mov	r0, r4
	bl	mpp_time_record
	ldr	r3, [r7]
	lsls	r1, r3, #22
	bpl	.L904
	ldr	r2, [r5]
	ldr	r3, [r2, #44]
	cbnz	r3, .L905
	ldr	r3, [r2]
.L905:
	ldr	r2, [r4]
	mov	r1, r10
	str	r3, [sp]
	ldr	r0, .L965+16
	ldr	r3, [r2, #40]
	movw	r2, #735
	bl	printk
.L904:
	ldrb	r3, [r5, #36]	@ zero_extendqisi2
	cbz	r3, .L906
	ldr	r3, [r5, #8]
	ldr	r3, [r3, #20]
	cbz	r3, .L906
	mov	r1, r4
	mov	r0, r5
	blx	r3
.L906:
	ldr	r0, [r5, #100]
	cbz	r0, .L907
	ldr	r3, [r0]
	cbz	r3, .L907
	adds	r0, r0, #4
	bl	down_read
.L907:
	mov	r1, r8
	movs	r0, #2
	bl	____atomic_set_bit
	ldr	r1, .L965+20
	movs	r3, #60
	add	r2, r4, #2208
	movs	r0, #1
	ldr	r1, [r1]
	bl	queue_delayed_work_on
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #24]
	cbz	r3, .L908
	mov	r1, r4
	mov	r0, r5
	blx	r3
.L908:
	ldr	r3, [r7]
	lsls	r2, r3, #26
	bpl	.L893
	movw	r2, #751
	mov	r1, r10
	ldr	r0, .L965+24
	bl	printk
	b	.L893
.L914:
	ldr	r0, [r6, #52]
	cmp	r0, r8
	beq	.L916
	subs	r5, r0, #188
	beq	.L910
	bl	list_del_init
	ldr	r3, [r6, #60]
	subs	r3, r3, #1
	str	r3, [r6, #60]
.L910:
	mov	r0, r4
	bl	mutex_unlock
	cbz	r5, .L911
	ldr	r3, [r7]
	lsls	r3, r3, #9
	bpl	.L912
	ldr	r3, [r9]
	ldr	r1, [r3, #44]
	cbnz	r1, .L913
	ldr	r1, [r3]
.L913:
	ldr	r2, [r6, #60]
	mov	r0, r10
	bl	printk
.L912:
	mov	r0, r5
	bl	mpp_session_deinit
.L911:
	mov	r0, r4
	bl	mutex_lock
	b	.L909
.L916:
	movs	r5, #0
	b	.L910
.L966:
	.align	2
.L965:
	.word	mpp_dev_debug
	.word	.LANCHOR2+490
	.word	.LC12
	.word	.LC104
	.word	.LC105
	.word	system_wq
	.word	.LC13
	.word	.LANCHOR2+514
	.word	.LC106
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
	ldr	r5, [r0, #2292]
	.pad #24
	sub	sp, sp, #24
	cbnz	r5, .L968
	ldr	r3, [r0]
	ldr	r5, [r3, #12]
.L968:
	add	r0, sp, #8
	bl	ktime_get_real_ts64
	ldr	r3, .L974
	ldr	r3, [r3]
	lsls	r3, r3, #23
	bpl	.L969
	ldr	r3, [r5]
	ldr	r1, [r3, #44]
	cbnz	r1, .L970
	ldr	r1, [r3]
.L970:
	ldr	r6, [r4, #2256]
	ldr	r2, [sp, #8]
	ldr	r0, [r4, #2260]
	subs	r2, r2, r6
	ldr	r6, .L974+4
	ldr	r3, [sp, #12]
	ldr	r5, [r4]
	umull	fp, ip, r2, r6
	sbc	r0, r3, r0
	ldr	r4, [r4, #2264]
	mla	ip, r6, r0, ip
	ldr	r0, [sp, #16]
	ldr	r6, .L974+8
	subs	r0, r0, r4
	smull	r6, r7, r0, r6
	asrs	r0, r0, #31
	rsb	r0, r0, r7, asr #6
	adds	r2, fp, r0
	adc	r3, ip, r0, asr #31
	ldr	r0, .L974+12
	strd	r2, [sp]
	mov	r3, r5
	ldr	r2, [r5, #40]
	bl	printk
.L969:
	movs	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, fp, pc}
.L975:
	.align	2
.L974:
	.word	mpp_dev_debug
	.word	1000000
	.word	274877907
	.word	.LC107
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
	ldr	r10, .L984
	mov	r9, r1
	ldr	fp, .L984+4
	mov	r7, r3
	mov	r4, r2
.L977:
	cmp	r7, r4
	bhi	.L980
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L980:
	ldr	r3, [sp, #40]
	cmp	r3, r4
	beq	.L978
	ldr	r3, [r10]
	lsls	r6, r4, #2
	ldr	r5, [r9, r4, lsl #2]
	lsls	r3, r3, #19
	bpl	.L979
	mov	r3, r5
	mov	r2, r6
	bic	r1, r4, #-1073741824
	mov	r0, fp
	bl	printk
.L979:
	ldr	r3, [r8, #60]
	add	r6, r6, r3
	.syntax unified
@ 95 "./arch/arm/include/asm/io.h" 1
	str r5, [r6]
@ 0 "" 2
	.thumb
	.syntax unified
.L978:
	adds	r4, r4, #1
	b	.L977
.L985:
	.align	2
.L984:
	.word	mpp_dev_debug
	.word	.LC108
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
	ldr	r8, .L993
	mov	r7, r1
	ldr	r9, .L993+4
	mov	r5, r3
	mov	r4, r2
.L987:
	cmp	r5, r4
	bhi	.L989
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L989:
	ldr	r3, [r6, #60]
	lsls	r2, r4, #2
	add	r3, r3, r2
	.syntax unified
@ 113 "./arch/arm/include/asm/io.h" 1
	ldr r10, [r3]
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r8]
	lsls	r3, r3, #18
	bpl	.L988
	mov	r3, r10
	bic	r1, r4, #-1073741824
	mov	r0, r9
	bl	printk
.L988:
	str	r10, [r7, r4, lsl #2]
	adds	r4, r4, #1
	b	.L987
.L994:
	.align	2
.L993:
	.word	mpp_dev_debug
	.word	.LC109
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
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r4, r0
	ldr	r3, [r0]
	mov	r6, r1
	ldr	r1, .L998
	mov	r7, r2
	ldr	r0, [r3, #356]
	bl	of_property_match_string
	subs	r5, r0, #0
	blt	.L997
	mov	r1, r7
	ldr	r0, [r4]
	bl	devm_clk_get
	mov	r3, r6
	mov	r2, r5
	ldr	r1, .L998+4
	str	r0, [r3], #8
	ldr	r0, [r4]
	ldr	r0, [r0, #356]
	bl	of_property_read_u32_index
	ldr	r0, [r4]
	add	r3, r6, #12
	mov	r2, r5
	ldr	r1, .L998+8
	ldr	r0, [r0, #356]
	bl	of_property_read_u32_index
	movs	r0, #0
.L995:
	pop	{r3, r4, r5, r6, r7, pc}
.L997:
	mvn	r0, #21
	b	.L995
.L999:
	.align	2
.L998:
	.word	.LC110
	.word	.LC111
	.word	.LC112
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
	cbz	r1, .L1001
	cbz	r2, .L1001
	cmp	r3, #4
	bhi	.L1002
	tbb	[pc, r3]
.L1004:
	.byte	(.L1008-.L1004)/2
	.byte	(.L1007-.L1004)/2
	.byte	(.L1006-.L1004)/2
	.byte	(.L1005-.L1004)/2
	.byte	(.L1003-.L1004)/2
	.p2align 1
.L1007:
	str	r2, [r0, #4]
.L1001:
	movs	r0, #0
	pop	{r3, pc}
.L1006:
	str	r2, [r0, #20]
	b	.L1001
.L1005:
	str	r2, [r0, #8]
	b	.L1001
.L1003:
	str	r2, [r0, #12]
	b	.L1001
.L1008:
	str	r2, [r0, #16]
	b	.L1001
.L1002:
	movw	r2, #2124
	ldr	r1, .L1015
	ldr	r0, .L1015+4
	bl	printk
	b	.L1001
.L1016:
	.align	2
.L1015:
	.word	.LANCHOR2+527
	.word	.LC113
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
	cbz	r0, .L1017
	ldr	r0, [r2, #4]
	cbnz	r0, .L1017
	cmp	r1, #3
	beq	.L1019
	cmp	r1, #4
	beq	.L1020
	cmp	r1, #2
	bne	.L1021
	ldr	r0, [r2, #20]
	ldr	r3, .L1026
	cmp	r0, #0
	it	eq
	moveq	r0, r3
	bx	lr
.L1020:
	ldr	r0, [r2, #12]
	cbnz	r0, .L1017
.L1019:
	ldr	r0, [r2, #8]
	cbnz	r0, .L1017
.L1021:
	ldr	r0, [r2, #16]
.L1017:
	bx	lr
.L1027:
	.align	2
.L1026:
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
	cbz	r4, .L1030
	bl	mpp_get_clk_info_rate_hz
	cbz	r0, .L1028
	str	r0, [r5, #24]
	mov	r1, r0
	mov	r0, r4
	bl	clk_set_rate
	movs	r0, #0
.L1028:
	pop	{r3, r4, r5, pc}
.L1030:
	mvn	r0, #21
	b	.L1028
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
	ldr	r3, .L1033
	bl	proc_create_data
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L1034:
	.align	2
.L1033:
	.word	.LANCHOR2+552
	.fnend
	.size	mpp_procfs_create_u32, .-mpp_procfs_create_u32
	.global	g_mpp_svr_fn_ops
	.global	g_srv
	.global	rockchip_mpp_fops
	.global	enc_info_item_name
	.global	mpp_device_name
	.section	.rodata
	.align	2
	.set	.LANCHOR2,. + 0
	.type	__func__.41308, %object
	.size	__func__.41308, 23
__func__.41308:
	.ascii	"mpp_chnl_get_iova_addr\000"
	.type	__func__.40480, %object
	.size	__func__.40480, 25
__func__.40480:
	.ascii	"mpp_process_task_default\000"
	.type	__func__.40972, %object
	.size	__func__.40972, 13
__func__.40972:
	.ascii	"mpp_dev_open\000"
	.type	__func__.40454, %object
	.size	__func__.40454, 14
__func__.40454:
	.ascii	"mpp_free_task\000"
	.type	__func__.40632, %object
	.size	__func__.40632, 17
__func__.40632:
	.ascii	"mpp_get_cmd_butt\000"
	.type	__func__.40811, %object
	.size	__func__.40811, 20
__func__.40811:
	.ascii	"mpp_process_request\000"
	.type	__func__.40616, %object
	.size	__func__.40616, 17
__func__.40616:
	.ascii	"mpp_parse_msg_v1\000"
	.type	__func__.41296, %object
	.size	__func__.41296, 17
__func__.41296:
	.ascii	"mpp_chnl_add_req\000"
	.type	__func__.40553, %object
	.size	__func__.40553, 24
__func__.40553:
	.ascii	"mpp_wait_result_default\000"
	.type	__func__.40282, %object
	.size	__func__.40282, 14
__func__.40282:
	.ascii	"get_task_msgs\000"
	.type	__func__.40952, %object
	.size	__func__.40952, 14
__func__.40952:
	.ascii	"mpp_dev_ioctl\000"
	.type	__func__.40881, %object
	.size	__func__.40881, 17
__func__.40881:
	.ascii	"mpp_collect_msgs\000"
	.type	__func__.40939, %object
	.size	__func__.40939, 14
__func__.40939:
	.ascii	"mpp_msgs_wait\000"
	.type	__func__.40978, %object
	.size	__func__.40978, 16
__func__.40978:
	.ascii	"mpp_dev_release\000"
	.type	__func__.41286, %object
	.size	__func__.41286, 17
__func__.41286:
	.ascii	"mpp_chnl_release\000"
.LC53:
	.ascii	"shared_\000"
	.space	24
	.type	__func__.40988, %object
	.size	__func__.40988, 14
__func__.40988:
	.ascii	"mpp_check_req\000"
	.type	__func__.40998, %object
	.size	__func__.40998, 28
__func__.40998:
	.ascii	"mpp_extract_reg_offset_info\000"
	.type	__func__.41003, %object
	.size	__func__.41003, 26
__func__.41003:
	.ascii	"mpp_query_reg_offset_info\000"
	.type	__func__.41013, %object
	.size	__func__.41013, 30
__func__.41013:
	.ascii	"mpp_translate_reg_offset_info\000"
	.type	__func__.41042, %object
	.size	__func__.41042, 25
__func__.41042:
	.ascii	"mpp_task_dump_mem_region\000"
	.type	__func__.41065, %object
	.size	__func__.41065, 18
__func__.41065:
	.ascii	"mpp_task_dump_reg\000"
	.type	__func__.41079, %object
	.size	__func__.41079, 21
__func__.41079:
	.ascii	"mpp_task_dump_hw_reg\000"
	.type	__func__.41151, %object
	.size	__func__.41151, 12
__func__.41151:
	.ascii	"mpp_dev_irq\000"
	.type	__func__.40473, %object
	.size	__func__.40473, 22
__func__.40473:
	.ascii	"mpp_task_timeout_work\000"
	.type	__func__.40522, %object
	.size	__func__.40522, 24
__func__.40522:
	.ascii	"mpp_task_worker_default\000"
	.type	__func__.40510, %object
	.size	__func__.40510, 13
__func__.40510:
	.ascii	"mpp_task_run\000"
	.type	__func__.41220, %object
	.size	__func__.41220, 25
__func__.41220:
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
	.size	g_mpp_svr_fn_ops, 28
g_mpp_svr_fn_ops:
	.word	mpp_chnl_open
	.word	mpp_chnl_register
	.word	mpp_chnl_release
	.word	mpp_chnl_add_req
	.word	mpp_chnl_get_iova_addr
	.word	0
	.word	mpp_chnl_get_dev
	.type	enc_info_item_name, %object
	.size	enc_info_item_name, 44
enc_info_item_name:
	.word	.LC114
	.word	.LC115
	.word	.LC116
	.word	.LC117
	.word	.LC118
	.word	.LC119
	.word	.LC120
	.word	.LC121
	.word	.LC122
	.word	.LC123
	.word	.LC124
	.type	mpp_device_name, %object
	.size	mpp_device_name, 116
mpp_device_name:
	.word	.LC125
	.word	.LC126
	.word	.LC127
	.word	.LC128
	.word	.LC129
	.space	12
	.word	.LC130
	.word	.LC131
	.space	8
	.word	.LC132
	.space	12
	.word	.LC133
	.word	.LC134
	.word	.LC135
	.space	20
	.word	.LC136
	.space	12
	.word	.LC137
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	g_srv, %object
	.size	g_srv, 4
g_srv:
	.space	4
	.type	__key.40481, %object
	.size	__key.40481, 0
__key.40481:
	.type	__key.40389, %object
	.size	__key.40389, 0
__key.40389:
	.type	__key.40589, %object
	.size	__key.40589, 0
__key.40589:
	.type	__key.40590, %object
	.size	__key.40590, 0
__key.40590:
	.type	__key.40592, %object
	.size	__key.40592, 0
__key.40592:
	.type	__key.40593, %object
	.size	__key.40593, 0
__key.40593:
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016rk_vcodec: session %p:%d attach\012\000"
.LC1:
	.ascii	"\0013rk_vcodec: invalid NULL process task function\012"
	.ascii	"\000"
.LC2:
	.ascii	"\0013rk_vcodec: invalid NULL wait result function\012"
	.ascii	"\000"
.LC3:
	.ascii	"%d\012\000"
.LC4:
	.ascii	"\0013rk_vcodec: %s:%d: session is null\000"
.LC5:
	.ascii	"\0013rk_vcodec: %s:%d: pid %d client %d found inval"
	.ascii	"id process function\012\000"
.LC6:
	.ascii	"\0013rk_vcodec: %s:%d: alloc_task failed.\012\000"
.LC7:
	.ascii	"&task->wait\000"
.LC8:
	.ascii	"\0013rk_vcodec: invalid msgs without session\012\000"
.LC9:
	.ascii	"\0016rk_vcodec: session %p:%d detach\012\000"
.LC10:
	.ascii	"&session->pending_lock\000"
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
	.ascii	"\0016rk_vcodec: %s:%d: task %d:%d free state 0x%lx "
	.ascii	"abort %d\012\000"
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
	.ascii	"\0016rk_vcodec: cmd %x, flags %08x, size %d, offset"
	.ascii	" %x\012\000"
.LC28:
	.ascii	"\0013rk_vcodec: %s:%d: mpp cmd %x is not supproted."
	.ascii	"\012\000"
.LC29:
	.ascii	"\0013rk_vcodec: %s:%d: fail, message count %d more "
	.ascii	"than %d.\012\000"
.LC30:
	.ascii	"\0013rk_vcodec: %s:%d: session %p pending list is e"
	.ascii	"mpty!\012\000"
.LC31:
	.ascii	"\0013rk_vcodec: %s:%d: timeout, pid %d session %p:%"
	.ascii	"d count %d cur_task %p index %d.\012\000"
.LC32:
	.ascii	"\0016rk_vcodec: %s:%d: task %d kref_%d\012\000"
.LC33:
	.ascii	"\0016rk_vcodec: %s:%d: session %p:%d msgs cnt %d\012"
	.ascii	"\000"
.LC34:
	.ascii	"\0013rk_vcodec: %s:%d: session %p\012\000"
.LC35:
	.ascii	"\0016rk_vcodec: release session had request\012\000"
.LC36:
	.ascii	"\0016rk_vcodec: shutdown had request\012\000"
.LC37:
	.ascii	"\0013rk_vcodec: %s:%d: unknown ioctl cmd %x\012\000"
.LC38:
	.ascii	"\0016rk_vcodec: cmd %x collect flags %08x, size %d,"
	.ascii	" offset %x\012\000"
.LC39:
	.ascii	"\0013rk_vcodec: %s:%d: mpp cmd %x is not supported."
	.ascii	"\012\000"
.LC40:
	.ascii	"\0013rk_vcodec: %s:%d: fd %d get session failed\012"
	.ascii	"\000"
.LC41:
	.ascii	"\0013rk_vcodec: %s:%d: copy_to_user failed.\012\000"
.LC42:
	.ascii	"\0016rk_vcodec: fd %d, session %d msg_cnt %d\012\000"
.LC43:
	.ascii	"\0013rk_vcodec: session %p:%d failed to get task ms"
	.ascii	"gs\000"
.LC44:
	.ascii	"\0013rk_vcodec: %s:%d: session %d message count %d "
	.ascii	"more than %d.\012\000"
.LC45:
	.ascii	"\0013rk_vcodec: %s:%d: session %d process cmd %x re"
	.ascii	"t %d\012\000"
.LC46:
	.ascii	"\0013rk_vcodec: %s:%d: collect msgs failed %d\012\000"
.LC47:
	.ascii	"\0016rk_vcodec: try to trigger abort task %d\012\000"
.LC48:
	.ascii	"\0013rk_vcodec: %s:%d: session %d wait result ret %"
	.ascii	"d\012\000"
.LC49:
	.ascii	"\0016rk_vcodec: session %p:%d task %d release\012\000"
.LC50:
	.ascii	"\0013rk_vcodec: invalid NULL session deinit functio"
	.ascii	"n\012\000"
.LC51:
	.ascii	"\0016rk_vcodec: session %p:%d deinit\012\000"
.LC52:
	.ascii	"\0013rk_vcodec: %s:%d: session is null\012\000"
.LC54:
	.ascii	"reset-names\000"
.LC55:
	.ascii	"%s is not found!\012\000"
.LC56:
	.ascii	"reset group is empty!\012\000"
.LC57:
	.ascii	"reset_group->rw_sem_on=%d\012\000"
.LC58:
	.ascii	"&queue->session_lock\000"
.LC59:
	.ascii	"&queue->pending_lock\000"
.LC60:
	.ascii	"&queue->mmu_lock\000"
.LC61:
	.ascii	"&queue->dev_lock\000"
.LC62:
	.ascii	"\0013rk_vcodec: %s:%d: error: base %x, offset %x\012"
	.ascii	"\000"
.LC63:
	.ascii	"\0013rk_vcodec: %s:%d: error: req_off %x, req_size "
	.ascii	"%x, off_s %x\012\000"
.LC64:
	.ascii	"\0013rk_vcodec: %s:%d: error: off_e %x, max_size %x"
	.ascii	"\012\000"
.LC65:
	.ascii	"\0013rk_vcodec: %s:%d: error: req_off %x, max_size "
	.ascii	"%x\012\000"
.LC66:
	.ascii	"\0013rk_vcodec: %s:%d: error: req_off %x, req_size "
	.ascii	"%x, max_size %x\012\000"
.LC67:
	.ascii	"\0013rk_vcodec: %s:%d: count %d, total %d, max_size"
	.ascii	" %d\012\000"
.LC68:
	.ascii	"\0016rk_vcodec: reg[%d] + offset %d\012\000"
.LC69:
	.ascii	"\0013rk_vcodec: %s:%d: --- dump mem region ---\012\000"
.LC70:
	.ascii	"\0013rk_vcodec: %s:%d: reg[%3d]: %pad, size %lx\012"
	.ascii	"\000"
.LC71:
	.ascii	"no memory region mapped\012\000"
.LC72:
	.ascii	"\0013rk_vcodec: %s:%d: --- dump task register ---\012"
	.ascii	"\000"
.LC73:
	.ascii	"\0013rk_vcodec: %s:%d: reg[%03d]: %04x: 0x%08x\012\000"
.LC74:
	.ascii	"\0013rk_vcodec: %s:%d: --- dump hardware register -"
	.ascii	"--\012\000"
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
	.ascii	"resetting...\012\000"
.LC100:
	.ascii	"reset done\012\000"
.LC101:
	.ascii	"\0013rk_vcodec: %s:%d: task has been handled\012\000"
.LC102:
	.ascii	"\0013rk_vcodec: %s:%d: task %d processing time out!"
	.ascii	"\012\000"
.LC103:
	.ascii	"\0013rk_vcodec: %s:%d: session %p, session->mpp is "
	.ascii	"null.\012\000"
.LC104:
	.ascii	"set grf failed\012\000"
.LC105:
	.ascii	"\0016rk_vcodec: %s:%d: pid %d run %s\012\000"
.LC106:
	.ascii	"\0016rk_vcodec: %s detach count %d\012\000"
.LC107:
	.ascii	"\0016rk_vcodec: %s: pid: %d, session: %p, time: %ll"
	.ascii	"d us\012\000"
.LC108:
	.ascii	"\0016rk_vcodec: write reg[%03d]: %04x: 0x%08x\012\000"
.LC109:
	.ascii	"\0016rk_vcodec: read reg[%03d] %04x: 0x%08x\012\000"
.LC110:
	.ascii	"clock-names\000"
.LC111:
	.ascii	"rockchip,normal-rates\000"
.LC112:
	.ascii	"rockchip,advanced-rates\000"
.LC113:
	.ascii	"\0013rk_vcodec: %s:%d: error mode %d\012\000"
.LC114:
	.ascii	"null\000"
.LC115:
	.ascii	"width\000"
.LC116:
	.ascii	"height\000"
.LC117:
	.ascii	"format\000"
.LC118:
	.ascii	"fps_in\000"
.LC119:
	.ascii	"fps_out\000"
.LC120:
	.ascii	"rc_mode\000"
.LC121:
	.ascii	"bitrate\000"
.LC122:
	.ascii	"gop_size\000"
.LC123:
	.ascii	"fps_calc\000"
.LC124:
	.ascii	"profile\000"
.LC125:
	.ascii	"VDPU1\000"
.LC126:
	.ascii	"VDPU2\000"
.LC127:
	.ascii	"VDPU1_PP\000"
.LC128:
	.ascii	"VDPU2_PP\000"
.LC129:
	.ascii	"AV1DEC\000"
.LC130:
	.ascii	"HEVC_DEC\000"
.LC131:
	.ascii	"RKVDEC\000"
.LC132:
	.ascii	"AVSPLUS_DEC\000"
.LC133:
	.ascii	"RKVENC\000"
.LC134:
	.ascii	"VEPU1\000"
.LC135:
	.ascii	"VEPU2\000"
.LC136:
	.ascii	"VEPU22\000"
.LC137:
	.ascii	"IEP2\000"
	.section	___ksymtab+get_mppdev_svr_ops,"a"
	.align	2
	.type	__ksymtab_get_mppdev_svr_ops, %object
	.size	__ksymtab_get_mppdev_svr_ops, 12
__ksymtab_get_mppdev_svr_ops:
	.word	get_mppdev_svr_ops
	.word	__kstrtab_get_mppdev_svr_ops
	.word	__kstrtabns_get_mppdev_svr_ops
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
