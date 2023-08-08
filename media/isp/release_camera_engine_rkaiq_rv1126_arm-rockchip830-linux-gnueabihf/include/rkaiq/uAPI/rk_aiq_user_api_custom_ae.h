/*
 *  Copyright (c) 2021 Rockchip Corporation
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

#ifndef _RK_AIQ_USER_API_CUSTOM_AE_H_
#define _RK_AIQ_USER_API_CUSTOM_AE_H_

#include "rk_aiq_types.h" /* common structs */
#include "rk_aiq_user_api_sysctl.h" /* rk_aiq_sys_ctx_t */
#include "ae/rk_aiq_types_ae_hw.h" /* include ae structs*/

RKAIQ_BEGIN_DECLARE

/* ae stats */
typedef struct rk_aiq_customAe_stat_s
{
    struct rawaebig_stat rawae_big;
    struct rawhist_stat rawhist_big;
} rk_aiq_customAe_stat_t;

/* all ae stats */
typedef struct rk_aiq_customAe_stats_s
{
    rk_aiq_customAe_stat_t rawae_stat; // before awb gain
    rk_aiq_customAe_stat_t extra;      // after awb gain, lsc, TMO
    RkAiqExpRealParam_t exp_real_params; //the exposure sync with stats
} rk_aiq_customAe_stats_t;

/* ae results */
typedef struct rk_aiq_customeAe_results_s
{
    bool  IsConverged; //true: converged; false: not converged
    float LumaDeviation;// (meanluma-target)/target, >0 : too bright, <0 : too dark, =0: meet the target
    float MeanLuma;//weighted meanluma, range:0~255
    RkAiqExpRealParam_t exp_real_params; //real value, only integration_time/analog_gain/dcg_mode needed right now
    struct window meas_win;
    unsigned char meas_weight[15 * 15];
} rk_aiq_customeAe_results_t;

typedef struct rk_aiq_customeAe_cbs_s
{
    /* ctx is the rk_aiq_sys_ctx_t which is corresponded to
     * camera, could be mapped to camera id.
     */
    int32_t (*pfn_ae_init)(void* ctx);
    int32_t (*pfn_ae_run)(void* ctx, const rk_aiq_customAe_stats_t* pstAeInfo,
                          rk_aiq_customeAe_results_t* pstAeResult);
    /* not used now */
    int32_t (*pfn_ae_ctrl)(void* ctx, uint32_t u32Cmd, void *pValue);
    int32_t (*pfn_ae_exit)(void* ctx);
} rk_aiq_customeAe_cbs_t;

/*!
 * \brief register custom Ae algo
 *
 * \param[in] ctx             context
 * \param[in] cbs             custom Ae callbacks
 * \note should be called after rk_aiq_uapi_sysctl_init
 */
XCamReturn
rk_aiq_uapi_customAE_register(const rk_aiq_sys_ctx_t* ctx, rk_aiq_customeAe_cbs_t* cbs);

/*!
 * \brief enable/disable custom Ae algo
 *
 * \param[in] ctx             context
 * \param[in] enable          enable/diable custom Ae
 * \note should be called after rk_aiq_uapi_customAE_register. If custom Ae was enabled,
 *       Rk ae will be stopped, vice versa.
 */
XCamReturn
rk_aiq_uapi_customAE_enable(const rk_aiq_sys_ctx_t* ctx, bool enable);

/*!
 * \brief unregister custom Ae algo
 *
 * \param[in] ctx             context
 * \note should be called after rk_aiq_uapi_customAE_register.
 */
XCamReturn
rk_aiq_uapi_customAE_unRegister(const rk_aiq_sys_ctx_t* ctx);

RKAIQ_END_DECLARE

#endif
