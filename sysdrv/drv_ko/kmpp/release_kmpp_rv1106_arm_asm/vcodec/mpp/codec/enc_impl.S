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
	.file	"enc_impl.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	enc_impl_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_init, %function
enc_impl_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r4, r1
	mov	r8, r0
	cbz	r0, .L2
	cbnz	r1, .L3
.L2:
	str	r4, [sp]
	mov	r3, r8
	movs	r2, #56
	ldr	r1, .L18
	ldr	r0, .L18+4
	mvn	r7, #2
	bl	printk
.L1:
	mov	r0, r7
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L3:
	ldr	r2, .L18+8
	movs	r3, #0
	str	r3, [r0]
	ldr	r3, [r1]
	ldr	r5, [r2]
	ldr	r1, [r5, #4]
	cmp	r3, r1
	beq	.L5
	ldr	r5, [r2, #4]
	ldr	r1, [r5, #4]
	cmp	r3, r1
	beq	.L5
	ldr	r5, [r2, #8]
	ldr	r2, [r5, #4]
	cmp	r3, r2
	bne	.L10
.L5:
	movs	r1, #28
	ldr	r0, .L18+12
	bl	mpp_osal_calloc
	ldr	r1, [r5, #8]
	mov	r6, r0
	ldr	r0, .L18+12
	bl	mpp_osal_calloc
	mov	r9, r0
	cbz	r0, .L6
	cbnz	r6, .L7
.L6:
	movs	r2, #71
	ldr	r1, .L18
	ldr	r0, .L18+16
	mvn	r7, #3
	bl	printk
	mov	r1, r6
	ldr	r0, .L18+12
	bl	mpp_osal_free
	mov	r1, r9
	ldr	r0, .L18+12
	bl	mpp_osal_free
	b	.L1
.L7:
	ldr	r3, [r5, #16]
	mov	r1, r4
	blx	r3
	mov	r7, r0
	cbz	r0, .L8
	movs	r2, #79
	ldr	r1, .L18
	ldr	r0, .L18+20
	bl	printk
	mov	r1, r6
	ldr	r0, .L18+12
	bl	mpp_osal_free
	mov	r1, r9
	ldr	r0, .L18+12
	bl	mpp_osal_free
	b	.L1
.L8:
	add	r2, r4, #20
	mov	r3, r6
	strd	r5, r9, [r6, #20]
.L9:
	ldr	r1, [r4], #4	@ unaligned
	cmp	r4, r2
	str	r1, [r3], #4	@ unaligned
	bne	.L9
	str	r6, [r8]
	b	.L1
.L10:
	mov	r7, #-1
	b	.L1
.L19:
	.align	2
.L18:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR1
	.word	.LANCHOR0+14
	.word	.LC1
	.word	.LC2
	.fnend
	.size	enc_impl_init, .-enc_impl_init
	.align	1
	.global	enc_impl_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_deinit, %function
enc_impl_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbnz	r0, .L21
	movs	r2, #100
	ldr	r1, .L27
	ldr	r0, .L27+4
	bl	printk
	mvn	r0, #2
.L20:
	pop	{r4, pc}
.L21:
	ldr	r3, [r0, #20]
	ldr	r3, [r3, #20]
	cbz	r3, .L23
	ldr	r0, [r0, #24]
	blx	r3
.L23:
	ldr	r1, [r4, #24]
	ldr	r0, .L27+8
	bl	mpp_osal_free
	ldr	r0, .L27+8
	mov	r1, r4
	bl	mpp_osal_free
	movs	r0, #0
	b	.L20
.L28:
	.align	2
.L27:
	.word	.LANCHOR0+28
	.word	.LC3
	.word	.LANCHOR0+44
	.fnend
	.size	enc_impl_deinit, .-enc_impl_deinit
	.align	1
	.global	enc_impl_proc_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_proc_cfg, %function
enc_impl_proc_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	cbnz	r0, .L30
	movs	r2, #118
	ldr	r1, .L33
	ldr	r0, .L33+4
	bl	printk
	mvn	r0, #2
.L29:
	pop	{r4, pc}
.L30:
	ldr	r3, [r0, #20]
	ldr	r3, [r3, #24]
	cbz	r3, .L32
	ldr	r0, [r0, #24]
	pop	{r4, lr}
	bx	r3
.L32:
	mov	r0, r3
	b	.L29
.L34:
	.align	2
.L33:
	.word	.LANCHOR0+60
	.word	.LC3
	.fnend
	.size	enc_impl_proc_cfg, .-enc_impl_proc_cfg
	.align	1
	.global	enc_impl_gen_hdr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_gen_hdr, %function
enc_impl_gen_hdr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	cbnz	r0, .L36
	movs	r2, #135
	ldr	r1, .L39
	ldr	r0, .L39+4
	bl	printk
	mvn	r0, #2
.L35:
	pop	{r4, pc}
.L36:
	ldr	r3, [r0, #20]
	ldr	r3, [r3, #28]
	cbz	r3, .L38
	ldr	r0, [r0, #24]
	pop	{r4, lr}
	bx	r3
.L38:
	mov	r0, r3
	b	.L35
.L40:
	.align	2
.L39:
	.word	.LANCHOR0+78
	.word	.LC3
	.fnend
	.size	enc_impl_gen_hdr, .-enc_impl_gen_hdr
	.align	1
	.global	enc_impl_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_start, %function
enc_impl_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	cbnz	r0, .L42
	movs	r2, #151
	ldr	r1, .L45
	ldr	r0, .L45+4
	bl	printk
	mvn	r0, #2
.L41:
	pop	{r4, pc}
.L42:
	ldr	r3, [r0, #20]
	ldr	r3, [r3, #32]
	cbz	r3, .L44
	ldr	r0, [r0, #24]
	pop	{r4, lr}
	bx	r3
.L44:
	mov	r0, r3
	b	.L41
.L46:
	.align	2
.L45:
	.word	.LANCHOR0+95
	.word	.LC3
	.fnend
	.size	enc_impl_start, .-enc_impl_start
	.align	1
	.global	enc_impl_proc_dpb
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_proc_dpb, %function
enc_impl_proc_dpb:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	cbnz	r0, .L48
	movs	r2, #167
	ldr	r1, .L51
	ldr	r0, .L51+4
	bl	printk
	mvn	r0, #2
.L47:
	pop	{r4, pc}
.L48:
	ldr	r3, [r0, #20]
	ldr	r3, [r3, #36]
	cbz	r3, .L50
	ldr	r0, [r0, #24]
	pop	{r4, lr}
	bx	r3
.L50:
	mov	r0, r3
	b	.L47
.L52:
	.align	2
.L51:
	.word	.LANCHOR0+110
	.word	.LC3
	.fnend
	.size	enc_impl_proc_dpb, .-enc_impl_proc_dpb
	.align	1
	.global	enc_impl_proc_hal
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_proc_hal, %function
enc_impl_proc_hal:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	cbz	r0, .L54
	cbnz	r1, .L55
.L54:
	movs	r2, #184
	ldr	r1, .L61
	ldr	r0, .L61+4
	bl	printk
	mvn	r0, #2
.L53:
	pop	{r4, pc}
.L55:
	ldr	r3, [r0, #20]
	ldr	r3, [r3, #40]
	cbz	r3, .L57
	ldr	r0, [r0, #24]
	pop	{r4, lr}
	bx	r3
.L57:
	mov	r0, r3
	b	.L53
.L62:
	.align	2
.L61:
	.word	.LANCHOR0+128
	.word	.LC3
	.fnend
	.size	enc_impl_proc_hal, .-enc_impl_proc_hal
	.align	1
	.global	enc_impl_add_prefix
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_add_prefix, %function
enc_impl_add_prefix:
	.fnstart
	@ args = 8, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	mov	r5, r2
	mov	r0, r1
	mov	r2, r3
	ldrd	r3, r6, [sp, #16]
	cbz	r1, .L64
	cbnz	r3, .L65
.L64:
	movs	r2, #202
	ldr	r1, .L71
	ldr	r0, .L71+4
	bl	printk
	mvn	r0, #2
.L63:
	pop	{r4, r5, r6, pc}
.L65:
	ldr	r1, [r4, #20]
	ldr	r4, [r1, #44]
	cbz	r4, .L67
	str	r6, [sp, #16]
	mov	r1, r5
	mov	ip, r4
	pop	{r4, r5, r6, lr}
	bx	ip
.L67:
	mov	r0, r4
	b	.L63
.L72:
	.align	2
.L71:
	.word	.LANCHOR0+146
	.word	.LC3
	.fnend
	.size	enc_impl_add_prefix, .-enc_impl_add_prefix
	.align	1
	.global	enc_impl_sw_enc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_sw_enc, %function
enc_impl_sw_enc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	cbz	r0, .L74
	cbnz	r1, .L75
.L74:
	movs	r2, #223
	ldr	r1, .L81
	ldr	r0, .L81+4
	bl	printk
	mvn	r0, #2
.L73:
	pop	{r4, pc}
.L75:
	ldr	r3, [r0, #20]
	ldr	r3, [r3, #48]
	cbz	r3, .L77
	ldr	r0, [r0, #24]
	pop	{r4, lr}
	bx	r3
.L77:
	mov	r0, r3
	b	.L73
.L82:
	.align	2
.L81:
	.word	.LANCHOR0+166
	.word	.LC3
	.fnend
	.size	enc_impl_sw_enc, .-enc_impl_sw_enc
	.align	1
	.global	enc_impl_proc_debug
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	enc_impl_proc_debug, %function
enc_impl_proc_debug:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbz	r1, .L84
	cbnz	r0, .L85
.L84:
	movs	r2, #239
	ldr	r1, .L90
	ldr	r0, .L90+4
	b	printk
.L85:
	ldr	r3, [r1, #20]
	ldr	r3, [r3, #52]
	cbz	r3, .L83
	ldr	r1, [r1, #24]
	bx	r3	@ indirect register sibling call
.L83:
	bx	lr
.L91:
	.align	2
.L90:
	.word	.LANCHOR0+182
	.word	.LC3
	.fnend
	.size	enc_impl_proc_debug, .-enc_impl_proc_debug
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.9292, %object
	.size	__func__.9292, 14
__func__.9292:
	.ascii	"enc_impl_init\000"
	.type	__FUNCTION__.9295, %object
	.size	__FUNCTION__.9295, 14
__FUNCTION__.9295:
	.ascii	"enc_impl_init\000"
	.type	__func__.9305, %object
	.size	__func__.9305, 16
__func__.9305:
	.ascii	"enc_impl_deinit\000"
	.type	__FUNCTION__.9306, %object
	.size	__FUNCTION__.9306, 16
__FUNCTION__.9306:
	.ascii	"enc_impl_deinit\000"
	.type	__func__.9314, %object
	.size	__func__.9314, 18
__func__.9314:
	.ascii	"enc_impl_proc_cfg\000"
	.type	__func__.9321, %object
	.size	__func__.9321, 17
__func__.9321:
	.ascii	"enc_impl_gen_hdr\000"
	.type	__func__.9328, %object
	.size	__func__.9328, 15
__func__.9328:
	.ascii	"enc_impl_start\000"
	.type	__func__.9335, %object
	.size	__func__.9335, 18
__func__.9335:
	.ascii	"enc_impl_proc_dpb\000"
	.type	__func__.9342, %object
	.size	__func__.9342, 18
__func__.9342:
	.ascii	"enc_impl_proc_hal\000"
	.type	__func__.9353, %object
	.size	__func__.9353, 20
__func__.9353:
	.ascii	"enc_impl_add_prefix\000"
	.type	__func__.9360, %object
	.size	__func__.9360, 16
__func__.9360:
	.ascii	"enc_impl_sw_enc\000"
	.type	__func__.9367, %object
	.size	__func__.9367, 20
__func__.9367:
	.ascii	"enc_impl_proc_debug\000"
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	enc_apis, %object
	.size	enc_apis, 12
enc_apis:
	.word	api_h264e
	.word	api_h265e
	.word	api_jpege
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: found NULL input controller %p config %"
	.ascii	"p\012\000"
.LC1:
	.ascii	"\0013%s:%d: failed to alloc encoder context\012\000"
.LC2:
	.ascii	"\0013%s:%d: failed to init controller\012\000"
.LC3:
	.ascii	"\0013%s:%d: found NULL input\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
