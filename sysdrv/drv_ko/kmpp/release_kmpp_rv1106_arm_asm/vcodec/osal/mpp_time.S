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
	.file	"mpp_time.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_time
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_time, %function
mpp_time:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, fp, lr}
	.save {r4, r5, fp, lr}
	.pad #16
	mov	r0, sp
	bl	ktime_get_real_ts64
	ldr	r3, .L2
	ldr	r4, .L2+4
	ldr	r0, [sp]
	ldr	r2, [sp, #4]
	umull	fp, ip, r0, r3
	mla	ip, r3, r2, ip
	ldr	r3, [sp, #8]
	smull	r4, r5, r3, r4
	asrs	r3, r3, #31
	rsb	r3, r3, r5, asr #6
	adds	r0, fp, r3
	adc	r1, ip, r3, asr #31
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, fp, pc}
.L3:
	.align	2
.L2:
	.word	1000000
	.word	274877907
	.fnend
	.size	mpp_time, .-mpp_time
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
