
#ifndef _TUYA_RING_BUFFER_
#define _TUYA_RING_BUFFER_

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"

typedef VOID (*FUNC_REQUEST_I_FRAME)(INT_T channel);

typedef enum {
	E_USER_STREAM_STORAGE = 0,
	E_USER_COULD_STORAGE = 1,
	E_USER_ECHO_SHOW = 2,
	E_USER_CHROMECAST = 3,
	E_USER_DOORBELL = 4,
	E_USER_P2P_USER = 5, // for multi P2P user, can be in format E_USER_P2P_USER+p2p_index
	E_USER_RTSP = 10,
	E_USER_TSTREAMER = 15,
	E_USER_NUM_MAX = 16
} USER_INDEX_E;

typedef struct {
	UINT_T index;
	MEDIA_FRAME_TYPE_E type;
	UCHAR_T *rawData;
	UINT_T size;
	UINT64_T pts;
	UINT64_T timestamp;
	UINT_T seqNo;
	UCHAR_T *extraData;
	UINT_T extraSize;
} Ring_Buffer_Node_S;

/* initialize one ring buffer for one stream(one channel)
channel: ring buffer channel num, multipul channel for one
bitrate: bitrate in Kbps
fps: framerate pre second
max_buffer_seconds: should be more than 1 GOP and less than 10 sencond. Set to 0 as default(10s).
requestIframeCB: call back function to request one I frame from video decoder. set to NULL if not
needed or for NON-video stream.
*/
OPERATE_RET tuya_ipc_ring_buffer_init(CHANNEL_E channel, UINT_T bitrate, UINT_T fps,
                                      UINT_T max_buffer_seconds,
                                      FUNC_REQUEST_I_FRAME requestIframeCB);

#if 0
/* destoray a ring buffer*/
OPERATE_RET tuya_ipc_ring_buffer_destory(CHANNEL_E channel);
#endif
/* append new frame into a ring buffer
 */
OPERATE_RET tuya_ipc_ring_buffer_append_data(CHANNEL_E channel, UCHAR_T *addr, UINT_T size,
                                             MEDIA_FRAME_TYPE_E type, UINT64_T pts);

/* get video/audio frame from ring buffer.
different user of the same stream channel can be independently mantain status with different
userIndex, and automaticlly get the next frame in the buffer if it's invoked continuously. NULL will
return if no more new frame exist inside ringbuffer. API will jump to the newest I frame or audio
frame is this GET is delayed too much time.
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_video_frame(CHANNEL_E channel, USER_INDEX_E userIndex,
                                                         BOOL_T isRetry);
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_audio_frame(CHANNEL_E channel, USER_INDEX_E userIndex,
                                                         BOOL_T isRetry);

/* start with the newest frame, find previous frame by count frameNum.
for video stream(channel), it will keep find previous frame until it's I frame.
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_find_pre_video_by_frame(CHANNEL_E channel,
                                                                 UINT_T frameNum);
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_find_pre_audio_by_frame(CHANNEL_E channel,
                                                                 UINT_T frameNum);

/* start with the newest frame, find previous frame by count frameNum, and update&anchor user(of
userIndex) to this frame node. for video stream(channel), it will keep find previous frame until
it's I frame.
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_pre_video_frame(CHANNEL_E channel,
                                                             USER_INDEX_E userIndex,
                                                             UINT_T frameNum);
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_pre_audio_frame(CHANNEL_E channel,
                                                             USER_INDEX_E userIndex,
                                                             UINT_T frameNum);

/* anchor user of userIndex to specified node.
normally this API is called after tuya_ipc_ring_buffer_find_pre_video_by_frame
*/
VOID tuya_ipc_ring_buffer_anchor_user_to_node(CHANNEL_E channel, USER_INDEX_E userIndex,
                                              Ring_Buffer_Node_S *node);

/* start from current frame node of user of userIndex, get the next target frame with right type,
 * and it's sequence number is greater than or equal to seqNo
 */
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_next_target_frame(CHANNEL_E channel,
                                                               USER_INDEX_E userIndex, UINT_T seqNo,
                                                               MEDIA_FRAME_TYPE_E type);

/* clean the state of user of userIndex, which means for next time
 * tuya_ipc_ring_buffer_get_video_frame will start from the newest frame node
 */
VOID tuya_ipc_ring_buffer_clean_user_state(CHANNEL_E channel, USER_INDEX_E userIndex);

/* check if ring buffer user is delayed too much
 */
BOOL_T tuya_ipc_ring_buffer_if_delay(CHANNEL_E channel, USER_INDEX_E userIndex);

/*
   get ring buffer max buf seconds
*/
INT_T tuya_ipc_ring_buffer_get_max_buf_secs();

/* set max frame size of channel
 */
INT_T tuya_ipc_ring_buffer_set_max_frame_size(CHANNEL_E channel, UINT_T maxFrameSize);

/* get max frame size, return main stream size
 */
INT_T tuya_ipc_ring_buffer_get_max_frame_size();

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_RING_BUFFER_*/
