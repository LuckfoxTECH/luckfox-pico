// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright © 1997-2003 by The XFree86 Project, Inc.
 * Copyright © 2007 Dave Airlie
 * Copyright © 2007-2008 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
 * Copyright 2005-2006 Luc Verhaegen
 * Copyright (c) 2001, Andy Ritger  aritger@nvidia.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

#include <common.h>
#include <drm_modes.h>
#include <linux/compat.h>
#include <malloc.h>

#define PICOS2KHZ(a)	(1000000000UL / (a))
#define KHZ2PICOS(a)	(1000000000UL / (a))

/**
 * drm_mode_create - create a new display mode
 *
 * Create a new, cleared drm_display_mode.
 *
 * Returns:
 * Pointer to new mode on success, NULL on error.
 */
struct drm_display_mode *drm_mode_create(void)
{
	struct drm_display_mode *nmode;

	nmode = malloc(sizeof(struct drm_display_mode));
	memset(nmode, 0, sizeof(struct drm_display_mode));
	if (!nmode)
		return NULL;

	return nmode;
}

/**
 * drm_mode_destroy - remove a mode
 * @mode: mode to remove
 */
void drm_mode_destroy(struct drm_display_mode *mode)
{
	if (!mode)
		return;

	kfree(mode);
}

static bool drm_mode_match_timings(const struct drm_display_mode *mode1,
				   const struct drm_display_mode *mode2)
{
	return mode1->hdisplay == mode2->hdisplay &&
	       mode1->hsync_start == mode2->hsync_start &&
	       mode1->hsync_end == mode2->hsync_end &&
	       mode1->htotal == mode2->htotal &&
	       mode1->hskew == mode2->hskew &&
	       mode1->vdisplay == mode2->vdisplay &&
	       mode1->vsync_start == mode2->vsync_start &&
	       mode1->vsync_end == mode2->vsync_end &&
	       mode1->vtotal == mode2->vtotal &&
	       mode1->vscan == mode2->vscan;
}

static bool drm_mode_match_clock(const struct drm_display_mode *mode1,
				 const struct drm_display_mode *mode2)
{
	/*
	 * do clock check convert to PICOS
	 * so fb modes get matched the same
	 */
	if (mode1->clock && mode2->clock)
		return KHZ2PICOS(mode1->clock) == KHZ2PICOS(mode2->clock);
	else
		return mode1->clock == mode2->clock;
}

static bool drm_mode_match_flags(const struct drm_display_mode *mode1,
				 const struct drm_display_mode *mode2)
{
	return (mode1->flags & ~DRM_MODE_FLAG_3D_MASK) ==
	       (mode2->flags & ~DRM_MODE_FLAG_3D_MASK);
}

static bool drm_mode_match_3d_flags(const struct drm_display_mode *mode1,
				    const struct drm_display_mode *mode2)
{
	return (mode1->flags & DRM_MODE_FLAG_3D_MASK) ==
	       (mode2->flags & DRM_MODE_FLAG_3D_MASK);
}

static bool drm_mode_match_aspect_ratio(const struct drm_display_mode *mode1,
					const struct drm_display_mode *mode2)
{
	return mode1->picture_aspect_ratio == mode2->picture_aspect_ratio;
}

/**
 * drm_mode_match - test modes for (partial) equality
 * @mode1: first mode
 * @mode2: second mode
 * @match_flags: which parts need to match (DRM_MODE_MATCH_*)
 *
 * Check to see if @mode1 and @mode2 are equivalent.
 *
 * Returns:
 * True if the modes are (partially) equal, false otherwise.
 */
bool drm_mode_match(const struct drm_display_mode *mode1,
		    const struct drm_display_mode *mode2,
		    unsigned int match_flags)
{
	if (!mode1 && !mode2)
		return true;

	if (!mode1 || !mode2)
		return false;

	if (match_flags & DRM_MODE_MATCH_TIMINGS &&
	    !drm_mode_match_timings(mode1, mode2))
		return false;

	if (match_flags & DRM_MODE_MATCH_CLOCK &&
	    !drm_mode_match_clock(mode1, mode2))
		return false;

	if (match_flags & DRM_MODE_MATCH_FLAGS &&
	    !drm_mode_match_flags(mode1, mode2))
		return false;

	if (match_flags & DRM_MODE_MATCH_3D_FLAGS &&
	    !drm_mode_match_3d_flags(mode1, mode2))
		return false;

	if (match_flags & DRM_MODE_MATCH_ASPECT_RATIO &&
	    !drm_mode_match_aspect_ratio(mode1, mode2))
		return false;

	return true;
}

/**
 * drm_mode_equal - test modes for equality
 * @mode1: first mode
 * @mode2: second mode
 *
 * Check to see if @mode1 and @mode2 are equivalent.
 *
 * Returns:
 * True if the modes are equal, false otherwise.
 */
bool drm_mode_equal(const struct drm_display_mode *mode1,
		    const struct drm_display_mode *mode2)
{
	return drm_mode_match(mode1, mode2,
			      DRM_MODE_MATCH_TIMINGS |
			      DRM_MODE_MATCH_CLOCK |
			      DRM_MODE_MATCH_FLAGS |
			      DRM_MODE_MATCH_3D_FLAGS |
			      DRM_MODE_MATCH_ASPECT_RATIO);
}

/**
 * drm_display_mode_to_videomode - fill in @vm using @dmode,
 * @dmode: drm_display_mode structure to use as source
 * @vm: videomode structure to use as destination
 *
 * Fills out @vm using the display mode specified in @dmode.
 */
void drm_display_mode_to_videomode(const struct drm_display_mode *dmode,
				   struct videomode *vm)
{
	vm->hactive = dmode->hdisplay;
	vm->hfront_porch = dmode->hsync_start - dmode->hdisplay;
	vm->hsync_len = dmode->hsync_end - dmode->hsync_start;
	vm->hback_porch = dmode->htotal - dmode->hsync_end;

	vm->vactive = dmode->vdisplay;
	vm->vfront_porch = dmode->vsync_start - dmode->vdisplay;
	vm->vsync_len = dmode->vsync_end - dmode->vsync_start;
	vm->vback_porch = dmode->vtotal - dmode->vsync_end;

	vm->pixelclock = dmode->clock * 1000;

	vm->flags = 0;
	if (dmode->flags & DRM_MODE_FLAG_PHSYNC)
		vm->flags |= DISPLAY_FLAGS_HSYNC_HIGH;
	else if (dmode->flags & DRM_MODE_FLAG_NHSYNC)
		vm->flags |= DISPLAY_FLAGS_HSYNC_LOW;
	if (dmode->flags & DRM_MODE_FLAG_PVSYNC)
		vm->flags |= DISPLAY_FLAGS_VSYNC_HIGH;
	else if (dmode->flags & DRM_MODE_FLAG_NVSYNC)
		vm->flags |= DISPLAY_FLAGS_VSYNC_LOW;
	if (dmode->flags & DRM_MODE_FLAG_INTERLACE)
		vm->flags |= DISPLAY_FLAGS_INTERLACED;
	if (dmode->flags & DRM_MODE_FLAG_DBLSCAN)
		vm->flags |= DISPLAY_FLAGS_DOUBLESCAN;
	if (dmode->flags & DRM_MODE_FLAG_DBLCLK)
		vm->flags |= DISPLAY_FLAGS_DOUBLECLK;
}
