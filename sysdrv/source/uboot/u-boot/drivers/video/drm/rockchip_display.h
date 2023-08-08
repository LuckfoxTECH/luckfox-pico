/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ROCKCHIP_DISPLAY_H
#define _ROCKCHIP_DISPLAY_H

#include <bmp_layout.h>
#include <drm_modes.h>
#include <edid.h>
#include <dm/ofnode.h>
#include <drm/drm_dsc.h>

/*
 * major: IP major vertion, used for IP structure
 * minor: big feature change under same structure
 */
#define VOP_VERSION(major, minor)	((major) << 8 | (minor))
#define VOP_MAJOR(version)		((version) >> 8)
#define VOP_MINOR(version)		((version) & 0xff)

#define VOP_VERSION_RK3528		VOP_VERSION(0x50, 0x17)
#define VOP_VERSION_RK3568		VOP_VERSION(0x40, 0x15)
#define VOP_VERSION_RK3588		VOP_VERSION(0x40, 0x17)

#define ROCKCHIP_OUTPUT_DUAL_CHANNEL_LEFT_RIGHT_MODE	BIT(0)
#define ROCKCHIP_OUTPUT_DUAL_CHANNEL_ODD_EVEN_MODE	BIT(1)
#define ROCKCHIP_OUTPUT_DATA_SWAP			BIT(2)
#define ROCKCHIP_OUTPUT_MIPI_DS_MODE			BIT(3)

#define ROCKCHIP_DSC_PPS_SIZE_BYTE			88

enum data_format {
	ROCKCHIP_FMT_ARGB8888 = 0,
	ROCKCHIP_FMT_RGB888,
	ROCKCHIP_FMT_RGB565,
	ROCKCHIP_FMT_YUV420SP = 4,
	ROCKCHIP_FMT_YUV422SP,
	ROCKCHIP_FMT_YUV444SP,
};

enum display_mode {
	ROCKCHIP_DISPLAY_FULLSCREEN,
	ROCKCHIP_DISPLAY_CENTER,
};

enum rockchip_cmd_type {
	CMD_TYPE_DEFAULT,
	CMD_TYPE_SPI,
	CMD_TYPE_MCU
};

enum rockchip_mcu_cmd {
	MCU_WRCMD = 0,
	MCU_WRDATA,
	MCU_SETBYPASS,
};

/*
 * display output interface supported by rockchip lcdc
 */
#define ROCKCHIP_OUT_MODE_P888		0
#define ROCKCHIP_OUT_MODE_BT1120	0
#define ROCKCHIP_OUT_MODE_P666		1
#define ROCKCHIP_OUT_MODE_P565		2
#define ROCKCHIP_OUT_MODE_BT656		5
#define ROCKCHIP_OUT_MODE_S888		8
#define ROCKCHIP_OUT_MODE_S888_DUMMY	12
#define ROCKCHIP_OUT_MODE_YUV420	14
/* for use special outface */
#define ROCKCHIP_OUT_MODE_AAAA		15

#define VOP_OUTPUT_IF_RGB	BIT(0)
#define VOP_OUTPUT_IF_BT1120	BIT(1)
#define VOP_OUTPUT_IF_BT656	BIT(2)
#define VOP_OUTPUT_IF_LVDS0	BIT(3)
#define VOP_OUTPUT_IF_LVDS1	BIT(4)
#define VOP_OUTPUT_IF_MIPI0	BIT(5)
#define VOP_OUTPUT_IF_MIPI1	BIT(6)
#define VOP_OUTPUT_IF_eDP0	BIT(7)
#define VOP_OUTPUT_IF_eDP1	BIT(8)
#define VOP_OUTPUT_IF_DP0	BIT(9)
#define VOP_OUTPUT_IF_DP1	BIT(10)
#define VOP_OUTPUT_IF_HDMI0	BIT(11)
#define VOP_OUTPUT_IF_HDMI1	BIT(12)

struct rockchip_mcu_timing {
	int mcu_pix_total;
	int mcu_cs_pst;
	int mcu_cs_pend;
	int mcu_rw_pst;
	int mcu_rw_pend;
	int mcu_hold_mode;
};

struct vop_rect {
	int width;
	int height;
};

struct rockchip_dsc_sink_cap {
	/**
	 * @slice_width: the number of pixel columns that comprise the slice width
	 * @slice_height: the number of pixel rows that comprise the slice height
	 * @block_pred: Does block prediction
	 * @native_420: Does sink support DSC with 4:2:0 compression
	 * @bpc_supported: compressed bpc supported by sink : 10, 12 or 16 bpc
	 * @version_major: DSC major version
	 * @version_minor: DSC minor version
	 * @target_bits_per_pixel_x16: bits num after compress and multiply 16
	 */
	u16 slice_width;
	u16 slice_height;
	bool block_pred;
	bool native_420;
	u8 bpc_supported;
	u8 version_major;
	u8 version_minor;
	u16 target_bits_per_pixel_x16;
};

struct display_rect {
	int x;
	int y;
	int w;
	int h;
};

struct bcsh_state {
	int brightness;
	int contrast;
	int saturation;
	int sin_hue;
	int cos_hue;
};

struct crtc_state {
	struct udevice *dev;
	struct rockchip_crtc *crtc;
	void *private;
	ofnode node;
	struct device_node *ports_node; /* if (ports_node) it's vop2; */
	int crtc_id;

	int format;
	u32 dma_addr;
	int ymirror;
	int rb_swap;
	int xvir;
	int post_csc_mode;
	int dclk_core_div;
	int dclk_out_div;
	struct display_rect src_rect;
	struct display_rect crtc_rect;
	struct display_rect right_src_rect;
	struct display_rect right_crtc_rect;
	bool yuv_overlay;
	bool post_r2y_en;
	bool post_y2r_en;
	bool bcsh_en;
	bool splice_mode;
	u8 splice_crtc_id;
	u8 dsc_id;
	u8 dsc_enable;
	u8 dsc_slice_num;
	u8 dsc_pixel_num;
	struct rockchip_mcu_timing mcu_timing;
	u32 dual_channel_swap;
	u32 feature;
	struct vop_rect max_output;

	u64 dsc_txp_clk_rate;
	u64 dsc_pxl_clk_rate;
	u64 dsc_cds_clk_rate;
	struct drm_dsc_picture_parameter_set pps;
	struct rockchip_dsc_sink_cap dsc_sink_cap;
};

struct panel_state {
	struct rockchip_panel *panel;

	ofnode dsp_lut_node;
};

struct overscan {
	int left_margin;
	int right_margin;
	int top_margin;
	int bottom_margin;
};

struct connector_state {
	struct rockchip_connector *connector;
	struct rockchip_connector *secondary;

	struct drm_display_mode mode;
	struct overscan overscan;
	u8 edid[EDID_SIZE * 4];
	int bus_format;
	int output_mode;
	int type;
	int output_if;
	int output_flags;
	int color_space;
	unsigned int bpc;

	/**
	 * @hold_mode: enabled when it's:
	 * (1) mcu hold mode
	 * (2) mipi dsi cmd mode
	 * (3) edp psr mode
	 */
	bool hold_mode;

	struct base2_disp_info *disp_info; /* disp_info from baseparameter 2.0 */

	u8 dsc_id;
	u8 dsc_slice_num;
	u8 dsc_pixel_num;
	u64 dsc_txp_clk;
	u64 dsc_pxl_clk;
	u64 dsc_cds_clk;
	struct rockchip_dsc_sink_cap dsc_sink_cap;
	struct drm_dsc_picture_parameter_set pps;

	struct {
		u32 *lut;
		int size;
	} gamma;
};

struct logo_info {
	int mode;
	char *mem;
	bool ymirror;
	u32 offset;
	u32 width;
	int height;
	u32 bpp;
};

struct rockchip_logo_cache {
	struct list_head head;
	char name[20];
	struct logo_info logo;
};

struct display_state {
	struct list_head head;

	const void *blob;
	ofnode node;

	struct crtc_state crtc_state;
	struct connector_state conn_state;
	struct panel_state panel_state;

	char ulogo_name[30];
	char klogo_name[30];

	struct logo_info logo;
	int logo_mode;
	int charge_logo_mode;
	void *mem_base;
	int mem_size;

	int enable;
	int is_init;
	int is_enable;
	bool is_klogo_valid;
	bool force_output;
	struct drm_display_mode force_mode;
	u32 force_bus_format;
};

int drm_mode_vrefresh(const struct drm_display_mode *mode);
int display_send_mcu_cmd(struct display_state *state, u32 type, u32 val);
bool drm_mode_is_420(const struct drm_display_info *display,
		     struct drm_display_mode *mode);
struct base2_disp_info *rockchip_get_disp_info(int type, int id);

void drm_mode_max_resolution_filter(struct hdmi_edid_data *edid_data,
				    struct vop_rect *max_output);
unsigned long get_cubic_lut_buffer(int crtc_id);
int rockchip_ofnode_get_display_mode(ofnode node, struct drm_display_mode *mode);

int display_rect_calc_hscale(struct display_rect *src, struct display_rect *dst,
			     int min_hscale, int max_hscale);
int display_rect_calc_vscale(struct display_rect *src, struct display_rect *dst,
			     int min_vscale, int max_vscale);

#endif
