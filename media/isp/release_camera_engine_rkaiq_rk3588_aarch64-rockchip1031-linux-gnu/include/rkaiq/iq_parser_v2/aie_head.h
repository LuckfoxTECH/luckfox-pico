/*
 * aie_head.h
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

#ifndef __CALIBDBV2_AIE_HEAD_H__
#define __CALIBDBV2_AIE_HEAD_H__ 

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

// #define M4_BOOL_DESC(ALIAS, DEFAULT, HIDE)

// #define M4_ARRAY_DESC(ALIAS, TYPE, SIZE, RANGE, DEFAULT,DIGIT,DYNAMIC, HIDE)

// #define M4_NUMBER_DESC(ALIAS, TYPE, RANGE, DEFAULT,DIGIT, HIDE)

typedef struct __ie_param {
    // M4_BOOL_DESC("enable", "1", 0)
    bool enable;
    // M4_NUMBER_DESC("mode", "u8", M4_RANGE(0,6), "0", M4_DIGIT(0), 0)
    unsigned char mode;
}IE_Param_t;

typedef struct __ie {
    // M4_STRUCT_DESC("IETuningPara", "normal_ui_style")
    IE_Param_t param;
} CalibDbV2_IE_t;

RKAIQ_END_DECLARE

#endif
