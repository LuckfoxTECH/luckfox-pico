/*
 * Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-21 20:25:14
 * @Company: Rockchip
 * @LastEditTime: 2020-05-21 20:25:50
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __PCBA_PLAYER_H
#define __PCBA_PLAYER_H__

int pcba_player_init(void);
void pcba_player_prepare(char *file_path, int info_only);
uint32_t pcba_player_get_total_time(char *file_path);
void pcba_player_stop(void);
void pcba_player_deinit();

#endif // __PCBA_REOCRD_H__
