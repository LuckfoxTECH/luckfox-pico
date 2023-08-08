#ifndef __TUYA_IPC_CHROMECAST_H__
#define __TUYA_IPC_CHROMECAST_H__

#include <stdint.h>

#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum TUYA_STREAM_TYPE_ {
	TUYA_STREAM_TYPE_AUDIO,
	TUYA_STREAM_TYPE_VIDEO,
} TUYA_STREAM_TYPE_E;

typedef struct TUYA_STREAM_FRAME_ {
	uint64_t utcms;          ///< utc time is ms
	uint64_t pts;            ///< pts time
	uint32_t seq;            ///< sequence num of the frame
	TUYA_STREAM_TYPE_E type; ///< frame type
	TUYA_CODEC_ID codec;
	int32_t length; ///< frame length
	void *pbuff;    ///< frame data buffer
} TUYA_STREAM_FRAME_S;

typedef INT_T (*TUYA_CHROMECAST_START)(PVOID_T context, PVOID_T priv_data);
typedef INT_T (*TUYA_CHROMECAST_STOP)(PVOID_T context, PVOID_T priv_data);
typedef INT_T (*TUYA_CHROMECAST_GET_FRAME)(PVOID_T context, TUYA_STREAM_TYPE_E type,
                                           TUYA_STREAM_FRAME_S *pframe);
typedef INT_T (*TUYA_CHROMECAST_GET_FRAME_RELEASE)(PVOID_T context, TUYA_STREAM_TYPE_E type,
                                                   TUYA_STREAM_FRAME_S *pframe);

typedef struct {
	PVOID_T pcontext;
	TUYA_CHROMECAST_START start;
	TUYA_CHROMECAST_STOP stop;
	TUYA_CHROMECAST_GET_FRAME get_frame;
	TUYA_CHROMECAST_GET_FRAME_RELEASE get_frame_release;
} TUYA_CHROMECAST_CALLBACK;

typedef enum TUYA_STREAM_SOURCE_ {
	TUYA_STREAM_SOURCE_RINGBUFFER,
	TUYA_STREAM_SOURCE_CUSTOMER,
} TUYA_STREAM_SOURCE_E;

typedef enum TUYA_STREAM_TRANSMIT_MODE_ {
	TUYA_STREAM_TRANSMIT_MODE_ASYNC,
	TUYA_STREAM_TRANSMIT_MODE_SYNC,
} TUYA_STREAM_TRANSMIT_MODE_E;

typedef struct {
	CHANNEL_E audio_channel;
	CHANNEL_E video_channel;
	TUYA_STREAM_SOURCE_E src;
	TUYA_STREAM_TRANSMIT_MODE_E mode;
	IPC_MEDIA_INFO_S *pminfo;
	TUYA_CHROMECAST_CALLBACK cbk;
} TUYA_CHROMECAST_PARAM_S;

OPERATE_RET tuya_ipc_chromecast_init(TUYA_CHROMECAST_PARAM_S *param);

OPERATE_RET tuya_ipc_chromecast_deinit(VOID);

OPERATE_RET tuya_ipc_chromecast_stop();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
