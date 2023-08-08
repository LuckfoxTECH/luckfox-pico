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

static void sample_a3dlut_usage()
{
    printf("Usage : \n");
    printf("  ImgProc API: \n");
    printf("\t 0) 3DLUT:         Set 3DLUT MANUAL Mode.\n");
    printf("\t 1) 3DLUT:         Set 3DLUT AUTO Mode.\n");
    printf("\t 2) 3DLUT:         Get 3DLUT Mode.\n");
    printf("\t 3) 3DLUT:         Set 3DLUT Manual Lut.\n");
    printf("\t 4) 3DLUT:         Get 3DLUT Lut.\n");
    printf("\t 5) 3DLUT:         Get A3DLUT Strength.\n");
    printf("\t 6) 3DLUT:         Get A3DLUT LutName.\n");
    printf("  Module API: \n");
    printf("\t a) 3DLUT:         Set 3dlut Attr & Sync .\n");
    printf("\t b) 3DLUT:         Set 3dlut Attr & Async .\n");
    printf("\t c) 3DLUT:         Set Mode Manual & Sync.\n");
    printf("\t d) 3DLUT:         Set Mode Manual & Async.\n");
    printf("\t e) 3DLUT:         Set Mode Auto & Sync.\n");
    printf("\t f) 3DLUT:         Set Mode Auto & Async.\n");
    printf("\t g) 3DLUT:         Set Manual attr & Sync.\n");
    printf("\t i) 3DLUT:         Set Manual attr & Async.\n");
    printf("\t j) 3DLUT:         Query A3DLUT Info.\n");
    printf("\n");
    printf("\t h) 3DLUT:         help.\n");
    printf("\t q) 3DLUT:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_a3dlut_info(const void *arg)
{
    printf ("enter 3DLUT modult test!\n");
}

/*
******************************
*
* ImgProc level API Sample Func
*
******************************
*/

static int sample_set_a3dlut_manual(const rk_aiq_sys_ctx_t* ctx)
{
    opMode_t mode = OP_MANUAL;
    rk_aiq_uapi2_setLut3dMode(ctx, mode);
    return 0;
}

static int sample_set_a3dlut_auto(const rk_aiq_sys_ctx_t* ctx)
{
    opMode_t mode = OP_AUTO;
    rk_aiq_uapi2_setLut3dMode(ctx, mode);
    return 0;
}

static int sample_get_a3dlut_mode(const rk_aiq_sys_ctx_t* ctx)
{
    opMode_t mode;
    rk_aiq_uapi2_getLut3dMode(ctx, &mode);
    if (mode == OP_MANUAL)
        printf("Get 3DLUT Mode is: Manual \n");
    else if (mode == OP_AUTO)
        printf("Get 3DLUT Mode is: AUTO\n");
    else
        printf("Get 3DLUT Mode is: INVALID\n");
    return 0;
}

static int sample_set_a3dlut_manual_lut(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_lut3d_table_t mlut;
    memset(&mlut, 0, sizeof(mlut));
    // create mlut
    rk_aiq_uapi2_getM3dLut(ctx, &mlut);
    memcpy(mlut.look_up_table_b, mlut.look_up_table_r, sizeof(unsigned short) * 729);
    for(int i = 0; i < 729; i++)
        mlut.look_up_table_g[i] = mlut.look_up_table_r[i]<<2;
    // set mlut
    rk_aiq_uapi2_setM3dLut(ctx, &mlut);
    printf("Set 3DLUT Manual Lut\n\n");
    return 0;
}

static int sample_get_a3dlut_lut(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_lut3d_table_t mlut;
    rk_aiq_uapi2_getM3dLut(ctx, &mlut);
    printf("Get 3DLUT Lut: ");
    printf("\t r: %d, %d, %d \n", mlut.look_up_table_r[0],
                                    mlut.look_up_table_r[1],
                                    mlut.look_up_table_r[2]);
    printf("\t g: %d, %d, %d \n", mlut.look_up_table_g[0],
                                    mlut.look_up_table_g[1],
                                    mlut.look_up_table_g[2]);
    printf("\t b: %d, %d, %d \n", mlut.look_up_table_b[0],
                                    mlut.look_up_table_b[1],
                                    mlut.look_up_table_b[2]);
    return 0;
}

static int sample_get_a3dlut_strength(const rk_aiq_sys_ctx_t* ctx)
{
    float alpha;
    rk_aiq_uapi2_getA3dLutStrth(ctx, &alpha);
    printf("Get A3DLUT Strength: %f\n\n", alpha);
    return 0;
}

static int sample_get_a3dlut_lutname(const rk_aiq_sys_ctx_t* ctx)
{
    char lutname[25];
    rk_aiq_uapi2_getA3dLutName(ctx, lutname);
    printf("Get A3DLUT LutName: %s\n\n", lutname);
    return 0;
}

/*
******************************
*
* Module level API Sample Func
*
******************************
*/

static int sample_3dlut_set3dlutAttr(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_lut3d_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_lut3d_attrib_t));
    //get
    ret = rk_aiq_user_api2_a3dlut_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set3dlutAttr failed in getting a3dlut attrib!");
    //modify
    attr.sync.sync_mode = sync;
    if (attr.mode > 1 ) {
        attr.mode = RK_AIQ_LUT3D_MODE_MANUAL;
        memset(attr.stManual.look_up_table_g, 0, 729*sizeof(unsigned short));
    } else {
        attr.mode = RK_AIQ_LUT3D_MODE_AUTO;
    }
    //set
    ret = rk_aiq_user_api2_a3dlut_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set 3DLUT Attr failed!");
    printf("set 3DLUT Attr\n\n");

    return 0;
}

static int sample_3dlut_get3dlutAttr(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_lut3d_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_lut3d_attrib_t));
    //get
    ret = rk_aiq_user_api2_a3dlut_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get 3dlut Attr failed!");
    printf("get 3DLUT Attr:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    printf("\t enable = %s\n", (attr.byPass ? "false" : "true"));
    if (attr.mode == RK_AIQ_LUT3D_MODE_MANUAL){
        printf("\t mode = Manual\n");
        printf("\t manual lut");
        printf("\t\t r: %d, %d, %d \n", attr.stManual.look_up_table_r[0],
                                        attr.stManual.look_up_table_r[1],
                                        attr.stManual.look_up_table_r[2]);
        printf("\t\t g: %d, %d, %d \n", attr.stManual.look_up_table_g[0],
                                        attr.stManual.look_up_table_g[1],
                                        attr.stManual.look_up_table_g[2]);
        printf("\t\t b: %d, %d, %d \n", attr.stManual.look_up_table_b[0],
                                        attr.stManual.look_up_table_b[1],
                                        attr.stManual.look_up_table_b[2]);
    } else if (attr.mode == RK_AIQ_LUT3D_MODE_AUTO){
        printf("\t mode = Auto\n");
    } else {
        printf("\t mode is invalid!");
    }
    return 0;
}

static int sample_3dlut_setModeManual(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_lut3d_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_lut3d_attrib_t));
    //get
    ret = rk_aiq_user_api2_a3dlut_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setModeManual failed in getting a3dlut attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_LUT3D_MODE_MANUAL;
    //set
    ret = rk_aiq_user_api2_a3dlut_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Mode Manual failed!");
    printf("set 3DLUT Mode Manual\n\n");

    return 0;
}

static int sample_3dlut_setModeAuto(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_lut3d_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_lut3d_attrib_t));
    //get
    ret = rk_aiq_user_api2_a3dlut_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setModeAuto failed in getting a3dlut attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_LUT3D_MODE_AUTO;
    //set
    ret = rk_aiq_user_api2_a3dlut_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Mode Auto failed!");
    printf("set 3DLUT Mode Auto\n\n");

    return 0;
}

static int sample_3dlut_getMode(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_lut3d_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_lut3d_attrib_t));
    //get
    ret = rk_aiq_user_api2_a3dlut_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "get 3dlut Mode failed!");
    printf("get 3DLUT Mode:\n\n");
    printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
    if (attr.mode == RK_AIQ_LUT3D_MODE_MANUAL){
        printf("\t mode = Manual\n");
    } else if (attr.mode == RK_AIQ_LUT3D_MODE_AUTO){
        printf("\t mode = Auto\n");
    } else {
        printf("\t mode is invalid!");
    }
    return 0;
}

static int sample_3dlut_setM3dlut(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    rk_aiq_lut3d_attrib_t attr;
    memset(&attr,0,sizeof(rk_aiq_lut3d_attrib_t));
    //get
    ret = rk_aiq_user_api2_a3dlut_GetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "setM3dlut failed in getting a3dlut attrib!");
    //modify
    attr.sync.sync_mode = sync;
    attr.mode = RK_AIQ_LUT3D_MODE_MANUAL;
    if (attr.stManual.look_up_table_g[2] > 0) {
        memset(attr.stManual.look_up_table_g, 0, 729*sizeof(unsigned short));
    } else {
        memcpy(attr.stManual.look_up_table_g, attr.stManual.look_up_table_r, \
                                sizeof(attr.stManual.look_up_table_r));
    }

    //set
    ret = rk_aiq_user_api2_a3dlut_SetAttrib(ctx, &attr);
    RKAIQ_SAMPLE_CHECK_RET(ret, "set Manual attr failed!");
    printf("set Manual attr\n\n");

    return 0;
}


static int sample_query_a3dlut_info(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_lut3d_querry_info_t info;
    rk_aiq_user_api2_a3dlut_Query3dlutInfo(ctx, &info);
    printf("Query 3DLUT Info:\n\n");
    printf("\t enable: %s, \n", (info.lut3d_en ? "true" : "false"));
    printf("\t lut: ");
    printf("\t\t r: %d, %d, %d \n", info.look_up_table_r[0],
                                info.look_up_table_r[1],
                                info.look_up_table_r[2]);
    printf("\t\t g: %d, %d, %d \n", info.look_up_table_g[0],
                                info.look_up_table_g[1],
                                info.look_up_table_g[2]);
    printf("\t\t b: %d, %d, %d \n", info.look_up_table_b[0],
                                info.look_up_table_b[1],
                                info.look_up_table_b[2]);
    printf("\t alpha: %f \n", info.alpha);
    printf("\t lutname: %s \n", info.name);
    return 0;
}

XCamReturn sample_a3dlut_module(const void *arg)
{
    int key = -1;
    CLEAR();

    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx;
    if (demo_ctx->camGroup){
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }

    /*TODO: when rkaiq_3A_server & rkisp_demo run in two different shell, rk_aiq_sys_ctx_t would be null?*/
    if (ctx == NULL) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    sample_a3dlut_usage ();

    do {

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        switch (key)
        {
            case 'h':
                CLEAR();
                sample_a3dlut_usage ();
                break;
            case '0':
                sample_set_a3dlut_manual(ctx);
                printf("Set 3DLUT MANUAL Mode\n\n");
                break;
            case '1':
                sample_set_a3dlut_auto(ctx);
                printf("Set 3DLUT AUTO Mode\n\n");
                break;
            case '2':
                sample_get_a3dlut_mode(ctx);
                break;
            case '3':
                sample_set_a3dlut_manual_lut(ctx);
                break;
            case '4':
                sample_get_a3dlut_lut(ctx);
                break;
            case '5':
                sample_get_a3dlut_strength(ctx);
                break;
            case '6':
                sample_get_a3dlut_lutname(ctx);
                break;
            case 'a':
                sample_3dlut_set3dlutAttr(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_3dlut_get3dlutAttr(ctx);
                break;
            case 'b':
                sample_3dlut_set3dlutAttr(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_3dlut_get3dlutAttr(ctx);
                usleep(40*1000);
                sample_3dlut_get3dlutAttr(ctx);
                break;
            case 'c':
                sample_3dlut_setModeManual(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_3dlut_getMode(ctx);
                break;
            case 'd':
                sample_3dlut_setModeManual(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_3dlut_getMode(ctx);
                usleep(40*1000);
                sample_3dlut_getMode(ctx);
                break;
            case 'e':
                sample_3dlut_setModeAuto(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_3dlut_getMode(ctx);
                break;
            case 'f':
                sample_3dlut_setModeAuto(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_3dlut_getMode(ctx);
                usleep(40*1000);
                sample_3dlut_getMode(ctx);
                break;
            case 'g':
                sample_3dlut_setM3dlut(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
                sample_3dlut_get3dlutAttr(ctx);
                break;
                break;
            case 'i':
                sample_3dlut_setM3dlut(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                sample_3dlut_get3dlutAttr(ctx);
                usleep(40*1000);
                sample_3dlut_get3dlutAttr(ctx);
                break;
            case 'j':
                sample_query_a3dlut_info(ctx);
                break;
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
