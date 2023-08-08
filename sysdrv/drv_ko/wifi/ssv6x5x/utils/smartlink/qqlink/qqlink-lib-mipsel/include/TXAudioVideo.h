/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TX_AUDIO_VIDEO_H__
#define __TX_AUDIO_VIDEO_H__ 
#include "TXSDKCommonDef.h"
CXX_EXTERN_BEGIN
typedef struct _tx_audio_encode_param
{
 unsigned char head_length;
 unsigned char audio_format;
 unsigned char encode_param;
 unsigned char frame_per_pkg;
 unsigned int sampling_info;
 unsigned int reserved;
} tx_audio_encode_param;
#define GET_SIMPLING_INFO(channel,sampling,bit) ((channel << 24) | (sampling << 16) | (bit << 8) | 0x00)
typedef struct _tx_av_callback
{
    bool (*on_start_camera)();
    bool (*on_stop_camera)();
    bool (*on_set_bitrate)(int bit_rate);
    bool (*on_start_mic)();
    bool (*on_stop_mic)();
    void (*on_recv_audiodata)(tx_audio_encode_param *param, unsigned char *pcEncData, int nEncDataLen);
} tx_av_callback;
SDK_API int tx_start_av_service( tx_av_callback *callback);
SDK_API int tx_stop_av_service();
SDK_API void tx_set_video_data(unsigned char *pcEncData, int nEncDataLen,
        int nFrameType, int nTimeStamps, int nGopIndex, int nFrameIndex, int nTotalIndex, int nAvgQP);
SDK_API void tx_set_audio_data(tx_audio_encode_param *param, unsigned char *pcEncData, int nEncDataLen);
CXX_EXTERN_END
#endif
