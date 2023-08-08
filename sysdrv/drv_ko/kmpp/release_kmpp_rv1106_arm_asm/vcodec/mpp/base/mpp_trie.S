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
	.file	"mpp_trie.c"
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
	.type	trie_get_node, %function
trie_get_node:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r2, r3, r4, r5, r6, lr}
	.save {r4, r5, r6, lr}
	.pad #16
	mov	r4, r0
	ldrd	r5, r3, [r0, #12]
	cmp	r3, r5
	blt	.L2
	lsls	r5, r5, #1
	movs	r1, #40
	ldr	r0, .L12
	muls	r1, r5, r1
	bl	mpp_osal_malloc
	mov	r6, r0
	cbnz	r0, .L3
	mov	r3, r5
	movs	r2, #64
	ldr	r1, .L12+4
	mov	r5, #-1
	ldr	r0, .L12+8
	bl	printk
.L1:
	mov	r0, r5
	add	sp, sp, #16
	@ sp needed
	pop	{r4, r5, r6, pc}
.L3:
	ldr	r3, .L12+12
	ldr	r3, [r3]
	lsls	r2, r3, #28
	bpl	.L5
	ldr	r3, [r4, #12]
	movs	r2, #70
	strd	r0, r5, [sp, #8]
	ldr	r1, .L12+4
	str	r3, [sp, #4]
	ldr	r3, [r4, #20]
	ldr	r0, .L12+16
	str	r3, [sp]
	mov	r3, r4
	bl	printk
.L5:
	ldr	r3, [r4, #12]
	movs	r2, #40
	ldr	r1, [r4, #20]
	mov	r0, r6
	muls	r2, r3, r2
	bl	memcpy
	str	r6, [r4, #20]
	str	r5, [r4, #12]
.L2:
	ldr	r5, [r4, #16]
	ldr	r2, [r4, #20]
	adds	r3, r5, #1
	str	r3, [r4, #16]
	movs	r3, #40
	muls	r3, r5, r3
	adds	r1, r2, r3
	str	r5, [r2, r3]
	mov	r3, #-1
	str	r3, [r1, #4]
	ldr	r3, .L12+12
	ldr	r3, [r3]
	lsls	r3, r3, #28
	bpl	.L1
	mov	r3, r5
	movs	r2, #83
	ldr	r1, .L12+4
	ldr	r0, .L12+20
	bl	printk
	b	.L1
.L13:
	.align	2
.L12:
	.word	.LANCHOR0
	.word	.LANCHOR0+14
	.word	.LC0
	.word	.LANCHOR1
	.word	.LC1
	.word	.LC2
	.fnend
	.size	trie_get_node, .-trie_get_node
	.align	1
	.global	mpp_trie_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_trie_init, %function
mpp_trie_init:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, r6, r7, lr}
	.save {r3, r4, r5, r6, r7, lr}
	mov	r7, r0
	mov	r6, r1
	ldr	r0, .L31
	movs	r1, #24
	mov	r5, r2
	bl	mpp_osal_calloc
	mov	r4, r0
	cbnz	r7, .L15
	movs	r2, #94
	ldr	r1, .L31+4
	ldr	r0, .L31+8
	bl	printk
	mvn	r0, #2
.L14:
	pop	{r3, r4, r5, r6, r7, pc}
.L15:
	cbnz	r0, .L17
	movs	r2, #100
	ldr	r1, .L31+4
	ldr	r0, .L31+12
	bl	printk
	ldr	r0, .L31+16
.L18:
	str	r4, [r7]
	b	.L14
.L17:
	cmp	r6, #0
	mov	r1, #40
	it	eq
	moveq	r6, #900
	str	r6, [r0, #12]
	muls	r1, r6, r1
	ldr	r0, .L31
	bl	mpp_osal_calloc
	str	r0, [r4, #20]
	cbnz	r0, .L20
	ldr	r3, [r4, #12]
	movs	r2, #108
	ldr	r1, .L31+4
	ldr	r0, .L31+20
.L30:
	bl	printk
	ldr	r1, [r4, #8]
	cbnz	r1, .L25
.L26:
	ldr	r1, [r4, #20]
	movs	r5, #0
	str	r5, [r4, #8]
	cbz	r1, .L24
	ldr	r0, .L31
	bl	mpp_osal_free
.L24:
	str	r5, [r4, #20]
	mov	r1, r4
	ldr	r0, .L31
	movs	r4, #0
	bl	mpp_osal_free
	ldr	r0, .L31+16
	b	.L18
.L20:
	cmp	r5, #0
	ldr	r0, .L31
	it	eq
	moveq	r5, #80
	str	r5, [r4]
	lsls	r1, r5, #2
	bl	mpp_osal_calloc
	str	r0, [r4, #8]
	cbnz	r0, .L23
	ldr	r3, [r4]
	movs	r2, #116
	ldr	r1, .L31+4
	ldr	r0, .L31+24
	b	.L30
.L23:
	mov	r0, r4
	bl	trie_get_node
	movs	r0, #0
	b	.L18
.L25:
	ldr	r0, .L31
	bl	mpp_osal_free
	b	.L26
.L32:
	.align	2
.L31:
	.word	.LANCHOR0+28
	.word	.LANCHOR0+42
	.word	.LC3
	.word	.LC4
	.word	-1006
	.word	.LC5
	.word	.LC6
	.fnend
	.size	mpp_trie_init, .-mpp_trie_init
	.align	1
	.global	mpp_trie_deinit
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_trie_deinit, %function
mpp_trie_deinit:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, r4, r5, lr}
	.save {r3, r4, r5, lr}
	mov	r4, r0
	cbnz	r0, .L34
	movs	r2, #140
	ldr	r1, .L44
	ldr	r0, .L44+4
	bl	printk
	mvn	r0, #2
.L33:
	pop	{r3, r4, r5, pc}
.L34:
	ldr	r1, [r0, #20]
	cbz	r1, .L36
	ldr	r0, .L44+8
	bl	mpp_osal_free
.L36:
	ldr	r1, [r4, #8]
	movs	r5, #0
	str	r5, [r4, #20]
	cbz	r1, .L37
	ldr	r0, .L44+8
	bl	mpp_osal_free
.L37:
	ldr	r0, .L44+8
	mov	r1, r4
	str	r5, [r4, #8]
	bl	mpp_osal_free
	movs	r0, #0
	b	.L33
.L45:
	.align	2
.L44:
	.word	.LANCHOR0+56
	.word	.LC3
	.word	.LANCHOR0+72
	.fnend
	.size	mpp_trie_deinit, .-mpp_trie_deinit
	.align	1
	.global	mpp_trie_add_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_trie_add_info, %function
mpp_trie_add_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, fp, lr}
	.pad #60
	sub	sp, sp, #60
	ldr	r8, [r1]
	mov	r4, r0
	str	r1, [sp, #36]
	mov	r1, #1024
	mov	r0, r8
	bl	strnlen
	str	r0, [sp, #44]
	cbz	r4, .L47
	ldr	r3, [sp, #36]
	cbnz	r3, .L48
.L47:
	ldr	r3, [sp, #36]
	movs	r2, #162
	ldr	r1, .L87
	ldr	r0, .L87+4
	str	r3, [sp]
	mov	r3, r4
	bl	printk
	mvn	r0, #2
.L46:
	add	sp, sp, #60
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
.L48:
	ldrd	r1, r3, [r4]
	cmp	r3, r1
	blt	.L50
	lsls	r6, r1, #1
	ldr	r0, .L87+8
	lsls	r1, r1, #3
	bl	mpp_osal_malloc
	mov	r5, r0
	cbnz	r0, .L51
	mov	r3, r6
	movs	r2, #172
	ldr	r1, .L87
	ldr	r0, .L87+12
	bl	printk
	mvn	r0, #3
	b	.L46
.L51:
	ldr	r3, .L87+16
	ldr	r3, [r3]
	lsls	r1, r3, #28
	bpl	.L52
	ldr	r3, [r4]
	movs	r2, #177
	strd	r0, r6, [sp, #8]
	ldr	r1, .L87
	str	r3, [sp, #4]
	ldr	r3, [r4, #8]
	ldr	r0, .L87+20
	str	r3, [sp]
	mov	r3, r4
	bl	printk
.L52:
	ldr	r2, [r4]
	mov	r1, r5
	ldr	r0, [r4, #8]
	lsls	r2, r2, #2
	bl	memcpy
	str	r5, [r4, #8]
	str	r6, [r4]
.L50:
	ldr	r3, .L87+16
	ldr	r3, [r3]
	lsls	r2, r3, #30
	bpl	.L53
	ldr	r3, [sp, #44]
	movs	r2, #183
	ldr	r1, .L87
	ldr	r0, .L87+24
	strd	r8, r3, [sp]
	mov	r3, r4
	bl	printk
.L53:
	ldr	r10, .L87+16
	movs	r6, #0
	mov	r7, r6
.L54:
	movs	r3, #40
	muls	r3, r7, r3
	str	r3, [sp, #48]
	ldr	r3, [r4, #20]
	ldr	r2, [sp, #48]
	str	r3, [sp, #52]
	add	fp, r3, r2
	ldr	r3, [sp, #44]
	cmp	r6, r3
	bge	.L60
	ldrb	r5, [r8, r6]	@ zero_extendqisi2
	cbnz	r5, .L61
.L60:
	ldr	r3, [r4, #4]
	ldr	r1, [sp, #36]
	adds	r2, r3, #1
	str	r2, [r4, #4]
	str	r3, [fp, #4]
	ldr	r2, [r4, #8]
	str	r1, [r2, r3, lsl #2]
	ldr	r0, [r10]
	ands	r0, r0, #2
	beq	.L46
	strd	r3, r1, [sp, #12]
	movs	r2, #237
	strd	r8, r7, [sp, #4]
	mov	r3, r4
	str	r6, [sp]
	ldr	r1, .L87
	ldr	r0, .L87+28
	bl	printk
	movs	r0, #0
	b	.L46
.L61:
	and	r3, r5, #15
	str	r3, [sp, #40]
	lsrs	r3, r5, #4
	add	fp, fp, r3, lsl #1
	ldr	r3, [r10]
	lsls	r3, r3, #30
	ldrsh	r9, [fp, #8]
	bpl	.L55
	ldr	r3, [sp, #40]
	movs	r2, #195
	strd	r7, r9, [sp, #24]
	strd	r6, r5, [sp, #4]
	str	r3, [sp, #20]
	lsrs	r3, r5, #4
	str	r8, [sp]
	strd	r5, r3, [sp, #12]
	mov	r3, r4
	ldr	r1, .L87
	ldr	r0, .L87+32
	bl	printk
.L55:
	cmp	r9, #0
	bne	.L56
	mov	r0, r4
	bl	trie_get_node
	ldr	r3, [r10]
	mov	r9, r0
	strh	r0, [fp, #8]	@ movhi
	lsls	r0, r3, #30
	bpl	.L56
	ldrd	r2, r3, [sp, #48]
	str	r9, [sp, #20]
	ldr	r1, .L87
	ldr	r3, [r3, r2]
	movs	r2, #203
	strd	r6, r5, [sp, #4]
	str	r8, [sp]
	strd	r5, r3, [sp, #12]
	mov	r3, r4
	ldr	r0, .L87+36
	bl	printk
.L56:
	movs	r3, #40
	mul	r3, r3, r9
	str	r3, [sp, #48]
	ldr	r3, [r4, #20]
	ldr	r2, [sp, #48]
	str	r3, [sp, #52]
	add	fp, r3, r2
	ldr	r3, [sp, #40]
	add	fp, fp, r3, lsl #1
	ldr	r3, [r10]
	lsls	r1, r3, #30
	ldrsh	r7, [fp, #8]
	bpl	.L57
	ldr	r3, [sp, #40]
	movs	r2, #212
	strd	r9, r7, [sp, #24]
	strd	r6, r5, [sp, #4]
	str	r3, [sp, #20]
	lsrs	r3, r5, #4
	str	r8, [sp]
	strd	r5, r3, [sp, #12]
	mov	r3, r4
	ldr	r1, .L87
	ldr	r0, .L87+40
	bl	printk
.L57:
	cbnz	r7, .L58
	mov	r0, r4
	bl	trie_get_node
	ldr	r3, [r10]
	lsls	r2, r3, #30
	mov	r7, r0
	strh	r0, [fp, #8]	@ movhi
	bpl	.L58
	ldrd	r2, r3, [sp, #48]
	str	r0, [sp, #20]
	ldr	r1, .L87
	ldr	r3, [r3, r2]
	movs	r2, #220
	strd	r6, r5, [sp, #4]
	str	r8, [sp]
	strd	r5, r3, [sp, #12]
	mov	r3, r4
	ldr	r0, .L87+44
	bl	printk
.L58:
	ldr	r3, [r10]
	lsls	r3, r3, #30
	bpl	.L59
	ldr	r3, [sp, #40]
	movs	r2, #228
	strd	r7, r7, [sp, #24]
	strd	r6, r5, [sp, #4]
	str	r3, [sp, #20]
	lsrs	r3, r5, #4
	str	r8, [sp]
	strd	r5, r3, [sp, #12]
	mov	r3, r4
	ldr	r1, .L87
	ldr	r0, .L87+48
	bl	printk
.L59:
	adds	r6, r6, #1
	b	.L54
.L88:
	.align	2
.L87:
	.word	.LANCHOR0+88
	.word	.LC7
	.word	.LANCHOR0+106
	.word	.LC8
	.word	.LANCHOR1
	.word	.LC9
	.word	.LC10
	.word	.LC16
	.word	.LC11
	.word	.LC12
	.word	.LC13
	.word	.LC14
	.word	.LC15
	.fnend
	.size	mpp_trie_add_info, .-mpp_trie_add_info
	.align	1
	.global	mpp_trie_get_info
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_trie_get_info, %function
mpp_trie_get_info:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r0, r1, r4, r5, r6, r7, r8, r9, r10, lr}
	.save {r4, r5, r6, r7, r8, r9, r10, lr}
	.pad #8
	mov	r6, r0
	ldr	r9, [r0, #20]
	mov	r0, r1
	mov	r7, r1
	bl	strlen
	mov	r10, r0
	cbz	r6, .L90
	cbnz	r7, .L91
.L90:
	str	r7, [sp]
	mov	r3, r6
	movs	r2, #254
	ldr	r1, .L118
	ldr	r0, .L118+4
	bl	printk
.L99:
	movs	r4, #0
	b	.L89
.L91:
	ldr	r3, .L118+8
	ldr	r2, [r3]
	mov	r8, r3
	tst	r2, #4
	beq	.L93
	strd	r7, r0, [sp]
	mov	r3, r6
	mov	r2, #258
	ldr	r1, .L118
	ldr	r0, .L118+12
	bl	printk
.L93:
	movs	r2, #0
	mov	r4, r9
	mov	r5, r2
.L94:
	cmp	r2, r10
	blt	.L96
.L95:
	ldr	r3, [r8]
	lsls	r3, r3, #29
	bpl	.L97
	ldr	r3, [r4, #4]
	mov	r2, #274
	ldr	r1, .L118
	ldr	r0, .L118+16
	strd	r4, r3, [sp]
	mov	r3, r5
	bl	printk
.L97:
	cmp	r5, #0
	beq	.L99
	cbz	r4, .L89
	ldr	r3, [r4, #4]
	cmp	r3, #0
	blt	.L99
	ldr	r2, [r6, #8]
	ldr	r4, [r2, r3, lsl #2]
.L89:
	mov	r0, r4
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, pc}
.L96:
	ldrb	r3, [r7, r2]	@ zero_extendqisi2
	lsrs	r1, r3, #4
	adds	r1, r1, #4
	ldrsh	r5, [r4, r1, lsl #1]
	cmp	r5, #0
	beq	.L95
	add	r5, r5, r5, lsl #2
	and	r3, r3, #15
	adds	r3, r3, #4
	add	r4, r9, r5, lsl #3
	ldrsh	r5, [r4, r3, lsl #1]
	cmp	r5, #0
	beq	.L95
	add	r4, r5, r5, lsl #2
	adds	r2, r2, #1
	add	r4, r9, r4, lsl #3
	b	.L94
.L119:
	.align	2
.L118:
	.word	.LANCHOR0+124
	.word	.LC17
	.word	.LANCHOR1
	.word	.LC18
	.word	.LC19
	.fnend
	.size	mpp_trie_get_info, .-mpp_trie_get_info
	.align	1
	.global	mpp_trie_get_node_count
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_trie_get_node_count, %function
mpp_trie_get_node_count:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbnz	r0, .L121
	mov	r2, #284
	ldr	r1, .L123
	ldr	r0, .L123+4
	bl	printk
	mov	r0, r4
.L120:
	pop	{r4, pc}
.L121:
	ldr	r0, [r0, #16]
	b	.L120
.L124:
	.align	2
.L123:
	.word	.LANCHOR0+142
	.word	.LC20
	.fnend
	.size	mpp_trie_get_node_count, .-mpp_trie_get_node_count
	.align	1
	.global	mpp_trie_get_info_count
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_trie_get_info_count, %function
mpp_trie_get_info_count:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	.save {r4, lr}
	mov	r4, r0
	cbnz	r0, .L126
	mov	r2, #294
	ldr	r1, .L128
	ldr	r0, .L128+4
	bl	printk
	mov	r0, r4
.L125:
	pop	{r4, pc}
.L126:
	ldr	r0, [r0, #4]
	b	.L125
.L129:
	.align	2
.L128:
	.word	.LANCHOR0+166
	.word	.LC20
	.fnend
	.size	mpp_trie_get_info_count, .-mpp_trie_get_info_count
	.global	mpp_trie_debug
	.section	.rodata
	.set	.LANCHOR0,. + 0
	.type	__FUNCTION__.7519, %object
	.size	__FUNCTION__.7519, 14
__FUNCTION__.7519:
	.ascii	"trie_get_node\000"
	.type	__func__.7520, %object
	.size	__func__.7520, 14
__func__.7520:
	.ascii	"trie_get_node\000"
	.type	__FUNCTION__.7528, %object
	.size	__FUNCTION__.7528, 14
__FUNCTION__.7528:
	.ascii	"mpp_trie_init\000"
	.type	__func__.7529, %object
	.size	__func__.7529, 14
__func__.7529:
	.ascii	"mpp_trie_init\000"
	.type	__func__.7535, %object
	.size	__func__.7535, 16
__func__.7535:
	.ascii	"mpp_trie_deinit\000"
	.type	__FUNCTION__.7536, %object
	.size	__FUNCTION__.7536, 16
__FUNCTION__.7536:
	.ascii	"mpp_trie_deinit\000"
	.type	__func__.7549, %object
	.size	__func__.7549, 18
__func__.7549:
	.ascii	"mpp_trie_add_info\000"
	.type	__FUNCTION__.7552, %object
	.size	__FUNCTION__.7552, 18
__FUNCTION__.7552:
	.ascii	"mpp_trie_add_info\000"
	.type	__func__.7570, %object
	.size	__func__.7570, 18
__func__.7570:
	.ascii	"mpp_trie_get_info\000"
	.type	__func__.7578, %object
	.size	__func__.7578, 24
__func__.7578:
	.ascii	"mpp_trie_get_node_count\000"
	.type	__func__.7583, %object
	.size	__func__.7583, 24
__func__.7583:
	.ascii	"mpp_trie_get_info_count\000"
	.bss
	.align	2
	.set	.LANCHOR1,. + 0
	.type	mpp_trie_debug, %object
	.size	mpp_trie_debug, 4
mpp_trie_debug:
	.space	4
	.section	.rodata.str1.1,"aMS",%progbits,1
.LC0:
	.ascii	"\0013%s:%d: failed to realloc new nodes %d\012\000"
.LC1:
	.ascii	"\0016%s:%d: trie %p enlarge node %p:%d -> %p:%d\012"
	.ascii	"\000"
.LC2:
	.ascii	"\0016%s:%d: get node %d\012\000"
.LC3:
	.ascii	"\0013%s:%d: invalid NULL input trie automation\012\000"
.LC4:
	.ascii	"\0013%s:%d: create trie impl failed\012\000"
.LC5:
	.ascii	"\0013%s:%d: create %d nodes failed\012\000"
.LC6:
	.ascii	"\0013%s:%d: failed to alloc %d storage\012\000"
.LC7:
	.ascii	"\0013%s:%d: invalid trie %p info %p\012\000"
.LC8:
	.ascii	"\0013%s:%d: failed to realloc new action %d\012\000"
.LC9:
	.ascii	"\0016%s:%d: trie %p enlarge info %p:%d -> %p:%d\012"
	.ascii	"\000"
.LC10:
	.ascii	"\0016%s:%d: trie %p add info %s len %d\012\000"
.LC11:
	.ascii	"\0016%s:%d: trie %p add %s at %2d char %c:%3d:%x:%x"
	.ascii	" node %d -> %d\012\000"
.LC12:
	.ascii	"\0016%s:%d: trie %p add %s at %2d char %c:%3d node "
	.ascii	"%d -> %d as new key0\012\000"
.LC13:
	.ascii	"\0016%s:%d: trie %p add %s at %2d char %c:%3d:%x:%x"
	.ascii	" node %d -> %d as key0\012\000"
.LC14:
	.ascii	"\0016%s:%d: trie %p add %s at %2d char %c:%3d node "
	.ascii	"%d -> %d as new child\012\000"
.LC15:
	.ascii	"\0016%s:%d: trie %p add %s at %2d char %c:%3d:%x:%x"
	.ascii	" node %d -> %d as key1\012\000"
.LC16:
	.ascii	"\0016%s:%d: trie %p add %d info %s at node %d pos %"
	.ascii	"d action %p done\012\000"
.LC17:
	.ascii	"\0013%s:%d: invalid trie %p name %p\012\000"
.LC18:
	.ascii	"\0016%s:%d: trie %p search %s len %2d start\012\000"
.LC19:
	.ascii	"\0016%s:%d: idx %d node %p info_id %d\012\000"
.LC20:
	.ascii	"\0013%s:%d: invalid NULL trie\012\000"
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
