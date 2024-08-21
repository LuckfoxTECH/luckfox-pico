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
 */

#ifndef SRC_RT_TASK_TASK_GRAPH_RTTASKNODECONTEXT_H_
#define SRC_RT_TASK_TASK_GRAPH_RTTASKNODECONTEXT_H_

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <map>

#include "rt_header.h"
#include "rt_metadata.h"
#include "RTStreamInfo.h"

class RTMediaBuffer;
class RTBufferListener;
class RTOutputStreamShared;
class RTMediaBufferPool;
class RTTaskNodeStat;
class RTTaskNodeContext {
 public:
    explicit RTTaskNodeContext(
            std::string nodeName,
            INT32 nodeId,
            std::vector<RTStreamInfo *> *inputInfos,
            std::vector<RTStreamInfo *> *outputInfos);
    ~RTTaskNodeContext();

    RTTaskNodeContext(const RTTaskNodeContext&) = delete;
    RTTaskNodeContext& operator=(const RTTaskNodeContext&) = delete;

    RT_RET              prepareForRun(RtMetaData* options);
    RT_RET              cleanupAfterRun();

    const std::string&  nodeName() const { return mNodeName; }
    INT32               nodeId() const { return mNodeId; }
    void                suspend() { mSuspend = true; }
    void                resume() { mSuspend = false; }
    RT_RET              flush();
    bool                isSuspend() { return mSuspend; }
    RtMetaData*         options() { return mOptions; }
    void                sendInterrupt(std::string reason);
    void                cancelInterrupt(std::string reason);
    void                setOutputBufferListener(RTBufferListener *listener);
    void                setMaxBatchPrcoessSize(INT32 maxBatchSize);
    INT32               getMaxBatchPrcoessSize();

    RTStreamInfo*       getInputInfo(std::string streamType = "none");
    RTStreamInfo*       getOutputInfo(std::string streamType = "none");

    RT_RET              getPackets(std::list<RTMediaBuffer *> *packets, std::string streamType = "none");

    RT_BOOL             hasInputStream(std::string streamType = "none");
    RT_BOOL             hasOutputStream(std::string streamType = "none");
    INT32               inputQueueSize(std::string streamType = "none");
    RT_BOOL             inputIsEmpty(std::string streamType = "none");
    RT_BOOL             outputIsEmpty(std::string streamType = "none");

    RTMediaBuffer*      inputHeadBuffer(std::string streamType = "none");
    RTMediaBuffer*      dequeInputBuffer(std::string streamType = "none");
    RT_RET              queueInputBuffer(RTMediaBuffer *packet, std::string streamTpye = "none");
    RTMediaBuffer*      dequeOutputBuffer(RT_BOOL block = RT_TRUE,
                                           UINT32 size = 0,
                                           std::string streamType = "none");
    RT_RET              queueOutputBuffer(RTMediaBuffer *packet, std::string streamType = "none");
    RT_RET              getBufferStat(RTTaskNodeStat *stat);

    RT_RET              reallocOutputBuffers(INT32 wantSize, std::string streamType = "none");

    RT_RET              attachOutStreamPool(RTMediaBufferPool *pool, std::string streamType = "none");
    RT_RET              detachOutStreamPool(std::string streamType = "none");
    RT_RET              pollOutputBuffer(RT_BOOL block = RT_TRUE,
                                          UINT32 size = 0,
                                          std::string streamType = "none");
    INT32               getOutputBufferSize(std::string streamType = "none");

    RT_RET              dump();

 private:
    std::vector<RTMediaBuffer *>*   inputs(std::string streamType = "none");
    RTOutputStreamShared*           outputs(std::string streamType = "none");

 private:
    std::map<std::string, std::vector<RTMediaBuffer *>> mInputs;
    std::map<std::string, RTOutputStreamShared *>       mOutputs;
    RtMetaData                     *mOptions;
    INT32                           mNodeId;
    std::string                     mNodeName;
    bool                            mSuspend = false;
    INT32                           mMaxBatchProcessSize = 4;
    std::vector<RTStreamInfo *>    *mInputInfos;
    std::vector<RTStreamInfo *>    *mOutputInfos;
    RtMutex                         mInputMutex;
};

#endif  // SRC_RT_TASK_TASK_GRAPH_RTTASKNODECONTEXT_H_

