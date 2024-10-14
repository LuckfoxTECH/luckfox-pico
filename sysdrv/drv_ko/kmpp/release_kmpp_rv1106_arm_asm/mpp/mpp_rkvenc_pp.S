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
	.file	"mpp_rkvenc_pp.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_rockchip_vepu_pp_driver:					
	.asciz 	"rockchip_vepu_pp_driver"					
__kstrtabns_rockchip_vepu_pp_driver:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_free_task, %function
vepu_pp_free_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r1
	bl	mpp_task_finalize
	mov	r0, r4
	bl	kfree
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	vepu_pp_free_task, .-vepu_pp_free_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_result, %function
vepu_pp_result:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	addw	r4, r1, #3736
	addw	r7, r1, #2888
	mov	r6, r1
	movs	r5, #0
.L3:
	ldr	r3, [r6, #3724]
	adds	r4, r4, #20
	cmp	r3, r5
	bhi	.L4
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L4:
	ldr	r2, [r4, #-20]
	mov	r1, r7
	ldr	r0, [r4, #-12]
	adds	r5, r5, #1
	bl	memcpy
	b	.L3
	.fnend
	.size	vepu_pp_result, .-vepu_pp_result
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_read, %function
mpp_read:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r2, r1
	ldr	r3, [r0, #60]
	add	r3, r3, r1
	.syntax unified
@ 113 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/io.h" 1
	ldr r4, [r3]
@ 0 "" 2
@ 747 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_common.h" 1
	dsb 
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L10
	ldr	r3, [r3]
	lsls	r3, r3, #18
	bpl	.L5
	mov	r3, r4
	lsrs	r1, r1, #2
	ldr	r0, .L10+4
	bl	printk
.L5:
	mov	r0, r4
	pop	{r4, pc}
.L11:
	.align	2
.L10:
	.word	mpp_dev_debug
	.word	.LC0
	.fnend
	.size	mpp_read, .-mpp_read
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_write_relaxed, %function
mpp_write_relaxed:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L17
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r3, [r3]
	mov	r4, r1
	mov	r5, r2
	lsls	r3, r3, #19
	bpl	.L13
	mov	r3, r2
	ldr	r0, .L17+4
	mov	r2, r1
	lsrs	r1, r1, #2
	bl	printk
.L13:
	ldr	r1, [r6, #60]
	add	r1, r1, r4
	.syntax unified
@ 95 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/io.h" 1
	str r5, [r1]
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L18:
	.align	2
.L17:
	.word	mpp_dev_debug
	.word	.LC1
	.fnend
	.size	mpp_write_relaxed, .-mpp_write_relaxed
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_write, %function
mpp_write:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L24
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r3, [r3]
	mov	r4, r1
	mov	r5, r2
	lsls	r3, r3, #19
	bpl	.L20
	mov	r3, r2
	ldr	r0, .L24+4
	mov	r2, r1
	lsrs	r1, r1, #2
	bl	printk
.L20:
	.syntax unified
@ 726 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_common.h" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r1, [r6, #60]
	add	r1, r1, r4
	.syntax unified
@ 95 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/io.h" 1
	str r5, [r1]
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L25:
	.align	2
.L24:
	.word	mpp_dev_debug
	.word	.LC1
	.fnend
	.size	mpp_write, .-mpp_write
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_irq, %function
vepu_pp_irq:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r1, #44
	mov	r4, r0
	bl	mpp_read
	movs	r2, #63
	movs	r1, #40
	str	r0, [r4, #52]
	mov	r0, r4
	bl	mpp_write
	ldr	r3, [r4, #52]
	tst	r3, #1
	ite	ne
	movne	r0, #2
	moveq	r0, #0
	pop	{r4, pc}
	.fnend
	.size	vepu_pp_irq, .-vepu_pp_irq
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_set_freq, %function
vepu_pp_set_freq:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	adds	r0, r0, #152
	ldr	r1, [r1, #2340]
	bl	mpp_clk_set_rate
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	vepu_pp_set_freq, .-vepu_pp_set_freq
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	clk_prepare_enable, %function
clk_prepare_enable:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	bl	clk_prepare
	mov	r4, r0
	cbnz	r0, .L30
	mov	r0, r5
	bl	clk_enable
	mov	r4, r0
	cbz	r0, .L30
	mov	r0, r5
	bl	clk_unprepare
.L30:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
	.fnend
	.size	clk_prepare_enable, .-clk_prepare_enable
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_init, %function
vepu_pp_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	add	r5, r0, #152
	ldr	r3, [r0, #104]
	mov	r4, r0
	ldr	r2, .L46
	mov	r1, r5
	adds	r3, r3, #196
	str	r3, [r0, #64]
	bl	mpp_get_clk_info
	cbz	r0, .L36
	movw	r2, #517
	ldr	r1, .L46+4
	ldr	r0, .L46+8
	bl	printk
.L36:
	ldr	r2, .L46+12
	add	r1, r4, #180
	mov	r0, r4
	bl	mpp_get_clk_info
	cbz	r0, .L37
	mov	r2, #520
	ldr	r1, .L46+4
	ldr	r0, .L46+16
	bl	printk
.L37:
	ldr	r2, .L46+20
	movs	r1, #0
	mov	r0, r5
	bl	mpp_set_clk_info_rate_hz
	ldr	r2, .L46+24
	movs	r1, #0
	mov	r0, r4
	bl	mpp_reset_control_get
	str	r0, [r4, #236]
	cbnz	r0, .L38
	movw	r2, #526
	ldr	r1, .L46+4
	ldr	r0, .L46+28
	bl	printk
.L38:
	ldr	r2, .L46+32
	movs	r1, #1
	mov	r0, r4
	bl	mpp_reset_control_get
	str	r0, [r4, #240]
	cbnz	r0, .L39
	movw	r2, #529
	ldr	r1, .L46+4
	ldr	r0, .L46+36
	bl	printk
.L39:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L47:
	.align	2
.L46:
	.word	.LC2
	.word	.LANCHOR0
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	300000000
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.fnend
	.size	vepu_pp_init, .-vepu_pp_init
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_remove, %function
vepu_pp_remove:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	adds	r0, r0, #16
	ldr	r4, [r0, #64]
	ldr	r1, .L49
	bl	_dev_info
	mov	r0, r4
	bl	mpp_dev_remove
	movs	r0, #0
	pop	{r4, pc}
.L50:
	.align	2
.L49:
	.word	.LC10
	.fnend
	.size	vepu_pp_remove, .-vepu_pp_remove
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_probe, %function
vepu_pp_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	add	r5, r0, #16
	ldr	r1, .L64
	mov	r6, r0
	mov	r0, r5
	bl	_dev_info
	mov	r2, #3520
	movs	r1, #248
	mov	r0, r5
	bl	devm_kmalloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L59
	str	r0, [r6, #80]
	mov	r0, r5
	bl	mpp_dev_of_node
	cbz	r0, .L54
	mov	r0, r5
	bl	mpp_dev_of_node
	mov	r1, r0
	ldr	r0, .L64+4
	bl	of_match_node
	cbz	r0, .L54
	ldr	r3, [r0, #192]
	str	r3, [r4, #4]
.L54:
	mov	r1, r6
	mov	r0, r4
	bl	mpp_dev_probe
	cbz	r0, .L56
	ldr	r1, .L64+8
.L63:
	mov	r0, r5
	mvn	r6, #21
	bl	_dev_err
.L51:
	mov	r0, r6
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L56:
	ldr	r3, [r6, #60]
	cbnz	r3, .L57
	ldr	r3, [r6, #16]
.L57:
	strd	r3, r4, [sp, #4]
	mov	r0, r5
	movs	r3, #128
	ldr	r2, .L64+12
	str	r3, [sp]
	ldr	r3, .L64+16
	ldr	r1, [r4, #48]
	bl	devm_request_threaded_irq
	mov	r6, r0
	cbz	r0, .L58
	ldr	r1, .L64+20
	b	.L63
.L58:
	movs	r3, #20
	ldr	r1, [r4, #104]
	str	r3, [r4, #92]
	mov	r0, r4
	bl	mpp_dev_register_srv
	ldr	r1, .L64+24
	mov	r0, r5
	bl	_dev_info
	b	.L51
.L59:
	mvn	r6, #11
	b	.L51
.L65:
	.align	2
.L64:
	.word	.LC11
	.word	.LANCHOR0+16
	.word	.LC12
	.word	mpp_dev_irq
	.word	mpp_dev_isr_sched
	.word	.LC13
	.word	.LC14
	.fnend
	.size	vepu_pp_probe, .-vepu_pp_probe
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_finish, %function
vepu_pp_finish:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r8, r0
	ldr	r4, .L77
	mov	r6, r1
	ldr	r2, [r4]
	lsls	r2, r2, #26
	bpl	.L67
	mov	r2, #428
	ldr	r1, .L77+4
	ldr	r0, .L77+8
	bl	printk
.L67:
	addw	r6, r6, #2884
	mov	r5, #2048
	movw	r7, #2072
.L68:
	mov	r1, r5
	mov	r0, r8
	bl	mpp_read
	adds	r5, r5, #4
	cmp	r5, r7
	str	r0, [r6, #4]!
	bne	.L68
	ldr	r3, [r4]
	lsls	r3, r3, #26
	bpl	.L69
	movw	r2, #433
	ldr	r1, .L77+4
	ldr	r0, .L77+12
	bl	printk
.L69:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
.L78:
	.align	2
.L77:
	.word	mpp_dev_debug
	.word	.LANCHOR0+408
	.word	.LC15
	.word	.LC16
	.fnend
	.size	vepu_pp_finish, .-vepu_pp_finish
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_run, %function
vepu_pp_run:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L98
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r2, [r3]
	mov	r5, r1
	mov	r8, r3
	tst	r2, #32
	beq	.L80
	movw	r2, #355
	ldr	r1, .L98+4
	ldr	r0, .L98+8
	bl	printk
.L80:
	movs	r2, #63
	movs	r1, #32
	str	r5, [r4, #88]
	mov	r0, r4
	bl	mpp_write_relaxed
	movs	r2, #0
	movs	r1, #36
	mov	r0, r4
	bl	mpp_write_relaxed
	movs	r2, #63
	movs	r1, #40
	mov	r0, r4
	bl	mpp_write_relaxed
	movs	r2, #7
	movs	r1, #48
	mov	r0, r4
	bl	mpp_write_relaxed
	movs	r2, #31
	movs	r1, #88
	mov	r0, r4
	bl	mpp_write_relaxed
	movs	r2, #0
	movs	r1, #92
	mov	r0, r4
	bl	mpp_write_relaxed
	movw	r2, #65535
	movs	r1, #96
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2868]
	mov	r1, #268
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2872]
	mov	r1, #272
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2876]
	mov	r1, #276
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2840]
	movs	r1, #60
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2844]
	movs	r1, #64
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2848]
	movs	r1, #68
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2852]
	movs	r1, #72
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2856]
	movs	r1, #76
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2860]
	movs	r1, #80
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2864]
	movs	r1, #84
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2344]
	movs	r1, #52
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2348]
	movs	r1, #56
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2352]
	mov	r1, #256
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2356]
	mov	r1, #260
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2360]
	mov	r1, #264
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2364]
	mov	r1, #280
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2368]
	mov	r1, #284
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r3, [r5, #2388]
	lsls	r6, r3, #31
	bmi	.L81
.L85:
	ldr	r3, [r5, #2444]
	lsls	r0, r3, #31
	bmi	.L82
.L83:
	ldr	r2, [r5, #2884]
	mov	r1, #1024
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2472]
	movw	r1, #1028
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2476]
	mov	r1, #1032
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2480]
	movw	r1, #1036
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2484]
	lsls	r1, r2, #31
	bpl	.L87
	mov	r1, #1280
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2488]
	movw	r1, #1284
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2492]
	mov	r1, #1288
	mov	r0, r4
	bl	mpp_write_relaxed
.L87:
	.syntax unified
@ 370 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_rkvenc_pp.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	movs	r2, #1
	movs	r1, #16
	mov	r0, r4
	bl	mpp_write
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L89
	mov	r2, #374
	ldr	r1, .L98+4
	ldr	r0, .L98+12
	bl	printk
.L89:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
.L81:
	ldr	r2, [r5, #2880]
	mov	r1, #512
	mov	r0, r4
	add	r7, r5, #2384
	bl	mpp_write_relaxed
	mov	r6, #516
.L84:
	mov	r1, r6
	ldr	r2, [r7, #4]!
	mov	r0, r4
	adds	r6, r6, #4
	bl	mpp_write_relaxed
	cmp	r6, #572
	bne	.L84
	b	.L85
.L82:
	addw	r7, r5, #2440
	mov	r6, #768
.L88:
	mov	r1, r6
	ldr	r2, [r7, #4]!
	mov	r0, r4
	adds	r6, r6, #4
	bl	mpp_write_relaxed
	cmp	r6, #796
	bne	.L88
	b	.L83
.L99:
	.align	2
.L98:
	.word	mpp_dev_debug
	.word	.LANCHOR0+423
	.word	.LC15
	.word	.LC16
	.fnend
	.size	vepu_pp_run, .-vepu_pp_run
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_alloc_task, %function
vepu_pp_alloc_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r7, r1
	ldr	r6, .L125
	mov	r5, r0
	ldr	r3, [r6]
	lsls	r1, r3, #26
	bpl	.L101
	movs	r2, #235
	ldr	r1, .L125+4
	ldr	r0, .L125+8
	bl	printk
.L101:
	ldr	r3, .L125+12
	mov	r1, #3520
	ldr	r0, [r3, #48]
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L100
	mov	r1, r0
	mov	r0, r5
	bl	mpp_task_init
	add	r5, r7, #60
	mov	fp, #0
	movw	r8, #513
	mov	r9, #20
	movw	r10, #514
.L103:
	ldr	r3, [r7, #48]
	cmp	fp, r3
	bcc	.L108
	ldr	r3, [r6]
	lsls	r2, r3, #22
	bpl	.L109
	ldr	r2, [r4, #3724]
	ldr	r1, [r4, #3400]
	ldr	r0, .L125+16
	bl	printk
.L109:
	movs	r3, #3
	str	r3, [r4, #2340]
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L100
	movs	r2, #249
	ldr	r1, .L125+4
	ldr	r0, .L125+20
	bl	printk
.L100:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L108:
	ldr	r2, [r5, #8]
	cbz	r2, .L104
	ldr	r3, [r5]
	cmp	r3, r8
	beq	.L105
	cmp	r3, r10
	beq	.L106
	cmp	r3, #512
	bne	.L104
	ldr	r1, [r5, #16]
	addw	r0, r4, #2344
	bl	memcpy
.L104:
	add	fp, fp, #1
	adds	r5, r5, #20
	b	.L103
.L105:
	ldr	r3, [r4, #3724]
	add	lr, r5, #16
	adds	r2, r3, #1
	str	r2, [r4, #3724]
	mla	r3, r9, r3, r4
	mov	r2, r5
	add	r3, r3, #3728
.L107:
	ldr	r0, [r2]	@ unaligned
	adds	r2, r2, #8
	ldr	r1, [r2, #-4]	@ unaligned
	cmp	r2, lr
	mov	ip, r3
	stmia	ip!, {r0, r1}
	mov	r3, ip
	bne	.L107
	ldr	r0, [r2]	@ unaligned
	str	r0, [ip]
	b	.L104
.L106:
	mov	r1, r5
	add	r0, r4, #2912
	bl	mpp_extract_reg_offset_info
	b	.L104
.L126:
	.align	2
.L125:
	.word	mpp_dev_debug
	.word	.LANCHOR0+435
	.word	.LC15
	.word	kmalloc_caches
	.word	.LC17
	.word	.LC16
	.fnend
	.size	vepu_pp_alloc_task, .-vepu_pp_alloc_task
	.section	.text.unlikely
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_shutdown, %function
vepu_pp_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	add	r5, r0, #16
	ldr	r4, [r0, #80]
	ldr	r1, .L138
	mov	r0, r5
	bl	_dev_info
	ldr	r3, [r4, #104]
	add	r2, r3, #84
	pld	[r2]
	.syntax unified
@ 223 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r1, [r2]
	add	r1, r1, #1
	strex	r0, r1, [r2]
	teq	r0, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	bl	ktime_get
	ldr	r3, .L138+4
	adds	r6, r0, r3
	mov	r3, #0
	adc	r7, r1, r3
.L133:
	ldr	r3, [r4, #80]
	cbz	r3, .L127
	bl	ktime_get
	cmp	r6, r0
	sbcs	r3, r7, r1
	bge	.L130
	ldr	r3, [r4, #80]
	cbnz	r3, .L137
.L127:
	pop	{r3, r4, r5, r6, r7, pc}
.L130:
	movw	r1, #20000
	movw	r0, #5001
	bl	usleep_range
	b	.L133
.L137:
	mov	r0, r5
	ldr	r1, .L138+8
	pop	{r3, r4, r5, r6, r7, lr}
	b	_dev_err
.L139:
	.align	2
.L138:
	.word	.LC18
	.word	200000000
	.word	.LC19
	.fnend
	.size	vepu_pp_shutdown, .-vepu_pp_shutdown
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_clk_on, %function
vepu_pp_clk_on:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r0, [r0, #152]
	cbz	r0, .L141
	bl	clk_prepare_enable
.L141:
	ldr	r0, [r4, #180]
	cbz	r0, .L142
	bl	clk_prepare_enable
.L142:
	ldr	r0, [r4, #208]
	cbz	r0, .L143
	bl	clk_prepare_enable
.L143:
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	vepu_pp_clk_on, .-vepu_pp_clk_on
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_clk_off, %function
vepu_pp_clk_off:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r5, [r0, #152]
	cbz	r5, .L154
	mov	r0, r5
	bl	clk_disable
	mov	r0, r5
	bl	clk_unprepare
.L154:
	ldr	r5, [r4, #180]
	cbz	r5, .L155
	mov	r0, r5
	bl	clk_disable
	mov	r0, r5
	bl	clk_unprepare
.L155:
	ldr	r4, [r4, #208]
	cbz	r4, .L156
	mov	r0, r4
	bl	clk_disable
	mov	r0, r4
	bl	clk_unprepare
.L156:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
	.fnend
	.size	vepu_pp_clk_off, .-vepu_pp_clk_off
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_reset, %function
vepu_pp_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r0, [r0, #236]
	cbz	r0, .L168
	ldr	r3, [r4, #240]
	cbz	r3, .L168
	ldr	r3, [r4, #244]
	cbz	r3, .L168
	bl	reset_control_assert
	ldr	r0, [r4, #240]
	cbz	r0, .L170
	bl	reset_control_assert
.L170:
	ldr	r0, [r4, #244]
	cbz	r0, .L171
	bl	reset_control_assert
.L171:
	ldr	r3, .L195
	ldr	r0, .L195+4
	ldr	r3, [r3, #4]
	blx	r3
	ldr	r0, [r4, #236]
	cbz	r0, .L172
	bl	reset_control_deassert
.L172:
	ldr	r0, [r4, #240]
	cbz	r0, .L173
	bl	reset_control_deassert
.L173:
	ldr	r0, [r4, #244]
	cbz	r0, .L168
	bl	reset_control_deassert
.L168:
	movs	r0, #0
	pop	{r4, pc}
.L196:
	.align	2
.L195:
	.word	arm_delay_ops
	.word	1073740
	.fnend
	.size	vepu_pp_reset, .-vepu_pp_reset
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_isr, %function
vepu_pp_isr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, [r0, #88]
	cbnz	r5, .L198
	ldr	r1, .L211
	ldr	r0, [r0]
	bl	_dev_err
.L199:
	movs	r0, #1
	pop	{r4, r5, r6, pc}
.L198:
	ldr	r6, .L211+4
	mov	r0, r5
	bl	mpp_time_diff
	ldr	r1, [r4, #52]
	movs	r3, #0
	str	r3, [r4, #88]
	ldr	r2, [r6]
	str	r1, [r5, #3396]
	lsls	r2, r2, #29
	bpl	.L200
	ldr	r0, .L211+8
	bl	printk
.L200:
	ldr	r3, [r5, #3396]
	tst	r3, #28
	beq	.L201
	add	r3, r4, #72
	pld	[r3]
	.syntax unified
@ 223 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r2, [r3]
	add	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
.L201:
	mov	r1, r5
	ldr	r0, [r5]
	bl	mpp_task_finish
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L199
	mov	r2, #414
	ldr	r1, .L211+12
	ldr	r0, .L211+16
	bl	printk
	b	.L199
.L212:
	.align	2
.L211:
	.word	.LC20
	.word	mpp_dev_debug
	.word	.LC21
	.word	.LANCHOR0+454
	.word	.LC16
	.fnend
	.size	vepu_pp_isr, .-vepu_pp_isr
	.global	rockchip_vepu_pp_driver
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.39087, %object
	.size	__func__.39087, 13
__func__.39087:
	.ascii	"vepu_pp_init\000"
	.space	3
	.type	mpp_vepu_pp_match, %object
	.size	mpp_vepu_pp_match, 392
mpp_vepu_pp_match:
	.space	64
	.ascii	"rockchip,rkvenc-pp-rv1106\000"
	.space	102
	.word	vepu_pp_data
	.space	196
	.type	__func__.39041, %object
	.size	__func__.39041, 15
__func__.39041:
	.ascii	"vepu_pp_finish\000"
	.type	__func__.39001, %object
	.size	__func__.39001, 12
__func__.39001:
	.ascii	"vepu_pp_run\000"
	.type	__func__.38953, %object
	.size	__func__.38953, 19
__func__.38953:
	.ascii	"vepu_pp_alloc_task\000"
	.type	__func__.39026, %object
	.size	__func__.39026, 12
__func__.39026:
	.ascii	"vepu_pp_isr\000"
	.space	2
	.type	vepu_pp_data, %object
	.size	vepu_pp_data, 20
vepu_pp_data:
	.word	25
	.word	vepu_pp_hw_info
	.space	4
	.word	vepu_pp_hw_ops
	.word	vepu_pp_dev_ops
	.data
	.align	2
	.type	rockchip_vepu_pp_driver, %object
	.size	rockchip_vepu_pp_driver, 104
rockchip_vepu_pp_driver:
	.word	vepu_pp_probe
	.word	vepu_pp_remove
	.word	vepu_pp_shutdown
	.space	8
	.word	.LC22
	.space	20
	.word	mpp_vepu_pp_match
	.space	48
	.space	8
	.type	vepu_pp_hw_info, %object
	.size	vepu_pp_hw_info, 24
vepu_pp_hw_info:
	.space	4
	.word	-1
	.space	16
	.type	vepu_pp_dev_ops, %object
	.size	vepu_pp_dev_ops, 68
vepu_pp_dev_ops:
	.space	16
	.word	vepu_pp_alloc_task
	.space	4
	.word	vepu_pp_run
	.word	vepu_pp_irq
	.word	vepu_pp_isr
	.word	vepu_pp_finish
	.word	vepu_pp_result
	.word	vepu_pp_free_task
	.space	20
	.type	vepu_pp_hw_ops, %object
	.size	vepu_pp_hw_ops, 36
vepu_pp_hw_ops:
	.word	vepu_pp_init
	.space	4
	.word	vepu_pp_clk_on
	.word	vepu_pp_clk_off
	.space	4
	.word	vepu_pp_set_freq
	.space	4
	.word	vepu_pp_reset
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016read reg[%03d]: %04x: 0x%08x\012\000"
.LC1:
	.ascii	"\0016write reg[%03d]: %04x: 0x%08x\012\000"
.LC2:
	.ascii	"aclk_vepu_pp\000"
.LC3:
	.ascii	"\0013%s:%d: failed on clk_get aclk\012\000"
.LC4:
	.ascii	"hclk_vepu_pp\000"
.LC5:
	.ascii	"\0013%s:%d: failed on clk_get hclk\012\000"
.LC6:
	.ascii	"rst_a_pp\000"
.LC7:
	.ascii	"\0013%s:%d: No aclk reset resource define\012\000"
.LC8:
	.ascii	"rst_h_pp\000"
.LC9:
	.ascii	"\0013%s:%d: No hclk reset resource define\012\000"
.LC10:
	.ascii	"remove device\012\000"
.LC11:
	.ascii	"probe device\012\000"
.LC12:
	.ascii	"probe sub driver failed\012\000"
.LC13:
	.ascii	"register interrupter runtime failed\012\000"
.LC14:
	.ascii	"probing finish\012\000"
.LC15:
	.ascii	"\0016%s:%d: enter\012\000"
.LC16:
	.ascii	"\0016%s:%d: leave\012\000"
.LC17:
	.ascii	"\0016w_req_cnt %d, r_req_cnt %d\012\000"
.LC18:
	.ascii	"shutdown device\012\000"
.LC19:
	.ascii	"wait total running time out\012\000"
.LC20:
	.ascii	"no current task\012\000"
.LC21:
	.ascii	"\0016irq_status: %08x\012\000"
.LC22:
	.ascii	"mpp_vepu_pp\000"
	.section	___ksymtab+rockchip_vepu_pp_driver,"a"
	.align	2
	.type	__ksymtab_rockchip_vepu_pp_driver, %object
	.size	__ksymtab_rockchip_vepu_pp_driver, 12
__ksymtab_rockchip_vepu_pp_driver:
	.word	rockchip_vepu_pp_driver
	.word	__kstrtab_rockchip_vepu_pp_driver
	.word	__kstrtabns_rockchip_vepu_pp_driver
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
