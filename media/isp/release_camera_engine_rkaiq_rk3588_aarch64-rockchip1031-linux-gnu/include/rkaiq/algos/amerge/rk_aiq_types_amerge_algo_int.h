/******************************************************************************
 *
 * Copyright 2019, Fuzhou Rockchip Electronics Co.Ltd . All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Fuzhou Rockchip Electronics Co.Ltd .
 *
 *
 *****************************************************************************/
#ifndef __RK_AIQ_TYPES_AMERGE_ALGO_INT_H__
#define __RK_AIQ_TYPES_AMERGE_ALGO_INT_H__

#include "RkAiqCalibDbTypes.h"
#include "amerge_head.h"
#include "amerge_uapi_head.h"


#define ISP20_HDRMGE_OE_CURVE_NUM   (17)
#define ISP20_HDRMGE_MD_CURVE_NUM   (17)
#define ISP3X_HDRMGE_OE_CURVE_NUM   ISP20_HDRMGE_OE_CURVE_NUM
#define ISP3X_HDRMGE_MD_CURVE_NUM   ISP20_HDRMGE_MD_CURVE_NUM

typedef enum merge_OpModeV21_e {
    MERGE_OPMODE_API_OFF = 0, // run IQ ahdr
    MERGE_OPMODE_MANU = 1, //run api manual ahdr
} merge_OpModeV21_t;

typedef uapiMergeCurrCtlData_t MergeCurrCtlData_t;

typedef struct mergeAttrV21_s
{
    merge_OpModeV21_t    opMode;
    mmergeAttrV21_t      stManual;
    MergeCurrCtlData_t   CtlInfo;
} mergeAttrV21_t;

typedef struct mergeAttrV30_s
{
    merge_OpModeV21_t    opMode;
    mMergeAttrV30_t stManual;
    MergeCurrCtlData_t CtlInfo;
} mergeAttrV30_t;

typedef struct mergeAttr_s {
    rk_aiq_uapi_sync_t sync;

    mergeAttrV21_t    attrV21;
    mergeAttrV30_t    attrV30;
} mergeAttr_t;

typedef struct MgeProcRes_s
{
    unsigned char  sw_hdrmge_mode;
    unsigned short sw_hdrmge_gain0_inv;
    unsigned short sw_hdrmge_gain0;
    unsigned short sw_hdrmge_gain1_inv;
    unsigned short sw_hdrmge_gain1;
    unsigned char  sw_hdrmge_gain2;
    unsigned char  sw_hdrmge_ms_dif_0p8;
    unsigned char  sw_hdrmge_lm_dif_0p9;
    unsigned char  sw_hdrmge_ms_dif_0p15;
    unsigned char  sw_hdrmge_lm_dif_0p15;
    unsigned short sw_hdrmge_l0_y[ISP20_HDRMGE_MD_CURVE_NUM];
    unsigned short sw_hdrmge_l1_y[ISP20_HDRMGE_MD_CURVE_NUM];
    unsigned short sw_hdrmge_e_y[ISP20_HDRMGE_OE_CURVE_NUM];
} MgeProcRes_t;

typedef struct MgeProcResV2_s
{
    unsigned char  sw_hdrmge_s_base;
    unsigned char  sw_hdrmge_mode;
    unsigned short sw_hdrmge_gain0_inv;
    unsigned short sw_hdrmge_gain0;
    unsigned short sw_hdrmge_gain1_inv;
    unsigned short sw_hdrmge_gain1;
    unsigned char  sw_hdrmge_gain2;
    unsigned char  sw_hdrmge_ms_dif_0p8;
    unsigned char  sw_hdrmge_lm_dif_0p9;
    unsigned char  sw_hdrmge_ms_dif_0p15;
    unsigned char  sw_hdrmge_lm_dif_0p15;
    unsigned short sw_hdrmge_l0_y[ISP3X_HDRMGE_MD_CURVE_NUM];
    unsigned short sw_hdrmge_l1_y[ISP3X_HDRMGE_MD_CURVE_NUM];
    unsigned short sw_hdrmge_e_y[ISP3X_HDRMGE_OE_CURVE_NUM];
    unsigned short sw_hdrmge_ms_thd1;
    unsigned short sw_hdrmge_ms_thd0;
    unsigned short sw_hdrmge_ms_scl;
    unsigned short sw_hdrmge_lm_thd1;
    unsigned short sw_hdrmge_lm_thd0;
    unsigned short sw_hdrmge_lm_scl;
} MgeProcResV2_t;

typedef struct RkAiqAmergeProcResult_s
{
    union {
        MgeProcRes_t Merge_v20;
        MgeProcResV2_t Merge_v30;
    };
    bool update;;
    bool LongFrameMode;
} RkAiqAmergeProcResult_t;

#endif
