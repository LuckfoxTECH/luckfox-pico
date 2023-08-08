/*
 * agic_head.h
 *
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

#ifndef __CALIBDBV2_AGIC_HEAD_H__
#define __CALIBDBV2_AGIC_HEAD_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

typedef struct Gic_setting_v20_s {
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ISO;
    int ISO_len;
    // M4_ARRAY_DESC("min_busy_thre", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "160",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_busy_thre;
    int min_busy_thre_len;
    // M4_ARRAY_DESC("min_grad_thr1", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "32",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_grad_thr1;
    int min_grad_thr1_len;
    // M4_ARRAY_DESC("min_grad_thr2", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "32",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_grad_thr2;
    int min_grad_thr2_len;
    // M4_ARRAY_DESC("k_grad1", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "5",M4_DIGIT(4), M4_DYNAMIC(1))
    float* k_grad1;
    int k_grad1_len;
    // M4_ARRAY_DESC("k_grad2", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* k_grad2;
    int k_grad2_len;
    // M4_ARRAY_DESC("gb_thre", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "7",M4_DIGIT(4), M4_DYNAMIC(1))
    float* gb_thre;
    int gb_thre_len;
    // M4_ARRAY_DESC("maxCorV", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "40",M4_DIGIT(4), M4_DYNAMIC(1))
    float* maxCorV;
    int maxCorV_len;
    // M4_ARRAY_DESC("maxCorVboth", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "8",M4_DIGIT(4), M4_DYNAMIC(1))
    float* maxCorVboth;
    int maxCorVboth_len;
    // M4_ARRAY_DESC("dark_thre", "f32", M4_SIZE(1,100), M4_RANGE(0,2047), "120",M4_DIGIT(4), M4_DYNAMIC(1))
    float* dark_thre;
    int dark_thre_len;
    // M4_ARRAY_DESC("dark_threHi", "f32", M4_SIZE(1,100), M4_RANGE(0,2047), "240",M4_DIGIT(4), M4_DYNAMIC(1))
    float* dark_threHi;
    int dark_threHi_len;
    // M4_ARRAY_DESC("k_grad1_dark", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "6",M4_DIGIT(4), M4_DYNAMIC(1))
    float* k_grad1_dark;
    int k_grad1_dark_len;
    // M4_ARRAY_DESC("k_grad2_dark", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* k_grad2_dark;
    int k_grad2_dark_len;
    // M4_ARRAY_DESC("min_grad_thr_dark1", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "64",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_grad_thr_dark1;
    int min_grad_thr_dark1_len;
    // M4_ARRAY_DESC("min_grad_thr_dark2", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "32",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_grad_thr_dark2;
    int min_grad_thr_dark2_len;
    // M4_ARRAY_DESC("noiseCurve_0", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* noiseCurve_0;
    int noiseCurve_0_len;
    // M4_ARRAY_DESC("noiseCurve_1", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* noiseCurve_1;
    int noiseCurve_1_len;
    // M4_ARRAY_DESC("GValueLimitLo", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "1280",M4_DIGIT(4), M4_DYNAMIC(1))
    float* GValueLimitLo;
    int GValueLimitLo_len;
    // M4_ARRAY_DESC("GValueLimitHi", "f32", M4_SIZE(1,100), M4_RANGE(0,4095), "1760",M4_DIGIT(4), M4_DYNAMIC(1))
    float* GValueLimitHi;
    int GValueLimitHi_len;
    // M4_ARRAY_DESC("textureStrength", "f32", M4_SIZE(1,100), M4_RANGE(0,2), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* textureStrength;
    int textureStrength_len;
    // M4_ARRAY_DESC("ScaleLo", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ScaleLo;
    int ScaleLo_len;
    // M4_ARRAY_DESC("ScaleHi", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ScaleHi;
    int ScaleHi_len;
    // M4_ARRAY_DESC("globalStrength", "f32", M4_SIZE(1,100), M4_RANGE(0,2), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* globalStrength;
    int globalStrength_len;
    // M4_ARRAY_DESC("noise_coea", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* noise_coea;
    int noise_coea_len;
    // M4_ARRAY_DESC("noise_coeb", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* noise_coeb;
    int noise_coeb_len;
    // M4_ARRAY_DESC("diff_clip", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* diff_clip;
    int diff_clip_len;
} Gic_setting_v20_t;

typedef struct Gic_setting_v21_s {
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ISO;
    int ISO_len;
    // M4_ARRAY_DESC("min_busy_thre", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "160",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_busy_thre;
    int min_busy_thre_len;
    // M4_ARRAY_DESC("min_grad_thr1", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "32",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_grad_thr1;
    int min_grad_thr1_len;
    // M4_ARRAY_DESC("min_grad_thr2", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "32",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_grad_thr2;
    int min_grad_thr2_len;
    // M4_ARRAY_DESC("k_grad1", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "5",M4_DIGIT(4), M4_DYNAMIC(1))
    float* k_grad1;
    int k_grad1_len;
    // M4_ARRAY_DESC("k_grad2", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* k_grad2;
    int k_grad2_len;
    // M4_ARRAY_DESC("gb_thre", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "7",M4_DIGIT(4), M4_DYNAMIC(1))
    float* gb_thre;
    int gb_thre_len;
    // M4_ARRAY_DESC("maxCorV", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "40",M4_DIGIT(4), M4_DYNAMIC(1))
    float* maxCorV;
    int maxCorV_len;
    // M4_ARRAY_DESC("maxCorVboth", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "8",M4_DIGIT(4), M4_DYNAMIC(1))
    float* maxCorVboth;
    int maxCorVboth_len;
    // M4_ARRAY_DESC("dark_thre", "f32", M4_SIZE(1,100), M4_RANGE(0,2047), "120",M4_DIGIT(4), M4_DYNAMIC(1))
    float* dark_thre;
    int dark_thre_len;
    // M4_ARRAY_DESC("dark_threHi", "f32", M4_SIZE(1,100), M4_RANGE(0,2047), "240",M4_DIGIT(4), M4_DYNAMIC(1))
    float* dark_threHi;
    int dark_threHi_len;
    // M4_ARRAY_DESC("k_grad1_dark", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "6",M4_DIGIT(4), M4_DYNAMIC(1))
    float* k_grad1_dark;
    int k_grad1_dark_len;
    // M4_ARRAY_DESC("k_grad2_dark", "f32", M4_SIZE(1,100), M4_RANGE(0,15), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* k_grad2_dark;
    int k_grad2_dark_len;
    // M4_ARRAY_DESC("min_grad_thr_dark1", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "64",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_grad_thr_dark1;
    int min_grad_thr_dark1_len;
    // M4_ARRAY_DESC("min_grad_thr_dark2", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "32",M4_DIGIT(4), M4_DYNAMIC(1))
    float* min_grad_thr_dark2;
    int min_grad_thr_dark2_len;
    // M4_ARRAY_DESC("noiseCurve_0", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* noiseCurve_0;
    int noiseCurve_0_len;
    // M4_ARRAY_DESC("noiseCurve_1", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* noiseCurve_1;
    int noiseCurve_1_len;
    // M4_ARRAY_DESC("NoiseScale", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* NoiseScale;
    int NoiseScale_len;
    // M4_ARRAY_DESC("NoiseBase", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* NoiseBase;
    int NoiseBase_len;
    // M4_ARRAY_DESC("globalStrength", "f32", M4_SIZE(1,100), M4_RANGE(0,2), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* globalStrength;
    int globalStrength_len;
    // M4_ARRAY_DESC("diff_clip", "f32", M4_SIZE(1,100), M4_RANGE(0,1023), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* diff_clip;
    int diff_clip_len;
} Gic_setting_v21_t;

typedef struct CalibDbGicV20_s {
    // M4_BOOL_DESC("enable", "0")
    bool                enable;
    // M4_BOOL_DESC("edge_en", "0")
    bool                edge_en;
    // M4_BOOL_DESC("noise_cut_en", "0")
    bool                noise_cut_en;
    // M4_NUMBER_DESC("gr_ration", "u8", M4_RANGE(0,3), "0", M4_DIGIT(0))
    unsigned char              gr_ration;
    // M4_ARRAY_TABLE_DESC("GicData", "array_table_ui","none")
    Gic_setting_v20_t GicData;
} CalibDbGicV20_t;

typedef struct CalibDbV2_Gic_V20_s {
    // M4_STRUCT_DESC("GicTuningPara", "normal_ui_style")
    CalibDbGicV20_t GicTuningPara;
} CalibDbV2_Gic_V20_t;

typedef struct CalibDbGicV21_s {
    // M4_BOOL_DESC("enable", "0")
    bool                enable;
    // M4_NUMBER_DESC("gr_ration", "u8", M4_RANGE(0,3), "0", M4_DIGIT(0))
    unsigned char              gr_ration;
    // M4_ARRAY_TABLE_DESC("GicData", "array_table_ui", "none")
    Gic_setting_v21_t GicData;
} CalibDbGicV21_t;

typedef struct CalibDbV2_Gic_V21_s {
    // M4_STRUCT_DESC("GicTuningPara", "normal_ui_style")
    CalibDbGicV21_t GicTuningPara;
} CalibDbV2_Gic_V21_t;


RKAIQ_END_DECLARE

#endif
