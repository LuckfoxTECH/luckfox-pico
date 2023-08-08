#include "third_party_awbV32_algo.h"
#include "uAPI2/rk_aiq_user_api2_camgroup.h"
#include "uAPI2/rk_aiq_user_api2_sysctl.h"

int32_t custom_awb_init(void* ctx)
{
    //TO DO
    printf("custom awb test: %s \n", __func__);
    rk_aiq_sys_ctx_t* sys_ctx = NULL;
    rk_aiq_camgroup_ctx_t* group_ctx = NULL;

    rk_aiq_cam_type_t cam_type = (rk_aiq_cam_type_t)(*(int*)(ctx));

    if (cam_type == RK_AIQ_CAM_TYPE_GROUP)
        group_ctx = (rk_aiq_camgroup_ctx_t*)ctx;
    else
        sys_ctx = (rk_aiq_sys_ctx_t*)ctx;

    if (group_ctx)
        printf("%s:%d group custom awb !\n", __func__, __LINE__);
    else
        printf("%s:%d single custom awb !\n", __func__, __LINE__);
    return 0;
}



int32_t custom_awb_run(void* ctx, const void* vpstAwbInfo,  void* vpstAwbResult)
{
    const rk_aiq_customAwb_stats_t* pstAwbInfo = (rk_aiq_customAwb_stats_t*) vpstAwbInfo;
    rk_aiq_customeAwb_results_t* pstAwbResult = (rk_aiq_customeAwb_results_t*) vpstAwbResult;
    rk_aiq_sys_ctx_t* sys_ctx = NULL;
    rk_aiq_camgroup_ctx_t* group_ctx = NULL;

    rk_aiq_cam_type_t cam_type = (rk_aiq_cam_type_t)(*(int*)(ctx));

    if (cam_type == RK_AIQ_CAM_TYPE_GROUP)
        group_ctx = (rk_aiq_camgroup_ctx_t*)ctx;
    else
        sys_ctx = (rk_aiq_sys_ctx_t*)ctx;

    if (group_ctx)
        printf("%s:%d group custom awb !\n", __func__, __LINE__);
    else
        printf("%s:%d single custom awb !\n", __func__, __LINE__);

    printf("custom awb test: %s \n", __func__);
    //TO DO, the following are just sample codes

    if (pstAwbInfo == nullptr){
        //1)initialize  awb gain, must be initialized
        pstAwbResult->awb_gain_algo={1.9,1.0,1.0,1.6};
        //2) initialize  pstAwbResult->awbHwConfig , must be initialized
        //  pstAwbResult->awbHwConfig has been initialized by RK
       //   the demo is in function   initCustomAwbHwConfigGw(rk_aiq_customAwb_hw_cfg_t  *awbHwConfig)} in rk_aiq_user_api2_custom_awb_v32.cpp
        pstAwbResult->awbHwConfig.maxR          =    230*16;;
        pstAwbResult->awbHwConfig.maxG          =    230*16;;
        pstAwbResult->awbHwConfig.maxB          =    230*16;;
        pstAwbResult->awbHwConfig.maxY          =    230*16;;
        pstAwbResult->awbHwConfig.minR          =    3*16;
        pstAwbResult->awbHwConfig.minG          =    3*16;
        pstAwbResult->awbHwConfig.minB          =    3*16;
        pstAwbResult->awbHwConfig.minY          =    3*16;
       //to do more paras
     }else if( pstAwbInfo != nullptr){
        //0) run your algo  to calc para in pstAwbResult based on  pstAwbInfo, for example
        float R=0,G=0,B=0;
        for(int i=0;i<RK_AIQ_AWB_GRID_NUM_TOTAL;i++){
            R+=pstAwbInfo->blockResult[i].Rvalue;
            G+=pstAwbInfo->blockResult[i].Gvalue;
            B+=pstAwbInfo->blockResult[i].Bvalue;
        }
        if(R>0.001&&B>0.001){
            pstAwbResult->awb_gain_algo.bgain = G/B;
            pstAwbResult->awb_gain_algo.gbgain = 1.0;
            pstAwbResult->awb_gain_algo.grgain = 1.0;
            pstAwbResult->awb_gain_algo.rgain = G/R;
        }
        printf("wbggain :(%f,%f,%f,%f)\n",pstAwbResult->awb_gain_algo.rgain,
            pstAwbResult->awb_gain_algo.grgain,pstAwbResult->awb_gain_algo.gbgain,
            pstAwbResult->awb_gain_algo.bgain);
        //1)update awb gain, pstAwbResult->awb_gain_algo, must be updated
        //pstAwbResult->awb_gain_algo={2.0,1.0,1.0,1.6};
        //2)update awb converged state,pstAwbResult->IsConverged, must be updated
        pstAwbResult->IsConverged = true;
        //3)update pstAwbResult->awbHwConfig},  no updating is also ok
        //pstAwbResult->awbHwConfig.maxG= 230;
    }

    return 0;
}

int32_t custom_awb_ctrl(void* ctx, uint32_t u32Cmd, void *pValue)
{
    //TO DO
    printf("custom awb test: %s \n", __func__);
    return 0;
}

int32_t custom_awb_exit(void* ctx)
{
    //TO DO
    printf("custom awb test: %s \n", __func__);
    return 0;
}
