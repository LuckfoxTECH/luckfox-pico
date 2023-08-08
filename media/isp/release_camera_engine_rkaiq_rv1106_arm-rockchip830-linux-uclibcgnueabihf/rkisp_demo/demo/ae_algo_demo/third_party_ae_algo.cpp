#include "third_party_ae_algo.h"
#include "uAPI2/rk_aiq_user_api2_camgroup.h"
#include "uAPI2/rk_aiq_user_api2_sysctl.h"

int32_t custom_ae_init(void* ctx)
{
    //TO DO
    printf("custom ae test: %s \n", __func__);
    rk_aiq_sys_ctx_t* sys_ctx = NULL;
    rk_aiq_camgroup_ctx_t* group_ctx = NULL;

    rk_aiq_cam_type_t cam_type = (rk_aiq_cam_type_t)(*(int*)(ctx));

    if (cam_type == RK_AIQ_CAM_TYPE_GROUP)
        group_ctx = (rk_aiq_camgroup_ctx_t*)ctx;
    else
        sys_ctx = (rk_aiq_sys_ctx_t*)ctx;

    if (group_ctx)
        printf("%s:%d group custom ae !\n", __func__, __LINE__);
    else
        printf("%s:%d single custom ae !\n", __func__, __LINE__);

    return 0;
}

int32_t custom_ae_run(void* ctx, const rk_aiq_customAe_stats_t* pstAeInfo,
                      rk_aiq_customeAe_results_t* pstAeResult)
{
    rk_aiq_sys_ctx_t* sys_ctx = NULL;
    rk_aiq_camgroup_ctx_t* group_ctx = NULL;

    rk_aiq_cam_type_t cam_type = (rk_aiq_cam_type_t)(*(int*)(ctx));

    if (cam_type == RK_AIQ_CAM_TYPE_GROUP)
        group_ctx = (rk_aiq_camgroup_ctx_t*)ctx;
    else
        sys_ctx = (rk_aiq_sys_ctx_t*)ctx;

    if (group_ctx)
        printf("%s:%d group custom ae !\n", __func__, __LINE__);
    else
        printf("%s:%d single custom ae !\n", __func__, __LINE__);

#if 0

    if(pstAeInfo != NULL) {
        float MeanLuma[3] = {0.0f};

        for(int i = 0; i < 225; i++) {
            MeanLuma[0] += (float)(pstAeInfo->rawae_stat[0].rawae_big.channelg_xy[i] >> 4); //channelg is 12bit, channelr/channelb is 10bit
            MeanLuma[1] += (float)(pstAeInfo->rawae_stat[1].rawae_big.channelg_xy[i] >> 4); //channelg is 12bit, channelr/channelb is 10bit
        }

        MeanLuma[0] /= 225.0f; //range:0~255 (equal to 8bit)
        MeanLuma[1] /= 225.0f; //range:0~255 (equal to 8bit)


        printf("custom ae test: %s lin gain=%f,time=%f,meanluma = %f\n", __func__, pstAeInfo->linear_exp.exp_real_params.analog_gain,
               pstAeInfo->linear_exp.exp_real_params.integration_time, MeanLuma[0]);
        printf("custom ae test: %s hdr sgain=%f,stime=%f,mgain=%f,mtime=%f,smeanluma = %f,mmeanluma=%f\n", __func__,
               pstAeInfo->hdr_exp[0].exp_real_params.analog_gain, pstAeInfo->hdr_exp[0].exp_real_params.integration_time,
               pstAeInfo->hdr_exp[1].exp_real_params.analog_gain, pstAeInfo->hdr_exp[1].exp_real_params.integration_time,
               MeanLuma[0], MeanLuma[1]);
    }


    static uint32_t frmid = 0;

    if(frmid % 20 < 10) {
        pstAeResult->linear_exp.exp_real_params.analog_gain = 2.0f;
        pstAeResult->linear_exp.exp_real_params.integration_time = 0.02f;
        pstAeResult->linear_exp.exp_real_params.digital_gain = 1.0f;
        pstAeResult->linear_exp.exp_real_params.isp_dgain = 1.0f;
        pstAeResult->linear_exp.exp_real_params.dcg_mode = 0;

        pstAeResult->linear_exp.exp_sensor_params.coarse_integration_time = 0x7bc;
        pstAeResult->linear_exp.exp_sensor_params.analog_gain_code_global = 0x14;

        pstAeResult->hdr_exp[0].exp_real_params.analog_gain = 1.0f;
        pstAeResult->hdr_exp[0].exp_real_params.integration_time = 0.003f;
        pstAeResult->hdr_exp[0].exp_real_params.digital_gain = 1.0f;
        pstAeResult->hdr_exp[0].exp_real_params.isp_dgain = 1.0f;
        pstAeResult->hdr_exp[0].exp_real_params.dcg_mode = 0;

        pstAeResult->hdr_exp[0].exp_sensor_params.analog_gain_code_global = 0x0;
        pstAeResult->hdr_exp[0].exp_sensor_params.coarse_integration_time = 0x128;

        pstAeResult->hdr_exp[1].exp_real_params.analog_gain = 2.0f;
        pstAeResult->hdr_exp[1].exp_real_params.integration_time = 0.01f;
        pstAeResult->hdr_exp[1].exp_real_params.digital_gain = 1.0f;
        pstAeResult->hdr_exp[1].exp_real_params.isp_dgain = 1.0f;
        pstAeResult->hdr_exp[1].exp_real_params.dcg_mode = 0;

        pstAeResult->hdr_exp[1].exp_sensor_params.analog_gain_code_global = 0x14;
        pstAeResult->hdr_exp[1].exp_sensor_params.coarse_integration_time = 0x3de;

    } else {
        pstAeResult->linear_exp.exp_real_params.analog_gain = 40.0f;
        pstAeResult->linear_exp.exp_real_params.integration_time = 0.03f;
        pstAeResult->linear_exp.exp_real_params.digital_gain = 1.0f;
        pstAeResult->linear_exp.exp_real_params.isp_dgain = 1.0f;
        pstAeResult->linear_exp.exp_real_params.dcg_mode = 1;

        pstAeResult->linear_exp.exp_sensor_params.analog_gain_code_global = 0x4f;
        pstAeResult->linear_exp.exp_sensor_params.coarse_integration_time = 0xb9a;


        pstAeResult->hdr_exp[0].exp_real_params.analog_gain = 6.0f;
        pstAeResult->hdr_exp[0].exp_real_params.integration_time = 0.003f;
        pstAeResult->hdr_exp[0].exp_real_params.digital_gain = 1.0f;
        pstAeResult->hdr_exp[0].exp_real_params.isp_dgain = 1.0f;
        pstAeResult->hdr_exp[0].exp_real_params.dcg_mode = 1;

        pstAeResult->hdr_exp[0].exp_sensor_params.analog_gain_code_global = 0x18;
        pstAeResult->hdr_exp[0].exp_sensor_params.coarse_integration_time = 0x128;


        pstAeResult->hdr_exp[1].exp_real_params.analog_gain = 40.0f;
        pstAeResult->hdr_exp[1].exp_real_params.integration_time = 0.01f;
        pstAeResult->hdr_exp[1].exp_real_params.digital_gain = 1.0f;
        pstAeResult->hdr_exp[1].exp_real_params.isp_dgain = 1.0f;
        pstAeResult->hdr_exp[1].exp_real_params.dcg_mode = 1;

        pstAeResult->hdr_exp[1].exp_sensor_params.analog_gain_code_global = 0x4f;
        pstAeResult->hdr_exp[1].exp_sensor_params.coarse_integration_time = 0x3de;
    }

    frmid++;

#endif

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
