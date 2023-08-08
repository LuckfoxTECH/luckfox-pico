/*
 * bayernr_head_v1.h
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

#ifndef __CALIBDBV2_BAYER_2DNR_HEADER_V23_H__
#define __CALIBDBV2_BAYER_2DNR_HEADER_V23_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

////////////////////////bayernr V23//////////////////////////////////////
typedef struct CalibDbV2_Bayer2dnrV23_C_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_ARRAY_DESC("lumapoint", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    int lumapoint[16];

    // M4_ARRAY_DESC("sigma", "s32", M4_SIZE(1,16), M4_RANGE(0,65535), "0.0", M4_DIGIT(0), M4_DYNAMIC(0))
    int sigma[16];

} CalibDbV2_Bayer2dnrV23_C_ISO_t;

typedef struct CalibDbV2_Bayer2dnrV23_C_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Calib_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_Bayer2dnrV23_C_ISO_t *Calib_ISO;
    int Calib_ISO_len;

} CalibDbV2_Bayer2dnrV23_C_Set_t;

typedef struct CalibDbV2_BlcV32_calib_s {

    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index1")
    float iso;

    // M4_NUMBER_DESC("Reference_r", "f32", M4_RANGE(-512,512), "256.0", M4_DIGIT(4))
    float Reference_r;

    // M4_NUMBER_DESC("Reference_gr", "f32", M4_RANGE(-512,512), "256.0", M4_DIGIT(4))
    float Reference_gr;

    // M4_NUMBER_DESC("Reference_gb", "f32", M4_RANGE(-512,512), "256.0", M4_DIGIT(4))
    float Reference_gb;

    // M4_NUMBER_DESC("Reference_b", "f32", M4_RANGE(-512,512), "256.0", M4_DIGIT(4))
    float Reference_b;

} CalibDbV2_BlcV32_calib_t;

typedef struct CalibDbV2_Bayer2dnrV23_Calib_s {
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_Bayer2dnrV23_C_Set_t *Setting;
    int Setting_len;

    // M4_STRUCT_LIST_DESC("Blc_Ref", M4_SIZE_DYNAMIC, "index1")
    CalibDbV2_BlcV32_calib_t *Blc_Ref;
    int Blc_Ref_len;
} CalibDbV2_Bayer2dnrV23_Calib_t;

typedef struct CalibDbV2_Bayer2dnrV23_gain_s {
    // M4_ARRAY_DESC("lumapoint", "u16", M4_SIZE(1,16), M4_RANGE(0,256), "[16,32,48,64,80,96,112,128,144,160,176,192,208,224,240,256]", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t gain_lumapoint[16];
    // M4_ARRAY_DESC("gain_adj", "u16", M4_SIZE(1,16), M4_RANGE(0,4095), "[16,32,48,64,80,96,112,128,144,160,176,192,208,224,240,256]", M4_DIGIT(2), M4_DYNAMIC(0))
    uint16_t gain_adj[16];

} CalibDbV2_Bayer2dnrV23_gain_t;


typedef struct CalibDbV2_Bayer2dnrV23_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_BOOL_DESC("gauss_guide", "1")
    bool gauss_guide;

    // M4_NUMBER_DESC("filter_strength", "f32", M4_RANGE(0, 16), "0.5", M4_DIGIT(2))
    float filter_strength;

    // M4_NUMBER_DESC("edgesofts", "f32", M4_RANGE(0, 16.0), "1.0", M4_DIGIT(2))
    float edgesofts;

    // M4_NUMBER_DESC("ratio", "f32", M4_RANGE(0, 1.0), "0.2", M4_DIGIT(2))
    float ratio;

    // M4_NUMBER_DESC("weight", "f32", M4_RANGE(0, 1.0), "1.0", M4_DIGIT(2))
    float weight;

    // M4_BOOL_DESC("gain_bypass", "1")
    bool gain_bypass;

    // M4_NUMBER_DESC("gain_scale", "f32", M4_RANGE(0, 16.0), "1.0", M4_DIGIT(2))
    float gain_scale;

    // M4_NUMBER_DESC("pix_diff", "u16", M4_RANGE(0, 16383), "16383", M4_DIGIT(0))
    uint16_t pix_diff;

    // M4_NUMBER_DESC("diff_thld", "u16", M4_RANGE(0, 1023), "1023", M4_DIGIT(0))
    uint16_t diff_thld;


    // M4_NUMBER_DESC("trans_mode", "u8", M4_RANGE(0, 3), "0", M4_DIGIT(0))
    uint8_t trans_mode;
    // M4_NUMBER_DESC("trans_offset", "u16", M4_RANGE(0, 8191), "256", M4_DIGIT(0))
    uint16_t trans_offset;
    // M4_NUMBER_DESC("itrans_offset", "u16", M4_RANGE(0, 65535), "32768", M4_DIGIT(0))
    uint16_t itrans_offset;
    // M4_NUMBER_DESC("trans_datmax", "u32", M4_RANGE(0, 1048575), "1048575", M4_DIGIT(0))
    uint32_t trans_datmax;


    // M4_NUMBER_DESC("hdr_dgain_scale_s", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_dgain_scale_s;

    // M4_NUMBER_DESC("hdr_dgain_scale_m", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_dgain_scale_m;

    // M4_ARRAY_TABLE_DESC("gain_adj", "array_table_ui", "none")
    CalibDbV2_Bayer2dnrV23_gain_t gain_adj;

} CalibDbV2_Bayer2dnrV23_T_ISO_t;

typedef struct CalibDbV2_Bayer2dnrV23_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_Bayer2dnrV23_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_Bayer2dnrV23_T_Set_t;


typedef struct CalibDbV2_Bayer2dnrV23_Tuning_s {
    // M4_BOOL_DESC("enable", "1")
    bool enable;
    // M4_BOOL_DESC("hdrdgain_ctrl_en", "0")
    bool hdrdgain_ctrl_en;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_Bayer2dnrV23_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_Bayer2dnrV23_Tuning_t;



typedef struct CalibDbV2_Bayer2dnrV23_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V23", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("CalibPara", "normal_ui_style")
    CalibDbV2_Bayer2dnrV23_Calib_t CalibPara;
    // M4_STRUCT_DESC("Bayernr2D", "normal_ui_style")
    CalibDbV2_Bayer2dnrV23_Tuning_t TuningPara;
} CalibDbV2_Bayer2dnrV23_t;

RKAIQ_END_DECLARE

#endif
