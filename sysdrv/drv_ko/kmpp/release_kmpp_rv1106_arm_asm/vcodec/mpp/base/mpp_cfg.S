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
	.file	"mpp_cfg.c"
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
	.type	show_api_type_err, %function
show_api_type_err:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	ldr	r2, .L2
	ldrb	r3, [r0, #4]	@ zero_extendqisi2
	ldr	r1, [r2, r1, lsl #2]
	and	r3, r3, #15
	str	r1, [sp]
	movs	r1, #27
	ldr	r3, [r2, r3, lsl #2]
	ldr	r2, [r0]
	ldr	r0, .L2+4
	bl	printk
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L3:
	.align	2
.L2:
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	show_api_type_err, .-show_api_type_err
	.text
	.align	1
	.global	check_cfg_api_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	check_cfg_api_info, %function
check_cfg_api_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldrh	r5, [r0, #4]
	ldrb	r3, [r0, #4]	@ zero_extendqisi2
	lsrs	r5, r5, #4
	and	r3, r3, #15
	cmp	r1, #5
	bhi	.L5
	tbb	[pc, r1]
.L7:
	.byte	(.L10-.L7)/2
	.byte	(.L10-.L7)/2
	.byte	(.L9-.L7)/2
	.byte	(.L9-.L7)/2
	.byte	(.L8-.L7)/2
	.byte	(.L6-.L7)/2
	.p2align 1
.L6:
	cmp	r3, #5
	beq	.L14
	movs	r1, #5
	bl	show_api_type_err
	mov	r0, #-1
.L11:
	cbnz	r5, .L4
	ldr	r2, [r4]
	mov	r3, r5
	ldr	r0, .L20
	movs	r1, #44
.L17:
	bl	printk
	b	.L18
.L14:
	movs	r0, #0
	b	.L11
.L8:
	cmp	r3, #4
	beq	.L16
	movs	r1, #4
.L13:
	mov	r0, r4
	bl	show_api_type_err
.L18:
	mov	r0, #-1
.L4:
	pop	{r3, r4, r5, pc}
.L10:
	cmp	r5, #4
.L19:
	bne	.L13
.L16:
	movs	r0, #0
	b	.L4
.L9:
	cmp	r5, #8
	b	.L19
.L5:
	mov	r3, r1
	ldr	r2, [r0]
	movs	r1, #74
	ldr	r0, .L20+4
	b	.L17
.L21:
	.align	2
.L20:
	.word	.LC1
	.word	.LC2
	.fnend
	.size	check_cfg_api_info, .-check_cfg_api_info
	.global	cfg_type_names
	.data
	.align	2
	.set	.LANCHOR0,. + 0
	.type	cfg_type_names, %object
	.size	cfg_type_names, 24
cfg_type_names:
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%d: cfg %s expect %s input NOT %s\012\000"
.LC1:
	.ascii	"\0013%d: cfg %s found invalid size %d\012\000"
.LC2:
	.ascii	"\0013%d: cfg %s found invalid cfg type %d\012\000"
.LC3:
	.ascii	"RK_S32\000"
.LC4:
	.ascii	"RK_U32\000"
.LC5:
	.ascii	"RK_S64\000"
.LC6:
	.ascii	"RK_U64\000"
.LC7:
	.ascii	"void *\000"
.LC8:
	.ascii	"struct\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
