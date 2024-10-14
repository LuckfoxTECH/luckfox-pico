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
 *       date: 2020-03-23
 *  reference: https://github.com/google/mediapipe
 */

#ifndef SRC_RT_TASK_TASK_GRAPH_RTTASKNODECONTEXTMANAGER_H_
#define SRC_RT_TASK_TASK_GRAPH_RTTASKNODECONTEXTMANAGER_H_

#include <memory>
#include <vector>

#include "rt_header.h"
#include "rt_metadata.h"
#include "RTStreamInfo.h"
#include "RTTaskNodeContext.h"

class RTTaskNodeContextManager {
 public:
    RTTaskNodeContextManager() {}
    virtual ~RTTaskNodeContextManager() {}

    RT_RET initialize(
        std::string nodeName,
        INT32 nodeId,
        std::vector<RTStreamInfo *> *inputInfo,
        std::vector<RTStreamInfo *> *outputInfo,
        bool isParallelRunning);

    RT_RET prepareForRun(RtMetaData *options);
    RT_RET cleanupAfterRun();

    RTTaskNodeContext *getDefaultNodeContext() const;

 private:
    std::unique_ptr<RTTaskNodeContext> mDefaultContext;
    std::vector<RTStreamInfo *> *mInputInfos;
    std::vector<RTStreamInfo *> *mOutputInfos;
    bool        mParallelRunning;
    RtMetaData* mOptions;
    std::string mNodeName;
    INT32       mNodeId;
};

#endif  // SRC_RT_TASK_TASK_GRAPH_RTTASKNODECONTEXTMANAGER_H_

