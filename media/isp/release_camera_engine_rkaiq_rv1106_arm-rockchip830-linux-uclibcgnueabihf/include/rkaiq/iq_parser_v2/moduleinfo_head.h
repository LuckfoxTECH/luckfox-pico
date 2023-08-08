/*
 * moduleinfo_head.h
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

#ifndef __CALIBDBV2_MODULE_INFO_HEADER_H__
#define __CALIBDBV2_MODULE_INFO_HEADER_H__
// #define M4_STRUCT_DESC(ALIAS, SIZE, UI_MODULE)
// #define M4_NUMBER_DESC(ALIAS, TYPE, SIZE, RANGE, DEFAULT)
// #define M4_STRING_DESC(ALIAS, SIZE, RANGE, DEFAULT)
// #define M4_ENUM_DESC(ALIAS, ENUM, DEFAULT)

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

typedef struct CalibDb_Sensor_Module_s {
    // M4_NUMBER_DESC("FNumber", "f32", M4_RANGE(1,100), "1.6", M4_DIGIT(2),M4_HIDE(0))
    float FNumber;
    // M4_NUMBER_DESC("EFL", "f32", M4_RANGE(0,200), "3.5", M4_DIGIT(2),M4_HIDE(0))
    float EFL;
    // M4_NUMBER_DESC("LensT", "f32", M4_RANGE(0,100), "90", M4_DIGIT(2),M4_HIDE(0))
    float LensT;
    // M4_NUMBER_DESC("IRCutT", "f32", M4_RANGE(0,100), "90", M4_DIGIT(2),M4_HIDE(0))
    float IRCutT;
} CalibDb_Sensor_Module_t;

typedef struct CalibDb_Module_ParaV2_s {
    // M4_STRUCT_DESC("SensorModule", "normal_ui_style")
    CalibDb_Sensor_Module_t sensor_module;
} CalibDb_Module_ParaV2_t;

RKAIQ_END_DECLARE

#endif
