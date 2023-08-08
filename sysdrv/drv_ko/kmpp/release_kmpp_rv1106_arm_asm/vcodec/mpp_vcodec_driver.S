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
	.file	"mpp_vcodec_driver.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vmpi_register_fn2vcocdec:					
	.asciz 	"vmpi_register_fn2vcocdec"					
__kstrtabns_vmpi_register_fn2vcocdec:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vmpi_unregister_fn2vcocdec:					
	.asciz 	"vmpi_unregister_fn2vcocdec"					
__kstrtabns_vmpi_unregister_fn2vcocdec:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vsm_buf_register_fn2vcocdec:					
	.asciz 	"vsm_buf_register_fn2vcocdec"					
__kstrtabns_vsm_buf_register_fn2vcocdec:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vdec_proc_debug, %function
vdec_proc_debug:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vdec_proc_debug, .-vdec_proc_debug
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.global	vsm_buf_register_fn2vcocdec
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vsm_buf_register_fn2vcocdec, %function
vsm_buf_register_fn2vcocdec:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L3
	movw	r1, #550
	str	r0, [r3]
	ldr	r0, .L3+4
	b	printk
.L4:
	.align	2
.L3:
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	vsm_buf_register_fn2vcocdec, .-vsm_buf_register_fn2vcocdec
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_copy_from_user, %function
_copy_from_user:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r3, sp
	bic	r3, r3, #8160
	mov	r6, r0
	bic	r3, r3, #31
	mov	r4, r2
	ldr	r3, [r3, #8]
	.syntax unified
@ 157 "./include/linux/uaccess.h" 1
	.syntax unified
adds r5, r1, r2; sbcscc r5, r5, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L8
	bl	arm_copy_from_user
	mov	r5, r0
.L6:
	cbz	r5, .L5
	subs	r0, r4, r5
	mov	r2, r5
	movs	r1, #0
	add	r0, r0, r6
	bl	memset
.L5:
	mov	r0, r5
	pop	{r4, r5, r6, pc}
.L8:
	mov	r5, r2
	b	.L6
	.fnend
	.size	_copy_from_user, .-_copy_from_user
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	_copy_to_user, %function
_copy_to_user:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r4, r5}
	.save {r4, r5}
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r3, [r3, #8]
	.syntax unified
@ 177 "./include/linux/uaccess.h" 1
	.syntax unified
adds r5, r0, r2; sbcscc r5, r5, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L13
	pop	{r4, r5}
	b	arm_copy_to_user
.L13:
	mov	r0, r2
	pop	{r4, r5}
	bx	lr
	.fnend
	.size	_copy_to_user, .-_copy_to_user
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_poll, %function
vcodec_poll:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r0
	ldr	r3, [r0, #128]
	mov	r5, r1
	ldrd	r0, r1, [r3]
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #88]
	add	r6, r0, #88
	mov	r4, r0
	cmp	r6, r3
	beq	.L15
.L19:
	movs	r0, #65
.L14:
	pop	{r3, r4, r5, r6, r7, pc}
.L15:
	ldr	r3, [r0, #24]
	cmp	r3, #2
	beq	.L17
	cbz	r5, .L18
	ldr	r3, [r5]
	cbz	r3, .L18
	adds	r1, r0, #48
	beq	.L18
	mov	r2, r5
	mov	r0, r7
	blx	r3
.L18:
	ldr	r3, [r4, #88]
	cmp	r6, r3
	bne	.L19
	ldr	r3, [r4, #24]
	cmp	r3, #2
	bne	.L20
.L17:
	movs	r0, #25
	b	.L14
.L20:
	movs	r0, #0
	b	.L14
	.fnend
	.size	vcodec_poll, .-vcodec_poll
	.align	1
	.global	vmpi_register_fn2vcocdec
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmpi_register_fn2vcocdec, %function
vmpi_register_fn2vcocdec:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L35
	strd	r0, r1, [r3, #4]
	cbz	r0, .L33
	b	vcodec_create_mpi_dev
.L33:
	bx	lr
.L36:
	.align	2
.L35:
	.word	.LANCHOR0
	.fnend
	.size	vmpi_register_fn2vcocdec, .-vmpi_register_fn2vcocdec
	.align	1
	.global	vmpi_unregister_fn2vcocdec
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmpi_unregister_fn2vcocdec, %function
vmpi_unregister_fn2vcocdec:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	mpp_vcodec_unregister_mpidev
	movs	r2, #0
	ldr	r3, .L38
	strd	r2, r2, [r3, #4]
	pop	{r3, pc}
.L39:
	.align	2
.L38:
	.word	.LANCHOR0
	.fnend
	.size	vmpi_unregister_fn2vcocdec, .-vmpi_unregister_fn2vcocdec
	.section	.exit.text,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_exit, %function
vcodec_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	mpp_vcodec_deinit
	ldr	r0, .L41
	bl	platform_driver_unregister
	ldr	r0, .L41+4
	bl	platform_driver_unregister
	ldr	r0, .L41+8
	pop	{r3, lr}
	b	printk
.L42:
	.align	2
.L41:
	.word	.LANCHOR1
	.word	mpp_service_driver
	.word	.LC1
	.fnend
	.size	vcodec_exit, .-vcodec_exit
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_remove, %function
vcodec_remove:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r0
	ldr	r4, [r0, #80]
	ldr	r0, [r4, #76]
	cbz	r0, .L44
	bl	proc_remove
	movs	r3, #0
	str	r3, [r4, #76]
.L44:
	ldr	r0, [r4, #80]
	cbz	r0, .L45
	bl	proc_remove
	movs	r3, #0
	str	r3, [r4, #80]
.L45:
	ldr	r0, [r4, #76]
	cbz	r0, .L46
	bl	proc_remove
	movs	r3, #0
	str	r3, [r4, #76]
.L46:
	add	r0, r5, #16
	ldr	r1, .L56
	mov	r5, r4
	bl	_dev_info
	ldr	r0, [r5], #12
	ldr	r1, [r4, #8]
	bl	device_destroy
	mov	r0, r5
	bl	cdev_del
	movs	r1, #1
	ldr	r0, [r4, #8]
	bl	unregister_chrdev_region
	ldr	r0, [r4]
	bl	class_destroy
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L57:
	.align	2
.L56:
	.word	.LC2
	.fnend
	.size	vcodec_remove, .-vcodec_remove
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	venc_proc_debug, %function
venc_proc_debug:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #44
	sub	sp, sp, #44
	mov	r5, r0
	bl	mpp_vcodec_get_enc_module_entry
	ldr	r3, [r0, #1568]
	mov	r4, r0
	cbz	r3, .L59
	ldr	r1, .L69
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L69+4
	ldr	r2, .L69+8
	mov	r0, r5
	ldr	r1, .L69+12
	str	r3, [sp]
	ldr	r3, .L69+16
	bl	seq_printf
	ldr	r2, [r4, #1568]
	ldr	r3, [r2, #64]
	ldrd	r0, [r2, #48]
	strd	r0, [sp, #8]
	ldrd	r0, [r3, #48]
	strd	r0, [sp]
	mov	r0, r5
	ldrd	r2, [r3, #32]
	ldr	r1, .L69+20
	bl	seq_printf
.L59:
	ldr	r8, .L69+64
	movs	r6, #0
	ldr	r9, .L69+68
	ldr	r10, .L69+72
.L61:
	movs	r1, #1
	mov	r0, r6
	bl	mpp_vcodec_get_chan_entry
	ldr	r3, [r0, #40]
	mov	r4, r0
	cmp	r3, #0
	beq	.L60
	ldr	fp, [r0, #32]
	mov	r1, r8
	ldr	r7, [r0, #20]
	mov	r0, r5
	bl	seq_puts
	ldr	r3, .L69+24
	ldr	r2, .L69+28
	mov	r0, r5
	ldr	r1, .L69+32
	str	r3, [sp, #20]
	ldr	r3, .L69+36
	strd	r10, r9, [sp, #24]
	str	r3, [sp, #16]
	ldr	r3, .L69+40
	str	r3, [sp, #12]
	ldr	r3, .L69+44
	str	r3, [sp, #8]
	ldr	r3, .L69+48
	str	r3, [sp, #4]
	ldr	r3, .L69+52
	str	r3, [sp]
	ldr	r3, .L69+56
	bl	seq_printf
	ldrd	r2, [r4, #136]
	ldr	r1, [r4, #104]
	cmp	r7, #0
	ldr	r0, [r4, #108]
	ite	le
	movle	r7, #0
	movgt	r7, #1
	cmp	fp, #0
	strd	r2, [sp, #32]
	ldrd	r2, [r4, #128]
	strd	r2, [sp, #24]
	mov	r2, r6
	ldr	r3, [r4, #148]
	str	r3, [sp, #20]
	ldr	r3, [r4, #144]
	str	r1, [sp, #8]
	ldr	r1, .L69+60
	strd	r0, r3, [sp, #12]
	mov	r0, r5
	ldr	r3, [r4, #112]
	str	r7, [sp]
	str	r3, [sp, #4]
	ite	le
	movle	r3, #0
	movgt	r3, #1
	bl	seq_printf
	mov	r2, r6
	ldr	r1, [r4, #40]
	mov	r0, r5
	bl	mpp_enc_proc_debug
.L60:
	adds	r6, r6, #1
	cmp	r6, #8
	bne	.L61
	mov	r0, r5
	bl	mpp_packet_pool_proc
	movs	r0, #0
	add	sp, sp, #44
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L70:
	.align	2
.L69:
	.word	.LC3
	.word	.LC7
	.word	.LC5
	.word	.LC6
	.word	.LC4
	.word	.LC8
	.word	.LC15
	.word	.LC11
	.word	.LC12
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC10
	.word	.LC21
	.word	.LC9
	.word	.LC13
	.word	.LC14
	.fnend
	.size	venc_proc_debug, .-venc_proc_debug
	.section	.init.text,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_init, %function
vcodec_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r0, .L76
	bl	printk
	movs	r1, #0
	ldr	r0, .L76+4
	bl	__platform_driver_register
	mov	r4, r0
	mov	r1, r0
	cbz	r0, .L72
	ldr	r0, .L76+8
.L75:
	bl	printk
.L71:
	mov	r0, r4
	pop	{r4, pc}
.L72:
	ldr	r0, .L76+12
	bl	__platform_driver_register
	mov	r4, r0
	cbz	r0, .L74
	mov	r1, r0
	ldr	r0, .L76+16
	b	.L75
.L74:
	bl	mpp_vcodec_init
	b	.L71
.L77:
	.align	2
.L76:
	.word	.LC22
	.word	mpp_service_driver
	.word	.LC23
	.word	.LANCHOR1
	.word	.LC24
	.fnend
	.size	vcodec_init, .-vcodec_init
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_ioctl, %function
vcodec_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r8, r0
	ldr	r4, [r0, #128]
	.pad #36
	sub	sp, sp, #36
	mov	r5, r2
	cbnz	r4, .L79
	ldr	r0, .L138
	mov	r2, r4
	movs	r1, #251
.L136:
	bl	printk
	mvn	r4, #21
	b	.L78
.L79:
	ldr	r3, [r4, #16]
	cmp	r3, #0
	ble	.L81
	ldr	r2, [r4]
	movs	r1, #255
	ldr	r0, .L138+4
	mvn	r4, #15
	bl	printk
.L78:
	mov	r0, r4
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L81:
	ldr	r3, .L138+8
	cmp	r1, r3
	bne	.L82
	movs	r2, #24
	movs	r1, #0
	add	r0, sp, #8
	bl	memset
	mov	r1, r5
	movs	r2, #24
	add	r0, sp, #8
	bl	_copy_from_user
	mov	r5, r0
	cmp	r0, #0
	bne	.L83
	add	r9, r4, #20
	mov	r0, r9
	bl	down
	ldr	r8, [r8, #128]
	ldrd	r4, r10, [r8]
	ldr	r0, [r8, #8]
	ldrd	fp, r3, [sp, #8]
	ldr	r6, [sp, #16]
	ldr	r7, [sp, #24]
	str	r3, [sp, #4]
	cbz	r6, .L84
	ldr	r3, [r8, #12]
	cmp	r6, r3
	bls	.L84
	cbz	r0, .L85
	bl	kfree
	strd	r5, r5, [r8, #8]
.L85:
	mov	r1, #3520
	mov	r0, r6
	bl	__kmalloc
	mov	r5, r0
	cmp	r0, #0
	beq	.L112
	strd	r0, r6, [r8, #8]
.L87:
	movw	r3, #257
	cmp	fp, r3
	beq	.L88
	cmp	fp, #258
	bcs	.L89
	cmp	fp, #2
	beq	.L113
	bhi	.L90
	cmp	fp, #0
	beq	.L91
	cmp	fp, #1
	beq	.L92
.L93:
	mov	r2, fp
	movs	r1, #235
	ldr	r0, .L138+12
	bl	printk
	b	.L114
.L84:
	mov	r5, r0
	b	.L87
.L90:
	cmp	fp, #3
	beq	.L94
	cmp	fp, #256
	bne	.L93
	mov	r1, r10
	mov	r0, r4
	bl	mpp_vcodec_chan_start
	b	.L134
.L89:
	cmp	fp, #1024
	beq	.L96
	bhi	.L97
	cmp	fp, #258
	beq	.L98
	movw	r3, #259
	cmp	fp, r3
	bne	.L93
	mov	r1, r10
	mov	r0, r4
	bl	mpp_vcodec_chan_resume
	b	.L134
.L97:
	movw	r3, #1537
	cmp	fp, r3
	beq	.L100
	movw	r3, #1538
	cmp	fp, r3
	bne	.L93
	cbz	r7, .L135
	cmp	r6, #0
	blt	.L135
	mov	r2, r6
	mov	r1, r7
	mov	r0, r5
	bl	_copy_from_user
	cbnz	r0, .L135
	mov	r2, r5
	mov	r1, r10
	mov	r0, r4
	bl	mpp_vcodec_chan_put_stream
	b	.L134
.L91:
	cmp	r6, #0
	bge	.L102
.L135:
	mvn	r4, #13
.L86:
	mov	r0, r9
	bl	up
	cmp	r4, #0
	beq	.L78
.L83:
	mvn	r4, #13
	b	.L78
.L102:
	mov	r2, r6
	mov	r1, r7
	mov	r0, r5
	bl	_copy_from_user
	cmp	r0, #0
	bne	.L135
	cmp	r6, #60
	beq	.L104
	movs	r1, #121
	ldr	r0, .L138+16
	bl	printk
	b	.L135
.L104:
	ldr	r3, [r5]
	mov	r0, r5
	str	r3, [r8, #4]
	bl	mpp_vcodec_chan_create
	mov	r2, r6
	mov	r1, r5
	mov	r4, r0
	mov	r0, r7
	bl	_copy_to_user
	cbz	r0, .L105
	movs	r1, #128
.L137:
	ldr	r0, .L138+20
	bl	printk
.L114:
	mvn	r4, #21
	b	.L86
.L105:
	ldr	r3, [r5, #8]
	str	r3, [r8]
	ldr	r3, [r5, #48]
	str	r3, [r8, #32]
	b	.L86
.L92:
	mov	r1, r10
	mov	r0, r4
	bl	mpp_vcodec_chan_destory
.L134:
	mov	r4, r0
	b	.L86
.L88:
	mov	r1, r10
	mov	r0, r4
	bl	mpp_vcodec_chan_stop
	b	.L134
.L98:
	mov	r1, r10
	mov	r0, r4
	bl	mpp_vcodec_chan_pause
	b	.L134
.L94:
	cbnz	r7, .L106
.L108:
	mov	r0, r4
	mov	r3, r5
	ldr	r2, [sp, #4]
	mov	r1, r10
	bl	mpp_vcodec_chan_control
	mov	r4, r0
	cmp	r0, #0
	bne	.L86
	cbz	r7, .L113
	cmp	r6, #0
	blt	.L135
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	bl	_copy_to_user
	cmp	r0, #0
	bne	.L135
.L113:
	movs	r4, #0
	b	.L86
.L106:
	cmp	r6, #0
	blt	.L135
	mov	r2, r6
	mov	r1, r7
	mov	r0, r5
	bl	_copy_from_user
	cmp	r0, #0
	beq	.L108
	b	.L135
.L96:
	cmp	r7, #0
	beq	.L135
	cmp	r6, #0
	blt	.L135
	mov	r2, r6
	mov	r1, r7
	mov	r0, r5
	bl	_copy_from_user
	cmp	r0, #0
	bne	.L135
	mov	r1, r5
	mov	r0, r4
	bl	mpp_vcodec_chan_push_frm
	b	.L134
.L100:
	cmp	r7, #0
	beq	.L135
	mov	r2, r5
	mov	r1, r10
	mov	r0, r4
	bl	mpp_vcodec_chan_get_stream
	cmp	r0, #0
	bne	.L114
	cmp	r6, #0
	bge	.L110
.L111:
	movs	r1, #217
	b	.L137
.L110:
	mov	r2, r6
	mov	r1, r5
	mov	r0, r7
	bl	_copy_to_user
	cmp	r0, #0
	beq	.L113
	b	.L111
.L112:
	mvn	r4, #11
	b	.L86
.L82:
	mov	r2, r1
	ldr	r0, .L138+24
	movw	r1, #279
	b	.L136
.L139:
	.align	2
.L138:
	.word	.LC25
	.word	.LC26
	.word	1074025985
	.word	.LC29
	.word	.LC27
	.word	.LC28
	.word	.LC30
	.fnend
	.size	vcodec_ioctl, .-vcodec_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_close, %function
vcodec_close:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, [r1, #128]
	cbnz	r4, .L141
	movs	r1, #62
	ldr	r0, .L148
	bl	printk
	mvn	r0, #21
.L140:
	pop	{r4, pc}
.L141:
	add	r3, r4, #16
	pld	[r3]
	.syntax unified
@ 223 "./arch/arm/include/asm/atomic.h" 1
	@ atomic_add
1:	ldrex	r2, [r3]
	add	r2, r2, #1
	strex	r1, r2, [r3]
	teq	r1, #0
	bne	1b
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #32]
	cbnz	r3, .L143
	ldrd	r0, r1, [r4]
	bl	mpp_vcodec_chan_destory
.L143:
	ldr	r0, [r4, #8]
	cbz	r0, .L144
	bl	kfree
	movs	r3, #0
	strd	r3, r3, [r4, #8]
.L144:
	mov	r0, r4
	bl	kfree
	movs	r0, #0
	b	.L140
.L149:
	.align	2
.L148:
	.word	.LC31
	.fnend
	.size	vcodec_close, .-vcodec_close
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_open, %function
vcodec_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L153
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r1
	ldr	r0, [r3, #24]
	mov	r1, #3520
	bl	kmem_cache_alloc
	cbz	r0, .L152
	movs	r3, #0
	add	r2, r0, #24
	str	r0, [r4, #128]
	movs	r1, #1
	str	r3, [r0, #16]
	strd	r1, r2, [r0, #20]
	str	r2, [r0, #28]
	mov	r0, r3
.L150:
	pop	{r4, pc}
.L152:
	mvn	r0, #11
	b	.L150
.L154:
	.align	2
.L153:
	.word	kmalloc_caches
	.fnend
	.size	vcodec_open, .-vcodec_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vcodec_probe, %function
vcodec_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	add	r6, r0, #16
	mov	r5, r0
	mov	r2, #3520
	movs	r1, #88
	mov	r0, r6
	bl	devm_kmalloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L166
	str	r0, [r5, #80]
	movs	r0, #0
	ldr	r2, .L180
	ldr	r1, .L180+4
	bl	__class_create
	cmn	r0, #4096
	str	r0, [r4]
	bls	.L157
	cbz	r0, .L157
	mov	r5, r0
.L155:
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L157:
	ldr	r8, .L180+20
	movs	r2, #1
	str	r6, [r4, #4]
	movs	r1, #0
	add	r0, r4, #8
	mov	r3, r8
	bl	alloc_chrdev_region
	mov	r5, r0
	cbz	r0, .L158
	ldr	r1, .L180+8
.L179:
	mov	r0, r6
	bl	_dev_err
	b	.L155
.L158:
	ldr	r9, .L180+52
	add	r7, r4, #12
	mov	r0, r7
	mov	r1, r9
	bl	cdev_init
	strd	r5, r9, [r4, #48]
	movs	r2, #1
	ldr	r1, [r4, #8]
	mov	r0, r7
	bl	cdev_add
	mov	r5, r0
	cbz	r0, .L159
	movs	r1, #1
	ldr	r0, [r4, #8]
	bl	unregister_chrdev_region
	ldr	r1, .L180+12
	b	.L179
.L159:
	ldr	r3, .L180+16
	mov	r1, r6
	str	r8, [sp, #4]
	str	r3, [sp]
	mov	r3, r0
	ldr	r2, [r4, #8]
	ldr	r0, [r4]
	bl	device_create
	mov	r1, r5
	str	r0, [r4, #72]
	mov	r0, r8
	bl	proc_mkdir
	mov	r1, r0
	str	r0, [r4, #76]
	cbz	r0, .L160
	cmn	r0, #4096
	bls	.L161
.L160:
	ldr	r2, .L180+20
	mov	r1, #418
	ldr	r0, .L180+24
	bl	printk
	movs	r3, #0
	str	r3, [r4, #76]
	b	.L155
.L161:
	ldr	r0, .L180+28
	bl	proc_mkdir
	str	r0, [r4, #84]
	cbz	r0, .L162
	cmn	r0, #4096
	bls	.L163
.L162:
	ldr	r2, .L180+28
	movw	r1, #425
	ldr	r0, .L180+24
	bl	printk
	movs	r3, #0
	str	r3, [r4, #84]
	b	.L155
.L163:
	ldr	r1, [r4, #76]
	ldr	r0, .L180+32
	bl	proc_mkdir
	str	r0, [r4, #80]
	cbz	r0, .L164
	cmn	r0, #4096
	bls	.L165
.L164:
	ldr	r2, .L180+32
	mov	r1, #432
	ldr	r0, .L180+24
	bl	printk
	movs	r3, #0
	str	r3, [r4, #80]
	b	.L155
.L165:
	str	r5, [sp]
	mov	r1, #292
	ldr	r2, [r4, #84]
	ldr	r3, .L180+36
	ldr	r0, .L180+40
	bl	proc_create_single_data
	ldr	r3, .L180+44
	mov	r1, #292
	ldr	r0, .L180+48
	str	r5, [sp]
	ldr	r2, [r4, #80]
	bl	proc_create_single_data
	b	.L155
.L166:
	mvn	r5, #11
	b	.L155
.L181:
	.align	2
.L180:
	.word	.LANCHOR0+12
	.word	.LC32
	.word	.LC34
	.word	.LC35
	.word	.LC36
	.word	.LC33
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	venc_proc_debug
	.word	.LC40
	.word	vdec_proc_debug
	.word	.LC41
	.word	.LANCHOR2
	.fnend
	.size	vcodec_probe, .-vcodec_probe
	.align	1
	.global	get_mpidev_ops
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_mpidev_ops, %function
get_mpidev_ops:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L184
	ldr	r2, [r4, #4]
	cbnz	r2, .L183
	mov	r1, #560
	ldr	r0, .L184+4
	bl	printk
.L183:
	ldr	r0, [r4, #4]
	pop	{r4, pc}
.L185:
	.align	2
.L184:
	.word	.LANCHOR0
	.word	.LC42
	.fnend
	.size	get_mpidev_ops, .-get_mpidev_ops
	.align	1
	.global	get_mpibuf_ops
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_mpibuf_ops, %function
get_mpibuf_ops:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	ldr	r4, .L188
	ldr	r2, [r4, #8]
	cbnz	r2, .L187
	movw	r1, #567
	ldr	r0, .L188+4
	bl	printk
.L187:
	ldr	r0, [r4, #8]
	pop	{r4, pc}
.L189:
	.align	2
.L188:
	.word	.LANCHOR0
	.word	.LC42
	.fnend
	.size	get_mpibuf_ops, .-get_mpibuf_ops
	.align	1
	.global	get_vsm_ops
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_vsm_ops, %function
get_vsm_ops:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L191
	ldr	r0, [r3]
	bx	lr
.L192:
	.align	2
.L191:
	.word	.LANCHOR0
	.fnend
	.size	get_vsm_ops, .-get_vsm_ops
	.global	vcodec_fops
	.section	.rodata
	.align	2
	.set	.LANCHOR2,. + 0
	.type	vcodec_fops, %object
	.size	vcodec_fops, 128
vcodec_fops:
	.space	36
	.word	vcodec_poll
	.word	vcodec_ioctl
	.space	12
	.word	vcodec_open
	.space	4
	.word	vcodec_close
	.space	60
	.type	vcodec_dt_ids, %object
	.size	vcodec_dt_ids, 392
vcodec_dt_ids:
	.space	64
	.ascii	"rockchip,vcodec\000"
	.space	112
	.space	4
	.space	196
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	vcodec_driver, %object
	.size	vcodec_driver, 104
vcodec_driver:
	.word	vcodec_probe
	.word	vcodec_remove
	.space	12
	.word	.LC33
	.space	20
	.word	vcodec_dt_ids
	.space	48
	.space	8
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	vsm_buf_ops, %object
	.size	vsm_buf_ops, 4
vsm_buf_ops:
	.space	4
	.type	mpidev_ops, %object
	.size	mpidev_ops, 4
mpidev_ops:
	.space	4
	.type	mpibuf_ops, %object
	.size	mpibuf_ops, 4
mpibuf_ops:
	.space	4
	.type	__key.32984, %object
	.size	__key.32984, 0
__key.32984:
	.section	.exitcall.exit,"aw"
	.align	2
	.type	__exitcall_vcodec_exit, %object
	.size	__exitcall_vcodec_exit, 4
__exitcall_vcodec_exit:
	.word	vcodec_exit
	.section	.initcall6.init,"aw"
	.align	2
	.type	__initcall__kmod_mpp_vcodec__209_620_vcodec_init6, %object
	.size	__initcall__kmod_mpp_vcodec__209_620_vcodec_init6, 4
__initcall__kmod_mpp_vcodec__209_620_vcodec_init6:
	.word	vcodec_init
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_license208, %object
	.size	__UNIQUE_ID_license208, 23
__UNIQUE_ID_license208:
	.ascii	"mpp_vcodec.license=GPL\000"
	.type	__UNIQUE_ID_file207, %object
	.size	__UNIQUE_ID_file207, 40
__UNIQUE_ID_file207:
	.ascii	"mpp_vcodec.file=drivers/kmpp/mpp_vcodec\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016mpp_vcodec: %d: vsm_buf_register_function ok\000"
.LC1:
	.ascii	"\0016mpp_vcodec: exit\012\000"
.LC2:
	.ascii	"remove device\012\000"
.LC3:
	.ascii	"\012--------venc thread status---------------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC4:
	.ascii	"run_cnt\000"
.LC5:
	.ascii	"last_runing\000"
.LC6:
	.ascii	"%15s%15s%15s\012\000"
.LC7:
	.ascii	"que_cnt\000"
.LC8:
	.ascii	"%15lld%15lld%15lld\012\000"
.LC9:
	.ascii	"\012--------venc chn runing status-----------------"
	.ascii	"---------------------------------------------------"
	.ascii	"\012\000"
.LC10:
	.ascii	"runing\000"
.LC11:
	.ascii	"ID\000"
.LC12:
	.ascii	"%8s%8s%10s%10s%10s%10s%10s%14s%15s%15s\012\000"
.LC13:
	.ascii	"last_cb_end\000"
.LC14:
	.ascii	"last_cb_start\000"
.LC15:
	.ascii	"cb_gap_time\000"
.LC16:
	.ascii	"gap_time\000"
.LC17:
	.ascii	"strm_out\000"
.LC18:
	.ascii	"strm_cnt\000"
.LC19:
	.ascii	"cfg_gap\000"
.LC20:
	.ascii	"combo_run\000"
.LC21:
	.ascii	"%8d%8u%10u%10u%10u%10u%10u%14u%15llu%15llu\012\000"
.LC22:
	.ascii	"\0016mpp_vcodec: init new\012\000"
.LC23:
	.ascii	"\0013mpp_service_driver device register failed (%d)"
	.ascii	".\012\000"
.LC24:
	.ascii	"\0013Platform device register failed (%d).\012\000"
.LC25:
	.ascii	"\0013mpp_vcodec: %d: ctx %p\012\000"
.LC26:
	.ascii	"\0013mpp_vcodec: %d: chl_id %d release request\000"
.LC27:
	.ascii	"\0013mpp_vcodec: %d: kernel vcodec_attr define is d"
	.ascii	"iff from user \012\000"
.LC28:
	.ascii	"\0013mpp_vcodec: %d: copy_to_user failed.\012\000"
.LC29:
	.ascii	"\0013mpp_vcodec: %d: unknown vcode req cmd %x\012\000"
.LC30:
	.ascii	"\0013mpp_vcodec: %d: unknown ioctl cmd %x\012\000"
.LC31:
	.ascii	"\0013mpp_vcodec: %d: chan id ctx is null\012\000"
.LC32:
	.ascii	"vcodec_class\000"
.LC33:
	.ascii	"vcodec\000"
.LC34:
	.ascii	"alloc dev_t failed\012\000"
.LC35:
	.ascii	"add device failed\012\000"
.LC36:
	.ascii	"%s\000"
.LC37:
	.ascii	"\0013mpp_vcodec: %d: failed on open procfs %s\012\000"
.LC38:
	.ascii	"enc\000"
.LC39:
	.ascii	"dec\000"
.LC40:
	.ascii	"venc_info\000"
.LC41:
	.ascii	"vdec_info\000"
.LC42:
	.ascii	"\0013mpp_vcodec: %d: should call vmpi_register_fn2v"
	.ascii	"cocdec \012\000"
	.section	___ksymtab+vmpi_register_fn2vcocdec,"a"
	.align	2
	.type	__ksymtab_vmpi_register_fn2vcocdec, %object
	.size	__ksymtab_vmpi_register_fn2vcocdec, 12
__ksymtab_vmpi_register_fn2vcocdec:
	.word	vmpi_register_fn2vcocdec
	.word	__kstrtab_vmpi_register_fn2vcocdec
	.word	__kstrtabns_vmpi_register_fn2vcocdec
	.section	___ksymtab+vmpi_unregister_fn2vcocdec,"a"
	.align	2
	.type	__ksymtab_vmpi_unregister_fn2vcocdec, %object
	.size	__ksymtab_vmpi_unregister_fn2vcocdec, 12
__ksymtab_vmpi_unregister_fn2vcocdec:
	.word	vmpi_unregister_fn2vcocdec
	.word	__kstrtab_vmpi_unregister_fn2vcocdec
	.word	__kstrtabns_vmpi_unregister_fn2vcocdec
	.section	___ksymtab+vsm_buf_register_fn2vcocdec,"a"
	.align	2
	.type	__ksymtab_vsm_buf_register_fn2vcocdec, %object
	.size	__ksymtab_vsm_buf_register_fn2vcocdec, 12
__ksymtab_vsm_buf_register_fn2vcocdec:
	.word	vsm_buf_register_fn2vcocdec
	.word	__kstrtab_vsm_buf_register_fn2vcocdec
	.word	__kstrtabns_vsm_buf_register_fn2vcocdec
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
