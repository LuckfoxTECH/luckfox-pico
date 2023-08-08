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
	add	r4, r1, #3712
	add	r7, r1, #2864
	mov	r6, r1
	movs	r5, #0
.L3:
	ldr	r3, [r6, #3700]
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
@ 113 "./arch/arm/include/asm/io.h" 1
	ldr r4, [r3]
@ 0 "" 2
@ 717 "drivers/kmpp/mpp/mpp_common.h" 1
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
@ 95 "./arch/arm/include/asm/io.h" 1
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
@ 696 "drivers/kmpp/mpp/mpp_common.h" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r1, [r6, #60]
	add	r1, r1, r4
	.syntax unified
@ 95 "./arch/arm/include/asm/io.h" 1
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
	adds	r0, r0, #132
	ldr	r1, [r1, #2316]
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
	add	r5, r0, #132
	ldr	r3, [r0, #104]
	mov	r4, r0
	ldr	r2, .L46
	mov	r1, r5
	adds	r3, r3, #196
	str	r3, [r0, #64]
	bl	mpp_get_clk_info
	cbz	r0, .L36
	movw	r2, #519
	ldr	r1, .L46+4
	ldr	r0, .L46+8
	bl	printk
.L36:
	ldr	r2, .L46+12
	add	r1, r4, #160
	mov	r0, r4
	bl	mpp_get_clk_info
	cbz	r0, .L37
	movw	r2, #522
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
	str	r0, [r4, #216]
	cbnz	r0, .L38
	mov	r2, #528
	ldr	r1, .L46+4
	ldr	r0, .L46+28
	bl	printk
.L38:
	ldr	r2, .L46+32
	movs	r1, #1
	mov	r0, r4
	bl	mpp_reset_control_get
	str	r0, [r4, #220]
	cbnz	r0, .L39
	movw	r2, #531
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
	movs	r1, #228
	mov	r0, r5
	bl	devm_kmalloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L59
	ldr	r1, [r6, #372]
	str	r0, [r6, #80]
	cbz	r1, .L54
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
	mov	r2, #430
	ldr	r1, .L77+4
	ldr	r0, .L77+8
	bl	printk
.L67:
	addw	r6, r6, #2860
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
	movw	r2, #435
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
	ldr	r3, .L102
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r2, [r3]
	mov	r5, r1
	mov	r8, r3
	tst	r2, #32
	beq	.L80
	movw	r2, #357
	ldr	r1, .L102+4
	ldr	r0, .L102+8
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
	ldr	r2, [r5, #2844]
	mov	r1, #268
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2848]
	mov	r1, #272
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2852]
	mov	r1, #276
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2816]
	movs	r1, #60
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2820]
	movs	r1, #64
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2824]
	movs	r1, #68
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2828]
	movs	r1, #72
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2832]
	movs	r1, #76
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2836]
	movs	r1, #80
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2840]
	movs	r1, #84
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2320]
	movs	r1, #52
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2324]
	movs	r1, #56
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2328]
	mov	r1, #256
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2332]
	mov	r1, #260
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2336]
	mov	r1, #264
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2340]
	mov	r1, #280
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2344]
	mov	r1, #284
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r3, [r5, #2364]
	lsls	r7, r3, #31
	bmi	.L81
.L85:
	ldr	r3, [r5, #2420]
	lsls	r6, r3, #31
	bmi	.L82
.L83:
	ldr	r3, [r5, #2448]
	lsls	r0, r3, #31
	bpl	.L87
	ldr	r2, [r5, #2860]
	mov	r1, #1024
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2448]
	movw	r1, #1028
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2452]
	mov	r1, #1032
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2456]
	movw	r1, #1036
	mov	r0, r4
	bl	mpp_write_relaxed
.L87:
	ldr	r2, [r5, #2460]
	lsls	r1, r2, #31
	bpl	.L89
	mov	r1, #1280
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2464]
	movw	r1, #1284
	mov	r0, r4
	bl	mpp_write_relaxed
	ldr	r2, [r5, #2468]
	mov	r1, #1288
	mov	r0, r4
	bl	mpp_write_relaxed
.L89:
	.syntax unified
@ 372 "drivers/kmpp/mpp/mpp_rkvenc_pp.c" 1
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
	bpl	.L90
	mov	r2, #376
	ldr	r1, .L102+4
	ldr	r0, .L102+12
	bl	printk
.L90:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
.L81:
	ldr	r2, [r5, #2856]
	mov	r1, #512
	mov	r0, r4
	addw	r7, r5, #2360
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
	add	r7, r5, #2416
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
.L103:
	.align	2
.L102:
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
	ldr	r6, .L129
	mov	r5, r0
	ldr	r3, [r6]
	lsls	r1, r3, #26
	bpl	.L105
	movs	r2, #235
	ldr	r1, .L129+4
	ldr	r0, .L129+8
	bl	printk
.L105:
	ldr	r3, .L129+12
	mov	r1, #3520
	ldr	r0, [r3, #48]
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L104
	mov	r1, r0
	mov	r0, r5
	bl	mpp_task_init
	add	r5, r7, #60
	mov	fp, #0
	movw	r8, #513
	mov	r9, #20
	movw	r10, #514
.L107:
	ldr	r3, [r7, #48]
	cmp	fp, r3
	bcc	.L112
	ldr	r3, [r6]
	lsls	r2, r3, #22
	bpl	.L113
	ldr	r2, [r4, #3700]
	ldr	r1, [r4, #3376]
	ldr	r0, .L129+16
	bl	printk
.L113:
	movs	r3, #3
	str	r3, [r4, #2316]
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L104
	movs	r2, #249
	ldr	r1, .L129+4
	ldr	r0, .L129+20
	bl	printk
.L104:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L112:
	ldr	r2, [r5, #8]
	cbz	r2, .L108
	ldr	r3, [r5]
	cmp	r3, r8
	beq	.L109
	cmp	r3, r10
	beq	.L110
	cmp	r3, #512
	bne	.L108
	ldr	r1, [r5, #16]
	add	r0, r4, #2320
	bl	memcpy
.L108:
	add	fp, fp, #1
	adds	r5, r5, #20
	b	.L107
.L109:
	ldr	r3, [r4, #3700]
	add	lr, r5, #16
	adds	r2, r3, #1
	str	r2, [r4, #3700]
	mla	r3, r9, r3, r4
	mov	r2, r5
	addw	r3, r3, #3704
.L111:
	ldr	r0, [r2]	@ unaligned
	adds	r2, r2, #8
	ldr	r1, [r2, #-4]	@ unaligned
	cmp	r2, lr
	mov	ip, r3
	stmia	ip!, {r0, r1}
	mov	r3, ip
	bne	.L111
	ldr	r0, [r2]	@ unaligned
	str	r0, [ip]
	b	.L108
.L110:
	mov	r1, r5
	addw	r0, r4, #2888
	bl	mpp_extract_reg_offset_info
	b	.L108
.L130:
	.align	2
.L129:
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
	ldr	r1, .L142
	mov	r0, r5
	bl	_dev_info
	ldr	r3, [r4, #104]
	add	r2, r3, #84
	pld	[r2]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
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
	ldr	r3, .L142+4
	adds	r6, r0, r3
	mov	r3, #0
	adc	r7, r1, r3
	bl	_cond_resched
.L137:
	ldr	r3, [r4, #80]
	cbz	r3, .L131
	bl	ktime_get
	cmp	r6, r0
	sbcs	r3, r7, r1
	bge	.L134
	ldr	r3, [r4, #80]
	cbnz	r3, .L141
.L131:
	pop	{r3, r4, r5, r6, r7, pc}
.L134:
	movw	r1, #20000
	movw	r0, #5001
	bl	usleep_range
	b	.L137
.L141:
	mov	r0, r5
	ldr	r1, .L142+8
	pop	{r3, r4, r5, r6, r7, lr}
	b	_dev_err
.L143:
	.align	2
.L142:
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
	ldr	r0, [r0, #132]
	cbz	r0, .L145
	bl	clk_prepare_enable
.L145:
	ldr	r0, [r4, #160]
	cbz	r0, .L146
	bl	clk_prepare_enable
.L146:
	ldr	r0, [r4, #188]
	cbz	r0, .L147
	bl	clk_prepare_enable
.L147:
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
	ldr	r5, [r0, #132]
	cbz	r5, .L158
	mov	r0, r5
	bl	clk_disable
	mov	r0, r5
	bl	clk_unprepare
.L158:
	ldr	r5, [r4, #160]
	cbz	r5, .L159
	mov	r0, r5
	bl	clk_disable
	mov	r0, r5
	bl	clk_unprepare
.L159:
	ldr	r4, [r4, #188]
	cbz	r4, .L160
	mov	r0, r4
	bl	clk_disable
	mov	r0, r4
	bl	clk_unprepare
.L160:
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
	ldr	r0, [r0, #216]
	cbz	r0, .L172
	ldr	r3, [r4, #220]
	cbz	r3, .L172
	ldr	r3, [r4, #224]
	cbz	r3, .L172
	bl	reset_control_assert
	ldr	r0, [r4, #220]
	cbz	r0, .L174
	bl	reset_control_assert
.L174:
	ldr	r0, [r4, #224]
	cbz	r0, .L175
	bl	reset_control_assert
.L175:
	ldr	r3, .L199
	ldr	r0, .L199+4
	ldr	r3, [r3, #4]
	blx	r3
	ldr	r0, [r4, #216]
	cbz	r0, .L176
	bl	reset_control_deassert
.L176:
	ldr	r0, [r4, #220]
	cbz	r0, .L177
	bl	reset_control_deassert
.L177:
	ldr	r0, [r4, #224]
	cbz	r0, .L172
	bl	reset_control_deassert
.L172:
	movs	r0, #0
	pop	{r4, pc}
.L200:
	.align	2
.L199:
	.word	arm_delay_ops
	.word	3221225
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
	cbnz	r5, .L202
	ldr	r1, .L215
	ldr	r0, [r0]
	bl	_dev_err
.L203:
	movs	r0, #1
	pop	{r4, r5, r6, pc}
.L202:
	ldr	r6, .L215+4
	mov	r0, r5
	bl	mpp_time_diff
	ldr	r1, [r4, #52]
	movs	r3, #0
	str	r3, [r4, #88]
	ldr	r2, [r6]
	str	r1, [r5, #3372]
	lsls	r2, r2, #29
	bpl	.L204
	ldr	r0, .L215+8
	bl	printk
.L204:
	ldr	r3, [r5, #3372]
	tst	r3, #28
	beq	.L205
	add	r3, r4, #72
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
.L205:
	mov	r1, r5
	ldr	r0, [r5]
	bl	mpp_task_finish
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L203
	mov	r2, #416
	ldr	r1, .L215+12
	ldr	r0, .L215+16
	bl	printk
	b	.L203
.L216:
	.align	2
.L215:
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
	.type	__func__.38986, %object
	.size	__func__.38986, 13
__func__.38986:
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
	.type	__func__.38940, %object
	.size	__func__.38940, 15
__func__.38940:
	.ascii	"vepu_pp_finish\000"
	.type	__func__.38900, %object
	.size	__func__.38900, 12
__func__.38900:
	.ascii	"vepu_pp_run\000"
	.type	__func__.38852, %object
	.size	__func__.38852, 19
__func__.38852:
	.ascii	"vepu_pp_alloc_task\000"
	.type	__func__.38925, %object
	.size	__func__.38925, 12
__func__.38925:
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
