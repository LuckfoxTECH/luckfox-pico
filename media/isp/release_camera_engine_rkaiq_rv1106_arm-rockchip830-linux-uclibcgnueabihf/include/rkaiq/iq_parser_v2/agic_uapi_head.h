/*
 *  Copyright (c) 2021 Rockchip Corporation
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
 */

#ifndef __AGIC_UAPI_HEAD_H__
#define __AGIC_UAPI_HEAD_H__

#include "stdint.h"
#include "rk_aiq_comm.h"

typedef struct rkaiq_gic_v1_param_selected_s {
    // M4_NUMBER_DESC("ISO", "u32", M4_RANGE(50, 65535), "50", M4_DIGIT(0))
    uint32_t iso;
    // M4_BOOL_DESC("bypass", "0")
    uint8_t bypass;
    // M4_NUMBER_DESC("gr_ratio", "u8", M4_RANGE(0, 3), "0", M4_DIGIT(0))
    uint8_t gr_ratio;
    // M4_NUMBER_DESC("min_busy_thre", "u16", M4_RANGE(0, 1023), "160", M4_DIGIT(0))
    uint16_t min_busy_thre;
    // M4_NUMBER_DESC("min_grad_thr1", "u16", M4_RANGE(0, 1023), "32", M4_DIGIT(0))
    uint16_t min_grad_thr1;
    // M4_NUMBER_DESC("min_grad_thr2", "u16", M4_RANGE(0, 1023), "32", M4_DIGIT(0))
    uint16_t min_grad_thr2;
    // M4_NUMBER_DESC("k_grad1", "u16", M4_RANGE(0, 15), "5", M4_DIGIT(0))
    uint16_t k_grad1;
    // M4_NUMBER_DESC("k_grad2", "u16", M4_RANGE(0, 15), "5", M4_DIGIT(0))
    uint16_t k_grad2;
    // M4_NUMBER_DESC("gb_thre", "u16", M4_RANGE(0, 15), "7", M4_DIGIT(0))
    uint16_t gb_thre;
    // M4_NUMBER_DESC("maxCorV", "u16", M4_RANGE(0, 1023), "40", M4_DIGIT(0))
    uint16_t maxCorV;
    // M4_NUMBER_DESC("maxCorVboth", "u16", M4_RANGE(0, 1023), "8", M4_DIGIT(0))
    uint16_t maxCorVboth;
    // M4_NUMBER_DESC("dark_thre", "u16", M4_RANGE(0, 2047), "120", M4_DIGIT(0))
    uint16_t dark_thre;
    // M4_NUMBER_DESC("dark_threHi", "u16", M4_RANGE(0, 2047), "240", M4_DIGIT(0))
    uint16_t dark_threHi;
    // M4_NUMBER_DESC("k_grad1_dark", "u16", M4_RANGE(0, 15), "6", M4_DIGIT(0))
    uint16_t k_grad1_dark;
    // M4_NUMBER_DESC("k_grad2_dark", "u16", M4_RANGE(0, 15), "1", M4_DIGIT(0))
    uint16_t k_grad2_dark;
    // M4_NUMBER_DESC("min_grad_thr_dark1", "u16", M4_RANGE(0, 1023), "64", M4_DIGIT(0))
    uint16_t min_grad_thr_dark1;
    // M4_NUMBER_DESC("min_grad_thr_dark2", "u16", M4_RANGE(0, 1023), "32", M4_DIGIT(0))
    uint16_t min_grad_thr_dark2;
    // M4_NUMBER_DESC("noiseCurve_0", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(4))
    float noiseCurve_0;
    // M4_NUMBER_DESC("noiseCurve_1", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(4))
    float noiseCurve_1;
    // M4_NUMBER_DESC("GValueLimitLo", "f32", M4_RANGE(0.0, 4095.0), "1280.0", M4_DIGIT(4))
    float GValueLimitLo;
    // M4_NUMBER_DESC("GValueLimitHi", "f32", M4_RANGE(0.0, 4095.0), "1760.0", M4_DIGIT(4))
    float GValueLimitHi;
    // M4_NUMBER_DESC("textureStrength", "f32", M4_RANGE(0.0, 2.0), "1.0", M4_DIGIT(4))
    float textureStrength;
    // M4_NUMBER_DESC("ScaleLo", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(4))
    float ScaleLo;
    // M4_NUMBER_DESC("ScaleHi", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(4))
    float ScaleHi;
    // M4_NUMBER_DESC("globalStrength", "f32", M4_RANGE(0.0, 2.0), "1.0", M4_DIGIT(4))
    float globalStrength;
    // M4_NUMBER_DESC("Difference Clip", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(4))
    uint16_t diff_clip;
} rkaiq_gic_v1_param_selected_t;

typedef struct rkaiq_gic_v2_param_selected_s {
    // M4_NUMBER_DESC("ISO", "u32", M4_RANGE(50, 65535), "50", M4_DIGIT(0))
    uint32_t iso;
    // M4_BOOL_DESC("bypass", "0")
    uint8_t bypass;
    // M4_NUMBER_DESC("gr_ratio", "u8", M4_RANGE(0, 3), "0", M4_DIGIT(0))
    uint8_t gr_ratio;
    // M4_NUMBER_DESC("min_busy_thre", "u16", M4_RANGE(0, 1023), "160", M4_DIGIT(0))
    uint16_t min_busy_thre;
    // M4_NUMBER_DESC("min_grad_thr1", "u16", M4_RANGE(0, 1023), "32", M4_DIGIT(0))
    uint16_t min_grad_thr1;
    // M4_NUMBER_DESC("min_grad_thr2", "u16", M4_RANGE(0, 1023), "32", M4_DIGIT(0))
    uint16_t min_grad_thr2;
    // M4_NUMBER_DESC("k_grad1", "u16", M4_RANGE(0, 15), "5", M4_DIGIT(0))
    uint16_t k_grad1;
    // M4_NUMBER_DESC("k_grad2", "u16", M4_RANGE(0, 15), "5", M4_DIGIT(0))
    uint16_t k_grad2;
    // M4_NUMBER_DESC("gb_thre", "u16", M4_RANGE(0, 15), "7", M4_DIGIT(0))
    uint16_t gb_thre;
    // M4_NUMBER_DESC("maxCorV", "u16", M4_RANGE(0, 1023), "40", M4_DIGIT(0))
    uint16_t maxCorV;
    // M4_NUMBER_DESC("maxCorVboth", "u16", M4_RANGE(0, 1023), "8", M4_DIGIT(0))
    uint16_t maxCorVboth;
    // M4_NUMBER_DESC("dark_thre", "u16", M4_RANGE(0, 2047), "120", M4_DIGIT(0))
    uint16_t dark_thre;
    // M4_NUMBER_DESC("dark_threHi", "u16", M4_RANGE(0, 2047), "240", M4_DIGIT(0))
    uint16_t dark_threHi;
    // M4_NUMBER_DESC("k_grad1_dark", "u16", M4_RANGE(0, 15), "6", M4_DIGIT(0))
    uint16_t k_grad1_dark;
    // M4_NUMBER_DESC("k_grad2_dark", "u16", M4_RANGE(0, 15), "1", M4_DIGIT(0))
    uint16_t k_grad2_dark;
    // M4_NUMBER_DESC("min_grad_thr_dark1", "u16", M4_RANGE(0, 1023), "64", M4_DIGIT(0))
    uint16_t min_grad_thr_dark1;
    // M4_NUMBER_DESC("min_grad_thr_dark2", "u16", M4_RANGE(0, 1023), "32", M4_DIGIT(0))
    uint16_t min_grad_thr_dark2;
    // M4_NUMBER_DESC("NoiseScale", "s32", M4_RANGE(0, 1023), "0", M4_DIGIT(0))
    int NoiseScale;
    // M4_NUMBER_DESC("NoiseBase", "s32", M4_RANGE(0, 1023), "0", M4_DIGIT(0))
    int NoiseBase;
    // M4_NUMBER_DESC("noiseCurve_0", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(4))
    float noiseCurve_0;
    // M4_NUMBER_DESC("noiseCurve_1", "f32", M4_RANGE(0.0, 1023.0), "0.0", M4_DIGIT(4))
    float noiseCurve_1;
    // M4_NUMBER_DESC("globalStrength", "f32", M4_RANGE(0.0, 2.0), "1.0", M4_DIGIT(4))
    float globalStrength;
    // M4_NUMBER_DESC("diff_clip", "u16", M4_RANGE(0, 1023), "0", M4_DIGIT(0))
    uint16_t diff_clip;
} rkaiq_gic_v2_param_selected_t;

enum { GIC_NORMAL = 0, GIC_HDR = 1, GIC_NIGHT = 2 };

typedef enum rkaiq_gic_api_op_mode_e {
    RKAIQ_GIC_API_OPMODE_OFF    = 0,  // run IQ agic
    RKAIQ_GIC_API_OPMODE_AUTO   = 1,  // run api auto agic
    RKAIQ_GIC_API_OPMODE_MANUAL = 2,  // run api manual agic
} rkaiq_gic_api_op_mode_t;

#define RKAIQ_GIC_MAX_ISO_CNT 16

typedef struct rkaiq_gic_v1_api_attr_s {
    rk_aiq_uapi_sync_t sync;
    // M4_BOOL_DESC("enable", "0")
    uint8_t gic_en;
    // M4_BOOL_DESC("edge_open", "0")
    uint8_t edge_open;
    // M4_BOOL_DESC("noise_cut_en", "0")
    uint8_t noise_cut_en;
    // M4_ENUM_DESC("op_mode", "rkaiq_gic_api_op_mode_t", "RKAIQ_GIC_API_OPMODE_AUTO");
    rkaiq_gic_api_op_mode_t op_mode;
    // M4_NUMBER_DESC("iso count", "u32", M4_RANGE(1, 16), "16", M4_DIGIT(0))
    uint32_t iso_cnt;
    // M4_STRUCT_LIST_DESC("auto params", M4_SIZE(1, 16), "normal_ui_style")
    rkaiq_gic_v1_param_selected_t auto_params[RKAIQ_GIC_MAX_ISO_CNT];
    // M4_STRUCT_DESC("manual params", "normal_ui_style")
    rkaiq_gic_v1_param_selected_t manual_param;
} rkaiq_gic_v1_api_attr_t;

typedef struct rkaiq_gic_v2_api_attr_s {
    rk_aiq_uapi_sync_t sync;
    // M4_BOOL_DESC("enable", "0")
    uint8_t gic_en;
    // M4_ENUM_DESC("op_mode", "rkaiq_gic_api_op_mode_t", "RKAIQ_GIC_API_OPMODE_AUTO");
    rkaiq_gic_api_op_mode_t op_mode;
    // M4_NUMBER_DESC("iso count", "u32", M4_RANGE(1, 16), "16", M4_DIGIT(0))
    uint32_t iso_cnt;
    // M4_STRUCT_LIST_DESC("auto params", M4_SIZE(1, 16), "normal_ui_style")
    rkaiq_gic_v2_param_selected_t auto_params[RKAIQ_GIC_MAX_ISO_CNT];
    // M4_STRUCT_DESC("manual params", "normal_ui_style")
    rkaiq_gic_v2_param_selected_t manual_param;
} rkaiq_gic_v2_api_attr_t;

#endif  /*__AGIC_UAPI_HEAD_H__*/
