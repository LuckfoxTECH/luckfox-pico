#include "rk_aiq_types.h"
#include "rk_aiq_user_api_custom_ae.h"

int32_t ae_init(int32_t s32Handle, const rk_aiq_ae_param_t *pstAeParam);

int32_t ae_run(int32_t s32Handle, const rk_aiq_ae_statistics_t *pstAeInfo,
                     rk_aiq_ae_result_t *pstAeResult, int32_t s32Rsv);

int32_t ae_ctrl(int32_t s32Handle, uint32_t u32Cmd, void *pValue);

int32_t ae_exit(int32_t s32Handle);

int32_t custom_ae_init(void* ctx);
int32_t custom_ae_run(void* ctx, const rk_aiq_customAe_stats_t* pstAeInfo,
                      rk_aiq_customeAe_results_t* pstAeResult);
int32_t custom_ae_ctrl(void* ctx, uint32_t u32Cmd, void *pValue);
int32_t custom_ae_exit(void* ctx);
