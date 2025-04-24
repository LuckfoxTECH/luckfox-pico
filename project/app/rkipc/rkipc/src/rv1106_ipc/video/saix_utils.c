/*
 * Copyright (c) 2025 NeuralSense AI Private Limited
 * Trading as swatah.ai. All rights reserved.
 *
 * This file is part of the swatah.ai software stack and is licensed under
 * the terms defined in the accompanying LICENSE file. Unauthorized copying,
 * distribution, or modification of this file, via any medium, is strictly prohibited.
 *
 * For more information, visit: https://swatah.ai
*/

#include "saix_utils.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "saix_cutils"

int rga_nv12_border(rga_buffer_t buf, int x, int y, int width, int height, int line_pixel,
	int color) {
im_rect rect_up = {x, y, width, line_pixel};
im_rect rect_buttom = {x, y + height - line_pixel, width, line_pixel};
im_rect rect_left = {x, y, line_pixel, height};
im_rect rect_right = {x + width - line_pixel, y, line_pixel, height};
IM_STATUS STATUS = imfill(buf, rect_up, color);
STATUS |= imfill(buf, rect_buttom, color);
STATUS |= imfill(buf, rect_left, color);
STATUS |= imfill(buf, rect_right, color);
return STATUS == IM_STATUS_SUCCESS ? 0 : 1;
}