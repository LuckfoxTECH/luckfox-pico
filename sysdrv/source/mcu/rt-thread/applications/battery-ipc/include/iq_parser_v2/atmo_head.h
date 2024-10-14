/*
 * atmo_head.h
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

#ifndef __CALIBDBV2_ATMO_HEADER_H__
#define __CALIBDBV2_ATMO_HEADER_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

typedef enum GlobalLumaMode_e {
    GLOBALLUMAMODE_ENVLV     = 0,
    GLOBALLUMAMODE_ISO       = 1,
} GlobalLumaMode_t;

typedef struct GlobalLumaData_s
{
    // M4_ARRAY_DESC("EnvLv", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float*            EnvLv;
    int EnvLv_len;
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ISO;
    int ISO_len;
    // M4_ARRAY_DESC("Strength", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.25",M4_DIGIT(4), M4_DYNAMIC(1))
    float*            Strength;
    int Strength_len;
} GlobalLumaData_t;

typedef struct CalibDbGlobalLuma_s
{
    // M4_ENUM_DESC("Mode", "GlobalLumaMode_t", "GLOBALLUMAMODE_ENVLV")
    GlobalLumaMode_t Mode;
    // M4_ARRAY_TABLE_DESC("GlobalLumaData", "array_table_ui", "none")
    GlobalLumaData_t GlobalLumaData;
    // M4_NUMBER_DESC("Tolerance", "f32", M4_RANGE(0,0.2), "0", M4_DIGIT(4))
    float            Tolerance;
} CalibDbGlobalLuma_t;

typedef enum DetailsHighLightMode_e {
    DETAILSHIGHLIGHTMODE_OEPDF       = 0,
    DETAILSHIGHLIGHTMODE_ENVLV       = 1,
} DetailsHighLightMode_t;

typedef struct HighLightData_s
{
    // M4_ARRAY_DESC("OEPdf", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* OEPdf;
    int OEPdf_len;
    // M4_ARRAY_DESC("EnvLv", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float*            EnvLv;
    int EnvLv_len;
    // M4_ARRAY_DESC("Strength", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.5",M4_DIGIT(4), M4_DYNAMIC(1))
    float*            Strength;
    int Strength_len;
} HighLightData_t;

typedef struct CalibDbDetailsHighLight_s
{
    // M4_ENUM_DESC("Mode", "DetailsHighLightMode_t", "DETAILSHIGHLIGHTMODE_ENVLV")
    DetailsHighLightMode_t Mode;
    // M4_ARRAY_TABLE_DESC("HighLightData", "array_table_ui", "none")
    HighLightData_t HighLightData;
    // M4_NUMBER_DESC("Tolerance", "f32", M4_RANGE(0,0.2), "0", M4_DIGIT(4))
    float                  Tolerance;
} CalibDbDetailsHighLight_t;

typedef enum DetailsLowLightMode_e {
    DETAILSLOWLIGHTMODE_FOCUSLUMA   = 0,
    DETAILSLOWLIGHTMODE_DARKPDF     = 1,
    DETAILSLOWLIGHTMODE_ISO          = 2,
} DetailsLowLightMode_t;

typedef struct LowLightData_s
{
    // M4_ARRAY_DESC("FocusLuma", "f32", M4_SIZE(1,100), M4_RANGE(0,100), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* FocusLuma;
    int FocusLuma_len;
    // M4_ARRAY_DESC("DarkPdf", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float* DarkPdf;
    int DarkPdf_len;
    // M4_ARRAY_DESC("ISO", "f32", M4_SIZE(1,100), M4_RANGE(0,10000000), "50",M4_DIGIT(4), M4_DYNAMIC(1))
    float* ISO;
    int ISO_len;
    // M4_ARRAY_DESC("Strength", "f32", M4_SIZE(1,100), M4_RANGE(1,4), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float*            Strength;
    int Strength_len;
} LowLightData_t;

typedef struct CalibDbDetailsLowLight_s
{
    // M4_ENUM_DESC("Mode", "DetailsLowLightMode_t", "DETAILSLOWLIGHTMODE_ISO")
    DetailsLowLightMode_t Mode;
    // M4_ARRAY_TABLE_DESC("LowLightData", "array_table_ui", "none")
    LowLightData_t LowLightData;
    // M4_NUMBER_DESC("Tolerance", "f32", M4_RANGE(0,0.2), "0", M4_DIGIT(4))
    float                  Tolerance;
} CalibDbDetailsLowLight_t;

typedef enum TmoTypeMode_e {
    TMOTYPEMODE_DYNAMICRANGE   = 0,
    TMOTYPEMODE_ENVLV           = 1,
} TmoTypeMode_t;

typedef struct TmoData_s
{
    // M4_ARRAY_DESC("DynamicRange", "f32", M4_SIZE(1,100), M4_RANGE(1,84), "1",M4_DIGIT(4), M4_DYNAMIC(1))
    float* DynamicRange;
    int DynamicRange_len;
    // M4_ARRAY_DESC("EnvLv", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0",M4_DIGIT(4), M4_DYNAMIC(1))
    float*            EnvLv;
    int EnvLv_len;
    // M4_ARRAY_DESC("Strength", "f32", M4_SIZE(1,100), M4_RANGE(0,1), "0.5",M4_DIGIT(4), M4_DYNAMIC(1))
    float*            Strength;
    int Strength_len;
} TmoData_t;

typedef struct CalibDbLocalTMO_s
{
    // M4_ENUM_DESC("Mode", "TmoTypeMode_t", "TMOTYPEMODE_DYNAMICRANGE")
    TmoTypeMode_t Mode;
    // M4_ARRAY_TABLE_DESC("LocalTmoData", "array_table_ui", "none")
    TmoData_t LocalTmoData;
    // M4_NUMBER_DESC("Tolerance", "f32", M4_RANGE(0,0.2), "0", M4_DIGIT(4))
    float                  Tolerance;
} CalibDbLocalTMO_t;

typedef struct CalibDbGlobaTMO_s
{
    // M4_BOOL_DESC("Enable", "1")
    bool         Enable;
    // M4_NUMBER_DESC("IIR", "f32", M4_RANGE(1,1000), "64", M4_DIGIT(0))
    float         IIR;
    // M4_ENUM_DESC("Mode", "TmoTypeMode_t", "TMOTYPEMODE_DYNAMICRANGE")
    TmoTypeMode_t Mode;
    // M4_ARRAY_TABLE_DESC("GlobalTmoData", "array_table_ui", "none")
    TmoData_t GlobalTmoData;
    // M4_NUMBER_DESC("Tolerance", "f32", M4_RANGE(0,0.2), "0", M4_DIGIT(4))
    float                  Tolerance;
} CalibDbGlobaTMO_t;

typedef struct CalibDbTmoV20_s
{
    // M4_BOOL_DESC("Enable", "1")
    bool         Enable;
    // M4_STRUCT_DESC("GlobalLuma", "normal_ui_style")
    CalibDbGlobalLuma_t       GlobalLuma;
    // M4_STRUCT_DESC("DetailsHighLight", "normal_ui_style")
    CalibDbDetailsHighLight_t DetailsHighLight;
    // M4_STRUCT_DESC("DetailsLowLight", "normal_ui_style")
    CalibDbDetailsLowLight_t  DetailsLowLight;
    // M4_STRUCT_DESC("LocalTMO", "normal_ui_style")
    CalibDbLocalTMO_t         LocalTMO;
    // M4_STRUCT_DESC("GlobaTMO", "normal_ui_style")
    CalibDbGlobaTMO_t         GlobaTMO;
    // M4_NUMBER_DESC("damp", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float damp;
} CalibDbTmoV20_t;

typedef struct CalibDbV2_tmo_s
{
    // M4_STRUCT_DESC("TmoTuningPara", "normal_ui_style")
    CalibDbTmoV20_t TmoTuningPara;
} CalibDbV2_tmo_t;


RKAIQ_END_DECLARE

#endif
