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
 *
 */
#ifndef _SAMPLE_ASSIST_H_
#define _SAMPLE_ASSIST_H_

	
#include "rk_comm_ive.h"
#include "rk_mpi_ive.h"

#define CLIP(a, maxv, minv)		 (((a)>(maxv)) ? (maxv) : (((a) < (minv)) ? (minv) : (a)))

RK_U32 RK_CalcStride(RK_U32 u32Width, RK_U32 u32Align);
RK_S32 RK_CreateIveImage(IVE_IMAGE_S *pstImage,IVE_IMAGE_TYPE_E enType, RK_U32 u32Width, RK_U32 u32Height);
RK_S32 RK_DestroyIveImage(IVE_IMAGE_S *pstImage);
RK_S32 RK_CreateIveMem(IVE_MEM_INFO_S *pstImage, RK_U32 u32Size);
RK_S32 RK_DestroyIveMem(IVE_MEM_INFO_S *pstImage);
RK_S32 RK_CreateIveData(IVE_DATA_S *pstImage, RK_U32 u32Width, RK_U32 u32Height);
RK_S32 RK_DestroyIveData(IVE_DATA_S *pstImage);
RK_S64 RK_GetTickCount(RK_VOID);
RK_DOUBLE RK_GetTickFrequency(RK_VOID);
RK_S32 RK_GenRand(RK_S32 s32Max,RK_S32 s32Min);

#endif /*_SAMPLE_ASSIST_H_*/