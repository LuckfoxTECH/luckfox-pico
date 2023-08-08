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
	.file	"h265e_rc.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.global	default_h265e
	.section	.rodata
	.align	2
	.type	default_h265e, %object
	.size	default_h265e, 48
default_h265e:
	.word	.LC0
	.word	16777220
	.word	664
	.word	rc_model_v2_init
	.word	rc_model_v2_deinit
	.word	0
	.word	rc_model_v2_check_reenc
	.word	rc_model_v2_start
	.word	rc_model_v2_end
	.word	rc_model_v2_hal_start
	.word	rc_model_v2_hal_end
	.word	rc_model_v2_proc_show
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"default\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
