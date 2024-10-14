#ifndef _RK_AIQ_UAPI_ACCM_INT_H_
#define _RK_AIQ_UAPI_ACCM_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "rk_aiq_types.h"
#include "accm/rk_aiq_types_accm_algo_int.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAccmHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_accm_SetAttrib(RkAiqAlgoContext *ctx,
                           const rk_aiq_ccm_attrib_t* attr,
                           bool need_sync);
XCamReturn
rk_aiq_uapi_accm_GetAttrib(const RkAiqAlgoContext *ctx,
                           rk_aiq_ccm_attrib_t *attr);
XCamReturn
rk_aiq_uapi_accm_v2_SetAttrib(RkAiqAlgoContext *ctx,
                           const rk_aiq_ccm_v2_attrib_t* attr,
                           bool need_sync);
XCamReturn
rk_aiq_uapi_accm_v2_GetAttrib(const RkAiqAlgoContext *ctx,
                           rk_aiq_ccm_v2_attrib_t *attr);
XCamReturn
rk_aiq_uapi_accm_QueryCcmInfo(const RkAiqAlgoContext *ctx,
                              rk_aiq_ccm_querry_info_t *ccm_querry_info );

#if RKAIQ_HAVE_CCM_V1
XCamReturn
rk_aiq_uapi_accm_SetIqParam(RkAiqAlgoContext *ctx,
                           const rk_aiq_ccm_calib_attrib_t* attr,
                           bool need_sync);
XCamReturn
rk_aiq_uapi_accm_GetIqParam(const RkAiqAlgoContext *ctx,
                            rk_aiq_ccm_calib_attrib_t* attr);
#elif RKAIQ_HAVE_CCM_V2
XCamReturn
rk_aiq_uapi_accm_SetIqParam(RkAiqAlgoContext *ctx,
                           const rk_aiq_ccm_v2_calib_attrib_t* attr,
                           bool need_sync);
XCamReturn
rk_aiq_uapi_accm_GetIqParam(const RkAiqAlgoContext *ctx,
                            rk_aiq_ccm_v2_calib_attrib_t* attr);
#endif

#endif
