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

#include "rk_mpi_ao.h"

#include "rk_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

RK_S32 TEST_AO_Start(AUDIO_DEV AoDev, AIO_ATTR_S *pstAoAttr, RK_U32 u32ChnCnt, AUDIO_SAMPLE_RATE_E *pEnSmpRates) {
    RK_S32 s32Ret = RK_SUCCESS;

    s32Ret = RK_MPI_AO_SetPubAttr(AoDev, pstAoAttr);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with 0x%x!", s32Ret);
        return s32Ret;
    }

    s32Ret = RK_MPI_AO_Enable(AoDev);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with 0x%x!", s32Ret);
        return s32Ret;
    }

    for (RK_S32 i = 0; i < u32ChnCnt; i++) {
        s32Ret =  RK_MPI_AO_EnableChn(AoDev, i);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("failed with 0x%x!", s32Ret);
            return s32Ret;
        }

        // set sample rate of input data
        s32Ret = RK_MPI_AO_EnableReSmp(AoDev, i, pEnSmpRates[i]);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("failed with 0x%x!", s32Ret);
            return s32Ret;
        }
    }

    return RK_SUCCESS;
}

RK_S32 TEST_AO_Stop(AUDIO_DEV aoDevId, RK_U32 u32ChnCnt) {
    RK_S32 s32Ret = RK_SUCCESS;

    for (RK_S32 i = 0; i < u32ChnCnt; i++) {
        s32Ret = RK_MPI_AO_DisableReSmp(aoDevId, i);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("failed with 0x%x!", s32Ret);
            return RK_FAILURE;
        }

        s32Ret = RK_MPI_AO_DisableChn(aoDevId, i);
        if (s32Ret != RK_SUCCESS) {
            RK_LOGE("failed with 0x%x!", s32Ret);
            return RK_FAILURE;
        }
    }

    s32Ret =  RK_MPI_AO_Disable(aoDevId);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("failed with 0x%x!", s32Ret);
        return RK_FAILURE;
    }

    return RK_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

