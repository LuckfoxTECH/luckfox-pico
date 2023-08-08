#ifndef _RK_AIQ_UAPI_AMERGE_INT_H_
#define _RK_AIQ_UAPI_AMERGE_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "rk_aiq_types_amerge_algo_int.h"


// TODO: move the structs define to a separate file,
//TODO: move the structs define to a separate file,
// eg. rk_aiq_uapi_awb_types.h ?

typedef mergeAttr_t amerge_attrib_t;


// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAwbHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_amerge_SetAttrib(RkAiqAlgoContext *ctx,
                             amerge_attrib_t attr,
                             bool need_sync);


XCamReturn
rk_aiq_uapi_amerge_GetAttrib(RkAiqAlgoContext *ctx,
                             amerge_attrib_t *attr);


#endif
