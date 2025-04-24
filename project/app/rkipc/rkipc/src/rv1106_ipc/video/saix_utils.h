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

#ifndef _SAIX_UTILS_H_
#define _SAIX_UTILS_H_
#include <rga/im2d.h>

int rga_nv12_border(rga_buffer_t buf, int x, int y, int width, int height, int line_pixel,
	int color);

#endif /* _SAIX_UTILS_H_ */