/*
 *  Copyright (c) 2023 Rockchip Corporation
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

static int sample_ainr_get_params(const rk_aiq_sys_ctx_t *ctx) {
    rk_ainr_param para;
    int ret = rk_aiq_uapi2_sysctl_getAinrParams(ctx, &para);

    printf("getAinrParams test for ainr params set: en=%d tuning_visual_flag: %d "
             "gain_tab_len:%d, gain_max:%d cur_gain:%f raw mean:%f, yuv mean:%f \n",
              para.enable, para.tuning_visual_flag, para.gain_tab_len, para.gain_max,
              para.gain, para.RawMeanluma, para.YuvMeanluma);
    printf("luma_point: %f,%f,%f,%f,%f,%f\n",
        para.luma_curve_tab[0],para.luma_curve_tab[1],para.luma_curve_tab[2],para.luma_curve_tab[3],para.luma_curve_tab[4],para.luma_curve_tab[5]);
    for (int i =0; i < 13; i++) {
        printf(">>>%d :gain_tab: %f, sigma: %f, shade: %f, sharp: %f, min_luma:%f, sat_scale:%f, dark_contrast: %f,"
                " ai_ratio: %f, mot_thresh: %f, static_thresh: %f, mot_nr_stren: %f \n",
                i, para.gain_tab[i], para.sigma_tab[i], para.shade_tab[i], para.sharp_tab[i], para.min_luma_tab[i],
                para.sat_scale_tab[i], para.dark_contrast_tab[i], para.ai_ratio_tab[i], para.mot_thresh_tab[i],
                para.static_thresh_tab[i], para.mot_nr_stren_tab[i]);
        printf("current simg curve iso_%f: sigma_curve: %f,%f,%f,%f,%f,%f\n", para.gain_tab[i] * 50,
        para.sigma_curve_tab[0][i],para.sigma_curve_tab[1][i],para.sigma_curve_tab[2][i],para.sigma_curve_tab[3][i],para.sigma_curve_tab[4][i],para.sigma_curve_tab[5][i]);
    }


    return ret;
}

uapi_case_t ainr_uapi_list[] = {
  {
    .desc = "Ainr: get ainr params",
    .func = (uapi_case_func)sample_ainr_get_params,
  },
  {
    .desc = NULL,
    .func = NULL,
  }
};

XCamReturn sample_ainr_module(const void *arg) {
  int key = -1;
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

  uapi_process_loop(ctx, ainr_uapi_list);

  return XCAM_RETURN_NO_ERROR;
}
