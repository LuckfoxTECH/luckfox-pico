/*
 * rk_aiq_types_acac_hw.h
 *
 *  Copyright (c) 2021 Rockchip Electronics Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Cody Xie <cody.xie@rock-chips.com>
 */

#ifndef _RK_AIQ_TYPE_ACAC_HW_H_
#define _RK_AIQ_TYPE_ACAC_HW_H_

#include <stdint.h>

#include "rk_aiq_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ISP3X_CAC_STRENGTH_NUM
#define ISP3X_CAC_STRENGTH_NUM 22
#endif

#ifndef ISP32_CAC_STRENGTH_NUM
#define ISP32_CAC_STRENGTH_NUM ISP3X_CAC_STRENGTH_NUM
#endif

typedef struct rkaiq_cac_v10_hw_param_s {
    uint8_t bypass_en;
    uint8_t center_en;

    uint8_t psf_sft_bit;
    uint16_t cfg_num;

    uint16_t center_width;
    uint16_t center_height;

    uint16_t strength[ISP3X_CAC_STRENGTH_NUM];

    uint32_t hsize;
    uint32_t vsize;
    int32_t buf_fd;
} __attribute__((packed)) rkaiq_cac_v10_hw_param_t;

typedef struct rkaiq_cac_v11_hw_param_s {
    uint8_t bypass_en;
    uint8_t center_en;
    uint8_t clip_g_mode;
    uint8_t edge_detect_en;
    uint8_t neg_clip0_en;

    uint8_t flat_thed_b;
    uint8_t flat_thed_r;

    uint8_t psf_sft_bit;
    uint16_t cfg_num;

    uint16_t center_width;
    uint16_t center_height;

    uint16_t strength[ISP32_CAC_STRENGTH_NUM];

    uint16_t offset_b;
    uint16_t offset_r;

    uint32_t expo_thed_b;
    uint32_t expo_thed_r;
    uint32_t expo_adj_b;
    uint32_t expo_adj_r;

    uint32_t hsize;
    uint32_t vsize;

    int32_t buf_fd;
} __attribute__((packed)) rkaiq_cac_v11_hw_param_t;

#ifdef __cplusplus
}
#endif

#endif
