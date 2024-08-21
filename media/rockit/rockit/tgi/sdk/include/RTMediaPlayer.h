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
 * author: martin.cheng@rock-chips.com
 *                  fxw@rock-chips.com
 *   date: 2020-04-27
 * module: RTMediaPlayer
 */

#ifndef SRC_RT_PLAYER_RTMEDIAPLAYER_H_
#define SRC_RT_PLAYER_RTMEDIAPLAYER_H_

#include <sys/types.h>
#include <inttypes.h>
#include "rt_header.h"                  // NOLINT
#include "RTPlayerDef.h"                // NOLINT
#include "RTMediaPlayerInterface.h"     // NOLINT

class  IMediaDataSource;
class  RTPlayerListener;
struct RTMediaPlayerContexts;

class RTMediaPlayer : public RTMediaPlayerInterface {
 public:
    RTMediaPlayer();
    ~RTMediaPlayer();

 public:
    /* parameter operations */
    virtual rt_status setUID(uid_t uid);
    virtual rt_status setDataSource(const char *url, const char *headers);
    virtual rt_status setDataSource(INT32 fd, INT64 offset, INT64 length);
    virtual rt_status setDataSource(IMediaDataSource *source);
    virtual rt_status setLooping(INT32 loop);
    virtual rt_status setListener(RTPlayerListener *listener);

    /* control operations */
    virtual rt_status initCheck();
    virtual rt_status prepare();
    virtual rt_status prepareAsync();
    virtual rt_status seekTo(INT64 usec);
    virtual rt_status start();
    virtual rt_status stop();
    virtual rt_status pause();
    virtual rt_status reset();
    virtual rt_status wait(INT64 timeUs = 0);

    /* parameter query operations */
    virtual rt_status getState();
    virtual rt_status getCurrentPosition(INT64 *usec);
    virtual rt_status getDuration(INT64 *usec);
    virtual rt_status dump(INT32 fd, const char *args);

    /* advanced query/control operations */
    virtual rt_status invoke(RtMetaData *request, RtMetaData *reply);
    virtual rt_status setParameter(INT32 key, RtMetaData *request);
    virtual rt_status getParameter(INT32 key, RtMetaData *reply);

    /* configuration video/audio/subtitle sink */
    virtual rt_status setVideoSink(const void *videoSink);
    virtual rt_status setAudioSink(const void *audioSink);
    virtual rt_status setSubteSink(const void *subteSink);

 public:
    /* extension interface for player, and cautiously modified. */
    rt_status setVolume(float leftVolume, float rightVolume);

 private:
    struct RTMediaPlayerContexts *mPlayerCtx;
};


#ifdef  __cplusplus
extern "C" {
#endif

void *createRockitPlayer();
void  destroyRockitPlayer(void **player);

#ifdef  __cplusplus
}
#endif

#endif  // SRC_RT_PLAYER_RTMEDIAPLAYER_H_
