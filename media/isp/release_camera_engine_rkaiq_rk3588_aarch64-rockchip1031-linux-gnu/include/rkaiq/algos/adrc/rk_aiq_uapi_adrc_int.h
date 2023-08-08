#ifndef _RK_AIQ_UAPI_ADRC_INT_H_
#define _RK_AIQ_UAPI_ADRC_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "rk_aiq_types_adrc_algo_int.h"
#include "adrc_uapi_head.h"

typedef drcAttr_t drc_attrib_t;

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAwbHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_adrc_SetAttrib(RkAiqAlgoContext *ctx,
                           drc_attrib_t attr,
                           bool need_sync);


XCamReturn
rk_aiq_uapi_adrc_GetAttrib(RkAiqAlgoContext *ctx,
                           drc_attrib_t *attr);


#endif
