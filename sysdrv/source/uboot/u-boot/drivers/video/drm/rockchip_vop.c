/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <fdtdec.h>
#include <fdt_support.h>
#include <asm/unaligned.h>
#include <asm/io.h>
#include <linux/list.h>
#include <linux/media-bus-format.h>
#include <clk.h>
#include <asm/arch/clock.h>
#include <linux/err.h>
#include <dm/device.h>
#include <dm/read.h>
#include <syscon.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_vop.h"

static inline int us_to_vertical_line(struct drm_display_mode *mode, int us)
{
	return us * mode->clock / mode->htotal / 1000;
}

static inline void set_vop_mcu_rs(struct vop *vop, int v)
{
	if (dm_gpio_is_valid(&vop->mcu_rs_gpio))
		dm_gpio_set_value(&vop->mcu_rs_gpio, v);
	else
		VOP_CTRL_SET(vop, mcu_rs, v);
}

static int to_vop_csc_mode(int csc_mode)
{
	switch (csc_mode) {
	case V4L2_COLORSPACE_SMPTE170M:
		return CSC_BT601L;
	case V4L2_COLORSPACE_REC709:
	case V4L2_COLORSPACE_DEFAULT:
		return CSC_BT709L;
	case V4L2_COLORSPACE_JPEG:
		return CSC_BT601F;
	case V4L2_COLORSPACE_BT2020:
		return CSC_BT2020;
	default:
		return CSC_BT709L;
	}
}

static bool is_yuv_output(uint32_t bus_format)
{
	switch (bus_format) {
	case MEDIA_BUS_FMT_YUV8_1X24:
	case MEDIA_BUS_FMT_YUV10_1X30:
	case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
	case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
	case MEDIA_BUS_FMT_YUYV8_2X8:
	case MEDIA_BUS_FMT_YVYU8_2X8:
	case MEDIA_BUS_FMT_UYVY8_2X8:
	case MEDIA_BUS_FMT_VYUY8_2X8:
	case MEDIA_BUS_FMT_YUYV8_1X16:
	case MEDIA_BUS_FMT_YVYU8_1X16:
	case MEDIA_BUS_FMT_UYVY8_1X16:
	case MEDIA_BUS_FMT_VYUY8_1X16:
		return true;
	default:
		return false;
	}
}

static bool is_uv_swap(uint32_t bus_format, uint32_t output_mode)
{
	/*
	 * FIXME:
	 *
	 * There is no media type for YUV444 output,
	 * so when out_mode is AAAA or P888, assume output is YUV444 on
	 * yuv format.
	 *
	 * From H/W testing, YUV444 mode need a rb swap.
	 */
	if ((bus_format == MEDIA_BUS_FMT_YUV8_1X24 ||
	     bus_format == MEDIA_BUS_FMT_YUV10_1X30) &&
	    (output_mode == ROCKCHIP_OUT_MODE_AAAA ||
	     output_mode == ROCKCHIP_OUT_MODE_P888))
		return true;
	else
		return false;
}

static int rockchip_vop_init_gamma(struct vop *vop, struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	struct connector_state *conn_state = &state->conn_state;
	u32 *lut = conn_state->gamma.lut;
	fdt_size_t lut_size;
	int i, lut_len;
	u32 *lut_regs;

	if (!conn_state->gamma.lut)
		return 0;

	i = dev_read_stringlist_search(crtc_state->dev, "reg-names", "gamma_lut");
	if (i < 0) {
		printf("Warning: vop not support gamma\n");
		return 0;
	}
	lut_regs = (u32 *)dev_read_addr_size(crtc_state->dev, "reg", &lut_size);
	if (lut_regs == (u32 *)FDT_ADDR_T_NONE) {
		printf("failed to get gamma lut register\n");
		return 0;
	}
	lut_len = lut_size / 4;
	if (lut_len != 256 && lut_len != 1024) {
		printf("Warning: unsupport gamma lut table[%d]\n", lut_len);
		return 0;
	}

	if (conn_state->gamma.size != lut_len) {
		int size = conn_state->gamma.size;
		u32 j, r, g, b, color;

		for (i = 0; i < lut_len; i++) {
			j = i * size / lut_len;
			r = lut[j] / size / size * lut_len / size;
			g = lut[j] / size % size * lut_len / size;
			b = lut[j] % size * lut_len / size;
			color = r * lut_len * lut_len + g * lut_len + b;

			writel(color, lut_regs + (i << 2));
		}
	} else {
		for (i = 0; i < lut_len; i++)
			writel(lut[i], lut_regs + (i << 2));
	}

	VOP_CTRL_SET(vop, dsp_lut_en, 1);
	VOP_CTRL_SET(vop, update_gamma_lut, 1);

	return 0;
}

static void vop_post_config(struct display_state *state, struct vop *vop)
{
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	u16 vtotal = mode->crtc_vtotal;
	u16 hact_st = mode->crtc_htotal - mode->crtc_hsync_start;
	u16 vact_st = mode->crtc_vtotal - mode->crtc_vsync_start;
	u16 hdisplay = mode->crtc_hdisplay;
	u16 vdisplay = mode->crtc_vdisplay;
	u16 hsize = hdisplay * (conn_state->overscan.left_margin + conn_state->overscan.right_margin) / 200;
	u16 vsize = vdisplay * (conn_state->overscan.top_margin + conn_state->overscan.bottom_margin) / 200;
	u16 hact_end, vact_end;
	u32 val;

	if (mode->flags & DRM_MODE_FLAG_INTERLACE)
		vsize = round_down(vsize, 2);

	hact_st += hdisplay * (100 - conn_state->overscan.left_margin) / 200;
	hact_end = hact_st + hsize;
	val = hact_st << 16;
	val |= hact_end;

	VOP_CTRL_SET(vop, hpost_st_end, val);
	vact_st += vdisplay * (100 - conn_state->overscan.top_margin) / 200;
	vact_end = vact_st + vsize;
	val = vact_st << 16;
	val |= vact_end;
	VOP_CTRL_SET(vop, vpost_st_end, val);
	val = scl_cal_scale2(vdisplay, vsize) << 16;
	val |= scl_cal_scale2(hdisplay, hsize);
	VOP_CTRL_SET(vop, post_scl_factor, val);
#define POST_HORIZONTAL_SCALEDOWN_EN(x)		((x) << 0)
#define POST_VERTICAL_SCALEDOWN_EN(x)		((x) << 1)
	VOP_CTRL_SET(vop, post_scl_ctrl,
		     POST_HORIZONTAL_SCALEDOWN_EN(hdisplay != hsize) |
		     POST_VERTICAL_SCALEDOWN_EN(vdisplay != vsize));
	if (mode->flags & DRM_MODE_FLAG_INTERLACE) {
		u16 vact_st_f1 = vtotal + vact_st + 1;
		u16 vact_end_f1 = vact_st_f1 + vsize;

		val = vact_st_f1 << 16 | vact_end_f1;
		VOP_CTRL_SET(vop, vpost_st_end_f1, val);
	}
}

static void vop_mcu_mode(struct display_state *state, struct vop *vop)
{
	struct crtc_state *crtc_state = &state->crtc_state;

	VOP_CTRL_SET(vop, mcu_clk_sel, 1);
	VOP_CTRL_SET(vop, mcu_type, 1);

	VOP_CTRL_SET(vop, mcu_hold_mode, 1);
	VOP_CTRL_SET(vop, mcu_pix_total, crtc_state->mcu_timing.mcu_pix_total);
	VOP_CTRL_SET(vop, mcu_cs_pst, crtc_state->mcu_timing.mcu_cs_pst);
	VOP_CTRL_SET(vop, mcu_cs_pend, crtc_state->mcu_timing.mcu_cs_pend);
	VOP_CTRL_SET(vop, mcu_rw_pst, crtc_state->mcu_timing.mcu_rw_pst);
	VOP_CTRL_SET(vop, mcu_rw_pend, crtc_state->mcu_timing.mcu_rw_pend);
}

static int rockchip_vop_preinit(struct display_state *state)
{
	const struct vop_data *vop_data = state->crtc_state.crtc->data;

	state->crtc_state.max_output = vop_data->max_output;

	return 0;
}

static int rockchip_vop_init(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct vop_data *vop_data = crtc->data;
	struct vop *vop;
	u16 hsync_len = mode->crtc_hsync_end - mode->crtc_hsync_start;
	u16 hdisplay = mode->crtc_hdisplay;
	u16 htotal = mode->crtc_htotal;
	u16 hact_st = mode->crtc_htotal - mode->crtc_hsync_start;
	u16 hact_end = hact_st + hdisplay;
	u16 vdisplay = mode->crtc_vdisplay;
	u16 vtotal = mode->crtc_vtotal;
	u16 vsync_len = mode->crtc_vsync_end - mode->crtc_vsync_start;
	u16 vact_st = mode->crtc_vtotal - mode->crtc_vsync_start;
	u16 vact_end = vact_st + vdisplay;
	struct clk dclk;
	u32 val, act_end;
	int ret;
	bool yuv_overlay = false, post_r2y_en = false, post_y2r_en = false;
	u16 post_csc_mode;
	bool dclk_inv;

	vop = malloc(sizeof(*vop));
	if (!vop)
		return -ENOMEM;
	memset(vop, 0, sizeof(*vop));

	crtc_state->private = vop;
	vop->regs = dev_read_addr_ptr(crtc_state->dev);
	vop->regsbak = malloc(vop_data->reg_len);
	vop->win = vop_data->win;
	vop->win_offset = vop_data->win_offset;
	vop->ctrl = vop_data->ctrl;
	vop->grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	if (vop->grf <= 0)
		printf("%s: Get syscon grf failed (ret=%p)\n",
		      __func__, vop->grf);

	vop->grf_ctrl = vop_data->grf_ctrl;
	vop->line_flag = vop_data->line_flag;
	vop->csc_table = vop_data->csc_table;
	vop->win_csc = vop_data->win_csc;
	vop->version = vop_data->version;

	/* Process 'assigned-{clocks/clock-parents/clock-rates}' properties */
	ret = clk_set_defaults(crtc_state->dev);
	if (ret)
		debug("%s clk_set_defaults failed %d\n", __func__, ret);

	ret = clk_get_by_name(crtc_state->dev, "dclk_vop", &dclk);
	if (!ret)
		ret = clk_set_rate(&dclk, mode->clock * 1000);
	if (IS_ERR_VALUE(ret)) {
		printf("%s: Failed to set dclk: ret=%d\n", __func__, ret);
		return ret;
	}

	memcpy(vop->regsbak, vop->regs, vop_data->reg_len);

	rockchip_vop_init_gamma(vop, state);

	ret = gpio_request_by_name(crtc_state->dev, "mcu-rs-gpios",
				   0, &vop->mcu_rs_gpio, GPIOD_IS_OUT);
	if (ret && ret != -ENOENT)
		printf("%s: Cannot get mcu rs GPIO: %d\n", __func__, ret);

	VOP_CTRL_SET(vop, global_regdone_en, 1);
	VOP_CTRL_SET(vop, axi_outstanding_max_num, 30);
	VOP_CTRL_SET(vop, axi_max_outstanding_en, 1);
	VOP_CTRL_SET(vop, reg_done_frm, 1);
	VOP_CTRL_SET(vop, win_gate[0], 1);
	VOP_CTRL_SET(vop, win_gate[1], 1);
	VOP_CTRL_SET(vop, win_channel[0], 0x12);
	VOP_CTRL_SET(vop, win_channel[1], 0x34);
	VOP_CTRL_SET(vop, win_channel[2], 0x56);
	VOP_CTRL_SET(vop, dsp_blank, 0);

	dclk_inv = (mode->flags & DRM_MODE_FLAG_PPIXDATA) ? 0 : 1;
	/* For improving signal quality, dclk need to be inverted by default on rv1106. */
	if ((VOP_MAJOR(vop->version) == 2 && VOP_MINOR(vop->version) == 12))
		dclk_inv = !dclk_inv;
	VOP_CTRL_SET(vop, dclk_pol, dclk_inv);

	val = 0x8;
	val |= (mode->flags & DRM_MODE_FLAG_NHSYNC) ? 0 : 1;
	val |= (mode->flags & DRM_MODE_FLAG_NVSYNC) ? 0 : (1 << 1);
	VOP_CTRL_SET(vop, pin_pol, val);

	switch (conn_state->type) {
	case DRM_MODE_CONNECTOR_LVDS:
		VOP_CTRL_SET(vop, rgb_en, 1);
		VOP_CTRL_SET(vop, rgb_pin_pol, val);
		VOP_CTRL_SET(vop, rgb_dclk_pol, dclk_inv);
		VOP_CTRL_SET(vop, lvds_en, 1);
		VOP_CTRL_SET(vop, lvds_pin_pol, val);
		VOP_CTRL_SET(vop, lvds_dclk_pol, dclk_inv);
		if (!IS_ERR_OR_NULL(vop->grf))
			VOP_GRF_SET(vop, grf_dclk_inv, dclk_inv);
		break;
	case DRM_MODE_CONNECTOR_eDP:
		VOP_CTRL_SET(vop, edp_en, 1);
		VOP_CTRL_SET(vop, edp_pin_pol, val);
		VOP_CTRL_SET(vop, edp_dclk_pol, dclk_inv);
		break;
	case DRM_MODE_CONNECTOR_HDMIA:
		VOP_CTRL_SET(vop, hdmi_en, 1);
		VOP_CTRL_SET(vop, hdmi_pin_pol, val);
		VOP_CTRL_SET(vop, hdmi_dclk_pol, 1);
		break;
	case DRM_MODE_CONNECTOR_DSI:
		VOP_CTRL_SET(vop, mipi_en, 1);
		VOP_CTRL_SET(vop, mipi_pin_pol, val);
		VOP_CTRL_SET(vop, mipi_dclk_pol, dclk_inv);
		VOP_CTRL_SET(vop, mipi_dual_channel_en,
			!!(conn_state->output_flags & ROCKCHIP_OUTPUT_DUAL_CHANNEL_LEFT_RIGHT_MODE));
		VOP_CTRL_SET(vop, data01_swap,
			!!(conn_state->output_flags & ROCKCHIP_OUTPUT_DATA_SWAP) ||
			crtc_state->dual_channel_swap);
		break;
	case DRM_MODE_CONNECTOR_DisplayPort:
		VOP_CTRL_SET(vop, dp_dclk_pol, 0);
		VOP_CTRL_SET(vop, dp_pin_pol, val);
		VOP_CTRL_SET(vop, dp_en, 1);
		break;
	case DRM_MODE_CONNECTOR_TV:
		if (vdisplay == CVBS_PAL_VDISPLAY)
			VOP_CTRL_SET(vop, tve_sw_mode, 1);
		else
			VOP_CTRL_SET(vop, tve_sw_mode, 0);
		VOP_CTRL_SET(vop, tve_dclk_pol, 1);
		VOP_CTRL_SET(vop, tve_dclk_en, 1);
		/* use the same pol reg with hdmi */
		VOP_CTRL_SET(vop, hdmi_pin_pol, val);
		VOP_CTRL_SET(vop, sw_genlock, 1);
		VOP_CTRL_SET(vop, sw_uv_offset_en, 1);
		VOP_CTRL_SET(vop, dither_up, 1);
		break;
	default:
		printf("unsupport connector_type[%d]\n", conn_state->type);
	}

	if (conn_state->output_mode == ROCKCHIP_OUT_MODE_AAAA &&
	    !(vop_data->feature & VOP_FEATURE_OUTPUT_10BIT))
		conn_state->output_mode = ROCKCHIP_OUT_MODE_P888;

	switch (conn_state->bus_format) {
	case MEDIA_BUS_FMT_RGB565_1X16:
		val = DITHER_DOWN_EN(1) | DITHER_DOWN_MODE(RGB888_TO_RGB565);
		break;
	case MEDIA_BUS_FMT_RGB666_1X18:
	case MEDIA_BUS_FMT_RGB666_1X24_CPADHI:
	case MEDIA_BUS_FMT_RGB666_1X7X3_SPWG:
	case MEDIA_BUS_FMT_RGB666_1X7X3_JEIDA:
		val = DITHER_DOWN_EN(1) | DITHER_DOWN_MODE(RGB888_TO_RGB666);
		break;
	case MEDIA_BUS_FMT_YUV8_1X24:
	case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
		val = DITHER_DOWN_EN(0) | PRE_DITHER_DOWN_EN(1);
		break;
	case MEDIA_BUS_FMT_YUV10_1X30:
	case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
		val = DITHER_DOWN_EN(0) | PRE_DITHER_DOWN_EN(0);
		break;
	case MEDIA_BUS_FMT_RGB888_1X24:
	case MEDIA_BUS_FMT_RGB888_1X7X4_SPWG:
	case MEDIA_BUS_FMT_RGB888_1X7X4_JEIDA:
	default:
		val = DITHER_DOWN_EN(0) | PRE_DITHER_DOWN_EN(0);
		break;
	}
	if (conn_state->output_mode == ROCKCHIP_OUT_MODE_AAAA)
		val |= PRE_DITHER_DOWN_EN(0);
	else
		val |= PRE_DITHER_DOWN_EN(1);
	val |= DITHER_DOWN_MODE_SEL(DITHER_DOWN_ALLEGRO);
	VOP_CTRL_SET(vop, dither_down, val);

	VOP_CTRL_SET(vop, dclk_ddr,
		     conn_state->output_mode == ROCKCHIP_OUT_MODE_YUV420 ? 1 : 0);
	VOP_CTRL_SET(vop, hdmi_dclk_out_en,
		     conn_state->output_mode == ROCKCHIP_OUT_MODE_YUV420 ? 1 : 0);

	if (is_uv_swap(conn_state->bus_format, conn_state->output_mode))
		VOP_CTRL_SET(vop, dsp_data_swap, DSP_RB_SWAP);
	else
		VOP_CTRL_SET(vop, dsp_data_swap, 0);

	VOP_CTRL_SET(vop, out_mode, conn_state->output_mode);

	if (VOP_CTRL_SUPPORT(vop, overlay_mode)) {
		yuv_overlay = is_yuv_output(conn_state->bus_format);
		VOP_CTRL_SET(vop, overlay_mode, yuv_overlay);
	}
	/*
	 * todo: r2y for win csc
	 */
	VOP_CTRL_SET(vop, dsp_out_yuv, is_yuv_output(conn_state->bus_format));

	if (yuv_overlay) {
		if (!is_yuv_output(conn_state->bus_format))
			post_y2r_en = true;
	} else {
		if (is_yuv_output(conn_state->bus_format))
			post_r2y_en = true;
	}

	crtc_state->yuv_overlay = yuv_overlay;
	post_csc_mode = to_vop_csc_mode(conn_state->color_space);
	VOP_CTRL_SET(vop, bcsh_r2y_en, post_r2y_en);
	VOP_CTRL_SET(vop, bcsh_y2r_en, post_y2r_en);
	VOP_CTRL_SET(vop, bcsh_r2y_csc_mode, post_csc_mode);
	VOP_CTRL_SET(vop, bcsh_y2r_csc_mode, post_csc_mode);

	/*
	 * Background color is 10bit depth if vop version >= 3.5
	 */
	if (!is_yuv_output(conn_state->bus_format))
		val = 0;
	else if (VOP_MAJOR(vop->version) == 3 &&
		 VOP_MINOR(vop->version) >= 5)
		val = 0x20010200;
	else
		val = 0x801080;
	VOP_CTRL_SET(vop, dsp_background, val);

	VOP_CTRL_SET(vop, htotal_pw, (htotal << 16) | hsync_len);
	val = hact_st << 16;
	val |= hact_end;
	VOP_CTRL_SET(vop, hact_st_end, val);
	val = vact_st << 16;
	val |= vact_end;
	VOP_CTRL_SET(vop, vact_st_end, val);
	if (mode->flags & DRM_MODE_FLAG_INTERLACE) {
		u16 vact_st_f1 = vtotal + vact_st + 1;
		u16 vact_end_f1 = vact_st_f1 + vdisplay;

		val = vact_st_f1 << 16 | vact_end_f1;
		VOP_CTRL_SET(vop, vact_st_end_f1, val);

		val = vtotal << 16 | (vtotal + vsync_len);
		VOP_CTRL_SET(vop, vs_st_end_f1, val);
		VOP_CTRL_SET(vop, dsp_interlace, 1);
		VOP_CTRL_SET(vop, p2i_en, 1);
		vtotal += vtotal + 1;
		act_end = vact_end_f1;
	} else {
		VOP_CTRL_SET(vop, dsp_interlace, 0);
		VOP_CTRL_SET(vop, p2i_en, 0);
		act_end = vact_end;
	}
	VOP_CTRL_SET(vop, vtotal_pw, (vtotal << 16) | vsync_len);
	vop_post_config(state, vop);
	VOP_CTRL_SET(vop, core_dclk_div,
		     !!(mode->flags & DRM_MODE_FLAG_DBLCLK));

	VOP_LINE_FLAG_SET(vop, line_flag_num[0], act_end - 3);
	VOP_LINE_FLAG_SET(vop, line_flag_num[1],
			  act_end - us_to_vertical_line(mode, 1000));
	if (state->crtc_state.mcu_timing.mcu_pix_total > 0)
		vop_mcu_mode(state, vop);
	vop_cfg_done(vop);

	return 0;
}

static uint16_t scl_vop_cal_scale(enum scale_mode mode, uint32_t src,
				  uint32_t dst, bool is_horizontal,
				  int vsu_mode, int *vskiplines)
{
	uint16_t val = 1 << SCL_FT_DEFAULT_FIXPOINT_SHIFT;

	if (is_horizontal) {
		if (mode == SCALE_UP)
			val = GET_SCL_FT_BIC(src, dst);
		else if (mode == SCALE_DOWN)
			val = GET_SCL_FT_BILI_DN(src, dst);
	} else {
		if (mode == SCALE_UP) {
			if (vsu_mode == SCALE_UP_BIL)
				val = GET_SCL_FT_BILI_UP(src, dst);
			else
				val = GET_SCL_FT_BIC(src, dst);
		} else if (mode == SCALE_DOWN) {
			if (vskiplines) {
				*vskiplines = scl_get_vskiplines(src, dst);
				val = scl_get_bili_dn_vskip(src, dst,
							    *vskiplines);
			} else {
				val = GET_SCL_FT_BILI_DN(src, dst);
			}
		}
	}

	return val;
}

static void scl_vop_cal_scl_fac(struct vop *vop,
				uint32_t src_w, uint32_t src_h, uint32_t dst_w,
				uint32_t dst_h, uint32_t pixel_format)
{
	uint16_t yrgb_hor_scl_mode, yrgb_ver_scl_mode;
	uint16_t cbcr_hor_scl_mode = SCALE_NONE;
	uint16_t cbcr_ver_scl_mode = SCALE_NONE;
	int hsub = drm_format_horz_chroma_subsampling(pixel_format);
	int vsub = drm_format_vert_chroma_subsampling(pixel_format);
	bool is_yuv = false;
	uint16_t cbcr_src_w = src_w / hsub;
	uint16_t cbcr_src_h = src_h / vsub;
	uint16_t vsu_mode;
	uint16_t lb_mode;
	uint32_t val;
	int vskiplines = 0;

	if (!vop->win->scl)
		return;

	if (!vop->win->scl->ext) {
		VOP_SCL_SET(vop, scale_yrgb_x,
			    scl_cal_scale2(src_w, dst_w));
		VOP_SCL_SET(vop, scale_yrgb_y,
			    scl_cal_scale2(src_h, dst_h));
		if (is_yuv) {
			VOP_SCL_SET(vop, scale_cbcr_x,
				    scl_cal_scale2(src_w, dst_w));
			VOP_SCL_SET(vop, scale_cbcr_y,
				    scl_cal_scale2(src_h, dst_h));
		}
		return;
	}

	yrgb_hor_scl_mode = scl_get_scl_mode(src_w, dst_w);
	yrgb_ver_scl_mode = scl_get_scl_mode(src_h, dst_h);

	if (is_yuv) {
		cbcr_hor_scl_mode = scl_get_scl_mode(cbcr_src_w, dst_w);
		cbcr_ver_scl_mode = scl_get_scl_mode(cbcr_src_h, dst_h);
		if (cbcr_hor_scl_mode == SCALE_DOWN)
			lb_mode = scl_vop_cal_lb_mode(dst_w, true);
		else
			lb_mode = scl_vop_cal_lb_mode(cbcr_src_w, true);
	} else {
		if (yrgb_hor_scl_mode == SCALE_DOWN)
			lb_mode = scl_vop_cal_lb_mode(dst_w, false);
		else
			lb_mode = scl_vop_cal_lb_mode(src_w, false);
	}

	VOP_SCL_SET_EXT(vop, lb_mode, lb_mode);
	if (lb_mode == LB_RGB_3840X2) {
		if (yrgb_ver_scl_mode != SCALE_NONE) {
			printf("ERROR : not allow yrgb ver scale\n");
			return;
		}
		if (cbcr_ver_scl_mode != SCALE_NONE) {
			printf("ERROR : not allow cbcr ver scale\n");
			return;
		}
		vsu_mode = SCALE_UP_BIL;
	} else if (lb_mode == LB_RGB_2560X4) {
		vsu_mode = SCALE_UP_BIL;
	} else {
		vsu_mode = SCALE_UP_BIC;
	}

	val = scl_vop_cal_scale(yrgb_hor_scl_mode, src_w, dst_w,
				true, 0, NULL);
	VOP_SCL_SET(vop, scale_yrgb_x, val);
	val = scl_vop_cal_scale(yrgb_ver_scl_mode, src_h, dst_h,
				false, vsu_mode, &vskiplines);
	VOP_SCL_SET(vop, scale_yrgb_y, val);

	VOP_SCL_SET_EXT(vop, vsd_yrgb_gt4, vskiplines == 4);
	VOP_SCL_SET_EXT(vop, vsd_yrgb_gt2, vskiplines == 2);

	VOP_SCL_SET_EXT(vop, yrgb_hor_scl_mode, yrgb_hor_scl_mode);
	VOP_SCL_SET_EXT(vop, yrgb_ver_scl_mode, yrgb_ver_scl_mode);
	VOP_SCL_SET_EXT(vop, yrgb_hsd_mode, SCALE_DOWN_BIL);
	VOP_SCL_SET_EXT(vop, yrgb_vsd_mode, SCALE_DOWN_BIL);
	VOP_SCL_SET_EXT(vop, yrgb_vsu_mode, vsu_mode);
	if (is_yuv) {
		val = scl_vop_cal_scale(cbcr_hor_scl_mode, cbcr_src_w,
					dst_w, true, 0, NULL);
		VOP_SCL_SET(vop, scale_cbcr_x, val);
		val = scl_vop_cal_scale(cbcr_ver_scl_mode, cbcr_src_h,
					dst_h, false, vsu_mode, &vskiplines);
		VOP_SCL_SET(vop, scale_cbcr_y, val);

		VOP_SCL_SET_EXT(vop, vsd_cbcr_gt4, vskiplines == 4);
		VOP_SCL_SET_EXT(vop, vsd_cbcr_gt2, vskiplines == 2);
		VOP_SCL_SET_EXT(vop, cbcr_hor_scl_mode, cbcr_hor_scl_mode);
		VOP_SCL_SET_EXT(vop, cbcr_ver_scl_mode, cbcr_ver_scl_mode);
		VOP_SCL_SET_EXT(vop, cbcr_hsd_mode, SCALE_DOWN_BIL);
		VOP_SCL_SET_EXT(vop, cbcr_vsd_mode, SCALE_DOWN_BIL);
		VOP_SCL_SET_EXT(vop, cbcr_vsu_mode, vsu_mode);
	}
}

static void vop_load_csc_table(struct vop *vop, u32 offset, const u32 *table)
{
	int i;

	/*
	 * so far the csc offset is not 0 and in the feature the csc offset
	 * impossible be 0, so when the offset is 0, should return here.
	 */
	if (!table || offset == 0)
		return;

	for (i = 0; i < 8; i++)
		vop_writel(vop, offset + i * 4, table[i]);
}

static int rockchip_vop_setup_csc_table(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	struct connector_state *conn_state = &state->conn_state;
	struct vop *vop = crtc_state->private;
	const uint32_t *csc_table = NULL;

	if (!vop->csc_table || !crtc_state->yuv_overlay)
		return 0;
	/* todo: only implement r2y*/
	switch (conn_state->color_space) {
	case V4L2_COLORSPACE_SMPTE170M:
		csc_table = vop->csc_table->r2y_bt601_12_235;
		break;
	case V4L2_COLORSPACE_REC709:
	case V4L2_COLORSPACE_DEFAULT:
	case V4L2_COLORSPACE_JPEG:
		csc_table = vop->csc_table->r2y_bt709;
		break;
	case V4L2_COLORSPACE_BT2020:
		csc_table = vop->csc_table->r2y_bt2020;
		break;
	default:
		csc_table = vop->csc_table->r2y_bt601;
		break;
	}

	vop_load_csc_table(vop, vop->win_csc->r2y_offset, csc_table);
	VOP_WIN_CSC_SET(vop, r2y_en, 1);

	return 0;
}

static int rockchip_vop_set_plane(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct vop_data *vop_data = crtc->data;
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	u32 act_info, dsp_info, dsp_st, dsp_stx, dsp_sty;
	struct vop *vop = crtc_state->private;
	int src_w = crtc_state->src_rect.w;
	int src_h = crtc_state->src_rect.h;
	int crtc_x = crtc_state->crtc_rect.x;
	int crtc_y = crtc_state->crtc_rect.y;
	int crtc_w = crtc_state->crtc_rect.w;
	int crtc_h = crtc_state->crtc_rect.h;
	int xvir = crtc_state->xvir;
	int x_mirror = 0, y_mirror = 0;

	if (crtc_w > crtc_state->max_output.width) {
		printf("ERROR: output w[%d] exceeded max width[%d]\n",
		       crtc_w, crtc_state->max_output.width);
		return -EINVAL;
	}

	act_info = (src_h - 1) << 16;
	act_info |= (src_w - 1) & 0xffff;

	dsp_info = (crtc_h - 1) << 16;
	dsp_info |= (crtc_w - 1) & 0xffff;

	dsp_stx = crtc_x + mode->crtc_htotal - mode->crtc_hsync_start;
	dsp_sty = crtc_y + mode->crtc_vtotal - mode->crtc_vsync_start;
	dsp_st = dsp_sty << 16 | (dsp_stx & 0xffff);
	/*
	 * vop full need to treats rgb888 as bgr888 so we reverse the rb swap to workaround
	 */
	if (crtc_state->format == ROCKCHIP_FMT_RGB888 && VOP_MAJOR(vop_data->version) == 3)
		crtc_state->rb_swap = !crtc_state->rb_swap;

	if (mode->flags & DRM_MODE_FLAG_YMIRROR)
		y_mirror = 1;
	else
		y_mirror = 0;
	if (mode->flags & DRM_MODE_FLAG_XMIRROR)
		x_mirror = 1;
	else
		x_mirror = 0;
	if (crtc_state->ymirror ^ y_mirror)
		y_mirror = 1;
	else
		y_mirror = 0;
	if (y_mirror) {
		if (VOP_CTRL_SUPPORT(vop, ymirror))
			crtc_state->dma_addr += (src_h - 1) * xvir * 4;
		else
			y_mirror = 0;
		}
	VOP_CTRL_SET(vop, ymirror, y_mirror);
	VOP_CTRL_SET(vop, xmirror, x_mirror);

	VOP_WIN_SET(vop, format, crtc_state->format);
	VOP_WIN_SET(vop, yrgb_vir, xvir);
	VOP_WIN_SET(vop, yrgb_mst, crtc_state->dma_addr);

	scl_vop_cal_scl_fac(vop, src_w, src_h, crtc_w, crtc_h,
			    crtc_state->format);

	VOP_WIN_SET(vop, act_info, act_info);
	VOP_WIN_SET(vop, dsp_info, dsp_info);
	VOP_WIN_SET(vop, dsp_st, dsp_st);
	VOP_WIN_SET(vop, rb_swap, crtc_state->rb_swap);

	VOP_WIN_SET(vop, src_alpha_ctl, 0);

	rockchip_vop_setup_csc_table(state);
	VOP_WIN_SET(vop, enable, 1);
	VOP_WIN_SET(vop, gate, 1);
	vop_cfg_done(vop);

	return 0;
}

static int rockchip_vop_prepare(struct display_state *state)
{
	return 0;
}

static int rockchip_vop_enable(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	struct vop *vop = crtc_state->private;

	VOP_CTRL_SET(vop, standby, 0);
	vop_cfg_done(vop);
	if (crtc_state->mcu_timing.mcu_pix_total > 0)
		VOP_CTRL_SET(vop, mcu_hold_mode, 0);

	return 0;
}

static int rockchip_vop_disable(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	struct vop *vop = crtc_state->private;

	VOP_CTRL_SET(vop, standby, 1);
	vop_cfg_done(vop);
	return 0;
}

static int rockchip_vop_fixup_dts(struct display_state *state, void *blob)
{
#if 0
	struct crtc_state *crtc_state = &state->crtc_state;
	struct panel_state *pstate = &state->panel_state;
	uint32_t phandle;
	char path[100];
	int ret, dsp_lut_node;

	if (!ofnode_valid(pstate->dsp_lut_node))
		return 0;
	ret = fdt_get_path(state->blob, pstate->dsp_lut_node, path, sizeof(path));
	if (ret < 0) {
		printf("failed to get dsp_lut path[%s], ret=%d\n",
			path, ret);
		return ret;
	}

	dsp_lut_node = fdt_path_offset(blob, path);
	phandle = fdt_get_phandle(blob, dsp_lut_node);
	if (!phandle) {
		phandle = fdt_alloc_phandle(blob);
		if (!phandle) {
			printf("failed to alloc phandle\n");
			return -ENOMEM;
		}

		fdt_set_phandle(blob, dsp_lut_node, phandle);
	}

	ret = fdt_get_path(state->blob, crtc_state->node, path, sizeof(path));
	if (ret < 0) {
		printf("failed to get route path[%s], ret=%d\n",
			path, ret);
		return ret;
	}

	do_fixup_by_path_u32(blob, path, "dsp-lut", phandle, 1);
#endif
	return 0;
}

static int rockchip_vop_send_mcu_cmd(struct display_state *state,
				     u32 type, u32 value)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	struct vop *vop = crtc_state->private;

	if (vop) {
		switch (type) {
		case MCU_WRCMD:
			set_vop_mcu_rs(vop, 0);
			VOP_CTRL_SET(vop, mcu_rw_bypass_port, value);
			set_vop_mcu_rs(vop, 1);
			break;
		case MCU_WRDATA:
			set_vop_mcu_rs(vop, 1);
			VOP_CTRL_SET(vop, mcu_rw_bypass_port, value);
			break;
		case MCU_SETBYPASS:
			VOP_CTRL_SET(vop, mcu_bypass, value ? 1 : 0);
			break;
		default:
			break;
		}
	}

	return 0;
}

static int rockchip_vop_mode_valid(struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	struct videomode vm;

	drm_display_mode_to_videomode(mode, &vm);

	if (vm.hactive < 32 || vm.vactive < 32 ||
	    (vm.hfront_porch * vm.hsync_len * vm.hback_porch *
	     vm.vfront_porch * vm.vsync_len * vm.vback_porch == 0)) {
		printf("ERROR: unsupported display timing\n");
		return -EINVAL;
	}

	return 0;
}

static int rockchip_vop_plane_check(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct vop_data *vop_data = crtc->data;
	const struct vop_win *win = vop_data->win;
	struct display_rect *src = &crtc_state->src_rect;
	struct display_rect *dst = &crtc_state->crtc_rect;
	int min_scale, max_scale;
	int hscale, vscale;

	min_scale = win->scl ? FRAC_16_16(1, 8) : VOP_PLANE_NO_SCALING;
	max_scale = win->scl ? FRAC_16_16(8, 1) : VOP_PLANE_NO_SCALING;

	hscale = display_rect_calc_hscale(src, dst, min_scale, max_scale);
	vscale = display_rect_calc_vscale(src, dst, min_scale, max_scale);
	if (hscale < 0 || vscale < 0) {
		printf("ERROR: scale factor is out of range\n");
		return -ERANGE;
	}

	return 0;
}

const struct rockchip_crtc_funcs rockchip_vop_funcs = {
	.preinit = rockchip_vop_preinit,
	.init = rockchip_vop_init,
	.set_plane = rockchip_vop_set_plane,
	.prepare = rockchip_vop_prepare,
	.enable = rockchip_vop_enable,
	.disable = rockchip_vop_disable,
	.fixup_dts = rockchip_vop_fixup_dts,
	.send_mcu_cmd = rockchip_vop_send_mcu_cmd,
	.mode_valid = rockchip_vop_mode_valid,
	.plane_check = rockchip_vop_plane_check,
};
