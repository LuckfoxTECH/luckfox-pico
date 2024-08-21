/* SPDX-License-Identifier:     GPL-2.0+ */
/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd
 *
 */

#ifndef __USBPLUG_H
#define __USBPLUG_H

struct blk_desc *usbplug_blk_get_devnum_by_type(enum if_type if_type, int devnum);

#endif