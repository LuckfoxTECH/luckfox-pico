/*
 * V4L2 subdev interface library
 *
 * Copyright (C) 2010-2014 Ideas on board SPRL
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

struct media_device;
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
int rk_v4l2_subdev_open(struct media_entity* entity);

/**
 * @brief Close a sub-device.
 * @param entity - sub-device media entity.
 *
 * Close the V4L2 subdev device node associated with the @a entity and opened by
 * a previous call to v4l2_subdev_open() (either explicit or implicit).
 */
void rk_v4l2_subdev_close(struct media_entity* entity);

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
int rk_v4l2_subdev_get_format(struct media_entity* entity, struct v4l2_mbus_framefmt* format, unsigned int pad,
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
int rk_v4l2_subdev_set_format(struct media_entity* entity, struct v4l2_mbus_framefmt* format, unsigned int pad,
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
int rk_v4l2_subdev_get_selection(struct media_entity* entity, struct v4l2_rect* rect, unsigned int pad,
                              unsigned int target, enum v4l2_subdev_format_whence which);

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
int rk_v4l2_subdev_set_selection(struct media_entity* entity, struct v4l2_rect* rect, unsigned int pad,
                              unsigned int target, enum v4l2_subdev_format_whence which);

/**
 * @brief Query the digital video capabilities of a pad.
 * @param entity - subdev-device media entity.
 * @param cap - capabilities to be filled.
 *
 * Retrieve the digital video capabilities of the @a entity pad specified by
 * @a cap.pad and store it in the @a cap structure.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int rk_v4l2_subdev_get_dv_timings_caps(struct media_entity* entity, struct v4l2_dv_timings_cap* caps);

/**
 * @brief Query the digital video timings of a sub-device
 * @param entity - subdev-device media entity.
 * @param timings timings to be filled.
 *
 * Retrieve the detected digital video timings for the currently selected input
 * of @a entity and store them in the @a timings structure.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int rk_v4l2_subdev_query_dv_timings(struct media_entity* entity, struct v4l2_dv_timings* timings);

/**
 * @brief Get the current digital video timings of a sub-device
 * @param entity - subdev-device media entity.
 * @param timings timings to be filled.
 *
 * Retrieve the current digital video timings for the currently selected input
 * of @a entity and store them in the @a timings structure.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int rk_v4l2_subdev_get_dv_timings(struct media_entity* entity, struct v4l2_dv_timings* timings);

/**
 * @brief Set the digital video timings of a sub-device
 * @param entity - subdev-device media entity.
 * @param timings timings to be set.
 *
 * Set the digital video timings of @a entity to @a timings. The driver is
 * allowed to modify the requested format, in which case @a timings is updated
 * with the modifications.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int rk_v4l2_subdev_set_dv_timings(struct media_entity* entity, struct v4l2_dv_timings* timings);

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

int rk_v4l2_subdev_get_frame_interval(struct media_entity* entity, struct v4l2_fract* interval, unsigned int pad);

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
int rk_v4l2_subdev_set_frame_interval(struct media_entity* entity, struct v4l2_fract* interval, unsigned int pad);

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
int rk_v4l2_subdev_parse_setup_formats(struct media_device* media, const char* p);

/**
 * @brief Convert media bus pixel code to string.
 * @param code - input string
 *
 * Convert media bus pixel code @a code to a human-readable string.
 *
 * @return A pointer to a string on success, NULL on failure.
 */
const char* rk_v4l2_subdev_pixelcode_to_string(enum v4l2_mbus_pixelcode code);

/**
 * @brief Parse string to media bus pixel code.
 * @param string - nul terminalted string, textual media bus pixel code
 *
 * Parse human readable string @a string to an media bus pixel code.
 *
 * @return media bus pixelcode on success, -1 on failure.
 */
enum v4l2_mbus_pixelcode rk_v4l2_subdev_string_to_pixelcode(const char* string);

/**
 * @brief Convert a field order to string.
 * @param field - field order
 *
 * Convert field order @a field to a human-readable string.
 *
 * @return A pointer to a string on success, NULL on failure.
 */
const char* rk_v4l2_subdev_field_to_string(enum v4l2_field field);

/**
 * @brief Parse string to field order.
 * @param string - nul terminated string, textual media bus pixel code
 *
 * Parse human readable string @a string to field order.
 *
 * @return field order on success, -1 on failure.
 */
enum v4l2_field rk_v4l2_subdev_string_to_field(const char* string);

/**
 * @brief Convert a colorspace to string.
 * @param colorspace - colorspace
 *
 * Convert colorspace @a colorspace to a human-readable string.
 *
 * @return A pointer to a string on success, NULL on failure.
 */
const char* rk_v4l2_subdev_colorspace_to_string(enum v4l2_colorspace colorspace);

/**
 * @brief Parse string to colorspace.
 * @param string - nul terminated string, textual colorspace
 *
 * Parse human readable string @a string to colorspace.
 *
 * @return colorspace on success, -1 on failure.
 */
enum v4l2_colorspace rk_v4l2_subdev_string_to_colorspace(const char* string);

/**
 * @brief Convert a transfer function to string.
 * @param xfer_func - transfer function
 *
 * Convert transfer function @a xfer_func to a human-readable string.
 *
 * @return A pointer to a string on success, NULL on failure.
 */
const char* rk_v4l2_subdev_xfer_func_to_string(enum v4l2_xfer_func xfer_func);

/**
 * @brief Parse string to transfer function.
 * @param string - nul terminated string, textual transfer function
 *
 * Parse human readable string @a string to xfer_func.
 *
 * @return xfer_func on success, -1 on failure.
 */
enum v4l2_xfer_func rk_v4l2_subdev_string_to_xfer_func(const char* string);

/**
 * @brief Convert a YCbCr encoding to string.
 * @param ycbcr_enc - YCbCr encoding
 *
 * Convert YCbCr encoding @a ycbcr_enc to a human-readable string.
 *
 * @return A pointer to a string on success, NULL on failure.
 */
const char* rk_v4l2_subdev_ycbcr_encoding_to_string(enum v4l2_ycbcr_encoding ycbcr_enc);

/**
 * @brief Parse string to YCbCr encoding.
 * @param string - nul terminated string, textual YCbCr encoding
 *
 * Parse human readable string @a string to YCbCr encoding.
 *
 * @return ycbcr_enc on success, -1 on failure.
 */
enum v4l2_ycbcr_encoding rk_v4l2_subdev_string_to_ycbcr_encoding(const char* string);

/**
 * @brief Convert a quantization to string.
 * @param quantization - quantization
 *
 * Convert quantization @a quantization to a human-readable string.
 *
 * @return A pointer to a string on success, NULL on failure.
 */
const char* rk_v4l2_subdev_quantization_to_string(enum v4l2_quantization quantization);

/**
 * @brief Parse string to quantization.
 * @param string - nul terminated string, textual quantization
 *
 * Parse human readable string @a string to quantization.
 *
 * @return quantization on success, -1 on failure.
 */
enum v4l2_quantization rk_v4l2_subdev_string_to_quantization(const char* string);

/**
 * @brief Enumerate library supported media bus pixel codes.
 * @param length - the number of the supported pixel codes
 *
 * Obtain pixel codes supported by libv4l2subdev.
 *
 * @return A pointer to the pixel code array
 */
const enum v4l2_mbus_pixelcode* rk_v4l2_subdev_pixelcode_list(unsigned int* length);

#ifdef __cplusplus
}
#endif

#endif
