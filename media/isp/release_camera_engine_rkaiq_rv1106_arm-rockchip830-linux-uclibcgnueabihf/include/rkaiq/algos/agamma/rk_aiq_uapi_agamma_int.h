#ifndef _RK_AIQ_UAPI_AGAMMA_INT_H_
#define _RK_AIQ_UAPI_AGAMMA_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "agamma/rk_aiq_types_agamma_algo_int.h"
//#include "agamma/rk_aiq_types_agamma_algo.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn rk_aiq_uapi_agamma_v10_SetAttrib(RkAiqAlgoContext* ctx,
                                            const rk_aiq_gamma_v10_attr_t* attr, bool need_sync);
XCamReturn rk_aiq_uapi_agamma_v10_GetAttrib(const RkAiqAlgoContext* ctx,
                                            rk_aiq_gamma_v10_attr_t* attr);
XCamReturn rk_aiq_uapi_agamma_v11_SetAttrib(RkAiqAlgoContext* ctx,
                                            const rk_aiq_gamma_v11_attr_t* attr, bool need_sync);
XCamReturn rk_aiq_uapi_agamma_v11_GetAttrib(const RkAiqAlgoContext* ctx,
                                            rk_aiq_gamma_v11_attr_t* attr);

#endif
