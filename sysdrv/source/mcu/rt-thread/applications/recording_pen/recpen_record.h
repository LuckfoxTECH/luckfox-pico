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

#ifndef __RECPEN_REOCRD_H__
#define __RECPEN_REOCRD_H__

int recpen_recorder_init(void);
void recpen_recorder_prepare(char *file_path);
void recpen_recorder_stop(void);
void recpen_recorder_deinit();

#endif // __RECPEN_REOCRD_H__
