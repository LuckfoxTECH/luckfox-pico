#ifndef __RKDEMUXER_H__
#define __RKDEMUXER_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define DEMUXER_VIDEO_YUV420SP_10BIT 0
#define DEMUXER_VIDEO_YUV420SP 1
#define DEMUXER_VIDEO_YUV400 2
#define DEMUXER_VIDEO_YUV422 3
#define DEMUXER_VIDEO_YUV444 4

typedef struct _RKDEMUXER_READ_PACKET_CALLBACK_S {
    void (*read_video_packet)(void *);
    void (*read_audio_packet)(void *);
} RKDEMUXER_READ_PACKET_CALLBACK_S;

typedef struct StDemuxerInput{
    void    *ptr;
    int8_t   s8ReadModeFlag;
    int8_t   s8VideoEnableFlag;
    int8_t   s8AudioEnableFlag;
    const char *transport;
    int rtsp_io_timeout;
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
int rkdemuxer_read_packet_start(void *demuxer_cfg, int64_t startPts);
int rkdemuxer_read_packet_stop(void *demuxer_cfg);
int rkdemuxer_read_one_video_packet(void *demuxer_cfg, DemuxerPacket *output_packet);
int rkdemuxer_read_one_audio_packet(void *demuxer_cfg, DemuxerPacket *output_packet);
int rkdemuxer_read_video_duration(void *demuxer_cfg, int64_t *duration);
int rkdemuxer_read_audio_duration(void *demuxer_cfg, int64_t *duration);
int rkdemuxer_seek_video_pts(void *demuxer_cfg, int64_t seekPts);
int rkdemuxer_seek_audio_pts(void *demuxer_cfg, int64_t seekPts);

#ifdef __cplusplus
}
#endif
#endif
