/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-26 11:29:33
 * @Company: Rockchip
 * @LastEditTime: 2020-05-26 17:45:01
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __PCBA_PUB_H__
#define __PCBA_PUB_H__

#define ROOT_NO_MOUNTED    (0x1UL << 0)
#define SD0_NO_MOUNTED     (0x1UL << 1)

int rootfs_check(void);
uint32_t check_audio_type(char *file_name);
uint32_t get_audio(const char *path, char *file_name, uint32_t index);
uint32_t scan_audio(const char *path);
#endif