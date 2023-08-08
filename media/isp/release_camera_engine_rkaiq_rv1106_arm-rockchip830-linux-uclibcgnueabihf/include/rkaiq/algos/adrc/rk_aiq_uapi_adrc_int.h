#ifndef _RK_AIQ_UAPI_ADRC_INT_H_
#define _RK_AIQ_UAPI_ADRC_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "rk_aiq_types_adrc_algo_int.h"
#include "adrc_uapi_head.h"


// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAwbHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn rk_aiq_uapi_adrc_v10_SetAttrib(RkAiqAlgoContext* ctx, const drcAttrV10_t* attr,
                                          bool need_sync);
XCamReturn rk_aiq_uapi_adrc_v10_GetAttrib(RkAiqAlgoContext* ctx, drcAttrV10_t* attr);
XCamReturn rk_aiq_uapi_adrc_v11_SetAttrib(RkAiqAlgoContext* ctx, const drcAttrV11_t* attr,
                                          bool need_sync);
XCamReturn rk_aiq_uapi_adrc_v11_GetAttrib(RkAiqAlgoContext* ctx, drcAttrV11_t* attr);
XCamReturn rk_aiq_uapi_adrc_v12_SetAttrib(RkAiqAlgoContext* ctx, const drcAttrV12_t* attr,
                                          bool need_sync);
XCamReturn rk_aiq_uapi_adrc_v12_GetAttrib(RkAiqAlgoContext* ctx, drcAttrV12_t* attr);

#endif
