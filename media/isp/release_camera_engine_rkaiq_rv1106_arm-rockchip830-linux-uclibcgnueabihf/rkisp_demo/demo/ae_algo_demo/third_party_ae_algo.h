#include "rk_aiq_types.h"
#include "rk_aiq_user_api2_custom_ae.h"

int32_t custom_ae_init(void* ctx);
int32_t custom_ae_run(void* ctx, const rk_aiq_customAe_stats_t* pstAeInfo,
                      rk_aiq_customeAe_results_t* pstAeResult);
int32_t custom_ae_ctrl(void* ctx, uint32_t u32Cmd, void *pValue);
int32_t custom_ae_exit(void* ctx);
