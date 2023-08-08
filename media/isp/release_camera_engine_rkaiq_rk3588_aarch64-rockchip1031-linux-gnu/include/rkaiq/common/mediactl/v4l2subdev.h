/*
 * V4L2 subdev interface library
 *
 * Copyright (C) 2010-2011 Ideas on board SPRL
 *
 * Contact: Laurent Pinchart <laurent.pinchart@ideasonboard.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SUBDEV_H__
#define __SUBDEV_H__

#include <linux/v4l2-subdev.h>

#ifdef __cplusplus
extern "C" {
#endif

struct media_entity;

/**
 * @brief Open a sub-device.
 * @param entity - sub-device media entity.
 *
 * Open the V4L2 subdev device node associated with @a entity. The file
 * descriptor is stored in the media_entity structure.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int v4l2_subdev_open(struct media_entity *entity);

/**
 * @brief Close a sub-device.
 * @param entity - sub-device media entity.
 *
 * Close the V4L2 subdev device node associated with the @a entity and opened by
 * a previous call to v4l2_subdev_open() (either explicit or implicit).
 */
void v4l2_subdev_close(struct media_entity *entity);

/**
 * @brief Retrieve the format on a pad.
 * @param entity - subdev-device media entity.
 * @param format - format to be filled.
 * @param pad - pad number.
 * @param which - identifier of the format to get.
 *
 * Retrieve the current format on the @a entity @a pad and store it in the
 * @a format structure.
 *
 * @a which is set to V4L2_SUBDEV_FORMAT_TRY to retrieve the try format stored
 * in the file handle, of V4L2_SUBDEV_FORMAT_ACTIVE to retrieve the current
 * active format.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int v4l2_subdev_get_format(struct media_entity *entity,
                           struct v4l2_mbus_framefmt *format, unsigned int pad,
                           enum v4l2_subdev_format_whence which);

/**
 * @brief Set the format on a pad.
 * @param entity - subdev-device media entity.
 * @param format - format.
 * @param pad - pad number.
 * @param which - identifier of the format to set.
 *
 * Set the format on the @a entity @a pad to @a format. The driver is allowed to
 * modify the requested format, in which case @a format is updated with the
 * modifications.
 *
 * @a which is set to V4L2_SUBDEV_FORMAT_TRY to set the try format stored in the
 * file handle, of V4L2_SUBDEV_FORMAT_ACTIVE to configure the device with an
 * active format.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int v4l2_subdev_set_format(struct media_entity *entity,
                           struct v4l2_mbus_framefmt *format, unsigned int pad,
                           enum v4l2_subdev_format_whence which);

/**
 * @brief Retrieve a selection rectangle on a pad.
 * @param entity - subdev-device media entity.
 * @param r - rectangle to be filled.
 * @param pad - pad number.
 * @param target - selection target
 * @param which - identifier of the format to get.
 *
 * Retrieve the @a target selection rectangle on the @a entity @a pad
 * and store it in the @a rect structure.
 *
 * @a which is set to V4L2_SUBDEV_FORMAT_TRY to retrieve the try
 * selection rectangle stored in the file handle, or
 * V4L2_SUBDEV_FORMAT_ACTIVE to retrieve the current active selection
 * rectangle.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int v4l2_subdev_get_selection(struct media_entity *entity,
                              struct v4l2_rect *rect, unsigned int pad, unsigned int target,
                              enum v4l2_subdev_format_whence which);

/**
 * @brief Set a selection rectangle on a pad.
 * @param entity - subdev-device media entity.
 * @param rect - crop rectangle.
 * @param pad - pad number.
 * @param target - selection target
 * @param which - identifier of the format to set.
 *
 * Set the @a target selection rectangle on the @a entity @a pad to @a
 * rect. The driver is allowed to modify the requested rectangle, in
 * which case @a rect is updated with the modifications.
 *
 * @a which is set to V4L2_SUBDEV_FORMAT_TRY to set the try crop rectangle
 * stored in the file handle, of V4L2_SUBDEV_FORMAT_ACTIVE to configure the
 * device with an active crop rectangle.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int v4l2_subdev_set_selection(struct media_entity *entity,
                              struct v4l2_rect *rect, unsigned int pad, unsigned int target,
                              enum v4l2_subdev_format_whence which);

/**
 * @brief Retrieve the frame interval on a sub-device.
 * @param entity - subdev-device media entity.
 * @param interval - frame interval to be filled.
 *
 * Retrieve the current frame interval on subdev @a entity and store it in the
 * @a interval structure.
 *
 * Frame interval retrieving is usually supported only on devices at the
 * beginning of video pipelines, such as sensors.
 *
 * @return 0 on success, or a negative error code on failure.
 */

int v4l2_subdev_get_frame_interval(struct media_entity *entity,
                                   struct v4l2_fract *interval);

/**
 * @brief Set the frame interval on a sub-device.
 * @param entity - subdev-device media entity.
 * @param interval - frame interval.
 *
 * Set the frame interval on subdev @a entity to @a interval. The driver is
 * allowed to modify the requested frame interval, in which case @a interval is
 * updated with the modifications.
 *
 * Frame interval setting is usually supported only on devices at the beginning
 * of video pipelines, such as sensors.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int v4l2_subdev_set_frame_interval(struct media_entity *entity,
                                   struct v4l2_fract *interval);

/**
 * @brief Parse a string and apply format, crop and frame interval settings.
 * @param media - media device.
 * @param p - input string
 * @param endp - pointer to string p where parsing ended (return)
 *
 * Parse string @a p and apply format, crop and frame interval settings to a
 * subdev pad specified in @a p. @a endp will be written a pointer where
 * parsing of @a p ended.
 *
 * Format strings are separeted by commas (,).
 *
 * @return 0 on success, or a negative error code on failure.
 */
int v4l2_subdev_parse_setup_formats(struct media_device *media, const char *p);

/**
 * @brief Convert media bus pixel code to string.
 * @param code - input string
 *
 * Convert media bus pixel code @a code to a human-readable string.
 *
 * @return A pointer to a string on success, NULL on failure.
 */
const char *v4l2_subdev_pixelcode_to_string(enum v4l2_mbus_pixelcode code);

/**
 * @brief Parse string to media bus pixel code.
 * @param string - input string
 * @param lenght - length of the string
 *
 * Parse human readable string @a string to an media bus pixel code.
 *
 * @return media bus pixelcode on success, -1 on failure.
 */
enum v4l2_mbus_pixelcode v4l2_subdev_string_to_pixelcode(const char *string,
        unsigned int length);
#ifdef __cplusplus
}
#endif
#endif
