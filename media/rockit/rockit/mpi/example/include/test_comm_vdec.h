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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VDEC_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VDEC_H_

#include "rk_common.h"
#include "rk_comm_vdec.h"
#include "rk_mpi_vdec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

RK_S32 TEST_VDEC_Start(VDEC_CHN VdecChn,
                        VDEC_CHN_ATTR_S *pstVdecAttr,
                        VDEC_CHN_PARAM_S *pstVdecParam,
                        VIDEO_DISPLAY_MODE_E enDispMode);
RK_S32 TEST_VDEC_Stop(VDEC_CHN VdecChn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  // SRC_TESTS_RT_MPI_COMMON_TEST_COMM_VDEC_H_
