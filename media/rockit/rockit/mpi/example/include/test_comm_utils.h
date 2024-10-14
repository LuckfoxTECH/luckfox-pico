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

#ifndef SRC_TESTS_RT_MPI_COMMON_TEST_COMM_UTILS_H_
#define SRC_TESTS_RT_MPI_COMMON_TEST_COMM_UTILS_H_

#include "rk_type.h"
#include "rk_debug.h"
#include "rk_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define CHECK_CHN_RET(express, chn, name)   \
    do {                                    \
        RK_S32 s32Ret;                      \
        s32Ret = express;                   \
        if (RK_SUCCESS != s32Ret) {         \
            RK_LOGE("\033[0;31m%s chn %d failed with %#x!\033[0;39m", name, chn, s32Ret); \
            return s32Ret;                  \
        }                                   \
    } while (0)

#define CHECK_RET(express, name)    \
    do {                            \
        RK_S32 s32Ret;              \
        s32Ret = express;           \
        if (RK_SUCCESS != s32Ret) { \
            RK_LOGE("\033[0;31m%s failed with %#x!\033[0;39m", name, s32Ret); \
            return s32Ret;          \
        }                           \
    } while (0)

#define RK_ARRAY_ELEMS(a)      (sizeof(a) / sizeof((a)[0]))
#define RK_ALIGN(x, a)         (((x) + (a) - 1) & ~((a) - 1))
#define RK_ALIGN_16(x)         RK_ALIGN(x, 16)
#define RK_ALIGN_64(x)         RK_ALIGN(x, 64)
#define RK_ALIGN_256(x)        RK_ALIGN(x, 256)
#define RK_ALIGN_256_ODD(x)    (RK_ALIGN(x, 256) | 256)

typedef enum _RTUriSchemeType {
    RK_URI_SCHEME_NONE = 0,
    RK_URI_SCHEME_LOCAL,
    RK_URI_SCHEME_HTTP,
    RK_URI_SCHEME_RTSP,
    RK_URI_SCHEME_SAMBA,
    RK_URI_SCHEME_HLS,
    RK_URI_SCHEME_RTP,
    RK_URI_SCHEME_UDP,
    RK_URI_SCHEME_MMS,
    RK_URI_SCHEME_MMSH,
    RK_URI_SCHEME_MMST,
    RK_URI_SCHEME_TTS,
    RK_URI_SCHEME_IPTV,
} URI_SCHEME_TYPE;

typedef enum _RKSocType {
    RK_SOC_UNKOWN = 0,
    RK_SOC_1126,
    RK_SOC_3568,
    RK_SOC_3588
} RKSocType;

RK_U64 TEST_COMM_GetNowUs();
URI_SCHEME_TYPE TEST_COMM_GetUriSchemeType(const char* uri);
RK_S32 TEST_COMM_OpenFileUris(const char *pCfgFileUri, char **pFileUris, RK_U32 u32UriCount);
void TEST_COMM_CloseFileUris(char **pFileUris, RK_U32 u32UriCount);
void TEST_COMM_MkDirs(const char *muldir, RK_S32 mode);
RK_S32 TEST_COMM_CheckFileSizeInRange(const char *pFilePath, RK_S32 minSize, RK_S32 maxSize);

RK_S32 TEST_COMM_FileReadOneFrame(const char *pFileName, VIDEO_FRAME_INFO_S *pstVideoFrame);
RK_S32 TEST_COMM_FileWriteOneFrame(const char *pFileName, VIDEO_FRAME_INFO_S *pstVideoFrame);
RKSocType TEST_COMM_GetSocType();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif  // SRC_TESTS_RT_MPI_COMMON_TEST_COMM_UTILS_H_

