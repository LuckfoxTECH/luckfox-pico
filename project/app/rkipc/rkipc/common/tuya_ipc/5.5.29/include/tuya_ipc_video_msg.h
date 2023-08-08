#ifndef __TUYA_IPC_DOORBELL_H__
#define __TUYA_IPC_DOORBELL_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"

#define MAX_MESSAGE_TIME 10
#define MAX_AUDIO_MSG_TIME 15

typedef enum { MSG_BOTH = 0, MSG_VIDEO_ONLY, MSG_AUDIO_ONLY, MSG_INVALID } MESSAGE_E;

/**
 * \fn OPERATE_RET tuya_ipc_video_msg_init
 * \brief initialize video msg leaving, which will malloc all needed memory after this
 * \param[in] media_setting is used to decide needed memroy
 * \param[in] type is the msg type(audio or video)
 * \param[in] msg_duration is the duration of msg(the video longgest duration is 10 seconds,audio:15
 * seconds) \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_video_msg_init(IPC_MEDIA_INFO_S *media_setting, MESSAGE_E type,
                                    INT_T msg_duration);

/**
 * \fn OPERATE_RET tuya_ipc_video_msg_uninit
 * \brief uninit video msg leaving, free used memory
 * \param[]
 * \return
 */
OPERATE_RET tuya_ipc_video_msg_uninit();

/**
 * \fn OPERATE_RET tuya_ipc_leave_video_msg
 * \brief leave a video msg from now.
 * \param[in] extra_data is the buffer of pic
 * \param[in] data_len is the buffer len
 * \return
 */
OPERATE_RET tuya_ipc_leave_video_msg(CHAR_T *extra_data, INT_T data_len);

/**
 * \fn OPERATE_RET tuya_ipc_md_video_msg
 * \brief push a md alarm with video
 * \param[in] extra_data is the buffer of pic
 * \param[in] data_len is the buffer len
 * \return
 */
OPERATE_RET tuya_ipc_md_video_msg(CHAR_T *extra_data, INT_T data_len);

/**
 * \fn BOOL_T tuya_ipc_video_msg_get_status
 * \brief get video msg status
 * \if status is busy return true or return false;
 */
BOOL_T tuya_ipc_video_msg_status_is_busy();

#ifdef __cplusplus
}
#endif

#endif
