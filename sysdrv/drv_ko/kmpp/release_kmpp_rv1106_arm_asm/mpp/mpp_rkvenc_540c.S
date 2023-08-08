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
	ldr	r3, [r0, #2316]
	movs	r2, #12
	ldr	r3, [r3, #24]
	mla	r3, r2, r3, r0
	movs	r2, #0
.L3:
	cmp	r0, r3
	bne	.L5
	movs	r0, #0
	bx	lr
.L5:
	ldr	r1, [r0, #2324]
	cbz	r1, .L4
	str	r2, [r0, #2324]
	str	r2, [r0, #2320]
.L4:
	adds	r0, r0, #12
	b	.L3
	.fnend
	.size	rkvenc_invalid_class_msg, .-rkvenc_invalid_class_msg
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
	ldr	r3, .L14
	ldr	r3, [r3]
	lsls	r3, r3, #18
	bpl	.L9
	mov	r3, r4
	lsrs	r1, r1, #2
	ldr	r0, .L14+4
	bl	printk
.L9:
	mov	r0, r4
	pop	{r4, pc}
.L15:
	.align	2
.L14:
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
@ 113 "./arch/arm/include/asm/io.h" 1
	ldr r4, [r3]
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L21
	ldr	r3, [r3]
	lsls	r3, r3, #18
	bpl	.L16
	mov	r3, r4
	lsrs	r1, r1, #2
	ldr	r0, .L21+4
	bl	printk
.L16:
	mov	r0, r4
	pop	{r4, pc}
.L22:
	.align	2
.L21:
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
	ldr	r3, .L28
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r3, [r3]
	mov	r4, r1
	mov	r5, r2
	lsls	r3, r3, #19
	bpl	.L24
	mov	r3, r2
	ldr	r0, .L28+4
	mov	r2, r1
	lsrs	r1, r1, #2
	bl	printk
.L24:
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
.L29:
	.align	2
.L28:
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
	ldr	r4, [r0, #2316]
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
	bcc	.L31
	cmp	r6, r0
	bcs	.L32
.L31:
	ldr	r2, [r2, #8]
	ldr	r1, .L33
	ldr	r0, .L33+4
	str	r2, [sp]
	movw	r2, #511
	bl	printk
.L32:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L34:
	.align	2
.L33:
	.word	.LANCHOR0
	.word	.LC3
	.fnend
	.size	rkvenc_update_req, .-rkvenc_update_req
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
	ldr	r6, [r0, #196]
	mov	r7, r0
	ldr	r9, .L52+36
	movs	r5, #0
	ldr	r10, .L52+20
	add	r8, r6, #24
	mov	r0, r6
	bl	down_read
	ldr	r1, .L52
	mov	r0, r4
	bl	seq_puts
	ldr	r1, .L52+4
	mov	r0, r4
	bl	seq_puts
	ldr	r2, .L52+8
	ldr	r1, .L52+12
	mov	r0, r4
	bl	seq_printf
	ldr	r2, .L52+16
	ldr	r1, .L52+20
	mov	r0, r4
	bl	seq_printf
.L37:
	lsls	r3, r5, #4
	ldr	r3, [r8, r3]
	cbz	r3, .L36
	ldr	r2, [r9, r5, lsl #2]
	mov	r1, r10
	mov	r0, r4
	bl	seq_printf
.L36:
	adds	r5, r5, #1
	cmp	r5, #11
	bne	.L37
	ldr	r1, .L52+24
	mov	r0, r4
	bl	seq_puts
	mov	r2, r7
	ldr	r1, .L52+28
	mov	r0, r4
	bl	seq_printf
	ldr	r2, [r7]
	ldr	r3, .L52+32
	mov	r0, r4
	ldr	r1, .L52+20
	add	r5, r6, #32
	ldr	r9, .L52+40
	add	r8, r6, #208
	ldr	r2, [r3, r2, lsl #2]
	bl	seq_printf
	ldr	r7, .L52+20
	ldr	r10, .L52+44
.L41:
	ldr	r3, [r5, #-8]
	cbz	r3, .L38
	cmp	r3, #1
	bne	.L39
	ldr	r2, [r5]
	mov	r1, r10
.L50:
	mov	r0, r4
	bl	seq_printf
.L38:
	adds	r5, r5, #16
	cmp	r5, r8
	bne	.L41
	ldr	r1, .L52+24
	mov	r0, r4
	bl	seq_puts
	mov	r0, r6
	bl	up_read
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L39:
	cmp	r3, #2
	mov	r1, r7
	ite	eq
	moveq	r2, r5
	movne	r2, r9
	b	.L50
.L53:
	.align	2
.L52:
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.word	mpp_device_name
	.word	enc_info_item_name
	.word	.LC13
	.word	.LC12
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
	cbz	r0, .L73
	ldr	r5, [r0, #200]
	cbz	r5, .L57
	mov	r0, r5
	bl	rkvenc_invalid_class_msg
	mov	r0, r5
	bl	kfree
.L57:
	ldr	r5, [r4, #204]
	cbz	r5, .L58
	mov	r0, r5
	bl	rkvenc_invalid_class_msg
	mov	r0, r5
	bl	kfree
.L58:
	ldr	r5, [r4, #196]
	ldr	r3, [r5, #20]
	cbz	r3, .L63
	ldr	r3, [r4, #12]
	ldr	r0, [r3, #244]
	bl	rk_dvbm_unlink
	movs	r3, #0
	str	r3, [r5, #20]
.L63:
	ldr	r3, [r5, #16]
	cbz	r3, .L59
	ldr	r3, [r4, #12]
	movs	r2, #0
	str	r2, [r3, #128]
.L59:
	ldr	r0, [r4, #196]
	cbz	r0, .L73
	bl	kfree
	movs	r3, #0
	str	r3, [r4, #196]
.L73:
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
	ldr	r6, .L96
	mov	r4, r1
	ldr	r3, [r6]
	mov	r10, r6
	tst	r3, #32
	beq	.L76
	movw	r2, #1612
	ldr	r1, .L96+4
	ldr	r0, .L96+8
	bl	printk
.L76:
	ldr	r3, [r7, #272]
	cmp	r3, #1
	bne	.L77
	ldr	r8, .L96+20
	mov	r5, r4
	mov	r9, #0
.L78:
	ldr	r3, [r4, #3244]
	cmp	r3, r9
	bhi	.L90
.L77:
	ldr	r0, [r10]
	ands	r0, r0, #32
	bne	.L79
.L75:
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L90:
	ldr	r3, [r4, #2316]
	movs	r2, #0
	ldr	r1, [r7, #268]
	ldr	ip, [r5, #3260]
	ldr	r0, [r3, #24]
.L81:
	cmp	r0, r2
	beq	.L91
	ldr	lr, [r3, #28]
	cmp	ip, lr
	bcc	.L82
	ldr	r6, [r3, #32]
	cmp	ip, r6
	bcs	.L82
	add	r2, r1, r2, lsl #2
	ldr	r3, [r4, #3568]
	ldr	r1, [r2, #12]
	ldr	r3, [r3, #40]
	sub	r1, r1, lr
	add	r1, r1, ip
	adds	r1, r3, r1
	bne	.L95
.L91:
	mvn	r0, #21
	b	.L75
.L82:
	adds	r2, r2, #1
	adds	r3, r3, #20
	b	.L81
.L95:
	ldr	r3, [r4]
	ldr	r0, [r5, #3264]
	ldr	r2, [r5, #3256]
	ldr	r3, [r3, #212]
	cbnz	r3, .L85
	cmp	r2, #0
	bge	.L86
.L88:
	movw	r2, #1630
	ldr	r1, .L96+4
	ldr	r0, .L96+12
	bl	printk
	mvn	r0, #4
	b	.L75
.L86:
	mov	r3, sp
	and	r3, r3, r8
	ldr	r3, [r3, #8]
	.syntax unified
@ 177 "./include/linux/uaccess.h" 1
	.syntax unified
adds ip, r0, r2; sbcscc ip, ip, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L87
	bl	arm_copy_to_user
	mov	r2, r0
.L87:
	cmp	r2, #0
	bne	.L88
.L89:
	add	r9, r9, #1
	adds	r5, r5, #20
	b	.L78
.L85:
	bl	memcpy
	b	.L89
.L79:
	movw	r2, #1641
	ldr	r1, .L96+4
	ldr	r0, .L96+16
	bl	printk
	movs	r0, #0
	b	.L75
.L97:
	.align	2
.L96:
	.word	mpp_dev_debug
	.word	.LANCHOR0+18
	.word	.LC14
	.word	.LC15
	.word	.LC16
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
	ldr	r4, [r0], #-188
	subs	r4, r4, #188
.L99:
	ldr	r2, [r5, #104]
	add	r3, r0, #188
	add	r1, r2, #712
	cmp	r3, r1
	bne	.L101
	add	r0, r2, #700
	bl	mutex_unlock
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L101:
	ldr	r3, [r0]
	cmp	r3, #16
	bne	.L100
	ldr	r3, [r0, #196]
	cbz	r3, .L100
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #60]
	cbz	r3, .L100
	mov	r1, r6
	blx	r3
.L100:
	ldr	r3, [r4, #188]
	mov	r0, r4
	sub	r4, r3, #188
	b	.L99
	.fnend
	.size	rkvenc_show_session_info, .-rkvenc_show_session_info
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
	ldr	r3, .L113
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r3, [r3]
	mov	r4, r1
	mov	r5, r2
	lsls	r3, r3, #19
	bpl	.L109
	mov	r3, r2
	ldr	r0, .L113+4
	mov	r2, r1
	lsrs	r1, r1, #2
	bl	printk
.L109:
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
.L114:
	.align	2
.L113:
	.word	mpp_dev_debug
	.word	.LC2
	.fnend
	.size	mpp_write, .-mpp_write
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
	beq	.L115
	mov	r1, r5
	ldr	r0, .L117
	pop	{r3, r4, r5, lr}
	b	printk
.L115:
	pop	{r3, r4, r5, pc}
.L118:
	.align	2
.L117:
	.word	.LC17
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
	ldr	r6, .L128
	ldr	r5, [r0, #132]
	ldr	r2, [r6]
	lsls	r2, r2, #26
	bpl	.L120
	movw	r2, #1961
	ldr	r1, .L128+4
	ldr	r0, .L128+8
	bl	printk
.L120:
	movw	r2, #1023
	ldr	r1, [r5, #256]
	mov	r0, r4
	bl	mpp_write
	movs	r2, #1
	ldr	r1, [r5, #248]
	mov	r0, r4
	bl	mpp_write
	ldr	r3, .L128+12
	ldr	r0, .L128+16
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
	bpl	.L121
	movw	r2, #1987
	ldr	r1, .L128+4
	ldr	r0, .L128+20
	bl	printk
.L121:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L129:
	.align	2
.L128:
	.word	mpp_dev_debug
	.word	.LANCHOR0+32
	.word	.LC14
	.word	arm_delay_ops
	.word	3221225
	.word	.LC16
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
	ldr	r6, .L151
	ldr	r5, [r0, #132]
	ldr	r2, [r6]
	lsls	r3, r2, #26
	bpl	.L131
	movw	r2, #1294
	ldr	r1, .L151+4
	ldr	r0, .L151+8
	bl	printk
.L131:
	ldr	r1, [r5, #264]
	mov	r0, r4
	bl	mpp_read
	str	r0, [r4, #52]
	cbz	r0, .L130
	mov	r2, #256
	ldr	r1, [r5, #256]
	mov	r0, r4
	bl	mpp_write
	ldr	r2, [r4, #52]
	ldr	r1, [r5, #260]
	mov	r0, r4
	bl	mpp_write
	movs	r2, #0
	ldr	r1, [r5, #264]
	mov	r0, r4
	bl	mpp_write
	ldr	r3, [r4, #52]
	cmp	r3, #32768
	bne	.L133
.L149:
	movs	r0, #1
.L130:
	pop	{r4, r5, r6, pc}
.L133:
	lsls	r0, r3, #31
	bpl	.L134
.L143:
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	ubfx	r0, r0, #0, #14
	str	r0, [r4, #252]
	mov	r0, r4
	bl	rkvenc_clear_dvbm_info
	ldr	r3, [r6]
	lsls	r1, r3, #26
	bmi	.L135
.L150:
	movs	r0, #2
	b	.L130
.L134:
	ands	r3, r3, #8192
	beq	.L136
	movw	r1, #1036
	mov	r0, r4
	bl	mpp_read
	mov	r1, #1024
	mov	r5, r0
	mov	r0, r4
	bl	mpp_read
	ldr	r3, [r4, #288]
	cmp	r5, r3
	itt	eq
	addeq	r3, r5, #1
	streq	r3, [r4, #288]
	ldr	r3, [r4, #288]
	cmp	r0, r3
	bhi	.L138
	movw	r1, #1028
	mov	r0, r4
	bl	mpp_read
	str	r0, [r4, #288]
.L138:
	ldr	r2, [r4, #288]
	movw	r1, #1036
	mov	r0, r4
	bl	mpp_write
	add	r2, r5, #12
	mov	r1, #1032
	mov	r0, r4
	bl	mpp_write
	ldr	r3, [r4, #52]
	ldr	r0, .L151+12
	str	r3, [r4, #56]
	bl	printk
	movs	r3, #1
.L136:
	ldr	r2, [r4, #52]
	lsls	r2, r2, #27
	bpl	.L139
	mov	r1, #700
	mov	r0, r4
	bl	mpp_read
	mov	r1, #688
	mov	r5, r0
	mov	r0, r4
	bl	mpp_read
	ldr	r3, [r4, #284]
	cmp	r5, r3
	itt	eq
	addeq	r3, r5, #1
	streq	r3, [r4, #284]
	ldr	r3, [r4, #284]
	cmp	r0, r3
	bhi	.L141
	mov	r1, #692
	mov	r0, r4
	bl	mpp_read
	str	r0, [r4, #284]
.L141:
	ldr	r2, [r4, #284]
	mov	r1, #696
	mov	r0, r4
	bl	mpp_write
	add	r2, r5, #12
	mov	r1, #700
	mov	r0, r4
	bl	mpp_write
	ldr	r3, [r4, #52]
	ldr	r0, .L151+16
	str	r3, [r4, #56]
	bl	printk
	b	.L149
.L139:
	cmp	r3, #0
	bne	.L149
	b	.L143
.L135:
	movw	r2, #1309
	ldr	r1, .L151+4
	ldr	r0, .L151+20
	bl	printk
	b	.L150
.L152:
	.align	2
.L151:
	.word	mpp_dev_debug
	.word	.LANCHOR0+45
	.word	.LC14
	.word	.LC18
	.word	.LC19
	.word	.LC16
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
	ldr	r5, [r0, #132]
	mov	r8, r1
	ldr	r9, [r1, #2324]
	ldr	r4, [r5, #28]
	ldr	r3, [r5, #32]
	lsrs	r4, r4, #2
	lsrs	r7, r3, #2
.L154:
	cmp	r4, r7
	bls	.L156
	ldr	r1, [r5, #252]
	movw	r2, #65535
	mov	r0, r6
	bl	mpp_write
	ldr	r3, [r8, #3568]
	ldr	r1, [r5, #288]
	mov	r0, r6
	ldr	r2, [r3, #32]
	bl	mpp_write
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L156:
	ldr	r3, [r5, #20]
	cmp	r3, r4
	beq	.L155
	ldr	r2, [r9, r4, lsl #2]
	lsls	r1, r4, #2
	mov	r0, r6
	bl	mpp_write
.L155:
	adds	r4, r4, #1
	b	.L154
	.fnend
	.size	rkvenc_run_start_link, .-rkvenc_run_start_link
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_link_fill_table, %function
rkvenc_link_fill_table:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r7, r2
	ldr	r4, [r2, #40]
	mov	r8, r0
	ldr	r3, [r1]
	mov	r5, r1
	ldr	fp, [r1, #2316]
	mov	r10, r1
	mov	r9, #0
	str	r3, [sp]
	ldrb	r3, [r4]	@ zero_extendqisi2
	mov	r6, fp
	orr	r3, r3, #4
	strb	r3, [r4]
	ldrb	r3, [r4, #4]	@ zero_extendqisi2
	ldr	r2, [r1, #2332]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #4]
	ldrb	r3, [r4, #8]	@ zero_extendqisi2
	ldr	r2, [r1, #2344]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #8]
	ldrb	r3, [r4, #12]	@ zero_extendqisi2
	ldr	r2, [r1, #2356]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #12]
	ldrb	r3, [r4, #16]	@ zero_extendqisi2
	ldr	r2, [r1, #2368]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #16]
	ldrb	r3, [r4, #20]	@ zero_extendqisi2
	ldr	r2, [r1, #2380]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #20]
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	ldr	r2, [r1, #2392]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #24]
	ldrb	r3, [r4, #28]	@ zero_extendqisi2
	ldr	r2, [r1, #2404]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #28]
	ldrb	r3, [r4, #32]	@ zero_extendqisi2
	orr	r3, r3, #1
	strb	r3, [r4, #32]
	add	r3, r0, #12
	str	r3, [sp, #4]
.L158:
	ldr	r2, [fp, #24]
	cmp	r2, r9
	bhi	.L162
	ldr	r0, [r8, #40]
	movs	r1, #0
	ldr	r2, [fp, #184]
	bic	r0, r0, #3
	add	r0, r0, r4
	bl	memset
	ldr	r3, [r5, #2324]
	ldr	r3, [r3, #96]
	cbz	r3, .L163
	ldr	r2, [r8]
	ldr	r1, [sp]
	str	r3, [r1, #16]
	ldr	r0, [r2, #244]
	bl	rk_dvbm_link
.L163:
	ldr	r0, [r8]
	movs	r3, #2
	ldrd	r1, r2, [r7, #32]
	ldr	r0, [r0]
	bl	dma_sync_single_for_device
	ldr	r0, [r8]
	movs	r3, #2
	ldrd	r1, r2, [r7, #32]
	ldr	r0, [r0]
	bl	dma_sync_single_for_cpu
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L162:
	ldr	r2, [r6, #44]
	cbz	r2, .L159
	ldr	r2, [r10, #2320]
	cbz	r2, .L159
	ldr	r3, [sp, #4]
	ldr	r2, [r6, #40]
	ldr	r1, [r6, #36]
	ldr	r0, [r3, r9, lsl #2]
	ldr	r3, [r6, #28]
	cmp	r1, #0
	it	eq
	moveq	r1, r3
	cbnz	r2, .L161
	ldr	r2, [r6, #32]
.L161:
	rsb	ip, r1, #4
	subs	r1, r1, r3
	ldr	r3, [r10, #2324]
	bic	r0, r0, #3
	bic	r1, r1, #3
	add	r2, r2, ip
	add	r1, r1, r3
	add	r0, r0, r4
	bl	memcpy
.L159:
	add	r9, r9, #1
	adds	r6, r6, #20
	add	r10, r10, #12
	b	.L158
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
	ldr	r7, .L208
	mov	r5, r1
	ldr	r4, [r0, #96]
	ldr	r3, [r7]
	lsls	r2, r3, #26
	bpl	.L175
	movw	r2, #1014
	ldr	r1, .L208+4
	ldr	r0, .L208+8
	bl	printk
.L175:
	ldr	r3, [r6, #72]
	cmp	r3, #0
	ble	.L176
.L205:
	movs	r4, #0
.L177:
	ldr	r3, [r7]
	lsls	r3, r3, #26
	bpl	.L174
	movw	r2, #1082
	ldr	r1, .L208+4
	ldr	r0, .L208+12
	bl	printk
.L174:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L176:
	ldr	r3, [r5]
	ldr	r3, [r3, #48]
	cmp	r3, #0
	bgt	.L205
	ldr	r3, [r6, #272]
	cmp	r3, #1
	beq	.L179
	cbz	r3, .L180
	cmp	r3, #2
	bne	.L205
	ldr	r8, [r6, #268]
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r4, #84]!
	cmp	r2, r4
	ite	eq
	moveq	r4, r5
	movne	r4, #0
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r4, #0
	beq	.L205
	add	r9, r8, #64
	mov	r0, r9
	bl	mutex_lock
	mov	r3, r8
	ldr	r6, [r3, #56]!
	cmp	r6, r3
	beq	.L186
	cbz	r6, .L186
	mov	r2, r6
	mov	r1, r5
	mov	r0, r8
	bl	rkvenc_link_fill_table
	add	r1, r8, #48
	mov	r0, r6
	bl	list_move_tail
	str	r6, [r5, #3568]
.L207:
	mov	r4, r5
.L186:
	mov	r0, r9
	bl	mutex_unlock
	b	.L177
.L180:
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r4, #84]!
	cmp	r2, r4
	it	ne
	movne	r5, #0
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L206:
	mov	r4, r5
	b	.L177
.L179:
	ldr	r8, [r6, #268]
	ldr	r6, [r5, #2200]
	ubfx	r6, r6, #12, #1
	cbz	r6, .L185
	ldr	r3, [r5, #2200]
	movw	r2, #1059
	ldr	r1, .L208+4
	ldr	r0, .L208+16
	bl	printk
	b	.L206
.L185:
	add	r9, r8, #64
	mov	r0, r9
	bl	mutex_lock
	mov	r3, r8
	ldr	r4, [r3, #56]!
	cmp	r4, r3
	beq	.L189
	cmp	r4, #0
	beq	.L186
	mov	r2, r4
	mov	r1, r5
	mov	r0, r8
	bl	rkvenc_link_fill_table
	add	r1, r8, #48
	mov	r0, r4
	bl	list_move_tail
	str	r4, [r5, #3568]
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #2200]
	orr	r3, r3, #4096
	str	r3, [r5, #2200]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L207
.L189:
	mov	r4, r6
	b	.L186
.L209:
	.align	2
.L208:
	.word	mpp_dev_debug
	.word	.LANCHOR0+56
	.word	.LC14
	.word	.LC16
	.word	.LC20
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
	adds	r0, r0, #136
	ldr	r1, [r1, #2912]
	bl	mpp_clk_set_rate
	ldr	r1, [r5, #2912]
	add	r0, r4, #192
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
	cbnz	r0, .L211
	mov	r0, r5
	bl	clk_enable
	mov	r4, r0
	cbz	r0, .L211
	mov	r0, r5
	bl	clk_unprepare
.L211:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
	.fnend
	.size	clk_prepare_enable, .-clk_prepare_enable
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
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	add	r7, r0, #136
	ldr	r3, [r0, #104]
	mov	r4, r0
	ldr	r2, .L232
	mov	r1, r7
	adds	r3, r3, #172
	str	r3, [r0, #64]
	bl	mpp_get_clk_info
	cbz	r0, .L217
	movw	r2, #1927
	ldr	r1, .L232+4
	ldr	r0, .L232+8
	bl	printk
.L217:
	ldr	r2, .L232+12
	add	r1, r4, #164
	mov	r0, r4
	bl	mpp_get_clk_info
	cbz	r0, .L218
	movw	r2, #1930
	ldr	r1, .L232+4
	ldr	r0, .L232+16
	bl	printk
.L218:
	add	r6, r4, #192
	ldr	r2, .L232+20
	mov	r0, r4
	mov	r1, r6
	bl	mpp_get_clk_info
	cbz	r0, .L219
	movw	r2, #1933
	ldr	r1, .L232+4
	ldr	r0, .L232+24
	bl	printk
.L219:
	mov	r2, r4
	movs	r5, #0
	ldr	r0, [r2], #220
	movs	r3, #1
	str	r5, [sp]
	ldr	r1, .L232+28
	ldr	r0, [r0, #356]
	bl	of_property_read_variable_u32_array
	ldr	r2, .L232+32
	mov	r1, r5
	mov	r0, r7
	bl	mpp_set_clk_info_rate_hz
	ldr	r2, .L232+36
	mov	r1, r5
	mov	r0, r6
	bl	mpp_set_clk_info_rate_hz
	ldr	r2, .L232+40
	mov	r1, r5
	mov	r0, r4
	bl	mpp_reset_control_get
	str	r0, [r4, #228]
	cbnz	r0, .L220
	movw	r2, #1945
	ldr	r1, .L232+4
	ldr	r0, .L232+44
	bl	printk
.L220:
	ldr	r2, .L232+48
	movs	r1, #1
	mov	r0, r4
	bl	mpp_reset_control_get
	str	r0, [r4, #232]
	cbnz	r0, .L221
	movw	r2, #1948
	ldr	r1, .L232+4
	ldr	r0, .L232+52
	bl	printk
.L221:
	ldr	r2, .L232+56
	movs	r1, #4
	mov	r0, r4
	bl	mpp_reset_control_get
	str	r0, [r4, #236]
	cbnz	r0, .L222
	movw	r2, #1951
	ldr	r1, .L232+4
	ldr	r0, .L232+60
	bl	printk
.L222:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L233:
	.align	2
.L232:
	.word	.LC21
	.word	.LANCHOR0+71
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	300000000
	.word	600000000
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.fnend
	.size	rkvenc_init, .-rkvenc_init
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
	bne	.L235
	cbz	r0, .L236
	ldr	r6, [r0, #196]
	cbnz	r6, .L237
.L236:
	movw	r2, #1665
	ldr	r1, .L255
	ldr	r0, .L255+4
	bl	printk
	mvn	r0, #21
.L234:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L237:
	ldr	r4, [r1, #8]
	ldr	r3, .L255+8
	lsrs	r4, r4, #4
	cmp	r4, #11
	ldr	r3, [r3]
	it	ge
	movge	r4, #11
	lsls	r3, r3, #27
	bpl	.L239
	mov	r1, r4
	ldr	r0, .L255+12
	bl	printk
.L239:
	ldr	r9, .L255+24
	movs	r5, #0
	ldr	r8, .L255
	ldr	r10, .L255+28
.L240:
	cmp	r5, r4
	bne	.L245
.L254:
	movs	r0, #0
	b	.L234
.L245:
	ldr	r1, [r7, #16]
	mov	r3, sp
	and	r3, r3, r9
	add	r1, r1, r5, lsl #4
	ldr	r3, [r3, #8]
	.syntax unified
@ 157 "./include/linux/uaccess.h" 1
	.syntax unified
adds r2, r1, #16; sbcscc r2, r2, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #16
	cbnz	r3, .L241
	add	r0, sp, #8
	bl	arm_copy_from_user
	mov	r2, r0
	cbz	r0, .L242
.L241:
	rsb	r0, r2, #16
	add	r3, sp, #8
	movs	r1, #0
	add	r0, r0, r3
	bl	memset
	movw	r2, #1675
	mov	r1, r8
	mov	r0, r10
	bl	printk
.L243:
	adds	r5, r5, #1
	b	.L240
.L246:
	subs	r1, r2, #1
	cmp	r1, #1
	bhi	.L244
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
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
	b	.L243
.L235:
	movw	r2, #1690
	ldr	r1, .L255
	ldr	r0, .L255+16
	bl	printk
	b	.L254
.L242:
	ldrd	r3, r2, [sp, #8]
	subs	r1, r3, #1
	cmp	r1, #9
	bls	.L246
.L244:
	str	r2, [sp]
	mov	r1, r8
	movw	r2, #1685
	ldr	r0, .L255+20
	bl	printk
	b	.L243
.L256:
	.align	2
.L255:
	.word	.LANCHOR0+83
	.word	.LC34
	.word	mpp_dev_debug
	.word	.LC35
	.word	.LC38
	.word	.LC37
	.word	-8192
	.word	.LC36
	.fnend
	.size	rkvenc_control, .-rkvenc_control
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	atomic_add.constprop.13, %function
atomic_add.constprop.13:
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
	.size	atomic_add.constprop.13, .-atomic_add.constprop.13
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
	ldr	r1, .L269
	mov	r0, r4
	bl	_dev_info
	ldr	r0, [r5, #104]
	adds	r0, r0, #84
	bl	atomic_add.constprop.13
	bl	ktime_get
	ldr	r3, .L269+4
	adds	r6, r0, r3
	mov	r3, #0
	adc	r7, r1, r3
	bl	_cond_resched
.L264:
	ldr	r3, [r5, #80]
	cbz	r3, .L260
	bl	ktime_get
	cmp	r6, r0
	sbcs	r3, r7, r1
	bge	.L261
	ldr	r3, [r5, #80]
	cbz	r3, .L260
	ldr	r1, .L269+8
	mov	r0, r4
	bl	_dev_err
.L260:
	mov	r0, r4
	ldr	r1, .L269+12
	pop	{r3, r4, r5, r6, r7, lr}
	b	_dev_info
.L261:
	mov	r1, #1000
	movs	r0, #251
	bl	usleep_range
	b	.L264
.L270:
	.align	2
.L269:
	.word	.LC39
	.word	200000000
	.word	.LC40
	.word	.LC41
	.fnend
	.size	rkvenc_shutdown, .-rkvenc_shutdown
	.text
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
	cbnz	r0, .L272
	movw	r2, #1739
	ldr	r1, .L294
	ldr	r0, .L294+4
	bl	printk
	mvn	r0, #21
.L271:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L272:
	ldr	r5, .L294+8
	mov	r1, #3520
	ldr	r8, [r0, #12]
	ldr	r0, [r5, #32]
	bl	kmem_cache_alloc
	mov	r6, r0
	cbnz	r0, .L274
.L293:
	mvn	r0, #11
	b	.L271
.L274:
	ldr	r2, .L294+12
	add	r7, r4, #208
	ldr	r1, .L294+16
	bl	__init_rwsem
	str	r6, [r4, #196]
	add	r6, r4, #200
.L279:
	mov	r1, #3520
	ldr	r0, [r5, #48]
	bl	kmem_cache_alloc
	cbz	r0, .L275
	ldr	r3, [r8, #4]
	mov	ip, r0
	mov	lr, #0
	ldr	r9, [r3, #4]
	mov	r2, r9
	str	r9, [r0, #2316]
.L276:
	ldr	r3, [r9, #24]
	cmp	r3, lr
	bhi	.L278
	str	r0, [r6], #4
	cmp	r6, r7
	bne	.L279
	movs	r0, #0
	b	.L271
.L278:
	ldr	r3, [ip, #2324]
	cbnz	r3, .L277
	ldr	r3, [r2, #32]
	ldr	r1, [r2, #28]
	adds	r3, r3, #4
	subs	r1, r3, r1
	str	r1, [ip, #2328]
.L277:
	add	lr, lr, #1
	adds	r2, r2, #20
	add	ip, ip, #12
	b	.L276
.L275:
	ldr	r0, [r4, #196]
	cbz	r0, .L280
	bl	kfree
.L280:
	ldr	r5, [r4, #200]
	cbz	r5, .L281
	mov	r0, r5
	bl	rkvenc_invalid_class_msg
	mov	r0, r5
	bl	kfree
.L281:
	ldr	r4, [r4, #204]
	cmp	r4, #0
	beq	.L293
	mov	r0, r4
	bl	rkvenc_invalid_class_msg
	mov	r0, r4
	bl	kfree
	b	.L293
.L295:
	.align	2
.L294:
	.word	.LANCHOR0+98
	.word	.LC42
	.word	kmalloc_caches
	.word	.LANCHOR1
	.word	.LC43
	.fnend
	.size	rkvenc_init_session, .-rkvenc_init_session
	.section	.text.unlikely
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_link_remove.constprop.2, %function
rkvenc_link_remove.constprop.2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	ldr	r5, .L311
	ldr	r3, [r5]
	lsls	r2, r3, #26
	bpl	.L297
	movw	r2, #2067
	ldr	r1, .L311+4
	ldr	r0, .L311+8
	bl	printk
	cbz	r4, .L298
.L305:
	add	r8, r4, #64
	mov	r6, r4
	mov	r0, r8
	bl	mutex_lock
	ldr	r0, [r6, #48]!
	ldr	r7, [r0]
.L299:
	cmp	r0, r6
	bne	.L300
	mov	r6, r4
	ldr	r0, [r6, #56]!
	ldr	r7, [r0]
.L301:
	cmp	r0, r6
	bne	.L302
	mov	r0, r8
	bl	mutex_unlock
	movs	r3, #0
	str	r3, [r4, #4]
.L298:
	ldr	r3, [r5]
	lsls	r3, r3, #26
	bpl	.L310
	movw	r2, #2085
	ldr	r1, .L311+4
	ldr	r0, .L311+12
	bl	printk
.L310:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, pc}
.L300:
	ldrd	r2, r3, [r0]
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r0, [r0]
	str	r0, [r0, #4]
	bl	mpp_dma_free
	mov	r0, r7
	ldr	r7, [r7]
	b	.L299
.L302:
	ldrd	r2, r3, [r0]
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r0, [r0]
	str	r0, [r0, #4]
	bl	mpp_dma_free
	mov	r0, r7
	ldr	r7, [r7]
	b	.L301
.L297:
	cmp	r0, #0
	bne	.L305
	b	.L310
.L312:
	.align	2
.L311:
	.word	mpp_dev_debug
	.word	.LANCHOR0+118
	.word	.LC14
	.word	.LC16
	.fnend
	.size	rkvenc_link_remove.constprop.2, .-rkvenc_link_remove.constprop.2
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
	ldr	r1, .L322
	bl	_dev_info
	ldr	r0, [r4, #244]
	cbz	r0, .L314
	bl	rk_dvbm_put
	movs	r3, #0
	str	r3, [r4, #244]
.L314:
	mov	r0, r4
	bl	mpp_dev_remove
	ldr	r0, [r4, #224]
	cbz	r0, .L315
	bl	proc_remove
	movs	r3, #0
	str	r3, [r4, #224]
.L315:
	ldr	r0, [r4, #268]
	bl	rkvenc_link_remove.constprop.2
	movs	r0, #0
	pop	{r4, pc}
.L323:
	.align	2
.L322:
	.word	.LC44
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
	ldr	r0, [r0, #136]
	cbz	r0, .L325
	bl	clk_prepare_enable
.L325:
	ldr	r0, [r4, #164]
	cbz	r0, .L326
	bl	clk_prepare_enable
.L326:
	ldr	r0, [r4, #192]
	cbz	r0, .L327
	bl	clk_prepare_enable
.L327:
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
	ldr	r5, [r0, #136]
	mov	r0, r5
	bl	clk_disable
	mov	r0, r5
	bl	clk_unprepare
	ldr	r5, [r4, #164]
	mov	r0, r5
	bl	clk_disable
	mov	r0, r5
	bl	clk_unprepare
	ldr	r4, [r4, #192]
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
	ldr	r6, .L411
	.pad #84
	sub	sp, sp, #84
	ldr	r1, .L411+4
	mov	r5, r0
	mov	r0, r7
	ldr	r3, [r6]
	str	r3, [sp, #76]
	bl	_dev_info
	mov	r2, #3520
	mov	r1, #292
	mov	r0, r7
	bl	devm_kmalloc
	mov	r4, r0
	str	r6, [sp, #28]
	cmp	r0, #0
	beq	.L367
	ldr	r1, [r5, #372]
	str	r0, [r5, #80]
	cbz	r1, .L340
	ldr	r0, .L411+8
	bl	of_match_node
	cbz	r0, .L341
	ldr	r3, [r0, #192]
	str	r3, [r4, #4]
.L340:
	mov	r1, r5
	mov	r0, r4
	bl	mpp_dev_probe
	mov	r9, r0
	cbnz	r0, .L339
	ldr	r3, [r5, #60]
	cbnz	r3, .L342
	ldr	r3, [r5, #16]
.L342:
	strd	r3, r4, [sp, #4]
	mov	r0, r7
	movs	r3, #128
	ldr	r2, .L411+12
	str	r3, [sp]
	ldr	r3, .L411+16
	ldr	r1, [r4, #48]
	bl	devm_request_threaded_irq
	mov	r9, r0
	cbz	r0, .L343
	mov	r0, r7
	ldr	r1, .L411+20
	bl	_dev_err
	mov	r0, r4
	bl	mpp_dev_remove
	b	.L339
.L341:
	ldr	r1, .L411+24
	mov	r0, r7
	bl	_dev_err
	mvn	r9, #18
.L339:
	mov	r0, r7
	ldr	r1, .L411+28
	bl	_dev_info
	mov	r0, r9
	ldr	r3, [sp, #28]
	ldr	r2, [sp, #76]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L366
	bl	__stack_chk_fail
.L343:
	movs	r3, #40
	str	r3, [r4, #92]
	ldr	r3, [r4, #4]
	ldr	r3, [r3, #4]
	str	r3, [r4, #132]
	ldr	r3, [r4]
	cbz	r3, .L344
	ldr	r3, [r3, #356]
	cbz	r3, .L344
	ldr	r3, [r3]
	cbz	r3, .L344
	ldr	r2, [r4, #104]
	cbz	r2, .L344
	ldr	r2, [r2, #76]
	cbz	r2, .L344
	ldr	r2, [r4, #116]
	movs	r1, #31
	add	r0, sp, #44
	str	r2, [sp]
	ldr	r2, .L411+32
	bl	snprintf
	ldr	r3, [r4, #104]
	ldr	r1, [r3, #76]
	add	r0, sp, #44
	bl	proc_mkdir
	mov	r2, r0
	str	r0, [r4, #224]
	cbz	r0, .L345
	cmn	r0, #4096
	bls	.L346
.L345:
	movw	r2, #1878
	ldr	r1, .L411+36
	ldr	r0, .L411+40
	bl	printk
	movs	r3, #0
	str	r3, [r4, #224]
.L344:
	ldr	r1, [r4, #104]
	mov	r0, r4
	bl	mpp_dev_register_srv
	movs	r2, #0
	ldr	r1, .L411+44
	ldr	r0, [r5, #372]
	bl	of_parse_phandle
	mov	r6, r0
	cmp	r0, #0
	bne	.L347
.L349:
	movw	r2, #2257
	ldr	r1, .L411+48
	ldr	r0, .L411+52
	bl	printk
.L348:
	movs	r3, #0
	str	r3, [r4, #272]
	ldr	r3, .L411+56
	ldr	r6, [r5, #372]
	ldr	r2, [r3]
	mov	r10, r3
	tst	r2, #32
	beq	.L350
	movw	r2, #2170
	ldr	r1, .L411+60
	ldr	r0, .L411+64
	bl	printk
.L350:
	movs	r5, #0
	movs	r3, #1
	str	r5, [sp]
	add	r2, r4, #264
	ldr	r1, .L411+68
	mov	r0, r6
	bl	of_property_read_variable_u32_array
	cmp	r0, r5
	bge	.L351
	str	r5, [r4, #272]
.L352:
	movs	r3, #1
	str	r3, [r4, #280]
	b	.L339
.L346:
	add	r3, r4, #140
	mov	r1, #420
	ldr	r0, .L411+72
	bl	mpp_procfs_create_u32
	add	r3, r4, #196
	ldr	r2, [r4, #224]
	mov	r1, #420
	ldr	r0, .L411+76
	bl	mpp_procfs_create_u32
	add	r3, r4, #92
	ldr	r2, [r4, #224]
	mov	r1, #420
	ldr	r0, .L411+80
	bl	mpp_procfs_create_u32
	add	r3, r4, #120
	ldr	r2, [r4, #224]
	mov	r1, #420
	ldr	r0, .L411+84
	bl	mpp_procfs_create_u32
	ldr	r3, .L411+88
	mov	r1, #292
	ldr	r0, .L411+92
	str	r4, [sp]
	ldr	r2, [r4, #224]
	bl	proc_create_single_data
	add	r3, r4, #272
	ldr	r2, [r4, #224]
	mov	r1, #420
	ldr	r0, .L411+96
	bl	mpp_procfs_create_u32
	b	.L344
.L347:
	bl	of_device_is_available
	cmp	r0, #0
	beq	.L349
	mov	r0, r6
	bl	of_find_device_by_node
	movs	r1, #1
	bl	rk_dvbm_get_port
	str	r0, [r4, #244]
	cmp	r0, #0
	beq	.L348
	ldr	r3, .L411+100
	add	r1, sp, #32
	strd	r3, r4, [sp, #32]
	bl	rk_dvbm_set_cb
	b	.L348
.L351:
	ldr	r2, [r4, #264]
	movs	r3, #1
	cmp	r2, #1
	bls	.L353
	str	r3, [r4, #272]
	mov	r0, r7
	ldr	r1, .L411+104
	bl	_dev_info
.L354:
	mov	r2, #3520
	movs	r1, #76
	mov	r0, r7
	bl	devm_kmalloc
	mov	r5, r0
	cmp	r0, #0
	beq	.L352
	add	fp, r0, #64
	movs	r3, #0
	ldr	r1, .L411+108
	str	r3, [r4, #276]
	mov	r0, fp
	ldr	r2, .L411+112
	bl	__mutex_init
	add	r3, r5, #48
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	add	r3, r5, #56
	str	r3, [r5, #56]
	str	r3, [r5, #60]
	ldr	r6, [r4, #132]
	str	r3, [sp, #20]
	ldr	r3, [r10]
	lsls	r1, r3, #26
	bpl	.L356
	movw	r2, #2101
	ldr	r1, .L411+116
	ldr	r0, .L411+64
	bl	printk
.L356:
	add	r3, r5, #12
	movs	r1, #0
	movs	r2, #36
	mov	r8, #128
	mov	r0, r3
	bl	memset
	movs	r1, #0
	mov	r3, r0
	mov	r0, r6
.L357:
	ldr	r2, [r6, #24]
	cmp	r1, r2
	bcc	.L359
	ldr	r3, [r4, #264]
	str	r3, [r5, #4]
	movs	r3, #0
.L410:
	str	r3, [sp, #24]
	ldr	r2, [sp, #24]
	ldr	r3, [r5, #4]
	cmp	r2, r3
	bcc	.L363
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
	bpl	.L364
	movw	r2, #2153
	ldr	r1, .L411+116
	ldr	r0, .L411+120
	bl	printk
.L364:
	ldr	r3, [r10]
	str	r5, [r4, #268]
	lsls	r3, r3, #26
	str	r4, [r5]
	bpl	.L352
	movw	r2, #2203
	ldr	r1, .L411+60
	ldr	r0, .L411+120
	bl	printk
	b	.L352
.L412:
	.align	2
.L411:
	.word	__stack_chk_guard
	.word	.LC45
	.word	.LANCHOR0+140
	.word	mpp_dev_irq
	.word	mpp_dev_isr_sched
	.word	.LC47
	.word	.LC46
	.word	.LC61
	.word	.LC48
	.word	.LANCHOR0+532
	.word	.LC49
	.word	.LC55
	.word	.LANCHOR0+551
	.word	.LC56
	.word	mpp_dev_debug
	.word	.LANCHOR0+572
	.word	.LC14
	.word	.LC57
	.word	.LC50
	.word	.LC25
	.word	.LC51
	.word	.LC52
	.word	rkvenc_show_session_info
	.word	.LC53
	.word	.LC54
	.word	rkvenc_callback
	.word	.LC58
	.word	.LC59
	.word	.LANCHOR1
	.word	.LANCHOR0+589
	.word	.LC16
.L353:
	str	r3, [r4, #264]
	movs	r3, #2
	str	r3, [r4, #272]
	b	.L354
.L359:
	ldr	r2, [r0, #44]
	cbz	r2, .L358
	str	r8, [r3, r1, lsl #2]
	ldr	r2, [r0, #44]
	adds	r2, r2, #127
	bic	r2, r2, #127
	add	r8, r8, r2
.L358:
	adds	r1, r1, #1
	adds	r0, r0, #20
	b	.L357
.L363:
	mov	r1, r8
	ldr	r0, [r4]
	bl	mpp_dma_alloc
	mov	r6, r0
	cbnz	r0, .L361
	ldr	r0, [r4]
	ldr	r1, .L413
	bl	_dev_err
	mov	r0, r5
	bl	rkvenc_link_remove.constprop.2
	b	.L352
.L361:
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
	beq	.L362
	ldr	r3, [r5, #60]
	ldr	r3, [r3, #40]
	ldrb	r2, [r3, #32]	@ zero_extendqisi2
	orr	r2, r2, #1
	strb	r2, [r3, #32]
	ldr	r2, [r6, #32]
	str	r2, [r3, #32]
.L362:
	ldr	r3, [r5, #60]
	mov	r0, fp
	ldr	r2, [sp, #20]
	str	r6, [r5, #60]
	strd	r2, r3, [r6]
	str	r6, [r3]
	bl	mutex_unlock
	ldr	r3, [sp, #24]
	adds	r3, r3, #1
	b	.L410
.L367:
	mvn	r9, #11
	b	.L339
.L366:
	add	sp, sp, #84
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L414:
	.align	2
.L413:
	.word	.LC60
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
	ldr	r6, .L469
	mov	r5, r0
	str	r1, [sp, #8]
	ldr	r7, [r0, #12]
	ldr	r3, [r6]
	lsls	r4, r3, #26
	bpl	.L416
	movw	r2, #878
	ldr	r1, .L469+4
	ldr	r0, .L469+8
	bl	printk
.L416:
	ldr	r4, [r5, #200]
	ldr	r3, [r4, #2200]
	cbz	r3, .L417
	ldr	r4, [r5, #204]
.L417:
	cmp	r4, #0
	beq	.L441
	add	r1, r4, #108
	mov	r2, r4
	movs	r3, #0
.L419:
	str	r3, [r2, #2320]
	adds	r2, r2, #12
	cmp	r2, r1
	bne	.L419
	str	r3, [r4, #2916]
	mov	r1, r4
	str	r3, [r4, #2920]
	mov	r0, r5
	str	r3, [r4, #3244]
	str	r4, [sp, #4]
	bl	mpp_task_init
	ldr	r3, [r7, #4]
	ldr	r9, [r3, #4]
	movs	r3, #1
	str	r3, [r4, #2308]
	ldr	r3, [r6]
	str	r9, [r4, #2272]
	lsls	r0, r3, #26
	str	r9, [r4, #2316]
	bpl	.L420
	movw	r2, #589
	ldr	r1, .L469+12
	ldr	r0, .L469+8
	bl	printk
.L420:
	ldr	r3, [sp, #8]
	mov	r8, #20
	add	r5, r3, #60
	movs	r3, #0
	str	r3, [sp]
.L421:
	ldr	r3, [sp, #8]
	ldr	r2, [sp]
	ldr	r3, [r3, #48]
	cmp	r2, r3
	bcc	.L436
	ldr	r3, [r6]
	lsls	r1, r3, #22
	bpl	.L437
	ldr	r2, [r4, #3244]
	ldr	r1, [r4, #2920]
	ldr	r0, .L469+16
	bl	printk
.L437:
	ldr	r3, [r6]
	lsls	r2, r3, #26
	bpl	.L438
	movw	r2, #635
	ldr	r1, .L469+12
	ldr	r0, .L469+8
	bl	printk
.L438:
	ldr	r3, [r4, #2324]
	movs	r1, #12
	ldr	r2, [r4, #2316]
	str	r3, [r4, #2280]
	ldr	r3, [r2, #228]
	mla	r1, r1, r3, r4
	ldr	r0, [r1, #2324]
	cmp	r0, #0
	beq	.L439
	ldr	r1, [r1, #2328]
	cmp	r1, #0
	beq	.L439
	movs	r1, #20
	mla	r1, r1, r3, r2
	ldr	r3, [r2, #232]
	ldr	r1, [r1, #28]
	subs	r3, r3, r1
	ldr	r1, [r2, #236]
	bic	r3, r3, #3
	ldr	r3, [r0, r3]
	lsrs	r3, r3, r1
	ldr	r1, [r2, #240]
	movs	r2, #1
	lsls	r2, r2, r1
	subs	r2, r2, #1
	ands	r3, r3, r2
	str	r3, [r4, #2312]
	ldr	r3, [r4, #2336]
	ldr	r3, [r3, #144]
	cmp	r3, #0
	itt	lt
	movlt	r3, #0
	strlt	r3, [r4, #2308]
	movs	r3, #3
	str	r3, [r4, #2912]
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L415
	movw	r2, #918
	ldr	r1, .L469+4
	ldr	r0, .L469+20
	bl	printk
.L415:
	ldr	r0, [sp, #4]
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L436:
	ldr	r3, [r5, #8]
	cmp	r3, #0
	beq	.L423
	ldr	r3, [r5]
	movw	r2, #513
	cmp	r3, r2
	beq	.L442
	movw	r2, #514
	cmp	r3, r2
	beq	.L425
	cmp	r3, #512
	bne	.L423
	add	r3, r4, #2320
	movs	r7, #0
	str	r3, [sp, #12]
	b	.L426
.L432:
	ldr	r2, [r4, #2316]
	ldr	r3, [r5, #12]
	mla	r2, r8, r7, r2
	ldr	r1, [r2, #32]
	cmp	r1, r3
	bcc	.L427
	ldr	r1, [r5, #8]
	ldr	r2, [r2, #28]
	add	r3, r3, r1
	subs	r3, r3, #4
	cmp	r2, r3
	bhi	.L427
	ldr	fp, [r4, #2920]
	mov	r2, r5
	mov	r1, r7
	mov	r0, r4
	mul	r10, r8, fp
	addw	r3, r10, #2924
	add	r10, r10, r4
	add	r3, r3, r4
	bl	rkvenc_update_req
	ldr	r2, [r4, #2316]
	ldr	r1, [r10, #2936]
	movs	r3, #0
	ldr	lr, [r2, #24]
.L428:
	cmp	lr, r3
	beq	.L430
	ldr	ip, [r2, #28]
	cmp	r1, ip
	bcc	.L429
	ldr	r0, [r2, #32]
	cmp	r1, r0
	bcs	.L429
	cmp	r1, ip
	bne	.L430
	mla	fp, r8, fp, r4
	movs	r2, #12
	mla	r3, r2, r3, r4
	ldr	r2, [fp, #2940]
	str	r2, [r3, #2324]
.L430:
	movs	r3, #12
	ldr	r1, [sp, #12]
	muls	r3, r7, r3
	movs	r2, #1
	str	r2, [r1, r3]
	ldr	r3, [r4, #2920]
	add	r3, r3, r2
	str	r3, [r4, #2920]
.L427:
	adds	r7, r7, #1
.L426:
	ldr	r3, [r9, #24]
	cmp	r7, r3
	bcc	.L432
.L423:
	ldr	r3, [sp]
	adds	r5, r5, #20
	adds	r3, r3, #1
	str	r3, [sp]
	b	.L421
.L429:
	adds	r3, r3, #1
	adds	r2, r2, #20
	b	.L428
.L435:
	ldr	r2, [r4, #2316]
	ldr	r3, [r5, #12]
	mla	r2, r8, r7, r2
	ldr	r1, [r2, #32]
	cmp	r1, r3
	bcc	.L434
	ldr	r1, [r5, #8]
	ldr	r2, [r2, #28]
	add	r3, r3, r1
	subs	r3, r3, #4
	cmp	r2, r3
	bhi	.L434
	ldr	r3, [r4, #3244]
	mov	r2, r5
	mov	r1, r7
	mov	r0, r4
	mla	r3, r8, r3, r4
	add	r3, r3, #3248
	bl	rkvenc_update_req
	mul	r3, fp, r7
	movs	r2, #1
	str	r2, [r10, r3]
	ldr	r3, [r4, #3244]
	add	r3, r3, r2
	str	r3, [r4, #3244]
.L434:
	adds	r7, r7, #1
.L424:
	ldr	r3, [r9, #24]
	cmp	r7, r3
	bcc	.L435
	b	.L423
.L442:
	movs	r7, #0
	add	r10, r4, #2320
	mov	fp, #12
	b	.L424
.L425:
	mov	r1, r5
	addw	r0, r4, #2428
	bl	mpp_extract_reg_offset_info
	b	.L423
.L439:
	mov	r0, r4
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #4]
	b	.L415
.L441:
	str	r4, [sp, #4]
	b	.L415
.L470:
	.align	2
.L469:
	.word	mpp_dev_debug
	.word	.LANCHOR0+613
	.word	.LC14
	.word	.LANCHOR0+631
	.word	.LC62
	.word	.LC16
	.fnend
	.size	rkvenc_alloc_task, .-rkvenc_alloc_task
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
	ldr	r0, .L481
	bl	printk
	mov	r1, #768
	mov	r0, r4
	bl	mpp_read
	and	r1, r0, #3
	ldr	r0, .L481+4
	bl	printk
	movw	r1, #1148
	mov	r0, r4
	bl	mpp_read
	lsrs	r1, r0, #31
	ldr	r0, .L481+8
	bl	printk
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L481+12
	bl	printk
	mov	r1, #20480
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L481+16
	bl	printk
	movw	r1, #20484
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L481+20
	bl	printk
	movw	r1, #20488
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L481+24
	bl	printk
	movw	r1, #20492
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L481+28
	bl	printk
	mov	r1, #20992
	mov	r0, r4
	bl	mpp_read
	mov	r1, r0
	ldr	r0, .L481+32
	bl	printk
	ldr	r3, .L481+36
	ldr	r3, [r3]
	lsls	r3, r3, #21
	bpl	.L471
	ldr	r1, .L481+40
	mvn	r7, #1
	ldr	r0, .L481+44
	bl	printk
	ldr	r6, .L481+48
	ldr	r9, .L481+52
.L474:
	cmp	r7, #4
	bhi	.L473
.L478:
	adds	r7, r7, #1
	adds	r6, r6, #20
	cmp	r7, #7
	bne	.L474
	ldr	r1, .L481+40
	ldr	r0, .L481+44
	pop	{r3, r4, r5, r6, r7, r8, r9, lr}
	b	printk
.L473:
	ldrd	r5, r8, [r6, #28]
.L476:
	cmp	r8, r5
	bcc	.L478
	mov	r1, r5
	mov	r0, r4
	bl	mpp_read
	mov	r1, r5
	adds	r5, r5, #4
	mov	r2, r0
	mov	r0, r9
	bl	printk
	b	.L476
.L471:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L482:
	.align	2
.L481:
	.word	.LC63
	.word	.LC64
	.word	.LC65
	.word	.LC66
	.word	.LC67
	.word	.LC68
	.word	.LC69
	.word	.LC70
	.word	.LC71
	.word	mpp_dev_debug
	.word	.LANCHOR0+655
	.word	.LC72
	.word	.LANCHOR2
	.word	.LC73
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
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r5, r0
	ldr	r3, [r1]
	mov	r4, r1
	ldr	r6, .L515
	ldr	r7, [r3, #196]
	ldr	r3, [r6]
	str	r6, [sp]
	tst	r3, #32
	beq	.L484
	movw	r2, #1541
	ldr	r1, .L515+4
	ldr	r0, .L515+8
	bl	printk
.L484:
	ldr	r3, [r5, #272]
	cbz	r3, .L485
	cmp	r3, #2
	bne	.L486
.L485:
	ldr	r3, [r5, #52]
	lsls	r2, r3, #23
	bmi	.L487
	ldr	r3, [r5, #120]
	cbz	r3, .L488
.L487:
	mov	r0, r5
	bl	rkvenc_dump_dbg
.L488:
	mov	r9, r4
	mov	fp, #0
.L489:
	ldr	r3, [r4, #3244]
	cmp	r3, fp
	bhi	.L493
	ldr	r3, [r4, #2316]
	movs	r1, #0
	ldr	r2, [r3, #264]
	ldr	r0, [r3, #24]
.L494:
	cmp	r0, r1
	beq	.L497
	ldr	ip, [r3, #28]
	cmp	r2, ip
	bcc	.L495
	ldr	r6, [r3, #32]
	cmp	r2, r6
	bcs	.L495
	movs	r3, #12
	sub	r2, r2, ip
	mla	r1, r3, r1, r4
	ldr	r3, [r1, #2324]
	cmn	r3, r2
	bne	.L496
.L497:
	ldr	r3, [r7, #16]
	cbz	r3, .L486
	ldr	r0, [r5, #244]
	bl	rk_dvbm_unlink
.L486:
	movs	r3, #0
	str	r3, [r5, #240]
	ldr	r3, [sp]
	ldr	r3, [r3]
	lsls	r3, r3, #26
	bpl	.L499
	movw	r2, #1579
	ldr	r1, .L515+4
	ldr	r0, .L515+12
	bl	printk
.L499:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L493:
	ldr	r3, [r9, #3264]
	mov	r8, #0
.L490:
	ldr	r2, [r9, #3256]
	cmp	r8, r2, lsr #2
	bcc	.L492
	add	fp, fp, #1
	add	r9, r9, #20
	b	.L489
.L492:
	ldr	r2, [r9, #3260]
	mov	r0, r5
	str	r3, [sp, #4]
	add	r10, r2, r8, lsl #2
	mov	r1, r10
	bl	mpp_read_relaxed
	ldr	r3, [sp, #4]
	str	r0, [r3, r8, lsl #2]
	ldr	r2, [r4, #2316]
	ldr	r2, [r2, #264]
	cmp	r2, r10
	itt	eq
	ldreq	r2, [r4, #2916]
	streq	r2, [r3, r8, lsl #2]
	add	r8, r8, #1
	b	.L490
.L495:
	adds	r1, r1, #1
	adds	r3, r3, #20
	b	.L494
.L496:
	ldr	r1, [r4, #2916]
	str	r1, [r3, r2]
	b	.L497
.L516:
	.align	2
.L515:
	.word	mpp_dev_debug
	.word	.LANCHOR0+671
	.word	.LC14
	.word	.LC16
	.fnend
	.size	rkvenc_finish, .-rkvenc_finish
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_isr, %function
rkvenc_isr:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r6, .L587
	.pad #20
	sub	sp, sp, #20
	ldr	r3, [r6]
	lsls	r0, r3, #26
	bpl	.L518
	movw	r2, #1439
	ldr	r1, .L587+4
	ldr	r0, .L587+8
	bl	printk
.L518:
	ldr	r7, [r4, #272]
	cmp	r7, #1
	beq	.L519
	cbz	r7, .L520
	cmp	r7, #2
	beq	.L521
.L522:
	ldr	r3, [r6]
	lsls	r3, r3, #26
	bpl	.L524
	movw	r2, #1527
	ldr	r1, .L587+4
	ldr	r0, .L587+12
	bl	printk
	b	.L524
.L520:
	ldr	r5, [r4, #88]
	cmp	r5, #0
	beq	.L524
	ldr	r3, [r5]
	mov	r0, r5
	ldr	r8, [r3, #196]
	bl	mpp_time_diff
	str	r7, [r4, #88]
	ldr	r3, [r8, #16]
	cbz	r3, .L526
	ldr	r2, [r4, #252]
	movw	r3, #16383
	cmp	r2, r3
	bne	.L527
	movs	r3, #1
	ldr	r1, .L587+16
	str	r3, [r4, #248]
	ldr	r0, [r4]
	bl	_dev_err
.L527:
	ldr	r3, [r4, #248]
	cbz	r3, .L526
	ldr	r3, [r4, #52]
	orr	r3, r3, #64
	str	r3, [r4, #52]
	movs	r3, #0
	str	r3, [r4, #248]
.L526:
	ldrd	r3, r2, [r4, #52]
	orrs	r3, r3, r2
	str	r3, [r5, #2916]
	movs	r3, #0
	str	r3, [r4, #56]
	ldr	r3, [r6]
	lsls	r1, r3, #29
	bpl	.L529
	ldr	r3, [r5, #2916]
	ldr	r0, .L587+20
	str	r3, [sp]
	ldr	r3, [r8, #16]
	ldr	r2, [r5, #2312]
	ldr	r1, [r5, #2276]
	bl	printk
.L529:
	ldr	r3, [r4, #132]
	ldr	r2, [r3, #272]
	ldr	r3, [r4, #52]
	tst	r2, r3
	beq	.L531
	ldr	r3, [r5, #2916]
	mov	r7, r4
	ldr	r1, .L587+24
	str	r3, [sp, #4]
	ldr	r3, [r8, #16]
	str	r3, [sp]
	ldr	r0, [r7], #72
	ldr	r3, [r5, #2312]
	ldr	r2, [r5, #2276]
	bl	_dev_err
	mov	r0, r7
	bl	atomic_add.constprop.13
	ldr	r3, [r6]
	lsls	r2, r3, #21
	bpl	.L531
	ldr	r1, [r5, #2916]
	ldr	r0, .L587+28
	bl	printk
	mov	r0, r4
	bl	rkvenc_dump_dbg
.L531:
	mov	r1, r5
	ldr	r0, [r5]
	bl	mpp_task_finish
	b	.L522
.L521:
	ldr	r5, [r4, #88]
	cbnz	r5, .L533
	ldr	r1, .L587+32
	ldr	r0, [r4]
	bl	_dev_err
.L524:
	movs	r0, #1
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L533:
	mov	r0, r5
	bl	mpp_time_diff
	movs	r3, #0
	str	r3, [r4, #88]
	ldr	r3, [r6]
	ldr	r1, [r4, #52]
	lsls	r3, r3, #29
	str	r1, [r5, #2916]
	bpl	.L534
	ldr	r0, .L587+28
	bl	printk
.L534:
	ldr	r3, [r4, #132]
	ldr	r2, [r3, #272]
	ldr	r3, [r5, #2916]
	tst	r2, r3
	beq	.L536
	add	r0, r4, #72
	bl	atomic_add.constprop.13
	ldr	r3, [r6]
	lsls	r7, r3, #21
	bpl	.L536
	ldr	r1, [r5, #2916]
	ldr	r0, .L587+28
	bl	printk
.L536:
	mov	r1, r5
	ldr	r0, [r5]
	bl	mpp_task_finish
	ldr	r0, [r4, #268]
	adds	r0, r0, #64
	bl	mutex_lock
	ldr	r1, [r4, #268]
	ldr	r0, [r5, #3568]
	adds	r1, r1, #56
	bl	list_move_tail
	ldr	r0, [r4, #268]
	adds	r0, r0, #64
	bl	mutex_unlock
	b	.L522
.L519:
	ldr	r3, [r4, #52]
	lsls	r5, r3, #23
	bmi	.L538
	ldr	r3, [r4, #120]
	cbz	r3, .L539
.L538:
	mov	r0, r4
	bl	rkvenc_dump_dbg
.L539:
	ldr	r10, [r4, #132]
	movs	r3, #0
	ldr	r3, [r3]
	mov	r0, r4
	ldr	r9, [r4, #268]
	ldr	r1, [r10, #292]
	str	r3, [sp, #8]
	bl	mpp_read_relaxed
	ldr	r3, [r4, #96]
	ldr	r5, [r3, #84]
	ldr	r7, [r5], #-20
	subs	r7, r7, #20
	lsr	fp, r0, #24
.L540:
	ldr	r1, [r4, #96]
	add	r8, r5, #20
	add	r2, r1, #84
	cmp	r8, r2
	bne	.L546
.L541:
	ldr	r3, [r1, #84]
	cmp	r2, r3
	bne	.L548
	ldr	r3, [sp, #8]
	ldr	r3, [r3, #16]
	cbz	r3, .L548
	ldr	r0, [r4, #244]
	bl	rk_dvbm_unlink
	ldr	r2, [r4, #52]
	ldr	r3, [r10, #272]
	tst	r2, r3
	beq	.L548
	mov	r0, r4
	bl	rkvenc_reset
.L548:
	ldr	r3, [r6]
	lsls	r2, r3, #26
	bpl	.L522
	movw	r2, #1430
	ldr	r1, .L587+36
	ldr	r0, .L587+12
	bl	printk
	b	.L522
.L546:
	ldr	r3, [r5, #3572]
	sub	r3, r3, fp
	tst	r3, #255
	bne	.L541
	add	r0, r5, #2208
	bl	cancel_delayed_work
	mov	r0, r5
	bl	mpp_time_diff
	ldr	r3, [r6]
	ldr	r1, [r4, #52]
	lsls	r0, r3, #29
	str	r1, [r5, #2916]
	bpl	.L542
	ldr	r0, .L587+40
	bl	printk
.L542:
	ldr	r3, [r5, #2316]
	ldr	r1, [r5, #3568]
	ldr	r0, [r9, #12]
	ldr	r3, [r3, #264]
	ldr	r2, [r5, #2916]
	add	r3, r3, r0
	ldr	r1, [r1, #40]
	bic	r3, r3, #3
	str	r2, [r1, r3]
	ldr	r3, [r5, #2204]
	cbnz	r3, .L543
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r2, [r5, #2200]
	orr	r2, r2, #128
	str	r2, [r5, #2200]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #1
	movs	r1, #3
	addw	r0, r5, #2284
	bl	__wake_up
.L543:
	add	r3, r9, #64
	str	r3, [sp, #12]
	mov	r0, r3
	bl	mutex_lock
	add	r1, r9, #56
	ldr	r0, [r5, #3568]
	bl	list_move_tail
	ldr	r3, [sp, #12]
	mov	r0, r3
	bl	mutex_unlock
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #2200]
	orr	r3, r3, #32
	str	r3, [r5, #2200]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldrd	r2, r3, [r5, #20]
	addw	r0, r5, #2252
	str	r3, [r2, #4]
	str	r2, [r3]
	str	r8, [r5, #20]
	str	r8, [r5, #24]
	pld	[r0]
	.syntax unified
@ 224 "./arch/arm/include/asm/atomic.h" 1
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
	bne	.L544
	bl	mpp_free_task
.L545:
	ldr	r3, [r7, #20]
	mov	r5, r7
	sub	r7, r3, #20
	b	.L540
.L544:
	cmp	r3, #0
	bgt	.L545
	movs	r1, #3
	bl	refcount_warn_saturate
	b	.L545
.L588:
	.align	2
.L587:
	.word	mpp_dev_debug
	.word	.LANCHOR0+685
	.word	.LC14
	.word	.LC16
	.word	.LC74
	.word	.LC75
	.word	.LC76
	.word	.LC77
	.word	.LC78
	.word	.LANCHOR0+696
	.word	.LC79
	.fnend
	.size	rkvenc_isr, .-rkvenc_isr
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_check_overflow, %function
rkvenc_check_overflow:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r6, r1
	.pad #20
	sub	sp, sp, #20
	movw	r1, #20488
	mov	r8, r2
	mov	r7, r3
	mov	r5, r0
	bl	mpp_read
	movw	r1, #20492
	mov	r4, r0
	mov	r0, r5
	bl	mpp_read
	movw	r1, #20484
	mov	r9, r0
	mov	r0, r5
	bl	mpp_read
	lsrs	r2, r4, #16
	lsr	r4, r9, #16
	ldr	r1, [r6]
	cmp	r4, r2
	ldr	r3, [r6, #4]
	it	cc
	movcc	r4, r2
	cmp	r1, r8
	lsl	r2, r4, #4
	bne	.L590
	cmp	r3, r7
	bhi	.L591
.L590:
	cbz	r3, .L592
	cmp	r1, r8
	beq	.L592
	ldr	r0, [r6, #8]
	cmp	r3, r0
	bcc	.L593
	strd	r1, r3, [sp]
	mov	r3, r7
	mov	r1, r8
	ldr	r0, .L601
	bl	printk
	movs	r3, #1
	str	r3, [r5, #248]
.L593:
	mov	r0, r5
	bl	rkvenc_dump_dbg
.L592:
	ldr	r3, [r6, #12]
	cmp	r3, r7
	bls	.L596
	movs	r1, #24
	mov	r0, r5
	bl	mpp_read
	ldr	r2, [r6, #12]
	movs	r1, #24
	bic	r0, r0, #16320
	bic	r0, r0, #63
	orrs	r2, r2, r0
	mov	r0, r5
	bl	mpp_write
	.syntax unified
@ 752 "drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r3, [r6, #4]
	mov	r2, r8
	ldr	r0, .L601+4
	str	r3, [sp, #4]
	ldr	r3, [r6]
	str	r3, [sp]
	mov	r3, r7
	ldr	r1, [r6, #12]
	bl	printk
.L596:
	movs	r4, #1
	b	.L589
.L591:
	ldr	r4, [r5, #240]
	ldr	r6, [r6, #8]
	cbz	r4, .L595
	subs	r4, r3, r2
	cmp	r4, r6
	bls	.L597
	strd	r3, r0, [sp, #4]
	mov	r3, r7
	str	r1, [sp]
	ldr	r0, .L601+8
	bl	printk
	movs	r3, #1
	str	r3, [r5, #248]
.L597:
	movs	r4, #0
	b	.L589
.L595:
	cmp	r3, r6
	bls	.L589
	strd	r3, r0, [sp, #4]
	mov	r3, r7
	str	r1, [sp]
	ldr	r0, .L601+8
	bl	printk
	movs	r3, #1
	str	r3, [r5, #248]
.L589:
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L602:
	.align	2
.L601:
	.word	.LC80
	.word	.LC81
	.word	.LC82
	.fnend
	.size	rkvenc_check_overflow, .-rkvenc_check_overflow
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rkvenc_run, %function
rkvenc_run:
	.fnstart
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #76
	sub	sp, sp, #76
	ldr	r3, [r1]
	mov	r4, r0
	ldr	r7, [r0, #132]
	mov	r5, r1
	ldr	r3, [r3, #196]
	str	r3, [sp, #20]
	ldr	r3, .L657
	ldr	r3, [r3]
	lsls	r0, r3, #26
	bpl	.L604
	movw	r2, #1125
	ldr	r1, .L657+4
	ldr	r0, .L657+8
	bl	printk
.L604:
	ldr	r8, [r4, #272]
	movs	r3, #0
	str	r3, [r4, #252]
	cmp	r8, #1
	beq	.L605
	cmp	r8, #0
	beq	.L606
	cmp	r8, #2
	beq	.L607
.L608:
	ldr	r3, .L657
	ldr	r0, [r3]
	ands	r0, r0, #32
	beq	.L603
	movw	r2, #1233
	ldr	r1, .L657+4
	ldr	r0, .L657+12
	bl	printk
	movs	r0, #0
	b	.L603
.L606:
	mov	r10, r5
	mov	r9, r8
	mov	fp, r8
.L609:
	ldr	r3, [r5, #2920]
	cmp	r3, fp
	bhi	.L619
	ldr	r6, .L657
	str	r5, [r4, #88]
	ldr	r3, [r6]
	lsls	r1, r3, #24
	bpl	.L620
	ldr	r3, [r5]
	movw	r1, #1148
	mov	r0, r4
	ldr	fp, [r5, #2276]
	ldr	r5, [r5, #2312]
	ldr	r10, [r3, #4]
	bl	mpp_read
	mov	r3, fp
	ldr	r1, .L657+4
	mov	r2, r10
	strd	r0, r9, [sp, #4]
	str	r5, [sp]
	ldr	r0, .L657+16
	bl	printk
.L620:
	movw	r1, #20484
	mov	r0, r4
	bl	mpp_read
	lsls	r2, r0, #21
	mov	r3, r0
	bpl	.L621
	movw	r2, #1177
	ldr	r1, .L657+4
	ldr	r0, .L657+20
	bl	printk
.L621:
	cmp	r9, #0
	beq	.L622
	movs	r3, #0
	ldr	r0, [r4, #244]
	str	r3, [r4, #248]
	bl	rk_dvbm_link
	movs	r1, #20
	ldr	r0, [r4, #244]
	add	r2, sp, #36
	bl	rk_dvbm_ctrl
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	ldr	r2, [sp, #40]
	ubfx	r3, r0, #0, #14
	mov	r5, r0
	cmp	r3, r2
	bcs	.L624
	ubfx	r2, r0, #14, #8
	add	r1, sp, #36
	mov	r0, r4
	bl	rkvenc_check_overflow
	cbnz	r0, .L624
	ldrd	r3, r2, [sp, #36]
	lsrs	r5, r5, #22
	movs	r1, #24
	mov	r0, r4
	orr	r2, r2, r3, lsl #14
	lsls	r5, r5, #22
	orrs	r2, r2, r5
	bl	mpp_write
	mov	r1, #776
	mov	r0, r4
	bl	mpp_read
	mov	r1, #776
	ldr	r3, [sp, #36]
	bic	r2, r0, #255
	mov	r0, r4
	orrs	r2, r2, r3
	bl	mpp_write
	ldr	r3, [r6]
	lsls	r3, r3, #15
	bpl	.L624
	ldrd	r2, r3, [sp, #36]
	ldr	r1, .L657+24
	ldr	r0, .L657+28
	bl	printk
.L624:
	ldr	r3, [sp, #20]
	movs	r5, #1
	mov	r2, r9
	mov	r0, r4
	str	r5, [r3, #20]
	ldr	r1, [r7, #280]
	bl	mpp_write_relaxed
	str	r5, [r4, #128]
.L622:
	ldr	r3, [sp, #20]
	str	r9, [r3, #16]
	.syntax unified
@ 1191 "drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	cmp	r9, #0
	bne	.L625
	mov	r2, r8
	ldr	r1, [r7, #244]
	mov	r0, r4
	bl	mpp_write
.L625:
	movs	r3, #1
	str	r3, [r4, #240]
	b	.L608
.L619:
	ldr	r3, [r5, #2316]
	movs	r2, #20
	movs	r1, #0
	add	r0, sp, #52
	ldr	r6, [r10, #2936]
	str	r3, [sp, #24]
	bl	memset
	movs	r0, #0
	ldr	r3, [sp, #24]
	ldr	ip, [r3, #24]
.L610:
	cmp	ip, r0
	bne	.L613
	mvn	r0, #21
.L603:
	add	sp, sp, #76
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L613:
	ldr	r2, [r3, #28]
	cmp	r6, r2
	bcc	.L611
	ldr	r1, [r3, #32]
	cmp	r6, r1
	bcs	.L611
	movs	r3, #12
	subs	r6, r6, r2
	mla	r0, r3, r0, r5
	str	r2, [sp, #64]
	ldr	r2, [r10, #2932]
	lsrs	r6, r6, #2
	add	r2, r6, r2, lsr #2
	str	r2, [sp, #24]
	ldr	r3, [r0, #2328]
	str	r3, [sp, #60]
	ldr	r3, [r0, #2324]
	str	r3, [sp, #68]
.L612:
	ldr	r2, [sp, #24]
	cmp	r6, r2
	bcc	.L618
	add	fp, fp, #1
	add	r10, r10, #20
	b	.L609
.L611:
	adds	r0, r0, #1
	adds	r3, r3, #20
	b	.L610
.L618:
	ldr	r1, [sp, #64]
	ldr	r0, [r7, #244]
	add	r1, r1, r6, lsl #2
	ldr	r2, [r3, r6, lsl #2]
	cmp	r0, r1
	beq	.L629
	cmp	r1, #696
	bne	.L616
	str	r2, [r4, #284]
.L617:
	ldr	r0, [r7, #280]
	ldr	r2, [r3, r6, lsl #2]
	cmp	r0, r1
	beq	.L630
	mov	r0, r4
	str	r3, [sp, #28]
	bl	mpp_write_relaxed
	ldr	r3, [sp, #28]
.L615:
	adds	r6, r6, #1
	b	.L612
.L616:
	movw	r0, #1036
	cmp	r1, r0
	it	eq
	streq	r2, [r4, #288]
	b	.L617
.L629:
	mov	r8, r2
	b	.L615
.L630:
	mov	r9, r2
	b	.L615
.L607:
	str	r3, [r4, #276]
	mov	r1, r5
	mov	r0, r4
	bl	rkvenc_run_start_link
	add	r3, r4, #276
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
	str	r2, [r5, #3572]
	str	r5, [r4, #88]
.L656:
	.syntax unified
@ 1217 "drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r3, [r4, #280]
	cmp	r3, #0
	beq	.L608
	movw	r2, #513
.L653:
	ldr	r1, [r7, #244]
	mov	r0, r4
	bl	mpp_write
	b	.L608
.L605:
	ldr	r1, [r7, #292]
	mov	r0, r4
	bl	mpp_read
	add	r6, r4, #276
	cbnz	r0, .L627
	str	r0, [r4, #276]
	mov	r1, r5
	mov	r0, r4
	bl	rkvenc_run_start_link
	pld	[r6]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add_return
1:	ldrex	r3, [r6]
	add	r3, r3, #1
	strex	r2, r3, [r6]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	str	r3, [r5, #3572]
	b	.L656
.L627:
	pld	[r6]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add_return
1:	ldrex	r3, [r6]
	add	r3, r3, #1
	strex	r2, r3, [r6]
	teq	r2, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	str	r3, [r5, #3572]
	.syntax unified
@ 1223 "drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r3, [r4, #280]
	cmp	r3, #0
	beq	.L608
	movw	r2, #769
	b	.L653
.L658:
	.align	2
.L657:
	.word	mpp_dev_debug
	.word	.LANCHOR0+712
	.word	.LC14
	.word	.LC16
	.word	.LC83
	.word	.LC84
	.word	.LANCHOR0+723
	.word	.LC85
	.fnend
	.size	rkvenc_run, .-rkvenc_run
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
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r6, r2
	mov	r4, r0
	cbz	r0, .L661
	subs	r1, r1, #18
	cmp	r1, #4
	bhi	.L661
	tbb	[pc, r1]
.L663:
	.byte	(.L667-.L663)/2
	.byte	(.L666-.L663)/2
	.byte	(.L665-.L663)/2
	.byte	(.L664-.L663)/2
	.byte	(.L662-.L663)/2
	.p2align 1
.L666:
	movs	r1, #24
	ldr	r5, [r2]
	bl	mpp_read
	str	r0, [sp, #12]
	cbnz	r5, .L668
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [sp, #12]
	bic	r3, r3, #16777216
	str	r3, [sp, #12]
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L668:
	ldr	r2, [sp, #12]
	movs	r1, #24
	mov	r0, r4
	bl	mpp_write
.L661:
	movs	r0, #0
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L665:
	ldr	r3, [r2]
	str	r3, [r0, #256]
	b	.L661
.L664:
	ldr	r3, [r2]
	str	r3, [r0, #260]
	b	.L661
.L662:
	movs	r1, #24
	bl	mpp_read
	mov	r1, r6
	ubfx	r7, r0, #14, #8
	ubfx	r8, r0, #0, #14
	mov	r5, r0
	mov	r0, r4
	mov	r3, r8
	mov	r2, r7
	bl	rkvenc_check_overflow
	cmp	r0, #0
	bne	.L661
	ldr	r3, [r6, #4]
	bic	r5, r5, #16320
	bic	r5, r5, #63
	movs	r1, #24
	orrs	r5, r5, r3
	mov	r0, r4
	str	r3, [r4, #252]
	mov	r2, r5
	bl	mpp_write
	.syntax unified
@ 820 "drivers/kmpp/mpp/mpp_rkvenc_540c.c" 1
	dsb st
@ 0 "" 2
	.thumb
	.syntax unified
	bl	arm_heavy_mb
	ldr	r3, .L674
	ldr	r3, [r3]
	lsls	r3, r3, #15
	bpl	.L670
	ldrd	r1, r2, [r6]
	ldr	r0, .L674+4
	bl	printk
.L670:
	movs	r1, #24
	mov	r0, r4
	bl	mpp_read
	cmp	r0, r5
	beq	.L661
	ldr	r3, [r6, #4]
	mov	r2, r8
	mov	r1, r7
	ldr	r0, .L674+8
	str	r3, [sp]
	ldr	r3, [r6]
	bl	printk
	b	.L661
.L667:
	bl	rkvenc_dump_dbg
	b	.L661
.L675:
	.align	2
.L674:
	.word	mpp_dev_debug
	.word	.LC86
	.word	.LC87
	.fnend
	.size	rkvenc_callback, .-rkvenc_callback
	.global	rockchip_rkvenc540c_driver
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.48856, %object
	.size	__func__.48856, 18
__func__.48856:
	.ascii	"rkvenc_update_req\000"
	.type	__func__.49399, %object
	.size	__func__.49399, 14
__func__.49399:
	.ascii	"rkvenc_result\000"
	.type	__func__.49574, %object
	.size	__func__.49574, 13
__func__.49574:
	.ascii	"rkvenc_reset\000"
	.type	__func__.49253, %object
	.size	__func__.49253, 11
__func__.49253:
	.ascii	"rkvenc_irq\000"
	.type	__func__.49095, %object
	.size	__func__.49095, 15
__func__.49095:
	.ascii	"rkvenc_prepare\000"
	.type	__func__.49563, %object
	.size	__func__.49563, 12
__func__.49563:
	.ascii	"rkvenc_init\000"
	.type	__func__.49430, %object
	.size	__func__.49430, 15
__func__.49430:
	.ascii	"rkvenc_control\000"
	.type	__func__.49471, %object
	.size	__func__.49471, 20
__func__.49471:
	.ascii	"rkvenc_init_session\000"
	.type	__func__.49617, %object
	.size	__func__.49617, 19
__func__.49617:
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
	.type	__func__.49552, %object
	.size	__func__.49552, 19
__func__.49552:
	.ascii	"rkvenc_procfs_init\000"
	.type	__func__.49725, %object
	.size	__func__.49725, 21
__func__.49725:
	.ascii	"rkvenc_probe_default\000"
	.type	__func__.49704, %object
	.size	__func__.49704, 17
__func__.49704:
	.ascii	"rkvenc_link_init\000"
	.type	__func__.49668, %object
	.size	__func__.49668, 24
__func__.49668:
	.ascii	"rkvenc_link_alloc_table\000"
	.type	__func__.49043, %object
	.size	__func__.49043, 18
__func__.49043:
	.ascii	"rkvenc_alloc_task\000"
	.type	__func__.48904, %object
	.size	__func__.48904, 24
__func__.48904:
	.ascii	"rkvenc_extract_task_msg\000"
	.type	__func__.48951, %object
	.size	__func__.48951, 16
__func__.48951:
	.ascii	"rkvenc_dump_dbg\000"
	.type	__func__.49349, %object
	.size	__func__.49349, 14
__func__.49349:
	.ascii	"rkvenc_finish\000"
	.type	__func__.49313, %object
	.size	__func__.49313, 11
__func__.49313:
	.ascii	"rkvenc_isr\000"
	.type	__func__.49302, %object
	.size	__func__.49302, 16
__func__.49302:
	.ascii	"rkvenc_link_isr\000"
	.type	__func__.49191, %object
	.size	__func__.49191, 11
__func__.49191:
	.ascii	"rkvenc_run\000"
	.type	__func__.49033, %object
	.size	__func__.49033, 19
__func__.49033:
	.ascii	"update_online_info\000"
	.space	2
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
	.set	.LANCHOR2,. + 0
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
	.word	.LC88
	.space	20
	.word	mpp_rkvenc_dt_match
	.space	48
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
	.space	4
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
	.set	.LANCHOR1,. + 0
	.type	__key.49472, %object
	.size	__key.49472, 0
__key.49472:
	.type	__key.49708, %object
	.size	__key.49708, 0
__key.49708:
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
	.ascii	"---------------------------------------------------"
	.ascii	"---\000"
.LC5:
	.ascii	"---------------------------------------------------"
	.ascii	"---\012\000"
.LC6:
	.ascii	"session\000"
.LC7:
	.ascii	"|%8s|\000"
.LC8:
	.ascii	"device\000"
.LC9:
	.ascii	"%8s|\000"
.LC10:
	.ascii	"\012\000"
.LC11:
	.ascii	"|%8p|\000"
.LC12:
	.ascii	"%8d|\000"
.LC13:
	.ascii	"null\000"
.LC14:
	.ascii	"\0016rkvenc_540c: %s:%d: enter\012\000"
.LC15:
	.ascii	"\0013rkvenc_540c: %s:%d: copy_to_user reg fail\012\000"
.LC16:
	.ascii	"\0016rkvenc_540c: %s:%d: leave\012\000"
.LC17:
	.ascii	"\0013rkvenc_540c: clear dvbm info failed 0x%08x 0x%"
	.ascii	"08x\012\000"
.LC18:
	.ascii	"\0013rkvenc_540c: jpeg overflow\012\000"
.LC19:
	.ascii	"\0013rkvenc_540c: video overflow\012\000"
.LC20:
	.ascii	"\0013rkvenc_540c: %s:%d: xxxx task filled, mpp_stat"
	.ascii	"e=%lx\012\000"
.LC21:
	.ascii	"aclk_vcodec\000"
.LC22:
	.ascii	"\0013rkvenc_540c: %s:%d: failed on clk_get aclk_vco"
	.ascii	"dec\012\000"
.LC23:
	.ascii	"hclk_vcodec\000"
.LC24:
	.ascii	"\0013rkvenc_540c: %s:%d: failed on clk_get hclk_vco"
	.ascii	"dec\012\000"
.LC25:
	.ascii	"clk_core\000"
.LC26:
	.ascii	"\0013rkvenc_540c: %s:%d: failed on clk_get clk_core"
	.ascii	"\012\000"
.LC27:
	.ascii	"rockchip,default-max-load\000"
.LC28:
	.ascii	"video_a\000"
.LC29:
	.ascii	"\0013rkvenc_540c: %s:%d: No aclk reset resource def"
	.ascii	"ine\012\000"
.LC30:
	.ascii	"video_h\000"
.LC31:
	.ascii	"\0013rkvenc_540c: %s:%d: No hclk reset resource def"
	.ascii	"ine\012\000"
.LC32:
	.ascii	"video_core\000"
.LC33:
	.ascii	"\0013rkvenc_540c: %s:%d: No core reset resource def"
	.ascii	"ine\012\000"
.LC34:
	.ascii	"\0013rkvenc_540c: %s:%d: session info null\012\000"
.LC35:
	.ascii	"\0016rkvenc_540c: codec info count %d\012\000"
.LC36:
	.ascii	"\0013rkvenc_540c: %s:%d: copy_from_user failed\012\000"
.LC37:
	.ascii	"\0013rkvenc_540c: %s:%d: codec info invalid, type %"
	.ascii	"d, flag %d\012\000"
.LC38:
	.ascii	"\0013rkvenc_540c: %s:%d: unknown mpp ioctl cmd %x\012"
	.ascii	"\000"
.LC39:
	.ascii	"shutdown device\012\000"
.LC40:
	.ascii	"wait total running time out\012\000"
.LC41:
	.ascii	"shutdown success\012\000"
.LC42:
	.ascii	"\0013rkvenc_540c: %s:%d: session is null\012\000"
.LC43:
	.ascii	"&priv->rw_sem\000"
.LC44:
	.ascii	"remove device\012\000"
.LC45:
	.ascii	"probing start\012\000"
.LC46:
	.ascii	"dt match failed!\012\000"
.LC47:
	.ascii	"register interrupter runtime failed\012\000"
.LC48:
	.ascii	"%s%d\000"
.LC49:
	.ascii	"\0013rkvenc_540c: %s:%d: failed on open procfs\012\000"
.LC50:
	.ascii	"aclk\000"
.LC51:
	.ascii	"session_buffers\000"
.LC52:
	.ascii	"dump_regs\000"
.LC53:
	.ascii	"sessions-info\000"
.LC54:
	.ascii	"link_mode\000"
.LC55:
	.ascii	"dvbm\000"
.LC56:
	.ascii	"\0013rkvenc_540c: %s:%d: failed to get device node\012"
	.ascii	"\000"
.LC57:
	.ascii	"rockchip,task-capacity\000"
.LC58:
	.ascii	"%d task capacity link mode detected\012\000"
.LC59:
	.ascii	"&link->list_mutex\000"
.LC60:
	.ascii	"dma alloc failed\012\000"
.LC61:
	.ascii	"probing finish\012\000"
.LC62:
	.ascii	"\0016rkvenc_540c: w_req_cnt=%d, r_req_cnt=%d\012\000"
.LC63:
	.ascii	"\0013rkvenc_540c: dvbm_en     -   %d\012\000"
.LC64:
	.ascii	"\0013rkvenc_540c: fmt         -   %d\012\000"
.LC65:
	.ascii	"\0013rkvenc_540c: jpg_en      -   %d\012\000"
.LC66:
	.ascii	"\0013rkvenc_540c: vsldy       -   0x%08x\012\000"
.LC67:
	.ascii	"\0013rkvenc_540c: st_wdg      -   0x%08x\012\000"
.LC68:
	.ascii	"\0013rkvenc_540c: st_ppl      -   0x%08x\012\000"
.LC69:
	.ascii	"\0013rkvenc_540c: st_vsp0     -   0x%08x\012\000"
.LC70:
	.ascii	"\0013rkvenc_540c: st_vsp1     -   0x%08x\012\000"
.LC71:
	.ascii	"\0013rkvenc_540c: vepu_cycle  -   0x%08x\012\000"
.LC72:
	.ascii	"\0016rkvenc_540c: === %s ===\012\000"
.LC73:
	.ascii	"\0016rkvenc_540c: reg[0x%0x] = 0x%08x\012\000"
.LC74:
	.ascii	"current frame has overflow\012\000"
.LC75:
	.ascii	"\0016rkvenc_540c: task %d fmt %d dvbm_en %d irq_sta"
	.ascii	"tus 0x%08x\012\000"
.LC76:
	.ascii	"task %d fmt %d dvbm_en %d irq_status 0x%08x\012\000"
.LC77:
	.ascii	"\0016rkvenc_540c: irq_status: %08x\012\000"
.LC78:
	.ascii	"no current task\012\000"
.LC79:
	.ascii	"\0016rkvenc_540c: link_irq_status: %08x\012\000"
.LC80:
	.ascii	"\0013rkvenc_540c: overflow vepu[frm: %d line: %d %d"
	.ascii	"] isp[frm: %d line: %d]\012\000"
.LC81:
	.ascii	"\0013rkvenc_540c: line cnt disorder set max %d [%d "
	.ascii	"%d]->[%d %d]\012\000"
.LC82:
	.ascii	"\0013rkvenc_540c: overflow vepu[frm: %d line: %d %d"
	.ascii	"] isp[frm: %d line: %d] wrk 0x%08x\012\000"
.LC83:
	.ascii	"\0016rkvenc_540c: %s session %d task %d enc_pic %d "
	.ascii	"jpeg_cfg %08x dvbm_en %d\012\000"
.LC84:
	.ascii	"\0013rkvenc_540c: %s:%d: enc started status %08x\012"
	.ascii	"\000"
.LC85:
	.ascii	"\0016rkvenc_540c: %s frame cnt %d line cnt %d\012\000"
.LC86:
	.ascii	"\0016rkvenc_540c: frame cnt %d line cnt %d\012\000"
.LC87:
	.ascii	"\0013rkvenc_540c: set frame info failed! [%d %d] ->"
	.ascii	" [%d %d]\012\000"
.LC88:
	.ascii	"mpp_rkvenc_540c\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
