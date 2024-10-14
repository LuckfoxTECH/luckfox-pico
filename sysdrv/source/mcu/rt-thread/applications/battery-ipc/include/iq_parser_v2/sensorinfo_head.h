/*
 * sensorinfo_head.h
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

#ifndef __CALIBDBV2_SENSOR_INFO_HEADER_H__
#define __CALIBDBV2_SENSOR_INFO_HEADER_H__

// #define M4_STRUCT_DESC(ALIAS, SIZE, UI_MODULE)
// #define M4_NUMBER_DESC(ALIAS, TYPE, SIZE, RANGE, DEFAULT)
// #define M4_STRING_DESC(ALIAS, SIZE, RANGE, DEFAULT)
// #define M4_ENUM_DESC(ALIAS, ENUM, DEFAULT)

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

/*****************************************************************************/
/**
 * @brief   ISP2.0 SensorInfo enum Params
 */
/*****************************************************************************/
typedef enum _CalibDb_ExpGainModeV2_e {
    EXPGAIN_MODE_LINEAR         = 0,
    EXPGAIN_MODE_NONLINEAR_DB   = 1,
    EXPGAIN_MODE_MAX            = 2
} CalibDb_ExpGainModeV2_t;

typedef enum  _CalibDb_HdrFrmNumV2_e {
    HDR_TWO_FRAME = 0,
    HDR_THREE_FRAME = 1,
} CalibDb_HdrFrmNumV2_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 SensorInfo config Params
 */
/*****************************************************************************/
typedef struct CalibDb_AecGainRangeV2_s {
    // M4_ENUM_DESC("GainMode", "CalibDb_ExpGainModeV2_t","EXPGAIN_MODE_LINEAR")
    CalibDb_ExpGainModeV2_t     GainMode;

    // M4_ARRAY_DESC("GainRange", "f32", M4_SIZE(1,7), M4_RANGE(-65535,65535), "0", M4_DIGIT(4), M4_DYNAMIC(2))
    float*                      GainRange;//contains 7 params per line
    int                         GainRange_len;
} CalibDb_AecGainRangeV2_t;

typedef struct CalibDb_CISNormalTimeSetV2_s {
    // M4_NUMBER_DESC("TimeRegMin", "u16", M4_RANGE(1,100), "1", M4_DIGIT(0),M4_HIDE(0))
    uint16_t                CISTimeRegMin;

    // M4_STRUCT_DESC("TimeRegMaxFac", "normal_ui_style")
    Cam2x1FloatMatrix_t     CISLinTimeRegMaxFac;

    // M4_STRUCT_DESC("TimeRegOdevity", "normal_ui_style")
    Cam2x1FloatMatrix_t     CISTimeRegOdevity;
} CalibDb_CISNormalTimeSetV2_t;

typedef struct CalibDb_CISHdrTimeSetV2_s {
    // M4_ENUM_DESC("name", "CalibDb_HdrFrmNumV2_t","HDR_TWO_FRAME")
    CalibDb_HdrFrmNumV2_t   name;

    // M4_BOOL_DESC("CISTimeRegUnEqualEn", "0")
    bool                    CISTimeRegUnEqualEn;

    // M4_NUMBER_DESC("CISTimeRegMin", "u16", M4_RANGE(1,100), "1", M4_DIGIT(0),M4_HIDE(0))
    uint16_t                CISTimeRegMin;

    // M4_STRUCT_DESC("CISHdrTimeRegSumFac", "normal_ui_style")
    Cam2x1FloatMatrix_t     CISHdrTimeRegSumFac;

    // M4_STRUCT_DESC("CISTimeRegMax", "normal_ui_style")
    Cam1x3ShortMatrix_t     CISTimeRegMax; //specially for Hdr that has limit on sframe/mframe, requiring max time line.value 0: no limit

    // M4_STRUCT_DESC("CISTimeRegOdevity", "normal_ui_style")
    Cam2x1FloatMatrix_t     CISTimeRegOdevity;
} CalibDb_CISHdrTimeSetV2_t;

typedef struct CalibDb_CISTimeSet_CombV2_s {
    // M4_STRUCT_DESC("Linear", "normal_ui_style")
    CalibDb_CISNormalTimeSetV2_t Linear;

    // M4_STRUCT_LIST_DESC("Hdr", M4_SIZE(1,2),"normal_ui_style")
    CalibDb_CISHdrTimeSetV2_t Hdr[2];
} CalibDb_CISTimeSet_CombV2_t;

typedef struct CalibDb_AeRangeV2_s {
    // M4_NUMBER_DESC("Min", "f32", M4_RANGE(0,65535), "0", M4_DIGIT(3),M4_HIDE(0))
    float                   Min;

    // M4_NUMBER_DESC("Max", "f32", M4_RANGE(0,65535), "0", M4_DIGIT(3),M4_HIDE(0))
    float                   Max;
} CalibDb_AeRangeV2_t;

typedef struct CalibDb_CISGainSetV2_s {
    // M4_STRUCT_DESC("CISAgainRange", "normal_ui_style")
    CalibDb_AeRangeV2_t       CISAgainRange; //sensor Again or LCG range

    // M4_STRUCT_DESC("CISExtraAgainRange", "normal_ui_style")
    CalibDb_AeRangeV2_t       CISExtraAgainRange; //add for HDR-DCG MODE, HCG range

    // M4_STRUCT_DESC("CISDgainRange", "normal_ui_style")
    CalibDb_AeRangeV2_t       CISDgainRange; //sensor Dgain

    // M4_STRUCT_DESC("CISIspDgainRange", "normal_ui_style")
    CalibDb_AeRangeV2_t       CISIspDgainRange; //Isp Dgain

    // M4_BOOL_DESC("CISHdrGainIndSetEn", "1")
    bool                   CISHdrGainIndSetEn; //bit 0:use the same value; bit 1: support use different gain value
} CalibDb_CISGainSetV2_t;

typedef struct CalibDb_CISHdrSetV2_s {
    // M4_BOOL_DESC("hdr_en", "0")
    bool hdr_en;

    // M4_ENUM_DESC("hdr_mode", "rk_aiq_isp_hdr_mode_t","RK_AIQ_ISP_HDR_MODE_2_LINE_HDR")
    rk_aiq_isp_hdr_mode_t hdr_mode;

    // M4_ENUM_DESC("line_mode", "rk_aiq_sensor_hdr_line_mode_t","RKAIQ_SENSOR_HDR_MODE_STAGGER")
    rk_aiq_sensor_hdr_line_mode_t line_mode;
} CalibDb_CISHdrSetV2_t;

typedef struct CalibDb_Dcg_ParamsV2_s {
    // M4_BOOL_DESC("support_en", "0")
    bool                support_en;

    // M4_ENUM_DESC("dcg_optype", "RKAiqOPMode_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t       dcg_optype;

    // M4_STRUCT_DESC("dcg_mode", "normal_ui_style")
    Cam1x3IntMatrix_t   dcg_mode;

    // M4_NUMBER_DESC("dcg_ratio", "f32", M4_RANGE(1,100), "1", M4_DIGIT(2),M4_HIDE(0))
    float               dcg_ratio;

    // M4_BOOL_DESC("sync_switch", "1")
    bool                sync_switch;

    // M4_NUMBER_DESC("lcg2hcg_gain_th", "f32", M4_RANGE(1,4096), "32", M4_DIGIT(2),M4_HIDE(0))
    float               lcg2hcg_gain_th;

    // M4_NUMBER_DESC("hcg2lcg_gain_th", "f32", M4_RANGE(1,4096), "16", M4_DIGIT(2),M4_HIDE(0))
    float               hcg2lcg_gain_th;
} CalibDb_Dcg_ParamsV2_t;

typedef struct CalibDb_DcgSetV2_s {
    // M4_STRUCT_DESC("Linear", "normal_ui_style")
    CalibDb_Dcg_ParamsV2_t Linear;

    // M4_STRUCT_DESC("Hdr", "normal_ui_style")
    CalibDb_Dcg_ParamsV2_t Hdr;
} CalibDb_DcgSetV2_t;

typedef struct CalibDb_ExpUpdateV2_s {
    // M4_NUMBER_DESC("time_update", "s32", M4_RANGE(0,10), "2", M4_DIGIT(0),M4_HIDE(0))
    int time_update;

    // M4_NUMBER_DESC("gain_update", "s32", M4_RANGE(0,10), "2", M4_DIGIT(0),M4_HIDE(0))
    int gain_update;

    // M4_NUMBER_DESC("dcg_update", "s32", M4_RANGE(0,10), "2", M4_DIGIT(0),M4_HIDE(0))
    int dcg_update;
} CalibDb_ExpUpdateV2_t;

typedef struct CalibDb_ExpUpdate_CombV2_s {
    // M4_STRUCT_DESC("Linear", "normal_ui_style")
    CalibDb_ExpUpdateV2_t Linear;

    // M4_STRUCT_DESC("Hdr", "normal_ui_style")
    CalibDb_ExpUpdateV2_t Hdr;
} CalibDb_ExpUpdate_CombV2_t;

typedef struct CalibDb_ResInfoV2_s {
    // M4_NUMBER_DESC("width", "u16", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(0))
    uint16_t width;
    // M4_NUMBER_DESC("height", "u16", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(0))
    uint16_t height;
} CalibDb_ResInfoV2_t;


typedef struct CalibDb_Sensor_ParaV2_s {
    // M4_STRUCT_DESC("Resolution", "normal_ui_style")
    CalibDb_ResInfoV2_t             resolution;

    // M4_STRUCT_DESC("Gain2Reg", "normal_ui_style")
    CalibDb_AecGainRangeV2_t        Gain2Reg;

    // M4_STRUCT_DESC("Time2Reg", "normal_ui_style")
    Cam1x4FloatMatrix_t             Time2Reg;

    // M4_STRUCT_DESC("CISGainSet", "normal_ui_style")
    CalibDb_CISGainSetV2_t          CISGainSet;

    // M4_STRUCT_DESC("CISTimeSet", "normal_ui_style")
    CalibDb_CISTimeSet_CombV2_t     CISTimeSet;

    // M4_STRUCT_DESC("CISHdrSet", "normal_ui_style")
    CalibDb_CISHdrSetV2_t           CISHdrSet;

    // M4_STRUCT_DESC("CISDcgSet", "normal_ui_style")
    CalibDb_DcgSetV2_t              CISDcgSet;

    // M4_STRUCT_DESC("CISExpUpdate", "normal_ui_style")
    CalibDb_ExpUpdate_CombV2_t      CISExpUpdate;

    // M4_NUMBER_DESC("CISMinFps", "f32", M4_RANGE(1,100), "10", M4_DIGIT(2),M4_HIDE(0))
    float                           CISMinFps;

    // M4_NUMBER_DESC("CISFlip", "u8", M4_RANGE(0,3), "0", M4_DIGIT(0),M4_HIDE(0))
    uint8_t                         CISFlip; // bit 0 : mirror, bit 1 : flip , range[0,3]
} CalibDb_Sensor_ParaV2_t;

RKAIQ_END_DECLARE

#endif
