/*
 * Copyright 2018 Rockchip Electronics Co. LTD
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
 * author: Hery.Xu@rock-chips.com
 *   date: 20181102
 * author: martin.cheng@rock-chips.com
 *   date: 20181126
 */

#ifndef SRC_RT_TASK_INCLUDE_RT_MESSAGE_H_
#define SRC_RT_TASK_INCLUDE_RT_MESSAGE_H_

#include "rt_header.h" // NOLINT
#include "rt_thread.h" // NOLINT

#ifdef __cplusplus
extern "C" {
#endif

// events from integration(player) layer.
enum RTMediaEvent {
    // mediaplayer msg
    RT_MEDIA_NOP               = 0,  // interface test message
    RT_MEDIA_PREPARED          = 1,
    RT_MEDIA_PLAYBACK_COMPLETE = 2,
    RT_MEDIA_BUFFERING_UPDATE  = 3,
    RT_MEDIA_SEEK_COMPLETE     = 4,
    RT_MEDIA_SET_VIDEO_SIZE    = 5,
    RT_MEDIA_STARTED           = 6,
    RT_MEDIA_PAUSED            = 7,
    RT_MEDIA_STOPPED           = 8,
    RT_MEDIA_SKIPPED           = 9,
    RT_MEDIA_TIMED_TEXT        = 99,
    RT_MEDIA_ERROR             = 100,
    RT_MEDIA_INFO              = 200,
    RT_MEDIA_SUBTITLE_DATA     = 201,
    RT_MEDIA_SEEK_ASYNC        = 14,
};

// commands used by integration(player) layer.
enum RTMediaCmd {
    RT_MEDIA_CMD_NOP              = 512,
    RT_MEDIA_CMD_SET_DATASOURCE,
    RT_MEDIA_CMD_PREPARE,
    RT_MEDIA_CMD_SEEKTO,
    RT_MEDIA_CMD_START,
    RT_MEDIA_CMD_STOP,
    RT_MEDIA_CMD_PAUSE,
    RT_MEDIA_CMD_RESET,
    RT_MEDIA_CMD_INVOKE,
    RT_MEDIA_CMD_MAX,
};

// events from node layer.
enum RTNodeEvent {
    RT_NODE_OK = 1024,
    RT_NODE_ERR_CODEC,
    RT_NODE_ERR_CODEC_VIDEO,
    RT_NODE_ERR_CODEC_AUDIO,

    // sub-events start
    RT_NODE_ERR_TINY_BASE = 1128,
    RT_NODE_ERR_CODEC_NONE,
    RT_NODE_ERR_FRAME,
    RT_NODE_ERR_SINK,
    RT_NODE_ERR_TINY_MAX,

    RT_NODE_ERR_FATAL_BASE = 1256,
    RT_NODE_ERR_PIXFORMAT,
    RT_NODE_ERR_CP_VCODEC,
    RT_NODE_ERR_HTTP,
    RT_NODE_ERR_INIT,
    RT_NODE_ERR_IO,
    RT_NODE_ERR_RUNTIME,
    RT_NODE_ERR_FATAL_MAX,
    RT_NODE_ERR_MAX,
    // sub-events end

    RT_NODE_INFO_BASE = 2048,
    RT_NODE_INFO_CACHE,
    RT_NODE_INFO_EOS_VIDEO,
    RT_NODE_INFO_EOS_AUDIO,
    RT_NODE_INFO_FIRST_FRAME,
    RT_NODE_INFO_SYNC,
    RT_NODE_INFO_RESTART_VIDEO,
    RT_NODE_INFO_RESTART_AUDIO,
    RT_NODE_INFO_RESTART_SUBTE,
    RT_NODE_INFO_SET_SPEED,
    RT_NODE_INFO_MAX,
};

enum RTNodeRequest {
    RT_NODE_REQ_BASE = 0,
    RT_NODE_REQ_RETRY_ONCE,
    RT_NODE_REQ_RETRY_NONE,
    RT_NODE_REQ_MAX,
};

typedef struct _rt_media_event {
    UINT32      cmd;
    const char *name;
} rt_media_event;


static const rt_media_event mMediaEvents[] = {
    { RT_MEDIA_NOP,               "EVENT_NOP" },
    { RT_MEDIA_PREPARED,          "EVENT_PREPARED" },
    { RT_MEDIA_PLAYBACK_COMPLETE, "EVENT_COMPLETE" },
    { RT_MEDIA_BUFFERING_UPDATE,  "EVENT_BUFFERING_UPDATE" },
    { RT_MEDIA_SEEK_COMPLETE,     "EVENT_SEEK_COMPLETE" },
    { RT_MEDIA_SET_VIDEO_SIZE,    "EVENT_SET_VIDEO_SIZE" },
    { RT_MEDIA_STARTED,           "EVENT_STARTED" },
    { RT_MEDIA_PAUSED,            "EVENT_PAUSED" },
    { RT_MEDIA_STOPPED,           "EVENT_STOPPED" },
    { RT_MEDIA_SKIPPED,           "EVENT_SKIPPED" },
    { RT_MEDIA_TIMED_TEXT,        "EVENT_TIMED_TEXT" },
    { RT_MEDIA_ERROR,             "EVENT_ERROR" },
    { RT_MEDIA_INFO,              "EVENT_INFO" },
    { RT_MEDIA_SUBTITLE_DATA,     "SUBTITLE_DATA" },
    { RT_MEDIA_SEEK_ASYNC,        "SEEK_ASYNC" },
};

static const rt_media_event mMediaCmds[] = {
    { RT_MEDIA_CMD_NOP,               "NOP" },
    { RT_MEDIA_CMD_SET_DATASOURCE,    "SET_DATASOURCE" },
    { RT_MEDIA_CMD_PREPARE,           "PREPARE" },
    { RT_MEDIA_CMD_SEEKTO,            "SEEKTO" },
    { RT_MEDIA_CMD_START,             "START" },
    { RT_MEDIA_CMD_STOP,              "STOP" },
    { RT_MEDIA_CMD_PAUSE,             "PAUSE" },
    { RT_MEDIA_CMD_RESET,             "RESET" },
    { RT_MEDIA_CMD_INVOKE,            "INVOKE" },
};

struct RTMsgHandler;
struct RTMsgLooper;

struct RTMessage {
    struct RTMsgData {
        INT32   mWhat;
        UINT64  mWhenUs;
        UINT32  mArgU32;
        UINT64  mArgU64;
        RT_PTR  mArgPtr;
        INT32   mResult;
    };

 public:
    RTMessage();
    RTMessage(UINT32 what, UINT32 arg32, UINT64 arg64, struct RTMsgHandler* handler = RT_NULL);
    RTMessage(UINT32 what, RT_PTR data, struct RTMsgHandler* handler = RT_NULL);
    ~RTMessage();

    void           setWhat(UINT32 what);
    const UINT32   getWhat();
    void           setData(RT_PTR data);
    const RT_PTR   getData();
    void           setWhenUs(UINT64 when);
    const UINT64   getWhenUs();
    void           setResult(INT32 result);
    INT32          getResult();
    void           setTarget(struct RTMsgHandler* handler);
    struct RTMsgHandler *getTarget() {
        return mHandler;
    }

    RTMessage*     dup();      // performs message deep copy
    const char*    toString();

    struct RTMsgData mData;

 private:
    void           initialize();
    RT_RET         deliver();

 private:
    friend struct RTMsgLooper;  // deliver()

    struct RTMsgHandler *mHandler;
    RtCondition         *mMsgCond;
    RT_BOOL              mSync;
};

#ifdef __cplusplus
}
#endif

#endif  // SRC_RT_TASK_INCLUDE_RT_MESSAGE_H_
