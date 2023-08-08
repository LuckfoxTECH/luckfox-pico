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
	.file	"vvi.c"
	.text
	.syntax unified
	.syntax unified
	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_rockit_get_tb_stream_info:					
	.asciz 	"rockit_get_tb_stream_info"					
__kstrtabns_rockit_get_tb_stream_info:					
	.asciz 	""					
	.previous						

	.section "__ksymtab_strings","aMS",%progbits,1	
__kstrtab_rockit_free_tb_stream_buf:					
	.asciz 	"rockit_free_tb_stream_buf"					
__kstrtabns_rockit_free_tb_stream_buf:					
	.asciz 	""					
	.previous						

	.thumb
	.syntax unified
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vi_compat_ioctl, %function
vi_compat_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	vi_compat_ioctl, .-vi_compat_ioctl
	.align	1
	.global	vvi_get_version
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_get_version, %function
vvi_get_version:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r4, r1
	ldr	r6, [r0, #128]
	movs	r2, #124
	movs	r1, #0
	mov	r0, r4
	ldr	r5, [r6, #160]
	bl	memset
	ldr	r3, [r5, #212]
	str	r3, [r4]
	ldr	r3, [r6]
	str	r3, [r4, #4]
	movs	r3, #0
	str	r3, [r4, #8]
	ldr	r0, [r5]
	bl	strlen
	movs	r3, #20
	str	r3, [r4, #52]
	movs	r3, #17
	str	r3, [r4, #88]
	cmp	r0, #31
	str	r0, [r4, #12]
	add	r0, r4, #16
	itt	hi
	movhi	r3, #31
	strhi	r3, [r4, #12]
	ldr	r2, [r4, #12]
	ldr	r1, [r5]
	bl	strncpy
	ldr	r2, [r4, #52]
	ldr	r1, .L4
	add	r0, r4, #56
	bl	strncpy
	ldr	r2, [r4, #88]
	ldr	r1, .L4+4
	add	r0, r4, #92
	bl	strncpy
	movs	r0, #0
	pop	{r4, r5, r6, pc}
.L5:
	.align	2
.L4:
	.word	.LC0
	.word	.LC1
	.fnend
	.size	vvi_get_version, .-vvi_get_version
	.global	__aeabi_uidiv
	.global	__aeabi_uidivmod
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	get_new_wrap_line, %function
get_new_wrap_line:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	mov	r5, r1
	mov	r7, r0
	mov	r1, r0
	.pad #24
	sub	sp, sp, #24
	mov	r8, r2
	mul	r0, r3, r2
	mov	r6, r3
	bl	__aeabi_uidiv
	movs	r3, #195
	ldr	r2, .L13
	movs	r1, #4
	mov	r4, r0
	str	r0, [sp, #4]
	str	r3, [sp]
	movs	r0, #8
	strd	r5, r8, [sp, #16]
	strd	r6, r7, [sp, #8]
	ldr	r3, .L13+4
	bl	vlog
	cmp	r4, r5
	bcs	.L9
.L8:
	mov	r1, r4
	mov	r0, r5
	bl	__aeabi_uidivmod
	and	r3, r4, #1
	orrs	r3, r1, r3
	beq	.L6
	subs	r4, r4, #1
	b	.L8
.L9:
	mov	r4, r5
.L6:
	mov	r0, r4
	add	sp, sp, #24
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L14:
	.align	2
.L13:
	.word	.LC2
	.word	.LANCHOR0
	.fnend
	.size	get_new_wrap_line, .-get_new_wrap_line
	.align	1
	.global	vvi_node_set_dev_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_set_dev_status, %function
vvi_node_set_dev_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	ldr	r3, [r1, #168]
	ldr	r4, [r1]
	cmp	r3, #184
	ldr	r2, [r0, #128]
	beq	.L16
	movw	r3, #514
	ldr	r2, .L25
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L25+4
	movs	r0, #8
	bl	vlog
.L23:
	mov	r4, #-1
	b	.L15
.L16:
	adds	r0, r4, #1
	bne	.L18
	ldr	r5, [r1, #4]
	cmp	r5, #2
	bls	.L19
	movs	r3, #3
	strd	r5, r3, [sp, #4]
	mov	r3, #520
.L24:
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L25+4
	movs	r0, #8
	ldr	r2, .L25+8
	bl	vlog
.L15:
	mov	r0, r4
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L19:
	ldr	r0, [r1, #8]
	cmp	r0, #5
	bls	.L20
	movs	r3, #6
	strd	r5, r3, [sp, #4]
	movw	r3, #525
	b	.L24
.L20:
	mov	r4, #1104
	ldr	r6, [r1, #172]
	muls	r4, r5, r4
	mla	r4, r3, r0, r4
	add	r4, r4, r2
	str	r6, [r4, #920]
	ldr	r3, [r1, #164]
	str	r3, [r4, #912]
.L22:
	movs	r4, #0
	b	.L15
.L18:
	cmp	r4, #2
	bls	.L21
	movs	r3, #3
	ldr	r2, .L25+12
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	mov	r3, #532
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L25+4
	bl	vlog
	b	.L23
.L21:
	mla	r4, r3, r4, r2
	ldr	r1, [r1, #172]
	str	r1, [r4, #368]
	b	.L22
.L26:
	.align	2
.L25:
	.word	.LC3
	.word	.LANCHOR0+18
	.word	.LC4
	.word	.LC5
	.fnend
	.size	vvi_node_set_dev_status, .-vvi_node_set_dev_status
	.align	1
	.global	vvi_node_get_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_get_status, %function
vvi_node_get_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r7, r1
	ldr	r3, [r1, #16]
	.pad #20
	sub	sp, sp, #20
	cmp	r3, #136
	beq	.L28
	mov	r3, #290
	ldr	r2, .L34
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L34+4
	movs	r0, #8
	bl	vlog
.L33:
	mov	r0, #-1
	b	.L27
.L28:
	movw	r3, #293
	ldr	r2, .L34+8
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L34+4
	movs	r0, #8
	bl	vlog
	ldrd	r1, r2, [r7]
	movs	r0, #8
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L33
	ldr	r4, [r0, #60]
	movs	r1, #5
	ldr	r2, .L34+12
	movs	r0, #8
	ldr	r8, .L34+4
	movs	r6, #0
	ldr	r3, [r4]
	ldr	r9, .L34+20
	str	r3, [sp, #4]
	mov	r3, #298
	str	r3, [sp]
	ldr	r3, .L34+4
	bl	vlog
	ldr	r3, [r4, #76]!
	ldr	r5, [r3], #-36
	subs	r5, r5, #36
.L30:
	add	r2, r3, #36
	cmp	r2, r4
	bne	.L32
	mov	r3, #308
	ldr	r2, .L34+16
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L34+4
	movs	r0, #8
	bl	vlog
	movs	r0, #0
.L27:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L32:
	ldr	r2, [r3, #16]
	cmp	r6, #7
	it	le
	addle	r1, r7, r6, lsl #2
	mov	r0, #8
	itt	le
	addle	r6, r6, #1
	strle	r2, [r1, #20]
	movs	r1, #5
	strd	r3, r2, [sp, #4]
	movw	r3, #305
	mov	r2, r9
	str	r3, [sp]
	mov	r3, r8
	bl	vlog
	ldr	r2, [r5, #36]
	mov	r3, r5
	sub	r5, r2, #36
	b	.L30
.L35:
	.align	2
.L34:
	.word	.LC6
	.word	.LANCHOR0+42
	.word	.LC7
	.word	.LC8
	.word	.LC10
	.word	.LC9
	.fnend
	.size	vvi_node_get_status, .-vvi_node_get_status
	.align	1
	.global	vvi_node_create
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_create, %function
vvi_node_create:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	.save {r4, r5, r6, r7, lr}
	mov	r4, r1
	ldr	r3, [r0, #128]
	.pad #20
	sub	sp, sp, #20
	ldr	r6, [r3, #160]
	ldr	r3, [r1, #24]
	cmp	r3, #164
	beq	.L37
	movs	r3, #88
	ldr	r2, .L61
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L61+4
	movs	r0, #8
	bl	vlog
.L58:
	mov	r0, #-1
	b	.L36
.L37:
	ldr	r0, .L61+8
	bl	mpi_allocator_find_by_name
	mov	r7, r0
	cbnz	r0, .L39
	movs	r3, #93
	ldr	r2, .L61+12
	str	r3, [sp]
	ldr	r3, .L61+4
	str	r0, [sp, #4]
.L60:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
	b	.L58
.L39:
	ldr	r5, [r4, #8]
	movs	r0, #8
	ldr	r1, [r4]
	mov	r2, r5
	bl	vsys_get_node_from_map
	mov	r3, r0
	cbz	r0, .L40
	movs	r3, #101
	str	r5, [sp, #4]
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L61+4
	movs	r0, #8
	ldr	r2, .L61+16
	bl	vlog
	mvn	r0, #1
.L36:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, pc}
.L40:
	ldr	r2, [r4]
	mov	r1, r5
	add	r6, r6, r2, lsl #2
	mov	r2, r0
	ldr	r0, [r6, #196]
	bl	mpi_dev_create_node_with_params
	mov	r6, r0
	str	r5, [sp, #4]
	cbnz	r0, .L41
	movs	r3, #106
	ldr	r2, .L61+20
	str	r3, [sp]
	ldr	r3, .L61+4
	b	.L60
.L41:
	movs	r3, #109
	movs	r1, #4
	str	r3, [sp]
	movs	r0, #8
	ldr	r3, .L61+4
	ldr	r2, .L61+24
	bl	vlog
	ldr	r1, .L61+28
	str	r1, [r6, #4]
	ldr	r3, [r4, #40]
	cmp	r3, #1
	bne	.L42
	ldr	r2, [r4, #36]
	cbnz	r2, .L43
	movs	r3, #4
	str	r3, [r4, #32]
.L43:
	ldr	r3, [r4, #32]
	mov	r0, r7
	cmp	r3, #1
	itett	eq
	streq	r1, [sp]
	strne	r1, [sp]
	moveq	r3, #4
	ldreq	r1, .L61+32
	ite	eq
	streq	r3, [r4, #32]
	movne	r1, #1
	bl	mpi_buf_pool_create
	ldr	r3, [r4, #32]
	ldr	r2, .L61+36
	movs	r1, #4
	str	r3, [sp, #8]
	mov	r5, r0
	ldr	r3, [r4, #8]
	movs	r0, #8
	str	r3, [sp, #4]
	movs	r3, #122
	str	r3, [sp]
	ldr	r3, .L61+4
	bl	vlog
.L59:
	cbnz	r5, .L57
.L49:
	ldr	r3, [r4, #8]
	movs	r1, #2
	ldr	r2, .L61+40
	movs	r0, #8
	str	r3, [sp, #4]
	movs	r3, #134
	str	r3, [sp]
	ldr	r3, .L61+4
	bl	vlog
	mov	r0, r6
	bl	mpi_node_destroy
	b	.L58
.L42:
	cbnz	r3, .L48
	ldr	r0, [r4, #44]
	bl	vsys_get_pool
	ldr	r3, [r4, #44]
	ldr	r2, .L61+44
	movs	r1, #4
	mov	r5, r0
	str	r3, [sp, #4]
	movs	r0, #8
	movs	r3, #128
	str	r3, [sp]
	ldr	r3, .L61+4
	bl	vlog
	b	.L59
.L57:
	mov	r1, r6
	mov	r0, r5
	bl	mpi_buf_pool_add_user
.L47:
	mov	r3, r6
	ldr	r2, [r4, #8]
	ldr	r1, [r4]
	movs	r0, #8
	bl	vsys_add_node_to_map
	mov	r1, r6
	mov	r0, r5
	bl	mpi_buf_pool_add_user
	movs	r3, #143
	ldr	r2, .L61+48
	movs	r1, #4
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L61+4
	bl	vlog
	movs	r0, #0
	b	.L36
.L48:
	movs	r5, #0
	cmp	r5, #0
	bne	.L47
	b	.L49
.L62:
	.align	2
.L61:
	.word	.LC11
	.word	.LANCHOR0+62
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.word	.LC16
	.word	.LC17
	.word	-2147483647
	.word	.LC18
	.word	.LC19
	.word	.LC20
	.word	.LC21
	.fnend
	.size	vvi_node_create, .-vvi_node_create
	.align	1
	.global	vvi_node_delete
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_delete, %function
vvi_node_delete:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #16
	mov	r6, r1
	ldr	r3, [r1, #24]
	ldr	r5, [r0, #128]
	cmp	r3, #164
	ldr	r8, [r1]
	sub	r7, r5, #32
	ldr	r4, [r1, #8]
	beq	.L64
	movs	r3, #162
	ldr	r2, .L78
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L78+4
	movs	r0, #8
	bl	vlog
	mov	r0, #-1
.L63:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L64:
	mov	r2, r4
	mov	r1, r8
	movs	r0, #8
	bl	vsys_get_node_from_map
	mov	r9, r0
	cbz	r0, .L66
	ldr	r10, [r0, #60]
	movs	r0, #8
	ldr	r2, [r6, #8]
	ldr	r1, [r6]
	bl	vsys_remove_node_from_map
	mov	r0, r9
	bl	mpi_node_destroy
	cmp	r10, #0
	beq	.L67
	movs	r1, #1
	mov	r0, r10
	bl	mpi_buf_pool_destroy
.L67:
	movs	r3, #173
	strd	r8, r4, [sp, #4]
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L78+4
	movs	r0, #8
	ldr	r2, .L78+8
	bl	vlog
.L66:
	cbz	r7, .L68
	mov	r0, #1104
	movs	r2, #184
	mul	r0, r0, r8
	movs	r1, #0
	mla	r4, r2, r4, r0
	add	r5, r5, r4
	add	r4, r4, #780
	adds	r0, r7, r4
	ldr	r6, [r5, #920]
	bl	memset
	str	r6, [r5, #920]
.L68:
	movs	r3, #183
	ldr	r2, .L78+12
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L78+4
	movs	r0, #8
	bl	vlog
	movs	r0, #0
	b	.L63
.L79:
	.align	2
.L78:
	.word	.LC11
	.word	.LANCHOR0+78
	.word	.LC22
	.word	.LC23
	.fnend
	.size	vvi_node_delete, .-vvi_node_delete
	.align	1
	.global	vvi_node_start
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_start, %function
vvi_node_start:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, lr}
	.save {r4, r5, r6, r7, r8, lr}
	.pad #8
	mov	r4, r1
	ldr	r3, [r0, #128]
	ldr	r5, [r3, #160]
	ldr	r3, [r1, #24]
	cmp	r3, #164
	beq	.L81
	movs	r3, #222
	ldr	r2, .L95
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L95+4
	movs	r0, #8
	bl	vlog
	mov	r0, #-1
.L80:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, pc}
.L81:
	ldr	r2, [r1, #8]
	movs	r0, #8
	ldr	r1, [r1]
	bl	vsys_get_node_from_map
	ldrb	r3, [r4, #52]	@ zero_extendqisi2
	mov	r7, r0
	cbz	r3, .L83
	ldr	r3, [r4, #48]
	str	r3, [r5, #224]
	ldr	r8, [r4, #28]
	ldr	r6, [r4, #48]
	mov	r0, r8
	mov	r1, r6
	bl	__aeabi_uidivmod
	cbz	r1, .L84
	mov	r3, r6
	ldr	r2, [r4, #12]
	mov	r1, r8
	ldr	r0, [r4, #20]
	bl	get_new_wrap_line
	mov	r6, r0
.L84:
	bl	mcu_wrap_end
	mov	r0, r6
	bl	mcu_set_wrap_line
	movs	r1, #0
	ldr	r0, [r4, #28]
	bl	mcu_wrap_config
	ldrb	r3, [r4, #52]	@ zero_extendqisi2
	str	r3, [r5, #216]
	ldr	r3, [r4, #48]
	str	r3, [r5, #220]
.L83:
	movs	r3, #1
	str	r3, [r5, #4]
	cbz	r7, .L85
	ldr	r2, .L95+8
	movs	r1, #5
	movs	r3, #242
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L95+4
	bl	vlog
	mov	r0, r7
	bl	mpi_node_streamon
	movs	r3, #244
	ldr	r2, .L95+12
	movs	r1, #5
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L95+4
	bl	vlog
.L85:
	movs	r3, #247
	ldr	r2, .L95+16
	str	r3, [sp]
	movs	r1, #4
	ldr	r3, .L95+4
	movs	r0, #8
	bl	vlog
	movs	r0, #0
	b	.L80
.L96:
	.align	2
.L95:
	.word	.LC11
	.word	.LANCHOR0+94
	.word	.LC24
	.word	.LC25
	.word	.LC26
	.fnend
	.size	vvi_node_start, .-vvi_node_start
	.align	1
	.global	vvi_node_stop
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_stop, %function
vvi_node_stop:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r4, r1
	ldr	r3, [r1, #24]
	ldr	r5, [r0, #128]
	cmp	r3, #164
	beq	.L98
	movw	r3, #259
	ldr	r2, .L108
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L108+4
	movs	r0, #8
	bl	vlog
	mov	r0, #-1
.L97:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L98:
	ldr	r2, [r1, #8]
	movs	r0, #8
	ldr	r1, [r1]
	bl	vsys_get_node_from_map
	mov	r6, r0
	cbz	r0, .L100
	ldr	r3, [r4, #8]
	movs	r1, #4
	ldr	r2, .L108+8
	movs	r0, #8
	str	r3, [sp, #8]
	ldr	r3, [r4]
	str	r3, [sp, #4]
	movw	r3, #265
	str	r3, [sp]
	ldr	r3, .L108+4
	bl	vlog
	movs	r1, #1
	mov	r0, r6
	bl	mpi_node_flush
	movs	r1, #0
	str	r1, [r5, #-28]
	mov	r2, #1104
	ldr	r0, [r4]
	ldr	r3, [r4, #8]
	muls	r2, r0, r2
	movs	r0, #184
	mla	r3, r0, r3, r2
	add	r3, r3, r5
	str	r1, [r3, #912]
.L100:
	ldr	r0, [r5, #184]
	cmp	r0, #0
	beq	.L97
	ldr	r4, [r4, #8]
	cbnz	r4, .L101
	strd	r4, r4, [r5, #184]
	bl	mcu_wrap_end
	movs	r0, #10
	bl	msleep
.L101:
	movs	r0, #0
	b	.L97
.L109:
	.align	2
.L108:
	.word	.LC11
	.word	.LANCHOR0+109
	.word	.LC27
	.fnend
	.size	vvi_node_stop, .-vvi_node_stop
	.align	1
	.global	vvi_mb_put
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_mb_put, %function
vvi_mb_put:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r5, r1
	ldr	r3, [r1, #16]
	cmp	r3, #136
	beq	.L111
	mov	r3, #322
	ldr	r2, .L115
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L115+4
	movs	r0, #8
	bl	vlog
.L113:
	mov	r0, #-1
	b	.L110
.L111:
	ldrd	r1, r2, [r1]
	movs	r0, #8
	bl	vsys_get_node_from_map
	mov	r6, r0
	cmp	r0, #0
	beq	.L113
	movs	r1, #4
	ldr	r0, [r5, #8]
	bl	mpi_obj_from_id
	ldr	r2, .L115+8
	movs	r1, #5
	ldr	r3, [r0, #64]
	mov	r4, r0
	strd	r0, r3, [sp, #8]
	movs	r0, #8
	ldr	r3, [r5, #8]
	str	r3, [sp, #4]
	movw	r3, #331
	str	r3, [sp]
	ldr	r3, .L115+4
	bl	vlog
	cbz	r4, .L114
	movw	r3, #333
	ldr	r2, .L115+12
	str	r3, [sp]
	movs	r1, #5
	ldr	r3, .L115+4
	movs	r0, #8
	bl	vlog
	mov	r0, r6
	mov	r1, r4
	bl	mpi_node_release_buffer
	movs	r0, #0
.L110:
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L114:
	mov	r0, r4
	b	.L110
.L116:
	.align	2
.L115:
	.word	.LC6
	.word	.LANCHOR0+123
	.word	.LC28
	.word	.LC29
	.fnend
	.size	vvi_mb_put, .-vvi_mb_put
	.align	1
	.global	vvi_mb_get
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_mb_get, %function
vvi_mb_get:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	mov	r4, r1
	ldr	r2, [r1, #16]
	.pad #36
	sub	sp, sp, #36
	ldr	r3, [r0, #128]
	cmp	r2, #136
	beq	.L118
	mov	r3, #348
	ldr	r2, .L130
	str	r3, [sp]
	ldr	r3, .L130+4
.L129:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
.L122:
	mov	r0, #-1
.L117:
	add	sp, sp, #36
	@ sp needed
	pop	{r4, r5, pc}
.L118:
	ldr	r3, [r3, #184]
	ldr	r2, [r1, #4]
	cbz	r3, .L120
	cbnz	r2, .L120
	movw	r3, #353
	ldr	r2, .L130+8
	str	r3, [sp]
	ldr	r3, .L130+4
	b	.L129
.L120:
	ldr	r1, [r4]
	movs	r0, #8
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L122
	ldr	r2, [r4, #12]
	str	r2, [r0, #344]
	bl	mpi_node_pick_out_buffer
	mov	r5, r0
	cmp	r0, #0
	beq	.L122
	ldr	r3, [r0, #64]
	add	r1, r0, #232
	movs	r2, #80
	add	r0, r4, #56
	ldr	r3, [r3]
	str	r3, [r4, #52]
	bl	memcpy
	ldrd	r2, [r4, #112]
	ldr	r1, [r5]
	movs	r0, #8
	str	r1, [r4, #8]
	strd	r2, [sp, #16]
	ldr	r3, [r4, #108]
	ldr	r2, .L130+12
	str	r5, [sp, #4]
	strd	r1, r3, [sp, #8]
	movw	r3, #369
	movs	r1, #5
	str	r3, [sp]
	ldr	r3, .L130+4
	bl	vlog
	ldrd	r2, [r5, #288]
	movs	r1, #5
	movs	r0, #8
	strd	r2, [sp, #24]
	ldr	r3, [r5, #240]
	ldr	r2, .L130+16
	str	r3, [sp, #16]
	ldr	r3, [r5, #236]
	str	r3, [sp, #12]
	ldr	r3, [r5, #64]
	ldr	r3, [r3]
	str	r3, [sp, #8]
	ldr	r3, [r4, #8]
	str	r3, [sp, #4]
	movw	r3, #373
	str	r3, [sp]
	ldr	r3, .L130+4
	bl	vlog
	movs	r0, #0
	b	.L117
.L131:
	.align	2
.L130:
	.word	.LC6
	.word	.LANCHOR0+134
	.word	.LC30
	.word	.LC31
	.word	.LC32
	.fnend
	.size	vvi_mb_get, .-vvi_mb_get
	.align	1
	.global	vvi_node_get_dev_status
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_get_dev_status, %function
vvi_node_get_dev_status:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	mov	r5, r1
	ldr	r3, [r1, #168]
	.pad #20
	sub	sp, sp, #20
	ldrd	r4, r7, [r1]
	cmp	r3, #184
	ldr	r6, [r1, #8]
	ldr	r9, [r0, #128]
	beq	.L133
	movw	r3, #549
	ldr	r2, .L152
	str	r3, [sp]
	ldr	r3, .L152+4
.L151:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
.L149:
	mov	r4, #-1
	b	.L132
.L133:
	ldr	r3, .L152+8
	ldr	r3, [r3]
	cbnz	r3, .L144
.L136:
	adds	r3, r4, #1
	bne	.L140
	cmp	r7, #2
	bls	.L141
	movs	r3, #3
	strd	r7, r3, [sp, #4]
	movw	r3, #570
.L150:
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L152+4
	movs	r0, #8
	ldr	r2, .L152+12
	bl	vlog
.L132:
	mov	r0, r4
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L138:
	bl	mcu_get_wrap_status
	cbnz	r0, .L137
	movs	r0, #10
	bl	msleep
.L135:
	adds	r8, r8, #-1
	bcs	.L138
.L139:
	movw	r3, #563
	ldr	r2, .L152+16
	str	r3, [sp]
	ldr	r3, .L152+4
	b	.L151
.L144:
	mov	r8, #50
	b	.L135
.L137:
	cmp	r8, #0
	bne	.L136
	b	.L139
.L141:
	cmp	r6, #5
	bls	.L142
	movs	r3, #6
	strd	r7, r3, [sp, #4]
	movw	r3, #575
	b	.L150
.L142:
	mov	r3, #1104
	movs	r4, #184
	muls	r7, r3, r7
	mla	r6, r4, r6, r7
	add	r6, r6, r9
	ldr	r3, [r6, #920]
	str	r3, [r5, #172]
	ldr	r3, [r6, #912]
	str	r3, [r5, #164]
.L148:
	movs	r4, #0
	b	.L132
.L140:
	cmp	r4, #2
	bls	.L143
	movs	r3, #3
	ldr	r2, .L152+20
	strd	r4, r3, [sp, #4]
	movs	r1, #2
	movw	r3, #582
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L152+4
	bl	vlog
	b	.L149
.L143:
	movs	r3, #184
	mla	r4, r3, r4, r9
	ldr	r3, [r4, #368]
	str	r3, [r5, #172]
	b	.L148
.L153:
	.align	2
.L152:
	.word	.LC3
	.word	.LANCHOR0+145
	.word	.LANCHOR1
	.word	.LC4
	.word	.LC33
	.word	.LC5
	.fnend
	.size	vvi_node_get_dev_status, .-vvi_node_get_dev_status
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vi_release, %function
vi_release:
	.fnstart
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #28
	sub	sp, sp, #28
	ldr	r4, [r1, #128]
	mov	r0, r4
	bl	mpi_obj_unref
	ldr	r3, [r4]
	ldr	r2, .L172
	movs	r1, #4
	movs	r0, #8
	str	r3, [sp, #4]
	movw	r3, #698
	str	r3, [sp]
	ldr	r3, .L172+4
	bl	vlog
	ldr	r3, [r4, #16]
	cmp	r3, #1
	bne	.L155
	ldr	fp, .L172+8
	add	r9, r4, #196
	add	r8, r4, #748
	movs	r5, #0
.L160:
	movs	r2, #184
	movs	r1, #0
	mov	r0, r9
	mov	r7, r8
	bl	memset
	movs	r6, #0
.L159:
	cbz	r7, .L156
	movs	r2, #184
	movs	r1, #0
	mov	r0, r7
	bl	memset
.L156:
	mov	r2, r6
	mov	r1, r5
	movs	r0, #8
	bl	vsys_get_node_from_map
	mov	r10, r0
	cbz	r0, .L157
	ldr	r3, [r0, #60]
	mov	r2, r6
	mov	r1, r5
	movs	r0, #8
	str	r3, [sp, #20]
	bl	vsys_remove_node_from_map
	mov	r0, r10
	bl	mpi_node_destroy
	ldr	r3, [sp, #20]
	cbz	r3, .L158
	movs	r1, #1
	mov	r0, r3
	bl	mpi_buf_pool_destroy
.L158:
	movw	r3, #714
	strd	r5, r6, [sp, #4]
	str	r3, [sp]
	mov	r2, fp
	ldr	r3, .L172+4
	movs	r1, #4
	movs	r0, #8
	bl	vlog
.L157:
	adds	r6, r6, #1
	adds	r7, r7, #184
	cmp	r6, #6
	bne	.L159
	adds	r5, r5, #1
	add	r9, r9, #184
	cmp	r5, #3
	add	r8, r8, #1104
	bne	.L160
	movs	r3, #0
	strd	r3, r3, [r4, #184]
.L155:
	movs	r0, #0
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L173:
	.align	2
.L172:
	.word	.LC34
	.word	.LANCHOR0+169
	.word	.LC22
	.fnend
	.size	vi_release, .-vi_release
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vi_open, %function
vi_open:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L175
	push	{r0, r1, r2, lr}
	.save {lr}
	.pad #12
	add	r0, r3, #36
	str	r0, [r1, #128]
	movs	r2, #13
	str	r2, [r3, #32]
	bl	mpi_obj_ref
	movw	r3, #686
	ldr	r2, .L175+4
	movs	r1, #4
	movs	r0, #8
	str	r3, [sp]
	ldr	r3, .L175+8
	bl	vlog
	movs	r0, #0
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L176:
	.align	2
.L175:
	.word	.LANCHOR1
	.word	.LC35
	.word	.LANCHOR0+180
	.fnend
	.size	vi_open, .-vi_open
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vi_ioctl, %function
vi_ioctl:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #8
	mov	r3, r2
	ldr	r2, [r0, #128]
	mov	r6, r1
	uxtb	r4, r6
	ldr	r1, [r2, #160]
	ldr	r5, [r1, #28]
	cmp	r5, r4
	bls	.L178
	.syntax unified
@ 87 "./arch/arm/include/asm/barrier.h" 1
	cmp	r4, r5
	sbc	r5, r4, r4
.inst.w 0xf3af8014
@ 0 "" 2
	.thumb
	.syntax unified
	ldr	r1, [r1, #24]
	ands	r4, r4, r5
	movs	r5, #20
	mla	r1, r5, r4, r1
	cbz	r1, .L178
	mov	r2, r6
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	b	mpi_ioctl_kernel
.L178:
	str	r4, [sp, #4]
	mov	r3, #668
	ldr	r2, .L183
	movs	r1, #2
	str	r3, [sp]
	movs	r0, #8
	ldr	r3, .L183+4
	bl	vlog
	mvn	r0, #21
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, pc}
.L184:
	.align	2
.L183:
	.word	.LC36
	.word	.LANCHOR0+188
	.fnend
	.size	vi_ioctl, .-vi_ioctl
	.align	1
	.global	vvi_node_set_chns_attr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_set_chns_attr, %function
vvi_node_set_chns_attr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	.pad #20
	sub	sp, sp, #20
	ldr	r2, [r1, #168]
	ldr	r5, [r1]
	cmp	r2, #184
	ldr	r3, [r1, #8]
	ldr	r0, [r0, #128]
	beq	.L186
	mov	r3, #462
	ldr	r2, .L192
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L192+4
	movs	r0, #8
	bl	vlog
.L191:
	mov	r0, #-1
.L185:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L186:
	cmp	r5, #2
	bls	.L188
	movs	r3, #3
	ldr	r2, .L192+8
	strd	r5, r3, [sp, #4]
	mov	r3, #466
	str	r3, [sp]
	ldr	r3, .L192+4
.L190:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
	b	.L191
.L188:
	cmp	r3, #5
	bls	.L189
	movs	r2, #6
	strd	r3, r2, [sp, #4]
	mov	r3, #470
	ldr	r2, .L192+12
	str	r3, [sp]
	ldr	r3, .L192+4
	b	.L190
.L189:
	mov	r4, #1104
	muls	r4, r5, r4
	mla	r3, r2, r3, r4
	adds	r4, r0, r3
	add	r0, r4, #748
	ldr	r5, [r4, #920]
	bl	memcpy
	movs	r0, #0
	str	r5, [r4, #920]
	b	.L185
.L193:
	.align	2
.L192:
	.word	.LC3
	.word	.LANCHOR0+197
	.word	.LC5
	.word	.LC37
	.fnend
	.size	vvi_node_set_chns_attr, .-vvi_node_set_chns_attr
	.align	1
	.global	vvi_node_get_chns_attr
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_node_get_chns_attr, %function
vvi_node_get_chns_attr:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, lr}
	.save {r4, r5, lr}
	mov	r5, r1
	ldr	r2, [r5, #168]
	.pad #20
	sub	sp, sp, #20
	ldr	r3, [r1]
	cmp	r2, #184
	ldr	r4, [r1, #8]
	ldr	r1, [r0, #128]
	beq	.L195
	movw	r3, #489
	ldr	r2, .L201
	str	r3, [sp]
	movs	r1, #2
	ldr	r3, .L201+4
	movs	r0, #8
	bl	vlog
.L200:
	mov	r0, #-1
.L194:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, pc}
.L195:
	cmp	r3, #2
	bls	.L197
	movs	r2, #3
	strd	r3, r2, [sp, #4]
	movw	r3, #493
	ldr	r2, .L201+8
	str	r3, [sp]
	ldr	r3, .L201+4
.L199:
	movs	r1, #2
	movs	r0, #8
	bl	vlog
	b	.L200
.L197:
	cmp	r4, #5
	bls	.L198
	movs	r3, #6
	ldr	r2, .L201+12
	strd	r4, r3, [sp, #4]
	movw	r3, #497
	str	r3, [sp]
	ldr	r3, .L201+4
	b	.L199
.L198:
	muls	r4, r2, r4
	mov	r0, #1104
	mla	r3, r0, r3, r4
	mov	r0, r5
	add	r1, r1, r3
	add	r1, r1, #748
	bl	memcpy
	movs	r0, #0
	b	.L194
.L202:
	.align	2
.L201:
	.word	.LC3
	.word	.LANCHOR0+220
	.word	.LC5
	.word	.LC37
	.fnend
	.size	vvi_node_get_chns_attr, .-vvi_node_get_chns_attr
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	kzalloc.constprop.2, %function
kzalloc.constprop.2:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L204
	mov	r1, #3520
	ldr	r0, [r3, #48]
	b	kmem_cache_alloc
.L205:
	.align	2
.L204:
	.word	kmalloc_caches
	.fnend
	.size	kzalloc.constprop.2, .-kzalloc.constprop.2
	.align	1
	.global	rockit_free_tb_stream_buf
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_free_tb_stream_buf, %function
rockit_free_tb_stream_buf:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	bl	kzalloc.constprop.2
	mov	r5, r0
	cbnz	r0, .L207
	ldr	r0, .L209
	bl	printk
	mov	r0, #-1
.L206:
	pop	{r3, r4, r5, pc}
.L207:
	ldr	r3, .L209+4
	movs	r4, #0
	str	r4, [r0, #24]
	str	r3, [r0, #4]
	bl	rkisp_rockit_free_tb_stream_buf
	mov	r0, r5
	bl	kfree
	mov	r0, r4
	b	.L206
.L210:
	.align	2
.L209:
	.word	.LC38
	.word	.LC39
	.fnend
	.size	rockit_free_tb_stream_buf, .-rockit_free_tb_stream_buf
	.align	1
	.global	rockit_get_tb_stream_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	rockit_get_tb_stream_info, %function
rockit_get_tb_stream_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	mov	r6, r0
	bl	kzalloc.constprop.2
	mov	r5, r0
	cbnz	r0, .L212
	ldr	r0, .L214
	bl	printk
	mov	r0, #-1
.L211:
	pop	{r4, r5, r6, pc}
.L212:
	ldr	r3, .L214+4
	movs	r4, #0
	mov	r1, r6
	str	r4, [r0, #24]
	str	r3, [r0, #4]
	bl	rkisp_rockit_get_tb_stream_info
	mov	r0, r5
	bl	kfree
	mov	r0, r4
	b	.L211
.L215:
	.align	2
.L214:
	.word	.LC38
	.word	.LC39
	.fnend
	.size	rockit_get_tb_stream_info, .-rockit_get_tb_stream_info
	.align	1
	.global	vvi_change_resolution
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_change_resolution, %function
vvi_change_resolution:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #12
	mov	r5, r1
	ldr	r3, [r0, #128]
	ldr	r7, [r3, #160]
	bl	kzalloc.constprop.2
	mov	r6, r0
	cbnz	r0, .L217
	ldr	r0, .L235
	bl	printk
.L226:
	mov	r4, #-1
	b	.L216
.L217:
	ldr	r3, [r5, #16]
	cmp	r3, #36
	beq	.L219
	ldr	r1, .L235+4
	mov	r2, #396
	ldr	r0, .L235+8
.L234:
	bl	printk
	b	.L226
.L219:
	ldrd	r1, r2, [r5]
	movs	r0, #8
	bl	vsys_get_node_from_map
	cmp	r0, #0
	beq	.L226
	ldr	r3, [r0, #56]
	movs	r1, #4
	ldr	r8, [r7, #220]
	ldr	r2, .L235+12
	str	r3, [r6, #24]
	ldr	r3, [r0, #44]
	movs	r0, #8
	ldrd	r10, r9, [r5, #20]
	ldr	r3, [r3, #44]
	str	r3, [r6, #4]
	ldr	r3, [r7, #216]
	str	r3, [sp, #4]
	movw	r3, #413
	str	r3, [sp]
	ldr	r3, .L235+4
	bl	vlog
	ldr	r3, [r7, #216]
	cbz	r3, .L220
	ldr	r3, [r5, #4]
	cbnz	r3, .L220
	ldr	r3, [r7, #224]
	mov	r1, r9
	ldr	r2, [r5, #8]
	mov	r0, r10
	bl	get_new_wrap_line
	mov	r8, r0
	bl	mcu_wrap_end
	movs	r0, #10
	bl	msleep
.L220:
	mov	fp, #2
.L224:
	mov	r0, r6
	bl	rkisp_rockit_pause_stream
	cbz	r0, .L221
	mov	r2, #424
	ldr	r1, .L235+4
	ldr	r0, .L235+16
	b	.L234
.L227:
	mov	fp, #1
	b	.L224
.L221:
	mov	r3, r8
	mov	r2, r9
	mov	r1, r10
	mov	r0, r6
	bl	rkisp_rockit_config_stream
	cbz	r0, .L222
	movw	r2, #429
	ldr	r1, .L235+4
	ldr	r0, .L235+20
	mvn	r4, #1
	bl	printk
.L216:
	mov	r0, r4
	add	sp, sp, #12
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L222:
	mov	r0, r6
	bl	rkisp_rockit_resume_stream
	mov	r4, r0
	cbz	r0, .L223
	movw	r2, #435
	ldr	r1, .L235+4
	ldr	r0, .L235+24
	mvn	r4, #2
	bl	printk
	b	.L216
.L223:
	cmp	fp, #1
	bne	.L227
	ldr	r3, [r7, #216]
	cbz	r3, .L225
	ldr	r3, [r5, #4]
	cbnz	r3, .L225
	mov	r0, r8
	bl	mcu_set_wrap_line
	mov	r1, r8
	mov	r0, r9
	bl	mcu_wrap_config
	str	r8, [r7, #220]
.L225:
	mov	r0, r6
	bl	kfree
	b	.L216
.L236:
	.align	2
.L235:
	.word	.LC38
	.word	.LANCHOR0+243
	.word	.LC40
	.word	.LC41
	.word	.LC42
	.word	.LC43
	.word	.LC44
	.fnend
	.size	vvi_change_resolution, .-vvi_change_resolution
	.align	1
	.global	vvi_probe
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_probe, %function
vvi_probe:
	.fnstart
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, lr}
	.save {r4, r5, r6, r7, r8, r9, lr}
	.pad #60
	sub	sp, sp, #60
	ldr	r7, .L241
	mov	r5, r0
	ldr	r4, .L241+4
	movw	r2, #4092
	movs	r1, #0
	ldr	r9, .L241+12
	ldr	r3, [r7]
	adds	r6, r4, #4
	ldr	r8, .L241+16
	mov	r0, r6
	str	r3, [sp, #52]
	bl	memset
	mov	r1, #3264
	mov	r0, r5
	bl	kstrdup
	str	r6, [r4, #196]
	ldr	r6, .L241+8
	mov	r2, r5
	movs	r1, #8
	movs	r3, #1
	movs	r5, #0
	str	r3, [r4, #216]
	str	r6, [r4, #28]
	movs	r3, #13
	str	r3, [r4, #32]
	str	r0, [r4, #4]
	add	r0, r4, #36
	bl	mpi_dev_register
	str	r5, [r4, #104]
	strd	r5, r5, [r4, #112]
	add	r1, r6, #260
	add	r0, r4, #36
	add	r6, r6, #388
	bl	mpi_sysfs_dev_add
	adds	r4, r4, #200
.L238:
	mov	r3, r5
	mov	r2, r9
	movs	r1, #32
	add	r0, sp, #20
	bl	snprintf
	add	r0, sp, #20
	bl	mpi_dev_find_by_name
	movw	r3, #762
	mov	r2, r8
	movs	r1, #4
	str	r0, [r4], #4
	strd	r5, r0, [sp, #4]
	movs	r0, #8
	str	r3, [sp]
	adds	r5, r5, #1
	mov	r3, r6
	bl	vlog
	cmp	r5, #3
	bne	.L238
	ldr	r2, [sp, #52]
	ldr	r3, [r7]
	cmp	r2, r3
	beq	.L239
	bl	__stack_chk_fail
.L239:
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, pc}
.L242:
	.align	2
.L241:
	.word	__stack_chk_guard
	.word	.LANCHOR1
	.word	.LANCHOR0+268
	.word	.LC45
	.word	.LC46
	.fnend
	.size	vvi_probe, .-vvi_probe
	.align	1
	.global	vvi_shutdown
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_shutdown, %function
vvi_shutdown:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	movs	r3, #1
	ldr	r4, .L244
	add	r0, r4, #36
	str	r3, [r4, #16]
	bl	mpi_dev_unregister
	ldr	r0, [r4, #4]
	pop	{r4, lr}
	b	kfree
.L245:
	.align	2
.L244:
	.word	.LANCHOR1
	.fnend
	.size	vvi_shutdown, .-vvi_shutdown
	.section	.init.text,"ax",%progbits
	.align	1
	.global	vvi_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_init, %function
vvi_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	.save {r3, lr}
	ldr	r0, .L247
	bl	vvi_probe
	movs	r0, #0
	pop	{r3, pc}
.L248:
	.align	2
.L247:
	.word	.LC17
	.fnend
	.size	vvi_init, .-vvi_init
	.section	.exit.text,"ax",%progbits
	.align	1
	.global	vvi_exit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	vvi_exit, %function
vvi_exit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	b	vvi_shutdown
	.fnend
	.size	vvi_exit, .-vvi_exit
	.global	wait_mcu_flag
	.global	g_vvi_dev
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	__func__.38245, %object
	.size	__func__.38245, 18
__func__.38245:
	.ascii	"get_new_wrap_line\000"
	.type	__func__.38395, %object
	.size	__func__.38395, 24
__func__.38395:
	.ascii	"vvi_node_set_dev_status\000"
	.type	__func__.38282, %object
	.size	__func__.38282, 20
__func__.38282:
	.ascii	"vvi_node_get_status\000"
	.type	__func__.38217, %object
	.size	__func__.38217, 16
__func__.38217:
	.ascii	"vvi_node_create\000"
	.type	__func__.38235, %object
	.size	__func__.38235, 16
__func__.38235:
	.ascii	"vvi_node_delete\000"
	.type	__func__.38257, %object
	.size	__func__.38257, 15
__func__.38257:
	.ascii	"vvi_node_start\000"
	.type	__func__.38271, %object
	.size	__func__.38271, 14
__func__.38271:
	.ascii	"vvi_node_stop\000"
	.type	__func__.38313, %object
	.size	__func__.38313, 11
__func__.38313:
	.ascii	"vvi_mb_put\000"
	.type	__func__.38328, %object
	.size	__func__.38328, 11
__func__.38328:
	.ascii	"vvi_mb_get\000"
	.type	__func__.38411, %object
	.size	__func__.38411, 24
__func__.38411:
	.ascii	"vvi_node_get_dev_status\000"
	.type	__func__.38488, %object
	.size	__func__.38488, 11
__func__.38488:
	.ascii	"vi_release\000"
	.type	__func__.38472, %object
	.size	__func__.38472, 8
__func__.38472:
	.ascii	"vi_open\000"
	.type	__func__.38456, %object
	.size	__func__.38456, 9
__func__.38456:
	.ascii	"vi_ioctl\000"
	.type	__func__.38364, %object
	.size	__func__.38364, 23
__func__.38364:
	.ascii	"vvi_node_set_chns_attr\000"
	.type	__func__.38379, %object
	.size	__func__.38379, 23
__func__.38379:
	.ascii	"vvi_node_get_chns_attr\000"
	.type	__func__.38343, %object
	.size	__func__.38343, 22
__func__.38343:
	.ascii	"vvi_change_resolution\000"
	.space	3
	.type	vvi_ioctls, %object
	.size	vvi_ioctls, 260
vvi_ioctls:
	.word	-1065585152
	.word	0
	.word	vvi_get_version
	.word	.LC47
	.word	0
	.word	-1062963711
	.word	0
	.word	vvi_node_create
	.word	.LC48
	.word	0
	.word	-1062963710
	.word	0
	.word	vvi_node_delete
	.word	.LC49
	.word	0
	.word	-1064798717
	.word	0
	.word	vvi_node_get_status
	.word	.LC50
	.word	0
	.word	-1062963708
	.word	0
	.word	vvi_node_start
	.word	.LC51
	.word	0
	.word	-1062963707
	.word	0
	.word	vvi_node_stop
	.word	.LC52
	.word	0
	.word	-1064798714
	.word	0
	.word	vvi_mb_put
	.word	.LC53
	.word	0
	.word	-1064798713
	.word	16
	.word	vvi_mb_get
	.word	.LC54
	.word	0
	.word	-1071352312
	.word	0
	.word	vvi_change_resolution
	.word	.LC55
	.word	0
	.word	-1061652983
	.word	0
	.word	vvi_node_set_chns_attr
	.word	.LC56
	.word	0
	.word	-1061652982
	.word	0
	.word	vvi_node_get_chns_attr
	.word	.LC57
	.word	0
	.word	-1061652981
	.word	0
	.word	vvi_node_set_dev_status
	.word	.LC58
	.word	0
	.word	-1061652980
	.word	0
	.word	vvi_node_get_dev_status
	.word	.LC59
	.word	0
	.type	vvi_driver_fops, %object
	.size	vvi_driver_fops, 128
vvi_driver_fops:
	.word	0
	.space	4
	.word	0
	.space	24
	.word	0
	.word	vi_ioctl
	.word	vi_compat_ioctl
	.word	0
	.space	4
	.word	vi_open
	.space	4
	.word	vi_release
	.space	60
	.type	__func__.38512, %object
	.size	__func__.38512, 10
__func__.38512:
	.ascii	"vvi_probe\000"
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	wait_mcu_flag, %object
	.size	wait_mcu_flag, 4
wait_mcu_flag:
	.space	4
	.type	g_vvi_dev, %object
	.size	g_vvi_dev, 4092
g_vvi_dev:
	.space	4092
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"-2023-02-02-15:30:51\000"
.LC1:
	.ascii	"vmpi:9093280df536e902dd5125cdc9953509ebc59a74-v1.4."
	.ascii	"32\000"
.LC2:
	.ascii	"\033[32m\033[1m%s,%d:tmp = %d, vi_res->wrap_line = "
	.ascii	"%d, cur_w = %d, cur_h = %d, max_res_width = %d\033["
	.ascii	"0m\000"
.LC3:
	.ascii	"\033[31m\033[1m%s,%d:struct vi_attr_kn_ctx mismatch"
	.ascii	"ing\033[0m\000"
.LC4:
	.ascii	"\033[31m\033[1m%s,%d:pipeId %d need less than %d\033"
	.ascii	"[0m\000"
.LC5:
	.ascii	"\033[31m\033[1m%s,%d:devId %d need less than %d\033"
	.ascii	"[0m\000"
.LC6:
	.ascii	"\033[31m\033[1m%s,%d:struct vvi_buff_info mismatchi"
	.ascii	"ng\033[0m\000"
.LC7:
	.ascii	"%s,%d:enter...\000"
.LC8:
	.ascii	"%s,%d:buf pool(%d) find\000"
.LC9:
	.ascii	"%s,%d:buf = %px ref cnt = %d\000"
.LC10:
	.ascii	"%s,%d:exit...\000"
.LC11:
	.ascii	"\033[31m\033[1m%s,%d:struct vvi_node_info mismatchi"
	.ascii	"ng\033[0m\000"
.LC12:
	.ascii	"valloc\000"
.LC13:
	.ascii	"\033[31m\033[1m%s,%d:error getting vdev  %p\033[0m\000"
.LC14:
	.ascii	"\033[32m\033[1m%s,%d:vvi chn:%d node have exist!\033"
	.ascii	"[0m\000"
.LC15:
	.ascii	"\033[31m\033[1m%s,%d:error create chn:%d  vvi node\033"
	.ascii	"[0m\000"
.LC16:
	.ascii	"\033[32m\033[1m%s,%d: vvi chn:%d node create succes"
	.ascii	"s\033[0m\000"
.LC17:
	.ascii	"vvi\000"
.LC18:
	.ascii	"\033[32m\033[1m%s,%d:vvi_node_create chn:%d pool cr"
	.ascii	"eate %d\033[0m\000"
.LC19:
	.ascii	"\033[31m\033[1m%s,%d:chn:%d pool create fail\033[0m"
	.ascii	"\000"
.LC20:
	.ascii	"\033[32m\033[1m%s,%d:vvi_node_create vi_node_info->"
	.ascii	"mbPool:%d\033[0m\000"
.LC21:
	.ascii	"\033[32m\033[1m%s,%d:vvi_node_create done\033[0m\000"
.LC22:
	.ascii	"\033[32m\033[1m%s,%d:delete vvi node: dev:%d, chn:%"
	.ascii	"d\033[0m\000"
.LC23:
	.ascii	"%s,%d:do vi vvi_node_delete\000"
.LC24:
	.ascii	"%s,%d:mpi_node_streamon start\000"
.LC25:
	.ascii	"%s,%d:mpi_node_add_out_watcher start\000"
.LC26:
	.ascii	"\033[32m\033[1m%s,%d:vvi_node_start done\033[0m\000"
.LC27:
	.ascii	"\033[32m\033[1m%s,%d:do vi vvi_node_stop dev:%d, ch"
	.ascii	"n:%d\033[0m\000"
.LC28:
	.ascii	"%s,%d:put buff: id:%d, buf:%p, dma_buf:%p\000"
.LC29:
	.ascii	"%s,%d:unref buffer\000"
.LC30:
	.ascii	"\033[31m\033[1m%s,%d:wrap mode not support get buff"
	.ascii	"er from chn 0\033[0m\000"
.LC31:
	.ascii	"%s,%d:get buff: %px, dma_buf id = %d, frame seq=%d,"
	.ascii	" pts:%lld\000"
.LC32:
	.ascii	"%s,%d:buf id:%d size:%d w:%d h:%d pts:%llu\000"
.LC33:
	.ascii	"\033[31m\033[1m%s,%d:wait mcu start fail\033[0m\000"
.LC34:
	.ascii	"\033[32m\033[1m%s,%d:%d\033[0m\000"
.LC35:
	.ascii	"\033[32m\033[1m%s,%d:--------- vi open ----------\033"
	.ascii	"[0m\000"
.LC36:
	.ascii	"\033[31m\033[1m%s,%d:vi_ioctl: error vvi ioctl, inv"
	.ascii	"alid ioctrl index %d\033[0m\000"
.LC37:
	.ascii	"\033[31m\033[1m%s,%d:chnId %d need less than %d\033"
	.ascii	"[0m\000"
.LC38:
	.ascii	"\0013no memory for rockit_cfg\000"
.LC39:
	.ascii	"rkisp-vir0\000"
.LC40:
	.ascii	"\0013%s[%d] struct vvi_resolution mismatching\000"
.LC41:
	.ascii	"\033[32m\033[1m%s,%d:vi_res->wrap_mode = %d\033[0m\000"
.LC42:
	.ascii	"\0013%s[%d]:rkisp_rockit_pause_stream fail\000"
.LC43:
	.ascii	"\0013%s[%d]:rkisp_rockit_config_stream fail\000"
.LC44:
	.ascii	"\0013%s[%d]:rkisp_rockit_resume_stream fail\000"
.LC45:
	.ascii	"rkisp-vir%d\000"
.LC46:
	.ascii	"\033[32m\033[1m%s,%d:>>>>>> rkisp-vir%d: %p\033[0m\000"
.LC47:
	.ascii	"VVI_IOCTL_VERSION\000"
.LC48:
	.ascii	"VVI_IOCTL_NODE_CREATE\000"
.LC49:
	.ascii	"VVI_IOCTL_NODE_DELETE\000"
.LC50:
	.ascii	"VVI_IOCTL_NODE_GET_STATUS\000"
.LC51:
	.ascii	"VVI_IOCTL_NODE_START\000"
.LC52:
	.ascii	"VVI_IOCTL_NODE_STOP\000"
.LC53:
	.ascii	"VVI_IOCTL_MB_PUT\000"
.LC54:
	.ascii	"VVI_IOCTL_MB_GET\000"
.LC55:
	.ascii	"VVI_IOCTL_RES_CHANGE\000"
.LC56:
	.ascii	"VVI_IOCTL_SET_ATTR\000"
.LC57:
	.ascii	"VVI_IOCTL_GET_ATTR\000"
.LC58:
	.ascii	"VVI_IOCTL_SET_DEV_STA\000"
.LC59:
	.ascii	"VVI_IOCTL_GET_DEV_STA\000"
	.section	___ksymtab_gpl+rockit_free_tb_stream_buf,"a"
	.align	2
	.type	__ksymtab_rockit_free_tb_stream_buf, %object
	.size	__ksymtab_rockit_free_tb_stream_buf, 12
__ksymtab_rockit_free_tb_stream_buf:
	.word	rockit_free_tb_stream_buf
	.word	__kstrtab_rockit_free_tb_stream_buf
	.word	__kstrtabns_rockit_free_tb_stream_buf
	.section	___ksymtab_gpl+rockit_get_tb_stream_info,"a"
	.align	2
	.type	__ksymtab_rockit_get_tb_stream_info, %object
	.size	__ksymtab_rockit_get_tb_stream_info, 12
__ksymtab_rockit_get_tb_stream_info:
	.word	rockit_get_tb_stream_info
	.word	__kstrtab_rockit_get_tb_stream_info
	.word	__kstrtabns_rockit_get_tb_stream_info
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
