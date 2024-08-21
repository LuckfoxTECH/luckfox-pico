/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <malloc.h>
#include <mp_boot.h>
#include <spl.h>
#include <part.h>
#include <drm_modes.h>
#include <spl_display.h>
#include <linux/hdmi.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_phy.h"

static struct base2_info base_parameter;

struct display_state *rockchip_spl_display_drv_probe(void)
{
	struct display_state *state = malloc(sizeof(struct display_state));
	if (!state)
		return NULL;

	memset(state, 0, sizeof(*state));

	rockchip_spl_vop_probe(&state->crtc_state);
	rockchip_spl_dw_hdmi_probe(&state->conn_state);
	inno_spl_hdmi_phy_probe(state);

	return state;
}

static int rockchip_spl_display_init(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	struct connector_state *conn_state = &state->conn_state;
	struct rockchip_connector *conn = conn_state->connector;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct rockchip_crtc_funcs *crtc_funcs = crtc->funcs;
	const struct rockchip_connector_funcs *conn_funcs = conn->funcs;
	struct drm_display_mode *mode = &state->conn_state.mode;
	int ret = 0;

	if (!crtc_funcs) {
		printf("failed to find crtc functions\n");
		return -ENXIO;
	}

	if (crtc_funcs->preinit) {
		ret = crtc_funcs->preinit(state);
		if (ret)
			return ret;
	}

	rockchip_display_make_crc32_table();
	if (conn_funcs->pre_init) {
		ret = conn_funcs->pre_init(conn, state);
		if (ret)
			return ret;
	}

	if (conn_funcs->init) {
		ret = conn_funcs->init(conn, state);
		if (ret)
			goto deinit;
	}

	if (conn->phy)
		rockchip_phy_init(conn->phy);

	if (conn_funcs->detect) {
		conn->hpd = conn_funcs->detect(conn, state);
		if (!conn->hpd)
			goto deinit;
	}

	if (conn_funcs->get_timing) {
		ret = conn_funcs->get_timing(conn, state);
		if (ret)
			goto deinit;
	}

	drm_mode_set_crtcinfo(mode, CRTC_INTERLACE_HALVE_V);
	if (p->flags & DRM_MODE_FLAG_DBLCLK)
		p->crtc_clock = 2 * p->clock;

	if (crtc_funcs->init) {
		ret = crtc_funcs->init(state);
		if (ret)
			goto deinit;
	}

	return 0;

deinit:
	rockchip_connector_deinit(state);
	return ret;
}

static int rockchip_spl_display_post_enable(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	struct connector_state *conn_state = &state->conn_state;
	struct rockchip_connector *conn = conn_state->connector;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct rockchip_crtc_funcs *crtc_funcs = crtc->funcs;
	const struct rockchip_connector_funcs *conn_funcs = conn->funcs;

	if (crtc_funcs->enable)
		crtc_funcs->enable(state);
	state->crtc_state.crtc->active = true;

	if (conn_funcs->enable)
		conn_funcs->enable(conn, state);

	return 0;
}

static void rockchip_spl_display_transmit_info_to_uboot(struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct spl_display_info *spl_disp_info = (struct spl_display_info *)CONFIG_SPL_VIDEO_BUF;

	/* transmit mode and bus_format to uboot */
	memcpy(&spl_disp_info->mode, &conn_state->mode, sizeof(conn_state->mode));
	spl_disp_info->bus_format = state->conn_state.bus_format;
	spl_disp_info->enabled = 1;
	flush_dcache_all();
}

int spl_init_display(struct task_data *data)
{
	struct display_state *state = NULL;
	struct drm_display_mode *mode;
	int ret = 0;

	state = rockchip_spl_display_drv_probe();
	if (!state) {
		printf("rockchip_spl_display_drv_probe failed\n");
		return -1;
	}

	ret = rockchip_spl_display_init(state);
	if (ret) {
		printf("rockchip_spl_display_init failed ret:%d\n", ret);
		return  -1;
	}

	if (!state->conn_state.connector->hpd) {
		printf("HDMI is unplug and exit\n");
		return 0;
	}

	ret = rockchip_spl_display_post_enable(state);
	if (ret) {
		printf("rockchip_spl_display_post_enable failed ret:%d\n", ret);
		return -1;
	}

	rockchip_spl_display_transmit_info_to_uboot(state);

	mode = &state->conn_state.mode;
	printf("SPL enable hdmi, detailed mode clock %u kHz, flags[%x]\n"
	       "    H: %04d %04d %04d %04d\n"
	       "    V: %04d %04d %04d %04d\n"
	       "bus_format: %x\n",
	       mode->clock, mode->flags,
	       mode->hdisplay, mode->hsync_start,
	       mode->hsync_end, mode->htotal,
	       mode->vdisplay, mode->vsync_start,
	       mode->vsync_end, mode->vtotal,
	       state->conn_state.bus_format);

	return ret;
}

struct base2_disp_info *rockchip_get_disp_info(int type, int id)
{
	struct base2_disp_info *disp_info;
	struct base2_disp_header *disp_header;
	int i = 0, offset = -1;
	u32 crc_val;
	u32 base2_length;
	void *base_parameter_addr = (void *)&base_parameter;
#ifdef CONFIG_MP_BOOT
	void *bp_addr = (void *)CONFIG_SPL_VIDEO_BUF;
	ulong ret;

	/* make sure the baseparameter is ready */
	ret = mpb_post(6);
	printf("SPL read baseparameter %s\n", ret < 0 ? "failed" : "success");
	memcpy(&base_parameter, bp_addr, sizeof(base_parameter));
#endif
	for (i = 0; i < 8; i++) {
		disp_header = &base_parameter.disp_header[i];
		if (disp_header->connector_type == type &&
		    disp_header->connector_id == id) {
			printf("disp info %d, type:%d, id:%d\n", i, type, id);
			offset = disp_header->offset;
			break;
		}
	}

	if (offset < 0)
		return NULL;
	disp_info = base_parameter_addr + offset;
	if (disp_info->screen_info[0].type != type ||
	    disp_info->screen_info[0].id != id) {
		printf("base2_disp_info couldn't be found, screen_info type[%d] or id[%d] mismatched\n",
		       disp_info->screen_info[0].type,
		       disp_info->screen_info[0].id);
		return NULL;
	}

	if (strncasecmp(disp_info->disp_head_flag, "DISP", 4))
		return NULL;

	if (base_parameter.major_version == 3 && base_parameter.minor_version == 0) {
		crc_val = rockchip_display_crc32c_cal((unsigned char *)disp_info,
						      sizeof(struct base2_disp_info) - 4);
		if (crc_val != disp_info->crc2) {
			printf("error: connector type[%d], id[%d] disp info crc2 check error\n",
			       type, id);
			return NULL;
		}
	} else {
		base2_length = sizeof(struct base2_disp_info) - sizeof(struct csc_info) -
			       sizeof(struct acm_data) - 10 * 1024 - 4;
		crc_val = rockchip_display_crc32c_cal((unsigned char *)disp_info, base2_length - 4);
		if (crc_val != disp_info->crc) {
			printf("error: connector type[%d], id[%d] disp info crc check error\n",
			       type, id);
			return NULL;
		}
	}

	return disp_info;
}

int spl_load_baseparamter(struct task_data *data)
{
	struct spl_load_info *info = &data->info;
	ulong addr = CONFIG_SPL_VIDEO_BUF;
	disk_partition_t part;

	debug("== Baseparam: start\n");

	if (part_get_info_by_name(info->dev, "baseparameter", &part) < 0) {
		printf("No baseparameter partition\n");
		return -ENOENT;
	} else {
		if (info->read(info, part.start, part.size, (void *)addr) != part.size)
			return -EIO;
		else
			flush_dcache_range(addr, addr + part.size * info->bl_len);
	}

	debug("== Baseparam: load OK\n");

	return 0;
}

