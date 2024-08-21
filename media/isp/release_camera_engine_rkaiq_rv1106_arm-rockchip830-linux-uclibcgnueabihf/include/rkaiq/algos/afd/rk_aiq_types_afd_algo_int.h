/*
* rk_aiq_types_afd_algo_int.h

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

#ifndef __RK_AIQ_TYPES_AFD_ALGO_INT_H__
#define __RK_AIQ_TYPES_AFD_ALGO_INT_H__

/**
 * @file rk_aiq_types_afd_algo_int.h
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
 * @defgroup AFD Auto flicker detection Module
 * @{
 *
 */
#include <list>
#include "RkAiqCalibDbV2Helper.h"
#include "RkAiqCalibDbV2.h"
#include "rk_aiq_types_afd_algo.h"
#include "ae/rk_aiq_types_ae_algo_int.h"


/*****************************************************************************/
/**
 * @brief   log level of afd
 */
/*****************************************************************************/
typedef struct AfdConfig_s {

    int                           Workingmode;

    /*params related to driver setting*/
    float                         LinePeriodsPerField;
    float                         PixelClockFreqMHZ;
    float                         PixelPeriodsPerLine;

    int                           RawWidth;
    int                           RawHeight;

    int                           ScaleH;
    int                           ScaleW;

    bool                          enable;

} AfdConfig_t;

/*****************************************************************************/
/**
 *          AfdInstanceConfig_t
 *
 * @brief   AFD Module instance configuration structure
 *
 *****************************************************************************/
typedef struct AfdContext_s* AfdHandle_t;    /**< handle to AEC context */

#define THUMBDATA_MAXNUM (4)
typedef struct ThumbData_s {
    uint8_t* baseaddr;
    uint32_t  frameid;
    RKAiqAecExpInfo_t expinfo;
} ThumbData_t;

typedef struct AfdInstanceConfig_s {
    AfdHandle_t              hAfd;            /**< handle returned by AfdInit() */
    AfdConfig_t*             afdCfg;
    std::list<ThumbData_t*> _thumbQueue;
} AfdInstanceConfig_t;

/*****************************************************************************/
/**
 * @brief   ISP2.0 AFD Algo Result Params
 */
/*****************************************************************************/
typedef struct AfdPreResult_s {

} AfdPreResult_t;

/*****************************************************************************/
/**
*     AfdcProcResult_t
* @brief   Afd_Result.
*
*/
/*****************************************************************************/
typedef struct AfdProcResult_s {
    bool                    Enable;
    AfdPeakRes_t            PeakRes;
} AfdProcResult_t;

typedef struct AfdPostResult_s {

} AfdPostResult_t;

/* @} AFD */

#endif /* __RK_AIQ_TYPES_AFD_ALGO_INT_H__*/
