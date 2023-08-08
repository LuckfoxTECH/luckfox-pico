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
	.file	"vepu_pp.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_register_vmpibuf_func_to_pp:					
	.asciz 	"register_vmpibuf_func_to_pp"					
__kstrtabns_register_vmpibuf_func_to_pp:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_unregister_vmpibuf_func_pp:					
	.asciz 	"unregister_vmpibuf_func_pp"					
__kstrtabns_unregister_vmpibuf_func_pp:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vepu_pp_create_chn:					
	.asciz 	"vepu_pp_create_chn"					
__kstrtabns_vepu_pp_create_chn:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vepu_pp_destroy_chn:					
	.asciz 	"vepu_pp_destroy_chn"					
__kstrtabns_vepu_pp_destroy_chn:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vepu_pp_control:					
	.asciz 	"vepu_pp_control"					
__kstrtabns_vepu_pp_control:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	IS_ERR_OR_NULL, %function
IS_ERR_OR_NULL:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbz	r0, .L3
	cmn	r0, #4096
	ite	ls
	movls	r0, #0
	movhi	r0, #1
	bx	lr
.L3:
	movs	r0, #1
	bx	lr
	.fnend
	.size	IS_ERR_OR_NULL, .-IS_ERR_OR_NULL
	.text
	.align	1
	.global	register_vmpibuf_func_to_pp
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	register_vmpibuf_func_to_pp, %function
register_vmpibuf_func_to_pp:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L5
	str	r0, [r3]
	bx	lr
.L6:
	.align	2
.L5:
	.word	.LANCHOR0
	.fnend
	.size	register_vmpibuf_func_to_pp, .-register_vmpibuf_func_to_pp
	.align	1
	.global	unregister_vmpibuf_func_pp
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	unregister_vmpibuf_func_pp, %function
unregister_vmpibuf_func_pp:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L8
	movs	r2, #0
	str	r2, [r3]
	bx	lr
.L9:
	.align	2
.L8:
	.word	.LANCHOR0
	.fnend
	.size	unregister_vmpibuf_func_pp, .-unregister_vmpibuf_func_pp
	.align	1
	.global	vepu_pp_control
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_control, %function
vepu_pp_control:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #1
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r5, r0
	.pad #28
	sub	sp, sp, #28
	mov	r6, r2
	ble	.L11
	mov	r3, r0
	movw	r2, #391
	ldr	r1, .L87
	ldr	r0, .L87+4
	bl	printk
	mov	r0, #-1
.L10:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L11:
	mov	r10, #636
	ldr	r3, .L87+8
	mul	r10, r10, r0
	cmp	r1, #16
	add	r8, r3, #4
	mov	r7, r3
	add	r4, r8, r10
	beq	.L13
	cmp	r1, #32
	beq	.L14
	cmp	r1, #48
	beq	.L15
	cmp	r1, #64
	beq	.L16
	cmp	r1, #80
	bne	.L82
	ldr	r3, [r4, #160]
	mov	r0, #16777472
	orr	r3, r3, #66846720
	orr	r3, r3, #65536
	str	r3, [r4, #160]
	mov	r3, #8388736
	str	r3, [r4, #164]
	add	r3, r3, #8388608
	str	r3, [r4, #168]
	mov	r3, #6291552
	strd	r0, r3, [r4, #172]
	sub	r3, r3, #5767168
	str	r3, [r4, #180]
	mov	r3, #524296
	str	r3, [r4, #184]
	b	.L86
.L13:
	add	r10, r10, r3
	ldr	r8, [r2, #8]
	ldr	r9, [r2, #12]
	ldrd	r3, ip, [r10, #32]
	cmp	r8, #0
	ldr	fp, [r2]
	clz	r3, r3
	ldrb	r2, [r4, #60]	@ zero_extendqisi2
	it	eq
	moveq	r8, #30
	cmp	r9, #0
	lsr	r3, r3, #5
	bfi	r2, r3, #0, #1
	strb	r2, [r4, #60]
	ldr	r2, [r10, #16]
	it	eq
	moveq	r9, #1
	cbnz	r2, .L21
	ldr	r3, [r10, #20]
	cbz	r3, .L22
.L21:
	mov	r1, #636
	add	r3, r8, #-1
	mla	r1, r1, r5, r7
	ldr	r1, [r1, #40]
	subs	r3, r1, r3
	it	ne
	movne	r3, #1
.L22:
	ldrb	r1, [r4, #60]	@ zero_extendqisi2
	bfi	r1, r3, #1, #1
	mov	r3, #636
	mla	r3, r3, r5, r7
	strb	r1, [r4, #60]
	ldr	r0, [r3, #24]
	cbnz	r0, .L23
	ldr	r3, [r3, #28]
	cbz	r3, .L24
.L23:
	clz	r3, ip
	lsrs	r3, r3, #5
.L24:
	ldrb	r1, [r4, #60]	@ zero_extendqisi2
	ldrh	r10, [r4, #66]
	bfi	r1, r3, #2, #1
	mov	r3, #636
	mla	r3, r3, r5, r7
	strb	r1, [r4, #60]
	ldrh	r1, [r4, #64]
	ldr	r7, [r3, #8]
	lsr	lr, r7, #3
	add	lr, lr, #-1
	ubfx	lr, lr, #0, #11
	bfi	r1, lr, #0, #11
	strh	r1, [r4, #64]	@ movhi
	ldr	r1, [r3, #12]
	lsrs	r1, r1, #3
	subs	r1, r1, #1
	bfi	r10, r1, #0, #11
	strh	r10, [r4, #66]	@ movhi
	ldrb	r10, [r4, #68]	@ zero_extendqisi2
	ldr	r1, [r6, #4]
	bfi	r10, r1, #0, #4
	ldr	r1, [r4, #72]
	strb	r10, [r4, #68]
	bic	r1, r1, #4128831
	str	r1, [r4, #72]
	ldr	r1, [r4, #76]
	and	r1, r1, #-1073692672
	str	r1, [r4, #76]
	ldr	r1, [r4, #80]
	bfi	r1, r7, #0, #17
	lsrs	r7, r7, #1
	str	r1, [r4, #80]
	strh	r7, [r4, #84]	@ movhi
	cbz	r2, .L25
	ldr	r2, [r3, #40]
	adds	r2, r2, #0
	it	ne
	movne	r2, #1
.L25:
	ldrb	r3, [r4, #104]	@ zero_extendqisi2
	ldr	r7, .L87+8
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #104]
	ands	r3, r3, #1
	beq	.L26
	mov	r3, #636
	mla	r3, r3, r5, r7
	ldr	r3, [r3, #40]
	adds	r3, r3, #-1
	it	ne
	movne	r3, #1
.L26:
	ldrb	r2, [r4, #104]	@ zero_extendqisi2
	bfi	r2, r3, #1, #1
	add	r3, lr, #4
	strb	r2, [r4, #104]
	mov	r2, #636
	mla	r2, r2, r5, r7
	asrs	r3, r3, #2
	adds	r3, r3, #7
	asrs	r3, r3, #3
	lsls	r3, r3, #4
	str	r3, [r4, #156]
	ldr	r3, [r2, #20]
	cbz	r3, .L27
	ldr	r3, [r2, #40]
	adds	r3, r3, #0
	it	ne
	movne	r3, #1
.L27:
	cmp	fp, #0
	str	r3, [r4, #160]
	ite	le
	movle	r1, #0
	movgt	r1, #1
	cbz	r0, .L28
	cmp	ip, #0
	ite	eq
	moveq	r0, r1
	movne	r0, #0
.L28:
	ldrb	r2, [r4, #188]	@ zero_extendqisi2
	bfi	r2, r0, #0, #1
	strb	r2, [r4, #188]
	ands	r2, r2, #1
	beq	.L29
	cmp	fp, r9
	ite	le
	movle	r2, #0
	movgt	r2, #1
.L29:
	ldrb	r3, [r4, #188]	@ zero_extendqisi2
	bfi	r3, r2, #1, #1
	strb	r3, [r4, #188]
	mov	r3, #636
	mla	r3, r3, r5, r7
	ldr	r3, [r3, #28]
	cbz	r3, .L30
	clz	r3, ip
	lsrs	r3, r3, #5
.L30:
	mov	r0, #636
	orr	r3, r3, r1, lsl #1
	mla	r0, r0, r5, r7
	orr	r3, r3, #4
	str	r3, [r4, #200]
	movs	r2, #0
	ldr	r1, [r6, #20]
	ldr	r3, [r0, #632]
	ldrd	r10, fp, [r0, #8]
	ldr	r0, [r0, #636]
	ldr	r3, [r3, #32]
	blx	r3
	ldr	r3, [r6, #4]
	cmp	r3, #6
	beq	.L31
	cmp	r3, #7
	bne	.L32
	mul	r10, fp, r10
	add	r3, r0, r10
	str	r3, [sp]
	add	r3, r3, r10, lsr #2
.L83:
	str	r3, [sp, #4]
.L32:
	ldr	r3, [sp]
	str	r0, [r4, #584]
	str	r3, [r4, #588]
	ldr	r3, [sp, #4]
	str	r3, [r4, #592]
	ldr	r3, [r6]
	tst	r3, #1
	mov	r3, #636
	mla	r3, r3, r5, r7
	itete	eq
	ldreq	r2, [r3, #48]
	ldrne	r2, [r3, #52]
	ldreq	r2, [r2, #8]
	ldrne	r2, [r2, #8]
	itete	eq
	streq	r2, [r4, #556]
	strne	r2, [r4, #556]
	ldreq	r3, [r3, #52]
	ldrne	r3, [r3, #48]
	ldr	r3, [r3, #8]
	strd	r3, r3, [r4, #560]
	mov	r3, #636
	mla	r1, r3, r5, r7
	mla	r5, r3, r5, r7
	ldr	r2, [r1, #36]
	ldr	r3, [r5, #40]
	adds	r2, r2, #1
	cmp	r9, r2
	add	r3, r3, #1
	it	eq
	moveq	r2, #0
	cmp	r8, r3
	str	r2, [r1, #36]
	bne	.L37
	movs	r3, #0
.L37:
	str	r3, [r5, #40]
	b	.L86
.L31:
	mla	r3, fp, r10, r0
	str	r3, [sp]
	b	.L83
.L88:
	.align	2
.L87:
	.word	.LANCHOR1
	.word	.LC0
	.word	.LANCHOR0
.L14:
	ldr	r1, [r6, #4]
	add	r10, r10, r3
	ldrb	r2, [r4, #188]	@ zero_extendqisi2
	bfi	r2, r1, #2, #2
	strb	r2, [r4, #188]
	ldr	r1, [r6, #8]
	ldrh	r2, [r4, #188]
	bfi	r2, r1, #4, #12
	strh	r2, [r4, #188]	@ movhi
	ldrb	r2, [r4, #190]	@ zero_extendqisi2
	ldr	r1, [r6, #12]
	bfi	r2, r1, #0, #3
	strb	r2, [r4, #190]
	ldrb	r2, [r4, #192]	@ zero_extendqisi2
	ldr	r1, [r6, #16]
	bfi	r2, r1, #0, #1
	strb	r2, [r4, #192]
	ldr	r1, [r6, #20]
	bfi	r2, r1, #1, #1
	strb	r2, [r4, #192]
	ldr	r2, [r4, #192]
	bic	r2, r2, #130560
	bic	r2, r2, #508
	orr	r2, r2, #25728
	orr	r2, r2, #76
	str	r2, [r4, #192]
	movs	r2, #64
	str	r2, [r4, #196]
	ldr	r3, [r10, #56]
	movs	r2, #0
	ldr	r3, [r3, #8]
	strd	r3, r3, [r4, #576]
	ldr	r3, [r10, #632]
	ldr	r1, [r6]
	ldr	r0, [r10, #636]
	ldr	r3, [r3, #32]
	blx	r3
	str	r0, [r4, #600]
.L86:
	movs	r0, #0
	b	.L10
.L15:
	ldr	r2, [r2, #4]
	ldrh	r3, [r4, #204]
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #204]	@ movhi
	ldr	r2, [r6, #8]
	ldr	r3, [r4, #204]
	bfi	r3, r2, #12, #18
	str	r3, [r4, #204]
	ldrh	r3, [r4, #208]
	movs	r2, #7
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #208]	@ movhi
	ldr	r3, [r4, #208]
	ldr	r2, [r6, #8]
	bfi	r3, r2, #12, #18
	str	r3, [r4, #208]
	b	.L86
.L16:
	add	r10, r10, r3
	ldr	r1, [r6]
	movs	r2, #0
	ldr	r5, .L89
	ldr	r3, [r10, #632]
	mov	r7, #1572864
	ldr	r0, [r10, #636]
	ldr	r6, .L89+4
	ldr	r3, [r3, #32]
	blx	r3
	ldr	ip, .L89+28
	str	r0, [r4, #596]
	ldr	r3, [r10, #60]
	ldr	r3, [r3, #8]
	strd	r3, r3, [r4, #568]
	ldr	r3, .L89+8
	strd	r5, r3, [r4, #108]
	movw	r3, #1798
	strd	r6, r3, [r4, #116]
	ldr	r3, .L89+12
	strd	r7, r3, [r4, #124]
	sub	r3, r3, #2097184
	str	r3, [r4, #132]
	ldr	r3, .L89+16
	str	r3, [r4, #136]
	sub	r3, r3, #4718592
	str	r3, [r4, #140]
	ldr	r3, .L89+20
	strd	ip, r3, [r4, #144]
	ldr	r3, .L89+24
	str	r3, [r4, #152]
	ldr	r3, [r10, #44]
	str	r3, [r4, #156]
	b	.L86
.L82:
	cmp	r1, #96
	bne	.L40
	add	r2, r7, r10
	add	r3, r10, #60
	add	r3, r3, r8
	mov	r1, #544
	str	r3, [sp, #12]
	movs	r3, #0
	strd	r1, r3, [sp, #16]
	ldr	r3, [r2, #632]
	ldr	r3, [r3, #16]
	cbz	r3, .L41
	add	r1, sp, #12
	ldr	r0, [r2, #636]
	blx	r3
.L41:
	add	r10, r10, #604
	movs	r2, #24
	add	r3, r8, r10
	str	r3, [sp, #12]
	movs	r3, #0
	strd	r2, r3, [sp, #16]
	mov	r3, #636
	mla	r3, r3, r5, r7
	ldr	r2, [r3, #632]
	ldr	r2, [r2, #20]
	cbz	r2, .L42
	add	r1, sp, #12
	ldr	r0, [r3, #636]
	blx	r2
.L42:
	mov	r3, #636
	mla	r1, r3, r5, r7
	mov	r4, r3
	ldr	r2, [r1, #632]
	ldr	r2, [r2, #24]
	cbz	r2, .L43
	ldr	r0, [r1, #636]
	blx	r2
.L43:
	mla	r3, r4, r5, r7
	ldr	r2, [r3, #632]
	ldr	r2, [r2, #28]
	cmp	r2, #0
	beq	.L86
	ldr	r0, [r3, #636]
	blx	r2
	b	.L86
.L40:
	cmp	r1, #112
	bne	.L45
	add	r10, r10, r3
	ldr	r3, [r10, #624]
	str	r3, [r2]
	ldr	r3, [r10, #628]
	str	r3, [r2, #4]
	b	.L86
.L45:
	cmp	r1, #128
	bne	.L86
	add	r10, r10, r3
	ldr	r3, [r10, #608]
	str	r3, [r6]
	ldr	r3, [r10, #612]
	str	r3, [r6, #4]
	ldr	r3, [r10, #616]
	str	r3, [r6, #8]
	ldr	r3, [r10, #620]
	str	r3, [r6, #12]
	b	.L86
.L90:
	.align	2
.L89:
	.word	134940683
	.word	151389702
	.word	101717012
	.word	4194352
	.word	6291504
	.word	1094799954
	.word	3158064
	.word	3145824
	.fnend
	.size	vepu_pp_control, .-vepu_pp_control
	.section	.text.unlikely
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_vmpibuf_func, %function
get_vmpibuf_func:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r3, .L96
	ldr	r3, [r3]
	mov	r0, r3
	bl	IS_ERR_OR_NULL
	cbz	r0, .L91
	ldr	r1, .L96+4
	ldr	r0, .L96+8
	bl	printk
	mvn	r3, #11
.L91:
	mov	r0, r3
	pop	{r3, pc}
.L97:
	.align	2
.L96:
	.word	.LANCHOR0
	.word	.LANCHOR1+16
	.word	.LC1
	.fnend
	.size	get_vmpibuf_func, .-get_vmpibuf_func
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_malloc_buffer, %function
pp_malloc_buffer:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r8, r1
	mov	r5, r0
	bl	get_vmpibuf_func
	mov	r7, r0
	movs	r0, #12
	bl	vmalloc
	mov	r4, r0
	bl	IS_ERR_OR_NULL
	mov	r6, r0
	cbz	r0, .L99
	movs	r2, #49
	ldr	r1, .L113
	ldr	r0, .L113+4
	mvn	r4, #11
	bl	printk
.L98:
	mov	r0, r4
	pop	{r4, r5, r6, r7, r8, pc}
.L99:
	ldr	r3, [r7]
	cmp	r3, #0
	beq	.L98
	mov	r0, r8
	blx	r3
	str	r0, [r4]
	cmp	r0, #0
	beq	.L98
	ldr	r3, [r7, #20]
	cmp	r3, #0
	beq	.L98
	blx	r3
	mov	r1, r0
	str	r0, [r4, #4]
	cmp	r0, #0
	beq	.L98
	ldr	r3, [r5, #628]
	mov	r2, r6
	ldr	r0, [r5, #632]
	ldr	r3, [r3, #32]
	blx	r3
	str	r0, [r4, #8]
	b	.L98
.L114:
	.align	2
.L113:
	.word	.LANCHOR1+33
	.word	.LC2
	.fnend
	.size	pp_malloc_buffer, .-pp_malloc_buffer
	.align	1
	.global	vepu_pp_create_chn
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_create_chn, %function
vepu_pp_create_chn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r2, r0
	mov	r6, r0
	mov	r5, r1
	ldr	r0, .L144
	ldr	r1, .L144+4
	bl	printk
	cmp	r6, #1
	ble	.L116
	mov	r3, r6
	movs	r2, #165
	ldr	r1, .L144+8
	ldr	r0, .L144+12
	bl	printk
.L143:
	mov	r10, #-1
.L115:
	mov	r0, r10
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L116:
	mov	r2, #636
	ldr	r7, .L144+16
	mul	r4, r2, r6
	add	r8, r7, #4
	movs	r1, #0
	add	r8, r8, r4
	add	r4, r4, r7
	mov	r0, r8
	bl	memset
	str	r6, [r4, #4]
	ldr	r3, [r5]
	str	r3, [r4, #8]
	ldr	r3, [r5, #4]
	str	r3, [r4, #12]
	ldr	r3, [r5, #8]
	str	r3, [r4, #16]
	ldr	r3, [r5, #12]
	str	r3, [r4, #20]
	ldr	r3, [r5, #16]
	str	r3, [r4, #24]
	ldr	r3, [r5, #20]
	str	r3, [r4, #28]
	ldr	r3, [r5, #24]
	str	r3, [r4, #32]
	ldr	r3, .L144+20
	str	r3, [r4, #632]
	ldr	r0, [r3, #4]
	bl	vmalloc
	str	r0, [r4, #636]
	cbnz	r0, .L118
	movs	r2, #184
	ldr	r1, .L144+8
	ldr	r0, .L144+24
	bl	printk
	b	.L143
.L118:
	ldr	r3, [r4, #632]
	movs	r1, #0
	ldr	r2, [r3, #4]
	bl	memset
	ldr	r3, [r4, #632]
	ldr	r0, [r4, #636]
	movs	r1, #25
	ldr	r3, [r3, #8]
	blx	r3
	ldr	fp, [r4, #32]
	ldrd	r5, r4, [r4, #8]
	adds	r5, r5, #31
	bic	r5, r5, #31
	cmp	fp, #0
	beq	.L119
	adds	r4, r4, #31
	bic	r4, r4, #31
.L120:
	asrs	r5, r5, #3
	asrs	r4, r4, #3
	cmp	fp, #0
	beq	.L121
	asrs	r3, r5, #1
	asr	r9, r4, #1
.L122:
	mul	r9, r9, r3
	mov	r0, r8
	lsl	r1, r9, #4
	bl	pp_malloc_buffer
	mov	r3, #636
	mla	r3, r3, r6, r7
	strd	r0, r0, [r3, #48]
	bl	IS_ERR_OR_NULL
	cmp	r0, #0
	beq	.L130
	movs	r2, #106
	ldr	r1, .L144+28
	ldr	r0, .L144+32
	mov	r10, #-1
	bl	printk
.L123:
	mov	r3, #636
	mla	r3, r3, r6, r7
	ldr	r3, [r3, #24]
	cbz	r3, .L124
	cmp	fp, #0
	beq	.L125
	asrs	r1, r5, #2
	asrs	r4, r4, #2
.L126:
	muls	r1, r4, r1
	mov	r0, r8
	adds	r1, r1, #63
	asrs	r1, r1, #6
	lsls	r1, r1, #4
	bl	pp_malloc_buffer
	mov	r3, #636
	mla	r3, r3, r6, r7
	str	r0, [r3, #56]
	bl	IS_ERR_OR_NULL
	cbz	r0, .L124
	movs	r2, #122
	ldr	r1, .L144+28
	ldr	r0, .L144+32
	mov	r10, #-1
	bl	printk
.L124:
	mov	r4, #636
	mla	r4, r4, r6, r7
	ldr	r3, [r4, #16]
	cmp	r3, #0
	beq	.L115
	add	r1, r9, #127
	mov	r0, r8
	asrs	r1, r1, #7
	lsls	r1, r1, #4
	bl	pp_malloc_buffer
	str	r0, [r4, #60]
	bl	IS_ERR_OR_NULL
	cbz	r0, .L127
	movs	r2, #139
	ldr	r1, .L144+28
	ldr	r0, .L144+32
	mov	r10, #-1
	bl	printk
.L127:
	adds	r2, r5, #3
	mov	r3, #636
	it	mi
	addmi	r2, r5, #6
	mla	r6, r3, r6, r7
	asrs	r2, r2, #2
	adds	r3, r2, #7
	it	mi
	addmi	r3, r2, #14
	asrs	r3, r3, #3
	lsls	r3, r3, #4
	str	r3, [r6, #44]
	b	.L115
.L119:
	adds	r4, r4, #31
	lsls	r5, r5, #2
	bic	r4, r4, #31
	lsls	r4, r4, #2
	b	.L120
.L121:
	add	r9, r5, #15
	asr	r9, r9, #4
	lsl	r3, r9, #3
	add	r9, r4, #15
	asr	r9, r9, #4
	lsl	r9, r9, #3
	b	.L122
.L130:
	mov	r10, r0
	b	.L123
.L125:
	add	r1, r5, #15
	adds	r4, r4, #15
	asrs	r1, r1, #4
	asrs	r4, r4, #4
	lsls	r1, r1, #2
	lsls	r4, r4, #2
	b	.L126
.L145:
	.align	2
.L144:
	.word	.LC3
	.word	.LANCHOR1+50
	.word	.LANCHOR1+69
	.word	.LC4
	.word	.LANCHOR0
	.word	pp_srv_api
	.word	.LC5
	.word	.LANCHOR1+88
	.word	.LC2
	.fnend
	.size	vepu_pp_create_chn, .-vepu_pp_create_chn
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_free_buffer, %function
pp_free_buffer:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	mov	r4, r1
	bl	get_vmpibuf_func
	mov	r6, r0
	cbz	r4, .L146
	ldr	r1, [r4, #4]
	cbz	r1, .L148
	ldr	r3, [r5, #628]
	ldr	r0, [r5, #632]
	ldr	r3, [r3, #36]
	blx	r3
.L148:
	ldr	r0, [r4]
	cbz	r0, .L149
	ldr	r3, [r6, #16]
	blx	r3
.L149:
	mov	r0, r4
	pop	{r4, r5, r6, lr}
	b	vfree
.L146:
	pop	{r4, r5, r6, pc}
	.fnend
	.size	pp_free_buffer, .-pp_free_buffer
	.align	1
	.global	vepu_pp_destroy_chn
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_destroy_chn, %function
vepu_pp_destroy_chn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r2, r0
	mov	r4, r0
	ldr	r1, .L168
	ldr	r0, .L168+4
	bl	printk
	cmp	r4, #1
	ble	.L157
	mov	r3, r4
	movs	r2, #202
	ldr	r1, .L168+8
	ldr	r0, .L168+12
	bl	printk
	mov	r0, #-1
.L156:
	pop	{r3, r4, r5, r6, r7, pc}
.L157:
	mov	r5, #636
	ldr	r7, .L168+16
	muls	r5, r4, r5
	adds	r6, r7, #4
	add	r6, r6, r5
	add	r5, r5, r7
	mov	r0, r6
	ldr	r1, [r5, #48]
	bl	pp_free_buffer
	ldr	r3, [r5, #24]
	cbz	r3, .L159
	ldr	r1, [r5, #56]
	mov	r0, r6
	bl	pp_free_buffer
.L159:
	mov	r3, #636
	mla	r3, r3, r4, r7
	ldr	r2, [r3, #16]
	cbz	r2, .L160
	ldr	r1, [r3, #60]
	mov	r0, r6
	bl	pp_free_buffer
.L160:
	mov	r3, #636
	movs	r5, #0
	mla	r4, r3, r4, r7
	ldr	r3, [r4, #632]
	ldr	r0, [r4, #636]
	ldr	r3, [r3, #12]
	blx	r3
	ldr	r0, [r4, #636]
	cmp	r0, #0
	beq	.L156
	bl	vfree
	str	r5, [r4, #636]
	mov	r0, r5
	b	.L156
.L169:
	.align	2
.L168:
	.word	.LANCHOR1+107
	.word	.LC3
	.word	.LANCHOR1+127
	.word	.LC6
	.word	.LANCHOR0
	.fnend
	.size	vepu_pp_destroy_chn, .-vepu_pp_destroy_chn
	.section	.rodata
	.set	.LANCHOR1,. + 0
	.type	__func__.18509, %object
	.size	__func__.18509, 16
__func__.18509:
	.ascii	"vepu_pp_control\000"
	.type	__FUNCTION__.18407, %object
	.size	__FUNCTION__.18407, 17
__FUNCTION__.18407:
	.ascii	"get_vmpibuf_func\000"
	.type	__func__.18414, %object
	.size	__func__.18414, 17
__func__.18414:
	.ascii	"pp_malloc_buffer\000"
	.type	__FUNCTION__.18441, %object
	.size	__FUNCTION__.18441, 19
__FUNCTION__.18441:
	.ascii	"vepu_pp_create_chn\000"
	.type	__func__.18442, %object
	.size	__func__.18442, 19
__func__.18442:
	.ascii	"vepu_pp_create_chn\000"
	.type	__func__.18432, %object
	.size	__func__.18432, 19
__func__.18432:
	.ascii	"pp_allocate_buffer\000"
	.type	__FUNCTION__.18453, %object
	.size	__FUNCTION__.18453, 20
__FUNCTION__.18453:
	.ascii	"vepu_pp_destroy_chn\000"
	.type	__func__.18454, %object
	.size	__func__.18454, 20
__func__.18454:
	.ascii	"vepu_pp_destroy_chn\000"
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_mpi_buf_fn, %object
	.size	g_mpi_buf_fn, 4
g_mpi_buf_fn:
	.space	4
	.type	g_pp_ctx, %object
	.size	g_pp_ctx, 1272
g_pp_ctx:
	.space	1272
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: vepu pp control channel id %d error\012"
	.ascii	"\000"
.LC1:
	.ascii	"\0013%s failed\012\000"
.LC2:
	.ascii	"\0013%s:%d: failed\012\000"
.LC3:
	.ascii	"\0016%s %d\012\000"
.LC4:
	.ascii	"\0013%s:%d: vepu pp create channel id %d error\012\000"
.LC5:
	.ascii	"\0013%s:%d: vepu pp vmalloc failed\012\000"
.LC6:
	.ascii	"\0013%s:%d: vepu pp destroy channel id %d error\012"
	.ascii	"\000"
	.section	___ksymtab+register_vmpibuf_func_to_pp,"a"
	.align	2
	.type	__ksymtab_register_vmpibuf_func_to_pp, %object
	.size	__ksymtab_register_vmpibuf_func_to_pp, 12
__ksymtab_register_vmpibuf_func_to_pp:
	.word	register_vmpibuf_func_to_pp
	.word	__kstrtab_register_vmpibuf_func_to_pp
	.word	__kstrtabns_register_vmpibuf_func_to_pp
	.section	___ksymtab+unregister_vmpibuf_func_pp,"a"
	.align	2
	.type	__ksymtab_unregister_vmpibuf_func_pp, %object
	.size	__ksymtab_unregister_vmpibuf_func_pp, 12
__ksymtab_unregister_vmpibuf_func_pp:
	.word	unregister_vmpibuf_func_pp
	.word	__kstrtab_unregister_vmpibuf_func_pp
	.word	__kstrtabns_unregister_vmpibuf_func_pp
	.section	___ksymtab+vepu_pp_control,"a"
	.align	2
	.type	__ksymtab_vepu_pp_control, %object
	.size	__ksymtab_vepu_pp_control, 12
__ksymtab_vepu_pp_control:
	.word	vepu_pp_control
	.word	__kstrtab_vepu_pp_control
	.word	__kstrtabns_vepu_pp_control
	.section	___ksymtab+vepu_pp_create_chn,"a"
	.align	2
	.type	__ksymtab_vepu_pp_create_chn, %object
	.size	__ksymtab_vepu_pp_create_chn, 12
__ksymtab_vepu_pp_create_chn:
	.word	vepu_pp_create_chn
	.word	__kstrtab_vepu_pp_create_chn
	.word	__kstrtabns_vepu_pp_create_chn
	.section	___ksymtab+vepu_pp_destroy_chn,"a"
	.align	2
	.type	__ksymtab_vepu_pp_destroy_chn, %object
	.size	__ksymtab_vepu_pp_destroy_chn, 12
__ksymtab_vepu_pp_destroy_chn:
	.word	vepu_pp_destroy_chn
	.word	__kstrtab_vepu_pp_destroy_chn
	.word	__kstrtabns_vepu_pp_destroy_chn
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
