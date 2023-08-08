#ifndef _RK_AIQ_UAPI_CAMGROUP_ABAYER2DNR_INT_V23_H_
#define _RK_AIQ_UAPI_CAMGROUP_ABAYER2DNR_INT_V23_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "abayer2dnrV23/rk_aiq_types_abayer2dnr_algo_int_v23.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.


XCamReturn
rk_aiq_uapi_camgroup_abayer2dnrV23_SetAttrib(RkAiqAlgoContext *ctx,
        const rk_aiq_bayer2dnr_attrib_v23_t *attr,
        bool need_sync);

XCamReturn
rk_aiq_uapi_camgroup_abayer2dnrV23_GetAttrib(const RkAiqAlgoContext *ctx,
        rk_aiq_bayer2dnr_attrib_v23_t *attr);

XCamReturn
rk_aiq_uapi_camgroup_abayer2dnrV23_SetStrength(const RkAiqAlgoContext *ctx,
        const rk_aiq_bayer2dnr_strength_v23_t* pStrength);

XCamReturn
rk_aiq_uapi_camgroup_abayer2dnrV23_GetStrength(const RkAiqAlgoContext *ctx,
        rk_aiq_bayer2dnr_strength_v23_t* pStrength);

XCamReturn
rk_aiq_uapi_camgroup_abayer2dnrV23_GetInfo(const RkAiqAlgoContext* ctx,
        rk_aiq_bayer2dnr_info_v23_t* pInfo);




#endif
