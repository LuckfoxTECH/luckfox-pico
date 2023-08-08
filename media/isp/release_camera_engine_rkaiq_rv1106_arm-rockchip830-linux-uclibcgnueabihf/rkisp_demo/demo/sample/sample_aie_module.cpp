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

static void sample_aie_usage()
{
    printf("Usage : \n");
    printf("  ImgProc API: \n");
    printf("\t a) AIE:         Set effect mode.\n");
    printf("\t b) AIE:         Get effect mode.\n");
    printf("  Module API: \n");
    printf("\t 1) AIE:         Set effect mode  & Async .\n");
    printf("\t 2) AIE:         Set effect mode  & Sync .\n");
    printf("\t 3) AIE:         Get attrib       & Async .\n");
    printf("\t 4) AIE:         Get attrib       & Sync .\n");
    printf("\n");
    printf("\t h) AIE:         help.\n");
    printf("\t q) AIE:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_aie_info(const void *arg)
{
    printf ("enter AIE module test!\n");
}

/*
******************************
*
* Module level API Sample Func
*
******************************
*/
//rv1106 only support {0,1}
static int sample_aie_set_mode(const rk_aiq_sys_ctx_t* ctx, int mode,
                              rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  aie_attrib_t attr;
  memset(&attr, 0, sizeof(aie_attrib_t));

  ret = rk_aiq_user_api2_aie_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "setAttr failed in getting aie attrib!");

  attr.sync.sync_mode   = sync;
  attr.mode              = (rk_aiq_ie_effect_t)mode;

  //set
  ret = rk_aiq_user_api2_aie_SetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "set aie Attr failed!");
  printf("set aie mode: %d, done: %d\n\n", mode, attr.sync.done);

  return 0;
}

static int sample_aie_get_attrib(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  aie_attrib_t attr;
  memset(&attr,0,sizeof(aie_attrib_t));
  //get
  ret = rk_aiq_user_api2_aie_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "get aie Attr failed!");
  printf("\t get aie mode: %d, done: %d\n", attr.mode, attr.sync.done);

  return 0;
}

/*
******************************
*
* ImgProc level API Sample Func
*
******************************
*/
// rv1106 only support {0,1}
static int sample_aie_set_effect_mode(const rk_aiq_sys_ctx_t* ctx, uint8_t mode)
{
    rk_aiq_uapi2_setColorMode(ctx, mode);
    return 0;
}

static int sample_aie_get_effect_mode(const rk_aiq_sys_ctx_t* ctx)
{
    unsigned int mode;
    rk_aiq_uapi2_getColorMode(ctx, &mode);
    switch (mode)
    {
    case 0:
        printf("Get AIE Mode is: NONE \n");
        break;
    case 1:
        printf("Get AIE Mode is: Black White \n");
        break;
    case 2:
        printf("Get AIE Mode is: NEGATIVE \n");
        break;
    case 3:
        printf("Get AIE Mode is: SEPIA \n");
        break;
    case 4:
        printf("Get AIE Mode is: EMBOSS \n");
        break;
    case 5:
        printf("Get AIE Mode is: SKETCH \n");
        break;
    case 6:
        printf("Get AIE Mode is: SHARPEN \n");
        break;
    default:
        break;
    }
    return 0;
}


XCamReturn sample_aie_module(const void *arg)
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

    aie_attrib_t default_attr;
    memset(&default_attr, 0, sizeof(aie_attrib_t));
    rk_aiq_user_api2_aie_GetAttrib(ctx, &default_attr);

    sample_aie_usage ();

    do {

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n");

        switch (key)
        {
            case 'h':
                CLEAR();
                sample_aie_usage ();
                break;
            case '1':
                printf("test aie effect mode iteratively in async mode...\n");
                for (int i = 0; i < 7; i++) {
                    sample_aie_set_mode(ctx, i, RK_AIQ_UAPI_MODE_ASYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_aie_set_mode(ctx, default_attr.mode, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case '2':
                printf("test aie effect mode iteratively in sync mode...\n");
                for (int i = 0; i < 7; i++) {
                    sample_aie_set_mode(ctx, i, RK_AIQ_UAPI_MODE_SYNC);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");

                sample_aie_set_mode(ctx, default_attr.mode, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case '3':
                sample_aie_get_attrib(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case '4':
                sample_aie_get_attrib(ctx, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case 'a':
                printf("test aie effect mode iteratively...\n");
                for (int i = 0; i < 7; i++) {
                    sample_aie_set_effect_mode(ctx, i);
                    usleep(10 * 1000);
                }
                printf("end of the test\n\n");
                sample_aie_set_effect_mode(ctx, default_attr.mode);
                break;
            case 'b':
                sample_aie_get_effect_mode(ctx);
                break;
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}
