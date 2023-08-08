#ifndef _THIRD_PARTY_AWBV_ALGO_H_
#define _THIRD_PARTY_AWBV_ALGO_H_

#include "rk_aiq_types.h"
#include "rk_aiq_user_api2_custom_awb.h"
#include "rk_aiq_user_api2_custom_awb_type_v3x.h"


int32_t custom_awb_init(void* ctx);
int32_t custom_awb_run(void* ctx, const void* pstAwbInfo,
                      void* pstAwbResult);
int32_t custom_awb_ctrl(void* ctx, uint32_t u32Cmd, void *pValue);
int32_t custom_awb_exit(void* ctx);

#include "third_party_awb_algo.cpp"
#endif
