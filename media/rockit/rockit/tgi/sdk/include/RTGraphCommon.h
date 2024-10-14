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
 * author: modified by <rimon.xu@rock-chips.com>
 *   date: 2020-12-12
 */

#ifndef SRC_RT_TASK_TASK_GRAPH_COMMON_H_
#define SRC_RT_TASK_TASK_GRAPH_COMMON_H_

typedef INT64  RTCBHandle;
typedef INT64  RTLinkHandle;

#define RT_INVALID_CB_HANDLE    -1
#define RT_INVALID_LINK_HANDLE  -1
#define RT_INVALID_STREAM_ID    -1
#define RT_INVALID_NODE_ID      -1

typedef enum GraphIOStreamMode {
    // Blocks and waits until none of the affected streams
    // are full. Note that if max_queue_size is set to -1, the packet will be
    // added regardless of queue size.
    WAIT_TILL_NOT_FULL,
    // Returns and does not add packet if any affected input
    // stream is full.
    ADD_IF_NOT_FULL,
    // drop packet when streams are full
    DROP_IF_FULL,
} RTGraphIOStreamMode;


#endif  // SRC_RT_TASK_TASK_GRAPH_COMMON_H_
