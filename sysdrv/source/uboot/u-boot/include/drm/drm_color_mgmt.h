/*
 * Copyright (c) 2016 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#ifndef __DRM_COLOR_MGMT_H__
#define __DRM_COLOR_MGMT_H__

enum drm_color_encoding {
	DRM_COLOR_YCBCR_BT601,
	DRM_COLOR_YCBCR_BT709,
	DRM_COLOR_YCBCR_BT2020,
	DRM_COLOR_ENCODING_MAX,
};

enum drm_color_range {
	DRM_COLOR_YCBCR_LIMITED_RANGE,
	DRM_COLOR_YCBCR_FULL_RANGE,
	DRM_COLOR_RANGE_MAX,
};

/**
 * enum drm_color_lut_tests - hw-specific LUT tests to perform
 *
 * The drm_color_lut_check() function takes a bitmask of the values here to
 * determine which tests to apply to a userspace-provided LUT.
 */
enum drm_color_lut_tests {
	/**
	 * @DRM_COLOR_LUT_EQUAL_CHANNELS:
	 *
	 * Checks whether the entries of a LUT all have equal values for the
	 * red, green, and blue channels.  Intended for hardware that only
	 * accepts a single value per LUT entry and assumes that value applies
	 * to all three color components.
	 */
	DRM_COLOR_LUT_EQUAL_CHANNELS = BIT(0),

	/**
	 * @DRM_COLOR_LUT_NON_DECREASING:
	 *
	 * Checks whether the entries of a LUT are always flat or increasing
	 * (never decreasing).
	 */
	DRM_COLOR_LUT_NON_DECREASING = BIT(1),
};

#endif /* __DRM_COLOR_MGMT_H__ */
