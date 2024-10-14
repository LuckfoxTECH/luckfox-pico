/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * (C) Copyright 2019 Rockchip Electronics Co., Ltd
 */

#ifndef _SPL_AB_H_
#define _SPL_AB_H_

#include <spl.h>
#include <android_avb/libavb_ab.h>
#include <android_avb/avb_ab_flow.h>

#define AB_METADATA_OFFSET 4

/*
 * spl_get_current_slot
 *
 * @dev_desc: block description
 * @partition: partition name
 * @slot: A/B slot
 *
 * return: 0 success, others fail.
 */
int spl_get_current_slot(struct blk_desc *dev_desc, char *partition,
			 char *slot);

/*
 * spl_ab_append_part_slot
 *
 * @dev_desc: block description
 * @part_name: partition name
 * @new_name: append the slot suffix
 *
 * return: 0 success, others fail.
 */
int spl_ab_append_part_slot(struct blk_desc *dev_desc,
			    const char *part_name,
			    char *new_name);

/*
 * spl_ab_decrease_tries
 *
 * @dev_desc: block description
 *
 * return: 0 success, others fail.
 */
int spl_ab_decrease_tries(struct blk_desc *dev_desc);

/*
 * spl_ab_decrease_reset
 *
 * @dev_desc: block description
 *
 * return: 0 success, others fail.
 */
int spl_ab_decrease_reset(struct blk_desc *dev_desc);

/**
 * Append ab slot info to bootargs
 *
 * @param fdt		FDT address in memory
 * @param slot		slot info
 * @return 0 if ok, else error
 */
int spl_ab_bootargs_append_slot(void *fdt, char *slot);

 #endif
