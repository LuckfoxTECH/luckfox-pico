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
 *     author: <hh@rock-chips.com>
 *       date: 2020-06-02
 */

#ifndef SRC_RT_TASK_APP_GRAPH_RTUACGRAPH_H_
#define SRC_RT_TASK_APP_GRAPH_RTUACGRAPH_H_

#include "rt_header.h"
#include "RTTaskGraph.h"

class RTUACGraph : public RTTaskGraph {
 public:
    explicit RTUACGraph(const char* tagName);
    virtual ~RTUACGraph();
};

#endif  // SRC_RT_TASK_APP_GRAPH_RTUACGRAPH_H_

