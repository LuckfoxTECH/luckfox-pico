/******************************************************************************
 *
 * Copyright 2019, Fuzhou Rockchip Electronics Co.Ltd . All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Fuzhou Rockchip Electronics Co.Ltd .
 *
 *
 *****************************************************************************/
#ifndef __RK_AIQ_UAPI_AE_INT_H__
#define __RK_AIQ_UAPI_AE_INT_H__
#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "rk_aiq_uapi_ae_int_types_v1.h"
#include "rk_aiq_uapi_ae_int_types_v2.h"

XCamReturn rk_aiq_uapi_ae_convExpSwAttr_v1Tov2(Uapi_ExpSwAttr_t* pExpSwAttrV1, Uapi_ExpSwAttrV2_t* pExpSwAttrV2);
XCamReturn rk_aiq_uapi_ae_convLinExpAttr_v1Tov2(Uapi_LinExpAttr_t* pLinExpAttrV1, Uapi_LinExpAttrV2_t* pLinExpAttrV2);
XCamReturn rk_aiq_uapi_ae_convHdrExpAttr_v1Tov2(Uapi_HdrExpAttr_t* pHdrExpAttrV1, Uapi_HdrExpAttrV2_t* pHdrExpAttrV2);
XCamReturn rk_aiq_uapi_ae_convExpSwAttr_v2Tov1(Uapi_ExpSwAttrV2_t* pExpSwAttrV2, Uapi_ExpSwAttr_t* pExpSwAttrV1);
XCamReturn rk_aiq_uapi_ae_convLinExpAttr_v2Tov1(Uapi_LinExpAttrV2_t* pLinExpAttrV2, Uapi_LinExpAttr_t* pLinExpAttrV1);
XCamReturn rk_aiq_uapi_ae_convHdrExpAttr_v2Tov1(Uapi_HdrExpAttrV2_t* pHdrExpAttrV2, Uapi_HdrExpAttr_t* pHdrExpAttrV1);

XCamReturn rk_aiq_uapi_ae_setExpSwAttr(RkAiqAlgoContext* ctx, const Uapi_ExpSwAttrV2_t* pExpSwAttr, bool isGroupAe, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getExpSwAttr(RkAiqAlgoContext* ctx, Uapi_ExpSwAttrV2_t* pExpSwAttr, bool isGroupAe);
XCamReturn rk_aiq_uapi_ae_setLinExpAttr(RkAiqAlgoContext* ctx, const Uapi_LinExpAttrV2_t* pLinExpAttr, bool isGroupAe, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getLinExpAttr(RkAiqAlgoContext* ctx, Uapi_LinExpAttrV2_t* pLinExpAttr, bool isGroupAe);
XCamReturn rk_aiq_uapi_ae_setHdrExpAttr(RkAiqAlgoContext* ctx, const Uapi_HdrExpAttrV2_t* pHdrExpAttr, bool isGroupAe, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getHdrExpAttr(RkAiqAlgoContext* ctx, Uapi_HdrExpAttrV2_t* pHdrExpAttr, bool isGroupAe);

XCamReturn rk_aiq_uapi_ae_setLinAeRouteAttr(RkAiqAlgoContext* ctx, const Uapi_LinAeRouteAttr_t* pLinAeRouteAttr, bool isGroupAe, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getLinAeRouteAttr(RkAiqAlgoContext* ctx, Uapi_LinAeRouteAttr_t* pLinAeRouteAttr, bool isGroupAe);
XCamReturn rk_aiq_uapi_ae_setHdrAeRouteAttr(RkAiqAlgoContext* ctx, const Uapi_HdrAeRouteAttr_t* pHdrAeRouteAttr, bool isGroupAe, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getHdrAeRouteAttr(RkAiqAlgoContext* ctx, Uapi_HdrAeRouteAttr_t* pHdrAeRouteAttr, bool isGroupAe);

XCamReturn rk_aiq_uapi_ae_setSyncTest(RkAiqAlgoContext* ctx, const Uapi_AecSyncTest_t* pAecSyncTestAttr, bool isGroupAe, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getSyncTest(RkAiqAlgoContext* ctx, Uapi_AecSyncTest_t* pAecSyncTestAttr, bool isGroupAe);
XCamReturn rk_aiq_uapi_ae_setIrisAttr(RkAiqAlgoContext* ctx, const Uapi_IrisAttrV2_t* pIrisAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getIrisAttr(RkAiqAlgoContext* ctx, Uapi_IrisAttrV2_t* pIrisAttr);

XCamReturn rk_aiq_uapi_ae_queryExpInfo(RkAiqAlgoContext* ctx, Uapi_ExpQueryInfo_t* pExpInfo, bool isGroupAe);
XCamReturn rk_aiq_uapi_ae_setExpWinAttr(RkAiqAlgoContext* ctx, const Uapi_ExpWin_t* pExpWinAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getExpWinAttr(RkAiqAlgoContext* ctx, Uapi_ExpWin_t* pExpWinAttr);


#endif /*__RK_AIQ_UAPI_AE_INT_H__*/
