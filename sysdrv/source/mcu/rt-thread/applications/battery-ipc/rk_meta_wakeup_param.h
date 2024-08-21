// Copyright 2023 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __RK_META_WAKEUP_PARAM_
#define __RK_META_WAKEUP_PARAM_
struct wakeup_param_info {
	uint32_t head;
	int32_t len;
	int32_t wakeup_mode;    // 0: pmu timer out, 1: gpio wakeup
	int32_t ae_wakeup_mode; // 0: use AE. 1: no use AE. 3: sleep. 4: mcu no run
	int32_t mcu_run_count;
	int32_t mcu_max_run_count;
	int32_t arm_run_count;
	int32_t arm_max_run_count;

	uint32_t crc32;
};
#endif // #ifndef __RK_META_WAKEUP_PARAM_
