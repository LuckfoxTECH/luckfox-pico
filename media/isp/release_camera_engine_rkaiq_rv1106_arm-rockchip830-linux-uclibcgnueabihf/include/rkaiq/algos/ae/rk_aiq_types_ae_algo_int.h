/*
* rk_aiq_types_ae_algo_int.h

* for rockchip v2.0.0
*
*  Copyright (c) 2019 Rockchip Corporation
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
/* for rockchip v2.0.0*/

#ifndef __RK_AIQ_TYPES_AE_ALGO_INT_H__
#define __RK_AIQ_TYPES_AE_ALGO_INT_H__

/**
 * @file rk_aiq_types_ae_algo_int.h
 *
 * @brief
 *
 *****************************************************************************/
/**
 * @page module_name_page Module Name
 * Describe here what this module does.
 *
 * For a detailed list of functions and implementation detail refer to:
 * - @ref module_name
 *
 * @defgroup AEC Auto exposure control Module
 * @{
 *
 */
#include "moduleinfo_head.h"
#include "aec_head.h"
#include "sensorinfo_head.h"
#include "rk_aiq_types_ae_algo.h"


#define MAX_HDR_FRAMENUM  (3)
/*****************************************************************************/
/**
 * @brief   The number of mean and hist.
 */
/*****************************************************************************/
#define ISP2_RAWAE_WINNUM_MAX  ((RAWAELITE_WIN_NUM > RAWAEBIG_WIN_NUM) ? RAWAELITE_WIN_NUM : RAWAEBIG_WIN_NUM)
#define ISP2_HIST_BINNUM_MAX  ((RAWHIST_BIN_N_MAX > SIHIST_BIN_N_MAX) ? RAWHIST_BIN_N_MAX : SIHIST_BIN_N_MAX)

/*****************************************************************************/
/**
 * predefined flicker period values for ECM module
 */
/*****************************************************************************/
#define ECM_TFLICKER_50HZ (1.0/100.0)                                  //!< predefined flicker period value for ECM module
#define ECM_TFLICKER_60HZ (1.0/120.0)                                  //!< predefined flicker period value for ECM module
#define ECM_DOT_NO         (6)

/*****************************************************************************/
/**
 * @brief   This typedef represents the histogram which is measured by the
 *          CamerIC ISP histogram module.
 *
 *****************************************************************************/
typedef uint32_t CamerIcHistBins_t[ISP2_HIST_BINNUM_MAX];

/*****************************************************************************/
/**
 * @brief   Array type for ISP EXP measurment values.
 *          CamerIC ISP RawAe grid-luma module.
 *
 *****************************************************************************/
typedef uint8_t CamerIcLiteMeanLuma_t[RAWAELITE_WIN_NUM];
typedef uint8_t CamerIcBigMeanLuma_t[RAWAEBIG_WIN_NUM];

/*****************************************************************************/
/**
 * @brief   log level of ae
 */
/*****************************************************************************/
typedef enum {
    PREPARE = 0x01,
    PREPROC = 0x02,
    PROC = 0x04,
    POST = 0x08,
    LDM = 0x10,
    CLM = 0x20,
    ECM = 0x40,
    HDRECM = 0x80,
} AecLogLevel_t;
/*****************************************************************************/
/**
 * @brief   update config of ae
 */
/*****************************************************************************/
typedef enum {
    UPDATE_PREPARE   = 0x00, //including configtype=init/changeworingmode/reset
    UPDATE_EXPSWATTR = 0x01,
    UPDATE_LINEXPATTR = 0x02,
    UPDATE_HDREXPATTR = 0x04,
    UPDATE_LINAEROUTEATTR = 0x08,
    UPDATE_HDRAEROUTEATTR = 0x10,
    UPDATE_IRISATTR = 0x20,
    UPDATE_SYNCTESTATTR = 0x40,
    UPDATE_EXPWINATTR = 0x80,
    UPDATE_EXPINFO = 0x100, //including vts/dcg
    UPDATE_EXPHWATTR = 0x200, //including aec_meas/hist_meass
    UPDATE_RESOLUTION = 0x400,
    UPDATE_CALIB = 0xffff,//update iq file
} AecUpdateAttrMode_t;

/*****************************************************************************/
/**
 * @brief   CamerIcIspStats related params
 */
/*****************************************************************************/
typedef enum CamIcYRangeMode_e {
    CAMIC_YRANGE_MODE_INVALID    = 0,    /**< invalid y range mode   */
    CAMIC_YRANGE_MODE_FULL       = 1,    /**< Y = 0.299 * R + 0.587 * G + 0.114 * B,full range BT601*/
    CAMIC_YRANGE_MODE_LIMITED    = 2,    /**< Y = 16 + 0.25R + 0.5G + 0.1094B,limited range*/
    CAMIC_YRANGE_MODE_MAX,
} CamIcYRangeMode_t;

typedef enum CamIcRawStatsMode_e {
    CAMIC_RAWSTATS_MODE_INVALID       = 0,    /**< lower border (only for an internal evaluation) */
    CAMIC_RAWSTATS_MODE_R             = 1,    /**< R channel */
    CAMIC_RAWSTATS_MODE_G             = 2,    /**< G channel */
    CAMIC_RAWSTATS_MODE_B             = 3,    /**< B channel */
    CAMIC_RAWSTATS_MODE_Y             = 4,    /**< luminance channel */
    CAMIC_RAWSTATS_MODE_MAX,                                  /**< upper border (only for an internal evaluation) */
} CamIcRawStatsMode_t;

typedef enum CamIcHistStatsMode_e {
    CAMIC_HIST_MODE_INVALID       = 0,    /**< lower border (only for an internal evaluation) */
    CAMIC_HIST_MODE_RGB_COMBINED  = 1,    /**< RGB combined histogram */
    CAMIC_HIST_MODE_R             = 2,    /**< R histogram */
    CAMIC_HIST_MODE_G             = 3,    /**< G histogram */
    CAMIC_HIST_MODE_B             = 4,    /**< B histogram */
    CAMIC_HIST_MODE_Y             = 5,    /**< luminance histogram */
    CAMIC_HIST_MODE_MAX,                                  /**< upper border (only for an internal evaluation) */
} CamIcHistStatsMode_t;

/*****************************************************************************/
/**
 *          CamerIcRawSwapMode_t
 *
 * @brief   mode type of RawSwap
 * 3frame:
 *             L-BIG   M-BIG     S-LITE
 *             L-BIG   M-LITE   S-BIG
 *             L-LITE  M-BIG    S-BIG
 * for Hdr 3frame all 3 modes is effective
 * for Hdr 2frame all 3 modes is effective
 * for Linear mode2 is the same as mode3, mode 1=LITE & mode 2/3=BIG
 */
/*****************************************************************************/
typedef enum CamerIcRawSwapMode_e {
    AEC_RAWSWAP_MODE_S_LITE    = 0,
    AEC_RAWSWAP_MODE_M_LITE    = 1,
    AEC_RAWSWAP_MODE_L_LITE    = 2,
    AEC_RAWSWAP_MODE_MAX,
} CamerIcRawSwapMode_t;

/*****************************************************************************/
/**
 *          CamerIcRawAe3SelMode_t
 *
 * @brief   data sel type of RAWAE3
 *
 *   bit: 0x00   chn[0]
 *         0x01   chn[1]
 *         0x02   chn[2]
 *         0x03   tmo
 *
 */
/*****************************************************************************/
typedef enum CamerIcRawAe3SelMode_e {
    AEC_RAWSEL_MODE_CHN_0    = 0,
    AEC_RAWSEL_MODE_CHN_1    = 1,
    AEC_RAWSEL_MODE_CHN_2    = 2,
    AEC_RAWSEL_MODE_TMO      = 3,
} CamerIcRawAe3SelMode_t;

/*****************************************************************************/
/**
 *          AIrisFNO_e
 *
 * @brief   enum Auto Iris FNO
 *          real Equivalent Iris Gain = 1 << AIRIS_F_NO_XX_X
 */
/*****************************************************************************/
typedef enum {
    AIRIS_F_NO_32_0 = 0,
    AIRIS_F_NO_22_0,
    AIRIS_F_NO_16_0,
    AIRIS_F_NO_11_0,
    AIRIS_F_NO_8_0,
    AIRIS_F_NO_5_6,
    AIRIS_F_NO_4_0,
    AIRIS_F_NO_2_8,
    AIRIS_F_NO_2_0,
    AIRIS_F_NO_1_4,
    AIRIS_F_NO_1_0
} AIrisFNO_t;
/*****************************************************************************/
/**
 *          AecDampingMode_t
 *
 * @brief   mode type of AEC Damping
 *
 */
/*****************************************************************************/
typedef enum {
    AEC_DAMPING_MODE_INVALID        = 0,        /* invalid (only used for initialization) */
    AEC_DAMPING_MODE_STILL_IMAGE    = 1,        /* damping mode still image */
    AEC_DAMPING_MODE_VIDEO          = 2,        /* damping mode video */
    AEC_DAMPING_MODE_MAX
} AecDampingMode_t;
/*****************************************************************************/
/**
 * @brief   mode type of Hdr-AEC
 *
 ****************************************************************************/
typedef enum {
    AEC_WORKING_MODE_NORMAL,
    AEC_WORKING_MODE_ISP_HDR2    = 0x10,
    AEC_WORKING_MODE_ISP_HDR3    = 0x20,
} AecWorkingMode_t;

typedef enum {
    ISP_HDR_MODE_2_FRAME_HDR = AEC_WORKING_MODE_ISP_HDR2 + 1,
    ISP_HDR_MODE_2_LINE_HDR = AEC_WORKING_MODE_ISP_HDR2 + 2,
    ISP_HDR_MODE_3_FRAME_HDR = AEC_WORKING_MODE_ISP_HDR3 + 1,
    ISP_HDR_MODE_3_LINE_HDR = AEC_WORKING_MODE_ISP_HDR3 + 2,
} IspHdrMode_t;

typedef enum {
    AEC_LINEHDR_MODE_DCG     = 0,       /* HDR DCG MODE,2frame (only one integration time value),3frame=DCG+VS */
    AEC_LINEHDR_MODE_STAGGER = 1,       /* HDR STAGGER MODE,2/3frame (independent integration time value) */
} AecLineHdrMode_t;

typedef enum {
    AEC_DCG_MODE_INVALID     = -1,       /*invalid, not support dcg configuration*/
    AEC_DCG_MODE_LCG         = 0,       /* LCG, use lower conversion gain*/
    AEC_DCG_MODE_HCG         = 1,       /* HCG, use higher conversion gain*/
} AecDcgMode_t;

typedef enum {
    LOW_TO_HIGH   = 0,        /* gain > up_thres: LSNR to HSNR*/
    HIGH_TO_LOW   = 1,       /* gain > up_thres: HSNR to LSNR*/
} NrSwitchMode_t;

typedef enum {
    NR_FEATURE_LEVEL_LOW    = 0,        /* Low level, LSNR*/
    NR_FEATURE_LEVEL_HIGH   = 1,       /* High level, HSNR*/
} AecNRFeatureLevel_t;

/*****************************************************************************/
/**
 *          AecFrameMode_t
 * @brief   Frame Mode for RawAe
 *
 * For LinearAe CH0=Normal Frame, CH1/2 not used
 * For HdrAe 2frame CH0=S Frame, CH1=L Frame, CH2 not used
 * For HdrAe 3frame CH0=S Frame, CH1=M Frame, CH2=L Frame
 */
/*****************************************************************************/
typedef enum {
    AEC_HDR_3FRAME_S = 0,
    AEC_HDR_3FRAME_M = 1,
    AEC_HDR_3FRAME_L = 2,

    AEC_HDR_2FRAME_S = 0,
    AEC_HDR_2FRAME_L = 1,

    AEC_EXTRA_FRAME = 3,
    AEC_NORMAL_FRAME = 0
} AecFrameMode_t;

/*****************************************************************************/
/**
 *          AecProcStrategyMode_t
 *
 * @brief   flicker period types for the AEC algorithm
 *
 */
/*****************************************************************************/
typedef enum {
    LOWLIGHT_DETECT = 0,
    HIGHLIGHT_DETECT = 1
} AecProcStrategyMode_t;

/*****************************************************************************/
/**
 *          AecEcmFlickerPeriod_t
 *
 * @brief   flicker period types for the AEC algorithm
 *
 */
/*****************************************************************************/
typedef enum {
    AEC_EXPOSURE_CONVERSION_FLICKER_OFF   = 0x00,
    AEC_EXPOSURE_CONVERSION_FLICKER_100HZ = 0x01,
    AEC_EXPOSURE_CONVERSION_FLICKER_120HZ = 0x02
} AecEcmFlickerPeriod_t;
/*****************************************************************************/
/**
 * @brief   mode type of Day or Night Switch
 *
 ****************************************************************************/
typedef enum
{
    TRIGGER_OFF = 0, //no trigger DayNight Switch, use fixed DNMode!
    HARDWARE_TRIGGER = 1, //have photosensitive element to trigger night mode  (hardware Trigger)
    SOFTWARE_TRIGGER = 2, //use software params to trigger night mode  (software Trigger)
} AecNightTriggerMode_t;

typedef enum
{
    IR_BW = 0, //use IR LED as supplement light, black&white night-vision mode (software)
    VB_RGB = 1, //use (visible light/no light) as supplement light, rgb night mode (software)
} AecNightMode_t;

typedef struct Aec_daynight_th_s {
    float         fac_th;
    uint8_t       holdon_times_th;
} Aec_daynight_th_t;

/*****************************************************************************/
/**
 *          Aec_stat_t
 *
 * @brief   AEC Module Hardware statistics structure
 *
 *****************************************************************************/

typedef RkAiqAecHwStatsRes_t AecStat_t;
typedef RKAiqAecExpInfo_t ExpInfo_t;

/*****************************************************************************/
/**
 *          AecConfig_t
 *
 * @brief   AEC Module configuration structure isp2; used for re-configuration as well
 *
 *****************************************************************************/
typedef struct Aec_Sensor_nr_switch_s {
    bool valid;
    uint32_t direct;
    uint32_t up_thres;
    uint32_t down_thres;
    uint32_t div_coeff;
} Aec_Sensor_nr_switch_t;

typedef struct Aec_AeRange_s {
    float                   Min;
    float                   Max;
} Aec_AeRange_t;

typedef struct Aec_LinAeRange_s {
    Aec_AeRange_t      stExpTimeRange;
    Aec_AeRange_t      stGainRange;
    Aec_AeRange_t      stIspDGainRange;
    Aec_AeRange_t      stPIrisRange;
} Aec_LinAeRange_t;

typedef struct Aec_HdrAeRange_s {
    Aec_AeRange_t      stExpTimeRange[3];
    Aec_AeRange_t      stGainRange[3];
    Aec_AeRange_t      stIspDGainRange[3];
    Aec_AeRange_t      stPIrisRange;
} Aec_HdrAeRange_t;

typedef struct Aec_uapi_advanced_attr_s {
    bool                          enable;
    uint8_t                       GridWeights[15 * 15];
    uint8_t                       DayGridWeights[RAWAEBIG_WIN_NUM];
    uint8_t                       NightGridWeights[RAWAEBIG_WIN_NUM];
    bool                          SetAeRangeEn;
    Aec_LinAeRange_t              SetLinAeRange;
    Aec_HdrAeRange_t              SetHdrAeRange;
} Aec_uapi_advanced_attr_t;

typedef enum AecHwVersion_e
{
    AEC_HARDWARE_V0 = 0,  //at most support Hdr 3_frame, 2 AEBIG & 1 AELITE for input raw, support yuv luma e.g. rk1126/1109
    AEC_HARDWARE_V1 = 1,  //at most support Hdr 2_frame, 1 AEBIG & 1 AELITE for input raw, not support yuv luma  e.g. rk356X
    AEC_HARDWARE_V2 = 2,  //at most support Hdr 3_frame, 2 AEBIG & 1 AELITE for input raw, not support yuv luma, share AEBIG3 with AF e.g. rk3588
    AEC_HARDWARE_V3 = 3,  //at most support Hdr 2_frame, 1 AEBIG & 1 AELITE for input raw, not support yuv luma, share AEBIG3 with AF e.g. rk1106
    AEC_HARDWARE_MAX,
} AecHwVersion_t;

typedef struct AecConfig_s {

    /*Aec Ctrl Configuration from calibdb, support User Api input Ctrl configuration*/

    //V2: json params
    CalibDb_AecCommon_AttrV2_t    CommCtrlV2;
    CalibDb_LinearAE_AttrV2_t     LinearAeCtrlV2;
    CalibDb_HdrAE_AttrV2_t        HdrAeCtrlV2;
    CalibDb_AecIrisCtrlV2_t       IrisCtrl;
    CalibDb_AeSyncTestV2_t        SyncTest; //special module for debug
    CalibDb_Sensor_ParaV2_t       SensorInfoV2;
    CalibDb_Module_ParaV2_t       ModuleInfoV2;


    RkAiqAecHwConfig_t            HwCtrl;
    AecHwVersion_t                AecHwVers;

    int                           Workingmode;
    int                           LineHdrMode;

    bool                          AfStatsPrior;

    /*params related to driver setting*/
    float                         LinePeriodsPerField;
    float                         PixelClockFreqMHZ;
    float                         PixelPeriodsPerLine;
    Aec_Sensor_nr_switch_t        nr_switch;

    /*continue to use some old params to keep the same with AecConfig_t*/
    AecDampingMode_t              DampingMode;              /**< damping mode */

    int                           RawWidth;
    int                           RawHeight;

    /*add for api using advanced params*/
    Aec_uapi_advanced_attr_t      ApiAdvanced;
    Aec_LinAeRange_t              LinAeRange;
    Aec_HdrAeRange_t              HdrAeRange;

    /*update attr flag*/
    uint16_t                      IsReconfig;
} AecConfig_t;
/*****************************************************************************/
/**
 *          AeInstanceConfig_t
 *
 * @brief   AF Module instance configuration structure
 *
 *****************************************************************************/
typedef struct AecContext_s* AeHandle_t;     /**< handle to AEC context */
typedef struct AecConfig_s* AeConfig_t;     /**< handle to AEC config */

typedef struct AeInstanceConfig_s {
    AeHandle_t              hAe;            /**< handle returned by AeInit() */
    AeConfig_t              aecCfg;
    bool                    lockaebyaf;
} AeInstanceConfig_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC Algo Result Params
 */
/*****************************************************************************/
typedef struct AecPreResult_s {
    float LowLightROIPdf[MAX_HDR_FRAMENUM];
    float HighLightROIPdf[MAX_HDR_FRAMENUM];
    float HighLightLuma[MAX_HDR_FRAMENUM];
    float OverExpROIPdf[MAX_HDR_FRAMENUM];
    float GlobalEnvLv[MAX_HDR_FRAMENUM];
    float MeanLuma[MAX_HDR_FRAMENUM];
    float DynamicRange;
    RkAiqExpParamComb_t LinearExp;
    RkAiqExpParamComb_t HdrExp[MAX_HDR_FRAMENUM];
    CamerIcHistBins_t AeRawHistBin[3];
} AecPreResult_t;

/*****************************************************************************/
/**
*     AecProcResult_t
* @brief   Aec_Result.
*
*/
/*****************************************************************************/
typedef struct AecProcResult_s {
    bool                          IsConverged;
    bool                          LongFrmMode;
    float                         LumaDeviation;
    float                         HdrLumaDeviation[MAX_HDR_FRAMENUM];
    int                           exp_set_cnt;
    RKAiqAecExpInfo_t             exp_set_tbl[MAX_AEC_EFFECT_FNUM + 1];
} AecProcResult_t;

typedef struct AecPostResult_s {
    RkAiqDCIrisParam_t      DCIris;

} AecPostResult_t;

/* @} AEC */

#endif /* __RK_AIQ_TYPES_AE_ALGO_INT_H__*/
