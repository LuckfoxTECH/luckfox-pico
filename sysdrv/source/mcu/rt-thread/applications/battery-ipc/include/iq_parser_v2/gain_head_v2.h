/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
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
 */

#ifndef __CALIBDBV2_GAIN_HEADER_V2_H__
#define __CALIBDBV2_GAIN_HEADER_V2_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

///////////////////////////gain v1//////////////////////////////////////


typedef struct CalibDbV2_GainV2_T_ISO_s {
    // M4_NUMBER_MARK_DESC("iso", "f32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index2")
    float iso;

    // M4_NUMBER_DESC("hdr_gain_scale_s", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_gain_scale_s;

    // M4_NUMBER_DESC("hdr_gain_scale_m", "f32", M4_RANGE(0, 128.0), "1.0", M4_DIGIT(2))
    float hdr_gain_scale_m;

} CalibDbV2_GainV2_T_ISO_t;


typedef struct CalibDbV2_GainV2_T_Set_s {
    // M4_STRING_MARK_DESC("SNR_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "LSNR",M4_DYNAMIC(0), "index1")
    char *SNR_Mode;
    // M4_STRING_DESC("Sensor_Mode", M4_SIZE(1,1), M4_RANGE(0, 64), "lcg", M4_DYNAMIC(0))
    char *Sensor_Mode;
    // M4_STRUCT_LIST_DESC("Tuning_ISO", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_GainV2_T_ISO_t *Tuning_ISO;
    int Tuning_ISO_len;
} CalibDbV2_GainV2_T_Set_t;

typedef struct CalibDbV2_GainV2_Tuning_s {
    // M4_BOOL_DESC("hdrgain_ctrl_enable", "1")
    bool hdrgain_ctrl_enable;
    // M4_STRUCT_LIST_DESC("Setting", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_GainV2_T_Set_t *Setting;
    int Setting_len;
} CalibDbV2_GainV2_Tuning_t;


typedef struct CalibDbV2_GainV2_s {
    // M4_STRING_DESC("Version", M4_SIZE(1,1), M4_RANGE(0, 64), "V2", M4_DYNAMIC(0))
    char *Version;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_GainV2_Tuning_t TuningPara;
} CalibDbV2_GainV2_t;

RKAIQ_END_DECLARE

#endif
