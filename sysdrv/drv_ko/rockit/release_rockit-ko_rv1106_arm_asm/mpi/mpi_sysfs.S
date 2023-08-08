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
	.file	"mpi_sysfs.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_global_mutex:					
	.asciz 	"mpi_global_mutex"					
__kstrtabns_mpi_global_mutex:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_sysfs_dev_add:					
	.asciz 	"mpi_sysfs_dev_add"					
__kstrtabns_mpi_sysfs_dev_add:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_sysfs_dev_remove:					
	.asciz 	"mpi_sysfs_dev_remove"					
__kstrtabns_mpi_sysfs_dev_remove:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_sysfs_hotplug_event:					
	.asciz 	"mpi_sysfs_hotplug_event"					
__kstrtabns_mpi_sysfs_hotplug_event:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_fs_buf_init:					
	.asciz 	"mpi_fs_buf_init"					
__kstrtabns_mpi_fs_buf_init:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_fs_buf_deinit:					
	.asciz 	"mpi_fs_buf_deinit"					
__kstrtabns_mpi_fs_buf_deinit:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_fs_buf_reset:					
	.asciz 	"mpi_fs_buf_reset"					
__kstrtabns_mpi_fs_buf_reset:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_fs_buf_read:					
	.asciz 	"mpi_fs_buf_read"					
__kstrtabns_mpi_fs_buf_read:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_fs_buf_loop_add:					
	.asciz 	"mpi_fs_buf_loop_add"					
__kstrtabns_mpi_fs_buf_loop_add:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.global	mpi_fs_buf_reset
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_fs_buf_reset, %function
mpi_fs_buf_reset:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r3, #0
	str	r3, [r0]
	str	r3, [r0, #12]
	bx	lr
	.fnend
	.size	mpi_fs_buf_reset, .-mpi_fs_buf_reset
	.align	1
	.global	mpi_sysfs_hotplug_event
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_sysfs_hotplug_event, %function
mpi_sysfs_hotplug_event:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	movs	r3, #130
	ldr	r2, .L3
	movs	r1, #5
	str	r3, [sp]
	movs	r0, #0
	ldr	r3, .L3+4
	bl	vlog
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L4:
	.align	2
.L3:
	.word	.LC0
	.word	.LANCHOR0
	.fnend
	.size	mpi_sysfs_hotplug_event, .-mpi_sysfs_hotplug_event
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_devnode, %function
mpi_devnode:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #44]
	cbnz	r2, .L6
	ldr	r2, [r0]
.L6:
	ldr	r1, .L7
	mov	r0, #3264
	b	kasprintf
.L8:
	.align	2
.L7:
	.word	.LC1
	.fnend
	.size	mpi_devnode, .-mpi_devnode
	.align	1
	.global	mpi_fs_buf_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_fs_buf_init, %function
mpi_fs_buf_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r5, r1
	mov	r4, r0
	cbnz	r0, .L10
.L12:
	mov	r0, #-1
.L9:
	pop	{r3, r4, r5, pc}
.L10:
	cmp	r1, #0
	beq	.L12
	cmp	r1, #10240
	bgt	.L13
	ldr	r3, .L21
	str	r3, [r0, #16]
.L14:
	movs	r0, #0
	str	r5, [r4, #8]
	str	r0, [r4]
	str	r0, [r4, #12]
	b	.L9
.L13:
	mov	r1, #3264
	mov	r0, r5
	bl	__kmalloc
	str	r0, [r4, #16]
	cmp	r0, #0
	bne	.L14
	b	.L12
.L22:
	.align	2
.L21:
	.word	cat_buf
	.fnend
	.size	mpi_fs_buf_init, .-mpi_fs_buf_init
	.align	1
	.global	mpi_fs_buf_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_fs_buf_deinit, %function
mpi_fs_buf_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cbz	r0, .L23
	ldr	r0, [r0, #16]
	ldr	r3, .L28
	cmp	r0, r3
	beq	.L23
	b	kfree
.L23:
	bx	lr
.L29:
	.align	2
.L28:
	.word	cat_buf
	.fnend
	.size	mpi_fs_buf_deinit, .-mpi_fs_buf_deinit
	.align	1
	.global	mpi_fs_buf_loop_add
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_fs_buf_loop_add, %function
mpi_fs_buf_loop_add:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r3, r2
	ldr	r5, [r0, #8]
	mov	r4, r0
	ldr	r2, [r0, #4]
	subs	r5, r5, #1
	cmp	r2, r5
	itt	ge
	movge	r2, #0
	strge	r2, [r0, #4]
	ldr	r0, [r0, #4]
	mov	r2, r1
	ldr	r6, [r4, #16]
	subs	r1, r5, r0
	add	r0, r0, r6
	bl	vsnprintf
	ldr	r3, [r4, #4]
	ldr	r2, [r4, #8]
	add	r3, r3, r0
	subs	r0, r2, #1
	cmp	r0, r3
	it	ge
	movge	r0, r3
	ldr	r3, [r4, #12]
	str	r0, [r4, #4]
	cmp	r3, r0
	it	lt
	strlt	r0, [r4, #12]
	ldrd	r3, r2, [r4, #12]
	movs	r0, #0
	strb	r0, [r2, r3]
	pop	{r4, r5, r6, pc}
	.fnend
	.size	mpi_fs_buf_loop_add, .-mpi_fs_buf_loop_add
	.align	1
	.global	mpi_sysfs_dev_add
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_sysfs_dev_add, %function
mpi_sysfs_dev_add:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r0
	ldr	r5, [r0, #96]
	.pad #20
	sub	sp, sp, #20
	cmp	r5, #0
	bne	.L37
	add	r7, r0, #100
	ldr	r6, .L38
	mov	r0, r7
	bl	cdev_init
	ldr	r1, [r6]
	ldr	r3, [r4]
	movs	r2, #1
	mov	r0, r7
	orr	r1, r3, r1, lsl #20
	bl	cdev_add
	ldr	r3, [r4, #44]
	ldr	r1, [r6]
	ldr	r0, .L38+4
	ldr	r2, [r4]
	orr	r2, r2, r1, lsl #20
	mov	r1, r5
	str	r3, [sp, #8]
	ldr	r3, .L38+8
	strd	r5, r3, [sp]
	mov	r3, r4
	ldr	r0, [r0]
	bl	device_create_with_groups
	ldr	r3, [r4, #44]
	ldr	r2, .L38+12
	movs	r1, #5
	str	r0, [r4, #96]
	mov	r0, r5
	str	r3, [sp, #4]
	movs	r3, #82
	str	r3, [sp]
	ldr	r3, .L38+16
	bl	vlog
	ldr	r3, [r4, #96]
	cmn	r3, #4096
	bls	.L36
	str	r3, [sp, #4]
	movs	r1, #2
	movs	r3, #85
	ldr	r2, .L38+20
	str	r3, [sp]
	mov	r0, r5
	ldr	r3, .L38+16
	bl	vlog
	mov	r0, #-1
.L34:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L36:
	ldr	r0, [r6]
	b	.L34
.L37:
	movs	r0, #0
	b	.L34
.L39:
	.align	2
.L38:
	.word	.LANCHOR1
	.word	.LANCHOR2
	.word	.LC2
	.word	.LC3
	.word	.LANCHOR0+24
	.word	.LC4
	.fnend
	.size	mpi_sysfs_dev_add, .-mpi_sysfs_dev_add
	.align	1
	.global	mpi_sysfs_dev_remove
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_sysfs_dev_remove, %function
mpi_sysfs_dev_remove:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r0
	ldr	r3, [r0, #96]
	cbz	r3, .L40
	ldr	r3, [r0, #44]
	movs	r1, #5
	ldr	r2, .L45
	movs	r0, #0
	str	r3, [sp, #4]
	movs	r3, #100
	str	r3, [sp]
	ldr	r3, .L45+4
	bl	vlog
	ldr	r0, [r4, #96]
	bl	device_unregister
	movs	r3, #0
	str	r3, [r4, #96]
.L40:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L46:
	.align	2
.L45:
	.word	.LC5
	.word	.LANCHOR0+42
	.fnend
	.size	mpi_sysfs_dev_remove, .-mpi_sysfs_dev_remove
	.align	1
	.global	mpi_fs_buf_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_fs_buf_read, %function
mpi_fs_buf_read:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, [r5, #12]
	mov	r0, r1
	cbnz	r4, .L48
.L52:
	mov	r4, #-1
.L47:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L48:
	ldr	r1, [r5]
	cmp	r4, r1
	ble	.L50
	subs	r4, r4, r1
	cmp	r4, r2
	it	ge
	movge	r4, r2
	cmp	r4, #0
	mov	r2, r4
	blt	.L52
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r3, [r3, #8]
	.syntax unified
@ 177 "./include/linux/uaccess.h" 1
	.syntax unified
adds r6, r0, r4; sbcscc r6, r6, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L51
	ldr	r3, [r5, #16]
	add	r1, r1, r3
	bl	arm_copy_to_user
	mov	r2, r0
.L51:
	cmp	r2, #0
	bne	.L52
	ldr	r3, [r5]
	add	r3, r3, r4
	str	r3, [r5]
	b	.L47
.L50:
	movs	r4, #0
	str	r4, [r5]
	b	.L47
	.fnend
	.size	mpi_fs_buf_read, .-mpi_fs_buf_read
	.align	1
	.global	mpi_sysfs_lease_event
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_sysfs_lease_event, %function
mpi_sysfs_lease_event:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	movs	r3, #112
	ldr	r2, .L54
	movs	r1, #5
	str	r3, [sp]
	movs	r0, #0
	ldr	r3, .L54+4
	bl	vlog
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L55:
	.align	2
.L54:
	.word	.LC6
	.word	.LANCHOR0+63
	.fnend
	.size	mpi_sysfs_lease_event, .-mpi_sysfs_lease_event
	.align	1
	.global	mpi_fs_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_fs_init, %function
mpi_fs_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r3, .L60
	movs	r2, #50
	movs	r1, #0
	add	r0, sp, #12
	bl	alloc_chrdev_region
	cmp	r0, #0
	bge	.L57
	movs	r3, #142
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L60+4
	movs	r0, #0
	ldr	r2, .L60+8
	bl	vlog
.L58:
	movs	r0, #0
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L57:
	ldr	r3, [sp, #12]
	movs	r0, #0
	ldr	r4, .L60+12
	ldr	r5, .L60+16
	adds	r2, r4, #4
	ldr	r1, .L60
	lsrs	r3, r3, #20
	str	r3, [r5]
	bl	__class_create
	cmn	r0, #4096
	str	r0, [r4]
	bhi	.L58
	movs	r2, #0
	adds	r1, r5, #4
	bl	class_create_file_ns
	cbz	r0, .L59
	ldr	r0, [r4]
	bl	class_destroy
	movs	r3, #0
	str	r3, [r4]
	b	.L58
.L59:
	ldr	r3, [r4]
	ldr	r2, .L60+20
	str	r2, [r3, #24]
	b	.L58
.L61:
	.align	2
.L60:
	.word	.LC7
	.word	.LANCHOR0+85
	.word	.LC8
	.word	.LANCHOR2
	.word	.LANCHOR1
	.word	mpi_devnode
	.fnend
	.size	mpi_fs_init, .-mpi_fs_init
	.align	1
	.global	mpi_fs_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_fs_exit, %function
mpi_fs_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	movs	r1, #50
	ldr	r5, .L67
	ldr	r4, .L67+4
	ldr	r0, [r5]
	lsls	r0, r0, #20
	bl	unregister_chrdev_region
	ldr	r0, [r4]
	cbz	r0, .L62
	cmn	r0, #4096
	bhi	.L62
	movs	r2, #0
	adds	r1, r5, #4
	bl	class_remove_file_ns
	ldr	r0, [r4]
	bl	class_destroy
	movs	r3, #0
	str	r3, [r4]
.L62:
	pop	{r3, r4, r5, pc}
.L68:
	.align	2
.L67:
	.word	.LANCHOR1
	.word	.LANCHOR2
	.fnend
	.size	mpi_fs_exit, .-mpi_fs_exit
	.align	1
	.global	mpi_fs_buf_add
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_fs_buf_add, %function
mpi_fs_buf_add:
	.fnstart
	@ args = 4, pretend = 12, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 1
	push	{r1, r2, r3}
	.save {r1, r2, r3}
	push	{r0, r1, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #8
	mov	r4, r0
	add	r3, sp, #20
	ldrd	r1, r0, [r0, #8]
	ldr	r2, [r3], #4
	subs	r1, r1, #1
	cmp	r0, r1
	str	r3, [sp, #4]
	blt	.L70
	ldr	r0, .L72
	bl	printk
	mov	r0, #-1
.L69:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, lr}
	add	sp, sp, #12
	bx	lr
.L70:
	ldr	r5, [r4, #16]
	subs	r1, r1, r0
	add	r0, r0, r5
	bl	vsnprintf
	ldr	r3, [r4, #12]
	ldr	r2, [r4, #16]
	add	r3, r3, r0
	movs	r0, #0
	str	r3, [r4, #12]
	strb	r0, [r2, r3]
	b	.L69
.L73:
	.align	2
.L72:
	.word	.LC9
	.fnend
	.size	mpi_fs_buf_add, .-mpi_fs_buf_add
	.global	cat_buf
	.global	mpi_global_mutex
	.global	mpi_class
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.36237, %object
	.size	__func__.36237, 24
__func__.36237:
	.ascii	"mpi_sysfs_hotplug_event\000"
	.type	__func__.36214, %object
	.size	__func__.36214, 18
__func__.36214:
	.ascii	"mpi_sysfs_dev_add\000"
	.type	__func__.36224, %object
	.size	__func__.36224, 21
__func__.36224:
	.ascii	"mpi_sysfs_dev_remove\000"
	.type	__func__.36233, %object
	.size	__func__.36233, 22
__func__.36233:
	.ascii	"mpi_sysfs_lease_event\000"
	.type	__func__.36248, %object
	.size	__func__.36248, 12
__func__.36248:
	.ascii	"mpi_fs_init\000"
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	major_id, %object
	.size	major_id, 4
major_id:
	.word	255
	.type	class_attr_version, %object
	.size	class_attr_version, 20
class_attr_version:
	.word	.LC10
	.short	292
	.space	2
	.word	show_class_attr_string
	.word	0
	.word	.LC11
	.type	mpi_global_mutex, %object
	.size	mpi_global_mutex, 12
mpi_global_mutex:
	.word	0
	.word	mpi_global_mutex+4
	.word	mpi_global_mutex+4
	.bss
	.align	2
	.set	.LANCHOR2,. + 0
	.type	mpi_class, %object
	.size	mpi_class, 4
mpi_class:
	.space	4
	.type	__key.36205, %object
	.size	__key.36205, 0
__key.36205:
	.type	cat_buf, %object
	.size	cat_buf, 10240
cat_buf:
	.space	10240
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"%s,%d:generating hotplug event\000"
.LC1:
	.ascii	"mpi/%s\000"
.LC2:
	.ascii	"%s\000"
.LC3:
	.ascii	"%s,%d:adding \"%s\" to sysfs\000"
.LC4:
	.ascii	"\033[31m\033[1m%s,%d:failed to register dev device:"
	.ascii	" %ld\033[0m\000"
.LC5:
	.ascii	"%s,%d:removing \"%s\" from sysfs\000"
.LC6:
	.ascii	"%s,%d:generating lease event\000"
.LC7:
	.ascii	"mpi\000"
.LC8:
	.ascii	"\033[31m\033[1m%s,%d:mpi get major device id fail ="
	.ascii	" %d\033[0m\000"
.LC9:
	.ascii	"memory too small\000"
.LC10:
	.ascii	"version\000"
.LC11:
	.ascii	"mpi 1.1.0 20060810\000"
	.section	___ksymtab_gpl+mpi_fs_buf_deinit,"a"
	.align	2
	.type	__ksymtab_mpi_fs_buf_deinit, %object
	.size	__ksymtab_mpi_fs_buf_deinit, 12
__ksymtab_mpi_fs_buf_deinit:
	.word	mpi_fs_buf_deinit
	.word	__kstrtab_mpi_fs_buf_deinit
	.word	__kstrtabns_mpi_fs_buf_deinit
	.section	___ksymtab_gpl+mpi_fs_buf_init,"a"
	.align	2
	.type	__ksymtab_mpi_fs_buf_init, %object
	.size	__ksymtab_mpi_fs_buf_init, 12
__ksymtab_mpi_fs_buf_init:
	.word	mpi_fs_buf_init
	.word	__kstrtab_mpi_fs_buf_init
	.word	__kstrtabns_mpi_fs_buf_init
	.section	___ksymtab_gpl+mpi_fs_buf_loop_add,"a"
	.align	2
	.type	__ksymtab_mpi_fs_buf_loop_add, %object
	.size	__ksymtab_mpi_fs_buf_loop_add, 12
__ksymtab_mpi_fs_buf_loop_add:
	.word	mpi_fs_buf_loop_add
	.word	__kstrtab_mpi_fs_buf_loop_add
	.word	__kstrtabns_mpi_fs_buf_loop_add
	.section	___ksymtab_gpl+mpi_fs_buf_read,"a"
	.align	2
	.type	__ksymtab_mpi_fs_buf_read, %object
	.size	__ksymtab_mpi_fs_buf_read, 12
__ksymtab_mpi_fs_buf_read:
	.word	mpi_fs_buf_read
	.word	__kstrtab_mpi_fs_buf_read
	.word	__kstrtabns_mpi_fs_buf_read
	.section	___ksymtab_gpl+mpi_fs_buf_reset,"a"
	.align	2
	.type	__ksymtab_mpi_fs_buf_reset, %object
	.size	__ksymtab_mpi_fs_buf_reset, 12
__ksymtab_mpi_fs_buf_reset:
	.word	mpi_fs_buf_reset
	.word	__kstrtab_mpi_fs_buf_reset
	.word	__kstrtabns_mpi_fs_buf_reset
	.section	___ksymtab_gpl+mpi_global_mutex,"a"
	.align	2
	.type	__ksymtab_mpi_global_mutex, %object
	.size	__ksymtab_mpi_global_mutex, 12
__ksymtab_mpi_global_mutex:
	.word	mpi_global_mutex
	.word	__kstrtab_mpi_global_mutex
	.word	__kstrtabns_mpi_global_mutex
	.section	___ksymtab_gpl+mpi_sysfs_dev_add,"a"
	.align	2
	.type	__ksymtab_mpi_sysfs_dev_add, %object
	.size	__ksymtab_mpi_sysfs_dev_add, 12
__ksymtab_mpi_sysfs_dev_add:
	.word	mpi_sysfs_dev_add
	.word	__kstrtab_mpi_sysfs_dev_add
	.word	__kstrtabns_mpi_sysfs_dev_add
	.section	___ksymtab_gpl+mpi_sysfs_dev_remove,"a"
	.align	2
	.type	__ksymtab_mpi_sysfs_dev_remove, %object
	.size	__ksymtab_mpi_sysfs_dev_remove, 12
__ksymtab_mpi_sysfs_dev_remove:
	.word	mpi_sysfs_dev_remove
	.word	__kstrtab_mpi_sysfs_dev_remove
	.word	__kstrtabns_mpi_sysfs_dev_remove
	.section	___ksymtab_gpl+mpi_sysfs_hotplug_event,"a"
	.align	2
	.type	__ksymtab_mpi_sysfs_hotplug_event, %object
	.size	__ksymtab_mpi_sysfs_hotplug_event, 12
__ksymtab_mpi_sysfs_hotplug_event:
	.word	mpi_sysfs_hotplug_event
	.word	__kstrtab_mpi_sysfs_hotplug_event
	.word	__kstrtabns_mpi_sysfs_hotplug_event
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
