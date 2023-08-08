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
	.file	"vepu_pp_service.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	pp_service_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_service_deinit, %function
pp_service_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r2, [r0, #4]
	cbz	r2, .L2
	ldr	r3, [r0, #400]
	ldr	r3, [r3, #8]
	cbz	r3, .L2
	mov	r0, r2
	blx	r3
.L2:
	movs	r0, #0
	pop	{r3, pc}
	.fnend
	.size	pp_service_deinit, .-pp_service_deinit
	.align	1
	.global	pp_service_iova_address
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_service_iova_address, %function
pp_service_iova_address:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #400]
	ldr	r3, [r3, #16]
	cbz	r3, .L10
	ldr	r0, [r0, #4]
	bx	r3
.L10:
	mov	r0, r3
	bx	lr
	.fnend
	.size	pp_service_iova_address, .-pp_service_iova_address
	.align	1
	.global	pp_service_iova_release
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_service_iova_release, %function
pp_service_iova_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #400]
	ldr	r3, [r3, #20]
	cbz	r3, .L11
	ldr	r0, [r0, #4]
	bx	r3	@ indirect register sibling call
.L11:
	bx	lr
	.fnend
	.size	pp_service_iova_release, .-pp_service_iova_release
	.align	1
	.global	pp_service_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_service_init, %function
pp_service_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	mov	r5, r1
	bl	get_mppdev_svr_ops
	str	r0, [r4, #400]
	cbnz	r0, .L14
	ldr	r0, .L22
.L21:
	bl	printk
	mov	r0, #-1
.L13:
	pop	{r3, r4, r5, pc}
.L14:
	ldr	r3, [r0]
	cbz	r3, .L16
	mov	r0, r5
	blx	r3
	str	r0, [r4, #4]
.L16:
	ldr	r3, [r4, #4]
	cbnz	r3, .L17
	ldr	r0, .L22+4
	b	.L21
.L17:
	movs	r0, #0
	b	.L13
.L23:
	.align	2
.L22:
	.word	.LC0
	.word	.LC1
	.fnend
	.size	pp_service_init, .-pp_service_init
	.align	1
	.global	pp_service_cmd_send
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_service_cmd_send, %function
pp_service_cmd_send:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r0
	ldr	r3, [r0, #8]
	subs	r2, r3, #1
	cmp	r2, #15
	bls	.L25
	str	r3, [sp]
	movs	r2, #120
	mov	r3, r0
	ldr	r1, .L32
	ldr	r0, .L32+4
	bl	printk
	mov	r0, #-1
.L24:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L25:
	cmp	r3, #1
	ble	.L27
	mov	r1, r0
	movs	r0, #24
	mla	r3, r0, r3, r4
.L28:
	ldr	r0, [r1, #20]
	adds	r1, r1, #24
	orr	r0, r0, #1
	str	r0, [r1, #-4]
	cmp	r3, r1
	bne	.L28
.L27:
	movs	r3, #24
	mla	r3, r3, r2, r4
	ldr	r2, [r3, #20]
	orr	r2, r2, #6
	str	r2, [r3, #20]
	ldr	r3, [r4, #400]
	ldr	r3, [r3, #12]
	cbz	r3, .L30
	add	r1, r4, #16
	ldr	r0, [r4, #4]
	blx	r3
.L29:
	movs	r3, #0
	strd	r3, r3, [r4, #8]
	b	.L24
.L30:
	mov	r0, r3
	b	.L29
.L33:
	.align	2
.L32:
	.word	.LANCHOR0
	.word	.LC2
	.fnend
	.size	pp_service_cmd_send, .-pp_service_cmd_send
	.align	1
	.global	pp_service_reg_wr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_service_reg_wr, %function
pp_service_reg_wr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, [r0, #8]
	mov	r6, r1
	cbnz	r4, .L35
	mov	r2, #384
	mov	r1, r4
	adds	r0, r0, #16
	bl	memset
.L35:
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
	.size	pp_service_reg_wr, .-pp_service_reg_wr
	.align	1
	.global	pp_service_reg_rd
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_service_reg_rd, %function
pp_service_reg_rd:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, [r0, #8]
	mov	r6, r1
	cbnz	r4, .L37
	mov	r2, #384
	mov	r1, r4
	adds	r0, r0, #16
	bl	memset
.L37:
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
	.size	pp_service_reg_rd, .-pp_service_reg_rd
	.align	1
	.global	pp_service_cmd_poll
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	pp_service_cmd_poll, %function
pp_service_cmd_poll:
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
	ldr	r3, [r4, #400]
	ldr	r3, [r3, #12]
	cbz	r3, .L39
	mov	r1, sp
	ldr	r0, [r4, #4]
	blx	r3
.L39:
	movs	r0, #0
	add	sp, sp, #24
	@ sp needed
	pop	{r4, pc}
	.fnend
	.size	pp_service_cmd_poll, .-pp_service_cmd_poll
	.global	pp_srv_api
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.7417, %object
	.size	__func__.7417, 20
__func__.7417:
	.ascii	"pp_service_cmd_send\000"
	.type	pp_srv_api, %object
	.size	pp_srv_api, 40
pp_srv_api:
	.word	.LC3
	.word	408
	.word	pp_service_init
	.word	pp_service_deinit
	.word	pp_service_reg_wr
	.word	pp_service_reg_rd
	.word	pp_service_cmd_send
	.word	pp_service_cmd_poll
	.word	pp_service_iova_address
	.word	pp_service_iova_release
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013get_mppdev_svr_ops fail\000"
.LC1:
	.ascii	"\0013mpp_chnl_open fail\000"
.LC2:
	.ascii	"\0013%s:%d: ctx %p invalid request count %d\012\000"
.LC3:
	.ascii	"vepu_pp\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
