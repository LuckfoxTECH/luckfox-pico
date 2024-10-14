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
	.file	"mpp_rkvenc_540c.c"
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
	.type	____atomic_set_bit, %function
____atomic_set_bit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r3, #1
	ldr	r4, [r1]
	lsls	r3, r3, r0
	orrs	r3, r3, r4
	str	r3, [r1]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	pop	{r4, pc}
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
	.type	rkvenc_invalid_class_msg, %function
rkvenc_invalid_class_msg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #2340]
	movs	r2, #12
	ldr	r3, [r3, #24]
	mla	r3, r2, r3, r0
	movs	r2, #0
.L4:
	cmp	r0, r3
	bne	.L6
	movs	r0, #0
	bx	lr
.L6:
	ldr	r1, [r0, #2348]
	cbz	r1, .L5
	str	r2, [r0, #2348]
	str	r2, [r0, #2344]
.L5:
	adds	r0, r0, #12
	b	.L4
	.fnend
	.size	rkvenc_invalid_class_msg, .-rkvenc_invalid_class_msg
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_get_class_reg, %function
rkvenc_get_class_reg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movs	r3, #0
	ldr	r2, [r0, #2340]
	ldr	r5, [r2, #24]
.L11:
	cmp	r3, r5
	bne	.L14
	movs	r0, #0
	b	.L10
.L14:
	ldr	r4, [r2, #28]
	cmp	r4, r1
	bhi	.L12
	ldr	r6, [r2, #32]
	cmp	r6, r1
	bls	.L12
	movs	r2, #12
	subs	r1, r1, r4
	mla	r3, r2, r3, r0
	ldr	r0, [r3, #2348]
	add	r0, r0, r1
.L10:
	pop	{r4, r5, r6, pc}
.L12:
	adds	r3, r3, #1
	adds	r2, r2, #20
	b	.L11
	.fnend
	.size	rkvenc_get_class_reg, .-rkvenc_get_class_reg
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
	ldr	r3, .L20
	ldr	r3, [r3]
	lsls	r3, r3, #18
	bpl	.L15
	mov	r3, r4
	lsrs	r1, r1, #2
	ldr	r0, .L20+4
	bl	printk
.L15:
	mov	r0, r4
	pop	{r4, pc}
.L21:
	.align	2
.L20:
	.word	mpp_dev_debug
	.word	.LC0
	.fnend
	.size	mpp_read, .-mpp_read
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_read_relaxed, %function
mpp_read_relaxed:
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
	.thumb
	.syntax unified
	ldr	r3, .L27
	ldr	r3, [r3]
	lsls	r3, r3, #18
	bpl	.L22
	mov	r3, r4
	lsrs	r1, r1, #2
	ldr	r0, .L27+4
	bl	printk
.L22:
	mov	r0, r4
	pop	{r4, pc}
.L28:
	.align	2
.L27:
	.word	mpp_dev_debug
	.word	.LC1
	.fnend
	.size	mpp_read_relaxed, .-mpp_read_relaxed
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
	ldr	r3, .L34
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r3, [r3]
	mov	r4, r1
	mov	r5, r2
	lsls	r3, r3, #19
	bpl	.L30
	mov	r3, r2
	ldr	r0, .L34+4
	mov	r2, r1
	lsrs	r1, r1, #2
	bl	printk
.L30:
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
.L35:
	.align	2
.L34:
	.word	mpp_dev_debug
	.word	.LC2
	.fnend
	.size	mpp_write_relaxed, .-mpp_write_relaxed
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_update_req, %function
rkvenc_update_req:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	ldr	r4, [r0, #2340]
	movs	r0, #20
	mla	r1, r0, r1, r4
	ldrd	r5, r6, [r1, #28]
	ldrd	r0, r1, [r2, #8]
	add	r0, r0, r1
	cmp	r1, r5
	sub	r0, r0, #4
	mov	r4, r6
	it	cc
	movcc	r1, r5
	cmp	r6, r0
	str	r1, [r3, #12]
	it	cs
	movcs	r4, r0
	adds	r4, r4, #4
	subs	r4, r4, r1
	str	r4, [r3, #8]
	ldr	r4, [r2, #12]
	subs	r1, r1, r4
	ldr	r4, [r2, #16]
	add	r1, r1, r4
	str	r1, [r3, #16]
	ldr	r3, [r2, #12]
	cmp	r3, r5
	bcc	.L37
	cmp	r6, r0
	bcs	.L38
.L37:
	ldr	r2, [r2, #8]
	ldr	r1, .L39
	ldr	r0, .L39+4
	str	r2, [sp]
	movw	r2, #530
	bl	printk
.L38:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L40:
	.align	2
.L39:
	.word	.LANCHOR0
	.word	.LC3
	.fnend
	.size	rkvenc_update_req, .-rkvenc_update_req
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
	ldr	r3, .L46
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r3, [r3]
	mov	r4, r1
	mov	r5, r2
	lsls	r3, r3, #19
	bpl	.L42
	mov	r3, r2
	ldr	r0, .L46+4
	mov	r2, r1
	lsrs	r1, r1, #2
	bl	printk
.L42:
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
.L47:
	.align	2
.L46:
	.word	mpp_dev_debug
	.word	.LC2
	.fnend
	.size	mpp_write, .-mpp_write
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_check_bs_overflow, %function
rkvenc_check_bs_overflow:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r0
	ldr	r3, [r0, #52]
	lsls	r5, r3, #31
	bmi	.L57
	ldr	r7, [r0, #88]
	ands	r3, r3, #8192
	ldr	r2, [r7]
	ldr	r5, [r2, #184]
	beq	.L58
	movw	r1, #1036
	bl	mpp_read
	mov	r1, #1024
	mov	r6, r0
	mov	r0, r4
	bl	mpp_read
	movw	r1, #1028
	mov	r8, r0
	mov	r0, r4
	bl	mpp_read
	strd	r8, r0, [r5, #216]
	mov	r3, r0
	ldr	r2, [r4, #308]
	strd	r2, r6, [r5, #224]
	ldr	r2, .L68
	ldr	r2, [r2]
	lsls	r0, r2, #13
	bpl	.L51
	ldr	r2, [r4, #308]
	str	r6, [sp, #4]
	ldr	r0, .L68+4
	str	r2, [sp]
	mov	r2, r8
	ldr	r1, [r7, #2276]
	bl	printk
.L51:
	ldr	r3, [r4, #308]
	cmp	r3, r6
	itt	eq
	addeq	r3, r6, #1
	streq	r3, [r4, #308]
	ldr	r3, [r4, #308]
	cmp	r3, r8
	bcc	.L53
	movw	r1, #1028
	mov	r0, r4
	bl	mpp_read
	str	r0, [r4, #308]
.L53:
	ldr	r2, [r4, #308]
	movw	r1, #1036
	mov	r0, r4
	bl	mpp_write
	add	r2, r6, #12
	mov	r1, #1032
	mov	r0, r4
	bl	mpp_write
	ldr	r3, [r4, #52]
	movs	r0, #1
	str	r3, [r4, #56]
.L50:
	ldr	r3, [r4, #52]
	lsls	r1, r3, #27
	bpl	.L48
	mov	r1, #700
	mov	r0, r4
	bl	mpp_read
	mov	r1, #688
	mov	r6, r0
	mov	r0, r4
	bl	mpp_read
	mov	r1, #692
	mov	r8, r0
	mov	r0, r4
	bl	mpp_read
	strd	r8, r0, [r5, #216]
	mov	r3, r0
	ldr	r2, [r4, #304]
	strd	r2, r6, [r5, #224]
	ldr	r2, .L68
	ldr	r2, [r2]
	lsls	r2, r2, #13
	bpl	.L54
	ldr	r2, [r4, #304]
	str	r6, [sp, #4]
	ldr	r0, .L68+8
	str	r2, [sp]
	mov	r2, r8
	ldr	r1, [r7, #2276]
	bl	printk
.L54:
	ldr	r3, [r4, #304]
	cmp	r3, r6
	itt	eq
	addeq	r3, r6, #1
	streq	r3, [r4, #304]
	ldr	r3, [r4, #304]
	cmp	r3, r8
	bcc	.L56
	mov	r1, #692
	mov	r0, r4
	bl	mpp_read
	str	r0, [r4, #304]
.L56:
	ldr	r2, [r4, #304]
	mov	r1, #696
	mov	r0, r4
	bl	mpp_write
	add	r2, r6, #12
	mov	r1, #700
	mov	r0, r4
	bl	mpp_write
	ldr	r3, [r4, #52]
	movs	r0, #1
	str	r3, [r4, #56]
.L48:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L58:
	mov	r0, r3
	b	.L50
.L57:
	movs	r0, #0
	b	.L48
.L69:
	.align	2
.L68:
	.word	mpp_dev_debug
	.word	.LC4
	.word	.LC5
	.fnend
	.size	rkvenc_check_bs_overflow, .-rkvenc_check_bs_overflow
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_clear_dvbm_info, %function
rkvenc_clear_dvbm_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	movs	r2, #0
	mov	r4, r0
	movs	r1, #96
	bl	mpp_write
	movs	r2, #0
	movs	r1, #24
	mov	r0, r4
	bl	mpp_write
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	movs	r1, #96
	mov	r5, r0
	mov	r0, r4
	bl	mpp_read
	orrs	r3, r5, r0
	mov	r2, r0
	beq	.L70
	mov	r1, r5
	ldr	r0, .L72
	pop	{r3, r4, r5, lr}
	b	printk
.L70:
	pop	{r3, r4, r5, pc}
.L73:
	.align	2
.L72:
	.word	.LC6
	.fnend
	.size	rkvenc_clear_dvbm_info, .-rkvenc_clear_dvbm_info
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_reset, %function
rkvenc_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r6, .L83
	ldr	r5, [r0, #152]
	ldr	r2, [r6]
	lsls	r2, r2, #26
	bpl	.L75
	movw	r2, #2247
	ldr	r1, .L83+4
	ldr	r0, .L83+8
	bl	printk
.L75:
	movw	r2, #1023
	ldr	r1, [r5, #256]
	mov	r0, r4
	bl	mpp_write
	movs	r2, #1
	ldr	r1, [r5, #248]
	mov	r0, r4
	bl	mpp_write
	ldr	r3, .L83+12
	ldr	r0, .L83+16
	ldr	r3, [r3, #4]
	blx	r3
	movs	r2, #3
	ldr	r1, [r5, #248]
	mov	r0, r4
	bl	mpp_write
	mov	r2, #-1
	ldr	r1, [r5, #260]
	mov	r0, r4
	bl	mpp_write
	movs	r2, #0
	ldr	r1, [r5, #264]
	mov	r0, r4
	bl	mpp_write
	movs	r2, #0
	ldr	r1, [r5, #248]
	mov	r0, r4
	bl	mpp_write
	mov	r0, r4
	bl	rkvenc_clear_dvbm_info
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L76
	movw	r2, #2259
	ldr	r1, .L83+4
	ldr	r0, .L83+20
	bl	printk
.L76:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L84:
	.align	2
.L83:
	.word	mpp_dev_debug
	.word	.LANCHOR0+18
	.word	.LC7
	.word	arm_delay_ops
	.word	1073740
	.word	.LC8
	.fnend
	.size	rkvenc_reset, .-rkvenc_reset
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_irq, %function
rkvenc_irq:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L95
	ldr	r6, [r0, #152]
	ldr	r2, [r5]
	lsls	r2, r2, #26
	bpl	.L86
	movw	r2, #1339
	ldr	r1, .L95+4
	ldr	r0, .L95+8
	bl	printk
.L86:
	ldr	r1, [r6, #264]
	mov	r0, r4
	bl	mpp_read
	str	r0, [r4, #52]
	cbz	r0, .L85
	mov	r2, #256
	ldr	r1, [r6, #256]
	mov	r0, r4
	bl	mpp_write
	ldr	r2, [r4, #52]
	ldr	r1, [r6, #260]
	mov	r0, r4
	bl	mpp_write
	movs	r2, #0
	ldr	r1, [r6, #264]
	mov	r0, r4
	bl	mpp_write
	ldr	r3, [r4, #52]
	cmp	r3, #32768
	bne	.L88
.L89:
	movs	r0, #1
.L85:
	pop	{r4, r5, r6, pc}
.L88:
	mov	r0, r4
	bl	rkvenc_check_bs_overflow
	cmp	r0, #0
	bne	.L89
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	ubfx	r0, r0, #0, #14
	str	r0, [r4, #272]
	mov	r0, r4
	bl	rkvenc_clear_dvbm_info
	ldr	r3, [r5]
	lsls	r3, r3, #26
	bpl	.L91
	movw	r2, #1355
	ldr	r1, .L95+4
	ldr	r0, .L95+12
	bl	printk
.L91:
	movs	r0, #2
	b	.L85
.L96:
	.align	2
.L95:
	.word	mpp_dev_debug
	.word	.LANCHOR0+31
	.word	.LC7
	.word	.LC8
	.fnend
	.size	rkvenc_irq, .-rkvenc_irq
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_run_start_link, %function
rkvenc_run_start_link:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r6, r0
	ldr	r5, [r0, #152]
	mov	r8, r1
	ldr	r9, [r1, #2348]
	ldr	r4, [r5, #28]
	ldr	r3, [r5, #32]
	lsrs	r4, r4, #2
	lsrs	r7, r3, #2
.L98:
	cmp	r4, r7
	bls	.L100
	ldr	r1, [r5, #252]
	movw	r2, #65535
	mov	r0, r6
	bl	mpp_write
	ldr	r3, [r8, #3592]
	ldr	r1, [r5, #288]
	mov	r0, r6
	ldr	r2, [r3, #32]
	bl	mpp_write
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L100:
	ldr	r3, [r5, #20]
	cmp	r3, r4
	beq	.L99
	ldr	r2, [r9, r4, lsl #2]
	lsls	r1, r4, #2
	mov	r0, r6
	bl	mpp_write
.L99:
	adds	r4, r4, #1
	b	.L98
	.fnend
	.size	rkvenc_run_start_link, .-rkvenc_run_start_link
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_dump_session, %function
rkvenc_dump_session:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r4, r1
	ldr	r5, [r0, #184]
	.pad #24
	sub	sp, sp, #24
	mov	r7, r0
	ldr	r9, .L122+96
	ldr	r10, .L122+12
	add	r8, r5, #24
	mov	r0, r5
	movs	r6, #0
	bl	down_read
	ldr	r1, .L122
	mov	r0, r4
	bl	seq_puts
	ldr	r1, .L122+4
	mov	r0, r4
	bl	seq_puts
	ldr	r2, .L122+8
	ldr	r1, .L122+12
	mov	r0, r4
	bl	seq_printf
	ldr	r2, .L122+16
	ldr	r1, .L122+12
	mov	r0, r4
	bl	seq_printf
.L103:
	lsls	r3, r6, #4
	ldr	r3, [r8, r3]
	cbz	r3, .L102
	ldr	r2, [r9, r6, lsl #2]
	mov	r1, r10
	mov	r0, r4
	bl	seq_printf
.L102:
	adds	r6, r6, #1
	cmp	r6, #11
	bne	.L103
	ldr	r3, .L122+20
	mov	r0, r4
	ldr	r2, .L122+24
	add	r6, r5, #32
	ldr	r1, .L122+28
	add	r8, r5, #208
	str	r3, [sp, #16]
	ldr	r3, .L122+32
	ldr	r9, .L122+100
	ldr	r10, .L122+56
	str	r3, [sp, #12]
	ldr	r3, .L122+36
	str	r3, [sp, #8]
	ldr	r3, .L122+40
	str	r3, [sp, #4]
	ldr	r3, .L122+44
	str	r3, [sp]
	ldr	r3, .L122+48
	bl	seq_printf
	ldr	r1, .L122+52
	mov	r0, r4
	bl	seq_puts
	ldr	r2, [r7, #196]
	ldr	r1, .L122+56
	mov	r0, r4
	bl	seq_printf
	ldr	r2, [r7]
	ldr	r3, .L122+60
	mov	r0, r4
	ldr	r1, .L122+12
	ldr	r7, .L122+12
	ldr	r2, [r3, r2, lsl #2]
	bl	seq_printf
.L107:
	ldr	r3, [r6, #-8]
	cbz	r3, .L104
	cmp	r3, #1
	bne	.L105
	ldr	r2, [r6]
	mov	r1, r10
.L120:
	mov	r0, r4
	bl	seq_printf
.L104:
	adds	r6, r6, #16
	cmp	r6, r8
	bne	.L107
	ldr	r2, [r5, #236]
	mov	r0, r4
	ldr	r3, [r5, #16]
	ldr	r1, .L122+64
	str	r2, [sp, #16]
	adds	r3, r3, #0
	ldr	r2, [r5, #232]
	it	ne
	movne	r3, #1
	str	r2, [sp, #12]
	ldr	r2, [r5, #212]
	str	r2, [sp, #8]
	ldr	r2, [r5, #208]
	str	r2, [sp, #4]
	ldr	r2, [r5, #204]
	str	r2, [sp]
	ldr	r2, [r5, #200]
	bl	seq_printf
	ldr	r1, .L122+52
	mov	r0, r4
	bl	seq_puts
	ldr	r3, [r5, #208]
	cbz	r3, .L108
	ldr	r1, .L122+68
	mov	r0, r4
	bl	seq_puts
	ldr	r1, .L122+4
	mov	r0, r4
	bl	seq_puts
	ldr	r3, .L122+72
	ldr	r2, .L122+76
	mov	r0, r4
	ldr	r1, .L122+80
	str	r3, [sp, #4]
	ldr	r3, .L122+84
	str	r3, [sp]
	ldr	r3, .L122+88
	bl	seq_printf
	ldr	r3, [r5, #228]
	ldr	r1, .L122+92
	mov	r0, r4
	str	r3, [sp, #4]
	ldr	r3, [r5, #224]
	str	r3, [sp]
	ldrd	r2, r3, [r5, #216]
	bl	seq_printf
.L108:
	mov	r0, r5
	bl	up_read
	movs	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L105:
	cmp	r3, #2
	mov	r1, r7
	ite	eq
	moveq	r2, r6
	movne	r2, r9
	b	.L120
.L123:
	.align	2
.L122:
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC17
	.word	.LC15
	.word	.LC16
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.word	.LC14
	.word	.LC22
	.word	.LC23
	.word	mpp_device_name
	.word	.LC25
	.word	.LC26
	.word	.LC30
	.word	.LC28
	.word	.LC29
	.word	.LC31
	.word	.LC27
	.word	.LC32
	.word	enc_info_item_name
	.word	.LC24
	.fnend
	.size	rkvenc_dump_session, .-rkvenc_dump_session
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_free_session, %function
rkvenc_free_session:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	cbz	r0, .L141
	ldr	r5, [r0, #188]
	cbz	r5, .L127
	mov	r0, r5
	bl	rkvenc_invalid_class_msg
	mov	r0, r5
	bl	kfree
.L127:
	ldr	r5, [r4, #192]
	cbz	r5, .L128
	mov	r0, r5
	bl	rkvenc_invalid_class_msg
	mov	r0, r5
	bl	kfree
.L128:
	ldr	r3, [r4, #184]
	ldr	r3, [r3, #16]
	cbz	r3, .L129
	ldr	r3, [r4, #12]
	movs	r2, #0
	str	r2, [r3, #128]
.L129:
	ldr	r0, [r4, #184]
	cbz	r0, .L141
	bl	kfree
	movs	r3, #0
	str	r3, [r4, #184]
.L141:
	movs	r0, #0
	pop	{r3, r4, r5, pc}
	.fnend
	.size	rkvenc_free_session, .-rkvenc_free_session
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_free_task, %function
rkvenc_free_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r1
	bl	mpp_task_finalize
	mov	r0, r4
	bl	rkvenc_invalid_class_msg
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	rkvenc_free_task, .-rkvenc_free_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_result, %function
rkvenc_result:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r7, r0
	ldr	r6, .L175
	mov	r4, r1
	ldr	r3, [r6]
	mov	r10, r6
	tst	r3, #32
	beq	.L144
	movw	r2, #1754
	ldr	r1, .L175+4
	ldr	r0, .L175+8
	bl	printk
.L144:
	ldr	r3, [r7, #292]
	cmp	r3, #1
	beq	.L145
	cbz	r3, .L146
	cmp	r3, #2
	bne	.L147
.L146:
	ldr	r3, [r4, #2200]
	lsls	r2, r3, #25
	bmi	.L162
	ldr	r3, [r4, #2200]
	lsls	r3, r3, #22
	bpl	.L147
.L162:
	mov	r0, #-1
	b	.L143
.L145:
	ldr	r8, .L175+20
	mov	r5, r4
	mov	r9, #0
.L148:
	ldr	r3, [r4, #3268]
	cmp	r3, r9
	bhi	.L159
.L147:
	ldr	r0, [r10]
	ands	r0, r0, #32
	beq	.L143
	movw	r2, #1786
	ldr	r1, .L175+4
	ldr	r0, .L175+12
	bl	printk
	movs	r0, #0
	b	.L143
.L159:
	ldr	r3, [r4, #2340]
	movs	r2, #0
	ldr	r1, [r7, #288]
	ldr	ip, [r5, #3284]
	ldr	r0, [r3, #24]
.L150:
	cmp	r0, r2
	beq	.L163
	ldr	lr, [r3, #28]
	cmp	ip, lr
	bcc	.L151
	ldr	r6, [r3, #32]
	cmp	ip, r6
	bcs	.L151
	add	r2, r1, r2, lsl #2
	ldr	r3, [r4, #3592]
	ldr	r1, [r2, #12]
	ldr	r3, [r3, #40]
	sub	r1, r1, lr
	add	r1, r1, ip
	adds	r1, r3, r1
	bne	.L174
.L163:
	mvn	r0, #21
	b	.L143
.L151:
	adds	r2, r2, #1
	adds	r3, r3, #20
	b	.L150
.L174:
	ldr	r3, [r4]
	ldr	r0, [r5, #3288]
	ldr	r2, [r5, #3280]
	ldr	r3, [r3, #200]
	cbnz	r3, .L154
	cmp	r2, #0
	bge	.L155
.L157:
	movw	r2, #1775
	ldr	r1, .L175+4
	ldr	r0, .L175+16
	bl	printk
	mvn	r0, #4
.L143:
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L155:
	mov	r3, sp
	and	r3, r3, r8
	ldr	r3, [r3, #8]
	.syntax unified
@ 177 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/include/linux/uaccess.h" 1
	.syntax unified
adds ip, r0, r2; sbcscc ip, ip, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L156
	bl	arm_copy_to_user
	mov	r2, r0
.L156:
	cmp	r2, #0
	bne	.L157
.L158:
	add	r9, r9, #1
	adds	r5, r5, #20
	b	.L148
.L154:
	bl	memcpy
	b	.L158
.L176:
	.align	2
.L175:
	.word	mpp_dev_debug
	.word	.LANCHOR0+42
	.word	.LC7
	.word	.LC8
	.word	.LC33
	.word	-8192
	.fnend
	.size	rkvenc_result, .-rkvenc_result
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_show_session_info, %function
rkvenc_show_session_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r5, [r0, #64]
	ldr	r0, [r5, #104]
	add	r0, r0, #700
	bl	mutex_lock
	ldr	r3, [r5, #104]
	ldr	r0, [r3, #712]
	ldr	r4, [r0], #-176
	subs	r4, r4, #176
.L178:
	ldr	r2, [r5, #104]
	add	r3, r0, #176
	add	r1, r2, #712
	cmp	r3, r1
	bne	.L180
	add	r0, r2, #700
	bl	mutex_unlock
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L180:
	ldr	r3, [r0]
	cmp	r3, #16
	bne	.L179
	ldr	r3, [r0, #184]
	cbz	r3, .L179
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #60]
	cbz	r3, .L179
	mov	r1, r6
	blx	r3
.L179:
	ldr	r3, [r4, #176]
	mov	r0, r4
	sub	r4, r3, #176
	b	.L178
	.fnend
	.size	rkvenc_show_session_info, .-rkvenc_show_session_info
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_link_fill_table, %function
rkvenc_link_fill_table:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r7, r2
	ldr	r4, [r2, #40]
	add	fp, r0, #12
	ldr	r10, [r1, #2340]
	mov	r8, r0
	mov	r5, r1
	mov	r9, #0
	ldrb	r3, [r4]	@ zero_extendqisi2
	mov	r6, r10
	orr	r3, r3, #4
	strb	r3, [r4]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	ldr	r2, [r1, #2356]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #4]
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	ldr	r2, [r1, #2368]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #8]
	ldrb	r3, [r4, #12]	@ zero_extendqisi2
	ldr	r2, [r1, #2380]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #12]
	ldrb	r3, [r4, #16]	@ zero_extendqisi2
	ldr	r2, [r1, #2392]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #16]
	ldrb	r3, [r4, #20]	@ zero_extendqisi2
	ldr	r2, [r1, #2404]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #20]
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	ldr	r2, [r1, #2416]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #24]
	ldrb	r3, [r4, #28]	@ zero_extendqisi2
	ldr	r2, [r1, #2428]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #28]
	ldrb	r3, [r4, #32]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r4, #32]
.L188:
	ldr	r3, [r10, #24]
	cmp	r3, r9
	bhi	.L192
	ldr	r0, [r8, #40]
	movs	r1, #0
	ldr	r2, [r10, #184]
	bic	r0, r0, #3
	add	r0, r0, r4
	bl	memset
	ldr	r0, [r8]
	ldrd	r1, r2, [r7, #32]
	movs	r3, #2
	ldr	r0, [r0]
	bl	dma_sync_single_for_device
	ldr	r0, [r8]
	movs	r3, #2
	ldrd	r1, r2, [r7, #32]
	ldr	r0, [r0]
	bl	dma_sync_single_for_cpu
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L192:
	ldr	r3, [r6, #44]
	cbz	r3, .L189
	ldr	r3, [r5, #2344]
	cbz	r3, .L189
	ldr	r2, [r6, #40]
	ldr	r1, [r6, #36]
	ldr	r3, [r6, #28]
	ldr	r0, [fp, r9, lsl #2]
	cmp	r1, #0
	it	eq
	moveq	r1, r3
	cbnz	r2, .L191
	ldr	r2, [r6, #32]
.L191:
	rsb	ip, r1, #4
	subs	r1, r1, r3
	ldr	r3, [r5, #2348]
	bic	r0, r0, #3
	bic	r1, r1, #3
	add	r2, r2, ip
	add	r1, r1, r3
	add	r0, r0, r4
	bl	memcpy
.L189:
	add	r9, r9, #1
	adds	r6, r6, #20
	adds	r5, r5, #12
	b	.L188
	.fnend
	.size	rkvenc_link_fill_table, .-rkvenc_link_fill_table
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_prepare, %function
rkvenc_prepare:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r6, r0
	ldr	r7, .L234
	mov	r5, r1
	ldr	r4, [r0, #96]
	ldr	r3, [r7]
	lsls	r2, r3, #26
	bpl	.L201
	movw	r2, #1028
	ldr	r1, .L234+4
	ldr	r0, .L234+8
	bl	printk
.L201:
	ldr	r3, [r6, #72]
	cmp	r3, #0
	ble	.L202
.L231:
	movs	r4, #0
.L203:
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L200
	mov	r2, #1096
	ldr	r1, .L234+4
	ldr	r0, .L234+12
	bl	printk
.L200:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L202:
	ldr	r3, [r5]
	ldr	r3, [r3, #36]
	cmp	r3, #0
	bgt	.L231
	ldr	r3, [r6, #292]
	cmp	r3, #1
	beq	.L205
	cbz	r3, .L206
	cmp	r3, #2
	bne	.L231
	ldr	r8, [r6, #288]
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r4, #60]!
	cmp	r2, r4
	ite	eq
	moveq	r4, r5
	movne	r4, #0
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r4, #0
	beq	.L231
	add	r9, r8, #64
	mov	r0, r9
	bl	mutex_lock
	mov	r3, r8
	ldr	r6, [r3, #56]!
	cmp	r6, r3
	beq	.L212
	cbz	r6, .L212
	mov	r2, r6
	mov	r1, r5
	mov	r0, r8
	bl	rkvenc_link_fill_table
	add	r1, r8, #48
	mov	r0, r6
	bl	list_move_tail
	str	r6, [r5, #3592]
.L233:
	mov	r4, r5
.L212:
	mov	r0, r9
	bl	mutex_unlock
	b	.L203
.L206:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r4, #60]!
	cmp	r2, r4
	it	ne
	movne	r5, #0
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L232:
	mov	r4, r5
	b	.L203
.L205:
	ldr	r8, [r6, #288]
	ldr	r6, [r5, #2200]
	ubfx	r6, r6, #12, #1
	cbz	r6, .L211
	ldr	r3, [r5, #2200]
	movw	r2, #1073
	ldr	r1, .L234+4
	ldr	r0, .L234+16
	bl	printk
	b	.L232
.L211:
	add	r9, r8, #64
	mov	r0, r9
	bl	mutex_lock
	mov	r3, r8
	ldr	r4, [r3, #56]!
	cmp	r4, r3
	beq	.L215
	cmp	r4, #0
	beq	.L212
	mov	r2, r4
	mov	r1, r5
	mov	r0, r8
	bl	rkvenc_link_fill_table
	add	r1, r8, #48
	mov	r0, r4
	bl	list_move_tail
	str	r4, [r5, #3592]
	addw	r1, r5, #2200
	movs	r0, #12
	bl	____atomic_set_bit
	b	.L233
.L215:
	mov	r4, r6
	b	.L212
.L235:
	.align	2
.L234:
	.word	mpp_dev_debug
	.word	.LANCHOR0+56
	.word	.LC7
	.word	.LC8
	.word	.LC34
	.fnend
	.size	rkvenc_prepare, .-rkvenc_prepare
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_set_freq, %function
rkvenc_set_freq:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	adds	r0, r0, #156
	ldr	r1, [r1, #2936]
	bl	mpp_clk_set_rate
	ldr	r1, [r5, #2936]
	add	r0, r4, #212
	bl	mpp_clk_set_rate
	movs	r0, #0
	pop	{r3, r4, r5, pc}
	.fnend
	.size	rkvenc_set_freq, .-rkvenc_set_freq
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
	cbnz	r0, .L237
	mov	r0, r5
	bl	clk_enable
	mov	r4, r0
	cbz	r0, .L237
	mov	r0, r5
	bl	clk_unprepare
.L237:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
	.fnend
	.size	clk_prepare_enable, .-clk_prepare_enable
	.align	1
	.global	rkvenc_dump_dbg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_dump_dbg, %function
rkvenc_dump_dbg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	movs	r1, #96
	mov	r4, r0
	bl	mpp_read
	and	r1, r0, #1
	ldr	r0, .L253
	bl	printk
	mov	r1, #768
	mov	r0, r4
	bl	mpp_read
	and	r1, r0, #3
	ldr	r0, .L253+4
	bl	printk
	movw	r1, #1148
	mov	r0, r4
	bl	mpp_read
	lsrs	r1, r0, #31
	ldr	r0, .L253+8
	bl	printk
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L253+12
	bl	printk
	mov	r1, #20480
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L253+16
	bl	printk
	movw	r1, #20484
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L253+20
	bl	printk
	movw	r1, #20488
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L253+24
	bl	printk
	movw	r1, #20492
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L253+28
	bl	printk
	mov	r1, #20992
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L253+32
	bl	printk
	ldr	r3, .L253+36
	ldr	r3, [r3]
	lsls	r3, r3, #21
	bpl	.L243
	ldr	r1, .L253+40
	mvn	r7, #1
	ldr	r0, .L253+44
	bl	printk
	ldr	r6, .L253+48
	ldr	r9, .L253+52
.L245:
	cmp	r7, #4
	bhi	.L244
.L248:
	adds	r7, r7, #1
	adds	r6, r6, #20
	cmp	r7, #7
	bne	.L245
.L243:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L244:
	ldrd	r5, r8, [r6, #28]
.L246:
	cmp	r8, r5
	bcc	.L248
	mov	r1, r5
	mov	r0, r4
	bl	mpp_read
	mov	r1, r5
	adds	r5, r5, #4
	mov	r2, r0
	mov	r0, r9
	bl	printk
	b	.L246
.L254:
	.align	2
.L253:
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC42
	.word	.LC43
	.word	mpp_dev_debug
	.word	.LANCHOR0+71
	.word	.LC44
	.word	.LANCHOR1
	.word	.LC45
	.fnend
	.size	rkvenc_dump_dbg, .-rkvenc_dump_dbg
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_finish, %function
rkvenc_finish:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r0
	ldr	r3, .L282
	mov	r4, r1
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #32
	beq	.L256
	movw	r2, #1687
	ldr	r1, .L282+4
	ldr	r0, .L282+8
	bl	printk
.L256:
	ldr	r3, [r5, #292]
	cbz	r3, .L257
	cmp	r3, #2
	bne	.L258
.L257:
	ldr	r3, [r5, #52]
	lsls	r2, r3, #23
	bmi	.L259
	ldr	r3, [r5, #120]
	cbz	r3, .L260
.L259:
	mov	r0, r5
	bl	rkvenc_dump_dbg
.L260:
	mov	r7, r4
	mov	r10, #0
.L261:
	ldr	r3, [r4, #3268]
	cmp	r3, r10
	bhi	.L265
	ldr	r3, [r4, #2340]
	mov	r0, r4
	ldr	r1, [r3, #264]
	bl	rkvenc_get_class_reg
	cbz	r0, .L258
	ldr	r3, [r4, #2940]
	str	r3, [r0]
.L258:
	movs	r3, #0
	str	r3, [r5, #260]
	ldr	r3, [r8]
	lsls	r3, r3, #26
	bpl	.L266
	movw	r2, #1721
	ldr	r1, .L282+4
	ldr	r0, .L282+12
	bl	printk
.L266:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L265:
	ldr	fp, [r7, #3288]
	movs	r6, #0
.L262:
	ldr	r3, [r7, #3280]
	cmp	r6, r3, lsr #2
	bcc	.L264
	add	r10, r10, #1
	adds	r7, r7, #20
	b	.L261
.L264:
	ldr	r3, [r7, #3284]
	mov	r0, r5
	add	r9, r3, r6, lsl #2
	mov	r1, r9
	bl	mpp_read_relaxed
	str	r0, [fp, r6, lsl #2]
	ldr	r3, [r4, #2340]
	ldr	r3, [r3, #264]
	cmp	r3, r9
	itt	eq
	ldreq	r3, [r4, #2940]
	streq	r3, [fp, r6, lsl #2]
	adds	r6, r6, #1
	b	.L262
.L283:
	.align	2
.L282:
	.word	mpp_dev_debug
	.word	.LANCHOR0+87
	.word	.LC7
	.word	.LC8
	.fnend
	.size	rkvenc_finish, .-rkvenc_finish
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_init, %function
rkvenc_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r0
	ldr	r0, [r0]
	add	r7, r4, #156
	bl	mpp_dev_of_node
	ldr	r3, [r4, #104]
	ldr	r2, .L300
	mov	r1, r7
	adds	r3, r3, #172
	str	r3, [r4, #64]
	mov	r8, r0
	mov	r0, r4
	bl	mpp_get_clk_info
	cbz	r0, .L285
	movw	r2, #2213
	ldr	r1, .L300+4
	ldr	r0, .L300+8
	bl	printk
.L285:
	ldr	r2, .L300+12
	add	r1, r4, #184
	mov	r0, r4
	bl	mpp_get_clk_info
	cbz	r0, .L286
	movw	r2, #2216
	ldr	r1, .L300+4
	ldr	r0, .L300+16
	bl	printk
.L286:
	add	r6, r4, #212
	ldr	r2, .L300+20
	mov	r0, r4
	mov	r1, r6
	bl	mpp_get_clk_info
	cbz	r0, .L287
	movw	r2, #2219
	ldr	r1, .L300+4
	ldr	r0, .L300+24
	bl	printk
.L287:
	movs	r5, #0
	movs	r3, #1
	add	r2, r4, #240
	ldr	r1, .L300+28
	str	r5, [sp]
	mov	r0, r8
	bl	of_property_read_variable_u32_array
	ldr	r2, .L300+32
	mov	r1, r5
	mov	r0, r7
	bl	mpp_set_clk_info_rate_hz
	ldr	r2, .L300+36
	mov	r1, r5
	mov	r0, r6
	bl	mpp_set_clk_info_rate_hz
	ldr	r2, .L300+40
	mov	r1, r5
	mov	r0, r4
	bl	mpp_reset_control_get
	str	r0, [r4, #248]
	cbnz	r0, .L288
	movw	r2, #2231
	ldr	r1, .L300+4
	ldr	r0, .L300+44
	bl	printk
.L288:
	ldr	r2, .L300+48
	movs	r1, #1
	mov	r0, r4
	bl	mpp_reset_control_get
	str	r0, [r4, #252]
	cbnz	r0, .L289
	movw	r2, #2234
	ldr	r1, .L300+4
	ldr	r0, .L300+52
	bl	printk
.L289:
	ldr	r2, .L300+56
	movs	r1, #4
	mov	r0, r4
	bl	mpp_reset_control_get
	str	r0, [r4, #256]
	cbnz	r0, .L290
	movw	r2, #2237
	ldr	r1, .L300+4
	ldr	r0, .L300+60
	bl	printk
.L290:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L301:
	.align	2
.L300:
	.word	.LC46
	.word	.LANCHOR0+101
	.word	.LC47
	.word	.LC48
	.word	.LC49
	.word	.LC50
	.word	.LC51
	.word	.LC52
	.word	300000000
	.word	600000000
	.word	.LC53
	.word	.LC54
	.word	.LC55
	.word	.LC56
	.word	.LC57
	.word	.LC58
	.fnend
	.size	rkvenc_init, .-rkvenc_init
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_add.constprop.15, %function
atomic_add.constprop.15:
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
	.size	atomic_add.constprop.15, .-atomic_add.constprop.15
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_shutdown, %function
rkvenc_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	add	r4, r0, #16
	ldr	r5, [r0, #80]
	ldr	r1, .L314
	mov	r0, r4
	bl	_dev_info
	ldr	r0, [r5, #104]
	adds	r0, r0, #84
	bl	atomic_add.constprop.15
	bl	ktime_get
	ldr	r3, .L314+4
	adds	r6, r0, r3
	mov	r3, #0
	adc	r7, r1, r3
.L309:
	ldr	r3, [r5, #80]
	cbz	r3, .L305
	bl	ktime_get
	cmp	r6, r0
	sbcs	r3, r7, r1
	bge	.L306
	ldr	r3, [r5, #80]
	cbz	r3, .L305
	ldr	r1, .L314+8
	mov	r0, r4
	bl	_dev_err
.L305:
	mov	r0, r4
	ldr	r1, .L314+12
	pop	{r3, r4, r5, r6, r7, lr}
	b	_dev_info
.L306:
	mov	r1, #1000
	movs	r0, #251
	bl	usleep_range
	b	.L309
.L315:
	.align	2
.L314:
	.word	.LC59
	.word	200000000
	.word	.LC60
	.word	.LC61
	.fnend
	.size	rkvenc_shutdown, .-rkvenc_shutdown
	.text
	.align	1
	.global	mpp_rkvenc_irq
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_rkvenc_irq, %function
mpp_rkvenc_irq:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r4, r1
	ldr	r0, [r1, #96]
	ldr	r6, [r1, #152]
	bl	mpp_taskqueue_get_running_task
	ldr	r7, .L374
	ldr	r3, [r7]
	lsls	r2, r3, #26
	mov	r5, r0
	bpl	.L317
	movw	r2, #1370
	ldr	r1, .L374+4
	ldr	r0, .L374+8
	bl	printk
.L317:
	ldr	r1, [r6, #264]
	mov	r0, r4
	bl	mpp_read
	str	r0, [r4, #52]
	cbz	r0, .L316
	mov	r2, #256
	ldr	r1, [r6, #256]
	mov	r0, r4
	bl	mpp_write
	ldr	r2, [r4, #52]
	ldr	r1, [r6, #260]
	mov	r0, r4
	bl	mpp_write
	movs	r2, #0
	ldr	r1, [r6, #264]
	mov	r0, r4
	bl	mpp_write
	ldr	r3, [r4, #52]
	cmp	r3, #32768
	bne	.L319
.L373:
	movs	r0, #1
.L316:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L319:
	mov	r0, r4
	bl	rkvenc_check_bs_overflow
	cmp	r0, #0
	bne	.L373
	cbnz	r5, .L321
	ldr	r1, .L374+12
	ldr	r0, [r4]
	bl	_dev_err
	b	.L373
.L321:
	ldr	r8, [r5]
	addw	r9, r5, #2200
	ldr	r6, [r8, #184]
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
	lsls	r3, r3, #28
	bpl	.L322
	ldr	r3, [r5, #2200]
	movw	r2, #1398
	ldr	r1, .L374+4
	ldr	r0, .L374+16
	str	r3, [sp]
	ldr	r3, [r5, #2276]
	bl	printk
	b	.L373
.L322:
	add	r0, r5, #2208
	bl	cancel_delayed_work
	mov	r1, r9
	movs	r0, #4
	bl	____atomic_set_bit
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	ubfx	r0, r0, #0, #14
	str	r0, [r4, #272]
	mov	r0, r4
	bl	rkvenc_clear_dvbm_info
	ldr	r3, [r6, #16]
	cbz	r3, .L324
	ldr	r2, [r4, #272]
	movw	r3, #16383
	cmp	r2, r3
	bne	.L325
	ldr	r3, [r7]
	movs	r2, #1
	str	r2, [r4, #268]
	lsls	r0, r3, #13
	bpl	.L326
	ldr	r0, .L374+20
	bl	printk
.L325:
	ldr	r2, [r4, #272]
	movw	r3, #16382
	cmp	r2, r3
	bne	.L327
	ldr	r3, [r4, #52]
	orr	r3, r3, #65536
	str	r3, [r4, #52]
	ldr	r3, [r6, #212]
	adds	r3, r3, #1
	str	r3, [r6, #212]
.L327:
	ldr	r3, [r4, #268]
	cbz	r3, .L324
.L326:
	ldr	r3, [r4, #52]
	orr	r3, r3, #64
	str	r3, [r4, #52]
	movs	r3, #0
	str	r3, [r4, #268]
	ldr	r3, [r6, #204]
	adds	r3, r3, #1
	str	r3, [r6, #204]
.L324:
	ldr	r3, [r4, #56]
	cbz	r3, .L329
	ldr	r3, [r6, #208]
	adds	r3, r3, #1
	str	r3, [r6, #208]
.L329:
	ldrd	r3, r2, [r4, #52]
	orrs	r3, r3, r2
	ldr	r2, [r7]
	str	r3, [r5, #2940]
	lsls	r1, r2, #29
	bpl	.L330
	str	r3, [sp]
	ldr	r0, .L374+24
	ldr	r3, [r6, #16]
	ldr	r2, [r5, #2336]
	ldr	r1, [r5, #2276]
	bl	printk
.L330:
	mov	r0, r5
	bl	mpp_time_diff
	mov	r1, r9
	movs	r0, #7
	bl	____atomic_set_bit
	mov	r1, r5
	ldr	r0, [r4, #96]
	bl	mpp_taskqueue_pop_running
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #36]
	cbz	r3, .L331
	mov	r1, r5
	mov	r0, r4
	blx	r3
.L331:
	ldr	r3, [r4, #152]
	ldr	r2, [r3, #272]
	ldr	r3, [r4, #52]
	tst	r2, r3
	beq	.L333
	ldr	r3, [r7]
	lsls	r2, r3, #13
	bpl	.L334
	ldr	r3, [r5, #2940]
	ldr	r0, .L374+24
	str	r3, [sp]
	ldr	r3, [r6, #16]
	ldr	r2, [r5, #2336]
	ldr	r1, [r5, #2276]
	bl	printk
.L334:
	ldr	r3, [r5, #2940]
	str	r3, [r6, #236]
	ldr	r3, [r6, #232]
	adds	r3, r3, #1
	str	r3, [r6, #232]
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #28]
	cbz	r3, .L333
	mov	r0, r4
	blx	r3
.L333:
	movs	r3, #0
	movs	r2, #1
	str	r3, [r6, #200]
	movs	r1, #3
	str	r3, [r4, #56]
	addw	r0, r5, #2284
	bl	__wake_up
	mov	r0, r4
	bl	mpp_taskqueue_trigger_work
	ldr	r3, [r8, #240]
	cbz	r3, .L336
	ldr	r2, [r5, #2308]
	cbz	r2, .L336
	ldr	r0, [r8, #196]
	blx	r3
.L336:
	add	r0, r4, #132
	bl	up_read
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L373
	movw	r2, #1463
	ldr	r1, .L374+4
	ldr	r0, .L374+28
	bl	printk
	b	.L373
.L375:
	.align	2
.L374:
	.word	mpp_dev_debug
	.word	.LANCHOR0+113
	.word	.LC7
	.word	.LC62
	.word	.LC63
	.word	.LC64
	.word	.LC65
	.word	.LC8
	.fnend
	.size	mpp_rkvenc_irq, .-mpp_rkvenc_irq
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_callback, %function
rkvenc_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r5, r2
	.pad #24
	sub	sp, sp, #24
	mov	r4, r0
	cbz	r0, .L378
	subs	r1, r1, #18
	cmp	r1, #4
	bhi	.L378
	tbb	[pc, r1]
.L380:
	.byte	(.L384-.L380)/2
	.byte	(.L383-.L380)/2
	.byte	(.L382-.L380)/2
	.byte	(.L381-.L380)/2
	.byte	(.L379-.L380)/2
	.p2align 1
.L383:
	movs	r1, #24
	ldr	r5, [r2]
	bl	mpp_read
	str	r0, [sp, #20]
	cbnz	r5, .L385
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [sp, #20]
	bic	r3, r3, #16777216
	str	r3, [sp, #20]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L385:
	ldr	r2, [sp, #20]
	movs	r1, #24
	mov	r0, r4
	bl	mpp_write
.L378:
	movs	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L382:
	ldr	r3, [r2]
	str	r3, [r0, #276]
	b	.L378
.L381:
	ldr	r3, [r2]
	str	r3, [r0, #280]
	b	.L378
.L379:
	movs	r1, #24
	bl	mpp_read
	movw	r1, #20488
	ubfx	r7, r0, #14, #8
	mov	r6, r0
	ubfx	r8, r0, #0, #14
	mov	r0, r4
	bl	mpp_read
	movw	r1, #20492
	mov	r9, r0
	mov	r0, r4
	bl	mpp_read
	movw	r1, #20484
	mov	r10, r0
	mov	r0, r4
	bl	mpp_read
	lsr	r2, r9, #16
	lsr	r9, r10, #16
	ldr	r1, [r5]
	cmp	r9, r2
	ldr	r3, [r5, #4]
	it	cc
	movcc	r9, r2
	cmp	r7, r1
	lsl	r2, r9, #4
	bne	.L386
	cmp	r8, r3
	bcc	.L387
.L386:
	cbz	r3, .L388
	cmp	r7, r1
	beq	.L388
	ldr	r0, [r5, #8]
	cmp	r3, r0
	bcc	.L389
	strd	r1, r3, [sp]
	mov	r3, r8
	mov	r1, r7
	ldr	r0, .L403
	bl	printk
	movs	r3, #1
	str	r3, [r4, #268]
.L389:
	mov	r0, r4
	bl	rkvenc_dump_dbg
.L388:
	ldr	r3, [r5, #12]
	cmp	r8, r3
	bcs	.L378
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	ldr	r2, [r5, #12]
	movs	r1, #24
	bic	r0, r0, #16320
	bic	r0, r0, #63
	orrs	r2, r2, r0
	mov	r0, r4
	bl	mpp_write
	.syntax unified
@ 772 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r3, [r5, #4]
	mov	r2, r7
	ldr	r0, .L403+4
	str	r3, [sp, #4]
	ldr	r3, [r5]
	str	r3, [sp]
	mov	r3, r8
	ldr	r1, [r5, #12]
	bl	printk
	b	.L378
.L387:
	ldr	ip, [r4, #260]
	ldr	r1, [r5, #8]
	cmp	ip, #0
	beq	.L391
	sub	ip, r3, r2
	cmp	r1, ip
.L402:
	bcs	.L392
	strd	r3, r0, [sp, #4]
	mov	r1, r7
	mov	r3, r8
	str	r7, [sp]
	ldr	r0, .L403+8
	bl	printk
	movs	r3, #1
	str	r3, [r4, #268]
.L392:
	ldr	r3, [r5, #4]
	bic	r6, r6, #16320
	bic	r6, r6, #63
	movs	r1, #24
	orrs	r6, r6, r3
	mov	r0, r4
	str	r3, [r4, #272]
	mov	r2, r6
	bl	mpp_write
	.syntax unified
@ 840 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r3, .L403+12
	ldr	r3, [r3]
	lsls	r3, r3, #15
	bpl	.L393
	ldrd	r1, r2, [r5]
	ldr	r0, .L403+16
	bl	printk
.L393:
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	cmp	r0, r6
	beq	.L378
	ldr	r3, [r5, #4]
	mov	r2, r8
	mov	r1, r7
	ldr	r0, .L403+20
	str	r3, [sp]
	ldr	r3, [r5]
	bl	printk
	b	.L378
.L391:
	cmp	r1, r3
	b	.L402
.L384:
	bl	rkvenc_dump_dbg
	b	.L378
.L404:
	.align	2
.L403:
	.word	.LC66
	.word	.LC67
	.word	.LC68
	.word	mpp_dev_debug
	.word	.LC69
	.word	.LC70
	.fnend
	.size	rkvenc_callback, .-rkvenc_callback
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_control, %function
rkvenc_control:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	movw	r2, #1027
	ldr	r3, [r1]
	.pad #24
	sub	sp, sp, #24
	mov	r7, r1
	cmp	r3, r2
	beq	.L406
	movw	r2, #1028
	cmp	r3, r2
	beq	.L407
	movw	r2, #1862
	ldr	r1, .L432
	ldr	r0, .L432+4
	bl	printk
	b	.L431
.L406:
	cbz	r0, .L409
	ldr	r6, [r0, #184]
	cbnz	r6, .L410
.L409:
	movw	r2, #1834
	ldr	r1, .L432
	ldr	r0, .L432+8
	bl	printk
	mvn	r0, #21
.L405:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L410:
	ldr	r4, [r1, #8]
	ldr	r3, .L432+12
	lsrs	r4, r4, #4
	cmp	r4, #11
	ldr	r3, [r3]
	it	ge
	movge	r4, #11
	lsls	r2, r3, #27
	bpl	.L412
	mov	r1, r4
	ldr	r0, .L432+16
	bl	printk
.L412:
	ldr	r9, .L432+24
	movs	r5, #0
	ldr	r8, .L432
	ldr	r10, .L432+28
.L413:
	cmp	r4, r5
	bne	.L418
.L431:
	movs	r0, #0
	b	.L405
.L418:
	ldr	r1, [r7, #16]
	mov	r3, sp
	and	r3, r3, r9
	add	r1, r1, r5, lsl #4
	ldr	r3, [r3, #8]
	.syntax unified
@ 157 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/include/linux/uaccess.h" 1
	.syntax unified
adds r2, r1, #16; sbcscc r2, r2, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #16
	cbnz	r3, .L414
	add	r0, sp, #8
	bl	arm_copy_from_user
	mov	r2, r0
	cmp	r0, #0
	beq	.L415
.L414:
	rsb	r0, r2, #16
	add	r3, sp, #8
	movs	r1, #0
	add	r0, r0, r3
	bl	memset
	movw	r2, #1844
	mov	r1, r8
	mov	r0, r10
	bl	printk
.L416:
	adds	r5, r5, #1
	b	.L413
.L422:
	subs	r1, r2, #1
	cmp	r1, #1
	bhi	.L417
	.syntax unified
@ 87 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
	cmp	r3, #11
	sbc	r1, r3, r3
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r3, r3, r1
	str	r3, [sp, #8]
	add	r1, r6, r3, lsl #4
	str	r2, [r1, #24]
	ldrd	r2, [sp, #16]
	strd	r2, [r1, #32]
	b	.L416
.L407:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r4, r0
	ldr	r9, .L432+32
	ldr	r6, [r4, #20]!
	mov	r10, #1
	ldr	r5, [r6], #-4
	subs	r5, r5, #4
.L419:
	adds	r3, r6, #4
	cmp	r3, r4
	bne	.L421
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L431
.L421:
	ldr	r3, [r6, #2200]
	lsls	r3, r3, #30
	bmi	.L420
	ldr	r1, [r6, #2276]
	mov	r0, r9
	bl	printk
	str	r10, [r6, #2312]
.L420:
	ldr	r3, [r5, #4]
	mov	r6, r5
	subs	r5, r3, #4
	b	.L419
.L415:
	ldrd	r3, r2, [sp, #8]
	subs	r1, r3, #1
	cmp	r1, #9
	bls	.L422
.L417:
	str	r2, [sp]
	mov	r1, r8
	movw	r2, #1854
	ldr	r0, .L432+20
	bl	printk
	b	.L416
.L433:
	.align	2
.L432:
	.word	.LANCHOR0+128
	.word	.LC76
	.word	.LC71
	.word	mpp_dev_debug
	.word	.LC72
	.word	.LC74
	.word	-8192
	.word	.LC73
	.word	.LC75
	.fnend
	.size	rkvenc_control, .-rkvenc_control
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_isr, %function
rkvenc_isr:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r6, .L502
	.pad #28
	sub	sp, sp, #28
	ldr	r3, [r6]
	lsls	r0, r3, #26
	bpl	.L435
	movw	r2, #1590
	ldr	r1, .L502+4
	ldr	r0, .L502+8
	bl	printk
.L435:
	ldr	r7, [r4, #292]
	cmp	r7, #1
	beq	.L436
	cbz	r7, .L437
	cmp	r7, #2
	beq	.L438
.L439:
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L441
	movw	r2, #1676
	ldr	r1, .L502+4
	ldr	r0, .L502+12
	bl	printk
	b	.L441
.L437:
	ldr	r5, [r4, #88]
	cmp	r5, #0
	beq	.L441
	ldr	r3, [r5]
	mov	r0, r5
	ldr	r8, [r3, #184]
	bl	mpp_time_diff
	str	r7, [r4, #88]
	ldr	r3, [r8, #16]
	cbz	r3, .L443
	ldr	r2, [r4, #272]
	movw	r3, #16383
	cmp	r2, r3
	bne	.L444
	movs	r3, #1
	ldr	r1, .L502+16
	str	r3, [r4, #268]
	ldr	r0, [r4]
	bl	_dev_err
.L444:
	ldr	r3, [r4, #268]
	cbz	r3, .L443
	ldr	r3, [r4, #52]
	orr	r3, r3, #64
	str	r3, [r4, #52]
	movs	r3, #0
	str	r3, [r4, #268]
.L443:
	ldrd	r3, r2, [r4, #52]
	orrs	r3, r3, r2
	str	r3, [r5, #2940]
	movs	r3, #0
	str	r3, [r4, #56]
	ldr	r3, [r6]
	lsls	r1, r3, #29
	bpl	.L446
	ldr	r3, [r5, #2940]
	ldr	r0, .L502+20
	str	r3, [sp]
	ldr	r3, [r8, #16]
	ldr	r2, [r5, #2336]
	ldr	r1, [r5, #2276]
	bl	printk
.L446:
	ldr	r3, [r4, #152]
	ldr	r2, [r3, #272]
	ldr	r3, [r4, #52]
	tst	r2, r3
	beq	.L448
	ldr	r3, [r5, #2940]
	mov	r7, r4
	ldr	r1, .L502+24
	str	r3, [sp, #4]
	ldr	r3, [r8, #16]
	str	r3, [sp]
	ldr	r0, [r7], #72
	ldr	r3, [r5, #2336]
	ldr	r2, [r5, #2276]
	bl	_dev_err
	mov	r0, r7
	bl	atomic_add.constprop.15
	ldr	r3, [r6]
	lsls	r2, r3, #21
	bpl	.L448
	ldr	r1, [r5, #2940]
	ldr	r0, .L502+28
	bl	printk
	mov	r0, r4
	bl	rkvenc_dump_dbg
.L448:
	mov	r1, r5
	ldr	r0, [r5]
	bl	mpp_task_finish
	b	.L439
.L438:
	ldr	r5, [r4, #88]
	cbnz	r5, .L450
	ldr	r1, .L502+32
	ldr	r0, [r4]
	bl	_dev_err
.L441:
	movs	r0, #1
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L450:
	mov	r0, r5
	bl	mpp_time_diff
	movs	r3, #0
	str	r3, [r4, #88]
	ldr	r3, [r6]
	ldr	r1, [r4, #52]
	lsls	r3, r3, #29
	str	r1, [r5, #2940]
	bpl	.L451
	ldr	r0, .L502+28
	bl	printk
.L451:
	ldr	r3, [r4, #152]
	ldr	r2, [r3, #272]
	ldr	r3, [r5, #2940]
	tst	r2, r3
	beq	.L453
	add	r0, r4, #72
	bl	atomic_add.constprop.15
	ldr	r3, [r6]
	lsls	r7, r3, #21
	bpl	.L453
	ldr	r1, [r5, #2940]
	ldr	r0, .L502+28
	bl	printk
.L453:
	mov	r1, r5
	ldr	r0, [r5]
	bl	mpp_task_finish
	ldr	r0, [r4, #288]
	adds	r0, r0, #64
	bl	mutex_lock
	ldr	r1, [r4, #288]
	ldr	r0, [r5, #3592]
	adds	r1, r1, #56
	bl	list_move_tail
	ldr	r0, [r4, #288]
	adds	r0, r0, #64
	bl	mutex_unlock
	b	.L439
.L436:
	ldr	r3, [r4, #52]
	lsls	r5, r3, #23
	bmi	.L455
	ldr	r3, [r4, #120]
	cbz	r3, .L456
.L455:
	mov	r0, r4
	bl	rkvenc_dump_dbg
.L456:
	ldr	r10, [r4, #152]
	movs	r3, #0
	ldr	r3, [r3]
	mov	r0, r4
	ldr	r9, [r4, #288]
	ldr	r1, [r10, #292]
	str	r3, [sp, #12]
	bl	mpp_read_relaxed
	ldr	r3, [r4, #96]
	ldr	r5, [r3, #60]
	ldr	r7, [r5], #-20
	subs	r7, r7, #20
	lsrs	r3, r0, #24
	str	r3, [sp, #16]
.L457:
	ldr	r1, [r4, #96]
	add	r8, r5, #20
	add	r2, r1, #60
	cmp	r8, r2
	bne	.L463
.L458:
	ldr	r3, [r1, #60]
	cmp	r2, r3
	bne	.L464
	ldr	r3, [sp, #12]
	ldr	r3, [r3, #16]
	cbz	r3, .L464
	ldr	r2, [r4, #52]
	ldr	r3, [r10, #272]
	tst	r2, r3
	beq	.L464
	mov	r0, r4
	bl	rkvenc_reset
.L464:
	ldr	r3, [r6]
	lsls	r2, r3, #26
	bpl	.L439
	movw	r2, #1581
	ldr	r1, .L502+36
	ldr	r0, .L502+12
	bl	printk
	b	.L439
.L463:
	ldr	r3, [r5, #3596]
	ldr	r0, [sp, #16]
	subs	r3, r3, r0
	tst	r3, #255
	bne	.L458
	add	r0, r5, #2208
	bl	cancel_delayed_work
	mov	r0, r5
	bl	mpp_time_diff
	ldr	r3, [r6]
	ldr	r1, [r4, #52]
	lsls	r0, r3, #29
	str	r1, [r5, #2940]
	bpl	.L459
	ldr	r0, .L502+40
	bl	printk
.L459:
	ldr	r3, [r5, #2340]
	addw	fp, r5, #2200
	ldr	r1, [r5, #3592]
	ldr	r0, [r9, #12]
	ldr	r3, [r3, #264]
	ldr	r2, [r5, #2940]
	add	r3, r3, r0
	ldr	r1, [r1, #40]
	bic	r3, r3, #3
	str	r2, [r1, r3]
	ldr	r3, [r5, #2204]
	cbnz	r3, .L460
	mov	r1, fp
	movs	r0, #7
	str	r3, [sp, #20]
	bl	____atomic_set_bit
	movs	r2, #1
	movs	r1, #3
	addw	r0, r5, #2284
	ldr	r3, [sp, #20]
	bl	__wake_up
.L460:
	add	r3, r9, #64
	str	r3, [sp, #20]
	mov	r0, r3
	bl	mutex_lock
	add	r1, r9, #56
	ldr	r0, [r5, #3592]
	bl	list_move_tail
	ldr	r3, [sp, #20]
	mov	r0, r3
	bl	mutex_unlock
	mov	r1, fp
	movs	r0, #5
	bl	____atomic_set_bit
	ldrd	r2, r3, [r5, #20]
	addw	r0, r5, #2252
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r8, [r5, #20]
	str	r8, [r5, #24]
	pld	[r0]
	.syntax unified
@ 224 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_fetch_sub
1:	ldrex	r3, [r0]
	sub	r2, r3, #1
	strex	r1, r2, [r0]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r3, #1
	bne	.L461
	bl	mpp_free_task
.L462:
	ldr	r3, [r7, #20]
	mov	r5, r7
	sub	r7, r3, #20
	b	.L457
.L461:
	cmp	r3, #0
	bgt	.L462
	movs	r1, #3
	bl	refcount_warn_saturate
	b	.L462
.L503:
	.align	2
.L502:
	.word	mpp_dev_debug
	.word	.LANCHOR0+143
	.word	.LC7
	.word	.LC8
	.word	.LC77
	.word	.LC65
	.word	.LC78
	.word	.LC79
	.word	.LC80
	.word	.LANCHOR0+154
	.word	.LC81
	.fnend
	.size	rkvenc_isr, .-rkvenc_isr
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_init_session, %function
rkvenc_init_session:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	cbnz	r0, .L505
	movw	r2, #1906
	ldr	r1, .L527
	ldr	r0, .L527+4
	bl	printk
	mvn	r0, #21
.L504:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L505:
	ldr	r5, .L527+8
	mov	r1, #3520
	ldr	r8, [r0, #12]
	ldr	r0, [r5, #32]
	bl	kmem_cache_alloc
	mov	r6, r0
	cbnz	r0, .L507
.L526:
	mvn	r0, #11
	b	.L504
.L507:
	ldr	r2, .L527+12
	add	r7, r4, #196
	ldr	r1, .L527+16
	bl	__init_rwsem
	str	r6, [r4, #184]
	add	r6, r4, #188
.L512:
	mov	r1, #3520
	ldr	r0, [r5, #48]
	bl	kmem_cache_alloc
	cbz	r0, .L508
	ldr	r3, [r8, #4]
	mov	ip, r0
	mov	lr, #0
	ldr	r9, [r3, #4]
	mov	r2, r9
	str	r9, [r0, #2340]
.L509:
	ldr	r3, [r9, #24]
	cmp	r3, lr
	bhi	.L511
	str	r0, [r6], #4
	cmp	r6, r7
	bne	.L512
	movs	r0, #0
	b	.L504
.L511:
	ldr	r3, [ip, #2348]
	cbnz	r3, .L510
	ldr	r3, [r2, #32]
	ldr	r1, [r2, #28]
	adds	r3, r3, #4
	subs	r1, r3, r1
	str	r1, [ip, #2352]
.L510:
	add	lr, lr, #1
	adds	r2, r2, #20
	add	ip, ip, #12
	b	.L509
.L508:
	ldr	r0, [r4, #184]
	cbz	r0, .L513
	bl	kfree
.L513:
	ldr	r5, [r4, #188]
	cbz	r5, .L514
	mov	r0, r5
	bl	rkvenc_invalid_class_msg
	mov	r0, r5
	bl	kfree
.L514:
	ldr	r4, [r4, #192]
	cmp	r4, #0
	beq	.L526
	mov	r0, r4
	bl	rkvenc_invalid_class_msg
	mov	r0, r4
	bl	kfree
	b	.L526
.L528:
	.align	2
.L527:
	.word	.LANCHOR0+170
	.word	.LC82
	.word	kmalloc_caches
	.word	.LANCHOR2
	.word	.LC83
	.fnend
	.size	rkvenc_init_session, .-rkvenc_init_session
	.section	.text.unlikely
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_link_remove.constprop.3, %function
rkvenc_link_remove.constprop.3:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r5, .L544
	ldr	r3, [r5]
	lsls	r2, r3, #26
	bpl	.L530
	movw	r2, #2340
	ldr	r1, .L544+4
	ldr	r0, .L544+8
	bl	printk
	cbz	r4, .L531
.L538:
	add	r8, r4, #64
	mov	r6, r4
	mov	r0, r8
	bl	mutex_lock
	ldr	r0, [r6, #48]!
	ldr	r7, [r0]
.L532:
	cmp	r0, r6
	bne	.L533
	mov	r6, r4
	ldr	r0, [r6, #56]!
	ldr	r7, [r0]
.L534:
	cmp	r0, r6
	bne	.L535
	mov	r0, r8
	bl	mutex_unlock
	movs	r3, #0
	str	r3, [r4, #4]
.L531:
	ldr	r3, [r5]
	lsls	r3, r3, #26
	bpl	.L543
	movw	r2, #2358
	ldr	r1, .L544+4
	ldr	r0, .L544+12
	bl	printk
.L543:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
.L533:
	ldrd	r2, r3, [r0]
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r0, [r0]
	str	r0, [r0, #4]
	bl	mpp_dma_free
	mov	r0, r7
	ldr	r7, [r7]
	b	.L532
.L535:
	ldrd	r2, r3, [r0]
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r0, [r0]
	str	r0, [r0, #4]
	bl	mpp_dma_free
	mov	r0, r7
	ldr	r7, [r7]
	b	.L534
.L530:
	cmp	r0, #0
	bne	.L538
	b	.L543
.L545:
	.align	2
.L544:
	.word	mpp_dev_debug
	.word	.LANCHOR0+190
	.word	.LC7
	.word	.LC8
	.fnend
	.size	rkvenc_link_remove.constprop.3, .-rkvenc_link_remove.constprop.3
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_remove, %function
rkvenc_remove:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	adds	r0, r0, #16
	ldr	r4, [r0, #64]
	ldr	r1, .L555
	bl	_dev_info
	ldr	r0, [r4, #264]
	cbz	r0, .L547
	bl	rk_dvbm_put
	movs	r3, #0
	str	r3, [r4, #264]
.L547:
	mov	r0, r4
	bl	mpp_dev_remove
	ldr	r0, [r4, #244]
	cbz	r0, .L548
	bl	proc_remove
	movs	r3, #0
	str	r3, [r4, #244]
.L548:
	ldr	r0, [r4, #288]
	bl	rkvenc_link_remove.constprop.3
	movs	r0, #0
	pop	{r4, pc}
.L556:
	.align	2
.L555:
	.word	.LC84
	.fnend
	.size	rkvenc_remove, .-rkvenc_remove
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_clk_on, %function
rkvenc_clk_on:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r0, [r0, #156]
	cbz	r0, .L558
	bl	clk_prepare_enable
.L558:
	ldr	r0, [r4, #184]
	cbz	r0, .L559
	bl	clk_prepare_enable
.L559:
	ldr	r0, [r4, #212]
	cbz	r0, .L560
	bl	clk_prepare_enable
.L560:
	movs	r0, #0
	pop	{r4, pc}
	.fnend
	.size	rkvenc_clk_on, .-rkvenc_clk_on
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_clk_off, %function
rkvenc_clk_off:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r5, [r0, #156]
	mov	r0, r5
	bl	clk_disable
	mov	r0, r5
	bl	clk_unprepare
	ldr	r5, [r4, #184]
	mov	r0, r5
	bl	clk_disable
	mov	r0, r5
	bl	clk_unprepare
	ldr	r4, [r4, #212]
	mov	r0, r4
	bl	clk_disable
	mov	r0, r4
	bl	clk_unprepare
	movs	r0, #0
	pop	{r3, r4, r5, pc}
	.fnend
	.size	rkvenc_clk_off, .-rkvenc_clk_off
	.section	.text.unlikely
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_probe, %function
rkvenc_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	add	r7, r0, #16
	ldr	r6, .L643
	.pad #84
	sub	sp, sp, #84
	ldr	r1, .L643+4
	mov	r5, r0
	mov	r0, r7
	ldr	r3, [r6]
	str	r3, [sp, #76]
	bl	_dev_info
	mov	r2, #3520
	mov	r1, #312
	mov	r0, r7
	bl	devm_kmalloc
	mov	r4, r0
	str	r6, [sp, #28]
	cmp	r0, #0
	beq	.L602
	str	r0, [r5, #80]
	mov	r0, r7
	bl	mpp_dev_of_node
	cbz	r0, .L573
	mov	r1, r7
	ldr	r0, .L643+8
	bl	of_match_device
	cbnz	r0, .L574
	ldr	r1, .L643+12
	mov	r0, r7
	bl	_dev_err
	mvn	r9, #18
.L572:
	mov	r0, r7
	ldr	r1, .L643+16
	bl	_dev_info
	mov	r0, r9
	ldr	r3, [sp, #28]
	ldr	r2, [sp, #76]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L601
	bl	__stack_chk_fail
.L574:
	ldr	r3, [r0, #192]
	str	r3, [r4, #4]
.L573:
	mov	r1, r5
	mov	r0, r4
	bl	mpp_dev_probe
	mov	r9, r0
	cmp	r0, #0
	bne	.L572
	add	r3, r4, #16
	mov	r1, r0
	movs	r2, #20
	mov	r0, r3
	bl	memset
	ldr	r3, .L643+20
	str	r3, [r4, #24]
	ldr	r3, [r5, #60]
	str	r0, [r4, #16]
	str	r0, [r4, #20]
	cbnz	r3, .L575
	ldr	r3, [r5, #16]
.L575:
	strd	r3, r4, [sp, #4]
	mov	r0, r7
	movs	r3, #128
	ldr	r2, .L643+24
	str	r3, [sp]
	movs	r3, #0
	ldr	r1, [r4, #48]
	bl	devm_request_threaded_irq
	mov	r9, r0
	cbz	r0, .L576
	mov	r0, r7
	ldr	r1, .L643+28
	bl	_dev_err
	mov	r0, r4
	bl	mpp_dev_remove
	b	.L572
.L576:
	movs	r3, #40
	str	r3, [r4, #92]
	ldr	r3, [r4, #4]
	ldr	r0, [r4]
	ldr	r3, [r3, #4]
	str	r3, [r4, #152]
	cbz	r0, .L578
	bl	mpp_dev_of_node
	cbz	r0, .L578
	ldr	r3, [r4, #104]
	cbz	r3, .L578
	ldr	r3, [r3, #76]
	cbz	r3, .L578
	ldr	r0, [r4]
	bl	mpp_dev_of_node
	ldr	r3, [r0]
	cbz	r3, .L578
	ldr	r2, [r4, #116]
	movs	r1, #31
	add	r0, sp, #44
	str	r2, [sp]
	ldr	r2, .L643+32
	bl	snprintf
	ldr	r3, [r4, #104]
	ldr	r1, [r3, #76]
	add	r0, sp, #44
	bl	proc_mkdir
	mov	r2, r0
	str	r0, [r4, #244]
	cbz	r0, .L580
	cmn	r0, #4096
	bls	.L581
.L580:
	movw	r2, #2163
	ldr	r1, .L643+36
	ldr	r0, .L643+40
	bl	printk
	movs	r3, #0
	str	r3, [r4, #244]
.L578:
	ldr	r1, [r4, #104]
	mov	r0, r4
	bl	mpp_dev_register_srv
	mov	r0, r7
	bl	mpp_dev_of_node
	movs	r2, #0
	ldr	r1, .L643+44
	bl	of_parse_phandle
	mov	r5, r0
	cmp	r0, #0
	bne	.L582
.L584:
	movw	r2, #2530
	ldr	r1, .L643+48
	ldr	r0, .L643+52
	bl	printk
.L583:
	movs	r3, #0
	mov	r0, r7
	str	r3, [r4, #292]
	bl	mpp_dev_of_node
	ldr	r3, .L643+56
	ldr	r2, [r3]
	mov	r10, r3
	tst	r2, #32
	mov	r6, r0
	beq	.L585
	movw	r2, #2443
	ldr	r1, .L643+60
	ldr	r0, .L643+64
	bl	printk
.L585:
	movs	r5, #0
	movs	r3, #1
	str	r5, [sp]
	add	r2, r4, #284
	ldr	r1, .L643+68
	mov	r0, r6
	bl	of_property_read_variable_u32_array
	cmp	r0, r5
	bge	.L586
	str	r5, [r4, #292]
.L587:
	movs	r3, #1
	str	r3, [r4, #300]
	b	.L572
.L581:
	add	r3, r4, #160
	mov	r1, #420
	ldr	r0, .L643+72
	bl	mpp_procfs_create_u32
	add	r3, r4, #216
	ldr	r2, [r4, #244]
	mov	r1, #420
	ldr	r0, .L643+76
	bl	mpp_procfs_create_u32
	add	r3, r4, #92
	ldr	r2, [r4, #244]
	mov	r1, #420
	ldr	r0, .L643+80
	bl	mpp_procfs_create_u32
	add	r3, r4, #120
	ldr	r2, [r4, #244]
	mov	r1, #420
	ldr	r0, .L643+84
	bl	mpp_procfs_create_u32
	ldr	r3, .L643+88
	mov	r1, #292
	ldr	r0, .L643+92
	str	r4, [sp]
	ldr	r2, [r4, #244]
	bl	proc_create_single_data
	add	r3, r4, #292
	ldr	r2, [r4, #244]
	mov	r1, #420
	ldr	r0, .L643+96
	bl	mpp_procfs_create_u32
	b	.L578
.L582:
	bl	of_device_is_available
	cmp	r0, #0
	beq	.L584
	mov	r0, r5
	bl	of_find_device_by_node
	movs	r1, #1
	bl	rk_dvbm_get_port
	str	r0, [r4, #264]
	cmp	r0, #0
	beq	.L583
	ldr	r3, .L643+100
	add	r1, sp, #32
	strd	r3, r4, [sp, #32]
	bl	rk_dvbm_set_cb
	b	.L583
.L644:
	.align	2
.L643:
	.word	__stack_chk_guard
	.word	.LC85
	.word	.LANCHOR0+212
	.word	.LC86
	.word	.LC101
	.word	mpp_rkvenc_worker
	.word	mpp_rkvenc_irq
	.word	.LC87
	.word	.LC88
	.word	.LANCHOR0+604
	.word	.LC89
	.word	.LC95
	.word	.LANCHOR0+623
	.word	.LC96
	.word	mpp_dev_debug
	.word	.LANCHOR0+644
	.word	.LC7
	.word	.LC97
	.word	.LC90
	.word	.LC50
	.word	.LC91
	.word	.LC92
	.word	rkvenc_show_session_info
	.word	.LC93
	.word	.LC94
	.word	rkvenc_callback
.L586:
	ldr	r2, [r4, #284]
	movs	r3, #1
	cmp	r2, #1
	bls	.L588
	str	r3, [r4, #292]
	mov	r0, r7
	ldr	r1, .L645
	bl	_dev_info
.L589:
	mov	r2, #3520
	movs	r1, #76
	mov	r0, r7
	bl	devm_kmalloc
	mov	r5, r0
	cmp	r0, #0
	beq	.L587
	add	fp, r0, #64
	movs	r3, #0
	ldr	r1, .L645+4
	str	r3, [r4, #296]
	mov	r0, fp
	ldr	r2, .L645+8
	bl	__mutex_init
	add	r3, r5, #48
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	add	r3, r5, #56
	str	r3, [r5, #56]
	str	r3, [r5, #60]
	ldr	r6, [r4, #152]
	str	r3, [sp, #20]
	ldr	r3, [r10]
	lsls	r1, r3, #26
	bpl	.L591
	movw	r2, #2374
	ldr	r1, .L645+12
	ldr	r0, .L645+16
	bl	printk
.L591:
	add	r3, r5, #12
	movs	r1, #0
	movs	r2, #36
	mov	r8, #128
	mov	r0, r3
	bl	memset
	movs	r1, #0
	mov	r3, r0
	mov	r0, r6
.L592:
	ldr	r2, [r6, #24]
	cmp	r1, r2
	bcc	.L594
	ldr	r3, [r4, #284]
	str	r3, [r5, #4]
	movs	r3, #0
.L642:
	str	r3, [sp, #24]
	ldr	r2, [sp, #24]
	ldr	r3, [r5, #4]
	cmp	r3, r2
	bhi	.L598
	mov	r0, fp
	bl	mutex_lock
	ldrd	r1, r3, [r5, #56]
	mov	r0, fp
	ldr	r3, [r3, #40]
	ldrb	r2, [r3, #32]	@ zero_extendqisi2
	orr	r2, r2, #1
	strb	r2, [r3, #32]
	ldr	r2, [r1, #32]
	str	r2, [r3, #32]
	bl	mutex_unlock
	ldr	r3, [r10]
	lsls	r2, r3, #26
	bpl	.L599
	movw	r2, #2426
	ldr	r1, .L645+12
	ldr	r0, .L645+20
	bl	printk
.L599:
	ldr	r3, [r10]
	str	r5, [r4, #288]
	lsls	r3, r3, #26
	str	r4, [r5]
	bpl	.L587
	movw	r2, #2476
	ldr	r1, .L645+24
	ldr	r0, .L645+20
	bl	printk
	b	.L587
.L588:
	str	r3, [r4, #284]
	movs	r3, #2
	str	r3, [r4, #292]
	b	.L589
.L594:
	ldr	r2, [r0, #44]
	cbz	r2, .L593
	str	r8, [r3, r1, lsl #2]
	ldr	r2, [r0, #44]
	adds	r2, r2, #127
	bic	r2, r2, #127
	add	r8, r8, r2
.L593:
	adds	r1, r1, #1
	adds	r0, r0, #20
	b	.L592
.L598:
	mov	r1, r8
	ldr	r0, [r4]
	bl	mpp_dma_alloc
	mov	r6, r0
	cbnz	r0, .L596
	ldr	r0, [r4]
	ldr	r1, .L645+28
	bl	_dev_err
	mov	r0, r5
	bl	rkvenc_link_remove.constprop.3
	b	.L587
.L596:
	ldr	r3, [r0, #40]
	ldr	r2, [r0, #32]
	ldr	r1, [r5, #16]
	add	r2, r2, r1
	str	r2, [r3, #4]
	ldr	r2, [r0, #32]
	ldr	r1, [r5, #20]
	add	r2, r2, r1
	str	r2, [r3, #8]
	ldr	r2, [r0, #32]
	ldr	r1, [r5, #24]
	add	r2, r2, r1
	str	r2, [r3, #12]
	ldr	r2, [r0, #32]
	ldr	r1, [r5, #28]
	add	r2, r2, r1
	str	r2, [r3, #16]
	ldr	r2, [r0, #32]
	ldr	r1, [r5, #32]
	add	r2, r2, r1
	str	r2, [r3, #20]
	ldr	r2, [r0, #32]
	ldr	r1, [r5, #36]
	add	r2, r2, r1
	str	r2, [r3, #24]
	ldr	r2, [r0, #32]
	ldr	r1, [r5, #40]
	add	r2, r2, r1
	str	r2, [r3, #28]
	str	r0, [r6]
	str	r0, [r6, #4]
	mov	r0, fp
	bl	mutex_lock
	ldr	r3, [r5, #56]
	ldr	r2, [sp, #20]
	cmp	r2, r3
	beq	.L597
	ldr	r3, [r5, #60]
	ldr	r3, [r3, #40]
	ldrb	r2, [r3, #32]	@ zero_extendqisi2
	orr	r2, r2, #1
	strb	r2, [r3, #32]
	ldr	r2, [r6, #32]
	str	r2, [r3, #32]
.L597:
	ldr	r3, [r5, #60]
	mov	r0, fp
	ldr	r2, [sp, #20]
	str	r6, [r5, #60]
	strd	r2, r3, [r6]
	str	r6, [r3]
	bl	mutex_unlock
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	b	.L642
.L602:
	mvn	r9, #11
	b	.L572
.L601:
	add	sp, sp, #84
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L646:
	.align	2
.L645:
	.word	.LC98
	.word	.LC99
	.word	.LANCHOR2
	.word	.LANCHOR0+661
	.word	.LC7
	.word	.LC8
	.word	.LANCHOR0+644
	.word	.LC100
	.fnend
	.size	rkvenc_probe, .-rkvenc_probe
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_alloc_task, %function
rkvenc_alloc_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r6, .L702
	mov	r7, r0
	str	r1, [sp, #8]
	ldr	r8, [r0, #12]
	ldr	r3, [r6]
	lsls	r4, r3, #26
	bpl	.L648
	movw	r2, #901
	ldr	r1, .L702+4
	ldr	r0, .L702+8
	bl	printk
.L648:
	ldr	r4, [r7, #188]
	ldr	r3, [r4, #2200]
	cbz	r3, .L649
	ldr	r4, [r7, #192]
	ldr	r3, [r4, #2200]
	cmp	r3, #0
	bne	.L650
.L649:
	add	r2, r4, #108
	mov	r3, r4
	movs	r5, #0
.L651:
	str	r5, [r3, #2344]
	adds	r3, r3, #12
	cmp	r2, r3
	bne	.L651
	str	r5, [r4, #2940]
	mov	r1, r4
	str	r5, [r4, #2944]
	mov	r0, r7
	str	r5, [r4, #3268]
	str	r4, [sp, #4]
	bl	mpp_task_init
	ldr	r3, [r8, #4]
	ldr	r9, [r3, #4]
	movs	r3, #1
	str	r3, [r4, #2308]
	ldr	r3, [r6]
	str	r9, [r4, #2272]
	lsls	r0, r3, #26
	str	r5, [r4, #2312]
	str	r9, [r4, #2340]
	bpl	.L653
	mov	r2, #608
	ldr	r1, .L702+12
	ldr	r0, .L702+8
	bl	printk
.L653:
	ldr	r3, [sp, #8]
	mov	r8, #20
	add	r5, r3, #60
	movs	r3, #0
	str	r3, [sp]
.L654:
	ldr	r3, [sp, #8]
	ldr	r2, [sp]
	ldr	r3, [r3, #48]
	cmp	r2, r3
	bcc	.L669
	ldr	r3, [r6]
	lsls	r1, r3, #22
	bpl	.L670
	ldr	r2, [r4, #3268]
	ldr	r1, [r4, #2944]
	ldr	r0, .L702+16
	bl	printk
.L670:
	ldr	r3, [r6]
	lsls	r2, r3, #26
	bpl	.L671
	movw	r2, #654
	ldr	r1, .L702+12
	ldr	r0, .L702+8
	bl	printk
.L671:
	ldr	r3, [r4, #2348]
	movs	r1, #12
	ldr	r2, [r4, #2340]
	str	r3, [r4, #2280]
	ldr	r3, [r2, #228]
	mla	r1, r1, r3, r4
	ldr	r0, [r1, #2348]
	cmp	r0, #0
	beq	.L672
	ldr	r1, [r1, #2352]
	cmp	r1, #0
	beq	.L672
	movs	r1, #20
	mla	r1, r1, r3, r2
	ldr	r3, [r2, #232]
	ldr	r1, [r1, #28]
	subs	r3, r3, r1
	ldr	r1, [r2, #236]
	bic	r3, r3, #3
	ldr	r3, [r0, r3]
	mov	r0, r4
	lsrs	r3, r3, r1
	ldr	r1, [r2, #240]
	movs	r2, #1
	lsls	r2, r2, r1
	mov	r1, #784
	subs	r2, r2, #1
	ands	r3, r3, r2
	str	r3, [r4, #2336]
	ldr	r3, [r4, #2360]
	ldr	r3, [r3, #144]
	cmp	r3, #0
	itt	lt
	movlt	r3, #0
	strlt	r3, [r4, #2308]
	bl	rkvenc_get_class_reg
	ldr	r3, [r0]
	ubfx	r3, r3, #0, #11
	adds	r3, r3, #1
	lsls	r3, r3, #3
	str	r3, [r4, #2324]
	ldrh	r3, [r0, #2]
	adds	r3, r3, #1
	lsls	r3, r3, #3
	str	r3, [r4, #2328]
	movs	r3, #3
	str	r3, [r4, #2936]
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L647
	mov	r2, #948
	ldr	r1, .L702+4
	ldr	r0, .L702+20
	bl	printk
.L647:
	ldr	r0, [sp, #4]
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L650:
	movw	r2, #911
	ldr	r1, .L702+4
	ldr	r0, .L702+24
	bl	printk
.L701:
	movs	r3, #0
	str	r3, [sp, #4]
	b	.L647
.L669:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L656
	ldr	r3, [r5]
	movw	r2, #513
	cmp	r3, r2
	beq	.L674
	movw	r2, #514
	cmp	r3, r2
	beq	.L658
	cmp	r3, #512
	bne	.L656
	addw	r3, r4, #2344
	movs	r7, #0
	str	r3, [sp, #12]
	b	.L659
.L665:
	ldr	r2, [r4, #2340]
	ldr	r3, [r5, #12]
	mla	r2, r8, r7, r2
	ldr	r1, [r2, #32]
	cmp	r1, r3
	bcc	.L660
	ldr	r1, [r5, #8]
	ldr	r2, [r2, #28]
	add	r3, r3, r1
	subs	r3, r3, #4
	cmp	r2, r3
	bhi	.L660
	ldr	fp, [r4, #2944]
	mov	r2, r5
	mov	r1, r7
	mov	r0, r4
	mul	r10, r8, fp
	addw	r3, r10, #2948
	add	r10, r10, r4
	add	r3, r3, r4
	bl	rkvenc_update_req
	ldr	r2, [r4, #2340]
	ldr	r1, [r10, #2960]
	movs	r3, #0
	ldr	lr, [r2, #24]
.L661:
	cmp	lr, r3
	beq	.L663
	ldr	ip, [r2, #28]
	cmp	r1, ip
	bcc	.L662
	ldr	r0, [r2, #32]
	cmp	r1, r0
	bcs	.L662
	cmp	r1, ip
	bne	.L663
	mla	fp, r8, fp, r4
	movs	r2, #12
	mla	r3, r2, r3, r4
	ldr	r2, [fp, #2964]
	str	r2, [r3, #2348]
.L663:
	movs	r3, #12
	ldr	r1, [sp, #12]
	muls	r3, r7, r3
	movs	r2, #1
	str	r2, [r1, r3]
	ldr	r3, [r4, #2944]
	add	r3, r3, r2
	str	r3, [r4, #2944]
.L660:
	adds	r7, r7, #1
.L659:
	ldr	r3, [r9, #24]
	cmp	r7, r3
	bcc	.L665
.L656:
	ldr	r3, [sp]
	adds	r5, r5, #20
	adds	r3, r3, #1
	str	r3, [sp]
	b	.L654
.L662:
	adds	r3, r3, #1
	adds	r2, r2, #20
	b	.L661
.L668:
	ldr	r2, [r4, #2340]
	ldr	r3, [r5, #12]
	mla	r2, r8, r7, r2
	ldr	r1, [r2, #32]
	cmp	r1, r3
	bcc	.L667
	ldr	r1, [r5, #8]
	ldr	r2, [r2, #28]
	add	r3, r3, r1
	subs	r3, r3, #4
	cmp	r2, r3
	bhi	.L667
	ldr	r3, [r4, #3268]
	mov	r2, r5
	mov	r1, r7
	mov	r0, r4
	mla	r3, r8, r3, r4
	addw	r3, r3, #3272
	bl	rkvenc_update_req
	mul	r3, fp, r7
	movs	r2, #1
	str	r2, [r10, r3]
	ldr	r3, [r4, #3268]
	add	r3, r3, r2
	str	r3, [r4, #3268]
.L667:
	adds	r7, r7, #1
.L657:
	ldr	r3, [r9, #24]
	cmp	r7, r3
	bcc	.L668
	b	.L656
.L674:
	movs	r7, #0
	addw	r10, r4, #2344
	mov	fp, #12
	b	.L657
.L658:
	mov	r1, r5
	addw	r0, r4, #2452
	bl	mpp_extract_reg_offset_info
	b	.L656
.L672:
	mov	r0, r4
	bl	kfree
	b	.L701
.L703:
	.align	2
.L702:
	.word	mpp_dev_debug
	.word	.LANCHOR0+685
	.word	.LC7
	.word	.LANCHOR0+703
	.word	.LC103
	.word	.LC8
	.word	.LC102
	.fnend
	.size	rkvenc_alloc_task, .-rkvenc_alloc_task
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_task_timeout, %function
rkvenc_task_timeout:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	sub	r5, r0, #2208
	mov	r6, r0
	ldr	r8, [r5, #2308]
	ldr	r7, [r5]
	cbnz	r7, .L705
	ldr	r1, .L720
	mov	r3, r5
	ldr	r0, .L720+4
	movw	r2, #1965
.L719:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	printk
.L705:
	ldr	r4, [r7, #12]
	cbnz	r4, .L706
	mov	r3, r7
	movw	r2, #1971
	ldr	r1, .L720
	ldr	r0, .L720+8
	b	.L719
.L706:
	ldr	r0, [r4, #48]
	bl	disable_irq
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r6, #-8]
	orr	r1, r3, #8
	str	r1, [r6, #-8]
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	lsls	r3, r3, #28
	bpl	.L707
	movw	r2, #1980
	ldr	r1, .L720
	ldr	r0, .L720+12
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	printk
.L707:
	ldr	r3, [r5, #2200]
	movw	r2, #1985
	ldr	r1, .L720
	subs	r6, r6, #8
	ldr	r0, .L720+16
	str	r3, [sp]
	ldr	r3, [r5, #2276]
	bl	printk
	mov	r0, r4
	bl	rkvenc_dump_dbg
	mov	r1, r6
	movs	r0, #6
	bl	____atomic_set_bit
	mov	r0, r5
	bl	mpp_time_diff
	mov	r1, r6
	movs	r0, #7
	bl	____atomic_set_bit
	mov	r1, r5
	ldr	r0, [r4, #96]
	bl	mpp_taskqueue_pop_running
	ldr	r3, [r7, #240]
	cbz	r3, .L708
	cmp	r8, #0
	beq	.L708
	ldr	r0, [r7, #196]
	blx	r3
.L708:
	ldr	r3, [r4, #8]
	ldr	r3, [r3, #28]
	cbz	r3, .L709
	mov	r0, r4
	blx	r3
.L709:
	mov	r0, r4
	bl	mpp_taskqueue_trigger_work
	add	r0, r4, #132
	bl	up_read
	ldr	r0, [r4, #48]
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	enable_irq
.L721:
	.align	2
.L720:
	.word	.LANCHOR0+727
	.word	.LC104
	.word	.LC105
	.word	.LC106
	.word	.LC107
	.fnend
	.size	rkvenc_task_timeout, .-rkvenc_task_timeout
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_run, %function
rkvenc_run:
	.fnstart
	@ args = 0, pretend = 0, frame = 80
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #108
	sub	sp, sp, #108
	ldr	r3, [r1]
	mov	r4, r0
	ldr	r7, [r0, #152]
	mov	r5, r1
	ldr	r3, [r3, #184]
	str	r3, [sp, #32]
	ldr	r3, .L781
	ldr	r3, [r3]
	lsls	r0, r3, #26
	bpl	.L723
	movw	r2, #1139
	ldr	r1, .L781+4
	ldr	r0, .L781+8
	bl	printk
.L723:
	ldr	r8, [r4, #292]
	movs	r3, #0
	str	r3, [r4, #272]
	cmp	r8, #1
	beq	.L724
	cmp	r8, #0
	beq	.L725
	cmp	r8, #2
	beq	.L726
.L727:
	ldr	r3, .L781
	ldr	r0, [r3]
	ands	r0, r0, #32
	beq	.L722
	movw	r2, #1262
	ldr	r1, .L781+4
	ldr	r0, .L781+12
	bl	printk
	movs	r0, #0
	b	.L722
.L725:
	mov	fp, r5
	mov	r9, r8
	str	r8, [sp, #28]
.L728:
	ldr	r2, [r5, #2944]
	ldr	r3, [sp, #28]
	cmp	r2, r3
	bhi	.L739
	mov	r1, #784
	str	r5, [r4, #88]
	mov	r0, r4
	bl	mpp_read
	ldr	r3, .L781
	ldr	r3, [r3]
	lsls	r1, r3, #24
	mov	r6, r0
	bpl	.L740
	ldr	r3, [r5]
	movw	r1, #1148
	mov	r0, r4
	ldr	fp, [r5, #2276]
	ldr	r10, [r3, #4]
	ldr	r3, [r5, #2336]
	str	r3, [sp, #28]
	bl	mpp_read
	lsrs	r2, r6, #16
	ubfx	r6, r6, #0, #11
	adds	r2, r2, #1
	adds	r6, r6, #1
	ldr	r1, .L781+4
	lsls	r2, r2, #3
	lsls	r6, r6, #3
	ldr	r3, [sp, #28]
	lsrs	r0, r0, #31
	str	r2, [sp, #16]
	mov	r2, r10
	str	r0, [sp, #4]
	str	r3, [sp]
	mov	r3, fp
	strd	r9, r6, [sp, #8]
	ldr	r0, .L781+16
	bl	printk
.L740:
	movw	r1, #20484
	mov	r0, r4
	bl	mpp_read
	lsls	r2, r0, #21
	mov	r3, r0
	bpl	.L741
	mov	r2, #1200
	ldr	r1, .L781+4
	ldr	r0, .L781+20
	bl	printk
.L741:
	cmp	r9, #0
	beq	.L742
	movs	r0, #0
	str	r0, [r4, #268]
	ldr	r6, [r5, #2316]
	add	r2, sp, #64
	movs	r1, #19
	str	r6, [sp, #100]
	bl	rk_dvbm_ctrl
	ldr	r3, [sp, #68]
	cbz	r3, .L743
	ldr	r3, [sp, #80]
	cbnz	r3, .L744
.L743:
	ldr	r1, .L781+24
	ldr	r0, [r4]
	bl	_dev_err
.L744:
	mov	r1, #776
	mov	r0, r4
	bl	mpp_read_relaxed
	mov	r1, #776
	bic	r2, r0, #255
	mov	r0, r4
	orrs	r2, r2, r6
	bl	mpp_write_relaxed
	mov	r1, #624
	mov	r0, r4
	ldr	r2, [sp, #64]
	bl	mpp_write_relaxed
	mov	r1, #628
	mov	r0, r4
	ldr	r2, [sp, #76]
	bl	mpp_write_relaxed
	mov	r1, #632
	mov	r0, r4
	ldr	r2, [sp, #68]
	bl	mpp_write_relaxed
	mov	r1, #636
	mov	r0, r4
	ldr	r2, [sp, #80]
	bl	mpp_write_relaxed
	mov	r1, #640
	mov	r0, r4
	ldr	r2, [sp, #72]
	bl	mpp_write_relaxed
	mov	r1, #644
	mov	r0, r4
	ldr	r2, [sp, #84]
	bl	mpp_write_relaxed
	mov	r1, #648
	mov	r0, r4
	ldr	r2, [sp, #84]
	bl	mpp_write_relaxed
	movw	r1, #1148
	mov	r0, r4
	bl	mpp_read_relaxed
	cmp	r0, #0
	bge	.L745
	ldr	r2, [sp, #68]
	mov	r1, #1040
	mov	r0, r4
	bl	mpp_write_relaxed
	movw	r1, #1044
	mov	r0, r4
	ldr	r2, [sp, #80]
	bl	mpp_write_relaxed
	mov	r1, #1048
	mov	r0, r4
	ldr	r2, [sp, #64]
	bl	mpp_write_relaxed
	movw	r1, #1052
	mov	r0, r4
	ldr	r2, [sp, #76]
	bl	mpp_write_relaxed
	mov	r1, #1056
	mov	r0, r4
	ldr	r2, [sp, #72]
	bl	mpp_write_relaxed
	movw	r1, #1060
	mov	r0, r4
	ldr	r2, [sp, #84]
	bl	mpp_write_relaxed
	mov	r1, #1064
	mov	r0, r4
	ldr	r2, [sp, #84]
	bl	mpp_write_relaxed
.L745:
	ldr	r3, [sp, #32]
	movs	r6, #1
	str	r6, [r3, #20]
	.syntax unified
@ 1207 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	mov	r2, r9
	ldr	r1, [r7, #280]
	mov	r0, r4
	bl	mpp_write_relaxed
	str	r6, [r4, #128]
.L742:
	ldr	r3, [sp, #32]
	movs	r6, #1
	str	r9, [r3, #16]
	str	r6, [r3, #200]
	.syntax unified
@ 1215 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	mvn	r3, #31
	str	r3, [r5, #2208]
	addw	r3, r5, #2212
	str	r3, [r5, #2212]
	str	r3, [r5, #2216]
	mov	r2, #2097152
	ldr	r3, .L781+28
	add	r0, r5, #2224
	ldr	r1, .L781+32
	str	r3, [r5, #2220]
	movs	r3, #0
	str	r3, [sp]
	bl	init_timer_key
	ldr	r1, .L781+36
	movs	r3, #20
	add	r2, r5, #2208
	mov	r0, r6
	ldr	r1, [r1]
	bl	queue_delayed_work_on
	addw	r1, r5, #2200
	mov	r0, r6
	bl	____atomic_set_bit
	cmp	r9, #0
	bne	.L746
	mov	r2, r8
	ldr	r1, [r7, #244]
	mov	r0, r4
	bl	mpp_write
.L746:
	movs	r3, #1
	str	r3, [r4, #260]
	b	.L727
.L739:
	ldr	r10, [r5, #2340]
	movs	r2, #20
	movs	r1, #0
	add	r0, sp, #44
	ldr	r6, [fp, #2960]
	bl	memset
	ldr	r3, [r10, #24]
	movs	r0, #0
.L729:
	cmp	r3, r0
	bne	.L732
	mvn	r0, #21
.L722:
	add	sp, sp, #108
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L732:
	ldr	r2, [r10, #28]
	cmp	r6, r2
	bcc	.L730
	ldr	r1, [r10, #32]
	cmp	r6, r1
	bcs	.L730
	movs	r1, #12
	subs	r6, r6, r2
	mla	r0, r1, r0, r5
	str	r2, [sp, #56]
	ldr	r2, [fp, #2956]
	lsrs	r6, r6, #2
	add	r3, r6, r2, lsr #2
	str	r3, [sp, #36]
	ldr	r1, [r0, #2352]
	ldr	r10, [r0, #2348]
	str	r1, [sp, #52]
	str	r10, [sp, #60]
.L731:
	ldr	r3, [sp, #36]
	cmp	r6, r3
	bcc	.L738
	ldr	r3, [sp, #28]
	add	fp, fp, #20
	adds	r3, r3, #1
	str	r3, [sp, #28]
	b	.L728
.L730:
	adds	r0, r0, #1
	add	r10, r10, #20
	b	.L729
.L738:
	ldr	r1, [sp, #56]
	ldr	r0, [r7, #244]
	add	r1, r1, r6, lsl #2
	ldr	r2, [r10, r6, lsl #2]
	cmp	r0, r1
	beq	.L750
	cmp	r1, #696
	bne	.L735
	str	r2, [r4, #304]
.L736:
	ldr	r0, [r7, #280]
	ldr	r2, [r10, r6, lsl #2]
	cmp	r0, r1
	beq	.L751
	ldr	r0, [r5, #2312]
	cbz	r0, .L737
	movw	r3, #1148
	cmp	r1, r3
	itt	eq
	biceq	r2, r2, #-2147483648
	streq	r2, [r10, r6, lsl #2]
.L737:
	ldr	r2, [r10, r6, lsl #2]
	mov	r0, r4
	bl	mpp_write_relaxed
.L734:
	adds	r6, r6, #1
	b	.L731
.L735:
	movw	r3, #1036
	cmp	r1, r3
	it	eq
	streq	r2, [r4, #308]
	b	.L736
.L750:
	mov	r8, r2
	b	.L734
.L751:
	mov	r9, r2
	b	.L734
.L782:
	.align	2
.L781:
	.word	mpp_dev_debug
	.word	.LANCHOR0+747
	.word	.LC7
	.word	.LC8
	.word	.LC108
	.word	.LC109
	.word	.LC110
	.word	rkvenc_task_timeout
	.word	delayed_work_timer_fn
	.word	system_wq
.L726:
	str	r3, [r4, #296]
	mov	r1, r5
	mov	r0, r4
	bl	rkvenc_run_start_link
	add	r3, r4, #296
	pld	[r3]
	.syntax unified
@ 223 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_add_return
1:	ldrex	r2, [r3]
	add	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	str	r2, [r5, #3596]
	str	r5, [r4, #88]
.L780:
	.syntax unified
@ 1246 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r3, [r4, #300]
	cmp	r3, #0
	beq	.L727
	movw	r2, #513
.L777:
	ldr	r1, [r7, #244]
	mov	r0, r4
	bl	mpp_write
	b	.L727
.L724:
	ldr	r1, [r7, #292]
	mov	r0, r4
	bl	mpp_read
	add	r6, r4, #296
	cbnz	r0, .L748
	str	r0, [r4, #296]
	mov	r1, r5
	mov	r0, r4
	bl	rkvenc_run_start_link
	pld	[r6]
	.syntax unified
@ 223 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_add_return
1:	ldrex	r3, [r6]
	add	r3, r3, #1
	strex	r2, r3, [r6]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	str	r3, [r5, #3596]
	b	.L780
.L748:
	pld	[r6]
	.syntax unified
@ 223 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/atomic.h" 1
	@ atomic_add_return
1:	ldrex	r3, [r6]
	add	r3, r3, #1
	strex	r2, r3, [r6]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	str	r3, [r5, #3596]
	.syntax unified
@ 1252 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r3, [r4, #300]
	cmp	r3, #0
	beq	.L727
	movw	r2, #769
	b	.L777
	.fnend
	.size	rkvenc_run, .-rkvenc_run
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_rkvenc_worker, %function
mpp_rkvenc_worker:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r0
	ldr	r3, .L799
	sub	r7, r0, #16
	ldr	r5, [r0, #80]
	ldr	r3, [r3]
	lsls	r3, r3, #26
	bpl	.L784
	movw	r2, #2011
	ldr	r1, .L799+4
	ldr	r0, .L799+8
	bl	printk
.L784:
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #56]
	add	r8, r5, #60
	cbz	r3, .L785
	ldr	r3, [r5, #60]
	cmp	r8, r3
	beq	.L785
	ldr	r0, [r4, #32]
	bl	disable_irq
	mov	r0, r7
	bl	mpp_dev_reset
	ldr	r0, [r4, #32]
	bl	enable_irq
.L785:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r0, r7
	bl	mpp_power_on
	.syntax unified
@ 29 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r9, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #60]
	cmp	r8, r3
	bne	.L787
	ldr	r3, [r4, #132]
	cbnz	r3, .L787
	mov	r0, r5
	bl	mpp_taskqueue_get_pending_task
	mov	r6, r0
	cbz	r0, .L787
	add	r0, r4, #116
	bl	down_read
	mov	r1, r8
	add	r0, r6, #20
	bl	list_move_tail
	mov	r0, r6
	bl	mpp_time_record
	addw	r1, r6, #2200
	movs	r0, #2
	bl	____atomic_set_bit
	mov	r1, r6
	mov	r0, r7
	bl	rkvenc_run
.L787:
	.syntax unified
@ 171 "/home/cww/ipc-sdk/otter3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r9	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #60]
	cmp	r8, r3
	bne	.L789
	mov	r0, r7
	bl	mpp_power_off
.L789:
	mov	r0, r5
	pop	{r3, r4, r5, r6, r7, r8, r9, lr}
	b	mpp_session_clean_detach
.L800:
	.align	2
.L799:
	.word	mpp_dev_debug
	.word	.LANCHOR0+758
	.word	.LC7
	.fnend
	.size	mpp_rkvenc_worker, .-mpp_rkvenc_worker
	.global	rockchip_rkvenc540c_driver
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.48984, %object
	.size	__func__.48984, 18
__func__.48984:
	.ascii	"rkvenc_update_req\000"
	.type	__func__.49798, %object
	.size	__func__.49798, 13
__func__.49798:
	.ascii	"rkvenc_reset\000"
	.type	__func__.49372, %object
	.size	__func__.49372, 11
__func__.49372:
	.ascii	"rkvenc_irq\000"
	.type	__func__.49539, %object
	.size	__func__.49539, 14
__func__.49539:
	.ascii	"rkvenc_result\000"
	.type	__func__.49207, %object
	.size	__func__.49207, 15
__func__.49207:
	.ascii	"rkvenc_prepare\000"
	.type	__func__.49079, %object
	.size	__func__.49079, 16
__func__.49079:
	.ascii	"rkvenc_dump_dbg\000"
	.type	__func__.49489, %object
	.size	__func__.49489, 14
__func__.49489:
	.ascii	"rkvenc_finish\000"
	.type	__func__.49787, %object
	.size	__func__.49787, 12
__func__.49787:
	.ascii	"rkvenc_init\000"
	.type	__func__.49389, %object
	.size	__func__.49389, 15
__func__.49389:
	.ascii	"mpp_rkvenc_irq\000"
	.type	__func__.49610, %object
	.size	__func__.49610, 15
__func__.49610:
	.ascii	"rkvenc_control\000"
	.type	__func__.49454, %object
	.size	__func__.49454, 11
__func__.49454:
	.ascii	"rkvenc_isr\000"
	.type	__func__.49443, %object
	.size	__func__.49443, 16
__func__.49443:
	.ascii	"rkvenc_link_isr\000"
	.type	__func__.49646, %object
	.size	__func__.49646, 20
__func__.49646:
	.ascii	"rkvenc_init_session\000"
	.type	__func__.49841, %object
	.size	__func__.49841, 19
__func__.49841:
	.ascii	"rkvenc_link_remove\000"
	.space	3
	.type	mpp_rkvenc_dt_match, %object
	.size	mpp_rkvenc_dt_match, 392
mpp_rkvenc_dt_match:
	.space	64
	.ascii	"rockchip,rkv-encoder-rv1106\000"
	.space	100
	.word	rkvenc_rv1106_data
	.space	196
	.type	__func__.49775, %object
	.size	__func__.49775, 19
__func__.49775:
	.ascii	"rkvenc_procfs_init\000"
	.type	__func__.49949, %object
	.size	__func__.49949, 21
__func__.49949:
	.ascii	"rkvenc_probe_default\000"
	.type	__func__.49928, %object
	.size	__func__.49928, 17
__func__.49928:
	.ascii	"rkvenc_link_init\000"
	.type	__func__.49892, %object
	.size	__func__.49892, 24
__func__.49892:
	.ascii	"rkvenc_link_alloc_table\000"
	.type	__func__.49157, %object
	.size	__func__.49157, 18
__func__.49157:
	.ascii	"rkvenc_alloc_task\000"
	.type	__func__.49032, %object
	.size	__func__.49032, 24
__func__.49032:
	.ascii	"rkvenc_extract_task_msg\000"
	.type	__func__.49677, %object
	.size	__func__.49677, 20
__func__.49677:
	.ascii	"rkvenc_task_timeout\000"
	.type	__func__.49303, %object
	.size	__func__.49303, 11
__func__.49303:
	.ascii	"rkvenc_run\000"
	.type	__func__.49690, %object
	.size	__func__.49690, 18
__func__.49690:
	.ascii	"mpp_rkvenc_worker\000"
	.type	rkvenc_pm_ops, %object
	.size	rkvenc_pm_ops, 92
rkvenc_pm_ops:
	.space	92
	.type	rkvenc_rv1106_data, %object
	.size	rkvenc_rv1106_data, 20
rkvenc_rv1106_data:
	.word	16
	.word	rkvenc_rv1106_hw_info
	.word	trans_rkvenc_rv1106
	.word	rkvenc_hw_ops
	.word	rkvenc_dev_ops_v2
	.type	trans_tbl_jpege_osd_rv1106, %object
	.size	trans_tbl_jpege_osd_rv1106, 32
trans_tbl_jpege_osd_rv1106:
	.short	81
	.short	82
	.short	90
	.short	91
	.short	99
	.short	100
	.short	108
	.short	109
	.short	117
	.short	118
	.short	126
	.short	127
	.short	135
	.short	136
	.short	144
	.short	145
	.type	trans_tbl_jpege_rv1106, %object
	.size	trans_tbl_jpege_rv1106, 8
trans_tbl_jpege_rv1106:
	.short	100
	.short	101
	.short	102
	.short	103
	.type	trans_tbl_h265e_rv1106_osd, %object
	.size	trans_tbl_h265e_rv1106_osd, 32
trans_tbl_h265e_rv1106_osd:
	.short	3
	.short	4
	.short	12
	.short	13
	.short	21
	.short	22
	.short	30
	.short	31
	.short	39
	.short	40
	.short	48
	.short	49
	.short	57
	.short	58
	.short	66
	.short	67
	.type	trans_tbl_h265e_rv1106, %object
	.size	trans_tbl_h265e_rv1106, 42
trans_tbl_h265e_rv1106:
	.short	7
	.short	8
	.short	9
	.short	10
	.short	11
	.short	12
	.short	13
	.short	14
	.short	15
	.short	16
	.short	17
	.short	18
	.short	19
	.short	20
	.short	21
	.short	22
	.short	23
	.short	100
	.short	101
	.short	102
	.short	103
	.type	trans_tbl_h264e_rv1106_osd, %object
	.size	trans_tbl_h264e_rv1106_osd, 32
trans_tbl_h264e_rv1106_osd:
	.short	3
	.short	4
	.short	12
	.short	13
	.short	21
	.short	22
	.short	30
	.short	31
	.short	39
	.short	40
	.short	48
	.short	49
	.short	57
	.short	58
	.short	66
	.short	67
	.type	trans_tbl_h264e_rv1106, %object
	.size	trans_tbl_h264e_rv1106, 42
trans_tbl_h264e_rv1106:
	.short	7
	.short	8
	.short	9
	.short	10
	.short	11
	.short	12
	.short	13
	.short	14
	.short	15
	.short	16
	.short	17
	.short	18
	.short	19
	.short	20
	.short	21
	.short	22
	.short	23
	.short	100
	.short	101
	.short	102
	.short	103
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	rkvenc_rv1106_hw_info, %object
	.size	rkvenc_rv1106_hw_info, 300
rkvenc_rv1106_hw_info:
	.word	254
	.word	0
	.space	4
	.word	160
	.word	253
	.word	4
	.word	9
	.word	0
	.word	288
	.space	12
	.word	624
	.word	1152
	.space	8
	.word	532
	.word	4096
	.word	4364
	.space	8
	.word	272
	.word	5888
	.word	6604
	.space	8
	.word	720
	.word	8192
	.word	8444
	.space	8
	.word	256
	.word	8672
	.word	11772
	.space	8
	.word	3104
	.word	12288
	.word	12908
	.space	8
	.word	624
	.word	16384
	.word	16972
	.space	8
	.word	592
	.word	20480
	.word	21332
	.space	12
	.word	2
	.word	1
	.word	0
	.word	6
	.word	3
	.word	1
	.word	768
	.word	0
	.word	2
	.word	16
	.word	20
	.word	32
	.word	36
	.word	40
	.word	44
	.word	56
	.word	10192
	.word	16416
	.word	96
	.word	320
	.word	112
	.word	16420
	.word	16424
	.type	rockchip_rkvenc540c_driver, %object
	.size	rockchip_rkvenc540c_driver, 104
rockchip_rkvenc540c_driver:
	.word	rkvenc_probe
	.word	rkvenc_remove
	.word	rkvenc_shutdown
	.space	8
	.word	.LC111
	.space	20
	.word	mpp_rkvenc_dt_match
	.space	36
	.word	rkvenc_pm_ops
	.space	8
	.space	8
	.type	rkvenc_dev_ops_v2, %object
	.size	rkvenc_dev_ops_v2, 68
rkvenc_dev_ops_v2:
	.space	16
	.word	rkvenc_alloc_task
	.word	rkvenc_prepare
	.word	rkvenc_run
	.word	rkvenc_irq
	.word	rkvenc_isr
	.word	rkvenc_finish
	.word	rkvenc_result
	.word	rkvenc_free_task
	.word	rkvenc_control
	.word	rkvenc_init_session
	.word	rkvenc_free_session
	.word	rkvenc_dump_session
	.word	rkvenc_dump_dbg
	.type	rkvenc_hw_ops, %object
	.size	rkvenc_hw_ops, 36
rkvenc_hw_ops:
	.word	rkvenc_init
	.space	4
	.word	rkvenc_clk_on
	.word	rkvenc_clk_off
	.space	4
	.word	rkvenc_set_freq
	.space	4
	.word	rkvenc_reset
	.space	4
	.type	trans_rkvenc_rv1106, %object
	.size	trans_rkvenc_rv1106, 48
trans_rkvenc_rv1106:
	.word	21
	.word	trans_tbl_h264e_rv1106
	.word	21
	.word	trans_tbl_h265e_rv1106
	.word	4
	.word	trans_tbl_jpege_rv1106
	.word	16
	.word	trans_tbl_h264e_rv1106_osd
	.word	16
	.word	trans_tbl_h265e_rv1106_osd
	.word	16
	.word	trans_tbl_jpege_osd_rv1106
	.bss
	.set	.LANCHOR2,. + 0
	.type	__key.49647, %object
	.size	__key.49647, 0
__key.49647:
	.type	__key.49932, %object
	.size	__key.49932, 0
__key.49932:
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016rkvenc_540c: read reg[%03d]: %04x: 0x%08x\012\000"
.LC1:
	.ascii	"\0016rkvenc_540c: read reg[%03d] %04x: 0x%08x\012\000"
.LC2:
	.ascii	"\0016rkvenc_540c: write reg[%03d]: %04x: 0x%08x\012"
	.ascii	"\000"
.LC3:
	.ascii	"\0013rkvenc_540c: %s:%d: warning over class, req of"
	.ascii	"f 0x%08x size %d\012\000"
.LC4:
	.ascii	"\0016rkvenc_540c: task %d jpeg bs overflow, buf[t:%"
	.ascii	"#x b:%#x w:%#x r:%#x]\012\000"
.LC5:
	.ascii	"\0016rkvenc_540c: task %d video bs overflow, buf[t:"
	.ascii	"%#x b:%#x w:%#x r:%#x]\012\000"
.LC6:
	.ascii	"\0013rkvenc_540c: clear dvbm info failed 0x%08x 0x%"
	.ascii	"08x\012\000"
.LC7:
	.ascii	"\0016rkvenc_540c: %s:%d: enter\012\000"
.LC8:
	.ascii	"\0016rkvenc_540c: %s:%d: leave\012\000"
.LC9:
	.ascii	"\012--------hw session infos\000"
.LC10:
	.ascii	"---------------------------------------------------"
	.ascii	"---\012\000"
.LC11:
	.ascii	"ID\000"
.LC12:
	.ascii	"%8s|\000"
.LC13:
	.ascii	"device\000"
.LC14:
	.ascii	"online\000"
.LC15:
	.ascii	"hw_running\000"
.LC16:
	.ascii	"%15s|%10s|%12s|%10s|%15s|%13s|%13s\000"
.LC17:
	.ascii	"enc_err_st\000"
.LC18:
	.ascii	"enc_err_cnt\000"
.LC19:
	.ascii	"wrap_src_mis\000"
.LC20:
	.ascii	"bs_ovfl\000"
.LC21:
	.ascii	"wrap_ovfl\000"
.LC22:
	.ascii	"\012\000"
.LC23:
	.ascii	"%8d|\000"
.LC24:
	.ascii	"null\000"
.LC25:
	.ascii	"%15d|%10d|%12d|%10d|%15d|%13d|%13x\000"
.LC26:
	.ascii	"\012--------bitstream buffer addr\000"
.LC27:
	.ascii	"bot\000"
.LC28:
	.ascii	"top\000"
.LC29:
	.ascii	"%10s|%10s|%10s|%10s|\012\000"
.LC30:
	.ascii	"rd\000"
.LC31:
	.ascii	"wr\000"
.LC32:
	.ascii	"%10x|%10x|%10x|%10x|\012\000"
.LC33:
	.ascii	"\0013rkvenc_540c: %s:%d: copy_to_user reg fail\012\000"
.LC34:
	.ascii	"\0013rkvenc_540c: %s:%d: xxxx task filled, mpp_stat"
	.ascii	"e=%lx\012\000"
.LC35:
	.ascii	"\0013rkvenc_540c: dvbm_en     -   %d\012\000"
.LC36:
	.ascii	"\0013rkvenc_540c: fmt         -   %d\012\000"
.LC37:
	.ascii	"\0013rkvenc_540c: jpg_en      -   %d\012\000"
.LC38:
	.ascii	"\0013rkvenc_540c: vsldy       -   0x%08x\012\000"
.LC39:
	.ascii	"\0013rkvenc_540c: st_wdg      -   0x%08x\012\000"
.LC40:
	.ascii	"\0013rkvenc_540c: st_ppl      -   0x%08x\012\000"
.LC41:
	.ascii	"\0013rkvenc_540c: st_vsp0     -   0x%08x\012\000"
.LC42:
	.ascii	"\0013rkvenc_540c: st_vsp1     -   0x%08x\012\000"
.LC43:
	.ascii	"\0013rkvenc_540c: vepu_cycle  -   0x%08x\012\000"
.LC44:
	.ascii	"\0016rkvenc_540c: === %s ===\012\000"
.LC45:
	.ascii	"\0016rkvenc_540c: reg[0x%0x] = 0x%08x\012\000"
.LC46:
	.ascii	"aclk_vcodec\000"
.LC47:
	.ascii	"\0013rkvenc_540c: %s:%d: failed on clk_get aclk_vco"
	.ascii	"dec\012\000"
.LC48:
	.ascii	"hclk_vcodec\000"
.LC49:
	.ascii	"\0013rkvenc_540c: %s:%d: failed on clk_get hclk_vco"
	.ascii	"dec\012\000"
.LC50:
	.ascii	"clk_core\000"
.LC51:
	.ascii	"\0013rkvenc_540c: %s:%d: failed on clk_get clk_core"
	.ascii	"\012\000"
.LC52:
	.ascii	"rockchip,default-max-load\000"
.LC53:
	.ascii	"video_a\000"
.LC54:
	.ascii	"\0013rkvenc_540c: %s:%d: No aclk reset resource def"
	.ascii	"ine\012\000"
.LC55:
	.ascii	"video_h\000"
.LC56:
	.ascii	"\0013rkvenc_540c: %s:%d: No hclk reset resource def"
	.ascii	"ine\012\000"
.LC57:
	.ascii	"video_core\000"
.LC58:
	.ascii	"\0013rkvenc_540c: %s:%d: No core reset resource def"
	.ascii	"ine\012\000"
.LC59:
	.ascii	"shutdown device\012\000"
.LC60:
	.ascii	"wait total running time out\012\000"
.LC61:
	.ascii	"shutdown success\012\000"
.LC62:
	.ascii	"found null task in irq\012\000"
.LC63:
	.ascii	"\0013rkvenc_540c: %s:%d: task %d has been handled s"
	.ascii	"tate %#lx\012\000"
.LC64:
	.ascii	"\0016rkvenc_540c: current frame has overflow\012\000"
.LC65:
	.ascii	"\0016rkvenc_540c: task %d fmt %d dvbm_en %d irq_sta"
	.ascii	"tus 0x%08x\012\000"
.LC66:
	.ascii	"\0013rkvenc_540c: overflow vepu[frm: %d line: %d %d"
	.ascii	"] isp[frm: %d line: %d]\012\000"
.LC67:
	.ascii	"\0013rkvenc_540c: line cnt disorder set max %d [%d "
	.ascii	"%d]->[%d %d]\012\000"
.LC68:
	.ascii	"\0013rkvenc_540c: overflow vepu[frm: %d line: %d %d"
	.ascii	"] isp[frm: %d line: %d] wrk 0x%08x\012\000"
.LC69:
	.ascii	"\0016rkvenc_540c: frame cnt %d line cnt %d\012\000"
.LC70:
	.ascii	"\0013rkvenc_540c: set frame info failed! [%d %d] ->"
	.ascii	" [%d %d]\012\000"
.LC71:
	.ascii	"\0013rkvenc_540c: %s:%d: session info null\012\000"
.LC72:
	.ascii	"\0016rkvenc_540c: codec info count %d\012\000"
.LC73:
	.ascii	"\0013rkvenc_540c: %s:%d: copy_from_user failed\012\000"
.LC74:
	.ascii	"\0013rkvenc_540c: %s:%d: codec info invalid, type %"
	.ascii	"d, flag %d\012\000"
.LC75:
	.ascii	"\0013rkvenc_540c: task %d disable_jpeg\012\000"
.LC76:
	.ascii	"\0013rkvenc_540c: %s:%d: unknown mpp ioctl cmd %x\012"
	.ascii	"\000"
.LC77:
	.ascii	"current frame has overflow\012\000"
.LC78:
	.ascii	"task %d fmt %d dvbm_en %d irq_status 0x%08x\012\000"
.LC79:
	.ascii	"\0016rkvenc_540c: irq_status: %08x\012\000"
.LC80:
	.ascii	"no current task\012\000"
.LC81:
	.ascii	"\0016rkvenc_540c: link_irq_status: %08x\012\000"
.LC82:
	.ascii	"\0013rkvenc_540c: %s:%d: session is null\012\000"
.LC83:
	.ascii	"&priv->rw_sem\000"
.LC84:
	.ascii	"remove device\012\000"
.LC85:
	.ascii	"probing start\012\000"
.LC86:
	.ascii	"dt match failed!\012\000"
.LC87:
	.ascii	"register interrupter runtime failed\012\000"
.LC88:
	.ascii	"%s%d\000"
.LC89:
	.ascii	"\0013rkvenc_540c: %s:%d: failed on open procfs\012\000"
.LC90:
	.ascii	"aclk\000"
.LC91:
	.ascii	"session_buffers\000"
.LC92:
	.ascii	"dump_regs\000"
.LC93:
	.ascii	"sessions-info\000"
.LC94:
	.ascii	"link_mode\000"
.LC95:
	.ascii	"dvbm\000"
.LC96:
	.ascii	"\0013rkvenc_540c: %s:%d: failed to get device node\012"
	.ascii	"\000"
.LC97:
	.ascii	"rockchip,task-capacity\000"
.LC98:
	.ascii	"%d task capacity link mode detected\012\000"
.LC99:
	.ascii	"&link->list_mutex\000"
.LC100:
	.ascii	"dma alloc failed\012\000"
.LC101:
	.ascii	"probing finish\012\000"
.LC102:
	.ascii	"\0013rkvenc_540c: %s:%d: can't found idle task\000"
.LC103:
	.ascii	"\0016rkvenc_540c: w_req_cnt=%d, r_req_cnt=%d\012\000"
.LC104:
	.ascii	"\0013rkvenc_540c: %s:%d: task %p, task->session is "
	.ascii	"null.\012\000"
.LC105:
	.ascii	"\0013rkvenc_540c: %s:%d: session %p, session->mpp i"
	.ascii	"s null.\012\000"
.LC106:
	.ascii	"\0013rkvenc_540c: %s:%d: task has been handled\012\000"
.LC107:
	.ascii	"\0013rkvenc_540c: %s:%d: task %d state %#lx process"
	.ascii	"ing time out!\012\000"
.LC108:
	.ascii	"\0016rkvenc_540c: %s session %d task %d fmt %d jpeg"
	.ascii	"_en %08x dvbm_en %d w/h [%d %d]\012\000"
.LC109:
	.ascii	"\0013rkvenc_540c: %s:%d: enc started status %08x\012"
	.ascii	"\000"
.LC110:
	.ascii	"the dvbm address do not ready!\012\000"
.LC111:
	.ascii	"mpp_rkvenc_540c\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
