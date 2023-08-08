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

typedef enum merge_OpMode_e {
    MERGE_OPMODE_AUTO   = 0,  // run auto merge
    MERGE_OPMODE_MANUAL = 1,  // run manual merge
} merge_OpMode_t;

typedef uapiMergeCurrCtlData_t MergeCurrCtlData_t;

// merge V10
typedef struct mergeAttrV10_s {
    rk_aiq_uapi_sync_t sync;

    merge_OpMode_t opMode;
    mMergeAttrV10_t stManual;
    CalibDbV2_merge_v10_t stAuto;
    MergeCurrCtlData_t Info;
} mergeAttrV10_t;

// merge V11
typedef struct mergeAttrV11_s {
    rk_aiq_uapi_sync_t sync;

    merge_OpMode_t opMode;
    mMergeAttrV11_t stManual;
    CalibDbV2_merge_v11_t stAuto;
    MergeCurrCtlData_t Info;
} mergeAttrV11_t;

// merge V12
typedef struct mergeAttrV12_s {
    rk_aiq_uapi_sync_t sync;

    merge_OpMode_t opMode;
    mMergeAttrV12_t stManual;
    CalibDbV2_merge_v12_t stAuto;
    MergeCurrCtlData_t Info;
} mergeAttrV12_t;

#endif
