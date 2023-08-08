/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2022 Fuzhou Rockchip Electronics Co., Ltd
 */
#ifndef _ROCKCHIP_DW_HDMI_QP_H_
#define _ROCKCHIP_DW_HDMI_QP_H_

/*
 * Rockchip connector callbacks.
 * If you want to know the details, please refer to rockchip_connector.h
 */
int rockchip_dw_hdmi_qp_init(struct rockchip_connector *conn, struct display_state *state);
void rockchip_dw_hdmi_qp_deinit(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_qp_prepare(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_qp_enable(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_qp_disable(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_qp_get_timing(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_qp_detect(struct rockchip_connector *conn, struct display_state *state);
int rockchip_dw_hdmi_qp_get_edid(struct rockchip_connector *conn, struct display_state *state);

#endif /* _ROCKCHIP_DW_HDMI_QP_H_ */
