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
	ldr	r3, .L15
	ldr	r3, [r3]
	mov	r0, r3
	bl	IS_ERR_OR_NULL
	cbz	r0, .L10
	ldr	r1, .L15+4
	ldr	r0, .L15+8
	bl	printk
	mvn	r3, #11
.L10:
	mov	r0, r3
	pop	{r3, pc}
.L16:
	.align	2
.L15:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	.LC0
	.fnend
	.size	get_vmpibuf_func, .-get_vmpibuf_func
	.align	1
	.global	vepu_pp_control
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vepu_pp_control, %function
vepu_pp_control:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #7
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r6, r0
	.pad #36
	sub	sp, sp, #36
	mov	r5, r2
	ble	.L18
	mov	r3, r0
	movw	r2, #427
	ldr	r1, .L103
	ldr	r0, .L103+4
	bl	printk
	mov	r0, #-1
.L17:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L18:
	mov	r7, #644
	ldr	r3, .L103+8
	muls	r7, r0, r7
	cmp	r1, #16
	mov	r9, r3
	add	r4, r7, r3
	beq	.L20
	cmp	r1, #32
	beq	.L21
	cmp	r1, #48
	beq	.L22
	cmp	r1, #64
	beq	.L23
	cmp	r1, #80
	beq	.L24
	cmp	r1, #17
	bne	.L25
	ldr	r3, [r2]
	str	r3, [r4, #4]
	ldr	r3, [r2, #4]
	str	r3, [r4, #8]
	ldr	r3, [r2, #8]
	str	r3, [r4, #20]
	ldr	r3, [r2, #12]
	str	r3, [r4, #24]
	ldr	r3, [r2, #16]
	str	r3, [r4, #28]
	ldr	r3, [r2, #20]
	str	r3, [r4, #32]
	ldr	r3, [r2, #24]
	str	r3, [r4, #36]
	movs	r3, #0
	strd	r3, r3, [r4, #40]
.L102:
	movs	r0, #0
	b	.L17
.L20:
	ldr	r3, [r2]
	ldr	r7, [r2, #8]
	ldr	r8, [r2, #12]
	str	r3, [sp, #8]
	cmp	r7, #0
	ldrd	r3, lr, [r4, #36]
	ldrb	r2, [r4, #68]	@ zero_extendqisi2
	it	eq
	moveq	r7, #30
	clz	r3, r3
	cmp	r8, #0
	lsr	r3, r3, #5
	it	eq
	moveq	r8, #1
	bfi	r2, r3, #0, #1
	strb	r2, [r4, #68]
	ldr	r2, [r4, #20]
	cbnz	r2, .L29
	ldr	r3, [r4, #24]
	cbz	r3, .L30
.L29:
	mov	r1, #644
	subs	r3, r7, #1
	mla	r1, r1, r6, r9
	ldr	r1, [r1, #44]
	subs	r3, r1, r3
	it	ne
	movne	r3, #1
.L30:
	ldrb	r1, [r4, #68]	@ zero_extendqisi2
	bfi	r1, r3, #1, #1
	mov	r3, #644
	mla	r3, r3, r6, r9
	strb	r1, [r4, #68]
	ldr	ip, [r3, #28]
	cmp	ip, #0
	bne	.L31
	ldr	r3, [r3, #32]
	cbz	r3, .L32
.L31:
	clz	r3, lr
	lsrs	r3, r3, #5
.L32:
	ldrb	r1, [r4, #68]	@ zero_extendqisi2
	bfi	r1, r3, #2, #1
	strb	r1, [r4, #68]
	mov	r1, #644
	ldrh	r3, [r4, #72]
	mla	r1, r1, r6, r9
	ldrh	r9, [r4, #74]
	ldr	r0, [r1, #4]
	lsr	fp, r0, #3
	add	fp, fp, #-1
	ubfx	fp, fp, #0, #11
	bfi	r3, fp, #0, #11
	strh	r3, [r4, #72]	@ movhi
	ldr	r3, [r1, #8]
	lsrs	r3, r3, #3
	subs	r3, r3, #1
	bfi	r9, r3, #0, #11
	strh	r9, [r4, #74]	@ movhi
	ldrb	r9, [r4, #76]	@ zero_extendqisi2
	ldr	r3, [r5, #4]
	bfi	r9, r3, #0, #4
	ldr	r3, [r4, #80]
	strb	r9, [r4, #76]
	bic	r3, r3, #4128831
	str	r3, [r4, #80]
	ldr	r3, [r4, #84]
	and	r3, r3, #-1073692672
	str	r3, [r4, #84]
	ldr	r3, [r4, #88]
	bfi	r3, r0, #0, #17
	lsrs	r0, r0, #1
	str	r3, [r4, #88]
	strh	r0, [r4, #92]	@ movhi
	cbz	r2, .L33
	ldr	r2, [r1, #44]
	adds	r2, r2, #0
	it	ne
	movne	r2, #1
.L33:
	ldrb	r3, [r4, #112]	@ zero_extendqisi2
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #112]
	ands	r2, r3, #1
	ldr	r3, .L103+8
	beq	.L34
	mov	r2, #644
	mla	r2, r2, r6, r3
	ldr	r2, [r2, #44]
	adds	r2, r2, #-1
	it	ne
	movne	r2, #1
.L34:
	ldrb	r1, [r4, #112]	@ zero_extendqisi2
	bfi	r1, r2, #1, #1
	add	r2, fp, #4
	strb	r1, [r4, #112]
	mov	r1, #644
	mla	r1, r1, r6, r3
	asrs	r2, r2, #2
	adds	r2, r2, #7
	asrs	r2, r2, #3
	lsls	r2, r2, #4
	str	r2, [r4, #164]
	ldr	r2, [r1, #24]
	cbz	r2, .L35
	ldr	r2, [r1, #44]
	adds	r2, r2, #0
	it	ne
	movne	r2, #1
.L35:
	str	r2, [r4, #168]
	ldr	r2, [sp, #8]
	cmp	r2, #0
	ite	le
	movle	r1, #0
	movgt	r1, #1
	cmp	ip, #0
	beq	.L36
	cmp	lr, #0
	ite	eq
	moveq	ip, r1
	movne	ip, #0
.L36:
	ldrb	r2, [r4, #196]	@ zero_extendqisi2
	bfi	r2, ip, #0, #1
	strb	r2, [r4, #196]
	ands	r2, r2, #1
	beq	.L37
	ldr	r2, [sp, #8]
	cmp	r2, r8
	ite	le
	movle	r2, #0
	movgt	r2, #1
.L37:
	ldrb	r0, [r4, #196]	@ zero_extendqisi2
	bfi	r0, r2, #1, #1
	mov	r2, #644
	mla	r2, r2, r6, r3
	strb	r0, [r4, #196]
	ldr	r2, [r2, #32]
	cbz	r2, .L38
	clz	r2, lr
	lsrs	r2, r2, #5
.L38:
	orr	r2, r2, r1, lsl #1
	orr	r2, r2, #4
	str	r2, [r4, #208]
	mov	r2, #644
	mla	r2, r2, r6, r3
	ldrd	fp, r9, [r2, #4]
	bl	get_vmpibuf_func
	ldr	r3, .L103+8
	ldr	r2, [r0, #24]
	cbz	r2, .L39
	ldr	r0, [r5, #20]
	blx	r2
	ldr	r3, .L103+8
	str	r0, [sp, #4]
.L39:
	ldr	r2, [r5, #4]
	cmp	r2, #6
	beq	.L40
	cmp	r2, #7
	bne	.L41
	mul	r9, r9, fp
	ldr	r2, [sp, #4]
	add	r10, r9, r2
	add	r2, r10, r9, lsr #2
	str	r2, [sp, #12]
.L41:
	ldr	r2, [sp, #4]
	strd	r2, r10, [r4, #592]
	ldr	r2, [sp, #12]
	str	r2, [r4, #600]
	ldr	r2, [r5]
	tst	r2, #1
	mov	r2, #644
	mla	r2, r2, r6, r3
	itete	eq
	ldreq	r1, [r2, #52]
	ldrne	r1, [r2, #56]
	ldreq	r1, [r1, #8]
	ldrne	r1, [r1, #8]
	itete	eq
	streq	r1, [r4, #564]
	strne	r1, [r4, #564]
	ldreq	r2, [r2, #56]
	ldrne	r2, [r2, #52]
	ldr	r2, [r2, #8]
	strd	r2, r2, [r4, #568]
	mov	r2, #644
	mla	r0, r2, r6, r3
	mla	r3, r2, r6, r3
	ldr	r1, [r0, #40]
	ldr	r2, [r3, #44]
	adds	r1, r1, #1
	cmp	r8, r1
	add	r2, r2, #1
	it	eq
	moveq	r1, #0
	cmp	r7, r2
	str	r1, [r0, #40]
	bne	.L46
	movs	r2, #0
.L46:
	str	r2, [r3, #44]
	b	.L102
.L40:
	ldr	r2, [sp, #4]
	mla	r10, r9, fp, r2
	str	r10, [sp, #12]
	b	.L41
.L104:
	.align	2
.L103:
	.word	.LANCHOR1+17
	.word	.LC1
	.word	g_pp_ctx
.L21:
	bl	get_vmpibuf_func
	ldr	r2, [r5, #4]
	ldrb	r3, [r4, #196]	@ zero_extendqisi2
	bfi	r3, r2, #2, #2
	strb	r3, [r4, #196]
	ldr	r2, [r5, #8]
	ldrh	r3, [r4, #196]
	bfi	r3, r2, #4, #12
	strh	r3, [r4, #196]	@ movhi
	ldrb	r3, [r4, #198]	@ zero_extendqisi2
	ldr	r2, [r5, #12]
	bfi	r3, r2, #0, #3
	strb	r3, [r4, #198]
	ldrb	r3, [r4, #200]	@ zero_extendqisi2
	ldr	r2, [r5, #16]
	bfi	r3, r2, #0, #1
	strb	r3, [r4, #200]
	ldr	r2, [r5, #20]
	bfi	r3, r2, #1, #1
	strb	r3, [r4, #200]
	ldr	r2, [r5, #24]
	bfi	r3, r2, #2, #4
	strb	r3, [r4, #200]
	ldr	r2, [r5, #28]
	ldrh	r3, [r4, #200]
	bfi	r3, r2, #6, #3
	strh	r3, [r4, #200]	@ movhi
	ldr	r2, [r5, #32]
	ldr	r3, [r4, #200]
	bfi	r3, r2, #9, #8
	str	r3, [r4, #200]
	movs	r3, #64
	str	r3, [r4, #204]
	ldr	r3, [r4, #60]
	ldr	r3, [r3, #8]
	strd	r3, r3, [r4, #584]
	ldr	r3, [r0, #24]
	cmp	r3, #0
	beq	.L102
	ldr	r0, [r5]
	blx	r3
	str	r0, [r4, #608]
	b	.L102
.L22:
	ldr	r2, [r2, #4]
	ldrh	r3, [r4, #212]
	bfi	r3, r2, #0, #12
	strh	r3, [r4, #212]	@ movhi
	ldr	r2, [r5, #8]
	ldr	r3, [r4, #212]
	bfi	r3, r2, #12, #18
	str	r3, [r4, #212]
	ldr	r3, [r4, #216]
	and	r3, r3, #-1073741824
	orr	r3, r3, #7
	str	r3, [r4, #216]
	b	.L102
.L23:
	bl	get_vmpibuf_func
	ldr	r3, [r0, #24]
	cbz	r3, .L48
	ldr	r0, [r5]
	blx	r3
	str	r0, [r4, #604]
.L48:
	mov	r1, #644
	ldr	r5, .L105
	mla	r6, r1, r6, r9
	ldr	r7, .L105+4
	mov	ip, #1572864
	ldr	r8, .L105+28
	ldr	r3, [r6, #64]
	ldr	r3, [r3, #8]
	strd	r3, r3, [r4, #576]
	ldr	r3, .L105+8
	strd	r5, r3, [r4, #116]
	movw	r3, #1798
	strd	r7, r3, [r4, #124]
	ldr	r3, .L105+12
	strd	ip, r3, [r4, #132]
	sub	r3, r3, #2097184
	str	r3, [r4, #140]
	ldr	r3, .L105+16
	str	r3, [r4, #144]
	sub	r3, r3, #4718592
	str	r3, [r4, #148]
	ldr	r3, .L105+20
	strd	r8, r3, [r4, #152]
	ldr	r3, .L105+24
	str	r3, [r4, #160]
	ldr	r3, [r6, #48]
	str	r3, [r4, #164]
	b	.L102
.L24:
	ldr	r3, [r4, #168]
	mov	r0, #16777472
	orr	r3, r3, #66846720
	orr	r3, r3, #65536
	str	r3, [r4, #168]
	mov	r3, #8388736
	str	r3, [r4, #172]
	add	r3, r3, #8388608
	str	r3, [r4, #176]
	mov	r3, #6291552
	strd	r0, r3, [r4, #180]
	sub	r3, r3, #5767168
	str	r3, [r4, #188]
	mov	r3, #524296
	str	r3, [r4, #192]
	b	.L102
.L25:
	cmp	r1, #96
	bne	.L49
	add	r3, r7, #68
	mov	r1, #544
	add	r3, r3, r9
	str	r3, [sp, #20]
	movs	r3, #0
	strd	r1, r3, [sp, #24]
	ldr	r3, [r4, #636]
	ldr	r3, [r3, #16]
	cbz	r3, .L50
	add	r1, sp, #20
	ldr	r0, [r4, #640]
	blx	r3
.L50:
	movs	r2, #24
	movs	r3, #0
	strd	r2, r3, [sp, #24]
	mov	r3, #644
	mla	r3, r3, r6, r9
	add	r7, r7, #612
	add	r7, r7, r9
	str	r7, [sp, #20]
	ldr	r2, [r3, #636]
	ldr	r2, [r2, #20]
	cbz	r2, .L51
	add	r1, sp, #20
	ldr	r0, [r3, #640]
	blx	r2
.L51:
	mov	r1, #644
	mla	r2, r1, r6, r9
	mov	r4, r1
	ldr	r3, [r2, #636]
	ldr	r3, [r3, #24]
	cbz	r3, .L52
	ldr	r0, [r2, #640]
	blx	r3
.L52:
	mla	r6, r4, r6, r9
	ldr	r3, [r6, #636]
	ldr	r3, [r3, #28]
	cmp	r3, #0
	beq	.L102
	ldr	r0, [r6, #640]
	blx	r3
	b	.L102
.L49:
	cmp	r1, #112
	bne	.L54
	ldr	r3, [r4, #628]
	str	r3, [r2]
	ldr	r3, [r4, #632]
	str	r3, [r2, #4]
	b	.L102
.L54:
	cmp	r1, #128
	bne	.L102
	mov	r1, #644
	mla	r6, r1, r6, r9
	ldr	r3, [r6, #612]
	str	r3, [r5]
	ldr	r3, [r6, #616]
	str	r3, [r5, #4]
	ldr	r3, [r6, #620]
	str	r3, [r5, #8]
	ldr	r3, [r6, #624]
	str	r3, [r5, #12]
	b	.L102
.L106:
	.align	2
.L105:
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
	.section	.text.unlikely,"ax",%progbits
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
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r1
	mov	r6, r0
	bl	get_vmpibuf_func
	mov	r5, r0
	movs	r0, #12
	bl	vmalloc
	mov	r4, r0
	bl	IS_ERR_OR_NULL
	cbz	r0, .L108
	movs	r2, #49
	ldr	r1, .L123
	ldr	r0, .L123+4
	mvn	r4, #11
	bl	printk
.L107:
	mov	r0, r4
	pop	{r3, r4, r5, r6, r7, pc}
.L108:
	ldr	r3, [r5]
	cmp	r3, #0
	beq	.L107
	mov	r0, r7
	blx	r3
	str	r0, [r4]
	cmp	r0, #0
	beq	.L107
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq	.L107
	blx	r3
	str	r0, [r4, #4]
	ldr	r3, [r5, #24]
	cbz	r3, .L110
	ldr	r0, [r4]
	blx	r3
	str	r0, [r4, #8]
.L110:
	ldr	r3, [r4, #8]
	adds	r3, r3, #1
	bne	.L107
	ldr	r3, [r6, #636]
	movs	r2, #0
	ldr	r1, [r4, #4]
	ldr	r0, [r6, #640]
	ldr	r3, [r3, #32]
	blx	r3
	str	r0, [r4, #8]
	b	.L107
.L124:
	.align	2
.L123:
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
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r2, r0
	mov	r5, r0
	mov	r4, r1
	ldr	r0, .L144
	ldr	r1, .L144+4
	bl	printk
	cmp	r5, #7
	ble	.L126
	mov	r3, r5
	movs	r2, #165
	ldr	r1, .L144+8
	ldr	r0, .L144+12
	bl	printk
.L143:
	mov	r9, #-1
.L125:
	mov	r0, r9
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L126:
	mov	r2, #644
	ldr	r6, .L144+16
	mul	r7, r2, r5
	movs	r1, #0
	add	r8, r7, r6
	mov	r0, r8
	bl	memset
	str	r5, [r7, r6]
	ldr	r3, [r4]
	ldr	r2, .L144+20
	str	r3, [r8, #4]
	ldr	r3, [r4, #4]
	str	r3, [r8, #8]
	ldr	r3, [r4, #8]
	str	r3, [r8, #20]
	ldr	r3, [r4, #12]
	str	r3, [r8, #24]
	ldr	r3, [r4, #16]
	str	r3, [r8, #28]
	ldr	r3, [r4, #20]
	str	r3, [r8, #32]
	ldr	r3, [r4, #24]
	str	r2, [r8, #636]
	str	r3, [r8, #36]
	ldr	r1, [r4, #28]
	subs	r3, r1, #1
	cmp	r3, #4096
	bcs	.L128
	ldr	r3, [r4, #32]
	subs	r3, r3, #1
	cmp	r3, #4096
	bcs	.L128
	str	r1, [r8, #12]
	ldr	r3, [r4, #32]
	str	r3, [r8, #16]
.L129:
	ldr	r0, [r2, #4]
	mov	r9, #644
	bl	vmalloc
	mla	r9, r9, r5, r6
	str	r0, [r9, #640]
	cbnz	r0, .L130
	movs	r2, #193
	ldr	r1, .L144+8
	ldr	r0, .L144+24
	bl	printk
	b	.L143
.L128:
	mov	r3, #644
	ldr	r1, [r4]
	mla	r3, r3, r5, r6
	str	r1, [r3, #12]
	ldr	r1, [r4, #4]
	str	r1, [r3, #16]
	b	.L129
.L130:
	ldr	r3, [r9, #636]
	movs	r1, #0
	ldr	r2, [r3, #4]
	bl	memset
	ldr	r3, [r9, #636]
	movs	r1, #25
	ldr	r0, [r9, #640]
	ldr	r3, [r3, #8]
	blx	r3
	ldr	r3, [r9, #12]
	ldr	r4, [r9, #16]
	mov	r0, r8
	adds	r3, r3, #31
	bic	r3, r3, #31
	adds	r4, r4, #31
	sbfx	r4, r4, #5, #25
	sbfx	r7, r3, #1, #29
	sbfx	r3, r3, #5, #25
	muls	r4, r3, r4
	lsls	r1, r4, #10
	bl	pp_malloc_buffer
	strd	r0, r0, [r9, #52]
	bl	IS_ERR_OR_NULL
	cmp	r0, #0
	beq	.L136
	movs	r2, #109
	ldr	r1, .L144+28
	ldr	r0, .L144+32
	mov	r9, #-1
	bl	printk
.L131:
	lsls	r1, r4, #4
	mov	r0, r8
	adds	r1, r1, #63
	asrs	r1, r1, #6
	lsls	r1, r1, #4
	bl	pp_malloc_buffer
	mov	r3, #644
	mla	r3, r3, r5, r6
	str	r0, [r3, #60]
	bl	IS_ERR_OR_NULL
	cbz	r0, .L132
	movs	r2, #125
	ldr	r1, .L144+28
	ldr	r0, .L144+32
	mov	r9, #-1
	bl	printk
.L132:
	lsls	r1, r4, #6
	mov	r0, r8
	adds	r1, r1, #127
	asrs	r1, r1, #7
	lsls	r1, r1, #4
	bl	pp_malloc_buffer
	mov	r3, #644
	mla	r3, r3, r5, r6
	str	r0, [r3, #64]
	bl	IS_ERR_OR_NULL
	cbz	r0, .L133
	movs	r2, #142
	ldr	r1, .L144+28
	ldr	r0, .L144+32
	mov	r9, #-1
	bl	printk
.L133:
	adds	r1, r7, #3
	mov	r3, #644
	it	mi
	addmi	r1, r7, #6
	mla	r3, r3, r5, r6
	asrs	r1, r1, #2
	adds	r2, r1, #7
	it	mi
	addmi	r2, r1, #14
	asrs	r2, r2, #3
	lsls	r2, r2, #4
	str	r2, [r3, #48]
	b	.L125
.L136:
	mov	r9, r0
	b	.L131
.L145:
	.align	2
.L144:
	.word	.LC3
	.word	.LANCHOR1+50
	.word	.LANCHOR1+69
	.word	.LC4
	.word	g_pp_ctx
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
	ldr	r3, [r5, #636]
	ldr	r0, [r5, #640]
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
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r2, r0
	mov	r4, r0
	ldr	r1, .L160
	ldr	r0, .L160+4
	bl	printk
	cmp	r4, #7
	ble	.L157
	mov	r3, r4
	movs	r2, #211
	ldr	r1, .L160+8
	ldr	r0, .L160+12
	bl	printk
	mov	r0, #-1
.L156:
	pop	{r3, r4, r5, pc}
.L157:
	ldr	r3, .L160+16
	mov	r2, #644
	movs	r5, #0
	mla	r4, r2, r4, r3
	ldr	r1, [r4, #52]
	mov	r0, r4
	bl	pp_free_buffer
	ldr	r1, [r4, #60]
	mov	r0, r4
	bl	pp_free_buffer
	ldr	r1, [r4, #64]
	mov	r0, r4
	bl	pp_free_buffer
	ldr	r3, [r4, #636]
	ldr	r0, [r4, #640]
	ldr	r3, [r3, #12]
	blx	r3
	ldr	r0, [r4, #640]
	cmp	r0, #0
	beq	.L156
	bl	vfree
	str	r5, [r4, #640]
	mov	r0, r5
	b	.L156
.L161:
	.align	2
.L160:
	.word	.LANCHOR1+107
	.word	.LC3
	.word	.LANCHOR1+127
	.word	.LC6
	.word	g_pp_ctx
	.fnend
	.size	vepu_pp_destroy_chn, .-vepu_pp_destroy_chn
	.section	.rodata
	.set	.LANCHOR1,. + 0
	.type	__FUNCTION__.18432, %object
	.size	__FUNCTION__.18432, 17
__FUNCTION__.18432:
	.ascii	"get_vmpibuf_func\000"
	.type	__func__.18543, %object
	.size	__func__.18543, 16
__func__.18543:
	.ascii	"vepu_pp_control\000"
	.type	__func__.18439, %object
	.size	__func__.18439, 17
__func__.18439:
	.ascii	"pp_malloc_buffer\000"
	.type	__FUNCTION__.18466, %object
	.size	__FUNCTION__.18466, 19
__FUNCTION__.18466:
	.ascii	"vepu_pp_create_chn\000"
	.type	__func__.18467, %object
	.size	__func__.18467, 19
__func__.18467:
	.ascii	"vepu_pp_create_chn\000"
	.type	__func__.18457, %object
	.size	__func__.18457, 19
__func__.18457:
	.ascii	"pp_allocate_buffer\000"
	.type	__FUNCTION__.18478, %object
	.size	__FUNCTION__.18478, 20
__FUNCTION__.18478:
	.ascii	"vepu_pp_destroy_chn\000"
	.type	__func__.18479, %object
	.size	__func__.18479, 20
__func__.18479:
	.ascii	"vepu_pp_destroy_chn\000"
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_mpi_buf_fn, %object
	.size	g_mpi_buf_fn, 4
g_mpi_buf_fn:
	.space	4
	.type	g_pp_ctx, %object
	.size	g_pp_ctx, 5152
g_pp_ctx:
	.space	5152
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s failed\012\000"
.LC1:
	.ascii	"\0013%s:%d: vepu pp control channel id %d error\012"
	.ascii	"\000"
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
