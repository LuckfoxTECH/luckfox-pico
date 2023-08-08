/*
 * (C) Copyright 2021 rkparm Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/rk_atags.h>

struct rkram_part {
	char name[PART_NAME_LEN];
	ulong start;
	ulong size;
	struct list_head node;
};

static LIST_HEAD(parts_head);

static int rkram_part_init(struct blk_desc *dev_desc,
			   struct list_head *parts_head)
{
	struct rkram_part *part;
	struct tag *t;
	int i, len;
	u64 start;
	u64 size;

	if (!atags_is_available()) {
		debug("%s: No ATAGS\n", __func__);
		return -ENODATA;
	}

	t = atags_get_tag(ATAG_RAM_PARTITION);
	if (!t) {
		debug("%s: No ATAGS ramdisk partition\n", __func__);
		return -ENODATA;
	}

	INIT_LIST_HEAD(parts_head);

	for (i = 0; i < t->u.ram_part.count; i++) {
		part = malloc(sizeof(*part));
		if (!part) {
			printf("%s: No memory\n", __func__);
			break;
		}

		len = strlen(t->u.ram_part.part[i].name) + 1;
		memcpy((char *)&part->name,
		       (char *)&t->u.ram_part.part[i].name, len);
		start = t->u.ram_part.part[i].start;
		size = t->u.ram_part.part[i].size;

		if (!IS_ALIGNED(start, dev_desc->blksz)) {
			printf("%s: addr 0x%llx is not %ld align\n",
			       part->name, start, dev_desc->blksz);
			return -EINVAL;
		} else if (!IS_ALIGNED(size, dev_desc->blksz)) {
			printf("%s: size 0x%llx is not %ld align\n",
			       part->name, size, dev_desc->blksz);
			return -EINVAL;
		}

		/* bytes to blksz */
		part->start = start / dev_desc->blksz;
		part->size = size / dev_desc->blksz;
		list_add_tail(&part->node, parts_head);

		debug("%s: name=%s, start=0x%lx, size=0x%lx, blksz=0x%lx\n",
		      __func__, part->name, part->start,
		      part->size, dev_desc->blksz);
	}

	return 0;
}

static void part_print_rkram_part(struct blk_desc *dev_desc)
{
	struct list_head *node;
	struct rkram_part *p;
	int i = 0;

	if (list_empty(&parts_head)) {
		printf("No Partition Table\n");
		return;
	}

	printf("Part\tStart LBA\tSize\t\tName\n");
	list_for_each(node, &parts_head) {
		p = list_entry(node, struct rkram_part, node);
		printf("%3d\t0x%08lx\t0x%08lx\t%s\n", (i++ + 1),
		       p->start, p->size, p->name);
	}
}

static int part_get_info_rkram_part(struct blk_desc *dev_desc, int idx,
				    disk_partition_t *info)
{
	struct rkram_part *p = NULL;
	struct list_head *node;
	int part_num = 1;

	if (idx < 1) {
		printf("Invalid partition no.%d\n", idx);
		return -EINVAL;
	}

	if (list_empty(&parts_head))
		return -EINVAL;

	list_for_each(node, &parts_head) {
		p = list_entry(node, struct rkram_part, node);
		if (idx == part_num)
			break;
		part_num++;
	}

	if (part_num < idx) {
		debug("%s Invalid partition no.%d\n", __func__, idx);
		return -EINVAL;
	}

	info->start = p->start;
	info->size = p->size;
	info->blksz = dev_desc->blksz;

	sprintf((char *)info->name, "%s", p->name);
	strcpy((char *)info->type, "U-Boot");
	info->bootable = 0;

	return 0;
}

static int part_test_rkram_part(struct blk_desc *dev_desc)
{
	return rkram_part_init(dev_desc, &parts_head);
}

/*
 * Add an 'a_a_' prefix so it comes before 'dos' and after 'a_efi'
 * in the linker list. We need to check this first.
 */
U_BOOT_PART_TYPE(a_a_rkram_part) = {
	.name		= "RKRAM_PART",
	.part_type	= PART_TYPE_RKRAM,
	.max_entries	= RKRAM_ENTRY_NUMBERS,
	.get_info	= part_get_info_ptr(part_get_info_rkram_part),
	.print		= part_print_ptr(part_print_rkram_part),
	.test		= part_test_rkram_part,
};
