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
	.file	"h265e_dpb.c"
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
	.type	h265e_find_cpb_frame.constprop.3, %function
h265e_find_cpb_frame.constprop.3:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r0
	ldrb	r5, [r1]	@ zero_extendqisi2
	mov	r6, r1
	ldrh	r7, [r1, #6]
	ands	r5, r5, #1
	beq	.L1
	ldr	r3, .L33
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #128
	beq	.L3
	mov	r3, r7
	movw	r2, #505
	ldr	r1, .L33+4
	ldr	r0, .L33+8
	bl	printk
.L3:
	movs	r3, #0
.L9:
	ldr	r2, [r4, #32]
	mov	r5, r4
	cmp	r2, #0
	beq	.L4
	ldrb	r1, [r4, #24]	@ zero_extendqisi2
	mov	r2, r1
	lsls	r1, r1, #31
	bpl	.L4
	ldrh	r1, [r4, #30]
	cmp	r7, r1
	bne	.L4
	ldr	r1, [r8]
	lsls	r0, r1, #24
	bpl	.L5
	and	r2, r2, #1
	ldr	r1, .L33+4
	strd	r3, r2, [sp]
	mov	r3, r7
	movw	r2, #515
	ldr	r0, .L33+12
	bl	printk
.L5:
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	ldrb	r2, [r6]	@ zero_extendqisi2
	eors	r3, r3, r2
	lsls	r1, r3, #25
	bpl	.L6
	mov	r1, #516
	ldr	r3, .L33+16
	str	r1, [sp]
	ldr	r2, .L33+20
	ldr	r0, .L33+24
	bl	printk
.L6:
	ldrb	r2, [r6]	@ zero_extendqisi2
	ldrb	r3, [r4, #24]	@ zero_extendqisi2
	eors	r3, r3, r2
	lsls	r2, r3, #24
	bpl	.L7
	movw	r1, #517
	ldr	r3, .L33+16
	str	r1, [sp]
	ldr	r2, .L33+28
	ldr	r0, .L33+24
	bl	printk
.L7:
	ldrb	r3, [r4, #25]	@ zero_extendqisi2
	ldrb	r2, [r6, #1]	@ zero_extendqisi2
	eors	r3, r3, r2
	lsls	r3, r3, #28
	beq	.L8
	movw	r1, #518
	ldr	r3, .L33+16
	str	r1, [sp]
	ldr	r2, .L33+32
	ldr	r0, .L33+24
	bl	printk
.L8:
	ldrb	r3, [r4, #25]	@ zero_extendqisi2
	ldrb	r2, [r6, #1]	@ zero_extendqisi2
	eors	r3, r3, r2
	tst	r3, #240
	beq	.L1
	movw	r1, #519
	ldr	r3, .L33+16
	str	r1, [sp]
	ldr	r2, .L33+36
	ldr	r0, .L33+24
	bl	printk
.L1:
	mov	r0, r5
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L4:
	adds	r3, r3, #1
	adds	r4, r4, #88
	cmp	r3, #16
	bne	.L9
	mov	r3, r7
	mov	r2, #524
	ldr	r1, .L33+4
	movs	r5, #0
	ldr	r0, .L33+40
	bl	printk
	b	.L1
.L34:
	.align	2
.L33:
	.word	h265e_debug
	.word	.LANCHOR0
	.word	.LC0
	.word	.LC1
	.word	.LANCHOR0+21
	.word	.LC2
	.word	.LC3
	.word	.LC4
	.word	.LC5
	.word	.LC6
	.word	.LC7
	.fnend
	.size	h265e_find_cpb_frame.constprop.3, .-h265e_find_cpb_frame.constprop.3
	.align	1
	.global	h265e_dpb_set_ref_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_set_ref_list, %function
h265e_dpb_set_ref_list:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r7, r1
	ldr	r6, .L63
	mov	r10, r2
	ldr	r5, [r0, #844]
	ldr	r3, [r6]
	lsls	r0, r3, #31
	bpl	.L36
	movs	r2, #28
	ldr	r1, .L63+4
	ldr	r0, .L63+8
	bl	printk
.L36:
	add	r9, r5, #8
	add	r1, r5, #136
	movs	r3, #0
	strd	r3, r3, [r5]
	mov	r2, r9
.L37:
	str	r3, [r2], #4
	cmp	r2, r1
	bne	.L37
	ldr	r3, [r7, #336]
	cmp	r3, #1
	ble	.L39
	ldr	fp, .L63+24
	add	r8, r7, #348
	movs	r4, #0
.L43:
	ldr	r3, [r6]
	lsls	r1, r3, #24
	bpl	.L40
	ldr	r3, [r8]
	movs	r2, #42
	ldr	r1, .L63+4
	mov	r0, fp
	str	r3, [sp]
	mov	r3, r4
	bl	printk
.L40:
	ldr	r3, [r8], #4
	cmp	r3, r10
	bne	.L41
	ldr	r3, [r6]
	lsls	r2, r3, #24
	bpl	.L42
	mov	r3, r4
	movs	r2, #45
	ldr	r1, .L63+4
	ldr	r0, .L63+12
	bl	printk
.L42:
	cbz	r4, .L39
	movs	r3, #1
	str	r4, [r5, #8]
	str	r3, [r5]
.L45:
	ldr	r2, [r7, #336]
	subs	r2, r2, #1
	cmp	r2, r3
	bgt	.L47
	adds	r4, r4, #2
	movs	r3, #0
	str	r3, [r5, r4, lsl #2]
.L39:
	movs	r3, #0
	str	r3, [r5, #4]
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L35
	movs	r2, #64
	ldr	r1, .L63+4
	ldr	r0, .L63+16
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	printk
.L41:
	ldr	r3, [r7, #336]
	adds	r4, r4, #1
	cmp	r3, r4
	bgt	.L43
	movs	r1, #50
	ldr	r0, .L63+20
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	printk
.L47:
	cmp	r3, r4
	it	ne
	strne	r3, [r9, r3, lsl #2]
	adds	r3, r3, #1
	b	.L45
.L35:
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L64:
	.align	2
.L63:
	.word	h265e_debug
	.word	.LANCHOR0+42
	.word	.LC8
	.word	.LC10
	.word	.LC11
	.word	.LC12
	.word	.LC9
	.fnend
	.size	h265e_dpb_set_ref_list, .-h265e_dpb_set_ref_list
	.align	1
	.global	h265e_dpb_init_curr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_init_curr, %function
h265e_dpb_init_curr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r6, r0
	ldr	r5, .L79
	mov	r4, r1
	ldr	r2, [r5]
	lsls	r2, r2, #31
	bpl	.L66
	movs	r2, #70
	ldr	r1, .L79+4
	ldr	r0, .L79+8
	bl	printk
.L66:
	ldr	r3, [r4, #32]
	cbz	r3, .L67
	movs	r1, #71
	ldr	r3, .L79+12
	str	r1, [sp]
	ldr	r2, .L79+16
	ldr	r0, .L79+20
	bl	printk
.L67:
	ldr	r3, [r4, #56]
	str	r6, [r4]
	cbnz	r3, .L68
	mov	r1, #1520
	ldr	r0, .L79+12
	bl	mpp_osal_calloc
	str	r0, [r4, #56]
.L68:
	movs	r3, #1
	strd	r3, r3, [r4, #32]
	ldr	r3, [r6]
	str	r3, [r4, #8]
	adds	r3, r3, #1
	str	r3, [r6]
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L69
	movs	r2, #83
	ldr	r1, .L79+4
	ldr	r0, .L79+24
	bl	printk
.L69:
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L80:
	.align	2
.L79:
	.word	h265e_debug
	.word	.LANCHOR0+65
	.word	.LC8
	.word	.LANCHOR0+85
	.word	.LC13
	.word	.LC3
	.word	.LC11
	.fnend
	.size	h265e_dpb_init_curr, .-h265e_dpb_init_curr
	.align	1
	.global	h265e_dpb_get_curr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_get_curr, %function
h265e_dpb_get_curr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r0
	ldr	r5, .L98
	ldr	r3, [r5]
	lsls	r1, r3, #31
	bpl	.L82
	movs	r2, #91
	ldr	r1, .L98+4
	ldr	r0, .L98+8
	bl	printk
.L82:
	add	r0, r4, #912
	movs	r2, #0
	movs	r1, #88
.L86:
	mul	r3, r1, r2
	ldr	r6, [r0, r3]
	cbnz	r6, .L83
	add	r2, r3, #880
	add	r2, r2, r4
	str	r2, [r4, #876]
	ldr	r2, [r5]
	lsls	r2, r2, #24
	bpl	.L85
	add	r3, r3, r4
	movs	r2, #96
	ldr	r1, .L98+4
	ldr	r3, [r3, #884]
	ldr	r0, .L98+12
	bl	printk
.L85:
	ldr	r1, [r4, #876]
	mov	r0, r4
	bl	h265e_dpb_init_curr
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L87
	movs	r2, #101
	ldr	r1, .L98+4
	ldr	r0, .L98+16
	bl	printk
.L87:
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L83:
	adds	r2, r2, #1
	cmp	r2, #17
	bne	.L86
	b	.L85
.L99:
	.align	2
.L98:
	.word	h265e_debug
	.word	.LANCHOR0+105
	.word	.LC8
	.word	.LC14
	.word	.LC11
	.fnend
	.size	h265e_dpb_get_curr, .-h265e_dpb_get_curr
	.align	1
	.global	h265e_dpb_frm_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_frm_deinit, %function
h265e_dpb_frm_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	ldr	r1, [r0, #56]
	cbz	r1, .L101
	ldr	r0, .L109
	bl	mpp_osal_free
.L101:
	movs	r3, #0
	str	r3, [r4, #56]
	str	r3, [r4, #36]
	ldr	r3, .L109+4
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L102
	movs	r2, #111
	ldr	r1, .L109+8
	ldr	r0, .L109+12
	bl	printk
.L102:
	movs	r0, #0
	pop	{r4, pc}
.L110:
	.align	2
.L109:
	.word	.LANCHOR0+124
	.word	h265e_debug
	.word	.LANCHOR0+145
	.word	.LC11
	.fnend
	.size	h265e_dpb_frm_deinit, .-h265e_dpb_frm_deinit
	.align	1
	.global	h265e_dpb_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_init, %function
h265e_dpb_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r0
	ldr	r6, .L122
	ldr	r2, [r6]
	lsls	r3, r2, #31
	bpl	.L112
	movs	r2, #122
	ldr	r1, .L122+4
	ldr	r0, .L122+8
	bl	printk
.L112:
	cbnz	r7, .L113
	mov	r3, r7
	movs	r2, #124
	ldr	r1, .L122+4
	ldr	r0, .L122+12
	bl	printk
	mvn	r0, #5
.L111:
	pop	{r3, r4, r5, r6, r7, pc}
.L113:
	movw	r1, #2376
	ldr	r0, .L122+16
	bl	mpp_osal_calloc
	mov	r5, r0
	cbz	r0, .L116
	movs	r4, #0
	movs	r3, #1
	mov	r2, #768
	strd	r3, r4, [r0, #20]
	strd	r4, r4, [r0, #8]
	mov	r1, r4
	strd	r4, r4, [r0, #28]
	adds	r0, r0, #40
	bl	memset
	mov	r1, #264
	ldr	r0, .L122+16
	bl	mpp_osal_calloc
	add	r2, r5, #884
	movs	r1, #88
	str	r0, [r5, #872]
.L115:
	mul	r3, r1, r4
	str	r4, [r2, r3]
	adds	r4, r4, #1
	cmp	r4, #17
	bne	.L115
	str	r5, [r7]
	ldr	r0, [r6]
	ands	r0, r0, #1
	beq	.L111
	movs	r2, #151
	ldr	r1, .L122+4
	ldr	r0, .L122+20
	bl	printk
	movs	r0, #0
	b	.L111
.L116:
	mvn	r0, #3
	b	.L111
.L123:
	.align	2
.L122:
	.word	h265e_debug
	.word	.LANCHOR0+166
	.word	.LC8
	.word	.LC15
	.word	.LANCHOR0+181
	.word	.LC11
	.fnend
	.size	h265e_dpb_init, .-h265e_dpb_init
	.align	1
	.global	h265e_dpb_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_deinit, %function
h265e_dpb_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r4, r0
	cbz	r0, .L126
	ldr	r6, .L145
	ldr	r2, [r6]
	lsls	r2, r2, #31
	bpl	.L127
	movs	r2, #162
	ldr	r1, .L145+4
	ldr	r0, .L145+8
	bl	printk
.L127:
	add	r5, r4, #880
	addw	r7, r4, #2376
.L129:
	ldr	r3, [r5, #36]
	cbz	r3, .L128
	mov	r0, r5
	bl	h265e_dpb_frm_deinit
.L128:
	adds	r5, r5, #88
	cmp	r5, r7
	bne	.L129
	ldr	r1, [r4, #872]
	cbz	r1, .L130
	ldr	r0, .L145+12
	bl	mpp_osal_free
.L130:
	movs	r3, #0
	mov	r1, r4
	str	r3, [r4, #872]
	ldr	r0, .L145+12
	bl	mpp_osal_free
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L126
	movs	r2, #172
	ldr	r1, .L145+4
	ldr	r0, .L145+16
	bl	printk
.L126:
	movs	r0, #0
	pop	{r3, r4, r5, r6, r7, pc}
.L146:
	.align	2
.L145:
	.word	h265e_debug
	.word	.LANCHOR0+196
	.word	.LC8
	.word	.LANCHOR0+213
	.word	.LC11
	.fnend
	.size	h265e_dpb_deinit, .-h265e_dpb_deinit
	.align	1
	.global	get_nal_unit_type
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_nal_unit_type, %function
get_nal_unit_type:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	ldr	r4, .L160
	mov	r5, r1
	ldr	r2, [r4]
	lsls	r3, r2, #31
	bpl	.L148
	movs	r2, #178
	ldr	r1, .L160+4
	ldr	r0, .L160+8
	bl	printk
.L148:
	cbz	r5, .L153
	ldr	r3, [r6, #876]
	ldr	r3, [r3, #80]
	cbnz	r3, .L153
	ldr	r3, [r6, #12]
	cmp	r3, #0
	ble	.L150
	cmp	r3, r5
	bgt	.L154
.L150:
	ldr	r3, [r6, #8]
	cmp	r3, #0
	ble	.L151
	cmp	r3, r5
	bgt	.L155
.L151:
	ldr	r4, [r4]
	ands	r4, r4, #1
	beq	.L156
	movs	r2, #198
	ldr	r1, .L160+4
	ldr	r0, .L160+12
	bl	printk
.L147:
	mov	r0, r4
	pop	{r4, r5, r6, pc}
.L153:
	movs	r4, #19
	b	.L147
.L154:
	movs	r4, #9
	b	.L147
.L155:
	movs	r4, #7
	b	.L147
.L156:
	movs	r4, #1
	b	.L147
.L161:
	.align	2
.L160:
	.word	h265e_debug
	.word	.LANCHOR0+230
	.word	.LC8
	.word	.LC11
	.fnend
	.size	get_nal_unit_type, .-get_nal_unit_type
	.align	1
	.global	sort_delta_poc
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	sort_delta_poc, %function
sort_delta_poc:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	add	r3, r0, #352
	ldr	r6, [r0, #336]
	movs	r1, #1
.L163:
	cmp	r6, r1
	bgt	.L167
	ldr	r2, [r0, #340]
	add	r3, r0, #348
	asrs	r1, r2, #1
	adds	r2, r2, #86
	add	r0, r0, r2, lsl #2
	movs	r2, #0
.L168:
	cmp	r1, r2
	bgt	.L169
	pop	{r4, r5, r6, r7, pc}
.L167:
	mov	r2, r3
	sub	r3, r3, #348
	ldr	r4, [r2], #4
	ldr	r7, [r2, #60]
	ldr	ip, [r2, #124]
.L164:
	cmp	r3, r0
	bne	.L166
	adds	r1, r1, #1
	mov	r3, r2
	b	.L163
.L166:
	ldr	r5, [r3, #344]
	cmp	r4, r5
	bge	.L165
	str	r5, [r3, #348]
	ldr	r5, [r3, #408]
	str	r4, [r3, #344]
	str	r7, [r3, #408]
	str	r5, [r3, #412]
	ldr	r5, [r3, #472]
	str	ip, [r3, #472]
	str	r5, [r3, #476]
.L165:
	subs	r3, r3, #4
	b	.L164
.L169:
	ldr	r6, [r3], #4
	adds	r2, r2, #1
	ldr	r7, [r0], #-4
	ldr	r5, [r3, #60]
	str	r7, [r3, #-4]
	ldr	r7, [r0, #68]
	ldr	r4, [r3, #124]
	str	r7, [r3, #60]
	ldr	r7, [r0, #132]
	str	r7, [r3, #124]
	str	r6, [r0, #4]
	str	r5, [r0, #68]
	str	r4, [r0, #132]
	b	.L168
	.fnend
	.size	sort_delta_poc, .-sort_delta_poc
	.align	1
	.global	h265e_dpb_apply_rps
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_apply_rps, %function
h265e_dpb_apply_rps:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r6, .L209
	mov	r5, r0
	mov	r7, r1
	mov	r9, r2
	ldr	r3, [r6]
	str	r6, [sp, #8]
	tst	r3, #1
	beq	.L171
	mov	r2, #256
	ldr	r1, .L209+4
	ldr	r0, .L209+8
	bl	printk
.L171:
	ldr	r10, .L209+4
	add	r4, r5, #912
	addw	r3, r5, #2408
	str	r3, [sp, #12]
.L184:
	ldr	r3, [r4, #4]
	cbz	r3, .L173
	ldr	r3, [r4, #24]
	ldr	r3, [r3, #328]
	cbz	r3, .L173
	mov	r8, #0
	add	fp, r7, #348
	mov	r5, r8
.L175:
	ldr	r3, [sp, #8]
	ldr	r0, [r4, #24]
	ldr	r1, [r3]
	ldrd	r2, r3, [r7, #340]
	add	r3, r3, r2
	cmp	r3, r5
	bgt	.L178
	add	r2, r5, #20
	add	r2, r7, r2, lsl #2
.L179:
	ldr	r6, [r7, #336]
	ldr	r3, [r0, #16]
	cmp	r6, r5
	bgt	.L182
	cmp	r9, r3
	beq	.L173
	cmp	r8, #0
	bne	.L173
	lsls	r2, r1, #24
	bpl	.L183
	movw	r2, #303
	mov	r1, r10
	ldr	r0, .L209+12
	bl	printk
.L183:
	ldr	r2, [r4, #24]
	movs	r3, #0
	str	r3, [r2, #328]
	str	r3, [r4, #12]
	str	r3, [r4]
	str	r3, [r4, #8]
.L173:
	ldr	r3, [sp, #12]
	adds	r4, r4, #88
	cmp	r4, r3
	bne	.L184
	ldr	r3, [sp, #8]
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L170
	mov	r2, #310
	ldr	r1, .L209+4
	ldr	r0, .L209+16
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	printk
.L178:
	lsls	r3, r1, #24
	bpl	.L176
	ldr	r3, [fp]
	movw	r2, #269
	mov	r1, r10
	strd	r9, r3, [sp]
	ldr	r3, [r0, #16]
	ldr	r0, .L209+20
	bl	printk
.L176:
	ldr	r3, [r4, #8]
	cbnz	r3, .L177
	ldr	r2, [r4, #24]
	ldr	r3, [fp]
	ldr	r2, [r2, #16]
	add	r3, r3, r9
	cmp	r3, r2
	bne	.L177
	ldr	r3, [fp, #64]
	mov	r8, #1
	add	ip, r3, #-1
	rsbs	r3, ip, #0
	adc	r3, r3, ip
	str	r3, [r4, #12]
.L177:
	adds	r5, r5, #1
	add	fp, fp, #4
	b	.L175
.L182:
	ldr	ip, [r4, #8]
	ldr	lr, [r2], #4
	cmp	ip, #0
	beq	.L181
	ldr	r6, [r2, #520]
	cmp	r6, r3
	bne	.L181
	ldr	r3, [r2, #328]
	mov	r8, #1
	subs	r6, r3, #1
	rsbs	r3, r6, #0
	adcs	r3, r3, r6
	str	r3, [r4, #12]
.L181:
	adds	r5, r5, #1
	b	.L179
.L170:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L210:
	.align	2
.L209:
	.word	h265e_debug
	.word	.LANCHOR0+248
	.word	.LC8
	.word	.LC17
	.word	.LC11
	.word	.LC16
	.fnend
	.size	h265e_dpb_apply_rps, .-h265e_dpb_apply_rps
	.align	1
	.global	h265e_dpb_dec_refresh_marking
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_dec_refresh_marking, %function
h265e_dpb_dec_refresh_marking:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r2
	ldr	r6, .L234
	mov	r4, r0
	mov	r5, r1
	ldr	r3, [r6]
	lsls	r2, r3, #31
	bpl	.L212
	mov	r2, #318
	ldr	r1, .L234+4
	ldr	r0, .L234+8
	bl	printk
.L212:
	sub	ip, r7, #16
	cmp	ip, #4
	bhi	.L213
	add	r3, r4, #904
	add	lr, r4, #2400
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
.L217:
	ldr	r7, [r3, #12]
	cbz	r7, .L215
	ldr	r7, [r3, #28]
	cmp	r7, r5
	beq	.L215
	ldr	r7, [r3, #32]
	str	r2, [r7, #328]
	ldr	r7, [r3, #28]
	str	r2, [r3, #16]
	cmp	r7, r5
	bge	.L215
	str	r2, [r3, #20]
	str	r2, [r3, #8]
	strd	r0, [r3]
.L215:
	adds	r3, r3, #88
	cmp	lr, r3
	bne	.L217
	cmp	ip, #2
	bhi	.L219
.L233:
	str	r5, [r4, #12]
.L219:
	ldr	r3, [r6]
	lsls	r3, r3, #31
	bpl	.L211
	mov	r2, #364
	ldr	r1, .L234+4
	ldr	r0, .L234+12
	pop	{r3, r4, r5, r6, r7, lr}
	b	printk
.L213:
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bne	.L220
	ldr	r3, [r4, #12]
	cmp	r3, r5
	bge	.L220
	add	ip, r4, #1496
	mov	r3, r4
	movs	r2, #0
.L222:
	ldr	r1, [r3, #916]
	cbz	r1, .L221
	ldr	r1, [r3, #932]
	cmp	r1, r5
	beq	.L221
	ldr	r0, [r4, #12]
	cmp	r1, r0
	ittt	ne
	ldrne	r1, [r3, #936]
	strne	r2, [r1, #328]
	strne	r2, [r3, #912]
.L221:
	adds	r3, r3, #88
	cmp	r3, ip
	bne	.L222
	movs	r3, #0
	str	r3, [r4, #16]
.L220:
	cmp	r7, #21
	bne	.L219
	movs	r3, #1
	str	r3, [r4, #16]
	b	.L233
.L211:
	pop	{r3, r4, r5, r6, r7, pc}
.L235:
	.align	2
.L234:
	.word	h265e_debug
	.word	.LANCHOR0+268
	.word	.LC8
	.word	.LC11
	.fnend
	.size	h265e_dpb_dec_refresh_marking, .-h265e_dpb_dec_refresh_marking
	.global	__aeabi_idivmod
	.global	__aeabi_idiv
	.align	1
	.global	h265e_dpb_arrange_lt_rps
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_arrange_lt_rps, %function
h265e_dpb_arrange_lt_rps:
	.fnstart
	@ args = 0, pretend = 0, frame = 464
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r9, r1
	ldr	r3, [r1, #372]
	.pad #476
	sub	sp, sp, #476
	ldr	r4, [r1, #520]
	ldr	r5, [r3, #160]
	ldr	r3, .L273
	ldrd	r7, r6, [r4, #340]
	ldr	r3, [r3]
	lsls	r2, r3, #31
	bpl	.L237
	movw	r2, #387
	ldr	r1, .L273+4
	ldr	r0, .L273+8
	bl	printk
.L237:
	ldr	r8, [r4, #672]
	cmp	r8, #0
	beq	.L236
	adds	r3, r7, r6
	movs	r2, #64
	str	r3, [sp, #12]
	movs	r1, #0
	movs	r3, #1
	add	r0, sp, #88
	lsl	r5, r3, r5
	bl	memset
	movs	r2, #64
	movs	r1, #0
	movs	r6, #0
	add	r0, sp, #152
	bl	memset
	movs	r2, #64
	movs	r1, #0
	add	r0, sp, #344
	bl	memset
	movs	r2, #64
	movs	r1, #0
	add	r0, sp, #280
	bl	memset
	movs	r2, #64
	movs	r1, #0
	add	fp, sp, #216
	mov	r0, fp
	bl	memset
	movs	r2, #64
	movs	r1, #0
	add	r0, sp, #408
	bl	memset
	ldr	r7, [r4, #336]
	add	r10, r7, #-1
	adds	r7, r7, #134
	add	r7, r4, r7, lsl #2
	str	fp, [sp, #8]
.L239:
	ldr	r3, [sp, #12]
	cmp	r10, r3
	bge	.L242
	cmp	r8, r6
	beq	.L243
	movw	r1, #409
	ldr	r3, .L273+12
	str	r1, [sp]
	ldr	r2, .L273+16
	ldr	r0, .L273+20
	bl	printk
.L243:
	mov	ip, #0
	add	r3, r8, #-1
	str	r3, [sp, #20]
.L244:
	cmp	r8, ip
	ble	.L247
	add	r3, sp, #216
	add	r6, sp, #88
	str	r3, [sp, #8]
	add	r0, sp, #280
	movs	r3, #0
	add	r1, sp, #152
	add	r2, sp, #344
	str	r3, [sp, #16]
	b	.L248
.L242:
	ldr	fp, [r7], #-4
	add	r3, sp, #88
	cmp	fp, #0
	str	fp, [r3, r6, lsl #2]
	add	r3, sp, #280
	ldr	r1, [r7, #68]
	str	r1, [r3, r6, lsl #2]
	mov	r1, r5
	blt	.L240
	mov	r0, fp
.L271:
	bl	__aeabi_idivmod
	add	r3, sp, #152
	add	r2, sp, #344
	str	r1, [r3, r6, lsl #2]
	ldr	r3, [sp, #8]
	str	r10, [r3, r6, lsl #2]
	sub	r3, fp, r1
	add	r10, r10, #-1
	str	r3, [r2, r6, lsl #2]
	adds	r6, r6, #1
	b	.L239
.L240:
	rsb	r0, fp, #0
	bl	__aeabi_idivmod
	subs	r0, r5, r1
	mov	r1, r5
	b	.L271
.L246:
	ldr	r10, [r2]
	ldr	lr, [r2, #4]
	ldr	r7, [sp, #16]
	cmp	r10, lr
	add	r7, r7, #1
	str	r7, [sp, #16]
	bge	.L245
	ldrd	r7, fp, [r6]
	ldr	r3, [sp, #8]
	strd	lr, r10, [r2]
	strd	fp, r7, [r6]
	ldrd	r7, fp, [r0]
	strd	fp, r7, [r0]
	ldrd	r7, fp, [r1]
	strd	fp, r7, [r1]
	ldrd	r7, lr, [r3]
	strd	lr, r7, [r3]
.L245:
	ldr	r3, [sp, #8]
	adds	r6, r6, #4
	adds	r0, r0, #4
	adds	r1, r1, #4
	adds	r2, r2, #4
	adds	r3, r3, #4
	str	r3, [sp, #8]
.L248:
	ldrd	r3, r7, [sp, #16]
	cmp	r7, r3
	bgt	.L246
	add	ip, ip, #1
	b	.L244
.L247:
	ldr	r6, [r9, #20]
	add	r10, sp, #408
	movs	r7, #0
	mov	fp, #1
.L249:
	cmp	r8, r7
	bgt	.L251
	add	r10, sp, #24
	movs	r1, #0
	movs	r2, #64
	mov	r0, r10
	bl	memset
	movs	r3, #0
	add	r1, sp, #216
.L252:
	cmp	r8, r3
	bgt	.L253
	cmp	r6, #0
	mov	r1, r5
	blt	.L254
	mov	r0, r6
.L272:
	bl	__aeabi_idivmod
	ldr	r8, [r4, #336]
	movs	r7, #0
	sub	fp, r6, r1
	add	r6, r8, #35
	add	r6, r4, r6, lsl #2
.L256:
	add	r3, r8, #-1
	ldr	r2, [sp, #12]
	subs	r3, r3, r7
	cmp	r2, r3
	ble	.L258
	ldr	r8, [r4, #336]
	ldr	r9, .L273+12
	add	r6, r8, #150
	ldr	r10, .L273+32
	ldr	fp, .L273+20
	add	r7, r8, #-1
	add	r6, r4, r6, lsl #2
.L259:
	ldr	r3, [sp, #12]
	sub	r5, r8, r7
	cmp	r7, r3
	bge	.L263
	ldr	r3, .L273
	ldr	r3, [r3]
	lsls	r3, r3, #31
	bpl	.L236
	movw	r2, #467
	ldr	r1, .L273+4
	ldr	r0, .L273+24
	bl	printk
.L236:
	add	sp, sp, #476
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L251:
	mov	r1, r5
	mov	r0, r6
	bl	__aeabi_idiv
	cmp	r0, #0
	it	gt
	strgt	fp, [r10, r7, lsl #2]
	adds	r7, r7, #1
	b	.L249
.L253:
	ldr	r2, [r1, r3, lsl #2]
	add	r2, r4, r2, lsl #2
	ldr	r2, [r2, #412]
	adds	r2, r2, #0
	it	ne
	movne	r2, #1
	str	r2, [r10, r3, lsl #2]
	adds	r3, r3, #1
	b	.L252
.L254:
	rsbs	r0, r6, #0
	bl	__aeabi_idivmod
	subs	r0, r5, r1
	mov	r1, r5
	b	.L272
.L258:
	add	r3, sp, #88
	mov	r1, r5
	ldr	r0, [r3, r7, lsl #2]
	add	r3, sp, #280
	str	r0, [r6, #396]
	ldr	r3, [r3, r7, lsl #2]
	ldr	r2, [r9, #16]
	subs	r3, r3, r2
	str	r3, [r6, #204]
	ldr	r3, [r10, r7, lsl #2]
	str	r3, [r6, #268]
	add	r3, sp, #152
	ldr	r3, [r3, r7, lsl #2]
	str	r3, [r6], #-4
	add	r3, r3, fp
	subs	r0, r3, r0
	bl	__aeabi_idiv
	add	r3, sp, #408
	str	r0, [r6, #68]
	cmp	r0, #0
	ldr	r3, [r3, r7, lsl #2]
	str	r3, [r6, #132]
	bge	.L257
	mov	r1, #456
	ldr	r3, .L273+12
	str	r1, [sp]
	ldr	r2, .L273+28
	ldr	r0, .L273+20
	bl	printk
.L257:
	adds	r7, r7, #1
	b	.L256
.L263:
	ldr	r3, [r4, #336]
	subs	r3, r3, #1
	subs	r5, r3, r5
.L260:
	ldr	r3, [sp, #12]
	cmp	r5, r3
	bge	.L262
	subs	r7, r7, #1
	subs	r6, r6, #4
	b	.L259
.L262:
	add	r3, r4, #604
	ldr	r2, [r6]
	ldr	r3, [r3, r5, lsl #2]
	cmp	r2, r3
	bne	.L261
	movw	r1, #463
	mov	r3, r9
	str	r1, [sp]
	mov	r2, r10
	mov	r0, fp
	bl	printk
.L261:
	subs	r5, r5, #1
	b	.L260
.L274:
	.align	2
.L273:
	.word	h265e_debug
	.word	.LANCHOR0+298
	.word	.LC8
	.word	.LANCHOR0+323
	.word	.LC18
	.word	.LC3
	.word	.LC11
	.word	.LC19
	.word	.LC20
	.fnend
	.size	h265e_dpb_arrange_lt_rps, .-h265e_dpb_arrange_lt_rps
	.align	1
	.global	h265e_dpb_cpb2rps
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_cpb2rps, %function
h265e_dpb_cpb2rps:
	.fnstart
	@ args = 0, pretend = 0, frame = 408
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #428
	sub	sp, sp, #428
	mov	r7, r3
	add	r3, r2, #524
	str	r3, [sp, #20]
	mov	r4, r2
	ldr	r3, .L323
	str	r1, [sp, #28]
	str	r0, [sp, #16]
	ldr	r3, [r3]
	lsls	r1, r3, #31
	bpl	.L276
	movw	r2, #569
	ldr	r1, .L323+4
	ldr	r0, .L323+8
	bl	printk
.L276:
	movs	r2, #64
	movs	r1, #1
	add	r0, sp, #40
	add	r9, r4, #872
	bl	memset
	mov	r2, #676
	movs	r1, #0
	ldr	r0, [sp, #20]
	bl	memset
	add	r1, r4, #936
	mov	r3, r9
	movs	r2, #0
.L277:
	str	r2, [r3], #4
	cmp	r3, r1
	str	r2, [r3, #60]
	str	r2, [r3, #124]
	bne	.L277
	movs	r2, #64
	movs	r1, #0
	mov	r0, r9
	bl	memset
	ldrsb	r3, [r7, #8]
	cmp	r3, #0
	bge	.L278
	ldr	r3, [r4, #372]
	ldrb	r3, [r3, #1]	@ zero_extendqisi2
	cbnz	r3, .L278
	movw	r1, #582
	ldr	r3, .L323+12
	str	r1, [sp]
	ldr	r2, .L323+16
	ldr	r0, .L323+20
	bl	printk
.L278:
	movs	r5, #0
	add	r8, r7, #24
	mov	r10, r5
	mov	fp, r5
	mov	r6, r5
	str	r5, [sp, #24]
.L284:
	adds	r3, r6, #3
	ldrb	r3, [r7, r3, lsl #3]	@ zero_extendqisi2
	lsls	r2, r3, #31
	bpl	.L279
	lsls	r5, r3, #25
	bpl	.L280
	movw	r1, #591
	ldr	r3, .L323+12
	str	r1, [sp]
	ldr	r2, .L323+24
	ldr	r0, .L323+20
	bl	printk
.L280:
	ldr	r3, .L323
	ldr	r3, [r3]
	lsls	r0, r3, #24
	bpl	.L281
	adds	r2, r6, #3
	ldr	r0, .L323+28
	ldrb	r3, [r7, r2, lsl #3]	@ zero_extendqisi2
	add	r2, r7, r2, lsl #3
	lsrs	r1, r3, #7
	str	r1, [sp, #12]
	ubfx	r1, r3, #6, #1
	and	r3, r3, #1
	str	r1, [sp, #8]
	str	r3, [sp, #4]
	ldrh	r3, [r2, #6]
	movw	r2, #595
	ldr	r1, .L323+4
	str	r3, [sp]
	mov	r3, r6
	bl	printk
.L281:
	ldr	r3, [sp, #16]
	mov	r1, r8
	add	r0, r3, #880
	bl	h265e_find_cpb_frame.constprop.3
	mov	r5, r0
	cbz	r0, .L279
	ldrd	r1, [r8]
	ldr	r0, [r0, #52]
	ldr	r3, [sp, #28]
	strd	r1, [sp, #32]
	adds	r1, r6, #3
	ldr	r2, .L323
	subs	r3, r0, r3
	ldrsb	r1, [r7, r1, lsl #3]
	ldr	r2, [r2]
	cmp	r1, #0
	and	r2, r2, #128
	blt	.L282
	ldrd	r0, [sp, #32]
	strd	r0, [r5, #24]
	movs	r0, #1
	ldr	r1, [sp, #24]
	add	fp, fp, r0
	add	r1, r4, r1, lsl #2
	str	r3, [r1, #872]
	str	r0, [r1, #936]
	ldr	r1, [sp, #24]
	add	r1, r1, r0
	str	r1, [sp, #24]
	cbz	r2, .L279
	strd	r8, r3, [sp, #4]
	mov	r2, #608
	str	fp, [sp]
	mov	r3, r6
	ldr	r1, .L323+4
	ldr	r0, .L323+32
	bl	printk
.L279:
	adds	r6, r6, #1
	add	r8, r8, #8
	cmp	r6, #8
	bne	.L284
	ldr	r0, [sp, #20]
	bl	sort_delta_poc
	ldr	r3, [r4, #368]
	cmp	r3, #2
	bne	.L285
	movs	r6, #0
	str	r6, [r4, #1192]
	str	r6, [r4, #1196]
	strd	r6, r6, [r4, #864]
	str	r6, [r4, #860]
.L286:
	cmp	r10, #0
	ldr	r9, .L323
	bgt	.L291
.L295:
	add	r3, fp, r10
	str	r3, [r4, #860]
	ldr	r3, [sp, #20]
	movs	r5, #0
	ldr	r2, [sp, #28]
	ldr	r0, [sp, #16]
	mov	r1, r3
	str	r3, [r4, #520]
	str	fp, [r4, #864]
	str	r5, [r4, #868]
	str	r10, [r4, #1196]
	bl	h265e_dpb_apply_rps
	mov	r1, r4
	ldr	r0, [sp, #16]
	bl	h265e_dpb_arrange_lt_rps
	mov	r2, r6
	ldrd	r3, r1, [sp, #16]
	add	r0, r3, #28
	bl	h265e_dpb_set_ref_list
	mov	r2, #264
	add	r0, r4, #1200
	ldr	r3, [sp, #16]
	ldr	r1, [r3, #872]
	bl	memcpy
	ldr	r0, [r9]
	ands	r0, r0, #1
	beq	.L275
	movw	r2, #671
	ldr	r1, .L323+4
	ldr	r0, .L323+36
	bl	printk
	mov	r0, r5
	b	.L275
.L282:
	ldr	r1, [r5, #12]
	add	ip, sp, #104
	mov	lr, #1
	str	r1, [ip, r10, lsl #2]
	add	ip, sp, #424
	add	ip, ip, r10, lsl #2
	str	r0, [ip, #-240]
	str	r3, [ip, #-160]
	ldr	r0, [r5, #56]
	ldr	r0, [r0, #372]
	ldr	r0, [r0, #160]
	lsl	r0, lr, r0
	cmp	r1, r0
	ite	lt
	movlt	r1, #0
	movge	r1, #1
	str	r1, [ip, #-80]
	ldrd	r0, [sp, #32]
	strd	r0, [r5, #24]
	cbz	r2, .L283
	strd	r8, r3, [sp, #4]
	mov	r2, #620
	str	r10, [sp]
	mov	r3, r6
	ldr	r1, .L323+4
	ldr	r0, .L323+40
	bl	printk
.L283:
	add	r10, r10, #1
	b	.L279
.L285:
	ldr	r3, [sp, #16]
	add	r1, r7, #16
	add	r0, r3, #880
	bl	h265e_find_cpb_frame.constprop.3
	mov	r5, r0
	cbnz	r0, .L287
	ldrh	r2, [r7, #22]
	movw	r1, #638
	ldr	r0, .L323+44
	bl	printk
	mov	r0, #-1
.L275:
	add	sp, sp, #428
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L287:
	ldr	r3, [sp, #28]
	ldr	r6, [r0, #52]
	subs	r6, r6, r3
	movs	r3, #0
.L289:
	cmp	r3, fp
	bge	.L286
	ldr	r2, [r9], #4
	adds	r3, r3, #1
	subs	r1, r2, r6
	rsbs	r2, r1, #0
	adcs	r2, r2, r1
	str	r2, [r9, #124]
	b	.L289
.L291:
	add	r7, fp, #151
	mov	r8, #0
	add	r7, r4, r7, lsl #2
.L294:
	ldr	r3, [r9]
	lsls	r3, r3, #24
	bpl	.L293
	str	fp, [sp]
	mov	r3, r10
	movw	r2, #651
	ldr	r1, .L323+4
	ldr	r0, .L323+48
	bl	printk
.L293:
	add	r3, sp, #104
	ldr	r3, [r3, r8, lsl #2]
	str	r3, [r7, #460]
	add	r3, sp, #184
	ldr	r3, [r3, r8, lsl #2]
	str	r3, [r7, #524]
	movs	r3, #1
	str	r3, [r7, #332]
	ldr	r3, [r5, #40]
	str	r3, [r7, #396]
	add	r3, sp, #264
	ldr	r3, [r3, r8, lsl #2]
	str	r3, [r7, #268]
	add	r3, sp, #344
	ldr	r3, [r3, r8, lsl #2]
	add	r8, r8, #1
	cmp	r10, r8
	str	r3, [r7], #4
	bne	.L294
	b	.L295
.L324:
	.align	2
.L323:
	.word	h265e_debug
	.word	.LANCHOR0+348
	.word	.LC8
	.word	.LANCHOR0+366
	.word	.LC21
	.word	.LC3
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC11
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.fnend
	.size	h265e_dpb_cpb2rps, .-h265e_dpb_cpb2rps
	.align	1
	.global	h265e_dpb_free_unsed
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_free_unsed, %function
h265e_dpb_free_unsed:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r7, r1
	ldr	r5, .L367
	mov	r8, r0
	ldr	r3, [r5]
	lsls	r1, r3, #31
	bpl	.L326
	movw	r2, #679
	ldr	r1, .L367+4
	ldr	r0, .L367+8
	bl	printk
.L326:
	ldrb	r3, [r7, #8]	@ zero_extendqisi2
	add	r4, r8, #880
	tst	r3, #64
	beq	.L328
	add	r1, r7, #8
	mov	r0, r4
	bl	h265e_find_cpb_frame.constprop.3
	mov	r6, r0
	cbz	r0, .L328
	ldr	r3, [r5]
	lsls	r2, r3, #24
	bpl	.L330
	ldr	r3, [r0, #56]
	movw	r2, #686
	ldr	r1, .L367+4
	ldr	r0, .L367+12
	ldr	r3, [r3, #16]
	bl	printk
.L330:
	ldr	r2, [r6, #56]
	movs	r3, #0
	strd	r3, r3, [r6, #40]
	str	r3, [r6, #32]
	str	r3, [r2, #328]
.L328:
	addw	r6, r8, #2376
	ldr	r8, .L367+4
	ldr	r10, .L367+20
	add	r9, r8, #21
.L339:
	ldr	r3, [r4, #32]
	cmp	r3, #0
	beq	.L332
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L333
	movw	r2, #535
	mov	r1, r9
	ldr	r0, .L367+8
	bl	printk
.L333:
	ldrh	r1, [r4, #30]
	add	r3, r7, #88
	add	r2, r7, #216
	mov	fp, #-1
.L335:
	ldrb	r0, [r3]	@ zero_extendqisi2
	lsls	r0, r0, #31
	bpl	.L334
	ldrh	r0, [r3, #6]
	cmp	r0, r1
	it	eq
	moveq	fp, #0
.L334:
	adds	r3, r3, #8
	cmp	r2, r3
	bne	.L335
	ldr	r3, [r5]
	lsls	r1, r3, #31
	bpl	.L336
	movw	r2, #547
	mov	r1, r9
	ldr	r0, .L367+16
	bl	printk
.L336:
	cmp	fp, #0
	beq	.L332
	ldr	r3, [r5]
	lsls	r2, r3, #24
	bpl	.L338
	ldr	r3, [r4, #56]
	mov	r2, #700
	mov	r1, r8
	mov	r0, r10
	ldr	r3, [r3, #16]
	bl	printk
.L338:
	ldr	r2, [r4, #56]
	movs	r3, #0
	strd	r3, r3, [r4, #40]
	str	r3, [r4, #32]
	str	r3, [r2, #328]
.L332:
	adds	r4, r4, #88
	cmp	r4, r6
	bne	.L339
	ldr	r3, [r5]
	lsls	r3, r3, #31
	bpl	.L325
	mov	r2, #708
	ldr	r1, .L367+4
	ldr	r0, .L367+16
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	b	printk
.L325:
	pop	{r3, r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L368:
	.align	2
.L367:
	.word	h265e_debug
	.word	.LANCHOR0+384
	.word	.LC8
	.word	.LC28
	.word	.LC11
	.word	.LC29
	.fnend
	.size	h265e_dpb_free_unsed, .-h265e_dpb_free_unsed
	.align	1
	.global	h265e_dpb_proc_cpb
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_proc_cpb, %function
h265e_dpb_proc_cpb:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r7, r1
	.pad #20
	sub	sp, sp, #20
	mov	r6, r0
	cbz	r0, .L369
	cbz	r1, .L369
	ldrb	r4, [r1, #8]	@ zero_extendqisi2
	ands	r4, r4, #32
	beq	.L390
	add	r2, r0, #1496
	movs	r3, #0
	movs	r4, #0
	movs	r5, #0
.L374:
	ldr	r1, [r0, #916]
	cbz	r1, .L373
	ldr	r1, [r0, #936]
	str	r3, [r1, #328]
	strd	r3, r3, [r0, #920]
	str	r3, [r0, #912]
	strd	r4, [r0, #904]
.L373:
	adds	r0, r0, #88
	cmp	r2, r0
	bne	.L374
.L369:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L390:
	ldr	r10, .L442+28
	mov	r8, r4
	ldr	fp, .L442+44
.L372:
	add	r5, r8, #3
	ldrb	r3, [r7, r5, lsl #3]	@ zero_extendqisi2
	lsls	r1, r3, #31
	bpl	.L375
	lsls	r2, r3, #25
	bpl	.L376
	mov	r1, #744
	ldr	r3, .L442
	str	r1, [sp]
	ldr	r2, .L442+4
	ldr	r0, .L442+8
	bl	printk
.L376:
	ldr	r3, [r10]
	lsls	r3, r3, #24
	bpl	.L377
	ldrb	r3, [r7, r5, lsl #3]	@ zero_extendqisi2
	ldr	r1, .L442+12
	ldr	r0, .L442+16
	lsrs	r2, r3, #7
	str	r2, [sp, #12]
	ubfx	r2, r3, #6, #1
	and	r3, r3, #1
	str	r3, [sp, #4]
	add	r3, r7, r5, lsl #3
	str	r2, [sp, #8]
	mov	r2, #748
	ldrh	r3, [r3, #6]
	str	r3, [sp]
	mov	r3, r8
	bl	printk
.L377:
	add	r3, r7, r5, lsl #3
	ldrb	r5, [r7, r5, lsl #3]	@ zero_extendqisi2
	ands	r5, r5, #1
	ldrh	r9, [r3, #6]
	beq	.L378
	ldr	r3, [r10]
	lsls	r5, r3, #24
	bpl	.L379
	mov	r3, r9
	movw	r2, #479
	mov	r1, fp
	ldr	r0, .L442+20
	bl	printk
.L379:
	add	r3, r6, #880
	movs	r2, #0
.L381:
	ldr	r1, [r3, #36]
	mov	r5, r3
	cmp	r1, #0
	beq	.L380
	ldrb	r0, [r3, #24]	@ zero_extendqisi2
	mov	r1, r0
	lsls	r0, r0, #31
	bpl	.L380
	ldrh	r0, [r3, #30]
	cmp	r9, r0
	bne	.L380
	ldr	r3, [r10]
	lsls	r3, r3, #24
	bpl	.L378
	and	r1, r1, #1
	mov	r3, r9
	strd	r2, r1, [sp]
	mov	r2, #488
	mov	r1, fp
	ldr	r0, .L442+24
	bl	printk
.L378:
	ldr	r3, [r5, #32]
	cbnz	r3, .L375
	add	r3, r7, r8, lsl #3
	movs	r4, #1
	str	r4, [r5, #32]
	ldrd	r2, [r3, #24]
	strd	r2, [r5, #24]
	ldr	r3, [r5, #56]
	str	r4, [r3, #328]
.L375:
	add	r8, r8, #1
	cmp	r8, #8
	bne	.L372
	cbz	r4, .L382
	ldr	r3, .L442+28
	ldr	r3, [r3]
	lsls	r0, r3, #24
	bpl	.L383
	mov	r2, #760
	ldr	r1, .L442+12
	ldr	r0, .L442+32
	bl	printk
.L383:
	movs	r2, #0
	add	r4, r6, #1496
	mov	r3, r6
	mov	r0, r2
.L385:
	ldr	r1, [r3, #912]
	cbz	r1, .L384
	ldr	r1, [r3, #936]
	ldr	r5, [r1, #16]
	ldr	r1, [r1, #20]
	cmp	r2, r5
	it	lt
	movlt	r2, r5
	cmp	r0, r1
	it	lt
	movlt	r0, r1
.L384:
	adds	r3, r3, #88
	cmp	r3, r4
	bne	.L385
	ldr	r3, [r6, #876]
	ldr	r1, [r3, #36]
	cbz	r1, .L386
	ldr	r4, [r3, #56]
	movs	r1, #0
	movs	r5, #0
	str	r1, [r4, #328]
	movs	r4, #0
	strd	r1, r1, [r3, #40]
	str	r1, [r3, #32]
	strd	r4, [r3, #24]
.L386:
	strd	r2, r0, [r6]
.L382:
	ldr	r5, .L442+28
	movs	r4, #0
	ldr	r7, .L442+12
	mov	r8, #0
	mov	r9, #0
.L389:
	ldr	r3, [r6, #916]
	cbz	r3, .L387
	ldr	r2, [r6, #912]
	cbnz	r2, .L387
	ldr	r1, [r5]
	lsls	r1, r1, #24
	bpl	.L388
	strd	r3, r2, [sp]
	mov	r1, r7
	mov	r3, r4
	mov	r2, #788
	ldr	r0, .L442+36
	bl	printk
.L388:
	strd	r8, [r6, #904]
.L387:
	adds	r4, r4, #1
	adds	r6, r6, #88
	cmp	r4, #17
	bne	.L389
	b	.L369
.L380:
	adds	r2, r2, #1
	adds	r3, r3, #88
	cmp	r2, #16
	bne	.L381
	mov	r3, r9
	mov	r2, #492
	mov	r1, fp
	ldr	r0, .L442+40
	bl	printk
	movs	r5, #0
	b	.L378
.L443:
	.align	2
.L442:
	.word	.LANCHOR0+427
	.word	.LC22
	.word	.LC3
	.word	.LANCHOR0+446
	.word	.LC23
	.word	.LC0
	.word	.LC1
	.word	h265e_debug
	.word	.LC30
	.word	.LC31
	.word	.LC7
	.word	.LANCHOR0+465
	.fnend
	.size	h265e_dpb_proc_cpb, .-h265e_dpb_proc_cpb
	.align	1
	.global	h265e_dpb_build_list
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h265e_dpb_build_list, %function
h265e_dpb_build_list:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r5, r0
	ldr	r3, [r0, #876]
	mov	r8, r1
	ldr	r7, .L472
	ldr	r4, [r3, #56]
	ldr	r3, [r7]
	lsls	r2, r3, #31
	ldr	r6, [r4, #16]
	bpl	.L445
	movw	r2, #801
	ldr	r1, .L472+4
	ldr	r0, .L472+8
	bl	printk
.L445:
	mov	r1, r6
	mov	r0, r5
	bl	get_nal_unit_type
	cmp	r0, #19
	bne	.L446
.L448:
	str	r6, [r5, #8]
.L447:
	ldr	r3, [r5, #8]
	mov	r1, r6
	mov	r0, r5
	str	r3, [r4, #24]
	ldr	r3, [r4, #328]
	clz	r3, r3
	lsrs	r3, r3, #5
	strb	r3, [r4, #7]
	bl	get_nal_unit_type
	ldrb	r3, [r4, #7]	@ zero_extendqisi2
	str	r0, [r4, #364]
	cbz	r3, .L449
	cmp	r0, #7
	beq	.L450
	cmp	r0, #9
	beq	.L451
	cmp	r0, #1
	bne	.L449
	movs	r3, #0
.L471:
	str	r3, [r4, #364]
.L449:
	mov	r1, r6
	ldr	r2, [r4, #364]
	mov	r0, r5
	bl	h265e_dpb_dec_refresh_marking
	mov	r1, r6
	mov	r3, r8
	mov	r2, r4
	mov	r0, r5
	bl	h265e_dpb_cpb2rps
	mov	r6, r0
	cmp	r0, #0
	bne	.L457
	ldr	r3, [r4, #520]
	add	r0, r5, #880
	ldr	r1, [r5, #20]
	ldr	r2, [r3, #336]
	cmp	r1, r2
	ite	le
	strle	r1, [r4, #40]
	strgt	r2, [r4, #40]
	mov	r1, r4
	ldr	r3, [r3, #336]
	ldr	r2, [r5, #24]
	cmp	r2, r3
	ite	le
	strle	r2, [r4, #44]
	strgt	r3, [r4, #44]
	bl	h265e_slice_set_ref_list
	ldr	r3, [r4, #368]
	cbnz	r3, .L456
	ldr	r3, [r4, #44]
	cbnz	r3, .L454
	movs	r3, #1
	str	r3, [r4, #368]
.L456:
	mov	r0, r4
	bl	h265e_slice_set_ref_poc_list
	ldr	r2, [r4, #368]
	movs	r3, #0
	str	r3, [r4, #340]
	cmp	r2, #2
	mov	r1, r8
	mov	r0, r5
	it	ne
	ldrne	r3, [r4, #860]
	str	r3, [r4, #352]
	bl	h265e_dpb_free_unsed
	ldr	r3, [r7]
	lsls	r3, r3, #31
	bpl	.L444
	movw	r2, #885
	ldr	r1, .L472+4
	ldr	r0, .L472+12
	bl	printk
.L444:
	mov	r0, r6
	pop	{r4, r5, r6, r7, r8, pc}
.L446:
	mov	r1, r6
	mov	r0, r5
	bl	get_nal_unit_type
	cmp	r0, #20
	bne	.L447
	b	.L448
.L450:
	movs	r3, #6
	b	.L471
.L451:
	movs	r3, #8
	b	.L471
.L457:
	mov	r6, #-1
	b	.L444
.L454:
	strb	r6, [r4, #12]
	b	.L456
.L473:
	.align	2
.L472:
	.word	h265e_debug
	.word	.LANCHOR0+487
	.word	.LC8
	.word	.LC11
	.fnend
	.size	h265e_dpb_build_list, .-h265e_dpb_build_list
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__func__.9719, %object
	.size	__func__.9719, 21
__func__.9719:
	.ascii	"h265e_find_cpb_frame\000"
	.type	__FUNCTION__.9722, %object
	.size	__FUNCTION__.9722, 21
__FUNCTION__.9722:
	.ascii	"h265e_find_cpb_frame\000"
	.type	__func__.9519, %object
	.size	__func__.9519, 23
__func__.9519:
	.ascii	"h265e_dpb_set_ref_list\000"
	.type	__func__.9533, %object
	.size	__func__.9533, 20
__func__.9533:
	.ascii	"h265e_dpb_init_curr\000"
	.type	__FUNCTION__.9534, %object
	.size	__FUNCTION__.9534, 20
__FUNCTION__.9534:
	.ascii	"h265e_dpb_init_curr\000"
	.type	__func__.9541, %object
	.size	__func__.9541, 19
__func__.9541:
	.ascii	"h265e_dpb_get_curr\000"
	.type	__FUNCTION__.9548, %object
	.size	__FUNCTION__.9548, 21
__FUNCTION__.9548:
	.ascii	"h265e_dpb_frm_deinit\000"
	.type	__func__.9549, %object
	.size	__func__.9549, 21
__func__.9549:
	.ascii	"h265e_dpb_frm_deinit\000"
	.type	__func__.9557, %object
	.size	__func__.9557, 15
__func__.9557:
	.ascii	"h265e_dpb_init\000"
	.type	__FUNCTION__.9558, %object
	.size	__FUNCTION__.9558, 15
__FUNCTION__.9558:
	.ascii	"h265e_dpb_init\000"
	.type	__func__.9568, %object
	.size	__func__.9568, 17
__func__.9568:
	.ascii	"h265e_dpb_deinit\000"
	.type	__FUNCTION__.9572, %object
	.size	__FUNCTION__.9572, 17
__FUNCTION__.9572:
	.ascii	"h265e_dpb_deinit\000"
	.type	__func__.9577, %object
	.size	__func__.9577, 18
__func__.9577:
	.ascii	"get_nal_unit_type\000"
	.type	__func__.9614, %object
	.size	__func__.9614, 20
__func__.9614:
	.ascii	"h265e_dpb_apply_rps\000"
	.type	__func__.9631, %object
	.size	__func__.9631, 30
__func__.9631:
	.ascii	"h265e_dpb_dec_refresh_marking\000"
	.type	__func__.9662, %object
	.size	__func__.9662, 25
__func__.9662:
	.ascii	"h265e_dpb_arrange_lt_rps\000"
	.type	__FUNCTION__.9666, %object
	.size	__FUNCTION__.9666, 25
__FUNCTION__.9666:
	.ascii	"h265e_dpb_arrange_lt_rps\000"
	.type	__func__.9767, %object
	.size	__func__.9767, 18
__func__.9767:
	.ascii	"h265e_dpb_cpb2rps\000"
	.type	__FUNCTION__.9771, %object
	.size	__FUNCTION__.9771, 18
__FUNCTION__.9771:
	.ascii	"h265e_dpb_cpb2rps\000"
	.type	__func__.9792, %object
	.size	__func__.9792, 21
__func__.9792:
	.ascii	"h265e_dpb_free_unsed\000"
	.type	__func__.9743, %object
	.size	__func__.9743, 22
__func__.9743:
	.ascii	"h265e_check_frame_cpb\000"
	.type	__FUNCTION__.9817, %object
	.size	__FUNCTION__.9817, 19
__FUNCTION__.9817:
	.ascii	"h265e_dpb_proc_cpb\000"
	.type	__func__.9820, %object
	.size	__func__.9820, 19
__func__.9820:
	.ascii	"h265e_dpb_proc_cpb\000"
	.type	__func__.9706, %object
	.size	__func__.9706, 22
__func__.9706:
	.ascii	"h265e_find_cpb_in_dpb\000"
	.type	__func__.9838, %object
	.size	__func__.9838, 21
__func__.9838:
	.ascii	"h265e_dpb_build_list\000"
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%s:%d: frm %d start finding slot \012\000"
.LC1:
	.ascii	"\0016%s:%d: frm %d match slot %d valid %d\012\000"
.LC2:
	.ascii	"p->is_non_ref == frm->is_non_ref\000"
.LC3:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC4:
	.ascii	"p->is_lt_ref == frm->is_lt_ref\000"
.LC5:
	.ascii	"p->lt_idx == frm->lt_idx\000"
.LC6:
	.ascii	"p->temporal_id == frm->temporal_id\000"
.LC7:
	.ascii	"\0013%s:%d: can not find match frm %d\012\000"
.LC8:
	.ascii	"\0016%s:%d: enter\012\000"
.LC9:
	.ascii	"\0016%s:%d: rps->delta_poc[%d] = %d\000"
.LC10:
	.ascii	"\0016%s:%d: get ref ref_idx %d\000"
.LC11:
	.ascii	"\0016%s:%d: leave\012\000"
.LC12:
	.ascii	"\0013%d: Did not find the right reference picture\000"
.LC13:
	.ascii	"!frm->on_used\000"
.LC14:
	.ascii	"\0016%s:%d: get free dpb slot_index %d\000"
.LC15:
	.ascii	"\0013%s:%d: invalid parameter %p \012\000"
.LC16:
	.ascii	"\0016%s:%d: outPic->slice->poc %d,curPoc %d dealt %"
	.ascii	"d\000"
.LC17:
	.ascii	"\0016%s:%d: free unreference buf poc %d\000"
.LC18:
	.ascii	"ctr == numLongPics\000"
.LC19:
	.ascii	"rps->delta_poc_msb_cycle_lt[i] >= 0\000"
.LC20:
	.ascii	"rps->real_poc[i] != rps->real_poc[j]\000"
.LC21:
	.ascii	"slice->sps->long_term_ref_pics_present_flag\000"
.LC22:
	.ascii	"!frm->is_non_ref\000"
.LC23:
	.ascii	"\0016%s:%d: idx %d frm %d valid %d is_non_ref %d lt"
	.ascii	"_ref %d\012\000"
.LC24:
	.ascii	"\0016%s:%d: found st %d st_size %d %p deat_poc %d\012"
	.ascii	"\000"
.LC25:
	.ascii	"\0016%s:%d: found lt %d lt_size %d %p dealt poc %d\012"
	.ascii	"\000"
.LC26:
	.ascii	"\0013%d: ref frame no found in refer index %d\000"
.LC27:
	.ascii	"\0016%s:%d: numLongTermRefPic %d nShortTerm %d\000"
.LC28:
	.ascii	"\0016%s:%d: free curr unreference buf poc %d\000"
.LC29:
	.ascii	"\0016%s:%d: cpb final unreference buf poc %d\000"
.LC30:
	.ascii	"\0016%s:%d: cpb roll back found\000"
.LC31:
	.ascii	"\0016%s:%d: reset index %d frame->inited %d rame->o"
	.ascii	"n_used %d\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
