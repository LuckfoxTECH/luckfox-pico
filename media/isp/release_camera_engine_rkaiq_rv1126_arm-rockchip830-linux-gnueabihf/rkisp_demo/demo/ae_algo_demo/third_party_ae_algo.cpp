#include "third_party_ae_algo.h"


int32_t ae_init(int32_t s32Handle, const rk_aiq_ae_param_t *pstAeParam)
{
    //TO DO
    return 0;
}

int32_t ae_run(int32_t s32Handle, const rk_aiq_ae_statistics_t *pstAeInfo,
               rk_aiq_ae_result_t *pstAeResult, int32_t s32Rsv)
{
    //TO DO
    return 0;
}

int32_t ae_ctrl(int32_t s32Handle, uint32_t u32Cmd, void *pValue)
{
    //TO DO
    return 0;
}

int32_t ae_exit(int32_t s32Handle)
{
    //TO DO
    return 0;
}

int32_t custom_ae_init(void* ctx)
{
    //TO DO
    printf("custom ae test: %s \n", __func__);
    return 0;
}

int32_t custom_ae_run(void* ctx, const rk_aiq_customAe_stats_t* pstAeInfo,
                      rk_aiq_customeAe_results_t* pstAeResult)
{
    //TO DO

    float MeanLuma = 0.0f;

    for(int i = 0; i < 225; i++)
        MeanLuma += (float)(pstAeInfo->rawae_stat.rawae_big.channelg_xy[i] >> 4); //channelg is 12bit, channelr/channelb is 10bit

    MeanLuma /= 225.0f; //range:0~255 (equal to 8bit)

    printf("custom ae test: %s gain=%f,time=%f,meanluma = %f\n", __func__, pstAeInfo->exp_real_params.analog_gain,
           pstAeInfo->exp_real_params.integration_time, MeanLuma);


    /*static uint16_t frmid = 0;

    if(frmid % 20 < 10){
        pstAeResult->exp_real_params.analog_gain = 2.0f;
        pstAeResult->exp_real_params.integration_time= 0.02f;
    }else{
        pstAeResult->exp_real_params.analog_gain = 6.0f;
        pstAeResult->exp_real_params.integration_time= 0.03f;
    }

    frmid++;*/


    return 0;
}

int32_t custom_ae_ctrl(void* ctx, uint32_t u32Cmd, void *pValue)
{
    //TO DO
    printf("custom ae test: %s \n", __func__);
    return 0;
}

int32_t custom_ae_exit(void* ctx)
{
    //TO DO
    printf("custom ae test: %s \n", __func__);
    return 0;
}
