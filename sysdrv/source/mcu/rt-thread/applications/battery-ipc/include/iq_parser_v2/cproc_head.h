/*
 * cproc_head.h
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

#ifndef __CALIBDBV2_CPROC_HEAD_H__
#define __CALIBDBV2_CPROC_HEAD_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

// #define M4_BOOL_DESC(ALIAS, DEFAULT, HIDE)

// #define M4_ARRAY_DESC(ALIAS, TYPE, SIZE, RANGE, DEFAULT,DIGIT,DYNAMIC, HIDE)

// #define M4_NUMBER_DESC(ALIAS, TYPE, RANGE, DEFAULT,DIGIT, HIDE)

typedef struct __cproc_param {
    // M4_BOOL_DESC("enable", "1", 0)
    bool enable;
    // M4_NUMBER_DESC("brightness", "u8", M4_RANGE(0,255), "128", M4_DIGIT(0), 0)
    unsigned char brightness;
    // M4_NUMBER_DESC("contrast", "u8", M4_RANGE(0,255), "128", M4_DIGIT(0), 0)
    unsigned char contrast;
    // M4_NUMBER_DESC("saturation", "u8", M4_RANGE(0,255), "128", M4_DIGIT(0), 0)
    unsigned char saturation;
    // M4_NUMBER_DESC("hue", "u8", M4_RANGE(0,255), "128", M4_DIGIT(0), 0)
    unsigned char hue;
} Cproc_Param_t;

typedef struct __cproc {
    // M4_STRUCT_DESC("CprocTuningPara", "normal_ui_style")
    Cproc_Param_t param;
} CalibDbV2_Cproc_t;

RKAIQ_END_DECLARE

#endif
