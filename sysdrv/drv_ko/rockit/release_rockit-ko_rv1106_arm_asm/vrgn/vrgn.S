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
	.file	"vrgn.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	list_del, %function
list_del:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldrd	r2, r3, [r0]
	str	r3, [r2, #4]
	str	r2, [r3]
	mov	r2, #256
	mov	r3, #290
	strd	r2, r3, [r0]
	bx	lr
	.fnend
	.size	list_del, .-list_del
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_compat_ioctl, %function
vrgn_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vrgn_compat_ioctl, .-vrgn_compat_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	node_delete_callback, %function
node_delete_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #12
	movs	r4, #0
	ldr	r3, [r0, #84]!
	subs	r3, r3, #4
.L4:
	adds	r2, r3, #4
	cmp	r2, r0
	bne	.L5
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r5, .L15
	ldr	r3, [r5, #20]
	cmp	r3, #1
	beq	.L6
.L10:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L5:
	str	r4, [r3]
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r5, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r3, #12]
	ldr	r2, [r1, #28]
	subs	r2, r2, #1
	str	r2, [r1, #28]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r5	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r3, #4]
	subs	r3, r3, #4
	b	.L4
.L6:
	ldr	r4, [r5, #196]!
	ldr	r8, .L15+4
	ldr	r9, .L15+8
	ldr	r6, [r4]
.L7:
	cmp	r4, r5
	beq	.L10
	cbz	r4, .L8
	ldr	r3, [r4, #28]
	cbnz	r3, .L8
	ldr	r3, [r4, #24]
	mov	r2, r9
	movs	r1, #4
	movs	r0, #3
	str	r3, [sp, #4]
	mov	r3, #378
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
	mov	r0, r4
	bl	list_del
	bl	kfree
.L8:
	mov	r4, r6
	ldr	r6, [r6]
	b	.L7
.L16:
	.align	2
.L15:
	.word	g_vrgn_dev
	.word	.LANCHOR0
	.word	.LC0
	.fnend
	.size	node_delete_callback, .-node_delete_callback
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	osd_sync_callback, %function
osd_sync_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r5, r0
	ldr	r4, [r5, #84]!
.L21:
	subs	r4, r4, #4
	adds	r3, r4, #4
	cmp	r3, r5
	bne	.L20
	pop	{r4, r5, r6, pc}
.L20:
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #48]
	cmp	r3, #1
	bne	.L19
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r6, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, [r4, #12]
	adds	r0, r0, #12
	bl	complete_all
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r6	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L19:
	ldr	r4, [r4, #4]
	b	.L21
	.fnend
	.size	osd_sync_callback, .-osd_sync_callback
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_release, %function
vrgn_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	ldr	r6, [r1, #128]
	ldr	r4, [r6]
	mov	r0, r4
	ldr	r5, [r4, #160]
	bl	mpi_obj_unref
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bne	.L27
	ldr	r2, [r4]
	ldr	r1, .L32
	ldr	r0, .L32+4
	bl	printk
	ldr	r4, [r5, #196]!
	ldr	r9, .L32
	ldr	r10, .L32+8
	ldr	r7, [r4]
.L24:
	cmp	r4, r5
	bne	.L26
.L27:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	adds	r0, r6, #4
	bl	mpi_fs_buf_deinit
	mov	r0, r6
	bl	kfree
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L26:
	cbz	r4, .L25
	ldr	r3, [r4, #28]
	cbnz	r3, .L25
	ldr	r3, [r4, #24]
	mov	r2, r10
	movs	r1, #4
	movs	r0, #3
	str	r3, [sp, #4]
	movw	r3, #1038
	str	r3, [sp]
	mov	r3, r9
	bl	vlog
	mov	r0, r4
	bl	list_del
	bl	kfree
.L25:
	mov	r4, r7
	ldr	r7, [r7]
	b	.L24
.L33:
	.align	2
.L32:
	.word	.LANCHOR0+21
	.word	.LC1
	.word	.LC0
	.fnend
	.size	vrgn_release, .-vrgn_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_ioctl, %function
vrgn_ioctl:
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
	ldr	r2, .L38
	ldr	r3, [r3]
	movs	r0, #3
	ldr	r8, [r3, #160]
	strd	r4, r3, [sp, #8]
	movw	r3, #974
	str	r4, [sp, #4]
	str	r3, [sp]
	ldr	r3, .L38+4
	bl	vlog
	ldr	r1, [r8, #192]
	cmp	r1, r4
	bhi	.L35
	movw	r3, #977
	str	r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L38+4
	movs	r0, #3
	ldr	r2, .L38+8
	bl	vlog
.L36:
.L34:
	mvn	r0, #21
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L35:
	.syntax unified
@ 87 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/barrier.h" 1
	cmp	r4, r1
	sbc	r1, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ands	r4, r4, r1
	ldr	r1, [r8, #188]
	movs	r3, #20
	mla	r1, r3, r4, r1
	cbnz	r1, .L37
	mov	r3, #984
	ldr	r2, .L38+12
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L38+4
	movs	r0, #3
	bl	vlog
	b	.L34
.L37:
	mov	r3, r7
	mov	r2, r6
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, lr}
	b	mpi_ioctl_kernel
.L39:
	.align	2
.L38:
	.word	.LC2
	.word	.LANCHOR0+34
	.word	.LC3
	.word	.LC4
	.fnend
	.size	vrgn_ioctl, .-vrgn_ioctl
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_read, %function
vrgn_read:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #36
	sub	sp, sp, #36
	ldr	r4, [r0, #128]
	mov	fp, r1
	str	r2, [sp, #28]
	ldr	r3, [r4], #4
	mov	r0, r4
	ldr	r7, [r3, #160]
	bl	mpi_fs_buf_read
	adds	r3, r0, #1
	mov	r5, r0
	bne	.L41
	ldr	r3, .L73
	mov	r0, r4
	ldr	r2, .L73+4
	ldr	r1, .L73+8
	str	r3, [sp]
	ldr	r3, .L73+12
	ldr	r2, [r2]
	bl	mpi_fs_buf_add
	ldr	r1, .L73+16
	mov	r0, r4
	ldr	r6, .L73+20
	bl	mpi_fs_buf_add
	ldr	r1, .L73+24
	mov	r0, r4
	ldr	r8, .L73+52
	bl	mpi_fs_buf_add
	ldr	r5, [r7, #196]
	add	r9, r6, #88
.L42:
	add	r3, r7, #196
	cmp	r5, r3
	beq	.L51
	ldr	r3, [r5, #24]
	str	r3, [sp, #24]
	ldr	r3, [r5, #32]
	cbz	r3, .L43
	cmp	r3, #1
	beq	.L43
	cmp	r3, #2
	beq	.L43
	cmp	r3, #3
	beq	.L43
	cmp	r3, #4
	bne	.L65
.L43:
	add	r3, r6, r3, lsl #3
	ldr	r3, [r3, #52]
.L44:
	ldrd	r10, ip, [r5, #40]
	movs	r1, #0
	ldr	r0, [r5, #36]
.L47:
	ldr	r2, [r9, r1, lsl #3]
	cmp	r0, r2
	bne	.L45
	add	r1, r6, r1, lsl #3
	ldr	lr, [r1, #92]
.L46:
	ldr	r0, [r5, #1752]
	cmp	r0, #0
	beq	.L66
	ldr	r1, [r0]
	ldr	r0, [r0, #16]
.L48:
	strd	r1, r0, [sp, #12]
	mov	r0, r4
	strd	ip, lr, [sp, #4]
	str	r10, [sp]
	ldr	r2, [sp, #24]
	ldr	r1, .L73+28
	bl	mpi_fs_buf_add
	cbz	r0, .L49
.L51:
	ldr	r7, .L73+32
	mov	r0, r4
	ldr	r1, .L73+36
	bl	mpi_fs_buf_add
	ldr	r1, .L73+40
	mov	r0, r4
	ldr	r9, .L73+56
	bl	mpi_fs_buf_add
	ldr	r5, [r7]
	ldr	r10, .L73+60
	subs	r5, r5, #36
.L50:
	add	r3, r5, #36
	cmp	r3, r7
	beq	.L59
	ldr	r3, [r5, #44]
	mov	r1, r9
	ldr	r2, [r5]
	mov	r0, r4
	ldr	r3, [r3, #44]
	bl	mpi_fs_buf_add
	cbz	r0, .L53
.L59:
	ldr	r1, .L73+44
	mov	r0, r4
	bl	mpi_fs_buf_add
	mov	r1, fp
	mov	r0, r4
	ldr	r2, [sp, #28]
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	mpi_fs_buf_read
.L65:
	mov	r3, r8
	b	.L44
.L45:
	adds	r1, r1, #1
	cmp	r1, #13
	bne	.L47
	mov	lr, r8
	b	.L46
.L66:
	mov	r1, #-1
	mov	r0, r1
	b	.L48
.L49:
	ldr	r2, [r5, #28]
	mov	r0, r4
	ldr	r1, .L73+48
	bl	mpi_fs_buf_add
	cmp	r0, #0
	bne	.L51
	ldr	r1, .L73+44
	mov	r0, r4
	bl	mpi_fs_buf_add
	ldr	r5, [r5]
	b	.L42
.L53:
	ldr	r3, [r5, #104]
	cbnz	r3, .L54
.L56:
	ldr	r1, .L73+44
	mov	r0, r4
	bl	mpi_fs_buf_add
	ldr	r5, [r5, #36]
	subs	r5, r5, #36
	b	.L50
.L54:
	mov	r8, r5
	ldr	r6, [r8, #84]!
.L72:
	subs	r6, r6, #4
	adds	r3, r6, #4
	cmp	r3, r8
	beq	.L56
	ldr	r2, [r6, #12]
	mov	r1, r10
	ldr	r3, [r6, #32]
	mov	r0, r4
	str	r3, [sp]
	ldr	r3, [r6, #28]
	ldr	r2, [r2, #24]
	bl	mpi_fs_buf_add
	cmp	r0, #0
	bne	.L56
	ldr	r6, [r6, #4]
	b	.L72
.L41:
	cbnz	r0, .L40
	mov	r0, r4
	bl	mpi_fs_buf_reset
.L40:
	mov	r0, r5
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L74:
	.align	2
.L73:
	.word	.LC8
	.word	g_vrgn_dev
	.word	.LC7
	.word	.LC6
	.word	.LC9
	.word	.LANCHOR0
	.word	.LC10
	.word	.LC11
	.word	mpi_node_list
	.word	.LC12
	.word	.LC13
	.word	.LC15
	.word	.LC14
	.word	.LC5
	.word	.LC16
	.word	.LC17
	.fnend
	.size	vrgn_read, .-vrgn_read
	.align	1
	.global	vrgn_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_version, %function
vrgn_version:
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
	beq	.L76
	mov	r3, #672
	ldr	r2, .L80
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L80+4
	movs	r0, #3
	bl	vlog
	mov	r0, #-1
.L75:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L76:
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
	b	.L75
.L81:
	.align	2
.L80:
	.word	.LC18
	.word	.LANCHOR0+192
	.fnend
	.size	vrgn_version, .-vrgn_version
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_open, %function
vrgn_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L87
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r7, r1
	ldr	r0, [r3, #20]
	mov	r1, #3520
	bl	kmem_cache_alloc
	mov	r4, r0
	cbnz	r0, .L83
	mov	r3, #1008
	ldr	r2, .L87+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L87+8
	movs	r0, #3
	bl	vlog
.L86:
	mov	r5, #-1
.L82:
	mov	r0, r5
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L83:
	mov	r1, #5120
	adds	r0, r0, #4
	bl	mpi_fs_buf_init
	mov	r5, r0
	cbz	r0, .L85
	mov	r0, r4
	bl	kfree
	b	.L86
.L85:
	ldr	r6, .L87+12
	mov	r0, r6
	bl	mpi_obj_ref
	str	r6, [r4]
	str	r4, [r7, #128]
	b	.L82
.L88:
	.align	2
.L87:
	.word	kmalloc_caches
	.word	.LC19
	.word	.LANCHOR0+205
	.word	g_vrgn_dev+4
	.fnend
	.size	vrgn_open, .-vrgn_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	filter_callback, %function
filter_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r6, r0
	.pad #24
	sub	sp, sp, #24
	mov	r7, r1
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r8, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L102
	mov	r1, #3520
	ldr	r0, [r3, #16]
	bl	kmem_cache_alloc
	mov	r4, r0
	cbz	r0, .L89
	cbz	r7, .L91
	mov	r0, r7
	bl	mpi_obj_ref
.L91:
	str	r7, [r4, #12]
	cbz	r6, .L92
	mov	r0, r6
	bl	mpi_obj_ref
.L92:
	ldr	r5, .L102+4
	movs	r1, #5
	str	r6, [r4, #8]
	movs	r0, #3
	add	r2, r5, #204
	ldr	r3, [r5, #208]
	str	r4, [r5, #208]
	strd	r2, r3, [r4]
	str	r4, [r3]
	ldr	r3, [r7]
	ldr	r2, .L102+8
	str	r3, [sp, #16]
	ldr	r3, [r6]
	strd	r4, r3, [sp, #8]
	ldr	r3, [r5, #48]
	str	r3, [sp, #4]
	mov	r3, #344
	str	r3, [sp]
	ldr	r3, .L102+12
	bl	vlog
	add	r0, r5, #168
	bl	up
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r8	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
.L89:
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L103:
	.align	2
.L102:
	.word	kmalloc_caches
	.word	g_vrgn_dev
	.word	.LC20
	.word	.LANCHOR0+215
	.fnend
	.size	filter_callback, .-filter_callback
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_thread_fn, %function
vrgn_thread_fn:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #92
	sub	sp, sp, #92
	ldr	fp, .L186+20
	mov	r9, r0
.L105:
	bl	kthread_should_stop
	cbz	r0, .L136
	movs	r0, #0
	add	sp, sp, #92
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L136:
	ldr	r3, [r9, #184]
	cbz	r3, .L106
	movs	r0, #1
	bl	msleep
	b	.L105
.L106:
	add	r0, r9, #168
	bl	down
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r1, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	mov	r3, r9
	ldr	r6, [r3, #204]!
	cmp	r6, r3
	beq	.L108
	cbnz	r6, .L109
.L108:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	b	.L105
.L109:
	mov	r0, r6
	bl	list_del
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r1	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldrd	r8, r4, [r6, #8]
	movs	r1, #5
	ldr	r2, .L186
	movs	r0, #3
	ldr	r3, [r4]
	str	r3, [sp, #16]
	ldr	r3, [r8]
	strd	r6, r3, [sp, #8]
	ldr	r3, [r9, #48]
	str	r3, [sp, #4]
	movw	r3, #275
	str	r3, [sp]
	ldr	r3, .L186+4
	bl	vlog
	ldr	r3, [r8, #104]
	cmp	r3, #0
	beq	.L110
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	str	r3, [sp, #52]
	ldr	r5, [r8, #92]
	ldr	r10, .L186+24
	subs	r5, r5, #4
.L111:
	adds	r3, r5, #4
	add	r2, r8, #92
	cmp	r3, r2
	bne	.L125
	ldr	r3, [sp, #52]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	cmp	r8, #0
	beq	.L130
	ldr	r3, [r8, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L127
.L130:
	mov	r0, r8
	bl	mpi_obj_unref
	mov	r0, r4
.L184:
	bl	mpi_obj_unref
	mov	r0, r6
	bl	kfree
	b	.L105
.L125:
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #1752]
	cmp	r3, #0
	beq	.L112
	mov	r1, #3520
	ldr	r0, [fp, #20]
	bl	kmem_cache_alloc
	mov	r1, #3520
	str	r0, [sp, #68]
	ldr	r0, [fp, #20]
	bl	kmem_cache_alloc
	mov	r1, #3520
	str	r0, [sp, #72]
	ldr	r0, [fp, #20]
	bl	kmem_cache_alloc
	ldr	r3, [sp, #68]
	str	r0, [sp, #76]
	cmp	r3, #0
	beq	.L119
	ldr	r2, [sp, #72]
	cmp	r2, #0
	beq	.L119
	cmp	r0, #0
	beq	.L119
	cmp	r4, #0
	beq	.L114
	ldr	r2, [r5, #12]
	ldr	r7, [r2, #1752]
	cmp	r7, #0
	beq	.L114
	ldr	r2, [r2, #1756]
	ldr	r1, [r4, #448]
	ldr	r0, [r7, #448]
	ldr	r2, [r2, #56]
	str	r1, [sp, #56]
	strd	r0, r1, [sp, #60]
	str	r2, [sp, #84]
	ldr	r2, [r5, #28]
	str	r2, [r3]
	ldr	r3, [sp, #68]
	ldr	r2, [r5, #32]
	str	r2, [r3, #4]
	ldr	r2, [r7, #620]
	ldr	r3, [sp, #68]
	str	r2, [r3, #8]
	ldr	r2, [r7, #624]
	ldr	r3, [sp, #68]
	str	r2, [r3, #12]
	ldr	r3, [sp, #68]
	ldr	r2, [r4, #628]
	str	r2, [r3, #20]
	ldr	r2, [r4, #632]
	str	r2, [r3, #24]
	ldr	r0, [r4, #640]
	bl	pixel_format_rk_to_rga
	ldr	r3, [sp, #68]
	str	r0, [r3, #16]
	ldr	r0, [r4, #648]
	bl	compress_mode_rk_to_rga
	movs	r1, #5
	ldr	r3, [sp, #68]
	str	r0, [r3, #28]
	str	r0, [sp, #44]
	movs	r0, #3
	ldr	r2, [r3, #16]
	str	r2, [sp, #40]
	ldr	r2, [r3, #24]
	str	r2, [sp, #36]
	ldr	r2, [r3, #20]
	str	r2, [sp, #32]
	ldr	r2, [r3, #12]
	str	r2, [sp, #28]
	ldr	r2, [r3, #8]
	str	r2, [sp, #24]
	ldr	r2, [r3, #4]
	str	r2, [sp, #20]
	ldr	r3, [r3]
	ldr	r2, .L186+8
	str	r3, [sp, #16]
	ldr	r3, [sp, #84]
	str	r3, [sp, #12]
	ldr	r3, [r4, #448]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movs	r3, #137
	str	r3, [sp]
	mov	r3, r10
	bl	vlog
	movs	r2, #0
	ldr	r3, [sp, #72]
	str	r2, [r3]
	ldr	r3, [sp, #72]
	str	r2, [r3, #4]
	ldr	r2, [r7, #620]
	ldr	r3, [sp, #72]
	str	r2, [r3, #8]
	ldr	r2, [r7, #624]
	ldr	r3, [sp, #72]
	str	r2, [r3, #12]
	ldr	r2, [r7, #628]
	ldr	r3, [sp, #72]
	str	r2, [r3, #20]
	ldr	r2, [r7, #632]
	str	r2, [r3, #24]
	ldr	r0, [r7, #640]
	bl	pixel_format_rk_to_rga
	movs	r2, #1
	movs	r1, #5
	ldr	r3, [sp, #72]
	str	r0, [r3, #16]
	movs	r0, #3
	ldr	r3, [sp, #72]
	str	r2, [r3, #28]
	str	r2, [sp, #44]
	ldr	r2, [r3, #16]
	str	r2, [sp, #40]
	ldr	r2, [r3, #24]
	str	r2, [sp, #36]
	ldr	r2, [r3, #20]
	str	r2, [sp, #32]
	ldr	r2, [r3, #12]
	str	r2, [sp, #28]
	ldr	r2, [r3, #8]
	str	r2, [sp, #24]
	ldr	r2, [r3, #4]
	str	r2, [sp, #20]
	ldr	r3, [r3]
	ldr	r2, .L186+12
	str	r3, [sp, #16]
	ldr	r3, [sp, #84]
	str	r3, [sp, #12]
	ldr	r3, [r7, #448]
	str	r3, [sp, #8]
	ldr	r3, [r7]
	str	r3, [sp, #4]
	movs	r3, #154
	str	r3, [sp]
	mov	r3, r10
	bl	vlog
	ldr	r2, [r5, #28]
	ldr	r3, [sp, #76]
	str	r2, [r3]
	ldr	r3, [sp, #76]
	ldr	r2, [r5, #32]
	str	r2, [r3, #4]
	ldr	r2, [r7, #620]
	ldr	r3, [sp, #76]
	str	r2, [r3, #8]
	ldr	r2, [r7, #624]
	ldr	r3, [sp, #76]
	str	r2, [r3, #12]
	ldr	r3, [sp, #76]
	ldr	r2, [r4, #628]
	str	r2, [r3, #20]
	ldr	r3, [sp, #76]
	ldr	r2, [r4, #632]
	str	r2, [r3, #24]
	ldr	r0, [r4, #640]
	bl	pixel_format_rk_to_rga
	ldr	r3, [sp, #76]
	str	r0, [r3, #16]
	ldr	r0, [r4, #648]
	bl	compress_mode_rk_to_rga
	movs	r1, #5
	ldr	r3, [sp, #76]
	str	r0, [r3, #28]
	str	r0, [sp, #44]
	movs	r0, #3
	ldr	r2, [r3, #16]
	str	r2, [sp, #40]
	ldr	r2, [r3, #24]
	str	r2, [sp, #36]
	ldr	r2, [r3, #20]
	str	r2, [sp, #32]
	ldr	r2, [r3, #12]
	str	r2, [sp, #28]
	ldr	r2, [r3, #8]
	str	r2, [sp, #24]
	ldr	r2, [r3, #4]
	str	r2, [sp, #20]
	ldr	r3, [r3]
	ldr	r2, .L186+16
	str	r3, [sp, #16]
	ldr	r3, [sp, #84]
	str	r3, [sp, #12]
	ldr	r3, [r4, #448]
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movs	r3, #171
	str	r3, [sp]
	mov	r3, r10
	bl	vlog
	add	r0, sp, #56
	bl	rga_mpi_commit
	mov	r7, r0
	cmp	r0, #0
	beq	.L115
	ldr	r0, [sp, #68]
	cbz	r0, .L123
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #68]
.L123:
	ldr	r0, [sp, #72]
	cbz	r0, .L124
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #72]
	b	.L124
.L187:
	.align	2
.L186:
	.word	.LC20
	.word	.LANCHOR0+231
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	kmalloc_caches
	.word	.LANCHOR0+246
.L114:
	mov	r0, r3
	movs	r7, #0
	bl	kfree
	ldr	r0, [sp, #72]
	str	r7, [sp, #68]
	cbz	r0, .L124
	bl	kfree
	str	r7, [sp, #72]
.L124:
	ldr	r0, [sp, #76]
	cbz	r0, .L119
	bl	kfree
.L119:
	ldr	r3, [r5, #12]
	movs	r0, #3
	ldr	r1, [r4, #448]
	ldr	r2, [r3, #1752]
	str	r1, [sp, #20]
	ldr	r1, [r4]
	str	r1, [sp, #16]
	ldr	r1, [r2, #448]
	str	r1, [sp, #12]
	movs	r1, #2
	ldr	r2, [r2]
	str	r2, [sp, #8]
	ldr	r3, [r3, #1756]
	ldr	r2, .L188
	ldr	r3, [r3]
	str	r3, [sp, #4]
	movw	r3, #289
	str	r3, [sp]
	ldr	r3, .L188+4
	bl	vlog
.L110:
	ldr	r0, [r6, #8]
	cbz	r0, .L135
	ldr	r3, [r0, #12]
	and	r3, r3, #6
	cmp	r3, #2
	beq	.L132
.L135:
	ldr	r0, [r6, #8]
	bl	mpi_obj_unref
	ldr	r0, [r6, #12]
	b	.L184
.L115:
	ldr	r2, [r5, #12]
	movs	r1, #5
	movs	r0, #3
	ldr	r3, [r2, #1752]
	ldr	r2, [r2, #1756]
	ldr	r2, [r2, #56]
	str	r2, [sp, #20]
	ldr	r2, [r4, #448]
	str	r2, [sp, #16]
	ldr	r2, [r4]
	str	r2, [sp, #12]
	ldr	r2, [r3, #448]
	str	r2, [sp, #8]
	ldr	r3, [r3]
	ldr	r2, .L188+8
	str	r3, [sp, #4]
	movs	r3, #208
	str	r3, [sp]
	ldr	r3, .L188+12
	bl	vlog
	ldr	r0, [sp, #68]
	cbz	r0, .L120
	bl	kfree
	str	r7, [sp, #68]
.L120:
	ldr	r0, [sp, #72]
	cbz	r0, .L121
	bl	kfree
	movs	r3, #0
	str	r3, [sp, #72]
.L121:
	ldr	r0, [sp, #76]
	cbz	r0, .L112
	bl	kfree
.L112:
	ldr	r5, [r5, #4]
	subs	r5, r5, #4
	b	.L111
.L127:
	ldr	r3, [r6, #8]
	movs	r2, #1
	mov	r1, r4
	mov	r0, r8
	ldr	r3, [r3, #100]
	cbz	r3, .L129
	bl	mpi_node_buffer_in
	b	.L130
.L129:
	bl	mpi_node_buffer_out
	b	.L130
.L132:
	ldr	r3, [r0, #100]
	movs	r2, #1
	ldr	r1, [r6, #12]
	cbz	r3, .L134
	bl	mpi_node_buffer_in
	b	.L135
.L134:
	bl	mpi_node_buffer_out
	b	.L135
.L189:
	.align	2
.L188:
	.word	.LC25
	.word	.LANCHOR0+231
	.word	.LC24
	.word	.LANCHOR0+270
	.fnend
	.size	vrgn_thread_fn, .-vrgn_thread_fn
	.align	1
	.global	list_rgn_find_by_handle
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	list_rgn_find_by_handle, %function
list_rgn_find_by_handle:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L194
	ldr	r3, [r2, #196]!
.L191:
	cmp	r3, r2
	bne	.L193
	movs	r3, #0
.L190:
	mov	r0, r3
	bx	lr
.L193:
	ldr	r1, [r3, #24]
	cmp	r1, r0
	beq	.L190
	ldr	r3, [r3]
	b	.L191
.L195:
	.align	2
.L194:
	.word	g_vrgn_dev
	.fnend
	.size	list_rgn_find_by_handle, .-list_rgn_find_by_handle
	.align	1
	.global	vrgn_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_create, %function
vrgn_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	.pad #12
	mov	r5, r1
	ldr	r0, [r1]	@ unaligned
	bl	list_rgn_find_by_handle
	ldr	r2, [r5, #1052]	@ unaligned
	movw	r3, #1148
	cmp	r2, r3
	mov	r6, r0
	beq	.L197
	movw	r3, #697
	ldr	r2, .L211
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L211+4
	movs	r0, #3
	bl	vlog
.L206:
	mov	r0, #-1
	b	.L196
.L197:
	cbz	r0, .L199
	mov	r2, #1048
	adds	r1, r5, #4
	adds	r0, r0, #32
	movs	r4, #0
	bl	memcpy
	str	r4, [r6, #1752]
	str	r4, [r6, #28]
	ldr	r3, [r5]	@ unaligned
	ldr	r2, .L211+8
	str	r3, [sp, #4]
	mov	r3, #704
	str	r3, [sp]
	ldr	r3, .L211+4
.L210:
	movs	r1, #4
	movs	r0, #3
	bl	vlog
	movs	r0, #0
.L196:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L199:
	ldr	r3, .L211+12
	mov	r1, #3520
	ldr	r7, [r5, #1144]	@ unaligned
	ldr	r0, [r3, #44]
	bl	kmem_cache_alloc
	mov	r4, r0
	cmp	r0, #0
	beq	.L206
	ldr	r3, [r5, #4]	@ unaligned
	cmp	r3, #1
	bne	.L200
	and	r7, r7, #63
	mov	r1, r6
	movs	r0, #18
	mov	r2, r7
	bl	vsys_get_node_from_map
	cbnz	r0, .L201
	mov	r0, r4
	bl	kfree
	mov	r3, r7
	movw	r2, #714
	ldr	r1, .L211+4
	ldr	r0, .L211+16
	bl	printk
	b	.L206
.L201:
	str	r0, [r4, #1756]
.L200:
	ldr	r3, [r5]	@ unaligned
	str	r3, [r4, #24]
	ldr	r3, [r5, #20]	@ unaligned
	cmp	r3, #1
	bne	.L202
	movs	r3, #0
	ldr	r2, .L211+20
	str	r3, [r4, #12]
	add	r0, r4, #16
	ldr	r1, .L211+24
	bl	__init_swait_queue_head
	mov	r3, #1000
	str	r3, [r4, #8]
.L202:
	mov	r2, #1048
	adds	r1, r5, #4
	add	r0, r4, #32
	bl	memcpy
	movs	r3, #0
	str	r3, [r4, #1752]
	str	r3, [r4, #28]
	ldr	r3, [r5, #4]	@ unaligned
	cmp	r3, #1
	bhi	.L204
	ldr	r0, [r5, #1140]	@ unaligned
	bl	get_mpi_buf
	cbz	r0, .L204
	ldr	r3, [r0, #444]
	str	r3, [r4, #1760]
	bl	mpi_obj_unref
.L204:
	ldr	r3, .L211+28
	adds	r3, r3, #196
	ldr	r2, [r3, #4]
	str	r4, [r3, #4]
	strd	r3, r2, [r4]
	str	r4, [r2]
	ldr	r3, [r5]	@ unaligned
	ldr	r2, .L211+32
	str	r3, [sp, #4]
	movw	r3, #739
	str	r3, [sp]
	ldr	r3, .L211+4
	b	.L210
.L212:
	.align	2
.L211:
	.word	.LC26
	.word	.LANCHOR0+280
	.word	.LC27
	.word	kmalloc_caches
	.word	.LC28
	.word	.LANCHOR1
	.word	.LC29
	.word	g_vrgn_dev
	.word	.LC30
	.fnend
	.size	vrgn_create, .-vrgn_create
	.align	1
	.global	vrgn_destroy
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_destroy, %function
vrgn_destroy:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r6, r1
	ldr	r5, [r1]	@ unaligned
	mov	r0, r5
	bl	list_rgn_find_by_handle
	ldr	r2, [r6, #1052]	@ unaligned
	movw	r3, #1148
	cmp	r2, r3
	mov	r4, r0
	beq	.L214
	movw	r3, #857
	ldr	r2, .L217
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L217+4
	movs	r0, #3
	bl	vlog
	mov	r0, #-1
.L213:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L214:
	cmp	r0, #0
	beq	.L213
	ldr	r3, [r0, #24]
	movs	r1, #4
	ldr	r2, .L217+8
	movs	r0, #3
	strd	r5, r3, [sp, #4]
	movw	r3, #863
	str	r3, [sp]
	ldr	r3, .L217+4
	bl	vlog
	mov	r0, r4
	bl	list_del
	bl	kfree
	movs	r0, #0
	b	.L213
.L218:
	.align	2
.L217:
	.word	.LC26
	.word	.LANCHOR0+292
	.word	.LC31
	.fnend
	.size	vrgn_destroy, .-vrgn_destroy
	.align	1
	.global	vrgn_upate_canvas_sync
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_upate_canvas_sync, %function
vrgn_upate_canvas_sync:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, lr}
	.save {r4, lr}
	.pad #8
	mov	r4, r1
	ldr	r0, [r1, #2]	@ unaligned
	bl	list_rgn_find_by_handle
	ldr	r3, [r4, #6]	@ unaligned
	cmp	r3, #178
	beq	.L220
	movw	r3, #877
	ldr	r2, .L224
	str	r3, [sp]
	ldr	r3, .L224+4
.L223:
	movs	r1, #2
	movs	r0, #3
	bl	vlog
	mov	r0, #-1
.L219:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L220:
	cbnz	r0, .L222
	movw	r3, #882
	ldr	r2, .L224+8
	str	r3, [sp]
	ldr	r3, .L224+4
	b	.L223
.L222:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r3, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r2, #0
	str	r2, [r0, #12]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r3	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, .L224+12
	adds	r0, r0, #12
	ldr	r2, [r0, #-4]
	umull	r2, r3, r2, r3
	lsrs	r1, r3, #3
	bl	wait_for_completion_interruptible_timeout
	cmp	r0, #0
	ite	gt
	movgt	r0, #0
	movle	r0, #1
	rsbs	r0, r0, #0
	b	.L219
.L225:
	.align	2
.L224:
	.word	.LC32
	.word	.LANCHOR0+305
	.word	.LC33
	.word	-858993459
	.fnend
	.size	vrgn_upate_canvas_sync, .-vrgn_upate_canvas_sync
	.align	1
	.global	vrgn_info_sync
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_info_sync, %function
vrgn_info_sync:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r4, r1
	ldr	r0, [r1]	@ unaligned
	bl	list_rgn_find_by_handle
	ldr	r2, [r4, #1052]	@ unaligned
	movw	r3, #6436
	cmp	r2, r3
	mov	r5, r0
	beq	.L227
	movw	r3, #901
	ldr	r2, .L235
	str	r3, [sp]
	ldr	r3, .L235+4
.L234:
	movs	r1, #2
	movs	r0, #3
	bl	vlog
	mov	r0, #-1
	b	.L226
.L227:
	cbz	r0, .L229
	add	r1, r0, #32
	mov	r2, #1048
	adds	r0, r4, #4
	bl	memcpy
	ldr	r3, [r5, #28]
	cbz	r3, .L230
	movs	r2, #72
	add	r1, r5, #1080
	muls	r2, r3, r2
	add	r0, r4, #1056
	bl	memcpy
	ldr	r3, [r5, #28]
	movs	r2, #12
	add	r1, r5, #1656
	add	r0, r4, #5664
	muls	r2, r3, r2
	bl	memcpy
.L230:
	ldr	r3, [r5, #28]
	add	r4, r4, #6432
	movs	r0, #0
	str	r3, [r4]	@ unaligned
.L226:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L229:
	movw	r3, #914
	ldr	r2, .L235+8
	str	r3, [sp]
	ldr	r3, .L235+4
	b	.L234
.L236:
	.align	2
.L235:
	.word	.LC34
	.word	.LANCHOR0+328
	.word	.LC35
	.fnend
	.size	vrgn_info_sync, .-vrgn_info_sync
	.align	1
	.global	vrgn_request_buffer
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_request_buffer, %function
vrgn_request_buffer:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r4, r1
	ldr	r0, [r1]	@ unaligned
	bl	list_rgn_find_by_handle
	ldr	r3, [r4, #12]	@ unaligned
	cmp	r3, #96
	mov	r5, r0
	beq	.L238
	mov	r3, #924
	ldr	r2, .L247
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L247+4
	movs	r0, #3
	bl	vlog
.L243:
	mov	r0, #-1
	b	.L237
.L238:
	cmp	r0, #0
	beq	.L243
	ldr	r3, [r0, #1760]
	cmp	r3, #0
	beq	.L243
	ldr	r0, [r0, #1752]
	cbz	r0, .L240
	bl	mpi_obj_unref
.L240:
	movs	r1, #0
	ldr	r0, [r5, #1760]
	bl	mpi_buf_pool_request_buf
	mov	r6, r0
	cbnz	r0, .L241
	ldr	r3, [r5, #1760]
	movs	r1, #2
	ldr	r2, .L247+8
	movs	r0, #3
	ldr	r3, [r3]
	str	r3, [sp, #4]
	movw	r3, #934
	str	r3, [sp]
	ldr	r3, .L247+4
	bl	vlog
	b	.L243
.L241:
	movs	r1, #0
	ldr	r0, [r0, #448]
	bl	dma_buf_fd
	movs	r2, #80
	add	r1, r6, #616
	str	r0, [r4, #4]	@ unaligned
	add	r0, r4, #16
	ldr	r3, [r6, #596]
	str	r3, [r4, #8]	@ unaligned
	bl	memcpy
	movs	r0, #0
.L237:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L248:
	.align	2
.L247:
	.word	.LC36
	.word	.LANCHOR0+343
	.word	.LC37
	.fnend
	.size	vrgn_request_buffer, .-vrgn_request_buffer
	.global	__aeabi_uidiv
	.align	1
	.global	set_osd_invert_buffer
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	set_osd_invert_buffer, %function
set_osd_invert_buffer:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r5, r0
	ldr	r7, [r0, #12]
	.pad #28
	sub	sp, sp, #28
	ldrd	r1, r6, [r0, #72]
	ldr	r0, [r7, #40]
	subs	r3, r1, #1
	add	r3, r3, r0
	rsbs	r0, r1, #0
	ands	r0, r0, r3
	bl	__aeabi_uidiv
	subs	r3, r6, #1
	mov	r1, r6
	adds	r0, r0, #7
	asrs	r4, r0, #3
	ldr	r0, [r7, #44]
	add	r3, r3, r0
	rsbs	r0, r6, #0
	ands	r0, r0, r3
	bl	__aeabi_uidiv
	muls	r4, r0, r4
	ldr	r0, .L258
	bl	mpi_allocator_find_by_name
	movs	r1, #1
	mov	r2, r4
	bl	mpi_allocator_alloc
	mov	r6, r0
	cbz	r0, .L250
	movs	r1, #0
	bl	mpi_buf_create
	mov	r4, r0
	cbz	r0, .L250
	ldr	r3, .L258+4
	str	r3, [r0, #4]
	mov	r0, r6
	bl	dma_buf_put
	str	r4, [r5, #16]
	ldr	r3, [r4, #448]
	movs	r1, #4
	movs	r0, #3
	ldr	r2, [r3, #4]
	ldr	r2, [r2, #28]
	strd	r3, r2, [sp, #12]
	ldr	r3, [r4, #596]
	ldr	r2, .L258+8
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	mov	r3, #436
	str	r3, [sp]
	ldr	r3, .L258+12
	bl	vlog
.L249:
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L250:
	mov	r3, #438
	ldr	r2, .L258+16
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L258+12
	movs	r0, #3
	bl	vlog
	b	.L249
.L259:
	.align	2
.L258:
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LANCHOR0+363
	.word	.LC41
	.fnend
	.size	set_osd_invert_buffer, .-set_osd_invert_buffer
	.align	1
	.global	get_list_node_rgn_from_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_list_node_rgn_from_node, %function
get_list_node_rgn_from_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	mov	r2, r1
	push	{r4, lr}
	.save {r4, lr}
	ldr	r3, [r2, #84]!
.L266:
	subs	r3, r3, #4
	adds	r4, r3, #4
	cmp	r4, r2
	bne	.L263
	ldr	r3, [r1, #92]!
.L267:
	subs	r3, r3, #4
	adds	r2, r3, #4
	cmp	r2, r1
	bne	.L265
	movs	r3, #0
.L260:
	mov	r0, r3
	pop	{r4, pc}
.L263:
	ldr	r4, [r3, #12]
	ldr	r4, [r4, #24]
	cmp	r4, r0
	beq	.L260
	ldr	r3, [r3, #4]
	b	.L266
.L265:
	ldr	r2, [r3, #12]
	ldr	r2, [r2, #24]
	cmp	r0, r2
	beq	.L260
	ldr	r3, [r3, #4]
	b	.L267
	.fnend
	.size	get_list_node_rgn_from_node, .-get_list_node_rgn_from_node
	.align	1
	.global	rgn_attach_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rgn_attach_node, %function
rgn_attach_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r6, r1
	ldr	r2, [r1, #1136]	@ unaligned
	mov	r5, r0
	ldr	r1, [r1, #1132]	@ unaligned
	ldr	r0, [r6, #1128]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r4, r0
	cbnz	r0, .L269
	ldr	r3, [r6, #1136]	@ unaligned
	movs	r1, #2
	ldr	r2, .L287
	movs	r0, #3
	str	r3, [sp, #12]
	ldr	r3, [r6, #1132]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r6, #1128]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #512
	str	r3, [sp]
	ldr	r3, .L287+4
	bl	vlog
	mov	r0, #-1
.L268:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L269:
	ldr	r3, [r4, #104]
	adds	r0, r0, #84
	cbnz	r3, .L271
	add	r3, r4, #92
	str	r0, [r4, #84]
	str	r0, [r4, #88]
	str	r3, [r4, #92]
	str	r3, [r4, #96]
.L271:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r10, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #32]
	cmp	r3, #0
	bne	.L272
	ldr	r3, [r4, #84]
	ldr	r2, [r3], #-4
	subs	r2, r2, #4
.L273:
	add	lr, r3, #4
	cmp	lr, r0
	bne	.L276
	adds	r3, r5, #4
	ldr	r2, [r4, #88]
	str	r3, [r4, #88]
.L286:
	strd	r0, r2, [r5, #4]
	str	r3, [r2]
	b	.L275
.L276:
	ldr	r1, [r5, #44]
	ldr	r7, [r3, #44]
	cmp	r7, r1
	bls	.L274
.L285:
	ldr	r1, [r3, #8]
	adds	r2, r5, #4
	str	r2, [r3, #8]
	strd	lr, r1, [r5, #4]
	str	r2, [r1]
.L275:
	ldr	r3, [r5, #12]
	ldr	r9, [r3, #32]
	cmp	r9, #1
	bne	.L280
	ldr	r8, [r4, #112]
	cmp	r8, #0
	bne	.L281
	ldr	r3, [r6, #1128]	@ unaligned
	mov	r1, r8
	mov	r0, r4
	ldr	r6, .L287+8
	cmp	r3, #4
	bne	.L282
	bl	mpi_node_add_in_filter
	str	r6, [r4, #332]
	str	r9, [r4, #100]
.L281:
	ldr	r3, [r4, #112]
	adds	r3, r3, #1
	str	r3, [r4, #112]
.L280:
	ldr	r3, [r5, #12]
	ldr	r3, [r3, #48]
	cmp	r3, #1
	bne	.L283
	ldr	r2, [r4, #116]
	ldr	r3, [r4, #108]
	cbnz	r2, .L284
	cbnz	r3, .L284
	ldr	r2, .L287+12
	str	r2, [r4, #116]
.L284:
	adds	r3, r3, #1
	str	r3, [r4, #108]
.L283:
	ldr	r3, [r4, #104]
	adds	r3, r3, #1
	str	r3, [r4, #104]
	ldr	r3, .L287+16
	str	r3, [r4, #120]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r10	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	b	.L268
.L274:
	ldr	r1, [r2, #4]
	mov	r3, r2
	subs	r2, r1, #4
	b	.L273
.L272:
	mov	r0, r4
	ldr	r3, [r0, #92]!
	ldr	r2, [r3], #-4
	subs	r2, r2, #4
.L277:
	add	lr, r3, #4
	cmp	lr, r0
	bne	.L279
	adds	r3, r5, #4
	ldr	r2, [r4, #96]
	str	r3, [r4, #96]
	b	.L286
.L279:
	ldr	r1, [r5, #44]
	ldr	r7, [r3, #44]
	cmp	r7, r1
	bhi	.L285
	ldr	r1, [r2, #4]
	mov	r3, r2
	subs	r2, r1, #4
	b	.L277
.L282:
	bl	mpi_node_add_out_filter
	str	r6, [r4, #344]
	str	r8, [r4, #100]
	b	.L281
.L288:
	.align	2
.L287:
	.word	.LC42
	.word	.LANCHOR0+385
	.word	filter_callback
	.word	osd_sync_callback
	.word	node_delete_callback
	.fnend
	.size	rgn_attach_node, .-rgn_attach_node
	.align	1
	.global	vrgn_attach_chn
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_attach_chn, %function
vrgn_attach_chn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #28
	sub	sp, sp, #28
	ldr	r0, [r1]	@ unaligned
	mov	r4, r1
	bl	list_rgn_find_by_handle
	mov	r5, r0
	cbz	r0, .L305
	ldr	r3, [r0, #28]
	cbz	r3, .L291
	ldr	r3, [r0, #48]
	cmp	r3, #1
	bne	.L291
	ldr	r3, [r0, #24]
	movs	r1, #2
	ldr	r2, .L307
	movs	r0, #3
	str	r3, [sp, #4]
	movw	r3, #757
	str	r3, [sp]
	ldr	r3, .L307+4
	bl	vlog
.L305:
	mov	r6, #-1
	b	.L289
.L291:
	ldr	r2, [r4, #1052]	@ unaligned
	movw	r3, #1148
	cmp	r2, r3
	beq	.L292
	movw	r3, #762
	ldr	r2, .L307+8
	str	r3, [sp]
	ldr	r3, .L307+4
.L306:
	movs	r1, #2
	movs	r0, #3
	bl	vlog
	b	.L305
.L292:
	ldr	r3, .L307+12
	movs	r7, #0
	mov	r9, r3
.L296:
	ldr	r1, [r3, #212]
	cmp	r1, #0
	bne	.L293
	movs	r2, #92
	add	r10, r4, #1056
	mul	r8, r2, r7
	add	r6, r8, #212
	add	fp, r9, r8
	add	r6, r6, r9
	mov	r0, r6
	bl	memset
	add	r0, r8, #232
	movs	r3, #1
	str	r5, [fp, #224]
	str	r3, [fp, #212]
	movs	r2, #72
	mov	r1, r10
	add	r0, r0, r9
	bl	memcpy
	ldr	r3, [fp, #300]
	cbz	r3, .L295
	mov	r0, r6
	bl	set_osd_invert_buffer
.L295:
	mov	r0, r6
	mov	r1, r4
	bl	rgn_attach_node
	mov	r6, r0
	cbz	r0, .L298
	ldr	r3, [r4, #1136]	@ unaligned
	movs	r1, #2
	ldr	r2, .L307+16
	movs	r0, #3
	str	r3, [sp, #16]
	ldr	r3, [r4, #1132]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #1128]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #786
	str	r3, [sp]
	ldr	r3, .L307+4
	bl	vlog
	movs	r3, #92
	mla	r7, r3, r7, r9
	movs	r3, #0
	str	r3, [r7, #212]
	b	.L305
.L293:
	adds	r7, r7, #1
	adds	r3, r3, #92
	cmp	r7, #64
	bne	.L296
	movw	r3, #774
	ldr	r2, .L307+20
	str	r3, [sp]
	ldr	r3, .L307+4
	b	.L306
.L298:
	ldr	r0, [r5, #28]
	movs	r2, #72
	mov	r1, r10
	mla	r0, r2, r0, r5
	add	r0, r0, #1080
	bl	memcpy
	ldr	r2, [r5, #28]
	movs	r3, #12
	muls	r3, r2, r3
	ldr	r2, [r4, #1128]	@ unaligned
	add	r3, r3, #1656
	adds	r0, r5, r3
	str	r2, [r5, r3]	@ unaligned
	ldr	r2, [r4, #1132]	@ unaligned
	str	r2, [r0, #4]	@ unaligned
	ldr	r2, [r4, #1136]	@ unaligned
	str	r2, [r0, #8]	@ unaligned
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #28]
	adds	r3, r3, #1
	str	r3, [r5, #28]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r4, #1136]	@ unaligned
	movs	r1, #4
	ldr	r2, .L307+24
	movs	r0, #3
	str	r3, [sp, #16]
	ldr	r3, [r4, #1132]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #1128]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #796
	str	r3, [sp]
	ldr	r3, .L307+4
	bl	vlog
.L289:
	mov	r0, r6
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L308:
	.align	2
.L307:
	.word	.LC43
	.word	.LANCHOR0+401
	.word	.LC26
	.word	g_vrgn_dev
	.word	.LC44
	.word	.LC46
	.word	.LC45
	.fnend
	.size	vrgn_attach_chn, .-vrgn_attach_chn
	.align	1
	.global	rgn_dettach_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rgn_dettach_node, %function
rgn_dettach_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r6, r0
	ldr	r2, [r0, #1136]	@ unaligned
	.pad #20
	sub	sp, sp, #20
	ldr	r1, [r0, #1132]	@ unaligned
	ldr	r0, [r0, #1128]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r5, r0
	cbnz	r0, .L310
	ldr	r3, [r6, #1136]	@ unaligned
	movs	r1, #2
	ldr	r2, .L329
	movs	r0, #3
	str	r3, [sp, #12]
	ldr	r3, [r6, #1132]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r6, #1128]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #567
	str	r3, [sp]
	ldr	r3, .L329+4
	bl	vlog
.L328:
	mov	r0, #-1
.L309:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L310:
	ldr	r3, [r0, #104]
	cbnz	r3, .L312
	movw	r3, #579
	ldr	r2, .L329+8
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L329+4
	movs	r0, #3
	bl	vlog
	b	.L328
.L312:
	mov	r1, r0
	ldr	r0, [r6]	@ unaligned
	bl	get_list_node_rgn_from_node
	mov	r4, r0
	cmp	r0, #0
	beq	.L309
	movw	r3, #585
	ldr	r2, .L329+12
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L329+4
	movs	r0, #3
	bl	vlog
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r7, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	adds	r0, r4, #4
	bl	list_del
	ldr	r3, [r5, #104]
	subs	r3, r3, #1
	str	r3, [r5, #104]
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #32]
	cmp	r3, #1
	bne	.L314
	ldr	r3, [r5, #112]
	subs	r3, r3, #1
	str	r3, [r5, #112]
	cbnz	r3, .L314
	ldr	r3, [r6, #1128]	@ unaligned
	mov	r0, r5
	cmp	r3, #4
	bne	.L316
	bl	mpi_node_del_in_filter
.L314:
	ldr	r3, [r4, #12]
	ldr	r3, [r3, #48]
	cmp	r3, #1
	bne	.L318
	ldr	r3, [r5, #108]
	subs	r3, r3, #1
	str	r3, [r5, #108]
	cbnz	r3, .L318
	ldr	r2, [r5, #116]
	cbz	r2, .L318
	str	r3, [r5, #116]
.L318:
	ldr	r0, [r4, #16]
	cbz	r0, .L320
	bl	mpi_obj_unref
	movs	r3, #0
	str	r3, [r4, #16]
.L320:
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r7	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	movs	r0, #0
	str	r0, [r4]
	b	.L309
.L316:
	bl	mpi_node_del_out_filter
	b	.L314
.L330:
	.align	2
.L329:
	.word	.LC42
	.word	.LANCHOR0+417
	.word	.LC47
	.word	.LC48
	.fnend
	.size	rgn_dettach_node, .-rgn_dettach_node
	.align	1
	.global	vrgn_dettach_chn
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_dettach_chn, %function
vrgn_dettach_chn:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #24
	sub	sp, sp, #24
	ldr	r0, [r1]	@ unaligned
	mov	r4, r1
	bl	list_rgn_find_by_handle
	mov	r5, r0
	cbz	r0, .L336
	ldr	r2, [r4, #1052]	@ unaligned
	movw	r3, #1148
	cmp	r2, r3
	beq	.L333
	movw	r3, #811
	ldr	r2, .L340
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L340+4
	movs	r0, #3
	bl	vlog
.L336:
	mov	r6, #-1
	b	.L331
.L333:
	ldr	r3, [r0, #28]
	cbnz	r3, .L334
	ldr	r3, [r0, #48]
	cmp	r3, #1
	bne	.L334
	ldr	r3, [r0, #24]
	movs	r1, #2
	ldr	r2, .L340+8
	movs	r0, #3
	str	r3, [sp, #4]
	mov	r3, #816
	str	r3, [sp]
	ldr	r3, .L340+4
	bl	vlog
	b	.L336
.L334:
	mov	r0, r4
	bl	rgn_dettach_node
	mov	r6, r0
	cbz	r0, .L335
	ldr	r3, [r4, #1136]	@ unaligned
	movs	r1, #2
	ldr	r2, .L340+12
	movs	r0, #3
	str	r3, [sp, #16]
	ldr	r3, [r4, #1132]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #1128]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #823
	str	r3, [sp]
	ldr	r3, .L340+4
	bl	vlog
.L335:
	.syntax unified
@ 29 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		mrs	r2, cpsr	@ arch_local_irq_save
	cpsid	i
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r3, [r5, #28]
	subs	r3, r3, #1
	str	r3, [r5, #28]
	.syntax unified
@ 171 "/home/gzc/otter_ipc_linux-bat-e3/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h" 1
		msr	cpsr_c, r2	@ local_irq_restore
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r0, [r5, #28]
	movs	r2, #72
	movs	r1, #0
	mla	r0, r2, r0, r5
	add	r0, r0, #1080
	bl	memset
	ldr	r2, [r5, #28]
	movs	r3, #12
	movs	r0, #3
	muls	r3, r2, r3
	movs	r2, #0
	add	r3, r3, #1656
	adds	r1, r5, r3
	str	r2, [r5, r3]	@ unaligned
	str	r2, [r1, #4]	@ unaligned
	str	r2, [r1, #8]	@ unaligned
	movs	r1, #4
	ldr	r3, [r4, #1136]	@ unaligned
	ldr	r2, .L340+16
	str	r3, [sp, #16]
	ldr	r3, [r4, #1132]	@ unaligned
	str	r3, [sp, #12]
	ldr	r3, [r4, #1128]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4]	@ unaligned
	str	r3, [sp, #4]
	movw	r3, #831
	str	r3, [sp]
	ldr	r3, .L340+4
	bl	vlog
.L331:
	mov	r0, r6
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, pc}
.L341:
	.align	2
.L340:
	.word	.LC26
	.word	.LANCHOR0+434
	.word	.LC49
	.word	.LC50
	.word	.LC51
	.fnend
	.size	vrgn_dettach_chn, .-vrgn_dettach_chn
	.align	1
	.global	update_canvas_to_node
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	update_canvas_to_node, %function
update_canvas_to_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r4, r0
	ldrb	r5, [r0]	@ zero_extendqisi2
	cmp	r5, #0
	beq	.L343
	ldr	r0, [r0, #2]	@ unaligned
	bl	list_rgn_find_by_handle
	mov	r5, r0
	cbnz	r0, .L344
.L360:
	mov	r0, #-1
	b	.L342
.L344:
	ldr	r0, [r4, #10]	@ unaligned
	bl	get_mpi_buf
	mov	r6, r0
	cmp	r0, #0
	beq	.L360
	ldr	r3, [r0, #16]
	cbnz	r3, .L347
	movw	r3, #637
	ldr	r2, .L361
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L361+4
	movs	r0, #3
	bl	vlog
.L347:
	ldr	r3, [r5, #1752]
	cmp	r3, r6
	bne	.L348
	ldr	r3, [r5, #48]
	cmp	r3, #1
	bls	.L348
	ldr	r3, [r4, #2]	@ unaligned
	movs	r1, #2
	ldr	r2, .L361+8
	movs	r0, #3
	str	r3, [sp, #4]
	movw	r3, #641
	str	r3, [sp]
	ldr	r3, .L361+4
	bl	vlog
.L348:
	movs	r2, #80
	add	r1, r4, #98
	add	r0, r6, #616
	bl	memcpy
	ldr	r0, [r5, #1760]
	str	r6, [r5, #1752]
	cmp	r0, #0
	bne	.L350
	ldr	r3, [r6, #444]
	str	r3, [r5, #1760]
.L342:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L343:
	ldrb	r3, [r0, #1]	@ zero_extendqisi2
	cmp	r3, #0
	beq	.L360
	ldr	r1, [r0, #18]	@ unaligned
	ldr	r2, [r0, #22]	@ unaligned
	ldr	r0, [r0, #14]	@ unaligned
	bl	vsys_get_node_from_map
	mov	r1, r0
	cbnz	r0, .L349
	ldr	r3, [r4, #22]	@ unaligned
	movs	r1, #2
	ldr	r2, .L361+12
	movs	r0, #3
	str	r3, [sp, #12]
	ldr	r3, [r4, #18]	@ unaligned
	str	r3, [sp, #8]
	ldr	r3, [r4, #14]	@ unaligned
	str	r3, [sp, #4]
	mov	r3, #652
	str	r3, [sp]
	ldr	r3, .L361+4
	bl	vlog
	b	.L360
.L349:
	ldr	r0, [r4, #2]	@ unaligned
	bl	get_list_node_rgn_from_node
	mov	r6, r0
	cmp	r0, #0
	beq	.L360
	movw	r3, #657
	ldr	r2, .L361+16
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L361+4
	movs	r0, #3
	bl	vlog
	movs	r2, #72
	add	r1, r4, #26
	add	r0, r6, #20
	bl	memcpy
.L350:
	movs	r0, #0
	b	.L342
.L362:
	.align	2
.L361:
	.word	.LC52
	.word	.LANCHOR0+451
	.word	.LC53
	.word	.LC42
	.word	.LC54
	.fnend
	.size	update_canvas_to_node, .-update_canvas_to_node
	.align	1
	.global	vrgn_update_canvas_to_chns
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_update_canvas_to_chns, %function
vrgn_update_canvas_to_chns:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, lr}
	.save {r4, r5, lr}
	.pad #12
	mov	r5, r1
	ldr	r3, [r1, #6]	@ unaligned
	cmp	r3, #178
	beq	.L364
	mov	r3, #840
	ldr	r2, .L369
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L369+4
	movs	r0, #3
	bl	vlog
	mov	r4, #-1
.L363:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, pc}
.L364:
	mov	r0, r1
	bl	update_canvas_to_node
	mov	r4, r0
	cmp	r0, #0
	beq	.L363
	ldr	r3, [r5, #2]	@ unaligned
	movs	r1, #2
	ldr	r2, .L369+8
	movs	r0, #3
	str	r3, [sp, #4]
	movw	r3, #846
	str	r3, [sp]
	ldr	r3, .L369+4
	bl	vlog
	b	.L363
.L370:
	.align	2
.L369:
	.word	.LC32
	.word	.LANCHOR0+473
	.word	.LC55
	.fnend
	.size	vrgn_update_canvas_to_chns, .-vrgn_update_canvas_to_chns
	.align	1
	.global	vrgn_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_probe, %function
vrgn_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	movw	r2, #6100
	ldr	r4, .L373
	mov	r6, r0
	movs	r1, #0
	ldr	r5, .L373+4
	mov	r0, r4
	bl	memset
	mov	r1, #3264
	mov	r0, r6
	bl	kstrdup
	mov	r2, r6
	movs	r1, #8
	movs	r3, #9
	str	r5, [r4, #188]
	str	r3, [r4, #192]
	str	r4, [r4, #164]
	str	r0, [r4]
	adds	r0, r4, #4
	bl	mpi_dev_register
	movs	r2, #0
	add	r0, r4, #196
	add	r1, r5, #180
	str	r0, [r4, #196]
	add	r3, r4, #204
	str	r0, [r4, #200]
	adds	r0, r4, #4
	str	r2, [r4, #72]
	strd	r2, r2, [r4, #80]
	str	r3, [r4, #204]
	str	r3, [r4, #208]
	add	r3, r4, #172
	str	r2, [r4, #168]
	strd	r3, r3, [r4, #172]
	bl	mpi_sysfs_dev_add
	ldr	r3, .L373+8
	mov	r2, #-1
	mov	r1, r4
	ldr	r0, .L373+12
	bl	kthread_create_on_node
	cmn	r0, #4096
	mov	r5, r0
	bhi	.L372
	bl	wake_up_process
.L372:
	str	r5, [r4, #180]
	pop	{r4, r5, r6, pc}
.L374:
	.align	2
.L373:
	.word	g_vrgn_dev
	.word	.LANCHOR0+500
	.word	.LC56
	.word	vrgn_thread_fn
	.fnend
	.size	vrgn_probe, .-vrgn_probe
	.align	1
	.global	vrgn_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_shutdown, %function
vrgn_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r3, #1
	ldr	r4, .L376
	ldr	r0, [r4, #180]
	str	r3, [r4, #184]
	bl	kthread_stop
	adds	r0, r4, #4
	pop	{r4, lr}
	b	mpi_dev_unregister
.L377:
	.align	2
.L376:
	.word	g_vrgn_dev
	.fnend
	.size	vrgn_shutdown, .-vrgn_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	vrgn_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_init, %function
vrgn_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L379
	bl	vrgn_probe
	movs	r0, #0
	pop	{r3, pc}
.L380:
	.align	2
.L379:
	.word	.LC56
	.fnend
	.size	vrgn_init, .-vrgn_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	vrgn_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vrgn_exit, %function
vrgn_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	vrgn_shutdown
	.fnend
	.size	vrgn_exit, .-vrgn_exit
	.global	g_vrgn_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.36582, %object
	.size	__func__.36582, 21
__func__.36582:
	.ascii	"node_delete_callback\000"
	.type	__func__.36894, %object
	.size	__func__.36894, 13
__func__.36894:
	.ascii	"vrgn_release\000"
	.type	__func__.36850, %object
	.size	__func__.36850, 11
__func__.36850:
	.ascii	"vrgn_ioctl\000"
	.space	3
	.type	tp, %object
	.size	tp, 40
tp:
	.word	0
	.word	.LC66
	.word	1
	.word	.LC67
	.word	2
	.word	.LC68
	.word	3
	.word	.LC69
	.word	4
	.word	.LC70
	.type	fmt, %object
	.size	fmt, 104
fmt:
	.word	65552
	.word	.LC71
	.word	65553
	.word	.LC72
	.word	65554
	.word	.LC71
	.word	65555
	.word	.LC71
	.word	65548
	.word	.LC73
	.word	65549
	.word	.LC74
	.word	65558
	.word	.LC73
	.word	65559
	.word	.LC73
	.word	65546
	.word	.LC75
	.word	65547
	.word	.LC76
	.word	65557
	.word	.LC77
	.word	65556
	.word	.LC77
	.word	65562
	.word	.LC78
	.type	__func__.36745, %object
	.size	__func__.36745, 13
__func__.36745:
	.ascii	"vrgn_version\000"
	.type	__func__.36872, %object
	.size	__func__.36872, 10
__func__.36872:
	.ascii	"vrgn_open\000"
	.type	__func__.36506, %object
	.size	__func__.36506, 16
__func__.36506:
	.ascii	"filter_callback\000"
	.type	__func__.36443, %object
	.size	__func__.36443, 15
__func__.36443:
	.ascii	"vrgn_thread_fn\000"
	.type	__func__.36424, %object
	.size	__func__.36424, 24
__func__.36424:
	.ascii	"vrgn_set_osd_frame_info\000"
	.type	__func__.36431, %object
	.size	__func__.36431, 10
__func__.36431:
	.ascii	"do_ex_osd\000"
	.type	__func__.36755, %object
	.size	__func__.36755, 12
__func__.36755:
	.ascii	"vrgn_create\000"
	.type	__func__.36806, %object
	.size	__func__.36806, 13
__func__.36806:
	.ascii	"vrgn_destroy\000"
	.type	__func__.36815, %object
	.size	__func__.36815, 23
__func__.36815:
	.ascii	"vrgn_upate_canvas_sync\000"
	.type	__func__.36828, %object
	.size	__func__.36828, 15
__func__.36828:
	.ascii	"vrgn_info_sync\000"
	.type	__func__.36836, %object
	.size	__func__.36836, 20
__func__.36836:
	.ascii	"vrgn_request_buffer\000"
	.type	__func__.36617, %object
	.size	__func__.36617, 22
__func__.36617:
	.ascii	"set_osd_invert_buffer\000"
	.type	__func__.36705, %object
	.size	__func__.36705, 16
__func__.36705:
	.ascii	"rgn_attach_node\000"
	.type	__func__.36768, %object
	.size	__func__.36768, 16
__func__.36768:
	.ascii	"vrgn_attach_chn\000"
	.type	__func__.36719, %object
	.size	__func__.36719, 17
__func__.36719:
	.ascii	"rgn_dettach_node\000"
	.type	__func__.36786, %object
	.size	__func__.36786, 17
__func__.36786:
	.ascii	"vrgn_dettach_chn\000"
	.type	__func__.36737, %object
	.size	__func__.36737, 22
__func__.36737:
	.ascii	"update_canvas_to_node\000"
	.type	__func__.36799, %object
	.size	__func__.36799, 27
__func__.36799:
	.ascii	"vrgn_update_canvas_to_chns\000"
	.type	vrgn_ioctls, %object
	.size	vrgn_ioctls, 180
vrgn_ioctls:
	.word	-1068732928
	.word	0
	.word	vrgn_version
	.word	.LC57
	.word	0
	.word	-998478335
	.word	0
	.word	vrgn_create
	.word	.LC58
	.word	0
	.word	-998478334
	.word	0
	.word	vrgn_destroy
	.word	.LC59
	.word	0
	.word	-998478333
	.word	0
	.word	vrgn_attach_chn
	.word	.LC60
	.word	0
	.word	-998478332
	.word	0
	.word	vrgn_dettach_chn
	.word	.LC61
	.word	0
	.word	-1062048251
	.word	0
	.word	vrgn_update_canvas_to_chns
	.word	.LC62
	.word	0
	.word	-1062048250
	.word	16
	.word	vrgn_upate_canvas_sync
	.word	.LC63
	.word	0
	.word	-651923961
	.word	0
	.word	vrgn_info_sync
	.word	.LC64
	.word	0
	.word	-1067422200
	.word	0
	.word	vrgn_request_buffer
	.word	.LC65
	.word	0
	.type	vrgn_driver_fops, %object
	.size	vrgn_driver_fops, 128
vrgn_driver_fops:
	.word	0
	.space	4
	.word	vrgn_read
	.space	24
	.word	0
	.word	vrgn_ioctl
	.word	vrgn_compat_ioctl
	.word	0
	.space	4
	.word	vrgn_open
	.space	4
	.word	vrgn_release
	.space	60
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	__key.13584, %object
	.size	__key.13584, 0
__key.13584:
	.type	g_vrgn_dev, %object
	.size	g_vrgn_dev, 6100
g_vrgn_dev:
	.space	6100
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[1;32mrelease list_rgn handle = %d success\033["
	.ascii	"0m\000"
.LC1:
	.ascii	"%s, %d\000"
.LC2:
	.ascii	"vrgn_ioctl: nr = %d, index = %d dev = %p\000"
.LC3:
	.ascii	"\033[1;31merror: index = %d\033[0m\000"
.LC4:
	.ascii	"\033[1;31merror: ioctl is null\033[0m\000"
.LC5:
	.ascii	"-1\000"
.LC6:
	.ascii	"vmpi:28ee389dbc33c2c7db812cd08e54fd3a0f2f992b-v1.4."
	.ascii	"81\000"
.LC7:
	.ascii	"module: %s\012version: %s\012build: %s\012\000"
.LC8:
	.ascii	"-2024-05-24-16:21:01\000"
.LC9:
	.ascii	"*****************************************rgn list**"
	.ascii	"*********************************************\012\000"
.LC10:
	.ascii	"handle---type------width---height---format----buf_i"
	.ascii	"d---refcnt---channel_num---------------------\012\000"
.LC11:
	.ascii	"%-#8d %-#9s %-#7d %-#8d %-#9s %-#8d %-#8d \000"
.LC12:
	.ascii	"****************************************node_rgn li"
	.ascii	"st*******************************************\012\000"
.LC13:
	.ascii	"id---name-------h-x---y---h-x---y---h-x---y---h-x--"
	.ascii	"-y---h-x---y---h-x---y---h-x---y---h-x---y---\012\000"
.LC14:
	.ascii	"%-#8d \000"
.LC15:
	.ascii	"\012\000"
.LC16:
	.ascii	"%-#4d %-#10s \000"
.LC17:
	.ascii	"%-#1d %-#3d %-#4d\000"
.LC18:
	.ascii	"\033[1;31mstruct vrgn_version mismatching\033[0m\000"
.LC19:
	.ascii	"\033[1;31mvrgn open fail\033[0m\000"
.LC20:
	.ascii	"%s ctx(%p) ready for node(%d) buf(%d)\012\000"
.LC21:
	.ascii	"src buf(%d: %p), rga: ctx_id(%d), x(%d), y(%d), w(%"
	.ascii	"d), h(%d), vw(%d), vh(%d), pf(0x%x), cp(%d)\000"
.LC22:
	.ascii	"pat buf(%d: %p), rga: ctx_id(%d), x(%d), y(%d), w(%"
	.ascii	"d), h(%d), vw(%d), vh(%d), pf(0x%x), cp(%d)\000"
.LC23:
	.ascii	"dst buf(%d: %p), rga: ctx_id(%d), x(%d), y(%d), w(%"
	.ascii	"d), h(%d), vw(%d), vh(%d), pf(0x%x), cp(%d)\000"
.LC24:
	.ascii	"vrga done buf(%d: %p->%d: %p) nick_id(%d)\000"
.LC25:
	.ascii	"\033[1;31mrga(%d) do buf(%d: %p->%d: %p) err!\033[0"
	.ascii	"m\000"
.LC26:
	.ascii	"\033[1;31mstruct vrgn_info mismatching\033[0m\000"
.LC27:
	.ascii	"\033[1;32mrgn handle = %d reduce osd size success\033"
	.ascii	"[0m\000"
.LC28:
	.ascii	"\0013%s[%d]: vrgn get rga node fail channel_id = %d"
	.ascii	"\012\000"
.LC29:
	.ascii	"&x->wait\000"
.LC30:
	.ascii	"\033[1;32mrgn handle = %d create success\033[0m\000"
.LC31:
	.ascii	"\033[1;32mdestroy rgn handle = %d, delete list_rgn "
	.ascii	"handle = %d success\033[0m\000"
.LC32:
	.ascii	"\033[1;31mstruct canvas_info mismatching\033[0m\000"
.LC33:
	.ascii	"\033[1;31mrgn handle = %d not find list_rgn\033[0m\000"
.LC34:
	.ascii	"\033[1;31mstruct sync_info mismatching\033[0m\000"
.LC35:
	.ascii	"\033[1;31mthere is not have rgn sync infomation\033"
	.ascii	"[0m\000"
.LC36:
	.ascii	"\033[1;31mstruct buffer_info mismatching\033[0m\000"
.LC37:
	.ascii	"\033[1;31mrequest buffer from buffer pool(%d)\033[0"
	.ascii	"m\000"
.LC38:
	.ascii	"valloc\000"
.LC39:
	.ascii	"vrgn_invert\000"
.LC40:
	.ascii	"\033[1;32minvert buffer(id:%d, size: %d, dma: %p, c"
	.ascii	"nt: %ld) alloc success!\033[0m\000"
.LC41:
	.ascii	"\033[1;31minvert buffer alloc failed!\033[0m\000"
.LC42:
	.ascii	"\033[1;31mget node [%d %d %d] fail\033[0m\000"
.LC43:
	.ascii	"\033[1;31mrgn handle = %d has attach one node!\033["
	.ascii	"0m\000"
.LC44:
	.ascii	"\033[1;31mrgn handle = %d attach node [%d %d %d] fa"
	.ascii	"il\033[0m\000"
.LC45:
	.ascii	"\033[1;32mrgn handle = %d attach node [%d %d %d] su"
	.ascii	"ccess\033[0m\000"
.LC46:
	.ascii	"\033[1;31mget list node rgn fail\033[0m\000"
.LC47:
	.ascii	"\033[1;31mnode [%d %d %d] is not has osd!\033[0m\000"
.LC48:
	.ascii	"unref list_node_rgn\000"
.LC49:
	.ascii	"\033[1;31mrgn handle = %d has dettach from node!\033"
	.ascii	"[0m\000"
.LC50:
	.ascii	"\033[1;31mrgn handle = %d dettach node [%d %d %d] f"
	.ascii	"ail\033[0m\000"
.LC51:
	.ascii	"\033[1;32mrgn handle = %d dettach from [%d, %d, %d]"
	.ascii	" success\033[0m\000"
.LC52:
	.ascii	"\033[1;31mmpi_buf refcnt err\033[0m\000"
.LC53:
	.ascii	"\033[1;31mrgn handle = %d update frame error\033[0m"
	.ascii	"\000"
.LC54:
	.ascii	"update list_node_rgn chn attr success\000"
.LC55:
	.ascii	"\033[1;31mrgn handle = %d update canvas to node fai"
	.ascii	"l\033[0m\000"
.LC56:
	.ascii	"vrgn\000"
.LC57:
	.ascii	"VRGN_IOCTL_VERSION\000"
.LC58:
	.ascii	"VRGN_IOCTL_CREATE\000"
.LC59:
	.ascii	"VRGN_IOCTL_DESTORY\000"
.LC60:
	.ascii	"VRGN_IOCTL_ATTACH_CH\000"
.LC61:
	.ascii	"VRGN_IOCTL_DETTACH_CH\000"
.LC62:
	.ascii	"VRGN_IOCTL_UPDATE_CANVAS\000"
.LC63:
	.ascii	"VRGN_IOCTL_UPDATE_SYNC\000"
.LC64:
	.ascii	"VRGN_IOCTL_INFO_SYNC\000"
.LC65:
	.ascii	"VRGN_IOCTL_REQUEST_BUFFER\000"
.LC66:
	.ascii	"OVERLAY\000"
.LC67:
	.ascii	"OVERLAY_EX\000"
.LC68:
	.ascii	"COVER\000"
.LC69:
	.ascii	"MOSAIC\000"
.LC70:
	.ascii	"LINE\000"
.LC71:
	.ascii	"ARGB8888\000"
.LC72:
	.ascii	"ABGR8888\000"
.LC73:
	.ascii	"ARGB4444\000"
.LC74:
	.ascii	"ABGR4444\000"
.LC75:
	.ascii	"ARGB1555\000"
.LC76:
	.ascii	"ABGR1555\000"
.LC77:
	.ascii	"ARGB5551\000"
.LC78:
	.ascii	"2BPP\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
