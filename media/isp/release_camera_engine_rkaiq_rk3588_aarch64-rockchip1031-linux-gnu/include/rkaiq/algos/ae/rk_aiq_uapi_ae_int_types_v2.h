#ifndef _RK_AIQ_UAPI_AE_INT_TYPES_V2_H_
#define _RK_AIQ_UAPI_AE_INT_TYPES_V2_H_
#include "rk_aiq_algo_des.h"
#include "rk_aiq_types_ae_algo_int.h"

/***************************  ATTENTION:  ************************************/
/*
 *      There are two sets of api-strcut defined, isp2.0/isp2.1 respectively.
 *      AE-API Needs to be compatible with isp2.0 & isp2.1 struct
 */
/*****************************************************************************/

/*****************************************************************************/
/**
 * @brief   ISP2.1 AEC API ExpSwAttr_t Params
 */
/*****************************************************************************/
typedef CalibDb_AeSpeedV2_t Uapi_AeSpeedV2_t;

typedef Aec_AeRange_t Uapi_AeRangeV2_t;

typedef Aec_LinAeRange_t Uapi_LinAeRangeV2_t;

typedef Aec_HdrAeRange_t Uapi_HdrAeRangeV2_t;

typedef CalibDb_AeFrmRateAttrV2_t Uapi_AeFpsAttrV2_t;

typedef CalibDb_AntiFlickerAttrV2_t Uapi_AntiFlickerV2_t;

typedef struct Uapi_AeAttrV2_s {

    Uapi_AeSpeedV2_t           stAeSpeed;
    //DelayFrmNum
    uint8_t                  BlackDelayFrame;
    uint8_t                  WhiteDelayFrame;
    //Auto/Fixed fps
    Uapi_AeFpsAttrV2_t         stFrmRate;
    Uapi_AntiFlickerV2_t       stAntiFlicker;
    //auto range
    Aec_LinAeRange_t              LinAeRange;//result LinAerange
    Aec_HdrAeRange_t              HdrAeRange;//result HdrAerange
} Uapi_AeAttrV2_t;

//manual exposure
typedef struct Uapi_LinMeAttrV2_s {
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
} Uapi_LinMeAttrV2_t;

typedef struct Uapi_HdrMeAttrV2_s {
    // M4_BOOL_DESC("ManualTimeEn", "1")
    bool                    ManualTimeEn;

    // M4_BOOL_DESC("ManualGainEn", "1")
    bool                    ManualGainEn;

    // M4_BOOL_DESC("ManualIspDgainEn", "1")
    bool                    ManualIspDgainEn;

    // M4_ARRAY_DESC("TimeValue", "f32", M4_SIZE(1,3), M4_RANGE(0,1), "0.003", M4_DIGIT(6), M4_DYNAMIC(0))
    float                   TimeValue[MAX_HDR_FRAMENUM];

    // M4_ARRAY_DESC("GainValue", "f32", M4_SIZE(1,3), M4_RANGE(1,4096), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float                   GainValue[MAX_HDR_FRAMENUM];

    // M4_ARRAY_DESC("IspDGainValue", "f32", M4_SIZE(1,3), M4_RANGE(1,4096), "1.0", M4_DIGIT(3), M4_DYNAMIC(0))
    float                   IspDGainValue[MAX_HDR_FRAMENUM];
} Uapi_HdrMeAttrV2_t;

typedef struct Uapi_MeAttrV2_s {
    // M4_STRUCT_DESC("LinearAE", "normal_ui_style")
    Uapi_LinMeAttrV2_t    LinearAE;

    // M4_STRUCT_DESC("HdrAE", "normal_ui_style")
    Uapi_HdrMeAttrV2_t    HdrAE;
} Uapi_MeAttrV2_t;

typedef Aec_uapi_advanced_attr_t Uapi_ExpSwAttr_AdvancedV2_t;

typedef struct Uapi_ExpSwAttrV2_s {
    rk_aiq_uapi_sync_t                 sync;
    uint8_t                            Enable;
    CalibDb_CamRawStatsModeV2_t        RawStatsMode;
    CalibDb_CamHistStatsModeV2_t       HistStatsMode;
    CalibDb_CamYRangeModeV2_t          YRangeMode;
    uint8_t                  AecRunInterval;

    // M4_ENUM_DESC("OPMode", "RKAiqOPMode_t","RK_AIQ_OP_MODE_AUTO")
    RKAiqOPMode_t            AecOpType;
    Cam15x15UCharMatrix_t    GridWeights;
    Uapi_AeAttrV2_t          stAuto;

    // M4_STRUCT_DESC("ManualCtrl", "normal_ui_style")
    Uapi_MeAttrV2_t          stManual;

    Uapi_ExpSwAttr_AdvancedV2_t stAdvanced;
} Uapi_ExpSwAttrV2_t;

/*****************************************************************************/
/**
 * @brief   ISP2.1 AEC API LinAeRouteAttr/HdrAeRouteAttr Params
 */
/*****************************************************************************/
typedef struct Uapi_LinAeRouteAttr_s {
    rk_aiq_uapi_sync_t            sync;
    CalibDb_LinAeRoute_AttrV2_t   Params;
} Uapi_LinAeRouteAttr_t;

typedef struct Uapi_HdrAeRouteAttr_s {
    rk_aiq_uapi_sync_t            sync;
    CalibDb_HdrAeRoute_AttrV2_t   Params;
} Uapi_HdrAeRouteAttr_t;

/*****************************************************************************/
/**
 * @brief   ISP2.1 AEC API IrisAttr Params
 */
/*****************************************************************************/
typedef struct Uapi_IrisAttrV2_s {
    rk_aiq_uapi_sync_t            sync;
    CalibDb_AecIrisCtrlV2_t       Params;
} Uapi_IrisAttrV2_t;

/*****************************************************************************/
/**
 * @brief   ISP2.1 AEC API AecSyncTest Params
 */
/*****************************************************************************/
typedef struct Uapi_AecSyncTest_s {
    rk_aiq_uapi_sync_t            sync;
    CalibDb_AeSyncTestV2_t        Params;
} Uapi_AecSyncTest_t;

/*****************************************************************************/
/**
 * @brief   ISP2.1 AEC API LinExpAttr/HdrExpAttr Params
 */
/*****************************************************************************/
typedef struct Uapi_LinExpAttrV2_s {
    rk_aiq_uapi_sync_t            sync;
    CalibDb_LinearAE_AttrV2_t     Params;
} Uapi_LinExpAttrV2_t;

typedef struct Uapi_HdrExpAttrV2_s {
    rk_aiq_uapi_sync_t            sync;
    CalibDb_HdrAE_AttrV2_t        Params;
} Uapi_HdrExpAttrV2_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0/2.1 AEC API ExpHwAttr Params
 */
/*****************************************************************************/
typedef struct Uapi_ExpWin_s {
    rk_aiq_uapi_sync_t            sync;
    window_t                      Params;
} Uapi_ExpWin_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0/2.1 AEC API ExpQueryInfo Params
 */
/*****************************************************************************/
typedef struct Uapi_ExpQueryInfo_s {
    // M4_BOOL_DESC("IsConverged", "0",M4_HIDE(1))
    bool              IsConverged;
    // M4_BOOL_DESC("IsExpMax", "0",M4_HIDE(1))
    bool              IsExpMax;
    // M4_NUMBER_DESC("LumaDeviation", "f32", M4_RANGE(-256,256), "0.0", M4_DIGIT(3),M4_HIDE(1))
    float             LumaDeviation;
    // M4_ARRAY_DESC("HdrLumaDeviation", "f32", M4_SIZE(1,3), M4_RANGE(-256,256), "0.0", M4_DIGIT(3), M4_DYNAMIC(0), M4_HIDE(1))
    float             HdrLumaDeviation[3];
    // M4_NUMBER_DESC("MeanLuma", "f32", M4_RANGE(0,256), "0.0", M4_DIGIT(2))
    float             MeanLuma;
    // M4_ARRAY_DESC("HdrMeanLuma", "f32", M4_SIZE(1,3), M4_RANGE(0,256), "0.0", M4_DIGIT(2), M4_DYNAMIC(0))
    float             HdrMeanLuma[3];
    // M4_NUMBER_DESC("GlobalEnvLux", "f32", M4_RANGE(0,65535), "0.0", M4_DIGIT(2),M4_HIDE(1))
    float             GlobalEnvLux;
    // M4_ARRAY_DESC("BlockEnvLux", "f32", M4_SIZE(15,15), M4_RANGE(0,65535), "0.0", M4_DIGIT(2), M4_DYNAMIC(0), M4_HIDE(1))
    float             BlockEnvLux[ISP2_RAWAE_WINNUM_MAX];
    // M4_STRUCT_DESC("CurExpInfo", "normal_ui_style");
    RKAiqAecExpInfo_t CurExpInfo;

    // M4_NUMBER_DESC("Piris", "u16", M4_RANGE(0,1024), "0", M4_DIGIT(0),M4_HIDE(1))
    unsigned short    Piris;

    // M4_NUMBER_DESC("LinePeriodsPerField", "f32", M4_RANGE(0,65535), "0", M4_DIGIT(2),M4_HIDE(1))
    float             LinePeriodsPerField;

    // M4_NUMBER_DESC("PixelPeriodsPerLine", "f32", M4_RANGE(0,65535), "0", M4_DIGIT(2),M4_HIDE(1))
    float             PixelPeriodsPerLine;

    // M4_NUMBER_DESC("PixelClockFreqMHZ", "f32", M4_RANGE(0,4096), "0", M4_DIGIT(2),M4_HIDE(1))
    float             PixelClockFreqMHZ;

} Uapi_ExpQueryInfo_t;


#endif
