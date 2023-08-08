#ifndef _RK_AIQ_UAPI_ABAYERNR_INT_V2_H_
#define _RK_AIQ_UAPI_ABAYERNR_INT_V2_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "arawnr2/rk_aiq_types_abayernr_algo_int_v2.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.


XCamReturn
rk_aiq_uapi_arawnrV2_SetAttrib(RkAiqAlgoContext *ctx,
                               rk_aiq_bayernr_attrib_v2_t *attr,
                               bool need_sync);

XCamReturn
rk_aiq_uapi_arawnrV2_GetAttrib(const RkAiqAlgoContext *ctx,
                               rk_aiq_bayernr_attrib_v2_t *attr);

XCamReturn
rk_aiq_uapi_rawnrV2_SetSFStrength(const RkAiqAlgoContext *ctx,
                                  float fPercent);

XCamReturn
rk_aiq_uapi_rawnrV2_SetTFStrength(const RkAiqAlgoContext *ctx,
                                  float fPercent);

XCamReturn
rk_aiq_uapi_rawnrV2_GetSFStrength(const RkAiqAlgoContext *ctx,
                                  float *pPercent);

XCamReturn
rk_aiq_uapi_rawnrV2_GetTFStrength(const RkAiqAlgoContext *ctx,
                                  float *pPercent);







#endif
