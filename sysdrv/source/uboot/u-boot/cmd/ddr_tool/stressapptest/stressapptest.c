// Copyright 2006 Google Inc. All Rights Reserved.
/* Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This is stressapptest for Rockchip platform in U-Boot, the design idea and
 * the patterns are from code.google.com/p/stressapptest.
 */

#include <common.h>
#include <amp.h>
#include <div64.h>
#include <malloc.h>
#include <asm/arch/rockchip_smccc.h>
#include "stressapptest.h"
#include "../ddr_tool_common.h"

#define __version__ "v1.3.0 20230713"

#if defined(CONFIG_ARM64)
/* Float operation in TOOLCHAIN_ARM32 will cause the compile error */
#define WARM_CPU
#endif

#define PAT_NUM			26
#define PATTERN_LIST_SIZE	(PAT_NUM * 2 * 4)

#define CPU_NUM_MAX		16

static u32 walking_1_data[] = {
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
	0x40000000, 0x20000000, 0x10000000, 0x08000000,
	0x04000000, 0x02000000, 0x01000000, 0x00800000,
	0x00400000, 0x00200000, 0x00100000, 0x00080000,
	0x00040000, 0x00020000, 0x00010000, 0x00008000,
	0x00004000, 0x00002000, 0x00001000, 0x00000800,
	0x00000400, 0x00000200, 0x00000100, 0x00000080,
	0x00000040, 0x00000020, 0x00000010, 0x00000008,
	0x00000004, 0x00000002, 0x00000001, 0x00000000
};

static struct pat walking_1 = {
	"walking_1",
	walking_1_data,
	ARRAY_SIZE(walking_1_data) - 1,	/* mask */
	{1, 1, 2, 1}	/* weight */
};

static u32 walking_1_x16_data[] =   {
	0x00020001, 0x00080004, 0x00200010, 0x00800040,
	0x02000100, 0x08000400, 0x20001000, 0x80004000,
	0x20004000, 0x08001000, 0x02000400, 0x00800100,
	0x00200040, 0x00080010, 0x00020004, 0x00000001
};

static struct pat walking_1_x16 = {
	"walking_1_x16",
	walking_1_x16_data,
	ARRAY_SIZE(walking_1_x16_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* Weight for choosing 32/64/128/256 bit wide of this pattern */
	/* Reuse for walking_0_x16, because of invert */
};

static u32 walking_1_x16_repeat_data[] =   {
	0x00010001, 0x00020002, 0x00040004, 0x00080008,
	0x00100010, 0x00200020, 0x00400040, 0x00800080,
	0x01000100, 0x02000200, 0x04000400, 0x08000800,
	0x10001000, 0x20002000, 0x40004000, 0x80008000,
	0x40004000, 0x20002000, 0x10001000, 0x08000800,
	0x04000400, 0x02000200, 0x01000100, 0x00800080,
	0x00400040, 0x00200020, 0x00100010, 0x00080008,
	0x00040004, 0x00020002, 0x00010001, 0x00000000
};

static struct pat walking_1_x16_repeat = {
	"walking_1_x16_repeat",
	walking_1_x16_repeat_data,
	ARRAY_SIZE(walking_1_x16_repeat_data) - 1,	/* mask */
	{2, 4, 2, 0}	/* Weight for choosing 32/64/128/256 bit wide of this pattern */
	/* Reuse for walking_0_x16_repeat, because of invert */
};

static u32 walking_inv_1_data[] = {
	0x00000001, 0xfffffffe, 0x00000002, 0xfffffffd,
	0x00000004, 0xfffffffb, 0x00000008, 0xfffffff7,
	0x00000010, 0xffffffef, 0x00000020, 0xffffffdf,
	0x00000040, 0xffffffbf, 0x00000080, 0xffffff7f,
	0x00000100, 0xfffffeff, 0x00000200, 0xfffffdff,
	0x00000400, 0xfffffbff, 0x00000800, 0xfffff7ff,
	0x00001000, 0xffffefff, 0x00002000, 0xffffdfff,
	0x00004000, 0xffffbfff, 0x00008000, 0xffff7fff,
	0x00010000, 0xfffeffff, 0x00020000, 0xfffdffff,
	0x00040000, 0xfffbffff, 0x00080000, 0xfff7ffff,
	0x00100000, 0xffefffff, 0x00200000, 0xffdfffff,
	0x00400000, 0xffbfffff, 0x00800000, 0xff7fffff,
	0x01000000, 0xfeffffff, 0x02000000, 0xfdffffff,
	0x04000000, 0xfbffffff, 0x08000000, 0xf7ffffff,
	0x10000000, 0xefffffff, 0x20000000, 0xdfffffff,
	0x40000000, 0xbfffffff, 0x80000000, 0x7fffffff,
	0x40000000, 0xbfffffff, 0x20000000, 0xdfffffff,
	0x10000000, 0xefffffff, 0x08000000, 0xf7ffffff,
	0x04000000, 0xfbffffff, 0x02000000, 0xfdffffff,
	0x01000000, 0xfeffffff, 0x00800000, 0xff7fffff,
	0x00400000, 0xffbfffff, 0x00200000, 0xffdfffff,
	0x00100000, 0xffefffff, 0x00080000, 0xfff7ffff,
	0x00040000, 0xfffbffff, 0x00020000, 0xfffdffff,
	0x00010000, 0xfffeffff, 0x00008000, 0xffff7fff,
	0x00004000, 0xffffbfff, 0x00002000, 0xffffdfff,
	0x00001000, 0xffffefff, 0x00000800, 0xfffff7ff,
	0x00000400, 0xfffffbff, 0x00000200, 0xfffffdff,
	0x00000100, 0xfffffeff, 0x00000080, 0xffffff7f,
	0x00000040, 0xffffffbf, 0x00000020, 0xffffffdf,
	0x00000010, 0xffffffef, 0x00000008, 0xfffffff7,
	0x00000004, 0xfffffffb, 0x00000002, 0xfffffffd,
	0x00000001, 0xfffffffe, 0x00000000, 0xffffffff
};

static struct pat walking_inv_1 = {
	"walking_inv_1",
	walking_inv_1_data,
	ARRAY_SIZE(walking_inv_1_data) - 1,	/* mask */
	{2, 2, 5, 5}	/* weight */
};

static u32 walking_inv_1_x16_data[] = {
	0xfffe0001, 0xfffd0002, 0xfffb0004, 0xfff70008,
	0xffef0010, 0xffdf0020, 0xffbf0040, 0xff7f0080,
	0xfeff0100, 0xfdff0200, 0xfbff0400, 0xf7ff0800,
	0xefff1000, 0xdfff2000, 0xbfff4000, 0x7fff8000,
	0xbfff4000, 0xdfff2000, 0xefff1000, 0xf7ff0800,
	0xfbff0400, 0xfdff0200, 0xfeff0100, 0xff7f0080,
	0xffbf0040, 0xffdf0020, 0xffef0010, 0xfff70008,
	0xfffb0004, 0xfffd0002, 0xfffe0001, 0xffff0000
};

static struct pat walking_inv_1_x16 = {
	"walking_inv_1_x16",
	walking_inv_1_x16_data,
	ARRAY_SIZE(walking_inv_1_x16_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* weight */
};

static u32 walking_inv_1_x16_repeat_data[] = {
	0x00010001, 0xfffefffe, 0x00020002, 0xfffdfffd,
	0x00040004, 0xfffbfffb, 0x00080008, 0xfff7fff7,
	0x00100010, 0xffefffef, 0x00200020, 0xffdfffdf,
	0x00400040, 0xffbfffbf, 0x00800080, 0xff7fff7f,
	0x01000100, 0xfefffeff, 0x02000200, 0xfdfffdff,
	0x04000400, 0xfbfffbff, 0x08000800, 0xf7fff7ff,
	0x10001000, 0xefffefff, 0x20002000, 0xdfffdfff,
	0x40004000, 0xbfffbfff, 0x80008000, 0x7fff7fff,
	0x40004000, 0xbfffbfff, 0x20002000, 0xdfffdfff,
	0x10001000, 0xefffefff, 0x08000800, 0xf7fff7ff,
	0x04000400, 0xfbfffbff, 0x02000200, 0xfdfffdff,
	0x01000100, 0xfefffeff, 0x00800080, 0xff7fff7f,
	0x00400040, 0xffbfffbf, 0x00200020, 0xffdfffdf,
	0x00100010, 0xffefffef, 0x00080008, 0xfff7fff7,
	0x00040004, 0xfffbfffb, 0x00020002, 0xfffdfffd,
	0x00010001, 0xfffefffe, 0x00000000, 0xffffffff
};

static struct pat walking_inv_1_x16_repeat = {
	"walking_inv_1_x16_repeat",
	walking_inv_1_x16_repeat_data,
	ARRAY_SIZE(walking_inv_1_x16_repeat_data) - 1,	/* mask */
	{2, 5, 5, 0}	/* weight */
};

static u32 walking_0_data[] = {
	0xfffffffe, 0xfffffffd, 0xfffffffb, 0xfffffff7,
	0xffffffef, 0xffffffdf, 0xffffffbf, 0xffffff7f,
	0xfffffeff, 0xfffffdff, 0xfffffbff, 0xfffff7ff,
	0xffffefff, 0xffffdfff, 0xffffbfff, 0xffff7fff,
	0xfffeffff, 0xfffdffff, 0xfffbffff, 0xfff7ffff,
	0xffefffff, 0xffdfffff, 0xffbfffff, 0xff7fffff,
	0xfeffffff, 0xfdffffff, 0xfbffffff, 0xf7ffffff,
	0xefffffff, 0xdfffffff, 0xbfffffff, 0x7fffffff,
	0xbfffffff, 0xdfffffff, 0xefffffff, 0xf7ffffff,
	0xfbffffff, 0xfdffffff, 0xfeffffff, 0xff7fffff,
	0xffbfffff, 0xffdfffff, 0xffefffff, 0xfff7ffff,
	0xfffbffff, 0xfffdffff, 0xfffeffff, 0xffff7fff,
	0xffffbfff, 0xffffdfff, 0xffffefff, 0xfffff7ff,
	0xfffffbff, 0xfffffdff, 0xfffffeff, 0xffffff7f,
	0xffffffbf, 0xffffffdf, 0xffffffef, 0xfffffff7,
	0xfffffffb, 0xfffffffd, 0xfffffffe, 0xffffffff
};

static struct pat walking_0 = {
	"walking_0",
	walking_0_data,
	ARRAY_SIZE(walking_0_data) - 1,	/* mask */
	{1, 1, 2, 1}	/* weight */
};

static u32 one_zero_data[] = {0x00000000, 0xffffffff};

static struct pat one_zero = {
	"one_zero",
	one_zero_data,
	ARRAY_SIZE(one_zero_data) - 1,	/* mask */
	{5, 5, 15, 5}	/* weight */
};

static unsigned int one_zero_x16_data[] = {0x0000ffff, 0x0000ffff};

static struct pat one_zero_x16 = {
	"one_zero_x16",
	one_zero_x16_data,
	ARRAY_SIZE(one_zero_x16_data) - 1,	/* mask */
	{5, 0, 0, 0}	/* weight */
};

static u32 just_0_data[] = {0x00000000, 0x00000000};

static struct pat just_0 = {
	"just_0",
	just_0_data,
	ARRAY_SIZE(just_0_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* weight */
};

static u32 just_1_data[] = {0xffffffff, 0xffffffff};

static struct pat just_1 = {
	"just_1",
	just_1_data,
	ARRAY_SIZE(just_1_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* weight */
};

static u32 just_5_data[] = {0x55555555, 0x55555555};

static struct pat just_5 = {
	"just_5",
	just_5_data,
	ARRAY_SIZE(just_5_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* weight */
};

static u32 just_a_data[] = {0xaaaaaaaa, 0xaaaaaaaa};

static struct pat just_a = {
	"just_a",
	just_a_data,
	ARRAY_SIZE(just_a_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* weight */
};

static u32 five_a_data[] = {0x55555555, 0xaaaaaaaa};

static struct pat five_a = {
	"five_a",
	five_a_data,
	ARRAY_SIZE(five_a_data) - 1,	/* mask */
	{1, 1, 1, 1}	/* weight */
};

static unsigned int five_a_x16_data[] = {0x5555aaaa, 0x5555aaaa};

static struct pat five_a_x16 = {
	"five_a_x16",
	five_a_x16_data,
	ARRAY_SIZE(five_a_x16_data) - 1,	/* mask */
	{1, 0, 0, 0}	/* weight */
};

static u32 five_a8_data[] = {
	0x5aa5a55a, 0xa55a5aa5, 0xa55a5aa5, 0x5aa5a55a
};

static struct pat five_a8 = {
	"five_a8",
	five_a8_data,
	ARRAY_SIZE(five_a8_data) - 1,	/* mask */
	{1, 1, 1, 1}	/* weight */
};

static u32 five_a8_x16_data[] = {0x5aa5a55a, 0xa55a5aa5};

static struct pat five_a8_x16 = {
	"five_a8_x16",
	five_a8_x16_data,
	ARRAY_SIZE(five_a8_x16_data) - 1,	/* mask */
	{1, 0, 0, 0}	/* weight */
};

static unsigned int five_a8_x16_repeat_data[] = {
	0x5aa55aa5, 0xa55aa55a, 0xa55aa55a, 0x5aa55aa5
};

static struct pat five_a8_x16_repeat = {
	"five_a8_x16_repeat",
	five_a8_x16_repeat_data,
	ARRAY_SIZE(five_a8_x16_repeat_data) - 1,	/* mask */
	{1, 1, 1, 0}	/* weight */
};

static u32 long_8b10b_data[] = {0x16161616, 0x16161616};

static struct pat long_8b10b = {
	"long_8b10b",
	long_8b10b_data,
	ARRAY_SIZE(long_8b10b_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* weight */
};

static u32 short_8b10b_data[] = {0xb5b5b5b5, 0xb5b5b5b5};

static struct pat short_8b10b = {
	"short_8b10b",
	short_8b10b_data,
	ARRAY_SIZE(short_8b10b_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* weight */
};

static u32 checker_8b10b_data[] = {0xb5b5b5b5, 0x4a4a4a4a};

static struct pat checker_8b10b = {
	"checker_8b10b",
	checker_8b10b_data,
	ARRAY_SIZE(checker_8b10b_data) - 1,	/* mask */
	{1, 0, 1, 1}	/* weight */
};

static u32 checker_8b10b_x16_data[] = {0xb5b54a4a, 0xb5b54a4a};

static struct pat checker_8b10b_x16 = {
	"checker_8b10b_x16",
	checker_8b10b_x16_data,
	ARRAY_SIZE(checker_8b10b_x16_data) - 1,	/* mask */
	{1, 0, 0, 0}	/* weight */
};

static u32 five_7_data[] = {0x55555557, 0x55575555};

static struct pat five_7 = {
	"five_7",
	five_7_data,
	ARRAY_SIZE(five_7_data) - 1,	/* mask */
	{0, 2, 0, 0}	/* weight */
};

static u32 five_7_x16_data[] = {0x55575557, 0x57555755};

static struct pat five_7_x16 = {
	"five_7_x16",
	five_7_x16_data,
	ARRAY_SIZE(five_7_x16_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* weight */
};

static u32 zero2_fd_data[] = {0x00020002, 0xfffdfffd};

static struct pat zero2_fd = {
	"zero2_fd",
	zero2_fd_data,
	ARRAY_SIZE(zero2_fd_data) - 1,	/* mask */
	{0, 2, 0, 0}	/* weight */
};

static u32 zero2_fd_x16_data[] = {0x02020202, 0xfdfdfdfd};

static struct pat zero2_fd_x16 = {
	"zero2_fd_x16",
	zero2_fd_x16_data,
	ARRAY_SIZE(zero2_fd_x16_data) - 1,	/* mask */
	{2, 0, 0, 0}	/* weight */
};

static struct pat *pat_array[] = {
	&walking_1,
	&walking_1_x16,
	&walking_1_x16_repeat,
	&walking_inv_1,
	&walking_inv_1_x16,
	&walking_inv_1_x16_repeat,
	&walking_0,
	&one_zero,
	&one_zero_x16,
	&just_0,
	&just_1,
	&just_5,
	&just_a,
	&five_a,
	&five_a_x16,
	&five_a8,
	&five_a8_x16,
	&five_a8_x16_repeat,
	&long_8b10b,
	&short_8b10b,
	&checker_8b10b,
	&checker_8b10b_x16,
	&five_7,
	&five_7_x16,
	&zero2_fd,
	&zero2_fd_x16
};

static u32 cpu_copy_err[CPU_NUM_MAX];
static u32 cpu_inv_err[CPU_NUM_MAX];

static u64 start_time_us;
static u64 test_time_us;

static bool cpu_init_finish[CPU_NUM_MAX];
static bool cpu_test_finish[CPU_NUM_MAX];
static bool pattern_page_init_finish;

#if (CPU_NUM_MAX > 1)
static ulong test_count = 0;
static ulong __gd;	/* set r9/x18 of secondary cpu to gd addr */
#endif
ulong __sp;		/* set sp of secondary cpu */

u32 print_mutex;	/* 0: unlock, 1: lock */

static u64 get_time_us(void)
{
	return lldiv(get_ticks(), CONFIG_SYS_HZ_CLOCK / (CONFIG_SYS_HZ * 1000));
}

static u64 run_time_us(void)
{
	return get_time_us() - start_time_us;
}

static void print_time_stamp(void)
{
	u64 time_us;

	time_us = run_time_us();

	printf("[%5d.%06d] ", (u32)(time_us / 1000000), (u32)(time_us % 1000000));
}

static u32 pattern_get(struct pattern *pattern, u32 offset)
{
	u32 ret;

	ret = pattern->pat->data_array[(offset >> pattern->repeat) &
				       pattern->pat->mask];

	return pattern->inv ? ~ret : ret;
}

static void pattern_adler_sum_calc(struct pattern *pattern,
				   struct stressapptest_params *sat)
{
	int i = 0;
	u64 a1 = 1;
	u64 b1 = 0;
	u64 a2 = 1;
	u64 b2 = 0;

	while (i < sat->block_size_byte / sizeof(u32)) {
		a1 += (u64)pattern_get(pattern, i++);
		b1 += a1;
		a1 += pattern_get(pattern, i++);
		b1 += a1;

		a2 += (u64)pattern_get(pattern, i++);
		b2 += a2;
		a2 += pattern_get(pattern, i++);
		b2 += a2;
	}

	pattern->adler_sum.a1 = a1;
	pattern->adler_sum.b1 = b1;
	pattern->adler_sum.a2 = a2;
	pattern->adler_sum.b2 = b2;
}

static void pattern_list_init(struct pattern *pattern_list,
			      struct stressapptest_params *sat)
{
	u32 weight_count = 0;
	int k = 0;

	for (int i = 0; i < PAT_NUM; i++) {
		for (int j = 0; j < 8; j++) {
			pattern_list[k].pat = pat_array[i];
			pattern_list[k].inv = j % 2;
			pattern_list[k].repeat = j / 2;
			pattern_list[k].weight = pattern_list[k].pat->weight[j / 2];
			pattern_adler_sum_calc(&pattern_list[k], sat);
			weight_count += pattern_list[k].weight;
			k++;
		}
	}

	sat->weight_count = weight_count;
}

static u32 get_max_page_num(ulong page_size_byte)
{
	ulong start_adr[CONFIG_NR_DRAM_BANKS], length[CONFIG_NR_DRAM_BANKS];
	u32 page_num = 0;

	get_print_available_addr(start_adr, length, 0);

	page_num = 0;
	for (int i = 0; i < ARRAY_SIZE(start_adr) || i < ARRAY_SIZE(length); i++) {
		if ((start_adr[i] == 0 && length[i] == 0))
			break;
		page_num += (u32)(length[i] / page_size_byte);
	}

	return page_num;
}

static int get_page_addr(struct page *page_list,
			 struct stressapptest_params *sat)
{
	ulong start_adr[CONFIG_NR_DRAM_BANKS], length[CONFIG_NR_DRAM_BANKS];
	ulong used_length;
	u32 page = 0;

	get_print_available_addr(start_adr, length, 0);

	printf("Address for test:\n	Start         End         Length\n");
	for (int i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		if ((start_adr[i] == 0 && length[i] == 0) || page >= sat->page_num)
			break;
		if (start_adr[i] + length[i] < sat->total_start_addr)
			continue;
		if (start_adr[i] < sat->total_start_addr) {
			length[i] -= sat->total_start_addr - start_adr[i];
			start_adr[i] = sat->total_start_addr;
		}

		used_length = 0;
		while (page < sat->page_num &&
		       length[i] >= used_length + sat->page_size_byte) {
			page_list[page].base_addr = (void *)(start_adr[i] + used_length);
			used_length += sat->page_size_byte;
			page++;
		}
		printf("	0x%09lx - 0x%09lx 0x%09lx\n",
		       start_adr[i], start_adr[i] + used_length, used_length);
	}

	printf("page_num = %d, page_size = 0x%lx, total_test_size = 0x%lx\n",
	       page, sat->page_size_byte, sat->page_size_byte * page);

	if (sat->total_test_size_mb == 0) {
		/* No arg for total_test_size_mb, test all available space by default. */
		sat->page_num = page;
	} else if (page < sat->page_num || page < sat->cpu_num * 4) {
		printf("ERROR: Cannot get enough pages to test.\n");
		printf("Please decrease page_size or test_size\n");

		return -1;
	}

	return 0;
}

static void page_init_valid(struct page *page, struct pattern *pattern_list,
			    struct stressapptest_params *sat)
{
	int target;
	int i = 0;
	u64 *mem;

	target = (rand() % sat->weight_count) + 1;
	do {
		target -= pattern_list[i++].weight;
		if (target <= 0)
			break;
	} while (i < PATTERN_LIST_SIZE);
	page->pattern = &pattern_list[--i];
	page->valid = 1;

	mem = (u64 *)page->base_addr;
	for (i = 0; i < sat->page_size_byte / sizeof(u64); i++)
		mem[i] = (u64)pattern_get(page->pattern, i * 2) |
			 (u64)pattern_get(page->pattern, i * 2 + 1) << 32;
}

static void page_init_empty(struct page *page)
{
	page->valid = 0;
}

static void page_init(struct pattern *pattern_list,
		      struct stressapptest_params *sat)
{
	int i, cpu;
	u32 empty_page_num;

	for (cpu = 0; cpu < sat->cpu_num; cpu++) {
		empty_page_num = 0;
		for (i = cpu; i < sat->page_num; i += sat->cpu_num) {
			if (rand() % 5 < 3) {
				page_list[i].valid = 1;
			} else {
				page_list[i].valid = 0;
				empty_page_num++;
			}
		}
		while (empty_page_num >= sat->page_num / sat->cpu_num / 2 && i > 0) {
			i -= sat->cpu_num;
			if (page_list[i].valid == 0) {
				page_list[i].valid = 1;
				empty_page_num--;
			}
		}
		i = cpu;
		while (empty_page_num < 2 && i < sat->page_num) {
			if (page_list[i].valid == 1) {
				page_list[i].valid = 0;
				empty_page_num++;
			}
			i += sat->cpu_num;
		}
	}

	for (i = 0; i < sat->page_num; i++) {
		if (page_list[i].valid == 1)
			page_init_valid(&page_list[i], pattern_list, sat);
		else
			page_init_empty(&page_list[i]);
	}
	flush_dcache_all();
}

static u32 page_rand_pick(struct page *page_list, bool valid,
			  struct stressapptest_params *sat, u8 cpu_id)
{
	u32 pick;

	pick = rand() % sat->page_num;
	pick = pick / sat->cpu_num * sat->cpu_num + cpu_id;
	if (pick >= sat->page_num)
		pick = cpu_id;

	while (page_list[pick].valid != valid) {
		pick += sat->cpu_num;
		if (pick >= sat->page_num)
			pick = cpu_id;
	}

	return pick;
}

static u32 block_mis_search(void *dst_addr, struct pattern *src_pattern, char *item,
			    struct stressapptest_params *sat, u8 cpu_id)
{
	u32 *dst_mem;
	u32 read, reread, expected;
	u32 err = 0;
	u32 *print_addr;
	int i, j;

	dst_mem = (u32 *)dst_addr;

	for (i = 0; i < sat->block_size_byte / sizeof(u32); i++) {
		read = dst_mem[i];
		expected = pattern_get(src_pattern, i);

		if (read != expected) {
			flush_dcache_range((ulong)&dst_mem[i], (ulong)&dst_mem[i + 1]);
			reread = dst_mem[i];

			lock_byte_mutex(&print_mutex);

			print_time_stamp();
			printf("%s Hardware Error: miscompare on CPU %d at 0x%lx:\n",
			       item, cpu_id, (ulong)&dst_mem[i]);
			printf("	read:    0x%08x\n", read);
			printf("	reread:  0x%08x(reread^read:0x%08x)\n",
			       reread, reread ^ read);
			printf("	expected:0x%08x(expected^read:0x%08x)\n",
			       expected, expected ^ read);
			printf("	\'%s%s%d\'", src_pattern->pat->name,
							  src_pattern->inv ? "~" : "",
							  32 << src_pattern->repeat);
			if (reread == expected)
				printf(" read error");
			printf("\n");

			/* Dump data around the error address */
			print_addr = &dst_mem[i] - 64;
			for (j = 0; j < 128; j += 8)
				printf("  [0x%010lx] 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x\n",
				       (ulong)(print_addr + j),
				       *(print_addr + j), *(print_addr + j + 1),
				       *(print_addr + j + 2), *(print_addr + j + 3),
				       *(print_addr + j + 4), *(print_addr + j + 5),
				       *(print_addr + j + 6), *(print_addr + j + 7));

			unlock_byte_mutex(&print_mutex);

			/* fix the error */
			dst_mem[i] = expected;
			err++;
			flush_dcache_range((ulong)&dst_mem[i], (ulong)&dst_mem[i + 1]);
		}
	}

	if (err == 0) {
		lock_byte_mutex(&print_mutex);
		printf("%s ERROR detected but cannot find mismatch data (maybe read error).\n", item);
		unlock_byte_mutex(&print_mutex);
	}

	return err;
}

static u32 block_inv_check(void *dst_addr, struct pattern *src_pattern,
			   struct stressapptest_params *sat, u8 cpu_id)
{
	u32 *dst_mem;
	u32 err = 0;
	int i = 0;
#if defined(WARM_CPU)
	double a, b, c, d;
#endif

	struct adler_sum adler_sum = {
		1, 0, 1, 0
	};

	dst_mem = (u32 *)dst_addr;

#if defined(WARM_CPU)
	a = 2.0 * dst_mem[0];
	b = 5.0 * dst_mem[0];
	c = 7.0 * dst_mem[0];
	d = 9.0 * dst_mem[0];
#endif

	while (i < sat->block_size_byte / sizeof(u32)) {
		adler_sum.a1 += dst_mem[i++];
		adler_sum.b1 += adler_sum.a1;
		adler_sum.a1 += dst_mem[i++];
		adler_sum.b1 += adler_sum.a1;

#if defined(WARM_CPU)
		a = a * b;
		b = b + c;
#endif

		adler_sum.a2 += dst_mem[i++];
		adler_sum.b2 += adler_sum.a2;
		adler_sum.a2 += dst_mem[i++];
		adler_sum.b2 += adler_sum.a2;
#if defined(WARM_CPU)
		c = c * d;
		d = d + d;
#endif
	}

#if defined(WARM_CPU)
	d = a + b + c + d;
	if (d == 1.0)
		/* Reference the result so that it can't be discarded by the compiler. */
		printf("This will probably never happen.\n");
#endif

	if (adler_sum.a1 != src_pattern->adler_sum.a1 ||
	    adler_sum.b1 != src_pattern->adler_sum.b1 ||
	    adler_sum.a2 != src_pattern->adler_sum.a2 ||
	    adler_sum.b2 != src_pattern->adler_sum.b2)
		err = block_mis_search(dst_addr, src_pattern, "Inv", sat, cpu_id);

	return err;
}

static void page_inv_up(void *src_addr, struct stressapptest_params *sat)
{
	void *dst_addr = src_addr;
	uint data;
	uint *dst_mem;

	for (int i = 0; i < sat->block_num; i++) {
		dst_mem = (uint *)dst_addr;
		for (int j = 0; j < sat->block_size_byte / sizeof(uint); j += 32) {
			for (int k = j; k < j + 32; k++) {
				data = dst_mem[k];
				dst_mem[k] = ~data;
			}
			flush_dcache_range((ulong)&dst_mem[j], (ulong)&dst_mem[j + 1]);
 		}
		dst_addr += sat->block_size_byte;
	}
}

static void page_inv_down(void *src_addr, struct stressapptest_params *sat)
{
	void *dst_addr = src_addr;
	uint data;
	uint *dst_mem;

	dst_addr += sat->block_size_byte * (sat->block_num - 1);

	for (int i = sat->block_num - 1; i >= 0; i--) {
		dst_mem = (uint *)dst_addr;
		for (int j = sat->block_size_byte / sizeof(uint) - 32; j >= 0; j -= 32) {
			for (int k = j + 31; k >= j; k--) {
				data = dst_mem[k];
				dst_mem[k] = ~data;
			}
			flush_dcache_range((ulong)&dst_mem[j], (ulong)&dst_mem[j + 1]);
 		}
		dst_addr -= sat->block_size_byte;
	}
}

static u32 page_inv(struct stressapptest_params *sat, u8 cpu_id)
{
	u32 src;
	void *dst_block_addr;
	u32 err = 0;

	src = page_rand_pick(page_list, 1, sat, cpu_id);	/* pick a valid page */
	dst_block_addr = page_list[src].base_addr;

	for (int i = 0; i < 4; i++) {
		if (rand() % 2 == 0)
			page_inv_up(page_list[src].base_addr, sat);
		else
			page_inv_down(page_list[src].base_addr, sat);
	}

	for (int i = 0; i < sat->block_num; i++) {
		err += block_inv_check(dst_block_addr, page_list[src].pattern, sat, cpu_id);
		dst_block_addr += sat->block_size_byte;
	}

	return err;
}

static u32 block_copy_check(void *dst_addr, struct adler_sum *adler_sum,
			    struct pattern *src_pattern, struct stressapptest_params *sat,
			    u8 cpu_id)
{
	u32 err = 0;

	if (adler_sum->a1 != src_pattern->adler_sum.a1 ||
	    adler_sum->b1 != src_pattern->adler_sum.b1 ||
	    adler_sum->a2 != src_pattern->adler_sum.a2 ||
	    adler_sum->b2 != src_pattern->adler_sum.b2)
		err = block_mis_search(dst_addr, src_pattern, "Copy", sat, cpu_id);

	return err;
}

static u32 block_copy(void *dst_addr, void *src_addr,
		      struct pattern *src_pattern,
		      struct stressapptest_params *sat, u8 cpu_id)
{
	u64 *dst_mem;
	u64 *src_mem;
	u64 data;
	int i = 0;
#if defined(WARM_CPU)
	double a, b, c, d;
#endif

	struct adler_sum adler_sum = {
		1, 0, 1, 0
	};

	dst_mem = (u64 *)dst_addr;
	src_mem = (u64 *)src_addr;

#if defined(WARM_CPU)
	a = 2.0 * src_mem[0];
	b = 5.0 * src_mem[0];
	c = 7.0 * src_mem[0];
	d = 9.0 * src_mem[0];
#endif

	while (i < sat->block_size_byte / sizeof(u64)) {
		data = src_mem[i];
		adler_sum.a1 += data & 0xffffffff;
		adler_sum.b1 += adler_sum.a1;
		adler_sum.a1 += data >> 32;
		adler_sum.b1 += adler_sum.a1;
		dst_mem[i] = data;
		i++;

#if defined(WARM_CPU)
		a = a * b;
		b = b + c;
#endif

		data = src_mem[i];
		adler_sum.a2 += data & 0xffffffff;
		adler_sum.b2 += adler_sum.a2;
		adler_sum.a2 += data >> 32;
		adler_sum.b2 += adler_sum.a2;
		dst_mem[i] = data;
		i++;

#if defined(WARM_CPU)
		c = c * d;
		d = d + d;
#endif
	}

#if defined(WARM_CPU)
	d = a + b + c + d;
	if (d == 1.0)
		/* Reference the result so that it can't be discarded by the compiler. */
		printf("This will probably never happen.\n");
#endif

	return block_copy_check(dst_addr, &adler_sum, src_pattern, sat, cpu_id);
}

static u32 page_copy(struct stressapptest_params *sat, u8 cpu_id)
{
	u32 dst;
	u32 src;
	void *dst_block_addr;
	void *src_block_addr;
	u32 err = 0;

	dst = page_rand_pick(page_list, 0, sat, cpu_id);	/* pick a empty page */
	dst_block_addr = page_list[dst].base_addr;
	src = page_rand_pick(page_list, 1, sat, cpu_id);	/* pick a valid page */
	src_block_addr = page_list[src].base_addr;

	for (int i = 0; i < sat->block_num; i++) {
		err += block_copy(dst_block_addr, src_block_addr,
				  page_list[src].pattern, sat, cpu_id);
		dst_block_addr += sat->block_size_byte;
		src_block_addr += sat->block_size_byte;
	}

	page_list[dst].pattern = page_list[src].pattern;
	page_list[dst].valid = 1;
	page_list[src].valid = 0;
	flush_dcache_range((ulong)&page_list[src], (ulong)&page_list[src + 1]);
	flush_dcache_range((ulong)&page_list[dst], (ulong)&page_list[dst + 1]);

	return err;
}

void secondary_main(void)
{
#if (CPU_NUM_MAX > 1)
	u8 cpu_id;
	ulong test = 0;

#ifndef CONFIG_ARM64
	asm volatile("mov r9, %0" : : "r" (__gd));	/* set r9 to gd addr */
#else
	asm volatile("mov x18, %0" : : "r" (__gd));	/* set x18 to gd addr */
#endif
	dcache_enable();
	icache_enable();

	udelay(100);

	flush_dcache_all();

	cpu_id = sat.cpu_num;
	cpu_init_finish[cpu_id] = 1;
	printf("CPU%d start OK.\n", cpu_id);

	while (pattern_page_init_finish == 0) {
		udelay(100);
		flush_dcache_all();
	}

	while (1) {
		udelay(100);
		flush_dcache_all();
		while (test < test_count) {
			cpu_test_finish[cpu_id] = 0;
			flush_dcache_all();
			while (run_time_us() < test_time_us) {
				if (rand() % 2 == 0)
					cpu_copy_err[cpu_id] += page_copy(&sat, cpu_id);
				else
					cpu_inv_err[cpu_id] += page_inv(&sat, cpu_id);
			}
			test++;
			cpu_test_finish[cpu_id] = 1;
			flush_dcache_range((ulong)&cpu_test_finish[cpu_id],
					   (ulong)&cpu_test_finish[cpu_id + 1]);
			flush_dcache_range((ulong)&cpu_copy_err[cpu_id],
					   (ulong)&cpu_copy_err[cpu_id + 1]);
			flush_dcache_range((ulong)&cpu_inv_err[cpu_id],
					   (ulong)&cpu_inv_err[cpu_id + 1]);
		}
	}
#else
	return;
#endif
}

static int doing_stressapptest(void)
{
	int i;
	u32 pre_10s;
	u32 now_10s;

	struct pattern pattern_list[PATTERN_LIST_SIZE];
	void *page_info;

	u32 all_copy_err = 0;
	u32 all_inv_err = 0;
	u32 cpu_no_response_err = 0;

	int ret = CMD_RET_SUCCESS;

	for (i = 0; i < CPU_NUM_MAX; i++) {
		cpu_copy_err[i] = 0;
		cpu_inv_err[i] = 0;
		cpu_init_finish[i] = 0;
		cpu_test_finish[i] = 0;
	}
	pattern_page_init_finish = 0;
	print_mutex = 0;
	asm volatile("clrex");

#if (CPU_NUM_MAX > 1)
	if (test_count == 0) {
		__gd = (ulong)gd;
		asm volatile("mov %0, sp" : "=r" (__sp));
		printf("CPU0 sp is at 0x%lx now.\n", __sp);
		__sp &= ~(ulong)0xffff;
		for (sat.cpu_num = 1; sat.cpu_num < CPU_NUM_MAX; sat.cpu_num++) {
			__sp -= 0x10000;
			flush_dcache_all();
			if (psci_cpu_on(sat.cpu_num, (ulong)secondary_init) == 0) {
				mdelay(10);
				printf("Calling CPU%d, sp = 0x%lx\n", sat.cpu_num, __sp);
			} else {
				break;
			}
			while (cpu_init_finish[sat.cpu_num] == 0) {
				udelay(1000);
				flush_dcache_all();
			}
		}
	}
#else
	sat.cpu_num = 1;
#endif

	if (sat.total_test_size_mb == 0)
		sat.page_num = get_max_page_num(sat.page_size_byte);
	else
		sat.page_num = (sat.total_test_size_mb << 20) / sat.page_size_byte;
	sat.block_num = sat.page_size_byte / sat.block_size_byte;

	udelay(100);

	page_info = malloc(sizeof(struct page) * sat.page_num);
	if (page_info == 0) {
		printf("ERROR: StressAppTest fail to malloc.\n");
		printf("Please try increasing CONFIG_SYS_MALLOC_LEN in include/configs/rxxxxx_common.h.\n");
		ret = CMD_RET_FAILURE;
		goto out;
	}
	page_list = (struct page *)page_info;

	if (get_page_addr(page_list, &sat) < 0) {
		ret = CMD_RET_FAILURE;
		goto out;
	}

	pattern_list_init(pattern_list, &sat);
	page_init(pattern_list, &sat);

#if (CPU_NUM_MAX > 1)
	if (sat.cpu_num > 1) {
		pattern_page_init_finish = 1;
		test_count++;
		flush_dcache_all();
	}
#endif

	pre_10s = (u32)(run_time_us() / 1000000 / 10);
	lock_byte_mutex(&print_mutex);
	print_time_stamp();
	printf("Start StressAppTest in U-Boot:\n");
	unlock_byte_mutex(&print_mutex);

	while (run_time_us() < test_time_us) {
		if (rand() % 2 == 0)
			cpu_copy_err[0] += page_copy(&sat, 0);
		else
			cpu_inv_err[0] += page_inv(&sat, 0);

		/* Print every 10 seconds */
		now_10s = (u32)(run_time_us() / 1000000 / 10);
		if (now_10s > pre_10s) {
			pre_10s = now_10s;
			lock_byte_mutex(&print_mutex);
			print_time_stamp();
			printf("Seconds remaining: %d\n", (u32)(test_time_us / 1000000 - now_10s * 10));
			unlock_byte_mutex(&print_mutex);
		}
	}

#if (CPU_NUM_MAX > 1)
	for (i = 1; i < sat.cpu_num; i++) {
		while (cpu_test_finish[i] == 0) {
			if ((u32)(run_time_us() / 1000000 / 10) > pre_10s + 6) {
				/* wait for secondary CPU in 60s */
				lock_byte_mutex(&print_mutex);
				print_time_stamp();
				printf("ERROR: Cannot wait for CPU%d to finish!\n", i);
				unlock_byte_mutex(&print_mutex);
				cpu_no_response_err++;
				break;
			}
			mdelay(1);
			flush_dcache_range((ulong)&cpu_test_finish[i],
					   (ulong)&cpu_test_finish[i + 1]);
		}
	}
	flush_dcache_all();
#endif

	for (i = 0; i < sat.cpu_num; i++) {
		all_copy_err += cpu_copy_err[i];
		all_inv_err += cpu_inv_err[i];
	}
	print_time_stamp();
	printf("StressAppTest Result: ");
	if (all_copy_err == 0 && all_inv_err == 0 && cpu_no_response_err == 0)
		printf("Pass.\n");
	else
		printf("FAIL!\nStressAppTest detects %d copy errors, %d inv errors.\n",
		       all_copy_err, all_inv_err);

out:
	free(page_info);

	return ret;
}

static int do_stressapptest(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	ulong test_time_sec = 20;
	ulong page_size_kb = 1024;

	sat.total_test_size_mb = 0;
	sat.block_size_byte = 4096;
	sat.total_start_addr = 0x0;

	printf("StressAppTest in U-Boot, " __version__ "\n");

	if (argc > 1) {
		if (strict_strtoul(argv[1], 0, &test_time_sec) < 0)
			return CMD_RET_USAGE;
		if (test_time_sec < 1)
			test_time_sec = 20;
	}
	if (argc > 2) {
		if (strict_strtoul(argv[2], 0, &sat.total_test_size_mb) < 0)
			return CMD_RET_USAGE;
		if (sat.total_test_size_mb < 1)
			sat.total_test_size_mb = 0;
	}
	if (argc > 3) {
		if (strict_strtoul(argv[3], 0, &sat.total_start_addr) < 0)
			return CMD_RET_USAGE;
		if (sat.total_start_addr < 0x1)
			sat.total_start_addr = 0x0;
	}
	if (argc > 4) {
		if (strict_strtoul(argv[4], 0, &page_size_kb) < 0)
			return CMD_RET_USAGE;
		if (page_size_kb < 1)
			page_size_kb = 1024;
	}

	sat.page_size_byte = page_size_kb << 10;

	start_time_us = get_time_us();
	test_time_us = (u64)test_time_sec * 1000000;

	/* Change rand seed. If not do this, rand() would be same after boot.*/
	srand((unsigned int)(start_time_us & 0xffffffff));

	return doing_stressapptest();
}

U_BOOT_CMD(stressapptest,	5,	1,	do_stressapptest,
	   "StressAppTest for Rockchip\n",
	   "\narg1: test time in second, default value is 20s.\n"
	   "arg2: test size in MB, default value is all available space.\n"
	   "arg3: start addr for test.\n"
	   "arg4: test page size in kB, default value is 1024kB(1MB).\n"
	   "example:\n"
	   "	stressapptest: test for 20s, test size is all available space, each page size is 1MB.\n"
	   "	stressapptest 43200 64: test for 12h, test size is 64MB, each page size is 1MB (64 pages).\n"
	   "	stressapptest 86400 1024 0x80000000: test for 24h, test size is 1024MB, start addr for test is 0x80000000, each page size is 1MB (1024 pages).\n"
	   "	stressapptest 43200 16 0x40000000 512: test for 12h, test size is 16MB, start addr for test is 0x40000000, each page size is 512kB (32 pages).\n"
);
