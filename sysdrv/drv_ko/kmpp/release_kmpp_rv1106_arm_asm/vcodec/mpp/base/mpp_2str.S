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
	.file	"mpp_2str.c"
	.text
	.align	1
	.global	strof_ctx_type
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_ctx_type, %function
strof_ctx_type:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L2
	ldr	r0, [r3, r0, lsl #2]
	bx	lr
.L3:
	.align	2
.L2:
	.word	.LANCHOR0
	.fnend
	.size	strof_ctx_type, .-strof_ctx_type
	.align	1
	.global	strof_coding_type
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_coding_type, %function
strof_coding_type:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #10
	bhi	.L5
	ldr	r3, .L8
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #12]
	bx	lr
.L5:
	add	r0, r0, #-16777216
	cmp	r0, #6
	ittte	ls
	ldrls	r3, .L8
	addls	r0, r3, r0, lsl #2
	ldrls	r0, [r0, #56]
	movhi	r0, #0
	bx	lr
.L9:
	.align	2
.L8:
	.word	.LANCHOR0
	.fnend
	.size	strof_coding_type, .-strof_coding_type
	.align	1
	.global	strof_pixel_fmt
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_pixel_fmt, %function
strof_pixel_fmt:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #17
	bhi	.L11
	ldr	r3, .L14
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #84]
	bx	lr
.L11:
	sub	r2, r0, #19
	movw	r3, #65533
	cmp	r2, r3
	itttt	ls
	subls	r0, r0, #65536
	ldrls	r3, .L14
	addls	r0, r3, r0, lsl #2
	ldrls	r0, [r0, #152]
	it	hi
	movhi	r0, #0
	bx	lr
.L15:
	.align	2
.L14:
	.word	.LANCHOR0
	.fnend
	.size	strof_pixel_fmt, .-strof_pixel_fmt
	.align	1
	.global	strof_rc_mode
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_rc_mode, %function
strof_rc_mode:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #3
	ittte	ls
	ldrls	r3, .L19
	addls	r3, r3, r0, lsl #2
	ldrls	r0, [r3, #208]
	movhi	r0, #0
	bx	lr
.L20:
	.align	2
.L19:
	.word	.LANCHOR0
	.fnend
	.size	strof_rc_mode, .-strof_rc_mode
	.align	1
	.global	strof_gop_mode
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_gop_mode, %function
strof_gop_mode:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #1
	ittte	ls
	ldrls	r3, .L24
	addls	r3, r3, r0, lsl #2
	ldrls	r0, [r3, #224]
	movhi	r0, #0
	bx	lr
.L25:
	.align	2
.L24:
	.word	.LANCHOR0
	.fnend
	.size	strof_gop_mode, .-strof_gop_mode
	.align	1
	.global	strof_rotation
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_rotation, %function
strof_rotation:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #4
	ittte	ls
	ldrls	r3, .L29
	addls	r3, r3, r0, lsl #2
	ldrls	r0, [r3, #232]
	movhi	r0, #0
	bx	lr
.L30:
	.align	2
.L29:
	.word	.LANCHOR0
	.fnend
	.size	strof_rotation, .-strof_rotation
	.align	1
	.global	strof_profle
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_profle, %function
strof_profle:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	cmp	r0, #9
	bhi	.L32
	cmp	r0, #8
	bcs	.L36
	cmp	r0, #7
	beq	.L34
.L42:
	movs	r0, #0
	bx	lr
.L32:
	ldr	r3, .L43
	cmp	r0, r3
	bne	.L42
	ldr	r3, .L43+4
	ldr	r0, .L43+8
	cmp	r1, #1
	it	hi
	movhi	r0, r3
	bx	lr
.L34:
	cmp	r1, #66
	beq	.L37
	cmp	r1, #77
	beq	.L38
	cmp	r1, #100
	beq	.L39
	ldr	r3, .L43+4
	ldr	r0, .L43+12
	cmp	r1, #110
	it	ne
	movne	r0, r3
	bx	lr
.L36:
	ldr	r0, .L43+16
	bx	lr
.L37:
	ldr	r0, .L43+20
	bx	lr
.L38:
	ldr	r0, .L43+8
	bx	lr
.L39:
	ldr	r0, .L43+24
	bx	lr
.L44:
	.align	2
.L43:
	.word	16777220
	.word	.LC3
	.word	.LC2
	.word	.LC1
	.word	.LC0
	.word	.LC5
	.word	.LC4
	.fnend
	.size	strof_profle, .-strof_profle
	.align	1
	.global	strof_bool
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_bool, %function
strof_bool:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, .L48
	ldr	r3, .L48+4
	cmp	r0, #0
	ite	ne
	movne	r0, r2
	moveq	r0, r3
	bx	lr
.L49:
	.align	2
.L48:
	.word	.LC6
	.word	.LC7
	.fnend
	.size	strof_bool, .-strof_bool
	.align	1
	.global	strof_drop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_drop, %function
strof_drop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L51
	add	r0, r3, r0, lsl #2
	ldr	r0, [r0, #248]
	bx	lr
.L52:
	.align	2
.L51:
	.word	.LANCHOR0
	.fnend
	.size	strof_drop, .-strof_drop
	.align	1
	.global	strof_suprmode
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	strof_suprmode, %function
strof_suprmode:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L54
	bx	lr
.L55:
	.align	2
.L54:
	.word	.LC8
	.fnend
	.size	strof_suprmode, .-strof_suprmode
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	ctx_type_str.5715, %object
	.size	ctx_type_str.5715, 12
ctx_type_str.5715:
	.word	.LC69
	.word	.LC70
	.word	.LC71
	.type	coding_type_str0.5719, %object
	.size	coding_type_str0.5719, 44
coding_type_str0.5719:
	.word	.LC58
	.word	.LC59
	.word	.LC60
	.word	.LC61
	.word	.LC62
	.word	.LC63
	.word	.LC64
	.word	.LC65
	.word	.LC66
	.word	.LC67
	.word	.LC68
	.type	coding_type_str1.5720, %object
	.size	coding_type_str1.5720, 28
coding_type_str1.5720:
	.word	.LC51
	.word	.LC52
	.word	.LC53
	.word	.LC54
	.word	.LC55
	.word	.LC56
	.word	.LC57
	.type	yuv_fmt_str.5724, %object
	.size	yuv_fmt_str.5724, 68
yuv_fmt_str.5724:
	.word	.LC35
	.word	.LC36
	.word	.LC37
	.word	.LC38
	.word	.LC39
	.word	.LC40
	.word	.LC41
	.word	.LC42
	.word	.LC43
	.word	.LC44
	.word	.LC45
	.word	.LC46
	.word	.LC47
	.word	.LC41
	.word	.LC48
	.word	.LC49
	.word	.LC50
	.type	rgb_fmt_str.5725, %object
	.size	rgb_fmt_str.5725, 56
rgb_fmt_str.5725:
	.word	.LC21
	.word	.LC22
	.word	.LC23
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.word	.LC27
	.word	.LC28
	.word	.LC29
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.word	.LC33
	.word	.LC34
	.type	rc_mode_str.5729, %object
	.size	rc_mode_str.5729, 16
rc_mode_str.5729:
	.word	.LC17
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.type	gop_mode_str.5733, %object
	.size	gop_mode_str.5733, 8
gop_mode_str.5733:
	.word	.LC15
	.word	.LC16
	.type	rotation_str.5737, %object
	.size	rotation_str.5737, 16
rotation_str.5737:
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.type	mode_str.5760, %object
	.size	mode_str.5760, 12
mode_str.5760:
	.word	.LC7
	.word	.LC9
	.word	.LC10
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"base\000"
.LC1:
	.ascii	"high10\000"
.LC2:
	.ascii	"main\000"
.LC3:
	.ascii	"unknown\000"
.LC4:
	.ascii	"high\000"
.LC5:
	.ascii	"baseline\000"
.LC6:
	.ascii	"Y\000"
.LC7:
	.ascii	"N\000"
.LC8:
	.ascii	"bit\000"
.LC9:
	.ascii	"normal\000"
.LC10:
	.ascii	"pskip\000"
.LC11:
	.ascii	"0\000"
.LC12:
	.ascii	"90\000"
.LC13:
	.ascii	"180\000"
.LC14:
	.ascii	"270\000"
.LC15:
	.ascii	"normalp\000"
.LC16:
	.ascii	"smartp\000"
.LC17:
	.ascii	"vbr\000"
.LC18:
	.ascii	"cbr\000"
.LC19:
	.ascii	"fixqp\000"
.LC20:
	.ascii	"avbr\000"
.LC21:
	.ascii	"RGB565\000"
.LC22:
	.ascii	"BGR565\000"
.LC23:
	.ascii	"RGB555\000"
.LC24:
	.ascii	"BGR555\000"
.LC25:
	.ascii	"RGB444\000"
.LC26:
	.ascii	"BGR444\000"
.LC27:
	.ascii	"RGB888\000"
.LC28:
	.ascii	"BGR888\000"
.LC29:
	.ascii	"RGB101010\000"
.LC30:
	.ascii	"BGR101010\000"
.LC31:
	.ascii	"ARGB8888\000"
.LC32:
	.ascii	"ABGR8888\000"
.LC33:
	.ascii	"BGRA8888\000"
.LC34:
	.ascii	"RGBA8888\000"
.LC35:
	.ascii	"YUV420SP\000"
.LC36:
	.ascii	"YUV420SP10\000"
.LC37:
	.ascii	"YUV422SP\000"
.LC38:
	.ascii	"YUV422SP10\000"
.LC39:
	.ascii	"YUV420P\000"
.LC40:
	.ascii	"YUV420SPVU\000"
.LC41:
	.ascii	"YUV422P\000"
.LC42:
	.ascii	"YUV422SP_VU\000"
.LC43:
	.ascii	"422_YUYV\000"
.LC44:
	.ascii	"422_YVYU\000"
.LC45:
	.ascii	"422_UYVY\000"
.LC46:
	.ascii	"422_VYUY\000"
.LC47:
	.ascii	"YUV400\000"
.LC48:
	.ascii	"YUV440SP\000"
.LC49:
	.ascii	"YUV411SP\000"
.LC50:
	.ascii	"YUV444SP\000"
.LC51:
	.ascii	"vc1\000"
.LC52:
	.ascii	"flv1\000"
.LC53:
	.ascii	"divx3\000"
.LC54:
	.ascii	"vp6\000"
.LC55:
	.ascii	"h265\000"
.LC56:
	.ascii	"avs+\000"
.LC57:
	.ascii	"avs\000"
.LC58:
	.ascii	"unused\000"
.LC59:
	.ascii	"autodetect\000"
.LC60:
	.ascii	"mpeg2\000"
.LC61:
	.ascii	"h263\000"
.LC62:
	.ascii	"mpeg4\000"
.LC63:
	.ascii	"wmv\000"
.LC64:
	.ascii	"rv\000"
.LC65:
	.ascii	"h264\000"
.LC66:
	.ascii	"mjpeg\000"
.LC67:
	.ascii	"vp8\000"
.LC68:
	.ascii	"vp9\000"
.LC69:
	.ascii	"dec\000"
.LC70:
	.ascii	"enc\000"
.LC71:
	.ascii	"isp\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
