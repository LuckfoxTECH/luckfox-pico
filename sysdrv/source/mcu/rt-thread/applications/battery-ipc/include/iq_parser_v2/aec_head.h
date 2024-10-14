/*
 * aec_head.h
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

#ifndef __CALIBDBV2_AEC_HEAD_H__
#define __CALIBDBV2_AEC_HEAD_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

// #define M4_STRUCT_DESC(ALIAS, SIZE, UI_MODULE)
// #define M4_NUMBER_DESC(ALIAS, TYPE, SIZE, RANGE, DEFAULT)
// #define M4_STRING_DESC(ALIAS, SIZE, RANGE, DEFAULT)
// #define M4_ENUM_DESC(ALIAS, ENUM, DEFAULT)

#define AECV2_PIRIS_STEP_TABLE_MAX (1024)
#define AECV2_MAX_GRIDWEIGHT_NUM (225)
#define AECV2_MAX_HDRFRAME_NUM (3)
#define AECV2_HDCIRIS_DOT_MAX_NUM (256)

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC enum Params
 */
/*****************************************************************************/
typedef enum _CalibDb_HdrAeRatioTypeV2_e {
    AECV2_HDR_RATIOTYPE_MODE_AUTO,
    AECV2_HDR_RATIOTYPE_MODE_FIX,
} CalibDb_HdrAeRatioTypeV2_t;

typedef enum _CalibDb_AeStrategyModeV2_e {
    AECV2_STRATEGY_MODE_LOWLIGHT,
    AECV2_STRATEGY_MODE_HIGHLIGHT,
} CalibDb_AeStrategyModeV2_t;

typedef enum _CalibDb_AeHdrLongFrmModeV2_e {
    AECV2_HDR_LONGFRMMODE_DISABLE             = 0,
    AECV2_HDR_LONGFRMMODE_AUTO                = 1,
    AECV2_HDR_LONGFRMMODE_ENABLE              = 2,
} CalibDb_AeHdrLongFrmModeV2_t;

typedef enum _CalibDb_AecMeasAreaModeV2_e {
    AECV2_MEASURE_AREA_AUTO    = 0,
    AECV2_MEASURE_AREA_UP,
    AECV2_MEASURE_AREA_BOTTOM,
    AECV2_MEASURE_AREA_LEFT,
    AECV2_MEASURE_AREA_RIGHT,
    AECV2_MEASURE_AREA_CENTER,
} CalibDb_AecMeasAreaModeV2_t;

typedef enum _CalibDb_FlickerFreqV2_e {
    AECV2_FLICKER_FREQUENCY_OFF   = 0,
    AECV2_FLICKER_FREQUENCY_50HZ = 1,
    AECV2_FLICKER_FREQUENCY_60HZ = 2,
    AECV2_FLICKER_FREQUENCY_AUTO = 3,
} CalibDb_FlickerFreqV2_t;

typedef enum _CalibDb_AntiFlickerModeV2_e {
    AECV2_ANTIFLICKER_AUTO_MODE,
    AECV2_ANTIFLICKER_NORMAL_MODE,
} CalibDb_AntiFlickerModeV2_t;

typedef enum _CalibDb_IrisTypeV2_e {
    IRISV2_DC_TYPE  = 0,
    IRISV2_P_TYPE   = 1,
    IRISV2_HDC_TYPE = 2,
} CalibDb_IrisTypeV2_t;

typedef enum _CalibDb_DelayTypeV2_e {
    DELAY_TYPE_FRAME = 0,
    DELAY_TYPE_TIME = 1,
} CalibDb_DelayTypeV2_t;

typedef enum _CalibDb_CamYRangeModeV2_e {
    CAM_YRANGEV2_MODE_INVALID    = 0,    /**< invalid y range mode   */
    CAM_YRANGEV2_MODE_FULL,              /**< Y = 0.299 * R + 0.587 * G + 0.114 * B,full range BT601*/
    CAM_YRANGEV2_MODE_LIMITED,           /**< Y = 16 + 0.25R + 0.5G + 0.1094B,limited range*/
    CAM_YRANGEV2_MODE_MAX,
} CalibDb_CamYRangeModeV2_t;

typedef enum _CalibDb_CamRawStatsModeV2_e {
    CAM_RAWSTATSV2_MODE_INVALID       = 0,    /**< lower border (only for an internal evaluation) */
    CAM_RAWSTATSV2_MODE_R,                 /**< R channel */
    CAM_RAWSTATSV2_MODE_G,                 /**< G channel */
    CAM_RAWSTATSV2_MODE_B,                 /**< B channel */
    CAM_RAWSTATSV2_MODE_Y,                 /**< luminance channel */
    CAM_RAWSTATSV2_MODE_MAX,                  /**< upper border (only for an internal evaluation) */
} CalibDb_CamRawStatsModeV2_t;

typedef enum _CalibDb_CamHistStatsModeV2_e {
    CAM_HISTV2_MODE_INVALID       = 0,    /**< lower border (only for an internal evaluation) */
    CAM_HISTV2_MODE_RGB_COMBINED  = 1,    /**< RGB combined histogram, only available for SiHist */
    CAM_HISTV2_MODE_R             = 2,    /**< R histogram */
    CAM_HISTV2_MODE_G             = 3,    /**< G histogram */
    CAM_HISTV2_MODE_B             = 4,    /**< B histogram */
    CAM_HISTV2_MODE_Y             = 5,    /**< Y luminance histogram */
    CAM_HISTV2_MODE_MAX,                                  /**< upper border (only for an internal evaluation) */
} CalibDb_CamHistStatsModeV2_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC algo CommonCtrl Config Params
 */
/*****************************************************************************/
typedef struct CalibDb_AeDelayFrmNumV2_s {
    // M4_ENUM_DESC("DelayType", "CalibDb_DelayTypeV2_t","DELAY_TYPE_FRAME")
    CalibDb_DelayTypeV2_t   DelayType;

    // M4_NUMBER_DESC("BlackDelay", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0),M4_HIDE(0))
    uint8_t                 BlackDelay;

    // M4_NUMBER_DESC("WhiteDelay", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0),M4_HIDE(0))
    uint8_t                 WhiteDelay;

} CalibDb_AeDelayFrmNumV2_t;

typedef struct CalibDb_AeDyDamp_s {
    // M4_BOOL_DESC("DyDampEn", "1")
    bool                    DyDampEn;

    // M4_ENUM_DESC("SlowOPType", "RKAiqOPMode_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t           SlowOPType;

    // M4_NUMBER_DESC("SlowRange", "f32", M4_RANGE(0,100), "15", M4_DIGIT(2),M4_HIDE(0))
    float                   SlowRange;

    // M4_NUMBER_DESC("SlowDamp", "f32", M4_RANGE(0,1), "0.95", M4_DIGIT(2),M4_HIDE(0))
    float                   SlowDamp;

} CalibDb_AeDyDamp_t;


typedef struct CalibDb_AeSpeedV2_s {
    // M4_BOOL_DESC("SmoothEn", "1")
    bool                    SmoothEn;

    // M4_NUMBER_DESC("DampOver", "f32", M4_RANGE(0,1.00), "0.15", M4_DIGIT(2),M4_HIDE(0))
    float                   DampOver;

    // M4_NUMBER_DESC("DampUnder", "f32", M4_RANGE(0,1.00), "0.45", M4_DIGIT(2),M4_HIDE(0))
    float                   DampUnder;

    // M4_NUMBER_DESC("DampDark2Bright", "f32", M4_RANGE(0,1.00), "0.15", M4_DIGIT(2),M4_HIDE(0))
    float                   DampDark2Bright;

    // M4_NUMBER_DESC("DampBright2Dark", "f32", M4_RANGE(0,1.00), "0.45", M4_DIGIT(2),M4_HIDE(0))
    float                   DampBright2Dark;

    // M4_STRUCT_DESC("DyDamp", "normal_ui_style")
    CalibDb_AeDyDamp_t      DyDamp;
} CalibDb_AeSpeedV2_t;

typedef struct CalibDb_AeFrmRateAttrV2_s {
    // M4_BOOL_DESC("isFpsFix", "1")
    bool                    isFpsFix;

    // M4_NUMBER_DESC("FpsValue", "f32", M4_RANGE(0,1024), "0", M4_DIGIT(1),M4_HIDE(0))
    float                   FpsValue;

} CalibDb_AeFrmRateAttrV2_t;

typedef struct CalibDb_AntiFlickerAttrV2_s {
    // M4_BOOL_DESC("enable", "1")
    bool                           enable;

    // M4_ENUM_DESC("Frequency", "CalibDb_FlickerFreqV2_t","AECV2_FLICKER_FREQUENCY_50HZ")
    CalibDb_FlickerFreqV2_t        Frequency;

    // M4_ENUM_DESC("Mode", "CalibDb_AntiFlickerModeV2_t","AECV2_ANTIFLICKER_AUTO_MODE")
    CalibDb_AntiFlickerModeV2_t    Mode;
} CalibDb_AntiFlickerAttrV2_t;

//manual exposure
typedef struct CalibDb_LinMeAttrV2_s {
    // M4_BOOL_DESC("ManualTimeEn", "1")
    bool                 ManualTimeEn;

    // M4_BOOL_DESC("ManualGainEn", "1")
    bool                 ManualGainEn;

    // M4_BOOL_DESC("ManualIspDgainEn", "1")
    bool                 ManualIspDgainEn;

    // M4_NUMBER_DESC("TimeValue", "f32", M4_RANGE(0,1), "0.003", M4_DIGIT(6),M4_HIDE(0))
    float                TimeValue; //unit: s = 10^6 us

    // M4_NUMBER_DESC("GainValue", "f32", M4_RANGE(1,4096), "1", M4_DIGIT(3),M4_HIDE(0))
    float                GainValue;

    // M4_NUMBER_DESC("IspDGainValue", "f32", M4_RANGE(1,4096), "1", M4_DIGIT(3),M4_HIDE(0))
    float                IspDGainValue;
} CalibDb_LinMeAttrV2_t;

typedef struct CalibDb_HdrMeAttrV2_s {
    // M4_BOOL_DESC("ManualTimeEn", "1")
    bool                    ManualTimeEn;

    // M4_BOOL_DESC("ManualGainEn", "1")
    bool                    ManualGainEn;

    // M4_BOOL_DESC("ManualIspDgainEn", "1")
    bool                    ManualIspDgainEn;

    // M4_ARRAY_DESC("TimeValue", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "0.003", M4_DIGIT(6), M4_DYNAMIC(0))
    float                   TimeValue[AECV2_MAX_HDRFRAME_NUM];

    // M4_ARRAY_DESC("GainValue", "f32", M4_SIZE(1,3), M4_RANGE(1,4096), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float                   GainValue[AECV2_MAX_HDRFRAME_NUM];

    // M4_ARRAY_DESC("IspDGainValue", "f32", M4_SIZE(1,3), M4_RANGE(1,4096), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float                   IspDGainValue[AECV2_MAX_HDRFRAME_NUM];
} CalibDb_HdrMeAttrV2_t;

typedef struct CalibDb_MeAttrV2_s {
    // M4_STRUCT_DESC("LinearAE", "normal_ui_style")
    CalibDb_LinMeAttrV2_t    LinearAE;

    // M4_STRUCT_DESC("HdrAE", "normal_ui_style")
    CalibDb_HdrMeAttrV2_t    HdrAE;
} CalibDb_MeAttrV2_t;

//win_scale = [h_off, v_off, h_size, v_size], range=[0,1]
typedef struct CalibDb_Aec_WinScale_s {
    // M4_NUMBER_DESC("h_offs", "f32", M4_RANGE(0,1), "0", M4_DIGIT(2),M4_HIDE(0))
    float h_offs;

    // M4_NUMBER_DESC("v_offs", "f32", M4_RANGE(0,1), "0", M4_DIGIT(2),M4_HIDE(0))
    float v_offs;

    // M4_NUMBER_DESC("h_size", "f32", M4_RANGE(0,1), "1", M4_DIGIT(2),M4_HIDE(0))
    float h_size;

    // M4_NUMBER_DESC("v_size", "f32", M4_RANGE(0,1), "1", M4_DIGIT(2),M4_HIDE(0))
    float v_size;
} CalibDb_Aec_WinScale_t;

typedef struct CalibDb_Aec_WinScaleV2_t {

    // M4_STRUCT_DESC("InRawWinScale", "normal_ui_style")
    CalibDb_Aec_WinScale_t        InRawWinScale;

    // M4_STRUCT_DESC("TmoRawWinScale", "normal_ui_style")
    CalibDb_Aec_WinScale_t        TmoRawWinScale;

    // M4_STRUCT_DESC("YuvWinScale", "normal_ui_style")
    CalibDb_Aec_WinScale_t        YuvWinScale;
} CalibDb_Aec_WinScaleV2_t;

typedef struct CalibDb_AeEnvLvCalibV2_s {
    // M4_BOOL_DESC("Enable", "0")
    bool         Enable;

    // M4_NUMBER_DESC("CalibFNumber", "f32", M4_RANGE(0,256), "1.6", M4_DIGIT(2),M4_HIDE(0))
    float        CalibFNumber;

    // M4_ARRAY_DESC("CurveCoeff", "f32", M4_SIZE(1,2), M4_RANGE(-1024,1024), "1.0", M4_DIGIT(5), M4_DYNAMIC(0))
    float        CurveCoeff[2];//y=ax+b, Coeff=[a,b]
} CalibDb_AeEnvLvCalibV2_t;

typedef struct CalibDb_AecCommon_AttrV2_s {
    // M4_BOOL_DESC("Enable", "1")
    bool                              Enable;

    // M4_NUMBER_DESC("AecRunInterval", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0),M4_HIDE(0))
    uint8_t                           AecRunInterval;

    // M4_ENUM_DESC("AecOpType", "RKAiqOPMode_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t                     AecOpType;

    // M4_ENUM_DESC("HistStatsMode", "CalibDb_CamHistStatsModeV2_t","CAM_HISTV2_MODE_G")
    CalibDb_CamHistStatsModeV2_t      HistStatsMode;

    // M4_ENUM_DESC("RawStatsMode", "CalibDb_CamRawStatsModeV2_t","CAM_RAWSTATSV2_MODE_G")
    CalibDb_CamRawStatsModeV2_t       RawStatsMode;

    // M4_ENUM_DESC("YRangeMode", "CalibDb_CamYRangeModeV2_t","CAM_YRANGEV2_MODE_FULL")
    CalibDb_CamYRangeModeV2_t         YRangeMode;

    // M4_ARRAY_DESC("AecGridWeight", "u8", M4_SIZE(15,15), M4_RANGE(0,32), "1", M4_DIGIT(0), M4_DYNAMIC(0))
    uint8_t                           AecGridWeight[AECV2_MAX_GRIDWEIGHT_NUM];

    // M4_STRUCT_DESC("AecManualCtrl", "normal_ui_style")
    CalibDb_MeAttrV2_t                AecManualCtrl;

    // M4_STRUCT_DESC("AecSpeed", "normal_ui_style")
    CalibDb_AeSpeedV2_t               AecSpeed;

    // M4_STRUCT_DESC("AecDelay", "normal_ui_style")
    CalibDb_AeDelayFrmNumV2_t         AecDelay;

    // M4_STRUCT_DESC("AecFrameRateMode", "normal_ui_style")
    CalibDb_AeFrmRateAttrV2_t         AecFrameRateMode;

    // M4_STRUCT_DESC("AecAntiFlicker", "normal_ui_style")
    CalibDb_AntiFlickerAttrV2_t       AecAntiFlicker;

    // M4_STRUCT_DESC("AecEnvLvCalib", "normal_ui_style")
    CalibDb_AeEnvLvCalibV2_t          AecEnvLvCalib;

    // M4_STRUCT_DESC("AecWinScale", "normal_ui_style")
    CalibDb_Aec_WinScaleV2_t          AecWinScale;
} CalibDb_AecCommon_AttrV2_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC algo LinAE Config Params
 */
/*****************************************************************************/
typedef struct CalibDb_LinExpInitExpV2_s {
    // M4_NUMBER_DESC("InitTimeValue", "f32", M4_RANGE(0,1), "0.003", M4_DIGIT(6),M4_HIDE(0))
    float                   InitTimeValue;

    // M4_NUMBER_DESC("InitGainValue", "f32", M4_RANGE(1,4096), "1", M4_DIGIT(3),M4_HIDE(0))
    float                   InitGainValue;

    // M4_NUMBER_DESC("InitIspDGainValue", "f32", M4_RANGE(1,4096), "1", M4_DIGIT(3),M4_HIDE(0))
    float                   InitIspDGainValue;

    // M4_NUMBER_DESC("InitPIrisGainValue", "s32", M4_RANGE(1,4096), "1024", M4_DIGIT(0),M4_HIDE(0))
    int                     InitPIrisGainValue;

    // M4_NUMBER_DESC("InitDCIrisDutyValue", "s32", M4_RANGE(0,100), "100", M4_DIGIT(0),M4_HIDE(0))
    int                     InitDCIrisDutyValue;

    // M4_NUMBER_DESC("InitHDCIrisTargetValue", "s32", M4_RANGE(0,1023), "1023", M4_DIGIT(0),M4_HIDE(0))
    int                     InitHDCIrisTargetValue;
} CalibDb_LinExpInitExpV2_t;

typedef struct CalibDb_LinAeRoute_AttrV2_s {
    // M4_ARRAY_DESC("TimeDot", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0", M4_DIGIT(6), M4_DYNAMIC(1))
    float*                    TimeDot;
    int                       TimeDot_len;
    // M4_ARRAY_DESC("GainDot", "f32", M4_SIZE(1,6), M4_RANGE(1,4096), "1", M4_DIGIT(3), M4_DYNAMIC(1))
    float*                    GainDot;
    int                       GainDot_len;
    // M4_ARRAY_DESC("IspDGainDot", "f32", M4_SIZE(1,6), M4_RANGE(1,4096), "1", M4_DIGIT(3), M4_DYNAMIC(1))
    float*                    IspDGainDot;
    int                       IspDGainDot_len;
    // M4_ARRAY_DESC("PIrisDot", "s32", M4_SIZE(1,6), M4_RANGE(1,4096), "1024", M4_DIGIT(0), M4_DYNAMIC(1))
    int*                      PIrisDot;
    int                       PIrisDot_len;
} CalibDb_LinAeRoute_AttrV2_t;

typedef struct CalibDb_AecDynamicSetpointV2_s {
    // M4_ARRAY_DESC("ExpLevel", "f32", M4_SIZE(1,6), M4_RANGE(0,4096), "0", M4_DIGIT(5), M4_DYNAMIC(1))
    float*                    ExpLevel;
    int                       ExpLevel_len;
    // M4_ARRAY_DESC("DySetpoint", "f32", M4_SIZE(1,6), M4_RANGE(0,255), "40", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                    DySetpoint;
    int                       DySetpoint_len;
} CalibDb_AecDynamicSetpointV2_t;

typedef struct CalibDb_BacklitSetPointV2_s {
    // M4_ARRAY_DESC("ExpLevel", "f32", M4_SIZE(1,6), M4_RANGE(0,4096), "0", M4_DIGIT(5), M4_DYNAMIC(1))
    float*                     ExpLevel;
    int                        ExpLevel_len;
    // M4_ARRAY_DESC("NonOEPdfTh", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0.4", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                     NonOEPdfTh;
    int                        NonOEPdfTh_len;
    // M4_ARRAY_DESC("LowLightPdfTh", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0.2", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                     LowLightPdfTh;
    int                        LowLightPdfTh_len;
    // M4_ARRAY_DESC("TargetLLLuma", "f32", M4_SIZE(1,6), M4_RANGE(0,255), "20", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                     TargetLLLuma;
    int                        TargetLLLuma_len;
} CalibDb_BacklitSetPointV2_t;

typedef struct CalibDb_AecBacklightV2_s {
    // M4_BOOL_DESC("Enable", "0")
    bool                        Enable;

    // M4_NUMBER_DESC("StrBias", "f32", M4_RANGE(-500,500), "0", M4_DIGIT(2),M4_HIDE(1))
    float                       StrBias;//uint: %

    // M4_ENUM_DESC("MeasArea", "CalibDb_AecMeasAreaModeV2_t","AECV2_MEASURE_AREA_AUTO")
    CalibDb_AecMeasAreaModeV2_t MeasArea;

    // M4_NUMBER_DESC("OEROILowTh", "f32", M4_RANGE(0,255), "150", M4_DIGIT(2),M4_HIDE(0))
    float                       OEROILowTh;

    // M4_NUMBER_DESC("LumaDistTh", "f32", M4_RANGE(0,100), "10", M4_DIGIT(2),M4_HIDE(0))
    float                       LumaDistTh;//uint: %

    // M4_NUMBER_DESC("LvLowTh", "f32", M4_RANGE(0,100), "0.3125", M4_DIGIT(4),M4_HIDE(0))
    float                       LvLowTh;

    // M4_NUMBER_DESC("LvHighTh", "f32", M4_RANGE(0,100), "7.5", M4_DIGIT(4),M4_HIDE(0))
    float                       LvHighTh;

    // M4_ARRAY_TABLE_DESC("BacklitSetPoint", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDb_BacklitSetPointV2_t BacklitSetPoint;
} CalibDb_AecBacklightV2_t;

typedef struct CalibDb_OverExpSetPointV2_s {
    // M4_ARRAY_DESC("OEpdf", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                      OEpdf;
    int                         OEpdf_len;
    // M4_ARRAY_DESC("LowLightWeight", "f32", M4_SIZE(1,6), M4_RANGE(0,20), "1", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                      LowLightWeight;
    int                         LowLightWeight_len;
    // M4_ARRAY_DESC("HighLightWeight", "f32", M4_SIZE(1,6), M4_RANGE(0,20), "2", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                      HighLightWeight;
    int                         HighLightWeight_len;
} CalibDb_OverExpSetPointV2_t;

typedef struct CalibDb_AecOverExpCtrlV2_s {
    // M4_BOOL_DESC("Enable", "0")
    bool                       Enable;

    // M4_NUMBER_DESC("StrBias", "f32", M4_RANGE(-500,500), "0", M4_DIGIT(2),M4_HIDE(1))
    float                      StrBias; //uint: %

    // M4_NUMBER_DESC("MaxWeight", "f32", M4_RANGE(0,20), "8", M4_DIGIT(2),M4_HIDE(0))
    float                      MaxWeight;

    // M4_NUMBER_DESC("HighLightTh", "f32", M4_RANGE(0,255), "150", M4_DIGIT(2),M4_HIDE(0))
    float                      HighLightTh;

    // M4_NUMBER_DESC("LowLightTh", "f32", M4_RANGE(0,255), "30", M4_DIGIT(2),M4_HIDE(0))
    float                      LowLightTh;

    // M4_ARRAY_TABLE_DESC("OverExpSetPoint", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDb_OverExpSetPointV2_t  OverExpSetPoint;
} CalibDb_AecOverExpCtrlV2_t;

typedef struct CalibDb_LinearAE_AttrV2_s {
    // M4_BOOL_DESC("RawStatsEn", "1")
    bool                            RawStatsEn;

    // M4_NUMBER_DESC("ToleranceIn", "f32", M4_RANGE(0,100), "10", M4_DIGIT(2),M4_HIDE(0))
    float                           ToleranceIn;//uint: %

    // M4_NUMBER_DESC("ToleranceOut", "f32", M4_RANGE(0,100), "15", M4_DIGIT(2),M4_HIDE(0))
    float                           ToleranceOut;//uint: %

    // M4_NUMBER_DESC("Evbias", "f32", M4_RANGE(-500,500), "0", M4_DIGIT(2),M4_HIDE(1))
    float                           Evbias; //uint: %

    // M4_ENUM_DESC("StrategyMode", "CalibDb_AeStrategyModeV2_t","AECV2_STRATEGY_MODE_LOWLIGHT")
    CalibDb_AeStrategyModeV2_t      StrategyMode;

    // M4_STRUCT_DESC("InitExp", "normal_ui_style")
    CalibDb_LinExpInitExpV2_t       InitExp;

    // M4_ARRAY_TABLE_DESC("Route", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDb_LinAeRoute_AttrV2_t     Route;

    // M4_ARRAY_TABLE_DESC("DySetpoint", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDb_AecDynamicSetpointV2_t  DySetpoint;

    // M4_STRUCT_DESC("BackLightCtrl", "normal_ui_style")
    CalibDb_AecBacklightV2_t        BackLightCtrl;

    // M4_STRUCT_DESC("OverExpCtrl", "normal_ui_style")
    CalibDb_AecOverExpCtrlV2_t      OverExpCtrl;
} CalibDb_LinearAE_AttrV2_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC algo HdrAE Config Params
 */
/*****************************************************************************/
typedef struct CalibDb_HdrExpInitExpV2_s {
    // M4_ARRAY_DESC("InitTimeValue", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "0.01", M4_DIGIT(6), M4_DYNAMIC(0))
    float                   InitTimeValue[AECV2_MAX_HDRFRAME_NUM];

    // M4_ARRAY_DESC("InitGainValue", "f32", M4_SIZE(1,3), M4_RANGE(1,4096), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float                   InitGainValue[AECV2_MAX_HDRFRAME_NUM];

    // M4_ARRAY_DESC("InitIspDGainValue", "f32", M4_SIZE(1,3), M4_RANGE(1,4096), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float                   InitIspDGainValue[AECV2_MAX_HDRFRAME_NUM];

    // M4_NUMBER_DESC("InitPIrisGainValue", "s32", M4_RANGE(1,4096), "1024", M4_DIGIT(0),M4_HIDE(0))
    int                     InitPIrisGainValue;

    // M4_NUMBER_DESC("InitDCIrisDutyValue", "s32", M4_RANGE(0,100), "100", M4_DIGIT(0),M4_HIDE(0))
    int                     InitDCIrisDutyValue;

    // M4_NUMBER_DESC("InitHDCIrisTargetValue", "s32", M4_RANGE(0,1023), "1023", M4_DIGIT(0),M4_HIDE(0))
    int                     InitHDCIrisTargetValue;
} CalibDb_HdrExpInitExpV2_t;

typedef struct CalibDb_ExpRatioV2_s {
    // M4_ARRAY_DESC("RatioExpDot", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0", M4_DIGIT(4), M4_DYNAMIC(1))
    float*            RatioExpDot;
    int               RatioExpDot_len;
    // M4_ARRAY_DESC("M2SRatioFix", "f32", M4_SIZE(1,6), M4_RANGE(1,256), "8", M4_DIGIT(2), M4_DYNAMIC(1))
    float*            M2SRatioFix;
    int               M2SRatioFix_len;
    // M4_ARRAY_DESC("L2MRatioFix", "f32", M4_SIZE(1,6), M4_RANGE(1,256), "8", M4_DIGIT(2), M4_DYNAMIC(1))
    float*            L2MRatioFix;
    int               L2MRatioFix_len;
    // M4_ARRAY_DESC("M2SRatioMax", "f32", M4_SIZE(1,6), M4_RANGE(1,256), "64", M4_DIGIT(2), M4_DYNAMIC(1))
    float*            M2SRatioMax;
    int               M2SRatioMax_len;
    // M4_ARRAY_DESC("L2MRatioMax", "f32", M4_SIZE(1,6), M4_RANGE(1,256), "64", M4_DIGIT(2), M4_DYNAMIC(1))
    float*            L2MRatioMax;
    int               L2MRatioMax_len;
} CalibDb_ExpRatioV2_t;

typedef struct CalibDb_ExpRatioCtrlV2_s {
    // M4_ENUM_DESC("ExpRatioType", "CalibDb_HdrAeRatioTypeV2_t","AECV2_HDR_RATIOTYPE_MODE_AUTO")
    CalibDb_HdrAeRatioTypeV2_t     ExpRatioType;

    // M4_ARRAY_TABLE_DESC("ExpRatio", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDb_ExpRatioV2_t           ExpRatio;
} CalibDb_ExpRatioCtrlV2_t;

typedef struct CalibDb_HdrAeRoute_AttrV2_s {
    // M4_ARRAY_DESC("Frm0TimeDot", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0", M4_DIGIT(6), M4_DYNAMIC(1))
    float*                    Frm0TimeDot;
    int                       Frm0TimeDot_len;
    // M4_ARRAY_DESC("Frm0GainDot", "f32", M4_SIZE(1,6), M4_RANGE(1,4096), "1", M4_DIGIT(3), M4_DYNAMIC(1))
    float*                    Frm0GainDot;
    int                       Frm0GainDot_len;
    // M4_ARRAY_DESC("Frm0IspDGainDot", "f32", M4_SIZE(1,6), M4_RANGE(1,4096), "1", M4_DIGIT(3), M4_DYNAMIC(1))
    float*                    Frm0IspDGainDot;
    int                       Frm0IspDGainDot_len;
    // M4_ARRAY_DESC("Frm1TimeDot", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0", M4_DIGIT(6), M4_DYNAMIC(1))
    float*                    Frm1TimeDot;
    int                       Frm1TimeDot_len;
    // M4_ARRAY_DESC("Frm1GainDot", "f32", M4_SIZE(1,6), M4_RANGE(1,4096), "1", M4_DIGIT(3), M4_DYNAMIC(1))
    float*                    Frm1GainDot;
    int                       Frm1GainDot_len;
    // M4_ARRAY_DESC("Frm1IspDGainDot", "f32", M4_SIZE(1,6), M4_RANGE(1,4096), "1", M4_DIGIT(3), M4_DYNAMIC(1))
    float*                    Frm1IspDGainDot;
    int                       Frm1IspDGainDot_len;
    // M4_ARRAY_DESC("Frm2TimeDot", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0", M4_DIGIT(6), M4_DYNAMIC(1))
    float*                    Frm2TimeDot;
    int                       Frm2TimeDot_len;
    // M4_ARRAY_DESC("Frm2GainDot", "f32", M4_SIZE(1,6), M4_RANGE(1,4096), "1", M4_DIGIT(3), M4_DYNAMIC(1))
    float*                    Frm2GainDot;
    int                       Frm2GainDot_len;
    // M4_ARRAY_DESC("Frm2IspDGainDot", "f32", M4_SIZE(1,6), M4_RANGE(1,4096), "1", M4_DIGIT(3), M4_DYNAMIC(1))
    float*                    Frm2IspDGainDot;
    int                       Frm2IspDGainDot_len;
    // M4_ARRAY_DESC("PIrisDot", "s32", M4_SIZE(1,6), M4_RANGE(1,4096), "1024", M4_DIGIT(0), M4_DYNAMIC(1))
    int*                      PIrisDot;
    int                       PIrisDot_len;
} CalibDb_HdrAeRoute_AttrV2_t;

typedef struct CalibDb_LfrmSetPointV2_s
{
    // M4_ARRAY_DESC("LExpLevel", "f32", M4_SIZE(1,6), M4_RANGE(0,4096), "0", M4_DIGIT(5), M4_DYNAMIC(1))
    float*                      LExpLevel;
    int                         LExpLevel_len;
    // M4_ARRAY_DESC("NonOEPdfTh", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0.4", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                      NonOEPdfTh;
    int                         NonOEPdfTh_len;
    // M4_ARRAY_DESC("LowLightPdfTh", "f32", M4_SIZE(1,6), M4_RANGE(0,1), "0.2", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                      LowLightPdfTh;
    int                         LowLightPdfTh_len;
    // M4_ARRAY_DESC("LSetPoint", "f32", M4_SIZE(1,6), M4_RANGE(0,255), "40", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                      LSetPoint;
    int                         LSetPoint_len;
    // M4_ARRAY_DESC("TargetLLLuma", "f32", M4_SIZE(1,6), M4_RANGE(0,255), "20", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                      TargetLLLuma;
    int                         TargetLLLuma_len;
} CalibDb_LfrmSetPointV2_t;

typedef struct CalibDb_LfrmCtrlV2_s
{
    // M4_NUMBER_DESC("OEROILowTh", "f32", M4_RANGE(0,255), "150", M4_DIGIT(2),M4_HIDE(0))
    float                       OEROILowTh;

    // M4_NUMBER_DESC("LvLowTh", "f32", M4_RANGE(0,100), "0.3125", M4_DIGIT(4),M4_HIDE(0))
    float                       LvLowTh;

    // M4_NUMBER_DESC("LvHighTh", "f32", M4_RANGE(0,100), "7.5", M4_DIGIT(4),M4_HIDE(0))
    float                       LvHighTh;

    // M4_ARRAY_TABLE_DESC("LfrmSetPoint", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDb_LfrmSetPointV2_t    LfrmSetPoint;
} CalibDb_LfrmCtrlV2_t;

typedef struct CalibDb_MfrmCtrlV2_s
{
    // M4_ARRAY_DESC("MExpLevel", "f32", M4_SIZE(1,6), M4_RANGE(0,4096), "0", M4_DIGIT(5), M4_DYNAMIC(1))
    float*                  MExpLevel;
    int                     MExpLevel_len;
    // M4_ARRAY_DESC("MSetPoint", "f32", M4_SIZE(1,6), M4_RANGE(0,255), "40", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                  MSetPoint;
    int                     MSetPoint_len;
} CalibDb_MfrmCtrlV2_t;

typedef struct CalibDb_SfrmSetPointV2_s
{
    // M4_ARRAY_DESC("SExpLevel", "f32", M4_SIZE(1,6), M4_RANGE(0,4096), "0", M4_DIGIT(6), M4_DYNAMIC(1))
    float*                 SExpLevel;
    int                    SExpLevel_len;
    // M4_ARRAY_DESC("SSetPoint", "f32", M4_SIZE(1,6), M4_RANGE(0,255), "20", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                 SSetPoint;
    int                    SSetPoint_len;
    // M4_ARRAY_DESC("TargetHLLuma", "f32", M4_SIZE(1,6), M4_RANGE(0,255), "150", M4_DIGIT(2), M4_DYNAMIC(1))
    float*                 TargetHLLuma;
    int                    TargetHLLuma_len;
} CalibDb_SfrmSetPointV2_t;

typedef struct CalibDb_SfrmCtrlV2_s
{
    // M4_BOOL_DESC("HLROIExpandEn", "0")
    bool                        HLROIExpandEn;

    // M4_NUMBER_DESC("HLLumaTolerance", "f32", M4_RANGE(0,100), "12", M4_DIGIT(2),M4_HIDE(0))
    float                       HLLumaTolerance;//unit:%

    // M4_ARRAY_TABLE_DESC("SfrmSetPoint", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDb_SfrmSetPointV2_t    SfrmSetPoint;
} CalibDb_SfrmCtrlV2_t;

typedef struct CalibDb_LongFrmCtrlV2_s {
    // M4_ENUM_DESC("mode", "CalibDb_AeHdrLongFrmModeV2_t","AECV2_HDR_LONGFRMMODE_DISABLE")
    CalibDb_AeHdrLongFrmModeV2_t    mode;

    // M4_NUMBER_DESC("SfrmMinLine", "u16", M4_RANGE(0,1024), "2", M4_DIGIT(0),M4_HIDE(0))
    uint16_t                        SfrmMinLine;//uint:line

    // M4_NUMBER_DESC("LfrmModeExpTh", "f32", M4_RANGE(0,100), "0.62", M4_DIGIT(4),M4_HIDE(0))
    float                           LfrmModeExpTh;
} CalibDb_LongFrmCtrlV2_t;

typedef struct CalibDb_HdrAE_AttrV2_s {
    // M4_NUMBER_DESC("ToleranceIn", "f32", M4_RANGE(0,100), "10", M4_DIGIT(2),M4_HIDE(0))
    float                           ToleranceIn;//unit:%

    // M4_NUMBER_DESC("ToleranceOut", "f32", M4_RANGE(0,100), "15", M4_DIGIT(2),M4_HIDE(0))
    float                           ToleranceOut;//unit:%

    // M4_NUMBER_DESC("Evbias", "f32", M4_RANGE(-500,500), "0", M4_DIGIT(0),M4_HIDE(1))
    float                           Evbias;

    // M4_ENUM_DESC("StrategyMode", "CalibDb_AeStrategyModeV2_t","AECV2_STRATEGY_MODE_LOWLIGHT")
    CalibDb_AeStrategyModeV2_t      StrategyMode;

    // M4_NUMBER_DESC("LumaDistTh", "f32", M4_RANGE(0,100), "10", M4_DIGIT(2),M4_HIDE(0))
    float                           LumaDistTh; //used for area-growing,unit:%

    // M4_STRUCT_DESC("InitExp", "normal_ui_style")
    CalibDb_HdrExpInitExpV2_t       InitExp;

    // M4_ARRAY_TABLE_DESC("Route", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDb_HdrAeRoute_AttrV2_t     Route;

    // M4_STRUCT_DESC("ExpRatioCtrl", "normal_ui_style")
    CalibDb_ExpRatioCtrlV2_t        ExpRatioCtrl;

    // M4_STRUCT_DESC("LongFrmMode", "normal_ui_style")
    CalibDb_LongFrmCtrlV2_t         LongFrmMode;

    // M4_STRUCT_DESC("LframeCtrl", "normal_ui_style")
    CalibDb_LfrmCtrlV2_t            LframeCtrl;

    // M4_ARRAY_TABLE_DESC("MframeCtrl", "array_table_ui", M4_INDEX_DEFAULT)
    CalibDb_MfrmCtrlV2_t            MframeCtrl;

    // M4_STRUCT_DESC("SframeCtrl", "normal_ui_style")
    CalibDb_SfrmCtrlV2_t            SframeCtrl;
} CalibDb_HdrAE_AttrV2_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC algo SyncTest Config Params
 */
/*****************************************************************************/
typedef struct CalibDb_LinAlterExpV2_s {
    // M4_NUMBER_DESC("TimeValue", "f32", M4_RANGE(0,1), "0.01", M4_DIGIT(6),M4_HIDE(0))
    float                    TimeValue;

    // M4_NUMBER_DESC("GainValue", "f32", M4_RANGE(1,4096), "1", M4_DIGIT(3),M4_HIDE(0))
    float                    GainValue;

    // M4_NUMBER_DESC("IspDGainValue", "f32", M4_RANGE(1,4096), "1", M4_DIGIT(3),M4_HIDE(0))
    float                    IspDGainValue;

    // M4_NUMBER_DESC("PIrisGainValue", "s32", M4_RANGE(1,4096), "1024", M4_DIGIT(0),M4_HIDE(0))
    int                      PIrisGainValue;

    // M4_NUMBER_DESC("DcgMode", "s32", M4_RANGE(-1,1), "0", M4_DIGIT(0),M4_HIDE(0))
    int                      DcgMode;
} CalibDb_LinAlterExpV2_t;

typedef struct CalibDb_HdrAlterExpV2_s {
    // M4_ARRAY_DESC("TimeValue", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "0.01", M4_DIGIT(6), M4_DYNAMIC(0))
    float                    TimeValue[AECV2_MAX_HDRFRAME_NUM];

    // M4_ARRAY_DESC("GainValue", "f32", M4_SIZE(1,3), M4_RANGE(1,4096), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float                    GainValue[AECV2_MAX_HDRFRAME_NUM];

    // M4_ARRAY_DESC("IspDGainValue", "f32", M4_SIZE(1,3), M4_RANGE(1,4096), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float                    IspDGainValue[AECV2_MAX_HDRFRAME_NUM];

    // M4_NUMBER_DESC("PIrisGainValues", "s32", M4_RANGE(1,4096), "1024", M4_DIGIT(0),M4_HIDE(0))
    int                      PIrisGainValue;

    // M4_ARRAY_DESC("DcgMode", "s32", M4_SIZE(1,3), M4_RANGE(-1,1), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int                      DcgMode[AECV2_MAX_HDRFRAME_NUM];
} CalibDb_HdrAlterExpV2_t;

typedef struct CalibDb_AlterExpV2_s {
    // M4_STRUCT_LIST_DESC("LinearAE", M4_SIZE_DYNAMIC,"normal_ui_style")
    CalibDb_LinAlterExpV2_t*       LinearAE;
    int                            LinearAE_len;

    // M4_STRUCT_LIST_DESC("HdrAE", M4_SIZE_DYNAMIC,"normal_ui_style")
    CalibDb_HdrAlterExpV2_t*       HdrAE;
    int                            HdrAE_len;
} CalibDb_AlterExpV2_t;

typedef struct CalibDb_AeSyncTestV2_s {
    // M4_BOOL_DESC("Enable", "0")
    bool                        Enable;

    // M4_NUMBER_DESC("IntervalFrm", "s32", M4_RANGE(0,1024), "5", M4_DIGIT(0),M4_HIDE(0))
    int                         IntervalFrm;

    // M4_STRUCT_DESC("AlterExp", "normal_ui_style")
    CalibDb_AlterExpV2_t        AlterExp;
} CalibDb_AeSyncTestV2_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC algo IrisCtrl Config Params
 */
/*****************************************************************************/
typedef struct CalibDb_MIris_AttrV2_s {
    // M4_NUMBER_DESC("PIrisGainValue", "s32", M4_RANGE(1,4096), "1024", M4_DIGIT(0),M4_HIDE(0))
    int                  PIrisGainValue;

    // M4_NUMBER_DESC("DCIrisHoldValue", "s32", M4_RANGE(0,100), "50", M4_DIGIT(0),M4_HIDE(0))
    int                  DCIrisHoldValue;

    // M4_NUMBER_DESC("HDCIrisTargetValue", "s32", M4_RANGE(0,1023), "1023", M4_DIGIT(0),M4_HIDE(0))
    int                  HDCIrisTargetValue;
} CalibDb_MIris_AttrV2_t;

typedef struct CalibDb_PIris_AttrV2_s {
    // M4_NUMBER_DESC("TotalStep", "u16", M4_RANGE(0,1024), "81", M4_DIGIT(0),M4_HIDE(0))
    uint16_t             TotalStep;

    // M4_NUMBER_DESC("EffcStep", "u16", M4_RANGE(0,1024), "64", M4_DIGIT(0),M4_HIDE(0))
    uint16_t             EffcStep;

    // M4_BOOL_DESC("ZeroIsMax", "1")
    bool                 ZeroIsMax;

    // M4_ARRAY_DESC("StepTable", "u16", M4_SIZE(1,1024), M4_RANGE(1,4096), "1024", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t             StepTable[AECV2_PIRIS_STEP_TABLE_MAX];
} CalibDb_PIris_AttrV2_t;

typedef struct CalibDb_DCIris_AttrV2_s {
    // M4_NUMBER_DESC("Kp", "f32", M4_RANGE(0,10), "1", M4_DIGIT(2),M4_HIDE(0))
    float       Kp;
    // M4_NUMBER_DESC("Ki", "f32", M4_RANGE(0,10), "0.2", M4_DIGIT(2),M4_HIDE(0))
    float       Ki;
    // M4_NUMBER_DESC("Kd", "f32", M4_RANGE(0,10), "0.5", M4_DIGIT(2),M4_HIDE(0))
    float       Kd;
    // M4_NUMBER_DESC("MinPwmDuty", "s32", M4_RANGE(0,100), "0", M4_DIGIT(0),M4_HIDE(0))
    int         MinPwmDuty;
    // M4_NUMBER_DESC("MaxPwmDuty", "s32", M4_RANGE(0,100), "100", M4_DIGIT(0),M4_HIDE(0))
    int         MaxPwmDuty;
    // M4_NUMBER_DESC("OpenPwmDuty", "s32", M4_RANGE(0,100), "50", M4_DIGIT(0),M4_HIDE(0))
    int         OpenPwmDuty;
    // M4_NUMBER_DESC("ClosePwmDuty", "s32", M4_RANGE(0,100), "60", M4_DIGIT(0),M4_HIDE(0))
    int         ClosePwmDuty;
} CalibDb_DCIris_AttrV2_t;

typedef struct CalibDb_HDCIris_AttrV2_s {
    // M4_NUMBER_DESC("DampOver", "f32", M4_RANGE(0,1), "0.15", M4_DIGIT(2),M4_HIDE(0))
    float       DampOver;

    // M4_NUMBER_DESC("DampUnder", "f32", M4_RANGE(0,1), "0.45", M4_DIGIT(2),M4_HIDE(0))
    float       DampUnder;

    // M4_BOOL_DESC("ZeroIsMax", "0")
    bool        ZeroIsMax;

    // M4_NUMBER_DESC("MinTarget", "s32", M4_RANGE(0,1023), "0", M4_DIGIT(0),M4_HIDE(0))
    int         MinTarget;

    // M4_NUMBER_DESC("MaxTarget", "s32", M4_RANGE(0,1023), "1023", M4_DIGIT(0),M4_HIDE(0))
    int         MaxTarget;

    // M4_ARRAY_DESC("ZoomTargetDot", "s32", M4_SIZE(1,256), M4_RANGE(0,1023), "1023", M4_DIGIT(0), M4_DYNAMIC(1),M4_HIDE(0))
    int         ZoomTargetDot[AECV2_HDCIRIS_DOT_MAX_NUM];

    // M4_ARRAY_DESC("ZoomDot", "s32", M4_SIZE(1,256), M4_RANGE(-32768,32767), "0", M4_DIGIT(0), M4_DYNAMIC(1),M4_HIDE(0))
    int         ZoomDot[AECV2_HDCIRIS_DOT_MAX_NUM];

    // M4_ARRAY_DESC("TargetDot", "s32", M4_SIZE(1,256), M4_RANGE(0,1023), "1023", M4_DIGIT(0), M4_DYNAMIC(1),M4_HIDE(0))
    int         IrisTargetDot[AECV2_HDCIRIS_DOT_MAX_NUM];

    // M4_ARRAY_DESC("GainDot", "s32", M4_SIZE(1,256), M4_RANGE(1,512), "512", M4_DIGIT(0), M4_DYNAMIC(1),M4_HIDE(0))
    int         GainDot[AECV2_HDCIRIS_DOT_MAX_NUM];

    // M4_NUMBER_DESC("ZoomTargetDot_len", "s32", M4_RANGE(1,256), "12", M4_DIGIT(0),M4_HIDE(0))
    int         ZoomTargetDot_len;

    // M4_NUMBER_DESC("IrisTargetDot_len", "s32", M4_RANGE(1,256), "12", M4_DIGIT(0),M4_HIDE(0))
    int         IrisTargetDot_len;
} CalibDb_HDCIris_AttrV2_t;

typedef struct CalibDb_AecIrisCtrlV2_s {
    // M4_BOOL_DESC("Enable", "0")
    bool                         Enable;

    // M4_ENUM_DESC("IrisType", "CalibDb_IrisTypeV2_t","IRISV2_DC_TYPE")
    CalibDb_IrisTypeV2_t         IrisType;

    // M4_BOOL_DESC("ManualEn", "0")
    bool                         ManualEn;

    // M4_STRUCT_DESC("ManualAttr", "normal_ui_style")
    CalibDb_MIris_AttrV2_t       ManualAttr;

    // M4_STRUCT_DESC("InitAttr", "normal_ui_style")
    CalibDb_MIris_AttrV2_t       InitAttr;

    // M4_STRUCT_DESC("PIrisAttr", "normal_ui_style")
    CalibDb_PIris_AttrV2_t       PIrisAttr;

    // M4_STRUCT_DESC("DCIrisAttr", "normal_ui_style")
    CalibDb_DCIris_AttrV2_t      DCIrisAttr;

    // M4_STRUCT_DESC("HDCIrisAttr", "normal_ui_style")
    CalibDb_HDCIris_AttrV2_t     HDCIrisAttr;
} CalibDb_AecIrisCtrlV2_t;

/*****************************************************************************/
/**
 * @brief   Global AEC tuning structure of isp2.0
 */
/*****************************************************************************/
typedef struct CalibDb_Aec_ParaV2_s {
    // M4_STRUCT_DESC("CommCtrl", "normal_ui_style")
    CalibDb_AecCommon_AttrV2_t          CommCtrl;

    // M4_STRUCT_DESC("LinearAeCtrl", "normal_ui_style")
    CalibDb_LinearAE_AttrV2_t           LinearAeCtrl;

    // M4_STRUCT_DESC("HdrAeCtrl", "normal_ui_style")
    CalibDb_HdrAE_AttrV2_t              HdrAeCtrl;

    // M4_STRUCT_DESC("IrisCtrl", "normal_ui_style")
    CalibDb_AecIrisCtrlV2_t             IrisCtrl;

    // M4_STRUCT_DESC("SyncTest", "normal_ui_style")
    CalibDb_AeSyncTestV2_t              SyncTest; //special module for debug
} CalibDb_Aec_ParaV2_t;

RKAIQ_END_DECLARE

#endif
