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
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	ldr	r2, [r1, #32]	@ unaligned
	ldr	r5, [r0, #128]
	cmp	r2, #76
	ldr	r6, [r5, #160]
	beq	.L7
	movs	r3, #60
	ldr	r2, .L10
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L10+4
	movs	r0, #6
	bl	vlog
	mov	r0, #-1
.L6:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L7:
	movs	r1, #0
	mov	r0, r4
	bl	memset
	ldr	r3, [r6, #196]
	str	r3, [r4]	@ unaligned
	ldr	r3, [r5]
	movs	r5, #0
	strb	r5, [r4, #8]
	strb	r5, [r4, #9]
	str	r3, [r4, #4]	@ unaligned
	strb	r5, [r4, #10]
	strb	r5, [r4, #11]
	ldr	r0, [r6, #192]
	bl	strlen
	movs	r3, #20
	strb	r5, [r4, #37]
	strb	r3, [r4, #36]
	movs	r3, #17
	strb	r5, [r4, #38]
	strb	r5, [r4, #39]
	strb	r3, [r4, #56]
	strb	r5, [r4, #57]
	strb	r5, [r4, #58]
	strb	r5, [r4, #59]
	cmp	r0, #15
	str	r0, [r4, #12]	@ unaligned
	bls	.L9
	movs	r3, #15
	strb	r5, [r4, #13]
	strb	r3, [r4, #12]
	strb	r5, [r4, #14]
	strb	r5, [r4, #15]
.L9:
	movs	r5, #0
	movs	r3, #16
	strb	r5, [r4, #37]
	adds	r0, r4, r3
	strb	r3, [r4, #36]
	strb	r3, [r4, #56]
	strb	r5, [r4, #38]
	strb	r5, [r4, #39]
	strb	r5, [r4, #57]
	strb	r5, [r4, #58]
	strb	r5, [r4, #59]
	ldr	r2, [r4, #12]	@ unaligned
	ldr	r1, [r6, #192]
	bl	strncpy
	ldr	r2, [r4, #36]	@ unaligned
	ldr	r1, .L10+8
	add	r0, r4, #40
	bl	strncpy
	ldr	r2, [r4, #56]	@ unaligned
	ldr	r1, .L10+12
	add	r0, r4, #60
	bl	strncpy
	mov	r0, r5
	b	.L6
.L11:
	.align	2
.L10:
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC1
	.word	.LC2
	.fnend
	.size	vpss_version, .-vpss_version
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
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r1
	ldr	r3, [r1, #9]	@ unaligned
	.pad #24
	sub	sp, sp, #24
	ldr	r6, [r1, #5]	@ unaligned
	cmp	r3, #25
	beq	.L13
	movs	r3, #93
	ldr	r2, .L35
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L35+4
	movs	r0, #6
	bl	vlog
.L32:
	mov	r0, #-1
.L12:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L13:
	ldr	r0, [r1, #13]	@ unaligned
	cmp	r0, #18
	beq	.L15
	cmp	r0, #23
	beq	.L15
	movs	r3, #98
	ldr	r2, .L35+8
	str	r3, [sp]
	ldr	r3, .L35+4
	str	r0, [sp, #4]
.L34:
	movs	r1, #2
	movs	r0, #6
	bl	vlog
	b	.L32
.L15:
	ldr	r1, [r4, #17]	@ unaligned
	ldr	r2, [r4, #21]	@ unaligned
	bl	vsys_get_node_from_map
	ldr	r1, [r4, #1]	@ unaligned
	mov	r5, r0
	cbnz	r0, .L16
	ldr	r3, [r4, #21]	@ unaligned
	ldr	r2, .L35+12
	str	r3, [sp, #16]
	ldr	r3, [r4, #17]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #13]	@ unaligned
	strd	r1, r3, [sp, #4]
	movs	r3, #106
	str	r3, [sp]
	ldr	r3, .L35+4
.L33:
	movs	r1, #2
	movs	r0, #6
	bl	vlog
	b	.L32
.L16:
	ldrb	r2, [r4]	@ zero_extendqisi2
	cbnz	r2, .L17
	movs	r0, #6
	adds	r6, r6, #1
	bl	vsys_get_node_from_map
	mov	r1, r0
	cbnz	r0, .L18
	ldr	r3, [r4, #1]	@ unaligned
	ldr	r2, .L35+16
	str	r3, [sp, #4]
	movs	r3, #113
	str	r3, [sp]
	ldr	r3, .L35+4
	b	.L34
.L18:
	ldr	r3, [r4, #13]	@ unaligned
	cmp	r3, #18
	bne	.L19
	mov	r1, r5
.L31:
	bl	mpi_node_bind
	cbz	r0, .L17
	ldr	r3, [r4, #21]	@ unaligned
	ldr	r2, .L35+20
	str	r3, [sp, #16]
	ldr	r3, [r4, #17]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #13]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4, #1]	@ unaligned
	str	r3, [sp, #4]
	movs	r3, #124
	str	r3, [sp]
	ldr	r3, .L35+4
	b	.L33
.L19:
	cmp	r3, #23
	bne	.L17
	mov	r0, r5
	b	.L31
.L17:
	mov	r3, r5
	mov	r2, r6
	ldr	r1, [r4, #1]	@ unaligned
	movs	r0, #6
	bl	vsys_add_node_to_map
	ldrb	r5, [r4]	@ zero_extendqisi2
	ldr	r2, [r4, #21]	@ unaligned
	ldr	r3, [r4, #1]	@ unaligned
	cbz	r5, .L21
	strd	r3, r2, [sp, #4]
	movs	r1, #4
	movs	r3, #132
	ldr	r2, .L35+24
	str	r3, [sp]
	movs	r0, #6
	ldr	r3, .L35+4
	bl	vlog
	movs	r0, #0
	b	.L12
.L21:
	str	r2, [sp, #12]
	movs	r1, #4
	ldr	r2, [r4, #5]	@ unaligned
	movs	r0, #6
	strd	r3, r2, [sp, #4]
	movs	r3, #136
	ldr	r2, .L35+28
	str	r3, [sp]
	ldr	r3, .L35+4
	bl	vlog
	mov	r0, r5
	b	.L12
.L36:
	.align	2
.L35:
	.word	.LC3
	.word	.LANCHOR1+13
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.word	.LC8
	.word	.LC9
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
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r4, r1
	ldr	r3, [r1, #9]	@ unaligned
	ldr	r7, [r1, #5]	@ unaligned
	cmp	r3, #25
	beq	.L38
	movs	r3, #159
	ldr	r2, .L63
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L63+4
	movs	r0, #6
	bl	vlog
.L60:
	mov	r0, #-1
.L37:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L38:
	ldr	r3, [r1, #13]	@ unaligned
	cmp	r3, #18
	beq	.L40
	cmp	r3, #23
	beq	.L40
	str	r3, [sp, #4]
	movs	r3, #164
	ldr	r2, .L63+8
	str	r3, [sp]
	ldr	r3, .L63+4
.L62:
	movs	r1, #2
	movs	r0, #6
	bl	vlog
	b	.L60
.L40:
	ldr	r1, [r4, #1]	@ unaligned
	movs	r2, #0
	movs	r0, #6
	bl	vsys_get_node_from_map
	ldr	r1, [r4, #1]	@ unaligned
	mov	r6, r0
	cbnz	r0, .L41
	movs	r3, #170
	str	r1, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L63+12
	ldr	r3, .L63+4
	b	.L62
.L41:
	ldrb	r3, [r4]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L49
	adds	r7, r7, #1
	movs	r0, #6
	mov	r2, r7
	bl	vsys_get_node_from_map
	mov	r1, r0
	cbnz	r0, .L43
	movs	r3, #178
	str	r7, [sp, #4]
	str	r3, [sp]
	ldr	r2, .L63+16
	ldr	r3, .L63+4
	b	.L62
.L43:
	ldr	r3, [r4, #13]	@ unaligned
	cmp	r3, #18
	bne	.L44
	mov	r0, r6
	bl	mpi_node_unbind
	ldr	r3, [r4, #5]	@ unaligned
	ldr	r2, .L63+20
	movs	r1, #4
	movs	r0, #6
	str	r3, [sp, #8]
	ldr	r3, [r4, #1]	@ unaligned
	str	r7, [sp, #12]
	str	r3, [sp, #4]
	movs	r3, #184
	str	r3, [sp]
	ldr	r3, .L63+4
	bl	vlog
.L45:
	mov	r2, r7
	ldr	r1, [r4, #1]	@ unaligned
	movs	r0, #6
	bl	vsys_remove_node_from_map
.L61:
	movs	r0, #0
	b	.L37
.L44:
	cmp	r3, #23
	bne	.L45
	mov	r1, r6
	bl	mpi_node_unbind
	ldr	r3, [r4, #1]	@ unaligned
	ldr	r2, .L63+24
	movs	r1, #4
	movs	r0, #6
	str	r7, [sp, #8]
	str	r3, [sp, #4]
	movs	r3, #188
	str	r3, [sp]
	ldr	r3, .L63+4
	bl	vlog
	b	.L45
.L49:
	ldr	r8, .L63+4
	movs	r5, #1
	ldr	r9, .L63+32
	ldr	r10, .L63+36
.L42:
	ldr	r1, [r4, #1]	@ unaligned
	mov	r2, r5
	movs	r0, #6
	bl	vsys_get_node_from_map
	mov	r1, r0
	cbz	r0, .L46
	cmp	r5, #5
	ldr	r7, [r0, #56]
	beq	.L47
	mov	r0, r6
	bl	mpi_node_unbind
	ldr	r3, [r4, #1]	@ unaligned
	subs	r2, r5, #1
	movs	r1, #4
	movs	r0, #6
	strd	r3, r2, [sp, #4]
	mov	r2, r10
	str	r3, [sp, #12]
	movs	r3, #201
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
.L48:
	mov	r2, r5
	ldr	r1, [r4, #1]	@ unaligned
	movs	r0, #6
	bl	vsys_remove_node_from_map
.L46:
	adds	r5, r5, #1
	cmp	r5, #6
	bne	.L42
	ldr	r1, [r4, #1]	@ unaligned
	movs	r2, #0
	mov	r0, r5
	bl	vsys_remove_node_from_map
	ldr	r3, [r4, #1]	@ unaligned
	ldr	r2, .L63+28
	movs	r1, #4
	mov	r0, r5
	str	r7, [sp, #8]
	str	r3, [sp, #4]
	movs	r3, #212
	str	r3, [sp]
	ldr	r3, .L63+4
	bl	vlog
	b	.L61
.L47:
	mov	r1, r6
	bl	mpi_node_unbind
	ldr	r3, [r4, #1]	@ unaligned
	mov	r2, r9
	movs	r1, #4
	movs	r0, #6
	strd	r3, r3, [sp, #4]
	movs	r3, #205
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
	b	.L48
.L64:
	.align	2
.L63:
	.word	.LC3
	.word	.LANCHOR1+31
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC17
	.word	.LC16
	.word	.LC15
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
.L66:
	bl	kthread_should_stop
	cbz	r0, .L69
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L69:
	ldr	r3, [r4]
	cbz	r3, .L67
	movs	r0, #1
	bl	msleep
	b	.L66
.L67:
	mov	r0, r5
	bl	down
	b	.L66
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
	ldr	r2, .L74
	ldr	r8, [r3, #160]
	movs	r0, #6
	strd	r4, r3, [sp, #8]
	movs	r3, #239
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r3, .L74+4
	bl	vlog
	ldr	r1, [r8, #188]
	cmp	r1, r4
	bhi	.L71
	movs	r3, #242
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L74+4
	movs	r0, #6
	ldr	r2, .L74+8
	bl	vlog
.L72:
.L70:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L71:
	.syntax unified
@ 87 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
	cmp	r4, r1
	sbc	r1, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r4, r4, r1
	ldr	r1, [r8, #184]
	movs	r3, #20
	mla	r1, r3, r4, r1
	cbnz	r1, .L73
	movs	r3, #249
	ldr	r2, .L74+12
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L74+4
	movs	r0, #6
	bl	vlog
	b	.L70
.L73:
	mov	r3, r7
	mov	r2, r6
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L75:
	.align	2
.L74:
	.word	.LC18
	.word	.LANCHOR1+52
	.word	.LC19
	.word	.LC20
	.fnend
	.size	vpss_ioctl, .-vpss_ioctl
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
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	movs	r2, #200
	ldr	r4, .L78
	mov	r7, r0
	movs	r1, #0
	ldr	r6, .L78+4
	movs	r5, #0
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r7
	bl	kstrdup
	movs	r3, #1
	mov	r2, r7
	str	r3, [r4, #196]
	movs	r1, #8
	movs	r3, #4
	str	r5, [r4, #72]
	str	r3, [r4, #188]
	strd	r5, r5, [r4, #80]
	str	r6, [r4, #184]
	str	r4, [r4, #164]
	str	r0, [r4, #192]
	adds	r0, r4, r3
	bl	mpi_dev_register
	mov	r2, r5
	add	r0, r4, #172
	add	r1, r6, #80
	strd	r0, r0, [r4, #172]
	adds	r0, r4, #4
	str	r5, [r4, #168]
	bl	mpi_sysfs_dev_add
	ldr	r3, .L78+8
	mov	r2, #-1
	mov	r1, r4
	ldr	r0, .L78+12
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L77
	bl	wake_up_process
.L77:
	str	r5, [r4, #180]
	pop	{r3, r4, r5, r6, r7, pc}
.L79:
	.align	2
.L78:
	.word	.LANCHOR0
	.word	.LANCHOR1+64
	.word	.LC21
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
	push	{r4, lr}
	.save {r4, lr}
	movs	r3, #1
	ldr	r4, .L81
	mov	r0, r4
	str	r3, [r0], #4
	bl	mpi_dev_unregister
	ldr	r0, [r4, #192]
	pop	{r4, lr}
	b	kfree
.L82:
	.align	2
.L81:
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
	ldr	r0, .L84
	bl	vpss_probe
	movs	r0, #0
	pop	{r3, pc}
.L85:
	.align	2
.L84:
	.word	.LC21
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
	.type	__func__.35758, %object
	.size	__func__.35758, 13
__func__.35758:
	.ascii	"vpss_version\000"
	.type	__func__.35768, %object
	.size	__func__.35768, 18
__func__.35768:
	.ascii	"vpss_map_node_set\000"
	.type	__func__.35782, %object
	.size	__func__.35782, 21
__func__.35782:
	.ascii	"vpss_map_node_remove\000"
	.type	__func__.35799, %object
	.size	__func__.35799, 11
__func__.35799:
	.ascii	"vpss_ioctl\000"
	.space	1
	.type	vpss_ioctls, %object
	.size	vpss_ioctls, 80
vpss_ioctls:
	.word	-1068732416
	.word	0
	.word	vpss_version
	.word	.LC22
	.word	0
	.word	-1072074751
	.word	0
	.word	vpss_map_node_set
	.word	.LC23
	.word	0
	.word	-1072074750
	.word	0
	.word	vpss_map_node_get
	.word	.LC24
	.word	0
	.word	-1072074749
	.word	0
	.word	vpss_map_node_remove
	.word	.LC25
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
	.size	g_vpss_dev, 200
g_vpss_dev:
	.space	200
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[1;31mstruct vpss_version mismatching\033[0m\000"
.LC1:
	.ascii	"-2024-05-24-16:21:01\000"
.LC2:
	.ascii	"vmpi:28ee389dbc33c2c7db812cd08e54fd3a0f2f992b-v1.4."
	.ascii	"81\000"
.LC3:
	.ascii	"\033[1;31mstruct vpss_map_info mismatching\033[0m\000"
.LC4:
	.ascii	"\033[1;31mVPSS sub mode id fail mod(%d)\033[0m\000"
.LC5:
	.ascii	"\033[1;31mVPSS GRP %d get sub node fail mod,dev,chn"
	.ascii	"(%d-%d-%d)\033[0m\000"
.LC6:
	.ascii	"\033[1;31mVPSS GRP %d get grp node fail\033[0m\000"
.LC7:
	.ascii	"\033[1;31mVPSS GRP %d bind sub node fail mod,dev,ch"
	.ascii	"n(%d-%d-%d)\033[0m\000"
.LC8:
	.ascii	"\033[1;32mVPSS GRP %d node map set to node %d succe"
	.ascii	"ss!\033[0m\000"
.LC9:
	.ascii	"\033[1;32mVPSS GRP %d CHN %d node map set to node %"
	.ascii	"d success!\033[0m\000"
.LC10:
	.ascii	"\033[1;31mVPSS sub mode id fail mod_id = %d\033[0m\000"
.LC11:
	.ascii	"\033[1;31mVPSS get grp node fail grp_id = %d\033[0m"
	.ascii	"\000"
.LC12:
	.ascii	"\033[1;31mVPSS get sub node fail channel_id = %d\033"
	.ascii	"[0m\000"
.LC13:
	.ascii	"\033[1;32mVPSS GRP %d CHN %d node map remove node %"
	.ascii	"d success!\033[0m\000"
.LC14:
	.ascii	"\033[1;32mVPSS GRP %d AIISP node map remove node %d"
	.ascii	" success!\033[0m\000"
.LC15:
	.ascii	"\033[1;32mVPSS GRP %d CHN %d node unbind to GRP %d "
	.ascii	"node success!\033[0m\000"
.LC16:
	.ascii	"\033[1;32mVPSS GRP %d AIISP node unbind to GRP %d n"
	.ascii	"ode success!\033[0m\000"
.LC17:
	.ascii	"\033[1;32mVPSS GRP %d node map remove node %d succe"
	.ascii	"ss!\033[0m\000"
.LC18:
	.ascii	"nr = %d, index = %d dev = %p\000"
.LC19:
	.ascii	"\033[1;31merror: index = %d\033[0m\000"
.LC20:
	.ascii	"\033[1;31merror: ioctl is null\033[0m\000"
.LC21:
	.ascii	"vpss\000"
.LC22:
	.ascii	"VPSS_IOCTL_VERSION\000"
.LC23:
	.ascii	"VPSS_IOCTL_MAP_NODE_SET\000"
.LC24:
	.ascii	"VPSS_IOCTL_MAP_NODE_GET\000"
.LC25:
	.ascii	"VPSS_IOCTL_MAP_NODE_REMOVE\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
