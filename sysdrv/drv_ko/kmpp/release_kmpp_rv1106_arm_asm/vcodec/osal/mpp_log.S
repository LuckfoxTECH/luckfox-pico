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
	.file	"mpp_log.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.global	mpp_debug
	.bss
	.align	2
	.type	mpp_debug, %object
	.size	mpp_debug, 4
mpp_debug:
	.space	4
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
