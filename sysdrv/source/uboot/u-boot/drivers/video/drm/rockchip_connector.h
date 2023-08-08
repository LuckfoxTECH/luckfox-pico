/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ROCKCHIP_CONNECTOR_H_
#define _ROCKCHIP_CONNECTOR_H_

#include "rockchip_bridge.h"
#include "rockchip_panel.h"

struct rockchip_connector {
	struct udevice *dev;
	struct rockchip_bridge *bridge;
	struct rockchip_panel *panel;
	struct rockchip_phy *phy;
	struct list_head head;
	int id;
	int type;

	const struct rockchip_connector_funcs *funcs;
	void *data;
};

struct rockchip_connector_funcs {
	/*
	 * pre init connector, prepare some parameter out_if, this will be
	 * used by rockchip_display.c and vop
	 */
	int (*pre_init)(struct rockchip_connector *connector, struct display_state *state);

	/*
	 * init connector, prepare resource to ensure
	 * detect and get_timing can works
	 */
	int (*init)(struct rockchip_connector *connector, struct display_state *state);

	void (*deinit)(struct rockchip_connector *connector, struct display_state *state);
	/*
	 * Optional, if connector not support hotplug,
	 * Returns:
	 *   0 means disconnected, else means connected
	 */
	int (*detect)(struct rockchip_connector *connector, struct display_state *state);
	/*
	 * Optional, if implement it, need fill the timing data:
	 *     state->conn_state->mode
	 * you can refer to the rockchip_display: display_get_timing(),
	 * Returns:
	 *   0 means success, else means failed
	 */
	int (*get_timing)(struct rockchip_connector *connector, struct display_state *state);
	/*
	 * Optional, if implement it, need fill the edid data:
	 *     state->conn_state->edid
	 * Returns:
	 *   0 means success, else means failed
	 */
	int (*get_edid)(struct rockchip_connector *connector, struct display_state *state);
	/*
	 * call before crtc enable.
	 */
	int (*prepare)(struct rockchip_connector *connector, struct display_state *state);
	/*
	 * call after crtc enable
	 */
	int (*enable)(struct rockchip_connector *connector, struct display_state *state);
	int (*disable)(struct rockchip_connector *connector, struct display_state *state);
	void (*unprepare)(struct rockchip_connector *connector, struct display_state *state);

	int (*check)(struct rockchip_connector *connector, struct display_state *state);
	int (*mode_valid)(struct rockchip_connector *connector, struct display_state *state);
};

const struct rockchip_connector *
rockchip_get_connector(const void *blob, int connector_node);
int rockchip_connector_bind(struct rockchip_connector *connector, struct udevice *dev, int id,
			    const struct rockchip_connector_funcs *funcs, void *data, int type);
struct rockchip_connector *get_rockchip_connector_by_device(struct udevice *dev);
int rockchip_connector_pre_init(struct display_state *state);
int rockchip_connector_init(struct display_state *state);
int rockchip_connector_deinit(struct display_state *state);
bool rockchip_connector_detect(struct display_state *state);
int rockchip_connector_get_timing(struct display_state *state);
int rockchip_connector_get_edid(struct display_state *state);
int rockchip_connector_pre_enable(struct display_state *state);
int rockchip_connector_enable(struct display_state *state);
int rockchip_connector_disable(struct display_state *state);
int rockchip_connector_post_disable(struct display_state *state);

#ifdef CONFIG_DRM_ROCKCHIP_ANALOGIX_DP
struct rockchip_dp_chip_data;
extern const struct rockchip_connector_funcs rockchip_analogix_dp_funcs;
extern const struct rockchip_dp_chip_data rk3399_analogix_edp_drv_data;
extern const struct rockchip_dp_chip_data rk3368_analogix_edp_drv_data;
extern const struct rockchip_dp_chip_data rk3288_analogix_dp_drv_data;
#endif
#endif
