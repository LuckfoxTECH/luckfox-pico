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
#include "rk_aiq_uapi_ae_int_types.h"

XCamReturn rk_aiq_uapi_ae_setExpSwAttr(RkAiqAlgoContext* ctx, const Uapi_ExpSwAttr_t* pExpSwAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getExpSwAttr(RkAiqAlgoContext* ctx, Uapi_ExpSwAttr_t* pExpSwAttr);
XCamReturn rk_aiq_uapi_ae_setLinAeDayRouteAttr(RkAiqAlgoContext* ctx, const Uapi_LinAeRouteAttr_t* pLinAeRouteAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getLinAeDayRouteAttr(RkAiqAlgoContext* ctx, Uapi_LinAeRouteAttr_t* pLinAeRouteAttr);
XCamReturn rk_aiq_uapi_ae_setLinAeNightRouteAttr(RkAiqAlgoContext* ctx, const Uapi_LinAeRouteAttr_t* pLinAeRouteAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getLinAeNightRouteAttr(RkAiqAlgoContext* ctx, Uapi_LinAeRouteAttr_t* pLinAeRouteAttr);
XCamReturn rk_aiq_uapi_ae_setHdrAeDayRouteAttr(RkAiqAlgoContext* ctx, const Uapi_HdrAeRouteAttr_t* pHdrAeRouteAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getHdrAeDayRouteAttr(RkAiqAlgoContext* ctx, Uapi_HdrAeRouteAttr_t* pHdrAeRouteAttr);
XCamReturn rk_aiq_uapi_ae_setHdrAeNightRouteAttr(RkAiqAlgoContext* ctx, const Uapi_HdrAeRouteAttr_t* pHdrAeRouteAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getHdrAeNightRouteAttr(RkAiqAlgoContext* ctx, Uapi_HdrAeRouteAttr_t* pHdrAeRouteAttr);
XCamReturn rk_aiq_uapi_ae_queryExpInfo(RkAiqAlgoContext* ctx, Uapi_ExpQueryInfo_t* pExpInfo);

XCamReturn rk_aiq_uapi_ae_setLinExpAttr(RkAiqAlgoContext* ctx, const Uapi_LinExpAttr_t* pLinExpAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getLinExpAttr(RkAiqAlgoContext* ctx, Uapi_LinExpAttr_t* pLinExpAttr);
XCamReturn rk_aiq_uapi_ae_setHdrExpAttr(RkAiqAlgoContext* ctx, const Uapi_HdrExpAttr_t* pHdrExpAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getHdrExpAttr(RkAiqAlgoContext* ctx, Uapi_HdrExpAttr_t* pHdrExpAttr);

XCamReturn rk_aiq_uapi_ae_setExpWinAttr(RkAiqAlgoContext* ctx, const Uapi_ExpWin_t* pExpWinAttr, bool need_sync);
XCamReturn rk_aiq_uapi_ae_getExpWinAttr(RkAiqAlgoContext* ctx, Uapi_ExpWin_t* pExpWinAttr);


#endif /*__RK_AIQ_UAPI_AE_INT_H__*/
