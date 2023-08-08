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
	.file	"rc.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	rc_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_init, %function
rc_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r10, r0
	mov	r4, r1
	mov	r8, r2
	cmp	r2, #0
	beq	.L15
	ldr	r7, [r2]
	ldr	r3, .L38
	cmp	r7, #0
	it	eq
	moveq	r7, r3
.L2:
	ldr	r3, .L38+4
	ldr	r2, [r3]
	mov	r9, r3
	tst	r2, #1
	beq	.L3
	str	r7, [sp]
	mov	r3, r4
	movs	r2, #78
	ldr	r1, .L38+8
	ldr	r0, .L38+12
	bl	printk
.L3:
	ldr	r1, .L38+16
	mov	r0, r7
	bl	strcmp
	ldr	r2, .L38+20
	movs	r3, #0
	add	ip, r2, #16
	mov	r6, r3
.L6:
	ldr	r1, [r2], #4
	ldr	r5, [r1, #4]
	cbnz	r0, .L4
	cmp	r5, r4
	bne	.L5
	ldr	r6, [ip, r3, lsl #2]
.L5:
	adds	r3, r3, #1
	cmp	r3, #4
	bne	.L6
	cmp	r6, #0
	bne	.L7
	movs	r1, #95
	ldr	r3, .L38+24
	str	r1, [sp]
	mov	r4, r6
	ldr	r2, .L38+28
	mov	r5, #-1
	ldr	r0, .L38+32
	bl	printk
.L8:
	str	r4, [r10]
	cmp	r8, #0
	beq	.L11
	str	r7, [r8]
.L11:
	ldr	r3, [r9]
	lsls	r3, r3, #31
	bpl	.L1
	mov	r3, r4
	movs	r2, #121
	ldr	r1, .L38+8
	ldr	r0, .L38+36
	bl	printk
.L1:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L15:
	ldr	r7, .L38
	b	.L2
.L4:
	cmp	r5, r4
	it	eq
	moveq	r6, r1
	b	.L5
.L9:
	mov	r3, #-1
	strd	r5, r6, [r4]
	str	r3, [r4, #284]
	cmp	r8, #0
	beq	.L18
	ldr	r5, [r8]
	cmp	r5, #0
	beq	.L8
	ldr	r2, [r6]
	movs	r1, #112
	ldr	r0, .L38+40
	movs	r5, #0
	bl	printk
	b	.L8
.L18:
	mov	r5, r8
	b	.L8
.L7:
	ldr	r1, [r6, #8]
	ldr	r0, .L38+24
	bl	mpp_osal_calloc
	mov	r1, #296
	mov	r5, r0
	ldr	r0, .L38+24
	bl	mpp_osal_calloc
	mov	r4, r0
	cbz	r0, .L13
	cmp	r5, #0
	bne	.L9
.L13:
	ldr	r3, [r6, #8]
	movs	r2, #103
	ldr	r1, .L38+8
	ldr	r0, .L38+44
	bl	printk
	cbz	r4, .L10
	mov	r1, r4
	ldr	r0, .L38+24
	bl	mpp_osal_free
.L10:
	cbz	r5, .L37
	mov	r1, r5
	ldr	r0, .L38+24
	bl	mpp_osal_free
.L37:
	movs	r4, #0
	mvn	r5, #3
	b	.L8
.L39:
	.align	2
.L38:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	.LANCHOR0+8
	.word	.LC0
	.word	.LC1
	.word	.LANCHOR2
	.word	.LANCHOR0+16
	.word	.LC2
	.word	.LC3
	.word	.LC6
	.word	.LC5
	.word	.LC4
	.fnend
	.size	rc_init, .-rc_init
	.align	1
	.global	rc_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_deinit, %function
rc_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r6, .L61
	ldr	r4, [r0, #4]
	ldr	r2, [r6]
	lsls	r2, r2, #31
	bpl	.L41
	mov	r3, r0
	movs	r2, #132
	ldr	r1, .L61+4
	ldr	r0, .L61+8
	bl	printk
.L41:
	cbz	r4, .L42
	ldr	r4, [r4, #16]
	cbz	r4, .L42
	ldr	r0, [r5]
	cbz	r0, .L48
	blx	r4
	ldr	r1, [r5]
	mov	r4, r0
	cbz	r1, .L43
	ldr	r0, .L61+12
	bl	mpp_osal_free
.L43:
	movs	r3, #0
	str	r3, [r5]
.L42:
	cbz	r5, .L44
	mov	r1, r5
	ldr	r0, .L61+12
	bl	mpp_osal_free
.L44:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L40
	mov	r3, r5
	movs	r2, #141
	ldr	r1, .L61+4
	ldr	r0, .L61+16
	bl	printk
.L40:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L48:
	mov	r4, r0
	b	.L42
.L62:
	.align	2
.L61:
	.word	.LANCHOR1
	.word	.LANCHOR0+24
	.word	.LC7
	.word	.LANCHOR0+34
	.word	.LC6
	.fnend
	.size	rc_deinit, .-rc_deinit
	.align	1
	.global	rc_update_usr_cfg
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_update_usr_cfg, %function
rc_update_usr_cfg:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, lr}
	mov	r6, r0
	ldr	r7, .L82
	mov	r4, r1
	ldr	r9, [r0, #4]
	ldr	r3, [r7]
	lsls	r2, r3, #31
	bpl	.L64
	mov	r3, r0
	movs	r2, #152
	ldr	r1, .L82+4
	ldr	r0, .L82+8
	bl	printk
.L64:
	add	r8, r6, #8
	mov	r1, r4
	adds	r4, r4, #12
	movs	r2, #252
	mov	r0, r8
	add	r5, r6, #260
	bl	memcpy
	ldmia	r4!, {r0, r1, r2, r3}
	stmia	r5!, {r0, r1, r2, r3}
	ldm	r4, {r0, r1}
	stm	r5, {r0, r1}
	cmp	r9, #0
	beq	.L65
	ldr	r3, [r9, #12]
	cbz	r3, .L65
	ldr	r0, [r6]
	cbz	r0, .L65
	mov	r1, r8
	blx	r3
.L65:
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L66
	mov	r3, r6
	movs	r2, #160
	ldr	r1, .L82+4
	ldr	r0, .L82+12
	bl	printk
.L66:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, r8, r9, pc}
.L83:
	.align	2
.L82:
	.word	.LANCHOR1
	.word	.LANCHOR0+44
	.word	.LC7
	.word	.LC6
	.fnend
	.size	rc_update_usr_cfg, .-rc_update_usr_cfg
	.align	1
	.global	rc_frm_check_drop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_frm_check_drop, %function
rc_frm_check_drop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L111
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r4, r0
	ldr	r2, [r3]
	mov	r9, r1
	ldr	r5, [r0, #4]
	mov	r8, r3
	tst	r2, #1
	beq	.L85
	mov	r3, r0
	movs	r2, #171
	ldr	r1, .L111+4
	ldr	r0, .L111+8
	bl	printk
.L85:
	cbz	r5, .L86
	ldr	r3, [r5, #20]
	cbz	r3, .L86
	ldr	r0, [r4]
	cbz	r0, .L86
	cmp	r9, #0
	beq	.L86
	mov	r1, r9
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, lr}
	bx	r3
.L86:
	ldr	r3, [r4, #264]
	ldr	r6, [r4, #280]
	ldr	r2, [r4, #268]
	ldr	r5, [r4, #284]
	muls	r6, r3, r6
	ldr	r3, [r4, #276]
	cmp	r2, #0
	mul	r7, r3, r2
	bgt	.L87
	movs	r1, #183
	ldr	r3, .L111+12
	str	r1, [sp]
	ldr	r2, .L111+16
	ldr	r0, .L111+20
	bl	printk
.L87:
	ldr	r3, [r4, #280]
	cmp	r3, #0
	bgt	.L88
	movs	r1, #184
	ldr	r3, .L111+12
	str	r1, [sp]
	ldr	r2, .L111+24
	ldr	r0, .L111+20
	bl	printk
.L88:
	cmp	r6, r7
	bge	.L89
	movs	r1, #185
	ldr	r3, .L111+12
	str	r1, [sp]
	ldr	r2, .L111+28
	ldr	r0, .L111+20
	bl	printk
.L89:
	cmp	r5, #0
	it	lt
	sublt	r5, r6, r7
	add	r5, r5, r7
	cmp	r6, r5
	it	le
	suble	r5, r5, r6
	str	r5, [r4, #284]
	ldrb	r3, [r9, #156]	@ zero_extendqisi2
	ite	le
	movle	r2, #0
	movgt	r2, #1
	bfi	r3, r2, #0, #1
	strb	r3, [r9, #156]
	ldr	r3, [r8]
	lsls	r3, r3, #31
	bpl	.L84
	str	r2, [sp]
	mov	r3, r4
	movs	r2, #202
	ldr	r1, .L111+4
	ldr	r0, .L111+32
	bl	printk
.L84:
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L112:
	.align	2
.L111:
	.word	.LANCHOR1
	.word	.LANCHOR0+62
	.word	.LC7
	.word	.LANCHOR0+80
	.word	.LC8
	.word	.LC3
	.word	.LC9
	.word	.LC10
	.word	.LC11
	.fnend
	.size	rc_frm_check_drop, .-rc_frm_check_drop
	.align	1
	.global	rc_frm_check_reenc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_frm_check_reenc, %function
rc_frm_check_reenc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #4]
	cbz	r3, .L113
	ldr	r3, [r3, #24]
	cbz	r3, .L113
	ldr	r0, [r0]
	cbz	r0, .L113
	cbz	r1, .L113
	bx	r3
.L113:
	movs	r0, #0
	bx	lr
	.fnend
	.size	rc_frm_check_reenc, .-rc_frm_check_reenc
	.align	1
	.global	rc_frm_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_frm_start, %function
rc_frm_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #4]
	cbz	r3, .L123
	ldr	r3, [r3, #28]
	cbz	r3, .L123
	ldr	r0, [r0]
	cbz	r0, .L123
	cbz	r1, .L123
	bx	r3
.L123:
	movs	r0, #0
	bx	lr
	.fnend
	.size	rc_frm_start, .-rc_frm_start
	.align	1
	.global	rc_frm_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_frm_end, %function
rc_frm_end:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #4]
	cbz	r3, .L133
	ldr	r3, [r3, #32]
	cbz	r3, .L133
	ldr	r0, [r0]
	cbz	r0, .L133
	cbz	r1, .L133
	bx	r3
.L133:
	movs	r0, #0
	bx	lr
	.fnend
	.size	rc_frm_end, .-rc_frm_end
	.align	1
	.global	rc_hal_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_hal_start, %function
rc_hal_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #4]
	cbz	r3, .L143
	ldr	r3, [r3, #36]
	cbz	r3, .L143
	ldr	r0, [r0]
	cbz	r0, .L143
	cbz	r1, .L143
	bx	r3
.L143:
	movs	r0, #0
	bx	lr
	.fnend
	.size	rc_hal_start, .-rc_hal_start
	.align	1
	.global	rc_hal_end
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_hal_end, %function
rc_hal_end:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #4]
	cbz	r3, .L153
	ldr	r3, [r3, #40]
	cbz	r3, .L153
	ldr	r0, [r0]
	cbz	r0, .L153
	cbz	r1, .L153
	bx	r3
.L153:
	movs	r0, #0
	bx	lr
	.fnend
	.size	rc_hal_end, .-rc_hal_end
	.align	1
	.global	rc_proc_show
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rc_proc_show, %function
rc_proc_show:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r3, [r1, #4]
	cbz	r3, .L164
	ldr	r3, [r3, #44]
	cbz	r3, .L164
	ldr	r1, [r1]
	cbz	r1, .L164
	blx	r3
.L164:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	rc_proc_show, .-rc_proc_show
	.global	rc_debug
	.global	smt_rc_api_ops
	.global	rc_api_ops
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	default_rc_api, %object
	.size	default_rc_api, 8
default_rc_api:
	.ascii	"default\000"
	.type	__func__.18845, %object
	.size	__func__.18845, 8
__func__.18845:
	.ascii	"rc_init\000"
	.type	__FUNCTION__.18849, %object
	.size	__FUNCTION__.18849, 8
__FUNCTION__.18849:
	.ascii	"rc_init\000"
	.type	__func__.18859, %object
	.size	__func__.18859, 10
__func__.18859:
	.ascii	"rc_deinit\000"
	.type	__FUNCTION__.18860, %object
	.size	__FUNCTION__.18860, 10
__FUNCTION__.18860:
	.ascii	"rc_deinit\000"
	.type	__func__.18868, %object
	.size	__func__.18868, 18
__func__.18868:
	.ascii	"rc_update_usr_cfg\000"
	.type	__func__.18876, %object
	.size	__func__.18876, 18
__func__.18876:
	.ascii	"rc_frm_check_drop\000"
	.type	__FUNCTION__.18882, %object
	.size	__FUNCTION__.18882, 18
__FUNCTION__.18882:
	.ascii	"rc_frm_check_drop\000"
	.type	__param_str_rc_debug, %object
	.size	__param_str_rc_debug, 20
__param_str_rc_debug:
	.ascii	"mpp_vcodec.rc_debug\000"
	.data
	.align	2
	.set	.LANCHOR2,. + 0
	.type	rc_api_ops, %object
	.size	rc_api_ops, 16
rc_api_ops:
	.word	default_h264e
	.word	default_h265e
	.word	default_jpege
	.word	default_vp8e
	.type	smt_rc_api_ops, %object
	.size	smt_rc_api_ops, 16
smt_rc_api_ops:
	.word	smt_h264e
	.word	smt_h265e
	.word	default_jpege
	.word	default_vp8e
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	rc_debug, %object
	.size	rc_debug, 4
rc_debug:
	.space	4
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_rc_debug105, %object
	.size	__UNIQUE_ID_rc_debug105, 51
__UNIQUE_ID_rc_debug105:
	.ascii	"mpp_vcodec.parm=rc_debug:bits rc debug information\000"
	.type	__UNIQUE_ID_rc_debugtype104, %object
	.size	__UNIQUE_ID_rc_debugtype104, 34
__UNIQUE_ID_rc_debugtype104:
	.ascii	"mpp_vcodec.parmtype=rc_debug:uint\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: enter type %x name %s\012\000"
.LC1:
	.ascii	"smart\000"
.LC2:
	.ascii	"api\000"
.LC3:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC4:
	.ascii	"\0013%s:%d: failed to create context size %d\012\000"
.LC5:
	.ascii	"\0016%d: using rc impl %s\012\000"
.LC6:
	.ascii	"\0016%s:%d: leave %p\012\000"
.LC7:
	.ascii	"\0016%s:%d: enter %p\012\000"
.LC8:
	.ascii	"cfg->fps_in_denorm >= 1\000"
.LC9:
	.ascii	"cfg->fps_out_denorm >= 1\000"
.LC10:
	.ascii	"rate_in >= rate_out\000"
.LC11:
	.ascii	"\0016%s:%d: leave %p drop %d\012\000"
	.section	__param,"a"
	.align	2
	.type	__param_rc_debug, %object
	.size	__param_rc_debug, 20
__param_rc_debug:
	.word	__param_str_rc_debug
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	rc_debug
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
