/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2021 Fuzhou Rockchip Electronics Co., Ltd
 */
#ifndef _ROCKCHIP_DW_HDMI_REG_H_
#define _ROCKCHIP_DW_HDMI_REG_H_

/*
 * Rockchip connector callbacks.
 * If you want to know the details, please refer to rockchip_connector.h
 */
int rockchip_dw_hdmi_init(struct rockchip_connector *conn, struct display_state *state);
void rockchip_dw_hdmi_deinit(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_prepare(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_enable(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_disable(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_get_timing(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_detect(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_get_edid(struct rockchip_connector *conn, struct display_state *state);

enum drm_connector_status
inno_dw_hdmi_phy_read_hpd(struct dw_hdmi *hdmi,
			  void *data);
void inno_dw_hdmi_phy_disable(struct rockchip_connector *conn, struct dw_hdmi *dw_hdmi,
			      void *data);
int inno_dw_hdmi_phy_init(struct rockchip_connector *conn, struct dw_hdmi *dw_hdmi,
			  void *data);
void inno_dw_hdmi_mode_valid(struct rockchip_connector *conn, struct dw_hdmi *hdmi, void *data);

#endif /* _ROCKCHIP_DW_HDMI_REG_H_ */
