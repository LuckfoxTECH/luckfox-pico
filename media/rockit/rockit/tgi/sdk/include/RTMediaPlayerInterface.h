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
 * author: rimon.xu@rock-chips.com
 *              fxw@rock-chips.com
 *   date: 2020-04-27
 * module: RTMediaPlayerInterface
 */

#ifndef INCLUDE_RT_PLAYER_RTMEDIAPLAYERINTERFACE_H_
#define INCLUDE_RT_PLAYER_RTMEDIAPLAYERINTERFACE_H_

#include <sys/types.h>
#include <inttypes.h>
#include "rt_metadata.h"      // NOLINT

class IMediaDataSource;

class RTPlayerListener {
 public:
    virtual ~RTPlayerListener() {}
    virtual void notify(INT32 msg, INT32 ext1, INT32 ext2, void* ptr) = 0;
};

class RTMediaPlayerInterface {
 public:
    RTMediaPlayerInterface() {}
    virtual ~RTMediaPlayerInterface() {}

    /* parameter operations */
    virtual rt_status setUID(uid_t uid) = 0;
    virtual rt_status setDataSource(const char *url, const char *headers) = 0;
    virtual rt_status setDataSource(INT32 fd, INT64 offset, INT64 length) = 0;
    virtual rt_status setDataSource(IMediaDataSource *source) = 0;
    virtual rt_status setLooping(INT32 loop) = 0;
    virtual rt_status setListener(RTPlayerListener *listener) = 0;

    /* control operations */
    virtual rt_status initCheck() = 0;
    virtual rt_status prepare() = 0;
    virtual rt_status prepareAsync() = 0;
    virtual rt_status seekTo(INT64 usec) = 0;
    virtual rt_status start() = 0;
    virtual rt_status stop() = 0;
    virtual rt_status pause() = 0;
    virtual rt_status reset() = 0;
    virtual rt_status wait(INT64 timeUs = 0) = 0;

    /* parameter query operations */
    virtual rt_status getState() = 0;
    virtual rt_status getCurrentPosition(INT64 *usec) = 0;
    virtual rt_status getDuration(INT64 *usec) = 0;
    virtual rt_status dump(INT32 fd, const char *args) = 0;

    /* advanced query/control operations */
    virtual rt_status invoke(RtMetaData *request, RtMetaData *reply) = 0;
    virtual rt_status setParameter(INT32 key, RtMetaData *request) = 0;
    virtual rt_status getParameter(INT32 key, RtMetaData *reply) = 0;

    /* configuration video/audio/subtitle sink */
    virtual rt_status setVideoSink(const void *videoSink) = 0;
    virtual rt_status setAudioSink(const void *audioSink) = 0;
    virtual rt_status setSubteSink(const void *subteSink) = 0;
};

#endif  // INCLUDE_RT_PLAYER_RTMEDIAPLAYERINTERFACE_H_
