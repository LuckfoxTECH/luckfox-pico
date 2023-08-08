/*
 * Copyright (c) 2016 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _BACKLIGHT_H
#define _BACKLIGHT_H

struct backlight_ops {
	/**
	 * enable() - Enable a backlight
	 *
	 * @dev:	Backlight device to enable
	 * @return 0 if OK, -ve on error
	 */
	int (*enable)(struct udevice *dev);

	/**
	 * disable() - Disable a backlight
	 *
	 * @dev:	Backlight device to disable
	 * @return 0 if OK, -ve on error
	 */
	int (*disable)(struct udevice *dev);
};

#define backlight_get_ops(dev)	((struct backlight_ops *)(dev)->driver->ops)

/**
 * backlight_enable() - Enable a backlight
 *
 * @dev:	Backlight device to enable
 * @return 0 if OK, -ve on error
 */
int backlight_enable(struct udevice *dev);

/**
 * backlight_disable() - Disable a backlight
 *
 * @dev:	Backlight device to disable
 * @return 0 if OK, -ve on error
 */
int backlight_disable(struct udevice *dev);

#endif
