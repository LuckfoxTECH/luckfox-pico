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

#ifndef SRC_RT_MEDIA_INCLUDE_RTMEDIABUFFERDEF_H_
#define SRC_RT_MEDIA_INCLUDE_RTMEDIABUFFERDEF_H_

#include "rt_error.h"

typedef enum _RTMediaBufferType {
    RT_MB_TYPE_BASE = 0,
    RT_MB_TYPE_VPKT = 1,
    RT_MB_TYPE_VFRAME = 2,
    RT_MB_TYPE_APKT = 3,
    RT_MB_TYPE_AFRAME = 4,
} RTMediaBufferType;

typedef enum _RTMBFlags {
    RT_MB_FLAG_NONE = 0,
    RT_MB_FLAG_EOS  = (1 << 1),
    RT_MB_FLAG_ERROR = (1 << 2),
    RT_MB_FLAG_DROP_IF_FULL = (1 << 3),
    RT_MB_FLAG_EXT_MB = (1 << 4),
    // add more
    RT_MB_FLAG_BUTT  = 0xffffffff
} RTMBFlags;

enum RtMediaBufferStatus {
    RT_MEDIA_BUFFER_STATUS_UNKNOWN = 0,
    RT_MEDIA_BUFFER_STATUS_UNUSED,
    RT_MEDIA_BUFFER_STATUS_READY,
    RT_MEDIA_BUFFER_STATUS_USING,
    RT_MEDIA_BUFFER_STATUS_INFO_CHANGE,
    RT_MEDIA_BUFFER_STATUS_PAST,
    RT_MEDIA_BUFFER_STATUS_UNSUPPORT,
    RT_MEDIA_BUFFER_STATUS_REALLOC,
    RT_MEDIA_BUFFER_STATUS_BOTTOM,
};

enum RTMediaBufferSite {
    RT_MEDIA_BUFFER_SITE_BY_US = 0,
    RT_MEDIA_BUFFER_SITE_DECODER,
    RT_MEDIA_BUFFER_SITE_RENDER,
    RT_MEDIA_BUFFER_SITE_ABANDON,
    RT_MEDIA_BUFFER_SITE_BOTTOM,
};

typedef RT_RET (*UserDataFree)(void *);

typedef struct _RTMBUserData {
    UserDataFree    freeFunc;
    void           *param;
} RTMBUserData;

#endif  // SRC_RT_MEDIA_INCLUDE_RTMEDIABUFFERDEF_H_
