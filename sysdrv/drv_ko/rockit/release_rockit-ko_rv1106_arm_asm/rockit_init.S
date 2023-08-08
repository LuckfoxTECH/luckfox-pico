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
	.file	"rockit_init.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.section	.text.unlikely,"ax",%progbits
	.align	1
	.global	vmcu_callback
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vmcu_callback, %function
vmcu_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	vmcu_load
	.fnend
	.size	vmcu_callback, .-vmcu_callback
	.section	.init.text,"ax",%progbits
	.align	1
	.global	rockit_load
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_load, %function
rockit_load:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	ldr	r0, .L12
	bl	printk
	ldr	r5, .L12+4
	bl	mpi_init
	bl	valloc_init
	bl	vsys_load
	bl	vrga_init
	bl	venc_init
	bl	rockit_rkisp_init
	bl	vvi_init
	bl	vpss_init
	bl	vrgn_init
	movs	r2, #0
	str	r2, [r5]
	ldr	r2, .L12+8
	ldr	r2, [r2]
	cbz	r2, .L3
	ldr	r4, .L12+12
	ldr	r3, .L12+16
	mov	r0, r4
	str	r3, [r4]
	bl	register_reboot_notifier
	cbz	r0, .L4
	ldr	r0, .L12+20
	bl	printk
.L4:
	bl	rk_tb_mcu_is_done
	cbz	r0, .L5
	bl	vmcu_load
.L3:
	ldr	r0, .L12+24
	bl	printk
	movs	r0, #0
	pop	{r3, r4, r5, pc}
.L5:
	ldr	r3, .L12+28
	str	r0, [r4, #20]
	ldr	r0, .L12+32
	str	r3, [r4, #24]
	bl	rk_tb_client_register_cb_head
	ldr	r0, .L12+36
	bl	printk
	movs	r3, #1
	str	r3, [r5]
	b	.L3
.L13:
	.align	2
.L12:
	.word	.LC0
	.word	wait_mcu_flag
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	reboot_notifier_callback
	.word	.LC1
	.word	.LC3
	.word	vmcu_callback
	.word	.LANCHOR1+12
	.word	.LC2
	.fnend
	.size	rockit_load, .-rockit_load
	.section	.text.unlikely
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	reboot_notifier_callback, %function
reboot_notifier_callback:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L15
	bl	printk
	bl	vsys_close
	bl	mcu_reset
	movs	r0, #0
	pop	{r3, pc}
.L16:
	.align	2
.L15:
	.word	.LC4
	.fnend
	.size	reboot_notifier_callback, .-reboot_notifier_callback
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	rockit_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_exit, %function
rockit_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	bl	vrgn_exit
	bl	vpss_exit
	bl	vvi_exit
	bl	rockit_rkisp_exit
	bl	venc_exit
	bl	vrga_exit
	bl	vsys_exit
	bl	valloc_exit
	bl	mpi_deinit
	ldr	r3, .L19
	ldr	r3, [r3]
	cbz	r3, .L17
	pop	{r3, lr}
	b	vmcu_exit
.L17:
	pop	{r3, pc}
.L20:
	.align	2
.L19:
	.word	.LANCHOR0
	.fnend
	.size	rockit_exit, .-rockit_exit
	.global	reboot_notif
	.global	en_mcu
	.section	.rodata
	.type	__param_str_en_mcu, %object
	.size	__param_str_en_mcu, 14
__param_str_en_mcu:
	.ascii	"rockit.en_mcu\000"
	.data
	.align	2
	.set	.LANCHOR0,. + 0
	.type	en_mcu, %object
	.size	en_mcu, 4
en_mcu:
	.word	1
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	reboot_notif, %object
	.size	reboot_notif, 12
reboot_notif:
	.space	12
	.type	client, %object
	.size	client, 16
client:
	.space	16
	.section	.exitcall.exit,"aw"
	.align	2
	.type	__exitcall_rockit_exit, %object
	.size	__exitcall_rockit_exit, 4
__exitcall_rockit_exit:
	.word	rockit_exit
	.section	.initcall6s.init,"aw"
	.align	2
	.type	__initcall__kmod_rockit__228_97_rockit_load6s, %object
	.size	__initcall__kmod_rockit__228_97_rockit_load6s, 4
__initcall__kmod_rockit__228_97_rockit_load6s:
	.word	rockit_load
	.section	.modinfo,"a"
	.type	__UNIQUE_ID_import_ns232, %object
	.size	__UNIQUE_ID_import_ns232, 75
__UNIQUE_ID_import_ns232:
	.ascii	"rockit.import_ns=VFS_internal_I_am_really_a_filesys"
	.ascii	"tem_and_am_NOT_a_driver\000"
	.type	__UNIQUE_ID_license231, %object
	.size	__UNIQUE_ID_license231, 19
__UNIQUE_ID_license231:
	.ascii	"rockit.license=GPL\000"
	.type	__UNIQUE_ID_file230, %object
	.size	__UNIQUE_ID_file230, 37
__UNIQUE_ID_file230:
	.ascii	"rockit.file=drivers/rockit-ko/rockit\000"
	.type	__UNIQUE_ID_author229, %object
	.size	__UNIQUE_ID_author229, 23
__UNIQUE_ID_author229:
	.ascii	"rockit.author=Rockchip\000"
	.type	__UNIQUE_ID_en_mcutype227, %object
	.size	__UNIQUE_ID_en_mcutype227, 28
__UNIQUE_ID_en_mcutype227:
	.ascii	"rockit.parmtype=en_mcu:uint\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"rockit_load start\000"
.LC1:
	.ascii	"register reboot callback fail mcu not start\000"
.LC2:
	.ascii	"rockit wait hpmcu complete, please check tb fw\000"
.LC3:
	.ascii	"rockit_load end\000"
.LC4:
	.ascii	"rockit get a system reboot event\000"
	.section	__param,"a"
	.align	2
	.type	__param_en_mcu, %object
	.size	__param_en_mcu, 20
__param_en_mcu:
	.word	__param_str_en_mcu
	.word	0
	.word	param_ops_uint
	.short	420
	.byte	-1
	.byte	0
	.word	en_mcu
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
