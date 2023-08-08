/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

/*
 * Example:
 *	ramoops: ramoops@110000 {
 *		compatible = "ramoops";
 *		reg = <0x0 0x110000 0x0 0xe0000>;
 *		boot-log-size = <0x8000>;
 *		boot-log-count = <0x1>
 *		console-size = <0x80000>;
 *		pmsg-size = <0x30000>;
 *		ftrace-size = <0x00000>;
 *		record-size = <0x14000>;
 *	};
 */

#include <common.h>
#include <asm/arch/rk_atags.h>

DECLARE_GLOBAL_DATA_PTR;

#define PERSISTENT_RAM_SIG		(0x43474244)
#define LOG_TYPE_MASK			(0x00000fff)

struct persistent_ram_buffer {
	u32    sig;
	u32    start;
	u32    size;
	u8     data[0];
};

enum log_type {
	LOG_TPL = 0,
	LOG_SPL,
	LOG_OPTEE,
	LOG_ATF,
	LOG_UBOOT,
	LOG_MAX = 12,
};

#if 0
/*
 * This function is to be called by tpl/ddr.
 *
 * u32 'conf' definition:
 *   [31:16] is pstore buffer base address (0x_[31:16]_0000), 64KB align.
 *   [15:12] is pstore buffer size [15:12]*4KB.
 *   [11:0] see 'enum log_type' per bit is for one stage, 1: enable, 0: disable.
 *      [0] tpl(ddr)
 *      [1] spl(miniloader)
 *      [2] optee(bl32)
 *      [3] atf(bl31)
 *      [4] U-Boot
 *      ...
 */
void pstore_atags_set_tag(u32 conf)
{
	struct tag_pstore t;
	u32 addr = conf & 0xffff0000;
	u32 size = conf & 0x0000f000;
	int i = 0;

	/* handle special: 0 is 64KB but not 0KB */
	if (size == 0)
		size = 0x10000;

	/* is enabled log type ? */
	conf &= LOG_TYPE_MASK;
	if (conf == 0)
		return;

	t.version = 0;
	for (i = 0; i < LOG_MAX; i++) {
		if (conf & (1 << (i))) {
			t.buf[i].addr = addr;
			t.buf[i].size = size;
		} else {
			t.buf[i].addr = 0x0;
			t.buf[i].size = 0x0;
		}
	}

	atags_set_tag(ATAG_PSTORE, &t);
}
#endif

int param_parse_pstore(void)
{
	struct persistent_ram_buffer *rb = NULL;
	u32 pstore_size = 0;
#if defined(CONFIG_ROCKCHIP_PRELOADER_ATAGS) && (CONFIG_PERSISTENT_RAM_ADDR == 0)
	struct tag *t;

	gd->pstore_addr = 0;
	gd->pstore_size = 0;

	t = atags_get_tag(ATAG_PSTORE);
	if (t) {
		gd->pstore_addr = t->u.pstore.buf[LOG_UBOOT].addr;
		gd->pstore_size = t->u.pstore.buf[LOG_UBOOT].size - sizeof(struct persistent_ram_buffer);
	}
#else
	gd->pstore_addr = CONFIG_PERSISTENT_RAM_ADDR;
	gd->pstore_size = CONFIG_PERSISTENT_RAM_SIZE - sizeof(struct persistent_ram_buffer);
#endif
	if (gd->pstore_addr) {
		rb = (struct persistent_ram_buffer *)gd->pstore_addr;
		pstore_size = gd->pstore_size;

		if (rb->sig != PERSISTENT_RAM_SIG) {
			rb->sig = PERSISTENT_RAM_SIG;
			rb->start = 0x0;
			rb->size = 0x0;
		}

		if (rb->size > pstore_size)
			rb->size = pstore_size;

		if (rb->start >= pstore_size)
			rb->start = 0;
	}

	return 0;
}

void putc_to_ram(const char c)
{
	struct persistent_ram_buffer *rb = (struct persistent_ram_buffer *)gd->pstore_addr;
	u32 pstore_size = gd->pstore_size;
	u8 *dst;

	if (!rb || pstore_size == 0)
		return;

	dst = rb->data + rb->start;
	*dst = c;

	if (rb->size < pstore_size)
		rb->size++;

	rb->start++;
	if (rb->start >= pstore_size)
		rb->start = 0;
}

void puts_to_ram(const char *str)
{
	while (*str) {
		putc_to_ram(*str++);
	}
}
