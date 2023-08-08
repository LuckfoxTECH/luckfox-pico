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
	.file	"mpp_service.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpp_service_driver:					
	.asciz 	"mpp_service_driver"					
__kstrtabns_mpp_service_driver:					
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
	.type	mpp_service_remove, %function
mpp_service_remove:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	adds	r0, r0, #16
	ldr	r4, [r0, #64]
	movs	r6, #0
	ldr	r1, .L24
	mov	r8, r6
	bl	_dev_info
	add	r7, r4, #464
.L2:
	ldr	r3, [r4, #460]
	cmp	r3, r6
	bhi	.L4
	add	r6, r4, #88
	add	r5, r4, #280
	add	r7, r4, #344
	mov	r8, #0
.L6:
	cbz	r4, .L5
	ldr	r3, [r5]
	cbz	r3, .L5
	mov	r0, r6
	bl	mpp_set_grf
	ldr	r0, [r5]
	bl	platform_driver_unregister
	str	r8, [r5]
.L5:
	adds	r5, r5, #4
	adds	r6, r6, #12
	cmp	r5, r7
	bne	.L6
	mov	r5, r4
	ldr	r1, [r4, #8]
	ldr	r0, [r5], #12
	bl	device_destroy
	mov	r0, r5
	bl	cdev_del
	movs	r1, #1
	ldr	r0, [r4, #8]
	bl	unregister_chrdev_region
	ldr	r0, [r4]
	bl	class_destroy
	ldr	r0, [r4, #76]
	cbz	r0, .L7
	bl	proc_remove
	movs	r3, #0
	str	r3, [r4, #76]
.L7:
	ldr	r3, .L24+4
	movs	r0, #0
	str	r0, [r3]
	pop	{r4, r5, r6, r7, r8, pc}
.L4:
	ldr	r5, [r7], #4
	cbz	r5, .L3
	ldr	r3, [r5, #28]
	cbz	r3, .L3
	mov	r0, r5
	bl	kthread_flush_worker
	ldr	r0, [r5, #28]
	bl	kthread_stop
	str	r8, [r5, #28]
.L3:
	adds	r6, r6, #1
	b	.L2
.L25:
	.align	2
.L24:
	.word	.LC0
	.word	g_srv
	.fnend
	.size	mpp_service_remove, .-mpp_service_remove
	.text
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_show_support_device, %function
mpp_show_support_device:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r5, r0
	ldr	r1, .L40
	movs	r4, #0
	ldr	r6, [r0, #64]
	bl	seq_puts
	ldr	r7, .L40+4
	ldr	r8, .L40+12
	ldr	r9, .L40+16
.L29:
	ldr	r3, [r6, #80]
	lsrs	r3, r3, r4
	lsls	r3, r3, #31
	bpl	.L27
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
	cmp	r4, #29
	sbc	r3, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r3, r3, r4
	adds	r3, r3, #86
	ldr	r10, [r6, r3, lsl #2]
	cmp	r10, #0
	beq	.L27
	ldr	r3, [r7, r4, lsl #2]
	mov	r2, r4
	mov	r1, r8
	mov	r0, r5
	bl	seq_printf
	ldr	r3, [r10, #4]
	ldr	r3, [r3, #4]
	ldr	r2, [r3, #8]
	cbz	r2, .L28
	ldr	r1, .L40+8
	mov	r0, r5
	bl	seq_printf
.L28:
	mov	r1, r9
	mov	r0, r5
	bl	seq_puts
.L27:
	adds	r4, r4, #1
	cmp	r4, #29
	bne	.L29
	movs	r0, #0
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L41:
	.align	2
.L40:
	.word	.LC1
	.word	mpp_device_name
	.word	.LC3
	.word	.LC2
	.word	.LC4
	.fnend
	.size	mpp_show_support_device, .-mpp_show_support_device
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_show_support_cmd, %function
mpp_show_support_cmd:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	ldr	r1, .L43
	bl	seq_puts
	mov	r0, r4
	movs	r2, #0
	ldr	r1, .L43+4
	bl	seq_printf
	ldr	r5, .L43+8
	mov	r0, r4
	movs	r2, #1
	ldr	r1, .L43+12
	bl	seq_printf
	mov	r0, r4
	movs	r2, #2
	ldr	r1, .L43+16
	bl	seq_printf
	movs	r2, #3
	mov	r0, r4
	ldr	r1, .L43+20
	bl	seq_printf
	mov	r1, r5
	mov	r0, r4
	bl	seq_puts
	mov	r0, r4
	mov	r2, #256
	ldr	r1, .L43+24
	bl	seq_printf
	mov	r0, r4
	mov	r2, #258
	ldr	r1, .L43+28
	bl	seq_printf
	movw	r2, #259
	mov	r0, r4
	ldr	r1, .L43+32
	bl	seq_printf
	mov	r1, r5
	mov	r0, r4
	bl	seq_puts
	mov	r0, r4
	mov	r2, #512
	ldr	r1, .L43+36
	bl	seq_printf
	mov	r0, r4
	movw	r2, #513
	ldr	r1, .L43+40
	bl	seq_printf
	mov	r0, r4
	movw	r2, #514
	ldr	r1, .L43+44
	bl	seq_printf
	movw	r2, #517
	mov	r0, r4
	ldr	r1, .L43+48
	bl	seq_printf
	mov	r1, r5
	mov	r0, r4
	bl	seq_puts
	mov	r0, r4
	mov	r2, #768
	ldr	r1, .L43+52
	bl	seq_printf
	movw	r2, #769
	mov	r0, r4
	ldr	r1, .L43+56
	bl	seq_printf
	mov	r1, r5
	mov	r0, r4
	bl	seq_puts
	mov	r0, r4
	mov	r2, #1024
	ldr	r1, .L43+60
	bl	seq_printf
	mov	r0, r4
	movw	r2, #1025
	ldr	r1, .L43+64
	bl	seq_printf
	mov	r0, r4
	movw	r2, #1026
	ldr	r1, .L43+68
	bl	seq_printf
	mov	r0, r4
	movw	r2, #1027
	ldr	r1, .L43+72
	bl	seq_printf
	mov	r0, r4
	movw	r2, #1028
	ldr	r1, .L43+76
	bl	seq_printf
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L44:
	.align	2
.L43:
	.word	.LC5
	.word	.LC6
	.word	.LC10
	.word	.LC7
	.word	.LC8
	.word	.LC9
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.fnend
	.size	mpp_show_support_cmd, .-mpp_show_support_cmd
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_show_version, %function
mpp_show_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r2, .L46
	ldr	r1, .L46+4
	bl	seq_printf
	movs	r0, #0
	pop	{r3, pc}
.L47:
	.align	2
.L46:
	.word	.LANCHOR0
	.word	.LC25
	.fnend
	.size	mpp_show_version, .-mpp_show_version
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_show_session_summary, %function
mpp_show_session_summary:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r0
	ldr	r4, [r0, #64]
	add	r6, r4, #700
	add	r4, r4, #712
	mov	r0, r6
	bl	mutex_lock
	ldr	r0, [r4]
	ldr	r5, [r0], #-180
	subs	r5, r5, #180
.L49:
	add	r3, r0, #180
	cmp	r3, r4
	bne	.L51
	mov	r0, r6
	bl	mutex_unlock
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L51:
	ldr	r3, [r0, #196]
	cbz	r3, .L50
	ldr	r3, [r0, #12]
	cbz	r3, .L50
	ldr	r3, [r3, #12]
	ldr	r3, [r3, #60]
	cbz	r3, .L50
	mov	r1, r7
	blx	r3
.L50:
	ldr	r3, [r5, #180]
	mov	r0, r5
	sub	r5, r3, #180
	b	.L49
	.fnend
	.size	mpp_show_session_summary, .-mpp_show_session_summary
	.section	.text.unlikely
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	of_property_read_u32, %function
of_property_read_u32:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	movs	r3, #0
	str	r3, [sp]
	movs	r3, #1
	bl	of_property_read_variable_u32_array
	and	r0, r0, r0, asr #31
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
	.fnend
	.size	of_property_read_u32, .-of_property_read_u32
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_add_driver, %function
mpp_add_driver:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	mov	r9, r3
	ldr	r3, [r0, #4]
	mov	r4, r0
	mov	r5, r1
	ldr	r1, .L67
	mov	r7, r2
	ldr	r6, [r3, #356]
	movs	r3, #0
	strd	r3, r3, [sp]
	mov	r0, r6
	bl	syscon_regmap_lookup_by_phandle
	mov	r8, r0
	cbz	r0, .L64
	cmn	r0, #4096
	bhi	.L64
	mov	r2, sp
	ldr	r1, .L67+4
	mov	r0, r6
	bl	of_property_read_u32
	cbnz	r0, .L64
	mov	r2, r9
	ldr	r1, .L67+8
	mov	r0, r6
	bl	of_property_match_string
	subs	r2, r0, #0
	blt	.L64
	add	r3, sp, #4
	ldr	r1, .L67+12
	mov	r0, r6
	bl	of_property_read_u32_index
	cbnz	r0, .L64
	movs	r0, #12
	ldr	r2, [sp]
	muls	r0, r5, r0
	adds	r3, r4, r0
	adds	r0, r0, #88
	add	r0, r0, r4
	str	r2, [r3, #88]
	ldr	r2, [sp, #4]
	str	r8, [r3, #96]
	str	r2, [r3, #92]
	bl	mpp_set_grf
.L64:
	movs	r1, #0
	mov	r0, r7
	bl	__platform_driver_register
	cbnz	r0, .L62
	adds	r5, r5, #70
	str	r7, [r4, r5, lsl #2]
.L62:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L68:
	.align	2
.L67:
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.fnend
	.size	mpp_add_driver, .-mpp_add_driver
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_service_probe, %function
mpp_service_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	add	r7, r0, #16
	.pad #28
	sub	sp, sp, #28
	ldr	r2, .L106
	ldr	r8, [r0, #372]
	mov	r5, r0
	ldr	r1, .L106+4
	mov	r0, r7
	movs	r3, #90
	str	r3, [sp, #20]
	bl	_dev_info
	ldr	r1, .L106+8
	mov	r0, r7
	bl	_dev_info
	mov	r2, #3520
	mov	r1, #724
	mov	r0, r7
	bl	devm_kmalloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L86
	str	r7, [r0, #4]
	movs	r0, #0
	str	r4, [r5, #80]
	str	r0, [r4, #84]
	ldr	r2, .L106+12
	ldr	r1, .L106+16
	bl	__class_create
	cmn	r0, #4096
	str	r0, [r4]
	bls	.L71
	cbz	r0, .L71
	mov	r5, r0
.L69:
	mov	r0, r5
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L71:
	add	r2, r4, #460
	ldr	r1, .L106+20
	mov	r0, r8
	bl	of_property_read_u32
	ldr	r2, [r4, #460]
	cmp	r2, #29
	bls	.L87
	ldr	r1, .L106+24
	movs	r3, #29
.L105:
	mov	r0, r7
	mvn	r5, #21
	bl	_dev_err
	b	.L69
.L75:
	mov	r0, r7
	bl	mpp_taskqueue_init
	mov	r6, r0
	cbz	r0, .L73
	mov	r2, r9
	mov	r1, r10
	bl	__kthread_init_worker
	mov	r3, fp
	mov	r2, #-1
	mov	r1, r6
	ldr	r0, .L106+28
	str	r5, [sp]
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r3, r0
	bhi	.L74
	str	r0, [sp, #12]
	bl	wake_up_process
	ldr	r3, [sp, #12]
.L74:
	str	r3, [r6, #28]
	mov	r0, r3
	add	r2, sp, #20
	movs	r1, #1
	bl	sched_setscheduler
	add	r3, r4, #464
	str	r6, [r3, r5, lsl #2]
.L73:
	adds	r5, r5, #1
.L72:
	ldr	r3, [r4, #460]
	cmp	r3, r5
	bhi	.L75
	add	r2, r4, #580
	ldr	r1, .L106+32
	mov	r0, r8
	bl	of_property_read_u32
	ldr	r2, [r4, #580]
	cmp	r2, #29
	bls	.L76
	movs	r3, #29
	ldr	r1, .L106+36
	b	.L105
.L87:
	movs	r5, #0
	ldr	r9, .L106+12
	ldr	r10, .L106+128
	ldr	fp, .L106+132
	b	.L72
.L76:
	cbz	r2, .L77
	ldr	r8, .L106+12
	movs	r5, #0
	ldr	r9, .L106+136
.L79:
	mov	r2, #3520
	movs	r1, #52
	mov	r0, r7
	bl	devm_kmalloc
	mov	r6, r0
	cbz	r0, .L78
	mov	r2, r8
	mov	r1, r9
	adds	r0, r0, #4
	bl	__init_rwsem
	add	r3, r4, #584
	str	r6, [r3, r5, lsl #2]
.L78:
	ldr	r3, [r4, #580]
	adds	r5, r5, #1
	cmp	r3, r5
	bhi	.L79
.L77:
	ldr	r8, .L106+44
	movs	r2, #1
	movs	r1, #0
	add	r0, r4, #8
	ldr	r9, [r4, #4]
	mov	r3, r8
	bl	alloc_chrdev_region
	mov	r5, r0
	cbz	r0, .L80
	ldr	r1, .L106+40
.L104:
	mov	r0, r9
	bl	_dev_err
	mov	r0, r7
	ldr	r2, .L106+44
	ldr	r1, .L106+48
	bl	_dev_err
	ldr	r0, [r4]
	bl	class_destroy
	b	.L69
.L80:
	ldr	r10, .L106+140
	add	r6, r4, #12
	mov	r0, r6
	mov	r1, r10
	bl	cdev_init
	strd	r5, r10, [r4, #48]
	movs	r2, #1
	ldr	r1, [r4, #8]
	mov	r0, r6
	bl	cdev_add
	mov	r5, r0
	cbz	r0, .L82
	movs	r1, #1
	ldr	r0, [r4, #8]
	bl	unregister_chrdev_region
	ldr	r1, .L106+52
	b	.L104
.L82:
	ldr	r3, .L106+56
	mov	r1, r9
	str	r8, [sp, #4]
	str	r3, [sp]
	mov	r3, r0
	ldr	r2, [r4, #8]
	ldr	r0, [r4]
	bl	device_create
	ldr	r2, .L106+12
	ldr	r1, .L106+60
	str	r0, [r4, #72]
	add	r0, r4, #700
	bl	__mutex_init
	add	r3, r4, #712
	mov	r1, r5
	str	r3, [r4, #712]
	str	r3, [r4, #716]
	mov	r0, r8
	bl	proc_mkdir
	mov	r2, r0
	str	r0, [r4, #76]
	cbz	r0, .L83
	cmn	r0, #4096
	bls	.L84
.L83:
	ldr	r3, .L106+44
	mov	r2, #256
	ldr	r1, .L106+64
	ldr	r0, .L106+68
	bl	printk
	movs	r3, #0
	str	r3, [r4, #76]
.L85:
	ldr	r3, .L106+72
	movs	r1, #13
	ldr	r2, .L106+76
	mov	r0, r4
	bl	mpp_add_driver
	ldr	r3, .L106+80
	ldr	r2, .L106+84
	movs	r1, #15
	mov	r0, r4
	bl	mpp_add_driver
	ldr	r1, .L106+88
	mov	r0, r7
	bl	_dev_info
	ldr	r3, .L106+92
	str	r4, [r3]
	b	.L69
.L84:
	str	r5, [sp]
	mov	r1, #292
	ldr	r3, .L106+96
	ldr	r0, .L106+100
	bl	proc_create_single_data
	ldr	r3, .L106+104
	mov	r1, #292
	ldr	r0, .L106+108
	str	r4, [sp]
	ldr	r2, [r4, #76]
	bl	proc_create_single_data
	ldr	r3, .L106+112
	mov	r1, #292
	ldr	r0, .L106+116
	str	r5, [sp]
	ldr	r2, [r4, #76]
	bl	proc_create_single_data
	ldr	r3, .L106+120
	mov	r1, #292
	ldr	r0, .L106+124
	str	r4, [sp]
	ldr	r2, [r4, #76]
	bl	proc_create_single_data
	b	.L85
.L86:
	mvn	r5, #11
	b	.L69
.L107:
	.align	2
.L106:
	.word	.LANCHOR0
	.word	.LC25
	.word	.LC30
	.word	.LANCHOR1
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	kthread_worker_fn
	.word	.LC36
	.word	.LC37
	.word	.LC40
	.word	.LC39
	.word	.LC52
	.word	.LC41
	.word	.LC42
	.word	.LC43
	.word	.LANCHOR0+82
	.word	.LC44
	.word	.LC49
	.word	rockchip_rkvenc540c_driver
	.word	.LC50
	.word	rockchip_vepu_pp_driver
	.word	.LC51
	.word	g_srv
	.word	mpp_show_version
	.word	.LC45
	.word	mpp_show_session_summary
	.word	.LC46
	.word	mpp_show_support_cmd
	.word	.LC47
	.word	mpp_show_support_device
	.word	.LC48
	.word	.LC34
	.word	.LC35
	.word	.LC38
	.word	rockchip_mpp_fops
	.fnend
	.size	mpp_service_probe, .-mpp_service_probe
	.global	mpp_service_driver
	.global	mpp_dev_debug
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	mpp_version, %object
	.size	mpp_version, 82
mpp_version:
	.ascii	"0cf529e0 author: Zhihua Wang 2022-12-30 [vcodec]: a"
	.ascii	"dd packet num module parameter\000"
	.type	__func__.37169, %object
	.size	__func__.37169, 16
__func__.37169:
	.ascii	"mpp_procfs_init\000"
	.space	2
	.type	mpp_dt_ids, %object
	.size	mpp_dt_ids, 392
mpp_dt_ids:
	.space	64
	.ascii	"rockchip,mpp-service\000"
	.space	107
	.space	4
	.space	196
	.type	__param_str_mpp_dev_debug, %object
	.size	__param_str_mpp_dev_debug, 24
__param_str_mpp_dev_debug:
	.ascii	"rk_vcodec.mpp_dev_debug\000"
	.data
	.align	2
	.type	mpp_service_driver, %object
	.size	mpp_service_driver, 104
mpp_service_driver:
	.word	mpp_service_probe
	.word	mpp_service_remove
	.space	12
	.word	.LC39
	.space	20
	.word	mpp_dt_ids
	.space	48
	.space	8
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__key.37183, %object
	.size	__key.37183, 0
__key.37183:
	.type	__key.37186, %object
	.size	__key.37186, 0
__key.37186:
	.type	__key.37195, %object
	.size	__key.37195, 0
__key.37195:
	.type	__key.37200, %object
	.size	__key.37200, 0
__key.37200:
	.type	mpp_dev_debug, %object
	.size	mpp_dev_debug, 4
mpp_dev_debug:
	.space	4
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_mpp_dev_debug227, %object
	.size	__UNIQUE_ID_mpp_dev_debug227, 66
__UNIQUE_ID_mpp_dev_debug227:
	.ascii	"rk_vcodec.parm=mpp_dev_debug:bit switch for mpp deb"
	.ascii	"ug information\000"
	.type	__UNIQUE_ID_mpp_dev_debugtype226, %object
	.size	__UNIQUE_ID_mpp_dev_debugtype226, 38
__UNIQUE_ID_mpp_dev_debugtype226:
	.ascii	"rk_vcodec.parmtype=mpp_dev_debug:uint\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"remove device\012\000"
.LC1:
	.ascii	"---- SUPPORT DEVICES ----\012\000"
.LC2:
	.ascii	"DEVICE[%2d]:%-10s\000"
.LC3:
	.ascii	"HW_ID:0x%08x\000"
.LC4:
	.ascii	"\012\000"
.LC5:
	.ascii	"------------- SUPPORT CMD -------------\012\000"
.LC6:
	.ascii	"QUERY_HW_SUPPORT:     0x%08x\012\000"
.LC7:
	.ascii	"QUERY_HW_ID:          0x%08x\012\000"
.LC8:
	.ascii	"QUERY_CMD_SUPPORT:    0x%08x\012\000"
.LC9:
	.ascii	"QUERY_BUTT:           0x%08x\012\000"
.LC10:
	.ascii	"----\012\000"
.LC11:
	.ascii	"INIT_CLIENT_TYPE:     0x%08x\012\000"
.LC12:
	.ascii	"INIT_TRANS_TABLE:     0x%08x\012\000"
.LC13:
	.ascii	"INIT_BUTT:            0x%08x\012\000"
.LC14:
	.ascii	"SET_REG_WRITE:        0x%08x\012\000"
.LC15:
	.ascii	"SET_REG_READ:         0x%08x\012\000"
.LC16:
	.ascii	"SET_REG_ADDR_OFFSET:  0x%08x\012\000"
.LC17:
	.ascii	"SEND_BUTT:            0x%08x\012\000"
.LC18:
	.ascii	"POLL_HW_FINISH:       0x%08x\012\000"
.LC19:
	.ascii	"POLL_BUTT:            0x%08x\012\000"
.LC20:
	.ascii	"RESET_SESSION:        0x%08x\012\000"
.LC21:
	.ascii	"TRANS_FD_TO_IOVA:     0x%08x\012\000"
.LC22:
	.ascii	"RELEASE_FD:           0x%08x\012\000"
.LC23:
	.ascii	"SEND_CODEC_INFO:      0x%08x\012\000"
.LC24:
	.ascii	"CONTROL_BUTT:         0x%08x\012\000"
.LC25:
	.ascii	"%s\012\000"
.LC26:
	.ascii	"rockchip,grf\000"
.LC27:
	.ascii	"rockchip,grf-offset\000"
.LC28:
	.ascii	"rockchip,grf-names\000"
.LC29:
	.ascii	"rockchip,grf-values\000"
.LC30:
	.ascii	"probe start\012\000"
.LC31:
	.ascii	"mpp_class\000"
.LC32:
	.ascii	"rockchip,taskqueue-count\000"
.LC33:
	.ascii	"rockchip,taskqueue-count %d must less than %d\012\000"
.LC34:
	.ascii	"(&queue->worker)->lock\000"
.LC35:
	.ascii	"queue_work%d\000"
.LC36:
	.ascii	"rockchip,resetgroup-count\000"
.LC37:
	.ascii	"rockchip,resetgroup-count %d must less than %d\012\000"
.LC38:
	.ascii	"&group->rw_sem\000"
.LC39:
	.ascii	"mpp_service\000"
.LC40:
	.ascii	"alloc dev_t failed\012\000"
.LC41:
	.ascii	"add device failed\012\000"
.LC42:
	.ascii	"%s\000"
.LC43:
	.ascii	"&srv->session_lock\000"
.LC44:
	.ascii	"\0013rk_vcodec: %s:%d: failed on mkdir /proc/%s\012"
	.ascii	"\000"
.LC45:
	.ascii	"version\000"
.LC46:
	.ascii	"sessions-summary\000"
.LC47:
	.ascii	"supports-cmd\000"
.LC48:
	.ascii	"supports-device\000"
.LC49:
	.ascii	"grf_rkvenc540c\000"
.LC50:
	.ascii	"grf_vepu_pp\000"
.LC51:
	.ascii	"probe success\012\000"
.LC52:
	.ascii	"register %s device\012\000"
	.section	___ksymtab+mpp_service_driver,"a"
	.align	2
	.type	__ksymtab_mpp_service_driver, %object
	.size	__ksymtab_mpp_service_driver, 12
__ksymtab_mpp_service_driver:
	.word	mpp_service_driver
	.word	__kstrtab_mpp_service_driver
	.word	__kstrtabns_mpp_service_driver
	.section	__param,"a"
	.align	2
	.type	__param_mpp_dev_debug, %object
	.size	__param_mpp_dev_debug, 20
__param_mpp_dev_debug:
	.word	__param_str_mpp_dev_debug
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	mpp_dev_debug
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
