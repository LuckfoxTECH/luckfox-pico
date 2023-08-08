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
	.file	"mpp_device.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_dev_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_init, %function
mpp_dev_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	mov	r7, r0
	mov	r8, r1
	ldr	r0, .L14
	movs	r1, #12
	bl	mpp_osal_calloc
	cbnz	r7, .L2
	movs	r2, #36
	ldr	r1, .L14+4
	ldr	r0, .L14+8
	bl	printk
	mvn	r0, #2
.L1:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L2:
	movs	r3, #0
	movs	r1, #12
	str	r3, [r7]
	ldr	r0, .L14
	bl	mpp_osal_calloc
	ldr	r6, .L14+12
	ldr	r1, [r6, #4]
	mov	r4, r0
	ldr	r0, .L14
	bl	mpp_osal_calloc
	mov	r5, r0
	cbz	r4, .L4
	cbnz	r0, .L5
.L4:
	str	r5, [sp]
	mov	r3, r4
	movs	r2, #46
	ldr	r1, .L14+4
	ldr	r0, .L14+16
	bl	printk
	cbz	r4, .L6
	mov	r1, r4
	ldr	r0, .L14
	bl	mpp_osal_free
.L6:
	cbz	r5, .L7
	mov	r1, r5
	ldr	r0, .L14
	bl	mpp_osal_free
.L7:
	mvn	r0, #3
	b	.L1
.L5:
	ldr	r3, [r6, #8]
	mov	r1, r8
	strd	r0, r6, [r4, #4]
	str	r8, [r4]
	str	r3, [sp, #12]
	str	r4, [r7]
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	bx	r3
.L15:
	.align	2
.L14:
	.word	.LANCHOR0
	.word	.LANCHOR0+13
	.word	.LC0
	.word	mpp_service_api
	.word	.LC1
	.fnend
	.size	mpp_dev_init, .-mpp_dev_init
	.align	1
	.global	mpp_dev_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_deinit, %function
mpp_dev_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	cbnz	r0, .L17
	movs	r2, #66
	ldr	r1, .L27
	ldr	r0, .L27+4
	mvn	r4, #2
	bl	printk
.L16:
	mov	r0, r4
	pop	{r3, r4, r5, pc}
.L17:
	ldr	r4, [r0, #8]
	cbz	r4, .L19
	ldr	r4, [r4, #12]
	cbz	r4, .L19
	ldr	r0, [r0, #4]
	cbz	r0, .L23
	blx	r4
.L23:
	mov	r4, r0
.L19:
	ldr	r1, [r5, #4]
	cbz	r1, .L20
	ldr	r0, .L27+8
	bl	mpp_osal_free
.L20:
	movs	r3, #0
	mov	r1, r5
	str	r3, [r5, #4]
	ldr	r0, .L27+8
	bl	mpp_osal_free
	b	.L16
.L28:
	.align	2
.L27:
	.word	.LANCHOR0+26
	.word	.LC0
	.word	.LANCHOR0+41
	.fnend
	.size	mpp_dev_deinit, .-mpp_dev_deinit
	.align	1
	.global	mpp_dev_ioctl
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_ioctl, %function
mpp_dev_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r3, r1
	mov	r1, r2
	push	{r4, lr}
	.save {r4, lr}
	cbz	r0, .L51
	ldr	r4, [r0, #8]
	cbz	r4, .L41
	ldr	r0, [r0, #4]
	cmp	r3, #6
	bhi	.L31
	tbb	[pc, r3]
.L33:
	.byte	(.L39-.L33)/2
	.byte	(.L38-.L33)/2
	.byte	(.L37-.L33)/2
	.byte	(.L36-.L33)/2
	.byte	(.L35-.L33)/2
	.byte	(.L34-.L33)/2
	.byte	(.L32-.L33)/2
	.p2align 1
.L39:
	ldr	r2, [r4, #16]
	cbz	r2, .L30
	pop	{r4, lr}
	bx	r2
.L38:
	ldr	r3, [r4, #20]
.L57:
	cbz	r3, .L30
	pop	{r4, lr}
	bx	r3
.L37:
	ldr	r3, [r4, #24]
	b	.L57
.L36:
	ldr	r3, [r4, #28]
	b	.L57
.L35:
	ldr	r3, [r4, #32]
	b	.L57
.L34:
	ldr	r3, [r4, #36]
.L61:
	cbz	r3, .L30
	pop	{r4, lr}
	bx	r3
.L32:
	ldr	r3, [r4, #40]
	b	.L61
.L31:
	movs	r2, #126
	ldr	r1, .L62
	ldr	r0, .L62+4
	bl	printk
.L51:
	movs	r3, #0
.L30:
	mov	r0, r3
	pop	{r4, pc}
.L41:
	mov	r3, r4
	b	.L30
.L63:
	.align	2
.L62:
	.word	.LANCHOR0+56
	.word	.LC2
	.fnend
	.size	mpp_dev_ioctl, .-mpp_dev_ioctl
	.align	1
	.global	mpp_dev_set_reg_offset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_set_reg_offset, %function
mpp_dev_set_reg_offset:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	strd	r1, r2, [sp]
	mov	r2, sp
	movs	r1, #2
	bl	mpp_dev_ioctl
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
	.fnend
	.size	mpp_dev_set_reg_offset, .-mpp_dev_set_reg_offset
	.align	1
	.global	mpp_dev_get_iova_address
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_get_iova_address, %function
mpp_dev_get_iova_address:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r8, r2
	ldrd	r7, r9, [r0, #4]
	mov	r5, r1
	cbnz	r1, .L66
	movs	r2, #154
	ldr	r1, .L76
	ldr	r0, .L76+4
	bl	printk
	mvn	r0, #21
.L65:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L66:
	ldr	r1, .L76+8
	mov	r0, r5
	bl	mpp_buffer_get_phy_caller
	adds	r2, r0, #1
	mov	r4, r0
	bne	.L68
	ldr	r1, .L76+8
	mov	r0, r5
	bl	mpp_buffer_get_dma_with_caller
	mov	r6, r0
	cbnz	r0, .L69
	movs	r1, #161
	ldr	r3, .L76+8
	str	r1, [sp]
	ldr	r2, .L76+12
	ldr	r0, .L76+16
	bl	printk
.L69:
	ldr	r3, [r9, #44]
	cbz	r3, .L68
	mov	r2, r8
	mov	r1, r6
	mov	r0, r7
	blx	r3
	adds	r3, r0, #1
	mov	r4, r0
	beq	.L68
	mov	r1, r0
	ldr	r2, .L76+8
	mov	r0, r5
	bl	mpp_buffer_set_phy_caller
.L68:
	mov	r0, r4
	b	.L65
.L77:
	.align	2
.L76:
	.word	.LANCHOR0+70
	.word	.LC3
	.word	.LANCHOR0+95
	.word	.LC4
	.word	.LC5
	.fnend
	.size	mpp_dev_get_iova_address, .-mpp_dev_get_iova_address
	.align	1
	.global	mpp_dev_get_iova_address2
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_get_iova_address2, %function
mpp_dev_get_iova_address2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r6, r2
	ldrd	r5, r7, [r0, #4]
	mov	r4, r1
	cbnz	r1, .L79
	movs	r1, #176
	ldr	r3, .L81
	str	r1, [sp]
	ldr	r2, .L81+4
	ldr	r0, .L81+8
	bl	printk
.L79:
	ldr	r3, [r7, #44]
	cbz	r3, .L80
	mov	r2, r6
	mov	r1, r4
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	bx	r3
.L80:
	mvn	r0, #21
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L82:
	.align	2
.L81:
	.word	.LANCHOR0+120
	.word	.LC4
	.word	.LC5
	.fnend
	.size	mpp_dev_get_iova_address2, .-mpp_dev_get_iova_address2
	.align	1
	.global	mpp_dev_get_mpi_ioaddress
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_get_mpi_ioaddress, %function
mpp_dev_get_mpi_ioaddress:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r6, r2
	ldrd	r5, r7, [r0, #4]
	mov	r0, r1
	cbnz	r1, .L84
	movs	r2, #189
	ldr	r1, .L89
	ldr	r0, .L89+4
	bl	printk
.L83:
	mvn	r0, #21
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L84:
	ldr	r1, .L89+8
	bl	mpi_buf_get_dma_with_caller
	mov	r4, r0
	cbnz	r0, .L86
	movs	r1, #193
	ldr	r3, .L89+8
	str	r1, [sp]
	ldr	r2, .L89+12
	ldr	r0, .L89+16
	bl	printk
.L86:
	ldr	r3, [r7, #44]
	cmp	r3, #0
	beq	.L83
	mov	r2, r6
	mov	r1, r4
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	bx	r3
.L90:
	.align	2
.L89:
	.word	.LANCHOR0+146
	.word	.LC3
	.word	.LANCHOR0+172
	.word	.LC4
	.word	.LC5
	.fnend
	.size	mpp_dev_get_mpi_ioaddress, .-mpp_dev_get_mpi_ioaddress
	.align	1
	.global	mpp_dev_chnl_register
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_dev_chnl_register, %function
mpp_dev_chnl_register:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #8]
	ldr	r3, [r3, #48]
	cbz	r3, .L91
	ldr	r0, [r0, #4]
	bx	r3	@ indirect register sibling call
.L91:
	bx	lr
	.fnend
	.size	mpp_dev_chnl_register, .-mpp_dev_chnl_register
	.align	1
	.global	mpp_get_dev
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_get_dev, %function
mpp_get_dev:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, [r0, #8]
	ldr	r3, [r3, #52]
	cbz	r3, .L94
	ldr	r0, [r0, #4]
	bx	r3
.L94:
	mov	r0, r3
	bx	lr
	.fnend
	.size	mpp_get_dev, .-mpp_get_dev
	.global	mpp_device_debug
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.7781, %object
	.size	__FUNCTION__.7781, 13
__FUNCTION__.7781:
	.ascii	"mpp_dev_init\000"
	.type	__func__.7783, %object
	.size	__func__.7783, 13
__func__.7783:
	.ascii	"mpp_dev_init\000"
	.type	__func__.7789, %object
	.size	__func__.7789, 15
__func__.7789:
	.ascii	"mpp_dev_deinit\000"
	.type	__FUNCTION__.7790, %object
	.size	__FUNCTION__.7790, 15
__FUNCTION__.7790:
	.ascii	"mpp_dev_deinit\000"
	.type	__func__.7809, %object
	.size	__func__.7809, 14
__func__.7809:
	.ascii	"mpp_dev_ioctl\000"
	.type	__func__.7826, %object
	.size	__func__.7826, 25
__func__.7826:
	.ascii	"mpp_dev_get_iova_address\000"
	.type	__FUNCTION__.7827, %object
	.size	__FUNCTION__.7827, 25
__FUNCTION__.7827:
	.ascii	"mpp_dev_get_iova_address\000"
	.type	__FUNCTION__.7838, %object
	.size	__FUNCTION__.7838, 26
__FUNCTION__.7838:
	.ascii	"mpp_dev_get_iova_address2\000"
	.type	__func__.7850, %object
	.size	__func__.7850, 26
__func__.7850:
	.ascii	"mpp_dev_get_mpi_ioaddress\000"
	.type	__FUNCTION__.7851, %object
	.size	__FUNCTION__.7851, 26
__FUNCTION__.7851:
	.ascii	"mpp_dev_get_mpi_ioaddress\000"
	.bss
	.align	2
	.type	mpp_device_debug, %object
	.size	mpp_device_debug, 4
mpp_device_debug:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: found NULL input ctx\012\000"
.LC1:
	.ascii	"\0013%s:%d: malloc failed impl %p impl_ctx %p\012\000"
.LC2:
	.ascii	"\0013%s:%d: invalid cmd %d\012\000"
.LC3:
	.ascii	"\0013%s:%d: input NULL\000"
.LC4:
	.ascii	"dma_buf\000"
.LC5:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
