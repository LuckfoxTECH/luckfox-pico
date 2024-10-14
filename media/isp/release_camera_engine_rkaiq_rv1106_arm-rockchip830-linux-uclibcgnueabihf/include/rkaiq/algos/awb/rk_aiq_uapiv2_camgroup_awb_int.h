/*
 * rk_aiq_uapi_camgroup_a3dlut_int.h
 *
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
#ifndef _RK_AIQ_UAPIV2_CAMGROUP_AWB_INT_H_
#define _RK_AIQ_UAPIV2_CAMGROUP_AWB_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "awb/rk_aiq_types_awb_algo_int.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAwbHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.


XCamReturn
rk_aiq_uapiV2_camgroup_awbV21_SetAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV21_attrib_t &attr,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_camgroup_awbV21_GetAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV21_attrib_t *attr);

XCamReturn
rk_aiq_uapiV2_camgroup_awb_SetMwbMode(RkAiqAlgoContext *ctx,
                          const rk_aiq_wb_op_mode_t &mode,
                          bool need_sync);

XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetMwbMode(RkAiqAlgoContext *ctx,
                          rk_aiq_wb_op_mode_t *mode);

XCamReturn
rk_aiq_uapiV2_camgroup_awb_SetMwbAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_wb_mwb_attrib_t &attr,
                          bool need_sync);

XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetMwbAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_wb_mwb_attrib_t *attr);

XCamReturn
rk_aiq_uapiV2_camgroup_awbV21_SetAwbAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV21_awb_attrib_t &attr,
                          bool need_sync);

XCamReturn
rk_aiq_uapiV2_camgroup_awbV21_GetAwbAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV21_awb_attrib_t *attr);

XCamReturn
rk_aiq_uapiV2_camgroup_awb_SetAwbGainAdjust(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wb_awb_wbGainAdjust_t &wbGainAdjust,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetAwbGainAdjust(RkAiqAlgoContext *ctx,
                           rk_aiq_uapiV2_wb_awb_wbGainAdjust_t *wbGainAdjust);

XCamReturn
rk_aiq_uapiV2_camgroup_awb_SetAwbGainOffset(RkAiqAlgoContext *ctx,
                          CalibDbV2_Awb_gain_offset_cfg_t wbGainOffset,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetAwbGainOffset(RkAiqAlgoContext *ctx,
                          CalibDbV2_Awb_gain_offset_cfg_t *wbGainOffset);
XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetCCT(const RkAiqAlgoContext *ctx,
                       rk_aiq_wb_cct_t *cct );

XCamReturn
rk_aiq_uapiV2_camgroup_awb_SetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         const CalibDbV2_Awb_Mul_Win_t  &multiWindow,
                         bool need_sync);
XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         CalibDbV2_Awb_Mul_Win_t  *multiWindow);

XCamReturn
rk_aiq_uapiV2_camgroup_awb_QueryWBInfo(const RkAiqAlgoContext *ctx,
                            rk_aiq_wb_querry_info_t *wb_querry_info );
XCamReturn
rk_aiq_uapiV2_camgroup_awb_Lock(RkAiqAlgoContext *ctx);

XCamReturn
rk_aiq_uapiV2_camgroup_awb_Unlock(const RkAiqAlgoContext *ctx);
XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetAlgoStat(const RkAiqAlgoContext *ctx,
                            rk_tool_awb_stat_res_full_t *awb_stat_algo );
XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetStrategyResult(const RkAiqAlgoContext *ctx,
                            rk_tool_awb_strategy_result_t *awb_strategy_result);
XCamReturn
rk_aiq_uapiV2_camgroup_awbV32_SetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         const rk_aiq_uapiV2_wbV32_awb_mulWindow_t  &multiWindow,
                         bool need_sync);
XCamReturn
rk_aiq_uapiV2_camgroup_awbV32_GetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         rk_aiq_uapiV2_wbV32_awb_mulWindow_t  *multiWindow);
XCamReturn
rk_aiq_uapiV2_camgroup_awbV32_SetAwbAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV32_awb_attrib_t &attr,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_camgroup_awbV32_GetAwbAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV32_awb_attrib_t *attr);
XCamReturn
rk_aiq_uapiV2_camgroup_awbV32_SetAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV32_attrib_t &attr,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_camgroup_awbV32_GetAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV32_attrib_t *attr);
XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetIQAutoExtPara(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_Wb_Awb_IqAtExtPa_V32_t *autoExtPara);
XCamReturn
rk_aiq_uapiV2_camgroup_awb_SetIQAutoExtPara(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_Wb_Awb_IqAtExtPa_V32_t *autoExtPara,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_camgroup_awb_GetIQAutoPara(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_Wb_Awb_IqAtPa_V32_t *autoPara);
XCamReturn
rk_aiq_uapiV2_camgroup_awb_SetIQAutoPara(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_Wb_Awb_IqAtPa_V32_t *autoPara,
                          bool need_sync);
#endif

