#ifndef _RK_AIQ_UAPI_AMFNR_INT_V1_H_
#define _RK_AIQ_UAPI_AMFNR_INT_V1_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "amfnr/rk_aiq_types_amfnr_algo_int_v1.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.


XCamReturn
rk_aiq_uapi_amfnr_SetAttrib_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_mfnr_attrib_v1_t *attr,
                          bool need_sync);

XCamReturn
rk_aiq_uapi_amfnr_GetAttrib_v1(const RkAiqAlgoContext *ctx,
                          rk_aiq_mfnr_attrib_v1_t *attr);

XCamReturn
rk_aiq_uapi_amfnr_SetIQPara_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_mfnr_IQPara_V1_t *pPara,
                          bool need_sync);

XCamReturn
rk_aiq_uapi_amfnr_GetIQPara_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_mfnr_IQPara_V1_t *pPara);

XCamReturn
rk_aiq_uapi_amfnr_SetLumaTFStrength_v1(const RkAiqAlgoContext *ctx,
                          float fPercent);

XCamReturn
rk_aiq_uapi_amfnr_GetLumaTFStrength_v1(const RkAiqAlgoContext *ctx,
                          float *pPercent);

XCamReturn
rk_aiq_uapi_amfnr_SetChromaTFStrength_v1(const RkAiqAlgoContext *ctx,
                          float fPercent);

XCamReturn
rk_aiq_uapi_amfnr_GetChromaTFStrength_v1(const RkAiqAlgoContext *ctx,
                          float *pPercent);                       

XCamReturn
rk_aiq_uapi_amfnr_SetJsonPara_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_mfnr_JsonPara_V1_t *pPara,
                          bool need_sync);

XCamReturn
rk_aiq_uapi_amfnr_GetJsonPara_v1(RkAiqAlgoContext *ctx,
                          rk_aiq_mfnr_JsonPara_V1_t *pPara);




#endif
