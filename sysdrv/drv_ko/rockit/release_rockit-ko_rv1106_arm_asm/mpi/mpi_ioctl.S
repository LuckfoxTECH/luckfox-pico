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
	.file	"mpi_ioctl.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_mpi_ioctl_kernel:					
	.asciz 	"mpi_ioctl_kernel"					
__kstrtabns_mpi_ioctl_kernel:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.global	mpi_ioctl_kernel
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpi_ioctl_kernel, %function
mpi_ioctl_kernel:
	.fnstart
	@ args = 0, pretend = 0, frame = 144
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r10, r1
	ldr	r1, .L31
	mov	r8, r3
	.pad #164
	sub	sp, sp, #164
	ubfx	r6, r2, #16, #14
	ldr	r9, [r10, #8]
	mov	fp, r0
	ldr	r3, [r1]
	str	r1, [sp, #16]
	str	r3, [sp, #156]
	ldr	r3, [r10]
	ands	r2, r2, r3
	ubfx	r7, r3, #16, #14
	ands	r4, r2, #1073741824
	it	ne
	movne	r4, r6
	cmp	r2, #0
	it	ge
	movge	r6, #0
	cmp	r7, r4
	it	cc
	movcc	r7, r4
	cmp	r7, r6
	it	cc
	movcc	r7, r6
	cmp	r9, #0
	bne	.L4
	movs	r3, #33
	ldr	r2, .L31+4
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L31+8
	mov	r0, r9
	bl	vlog
	mov	r5, r9
	mvn	r4, #21
.L5:
	mov	r0, r5
	bl	kfree
	b	.L1
.L4:
	cmp	r7, #128
	bls	.L19
	mov	r1, #3264
	mov	r0, r7
	bl	__kmalloc
	mov	r5, r0
	cbnz	r0, .L6
	mvn	r4, #11
	b	.L5
.L19:
	add	r5, sp, #28
.L6:
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r3, [r3, #8]
	.syntax unified
@ 157 "./include/linux/uaccess.h" 1
	.syntax unified
adds r2, r8, r4; sbcscc r2, r2, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L21
	mov	r2, r4
	mov	r1, r8
	mov	r0, r5
	bl	arm_copy_from_user
	mov	r3, r0
.L7:
	cbz	r3, .L9
	subs	r0, r4, r3
	mov	r2, r3
	movs	r1, #0
	add	r0, r0, r5
	str	r3, [sp, #20]
	bl	memset
	ldr	r2, .L31+12
	movs	r1, #2
	movs	r0, #0
	ldr	r3, [sp, #20]
	str	r3, [sp, #4]
	movs	r3, #51
	str	r3, [sp]
	ldr	r3, .L31+8
.L30:
	bl	vlog
	mvn	r4, #13
.L10:
	add	r3, sp, #28
	cmp	r5, r3
	bne	.L5
.L1:
	ldr	r3, [sp, #16]
	mov	r0, r4
	ldr	r2, [sp, #156]
	ldr	r3, [r3]
	cmp	r2, r3
	beq	.L17
	bl	__stack_chk_fail
.L21:
	mov	r3, r4
	b	.L7
.L9:
	cmp	r4, r7
	bcs	.L11
	subs	r2, r7, r4
	movs	r1, #0
	adds	r0, r5, r4
	bl	memset
.L11:
	ldr	r3, [r10, #4]
	lsls	r3, r3, #27
	bpl	.L12
	mov	r1, r5
	mov	r0, fp
	blx	r9
	mov	r4, r0
.L13:
	ldr	r3, .L31+8
	movs	r2, #73
	movs	r1, #5
	str	r2, [sp]
	movs	r0, #0
	ldr	r2, .L31+16
	strd	r3, r4, [sp, #4]
	bl	vlog
	mov	r3, sp
	bic	r3, r3, #8160
	bic	r3, r3, #31
	ldr	r3, [r3, #8]
	.syntax unified
@ 177 "./include/linux/uaccess.h" 1
	.syntax unified
adds r2, r8, r6; sbcscc r2, r2, r3; movcc r3, #0
@ 0 "" 2
	.thumb
	.syntax unified
	cbnz	r3, .L15
	mov	r2, r6
	mov	r1, r5
	mov	r0, r8
	bl	arm_copy_to_user
	mov	r6, r0
.L15:
	cmp	r6, #0
	beq	.L10
	movs	r3, #76
	movs	r0, #0
	ldr	r2, .L31+12
	movs	r1, #2
	strd	r3, r0, [sp]
	ldr	r3, .L31+8
	b	.L30
.L12:
	ldr	r0, [r10, #16]
	cbz	r0, .L14
	bl	mutex_lock
	mov	r1, r5
	mov	r0, fp
	blx	r9
	mov	r4, r0
	ldr	r0, [r10, #16]
.L29:
	bl	mutex_unlock
	b	.L13
.L14:
	ldr	r0, .L31+20
	bl	mutex_lock
	mov	r1, r5
	mov	r0, fp
	blx	r9
	mov	r4, r0
	ldr	r0, .L31+20
	b	.L29
.L17:
	add	sp, sp, #164
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L32:
	.align	2
.L31:
	.word	__stack_chk_guard
	.word	.LC0
	.word	.LANCHOR0
	.word	.LC1
	.word	.LC2
	.word	mpi_global_mutex
	.fnend
	.size	mpi_ioctl_kernel, .-mpi_ioctl_kernel
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.36236, %object
	.size	__func__.36236, 17
__func__.36236:
	.ascii	"mpi_ioctl_kernel\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\033[31m\033[1m%s,%d:error: no function\033[0m\000"
.LC1:
	.ascii	"\033[31m\033[1m%s,%d:error: copy_from_user fail: re"
	.ascii	"t: %d\033[0m\000"
.LC2:
	.ascii	"%s,%d:%s:retcode = %d\000"
	.section	___ksymtab+mpi_ioctl_kernel,"a"
	.align	2
	.type	__ksymtab_mpi_ioctl_kernel, %object
	.size	__ksymtab_mpi_ioctl_kernel, 12
__ksymtab_mpi_ioctl_kernel:
	.word	mpi_ioctl_kernel
	.word	__kstrtab_mpi_ioctl_kernel
	.word	__kstrtabns_mpi_ioctl_kernel
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
