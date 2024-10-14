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

#ifndef SRC_RT_MEDIA_INCLUDE_RTAUDIOFRAME_H_
#define SRC_RT_MEDIA_INCLUDE_RTAUDIOFRAME_H_

#include "RTAVShellBuffer.h"

class RTAudioFrame : public RTAVShellBuffer {
 public:
    RTAudioFrame();
    RTAudioFrame(const RTAudioFrame& in);
    RTAudioFrame& operator=(const RTAudioFrame& in);

    virtual ~RTAudioFrame() = default;

    virtual RTMediaBufferType getType();
    virtual void reset();

    RTAudioFormat getAudioFormat() const;
    INT32  getSoundMode() const;
    INT32  getChannels() const;
    INT32  getSampleRate() const;
    INT64  getChannelLayout() const;
    INT32  getFrameNum() const;
    INT32  getDuration() const;
    UINT64 getPts() const;
    UINT64 getDts() const;
    UINT32 getSeq() const;
    UINT32 getReadSize() const;

    void  setAudioFormat(const RTAudioFormat audioFormat);
    void  setSoundMode(const INT32 soundMode);
    void  setChannels(INT32 channels);
    void  setSampleRate(INT32 sampleRate);
    void  setChannelLayout(INT64 channelLayout);
    void  setFrameNum(INT32 frames);
    void  setDuration(INT32 duration);
    void  setPts(const UINT64 pts);
    void  setDts(const UINT64 dts);
    void  setSeq(const UINT32 seq);
    void  setReadSize(const UINT32 size);
    void  clone(const RTAudioFrame& in);

 private:
    void baseInit();

 private:
    RTAudioFormat mAudioFormat;
    INT32         mSoundMode;
    UINT64        mPts;
    UINT64        mDts;
    INT32         mChannels;
    INT32         mSampleRate;
    INT64         mChannelLayout;
    INT32         mDuration;
    UINT32        mSeq;
    UINT32        mFrameNum;
    UINT32        mReadSize;
};

RTAudioFrame* reinterpret_aframe(RTMediaBuffer *buffer);
RTAudioFrame* construct_aframe(RTMediaBuffer *buffer);

#endif  // SRC_RT_MEDIA_INCLUDE_RTAUDIOFRAME_H_

