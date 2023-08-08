/*
 * fec_head.h
 *
 *  Copyright (c) 2021 Rockchip Electronics Co., Ltd.
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

#ifndef __CALIBDBV2_FEC_HEAD_H__
#define __CALIBDBV2_FEC_HEAD_H__

#include <rk_aiq_comm.h>

RKAIQ_BEGIN_DECLARE

typedef struct CalibDbV2_Fec_Param_s {
    unsigned char fec_en;
    char meshfile[256];
    int correct_level;
    double light_center[2]; // light center
    double coefficient[4]; // the distortion coefficient of the fisheye lens
} CalibDbV2_Fec_Param_t;

typedef struct CalibDbV2_FEC_s {
    // M4_STRUCT_DESC("param", "normal_ui_style")
    CalibDbV2_Fec_Param_t param;
} CalibDbV2_FEC_t;

RKAIQ_END_DECLARE

#endif
