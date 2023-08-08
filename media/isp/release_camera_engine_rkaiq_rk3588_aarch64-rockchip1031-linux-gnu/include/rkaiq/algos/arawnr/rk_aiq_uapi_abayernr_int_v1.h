#ifndef _RK_AIQ_UAPI_ABAYERNR_INT_V1_H_
#define _RK_AIQ_UAPI_ABAYERNR_INT_V1_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "arawnr/rk_aiq_types_abayernr_algo_int_v1.h"


#if 1
XCamReturn
rk_aiq_uapi_abayernr_SetAttrib_v1(RkAiqAlgoContext *ctx,
                                  rk_aiq_bayernr_attrib_v1_t *attr,
                                  bool need_sync);

XCamReturn
rk_aiq_uapi_abayernr_GetAttrib_v1(const RkAiqAlgoContext *ctx,
                                  rk_aiq_bayernr_attrib_v1_t *attr);

XCamReturn
rk_aiq_uapi_abayernr_SetIQPara_v1(RkAiqAlgoContext *ctx,
                                  rk_aiq_bayernr_IQPara_V1_t *pPara,
                                  bool need_sync);

XCamReturn
rk_aiq_uapi_abayernr_GetIQPara_v1(RkAiqAlgoContext *ctx,
                                  rk_aiq_bayernr_IQPara_V1_t *pPara);


XCamReturn
rk_aiq_uapi_abayernr_SetRawnrSFStrength_v1(const RkAiqAlgoContext *ctx,
        float fPercent);

XCamReturn
rk_aiq_uapi_abayernr_GetRawnrSFStrength_v1(const RkAiqAlgoContext *ctx,
        float *pPercent);


XCamReturn
rk_aiq_uapi_abayernr_SetJsonPara_v1(RkAiqAlgoContext *ctx,
                                    rk_aiq_bayernr_JsonPara_V1_t *pPara,
                                    bool need_sync);

XCamReturn
rk_aiq_uapi_abayernr_GetJsonPara_v1(RkAiqAlgoContext *ctx,
                                    rk_aiq_bayernr_JsonPara_V1_t *pPara);
#endif


#endif
