/*
* rk_aiq_types_afd_algo.h

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

#ifndef __RK_AIQ_TYPES_AFD_ALGO_H__
#define __RK_AIQ_TYPES_AFD_ALGO_H__

/**
 * @file rk_aiq_types_afd_algo.h
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

#include "rk_aiq_comm.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

RKAIQ_BEGIN_DECLARE

typedef struct rk_aiq_afd_cfg_s {
    bool enable;
    int  ratio;
} rk_aiq_afd_cfg_t;

RKAIQ_END_DECLARE

/* @} AFD */
#endif /*__RK_AIQ_TYPES_AFD_ALGO_H__*/
