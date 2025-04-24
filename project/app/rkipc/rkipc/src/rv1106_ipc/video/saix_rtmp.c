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

#include "saix_common_defines.h"
#include "saix_rtmp.h"

int saix_ipc_rtmp_init() {
	int ret = 0;
	ret |= rk_rtmp_init(0, RTMP_URL_0);
	ret |= rk_rtmp_init(1, RTMP_URL_1);
	// ret |= rk_rtmp_init(2, RTMP_URL_2);

	return ret;
}

int saix_ipc_rtmp_deinit() {
	int ret = 0;
	ret |= rk_rtmp_deinit(0);
	ret |= rk_rtmp_deinit(1);
	// ret |= rk_rtmp_deinit(2);

	return ret;
}
