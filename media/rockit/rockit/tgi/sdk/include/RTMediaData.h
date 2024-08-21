/*
 * Copyright 2019 Rockchip Electronics Co. LTD
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
 * Author: martin.cheng@rock-chips.com
 *   Date: 2019/01/22
 */

#ifndef SRC_RT_MEDIA_INCLUDE_RTMEDIADATA_H_
#define SRC_RT_MEDIA_INCLUDE_RTMEDIADATA_H_

#include "RTMediaDef.h"
#include "RTMediaMetaKeys.h"
#include "RTMediaPixel.h"

typedef RT_RET (*RT_RAW_FREE)(void*);

typedef struct _RTPacket {
    INT64    mPts;
    INT64    mDts;
    INT64    mPos;
    INT32    mTrackIndex;
    INT32    mFlags;
    INT64    mDuration;
    uint8_t *mData;
    INT32    mSize;
    INT32    mSerial;
    RT_BOOL  mKey;
    void*    mRawPtr;
    RTTrackType mType;
    RT_RAW_FREE mFuncFree;
} RTPacket;

typedef struct _RTFrame {
    INT64   mPts;               /* with unit of us*/
    INT64   mDts;               /* with unit of us*/
    INT32   mFrameW;
    INT32   mFrameH;
    INT32   mDisplayW;
    INT32   mDisplayH;
    INT32   mFormat;
    INT32   mDuration;
    INT32   mFrameEos;
    INT32   mFrameErr;

    /**
     * quality (between 1 (good) and FF_LAMBDA_MAX (bad))
     */
    INT32   mQuality;
    INT32   mFrameType;
    INT32   mFieldOrder;

    void*   mData;
    INT32   mSize;
    INT32   mSerial;
    void*   mRawPtr;
    RT_RAW_FREE mFuncFree;
} RTFrame;

class RTMediaBuffer;

/* utils for rt_packet */
RT_RET rt_utils_packet_free(RTPacket* rt_pkt);
RT_RET rt_mediabuf_from_packet(RTMediaBuffer* media_buf, RTPacket* rt_pkt);
RT_RET rt_mediabuf_goto_packet(RTMediaBuffer* media_buf, RTPacket* rt_pkt);

/* utils for rt_frame */
RT_RET rt_utils_frame_free(RTFrame* rt_frame);
RT_RET rt_mediabuf_from_frame(RTMediaBuffer* media_buf, RTFrame* rt_frame);
RT_RET rt_mediabuf_goto_frame(RTMediaBuffer* media_buf, RTFrame* rt_frame);

/* utils for track parameters */
RT_RET rt_medatdata_from_trackpar(RtMetaData* meta, RTTrackParam* tpar);
RT_RET rt_medatdata_goto_trackpar(RtMetaData* meta, RTTrackParam* tpar);
RT_RET rt_utils_dump_track(RtMetaData* meta, RT_BOOL full = false);

#endif  // SRC_RT_MEDIA_INCLUDE_RTMEDIADATA_H_
