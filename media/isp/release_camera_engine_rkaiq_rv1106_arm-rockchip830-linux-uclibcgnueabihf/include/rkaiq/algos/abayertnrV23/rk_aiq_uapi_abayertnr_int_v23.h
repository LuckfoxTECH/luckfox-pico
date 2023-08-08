#ifndef _RK_AIQ_UAPI_ABAYERTNR_INT_V23_H_
#define _RK_AIQ_UAPI_ABAYERTNR_INT_V23_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "abayertnrV23/rk_aiq_types_abayertnr_algo_int_v23.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.


XCamReturn
rk_aiq_uapi_abayertnrV23_SetAttrib(RkAiqAlgoContext *ctx,
                                   const rk_aiq_bayertnr_attrib_v23_t *attr,
                                   bool need_sync);

XCamReturn
rk_aiq_uapi_abayertnrV23_GetAttrib(const RkAiqAlgoContext *ctx,
                                   rk_aiq_bayertnr_attrib_v23_t *attr);


XCamReturn
rk_aiq_uapi_abayertnrV23_SetStrength(const RkAiqAlgoContext *ctx,
                                     const rk_aiq_bayertnr_strength_v23_t* pStrength);

XCamReturn
rk_aiq_uapi_abayertnrV23_GetStrength(const RkAiqAlgoContext *ctx,
                                     rk_aiq_bayertnr_strength_v23_t* pStrength);

XCamReturn
rk_aiq_uapi_abayertnrV23_GetInfo(const RkAiqAlgoContext* ctx,
                                 rk_aiq_bayertnr_info_v23_t* pInfo);



#endif
