// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _RK_TUYA_IPC_H_
#define _RK_TUYA_IPC_H_

#include "common.h"
#include "tuya_cloud_base_defs.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_g711_utils.h"
#include "tuya_ipc_api.h"
#include "tuya_ipc_media.h"
#include "tuya_ipc_p2p.h"
#include "tuya_ring_buffer.h"
#include "wifi_hwl.h"

typedef int (*rk_tuya_ao_create)();
typedef int (*rk_tuya_ao_write)(unsigned char *, int);
typedef int (*rk_tuya_ao_destroy)();
void rk_tuya_ao_create_register(rk_tuya_ao_create callback_ptr);
void rk_tuya_ao_write_register(rk_tuya_ao_write callback_ptr);
void rk_tuya_ao_destroy_register(rk_tuya_ao_destroy callback_ptr);

int rk_tuya_init();
int rk_tuya_deinit();
int rk_tuya_push_video(unsigned char *buffer, unsigned int buffer_size, int64_t present_time,
                       int nal_type);
int rk_tuya_push_audio(unsigned char *buffer, unsigned int buffer_size, int64_t present_time);
int rk_tuya_low_power_enable();
int rk_tuya_low_power_disable();

#endif
