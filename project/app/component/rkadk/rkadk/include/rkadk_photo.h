/*
 * Copyright (c) 2021 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef __RKADK_PHOTO_H__
#define __RKADK_PHOTO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "rk_comm_video.h"
#include "rkadk_common.h"

#define RKADK_MPF_LARGE_THUMB_NUM_MAX 2

/** photo type enum */
typedef enum {
  RKADK_PHOTO_TYPE_SINGLE = 0,
  RKADK_PHOTO_TYPE_MULTIPLE,
  RKADK_PHOTO_TYPE_LAPSE, // TODO
  RKADK_PHOTO_TYPE_BUTT
} RKADK_PHOTO_TYPE_E;

typedef enum {
  RKADK_JPG_THUMB_TYPE_DCF,
  RKADK_JPG_THUMB_TYPE_MFP1,
  RKADK_JPG_THUMB_TYPE_MFP2,
  RKADK_JPG_THUMB_TYPE_BUTT
} RKADK_JPG_THUMB_TYPE_E;

typedef enum {
  RKADK_PHOTO_MPF_SINGLE = 0,
  RKADK_PHOTO_MPF_MULTI,
  RKADK_PHOTO_MPF_BUTT
} RKADK_PHOTO_MPF_MODE_E;

typedef RKADK_THUMB_ATTR_S RKADK_PHOTO_DATA_ATTR_S;

/** single photo attr */
typedef struct {
  // TODO
  /* s32TimeSec is 0 that means photo immediately, larger than 0 that means
   * photo delay s32TimeSec second */
  RKADK_S32 s32Time_sec;
} RKADK_PHOTO_SINGLE_ATTR_S;

/** lapse photo attr */
typedef struct {
  RKADK_S32 s32Interval_ms; /* unit: millisecond */
} RKADK_PHOTO_LAPSE_ATTR_S;

/** burst photo attr */
typedef struct {
  /* s32Count is -1 that means continuous photo, larger than 0 that meas photo
   * number */
  RKADK_S32 s32Count;
} RKADK_PHOTO_MULTIPLE_ATTR_S;

/* photo thumbnail MPF config */
typedef struct {
  RKADK_U8 u8LargeThumbNum;
  SIZE_S astLargeThumbSize[RKADK_MPF_LARGE_THUMB_NUM_MAX];
} RKADK_PHOTO_MPF_CFG_S;

/* photo thumbnail MPF attr */
typedef struct {
  RKADK_PHOTO_MPF_MODE_E eMode;
  RKADK_PHOTO_MPF_CFG_S sCfg;
} RKADK_PHOTO_MPF_ATTR_S;

/* photo thumbnail attr */
typedef struct {
  RKADK_BOOL bSupportDCF; /* Fixed resolution: 160 * 120 jpg */
  RKADK_PHOTO_MPF_ATTR_S stMPFAttr;
} RKADK_PHOTO_THUMB_ATTR_S;

/* photo recv data */
typedef struct {
  RKADK_U8 *pu8DataBuf;
  RKADK_U32 u32DataLen;
  RKADK_U32 u32CamId;
} RKADK_PHOTO_RECV_DATA_S;

/* photo data recv callback */
typedef void (*RKADK_PHOTO_DATA_RECV_FN_PTR)(RKADK_PHOTO_RECV_DATA_S *pstData);

typedef struct {
  RKADK_PHOTO_TYPE_E enPhotoType;
  union tagPhotoTypeAttr {
    RKADK_PHOTO_SINGLE_ATTR_S stSingleAttr;
    RKADK_PHOTO_LAPSE_ATTR_S stLapseAttr; // TODO
    RKADK_PHOTO_MULTIPLE_ATTR_S stMultipleAttr;
  } unPhotoTypeAttr;
} RKADK_TAKE_PHOTO_ATTR_S;

typedef struct {
  RKADK_U32 u32CamId; /** cam id, 0--front 1--rear */
  RKADK_PHOTO_THUMB_ATTR_S stThumbAttr;
  RKADK_PHOTO_DATA_RECV_FN_PTR pfnPhotoDataProc;
} RKADK_PHOTO_ATTR_S;

/****************************************************************************/
/*                            Interface Definition                          */
/****************************************************************************/
/**
 * @brief init photo, it should be called first
 * @param[in] pstPhotoAttr: photo attribute
 * @return 0 success, non-zero error code.
 */
RKADK_S32 RKADK_PHOTO_Init(RKADK_PHOTO_ATTR_S *pstPhotoAttr,
                           RKADK_MW_PTR *ppHandle);

/**
 * @brief deinit photo
 * @param[in] u32CamId: camera id
 * @return 0 success, non-zero error code.
 */
RKADK_S32 RKADK_PHOTO_DeInit(RKADK_MW_PTR pHandle);

/**
 * @brief take photo
 * @param[in] pstPhotoAttr: photo attribute
 * @return 0 success, non-zero error code.
 */
RKADK_S32 RKADK_PHOTO_TakePhoto(RKADK_MW_PTR pHandle,
                                RKADK_TAKE_PHOTO_ATTR_S *pstAttr);

/**
 * @brief get thumbnail in jpg
 * @param[in] pszFileName: file name
 * @param[in] type: thumbnail type
 * @param[out] pu8Buf: thumbnail data
 * @param[in/out] pu32Size: in: pu8Buf size, out: thumbnail size
 * @return 0 success, non-zero error code.
 */
RKADK_S32 RKADK_PHOTO_GetThmInJpg(RKADK_U32 u32CamId, RKADK_CHAR *pszFileName,
                                  RKADK_JPG_THUMB_TYPE_E eThmType,
                                  RKADK_U8 *pu8Buf, RKADK_U32 *pu32Size);

RKADK_S32 RKADK_PHOTO_GetThmInJpgEx(RKADK_U32 u32CamId, RKADK_CHAR *pszFileName,
                                    RKADK_JPG_THUMB_TYPE_E eThmType,
                                    RKADK_THUMB_ATTR_S *pstThumbAttr);

RKADK_S32 RKADK_PHOTO_ThumbBufFree(RKADK_THUMB_ATTR_S *pstThumbAttr);

RKADK_S32 RKADK_PHOTO_Reset(RKADK_MW_PTR *pHandle);

#ifdef __cplusplus
}
#endif
#endif
