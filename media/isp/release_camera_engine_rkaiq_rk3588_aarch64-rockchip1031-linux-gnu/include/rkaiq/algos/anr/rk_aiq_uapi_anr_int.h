#ifndef _RK_AIQ_UAPI_ANR_INT_H_
#define _RK_AIQ_UAPI_ANR_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "anr/rk_aiq_types_anr_algo_int.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_anr_SetAttrib(RkAiqAlgoContext *ctx,
                          rk_aiq_nr_attrib_t *attr,
                          bool need_sync);

XCamReturn
rk_aiq_uapi_anr_GetAttrib(const RkAiqAlgoContext *ctx,
                          rk_aiq_nr_attrib_t *attr);

XCamReturn
rk_aiq_uapi_anr_SetIQPara(RkAiqAlgoContext *ctx,
                          rk_aiq_nr_IQPara_t *pPara,
                          bool need_sync);

XCamReturn
rk_aiq_uapi_anr_GetIQPara(RkAiqAlgoContext *ctx,
                          rk_aiq_nr_IQPara_t *pPara);

XCamReturn
rk_aiq_uapi_anr_SetLumaSFStrength(const RkAiqAlgoContext *ctx,
                          float fPercnt);

XCamReturn
rk_aiq_uapi_anr_SetLumaTFStrength(const RkAiqAlgoContext *ctx,
                          float fPercnt);

XCamReturn
rk_aiq_uapi_anr_GetLumaSFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent);

XCamReturn
rk_aiq_uapi_anr_GetLumaTFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent);


XCamReturn
rk_aiq_uapi_anr_SetChromaSFStrength(const RkAiqAlgoContext *ctx,
                          float fPercnt);

XCamReturn
rk_aiq_uapi_anr_SetChromaTFStrength(const RkAiqAlgoContext *ctx,
                          float fPercnt);

XCamReturn
rk_aiq_uapi_anr_GetChromaSFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent);

XCamReturn
rk_aiq_uapi_anr_GetChromaTFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent);

XCamReturn
rk_aiq_uapi_anr_SetRawnrSFStrength(const RkAiqAlgoContext *ctx,
                          float fPercent);

XCamReturn
rk_aiq_uapi_anr_GetRawnrSFStrength(const RkAiqAlgoContext *ctx,
                          float *pPercent);


#endif
