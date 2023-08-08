/*
 * (C) Copyright 2020 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 * Author: Wenping Zhang <wenping.zhang@rock-chips.com>
 */

#ifndef RK_EBC_H
#define RK_EBC_H

#include "epdlut/epd_lut.h"

struct ebc_panel {
	u32 width;
	u32 height;
	u32 vir_width;
	u32 vir_height;
	u32 width_mm;
	u32 height_mm;

	u32 sdck;
	u32 lsl;
	u32 lbl;
	u32 ldl;
	u32 lel;
	u32 gdck_sta;
	u32 lgonl;
	u32 fsl;
	u32 fbl;
	u32 fdl;
	u32 fel;
	u32 panel_16bit;
	u32 panel_color;
	u32 mirror;
	u32 rearrange;
	u32 disp_pbuf;
	u32 disp_pbuf_size;
	u32 *lut_pbuf;
	u32 lut_pbuf_size;
	struct epd_lut_data lut_data;
	struct epd_lut_ops lut_ops;
};

struct rk_ebc_tcon_ops {
	int (*enable)(struct udevice *dev, struct ebc_panel *panel);
	int (*disable)(struct udevice *dev);
	int (*dsp_mode_set)(struct udevice *dev, int update_mode,
			    int display_mode, int three_win_mode,
			    int eink_mode);
	int (*image_addr_set)(struct udevice *dev, u32 pre_image_addr,
			      u32 cur_image_addr);
	int (*frame_addr_set)(struct udevice *dev, u32 frame_addr);
	int (*lut_data_set)(struct udevice *dev, unsigned int *lut_data,
			    int frame_count, int lut_32);
	int (*frame_start)(struct udevice *dev, int frame_total);
	int (*wait_for_last_frame_complete)(struct udevice *dev);
};

#define ebc_tcon_get_ops(dev)	((struct rk_ebc_tcon_ops *)(dev)->driver->ops)

/*
 *interface for ebc power control
 */
struct rk_ebc_pwr_ops {
	int (*power_on)(struct udevice *dev);
	int (*power_down)(struct udevice *dev);
	int (*temp_get)(struct udevice *dev, u32 *temp);
	int (*vcom_set)(struct udevice *dev, u32 vcom);
};

#define ebc_pwr_get_ops(dev)	((struct rk_ebc_pwr_ops *)(dev)->driver->ops)

//display mode define
#define DIRECT_MODE		0
#define LUT_MODE		1
#define THREE_WIN_MODE		1
#define EINK_MODE		1

#endif
