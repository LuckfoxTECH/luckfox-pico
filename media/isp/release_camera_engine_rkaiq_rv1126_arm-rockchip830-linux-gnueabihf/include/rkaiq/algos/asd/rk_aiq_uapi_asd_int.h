#ifndef _RK_AIQ_UAPI_ASD_INT_H_
#define _RK_AIQ_UAPI_ASD_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"

typedef struct asd_attrib_s {
    float cur_m2r;
} asd_attrib_t;

XCamReturn
rk_aiq_uapi_asd_SetAttrib
(
    RkAiqAlgoContext* ctx,
    asd_attrib_t attr,
    bool need_sync
);

XCamReturn
rk_aiq_uapi_asd_GetAttrib
(
    RkAiqAlgoContext*  ctx,
    asd_attrib_t* attr
);

#endif//_RK_AIQ_UAPI_ASD_INT_H_