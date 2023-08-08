// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2008-2018 Fuzhou Rockchip Electronics Co., Ltd
 */

#include "rockchip_bridge.h"

void rockchip_bridge_init(struct rockchip_bridge *bridge,
			  struct rockchip_connector *conn,
			  struct display_state *state)
{
	if (!bridge)
		return;

	bridge->conn = conn;
	bridge->state = state;

	if (bridge->next_bridge)
		rockchip_bridge_init(bridge->next_bridge, conn, state);
}

void rockchip_bridge_pre_enable(struct rockchip_bridge *bridge)
{
	if (!bridge)
		return;

	if (bridge->funcs && bridge->funcs->pre_enable)
		bridge->funcs->pre_enable(bridge);

	if (bridge->next_bridge)
		rockchip_bridge_pre_enable(bridge->next_bridge);
}

void rockchip_bridge_post_disable(struct rockchip_bridge *bridge)
{
	if (!bridge)
		return;

	if (bridge->next_bridge)
		rockchip_bridge_post_disable(bridge->next_bridge);

	if (bridge->funcs && bridge->funcs->post_disable)
		bridge->funcs->post_disable(bridge);
}

void rockchip_bridge_enable(struct rockchip_bridge *bridge)
{
	if (!bridge)
		return;

	if (bridge->funcs && bridge->funcs->enable)
		bridge->funcs->enable(bridge);

	if (bridge->next_bridge)
		rockchip_bridge_enable(bridge->next_bridge);
}

void rockchip_bridge_disable(struct rockchip_bridge *bridge)
{
	if (!bridge)
		return;

	if (bridge->next_bridge)
		rockchip_bridge_disable(bridge->next_bridge);

	if (bridge->funcs && bridge->funcs->disable)
		bridge->funcs->disable(bridge);
}

void rockchip_bridge_mode_set(struct rockchip_bridge *bridge,
			      const struct drm_display_mode *mode)
{
	if (!bridge || !mode)
		return;

	if (bridge->funcs && bridge->funcs->mode_set)
		bridge->funcs->mode_set(bridge, mode);

	if (bridge->next_bridge)
		rockchip_bridge_mode_set(bridge->next_bridge, mode);
}

bool rockchip_bridge_detect(struct rockchip_bridge *bridge)
{
	if (bridge->funcs && bridge->funcs->detect)
		if (!bridge->funcs->detect(bridge))
			return false;

	if (bridge->next_bridge)
		return rockchip_bridge_detect(bridge->next_bridge);

	return true;
}
