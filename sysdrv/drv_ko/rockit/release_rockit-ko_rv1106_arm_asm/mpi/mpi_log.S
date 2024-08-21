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
	.global	vlog
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vlog, %function
vlog:
	.fnstart
	@ args = 4, pretend = 0, frame = 288
	@ frame_needed = 0, uses_anonymous_args = 1
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r9, r3
	ldr	r5, .L5
	.pad #320
	sub	sp, sp, #320
	ldr	r4, .L5+4
	mov	r7, r0
	mov	r8, r1
	mov	r10, r2
	ldr	r3, [r5]
	str	r3, [sp, #316]
	ldr	r3, [r4, r0, lsl #2]
	cmp	r3, r1
	blt	.L3
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	add	r0, sp, #40
	add	r7, r4, r7, lsl #2
	bl	ktime_get_ts64
	ldr	r0, .L5+8
	add	r4, r4, r8, lsl #2
	ldrh	r3, [sp, #352]
	str	r10, [sp, #28]
	strd	r9, r3, [sp, #20]
	ldr	r3, [sp, #48]
	smull	r0, r1, r3, r0
	asrs	r3, r3, #31
	add	r0, sp, #60
	rsb	r3, r3, r1, asr #6
	str	r3, [sp, #16]
	ldrd	r2, [sp, #40]
	mov	r1, #256
	strd	r2, [sp, #8]
	ldr	r3, [r4, #192]
	ldr	r2, .L5+12
	str	r3, [sp]
	ldr	r3, [r7, #96]
	bl	snprintf
	ldr	r0, .L5+16
	add	r2, sp, #356
	add	r1, sp, #60
	str	r2, [sp, #36]
	bl	mpi_fs_buf_loop_add
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L3:
	ldr	r2, [sp, #316]
	movs	r0, #0
	ldr	r3, [r5]
	cmp	r2, r3
	beq	.L4
	bl	__stack_chk_fail
.L4:
	add	sp, sp, #320
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L6:
	.align	2
.L5:
	.word	__stack_chk_guard
	.word	.LANCHOR0
	.word	274877907
	.word	.LC0
	.word	.LANCHOR1+168
	.fnend
	.size	vlog, .-vlog
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
.L8:
	bl	kthread_should_stop
	cbz	r0, .L11
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L11:
	ldr	r3, [r4, #204]
	cbz	r3, .L9
	movs	r0, #1
	bl	msleep
	b	.L8
.L9:
	mov	r0, r5
	bl	down
	b	.L8
	.fnend
	.size	vlog_thread_fn, .-vlog_thread_fn
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
	ldr	r2, .L13
	movs	r1, #5
	movs	r0, #0
	ldr	r3, [r3]
	str	r3, [sp, #4]
	movs	r3, #231
	str	r3, [sp]
	ldr	r3, .L13+4
	bl	vlog
	mov	r0, r4
	bl	kfree
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L14:
	.align	2
.L13:
	.word	.LC1
	.word	.LANCHOR2
	.fnend
	.size	vlog_release, .-vlog_release
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
	ldr	r0, .L17
	bl	mpi_fs_buf_read
	cmp	r0, #-1
	it	eq
	moveq	r0, #0
	pop	{r3, pc}
.L18:
	.align	2
.L17:
	.word	.LANCHOR1+168
	.fnend
	.size	vlog_read, .-vlog_read
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
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r3, [r3]
	ldr	r6, [r3, #160]
	ldr	r3, [r1, #32]	@ unaligned
	cmp	r3, #76
	beq	.L20
	movs	r3, #123
	ldr	r2, .L24
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L24+4
	movs	r0, #0
	bl	vlog
	mov	r0, #-1
.L19:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L20:
	movs	r5, #0
	movs	r3, #1
	strb	r5, [r1, #1]
	strb	r3, [r1]
	strb	r5, [r1, #2]
	strb	r5, [r1, #3]
	strb	r5, [r1, #4]
	strb	r5, [r1, #5]
	strb	r5, [r1, #6]
	strb	r5, [r1, #7]
	strb	r5, [r1, #8]
	strb	r5, [r1, #9]
	strb	r5, [r1, #10]
	strb	r5, [r1, #11]
	ldr	r0, [r6]
	bl	strlen
	cmp	r0, #16
	ite	ls
	strls	r0, [r4, #12]	@ unaligned
	strbhi	r5, [r4, #13]
	add	r0, r4, #16
	itttt	hi
	strbhi	r5, [r4, #14]
	movhi	r3, #16
	strbhi	r5, [r4, #15]
	strbhi	r3, [r4, #12]
	ldr	r2, [r4, #12]	@ unaligned
	ldr	r1, [r6]
	bl	strncpy
	movs	r0, #0
	strb	r0, [r4, #36]
	strb	r0, [r4, #37]
	strb	r0, [r4, #38]
	strb	r0, [r4, #39]
	strb	r0, [r4, #56]
	strb	r0, [r4, #57]
	strb	r0, [r4, #58]
	strb	r0, [r4, #59]
	b	.L19
.L25:
	.align	2
.L24:
	.word	.LC2
	.word	.LANCHOR2+13
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
	ldr	r3, [r1, #8]	@ unaligned
	movs	r0, #0
	ldr	r2, .L34
	str	r3, [sp, #8]
	ldr	r3, [r1]	@ unaligned
	movs	r1, #4
	str	r3, [sp, #4]
	movs	r3, #148
	str	r3, [sp]
	ldr	r3, .L34+4
	bl	vlog
	ldr	r3, [r4, #4]	@ unaligned
	cmp	r3, #12
	beq	.L27
	movs	r3, #150
	ldr	r2, .L34+8
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L34+4
	movs	r0, #0
	bl	vlog
.L31:
	mov	r0, #-1
	b	.L26
.L27:
	ldr	r2, [r4]	@ unaligned
	cmp	r2, #24
	bhi	.L31
	ldr	r3, .L34+12
	bne	.L29
	movs	r2, #0
.L30:
	ldr	r1, [r4, #8]	@ unaligned
	adds	r2, r2, #1
	cmp	r2, #24
	str	r1, [r3], #4
	bne	.L30
.L33:
	movs	r0, #0
.L26:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, pc}
.L29:
	ldr	r1, [r4, #8]	@ unaligned
	str	r1, [r3, r2, lsl #2]
	b	.L33
.L35:
	.align	2
.L34:
	.word	.LC3
	.word	.LANCHOR2+26
	.word	.LC4
	.word	.LANCHOR0
	.fnend
	.size	vlog_set_level, .-vlog_set_level
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
	ldr	r3, .L40
	mov	r6, r0
	mov	r5, r1
	mov	r7, r2
	adds	r3, r3, #4
	ldr	r2, .L40+4
	ldr	r8, [r3, #160]
	movs	r1, #5
	strd	r4, r3, [sp, #8]
	movs	r0, #0
	movs	r3, #184
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r3, .L40+8
	bl	vlog
	ldr	r0, [r8, #212]
	cmp	r0, r4
	bhi	.L37
	movs	r3, #187
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L40+8
	movs	r0, #0
	ldr	r2, .L40+12
	bl	vlog
.L38:
.L36:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L37:
	.syntax unified
@ 87 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
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
	cbnz	r0, .L39
	movs	r3, #194
	ldr	r2, .L40+16
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L40+8
	bl	vlog
	b	.L36
.L39:
	mov	r1, r0
	mov	r3, r7
	mov	r2, r5
	mov	r0, r6
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L41:
	.align	2
.L40:
	.word	.LANCHOR1
	.word	.LC5
	.word	.LANCHOR2+41
	.word	.LC6
	.word	.LC7
	.fnend
	.size	vlog_ioctl, .-vlog_ioctl
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
	ldr	r3, .L45
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r7, r1
	ldr	r0, [r3, #12]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r5, r0
	cbnz	r0, .L43
	movs	r3, #218
	ldr	r2, .L45+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L45+8
	bl	vlog
	mov	r0, #-1
.L42:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L43:
	ldr	r4, .L45+12
	adds	r6, r4, #4
	mov	r0, r6
	bl	mpi_obj_ref
	ldr	r3, [r4, #4]
	str	r6, [r5]
	movs	r1, #5
	str	r5, [r7, #128]
	movs	r0, #0
	ldr	r2, .L45+16
	str	r3, [sp, #4]
	movs	r3, #223
	str	r3, [sp]
	ldr	r3, .L45+8
	bl	vlog
	movs	r0, #0
	b	.L42
.L46:
	.align	2
.L45:
	.word	kmalloc_caches
	.word	.LC8
	.word	.LANCHOR2+52
	.word	.LANCHOR1
	.word	.LC1
	.fnend
	.size	vlog_open, .-vlog_open
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
	ldr	r4, .L49
	mov	r5, r0
	movs	r1, #0
	ldr	r6, .L49+4
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
	movs	r2, #0
	add	r0, r4, #192
	add	r1, r6, #40
	strd	r0, r0, [r4, #192]
	adds	r0, r4, #4
	str	r2, [r4, #72]
	strd	r2, r2, [r4, #80]
	str	r2, [r4, #188]
	bl	mpi_sysfs_dev_add
	mov	r3, r5
	mov	r2, #-1
	mov	r1, r4
	ldr	r0, .L49+8
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L48
	bl	wake_up_process
.L48:
	str	r5, [r4, #200]
	pop	{r4, r5, r6, pc}
.L50:
	.align	2
.L49:
	.word	.LANCHOR1
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
	.type	__func__.35894, %object
	.size	__func__.35894, 13
__func__.35894:
	.ascii	"vlog_release\000"
	.type	__func__.35842, %object
	.size	__func__.35842, 13
__func__.35842:
	.ascii	"vlog_version\000"
	.type	__func__.35849, %object
	.size	__func__.35849, 15
__func__.35849:
	.ascii	"vlog_set_level\000"
	.type	__func__.35865, %object
	.size	__func__.35865, 11
__func__.35865:
	.ascii	"vlog_ioctl\000"
	.type	__func__.35887, %object
	.size	__func__.35887, 10
__func__.35887:
	.ascii	"vlog_open\000"
	.space	2
	.type	vlog_ioctls, %object
	.size	vlog_ioctls, 40
vlog_ioctls:
	.word	-1068733440
	.word	0
	.word	vlog_version
	.word	.LC9
	.word	0
	.word	-1072927743
	.word	0
	.word	vlog_set_level
	.word	.LC10
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
	.set	.LANCHOR0,. + 0
	.type	current_log_level, %object
	.size	current_log_level, 96
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
	.word	4
	.word	4
	.word	4
	.type	module_name, %object
	.size	module_name, 96
module_name:
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
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.type	level_name, %object
	.size	level_name, 24
level_name:
	.word	.LC11
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	g_vlog_dev, %object
	.size	g_vlog_dev, 216
g_vlog_dev:
	.space	216
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"[%-6.6s][%-3.3s][%9lld.%-6ld] {%-18.18s:%04d} %s\015"
	.ascii	"\012\000"
.LC1:
	.ascii	"%d\000"
.LC2:
	.ascii	"\033[1;31mstruct vlog_version mismatching\033[0m\000"
.LC3:
	.ascii	"\033[1;32mmod = %d, level = %d\033[0m\000"
.LC4:
	.ascii	"\033[1;31mstruct vlog_level mismatching\033[0m\000"
.LC5:
	.ascii	"vlog_ioctl: nr = %d, index = %d dev = %p\000"
.LC6:
	.ascii	"\033[1;31merror: index = %d\033[0m\000"
.LC7:
	.ascii	"\033[1;31merror: ioctl is null\033[0m\000"
.LC8:
	.ascii	"\033[1;31mvalloc open fail\033[0m\000"
.LC9:
	.ascii	"VLOG_IOCTL_VERSION\000"
.LC10:
	.ascii	"VLOG_IOCTL_SET_LOG_LEVEL\000"
.LC11:
	.ascii	"off\000"
.LC12:
	.ascii	"err\000"
.LC13:
	.ascii	"wrn\000"
.LC14:
	.ascii	"inf\000"
.LC15:
	.ascii	"dbg\000"
.LC16:
	.ascii	"cmpi\000"
.LC17:
	.ascii	"mb\000"
.LC18:
	.ascii	"sys\000"
.LC19:
	.ascii	"rgn\000"
.LC20:
	.ascii	"venc\000"
.LC21:
	.ascii	"vdec\000"
.LC22:
	.ascii	"vpss\000"
.LC23:
	.ascii	"vgs\000"
.LC24:
	.ascii	"vi\000"
.LC25:
	.ascii	"vo\000"
.LC26:
	.ascii	"ai\000"
.LC27:
	.ascii	"ao\000"
.LC28:
	.ascii	"aenc\000"
.LC29:
	.ascii	"adec\000"
.LC30:
	.ascii	"tde\000"
.LC31:
	.ascii	"isp\000"
.LC32:
	.ascii	"wbc\000"
.LC33:
	.ascii	"avs\000"
.LC34:
	.ascii	"rga\000"
.LC35:
	.ascii	"af\000"
.LC36:
	.ascii	"ivs\000"
.LC37:
	.ascii	"gpu\000"
.LC38:
	.ascii	"nn\000"
.LC39:
	.ascii	"aiisp\000"
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
