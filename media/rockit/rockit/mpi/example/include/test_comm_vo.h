/*
 * Copyright 2021 Rockchip Electronics Co. LTD
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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VO_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VO_H_

#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define RK356X_VO_DEV_HD0 0
#define RK356X_VO_DEV_HD1 1

enum {
  RK356X_VOP_LAYER_CLUSTER0,
  RK356X_VOP_LAYER_CLUSTER0_1,
  RK356X_VOP_LAYER_CLUSTER1,
  RK356X_VOP_LAYER_CLUSTER1_1,
  RK356X_VOP_LAYER_ESMART_0,
  RK356X_VOP_LAYER_ESMART_1,
  RK356X_VOP_LAYER_SMART0,
  RK356X_VOP_LAYER_SMART1,
};

void TEST_VO_GetDisplaySize(RK_S32 enIntfSync, RK_U32 *s32W, RK_U32 *s32H);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VPSS_H_
