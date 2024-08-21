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
 */

#ifndef INCLUDE_RT_PLAYER_RTSURFACEINTERFACE_H_
#define INCLUDE_RT_PLAYER_RTSURFACEINTERFACE_H_

#include <sys/types.h>
#include <inttypes.h>
#include "rt_type.h"   // NOLINT

typedef struct RT_NATIVEWINDOW_BUFFER_INFO {
    INT32           dupFd;
    INT32           name;   // this name can be used by other process
    INT32           size;
    unsigned long   phyAddr;  // NOLINT
    void           *graphicBuffer;
    void           *window;
    void           *windowBuf;
} RTNativeWindowBufferInfo;

typedef struct RT_SIDEBAND_INFO {
    INT32 structSize;
    INT32 structVersion;
    INT32 left;
    INT32 top;
    INT32 right;
    INT32 bottom;
    INT32 usage;
    INT32 width;
    INT32 height;
    INT32 format;
    INT32 dataSpace;
    INT32 transform;
} RTSidebandInfo;

class RTSurfaceInterface {
 public:
    virtual ~RTSurfaceInterface() {}

    virtual INT32 connect(INT32 mode) = 0;
    virtual INT32 disconnect(INT32 mode) = 0;

    virtual INT32 allocateBuffer(RTNativeWindowBufferInfo *info) = 0;
    virtual INT32 freeBuffer(void *buf, INT32 fence) = 0;
    virtual INT32 remainBuffer(void *buf, INT32 fence) = 0;
    virtual INT32 queueBuffer(void *buf, INT32 fence) = 0;
    virtual INT32 dequeueBuffer(void **buf) = 0;
    virtual INT32 dequeueBufferAndWait(RTNativeWindowBufferInfo *info) = 0;
    virtual INT32 mmapBuffer(RTNativeWindowBufferInfo *info, void **ptr) = 0;
    virtual INT32 munmapBuffer(void **ptr, INT32 size, void *buf) = 0;

    virtual INT32 setCrop(INT32 left, INT32 top, INT32 right, INT32 bottom) = 0;
    virtual INT32 setUsage(INT32 usage) = 0;
    virtual INT32 setScalingMode(INT32 mode) = 0;
    virtual INT32 setDataSpace(INT32 dataSpace) = 0;
    virtual INT32 setTransform(INT32 transform) = 0;
    virtual INT32 setSwapInterval(INT32 interval) = 0;
    virtual INT32 setBufferCount(INT32 bufferCount) = 0;
    virtual INT32 setBufferGeometry(INT32 width, INT32 height, INT32 format) = 0;
    virtual INT32 setSidebandStream(RTSidebandInfo info) = 0;

    virtual INT32 query(INT32 cmd, INT32 *param) = 0;
    virtual void* getNativeWindow() = 0;
};


#endif  // INCLUDE_RT_PLAYER_RTSURFACEINTERFACE_H_

