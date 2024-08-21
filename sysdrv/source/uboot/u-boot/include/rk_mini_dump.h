/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef RK_MINIDUMP_H
#define RK_MINIDUMP_H

#include <elf.h>

#define MD_MAX_NAME_LENGTH		16
/* md_region -  Minidump table entry
 * @name:	Entry name, Minidump will dump binary with this name.
 * @id:		Entry ID, used only for SDI dumps.
 * @virt_addr:  Address of the entry.
 * @phys_addr:	Physical address of the entry to dump.
 * @size:	Number of byte to dump from @address location
 *		it should be 4 byte aligned.
 */
struct md_region {
	char	name[MD_MAX_NAME_LENGTH];
	u32	id;
	u64	virt_addr;
	u64	phys_addr;
	u64	size;
};

void rk_minidump_init(void);

#ifdef CONFIG_ARM64
void rk_minidump_get_el64(void **ram_image_addr, Elf64_Xword *ram_image_size);
#else
void rk_minidump_get_el32(void **ram_image_addr, Elf32_Word *ram_image_size);
#endif

struct md_region *md_get_region(char *name);
u32 md_no_fault_handler(struct pt_regs *pt_regs, unsigned int esr);
#endif
