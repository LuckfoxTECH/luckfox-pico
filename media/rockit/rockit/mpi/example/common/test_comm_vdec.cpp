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

#include "rk_debug.h"
#include "rk_mpi_vdec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

RK_S32 TEST_VDEC_Start(VDEC_CHN VdecChn,
                        VDEC_CHN_ATTR_S *pstVdecAttr,
                        VDEC_CHN_PARAM_S *pstVdecParam,
                        VIDEO_DISPLAY_MODE_E enDispMode) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_VDEC_CreateChn(VdecChn, pstVdecAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("create %d vdec failed with %#x!", VdecChn, s32Ret);
        return s32Ret;
    }
    s32Ret = RK_MPI_VDEC_SetChnParam(VdecChn, pstVdecParam);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VDEC_SetChnParam failed with %#x!", s32Ret);
        return s32Ret;
    }
    s32Ret = RK_MPI_VDEC_StartRecvStream(VdecChn);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VDEC_StartRecvStream failed with %#x!", s32Ret);
        return s32Ret;
    }
    s32Ret = RK_MPI_VDEC_SetDisplayMode(VdecChn, enDispMode);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_VDEC_SetDisplayMode failed with %#x!", s32Ret);
        return s32Ret;
    }

    return RK_SUCCESS;
}

RK_S32 TEST_VDEC_Stop(VDEC_CHN VdecChn) {
    RK_S32 s32Ret;

    s32Ret = RK_MPI_VDEC_StopRecvStream(VdecChn);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_VDEC_StopRecvStream failed with %#x!", s32Ret);
    }

    s32Ret = RK_MPI_VDEC_DestroyChn(VdecChn);
    if (RK_SUCCESS != s32Ret) {
        RK_LOGE("RK_MPI_VDEC_DestroyChn failed with %#x!", s32Ret);
    }

    return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
