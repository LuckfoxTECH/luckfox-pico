#ifndef _RK_AIQ_UAPI_ADEBAYER_INT_H_
#define _RK_AIQ_UAPI_ADEBAYER_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"

typedef struct adebayer_attrib_s {
    unsigned char enable;
    unsigned char enhance_strength[9];
    unsigned char low_freq_thresh;
    unsigned char high_freq_thresh;
} adebayer_attrib_t;

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAdebayerHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_adebayer_SetAttrib
(
    RkAiqAlgoContext* ctx,
    adebayer_attrib_t attr,
    bool need_sync
);

XCamReturn
rk_aiq_uapi_adebayer_GetAttrib
(
    RkAiqAlgoContext*  ctx,
    adebayer_attrib_t* attr
);

#endif//_RK_AIQ_UAPI_ADEBAYER_INT_H_