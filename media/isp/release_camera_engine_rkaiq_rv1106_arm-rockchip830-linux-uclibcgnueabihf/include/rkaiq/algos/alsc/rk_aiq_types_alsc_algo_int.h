/*
 *rk_aiq_types_alsc_algo_int.h
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

#ifndef _RK_AIQ_TYPE_ALSC_ALGO_INT_H_
#define _RK_AIQ_TYPE_ALSC_ALGO_INT_H_
#include "alsc/rk_aiq_types_alsc_algo.h"
#include "alsc_uapi_head.h"
#include "rk_aiq_types_alsc_common.h"


RKAIQ_BEGIN_DECLARE

/** @brief LSC will use these data from other module*/
typedef struct {
    uint32_t flag;

    uint16_t width;
    uint16_t height;
    uint16_t table_size;

    uint16_t *lsc_r;
    uint16_t *lsc_b;
    uint16_t *lsc_gr;
    uint16_t *lsc_gb;
} AlscOtpInfo_t;

typedef struct alsc_sw_info_s {
    float sensorGain;
    float awbGain[2];
    float awbIIRDampCoef;
    float varianceLuma;
    bool grayMode;
    bool awbConverged;
    int prepare_type;
    RkAiqBayerPattern_t bayerPattern;
    AlscOtpInfo_t otpInfo;
} alsc_sw_info_t;

/**
 * @brief Remember, 4 channel data array must be in the head of struct,
 * and with the order of (r, gr, gb, b)!
*/
typedef struct rk_aiq_lsc_table_s rk_aiq_lsc_mlsc_attrib_t;

typedef enum rk_aiq_lsc_op_mode_s {
    RK_AIQ_LSC_MODE_INVALID                     = 0,        /**< initialization value */
    RK_AIQ_LSC_MODE_MANUAL                      = 1,        /**< run manual lens shading correction */
    RK_AIQ_LSC_MODE_AUTO                        = 2,        /**< run auto lens shading correction */
    RK_AIQ_LSC_MODE_MAX
} rk_aiq_lsc_op_mode_t;

typedef struct Lsc_v2_Resolution_s {
    char        name[LSC_NAME];
    uint16_t    lsc_sect_size_x[16];
    uint16_t    lsc_sect_size_y[16];
} Lsc_v2_Resolution_t;

typedef struct CalibDbV2_Lsc_Common_fixed_s {
    bool enable;
    Lsc_v2_Resolution_t resolutionAll[LSC_MAX_RESOLUTION];
    int resolutionAll_len;
} CalibDbV2_Lsc_Common_fixed_t;

typedef struct CalibDbV2_AlscCof_ill_fixed_s {
    uint32_t    usedForCase;
    char        name[LSC_NAME];
    float       wbGain[2];
    lsc_name_t tableUsed[LSC_MAX_ILLUMINANT*LSC_MAX_RESOLUTION];
    int         tableUsed_len;
    float      gains[100];
    int         gains_len;
    float      vig[100];
    int         vig_len;
} CalibDbV2_AlscCof_ill_fixed_t;

typedef struct CalibDbV2_AlscCof_fixed_s {
    bool damp_enable;
    CalibDbV2_AlscCof_ill_fixed_t illAll[LSC_MAX_ILLUMINANT];
    int illAll_len;
} CalibDbV2_AlscCof_fixed_t;

typedef struct CalibDbV2_LscTable_fixed_s {
    CalibDbV2_LscTableProfile_t tableAll[LSC_MAX_ILLUMINANT*LSC_MAX_RESOLUTION];
    int tableAll_len;
} CalibDbV2_LscTable_fixed_t;

typedef struct rk_aiq_lsc_alsc_attrib_s {
   CalibDbV2_Lsc_Common_fixed_t common;
   CalibDbV2_AlscCof_fixed_t alscCoef;
   CalibDbV2_LscTable_fixed_t tbl;
} rk_aiq_lsc_alsc_attrib_t;

/** @brief struct for setting-api, should careful consider when want to modify it*/
typedef struct rk_aiq_lsc_attrib_s {
    bool byPass;
    rk_aiq_lsc_op_mode_t mode;
    rk_aiq_lsc_mlsc_attrib_t stManual;
    rk_aiq_lsc_alsc_attrib_t stAuto;
    rk_aiq_uapi_sync_t sync;
} rk_aiq_lsc_attrib_t;

/** @brief struct for setting-api, should careful consider when want to modify it*/
typedef struct rk_aiq_lsc_querry_info_s {
    bool lsc_en;
    unsigned short r_data_tbl[LSC_DATA_TBL_SIZE];
    unsigned short gr_data_tbl[LSC_DATA_TBL_SIZE];
    unsigned short gb_data_tbl[LSC_DATA_TBL_SIZE];
    unsigned short b_data_tbl[LSC_DATA_TBL_SIZE];
} rk_aiq_lsc_querry_info_t;

RKAIQ_END_DECLARE

#endif

