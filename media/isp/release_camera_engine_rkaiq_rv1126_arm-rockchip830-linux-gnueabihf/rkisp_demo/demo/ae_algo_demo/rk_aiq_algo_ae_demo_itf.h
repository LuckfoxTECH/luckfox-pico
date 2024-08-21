
#ifndef __RK_AIQ_TYPES_AE_ALGO_DEMO_H__
#define __RK_AIQ_TYPES_AE_ALGO_DEMO_H__

#include "rk_aiq_user_api_sysctl.h"
XCAM_BEGIN_DECLARE

XCamReturn
rk_aiq_AELibRegCallBack(const rk_aiq_sys_ctx_t* ctx, rk_aiq_ae_register_t *pstRegister, int ChannelID);

XCamReturn
rk_aiq_AELibunRegCallBack(const rk_aiq_sys_ctx_t* ctx, int ChannelID);

XCAM_END_DECLARE

#endif /*__RK_AIQ_TYPES_AE_ALGO_DEMO_H__*/
