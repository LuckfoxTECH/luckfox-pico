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
 *     author: <fxw@rock-chips.com>
 *       date: 2020-09-03
 *     module: RTAIGraph
 */

#ifndef SRC_RT_TASK_APP_GRAPH_RTAIGRAPH_H_
#define SRC_RT_TASK_APP_GRAPH_RTAIGRAPH_H_

#include "rt_header.h"
#include "RTTaskGraph.h"

class RTAIGraph : public RTTaskGraph {
 public:
    explicit RTAIGraph(const char* tagName);
    ~RTAIGraph();

    RT_RET prepare();
    RT_RET start();
    RT_RET stop();

    RT_RET preload(RtMetaData *meta);
    RT_RET recognize(RTMediaBuffer *buffer);
    RT_RET observeOutputStream(std::function<RT_RET(RTMediaBuffer *)> streamCallback);

 private:
    RT_RET initialize(const char* tagName);
    RT_RET deinitialize();
};

#endif  // SRC_RT_TASK_APP_GRAPH_RTAIGRAPH_H_
