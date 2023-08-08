#ifndef _RK_AIQ_UAPI_ASHARP_INT_V4_H_
#define _RK_AIQ_UAPI_ASHARP_INT_V4_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "asharp4/rk_aiq_types_asharp_algo_int_v4.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.


XCamReturn
rk_aiq_uapi_asharpV4_SetAttrib(RkAiqAlgoContext *ctx,
                               rk_aiq_sharp_attrib_v4_t *attr,
                               bool need_sync);

XCamReturn
rk_aiq_uapi_asharpV4_GetAttrib(const RkAiqAlgoContext *ctx,
                               rk_aiq_sharp_attrib_v4_t *attr);

XCamReturn
rk_aiq_uapi_asharpV4_SetStrength(const RkAiqAlgoContext *ctx,
                                 rk_aiq_sharp_strength_v4_t *pStrength);

XCamReturn
rk_aiq_uapi_asharpV4_GetStrength(const RkAiqAlgoContext *ctx,
                                 rk_aiq_sharp_strength_v4_t *pStrength);

XCamReturn
rk_aiq_uapi_asharpV4_GetInfo(const RkAiqAlgoContext *ctx,
                             rk_aiq_sharp_info_v4_t *pInfo);




#endif
