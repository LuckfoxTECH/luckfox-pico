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

#include "rk_mpi_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef ARRAY_LENGTH
#define ARRAY_LENGTH(a) (sizeof (a) / sizeof (a)[0])
#endif

typedef struct rkVO_Timing_S {
    RK_S32  enIntfSync;
    RK_U32  u32Width;
    RK_U32  u32Height;
} VO_Timing_S;

static VO_Timing_S stVoTimings[] = {
    {VO_OUTPUT_640x480_60, 640, 480},
    {VO_OUTPUT_800x600_60, 800, 600},
    {VO_OUTPUT_1024x768_60, 1024, 768},
    {VO_OUTPUT_1280x1024_60, 1280, 1024},
    {VO_OUTPUT_1366x768_60, 1366, 768},
    {VO_OUTPUT_1440x900_60, 1440, 900},
    {VO_OUTPUT_1280x800_60, 1280, 800},
    {VO_OUTPUT_1600x1200_60, 1600, 1200},
    {VO_OUTPUT_1680x1050_60, 1680, 1050},
    {VO_OUTPUT_1920x1200_60, 1920, 1200},
};

void TEST_VO_GetDisplaySize(RK_S32 enIntfSync, RK_U32 *s32W, RK_U32 *s32H) {
    switch (enIntfSync) {
        case VO_OUTPUT_PAL:
        case VO_OUTPUT_576P50:
            *s32W = 720;
            *s32H = 576;
            break;
        case VO_OUTPUT_NTSC:
        case VO_OUTPUT_480P60:
            *s32W = 720;
            *s32H = 480;
            break;
        case VO_OUTPUT_720P50:
        case VO_OUTPUT_720P60:
            *s32W = 1280;
            *s32H = 720;
            break;
        case VO_OUTPUT_1080P24:
        case VO_OUTPUT_1080P25:
        case VO_OUTPUT_1080P30:
        case VO_OUTPUT_1080I50:
        case VO_OUTPUT_1080I60:
        case VO_OUTPUT_1080P50:
        case VO_OUTPUT_1080P60:
            *s32W = 1920;
            *s32H = 1080;
            break;
        case VO_OUTPUT_3840x2160_24:
        case VO_OUTPUT_3840x2160_25:
        case VO_OUTPUT_3840x2160_30:
        case VO_OUTPUT_3840x2160_50:
        case VO_OUTPUT_3840x2160_60:
            *s32W = 3840;
            *s32H = 2160;
            break;
        case VO_OUTPUT_4096x2160_24:
        case VO_OUTPUT_4096x2160_25:
        case VO_OUTPUT_4096x2160_30:
        case VO_OUTPUT_4096x2160_50:
        case VO_OUTPUT_4096x2160_60:
            *s32W = 4096;
            *s32H = 2160;
            break;
        default:
            for (RK_S32 i = 0; i < ARRAY_LENGTH(stVoTimings); i++) {
                if (stVoTimings[i].enIntfSync == enIntfSync) {
                    *s32W = stVoTimings[i].u32Width;
                    *s32H = stVoTimings[i].u32Height;
                    break;
                }
            }
            break;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
