#ifndef _RK_AIQ_UAPI_ACNR_INT_V1_H_
#define _RK_AIQ_UAPI_ACNR_INT_V1_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "acnr/rk_aiq_types_acnr_algo_int_v1.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_acnrV1_SetAttrib(RkAiqAlgoContext *ctx,
                             rk_aiq_cnr_attrib_v1_t *attr,
                             bool need_sync);

XCamReturn
rk_aiq_uapi_acnrV1_GetAttrib(const RkAiqAlgoContext *ctx,
                             rk_aiq_cnr_attrib_v1_t *attr);

XCamReturn
rk_aiq_uapi_acnrV1_SetChromaSFStrength(const RkAiqAlgoContext *ctx,
                                       float fPercent);

XCamReturn
rk_aiq_uapi_acnrV1_GetChromaSFStrength(const RkAiqAlgoContext *ctx,
                                       float *pPercent);


#endif
