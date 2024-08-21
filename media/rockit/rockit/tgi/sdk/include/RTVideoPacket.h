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

#ifndef SRC_RT_MEDIA_INCLUDE_RTVSTREAMBUFFER_H_
#define SRC_RT_MEDIA_INCLUDE_RTVSTREAMBUFFER_H_

#include "RTAVShellBuffer.h"

class RTVideoPacket : public RTAVShellBuffer {
 public:
    RTVideoPacket();
    virtual ~RTVideoPacket() = default;

    virtual RTMediaBufferType getType();
    virtual void reset();

    UINT64    getPts() const;
    UINT64    getDts() const;
    INT32     getEndOfFrame() const;
    INT32     getTimeout() const;
    INT32     getDuration() const;
    INT32     getSeq() const;
    INT32     getIsExtraData() const;

    void  setPts(const UINT64 pts);
    void  setDts(const UINT64 dts);
    void  setEndOfFrame(const INT32 endOfFrame);
    void  setTimeout(const INT32 timeout);
    void  setDuration(const INT32 duration);
    void  setSeq(const INT32 seq);
    void  setIsExtraData(const INT32 isExtraData);

 private:
    void baseInit();

 private:
    UINT64  mPts;
    UINT64  mDts;
    INT32   mSeq;
    INT32   mEndOfFrame;
    INT32   mTimeout;
    INT32   mDuration;
    INT32   mIsExtraData;
};

RTVideoPacket* reinterpret_vpacket(RTMediaBuffer *buffer);
RTVideoPacket* construct_vpacket(RTMediaBuffer *buffer);

#endif  // SRC_RT_MEDIA_INCLUDE_RTVSTREAMBUFFER_H_
