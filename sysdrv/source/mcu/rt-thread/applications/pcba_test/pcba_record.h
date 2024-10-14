/*
 * @Copyright: Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * @License: SPDX-License-Identifier: Apache-2.0
 *
 * @Author: Chad.ma
 * @Date: 2020-05-21 20:19:41
 * @Company: Rockchip
 * @LastEditTime: 2020-06-29 11:24:17
 * @LastEditors: Do not edit
 * @Description:
 */

#ifndef __PCBA_REOCRD_H__
#define __PCBA_REOCRD_H__

int pcba_recorder_init(void);
void pcba_recorder_prepare(char *file_path);
void pcba_recorder_stop(void);
void pcba_recorder_deinit();

#endif // __PCBA_REOCRD_H__