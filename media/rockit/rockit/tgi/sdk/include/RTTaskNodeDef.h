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
 * author: modified by <martin.cheng@rock-chips.com>
 *   date: 2020-04-23
 *  title: task node stub
 */

#ifndef SRC_RT_TASK_TASK_NODE_RTTASKNODEDEF_H_
#define SRC_RT_TASK_TASK_NODE_RTTASKNODEDEF_H_

#include "RTMediaBufferDef.h"

#define RT_PADDING 1

typedef enum _RTStubUid {
    kStubNone              = 0,
    /* node stubs for media format */
    kStubDemuxerTMedia     = MKTAG('f', 'm', 'p', 'g'),
    kStubDemuxerFile       = MKTAG('f', 'f', 'i', 'l'),

    /* node stubs for media codec */
    kStubCodecMppDecoder     = MKTAG('c', 'm', 'd', 'c'),
    kStubCodecMppEncoder     = MKTAG('c', 'm', 'e', 'e'),
    kStubCodecAudioDecoder   = MKTAG('c', 'a', 'o', 'd'),
    kStubCodecAudioEncoder   = MKTAG('c', 'a', 'o', 'e'),

    /* node stubs for media sink */
    kStubSinkVideo         = MKTAG('s', 'v', 'i', 'd'),
    kStubSinkAudio         = MKTAG('s', 'v', 'i', 'l'),
    kStubSinkFile          = MKTAG('s', 'f', 'i', 'l'),
    kStubLinkOutput        = MKTAG('l', 'k', 'o', 'p'),

    /* node stubs for media filter */
    kStubFilterRKRga       = MKTAG('f', 'r', 'g', 'a'),
    kStubFilterRKNpu       = MKTAG('f', 'n', 'p', 'u'),
    kStubFilterImage       = MKTAG('f', 'i', 'm', 'g'),
    kStubFilterScale       = MKTAG('f', 's', 'c', 'a'),
    kStubFilterRockx       = MKTAG('f', 'r', 'k', 'x'),
    kStubFilterResample    = MKTAG('f', 'a', 'r', 's'),
    kStubFilterStAsteria   = MKTAG('f', 's', 'a', 't'),
    kStubFilterVolume      = MKTAG('f', 'a', 'v', 'l'),
    kStubFilterEptz        = MKTAG('e', 'p', 't', 'z'),
    kStubfilterAudio3A     = MKTAG('f', 'a', 'd', 'a'),
    kStubfilterAnr         = MKTAG('f', 'a', 'n', 'r'),
    kStubfilterGLPss       = MKTAG('g', 'l', 'p', 's'),
    kStubfilterVpss        = MKTAG('v', 'p', 's', 's'),
    kStubFilterAvs         = MKTAG('f', 'a', 'v', 's'),
    kStubFilterMpiVo       = MKTAG('f', 'm', 'v', 'o'),
    kStubFilterWbcVo       = MKTAG('f', 'w', 'v', 'o'),

    kStubfilterSKV        = MKTAG('f', 's', 'k', 'v'),
    kStubfilterSKVAec     = MKTAG('f', 's', 'a', 'e'),
    kStubFilterSKVAgc     = MKTAG('f', 'a', 'g', 'c'),
    kStubFilterSKVBF      = MKTAG('f', 's', 'b', 'f'),
    kStubFilterSKVDoa     = MKTAG('f', 's', 'd', 'a'),
    kStubFilterTrackMode  = MKTAG('f', 't', 'k', 'm'),
    kStubFilterAHPF       = MKTAG('f', 'a', 'h', 'p'),
    kStubFilterLHPF       = MKTAG('f', 'a', 'l', 'p'),
    kStubFilterAChnSwap   = MKTAG('f', 'c', 's', 'p'),

    /* node stubs for media device */
    kStubDeviceV4L2        = MKTAG('d', 'v', 'l', 'v'),
    kStubDeviceALSACap     = MKTAG('d', 'a', 'l', 'c'),
    kStubDeviceALSAPlay    = MKTAG('d', 'a', 'l', 'p'),
    kStubDeviceMultiCap    = MKTAG('m', 'l', 't', 'c'),
    kStubDeviceExternal    = MKTAG('d', 'e', 'x', 't'),
    kStubFilterAIMatting   = MKTAG('a', 'i', 'm', 't'),
} RTStubUid;

typedef enum _RTPadType {
    RT_PAD_UNKNOWN,
    RT_PAD_SRC,
    RT_PAD_SINK,
    RT_PAD_LINK,
    RT_PAD_MAX,
} RTPadType;

typedef struct _RTCapPair {
    const char* mKey;
    const char* mValue;
} RTCapPair;

typedef struct _RTPadCaps {
    const char         *mMime;
    RTPadType           mPadType;
    RTMediaBufferType   mBufferType;
    RTCapPair           mCapPairs;
} RTPadCaps;

#endif  // SRC_RT_TASK_TASK_NODE_RTTASKNODEDEF_H_
