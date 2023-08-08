#ifndef _RK_AIQ_UAPI_AE_INT_TYPES_V1_H_
#define _RK_AIQ_UAPI_AE_INT_TYPES_V1_H_
#include "rk_aiq_algo_des.h"
#include "rk_aiq_types_ae_algo_int.h"
#include "RkAiqCalibDbTypes.h"

/***************************  ATTENTION:  ************************************/
/*
 *      There are two sets of api-strcut defined, isp2.0/isp2.1 respectively.
 *      AE-API Needs to be compatible with isp2.0 & isp2.1 struct
 */
/*****************************************************************************/

/*****************************************************************************/
/**
 * @brief   ISP2.0 AEC API ExpSwAttr_t Params & LinExpAttr_t & HdrExpAttr_t
 */
/*****************************************************************************/

typedef CalibDb_AeSpeed_t Uapi_AeSpeed_t;

typedef CalibDb_AeRange_t Uapi_AeRange_t;

typedef CalibDb_LinAeRange_t Uapi_LinAeRange_t;

typedef CalibDb_HdrAeRange_t Uapi_HdrAeRange_t;

typedef CalibDb_AeFrmRateAttr_t Uapi_AeFpsAttr_t;

typedef CalibDb_LinExpInitExp_t Uapi_LinExpInitExp_t;

typedef CalibDb_HdrExpInitExp_t Uapi_HdrExpInitExp_t;

typedef CalibDb_ExpInitExp_t Uapi_ExpInitExp_t;

typedef CalibDb_AntiFlickerAttr_t Uapi_AntiFlicker_t;

typedef CalibDb_AeAttr_t Uapi_AeAttr_t;

typedef CalibDb_AecIrisCtrl_t Uapi_IrisAttr_t;

typedef CalibDb_LinMeAttr_t Uapi_LinMeAttr_t;

typedef CalibDb_HdrMeAttr_t Uapi_HdrMeAttr_t;

typedef CalibDb_MeAttr_t Uapi_MeAttr_t;

typedef Aec_uapi_advanced_attr_t Uapi_ExpSwAttr_Advanced_t;

typedef struct Uapi_ExpSwAttr_s {
    uint8_t                          enable;
    CalibDb_CamRawStatsMode_t        RawStatsMode;
    CalibDb_CamHistStatsMode_t       HistStatsMode;
    CalibDb_CamYRangeMode_t          YRangeMode;
    uint8_t                  AecRunInterval;
    RKAiqOPMode_t            AecOpType;
    //GridWeight
    Cam15x15UCharMatrix_t      DayGridWeights;
    Cam15x15UCharMatrix_t      NightGridWeights;
    int                        DayWeightNum;
    int                        NightWeightNum;

    //DayOrNight Switch
    uint8_t                  DNTrigger;
    CalibDb_AecDayNightMode_t   DNMode;
    uint8_t                  FillLightMode;

    Uapi_IrisAttr_t          stIris;
    Uapi_AntiFlicker_t       stAntiFlicker;
    Uapi_AeAttr_t            stAuto;
    Uapi_MeAttr_t            stManual;
    Uapi_ExpInitExp_t        stInitExp;

    Uapi_ExpSwAttr_Advanced_t stAdvanced;
} Uapi_ExpSwAttr_t;

typedef CalibDb_LinearAE_Attr_t Uapi_LinExpAttr_t;

typedef CalibDb_HdrAE_Attr_t Uapi_HdrExpAttr_t;

#endif
