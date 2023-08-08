#ifndef __TUYA_IPC_AI_DETECT_STORAGE_H__
#define __TUYA_IPC_AI_DETECT_STORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "tuya_ipc_cloud_storage.h"

/**
 * \fn OPERATE_RET tuya_ipc_ai_detect_storage_init
 * \brief
 * \     Init ai detect function.
 * \     media info is needed, init the buffer storing the pic and video
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ai_detect_storage_init(IPC_MEDIA_INFO_S *media_setting);

/**
 * \fn OPERATE_RET tuya_ipc_ai_detect_storage_exit
 * \brief Exit ai detect function.
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ai_detect_storage_exit();

/**
 * \fn OPERATE_RET tuya_ipc_ai_detect_storage_start
 * \brief
 * \      When the motion started, use this api to start ai detect,
 * \      to check is there any body or face in the pic.
 * \      Don't need to care about the pic to check, this api well get
 * \      the pic by VOID tuya_ipc_get_snapshot_cb(char* pjbuf, int* size),
 * \      so you need to complete the tuya_ipc_get_snapshot_cb function
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ai_detect_storage_start();

/**
 * \fn OPERATE_RET tuya_ipc_ai_detect_storage_stop
 * \brief When the motion stopped, use this api to stop detect
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ai_detect_storage_stop();

/**
 * \fn OPERATE_RET tuya_ipc_ai_detect_storage_pause
 * \brief IN sleep mode or don't want to detect the pic, use this
 * \      api to pause
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ai_detect_storage_pause();

/**
 * \fn OPERATE_RET tuya_ipc_ai_detect_storage_resume
 * \brief use api to resume ai detect
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ai_detect_storage_resume();

#ifdef __cplusplus
}
#endif

#endif
