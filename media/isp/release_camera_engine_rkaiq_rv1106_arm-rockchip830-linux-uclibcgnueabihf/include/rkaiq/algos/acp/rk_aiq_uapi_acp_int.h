#ifndef _RK_AIQ_UAPI_ACP_INT_H_
#define _RK_AIQ_UAPI_ACP_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"

typedef struct acp_attrib_s {
    rk_aiq_uapi_sync_t sync;

    uint8_t brightness; /* 0 ~ 255 */
    uint8_t contrast; /* 0 ~ 255 */
    uint8_t saturation; /* 0 ~ 255 */
    uint8_t hue; /* 0 ~ 255 */
} acp_attrib_t;

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAdebayerHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn rk_aiq_uapi_acp_SetAttrib(RkAiqAlgoContext* ctx,
                                    const acp_attrib_t* attr,
                                    bool need_sync);

XCamReturn rk_aiq_uapi_acp_GetAttrib(RkAiqAlgoContext*  ctx,
                                    acp_attrib_t* attr);

#endif//_RK_AIQ_UAPI_ACP_INT_H_
