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
 * src author: <mediapipe-team@google.com>
 * new author: modified by <rimon.xu@rock-chips.com>
 *       date: 2020-06-16
 *  reference: https://github.com/google/mediapipe
 */

#ifndef SRC_RT_TASK_TASK_GRAPH_RTSTREAMINFO_H_
#define SRC_RT_TASK_TASK_GRAPH_RTSTREAMINFO_H_

#include "rt_header.h"

class RTStreamInfo {
 public:
    RTStreamInfo() {
        mStreamType = "none";
        mStreamName = "none";
    }
    ~RTStreamInfo() {}

    void setStreamType(std::string streamType) {
        mStreamType = streamType;
    }
    std::string streamType() {
        return mStreamType;
    }

    void setStreamName(std::string streamName) {
        mStreamName = streamName;
    }
    std::string streamName() {
        return mStreamName;
    }

    void setStreamId(INT32 streamId) {
        mStreamId = streamId;
    }

    INT32 streamId() {
        return mStreamId;
    }

 private:
    std::string mStreamType;
    std::string mStreamName;
    INT32 mStreamId = -1;
};

#endif  // SRC_RT_TASK_TASK_GRAPH_RTSTREAMINFO_H_
