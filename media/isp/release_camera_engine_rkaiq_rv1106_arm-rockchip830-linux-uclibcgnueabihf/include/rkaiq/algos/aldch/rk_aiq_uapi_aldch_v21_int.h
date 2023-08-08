#ifndef _RK_AIQ_UAPI_ALDCH_V21_INT_H_
#define _RK_AIQ_UAPI_ALDCH_V21_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "aldch/rk_aiq_types_aldch_algo_int.h"

typedef rk_aiq_ldch_v21_cfg_t rk_aiq_ldch_v21_attrib_t;

/*
 * need_sync means the implementation should consider
 * the thread synchronization
 * if called by RkAiqAlscHandleInt, the sync has been done
 * in framework. And if called by user app directly,
 * sync should be done in inner. now we just need implement
 * the case of need_sync == false; need_sync is for future usage.
 */

XCamReturn rk_aiq_uapi_aldch_v21_SetAttrib(RkAiqAlgoContext *ctx,
                                      rk_aiq_ldch_v21_attrib_t attr,
                                      bool need_sync);
XCamReturn rk_aiq_uapi_aldch_v21_GetAttrib(const RkAiqAlgoContext *ctx,
                                      rk_aiq_ldch_v21_attrib_t *attr);

#endif
