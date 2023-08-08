/*
 * Copyright 2014 Broadcom Corporation.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _FB_MMC_H_
#define _FB_MMC_H_
void fb_mmc_flash_write(const char *cmd, void *download_buffer,
			unsigned int download_bytes, char *response);
void fb_mmc_erase(const char *cmd, char *response);

lbaint_t fb_mmc_get_erase_grp_size(void);

#endif
