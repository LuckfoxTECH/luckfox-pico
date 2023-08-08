/*
 *  Copyright (c) 2019 Rockchip Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "sample_comm.h"

#define safe_free(x) if(NULL!=(x))\
                           free(x); x=NULL;

void sample_print_awb_info(const void *arg)
{
    printf ("enter AWB modult test!\n");
}

void sample_print_awb32_info(const void *arg)
{
    printf ("enter AWBV32 modult test!\n");
}
/*
******************************
*
* ImgProc level API Sample Func
*
******************************
*/

static int sample_set_wbmode_manual(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_setWBMode(ctx, OP_MANUAL);
    return 0;
}

static int sample_set_wbmode_auto(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_setWBMode(ctx, OP_AUTO);
    return 0;
}

static int sample_get_wbmode(const rk_aiq_sys_ctx_t* ctx)
{
    opMode_t mode;
    rk_aiq_uapi2_getWBMode(ctx, &mode);
    printf("get WBMode=%d\n\n", mode);
    return 0;
}

static int sample_lock_awb(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_lockAWB(ctx);
    return 0;
}

static int sample_unlock_awb(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_unlockAWB(ctx);
    return 0;
}

static int sample_set_mwb_scene(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_setMWBScene(ctx, RK_AIQ_WBCT_TWILIGHT);
    return 0;
}

static int sample_get_mwb_scene(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_wb_scene_t scene;
    rk_aiq_uapi2_getMWBScene(ctx, &scene);
    printf("get MWBScene=%d\n\n", scene);
    return 0;
}

static int sample_set_mwb_gain(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_wb_gain_t gain;
    gain.rgain = 0.5f;
    gain.grgain = 0.5f;
    gain.gbgain = 0.5f;
    gain.bgain = 0.5f;
    rk_aiq_uapi2_setMWBGain(ctx, &gain);
    return 0;
}

static int sample_get_mwb_gain(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_wb_gain_t gain;
    rk_aiq_uapi2_getWBGain(ctx, &gain);
    printf("get WBGain=[%f %f %f %f]\n", gain.rgain, gain.grgain, gain.gbgain, gain.bgain);
    return 0;
}

static int sample_set_mwb_ct(const rk_aiq_sys_ctx_t* ctx)
{
    unsigned int cct;
    cct = 6000;
    rk_aiq_uapi2_setMWBCT(ctx, cct);
    return 0;
}

static int sample_get_mwb_ct(const rk_aiq_sys_ctx_t* ctx)
{
    unsigned int cct;
    rk_aiq_uapi2_getWBCT(ctx, &cct);
    printf("get cct=%d\n\n", cct);
    return 0;
}

static int sample_set_awb_gainoffset(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wb_awb_wbGainOffset_t offset2 = {{RK_AIQ_UAPI_MODE_DEFAULT,false}, {true,{0.5,0,0,0.5}}};
    rk_aiq_uapi2_setAwbGainOffsetAttrib(ctx,offset2 );
    return 0;
}

static int sample_get_awb_gainoffset(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wb_awb_wbGainOffset_t offset ;
    rk_aiq_uapi2_getAwbGainOffsetAttrib(ctx, &offset);
    printf("get WbGainOffset{%d,[%f,%f,%f,%f]}\n\n",
        offset.gainOffset.enable,
        offset.gainOffset.offset[0],
        offset.gainOffset.offset[1],
        offset.gainOffset.offset[2],
        offset.gainOffset.offset[3]);
    return 0;
}

static int sample_set_awb_gain_adjust(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wb_awb_wbGainAdjust_t adjust ;
    memset(&adjust,0,sizeof(rk_aiq_uapiV2_wb_awb_wbGainAdjust_t));
    //get
    rk_aiq_uapi2_getAwbGainAdjustAttrib(ctx, &adjust);
    //modfiy
    adjust.sync.sync_mode = RK_AIQ_UAPI_MODE_DEFAULT;
    adjust.enable = true;
    //set
    rk_aiq_uapi2_setAwbGainAdjustAttrib(ctx, adjust);
    printf("set AwbGainAdjust\n\n");
    //free
    for(int i = 0; i < adjust.lutAll_len; i++) {
        safe_free(adjust.lutAll[i].cri_lut_out);
        safe_free(adjust.lutAll[i].ct_lut_out);
    }
    safe_free(adjust.lutAll);
    return 0;
}

static int sample_get_awb_gain_adjust(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wb_awb_wbGainAdjust_t adjust ;
    memset(&adjust,0,sizeof(rk_aiq_uapiV2_wb_awb_wbGainAdjust_t));
    //get
    rk_aiq_uapi2_getAwbGainAdjustAttrib(ctx,&adjust );
    printf("get AwbGainAdjust {%d}\n\n",adjust.enable);
    //free
    for(int i = 0; i < adjust.lutAll_len; i++) {
        safe_free(adjust.lutAll[i].cri_lut_out);
        safe_free(adjust.lutAll[i].ct_lut_out);
    }
    safe_free(adjust.lutAll);
    return 0;
}

static int sample_set_awbv30_atrr(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wbV30_attrib_t attr ;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wbV30_attrib_t));
    //get
    rk_aiq_uapi2_getAwbV30AllAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = RK_AIQ_UAPI_MODE_DEFAULT;
    attr.stAuto.wbGainAdjust.enable = true;
    //set
    rk_aiq_uapi2_setAwbV30AllAttrib(ctx, attr);
    printf("set rk_aiq_uapiV2_wbV30_attrib\n\n");
    //free
    for(int i = 0; i < attr.stAuto.wbGainAdjust.lutAll_len; i++) {
        safe_free(attr.stAuto.wbGainAdjust.lutAll[i].cri_lut_out);
        safe_free(attr.stAuto.wbGainAdjust.lutAll[i].ct_lut_out);
    }
    safe_free(attr.stAuto.wbGainAdjust.lutAll);
    return 0;
}

/*
******************************
*
* Module level API Sample Func
*
******************************
*/

static int sample_awb_awbv21_setAllAttr(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_wbV21_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wbV21_attrib_t));
    //get
    rk_aiq_user_api2_awbV21_GetAllAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.stAuto.wbGainAdjust.enable = !attr.stAuto.wbGainAdjust.enable;
    //set
    rk_aiq_user_api2_awbV21_SetAllAttrib(ctx, attr);
    printf("set Awbv21 AllAttr\n\n");
    return 0;
}

static int sample_awb_awbv21_getAllAttr(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wbV21_attrib_t attr ;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wbV21_attrib_t));
    //get
    rk_aiq_user_api2_awbV21_GetAllAttrib(ctx, &attr);
    printf("get Awbv21 AllAttr:\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t bypass = %d\n", attr.byPass);
    printf("\t mode = %s\n", (attr.mode > 0 ? "auto" : "manual"));
    if (attr.mode > 0) {
        printf("\t wbGainAdjustEn = %s\n",
                            (attr.stAuto.wbGainAdjust.enable > 0 ? "true" : "false"));
        printf("\t wbGainOffset = {%s,[%f,%f,%f,%f]}\n\n",
                            (attr.stAuto.wbGainOffset.enable > 0 ? "true" : "false"),
                            attr.stAuto.wbGainOffset.offset[0],
                            attr.stAuto.wbGainOffset.offset[1],
                            attr.stAuto.wbGainOffset.offset[2],
                            attr.stAuto.wbGainOffset.offset[3]);
    } else {
        switch (attr.stManual.mode)
        {
            case RK_AIQ_MWB_MODE_CCT:
                printf("\t manual mode = RK_AIQ_MWB_MODE_CCT\n");
                printf("\t manual cct = %f, ccri = %f\n",
                                attr.stManual.para.cct.CCT,
                                attr.stManual.para.cct.CCRI);
                break;
            case RK_AIQ_MWB_MODE_WBGAIN:
                printf("\t manual mode = RK_AIQ_MWB_MODE_WBGAIN\n");
                printf("\t manual wbgain = [%f, %f, %f, %f]\n",
                                attr.stManual.para.gain.rgain,
                                attr.stManual.para.gain.grgain,
                                attr.stManual.para.gain.gbgain,
                                attr.stManual.para.gain.bgain);
                break;
            case RK_AIQ_MWB_MODE_SCENE:
                printf("\t manual mode = RK_AIQ_MWB_MODE_SCENE\n");
                printf("\t manual scene = %d\n",
                                attr.stManual.para.scene);
                break;
            default:
                printf("\t manual mode is invalid!\n");
                break;
        }
    }
    return 0;
}

static int sample_awb_awbv30_setAllAttr(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_wbV30_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wbV30_attrib_t));
    //get
    rk_aiq_user_api2_awbV30_GetAllAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.stAuto.wbGainAdjust.enable = !attr.stAuto.wbGainAdjust.enable;
    //set
    rk_aiq_user_api2_awbV30_SetAllAttrib(ctx, attr);
    printf("set Awbv30 AllAttr\n\n");
    return 0;
}

static int sample_awb_awbv30_getAllAttr(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wbV30_attrib_t attr ;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wbV30_attrib_t));
    //get
    rk_aiq_user_api2_awbV30_GetAllAttrib(ctx, &attr);
    printf("get Awbv30 AllAttr:\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t bypass = %d\n", attr.byPass);
    printf("\t mode = %s\n", (attr.mode > 0 ? "auto" : "manual"));
    if (attr.mode > 0) {
        printf("\t wbGainAdjustEn = %s\n",
                            (attr.stAuto.wbGainAdjust.enable > 0 ? "true" : "false"));
        printf("\t wbGainOffset = {%s,[%f,%f,%f,%f]}\n\n",
                            (attr.stAuto.wbGainOffset.enable > 0 ? "true" : "false"),
                            attr.stAuto.wbGainOffset.offset[0],
                            attr.stAuto.wbGainOffset.offset[1],
                            attr.stAuto.wbGainOffset.offset[2],
                            attr.stAuto.wbGainOffset.offset[3]);
    } else {
        switch (attr.stManual.mode)
        {
            case RK_AIQ_MWB_MODE_CCT:
                printf("\t manual mode = RK_AIQ_MWB_MODE_CCT\n");
                printf("\t manual cct = %f, ccri = %f\n",
                                attr.stManual.para.cct.CCT,
                                attr.stManual.para.cct.CCRI);
                break;
            case RK_AIQ_MWB_MODE_WBGAIN:
                printf("\t manual mode = RK_AIQ_MWB_MODE_WBGAIN\n");
                printf("\t manual wbgain = [%f, %f, %f, %f]\n",
                                attr.stManual.para.gain.rgain,
                                attr.stManual.para.gain.grgain,
                                attr.stManual.para.gain.gbgain,
                                attr.stManual.para.gain.bgain);
                break;
            case RK_AIQ_MWB_MODE_SCENE:
                printf("\t manual mode = RK_AIQ_MWB_MODE_SCENE\n");
                printf("\t manual scene = %d\n",
                                attr.stManual.para.scene);
                break;
            default:
                printf("\t manual mode is invalid!\n");
                break;
        }
    }
    return 0;
}

static int sample_awb_getCct(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_wb_cct_t cct;
    memset(&cct,0,sizeof(rk_aiq_wb_cct_t));
    //get
    rk_aiq_user_api2_awb_GetCCT(ctx, &cct);
    printf("get rk_aiq_user_api2_awb_CCT:\n");
    printf("\t CCRI = %f, CCT = %f\n\n", cct.CCRI, cct.CCT);

    return 0;
}

static int sample_query_wb_info(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_wb_querry_info_t wb_querry_info;
    rk_aiq_user_api2_awb_QueryWBInfo(ctx, &wb_querry_info);
    printf("Query AWB Info\n\n");
    printf("\t AWBGain = [%f %f %f %f]\n", wb_querry_info.gain.rgain,
                                            wb_querry_info.gain.grgain,
                                            wb_querry_info.gain.gbgain,
                                            wb_querry_info.gain.bgain);
    printf("\t cctGloabl: CCT = %f, CCRI = %f \n", wb_querry_info.cctGloabl.CCT, wb_querry_info.cctGloabl.CCRI);
    printf("\t awbConverged: %s \n", (wb_querry_info.awbConverged ? "true" : "false"));
    printf("\t LVValue: %d \n", wb_querry_info.LVValue);
    return 0;
}

static int sample_awb_lock(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_user_api2_awb_Lock(ctx);
    printf("AWB Lock\n");

    return 0;
}

static int sample_awb_unlock(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_user_api2_awb_Unlock(ctx);
    printf("AWB Unlock\n");

    return 0;
}

static int sample_awb_setModeManual(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_wb_opMode_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wb_opMode_t));
    //get
    rk_aiq_user_api2_awb_GetWpModeAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_WB_MODE_MANUAL;
    //set
    rk_aiq_user_api2_awb_SetWpModeAttrib(ctx, attr);
    printf("set Mode Manual\n\n");

    return 0;
}

static int sample_awb_setModeAuto(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_wb_opMode_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wb_opMode_t));
    //get
    rk_aiq_user_api2_awb_GetWpModeAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_WB_MODE_AUTO;
    //set
    rk_aiq_user_api2_awb_SetWpModeAttrib(ctx, attr);
    printf("set Mode Auto\n\n");

    return 0;
}

static int sample_awb_getMode(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wb_opMode_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wb_opMode_t));
    //get
    rk_aiq_user_api2_awb_GetWpModeAttrib(ctx, &attr);
    printf("get AWB Mode: %s\n\n", (attr.mode > 0 ? "Auto" : "Manual"));
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);

    return 0;
}

static int sample_awb_setMwb(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_wb_mwb_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_wb_mwb_attrib_t));
    //set mode MANUAL
    sample_awb_setModeManual(ctx, sync);
    //get
    rk_aiq_user_api2_awb_GetMwbAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_MWB_MODE_WBGAIN;
    if (attr.para.gain.rgain == 1.0) {
        attr.para.gain.rgain = 0.5f;
        attr.para.gain.grgain = 0.5f;
        attr.para.gain.gbgain = 0.5f;
        attr.para.gain.bgain = 0.5f;
    } else {
        attr.para.gain.rgain = 1.0f;
        attr.para.gain.grgain = 0.5f;
        attr.para.gain.gbgain = 0.5f;
        attr.para.gain.bgain = 1.0f;
    }

    //set
    rk_aiq_user_api2_awb_SetMwbAttrib(ctx, attr);
    printf("set Manual attr\n\n");

    return 0;
}

static int sample_awb_setMwb1(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_wb_mwb_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_wb_mwb_attrib_t));
    //set mode MANUAL
    sample_awb_setModeManual(ctx, sync);
    //get
    rk_aiq_user_api2_awb_GetMwbAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_MWB_MODE_SCENE;

    //set
    rk_aiq_user_api2_awb_SetMwbAttrib(ctx, attr);
    printf("set Manual attr\n\n");

    return 0;
}

static int sample_awb_setMwb2(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_wb_mwb_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_wb_mwb_attrib_t));
    //get
    rk_aiq_user_api2_awb_GetMwbAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_MWB_MODE_WBGAIN;
    if (attr.para.gain.rgain == 1.0) {
        attr.para.gain.rgain = 0.5f;
        attr.para.gain.grgain = 0.5f;
        attr.para.gain.gbgain = 0.5f;
        attr.para.gain.bgain = 0.5f;
    } else {
        attr.para.gain.rgain = 1.0f;
        attr.para.gain.grgain = 0.5f;
        attr.para.gain.gbgain = 0.5f;
        attr.para.gain.bgain = 1.0f;
    }

    //set
    rk_aiq_user_api2_awb_SetMwbAttrib(ctx, attr);
    printf("set Manual attr\n\n");

    return 0;
}

static int sample_awb_getMwbAttr(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_wb_mwb_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_wb_mwb_attrib_t));
    //get
    rk_aiq_user_api2_awb_GetMwbAttrib(ctx, &attr);
    printf("get Manual attr:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    switch (attr.mode)
    {
        case RK_AIQ_MWB_MODE_CCT:
            printf("\t manual mode = RK_AIQ_MWB_MODE_CCT\n");
            printf("\t manual cct = %f, ccri = %f\n",
                            attr.para.cct.CCT,
                            attr.para.cct.CCRI);
            break;
        case RK_AIQ_MWB_MODE_WBGAIN:
            printf("\t manual mode = RK_AIQ_MWB_MODE_WBGAIN\n");
            printf("\t manual wbgain = [%f, %f, %f, %f]\n",
                            attr.para.gain.rgain,
                            attr.para.gain.grgain,
                            attr.para.gain.gbgain,
                            attr.para.gain.bgain);
            break;
        case RK_AIQ_MWB_MODE_SCENE:
            printf("\t manual mode = RK_AIQ_MWB_MODE_SCENE\n");
            printf("\t manual scene = %d\n", attr.para.scene);
            break;
        default:
            printf("\t manual mode is invalid!\n");
            break;
    }

    return 0;
}

static int sample_awb_setWbGainAdjust(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_wb_awb_wbGainAdjust_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wb_awb_wbGainAdjust_t));
    //set mode AUTO
    sample_awb_setModeAuto(ctx, sync);
    //get
    rk_aiq_user_api2_awb_GetWbGainAdjustAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.enable = !attr.enable;
    //set
    rk_aiq_user_api2_awb_SetWbGainAdjustAttrib(ctx, attr);
    printf("set AWbGainAdjust\n\n");
    //free
    for(int i = 0; i < attr.lutAll_len; i++) {
        //printf("free attr->lutAll[%d].cri_lut_out= %p\n",i,attr.lutAll[i].cri_lut_out);
        safe_free(attr.lutAll[i].cri_lut_out);
        safe_free(attr.lutAll[i].ct_lut_out);
    }
    //printf(" free attr->lutAll= %p\n",attr.lutAll);
    safe_free(attr.lutAll);
    return 0;
}

static int sample_awb_getWbGainAdjust(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wb_awb_wbGainAdjust_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wb_awb_wbGainAdjust_t));
    //get
    rk_aiq_user_api2_awb_GetWbGainAdjustAttrib(ctx, &attr);
    printf("get AWbGainAdjust:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t enable = %s\n", (attr.enable ? "true" : "false"));
    //free
    for(int i = 0; i < attr.lutAll_len; i++) {
        //printf("free attr->lutAll[%d].cri_lut_out= %p\n",i,attr.lutAll[i].cri_lut_out);
        safe_free(attr.lutAll[i].cri_lut_out);
        safe_free(attr.lutAll[i].ct_lut_out);
    }
    //printf(" free attr->lutAll= %p\n",attr.lutAll);
    safe_free(attr.lutAll);
    return 0;
}

static int sample_awb_setWbGainOffset(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_wb_awb_wbGainOffset_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wb_awb_wbGainOffset_t));
    //set mode AUTO
    sample_awb_setModeAuto(ctx, sync);
    //get
    rk_aiq_user_api2_awb_GetWbGainOffsetAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.gainOffset.enable = !attr.gainOffset.enable;
    attr.gainOffset.offset[0] = 1.0f;
    attr.gainOffset.offset[1] = 0;
    attr.gainOffset.offset[2] = 0;
    attr.gainOffset.offset[3] = 0;
    //set
    rk_aiq_user_api2_awb_SetWbGainOffsetAttrib(ctx, attr);
    printf("set WbGainOffset\n\n");

    return 0;
}

static int sample_awb_getWbGainOffset(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wb_awb_wbGainOffset_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wb_awb_wbGainOffset_t));
    //get
    rk_aiq_user_api2_awb_GetWbGainOffsetAttrib(ctx, &attr);
    printf("get WbGainOffset:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("WbGainOffset = {%d,[%f,%f,%f,%f]}\n\n",
        attr.gainOffset.enable,
        attr.gainOffset.offset[0],
        attr.gainOffset.offset[1],
        attr.gainOffset.offset[2],
        attr.gainOffset.offset[3]);
    return 0;
}

static int sample_awb_setWbAwbMultiWindow(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_wb_awb_mulWindow_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wb_awb_mulWindow_t));
    //get
    rk_aiq_user_api2_awb_GetMultiWindowAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.multiWindw.enable = !attr.multiWindw.enable;
    attr.multiWindw.multiwindowMode = CALIB_AWB_WIN_USELESS;
    //set
    rk_aiq_user_api2_awb_SetMultiWindowAttrib(ctx, attr);
    printf("set Awb MultiWindow\n\n");

    return 0;
}

static int sample_awb_getWbAwbMultiWindow(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wb_awb_mulWindow_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wb_awb_mulWindow_t));
    //get
    rk_aiq_user_api2_awb_GetMultiWindowAttrib(ctx, &attr);
    printf("get Awb MultiWindow:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t enable = %s\n\n", (attr.multiWindw.enable ? "true" : "false"));
    return 0;
}

#if 0
static int sample_awb_printflog(const rk_aiq_sys_ctx_t* ctx)
{
    rk_tool_awb_stat_res_full_t awb_measure_result;
    rk_tool_awb_strategy_result_t strategy_result;

    //for(int i=0;i<100;i++)
    {
        memset(&awb_measure_result,0,sizeof(awb_measure_result));
        rk_aiq_user_api2_awb_getAlgoSta(ctx, &awb_measure_result);
        memset(&strategy_result,0,sizeof(strategy_result));
        rk_aiq_user_api2_awb_getStrategyResult(ctx, &strategy_result);
        printf("------------%d---------------\n",strategy_result.count);
        printf("Global CCT:%f,CCRI:%f,valid:%d\n",strategy_result.cctGloabl.CCT,strategy_result.cctGloabl.CCRI,
            strategy_result.cctGloabl.valid);
        printf("wbgain_s6(after damping)(rggb):(%f,%f,%f,%f), awbConverged(%d)  ,LVValue(%d), WPType(%d),df(%1.2f)\n",strategy_result.stat3aAwbGainOut[AWB_CHANNEL_R],
                strategy_result.stat3aAwbGainOut[AWB_CHANNEL_GR], strategy_result.stat3aAwbGainOut[AWB_CHANNEL_GB],
                strategy_result.stat3aAwbGainOut[AWB_CHANNEL_B], strategy_result.awbConverged, strategy_result.LVValue,strategy_result.WPType,
                strategy_result.wbGainDampFactor);
        printf("WPNo(normal,big):(%d,%d),vaild wp number in standard light(%d), vaild wp number in extra light(%d),xy_area_type %d \n",
            awb_measure_result.WpNo[0], awb_measure_result.WpNo[1],
            strategy_result.WPTotalNUM, awb_measure_result.extraLightResult.WpNo,strategy_result.xy_area_type);
        printf("select white point range type (0-normal xy range,1-big xy range, 3-extra light) : %d, runInterval(%d),tolerance(%f) \n",strategy_result.xy_area_type,
            strategy_result.runInterval, strategy_result.tolerance);
        //printf("the effective light source slecetion to 3dyuv is :[%d,%d,%d,%d]\n", para->awb_cfg_effect_v201->threeDyuvIllu[0], para->awb_cfg_effect_v201->threeDyuvIllu[1],
        //         para->awb_cfg_effect_v201->threeDyuvIllu[2], para->awb_cfg_effect_v201->threeDyuvIllu[3]);
        printf("LVLevel(%d),LVType(%d)\n", strategy_result.LVLevel, strategy_result.LVType);
        printf("wbGainSgc for WPType1(rggb):(%f,%f,%f,%f) ,wbWeightSgc(%f),sgcGainEqu2Tem(%d)\n", strategy_result.wbGainSgc[0], strategy_result.wbGainSgc[1],
                 strategy_result.wbGainSgc[2], strategy_result.wbGainSgc[3], strategy_result.wbWeightSgc, strategy_result.sgcGainEqu2Tem);
        printf("wbGainSpa  for WPType1 (rggb):(%f,%f,%f,%f) ,wbWeightSpa(%f), spaGainEqu2Tem(%d) \n", strategy_result.wbGainSpa[AWB_CHANNEL_R], strategy_result.wbGainSpa[AWB_CHANNEL_GR],
                 strategy_result.wbGainSpa[AWB_CHANNEL_GB], strategy_result.wbGainSpa[AWB_CHANNEL_B], strategy_result.wbWeightSpa, strategy_result.spaGainEqu2Tem);
        printf("wbGainTep for WPType1 (rggb):(%f,%f,%f,%f)\n", strategy_result.wbGainTep[AWB_CHANNEL_R], strategy_result.wbGainTep[AWB_CHANNEL_GR],
                 strategy_result.wbGainTep[AWB_CHANNEL_GB], strategy_result.wbGainTep[AWB_CHANNEL_B]);
        printf("wbGainType1 (rggb):(%f,%f,%f,%f)\n", strategy_result.wbGainType1[AWB_CHANNEL_R], strategy_result.wbGainType1[AWB_CHANNEL_GR],
                 strategy_result.wbGainType1[AWB_CHANNEL_GB], strategy_result.wbGainType1[AWB_CHANNEL_B]);
        printf("wbGainType3(rggb):(%f,%f,%f,%f)\n", strategy_result.wbGainType3[AWB_CHANNEL_R], strategy_result.wbGainType3[AWB_CHANNEL_GR],
                 strategy_result.wbGainType3[AWB_CHANNEL_GB], strategy_result.wbGainType3[AWB_CHANNEL_B]);
        printf("wbgain_s1 (mix wbGainType1 and wbGainType3 ) :(%f,%f,%f,%f) is updated (%d), weight of wbGainType3 %f\n", strategy_result.wbGainIntpStrategy[AWB_CHANNEL_R],
            strategy_result.wbGainIntpStrategy[AWB_CHANNEL_GR], strategy_result.wbGainIntpStrategy[AWB_CHANNEL_GB],
            strategy_result.wbGainIntpStrategy[AWB_CHANNEL_B], strategy_result.updateFlag, strategy_result.wbWeightType3);
        printf("wbgain_s2 (caga) :(%f,%f,%f,%f) \n", strategy_result.wbGainCaga[AWB_CHANNEL_R],
            strategy_result.wbGainCaga[AWB_CHANNEL_GR], strategy_result.wbGainCaga[AWB_CHANNEL_GB],
            strategy_result.wbGainCaga[AWB_CHANNEL_B]);
        printf("wbgain_s3 (wbgainclip) :(%f,%f,%f,%f) \n", strategy_result.wbGainClip[AWB_CHANNEL_R],
            strategy_result.wbGainClip[AWB_CHANNEL_GR], strategy_result.wbGainClip[AWB_CHANNEL_GB],
            strategy_result.wbGainClip[AWB_CHANNEL_B]);
        printf("wbgain_s4 (wbgainAdjust) :(%f,%f,%f,%f) \n", strategy_result.wbGainAdjust[AWB_CHANNEL_R],
            strategy_result.wbGainAdjust[AWB_CHANNEL_GR], strategy_result.wbGainAdjust[AWB_CHANNEL_GB],
            strategy_result.wbGainAdjust[AWB_CHANNEL_B]);
        printf("wbgain_s5 (wbgainOffest) :(%f,%f,%f,%f) \n", strategy_result.wbGainOffset[AWB_CHANNEL_R],
            strategy_result.wbGainOffset[AWB_CHANNEL_GR], strategy_result.wbGainOffset[AWB_CHANNEL_GB],
            strategy_result.wbGainOffset[AWB_CHANNEL_B]);
        printf("hdrFrameChoose %d\n",awb_measure_result.effectHwPara.hdrFrameChoose);
        printf("select algorithm method based on stat_mode 0 (0-wp,1-gw) %d\n",strategy_result.algMethod);
        char str1[500];
        sprintf(str1, "%s", "WpNoHist:       ");
        for (int p = 0; p < RK_AIQ_AWBWP_WEIGHT_CURVE_DOT_NUM - 1; p++) {
            char str2[100];
            sprintf(str2, "%6d,",  awb_measure_result.WpNoHist[p]);
            strcat(str1,str2);
        }
        printf("%s\n",str1);
        if (fabs(strategy_result.wbWeightSgc - 1) < 0.001 && strategy_result.wbWeightType3 < 0.02)
        {
            //printf("current light source : %d  (%d,%d,%d)\n", para->sinColorResult.illEst,
            //         para->sinColorResult.voteResult[0], para->sinColorResult.voteResult[1], para->sinColorResult.voteResult[2]);
           // printf("current color : %d\n", para->sinColorResult.colorEst);
            for (int i = 0; i < strategy_result.lightNum; i++)
            {
                printf(" %s:\n", strategy_result.illInf[i].illName);
                //printf(" %s:\n", strategy_result.illConf[i].illName);
                //type0
                for (int m = 0; m < 2; m++) {
                    printf("     type%d: gain (rg,bg):(%f,%f) WPNo(%d)\n", m, awb_measure_result.light[i].xYType[m].gain[0],
                             awb_measure_result.light[i].xYType[m].gain[3], awb_measure_result.light[i].xYType[m].WpNo);
                }
            }

            printf("blockresult[15][15]:");
            for (int i = 0; i < RK_AIQ_AWB_GRID_NUM_VERHOR * RK_AIQ_AWB_GRID_NUM_VERHOR; i++)
            {
                if (i % 15 == 0)
                {
                    printf("     ");
                }
                printf("%d (%.7f,%.7f,%.7f), \n", i, awb_measure_result.blkSgcResult[i].R, awb_measure_result.blkSgcResult[i].G,
                         awb_measure_result.blkSgcResult[i].B);
            }
            printf("\n");
        }
        else
        {
            if (strategy_result.wbWeightSgc > 0.001) {
                //printf("current light source : %s  (%d,%d,%d)\n", strategy_result.illConf[para->sinColorResult.illEst].illName,
                //         para->sinColorResult.voteResult[0], para->sinColorResult.voteResult[1], para->sinColorResult.voteResult[2]);
                //printf("current color : %d\n", para->sinColorResult.colorEst);
            }

            for (int i = 0; i < strategy_result.lightNum; i++)
            {
                //printf("%s:\n", strategy_result.illConf[i].illName);
                printf(" %s:\n", strategy_result.illInf[i].illName);
                printf("     strategy_result.gain (rggb):(%f,%f,%f,%f) \n",
                         strategy_result.illInf[i].gainValue[0], strategy_result.illInf[i].gainValue[1],
                         strategy_result.illInf[i].gainValue[2], strategy_result.illInf[i].gainValue[3]
                        );
                printf("     prob_total(%f),prob_dis(%f),prob_LV(%f),prob_WPNO(%f)\n", strategy_result.illInf[i].prob_total, strategy_result.illInf[i].prob_dis,
                         strategy_result.illInf[i].prob_LV, strategy_result.illInf[i].prob_WPNO);
                printf("     spatial gain(rggb):(%f,%f,%f,%f),statistics gain weight(%f)\n", strategy_result.illInf[i].spatialGainValue[0], strategy_result.illInf[i].spatialGainValue[1],
                         strategy_result.illInf[i].spatialGainValue[2], strategy_result.illInf[i].spatialGainValue[3], strategy_result.illInf[i].staWeight);

                for (int m = 0; m < 2; m++) {

                    if (m == 2) {
                        printf("     type%d: gain (rg,bg):(%f,%f) WPNo(%d)  Weight(%f)\n", m,
                                 awb_measure_result.light[i].xYType[m].gain[0],
                                 awb_measure_result.light[i].xYType[m].gain[3],
                                 awb_measure_result.light[i].xYType[m].WpNo,
                                 strategy_result.illInf[i].xyType2Weight);

                    }
                    else {
                        printf("     type%d: gain (rg,bg):(%f,%f) WPNo(%d)\n", m,
                                 awb_measure_result.light[i].xYType[m].gain[0],
                                 awb_measure_result.light[i].xYType[m].gain[3],
                                 awb_measure_result.light[i].xYType[m].WpNo);

                    }
                }

            }

        }

        printf("\n");
    }



    return 0;
}
#endif

static int sample_awb_getStrategyResult(const rk_aiq_sys_ctx_t* ctx)
{
    rk_tool_awb_strategy_result_t attr;
    memset(&attr,0,sizeof(attr));
    rk_aiq_user_api2_awb_getStrategyResult(ctx, &attr);
    return 0;
}

static int sample_awb_setWbV32AwbMultiWindow(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_wbV32_awb_mulWindow_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wbV32_awb_mulWindow_t));
    //get
    rk_aiq_user_api2_awbV32_GetMultiWindowAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.enable = !attr.enable;
    attr.window[0][0]= 0;
    attr.window[0][1]= 0;
    attr.window[0][2]= 0.25;
    attr.window[0][3]= 0;
    //set
    rk_aiq_user_api2_awbV32_SetMultiWindowAttrib(ctx, attr);
    printf("set Awb MultiWindow\n\n");

    return 0;
}

static int sample_awb_getWbV32AwbMultiWindow(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wbV32_awb_mulWindow_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wbV32_awb_mulWindow_t));
    //get
    rk_aiq_user_api2_awbV32_GetMultiWindowAttrib(ctx, &attr);
    printf("get Awb MultiWindow:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t enable = %s\n\n", (attr.enable ? "true" : "false"));
    return 0;
}


static int sample_awb_awbv32_setAllAttr(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_wbV32_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wbV32_attrib_t));
    //get
    rk_aiq_user_api2_awbV32_GetAllAttrib(ctx, &attr);
    //modify
    attr.sync.sync_mode = sync;
    attr.stAuto.wbGainOffset.enable = !attr.stAuto.wbGainOffset.enable;
    //set
    rk_aiq_user_api2_awbV32_SetAllAttrib(ctx, attr);
    printf("set Awbv32 AllAttr\n\n");
    return 0;
}

static int sample_awb_awbv32_getAllAttr(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapiV2_wbV32_attrib_t attr ;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_wbV32_attrib_t));
    //get
    rk_aiq_user_api2_awbV32_GetAllAttrib(ctx, &attr);
    printf("get Awbv32 AllAttr:\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t bypass = %d\n", attr.byPass);
    printf("\t mode = %s\n", (attr.mode > 0 ? "auto" : "manual"));
    if (attr.mode > 0) {
        printf("\t wbGainOffset = {%s,[%f,%f,%f,%f]}\n\n",
                            (attr.stAuto.wbGainOffset.enable > 0 ? "true" : "false"),
                            attr.stAuto.wbGainOffset.offset[0],
                            attr.stAuto.wbGainOffset.offset[1],
                            attr.stAuto.wbGainOffset.offset[2],
                            attr.stAuto.wbGainOffset.offset[3]);
    } else {
        switch (attr.stManual.mode)
        {
            case RK_AIQ_MWB_MODE_CCT:
                printf("\t manual mode = RK_AIQ_MWB_MODE_CCT\n");
                printf("\t manual cct = %f, ccri = %f\n",
                                attr.stManual.para.cct.CCT,
                                attr.stManual.para.cct.CCRI);
                break;
            case RK_AIQ_MWB_MODE_WBGAIN:
                printf("\t manual mode = RK_AIQ_MWB_MODE_WBGAIN\n");
                printf("\t manual wbgain = [%f, %f, %f, %f]\n",
                                attr.stManual.para.gain.rgain,
                                attr.stManual.para.gain.grgain,
                                attr.stManual.para.gain.gbgain,
                                attr.stManual.para.gain.bgain);
                break;
            case RK_AIQ_MWB_MODE_SCENE:
                printf("\t manual mode = RK_AIQ_MWB_MODE_SCENE\n");
                printf("\t manual scene = %d\n",
                                attr.stManual.para.scene);
                break;
            default:
                printf("\t manual mode is invalid!\n");
                break;
        }
    }
    return 0;
}

static int sample_awb_WriteAwbIn(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    static int call_cnt =0;
    rk_aiq_uapiV2_awb_wrtIn_attr_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_awb_wrtIn_attr_t));
    attr.en = true;
    attr.mode = 1; // 1 means rgb ,0 means raw
    attr.call_cnt = call_cnt++;
    sprintf(attr.path,"/tmp");
    rk_aiq_user_api2_awb_WriteAwbIn(ctx, attr);
    printf("Write awb input \n\n");

    return 0;
}
static int sample_awb_setFFWbgain(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    rk_aiq_uapiV2_awb_ffwbgain_attr_t attr;
    memset(&attr,0,sizeof(rk_aiq_uapiV2_awb_ffwbgain_attr_t));
    attr.wggain={1.0,1.0,1.0,1.0};
    rk_aiq_user_api2_awb_SetFFWbgainAttrib(ctx, attr);
    printf("setFFWbgain \n\n");

    return 0;
}

//#if rk356x || rk3588
static void sample_awb1_usage()
{
    printf("Usage : \n");
    printf("  ImgProc API: \n");
    printf("\t 0) AWB: setWBMode-OP_MANUAL.\n");
    printf("\t 1) AWB: setWBMode-OP_AUTO.\n");
    printf("\t 2) AWB: getWBMode.\n");
    printf("\t 3) AWB: lockAWB.\n");
    printf("\t 4) AWB: unlockAWB.\n");
    printf("\t 5) AWB: setMWBScene.\n");
    printf("\t 6) AWB: getMWBScene.\n");
    printf("\t 7) AWB: setMWBGain.\n");
    printf("\t 8) AWB: getWBGain.\n");
    printf("\t 9) AWB: setMWBCT.\n");
    printf("\t a) AWB: getWBCT.\n");
    printf("\t b) AWB: setWbGainOffset.\n");
    printf("\t c) AWB: getWbGainOffset.\n");
    printf("\t d) AWB: setAwbGainAdjust.\n");
    printf("\t e) AWB: getAwbGainAdjust.\n");
    printf("\t f) AWB: setAllAttrib.\n");
    printf("\n");
    printf("  Module API: \n");
    printf("\t A) AWB: set Awbv21 AllAttr & Sync.\n");
    printf("\t B) AWB: set Awbv21 AllAttr & Async.\n");
    printf("\t C) AWB: set Awbv30 AllAttr & Sync.\n");
    printf("\t D) AWB: set Awbv30 AllAttr & Async.\n");
    printf("\t E) AWB: get CCT.\n");
    printf("\t F) AWB: Query Awb Info.\n");
    printf("\t G) AWB: Lock.\n");
    printf("\t I) AWB: Unlock.\n");
    printf("\t J) AWB: set Mode Manual & Sync.\n");
    printf("\t K) AWB: set Mode Manual & Async.\n");
    printf("\t L) AWB: set Mode Auto & Sync.\n");
    printf("\t M) AWB: set Mode Auto & Async.\n");
    printf("\t N) AWB: set Manual attr & Sync.\n");
    printf("\t O) AWB: set Manual attr & Async.\n");
    printf("\t P) AWB: set AwbGainAdjust & Sync.\n");
    printf("\t R) AWB: set AwbGainAdjust & Async.\n");
    printf("\t S) AWB: set WbGainOffset & Sync.\n");
    printf("\t T) AWB: set WbGainOffset & Async.\n");
    printf("\n");
    printf("\t h) AWB: help.\n");
    printf("\t q) AWB: return to main sample screen.\n");

}

//for rk356x ,rk3588
XCamReturn sample_awb_module(const void *arg)
{
    int key = -1;
    CLEAR();
    rk_aiq_wb_scene_t scene;
    rk_aiq_wb_gain_t gain;
    rk_aiq_wb_cct_t ct;
    opMode_t mode;
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup){
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }
    if (ctx == nullptr) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }
    unsigned int cct;

    sample_awb1_usage ();
    do {
        printf("\t please press the key: ");
        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        switch (key)
        {
            case 'h':
                CLEAR();
                sample_awb1_usage ();
                break;
            case '0':
                sample_set_wbmode_manual(ctx);
                printf("setWBMode manual\n\n");
                break;
            case '1':
                sample_set_wbmode_auto(ctx);
                printf("setWBMode auto\n\n");
                break;
            case '2':
                sample_get_wbmode(ctx);
                break;
            case '3':
                sample_lock_awb(ctx);
                printf("lockAWB\n\n");
                break;
            case '4':
                sample_unlock_awb(ctx);
                printf("unlockAWB\n\n");
                break;
            case '5':
                sample_set_mwb_scene(ctx);
                printf("setMWBScene\n\n");
                break;
            case '6':
                sample_get_mwb_scene(ctx);
                break;
            case '7':
                sample_set_mwb_gain(ctx);
                printf("setMWBGain\n\n");
                break;
            case '8':
                sample_get_mwb_gain(ctx);
                break;
            case '9':
                sample_set_mwb_ct(ctx);
                break;
            case 'a':
                sample_get_mwb_ct(ctx);
                break;
            case 'b':
                sample_set_awb_gainoffset(ctx);
                printf("setAWBGainOffset\n\n");
                break;
            case 'c':
                sample_get_awb_gainoffset(ctx);
                break;
            case 'd':
                sample_set_awb_gain_adjust(ctx);
                break;
            case 'e':
                sample_get_awb_gain_adjust(ctx);
                break;
            case 'f':
                sample_set_awbv30_atrr(ctx);
                break;
            case 'A':
                sample_awb_awbv21_setAllAttr(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_awbv21_getAllAttr(ctx);
                break;
            case 'B':
                sample_awb_awbv21_setAllAttr(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_awbv21_getAllAttr(ctx);
                usleep(40 * 1000);
                sample_awb_awbv21_getAllAttr(ctx);
                break;
            case 'C':
                sample_awb_awbv30_setAllAttr(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_awbv30_getAllAttr(ctx);
                break;
            case 'D':
                sample_awb_awbv30_setAllAttr(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_awbv30_getAllAttr(ctx);
                usleep(40 * 1000);
                sample_awb_awbv30_getAllAttr(ctx);
                break;
            case 'E':
                sample_awb_getCct(ctx);
                break;
            case 'F':
                sample_query_wb_info(ctx);
                break;
            case 'G':
                sample_awb_lock(ctx);
                break;
            case 'I':
                sample_awb_unlock(ctx);
                break;
            case 'J':
                sample_awb_setModeManual(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getMode(ctx);
                break;
            case 'K':
                sample_awb_setModeManual(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMode(ctx);
                usleep(40 * 1000);
                sample_awb_getMode(ctx);
                break;
            case 'L':
                sample_awb_setModeAuto(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getMode(ctx);
                break;
            case 'M':
                sample_awb_setModeAuto(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMode(ctx);
                usleep(40 * 1000);
                sample_awb_getMode(ctx);
                break;
            case 'N':
                sample_awb_setMwb(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getMwbAttr(ctx);
                break;
            case 'O':
                sample_awb_setMwb(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMwbAttr(ctx);
                usleep(40 * 1000);
                sample_awb_getMwbAttr(ctx);
                break;
            case 'P':
                sample_awb_setWbGainAdjust(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getWbGainAdjust(ctx);
                break;
            case 'R':
                sample_awb_setWbGainAdjust(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getWbGainAdjust(ctx);
                usleep(40 * 1000);
                sample_awb_getWbGainAdjust(ctx);
                break;
            case 'S':
                sample_awb_setWbGainOffset(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getWbGainOffset(ctx);
                break;
            case 'T':
                sample_awb_setWbGainOffset(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getWbGainOffset(ctx);
                usleep(40 * 1000);
                sample_awb_getWbGainOffset(ctx);
                break;
            // NOT Support MultiWindow
            case 'U':
                sample_awb_setWbAwbMultiWindow(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getWbAwbMultiWindow(ctx);
                break;
            case 'V':
                sample_awb_setWbAwbMultiWindow(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getWbAwbMultiWindow(ctx);
                usleep(40 * 1000);
                sample_awb_getWbAwbMultiWindow(ctx);
                break;
            case 'X':
                sample_awb_setMwb1(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMwbAttr(ctx);
                sample_awb_setMwb2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMwbAttr(ctx);

                usleep(40 * 1000);
                sample_awb_getMwbAttr(ctx);
                break;
#if 0
            case 'Y':
                sample_awb_printflog(ctx);
                break;
#endif
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');
    return XCAM_RETURN_NO_ERROR;

}
//#elseif rv1106
static void sample_awb32_usage()
{
    printf("Usage : \n");
    printf("  ImgProc API: \n");
    printf("\t 0) AWB: setWBMode-OP_MANUAL.\n");
    printf("\t 1) AWB: setWBMode-OP_AUTO.\n");
    printf("\t 2) AWB: getWBMode.\n");
    printf("\t 3) AWB: lockAWB.\n");
    printf("\t 4) AWB: unlockAWB.\n");
    printf("\t 5) AWB: setMWBScene.\n");
    printf("\t 6) AWB: getMWBScene.\n");
    printf("\t 7) AWB: setMWBGain.\n");
    printf("\t 8) AWB: getWBGain.\n");
    printf("\t 9) AWB: setMWBCT.\n");
    printf("\t a) AWB: getWBCT.\n");
    printf("\t b) AWB: setWbGainOffset.\n");
    printf("\t c) AWB: getWbGainOffset.\n");
    printf("\t d) AWB: setAwbGainAdjust.\n");
    printf("\t e) AWB: getAwbGainAdjust.\n");
    printf("\t f) AWB: setAllAttrib.\n");
    printf("\n");
    printf("  Module API: \n");
    printf("\t A) AWB: set Awbv32 AllAttr & Sync.\n");
    printf("\t B) AWB: set Awbv32 AllAttr & Async.\n");
    printf("\t E) AWB: get CCT.\n");
    printf("\t F) AWB: Query Awb Info.\n");
    printf("\t G) AWB: Lock.\n");
    printf("\t I) AWB: Unlock.\n");
    printf("\t J) AWB: set Mode Manual & Sync.\n");
    printf("\t K) AWB: set Mode Manual & Async.\n");
    printf("\t L) AWB: set Mode Auto & Sync.\n");
    printf("\t M) AWB: set Mode Auto & Async.\n");
    printf("\t N) AWB: set Manual attr & Sync.\n");
    printf("\t O) AWB: set Manual attr & Async.\n");
    printf("\t P) AWB: set AwbGainAdjust & Sync.\n");
    printf("\t R) AWB: set AwbGainAdjust & Async.\n");
    printf("\t S) AWB: set WbGainOffset & Sync.\n");
    printf("\t T) AWB: set WbGainOffset & Async.\n");
    printf("\t U) AWB: set MultiWindow & Sync.\n");
    printf("\t V) AWB: set MultiWindow & Async.\n");
    printf("\n");
    printf("\t h) AWB: help.\n");
    printf("\t q) AWB: return to main sample screen.\n");

}
//for rv1106
XCamReturn sample_awb32_module(const void *arg)
{
    int key = -1;
    CLEAR();
    rk_aiq_wb_scene_t scene;
    rk_aiq_wb_gain_t gain;
    rk_aiq_wb_cct_t ct;
    opMode_t mode;
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup){
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }
    if (ctx == nullptr) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }
    unsigned int cct;

    sample_awb32_usage ();
    do {
        printf("\t please press the key: ");
        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        switch (key)
        {
            case 'h':
                CLEAR();
                sample_awb32_usage ();
                break;
            case '0':
                sample_set_wbmode_manual(ctx);
                printf("setWBMode manual\n\n");
                break;
            case '1':
                sample_set_wbmode_auto(ctx);
                printf("setWBMode auto\n\n");
                break;
            case '2':
                sample_get_wbmode(ctx);
                break;
            case '3':
                sample_lock_awb(ctx);
                printf("lockAWB\n\n");
                break;
            case '4':
                sample_unlock_awb(ctx);
                printf("unlockAWB\n\n");
                break;
            case '5':
                sample_set_mwb_scene(ctx);
                printf("setMWBScene\n\n");
                break;
            case '6':
                sample_get_mwb_scene(ctx);
                break;
            case '7':
                sample_set_mwb_gain(ctx);
                printf("setMWBGain\n\n");
                break;
            case '8':
                sample_get_mwb_gain(ctx);
                break;
            case '9':
                sample_set_mwb_ct(ctx);
                break;
            case 'a':
                sample_get_mwb_ct(ctx);
                break;
            case 'b':
                sample_set_awb_gainoffset(ctx);
                printf("setAWBGainOffset\n\n");
                break;
            case 'c':
                sample_get_awb_gainoffset(ctx);
                break;
            case 'd':
                sample_set_awb_gain_adjust(ctx);
                break;
            case 'e':
                sample_get_awb_gain_adjust(ctx);
                break;
            case 'A':
                sample_awb_awbv32_setAllAttr(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_awbv32_getAllAttr(ctx);
                break;
            case 'B':
                sample_awb_awbv32_setAllAttr(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_awbv32_getAllAttr(ctx);
                usleep(40 * 1000);
                sample_awb_awbv32_getAllAttr(ctx);
                break;
            case 'E':
                sample_awb_getCct(ctx);
                break;
            case 'F':
                sample_query_wb_info(ctx);
                break;
            case 'G':
                sample_awb_lock(ctx);
                break;
            case 'I':
                sample_awb_unlock(ctx);
                break;
            case 'J':
                sample_awb_setModeManual(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getMode(ctx);
                break;
            case 'K':
                sample_awb_setModeManual(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMode(ctx);
                usleep(40 * 1000);
                sample_awb_getMode(ctx);
                break;
            case 'L':
                sample_awb_setModeAuto(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getMode(ctx);
                break;
            case 'M':
                sample_awb_setModeAuto(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMode(ctx);
                usleep(40 * 1000);
                sample_awb_getMode(ctx);
                break;
            case 'N':
                sample_awb_setMwb(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getMwbAttr(ctx);
                break;
            case 'O':
                sample_awb_setMwb(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMwbAttr(ctx);
                usleep(40 * 1000);
                sample_awb_getMwbAttr(ctx);
                break;
            case 'P':
                sample_awb_setWbGainAdjust(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getWbGainAdjust(ctx);
                break;
            case 'R':
                sample_awb_setWbGainAdjust(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getWbGainAdjust(ctx);
                usleep(40 * 1000);
                sample_awb_getWbGainAdjust(ctx);
                break;
            case 'S':
                sample_awb_setWbGainOffset(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getWbGainOffset(ctx);
                break;
            case 'T':
                sample_awb_setWbGainOffset(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getWbGainOffset(ctx);
                usleep(40 * 1000);
                sample_awb_getWbGainOffset(ctx);
                break;
            case 'U':
                sample_awb_setWbV32AwbMultiWindow(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_awb_getWbV32AwbMultiWindow(ctx);
                break;
            case 'V':
                sample_awb_setWbV32AwbMultiWindow(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getWbV32AwbMultiWindow(ctx);
                usleep(40 * 1000);
                sample_awb_getWbV32AwbMultiWindow(ctx);
                break;
            case 'X':
                sample_awb_setMwb1(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMwbAttr(ctx);
                sample_awb_setMwb2(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_awb_getMwbAttr(ctx);
                usleep(40 * 1000);
                sample_awb_getMwbAttr(ctx);
                break;
            case 'Y':
                sample_awb_WriteAwbIn(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case 'Z':
                sample_awb_setFFWbgain(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                break;
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');
    return XCAM_RETURN_NO_ERROR;
}
//#endif


