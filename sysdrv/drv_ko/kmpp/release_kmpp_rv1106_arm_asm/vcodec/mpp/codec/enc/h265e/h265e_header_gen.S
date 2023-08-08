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
	.file	"h265e_header_gen.c"
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
	.type	h265e_nal_encode, %function
h265e_nal_encode:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #44
	sub	sp, sp, #44
	ldrd	r5, r7, [r1, #20]
	mov	r8, #0
	mov	r9, #1
	mov	r6, r1
	strb	r8, [r0]
	adds	r1, r0, #4
	strb	r8, [r0, #1]
	movs	r2, #10
	strb	r8, [r0, #2]
	mov	r4, r0
	strb	r9, [r0, #3]
	add	r0, sp, #4
	bl	mpp_writer_init
	mov	r2, r9
	mov	r1, r8
	add	r5, r5, r7
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #6
	ldr	r1, [r6, #4]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #6
	mov	r1, r8
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #3
	mov	r1, r9
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	cmp	r7, r5
	bcc	.L2
	adds	r3, r4, #6
.L3:
	subs	r3, r3, r4
	strd	r3, r4, [r6, #20]
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L2:
	mov	r3, r7
	ldrb	r2, [r3], #1	@ zero_extendqisi2
	cmp	r5, r3
	add	r3, r4, #7
	strb	r2, [r4, #6]
	bls	.L3
	ldrb	r0, [r7, #1]	@ zero_extendqisi2
	adds	r2, r7, #2
	add	r1, r4, #8
	strb	r0, [r4, #7]
.L5:
	cmp	r5, r2
	bne	.L6
	subs	r3, r5, r7
	subs	r3, r3, #2
	add	r3, r3, r1
	b	.L3
.L6:
	ldrb	r0, [r2], #1	@ zero_extendqisi2
	strb	r0, [r3, #1]!
	b	.L5
	.fnend
	.size	h265e_nal_encode, .-h265e_nal_encode
	.align	1
	.global	code_profile_tier
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	code_profile_tier, %function
code_profile_tier:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	movs	r3, #0
	mov	r5, r1
	mov	r4, r0
	movs	r2, #2
	ldr	r1, [r1, #44]
	bl	h265e_stream_write_with_log
	movs	r2, #0
	ldrb	r1, [r5]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	movs	r3, #0
	movs	r2, #5
	ldr	r1, [r5, #40]
	mov	r0, r4
	adds	r6, r5, #1
	bl	h265e_stream_write_with_log
	add	r7, r5, #33
.L8:
	ldrb	r1, [r6], #1	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	cmp	r6, r7
	bne	.L8
	ldrb	r1, [r5, #33]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r5, #34]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r5, #35]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r5, #36]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	movs	r3, #0
	mov	r1, r3
	movs	r2, #16
	mov	r0, r4
	bl	h265e_stream_write_with_log
	movs	r3, #0
	mov	r1, r3
	movs	r2, #16
	mov	r0, r4
	bl	h265e_stream_write_with_log
	movs	r3, #0
	mov	r0, r4
	movs	r2, #12
	mov	r1, r3
	pop	{r4, r5, r6, r7, r8, lr}
	b	h265e_stream_write_with_log
	.fnend
	.size	code_profile_tier, .-code_profile_tier
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	code_profile_tier_level.constprop.1, %function
code_profile_tier_level.constprop.1:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r1
	mov	r5, r0
	mov	r6, r2
	bl	code_profile_tier
	movs	r3, #0
	movs	r2, #8
	ldr	r1, [r4, #48]
	mov	r0, r5
	ldr	r9, .L27
	bl	h265e_stream_write_with_log
	add	r7, r4, #364
	sub	r9, r9, r4
	mov	r8, r4
.L11:
	add	r3, r9, r7
	cmp	r6, r3
	bgt	.L12
	cmp	r6, #0
	ble	.L10
	mov	r7, r6
.L13:
	movs	r3, #0
	movs	r2, #2
	mov	r1, r3
	mov	r0, r5
	adds	r7, r7, #1
	bl	h265e_stream_write_with_log
	cmp	r7, #8
	bne	.L13
	add	r6, r6, #370
	add	r7, r4, #370
	add	r4, r4, r6
.L14:
	cmp	r7, r4
	bne	.L17
.L10:
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L12:
	ldrb	r1, [r7], #1	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	ldrb	r1, [r7, #5]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	b	.L11
.L17:
	ldrb	r3, [r7, #-6]	@ zero_extendqisi2
	mov	r9, r7
	add	r6, r8, #52
	cbz	r3, .L15
	mov	r1, r6
	mov	r0, r5
	bl	code_profile_tier
.L15:
	ldrb	r3, [r9]	@ zero_extendqisi2
	adds	r7, r7, #1
	cbz	r3, .L16
	movs	r3, #0
	movs	r2, #8
	ldr	r1, [r8, #100]
	mov	r0, r5
	bl	h265e_stream_write_with_log
.L16:
	mov	r8, r6
	b	.L14
.L28:
	.align	2
.L27:
	.word	-364
	.fnend
	.size	code_profile_tier_level.constprop.1, .-code_profile_tier_level.constprop.1
	.align	1
	.global	codeVUI
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	codeVUI, %function
codeVUI:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	movs	r2, #0
	mov	r4, r1
	ldrb	r1, [r1]	@ zero_extendqisi2
	mov	r5, r0
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r4]	@ zero_extendqisi2
	cbz	r3, .L31
	movs	r3, #0
	movs	r2, #8
	ldr	r1, [r4, #20]
	mov	r0, r5
	bl	h265e_stream_write_with_log
	ldr	r3, [r4, #20]
	cmp	r3, #255
	bne	.L31
	movs	r3, #0
	movs	r2, #16
	ldr	r1, [r4, #24]
	mov	r0, r5
	bl	h265e_stream_write_with_log
	movs	r3, #0
	movs	r2, #16
	ldr	r1, [r4, #28]
	mov	r0, r5
	bl	h265e_stream_write_with_log
.L31:
	movs	r2, #0
	ldrb	r1, [r4, #1]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r4, #1]	@ zero_extendqisi2
	cbz	r3, .L33
	movs	r2, #0
	ldrb	r1, [r4, #2]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
.L33:
	movs	r2, #0
	ldrb	r1, [r4, #3]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r4, #3]	@ zero_extendqisi2
	cbz	r3, .L35
	movs	r3, #0
	movs	r2, #3
	ldr	r1, [r4, #32]
	mov	r0, r5
	bl	h265e_stream_write_with_log
	movs	r2, #0
	ldrb	r1, [r4, #4]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	ldrb	r1, [r4, #5]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r4, #5]	@ zero_extendqisi2
	cbz	r3, .L35
	movs	r3, #0
	movs	r2, #8
	ldr	r1, [r4, #36]
	mov	r0, r5
	bl	h265e_stream_write_with_log
	movs	r3, #0
	movs	r2, #8
	ldr	r1, [r4, #40]
	mov	r0, r5
	bl	h265e_stream_write_with_log
	movs	r3, #0
	movs	r2, #8
	ldr	r1, [r4, #44]
	mov	r0, r5
	bl	h265e_stream_write_with_log
.L35:
	movs	r2, #0
	ldrb	r1, [r4, #6]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r4, #6]	@ zero_extendqisi2
	cbz	r3, .L37
	movs	r2, #0
	ldr	r1, [r4, #48]
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r4, #52]
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
.L37:
	movs	r2, #0
	ldrb	r1, [r4, #7]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	ldrb	r1, [r4, #8]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	ldrb	r1, [r4, #12]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	ldrb	r1, [r4, #9]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r4, #9]	@ zero_extendqisi2
	cbz	r3, .L38
	movs	r2, #0
	ldr	r1, [r4, #56]
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r4, #60]
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r4, #64]
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r4, #68]
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
.L38:
	movs	r2, #0
	ldrb	r1, [r4, #10]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cbz	r3, .L39
	movs	r2, #0
	ldr	r1, [r4, #72]
	mov	r0, r5
	bl	h265e_stream_write32
	movs	r2, #0
	ldr	r1, [r4, #76]
	mov	r0, r5
	bl	h265e_stream_write32
	movs	r2, #0
	ldrb	r1, [r4, #11]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r4, #11]	@ zero_extendqisi2
	cbz	r3, .L40
	movs	r2, #0
	ldr	r1, [r4, #80]
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
.L40:
	movs	r2, #0
	ldrb	r1, [r4, #13]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
.L39:
	movs	r2, #0
	ldrb	r1, [r4, #14]	@ zero_extendqisi2
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r4, #14]	@ zero_extendqisi2
	cbz	r3, .L29
	ldrb	r1, [r4, #15]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r4, #16]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r4, #17]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r5
	bl	h265e_stream_write1_with_log
	ldr	r1, [r4, #84]
	movs	r2, #0
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
	ldr	r1, [r4, #88]
	ldr	r2, .L60
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
	ldr	r1, [r4, #92]
	movs	r2, #0
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
	ldr	r1, [r4, #96]
	movs	r2, #0
	mov	r0, r5
	bl	h265e_stream_write_ue_with_log
	ldr	r1, [r4, #100]
	mov	r0, r5
	movs	r2, #0
	pop	{r3, r4, r5, lr}
	b	h265e_stream_write_ue_with_log
.L29:
	pop	{r3, r4, r5, pc}
.L61:
	.align	2
.L60:
	.word	.LC2
	.fnend
	.size	codeVUI, .-codeVUI
	.align	1
	.global	h265e_nal_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_nal_start, %function
h265e_nal_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r4, #36
	ldr	r3, [r0]
	mla	r3, r4, r3, r0
	strd	r2, r1, [r3, #4]
	movs	r2, #1
	movs	r1, #0
	str	r2, [r3, #12]
	str	r1, [r3, #24]
	ldr	r2, [r0, #160]
	ldr	r0, [r0, #180]
	add	r2, r2, r0
	strd	r2, r1, [r3, #28]
	pop	{r4, pc}
	.fnend
	.size	h265e_nal_start, .-h265e_nal_start
	.align	1
	.global	h265e_nal_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_nal_end, %function
h265e_nal_end:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r3, [r0]
	movs	r1, #255
	ldr	r2, [r4, #180]
	ldr	r0, [r0, #160]
	add	r0, r0, r2
	movs	r2, #36
	mla	r3, r2, r3, r4
	ldr	r2, [r3, #28]
	subs	r2, r0, r2
	str	r2, [r3, #24]
	movs	r2, #64
	bl	memset
	ldr	r3, [r4]
	adds	r3, r3, #1
	str	r3, [r4]
	pop	{r4, pc}
	.fnend
	.size	h265e_nal_end, .-h265e_nal_end
	.align	1
	.global	h265e_init_extra_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_init_extra_info, %function
h265e_init_extra_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r1, #1536
	mov	r4, r0
	ldr	r0, .L67
	bl	mpp_osal_calloc
	movs	r3, #0
	str	r0, [r4, #148]
	mov	r0, r4
	str	r3, [r0], #160
	bl	h265e_stream_init
	mov	r1, #1024
	ldr	r0, .L67+4
	bl	mpp_osal_calloc
	ldr	r3, .L67+8
	add	r2, r3, #16
	str	r0, [r4, #152]
.L65:
	ldr	r1, [r3], #4	@ unaligned
	cmp	r3, r2
	str	r1, [r0], #4	@ unaligned
	bne	.L65
	movs	r0, #0
	pop	{r4, pc}
.L68:
	.align	2
.L67:
	.word	.LANCHOR0
	.word	.LANCHOR0+16
	.word	.LANCHOR0+38
	.fnend
	.size	h265e_init_extra_info, .-h265e_init_extra_info
	.align	1
	.global	h265e_deinit_extra_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_deinit_extra_info, %function
h265e_deinit_extra_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	adds	r0, r0, #160
	bl	h265e_stream_deinit
	ldr	r1, [r4, #148]
	cbz	r1, .L70
	ldr	r0, .L78
	bl	mpp_osal_free
.L70:
	ldr	r1, [r4, #152]
	movs	r5, #0
	str	r5, [r4, #148]
	str	r5, [r4]
	cbz	r1, .L71
	ldr	r0, .L78+4
	bl	mpp_osal_free
.L71:
	str	r5, [r4, #152]
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L79:
	.align	2
.L78:
	.word	.LANCHOR0+54
	.word	.LANCHOR0+72
	.fnend
	.size	h265e_deinit_extra_info, .-h265e_deinit_extra_info
	.global	__aeabi_uidiv
	.align	1
	.global	h265e_set_extra_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_set_extra_info, %function
h265e_set_extra_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 56
	@ frame_needed = 0, uses_anonymous_args = 0
	movw	r3, #19008
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r6, r0
	ldr	r7, [r0, r3]
	.pad #68
	sub	sp, sp, #68
	ldr	r3, .L216
	add	r9, r0, #12
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #1
	beq	.L81
	movw	r2, #958
	ldr	r1, .L216+4
	ldr	r0, .L216+8
	bl	printk
.L81:
	mov	r4, r7
	movs	r3, #0
	str	r3, [r4], #160
	mov	r0, r4
	bl	h265e_stream_reset
	movs	r2, #3
	movs	r1, #32
	mov	r0, r7
	bl	h265e_nal_start
	mov	r1, r9
	mov	r0, r6
	bl	h265e_set_vps
	ldr	r3, [r8]
	lsls	r3, r3, #31
	bpl	.L82
	movw	r2, #309
	ldr	r1, .L216+12
	ldr	r0, .L216+8
	bl	printk
.L82:
	mov	r0, r4
	add	r5, r6, #24
	bl	h265e_stream_realign
	ldr	r3, [r7, #180]
	movs	r2, #4
	ldr	r1, [r6, #12]
	mov	r0, r4
	mov	fp, #0
	str	r3, [sp, #12]
	movs	r3, #0
	bl	h265e_stream_write_with_log
	movs	r3, #0
	movs	r2, #2
	movs	r1, #3
	mov	r0, r4
	bl	h265e_stream_write_with_log
	movs	r3, #0
	mov	r1, r3
	movs	r2, #6
	mov	r0, r4
	bl	h265e_stream_write_with_log
	movs	r3, #0
	movs	r2, #3
	ldr	r1, [r6, #16]
	mov	r0, r4
	bl	h265e_stream_write_with_log
	movs	r2, #0
	ldr	r1, [r6, #20]
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	movs	r3, #0
	movs	r2, #16
	movw	r1, #65535
	mov	r0, r4
	bl	h265e_stream_write_with_log
	add	r1, r9, #4224
	ldr	r2, [r6, #16]
	adds	r1, r1, #4
	mov	r0, r4
	bl	code_profile_tier_level.constprop.1
	movs	r2, #0
	movs	r1, #1
	mov	r0, r4
	bl	h265e_stream_write1_with_log
.L83:
	ldr	r1, [r5, #28]
	mov	r10, r5
	movs	r2, #0
	mov	r0, r4
	adds	r5, r5, #4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r10]
	mov	r0, r4
	add	fp, fp, #1
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r5, #52]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldr	r3, [r6, #16]
	cmp	fp, r3
	bls	.L83
	ldr	r3, [r6, #108]
	cmp	r3, #1
	bls	.L84
	movw	r1, #339
	ldr	r3, .L216+16
	str	r1, [sp]
	ldr	r2, .L216+20
	ldr	r0, .L216+24
	bl	printk
.L84:
	ldr	r3, [r6, #112]
	cbz	r3, .L85
	movw	r1, #341
	ldr	r3, .L216+16
	str	r1, [sp]
	ldr	r2, .L216+28
	ldr	r0, .L216+24
	bl	printk
.L85:
	movs	r3, #0
	movs	r2, #6
	ldr	r1, [r6, #112]
	mov	r0, r4
	bl	h265e_stream_write_with_log
	movs	r2, #0
	mov	r1, r2
	str	r2, [r6, #116]
	mov	r0, r4
	add	fp, r6, #124
	bl	h265e_stream_write_ue_with_log
	movs	r5, #1
.L215:
	ldr	r2, [r6, #116]
	movs	r3, #1
	cmp	r5, r2
	bls	.L87
	movw	r5, #4216
	movs	r2, #0
	ldrb	r1, [r6, r5]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r6, r5]	@ zero_extendqisi2
	cbz	r3, .L88
	movw	r1, #4220
	movs	r3, #0
	movs	r2, #32
	ldr	r1, [r6, r1]
	mov	r0, r4
	movw	r5, #4228
	bl	h265e_stream_write_with_log
	add	r1, r6, #4224
	movs	r3, #0
	movs	r2, #32
	ldr	r1, [r1]
	mov	r0, r4
	bl	h265e_stream_write_with_log
	movs	r2, #0
	ldrb	r1, [r6, r5]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r6, r5]	@ zero_extendqisi2
	cbz	r3, .L89
	movw	r3, #4232
	movs	r2, #0
	ldr	r1, [r6, r3]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
.L89:
	movs	r2, #0
	mov	r0, r4
	str	r2, [r6, #108]
	mov	r1, r2
	bl	h265e_stream_write_ue_with_log
.L88:
	movs	r2, #0
	mov	r0, r4
	mov	r1, r2
	bl	h265e_stream_write1_with_log
	mov	r0, r4
	bl	h265e_stream_rbsp_trailing
	mov	r0, r4
	bl	h265e_stream_flush
	ldr	r3, [r8]
	lsls	r5, r3, #22
	bpl	.L90
	ldr	r2, [sp, #12]
	ldr	r3, [r7, #180]
	ldr	r1, .L216+12
	subs	r3, r3, r2
	ldr	r0, .L216+32
	movw	r2, #403
	lsls	r3, r3, #3
	bl	printk
.L90:
	ldr	r3, [r8]
	lsls	r0, r3, #31
	bpl	.L91
	movw	r2, #405
	ldr	r1, .L216+12
	ldr	r0, .L216+36
	bl	printk
.L91:
	add	r5, r6, #4608
	mov	r0, r7
	adds	r5, r5, #8
	bl	h265e_nal_end
	movs	r2, #3
	movs	r1, #33
	mov	r0, r7
	bl	h265e_nal_start
	mov	r2, r9
	mov	r1, r5
	mov	r0, r6
	bl	h265e_set_sps
	ldr	ip, .L216+40
	ldm	ip, {r0, r1, r2, r3}
	add	lr, sp, #32
	stm	lr, {r0, r1, r2, r3}
	add	r3, ip, #16
	add	lr, sp, #64
	ldm	r3, {r0, r1, r2, r3}
	stmdb	lr, {r0, r1, r2, r3}
	ldr	r3, [r8]
	lsls	r1, r3, #31
	bpl	.L92
	movw	r2, #541
	add	r1, ip, #32
	ldr	r0, .L216+8
	bl	printk
.L92:
	mov	r0, r4
	bl	h265e_stream_realign
	ldr	r3, [r7, #180]
	movs	r2, #4
	ldr	r1, [r5, #52]
	mov	r0, r4
	str	r3, [sp, #16]
	movs	r3, #0
	bl	h265e_stream_write_with_log
	movs	r3, #0
	movs	r2, #3
	ldr	r1, [r5, #60]
	mov	r0, r4
	bl	h265e_stream_write_with_log
	ldrb	r1, [r5, #6]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	movw	r3, #13648
	ldr	r1, [r5, r3]
	mov	r0, r4
	ldr	r2, [r5, #60]
	bl	code_profile_tier_level.constprop.1
	movs	r2, #0
	ldr	r1, [r5, #48]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r5, #56]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldr	r3, [r5, #56]
	cmp	r3, #4
	bne	.L93
	movw	r3, #4616
	movs	r2, #0
	ldrb	r1, [r6, r3]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	b	.L217
.L218:
	.align	2
.L216:
	.word	h265e_debug
	.word	.LANCHOR0+96
	.word	.LC3
	.word	.LANCHOR0+117
	.word	.LANCHOR0+133
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LANCHOR0+152
.L217:
.L93:
	movs	r2, #0
	ldr	r1, [r5, #64]
	mov	r0, r4
	movw	r8, #4469
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r5, #68]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldrb	r1, [r5, r8]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r5, r8]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L94
	ldr	r3, [r5, #56]
	add	r2, sp, #64
	add	r3, r2, r3, lsl #2
	ldr	r1, [r3, #-32]
	movw	r3, #4516
	ldr	r0, [r5, r3]
	bl	__aeabi_uidiv
	movs	r2, #0
	mov	r1, r0
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldr	r3, [r5, #56]
	add	r2, sp, #64
	add	r3, r2, r3, lsl #2
	ldr	r1, [r3, #-32]
	movw	r3, #4520
	ldr	r0, [r5, r3]
	bl	__aeabi_uidiv
	movs	r2, #0
	mov	r1, r0
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldr	r3, [r5, #56]
	add	r2, sp, #64
	add	r3, r2, r3, lsl #2
	ldr	r1, [r3, #-16]
	movw	r3, #4524
	ldr	r0, [r5, r3]
	bl	__aeabi_uidiv
	movs	r2, #0
	mov	r1, r0
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldr	r3, [r5, #56]
	add	r2, sp, #64
	add	r3, r2, r3, lsl #2
	ldr	r1, [r3, #-16]
	movw	r3, #4528
	ldr	r0, [r5, r3]
	bl	__aeabi_uidiv
	movs	r2, #0
	mov	r1, r0
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
.L94:
	movs	r2, #0
	ldr	r1, [r5, #144]
	mov	r0, r4
	add	r8, r6, #4704
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r5, #148]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldr	r1, [r5, #160]
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, #4
	add	r8, r8, #4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	movs	r1, #1
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	mov	r9, #0
.L95:
	ldr	r1, [r8, #208]
	mov	r10, r8
	movs	r2, #0
	mov	r0, r4
	add	r8, r8, #4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r10]
	mov	r0, r4
	add	r9, r9, #1
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r8, #232]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldr	r3, [r5, #60]
	cmp	r9, r3
	bls	.L95
	movs	r2, #0
	ldr	r1, [r5, #72]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r5, #76]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r5, #124]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r5, #120]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldr	r1, [r5, #128]
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, #1
	bl	h265e_stream_write_ue_with_log
	ldr	r1, [r5, #132]
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, #1
	bl	h265e_stream_write_ue_with_log
	ldrb	r1, [r5, #10]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r5, #10]	@ zero_extendqisi2
	cmp	r3, #1
	bne	.L96
	movs	r2, #0
	mov	r0, r4
	mov	r1, r2
	bl	h265e_stream_write1_with_log
.L97:
	ldrb	r1, [r5, #3]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r5, #9]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r5, #4]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r5, #4]	@ zero_extendqisi2
	cbz	r3, .L109
	ldr	r1, [r5, #152]
	movs	r3, #0
	movs	r2, #4
	mov	r0, r4
	subs	r1, r1, #1
	bl	h265e_stream_write_with_log
	ldr	r1, [r5, #156]
	ldr	r3, .L219
	movs	r2, #4
	subs	r1, r1, #1
	mov	r0, r4
	bl	h265e_stream_write_with_log
	ldr	r1, [r5, #140]
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, #3
	bl	h265e_stream_write_ue_with_log
	ldrd	r1, r3, [r5, #136]
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r3
	bl	h265e_stream_write_ue_with_log
	ldrb	r1, [r5, #5]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
.L109:
	ldr	r3, [r5, #60]
	adds	r3, r3, #1
	bne	.L110
	movw	r1, #670
	ldr	r3, .L219+4
	str	r1, [sp]
	ldr	r2, .L219+8
	ldr	r0, .L219+12
	bl	printk
.L110:
	movw	r8, #9068
	ldr	r2, .L219+16
	ldr	r1, [r6, r8]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldr	r10, .L219+56
	add	r8, r8, r6
	mov	r9, #0
.L111:
	ldr	r3, [r8]
	cmp	r9, r3
	bcc	.L112
	ldrb	r1, [r5, #1]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r5, #1]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L113
.L118:
	ldrb	r1, [r5, #2]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	ldrb	r1, [r5, #7]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	ldrb	r1, [r5, #8]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r5, #8]	@ zero_extendqisi2
	cbz	r3, .L115
	add	r1, r5, #4448
	mov	r0, r4
	adds	r1, r1, #12
	bl	codeVUI
.L115:
	movs	r2, #0
	mov	r0, r4
	mov	r1, r2
	bl	h265e_stream_write1_with_log
	mov	r0, r4
	bl	h265e_stream_rbsp_trailing
	mov	r0, r4
	bl	h265e_stream_flush
	ldr	r3, .L219+20
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #512
	beq	.L119
	ldr	r2, [sp, #16]
	ldr	r3, [r7, #180]
	ldr	r1, .L219+24
	subs	r3, r3, r2
	ldr	r0, .L219+28
	mov	r2, #716
	lsls	r3, r3, #3
	bl	printk
.L119:
	ldr	r3, [r9]
	lsls	r2, r3, #31
	bpl	.L120
	movw	r2, #718
	ldr	r1, .L219+24
	ldr	r0, .L219+32
	bl	printk
.L120:
	add	r8, r6, #18176
	mov	r0, r7
	add	r8, r8, #92
	bl	h265e_nal_end
	movs	r2, #3
	movs	r1, #34
	mov	r0, r7
	bl	h265e_nal_start
	mov	r2, r5
	mov	r1, r8
	mov	r0, r6
	bl	h265e_set_pps
	mov	r0, r4
	bl	h265e_stream_realign
	ldr	r3, [r9]
	ldr	r10, [r7, #180]
	lsls	r3, r3, #31
	bpl	.L121
	movw	r2, #738
	ldr	r1, .L219+36
	ldr	r0, .L219+40
	bl	printk
.L121:
	movs	r2, #0
	ldr	r1, [r8, #24]
	mov	r0, r4
	movw	r5, #18268
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r8, #28]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	mov	r1, r2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r8, #10]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	movs	r3, #0
	movs	r2, #3
	ldr	r1, [r8, #720]
	mov	r0, r4
	bl	h265e_stream_write_with_log
	movs	r2, #0
	ldrb	r1, [r8, #13]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r8, #12]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldr	r1, [r8, #48]
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, #1
	bl	h265e_stream_write_ue_with_log
	ldr	r1, [r8, #52]
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, #1
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r8, #32]
	mov	r0, r4
	bl	h265e_stream_write_se_with_log
	ldrb	r1, [r8, #1]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r8, #19]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r6, r5]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r6, r5]	@ zero_extendqisi2
	cbz	r3, .L122
	movs	r2, #0
	ldr	r1, [r8, #36]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
.L122:
	movs	r2, #0
	ldr	r1, [r8, #40]
	mov	r0, r4
	bl	h265e_stream_write_se_with_log
	movs	r2, #0
	ldr	r1, [r8, #44]
	mov	r0, r4
	bl	h265e_stream_write_se_with_log
	ldrb	r1, [r8, #2]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r8, #8]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r8, #9]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r8, #18]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldr	r2, .L219+44
	ldrb	r1, [r8, #14]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r8, #20]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r8, #14]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L123
	movs	r2, #0
	ldr	r1, [r8, #56]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldr	r1, [r8, #192]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	movs	r2, #0
	ldrb	r1, [r8, #15]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	ldrb	r5, [r8, #15]	@ zero_extendqisi2
	cbnz	r5, .L124
	add	fp, r6, #18304
	add	fp, fp, #24
.L125:
	ldr	r3, [r8, #56]
	cmp	r5, r3
	blt	.L128
	add	r6, r6, #18432
	movs	r5, #0
	adds	r6, r6, #28
.L129:
	ldr	r3, [r8, #192]
	cmp	r5, r3
	blt	.L130
.L124:
	ldr	r3, [r8, #56]
	ldr	r2, [r8, #192]
	cmn	r3, r2
	bne	.L127
	movw	r1, #819
	ldr	r3, .L219+48
	str	r1, [sp]
	ldr	r2, .L219+52
	ldr	r0, .L219+12
	bl	printk
.L127:
	ldrb	r1, [r8, #11]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	b	.L220
.L221:
	.align	2
.L219:
	.word	.LC9
	.word	.LANCHOR0+200
	.word	.LC10
	.word	.LC5
	.word	.LC11
	.word	h265e_debug
	.word	.LANCHOR0+184
	.word	.LC13
	.word	.LC8
	.word	.LANCHOR0+216
	.word	.LC3
	.word	.LC14
	.word	.LANCHOR0+232
	.word	.LC15
	.word	.LC12
.L220:
.L123:
	ldrb	r1, [r8, #5]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r8, #4]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r3, [r8, #4]	@ zero_extendqisi2
	cbz	r3, .L132
	ldrb	r1, [r8, #6]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r1, [r8, #7]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	ldrb	r5, [r8, #7]	@ zero_extendqisi2
	cbnz	r5, .L132
	mov	r2, r5
	ldr	r1, [r8, #708]
	mov	r0, r4
	bl	h265e_stream_write_se_with_log
	mov	r2, r5
	ldr	r1, [r8, #712]
	mov	r0, r4
	bl	h265e_stream_write_se_with_log
.L132:
	ldrb	r1, [r8, #16]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	ldrb	r1, [r8, #17]	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	ldr	r1, [r8, #716]
	mov	r0, r4
	bl	h265e_stream_write_ue_with_log
	ldrb	r1, [r8, #3]	@ zero_extendqisi2
	movs	r2, #0
	mov	r0, r4
	subs	r1, r1, r2
	it	ne
	movne	r1, #1
	bl	h265e_stream_write1_with_log
	movs	r2, #0
	mov	r1, r2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	mov	r0, r4
	bl	h265e_stream_rbsp_trailing
	mov	r0, r4
	bl	h265e_stream_flush
	ldr	r3, [r9]
	lsls	r5, r3, #22
	bpl	.L134
	ldr	r3, [r7, #180]
	movw	r2, #881
	ldr	r1, .L222
	sub	r3, r3, r10
	ldr	r0, .L222+4
	lsls	r3, r3, #3
	bl	printk
.L134:
	ldr	r3, [r9]
	lsls	r4, r3, #31
	bpl	.L135
	movw	r2, #882
	ldr	r1, .L222
	ldr	r0, .L222+8
	bl	printk
.L135:
	mov	r0, r7
	bl	h265e_nal_end
	ldr	r3, [r9]
	ldr	r5, [r7, #148]
	lsls	r0, r3, #31
	ldr	r6, [r7]
	bpl	.L136
	movs	r2, #178
	ldr	r1, .L222+12
	ldr	r0, .L222+16
	bl	printk
.L136:
	adds	r4, r7, #4
	mov	r8, #0
	b	.L138
.L87:
	str	r3, [fp], #4
	movs	r2, #0
	movs	r1, #1
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	adds	r5, r5, #1
	b	.L215
.L96:
	cmp	r3, #2
	bne	.L97
	ldr	r10, .L222+32
	movs	r2, #0
	movs	r1, #1
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	mov	r8, #0
	movs	r3, #16
.L107:
	cmp	r8, #1
	mov	r9, #0
	add	r3, r10, r3, lsl #2
	str	r3, [sp, #20]
	ite	gt
	movgt	r2, #16
	movle	r2, #8
	str	r2, [sp, #24]
.L106:
	cmp	r8, #0
	beq	.L146
	cmp	r8, #3
	beq	.L100
	ldr	r3, .L222+20
	add	r2, r3, #256
	cmp	r9, #2
	it	gt
	movgt	r3, r2
	mov	fp, r3
.L145:
	movs	r2, #0
	movs	r1, #1
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	cmp	r8, #1
	ble	.L102
	movs	r2, #0
	movs	r1, #8
	mov	r0, r4
	bl	h265e_stream_write_se_with_log
.L102:
	ldr	r1, [sp, #24]
	str	r10, [sp, #12]
.L105:
	ldr	r2, [sp, #12]
	ldr	r3, [r2], #4
	ldr	r3, [fp, r3, lsl #2]
	str	r2, [sp, #12]
	subs	r1, r3, r1
	cmp	r1, #127
	ble	.L103
	sub	r1, r1, #256
.L104:
	movs	r2, #0
	mov	r0, r4
	str	r3, [sp, #28]
	bl	h265e_stream_write_se_with_log
	ldr	r3, [sp, #28]
	ldr	r2, [sp, #12]
	mov	r1, r3
	ldr	r3, [sp, #20]
	cmp	r3, r2
	bne	.L105
.L144:
	add	r9, r9, #1
	cmp	r9, #6
	bne	.L106
	add	r8, r8, #1
	movs	r3, #64
	cmp	r8, #4
	ldr	r10, .L222+36
	bne	.L107
	b	.L97
.L100:
	cmp	r9, #0
	bne	.L101
	ldr	fp, .L222+20
	b	.L145
.L103:
	cmn	r1, #128
	it	lt
	addlt	r1, r1, #256
	b	.L104
.L112:
	movw	r1, #677
	mov	r0, r10
	bl	printk
	add	r9, r9, #1
	b	.L111
.L113:
	movs	r2, #0
	ldr	r1, [r5, #164]
	mov	r0, r4
	ldr	r10, .L222+40
	bl	h265e_stream_write_ue_with_log
	add	r9, r6, #4768
	add	r8, r6, #4608
	add	r9, r9, #16
	add	r8, r8, #20
	sub	r10, r10, r6
.L116:
	ldr	r2, [r5, #164]
	add	r3, r10, r8
	cmp	r2, r3
	bls	.L118
	ldr	r2, [r5, #160]
	movs	r3, #0
	ldr	r1, [r9], #4
	mov	r0, r4
	bl	h265e_stream_write_with_log
	movs	r2, #0
	ldrb	r1, [r8], #1	@ zero_extendqisi2
	mov	r0, r4
	bl	h265e_stream_write1_with_log
	b	.L116
.L128:
	mov	r3, fp
	movs	r2, #0
	ldr	r1, [r3, #4]
	mov	r0, r4
	ldr	r3, [r3]
	adds	r5, r5, #1
	subs	r1, r1, #1
	add	fp, fp, #4
	subs	r1, r1, r3
	bl	h265e_stream_write_ue_with_log
	b	.L125
.L130:
	mov	r3, r6
	ldr	r1, [r6, #8]
	ldr	r3, [r3]
	movs	r2, #0
	mov	r0, r4
	adds	r5, r5, #1
	subs	r1, r1, r3
	adds	r6, r6, #4
	bl	h265e_stream_write_ue_with_log
	b	.L129
.L139:
	cmp	r8, #0
	beq	.L148
	ldr	r2, [r4, #4]
	subs	r2, r2, #32
	cmp	r2, #2
	ite	hi
	movhi	r2, #0
	movls	r2, #1
.L137:
	str	r2, [r4, #8]
	mov	r1, r4
	mov	r0, r5
	add	r8, r8, #1
	bl	h265e_nal_encode
	ldr	r3, [r4, #20]
	adds	r4, r4, #36
	add	r5, r5, r3
.L138:
	cmp	r6, r8
	bgt	.L139
	ldr	r3, [r9]
	lsls	r1, r3, #22
	bpl	.L140
	ldr	r3, [r7, #148]
	movs	r2, #197
	ldr	r1, .L222+12
	subs	r3, r5, r3
	ldr	r0, .L222+24
	bl	printk
.L140:
	ldr	r3, [r9]
	lsls	r2, r3, #31
	bpl	.L189
	movs	r2, #199
	ldr	r1, .L222+12
	ldr	r0, .L222+8
	bl	printk
	ldr	r3, [r9]
	lsls	r3, r3, #31
	bpl	.L189
	movw	r2, #979
	ldr	r1, .L222+28
	ldr	r0, .L222+8
	bl	printk
.L189:
	movs	r0, #0
	add	sp, sp, #68
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L148:
	movs	r2, #1
	b	.L137
.L101:
	cmp	r9, #1
	bne	.L144
	ldr	fp, .L222+44
	b	.L145
.L146:
	ldr	fp, .L222+48
	b	.L145
.L223:
	.align	2
.L222:
	.word	.LANCHOR0+216
	.word	.LC16
	.word	.LC8
	.word	.LANCHOR0+248
	.word	.LC3
	.word	.LANCHOR1+64
	.word	.LC17
	.word	.LANCHOR0+96
	.word	.LANCHOR1
	.word	.LANCHOR1+640
	.word	-4628
	.word	.LANCHOR1+320
	.word	.LANCHOR1+576
	.fnend
	.size	h265e_set_extra_info, .-h265e_set_extra_info
	.align	1
	.global	h265e_data_to_sei
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_data_to_sei, %function
h265e_data_to_sei:
	.fnstart
	@ args = 0, pretend = 0, frame = 80
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r8, r3
	ldr	r4, .L247
	mov	r6, r0
	.pad #84
	sub	sp, sp, #84
	mov	r5, r1
	mov	r9, r2
	ldr	r3, [r4]
	lsls	r0, r3, #31
	bpl	.L225
	movw	r2, #990
	ldr	r1, .L247+4
	ldr	r0, .L247+8
	bl	printk
.L225:
	add	r0, sp, #36
	bl	h265e_stream_init
	movs	r2, #36
	movs	r1, #0
	mov	r0, sp
	bl	memset
	movs	r3, #39
	ldr	r2, [sp, #56]
	str	r3, [sp, #4]
	ldr	r3, [sp, #36]
	add	r3, r3, r2
	str	r3, [sp, #24]
	ldr	r3, [r4]
	lsls	r1, r3, #31
	bpl	.L226
	movs	r2, #211
	ldr	r1, .L247+12
	ldr	r0, .L247+8
	bl	printk
.L226:
	add	r0, sp, #36
	add	r10, r8, #16
	bl	h265e_stream_realign
	movs	r3, #0
	movs	r2, #8
	movs	r1, #5
	sub	fp, r8, #239
	movs	r7, #0
	add	r0, sp, #36
	bl	h265e_stream_write_with_log
.L227:
	cmp	r7, fp
	mov	r3, #0
	mov	r2, #8
	ble	.L228
	sub	r1, r10, r7
	add	r0, sp, #36
	bl	h265e_stream_write_with_log
	subs	r7, r5, #1
	adds	r5, r5, #15
.L229:
	ldrb	r1, [r7, #1]!	@ zero_extendqisi2
	movs	r3, #0
	movs	r2, #8
	add	r0, sp, #36
	bl	h265e_stream_write_with_log
	cmp	r5, r7
	bne	.L229
	movs	r5, #0
.L230:
	cmp	r8, r5
	bgt	.L231
	add	r0, sp, #36
	bl	h265e_stream_rbsp_trailing
	ldr	r3, [r4]
	lsls	r2, r3, #31
	bpl	.L232
	movs	r2, #235
	ldr	r1, .L247+12
	ldr	r0, .L247+16
	bl	printk
.L232:
	ldr	r2, [sp, #56]
	mov	r1, sp
	ldr	r3, [sp, #36]
	mov	r0, r6
	add	r3, r3, r2
	ldr	r2, [sp, #24]
	subs	r3, r3, r2
	str	r3, [sp, #20]
	bl	h265e_nal_encode
	add	r0, sp, #36
	bl	h265e_stream_deinit
	ldr	r3, [r4]
	lsls	r3, r3, #31
	bpl	.L233
	mov	r2, #1008
	ldr	r1, .L247+4
	ldr	r0, .L247+16
	bl	printk
.L233:
	ldr	r0, [sp, #20]
	add	sp, sp, #84
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L228:
	movs	r1, #255
	add	r0, sp, #36
	bl	h265e_stream_write_with_log
	adds	r7, r7, #255
	b	.L227
.L231:
	ldrb	r1, [r9, r5]	@ zero_extendqisi2
	movs	r3, #0
	movs	r2, #8
	add	r0, sp, #36
	bl	h265e_stream_write_with_log
	adds	r5, r5, #1
	b	.L230
.L248:
	.align	2
.L247:
	.word	h265e_debug
	.word	.LANCHOR0+271
	.word	.LC3
	.word	.LANCHOR0+289
	.word	.LC8
	.fnend
	.size	h265e_data_to_sei, .-h265e_data_to_sei
	.align	1
	.global	h265e_get_extra_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_get_extra_info, %function
h265e_get_extra_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r4, r0
	mov	r9, r1
	cmp	r1, #0
	beq	.L255
	ldr	r5, .L263
	ldr	r3, [r5]
	lsls	r2, r3, #31
	bpl	.L251
	movw	r2, #1021
	ldr	r1, .L263+4
	ldr	r0, .L263+8
	bl	printk
.L251:
	movw	r3, #19008
	ldr	r10, .L263+4
	ldr	r8, [r4, r3]
	movs	r6, #0
	ldr	fp, .L263+16
	mov	r7, r6
	add	r4, r8, #8
.L252:
	ldr	r3, [r8]
	cmp	r3, r7
	bgt	.L254
	mov	r0, r9
	mov	r1, r6
	bl	mpp_packet_set_length
	ldr	r0, [r5]
	ands	r0, r0, #1
	beq	.L249
	movw	r2, #1035
	ldr	r1, .L263+4
	ldr	r0, .L263+12
	bl	printk
	movs	r0, #0
.L249:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L254:
	ldr	r3, [r5]
	lsls	r3, r3, #22
	bpl	.L253
	ldr	r3, [r4, #16]
	movw	r2, #1028
	mov	r1, r10
	mov	r0, fp
	str	r3, [sp]
	ldr	r3, [r4]
	bl	printk
.L253:
	ldrd	r3, r2, [r4, #16]
	mov	r1, r6
	mov	r0, r9
	adds	r7, r7, #1
	bl	mpp_packet_write
	ldr	r3, [r4, #16]
	adds	r4, r4, #36
	add	r6, r6, r3
	b	.L252
.L255:
	mov	r0, #-1
	b	.L249
.L264:
	.align	2
.L263:
	.word	h265e_debug
	.word	.LANCHOR0+305
	.word	.LC3
	.word	.LC8
	.word	.LC18
	.fnend
	.size	h265e_get_extra_info, .-h265e_get_extra_info
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.9873, %object
	.size	__FUNCTION__.9873, 16
__FUNCTION__.9873:
	.ascii	"h265e_nals_init\000"
	.type	__FUNCTION__.10047, %object
	.size	__FUNCTION__.10047, 22
__FUNCTION__.10047:
	.ascii	"h265e_init_extra_info\000"
	.type	h265e_sei_uuid.10046, %object
	.size	h265e_sei_uuid.10046, 16
h265e_sei_uuid.10046:
	.byte	103
	.byte	-4
	.byte	106
	.byte	60
	.byte	-40
	.byte	92
	.byte	68
	.byte	30
	.byte	-121
	.byte	-5
	.byte	63
	.byte	-85
	.byte	-20
	.byte	-77
	.byte	54
	.byte	119
	.type	__FUNCTION__.9877, %object
	.size	__FUNCTION__.9877, 18
__FUNCTION__.9877:
	.ascii	"h265e_nals_deinit\000"
	.type	__FUNCTION__.10052, %object
	.size	__FUNCTION__.10052, 24
__FUNCTION__.10052:
	.ascii	"h265e_deinit_extra_info\000"
	.type	__func__.10060, %object
	.size	__func__.10060, 21
__func__.10060:
	.ascii	"h265e_set_extra_info\000"
	.type	__func__.9970, %object
	.size	__func__.9970, 16
__func__.9970:
	.ascii	"h265e_vps_write\000"
	.type	__FUNCTION__.9974, %object
	.size	__FUNCTION__.9974, 16
__FUNCTION__.9974:
	.ascii	"h265e_vps_write\000"
	.space	3
.LC0:
	.word	1
	.word	2
	.word	2
	.word	1
.LC1:
	.word	1
	.word	2
	.word	1
	.word	1
	.type	__func__.9999, %object
	.size	__func__.9999, 16
__func__.9999:
	.ascii	"h265e_sps_write\000"
	.type	__FUNCTION__.10003, %object
	.size	__FUNCTION__.10003, 16
__FUNCTION__.10003:
	.ascii	"h265e_sps_write\000"
	.type	__func__.10019, %object
	.size	__func__.10019, 16
__func__.10019:
	.ascii	"h265e_pps_write\000"
	.type	__FUNCTION__.10026, %object
	.size	__FUNCTION__.10026, 16
__FUNCTION__.10026:
	.ascii	"h265e_pps_write\000"
	.type	__func__.9906, %object
	.size	__func__.9906, 23
__func__.9906:
	.ascii	"h265e_encapsulate_nals\000"
	.type	__func__.10070, %object
	.size	__func__.10070, 18
__func__.10070:
	.ascii	"h265e_data_to_sei\000"
	.type	__func__.9926, %object
	.size	__func__.9926, 16
__func__.9926:
	.ascii	"h265e_sei_write\000"
	.type	__func__.10078, %object
	.size	__func__.10078, 21
__func__.10078:
	.ascii	"h265e_get_extra_info\000"
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	scan4, %object
	.size	scan4, 64
scan4:
	.word	0
	.word	4
	.word	1
	.word	8
	.word	5
	.word	2
	.word	12
	.word	9
	.word	6
	.word	3
	.word	13
	.word	10
	.word	7
	.word	14
	.word	11
	.word	15
	.type	sclCoefIntra8, %object
	.size	sclCoefIntra8, 256
sclCoefIntra8:
	.word	16
	.word	16
	.word	16
	.word	16
	.word	17
	.word	18
	.word	21
	.word	23
	.word	16
	.word	16
	.word	16
	.word	16
	.word	17
	.word	19
	.word	21
	.word	24
	.word	16
	.word	16
	.word	17
	.word	18
	.word	20
	.word	21
	.word	24
	.word	28
	.word	16
	.word	16
	.word	18
	.word	21
	.word	23
	.word	26
	.word	30
	.word	34
	.word	17
	.word	17
	.word	20
	.word	23
	.word	29
	.word	33
	.word	35
	.word	35
	.word	18
	.word	19
	.word	21
	.word	26
	.word	33
	.word	36
	.word	36
	.word	36
	.word	21
	.word	21
	.word	24
	.word	30
	.word	35
	.word	36
	.word	37
	.word	38
	.word	23
	.word	24
	.word	28
	.word	34
	.word	35
	.word	36
	.word	38
	.word	39
	.type	sclCoefInter8, %object
	.size	sclCoefInter8, 256
sclCoefInter8:
	.word	16
	.word	16
	.word	16
	.word	16
	.word	17
	.word	18
	.word	21
	.word	23
	.word	16
	.word	16
	.word	16
	.word	16
	.word	17
	.word	19
	.word	21
	.word	24
	.word	16
	.word	16
	.word	17
	.word	18
	.word	20
	.word	21
	.word	24
	.word	27
	.word	16
	.word	16
	.word	18
	.word	21
	.word	23
	.word	26
	.word	29
	.word	30
	.word	17
	.word	17
	.word	20
	.word	23
	.word	29
	.word	31
	.word	31
	.word	32
	.word	18
	.word	19
	.word	21
	.word	26
	.word	31
	.word	31
	.word	33
	.word	33
	.word	21
	.word	21
	.word	24
	.word	29
	.word	31
	.word	33
	.word	34
	.word	34
	.word	23
	.word	24
	.word	27
	.word	30
	.word	32
	.word	33
	.word	34
	.word	35
	.type	sclCoef4, %object
	.size	sclCoef4, 64
sclCoef4:
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.word	16
	.type	scan8, %object
	.size	scan8, 256
scan8:
	.word	0
	.word	8
	.word	1
	.word	16
	.word	9
	.word	2
	.word	24
	.word	17
	.word	10
	.word	3
	.word	32
	.word	25
	.word	18
	.word	11
	.word	4
	.word	40
	.word	33
	.word	26
	.word	19
	.word	12
	.word	5
	.word	48
	.word	41
	.word	34
	.word	27
	.word	20
	.word	13
	.word	6
	.word	56
	.word	49
	.word	42
	.word	35
	.word	28
	.word	21
	.word	14
	.word	7
	.word	57
	.word	50
	.word	43
	.word	36
	.word	29
	.word	22
	.word	15
	.word	58
	.word	51
	.word	44
	.word	37
	.word	30
	.word	23
	.word	59
	.word	52
	.word	45
	.word	38
	.word	31
	.word	60
	.word	53
	.word	46
	.word	39
	.word	61
	.word	54
	.word	47
	.word	62
	.word	55
	.word	63
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC2:
	.ascii	"max_bytes_per_pic_denom\000"
.LC3:
	.ascii	"\0016%s:%d: enter\012\000"
.LC4:
	.ascii	"vps->vps_num_hrd_parameters <= 1\000"
.LC5:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC6:
	.ascii	"vps->vps_max_nuh_reserved_zero_layer_id < 1\000"
.LC7:
	.ascii	"\0016%s:%d: write pure vps head size: %d bits\000"
.LC8:
	.ascii	"\0016%s:%d: leave\012\000"
.LC9:
	.ascii	"pcm_sample_bit_depth_chroma_minus1\000"
.LC10:
	.ascii	"sps->vps_max_sub_layers_minus1 + 1 > 0\000"
.LC11:
	.ascii	"num_short_term_ref_pic_sets\000"
.LC12:
	.ascii	"\0016%d: todo num_short_term_ref_pic_sets\000"
.LC13:
	.ascii	"\0016%s:%d: write pure sps head size: %d bits\000"
.LC14:
	.ascii	"tiles_enabled_flag\000"
.LC15:
	.ascii	"(pps->num_tile_columns_minus1 + pps->num_tile_rows_"
	.ascii	"minus1) != 0\000"
.LC16:
	.ascii	"\0016%s:%d: write pure pps size: %d bits\000"
.LC17:
	.ascii	"\0016%s:%d: nals total size: %d bytes\000"
.LC18:
	.ascii	"\0016%s:%d: get extra info nal type %d, size %d byt"
	.ascii	"es\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
