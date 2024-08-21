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
 * author: hh@rock-chips.com
 *   date: 2020-5-19
 * module: video filter with rknn/rockx/rockface
 */
#ifndef SRC_RT_MEDIA_AV_FILTER_INCLUDE_RTMEDIAROCKX_H_
#define SRC_RT_MEDIA_AV_FILTER_INCLUDE_RTMEDIAROCKX_H_

#include "rt_type.h"                    // NOLINT
#include <string>

#ifdef HAVE_ROCKX
#include <rockx/rockx.h>
#endif

#define ROCKX_FACE_DETECT       "rockx_face_detect"
#define ROCKX_FACE_LANDMARK     "rockx_face_landmark"
#define ROCKX_POSE_BODY         "rockx_pose_body"
#define ROCKX_POSE_BODY_V2      "rockx_pose_body_v2"
#define ROCKX_POSE_FINGER       "rockx_pose_finger"
#define ROCKX_FACE_GENDER_AGE   "rockx_face_gender_age"


typedef struct _RTRockxCfg {
    //  path of "librockx.so"
    char *path;
    //  model which is will be loaded by rknn
    char *model;
    //  format of input datas
    char *format;
    //  width of input datas
    INT32 width;
    //  width of input datas
    INT32 height;

    // add more
} RTRockxCfg;

typedef void (*RknnCallBack) (void* handler, int type, void *ptr, int size);
typedef void* RknnHandler;


typedef struct {
#ifdef HAVE_ROCKFACE
    rockface_det_t base;
    rockface_attribute_t attr;
    rockface_landmark_t landmark;
    rockface_angle_t angle;
    rockface_feature_t feature;
#endif

#ifdef HAVE_ROCKX
    rockx_object_t object;
#endif
} RTFaceInfo;

typedef struct {
#ifdef HAVE_ROCKX
    rockx_face_landmark_t object;
#endif
} RTLandmarkInfo;

typedef struct {
#ifdef HAVE_ROCKFACE
    rockface_det_t base;
#endif

#ifdef HAVE_ROCKX
    rockx_keypoints_t object;
#endif
} RTBodyInfo;

typedef struct {
#ifdef USE_ROCKX
    rockx_keypoints_t object;
#endif
} RTFingerInfo;

typedef enum {
    RT_SUCCESS = 0,
    RT_FAILURE,
    RT_TIMEOUT,
    RT_UNKNOW,
} RTAuthorizedStatus;

typedef enum {
    RT_RKNN_TYPE_NONE = -1,
    RT_RKNN_TYPE_FACE = 0,
    RT_RKNN_TYPE_BODY,
    RT_RKNN_TYPE_FINGER,
    RT_RKNN_TYPE_LANDMARK,
    RT_RKNN_TYPE_AUTHORIZED_STATUS,
} RTRknnResultType;

typedef struct {
    INT32 index;
    INT32 img_w;
    INT32 img_h;
    INT64 timeval;
    RTRknnResultType type;
    RTAuthorizedStatus status;
    union {
        RTBodyInfo body_info;
        RTFaceInfo face_info;
        RTLandmarkInfo landmark_info;
        RTFingerInfo   finger_info;
    };
} RTRknnResult;

typedef struct {
    // how many result in results poiont
    INT32 counter;
    // the results of rknn output
    RTRknnResult* results;
} RTRknnAnalysisResults;

typedef struct {
    INT32 dataSize;
    INT32 width;
    INT32 height;
    INT32 format;
    INT32 angle;
    INT32 mirror;
    INT32 faceID;
    unsigned char *feature;
    INT32 featureLen;
} RTKKMattingFaceInfo;

typedef struct {
    INT32 faceCount;
    RTKKMattingFaceInfo *faceInfo;
} RTKKAIMattingResult;

#endif  // SRC_RT_MEDIA_AV_FILTER_INCLUDE_RTMEDIAROCKX_H_

