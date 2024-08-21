/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-12-13     Jair Wu      First version
 *
 */

#ifndef DUAL_TRACK_H
#define DUAL_TRACK_H
#ifdef __cplusplus
extern "C" {
#endif

#include "audio_server.h"

typedef struct dt_recorder *dt_recorder_handle_t;
dt_recorder_handle_t dt_capture_create(recorder_cfg_t *cfg);
void dt_capture_destroy(dt_recorder_handle_t self);
void dt_capture_start(dt_recorder_handle_t self, record_cfg_t *cfg);
void dt_capture_stop(dt_recorder_handle_t self);


#ifdef __cplusplus
}
#endif
#endif
