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
	.file	"argparse.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_error.constprop.1, %function
argparse_error.constprop.1:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	tst	r2, #2
	mov	r2, r1
	itete	ne
	ldrne	r1, [r0, #8]
	ldrbeq	r1, [r0, #4]	@ zero_extendqisi2
	ldrne	r0, .L4
	ldreq	r0, .L4+4
	b	printk
.L5:
	.align	2
.L4:
	.word	.LC0
	.word	.LC1
	.fnend
	.size	argparse_error.constprop.1, .-argparse_error.constprop.1
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_getvalue, %function
argparse_getvalue:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	movs	r3, #0
	ldr	r6, [r1, #12]
	mov	r4, r0
	mov	r5, r1
	mov	r7, r2
	str	r3, [sp, #4]
	cmp	r6, #0
	beq	.L7
	ldr	r3, [r1]
	subs	r3, r3, #2
	cmp	r3, #4
	bhi	.L26
	tbb	[pc, r3]
.L10:
	.byte	(.L14-.L10)/2
	.byte	(.L13-.L10)/2
	.byte	(.L12-.L10)/2
	.byte	(.L11-.L10)/2
	.byte	(.L9-.L10)/2
	.p2align 1
.L14:
	ldr	r3, [r6]
	lsls	r2, r7, #31
	ite	mi
	addmi	r3, r3, #-1
	addpl	r3, r3, #1
	str	r3, [r6]
	ldr	r3, [r1, #12]
	ldr	r2, [r3]
	cmp	r2, #0
	blt	.L17
.L26:
	movs	r0, #0
	b	.L6
.L17:
	movs	r0, #0
	str	r0, [r3]
.L6:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L13:
	ands	r0, r7, #1
	ldr	r3, [r6]
	ldr	r2, [r1, #24]
	beq	.L18
	bic	r3, r3, r2
	str	r3, [r6]
	b	.L26
.L18:
	orrs	r3, r3, r2
	str	r3, [r6]
	b	.L6
.L9:
	ldr	r0, [r0, #36]
	cbz	r0, .L19
	str	r0, [r6]
	movs	r0, #0
	str	r0, [r4, #36]
	b	.L6
.L19:
	ldr	r3, [r4, #20]
	cmp	r3, #1
	ble	.L20
	subs	r3, r3, #1
	str	r3, [r4, #20]
	ldr	r3, [r4, #24]
	adds	r2, r3, #4
	str	r2, [r4, #24]
	ldr	r2, [r1, #12]
	ldr	r3, [r3, #4]
	str	r3, [r2]
	b	.L6
.L20:
	ldr	r1, .L40
	mov	r2, r7
.L39:
	mov	r0, r5
	bl	argparse_error.constprop.1
.L7:
	ldr	r3, [r5, #20]
	cmp	r3, #0
	beq	.L26
	mov	r1, r5
	mov	r0, r4
	blx	r3
	b	.L6
.L12:
	ldr	r0, [r0, #36]
	cbz	r0, .L21
	movs	r2, #0
	add	r1, sp, #4
	bl	simple_strtol
	movs	r3, #0
	str	r0, [r6]
	str	r3, [r4, #36]
.L22:
	ldr	r3, [sp, #4]
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L26
	mov	r2, r7
	ldr	r1, .L40+4
	b	.L39
.L21:
	ldr	r3, [r4, #20]
	cmp	r3, #1
	ble	.L20
	subs	r3, r3, #1
	str	r3, [r4, #20]
	ldr	r3, [r4, #24]
	adds	r2, r3, #4
	str	r2, [r4, #24]
	mov	r2, r0
	ldr	r6, [r1, #12]
	ldr	r0, [r3, #4]
	add	r1, sp, #4
	bl	simple_strtol
	str	r0, [r6]
	b	.L22
.L11:
	ldr	r3, [r0, #36]
	cbz	r3, .L24
	movs	r3, #0
	str	r3, [r0, #36]
.L25:
	movs	r3, #0
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L26
	mov	r2, r7
	ldr	r1, .L40+8
	b	.L39
.L24:
	ldr	r3, [r0, #20]
	cmp	r3, #1
	ble	.L20
	subs	r3, r3, #1
	str	r3, [r0, #20]
	b	.L25
.L41:
	.align	2
.L40:
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.fnend
	.size	argparse_getvalue, .-argparse_getvalue
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_short_opt, %function
argparse_short_opt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4}
	.save {r4}
.L43:
	ldr	r3, [r1]
	cbnz	r3, .L46
	mvn	r0, #1
	pop	{r4}
	bx	lr
.L46:
	ldr	r2, [r0, #36]
	ldrb	r4, [r1, #4]	@ zero_extendqisi2
	ldrb	r3, [r2]	@ zero_extendqisi2
	cmp	r4, r3
	bne	.L44
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	cbz	r3, .L45
	adds	r3, r2, #1
.L45:
	str	r3, [r0, #36]
	movs	r2, #0
	pop	{r4}
	b	argparse_getvalue
.L44:
	adds	r1, r1, #32
	b	.L43
	.fnend
	.size	argparse_short_opt, .-argparse_short_opt
	.align	1
	.global	argparse_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_init, %function
argparse_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r4, r0
	mov	r7, r1
	mov	r6, r2
	movs	r1, #0
	movs	r2, #20
	add	r0, r0, r2
	mov	r5, r3
	bl	memset
	movs	r0, #0
	strd	r7, r6, [r4]
	str	r5, [r4, #8]
	strd	r0, r0, [r4, #12]
	pop	{r3, r4, r5, r6, r7, pc}
	.fnend
	.size	argparse_init, .-argparse_init
	.align	1
	.global	argparse_describe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_describe, %function
argparse_describe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	strd	r1, r2, [r0, #12]
	movs	r0, #0
	bx	lr
	.fnend
	.size	argparse_describe, .-argparse_describe
	.section	.text.unlikely
	.align	1
	.global	argparse_usage
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_usage, %function
argparse_usage:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r6, r0
	ldr	r4, [r0, #4]
	cbz	r4, .L51
	ldr	r1, [r4]
	ldr	r0, .L103
	bl	printk
	ldr	r5, .L103+4
.L52:
	ldr	r1, [r4, #4]!
	cbz	r1, .L53
	ldrb	r3, [r1]	@ zero_extendqisi2
	cbz	r3, .L53
	mov	r0, r5
	bl	printk
	b	.L52
.L51:
	ldr	r0, .L103+8
	bl	printk
.L53:
	ldr	r1, [r6, #12]
	cbz	r1, .L55
	ldr	r0, .L103+12
	bl	printk
.L55:
	ldr	r0, .L103+16
	movs	r4, #0
	bl	printk
	ldr	r5, [r6]
	adds	r5, r5, #32
	mov	r8, r5
.L56:
	ldr	r9, [r8, #-32]
	cmp	r9, #0
	bne	.L62
	ldr	r8, .L103+48
	adds	r7, r4, #4
	ldr	r9, .L103+52
	ldr	r10, .L103+56
.L63:
	ldr	r3, [r5, #-32]
	cbnz	r3, .L74
	ldr	r1, [r6, #16]
	cbz	r1, .L93
	ldr	r0, .L103+12
	bl	printk
.L93:
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L62:
	ldrb	r7, [r8, #-28]	@ zero_extendqisi2
	ldr	r0, [r8, #-24]
	cmp	r7, #0
	bne	.L57
	cbz	r0, .L58
.L76:
	bl	strlen
	adds	r7, r7, #2
	add	r0, r0, r7
.L58:
	cmp	r9, #4
	bne	.L59
.L101:
	adds	r0, r0, #6
	b	.L60
.L78:
	movs	r7, #4
	b	.L76
.L59:
	cmp	r9, #5
	beq	.L101
	cmp	r9, #6
	beq	.L101
.L60:
	adds	r0, r0, #3
	add	r8, r8, #32
	bic	r0, r0, #3
	cmp	r4, r0
	it	cc
	movcc	r4, r0
	b	.L56
.L74:
	cmp	r3, #1
	bne	.L64
	ldr	r0, .L103+16
	bl	printk
	ldr	r1, [r5, #-16]
	ldr	r0, .L103+20
	bl	printk
	ldr	r0, .L103+16
	bl	printk
.L65:
	adds	r5, r5, #32
	b	.L63
.L64:
	mov	r0, r8
	bl	printk
	ldrb	r1, [r5, #-28]	@ zero_extendqisi2
	mov	r4, r0
	cbz	r1, .L66
	ldr	r0, .L103+24
	bl	printk
	add	r4, r4, r0
.L66:
	ldr	r3, [r5, #-24]
	cbz	r3, .L67
	ldrb	r3, [r5, #-28]	@ zero_extendqisi2
	cbz	r3, .L67
	ldr	r0, .L103+28
	bl	printk
	add	r4, r4, r0
.L67:
	ldr	r1, [r5, #-24]
	cbz	r1, .L68
	ldr	r0, .L103+32
	bl	printk
	add	r4, r4, r0
.L68:
	ldr	r3, [r5, #-32]
	cmp	r3, #4
	bne	.L69
	ldr	r0, .L103+36
.L102:
	bl	printk
	add	r4, r4, r0
.L70:
	cmp	r4, r7
	bhi	.L72
	subs	r1, r7, r4
.L73:
	ldr	r3, [r5, #-16]
	mov	r2, r9
	adds	r1, r1, #2
	mov	r0, r10
	bl	printk
	b	.L65
.L69:
	cmp	r3, #5
	bne	.L71
	ldr	r0, .L103+40
	b	.L102
.L71:
	cmp	r3, #6
	bne	.L70
	ldr	r0, .L103+44
	b	.L102
.L72:
	ldr	r0, .L103+16
	bl	printk
	mov	r1, r7
	b	.L73
.L57:
	cmp	r0, #0
	bne	.L78
	movs	r0, #2
	b	.L58
.L104:
	.align	2
.L103:
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC10
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC11
	.word	.LC18
	.word	.LC19
	.fnend
	.size	argparse_usage, .-argparse_usage
	.text
	.align	1
	.global	argparse_parse
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_parse, %function
argparse_parse:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	ldr	r5, [r0]
	subs	r1, r1, #1
	ldr	r6, .L155
	adds	r3, r2, #4
	adds	r5, r5, #32
	str	r1, [r0, #20]
	strd	r3, r2, [r0, #24]
.L106:
	ldr	r1, [r5, #-32]
	cbnz	r1, .L108
.L109:
	ldrd	r2, r3, [r4, #20]
	cmp	r2, #0
	beq	.L112
	ldr	r5, [r3]
	ldrb	r1, [r5]	@ zero_extendqisi2
	cmp	r1, #45
	bne	.L110
	ldrb	r1, [r5, #1]	@ zero_extendqisi2
	cbnz	r1, .L111
.L110:
	ldr	r2, [r4, #8]
	lsls	r6, r2, #31
	bmi	.L112
	ldrd	r1, r2, [r4, #28]
	adds	r0, r2, #1
	str	r0, [r4, #32]
	ldr	r3, [r3]
	str	r3, [r1, r2, lsl #2]
.L113:
	ldr	r3, [r4, #20]
	subs	r3, r3, #1
	str	r3, [r4, #20]
	ldr	r3, [r4, #24]
	adds	r3, r3, #4
	str	r3, [r4, #24]
	b	.L109
.L108:
	cmp	r1, #6
	bls	.L107
	mov	r0, r6
	bl	printk
.L107:
	adds	r5, r5, #32
	b	.L106
.L111:
	cmp	r1, #45
	beq	.L114
	adds	r5, r5, #1
	ldr	r1, [r4]
	str	r5, [r4, #36]
	mov	r0, r4
	bl	argparse_short_opt
	adds	r5, r0, #3
	beq	.L131
.L154:
	adds	r0, r0, #2
	bne	.L116
.L117:
	ldr	r3, [r4, #24]
	ldr	r0, .L155+4
	ldr	r1, [r3]
	bl	printk
	mov	r0, r4
	bl	argparse_usage
.L131:
	mov	r0, #-1
	b	.L105
.L116:
	ldr	r3, [r4, #36]
	cmp	r3, #0
	beq	.L113
	ldr	r1, [r4]
	mov	r0, r4
	bl	argparse_short_opt
	adds	r1, r0, #3
	bne	.L154
.L118:
	mvn	r0, #2
.L105:
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L114:
	ldrb	r1, [r5, #2]	@ zero_extendqisi2
	cbnz	r1, .L121
	subs	r2, r2, #1
	adds	r3, r3, #4
	str	r2, [r4, #20]
	str	r3, [r4, #24]
.L112:
	ldrd	r3, r0, [r4, #28]
	ldr	r2, [r4, #20]
	ldr	r1, [r4, #24]
	add	r0, r3, r0, lsl #2
	lsls	r2, r2, #2
	bl	memmove
	ldr	r2, [r4, #20]
	ldr	r3, [r4, #32]
	movs	r1, #0
	add	r3, r3, r2
	ldr	r2, [r4, #28]
	str	r1, [r2, r3, lsl #2]
	ldr	r0, [r4, #32]
	ldr	r3, [r4, #20]
	add	r0, r0, r3
	b	.L105
.L121:
	ldr	r6, [r4]
	add	r9, r5, #2
	add	r10, r5, #5
.L122:
	ldr	r8, [r6]
	cmp	r8, #0
	beq	.L117
	ldr	fp, [r6, #8]
	cmp	fp, #0
	beq	.L123
	mov	r0, fp
	bl	strlen
	mov	r1, fp
	mov	r7, r0
	mov	r2, r0
	mov	r0, r9
	bl	strncmp
	cbnz	r0, .L124
	adds	r3, r9, r7
	bne	.L125
.L124:
	ldr	r3, [r6, #28]
	lsls	r2, r3, #31
	bmi	.L123
	sub	r8, r8, #2
	cmp	r8, #1
	bhi	.L123
	ldr	r1, .L155+8
	adds	r2, r5, #1
.L127:
	ldrb	r3, [r1], #1	@ zero_extendqisi2
	cbz	r3, .L126
	ldrb	r0, [r2, #1]!	@ zero_extendqisi2
	cmp	r0, r3
	beq	.L127
.L123:
	adds	r6, r6, #32
	b	.L122
.L126:
	mov	r2, r7
	mov	r1, fp
	mov	r0, r10
	bl	strncmp
	cmp	r0, #0
	bne	.L123
	adds	r3, r10, r7
	beq	.L123
	movs	r0, #1
.L125:
	ldrb	r2, [r3]	@ zero_extendqisi2
	cbz	r2, .L128
	cmp	r2, #61
	bne	.L123
	adds	r3, r3, #1
	str	r3, [r4, #36]
.L128:
	orr	r2, r0, #2
	mov	r1, r6
	mov	r0, r4
	bl	argparse_getvalue
	adds	r3, r0, #3
	beq	.L118
	adds	r0, r0, #2
	bne	.L113
	b	.L117
.L156:
	.align	2
.L155:
	.word	.LC20
	.word	.LC22
	.word	.LC21
	.fnend
	.size	argparse_parse, .-argparse_parse
	.section	.text.unlikely
	.align	1
	.global	argparse_help_cb
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_help_cb, %function
argparse_help_cb:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	argparse_usage
	mvn	r0, #2
	pop	{r3, pc}
	.fnend
	.size	argparse_help_cb, .-argparse_help_cb
	.text
	.align	1
	.global	argparse_extract
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_extract, %function
argparse_extract:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	ldrb	r4, [r2]	@ zero_extendqisi2
.L159:
	mov	r3, r0
	ldrb	r5, [r0], #1	@ zero_extendqisi2
	cmp	r5, r4
	beq	.L159
	str	r3, [r1]
	movs	r0, #0
	ldrb	r1, [r2]	@ zero_extendqisi2
.L160:
	ldrb	r2, [r3, r0]	@ zero_extendqisi2
	cmp	r2, r1
	beq	.L158
	cbnz	r2, .L162
.L158:
	pop	{r4, r5, pc}
.L162:
	adds	r0, r0, #1
	b	.L160
	.fnend
	.size	argparse_extract, .-argparse_extract
	.align	1
	.global	argparse_excute_main
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	argparse_excute_main, %function
argparse_excute_main:
	.fnstart
	@ args = 0, pretend = 0, frame = 384
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #388
	sub	sp, sp, #388
	mov	r5, r0
	mov	r6, r1
	movs	r2, #128
	movs	r1, #0
	mov	r0, sp
	movs	r4, #0
	bl	memset
	add	r7, sp, #256
.L170:
	mov	r0, r5
.L166:
	mov	r3, r0
	mov	r5, r0
	ldrb	r2, [r3], #1	@ zero_extendqisi2
	cmp	r2, #32
	beq	.L173
	movs	r3, #0
.L167:
	mov	r1, r0
	adds	r0, r0, #1
	ldrb	r2, [r1]	@ zero_extendqisi2
	ands	r2, r2, #223
	bne	.L168
	cbz	r3, .L169
	cmp	r4, #32
	beq	.L169
	str	r3, [r7, r4, lsl #2]
	adds	r3, r3, #1
	str	r5, [sp, r4, lsl #2]
	adds	r4, r4, #1
	strb	r2, [r1]
	add	r5, r5, r3
	b	.L170
.L173:
	mov	r0, r3
	b	.L166
.L168:
	adds	r3, r3, #1
	b	.L167
.L169:
	movs	r2, #128
	mov	r1, sp
	add	r0, sp, r2
	bl	memcpy
	mov	r0, r4
	subs	r4, r4, #1
	add	r1, sp, #128
	blx	r6
	movs	r3, #0
	movs	r1, #32
	add	r2, sp, #256
.L171:
	cmp	r4, r3
	bgt	.L172
	add	sp, sp, #388
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L172:
	ldr	r5, [sp, r3, lsl #2]
	ldr	r0, [r2, r3, lsl #2]
	adds	r3, r3, #1
	strb	r1, [r5, r0]
	b	.L171
	.fnend
	.size	argparse_excute_main, .-argparse_excute_main
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\001cerror: option `--%s` %s\012\000"
.LC1:
	.ascii	"\001cerror: option `-%c` %s\012\000"
.LC2:
	.ascii	"requires a value\000"
.LC3:
	.ascii	"expects an integer value\000"
.LC4:
	.ascii	"expects a numerical value\000"
.LC5:
	.ascii	"\001cUsage: %s\012\000"
.LC6:
	.ascii	"\001c   or: %s\012\000"
.LC7:
	.ascii	"\001cUsage:\012\000"
.LC8:
	.ascii	"\001c%s\012\000"
.LC9:
	.ascii	"\001c\012\000"
.LC10:
	.ascii	"\001c%s\000"
.LC11:
	.ascii	"\001c    \000"
.LC12:
	.ascii	"\001c-%c\000"
.LC13:
	.ascii	"\001c, \000"
.LC14:
	.ascii	"\001c--%s\000"
.LC15:
	.ascii	"\001c=<int>\000"
.LC16:
	.ascii	"\001c=<flt>\000"
.LC17:
	.ascii	"\001c=<str>\000"
.LC18:
	.ascii	"\000"
.LC19:
	.ascii	"\001c%*s%s\012\000"
.LC20:
	.ascii	"\001cwrong option type: %d\000"
.LC21:
	.ascii	"no-\000"
.LC22:
	.ascii	"\001cerror: unknown option `%s`\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
