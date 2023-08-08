#ifndef _RK_AIQ_UAPI_AYNR_INT_V22_H_
#define _RK_AIQ_UAPI_AYNR_INT_V22_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "aynrV22/rk_aiq_types_aynr_algo_int_v22.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.


XCamReturn
rk_aiq_uapi_aynrV22_SetAttrib(RkAiqAlgoContext *ctx,
                              const rk_aiq_ynr_attrib_v22_t *attr,
                              bool need_sync);

XCamReturn
rk_aiq_uapi_aynrV22_GetAttrib(const RkAiqAlgoContext *ctx,
                              rk_aiq_ynr_attrib_v22_t *attr);

XCamReturn
rk_aiq_uapi_aynrV22_SetLumaSFStrength(const RkAiqAlgoContext *ctx,
                                      const rk_aiq_ynr_strength_v22_t* pStrength);

XCamReturn
rk_aiq_uapi_aynrV22_GetLumaSFStrength(const RkAiqAlgoContext *ctx,
                                      rk_aiq_ynr_strength_v22_t* pStrength);


XCamReturn
rk_aiq_uapi_aynrV22_GetInfo(const RkAiqAlgoContext* ctx,
                            rk_aiq_ynr_info_v22_t* pInfo);



#endif
