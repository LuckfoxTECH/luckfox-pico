#ifndef _RK_AIQ_UAPI_A3DLUT_INT_H_
#define _RK_AIQ_UAPI_A3DLUT_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "a3dlut/rk_aiq_types_a3dlut_algo_int.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqA3dlutHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_a3dlut_SetAttrib(RkAiqAlgoContext *ctx,
                             const rk_aiq_lut3d_attrib_t* attr,
                             bool need_sync);
XCamReturn
rk_aiq_uapi_a3dlut_GetAttrib(const RkAiqAlgoContext *ctx,
                             rk_aiq_lut3d_attrib_t *attr);
XCamReturn
rk_aiq_uapi_a3dlut_Query3dlutInfo(const RkAiqAlgoContext *ctx,
                                  rk_aiq_lut3d_querry_info_t *lut3d_querry_info );

#endif
