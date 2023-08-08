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
	.file	"h264e_sps.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	h264e_sps_update
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_sps_update, %function
h264e_sps_update:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	movw	r3, #4700
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r4, r0
	mov	r10, r0
	.pad #20
	sub	sp, sp, #20
	ldr	r0, [r1, r3]
	mov	r5, r1
	bl	mpp_enc_ref_cfg_get_cpb_info
	ldrd	r2, r3, [r5, #12]
	ldr	r9, [r5, #152]
	add	r7, r2, #15
	add	r6, r3, #15
	bic	r7, r7, #15
	bic	r6, r6, #15
	sub	r8, r7, r2
	subs	r3, r6, r3
	mul	r2, r6, r7
	asr	ip, r2, #8
	ldr	r2, .L25
	mov	lr, r2
	mov	fp, r0
	str	r3, [sp, #12]
	ldr	r0, [r5, #600]
	ldr	r3, [r5, #596]
	str	r0, [sp, #8]
	str	r3, [r10], #96
	movs	r3, #1
	strd	r3, r3, [r4, #4]
	movs	r3, #0
	strd	r3, r3, [r4, #12]
	strd	r3, r3, [r4, #20]
.L4:
	ldr	r1, [r2, #8]
	cmp	r1, ip
	blt	.L2
	movs	r2, #24
	muls	r3, r2, r3
	add	r2, lr, r3
	ldr	r3, [lr, r3]
	cmp	r0, r3
	bge	.L3
	cmp	r3, #99
	beq	.L3
	ldr	r2, [r2, #20]
	movs	r1, #95
	ldr	r0, .L25+4
	str	r3, [sp, #8]
	bl	printk
.L3:
	ldr	r3, [sp, #8]
	movs	r2, #0
	cmp	r9, #0
	str	r3, [r4, #28]
	mov	r3, #1
	strd	r2, r3, [r4, #32]
	ldr	r3, [r5, #580]
	str	r3, [r4, #44]
	ldr	r3, [r5, #584]
	str	r3, [r4, #48]
	ldr	r3, [r5, #588]
	str	r3, [r4, #40]
	bge	.L5
	movs	r1, #112
	ldr	r3, .L25+8
	str	r1, [sp]
	ldr	r2, .L25+12
	ldr	r0, .L25+16
	bl	printk
.L6:
	mov	r0, r9
	bl	mpp_log2
	cmp	r0, #16
	bgt	.L17
	mov	r0, r9
	bl	mpp_log2
.L8:
	cmp	r0, #4
	mov	r3, r0
	ldr	r2, [r4, #40]
	it	lt
	movlt	r3, #4
	cmp	r0, #3
	sub	r3, r3, #4
	it	lt
	movlt	r0, #3
	subs	r0, r0, #3
	cmp	r2, r3
	it	lt
	strlt	r3, [r4, #40]
	cmp	r0, #12
	ldr	r3, [r4, #48]
	it	ge
	movge	r0, #12
	cmp	r0, r3
	it	gt
	strgt	r0, [r4, #48]
	b	.L7
.L2:
	adds	r3, r3, #1
	adds	r2, r2, #24
	cmp	r3, #20
	bne	.L4
	b	.L3
.L5:
	cmp	r9, #1
	bhi	.L6
	movs	r3, #12
	str	r3, [r4, #40]
	str	r3, [r4, #48]
.L7:
	ldr	r3, [fp]
	asrs	r7, r7, #4
	asrs	r6, r6, #4
	str	r3, [r4, #52]
	ldr	r3, [r5, #592]
	str	r7, [r4, #60]
	clz	r3, r3
	str	r6, [r4, #64]
	lsrs	r3, r3, #5
	str	r3, [r4, #56]
	movs	r3, #1
	str	r3, [r4, #68]
	ldr	r2, [r5, #612]
	str	r2, [r4, #72]
	ldr	r2, [sp, #12]
	orrs	r1, r8, r2
	beq	.L10
	str	r3, [r4, #76]
	movs	r3, #0
	str	r2, [r4, #92]
	strd	r3, r8, [r4, #80]
	str	r3, [r4, #88]
.L11:
	movs	r2, #176
	movs	r1, #0
	mov	r0, r10
	bl	memset
	movs	r2, #1
	str	r2, [r4, #96]
	str	r2, [r4, #164]
	ldr	r3, [r5, #144]
	lsls	r3, r3, r2
	str	r3, [r4, #172]
	ldr	r3, [r5, #148]
	str	r3, [r4, #168]
	ldr	r3, [r5, #140]
	clz	r3, r3
	lsrs	r3, r3, #5
	str	r3, [r4, #176]
	movs	r3, #5
	str	r3, [r4, #128]
	ldr	r3, [r5, #52]
	cmp	r3, #2
	itt	eq
	streq	r2, [r4, #124]
	streq	r2, [r4, #132]
	ldr	r3, [r5, #44]
	cmp	r3, #22
	bhi	.L13
	cmp	r3, #2
	bne	.L14
.L13:
	ldr	r3, [r5, #48]
	cmp	r3, #18
	bhi	.L15
	cmp	r3, #2
	bne	.L14
.L15:
	ldr	r3, [r5, #40]
	cmp	r3, #14
	bhi	.L16
	cmp	r3, #2
	beq	.L16
.L14:
	movs	r3, #1
	str	r3, [r4, #124]
	str	r3, [r4, #136]
	ldr	r3, [r5, #44]
	str	r3, [r4, #140]
	ldr	r3, [r5, #48]
	str	r3, [r4, #144]
	ldr	r3, [r5, #40]
	str	r3, [r4, #148]
.L16:
	movs	r3, #1
	strd	r3, r3, [r4, #240]
	movs	r3, #16
	strd	r3, r3, [r4, #256]
	ldr	r3, [fp]
	movs	r0, #0
	str	r3, [r4, #268]
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L17:
	movs	r0, #16
	b	.L8
.L10:
	str	r1, [r4, #76]
	movs	r2, #16
	add	r0, r4, #80
	bl	memset
	b	.L11
.L26:
	.align	2
.L25:
	.word	.LANCHOR0
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC1
	.word	.LC2
	.fnend
	.size	h264e_sps_update, .-h264e_sps_update
	.align	1
	.global	h264e_sps_to_packet
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_sps_to_packet, %function
h264e_sps_to_packet:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	mov	r4, r0
	.pad #40
	sub	sp, sp, #40
	mov	r0, r1
	mov	r5, r1
	mov	r7, r2
	bl	mpp_packet_get_pos
	mov	r8, r0
	mov	r0, r5
	bl	mpp_packet_get_data
	mov	r9, r0
	mov	r0, r5
	bl	mpp_packet_get_size
	mov	r10, r0
	mov	r0, r5
	bl	mpp_packet_get_length
	add	r2, r9, r10
	add	r1, r8, r0
	mov	r6, r0
	subs	r2, r2, r1
	add	r0, sp, #4
	bl	mpp_writer_init
	movs	r2, #24
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	movs	r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #2
	movs	r1, #3
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #5
	movs	r1, #7
	add	r0, sp, #4
	bl	mpp_writer_put_raw_bits
	movs	r2, #8
	ldr	r1, [r4]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #4]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #8]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #12]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #16]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #20]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #24]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #2
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #8
	ldr	r1, [r4, #28]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r1, [r4, #32]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r3, [r4]
	cmp	r3, #99
	ble	.L28
	ldr	r1, [r4, #36]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	movs	r1, #0
	add	r0, sp, #4
	bl	mpp_writer_put_bits
.L28:
	ldr	r1, [r4, #40]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #44]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r3, [r4, #44]
	cbnz	r3, .L29
	ldr	r1, [r4, #48]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
.L29:
	ldr	r1, [r4, #52]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	movs	r2, #1
	ldr	r1, [r4, #56]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r1, [r4, #60]
	subs	r1, r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #64]
	subs	r1, r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	movs	r2, #1
	ldr	r1, [r4, #68]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #72]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #76]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #76]
	cbz	r3, .L30
	ldr	r1, [r4, #80]
	add	r0, sp, #4
	add	r1, r1, r1, lsr #31
	asrs	r1, r1, #1
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #84]
	add	r1, r1, r1, lsr #31
	asrs	r1, r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #88]
	add	r1, r1, r1, lsr #31
	asrs	r1, r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #92]
	add	r1, r1, r1, lsr #31
	asrs	r1, r1, #1
	add	r0, sp, #4
	bl	mpp_writer_put_ue
.L30:
	movs	r2, #1
	ldr	r1, [r4, #96]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #96]
	cmp	r3, #0
	beq	.L32
	movs	r2, #1
	ldr	r1, [r4, #100]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #100]
	cbz	r3, .L34
	movs	r2, #8
	ldr	r1, [r4, #104]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #104]
	cmp	r3, #255
	bne	.L34
	movs	r2, #16
	ldr	r1, [r4, #108]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #16
	ldr	r1, [r4, #112]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
.L34:
	movs	r2, #1
	ldr	r1, [r4, #116]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #116]
	cbz	r3, .L36
	movs	r2, #1
	ldr	r1, [r4, #120]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
.L36:
	movs	r2, #1
	ldr	r1, [r4, #124]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #124]
	cbz	r3, .L38
	movs	r2, #3
	ldr	r1, [r4, #128]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #132]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #136]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #136]
	cbz	r3, .L38
	movs	r2, #8
	ldr	r1, [r4, #140]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #8
	ldr	r1, [r4, #144]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #8
	ldr	r1, [r4, #148]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
.L38:
	movs	r2, #1
	ldr	r1, [r4, #152]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #152]
	cbz	r3, .L40
	ldr	r1, [r4, #156]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #160]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
.L40:
	movs	r2, #1
	ldr	r1, [r4, #164]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #164]
	cbz	r3, .L41
	movs	r2, #16
	ldrh	r1, [r4, #170]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #16
	ldrh	r1, [r4, #168]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #16
	ldrh	r1, [r4, #174]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #16
	ldrh	r1, [r4, #172]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #176]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
.L41:
	movs	r2, #1
	ldr	r1, [r4, #180]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #184]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #236]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	movs	r2, #1
	ldr	r1, [r4, #240]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r3, [r4, #240]
	cbz	r3, .L32
	movs	r2, #1
	ldr	r1, [r4, #244]
	add	r0, sp, #4
	bl	mpp_writer_put_bits
	ldr	r1, [r4, #248]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #252]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #256]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #260]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #264]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
	ldr	r1, [r4, #268]
	add	r0, sp, #4
	bl	mpp_writer_put_ue
.L32:
	add	r0, sp, #4
	bl	mpp_writer_trailing
	add	r0, sp, #4
	bl	mpp_writer_bytes
	cbz	r7, .L43
	str	r0, [r7]
.L43:
	adds	r1, r0, r6
	mov	r0, r5
	bl	mpp_packet_set_length
	movs	r0, #0
	add	sp, sp, #40
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
	.fnend
	.size	h264e_sps_to_packet, .-h264e_sps_to_packet
	.align	1
	.global	h264e_sps_dump
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	h264e_sps_dump, %function
h264e_sps_dump:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	h264e_sps_dump, .-h264e_sps_dump
	.global	level_infos
	.section	.rodata
	.set	.LANCHOR1,. + 0
	.type	__FUNCTION__.9249, %object
	.size	__FUNCTION__.9249, 17
__FUNCTION__.9249:
	.ascii	"h264e_sps_update\000"
	.data
	.align	2
	.set	.LANCHOR0,. + 0
	.type	level_infos, %object
	.size	level_infos, 480
level_infos:
	.word	10
	.word	1485
	.word	99
	.word	396
	.word	64
	.word	.LC3
	.word	99
	.word	1485
	.word	99
	.word	396
	.word	128
	.word	.LC4
	.word	11
	.word	3000
	.word	396
	.word	900
	.word	192
	.word	.LC5
	.word	12
	.word	6000
	.word	396
	.word	2376
	.word	384
	.word	.LC6
	.word	13
	.word	11880
	.word	396
	.word	2376
	.word	768
	.word	.LC7
	.word	20
	.word	11880
	.word	396
	.word	2376
	.word	2000
	.word	.LC8
	.word	21
	.word	19800
	.word	792
	.word	4752
	.word	4000
	.word	.LC9
	.word	22
	.word	20250
	.word	1620
	.word	8100
	.word	4000
	.word	.LC10
	.word	30
	.word	40500
	.word	1620
	.word	8100
	.word	10000
	.word	.LC11
	.word	31
	.word	108000
	.word	3600
	.word	18000
	.word	14000
	.word	.LC12
	.word	32
	.word	216000
	.word	5120
	.word	20480
	.word	20000
	.word	.LC13
	.word	40
	.word	245760
	.word	8192
	.word	32768
	.word	20000
	.word	.LC14
	.word	41
	.word	245760
	.word	8192
	.word	32768
	.word	50000
	.word	.LC15
	.word	42
	.word	522240
	.word	8704
	.word	34816
	.word	50000
	.word	.LC16
	.word	50
	.word	589824
	.word	22080
	.word	110400
	.word	135000
	.word	.LC17
	.word	51
	.word	983040
	.word	36864
	.word	184320
	.word	240000
	.word	.LC18
	.word	52
	.word	2073600
	.word	36864
	.word	184320
	.word	240000
	.word	.LC19
	.word	60
	.word	4177920
	.word	139264
	.word	696320
	.word	240000
	.word	.LC20
	.word	61
	.word	8355840
	.word	139264
	.word	696320
	.word	480000
	.word	.LC21
	.word	62
	.word	16711680
	.word	139264
	.word	696320
	.word	800000
	.word	.LC22
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0016%d: set level to %s\012\000"
.LC1:
	.ascii	"gop >= 0\000"
.LC2:
	.ascii	"\0013%d: Assertion %s failed at %s:%d\012\000"
.LC3:
	.ascii	"1\000"
.LC4:
	.ascii	"1.b\000"
.LC5:
	.ascii	"1.1\000"
.LC6:
	.ascii	"1.2\000"
.LC7:
	.ascii	"1.3\000"
.LC8:
	.ascii	"2\000"
.LC9:
	.ascii	"2.1\000"
.LC10:
	.ascii	"2.2\000"
.LC11:
	.ascii	"3\000"
.LC12:
	.ascii	"3.1\000"
.LC13:
	.ascii	"3.2\000"
.LC14:
	.ascii	"4\000"
.LC15:
	.ascii	"4.1\000"
.LC16:
	.ascii	"4.2\000"
.LC17:
	.ascii	"5\000"
.LC18:
	.ascii	"5.1\000"
.LC19:
	.ascii	"5.2\000"
.LC20:
	.ascii	"6\000"
.LC21:
	.ascii	"6.1\000"
.LC22:
	.ascii	"6.2\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
