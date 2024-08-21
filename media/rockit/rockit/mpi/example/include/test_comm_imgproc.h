/*
 * Copyright 2020 Rockchip Electronics Co. LTD
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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_COMM_IMGPROC_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_COMM_IMGPROC_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "rk_debug.h"
#include "rk_comm_video.h"

#define DEFAULT_IMAGE_FUZZY_DIFF_THRESHOLD      0.1

RK_S32 TEST_COMM_FillImage(RK_U8 *buf, RK_U32 width, RK_U32 height,
                   RK_U32 hor_stride, RK_U32 ver_stride, PIXEL_FORMAT_E fmt,
                   RK_U32 frame_count);

RK_BOOL TEST_COMM_CompareImageFuzzy(
        RK_U8 *pu8Src, RK_U8 *pu8Dst, RK_U32 u32Stride,
        RK_U32 u32Width, RK_U32 u32Height, RK_DOUBLE dThreshold);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  // SRC_TESTS_RT_MPI_COMMON_TEST_COMM_IMGPROC_H_

