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

#include "uAPI2/rk_aiq_user_api2_alsc.h"

/*
 ******************************
 *
 * Module level API Sample Func
 *
 ******************************
 */

static int sample_lsc_set_attr(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  rk_aiq_lsc_attrib_t attr;
  memset(&attr, 0, sizeof(rk_aiq_lsc_attrib_t));

  ret = rk_aiq_user_api2_alsc_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "setlscAttr failed in getting lsc attrib!");

  printf("LSC r_data_tbl[0]:%d\n", attr.stManual.r_data_tbl[0]);

  attr.sync.sync_mode = sync;
  if (attr.mode == RK_AIQ_LSC_MODE_AUTO) {
    attr.mode = RK_AIQ_LSC_MODE_MANUAL;
    attr.byPass = false;
    memset(attr.stManual.r_data_tbl, 4096, 17*17*sizeof(unsigned short));
  } else {
    attr.mode = RK_AIQ_LSC_MODE_AUTO;
  }
  //set
  ret = rk_aiq_user_api2_alsc_SetAttrib(ctx, attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "set lsc Attr failed!");
  printf("set lsc Attr\n\n");

  return 0;
}

static int sample_lsc_get_attr(const rk_aiq_sys_ctx_t* ctx)
{
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  rk_aiq_lsc_attrib_t attr;
  memset(&attr,0,sizeof(rk_aiq_lsc_attrib_t));
  //get
  ret = rk_aiq_user_api2_alsc_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "get lsc Attr failed!");
  printf("get lsc Attr:\n\n");
  printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
  printf("\t enable = %s\n", (attr.byPass ? "false" : "true"));
  if (attr.mode == RK_AIQ_LSC_MODE_MANUAL){
    printf("\t mode = Manual\n");
    printf("\t manual table\n");
    printf("\t\t r: %d, %d, %d \n",
           attr.stManual.r_data_tbl[0],
           attr.stManual.r_data_tbl[1],
           attr.stManual.r_data_tbl[2]);
    printf("\t\t gr: %d, %d, %d \n",
           attr.stManual.gr_data_tbl[0],
           attr.stManual.gr_data_tbl[1],
           attr.stManual.gr_data_tbl[2]);
    printf("\t\t gb: %d, %d, %d \n",
           attr.stManual.gb_data_tbl[0],
           attr.stManual.gb_data_tbl[1],
           attr.stManual.gb_data_tbl[2]);
    printf("\t\t b: %d, %d, %d \n",
           attr.stManual.b_data_tbl[0],
           attr.stManual.b_data_tbl[1],
           attr.stManual.b_data_tbl[2]);
  } else if (attr.mode == RK_AIQ_LSC_MODE_AUTO) {
    printf("\t mode = Auto\n");
  } else {
    printf("\t mode is Manual");
  }
  return 0;
}

static int sample_lsc_set_attr_async(const rk_aiq_sys_ctx_t* ctx)
{
  sample_lsc_set_attr(ctx, RK_AIQ_UAPI_MODE_ASYNC);
  sample_lsc_get_attr(ctx);
  usleep(40*1000);
  sample_lsc_get_attr(ctx);

  return 0;
}

static int sample_lsc_set_attr_sync(const rk_aiq_sys_ctx_t* ctx)
{
  sample_lsc_set_attr(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
  sample_lsc_get_attr(ctx);

  return 0;
}

uapi_case_t lsc_uapi_list[] = {
  { .desc = "ALSC: set lsc gain table async",
    .func = (uapi_case_func)sample_lsc_set_attr_async
  },
  { .desc = "ALSC: set lsc gain table sync",
    .func = (uapi_case_func)sample_lsc_set_attr_sync
  },
  { .desc = "ALSC: get lsc gain table",
    .func = (uapi_case_func)sample_lsc_get_attr
  },
  {
    .desc = NULL,
    .func = NULL,
  }
};

XCamReturn sample_alsc_module(const void* arg)
{
  CLEAR();
  const demo_context_t* demo_ctx = (demo_context_t*)arg;
  const rk_aiq_sys_ctx_t* ctx;
  if (demo_ctx->camGroup) {
    ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
  } else {
    ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
  }

  if (ctx == nullptr) {
    ERR("%s, ctx is nullptr\n", __FUNCTION__);
    return XCAM_RETURN_ERROR_PARAM;
  }

  uapi_process_loop(ctx, lsc_uapi_list);

  return XCAM_RETURN_NO_ERROR;
}
