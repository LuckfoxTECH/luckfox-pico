#ifndef _RK_AIQ_UAPI_AYNR_INT_V1_H_
#define _RK_AIQ_UAPI_AYNR_INT_V1_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "aynr/rk_aiq_types_aynr_algo_int_v1.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.


XCamReturn
rk_aiq_uapi_aynr_SetAttrib_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_ynr_attrib_v1_t *attr,
                          bool need_sync);

XCamReturn
rk_aiq_uapi_aynr_GetAttrib_v1(const RkAiqAlgoContext *ctx,
                          rk_aiq_ynr_attrib_v1_t *attr);

XCamReturn
rk_aiq_uapi_aynr_SetIQPara_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_ynr_IQPara_V1_t *pPara,
                          bool need_sync);

XCamReturn
rk_aiq_uapi_aynr_GetIQPara_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_ynr_IQPara_V1_t *pPara);

XCamReturn
rk_aiq_uapi_aynr_SetLumaSFStrength_v1(const RkAiqAlgoContext *ctx,
                          float fPercent);

XCamReturn
rk_aiq_uapi_aynr_GetLumaSFStrength_v1(const RkAiqAlgoContext *ctx,
                          float *pPercent);

XCamReturn
rk_aiq_uapi_aynr_SetJsonPara_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_ynr_JsonPara_V1_t *pPara,
                          bool need_sync);

XCamReturn
rk_aiq_uapi_aynr_GetJsonPara_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_ynr_JsonPara_V1_t *pPara);




#endif
