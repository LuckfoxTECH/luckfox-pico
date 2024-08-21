// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2023 Rockchip Electronics Co., Ltd
 *
 */
#include <dm/device.h>
#include <dm/read.h>

#include "drm_of.h"

enum drm_of_lvds_pixels {
	DRM_OF_LVDS_EVEN = BIT(0),
	DRM_OF_LVDS_ODD = BIT(1),
	DRM_OF_LVDS_LEFT = BIT(2),
	DRM_OF_LVDS_RIGHT = BIT(3),
};

static int
drm_of_lvds_get_port_pixels_type(const struct device_node *port_node)
{
	ofnode node = np_to_ofnode(port_node);

	bool even_pixels =
		ofnode_read_bool(node, "dual-lvds-even-pixels");
	bool odd_pixels =
		ofnode_read_bool(node, "dual-lvds-odd-pixels");
	bool left_pixels =
		ofnode_read_bool(node, "dual-lvds-left-pixels");
	bool right_pixels =
		ofnode_read_bool(node, "dual-lvds-right-pixels");

	return (even_pixels ? DRM_OF_LVDS_EVEN : 0) |
		   (odd_pixels ? DRM_OF_LVDS_ODD : 0) |
		   (left_pixels ? DRM_OF_LVDS_LEFT : 0) |
		   (right_pixels ? DRM_OF_LVDS_RIGHT : 0);
}

static int
drm_of_lvds_get_remote_pixels_type(const struct device_node *port_node)
{
	ofnode node = np_to_ofnode(port_node);
	ofnode endpoint;
	uint phandle;
	int pixels_type = -EPIPE;

	ofnode_for_each_subnode(endpoint, node) {
		int current_pt;
		const char *name;

		if (!ofnode_is_available(endpoint))
			continue;

		name = ofnode_get_name(endpoint);
		if (strncmp(name, "endpoint", 8) != 0)
			continue;

		if (ofnode_read_u32(endpoint, "remote-endpoint", &phandle))
			continue;

		endpoint = ofnode_get_by_phandle(phandle);
		if (!ofnode_valid(endpoint) || !ofnode_is_available(endpoint))
			continue;

		endpoint = ofnode_get_parent(endpoint);
		if (!ofnode_valid(endpoint))
			continue;

		current_pt =
			drm_of_lvds_get_port_pixels_type(ofnode_to_np(endpoint
				));
		if (pixels_type < 0)
			pixels_type = current_pt;

		/*
		 * Sanity check, ensure that all remote endpoints have the same
		 * pixel type. We may lift this restriction later if we need to
		 * support multiple sinks with different dual-link
		 * configurations by passing the endpoints explicitly to
		 * drm_of_lvds_get_dual_link_pixel_order().
		 */
		if (!current_pt || pixels_type != current_pt)
			return -EINVAL;
	}

	return pixels_type;
}

/**
 * drm_of_lvds_get_dual_link_pixel_order - Get LVDS dual-link pixel order
 * @port1: First DT port node of the Dual-link LVDS source
 * @port2: Second DT port node of the Dual-link LVDS source
 *
 * An LVDS dual-link connection is made of two links, the two link can transmit
 * odd pixels and even pixels independently, or the two link can also transmit
 * left pixels and right pixels independently. This function returns for two
 * ports of an LVDS dual-link source, based on the requirements of the connected
 * sink.
 *
 * The pixel order is determined from the dual-lvds-even-pixels +
 * dual-lvds-odd-pixels or dual-lvds-left-pixels + dual-lvds-right-pixels
 * properties in the sink's DT port nodes. If those
 * properties are not present, or if their usage is not valid, this function
 * returns -EINVAL.
 *
 * If either port is not connected, this function returns -EPIPE.
 *
 * @port1 and @port2 are typically DT sibling nodes, but may have different
 * parents when, for instance, two separate LVDS encoders carry the even and
 * odd pixels.
 *
 * Return:
 * * DRM_LVDS_DUAL_LINK_EVEN_ODD_PIXELS - @port1 carries even pixels and @port2
 *   carries odd pixels
 * * DRM_LVDS_DUAL_LINK_ODD_EVEN_PIXELS - @port1 carries odd pixels and @port2
 *   carries even pixels
 * * DRM_LVDS_DUAL_LINK_LEFT_RIGHT_PIXELS - @port1 carries left pixels and
 *   @port2 carries right pixels
 * * DRM_LVDS_DUAL_LINK_RIGHT_LEFT_PIXELS - @port1 carries right pixels and
 *   @port2 carries left pixels

 * * -EINVAL - @port1 and @port2 are not connected to a dual-link LVDS sink, or
 *   the sink configuration is invalid
 * * -EPIPE - when @port1 or @port2 are not connected
 */
int drm_of_lvds_get_dual_link_pixel_order(const struct device_node *port1,
					  const struct device_node *port2)
{
	int remote_p1_pt, remote_p2_pt;

	if (!port1 || !port2)
		return -EINVAL;

	remote_p1_pt = drm_of_lvds_get_remote_pixels_type(port1);
	if (remote_p1_pt < 0)
		return remote_p1_pt;

	remote_p2_pt = drm_of_lvds_get_remote_pixels_type(port2);
	if (remote_p2_pt < 0)
		return remote_p2_pt;

	/*
	 * A valid dual-lVDS bus is found when one remote port is marked with
	 * "dual-lvds-even-pixels" or "dual-lvds-left-pixels", and the other
	 * remote port is marked with "dual-lvds-odd-pixels"or
	 * "dual-lvds-right-pixels", bail out if the markers are not right.
	 */
	if ((remote_p1_pt + remote_p2_pt !=
		DRM_OF_LVDS_EVEN + DRM_OF_LVDS_ODD) &&
		(remote_p1_pt + remote_p2_pt !=
		DRM_OF_LVDS_LEFT + DRM_OF_LVDS_RIGHT))
		return -EINVAL;

	if (remote_p1_pt == DRM_OF_LVDS_EVEN)
		return DRM_LVDS_DUAL_LINK_EVEN_ODD_PIXELS;
	else if (remote_p1_pt == DRM_OF_LVDS_ODD)
		return DRM_LVDS_DUAL_LINK_ODD_EVEN_PIXELS;
	else if (remote_p1_pt == DRM_OF_LVDS_LEFT)
		return DRM_LVDS_DUAL_LINK_LEFT_RIGHT_PIXELS;
	else
		return DRM_LVDS_DUAL_LINK_RIGHT_LEFT_PIXELS;
}
