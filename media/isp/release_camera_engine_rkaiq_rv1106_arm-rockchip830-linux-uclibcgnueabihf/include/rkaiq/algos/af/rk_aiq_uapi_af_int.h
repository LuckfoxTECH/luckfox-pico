#ifndef _RK_AIQ_UAPI_AF_INT_H_
#define _RK_AIQ_UAPI_AF_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "rk_aiq_types.h"
#include "af/rk_aiq_types_af_algo_int.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAwbHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_af_SetAttrib(RkAiqAlgoContext *ctx,
                         rk_aiq_af_attrib_t attr,
                         bool need_sync);
XCamReturn
rk_aiq_uapi_af_GetAttrib(const RkAiqAlgoContext *ctx,
                         rk_aiq_af_attrib_t *attr);
XCamReturn
rk_aiq_uapi_af_Lock(RkAiqAlgoContext *ctx);
XCamReturn
rk_aiq_uapi_af_Unlock(const RkAiqAlgoContext *ctx);
XCamReturn
rk_aiq_uapi_af_Oneshot(const RkAiqAlgoContext *ctx);
XCamReturn
rk_aiq_uapi_af_ManualTriger(const RkAiqAlgoContext *ctx);
XCamReturn
rk_aiq_uapi_af_Tracking(const RkAiqAlgoContext *ctx);
XCamReturn
rk_aiq_uapi_af_setZoomIndex(const RkAiqAlgoContext *ctx, int index);
XCamReturn
rk_aiq_uapi_af_getZoomIndex(const RkAiqAlgoContext *ctx, int *index);
XCamReturn
rk_aiq_uapi_af_endZoomChg(const RkAiqAlgoContext *ctx);
XCamReturn
rk_aiq_uapi_af_startZoomCalib(const RkAiqAlgoContext *ctx);
XCamReturn
rk_aiq_uapi_af_getSearchPath(const RkAiqAlgoContext *ctx, rk_aiq_af_sec_path_t* path);
XCamReturn
rk_aiq_uapi_af_getSearchResult(const RkAiqAlgoContext *ctx, rk_aiq_af_result_t* result);
XCamReturn
rk_aiq_uapi_af_getFocusRange(const RkAiqAlgoContext *ctx, rk_aiq_af_focusrange* range);
XCamReturn
rk_aiq_uapi_af_resetZoom(const RkAiqAlgoContext *ctx);

#endif
