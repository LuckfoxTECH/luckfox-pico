/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

	.arch armv8-a+nosimd
	.file	"rkf_waveform.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.section	.text.get_wf_buf,"ax",@progbits
	.align	2
	.type	get_wf_buf, %function
get_wf_buf:
.LFB219:
	.file 1 "drivers/video/rk_eink/epdlut/rkf_waveform.c"
	.loc 1 193 0
	.cfi_startproc
.LVL0:
	.loc 1 194 0
	adrp	x2, .LANCHOR0
	.loc 1 201 0
	cmp	w1, 6
	.loc 1 194 0
	ldr	x2, [x2, #:lo12:.LANCHOR0]
.LVL1:
	.loc 1 201 0
	bhi	.L15
	adrp	x3, .L4
	add	x3, x3, :lo12:.L4
	ldrb	w1, [x3,w1,uxtw]
	adr	x3, .Lrtx4
	add	x1, x3, w1, sxtb #2
	br	x1
.Lrtx4:
.LVL2:
	.section	.rodata.get_wf_buf,"a",@progbits
	.align	0
	.align	2
.L4:
	.byte	(.L3 - .Lrtx4) / 4
	.byte	(.L5 - .Lrtx4) / 4
	.byte	(.L6 - .Lrtx4) / 4
	.byte	(.L7 - .Lrtx4) / 4
	.byte	(.L8 - .Lrtx4) / 4
	.byte	(.L9 - .Lrtx4) / 4
	.byte	(.L10 - .Lrtx4) / 4
	.section	.text.get_wf_buf
.L3:
	.loc 1 203 0
	add	x1, x2, 148
.LVL3:
	.loc 1 204 0
	add	x3, x2, 596
.LVL4:
.L11:
.LBB31:
.LBB32:
.LBB33:
	.loc 1 176 0
	cmp	w0, 0
.LBE33:
.LBB34:
	.loc 1 177 0
	mov	w4, 49
.LBE34:
.LBB35:
	.loc 1 176 0
	csel	w0, w0, wzr, ge
.LVL5:
.LBE35:
	.loc 1 173 0
	mov	w5, 0
.LBB36:
	.loc 1 177 0
	cmp	w0, 49
	csel	w0, w0, w4, le
.LVL6:
	mov	x4, 0
.LVL7:
.L14:
.LBE36:
	.loc 1 180 0
	ldrb	w6, [x1, x4]
	cmp	w0, w6
	bgt	.L12
.LVL8:
	.loc 1 185 0
	add	x5, x1, x4
	ldrb	w5, [x5, 1]
	cbz	w5, .L16
	mov	w5, w4
.L12:
.LVL9:
	add	x4, x4, 1
.LVL10:
	.loc 1 179 0
	cmp	x4, 50
	bne	.L14
.LVL11:
.L13:
.LBE32:
.LBE31:
	.loc 1 236 0
	ldr	w0, [x3, w5, sxtw 2]
	add	x0, x2, x0
	.loc 1 238 0
	ret
.LVL12:
.L5:
	.loc 1 207 0
	add	x1, x2, 468
.LVL13:
	.loc 1 208 0
	add	x3, x2, 1876
.LVL14:
	.loc 1 209 0
	b	.L11
.LVL15:
.L6:
	.loc 1 211 0
	add	x1, x2, 212
.LVL16:
	.loc 1 212 0
	add	x3, x2, 852
.LVL17:
	.loc 1 213 0
	b	.L11
.LVL18:
.L7:
	.loc 1 215 0
	add	x1, x2, 276
.LVL19:
	.loc 1 216 0
	add	x3, x2, 1108
.LVL20:
	.loc 1 217 0
	b	.L11
.LVL21:
.L8:
	.loc 1 219 0
	add	x1, x2, 340
.LVL22:
	.loc 1 220 0
	add	x3, x2, 1364
.LVL23:
	.loc 1 221 0
	b	.L11
.LVL24:
.L9:
	.loc 1 223 0
	add	x1, x2, 404
.LVL25:
	.loc 1 224 0
	add	x3, x2, 1620
.LVL26:
	.loc 1 225 0
	b	.L11
.LVL27:
.L10:
	.loc 1 227 0
	add	x1, x2, 532
.LVL28:
	.loc 1 228 0
	add	x3, x2, 2132
.LVL29:
	.loc 1 229 0
	b	.L11
.LVL30:
.L16:
.LBB38:
.LBB37:
	.loc 1 185 0
	mov	w5, w4
	b	.L13
.LVL31:
.L15:
.LBE37:
.LBE38:
	.loc 1 231 0
	mov	x0, 0
.LVL32:
	.loc 1 239 0
	ret
	.cfi_endproc
.LFE219:
	.size	get_wf_buf, .-get_wf_buf
	.section	.text.decode_wf_data,"ax",@progbits
	.align	2
	.type	decode_wf_data, %function
decode_wf_data:
.LFB221:
	.loc 1 248 0
	.cfi_startproc
.LVL33:
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	.loc 1 251 0
	uxtw	x20, w1
.LVL34:
	.loc 1 248 0
	str	x21, [sp, 32]
	.cfi_offset 21, -16
.LBB39:
.LBB40:
	.file 2 "include/linux/compat.h"
	.loc 2 58 0
	mov	w1, 32768
.LVL35:
.LBE40:
.LBE39:
	.loc 1 248 0
	mov	x21, x0
.LBB42:
.LBB41:
	.loc 2 58 0
	mov	x0, x20
.LVL36:
	bl	kmalloc
.LVL37:
	mov	x19, x0
.LVL38:
.LBE41:
.LBE42:
	.loc 1 252 0
	cbz	x0, .L19
	.loc 1 255 0
	mov	x2, x20
	add	x1, x21, 4
	bl	memcpy
.LVL39:
.L19:
	.loc 1 258 0
	mov	x0, x19
	ldr	x21, [sp, 32]
.LVL40:
	ldp	x19, x20, [sp, 16]
.LVL41:
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa 31, 0
	ret
	.cfi_endproc
.LFE221:
	.size	decode_wf_data, .-decode_wf_data
	.section	.text.parse_wf_gray16,"ax",@progbits
	.align	2
	.type	parse_wf_gray16, %function
parse_wf_gray16:
.LFB225:
	.loc 1 349 0
	.cfi_startproc
.LVL42:
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	str	x19, [sp, 16]
	.cfi_offset 19, -16
	.loc 1 349 0
	mov	x19, x0
	mov	w0, w1
.LVL43:
	.loc 1 355 0
	mov	w1, w2
.LVL44:
	bl	get_wf_buf
.LVL45:
	.loc 1 356 0
	ldrb	w1, [x0]
.LVL46:
	.loc 1 358 0
	str	w1, [x19]
.LVL47:
	.loc 1 362 0
	lsl	w1, w1, 6
.LVL48:
	bl	decode_wf_data
.LVL49:
	str	x0, [x19, 8]
	.loc 1 366 0
	cmp	x0, 0
	.loc 1 367 0
	ldr	x19, [sp, 16]
.LVL50:
	mov	w0, -22
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 19
	.cfi_def_cfa 31, 0
	csel	w0, w0, wzr, eq
	ret
	.cfi_endproc
.LFE225:
	.size	parse_wf_gray16, .-parse_wf_gray16
	.section	.text.rkf_wf_input,"ax",@progbits
	.align	2
	.global	rkf_wf_input
	.type	rkf_wf_input, %function
rkf_wf_input:
.LFB216:
	.loc 1 129 0
	.cfi_startproc
.LVL51:
	stp	x29, x30, [sp, -80]!
	.cfi_def_cfa_offset 80
	.cfi_offset 29, -80
	.cfi_offset 30, -72
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -48
	.cfi_offset 22, -40
	.loc 1 133 0
	adrp	x22, .LANCHOR0
	.loc 1 129 0
	stp	x19, x20, [sp, 16]
	.loc 1 133 0
	ldr	x1, [x22, #:lo12:.LANCHOR0]
	.loc 1 129 0
	stp	x23, x24, [sp, 48]
	str	x25, [sp, 64]
	.cfi_offset 19, -64
	.cfi_offset 20, -56
	.cfi_offset 23, -32
	.cfi_offset 24, -24
	.cfi_offset 25, -16
	.loc 1 133 0
	cbnz	x1, .L40
	mov	x19, x0
.LVL52:
.LBB51:
.LBB52:
	.loc 1 94 0
	mov	x2, 12
	adrp	x1, .LC0
	add	x0, x0, 4
.LVL53:
	add	x1, x1, :lo12:.LC0
	bl	strncmp
.LVL54:
	mov	w21, w0
	cbz	w0, .L30
	.loc 1 97 0
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	printf
.LVL55:
.LBE52:
.LBE51:
	.loc 1 138 0
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
.L44:
	.loc 1 144 0
	bl	printf
.LVL56:
	.loc 1 145 0
	mov	w21, -1
	b	.L28
.LVL57:
.L36:
.LBB53:
.LBB54:
.LBB55:
.LBB56:
	.loc 1 87 0
	ldrb	w0, [x19, x1]
	add	x1, x1, 1
.LVL58:
	eor	w0, w0, w2, lsr 24
	ldr	w0, [x20, x0, lsl 2]
	eor	w2, w0, w2, lsl 8
.LVL59:
	b	.L35
.LVL60:
.L40:
.LBE56:
.LBE55:
.LBE54:
.LBE53:
	.loc 1 134 0
	mov	w21, -16
.LVL61:
.L28:
	.loc 1 153 0
	mov	w0, w21
	ldr	x25, [sp, 64]
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 80
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 25
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa 31, 0
	ret
.LVL62:
.L30:
	.cfi_restore_state
.LBB70:
.LBB69:
	.loc 1 107 0
	ldr	w24, [x19]
	cmp	w24, 262144
	bgt	.L38
.LBB67:
.LBB65:
	.loc 1 84 0
	adrp	x20, .LANCHOR1
	add	x23, x20, :lo12:.LANCHOR1
.LBE65:
.LBE67:
	.loc 1 110 0
	ldr	w25, [x19, w24, sxtw]
.LVL63:
.LBB68:
.LBB66:
	.loc 1 84 0
	mov	x2, 1024
	mov	w1, 0
	mov	x0, x23
	bl	memset
.LVL64:
.LBB57:
.LBB58:
	.loc 1 70 0
	mov	w4, 3511
.LBE58:
.LBE57:
	.loc 1 84 0
	mov	x3, 0
.LBB62:
.LBB59:
	.loc 1 70 0
	movk	w4, 0x4c1, lsl 16
.LVL65:
.L34:
	lsl	w0, w3, 22
.LVL66:
.LBE59:
.LBE62:
	.loc 1 84 0
	mov	w2, 8
.LBB63:
.LBB60:
	.loc 1 67 0
	mov	w1, 0
.LVL67:
.L33:
	.loc 1 69 0
	eor	w6, w0, w1
	lsl	w1, w1, 1
.LVL68:
	.loc 1 70 0
	mov	w5, w1
	cmp	w6, 0
	eor	w1, w1, w4
	.loc 1 73 0
	lsl	w0, w0, 1
.LVL69:
	.loc 1 70 0
	csel	w1, w1, w5, lt
.LVL70:
	.loc 1 68 0
	subs	w2, w2, #1
.LVL71:
	bne	.L33
	.loc 1 75 0
	str	w1, [x3, x23]
	add	x3, x3, 4
	.loc 1 65 0
	cmp	x3, 1024
	bne	.L34
.LBE60:
.LBE63:
	.loc 1 87 0
	add	x20, x20, :lo12:.LANCHOR1
.LBB64:
.LBB61:
	.loc 1 65 0
	mov	x1, 0
.LVL72:
.L35:
.LBE61:
.LBE64:
	.loc 1 86 0
	cmp	w24, w1
	bhi	.L36
.LVL73:
.LBE66:
.LBE68:
	.loc 1 113 0
	cmp	w25, w2
	beq	.L37
	.loc 1 114 0
	adrp	x0, .LC3
	mov	w1, w25
	add	x0, x0, :lo12:.LC3
	bl	printf
.LVL74:
.L38:
.LBE69:
.LBE70:
	.loc 1 144 0
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	b	.L44
.LVL75:
.L37:
	.loc 1 148 0
	add	x1, x19, 20
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	bl	printf
.LVL76:
	.loc 1 150 0
	str	x19, [x22, #:lo12:.LANCHOR0]
	.loc 1 152 0
	b	.L28
	.cfi_endproc
.LFE216:
	.size	rkf_wf_input, .-rkf_wf_input
	.section	.text.rkf_wf_get_version,"ax",@progbits
	.align	2
	.global	rkf_wf_get_version
	.type	rkf_wf_get_version, %function
rkf_wf_get_version:
.LFB217:
	.loc 1 163 0
	.cfi_startproc
	.loc 1 164 0
	adrp	x0, .LANCHOR0
	ldr	x1, [x0, #:lo12:.LANCHOR0]
	.loc 1 167 0
	add	x0, x1, 84
	cmp	x1, 0
	.loc 1 168 0
	csel	x0, x0, xzr, ne
	ret
	.cfi_endproc
.LFE217:
	.size	rkf_wf_get_version, .-rkf_wf_get_version
	.section	.text.rkf_wf_get_lut,"ax",@progbits
	.align	2
	.global	rkf_wf_get_lut
	.type	rkf_wf_get_lut, %function
rkf_wf_get_lut:
.LFB229:
	.loc 1 462 0
	.cfi_startproc
.LVL77:
	.loc 1 467 0
	adrp	x3, .LANCHOR0
	ldr	x3, [x3, #:lo12:.LANCHOR0]
	cbz	x3, .L74
	.loc 1 470 0
	cbz	x0, .L75
	.loc 1 462 0
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	add	x29, sp, 0
	.cfi_def_cfa_register 29
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	.loc 1 474 0
	adrp	x20, .LANCHOR2
	.loc 1 462 0
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -16
	.cfi_offset 22, -8
	.loc 1 474 0
	ldr	w3, [x20, #:lo12:.LANCHOR2]
	cmp	w3, w2
	adrp	x3, .LANCHOR3
	bne	.L50
	.loc 1 474 0 is_stmt 0 discriminator 1
	ldr	w4, [x3, #:lo12:.LANCHOR3]
	cmp	w4, w1
	beq	.L76
.L50:
	mov	x19, x0
	.loc 1 481 0 is_stmt 1
	ldr	x0, [x0, 8]
.LVL78:
	.loc 1 477 0
	str	w2, [x20, #:lo12:.LANCHOR2]
	mov	w21, w1
	.loc 1 478 0
	str	w1, [x3, #:lo12:.LANCHOR3]
	.loc 1 481 0
	cbz	x0, .L51
.LVL79:
.LBB104:
.LBB105:
	.loc 2 81 0
	bl	free
.LVL80:
.LBE105:
.LBE104:
	.loc 1 483 0
	str	xzr, [x19, 8]
.L51:
	.loc 1 486 0
	cmp	w21, 2
	beq	.L56
	.loc 1 489 0
	cmp	w21, 11
	beq	.L53
	.loc 1 493 0
	sub	w21, w21, #1
.LVL81:
	cmp	w21, 9
	bhi	.L70
	adrp	x0, .L55
	add	x0, x0, :lo12:.L55
	ldrb	w0, [x0,w21,uxtw]
	adr	x1, .Lrtx55
	add	x0, x1, w0, sxtb #2
	br	x0
.Lrtx55:
	.section	.rodata.rkf_wf_get_lut,"a",@progbits
	.align	0
	.align	2
.L55:
	.byte	(.L54 - .Lrtx55) / 4
	.byte	(.L70 - .Lrtx55) / 4
	.byte	(.L56 - .Lrtx55) / 4
	.byte	(.L57 - .Lrtx55) / 4
	.byte	(.L58 - .Lrtx55) / 4
	.byte	(.L59 - .Lrtx55) / 4
	.byte	(.L56 - .Lrtx55) / 4
	.byte	(.L60 - .Lrtx55) / 4
	.byte	(.L53 - .Lrtx55) / 4
	.byte	(.L61 - .Lrtx55) / 4
	.section	.text.rkf_wf_get_lut
.L54:
.LVL82:
.LBB106:
.LBB107:
	.loc 1 325 0
	ldr	w0, [x20, #:lo12:.LANCHOR2]
	mov	w1, 0
	bl	get_wf_buf
.LVL83:
	.loc 1 326 0
	ldrb	w22, [x0]
.LVL84:
	.loc 1 329 0
	add	w20, w22, 15
	lsr	w20, w20, 4
.LVL85:
	.loc 1 330 0
	lsl	w1, w20, 2
.LVL86:
	bl	decode_wf_data
.LVL87:
	mov	x21, x0
.LVL88:
	.loc 1 331 0
	cbz	x0, .L70
	.loc 1 335 0
	str	w22, [x19]
.LVL89:
.LBB108:
.LBB109:
.LBB110:
.LBB111:
	.loc 2 58 0
	mov	w1, 32768
	ubfiz	x0, x20, 10, 5
.LVL90:
	bl	kmalloc
.LVL91:
.LBE111:
.LBE110:
	.loc 1 272 0
	cbz	x0, .L78
	mov	x2, 0
.LVL92:
.L64:
	.loc 1 275 0
	cmp	w20, w2
	bgt	.L68
.LVL93:
.L63:
.LBE109:
.LBE108:
	.loc 1 338 0
	str	x0, [x19, 8]
	.loc 1 339 0
	cbnz	x0, .L69
.LVL94:
.LBB114:
.LBB115:
	.loc 2 81 0
	mov	x0, x21
	bl	free
.LVL95:
.L70:
.LBE115:
.LBE114:
.LBE107:
.LBE106:
	.loc 1 539 0
	mov	w0, -1
	b	.L48
.LVL96:
.L68:
.LBB120:
.LBB119:
.LBB116:
.LBB113:
	.loc 1 276 0
	ldr	w7, [x21, x2, lsl 2]
.LVL97:
	lsl	x6, x2, 8
	mov	x3, 0
.LVL98:
.L67:
.LBB112:
	.loc 1 278 0
	lsl	w1, w3, 1
	mov	w4, 0
	asr	w1, w7, w1
	and	w1, w1, 3
.LVL99:
	.loc 1 279 0
	mov	w5, w1
.LVL100:
.L65:
	.loc 1 282 0
	lsl	w8, w1, w4
	add	w4, w4, 2
	orr	w5, w5, w8
.LVL101:
	.loc 1 281 0
	cmp	w4, 32
	bne	.L65
	add	x4, x6, x3, lsl 4
	mov	x1, 0
.LVL102:
	add	x4, x0, x4, lsl 2
.LVL103:
.L66:
	.loc 1 285 0
	str	w5, [x4, x1, lsl 2]
.LVL104:
	add	x1, x1, 1
.LVL105:
	.loc 1 284 0
	cmp	x1, 16
	bne	.L66
.LVL106:
	add	x3, x3, 1
.LVL107:
.LBE112:
	.loc 1 277 0
	cmp	x3, 16
	bne	.L67
.LVL108:
	add	x2, x2, 1
.LVL109:
	b	.L64
.LVL110:
.L78:
	.loc 1 273 0
	mov	x0, 0
	b	.L63
.LVL111:
.L69:
.LBE113:
.LBE116:
.LBB117:
.LBB118:
	.loc 2 81 0
	mov	x0, x21
.LVL112:
.L107:
.LBE118:
.LBE117:
.LBE119:
.LBE120:
.LBB121:
.LBB122:
.LBB123:
.LBB124:
	bl	free
.LVL113:
	b	.L76
.L60:
.LBE124:
.LBE123:
.LBE122:
.LBE121:
	.loc 1 507 0
	mov	w2, 3
.LVL114:
.L106:
	ldr	w1, [x20, #:lo12:.LANCHOR2]
	mov	x0, x19
	bl	parse_wf_gray16
.LVL115:
	.loc 1 508 0
	cmp	w0, 0
	csetm	w0, ne
.LVL116:
.L48:
	.loc 1 553 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa 31, 0
	ret
.LVL117:
.L53:
	.cfi_restore_state
	.loc 1 513 0
	mov	w2, 4
	b	.L106
.LVL118:
.L61:
	.loc 1 519 0
	mov	w2, 5
	b	.L106
.LVL119:
.L56:
	.loc 1 525 0
	mov	w2, 2
	b	.L106
.LVL120:
.L57:
.LBB130:
.LBB131:
	.loc 1 376 0
	ldr	w0, [x20, #:lo12:.LANCHOR2]
	mov	w1, 1
	bl	get_wf_buf
.LVL121:
	.loc 1 377 0
	ldrb	w20, [x0]
.LVL122:
	.loc 1 381 0
	lsl	w1, w20, 6
.LVL123:
	bl	decode_wf_data
.LVL124:
	str	x0, [x19, 8]
	.loc 1 382 0
	cbz	x0, .L70
	.loc 1 385 0
	str	w20, [x19]
.LVL125:
.L76:
.LBE131:
.LBE130:
	.loc 1 475 0
	mov	w0, 0
	b	.L48
.LVL126:
.L58:
	.loc 1 537 0
	ldr	w20, [x20, #:lo12:.LANCHOR2]
.LVL127:
.LBB132:
.LBB129:
	.loc 1 398 0
	mov	w1, 2
	mov	w0, w20
	bl	get_wf_buf
.LVL128:
	.loc 1 399 0
	ldrb	w1, [x0]
.LVL129:
	.loc 1 401 0
	str	w1, [x19]
.LVL130:
	.loc 1 405 0
	lsl	w1, w1, 6
.LVL131:
	bl	decode_wf_data
.LVL132:
	str	x0, [x19, 8]
	.loc 1 406 0
	cbz	x0, .L70
	.loc 1 411 0
	mov	w1, 1
	mov	w0, w20
	bl	get_wf_buf
.LVL133:
.LBB125:
.LBB126:
	.loc 1 243 0
	ldrb	w20, [x0]
.LVL134:
.LBE126:
.LBE125:
	.loc 1 414 0
	ldr	w1, [x19]
	orr	w1, w1, w20, lsl 8
	str	w1, [x19]
.LVL135:
	.loc 1 419 0
	lsl	w1, w20, 6
.LVL136:
	bl	decode_wf_data
.LVL137:
	.loc 1 420 0
	cbz	x0, .L70
	ldr	x7, [x19, 8]
.LVL138:
	ubfiz	x20, x20, 6, 8
.LVL139:
	mov	x2, 0
.LVL140:
.L71:
	add	x5, x7, x2
	add	x6, x0, x2
.LVL141:
.LBB127:
.LBB128:
	.loc 1 303 0
	cmp	x2, x20
	beq	.L107
	mov	x1, 0
.LVL142:
.L72:
	.loc 1 305 0
	ldr	w3, [x6, x1]
.LVL143:
	.loc 1 307 0
	ldr	w4, [x5, x1]
.LVL144:
	.loc 1 306 0
	and	w3, w3, -1073741821
.LVL145:
	.loc 1 308 0
	and	w4, w4, 1073741820
.LVL146:
	.loc 1 310 0
	orr	w3, w3, w4
.LVL147:
	str	w3, [x5, x1]
.LVL148:
	add	x1, x1, 4
.LVL149:
	.loc 1 304 0
	cmp	x1, 64
	bne	.L72
	add	x2, x2, 64
	b	.L71
.LVL150:
.L59:
.LBE128:
.LBE127:
.LBE129:
.LBE132:
.LBB133:
.LBB134:
	.loc 1 437 0
	ldr	w0, [x20, #:lo12:.LANCHOR2]
	mov	w1, 6
	bl	get_wf_buf
.LVL151:
	.loc 1 438 0
	ldrb	w1, [x0]
.LVL152:
	.loc 1 440 0
	str	w1, [x19]
.LVL153:
	.loc 1 444 0
	lsl	w1, w1, 6
.LVL154:
	bl	decode_wf_data
.LVL155:
	.loc 1 445 0
	cmp	x0, 0
	.loc 1 444 0
	str	x0, [x19, 8]
	.loc 1 445 0
	csetm	w0, eq
	b	.L48
.LVL156:
.L74:
	.cfi_def_cfa 31, 0
	.cfi_restore 19
	.cfi_restore 20
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 29
	.cfi_restore 30
.LBE134:
.LBE133:
	.loc 1 468 0
	mov	w0, -19
.LVL157:
	ret
.LVL158:
.L75:
	.loc 1 471 0
	mov	w0, -22
.LVL159:
	ret
	.cfi_endproc
.LFE229:
	.size	rkf_wf_get_lut, .-rkf_wf_get_lut
	.section	.bss.crc32_table,"aw",@nobits
	.align	3
	.set	.LANCHOR1,. + 0
	.type	crc32_table, %object
	.size	crc32_table, 1024
crc32_table:
	.zero	1024
	.section	.bss.global_waveform,"aw",@nobits
	.align	3
	.set	.LANCHOR0,. + 0
	.type	global_waveform, %object
	.size	global_waveform, 8
global_waveform:
	.zero	8
	.section	.data.sftemp.6190,"aw",@progbits
	.align	2
	.set	.LANCHOR2,. + 0
	.type	sftemp.6190, %object
	.size	sftemp.6190, 4
sftemp.6190:
	.word	-1
	.section	.data.stype.6189,"aw",@progbits
	.align	2
	.set	.LANCHOR3,. + 0
	.type	stype.6189, %object
	.size	stype.6189, 4
stype.6189:
	.word	13
	.section	.rodata.rkf_wf_input.str1.1,"aMS",@progbits,1
.LC0:
	.string	"rkf waveform"
.LC1:
	.string	"rkf: check format failed\n"
.LC2:
	.string	"rkf: failed to check RKF file format\n"
.LC3:
	.string	"[EINK]: waveform crc err readcrc = %x crccheck = %x\n"
.LC4:
	.string	"rkf: failed to check crc RKF waveform\n"
.LC5:
	.string	"rkf file version: %s\n"
	.text
.Letext0:
	.file 3 "include/common.h"
	.file 4 "./arch/arm/include/asm/types.h"
	.file 5 "./arch/arm/include/asm/posix_types.h"
	.file 6 "include/linux/types.h"
	.file 7 "include/errno.h"
	.file 8 "include/linux/string.h"
	.file 9 "include/efi.h"
	.file 10 "include/ide.h"
	.file 11 "include/part.h"
	.file 12 "include/flash.h"
	.file 13 "include/lmb.h"
	.file 14 "include/asm-generic/u-boot.h"
	.file 15 "./arch/arm/include/asm/u-boot-arm.h"
	.file 16 "include/linux/libfdt_env.h"
	.file 17 "include/linux/../../scripts/dtc/libfdt/fdt.h"
	.file 18 "include/linux/libfdt.h"
	.file 19 "include/image.h"
	.file 20 "include/net.h"
	.file 21 "include/dm/uclass-id.h"
	.file 22 "include/malloc.h"
	.file 23 "drivers/video/rk_eink/epdlut/epd_lut.h"
	.file 24 "include/stdio.h"
	.file 25 "include/log.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x1d7c
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.4byte	.LASF390
	.byte	0xc
	.4byte	.LASF391
	.4byte	.LASF392
	.4byte	.Ldebug_ranges0+0x210
	.8byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.4byte	.LASF4
	.byte	0x3
	.byte	0xd
	.4byte	0x34
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF0
	.uleb128 0x4
	.4byte	0x34
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.4byte	.LASF1
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.4byte	.LASF2
	.uleb128 0x5
	.4byte	.LASF23
	.byte	0x7
	.byte	0xc
	.4byte	0x59
	.uleb128 0x6
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.4byte	.LASF3
	.uleb128 0x2
	.4byte	.LASF5
	.byte	0x4
	.byte	0xc
	.4byte	0x34
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.4byte	.LASF6
	.uleb128 0x2
	.4byte	.LASF7
	.byte	0x4
	.byte	0x12
	.4byte	0x84
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.4byte	.LASF9
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.4byte	.LASF10
	.uleb128 0x7
	.string	"u8"
	.byte	0x4
	.byte	0x1f
	.4byte	0x34
	.uleb128 0x4
	.4byte	0x99
	.uleb128 0x7
	.string	"u32"
	.byte	0x4
	.byte	0x25
	.4byte	0x84
	.uleb128 0x7
	.string	"u64"
	.byte	0x4
	.byte	0x28
	.4byte	0x92
	.uleb128 0x2
	.4byte	.LASF11
	.byte	0x4
	.byte	0x31
	.4byte	0x92
	.uleb128 0x2
	.4byte	.LASF12
	.byte	0x4
	.byte	0x32
	.4byte	0x92
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.4byte	.LASF13
	.uleb128 0x8
	.byte	0x8
	.4byte	0xe8
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF14
	.uleb128 0x4
	.4byte	0xe1
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.4byte	.LASF15
	.uleb128 0x2
	.4byte	.LASF16
	.byte	0x5
	.byte	0x21
	.4byte	0x40
	.uleb128 0x8
	.byte	0x8
	.4byte	0xe1
	.uleb128 0x2
	.4byte	.LASF17
	.byte	0x6
	.byte	0x35
	.4byte	0xf4
	.uleb128 0x2
	.4byte	.LASF18
	.byte	0x6
	.byte	0x59
	.4byte	0x47
	.uleb128 0x2
	.4byte	.LASF19
	.byte	0x6
	.byte	0x5b
	.4byte	0x40
	.uleb128 0x2
	.4byte	.LASF20
	.byte	0x6
	.byte	0x69
	.4byte	0x67
	.uleb128 0x2
	.4byte	.LASF21
	.byte	0x6
	.byte	0x97
	.4byte	0x79
	.uleb128 0x2
	.4byte	.LASF22
	.byte	0x6
	.byte	0x9f
	.4byte	0x84
	.uleb128 0x9
	.byte	0x8
	.uleb128 0x5
	.4byte	.LASF24
	.byte	0x8
	.byte	0xb
	.4byte	0xff
	.uleb128 0x3
	.byte	0x1
	.byte	0x2
	.4byte	.LASF25
	.uleb128 0xa
	.4byte	0xe1
	.4byte	0x166
	.uleb128 0xb
	.byte	0
	.uleb128 0xc
	.4byte	.LASF26
	.byte	0x9
	.2byte	0x140
	.4byte	0x15b
	.uleb128 0xc
	.4byte	.LASF27
	.byte	0x9
	.2byte	0x143
	.4byte	0x15b
	.uleb128 0xc
	.4byte	.LASF28
	.byte	0x9
	.2byte	0x143
	.4byte	0x15b
	.uleb128 0x8
	.byte	0x8
	.4byte	0x190
	.uleb128 0xd
	.uleb128 0xa
	.4byte	0x11b
	.4byte	0x19c
	.uleb128 0xb
	.byte	0
	.uleb128 0x5
	.4byte	.LASF29
	.byte	0xa
	.byte	0x10
	.4byte	0x191
	.uleb128 0xa
	.4byte	0x34
	.4byte	0x1b7
	.uleb128 0xe
	.4byte	0xd4
	.byte	0x5
	.byte	0
	.uleb128 0xf
	.4byte	.LASF32
	.byte	0x10
	.byte	0xb
	.byte	0xf
	.4byte	0x1dc
	.uleb128 0x10
	.4byte	.LASF30
	.byte	0xb
	.byte	0x10
	.4byte	0xff
	.byte	0
	.uleb128 0x10
	.4byte	.LASF31
	.byte	0xb
	.byte	0x11
	.4byte	0x1f5
	.byte	0x8
	.byte	0
	.uleb128 0x4
	.4byte	0x1b7
	.uleb128 0x11
	.4byte	0x59
	.4byte	0x1f5
	.uleb128 0x12
	.4byte	0x59
	.uleb128 0x12
	.4byte	0x59
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.4byte	0x1e1
	.uleb128 0xa
	.4byte	0x1dc
	.4byte	0x206
	.uleb128 0xb
	.byte	0
	.uleb128 0x4
	.4byte	0x1fb
	.uleb128 0x5
	.4byte	.LASF32
	.byte	0xb
	.byte	0xdf
	.4byte	0x206
	.uleb128 0x13
	.2byte	0x1220
	.byte	0xc
	.byte	0x13
	.4byte	0x26a
	.uleb128 0x10
	.4byte	.LASF33
	.byte	0xc
	.byte	0x14
	.4byte	0x11b
	.byte	0
	.uleb128 0x10
	.4byte	.LASF34
	.byte	0xc
	.byte	0x15
	.4byte	0x110
	.byte	0x8
	.uleb128 0x10
	.4byte	.LASF35
	.byte	0xc
	.byte	0x16
	.4byte	0x11b
	.byte	0x10
	.uleb128 0x10
	.4byte	.LASF36
	.byte	0xc
	.byte	0x17
	.4byte	0x26a
	.byte	0x18
	.uleb128 0x14
	.4byte	.LASF37
	.byte	0xc
	.byte	0x18
	.4byte	0x27b
	.2byte	0x1018
	.uleb128 0x15
	.string	"mtd"
	.byte	0xc
	.byte	0x31
	.4byte	0x291
	.2byte	0x1218
	.byte	0
	.uleb128 0xa
	.4byte	0x11b
	.4byte	0x27b
	.uleb128 0x16
	.4byte	0xd4
	.2byte	0x1ff
	.byte	0
	.uleb128 0xa
	.4byte	0x29
	.4byte	0x28c
	.uleb128 0x16
	.4byte	0xd4
	.2byte	0x1ff
	.byte	0
	.uleb128 0x17
	.4byte	.LASF393
	.uleb128 0x8
	.byte	0x8
	.4byte	0x28c
	.uleb128 0x2
	.4byte	.LASF38
	.byte	0xc
	.byte	0x37
	.4byte	0x216
	.uleb128 0xa
	.4byte	0x297
	.4byte	0x2ad
	.uleb128 0xb
	.byte	0
	.uleb128 0x5
	.4byte	.LASF39
	.byte	0xc
	.byte	0x39
	.4byte	0x2a2
	.uleb128 0x3
	.byte	0x10
	.byte	0x4
	.4byte	.LASF40
	.uleb128 0xf
	.4byte	.LASF41
	.byte	0x10
	.byte	0xd
	.byte	0x10
	.4byte	0x2e4
	.uleb128 0x10
	.4byte	.LASF42
	.byte	0xd
	.byte	0x11
	.4byte	0xbe
	.byte	0
	.uleb128 0x10
	.4byte	.LASF33
	.byte	0xd
	.byte	0x12
	.4byte	0xc9
	.byte	0x8
	.byte	0
	.uleb128 0x18
	.4byte	.LASF43
	.2byte	0x120
	.byte	0xd
	.byte	0x15
	.4byte	0x316
	.uleb128 0x19
	.string	"cnt"
	.byte	0xd
	.byte	0x16
	.4byte	0x40
	.byte	0
	.uleb128 0x10
	.4byte	.LASF33
	.byte	0xd
	.byte	0x17
	.4byte	0xc9
	.byte	0x8
	.uleb128 0x10
	.4byte	.LASF44
	.byte	0xd
	.byte	0x18
	.4byte	0x316
	.byte	0x10
	.byte	0
	.uleb128 0xa
	.4byte	0x2bf
	.4byte	0x326
	.uleb128 0xe
	.4byte	0xd4
	.byte	0x10
	.byte	0
	.uleb128 0x1a
	.string	"lmb"
	.2byte	0x240
	.byte	0xd
	.byte	0x1b
	.4byte	0x34d
	.uleb128 0x10
	.4byte	.LASF45
	.byte	0xd
	.byte	0x1c
	.4byte	0x2e4
	.byte	0
	.uleb128 0x14
	.4byte	.LASF46
	.byte	0xd
	.byte	0x1d
	.4byte	0x2e4
	.2byte	0x120
	.byte	0
	.uleb128 0x1b
	.string	"lmb"
	.byte	0xd
	.byte	0x20
	.4byte	0x326
	.uleb128 0x1c
	.byte	0x10
	.byte	0xe
	.byte	0x5d
	.4byte	0x379
	.uleb128 0x10
	.4byte	.LASF36
	.byte	0xe
	.byte	0x5e
	.4byte	0xb3
	.byte	0
	.uleb128 0x10
	.4byte	.LASF33
	.byte	0xe
	.byte	0x5f
	.4byte	0xb3
	.byte	0x8
	.byte	0
	.uleb128 0x18
	.4byte	.LASF47
	.2byte	0x150
	.byte	0xe
	.byte	0x1b
	.4byte	0x477
	.uleb128 0x10
	.4byte	.LASF48
	.byte	0xe
	.byte	0x1c
	.4byte	0x40
	.byte	0
	.uleb128 0x10
	.4byte	.LASF49
	.byte	0xe
	.byte	0x1d
	.4byte	0xc9
	.byte	0x8
	.uleb128 0x10
	.4byte	.LASF50
	.byte	0xe
	.byte	0x1e
	.4byte	0x40
	.byte	0x10
	.uleb128 0x10
	.4byte	.LASF51
	.byte	0xe
	.byte	0x1f
	.4byte	0x40
	.byte	0x18
	.uleb128 0x10
	.4byte	.LASF52
	.byte	0xe
	.byte	0x20
	.4byte	0x40
	.byte	0x20
	.uleb128 0x10
	.4byte	.LASF53
	.byte	0xe
	.byte	0x21
	.4byte	0x40
	.byte	0x28
	.uleb128 0x10
	.4byte	.LASF54
	.byte	0xe
	.byte	0x22
	.4byte	0x40
	.byte	0x30
	.uleb128 0x10
	.4byte	.LASF55
	.byte	0xe
	.byte	0x24
	.4byte	0x40
	.byte	0x38
	.uleb128 0x10
	.4byte	.LASF56
	.byte	0xe
	.byte	0x25
	.4byte	0x40
	.byte	0x40
	.uleb128 0x10
	.4byte	.LASF57
	.byte	0xe
	.byte	0x26
	.4byte	0x40
	.byte	0x48
	.uleb128 0x10
	.4byte	.LASF58
	.byte	0xe
	.byte	0x31
	.4byte	0x40
	.byte	0x50
	.uleb128 0x10
	.4byte	.LASF59
	.byte	0xe
	.byte	0x32
	.4byte	0x40
	.byte	0x58
	.uleb128 0x10
	.4byte	.LASF60
	.byte	0xe
	.byte	0x33
	.4byte	0x1a7
	.byte	0x60
	.uleb128 0x10
	.4byte	.LASF61
	.byte	0xe
	.byte	0x34
	.4byte	0x47
	.byte	0x66
	.uleb128 0x10
	.4byte	.LASF62
	.byte	0xe
	.byte	0x35
	.4byte	0x40
	.byte	0x68
	.uleb128 0x10
	.4byte	.LASF63
	.byte	0xe
	.byte	0x36
	.4byte	0x40
	.byte	0x70
	.uleb128 0x10
	.4byte	.LASF64
	.byte	0xe
	.byte	0x57
	.4byte	0x11b
	.byte	0x78
	.uleb128 0x10
	.4byte	.LASF65
	.byte	0xe
	.byte	0x58
	.4byte	0x11b
	.byte	0x80
	.uleb128 0x10
	.4byte	.LASF66
	.byte	0xe
	.byte	0x5b
	.4byte	0x84
	.byte	0x88
	.uleb128 0x10
	.4byte	.LASF67
	.byte	0xe
	.byte	0x60
	.4byte	0x477
	.byte	0x90
	.byte	0
	.uleb128 0xa
	.4byte	0x358
	.4byte	0x487
	.uleb128 0xe
	.4byte	0xd4
	.byte	0xb
	.byte	0
	.uleb128 0x2
	.4byte	.LASF68
	.byte	0xe
	.byte	0x62
	.4byte	0x379
	.uleb128 0x5
	.4byte	.LASF69
	.byte	0xf
	.byte	0x13
	.4byte	0x11b
	.uleb128 0x5
	.4byte	.LASF70
	.byte	0xf
	.byte	0x14
	.4byte	0x11b
	.uleb128 0x5
	.4byte	.LASF71
	.byte	0xf
	.byte	0x15
	.4byte	0x11b
	.uleb128 0x5
	.4byte	.LASF72
	.byte	0xf
	.byte	0x16
	.4byte	0x11b
	.uleb128 0x5
	.4byte	.LASF73
	.byte	0xf
	.byte	0x17
	.4byte	0x11b
	.uleb128 0x5
	.4byte	.LASF74
	.byte	0xf
	.byte	0x18
	.4byte	0x11b
	.uleb128 0x5
	.4byte	.LASF75
	.byte	0xf
	.byte	0x19
	.4byte	0x11b
	.uleb128 0x8
	.byte	0x8
	.4byte	0x3b
	.uleb128 0x8
	.byte	0x8
	.4byte	0x34
	.uleb128 0x2
	.4byte	.LASF76
	.byte	0x10
	.byte	0x11
	.4byte	0x131
	.uleb128 0xf
	.4byte	.LASF77
	.byte	0x28
	.byte	0x11
	.byte	0x39
	.4byte	0x57b
	.uleb128 0x10
	.4byte	.LASF78
	.byte	0x11
	.byte	0x3a
	.4byte	0x4eb
	.byte	0
	.uleb128 0x10
	.4byte	.LASF79
	.byte	0x11
	.byte	0x3b
	.4byte	0x4eb
	.byte	0x4
	.uleb128 0x10
	.4byte	.LASF80
	.byte	0x11
	.byte	0x3c
	.4byte	0x4eb
	.byte	0x8
	.uleb128 0x10
	.4byte	.LASF81
	.byte	0x11
	.byte	0x3d
	.4byte	0x4eb
	.byte	0xc
	.uleb128 0x10
	.4byte	.LASF82
	.byte	0x11
	.byte	0x3e
	.4byte	0x4eb
	.byte	0x10
	.uleb128 0x10
	.4byte	.LASF83
	.byte	0x11
	.byte	0x3f
	.4byte	0x4eb
	.byte	0x14
	.uleb128 0x10
	.4byte	.LASF84
	.byte	0x11
	.byte	0x40
	.4byte	0x4eb
	.byte	0x18
	.uleb128 0x10
	.4byte	.LASF85
	.byte	0x11
	.byte	0x43
	.4byte	0x4eb
	.byte	0x1c
	.uleb128 0x10
	.4byte	.LASF86
	.byte	0x11
	.byte	0x46
	.4byte	0x4eb
	.byte	0x20
	.uleb128 0x10
	.4byte	.LASF87
	.byte	0x11
	.byte	0x49
	.4byte	0x4eb
	.byte	0x24
	.byte	0
	.uleb128 0xc
	.4byte	.LASF88
	.byte	0x12
	.2byte	0x136
	.4byte	0x587
	.uleb128 0x8
	.byte	0x8
	.4byte	0x4f6
	.uleb128 0x1d
	.4byte	.LASF89
	.byte	0x40
	.byte	0x13
	.2byte	0x137
	.4byte	0x637
	.uleb128 0x1e
	.4byte	.LASF90
	.byte	0x13
	.2byte	0x138
	.4byte	0x131
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF91
	.byte	0x13
	.2byte	0x139
	.4byte	0x131
	.byte	0x4
	.uleb128 0x1e
	.4byte	.LASF92
	.byte	0x13
	.2byte	0x13a
	.4byte	0x131
	.byte	0x8
	.uleb128 0x1e
	.4byte	.LASF93
	.byte	0x13
	.2byte	0x13b
	.4byte	0x131
	.byte	0xc
	.uleb128 0x1e
	.4byte	.LASF94
	.byte	0x13
	.2byte	0x13c
	.4byte	0x131
	.byte	0x10
	.uleb128 0x1e
	.4byte	.LASF95
	.byte	0x13
	.2byte	0x13d
	.4byte	0x131
	.byte	0x14
	.uleb128 0x1e
	.4byte	.LASF96
	.byte	0x13
	.2byte	0x13e
	.4byte	0x131
	.byte	0x18
	.uleb128 0x1e
	.4byte	.LASF97
	.byte	0x13
	.2byte	0x13f
	.4byte	0x126
	.byte	0x1c
	.uleb128 0x1e
	.4byte	.LASF98
	.byte	0x13
	.2byte	0x140
	.4byte	0x126
	.byte	0x1d
	.uleb128 0x1e
	.4byte	.LASF99
	.byte	0x13
	.2byte	0x141
	.4byte	0x126
	.byte	0x1e
	.uleb128 0x1e
	.4byte	.LASF100
	.byte	0x13
	.2byte	0x142
	.4byte	0x126
	.byte	0x1f
	.uleb128 0x1e
	.4byte	.LASF101
	.byte	0x13
	.2byte	0x143
	.4byte	0x637
	.byte	0x20
	.byte	0
	.uleb128 0xa
	.4byte	0x126
	.4byte	0x647
	.uleb128 0xe
	.4byte	0xd4
	.byte	0x1f
	.byte	0
	.uleb128 0x1f
	.4byte	.LASF102
	.byte	0x13
	.2byte	0x144
	.4byte	0x58d
	.uleb128 0x1d
	.4byte	.LASF103
	.byte	0x30
	.byte	0x13
	.2byte	0x146
	.4byte	0x6d5
	.uleb128 0x1e
	.4byte	.LASF36
	.byte	0x13
	.2byte	0x147
	.4byte	0x11b
	.byte	0
	.uleb128 0x20
	.string	"end"
	.byte	0x13
	.2byte	0x147
	.4byte	0x11b
	.byte	0x8
	.uleb128 0x1e
	.4byte	.LASF104
	.byte	0x13
	.2byte	0x148
	.4byte	0x11b
	.byte	0x10
	.uleb128 0x1e
	.4byte	.LASF105
	.byte	0x13
	.2byte	0x148
	.4byte	0x11b
	.byte	0x18
	.uleb128 0x1e
	.4byte	.LASF106
	.byte	0x13
	.2byte	0x149
	.4byte	0x11b
	.byte	0x20
	.uleb128 0x1e
	.4byte	.LASF107
	.byte	0x13
	.2byte	0x14a
	.4byte	0x126
	.byte	0x28
	.uleb128 0x1e
	.4byte	.LASF108
	.byte	0x13
	.2byte	0x14a
	.4byte	0x126
	.byte	0x29
	.uleb128 0x20
	.string	"os"
	.byte	0x13
	.2byte	0x14a
	.4byte	0x126
	.byte	0x2a
	.uleb128 0x1e
	.4byte	.LASF109
	.byte	0x13
	.2byte	0x14b
	.4byte	0x126
	.byte	0x2b
	.byte	0
	.uleb128 0x1f
	.4byte	.LASF110
	.byte	0x13
	.2byte	0x14c
	.4byte	0x653
	.uleb128 0x21
	.4byte	.LASF111
	.2byte	0x380
	.byte	0x13
	.2byte	0x152
	.4byte	0x87e
	.uleb128 0x1e
	.4byte	.LASF112
	.byte	0x13
	.2byte	0x158
	.4byte	0x87e
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF113
	.byte	0x13
	.2byte	0x159
	.4byte	0x647
	.byte	0x8
	.uleb128 0x1e
	.4byte	.LASF114
	.byte	0x13
	.2byte	0x15a
	.4byte	0x11b
	.byte	0x48
	.uleb128 0x1e
	.4byte	.LASF115
	.byte	0x13
	.2byte	0x15d
	.4byte	0xdb
	.byte	0x50
	.uleb128 0x1e
	.4byte	.LASF116
	.byte	0x13
	.2byte	0x15f
	.4byte	0x147
	.byte	0x58
	.uleb128 0x1e
	.4byte	.LASF117
	.byte	0x13
	.2byte	0x160
	.4byte	0xdb
	.byte	0x60
	.uleb128 0x1e
	.4byte	.LASF118
	.byte	0x13
	.2byte	0x161
	.4byte	0x59
	.byte	0x68
	.uleb128 0x1e
	.4byte	.LASF119
	.byte	0x13
	.2byte	0x163
	.4byte	0x147
	.byte	0x70
	.uleb128 0x1e
	.4byte	.LASF120
	.byte	0x13
	.2byte	0x164
	.4byte	0xdb
	.byte	0x78
	.uleb128 0x1e
	.4byte	.LASF121
	.byte	0x13
	.2byte	0x165
	.4byte	0x59
	.byte	0x80
	.uleb128 0x1e
	.4byte	.LASF122
	.byte	0x13
	.2byte	0x167
	.4byte	0x147
	.byte	0x88
	.uleb128 0x1e
	.4byte	.LASF123
	.byte	0x13
	.2byte	0x168
	.4byte	0xdb
	.byte	0x90
	.uleb128 0x1e
	.4byte	.LASF124
	.byte	0x13
	.2byte	0x169
	.4byte	0x59
	.byte	0x98
	.uleb128 0x1e
	.4byte	.LASF125
	.byte	0x13
	.2byte	0x16b
	.4byte	0x147
	.byte	0xa0
	.uleb128 0x1e
	.4byte	.LASF126
	.byte	0x13
	.2byte	0x16c
	.4byte	0xdb
	.byte	0xa8
	.uleb128 0x1e
	.4byte	.LASF127
	.byte	0x13
	.2byte	0x16d
	.4byte	0x59
	.byte	0xb0
	.uleb128 0x20
	.string	"os"
	.byte	0x13
	.2byte	0x171
	.4byte	0x6d5
	.byte	0xb8
	.uleb128 0x20
	.string	"ep"
	.byte	0x13
	.2byte	0x172
	.4byte	0x11b
	.byte	0xe8
	.uleb128 0x1e
	.4byte	.LASF128
	.byte	0x13
	.2byte	0x174
	.4byte	0x11b
	.byte	0xf0
	.uleb128 0x1e
	.4byte	.LASF129
	.byte	0x13
	.2byte	0x174
	.4byte	0x11b
	.byte	0xf8
	.uleb128 0x22
	.4byte	.LASF130
	.byte	0x13
	.2byte	0x176
	.4byte	0xff
	.2byte	0x100
	.uleb128 0x22
	.4byte	.LASF131
	.byte	0x13
	.2byte	0x177
	.4byte	0x11b
	.2byte	0x108
	.uleb128 0x22
	.4byte	.LASF132
	.byte	0x13
	.2byte	0x179
	.4byte	0x11b
	.2byte	0x110
	.uleb128 0x22
	.4byte	.LASF133
	.byte	0x13
	.2byte	0x17a
	.4byte	0x11b
	.2byte	0x118
	.uleb128 0x22
	.4byte	.LASF134
	.byte	0x13
	.2byte	0x17b
	.4byte	0x11b
	.2byte	0x120
	.uleb128 0x22
	.4byte	.LASF135
	.byte	0x13
	.2byte	0x17c
	.4byte	0x11b
	.2byte	0x128
	.uleb128 0x23
	.string	"kbd"
	.byte	0x13
	.2byte	0x17d
	.4byte	0x884
	.2byte	0x130
	.uleb128 0x22
	.4byte	.LASF136
	.byte	0x13
	.2byte	0x180
	.4byte	0x59
	.2byte	0x138
	.uleb128 0x22
	.4byte	.LASF137
	.byte	0x13
	.2byte	0x18d
	.4byte	0x59
	.2byte	0x13c
	.uleb128 0x23
	.string	"lmb"
	.byte	0x13
	.2byte	0x190
	.4byte	0x326
	.2byte	0x140
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.4byte	0x647
	.uleb128 0x8
	.byte	0x8
	.4byte	0x487
	.uleb128 0x1f
	.4byte	.LASF138
	.byte	0x13
	.2byte	0x196
	.4byte	0x6e1
	.uleb128 0xc
	.4byte	.LASF139
	.byte	0x13
	.2byte	0x198
	.4byte	0x88a
	.uleb128 0x24
	.4byte	.LASF271
	.byte	0x4
	.4byte	0x59
	.byte	0x15
	.byte	0xe
	.4byte	0xb05
	.uleb128 0x25
	.4byte	.LASF140
	.byte	0
	.uleb128 0x25
	.4byte	.LASF141
	.byte	0x1
	.uleb128 0x25
	.4byte	.LASF142
	.byte	0x2
	.uleb128 0x25
	.4byte	.LASF143
	.byte	0x3
	.uleb128 0x25
	.4byte	.LASF144
	.byte	0x4
	.uleb128 0x25
	.4byte	.LASF145
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF146
	.byte	0x6
	.uleb128 0x25
	.4byte	.LASF147
	.byte	0x7
	.uleb128 0x25
	.4byte	.LASF148
	.byte	0x8
	.uleb128 0x25
	.4byte	.LASF149
	.byte	0x9
	.uleb128 0x25
	.4byte	.LASF150
	.byte	0xa
	.uleb128 0x25
	.4byte	.LASF151
	.byte	0xb
	.uleb128 0x25
	.4byte	.LASF152
	.byte	0xc
	.uleb128 0x25
	.4byte	.LASF153
	.byte	0xd
	.uleb128 0x25
	.4byte	.LASF154
	.byte	0xe
	.uleb128 0x25
	.4byte	.LASF155
	.byte	0xf
	.uleb128 0x25
	.4byte	.LASF156
	.byte	0x10
	.uleb128 0x25
	.4byte	.LASF157
	.byte	0x11
	.uleb128 0x25
	.4byte	.LASF158
	.byte	0x12
	.uleb128 0x25
	.4byte	.LASF159
	.byte	0x13
	.uleb128 0x25
	.4byte	.LASF160
	.byte	0x14
	.uleb128 0x25
	.4byte	.LASF161
	.byte	0x15
	.uleb128 0x25
	.4byte	.LASF162
	.byte	0x16
	.uleb128 0x25
	.4byte	.LASF163
	.byte	0x17
	.uleb128 0x25
	.4byte	.LASF164
	.byte	0x18
	.uleb128 0x25
	.4byte	.LASF165
	.byte	0x19
	.uleb128 0x25
	.4byte	.LASF166
	.byte	0x1a
	.uleb128 0x25
	.4byte	.LASF167
	.byte	0x1b
	.uleb128 0x25
	.4byte	.LASF168
	.byte	0x1c
	.uleb128 0x25
	.4byte	.LASF169
	.byte	0x1d
	.uleb128 0x25
	.4byte	.LASF170
	.byte	0x1e
	.uleb128 0x25
	.4byte	.LASF171
	.byte	0x1f
	.uleb128 0x25
	.4byte	.LASF172
	.byte	0x20
	.uleb128 0x25
	.4byte	.LASF173
	.byte	0x21
	.uleb128 0x25
	.4byte	.LASF174
	.byte	0x22
	.uleb128 0x25
	.4byte	.LASF175
	.byte	0x23
	.uleb128 0x25
	.4byte	.LASF176
	.byte	0x24
	.uleb128 0x25
	.4byte	.LASF177
	.byte	0x25
	.uleb128 0x25
	.4byte	.LASF178
	.byte	0x26
	.uleb128 0x25
	.4byte	.LASF179
	.byte	0x27
	.uleb128 0x25
	.4byte	.LASF180
	.byte	0x28
	.uleb128 0x25
	.4byte	.LASF181
	.byte	0x29
	.uleb128 0x25
	.4byte	.LASF182
	.byte	0x2a
	.uleb128 0x25
	.4byte	.LASF183
	.byte	0x2b
	.uleb128 0x25
	.4byte	.LASF184
	.byte	0x2c
	.uleb128 0x25
	.4byte	.LASF185
	.byte	0x2d
	.uleb128 0x25
	.4byte	.LASF186
	.byte	0x2e
	.uleb128 0x25
	.4byte	.LASF187
	.byte	0x2f
	.uleb128 0x25
	.4byte	.LASF188
	.byte	0x30
	.uleb128 0x25
	.4byte	.LASF189
	.byte	0x31
	.uleb128 0x25
	.4byte	.LASF190
	.byte	0x32
	.uleb128 0x25
	.4byte	.LASF191
	.byte	0x33
	.uleb128 0x25
	.4byte	.LASF192
	.byte	0x34
	.uleb128 0x25
	.4byte	.LASF193
	.byte	0x35
	.uleb128 0x25
	.4byte	.LASF194
	.byte	0x36
	.uleb128 0x25
	.4byte	.LASF195
	.byte	0x37
	.uleb128 0x25
	.4byte	.LASF196
	.byte	0x38
	.uleb128 0x25
	.4byte	.LASF197
	.byte	0x39
	.uleb128 0x25
	.4byte	.LASF198
	.byte	0x3a
	.uleb128 0x25
	.4byte	.LASF199
	.byte	0x3b
	.uleb128 0x25
	.4byte	.LASF200
	.byte	0x3c
	.uleb128 0x25
	.4byte	.LASF201
	.byte	0x3d
	.uleb128 0x25
	.4byte	.LASF202
	.byte	0x3e
	.uleb128 0x25
	.4byte	.LASF203
	.byte	0x3f
	.uleb128 0x25
	.4byte	.LASF204
	.byte	0x40
	.uleb128 0x25
	.4byte	.LASF205
	.byte	0x41
	.uleb128 0x25
	.4byte	.LASF206
	.byte	0x42
	.uleb128 0x25
	.4byte	.LASF207
	.byte	0x43
	.uleb128 0x25
	.4byte	.LASF208
	.byte	0x44
	.uleb128 0x25
	.4byte	.LASF209
	.byte	0x45
	.uleb128 0x25
	.4byte	.LASF210
	.byte	0x46
	.uleb128 0x25
	.4byte	.LASF211
	.byte	0x47
	.uleb128 0x25
	.4byte	.LASF212
	.byte	0x48
	.uleb128 0x25
	.4byte	.LASF213
	.byte	0x49
	.uleb128 0x25
	.4byte	.LASF214
	.byte	0x4a
	.uleb128 0x25
	.4byte	.LASF215
	.byte	0x4b
	.uleb128 0x25
	.4byte	.LASF216
	.byte	0x4c
	.uleb128 0x25
	.4byte	.LASF217
	.byte	0x4d
	.uleb128 0x25
	.4byte	.LASF218
	.byte	0x4e
	.uleb128 0x25
	.4byte	.LASF219
	.byte	0x4f
	.uleb128 0x25
	.4byte	.LASF220
	.byte	0x50
	.uleb128 0x25
	.4byte	.LASF221
	.byte	0x51
	.uleb128 0x25
	.4byte	.LASF222
	.byte	0x52
	.uleb128 0x25
	.4byte	.LASF223
	.byte	0x53
	.uleb128 0x25
	.4byte	.LASF224
	.byte	0x54
	.uleb128 0x25
	.4byte	.LASF225
	.byte	0x55
	.uleb128 0x25
	.4byte	.LASF226
	.byte	0x56
	.uleb128 0x25
	.4byte	.LASF227
	.byte	0x57
	.uleb128 0x25
	.4byte	.LASF228
	.byte	0x58
	.uleb128 0x25
	.4byte	.LASF229
	.byte	0x59
	.uleb128 0x25
	.4byte	.LASF230
	.byte	0x5a
	.uleb128 0x25
	.4byte	.LASF231
	.byte	0x5b
	.uleb128 0x25
	.4byte	.LASF232
	.byte	0x5c
	.uleb128 0x25
	.4byte	.LASF233
	.byte	0x5d
	.uleb128 0x25
	.4byte	.LASF234
	.byte	0x5e
	.uleb128 0x25
	.4byte	.LASF235
	.byte	0x5f
	.uleb128 0x25
	.4byte	.LASF236
	.byte	0x60
	.uleb128 0x25
	.4byte	.LASF237
	.byte	0x61
	.uleb128 0x26
	.4byte	.LASF238
	.sleb128 -1
	.byte	0
	.uleb128 0x27
	.byte	0x4
	.4byte	0x84
	.byte	0x19
	.byte	0xf6
	.4byte	0xb18
	.uleb128 0x25
	.4byte	.LASF239
	.byte	0x5
	.byte	0
	.uleb128 0xa
	.4byte	0xe1
	.4byte	0xb28
	.uleb128 0xe
	.4byte	0xd4
	.byte	0x1f
	.byte	0
	.uleb128 0x5
	.4byte	.LASF240
	.byte	0x3
	.byte	0xad
	.4byte	0x11b
	.uleb128 0xa
	.4byte	0x99
	.4byte	0xb3e
	.uleb128 0xb
	.byte	0
	.uleb128 0x5
	.4byte	.LASF241
	.byte	0x3
	.byte	0xaf
	.4byte	0xb33
	.uleb128 0x5
	.4byte	.LASF242
	.byte	0x3
	.byte	0xb0
	.4byte	0xb33
	.uleb128 0x5
	.4byte	.LASF243
	.byte	0x3
	.byte	0xfe
	.4byte	0x11b
	.uleb128 0x5
	.4byte	.LASF244
	.byte	0x3
	.byte	0xff
	.4byte	0x11b
	.uleb128 0xc
	.4byte	.LASF245
	.byte	0x3
	.2byte	0x100
	.4byte	0x11b
	.uleb128 0xf
	.4byte	.LASF246
	.byte	0x4
	.byte	0x14
	.byte	0x2e
	.4byte	0xb8f
	.uleb128 0x10
	.4byte	.LASF247
	.byte	0x14
	.byte	0x2f
	.4byte	0x131
	.byte	0
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.4byte	0x29
	.uleb128 0xa
	.4byte	0x99
	.4byte	0xba5
	.uleb128 0xe
	.4byte	0xd4
	.byte	0x5
	.byte	0
	.uleb128 0xc
	.4byte	.LASF248
	.byte	0x14
	.2byte	0x1fd
	.4byte	0xb76
	.uleb128 0xc
	.4byte	.LASF249
	.byte	0x14
	.2byte	0x1fe
	.4byte	0xb76
	.uleb128 0xc
	.4byte	.LASF250
	.byte	0x14
	.2byte	0x200
	.4byte	0xb76
	.uleb128 0xc
	.4byte	.LASF251
	.byte	0x14
	.2byte	0x205
	.4byte	0xb18
	.uleb128 0xc
	.4byte	.LASF252
	.byte	0x14
	.2byte	0x206
	.4byte	0xb18
	.uleb128 0xa
	.4byte	0xe1
	.4byte	0xbf1
	.uleb128 0xe
	.4byte	0xd4
	.byte	0x3f
	.byte	0
	.uleb128 0xc
	.4byte	.LASF253
	.byte	0x14
	.2byte	0x207
	.4byte	0xbe1
	.uleb128 0xc
	.4byte	.LASF254
	.byte	0x14
	.2byte	0x209
	.4byte	0xb95
	.uleb128 0xc
	.4byte	.LASF255
	.byte	0x14
	.2byte	0x20a
	.4byte	0xb95
	.uleb128 0xc
	.4byte	.LASF256
	.byte	0x14
	.2byte	0x20b
	.4byte	0xb76
	.uleb128 0xc
	.4byte	.LASF257
	.byte	0x14
	.2byte	0x20c
	.4byte	0xb76
	.uleb128 0xc
	.4byte	.LASF258
	.byte	0x14
	.2byte	0x20d
	.4byte	0xb8f
	.uleb128 0xa
	.4byte	0xb8f
	.4byte	0xc49
	.uleb128 0xe
	.4byte	0xd4
	.byte	0x3
	.byte	0
	.uleb128 0xc
	.4byte	.LASF259
	.byte	0x14
	.2byte	0x20e
	.4byte	0xc39
	.uleb128 0xc
	.4byte	.LASF260
	.byte	0x14
	.2byte	0x20f
	.4byte	0xb8f
	.uleb128 0xc
	.4byte	.LASF261
	.byte	0x14
	.2byte	0x210
	.4byte	0x59
	.uleb128 0xa
	.4byte	0xa3
	.4byte	0xc7d
	.uleb128 0xe
	.4byte	0xd4
	.byte	0x5
	.byte	0
	.uleb128 0x4
	.4byte	0xc6d
	.uleb128 0xc
	.4byte	.LASF262
	.byte	0x14
	.2byte	0x211
	.4byte	0xc7d
	.uleb128 0xc
	.4byte	.LASF263
	.byte	0x14
	.2byte	0x212
	.4byte	0xc7d
	.uleb128 0xc
	.4byte	.LASF264
	.byte	0x14
	.2byte	0x216
	.4byte	0x110
	.uleb128 0xc
	.4byte	.LASF265
	.byte	0x14
	.2byte	0x217
	.4byte	0x110
	.uleb128 0xc
	.4byte	.LASF266
	.byte	0x14
	.2byte	0x219
	.4byte	0x59
	.uleb128 0xa
	.4byte	0xe1
	.4byte	0xccf
	.uleb128 0x16
	.4byte	0xd4
	.2byte	0x3ff
	.byte	0
	.uleb128 0xc
	.4byte	.LASF267
	.byte	0x14
	.2byte	0x220
	.4byte	0xcbe
	.uleb128 0xc
	.4byte	.LASF268
	.byte	0x14
	.2byte	0x222
	.4byte	0xa8
	.uleb128 0xc
	.4byte	.LASF269
	.byte	0x14
	.2byte	0x224
	.4byte	0xa8
	.uleb128 0xc
	.4byte	.LASF270
	.byte	0x14
	.2byte	0x230
	.4byte	0xb76
	.uleb128 0x28
	.4byte	.LASF272
	.byte	0x4
	.4byte	0x84
	.byte	0x14
	.2byte	0x286
	.4byte	0xd29
	.uleb128 0x25
	.4byte	.LASF273
	.byte	0
	.uleb128 0x25
	.4byte	.LASF274
	.byte	0x1
	.uleb128 0x25
	.4byte	.LASF275
	.byte	0x2
	.uleb128 0x25
	.4byte	.LASF276
	.byte	0x3
	.byte	0
	.uleb128 0xc
	.4byte	.LASF277
	.byte	0x14
	.2byte	0x28c
	.4byte	0xcff
	.uleb128 0xc
	.4byte	.LASF278
	.byte	0x16
	.2byte	0x3ba
	.4byte	0x11b
	.uleb128 0xc
	.4byte	.LASF279
	.byte	0x16
	.2byte	0x3bb
	.4byte	0x11b
	.uleb128 0xc
	.4byte	.LASF280
	.byte	0x16
	.2byte	0x3bc
	.4byte	0x11b
	.uleb128 0xf
	.4byte	.LASF281
	.byte	0x4
	.byte	0x2
	.byte	0xd
	.4byte	0xd72
	.uleb128 0x19
	.string	"pid"
	.byte	0x2
	.byte	0xe
	.4byte	0x59
	.byte	0
	.byte	0
	.uleb128 0x5
	.4byte	.LASF282
	.byte	0x2
	.byte	0x11
	.4byte	0xd7d
	.uleb128 0x8
	.byte	0x8
	.4byte	0xd59
	.uleb128 0x24
	.4byte	.LASF283
	.byte	0x4
	.4byte	0x84
	.byte	0x17
	.byte	0x14
	.4byte	0xde2
	.uleb128 0x25
	.4byte	.LASF284
	.byte	0x1
	.uleb128 0x25
	.4byte	.LASF285
	.byte	0x2
	.uleb128 0x25
	.4byte	.LASF286
	.byte	0x3
	.uleb128 0x25
	.4byte	.LASF287
	.byte	0x4
	.uleb128 0x25
	.4byte	.LASF288
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF289
	.byte	0x6
	.uleb128 0x25
	.4byte	.LASF290
	.byte	0x7
	.uleb128 0x25
	.4byte	.LASF291
	.byte	0x8
	.uleb128 0x25
	.4byte	.LASF292
	.byte	0x9
	.uleb128 0x25
	.4byte	.LASF293
	.byte	0xa
	.uleb128 0x25
	.4byte	.LASF294
	.byte	0xb
	.uleb128 0x25
	.4byte	.LASF295
	.byte	0xc
	.uleb128 0x25
	.4byte	.LASF296
	.byte	0xd
	.byte	0
	.uleb128 0xf
	.4byte	.LASF297
	.byte	0x10
	.byte	0x17
	.byte	0x43
	.4byte	0xe07
	.uleb128 0x10
	.4byte	.LASF298
	.byte	0x17
	.byte	0x44
	.4byte	0x84
	.byte	0
	.uleb128 0x10
	.4byte	.LASF299
	.byte	0x17
	.byte	0x45
	.4byte	0xe07
	.byte	0x8
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.4byte	0x84
	.uleb128 0x8
	.byte	0x8
	.4byte	0xde2
	.uleb128 0x24
	.4byte	.LASF300
	.byte	0x4
	.4byte	0x84
	.byte	0x1
	.byte	0x15
	.4byte	0xe4e
	.uleb128 0x25
	.4byte	.LASF301
	.byte	0
	.uleb128 0x25
	.4byte	.LASF302
	.byte	0x1
	.uleb128 0x25
	.4byte	.LASF303
	.byte	0x2
	.uleb128 0x25
	.4byte	.LASF304
	.byte	0x3
	.uleb128 0x25
	.4byte	.LASF305
	.byte	0x4
	.uleb128 0x25
	.4byte	.LASF306
	.byte	0x5
	.uleb128 0x25
	.4byte	.LASF307
	.byte	0x6
	.byte	0
	.uleb128 0x18
	.4byte	.LASF308
	.2byte	0x954
	.byte	0x1
	.byte	0x1f
	.4byte	0xf64
	.uleb128 0x10
	.4byte	.LASF309
	.byte	0x1
	.byte	0x20
	.4byte	0x59
	.byte	0
	.uleb128 0x10
	.4byte	.LASF310
	.byte	0x1
	.byte	0x21
	.4byte	0xf64
	.byte	0x4
	.uleb128 0x10
	.4byte	.LASF83
	.byte	0x1
	.byte	0x22
	.4byte	0xf64
	.byte	0x14
	.uleb128 0x10
	.4byte	.LASF311
	.byte	0x1
	.byte	0x23
	.4byte	0xf64
	.byte	0x24
	.uleb128 0x10
	.4byte	.LASF312
	.byte	0x1
	.byte	0x24
	.4byte	0xf64
	.byte	0x34
	.uleb128 0x10
	.4byte	.LASF313
	.byte	0x1
	.byte	0x25
	.4byte	0xf64
	.byte	0x44
	.uleb128 0x10
	.4byte	.LASF314
	.byte	0x1
	.byte	0x26
	.4byte	0xbe1
	.byte	0x54
	.uleb128 0x10
	.4byte	.LASF315
	.byte	0x1
	.byte	0x27
	.4byte	0xbe1
	.byte	0x94
	.uleb128 0x10
	.4byte	.LASF316
	.byte	0x1
	.byte	0x28
	.4byte	0xbe1
	.byte	0xd4
	.uleb128 0x14
	.4byte	.LASF317
	.byte	0x1
	.byte	0x29
	.4byte	0xbe1
	.2byte	0x114
	.uleb128 0x14
	.4byte	.LASF318
	.byte	0x1
	.byte	0x2a
	.4byte	0xbe1
	.2byte	0x154
	.uleb128 0x14
	.4byte	.LASF319
	.byte	0x1
	.byte	0x2b
	.4byte	0xbe1
	.2byte	0x194
	.uleb128 0x14
	.4byte	.LASF320
	.byte	0x1
	.byte	0x2c
	.4byte	0xbe1
	.2byte	0x1d4
	.uleb128 0x14
	.4byte	.LASF321
	.byte	0x1
	.byte	0x2d
	.4byte	0xbe1
	.2byte	0x214
	.uleb128 0x14
	.4byte	.LASF322
	.byte	0x1
	.byte	0x2e
	.4byte	0xf74
	.2byte	0x254
	.uleb128 0x14
	.4byte	.LASF323
	.byte	0x1
	.byte	0x2f
	.4byte	0xf74
	.2byte	0x354
	.uleb128 0x14
	.4byte	.LASF324
	.byte	0x1
	.byte	0x30
	.4byte	0xf74
	.2byte	0x454
	.uleb128 0x14
	.4byte	.LASF325
	.byte	0x1
	.byte	0x31
	.4byte	0xf74
	.2byte	0x554
	.uleb128 0x14
	.4byte	.LASF326
	.byte	0x1
	.byte	0x32
	.4byte	0xf74
	.2byte	0x654
	.uleb128 0x14
	.4byte	.LASF327
	.byte	0x1
	.byte	0x33
	.4byte	0xf74
	.2byte	0x754
	.uleb128 0x14
	.4byte	.LASF328
	.byte	0x1
	.byte	0x34
	.4byte	0xf74
	.2byte	0x854
	.byte	0
	.uleb128 0xa
	.4byte	0xe1
	.4byte	0xf74
	.uleb128 0xe
	.4byte	0xd4
	.byte	0xf
	.byte	0
	.uleb128 0xa
	.4byte	0x84
	.4byte	0xf84
	.uleb128 0xe
	.4byte	0xd4
	.byte	0x3f
	.byte	0
	.uleb128 0x29
	.4byte	.LASF329
	.byte	0x1
	.byte	0x38
	.4byte	0xf99
	.uleb128 0x9
	.byte	0x3
	.8byte	global_waveform
	.uleb128 0x8
	.byte	0x8
	.4byte	0xe4e
	.uleb128 0xa
	.4byte	0x84
	.4byte	0xfaf
	.uleb128 0xe
	.4byte	0xd4
	.byte	0xff
	.byte	0
	.uleb128 0x29
	.4byte	.LASF330
	.byte	0x1
	.byte	0x39
	.4byte	0xf9f
	.uleb128 0x9
	.byte	0x3
	.8byte	crc32_table
	.uleb128 0x2a
	.4byte	.LASF364
	.byte	0x1
	.2byte	0x1cd
	.4byte	0x59
	.8byte	.LFB229
	.8byte	.LFE229-.LFB229
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x14e4
	.uleb128 0x2b
	.4byte	.LASF331
	.byte	0x1
	.2byte	0x1cd
	.4byte	0xe0d
	.4byte	.LLST35
	.uleb128 0x2b
	.4byte	.LASF332
	.byte	0x1
	.2byte	0x1cd
	.4byte	0xd83
	.4byte	.LLST36
	.uleb128 0x2b
	.4byte	.LASF333
	.byte	0x1
	.2byte	0x1cd
	.4byte	0x59
	.4byte	.LLST37
	.uleb128 0x2c
	.4byte	.LASF334
	.byte	0x1
	.2byte	0x1cf
	.4byte	0xd83
	.uleb128 0x9
	.byte	0x3
	.8byte	stype.6189
	.uleb128 0x2c
	.4byte	.LASF335
	.byte	0x1
	.2byte	0x1d0
	.4byte	0x59
	.uleb128 0x9
	.byte	0x3
	.8byte	sftemp.6190
	.uleb128 0x2d
	.string	"ret"
	.byte	0x1
	.2byte	0x1d1
	.4byte	0x59
	.4byte	.LLST38
	.uleb128 0x2e
	.4byte	0x1cfb
	.8byte	.LBB104
	.8byte	.LBE104-.LBB104
	.byte	0x1
	.2byte	0x1e2
	.4byte	0x1085
	.uleb128 0x2f
	.4byte	0x1d07
	.4byte	.LLST39
	.uleb128 0x30
	.8byte	.LVL80
	.4byte	0x1d3a
	.byte	0
	.uleb128 0x31
	.4byte	0x168a
	.8byte	.LBB106
	.4byte	.Ldebug_ranges0+0x180
	.byte	0x1
	.2byte	0x1ef
	.4byte	0x124c
	.uleb128 0x2f
	.4byte	0x16a7
	.4byte	.LLST40
	.uleb128 0x2f
	.4byte	0x169b
	.4byte	.LLST41
	.uleb128 0x32
	.4byte	.Ldebug_ranges0+0x180
	.uleb128 0x33
	.4byte	0x16b3
	.uleb128 0x34
	.4byte	0x16bf
	.4byte	.LLST42
	.uleb128 0x34
	.4byte	0x16cb
	.4byte	.LLST43
	.uleb128 0x34
	.4byte	0x16d7
	.4byte	.LLST44
	.uleb128 0x31
	.4byte	0x1750
	.8byte	.LBB108
	.4byte	.Ldebug_ranges0+0x1b0
	.byte	0x1
	.2byte	0x152
	.4byte	0x11bd
	.uleb128 0x2f
	.4byte	0x1761
	.4byte	.LLST45
	.uleb128 0x32
	.4byte	.Ldebug_ranges0+0x1b0
	.uleb128 0x34
	.4byte	0x176d
	.4byte	.LLST46
	.uleb128 0x33
	.4byte	0x1779
	.uleb128 0x34
	.4byte	0x1785
	.4byte	.LLST47
	.uleb128 0x34
	.4byte	0x178f
	.4byte	.LLST48
	.uleb128 0x34
	.4byte	0x1799
	.4byte	.LLST49
	.uleb128 0x34
	.4byte	0x17a3
	.4byte	.LLST50
	.uleb128 0x34
	.4byte	0x17ad
	.4byte	.LLST51
	.uleb128 0x34
	.4byte	0x17b9
	.4byte	.LLST52
	.uleb128 0x34
	.4byte	0x17c5
	.4byte	.LLST53
	.uleb128 0x2e
	.4byte	0x1d13
	.8byte	.LBB110
	.8byte	.LBE110-.LBB110
	.byte	0x1
	.2byte	0x10f
	.4byte	0x1197
	.uleb128 0x2f
	.4byte	0x1d2e
	.4byte	.LLST54
	.uleb128 0x2f
	.4byte	0x1d23
	.4byte	.LLST55
	.uleb128 0x35
	.8byte	.LVL91
	.4byte	0x1d46
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x8
	.byte	0x84
	.sleb128 0
	.byte	0x3a
	.byte	0x24
	.byte	0xa
	.2byte	0x7c00
	.byte	0x1a
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x3
	.byte	0xa
	.2byte	0x8000
	.byte	0
	.byte	0
	.uleb128 0x37
	.8byte	.LBB112
	.8byte	.LBE112-.LBB112
	.uleb128 0x34
	.4byte	0x17d2
	.4byte	.LLST56
	.uleb128 0x34
	.4byte	0x17de
	.4byte	.LLST57
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x2e
	.4byte	0x1cfb
	.8byte	.LBB114
	.8byte	.LBE114-.LBB114
	.byte	0x1
	.2byte	0x154
	.4byte	0x11f7
	.uleb128 0x2f
	.4byte	0x1d07
	.4byte	.LLST58
	.uleb128 0x35
	.8byte	.LVL95
	.4byte	0x1d3a
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x85
	.sleb128 0
	.byte	0
	.byte	0
	.uleb128 0x2e
	.4byte	0x1cfb
	.8byte	.LBB117
	.8byte	.LBE117-.LBB117
	.byte	0x1
	.2byte	0x158
	.4byte	0x121d
	.uleb128 0x2f
	.4byte	0x1d07
	.4byte	.LLST59
	.byte	0
	.uleb128 0x38
	.8byte	.LVL83
	.4byte	0x18b8
	.4byte	0x1234
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x1
	.byte	0x30
	.byte	0
	.uleb128 0x35
	.8byte	.LVL87
	.4byte	0x17ec
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x4
	.byte	0x84
	.sleb128 0
	.byte	0x32
	.byte	0x24
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x31
	.4byte	0x1532
	.8byte	.LBB121
	.4byte	.Ldebug_ranges0+0x1e0
	.byte	0x1
	.2byte	0x219
	.4byte	0x13c6
	.uleb128 0x2f
	.4byte	0x154f
	.4byte	.LLST60
	.uleb128 0x2f
	.4byte	0x1543
	.4byte	.LLST61
	.uleb128 0x32
	.4byte	.Ldebug_ranges0+0x1e0
	.uleb128 0x33
	.4byte	0x155b
	.uleb128 0x34
	.4byte	0x1567
	.4byte	.LLST62
	.uleb128 0x34
	.4byte	0x1573
	.4byte	.LLST63
	.uleb128 0x34
	.4byte	0x157f
	.4byte	.LLST64
	.uleb128 0x2e
	.4byte	0x1cfb
	.8byte	.LBB123
	.8byte	.LBE123-.LBB123
	.byte	0x1
	.2byte	0x1aa
	.4byte	0x12ce
	.uleb128 0x2f
	.4byte	0x1d07
	.4byte	.LLST65
	.uleb128 0x30
	.8byte	.LVL113
	.4byte	0x1d3a
	.byte	0
	.uleb128 0x2e
	.4byte	0x189c
	.8byte	.LBB125
	.8byte	.LBE125-.LBB125
	.byte	0x1
	.2byte	0x19c
	.4byte	0x12f4
	.uleb128 0x2f
	.4byte	0x18ac
	.4byte	.LLST66
	.byte	0
	.uleb128 0x2e
	.4byte	0x16e4
	.8byte	.LBB127
	.8byte	.LBE127-.LBB127
	.byte	0x1
	.2byte	0x1a8
	.4byte	0x1367
	.uleb128 0x2f
	.4byte	0x16fd
	.4byte	.LLST67
	.uleb128 0x2f
	.4byte	0x16f1
	.4byte	.LLST68
	.uleb128 0x37
	.8byte	.LBB128
	.8byte	.LBE128-.LBB128
	.uleb128 0x33
	.4byte	0x1709
	.uleb128 0x34
	.4byte	0x1715
	.4byte	.LLST69
	.uleb128 0x34
	.4byte	0x1721
	.4byte	.LLST70
	.uleb128 0x34
	.4byte	0x172d
	.4byte	.LLST71
	.uleb128 0x34
	.4byte	0x1739
	.4byte	.LLST72
	.uleb128 0x34
	.4byte	0x1745
	.4byte	.LLST73
	.byte	0
	.byte	0
	.uleb128 0x38
	.8byte	.LVL128
	.4byte	0x18b8
	.4byte	0x1384
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x84
	.sleb128 0
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x1
	.byte	0x32
	.byte	0
	.uleb128 0x30
	.8byte	.LVL132
	.4byte	0x17ec
	.uleb128 0x38
	.8byte	.LVL133
	.4byte	0x18b8
	.4byte	0x13ae
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x84
	.sleb128 0
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x1
	.byte	0x31
	.byte	0
	.uleb128 0x35
	.8byte	.LVL137
	.4byte	0x17ec
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x4
	.byte	0x84
	.sleb128 0
	.byte	0x36
	.byte	0x24
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x2e
	.4byte	0x158c
	.8byte	.LBB130
	.8byte	.LBE130-.LBB130
	.byte	0x1
	.2byte	0x213
	.4byte	0x144f
	.uleb128 0x2f
	.4byte	0x15a9
	.4byte	.LLST74
	.uleb128 0x2f
	.4byte	0x159d
	.4byte	.LLST75
	.uleb128 0x37
	.8byte	.LBB131
	.8byte	.LBE131-.LBB131
	.uleb128 0x34
	.4byte	0x15b5
	.4byte	.LLST76
	.uleb128 0x34
	.4byte	0x15c1
	.4byte	.LLST77
	.uleb128 0x34
	.4byte	0x15cd
	.4byte	.LLST78
	.uleb128 0x38
	.8byte	.LVL121
	.4byte	0x18b8
	.4byte	0x1437
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x1
	.byte	0x31
	.byte	0
	.uleb128 0x35
	.8byte	.LVL124
	.4byte	0x17ec
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x4
	.byte	0x84
	.sleb128 0
	.byte	0x36
	.byte	0x24
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x2e
	.4byte	0x14e4
	.8byte	.LBB133
	.8byte	.LBE133-.LBB133
	.byte	0x1
	.2byte	0x21f
	.4byte	0x14cf
	.uleb128 0x2f
	.4byte	0x1501
	.4byte	.LLST79
	.uleb128 0x2f
	.4byte	0x14f5
	.4byte	.LLST80
	.uleb128 0x37
	.8byte	.LBB134
	.8byte	.LBE134-.LBB134
	.uleb128 0x34
	.4byte	0x150d
	.4byte	.LLST81
	.uleb128 0x34
	.4byte	0x1519
	.4byte	.LLST82
	.uleb128 0x34
	.4byte	0x1525
	.4byte	.LLST83
	.uleb128 0x38
	.8byte	.LVL151
	.4byte	0x18b8
	.4byte	0x14c0
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x1
	.byte	0x36
	.byte	0
	.uleb128 0x30
	.8byte	.LVL155
	.4byte	0x17ec
	.byte	0
	.byte	0
	.uleb128 0x35
	.8byte	.LVL115
	.4byte	0x15da
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x83
	.sleb128 0
	.byte	0
	.byte	0
	.uleb128 0x39
	.4byte	.LASF338
	.byte	0x1
	.2byte	0x1af
	.4byte	0x59
	.byte	0x1
	.4byte	0x1532
	.uleb128 0x3a
	.4byte	.LASF331
	.byte	0x1
	.2byte	0x1af
	.4byte	0xe0d
	.uleb128 0x3a
	.4byte	.LASF336
	.byte	0x1
	.2byte	0x1af
	.4byte	0x59
	.uleb128 0x3b
	.4byte	.LASF337
	.byte	0x1
	.2byte	0x1b1
	.4byte	0x4df
	.uleb128 0x3b
	.4byte	.LASF298
	.byte	0x1
	.2byte	0x1b2
	.4byte	0x84
	.uleb128 0x3b
	.4byte	.LASF309
	.byte	0x1
	.2byte	0x1b3
	.4byte	0x84
	.byte	0
	.uleb128 0x39
	.4byte	.LASF339
	.byte	0x1
	.2byte	0x186
	.4byte	0x59
	.byte	0x1
	.4byte	0x158c
	.uleb128 0x3a
	.4byte	.LASF331
	.byte	0x1
	.2byte	0x186
	.4byte	0xe0d
	.uleb128 0x3a
	.4byte	.LASF336
	.byte	0x1
	.2byte	0x186
	.4byte	0x59
	.uleb128 0x3b
	.4byte	.LASF340
	.byte	0x1
	.2byte	0x188
	.4byte	0xde2
	.uleb128 0x3b
	.4byte	.LASF337
	.byte	0x1
	.2byte	0x189
	.4byte	0x4df
	.uleb128 0x3b
	.4byte	.LASF298
	.byte	0x1
	.2byte	0x18a
	.4byte	0x84
	.uleb128 0x3b
	.4byte	.LASF309
	.byte	0x1
	.2byte	0x18b
	.4byte	0x84
	.byte	0
	.uleb128 0x39
	.4byte	.LASF341
	.byte	0x1
	.2byte	0x171
	.4byte	0x59
	.byte	0x1
	.4byte	0x15da
	.uleb128 0x3a
	.4byte	.LASF331
	.byte	0x1
	.2byte	0x171
	.4byte	0xe0d
	.uleb128 0x3a
	.4byte	.LASF336
	.byte	0x1
	.2byte	0x171
	.4byte	0x59
	.uleb128 0x3b
	.4byte	.LASF337
	.byte	0x1
	.2byte	0x173
	.4byte	0x4df
	.uleb128 0x3b
	.4byte	.LASF298
	.byte	0x1
	.2byte	0x174
	.4byte	0x84
	.uleb128 0x3b
	.4byte	.LASF309
	.byte	0x1
	.2byte	0x175
	.4byte	0x84
	.byte	0
	.uleb128 0x3c
	.4byte	.LASF349
	.byte	0x1
	.2byte	0x15c
	.4byte	0x59
	.8byte	.LFB225
	.8byte	.LFE225-.LFB225
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x168a
	.uleb128 0x2b
	.4byte	.LASF331
	.byte	0x1
	.2byte	0x15c
	.4byte	0xe0d
	.4byte	.LLST16
	.uleb128 0x2b
	.4byte	.LASF336
	.byte	0x1
	.2byte	0x15c
	.4byte	0x59
	.4byte	.LLST17
	.uleb128 0x2b
	.4byte	.LASF332
	.byte	0x1
	.2byte	0x15c
	.4byte	0xe13
	.4byte	.LLST18
	.uleb128 0x3d
	.4byte	.LASF337
	.byte	0x1
	.2byte	0x15e
	.4byte	0x4df
	.4byte	.LLST19
	.uleb128 0x3d
	.4byte	.LASF298
	.byte	0x1
	.2byte	0x15f
	.4byte	0x84
	.4byte	.LLST20
	.uleb128 0x3d
	.4byte	.LASF309
	.byte	0x1
	.2byte	0x160
	.4byte	0x84
	.4byte	.LLST21
	.uleb128 0x38
	.8byte	.LVL45
	.4byte	0x18b8
	.4byte	0x167c
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x3
	.byte	0xf3
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x3
	.byte	0xf3
	.uleb128 0x1
	.byte	0x52
	.byte	0
	.uleb128 0x30
	.8byte	.LVL49
	.4byte	0x17ec
	.byte	0
	.uleb128 0x39
	.4byte	.LASF342
	.byte	0x1
	.2byte	0x13d
	.4byte	0x59
	.byte	0x1
	.4byte	0x16e4
	.uleb128 0x3a
	.4byte	.LASF331
	.byte	0x1
	.2byte	0x13d
	.4byte	0xe0d
	.uleb128 0x3a
	.4byte	.LASF336
	.byte	0x1
	.2byte	0x13d
	.4byte	0x59
	.uleb128 0x3b
	.4byte	.LASF340
	.byte	0x1
	.2byte	0x13f
	.4byte	0xde2
	.uleb128 0x3b
	.4byte	.LASF337
	.byte	0x1
	.2byte	0x140
	.4byte	0x4df
	.uleb128 0x3b
	.4byte	.LASF298
	.byte	0x1
	.2byte	0x141
	.4byte	0x84
	.uleb128 0x3b
	.4byte	.LASF309
	.byte	0x1
	.2byte	0x142
	.4byte	0x84
	.byte	0
	.uleb128 0x3e
	.4byte	.LASF375
	.byte	0x1
	.2byte	0x124
	.byte	0x1
	.4byte	0x1750
	.uleb128 0x3a
	.4byte	.LASF340
	.byte	0x1
	.2byte	0x124
	.4byte	0xe0d
	.uleb128 0x3a
	.4byte	.LASF331
	.byte	0x1
	.2byte	0x125
	.4byte	0xe0d
	.uleb128 0x3b
	.4byte	.LASF343
	.byte	0x1
	.2byte	0x127
	.4byte	0x59
	.uleb128 0x3b
	.4byte	.LASF344
	.byte	0x1
	.2byte	0x128
	.4byte	0xe07
	.uleb128 0x3b
	.4byte	.LASF345
	.byte	0x1
	.2byte	0x128
	.4byte	0xe07
	.uleb128 0x3b
	.4byte	.LASF336
	.byte	0x1
	.2byte	0x129
	.4byte	0x59
	.uleb128 0x3b
	.4byte	.LASF346
	.byte	0x1
	.2byte	0x129
	.4byte	0x59
	.uleb128 0x3f
	.string	"i"
	.byte	0x1
	.2byte	0x12a
	.4byte	0x59
	.byte	0
	.uleb128 0x39
	.4byte	.LASF347
	.byte	0x1
	.2byte	0x104
	.4byte	0xe07
	.byte	0x1
	.4byte	0x17ec
	.uleb128 0x3a
	.4byte	.LASF340
	.byte	0x1
	.2byte	0x104
	.4byte	0xe0d
	.uleb128 0x3b
	.4byte	.LASF344
	.byte	0x1
	.2byte	0x106
	.4byte	0xe07
	.uleb128 0x3b
	.4byte	.LASF345
	.byte	0x1
	.2byte	0x106
	.4byte	0xe07
	.uleb128 0x3f
	.string	"i"
	.byte	0x1
	.2byte	0x107
	.4byte	0x59
	.uleb128 0x3f
	.string	"j"
	.byte	0x1
	.2byte	0x107
	.4byte	0x59
	.uleb128 0x3f
	.string	"k"
	.byte	0x1
	.2byte	0x107
	.4byte	0x59
	.uleb128 0x3f
	.string	"h"
	.byte	0x1
	.2byte	0x107
	.4byte	0x59
	.uleb128 0x3b
	.4byte	.LASF348
	.byte	0x1
	.2byte	0x108
	.4byte	0x59
	.uleb128 0x3f
	.string	"num"
	.byte	0x1
	.2byte	0x109
	.4byte	0x59
	.uleb128 0x3f
	.string	"len"
	.byte	0x1
	.2byte	0x10a
	.4byte	0x59
	.uleb128 0x40
	.uleb128 0x3b
	.4byte	.LASF336
	.byte	0x1
	.2byte	0x116
	.4byte	0x59
	.uleb128 0x3b
	.4byte	.LASF346
	.byte	0x1
	.2byte	0x117
	.4byte	0x59
	.byte	0
	.byte	0
	.uleb128 0x41
	.4byte	.LASF350
	.byte	0x1
	.byte	0xf6
	.4byte	0xe07
	.8byte	.LFB221
	.8byte	.LFE221-.LFB221
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x189c
	.uleb128 0x42
	.4byte	.LASF337
	.byte	0x1
	.byte	0xf6
	.4byte	0x4df
	.4byte	.LLST12
	.uleb128 0x42
	.4byte	.LASF309
	.byte	0x1
	.byte	0xf7
	.4byte	0x84
	.4byte	.LLST13
	.uleb128 0x43
	.string	"out"
	.byte	0x1
	.byte	0xf9
	.4byte	0x4e5
	.uleb128 0x44
	.4byte	0x1d13
	.8byte	.LBB39
	.4byte	.Ldebug_ranges0+0x90
	.byte	0x1
	.byte	0xfb
	.4byte	0x187b
	.uleb128 0x2f
	.4byte	0x1d2e
	.4byte	.LLST14
	.uleb128 0x2f
	.4byte	0x1d23
	.4byte	.LLST15
	.uleb128 0x35
	.8byte	.LVL37
	.4byte	0x1d46
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x84
	.sleb128 0
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x3
	.byte	0xa
	.2byte	0x8000
	.byte	0
	.byte	0
	.uleb128 0x35
	.8byte	.LVL39
	.4byte	0x1d51
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x83
	.sleb128 0
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x2
	.byte	0x85
	.sleb128 4
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x52
	.uleb128 0x2
	.byte	0x84
	.sleb128 0
	.byte	0
	.byte	0
	.uleb128 0x45
	.4byte	.LASF351
	.byte	0x1
	.byte	0xf1
	.4byte	0x34
	.byte	0x3
	.4byte	0x18b8
	.uleb128 0x46
	.4byte	.LASF337
	.byte	0x1
	.byte	0xf1
	.4byte	0x4df
	.byte	0
	.uleb128 0x41
	.4byte	.LASF352
	.byte	0x1
	.byte	0xc0
	.4byte	0x4df
	.8byte	.LFB219
	.8byte	.LFE219-.LFB219
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x19b5
	.uleb128 0x42
	.4byte	.LASF336
	.byte	0x1
	.byte	0xc0
	.4byte	0x59
	.4byte	.LLST0
	.uleb128 0x42
	.4byte	.LASF353
	.byte	0x1
	.byte	0xc0
	.4byte	0x59
	.4byte	.LLST1
	.uleb128 0x29
	.4byte	.LASF354
	.byte	0x1
	.byte	0xc2
	.4byte	0xf99
	.uleb128 0x1
	.byte	0x52
	.uleb128 0x47
	.4byte	.LASF355
	.byte	0x1
	.byte	0xc3
	.4byte	0xe07
	.4byte	.LLST2
	.uleb128 0x47
	.4byte	.LASF356
	.byte	0x1
	.byte	0xc4
	.4byte	0xff
	.4byte	.LLST3
	.uleb128 0x47
	.4byte	.LASF357
	.byte	0x1
	.byte	0xc5
	.4byte	0x84
	.4byte	.LLST4
	.uleb128 0x48
	.string	"pt"
	.byte	0x1
	.byte	0xc6
	.4byte	0x84
	.4byte	.LLST5
	.uleb128 0x49
	.4byte	.LASF337
	.byte	0x1
	.byte	0xc7
	.4byte	0x147
	.uleb128 0x4a
	.4byte	0x19b5
	.8byte	.LBB31
	.4byte	.Ldebug_ranges0+0
	.byte	0x1
	.byte	0xea
	.uleb128 0x2f
	.4byte	0x19d0
	.4byte	.LLST6
	.uleb128 0x2f
	.4byte	0x19c5
	.4byte	.LLST7
	.uleb128 0x32
	.4byte	.Ldebug_ranges0+0
	.uleb128 0x34
	.4byte	0x19db
	.4byte	.LLST8
	.uleb128 0x34
	.4byte	0x19e6
	.4byte	.LLST9
	.uleb128 0x4b
	.4byte	.Ldebug_ranges0+0x30
	.4byte	0x199a
	.uleb128 0x33
	.4byte	0x19f4
	.uleb128 0x33
	.4byte	0x19ff
	.byte	0
	.uleb128 0x32
	.4byte	.Ldebug_ranges0+0x60
	.uleb128 0x34
	.4byte	0x1a0c
	.4byte	.LLST10
	.uleb128 0x34
	.4byte	0x1a17
	.4byte	.LLST11
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x45
	.4byte	.LASF358
	.byte	0x1
	.byte	0xab
	.4byte	0x59
	.byte	0x1
	.4byte	0x1a24
	.uleb128 0x46
	.4byte	.LASF355
	.byte	0x1
	.byte	0xab
	.4byte	0xff
	.uleb128 0x46
	.4byte	.LASF336
	.byte	0x1
	.byte	0xab
	.4byte	0x59
	.uleb128 0x49
	.4byte	.LASF359
	.byte	0x1
	.byte	0xad
	.4byte	0x59
	.uleb128 0x43
	.string	"i"
	.byte	0x1
	.byte	0xae
	.4byte	0x59
	.uleb128 0x4c
	.4byte	0x1a0b
	.uleb128 0x49
	.4byte	.LASF360
	.byte	0x1
	.byte	0xb0
	.4byte	0x59
	.uleb128 0x49
	.4byte	.LASF361
	.byte	0x1
	.byte	0xb0
	.4byte	0x59
	.byte	0
	.uleb128 0x40
	.uleb128 0x49
	.4byte	.LASF362
	.byte	0x1
	.byte	0xb1
	.4byte	0x59
	.uleb128 0x49
	.4byte	.LASF363
	.byte	0x1
	.byte	0xb1
	.4byte	0x59
	.byte	0
	.byte	0
	.uleb128 0x4d
	.4byte	.LASF394
	.byte	0x1
	.byte	0xa2
	.4byte	0xdb
	.8byte	.LFB217
	.8byte	.LFE217-.LFB217
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x4e
	.4byte	.LASF365
	.byte	0x1
	.byte	0x80
	.4byte	0x59
	.8byte	.LFB216
	.8byte	.LFE216-.LFB216
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x1c27
	.uleb128 0x42
	.4byte	.LASF366
	.byte	0x1
	.byte	0x80
	.4byte	0x147
	.4byte	.LLST22
	.uleb128 0x43
	.string	"ret"
	.byte	0x1
	.byte	0x82
	.4byte	0x59
	.uleb128 0x48
	.string	"wf"
	.byte	0x1
	.byte	0x83
	.4byte	0xf99
	.4byte	.LLST23
	.uleb128 0x4f
	.4byte	0x1c59
	.8byte	.LBB51
	.8byte	.LBE51-.LBB51
	.byte	0x1
	.byte	0x88
	.4byte	0x1af4
	.uleb128 0x2f
	.4byte	0x1c69
	.4byte	.LLST24
	.uleb128 0x38
	.8byte	.LVL54
	.4byte	0x1d5d
	.4byte	0x1ad8
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x83
	.sleb128 4
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x9
	.byte	0x3
	.8byte	.LC0
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x52
	.uleb128 0x1
	.byte	0x3c
	.byte	0
	.uleb128 0x35
	.8byte	.LVL55
	.4byte	0x1d68
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x9
	.byte	0x3
	.8byte	.LC1
	.byte	0
	.byte	0
	.uleb128 0x44
	.4byte	0x1c27
	.8byte	.LBB53
	.4byte	.Ldebug_ranges0+0xc0
	.byte	0x1
	.byte	0x8e
	.4byte	0x1bf8
	.uleb128 0x2f
	.4byte	0x1c37
	.4byte	.LLST25
	.uleb128 0x32
	.4byte	.Ldebug_ranges0+0xc0
	.uleb128 0x33
	.4byte	0x1c42
	.uleb128 0x34
	.4byte	0x1c4d
	.4byte	.LLST26
	.uleb128 0x44
	.4byte	0x1c75
	.8byte	.LBB55
	.4byte	.Ldebug_ranges0+0xf0
	.byte	0x1
	.byte	0x6f
	.4byte	0x1bd5
	.uleb128 0x2f
	.4byte	0x1c90
	.4byte	.LLST27
	.uleb128 0x2f
	.4byte	0x1c85
	.4byte	.LLST28
	.uleb128 0x32
	.4byte	.Ldebug_ranges0+0xf0
	.uleb128 0x34
	.4byte	0x1c9b
	.4byte	.LLST29
	.uleb128 0x34
	.4byte	0x1ca4
	.4byte	.LLST30
	.uleb128 0x44
	.4byte	0x1cb0
	.8byte	.LBB57
	.4byte	.Ldebug_ranges0+0x130
	.byte	0x1
	.byte	0x55
	.4byte	0x1bb3
	.uleb128 0x50
	.4byte	0x1cbc
	.uleb128 0x50
	.4byte	0x1cc7
	.uleb128 0x32
	.4byte	.Ldebug_ranges0+0x130
	.uleb128 0x34
	.4byte	0x1cd2
	.4byte	.LLST31
	.uleb128 0x34
	.4byte	0x1cdb
	.4byte	.LLST32
	.uleb128 0x34
	.4byte	0x1ce4
	.4byte	.LLST33
	.uleb128 0x34
	.4byte	0x1cef
	.4byte	.LLST34
	.byte	0
	.byte	0
	.uleb128 0x35
	.8byte	.LVL64
	.4byte	0x1d73
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x87
	.sleb128 0
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x1
	.byte	0x30
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x52
	.uleb128 0x3
	.byte	0xa
	.2byte	0x400
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x35
	.8byte	.LVL74
	.4byte	0x1d68
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x9
	.byte	0x3
	.8byte	.LC3
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x2
	.byte	0x89
	.sleb128 0
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x30
	.8byte	.LVL56
	.4byte	0x1d68
	.uleb128 0x35
	.8byte	.LVL76
	.4byte	0x1d68
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x9
	.byte	0x3
	.8byte	.LC5
	.uleb128 0x36
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x2
	.byte	0x83
	.sleb128 20
	.byte	0
	.byte	0
	.uleb128 0x45
	.4byte	.LASF367
	.byte	0x1
	.byte	0x66
	.4byte	0x59
	.byte	0x1
	.4byte	0x1c59
	.uleb128 0x46
	.4byte	.LASF354
	.byte	0x1
	.byte	0x66
	.4byte	0xf99
	.uleb128 0x49
	.4byte	.LASF368
	.byte	0x1
	.byte	0x68
	.4byte	0xa8
	.uleb128 0x49
	.4byte	.LASF369
	.byte	0x1
	.byte	0x69
	.4byte	0xa8
	.byte	0
	.uleb128 0x45
	.4byte	.LASF370
	.byte	0x1
	.byte	0x5c
	.4byte	0x59
	.byte	0x1
	.4byte	0x1c75
	.uleb128 0x51
	.string	"buf"
	.byte	0x1
	.byte	0x5c
	.4byte	0xff
	.byte	0
	.uleb128 0x45
	.4byte	.LASF371
	.byte	0x1
	.byte	0x4f
	.4byte	0x84
	.byte	0x1
	.4byte	0x1cb0
	.uleb128 0x46
	.4byte	.LASF372
	.byte	0x1
	.byte	0x4f
	.4byte	0x4e5
	.uleb128 0x46
	.4byte	.LASF373
	.byte	0x1
	.byte	0x4f
	.4byte	0x84
	.uleb128 0x43
	.string	"i"
	.byte	0x1
	.byte	0x51
	.4byte	0x84
	.uleb128 0x49
	.4byte	.LASF374
	.byte	0x1
	.byte	0x52
	.4byte	0x84
	.byte	0
	.uleb128 0x52
	.4byte	.LASF376
	.byte	0x1
	.byte	0x3b
	.byte	0x1
	.4byte	0x1cfb
	.uleb128 0x46
	.4byte	.LASF377
	.byte	0x1
	.byte	0x3b
	.4byte	0x84
	.uleb128 0x46
	.4byte	.LASF378
	.byte	0x1
	.byte	0x3b
	.4byte	0xe07
	.uleb128 0x43
	.string	"i"
	.byte	0x1
	.byte	0x3d
	.4byte	0x84
	.uleb128 0x43
	.string	"j"
	.byte	0x1
	.byte	0x3d
	.4byte	0x84
	.uleb128 0x49
	.4byte	.LASF379
	.byte	0x1
	.byte	0x3e
	.4byte	0x84
	.uleb128 0x49
	.4byte	.LASF374
	.byte	0x1
	.byte	0x3f
	.4byte	0x84
	.byte	0
	.uleb128 0x52
	.4byte	.LASF380
	.byte	0x2
	.byte	0x4f
	.byte	0x3
	.4byte	0x1d13
	.uleb128 0x46
	.4byte	.LASF381
	.byte	0x2
	.byte	0x4f
	.4byte	0x18a
	.byte	0
	.uleb128 0x45
	.4byte	.LASF382
	.byte	0x2
	.byte	0x38
	.4byte	0x147
	.byte	0x3
	.4byte	0x1d3a
	.uleb128 0x46
	.4byte	.LASF33
	.byte	0x2
	.byte	0x38
	.4byte	0x105
	.uleb128 0x46
	.4byte	.LASF383
	.byte	0x2
	.byte	0x38
	.4byte	0x13c
	.byte	0
	.uleb128 0x53
	.4byte	.LASF384
	.4byte	.LASF384
	.byte	0x16
	.2byte	0x399
	.uleb128 0x54
	.4byte	.LASF385
	.4byte	.LASF385
	.byte	0x2
	.byte	0x36
	.uleb128 0x53
	.4byte	.LASF386
	.4byte	.LASF386
	.byte	0x16
	.2byte	0x16d
	.uleb128 0x54
	.4byte	.LASF387
	.4byte	.LASF387
	.byte	0x8
	.byte	0x2a
	.uleb128 0x54
	.4byte	.LASF388
	.4byte	.LASF388
	.byte	0x18
	.byte	0x13
	.uleb128 0x53
	.4byte	.LASF389
	.4byte	.LASF389
	.byte	0x16
	.2byte	0x16c
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xd
	.byte	0
	.byte	0
	.uleb128 0x27
	.uleb128 0x4
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x28
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x29
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x2a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2b
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2c
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x2d
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2e
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x30
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x31
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x32
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x33
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x34
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x35
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x36
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0x18
	.uleb128 0x2111
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x37
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0x38
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x39
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3a
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3b
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3c
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3d
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x3e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3f
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x40
	.uleb128 0xb
	.byte	0x1
	.byte	0
	.byte	0
	.uleb128 0x41
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x42
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x43
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x44
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x45
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x46
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x47
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x48
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x49
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4a
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x4b
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4c
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4d
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x4e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4f
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x50
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x51
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x52
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x53
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x54
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST35:
	.8byte	.LVL77
	.8byte	.LVL78
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL78
	.8byte	.LVL116
	.2byte	0x1
	.byte	0x63
	.8byte	.LVL116
	.8byte	.LVL117
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	.LVL117
	.8byte	.LVL125
	.2byte	0x1
	.byte	0x63
	.8byte	.LVL125
	.8byte	.LVL126
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	.LVL126
	.8byte	.LVL156
	.2byte	0x1
	.byte	0x63
	.8byte	.LVL156
	.8byte	.LVL157
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL157
	.8byte	.LVL158
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	.LVL158
	.8byte	.LVL159
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL159
	.8byte	.LFE229
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST36:
	.8byte	.LVL77
	.8byte	.LVL80-1
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL80-1
	.8byte	.LVL81
	.2byte	0x1
	.byte	0x65
	.8byte	.LVL81
	.8byte	.LVL88
	.2byte	0x3
	.byte	0x85
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL88
	.8byte	.LVL114
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x51
	.byte	0x9f
	.8byte	.LVL118
	.8byte	.LVL119
	.2byte	0x3
	.byte	0x85
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL119
	.8byte	.LVL120
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x51
	.byte	0x9f
	.8byte	.LVL120
	.8byte	.LVL125
	.2byte	0x3
	.byte	0x85
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL125
	.8byte	.LVL126
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x51
	.byte	0x9f
	.8byte	.LVL126
	.8byte	.LVL140
	.2byte	0x3
	.byte	0x85
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL140
	.8byte	.LVL150
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x51
	.byte	0x9f
	.8byte	.LVL150
	.8byte	.LVL156
	.2byte	0x3
	.byte	0x85
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL156
	.8byte	.LFE229
	.2byte	0x1
	.byte	0x51
	.8byte	0
	.8byte	0
.LLST37:
	.8byte	.LVL77
	.8byte	.LVL80-1
	.2byte	0x1
	.byte	0x52
	.8byte	.LVL80-1
	.8byte	.LVL156
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x52
	.byte	0x9f
	.8byte	.LVL156
	.8byte	.LFE229
	.2byte	0x1
	.byte	0x52
	.8byte	0
	.8byte	0
.LLST38:
	.8byte	.LVL115
	.8byte	.LVL116
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST39:
	.8byte	.LVL79
	.8byte	.LVL80-1
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST40:
	.8byte	.LVL82
	.8byte	.LVL83-1
	.2byte	0x9
	.byte	0x3
	.8byte	sftemp.6190
	.8byte	0
	.8byte	0
.LLST41:
	.8byte	.LVL82
	.8byte	.LVL95
	.2byte	0x1
	.byte	0x63
	.8byte	.LVL96
	.8byte	.LVL112
	.2byte	0x1
	.byte	0x63
	.8byte	0
	.8byte	0
.LLST42:
	.8byte	.LVL83
	.8byte	.LVL87-1
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST43:
	.8byte	.LVL84
	.8byte	.LVL95
	.2byte	0x1
	.byte	0x66
	.8byte	.LVL96
	.8byte	.LVL112
	.2byte	0x1
	.byte	0x66
	.8byte	0
	.8byte	0
.LLST44:
	.8byte	.LVL85
	.8byte	.LVL86
	.2byte	0x5
	.byte	0x84
	.sleb128 0
	.byte	0x32
	.byte	0x24
	.byte	0x9f
	.8byte	.LVL86
	.8byte	.LVL87-1
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL87-1
	.8byte	.LVL95
	.2byte	0x5
	.byte	0x84
	.sleb128 0
	.byte	0x32
	.byte	0x24
	.byte	0x9f
	.8byte	.LVL96
	.8byte	.LVL112
	.2byte	0x5
	.byte	0x84
	.sleb128 0
	.byte	0x32
	.byte	0x24
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST45:
	.8byte	.LVL89
	.8byte	.LVL93
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+4276
	.sleb128 0
	.8byte	.LVL96
	.8byte	.LVL111
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+4276
	.sleb128 0
	.8byte	0
	.8byte	0
.LLST46:
	.8byte	.LVL89
	.8byte	.LVL90
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL90
	.8byte	.LVL92
	.2byte	0x1
	.byte	0x65
	.8byte	.LVL92
	.8byte	.LVL93
	.2byte	0x8
	.byte	0x72
	.sleb128 0
	.byte	0x32
	.byte	0x24
	.byte	0x85
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	.LVL96
	.8byte	.LVL109
	.2byte	0x8
	.byte	0x72
	.sleb128 1
	.byte	0x32
	.byte	0x24
	.byte	0x85
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	.LVL109
	.8byte	.LVL110
	.2byte	0x8
	.byte	0x72
	.sleb128 0
	.byte	0x32
	.byte	0x24
	.byte	0x85
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	.LVL110
	.8byte	.LVL111
	.2byte	0x1
	.byte	0x65
	.8byte	0
	.8byte	0
.LLST47:
	.8byte	.LVL92
	.8byte	.LVL93
	.2byte	0x1
	.byte	0x52
	.8byte	.LVL96
	.8byte	.LVL108
	.2byte	0x1
	.byte	0x52
	.8byte	.LVL108
	.8byte	.LVL109
	.2byte	0x3
	.byte	0x72
	.sleb128 1
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST48:
	.8byte	.LVL97
	.8byte	.LVL98
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	.LVL98
	.8byte	.LVL106
	.2byte	0x1
	.byte	0x53
	.8byte	.LVL106
	.8byte	.LVL107
	.2byte	0x3
	.byte	0x73
	.sleb128 1
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST49:
	.8byte	.LVL99
	.8byte	.LVL100
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST50:
	.8byte	.LVL103
	.8byte	.LVL104
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL104
	.8byte	.LVL105
	.2byte	0x3
	.byte	0x71
	.sleb128 1
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST51:
	.8byte	.LVL97
	.8byte	.LVL110
	.2byte	0x1
	.byte	0x57
	.8byte	0
	.8byte	0
.LLST52:
	.8byte	.LVL89
	.8byte	.LVL92
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	.LVL92
	.8byte	.LVL93
	.2byte	0x5
	.byte	0x72
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x9f
	.8byte	.LVL96
	.8byte	.LVL98
	.2byte	0x5
	.byte	0x72
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x9f
	.8byte	.LVL98
	.8byte	.LVL103
	.2byte	0xa
	.byte	0x73
	.sleb128 0
	.byte	0x34
	.byte	0x24
	.byte	0x72
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x22
	.byte	0x9f
	.8byte	.LVL103
	.8byte	.LVL104
	.2byte	0xd
	.byte	0x73
	.sleb128 0
	.byte	0x34
	.byte	0x24
	.byte	0x72
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x22
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	.LVL104
	.8byte	.LVL105
	.2byte	0xf
	.byte	0x73
	.sleb128 0
	.byte	0x34
	.byte	0x24
	.byte	0x72
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x22
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x23
	.uleb128 0x1
	.byte	0x9f
	.8byte	.LVL106
	.8byte	.LVL107
	.2byte	0xc
	.byte	0x73
	.sleb128 0
	.byte	0x34
	.byte	0x24
	.byte	0x72
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x22
	.byte	0x23
	.uleb128 0x10
	.byte	0x9f
	.8byte	.LVL110
	.8byte	.LVL111
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST53:
	.8byte	.LVL89
	.8byte	.LVL93
	.2byte	0x1
	.byte	0x64
	.8byte	.LVL96
	.8byte	.LVL111
	.2byte	0x1
	.byte	0x64
	.8byte	0
	.8byte	0
.LLST54:
	.8byte	.LVL89
	.8byte	.LVL91
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST55:
	.8byte	.LVL89
	.8byte	.LVL91
	.2byte	0xb
	.byte	0x84
	.sleb128 0
	.byte	0x3a
	.byte	0x24
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x8
	.byte	0x20
	.byte	0x26
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST56:
	.8byte	.LVL99
	.8byte	.LVL102
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL102
	.8byte	.LVL107
	.2byte	0xd
	.byte	0x77
	.sleb128 0
	.byte	0x73
	.sleb128 0
	.byte	0x31
	.byte	0x24
	.byte	0x8
	.byte	0xff
	.byte	0x1a
	.byte	0x26
	.byte	0x33
	.byte	0x1a
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST57:
	.8byte	.LVL99
	.8byte	.LVL100
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL100
	.8byte	.LVL110
	.2byte	0x1
	.byte	0x55
	.8byte	0
	.8byte	0
.LLST58:
	.8byte	.LVL94
	.8byte	.LVL95
	.2byte	0x1
	.byte	0x65
	.8byte	0
	.8byte	0
.LLST59:
	.8byte	.LVL111
	.8byte	.LVL112
	.2byte	0x1
	.byte	0x65
	.8byte	0
	.8byte	0
.LLST60:
	.8byte	.LVL127
	.8byte	.LVL134
	.2byte	0x1
	.byte	0x64
	.8byte	0
	.8byte	0
.LLST61:
	.8byte	.LVL127
	.8byte	.LVL150
	.2byte	0x1
	.byte	0x63
	.8byte	0
	.8byte	0
.LLST62:
	.8byte	.LVL128
	.8byte	.LVL132-1
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL133
	.8byte	.LVL137-1
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST63:
	.8byte	.LVL129
	.8byte	.LVL131
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL131
	.8byte	.LVL132-1
	.2byte	0x2
	.byte	0x83
	.sleb128 0
	.8byte	.LVL134
	.8byte	.LVL139
	.2byte	0x1
	.byte	0x64
	.8byte	0
	.8byte	0
.LLST64:
	.8byte	.LVL130
	.8byte	.LVL131
	.2byte	0x5
	.byte	0x71
	.sleb128 0
	.byte	0x36
	.byte	0x24
	.byte	0x9f
	.8byte	.LVL131
	.8byte	.LVL132-1
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL135
	.8byte	.LVL136
	.2byte	0x5
	.byte	0x84
	.sleb128 0
	.byte	0x36
	.byte	0x24
	.byte	0x9f
	.8byte	.LVL136
	.8byte	.LVL137-1
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL137-1
	.8byte	.LVL139
	.2byte	0x5
	.byte	0x84
	.sleb128 0
	.byte	0x36
	.byte	0x24
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST65:
	.8byte	.LVL112
	.8byte	.LVL113-1
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST66:
	.8byte	.LVL133
	.8byte	.LVL134
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST67:
	.8byte	.LVL138
	.8byte	.LVL150
	.2byte	0x1
	.byte	0x63
	.8byte	0
	.8byte	0
.LLST68:
	.8byte	.LVL138
	.8byte	.LVL150
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+4731
	.sleb128 0
	.8byte	0
	.8byte	0
.LLST69:
	.8byte	.LVL138
	.8byte	.LVL140
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL141
	.8byte	.LVL142
	.2byte	0x1
	.byte	0x56
	.8byte	.LVL142
	.8byte	.LVL148
	.2byte	0x6
	.byte	0x76
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	.LVL148
	.8byte	.LVL149
	.2byte	0x8
	.byte	0x76
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x23
	.uleb128 0x4
	.byte	0x9f
	.8byte	.LVL149
	.8byte	.LVL150
	.2byte	0x6
	.byte	0x76
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST70:
	.8byte	.LVL138
	.8byte	.LVL140
	.2byte	0x1
	.byte	0x57
	.8byte	.LVL141
	.8byte	.LVL142
	.2byte	0x1
	.byte	0x55
	.8byte	.LVL142
	.8byte	.LVL148
	.2byte	0x6
	.byte	0x75
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	.LVL148
	.8byte	.LVL149
	.2byte	0x8
	.byte	0x75
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x23
	.uleb128 0x4
	.byte	0x9f
	.8byte	.LVL149
	.8byte	.LVL150
	.2byte	0x6
	.byte	0x75
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST71:
	.8byte	.LVL143
	.8byte	.LVL145
	.2byte	0xa
	.byte	0x73
	.sleb128 0
	.byte	0x11
	.sleb128 -1073741821
	.byte	0x1a
	.byte	0x9f
	.8byte	.LVL145
	.8byte	.LVL147
	.2byte	0x1
	.byte	0x53
	.8byte	.LVL147
	.8byte	.LVL148
	.2byte	0xf
	.byte	0x76
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x94
	.byte	0x4
	.byte	0x11
	.sleb128 -1073741821
	.byte	0x1a
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST72:
	.8byte	.LVL144
	.8byte	.LVL146
	.2byte	0x9
	.byte	0x74
	.sleb128 0
	.byte	0xc
	.4byte	0x3ffffffc
	.byte	0x1a
	.byte	0x9f
	.8byte	.LVL146
	.8byte	.LVL150
	.2byte	0x1
	.byte	0x54
	.8byte	0
	.8byte	0
.LLST73:
	.8byte	.LVL138
	.8byte	.LVL140
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST74:
	.8byte	.LVL120
	.8byte	.LVL121-1
	.2byte	0x9
	.byte	0x3
	.8byte	sftemp.6190
	.8byte	0
	.8byte	0
.LLST75:
	.8byte	.LVL120
	.8byte	.LVL125
	.2byte	0x1
	.byte	0x63
	.8byte	0
	.8byte	0
.LLST76:
	.8byte	.LVL121
	.8byte	.LVL124-1
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST77:
	.8byte	.LVL122
	.8byte	.LVL125
	.2byte	0x1
	.byte	0x64
	.8byte	0
	.8byte	0
.LLST78:
	.8byte	.LVL122
	.8byte	.LVL123
	.2byte	0x5
	.byte	0x84
	.sleb128 0
	.byte	0x36
	.byte	0x24
	.byte	0x9f
	.8byte	.LVL123
	.8byte	.LVL124-1
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL124-1
	.8byte	.LVL125
	.2byte	0x5
	.byte	0x84
	.sleb128 0
	.byte	0x36
	.byte	0x24
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST79:
	.8byte	.LVL150
	.8byte	.LVL151-1
	.2byte	0x9
	.byte	0x3
	.8byte	sftemp.6190
	.8byte	0
	.8byte	0
.LLST80:
	.8byte	.LVL150
	.8byte	.LVL156
	.2byte	0x1
	.byte	0x63
	.8byte	0
	.8byte	0
.LLST81:
	.8byte	.LVL151
	.8byte	.LVL155-1
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST82:
	.8byte	.LVL152
	.8byte	.LVL154
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL154
	.8byte	.LVL155-1
	.2byte	0x2
	.byte	0x83
	.sleb128 0
	.8byte	0
	.8byte	0
.LLST83:
	.8byte	.LVL153
	.8byte	.LVL154
	.2byte	0x5
	.byte	0x71
	.sleb128 0
	.byte	0x36
	.byte	0x24
	.byte	0x9f
	.8byte	.LVL154
	.8byte	.LVL155-1
	.2byte	0x1
	.byte	0x51
	.8byte	0
	.8byte	0
.LLST16:
	.8byte	.LVL42
	.8byte	.LVL43
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL43
	.8byte	.LVL50
	.2byte	0x1
	.byte	0x63
	.8byte	.LVL50
	.8byte	.LFE225
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST17:
	.8byte	.LVL42
	.8byte	.LVL44
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL44
	.8byte	.LVL45-1
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL45-1
	.8byte	.LFE225
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x51
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST18:
	.8byte	.LVL42
	.8byte	.LVL45-1
	.2byte	0x1
	.byte	0x52
	.8byte	.LVL45-1
	.8byte	.LFE225
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x52
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST19:
	.8byte	.LVL45
	.8byte	.LVL49-1
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST20:
	.8byte	.LVL46
	.8byte	.LVL48
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL48
	.8byte	.LVL49-1
	.2byte	0x2
	.byte	0x83
	.sleb128 0
	.8byte	0
	.8byte	0
.LLST21:
	.8byte	.LVL47
	.8byte	.LVL48
	.2byte	0x5
	.byte	0x71
	.sleb128 0
	.byte	0x36
	.byte	0x24
	.byte	0x9f
	.8byte	.LVL48
	.8byte	.LVL49-1
	.2byte	0x1
	.byte	0x51
	.8byte	0
	.8byte	0
.LLST12:
	.8byte	.LVL33
	.8byte	.LVL36
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL36
	.8byte	.LVL40
	.2byte	0x1
	.byte	0x65
	.8byte	.LVL40
	.8byte	.LFE221
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST13:
	.8byte	.LVL33
	.8byte	.LVL35
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL35
	.8byte	.LVL41
	.2byte	0x1
	.byte	0x64
	.8byte	.LVL41
	.8byte	.LFE221
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x51
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST14:
	.8byte	.LVL34
	.8byte	.LVL38
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST15:
	.8byte	.LVL34
	.8byte	.LVL38
	.2byte	0x1
	.byte	0x64
	.8byte	0
	.8byte	0
.LLST0:
	.8byte	.LVL0
	.8byte	.LVL5
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL5
	.8byte	.LVL12
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	.LVL12
	.8byte	.LVL30
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL30
	.8byte	.LVL31
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	.LVL31
	.8byte	.LVL32
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL32
	.8byte	.LFE219
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST1:
	.8byte	.LVL0
	.8byte	.LVL2
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL2
	.8byte	.LVL31
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x51
	.byte	0x9f
	.8byte	.LVL31
	.8byte	.LFE219
	.2byte	0x1
	.byte	0x51
	.8byte	0
	.8byte	0
.LLST2:
	.8byte	.LVL4
	.8byte	.LVL12
	.2byte	0x1
	.byte	0x53
	.8byte	.LVL14
	.8byte	.LVL15
	.2byte	0x1
	.byte	0x53
	.8byte	.LVL17
	.8byte	.LVL18
	.2byte	0x1
	.byte	0x53
	.8byte	.LVL20
	.8byte	.LVL21
	.2byte	0x1
	.byte	0x53
	.8byte	.LVL23
	.8byte	.LVL24
	.2byte	0x1
	.byte	0x53
	.8byte	.LVL26
	.8byte	.LVL27
	.2byte	0x1
	.byte	0x53
	.8byte	.LVL29
	.8byte	.LVL31
	.2byte	0x1
	.byte	0x53
	.8byte	0
	.8byte	0
.LLST3:
	.8byte	.LVL3
	.8byte	.LVL12
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL13
	.8byte	.LVL15
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL16
	.8byte	.LVL18
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL19
	.8byte	.LVL21
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL22
	.8byte	.LVL24
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL25
	.8byte	.LVL27
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL28
	.8byte	.LVL31
	.2byte	0x1
	.byte	0x51
	.8byte	0
	.8byte	0
.LLST4:
	.8byte	.LVL11
	.8byte	.LVL12
	.2byte	0xd
	.byte	0x75
	.sleb128 0
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x8
	.byte	0x20
	.byte	0x26
	.byte	0x32
	.byte	0x24
	.byte	0x73
	.sleb128 0
	.byte	0x22
	.8byte	0
	.8byte	0
.LLST5:
	.8byte	.LVL11
	.8byte	.LVL12
	.2byte	0x1
	.byte	0x55
	.8byte	0
	.8byte	0
.LLST6:
	.8byte	.LVL4
	.8byte	.LVL5
	.2byte	0x13
	.byte	0x70
	.sleb128 0
	.byte	0x12
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x30
	.byte	0x16
	.byte	0x14
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x2b
	.byte	0x28
	.2byte	0x1
	.byte	0x16
	.byte	0x13
	.byte	0x9f
	.8byte	.LVL5
	.8byte	.LVL6
	.2byte	0x14
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x12
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x30
	.byte	0x16
	.byte	0x14
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x2b
	.byte	0x28
	.2byte	0x1
	.byte	0x16
	.byte	0x13
	.byte	0x9f
	.8byte	.LVL6
	.8byte	.LVL11
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL30
	.8byte	.LVL31
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST7:
	.8byte	.LVL4
	.8byte	.LVL11
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL30
	.8byte	.LVL31
	.2byte	0x1
	.byte	0x51
	.8byte	0
	.8byte	0
.LLST8:
	.8byte	.LVL4
	.8byte	.LVL7
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	.LVL7
	.8byte	.LVL8
	.2byte	0x1
	.byte	0x55
	.8byte	.LVL9
	.8byte	.LVL11
	.2byte	0x1
	.byte	0x55
	.8byte	0
	.8byte	0
.LLST9:
	.8byte	.LVL6
	.8byte	.LVL7
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	.LVL7
	.8byte	.LVL9
	.2byte	0x1
	.byte	0x54
	.8byte	.LVL9
	.8byte	.LVL10
	.2byte	0x3
	.byte	0x74
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL30
	.8byte	.LVL31
	.2byte	0x1
	.byte	0x54
	.8byte	0
	.8byte	0
.LLST10:
	.8byte	.LVL4
	.8byte	.LVL5
	.2byte	0x13
	.byte	0x70
	.sleb128 0
	.byte	0x12
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x30
	.byte	0x16
	.byte	0x14
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x2b
	.byte	0x28
	.2byte	0x1
	.byte	0x16
	.byte	0x13
	.byte	0x9f
	.8byte	.LVL5
	.8byte	.LVL6
	.2byte	0x14
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x12
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x30
	.byte	0x16
	.byte	0x14
	.byte	0x8
	.byte	0x20
	.byte	0x24
	.byte	0x2b
	.byte	0x28
	.2byte	0x1
	.byte	0x16
	.byte	0x13
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST11:
	.8byte	.LVL4
	.8byte	.LVL6
	.2byte	0x3
	.byte	0x8
	.byte	0x31
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST22:
	.8byte	.LVL51
	.8byte	.LVL53
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL53
	.8byte	.LVL60
	.2byte	0x1
	.byte	0x63
	.8byte	.LVL60
	.8byte	.LVL61
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL61
	.8byte	.LVL62
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	.LVL62
	.8byte	.LFE216
	.2byte	0x1
	.byte	0x63
	.8byte	0
	.8byte	0
.LLST23:
	.8byte	.LVL51
	.8byte	.LVL53
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL53
	.8byte	.LVL60
	.2byte	0x1
	.byte	0x63
	.8byte	.LVL60
	.8byte	.LVL61
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL61
	.8byte	.LVL62
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.8byte	.LVL62
	.8byte	.LFE216
	.2byte	0x1
	.byte	0x63
	.8byte	0
	.8byte	0
.LLST24:
	.8byte	.LVL52
	.8byte	.LVL53
	.2byte	0x3
	.byte	0x70
	.sleb128 4
	.byte	0x9f
	.8byte	.LVL53
	.8byte	.LVL54-1
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL54-1
	.8byte	.LVL55
	.2byte	0x3
	.byte	0x83
	.sleb128 4
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST25:
	.8byte	.LVL57
	.8byte	.LVL60
	.2byte	0x1
	.byte	0x63
	.8byte	.LVL62
	.8byte	.LVL74
	.2byte	0x1
	.byte	0x63
	.8byte	0
	.8byte	0
.LLST26:
	.8byte	.LVL57
	.8byte	.LVL60
	.2byte	0x1
	.byte	0x69
	.8byte	.LVL63
	.8byte	.LVL74
	.2byte	0x1
	.byte	0x69
	.8byte	0
	.8byte	0
.LLST27:
	.8byte	.LVL57
	.8byte	.LVL60
	.2byte	0x1
	.byte	0x68
	.8byte	.LVL63
	.8byte	.LVL64-1
	.2byte	0x2
	.byte	0x83
	.sleb128 0
	.8byte	.LVL64-1
	.8byte	.LVL73
	.2byte	0x1
	.byte	0x68
	.8byte	0
	.8byte	0
.LLST28:
	.8byte	.LVL57
	.8byte	.LVL58
	.2byte	0x8
	.byte	0x83
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x23
	.uleb128 0x1
	.byte	0x9f
	.8byte	.LVL58
	.8byte	.LVL60
	.2byte	0x6
	.byte	0x83
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	.LVL63
	.8byte	.LVL72
	.2byte	0x1
	.byte	0x63
	.8byte	.LVL72
	.8byte	.LVL73
	.2byte	0x6
	.byte	0x83
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST29:
	.8byte	.LVL57
	.8byte	.LVL58
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL72
	.8byte	.LVL73
	.2byte	0x1
	.byte	0x51
	.8byte	0
	.8byte	0
.LLST30:
	.8byte	.LVL57
	.8byte	.LVL60
	.2byte	0x1
	.byte	0x52
	.8byte	.LVL63
	.8byte	.LVL72
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	.LVL72
	.8byte	.LVL73
	.2byte	0x1
	.byte	0x52
	.8byte	0
	.8byte	0
.LLST31:
	.8byte	.LVL64
	.8byte	.LVL65
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST32:
	.8byte	.LVL66
	.8byte	.LVL67
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	.LVL67
	.8byte	.LVL70
	.2byte	0x5
	.byte	0x38
	.byte	0x72
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.8byte	.LVL70
	.8byte	.LVL71
	.2byte	0x5
	.byte	0x39
	.byte	0x72
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.8byte	.LVL71
	.8byte	.LVL72
	.2byte	0x5
	.byte	0x38
	.byte	0x72
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.8byte	0
	.8byte	0
.LLST33:
	.8byte	.LVL66
	.8byte	.LVL69
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL70
	.8byte	.LVL72
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LLST34:
	.8byte	.LVL57
	.8byte	.LVL60
	.2byte	0x7
	.byte	0x87
	.sleb128 0
	.byte	0x73
	.sleb128 0
	.byte	0x22
	.byte	0x34
	.byte	0x1c
	.8byte	.LVL66
	.8byte	.LVL67
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.8byte	.LVL67
	.8byte	.LVL68
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL70
	.8byte	.LVL72
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL72
	.8byte	.LVL74-1
	.2byte	0x7
	.byte	0x87
	.sleb128 0
	.byte	0x73
	.sleb128 0
	.byte	0x22
	.byte	0x34
	.byte	0x1c
	.8byte	.LVL75
	.8byte	.LVL76-1
	.2byte	0x7
	.byte	0x87
	.sleb128 0
	.byte	0x73
	.sleb128 0
	.byte	0x22
	.byte	0x34
	.byte	0x1c
	.8byte	0
	.8byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x7c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x8
	.byte	0
	.2byte	0
	.2byte	0
	.8byte	.LFB219
	.8byte	.LFE219-.LFB219
	.8byte	.LFB221
	.8byte	.LFE221-.LFB221
	.8byte	.LFB225
	.8byte	.LFE225-.LFB225
	.8byte	.LFB216
	.8byte	.LFE216-.LFB216
	.8byte	.LFB217
	.8byte	.LFE217-.LFB217
	.8byte	.LFB229
	.8byte	.LFE229-.LFB229
	.8byte	0
	.8byte	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.8byte	.LBB31
	.8byte	.LBE31
	.8byte	.LBB38
	.8byte	.LBE38
	.8byte	0
	.8byte	0
	.8byte	.LBB33
	.8byte	.LBE33
	.8byte	.LBB35
	.8byte	.LBE35
	.8byte	0
	.8byte	0
	.8byte	.LBB34
	.8byte	.LBE34
	.8byte	.LBB36
	.8byte	.LBE36
	.8byte	0
	.8byte	0
	.8byte	.LBB39
	.8byte	.LBE39
	.8byte	.LBB42
	.8byte	.LBE42
	.8byte	0
	.8byte	0
	.8byte	.LBB53
	.8byte	.LBE53
	.8byte	.LBB70
	.8byte	.LBE70
	.8byte	0
	.8byte	0
	.8byte	.LBB55
	.8byte	.LBE55
	.8byte	.LBB67
	.8byte	.LBE67
	.8byte	.LBB68
	.8byte	.LBE68
	.8byte	0
	.8byte	0
	.8byte	.LBB57
	.8byte	.LBE57
	.8byte	.LBB62
	.8byte	.LBE62
	.8byte	.LBB63
	.8byte	.LBE63
	.8byte	.LBB64
	.8byte	.LBE64
	.8byte	0
	.8byte	0
	.8byte	.LBB106
	.8byte	.LBE106
	.8byte	.LBB120
	.8byte	.LBE120
	.8byte	0
	.8byte	0
	.8byte	.LBB108
	.8byte	.LBE108
	.8byte	.LBB116
	.8byte	.LBE116
	.8byte	0
	.8byte	0
	.8byte	.LBB121
	.8byte	.LBE121
	.8byte	.LBB132
	.8byte	.LBE132
	.8byte	0
	.8byte	0
	.8byte	.LFB219
	.8byte	.LFE219
	.8byte	.LFB221
	.8byte	.LFE221
	.8byte	.LFB225
	.8byte	.LFE225
	.8byte	.LFB216
	.8byte	.LFE216
	.8byte	.LFB217
	.8byte	.LFE217
	.8byte	.LFB229
	.8byte	.LFE229
	.8byte	0
	.8byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF368:
	.string	"crccheck"
.LASF359:
	.string	"level"
.LASF82:
	.string	"off_mem_rsvmap"
.LASF165:
	.string	"UCLASS_I2C_EEPROM"
.LASF319:
	.string	"gld16_temp_list"
.LASF17:
	.string	"size_t"
.LASF170:
	.string	"UCLASS_IRQ"
.LASF132:
	.string	"initrd_start"
.LASF164:
	.string	"UCLASS_I2C"
.LASF13:
	.string	"sizetype"
.LASF252:
	.string	"net_hostname"
.LASF36:
	.string	"start"
.LASF205:
	.string	"UCLASS_SPI"
.LASF168:
	.string	"UCLASS_I2S"
.LASF274:
	.string	"NETLOOP_RESTART"
.LASF166:
	.string	"UCLASS_I2C_GENERIC"
.LASF178:
	.string	"UCLASS_MOD_EXP"
.LASF228:
	.string	"UCLASS_IO_DOMAIN"
.LASF291:
	.string	"WF_TYPE_GL16"
.LASF199:
	.string	"UCLASS_RKNAND"
.LASF141:
	.string	"UCLASS_DEMO"
.LASF324:
	.string	"gl16_list"
.LASF90:
	.string	"ih_magic"
.LASF321:
	.string	"a2_temp_list"
.LASF145:
	.string	"UCLASS_TEST_PROBE"
.LASF224:
	.string	"UCLASS_KEY"
.LASF62:
	.string	"bi_intfreq"
.LASF11:
	.string	"phys_addr_t"
.LASF219:
	.string	"UCLASS_VIDEO_BRIDGE"
.LASF5:
	.string	"__u8"
.LASF215:
	.string	"UCLASS_USB_DEV_GENERIC"
.LASF357:
	.string	"wf_offset"
.LASF273:
	.string	"NETLOOP_CONTINUE"
.LASF208:
	.string	"UCLASS_SPI_GENERIC"
.LASF38:
	.string	"flash_info_t"
.LASF183:
	.string	"UCLASS_PANEL"
.LASF107:
	.string	"comp"
.LASF102:
	.string	"image_header_t"
.LASF137:
	.string	"state"
.LASF158:
	.string	"UCLASS_CROS_EC"
.LASF392:
	.string	"/home/lyx/rk3566-11-eink/u-boot"
.LASF56:
	.string	"bi_dsp_freq"
.LASF104:
	.string	"image_start"
.LASF143:
	.string	"UCLASS_TEST_FDT"
.LASF47:
	.string	"bd_info"
.LASF341:
	.string	"parse_wf_gray2"
.LASF271:
	.string	"uclass_id"
.LASF94:
	.string	"ih_load"
.LASF242:
	.string	"__dtb_dt_spl_begin"
.LASF338:
	.string	"parse_wf_a2"
.LASF7:
	.string	"__u32"
.LASF148:
	.string	"UCLASS_PCI_EMUL"
.LASF297:
	.string	"epd_lut_data"
.LASF258:
	.string	"net_tx_packet"
.LASF223:
	.string	"UCLASS_FG"
.LASF257:
	.string	"net_server_ip"
.LASF349:
	.string	"parse_wf_gray16"
.LASF265:
	.string	"net_native_vlan"
.LASF221:
	.string	"UCLASS_VIDEO_CRTC"
.LASF284:
	.string	"WF_TYPE_RESET"
.LASF157:
	.string	"UCLASS_CODEC"
.LASF390:
	.ascii	"GNU C11 6.3.1 20170404 -ms"
	.string	"trict-align -march=armv8-a+nosimd -mlittle-endian -mabi=lp64 -g -Os -fno-builtin -ffreestanding -fshort-wchar -fno-stack-protector -fno-delete-null-pointer-checks -fstack-usage -fno-pic -ffunction-sections -fdata-sections -ffixed-r9 -fno-common -ffixed-x18"
.LASF23:
	.string	"errno"
.LASF303:
	.string	"RKF_WF_GC16"
.LASF15:
	.string	"long int"
.LASF51:
	.string	"bi_flashsize"
.LASF188:
	.string	"UCLASS_PHY"
.LASF75:
	.string	"IRQ_STACK_START_IN"
.LASF86:
	.string	"size_dt_strings"
.LASF218:
	.string	"UCLASS_VIDEO"
.LASF268:
	.string	"net_boot_file_size"
.LASF311:
	.string	"timeandday"
.LASF96:
	.string	"ih_dcrc"
.LASF61:
	.string	"bi_ethspeed"
.LASF140:
	.string	"UCLASS_ROOT"
.LASF351:
	.string	"get_wf_frm_num"
.LASF29:
	.string	"ide_bus_offset"
.LASF255:
	.string	"net_server_ethaddr"
.LASF64:
	.string	"bi_arch_number"
.LASF3:
	.string	"signed char"
.LASF171:
	.string	"UCLASS_KEYBOARD"
.LASF20:
	.string	"uint8_t"
.LASF98:
	.string	"ih_arch"
.LASF79:
	.string	"totalsize"
.LASF91:
	.string	"ih_hcrc"
.LASF344:
	.string	"idata"
.LASF106:
	.string	"load"
.LASF290:
	.string	"WF_TYPE_GC16"
.LASF213:
	.string	"UCLASS_TPM"
.LASF41:
	.string	"lmb_property"
.LASF84:
	.string	"last_comp_version"
.LASF0:
	.string	"unsigned char"
.LASF66:
	.string	"bi_andr_version"
.LASF139:
	.string	"images"
.LASF227:
	.string	"UCLASS_DVFS"
.LASF57:
	.string	"bi_ddr_freq"
.LASF172:
	.string	"UCLASS_LED"
.LASF93:
	.string	"ih_size"
.LASF202:
	.string	"UCLASS_SCMI_AGENT"
.LASF231:
	.string	"UCLASS_MDIO"
.LASF25:
	.string	"_Bool"
.LASF277:
	.string	"net_state"
.LASF167:
	.string	"UCLASS_I2C_MUX"
.LASF298:
	.string	"frame_num"
.LASF14:
	.string	"char"
.LASF27:
	.string	"_binary_u_boot_bin_start"
.LASF389:
	.string	"memset"
.LASF217:
	.string	"UCLASS_USB_GADGET_GENERIC"
.LASF275:
	.string	"NETLOOP_SUCCESS"
.LASF331:
	.string	"output"
.LASF337:
	.string	"pbuf"
.LASF281:
	.string	"p_current"
.LASF272:
	.string	"net_loop_state"
.LASF356:
	.string	"templist"
.LASF305:
	.string	"RKF_WF_GLR16"
.LASF156:
	.string	"UCLASS_AMP"
.LASF194:
	.string	"UCLASS_PWRSEQ"
.LASF122:
	.string	"fit_hdr_fdt"
.LASF266:
	.string	"net_restart_wrap"
.LASF254:
	.string	"net_ethaddr"
.LASF363:
	.string	"_min2"
.LASF383:
	.string	"flags"
.LASF343:
	.string	"gray"
.LASF99:
	.string	"ih_type"
.LASF80:
	.string	"off_dt_struct"
.LASF52:
	.string	"bi_flashoffset"
.LASF267:
	.string	"net_boot_file_name"
.LASF367:
	.string	"check_wf_crc"
.LASF142:
	.string	"UCLASS_TEST"
.LASF92:
	.string	"ih_time"
.LASF358:
	.string	"get_wf_level"
.LASF195:
	.string	"UCLASS_RAM"
.LASF161:
	.string	"UCLASS_ETH"
.LASF103:
	.string	"image_info"
.LASF348:
	.string	"temp_data"
.LASF109:
	.string	"arch"
.LASF328:
	.string	"a2_list"
.LASF340:
	.string	"input"
.LASF336:
	.string	"temp"
.LASF31:
	.string	"select_hwpart"
.LASF259:
	.string	"net_rx_packets"
.LASF278:
	.string	"mem_malloc_start"
.LASF74:
	.string	"_datarelro_start_ofs"
.LASF19:
	.string	"ulong"
.LASF339:
	.string	"parse_wf_auto"
.LASF95:
	.string	"ih_ep"
.LASF43:
	.string	"lmb_region"
.LASF354:
	.string	"waveform"
.LASF388:
	.string	"printf"
.LASF117:
	.string	"fit_uname_os"
.LASF58:
	.string	"bi_bootflags"
.LASF256:
	.string	"net_ip"
.LASF250:
	.string	"net_dns_server"
.LASF270:
	.string	"net_ping_ip"
.LASF45:
	.string	"memory"
.LASF77:
	.string	"fdt_header"
.LASF279:
	.string	"mem_malloc_end"
.LASF174:
	.string	"UCLASS_MAILBOX"
.LASF129:
	.string	"rd_end"
.LASF364:
	.string	"rkf_wf_get_lut"
.LASF134:
	.string	"cmdline_start"
.LASF333:
	.string	"temperture"
.LASF345:
	.string	"odata"
.LASF322:
	.string	"reset_list"
.LASF203:
	.string	"UCLASS_SCSI"
.LASF391:
	.string	"drivers/video/rk_eink/epdlut/rkf_waveform.c"
.LASF175:
	.string	"UCLASS_MASS_STORAGE"
.LASF1:
	.string	"long unsigned int"
.LASF150:
	.string	"UCLASS_SIMPLE_BUS"
.LASF301:
	.string	"RKF_WF_RESET"
.LASF310:
	.string	"format"
.LASF251:
	.string	"net_nis_domain"
.LASF286:
	.string	"WF_TYPE_GRAY4"
.LASF360:
	.string	"_max1"
.LASF361:
	.string	"_max2"
.LASF124:
	.string	"fit_noffset_fdt"
.LASF276:
	.string	"NETLOOP_FAIL"
.LASF222:
	.string	"UCLASS_WDT"
.LASF53:
	.string	"bi_sramstart"
.LASF28:
	.string	"_binary_u_boot_bin_end"
.LASF85:
	.string	"boot_cpuid_phys"
.LASF120:
	.string	"fit_uname_rd"
.LASF320:
	.string	"du_temp_list"
.LASF209:
	.string	"UCLASS_SYSCON"
.LASF115:
	.string	"fit_uname_cfg"
.LASF204:
	.string	"UCLASS_SERIAL"
.LASF264:
	.string	"net_our_vlan"
.LASF153:
	.string	"UCLASS_BLK"
.LASF110:
	.string	"image_info_t"
.LASF149:
	.string	"UCLASS_USB_EMUL"
.LASF385:
	.string	"kmalloc"
.LASF306:
	.string	"RKF_WF_GLD16"
.LASF187:
	.string	"UCLASS_PCI_GENERIC"
.LASF330:
	.string	"crc32_table"
.LASF105:
	.string	"image_len"
.LASF318:
	.string	"glr16_temp_list"
.LASF59:
	.string	"bi_ip_addr"
.LASF369:
	.string	"readcrc"
.LASF189:
	.string	"UCLASS_PINCONFIG"
.LASF347:
	.string	"decode_wf_reset"
.LASF211:
	.string	"UCLASS_THERMAL"
.LASF113:
	.string	"legacy_hdr_os_copy"
.LASF9:
	.string	"long long int"
.LASF350:
	.string	"decode_wf_data"
.LASF292:
	.string	"WF_TYPE_GLR16"
.LASF24:
	.string	"___strtok"
.LASF269:
	.string	"net_boot_file_expected_size_in_blocks"
.LASF16:
	.string	"__kernel_size_t"
.LASF182:
	.string	"UCLASS_NVME"
.LASF111:
	.string	"bootm_headers"
.LASF37:
	.string	"protect"
.LASF214:
	.string	"UCLASS_USB"
.LASF225:
	.string	"UCLASS_RC"
.LASF159:
	.string	"UCLASS_DISPLAY"
.LASF263:
	.string	"net_null_ethaddr"
.LASF327:
	.string	"du_list"
.LASF226:
	.string	"UCLASS_CHARGE_DISPLAY"
.LASF243:
	.string	"load_addr"
.LASF116:
	.string	"fit_hdr_os"
.LASF374:
	.string	"n_accum"
.LASF72:
	.string	"_datarelrolocal_start_ofs"
.LASF8:
	.string	"unsigned int"
.LASF18:
	.string	"ushort"
.LASF346:
	.string	"temp1"
.LASF295:
	.string	"WF_TYPE_GRAY32"
.LASF191:
	.string	"UCLASS_PMIC"
.LASF112:
	.string	"legacy_hdr_os"
.LASF81:
	.string	"off_dt_strings"
.LASF240:
	.string	"monitor_flash_len"
.LASF176:
	.string	"UCLASS_MISC"
.LASF49:
	.string	"bi_memsize"
.LASF308:
	.string	"rkf_waveform"
.LASF329:
	.string	"global_waveform"
.LASF26:
	.string	"image_base"
.LASF379:
	.string	"n_data"
.LASF210:
	.string	"UCLASS_SYSRESET"
.LASF288:
	.string	"WF_TYPE_AUTO"
.LASF282:
	.string	"current"
.LASF307:
	.string	"RKF_WF_A2"
.LASF163:
	.string	"UCLASS_FIRMWARE"
.LASF173:
	.string	"UCLASS_LPC"
.LASF169:
	.string	"UCLASS_IDE"
.LASF355:
	.string	"list"
.LASF381:
	.string	"block"
.LASF138:
	.string	"bootm_headers_t"
.LASF55:
	.string	"bi_arm_freq"
.LASF197:
	.string	"UCLASS_REMOTEPROC"
.LASF108:
	.string	"type"
.LASF119:
	.string	"fit_hdr_rd"
.LASF54:
	.string	"bi_sramsize"
.LASF315:
	.string	"reset_temp_list"
.LASF162:
	.string	"UCLASS_GPIO"
.LASF192:
	.string	"UCLASS_PWM"
.LASF40:
	.string	"long double"
.LASF387:
	.string	"strncmp"
.LASF325:
	.string	"glr16_list"
.LASF128:
	.string	"rd_start"
.LASF193:
	.string	"UCLASS_POWER_DOMAIN"
.LASF196:
	.string	"UCLASS_REGULATOR"
.LASF245:
	.string	"save_size"
.LASF154:
	.string	"UCLASS_CLK"
.LASF71:
	.string	"_datarel_start_ofs"
.LASF101:
	.string	"ih_name"
.LASF97:
	.string	"ih_os"
.LASF293:
	.string	"WF_TYPE_GLD16"
.LASF135:
	.string	"cmdline_end"
.LASF260:
	.string	"net_rx_packet"
.LASF239:
	.string	"LOGF_MAX_CATEGORIES"
.LASF373:
	.string	"a_size"
.LASF33:
	.string	"size"
.LASF10:
	.string	"long long unsigned int"
.LASF89:
	.string	"image_header"
.LASF309:
	.string	"length"
.LASF287:
	.string	"WF_TYPE_GRAY2"
.LASF21:
	.string	"__be32"
.LASF50:
	.string	"bi_flashstart"
.LASF118:
	.string	"fit_noffset_os"
.LASF362:
	.string	"_min1"
.LASF177:
	.string	"UCLASS_MMC"
.LASF238:
	.string	"UCLASS_INVALID"
.LASF302:
	.string	"RKF_WF_DU"
.LASF216:
	.string	"UCLASS_USB_HUB"
.LASF232:
	.string	"UCLASS_EBC"
.LASF296:
	.string	"WF_TYPE_MAX"
.LASF394:
	.string	"rkf_wf_get_version"
.LASF39:
	.string	"flash_info"
.LASF83:
	.string	"version"
.LASF44:
	.string	"region"
.LASF87:
	.string	"size_dt_struct"
.LASF234:
	.string	"UCLASS_RNG"
.LASF179:
	.string	"UCLASS_MTD"
.LASF378:
	.string	"crc_table"
.LASF76:
	.string	"fdt32_t"
.LASF332:
	.string	"lut_type"
.LASF181:
	.string	"UCLASS_NORTHBRIDGE"
.LASF60:
	.string	"bi_enetaddr"
.LASF382:
	.string	"kzalloc"
.LASF393:
	.string	"mtd_info"
.LASF246:
	.string	"in_addr"
.LASF261:
	.string	"net_rx_packet_len"
.LASF334:
	.string	"stype"
.LASF241:
	.string	"__dtb_dt_begin"
.LASF198:
	.string	"UCLASS_RESET"
.LASF121:
	.string	"fit_noffset_rd"
.LASF152:
	.string	"UCLASS_AHCI"
.LASF12:
	.string	"phys_size_t"
.LASF155:
	.string	"UCLASS_CPU"
.LASF70:
	.string	"FIQ_STACK_START"
.LASF136:
	.string	"verify"
.LASF30:
	.string	"name"
.LASF233:
	.string	"UCLASS_EINK_DISPLAY"
.LASF184:
	.string	"UCLASS_PANEL_BACKLIGHT"
.LASF212:
	.string	"UCLASS_TIMER"
.LASF207:
	.string	"UCLASS_SPI_FLASH"
.LASF316:
	.string	"gc16_temp_list"
.LASF63:
	.string	"bi_busfreq"
.LASF6:
	.string	"short int"
.LASF190:
	.string	"UCLASS_PINCTRL"
.LASF200:
	.string	"UCLASS_RAMDISK"
.LASF353:
	.string	"mode"
.LASF376:
	.string	"crc_build_table32"
.LASF147:
	.string	"UCLASS_I2C_EMUL"
.LASF206:
	.string	"UCLASS_SPMI"
.LASF144:
	.string	"UCLASS_TEST_BUS"
.LASF371:
	.string	"crc_32"
.LASF300:
	.string	"rkf_waveform_type"
.LASF244:
	.string	"save_addr"
.LASF133:
	.string	"initrd_end"
.LASF380:
	.string	"kfree"
.LASF342:
	.string	"parse_wf_reset"
.LASF326:
	.string	"gld16_list"
.LASF73:
	.string	"_datarellocal_start_ofs"
.LASF313:
	.string	"panel_info"
.LASF237:
	.string	"UCLASS_COUNT"
.LASF32:
	.string	"block_drvr"
.LASF386:
	.string	"memcpy"
.LASF22:
	.string	"gfp_t"
.LASF35:
	.string	"flash_id"
.LASF229:
	.string	"UCLASS_CRYPTO"
.LASF127:
	.string	"fit_noffset_setup"
.LASF88:
	.string	"working_fdt"
.LASF253:
	.string	"net_root_path"
.LASF160:
	.string	"UCLASS_DMA"
.LASF68:
	.string	"bd_t"
.LASF235:
	.string	"UCLASS_DMC"
.LASF247:
	.string	"s_addr"
.LASF352:
	.string	"get_wf_buf"
.LASF314:
	.string	"full_version"
.LASF262:
	.string	"net_bcast_ethaddr"
.LASF65:
	.string	"bi_boot_params"
.LASF335:
	.string	"sftemp"
.LASF280:
	.string	"mem_malloc_brk"
.LASF248:
	.string	"net_gateway"
.LASF48:
	.string	"bi_memstart"
.LASF283:
	.string	"epd_lut_type"
.LASF220:
	.string	"UCLASS_VIDEO_CONSOLE"
.LASF289:
	.string	"WF_TYPE_A2"
.LASF46:
	.string	"reserved"
.LASF384:
	.string	"free"
.LASF236:
	.string	"UCLASS_PD"
.LASF67:
	.string	"bi_dram"
.LASF377:
	.string	"a_poly"
.LASF2:
	.string	"short unsigned int"
.LASF78:
	.string	"magic"
.LASF372:
	.string	"a_data"
.LASF125:
	.string	"fit_hdr_setup"
.LASF294:
	.string	"WF_TYPE_GCC16"
.LASF114:
	.string	"legacy_hdr_valid"
.LASF42:
	.string	"base"
.LASF201:
	.string	"UCLASS_RTC"
.LASF34:
	.string	"sector_count"
.LASF100:
	.string	"ih_comp"
.LASF285:
	.string	"WF_TYPE_GRAY16"
.LASF130:
	.string	"ft_addr"
.LASF230:
	.string	"UCLASS_ETH_PHY"
.LASF304:
	.string	"RKF_WF_GL16"
.LASF370:
	.string	"check_wf_format"
.LASF365:
	.string	"rkf_wf_input"
.LASF4:
	.string	"uchar"
.LASF131:
	.string	"ft_len"
.LASF375:
	.string	"decode_wf_auto"
.LASF317:
	.string	"gl16_temp_list"
.LASF299:
	.string	"data"
.LASF180:
	.string	"UCLASS_NOP"
.LASF126:
	.string	"fit_uname_setup"
.LASF185:
	.string	"UCLASS_PCH"
.LASF186:
	.string	"UCLASS_PCI"
.LASF249:
	.string	"net_netmask"
.LASF323:
	.string	"gc16_list"
.LASF69:
	.string	"IRQ_STACK_START"
.LASF151:
	.string	"UCLASS_ADC"
.LASF366:
	.string	"waveform_file"
.LASF146:
	.string	"UCLASS_SPI_EMUL"
.LASF312:
	.string	"panel_name"
.LASF123:
	.string	"fit_uname_fdt"
	.hidden	free
	.ident	"GCC: (Linaro GCC 6.3-2017.05) 6.3.1 20170404"
	.section	.note.GNU-stack,"",@progbits
