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

#ifndef SRC_RT_MEDIA_INCLUDE_RTVFRAMEBUFFER_H_
#define SRC_RT_MEDIA_INCLUDE_RTVFRAMEBUFFER_H_

#include <map>

#include "RTAVShellBuffer.h"

class RTVideoFrame : public RTAVShellBuffer {
 public:
    RTVideoFrame();
    explicit RTVideoFrame(UINT32 size);
    virtual ~RTVideoFrame() = default;
    RTVideoFrame& operator=(const RTVideoFrame& in);

    virtual RTMediaBufferType getType();
    virtual void reset();

    UINT32 getWidth() const;
    UINT32 getHeight() const;
    UINT32 getVirWidth() const;
    UINT32 getVirHeight() const;
    RTPixelFormat getPixelFormat() const;
    RTCompressMode getCompressMode() const;
    RTRect getOpRect() const;
    RTRect getBaseRect() const;
    RTRect getRect() const;
    INT64 getPts() const;
    INT64 getDts() const;
    INT32 getSeq() const;
    INT32 getDuration() const;
    INT32 getRotation() const;

    void  setWidth(const UINT32 width);
    void  setHeight(const UINT32 height);
    void  setVirWidth(const UINT32 virWidth);
    void  setVirHeight(const UINT32 virHeight);
    void  setPixelFormat(const RTPixelFormat pixelFormat);
    void  setCompressMode(const RTCompressMode mode);
    void  setOpRect(const RTRect rect);
    void  setBaseRect(const RTRect rect);
    void  setPts(const INT64 pts);
    void  setDts(const INT64 dts);
    void  setSeq(const INT32 seq);
    void  setDuration(const INT32 duration);
    void  setRotation(const INT32 rotation);
    void  clone(const RTVideoFrame& in);

 private:
    void baseInit();

 private:
     UINT32 mWidth;
     UINT32 mHeight;
     UINT32 mVirWidth;
     UINT32 mVirHeight;
     RTRect mOpRect;
     RTRect mBaseRect;
     INT64  mPts;
     INT64  mDts;
     INT32  mSeq;
     INT32  mDuration;
     INT32  mRotation;
     RTPixelFormat mPixelFormat;
     RTCompressMode mCompressMode;
};

RTVideoFrame* clone_vframe(RTVideoFrame *buffer);
RTVideoFrame* reinterpret_vframe(RTMediaBuffer *buffer);
RTVideoFrame* construct_vframe(RTMediaBuffer *buffer);

#endif  // SRC_RT_MEDIA_INCLUDE_RTVFRAMEBUFFER_H_
