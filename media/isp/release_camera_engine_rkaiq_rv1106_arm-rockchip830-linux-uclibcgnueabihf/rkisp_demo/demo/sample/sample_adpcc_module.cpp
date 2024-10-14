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

/*
 ******************************
 *
 * Module level API Sample Func
 *
 ******************************
 */

static int sample_dpcc_set_attr(const rk_aiq_sys_ctx_t *ctx,
                                rk_aiq_uapi_mode_sync_e sync) {
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  rk_aiq_dpcc_attrib_V20_t attr;
  memset(&attr, 0, sizeof(rk_aiq_dpcc_attrib_V20_t));

  ret = rk_aiq_user_api2_adpcc_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "set dpccAttr failed in getting dpcc attrib!");

  printf("dpcc set config:[%d][%d][%d]\n",
         attr.stManual.stOnfly.expert_mode.stage1_use_set_1,
         attr.stManual.stOnfly.expert_mode.stage1_use_set_2,
         attr.stManual.stOnfly.expert_mode.stage1_use_set_3);

  attr.sync.sync_mode = sync;
  if (attr.eMode == ADPCC_OP_MODE_AUTO) {
    attr.eMode = ADPCC_OP_MODE_MANUAL;
    attr.stManual.stOnfly.mode = ADPCC_ONFLY_MODE_EXPERT;
    attr.stManual.stOnfly.expert_mode.stage1_use_set_1 = 1;
    attr.stManual.stOnfly.expert_mode.stage1_use_set_2 = 1;
    attr.stManual.stOnfly.expert_mode.stage1_use_set_3 = 1;
  } else {
    attr.eMode = ADPCC_OP_MODE_AUTO;
  }
  // set
  ret = rk_aiq_user_api2_adpcc_SetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "set dpcc Attr failed!");
  printf("set dpcc Attr\n\n");

  return 0;
}

static int sample_dpcc_get_attr(const rk_aiq_sys_ctx_t *ctx) {
  XCamReturn ret = XCAM_RETURN_NO_ERROR;
  rk_aiq_dpcc_attrib_V20_t attr;
  memset(&attr, 0, sizeof(rk_aiq_dpcc_attrib_V20_t));
  // get
  ret = rk_aiq_user_api2_adpcc_GetAttrib(ctx, &attr);
  RKAIQ_SAMPLE_CHECK_RET(ret, "get dpcc Attr failed!");
  printf("get dpcc Attr:\n\n");
  printf("\t sync = %d, done = %d\n", attr.sync.sync_mode, attr.sync.done);
  if (attr.eMode == ADPCC_OP_MODE_MANUAL) {
    printf("\t mode = Manual\n");
    printf("dpcc set config:[%d][%d][%d]\n",
           attr.stManual.stOnfly.expert_mode.stage1_use_set_1,
           attr.stManual.stOnfly.expert_mode.stage1_use_set_2,
           attr.stManual.stOnfly.expert_mode.stage1_use_set_3);
  } else if (attr.eMode == ADPCC_OP_MODE_AUTO) {
    printf("\t mode = Auto\n");
  } else {
    printf("\t mode is unkown");
  }
  return 0;
}

static int sample_dpcc_set_attr_async(const rk_aiq_sys_ctx_t *ctx) {
  sample_dpcc_set_attr(ctx, RK_AIQ_UAPI_MODE_ASYNC);
  sample_dpcc_get_attr(ctx);
  usleep(40 * 1000);
  sample_dpcc_get_attr(ctx);

  return 0;
}

static int sample_dpcc_set_attr_sync(const rk_aiq_sys_ctx_t *ctx) {
  sample_dpcc_set_attr(ctx, RK_AIQ_UAPI_MODE_DEFAULT);
  sample_dpcc_get_attr(ctx);

  return 0;
}

uapi_case_t dpcc_uapi_list[] = {
  {
    .desc = "Adpcc: set dpcc config async",
    .func = (uapi_case_func)sample_dpcc_set_attr_async
  },
  {
    .desc = "Adpcc: set dpcc config sync",
    .func = (uapi_case_func)sample_dpcc_set_attr_sync
  },
  {
    .desc = "Adpcc: get dpcc config",
    .func = (uapi_case_func)sample_dpcc_get_attr
  },
  {
    .desc = NULL,
    .func = NULL,
  }
};

XCamReturn sample_adpcc_module(const void *arg) {
  CLEAR();
  const demo_context_t *demo_ctx = (demo_context_t *)arg;
  const rk_aiq_sys_ctx_t *ctx;
  if (demo_ctx->camGroup) {
    ctx = (rk_aiq_sys_ctx_t *)(demo_ctx->camgroup_ctx);
  } else {
    ctx = (rk_aiq_sys_ctx_t *)(demo_ctx->aiq_ctx);
  }

  if (ctx == nullptr) {
    ERR("%s, ctx is nullptr\n", __FUNCTION__);
    return XCAM_RETURN_ERROR_PARAM;
  }

  uapi_process_loop(ctx, dpcc_uapi_list);

  return XCAM_RETURN_NO_ERROR;
}
