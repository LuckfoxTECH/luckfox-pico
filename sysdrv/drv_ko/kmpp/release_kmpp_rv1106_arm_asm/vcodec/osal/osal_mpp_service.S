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
	.file	"osal_mpp_service.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_service_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_deinit, %function
mpp_service_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r2, [r0, #4]
	cbz	r2, .L2
	ldr	r3, [r0, #932]
	ldr	r3, [r3, #8]
	cbz	r3, .L2
	mov	r0, r2
	blx	r3
.L2:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	mpp_service_deinit, .-mpp_service_deinit
	.align	1
	.global	mpp_service_chnl_register
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_chnl_register, %function
mpp_service_chnl_register:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0
	ldr	r0, [r0, #4]
	cbz	r0, .L9
	ldr	r3, [r3, #932]
	ldr	r3, [r3, #4]
	cbz	r3, .L9
	bx	r3
.L9:
	bx	lr
	.fnend
	.size	mpp_service_chnl_register, .-mpp_service_chnl_register
	.align	1
	.global	mpp_service_iova_address
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_iova_address, %function
mpp_service_iova_address:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #932]
	ldr	r3, [r3, #16]
	cbz	r3, .L18
	ldr	r0, [r0, #4]
	bx	r3
.L18:
	mov	r0, r3
	bx	lr
	.fnend
	.size	mpp_service_iova_address, .-mpp_service_iova_address
	.align	1
	.global	mpp_service_get_dev
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_get_dev, %function
mpp_service_get_dev:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #932]
	ldr	r3, [r3, #24]
	cbz	r3, .L20
	ldr	r0, [r0, #4]
	bx	r3
.L20:
	mov	r0, r3
	bx	lr
	.fnend
	.size	mpp_service_get_dev, .-mpp_service_get_dev
	.align	1
	.global	mpp_service_run_task
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_run_task, %function
mpp_service_run_task:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #932]
	ldr	r3, [r3, #28]
	cbz	r3, .L22
	ldr	r0, [r0, #4]
	bx	r3
.L22:
	mov	r0, r3
	bx	lr
	.fnend
	.size	mpp_service_run_task, .-mpp_service_run_task
	.align	1
	.global	mpp_service_chnl_check_running
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_chnl_check_running, %function
mpp_service_chnl_check_running:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #932]
	ldr	r3, [r3, #32]
	cbz	r3, .L24
	ldr	r0, [r0, #4]
	bx	r3
.L24:
	mov	r0, r3
	bx	lr
	.fnend
	.size	mpp_service_chnl_check_running, .-mpp_service_chnl_check_running
	.align	1
	.global	mpp_service_chnl_control
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_chnl_control, %function
mpp_service_chnl_control:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {lr}
	.pad #28
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r3, #0
	str	r3, [sp, #20]
	ldr	r3, [r0, #932]
	str	r2, [sp, #16]
	str	r1, [sp]
	ldr	r3, [r3, #12]
	cbz	r3, .L27
	mov	r1, sp
	ldr	r0, [r0, #4]
	blx	r3
.L25:
	add	sp, sp, #28
	@ sp needed
	ldr	pc, [sp], #4
.L27:
	mov	r0, r3
	b	.L25
	.fnend
	.size	mpp_service_chnl_control, .-mpp_service_chnl_control
	.align	1
	.global	mpp_service_cmd_send
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_cmd_send, %function
mpp_service_cmd_send:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r0
	ldr	r2, [r0, #8]
	subs	r3, r2, #1
	cmp	r3, #15
	bls	.L29
	str	r2, [sp]
	mov	r3, r0
	mov	r2, #408
	ldr	r1, .L40
	ldr	r0, .L40+4
	bl	printk
	mvn	r0, #5
.L28:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L29:
	ldr	r1, [r0, #400]
	cbz	r1, .L31
	movs	r3, #24
	movw	r5, #515
	mla	r3, r3, r2, r0
	lsls	r1, r1, #3
	movs	r0, #0
	adds	r2, r2, #1
	strd	r5, r0, [r3, #16]
	add	r5, r4, #404
	strd	r1, r0, [r3, #24]
	strd	r5, r0, [r3, #32]
	str	r2, [r4, #8]
.L31:
	ldr	r3, [r4, #8]
	cmp	r3, #1
	ble	.L32
	movs	r1, #24
	mov	r2, r4
	mla	r1, r1, r3, r4
.L33:
	ldr	r0, [r2, #20]
	adds	r2, r2, #24
	orr	r0, r0, #1
	str	r0, [r2, #-4]
	cmp	r1, r2
	bne	.L33
.L32:
	subs	r3, r3, #1
	movs	r2, #24
	mla	r3, r2, r3, r4
	ldr	r2, [r3, #20]
	orr	r2, r2, #6
	str	r2, [r3, #20]
	ldr	r3, [r4, #932]
	ldr	r3, [r3, #12]
	cbz	r3, .L35
	add	r1, r4, #16
	ldr	r0, [r4, #4]
	blx	r3
.L34:
	movs	r3, #0
	strd	r3, r3, [r4, #8]
	str	r3, [r4, #400]
	b	.L28
.L35:
	mov	r0, r3
	b	.L34
.L41:
	.align	2
.L40:
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	mpp_service_cmd_send, .-mpp_service_cmd_send
	.align	1
	.global	mpp_service_reg_wr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_reg_wr, %function
mpp_service_reg_wr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, [r0, #8]
	mov	r6, r1
	cbnz	r4, .L43
	mov	r2, #384
	mov	r1, r4
	adds	r0, r0, #16
	bl	memset
.L43:
	movs	r3, #24
	movs	r0, #0
	mla	r3, r3, r4, r5
	mov	r2, #512
	adds	r4, r4, #1
	strd	r2, r0, [r3, #16]
	ldr	r2, [r6, #4]
	str	r2, [r3, #24]
	ldr	r2, [r6, #8]
	str	r2, [r3, #28]
	ldr	r1, [r6]
	strd	r1, r0, [r3, #32]
	str	r4, [r5, #8]
	pop	{r4, r5, r6, pc}
	.fnend
	.size	mpp_service_reg_wr, .-mpp_service_reg_wr
	.align	1
	.global	mpp_service_reg_rd
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_reg_rd, %function
mpp_service_reg_rd:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, [r0, #8]
	mov	r6, r1
	cbnz	r4, .L45
	mov	r2, #384
	mov	r1, r4
	adds	r0, r0, #16
	bl	memset
.L45:
	movs	r3, #24
	movs	r0, #0
	mla	r3, r3, r4, r5
	movw	r2, #513
	adds	r4, r4, #1
	strd	r2, r0, [r3, #16]
	ldr	r2, [r6, #4]
	str	r2, [r3, #24]
	ldr	r2, [r6, #8]
	str	r2, [r3, #28]
	ldr	r1, [r6]
	strd	r1, r0, [r3, #32]
	str	r4, [r5, #8]
	pop	{r4, r5, r6, pc}
	.fnend
	.size	mpp_service_reg_rd, .-mpp_service_reg_rd
	.align	1
	.global	mpp_service_cmd_poll
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_cmd_poll, %function
mpp_service_cmd_poll:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	.pad #24
	sub	sp, sp, #24
	mov	r4, r0
	movs	r2, #16
	movs	r1, #0
	add	r0, sp, #8
	bl	memset
	movs	r3, #2
	mov	r2, #768
	strd	r2, r3, [sp]
	ldr	r3, [r4, #932]
	ldr	r3, [r3, #12]
	cbz	r3, .L47
	mov	r1, sp
	ldr	r0, [r4, #4]
	blx	r3
.L47:
	movs	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, pc}
	.fnend
	.size	mpp_service_cmd_poll, .-mpp_service_cmd_poll
	.align	1
	.global	mpp_service_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_init, %function
mpp_service_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	get_mppdev_svr_ops
	str	r0, [r4, #932]
	cbnz	r0, .L52
	ldr	r0, .L60
	mov	r1, #286
.L59:
	bl	printk
	mov	r0, #-1
.L51:
	pop	{r3, r4, r5, pc}
.L52:
	ldr	r3, [r0]
	cbz	r3, .L54
	mov	r0, r5
	blx	r3
	str	r0, [r4, #4]
.L54:
	ldr	r3, [r4, #4]
	cbnz	r3, .L55
	mov	r1, #296
	ldr	r0, .L60+4
	b	.L59
.L55:
	movs	r0, #0
	b	.L51
.L61:
	.align	2
.L60:
	.word	.LC1
	.word	.LC2
	.fnend
	.size	mpp_service_init, .-mpp_service_init
	.align	1
	.global	mpp_service_rcb_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_rcb_info, %function
mpp_service_rcb_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r3, [r0, #928]
	cbz	r3, .L67
	ldr	r3, [r1, #4]
	cbz	r3, .L66
	ldr	r3, [r0, #400]
	cmp	r3, #31
	ble	.L64
	movw	r2, #375
	ldr	r1, .L68
	ldr	r0, .L68+4
	bl	printk
	mov	r0, #-1
.L62:
	pop	{r3, pc}
.L64:
	adds	r2, r3, #1
	str	r2, [r0, #400]
	ldr	r2, [r1]
	add	r0, r0, r3, lsl #3
	str	r2, [r0, #404]
	ldr	r3, [r1, #4]
	str	r3, [r0, #408]
.L67:
	movs	r0, #0
	b	.L62
.L66:
	mov	r0, r3
	b	.L62
.L69:
	.align	2
.L68:
	.word	.LANCHOR0+21
	.word	.LC3
	.fnend
	.size	mpp_service_rcb_info, .-mpp_service_rcb_info
	.align	1
	.global	mpp_service_set_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_set_info, %function
mpp_service_set_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r3, [r0, #924]
	mov	r6, r1
	cbz	r3, .L71
	ldr	r4, [r0, #660]
	cbnz	r4, .L72
	mov	r2, #256
	mov	r1, r4
	add	r0, r0, #664
	bl	memset
.L72:
	add	r4, r5, r4, lsl #4
	add	r3, r6, #16
	add	r4, r4, #664
	mov	r1, r6
.L73:
	ldr	r2, [r1], #4	@ unaligned
	cmp	r1, r3
	str	r2, [r4], #4	@ unaligned
	bne	.L73
	ldr	r3, [r5, #660]
	adds	r3, r3, #1
	str	r3, [r5, #660]
.L71:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
	.fnend
	.size	mpp_service_set_info, .-mpp_service_set_info
	.global	mpp_service_api
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.7937, %object
	.size	__func__.7937, 21
__func__.7937:
	.ascii	"mpp_service_cmd_send\000"
	.type	__func__.7926, %object
	.size	__func__.7926, 21
__func__.7926:
	.ascii	"mpp_service_rcb_info\000"
	.space	2
	.type	mpp_service_api, %object
	.size	mpp_service_api, 68
mpp_service_api:
	.word	.LC4
	.word	936
	.word	mpp_service_init
	.word	mpp_service_deinit
	.word	mpp_service_reg_wr
	.word	mpp_service_reg_rd
	.word	0
	.word	mpp_service_rcb_info
	.word	mpp_service_set_info
	.word	mpp_service_cmd_send
	.word	mpp_service_cmd_poll
	.word	mpp_service_iova_address
	.word	mpp_service_chnl_register
	.word	mpp_service_get_dev
	.word	mpp_service_run_task
	.word	mpp_service_chnl_check_running
	.word	mpp_service_chnl_control
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: ctx %p invalid request count %d\012\000"
.LC1:
	.ascii	"\0013%d: get_mppdev_svr_ops fail\000"
.LC2:
	.ascii	"\0013%d: mpp_chnl_open fail\000"
.LC3:
	.ascii	"\0013%s:%d: reach max offset definition\012\000"
.LC4:
	.ascii	"mpp_service\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
