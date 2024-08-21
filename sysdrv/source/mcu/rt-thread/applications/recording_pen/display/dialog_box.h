/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-03-04     Jair Wu      First version
 *
 */

#ifndef __DIALOG_BOX_H__
#define __DIALOG_BOX_H__

#define DIALOG_BOX_NONE     0
#define DIALOG_BOX_EXIST    1
#define DIALOG_BOX_CONFIRM  2
#define DIALOG_BOX_CANCEL   3

typedef void (*dialog_callback)(int ret);

void dialog_box_init(const char *str, dialog_callback cb);
void dialog_box_deinit(void);
int dialog_box_key(size_t type, size_t value);

#endif
