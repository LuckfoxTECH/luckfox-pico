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
	.file	"rga_utils.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_pixel_format_rk_to_rga:					
	.asciz 	"pixel_format_rk_to_rga"					
__kstrtabns_pixel_format_rk_to_rga:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_pixel_format_rga_to_rk:					
	.asciz 	"pixel_format_rga_to_rk"					
__kstrtabns_pixel_format_rga_to_rk:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_compress_mode_rk_to_rga:					
	.asciz 	"compress_mode_rk_to_rga"					
__kstrtabns_compress_mode_rk_to_rga:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_compress_mode_rga_to_rk:					
	.asciz 	"compress_mode_rga_to_rk"					
__kstrtabns_compress_mode_rga_to_rk:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_crop_rect_ratio_to_abs:					
	.asciz 	"crop_rect_ratio_to_abs"					
__kstrtabns_crop_rect_ratio_to_abs:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.global	pixel_format_rk_to_rga
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pixel_format_rk_to_rga, %function
pixel_format_rk_to_rga:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L6
	movs	r3, #0
.L4:
	ldr	r1, [r2, r3, lsl #3]
	cmp	r1, r0
	bne	.L2
	add	r3, r2, r3, lsl #3
	ldr	r0, [r3, #4]
	bx	lr
.L2:
	adds	r3, r3, #1
	cmp	r3, #19
	bne	.L4
	mov	r0, #-1
	bx	lr
.L7:
	.align	2
.L6:
	.word	.LANCHOR0
	.fnend
	.size	pixel_format_rk_to_rga, .-pixel_format_rk_to_rga
	.align	1
	.global	pixel_format_rga_to_rk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pixel_format_rga_to_rk, %function
pixel_format_rga_to_rk:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L13
	movs	r3, #0
.L11:
	add	r1, r2, r3, lsl #3
	ldr	r1, [r1, #4]
	cmp	r1, r0
	bne	.L9
	ldr	r0, [r2, r3, lsl #3]
	bx	lr
.L9:
	adds	r3, r3, #1
	cmp	r3, #19
	bne	.L11
	mov	r0, #-1
	bx	lr
.L14:
	.align	2
.L13:
	.word	.LANCHOR0
	.fnend
	.size	pixel_format_rga_to_rk, .-pixel_format_rga_to_rk
	.align	1
	.global	crop_rect_ratio_to_abs
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	crop_rect_ratio_to_abs, %function
crop_rect_ratio_to_abs:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	ldr	r6, [r1]
	ldr	r8, .L24
	muls	r6, r2, r6
	smull	r4, r5, r6, r8
	ldr	r4, [r1, #4]
	asrs	r6, r6, #31
	muls	r4, r3, r4
	rsb	r5, r6, r5, asr #6
	adds	r5, r5, #1
	bics	r5, r5, #1
	smull	r6, r7, r4, r8
	asr	r4, r4, #31
	rsb	r4, r4, r7, asr #6
	ldr	r7, [r1, #8]
	add	r4, r4, #1
	bic	r4, r4, #1
	mul	ip, r7, r2
	smull	r6, r7, ip, r8
	ldr	r6, [r1, #12]
	asr	ip, ip, #31
	mul	r6, r6, r3
	rsb	r7, ip, r7, asr #6
	add	r7, r7, #1
	bic	r7, r7, #1
	smull	r8, r9, r6, r8
	asr	r1, r6, #31
	rsb	r1, r1, r9, asr #6
	add	r1, r1, #1
	bic	r1, r1, #1
	bmi	.L20
	cmp	r5, r2
	it	ge
	movge	r5, r2
.L16:
	cmp	r4, #0
	blt	.L21
	cmp	r4, r3
	it	ge
	movge	r4, r3
.L17:
	cmp	r7, #0
	blt	.L22
	subs	r2, r2, r5
	cmp	r7, r2
	it	ge
	movge	r7, r2
.L18:
	cmp	r1, #0
	blt	.L23
	subs	r3, r3, r4
	cmp	r1, r3
	it	ge
	movge	r1, r3
.L19:
	strd	r5, r4, [r0]
	strd	r7, r1, [r0, #8]
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L20:
	movs	r5, #0
	b	.L16
.L21:
	movs	r4, #0
	b	.L17
.L22:
	movs	r7, #0
	b	.L18
.L23:
	movs	r1, #0
	b	.L19
.L25:
	.align	2
.L24:
	.word	274877907
	.fnend
	.size	crop_rect_ratio_to_abs, .-crop_rect_ratio_to_abs
	.align	1
	.global	compress_mode_rk_to_rga
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	compress_mode_rk_to_rga, %function
compress_mode_rk_to_rga:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbz	r0, .L27
	cmp	r0, #1
	bne	.L29
.L27:
	ldr	r3, .L33
	add	r3, r3, r0, lsl #3
	ldr	r0, [r3, #156]
	bx	lr
.L29:
	mov	r0, #-1
	bx	lr
.L34:
	.align	2
.L33:
	.word	.LANCHOR0
	.fnend
	.size	compress_mode_rk_to_rga, .-compress_mode_rk_to_rga
	.align	1
	.global	compress_mode_rga_to_rk
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	compress_mode_rga_to_rk, %function
compress_mode_rga_to_rk:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #1
	beq	.L38
	cmp	r0, #2
	bne	.L39
	movs	r2, #1
.L36:
	ldr	r3, .L40
	add	r3, r3, r2, lsl #3
	ldr	r0, [r3, #152]
	bx	lr
.L38:
	movs	r2, #0
	b	.L36
.L39:
	mov	r0, #-1
	bx	lr
.L41:
	.align	2
.L40:
	.word	.LANCHOR0
	.fnend
	.size	compress_mode_rga_to_rk, .-compress_mode_rga_to_rk
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	srga_format_map, %object
	.size	srga_format_map, 152
srga_format_map:
	.word	4
	.word	11
	.word	0
	.word	10
	.word	6
	.word	14
	.word	11
	.word	21
	.word	7
	.word	9
	.word	2
	.word	8
	.word	10
	.word	30
	.word	9
	.word	28
	.word	17
	.word	24
	.word	18
	.word	26
	.word	65536
	.word	4
	.word	65537
	.word	36
	.word	65542
	.word	2
	.word	65543
	.word	7
	.word	65552
	.word	40
	.word	65553
	.word	44
	.word	65554
	.word	3
	.word	65555
	.word	0
	.word	65558
	.word	38
	.type	srga_compress_mode_map, %object
	.size	srga_compress_mode_map, 16
srga_compress_mode_map:
	.word	0
	.word	1
	.word	1
	.word	2
	.section	___ksymtab_gpl+compress_mode_rga_to_rk,"a"
	.align	2
	.type	__ksymtab_compress_mode_rga_to_rk, %object
	.size	__ksymtab_compress_mode_rga_to_rk, 12
__ksymtab_compress_mode_rga_to_rk:
	.word	compress_mode_rga_to_rk
	.word	__kstrtab_compress_mode_rga_to_rk
	.word	__kstrtabns_compress_mode_rga_to_rk
	.section	___ksymtab_gpl+compress_mode_rk_to_rga,"a"
	.align	2
	.type	__ksymtab_compress_mode_rk_to_rga, %object
	.size	__ksymtab_compress_mode_rk_to_rga, 12
__ksymtab_compress_mode_rk_to_rga:
	.word	compress_mode_rk_to_rga
	.word	__kstrtab_compress_mode_rk_to_rga
	.word	__kstrtabns_compress_mode_rk_to_rga
	.section	___ksymtab_gpl+crop_rect_ratio_to_abs,"a"
	.align	2
	.type	__ksymtab_crop_rect_ratio_to_abs, %object
	.size	__ksymtab_crop_rect_ratio_to_abs, 12
__ksymtab_crop_rect_ratio_to_abs:
	.word	crop_rect_ratio_to_abs
	.word	__kstrtab_crop_rect_ratio_to_abs
	.word	__kstrtabns_crop_rect_ratio_to_abs
	.section	___ksymtab_gpl+pixel_format_rga_to_rk,"a"
	.align	2
	.type	__ksymtab_pixel_format_rga_to_rk, %object
	.size	__ksymtab_pixel_format_rga_to_rk, 12
__ksymtab_pixel_format_rga_to_rk:
	.word	pixel_format_rga_to_rk
	.word	__kstrtab_pixel_format_rga_to_rk
	.word	__kstrtabns_pixel_format_rga_to_rk
	.section	___ksymtab_gpl+pixel_format_rk_to_rga,"a"
	.align	2
	.type	__ksymtab_pixel_format_rk_to_rga, %object
	.size	__ksymtab_pixel_format_rk_to_rga, 12
__ksymtab_pixel_format_rk_to_rga:
	.word	pixel_format_rk_to_rga
	.word	__kstrtab_pixel_format_rk_to_rga
	.word	__kstrtabns_pixel_format_rk_to_rga
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
