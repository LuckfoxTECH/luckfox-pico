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
 * author: ywj@rock-chips.com
 *         fxw@rock-chips.com
 *   date: 2019/11/05
 * module: RTTVPlayerInterface
 */

#ifndef INCLUDE_RT_PLAYER_RTTVPLAYERINTERFACE_H_
#define INCLUDE_RT_PLAYER_RTTVPLAYERINTERFACE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <inttypes.h>
#include "rt_metadata.h"      // NOLINT

class IMediaDataSource;

class RTTVPlayerInterface {
 public:
    RTTVPlayerInterface() {}
    virtual ~RTTVPlayerInterface() {}

    /* parameter operations */
    virtual rt_status setDataSource(IMediaDataSource *ptr) = 0;

    /* control operations */
    virtual rt_status prepare() = 0;
    virtual rt_status prepareAsync() = 0;;
    virtual rt_status seek() = 0;
    virtual rt_status start() = 0;
    virtual rt_status stop() = 0;
    virtual rt_status pause() = 0;
    virtual rt_status reset() = 0;

    /* parameter query operations */
    virtual rt_status getState() = 0;
    virtual rt_status getCurrentPosition(INT64 *usec) = 0;
    virtual rt_status invoke(RtMetaData* request, RtMetaData *reply) = 0;

    virtual rt_status setVideoSink(const void *videoSink) = 0;
    virtual rt_status setAudioSink(const void *audioSink) = 0;
    virtual rt_status setListener(RTPlayerListener *listener) = 0;
};

#ifdef  __cplusplus
}
#endif

#endif  // INCLUDE_RT_PLAYER_RTTVPLAYERINTERFACE_H_
