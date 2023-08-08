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
#ifndef __RK_AIQ_TYPES_ADRC_ALGO_INT_H__
#define __RK_AIQ_TYPES_ADRC_ALGO_INT_H__

#include "RkAiqCalibDbTypes.h"
#include "adrc_head.h"
#include "adrc_uapi_head.h"

typedef enum drc_OpMode_s {
    DRC_OPMODE_AUTO   = 0,  // run auto drc
    DRC_OPMODE_MANUAL = 1,  // run manual drc
} drc_OpMode_t;

// drc attr V10
typedef struct drcAttrV10_s {
    rk_aiq_uapi_sync_t sync;

    drc_OpMode_t opMode;
    CalibDbV2_drc_V10_t stAuto;
    mdrcAttr_V10_t stManual;
    DrcInfoV10_t Info;
} drcAttrV10_t;

// drc attr V11

typedef struct drcAttrV11_s {
    rk_aiq_uapi_sync_t sync;

    drc_OpMode_t opMode;
    CalibDbV2_drc_V11_t stAuto;
    mdrcAttr_V11_t stManual;
    DrcInfoV11_t Info;
} drcAttrV11_t;

// drc attr V12
typedef struct drcAttrV12_s {
    rk_aiq_uapi_sync_t sync;

    drc_OpMode_t opMode;
    CalibDbV2_drc_V12_t stAuto;
    mdrcAttr_V12_t stManual;
    DrcInfoV12_t Info;
} drcAttrV12_t;

#endif
