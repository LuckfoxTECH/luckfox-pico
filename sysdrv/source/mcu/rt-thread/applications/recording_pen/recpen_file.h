/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-24     Jair Wu      First version
 *
 */

#ifndef __RECPEN_FILE_H__
#define __RECPEN_FILE_H__

#include "recpen.h"

#define ROOT_NO_MOUNTED    (0x1UL << 0)
#define SD0_NO_MOUNTED     (0x1UL << 1)

void get_app_info(void);
void save_app_info(void);
int rootfs_check(void);
uint32_t scan_audio(const char *path);
uint32_t get_audio(const char *path, char *file_name, uint32_t index);
uint32_t remove_file(const char *path, char *file_name);
rt_err_t recpen_increase_file();
rt_err_t recpen_decrease_file();
rt_err_t recpen_next_file();
rt_err_t recpen_prev_file();

#endif