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
	.file	"mpp_bitread.c"
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
	.type	update_curbyte, %function
update_curbyte:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	ldr	r3, [r0, #4]
	cbnz	r3, .L2
.L6:
	mvn	r0, #6
.L1:
	pop	{r4, r5, pc}
.L2:
	ldr	r2, [r0, #56]
	cbnz	r2, .L4
.L5:
	ldr	r3, [r0]
	movs	r5, #0
	adds	r2, r3, #1
	str	r2, [r0]
	ldrb	r4, [r3]	@ zero_extendqisi2
	ldr	r3, [r0, #4]
	subs	r3, r3, #1
	str	r3, [r0, #4]
	movs	r3, #8
	str	r3, [r0, #16]
	ldrd	r1, r3, [r0, #24]
	strd	r4, [r0, #8]
	orr	r2, r4, r1, lsl #8
	lsls	r3, r3, #8
	orr	r3, r3, r1, lsr #24
	strd	r2, r3, [r0, #24]
	movs	r0, #0
	b	.L1
.L4:
	ldr	r2, [r0]
	ldrb	r1, [r2]	@ zero_extendqisi2
	cmp	r1, #3
	bne	.L5
	ldrh	r4, [r0, #24]
	movs	r5, #0
	orrs	r1, r4, r5
	bne	.L5
	adds	r2, r2, #1
	subs	r3, r3, #1
	strd	r2, r3, [r0]
	movw	r4, #65535
	ldrd	r2, r1, [r0, #32]
	adds	r2, r2, #1
	adcs	r1, r1, r5
	movs	r5, #0
	strd	r2, r1, [r0, #32]
	strd	r4, [r0, #24]
	cmp	r3, #0
	bne	.L5
	b	.L6
	.fnend
	.size	update_curbyte, .-update_curbyte
	.align	1
	.global	mpp_read_bits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_read_bits, %function
mpp_read_bits:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r1, #31
	mov	r3, #0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r4, r0
	mov	r7, r1
	mov	r8, r2
	str	r3, [r2]
	ble	.L16
.L13:
	mvn	r0, #6
.L10:
	pop	{r4, r5, r6, r7, r8, pc}
.L14:
	subs	r0, r6, r0
	lsls	r3, r3, r0
	mov	r0, r4
	orrs	r3, r3, r5
	str	r3, [r8]
	ldr	r3, [r4, #16]
	subs	r6, r6, r3
	bl	update_curbyte
	cmp	r0, #0
	bne	.L13
.L11:
	ldr	r0, [r4, #16]
	ldr	r5, [r8]
	cmp	r6, r0
	ldrd	r3, ip, [r4, #8]
	bgt	.L14
	subs	r0, r0, r6
	rsb	r1, r0, #32
	subs	r2, r0, #32
	lsr	r3, r3, r0
	lsl	r1, ip, r1
	orr	r3, r3, r1
	itt	pl
	asrpl	r2, ip, r2
	orrpl	r3, r3, r2
	movs	r0, #0
	orrs	r3, r3, r5
	movs	r5, #1
	lsls	r5, r5, r7
	subs	r5, r5, #1
	ands	r3, r3, r5
	str	r3, [r8]
	ldr	r3, [r4, #16]
	ldr	r1, [r4, #40]
	subs	r6, r3, r6
	str	r6, [r4, #16]
	add	r1, r1, r7
	str	r1, [r4, #40]
	b	.L10
.L16:
	mov	r6, r1
	b	.L11
	.fnend
	.size	mpp_read_bits, .-mpp_read_bits
	.align	1
	.global	mpp_read_longbits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_read_longbits, %function
mpp_read_longbits:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r1, #31
	mov	r3, #0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r6, r0
	mov	r4, r1
	mov	r5, r2
	strd	r3, r3, [sp]
	bgt	.L18
	bl	mpp_read_bits
.L17:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L18:
	mov	r2, sp
	movs	r1, #16
	bl	mpp_read_bits
	cbz	r0, .L20
.L21:
	mvn	r0, #6
	b	.L17
.L20:
	add	r2, sp, #4
	sub	r1, r4, #16
	mov	r0, r6
	bl	mpp_read_bits
	cmp	r0, #0
	bne	.L21
	ldrd	r2, r3, [sp]
	orr	r3, r3, r2, lsl #16
	str	r3, [r5]
	b	.L17
	.fnend
	.size	mpp_read_longbits, .-mpp_read_longbits
	.align	1
	.global	mpp_skip_bits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_skip_bits, %function
mpp_skip_bits:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	mov	r6, r1
	mov	r5, r1
.L23:
	ldr	r3, [r4, #16]
	cmp	r3, r5
	blt	.L25
	subs	r3, r3, r5
	str	r3, [r4, #16]
	ldr	r3, [r4, #40]
	movs	r0, #0
	add	r3, r3, r6
	str	r3, [r4, #40]
.L22:
	pop	{r4, r5, r6, pc}
.L25:
	mov	r0, r4
	subs	r5, r5, r3
	bl	update_curbyte
	cmp	r0, #0
	beq	.L23
	mvn	r0, #6
	b	.L22
	.fnend
	.size	mpp_skip_bits, .-mpp_skip_bits
	.align	1
	.global	mpp_skip_longbits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_skip_longbits, %function
mpp_skip_longbits:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r1
	movs	r1, #16
	mov	r4, r0
	bl	mpp_skip_bits
	cbnz	r0, .L28
	sub	r1, r5, #16
	mov	r0, r4
	bl	mpp_skip_bits
	cbz	r0, .L27
.L28:
	mvn	r0, #6
.L27:
	pop	{r3, r4, r5, pc}
	.fnend
	.size	mpp_skip_longbits, .-mpp_skip_longbits
	.align	1
	.global	mpp_show_bits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_show_bits, %function
mpp_show_bits:
	.fnstart
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	ldmia	r5!, {r0, r1, r2, r3}
	.pad #68
	sub	sp, sp, #68
	cmp	r6, #31
	mov	r4, sp
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldm	r5, {r0, r1, r2, r3}
	stm	r4, {r0, r1, r2, r3}
	mov	r2, r7
	mov	r1, r6
	mov	r0, sp
	bgt	.L34
	bl	mpp_read_bits
.L33:
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L34:
	bl	mpp_read_longbits
	b	.L33
	.fnend
	.size	mpp_show_bits, .-mpp_show_bits
	.align	1
	.global	mpp_show_longbits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_show_longbits, %function
mpp_show_longbits:
	.fnstart
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	ldmia	r5!, {r0, r1, r2, r3}
	.pad #68
	sub	sp, sp, #68
	mov	r4, sp
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldmia	r5!, {r0, r1, r2, r3}
	stmia	r4!, {r0, r1, r2, r3}
	ldm	r5, {r0, r1, r2, r3}
	stm	r4, {r0, r1, r2, r3}
	mov	r2, r7
	mov	r1, r6
	mov	r0, sp
	bl	mpp_read_longbits
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
	.fnend
	.size	mpp_show_longbits, .-mpp_show_longbits
	.align	1
	.global	mpp_read_ue
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_read_ue, %function
mpp_read_ue:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r6, r0
	mov	r5, r1
	mov	r4, #-1
.L40:
	mov	r2, sp
	movs	r1, #1
	mov	r0, r6
	bl	mpp_read_bits
	cbz	r0, .L38
.L41:
	mvn	r0, #6
.L37:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L38:
	ldr	r3, [sp]
	adds	r4, r4, #1
	cmp	r3, #0
	beq	.L40
	cmp	r4, #31
	bgt	.L41
	movs	r3, #1
	cmp	r4, #0
	lsl	r3, r3, r4
	add	r3, r3, #-1
	str	r3, [r5]
	ble	.L37
	add	r2, sp, #4
	mov	r1, r4
	mov	r0, r6
	bl	mpp_read_bits
	cmp	r0, #0
	bne	.L41
	ldr	r3, [r5]
	ldr	r2, [sp, #4]
	add	r3, r3, r2
	str	r3, [r5]
	b	.L37
	.fnend
	.size	mpp_read_ue, .-mpp_read_ue
	.align	1
	.global	mpp_read_se
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_read_se, %function
mpp_read_se:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r1
	add	r1, sp, #4
	bl	mpp_read_ue
	cbnz	r0, .L47
	ldr	r2, [sp, #4]
	lsrs	r3, r2, #1
	lsls	r2, r2, #31
	ite	pl
	rsbpl	r3, r3, #0
	addmi	r3, r3, #1
	str	r3, [r4]
.L44:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L47:
	mvn	r0, #6
	b	.L44
	.fnend
	.size	mpp_read_se, .-mpp_read_se
	.align	1
	.global	mpp_has_more_rbsp_data
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_has_more_rbsp_data, %function
mpp_has_more_rbsp_data:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r5, r0
.L50:
	ldr	r3, [r5, #4]
	cbz	r3, .L51
	ldr	r2, [r5]
	subs	r3, r3, #1
	ldrb	r2, [r2, r3]	@ zero_extendqisi2
	cbz	r2, .L52
.L51:
	ldr	r4, [r5, #16]
	cbz	r4, .L53
.L56:
	ldr	r3, [r5, #4]
	movs	r0, #1
	cbz	r3, .L62
.L49:
	pop	{r3, r4, r5, r6, r7, pc}
.L52:
	str	r3, [r5, #4]
	b	.L50
.L53:
	mov	r0, r5
	bl	update_curbyte
	cmp	r0, #0
	beq	.L56
	mov	r0, r4
	b	.L49
.L62:
	ldr	r3, [r5, #16]
	subs	r3, r3, #1
	lsls	r0, r0, r3
	ldrd	r2, r3, [r5, #8]
	subs	r0, r0, #1
	asrs	r7, r0, #31
	ands	r0, r0, r2
	ands	r3, r3, r7
	orrs	r3, r0, r3
	ite	ne
	movne	r0, #1
	moveq	r0, #0
	b	.L49
	.fnend
	.size	mpp_has_more_rbsp_data, .-mpp_has_more_rbsp_data
	.align	1
	.global	mpp_set_bitread_ctx
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_set_bitread_ctx, %function
mpp_set_bitread_ctx:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	mov	r6, r1
	mov	r5, r2
	movs	r1, #0
	movs	r2, #56
	adds	r0, r0, #8
	bl	memset
	movw	r2, #65535
	movs	r3, #0
	strd	r6, r5, [r4]
	strd	r2, [r4, #24]
	strd	r6, r5, [r4, #44]
	pop	{r4, r5, r6, pc}
	.fnend
	.size	mpp_set_bitread_ctx, .-mpp_set_bitread_ctx
	.align	1
	.global	mpp_set_pre_detection
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_set_pre_detection, %function
mpp_set_pre_detection:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #1
	str	r3, [r0, #56]
	bx	lr
	.fnend
	.size	mpp_set_pre_detection, .-mpp_set_pre_detection
	.align	1
	.global	mpp_align_get_bits
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_align_get_bits, %function
mpp_align_get_bits:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r1, [r0, #16]
	cbz	r1, .L66
	bl	mpp_skip_bits
.L66:
	ldr	r0, [r4]
	pop	{r4, pc}
	.fnend
	.size	mpp_align_get_bits, .-mpp_align_get_bits
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
