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

#ifndef __RECPEN_PLAYER_H
#define __RECPEN_PLAYER_H__

int recpen_player_init(void);
void recpen_player_prepare(char *file_path, int info_only);
uint32_t recpen_player_get_total_time(char *file_path);
void recpen_player_stop(void);
void recpen_player_deinit();

#endif // __RECPEN_REOCRD_H__
