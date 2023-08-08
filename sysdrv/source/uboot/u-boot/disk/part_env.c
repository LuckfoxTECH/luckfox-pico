/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <malloc.h>

#ifdef HAVE_BLOCK_DEVICE
struct env_part {
	char name[PART_NAME_LEN];
	lbaint_t start;
	lbaint_t size;
	struct list_head node;
};

#define DEV_NUM(dev_desc)  (((dev_desc)->if_type << 8) + (dev_desc)->devnum)

/*
 * What's this?
 *
 * There maybe different storage media will use this partition driver,
 * e.g. rkand with SD card. So we need a flag info to recognize it
 * and rebuild partition table.
 */
static int dev_num = -1;
static LIST_HEAD(parts_head);

#if CONFIG_IS_ENABLED(ENVF)
extern char *envf_get_part_table(struct blk_desc *desc);
#endif

static unsigned long long memparse(const char *ptr, char **retptr)
{
	char *endptr;	/* local pointer to end of parsed string */
	unsigned long long ret = simple_strtoull(ptr, &endptr, 0);

	switch (*endptr) {
	case 'E':
	case 'e':
		ret <<= 10;
		/* fall through */
	case 'P':
	case 'p':
		ret <<= 10;
		/* fall through */
	case 'T':
	case 't':
		ret <<= 10;
		/* fall through */
	case 'G':
	case 'g':
		ret <<= 10;
		/* fall through */
	case 'M':
	case 'm':
		ret <<= 10;
		/* fall through */
	case 'K':
	case 'k':
		ret <<= 10;
		endptr++;
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}

static int env_init_parts(struct blk_desc *dev_desc, struct list_head *parts_head)
{
	struct env_part *part;
	lbaint_t size, start = 0;
	int len, offset = 0;
	char *next, *pend;
	char *parts_list = NULL;

#if CONFIG_IS_ENABLED(ENVF)
	parts_list = envf_get_part_table(dev_desc);
#else
	parts_list = ENV_PARTITIONS;
#endif
	if (!parts_list)
		return -EINVAL;

	next = strchr(parts_list, ':');
	INIT_LIST_HEAD(parts_head);
	while (next) {
		/* Skip ':' and ',' */
		next++;
		if (*next == '-') {
			size = (~0UL);
			next++;
		} else {
			size = (lbaint_t)memparse(next, &next);
		}
		if (*next == '@') {
			next++;
			start = (lbaint_t)memparse(next, &next);
		}
		next++;
		pend = strchr(next, ')');
		if (!pend)
			break;

		len = min_t(int, pend - next, PART_NAME_LEN);
		part = malloc(sizeof(*part));
		if (!part)
			break;
		part->start = (start + offset) / dev_desc->blksz;
		start += size;
		/* Last partition use all remain space */
		if (size == (~0UL))
			part->size = dev_desc->lba - part->start;
		else
			part->size = size / dev_desc->blksz;
		strncpy(part->name, next, len);
		part->name[len] = '\0';
		list_add_tail(&part->node, parts_head);
		next = strchr(next, ',');
	}

	dev_num = DEV_NUM(dev_desc);

	return 0;
}

void part_print_env(struct blk_desc *dev_desc)
{
	struct list_head *node;
	struct env_part *p;
	int i = 0;
	int ret;

	if (list_empty(&parts_head) || (dev_num != DEV_NUM(dev_desc))) {
		ret = env_init_parts(dev_desc, &parts_head);
		if (ret) {
			printf("%s: Invalid env parameter\n", __func__);
			return;
		}
	}

	printf("Part\tStart LBA\tSize\t\tName\n");
	list_for_each(node, &parts_head) {
		p = list_entry(node, struct env_part, node);
		printf("%3d\t0x%08lx\t0x%08lx\t%s\n", (i++ + 1),
		       p->start, p->size, p->name);
	}
}

static int part_get_info_env(struct blk_desc *dev_desc, int idx,
			     disk_partition_t *info)
{
	struct env_part *p = NULL;
	struct list_head *node;
	int part_num = 1;
	int ret = 0;

	if (idx < 1) {
		printf("%s: Invalid partition no.%d\n", __func__, idx);
		return -EINVAL;
	}

	if (list_empty(&parts_head) || (dev_num != DEV_NUM(dev_desc))) {
		ret = env_init_parts(dev_desc, &parts_head);
		if (ret) {
			printf("%s: Invalid env partition\n", __func__);
			return -1;
		}
	}

	list_for_each(node, &parts_head) {
		p = list_entry(node, struct env_part, node);
		if (idx == part_num)
			break;
		part_num++;
	}

	if (part_num < idx) {
		debug("%s: Invalid partition no.%d\n", __func__, idx);
		return -EINVAL;
	}

	info->start = p->start;
	info->size  = p->size;
	info->blksz = dev_desc->blksz;
	info->bootable = 0;

	sprintf((char *)info->name, "%s", p->name);
	strcpy((char *)info->type, "U-Boot");

	return 0;
}

static int part_test_env(struct blk_desc *dev_desc)
{
	return env_init_parts(dev_desc, &parts_head) ? -1 : 0;
}

/*
 * Add an 'a_b_' prefix so it comes before 'a_efi'.
 */
U_BOOT_PART_TYPE(a_b_env) = {
	.name		= "ENV",
	.part_type	= PART_TYPE_ENV,
	.max_entries	= ENV_ENTRY_NUMBERS,
	.get_info	= part_get_info_ptr(part_get_info_env),
	.print		= part_print_ptr(part_print_env),
	.test		= part_test_env,
};
#endif

