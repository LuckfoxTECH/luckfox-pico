/*
 * Copyright (c) 2022 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef __RKDEMUXER_H__
#define __RKDEMUXER_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define DEMUXER_VIDEO_YUV420SP_10BIT 0
#define DEMUXER_VIDEO_YUV420SP 1

typedef struct _RKDEMUXER_READ_PACKET_CALLBACK_S {
    void (*read_video_packet)(void *);
    void (*read_audio_packet)(void *);
} RKDEMUXER_READ_PACKET_CALLBACK_S;

typedef struct StDemuxerInput{
    void    *ptr;
    int8_t   s8ReadModeFlag;
    int8_t   s8VideoEnableFlag;
    int8_t   s8AudioEnableFlag;
} DemuxerInput;

typedef struct StDemuxerParam{
    int32_t  s32TotalTime;
    int8_t  *pVideoCodec;
    int32_t  s32VideoWidth;
    int32_t  s32VideoHeigh;
    int8_t   s8VideoFormat;
    int32_t  s32VideoAvgFrameRate;
    int32_t  s32VideoTimeBaseNum;
    int32_t  s32VideoTimeBaseDen;
    int64_t  s64VideoFirstPTS;
    int8_t  *pAudioCodec;
    int32_t  s32AudioChannels;
    int32_t  s32AudioSampleRate;
    int8_t   s8AudioFormat;
    int64_t  s64AudioFirstPTS;
    int32_t  s32AudioTimeBaseNum;
    int32_t  s32AudioTimeBaseDen;
    RKDEMUXER_READ_PACKET_CALLBACK_S pstReadPacketCallback;
} DemuxerParam;

typedef struct StDemuxerPacket{
    void    *ptr;
    int8_t   s8EofFlag;
    int8_t   s8SpecialFlag;
    int8_t  *s8PacketData;
    int32_t  s32PacketSize;
    int32_t  s32Series;
    int64_t  s64Pts;
    int64_t  s64Duration;
    int64_t  s64Pos;
} DemuxerPacket;

int rkdemuxer_init(void **demuxer_cfg, DemuxerInput *ptr);
void rkdemuxer_deinit(void **demuxer_cfg);
int rkdemuxer_get_param(void *demuxer_cfg, const char *input_name, DemuxerParam *ptr);
int rkdemuxer_read_packet_start(void *demuxer_cfg);
int rkdemuxer_read_packet_stop(void *demuxer_cfg);
int rkdemuxer_read_one_video_packet(void *demuxer_cfg, DemuxerPacket *output_packet);
int rkdemuxer_read_one_audio_packet(void *demuxer_cfg, DemuxerPacket *output_packet);

#ifdef __cplusplus
}
#endif
#endif
