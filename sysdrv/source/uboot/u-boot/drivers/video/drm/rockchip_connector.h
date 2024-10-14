/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ROCKCHIP_CONNECTOR_H_
#define _ROCKCHIP_CONNECTOR_H_

#ifdef CONFIG_SPL_BUILD
struct rockchip_connector {
	struct rockchip_phy *phy;
	int id;
	int type;
	bool hpd;

	const struct rockchip_connector_funcs *funcs;
	void *data;
};
#else
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
	bool hpd;

	const struct rockchip_connector_funcs *funcs;
	void *data;
};
#endif

/**
 * enum drm_bus_flags - bus_flags info for &drm_display_info
 *
 * This enum defines signal polarities and clock edge information for signals on
 * a bus as bitmask flags.
 *
 * The clock edge information is conveyed by two sets of symbols,
 * DRM_BUS_FLAGS_*_DRIVE_\* and DRM_BUS_FLAGS_*_SAMPLE_\*. When this enum is
 * used to describe a bus from the point of view of the transmitter, the
 * \*_DRIVE_\* flags should be used. When used from the point of view of the
 * receiver, the \*_SAMPLE_\* flags should be used. The \*_DRIVE_\* and
 * \*_SAMPLE_\* flags alias each other, with the \*_SAMPLE_POSEDGE and
 * \*_SAMPLE_NEGEDGE flags being equal to \*_DRIVE_NEGEDGE and \*_DRIVE_POSEDGE
 * respectively. This simplifies code as signals are usually sampled on the
 * opposite edge of the driving edge. Transmitters and receivers may however
 * need to take other signal timings into account to convert between driving
 * and sample edges.
 */
enum drm_bus_flags {
	/**
	 * @DRM_BUS_FLAG_DE_LOW:
	 *
	 * The Data Enable signal is active low
	 */
	DRM_BUS_FLAG_DE_LOW = BIT(0),

	/**
	 * @DRM_BUS_FLAG_DE_HIGH:
	 *
	 * The Data Enable signal is active high
	 */
	DRM_BUS_FLAG_DE_HIGH = BIT(1),

	/**
	 * @DRM_BUS_FLAG_PIXDATA_DRIVE_POSEDGE:
	 *
	 * Data is driven on the rising edge of the pixel clock
	 */
	DRM_BUS_FLAG_PIXDATA_DRIVE_POSEDGE = BIT(2),

	/**
	 * @DRM_BUS_FLAG_PIXDATA_DRIVE_NEGEDGE:
	 *
	 * Data is driven on the falling edge of the pixel clock
	 */
	DRM_BUS_FLAG_PIXDATA_DRIVE_NEGEDGE = BIT(3),

	/**
	 * @DRM_BUS_FLAG_PIXDATA_SAMPLE_POSEDGE:
	 *
	 * Data is sampled on the rising edge of the pixel clock
	 */
	DRM_BUS_FLAG_PIXDATA_SAMPLE_POSEDGE = DRM_BUS_FLAG_PIXDATA_DRIVE_NEGEDGE,

	/**
	 * @DRM_BUS_FLAG_PIXDATA_SAMPLE_NEGEDGE:
	 *
	 * Data is sampled on the falling edge of the pixel clock
	 */
	DRM_BUS_FLAG_PIXDATA_SAMPLE_NEGEDGE = DRM_BUS_FLAG_PIXDATA_DRIVE_POSEDGE,

	/**
	 * @DRM_BUS_FLAG_DATA_MSB_TO_LSB:
	 *
	 * Data is transmitted MSB to LSB on the bus
	 */
	DRM_BUS_FLAG_DATA_MSB_TO_LSB = BIT(4),

	/**
	 * @DRM_BUS_FLAG_DATA_LSB_TO_MSB:
	 *
	 * Data is transmitted LSB to MSB on the bus
	 */
	DRM_BUS_FLAG_DATA_LSB_TO_MSB = BIT(5),

	/**
	 * @DRM_BUS_FLAG_SYNC_DRIVE_POSEDGE:
	 *
	 * Sync signals are driven on the rising edge of the pixel clock
	 */
	DRM_BUS_FLAG_SYNC_DRIVE_POSEDGE = BIT(6),

	/**
	 * @DRM_BUS_FLAG_SYNC_DRIVE_NEGEDGE:
	 *
	 * Sync signals are driven on the falling edge of the pixel clock
	 */
	DRM_BUS_FLAG_SYNC_DRIVE_NEGEDGE = BIT(7),

	/**
	 * @DRM_BUS_FLAG_SYNC_SAMPLE_POSEDGE:
	 *
	 * Sync signals are sampled on the rising edge of the pixel clock
	 */
	DRM_BUS_FLAG_SYNC_SAMPLE_POSEDGE = DRM_BUS_FLAG_SYNC_DRIVE_NEGEDGE,

	/**
	 * @DRM_BUS_FLAG_SYNC_SAMPLE_NEGEDGE:
	 *
	 * Sync signals are sampled on the falling edge of the pixel clock
	 */
	DRM_BUS_FLAG_SYNC_SAMPLE_NEGEDGE = DRM_BUS_FLAG_SYNC_DRIVE_POSEDGE,

	/**
	 * @DRM_BUS_FLAG_SHARP_SIGNALS:
	 *
	 *  Set if the Sharp-specific signals (SPL, CLS, PS, REV) must be used
	 */
	DRM_BUS_FLAG_SHARP_SIGNALS = BIT(8),
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
