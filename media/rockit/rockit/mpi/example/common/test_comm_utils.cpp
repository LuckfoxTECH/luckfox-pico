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
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <cstring>
#include <stdlib.h>
#include <cerrno>
#include <unistd.h>

#include "test_comm_utils.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_sys.h"
#include "rk_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
static RKSocType gEnRkSocType;

RK_U64 TEST_COMM_GetNowUs() {
    struct timespec time = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

URI_SCHEME_TYPE TEST_COMM_GetUriSchemeType(const char* uri) {
    URI_SCHEME_TYPE schemeType = RK_URI_SCHEME_LOCAL;
    if ((RK_NULL == uri) || (strlen(uri) < 4)) {
        return schemeType;
    }

    if (!strncmp("http://", uri, 7) || !strncmp("https://", uri, 8)) {
        schemeType = RK_URI_SCHEME_HTTP;
    } else if (!strncmp("rtsp://", uri, 7) || !strncmp("rtmp://", uri, 7)) {
        RK_LOGD("uri is with rtsp or rtmp scheme type");
        schemeType = RK_URI_SCHEME_RTSP;
    } else if (!strncmp("/data/smb/", uri, 10)) {
        RK_LOGD("uri is with /data/smb scheme type");
        schemeType = RK_URI_SCHEME_SAMBA;
    } else if (!strncmp("/data/nfs/", uri, 10)) {
        RK_LOGD("uri is with /data/nfs schemeType (signed as samba)");
        schemeType = RK_URI_SCHEME_SAMBA;
    } else if (strstr(uri, "m3u8")) {
        RK_LOGD("uri is with m3u8 scheme type");
        schemeType = RK_URI_SCHEME_HLS;
    } else if (!strncmp("rtp:", uri, 4)) {
        RK_LOGD("uri is with rtp scheme type");
        schemeType = RK_URI_SCHEME_RTP;
    } else if (!strncmp("udp:", uri, 4)) {
        RK_LOGD("uri is with udp scheme type");
        schemeType = RK_URI_SCHEME_UDP;
    } else if (!strncmp("mms://", uri, 6)) {
        RK_LOGD("uri is with mms scheme type");
        schemeType = RK_URI_SCHEME_MMS;
    } else if (!strncmp("mmsh://", uri, 7)) {
        RK_LOGD("uri is with mmsh scheme type");
        schemeType = RK_URI_SCHEME_MMSH;
    } else if (!strncmp("mmst://", uri, 7)) {
        RK_LOGD("uri is with mmst scheme type");
        schemeType = RK_URI_SCHEME_MMST;
    } else if (strstr(uri, "app_tts-cache")) {
        RK_LOGD("uri is with tts scheme type");
        schemeType = RK_URI_SCHEME_TTS;
    }  else if (strstr(uri, "cache://")) {
        schemeType = RK_URI_SCHEME_IPTV;
    }
    return schemeType;
}

RK_S32 TEST_COMM_OpenFileUris(const char *pCfgFileUri, char **pFileUris, RK_U32 u32UriCount) {
    FILE *fp = RK_NULL;
    RK_U32 u32Count = 0;
    RK_U32 u32FileLen = 0;
    RK_U32 u32Len = 0;
    RK_S32 s32Ret = 0;
    char *pLine = RK_NULL;
    char *pBuffer = RK_NULL;
    size_t sLen = 0;
    size_t sReadLen = 0;

    if ((fp = fopen(pCfgFileUri, "r")) == RK_NULL) {
        RK_LOGE("Error! opening file %s error is %s", pCfgFileUri, strerror(errno));
        return RK_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    u32FileLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while (sReadLen = getline(&pLine, &sLen, fp) != -1) {
        pBuffer = reinterpret_cast<char *>(malloc(strlen(pLine) + 1));
        RK_S32 len = strlen(pLine);
        snprintf(pBuffer, strlen(pLine), "%s", pLine);
        while (len) {
            if (pBuffer[len] == '\r') {
                char *tmp = reinterpret_cast<char *>(malloc(strlen(pLine) - len + 1));
                memcpy(tmp, &(pBuffer[len + 1]), strlen(pLine) - len);
                memcpy(&(pBuffer[len]), tmp, strlen(pLine) - len);
                free(tmp);
            }
            len--;
        }
        pFileUris[u32Count] = pBuffer;
        free(pLine);
        sLen = RK_NULL;

        RK_LOGV("url %s", pFileUris[u32Count]);
        u32Count++;

        if (u32Count >= u32UriCount)
            break;
    }

    fclose(fp);

    return RK_SUCCESS;
}

void TEST_COMM_CloseFileUris(char **pFileUris, RK_U32 u32UriCount) {
    RK_U32 u32Index = 0;

    for (u32Index = 0; u32Index < u32UriCount; u32Index++) {
        if (pFileUris[u32Index] != RK_NULL) {
            free(pFileUris[u32Index]);
            pFileUris[u32Index] = RK_NULL;
        }
    }
}

RK_S32 TEST_COMM_FileReadOneFrame(const char *pFileName, VIDEO_FRAME_INFO_S *pstVideoFrame) {
    RK_U32 u32ReadSize = 0;
    RK_S32 s32Ret = RK_SUCCESS;
    FILE *fp = RK_NULL;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;

    if (!pFileName || !pstVideoFrame) {
        return RK_FAILURE;
    }

    stPicBufAttr.u32Width = pstVideoFrame->stVFrame.u32VirWidth;
    stPicBufAttr.u32Height = pstVideoFrame->stVFrame.u32VirHeight;
    stPicBufAttr.enPixelFormat = pstVideoFrame->stVFrame.enPixelFormat;
    stPicBufAttr.enCompMode = pstVideoFrame->stVFrame.enCompressMode;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_CAL_VGS_GetPicBufferSize failed. err=0x%x", s32Ret);
        return s32Ret;
    }

    fp = fopen(pFileName, "rb");
    if (fp == RK_NULL) {
        RK_LOGE("fopen %s failed, error: %s", pFileName, strerror(errno));
        s32Ret = RK_FAILURE;
        goto __FAILED;
    }
    u32ReadSize = fread(RK_MPI_MB_Handle2VirAddr(pstVideoFrame->stVFrame.pMbBlk),
                        1, stMbPicCalResult.u32MBSize, fp);
    if (u32ReadSize == 0) {
        RK_LOGE("fread %p failed! request %d", pFileName, stMbPicCalResult.u32MBSize);
        s32Ret = RK_FAILURE;
        goto __FAILED;
    }

    RK_MPI_SYS_MmzFlushCache(pstVideoFrame->stVFrame.pMbBlk, RK_FALSE);

__FAILED:
    if (fp) {
        fclose(fp);
    }

    return s32Ret;
}

RK_S32 TEST_COMM_FileWriteOneFrame(const char *pFileName, VIDEO_FRAME_INFO_S *pstVideoFrame) {
    RK_S32 s32Ret = RK_SUCCESS;
    FILE *fp = RK_NULL;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;

    if (!pFileName || !pstVideoFrame) {
        return RK_FAILURE;
    }

    stPicBufAttr.u32Width = pstVideoFrame->stVFrame.u32VirWidth;
    stPicBufAttr.u32Height = pstVideoFrame->stVFrame.u32VirHeight;
    stPicBufAttr.enPixelFormat = pstVideoFrame->stVFrame.enPixelFormat;
    stPicBufAttr.enCompMode = pstVideoFrame->stVFrame.enCompressMode;
    s32Ret = RK_MPI_CAL_VGS_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS) {
        RK_LOGE("RK_MPI_CAL_VGS_GetPicBufferSize failed. err=0x%x", s32Ret);
        return s32Ret;
    }

    RK_MPI_SYS_MmzFlushCache(pstVideoFrame->stVFrame.pMbBlk, RK_TRUE);

    fp = fopen(pFileName, "wb");
    if (fp == RK_NULL) {
        RK_LOGE("open %s failed, error: %s", pFileName, strerror(errno));
        s32Ret = RK_FAILURE;
        goto __FAILED;
    }
    fwrite(RK_MPI_MB_Handle2VirAddr(pstVideoFrame->stVFrame.pMbBlk),
            1, stMbPicCalResult.u32MBSize, fp);
    fflush(fp);

__FAILED:
    if (fp) {
        fclose(fp);
    }

    return RK_SUCCESS;
}

void TEST_COMM_MkDirs(const char *muldir, RK_S32 mode) {
    RK_S32 i, len;
    char str[512];

    strncpy(str, muldir, 512);
    len = strlen(str);
    for (i = 0; i < len; i++) {
        if (str[i] == '/') {
            str[i] = '\0';
            if (access(str, 0)) {
                mkdir(str, mode);
            }
            str[i]='/';
        }
    }
    if (len > 0 && access(str, 0)) {
        mkdir(str, mode);
    }

    return;
}

RK_S32 TEST_COMM_CheckFileSizeInRange(const char *pFilePath,
                                                          RK_S32 minSize,
                                                          RK_S32 maxSize) {
    struct stat st;
    FILE *fp;

    if (access(pFilePath, 0)) {
        RK_LOGE("can't find file %s!", pFilePath);
        return RK_FAILURE;
    }

    if (stat(pFilePath, &st) == 0) {
        if (st.st_size >= minSize && st.st_size <= maxSize) {
            return RK_SUCCESS;
        } else {
            RK_LOGE("file %s size:%lld is out range[%d,%d]!",
                     pFilePath, st.st_size, minSize, maxSize);
            return RK_FAILURE;
        }
    } else {
        RK_LOGE("can't determine size of %s: %s!", pFilePath, strerror(errno));
        return RK_FAILURE;
    }
}

RKSocType TEST_COMM_GetSocType() {
    char achBuf[1024] = "";
    FILE *fd = NULL;
    RKSocType socType = RK_SOC_1126;

    if (gEnRkSocType)
        return gEnRkSocType;

    fd = fopen("/proc/device-tree/compatible", "r");
    memset(achBuf, 0, sizeof(achBuf));
    if (fd != NULL) {
        fread(achBuf, 1, sizeof(achBuf), fd);
        if ((strstr(achBuf, "rockchip")) ||
            (strstr(achBuf, "Rockchip")) ||
            (strstr(achBuf, "RK30board"))) {
            if (strstr(achBuf, "1126") || (strstr(achBuf, "1109")))
                socType = RK_SOC_1126;
            else if (strstr(achBuf, "3566") || (strstr(achBuf, "3568")))
                socType = RK_SOC_3568;
            else if (strstr(achBuf, "3588"))
                socType = RK_SOC_3588;
        } else {
            RK_LOGE("not match rockchips device.");
        }
        fclose(fd);
    } else {
        RK_LOGE("open failed.");
    }
    gEnRkSocType = socType;
    RK_LOGD("get the rkchip:%d.", socType);

    return socType;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
