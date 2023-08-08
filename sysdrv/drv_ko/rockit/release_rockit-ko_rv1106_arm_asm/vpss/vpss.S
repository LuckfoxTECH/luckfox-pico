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
	.file	"vpss.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	vpss_map_node_get
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_map_node_get, %function
vpss_map_node_get:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vpss_map_node_get, .-vpss_map_node_get
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_compat_ioctl, %function
vpss_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vpss_compat_ioctl, .-vpss_compat_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_open, %function
vpss_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L4
	movs	r0, #0
	str	r3, [r1, #128]
	bx	lr
.L5:
	.align	2
.L4:
	.word	.LANCHOR0+4
	.fnend
	.size	vpss_open, .-vpss_open
	.align	1
	.global	vpss_map_node_set
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_map_node_set, %function
vpss_map_node_set:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r1
	ldrd	r5, r3, [r1, #8]
	.pad #28
	sub	sp, sp, #28
	cmp	r3, #28
	beq	.L7
	movs	r3, #71
	ldr	r2, .L20
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L20+4
	movs	r0, #0
.L17:
	bl	vlog
	b	.L16
.L7:
	ldr	r0, [r1, #16]
	cmp	r0, #18
	beq	.L9
	movs	r3, #76
	ldr	r2, .L20+8
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L20+4
	str	r0, [sp, #4]
	movs	r0, #0
.L19:
	bl	vlog
.L16:
	mov	r0, #-1
.L6:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L9:
	ldrd	r1, r2, [r1, #20]
	bl	vsys_get_node_from_map
	mov	r7, r0
	cbnz	r0, .L10
	ldr	r3, [r4, #24]
	ldr	r2, .L20+12
	str	r3, [sp, #4]
	movs	r3, #82
	str	r3, [sp]
	ldr	r3, .L20+4
.L18:
	movs	r1, #2
	b	.L19
.L10:
	ldrb	r6, [r4]	@ zero_extendqisi2
	cbnz	r6, .L11
	mov	r2, r6
	ldr	r1, [r4, #4]
	movs	r0, #6
	adds	r5, r5, #1
	bl	vsys_get_node_from_map
	cbnz	r0, .L12
	ldr	r3, [r4, #4]
	ldr	r2, .L20+16
	str	r3, [sp, #4]
	movs	r3, #89
	str	r3, [sp]
	ldr	r3, .L20+4
	b	.L18
.L12:
	mov	r1, r7
	bl	mpi_node_bind
	cbz	r0, .L11
	movs	r3, #94
	ldr	r2, .L20+20
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L20+4
	mov	r0, r6
	b	.L17
.L11:
	mov	r3, r7
	mov	r2, r5
	ldr	r1, [r4, #4]
	movs	r0, #6
	bl	vsys_add_node_to_map
	ldr	r3, [r4, #24]
	ldr	r2, .L20+24
	movs	r1, #5
	movs	r0, #0
	str	r3, [sp, #16]
	ldr	r3, [r4, #8]
	str	r3, [sp, #12]
	ldr	r3, [r4, #4]
	str	r3, [sp, #8]
	ldrb	r3, [r4]	@ zero_extendqisi2
	str	r3, [sp, #4]
	movs	r3, #101
	str	r3, [sp]
	ldr	r3, .L20+4
	bl	vlog
	movs	r0, #0
	b	.L6
.L21:
	.align	2
.L20:
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.fnend
	.size	vpss_map_node_set, .-vpss_map_node_set
	.align	1
	.global	vpss_map_node_remove
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_map_node_remove, %function
vpss_map_node_remove:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r4, r1
	ldrd	r6, r3, [r1, #8]
	.pad #20
	sub	sp, sp, #20
	cmp	r3, #28
	beq	.L23
	movs	r3, #118
	ldr	r2, .L39
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L39+4
	movs	r0, #0
	bl	vlog
.L35:
	mov	r0, #-1
.L22:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L23:
	ldr	r3, [r1, #16]
	cmp	r3, #18
	beq	.L25
	str	r3, [sp, #4]
	movs	r3, #122
	ldr	r2, .L39+8
	movs	r1, #2
	str	r3, [sp]
	movs	r0, #0
	ldr	r3, .L39+4
.L38:
	bl	vlog
	b	.L35
.L25:
	movs	r2, #0
	ldr	r1, [r1, #4]
	movs	r0, #6
	bl	vsys_get_node_from_map
	mov	r7, r0
	cbnz	r0, .L26
	ldr	r3, [r4, #4]
	ldr	r2, .L39+12
	str	r3, [sp, #4]
	movs	r3, #128
	str	r3, [sp]
	ldr	r3, .L39+4
.L37:
	movs	r1, #2
	b	.L38
.L26:
	ldrb	r5, [r4]	@ zero_extendqisi2
	cbnz	r5, .L30
	adds	r6, r6, #1
	ldr	r1, [r4, #4]
	movs	r0, #6
	mov	r2, r6
	bl	vsys_get_node_from_map
	mov	r1, r0
	cbnz	r0, .L28
	movs	r3, #136
	str	r6, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L39+16
	ldr	r3, .L39+4
	b	.L37
.L28:
	mov	r0, r7
	bl	mpi_node_unbind
	mov	r2, r6
	ldr	r1, [r4, #4]
	movs	r0, #6
	bl	vsys_remove_node_from_map
	ldr	r3, [r4, #8]
	ldr	r2, .L39+20
	movs	r1, #5
	mov	r0, r5
	str	r3, [sp, #8]
	ldr	r3, [r4, #4]
	str	r6, [sp, #12]
	str	r3, [sp, #4]
	movs	r3, #142
	str	r3, [sp]
	ldr	r3, .L39+4
	bl	vlog
.L36:
	movs	r0, #0
	b	.L22
.L30:
	ldr	r6, .L39+4
	movs	r5, #1
	ldr	r8, .L39+24
.L27:
	ldr	r1, [r4, #4]
	mov	r2, r5
	movs	r0, #6
	bl	vsys_get_node_from_map
	mov	r1, r0
	cbz	r0, .L29
	ldr	r9, [r0, #56]
	mov	r0, r7
	bl	mpi_node_unbind
	mov	r2, r5
	ldr	r1, [r4, #4]
	movs	r0, #6
	bl	vsys_remove_node_from_map
	subs	r3, r5, #1
	mov	r2, r8
	movs	r1, #5
	movs	r0, #0
	str	r3, [sp, #8]
	ldr	r3, [r4, #4]
	str	r9, [sp, #12]
	str	r3, [sp, #4]
	movs	r3, #153
	str	r3, [sp]
	mov	r3, r6
	bl	vlog
.L29:
	adds	r5, r5, #1
	cmp	r5, #5
	bne	.L27
	movs	r2, #0
	ldr	r1, [r4, #4]
	movs	r0, #6
	bl	vsys_remove_node_from_map
	b	.L36
.L40:
	.align	2
.L39:
	.word	.LC0
	.word	.LANCHOR1+18
	.word	.LC1
	.word	.LC3
	.word	.LC2
	.word	.LC6
	.word	.LC7
	.fnend
	.size	vpss_map_node_remove, .-vpss_map_node_remove
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_thread_fn, %function
vpss_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	add	r5, r0, #168
	mov	r4, r0
.L42:
	ldr	r3, [r4, #184]
	cbz	r3, .L43
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L43:
	mov	r0, r5
	bl	down
	b	.L42
	.fnend
	.size	vpss_thread_fn, .-vpss_thread_fn
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_ioctl, %function
vpss_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #16
	uxtb	r4, r1
	ldr	r3, [r0, #128]
	mov	r5, r0
	mov	r6, r1
	mov	r7, r2
	movs	r1, #5
	ldr	r2, .L48
	ldr	r8, [r3, #160]
	movs	r0, #0
	strd	r4, r3, [sp, #8]
	movs	r3, #183
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r3, .L48+4
	bl	vlog
	ldr	r0, [r8, #192]
	cmp	r0, r4
	bhi	.L45
	movs	r3, #186
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L48+4
	movs	r0, #0
	ldr	r2, .L48+8
	bl	vlog
.L46:
.L44:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L45:
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
	cmp	r4, r0
	sbc	r0, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r4, r4, r0
	ldr	r0, [r8, #188]
	movs	r3, #20
	mla	r0, r3, r4, r0
	cbnz	r0, .L47
	movs	r3, #193
	ldr	r2, .L48+12
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L48+4
	bl	vlog
	b	.L44
.L47:
	mov	r1, r0
	mov	r3, r7
	mov	r2, r6
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L49:
	.align	2
.L48:
	.word	.LC8
	.word	.LANCHOR1+39
	.word	.LC9
	.word	.LC10
	.fnend
	.size	vpss_ioctl, .-vpss_ioctl
	.align	1
	.global	vpss_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_version, %function
vpss_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r5, [r3, #160]
	ldr	r3, [r1, #32]
	cmp	r3, #76
	beq	.L51
	movs	r3, #48
	ldr	r2, .L55
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L55+4
	movs	r0, #0
	bl	vlog
	mov	r0, #-1
.L50:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L51:
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
	b	.L50
.L56:
	.align	2
.L55:
	.word	.LC0
	.word	.LANCHOR1+50
	.fnend
	.size	vpss_version, .-vpss_version
	.align	1
	.global	vpss_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_probe, %function
vpss_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movs	r2, #196
	ldr	r4, .L59
	mov	r6, r0
	movs	r1, #0
	ldr	r5, .L59+4
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r6
	bl	kstrdup
	movs	r3, #4
	mov	r2, r6
	movs	r1, #8
	str	r5, [r4, #188]
	str	r3, [r4, #192]
	str	r4, [r4, #164]
	str	r0, [r4]
	adds	r0, r4, r3
	bl	mpi_dev_register
	add	r1, r5, #80
	add	r0, r4, #172
	strd	r0, r0, [r4, #172]
	adds	r0, r4, #4
	movs	r3, #0
	str	r3, [r4, #72]
	strd	r3, r3, [r4, #80]
	str	r3, [r4, #168]
	bl	mpi_sysfs_dev_add
	ldr	r3, .L59+8
	mov	r2, #-1
	mov	r1, r4
	ldr	r0, .L59+12
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L58
	bl	wake_up_process
.L58:
	str	r5, [r4, #180]
	pop	{r4, r5, r6, pc}
.L60:
	.align	2
.L59:
	.word	.LANCHOR0
	.word	.LANCHOR1+64
	.word	.LC11
	.word	vpss_thread_fn
	.fnend
	.size	vpss_probe, .-vpss_probe
	.align	1
	.global	vpss_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_shutdown, %function
vpss_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L62
	movs	r3, #1
	adds	r0, r0, #4
	str	r3, [r0, #180]
	b	mpi_dev_unregister
.L63:
	.align	2
.L62:
	.word	.LANCHOR0
	.fnend
	.size	vpss_shutdown, .-vpss_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	vpss_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_init, %function
vpss_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L65
	bl	vpss_probe
	movs	r0, #0
	pop	{r3, pc}
.L66:
	.align	2
.L65:
	.word	.LC11
	.fnend
	.size	vpss_init, .-vpss_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	vpss_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vpss_exit, %function
vpss_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	vpss_shutdown
	.fnend
	.size	vpss_exit, .-vpss_exit
	.global	g_vpss_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__func__.36267, %object
	.size	__func__.36267, 18
__func__.36267:
	.ascii	"vpss_map_node_set\000"
	.type	__func__.36281, %object
	.size	__func__.36281, 21
__func__.36281:
	.ascii	"vpss_map_node_remove\000"
	.type	__func__.36298, %object
	.size	__func__.36298, 11
__func__.36298:
	.ascii	"vpss_ioctl\000"
	.type	__func__.36257, %object
	.size	__func__.36257, 13
__func__.36257:
	.ascii	"vpss_version\000"
	.space	1
	.type	vpss_ioctls, %object
	.size	vpss_ioctls, 80
vpss_ioctls:
	.word	-1068732416
	.word	0
	.word	vpss_version
	.word	.LC12
	.word	0
	.word	-1071878143
	.word	0
	.word	vpss_map_node_set
	.word	.LC13
	.word	0
	.word	-1071878142
	.word	0
	.word	vpss_map_node_get
	.word	.LC14
	.word	0
	.word	-1071878141
	.word	0
	.word	vpss_map_node_remove
	.word	.LC15
	.word	0
	.type	vpss_driver_fops, %object
	.size	vpss_driver_fops, 128
vpss_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	0
	.word	vpss_ioctl
	.word	vpss_compat_ioctl
	.word	0
	.space	4
	.word	vpss_open
	.space	4
	.word	0
	.space	60
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	g_vpss_dev, %object
	.size	g_vpss_dev, 196
g_vpss_dev:
	.space	196
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[31m\033[1m%s,%d:struct vpss_version mismatchin"
	.ascii	"g\033[0m\000"
.LC1:
	.ascii	"\033[31m\033[1m%s,%d:vpss sub mode id fail mod_id ="
	.ascii	" %d\033[0m\000"
.LC2:
	.ascii	"\033[31m\033[1m%s,%d:vpss get sub node fail channel"
	.ascii	"_id = %d\033[0m\000"
.LC3:
	.ascii	"\033[31m\033[1m%s,%d:vpss get grp node fail grp_id "
	.ascii	"= %d\033[0m\000"
.LC4:
	.ascii	"\033[31m\033[1m%s,%d:vpss bind sub node fail\033[0m"
	.ascii	"\000"
.LC5:
	.ascii	"%s,%d:vpss map is_grp = %d, grp_id = %d, chn_id = %"
	.ascii	"d, sub_ch_id = %d\000"
.LC6:
	.ascii	"%s,%d:vpss grp_id = %d channel_id = %d unbind sub n"
	.ascii	"ode channel_id = %d success\000"
.LC7:
	.ascii	"%s,%d:vpss grp_id = %d channel_id = %d unmap sub no"
	.ascii	"de channel_id = %d success\000"
.LC8:
	.ascii	"%s,%d:nr = %d, index = %d dev = %p\000"
.LC9:
	.ascii	"\033[31m\033[1m%s,%d:error: index = %d\033[0m\000"
.LC10:
	.ascii	"\033[31m\033[1m%s,%d:error: ioctl is null\033[0m\000"
.LC11:
	.ascii	"vpss\000"
.LC12:
	.ascii	"VPSS_IOCTL_VERSION\000"
.LC13:
	.ascii	"VPSS_IOCTL_MAP_NODE_SET\000"
.LC14:
	.ascii	"VPSS_IOCTL_MAP_NODE_GET\000"
.LC15:
	.ascii	"VPSS_IOCTL_MAP_NODE_REMOVE\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
