#ifndef _RK_AIQ_UAPI_AMERGE_INT_H_
#define _RK_AIQ_UAPI_AMERGE_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "rk_aiq_types_amerge_algo_int.h"


// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAwbHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn rk_aiq_uapi_amerge_v10_SetAttrib(RkAiqAlgoContext* ctx, const mergeAttrV10_t* attr,
                                            bool need_sync);
XCamReturn rk_aiq_uapi_amerge_v10_GetAttrib(RkAiqAlgoContext* ctx, mergeAttrV10_t* attr);
XCamReturn rk_aiq_uapi_amerge_v11_SetAttrib(RkAiqAlgoContext* ctx, const mergeAttrV11_t* attr,
                                            bool need_sync);
XCamReturn rk_aiq_uapi_amerge_v11_GetAttrib(RkAiqAlgoContext* ctx, mergeAttrV11_t* attr);
XCamReturn rk_aiq_uapi_amerge_v12_SetAttrib(RkAiqAlgoContext* ctx, const mergeAttrV12_t* attr,
                                            bool need_sync);
XCamReturn rk_aiq_uapi_amerge_v12_GetAttrib(RkAiqAlgoContext* ctx, mergeAttrV12_t* attr);

#endif
