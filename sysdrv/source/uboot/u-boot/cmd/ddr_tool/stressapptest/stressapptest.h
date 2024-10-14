/* Copyright 2006 Google Inc. All Rights Reserved. */
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

#ifndef __CMD_DDR_TOOL_STRESSAPPTEST_STRESSAPPTEST_H
#define __CMD_DDR_TOOL_STRESSAPPTEST_STRESSAPPTEST_H

struct stressapptest_params {
	ulong total_start_addr;
	ulong total_test_size_mb;
	/* total_test_size = page_size * page_num */
	ulong page_size_byte;
	u32 page_num;
	/* page_size = block_size * block_num */
	u32 block_size_byte;
	u32 block_num;

	u32 weight_count;

	u8 cpu_num;
} sat;

struct pat {
	const char *name;
	const unsigned int *data_array;
	/* mask = size - 1, So data_array[index & mask] is always valid. */
	const unsigned int mask;
	const unsigned int weight[4];	/* Weighted frequency of this pattern. */
};

struct adler_sum {
	u64 a1;
	u64 b1;
	u64 a2;
	u64 b2;
};

struct pattern {
	struct pat *pat;
	bool inv;
	u32 repeat;
	u32 weight;
	struct adler_sum adler_sum;
};

struct page {
	void *base_addr;
	struct pattern *pattern;
	bool valid;	/* 1: valid, 0: empty */
} *page_list;

extern void secondary_init(void);
extern void lock_byte_mutex(u32 *flag);
extern u32 unlock_byte_mutex(u32 *flag);

#endif /* __CMD_DDR_TOOL_STRESSAPPTEST_STRESSAPPTEST_H */
