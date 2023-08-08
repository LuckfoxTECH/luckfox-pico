#ifndef _RK_AIQ_UAPIV2_AWB_INT_H_
#define _RK_AIQ_UAPIV2_AWB_INT_H_

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
rk_aiq_uapiV2_awbV20_SetAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV20_attrib_t &attr,
                          bool need_sync);

XCamReturn
rk_aiq_uapiV2_awbV20_GetAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV20_attrib_t *attr);

XCamReturn
rk_aiq_uapiV2_awbV21_SetAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV21_attrib_t &attr,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_awbV21_GetAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV21_attrib_t *attr);

XCamReturn
rk_aiq_uapiV2_awb_SetMwbMode(RkAiqAlgoContext *ctx,
                          const rk_aiq_wb_op_mode_t &mode,
                          bool need_sync);

XCamReturn
rk_aiq_uapiV2_awb_GetMwbMode(RkAiqAlgoContext *ctx,
                          rk_aiq_wb_op_mode_t *mode);

XCamReturn
rk_aiq_uapiV2_awb_SetMwbAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_wb_mwb_attrib_t &attr,
                          bool need_sync);

XCamReturn
rk_aiq_uapiV2_awb_GetMwbAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_wb_mwb_attrib_t *attr);

XCamReturn
rk_aiq_uapiV2_awbV20_SetAwbAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV20_awb_attrib_t &attr,
                          bool need_sync);

XCamReturn
rk_aiq_uapiV2_awbV20_GetAwbAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV20_awb_attrib_t *attr);

XCamReturn
rk_aiq_uapiV2_awbV21_SetAwbAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV21_awb_attrib_t &attr,
                          bool need_sync);

XCamReturn
rk_aiq_uapiV2_awbV21_GetAwbAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV21_awb_attrib_t *attr);

XCamReturn
rk_aiq_uapiV2_awb_SetAwbGainAdjust(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wb_awb_wbGainAdjust_t &wbGainAdjust,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_awb_GetAwbGainAdjust(RkAiqAlgoContext *ctx,
                           rk_aiq_uapiV2_wb_awb_wbGainAdjust_t *wbGainAdjust);

XCamReturn
rk_aiq_uapiV2_awb_SetAwbGainOffset(RkAiqAlgoContext *ctx,
                          CalibDbV2_Awb_gain_offset_cfg_t wbGainOffset,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_awb_GetAwbGainOffset(RkAiqAlgoContext *ctx,
                          CalibDbV2_Awb_gain_offset_cfg_t *wbGainOffset);
XCamReturn
rk_aiq_uapiV2_awb_GetCCT(const RkAiqAlgoContext *ctx,
                       rk_aiq_wb_cct_t *cct );

XCamReturn
rk_aiq_uapiV2_awb_SetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         const CalibDbV2_Awb_Mul_Win_t  &multiWindow,
                         bool need_sync);
XCamReturn
rk_aiq_uapiV2_awb_GetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         CalibDbV2_Awb_Mul_Win_t  *multiWindow);

XCamReturn
rk_aiq_uapiV2_awb_QueryWBInfo(const RkAiqAlgoContext *ctx,
                            rk_aiq_wb_querry_info_t *wb_querry_info );
XCamReturn
rk_aiq_uapiV2_awb_Lock(RkAiqAlgoContext *ctx);

XCamReturn
rk_aiq_uapiV2_awb_Unlock(const RkAiqAlgoContext *ctx);

XCamReturn
rk_aiq_uapiV2_awb_GetAlgoStat(const RkAiqAlgoContext *ctx,rk_tool_awb_stat_res_full_t *awb_stat_algo);

XCamReturn
rk_aiq_uapiV2_awb_GetStrategyResult(const RkAiqAlgoContext *ctx,rk_tool_awb_strategy_result_t *awb_strategy_result);
XCamReturn
rk_aiq_uapiV2_awbV32_SetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         const rk_aiq_uapiV2_wbV32_awb_mulWindow_t  &multiWindow,
                         bool need_sync);
XCamReturn
rk_aiq_uapiV2_awbV32_GetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         rk_aiq_uapiV2_wbV32_awb_mulWindow_t  *multiWindow);
XCamReturn
rk_aiq_uapiV2_awbV32_SetAwbAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV32_awb_attrib_t &attr,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_awbV32_GetAwbAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV32_awb_attrib_t *attr);

XCamReturn
rk_aiq_uapiV2_awbV32_SetAttrib(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_wbV32_attrib_t &attr,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_awbV32_GetAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_uapiV2_wbV32_attrib_t *attr);
XCamReturn
rk_aiq_uapiV2_awbV32_GetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         rk_aiq_uapiV2_wbV32_awb_mulWindow_t  *multiWindow);
XCamReturn
rk_aiq_uapiV2_awbV32_SetAwbMultiwindow(RkAiqAlgoContext *ctx,
                         const rk_aiq_uapiV2_wbV32_awb_mulWindow_t  &multiWindow,
                         bool need_sync);
XCamReturn
rk_aiq_uapiV2_awb_WriteInput(RkAiqAlgoContext *ctx,
                          const rk_aiq_uapiV2_awb_wrtIn_attr_t &attr,
                          bool need_sync);
XCamReturn
rk_aiq_uapiV2_awb_SetFstFrWbgain(RkAiqAlgoContext *ctx,
                          const rk_aiq_wb_gain_t &wbgain,
                          bool need_sync);

#endif

