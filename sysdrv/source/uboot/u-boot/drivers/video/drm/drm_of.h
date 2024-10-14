/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd
 *
 */
#ifndef __DRM_OF_H__
#define __DRM_OF_H__

#include <dm/ofnode.h>
#include <dm/of_access.h>

/**
 * enum drm_lvds_dual_link_pixels - Pixel order of an LVDS dual-link connection
 * @DRM_LVDS_DUAL_LINK_EVEN_ODD_PIXELS: Even pixels are expected to be
 *    generated from the first port, odd pixels from the second port
 * @DRM_LVDS_DUAL_LINK_ODD_EVEN_PIXELS: Odd pixels are expected to be
 *    generated from the first port, even pixels from the second port
 * @DRM_LVDS_DUAL_LINK_LEFT_RIGHT_PIXELS: Left pixels are expected to be
 *    generated from the first port, right pixels from the second port
 * @DRM_LVDS_DUAL_LINK_RIGHT_LEFT_PIXELS: Right pixels are expected to be
 *    generated from the first port, left pixels from the second port
 */
enum drm_lvds_dual_link_pixels {
	DRM_LVDS_DUAL_LINK_EVEN_ODD_PIXELS = 0,
	DRM_LVDS_DUAL_LINK_ODD_EVEN_PIXELS = 1,
	DRM_LVDS_DUAL_LINK_LEFT_RIGHT_PIXELS = 2,
	DRM_LVDS_DUAL_LINK_RIGHT_LEFT_PIXELS = 3,
};

int drm_of_lvds_get_dual_link_pixel_order(const struct device_node *port1,
					  const struct device_node *port2);

#endif
