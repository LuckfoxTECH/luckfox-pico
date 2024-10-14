/*
 * Copyright (C) 2021 Rockchip Electronics Co., Ltd.
 * Authors:
 *  Cerf Yu <cerf.yu@rock-chips.com>
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
 */

/* im2d_slt config */
#define IM2D_SLT_THREAD_EN              1   /* Enable multi-threaded mode. */
#define IM2D_SLT_THREAD_MAX             10  /* Maximum number of threads. */
#define IM2D_SLT_WHILE_EN               1   /* Enable while mode, 1: while, 0 single. */
#define IM2D_SLT_WHILE_NUM              3   /* Number of while mode. */

#define IM2D_SLT_DRM_BUFFER_EN          0   /* Enable use drm buffer. */
#define IM2D_SLT_GRAPHICBUFFER_EN       1   /* Enable use GraphicBuffer. */
#define IM2D_SLT_BUFFER_CACHEABLE       0   /* Enable buffer cache. Only support GraphicBuffer. */
#define IM2D_SLT_BUFFER_PHY_EN          0   /* Enable physical memory. Only support drm buffer.*/

#define IM2D_SLT_TEST_RGA2_EN           1   /* Enable rga2 case. */
#define IM2D_SLT_TEST_RGA3_0_EN         1   /* Enable rga3_core0 case. */
#define IM2D_SLT_TEST_RGA3_1_EN         1   /* Enable rga3_core1 case. */
#define IM2D_SLT_TEST_RGA3_0_FBC_EN     1   /* Enable rga3_core0 fbc_mode case. */
#define IM2D_SLT_TEST_RGA3_1_FBC_EN     1   /* Enable rga3_core1 fbc_mode case. */

#define IM2D_SLT_DEFAULT_WIDTH          1280                        /* Default image width. */
#define IM2D_SLT_DEFAULT_HEIGHT         720                         /* Default image height. */
#if IM2D_SLT_GRAPHICBUFFER_EN
#define IM2D_SLT_DEFAULT_FORMAT         HAL_PIXEL_FORMAT_RGBA_8888  /* Default image format. */
#else
#define IM2D_SLT_DEFAULT_FORMAT         RK_FORMAT_RGBA_8888         /* Default image format. */
#endif

#define IM2D_SLT_DEFAULT_INPUT_PATH     "/data"
#define IM2D_SLT_DEFAULT_OUTPUT_PATH    "/data"