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
 */

#ifndef INCLUDE_RT_PLAYER_RTSUBTITLESINKINTERFACE_H_
#define INCLUDE_RT_PLAYER_RTSUBTITLESINKINTERFACE_H_

#include <sys/types.h>
#include <inttypes.h>
#include "rt_type.h"   // NOLINT

typedef enum _RTSubRenderType {
    RENDER_RGA,
    RENDER_GPU
} RTSubRenderType;

typedef struct _RTSubFrame {
    void* data;
    INT32 x;
    INT32 y;
    INT32 width;
    INT32 height;
    INT32 subWidth;
    INT32 subHeight;
    bool  needcrop;
} RTSubFrame;

class RTSubtitleSinkInterface {
 public:
    virtual ~RTSubtitleSinkInterface() {}

    virtual void        create(INT32 renderType, INT32 display) = 0;
    virtual void        destroy() = 0;
    virtual void        initScene() = 0;
    virtual void        showScene() = 0;    // swap buffer to screen
    virtual void        render(RTSubFrame *frame) = 0;
    virtual void        clean() = 0;
    virtual void        show() = 0;
    virtual void        hide() = 0;
    virtual bool        isShowing() = 0;
};

#endif  // INCLUDE_RT_PLAYER_RTSUBTITLESINKINTERFACE_H_

