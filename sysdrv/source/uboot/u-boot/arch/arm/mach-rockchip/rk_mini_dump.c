/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <linux/types.h>
#include <asm/io.h>
#include <rk_mini_dump.h>

/* don't modify it, it is behind pstore memory space */
#ifdef CONFIG_ROCKCHIP_MINIDUMP_SMEM_BASE
#define SMEM_BASE		CONFIG_ROCKCHIP_MINIDUMP_SMEM_BASE
#else
#define SMEM_BASE		0x1f0000
#endif

#ifdef CONFIG_ROCKCHIP_MINIDUMP_MAX_ELF_SIZE
#define MAX_ELF_SIZE		CONFIG_ROCKCHIP_MINIDUMP_MAX_ELF_SIZE
#else
#define MAX_ELF_SIZE		0x2000000
#endif

#ifdef CONFIG_ROCKCHIP_MINIDUMP_MAX_ENTRIES
#define MAX_NUM_ENTRIES		(CONFIG_ROCKCHIP_MINIDUMP_MAX_ENTRIES + 1)
#else
#define MAX_NUM_ENTRIES		129
#endif

/* Bootloader has 16 byte support, 4 bytes reserved for itself */
#define MAX_REGION_NAME_LENGTH	16
#define MAX_STRTBL_SIZE		(MAX_NUM_ENTRIES * MAX_REGION_NAME_LENGTH)

/**
 * md_table : Local Minidump toc holder
 * @num_regions : Number of regions requested
 * @md_ss_toc  : HLOS toc pointer
 * @md_gbl_toc : Global toc pointer
 * @md_regions : HLOS regions base pointer
 * @entry : array of HLOS regions requested
 */
struct md_table {
	u32			revision;
	u32                     num_regions;
	struct md_ss_toc	*md_ss_toc;
	struct md_global_toc	*md_gbl_toc;
	struct md_ss_region	*md_regions;
	struct md_region	entry[MAX_NUM_ENTRIES];
};

#define MAX_NUM_OF_SS		2

/**
 * md_ss_toc: Sub system SMEM Table of content
 * @md_ss_toc_init : SS toc init status
 * @md_ss_enable_status : if set to 1, Bootloader would dump this SS regions
 * @encryption_status: Encryption status for this subsystem
 * @encryption_required : Decides to encrypt the SS regions or not
 * @ss_region_count : Number of regions added in this SS toc
 * @md_ss_smem_regions_baseptr : regions base pointer of the Subsystem
 * @elf_header : base pointer of the minidump elf header
 * @minidump_table : base pointer of the minidump_table
 */
struct md_ss_toc {
	u32			md_ss_toc_init;
	u32			md_ss_enable_status;
	u32			encryption_status;
	u32			encryption_required;
	u32			ss_region_count;
	u64			md_ss_smem_regions_baseptr;
	u64			elf_header;
	u64			elf_size;
	u64			minidump_table;
};

/**
 * md_global_toc: Global Table of Content
 * @md_toc_init : Global Minidump init status
 * @md_revision : Minidump revision
 * @md_enable_status : Minidump enable status
 * @md_ss_toc : Array of subsystems toc
 */
struct md_global_toc {
	u32			md_toc_init;
	u32			md_revision;
	u32			md_enable_status;
	struct md_ss_toc	md_ss_toc[MAX_NUM_OF_SS];
};

/* Bootloader has 16 byte support, 4 bytes reserved for itself */
#define MAX_REGION_NAME_LENGTH	16

#define MD_REGION_VALID		('V' << 24 | 'A' << 16 | 'L' << 8 | 'I' << 0)
#define MD_REGION_INVALID	('I' << 24 | 'N' << 16 | 'V' << 8 | 'A' << 0)
#define MD_REGION_INIT		('I' << 24 | 'N' << 16 | 'I' << 8 | 'T' << 0)
#define MD_REGION_NOINIT	0

#define MD_SS_ENCR_REQ		(0 << 24 | 'Y' << 16 | 'E' << 8 | 'S' << 0)
#define MD_SS_ENCR_NOTREQ	(0 << 24 | 0 << 16 | 'N' << 8 | 'R' << 0)
#define MD_SS_ENCR_NONE		('N' << 24 | 'O' << 16 | 'N' << 8 | 'E' << 0)
#define MD_SS_ENCR_DONE		('D' << 24 | 'O' << 16 | 'N' << 8 | 'E' << 0)
#define MD_SS_ENCR_START	('S' << 24 | 'T' << 16 | 'R' << 8 | 'T' << 0)
#define MD_SS_ENABLED		('E' << 24 | 'N' << 16 | 'B' << 8 | 'L' << 0)
#define MD_SS_DISABLED		('D' << 24 | 'S' << 16 | 'B' << 8 | 'L' << 0)

#define EM_AARCH64	183	/* ARM 64 bit */

/**
 * md_ss_region - Minidump region
 * @name		: Name of the region to be dumped
 * @seq_num:		: Use to differentiate regions with same name.
 * @md_valid		: This entry to be dumped (if set to 1)
 * @region_base_address	: Physical address of region to be dumped
 * @region_size		: Size of the region
 */
struct md_ss_region {
	char	name[MAX_REGION_NAME_LENGTH];
	u32	seq_num;
	u32	md_valid;
	u64	region_base_address;
	u64	region_size;
};

#define NO_FAULT_TAG 0x55aa55aa
static u32 no_fault;
static struct md_table *minidump_table;

u32 md_no_fault_handler(struct pt_regs *pt_regs, unsigned int esr)
{
	if (no_fault == NO_FAULT_TAG) {
		no_fault = 0;
		return 1;
	}
	return 0;
}

#if defined(CONFIG_ROCKCHIP_RK3588)
static u32 md_is_ddr_addr(void *addr)
{
	/* peripheral address space */
	if (addr >= (void *)0xf0000000 && addr <= (void *)0x100000000)
		return 0;
	/* pcie address space */
	if (addr > (void *)0x800000000)
		return 0;
	return 1;
}
#else
static u32 md_is_ddr_addr(void *addr)
{
	return 1;
}
#endif

static u32 md_is_uboot_addr(void *addr)
{
	volatile u32 *p_no_fault = &no_fault;

	if(!md_is_ddr_addr(addr))
		return 0;

	*p_no_fault = NO_FAULT_TAG;
	readb(addr);
	return *p_no_fault;
}

struct md_region *md_get_region(char *name)
{
	struct md_region *mdr;
	int i, regno;

	if (!md_is_uboot_addr((void *)minidump_table))
		return NULL;

	regno = minidump_table->num_regions;
	for (i = 0; i < regno; i++) {
		mdr = &minidump_table->entry[i];
		if (!strcmp(mdr->name, name))
			return mdr;
	}
	return NULL;
}

#ifdef CONFIG_ARM64
static Elf64_Xword rk_dump_elf64_image_phdr(void *ram_image,
					    Elf64_Addr ehaddr, Elf64_Xword ehsize)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)ehaddr;
	Elf64_Phdr *phdr = NULL, *phdr_next = NULL;
	Elf64_Shdr *shdr = NULL, *shdr_next = NULL;
	unsigned int i = 0, error = 0, phdr_off = 0, strtbl_off = 0;
	unsigned int size = 0, elf_size = ehsize;

	if (!md_is_uboot_addr((void *)ehdr))
		return 0;

	if (!md_is_uboot_addr((void *)ram_image) ||
	    !md_is_uboot_addr((void *)ram_image + MAX_ELF_SIZE - 4))
		return 0;

	memcpy(ehdr->e_ident, ELFMAG, SELFMAG);
	ehdr->e_ident[EI_CLASS] = ELFCLASS64;
	ehdr->e_ident[EI_DATA] = ELFDATA2LSB;
	ehdr->e_ident[EI_VERSION] = EV_CURRENT;
	ehdr->e_ident[EI_OSABI] = ELFOSABI_NONE;

	if (ehdr->e_type != ET_CORE) {
		error++;
		ehdr->e_type = ET_CORE;
	}
	if (ehdr->e_machine != EM_AARCH64) {
		error++;
		ehdr->e_machine = EM_AARCH64;
	}
	if (ehdr->e_version != EV_CURRENT) {
		error++;
		ehdr->e_version = EV_CURRENT;
	}
	if (ehdr->e_ehsize != sizeof(*ehdr)) {
		error++;
		ehdr->e_ehsize = sizeof(*ehdr);
	}
	if (ehdr->e_phentsize != sizeof(*phdr)) {
		error++;
		ehdr->e_phentsize = sizeof(*phdr);
	}
	if (ehdr->e_shentsize != sizeof(*shdr)) {
		error++;
		ehdr->e_shentsize = sizeof(*shdr);
	}
	if (ehdr->e_shoff != sizeof(*ehdr)) {
		error++;
		ehdr->e_shoff = sizeof(*ehdr);
	}

	phdr_off = sizeof(*ehdr) + (sizeof(*shdr) * MAX_NUM_ENTRIES);

	if (ehdr->e_phoff != phdr_off) {
		error++;
		ehdr->e_phoff = phdr_off;
	}

	printf("Minidump header error:0x%x\n", error);
	/* If there are much error, maybe ehdr address is wrong */
	if (error > 6)
		return 0;

	ehdr->e_shstrndx = 1;
	phdr = (Elf64_Phdr *)(ehaddr + ehdr->e_phoff);
	shdr = (Elf64_Shdr *)(ehaddr + ehdr->e_shoff);

	shdr->sh_name = 0;
	shdr->sh_type = 0;
	shdr->sh_flags = 0;
	shdr->sh_addr = 0;
	shdr->sh_offset = 0;
	shdr->sh_size = 0;
	shdr->sh_link = 0;
	shdr->sh_info = 0;
	shdr->sh_addralign = 0;
	shdr->sh_entsize = 0;

	shdr++;
	if (shdr->sh_name >= MAX_STRTBL_SIZE)
		shdr->sh_name = 0;
	shdr->sh_type = SHT_STRTAB;
	shdr->sh_flags = 0;
	shdr->sh_addr = 0;
	shdr->sh_offset = phdr_off + (sizeof(*phdr) * MAX_NUM_ENTRIES);
	shdr->sh_size = MAX_STRTBL_SIZE;
	shdr->sh_link = 0;
	shdr->sh_info = 0;
	shdr->sh_addralign = 0;
	shdr->sh_entsize = 0;

	shdr++;
	/* 3rd section is for minidump_table VA, used by parsers */
	if (shdr->sh_name >= MAX_STRTBL_SIZE)
		shdr->sh_name = 0;
	shdr->sh_type = SHT_PROGBITS;
	shdr->sh_flags = 0;
	shdr->sh_offset = 0;
	shdr->sh_size = 0;
	shdr->sh_link = 0;
	shdr->sh_info = 0;
	shdr->sh_addralign = 0;
	shdr->sh_entsize = 0;

	shdr++;
	shdr->sh_flags = 0;
	shdr->sh_link = 0;
	shdr->sh_info = 0;
	shdr->sh_addralign = 0;
	shdr->sh_entsize = 0;

	strtbl_off = phdr_off + (sizeof(*phdr) * MAX_NUM_ENTRIES);
	strtbl_off += MAX_STRTBL_SIZE;

	if (phdr->p_offset != strtbl_off)
		phdr->p_offset = strtbl_off;
	if (shdr->sh_offset != strtbl_off)
		shdr->sh_offset = strtbl_off;

	phdr->p_filesz &= GENMASK(23, 0);	/* 16MB */
	phdr->p_memsz &= GENMASK(23, 0);	/* 16MB */
	shdr->sh_size &= GENMASK(23, 0);	/* 16MB */

	if (phdr->p_filesz == phdr->p_memsz) {
		size = phdr->p_filesz;
		shdr->sh_size = size;
	} else if (phdr->p_filesz == shdr->sh_size) {
		size = phdr->p_filesz;
		phdr->p_memsz = size;
	} else if (phdr->p_memsz == shdr->sh_size) {
		size = phdr->p_memsz;
		phdr->p_filesz = size;
	} else {
		printf("Minidump error first phdr p_filesz:0x%llx p_memsz:0x%llx sh_size:0x%llx\n",
		       phdr->p_filesz, phdr->p_memsz, shdr->sh_size);
		return 0;
	}

	phdr++;
	shdr++;
	phdr_next = phdr + 1;
	shdr_next = shdr + 1;

	memset(ram_image, 0x0, 0x18000);

	phdr->p_offset &= MAX_ELF_SIZE - 1;
	shdr->sh_offset &= MAX_ELF_SIZE - 1;
	elf_size &= MAX_ELF_SIZE - 1;
	if (phdr->p_offset == shdr->sh_offset) {
		elf_size = phdr->p_offset;
	} else if (phdr->p_offset == elf_size) {
		shdr->sh_offset = phdr->p_offset;
	} else if (elf_size == shdr->sh_offset) {
		phdr->p_offset = shdr->sh_offset;
	} else {
		printf("Minidump error phdr[1] p_offset:0x%llx sh_offset:0x%llx elf_size:0x%x\n",
		       phdr->p_offset, shdr->sh_offset, elf_size);
		return 0;
	}

	/* save phdr space */
	for (i = 1; i < MAX_NUM_ENTRIES; i++) {
		void *src = NULL;
		void *dst = NULL;

		if (phdr->p_vaddr == 0 || shdr->sh_addr == 0)
			break;

		phdr->p_offset &= MAX_ELF_SIZE - 1;
		shdr->sh_offset &= MAX_ELF_SIZE - 1;

		if (phdr->p_offset != elf_size)
			phdr->p_offset = elf_size;

		if (shdr->sh_offset != elf_size)
			shdr->sh_offset = elf_size;

		phdr->p_paddr &= GENMASK(34, 0);	/* 32GB */
		phdr->p_align &= GENMASK(34, 0);	/* 32GB */
		shdr->sh_info &= GENMASK(34, 0);	/* 32GB */

		if (phdr->p_paddr != phdr->p_align && phdr->p_align == shdr->sh_entsize)
			phdr->p_paddr = phdr->p_align;

		phdr->p_type &= 0xf;
		phdr->p_flags &= 0xf;
		phdr->p_filesz &= GENMASK(23, 0);	/* 16MB */
		phdr->p_memsz &= GENMASK(23, 0);	/* 16MB */
		phdr->p_align = 0;

		if (phdr->p_vaddr != shdr->sh_addr) {
			if (shdr->sh_addr == shdr->sh_addralign)
				phdr->p_vaddr = shdr->sh_addr;
			else if (phdr->p_vaddr == shdr->sh_addralign)
				shdr->sh_addr = phdr->p_vaddr;
			else
				printf("Minidump error phdr[%d] p_vaddr:0x%llx sh_addr:0x%llx sh_addralign:0x%llx\n",
				       i, phdr->p_vaddr, shdr->sh_addr, shdr->sh_addralign);
		}

		if (shdr->sh_name >= MAX_STRTBL_SIZE)
			shdr->sh_name = 0;
		shdr->sh_type = SHT_PROGBITS;
		shdr->sh_flags = SHF_WRITE;
		shdr->sh_size &= GENMASK(23, 0);	/* 16MB */
		shdr->sh_link = 0;
		shdr->sh_info = 0;
		shdr->sh_addralign = 0;
		shdr->sh_entsize = 0;

		if (phdr->p_filesz == phdr->p_memsz) {
			size = phdr->p_filesz;
			shdr->sh_size = size;
		} else if (phdr->p_filesz == shdr->sh_size) {
			size = phdr->p_filesz;
			phdr->p_memsz = size;
		} else if (phdr->p_memsz == shdr->sh_size) {
			size = phdr->p_memsz;
			phdr->p_filesz = size;
		} else {
			if ((phdr_next->p_offset == shdr_next->sh_offset) &&
			    (phdr_next->p_offset != 0)) {
				size = phdr_next->p_offset - phdr->p_offset;
				phdr->p_filesz = size;
				phdr->p_memsz = size;
				shdr->sh_size = size;
			} else {
				printf("Minidump error phdr[%d] p_filesz:0x%llx p_memsz:0x%llx sh_size:0x%llx",
				       i, phdr->p_filesz, phdr->p_memsz, shdr->sh_size);
				printf("p_offset:0x%llx sh_offset:0x%llx\n", phdr_next->p_offset,
				       shdr_next->sh_offset);
				return 0;
			}
		}

		elf_size += size;
		src = (void *)(Elf64_Addr)phdr->p_paddr;
		dst = ram_image + phdr->p_offset;

		if (size > MAX_ELF_SIZE / 2)
			goto donot_cpy;

		if (!md_is_uboot_addr(src) || !md_is_uboot_addr(src + size - 1)) {
			printf("Minidump error src 0x%p-0x%p\n", src, src + size - 1);
			goto donot_cpy;
		}
		if (!md_is_uboot_addr(dst) || !md_is_uboot_addr(dst + size - 1)) {
			printf("Minidump error dst 0x%p-0x%p\n", dst, dst + size - 1);
			goto donot_cpy;
		}
		if (size)
			memcpy(dst, src, size);
donot_cpy:
		phdr++;
		shdr++;
		phdr_next++;
		shdr_next++;
	}

	if (ehdr->e_phnum != i)
		ehdr->e_phnum = i;
	if ((ehdr->e_phnum + 3) != ehdr->e_shnum)
		ehdr->e_shnum = ehdr->e_phnum + 3;

	/* copy ehdr to ram image */
	memcpy(ram_image, (void *)ehdr, ehsize);
	flush_cache((unsigned long)ram_image, elf_size);
	printf("Minidump.elf 0x%x@0x%p\n", elf_size, ram_image);
	return elf_size;
}
#else
static Elf32_Word rk_dump_elf32_image_phdr(void *ram_image, Elf32_Addr ehaddr,
					   Elf32_Word ehsize)
{
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)ehaddr;
	Elf32_Phdr *phdr = (Elf32_Phdr *)(ehaddr + ehdr->e_phoff);
	Elf32_Word ram_image_size = 0;
	int i;

	/* copy ehdr to ram image */
	memcpy(ram_image, (void *)ehdr, ehsize);

	/* save phdr space */
	for (i = 0; i < ehdr->e_phnum; ++i) {
		void *src = (void *)(Elf32_Addr)phdr->p_paddr;
		void *dst = ram_image + phdr->p_offset;

		if (phdr->p_filesz)
			memcpy(dst, src, phdr->p_filesz);
		if (phdr->p_filesz != phdr->p_memsz)
			memset(dst + phdr->p_filesz, 0x00,
			       phdr->p_memsz - phdr->p_filesz);
		++phdr;
	}

	phdr--;
	ram_image_size = phdr->p_memsz + phdr->p_offset;
	printf("Minidump.elf 0x%llx@0x%p\n", ram_image_size, ram_image);
	return ram_image_size;
}
#endif

void rk_minidump_init(void)
{
	struct md_global_toc *mdg_toc = (struct md_global_toc *)SMEM_BASE;
	struct md_ss_toc *md_ss_toc = &mdg_toc->md_ss_toc[0];
	struct md_ss_region *mdreg;

	printf("Minidump: init...\n");
	mdg_toc->md_toc_init = 1;
	mdg_toc->md_revision = 1;
	mdg_toc->md_enable_status = 0;

	if (md_ss_toc->md_ss_enable_status == MD_SS_ENABLED) {
		/* linux would set it 1, so we set it 0 here */
		md_ss_toc->md_ss_enable_status = 0;
		flush_cache((unsigned long)md_ss_toc, 8);
		mdreg = (struct md_ss_region *)md_ss_toc->md_ss_smem_regions_baseptr;
		minidump_table = (struct md_table *)md_ss_toc->minidump_table;
#ifdef CONFIG_ARM64
		md_ss_toc->elf_size = rk_dump_elf64_image_phdr((void *)md_ss_toc->elf_header,
					 (Elf64_Addr)mdreg->region_base_address,
					 (Elf64_Xword)mdreg->region_size);
#else
		md_ss_toc->elf_size = rk_dump_elf32_image_phdr((void *)md_ss_toc->elf_header,
					 (Elf32_Addr)mdreg->region_base_address,
					 (Elf32_Word)mdreg->region_size);
#endif
	}
}

#ifdef CONFIG_ARM64
void rk_minidump_get_el64(void **ram_image_addr, Elf64_Xword *ram_image_size)
{
	struct md_global_toc *mdg_toc = (struct md_global_toc *)SMEM_BASE;
	struct md_ss_toc *md_ss_toc = &mdg_toc->md_ss_toc[0];

	*ram_image_addr = (void *)md_ss_toc->elf_header;
	*ram_image_size = md_ss_toc->elf_size;
}
#else
void rk_minidump_get_el32(void **ram_image_addr, Elf32_Word *ram_image_size)
{
	struct md_global_toc *mdg_toc = (struct md_global_toc *)SMEM_BASE;
	struct md_ss_toc *md_ss_toc = &mdg_toc->md_ss_toc[0];

	*ram_image_addr = (void *)md_ss_toc->elf_header;
	*ram_image_size = md_ss_toc->elf_size;
}
#endif
