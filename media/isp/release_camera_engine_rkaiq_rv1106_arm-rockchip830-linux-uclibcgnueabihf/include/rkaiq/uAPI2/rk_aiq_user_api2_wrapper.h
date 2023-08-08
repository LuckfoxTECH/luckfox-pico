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

#ifndef __RK_AIQ_USER_API2_WRAPPER_H__
#define __RK_AIQ_USER_API2_WRAPPER_H__

#include "rk_aiq_user_api_sysctl.h"

int rk_aiq_uapi_sysctl_swWorkingModeDyn2(const rk_aiq_sys_ctx_t *ctx,
        work_mode_t *mode);

int rk_aiq_uapi_sysctl_getWorkingModeDyn(const rk_aiq_sys_ctx_t *ctx,
        work_mode_t *mode);

int rk_aiq_uapi2_setWBMode2(rk_aiq_sys_ctx_t *ctx, uapi_wb_mode_t *mode);

int rk_aiq_uapi2_getWBMode2(rk_aiq_sys_ctx_t *ctx, uapi_wb_mode_t *mode);

int rk_aiq_user_api2_set_scene(const rk_aiq_sys_ctx_t *sys_ctx,
                               aiq_scene_t *scene);

int rk_aiq_user_api2_get_scene(const rk_aiq_sys_ctx_t *sys_ctx,
                               aiq_scene_t *scene);

int rk_aiq_uapi_get_awb_stat(const rk_aiq_sys_ctx_t* sys_ctx,
                             rk_tool_awb_stat_res2_v30_t* awb_stat);

int rk_aiq_uapi_get_ae_hwstats(const rk_aiq_sys_ctx_t* sys_ctx,
                               uapi_ae_hwstats_t* ae_hwstats);
int rk_aiq_uapi_get_awbV32_stat(const rk_aiq_sys_ctx_t* sys_ctx,
                                rk_tool_isp_awb_stats_v32_t* awb_stat);
int rk_aiq_uapi_get_awbV21_stat(const rk_aiq_sys_ctx_t* sys_ctx,
                                rk_tool_awb_stat_res2_v201_t* awb_stat);

XCamReturn rk_aiq_user_api2_amerge_v10_queryinfo(const rk_aiq_sys_ctx_t* sys_ctx,
        uapiMergeCurrCtlData_t* merge_info);

XCamReturn rk_aiq_set_amerge_v10_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mMergeAttrV10_t* manual);

XCamReturn rk_aiq_get_amerge_v10_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mMergeAttrV10_t* manual);

XCamReturn rk_aiq_user_api2_amerge_v11_queryinfo(const rk_aiq_sys_ctx_t* sys_ctx,
        uapiMergeCurrCtlData_t* merge_info);

XCamReturn rk_aiq_set_amerge_v11_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mMergeAttrV11_t* manual);

XCamReturn rk_aiq_get_amerge_v11_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mMergeAttrV11_t* manual);

XCamReturn rk_aiq_user_api2_amerge_v12_queryinfo(const rk_aiq_sys_ctx_t* sys_ctx,
        uapiMergeCurrCtlData_t* merge_info);

XCamReturn rk_aiq_set_amerge_v12_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mMergeAttrV12_t* manual);

XCamReturn rk_aiq_get_amerge_v12_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mMergeAttrV12_t* manual);

XCamReturn rk_aiq_user_api2_adrc_v10_queryinfo(const rk_aiq_sys_ctx_t* sys_ctx,
        DrcInfoV10_t* drc_info);

XCamReturn rk_aiq_set_adrc_v10_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx, mdrcAttr_V10_t* manual);

XCamReturn rk_aiq_get_adrc_v10_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx, mdrcAttr_V10_t* manual);

XCamReturn rk_aiq_user_api2_adrc_v11_queryinfo(const rk_aiq_sys_ctx_t* sys_ctx,
        DrcInfoV11_t* drc_info);

XCamReturn rk_aiq_set_adrc_v11_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx, mdrcAttr_V11_t* manual);

XCamReturn rk_aiq_get_adrc_v11_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx, mdrcAttr_V11_t* manual);

XCamReturn rk_aiq_user_api2_adrc_v12_queryinfo(const rk_aiq_sys_ctx_t* sys_ctx,
        DrcInfoV12_t* drc_info);

XCamReturn rk_aiq_set_adrc_v12_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx, mdrcAttr_V12_t* manual);

XCamReturn rk_aiq_get_adrc_v12_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx, mdrcAttr_V12_t* manual);

XCamReturn rk_aiq_set_agamma_v10_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        AgammaApiManualV10_t* manual);

XCamReturn rk_aiq_get_agamma_v10_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        AgammaApiManualV10_t* manual);

XCamReturn rk_aiq_set_agamma_v11_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        AgammaApiManualV11_t* manual);

XCamReturn rk_aiq_get_agamma_v11_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        AgammaApiManualV11_t* manual);

XCamReturn rk_aiq_user_api2_adehaze_v11_queryinfo(const rk_aiq_sys_ctx_t* sys_ctx,
        mDehazeAttrInfoV11_t* dehaze_info);

XCamReturn rk_aiq_set_adehaze_v11_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mDehazeAttrV11_t* manual);

XCamReturn rk_aiq_get_adehaze_v11_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mDehazeAttrV11_t* manual);

XCamReturn rk_aiq_user_api2_adehaze_v12_queryinfo(const rk_aiq_sys_ctx_t* sys_ctx,
        mDehazeAttrInfoV11_t* dehaze_info);

XCamReturn rk_aiq_set_adehaze_v12_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mDehazeAttrV12_t* manual);

XCamReturn rk_aiq_get_adehaze_v12_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        mDehazeAttrV12_t* manual);

XCamReturn rk_aiq_set_accm_v1_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        rk_aiq_ccm_mccm_attrib_t* manual);

XCamReturn rk_aiq_get_accm_v1_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        rk_aiq_ccm_mccm_attrib_t* manual);

XCamReturn rk_aiq_set_accm_v2_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        rk_aiq_ccm_mccm_attrib_v2_t* manual);

XCamReturn rk_aiq_get_accm_v2_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
        rk_aiq_ccm_mccm_attrib_v2_t* manual);

XCamReturn rk_aiq_set_acgc_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
                                       Cgc_Param_t* manual) ;

XCamReturn rk_aiq_get_acgc_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
                                       Cgc_Param_t* manual);

XCamReturn rk_aiq_set_acsm_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
                                       Csm_Param_t* manual) ;

XCamReturn rk_aiq_get_acsm_manual_attr(const rk_aiq_sys_ctx_t* sys_ctx,
                                       Csm_Param_t* manual);

XCamReturn rk_aiq_set_ablc_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
        ablc_uapi_manual_t *manual);

XCamReturn rk_aiq_get_ablc_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
        ablc_uapi_manual_t *manual);

XCamReturn rk_aiq_set_asharp_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
        asharp_uapi_manual_t *manual);

XCamReturn rk_aiq_get_asharp_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
        asharp_uapi_manual_t *manual);

XCamReturn
rk_aiq_set_abayer2dnr_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                                  abayer2dnr_uapi_manual_t *manual);

XCamReturn
rk_aiq_get_abayer2dnr_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                                  abayer2dnr_uapi_manual_t *manual);

XCamReturn
rk_aiq_set_abayertnr_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                                 abayertnr_uapi_manual_t *manual);

XCamReturn
rk_aiq_get_abayertnr_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                                 abayertnr_uapi_manual_t *manual);


XCamReturn
rk_aiq_set_aynr_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                            aynr_uapi_manual_t *manual);

XCamReturn
rk_aiq_get_aynr_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                            aynr_uapi_manual_t *manual);

XCamReturn
rk_aiq_set_acnr_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                            acnr_uapi_manual_t *manual);

XCamReturn
rk_aiq_get_acnr_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                            acnr_uapi_manual_t *manual);

XCamReturn
rk_aiq_set_again_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                             again_uapi_manual_t *manual);

XCamReturn
rk_aiq_get_again_manual_attr(const rk_aiq_sys_ctx_t *sys_ctx,
                             again_uapi_manual_t *manual);

XCamReturn
rk_aiq_get_ablc_info(const rk_aiq_sys_ctx_t *sys_ctx,
                     ablc_uapi_info_t *info);

XCamReturn
rk_aiq_get_abayertnr_info(const rk_aiq_sys_ctx_t *sys_ctx,
                          abayertnr_uapi_info_t *info);

XCamReturn
rk_aiq_get_abayer2dnr_info(const rk_aiq_sys_ctx_t *sys_ctx,
                           abayer2dnr_uapi_info_t *info) ;
XCamReturn
rk_aiq_get_aynr_info(const rk_aiq_sys_ctx_t *sys_ctx,
                     aynr_uapi_info_t *info);
XCamReturn
rk_aiq_get_acnr_info(const rk_aiq_sys_ctx_t *sys_ctx,
                     acnr_uapi_info_t *info);
XCamReturn
rk_aiq_get_asharp_info(const rk_aiq_sys_ctx_t *sys_ctx,
                       asharp_uapi_info_t *info);

XCamReturn
rk_aiq_get_again_info(const rk_aiq_sys_ctx_t *sys_ctx,
                      again_uapi_info_t *info);



#endif /*__RK_AIQ_USER_API2_WRAPPER_H__*/
