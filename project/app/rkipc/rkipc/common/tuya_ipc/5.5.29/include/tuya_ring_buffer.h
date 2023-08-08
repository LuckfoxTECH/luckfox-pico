
#ifndef _TUYA_RING_BUFFER_
#define _TUYA_RING_BUFFER_

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"
#if ENABLE_MONITOR_WORK
#include "ty_monitor.h"
#define RING_BUF_CHECK_PERIOD 2
#define RING_BUF_CHECK_ERR_LIMIT 20
#endif
typedef enum {
	E_RBUF_READ,
	E_RBUF_WRITE,
} RBUF_OPEN_TYPE_E;

typedef VOID (*FUNC_REQUEST_KEY_FRAME)(INT_T device, INT_T channel, IPC_STREAM_E stream);

typedef VOID *(*RING_BUFFER_MALLOC_FUNC)(SIZE_T size);
typedef VOID (*RING_BUFFER_FREE_FUNC)(VOID *ptr);

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
	UINT_T seqSync;
} Ring_Buffer_Node_S;

typedef struct {
	UINT_T bitrate;
	UINT_T fps;
	UINT_T max_buffer_seconds;
	FUNC_REQUEST_KEY_FRAME requestKeyFrameCB;
} Ring_Buffer_Init_Param_S;

typedef void *Ring_Buffer_User_Handle_S;

/* initialize one ring buffer for one channel(one device)
device: ring buffer device num, multipul device for one
bitrate: bitrate in Kbps
fps: framerate pre second
max_buffer_seconds: should be more than 1 GOP and less than 10 sencond. Set to 0 as default(10s).
requestIframeCB: call back function to request one I frame from video decoder. set to NULL if not
needed or for NON-video channel.
*/
OPERATE_RET tuya_ipc_ring_buffer_init(INT_T device, INT_T channel, IPC_STREAM_E stream,
                                      Ring_Buffer_Init_Param_S *pparam);

/* uninitialize one ring buffer for one channel(one device)
device: ring buffer device num, multipul device for one
*/
OPERATE_RET tuya_ipc_ring_buffer_uninit(INT_T device, INT_T channel, IPC_STREAM_E stream);

/* set the max vidoe afterwards of the first frame got by tuya_ipc_ring_buffer_get_video_frame,
 * default 3s */
OPERATE_RET tuya_ipc_ring_buffer_set_max_video_afterwards(UINT_T afterwards_sec);

#if 0
/* destoray a ring buffer*/
OPERATE_RET tuya_ipc_ring_buffer_destory(IPC_STREAM_E device);
#endif

int tuya_ipc_ring_buffer_get_video_num_skill(INT_T device, INT_T channel);
/* open a new session for read/write
 */
Ring_Buffer_User_Handle_S tuya_ipc_ring_buffer_open(INT_T device, INT_T channel,
                                                    IPC_STREAM_E stream,
                                                    RBUF_OPEN_TYPE_E open_type);
/* close session
 */
OPERATE_RET tuya_ipc_ring_buffer_close(Ring_Buffer_User_Handle_S handle);
/* append new frame into a ring buffer
 */
OPERATE_RET tuya_ipc_ring_buffer_append_data(Ring_Buffer_User_Handle_S handle, UCHAR_T *addr,
                                             UINT_T size, MEDIA_FRAME_TYPE_E type, UINT64_T pts);

OPERATE_RET tuya_ipc_ring_buffer_append_data_with_timestamp(Ring_Buffer_User_Handle_S handle,
                                                            UCHAR_T *addr, UINT_T size,
                                                            MEDIA_FRAME_TYPE_E type, UINT64_T pts,
                                                            UINT64_T timestamp);

/* append extra data to current head node of ring buffer
this API is for the situation:
different format of the same channel frame needed to be buffered, both PCM&AAC for the same audio
frame e.g. data will be stored in extraData/extraSize field. NOTE:  MUST be called after
tuya_ipc_ring_buffer_append_data
*/
OPERATE_RET tuya_ipc_ring_buffer_append_extra_data(Ring_Buffer_User_Handle_S handle,
                                                   UCHAR_T *dataAddr, UINT_T dataSize);

/* get video/audio frame from ring buffer.
different user of the same channel device can be independently mantain status with different
userIndex, and automaticlly get the next frame in the buffer if it's invoked continuously. NULL will
return if no more new frame exist inside ringbuffer. API will jump to the newest I frame or audio
frame is this GET is delayed too much time.
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_frame(Ring_Buffer_User_Handle_S handle,
                                                   BOOL_T isRetry);

Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_av_frame(Ring_Buffer_User_Handle_S v_handle,
                                                      Ring_Buffer_User_Handle_S a_handle,
                                                      BOOL_T isRetry);

/* start with the newest frame, find previous frame by count frameNum.
for video channel(device), it will keep find previous frame until it's I frame.
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_find_pre_by_frame(Ring_Buffer_User_Handle_S handle,
                                                           UINT_T frameNum);

/* start with the newest frame, find previous frame by count frameNum, and update&anchor user(of
userIndex) to this frame node. for video channel(device), it will keep find previous frame until
it's I frame.
*/
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_pre_frame(Ring_Buffer_User_Handle_S handle,
                                                       UINT_T frameNum);

/* anchor user of userIndex to specified node.
normally this API is called after tuya_ipc_ring_buffer_find_pre_video_by_frame
*/
VOID tuya_ipc_ring_buffer_anchor_user_to_node(Ring_Buffer_User_Handle_S handle,
                                              Ring_Buffer_Node_S *node);

/* start from current frame node of user of userIndex, get the next target frame with right type,
 * and it's sequence number is greater than or equal to seqNo
 */
Ring_Buffer_Node_S *tuya_ipc_ring_buffer_get_next_target_frame(Ring_Buffer_User_Handle_S handle,
                                                               UINT_T seqNo,
                                                               MEDIA_FRAME_TYPE_E type);

/* clean the state of user of userIndex, which means for next time
 * tuya_ipc_ring_buffer_get_video_frame will start from the newest frame node
 */
VOID tuya_ipc_ring_buffer_clean_user_state(Ring_Buffer_User_Handle_S handle);

/* clean the state of user of userIndex and data buffer, which means
   tuya_ipc_ring_buffer_get_pre_video_frame cann't get data earlier than the time this func called,
   for next time tuya_ipc_ring_buffer_get_video_frame will start from the newest frame node
*/
VOID tuya_ipc_ring_buffer_clean_user_state_and_buffer(Ring_Buffer_User_Handle_S handle);

/* check if ring buffer user is delayed too much
 */
BOOL_T tuya_ipc_ring_buffer_if_delay(Ring_Buffer_User_Handle_S handle);

/*
   get ring buffer max buf seconds
*/
INT_T tuya_ipc_ring_buffer_get_max_buf_secs(int device, int channel, IPC_STREAM_E stream);

/* set max frame size of device
 */
INT_T tuya_ipc_ring_buffer_set_max_frame_size(INT_T device, INT_T channel, IPC_STREAM_E stream,
                                              UINT_T maxFrameSize);

/* get max frame size, return main channel size
 */
INT_T tuya_ipc_ring_buffer_get_max_frame_size(INT_T device, INT_T channel, IPC_STREAM_E stream);
/*
    synchronize specified streams
*/
OPERATE_RET tuya_ipc_ring_buffer_sync_stream(Ring_Buffer_User_Handle_S handle_video,
                                             Ring_Buffer_User_Handle_S handle_audio);

/*
    set pre-record frame position, and then just get frame
    must confirm return OPRT_OK
*/
OPERATE_RET tuya_ipc_ring_buffer_anchor_user(Ring_Buffer_User_Handle_S handle, UINT_T frameNum,
                                             BOOL_T checkOverlap);

OPERATE_RET tuya_ipc_ring_buffer_hook_mem(RING_BUFFER_MALLOC_FUNC malloc_func,
                                          RING_BUFFER_FREE_FUNC free_func);

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_RING_BUFFER_*/
