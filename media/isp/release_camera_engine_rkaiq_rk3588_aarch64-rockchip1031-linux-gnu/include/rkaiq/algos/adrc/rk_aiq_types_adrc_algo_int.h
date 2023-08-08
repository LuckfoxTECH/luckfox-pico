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
#include "rk_aiq_types_adrc_stat_v200.h"

#define AIQ_ISP3X_DRC_Y_NUM 17


typedef struct DrcProcResV21_s
{
    int sw_drc_offset_pow2;
    int sw_drc_compres_scl;
    int sw_drc_position;
    int sw_drc_delta_scalein;
    int sw_drc_hpdetail_ratio;
    int sw_drc_lpdetail_ratio;
    int sw_drc_weicur_pix;
    int sw_drc_weipre_frame;
    int sw_drc_force_sgm_inv0;
    int sw_drc_motion_scl;
    int sw_drc_edge_scl;
    int sw_drc_space_sgm_inv1;
    int sw_drc_space_sgm_inv0;
    int sw_drc_range_sgm_inv1;
    int sw_drc_range_sgm_inv0;
    int sw_drc_weig_maxl;
    int sw_drc_weig_bilat;
    int sw_drc_gain_y[ISP21_DRC_Y_NUM];
    int sw_drc_compres_y[ISP21_DRC_Y_NUM];
    int sw_drc_scale_y[ISP21_DRC_Y_NUM];
    int sw_drc_iir_weight;
    int sw_drc_min_ogain;
} DrcProcResV21_t;

typedef struct DrcProcResV30_s {
    int bypass_en;
    int offset_pow2;
    int compres_scl;
    int position;
    int delta_scalein;
    int hpdetail_ratio;
    int lpdetail_ratio;
    int weicur_pix;
    int weipre_frame;
    int bilat_wt_off;
    int force_sgm_inv0;
    int motion_scl;
    int edge_scl;
    int space_sgm_inv1;
    int space_sgm_inv0;
    int range_sgm_inv1;
    int range_sgm_inv0;
    int weig_maxl;
    int weig_bilat;
    int enable_soft_thd;
    int bilat_soft_thd;
    int gain_y[AIQ_ISP3X_DRC_Y_NUM];
    int compres_y[AIQ_ISP3X_DRC_Y_NUM];
    int scale_y[AIQ_ISP3X_DRC_Y_NUM];
    int wr_cycle;
    int iir_weight;
    int min_ogain;
} DrcProcResV30_t;

typedef struct DrcProcRes_s {
    union {
        DrcProcResV21_t Drc_v21;
        DrcProcResV30_t Drc_v30;
    };
} DrcProcRes_t;

typedef struct RkAiqAdrcProcResult_s
{
    bool update;
    bool bDrcEn;
    DrcProcRes_t DrcProcRes;
} RkAiqAdrcProcResult_t;


typedef enum AdrcVersion_e {
    ADRC_VERSION_356X = 0,
    ADRC_VERSION_3588 = 1,
    ADRC_VERSION_MAX
} AdrcVersion_t;

typedef enum drc_OpMode_s {
    DRC_OPMODE_AUTO = 0,  // run auto
    DRC_OPMODE_MANU = 1,  // run manual
} drc_OpMode_t;

typedef struct drcAttr_s {
    rk_aiq_uapi_sync_t sync;

    AdrcVersion_t Version;
    drc_OpMode_t opMode;
    adrcAttr_V21_t stAutoV21;
    adrcAttr_V30_t stAutoV30;
    mdrcAttr_V21_t stManualV21;
    mdrcAttr_V30_t stManualV30;
    DrcInfo_t Info;
} drcAttr_t;


#endif
