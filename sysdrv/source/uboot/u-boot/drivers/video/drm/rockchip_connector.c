// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Rockchip Connector Helper Function
 *
 * Copyright (C) 2021 Rockchip Electronics Co., Ltd
 */

#include <dm/device.h>
#include <dm/read.h>
#include <linux/compat.h>
#include <linux/list.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_phy.h"

#ifdef CONFIG_SPL_BUILD
int rockchip_connector_bind(struct rockchip_connector *conn, struct udevice *dev, int id,
			    const struct rockchip_connector_funcs *funcs, void *data, int type)
{
	conn->id = id;
	conn->funcs = funcs;
	conn->data = data;
	conn->type = type;

	return 0;
}

#else
static LIST_HEAD(rockchip_connector_list);

int rockchip_connector_bind(struct rockchip_connector *conn, struct udevice *dev, int id,
			    const struct rockchip_connector_funcs *funcs, void *data, int type)
{
	conn->id = id;
	conn->dev = dev;
	conn->funcs = funcs;
	conn->data = data;
	conn->type = type;
	list_add_tail(&conn->head, &rockchip_connector_list);

	return 0;
}

struct rockchip_connector *get_rockchip_connector_by_device(struct udevice *dev)
{
	struct rockchip_connector *conn;

	list_for_each_entry(conn, &rockchip_connector_list, head) {
		if (conn->dev == dev)
			return conn;
	}

	return NULL;
}

int rockchip_connector_pre_init(struct display_state *state)
{
	int ret = 0;
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	if (conn->funcs->pre_init) {
		ret = conn->funcs->pre_init(conn, state);
		if (ret)
			return ret;
		if (state->conn_state.secondary) {
			conn = state->conn_state.connector;
			ret = conn->funcs->pre_init(conn, state);
			if (ret)
				return ret;
		}
	}

	return ret;
}

static int rockchip_connector_path_init(struct rockchip_connector *conn,
					struct display_state *state)
{
	int ret = 0;

	if (conn->panel)
		rockchip_panel_init(conn->panel, conn, state);

	if (conn->bridge)
		rockchip_bridge_init(conn->bridge, conn, state);

	if (conn->funcs->init) {
		ret = conn->funcs->init(conn, state);
		if (ret)
			return ret;
	}

	if (conn->phy)
		rockchip_phy_init(conn->phy);

	return ret;
}

int rockchip_connector_init(struct display_state *state)
{
	int ret = 0;
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	ret = rockchip_connector_path_init(conn, state);
	if (ret)
		return ret;
	if (state->conn_state.secondary) {
		conn = state->conn_state.secondary;
		ret = rockchip_connector_path_init(conn, state);
		if (ret)
			return ret;
	}

	return ret;
}


static bool rockchip_connector_path_detect(struct rockchip_connector *conn,
					   struct display_state *state)
{
	int ret;

	if (conn->funcs->detect) {
		ret = conn->funcs->detect(conn, state);
		if (!ret) {
			printf("%s disconnected\n", conn->dev->name);
			return false;
		}
	}

	if (conn->bridge) {
		ret = rockchip_bridge_detect(conn->bridge);
		if (!ret) {
			printf("%s disconnected\n",
			       dev_np(conn->bridge->dev)->full_name);
			return false;
		}
	}

	return true;
}

bool rockchip_connector_detect(struct display_state *state)
{
	bool ret;
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	ret = rockchip_connector_path_detect(conn, state);
	if (!ret)
		return false;
	if (state->conn_state.secondary) {
		conn = state->conn_state.secondary;
		ret = rockchip_connector_path_detect(conn, state);
		if (!ret)
			return false;
	}

	return true;
}

int rockchip_connector_get_timing(struct display_state *state)
{
	int ret = 0;
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	if (conn->funcs->get_timing) {
		ret = conn->funcs->get_timing(conn, state);
		if (ret)
			return ret;
		if (state->conn_state.secondary) {
			conn = state->conn_state.secondary;
			ret = conn->funcs->get_timing(conn, state);
			if (ret)
				return ret;
		}
	}

	return ret;
}

int rockchip_connector_get_edid(struct display_state *state)
{
	int ret = 0;
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	if (conn->funcs->get_edid) {
		ret = conn->funcs->get_edid(conn, state);
		if (ret)
			return ret;
		if (state->conn_state.secondary) {
			conn = state->conn_state.secondary;
			ret = conn->funcs->get_edid(conn, state);
			if (ret)
				return ret;
		}
	}

	return ret;
}

static int rockchip_connector_path_pre_enable(struct rockchip_connector *conn,
					      struct display_state *state)
{
	if (conn->funcs->prepare)
		conn->funcs->prepare(conn, state);

	if (conn->bridge)
		rockchip_bridge_pre_enable(conn->bridge);

	if (conn->panel)
		rockchip_panel_prepare(conn->panel);

	return 0;
}

int rockchip_connector_pre_enable(struct display_state *state)
{
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	rockchip_connector_path_pre_enable(conn, state);
	if (state->conn_state.secondary) {
		conn = state->conn_state.secondary;
		rockchip_connector_path_pre_enable(conn, state);
	}

	return 0;
}

static int rockchip_connector_path_enable(struct rockchip_connector *conn,
					  struct display_state *state)
{
	if (conn->funcs->enable)
		conn->funcs->enable(conn, state);

	if (conn->bridge)
		rockchip_bridge_enable(conn->bridge);

	if (conn->panel)
		rockchip_panel_enable(conn->panel);

	return 0;
}

int rockchip_connector_enable(struct display_state *state)
{
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	rockchip_connector_path_enable(conn, state);
	if (state->conn_state.secondary) {
		conn = state->conn_state.secondary;
		rockchip_connector_path_enable(conn, state);
	}

	return 0;
}

static int rockchip_connector_path_disable(struct rockchip_connector *conn,
					   struct display_state *state)
{
	if (conn->panel)
		rockchip_panel_disable(conn->panel);

	if (conn->bridge)
		rockchip_bridge_disable(conn->bridge);

	if (conn->funcs->disable)
		conn->funcs->disable(conn, state);

	return 0;
}

int rockchip_connector_disable(struct display_state *state)
{
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	rockchip_connector_path_disable(conn, state);
	if (state->conn_state.secondary) {
		conn = state->conn_state.secondary;
		rockchip_connector_path_disable(conn, state);
	}

	return 0;
}

static int rockchip_connector_path_post_disable(struct rockchip_connector *conn,
						struct display_state *state)
{
	if (conn->panel)
		rockchip_panel_unprepare(conn->panel);

	if (conn->bridge)
		rockchip_bridge_post_disable(conn->bridge);

	if (conn->funcs->unprepare)
		conn->funcs->unprepare(conn, state);

	return 0;
}

int rockchip_connector_post_disable(struct display_state *state)
{
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	rockchip_connector_path_post_disable(conn, state);
	if (state->conn_state.secondary) {
		conn = state->conn_state.secondary;
		rockchip_connector_path_post_disable(conn, state);
	}

	return 0;
}
#endif

int rockchip_connector_deinit(struct display_state *state)
{
	struct rockchip_connector *conn;

	conn = state->conn_state.connector;
	if (conn->funcs->deinit) {
		conn->funcs->deinit(conn, state);
		if (state->conn_state.secondary) {
			conn = state->conn_state.secondary;
			conn->funcs->deinit(conn, state);
		}
	}

	return 0;
}
