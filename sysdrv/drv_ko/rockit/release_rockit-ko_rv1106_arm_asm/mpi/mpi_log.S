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
	.file	"mpi_log.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_vlog:					
	.asciz 	"vlog"					
__kstrtabns_vlog:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog_compat_ioctl, %function
vlog_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vlog_compat_ioctl, .-vlog_compat_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog_thread_fn, %function
vlog_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	add	r5, r0, #188
	mov	r4, r0
.L3:
	ldr	r3, [r4, #204]
	cbz	r3, .L4
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L4:
	mov	r0, r5
	bl	down
	b	.L3
	.fnend
	.size	vlog_thread_fn, .-vlog_thread_fn
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog_read, %function
vlog_read:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L7
	bl	mpi_fs_buf_read
	cmp	r0, #-1
	it	eq
	moveq	r0, #0
	pop	{r3, pc}
.L8:
	.align	2
.L7:
	.word	.LANCHOR0+168
	.fnend
	.size	vlog_read, .-vlog_read
	.global	__aeabi_idivmod
	.align	1
	.global	format_time_stamp
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	format_time_stamp, %function
format_time_stamp:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r6, r0
	ldr	r4, .L14
	add	r8, r6, #-1
	ldr	r10, .L14+8
	mov	r0, r1
	mov	r7, r2
	mov	r9, r8
	movs	r5, #0
.L10:
	mov	r1, r4
	adds	r5, r5, #1
	bl	__aeabi_idivmod
	smull	r2, r3, r4, r10
	cmp	r5, #9
	asr	r4, r4, #31
	rsb	r4, r4, r3, asr #2
	add	r0, r0, #48
	strb	r0, [r9, #1]!
	mov	r0, r1
	bne	.L10
	ldr	r4, .L14+4
	movs	r5, #10
	ldr	r9, .L14+8
	movs	r3, #46
	strb	r3, [r6, #9]
.L11:
	mov	r1, r4
	mov	r0, r7
	bl	__aeabi_idivmod
	smull	r2, r3, r4, r9
	adds	r5, r5, #1
	cmp	r5, #16
	asr	r4, r4, #31
	rsb	r4, r4, r3, asr #2
	add	r0, r0, #48
	mov	r7, r1
	strb	r0, [r8, r5]
	bne	.L11
	movs	r3, #0
	strb	r3, [r6, #16]
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L15:
	.align	2
.L14:
	.word	100000000
	.word	100000
	.word	1717986919
	.fnend
	.size	format_time_stamp, .-format_time_stamp
	.align	1
	.global	vlog
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog, %function
vlog:
	.fnstart
	@ args = 4, pretend = 8, frame = 96
	@ frame_needed = 0, uses_anonymous_args = 1
	push	{r2, r3}
	.save {r2, r3}
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #100
	sub	sp, sp, #100
	ldr	r6, .L19
	mov	r4, r0
	ldr	r5, .L19+4
	mov	r10, r1
	ldr	r9, [sp, #136]
	ldr	r3, [r6]
	str	r3, [sp, #92]
	ldr	r3, [r5, r0, lsl #2]
	cmp	r3, r1
	blt	.L17
	.syntax unified
@ 29 "./arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r0, sp, #8
	add	r4, r5, r4, lsl #2
	bl	ktime_get_ts64
	mov	fp, #91
	ldr	r4, [r4, #92]
	movs	r3, #13
	add	r5, r5, r10, lsl #2
	movs	r7, #93
	mov	r0, r4
	strb	r3, [sp, #28]
	movs	r3, #10
	strb	fp, [sp, #30]
	strb	r3, [sp, #29]
	bl	strlen
	mov	r1, r4
	mov	r2, r0
	add	r0, sp, #31
	bl	memcpy
	mov	r0, r4
	bl	strlen
	ldr	r5, [r5, #184]
	add	r3, sp, #96
	adds	r4, r0, #5
	add	r3, r3, r0
	mov	r0, r5
	strb	r7, [r3, #-65]
	strb	fp, [r3, #-64]
	bl	strlen
	mov	r1, r5
	add	r3, sp, #28
	mov	r2, r0
	adds	r0, r3, r4
	bl	memcpy
	mov	r0, r5
	bl	strlen
	add	r0, r0, r4
	ldr	r2, [sp, #16]
	ldr	r4, .L19+8
	add	r3, sp, #96
	add	r10, r3, r0
	add	r3, sp, #28
	adds	r0, r0, #2
	ldr	r1, [sp, #8]
	smull	r4, r5, r2, r4
	asrs	r2, r2, #31
	add	r0, r0, r3
	strb	r7, [r10, #-68]
	strb	fp, [r10, #-67]
	rsb	r2, r2, r5, asr #6
	bl	format_time_stamp
	ldr	r0, .L19+12
	movs	r3, #0
	strb	r7, [r10, #-50]
	strb	r3, [r10, #-49]
	ldr	r2, [sp, #4]
	add	r1, sp, #28
	bl	mpi_fs_buf_loop_add
	mov	r1, r9
	ldr	r0, .L19+12
	add	r2, sp, #140
	str	r2, [sp, #4]
	bl	mpi_fs_buf_loop_add
	.syntax unified
@ 171 "./arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L17:
	ldr	r2, [sp, #92]
	movs	r0, #0
	ldr	r3, [r6]
	cmp	r2, r3
	beq	.L18
	bl	__stack_chk_fail
.L18:
	add	sp, sp, #100
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	add	sp, sp, #8
	bx	lr
.L20:
	.align	2
.L19:
	.word	__stack_chk_guard
	.word	.LANCHOR1
	.word	274877907
	.word	.LANCHOR0+168
	.fnend
	.size	vlog, .-vlog
	.align	1
	.global	vlog_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog_version, %function
vlog_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r3, [r3]
	ldr	r5, [r3, #160]
	ldr	r3, [r1, #32]
	cmp	r3, #76
	beq	.L22
	movs	r3, #115
	ldr	r2, .L26
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L26+4
	movs	r0, #0
	bl	vlog
	mov	r0, #-1
.L21:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L22:
	movs	r3, #0
	movs	r2, #1
	str	r3, [r1, #8]
	strd	r2, r3, [r1]
	ldr	r0, [r5]
	bl	strlen
	cmp	r0, #16
	it	ls
	strls	r0, [r4, #12]
	add	r0, r4, #16
	itt	hi
	movhi	r3, #16
	strhi	r3, [r4, #12]
	ldr	r2, [r4, #12]
	ldr	r1, [r5]
	bl	strncpy
	movs	r0, #0
	str	r0, [r4, #36]
	str	r0, [r4, #56]
	b	.L21
.L27:
	.align	2
.L26:
	.word	.LC0
	.word	.LANCHOR2
	.fnend
	.size	vlog_version, .-vlog_version
	.align	1
	.global	vlog_set_level
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog_set_level, %function
vlog_set_level:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, lr}
	.save {r4, lr}
	.pad #16
	mov	r4, r1
	ldr	r3, [r1, #8]
	movs	r0, #0
	ldr	r2, .L36
	str	r3, [sp, #8]
	ldr	r3, [r1]
	movs	r1, #4
	str	r3, [sp, #4]
	movs	r3, #140
	str	r3, [sp]
	ldr	r3, .L36+4
	bl	vlog
	ldr	r3, [r4, #4]
	cmp	r3, #12
	beq	.L29
	movs	r3, #142
	ldr	r2, .L36+8
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L36+4
	movs	r0, #0
	bl	vlog
.L33:
	mov	r0, #-1
	b	.L28
.L29:
	ldr	r2, [r4]
	cmp	r2, #23
	bhi	.L33
	ldr	r3, .L36+12
	bne	.L31
	movs	r2, #0
.L32:
	ldr	r1, [r4, #8]
	adds	r2, r2, #1
	cmp	r2, #23
	str	r1, [r3], #4
	bne	.L32
.L35:
	movs	r0, #0
.L28:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L31:
	ldr	r1, [r4, #8]
	str	r1, [r3, r2, lsl #2]
	b	.L35
.L37:
	.align	2
.L36:
	.word	.LC1
	.word	.LANCHOR2+13
	.word	.LC2
	.word	.LANCHOR1
	.fnend
	.size	vlog_set_level, .-vlog_set_level
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog_release, %function
vlog_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	ldr	r4, [r1, #128]
	ldr	r0, [r4]
	bl	mpi_obj_unref
	ldr	r3, [r4]
	ldr	r2, .L39
	movs	r1, #5
	movs	r0, #0
	ldr	r3, [r3]
	str	r3, [sp, #4]
	movs	r3, #223
	str	r3, [sp]
	ldr	r3, .L39+4
	bl	vlog
	mov	r0, r4
	bl	kfree
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L40:
	.align	2
.L39:
	.word	.LC3
	.word	.LANCHOR2+28
	.fnend
	.size	vlog_release, .-vlog_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog_open, %function
vlog_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L44
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r7, r1
	ldr	r0, [r3, #12]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r5, r0
	cbnz	r0, .L42
	movs	r3, #210
	ldr	r2, .L44+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L44+8
	bl	vlog
	mov	r0, #-1
.L41:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L42:
	ldr	r4, .L44+12
	adds	r6, r4, #4
	mov	r0, r6
	bl	mpi_obj_ref
	ldr	r3, [r4, #4]
	str	r6, [r5]
	movs	r1, #5
	str	r5, [r7, #128]
	movs	r0, #0
	ldr	r2, .L44+16
	str	r3, [sp, #4]
	movs	r3, #215
	str	r3, [sp]
	ldr	r3, .L44+8
	bl	vlog
	movs	r0, #0
	b	.L41
.L45:
	.align	2
.L44:
	.word	kmalloc_caches
	.word	.LC4
	.word	.LANCHOR2+41
	.word	.LANCHOR0
	.word	.LC3
	.fnend
	.size	vlog_open, .-vlog_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog_ioctl, %function
vlog_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	uxtb	r4, r1
	ldr	r3, .L50
	mov	r6, r0
	mov	r5, r1
	mov	r7, r2
	adds	r3, r3, #4
	ldr	r2, .L50+4
	ldr	r8, [r3, #160]
	movs	r1, #5
	strd	r4, r3, [sp, #8]
	movs	r0, #0
	movs	r3, #176
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r3, .L50+8
	bl	vlog
	ldr	r0, [r8, #212]
	cmp	r0, r4
	bhi	.L47
	movs	r3, #179
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L50+8
	movs	r0, #0
	ldr	r2, .L50+12
	bl	vlog
.L48:
.L46:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L47:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
	cmp	r4, r0
	sbc	r0, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r4, r4, r0
	ldr	r0, [r8, #208]
	movs	r3, #20
	mla	r0, r3, r4, r0
	cbnz	r0, .L49
	movs	r3, #186
	ldr	r2, .L50+16
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L50+8
	bl	vlog
	b	.L46
.L49:
	mov	r1, r0
	mov	r3, r7
	mov	r2, r5
	mov	r0, r6
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L51:
	.align	2
.L50:
	.word	.LANCHOR0
	.word	.LC5
	.word	.LANCHOR2+51
	.word	.LC6
	.word	.LC7
	.fnend
	.size	vlog_ioctl, .-vlog_ioctl
	.align	1
	.global	vlog_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog_probe, %function
vlog_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movs	r2, #216
	ldr	r4, .L54
	mov	r5, r0
	movs	r1, #0
	ldr	r6, .L54+4
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r5
	bl	kstrdup
	mov	r1, #51200
	movs	r3, #2
	str	r4, [r4, #164]
	str	r3, [r4, #212]
	str	r6, [r4, #208]
	str	r0, [r4]
	add	r0, r4, #168
	bl	mpi_fs_buf_init
	mov	r2, r5
	movs	r1, #8
	adds	r0, r4, #4
	bl	mpi_dev_register
	add	r0, r4, #192
	add	r1, r6, #40
	strd	r0, r0, [r4, #192]
	adds	r0, r4, #4
	movs	r3, #0
	str	r3, [r4, #72]
	strd	r3, r3, [r4, #80]
	str	r3, [r4, #188]
	bl	mpi_sysfs_dev_add
	mov	r3, r5
	mov	r2, #-1
	mov	r1, r4
	ldr	r0, .L54+8
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L53
	bl	wake_up_process
.L53:
	str	r5, [r4, #200]
	pop	{r4, r5, r6, pc}
.L55:
	.align	2
.L54:
	.word	.LANCHOR0
	.word	.LANCHOR2+64
	.word	vlog_thread_fn
	.fnend
	.size	vlog_probe, .-vlog_probe
	.global	g_vlog_dev
	.global	current_log_level
	.global	level_name
	.global	module_name
	.section	.rodata
	.align	2
	.set	.LANCHOR2,. + 0
	.type	__func__.36344, %object
	.size	__func__.36344, 13
__func__.36344:
	.ascii	"vlog_version\000"
	.type	__func__.36351, %object
	.size	__func__.36351, 15
__func__.36351:
	.ascii	"vlog_set_level\000"
	.type	__func__.36396, %object
	.size	__func__.36396, 13
__func__.36396:
	.ascii	"vlog_release\000"
	.type	__func__.36389, %object
	.size	__func__.36389, 10
__func__.36389:
	.ascii	"vlog_open\000"
	.type	__func__.36367, %object
	.size	__func__.36367, 11
__func__.36367:
	.ascii	"vlog_ioctl\000"
	.space	2
	.type	vlog_ioctls, %object
	.size	vlog_ioctls, 40
vlog_ioctls:
	.word	-1068733440
	.word	0
	.word	vlog_version
	.word	.LC8
	.word	0
	.word	-1072927743
	.word	0
	.word	vlog_set_level
	.word	.LC9
	.word	0
	.type	vlog_driver_fops, %object
	.size	vlog_driver_fops, 128
vlog_driver_fops:
	.word	0
	.space	4
	.word	vlog_read
	.space	24
	.word	0
	.word	vlog_ioctl
	.word	vlog_compat_ioctl
	.word	0
	.space	4
	.word	vlog_open
	.space	4
	.word	vlog_release
	.space	60
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	current_log_level, %object
	.size	current_log_level, 92
current_log_level:
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.word	4
	.space	8
	.type	module_name, %object
	.size	module_name, 92
module_name:
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
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.word	.LC35
	.space	8
	.type	level_name, %object
	.size	level_name, 24
level_name:
	.word	.LC10
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_vlog_dev, %object
	.size	g_vlog_dev, 216
g_vlog_dev:
	.space	216
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[31m\033[1m%s,%d:struct vlog_version mismatchin"
	.ascii	"g\033[0m\000"
.LC1:
	.ascii	"\033[32m\033[1m%s,%d:mod = %d, level = %d\033[0m\000"
.LC2:
	.ascii	"\033[31m\033[1m%s,%d:struct vlog_level mismatching\033"
	.ascii	"[0m\000"
.LC3:
	.ascii	"%s,%d:%d\000"
.LC4:
	.ascii	"\033[31m\033[1m%s,%d:valloc open fail\033[0m\000"
.LC5:
	.ascii	"%s,%d:vlog_ioctl: nr = %d, index = %d dev = %p\000"
.LC6:
	.ascii	"\033[31m\033[1m%s,%d:error: index = %d\033[0m\000"
.LC7:
	.ascii	"\033[31m\033[1m%s,%d:error: ioctl is null\033[0m\000"
.LC8:
	.ascii	"VLOG_IOCTL_VERSION\000"
.LC9:
	.ascii	"VLOG_IOCTL_SET_LOG_LEVEL\000"
.LC10:
	.ascii	"off\000"
.LC11:
	.ascii	"err\000"
.LC12:
	.ascii	"wrn\000"
.LC13:
	.ascii	"inf\000"
.LC14:
	.ascii	"dbg\000"
.LC15:
	.ascii	"cmpi\000"
.LC16:
	.ascii	"mb\000"
.LC17:
	.ascii	"sys\000"
.LC18:
	.ascii	"rgn\000"
.LC19:
	.ascii	"venc\000"
.LC20:
	.ascii	"vdec\000"
.LC21:
	.ascii	"vpss\000"
.LC22:
	.ascii	"vgs\000"
.LC23:
	.ascii	"vi\000"
.LC24:
	.ascii	"vo\000"
.LC25:
	.ascii	"ai\000"
.LC26:
	.ascii	"ao\000"
.LC27:
	.ascii	"aenc\000"
.LC28:
	.ascii	"adec\000"
.LC29:
	.ascii	"tde\000"
.LC30:
	.ascii	"isp\000"
.LC31:
	.ascii	"wbc\000"
.LC32:
	.ascii	"avs\000"
.LC33:
	.ascii	"rga\000"
.LC34:
	.ascii	"af\000"
.LC35:
	.ascii	"ivs\000"
	.section	___ksymtab_gpl+vlog,"a"
	.align	2
	.type	__ksymtab_vlog, %object
	.size	__ksymtab_vlog, 12
__ksymtab_vlog:
	.word	vlog
	.word	__kstrtab_vlog
	.word	__kstrtabns_vlog
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
